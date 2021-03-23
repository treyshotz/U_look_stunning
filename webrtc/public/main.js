var socket = io('/');
var videoContainer = document.getElementById("video-container");
var chatInput = document.getElementById('chat-input')
var chatBox = document.getElementById('chatbox');
var usernameInput = document.getElementById('username-input');
var chatConnections = [];
var streamConnections = [];

var myStream;
var myUsername;
var myPeer;

/**
 * This function runs when the 'enter room' button is clicked
 * If there was any input, the username is stored and the main
 * content of the page is shown. It also starts the main part
 * of the script.
 */
function enterRoom(){
    if(usernameInput.value != ''){
        document.getElementById('username-container').style.display = 'none';
        document.getElementsByTagName('main')[0].style.display = 'block';
        myUsername = usernameInput.value;
        start();
    }
}

/**
 * Wrapper for all main functionallity
 */
function start(){

    /*
    Makes it possible to press enter when in the inputfield
    */
    chatInput.addEventListener('keypress', event => {
        if (event.key === 'Enter') {
          sendMsg();
        }
    });

    /*
    Makes it possible to press enter when in the inputfield
    */
    usernameInput.addEventListener('keypress', event => {
        if (event.key === 'Enter') {
          enterRoom();
        }
    });

    myPeer = new Peer(undefined, {
        host: '/',
        port: '9000',
        config: {
            'iceServers': [
                { url: 'stun:stun1.l.google.com:19302' },
            ]
        }
    });

    var constraints = {
        video: 'true',
        audio: 'true'
    };

    /*
    When peer object is created we ask for video and audio from the user.
    we 
    */
    myPeer.on('open', id => {
        navigator.mediaDevices.getUserMedia(constraints)
        .then(stream => {
            myStream = stream;
            var myVideo = document.createElement('video')
            myVideo.muted = true;
            addNewStream(myVideo, myStream, id, myUsername)
            socket.emit('join', ROOM, id, myUsername)
        })
        .catch(err => {
            console.log(err)
            socket.emit('join', ROOM, id, myUsername)
            myStream = new MediaStream()
        })
    });
    
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
    
    myPeer.on('connection', connection => {
        console.log("New chat connection with user: " + connection.peer)
    
        chatConnections.push(connection)
        connection.on('data', data => {
            addChatEntry(data, connection.options.metadata.username, false)
        })
    
        myPeer.on('close', () => {
            console.log("Removing chat connection")
            chatConnections.remove(connection)
        })
    })
    
    myPeer.on('call', call => {
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
    var call = myPeer.call(userId, myStream, {metadata: {
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
    var connection = myPeer.connect(userId, {metadata: {
        username: myUsername
    }})

    connection.on('open', () => {
        console.log("New chat connection with user: " + userId)
        chatConnections.push(connection)
        connection.on('data', data => {
            addChatEntry(data, username, false)
        })

        myPeer.on('close', () => {
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
            document.getElementsByTagName('body')[0].style.overflow = 'auto';
        }
        else{
            container.classList.add('fullscreen')
            document.getElementsByTagName('body')[0].style.overflow = 'hidden';
        }
    })

    container.appendChild(video)
    container.appendChild(label)
    
    videoContainer.appendChild(container)
}