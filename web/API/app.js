var redis = require('redis');
var express = require('express');
var path = require('path');
var favicon = require('serve-favicon');
var logger = require('morgan');
var cookieParser = require('cookie-parser');
var bodyParser = require('body-parser');

var routes = require('./routes/index');
var objects = require('./routes/objects');
var orders = require('./routes/orders');
var exchanges = require('./routes/exchanges');
var products = require('./routes/products');
var product = require('./routes/product');
var depths = require('./routes/depths');
var fills = require('./routes/fills');
var graphs = require('./routes/graphs');

var app = express();

redisCon = redis.createClient(6379);

redisSub = redis.createClient(6379);

// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'jade');

// uncomment after placing your favicon in /public
//app.use(favicon(__dirname + '/public/favicon.ico'));
app.use(logger('dev'));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

app.use('/', routes);
app.use('', routes);
app.use('/objects', objects);
app.use('/orders', orders);
app.use('/exchanges', exchanges);
app.use('/products', products);
app.use('/product', product);
app.use('/depths', depths);
app.use('/fills', fills);
app.use('/graphs', graphs);

// catch 404 and forward to error handler
app.use(function(req, res, next) {
    var err = new Error('Not Found');
    err.status = 404;
    next(err);
});

// error handlers

// development error handler
// will print stacktrace
if (app.get('env') === 'development') {
    app.use(function(err, req, res, next) {
        res.status(err.status || 500);
        res.render('error', {
            message: err.message,
            error: err
        });
    });
}

// production error handler
// no stacktraces leaked to user
app.use(function(err, req, res, next) {
    res.status(err.status || 500);
    res.render('error', {
        message: err.message,
        error: {}
    });
});


module.exports = app;
