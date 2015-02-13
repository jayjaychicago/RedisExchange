<?php
session_start();
//ini_set('display_errors',1);
//error_reporting(E_ALL);
// newExchange allows the creation of an exchange
// you have to place this library along with your other php libraries
// if you do a require_once "instabid.php" and it's not in the right spot
// the error message will tell you where to place it

function newExchange($email, $password, $name) {

$url = 'http://api.instabid.io:3000/exchanges/';
$data = array('email' => $email, 'password' => $password, 'name' => $name);
// use key 'http' even if you send the request to https://...
$options = array(
    'http' => array(
        'header'  => "Content-type: application/x-www-form-urlencoded\r\n",
        'method'  => 'POST',
        'content' => http_build_query($data),
    ),
);
$context  = stream_context_create($options);
$result = file_get_contents($url, false, $context);
return $result;

}


// Places a bid: User wants to buy or sell such qty at such price for this product located on this exchange
// buy_or_sell should be in the format B or S
function newOrder($exchange, $product, $buy_or_sell, $qty, $price, $user) {

$url = 'http://api.instabid.io:3000/orders/';
$data = array('exchange' => $exchange, 'product' => $product, 'privateKey' => 'not_in_place_yet', 'side' => $buy_or_sell, 'qty' => $qty, 'price' => $price, 'user' => $user);
// use key 'http' even if you send the request to https://...
$options = array(
    'http' => array(
        'header'  => "Content-type: application/x-www-form-urlencoded\r\n",
        'method'  => 'POST',
        'content' => http_build_query($data),
    ),
);
$context  = stream_context_create($options);
$result = file_get_contents($url, false, $context);
return $result;

}

// Creates a new product for that exchange
function newProduct($exchange, $product) {

$url = 'http://api.instabid.io:3000/products/';
$data = array('exchange' => $exchange, 'name' => $product, 'privateKey' => 'not_in_place_yet');
// use key 'http' even if you send the request to https://...
$options = array(
    'http' => array(
        'header'  => "Content-type: application/x-www-form-urlencoded\r\n",
        'method'  => 'POST',
        'content' => http_build_query($data),
    ),
);
$context  = stream_context_create($options);
$result = file_get_contents($url, false, $context);
return $result;

}

function getDepth($exchange, $product, $level) {

$url = 'http://api.instabid.io:3000/depths/';
$data = array('exchangeName' => $exchange, 'productName' => $product, 'token' => 'not_in_place_yet', 'level' => $level);
// use key 'http' even if you send the request to https://...
$options = array(
    'http' => array(
        'header'  => "Content-type: application/x-www-form-urlencoded\r\n",
        'method'  => 'GET',
        'content' => http_build_query($data),
    ),
);
$context  = stream_context_create($options);
$result = file_get_contents($url, false, $context);
return $result;

}

function getOrder($exchange, $product, $level, $user) {

$url = 'http://api.instabid.io:3000/orders/';
$data = array('exchangeName' => $exchange, 'productName' => $product, 'token' => 'not_in_place_yet', 'level' => $level, 'user' => $user);
// use key 'http' even if you send the request to https://...
$options = array(
    'http' => array(
        'header'  => "Content-type: application/x-www-form-urlencoded\r\n",
        'method'  => 'GET',
        'content' => http_build_query($data),
    ),
);
$context  = stream_context_create($options);
$result = file_get_contents($url, false, $context);
return $result;

}

function getFill($exchange, $product, $level, $user) {

$url = 'http://api.instabid.io:3000/fills/';
$data = array('exchangeName' => $exchange, 'productName' => $product, 'token' => 'not_in_place_yet', 'level' => $level, 'user' => $user);
// use key 'http' even if you send the request to https://...
$options = array(
    'http' => array(
        'header'  => "Content-type: application/x-www-form-urlencoded\r\n",
        'method'  => 'GET',
        'content' => http_build_query($data),
    ),
);
$context  = stream_context_create($options);
$result = file_get_contents($url, false, $context);
return $result;

}

// Beyond from being a library of function, instabid.php can also be used in
// conjunction with instabid.js where instabid.js posts to instabid.php which
// in turn submits API calls to Instabid. This is where we handle posts
// from instabid.js to instabid.php on YOUR server (not instabid's)
// This represents a risk of a) calling POSTS from the outisde and b) spoofing
// spoofing users from the inside. To avoid that. This instabid library requires
// that session variables intabid_userId be allocated

