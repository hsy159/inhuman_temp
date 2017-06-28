#pragma once

#include <string.h> // memset


/*
// https://en.wikipedia.org/wiki/STL_(file_format)
// Binary STL
UINT8[80] - Header
UINT32 - Number of triangles

foreach triangle
REAL32[3] - Normal vector
REAL32[3] - Vertex 1
REAL32[3] - Vertex 2
REAL32[3] - Vertex 3
UINT16 - Attribute byte count
end
*/

// 20170303 sdk struct 에서 short 사이즈 만큼만 설정하기 위해서
#pragma pack(push, 2)
typedef struct _BinarySTLtriangle
{
	float Normal[3];
	float Vertex1[3];
	float Vertex2[3];
	float Vertex3[3];
	short Attribute;
} BinarySTLtriangle;
#pragma pack(pop)

typedef struct _VERTEX
{
	float x;
	float y;
	float z;

	_VERTEX()
	{
		x = 0.f;
		y = 0.f;
		z = 0.f;
	};
	_VERTEX(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	};
	_VERTEX(float src[3])
	{
		x = src[0];
		y = src[1];
		z = src[2];
	};
} VERTEX;

typedef struct _FACE
{
	int List[3];
	VERTEX faceNormal;

	_FACE()
	{
		List[0] = 0;
		List[1] = 0;
		List[2] = 0;
	};
} FACE;

typedef struct _ScanData
{
	wchar_t	ScanID[20];
	wchar_t	ScanDate[20];
	wchar_t	FileName[30];

	float Stature;
	float WeightL;
	float WeightR;
	float Volume;
	float SA;
	float Breast;
	float BreastY;
	float Waist;
	float Hip;

	VERTEX Crouch;
	VERTEX AxillaL;
	VERTEX AxillaR;

	_ScanData()
	{
		memset(ScanID, 0x00, sizeof(wchar_t) * 20);
		memset(ScanDate, 0x00, sizeof(wchar_t) * 20);
		memset(FileName, 0x00, sizeof(wchar_t) * 30);

		Stature = WeightL = WeightR = Volume = SA = Breast = BreastY = Waist = Hip = 0.f;
	}
} ScanData;

#define ZeroDivisor(x) (0 == x ? 1 : x)


/*
1장 리포트 내용
거북목 : distap - distbp
골반불균형 : distcp 차이
*/

