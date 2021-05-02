<!DOCTYPE html>
<?php
	$tablename = "datalog";
	$titlepage = "Humidity Temperature";
	
	session_name('humitemp');
	session_start();
	include("config-iot.php");
	date_default_timezone_set("Asia/Kuala_Lumpur");
	define('BASE_URL','http://hasrulnizam.com');
	extract($_GET);
	
	if(isset($_GET['set']) && isset($_GET['id']))
	{
		$sql = "SELECT *FROM ".$tablename." WHERE status='humitempDesc' AND id='$id' LIMIT 1";
		$result = mysqli_query($conn, $sql);
		if($row = mysqli_fetch_array($result))
		{
			$_SESSION['id']=$id;
			$_SESSION['description']=$row['description'];
		}
		else
		{
			$_SESSION['id']='';
			$_SESSION['description']='';
		}
		?>
		<script>
		self.location='dht22-data.php'; 
		</script>
		<?php
	}
	else
	{
		if(isset($_SESSION['id']) && $_SESSION['id']!='') $id=$_SESSION['id'];
	}
?>
<html>
<head>
	<title>Humidity Temperature</title>
	<!-- jquery.com -->
	<script src="https://ajax.googleapis.com/ajax/libs/jquery/1.12.2/jquery.min.js"></script>
	<!-- getbootstrap.com -->
	<link rel="stylesheet" href="http://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap.min.css">
	<script src="http://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/js/bootstrap.min.js"></script>
	<!-- datatables.net -->
	<link rel="stylesheet" href="https://cdn.datatables.net/1.10.12/css/jquery.dataTables.min.css">
	<script src="https://cdn.datatables.net/1.10.12/js/jquery.dataTables.min.js"></script>
	<!-- canvasjs.com  -->
	<script src="http://canvasjs.com/assets/script/canvasjs.min.js"></script>
	
	<!-- <script>
	var xmlhttp = new XMLHttpRequest();
	var url = '<?php echo BASE_URL ?>'+'/iot/humidity-temperature-data.php?id='+'<?php echo $id ?>';
	
	setInterval(function(){	
		var dataHumidity = [], dataTemperature = [];
		xmlhttp.onreadystatechange=function(){
			if (xmlhttp.readyState == 4 && xmlhttp.status == 200){				
				var arr = JSON.parse(xmlhttp.responseText);
				var i,end,last=70;
				
				end=arr.length;
				if(end>last) end=last;

				for(i=end-1;i>=0;i--){
					dataHumidity.push({ label:arr[i].time, y:parseFloat(arr[i].humidity) });
					dataTemperature.push({ label:arr[i].time, y:parseFloat(arr[i].temperature) });
				}
			}
		}
		xmlhttp.open("GET", url, false);
		xmlhttp.send();
	}, 1000);
	</script> -->
	
	<script>
		$(document).ready(function() {
			$('#table1').DataTable( {
				"paging":   true,
				"info":     true
			} );
		} );
	</script>
</head>

<body>
	<div class="container">
	
	<div>
		<h2 align="center">Humidity Temperature</h2>
	</div>	
		<?php
			if(isset($_SESSION['id']) && $_SESSION['id']!=""){ 
			$sql = "SELECT *FROM ".$tablename." WHERE status='humitempData' AND id='$id' ORDER BY timestamp DESC LIMIT 5000";
			$result = mysqli_query($conn, $sql);
			if($result)
			{
				?>
				<center>
				<table id="table1" class="display table-bordered" width="100%">
				<thead>
					<tr>
						<th style="text-align: center;">No</th>
						<th style="text-align: center;">Date & Time</th>
						<th style="text-align: center;">Humidity (%)</th>
						<th style="text-align: center;">Temperature (°C)</th>
					</tr>
				</thead>
				<tbody>

				<?php
				$no = 0;
				while($row = mysqli_fetch_array($result)) 
				{	
					$no = $no +1;
					echo "<tr>";
					echo "<td>";
					echo "<center>".$no."</center>";
					echo "</td>";
					echo "<td>";
					$date=date("d/m/y h:i:sA",$row["timestamp"]);
					echo "<center>".$date."</center>";
					echo "</td>";
					echo "<td>";
					echo "<center>".$row["humidity"]."</center>";
					echo "</td>";
					echo "<td>";
					echo "<center>".$row["temperature"]."</center>";
					echo "</td>";
					echo "</tr>";
				}
				echo "</tbody>";
				echo "</table></center>";
			}
			mysqli_close($conn);
		} ?>
		
	</div>
</body>
</html>