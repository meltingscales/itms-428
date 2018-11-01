import os


class Config(object):
    PROJECT_DIR = os.path.dirname(__file__)
    LOGIN_FILE_NAME = 'login_info.txt'
    SERVER_IP = '127.0.0.1'  # I'm assuming you're running this locally.
    DATABASE_NAME = 'itms428'
    SECRET_KEY = os.environ.get('SECRET_KEY', 'this_is_not_secure_so_please_dont_do_it_in_a_production_server_k_thx_bai')
