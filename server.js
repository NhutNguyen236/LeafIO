var express = require("express");
var mongoose = require("mongoose");

var app = express();
var bodyParser = require("body-parser");

// Get controllers
var tempController = require("./controllers/TempController");

// connect to MongoDB
var url =
    "mongodb://localhost:27017/LEAFIO";
var db = mongoose.connect(
    url,
    {
        useNewUrlParser: true,
        useUnifiedTopology: true
    },
    (err) => {
        if (!err) {
            console.log("MongoDB Connection Succeeded.");
        } else {
            console.log("Error in DB connection : " + err);
        }
    }
);

var db = mongoose.connection;
db.on("error", console.error.bind(console, "connection error:"));
db.once("open", function () {});
app.get("/", function (req, res) {
    res.send("Hello from Server");
});

app.use(bodyParser.urlencoded({ extended: false }));
app.post("/", function (req, res) {
    res.send("Got the temp data, thanks..!!");
    // save the record to the database
    // tempController.saveTemp(req.body);
    console.log(req.body.temp)
    console.log(JSON.stringify(req.body));
});

var server = app.listen(8000, () => {
    console.log("http://localhost:8000");
});
