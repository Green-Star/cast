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

function initData() {
    internalData = new APIData();
}

function getAPIData() {
    internalData.upload_progress += 30;
    if (internalData.upload_progress >= 100) {
	internalData.upload_progress = 100;
	internalData.upload_complete = true;
    }
    return internalData;
}

function updateData(data) {
    /* Do stuff on process.stdin */
}

exports.initData = initData;
exports.getAPIData = getAPIData;
