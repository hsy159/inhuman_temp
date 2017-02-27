(function () {
    'use strict';

    angular
        .module('app')
        .controller('MyReportController', ['ScanMetaData', 'Report', 'Client', '$scope',
            function (ScanMetaData, Report, Client, $scope) {

                $scope.my = Client.getCurrentId();
                $scope.myReport = [];
                $scope.myscanmetadata = [];
                ScanMetaData
                    .find(
                    {
                        filter: {
                            where: {
                                clientId: $scope.my
                            }
                        }
                    }
                    )
                    .$promise
                    .then(function (scanmetadata) {
                        for (var i = 0; i < scanmetadata.length; i++) {
                            $scope.myscanmetadata.push(scanmetadata[i]);
                            Report
                                .find(
                                {
                                    filter: {
                                        where: {
                                            metaData: scanmetadata[i].id
                                        }
                                    }
                                }
                                )
                                .$promise
                                .then(function (report) {
                                    $scope.myReport.push(report[0]);
                                });
                        }
                    });



                // $scope.my = Client.getCurrentId();
                // $scope.myMetaData = ScanMetaData.find(
                //     {
                //         where: {
                //             clientId: $scope.my
                //         }
                //     }
                // );
                // $scope.myReport = Report.find();
                // var _myMetaData = ScanMetaData.find(
                //     {
                //         where: {
                //             clientId: $scope.my
                //         }

                //     }
                // );
                // $scope.myMetaData = _myMetaData;
                // $scope.myMetaDataId = _myMetaData[0].scannerId;


                // $scope.myReport = Report.find(
                //     // {
                //     //     where: {
                //     //         metaData: $scope.myMetaDataId
                //     //     }
                //     // },
                //     function(err, result2) {
                //         console.log(result2);
                //     }
                // );
                // $scope.myMetaData = $filter("filter")(ScanMetaData, {clientId:$scope.my}, true)
                // $scope.myReport = $filter("filter")(Report, {metaData:$scope.myMetaData.getCurrentId})
                // $scope.myMetaData = ScanMetaData.find({
                //     where: {
                //         clientId:$scope.my
                //     }
                // }, function(err, result){
                //     console.log(result);
                // })
            }]);
})();
