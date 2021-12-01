var express = require('express');
var app = express(); 
var bodyParser = require('body-parser') 

app.get('/', function (req, res) { 
    res.send("Hello from Server"); 
})

app.use(bodyParser.urlencoded({ extended: false })) 
app.post('/', function(req, res) {    
    res.send('Got the temp data, thanks..!!');     
    console.log(JSON.stringify(req.body)); 
}) 

var server = app.listen(8000, () => {    
    console.log("http://localhost:8000") 
})