var fs = require('fs')
    , http = require('http')
    , socketio = require('socket.io')
    , serialport = require('serialport')
    , zmq = require('zmq')
    , port = 'tcp://67.194.60.45:6666'
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
function parseData(data) {
    if(data.length <= 0) {
	return '';
    };
    switch(data[0]) {
	case 0:
	   var msg = data.slice(3);
	   console.log("printf msg = " +msg);
	   break;
	case 1:
	   console.log("Printf_extended"); 
	   break;
	case 2:
	   console.log("game host, driver is " + data[1]); 
	   break;
	case 3:
	   console.log("game join"); 
	   console.log("driver joined " + data);
	   console.log(data);
	   break;
	case 4:
	   console.log("game start"); 
	   console.log("here are players: " + data);
	   break;
	case 5:
	   console.log("game over"); 
	   break;
	case 6:
	   console.log("game event");
	   var subj = data[1];
	   var obj = data[2];
	   var act = data[3];
	   var item = data[3];
	   var returnObj = {
		subject:subj,
		object:obj,
		item:item,
		action:act
	   };
	   console.log(data);
	   console.log(returnObj);
	   return returnObj;
	   break;
        case 7:
	   console.log("xb_msg_player_left");
	   break;
	case 8:
	   console.log("ack");
	   break;
    }
    var returnObj = {
	subject:'player1',
        object:'player2',
	item: 'GREEN_SHELL',
	action:'hit'
    };
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
	    clients[c].broadcast.emit('message',
	    JSON.stringify(dObj));
	    //Only go through once
	    break;
	}
    }
});
