var http = require('http');
var fs = require('fs');
var sleep = require('system-sleep');


const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline');
const port = new SerialPort('COM13', { baudRate: 2000000 });
const parser = port.pipe(new Readline({ delimiter: '\n' }));



var zmienna = 0;
var zmienna2 = 0;
// Loading the file index.html displayed to the client
var server = http.createServer(function(req, res) {
    fs.readFile('./index.html', 'utf-8', function(error, content) {
        res.writeHead(200, {"Content-Type": "text/html"});
        
        res.end(content);
    });
});

// Loading socket.io
var io = require('socket.io').listen(server);

io.sockets.on('connection', function (socket, username) {
  // NEW -----------------------------------------------------------------------------------------
  
  // Read the port data
port.on("open", () => {
    console.log('serial port open');
  });
  
  parser.on('data', data =>{
      zmienna = data; 
    console.log('new:', data);
    socket.emit('message', '' + zmienna);
    
  });
  
    
  // NEW -----------------------------------------------------------------------------------------
        
    
    // The other clients are told that someone new has arrived
    socket.broadcast.emit('message', 'Another client has just connected!');

    // As soon as the username is received, it's stored as a session variable
    socket.on('little_newbie', function(username) {
        socket.username = username;
    });

    // When a "message" is received (click on the button), it's logged in the console
    socket.on('message', function (message) {
        // The username of the person who clicked is retrieved from the session variables
        console.log(socket.username + ' is speaking to me! They\'re saying: ' + message);
    }); 
});


server.listen(8080);