<?php
	$filename = '/home/pi/Documents/ECE477_repo/Project/FinalCode/log.txt';
	$contents = file($filename);
	
	$i = 0;
	foreach($contents as $line) {
		$body[$i] = $line;
		$i = $i + 1;
	}
	echo json_encode($body);
?>
