var fs = require('fs')
    , http = require('http')
    , socketio = require('socket.io')
    , serialport = require('serialport')
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
	/*

var server = http.createServer(function(req, res) {
    res.writeHead(200, { 'Content-type': 'text/html'});
    res.end(fs.readFileSync(__dirname + '/index.html'));
    req.addListener('end', function() {
    folder.serve(req, res, function(err, result) {
	if (err) {
	    console.error('Error serving %s - %s',
	    req.url, err.message);
	    if (err.status === 404 || err.status === 500) {
		folder.serveFile(util.format('/%d.html', err.status), err.status, {}, req, res);
	    } else {
		res.writeHead(err.status, err.headers);
		res.end();
	    }
	} else {
	    console.log('%s - %s', req.url, res.message); 
	}
});
*/
    /*
    req.addListener('end', function() {
	folder.serve(req,res);
    });
    */
}).listen(8080, function() {
    console.log('Listening at: http://localhost:8080');
});

    /*`
var server = http.createServer(app);
server.listen(8080, function() {
    console.log('Listening at: http://localhost:8080');
});
app.use(express.static(__dirname+"/jQuery"));
app.use(express.static(__dirname+"/bootstrap"));
*/
var SerialPort = serialport.SerialPort;
var serialPort = new SerialPort("/dev/ttyUSB0", {
    baudrate: 57600,
    databits: 8,
    stopbits: 1,
    parity: 'none',
    parser: serialport.parsers.readline("\n")
});
var socket = socketio.listen(server);
var numConnections = 0;
var clients = new Object();

socket.on('connection', function(client){ 
    console.log("Id: " + client.id);
    clients[client.id] = client;
    client.send("hello world");
    client.on('message', function (msg) {
	console.log('Message Received: ', msg);
	client.broadcast.emit('message', msg);
    });
    /*
    client.on('disconnect', function () {
	delete clients[client.id];
    });
    */
});
/*
socketio.listen(server).on('connection', function (socketA) {
    socketA.on('message', function (msg) {
	console.log('Message Received: ', msg);
	socket.broadcast.emit('message', msg);
    });

});
*/
    serialPort.on("open", function() {
	console.log('open serial');
	serialPort.on('data', function(data) {
	    //console.log('data received: ' + data);
	    //console.log('len = ' + data.length);
	    for(var c in clients) {
		if (clients.hasOwnProperty(c)) {
		    for(var cl in clients) {
		        console.log(cl + ' ' + clients[cl]);
		    }
		    console.log('@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@');
		    console.log('length = ' + data.length);
		    console.log('msg = ' + data);
		    var dObj = parseData(data);
		    clients[c].broadcast.emit('message',
			JSON.stringify(dObj));
		    //Only go through once
		    break;
		}
	    }
	});
    });
function parseData(data) {
    var returnObj = {
	subject:'player1',
        object:'player1',
	item: 'GREEN_SHELL',
	action:'hit'
    };
    return returnObj;
}
