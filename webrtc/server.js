var express = require('express')
var app = express()
var fs = require('fs')

// Set up a route to redirect http to https
var http = express();
http.get('*', (req, res) => {  
    res.redirect('https://' + req.headers.host + req.url);
})

var https = require('https')
var httpsServer = https.createServer({
    key: fs.readFileSync('cert/key.pem'),
    cert: fs.readFileSync('cert/cert.pem')
    },
    app
)

//For signaling
var socketIo = require('socket.io')
var io = socketIo(httpsServer)

//For generating roomIds
var {"v4": uuidv4} = require('uuid')

//Folder for js and css
app.use(express.static('public'))

app.get('/', (req, res) => {
    res.redirect(`/${uuidv4()}`)
})

// Serve the index.html file as content of the / route
app.get('/:room', (req, res) => {
    res.render('index.ejs', {roomId: req.params.room})
})

io.on('connection', socket => {
    socket.on('join', (roomId, userId, username) => {
        console.log(roomId + " " + userId)
        socket.join(roomId)
        socket.to(roomId).emit('joined', userId, username)

        socket.on('disconnect', () => {
            socket.to(roomId).emit('disconnected', userId)
        })
    })
})

http.listen(80);
httpsServer.listen(443)