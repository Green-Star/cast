var router = require("./router");
var express = require("express");
var http = require("http");
var bodyParser = require("body-parser");
var dataHandler = require("./dataHandler");
var io = require("socket.io");

function start(port) {
    var app = express();
    var server = http.Server(app);
    var connectionHandler = io(server);

    /* Remember every connections */
    connectionHandler.on('connection', (socket) => {});
    
    /* Initialize data and start listening on stdin for incoming data */
    dataHandler.initData(connectionHandler);
    
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
    server.listen(port);
}

exports.start = start;

