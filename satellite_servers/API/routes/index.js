var express = require('express');
var router = express.Router();

router.get('/', function(req, res) {
  html = ""
  html = html + '<html><a href="http://54.174.141.230:3000/objects/test/?id=foo">Base test is here</a><html>';
  html = html + "<BR></BR>"
 html = html + '<html><a href="http://54.174.141.230:3000/objects/test2/foo">Base test2 is here</a><html>';
  html = html + "<BR></BR>"
 html = html + '<html><a href="http://54.174.141.230:3000/objects/test3/fooo">Base test3 is here</a><html>';
  html = html + "<BR></BR>"
 html = html + '<html><a href="http://54.174.141.230:3000/orders/">Execution is here</a><html>';
  html = html + "<BR></BR>"
  res.send(html);
});


module.exports = router;
