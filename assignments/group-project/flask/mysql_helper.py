import sys

from MySQLdb import Connection

sys.path.append('..')

from data.mysql_sample_db import UsersDatabase


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
