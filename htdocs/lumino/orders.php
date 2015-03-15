<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Instabid - Widgets</title>
<?php
session_start();
//ini_set('display_errors',1);
//error_reporting(E_ALL);

        include "instabid.php";
        $email = $_SESSION["email"];
 //       if ($email == "") { header( 'Location: /register.php' ) ; }
?>


<link href="css/bootstrap.min.css" rel="stylesheet">
<link href="css/datepicker3.css" rel="stylesheet">
<link href="css/styles.css" rel="stylesheet">
<script src="//ajax.googleapis.com/ajax/libs/jquery/2.1.3/jquery.min.js"></script>
<script type="text/javascript" src="http://www.instabid.io/manage/assets/instabid.js?version=<?php echo rand(1,1000000); ?>"></script>
<script type="text/javascript" src="http://www.instabid.io/manage/assets/instabid_realtime.js?version=<?php echo rand(1,1000000); ?>"></script>
<script type="text/javascript" src="http://www.instabid.io/manage/assets/instabid-ui.js?version=<?php echo rand(1,1000000); ?>"></script>

<!--[if lt IE 9]>
<script src="js/html5shiv.js"></script>
<script src="js/respond.min.js"></script>
<![endif]-->

<script>
jQuery(document).ready(function ($) {
        $("#liBids").addClass("active");
        $("#sub-item-1").addClass("in");

orderToTable("<?php echo $_GET["exchange"] ;?>", "<?php echo $_GET["product"] ;?>", $("#orderTable"),'admin', 'class="table table-hover gradienttable" data-sort-name="Product Name" data-sort-order="desc"');

});
</script>
</head>

