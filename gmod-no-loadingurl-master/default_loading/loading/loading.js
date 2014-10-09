var g_TotalFiles = 0;
var g_NeededFiles = 0;

var icons = {
	"workshop": "box", // Because dumb

	"bsp": "world",

	"vmt": "palette",
	"vtf": "palette",
	"png": "palette",

	"mdl": "brick",
	"vtx": "brick",
	"vvd": "brick",
	"phy": "brick",

	"mp3": "sound",
	"wav": "sound",

	"ttf": "font",

	"dat": "page",
	"txt": "page",

	"default": "bomb"
}

var message_buffer = []

function AddMessageToBuffer(text) {
	var message = document.createElement("div");
	message.className = "message";
	message.textContent = text;

	var messages = document.getElementById("messages");

	messages.insertBefore(message, messages.firstChild);

	message_buffer.unshift(message);

	if (message_buffer.length >= 12) {
		var oldest_message = messages.lastChild;
		message_buffer.pop();

		messages.removeChild(oldest_message);
	}

	for (var i = 0; i < message_buffer.length; i++) {
		var message = message_buffer[i];

		message.style.opacity = 1 - (1/12) * i;
	}
}

function AddIcon(type) {
	var icon = document.createElement("div");
	icon.className = "icon icon-" + type;

	var icon_container = document.getElementById("icon_container");
	icon_container.appendChild(icon);

	icon.style.top = (5 + Math.random() * 45).toString() + "%";
	icon.style.left = (10 + Math.random() * 80).toString() + "%";
}

function GameDetails(servername, serverurl, mapname, maxplayers, steamid, gamemode) {

}

function DownloadingFile(fileName) {
	if (fileName.slice(-1) == "'") {
		return;
	}

	AddMessageToBuffer("Downloading " + fileName);

	var name_lower = fileName.toLowerCase();

	var id = name_lower.match(/.*[\s-.](\S+)/)[1];

	if (id) {
		var icon_type = icons[id];

		if (!icon_type) {
			icon_type = icons['default'];
		}

		AddIcon(icon_type);
	}

	g_NeededFiles = Math.max(0, g_NeededFiles - 1);

	document.getElementById("needed-files").textContent = g_NeededFiles;	
}

function SetStatusChanged(status) {
	if (status.indexOf("Extracting 'Keypad") != -1) {
		AddMessageToBuffer("Running the greatest addon ever");
	}

	if (status == "Sending client info...") {
		AddMessageToBuffer("Brace yourself!");
	}
}

function SetFilesTotal(total) {
	g_TotalFiles = total;
}

function SetFilesNeeded(needed) {
	g_NeededFiles = needed

	if (document.getElementById("headline")) {
		var headline = document.getElementById("headline");

		if (needed <= 0) {
			headline.style.visibility = 'hidden';
		} else {
			headline.style.visibility = 'visible';
		}
	}

	if (document.getElementById("needed-files")) {
		document.getElementById("needed-files").textContent = g_NeededFiles;
	}
}

document.addEventListener('DOMContentLoaded', function() {
	AddMessageToBuffer("Initialising connection");

	document.getElementById("needed-files").textContent = g_NeededFiles;

	var headline = document.getElementById("headline");

	if (g_NeededFiles <= 0) {
		headline.style.visibility = 'hidden';
	} else {
		headline.style.visibility = 'visible';
	}
	
}, false);