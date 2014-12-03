<html>
<head>
<?php 
session_start();
$_SESSION["connected"] = "false";
$objectId = $_GET["objectId"];
$option = $_GET["option"]; 
$size = $_GET["size"];

if ($_SESSION["connected"] != "true") {
        require 'predis/autoload.php';
        connect_redis();
}

//$object=$_GET["object"];
$object=$objectId;
//echo "This is the market depth for " . $object . ":<BR>";

$result1 = $_SESSION["client"]->ZRANGEBYSCORE($object .":DEPTHS", "-INF","0",array('withscores' => true));
$result2 = $_SESSION["client"]->ZREVRANGEBYSCORE($object .":DEPTHS", "+INF","0",array('withscores' => true));
$totalAskDepth = count($result1);
$totalBidDepth = count($result2);

?>
        <meta charset="utf-8">
        <title>Market </title>
  <link rel="stylesheet" href="http://code.jquery.com/ui/1.10.3/themes/smoothness/jquery-ui.css">
  <script src="http://code.jquery.com/jquery-1.10.2.min.js"></script> 
  <!-- <script src="http://code.jquery.com/jquery-1.9.1.js"></script> 
  <script src="http://code.jquery.com/ui/1.10.3/jquery-ui.js"></script> -->
  <script src="http://code.jquery.com/ui/1.10.3/jquery-ui.min.js"></script>
