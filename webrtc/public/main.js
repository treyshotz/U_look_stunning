var socket = io('/')
var peer = new Peer(undefined, {
    host: '/',
    port: '443',
    config: {
        'iceServers': [
            { url: 'stun:stun1.l.google.com:19302' },
        ]
    }

})

var videoContainer = document.getElementById("video-container")
var chatBox = document.getElementById('chatbox')
var chatConnections = []
var streamConnections = []
var myStream
var myUsername

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
        addNewStream(myVideo, myStream, id)
        socket.emit('join', ROOM, id)
    })
})

function enterRoom(){
    var input = document.getElementById('username-input')
    if(input.value != ''){
        document.getElementById('login-wrapper').style.display = 'none'
        document.getElementById('video-container').style.display = 'grid'
        myUsername = input.value
    }
}

socket.on('joined', userId => {
    console.log("Socket joined room. Trying to connect to stream")
    connectToNewStream(userId)
    connectToNewChatUser(userId)
})

socket.on('disconnected', userId => {
    console.log('Disconnected user with id: ' + userId)
    console.log(chatConnections)
    videoContainer.removeChild(document.getElementById(userId))
})

function sendMsg(){
    var message = document.getElementById('chat-input')
    if(message.value){
        broadcastMessage(message.value)
        addChatEntry(message.value, myUsername, true)
    }
    message.value = ""
}

function addChatEntry(message, username, initiator){
    var chatElement = document.createElement('div')
    chatElement.classList.add('chat-item')
    var label = document.createElement('label')
    label.classList.add('chat-item-label')
    label.innerHTML = username + ":"
    var content = document.createElement('p')
    content.classList.add('chat-item-content')
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
            console.log(myUsername)
            connection.send({message: message, username: myUsername})
        })
    }
}

function connectToNewChatUser(userId){
    var connection = peer.connect(userId)

    connection.on('open', () => {
        console.log("New chat connection with user: " + userId)
        chatConnections.push(connection)
        connection.on('data', data => {
            addChatEntry(data.message, data.username, false)
        })

        peer.on('close', () => {
            console.log("Removing chat connection")
            chatConnections.remove(connection)
        })
    })
}

peer.on('connection', connection => {
    console.log("New chat connection with user: " + connection.peer)

    chatConnections.push(connection)
    connection.on('data', data => {
        addChatEntry(data.message, data.username, false)
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
        if(!streamConnections.includes(call.peer)){
            streamConnections.push(call.peer)
            var newVideo = document.createElement('video')
            addNewStream(newVideo, incommingStream , call.peer)
        }
    })
    call.on('close', () => {
        streamConnections.remove(call.Peer)
        newVideo.remove()
    })
})

function connectToNewStream(userId){
    console.log('Calling user with id: ' + userId)
    var call = peer.call(userId, myStream)

    call.on('stream', incommingStream => {
        if(!streamConnections.includes(userId)){
            streamConnections.push(userId)
            var newVideo = document.createElement('video')
            addNewStream(newVideo, incommingStream , call.peer)
        }
    })
    call.on('close', () => {
        streamConnections.remove(userId)
        newVideo.remove()
    })
}

function addNewStream(video, stream, id) {
    video.setAttribute('id', id);
    video.srcObject = stream
    video.addEventListener('loadedmetadata', (event) => {
        video.play()
    })

    video.addEventListener('click', (event) => {
        if(video.classList.contains('in-focus')){
            video.classList.remove('in-focus')
        }
        else{
            var infocus = document.getElementsByClassName('in-focus')
            if(infocus.length !== 0){
                infocus[0].classList.remove('in-focus')
            }
            video.classList.add('in-focus')
        }
    })
    
    videoContainer.appendChild(video)
}