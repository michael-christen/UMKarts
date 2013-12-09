$(function(){
    //Take care of socket connections
    var iosocket = io.connect();
    iosocket.on('connect', function () {
	iosocket.on('message', function(message) {
	    //$('#incomingChatMessages').append($('<li></li>').text(message));
	    var obj = JSON.parse(message);
	    insertEvent(obj.subject,obj.object,obj.item,obj.action);
	});
    });

    //Creation of div elements and such
    var numPlayers = 0;
    var selectedPlayerId = null;
    var ItemEnum = {
	'GREEN_SHELL': {
	    name:"Green Shell",
	    img:"/static/media/Item_greenshell.png"
	 },
	'RED_SHELL': {
	    name:"Red Shell",
	    img:"/static/media/Item_redshell.png"
	 },
	'BLUE_SHELL': {
	    name:"Blue Shell",
	    img:"/static/media/Item_blueshell.png"
	 },
	'MUSHROOM': {
	    name:"Mushroom",
	    img:"/static/media/Item_mushroom.png"
	 },
	'SUPER_MUSHROOM': {
	    name:"Super Mushroom",
	    img:"/static/media/Item_supermushroom.png"
	 },
	'STAR': {
	    name:"Star",
	    img:"/static/media/Item_star.png"
	 },
	'LIGHTNING': {
	    name:"Lightning",
   	    img:"/static/media/Item_lightning.png"
	},
	'EMPTY': {
	    name: "none",
	    img:"/static/media/Item_empty.png"
	}
    };
    function getItemById(id) {
	return ItemEnum[id];
    }
    var CharacterEnum = {
	'MARIO': {
	    img: "/static/media/Mario_inactive.png",
            name: "Mario"
	},
	'LUIGI': {
	    img: "/static/media/Luigi_inactive.png",
            name: "Luigi"
	},
	'PEACH': {
	    img: "/static/media/Peach_inactive.png",
            name: "Peach"
	},
	'TOAD': {
	    img: "/static/media/Toad_inactive.png",
            name: "Toad"
	},
	'YOSHI': {
	    img: "/static/media/Yoshi_inactive.png",
            name: "Yoshi"
	},
	'DK': {
	    img: "/static/media/DK_inactive.png",
            name: "DK"
	},
	'WARIO': {
	    img: "/static/media/Wario_inactive.png",
            name: "Wario"
	},
	'BOWSER': {
	    img: "/static/media/Bowser_inactive.png",
            name: "Bowser"
	}
    };
    var nameMap = {
	'player0':'MARIO',
	'player1':'LUIGI',
	'player2':'WARIO',
	'player3':'PEACH'
    };
    function getCharById(id) {
	if(nameMap[id]) {
	    id = nameMap[id];
	}
	return CharacterEnum[id];
    }

    function generatePlayerStr(num) {
	var id = num;
	var playerName = "player"+id;
	var pSrc = '/static/media/Person_qmark.png';
	if(!(playerName in nameMap)) {
	    nameMap[playerName] = playerName;
	}
	else {
	    playerName = nameMap[playerName];
	    pSrc=CharacterEnum[playerName]['img'];
	}
	var itemSrc = '/static/media/Item_empty.png';
	var playerStatusStr = " \
	   <div id='player"+id+"'class='playerstatus row-fluid'> \
	   <img class='char span3' src='"+pSrc+"'> \
	   <div class='name span3'>"+playerName+"</div> \
	   <div class='lives span3'>***</div> \
	   <img class='item span3' src='"+itemSrc+"'>\
	   </div>";
	return playerStatusStr;
    }
    function insertPlayer(num) {
	$('#allplayerstatus').append(generatePlayerStr(num));
    }
    var numEvents = 0;

    
    /*
       action -> [picked up, used, hit, joined, died, won, start]
       (last is a game state)
       subject | object -> all players
       item -> seen in ENUM
    */
    function generateEvent(subject, object, item, action) {
	var id = numEvents ++;
	var subjObj = getCharById(subject);
	var objObj = getCharById(object);
	var itemObj = getItemById(item);
	var newsBody = '';
	//Handle display of characters
	if(action == 'picked up') {
	    console.log(itemObj);
	    $('#'+subject).find('.item').attr('src',itemObj.img);
	}
	else if(action == 'used') {
	    $('#'+subject).find('.item').attr('src',ItemEnum['EMPTY'].img);
	}
	else if(action == 'hit') {
	    var origStr = $('#'+object).find('.lives').text();
	    origStr = origStr.substr(0,origStr.length-1);
	    $('#'+object).find('.lives').text(origStr);
	}
	/*
	else if(action =='joined') {
	    insertPlayer();
	}
	*/
	if(action == 'start') {
	    console.log("STARTING");
	    var players = subject.split(' ');
	    console.log(players);
	    //Last is ''
	    for(var i = 0; i < players.length-1; ++i) {
		insertPlayer(players[i]);
	    }
	    newsBody = "\
		<p>The Game has Started with players \
		" + subject + "\
		</p>\
		";
	}
	else {
	    if(subjObj) {
		newsBody = "<img class='subj span2' src='"+subjObj.img+"'>";
	    }
	    //Unnamed
	    else {
		newsBody = "<div class='subj span2'>"+subject+"</div>";
	    }
	    if(itemObj) {
		newsBody += "<div class='action span3 center'>"+action+"</div>";
		if(object) {
		    if(objObj){
			newsBody += "<img class='obj span2' src='"+objObj.img+"'>";
		    }
		    else {
			newsBody += "<div class='obj span2'>"+object+"</div>";
		    }
		    newsBody += "<div class='span1'> with </div>";
		}
		newsBody += "<img class='item span2' src='"+itemObj.img+"'>";
	    }
	}
	var newsStr = "\
	   <div id='news"+id+"' class='newsEvent row-fluid'>\
	   "+newsBody+"\
	   </div>";
	return newsStr;
    }
    function insertEvent(subj,obj,item,act) {
	$('#newsfeed').prepend(generateEvent(subj,obj,item,act));
    }
    /*
    insertPlayer();
    insertEvent("", "", "", "start");
    insertEvent("player0", "", "GREEN_SHELL", "picked up");
    insertEvent("MARIO", "LUIGI", "GREEN_SHELL", "hit");
    */
    $(document).ready(function() {
	$('.playerstatus').on('click', function(event) {
	    selectedPlayerId = $(this).attr('id');
	    $('#myModal').modal();
	});
	$('.pselect').on('click', function(event) {
	    console.log("clicked");
	    if(selectedPlayerId) {
		var charId = $(this).attr('id');
		var charObj = getCharById(charId);
		nameMap[selectedPlayerId] = charObj.name;
		var $player = $('#'+selectedPlayerId);
		$player.find('.char').attr('src',
		    charObj.img);
		$player.find('.name').text(charObj.name);
		selectedPlayerId = null;
		$('#myModal').modal('toggle');
	    }
	});
    });
});

