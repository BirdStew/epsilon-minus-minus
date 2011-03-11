<html>
<head>
<title> epsilon-minus-minus</title>

<script> type = "text/javascript">
document.write("Welcome to epsilon-minus-minus!")
</script>
<noscript>Sorry, your browser does not support necessary Javascript
</noscript>

<body>
<form method="post" action="<?php echo $PHP_SELF;?>">

N: Number of bits in original words<br/>
<select name="N">
<?php
$i=1;$MAX_N=20;
while($i<=$MAX_N){echo "<option value=\"$i\">$i</option>";$i++;}?>
</select>:<br/>

K: number of bits in code words<br/>
<select name="K">
<?php
$i=1;$MAX_K=30;
while($i<=$MAX_N){echo "<option value=\"$i\">$i</option>";$i++;}?>
</select>:<br/>

P:(K-N) Number of parity bits<br/>
<select name="P">
<?php
$i=1;$MAX_P=10;
while($i<=$MAX_P){echo "<option value=\"%i\">$i</option>";$i++;}?>
</select>:<br/>
<input type="submit" value="submit" name="submit">
</form>

