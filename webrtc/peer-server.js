const fs = require('fs');
const { PeerServer } = require('peer');
 
const peerServer = PeerServer({
  port: 443,
  ssl: {
    key: fs.readFileSync('/path/to/your/ssl/key/here.key'),
    cert: fs.readFileSync('/path/to/your/ssl/certificate/here.crt')
  }
});