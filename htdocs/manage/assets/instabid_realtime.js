    var instabid_listen = []; // list of the real time services you're subscribed to
    var alreadyAskedDepth = [];
    var alreadyAskedL1 = [];
    var alreadyAskedOrder = [];
    var alreadyAskedFill = [];
    var sockjs;
    var protocol = location.protocol;
    if (protocol !== "http:" && protocol !== "https:") {
        protocol = "http:";
    }
    var script = document.createElement("script");
    script.onload = foo;
    script.src = protocol + "//cdn.jsdelivr.net/sockjs/0.3.4/sockjs.min.js";
    //script.src = protocol + "//cdn.sockjs.org/sockjs-0.3.min.js";
    document.head.appendChild(script);

function foo() {
	sockjs = instabid_connectSocket();
}

function instabid_connectSocket() {
   var sockjs_url = "http://www.instabid.io:8880/channel"; 
      sockjs = new SockJS(sockjs_url);

    var print = function(message){
	//alert(message);

	if (typeof(instabid_internal_feedback) == "function" && instabid_ui_loaded == 1) {
		instabid_internal_feedback(message);
	}
	else if (typeof(instabid_feedback) == "function") {
		instabid_feedback(message);
	} else {
		console.log('You have to add the instabid_feedback(json string) in your code to consume this instabid feedback: ' + message);
	}
    }

    sockjs.onopen    = function(e) {
	instabid_feed(sockjs);
    }

    sockjs.onmessage = function(e) {
   /*             var oldCountAsk = countAsk;
                var oldCountBid = countBid;
                if (oldCountAsk == 0 ) {location.reload();};
                if (oldCountBid == 0 ) {location.reload();}; */
                print(e.data); 
    }
    sockjs.onclose   = function()  {

        if (typeof(instabid_internal_onclose) == "function" && instabid_ui_loaded == 1) {
                instabid_internal_onclose();
        }
        else if (typeof(instabid_onclose) == "function") {
                instabid_onclose();
        } else {
                console.log('You have to add the instabid_onclose() function in your code to react to Instabid getting disconnected from the exchange server' );
        }

};

    return sockjs;

}

function getDepthUpdate(exchange, product, level, sockjsIn) {
// This provides an update whenever the depth for a specific market/product gets updated. The depth is the various
// prices and qtys of bids (and ask) for a product grouped by side and by price
	//console.log('requesting ALLDEPTH for exchange ' +  exchange + ' product' + product + ' level: ' + level );
	sockjsIn.send('{"event":"AllDepth","channel_id":"' + exchange + ':' + product + '","nick":"admin"}' );
}

function getHighestBidLowestAsk(exchange, product, sockjsIn) {
	sockjsIn.send('{"event":"SimplePriceFeed","channel_id":"' + exchange + ':' + product + '","nick":"admin"}' );
}

function getOrderUpdate(exchange, product, level, user, sockjsIn) {
        sockjsIn.send('{"event":"OrderFeed","channel_id":"' + exchange + ':' + product + '","nick":"' + user + '"}' );
}

function getFillUpdate(exchange, product, level, user, sockjsIn) {
        sockjsIn.send('{"event":"FillFeed","channel_id":"' + exchange + ':' + product + '","nick":"' + user + '"}' );
}

function instabid_feed(sockjs1) {
// This is the function called on open of the socket
// It reads all the feed requests called by the developer
// sends them to the socket server which sends them to the exchange server
// via the pub sub
	if (instabid_listen.length > 0) {
		for (var i = 0, len = instabid_listen.length; i < len; i++) {
			feedRequestType = instabid_listen[i][0];
		//alert('The table tell us to ask the server to send us: ' + instabid_listen[i][0]);
		switch (feedRequestType) {
			case "SIMPLEPRICEFEED":
		//		alert('asking the server to send us simple price');
				getHighestBidLowestAsk(instabid_listen[i][1],instabid_listen[i][2],sockjs1);
				break;
			case "ALLDEPTH":
		//		alert('asking the server to send us all depth');
				getDepthUpdate(instabid_listen[i][1],instabid_listen[i][2],instabid_listen[i][3],sockjs1);
				break;
			case "ORDER":
		//		alert('asking the server to send us order');
				getOrderUpdate(instabid_listen[i][1],instabid_listen[i][2],instabid_listen[i][3],instabid_listen[i][4],sockjs1);
				break;
			case "FILL":
		//		alert('asking the server to send us fill');
				getFillUpdate(instabid_listen[i][1],instabid_listen[i][2],instabid_listen[i][3],instabid_listen[i][4], sockjs1);
				break;
			default:
				console.log('Did not recognize the Instabid feed request');
		}
		}
	}
}

function instabid_feed_depth(exchange, product, level) {
//function called by developer to get depth
	//alert(alreadyAskedDepth.indexOf('ALLDEPTH' + exchange + product));
	if (alreadyAskedDepth.indexOf('ALLDEPTH' + exchange + product) == -1) {
		instabid_listen.push(['ALLDEPTH', exchange, product, level]);
	}
	alreadyAskedDepth.push('ALLDEPTH' + exchange + product);
}

function instabid_feed_highest_bid_and_lowest_sell(exchange, product) {
//function called by developer to get level 1 depth
	if (alreadyAskedL1.indexOf('SIMPLEPRICEFEED' + exchange + product) == -1) {
		instabid_listen.push(['SIMPLEPRICEFEED', exchange, product]);
	}
	alreadyAskedL1.push('SIMPLEPRICEFEED' + exchange + product);
}

function instabid_feed_order(exchange, product, level, user) {
//function called by developer to get depth
	if (alreadyAskedOrder.indexOf('ORDER' + exchange + product + user) == -1) {
        	instabid_listen.push(['ORDER', exchange, product, level, user]);
	}
	alreadyAskedOrder.push('ORDER' + exchange + product + user);
	
}

function instabid_feed_fill(exchange, product, level, user) {
//function called by developer to get depth
	if (alreadyAskedFill.indexOf('FILL' + exchange + product + user) == -1) {
        	instabid_listen.push(['FILL', exchange, product, level, user]);
	}
	alreadyAskedFill.push('FILL' + exchange + product + user);
	
}
