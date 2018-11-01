import sys

from flaskext.mysql import MySQL

from forms import LoginForm

sys.path.append('..')  # Allows us to import stuff in above folder.

from data.mysql_sample_db import UsersDatabase

from flask import Flask, render_template, flash, redirect
from shared_lib import get_login_creds
from constants import *

username, password = get_login_creds(os.path.join('..', Config.LOGIN_FILE_NAME))

app = Flask(__name__)

mysql = MySQL()

app.config['SECRET_KEY'] = 'itsasecret'

app.config['MYSQL_DATABASE_HOST'] = Config.SERVER_IP
app.config['MYSQL_DATABASE_USER'] = username
app.config['MYSQL_DATABASE_PASSWORD'] = password
app.config['MYSQL_DATABASE_DB'] = Config.DATABASE_NAME

mysql.init_app(app)

connection = mysql.connect()


def user_exists(username: str) -> bool:
    cursor = connection.cursor()

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


def login(username: str, password: str) -> bool:
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


@app.route("/")
def main():
    message = "Logging in to MySQL with {}:{}, eh?".format(username, "<PASSWORD>")

    return render_template('index.html', message=message)


@app.route("/data")
def stats():
    return render_template('data.html', data={
        'cool_table': {
            "header": ['ID', 'data'],
            "data": [['1', 'cool'],
                     ['2', 'not cool']],
        },
        'cooler_table': {
            "header": ['ID', 'data'],
            "data": [['3', 'cool'],
                     ['4', 'not cool']],

        }
    })


@app.route('/admin')
def admin():
    data = {
        UsersDatabase.table_name: {
            'data': (),
            'header': (),
        }
    }

    cursor = connection.cursor()

    cursor.execute(f"SELECT * FROM {UsersDatabase.table_name};")
    data[UsersDatabase.table_name]['data'] = cursor.fetchall()

    cursor.execute(f"SHOW columns FROM {UsersDatabase.table_name};")
    results = cursor.fetchall()  # List of column header infos.
    results = (result[0] for result in results)  # Get first field, the name.
    data[UsersDatabase.table_name]['header'] = results

    cursor.close()
    return render_template('data.html', data=data)


@app.route('/login', methods=['GET', 'POST'])
def login():
    form = LoginForm()

    if form.validate_on_submit():
        flash('Login requested for user {}, remember_me={}'.format(
            form.username.data, form.remember_me.data))

        return redirect('../')

    return render_template('login.html', title="Sign in", form=form)


if __name__ == '__main__':
    app.run()

    # Sanity checks.
    assert (login("henry", "iliketofarm"))
    assert (not login("henry", "notafarmer"))

    assert (user_exists("henry"))
    assert (not user_exists("tiffany_the_lord_of_squids"))
