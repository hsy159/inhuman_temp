(function () {
    'use strict'
    angular
        .module('app')
        .controller('3DController', ['$scope', 'LandmarkAuto', 'Client',
            function ($scope, LandmarkAuto, Client) {
                $scope.my = Client.getCurrentId();  //현재 로그인한 고객의 ID 찾음
                $scope.reportinfo_arr = []; 

                LandmarkAuto.find( 
                    // 자동추출한 특징점 DB에서 위에서 찾았던 현재 로그인 고객 ID와 일치하는 것을 찾음
                    {
                        filter: {
                            where: {
                                id: $scope.my
                            }
                        }
                    }
                )
                    .$promise
                    .then(function (reportinfo) {
                        $scope.reportinfo_arr.push(reportinfo[0]);
                        $scope.standard = $scope.reportinfo_arr[0].properties.data.standard;
                        $scope.non_standard = $scope.reportinfo_arr[0].properties.data.nonstandard;
                        threeD();
                    }); // 일치하는 ID로 찾은 것을 reportinfo_arr에 push하고 그 report 정보로 3D를 그림.

                function threeD() {
                    $scope.select_dot = function (confirmed, st_or_nonst, key) { 
                        // 점 테이블에서 선택된 점들의 색이 변한다.
                        if(confirmed == true) { // 만약 선택된 점들이면 색을 변환하는 함수 실행
                            selected_colorchage(st_or_nonst, key);
                        }
                        else {// 만약 선택된 점들이 아니면 색을 변환하지 않는 함수 실행
                            unselected_colorchage(st_or_nonst, key);
                        }                                            
                    };
                    if (!Detector.webgl) Detector.addGetWebGLMessage();
                    var container, stats, controls, spotLight;
                    var camera, cameraTarget, scene, renderer, vertical_grid;
                    $scope.standard_arr = [];
                    $scope.non_standard_arr = [];
                    init();
                    animate();

                    function init() {

                        container = document.getElementById("3d_view"); // 3d 그림을 담을 container를 설정.

                        camera = new THREE.PerspectiveCamera(70, window.innerWidth / window.innerHeight, 0.1, 500); 
                        camera.position.set(0, 0, 0); // 3d 그림을 쳐다볼 카메라 위치, 관점을 설정

                        cameraTarget = new THREE.Vector3(0, 0, 0);

                        scene = new THREE.Scene();

                        var loader = new THREE.STLLoader();

                        var material = new THREE.MeshPhongMaterial({ color: 0xafeeee, specular: 0x111111, shininess: 200 });
                        // 3d 파일을 load해서 위치와 크기를 조정한 후 scene에 추가.
                        loader.load('/api/filestores/3dmodel/download/20161013_125305_3914.stl', function (geometry) {

                            var mesh = new THREE.Mesh(geometry, material);
                            mesh.position.set(0.36, 0.7, 0.6);
                            mesh.scale.set(0.0008, 0.0008, 0.0008);

                            scene.add(mesh);
                        });

                        // Axis 축 생성
                        var axisHelper1 = new THREE.AxisHelper(10);
                        var axisHelper2 = new THREE.AxisHelper(-10);
                        scene.add(axisHelper1);
                        scene.add(axisHelper2);

                        // Light 이 부분이 없으면 몸이 검정색으로 나옴.
                        var ambient = new THREE.AmbientLight(0x404040);
                        scene.add(ambient);

                        // standard 점들의 위치를 그리는 것. 점은 3차원 구를 아주 조그맣게 만든 것
                        for (var dot in $scope.standard) {
                            var geo = new THREE.SphereGeometry(0.01, 100);
                            var mat = new THREE.MeshBasicMaterial({ color: 0x000000 });
                            var sphere = new THREE.Mesh(geo, mat);
                            sphere.position.set($scope.standard[dot].x * 0.0008 + 0.36, $scope.standard[dot].z * 0.0008 + 0.6, -($scope.standard[dot].y * 0.0008 + 0.7));
                            scene.add(sphere);
                            sphere.key_num = dot;
                            $scope.standard_arr.push(sphere);
                        }// non_standard 점들의 위치를 그리는 것.  점은 3차원 구를 아주 조그맣게 만든 것
                        for (var dot in $scope.non_standard) {
                            var geo = new THREE.SphereGeometry(0.01, 100);
                            var mat = new THREE.MeshBasicMaterial({ color: 0x000000 });
                            var sphere = new THREE.Mesh(geo, mat);
                            sphere.position.set($scope.non_standard[dot].x * 0.0008 + 0.36, $scope.non_standard[dot].z * 0.0008 + 0.6, -($scope.non_standard[dot].y * 0.0008 + 0.7));
                            scene.add(sphere);
                            sphere.key_num = dot;
                            $scope.non_standard_arr.push(sphere);
                        }
                        console.log($scope.standard_arr[0].material.color.r);

                        renderer = new THREE.WebGLRenderer({ antialias: true });
                        renderer.setClearColor(0xdddddd);
                        renderer.setPixelRatio(window.devicePixelRatio);
                        renderer.setSize(window.innerWidth / 1.3, window.innerHeight / 1.3);

                        renderer.gammaInput = true;
                        renderer.gammaOutput = true;

                        renderer.shadowMap.enabled = true;
                        renderer.shadowMap.renderReverseSided = false;

                        container.appendChild(renderer.domElement);


                        // orbit control 마우스로 control할 수 있도록 설정하는 부분
                        controls = new THREE.OrbitControls(camera, renderer.domElement);
                        controls.maxPolarAngle = Math.PI;
                        controls.enableZoom = true;
                        controls.enableDamping = true;
                        controls.dampingFactor = 1.25;

                        window.addEventListener('resize', onWindowResize, false);
                        // 파일을 정면에서 바라보도록 카메라를 0,0,1 set
                        camera.position.x = 0;
                        camera.position.y = 0;
                        camera.position.z = 1;
                    }
                    function onWindowResize() {
                        camera.aspect = window.innerWidth / window.innerHeight;
                        camera.updateProjectionMatrix();

                        renderer.setSize(window.innerWidth, window.innerHeight);
                    }

                    function animate() {
                        requestAnimationFrame(animate);
                        render();
                        controls.update();
                    }

                    function render() {
                        camera.lookAt(cameraTarget);
                        renderer.render(scene, camera);
                    }

                    function selected_colorchage(st_or_nonst, key) { // 선택된 점들의 색이 변하도록 하는 함수
                        if (st_or_nonst == 1) { // standard 점인 것들의 색을 변환하는 함수
                            for (var i in $scope.standard_arr) {
                                if ($scope.standard_arr[i].key_num == key) { // 선택된 것들을 확인해서
                                    $scope.standard_arr[i].material.color.r = 0xff; // 선택된 점이면 해당 색으로 변화
                                    console.log($scope.standard_arr[i].material.color);
                                    console.log(i);
                                    console.log($scope.standard_arr[i].key_num);
                                }
                            }
                        }
                        else {// non standard 점인 것들에 대한 색을 변환
                            for (var i in $scope.non_standard_arr) {
                                if ($scope.non_standard_arr[i].key_num == key) { // 선택된 것들을 확인해서
                                    $scope.non_standard_arr[i].material.color.r = 0xff; // 선택된 점이면 해당 색으로 변화
                                    console.log($scope.non_standard_arr[i].material.color);
                                    console.log(i);
                                    console.log($scope.non_standard_arr[i].key_num);
                                }
                            }
                        }
                    }
                    function unselected_colorchage(st_or_nonst, key) { // 선택되지 않은 점들의 색을 설정하는 함수
                        if (st_or_nonst == 1) { // standard 점인지 체크
                            for (var i in $scope.standard_arr) {
                                if ($scope.standard_arr[i].key_num == key) { // 선택되지 않은 것들을 확인해서
                                    $scope.standard_arr[i].material.color.r = 0x00; // 검정색으로 설정
                                    console.log($scope.standard_arr[i].material.color);
                                    console.log(i);
                                    console.log($scope.standard_arr[i].key_num);
                                }
                            }
                        }
                        else {
                            for (var i in $scope.non_standard_arr) { // nonstandard 점들의 배열에서
                                if ($scope.non_standard_arr[i].key_num == key) {// 선택되지 않은 것들을 확인해서
                                    $scope.non_standard_arr[i].material.color.r = 0x00;// 검정색으로 설정
                                    console.log($scope.non_standard_arr[i].material.color);
                                    console.log(i);
                                    console.log($scope.non_standard_arr[i].key_num);
                                }
                            }
                        }
                    }
                }


            }]);
})();
