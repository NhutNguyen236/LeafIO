/**
 * MongoDB Schema for temperature
 * We will need to store the temperature value, the time it was recorded.
 * Turn on Timestamps to record time esp uploaded the records
 */

var mongoose = require("mongoose");
var Schema = mongoose.Schema;

(lightSchema = new Schema(
    {
        light: String,
    },
    { collection: "light", timestamps: true }
)),
    // model is very important, point to the right database(model) name to get access correctly
    (Light = mongoose.model("light", lightSchema));

// So we are now in Users.user
module.exports = Light;
