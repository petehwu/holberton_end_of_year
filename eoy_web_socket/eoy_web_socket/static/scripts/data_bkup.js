$(function () {
	// populate places of the website
	function doPoll() {
		const uriEndpoint = 'https://hippocamp.site/api/v1/summary';
		$.get(uriEndpoint, function(results) {
			$('section.plant_data').empty();
			$.each(results, function (index, pi) {
				const htmlStr = '<div class="flex-container">' +
					'<div> Information:' +
					'<a href="https://en.wikipedia.org/wiki/Zamioculcas">' +
					pi.plant_name + 
					'</a>' +
					'</div>' +
					'<div> Last Watered: <div>' +
					pi.last_watered +
					'</div>' +
					'</div>' +
					'<div> Days Since Watered: <div>' +
					pi.days_since_watering +
					'</div>' + 
					'</div>' +
					'<div> Frequency: <div>' +
					pi.watering_freq +
					'</div>' +
					'</div>' +
					'<div> Latest Reading: <div>' +
					pi.sensor_value +
					'</div>' +
					'</div>' +
					'</div>';
				$('section.plant_data').append(htmlStr);
				//const article = document.createElement('article');
				//article.innerHTML = htmlStr;
				//$(article).insertAfter('section.plant_data');
			});
		setTimeout(doPoll, 10000);
		});
		// end populate places of the website
	};
	doPoll();
})
