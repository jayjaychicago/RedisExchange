var bidList = [];
var simpleBidList = [];
var orderList = [];
var fillList = [];
var depthTableThere = 0;
var bidTableThere = 0;
var orderTableThere = 0;
var fillTableThere = 0;
var productTableThere = 0;
var instabid_user = "";
var placeBidThere = 0; // that control was added
var placeBidNumberOverall = 0;
var placeBidOpen = 0; // Somebody opened the control to trade
var placeSimpleBidOpen = 0;
var buyNow = 0;
var instabid_ui_loaded = 1;


if (!window.jQuery) {
    var protocol = location.protocol;
    if (protocol !== "http:" && protocol !== "https:") {
        protocol = "http:";
    }
    var script = document.createElement("script");
    //script.onload = foo;
    script.src = protocol + "//ajax.googleapis.com/ajax/libs/jquery/1/jquery.min.js";
    document.head.appendChild(script);
}

// instabid-ui.js subscribes itself to the socket connection and when realtime updates arrive propagates the data to the UIs on the page
function instabid_internal_feedback(msg) {
	var jmsg = JSON.parse(msg); 
	console.log(msg);
	if (jmsg.msgType == "PF") {
		if (depthTableThere == 1) {
        		updateDepthTable(jmsg);
		}
		if (bidTableThere == 1) {
			updateBidTable(jmsg);
		}
	}
	else if (orderTableThere == 1 && jmsg.msgType == "O") {
		updateOrderTable(jmsg);
	}
	else if (fillTableThere == 1 && jmsg.msgType == "F") {
		updateFillTable(jmsg);
	}
}
// This deals with the GUI being disconnected from its server
function instabid_internal_onclose() {
	console.log('Connection lost');
}

function checkIfRealtimeInstabidLoadedOrLoad() {
	if (typeof(instabid_connectSocket) != "function") {
jQuery.getScript("http://www.instabid.io/manage/assets/instabid_realtime.js")
	.done(function() {
		/* yay, all good, do something */
		alert('yay');
	})
	.fail(function() {
			alert('fail');
		/* boo, fall back to something else */
});
/*
    		var protocol = location.protocol;
    		if (protocol !== "http:" && protocol !== "https:") {
        		protocol = "http:";
    		}
    	var script = document.createElement("script");
    	script.src = protocol + "//www.instabid.io/manage/assets/instabid_realtime.js";
    	document.head.appendChild(script);
*/
	}
}


function populatePlaceBid(price,qty, side) {
	if ($("#price").length && $("#qty").length) {
		$("#price").val(price);
		$("#qty").val(qty);
		if (side == "S") {
			$("#placeBidBuy").click();
			//$("#buy").attr('checked', true);
		}
		else {
			$("#placeBidSell").click();
			//$("#sell").attr('checked', true);
		}
	if (placeBidOpen == 0) {
		$("#placeBidHeader").click();
	}
	}
}

function populatePlaceSimpleBid(price,qty, side) {
	if ($("#price").length && $("#qty").length) {
		$("#priceS").val(price);
		$("#qtyS").val(qty);

	if (placeSimpleBidOpen == 0) {
	//	$("#placeBidHeaderS").click();
	}
	}
}

function placeGraph(exchange, product, divToPlaceGraph) {
$.getJSON('http://www.highcharts.com/samples/data/jsonp.php?filename=aapl-c.json&callback=?', function (data) {
        // Create the chart
        divToPlaceGraph.highcharts('StockChart', {


            rangeSelector : {
                selected : 1,
                inputEnabled: $('#container').width() > 480
            },

            title : {
                text : 'test (real graph will build once you trade a little more)'
            },

            series : [{
                name : 'AAPL',
                data : data,
                tooltip: {
                    valueDecimals: 2
                }
            }]
        });
    });
}


function productsToTable(exchange, divToPlaceProducts, classAttributes) {
	productTableThere = 1;
	finalTable = '<TABLE ' + classAttributes + '><THEAD><TR><TH>Product</TH><TH>Highest Bid</TH><TH>Lowest Price</TH><TH>Volume traded</TH><TH>Trade</TH></TR></THEAD><TBODY>';
	var products = getProducts(exchange, function(msg) {
			var jmsg = JSON.parse(msg);
                        var i = 0;
                        while (i<jmsg.PRODUCTS.length) {
                                finalTable = finalTable + "<TR id='instabid_PL" + jmsg.PRODUCTS[i].product + "'><TD id='instabid_P" + jmsg.PRODUCTS[i].product + "'><a href='/product.php?exchange=" + exchange + "&product=" + jmsg.PRODUCTS[i].product + "'>" + jmsg.PRODUCTS[i].product + "&nbsp;<span class='glyphicon glyphicon-pencil' aria-hidden='true'></span></a></TD><TD>-</TD><TD>-</TD><TD>-</TD><TD><div id='placeBid" + jmsg.PRODUCTS[i].product + "'></div></TD></TR>";
//                                console.log(jmsg.SELLDEPTHS[i].QTY + '/' + Math.abs(jmsg.SELLDEPTHS[i].PRICE));
                                i = i + 1;
                        }
			finalTable = finalTable + "</TBODY></TABLE>"
		divToPlaceProducts.append(finalTable);
                        var i = 0;
                        while (i<jmsg.PRODUCTS.length) {
				placeBid(exchange, jmsg.PRODUCTS[i].product, $("#placeBid" + jmsg.PRODUCTS[i].product), '{"border":"0"}');	
				i = i + 1;
			}
	});
}

