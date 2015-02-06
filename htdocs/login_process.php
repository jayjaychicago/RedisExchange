<?php
session_start();
//ini_set('display_errors',1);
//error_reporting(E_ALL);

function connect_redis() {
        //echo "Connecting to redis...";
        require_once 'predis/autoload.php';

        $single_server = array('host' => '127.0.0.1', 'port' => 6379, 'database' => 15);

        $multiple_servers = array( array('host' => '127.0.0.1', 'port' => 6379, 'database' => 15, 'alias' => 'first', ), array('host' => '127.0.0.1', 'port' => 6380, 'database' => 15, 'alias' => 'second', ), );

        // simple set and get scenario
        $time_start = microtime(true);
        $_SESSION["client"] = new Predis\Client($single_server);
}

connect_redis();


	include "instabid.php";
        $email = $_POST["email"];
        $password = $_POST["password"];

	$userExists = $_SESSION["client"]->SISMEMBER("INSTABID_USERS", $email);

	if ($userExists == 0) {
		echo '{"result":"FAIL","comment":"This user email doesn\'t exist!"}';
		die();
	}
	$password_check = $_SESSION["client"]->GET("INSTABID_PASSWORD:" . $email);
	if ($password != $password_check) {
		echo '{"result":"FAIL","comment":"Wrong password"}';
	}
	else {
			authorize_user_to_bid('admin');
			$_SESSION["email"] = $email;
			$exchangeName = $_SESSION["client"]->GET("FIRSTEXCHANGE:" . $email);
        		$productName = $_SESSION["client"]->GET("FIRSTPRODUCT:" . $email);
			echo '{"result":"SUCCESS","comment":"Logged in","exchange":"' . $exchangeName . '","product":"' . $productName . '"}';
	}
?>
