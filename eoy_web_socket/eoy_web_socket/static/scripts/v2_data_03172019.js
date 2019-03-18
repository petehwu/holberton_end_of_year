$(function () {
	const uriEndpoint = 'https://hippocamp.site/api/v1/summary';
	$.get(uriEndpoint, function(results) {
		//$('section.plant_data').empty();
		$.each(results, function (index, pi) {
			const htmlStr = '<div class="flex-container">' +
				'<div name="sensor_' + pi.sensor_id + '">' +
				'<input type="button" class="poke"  name="' + pi.sensor_id +'" value="Locate Sensor" id="poke' + pi.sensor_id + '" />' +
				'</div>' +
				'<div name="2sensor_"' + pi.sensor_id + '">' +
				'<input type="button" class="poke5" name="' + pi.sensor_id +'" value="Locate Sensor" id="poker' + pi.sensor_id + '" />' +
				'</div>' +
				'<div> Information:' +
				'<a href="https://en.wikipedia.org/wiki/Zamioculcas">' +
				pi.plant_name +
				'</a>' +
				'</div>' +
				'<div> Last Watered: <div>' +
				pi.last_watered +
				'</div>' +
				'</div>' +
				'<div name="watered_' + pi.sensor_id + '"> Days Since Watered: <div>' +
				pi.days_since_watering +
				'</div>' +
				'</div>' +
				'<div> Frequency: <div>' +
				pi.watering_freq +
				'</div>' +
				'</div>' +
				'<div name="reading"' + pi.sensor_id + '"> Latest Reading: <div>' +
				pi.sensor_value +
				'</div>' +
				'</div>' +
				'</div>';
			$('section.plant_data').append(htmlStr);
			//const article = document.createElement('article');
			//article.innerHTML = htmlStr;
			//$(article).insertAfter('section.plant_data');
		});
	});
        const socket = io.connect('https://hippocamp.site', {secure: true});
        //var socket = io.connect('https://petehwu.online', {secure: true});
	$(document).ready(function () {
        socket.on('message', function(message) {
                alert('The server has a message for you: ' + message);
        })
	$('button[class^="poke"]').click(function () {
		alert('y');
		socket.emit('arduino', 'something');
	});
        $('.poke').click(function () {
		alert('here');
                socket.emit('arduino', $('#poke.name'));
        })
        $('.poke5').click(function () {
		alert('here2');
                socket.emit('arduino', $('#poke5.value'));
        })
	$('#poke50').click(function () {
		alert('5-');
		socket.emit('arduino', '6');
	})
	$('#poke1').click(function () {
		alert('xx');
		socket.emit('arduino', '06');
	})
	$(document).on('click', '.poke', function(){
		socket.emit('arduino', $(this).attr("name"));
	})
	});

})
