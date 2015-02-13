if (!window.jQuery) {
    var protocol = location.protocol;
    if (protocol !== "http:" && protocol !== "https:") {
        protocol = "http:";
    }
    var script = document.createElement("script");
    //script.onload = foo;
    script.src = protocol + "//ajax.googleapis.com/ajax/libs/jquery/1/jquery.min.js";
    document.head.appendChild(script);
}

function setInstabidProduct(exchange, product, property, value, callback) {
var dta = {
        exchange: exchange,
        product: product,
	property: property,
	value: value,
        APICall: 'setProduct'
        };

var jqxhr = $.ajax({
        type: 'POST',
        url: '/instabid.php',
        async: false,
        data: dta,
        success: callback,
        error: function(jqXHR,textStatus,errorThrown) {
                console.log( jqXHR.status );
                console.log( textStatus );
                console.log( errorThrown );
        }
    });

}

function newExchange(email, password, name, callback) {

var dta = {
	email: email,
	password: password,
	name: name
	};

var jqxhr = $.ajax({
	type: 'POST',
	url: 'http://www.instabid.org:3000/exchanges/', 
	async: false,
	data: dta,
	success: callback,
  	error: function(jqXHR,textStatus,errorThrown) {
		console.log( jqXHR.status );
                console.log( textStatus );
                console.log( errorThrown );
  	}
    }); 
}


function newProduct(exchange, product, callback) {
var dta = {
        exchange: exchange,
        product: product,
        APICall: 'newProduct'
        };

var jqxhr = $.ajax({
        type: 'POST',
        url: '/instabid.php',
        async: false,
        data: dta,
        success: callback,
        error: function(jqXHR,textStatus,errorThrown) {
                console.log( jqXHR.status );
                console.log( textStatus );
                console.log( errorThrown );
        }
    });

}

function newOrder(exchange, product, buy_or_sell, qty, price, user, callback) {
//alert("User " + user + " is going to " + buy_or_sell + " " + qty + " lots of " + product + " on exchange " + exchange + " at a price of " + price);
var dta = {
	exchange: exchange,
	product: product,
	buy_or_sell: buy_or_sell,
	qty: qty,
	price: price,
	user: user,
	APICall: 'newOrder'
	};

var jqxhr = $.ajax({
	type: 'POST',
	url: '/instabid.php', 
	async: false,
	data: dta,
	success: callback,
  	error: function(jqXHR,textStatus,errorThrown) {
		console.log( jqXHR.status );
                console.log( textStatus );
                console.log( errorThrown );
  	}
    }); 

}

function getProduct(exchange, product, callback) {
//alert("first: " + exchange + product + level);
var dta = {
        exchange: exchange,
        product: product,
        APICall: 'getProduct'
        };

var jqxhr = $.ajax({
        type: 'GET',
        url: '/instabid.php',
        async: false,
        data: dta,
        success: callback,
        error: function(jqXHR,textStatus,errorThrown) {
                console.log( jqXHR.status );
                console.log( textStatus );
                console.log( errorThrown );
        }
    });
}


function getProducts(exchange, callback) {
//alert("first: " + exchange + product + level);
var dta = {
        exchange: exchange,
	APICall: 'getProducts'
        };

var jqxhr = $.ajax({
        type: 'GET',
        url: '/instabid.php',
        async: false,
        data: dta,
        success: callback,
        error: function(jqXHR,textStatus,errorThrown) {
                console.log( jqXHR.status );
                console.log( textStatus );
                console.log( errorThrown );
        }
    });
}


function getDepth(exchange, product, level, callback) {
//alert("first: " + exchange + product + level);
var dta = {
        exchange: exchange,
        product: product,
	level: level,
        APICall: 'getDepth'
        };

var jqxhr = $.ajax({
        type: 'GET',
        url: '/instabid.php',
        async: false,
        data: dta,
        success: callback,
        error: function(jqXHR,textStatus,errorThrown) {
                console.log( jqXHR.status );
                console.log( textStatus );
                console.log( errorThrown );
        }
    });

}

function getOrder(exchange, product, level, user, callback) {
//alert("first: " + exchange + product + user);
var dta = {
        exchange: exchange,
        product: product,
        level: level,
	user: user,
        APICall: 'getOrder'
        };

var jqxhr = $.ajax({
        type: 'GET',
        url: '/instabid.php',
        async: false,
        data: dta,
        success: callback,
        error: function(jqXHR,textStatus,errorThrown) {
                console.log( jqXHR.status );
                console.log( textStatus );
                console.log( errorThrown );
        }
    });

}

function getFill(exchange, product, level, user, callback) {
//alert("first: " + exchange + product + user);
var dta = {
        exchange: exchange,
        product: product,
        level: level,
        user: user,
        APICall: 'getFill'
        };

var jqxhr = $.ajax({
        type: 'GET',
        url: '/instabid.php',
        async: false,
        data: dta,
        success: callback,
        error: function(jqXHR,textStatus,errorThrown) {
                console.log( jqXHR.status );
                console.log( textStatus );
                console.log( errorThrown );
        }
    });

}

