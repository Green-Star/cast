"use strict";

class APIData {
    constructor() {
	this.upload_progress = 0;
	this.upload_complete = false;
	this.is_playing = false;
	this.time = 0;
	this.length = 0;
	this.volume = 0;
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
