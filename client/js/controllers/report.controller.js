(function(){
  'use strict';

angular
    .module('app')
    .controller('ReportController',['Report','$scope',
        function(Report,$scope){
            $scope.reports = Report.find();
        }]);
})();
