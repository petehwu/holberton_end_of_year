#!/usr/bin/python3
""" default entry point for api
"""

from api.v1.views import app_views
from flask import Flask
from flask import jsonify
from flask import make_response
from flask_cors import CORS


app = Flask(__name__)
cors = CORS(app, resources={r"/*": {"origins": "*"}})

app.register_blueprint(app_views)
app.url_map.strict_slashes = False


@app.errorhandler(404)
def page_not_found(error):
    """return page not found error"""
    return make_response(jsonify({"error": "Not found"}), 404)

if __name__ == '__main__':
    app.run(host='127.0.0.1', port=5001, threaded=True, debug=True )
