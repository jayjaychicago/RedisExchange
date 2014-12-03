RedisExchange
=============

A trading engine with a redis datastore

Requirements
============

You need an ubuntu server running a recent version of redis to run this.
The ruby version of the code requires Deamons too so that it can run in the background.
If you want to serve the trading engine online for web browsers to trade, you also need to run Node.js

What it does
============
This creates a trading engine that will take execution orders via json over Redis channels.
More to come soon
