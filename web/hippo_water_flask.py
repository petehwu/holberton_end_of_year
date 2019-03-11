from flask import Flask
from flask import request
from flask_mysqldb import MySQL
import json

app = Flask(__name__)
app.config['MYSQL_HOST'] = 'localhost'
app.config['MYSQL_USER'] = 'hippo_dev'
app.config['MYSQL_PASSWORD'] = 'hippo_dev_pwd'
app.config['MYSQL_DB'] = 'hippo_water_db'
mysql = MySQL(app)

@app.route('/', methods = ['GET'], strict_slashes=False)
def index():
    """ route to display webpage content
    """
    return "<h1 style='color:blue'>Place Holder for website</h1>"


@app.route('/sensor_in', methods=['POST'], strict_slashes=False)
def sensor_data():
    """ route to get data from sensors
    """
    data = request.data
    print(data)
    dataDict = json.loads(data)
    sensor_id = dataDict.get('sensor_id')
    sensor_value = dataDict.get('sensor_value')
    cur = mysql.connection.cursor()
    cur.execute('INSERT INTO sensor_data(sensor_id, sensor_value)\
                 VALUES(%d, %d)' % (sensor_id, sensor_value))
    mysql.connection.commit()
    cur.close()
    
    return "<h1 style='color:blue'>got_data</h1>"

if __name__ == "__main__":
    app.run(host='127.0.0.1')
