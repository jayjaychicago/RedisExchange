<!doctype html>
<html><head>
	<?php
		session_start();	
		$objectId = $_GET["objectId"];
		$backgroundColor = '#' . $_GET["backgroundColor"];
		$defaultPrice = $_GET["defaultPrice"];
		$fontSize = $_GET["fontSize"];
		$font = $_GET["font"];
		$color = $_GET["color"];
		$button = strtoupper($_GET["button"]);
		$option = strtoupper($_GET["option"]);
	?>
	<title><?php echo "Trading " . $font . $objectId; ?></title>
    <script src="http://ajax.googleapis.com/ajax/libs/jquery/1.10.2/jquery.min.js"></script>
    <script src="http://cdn.sockjs.org/sockjs-0.3.min.js"></script>

    <style>
    


        body {
            background-color: <?php if ($backgroundColor == '#') { echo "#F0F0F0" ;} else {echo backgroundColor;}; ?>;
            font-family:  <?php if (strlen($font) == 0 ) { echo "\"Arial\";";} else {echo "\"" . $font . "\";";}; ?> 
            font-size: <?php if (strlen($fontSize) == 0 ) { echo "12"; } else {echo $fontSize;};  ?>px;
	    margin-top:1px;
	    margin-left:1px;
	    <?php if (strlen($color) != 0) { if ($color != "white") {echo "color: #" . $color . ";";} else {echo "color: white;";};} ; ?>
        }
                
        img
		{
		vertical-align:middle;
		}
		

    </style>

</head><body lang="en">
<!-- <img id="liklogo" src="smallLogo.png" height="12" width="10" />--> $<div id="SimplePrice" class="expose" style="display: inline;" onclick="window.open('/combo.php?objectId=<?php echo $objectId; ?>','targetWindow','toolbar=no,location=no,status=no,menubar=no,scrollbars=yes,resizable=yes,width=600,height=350')"></div><?php echo ($option=="PRO") ? '<font style="padding-left:10px; font-size:' . $fontSize*0.5 . 'px;">+0.0 (0.0%)</font>' : '' ;  echo ($button=="NO") ? '' : '<img id="bidbutton" src="bid_button.png" height="20" width="48" style="margin-left: 5px; display: inline; top:-1px; "  onclick="window.open(\'/combo.php?objectId=' . $objectId . '\',\'targetWindow\',\'toolbar=no,location=no,status=no,menubar=no,scrollbars=yes,resizable=yes,width=600,height=350\')" />' ?>
<div id="overlay"></div>

<script>
    //var sockjs_url = 'http://www.pinterbay.com:8880/chat';
    var sockjs_url = <?php echo "'http"; if ($_SERVER["HTTPS"] == "on") {echo "s";} echo "://" . $_SERVER["SERVER_NAME"] . ":8880/channel';";  ?>
    var sockjs = new SockJS(sockjs_url);

    var div  = $('#SimplePrice');

    var print = function(message){
	try {
                var jsonTab = JSON.parse(message);
            }
        catch(e)
            {
                alert(e);
            }

                if (jsonTab.msgType == "S") {
                        $('#SimplePrice').html(jsonTab.P);
                }

		  //div.html(message);
    }

    sockjs.onopen    = function()  {
    	//sockjs.send('ObjectId:<?php echo $objectId ?>');
    	//sockjs.send('UserId:Julien');
    	//sockjs.send('RequestBestDepth:Julien{<?php echo $objectId ?>}');
	sockjs.send('{"event":"SimplePriceFeed","channel_id":"<?php echo $objectId; ?>","nick":"<?php echo $_SESSION["userId"]; ?>"}' );
    };
    sockjs.onmessage = function(e) {print(e.data);};
    sockjs.onclose   = function()  {print('N/A');};

  

</script>
</body>
</html>
