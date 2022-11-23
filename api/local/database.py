from flask import Flask
from database import db, migrate
import models
import views
from schemas import ma
from schemas.output import UserSchema, PostSchema
from schemas.input import UserInputSchema

def create_app(config_object=None, db_path='sqlite:///database.db'):
    app = Flask(__name__)
    app.config['SQLALCHEMY_DATABASE_URI'] = db_path
    app.register_blueprint(views.general_template, url_prefix='/api/v1')

    ma.init_app(app)
    db.init_app(app)
    migrate.init_app(app, db)

    return app
