<?php
//ini_set('display_errors', 'On');
//echo "version jjj";
session_start();
?>
<?php
$_SESSION["connected"]=false;
connect_redis();
$time_start = microtime(true);

date_default_timezone_set("UTC");
$time = gmmktime();
$humanTime = date("YmdHis", $time);
$eow = 9999999999999999;
$newTimestamp = $eow - date("YmdHis", $time) . '00';
$userId = $_SESSION["userId"];

$newPrice = $_POST["price"];
$newPrice = str_replace('$','',$newPrice);
$newQty = $_POST["qty"];
$newSide = strtoupper($_POST["side"]);
if ($newSide!='B') {$newSide='S';};
//echo $newTimestamp;

//$newObject = $_POST["object"];
$newObject = $_GET["objectId"];
if ($_POST["objectId"] != "") {$newObject = $_POST["objectId"];}

$message = '{ "object":"' . $newObject .  '","timestamp":"undefined","price":"' . $newPrice .  '","type":"limit","side":"' . $newSide . '","qty":"' . $newQty . '","number":0,"user":"' . $userId . '","qtyfilled":"0","lastchange":"na","venue":"x"}';
//echo $message;

$_SESSION["client"] -> PUBLISH('ORDERS:'. $newObject, $message); 
echo '{"status":"1"}';
//echo 'done';


