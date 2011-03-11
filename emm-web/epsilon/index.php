<?php 

if(isset($_POST['submit'])) 
{
   $estr = "core/emm-core -w ". escapeshellarg($_POST['w']) . " -p " . escapeshellarg($_POST['p']) . " -t ". escapeshellarg($_POST['pyt']) ." core/demo.bmp";
   echo 'Exec String '.$estr . '<br>'; 
   $jout = shell_exec($estr);

   $jout = json_decode($jout, true);
  

   echo 'N: ' . $jout['n'] . '<br>'; 
   echo 'K: ' . $jout['k'] . '<br>';
   echo 'Distance: ' . $jout['d'] . '<br>';

   echo '<br>Generator Matrix<br><div>' . str_replace(',','<br>',$jout['gen']) . '<div>';
   echo '<br>Control Matrix<br><div>' . str_replace(',','<br>',$jout['con']) . '<div>';

}

?>


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
<noscript>We're sorry, but your browser does not support Javascript.</noscript>
</head>
<body style="color:white;background:black">
<h1 style="text-align:center">epsilon-minus-minus</h1>

<div id="output" style="color:white;background:black">
<h2 style="text-align:center">Instructional</h2>
<p>welcome to the epsilon-minus-minus instructional!<br/>
<br/>

epsilon-minus-minus is an open Source project that explores the properties of error-detection schemes in linear codes. Within this instructional you will find the following:</br></br>
What is Coding Theory?</br>
Linear Codes</br></br>
</p>
</br></br></br></br>
</div>

<div id="nav">
	<button type="button" value="Parity Bit">Parity Bit</button>
	<button type="button" value="Panda">Panda</button>
</div>


<form name="cmdform" method="post" action="index.php">


Word length:
<select name="w">
<?php
for($i=2; $i<13; $i++)
  echo '<option value="'.$i.'">'.$i.'</option>';
?>
</select>
<br>

Parity Length:
<select name="p">
<?php
for($i=2; $i<13; $i++)
  echo '<option value="'.$i.'">'.$i.'</option>';
?>
</select>
<br>

Parity Type:
<select name="pyt">
  <option value="1" >Dense</option>
  <option value="2" >Low Density</option>
  <option value="4" >Random</option>
</select>
<br>
<br>
<input name="submit" type="submit" value="submit">
</form>

<button type="button" id="request" value="2-3-1">Request Data</button>

</body>
</html>

