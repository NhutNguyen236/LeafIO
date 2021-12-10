var express = require("express");
var app = express();
var ejs = require("ejs");
var mongoose = require("mongoose");
var path = require("path");
var server = require("http").createServer(app);
var io = require("socket.io")(server);

// dotenv config
var dotenv = require("dotenv");
dotenv.config({ path: "./config/.env" });

// nodemailer for sending emails and nodemailer-handlebars for templating mailing
var nodemailer = require("nodemailer");
const hbs = require("nodemailer-handlebars");

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

// ======================Nodemailer email config==============================//
// transporter for sending emails
const transporter = nodemailer.createTransport({
    service: "gmail",
    auth: {
        // email and password is in .env but you have to define your new one :D
        user: process.env.EMAIL,
        pass: process.env.EMAIL_PASSWORD,
    },
});

// Send to client a waiting email
transporter.use(
    "compile",
    hbs({
        viewEngine: {
            defaultLayout: false,
            extName: "express-handlebars",
        },
        viewPath: "./views",
    })
);

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

                            // send email to client if the soil moisture is less than 10%
                            if (data.soil < 10) {
                                let mailOptionsClient = {
                                    from: process.env.EMAIL,
                                    to: "phamp9331@gmail.com",
                                    subject:
                                        "⚠ Please keep your eyes on your garden ⚠",
                                    template: "mail",
                                    context: {
                                        name: 'Nhut',
                                        temperature: data.temp,
                                        soil: data.soil,
                                        light: data.light
                                    }, // send extra values to template
                                };

                                transporter.sendMail(
                                    mailOptionsClient,
                                    (err, data) => {
                                        var msg = undefined;
                                        if (err) {
                                            console.log(err);
                                            msg =
                                                "We are facing some technical difficulties here, come back later 😥";
                                            console.log(msg);
                                        }
                                        msg = "OK";
                                        console.log(msg)
                                    }
                                );
                            }
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
var pump = { state: false };
var fan = { state: false };
var manualmode = { state: false };

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

    // emit a socket for water pump
    io.sockets.emit("pump", pump);
    client.on("flow", function (state) {
        pump.state = !pump.state;
        console.log("id: " + client.id + " water pump: " + pump.state);
        io.sockets.emit("pump", pump);
    });

    // emit a socket for garden fan
    io.sockets.emit("fan", fan);
    client.on("spin", function (state) {
        fan.state = !fan.state;
        console.log("id: " + client.id + " fan: " + fan.state);
        io.sockets.emit("fan", fan);
    });

    // emit a socket for triggering manual mode
    io.sockets.emit("mode", manualmode);
    client.on("trigger", function (state) {
        manualmode.state = !manualmode.state;
        console.log("id: " + client.id + " manualmode: " + manualmode.state);
        io.sockets.emit("mode", manualmode);
    });
});

// define port
const port = process.env.PORT || 3000;

var server = server.listen(port, () => {
    console.log(`http://localhost:${port}`);
});