// Appends an html table with the depth of exchange:product to the divToPlaceTable with the attributes classAttributes
function depthToTable(exchange, product, divToPlaceTable, classAttributes) {
		depthTableThere = 1;
                finalTable = '<TABLE ' + classAttributes + '><THEAD><TR><TH>Bids</TH><TH>Bid Price</TH><TH>Sale Price</TH><TH>Qty for Sale</TH></TR></THEAD>';
                finalTable = finalTable + "<TBODY id='instabid_depthTable_body'>";
                var depth = getDepth(exchange,product,5,function(msg) {
                        var jmsg = JSON.parse(msg);
                        var i = 0;
                        while (i<jmsg.SELLDEPTHS.length) {
				bidList.push([parseFloat(jmsg.SELLDEPTHS[i].PRICE), jmsg.SELLDEPTHS[i].NUM]);
                                finalTable = finalTable + "<TR id='instabid_L" + jmsg.SELLDEPTHS[i].NUM + "' onclick='populatePlaceBid(" + Math.abs(jmsg.SELLDEPTHS[i].PRICE) + "," + jmsg.SELLDEPTHS[i].QTY + ",\"S\")'><TD></TD><TD></TD><TD id='instabid_P" + jmsg.SELLDEPTHS[i].NUM + "'>$" + Math.abs(jmsg.SELLDEPTHS[i].PRICE) + "</TD><TD id='instabid_Q" + jmsg.SELLDEPTHS[i].NUM + "'>" + jmsg.SELLDEPTHS[i].QTY + "</TD></TR>";
//                                console.log(jmsg.SELLDEPTHS[i].QTY + '/' + Math.abs(jmsg.SELLDEPTHS[i].PRICE));
                                i = i + 1;
                        }
                        var i = 0;
                        while (i<jmsg.BUYDEPTHS.length) {
				bidList.push([parseFloat(jmsg.BUYDEPTHS[i].PRICE), jmsg.BUYDEPTHS[i].NUM]);
                                finalTable = finalTable + "<TR id='instabid_L" + jmsg.BUYDEPTHS[i].NUM + "' onclick='populatePlaceBid(" + Math.abs(jmsg.BUYDEPTHS[i].PRICE) + "," + jmsg.BUYDEPTHS[i].QTY + ",\"B\")'><TD id='instabid_Q" + jmsg.BUYDEPTHS[i].NUM + "'>" + jmsg.BUYDEPTHS[i].QTY + "</TD><TD id='instabid_P" + jmsg.BUYDEPTHS[i].NUM + "'>$" + jmsg.BUYDEPTHS[i].PRICE + "</TD><TD></TD><TD></TD></TR>";
//                                console.log(jmsg.BUYDEPTHS[i].QTY + '/' + jmsg.BUYDEPTHS[i].PRICE);
                                i = i + 1;
                        }
                });     
                divToPlaceTable.append(finalTable);
		//checkIfRealtimeInstabidLoadedOrLoad(); // THIS DOES NOT WORK FOR SOME REASON so currently relying on loading the realtime library from the html calling the ui library
		instabid_feed_depth(exchange,product,5);
}



// Appends an html table with the bids of exchange:product to the divToPlaceTable with the attributes classAttributes
function bidToTable(exchange, product, divToPlaceTable, classAttributes) {
		bidTableThere = 1;
                finalTable = '<TABLE ' + classAttributes + '><THEAD><TR><TH>Bids</TH><TH>Bid Price</TH></TR></THEAD>';
                finalTable = finalTable + "<TBODY id='instabid_bidTable_body'>";
                var depth = getDepth(exchange,product,5,function(msg) {
                        var jmsg = JSON.parse(msg);
         /*               var i = 0;
                        while (i<jmsg.SELLDEPTHS.length) {
				bidList.push([parseFloat(jmsg.SELLDEPTHS[i].PRICE), jmsg.SELLDEPTHS[i].NUM]);
                                finalTable = finalTable + "<TR id='instabid_L" + jmsg.SELLDEPTHS[i].NUM + "' onclick='populatePlaceBid(" + Math.abs(jmsg.SELLDEPTHS[i].PRICE) + "," + jmsg.SELLDEPTHS[i].QTY + ",\"S\")'><TD></TD><TD></TD><TD id='instabid_P" + jmsg.SELLDEPTHS[i].NUM + "'>$" + Math.abs(jmsg.SELLDEPTHS[i].PRICE) + "</TD><TD id='instabid_Q" + jmsg.SELLDEPTHS[i].NUM + "'>" + jmsg.SELLDEPTHS[i].QTY + "</TD></TR>";
//                                console.log(jmsg.SELLDEPTHS[i].QTY + '/' + Math.abs(jmsg.SELLDEPTHS[i].PRICE));
                                i = i + 1;
                        } */
                        var i = 0;
                        while (i<jmsg.BUYDEPTHS.length) {
				simpleBidList.push([parseFloat(jmsg.BUYDEPTHS[i].PRICE), jmsg.BUYDEPTHS[i].NUM]);
                                finalTable = finalTable + "<TR id='instabid_SL" + jmsg.BUYDEPTHS[i].NUM + "' onclick='populatePlaceSimpleBid(" + Math.abs(jmsg.BUYDEPTHS[i].PRICE) + "," + jmsg.BUYDEPTHS[i].QTY + ",\"B\")'><TD id='instabid_SQ" + jmsg.BUYDEPTHS[i].NUM + "'>" + jmsg.BUYDEPTHS[i].QTY + "</TD><TD id='instabid_SP" + jmsg.BUYDEPTHS[i].NUM + "'>$" + jmsg.BUYDEPTHS[i].PRICE + "</TD></TR>";
//                                console.log(jmsg.BUYDEPTHS[i].QTY + '/' + jmsg.BUYDEPTHS[i].PRICE);
                                i = i + 1;
                        }
                });     
                divToPlaceTable.append(finalTable);
		//checkIfRealtimeInstabidLoadedOrLoad(); // THIS DOES NOT WORK FOR SOME REASON so currently relying on loading the realtime library from the html calling the ui library
		//instabid_feed_bid(exchange,product,5);
		$("#placeBidHeaderS").click();
		instabid_feed_depth(exchange,product,5);
}



