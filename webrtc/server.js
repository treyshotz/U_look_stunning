var os = require('os')
var express = require('express')
var app = express()
var http = require('http')
var server = http.createServer(app)
server.listen(3001)

//For signaling
var socketIo = require('socket.io')
var io = socketIo(server)

//For generating roomIds
var {"v4": uuidv4} = require('uuid')

//Folder for js and css
app.use(express.static('public'))

//If user doesn't enter any room id a random one is created
//Redirects client to the created roomid
app.get('/', (req, res) => {
    res.redirect(`/${uuidv4()}`)
})

app.get('/:room', (req, res) => {
    res.render('index.ejs', { roomId: req.params.room})
})

io.on('connection', (client) => {
    client.on('join', (roomId, userId) => {
        console.log(roomId + " " + userId)
        client.join(roomId)
        client.to(roomId).emit('joined', userId)
    })
})