<script src="http://cdn.sockjs.org/sockjs-0.3.min.js"></script>
<script>
  var countAsk = <?php echo $totalAskDepth; ?>;
  var countBid = <?php echo $totalBidDepth; ?>;
  $userId = "<?php echo $_SESSION["userId"]; ?>";
  $(function() {
    $( "#side" ).buttonset();
    $("button").button();
  });

    var sockjs_url = <?php echo "'http"; if ($_SERVER["HTTPS"] == "on") {echo "s";} echo "://" . $_SERVER["SERVER_NAME"] . ":8880/channel';";  ?>
    var sockjs = new SockJS(sockjs_url);

    //var div  = $('#SimplePrice');

    var print = function(message){
		//alert('toto');
                  //div.html(message);
		var obj = JSON.parse(message);
		//var P = parseFloat(message.substr(6,message.indexOf('"',6)-6));
		var P = parseFloat(obj.P);
		var PP = P;
		if (PP<0) {PP=PP*-1;} // makes PP absolute
		/*var Vpos = message.indexOf('V')+4;
		var Vend = message.indexOf('"',Vpos);
		var V = message.substr(Vpos , Vend-Vpos); */
		var V = obj.V;
		/*var Npos = message.indexOf('N')+4;
		var Nend = message.indexOf('"',Npos);
		var N = message.substr(Npos , Nend-Npos);*/
		var N = obj.N;
		var RR = 1.2;
		var lastP = 9999999999;
		if (P<0) {lastP=lastP*-1;}; 
		var somethingChanged=0;
		//alert('P=' + P + ' V=' + V + ' N=' + N);
		$('#DEPTHNUM' + N).html(V);
		if (V == "0") {
			$('#TR' + N).remove();
			if (P<0) {countAsk--; } else {countBid--; }
		};
		if (($("#DEPTHNUM" + N).length)==0) {
			//alert('new depth');
			$('#DepthTable tr').each(function()
				{
				//me = $(this);
				TT = parseFloat($(this).attr('price'));
				if (TT<0) 
					{ meAsk = $(this); }
				else
					{ meBid = $(this); };
					if (P < 0) {
					// browsing the asks
						//alert("lastP: " + lastP + " P: " + P + " TT: " + TT);
						//alert((lastP < P) && (P < TT));
						if ((lastP < P) && (P < TT) && (TT<0) && V != 0) { 
							$("<tr id='TR" + N +"' price='" + P + "'  data-toggle='modal' data-id='1' data-target='#orderModal'><td></td><td></td><td price='" + PP +"' qty='" + V + "' id='" + N + "' class='Sprice'>$" + PP + "</td><td price='" + P + "' qty='" + V + "' id='DEPTHNUM" + N + "' class='Sqty'>" + V + "</td></tr>").insertBefore($(this)); 
							if (P<0) {countAsk++; } else {countBid++; }
							somethingChanged=1;
							return false;
						}
						lastP = TT;
						if ($.isNumeric(lastP)==false) { lastP = -9999999999;};
					}
					else {
					// browsing the bids	
						//alert("lastP: " + lastP + "> P: " + P + "> TT: " + TT);
						//alert((lastP > P) && (P > TT));
						if ((lastP > P) && (P > TT) && (TT>0) && V != 0) { 
							//alert("<tr id='" + P + "' data-toggle='modal' data-id='1' data-target='#orderModal'><td price='" + P + "' qty='" + V + "' id='DEPTHNUM" + N + "' class='Bqty'>" + V + "</td><td price='" + P + "' qty='" + V + "' id='" + N + "' class='Bprice'>$" + P + "</td><td></td><td></td></tr>"); 
							$("<tr id='TR" + N + "'  price='" + P + "' data-toggle='modal' data-id='1' data-target='#orderModal'><td price='" + P + "' qty='" + V + "' id='DEPTHNUM" + N + "' class='Bqty'>" + V + "</td><td price='" + P + "' qty='" + V + "' id='" + N + "' class='Bprice'>$" + P + "</td><td></td><td></td></tr>").insertBefore($(this)); 
							if (P<0) {countAsk++; } else {countBid++; }
							somethingChanged=1;
							return false;
						}
						if (TT>0) {lastP = TT;};
						if ($.isNumeric(lastP)==false) { lastP = 9999999999;};
					
					}
				})
				// out of the table loop
				if (somethingChanged!=1 && V!=0) {
					// we did not find something to change in the table loop
					if (P<0) {
						// if we couldn't place the ask inside the table, it must go at the end
						$("<tr id='TR" + N +"'  price='" + P + "' data-toggle='modal' data-id='1' data-target='#orderModal'><td></td><td></td><td price='" + PP +"' qty='" + V + "' id='" + N + "' class='Sprice'>$" + PP + "</td><td price='" + P + "' qty='" + V + "' id='DEPTHNUM" + N + "' class='Sqty'>" + V + "</td></tr>").insertAfter(meAsk);
						if (P<0) {countAsk++; } else {countBid++; }
					}
					else {
						 if (TT<0) {meBid = meAsk;};
						 $("<tr id='TR" + N + "'  price='" + P + "' data-toggle='modal' data-id='1' data-target='#orderModal'><td price='" + P + "' qty='" + V + "' id='DEPTHNUM" + N + "' class='Bqty'>" + V + "</td><td price='" + P + "' qty='" + V + "' id='" + N + "' class='Bprice'>$" + P + "</td><td></td><td></td></tr>").insertAfter(meBid);
						if (P<0) {countAsk++; } else {countBid++; }
					}
				}
		}
		//alert('changing the depth of ' + P + ' to become ' + V);
$('#Depth tr td').click(function(){
    var cid = $(this).attr('id');
    var cval = $(this).text();
    cval = cval.replace("$","");
    var cclass = $(this).attr('class');

        var qty = $(this).attr('qty');
        var price = Math.abs(parseFloat($(this).attr('price')));

	$(input[name="price"]).val(price);
        //$(input[name="price"]).val(price);
        if (cclass == 'Sqty' || cclass == 'Bqty')
                { $(input[name="qty"]).val(qty); }
        else
                 { $(input[name="qty"]).val(""); }
        ;

$(function() {
    $( "#OE" ).dialog({ width: 180, modal: true, title:"Order"});
  });

});		
    }

    sockjs.onopen    = function()  {
        //sockjs.send('ObjectId:<?php echo $objectId ?>');
        //sockjs.send('UserId:Julien');
        //sockjs.send('RequestBestDepth:Julien{<?php echo $objectId ?>}');
	sockjs.send('{"event":"AllDepth","channel_id":"<?php echo $objectId; ?>","nick":"<?php echo $_SESSION["userId"]; ?>"}' );
    };
    sockjs.onmessage = function(e) {
		var oldCountAsk = countAsk;
		var oldCountBid = countBid;
		if (oldCountAsk == 0 ) {location.reload();};
		if (oldCountBid == 0 ) {location.reload();};
		print(e.data); 
    }
    sockjs.onclose   = function()  {alert('Connexion lost');};


</script>

