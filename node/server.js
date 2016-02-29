var http = require('http');
var fs = require('fs');
var config = require('./config.js');
var server = http.createServer();

server.on('request', function(req, res) {
    fs.readFile(__dirname + '/' + config.htdoc + '/' + req.url, 'utf-8', function(err, data) {
        if (err) {
            res.writeHead(404, {'Content-Type': 'text/plain'});
            res.write('Not found resource.');
            return res.end();
        }
        res.writeHead(200, {'Content-Type': 'text/plain'});
        res.write(data);
        res.end();
    });
});
server.listen(config.port);

console.log(config);
console.log('Server listening ...');

