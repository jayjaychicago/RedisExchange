RedisExchange
=============

A universal trading engine.
You want to create a fantasy football trading site? A ad exchange? A bartering application on your website? An RFP where suppliers can compete for your contracts? Anything with a bidding tool?
This will allow you to do so.
The full live solution can be used at www.instabid.io

Requirements
============

You need an ubuntu server running a recent version of redis to run this. You also need ruby, python, node.js, express, sockjs the server and client libraries
The ruby version of the code requires Deamons too so that it can run in the background.
If you want to serve the trading engine online for web browsers to trade, you also need to run Node.js

What it does
============
This creates a trading engine that will take execution orders for anything.
Say that you want to create a bitcoin exchange.
You want your user to make a javascript call from her/his browser:

Using instabid.js:
newOrder('bitcoin exchange', 'bitcoin', 'Buy', 20, $300, 'user123', function() {alert('hurray!');}); 

That generates a post method that is sent to instabid.php which will add a privateKey and send it to the API server (api.js a node server) that will in turn be transported by the redis pub/sub to the exchange server exchange.rb

At this point, the exchange server exchange.rb will match it against sale orders and reply to the API call. At the same time, it will send a redis pub/sub message to the websocket server (socketServer.js). This guy will send a message to all web browser client (instabid_realtime.js) who have subscribed to messages about the object 'bitcoin', part of the exchange 'bitcoin exchange').

Tada! You have a real time exchange of anything and everything using Jquery, websockets, a rest API based on Node.js and express, a websocket server based on node.js and express and an exchange server written in Ruby (migrated to C++ if you look at the follow distro: https://github.com/patefacio/RedisExchange.git)
