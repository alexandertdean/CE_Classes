<?php
	$state_fd = fopen("/home/pi/Documents/ECE477_repo/Project/FinalCode/alarm_state.txt", "r");
	$state = fread($state_fd, "1");
	fclose($state_fd);
	echo $state;
?>
