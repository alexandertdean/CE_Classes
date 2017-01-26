<!DOCTYPE HTML>
<html>
<head>
<style>
.error {color: #FF0000;}
</style>
</head>
<body bgcolor="#2E4172">

<?php
// define variables and set to empty values
$emailErr = "";
$email = "";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
   if (empty($_POST["email"])) {
     $emailErr = "Email is required";
   } else {
     $email = test_input($_POST["email"]);
     // check if e-mail address is well-formed
     if (!filter_var($email, FILTER_VALIDATE_EMAIL)) {
       $emailErr = "Invalid email format";
     }
   }
}

function test_input($data) {
   $data = trim($data);
   $data = stripslashes($data);
   $data = htmlspecialchars($data);
   return $data;
}
?>
<div span style="color:#FFF;text-align:center;"><h1>ECE 477 Project Server</h1></div>
<div span style="color:#FFF;text-align:center;"><h1>Setup Page</h1></div>

<div style='text-align:center'><p><span class="error">* Required Field.</span></p></div>
<form span style="color:#FFF;text-align:center;" method="post" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]);?>">
   E-Mail: <input type="text" name="email" value="<?php echo $email;?>">
   <span class="error">* <?php echo $emailErr;?></span>
   <br><br>
   <input type="submit" name="submit" value="Update E-Mail">
   <input type=button onClick="location.href='log.php'" value='Go to Alarm Log'>
</form>

<?php
$open = fopen("email",'w+');
$new = "\n";

if(fwrite($open,$email)){
   fwrite($open,$new);
}
?>

</body>
</html>