<style type='text/css'>
#bothDepthAndOE{
    width: <?php if ($option != "PRO") {echo "550px;";} else {echo "auto;";};  ?>
    height: <?php if ($option != "PRO") {echo "300px;";} else {echo "auto;";};  ?>
    overflow: hidden;
    margin: 0px;
}
#Depth{
    width: <?php if ($option != "PRO") {echo "550px;";} else {echo "auto;";};  ?>
    height: <?php if ($option != "PRO") {echo "300px;";} else {echo "auto;";};  ?>
    float: left;
    left: 10px;
}
#OE{
    width: 0px;
    width:0px;
    overflow: hidden;
    margin-top: 0px;
    font-family: "Lucida Sans Unicode", "Lucida Grande", Sans-Serif;
    font-size: 12px;
    background: #e8edff;
}


#textHeader
{
	font-family: "Lucida Sans Unicode", "Lucida Grande", Sans-Serif;
	font-size: 12px;
	margin: 0px;
	width: 590px;
	text-align: center;
	border-collapse: separate;
	empty-cells:hide;
	border:1px solid transparent;
}

#DepthTable
{
	font-family: "Lucida Sans Unicode", "Lucida Grande", Sans-Serif;
	font-size: 12px;
    margin: <?php if ($option != "PRO") {echo "45px;";} else {echo "0;";};  ?>
	width: 480px;
	width: <?php if ($size == "") {echo "480";} else {echo $size *480;};  ?>px;
	text-align: center;
	border-collapse: separate;
	empty-cells:hide;
	border:1px solid transparent;
}
#DepthTable th
{
	font-size: 13px;
	font-weight: normal;
	padding: <?php if ($size == "") {echo "8";} else {echo $size *8;};  ?>px;
	background: #b9c9fe;
	color: #039;
}
#DepthTable td
{
	padding: <?php if ($size == "") {echo "8";} else {echo $size *8;};  ?>px;
	background: #e8edff; 
	color: #669;
}
#DepthTable tr:hover td
{
	background: #d0dafd;
	color: #339;
}

#side
{
	margin:20px;
	margin-right:20px;
        margin-top:1px;
}

.myfields {
    border-color:transparent;
    color: #039;
}

.myfields label, .myfields input {
    display:inline-block;
}

.myfields label {
    width:40px;
}
.myfields input {
    width:80px;
height: 30px;
padding: 5px;
text-align: center
}

.ui-widget{font-size:12px;
}

.ui-dialog-titlebar {
  background-color: #B1B5C4;
  background-image: none;
  color: #000;
}

.ui-widget-content {
width:260px;
}

.ui-widget-content {
background: #193E5F;
}

