<?php
	
	//open named pipe
	//http://stackoverflow.com/questions/13124557/writing-to-a-pipe-in-linux-using-php
	$out = fopen("/tmp/bryan", "w");

	//Parse which button was pressed
	//http://stackoverflow.com/questions/2680160/how-can-i-tell-which-button-was-clicked-in-a-php-form-submit
 	if ($_SERVER['REQUEST_METHOD'] === 'POST'){

		if(isset($_POST['node_3_down'])){
			fwrite($out, "501");
		}
		elseif(isset($_POST['node_2_up'])){
			fwrite($out, "400");
		}
		elseif(isset($_POST['node_2_down'])){
			fwrite($out, "431");
		}
		elseif(isset($_POST['node_1_up'])){
			fwrite($out, "300");
		}
		elseif(isset($_POST['car_3'])){
			fwrite($out, "205");
		}
		elseif(isset($_POST['car_2'])){
			fwrite($out, "204");
		}
		elseif(isset($_POST['car_1'])){
			fwrite($out, "203");
		}
		elseif(isset($_POST['door_open'])){
			fwrite($out, "211");
		}
		elseif(isset($_POST['door_close'])){
			fwrite($out, "210");
		}
		else{
			fwrite($out, "error");
			echo "error";
		}
	}

	//reload main control page
	//http://stackoverflow.com/questions/22745425/how-to-return-to-the-previous-page-after-a-submit-form-with-php

	fclose($out);

?>

<script>
     window.history.go(-1);
 </script>
