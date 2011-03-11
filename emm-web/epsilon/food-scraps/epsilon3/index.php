<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<title>epsilon-minus-minus</title>
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
	
	$('#output').ajaxStart(function(){
		$(this).html('Loading...');
	});
	
	$('#nav button').bind('click', function(e){
		var sender = $(this);
		$.ajax({
			type: 'POST',
			url: 'assets/async/get_wiki.php',
			data: {
				t: sender.val()
			},
			success: function(r){
				$('#output').html(r);
			}
		});
	});
	
	$('#request').bind('click', function(e){
		var sender = $(this);
		$.ajax({
			type: 'POST',
			url: 'assets/async/get_data.php',
			data: {
				f: sender.val()
			},
			success: function(r){
				var data = $.parseJSON(r);
				$('#output').empty();
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

<div id="nav">
	<button type="button" value="Parity Bit">Parity Bit</button>
	<button type="button" value="Code Theory">Code Theory</button>
</div>

<hr />

<button type="button" id="request" value="2-3-1">Request Data</button>

</body>
</html>
