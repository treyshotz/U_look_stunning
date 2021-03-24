var socket = io('/'); //For communications with the server

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
    Creates a new peer with a specified stunserver. By leaving id to
    undefined it will generate one for us
    */
    myPeer = new Peer(undefined, {
        host: '/',
        port: '9000',
        config: {
            'iceServers': [
                { url: 'stun:158.38.166.242:4040' },
            ]
        }
    });

    var constraints = {
        video: 'true',
        audio: 'true'
    };

    /*
    When peer object is created we ask for video and audio from the user.
    If a stream is provided we add it the the videocontainer. We are telling
    the signalling server that we are joining a specific room.
    */
    myPeer.on('open', id => {
        navigator.mediaDevices.getUserMedia(constraints)
        .then(stream => {
            myStream = stream;
            var myVideo = document.createElement('video');
            myVideo.muted = true;
            addNewStream(myVideo, myStream, id, myUsername);
            socket.emit('join', ROOM, id, myUsername);
        })
        .catch(err => {
            console.log(err);
            myStream = new MediaStream();
            socket.emit('join', ROOM, id, myUsername);
        });
    });
    
    /*
    The signalling server tells us that a new peer har joined.
    We attempt to connet to it by stream and chat.
    */
    socket.on('joined', (userId, username) => {
        console.log("Socket joined room. Trying to connect to stream");
        connectToNewStream(userId, username);
        connectToNewChatUser(userId, username);
    });
    
    /*
    The signalling server tells us that a peer has left the domain.
    If we had a connection to it by chat we remove its container.
    */
    socket.on('disconnected', userId => {
        console.log('Disconnected user with id: ' + userId);
        var video = document.getElementById(userId);
        if(video){
            videoContainer.removeChild(document.getElementById(userId));
        }
    });
    
    /*
    When someone creates a chat connection to us
    */
    myPeer.on('connection', connection => {
        console.log("New chat connection with user: " + connection.peer);

        chatConnections.push(connection);

        connection.on('data', data => {
            addChatEntry(data, connection.options.metadata.username, false);
        });
    
        myPeer.on('close', () => {
            console.log("Removing chat connection");
            chatConnections.remove(connection);
        });
    });

    /*
    When recieving a call request (video stream)
    */
    myPeer.on('call', call => {
        console.log("Incomming call from user with id:" + call.peer);
        call.answer(myStream);
    
        /*
        If a stream with non-empty video and autio tracks is provided
        we add a it to our videocontainer.
        */
        call.on('stream', incommingStream => {//This event is called for each media channel.
            if(!streamConnections.includes(call.peer)){//Therefore we check if a connection already exists
                streamConnections.push(call.peer);
                var newVideo = document.createElement('video');
                addNewStream(newVideo, incommingStream , call.peer, call.options.metadata.username);
            }
        });

        call.on('close', () => {
            streamConnections.remove(call.Peer);
            newVideo.remove();
        });
    })
}

/**
 * Function for atablishing a peer to peer 
 * chat connection to a newly joined peer.
 * @param {*} userId peer id of the joined peer
 * @param {*} username username of the joined peer
 */
function connectToNewStream(userId, username){
    console.log('Calling user with id: ' + userId);

    var call = myPeer.call(userId, myStream, {metadata: {
        username: myUsername
    }});

    /*
    This is called the called peer responds with answer() and
    the stream has non-empty media tracks
    */
    call.on('stream', incommingStream => {
        if(!streamConnections.includes(userId)){
            streamConnections.push(userId);
            var newVideo = document.createElement('video');
            addNewStream(newVideo, incommingStream , call.peer, username);
        }
    });

    call.on('close', () => {
        streamConnections.remove(userId);
        newVideo.remove();
    });
}

/**
 * A method for adding a new chat enty into the 
 * chat box.
 * @param {*} message the content of the message
 * @param {*} username the name of the one who sent it
 * @param {*} initiator if it was sendt by you
 */
function addChatEntry(message, username, initiator){
    var chatElement = document.createElement('div');
    chatElement.classList.add('chat-entry');

    var label = document.createElement('label');
    label.innerHTML = username + ":";

    var content = document.createElement('p');
    content.classList.add('chat-entry-content');
    content.innerHTML = message;

    if(initiator){
        label.classList.add('created');
        content.classList.add('created');
    }

    chatElement.appendChild(label);
    chatElement.appendChild(content);

    chatBox.appendChild(chatElement);
}

/**
 * Method for broadcasting a message to all chat-
 * connections.
 * @param {*} message input content
 */
function broadcastMessage(message){
    if(chatConnections.length > 0){
        chatConnections.forEach(connection => {
            connection.send(message);
        });
    }
}

/**
 * Function for atablishing a peer to peer 
 * chat connection to a newly joined peer.
 * @param {*} userId peer id of the joined peer
 * @param {*} username username of the joined peer
 */
function connectToNewChatUser(userId, username){
    var connection = myPeer.connect(userId, {metadata: {
        username: myUsername
    }});

    connection.on('open', () => {
        console.log("New chat connection with user: " + userId);
        chatConnections.push(connection);
        connection.on('data', data => {
            addChatEntry(data, username, false);
        });

        myPeer.on('close', () => {
            console.log("Removing chat connection");
            chatConnections.remove(connection);
        });
    });
}

/**
 * Function for sending what is in the inputfield of the chat
 */
function sendMsg(){
    var message = document.getElementById('chat-input');
    if(message.value){
        broadcastMessage(message.value);
        addChatEntry(message.value, myUsername, true);
    }
    message.value = "";
}

/**
 * Function for adding a video stream to the video container
 * @param {*} video the video element
 * @param {*} stream the source of the video
 * @param {*} id peer id of the streams author
 * @param {*} username username of the streams author
 */
function addNewStream(video, stream, id, username) {
    video.srcObject = stream;
    video.addEventListener('loadedmetadata', () => {
        video.play();
    });

    /*
    Wrapper for video and video-label
    */
    var container = document.createElement('div');
    container.style.position = 'relative';
    container.setAttribute('id', id);

    /*
    Creating a label with username on top of video
    */
    var label = document.createElement('label');
    label.classList.add('overlay');
    label.innerHTML = username;

    /*
    Making video fullscreen when clicked
    */
    container.addEventListener('click', () => {
        if(container.classList.contains('fullscreen')){
            container.classList.remove('fullscreen');
            document.getElementsByTagName('body')[0].style.overflow = 'auto';
        }
        else{
            container.classList.add('fullscreen')
            document.getElementsByTagName('body')[0].style.overflow = 'hidden';
        }
    });

    container.appendChild(video);
    container.appendChild(label);
    
    videoContainer.appendChild(container);
}


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
=======
    videoContainer.appendChild(container)
}
