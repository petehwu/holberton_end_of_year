#!/usr/bin/python3
"""  Summary to show data for all sensors
"""
from api.v1.views import app_views
from flask import jsonify
from api.v1.views import db


@app_views.route('/status')
def status():
    """returns a json ok string
    """
    return jsonify({"status": "OK"})


@app_views.route('/summary')
def stats():
    """returns a json with summary of sensor data
    """
    cur = db.cursor()
    rows = cur.execute('SELECT \
	d.sensor_id, \
	d.plant_name, \
	d.watering_freq, \
	d.last_watered, \
	d.days_since_watering, \
	g.sensor_value \
	FROM ( SELECT \
		a.sensor_id, \
		a.plant_name, \
		a.watering_freq, \
		max(b.created_at) AS last_watered, \
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
			created_at \
			FROM sensor_data) AS e \
		JOIN (SELECT \
			sensor_id, \
			MAX(created_at) AS c_at \
			FROM sensor_data GROUP BY sensor_id) AS f \
		ON e.sensor_id = f.sensor_id \
		AND e.created_at = f.c_at) AS g \
	ON d.sensor_id = g.sensor_id ORDER BY d.sensor_id ASC')
    print(cur.description)
    if (rows > 0):
        row_headers = [x[0] for x in cur.description]
        res = cur.fetchall()
        json_data = []
        for result in res:
            json_data.append(dict(zip(row_headers, result)))
    cur.close()

    return jsonify(json_data)
