(function(){
  'use strict';

angular
    .module('app')
    .controller('MyReportController',['Report','User','$scope',
        function(Report,User,$scope){
            $scope.my = User.getCurrentId();
            $scope.myreport = Report.findById({id:$scope.my});
        }]);
})();
