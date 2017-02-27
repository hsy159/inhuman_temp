(function () {
    'use strict'
    angular
        .module('app')
        .controller('3DController', ['$scope', 'Report', 'User',
            function ($scope, Report, User) {
                $scope.my = User.getCurrentId();
                $scope.reportinfo = Report.findById({ id: $scope.my });

                if (!Detector.webgl) Detector.addGetWebGLMessage();
                var container, stats, controls, spotLight;
                var camera, cameraTarget, scene, renderer, vertical_grid;
                init();
                animate();

                function init() {

                    container = document.getElementById("3d_view");

                    camera = new THREE.PerspectiveCamera(70, window.innerWidth / window.innerHeight, 0.1, 500);
                    camera.position.set(0, 0, 0);

                    cameraTarget = new THREE.Vector3(0, 0, 0);

                    scene = new THREE.Scene();

                    var loader = new THREE.STLLoader();

                    // Binary files

                    var material = new THREE.MeshPhongMaterial({ color: 0xafeeee, specular: 0x111111, shininess: 200 });

                    loader.load('/api/filestores/3dmodel/download/example3.stl', function (geometry) {

                        var mesh = new THREE.Mesh(geometry, material);
                        //var mesh = new THREE.SkinnedMesh(geometry, material);
                        mesh.position.set(0.36, 0.7, 0.6);
                        mesh.scale.set(0.0008, 0.0008, 0.0008);

                        scene.add(mesh);
                    });

                    // Axis
                    var axisHelper1 = new THREE.AxisHelper(10);
                    var axisHelper2 = new THREE.AxisHelper(-10);
                    scene.add(axisHelper1);
                    scene.add(axisHelper2);
                    
                    // Light
       
                    var ambient = new THREE.AmbientLight(0x404040);
                    scene.add(ambient);
                    
                    spotLight = new THREE.SpotLight(0x404040);
                    spotLight.position.set(10, 10, 15);
                    spotLight.castShadow = true;
                    spotLight.shadow.camera.near = 8;
                    spotLight.shadow.camera.far = 30;
                    spotLight.shadow.camera.visible = false;
                    spotLight.shadow.mapSize.width = 1024;
                    spotLight.shadow.mapSize.height = 1024;
                    spotLight.name = 'Spot Light';
                    scene.add(spotLight);
                    // renderer

                    renderer = new THREE.WebGLRenderer({ antialias: true });
                    renderer.setClearColor(0xdddddd);
                    renderer.setPixelRatio(window.devicePixelRatio);
                    renderer.setSize(window.innerWidth/1.3, window.innerHeight/1.3);

                    renderer.gammaInput = true;
                    renderer.gammaOutput = true;

                    renderer.shadowMap.enabled = true;
                    renderer.shadowMap.renderReverseSided = false;

                    container.appendChild(renderer.domElement);


                    // orbit control

                    controls = new THREE.OrbitControls(camera, renderer.domElement);
                    controls.maxPolarAngle = Math.PI;
                    controls.enableZoom = true;
                    controls.enableDamping = true;
                    controls.dampingFactor = 1.25;

                    window.addEventListener('resize', onWindowResize, false);

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
            }]);

})();