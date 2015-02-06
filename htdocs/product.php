<?php
session_start();
//ini_set('display_errors',1);
//error_reporting(E_ALL);

        include "instabid.php";
        $email = $_SESSION["email"];
	if ($email == "") { header( 'Location: /register.php' ) ; }
?>
<!DOCTYPE html>
<html lang="en">
<head>
<!-- Le styles -->
<script src="//ajax.googleapis.com/ajax/libs/jquery/2.1.3/jquery.min.js"></script>
<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.1/css/bootstrap.min.css">

<!-- Optional theme -->
<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.1/css/bootstrap-theme.min.css">

<!-- Latest compiled and minified JavaScript -->
<script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.1/js/bootstrap.min.js"></script>
<script type="text/javascript" src="http://www.instabid.io/manage/assets/instabid.js?version=<?php echo rand(1,1000000); ?>"></script>
<script type="text/javascript" src="http://www.instabid.io/manage/assets/instabid_realtime.js?version=<?php echo rand(1,1000000); ?>"></script>
<script type="text/javascript" src="http://www.instabid.io/manage/assets/instabid-ui.js?version=<?php echo rand(1,1000000); ?>"></script>

<script src="highchart/js/highstock.js"></script>
<script src="highchart/js/modules/exporting.js"></script>

<style>
.table th, .table td {
  text-align: center;
}
</style>
<script>
</script>
</head>
 
<body>
<nav class="navbar navbar-default">
  <div class="container-fluid">
    <!-- Brand and toggle get grouped for better mobile display -->
    <div class="navbar-header">
      <button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#bs-example-navbar-collapse-1">
        <span class="sr-only">Toggle navigation</span>
        <span class="icon-bar"></span>
        <span class="icon-bar"></span>
        <span class="icon-bar"></span>
      </button>
      <a class="navbar-brand" href="#">Instabid</a>
    </div>
    <div class="collapse navbar-collapse" id="bs-example-navbar-collapse-1">
      <ul class="nav navbar-nav">
        <!-- <li class="active"><a href="#">Link <span class="sr-only">(current)</span></a></li>
        <li><a href="#">Link</a></li> -->
        <li class="dropdown">
          <a href="#" class="dropdown-toggle" data-toggle="dropdown" role="button" aria-expanded="false">My Instabid <span class="caret"></span></a>
          <ul class="dropdown-menu" role="menu">
            <li><a href="/API.php">API howto</a></li>
            <li><a href="/GUI.php">GUI howto</a></li>
            <li class="divider"></li>
            <li><a href="/dashboard_instabid.php">My products</a></li>
          </ul>
        </li>
      </ul> 
      <ul class="nav navbar-nav navbar-right">
       <!-- <li><a href="#">Link</a></li> -->
      <!--<form class="navbar-form navbar-left" role="search">
        <div class="form-group">
          <input type="text" class="form-control" placeholder="Search">
        </div>
        <button type="submit" class="btn btn-default">Search a product</button>
      </form>-->
        <li class="dropdown">
          <a href="#" class="dropdown-toggle" data-toggle="dropdown" role="button" aria-expanded="false"><?php echo $email; ?> <span class="caret"></span></a>
          <ul class="dropdown-menu" role="menu">
            <!--<li><a href="#">Action</a></li>
            <li><a href="#">Another action</a></li> -->
            <li><a href="/">Main page</a></li>
            <li class="divider"></li>
            <li><a href="logoff_instabid.php">Log off</a></li>
          </ul>
        </li>
      </ul>
    </div><!-- /.navbar-collapse -->
  </div><!-- /.container-fluid -->
</nav>
<div class="container" style="margin-top:20px;">
 
 
<!-------->
<div id="content">
    <ul id="tabs" class="nav nav-tabs" data-tabs="tabs">
        <li class="active"><a href="#cyan" data-toggle="tab">Simple bids</a></li>
        <li><a href="#red" data-toggle="tab">Bids vs Inventory</a></li>
        <li><a href="#orange" data-toggle="tab">Review orders placed</a></li>
        <li><a href="#yellow" data-toggle="tab">Review Orders filled</a></li>
        <li><a href="#green" data-toggle="tab">Price Graph</a></li>
        <!--<li><a href="#blue" data-toggle="tab">More</a></li>-->
        <li><a href="#pink" data-toggle="tab">My products</a></li>
    </ul>
