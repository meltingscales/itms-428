# OVERVIEW

This folder contains all the materials needed to setup a database for the ITMS 428 group project.

# SETUP

1. Install [Python 3.6.*](https://www.python.org/downloads/).

2. Make sure `pip` is installed. To test this, run `pip` anywhere.

3. Inside this folder, run `pip install pipenv`.
   This installs a module for Python called 'pipenv', which I use to manage dependencies.
   You may notice a file called `Pipfile`. That's where I put all the dependencies!
   
4. Inside this folder, run `pipenv install`.
   
   This tells pipenv to create a virtual environment, segregated from your system's Python
   installation, that has the dependencies located in the Pipfile.

That's it for setup!

To run any script, use `pipenv run python script_name.py`.