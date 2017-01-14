function getAPIData(callback, error) {
    var URL = "/api/data";
    var req = new XMLHttpRequest();
    req.open("GET", URL, true);
    req.onreadystatechange = function(event) {
	if (req.readyState == 4) {
	    if (req.status == 200) {
		var data = JSON.parse(req.responseText);
		callback(data);
	    }

	    if (req.status >= 400 && typeof error !== "undefined") {
		error(req);
	    }
	}
    };
    req.send(null);
}

function error(req) {
    console.log("Server error : " + req.responseText);
}

(function clientRenderer() {
    getAPIData(clientUpdateData, error);
})();
