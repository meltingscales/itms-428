from flask_wtf import FlaskForm
from wtforms import StringField, PasswordField, BooleanField, SubmitField
from wtforms.validators import DataRequired

from cool_app import connection
from mysql_helper import user_exists, login_valid


class LoginForm(FlaskForm):
    username = StringField('Username', validators=[DataRequired()])
    password = PasswordField('Password', validators=[DataRequired()])
    remember_me = BooleanField('Remember Me')
    submit = SubmitField('Sign In')

    def validate(self):
        rv = FlaskForm.validate(self)
        if not rv:
            return False

        if not user_exists(self.username.data, connection):
            self.username.errors.append("User does not exist.")
            return False

        if not login_valid(self.username.data, self.password.data, connection):
            self.password.errors.append('Invalid password.')
            return False

        return True
