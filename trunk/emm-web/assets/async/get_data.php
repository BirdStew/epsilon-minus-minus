<?php
define('OUTPUT_PATH','../files');
define('PROGRAM_PATH','../core/emm-core');


$wordLen = $_POST['w'];
$parityLen = $_POST['p'];
$parityType = $_POST['pyt'];
$errorProb = $_POST['e'];
$offset = 54; // for birmaps
$messagePath = '../core/demo.bmp';


$args = "-w $wordLen -p $parityLen -t $parityType -e $errorProb -s $offset -o ".OUTPUT_PATH." $messagePath";

$args = escapeshellcmd($args);
echo PROGRAM_PATH." $args";
exec(PROGRAM_PATH." $args");
?>
