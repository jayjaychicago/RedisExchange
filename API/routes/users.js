var express = require('express');
var router = express.Router();
var redis = require('redis');
var uuid = require('node-uuid');

redisCon = redis.createClient(6379);

redisSub = redis.createClient(6379);

router.get('/createUser', function(req, res) {
        //var conn = uuid.v1();
        var conn = 0;
        redisCon.multi()
                .select(15)
                .exec(function (err, replies) {
                        conn = uuid.v1();
                        redisSub.subscribe('PrivateMessageOUT:' + conn);
                        redisCon.publish("PrivateMessageIN:" + conn, '{"action":"createUser","firstName":"' + req.query.firstName) + '"}';
                        redisSub.on("message", function(RedisChannel, message){
                                var privateMessage = JSON.parse(message);
                                res.send(privateMessage.status);
                                redisSub.unsubscribe('PrivateMessageOUT:' + conn);
                        });
                });
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
