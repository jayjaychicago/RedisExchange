var express = require('express');
var router = express.Router();
var redis = require('redis');

redisCon = redis.createClient(6379);

router.get('/test', function(req, res) {
redisCon.set(req.query.id,"bar");
        redisCon.get(req.query.id, function(error, result) {
                if (error) console.log("Error: " + error);
                else res.json(result);
        });
});


router.get('/test2/:name', function(req, res) {
redisCon.set(req.params.name,"bar");
        redisCon.get(req.params.name, function(error, result) {
                if (error) console.log("Error: " + error);
                else res.json(result);
        });
});


router.get('/test3/:name', function(req, res) {
redisCon.hmset(req.params.name, { '1':'b' , '2':'a' , '3' : 'r'} );
        redisCon.hgetall(req.params.name, function(error, result) {
                if (error) console.log("Error: " + error);
                else {
                        res.json(result);
                     }
        });
});

module.exports = router;
