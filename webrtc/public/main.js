var client = io('/')
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
var myVideo = document.createElement('video')
myVideo.muted = true


var constraints = {
    video: 'true',
    audio: 'true'
}

peer.on('open', id => {
    console.log('Peer: ' + id)
    if(!(id === null)){
        client.emit('join', ROOM, id)
    }
    else{
        console.log("Did not get peer id")
    }


    navigator.mediaDevices.getUserMedia(constraints)
    .then(stream => {
        addStreamToVideoObject(myVideo, stream)

        client.on('joined', userId => {
            console.log("Socket joined room. Trying to connect to stream")
            connectToNewStream(userId, stream)
        })
    })
    .catch(function(err) {
        console.log(err)
    });
})

client.on('disconnected', userId => {
    console.log('Disconnected user with id: ' + userId)
    videoContainer.removeChild(document.getElementById(userId))
})

peer.on('call', call => {
    navigator.mediaDevices.getUserMedia(constraints)
    .then(stream => {
        call.answer(stream)
        var video = document.createElement('video')
        video.setAttribute('id', call.peer)
        console.log("Answered user with id: " + call.peer)
        call.on('stream', stream => {
            addStreamToVideoObject(video, stream)
        })
        call.on('close', () => {
            video.remove()
        })
    })
})

function connectToNewStream(userId, stream){
    console.log("Calling " + userId)
    const call = peer.call(userId, stream)
    var video = document.createElement('video')
    video.setAttribute('id', userId);
    console.log('Calling user with id: ' + userId)
    call.on('stream', userVideoStream => {
        addStreamToVideoObject(video, userVideoStream)
    })
    call.on('close', () => {
        video.remove()
    })
}

function addStreamToVideoObject(video, stream) {
    video.srcObject = stream
    video.addEventListener('loadedmetadata', (event) => {
        video.play()
    });
    videoContainer.appendChild(video)
}