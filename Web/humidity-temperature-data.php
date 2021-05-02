<?php
	$tablename = "datalog";
	$titlepage = "Humidity Temperature";
	
	session_start();
	include("config-iot.php");
	date_default_timezone_set("Asia/Kuala_Lumpur");
	extract($_GET);
	
	if(!isset($_GET['timestamp'])) $timestamp=time();
	if(!isset($_GET['interval'])) $interval=1;
	if(!isset($_GET['id'])) $id="demo";

	$time=date("h:i:sA",$timestamp);
	
	if(isset($_GET['timestamp']) && !isset($_GET['description']) && !isset($_GET['data']))
	{
		echo time();
	}
	else if(isset($_GET['datetime']))
	{
		$outp = '[{"date":"'.date("d/m/y",$timestamp).'","time":"'.date("h:i:sA",$timestamp).'"}]';
		echo($outp);
	}
	else if(isset($_GET['description']))
	{
		$sql = "SELECT *FROM ".$tablename." WHERE status='humitempDesc' AND id='$id' LIMIT 1";
		$result = mysqli_query($conn,$sql);
		if($row = mysqli_fetch_array($result))
		{
			$sql = "UPDATE ".$tablename." SET timestamp='$timestamp',description='$description' WHERE status='humitempDesc' AND id='$id';";
			if(mysqli_query($conn,$sql) == TRUE) echo "Update at ".date("d/m/y h:i:sA",$timestamp);
		}
		else
		{
			$sql = "INSERT INTO ".$tablename." (status,id,timestamp,description) VALUES ('humitempDesc','$id','$timestamp','$description')";
			if(mysqli_query($conn,$sql) == TRUE) echo "Insert at ".date("d/m/y h:i:sA",$timestamp);
		}
 		$sql = "UPDATE ".$tablename." SET ";
		if(isset($_GET['temperature'])) $sql=$sql."temperature='yes',";
		else  $sql=$sql."temperature='',";
		if(isset($_GET['humidity'])) $sql=$sql."humidity='yes'";
		else  $sql=$sql."humidity=''";
		$sql=$sql." WHERE status='humitempDesc' AND id='$id';";
		mysqli_query($conn,$sql);
	}
	else if(isset($_GET['data']))
	{
		$sql = "SELECT *FROM ".$tablename." WHERE status='humitempDesc' AND id='$id' LIMIT 1";
		$result = mysqli_query($conn, $sql);
		if($row = mysqli_fetch_array($result))
		{
			$description=$row["description"];
			$sql = "SELECT *FROM ".$tablename." WHERE status='humitempData' AND id='$id' ORDER BY timestamp DESC LIMIT 1";
			$result = mysqli_query($conn, $sql);
			if($row = mysqli_fetch_array($result))
			{
				$last=$row["timestamp"];
				if(intval(($timestamp+8*60*60-1)/$interval)==intval(($last+8*60*60-1)/$interval))
				{
					$sql = "UPDATE ".$tablename." SET id='$id',timestamp='$timestamp'";
					if(isset($_GET['temperature'])) $sql=$sql.",temperature='$temperature'";
					if(isset($_GET['humidity'])) $sql=$sql.",humidity='$humidity'";
					$sql=$sql."WHERE status='humitempData' AND id='$id' AND timestamp='$last';";
					if(mysqli_query($conn, $sql) == TRUE) echo $description." update at ".date("d/m/y h:i:sA",$timestamp);
				}
				else
				{
					$sql = "INSERT INTO ".$tablename." (status,id,timestamp";
					if(isset($_GET['temperature'])) $sql=$sql.",temperature";
					if(isset($_GET['humidity'])) $sql=$sql.",humidity";
					$sql=$sql.") VALUES ('humitempData','$id','$timestamp'";
					if(isset($_GET['temperature'])) $sql=$sql.",'$temperature'";
					if(isset($_GET['humidity'])) $sql=$sql.",'$humidity'";
					$sql=$sql.")";
					if(mysqli_query($conn, $sql) == TRUE) echo $description." insert at ".date("d/m/y h:i:sA",$timestamp);
				}
			}
			else
			{
				$sql = "INSERT INTO ".$tablename." (status,id,timestamp";
				if(isset($_GET['temperature'])) $sql=$sql.",temperature";
				if(isset($_GET['humidity'])) $sql=$sql.",humidity";
				$sql=$sql.") VALUES ('humitempData','$id','$timestamp'";
				if(isset($_GET['temperature'])) $sql=$sql.",'$temperature'";
				if(isset($_GET['humidity'])) $sql=$sql.",'$humidity'";
				$sql=$sql.")";
				if(mysqli_query($conn, $sql) == TRUE) echo $description." insert at ".date("d/m/y h:i:sA",$timestamp);
			}
		}
		else
		{
			echo "ID Not Exist";
		}
	}
	else if(isset($_GET['delete']))
	{
		$sql = "DELETE FROM ".$tablename." WHERE status='humitempData' AND id='$delete'";
		if(mysqli_query($conn, $sql) == TRUE)
		{
			echo "Data Deleted"; 				
		}
		else echo "Error: " . $sql . "<br>" . mysqli_error($conn);
	}
	else if(isset($_GET['id']))
	{
		header("Access-Control-Allow-Origin: *");
		header("Content-Type: application/json; charset=UTF-8");
		
		$sql = "SELECT *FROM ".$tablename." WHERE status='humitempData' AND id='$id' ORDER BY timestamp DESC";
		$result = mysqli_query($conn, $sql);
		$outp = '[';
		while($row = mysqli_fetch_array($result))
		{
			if ($outp != '[') {$outp .= ',';}
			$outp .= '{"id":"' . $row['id'] . '",'; 
			$outp .= '"temperature":"' . $row['temperature'] . '",'; 
			$outp .= '"humidity":"' . $row['humidity'] . '",';
			$outp .= '"time":"' . date("h:i:sA",$row['timestamp']) . '",';
			$outp .= '"timestamp":"' . $row['timestamp'] . '"}';
		}
		$outp .=']';
		echo($outp);
	}
	mysqli_close($conn);
?>