'use strict';

module.exports = function(Report) {
  // listReports
  Report.listReports = function(cb) {
    Report.find({
      fields: {
        //id: false
      }
    }, cb);
  };
  Report.remoteMethod('listReports', {
    returns: {
      arg: 'reports',
      type: 'array'
    },
    http: {
      path: '/list-reports',
      verb: 'get'
    }
  });
};