// Appends an html table with the order of exchange:product to the divToPlaceTable with the attributes classAttributes
function orderToTable(exchange, product, divToPlaceTable, user, classAttributes) {
                orderTableThere = 1;
                finalTable = '<TABLE ' + classAttributes + '><THEAD><TR><TH>Side</TH><TH>Bid</TH><TH>Got</TH><TH>Item</TH><TH>Price</TH><TH>Time</TH><TH>Action</TH></TR></THEAD>';
                finalTable = finalTable + "<TBODY id='instabid_orderTable_body'>";
                var order = getOrder(exchange,product,5,user,function(msg) {
			//console.log(msg);
                        var jmsg = JSON.parse(msg);
                        var i = 0;
                        while (i<jmsg.ORDERS.length) {
				var priceO = Math.abs(jmsg.ORDERS[i].PRICE);
				if (parseFloat(jmsg.ORDERS[i].PRICE)>=0) {
					var sideO= "B";
				}
				else {
					var sideO = "S";
				}
				var timestampO = jmsg.ORDERS[i].TIMESTAMP;
				var qtyO = parseInt(jmsg.ORDERS[i].QTY);
				var filledO = parseInt(jmsg.ORDERS[i].QTYFILLED);
				var cancelText = "";
				if ((qtyO - filledO) != 0) {
					cancelText = "Cancel " + (qtyO - filledO) + " left" ;
				}
                                orderList.push([parseFloat(jmsg.ORDERS[i].PRICE), jmsg.ORDERS[i].NUM]);
                                finalTable = finalTable + "<TR id='instabid_OL" + jmsg.ORDERS[i].NUM + "' onclick='populatePlaceBid(" + Math.abs(jmsg.ORDERS[i].PRICE) + "," + jmsg.ORDERS[i].QTY + ",\"S\")'><TD>" + sideO + "</TD><TD id='instabid_OQ" + jmsg.ORDERS[i].NUM + "'>" + jmsg.ORDERS[i].QTY + "</TD><TD id='instabid_OF" + jmsg.ORDERS[i].NUM + "'>" + jmsg.ORDERS[i].QTYFILLED + "</TD><TD>" + product  + "</TD><TD id='instabid_OP" + jmsg.ORDERS[i].NUM + "'>$" + Math.abs(jmsg.ORDERS[i].PRICE) + "</TD><TD id='instabid_OT" + jmsg.ORDERS[i].NUM + "'>20" + timestampO.substr(0,6) + " " + timestampO.substr(6,2) + ':' + timestampO.substr(8,2) + ":" + timestampO.substr(10,2) + "</TD><TD><button>" + cancelText +  " </button></TD></TR>";
//                                console.log(jmsg.SELLDEPTHS[i].QTY + '/' + Math.abs(jmsg.SELLDEPTHS[i].PRICE));
                                i = i + 1;
                        }
                });
                divToPlaceTable.append(finalTable);
                //checkIfRealtimeInstabidLoadedOrLoad(); // THIS DOES NOT WORK FOR SOME REASON so currently relying on loading the realtime library from the html calling the ui library
                instabid_feed_order(exchange,product,5,user);
}


