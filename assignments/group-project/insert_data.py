import os
import xlrd
import MySQLdb

PROJECT_DIR = os.path.dirname(__file__)
LOGIN_FILE_NAME = 'login_info.txt'
SERVER_IP = '127.0.0.1'  # I'm assuming you're running this locally.
DATABASE_NAME = 'itms428'


def has_database(connection: MySQLdb.connection, name: str) -> bool:
    result: MySQLdb.result

    result = connection.query(
        f"""SELECT SCHEMA_NAME
                FROM
            INFORMATION_SCHEMA.SCHEMATA
                WHERE SCHEMA_NAME = '{name}'""")

    result = connection.store_result()

    rows = result.fetch_row()

    if len(rows) is 0:
        return False

    return True


def get_login_creds(path: str) -> (str, str,):
    """Human-proof way to get login info from plaintext file."""

    try:
        file = open(path, 'r')
    except FileNotFoundError as e:
        print(f"Please make {LOGIN_FILE_NAME}, and put your username and password in it, separated by a newline.")
        exit(1)
    except Exception as e:
        print(f"Not sure what went wrong.")
        print(f"Make sure all the paths and locations are right and try again.")
        print(f"Also, send this error to me, please:")
        print(e)

    lines = file.readlines()

    # Fix for blank password.
    if (len(lines) is 1) and ('\n' in lines[0]):
        lines = lines[0].split('\n')

    if len(lines) < 2:
        print(f"There's less than two lines in {LOGIN_FILE_NAME}.")
        print(f"Please make sure you have a username and password in there, separated by a newline.")
        exit(1)

    username, password = (item.strip() for item in lines[0:2])

    return username, password


if __name__ == '__main__':
    username, password = get_login_creds(os.path.join(PROJECT_DIR, LOGIN_FILE_NAME))

    print(f"Using the following credentials from '{LOGIN_FILE_NAME}':")
    print(f"{username}:<PASSWORD>")

    connection = MySQLdb.connect(host=SERVER_IP, user=username, passwd=password)
    print(connection)
    print("If you see some stuff in angle brackets above, IT WORKED! CONGRATULES!")

    # If they don't have the database, create it.
    if not has_database(connection, DATABASE_NAME):
        print(f"DB '{DATABASE_NAME}' does not exist. Creating...")
        connection.query(f"CREATE DATABASE {DATABASE_NAME}")
    else:
        print(f"DB '{DATABASE_NAME}' exists.")
