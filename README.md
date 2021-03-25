# U_look_stunning
The stun server is available through a public ip address. The address is https://madslun.nerdvana.tihlde.org, and the port is 4040. If accessing this website through a browser you will be met with the signaling server, making it possible to video chat with other people. Our continuous deployment is unfortunately private, so due to security issues we will not provide credentials for accessing the CD.

To run this project run `docker-compose up`

### Introduction:
U_look_stunning is a peer to peer video conferencing service where you can video and text chat with other users, using a self-implemented STUN server and signaling server. The stun server was written in C++ and the signaling server was written in Javascript. 


### Implemented functionallity:
The current version allows multiple users to join the same chat room to chat through video and audio, as well as text. Before a user enters the chat room they can set their display name for the session. Users are able to enlarge their own or another user's video by clicking on the video screen they wish to enlarge.

Behind the scenes the user uses the implemented STUN-server to get their public IP address in the peer to peer connection. 

The project uses Jenkins, webhook and docker-compose to perform continuous deployment. Each time a branch is merged with main Jenkins webhooks main and run the shell command to replace the old images, and to run the new containers.    


### Future implementation:
* Allow users to send files to each other
* Allow users to share their screen with each other
* Allow users without camera to stream audio
* Allow users to select camera and audio options
* Add attributes to the STUN-message
* Improve thread safety while multithreading on stun server, using worker threads and event loop
* Improve error handling on stun server
* Implement IPv6 handling in stun server
* Obtain a ssl certificate from a certificate authority
* Improve video fit area
* Implement login system
* Implement share link button
* Implement options for creating a new room (Max connections, room admin ect.)
* Write tests for the different parts of the project


### Built with:

##### Express
A framework built on Node used to build web-applications. This was used to ease the process of responding to domain get requests.

##### Nodemon
A framework that helps developing node.js based applications by automatically restarting the application when a change is saved. This was used for restarting the signalling server on change.

##### Socket io
A library for web applications that enables real time communication between clients and servers. This was used for signalling.

##### Ejs
A tempating language that lets you generate html markup in javascript. We used this since it made it easy to send room id when rendering the view.


##### Peerjs
An easy to use WebRTC peer-to-peer connection api. This was used for starting a peer server and peer connections, both video and chat, between users.

##### Bootstrap
A tool for making the process of styling and scale the website easier. In our project this was only used for dividing the space between the video area and the chat area, as well as making it wrap if the screen size became smaller.

##### Uuid
An api that generates random IDs. In our application this was used to generate room id when none was provided in the url.

##### Npm
Package manager for the node.js platform. This was used for installing the dependencies in the webrtc part of the project.

##### Cmake
Popular project handler for c/c++ based programs. This was used for developing the stun server.

##### Docker
Docker is a platform used for developing, shipping and running applications. It uses OS-level virtualization to run isolated software in packages called containers that can be run regardless of the users OS.  

##### Tihlde server
Through a friend of Mads, Torbjør Øverås, we got hands on a Tihlde server that we could use for developing this project. He used his spare time setting up the server. Special thanks to him.


### Installation:

There is no installation needed as U_look_stunning utilizes docker to create images with all the necessary installation already baked in, as well as docker-compose to execute the containers. To build the project simply run the command `docker-compose up` in the U_look_stunning folder.

### Testing:

We unfortunately did not have time to implement tests due to learning completely new languages as the project went on. 

### Api documentation:
https://peerjs.com/docs.html
http://expressjs.com/en/api.html
https://www.npmjs.com/package/nodemon
https://socket.io/docs/v4/index.html
https://www.npmjs.com/package/uuid
https://cmake.org/documentation/
https://ejs.co/#docs

### Made by 
[Simon Jensen](https://github.com/navyjumper)
[Lars Brodin Østby](https://github.com/Larsbost)
[Mads Lundegaard](https://github.com/treyshotz)
