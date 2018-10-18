'''
Global Terrorism Database at https://www.start.umd.edu/gtd/contact/

Open Flight Data at https://openflights.org/data.html

U.S. Government Open Data at https://www.data.gov/

18 places to find data sets for data science projects at https://www.dataquest.io/blog/free-datasets-for-projects/

'''

from sqlite3 import *
from pprint import pprint
import pandas as pd

filename = 'airports'
fileext = 'dat'
dbext = 'db'

dbname = '.'.join((filename, dbext))

print(f"Connecting to '{dbname}'.")
c = connect(dbname)

try:
    c.execute('''
    CREATE TABLE airports(
        id      INTEGER PRIMARY KEY NOT NULL,
        name    STRING,
        loc1    STRING,
        loc2    STRING,
        code1   STRING,
        code2   STRING,
        lat     DOUBLE,
        lon     DOUBLE,
        num1    INTEGER,
        num2    INTEGER,
        char1   STRING,
        loc3    STRING,
        type1   STRING,
        type2   STRING
    )
    ''')
except OperationalError as e:  # i.e. table already exists
    print(str(e))
    pass

fn = '.'.join((filename, fileext))

print(f"Opening '{fn}'")

with open(fn, 'r', encoding='utf-8') as f:
    for line in f.readlines():
        try:
            line = line.strip()  # remove whitespace
            line = line.replace(r'\N', r'null')  # why do they put \N? we will never know!
            line = line.replace(r'\"', r"'")  # :>)

            ls = line.split(',')

            sql = 'INSERT INTO airports VALUES(' + line + ");"
            print(sql)

            c.execute(sql)

        except Exception as e:
            print(line)
            print(e)

c.commit()

pd.set_option('display.width', 1000)
pd.set_option('display.max_columns', 500)

print(pd.read_sql_query("""
SELECT 
    id, name, lat, lon
FROM 
	airports 
WHERE 
	lat >= 3
	AND
	lat <= 6
ORDER BY 
	lat ASC;
	""", c))

c.close()
