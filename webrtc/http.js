var express = require('express')

//Http server for redirecting to https
var httpServer = express();
httpServer.get('*', (req, res) => {  
    res.redirect('https://' + req.headers.host + req.url);
})

httpServer.listen(80)