<div id="my-tab-content" class="tab-content">
        <div class="tab-pane" id="red">

            <h2>View bids summary & Place Orders (<?php echo $_GET["product"] ;?>)</h2>
               <div style="position:relative;">	
			<div class="table-responsive" id="depthTable" style="width:500px;position:absolute"></div>
               		 <div id="placeBid" style="width:410px;margin-left:530px;margin-top:10px;position:absolute;" ></div>
		</div>
        </div>
        <div class="tab-pane" id="orange">
            <h2>Review orders placed</h2>
		<div class="table-responsive" id="orderTable" style="width:100%"></div>
        </div>
        <div class="tab-pane" id="yellow">
            <h2>Review Orders filled</h2>
		<div class="table-responsive" id="fillTable" style="width:100%"></div>
        </div>
        <div class="tab-pane" id="green">
            <h2>Price Graph</h2>
		<div class="table-responsive" id="graph" style="width:100%"></div>
        </div>
        <div class="tab-pane" id="blue">
            <h2>More<h2>
		We will be adding positions, pnl upon position close soon
        </div>
        <div class="tab-pane active" id="cyan">
	<h2>Simple bids collection and checkout for product <?php echo $_GET["product"] ;?></h2>
	<div style="width:350px;" id="simpleBids"></div>
	<div style="width:350px;" id="simpleBidsTable"></div>
	</div>
        <div class="tab-pane" id="pink">
            <h2>My products</h2>
		<div id="productList"></div>
		<form class="form-inline">
                        <div class="form-group">
                        <label class="sr-only" for="newProductName">Product name</label>
                        <input class="form-control" id="newProductName" placeholder="Enter Product Name">
                        </div>
                        <button type="submit" onclick="return false;" id="newProductButton" class="btn btn-default">Create Product</button>
        	</form>
        </div>
</div>

<!--
        <div class="panel panel-default" style="width:50%">
            <div class="panel-heading" >
                <h6 class="panel-title">
                    <a data-toggle="collapse" data-parent="#accordion" href="#collapseTwo">Place a bid <span style="float:right" class="glyphicon glyphicon-chevron-down glyphicon-align-right" aria-hidden="true"></span></a>
                </h6>
            </div>
            <div id="collapseTwo" class="panel-collapse collapse">
                <div class="panel-body">
			<form class="form-inline">
        <div class="btn-group" data-toggle="buttons">
                <label class="btn btn-primary btn-sm">
                        <input type="radio" name="buySell" id="buy" value="B" autocomplete="off">Buy
                </label>
                <label class="btn btn-primary btn-sm">
                        <input type="radio" name="buySell" id="sell" value="S" autocomplete="off">Sell
                </label>
        </div>
        <div class="input-group input-group-sm">
               <input type="number" id="qty" class="form-control" placeholder="Qty">
        </div>
        <div class="input-group input-group-sm">
               <span class="input-group-addon">$</span>
               <input type="number" id="price" class="form-control" placeholder="Price">
               <input type="hidden" id="exchangeName" name="exchangeName" value="" />
               <input type="hidden" id="productName" name="productName" value="" />
        </div>
        <button type="submit" onclick="return false;" id="tradeButton" class="btn btn-default btn-sm">Trade</button>
</form>	
                </div>
            </div>
        </div>
-->
<!--
    <ul id="tabs" class="nav nav-tabs" data-tabs="tabs">
        <li class="active"><a href="#red" data-toggle="tab">Depth</a></li>
        <li><a href="#orange" data-toggle="tab">Placing/Cancelling bids</a></li>
        <li><a href="#magenta" data-toggle="tab">Products</a></li>
        <li><a href="#yellow" data-toggle="tab">Orders</a></li>
        <li><a href="#green" data-toggle="tab">Fills</a></li>
        <li><a href="#blue" data-toggle="tab">More</a></li>
    </ul>