<body>
	<nav class="navbar navbar-inverse navbar-fixed-top" role="navigation">
		<div class="container-fluid">
			<div class="navbar-header">
				<button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#sidebar-collapse">
					<span class="sr-only">Toggle navigation</span>
					<span class="icon-bar"></span>
					<span class="icon-bar"></span>
					<span class="icon-bar"></span>
				</button>
				<a class="navbar-brand" href="#"><span>Instabid</span>Admin</a>
				<ul class="user-menu">
					<li class="dropdown pull-right">
						<a href="#" class="dropdown-toggle" data-toggle="dropdown"><span class="glyphicon glyphicon-user"></span> <?php echo $_SESSION["email"]; ?> <span class="caret"></span></a>
						<ul class="dropdown-menu" role="menu">
							<li><a href="#"><span class="glyphicon glyphicon-user"></span> Profile</a></li>
							<li><a href="#"><span class="glyphicon glyphicon-cog"></span> Settings</a></li>
							<li><a href="/logoff_instabid.php"><span class="glyphicon glyphicon-log-out"></span> Logout</a></li>
						</ul>
					</li>
				</ul>
			</div>
		</div><!-- /.container-fluid -->
	</nav>
		
	<div id="sidebar-collapse" class="col-sm-3 col-lg-2 sidebar">
		<form role="search">
			<div class="form-group">
				<input type="text" class="form-control" placeholder="Search">
			</div>
		</form>
		<ul class="nav menu">
			<li ><a href="dash.php"><span class="glyphicon glyphicon-dashboard"></span> Dashboard</a></li>
			<li class="parent active">
				<a href="#">
					<span class="glyphicon glyphicon-list"></span> Widgets <span data-toggle="collapse" href="#sub-item-1" class="icon pull-right"><em class="glyphicon glyphicon-s glyphicon-plus"></em></span> 
				</a>
				<ul class="children collapse in" id="sub-item-1">
					<li class="active">
						<a href="/lumino/sbc.php?exchange=<?php echo $_GET["exchange"]; ?>&product=<?php echo $_GET["product"] ;?>">
							<span class="glyphicon glyphicon-share-alt"></span> Simple bid collection 
						</a>
					</li>
					<li>
						<a class="" href="/lumino/sd.php?exchange=<?php echo $_GET["exchange"]; ?>&product=<?php echo $_GET["product"] ;?>">
							<span class="glyphicon glyphicon-share-alt"></span> Supply vs Demand 
						</a>
					</li>
					<li>
						<a class="" href="/lumino/orders.php?exchange=<?php echo $_GET["exchange"]; ?>&product=<?php echo $_GET["product"] ;?>">
							<span class="glyphicon glyphicon-share-alt"></span> Orders 
						</a>
					</li>
					<li>
						<a class="" href="/lumino/fills.php?exchange=<?php echo $_GET["exchange"]; ?>&product=<?php echo $_GET["product"] ;?>">
							<span class="glyphicon glyphicon-share-alt"></span> Fills 
						</a>
					</li>
					<li>
						<a class="" href="/lumino/graph.php?exchange=<?php echo $_GET["exchange"]; ?>&product=<?php echo $_GET["product"] ;?>">
							<span class="glyphicon glyphicon-share-alt"></span> Price Graph 
						</a>
					</li>
				</ul>
			</li>
			<li><a href="/lumino/products.php?exchange=<?php echo $_GET["exchange"]; ?>&product=<?php echo $_GET["product"] ;?>"><span class="glyphicon glyphicon-th"></span> My products</a></li>
			<li><a href="/lumino/exchanges.php?exchange=<?php echo $_GET["exchange"]; ?>&product=<?php echo $_GET["product"] ;?>"><span class="glyphicon glyphicon-stats"></span> My exchanges</a></li>
			<li role="presentation" class="divider"></li>
			<li><a href="tables.html"><span class="glyphicon glyphicon-list-alt"></span> Getting started</a></li>
			<li><a href="forms.html"><span class="glyphicon glyphicon-pencil"></span> API docs</a></li>
			<li><a href="panels.html"><span class="glyphicon glyphicon-info-sign"></span> UI docs</a></li>
			<li role="presentation" class="divider"></li>
			<li><a href="login.html"><span class="glyphicon glyphicon-user"></span> Login Page</a></li>
		</ul>
		<div class="attribution">Dashboard by <a href="http://www.instabid.io">Instabid</a></div>
	</div><!--/.sidebar-->
		
	<div class="col-sm-9 col-sm-offset-3 col-lg-10 col-lg-offset-2 main">			
		<div class="row">
			<ol class="breadcrumb">
				<li><a href="#"><span class="glyphicon glyphicon-home"></span></a></li>
				<li class="active">Widgets / Orders</li>
			</ol>
		</div><!--/.row-->
		
		<div class="row">
			<div class="col-lg-12">
				<h1 class="page-header">Widgets</h1>
			</div>
		</div><!--/.row-->
	<!--	
		<div class="row">
			<div class="col-xs-12 col-md-6 col-lg-3">
				<div class="panel panel-blue panel-widget ">
					<div class="row no-padding">
						<div class="col-sm-3 col-lg-5 widget-left">
							<em class="glyphicon glyphicon-shopping-cart glyphicon-l"></em>
						</div>
						<div class="col-sm-9 col-lg-7 widget-right">
							<div class="large">0</div>
							<div class="text-muted">Orders</div>
						</div>
					</div>
				</div>
			</div>
			<div class="col-xs-12 col-md-6 col-lg-3">
				<div class="panel panel-orange panel-widget">
					<div class="row no-padding">
						<div class="col-sm-3 col-lg-5 widget-left">
							<em class="glyphicon glyphicon-transfer glyphicon-l"></em>
						</div>
						<div class="col-sm-9 col-lg-7 widget-right">
							<div class="large">0</div>
							<div class="text-muted">Fills</div>
						</div>
					</div>
				</div>
			</div>
			<div class="col-xs-12 col-md-6 col-lg-3">
				<div class="panel panel-teal panel-widget">
					<div class="row no-padding">
						<div class="col-sm-3 col-lg-5 widget-left">
							<em class="glyphicon glyphicon-tag glyphicon-l"></em>
						</div>
						<div class="col-sm-9 col-lg-7 widget-right">
							<div class="large">1</div>
							<div class="text-muted">Products</div>
						</div>
					</div>
				</div>
			</div>
			<div class="col-xs-12 col-md-6 col-lg-3">
				<div class="panel panel-red panel-widget">
					<div class="row no-padding">
						<div class="col-sm-3 col-lg-5 widget-left">
							<em class="glyphicon glyphicon-home glyphicon-l"></em>
						</div>
						<div class="col-sm-9 col-lg-7 widget-right">
							<div class="large">1</div>
							<div class="text-muted">Exchange</div>
						</div>
					</div>
				</div>
			</div>
		</div>-->
		
		<div class="row">
			<div class="col-lg-12">
				<div class="panel panel-default">
					<div class="panel-heading">Orders</div>
					<div class="panel-body">
					<div id="orderTable"></div>
						<!--
						<div class="canvas-wrapper">
							<canvas class="main-chart" id="line-chart" height="200" width="600"></canvas>
						</div>
						-->
					</div>
				</div>
			</div>
		</div><!--/.row-->
	<!--	
		<div class="row">
			<div class="col-xs-6 col-md-3">
				<div class="panel panel-default">
					<div class="panel-body easypiechart-panel">
						<h4>New Orders</h4>
						<div class="easypiechart" id="easypiechart-blue" data-percent="92" ><span class="percent">92%</span>
						</div>
					</div>
				</div>
			</div>
			<div class="col-xs-6 col-md-3">
				<div class="panel panel-default">
					<div class="panel-body easypiechart-panel">
						<h4>Comments</h4>
						<div class="easypiechart" id="easypiechart-orange" data-percent="65" ><span class="percent">65%</span>
						</div>
					</div>
				</div>
			</div>
			<div class="col-xs-6 col-md-3">
				<div class="panel panel-default">
					<div class="panel-body easypiechart-panel">
						<h4>New Users</h4>
						<div class="easypiechart" id="easypiechart-teal" data-percent="56" ><span class="percent">56%</span>
						</div>
					</div>
				</div>
			</div>
			<div class="col-xs-6 col-md-3">
				<div class="panel panel-default">
					<div class="panel-body easypiechart-panel">
						<h4>Visitors</h4>
						<div class="easypiechart" id="easypiechart-red" data-percent="27" ><span class="percent">27%</span>
						</div>
					</div>
				</div>
			</div>
		</div> -->

		<div class="row">
			<div class="col-md-8">
			
				<div class="panel panel-default chat">
					<div class="panel-heading" id="accordion"><span class="glyphicon glyphicon-comment"></span> Hello World</div>
					<div class="panel-body">
