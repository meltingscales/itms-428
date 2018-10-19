# OVERVIEW

This folder contains all the materials needed to setup a database for the ITMS 428 group project.

# EXTRA SETUP FOR LINUX

If you're using Linux, (I tested this on Ubuntu), you'll need to run the following command BEFORE setting up Python.

`apt-get install libmysqlclient-dev python-dev`

This is because the 'mysqlclient' package is built from source with the version I'm using, at least for Linux.

It presumably needs `libmysqlclient-dev`'s source files, as well as `python-dev`'s source files too.

# SETUP DATABASE

I'll have you all use MySQL because of its popularity, and because it's easy to install on Windows, Linux, and OSX.

1. Download & install WAMP/LAMP/XAMPP/MAMP
    - [I use Windows!](http://www.wampserver.com/en/)
    - [I use Linux!](https://www.linode.com/docs/web-servers/lamp/install-lamp-stack-on-ubuntu-16-04/)
    - [I, for some inconcievable reason, use Solaris!](https://www.apachefriends.org/index.html)
    - [I use OSX!](https://www.mamp.info/en/)

      (P.S. If MAMP for OSX sucks, use XAMPP. It's cross-platform.)

 2. Find out the default username and password.

    It _might_ be different for different OSes. Not sure.

    You will need this for the Python step.

# SETUP PYTHON

1. Install [Python 3.6.*](https://www.python.org/downloads/).

2. Make sure `pip` is installed. To test this, run `pip` anywhere.

3. Inside this folder, run `pip install pipenv`.
   This installs a module for Python called 'pipenv', which I use to manage dependencies.
   You may notice a file called `Pipfile`. That's where I put all the dependencies!
   
4. Inside this folder, run `pipenv install`.
   
   This tells pipenv to create a virtual environment, segregated from your system's Python
   installation, that has the dependencies located in the Pipfile.

5. Make a file in this folder called `login_info.txt`.

   On the first line, put the username.
   On the second line, put the password.
   
   This file will NOT be tracked by Git, due to the `/.gitignore` file at the repo's root.
   
   The reason I have you do this is so that if one were to fork this repo, my personal login
   info would not be forced upon you, and your copy would not contain this.
   
That's it for setup!

To run any script, use `pipenv run python script_name.py`.