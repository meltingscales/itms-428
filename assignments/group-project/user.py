class User(object):

    def __init__(self, *args, **kwargs):
        self.username = kwargs.get("username", "test_user")
        self.password = kwargs.get("password", "test_password")
        self.host = kwargs.get("host", "localhost")

    def create_statement(self):
        return f"""CREATE USER '{self.username}'@'{self.host}' IDENTIFIED BY '{self.password}';"""
