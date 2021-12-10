// Add temperature to database
var mongoose = require("mongoose");
var express = require("express");
var router = express.Router();

// get temperature model
var Soil = require("../models/soil");

let SoilController = {};

SoilController.addSoil = function (req, res) {
    var soil = new Soil();
    soil.soil = req.body.soil;
    soil.save(function (err) {
        if (err) {
            res.send(err);
        }
        console.log("Soil added");
    });
};

module.exports = SoilController;