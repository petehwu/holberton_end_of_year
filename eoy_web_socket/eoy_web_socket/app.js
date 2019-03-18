var mysql = require('mysql');
var con = mysql.createConnection({
	host: 'localhost',
	user: 'hippo_dev',
	password: 'hippo_dev_pwd',
	database: 'hippo_water_db'
});

var fs = require('fs');
var util  = require('util');


var express = require('express');
var app = express();
var server  = require('http').createServer(app);
var io = require('socket.io')(server, {
	pingInterval: 90000,
	pingTimeout: 10000
});
app.use(express.static(__dirname + '/node_modules'));
app.use('/static', express.static(__dirname + '/static'));
app.get('/', function(req, res, next) {
	res.sendFile(__dirname + '/basic_page.html');
});
server.listen(5010);
con.connect(function(err) {
	if (err) throw err;
		console.log('connected to mysql');
});

// app listens on port 5010
//app.listen(5010);

//function handler (req, res) {
//	console.log(__dirname);
//	fs.readFile(__dirname + '/index.html',
//	fs.readFile(__dirname + '/basic_page.html',
 // 	function (err, data) {
  //  		if (err) {
//      			res.writeHead(500);
//      			return res.end('Error loading index.html');
//    		}
//    		res.writeHead(200);
//    		res.end(data);
//  	});
//}

function ParseJson(jsondata) {
	try {
		return JSON.parse(jsondata);
    	} catch (error) {
		return null;
    	}
}

function sendTime() {
    io.sockets.emit('atime', { time: new Date().toJSON() });
}
io.on('connection', function (socket) {
	console.log("Connected");
	socket.emit('welcome', { message: 'Connected!' });
	socket.on('connection', function (data) {
		console.log(data);
	});
	socket.on('reported', function (data) {
		console.log(data);
		con.query('INSERT INTO sensor_data SET ?', data, (err, res) =>{
	 		if(err) throw err;
			 	console.log('inserted:');
 		});
		io.sockets.emit('reload', {message: 'reload'});
		console.log('report received, sensor:  ' + data.sensor_id + ' value ' + data.sensor_value);
 	});
	socket.on('watered', function (data) {
		con.query('INSERT INTO sensor_watered SET ?', data, (err, res) =>{
			if(err) throw err;
		 		console.log('last insert id: ', res.id);
	 	});
		io.sockets.emit('reload', {message: 'watered'});
		console.log('water received, sensor:  ' + data.sensor_id + ' value ' + data.sensor_value);
  	});
	socket.on('JSON', function (data) {
	//	console.log(data);
		var jsonStr = JSON.stringify(data);
	    	var parsed = ParseJson(jsonStr);
		console.log(parsed);
		console.log(parsed.sensor);
  	});
 	socket.on('arduino', function (data) {
		console.log("web button pressed");
	//	io.sockets.emit('arduino', { message: 'R0' });
	 	io.sockets.emit('locate', { sensor_id: data});
    		console.log(data);
  	});

});
