<?php
	$email_fd = fopen("/home/pi/Documents/ECE477_repo/Project/FinalCode/email.txt", "w+");
	fwrite($email_fd, $_POST);
	fclose($email_fd);
?>
