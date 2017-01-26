#!/usr/local/bin/php
<?php
	$db = new SQLite3('/home/pi/Documents/ECE331/templogger/temp.db');				#opens SQLite3 database or creates it if it doesn't exist (SHOULD NOT HAPPEN)
	if (!$db) {											
		echo $db->lastErrorMsg();
	}
	parse_str(implode('&', array_slice($argv, 1)), $_GET);						#separates command line arguments such that if a=1 is passed, $_GET['a'] will be 1
	$date = new DateTime('now');									#gets current date and time
	$ret = $db->exec('INSERT INTO DATA VALUES (NULL, ' . $_GET['temp'] . ', \'' . $date->format('m/d/Y') . '\', \'' . $date->format('H:i') . '\');');	#stores passed temperature into SQLite3 database at current date and time
	if(!$ret) {
		echo $db->lastErrorMsg();
	}
	$db->close();	
?>
