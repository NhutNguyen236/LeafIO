/**
 * MongoDB Schema for temperature
 * We will need to store the temperature value, the time it was recorded.
 * Turn on Timestamps to record time esp uploaded the records
 */

var mongoose = require("mongoose");
var Schema = mongoose.Schema;

(tempSchema = new Schema(
    {
        temperature: String
    },
    { collection: "temperature", timestamps: true }
)),
    // model is very important, point to the right database(model) name to get access correctly
    (Temperature = mongoose.model("temp", tempSchema));

// So we are now in Users.user
module.exports = Temperature;
