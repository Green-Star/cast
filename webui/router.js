var path = require("path");
var dataHandler = require("./dataHandler");

function route(app) {
    app.get("/api/data", function(req, res) {
	var tmp = dataHandler.getAPIData();
	console.log(tmp);
	res.status(200).json(tmp);
    });
    
    app.get("/", function(req, res) {
	console.log("app.get");
	res.sendFile(path.join(__dirname, "index.html"));
    });

    app.get("/audio", function(req, res) {
	console.log("/audio : " + req.query.volume);
	
	process.stdout.write("add_volume "+ req.query.volume + "\n");

	res.redirect("/");
    });

    app.post("/audio", function(req, res) {
	console.log(req.body);

	process.stdout.write("set_volume " + req.body.volume + "\n");
	
	res.redirect("/");	
    });
}

exports.route = route;