/*
2017-02-15
http://sizekorea.kats.go.kr/04_terminology/measurementWord4.asp
인체측정 표준화 용어 - 기준점 용어 - 표 4: 기준점 용어 비교

번호 부위 표준화 연구 용어 영어 구용어
-1 머리부위 머리마루점* Vertex 두정점
2 머리부위 머리옆점 -오른쪽과 왼쪽* Euryon -right and left 측두점
3 머리부위 눈살점* Glabella 미간점
4 머리부위 눈초리점* Ectocanthus
5 머리부위 눈확아래점* Inferior Orbitale(Infraorbitale) 안와점
6 머리부위 귀구슬점 -오른쪽과 왼쪽* Tragion -right and left 이주점
7 머리부위 코뿌리점* Sellion(Nasion)
8 머리부위 턱끝점* Menton 하악점
9 머리부위 뒤통수돌출점* Occiput 후두점, 뒤통수점
10 머리부위 뒤통수점* Inion
-11 목부위 목뒤점 Cervicale 경추점, 목뒷점
-12 목부위 목앞점 Anterior Neck 전경점, 경와점
-13 목부위 목옆점 -오른쪽과 왼쪽 Lateral Neck -right and left 경측점
-14 목부위 방패연골아래점* Inferior Thyroid(Infrathyroid) 갑상선연골밑점
15 몸통부위 몸통 어깨점 -오른쪽과 왼쪽 Acromion -right and left 견봉점
-16 몸통부위 어깨가쪽점 -오른쪽과 왼쪽 Lateral Shoulder -right and left 견선점, 어깨끝점
-17 몸통부위 어깨가운데점 Midshoulder 어깨중점
-18 몸통부위 겨드랑점* Axilla 액와점
19 몸통부위 겨드랑앞점 -오른쪽과 왼쪽 Anterior Axilla -right and left 전액와점,앞겨드랑점
20 몸통부위 겨드랑뒤점 -오른쪽과 왼쪽 Posterior Axilla -right and left 후액와점,뒷겨드랑점
21 몸통부위 겨드랑앞벽점 -오른쪽과 왼쪽 Anterior Midaxilla -right and left 앞품점
22 몸통부위 겨드랑뒤벽점 -오른쪽과 왼쪽 Posterior Midaxilla -right and left 뒤품점
23 몸통부위 등뼈위겨드랑수준점 Axillary Level at Midspine 진동깊이점
-24 몸통부위 겨드랑앞접힘점 -오른쪽과 왼쪽* Anterior Axillary Fold -right and left
-25 몸통부위 겨드랑뒤접힘점 -오른쪽과 왼쪽* Posterior Axillary Fold -right and left
26 몸통부위 복장뼈가운데점 Mesosternal 흉골중점
-27 몸통부위 젖꼭지점 -오른쪽과 왼쪽 Nipple -right and left 유두점
-28 몸통부위 젖가슴아래점* Inferior Breast
29 몸통부위 열째갈비뼈점* Tenth Rib
30 몸통부위 엉덩뼈능선점* liocristale 장능점,장골능선점
-31 몸통부위 허리옆점<자연스럽게 들어간 자리>-오른쪽과 왼쪽 Lateral Waist -right
-32 몸통부위 허리앞점 Anterior Waist
-33 몸통부위 허리뒤점 Posterior Waist
-34 몸통부위 배꼽점* Anterior Waist 제점
-35 몸통부위 배꼽수준허리옆점-오른쪽과 왼쪽 Lateral Waist-right and left
-36 몸통부위 배꼽수준허리뒤점 Posterior Waist
37 몸통부위 위앞엉덩뼈가시점 Anterior Superior Iliac Spine 장극점, 장골극점
-38 몸통부위 엉덩이돌출점 Buttock Protrusion
-39 몸통부위 볼기고랑점 Gluteal Fold 둔구점
-40 몸통부위 샅점* Crotch 회음점
-41 몸통부위 앉은배돌출점* Abdominal Protrusion, Sitting
42 다리부위 다리 정강뼈위점 Tibiale 경골점,무릎마디안쪽점
43 다리부위 무릎뼈위점 Superior Patella(Suprapatella)
-44 다리부위 무릎뼈가운데점 Midpatella 슬개골중점,무릎점
45 다리부위 무릎뼈아래점 Inferior Patella(Infrapatella)
46 다리부위 넙다리가운데점 Midthigh
47 다리부위 오금점* Posterior Juncture of Calf and Thigh
-48 다리부위 장딴지돌출점* Calf Protrusion 하퇴돌출점
-49 다리부위 종아리아래점* Inferior Leg
-50 다리부위 안쪽복사점* Medial Malleous 내과점
-51 다리부위 가쪽복사점 Lateral Malleous 외과점,바깥복사점
52 다리부위 앉은넙다리위점* Superior Thigh, Sitting
53 다리부위 앉은무릎앞점* Anterior Knee, Sitting
54 다리부위 앉은오금점* Posterior Juncture of Calf and Thigh -Sitt
55 발부위 발안쪽점 Metatarsophalangeal Ⅰ 발안쪽옆점
56 발부위 발가쪽점 Metatarsophalangeal Ⅴ 발바깥옆점
57 발부위 발끝점* Acropodion 족선점
58 발부위 발꿈치점* Pternion 종점, 발뒤꿈치점
59 팔부위 팔 어깨세모근점 -오른쪽과 왼쪽* Deltoid Point -right and left
60 팔부위 위팔두갈래근점* Biceps
-61 팔부위 노뼈위점 Radiale
-62 팔부위 팔꿈치가운데점 Center Olecranon 팔꿈치점
63 팔부위 팔꿈치뒤점* Rear Olecranon
64 팔부위 팔꿈치아래점* Bottom Olecranon
65 팔부위 팔꿈치가쪽점* Lateral Humeral Epicondyle
-66 팔부위 손목안쪽점 Ulnar Styloid 척골경상돌기점, 손목점
-67 팔부위 손목가쪽점 Radial Styloid 요골경상돌기점
68 손부위 손안쪽점 Metacarpale Ⅴ 손안쪽옆점
69 손부위 손가쪽점 Metacarpale Ⅱ 손바깥옆점
70 손부위 손끝점* Dactylion Ⅲ 지선점

6차 인체치수조사사업 기준점 대응 31개
-1 머리부위 머리마루점* Vertex 두정점
-11 목부위 목뒤점 Cervicale 경추점, 목뒷점
-12 목부위 목앞점 Anterior Neck 전경점, 경와점
-13 목부위 목옆점 -오른쪽과 왼쪽 Lateral Neck -right and left 경측점
-14 목부위 방패연골아래점* Inferior Thyroid(Infrathyroid) 갑상선연골밑점
-16 몸통부위 어깨가쪽점 -오른쪽과 왼쪽 Lateral Shoulder -right and left 견선점, 어깨끝점
-17 몸통부위 어깨가운데점 Midshoulder 어깨중점
-18 몸통부위 겨드랑점* Axilla 액와점
-24 몸통부위 겨드랑앞접힘점 -오른쪽과 왼쪽* Anterior Axillary Fold -right and left
-25 몸통부위 겨드랑뒤접힘점 -오른쪽과 왼쪽* Posterior Axillary Fold -right and left
-27 몸통부위 젖꼭지점 -오른쪽과 왼쪽 Nipple -right and left 유두점
-28 몸통부위 젖가슴아래점* Inferior Breast
-31 몸통부위 허리옆점<자연스럽게 들어간 자리>-오른쪽과 왼쪽 Lateral Waist -right
-32 몸통부위 허리앞점 Anterior Waist
-33 몸통부위 허리뒤점 Posterior Waist
-34 몸통부위 배꼽점* Anterior Waist 제점
-35 몸통부위 배꼽수준허리옆점-오른쪽과 왼쪽 Lateral Waist-right and left
-36 몸통부위 배꼽수준허리뒤점 Posterior Waist
-38 몸통부위 엉덩이돌출점 Buttock Protrusion
-39 몸통부위 볼기고랑점 Gluteal Fold 둔구점
-40 몸통부위 샅점* Crotch 회음점
-41 몸통부위 앉은배돌출점* Abdominal Protrusion, Sitting
-44 다리부위 무릎뼈가운데점 Midpatella 슬개골중점,무릎점
-48 다리부위 장딴지돌출점* Calf Protrusion 하퇴돌출점
-49 다리부위 종아리아래점* Inferior Leg
-50 다리부위 안쪽복사점* Medial Malleous 내과점
-51 다리부위 가쪽복사점 Lateral Malleous 외과점,바깥복사점
-61 팔부위 노뼈위점 Radiale
-62 팔부위 팔꿈치가운데점 Center Olecranon 팔꿈치점
-66 팔부위 손목안쪽점 Ulnar Styloid 척골경상돌기점, 손목점
-67 팔부위 손목가쪽점 Radial Styloid 요골경상돌기점
*/

