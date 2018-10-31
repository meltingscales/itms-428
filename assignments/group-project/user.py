from data.mysql_sample_db import *


class User(object):

    def __init__(self, *args, **kwargs):
        self.username = kwargs.get("username", "test_user")
        self.password = kwargs.get("password", "test_password")
        self.host = kwargs.get("host", "localhost")
        self.global_privs = kwargs.get("global_privs", [])
        self.table_privs = kwargs.get("table_privs", {})

    def name_and_host(self) -> str:
        return f"""'{self.username}'@'{self.host}'"""

    def create_statement(self) -> str:
        return f"""CREATE USER {self.name_and_host()} IDENTIFIED BY '{self.password}';"""

    def grant_global_priv(self) -> str:
        allprivs = ', '.join(self.global_privs)

        return f"GRANT {allprivs} ON *.* TO {self.name_and_host()};"

    def grant_table_priv(self) -> [str]:
        ret = []

        table_name: str
        privileges: [str]

        for table_name, privileges in self.table_privs.items():
            ret.append(f"GRANT {', '.join(privileges)} ON {table_name} TO {self.name_and_host()};")

        return ret


ALL_USERS = [
    User(username=f'{Config.DATABASE_NAME}_admin', password='totallysecure',
         table_privs={
             f"{Config.DATABASE_NAME}.*": ["ALL"]
         }),

    User(username=f'{Config.DATABASE_NAME}_farmer_payment', password='iamafarmer',
         table_privs={
             f"{Config.DATABASE_NAME}.{FarmerDatabase.table_name}": ["SELECT", "CREATE", "UPDATE"]
         }),

    User(username=f'{Config.DATABASE_NAME}_product_manager', password='icannotmanage',
         table_privs={
             f"{Config.DATABASE_NAME}.{ProductLinesData.table_name}": ["SELECT", "CREATE", "UPDATE", "DELETE"],
             f"{Config.DATABASE_NAME}.{ProductsData.table_name}": ["SELECT", "CREATE", "UPDATE", "DELETE"],
             f"{Config.DATABASE_NAME}.{OfficesData.table_name}": ["SELECT"],
         }),

    User(username=f"{Config.DATABASE_NAME}_payment_manager", password='cashmoney',
         table_privs={
             f"{Config.DATABASE_NAME}.{PaymentsData.table_name}": ["SELECT", "CREATE", "UPDATE"],
             f"{Config.DATABASE_NAME}.{OrdersData.table_name}": ["SELECT", "UPDATE"],
             f"{Config.DATABASE_NAME}.{OrderDetailsData.table_name}": ["SELECT", "UPDATE"],
             f"{Config.DATABASE_NAME}.{CustomerData.table_name}": ["SELECT"],
             f"{Config.DATABASE_NAME}.{FarmerDatabase.table_name}": ["SELECT"],
         }),

    User(username=f'{Config.DATABASE_NAME}_human_resources', password='givemeyourorgansmeatbags!!!',
         table_privs={
             f"{Config.DATABASE_NAME}.{CustomerData.table_name}": ["SELECT", "CREATE", "UPDATE", "DELETE"],
             f"{Config.DATABASE_NAME}.{EmployeeData.table_name}": ["SELECT", "CREATE", "UPDATE", "DELETE"],
             f"{Config.DATABASE_NAME}.{OfficesData.table_name}": ["SELECT"],
         }),

]
