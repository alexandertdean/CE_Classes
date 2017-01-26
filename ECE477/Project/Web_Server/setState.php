<?php
	$arm = "A";
	$disarm = "D";
	if (isset($_POST["armed"])) {
		$state_fd = fopen("alarm_state.txt", "w+");
		fwrite($state_fd, $arm);
		fclose($state_fd);
	} else {
		$state_fd = fopen("alarm_state.txt", "w+");
		fwrite($state_fd, $disarm);
		fclose($state_fd);
	}
?>
