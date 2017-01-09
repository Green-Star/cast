var APIData;

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

function clientUpdateData(data) {
   if (data.upload_complete === true) {
	document.getElementById("upload_bar").style.display = "none";
	document.getElementById("upload_complete").style.display = "inline";
    }
    else {
	document.getElementById("upload_bar").value = data.upload_progress;
	document.getElementById("upload_bar").style.display = "inline";
	document.getElementById("upload_complete").style.display = "none";
    }

    APIData = data;
}

(function clientRenderer() {
    getAPIData(clientUpdateData, error);
})();
