// Add temperature to database
var mongoose = require("mongoose");
var express = require("express");
var router = express.Router();

// get temperature model
var Light = require("../models/light");

let LightController = {};

LightController.addLight = function (req, res) {
    var light = new Light();
    light.light = req.body.light;
    light.save(function (err) {
        if (err) {
            res.send(err);
        }
        console.log("Light added");
    });
};

module.exports = LightController;