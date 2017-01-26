<html>
<head>
<title>Temperature Log</title>
</head>
<body>
<?php
	header('Content-type: image/png');
	$png_image = imagecreate(150, 150);
	$imagecolorallocate($png_image, 15, 142, 210);
	imagepng($png_image)
	imagedestroy($png_iamge);
?>
</body>
</html>