/*
echo "You have ";

if ($newSide == "b") { echo "bought ";
} else { echo "sold ";
}

echo $_POST["qty"];
echo " ";

echo $_POST["object"];

echo " at ";
echo $_POST["price"];

$newOrderNum = $_SESSION["client"] -> incr("$newObject:ORDERS:NUM");
echo "<br>This is your $newOrderNum th order for this specific object<br>";

$_SESSION["client"] -> ZADD($newObject . ':ORDERS', $newPrice, $newTimestamp . ' ORD' . $newOrderNum);
$_SESSION["client"] -> ZADD($newObject. ':ORDERS:TIMESTAMP', $humanTime, $newOrderNum);
$newQtyfilled = 0;
$_SESSION["client"] -> hmset($newObject . ":ORDER:DETAILS:" . $newOrderNum, array('qty' => $newQty, 'qtyfilled' => 0, 'userid' => rand(), 'price' => $newPrice));

change_depth($newObject, $newPrice, $newQty);
echo "<br>Order successfully executed. Now let's see if it matches: <br>";

$newUserId = rand();

match_and_adjust_depth($newOrderNum, $newSide, $newQty, $newPrice, $newObject, $newTimestamp, $newUserId);

echo("<br>Now that's it for the match analysis<br>");

$time_end = microtime(true);

$diff = $time_end - $time_start;
echo "<br/>" . $diff . " time to execute";
//var_dump($retval);

function match_and_adjust_depth($newOrderNum, $newSide, $newQty, $newPrice, $newObject, $newTimestamp, $newUserId) {
	$newPrice = abs($newPrice);
	$newQtyfilled = 0; // for now we are assuming this is a new order so the qty filled is always 0
	
	if ($newSide == "b") {
		echo "Your buy order will match with the following sells by order of preference:<br>";
		$matchingOldOrders = $_SESSION["client"] -> ZREVRANGEBYSCORE($newObject . ":ORDERS", "0", "-" . $newPrice, array('withscores' => true));
	} else {
		echo "Your sell order will match with the following buys by order of preference:<br>";
		$matchingOldOrders = $_SESSION["client"] -> ZREVRANGEBYSCORE($newObject . ":ORDERS", "+INF", $newPrice, array('withscores' => true));
	}

	$i = 0;
	if (count($matchingOldOrders) <> 0) {
		// we found a series of orders which are candidates for a match
		do {
			//echo $matchingOldOrders[$i][0] . " is a candidate for a match<BR>";
			// the two orders are represented by new: ($newPrice/$newOrderNum/$newObject) AND OLD: $matchingOldOrders[$i][0] to delete NEW ZREM $newObject:ORDERS $newTimestamp . $newOrderNum 
			// You have to parse the $result grid, find and reduce the corresponding orders/depth and at the same time reduce the new orders/depth

			// first we have to strip the order number and use that to then touch the order details
			$oldFullOrderNum = $matchingOldOrders[$i][0];
			$oldPrice = $matchingOldOrders[$i][1];
			$oldShortOrderNum = substr($oldFullOrderNum, strrpos($oldFullOrderNum,'ORD')+3);
			echo "Candidate for a match: " . $oldFullOrderNum . " and the short version is " . $oldShortOrderNum . "<br>";
			$matchingSingleOldOrder = $_SESSION["client"]-> HMGET($newObject . ":ORDER:DETAILS:" . $oldShortOrderNum, array('qty','qtyfilled','userid'));
			if (count($matchingSingleOldOrder)<>0) {
				//echo "Qty: " . $matchingSingleOldOrder[0] . " qtyfilled " . $matchingSingleOldOrder[1] . "<br>";
				echo "Oldqty: " . $matchingSingleOldOrder[0] . " Oldqtyfilled: " . $matchingSingleOldOrder[1] . "<br>";	
				$oldQty = $matchingSingleOldOrder[0];
				$oldQtyFilled = $matchingSingleOldOrder[1];
				$oldUserId = $matchingSingleOldOrder[2];
				if($oldPrice>=0) {$oldSide="b";} else {$oldSide="s";}

					//if ($newQty-$newQtyfilled >= $oldQty - $oldQtyFilled ) {
						$qtyMatchableOld = $oldQty - $oldQtyFilled;
						echo "<br>The qtyMatchableOld is" . $qtyMatchableOld;
						$qtyMatchableNew = $newQty-$newQtyfilled;
						echo "<br> The qtyMatchableNew is " . $qtyMatchableNew;
						$qtyMatch = min($qtyMatchableNew,$qtyMatchableOld);
						if ($qtyMatchableOld <= 0 or $qtyMatchableNew<= 0) {goto jumpThisMatch;} 
						// the new order can fill or partial fill the old order
						// actions: reduce order details, working orders (if details went to zero), depth. Increase fills
						echo "we have a fill or partial fill<br>";
						$newFillNum = $_SESSION["client"] -> incr("$newObject:FILLS:NUM"); // create a fill id
						//$_SESSION["client"] -> ZADD($newObject . ':FILLS', $newPrice, $newTimestamp . ' ORD' . $newOrderNum);
						// we decided to persist fills inside the database rather than in redis
						if ($newSide == "b") {
							// the new order is the buy and the old order is the sell
							if ($_SESSION["db_or_redis"]=="db") {	
								$query = "INSERT INTO tbl_fills (fil_price_match, fil_qty_match, fil_id, fil_buy_user_id, fil_sell_user_id, fil_object_id) values (";
								$query = $query . $oldPrice . "," . $qtyMatch . ",'" . $newFillNum . "','" . $newUserId . "','" . $oldUserId . "','" . $newObject . "')";
							}
							else {
								$_SESSION["client"] -> hmset($newObject . ":FILL:DETAILS:" . $newFillNum, array('price_match' =>$oldPrice, 'qty_match' => $qtyMatch, 'buy_user_id' => $newUserId, 'sell_user_id'=>$oldUserId, 'object_id'=>$newObject )); // fully fill the old order
							}
							echo "old order: " . $oldSide . " " . $oldQty . " lots (already filled up to ". $oldQtyFilled . " lots) of " . $newObject . " @ " . $oldPrice . " MATCHED FOR " . $qtyMatch . " lots<br>";
								
						}
						else {
							// the old order is the buy and the new order is the sell
							if ($_SESSION["db_or_redis"]=="db") {
								$query = "INSERT INTO tbl_fills (fil_price_match, fil_qty_match, fil_id, fil_buy_user_id, fil_sell_user_id, fil_object_id) values (";
								$query = $query . $oldPrice . "," . $qtyMatch . ",'" . $newFillNum . "','" . $oldUserId . "','" . $newUserId . "','" . $newObject . "')";	
							}
							else {
								$_SESSION["client"] -> hmset($newObject . ":FILL:DETAILS:" . $newFillNum, array('price_match' =>$oldPrice, 'qty_match' => $qtyMatch, 'buy_user_id' => $oldUserId, 'sell_user_id'=>$newUserId, 'object_id'=>$newObject )); // fully fill the old order																
							}
								
							echo "old order: " . $oldSide . " " . $oldQty . " lots (already filled up to ". $oldQtyFilled . " lots) of " . $newObject . " @ " . $oldPrice . " MATCHED FOR " . $qtyMatch . " lots<br>";							
						}
						// we store the actual fil
						if ($_SESSION["db_or_redis"]=="db") {
							$result = mysql_query($query) or die('Query failed: ' . mysql_error());
						}
						else {
							// in the case of redis, OBJ:FILLS is a sorted set + a hash with details of the fill
							$time = gmmktime();
							$filTimestamp = date("YmdHis", $time) . '00';
							$_SESSION["client"] -> ZADD($newObject . ':FILLS', $filTimestamp, $newFillNum);
							//echo "<BR>The newQtyFilled is " . $newQtyfilled . " and the qtymatch is " . $qtyMatch . "<BR>";
							$newQtyfilled=$newQtyfilled+$qtyMatch; // the new order filled by the qty of the match
							$oldQtyFilled=$oldQtyFilled+$qtyMatch; // the old order is filled with the qty of the match
							$_SESSION["client"] -> hset($newObject . ":ORDER:DETAILS:" . $newOrderNum, 'qtyfilled',$newQtyfilled); // partially or fully fill the new order
							$_SESSION["client"] -> hset($newObject . ":ORDER:DETAILS:" . $oldShortOrderNum, 'qtyfilled',$oldQtyFilled); // fully fill the old order
							if ($oldQtyFilled == $oldQty) { $_SESSION["client"] -> zrem($newObject . ":ORDERS", $oldFullOrderNum);} // delete the old order from working orders
							if ($newQtyFilled == $newQty) { $_SESSION["client"] -> zrem($newObject . ":ORDERS", $newFullOrderNum);} // delete the old order from working orders
							if ($oldPrice>=0) {
								change_depth($newObject, $oldPrice, -1*$qtyMatch);
								change_depth($newObject, -1*$newPrice, -1*$qtyMatch);
							}
							else {
								change_depth($newObject, $oldPrice, -1*$qtyMatch);
								change_depth($newObject, $newPrice, -1*$qtyMatch);								
							}														
						}				
						//echo $query;
					//}
				}
				jumpThisMatch:
				$i++;
		} while ($i<count($matchingOldOrders) and ($newQtyFilled<$newQty));
	} else {
		// We found no opposite order to match against so update depth
		echo "No counterparty was found to match your order";
		$matchingOldOrders = $_SESSION["client"] -> ZRANGEBYSCORE($newObject . ":DEPTHS", $newPrice, $newPrice);
		$newQty_already_in_depth = 0;
		if (count($matchingOldOrders) <> 0) {
			// there is an existing depth at that price
			$newQty_already_in_depth = $matchingOldOrders[0];
			echo "<BR>Found an existing depth of " . $newQty_already_in_depth . " for that price. Updating it to " . ($newQty_already_in_depth + $newQty);
			// we destroy the old depth
			$_SESSION["client"] -> ZREM($newObject . ":DEPTH", $newQty_already_in_depth);
		}
		// we book in the new depth
		$_SESSION["client"] -> ZADD($newObject . ":DEPTH", $newPrice, $newQty_already_in_depth + $newQty);
	}

}
*/
function connect_redis() {
	//echo "Connecting to redis...";
if ($_SESSION["connected"] != "true") {
        require_once 'predis/autoload.php';
	
	$single_server = array('host' => '127.0.0.1', 'port' => 6379, 'database' => 15);

	$multiple_servers = array( array('host' => '127.0.0.1', 'port' => 6379, 'database' => 15, 'alias' => 'first', ), array('host' => '127.0.0.1', 'port' => 6380, 'database' => 15, 'alias' => 'second', ), );

	// simple set and get scenario
	$time_start = microtime(true);
	$_SESSION["client"] = new Predis\Client($single_server); 
};

}
/*
function connect_mysql() {


$_SESSION["link"]=mysql_connect('localhost','root','topaze123')
        or die('Could not connect: ' . mysql_error());
echo 'Connected mysql successfully';
mysql_select_db('likwefy') or die('Could not select database');


	
}

function change_depth($object, $price, $qty_change){
	// check if depth exist, if not create depth
	echo "<br>Changing depth of price " . $price . " by " . $qty_change . "</br>"; 
	$depthGrid = $_SESSION["client"] -> ZRANGEBYSCORE($object . ":DEPTHS", $price, $price, array('withscores' => false));
	if (count($depthGrid)==0) {
		// no previous depth found so we just create that depth
		$newDepthNum = $_SESSION["client"] -> incr("$object:DEPTHS:NUM");
		$_SESSION["client"] -> ZADD($object . ':DEPTHS', $price, $newDepthNum);
		$_SESSION["client"] -> SET($object . ':DEPTH:QTY:' . $newDepthNum , $qty_change); // create a depth for that price
	}
	else {
		echo "<br> we found an existing depth<br>";
		// previous depth, increase it
		$newDepthNum = $depthGrid[0][0];
		$qty = $_SESSION["client"] -> GET($object. ':DEPTH:QTY:'. $newDepthNum);
		echo "the existing depth qty is " . $qty;
	
		// we increase/decrease the depth
		$final_depth = $_SESSION["client"] -> INCRBY($object . ':DEPTH:QTY:' . $newDepthNum, $qty_change); // add to the depth
		if ($final_depth==0) {
			$_SESSION["client"] -> DEL($object . ':DEPTH:QTY:' . $newDepthNum);
			$_SESSION["client"] -> ZREM($object . ':DEPTHS', $newDepthNum);
		}
	}
	
}
	
	
*/
?>
