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

io.sockets.on('connection', (client) => {
    console.log('Client connected: ' + client.id)

    client.on('offer', (details) => {
        client.broadcast('offer', details)
        console.log('Client: ' + client.id + ' offers ' + JSON.stringify(details))
    })
    
    client.on('answer', (details) => {
        client.broadcast('answer', details)
        console.log('Client: ' + client.id + ' responds with ' + JSON.stringify(details))
    })

    client.on('candidate', (details) => {
        client.broadcast('candidate', details)
        console.log('Candidate: ' + JSON.stringify(details))
    })

    client.broadcast.emit('createoffer', {})
})