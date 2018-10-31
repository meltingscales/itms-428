import html
import sys

sys.path.append('..')  # Allows us to import stuff in above folder.

from flask import Flask
from shared_lib import get_login_creds

app = Flask(__name__)

uname, pw = get_login_creds("../login_info.txt")


@app.route("/")
def main():
    return "Logging in to MySQL with {}:{}, eh?".format(uname, html.escape("<PASSWORD>"))


if __name__ == '__main__':
    app.run()
