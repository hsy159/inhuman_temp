(function () {
    'use strict'

    angular
        .module('app')
        .controller('AddOnController', ['$scope', '$http', 'LandmarkAuto',
            function ($scope, $http, LandmarkAuto) {
                console.log('execute before...');
                $http
                    .get('/addon')
                    .then(function (data) {
                        $scope.test = data;
                        LandmarkAuto.create({
                            properties: $scope.test
                        });
                    });
            }
        ]);
})();