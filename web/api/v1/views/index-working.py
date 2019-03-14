#!/usr/bin/python3
"""  Summary to show data for all sensors
"""
from api.v1.views import app_views
from flask import jsonify

@app_views.route('/status')
def status():
    """returns a json ok string
    """
    return jsonify({"status": "OK"})


