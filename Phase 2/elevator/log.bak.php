<?php

//include parse function
//include 'functions.php';
//Set variables
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
	
	//Populate list of logs in a table:
		$result = mysqli_query($con, "SELECT * FROM Logging ORDER BY id DESC LIMIT 20");
		echo "<table><tr><th>Node</th><th>Instruction Type</th><th>Command</th></tr>";
		while($row = mysqli_fetch_array($result)){
			echo "<tr>";
			$temp = 1;
			//foreach($row as $field){
			//	echo "<td>" . ' field $num ' . htmlspecialchars($field) . "|</td>";
			//	$temp++;
			//}
			echo "<td>" . $row["Node"] . "</td> <td>" . $row["Inst"] . "</td> <td>" . $row["CMD"] . "</td>";
			//print translation of command
			//echo "<td>" . CANLookup($row) . "</td>";
			echo "</tr>";	
			}
			
		}
		echo "</table>";

		mysqli_close($con);	//clean up database connection
	?>

