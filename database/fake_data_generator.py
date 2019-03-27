""" script used to create some fake data
"""
import mysql.connector

db = mysql.connector.connect(host="localhost",
                              user="hippo_dev",
                              passwd="hippo_dev_pwd",
                              db="hippo_water_db")
cur = db.cursor()
val = 100
sql = "INSERT INTO sensor_data(sensor_id, sensor_value, created_at) VALUES (1, 3100, '2019-01-01')"
cur.execute("INSERT INTO sensor_data(sensor_id, sensor_value, created_at) VALUES (%d, %d, '%s')" % (1, 3000, '2019-03-01'))
for i in range(1, 23):
    if (i < 10):
        datestr = '2019-03-0' + str(i) 
    else:
        datestr = '2019-03-' + str(i) 
    print(datestr)
    cur.execute("INSERT INTO sensor_data(sensor_id, sensor_value, created_at) VALUES (%d, %d, '%s')" % (1, (val * i), datestr))
    #insertStr = (1, val * i, datestr)
    #cur.execute(sql, insertStr)
    db.commit()
db.close()
