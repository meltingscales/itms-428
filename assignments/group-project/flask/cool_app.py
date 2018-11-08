import os
import sys

from flaskext.mysql import MySQL

import forms
from mysql_helper import login_valid, user_exists, update_login_time_to_now
from shared_lib import get_login_creds

sys.path.append('..')  # Allows us to import stuff in above folder.

from data.mysql_sample_db import UsersDatabase, Config

from flask import render_template, flash, redirect, url_for, Flask

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
    form = forms.LoginForm()

    if form.validate_on_submit():

        flash('Login requested for user {}, remember_me={}'.format(
            form.username.data, form.remember_me.data))

        if login_valid(username=form.username.data, password=form.password.data, connection=connection):
            flash("Login is correct. Welcome, {}!".format(form.username.data))
            update_login_time_to_now(form.username.data, connection)
        return redirect(url_for('main'))

    return render_template('login.html', title="Sign in", form=form)


if __name__ == '__main__':
    app.run()

    # Sanity checks.
    assert (login_valid("henry", "iliketofarm", connection))
    assert (not login_valid("henry", "notafarmer", connection))

    assert (user_exists("henry", connection))
    assert (not user_exists("tiffany_the_lord_of_squids", connection))
