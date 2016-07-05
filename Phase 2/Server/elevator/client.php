
<!doctype html>

<html lang="en">

<!-- Define visual cues with CSS -->
<style>

	body	{

	}

	table	{
		border: 1px solid black;
	}

	th, td	{
		border-left: 1px solid black;
		border-bottom: 1px solid black;
	}

	#control_interface {
		border: 1px solid black;
		max-width: 600px;
		min-height: 300px;
		display: table;
	}

	.control {
		text-align: left;
		border: 1px solid black;
		max-width: 200px;
	}

	.control_wrapper {
		border: 1px solid black;
		width: 50%;
	}

	#current_info {
		border: 1px solid black;
		display: inline-block;
	}

	#log {
		border: 1px solid black;
		display: inline-block;
		width: 700px;
	}

	#log_frame {
		height: 544px;
		width: 100%;
	}

</style>

<!-- File Header -->
<head>

<title>Elevator Control</title>
</head>

<body>

<?php
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


<!---------------------------Control Interface------------------------------>
<h1>Elevator Control and Logging</h1>

<form action="control.php" method="post">
	<div id="control_interface">
		<div style="display: table-row;">
			<div id="floor_node_controls" class="control_wrapper" style="display: table-cell;">	
				<h3>Floor Node Summons</h3>
				<div class="control">
					<button type="submit" name="node_3_down" id="node_3_down" value="node_3_down">▼</button>3rd Floor
				</div>
				<div class="control">
					<button type="submit" name="node_2_up" id="node_2_up" value="node_2_up">▲</button><br />
					<button type="submit" name="node_2_down" id="node_2_down" value="node_2_down">▼</button> 2nd Floor
				</div>
				<div class="control">
					<button type="submit" name="node_1_up" id="node_1_up" value="node_1_up">▲</button>1st Floor
				</div>
			</div>

			<div id="car_controls" class="control_wrapper" style="display: table-cell;">
				<h3>Car Controls</h3>
				<div class="control">
					<button type="submit" name="car_3" id="car_3" value="car_3">Floor 3</button>
				</div>		
				<div class="control">
					<button type="submit" name="car_2" id="car_2" value="car_2">Floor 2</button>
				</div>
				<div class="control">
					<button type="submit" name="car_1" value="car_1" value="car_1">Floor 1</button>
				</div>
				<div class="control">
					<button type="submit" name="door_open" id="door_open" value="door_open">Open Door</button>
				</div>
				<div class="control">
					<button type="submit" name="door_close" id="door_close" value="door_close">Close Door</button>
				</div>
			</div>
		</div>
	</div>
</form>
<br />
<br />
<!-------------------------------------------------------------------------->


<!-------------------------------Current Info------------------------------->


<!-------------------------------------------------------------------------->


<!--------------------Current Info & Log display table---------------------->
<!--code based on http://stackoverflow.com/questions/6976092/how-to-refresh-an-iframe-in-php-page-->
<div id="log">
	<h3>CAN Event Log</h3>
	<!-- <button type="button" name="clear_log" value="clear_log">Clear</button> -->
	<!--Command display table is in an iframe that refreshes every half second, to keep the command printout updated-->
	<script type="text/javascript">
	    var timer;
	    function refreshIframe(){
	    if(timer)
	    clearInterval(timer)
	    timer = setTimeout(refreshIframe,500)
	    var iframe = document.getElementById('log_frame'); 
	    iframe.src='log.php';
	    }

	    refreshIframe();
	</script>
	<iframe id="log_frame" src="log.php" width="100%" height="300px";>
	  <p>Your browser does not support iframes.</p>
	</iframe>

</div>

<!--------------------------------------------------------------------------->



</body>
</html>
