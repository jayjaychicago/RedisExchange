var express = require('express');
var router = express.Router();
var uuid = require('node-uuid');
var iConn = 0;


router.get('/', function(req, res) {
//	console.log(req);
        var conn = uuid.v1() + '-DEPTH-GET' + iConn;
	iConn = iConn +1;
	console.log("*** DEPTH GET***");
	if (!req.body.productName || !req.body.exchangeName) {
		console.log("Got a request with an empty product or Exchange");
		res.end('{"msgType":"ERROR","message":"GET DEPTHS: Either the productName or the exchangeName were empty"}');
	}
	else {
		console.log(conn);
		var level = req.body.level;
		if (level == null) { level = 10; }
		if (level > 10) { level = 10; }
		console.log('Request for depth');
		console.log('productName: ' + req.body.productName);
		console.log('exchangeName: ' + req.body.exchangeName);
		console.log('level: ' + req.body.level);
		console.log('level corrected: ' + level);
		//var conn = 0;
		if (req.body.productName != '') {
		redisCon.multi()
			.select(15)
			.exec(function (err, replies) {
				//conn = uuid.v1();
				redisSub.subscribe('PrivateMessageOUT:' + conn);
				redisCon.publish("GETDEPTH:" + conn , '{"conn":"' + conn + '","productName":"' + req.body.productName + '","level":"' + level + '", "token":"' + conn + '","exchangeName":"' + req.body.exchangeName + '"}');
				redisSub.once("message", function(RedisChannel, message){
					//var privateMessage = JSON.parse(message);
					//res.send(privateMessage.status);
					console.log('PROPAGATING DEPTH: ' + message);
					//redisSub.removeListener("message", function() {});
					redisSub.unsubscribe('PrivateMessageOUT:' + conn);
					console.log('Making sure were disconnected from redis for the depth');
					res.end(message);
				});
			});
		}
	}
});


module.exports = router;
