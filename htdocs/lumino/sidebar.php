
	<div id="sidebar-collapse" class="col-sm-3 col-lg-2 sidebar">
		<form role="search">
			<div class="form-group">
				<input type="text" class="form-control" placeholder="Search">
			</div>
		</form>
		<ul class="nav menu">
			<li id="liProducts"><a href="/lumino/products.php?exchange=<?php echo $_GET["exchange"]; ?>&product=<?php echo $_GET["product"] ;?>"><span class="glyphicon glyphicon-th"></span> My products</a></li>
			<!--<li ><a href="/lumino/dash.php?exchange=<?php echo $_GET["exchange"]; ?>&product=<?php echo $_GET["product"] ;?>"><span class="glyphicon glyphicon-dashboard"></span> Dashboard</a></li>-->
			<li class="parent " id="liBids">
				<a href="/lumino/sbc.php?exchange=<?php echo $_GET["exchange"]; ?>&product=<?php echo $_GET["product"] ;?>">
					<span class="glyphicon glyphicon-list"></span> Bids <span data-toggle="collapse" href="#sub-item-1" class="icon pull-right"><em class="glyphicon glyphicon-s glyphicon-plus"></em></span> 
				</a>
				<ul class="children collapse " id="sub-item-1">
					<li >
						<a href="/lumino/sbc.php?exchange=<?php echo $_GET["exchange"]; ?>&product=<?php echo $_GET["product"] ;?>" id="sbc">
							<span class="glyphicon glyphicon-share-alt"></span> For Buyers 
						</a>
					</li>
					<li>
						<a class="" href="/lumino/sd.php?exchange=<?php echo $_GET["exchange"]; ?>&product=<?php echo $_GET["product"] ;?>" id="sd">
							<span class="glyphicon glyphicon-share-alt"></span> For Marketplace 
						</a>
					</li>
					<!--<li>
						<a class="" href="/lumino/orders.php?exchange=<?php echo $_GET["exchange"]; ?>&product=<?php echo $_GET["product"] ;?>">
						<span class="glyphicon glyphicon-share-alt"></span> Bid History 
					</a>
				</li>
				<li>
					<a class="" href="/lumino/fills.php?exchange=<?php echo $_GET["exchange"]; ?>&product=<?php echo $_GET["product"] ;?>">
						<span class="glyphicon glyphicon-share-alt"></span> Completed deals 
					</a>
				</li>
				<li>
					<a class="" href="/lumino/graph.php?exchange=<?php echo $_GET["exchange"]; ?>&product=<?php echo $_GET["product"] ;?>">
						<span class="glyphicon glyphicon-share-alt"></span> Price History 
					</a>
				</li>-->
			</ul>
		</li>
		<!--<li><a href="/lumino/exchanges.php?exchange=<?php echo $_GET["exchange"]; ?>&product=<?php echo $_GET["product"] ;?>"><span class="glyphicon glyphicon-stats"></span> My exchanges</a></li>-->
		<li role="presentation" class="divider"></li>
		<li><a href="tables.html"><span class="glyphicon glyphicon-list-alt"></span> Getting started</a></li>
		<li class="parent ">
		<a href="#">
                                        <span class="glyphicon glyphicon-list"></span> API docs <span data-toggle="collapse" href="#api-sub-item-1" class="icon pull-right"><em class="glyphicon glyphicon-s glyphicon-plus"></em></span> 
	</a>
                                <ul class="children collapse " id="api-sub-item-1">
                                        <li >
                                                	<a class="" href="/lumino/api-sbc.php?exchange=<?php echo $_GET["exchange"]; ?>&product=<?php echo $_GET["product"] ;?>">
                                                        <span class="glyphicon glyphicon-share-alt"></span> Place a bid 
                                                </a>
                                        </li>
                                        <li>
                                                <a class="" href="/lumino/api-sd.php?exchange=<?php echo $_GET["exchange"]; ?>&product=<?php echo $_GET["product"] ;?>">
                                                        <span class="glyphicon glyphicon-share-alt"></span> Answer bids 
                                                </a>
                                        </li>
                                        <li>
                                                <a class="" href="/lumino/api-orders.php?exchange=<?php echo $_GET["exchange"]; ?>&product=<?php echo $_GET["product"] ;?>">
                                                <span class="glyphicon glyphicon-share-alt"></span> Bid History 
                                        </a>
                                </li>
                                <li>
                                        <a class="" href="/lumino/api-fills.php?exchange=<?php echo $_GET["exchange"]; ?>&product=<?php echo $_GET["product"] ;?>">
                                                <span class="glyphicon glyphicon-share-alt"></span> Completed deals 
                                        </a>
                                </li>
                                <li>
                                        <a class="" href="/lumino/api-graph.php?exchange=<?php echo $_GET["exchange"]; ?>&product=<?php echo $_GET["product"] ;?>">
                                                <span class="glyphicon glyphicon-share-alt"></span> Price History 
                                        </a>
                                </li>
                                <li>
                                        <a class="" href="/lumino/api-products.php?exchange=<?php echo $_GET["exchange"]; ?>&product=<?php echo $_GET["product"] ;?>">
                                                <span class="glyphicon glyphicon-share-alt"></span> List products
                                        </a>
                                </li>
                        </ul>
                </li>
		<li class="parent " id="liUiDocs">
					<a href="#">
                                        <span class="glyphicon glyphicon-list"></span> UI docs <span data-toggle="collapse" href="#ui-sub-item-1" class="icon pull-right"><em class="glyphicon glyphicon-s glyphicon-plus"></em></span> 
					</a>
                                <ul class="children collapse " id="ui-sub-item-1">
                                        <li >
							<a class="" href="/lumino/ui-sbc.php?exchange=<?php echo $_GET["exchange"]; ?>&product=<?php echo $_GET["product"] ;?>" id="uisbc">
                                                        <span class="glyphicon glyphicon-share-alt"></span> Place a bid 
                                                </a>
                                        </li>
                                        <li>
                                                <a class="" href="/lumino/ui-sd.php?exchange=<?php echo $_GET["exchange"]; ?>&product=<?php echo $_GET["product"] ;?>" id="uisd">
                                                        <span class="glyphicon glyphicon-share-alt"></span> Answer bids 
                                                </a>
                                        </li>
                                        <li>
                                                <a class="" href="/lumino/ui-orders.php?exchange=<?php echo $_GET["exchange"]; ?>&product=<?php echo $_GET["product"] ;?>" id="uiorders">
                                                <span class="glyphicon glyphicon-share-alt"></span> Bid History 
                                        </a>
                                </li>
                                <li>
                                        <a class="" href="/lumino/ui-fills.php?exchange=<?php echo $_GET["exchange"]; ?>&product=<?php echo $_GET["product"] ;?>" id="uifills">
                                                <span class="glyphicon glyphicon-share-alt"></span> Completed deals 
                                        </a>
                                </li>
                                <li>
                                        <a class="" href="/lumino/ui-graph.php?exchange=<?php echo $_GET["exchange"]; ?>&product=<?php echo $_GET["product"] ;?>" id="uigraph">
                                                <span class="glyphicon glyphicon-share-alt"></span> Price History 
                                        </a>
                                </li>
                                <li>
                                        <a class="" href="/lumino/ui-products.php?exchange=<?php echo $_GET["exchange"]; ?>&product=<?php echo $_GET["product"] ;?>" id="uiproducts">
                                                <span class="glyphicon glyphicon-share-alt"></span> List products
                                        </a>
                                </li>
                        </ul>
                </li>
		<li role="presentation" class="divider"></li>
		<li><a href="settings.php?exchange=<?php echo $_GET["exchange"]; ?>&product=<?php echo $_GET["product"] ;?>"><span class="glyphicon glyphicon-cog"></span> Settings</a></li>
		</ul>
	</div>
