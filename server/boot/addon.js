// const createObject = require('../../build/Debug/addon');

// const obj = createObject();

// // // 순서 1
// // // 특징점 계산
// // j = obj.getLandmark(0, "addon/20161013_125305_3914.stl");
// // // console.log(typeof(j));
// // console.log('[순서 1 getLandmark]');
// // console.log(j);

// // // 순서 2
// // // 특징점 수동보정
// // console.log('[순서 2 특징점 수동보정]');

// // // 순서 3
// // // 치수 계산
// // j = obj.getBodySize(0, "addon/20161013_125305_3914.stl");
// // // console.log(typeof(j));
// // console.log('[순서 3 getBodySize]');
// // console.log(j);

// // // 순서 4
// // // 리포트 계산
// var All_array = new Array();
// var BodySize = obj.getBodySize(0, "addon/20161013_125305_3914.stl");
// All_array.push(BodySize);
// obj.loadFile("addon/20161013_125305_3914.stl");
// console.log('[순서 4 리포트 계산]');

// var j;
// /*
// input : Img/VnS_F.png, VnS_M.png
// m_ClickPoints 8개
// 1 - 11 목부위 목뒤점 Cervicale >>>> 목뒤점
// 2 - 18 몸통부위 겨드랑점* Axilla >>>> 겨드랑점R
// 3 - 18 몸통부위 겨드랑점* Axilla >>>> 겨드랑점L
// 4 - 28 몸통부위 젖가슴아래점* Inferior Breast >>>> 젖가슴아래점(UnderBust) >>>> 남자 값 필요!!!
// 5 - 34 몸통부위 배꼽점* Anterior Waist >>>> 배꼽점(Navel)
// 6 - 40 몸통부위 샅점* Crotch >>>> 샅점(Crotch)
// 7 - 44 다리부위 무릎뼈가운데점 Midpatella >>>> 무릎뼈가운데점(Knee) R 필요!!!
// 8 - 44 다리부위 무릎뼈가운데점 Midpatella >>>> 무릎뼈가운데점(Knee) L 필요!!!
// */
// j = obj.calcVnSData(
//   [
//     { x: 1, y: 2, z: 3 }, // 11 >> 높이
//     { x: 2, y: 5, z: 6 }, // 18R
//     { x: 3, y: 5, z: 6 }, // 18L
//     { x: 4, y: 5, z: 6 }, // 28 >> 높이
//     { x: 5, y: 5, z: 6 }, // 34 >> 높이
//     { x: 6, y: 5, z: 6 }, // 40 >> 높이
//     { x: 7, y: 5, z: 6 }, // 44R
//     { x: 8, y: 5, z: 6 } // 44L
//   ]
// );
// //console.log(typeof(j));
// console.log('[calcVnSData]');
// console.log(j);
// All_array.push(j);
// /*
// input : Img/Lateral_F.png, Lateral_M.png
// m_ClickPoints 10개
// 1 - 귀R 필요!!!
// 2 - 귀L 필요!!!
// 3 - 16 몸통부위 어깨가쪽점 -오른쪽과 왼쪽 Lateral Shoulder >>>> 어깨가쪽점R(Shoulder-R)
// 4 - L
// 5 - 엉덩이 돌출부의 옆점 RL 필요!!!
// 6 - L
// 7 - 무릎 관절점 RL 필요!!!
// 8 - L
// 9 - 51 다리부위 가쪽복사점 Lateral Malleous >>>> 가쪽복사점(Ankle) RL 필요!!!
// 10 - L
// */
// j = obj.calcLateralData(
//   [
//     { x: 1, y: 2, z: 3 }, // 귀R
//     { x: 2, y: 5, z: 6 }, // 귀L
//     { x: 3, y: 5, z: 6 }, // 16R
//     { x: 4, y: 5, z: 6 }, // 16L
//     { x: 5, y: 5, z: 6 }, // 엉덩이 돌출부의 옆점 R
//     { x: 6, y: 5, z: 6 }, // 엉덩이 돌출부의 옆점 L
//     { x: 7, y: 5, z: 6 }, // 무릎 관절점 R
//     { x: 8, y: 5, z: 6 }, // 무릎 관절점 L
//     { x: 8, y: 5, z: 6 }, // 51R
//     { x: 8, y: 5, z: 6 } // 51L
//   ]
// );
// // console.log(typeof(j));
// console.log('[calcLateralData]');
// console.log(j);
// All_array.push(j);

