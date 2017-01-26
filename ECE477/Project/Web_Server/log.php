<!DOCTYPE HTML>
<html>
<head>
	<title>Alarm Log</title>

	<link rel = "stylesheet" type = "text/css" href = "https://bootswatch.com/yeti/bootstrap.min.css">
</head>
<body>
<nav class = "navbar navbar-default navbar-fixed-top">
	<div class = "container">
		<span class = "navbar-header">
	
		</span>
	</div>
</nav>
<div span style="color:#FFF;text-align:center;"><h1>ECE 477 Project Server</h1></div>
<div span style="color:#FFF;text-align:center;"><h1>Log of Alarm Information</h1></div>
<form align="center" action="" method="POST">
    <input type="submit" name="arm" value="Turn on Alarm">
    <input type="submit" name="off" value="Turn off Alarm">
    <br></br>
</form>

<?php
$open = fopen("/home/pi/Documents/ECE477_repo/Project/FinalCode/alarm_state.txt",'w+');
$arm = "A";
$off = "D";
if (isset($_POST["arm"]))
{
   	if(fwrite($open,$arm)){

   		echo "<p align='center'><font color=blue  size='6pt'>***Alarm Armed***</font> </p>";
   	}
}
if (isset($_POST["off"]))
{
   if(fwrite($open,$off)){ 
        echo "<p align='center'> <font color=blue  size='6pt'>***Alarm Disarmed***</font> </p>";
   }
}
fclose($open);
$_POST = array();
?>

<form style='text-align:center' method="post" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]);?>">
   <input type=button onClick="location.href='setup.php'" value='Update E-Mail Information'>
</form>


</body>
</html>


<html>
<body>
<p></p>
<table align="center" border="3" width="1000" height="150" bordercolor="blue" bgcolor="white">
<tr>
<th>Date</th>
<th>Time</th>
<th>Status</th>
</tr>
<tr><td align="center">
<?php
echo date("m-d-y")."<br>";
?>
</td><td align="center">
<?php
echo date("H:i:s", time())."<br>";
?>

</td><td align="center">
//<?php
//$f = fopen("test_file", "r");
// Read line by line until end of file
//while(!feof($f)) {
//    echo fgets($f) . "<br />";
//}

fclose($f);
?>
</td></tr>
<tr><td>       </td><td>            </td><td>         </td></tr>
</table>
</body>
</html>
