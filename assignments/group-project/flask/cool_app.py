import sys

from flaskext.mysql import MySQL

sys.path.append('..')  # Allows us to import stuff in above folder.

from flask import Flask, render_template
from shared_lib import get_login_creds
from constants import *

username, password = get_login_creds(os.path.join('..', LOGIN_FILE_NAME))

app = Flask(__name__)

mysql = MySQL()

app.config['MYSQL_DATABASE_HOST'] = SERVER_IP
app.config['MYSQL_DATABASE_USER'] = username
app.config['MYSQL_DATABASE_PASSWORD'] = password
app.config['MYSQL_DATABASE_DB'] = DATABASE_NAME

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


if __name__ == '__main__':
    app.run()
