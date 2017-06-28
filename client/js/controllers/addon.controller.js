(function () {
    'use strict'

    angular
        .module('app')
        .controller('AddOnController', ['$scope', '$http', 'LandmarkAuto',
            function ($scope, $http, LandmarkAuto) {
                console.log('execute before...');
                $http
                    .get('/addon')  //route의 addon 실행(특징점 찾는 함수)
                    .then(function (data) {
                        $scope.test = data;
                        LandmarkAuto.create({ // 찾아낸 자동 특징점을 갖고 LandmarkAuto DB에 추가.
                            AutoSizer_Ver:"0.0.1",
                            SizeKorea_Ver:"0.0.6",                            
                            properties: $scope.test //특징점을 LandmarkAuto에 생성. 누구의 정보인지 알 수 있도록 Client_id같은 것도 추가해야합니다.
                        });
                    });
            }
        ]);
})();