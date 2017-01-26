<?php
	$email_fd = fopen("email.txt", "w+");
	fwrite($email_fd, $_POST);
	fclose($email_fd);
?>