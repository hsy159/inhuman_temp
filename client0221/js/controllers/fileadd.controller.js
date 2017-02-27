(function(){
    'use strict'
    
    angular
        .module('app')
        .controller('AddFileController',['$scope','Filestore',
        function($scope,Filestore){
            $scope.zzz=$scope.userfile;
            $scope.test = Filestore.upload({file:$scope.userfile});
        }]);

})();
