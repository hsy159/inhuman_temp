(function(){
    'use strict'
    
    angular
        .module('app')
        .controller('FileController',['$scope','Filestore',
        function($scope,Filestore){
            $scope.model = Filestore.getFiles({container:'3dmodel'});
            $scope.upload = Filestore.getFiles({container:'uploadFile'});
        }]);

})();
