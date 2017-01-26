<?php
	$state_fd = fopen("alarm_state.txt", "r+");
	$state = fscanf($state_fd, "%c");
	echo $state;
	fclose($state_fd);
?>