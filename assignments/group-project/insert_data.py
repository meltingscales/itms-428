import os
import xlrd

PROJECT_DIR = os.path.dirname(__file__)
LOGIN_FILE_NAME = 'login_info.txt'
SERVER_IP = '127.0.0.1' # I'm assuming you're running this locally.


def get_login_creds(path: str) -> (str, str,):
    """Human-proof way to get login info from plaintext file."""

    try:
        file = open(path, 'r')
    except FileNotFoundError as e:
        print(f"Please make {LOGIN_FILE_NAME}, and put your username and password in it, separated by a newline.")
        exit(1)

    lines = file.readlines()

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
