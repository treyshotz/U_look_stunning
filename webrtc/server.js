var express = require('express');
var app = express();
var fs = require('fs');

//Https server including ssl certificate
var https = require('https');
var httpsServer = https.createServer({
    key: fs.readFileSync('cert/key.pem'),
    cert: fs.readFileSync('cert/cert.pem')
    },
    app
);

//For signaling
var socketIo = require('socket.io');
var io = socketIo(httpsServer);

//For generating roomIds
var {"v4": uuidv4} = require('uuid');

//Folder for js and css used by view
app.use(express.static('public'));

//If no room is provided it will be redirected to a generated one
app.get('/', (req, res) => {
    res.redirect(`/${uuidv4()}`);
});

//Serve the index.ejs file as content of the / route
app.get('/:room', (req, res) => {
    res.render('index.ejs', {roomId: req.params.room}); //Passes roomid into the ejs file
});

/*
This executes when there is a new connection to the signalling server. If a peer
join or leave the signalling server, it broadcasts this anyone in that room.
*/
io.on('connection', socket => {
    socket.on('join', (roomId, userId, username) => {
        console.log(roomId + " " + userId);
        socket.join(roomId);
        socket.to(roomId).emit('joined', userId, username);

        socket.on('disconnect', () => {
            socket.to(roomId).emit('disconnected', userId);
        });
    });
});

httpsServer.listen(443);

//Http server for redirecting to https
var httpServer = express();
httpServer.get('*', (req, res) => {  
    res.redirect('https://' + req.headers.host + req.url);
})

httpServer.listen(80)