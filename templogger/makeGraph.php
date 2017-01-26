<?php
	$date = new DateTime('now');								#gets current date and time
	$db = new SQLite3("/home/pi/Documents/ECE331/templogger/temp.db");			#opens database of temperature values
	header("Content-type: image/png");							#tells the HTML page to prepare for a PNG image
	$img = imagecreate(1600, 800);								#creates template for 1600x800 pixel image ((Obviously not supported by all systems))
	imagecolorallocate($img, 255,255,255);							#sets background of image to white
	$prevTemp = 0;										#initializes previous temperature value
	$linecolor = imagecolorallocate($img, 0, 0, 0);						#initializes black color for use in grid and labels
	$faren = imagecolorallocate($img, 255, 0, 0);						#initializes red color for use in graph


	imageline($img, 1520, 0, 1520, 700, $linecolor);					#create right border of graph area
	imageline($img, 80, 0, 80, 700, $linecolor);						#creates left border of graph area
	imageline($img, 80, 700, 1520, 700, $linecolor);					#creates bottom border of graph area
	imagestring($img, 5, 760, 780, "Time", $linecolor);					#labels x-axis
	imagestring($img, 5, 1, 400, "Temp.", $linecolor);					#labels y-axis
	imagestring($img, 5, 1, 420,  '(F)', $linecolor);					#y-axis label unit
	imagestring($img, 5, 540, 10, "Temperature in Alex's Room over the Last 24 Hours", $linecolor);					#sets title of graph
	$i = 0;																
	
	imagestring($img, 4, 1525, 700, "NOW", $linecolor);
	$tempDate = new DateTime('now');												#get current date and time that will not be used for graphing
	do {
		imageline($img, 1520 - (60 * $i) - $date->format('i'), 700, 1520 - (60 * $i) - $date->format('i'), 720, $linecolor);	#sets notches on x-axis for each hour
		imagestring($img, 4, 1520 - (60 * $i) - $date->format('i') - 20, 730, $tempDate->format('H') . ':00', $linecolor);	#labels notches with 24-hour time
		$tempDate->modify('-1 hour');												
		$i++;
	} while ((1520 - (60 * $i) - $date->format('i')) > 80);										#continues until left border is reached

	for ($i = 0; $i < 10; $i++) {
		imageline($img, 80, ($i + 1) * 80 - 100, 60, ($i + 1) * 80 - 100, $linecolor);						#sets notches on y-axis for temps in increments of 10
		imagedashedline($img, 80, ($i + 1) * 80 - 100, 1520, ($i + 1) * 80 - 100, $linecolor);					#gridlines for easier reading
		imagestring($img, 4, 40, ($i + 1) * 80 - 110, (9 - $i ) * 10, $linecolor);						#labels notches with Farenheit temperature
	}

	for ($i = 0; $i < 1440; $i++) {													#24 hours * 60 minutes
		$return = $db->query('SELECT * FROM data where date=\'' . $date->format('m/d/Y') .'\' and time=\'' . $date->format('H:i')  . '\';');		#gets row from database for provided date and time
		if ($return != FALSE) {	
			$data = $return->fetchArray();											#separates datebase returned row into an array
			if ($data != FALSE) {
				$x1 = 1520 - $i;											#shifts x coordinates each iteration
				$x2 = 1520 - ($i + 1);
				if ($prevTemp != 0) {											#will not make a line if last read was unsuccessful. If this is ever used in winter, will error at freezing temperature		
					imageline($img, $x2, 700 - $data[1] * 8, $x1, 700 - $prevTemp * 8, $faren);			#makes a line from provided x values and previous and current temperature values
				}
				$prevTemp = $data[1];
			} else {
			$prevTemp = 0;
			}
		}
		$date->modify('-1 minute');												#decrements one minute in time
	}
	imagepng($img);															#creates the image
	imagedestroy($img);														#frees memory space from image
?>
