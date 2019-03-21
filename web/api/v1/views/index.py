#!/usr/bin/python3
"""  Summary to show data for all sensors
"""
from api.v1.views import app_views
from flask import jsonify
import MySQLdb


@app_views.route('/status')
def status():
    """returns a json ok string
    """
    return jsonify({"status": "OK"})

@app_views.route('graph/<sensor_id>',
                 strict_slashes=False, methods=['GET'])
def get_graph_date(sensor_id):
    """ The method gets the data for graphying moisture levels
    """
    db = MySQLdb.connect(host="localhost",
                         user="hippo_dev",
                         passwd="hippo_dev_pwd",
                         db="hippo_water_db")
    cur = db.cursor()
    rows = cur.execute('SELECT \
            sensor_value \
            FROM sensor_data \
            WHERE sensor_id = ' + sensor_id + ' ORDER BY id ASC')
    json_data = []
    if (rows > 0):
        row_headers = ['y']
        res = cur.fetchall()
        for result in res:
            json_data.append(dict(zip(row_headers, result)))
    else:
        json_data.append(dict(y=0))
    cur.close()
    db.close()

    return jsonify(json_data)

@app_views.route('/summary',
                 strict_slashes=False, methods=['GET'])
def stats():
    """ this method gets the summary information to
    populate the rows in the website
    """
    db = MySQLdb.connect(host="localhost",
                         user="hippo_dev",
                         passwd="hippo_dev_pwd",
                         db="hippo_water_db")

    """returns a json with summary of sensor data
    """
    cur = db.cursor()
    rows = cur.execute('SELECT \
	d.sensor_id, \
	d.plant_name, \
	d.plant_url, \
	d.watering_freq, \
	IFNULL(d.last_watered_day, "No Data Found!") as last_watered_day, \
	IFNULL(d.last_watered_date, "No Data Found!") as last_watered_date, \
	IFNULL(d.days_since_watering, "No Data Found!") as days_since_watering, \
	IFNULL(g.sensor_value, "No Data Found!") as sensor_value \
	FROM ( SELECT \
		a.sensor_id, \
		a.plant_name, \
                a.plant_url, \
		a.watering_freq, \
		date_format(max(b.created_at), "%W") AS last_watered_day, \
		date_format(max(b.created_at), "%m-%d-%Y") AS last_watered_date, \
		DATEDIFF(now(), max(b.created_at)) AS days_since_watering \
		FROM sensor_info AS a \
		LEFT OUTER JOIN sensor_watered AS b \
		ON a.sensor_id = b.sensor_id \
		GROUP BY a.sensor_id, a.plant_name, a.watering_freq) AS d \
	LEFT OUTER JOIN ( SELECT \
		e.sensor_id, \
		e.sensor_value, \
		e.created_at \
		FROM ( SELECT \
			sensor_id, \
			sensor_value, \
			created_at, \
			id \
			FROM sensor_data) AS e \
		JOIN (SELECT \
			sensor_id, \
                        MAX(id) as c_id \
			FROM sensor_data GROUP BY sensor_id) AS f \
		ON e.sensor_id = f.sensor_id \
		AND e.id = f.c_id) AS g \
	ON d.sensor_id = g.sensor_id ORDER BY d.sensor_id ASC')
    if (rows > 0):
        row_headers = [x[0] for x in cur.description]
        res = cur.fetchall()
        json_data = []
        for result in res:
            json_data.append(dict(zip(row_headers, result)))
    cur.close()
    db.close()

    return jsonify(json_data)
