<?php
	$arm = "A";
	$disarm = "D";
	if ($_POST["armed"] == "1") {
		$state_fd = fopen("/home/pi/Documents/ECE477_repo/Project/FinalCode/alarm_state.txt", "w+");
		fwrite($state_fd, $arm);
		fclose($state_fd);
	} else {
		$state_fd = fopen("/home/pi/Documents/ECE477_repo/Project/FinalCode/alarm_state.txt", "w+");
		fwrite($state_fd, $disarm);
		fclose($state_fd);
	}
?>
