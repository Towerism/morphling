from flask_wtf import FlaskForm
from wtforms import StringField, BooleanField
from wtforms.validators import DataRequired

class MyForm(FlaskForm):
    title = StringField('title', validators=[DataRequired()])
    year = StringField('year', validators=[DataRequired()])
    rating = StringField('rating', validators=[DataRequired()])