#lp-pom-button-21, #lp-pom-button-login, #lp-pom-button-admin, #lp-pom-button-reset, #lp-pom-button-create {
  position:absolute;
  left:0px;
  top:170px;
  z-index:7;
  width:308px;
  height:48px;
  behavior:url(/PIE.htc);
  background-color:#f7941d;
  background:-webkit-linear-gradient(#f7941d,#d75305);
  background:-moz-linear-gradient(#f7941d,#d75305);
  background:-ms-linear-gradient(#f7941d,#d75305);
  background:-o-linear-gradient(#f7941d,#d75305);
  background:linear-gradient(#f7941d,#d75305);
  box-shadow:inset 0px 1px 0px #ffbb6a,inset 0 -1px 2px #a33f03;
  text-shadow:1px 1px #521601;
  -pie-background:linear-gradient(#f7941d,#d75305);
  color:#fff;
  border-style:solid;
  border-width:1px;
  border-color:#333333;
  font-size:17px;
  line-height:20px;
  font-weight:bold;
  text-align:center;
  background-repeat:no-repeat;
}

</style>
<BODY>
<div id="bothDepthAndOE">
<div id="Depth">
<?php



$i=0;
$noDepth = 0;
if (count($result1)>0) {
	echo "<TABLE id='DepthTable' border='1' align='center'>";
	echo "<thead><tr><th># of bids</th><th>Bid Prices</th><th>Buy Now at</th><th>Max qty avail</th></tr>";
	do {
		if ($result1[$i][1] < 0) {$prefix = "N";} else {$prefix = "";};
		$realprice = $result1[$i][1];
		$price = abs($realprice);
		$depthNum = $result1[$i][0];
		$qty = $_SESSION["client"]->get($object . ":DEPTH:QTY:" . $depthNum);
		$idDepth = $depthNum;
		echo "<tr id=\"TR" . $depthNum . "\" price=\"" . $realprice . "\" data-toggle=\"modal\" data-id=\"1\" data-target=\"#orderModal\"><td></td><td></td><td price='" . $price . "' qty='" . $qty . "' id=\"" . $qty . "\" class=\"Sprice\">$" . abs($price) . "</td><td price='" . $price . "' qty='" . $qty . "' id=\"DEPTHNUM" . $depthNum . "\" class=\"Sqty\">" . $qty . "</td></tr>";
		$i++;
		} while ($i<count($result1));
		$noDepth = 1;
	}



$i=0;
if (count($result2)>0) {
		if (count($result1)==0) {
			echo "<TABLE id='DepthTable' border='1' align='center'>";
			echo "<tr><td># of bids</td><td>Bid Prices</td><td>Sale Prices</td><td># for sale</td></tr>";
		}
		do {
			if ($result1[$i][1] < 0) {$prefix = "N";} else {$prefix = "";};
			$realprice = $result2[$i][1];
			$price = abs($realprice);
			$depthNum = $result2[$i][0];
			$qty = $_SESSION["client"]->get($object . ":DEPTH:QTY:" . $depthNum);
			$idDepth = $depthNum;
			echo "<tr id=\"TR" . $depthNum . "\" price=\"" . $realprice . "\" data-toggle=\"modal\" data-id=\"1\" data-target=\"#orderModal\"><td price='" . $price . "' qty='" . $qty . "' id=\"DEPTHNUM" . $depthNum . "\" class=\"Bqty\">" . $qty . "</td><td price='" . $price . "' qty='" . $qty . "' id=\"" . $qty . "\" class=\"Bprice\">$" . $price . "</td><td></td><td></td></tr>";
			$i++;
			} while ($i<count($result2));
		$noDepth = 1;
	}


echo "</TABLE>";


function connect_redis() {
        #echo "Connecting to redis...";
if ($_SESSION["connected"] != "true") {
        require_once 'predis/autoload.php';
        $single_server = array('host' => '127.0.0.1', 'port' => 6379, 'database' => 15);

        $multiple_servers = array( array('host' => '127.0.0.1', 'port' => 6379, 'database' => 15, 'alias' => 'first', ), array('host' => '127.0.0.1', 'port' => 6380, 'database' => 15, 'alias' => 'second', ), );

        // simple set and get scenario
        $time_start = microtime(true);
        $_SESSION["client"] = new Predis\Client($single_server);
        $_SESSION["connected"] = "true";
};

}

?>
</div>
<div id="OE">
<form name="input" id="input" action="redisNew.php?objectId=<?php echo $objectId; ?>" method="post">
<fieldset id="myfieldsid" class="myfields">
<div id="Qty">
<label for="qty">Qty:</label><input type="text" id="qty" name="qty" autocomplete="off">
</div>
<div id="Price">
<label for="price">Price:</label><input type="text" id="price" name="price" autocomplete="off" style="margin-top:5px;">
</div>
</fieldset>


  <div id="side">
    <input type="radio" class="side" id="radio1" name="side" value="B"><label for="radio1">Buy</label>
    <input type="radio" class="side" id="radio2" name="side" value="S"><label for="radio2">Sell</label>
  </div>


  <script>

 function show_box() {
   $("#dialog-confirm").dialog({
     autoOpen: false,
     width: 400,
     buttons: [{
       text: "Yes",
       click: function () {
 	 $(this).closest("form").submit();
         $(this).dialog("close");
       }
     }, {
       text: "No",
       click: function () {
         $(this).dialog("close");
       }
     }, {
       text: "Cancel",
       click: function () {
         $(this).dialog("close");
       }
     }]
   });
   $("#dialog-confirm").dialog("open");
   //});
 }


$('input[type=radio]').click(function() {
 //show_box();
 if ($userId == "") {
	$('#loginFormDiv').dialog({
                position: ['top',10],
		title: 'Please login first'
                                    }
        );
 }
 else {
	$(this).closest("form").submit();
}
 

});

$(document).ready(function(){
// $('#amendDetailsButton').hide();
//$('#arrow').show(0).delay(500).hide(0).delay(500).show(0).delay(500).hide(0).delay(500).show(0).delay(500).hide(0);

//$('#textHeader').fadeOut(4000, function() {
    // Animation complete.
$('#amendDetailsButton').hide();
var option = "<?php echo $option;?>";
var size = "<?php echo $size;?>";

<?php
if ($noDepth ==0) {
	echo "$('#arrow').hide();";
	echo "$('#textHeader').hide();";
        echo('$( "#OE" ).dialog({ width: 180, modal: true, title:"Order"})');
}
else {
	if ($option != "PRO") { 
		echo "$('#arrow').show(0).delay(500).hide(0).delay(500).show(0).delay(500).hide(0).delay(500).show(0).delay(500).hide(0);";
		echo "$('#textHeader').fadeOut(4000, function() {});";
	}
	else {
		echo '$("#textHeader").hide();';
		echo '$("#arrow").hide();';
	}
}

?>
/*    $('#Depth').click(function(e) {  
		var $resized;
		if ($resized != "true") {window.resizeTo(850, 410);};
		$resized = "true";
		$('#Depth').css("width","590px");
		$('#bothDepthAndOE').css("width","800px");
		$('#OE').css("width","150px");
		$('#OE').show();
    });
*/

//$('#qty').spinner();

$('#lp-pom-button-reset').click(function(){
        window.location = "forgotPassword.php?incoming=Depth";
});

$('#lp-pom-button-create').click(function(){
	$('#lp-pom-button-reset').hide();
	$('#lp-pom-button-login').hide();
	$('#loginPassword').hide();
	$('#lblloginPassword').hide();
		
	if ($('#loginEmail').val()=="") {
		alert('Please type your email');
	}
	else {
		$.ajax({type:"POST",
                url:"createUserAjax.php",
                cache:false,
                data:$("#loginForm").serializeArray(),
                success: function (data,textStatus,jqXHR) {
                        if (data!=0) {
                                result = JSON.parse(data);
				$('#loginEmail').hide();
				$('#lp-pom-button-create').hide();
				$('#lblloginEmail').text('Congratulations! You opened your instabid account. Your email is ' + result.email + ' and your password is ' + result.password + '. You can close this window and trade now!');
				$userId = result.userId;	
				//alert(data);
                                //$("#loginFormDiv").dialog( "close" );
                        }
                        else
                        {
                                alert("There was an error in creating your user!")
                        }
                }
               })
	}
});

$('#lp-pom-button-login').click(function(){
	//alert('Somebody pressed the login button');
        $.ajax({type:"POST",
                url:"login.php",
                cache:false,
                data:$("#loginForm").serializeArray(),
                success: function (data,textStatus,jqXHR) {
                        if (data!=0) {
				result = JSON.parse(data);
				$userId = result.userId;
				alert("User successfully logged in!");
				$("#loginFormDiv").dialog( "close" );
  			}
			else
			{
				alert("Username and password did not match!")
			}
		}
	       }) 
					 });

    $('#amendDetailsButton').click(function(e) {
	$('#orderDetails').show();
	$('#orderDetails').append('<iframe style="margin-top:50px;" id="orderDetailsFrame" height=150 width=750 src="/grid.php?object=<?php echo $objectId . "&ordersWidth=730" ; ?>" frameborder="0" scrolling="no" allowfullscreen> </iframe>');
	$('#fillDetails').append('<iframe style="margin-top:0px;" id="fillDetailsFrame" height=150 width=750 src="/fillsGrid.php?object=<?php echo $objectId . "&ordersWidth=730" ; ?>" frameborder="0" scrolling="no" allowfullscreen> </iframe>');
	$('#graph').append('<iframe style="margin-top:20px;" id="graphFrame" height=250 width=750 src="/graph.php?object=<?php echo $objectId . "&ordersWidth=730" ; ?>" frameborder="0" scrolling="no" allowfullscreen> </iframe>');
	window.resizeTo(850, 850);
    });

$('#Depth tr td').click(function(){
    var cid = $(this).attr('id');
    var cval = $(this).text();
    cval = cval.replace("$","");
    var cclass = $(this).attr('class');

	var qty = $(this).attr('qty');
	var price = $(this).attr('price');


	$(input[name="price"]).val(price);
	if (cclass == 'Sqty' || cclass == 'Bqty') 
		{ $(input[name="qty"]).val(qty); }
	else
		 { $(input[name="qty"]).val(""); }
	;

$(function() {
    $( "#OE" ).dialog({ width: 180, modal: true, title:"Order"});
  });

});


var request;
// bind to the submit event of our form
$("#input").submit(function(event){
    // abort any pending request
    if (request) {
        request.abort();
    }
    // setup some local variables
    var $form = $(this);
    // let's select and cache all the fields
    var $inputs = $form.find("input, select, button, textarea");
    // serialize the data in the form
    var serializedData = $form.serialize();

    // let's disable the inputs for the duration of the ajax request
    $inputs.prop("disabled", true);

    // fire off the request to /form.php
    request = $.ajax({
        url: "redisNew.php?objectId=<?php echo $objectId; ?>",
        type: "post",
        data: serializedData
    });

    // callback handler that will be called on success
    request.done(function (response, textStatus, jqXHR){
        // log a message to the console
        console.log("Hooray, it worked!");
    });

    // callback handler that will be called on failure
    request.fail(function (jqXHR, textStatus, errorThrown){
        // log the error to the console
        console.error(
            "The following error occured: "+
            textStatus, errorThrown
        );
    });

    // callback handler that will be called regardless
    // if the request failed or succeeded
    request.always(function () {
        // reenable the inputs
        $inputs.prop("disabled", false);
    });

    // prevent default posting of form
    event.preventDefault();
});



});

/*
  $(function() {
    $( "input[type=submit], a, button" )
      .button()
      .click(function( event ) {
	$.post( "redisNew.php", $( "#input" ).serialize() );
      });
  }); 
*/



  </script>
<script>
/*
$(function(){
    $('#orderModal').modal({
        keyboard: true,
        backdrop: "static",
        show:false,

    }).on('show', function(){ //subscribe to show method
          var getIdFromRow = $(event.target).closest('tr').data('id'); //get the id from tr
        //make your ajax call populate items or what even you need
        $(this).find('#orderDetails').html($('<b> Order Id selected: ' + getIdFromRow  + '</b>'))
    });
});
*/
</script>


</form>
<div id="amendDetails">
<button id="amendDetailsButton" style="margin-left:12px; margin-bottom:12px;">Details/Amend</button>
</div>

</div>
</divauto>
<div id="textHeader" style="color:red; font-weight:bold; position: absolute; top: 25px; left: 0px;">Click on grid above to begin bidding</div>
<div id="orderDetails">
</div>
<div id="fillDetails">
</div>
<div id="graph">
</div>
<div id="arrow" style="position:absolute;left:290px;top:<?php echo(120+($totalAskDepth-1)*40); ?>px;">
<img width=40 src="redarrow.png">
</div>


<div id="loginFormDiv" name="loginFormDiv" style="display:none;">
<form id="loginForm" action="login.php" method="POST">
<fieldset class="clearfix" style="width: auto; height: 143px; border:0px;">
<div style="width:auto;">
<label for="loginEmail" id="lblloginEmail" style="color: #cdd6e4; font-size:12px;">Email&nbsp;</label>
<input type="text" id="loginEmail" name="loginEmail" class="text" style="top: 21px; left: 0px; margin-left:29px; width: 180px; font-size: 12px; height: 20px; padding-left: 12px; padding-right: 12px; lineheight: 15px;">
<a class="lp-element lp-pom-button" id="lp-pom-button-reset" style="top:72px; left: 125px; width:120px; height:20px; font-size:12px; "><span class="label" id="button_content_reset" style="margin-top: -30px;">Forgot password</span></a>
</div>
<label for="loginPassword" id="lblloginPassword" class="main" style="top: 0px; width: auto; height: auto; color: #cdd6e4; font-size:12px;">
Password&nbsp;
</label>
<input type="password" id="loginPassword" name="loginPassword" class="text" style="top: 24px; margin-top:3px; left: 0px; width: 180px; font-size: 12px; height: 20px; padding-left: 12px; padding-right: 12px; lineheight: 15px;">

<a class="lp-element lp-pom-button" id="lp-pom-button-login" style="top:72px; left: 70px; width:45px; height:20px; font-size:12px;"><span class="label" id="button_content_login" style="margin-top: -40px;">Login</span></a>
<a class="lp-element lp-pom-button" id="lp-pom-button-create" style="top:102px; left: 70px; width:175px; height:20px; font-size:12px;"><span class="label" id="button_content_login" style="margin-top: -40px;">Create new account</span></a>
<input type="submit" style="display:none;">
</fieldset>
</form>
</div>


</body>
<!-- <div id="dialog-confirm" title="Empty the recycle bin?">
  <p><span class="ui-icon ui-icon-alert" style="float:left; margin:0 7px 20px 0;"></span>These items will be permanently deleted and cannot be recovered. Are you sure?</p>
</div> -->
</html>
