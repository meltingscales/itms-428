'''
Global Terrorism Database at https://www.start.umd.edu/gtd/contact/

Open Flight Data at https://openflights.org/data.html

U.S. Government Open Data at https://www.data.gov/

18 places to find data sets for data science projects at https://www.dataquest.io/blog/free-datasets-for-projects/

'''

from sqlite3 import *
from pprint import pprint

filename = 'airports'
fileext = 'dat'
dbext = 'db'

c = connect('.'.join((filename, dbext)))

try:
    pass
except Exception as e:
    pass

with open('.'.join((filename, fileext)), 'r', encoding='utf-8') as f:
    for line in f.readlines():
        print(str(line))
