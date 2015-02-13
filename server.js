// SockJS Dynamic Multiple Channel Example
// Copyright (c)2013 Mahesh Raju / coder@mahesh.net.
// Distributed under the Apache License, Version 2.0
// https://github.com/wrecker/sockjs-dynamic-channels

var express = require('express');
var http = require('http');
var path = require('path');
var util = require('util');
var redis   = require('redis'); 

var app = express();
var sockjs = require('sockjs');
var sockjs_server = sockjs.createServer();

// all environments
app.set('port', process.env.PORT || 8880);
app.use(express.favicon());
app.use(express.logger('dev'));
app.use(express.json());
app.use(express.urlencoded());
app.use(express.methodOverride());
app.use(express.cookieParser('your secret here'));
app.use(express.session());
app.use(app.router);
app.use(express.static(path.join(__dirname, 'public')));

// development only
if ('development' == app.get('env')) {
  app.use(express.errorHandler());
}

// To keep track of connections attached to a channel
var channel_conn_map = {};
// To keep track of a connections channel
var uuid_channel_map = {};

var sockets = {};

redisSub = redis.createClient();
redisPub = redis.createClient();
redisSub.on("message", function(RedisChannel, message){
	// This painful piece of code could profit from changing the pub/sub format from SimpleQtyFeedB:1 message {"msgType":"QB","V":"1"} to QB1 message {"msgType":"QB","V":"1"}
//	console.log('On channel ' + RedisChannel + ' RAW REDIS MESSAGE: ' + message);
	console.log('Testing ' + RedisChannel.substr(0,8) + ' to see if it s equal to FillFeed:');
	if (RedisChannel.substr(0,16) == 'SimplePriceFeed:')
		{
		//	console.log('Sending simplePriceFeed update ' + message + ' from redis to all in channel S' + RedisChannel.substr(16));
			sendEventToChannel('S' + RedisChannel.substr(16), "SimplePriceFeed", message,"SERVER");
		}
	else if (RedisChannel.substr(0,17) == "PrivateMessageOUT")
		{
		//	console.log('Sending PrivateMessageOut ' + message + ' to ' + RedisChannel.substr(18));
			sendEventToListener(sockets[RedisChannel.substr(18)], 'PrivateMessageOUT', message);
		}
 	else if (RedisChannel.substr(0,9) == 'PriceFeed')
                {
                        console.log(RedisChannel);
                        console.log('Sending PriceFeed message ' + message + ' to all in channel A' + RedisChannel.substr(10));
			sendEventToChannel('A' + RedisChannel.substr(10), "PriceFeed", message, "SERVER","SERVER");
                }
        else if (RedisChannel.substr(0,16) == 'SimplePriceFeedB')
                {
                        console.log(RedisChannel);
                        console.log('Sending SimplePriceFeedB message ' + message + ' to all in channel B' + RedisChannel.substr(17));
                        sendEventToChannel('B' + RedisChannel.substr(17), "PriceFeedB", message, "SERVER","SERVER");
                }
        else if (RedisChannel.substr(0,14) == 'SimpleQtyFeed:')
                {
                        console.log(RedisChannel);
                        console.log('Sending SimpleQtyFeed message ' + message + ' to all in channel QS' + RedisChannel.substr(14));
                        sendEventToChannel('QS' + RedisChannel.substr(14), "SimpleQtyFeed", message, "SERVER","SERVER");
                }
        else if (RedisChannel.substr(0,14) == 'SimpleQtyFeedB')
                {
                        console.log(RedisChannel);
                        console.log('Sending SimpleQtyFeedB message ' + message + ' to all in channel QB' + RedisChannel.substr(15));
                        sendEventToChannel('QB' + RedisChannel.substr(15), "SimpleQtyFeedB", message, "SERVER","SERVER");
                }
         else if (RedisChannel.substr(0,13) == 'LastPriceFeed')
                {
                        console.log(RedisChannel);
                        console.log('Sending LastPriceFeed message ' + message + ' to all in channel L' + RedisChannel.substr(14));
                        sendEventToChannel('L' + RedisChannel.substr(14), "LastPriceFeed", message, "SERVER","SERVER");
                }
         else if (RedisChannel.substr(0,20) == 'DailyTotalVolumeFeed')
                {
                        console.log(RedisChannel);
                        console.log('Sending DailyTotalVolumeFeed message ' + message + ' to all in channel DTV' + RedisChannel.substr(21));
                        sendEventToChannel('DTV' + RedisChannel.substr(21), "DailyTotalVolumeFeed", message, "SERVER","SERVER");
                }
         else if (RedisChannel.substr(0,8) == 'FillFeed')
                {
                        console.log(RedisChannel);
                        console.log('Sending FillFeed message ' + message + ' to all in channel F' + RedisChannel.substr(9));
                        sendEventToChannel('F' + RedisChannel.substr(9), "FillFeed", message, "SERVER","SERVER");
                }
         else if (RedisChannel.substr(0,9) == 'OrderFeed')
                {
                        console.log(RedisChannel);
                        console.log('Sending OrderFeed message ' + message + ' to all in channel O' + RedisChannel.substr(10));
                        sendEventToChannel('O' + RedisChannel.substr(10), "OrderFeed", message, "SERVER","SERVER");
                }
        else if (RedisChannel.substr(0,16) == 'ObjectDetailFeed')
                {
                        console.log(RedisChannel);
                        console.log('Sending ObjectDetail message ' + message + ' to all in channel OD' + RedisChannel.substr(17));
                        sendEventToChannel('OD' + RedisChannel.substr(17), "ObjectDetailFeed", message, "SERVER","SERVER");
                }
	else
		{
		//	console.log("I don't know that message");	
		}

});

