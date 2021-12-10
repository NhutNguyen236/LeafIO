var express = require("express");
var app = express();
var ejs = require("ejs");
var mongoose = require("mongoose");
var path = require("path");
var server = require("http").createServer(app);
var io = require("socket.io")(server);
var nodemailer = require("nodemailer");

var bodyParser = require("body-parser");

// view engine setup
app.set("views", path.join(__dirname, "views"));
app.set("view engine", "ejs");
app.use(bodyParser.urlencoded({ extended: false }));

// Get controllers
var tempController = require("./controllers/TempController");
var soilController = require("./controllers/SoilController");
var lightController = require("./controllers/LightController");

// get models
var Temperature = require("./models/temperature");
var Soil = require("./models/soil");
var Light = require("./models/light");

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
    // init an empty json object for data
    let data = {};
    // get temperature, soil, light data from mongodb with the nearest time
    Temperature.findOne({})
        .sort({ createdAt: -1 })
        .then(function (temp) {
            data.temp = temp.temperature;

            Soil.findOne({})
                .sort({ createdAt: -1 })
                .then(function (soil) {
                    data.soil = soil.soil;

                    Light.findOne({})
                        .sort({ createdAt: -1 })
                        .then(function (light) {
                            data.light = light.light;
                            

                            res.render("index", { data: data });
                        });
                });
        });
});

// app.post("/", function (req, res) {
//     res.send("Got the temp data, thanks..!!");
//     // save the record to the database
//     // tempController.saveTemp(req.body);
//     console.log(req.body.temperature);

//     // add temperature to the database
//     tempController.addTemp(req, res);

//     // add soil moisture to the database
//     soilController.addSoil(req, res);

//     // add light to the database
//     lightController.addLight(req, res);

//     console.log(JSON.stringify(req.body));
// });

var light = { state: false };
var automode = { state: false };

// socket.io part
io.on("connection", function (client) {
    console.log("Client connected...");

    // handle client connection
    client.on("join", function (data) {
        console.log(data);
    });

    // handle client disconnection
    client.on("disconnect", function () {
        console.log("Client disconnected!");
    });

    // emit a socket for toggling led
    io.sockets.emit("led", light);
    client.on("toggle", function (state) {
        light.state = !light.state;
        console.log("id: " + client.id + " light: " + light.state);
        io.sockets.emit("led", light);
    });

    // emit a socket for triggering manual mode
    io.sockets.emit("mode", automode);
    client.on("trigger", function (state) {
        automode.state = !automode.state;
        light.state = light.state;
        console.log("id: " + client.id + " automode: " + automode.state);
        console.log("id: " + client.id + " light: " + light.state);
        io.sockets.emit("led", light, "mode", automode);
    });
});

// define port
const port = process.env.PORT || 3000;

var server = server.listen(port, () => {
    console.log(`http://localhost:${port}`);
});
