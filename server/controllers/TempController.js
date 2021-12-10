// Add temperature to database
var mongoose = require("mongoose");
var express = require("express");
var router = express.Router();

// get temperature model
var Temperature = require("../models/temperature");

let TempController = {};

TempController.addTemp = function (req, res) {
    var temp = new Temperature();
    temp.temperature = req.body.temperature;
    temp.save(function (err) {
        if (err) {
            res.send(err);
        }
        console.log("Temperature added");
    });
};

// get all the temp from database
TempController.getTemp = function (req, res) {
    Temperature.find(function (err, temperatures) {
        if (err) {
            res.send(err);
        }
        res.json(temperatures);
    });
};

// get temperature by the nearest date
TempController.getTempByNearestDate = function () {
    var temperature = 0;

    Temperature.findOne({}).sort({createdAt: -1}).then(function(temp){
        temperature = temp.temperature;
        returnTemp();
    })

    console.log("hello" + temperature);
    
    function returnTemp() {
        return temperature;
    }
};

module.exports = TempController;