// Appends an html table with the fills of exchange:product to the divToPlaceTable with the attributes classAttributes
function fillToTable(exchange, product, divToPlaceTable, user, classAttributes) {
                fillTableThere = 1;
		instabid_user = user;
                finalTable = '<TABLE ' + classAttributes + '><THEAD><TR><TH>Side</TH><TH>Qty</TH><TH>Item</TH><TH>Price</TH><TH>Time</TH><TH>Buyer</TH><TH>Seller</TH></TR></THEAD>';
                finalTable = finalTable + "<TBODY id='instabid_fillTable_body'>";
                var fill = getFill(exchange,product,5,user,function(msg) {
                        //console.log(msg);
                        var jmsg = JSON.parse(msg);
                        var i = 0;
                        while (i<jmsg.FILLS.length) {
                                var priceF = Math.abs(jmsg.FILLS[i].PRICE);
                                var timestampF = jmsg.FILLS[i].TIMESTAMP;
                                var qtyF = parseInt(jmsg.FILLS[i].QTY);
                                var filledF = parseInt(jmsg.FILLS[i].QTYFILLED);
				var productName = product;
				var buyerF = jmsg.FILLS[i].BUYER;
				var sellerF = jmsg.FILLS[i].SELLER;
                                var sideF= "U";
				if (sellerF == buyerF) {
					sideF = "X";
				}
				else {
					if (user == buyerF) {
						sideF = "B";
					}
					else if (user == sellerF) {
						sideF = "S";
					}
					else {
						sideF = "-";
					}
				}
                                if ((qtyF - filledF) != 0) {
                                        cancelText = "Cancel " + (qtyF - filledF) + " left" ;
                                }
                                fillList.push([parseFloat(jmsg.FILLS[i].PRICE), jmsg.FILLS[i].NUM]);
                                finalTable = finalTable + "<TR id='instabid_FS" + jmsg.FILLS[i].NUM + "'><TD>" + sideF + "</TD><TD id='instabid_FQ" + jmsg.FILLS[i].NUM + "'>" + jmsg.FILLS[i].QTY + "</TD><TD id='instabid_FP" + jmsg.FILLS[i].NUM + "'>" + productName + "</TD><TD>$" + Math.abs(jmsg.FILLS[i].PRICE)  + "</TD><TD id='instabid_FT" + jmsg.FILLS[i].NUM + "'>20" + timestampF.substr(0,6) + " " + timestampF.substr(6,2) + ':' + timestampF.substr(8,2) + ":" + timestampF.substr(10,2) + "</TD><TD>" + buyerF + "</TD><TD>" + sellerF + "</TD></TR>";
//                                console.log(jmsg.SELLDEPTHS[i].QTY + '/' + Math.abs(jmsg.SELLDEPTHS[i].PRICE));
                                i = i + 1;
                        }
                });
                divToPlaceTable.append(finalTable);
                //checkIfRealtimeInstabidLoadedOrLoad(); // THIS DOES NOT WORK FOR SOME REASON so currently relying on loading the realtime library from the html calling the ui library
                instabid_feed_fill(exchange,product,5,user);
}