<p>The basic Instabid bidding tool is very simple. It allows you to capture your users bids for your products.</p>
<p>It uses Instabid.js and Instabid.php working together to send the bids to the Instabid cloud. The answers (whether the bids are filled or not) are then displayed using instabid_realtime.js</p>
<p>You will need to download these files and put them in your web root directory.</p>
<p>Also, all libraries use jQuery and bootstrap to look beautiful.</p>
<p>Here's a simple "Hello World":</p>
<span class="hljs-string"><XMP style="color:black; font-size:8px;">
<HTML>
<?php echo "<?php"; ?>

include "instabid.php";
$userId = $_SESSION["userId"]; // Instabid needs userIds but does not maintain users. 
authorize_user_to_bid($userId); // This tells instabid that you're ok for this user to bid. Typically done in the login processing form
<?php echo "?>"; ?>

<HEAD>
<script src="//ajax.googleapis.com/ajax/libs/jquery/2.1.3/jquery.min.js"></script>
<script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.1/js/bootstrap.min.js"></script>
<script type="text/javascript" src="http://www.instabid.io/manage/assets/instabid.js>"></script>
<script type="text/javascript" src="http://www.instabid.io/manage/assets/instabid_realtime.js>"></script>
<script type="text/javascript" src="http://www.instabid.io/manage/assets/instabid-ui.js>"></script>
<SCRIPT>
jQuery(document).ready(function ($) {

var exchange = "1234567890"; // You can find your exchange number in the My exchanges tab
var product = "test"; // You can find your product name in the My products tab

orderToTable(exchange, product, $("#orderTable"),'admin', 'class="table table-hover gradienttable" data-sort-name="Product Name" data-sort-order="desc"');

});

