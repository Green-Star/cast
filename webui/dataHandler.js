"use strict";

class APIData {
    constructor() {
	this.upload_complete = false;
	this.upload_progress = 0;
	this.time = 0;
	this.length = 0;
	this.volume = 100;
	this.play = false;
	this.video_tracks = [];
	this.audio_tracks = [];
	this.subtitles_tracks = [];
    }
}

var internalData;
var eventHandler;

function initData(handler) {
    internalData = new APIData();
    eventHandler = handler;
    /* Start listening on process.stdin */
    updateData();
}

function getAPIData() {
    internalData.upload_progress += 30;
    if (internalData.upload_progress >= 100) {
	internalData.upload_progress = 100;
	internalData.upload_complete = true;
    }
    return internalData;
}

function updateData() {
    /* Wait for incoming data on process.stdin */
    process.stdin.on("readable", () => {
	var data = JSON.parse(process.stdin.read());
	if (data !== null) {
	    internalData = data;
	    /* Send it to the client */
	    eventHandler.emit('data updated', data);
	}
    });
}

exports.initData = initData;
exports.getAPIData = getAPIData;
