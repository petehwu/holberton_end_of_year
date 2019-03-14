#usr/bin/python3
""" main entry point for gunicorn 
"""

from api.v1.app import app as application


if __name__ == '__main__':
    application.run()
 
