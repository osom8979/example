var express = require('express');
var router = express.Router();

var exec = require('child_process').exec;
var cmd = 'scanimage -d "smfp:usb;04e8;3469;071KB8KFBA00T6H" --format tiff > /home/share/scan.tiff';

/* GET home page. */
router.get('/', function(req, res, next) {
  res.render('index', { title: 'BOGO-SCANNER' });
});

router.post('/scan', function(req, res, next) {
    exec(cmd, function(error, stdout, stderr) {
      // command output is in stdout
      console.log('scan!');
    });
  res.send('Send the scan query...<br />Please check the shared directory.');
});

module.exports = router;

