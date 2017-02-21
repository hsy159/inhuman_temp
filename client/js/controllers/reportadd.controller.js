(function(){
  'use strict';

angular
    .module('app')
    .controller('AddReportController',['Report','User','$scope','$state',
        function (Report,User,$scope,$state){
                $scope.userid = User.getCurrentId();
                $scope.input ={
                    email:'',
                    description:''
                };
                $scope.reportadd = function(){
                    Report.create({
                        operator_id:$scope.userid,
                        client_email:$scope.input.email,
                        description:$scope.input.description
                    });
                $state.go('myreport');
                }
        }
        ]);
    }
)();