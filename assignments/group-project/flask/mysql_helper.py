import datetime
import sys

from MySQLdb import Connection

sys.path.append('..')

from data.mysql_sample_db import UsersDatabase


def dump_table_to_dict(table_name: str, connection: Connection):
    """Given a table name, return a dictionary that contains all its data.
    Example:

        foo('users_table', <CONNECTION_OBJECT>) -> {
            'data': (
                ('henry','iliketofarm', ...),
                ('reshma','coolestmanager', ...),
            ),
            'header': ('username', 'password', ...),
        }
        """

    data = {
        table_name: {
            'data': (),
            'header': (),
        }
    }

    cursor = connection.cursor()

    cursor.execute(f"SELECT * FROM {table_name};")
    data[table_name]['data'] = cursor.fetchall()

    cursor.execute(f"SHOW columns FROM {table_name};")
    results = cursor.fetchall()  # List of column header infos.
    results = (result[0] for result in results)  # Get first field, the name.
    data[table_name]['header'] = results

    cursor.close()

    return data

def get_current_datetime() -> str:
    """Gives you a mySQL-friendly string that's the current datetime."""
    return datetime.datetime.now().strftime('%y-%m-%d %H:%M:%S.%f')


def update_login_time(username: str, time: str, connection: Connection) -> None:
    cursor = connection.cursor()

    cursor.execute(f"""
    UPDATE 
        {UsersDatabase.table_name}
    SET 
        last_logged_in = TIME('{time}')
    WHERE
        username LIKE '{username}';
    """)

    cursor.close()


def update_login_time_to_now(username: str, connection: Connection) -> None:
    """Small utility function to update login time to 'right now'."""
    update_login_time(username, get_current_datetime(), connection)


def user_exists(username: str, connection: Connection) -> bool:
    cursor = connection.cursor()

    #
    cursor.execute(f"""
    SELECT
        COUNT(*)
    FROM 
        {UsersDatabase.table_name}
    WHERE 
        username LIKE "{username}" """)

    x = cursor.fetchone()

    cursor.close()

    return x[0] > 0


def login_valid(username: str, password: str, connection: Connection) -> bool:
    """ Does this username and password combination exist? """

    cursor = connection.cursor()

    cursor.execute(f"""
        SELECT
            COUNT(*)
        FROM
            {UsersDatabase.table_name}
        WHERE 
            username LIKE "{username}" 
                AND
            password LIKE "{password}";""")

    x = cursor.fetchone()

    cursor.close()

    return x[0] > 0
