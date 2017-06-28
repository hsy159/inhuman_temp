(function () {
    'use strict'

    angular
        .module('app')
        .controller('BodySizeController', ['$scope', '$http', 'Report','Client','ScanMetaData',
            function ($scope, $http, Report,Client,ScanMetaData) {
                console.log('execute before...');
                $http
                    .get('/bodysize') //route의 bodysize 실행(특징점으로부터 신체의 사이즈를 계산하는 함수)
                    .then(function (data) {
                        $scope.clientId = Client.getCurrentId(); // 현재 로그인한 client의 ID를 찾는다.
                        //$scope.meta = ScanMetaData.find({clientId:'$scope.clientId'});
                        $scope.test = data; // 계산한 신체 사이즈를 test라는 변수에 저장
                        Report.create({ // report DB에 계산한 신체 사이즈와 테스트용 metaData를 설정해서 저장
                            //metaData: $scope.meta,
                            metaData: "test!",
                            properties: $scope.test
                        });
                    });
            }
        ]);
})();