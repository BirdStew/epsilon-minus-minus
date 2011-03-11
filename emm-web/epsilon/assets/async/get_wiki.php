<?php
ob_start();

$ch = curl_init();
		
curl_setopt_array($ch, array(
	CURLOPT_URL => 'http://en.wikipedia.org/w/api.php',
	//CURLOPT_POST => 0,
	CURLOPT_USERAGENT => 'epsilon-minus-minus',
	CURLOPT_POSTFIELDS => http_build_query(array(
		'format' => 'php',
		'action' => 'query',
		'prop' => 'revisions',
		'titles' => $_POST['t'],
		'rvprop' => 'content',
		'redirects' => 1
	))
));

curl_exec($ch);

$content = unserialize(ob_get_clean());

curl_close($ch);

$content = array_keys($content['query']['pages']);

ob_start();

$ch = curl_init();

curl_setopt_array($ch, array(
	CURLOPT_URL => 'http://en.wikipedia.org/w/api.php',
	CURLOPT_POST => 1,
	CURLOPT_USERAGENT => 'epsilon-minus-minus',
	CURLOPT_POSTFIELDS => http_build_query(array(
		'format' => 'php',
		'action' => 'parse',
		'pageid' => $content[0],
		'prop' => 'text|sections'
	))
));

curl_exec($ch);

$content = unserialize(ob_get_clean());

curl_close($ch);

echo str_replace('href="/wiki/', 'href="http://en.wikipedia.org/wiki/', $content['parse']['text']['*']);
?>
