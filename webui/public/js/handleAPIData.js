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

    var video_tracks = document.getElementById("video_tracks");
    data.video_tracks.forEach((element) => updateSelect(video_tracks, element));

    var audio_tracks = document.getElementById("audio_tracks");
    data.audio_tracks.forEach((element) => updateSelect(audio_tracks, element));

    var subtitles_tracks = document.getElementById("subtitles_tracks");
    data.subtitles_tracks.forEach((element) => updateSelect(subtitles_tracks, element));

    APIData = data;
}
