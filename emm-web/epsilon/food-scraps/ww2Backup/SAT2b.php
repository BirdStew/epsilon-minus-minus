<?php

	$one = $_POST['one'];
	$two = $_POST['two'];
	$three = $_POST['three'];
	$four = $_POST['four'];

	$pattern =  '/^([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])$/';

if(preg_match($pattern, $one)){
	echo "Valid . ";}
else{
	echo "Not Valid . ";}

if(preg_match($pattern, $two)){
	echo "Valid . ";}
else{
	echo "Not Valid . ";}

if(preg_match($pattern, $three)){
	echo "Valid . ";}
else{
	echo "Not Valid . ";}

if(preg_match($pattern, $four)){
	echo "Valid<br />";}
else{
	echo "Not Valid<br />";}
?>