<div id="my-tab-content" class="tab-content">
        <div class="tab-pane active" id="red">

	</div>
        <div class="tab-pane" id="orange">

	</div>
        <div class="tab-pane" id="magenta">

	</div>
        <div class="tab-pane" id="yellow">

	</div>
        <div class="tab-pane" id="green">

	</div>
        <div class="tab-pane" id="blue">
        </div>
-->

 
 
<script type="text/javascript">
    jQuery(document).ready(function ($) {
        $('#tabs').tab();

	depthToTable("<?php echo $_GET["exchange"] ;?>", "<?php echo $_GET["product"] ;?>", $("#depthTable"), 'class="table table-hover gradienttable" data-sort-name="Product Name" data-sort-order="desc"');

	placeBid("<?php echo $_GET["exchange"] ;?>", "<?php echo $_GET["product"] ;?>", $("#placeBid"));
	//var titi = getOrder("<?php echo $_GET["exchange"] ;?>", "<?php echo $_GET["product"] ;?>", 5, 'admin', function(msg){alert(msg);})
	orderToTable("<?php echo $_GET["exchange"] ;?>", "<?php echo $_GET["product"] ;?>", $("#orderTable"),'admin', 'class="table table-hover gradienttable" data-sort-name="Product Name" data-sort-order="desc"');
	fillToTable("<?php echo $_GET["exchange"] ;?>", "<?php echo $_GET["product"] ;?>", $("#fillTable"),'admin', 'class="table table-hover gradienttable" data-sort-name="Product Name" data-sort-order="desc"');
$('#tabs').tab();
	placeGraph("<?php echo $_GET["exchange"] ;?>", "<?php echo $_GET["product"] ;?>",$("#graph"));
	var firstTime = "<?php echo $_GET["firstTime"]; ?>";
		populatePlaceBid(1,1,"B");
	if (firstTime == "1") {
		$("#placeBidHeader").append("<span id='entice' style='color:red; min-width:200px;'>Try me out!</span>");
		$("#entice").fadeOut(9999);
		populatePlaceBid(1,1,"B");
	}
	productsToTable("<?php echo $_GET["exchange"] ;?>", $("#productList"), 'class="table table-hover gradienttable" data-sort-name="Product Name" data-sort-order="desc"');

	$("#newProductButton").click(function(){
		var newProductI = newProduct("<?php echo $_GET["exchange"] ;?>",$("#newProductName").val(), function(msg) {
			console.log(msg);
			jmsg = JSON.parse(msg);
			if (jmsg["product"]) {
			window.location.href = "/product.php?exchange=<?php echo $_GET["exchange"] ;?>&product=" + jmsg["product"];
			}
			else {
				alert(jmsg["msg"]);
			}
		});
	});

	placeSimpleBid("<?php echo $_GET["exchange"] ;?>", "<?php echo $_GET["product"] ;?>", $("#simpleBids"));
	bidToTable("<?php echo $_GET["exchange"] ;?>", "<?php echo $_GET["product"] ;?>", $("#simpleBidsTable"), 'class="table table-hover gradienttable" data-sort-name="Product Name" data-sort-order="desc"');
});
</script>    
</div> <!-- container -->
 
 
<script type='text/javascript'>(function () { var done = false;var script = document.createElement('script');script.async = true;script.type = 'text/javascript';script.src = 'https://app.purechat.com/VisitorWidget/WidgetScript';document.getElementsByTagName('HEAD').item(0).appendChild(script);script.onreadystatechange = script.onload = function (e) {if (!done && (!this.readyState || this.readyState == 'loaded' || this.readyState == 'complete')) {var w = new PCWidget({ c: '60bfee60-b491-4bbc-9063-0b0d1a0e3e83', f: true });done = true;}};})();</script>
</body>
</html>
