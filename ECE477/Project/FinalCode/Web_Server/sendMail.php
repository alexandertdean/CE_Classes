<?php
	$to = "alexander.dean94@gmail.com";
	$sub = "Alarm Triggered";
	$txt = "Your alarm has been triggered!";
	$headers = "From: alexander.dean94@gmail.com";
	mail($to, $sub, $txt, $headers);
?>
