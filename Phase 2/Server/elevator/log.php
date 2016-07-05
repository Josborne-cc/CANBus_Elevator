<!doctype html>

<html lang="en">

<style>
	#current_info {
		font-size: large;
}
</style>

<?php

include 'functions.php';
//Set variables
/*	
	$HostName = "localhost";
	$DBName = "elevator";
	$LogTable = "Logging";
	$MYSQLUserName = "client";
	$MYSQLUserPass = "password";
*/
	$HostName = "localhost";
	$DBName = "CAN";
	$LogTable = "Logging";
	$MYSQLUserName = "user";
	$MYSQLUserPass = "password";
?>


	<?php
		//attach to database
		$con = mysqli_connect($HostName, $MYSQLUserName, $MYSQLUserPass, $DBName);

		//check if logging table exists
		//if(mysql_num_rows(mysqli_query("SHOW TABLES LIKE '".$LogTable."'"))!=1){
		//	echo "Log table not found";
		//	die();
		//}
	
		//Update Current floor:
		$result = mysqli_query($con, "SELECT * FROM Logging WHERE Node = 3 AND Inst = 2");
		$curfloor = "Unknown";		
		while($row = mysqli_fetch_array($result)){
			$curfloor = $row["CMD"];
		}		
		echo "<div id=\"current_info\"> Current floor: <b>" . CurrentFloor($curfloor) . "</b></div><br />";

		//Populate list of logs in a table:
		$result = mysqli_query($con, "SELECT * FROM Logging ORDER BY id DESC LIMIT 20");
		echo "<table><tr><th>ID</th><th>Node</th><th>Instruction Type</th><th>Command</th><th>Diagnostic Logging</th></tr>";
		while($row = mysqli_fetch_array($result)){
			echo "<tr>";
			$temp = 1;
			//foreach($row as $field){
			//	echo "<td>" . ' field $num ' . htmlspecialchars($field) . "|</td>";
			//	$temp++;
			//}
			echo "<td>" . $row["id"] . "</td><td>  " . $row["Node"] . "</td> <td>" . $row["Inst"] . "</td> <td>" . $row["CMD"] . "</td>";
			echo "<td>" . CANLookup($row) . "</td>";
			echo "</tr>";
		}
		echo "</table>";

		mysqli_close($con);	//clean up database connection
	?>

</html>

