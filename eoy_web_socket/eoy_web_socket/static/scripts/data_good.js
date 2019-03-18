$(function () {
	function doPoll() {
		const uriEndpoint = 'https://hippocamp.site/api/v1/summary';
		$.get(uriEndpoint, function(results) {
			$('section.plant_data').empty();
			$.each(results, function (index, pi) {
				const htmlStr = '<div class="flex-container">' +
					'<div name="sensor_' + pi.sensor_id + '">' +
					'<input type="button" class="poke"  name="' + pi.sensor_id +'" value="Locate" id="poke' + pi.sensor_id + '" />' +
					'</div>' +
					'<div> Information:' +
					'<a href="https://en.wikipedia.org/wiki/Zamioculcas">' +
					pi.plant_name +
					'</a>' +
					'</div>' +
					'<div id="water_' + pi.sensor_id + '"> Last Watered: <div>' +
					pi.last_watered +
					'</div>' +
					'</div>' +
					'<div id="days_' + pi.sensor_id + '"> Days Since Watered: <div>' +
					pi.days_since_watering +
					'</div>' +
					'</div>' +
					'<div> Frequency: <div>' +
					pi.watering_freq +
					'</div>' +
					'</div>' +
					'<div name="reading_' + pi.sensor_id + '"> Latest Reading: <div>' +
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
	};
        const socket = io.connect('https://hippocamp.site', {secure: true});
        //var socket = io.connect('https://petehwu.online', {secure: true});
	$(document).ready(function () {
		socket.on('welcome', function(message) {
			alert('The server welcomes you: ' + message);
			doPoll();
		})
		socket.on('reload', function(message) {
			alert('reload');
			doPoll();
		});
		$(document).on('click', '.poke', function(){
			socket.emit('arduino', $(this).attr("name"));
		})
	});

})
