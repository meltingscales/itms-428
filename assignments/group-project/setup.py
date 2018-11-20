from data.mysql_sample_db import *
from shared_lib import get_login_creds
from trigger import triggers
from user import ALL_USERS

ALL_TABLES: [GenericData] = [UserTypesDatabase, ProductLinesData, EmployeeData, CustomerData, PaymentsData, OfficesData,
                             OrdersData, ProductsData, OrderDetailsData, TestDatabase, StatsDatabase, UsersDatabase, SecurityTrainingRecordsDatabase]


def has_trigger(connection: MySQLdb.connection, name: str, db: str = Config.DATABASE_NAME) -> bool:
    result: MySQLdb.result

    query = f"""SHOW TRIGGERS FROM {db} WHERE `Trigger` LIKE '{name}';"""

    connection.query(query)

    result = connection.store_result()

    rows = result.fetch_row()

    if len(rows) <= 0:
        return False

    return True


def delete_trigger(connection: MySQLdb.connection, name: str):
    connection.query(f"""DROP TRIGGER {name};""")


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


def has_user(connection: MySQLdb.connection, name: str) -> bool:
    result: MySQLdb.result

    cstr = f"""SELECT User FROM mysql.user WHERE User = "{name}";"""

    connection.query(cstr)

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


def create_tables(connection: MySQLdb.connection):
    if not has_table(connection, FarmerDatabase.table_name):
        print(f"You don't have the {FarmerDatabase.table_name} table, so we'll make it.")
        FarmerDatabase.create_table(connection)
        FarmerDatabase.insert_all_data(connection)
    else:
        print(f"You have the {FarmerDatabase.table_name} table. Not modifying it.")

    for datum in ALL_TABLES:
        if not has_table(connection, datum.table_name):
            print(f"You don't have the {datum.table_name} table, so we'll make it.")

            try:
                connection.query(datum.table_def)
            except Exception as e:
                print(e)
                print(datum.table_def)

            if datum.insert_statements is not None:
                try:
                    connection.query(datum.insert_statements)
                except Exception as e:
                    print(e)
                    print(datum.insert_statements)
        else:
            print(f"You have the {datum.table_name} table. Not modifying it.")


def create_users(connection: MySQLdb.Connection):
    for user in ALL_USERS:

        if not has_user(connection, user.username):
            print(f"Creating user '{user.username}...'")

            qstr = user.create_statement()
            print(qstr)
            connection.query(qstr)

        if has_user(connection, user.username):
            print(f"User '{user.username}' already exists.")

        if len(user.global_privs) > 0:
            print(f"Granting global privileges for '{user.username}'.")
            qstr = user.grant_global_priv()
            print(qstr)
            connection.query(qstr)

        if len(user.table_privs.keys()) > 0:
            print(f"Granting table privileges for '{user.username}'.")

            qstrs = user.grant_table_priv()

            for qstr in qstrs:
                print(qstr)
                connection.query(qstr)

        print()


def create_triggers(connection: MySQLdb.connection):
    for trigger in triggers:

        if not has_trigger(connection, trigger.name):

            query = trigger.generate_trigger()

            print(f"Making trigger '{trigger.name}'.")
            print(query)
            connection.query(query)
        else:
            print(f"Trigger '{trigger.name}' already exists.")


if __name__ == '__main__':
    username, password = get_login_creds(os.path.join(Config.PROJECT_DIR, Config.LOGIN_FILE_NAME))

    print(f"Using the following credentials from '{Config.LOGIN_FILE_NAME}':")
    print(f'{username}:<PASSWORD>\n')

    connection = MySQLdb.connect(host=Config.SERVER_IP, user=username, passwd=password)
    print(connection)
    print("If you see some stuff in angle brackets above, IT WORKED! CONGRATULES!\n")

    # If they don't have the database, create it.
    if not has_database(connection, Config.DATABASE_NAME):
        print(f"DB '{Config.DATABASE_NAME}' does not exist. Creating...")
        connection.query(f"CREATE DATABASE {Config.DATABASE_NAME}")
    else:
        print(f"DB '{Config.DATABASE_NAME}' exists.")
    print()

    # Use the database.
    connection.query(f'USE {Config.DATABASE_NAME};')

    create_tables(connection)

    create_users(connection)

    create_triggers(connection)
