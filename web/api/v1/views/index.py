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
    rows = cur.execute('SELECT\
                a.sensor_id,\
                a.plant_name,\
                a.watering_freq,\
                max(b.created_at) as last_watered,\
                datediff(now(), max(b.created_at)) as days_since_watering\
                FROM sensor_info a left outer join sensor_watered b\
                ON a.sensor_id = b.sensor_id\
                GROUP BY a.sensor_id, a.plant_name, a.watering_freq')
    print(cur.description)
    if (rows > 0):
        row_headers = [x[0] for x in cur.description]
        res = cur.fetchall()
        json_data = []
        for result in res:
            json_data.append(dict(zip(row_headers, result)))
    cur.close()

    return jsonify(json_data)