function placeBid(exchange, product, divToPlaceTable, formatOptions) {

	formatOptions = (typeof formatOptions === "undefined") ? '{"border":"1","simpleBid":"0"}' : formatOptions;
	jFormatOptions = JSON.parse(formatOptions);
	var cc = 0;
	getProduct(exchange,product, function(msg) {
                var jmsg = JSON.parse(msg);
                if (jmsg["result"] == "SUCCESS") {
                        cc = jmsg["cc"]; 
                }       
        });

placeBidThere = 1;
var table = "";

if (cc == 1) {
	table = table + '<form style="display:none;" id="stripeForm" action="" method="POST">';
	  table = table + '<script ';
	    table = table + ' src="https://checkout.stripe.com/checkout.js" class="stripe-button"';
	    table = table + ' data-key="pk_test_6pRNASCoBOKtIshFeQd4XMUh"';
	    table = table + ' data-amount="2000"';
	    table = table + ' data-name="Demo Site"';
	    table = table + ' data-description="Before you can place a bid. We have to pre-authorize your card for the whole value of the bid. You will not be charged except if your bid is accepted"';
	    table = table + ' data-image="/128x128.png">';
	  table = table + '</script>';
	table = table + '</form>';
}

if (jFormatOptions["border"]=="0") {
	var placeBidNumber = placeBidNumberOverall;
}
else {
	var placeBidNumber = "";
}

if (jFormatOptions["simpleBid"]=="1") {
	var simpleBid = true;
	var simpleBidAppend = "S";
}
else {
	var simpleBid = false;
	var simpleBidAppend = "";
}

if (jFormatOptions["border"] == 0) {
	table = table + "<div style='width:380px; '>";
}
table = table + '<div id="bidTool' + placeBidNumber + simpleBidAppend + '" ';
if (jFormatOptions["border"] == 1) {
	table = table + ' class="panel panel-default" style="border: 1px solid transparent; border-color:#DDD;"';
}
table = table + ' >';
if (jFormatOptions["border"] == 1) {
	table = table + '            <div class="panel-heading" style="height:40px; background-image:linear-gradient(to bottom, #F5F5F5 0px, #E8E8E8 100%);">';
	table = table + '                <h6 class="panel-title">';
	table = table + '                    <a id="placeBidHeader' + simpleBidAppend + '" data-toggle="collapse" data-parent="#accordion" href="#collapseTwo' + placeBidNumber + simpleBidAppend + '" onclick="placeBidOpen=1;">' + ((simpleBid) ? 'Make an offer': 'Place/Anwer bids ') + '<span style="float:right" class="glyphicon glyphicon-chevron-down glyphicon-align-right" aria-hidden="true"></span></a>';
	table = table + '                </h6>';
	table = table + '            </div>';
	table = table + '            <div id="collapseTwo' + placeBidNumber + simpleBidAppend + '" class="panel-collapse collapse">';
	table = table + '                <div class="panel-body">';
}
table = table + '                        <form class="form-inline">';
if (simpleBid) {
	table = table + '               <input type="hidden" name="buySell' + placeBidNumber +  simpleBidAppend + '" id="placeBidBuy' + placeBidNumber +  simpleBidAppend + '" value="B" />';
}
else {
	table = table + '        <div class="btn-group" data-toggle="buttons">';
	table = table + '                <label class="btn btn-primary btn-sm">';
	table = table + '                        <input type="radio" name="buySell' + placeBidNumber +  simpleBidAppend + '" id="placeBidBuy' + placeBidNumber +  simpleBidAppend + '" value="B" autocomplete="off">Buy';
	table = table + '                </label>';
	table = table + '                <label class="btn btn-primary btn-sm">';
	table = table + '                        <input type="radio" name="buySell' + placeBidNumber +  simpleBidAppend + '" id="placeBidSell' + placeBidNumber +  simpleBidAppend +  '" value="S" autocomplete="off">Sell';
	table = table + '                </label>';
	table = table + '        </div>';
}
table = table + '        <div class="input-group input-group-sm">';
table = table + '               <input type="number" id="qty' + placeBidNumber +  simpleBidAppend + '" style="max-width:100px;" class="form-control" placeholder="Qty">';
table = table + '        </div>';
table = table + '        <div class="input-group input-group-sm">';
table = table + '               <span class="input-group-addon">$</span>';
table = table + '               <input type="number" id="price' + placeBidNumber +  simpleBidAppend + '" style="max-width:100px;" class="form-control" placeholder="Price">';
table = table + '               <input type="hidden" id="exchangeName' + placeBidNumber +  simpleBidAppend + '" name="exchangeName' + placeBidNumber +  simpleBidAppend + '" value="' + exchange + '" />';
table = table + '               <input type="hidden" id="productName' + placeBidNumber +  simpleBidAppend + '" name="productName' + placeBidNumber +  simpleBidAppend + '" value="' + product + '" />';
table = table + '        </div>';
table = table + '        <button type="submit" onclick="return false;" id="tradeButton' + placeBidNumber +  simpleBidAppend + '" class="btn btn-default btn-sm">' + ((simpleBid) ? 'Bid': 'Trade') + '</button>';
table = table + '</form>';
if (jFormatOptions["border"] == 1) {
	table = table + '                </div>';
	table = table + '            </div>';
}
table = table + '        </div>';
if (jFormatOptions["border"] == 0) {
        table = table + "</div>";
}
divToPlaceTable.append(table);


$("#tradeButton" + placeBidNumber +  simpleBidAppend).click(function() {
		if (cc==1) {
			$(".stripe-button-el")[0].click(function() {
			});
		}
		if (simpleBid) {
			var buySell = $('input[name=buySell' + placeBidNumber +  simpleBidAppend + ']').val();
		} 
		else {
                	var buySell = $('input[name=buySell' + placeBidNumber +  simpleBidAppend + ']:checked').val();
		}
                //alert("You want to trade" + $("#exchangeName").val() + $("#productName").val() + buySell + $("#qty").val() + $("#price").val() + 'admin'  );
                if (buySell == "B" || buySell == "S") {
			var fills = newOrder($("#exchangeName" + placeBidNumber + simpleBidAppend ).val() , $("#productName" + placeBidNumber +  simpleBidAppend).val() , buySell , $("#qty"  + placeBidNumber +  simpleBidAppend).val() , $("#price" + placeBidNumber +  simpleBidAppend).val() , "admin", function(msg) { 
        	//alert(msg);
        	msgJson = JSON.parse(msg);
        	if (msgJson["fillCount"] == 0) { 
                	//$("#productDiv").append('<div class="alert alert-warning"><a href="#" class="close" data-dismiss="alert">&times;</a><strong>Warning!</strong> ' + msg + '</div>');
                	$("#bidTool" + placeBidNumber +  simpleBidAppend).after('<div style="padding:10px; margin-top:10px;" id="alertPop" class="alert alert-warning"><a href="#" class="close" data-dismiss="alert">&times;</a><strong>Bid successfully executed!</strong> However your bid was not immediately filled</div>');
			$("#alertPop").fadeOut(9999);
			$("#alertPop").hover(function() {
				$(this).stop().animate({opacity:'100'}); 
			});
        	}
        	else {
                	for (var i = 0; i < msgJson.fills.length; i++) {
                        	var fill = msgJson.fills[i];
                        	$("#bidTool" + placeBidNumber +  simpleBidAppend).after('<div style="padding:10px; margin-top:10px;" id="alertPop" class="alert alert-warning"><a href="#" class="close" data-dismiss="alert">&times;</a><strong>Succesfully filled!</strong> Filled ' + fill.qty + ' at $' + fill.price + '</div>');
			 $("#alertPop").fadeOut(9999);
                        $("#alertPop").hover(function() {
                                $(this).stop().animate({opacity:'100'});
                        });

                	}
        	}

 			} );
		}
		else {
			alert('Please make sure to select Buy or Sell');
		}
        });
	if (jFormatOptions["border"]=="0") {
		placeBidNumberOverall = placeBidNumberOverall + 1;
	}
}

