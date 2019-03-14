$(function () {
	// populate places of the website
	const uriEndpoint = 'https://hippocamp.site/api/v1/summary';
	$.ajax({
		type: 'GET',
		url: uriEndpoint,
		data: '{}',
		dataType: 'json',
		contentType: 'application/json',
		success: function (results) {
			console.log('results returned: ' + results.length); // debug message
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
					pi.water_freq +
					'</div>' +
					'</div>' +
					'</div>';
				const article = document.createElement('article');
				article.innerHTML = htmlStr;
				$(article).insertAfter('div.flex-col');
			});
		}
	});
	// end populate places of the website

})
