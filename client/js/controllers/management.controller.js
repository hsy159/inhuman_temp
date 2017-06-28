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
                $scope.birth_Year = [];
                $scope.age = [];
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
                                    $scope.birth_Year.push(new Date(client[0].birth).getFullYear());
                                    $scope.age.push($scope.this_Year - $scope.birth_Year[$scope.birth_Year.length-1]+1);
                                });
                        }
                    });

            }]);
})();