</SCRIPT>
</HEAD>
<BODY>
	<div id="orderTable">
</BODY>
</HTML>
</XMP></span>

						<!--<ul>
							<li class="left clearfix">
								<span class="chat-img pull-left">
									<img src="http://placehold.it/80/30a5ff/fff" alt="User Avatar" class="img-circle" />
								</span>
								<div class="chat-body clearfix">
									<div class="header">
										<strong class="primary-font">John Doe</strong> <small class="text-muted">32 mins ago</small>
									</div>
									<p>
										Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nulla ante turpis, rutrum ut ullamcorper sed, dapibus ac nunc. Vivamus luctus convallis mauris, eu gravida tortor aliquam ultricies. 
									</p>
								</div>
							</li>
							<li class="right clearfix">
								<span class="chat-img pull-right">
									<img src="http://placehold.it/80/dde0e6/5f6468" alt="User Avatar" class="img-circle" />
								</span>
								<div class="chat-body clearfix">
									<div class="header">
										<strong class="pull-left primary-font">Jane Doe</strong> <small class="text-muted">6 mins ago</small>
									</div>
									<p>
										Mauris dignissim porta enim, sed commodo sem blandit non. Ut scelerisque sapien eu mauris faucibus ultrices. Nulla ac odio nisl. Proin est metus, interdum scelerisque quam eu, eleifend pretium nunc. Suspendisse finibus auctor lectus, eu interdum sapien.
									</p>
								</div>
							</li>
							<li class="left clearfix">
								<span class="chat-img pull-left">
									<img src="http://placehold.it/80/30a5ff/fff" alt="User Avatar" class="img-circle" />
								</span>
								<div class="chat-body clearfix">
									<div class="header">
										<strong class="primary-font">John Doe</strong> <small class="text-muted">32 mins ago</small>
									</div>
									<p>
										Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nulla ante turpis, rutrum ut ullamcorper sed, dapibus ac nunc. Vivamus luctus convallis mauris, eu gravida tortor aliquam ultricies. 
									</p>
								</div>
							</li>
						</ul>-->
					</div>
				<!--	
					<div class="panel-footer">
						<div class="input-group">
							<input id="btn-input" type="text" class="form-control input-md" placeholder="Type your message here..." />
							<span class="input-group-btn">
								<button class="btn btn-success btn-md" id="btn-chat">Send</button>
							</span>
						</div>
					</div> -->
				</div>
				
			</div><!--/.col-->
			
			<div class="col-md-4">
			
				<div class="panel panel-blue">
					<div class="panel-heading dark-overlay"><span class="glyphicon glyphicon-check"></span>To-do List</div>
					<div class="panel-body">
						<ul class="todo-list">
						<li class="todo-list-item">
								<div class="checkbox">
									<input type="checkbox" id="checkbox" />
									<label for="checkbox">Download <a href="/download/instabid.php">instabid.php</a></label>
								</div>
								<div class="pull-right action-buttons">
									<a href="#"><span class="glyphicon glyphicon-pencil"></span></a>
									<a href="#" class="flag"><span class="glyphicon glyphicon-flag"></span></a>
									<a href="#" class="trash"><span class="glyphicon glyphicon-trash"></span></a>
								</div>
							</li>
							<li class="todo-list-item">
								<div class="checkbox">
									<input type="checkbox" id="checkbox" />
									<label for="checkbox">Download <a href="/download/instabid.js">instabid.js</a></label>
								</div>
								<div class="pull-right action-buttons">
									<a href="#"><span class="glyphicon glyphicon-pencil"></span></a>
									<a href="#" class="flag"><span class="glyphicon glyphicon-flag"></span></a>
									<a href="#" class="trash"><span class="glyphicon glyphicon-trash"></span></a>
								</div>
							</li>
							<li class="todo-list-item">
								<div class="checkbox">
									<input type="checkbox" id="checkbox" />
									<label for="checkbox">Download <a href="/download/instabid_realtime.js">instabid_realtime.js</a></label>
								</div>
								<div class="pull-right action-buttons">
									<a href="#"><span class="glyphicon glyphicon-pencil"></span></a>
									<a href="#" class="flag"><span class="glyphicon glyphicon-flag"></span></a>
									<a href="#" class="trash"><span class="glyphicon glyphicon-trash"></span></a>
								</div>
							</li>
							<li class="todo-list-item">
								<div class="checkbox">
									<input type="checkbox" id="checkbox" />
									<label for="checkbox">Include jQuery and Bootstrap</label>
								</div>
								<div class="pull-right action-buttons">
									<a href="#"><span class="glyphicon glyphicon-pencil"></span></a>
									<a href="#" class="flag"><span class="glyphicon glyphicon-flag"></span></a>
									<a href="#" class="trash"><span class="glyphicon glyphicon-trash"></span></a>
								</div>
							</li>
							<li class="todo-list-item">
								<div class="checkbox">
									<input type="checkbox" id="checkbox" />
									<label for="checkbox">Include instabid libraries</label>
								</div>
								<div class="pull-right action-buttons">
									<a href="#"><span class="glyphicon glyphicon-pencil"></span></a>
									<a href="#" class="flag"><span class="glyphicon glyphicon-flag"></span></a>
									<a href="#" class="trash"><span class="glyphicon glyphicon-trash"></span></a>
								</div>
							</li>
							<li class="todo-list-item">
								<div class="checkbox">
									<input type="checkbox" id="checkbox" />
									<label for="checkbox">Begin bidding!</label>
								</div>
								<div class="pull-right action-buttons">
									<a href="#"><span class="glyphicon glyphicon-pencil"></span></a>
									<a href="#" class="flag"><span class="glyphicon glyphicon-flag"></span></a>
									<a href="#" class="trash"><span class="glyphicon glyphicon-trash"></span></a>
								</div>
							</li>
						</ul>
					</div>
