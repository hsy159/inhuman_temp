(function () {
    'use strict';

    angular
        .module('app')
        .controller('ManagementController', ['ScanMetaData', 'Report', 'Client', 'Operator', '$scope',
            function (ScanMetaData, Report, Client, Operator, $scope) {
                $scope.my = Operator.getCurrentId();
                $scope.myScanMetaData = [];
                $scope.myClient = [];
                $scope.myReport = [];
                $scope.this_Year = new Date().getFullYear();
                $scope.birth = [];
                $scope.getAge = function(a,b) {
                    return b-a+1;
                }
                ScanMetaData
                    .find(
                    {
                        filter: {
                            where: {
                                operatorId: $scope.my
                            }
                        }
                    }
                    )
                    .$promise
                    .then(function (scanmetadata) {
                        for (var i = 0; i < scanmetadata.length; i++) {
                            $scope.myScanMetaData.push(scanmetadata[i]);
                            Client
                                .find(
                                {
                                    filter: {
                                        where: {
                                            id: scanmetadata[i].clientId
                                        }
                                    }
                                }
                                )
                                .$promise
                                .then(function (client) {
                                    $scope.myClient.push(client[0]);
                                    $scope.birth.push(client[0].birth);
                                });
                        }
                    });

            }]);
})();
