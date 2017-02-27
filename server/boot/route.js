'use strict';

module.exports = function (app) {
  app.get('/addon', function (req, res) {
    const createObject = require('../../build/Debug/addon');
    const obj = createObject(20);

    var landmark = obj.getLandmark(0, "addon/20161013_125305_3914.stl");
    console.log(typeof(landmark));
    console.log(landmark);

    // var BodySize = obj.getBodySize(0, "addon/20161013_125305_3914.stl");
    // console.log(typeof (BodySize));
    //console.log(BodySize);
    res.send(landmark);
  });
  
  app.get('/bodysize', function (req, res) {
    const createObject = require('../../build/Debug/addon');
    const obj = createObject(20);

    var BodySize = obj.getBodySize(0, "addon/20161013_125305_3914.stl");
    console.log(typeof (BodySize));
    console.log(BodySize);
    res.send(BodySize);
  });
 };