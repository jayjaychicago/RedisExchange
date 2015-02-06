var express = require('express');
var router = express.Router();
var uuid = require('node-uuid');
var iConn = 0;

router.get('/', function(req, res) {

//      console.log(req);
        var conn = uuid.v1()  + '-PRODUCTS-GET' + iConn;
        console.log("*** PRODUCTS GET***");
        if (!req.body.exchangeName) {
                console.log("Got a request with an empty Exchange");
                res.end('{"msgType":"ERROR","message":"GET PRODUCTS: the productName was empty"}');
        }
	else {
		iConn = iConn + 1;
		var exchange = req.body.exchangeName;
		console.log('***** GET products for this exchange *****');
		console.log('exchangeName: ' + exchange);
		if (exchange != '') {
		redisCon.multi()
			.select(15)
			.exec(function (err, replies) {
				//conn = uuid.v1();
				redisSub.subscribe('PrivateMessageOUT:' + conn);
				redisCon.publish("productsByExchange", '{"conn":"' + conn + '","exchange":"' + exchange + '","token":"' + conn + '"}');
				//redisSub.addListener("message", function(RedisChannel, message){
				redisSub.once("message", function(RedisChannel, message){
					//var privateMessage = JSON.parse(message);
					//res.send(privateMessage.status);
					console.log('PROPAGATING PRODUCT: ' + message);
					//redisSub.removeListener("message", function() {});
					redisSub.unsubscribe('PrivateMessageOUT:' + conn);
					res.end(message);
					});
			});
		}
	}
});

router.post('/', function(req, res) {
				console.log('Request for new product of name: ' + req.body.name + ' / ' + req.body.privateKey + ' on exchange ' + req.body.exchange);
				if (!req.body.name || !req.body.exchange) {
					console.log("Got a request with an empty product or Exchange");
					res.end('{"msgType":"ERROR","message":"POST products: Either the productName or the exchangeName were empty"}');
				}
				else {
					res.setHeader("Access-Control-Allow-Origin", "*");
					res.setHeader("Access-Control-Allow-Methods", "GET, POST");
	//res.end('Congratulations!');
		var conn = uuid.v1() + '-PRODUCT-POST' + iConn;
		iConn = iConn + 1;
		//var conn = 0;
		redisCon.multi()
			.select(15)
			.exec(function (err, replies) {
				//conn = uuid.v1();
				redisSub.subscribe('PrivateMessageOUT:' + conn);
				redisCon.publish("newProduct" , '{"conn":"' + conn + '","name":"' + req.body.name + '","privateKey":"' + req.body.privateKey + '","exchange":"' + req.body.exchange + '" }');
				redisSub.once("message", function(RedisChannel, message){
	//                        redisSub.addListener("message", function(RedisChannel, message){
					//var privateMessage = JSON.parse(message);
					//res.send(privateMessage.status);
					console.log(message);
					//redisSub.removeListener("message", function() {});
					redisSub.unsubscribe('PrivateMessageOUT:' + conn);
					res.end(message);
				});
			});
		}
});

/*
 redisCon.multi()
        .select(15)
        .publish("createUser", req.query.firstName)
        .exec(function (err, replies) {
                if (err) console.log("Error: " + err);
                else {
                        replies.forEach(function (reply, index) {
                        console.log("Reply " + index + ": " + reply.toString());
                        });
                res.send('User successfully created');
                }
        });
});
*/


module.exports = router;
