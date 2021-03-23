function enterRoom(){
    var input = document.getElementById('username-input')
    if(input.value != ''){
        document.getElementById('username-container').style.display = 'none'
        document.getElementsByTagName('main')[0].style.display = 'block'
        myUsername = input.value
        start()
    }
}

var videoContainer = document.getElementById("video-container")
var chatBox = document.getElementById('chatbox')
var chatConnections = []
var streamConnections = []
var myStream
var myUsername
var peer

function start(){
    var socket = io('/')
    peer = new Peer(undefined, {
        host: '/',
        port: '9000',
        config: {
            'iceServers': [
                { url: 'stun:127.0.0.1:4040' },
            ]
        }
    })

    var constraints = {
        video: 'true',
        audio: 'true'
    }

    peer.on('open', id => {
        navigator.mediaDevices.getUserMedia(constraints)
        .then(stream => {
            myStream = stream
            var myVideo = document.createElement('video')
            myVideo.muted = true
            addNewStream(myVideo, myStream, id, myUsername)
            socket.emit('join', ROOM, id, myUsername)
        })
        .catch(err => {
            console.log(err)
            socket.emit('join', ROOM, id, myUsername)
            myStream = new MediaStream()
        })
    })
    
    socket.on('joined', (userId, username) => {
        console.log("Socket joined room. Trying to connect to stream")
        connectToNewStream(userId, username)
        connectToNewChatUser(userId, username)
    })
    
    socket.on('disconnected', userId => {
        console.log('Disconnected user with id: ' + userId)
        console.log(chatConnections)
        var video = document.getElementById(userId)
        if(video){
            videoContainer.removeChild(document.getElementById(userId))
        }
    })
    
    peer.on('connection', connection => {
        console.log("New chat connection with user: " + connection.peer)
    
        chatConnections.push(connection)
        connection.on('data', data => {
            addChatEntry(data, connection.options.metadata.username, false)
        })
    
        peer.on('close', () => {
            console.log("Removing chat connection")
            chatConnections.remove(connection)
        })
    })
    
    peer.on('call', call => {
        console.log("Incomming call from user with id:" + call.peer)
        call.answer(myStream)
    
        call.on('stream', incommingStream => {
            console.log(incommingStream)
            if(!streamConnections.includes(call.peer)){
                streamConnections.push(call.peer)
                var newVideo = document.createElement('video')
                addNewStream(newVideo, incommingStream , call.peer, call.options.metadata.username)
            }
        })
        call.on('close', () => {
            streamConnections.remove(call.Peer)
            newVideo.remove()
        })
    })
}

function connectToNewStream(userId, username){
    console.log('Calling user with id: ' + userId)
    var call = peer.call(userId, myStream, {metadata: {
        username: myUsername
    }})
    call.on('stream', incommingStream => {
        console.log(incommingStream)
        if(!streamConnections.includes(userId)){
            streamConnections.push(userId)
            var newVideo = document.createElement('video')
            addNewStream(newVideo, incommingStream , call.peer, username)
        }
    })
    call.on('close', () => {
        streamConnections.remove(userId)
        newVideo.remove()
    })
}

function addChatEntry(message, username, initiator){
    var chatElement = document.createElement('div')
    chatElement.classList.add('chat-entry')
    var label = document.createElement('label')
    label.innerHTML = username + ":"
    var content = document.createElement('p')
    content.classList.add('chat-entry-content')
    content.innerHTML = message

    if(initiator){
        label.classList.add('created')
        content.classList.add('created')
    }
    chatElement.appendChild(label)
    chatElement.appendChild(content)
    chatBox.appendChild(chatElement)
}

function broadcastMessage(message){
    if(chatConnections.length > 0){
        chatConnections.forEach(connection => {
            connection.send(message)
        })
    }
}

function connectToNewChatUser(userId, username){
    var connection = peer.connect(userId, {metadata: {
        username: myUsername
    }})

    connection.on('open', () => {
        console.log("New chat connection with user: " + userId)
        chatConnections.push(connection)
        connection.on('data', data => {
            addChatEntry(data, username, false)
        })

        peer.on('close', () => {
            console.log("Removing chat connection")
            chatConnections.remove(connection)
        })
    })
}

function sendMsg(){
    var message = document.getElementById('chat-input')
    if(message.value){
        broadcastMessage(message.value)
        addChatEntry(message.value, myUsername, true)
    }
    message.value = ""
}

function addNewStream(video, stream, id, username) {
    video.srcObject = stream
    video.addEventListener('loadedmetadata', () => {
        video.play()
    })
    var container = document.createElement('div')
    container.style.position = 'relative'
    var label = document.createElement('label')
    label.classList.add('overlay')
    label.innerHTML = username
    container.setAttribute('id', id);

    container.addEventListener('click', () => {
        if(container.classList.contains('fullscreen')){
            container.classList.remove('fullscreen')
        }
        else{
            container.classList.add('fullscreen')
        }
    })

    container.appendChild(video)
    container.appendChild(label)
    
    videoContainer.appendChild(container)
}
