var socket = io('/')
var peer = new Peer(undefined, {
    host: '/',
    port: '3000'
})
var videoContainer = document.getElementById("video-container")
var myVideo = document.createElement('video')
myVideo.muted = true

peer.on('open', (id) => {
    console.log(id)
    if(!(id === null)){
        socket.emit('join', ROOM, id)
    }
    else{
        console.log("Did not get peer id")
    }
})

var constraints = {
    video: 'true',
    audio: 'true'
}

navigator.mediaDevices.getUserMedia(constraints)
.then(stream => {
    addStreamToVideoObject(myVideo, stream)

    peer.on('call', call => {
        call.answer(stream)
    })

    socket.on('joined', (userId) => {
        console.log("Socket joined room. Trying to connect to stream")
        connectToNewStream(userId, stream)
    })
})
.catch(function(err) {
    console.log(err)
});

function connectToNewStream(userId, stream){
    console.log("Calling " + userId)
    const call = peer.call(userId, stream)
    var video = document.createElement('video')
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