<!DOCTYPE HTML>
<html>
<head>
<title> ECE 331 Calendar </title>
<!-- Latest compiled and minified CSS -->
<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap.min.css" integrity="sha384-1q8mTJOASx8j1Au+a5WDVnPi2lkFfwwEAa8hDDdjZlpLegxhjVME1fgjWPGmkzs7" crossorigin="anonymous">

<!-- Optional theme -->
<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap-theme.min.css" integrity="sha384-fLW2N01lMqjakBkx3l/M9EahuwpSfeNvV63J5ezn3uZzapT0u7EYsXMjQV+0En5r" crossorigin="anonymous">

<!-- Latest compiled and minified JavaScript -->
<script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/js/bootstrap.min.js" integrity="sha384-0mSbJDEHialfmuBBQP6A4Qrprq5OVfW37PRR3j5ELqxss1yVqOtnepnHVP9aJ7xS" crossorigin="anonymous"></script>
</head>
<body style = 'background:#003366'>
<div class = 'container'>
<form action = "index.php" method = 'post'>
<div class = 'row'>
<div class = 'col-xs-4' style = 'padding:20px'>
<div  class = "input-group">
<?php
if (array_key_exists('year', $_POST)) {
	$x = $_POST['year'];
	print "<INPUT placeholder = 'Year' type = 'text' name = 'year' value = '$x' class='form-control' />\n";
} else {
	print "<INPUT placeholder = 'Year' type = 'text' name = 'year' class = 'form-control' />\n";
}
?>
<span class = 'input-group-btn'>
<input type = 'submit' class = 'btn btn-primary'>
</span>
</div>
</div>
</div>
</form>

<?php
if (array_key_exists('year', $_POST)) {
	calgen($_POST['year']);
}


function calgen($year)
{
	$date = new DateTime('now');
	$cal = new DateTime($year . '-01-01');
	$curMonth = $date->format('F');
	$curDay = $date->format('j');
	$curYear = $date->format('Y');
	print "<div class = 'row'>\n";
	for ($j = 0; $j < 12; $j++) {
		$mon = $cal->format('F');
		$dayOfWeek = $cal->format('w');
		$cal->sub(new DateInterval('P' . $dayOfWeek . 'D'));
		print "<div class = 'col-xs-4' style = 'height:350px'>\n";
		print "<table class = 'table table-bordered' style = 'background:#FFFFFF'>\n";
		print "<tr><th colspan = 7 class = 'bg-primary text-center'>$mon</tr>\n";
		print "<tr><th>Sun</th><th>Mon</th><th>Tue</th><th>Wed</th><th>Thu</th><th>Fri</th><th>Sat</th></tr>\n";
		for ($i = 0; $i < (7 * 6); $i++) {
			if ($i%7 == 0) {
				print "<TR>\n";
			}
			$tempMon = $cal->format('F');
			$calDay = $cal->format('j');
			if ($tempMon == $mon) {
				if ($mon == $curMonth && $calDay == $curDay && $year == $curYear) {
					print "<td align = 'center' class = 'bg-info'> $calDay\n";
				} else {
					print "<td align = 'center'> $calDay\n";
				}
			} else {
				if ($calDay == 1) {
					$dayOfWeek = $cal->format('w');
					if ($dayOfWeek != 0) {
						for ($dayOfWeeek; $dayOfWeek < 7; $dayOfWeek++) {
							print "<td> </td>";
						}
					}
					break;
				}
				print "<td align = 'center'> \n";
			}
			$cal->add(new DateInterval('P1D'));
			if ($i%7 == 6) {
				print "</tr>\n";
			}
		}
		print "</table>\n";
		print "</div>\n";
	}
	print "</div>\n";
}
?>
</div>
</body>
</html>
