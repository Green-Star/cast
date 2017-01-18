var APIData;

function updateSelect(select, element) {
    var option = document.createElement("option");
    option.value = element.id;
    option.name = element.name;
    option.text = element.name;
    if (element.language) {
	option.text += " - " + element.language;
    }
    option.selected = element.selected;
    select.add(option);
}

function getFormattedTime(seconds) {
    var hours = seconds / 3600;
    seconds %= 3600;
    var minutes = seconds / 60;
    seconds %= 60;
    
    var string = (hours >= 1) ? Math.trunc(hours) + ":" : "";
    string += (minutes < 10) ? "0" : "";
    string += Math.trunc(minutes) + ":";
    string += (seconds < 10) ? "0" : "";
    string += Math.trunc(seconds);

    return string;
}

function clientUpdateData(data) {
   if (data.upload_complete === true) {
	document.getElementById("upload_bar").className = "display-none";
	document.getElementById("upload_complete").className = "display-inline";
    }
    else {
	document.getElementById("upload_bar").value = data.upload_progress;
	document.getElementById("upload_bar").className = "display-inline";
	document.getElementById("upload_complete").className = "display-none";
    }

    document.getElementById("audio_bar").value = data.volume;

    document.getElementById("time_value").innerHTML = getFormattedTime(data.time);
    document.getElementById("video_progress_bar").value = data.time;
    document.getElementById("video_progress_bar").max = data.length;
    document.getElementById("length_value").innerHTML = getFormattedTime(data.length);
    
    if (data.is_playing === true) {
	document.getElementById("pause_button").className = "display-inline";
	document.getElementById("pause_button").form.className = "display-inline";
	document.getElementById("play_button").className = "display-none";
	document.getElementById("play_button").form.className = "display-none";
    }
    else {
	document.getElementById("pause_button").className = "display-none";
	document.getElementById("pause_button").form.className = "display-none";
	document.getElementById("play_button").className = "display-inline";
	document.getElementById("play_button").form.className = "display-inline";
    }

    var video_tracks = document.getElementById("video_tracks");
    data.video_tracks.forEach((element) => updateSelect(video_tracks, element));

    var audio_tracks = document.getElementById("audio_tracks");
    data.audio_tracks.forEach((element) => updateSelect(audio_tracks, element));

    var subtitles_tracks = document.getElementById("subtitles_tracks");
    data.subtitles_tracks.forEach((element) => updateSelect(subtitles_tracks, element));

    APIData = data;
}
