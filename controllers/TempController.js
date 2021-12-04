// Add temperature to database
var mongoose = require("mongoose");
var express = require("express");
var router = express.Router();

// get temperature model
var Temperature = require("../models/temperature");

let TempController = {};

TempController.addTemperature = function (req, res) {
    var temp = new Temperature();
    temp.temperature = req.body.temperature;
    temp.save(function (err) {
        if (err) {
            res.send(err);
        }
        console.log("Temperature added");
    });
};
