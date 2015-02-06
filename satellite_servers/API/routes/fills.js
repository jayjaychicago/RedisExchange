var express = require('express');
var router = express.Router();
var uuid = require('node-uuid');
var iConn = 0;


router.get('/', function(req, res) {
//      console.log(req);
        var conn = uuid.v1()  + '-FILLS-GET' + iConn;
        console.log("*** FILLS GET***");
        if (!req.body.productName || !req.body.exchangeName) {
                console.log("Got a request with an empty product or Exchange");
                res.end('{"msgType":"ERROR","message":"GET-FILLS:Either the productName or the exchangeName were empty"}');
        }
	else {
		iConn = iConn + 1;
		var level = req.body.level;
		if (level == null) { level = 10; }
		if (level > 10) { level = 10; }
		console.log('Request for fills');
		console.log('productName: ' + req.body.productName);
		console.log('exchangeName: ' + req.body.exchangeName);
		console.log('level: ' + req.body.level);
		console.log('level corrected: ' + level);
		console.log('user: ' + req.body.user);
		//var conn = 0;
		if (req.body.productName != '') {
		redisCon.multi()
			.select(15)
			.exec(function (err, replies) {
				//conn = uuid.v1();
				redisSub.subscribe('PrivateMessageOUT:' + conn);
				redisCon.publish("GETFILL:" + conn , '{"conn":"' + conn + '","productName":"' + req.body.productName + '","level":"' + level + '", "token":"' + conn + '","exchangeName":"' + req.body.exchangeName + '","user":"' + req.body.user + '"}');
				//redisSub.addListener("message", function(RedisChannel, message){
				redisSub.once("message", function(RedisChannel, message){
                                //var privateMessage = JSON.parse(message);
                                //res.send(privateMessage.status);
                                console.log('PROPAGATING FILL: ' + message);
				//redisSub.removeListener("message", function() {});
                                redisSub.unsubscribe('PrivateMessageOUT:' + conn);
                                res.end(message);
				});
			});
		}
        }
});
/*
router.post('/', function(req, res) {
        var conn = uuid.v1();
        //var conn = 0;
        redisCon.multi()
                .select(15)
                .exec(function (err, replies) {
                        //conn = uuid.v1();
                        redisSub.subscribe('PrivateMessageOUT:' + conn);
			console.log(req.body.side + ' ' + req.body.qty + ' @ ' + req.body.price + ' of ' + req.body.exchange + ':' + req.body.product);
                        redisCon.publish("ORDERS:" + conn , '{"conn":"' + conn + '","object":"' + req.body.exchange + ':' + req.body.product + '","side":"' + req.body.side + '", "type":"9", "qty":"' + req.body.qty + '", "price":"' + req.body.price + '", "user":"' + req.body.user + '", "venue":"3"}');
                        redisSub.on("message", function(RedisChannel, message){
                                //var privateMessage = JSON.parse(message);
                                //res.send(privateMessage.status);
                                redisSub.unsubscribe('PrivateMessageOUT:' + conn);
                                res.end(message);
                        });
                });
});
*/
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
