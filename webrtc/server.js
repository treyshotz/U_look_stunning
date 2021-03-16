var express = require('express');
var app = express();
var {"v4": uuidv4} = require('uuid');
var http = require('http');

var httpServer = http.createServer(app);

httpServer.listen(3001);


//For https
//var https = require('https');
//var privateKey  = fs.readFileSync('sslcert/server.key', 'utf8');
//var certificate = fs.readFileSync('sslcert/server.crt', 'utf8');
//var credentials = {key: privateKey, cert: certificate};
//var httpsServer = https.createServer(credentials, app);
//httpsServer.listen(8443);


app.set('view', 'ejs')
app.use(express.static('public'))

//If user doesn't enter any room id a random one is created and passed
app.get('/', (req, res) => {
    res.redirect(`/${uuidv4()}`)
})

app.get('/room', (req, res) => {
    res.render('room', { roomId: req.params.room})
})