<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<title>Untitled Document</title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">

<script type="text/javascript" src="assets/js/jquery.js"></script>

<script type="text/javascript">
jQuery(function($){
	function recursive(target, execution){
		if(typeof(target) == 'object')
			for(var i in target)
				recursive(target[i], execution);
		else
			execution(target);
	};

	$('#request').bind('click', function(e){
		$.ajax({
			type: 'POST',
			url: 'get_data.php',
			data: {
				filename: '2-3-1'
			},
			success: function(r){
				var data = $.parseJSON(r);
				recursive(data, function(target){
					$('#output').append(target + '<br />');
				});
			}
		});
	});
});
</script>

</head>
<body>

<div id="output">



</div>

<button type="button" id="request">Request Data</button>

</body>
</html>