function placeSimpleBid(exchange, product, divToPlaceTable, formatOptions) {
	return placeBid(exchange, product, divToPlaceTable, '{"border":"1","simpleBid":"1"}');
}

function updateOrderTable(jsonMsg) {
	orderFeedback = "<TR>";
	orderFeedback = orderFeedback + "<TD>" + jsonMsg.S + "</TD>";
	orderFeedback = orderFeedback + "<TD>" + jsonMsg.V + "</TD>";
	orderFeedback = orderFeedback + "<TD>" + "-" + "</TD>";
	orderFeedback = orderFeedback + "<TD>" + jsonMsg.obj.substr(jsonMsg.obj.search(":")+1) + "</TD>";
	orderFeedback = orderFeedback + "<TD>" + "$" + Math.abs(jsonMsg.P) + "</TD>";
	orderFeedback = orderFeedback + "<TD>" + "20" + jsonMsg.T.substr(0,6) + " " + jsonMsg.T.substr(6,2) + ":" + jsonMsg.T.substr(8,2) + ":" + jsonMsg.T.substr(10,2) + "</TD>";
	orderFeedback = orderFeedback + "<TD>" + "<button>Cancel</button>" + "</TD>";
	orderFeedback = orderFeedback + "</TR>";
	
	$("#instabid_orderTable_body").prepend(orderFeedback);
	//alert(orderFeedback);
}

function updateFillTable(jsonMsg) {
	if (jsonMsg.B == jsonMsg.S) {
		var fSide = "X";
	}
	else if (jsonMsg.B == instabid_user) {
		var fSide = "B";
	}
	else if (jsonMsg.S == instabid_user) {
		var fSide = "S";
	}
	else {
		var fSide = "-";
	}
        fillFeedback = "<TR>";
        fillFeedback = fillFeedback + "<TD>" + fSide + "</TD>";
        fillFeedback = fillFeedback + "<TD>" + jsonMsg.V + "</TD>";
        fillFeedback = fillFeedback + "<TD>" + jsonMsg.obj.substr(jsonMsg.obj.search(":")+1) + "</TD>";
        fillFeedback = fillFeedback + "<TD>" + "$" + Math.abs(jsonMsg.P) + "</TD>";
        fillFeedback = fillFeedback + "<TD>" + "-" /* "20" + jsonMsg.T.substr(0,6) + " " + jsonMsg.T.substr(6,2) + ":" + jsonMsg.T.substr(8,2) + ":" + jsonMsg.T.substr(10,2) */ + "</TD>";
        fillFeedback = fillFeedback + "<TD>" + jsonMsg.B + "</TD>";
        fillFeedback = fillFeedback + "<TD>" + jsonMsg.S + "</TD>";
        fillFeedback = fillFeedback + "</TR>";

        $("#instabid_fillTable_body").prepend(fillFeedback);
	
}

