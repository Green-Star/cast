var path = require("path");
var dataHandler = require("./dataHandler");

function route(app) {
    app.get("/api/data", function(req, res) {
	res.status(200).json(dataHandler.getAPIData());
    });
    
    app.get("/", function(req, res) {
	res.sendFile(path.join(__dirname, "index.html"));
    });

    app.get("/audio", function(req, res) {	
	process.stdout.write("add_volume "+ req.query.volume + "\n");

	res.redirect("/");
    });

    app.post("/audio", function(req, res) {
	process.stdout.write("set_volume " + req.body.volume + "\n");
	
	res.redirect("/");	
    });

    app.post("/video_tracks", function(req, res) {	
	process.stdout.write("set_video_track " + req.body.video_tracks + "\n");
	
	res.redirect("/");
    });

    app.post("/audio_tracks", function(req, res) {
	process.stdout.write("set_audio_track " + req.body.audio_tracks + "\n");

	res.redirect("/");
    });

    app.post("/subtitles_tracks", function(req, res) {
	process.stdout.write("set_subtitles_track " + req.body.subtitles_tracks + "\n");

	res.redirect("/");
    });

    app.post("/rewind", function(req, res) {
	process.stdout.write("set_time 0\n");

	res.redirect("/");
    });

    app.post("/pause", function(req, res) {
	process.stdout.write("pause\n");

	res.redirect("/");
    });

    app.get("/pause", function(req, res) {
	process.stdout.write("play\n");

	res.redirect("/");
    });

    app.post("/stop", function(req, res) {
	process.stdout.write("stop\n");

	res.redirect("/");
    });

    app.get("/video", function(req, res) {
	process.stdout.write("add_time " + req.query.time + "\n");

	res.redirect("/");
    });
}

exports.route = route;
