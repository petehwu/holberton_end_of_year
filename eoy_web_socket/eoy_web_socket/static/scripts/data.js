$(function () {
	function doPoll() {
		const uriEndpoint = 'https://hippocamp.site/api/v1/summary';
		$.get(uriEndpoint, function(results) {
			$('section.plant_data').empty();
			$.each(results, function (index, pi) {
				let freq = 7;
				let color = "flex-container";
				if (pi.watering_freq == "Monthly") {
					freq = 30;
				}
				if (pi.days_since_watering >= freq) {
					color = "flex-container-red";
				}
				const htmlStr = '<div class="' + color + '">' +
					'<div class="row_element">' +
					'<input type="button" class="poke"  name="' + pi.sensor_id +'" value="Locate" id="poke' + pi.sensor_id + '" />' +
					'</div>' +
					'<div class="row_element"> Information:' +
					'<a href="' + pi.plant_url + '">' +
					pi.plant_name +
					'</a>' +
					'</div>' +
					'<div class="row_element"> Last Watered: <div>' +
					pi.last_watered_date +
					'</div>' +
					'</div>' +
					'<div class="row_element"> Days Since Watered: <div>' +
					pi.days_since_watering +
					'</div>' +
					'</div>' +
					'<div class="row_element"> Frequency: <div>' +
					pi.watering_freq +
					'</div>' +
					'</div>' +
					'<div class="row_element"> Latest Reading: <div>' +
					pi.sensor_value +
					'<div class="popover" name="' + pi.sensor_id + '">' +
					'</div>'
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
	socket.on('welcome', function(message) {
		console.log('The server welcomes you: ');
		doPoll();
	})
	socket.on('reload', function(message) {
		console.log('reload');
		doPoll();
	});
	$(document).on('click', '.poke', function(){
		socket.emit('arduino', $(this).attr("name"));
	})

})
