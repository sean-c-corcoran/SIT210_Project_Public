<?php
$username = "Your Username";
$password = "Your Password";
$database = "Your Database";

$mysqli = new mysqli("localhost", $username, $password, $database);


$field1 = $mysqli->real_escape_string($_POST['field1']);
$field2 = $mysqli->real_escape_string($_POST['field2']);


$query = "INSERT INTO devicesTest (deviceID, friendlyName)
            VALUES ('{$field1}','{$field2}')";

$mysqli->query($query);
$mysqli->close();
echo "<b> <center>Device Created!</center> </b> <br> <br>";
?>
<?php
// change to reflect your correct IP/DNS address
header("Location: //192.168.1.187/");
exit;
?>