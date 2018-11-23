from flask import flash
from flask_wtf import FlaskForm
import datetime
import time
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
    """Gives you a MySQL-friendly string that's the current datetime."""
    return mysql_friendly_datetime(datetime.datetime.now())

def get_current_datetimee() -> str:
    """Gives you a MySQL-friendly string that's the current datetime."""
    return datetime.datetime.now()

def mysql_friendly_datetime(dt: datetime) -> str:
    """Turns a `datetime` object into a MySQL DATETIME format."""
    return dt.strftime('%y-%m-%d %H:%M:%S.%f')


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


def update_freezestart_time_to_now(username: str, connection: Connection) -> None:
    """Small utility function to update login time to 'right now'."""
    update_freezestart_time(username, get_current_datetimee(), connection)
    
def update_freezestart_time(username: str, time: str, connection: Connection) -> None:
    cursorx = connection.cursor()

    cursorx.execute(f"""
    UPDATE 
        {UsersDatabase.table_name}
    SET 
        freezestarttime = TIME('{time}')
    WHERE
        username LIKE '{username}';
    """)

    cursorx.close()

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
            incorrect_logins
        FROM
            {UsersDatabase.table_name}
        WHERE 
            username LIKE "{username}";
            """)
    cnt = int(cursor.fetchone()[0])
    cursor.close()
    cursor1 = connection.cursor()
    if cnt < 3:
        cursor1.execute(f"""
                       SELECT
                       COUNT(*)
                       FROM
                       {UsersDatabase.table_name}
                       WHERE 
                       username LIKE "{username}" 
                       AND
                       password LIKE "{password}";""")
        x = cursor1.fetchone()
        cursor1.close()
        return x[0] > 0
    else:
        freeze_window(username, password, connection)
        
def login_invalid(username: str, password: str, connection: Connection) -> None:
    """ A bunch of code that should disable the login attemps for 300 seconds """
    
    cursor = connection.cursor()
    cursor.execute(f"""
        SELECT
            incorrect_logins
        FROM
            {UsersDatabase.table_name}
        WHERE 
            username LIKE "{username}";
            """)
    cnt = int(cursor.fetchone()[0])
    #flash(cnt)
    cursor.close()
    cursor1 = connection.cursor()
    if cnt < 2:
        cursor1.execute(f"""  
                       UPDATE 
                       {UsersDatabase.table_name}
                       SET 
                       incorrect_logins = incorrect_logins + 1 
                       WHERE
                       username LIKE '{username}';
                       """)
        cursor1.close()
    elif cnt == 2:
        cursor1.execute(f"""  
                       UPDATE 
                       {UsersDatabase.table_name}
                       SET 
                       incorrect_logins = incorrect_logins + 1 
                       WHERE
                       username LIKE '{username}';
                       """)
        cursor1.close()
        update_freezestart_time_to_now(username, connection)
    else:  
        
        freeze_window(username, password, connection)
        
        
def freeze_window(username: str, password: str, connection: Connection):
    cursor3 = connection.cursor()
    cursor3.execute(f"""
        SELECT
            freezestarttime
        FROM
            {UsersDatabase.table_name}
        WHERE 
            username LIKE "{username}";
            """)
    starttime = cursor3.fetchone()[0]
    flash(starttime)
    cursor3.close()
    print("starttime ",starttime)
    endtime = starttime + datetime.timedelta(seconds=45)
    print("endtime ", endtime)
    cursor2 = connection.cursor()
    if endtime < datetime.datetime.now():
        cursor2.execute(f"""  
                   UPDATE 
                   {UsersDatabase.table_name}
                   SET 
                   incorrect_logins = 0
                   WHERE
                   username LIKE '{username}';
                   """)
        cursor2.close()
        sys.exit()
    else:
        flash("Your account is locked for 45 seconds, due to multiple failure attemps")
        
    