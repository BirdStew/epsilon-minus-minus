<?php
require "array_fun.php";
//#1
foreach ($months as $value){
echo "<b>$value</b><br>";}
echo "<br>";
//#2
foreach (array_reverse($months, true) as $value){
echo "<i>$value</i><br>";}
echo "<br>";
//#3
foreach ($days_in_month as $key => $value){
echo "<font color = blue>$key $value</font><br>";}
echo "<br>";
//#4
function asc($a, $b){
if ($a == $b) {
return 0;}
return ($a < $b) ? -1 : 1;}
$tmp = $days_in_month;
uasort($tmp, "asc");
foreach ($tmp as $key => $value){
echo "<font color = red>$key $value</font><br>";}
echo "<br>";
//#5
function desc($a, $b){
if ($a ==$b) {
return 0;}
return ($b < $a) ? -1 : 1;}
$tmp = $days_in_month;
uasort($tmp, "desc");
foreach ($tmp as $key => $value){
echo "<font color = green>$key $value</font><br>";}
echo "<br>";
//#6
$derp = array("blue", "red", "green");
foreach ($days_in_month as $key => $value){
$i = ($i + 1) % 3;
echo "<font color = $derp[$i]>$key $value</font><br>";}
echo "<br>";
//#7
foreach ($days_in_month as $key => $value){
if($value == 31){
echo "<font color = orange>$key $value</font><br>";}}
echo "<br>";
//#8
foreach ($seasons as $key => $value){
echo "$key<br>";
foreach ($value as $key => $value){
foreach ($value as $key => $value){
echo "$value ";}}
echo "<br>";}
?>
