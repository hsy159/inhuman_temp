// http://loopback.io/doc/en/lb3/Create-new-models.html#update-boot-script-to-add-data
var async = require('async');

module.exports = function(app) {
  //data sources
  var mongoDs = app.dataSources.inhuman;
  var sizekorea_ver = '0.0.6';

  //create all models
  async.parallel({
    parallel_sizekorea: async.apply(createSizeKorea)
  }, function(err, results) {
    console.log(results);
    if (err)
      throw err;

    console.log('> sizekorea model created sucessfully[' + sizekorea_ver + ']');
  });

  //create SizeKorea
  function createSizeKorea(cb) {
    //console.log(1);
    var SizeKorea = app.models.SizeKorea;

    /*
    // 20170224 sdk 현재 모델에서 findOrCreate 가 동작하지 않음.
    SizeKorea.findOrCreate({
      where: {
        version: sizekorea_ver
      }
    }, sizekoreaInit, function(err, instance, created) {
      console.log(instance);
      console.log(created);
      console.log(11);
      //console.log(sizekoreaInit);
      if (err) {
        console.log(111);
        //console.log(err);
        return cb(err);
      }
    });
    */

    SizeKorea.findOne({
      where: {
        version: sizekorea_ver
      }
    }, sizekoreaInit, function(err, model) {
      //console.log(model);
      //console.log(11);

      if (err) {
        //console.log(111);
        return cb(err);
      }

      if (null == model) {
        SizeKorea.create(sizekoreaInit, cb);
      } else {
        console.log('> sizekorea model existed[' + sizekorea_ver + ']');
      }
    });
  };

  var sizekoreaInit = {
    version: sizekorea_ver,
    makedate: new Date(),
    landmark: {
      '1': { part: '머리부위', std: '머리마루점*', eng: 'Vertex' },
      '2': { part: '머리부위', std: '머리옆점*', eng: 'Euryon', rl: true },
      '3': { part: '머리부위', std: '눈살점*', eng: 'Glabella' },
      '4': { part: '머리부위', std: '눈초리점*', eng: 'Ectocanthus' },
      '5': { part: '머리부위', std: '눈확아래점*', eng: 'Inferior Orbitale(Infraorbitale)' },
      '6': { part: '머리부위', std: '귀구슬점*', eng: 'Tragion', rl: true },
      '7': { part: '머리부위', std: '코뿌리점*', eng: 'Sellion(Nasion)' },
      '8': { part: '머리부위', std: '턱끝점*', eng: 'Menton' },
      '9': { part: '머리부위', std: '뒤통수돌출점*', eng: 'Occiput' },
      '10': { part: '머리부위', std: '뒤통수점*', eng: 'Inion' },
      '11': { part: '목부위', std: '목뒤점', eng: 'Cervicale' },
      '12': { part: '목부위', std: '목앞점', eng: 'Anterior Neck' },
      '13': { part: '목부위', std: '목옆점', eng: 'Lateral Neck', rl: true },
      '14': { part: '목부위', std: '방패연골아래점*', eng: 'Inferior Thyroid(Infrathyroid)' },
      '15': { part: '몸통부위', std: '몸통 어깨점', eng: 'Acromion', rl: true },
      '16': { part: '몸통부위', std: '어깨가쪽점', eng: 'Lateral Shoulder', rl: true },
      '17': { part: '몸통부위', std: '어깨가운데점', eng: 'Midshoulder' },
      '18': { part: '몸통부위', std: '겨드랑점*', eng: 'Axilla' },
      '19': { part: '몸통부위', std: '겨드랑앞점', eng: 'Anterior Axilla', rl: true },
      '20': { part: '몸통부위', std: '겨드랑뒤점', eng: 'Posterior Axilla', rl: true },
      '21': { part: '몸통부위', std: '겨드랑앞벽점', eng: 'Anterior Midaxilla', rl: true },
      '22': { part: '몸통부위', std: '겨드랑뒤벽점', eng: 'Posterior Midaxilla', rl: true },
      '23': { part: '몸통부위', std: '등뼈위겨드랑수준점', eng: 'Axillary Level at Midspine' },
      '24': { part: '몸통부위', std: '겨드랑앞접힘점*', eng: 'Anterior Axillary Fold', rl: true },
      '25': { part: '몸통부위', std: '겨드랑뒤접힘점*', eng: 'Posterior Axillary Fold', rl: true },
      '26': { part: '몸통부위', std: '복장뼈가운데점', eng: 'Mesosternal' },
      '27': { part: '몸통부위', std: '젖꼭지점', eng: 'Nipple', rl: true },
      '28': { part: '몸통부위', std: '젖가슴아래점*', eng: 'Inferior Breast' },
      '29': { part: '몸통부위', std: '열째갈비뼈점*', eng: 'Tenth Rib' },
      '30': { part: '몸통부위', std: '엉덩뼈능선점*', eng: 'liocristale' },
      '31': { part: '몸통부위', std: '허리옆점<자연스럽게 들어간 자리>', eng: 'Lateral Waist', rl: true },
      '32': { part: '몸통부위', std: '허리앞점', eng: 'Anterior Waist' },
      '33': { part: '몸통부위', std: '허리뒤점', eng: 'Posterior Waist' },
      '34': { part: '몸통부위', std: '배꼽점*', eng: 'Anterior Waist' },
      '35': { part: '몸통부위', std: '배꼽수준허리옆점', eng: 'Lateral Waist', rl: true },
      '36': { part: '몸통부위', std: '배꼽수준허리뒤점', eng: 'Posterior Waist' },
      '37': { part: '몸통부위', std: '위앞엉덩뼈가시점', eng: 'Anterior Superior Iliac Spine' },
      '38': { part: '몸통부위', std: '엉덩이돌출점', eng: 'Buttock Protrusion' },
      '39': { part: '몸통부위', std: '볼기고랑점', eng: 'Gluteal Fold' },
      '40': { part: '몸통부위', std: '샅점*', eng: 'Crotch' },
      '41': { part: '몸통부위', std: '앉은배돌출점*', eng: 'Abdominal Protrusion, sitting' },
      '42': { part: '다리부위', std: '다리 정강뼈위점', eng: 'Tibiale' },
      '43': { part: '다리부위', std: '무릎뼈위점', eng: 'Superior Patella(Suprapatella)' },
      '44': { part: '다리부위', std: '무릎뼈가운데점', eng: 'Midpatella' },
      '45': { part: '다리부위', std: '무릎뼈아래점', eng: 'Inferior Patella(Infrapatella)' },
      '46': { part: '다리부위', std: '넙다리가운데점', eng: 'Midthigh' },
      '47': { part: '다리부위', std: '오금점*', eng: 'Posterior Juncture of Calf and Thigh' },
      '48': { part: '다리부위', std: '장딴지돌출점*', eng: 'Calf Protrusion' },
      '49': { part: '다리부위', std: '종아리아래점*', eng: 'Inferior Leg' },
      '50': { part: '다리부위', std: '안쪽복사점*', eng: 'Medial Malleous' },
      '51': { part: '다리부위', std: '가쪽복사점', eng: 'Lateral Malleous' },
      '52': { part: '다리부위', std: '앉은넙다리위점*', eng: 'Superior Thigh, sitting' },
      '53': { part: '다리부위', std: '앉은무릎앞점*', eng: 'Anterior Knee, sitting' },
      '54': { part: '다리부위', std: '앉은오금점*', eng: 'Posterior Juncture of Calf and Thigh, sitting' },
      '55': { part: '발부위', std: '발안쪽점', eng: 'Metatarsophalangeal Ⅰ' },
      '56': { part: '발부위', std: '발가쪽점', eng: 'Metatarsophalangeal Ⅴ' },
      '57': { part: '발부위', std: '발끝점*', eng: 'Acropodion' },
      '58': { part: '발부위', std: '발꿈치점*', eng: 'Pternion' },
      '59': { part: '팔부위', std: '팔 어깨세모근점*', eng: 'Deltoid Point', rl: true },
      '60': { part: '팔부위', std: '위팔두갈래근점*', eng: 'Biceps' },
      '61': { part: '팔부위', std: '노뼈위점', eng: 'Radiale' },
      '62': { part: '팔부위', std: '팔꿈치가운데점', eng: 'Center Olecranon' },
      '63': { part: '팔부위', std: '팔꿈치뒤점*', eng: 'Rear Olecranon' },
      '64': { part: '팔부위', std: '팔꿈치아래점*', eng: 'Bottom Olecranon' },
      '65': { part: '팔부위', std: '팔꿈치가쪽점*', eng: 'Lateral Humeral Epicondyle' },
      '66': { part: '팔부위', std: '손목안쪽점', eng: 'Ulnar Styloid' },
      '67': { part: '팔부위', std: '손목가쪽점', eng: 'Radial Styloid' },
      '68': { part: '손부위', std: '손안쪽점', eng: 'Metacarpale Ⅴ' },
      '69': { part: '손부위', std: '손가쪽점', eng: 'Metacarpale Ⅱ' },
      '70': { part: '손부위', std: '손끝점*', eng: 'Dactylion Ⅲ' }
    },
    measurement: {
      '101': { part: '높이/너비/두께', std: '머리위로뻗은주먹높이*', eng: 'Overhead Fist Reach' },
      '102': { part: '높이/너비/두께', std: '키*', eng: 'Stature' },
      '103': { part: '높이/너비/두께', std: '눈높이*', eng: 'Eye Height' },
      '104': { part: '높이/너비/두께', std: '목뒤높이*', eng: 'Cervical Height' },
      '105': { part: '높이/너비/두께', std: '어깨높이*', eng: 'Acromion Height' },
      '106': { part: '높이/너비/두께', std: '어깨가쪽높이*', eng: 'Shoulder Height' },
      '107': { part: '높이/너비/두께', std: '겨드랑높이*', eng: 'Axilla Height' },
      '108': { part: '높이/너비/두께', std: '굽힌팔꿈치높이*', eng: 'Elbow Height' },
      '109': { part: '높이/너비/두께', std: '엉덩이높이*', eng: 'Hip Height' },
      '110': { part: '높이/너비/두께', std: '샅높이*', eng: 'Crotch Height' },
      '111': { part: '높이/너비/두께', std: '주먹높이*', eng: 'Fist Height' },
      '112': { part: '높이/너비/두께', std: '허리높이*', eng: 'Waist Height' },
      '113': { part: '높이/너비/두께', std: '배꼽수준허리높이*', eng: 'Waist Height (Omphalion)' },
      '114': { part: '높이/너비/두께', std: '위앞엉덩뼈가시높이*', eng: 'Iliac Spine Height' },
      '115': { part: '높이/너비/두께', std: '무릎높이*', eng: 'Knee Height' },
      '116': { part: '높이/너비/두께', std: '가슴너비*', eng: 'Chest Breadth' },
      '117': { part: '높이/너비/두께', std: '젖가슴너비*', eng: 'Bust Breadth' },
      '118': { part: '높이/너비/두께', std: '허리너비*', eng: 'Waist Breadth I(Natural Indentation)' },
      '119': { part: '높이/너비/두께', std: '배꼽수준허리너비*', eng: 'Waist Breadth(Omphalion)' },
      '120': { part: '높이/너비/두께', std: '엉덩이너비*', eng: 'Hip Width' },
      '121': { part: '높이/너비/두께', std: '겨드랑두께*', eng: 'Armscye Depth' },
      '122': { part: '높이/너비/두께', std: '가슴두께*', eng: 'Chest Depth, standing' },
      '123': { part: '높이/너비/두께', std: '젖가슴두께*', eng: 'Bust Depth' },
      '124': { part: '높이/너비/두께', std: '허리두께*', eng: 'Waist Depth(Natural Indentation)' },
      '125': { part: '높이/너비/두께', std: '배꼽수준허리두께*', eng: 'Waist Depth (Omphalion)' },
      '126': { part: '높이/너비/두께', std: '엉덩이두께*', eng: 'Hip Depth' },
      '127': { part: '높이/너비/두께', std: '엉덩이수직길이*', eng: 'Body Rise' },
      '128': { part: '높이/너비/두께', std: '몸통수직길이*', eng: 'Vertical Trunk Length' },
      '129': { part: '높이/너비/두께', std: '벽면앞으로뻗은주먹수평길이*', eng: 'Grip Reach; forward' },
      '130': { part: '높이/너비/두께', std: '벽면몸통두께*', eng: 'Body depth, standing' },
      '131': { part: '높이/너비/두께', std: '벽면어깨수평길이*', eng: 'Wall-Acromion Distance' },
      '132': { part: '높이/너비/두께', std: '몸무게*', eng: 'Weight' },
      '201': { part: '둘레/길이', std: '앞중심길이*', eng: 'Waist Front Length' },
      '202': { part: '둘레/길이', std: '배꼽수준앞중심길이*', eng: 'Waist Front Length (Omphalion)' },
      '203': { part: '둘레/길이', std: '겨드랑앞벽사이길이*', eng: 'Interscye, front' },
      '204': { part: '둘레/길이', std: '겨드랑앞접힘사이길이*', eng: 'Interscye Fold, front' },
      '205': { part: '둘레/길이', std: '젖꼭지사이수평길이*', eng: 'Bust Point-Bust Point' },
      '206': { part: '둘레/길이', std: '목둘레*', eng: 'Neck Circumference' },
      '207': { part: '둘레/길이', std: '목밑둘레*', eng: 'Neck Base Circumference' },
      '208': { part: '둘레/길이', std: '가슴둘레*', eng: 'Chest Circumference' },
      '209': { part: '둘레/길이', std: '젖가슴둘레*', eng: 'Bust Circumference' },
      '210': { part: '둘레/길이', std: '젖가슴아래둘레*', eng: 'Underbust Circumference' },
      '211': { part: '둘레/길이', std: '허리둘레*', eng: 'Waist Circumference(Natural Indentation)' },
      '212': { part: '둘레/길이', std: '배꼽수준허리둘레*', eng: 'Waist Circumference (Omphalion)' },
      '213': { part: '둘레/길이', std: '엉덩이둘레*', eng: 'Hip Circumference' },
      '214': { part: '둘레/길이', std: '넙다리둘레*', eng: 'Thigh Circumference' },
      '215': { part: '둘레/길이', std: '넙다리중간둘레*', eng: 'Midthigh Circumference' },
      '216': { part: '둘레/길이', std: '무릎둘레*', eng: 'Knee Circumference' },
      '217': { part: '둘레/길이', std: '무릎아래둘레*', eng: 'Lower Knee Circumference' },
      '218': { part: '둘레/길이', std: '장딴지둘레*', eng: 'Calf Circumference' },
      '219': { part: '둘레/길이', std: '종아리최소둘레*', eng: 'Minimum Leg Circumference' },
      '220': { part: '둘레/길이', std: '발목최대둘레*', eng: 'Ankle Circumference' },
      '221': { part: '둘레/길이', std: '어깨길이*', eng: 'Shoulder Length' },
      '222': { part: '둘레/길이', std: '목뒤등뼈위겨드랑수준길이*', eng: 'Scye depth' },
      '223': { part: '둘레/길이', std: '등길이*', eng: 'Waist Back Length(Natural Indentation)' },
      '224': { part: '둘레/길이', std: '배꼽수준등길이*', eng: 'Waist Back Length (Omphalion)' },
      '225': { part: '둘레/길이', std: '목뒤오금길이*', eng: 'Cervicale to Popliteal Length' },
      '226': { part: '둘레/길이', std: '총길이*', eng: 'Total length' },
      '227': { part: '둘레/길이', std: '넙다리직선길이*', eng: 'Thigh Vertical Length' },
      '228': { part: '둘레/길이', std: '어깨사이길이*', eng: 'Biacromion Length' },
      '229': { part: '둘레/길이', std: '어깨가쪽사이길이*', eng: 'Bishoulder Length' },
      '230': { part: '둘레/길이', std: '겨드랑뒤벽사이길이*', eng: 'Back Interscye, Length' },
      '231': { part: '둘레/길이', std: '겨드랑뒤벽접힘사이길이*', eng: 'Back Interscye Fold, Length' },
      '232': { part: '둘레/길이', std: '목뒤젖꼭지길이*', eng: 'Cervical to Breast Point Length' },
      '233': { part: '둘레/길이', std: '목뒤젖꼭지허리둘레선길이*', eng: 'Cervical to Waist Length' },
      '234': { part: '둘레/길이', std: '목옆젖꼭지길이*', eng: 'Neck Point to Breast Point' },
      '235': { part: '둘레/길이', std: '목옆젖꼭지허리둘레선길이*', eng: 'Neck Point to Breast Point to Waistline' },
      '236': { part: '둘레/길이', std: '위팔길이*', eng: 'Upperarm Length' },
      '237': { part: '둘레/길이', std: '팔길이*', eng: 'Arm Length' },
      '238': { part: '둘레/길이', std: '팔안쪽길이*', eng: 'Underarm Length' },
      '239': { part: '둘레/길이', std: '목뒤손목안쪽길이*', eng: 'Cervicale to Wrist Length' },
      '240': { part: '둘레/길이', std: '겨드랑둘레*', eng: 'Armscye Circumference' },
      '241': { part: '둘레/길이', std: '위팔둘레*', eng: 'Upper Arm Circumference' },
      '242': { part: '둘레/길이', std: '팔꿈치둘레*', eng: 'Elbow Circumference' },
      '243': { part: '둘레/길이', std: '손목둘레*', eng: 'Wrist Circumference' },
      '244': { part: '둘레/길이', std: '엉덩이옆길이*', eng: 'Waist to Hip Length' },
      '245': { part: '둘레/길이', std: '다리가쪽길이*', eng: 'Outside Leg Length' },
      '246': { part: '둘레/길이', std: '몸통세로둘레*', eng: 'Trunk Circumference' },
      '247': { part: '둘레/길이', std: '샅앞뒤길이*', eng: 'Crotch Length(Natural Indentation)' },
      '248': { part: '둘레/길이', std: '배꼽수준샅앞뒤길이*', eng: 'Crotch Length (Omphalion)' },
      '301': { part: '앉은자세', std: '앉은키*', eng: 'Sitting Height' },
      '302': { part: '앉은자세', std: '앉은눈높이*', eng: 'Eye Height, sitting' },
      '303': { part: '앉은자세', std: '앉은목뒤높이*', eng: 'Cervicale Height, sitting' },
      '304': { part: '앉은자세', std: '앉은어깨높이*', eng: 'Shoulder Height, sitting' },
      '305': { part: '앉은자세', std: '앉은팔꿈치높이*', eng: 'Elbow Height, sitting' },
      '306': { part: '앉은자세', std: '앉은넙다리높이*', eng: 'Thigh Clearance' },
      '307': { part: '앉은자세', std: '앉은무릎높이*', eng: 'Knee Height, sitting' },
      '308': { part: '앉은자세', std: '앉은오금높이*', eng: 'Popliteal Height' },
      '309': { part: '앉은자세', std: '앉은엉덩이무릎수평길이*', eng: 'Buttock-Knee Length' },
      '310': { part: '앉은자세', std: '앉은엉덩이오금수평길이*', eng: 'Buttock-Popliteal Length' },
      '311': { part: '앉은자세', std: '앉은엉덩이배두께*', eng: 'Buttock-Abdomen Depth, sitting' },
      '312': { part: '앉은자세', std: '아래팔수평길이*', eng: 'Elbow-Wrist Length' },
      '313': { part: '앉은자세', std: '위팔수직길이*', eng: 'Shoulder-Elbow Length' },
      '314': { part: '앉은자세', std: '팔꿈치손끝수평길이*', eng: 'Forearm-Fingertip Length' },
      '315': { part: '앉은자세', std: '팔꿈치주먹수평길이*', eng: 'Elbow-Grip Length' },
      '316': { part: '앉은자세', std: '앉은배두께*', eng: 'Abdominal Depth, sitting' },
      '317': { part: '앉은자세', std: '어깨너비*', eng: 'Biacromial Breadth' },
      '318': { part: '앉은자세', std: '위팔사이너비*', eng: 'Bideltoid Breadth' },
      '319': { part: '앉은자세', std: '팔꿈치사이너비*', eng: 'Elbow to Elbow Breadth' },
      '320': { part: '앉은자세', std: '앉은엉덩이너비*', eng: 'Hip Breadth, sitting' },
      '401': { part: '머리/손/발', std: '머리두께*', eng: 'Head Length' },
      '402': { part: '머리/손/발', std: '머리너비*', eng: 'Head Breadth' },
      '403': { part: '머리/손/발', std: '머리수직길이*', eng: 'Head Height' },
      '404': { part: '머리/손/발', std: '얼굴수직길이*', eng: 'Face Length' },
      '405': { part: '머리/손/발', std: '머리둘레*', eng: 'Head Circumference' },
      '406': { part: '머리/손/발', std: '귀구슬사이머리위길이*', eng: 'Bitragion Arc' },
      '407': { part: '머리/손/발', std: '눈살뒤통수길이*', eng: 'Sagital Arc' },
      '408': { part: '머리/손/발', std: '눈동자사이너비*', eng: 'Interpupilary Breadth' },
      '409': { part: '머리/손/발', std: '손직선길이*', eng: 'Hand Length' },
      '410': { part: '머리/손/발', std: '손바닥직선길이*', eng: 'Palm Length Perpendicular' },
      '411': { part: '머리/손/발', std: '손너비*', eng: 'Hand Breadth at Metacarpals' },
      '412': { part: '머리/손/발', std: '둘째손가락직선길이*', eng: 'Index Finger Length' },
      '413': { part: '머리/손/발', std: '둘째손가락첫째관절너비*', eng: 'Index Finger Breadth,Proximal' },
      '414': { part: '머리/손/발', std: '둘째손가락둘째관절너비*', eng: 'Index Finger Breadth, Distal' },
      '415': { part: '머리/손/발', std: '손두께*', eng: 'Hand Depth' },
      '416': { part: '머리/손/발', std: '막대쥔손안둘레*', eng: 'Inner Grip Circumference' },
      '417': { part: '머리/손/발', std: '손둘레*', eng: 'Hand Circumference' },
      '418': { part: '머리/손/발', std: '발너비*', eng: 'Foot Breadth' },
      '419': { part: '머리/손/발', std: '발직선길이*', eng: 'Foot Length' },
      '420': { part: '머리/손/발', std: '가쪽복사높이*', eng: 'Lateral Malleolus Height' }
    }
  };
};