// Send a JSON object to all listeners of a channel.
// If from_uuid matches a valid listener connection, the message
// will not be sent to that connection.
function sendEventToChannel(channel_id, event, data, from_uuid, from_nick) {
    from_uuid = from_uuid || "0";
    from_nick = from_nick || "";

    // Get List of other listeners on this channel and
    // send the message.
    console.log('Sending ' + data + ' to all members of channel ' + channel_id );
    console.log("********************** Checking if there is a conn stored in channel_conn_map[" + channel_id + "]");
    conns = channel_conn_map[channel_id];
    if (conns) {
    console.log("********************** Looking up all the conns in the matrix channel_conn_map[" + channel_id + "] to see if one has an id of " + from_uuid);
    for (var i = 0; i < conns.length; i++) {
        if (conns[i].id === from_uuid)
            continue;
	    console.log("found one!");
        sendEventToListener(conns[i], event, data, from_nick);
    }
	       }
    else {
//	console.log("********************* There was no conns inside channel_conn_map[" + channel_id + "]");
//	console.log('Major issue: Looked up all the conns for channel_id: ' + channel_id + ' and found Nothing. Very unexpected');
	}
}

// Send a JSON message to the connection.
function sendEventToListener(conn, event, data, nick) {
    //nick = nick || "";
    conn.write(data/*JSON.stringify({event: event, data: data, nick: nick})*/);
}

function isArray(what) {
    return Object.prototype.toString.call(what) === '[object Array]';
}

