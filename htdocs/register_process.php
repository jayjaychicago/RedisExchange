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
        $password2 = $_POST["password2"];

	if ($password != $password2) {
		echo "FAIL: You did not type the same password twice!";

		die();
	}
	$alreadyThere = $_SESSION["client"]->sadd("INSTABID_USERS", $email);
	if ($alreadyThere == 0) {
		echo "FAIL: User already exists";
	}
	else {
		$_SESSION["client"]->SET("INSTABID_PASSWORD:" . $email, $password);
		$exchangeName = rand(10000000,100000000000000);
		$result = newExchange($email,$password,$exchangeName);
		$j = json_decode($result, true);
		if ($j["result"] != "FAIL") {
			authorize_user_to_bid('admin');
			$privateKey = $j["PrivateKey"];
			$_SESSION["privateKey"] = $privateKey;
			$_SESSION["email"]=$email;
			$_SESSION["exchangeName"] = $exchangeName;
			$productName = 'test';
			$newProduct = newProduct($exchangeName,$productName);
			$jp = json_decode($newProduct, true);
			if ($jp["result"] != "FAIL") {
				$products = array($productName);
				$_SESSION["products"] = array($productName);
				$_SESSION["client"]->SET("FIRSTEXCHANGE:" . $email, $exchangeName);
				$_SESSION["client"]->SET("FIRSTPRODUCT:" . $email, $productName);
			}
			//header( 'Location: dashboard_instabid.php' ) ; 
			echo "SUCCESS:?exchange=" . $exchangeName . "&product=" . $productName ;
			//echo $newProduct;
			//echo "SUCCESS";
		}
		else {
			echo "SUCCESS";
			//header( 'Location: /register.php?msg=' + $j["msg"] ) ; 
		}
	}
?>
