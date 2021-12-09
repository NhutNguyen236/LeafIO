var express = require("express");
var mongoose = require("mongoose");
var path = require("path");

var app = express();
var bodyParser = require("body-parser");

// view engine setup
app.set("views", path.join(__dirname, "views"));
app.set("view engine", "ejs");

// Get controllers
var tempController = require("./controllers/TempController");
app.use(express.static(path.join(__dirname, "public")));

// connect to MongoDB
var url = "mongodb://localhost:27017/LEAFIO";
var db = mongoose.connect(
    url,
    {
        useNewUrlParser: true,
        useUnifiedTopology: true,
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

// route get
app.get("/", function (req, res) {
    res.render("index");
});

app.use(bodyParser.urlencoded({ extended: false }));

app.post("/", function (req, res) {
    res.send("Got the temp data, thanks..!!");
    // save the record to the database
    // tempController.saveTemp(req.body);
    console.log(typeof req.body);
    console.log(req.body);
    console.log(JSON.stringify(req.body));
});

var server = app.listen(8000, () => {
    console.log("http://localhost:8000");
});
