<?php
	$servername = "localhost";
	$username = "hasrul";
	$password = "123456"; 
	$dbname = "esp32_dht"; 
	$conn = mysqli_connect($servername, $username, $password, $dbname);
	if(mysqli_connect_errno()) die("Error : " . mysqli_connect_error());
?>