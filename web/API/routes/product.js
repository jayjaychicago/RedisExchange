var express = require('express');
var router = express.Router();
var uuid = require('node-uuid');
var iConn = 0;

router.get('/', function(req, res) {
//      console.log(req);
        var conn = uuid.v1()  + '-PRODUCT-GET' + iConn;
        console.log("*** PRODUCT GET***");
	var exchange = req.body.exchangeName;
	var product = req.body.name;
	console.log('exchangeName: ' + exchange);
	console.log('product name: ' + product);
        if (!req.body.exchangeName || !req.body.name) {
                console.log("Got a request with an empty Exchange");
                res.end('{"msgType":"ERROR","message":"GET PRODUCT: the product name or exchangeName was empty"}');
        }
        else {
                iConn = iConn + 1;
                var exchange = req.body.exchangeName;
		var product = req.body.name;
                console.log('***** GET product details for this exchange *****');
                if (exchange != '') {
                redisCon.multi()
                        .select(15)
                        .exec(function (err, replies) {
                                //conn = uuid.v1();
                                redisSub.subscribe('PrivateMessageOUT:' + conn);
                                redisCon.publish("productDetails", '{"conn":"' + conn + '","exchange":"' + exchange + '","name":"' + product + '","token":"' + conn + '"}');
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

router.patch('/', function(req, res) {
	console.log('*** PRODUCT PATCH *****');
	console.log('exchangeName: ' + req.body.exchange);
	console.log('product: ' + req.body.name);
	console.log('property: ' + req.body.property);
	console.log('value: ' + req.body.value);
	console.log('Request to set product of name: ' + req.body.name + ' / ' + req.body.privateKey + ' on exchange ' + req.body.exchange);
                                if (!req.body.name || !req.body.exchange) {
                                        console.log("Got a request with an empty product or Exchange");
                                        res.end('{"msgType":"ERROR","message":"POST products: Either the productName or the exchangeName were empty"}');
                                }
                                else {
                                        res.setHeader("Access-Control-Allow-Origin", "*");
                                        res.setHeader("Access-Control-Allow-Methods", "GET, POST, PATCH");
        //res.end('Congratulations!');
                var conn = uuid.v1() + '-PRODUCT-PATCH' + iConn;
                iConn = iConn + 1;
                //var conn = 0;
                redisCon.multi()
                        .select(15)
                        .exec(function (err, replies) {
                                //conn = uuid.v1();
                                redisSub.subscribe('PrivateMessageOUT:' + conn);
                                redisCon.publish("setProduct" , '{"conn":"' + conn + '","name":"' + req.body.name + '","privateKey":"' + req.body.privateKey + '","exchange":"' + req.body.exchange + '","property":"' + req.body.property + '","value":"' + req.body.value + '" }');
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


module.exports = router;