function updateDepthTable(jsonMsg) {
	//console.log(msg);
        //var jsonMsg = JSON.parse(msg);
        var msgType = jsonMsg["msgType"];
        if (msgType == "PF") {
                //var jsonMsg = JSON.parse(msg);
                var jproduct = jsonMsg["obj"];
                var jprice = parseFloat(jsonMsg["P"]) ;
                var jqty = jsonMsg["V"];
                var jnum = jsonMsg["N"];
        }
        if ($("#instabid_P" + jnum).length) /* this depth is represented on the table */ {
                if (jqty > 0) {
                        //alert("Changing cell instabid_Q" + jprice + ' / currently is: ' + $("#instabid_Q" + jprice).html());
                        $("#instabid_Q" + jnum).html(jqty);
                }
                else {
                        $("#instabid_L" + jnum).remove();
                        var i = 0;
                        while(jnum != bidList[i][1] && i < bidList.length-1) {
                                //alert('checking line ' + i + ' (max lines: ' + bidList.length + ')');
                                i = i + 1;
                        }
                        bidList.splice(i,1);
                }
        }
        else {
                if (bidList.length == 0) {
                        bidList.push([jprice, jnum]);
                        if (jprice<0) {
				if (jqty == 0) {
					//alert('there'); nothing to do because this new sell self destroys against the buy
				}
				else {
                                	$("#instabid_depthTable_body").html("<TR id='instabid_L" + jnum + "'><TD></TD><TD></TD><TD id='instabid_P" + jnum + "'>$" + Math.abs(jprice) + "</TD><TD id='instabid_Q" + jnum + "'>" + jqty + "</TD></TR>");
				}
                        }
                        else {
                                $("#instabid_depthTable_body").html("<TR id='instabid_L" + jnum + "'><TD id='instabid_Q" + jnum + "'>" + jqty + "</TD><TD id='instabid_P" + jnum + "'>$" + Math.abs(jprice) + "</TD><TD></TD><TD></TD></TR>");
                        }
                }
                else {
                var i = 0;
                while(Math.abs(jprice)<Math.abs(parseFloat(bidList[i][0])) && i < bidList.length-1) {
                        //alert('checking line ' + i + ' (max lines: ' + bidList.length + ')');
                        i = i + 1;
                }
                // don't forget to splice bidList
                if (jprice<0) {
                        if (parseFloat(bidList[i][0])<0) {
					//alert(Math.abs(jprice) + "<" + Math.abs(bidList[i][0]));
				if (i == 0 && (Math.abs(jprice)>Math.abs(bidList[i][0]))) {
                                $("#instabid_L" + bidList[i][1]).before("<TR id='instabid_L" + jnum + "'><TD></TD><TD></TD><TD id='instabid_P" + jnum + "'>$" + Math.abs(jprice) + "</TD><TD id='instabid_Q" + jnum + "'>" + jqty + "</TD></TR>");
                                bidList.splice(i, 0, [jprice, jnum]);
				}
				else {
					if (Math.abs(jprice)<Math.abs(bidList[i][0])) {
                                		$("#instabid_L" + bidList[i][1]).after("<TR id='instabid_L" + jnum + "'><TD></TD><TD></TD><TD id='instabid_P" + jnum + "'>$" + Math.abs(jprice) + "</TD><TD id='instabid_Q" + jnum + "'>" + jqty + "</TD></TR>");
                                		bidList.splice(i+1, 0, [jprice, jnum]);
					}
					else {
                                		$("#instabid_L" + bidList[i][1]).before("<TR id='instabid_L" + jnum + "'><TD></TD><TD></TD><TD id='instabid_P" + jnum + "'>$" + Math.abs(jprice) + "</TD><TD id='instabid_Q" + jnum + "'>" + jqty + "</TD></TR>");
                                		bidList.splice(i, 0, [jprice, jnum]);
					}
				}
                        }
                        else {
                                $("#instabid_L" + bidList[i][1]).before("<TR id='instabid_L" + jnum + "'><TD></TD><TD></TD><TD id='instabid_P" + jnum + "'>$" + Math.abs(jprice) + "</TD><TD id='instabid_Q" + jnum + "'>" + jqty + "</TD></TR>");
                                bidList.splice(i, 0, [jprice, jnum]);
                        }
                }
                else { //new price is positive
			// but placed directly under a sell	
			if (bidList[i][0] < 0) {
					$("#instabid_L" + bidList[i][1]).after("<TR id='instabid_L" + jnum + "'><TD id='instabid_Q" + jnum + "'>" + jqty + "</TD><TD id='instabid_P" + jnum + "'>$" + Math.abs(jprice) + "</TD><TD></TD><TD></TD></TR>");
			bidList.splice(i+1, 0, [jprice, jnum]);
			}
			else {
				if ((i+1) < parseFloat(bidList.length)) {
					$("#instabid_L" + bidList[i][1]).before("<TR id='instabid_L" + jnum + "'><TD id='instabid_Q" + jnum + "'>" + jqty + "</TD><TD id='instabid_P" + jnum + "'>$" + Math.abs(jprice) + "</TD><TD></TD><TD></TD></TR>");
			bidList.splice(i, 0, [jprice, jnum]);
				}
				else {
					if (jprice>parseFloat(bidList[i][0])) {
					$("#instabid_L" + bidList[i][1]).before("<TR id='instabid_L" + jnum + "'><TD id='instabid_Q" + jnum + "'>" + jqty + "</TD><TD id='instabid_P" + jnum + "'>$" + Math.abs(jprice) + "</TD><TD></TD><TD></TD></TR>");
                bidList.splice(i, 0, [jprice, jnum]);
					}
					else {
					$("#instabid_L" + bidList[i][1]).after("<TR id='instabid_L" + jnum + "'><TD id='instabid_Q" + jnum + "'>" + jqty + "</TD><TD id='instabid_P" + jnum + "'>$" + Math.abs(jprice) + "</TD><TD></TD><TD></TD></TR>");
                bidList.splice(i+1, 0, [jprice, jnum]);
					}

				}
			}
                }
        }
        }
        //alert(bidList[0]);

}