<!--
					<div class="panel-footer">
						<div class="input-group">
							<input id="btn-input" type="text" class="form-control input-md" placeholder="Add new task" />
							<span class="input-group-btn">
								<button class="btn btn-primary btn-md" id="btn-todo">Add</button>
							</span>
						</div>
					</div> -->
				</div>
								
			</div><!--/.col-->
		</div><!--/.row-->
	</div>	<!--/.main-->

	<script src="js/jquery-1.11.1.min.js"></script>
	<script src="js/bootstrap.min.js"></script>
	<script src="js/chart.min.js"></script>
	<script src="js/chart-data.js"></script>
	<script src="js/easypiechart.js"></script>
	<script src="js/easypiechart-data.js"></script>
	<script src="js/bootstrap-datepicker.js"></script>
	<script>
		$('#calendar').datepicker({
		});

		!function ($) {
		    $(document).on("click","ul.nav li.parent > a > span.icon", function(){          
		        $(this).find('em:first').toggleClass("glyphicon-minus");      
		    }); 
		    $(".sidebar span.icon").find('em:first').addClass("glyphicon-plus");
		}(window.jQuery);

		$(window).on('resize', function () {
		  if ($(window).width() > 768) $('#sidebar-collapse').collapse('show')
		})
		$(window).on('resize', function () {
		  if ($(window).width() <= 767) $('#sidebar-collapse').collapse('hide')
		})
	</script>	
</body>

</html>