// /*
// input : Img/HipKnee_F.png, HipKnee_M.png
// m_ClickPoints 6개
// 1 - 51 다리부위 가쪽복사점 Lateral Malleous >>>> 가쪽복사점(Ankle) RL 필요!!!
// 2 - 44 다리부위 무릎뼈가운데점 Midpatella >>>> 무릎뼈가운데점(Knee) R 필요!!!
// 3 - 44 다리부위 무릎뼈가운데점 Midpatella >>>> 무릎뼈가운데점(Knee) L 필요!!!
// 4 - 46 다리부위 넙다리가운데점 Midthigh 필요!!!
// 5 - 67 팔부위 손목가쪽점 Radial Styloid >>>> 손목가쪽점-R(Wrist Out)
// 6 - 67 팔부위 손목가쪽점 Radial Styloid >>>> L 필요!!!
// */
// j = obj.calcHipKneeData(
//   [
//     { x: 1, y: 2, z: 3 }, // 51R 또는 51RL >> 높이
//     { x: 2, y: 5, z: 6 }, // 44R
//     { x: 3, y: 5, z: 6 }, // 44L
//     { x: 4, y: 5, z: 6 }, // 46 >> 높이
//     { x: 5, y: 5, z: 6 }, // 67R
//     { x: 6, y: 5, z: 6 } // 67L
//   ]
// );
// // console.log(typeof(j));
// console.log('[calcHipKneeData]');
// console.log(j);
// All_array.push(j);

// /*
// input : Img/Spine_F.png, Spine_M.png
// m_ClickPoints 5개
// 1 - 턱 점 필요!!!
// 2 - 16 몸통부위 어깨가쪽점 Lateral Shoulder R >>>> 어깨가쪽점R(Shoulder-R)
// 3 - 16 몸통부위 어깨가쪽점 Lateral Shoulder L >>>> 어깨가쪽점L(Shoulder-L)
// 4 - 18 몸통부위 겨드랑점* Axilla >>>> 겨드랑점R(Armpit-R)
// 5 - 18 몸통부위 겨드랑점* Axilla >>>> 겨드랑점L(Armpit-L)
// */
// j = obj.calcSpineData(
//   [
//     { x: 1, y: 2, z: 3 }, // 턱 점 >> 높이
//     { x: 2, y: 5, z: 6 }, // 16R
//     { x: 3, y: 5, z: 6 }, // 16L
//     { x: 4, y: 5, z: 6 }, // 18R
//     { x: 5, y: 5, z: 6 } // 18L
//   ]
// );
// // console.log(typeof(j));
// console.log('[calcSpineData]');
// console.log(j);
// All_array.push(j);

// /*
// input : Img/Shape_F.png, Shape_M.png
// m_ClickPoints 3개
// 1 - 27 몸통부위 젖꼭지점 Nipple >>>> 젖꼭지점RL 평균
// 2 - 34 몸통부위 배꼽점* Anterior Waist >>>> 배꼽점(Navel)
// 3 - 38 몸통부위 엉덩이돌출점 Buttock Protrusion >>>> 엉덩이돌출점(Hip)
// */
// j = obj.calcShapeData(
//   [
//     { x: 1, y: 2, z: 3 }, // 27 >> RL 평균 높이
//     { x: 2, y: 5, z: 6 }, // 34 >> 높이
//     { x: 3, y: 5, z: 6 } // 38 >> 높이
//   ]
// );
// // console.log(typeof(j));
// console.log('[calcShapeData]');
// console.log(j);
// All_array.push(j);
// console.log("push test!");
// console.log(All_array);

