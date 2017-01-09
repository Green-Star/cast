var router = require("./router");
var express = require("express");
var app = express();
var bodyParser = require("body-parser");
var dataHandler = require("./dataHandler");

function start(port) {
    /* Initialize data and start listening on stdin for incoming data */
    dataHandler.initData();

    /* Get all data/stuff of the body (POST) parameters */
    /* Parse application/json */
    app.use(bodyParser.json());
    /* Parse application/vnd.api+json as json */
    app.use(bodyParser.json({ type: 'application/vnd.api+json' })); 
    /* Parse application/x-www-form-urlencoded */
    app.use(bodyParser.urlencoded({ extended: true })); 

    /* Set the static files location /public/index.html will be /index.html for users */
    app.use(express.static(__dirname + '/public')); 
    router.route(app);
    app.listen(port);
    console.log("Server ready on port " + port);
}

exports.start = start;

