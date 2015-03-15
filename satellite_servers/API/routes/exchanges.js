var express = require('express');
var router = express.Router();
var uuid = require('node-uuid');
var iConn = 0;


router.get('/', function(req, res) {
	res.end('Hello world');
});

router.post('/', function(req, res) {
	console.log('Request for new exchange by: ' + req.body.email + ' / ' + req.body.password);
	if (!req.body.email || !req.body.password) {
		console.log("Got a request with an empty email or password");
		res.end('{"msgType":"ERROR","message":"POST-EXCHANGES:Either the email or the password were empty"}');
	}
	else {
		res.setHeader("Access-Control-Allow-Origin", "*");
		res.setHeader("Access-Control-Allow-Methods", "GET, POST");
		var conn = uuid.v1()  + '-EXCHANGE-POST-' + iConn;
		iConn = iConn + 1;
		//var conn = 0;
		redisCon.multi()
			.select(15)
			.exec(function (err, replies) {
				//conn = uuid.v1();
				redisSub.subscribe('PrivateMessageOUT:' + conn);
				redisCon.publish("newExchange" , '{"conn":"' + conn + '","email":"' + req.body.email + '","password":"' + req.body.password + '","name":"' + req.body.name + '"}');
				//redisSub.addListener("message", function(RedisChannel, message){
		redisSub.once("message", function(RedisChannel, message){
			//var privateMessage = JSON.parse(message);
			//res.send(privateMessage.status);
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
