from data.mysql_sample_db import TestDatabase


class Trigger(object):
    name = "iamatrigger"
    trigger = f"""INSERT INTO """

    def __init__(self, name: str, trigger: str):
        self.name = name
        self.trigger = trigger

    def generate_trigger(self):
        return self.trigger.format(name=self.name)


triggers = [
    Trigger('coolertrigger', f"""CREATE TRIGGER {"{name}"}
    AFTER INSERT ON {TestDatabase.table_name}
    FOR EACH ROW
    BEGIN
        INSERT INTO {TestDatabase.table_name} VALUES (NULL, "hello!");
    END;
    """)
]