/*
http://sizekorea.kats.go.kr/04_terminology/measurementWord5.asp
인체측정 표준화 용어 - 측정항목 용어 - 표 5: 측정항목 용어 비교

번호 부위 표준화 연구 용어 영어 구용어
101 높이/너비/두께 머리위로뻗은주먹높이* Overhead Fist Reach 머리위로뻗은손주먹끝높이
-102 높이/너비/두께 키* Stature 신장
103 높이/너비/두께 눈높이* Eye Height
-104 높이/너비/두께 목뒤높이* Cervical Height 경추고
105 높이/너비/두께 어깨높이* Acromion Height 견봉고
-106 높이/너비/두께 어깨가쪽높이* Shoulder Height
-107 높이/너비/두께 겨드랑높이* Axilla Height 겨드랑이점높이, 액와고
108 높이/너비/두께 굽힌팔꿈치높이* Elbow Height
-109 높이/너비/두께 엉덩이높이* Hip Height 둔부높이
110 높이/너비/두께 샅높이* Crotch Height 회음높이
111 높이/너비/두께 주먹높이* Fist Height
-112 높이/너비/두께 허리높이* Waist Height
-113 높이/너비/두께 배꼽수준허리높이* Waist Height (Omphalion)
114 높이/너비/두께 위앞엉덩뼈가시높이* Iliac Spine Height 장골극높이
115 높이/너비/두께 무릎높이* Knee Height 무릎마디안쪽높이, 경골높
-116 높이/너비/두께 가슴너비* Chest Breadth 윗가슴너비
-117 높이/너비/두께 젖가슴너비* Bust Breadth 가슴너비
-118 높이/너비/두께 허리너비* Waist Breadth I(Natural Indentation)
-119 높이/너비/두께 배꼽수준허리너비* Waist Breadth(Omphalion)
-120 높이/너비/두께 엉덩이너비* Hip Width
-121 높이/너비/두께 겨드랑두께* Armscye Depth 진동두께
-122 높이/너비/두께 가슴두께* Chest Depth, standing
-123 높이/너비/두께 젖가슴두께* Bust Depth 가슴두께
-124 높이/너비/두께 허리두께* Waist Depth(Natural Indentation)
-125 높이/너비/두께 배꼽수준허리두께* Waist Depth (Omphalion)
-126 높이/너비/두께 엉덩이두께* Hip Depth
-127 높이/너비/두께 엉덩이수직길이* Body Rise
-128 높이/너비/두께 몸통수직길이* Vertical Trunk Length
129 높이/너비/두께 벽면앞으로뻗은주먹수평길이* Grip Reach; forward
130 높이/너비/두께 벽면몸통두께* Body depth, standing
131 높이/너비/두께 벽면어깨수평길이* Wall-Acromion Distance
132 높이/너비/두께 몸무게* Weight 체중
-201 둘레/길이 앞중심길이* Waist Front Length
-202 둘레/길이 배꼽수준앞중심길이* Waist Front Length (Omphalion)
203 둘레/길이 겨드랑앞벽사이길이* Interscye, front 앞품
-204 둘레/길이 겨드랑앞접힘사이길이* Interscye Fold, front
-205 둘레/길이 젖꼭지사이수평길이* Bust Point-Bust Point 젖꼭지간격, 유폭
-206 둘레/길이 목둘레* Neck Circumference
-207 둘레/길이 목밑둘레* Neck Base Circumference
-208 둘레/길이 가슴둘레* Chest Circumference 윗가슴둘레
-209 둘레/길이 젖가슴둘레* Bust Circumference 가슴둘레, 흉위
-210 둘레/길이 젖가슴아래둘레* Underbust Circumference 밑가슴둘레
-211 둘레/길이 허리둘레* Waist Circumference(Natural Indentation) 동위
-212 둘레/길이 배꼽수준허리둘레* Waist Circumference (Omphalion)
-213 둘레/길이 엉덩이둘레* Hip Circumference 둔위. 요위
-214 둘레/길이 넙다리둘레* Thigh Circumference 넓적다리둘레, 대퇴둘레
215 둘레/길이 넙다리중간둘레* Midthigh Circumference
-216 둘레/길이 무릎둘레* Knee Circumference
217 둘레/길이 무릎아래둘레* Lower Knee Circumference
-218 둘레/길이 장딴지둘레* Calf Circumference 하퇴최대둘레
-219 둘레/길이 종아리최소둘레* Minimum Leg Circumference
-220 둘레/길이 발목최대둘레* Ankle Circumference
-221 둘레/길이 어깨길이* Shoulder Length
-222 둘레/길이 목뒤등뼈위겨드랑수준길이* Scye depth 진동길이, 진동깊이
-223 둘레/길이 등길이* Waist Back Length(Natural Indentation)
-224 둘레/길이 배꼽수준등길이* Waist Back Length (Omphalion)
225 둘레/길이 목뒤오금길이* Cervicale to Popliteal Length
226 둘레/길이 총길이* Total length
227 둘레/길이 넙다리직선길이* Thigh Vertical Length
228 둘레/길이 어깨사이길이* Biacromion Length
-229 둘레/길이 어깨가쪽사이길이* Bishoulder Length 어깨끝점사이길이
230 둘레/길이 겨드랑뒤벽사이길이* Back Interscye, Length 뒤품
-231 둘레/길이 겨드랑뒤벽접힘사이길이* Back Interscye Fold, Length
-232 둘레/길이 목뒤젖꼭지길이* Cervical to Breast Point Length
-233 둘레/길이 목뒤젖꼭지허리둘레선길이* Cervical to Waist Length
-234 둘레/길이 목옆젖꼭지길이* Neck Point to Breast Point 목옆점-젖꼭지점길이, 유
-235 둘레/길이 목옆젖꼭지허리둘레선길이* Neck Point to Breast Point to Waistline 목옆점-젖꼭지점-허리둘레
236 둘레/길이 위팔길이* Upperarm Length
-237 둘레/길이 팔길이* Arm Length 소매길이
238 둘레/길이 팔안쪽길이* Underarm Length 안쪽팔길이, 안소매길이
239 둘레/길이 목뒤손목안쪽길이* Cervicale to Wrist Length 목뒤점-어깨끝점-손목안쪽
240 둘레/길이 겨드랑둘레* Armscye Circumference 진동둘레
241 둘레/길이 위팔둘레* Upper Arm Circumference 윗팔둘레, 상완둘레
242 둘레/길이 팔꿈치둘레* Elbow Circumference
243 둘레/길이 손목둘레* Wrist Circumference
-244 둘레/길이 엉덩이옆길이* Waist to Hip Length 허리옆점-엉덩이둘레선길이밑위길이
245 둘레/길이 다리가쪽길이* Outside Leg Length
246 둘레/길이 몸통세로둘레* Trunk Circumference
247 둘레/길이 샅앞뒤길이* Crotch Length(Natural Indentation) 밑위앞뒤길이
248 둘레/길이 배꼽수준샅앞뒤길이* Crotch Length (Omphalion)
301 앉은자세 앉은키* Sitting Height 좌고
302 앉은자세 앉은눈높이* Eye Height, sitting 앉은 경추고
303 앉은자세 앉은목뒤높이* Cervicale Height, sitting
304 앉은자세 앉은어깨높이* Shoulder Height, sitting
305 앉은자세 앉은팔꿈치높이* Elbow Height, sitting
306 앉은자세 앉은넙다리높이* Thigh Clearance 앉은넓적다리두께
307 앉은자세 앉은무릎높이* Knee Height, sitting
308 앉은자세 앉은오금높이* Popliteal Height
309 앉은자세 앉은엉덩이무릎수평길이* Buttock-Knee Length 앉은엉덩이무릎길이
310 앉은자세 앉은엉덩이오금수평길이* Buttock-Popliteal Length 앉은엉덩이오금길이
311 앉은자세 앉은엉덩이배두께* Buttock-Abdomen Depth,sitting
312 앉은자세 아래팔수평길이* Elbow-Wrist Length
313 앉은자세 위팔수직길이* Shoulder-Elbow Length
314 앉은자세 팔꿈치손끝수평길이* Forearm-Fingertip Length
315 앉은자세 팔꿈치주먹수평길이* Elbow-Grip Length
316 앉은자세 앉은배두께* Abdominal Depth, sitting
317 앉은자세 어깨너비* Biacromial Breadth 견봉너비
318 앉은자세 위팔사이너비* Bideltoid Breadth 몸통너비
319 앉은자세 팔꿈치사이너비* Elbow to Elbow Breadth
320 앉은자세 앉은엉덩이너비* Hip Breadth, sitting
401 머리/손/발 머리두께* Head Length 두장
402 머리/손/발 머리너비* Head Breadth 두폭
403 머리/손/발 머리수직길이* Head Height 머리길이, 전두고
404 머리/손/발 얼굴수직길이* Face Length
405 머리/손/발 머리둘레* Head Circumference 두위
406 머리/손/발 귀구슬사이머리위길이* Bitragion Arc 구구슬사이머리마루점길이
407 머리/손/발 눈살뒤통수길이* Sagital Arc
408 머리/손/발 눈동자사이너비* Interpupilary Breadth
409 머리/손/발 손직선길이* Hand Length 손길이
410 머리/손/발 손바닥직선길이* Palm Length Perpendicular 손바닥길이
411 머리/손/발 손너비* Hand Breadth at Metacarpals
412 머리/손/발 둘째손가락직선길이* Index Finger Length
413 머리/손/발 둘째손가락첫째관절너비* Index Finger Breadth,Proximal
414 머리/손/발 둘째손가락둘째관절너비* Index Finger Breadth, Distal
415 머리/손/발 손두께* Hand Depth
416 머리/손/발 막대쥔손안둘레* Inner Grip Circumference
417 머리/손/발 손둘레* Hand Circumference
418 머리/손/발 발너비* Foot Breadth
419 머리/손/발 발직선길이* Foot Length 발길이
-420 머리/손/발 가쪽복사높이* Lateral Malleolus Height 바깥복사점높이, 외과고
*/
