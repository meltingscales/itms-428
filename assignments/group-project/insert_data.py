import os

import MySQLdb

PROJECT_DIR = os.path.dirname(__file__)
LOGIN_FILE_NAME = 'login_info.txt'
SERVER_IP = '127.0.0.1'  # I'm assuming you're running this locally.
DATABASE_NAME = 'itms428'


class FarmerDatabase(object):
    name = 'farmer_payment'
    data_filepath = os.path.join(PROJECT_DIR, 'data/2008_farmer_payment_data.big.txt')
    delimiter = ';'
    limit = 10000

    @classmethod
    def create_table(cls, connection: MySQLdb.connection):
        connection.query(f'''
        CREATE TABLE {cls.name} (
            id      INTEGER     NOT NULL UNIQUE AUTO_INCREMENT,
            n1      INTEGER     NOT NULL,
            n2      INTEGER     NOT NULL,
            anum    VARCHAR(20) NOT NULL,
            n3      INTEGER     NOT NULL,
            year1   INTEGER     NOT NULL,
            n4      INTEGER     NOT NULL,
            money   DOUBLE      NOT NULL,
            date1   DATE        NOT NULL,
            zone    VARCHAR(20) NOT NULL,
            n5      INTEGER     NOT NULL,
            year2   INTEGER     NOT NULL,
            year3   INTEGER     NOT NULL,
            n6      INTEGER     NOT NULL            
        );
            
        ''')

    @classmethod
    def insert_all_data(cls, connection: MySQLdb.connection):
        file = open(cls.data_filepath, 'r')

        i = 0
        for line in file:
            line = line.strip()

            values = line.split(cls.delimiter)  # Split by delimiter
            values = [value.strip() for value in values]  # Remove whitespace

            for x in [0, 1, 3, 4, 5, 9, 10, 11, 12]:  # These ones are ints.
                values[x] = int(values[x])

            values[6] = float(values[6].replace(',', ''))  # This is the money amount. It may have a comma.
            # print(values)

            formatted_values = []

            for value in values:
                if isinstance(value, str):
                    formatted_values.append(f"'{value}'")
                else:
                    formatted_values.append(str(value))

            insert_statement = f'''INSERT INTO {cls.name} 
            VALUES(NULL, {','.join([str(value) for value in formatted_values])})'''

            connection.autocommit = False
            connection.query(insert_statement)

            if i >= cls.limit:
                print(f"I think {cls.limit} rows for {cls.name} is enough.")
                break

            i += 1

        print(f"Inserted {i} rows into {cls.name}.")

        connection.commit()

        file.close()


def has_database(connection: MySQLdb.connection, name: str) -> bool:
    result: MySQLdb.result

    connection.query(
        f"""SELECT SCHEMA_NAME
                FROM
            INFORMATION_SCHEMA.SCHEMATA
                WHERE SCHEMA_NAME = '{name}'""")

    result = connection.store_result()

    rows = result.fetch_row()

    if len(rows) is 0:
        return False

    return True


def has_table(connection: MySQLdb.connection, name: str) -> bool:
    result: MySQLdb.result

    connection.query(f'''SHOW TABLES LIKE '{name}';''')

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
    print(f'{username}:<PASSWORD>\n')

    connection = MySQLdb.connect(host=SERVER_IP, user=username, passwd=password)
    print(connection)
    print("If you see some stuff in angle brackets above, IT WORKED! CONGRATULES!\n")

    # If they don't have the database, create it.
    if not has_database(connection, DATABASE_NAME):
        print(f"DB '{DATABASE_NAME}' does not exist. Creating...")
        connection.query(f"CREATE DATABASE {DATABASE_NAME}")
    else:
        print(f"DB '{DATABASE_NAME}' exists.")
    print()

    # Use the database.
    connection.query(f'USE {DATABASE_NAME};')

    if not has_table(connection, FarmerDatabase.name):
        print(f"You don't have the {FarmerDatabase.name} table, so we'll make it.")
        FarmerDatabase.create_table(connection)
        FarmerDatabase.insert_all_data(connection)