sockjs_server.on('connection', function(conn) {
    console.log("Start Connection: " + conn);
    redisSub.subscribe('PrivateMessageOUT:' + conn);
    conn.on('data', function(ws_msg) {
	sockets[conn+''] = conn;	
        console.log('Remote Message: ' + ws_msg);

	try {
        	var msgJSON = JSON.parse(ws_msg);
    	}
	catch(e) {
	//	console.log(e);
	}
	if (!isArray(msgJSON)) {
		msgJSON[0] = msgJSON;
	}
	
//	console.log('The incoming message from the browser contained ' + msgJSON.length + ' JSON objects')
       	i=0
       	do {
		msg = msgJSON[i];
		try {
			var msgEvent = msg.event;
		}
		catch(e) {
			console.log(e);
			msgEvent = "unknown";
		}
	//	console.log('Analyzing message number ' + i + ' which is an event ' + msg.event);
		if (msgEvent === 'JoinChannel') {
            			if (msg.channel_id) {
                			conn.nick = msg.nick;
                			if (!(msg.channel_id in channel_conn_map)) {
                    				channel_conn_map[msg.channel_id] = [];
                			}
                			channel_conn_map[msg.channel_id].push(this);
                			uuid_channel_map[this.id] = msg.channel_id;

                			redisSub.subscribe('SimplePriceFeed:' + msg.channel_id);
                			redisSub.subscribe('PrivateMessageOUT:' + conn);

                			sendEventToListener(this, "JoinedChannel", "Hello!");
                			// Notify everyone about the new listener and count
                			sendEventToChannel(msg.channel_id, "ListenerJoined", msg.nick, conn.id);
                			sendEventToChannel(msg.channel_id, "#Listeners", channel_conn_map[msg.channel_id].length);
            			} 
				else {
                			conn.close(1, "Missing Channel ID");
                		//	console.log("Missing Channel ID. Closing Connection.");
            			}
        		}
        	else if (msgEvent === 'SimplePriceFeed') {
            		redisObject = msg.channel_id;
               		msg.channel_id = 'S' + msg.channel_id;
            		if (redisObject) {
                		conn.nick = msg.nick;
                		if (!(msg.channel_id in channel_conn_map)) {
                  			channel_conn_map[msg.channel_id] = [];
                		}
                		channel_conn_map[msg.channel_id].push(this);
                		//console.log("Adding " + this + " to the channel_conn_map[" + msg.channel_id + "]");
                		uuid_channel_map[this.id] = msg.channel_id;

                		redisSub.subscribe('SimplePriceFeed:' + redisObject);
                		//redisSub.subscribe('PrivateMessageOUT:' + conn);
		                redisPub.publish('PrivateMessageIN:' + conn, '{"messageType":"RequestBestDepth","objectId":"' + redisObject +'"}');
        	        } else {
	                		conn.close(1, "Missing Channel ID");
                		//	console.log("Missing Channel ID. Closing Connection.");
            	   	  }
        	}
                else if (msgEvent === 'SimplePriceFeedB') {
                        redisObject = msg.channel_id;
                        msg.channel_id = 'B' + msg.channel_id;
                        if (redisObject) {
                                conn.nick = msg.nick;
                                if (!(msg.channel_id in channel_conn_map)) {
                                        channel_conn_map[msg.channel_id] = [];
                                }
                                channel_conn_map[msg.channel_id].push(this);
                                //console.log("Adding " + this + " to the channel_conn_map[" + msg.channel_id + "]");
                                uuid_channel_map[this.id] = msg.channel_id;

                                redisSub.subscribe('SimplePriceFeedB:' + redisObject);
                                //redisSub.subscribe('PrivateMessageOUT:' + conn);
                                redisPub.publish('PrivateMessageIN:' + conn, '{"messageType":"RequestBestDepthB","objectId":"' + redisObject +'"}');
                        } else {
                                        conn.close(1, "Missing Channel ID");
                                        console.log("Missing Channel ID. Closing Connection.");
                          }
                }
                else if (msgEvent === 'SimpleQtyFeed') {
                        redisObject = msg.channel_id;
                        msg.channel_id = 'QS' + msg.channel_id;
		//	console.log("********************** Because we received notification from the browser, we know that we want SimpleQtyFeed to communicate over channel " + msg.channel_id);
                        if (redisObject) {
                                conn.nick = msg.nick;
			//	console.log("******************* Asking if " + msg.channel_id + " exist as an index in our matrix of conns channel_conn_map[]" );
                                if (!(msg.channel_id in channel_conn_map)) {
				//	console.log("***************** That matrix doesn't exist so we create it: channel_conn_map[" + msg.channel_id + "]");
                                        channel_conn_map[msg.channel_id] = [];
                                }
                                channel_conn_map[msg.channel_id].push(this);
			//	console.log("******************* And add the whole conn into the matrix");
                                //console.log("Adding " + this + " to the channel_conn_map[" + msg.channel_id + "]");
                                uuid_channel_map[this.id] = msg.channel_id;
			//	console.log("******************* and keep track of the channel id in the conn id matrix: uuid_channel_map[" + this.id + "] = " + msg.channel_id);

                                redisSub.subscribe('SimpleQtyFeed:' + redisObject);
                                //redisSub.subscribe('PrivateMessageOUT:' + conn);
                                redisPub.publish('PrivateMessageIN:' + conn, '{"messageType":"RequestQtyFeed","objectId":"' + redisObject +'"}');
                        } else {
                                        conn.close(1, "Missing Channel ID");
                                        console.log("Missing Channel ID. Closing Connection.");
                          }
                }
                else if (msgEvent === 'SimpleQtyFeedB') {
                        redisObject = msg.channel_id;
                        msg.channel_id = 'QB' + msg.channel_id;
                        if (redisObject) {
                                conn.nick = msg.nick;
                                if (!(msg.channel_id in channel_conn_map)) {
                                        channel_conn_map[msg.channel_id] = [];
                                }
                                channel_conn_map[msg.channel_id].push(this);
                                //console.log("Adding " + this + " to the channel_conn_map[" + msg.channel_id + "]");
                                uuid_channel_map[this.id] = msg.channel_id;

                                redisSub.subscribe('SimpleQtyFeedB:' + redisObject);
                                //redisSub.subscribe('PrivateMessageOUT:' + conn);
                                redisPub.publish('PrivateMessageIN:' + conn, '{"messageType":"RequestQtyFeedB","objectId":"' + redisObject +'"}');
                        } else {
                                        conn.close(1, "Missing Channel ID");
                                        console.log("Missing Channel ID. Closing Connection.");
                          }
                }
		else if (msgEvent === 'AllDepth') {
                	redisObject = msg.channel_id;
        	        msg.channel_id = 'A' + msg.channel_id;
	            if (redisObject) {
                	conn.nick = msg.nick;
        	        if (!(msg.channel_id in channel_conn_map)) {
	                    channel_conn_map[msg.channel_id] = [];
                	}
        	        channel_conn_map[msg.channel_id].push(this);
 	               uuid_channel_map[this.id] = msg.channel_id;

                	redisSub.subscribe('PriceFeed:' + redisObject);
                	//redisSub.subscribe('PrivateMessageOUT:' + conn);
        	    } else {
	                conn.close(1, "Missing Channel ID");
                //	console.log("Missing Channel ID. Closing Connection.");
            	      }
        	}
		else if (msgEvent === 'DailyTotalVolume') {
                        redisObject = msg.channel_id;
                        msg.channel_id = 'DTV' + msg.channel_id;
                    if (redisObject) {
                        conn.nick = msg.nick;
                        if (!(msg.channel_id in channel_conn_map)) {
                            channel_conn_map[msg.channel_id] = [];
                        }
                        channel_conn_map[msg.channel_id].push(this);
                       uuid_channel_map[this.id] = msg.channel_id;

                        redisSub.subscribe('DailyTotalVolumeFeed:' + redisObject);
                        //redisSub.subscribe('PrivateMessageOUT:' + conn);
                        redisPub.publish('PrivateMessageIN:' + conn, '{"messageType":"RequestDailyTotalVolume","objectId":"' + redisObject +'"}');
 
                        //redisSub.subscribe('PrivateMessageOUT:' + conn);
                    } else {
                        conn.close(1, "Missing Channel ID");
                        console.log("Missing Channel ID. Closing Connection.");
                      }
                }
                else if (msgEvent === 'LastPrice') {
                        redisObject = msg.channel_id;
                        msg.channel_id = 'L' + msg.channel_id;
                    if (redisObject) {
                        conn.nick = msg.nick;
                        if (!(msg.channel_id in channel_conn_map)) {
                            channel_conn_map[msg.channel_id] = [];
                        }
                        channel_conn_map[msg.channel_id].push(this);
                       uuid_channel_map[this.id] = msg.channel_id;

                        redisSub.subscribe('LastPriceFeed:' + redisObject);
                        //redisSub.subscribe('PrivateMessageOUT:' + conn);
                        redisPub.publish('PrivateMessageIN:' + conn, '{"messageType":"RequestLastPrice","objectId":"' + redisObject +'"}');

                        //redisSub.subscribe('PrivateMessageOUT:' + conn);
                    } else {
                        conn.close(1, "Missing Channel ID");
                        console.log("Missing Channel ID. Closing Connection.");
                      }
                }
                else if (msgEvent === 'FillFeed') {
                        redisObject = msg.channel_id;
                        msg.channel_id = 'F' + msg.channel_id + ':USER:' + msg.nick;
                    if (redisObject) {
                        conn.nick = msg.nick;
                        if (!(msg.channel_id in channel_conn_map)) {
                            channel_conn_map[msg.channel_id] = [];
                        }
                        channel_conn_map[msg.channel_id].push(this);
                       uuid_channel_map[this.id] = msg.channel_id;

                        redisSub.subscribe('FillFeed:' + redisObject + ':USER:' + msg.nick);
                        //redisSub.subscribe('PrivateMessageOUT:' + conn);
                        //redisPub.publish('PrivateMessageIN:' + conn, '{"messageType":"RequestLastPrice","objectId":"' + redisObject +'"}');

                        //redisSub.subscribe('PrivateMessageOUT:' + conn);
                    } else {
                        conn.close(1, "Missing Channel ID");
                        console.log("Missing Channel ID. Closing Connection.");
                      }
                }
                else if (msgEvent === 'OrderFeed') {
                        redisObject = msg.channel_id;
                        msg.channel_id = 'O' + msg.channel_id;
                    if (redisObject) {
                        conn.nick = msg.nick;
                        if (!(msg.channel_id in channel_conn_map)) {
                            channel_conn_map[msg.channel_id] = [];
                        }
                        channel_conn_map[msg.channel_id].push(this);
                       uuid_channel_map[this.id] = msg.channel_id;

                        redisSub.subscribe('OrderFeed:' + redisObject);
                        //redisSub.subscribe('PrivateMessageOUT:' + conn);
                        //redisPub.publish('PrivateMessageIN:' + conn, '{"messageType":"RequestLastPrice","objectId":"' + redisObject +'"}');

                        //redisSub.subscribe('PrivateMessageOUT:' + conn);
                    } else {
                        conn.close(1, "Missing Channel ID");
                        console.log("Missing Channel ID. Closing Connection.");
                      }
                }
                else if (msgEvent === 'ObjectDetail') {
                        redisObject = msg.channel_id;
                        msg.channel_id = 'OD' + msg.channel_id;
                    if (redisObject) {
                        conn.nick = msg.nick;
                        if (!(msg.channel_id in channel_conn_map)) {
                            channel_conn_map[msg.channel_id] = [];
                        }
                        channel_conn_map[msg.channel_id].push(this);
                       uuid_channel_map[this.id] = msg.channel_id;

                        redisSub.subscribe('ObjectDetailFeed:' + redisObject);
                        //redisSub.subscribe('PrivateMessageOUT:' + conn);
                        //redisPub.publish('PrivateMessageIN:' + conn, '{"messageType":"RequestLastPrice","objectId":"' + redisObject +'"}');

                        //redisSub.subscribe('PrivateMessageOUT:' + conn);
                    } else {
                        conn.close(1, "Missing Channel ID");
                        console.log("Missing Channel ID. Closing Connection.");
                      }
                }

	        else {
            		if (!msgEvent) {
                	//	console.log("Invalid message format: " + message);
        	    	} 
			else {
        	        	// Get Channel-ID from the uuid of the sender
	                	channel_id = uuid_channel_map[conn.id];
                		//sendEventToChannel(channel_id, "Message", msg.data, conn.id,
        	         	//       conn.nick);
	            	}
        	}
		i++;
       	} while (i<msgJSON.length);
    });

    conn.on('close', function() {
        console.log("Connection's UUID:" + this.id);
        // Get Channel-ID from the uuid of the connection
        channel_id = uuid_channel_map[this.id];
        conns = channel_conn_map[channel_id];
        // Remove conn from channel's list of connections
        if (typeof conns != 'undefined') conns.splice(conns.indexOf(this), 1);
        delete uuid_channel_map[this.id];
	delete sockets[conn];
        console.log("Lost Connection.");
        // Notify everyone about the new listener count
        /*if (typeof channel_conn_map[channel_id] != 'undefined')
            sendEventToChannel(channel_id, "ListenerLeft", conn.nick, conn.id);
            sendEventToChannel(channel_id, "#Listeners",
                    channel_conn_map[channel_id].length); */
    });

});

app.get('/', function (req, res) {
    res.sendfile(__dirname + '/index.html');
});

var server = http.createServer(app);
server.listen(app.get('port'), function(){
  console.log('Express server listening on port ' + app.get('port'));
});

sockjs_server.installHandlers(server, {prefix:'/channel'});