function authorize_user_to_bid($userId) {
	session_start();
	$_SESSION["instabid_userId"] = $userId;
}

function deauthorize_user_to_bid($userId) {
	session_start();
	$_SESSION["instabid_userId"] = NULL;
}

function newOrderViaJS($exchange, $product, $buy_or_sell, $qty, $price, $user) {
	if ($_SESSION["instabid_userId"] == NULL) {
		return '{"result":"FAIL","msg":"Sorry but this user is unidentified creating a risk of outside fraudulent access. You have to call the function authorize_user_to_bid(userId) in instabid.php first"}';
	}
	elseif ($_SESSION["instabid_userId"] != $user) {
		return '{"result":"FAIL","msg":"Sorry but this user is identified differently in the javascript call than in the php function authorize_user_to_bid(userId) in instabid.php you called earlier. This user could be attempting to spoof another user"}';
	}
	else {
		//return $_SESSION["instabid_userId"];
		return newOrder($exchange,$product,$buy_or_sell,$qty,$price,$user);
	}
}

// instabid.js posts to instabid.php which call this function which calls newProduct
function newProductViaJS($exchange, $product) {
        if ($_SESSION["instabid_userId"] == NULL) {
                return '{"result":"FAIL","msg":"Sorry but this user is unidentified creating a risk of outside fraudulent access. You have to call the function authorize_user_to_bid(userId) in instabid.php first"}';
        }
        else {
                //return $_SESSION["instabid_userId"];
                return newProduct($exchange,$product);
        }
}

function getDepthViaJS($exchange, $product, $level) {
        if ($_SESSION["instabid_userId"] == NULL) {
                return '{"result":"FAIL","msg":"Sorry but this user is unidentified creating a risk of outside fraudulent access. You have to call the function authorize_user_to_bid(userId) in instabid.php first"}';
        }
        else {
                //return $_SESSION["instabid_userId"];
                return getDepth($exchange,$product,$level);
        }
}

function getOrderViaJS($exchange, $product, $level, $user) {
        if ($_SESSION["instabid_userId"] == NULL) {
                return '{"result":"FAIL","msg":"Sorry but this user is unidentified creating a risk of outside fraudulent access. You have to call the function authorize_user_to_bid(userId) in instabid.php first"}';
        }
        else {
                //return $_SESSION["instabid_userId"];
                return getOrder($exchange,$product,$level,$user);
        }
}

function getFillViaJS($exchange, $product, $level, $user) {
        if ($_SESSION["instabid_userId"] == NULL) {
                return '{"result":"FAIL","msg":"Sorry but this user is unidentified creating a risk of outside fraudulent access. You have to call the function authorize_user_to_bid(userId) in instabid.php first"}';
        }
        else {
                //return $_SESSION["instabid_userId"];
                return getFill($exchange,$product,$level,$user);
        }
}

if ($_SERVER['REQUEST_METHOD'] == "POST") {
	if ($_POST["APICall"] == "newOrder") {
		echo newOrderViaJS($_POST["exchange"],$_POST["product"],$_POST["buy_or_sell"],$_POST["qty"],$_POST["price"],$_POST["user"]);
	}
	elseif ($_POST["APICall"] == "newProduct") {
		echo newProductViaJS($_POST["exchange"],$_POST["product"]);
	}
}
elseif ($_SERVER['REQUEST_METHOD'] == "GET") {
	if ($_GET["APICall"] == "getDepth") {
		//echo $_GET["exchange"] . $_GET["product"] . $_GET["level"]; 
		echo getDepthViaJS($_GET["exchange"],$_GET["product"],$_GET["level"]);
	}
	if ($_GET["APICall"] == "getOrder") {
		//echo $_GET["exchange"] . $_GET["product"] . $_GET["level"]; 
		echo getOrderViaJS($_GET["exchange"],$_GET["product"],$_GET["level"],$_GET["user"]);
	}
       if ($_GET["APICall"] == "getFill") {
                //echo $_GET["exchange"] . $_GET["product"] . $_GET["level"]; 
                echo getFillViaJS($_GET["exchange"],$_GET["product"],$_GET["level"],$_GET["user"]);
        }
}

?>
