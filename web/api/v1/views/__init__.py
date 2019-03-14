#!/usr/bin/python3
""" import Blueprint and defines all the routes
"""

from flask import Blueprint
import MySQLdb

db = MySQLdb.connect(host="localhost",
                     user="hippo_dev",
                     passwd="hippo_dev_pwd",
                     db="hippo_water_db")

app_views = Blueprint('app_views', __name__, url_prefix='/api/v1')
if app_views:
    from api.v1.views.index import *
