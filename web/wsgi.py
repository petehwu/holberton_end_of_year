#usr/bin/python3
""" main entry point for gunicorn 
"""

from hippo_water_flask import app as application


if __name__ == '__main__':
    application.run()
 
