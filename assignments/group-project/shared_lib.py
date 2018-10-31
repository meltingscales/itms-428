from constants import LOGIN_FILE_NAME


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

