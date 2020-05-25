<?php
$mysqli = new mysqli("localhost", "your username", "password", "database name");
if($mysqli->connect_error) {
  exit('Could not connect');
}


$sql = "SELECT deviceID FROM devicesTest 
LIMIT 1 OFFSET ?"; 

$stmt = $mysqli->prepare($sql);
$stmt->bind_param("s", $_GET['q']);
$stmt->execute();
$stmt->store_result();
$stmt->bind_result($deviceID);
$stmt->fetch();
$stmt->close();

echo $deviceID;
?>