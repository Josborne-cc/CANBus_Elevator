<?php
//CANLookup()
//determine the meaning of a CAN-bus packet
//output: string, either the meaning of the command or unknown if not found
function CANLookup($row){
	$floor = 0;
	//Node instruction is for
	switch($row["Node"]){
		case '0':
			$command = "Controller - Deprecated";
			break;

		case '1':
			$command = "PID - Deprecated";
			return $command;

		case '2':
			$command = "Car";
			//elevator car specific instructions
			switch($row["Inst"]){
				case '0':
					$command .= " - Floor Button";
					//Floor button CMDs
					switch($row["CMD"]){
						case '0':
							$command .= " - On";
							break;
						case '1':
							$command .= " - Off";
							break;
						default:
							$command .= " - Unknown";
							return $command;
					}
					break;

				case '1':
					$command .= " - Door";
					//Door CMDs
					switch($row["CMD"]){
						case '0':
							$command .= " - Close";
							break;
						case '1':
							$command .= " - Open";
							break;
						default:
							$command .= " - Unknown";
							return $command;
					}
					break;
				
				case '2':
					$command .= " - Floor Indicator";
					//Floor Indicator CMDs
					switch($row["CMD"]){
						case '0':
							$command .= " - On";
							break;
						case '1':
							$command .= " - Off";
							break;
						case '3':
							$command .= " - Car at Floor 1";
							break;
						case '4':
							$command .= " - Car at Floor 2";
							break;
						case '5':
							$command .= " - Car at Floor 3";
							break;
						default:
							$command .= " - Unknown";
							return $command;
					}
					break;

				case '3':
					$command .= " - Emergency Stop";
					//Emergency Stop CMDs
					switch($row["CMD"]){
						case '0':
							$command .= " - On";
							break;
						case '1':
							$command .= " - Off";
							break;
						default:
							$command .= " - Unknown";
							return $command;
					}
					break;

				case '4':
					$command .= " - Button Indicator - On";
					break;

				case '5':
					$command .= " - Button Indicator - Off";
					break;

				default:
					$command .= " - Unknown";
					return $command;
				}
			break;

		case '3':
			$floor = 1;
			$command = "Floor 1";
			break;			

		case '4':
			$floor = 2;
			$command = "Floor 2";
			break;

		case '5':
			$floor = 3;
			$command = "Floor 3";
			break;

		default:
			$command = "Unknown";
			return $command;
		}

	//floor specific commands
	if($floor > 0){	
		switch($row["Inst"]){
			case '0':
				$command .= " - Call Switch 1";
				//Call Switch CMDs
				switch($row["CMD"]){
					case '0':
						$command .= " - Up";
						break;
					case '1':
						$command .= " - Down";
						break;
					default:
						$command .= " - Unknown";
						return $command;
				}
				break;
			case '1':
				$command .= " - Call LED 1";
				//Call LED CMDs
				switch($row["CMD"]){
					case '0':
						$command .= " - Off";
						break;
					case '1':
						$command .= " - On";
						break;
					default:
						$command .= " - Unknown";
						return $command;
				}
				break;
			case '2':
				$command .= " - Floor Indicator";
				//Floor Indicator CMDs
				switch($row["CMD"]){
					case '0':
						$command .= " - Off";
						break;
					case '1':
						$command .= " - On";
						break;
					case '3':
						$command .= " - Car at Floor 1";
						break;
					case '4':
						$command .= " - Car at Floor 2";
						break;
					case '5':
						$command .= " - Car at Floor 3";
						break;
					default:
						$command .= " - Unknown";
						return $command;
				}
				break;
				case '3':
				$command .= " - Call Switch 2";
				//Call Switch CMDs
				switch($row["CMD"]){
					case '0':
						$command .= " - Up";
						break;
					case '1':
						$command .= " - Down";
						break;
					default:
						$command .= " - Unknown";
						return $command;
				}
				break;
			case '4':
				$command .= " - Call LED 2";
				//Call LED CMDs
				switch($row["CMD"]){
					case '0':
						$command .= " - Off";
						break;
					case '1':
						$command .= " - On";
						break;
					default:
						$command .= " - Unknown";
						return $command;
				}
				break;
			default:
				$command .= " - Unknown";
				return $command;
				break;
		}
	}
	return $command;
}

//CurrentFloor()
//Returns floor value based on CMD value of Floor Indicator message sent to Elevator Car
function CurrentFloor($cmdval){
	switch($cmdval){
		case 3:
			return 1;
		case 4:
			return 2;
		case 5: return 3;
		default: return "Unknown";
	}
}
?>
