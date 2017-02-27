(function () {
    'use strict'

    angular
        .module('app')
        .controller('BodySizeController', ['$scope', '$http', 'Report',
            function ($scope, $http, Report) {
                console.log('execute before...');
                $http
                    .get('/addon')
                    .then(function (data) {
                        $scope.test = data;
                        Report.create({
                            metaData: "test!!",
                            properties: $scope.test
                        });
                    });
            }
        ]);
})();