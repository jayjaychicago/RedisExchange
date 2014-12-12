var express = require('express');
var router = express.Router();

router.get('/', function(req, res) {
  html = ""
  html = html + '<html><a href="/objects/test/?id=foo">Base test is here</a><html>';
  html = html + "<BR></BR>"
 html = html + '<html><a href="/objects/test2/foo">Base test2 is here</a><html>';
  html = html + "<BR></BR>"
 html = html + '<html><a href="/objects/test3/fooo">Base test3 is here</a><html>';
  html = html + "<BR></BR>"
 html = html + '<html><a href="/users/createUser/?firstName=toto">Create user</a><html>';
  html = html + "<BR></BR>"
  res.send(html);
});


module.exports = router;