function updateBidTable(jsonMsg) {
	//console.log(msg);
        //var jsonMsg = JSON.parse(msg);
        var msgType = jsonMsg["msgType"];
	//alert(msgType);
        if (msgType == "PF") {
                //var jsonMsg = JSON.parse(msg);
                var jproduct = jsonMsg["obj"];
                var jprice = parseFloat(jsonMsg["P"]) ;
                var jqty = jsonMsg["V"];
                var jnum = jsonMsg["N"];
        }
        if ($("#instabid_SP" + jnum).length)  {
                if (jqty > 0) {
                        //alert("Changing cell instabid_Q" + jprice + ' / currently is: ' + $("#instabid_Q" + jprice).html());
                        $("#instabid_SQ" + jnum).html(jqty);
                }
                else {
                        $("#instabid_SL" + jnum).remove();
                        var i = 0;
                        while(jnum != simpleBidList[i][1] && i < simpleBidList.length-1) {
                                //alert('checking line ' + i + ' (max lines: ' + simpleBidList.length + ')');
                                i = i + 1;
                        }
                        simpleBidList.splice(i,1);
                }
        }
        else {
                if (simpleBidList.length == 0) {
                        simpleBidList.push([jprice, jnum]);
                        if (jprice<0) {/*
				if (jqty == 0) {
					//alert('there'); nothing to do because this new sell self destroys against the buy
				}
				else {
                                	$("#instabid_depthTable_body").html("<TR id='instabid_L" + jnum + "'><TD></TD><TD></TD><TD id='instabid_P" + jnum + "'>$" + Math.abs(jprice) + "</TD><TD id='instabid_Q" + jnum + "'>" + jqty + "</TD></TR>");
				}
                        */}
                        else {
                                $("#instabid_bidTable_body").html("<TR id='instabid_SL" + jnum + "'><TD id='instabid_SQ" + jnum + "'>" + jqty + "</TD><TD id='instabid_SP" + jnum + "'>$" + Math.abs(jprice) + "</TD><TD></TD><TD></TD></TR>");
                        }
                }
                else {
                var i = 0;
                while(Math.abs(jprice)<Math.abs(parseFloat(simpleBidList[i][0])) && i < simpleBidList.length-1) {
                        //alert('checking line ' + i + ' (max lines: ' + simpleBidList.length + ')');
                        i = i + 1;
                }
                // don't forget to splice bidList
                if (jprice<0) {/*
                        if (parseFloat(simpleBidList[i][0])<0) {
					//alert(Math.abs(jprice) + "<" + Math.abs(simpleBidList[i][0]));
				if (i == 0 && (Math.abs(jprice)>Math.abs(simpleBidList[i][0]))) {
                                $("#instabid_SL" + simpleBidList[i][1]).before("<TR id='instabid_SL" + jnum + "'><TD></TD><TD></TD><TD id='instabid_SP" + jnum + "'>$" + Math.abs(jprice) + "</TD><TD id='instabid_SQ" + jnum + "'>" + jqty + "</TD></TR>");
                                simpleBidList.splice(i, 0, [jprice, jnum]);
				}
				else {
					if (Math.abs(jprice)<Math.abs(simpleBidList[i][0])) {
                                		$("#instabid_SL" + simpleBidList[i][1]).after("<TR id='instabid_SL" + jnum + "'><TD></TD><TD></TD><TD id='instabid_SP" + jnum + "'>$" + Math.abs(jprice) + "</TD><TD id='instabid_SQ" + jnum + "'>" + jqty + "</TD></TR>");
                                		simpleBidList.splice(i+1, 0, [jprice, jnum]);
					}
					else {
                                		$("#instabid_SL" + simpleBidList[i][1]).before("<TR id='instabid_SL" + jnum + "'><TD></TD><TD></TD><TD id='instabid_SP" + jnum + "'>$" + Math.abs(jprice) + "</TD><TD id='instabid_SQ" + jnum + "'>" + jqty + "</TD></TR>");
                                		simpleBidList.splice(i, 0, [jprice, jnum]);
					}
				}
                        }
                        else {
                                $("#instabid_SL" + simpleBidList[i][1]).before("<TR id='instabid_SL" + jnum + "'><TD></TD><TD></TD><TD id='instabid_SP" + jnum + "'>$" + Math.abs(jprice) + "</TD><TD id='instabid_SQ" + jnum + "'>" + jqty + "</TD></TR>");
                                simpleBidList.splice(i, 0, [jprice, jnum]);
                        }
                */}
                else { //new price is positive
			// but placed directly under a sell	
			if (simpleBidList[i][0] < 0) {
					$("#instabid_SL" + simpleBidList[i][1]).after("<TR id='instabid_SL" + jnum + "'><TD id='instabid_SQ" + jnum + "'>" + jqty + "</TD><TD id='instabid_SP" + jnum + "'>$" + Math.abs(jprice) + "</TD><TD></TD><TD></TD></TR>");
			simpleBidList.splice(i+1, 0, [jprice, jnum]);
			}
			else {
				if ((i+1) < parseFloat(simpleBidList.length)) {
					$("#instabid_SL" + simpleBidList[i][1]).before("<TR id='instabid_SL" + jnum + "'><TD id='instabid_SQ" + jnum + "'>" + jqty + "</TD><TD id='instabid_SP" + jnum + "'>$" + Math.abs(jprice) + "</TD><TD></TD><TD></TD></TR>");
			simpleBidList.splice(i, 0, [jprice, jnum]);
				}
				else {
					if (jprice>parseFloat(simpleBidList[i][0])) {
					$("#instabid_SL" + simpleBidList[i][1]).before("<TR id='instabid_SL" + jnum + "'><TD id='instabid_SQ" + jnum + "'>" + jqty + "</TD><TD id='instabid_SP" + jnum + "'>$" + Math.abs(jprice) + "</TD><TD></TD><TD></TD></TR>");
                simpleBidList.splice(i, 0, [jprice, jnum]);
					}
					else {
					$("#instabid_SL" + simpleBidList[i][1]).after("<TR id='instabid_SL" + jnum + "'><TD id='instabid_SQ" + jnum + "'>" + jqty + "</TD><TD id='instabid_SP" + jnum + "'>$" + Math.abs(jprice) + "</TD><TD></TD><TD></TD></TR>");
                simpleBidList.splice(i+1, 0, [jprice, jnum]);
					}

				}
			}
                }
        }
        }
        //alert(bidList[0]);

}
