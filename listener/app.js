var fs = require('fs')
    , http = require('http')
    , socketio = require('socket.io')
    , zmq = require('zmq')
    , port = 'tcp://67.194.114.219:6666'
    //, port = 'tcp://67.194.42.30:6666'
    , static = require('node-static')
    , util   = require('util')
    , path   = require('path');

//var folder = new(static.Server)(__dirname + '/static');
var server = http.createServer(function(request, response) {
    console.log('request starting...');
    var filePath = '.' + request.url;
    if (filePath == './') {
	filePath = './index.html';
    }
		
    var extname = path.extname(filePath);
    var contentType = 'text/html';
    //load static files
    switch (extname) {
	case '.js':
		contentType = 'text/javascript';
		break;
	case '.css':
		contentType = 'text/css';
		break;
    }
    path.exists(filePath, function(exists) {
	if (exists) {
	    fs.readFile(filePath, function(error, content) {
		if (error) {
		    response.writeHead(500);
		    response.end();
		} else {
		    response.writeHead(200, { 'Content-Type': contentType });
		    response.end(content, 'utf-8');
		}
	    });
	} else {
	    response.writeHead(404);
	    response.end();
	}
    });
}).listen(8080, function() {
    console.log('Listening at: http://localhost:8080');
});

var socket = socketio.listen(server);
var numConnections = 0;
var clients = new Object();

socket.on('connection', function(client){ 
    console.log("Id: " + client.id);
    clients[client.id] = client;
});

var actionArr = [
    'picked up',
    'used',
    'hit',
    'joined',
    'died',
    'won',
    'start'
];

var itemArr = [
    'GREEN_SHELL',
    'MUSHROOM',
    'LIGHTNING',
    'STAR',
    'RED_SHELL',
    'BLUE_SHELL',
    'SUPER_MUSHROOM',
    'EMPTY'
];

function parseData(data) {
    var returnObj = null;
    if(data.length <= 0) {
	return returnObj;
    };
    switch(data[0]) {
	case 0:
	   var sender = data[3];
	   var msg = data.slice(4);
	   console.log("printf: sender = " 
		   + sender+ " msg = " +msg);
	   break;
	case 1:
	   console.log("Printf_extended"); 
	   break;
	case 2:
	   console.log("driver " + data[3] + " is hosting a game"); 
	   break;
	case 3:
	   console.log("driver " + data[3] + "joined game"); 
	   break;
	case 4:
	   var pStr = '';
	   var numPlayers = data[4];
	   for(var i = 0; i < numPlayers; ++i) {
	       pStr += data[5+i] + ' ';
	   }
	   console.log("game starting with players: " + pStr);
	   console.log("game is being hosted by player " + data[3]);
	   returnObj = {
		subject: pStr,
		object:null,
		action:'start',
		item:null
	   };
	   break;
	case 5:
	   console.log("game over declared by player " + data[3]); 
	   break;
	case 6:
	   console.log("game event");
	   var subj = data[4];
	   var obj = data[5];
	   var act = data[6];
	   var item = data[7];
	   //Lose life
	   if(act == 3) {
	       break;
	   }
	   returnObj = {
		subject:'player'+subj,
		object:'player'+obj,
		item:itemArr[item],
		action:actionArr[act]
	   };
	   //is actually hit by
	   if(returnObj['action'] == 'hit') {
	       var tmp = returnObj['object'];
	       returnObj['object'] = returnObj['subject'];
	       returnObj['subject']= tmp;
	   }
	   if(returnObj['object'] == 'player255') {
	       returnObj['object'] = null;
	   }
	   console.log(returnObj);
	   return returnObj;
	   break;
        case 7:
	   console.log("xb_msg_player_left");
	   console.log("Player " + data[3] + " noticed player "
		   + data[4] + " left");
	   break;
	case 8:
	   console.log("ack");
	   break;
    }
    return returnObj;
}

var tcpSocket = zmq.socket('sub');
tcpSocket.identity = 'subscriber' + process.pid;
tcpSocket.connect(port);
tcpSocket.subscribe('');
tcpSocket.on('message', function(data) {
    for(var c in clients) {
	if (clients.hasOwnProperty(c)) {
	    for(var cl in clients) {
		console.log(cl + ' ' + clients[cl]);
	    }
	    /*
	    console.log('length = ' + data.length);
	    console.log('msg = ' + data);
	    for(var i=0; i < data.length; ++i) {
	        console.log(data[i]);
	    }
	    */
	    var dObj = parseData(data);
	    if(dObj) {
		clients[c].broadcast.emit('message',
		    JSON.stringify(dObj));
	    }
	    //Only go through once
	    break;
	}
    }
});
