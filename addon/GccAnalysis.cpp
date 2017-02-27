#include "GccAnalysis.h"

#include "GccGeometry.h"

//#include <stdio.h> // printf
#include <string.h> // memcpy
#include <algorithm> // std::max

//using namespace std;

// json 만들기
const std::string JSONOpen = "{";
const std::string JSONClose = "}";
const std::string JSONColon = ":";
const std::string JSONComma = ",";
const std::string JSONAryOpen = "[";
const std::string JSONAryClose = "]";

CGccAnalysis::CGccAnalysis()
	: m_VertexList(nullptr)
	, m_VertexLength(0)
	, m_FaceList(nullptr)
	, m_FaceLength(0)
	, m_ClickPoints(nullptr)
	, m_ClickPointsCount(0)
{
	int i = 0;

	for (i = 0; i < SIZE_VNS; i++)
	{
		m_VnSData[i] = 0.f;
	}

	for (i = 0; i < SIZE_LATERAL; i++)
	{
		m_LateralData[i] = 0.f;
	}

	for (i = 0; i < SIZE_HIPKNEE; i++)
	{
		m_HipKneeData[i] = 0.f;
	}

	for (i = 0; i < SIZE_SPINE; i++)
	{
		m_CurSpineData[i] = 0.f;
		m_PreSpineData[i] = 0.f;
	}

	for (i = 0; i < SIZE_SHAPE; i++)
	{
		m_ShapeData[i] = 0.f;
	}
}


CGccAnalysis::~CGccAnalysis()
{
	InitClickPoints();
}


void CGccAnalysis::InitClickPoints(int nCnt)
{
	if (m_ClickPoints)
	{
		delete[] m_ClickPoints;
		m_ClickPoints = nullptr;
	}

	m_ClickPointsCount = nCnt;

	m_ClickPoints = new VERTEX[m_ClickPointsCount];
	memset(m_ClickPoints, 0x00, sizeof(VERTEX) * m_ClickPointsCount);
}

const std::string CGccAnalysis::CalcVnSData()
{
	CGccGeometry geo;
	float volume[6][2], surface[6][2] = { 0.f };

	// test code
	InitClickPoints(8);
	// test code

	/*
	input : Img/VnS_F.png, VnS_M.png
	m_ClickPoints 8개
	1 - 11 목부위 목뒤점 Cervicale >>>> 목뒤점
	2 - 18 몸통부위 겨드랑점* Axilla >>>> 겨드랑점R
	3 - 18 몸통부위 겨드랑점* Axilla >>>> 겨드랑점L
	4 - 28 몸통부위 젖가슴아래점* Inferior Breast >>>> 젖가슴아래점(UnderBust) >>>> 남자 값 필요!!!
	5 - 34 몸통부위 배꼽점* Anterior Waist >>>> 배꼽점(Navel)
	6 - 40 몸통부위 샅점* Crotch >>>> 샅점(Crotch)
	7 - 44 다리부위 무릎뼈가운데점 Midpatella >>>> 무릎뼈가운데점(Knee) R 필요!!!
	8 - 44 다리부위 무릎뼈가운데점 Midpatella >>>> 무릎뼈가운데점(Knee) L 필요!!!
	*/
	geo.CalcPartsVolumeNSurface(m_VertexList, m_VertexLength, m_FaceList, m_FaceLength, m_ClickPoints, volume, surface);

	for (int i = 0; i < 6; i++)
	{
		m_VnSData[i] = volume[i][0] / 1000000.f;
		m_VnSData[i + 6] = volume[i][1] / 1000000.f;
		m_VnSData[i + 12] = surface[i][0] / 1000000.f;
		m_VnSData[i + 18] = surface[i][1] / 1000000.f;
	}

	//m_ReportView.m_VnS.SetMainData(m_VnSData);

	/*
	chest 가슴, 흉부
	abdomen 배, 복부
	pelvis 골반
	femur 대퇴골
	tibia 정강이뼈, 경골
	upper extremity 팔, 상지

	volume 용량, 용적
	surface 표면
	*/

	const int nCNT = 6;
	const int nCNTSUB = 2;
	std::string strKeys[nCNT] = { "\"chest\"", "\"abdomen\"", "\"pelvis\"", "\"femur\"", "\"tibia\"", "\"upper_extremity\"" };
	std::string strKeysSub1[nCNTSUB] = { "\"volume\"", "\"surface\"" };
	std::string strKeysSub2[nCNTSUB] = { "\"R\"", "\"L\"" };

	/*
	output : 24개
	{
		"chest":   {"volume": {"R":123, "L": 234}, "surface" : {"R":123, "L": 234}},
		"abdomen": {"volume": {"R":123, "L": 234}, "surface" : {"R":123, "L": 234}},
		"pelvis":  {"volume": {"R":123, "L": 234}, "surface" : {"R":123, "L": 234}},
		"femur":   {"volume": {"R":123, "L": 234}, "surface" : {"R":123, "L": 234}},
		"tibia":   {"volume": {"R":123, "L": 234}, "surface" : {"R":123, "L": 234}},
		"upper extremity": {"volume": {"R":123, "L": 234}, "surface": {"R":123, "L": 234}}
	}
	*/

	int i, j, k = 0;
	std::string strJson = JSONOpen;

	for (i = 0; i < nCNT; i++)
	{
		strJson += strKeys[i] + JSONColon + JSONOpen;

		for (j = 0; j < nCNTSUB; j++)
		{
			strJson += strKeysSub1[j] + JSONColon + JSONOpen;

			for (k = 0; k < nCNTSUB; k++)
			{
				strJson += strKeysSub2[k] + JSONColon + std::to_string(m_VnSData[(i * (nCNTSUB + nCNTSUB)) + (j * nCNTSUB) + k]);

				if (k < nCNTSUB - 1)
				{
					strJson += JSONComma;
				}
			}

			strJson += JSONClose;

			if (j < nCNTSUB - 1)
			{
				strJson += JSONComma;
			}
		}

		strJson += JSONClose;

		if (i < nCNT - 1)
		{
			strJson += JSONComma;
		}
	}

	strJson += JSONClose;

	return strJson;
}

const std::string CGccAnalysis::CalcLateralData()
{
	//
	VERTEX Temp[10];

	// test code
	InitClickPoints(10);
	// test code

	for (int i = 0; i < 5; i++)
	{
		Temp[i * 2] = m_ClickPoints[i];
		Temp[i * 2 + 1] = m_ClickPoints[i + 5];
	}

	memcpy(m_ClickPoints, Temp, sizeof(VERTEX) * 10);

	/*
	input : Img/Lateral_F.png, Lateral_M.png
	m_ClickPoints 10개
	변환 됨 : RRRRRLLLLL to RLRLRLRLRL
	1 - 귀R 필요!!!
	2 - 귀L 필요!!!
	3 - 16 몸통부위 어깨가쪽점 -오른쪽과 왼쪽 Lateral Shoulder >>>> 어깨가쪽점R(Shoulder-R)
	4 - L
	5 - 엉덩이 돌출부의 옆점 RL 필요!!!
	6 - L
	7 - 무릎 관절점 RL 필요!!!
	8 - L
	9 - 51 다리부위 가쪽복사점 Lateral Malleous >>>> 가쪽복사점(Ankle) RL 필요!!!
	10 - L
	*/

	//
	CGccGeometry geo;
	int count = 0;
	VERTEX V1, V2, P1, P2, P3;

	if (m_ClickPointsCount >= 3)
	{
		count = 0;

		for (int i = 0; i < m_ClickPointsCount - 2; i++)
		{
			P1 = m_ClickPoints[i];
			P1.x = 0;
			P2 = m_ClickPoints[i + 2];
			P2.x = 0;
			P3 = m_ClickPoints[i + 2];
			P3.x = 0;
			P3.y += 1;

			V1 = geo.CalcLineVector(P2, P1);
			V2 = geo.CalcLineVector(P2, P3);

			m_LateralData[count] = geo.CalcVectorInnerProduct(V1, V2) * __RADIAN;
			m_LateralData[count + 8] = fabsf(sqrtf((P1.y - P2.y) * (P1.y - P2.y) + (P1.z - P2.z) * (P1.z - P2.z)));

			count++;
		}
	}

	if (m_ClickPointsCount >= 5)
	{
		count = 16;

		for (int i = 0; i < m_ClickPointsCount - 4; i++)
		{
			P1 = m_ClickPoints[i];
			P1.x = 0;
			P2 = m_ClickPoints[i + 2];
			P2.x = 0;
			P3 = m_ClickPoints[i + 4];
			P3.x = 0;

			V1 = geo.CalcLineVector(P2, P1);
			V2 = geo.CalcLineVector(P2, P3);

			m_LateralData[count] = geo.CalcVectorInnerProduct(V1, V2) * __RADIAN;

			count++;
		}
	}

	count = 22;

	for (int i = 0; i < m_ClickPointsCount; i++, count++)
	{
		m_LateralData[count] = fabsf(m_ClickPoints[i].z);
	}

	//m_ReportView.m_Lateral.SetMainData(m_LateralData);
	//m_ReportView.m_Lateral.SetPoints(m_ClickPoints);

	/*
	output : 32개
	m_LateralData[0~<=7]
	0 - ab와 plumb line의 각 R
	1 - ab와 plumb line의 각 L
	2 - bc와 plumb line의 각 R
	3 - bc와 plumb line의 각 L
	4 - cd와 plumb line의 각 R
	5 - cd와 plumb line의 각 L
	6 - de와 plumb line의 각 R
	7 - de와 plumb line의 각 L
	m_LateralData[8~<=15] (/ 10.f) 나누기 필요
	8 - distance ab R
	9 - distance ab L
	10 - distance bc R
	11 - distance bc L
	12 - distance cd R
	13 - distance cd L
	14 - distance de R
	15 - distance de L
	m_LateralData[16~<=21]
	16 - abc R
	17 - abc L
	18 - bcd R
	19 - bcd L
	20 - cde R
	21 - cde L
	m_LateralData[22~<=31] (/ 10.f) 나누기 필요
	22 - distance a to plumb line R
	23 - distance a to plumb line L
	24 - distance b to plumb line R
	25 - distance b to plumb line L
	26 - distance c to plumb line R
	27 - distance c to plumb line L
	28 - distance d to plumb line R
	29 - distance d to plumb line L
	30 - distance e to plumb line R
	31 - distance e to plumb line L

	{
		"anglABP": {"R": , "L": , "D": },
		"anglBCP": {"R": , "L": , "D": },
		"anglCDP": {"R": , "L": , "D": },
		"anglDEP": {"R": , "L": , "D": },
		"distAB":  {"R": , "L": , "D": },
		"distBC":  {"R": , "L": , "D": },
		"distCD":  {"R": , "L": , "D": },
		"distDE":  {"R": , "L": , "D": },
		"anglABC": {"R": , "L": , "D": },
		"anglBCD": {"R": , "L": , "D": },
		"anglCDE": {"R": , "L": , "D": },
		"distAP":  {"R": , "L": , "D": },
		"distBP":  {"R": , "L": , "D": },
		"distCP":  {"R": , "L": , "D": },
		"distDP":  {"R": , "L": , "D": },
		"distEP":  {"R": , "L": , "D": }
	}
	*/

	const int nCNT = 16;
	const int nCNTSUB = 2; // RL
	std::string strKeys[nCNT] = {
		"\"anglABP\"", "\"anglBCP\"", "\"anglCDP\"", "\"anglDEP\"",
		"\"distAB\"", "\"distBC\"", "\"distCD\"", "\"distDE\"",
		"\"anglABC\"", "\"anglBCD\"", "\"anglCDE\"",
		"\"distAP\"", "\"distBP\"", "\"distCP\"", "\"distDP\"", "\"distEP\""
	};
	//std::string strKeysSub[nCNTSUB + 1] = { "\"R\"", "\"L\"", "\"D\"" };
	std::string strKeysSub[nCNTSUB] = { "\"R\"", "\"L\"" };

	int i, j = 0;

	// / 10.f
	for (i = 8; i < 16; i++)
	{
		m_LateralData[i] *= 0.1f;
	}

	for (i = 22; i < 32; i++)
	{
		m_LateralData[i] *= 0.1f;
	}

	//
	std::string strJson = JSONOpen;

	for (i = 0; i < nCNT; i++)
	{
		strJson += strKeys[i] + JSONColon + JSONOpen;

		for (j = 0; j < nCNTSUB; j++)
		{
			strJson += strKeysSub[j] + JSONColon + std::to_string(m_LateralData[(i * nCNTSUB) + j]);
			
			if (j < nCNTSUB - 1)
			{
				strJson += JSONComma;
			}
		}

		/*
		// 차이
		float fd = fabsf(m_LateralData[j - 2] - m_LateralData[j - 1]);
		strJson += strKeysSub[2] + JSONColon + std::to_string(fd) + JSONClose;
		*/

		strJson += JSONClose;

		if (i < nCNT - 1)
		{
			strJson += JSONComma;
		}
	}

	strJson += JSONClose;

	return strJson;
}

const std::string CGccAnalysis::CalcHipKneeData()
{
	CGccGeometry geo;
	VERTEX* GirthPT = nullptr;
	int GirthLen[4] = { 0 };
	VERTEX vxMin, vxMax;
	VERTEX vxTemp1, vxTemp2;

	// test code
	InitClickPoints(6);
	// test code

	/*
	input : Img/HipKnee_F.png, HipKnee_M.png
	m_ClickPoints 6개
	1 - 51 다리부위 가쪽복사점 Lateral Malleous >>>> 가쪽복사점(Ankle) RL 필요!!!
	2 - 44 다리부위 무릎뼈가운데점 Midpatella >>>> 무릎뼈가운데점(Knee) R 필요!!!
	3 - 44 다리부위 무릎뼈가운데점 Midpatella >>>> 무릎뼈가운데점(Knee) L 필요!!!
	4 - 46 다리부위 넙다리가운데점 Midthigh 필요!!!
	5 - 67 팔부위 손목가쪽점 Radial Styloid >>>> 손목가쪽점-R(Wrist Out)
	6 - 67 팔부위 손목가쪽점 Radial Styloid >>>> L 필요!!!
	*/

	//calc PT
	//calc Ankle Center
	geo.GetOutlinePoints(m_VertexList, m_VertexLength, m_FaceList, m_FaceLength, &GirthPT, GirthLen, m_ClickPoints[0].y);

	if (0 == GirthLen[1])
	{
		int ptLen = GirthLen[0];
		geo.DivideBasedOnPoint(&GirthPT, ptLen, GirthLen, VERTEX(0, 0, 0), 0);

		geo.GetMinMax(GirthPT, GirthLen[0], vxMin, vxMax);
		vxTemp1.x = vxMin.x + fabsf(vxMax.x - vxMin.x) / 2.f;
		vxTemp1.y = vxMin.y + fabsf(vxMax.y - vxMin.y) / 2.f;
		vxTemp1.z = vxMax.z - fabsf(vxMax.z - vxMin.z) / 3.f;

		geo.GetMinMax(GirthPT + GirthLen[0], GirthLen[1] - GirthLen[0], vxMin, vxMax);
		vxTemp2.x = vxMin.x + (vxMax.x - vxMin.x) / 2.f;
		vxTemp2.y = vxMin.y + (vxMax.y - vxMin.y) / 2.f;
		vxTemp2.z = vxMax.z - fabsf(vxMax.z - vxMin.z) / 3.f;

		m_HipKneeData[16] = 0;
	}
	else if (0 == GirthLen[3])
	{
		geo.GetMinMax(GirthPT, GirthLen[0], vxMin, vxMax);
		vxTemp1.x = vxMin.x + fabsf(vxMax.x - vxMin.x) / 2.f;
		vxTemp1.y = vxMin.y + fabsf(vxMax.y - vxMin.y) / 2.f;
		vxTemp1.z = vxMax.z - fabsf(vxMax.z - vxMin.z) / 3.f;

		m_HipKneeData[16] = vxMax.x;

		geo.GetMinMax(GirthPT + GirthLen[0], GirthLen[1] - GirthLen[0], vxMin, vxMax);
		vxTemp2.x = vxMin.x + fabsf(vxMax.x - vxMin.x) / 2.f;
		vxTemp2.y = vxMin.y + fabsf(vxMax.y - vxMin.y) / 2.f;
		vxTemp2.z = vxMax.z - fabsf(vxMax.z - vxMin.z) / 3.f;

		m_HipKneeData[16] -= vxMin.x;
		m_HipKneeData[16] = fabsf(m_HipKneeData[16]);
	}

	m_HipKneePoint[0] = vxTemp1;
	m_HipKneePoint[1] = vxTemp2;

	//calc Knee Center
	VERTEX vxMinX, vxMaxX, vxMinY, vxMaxY, vxMinZ, vxMaxZ;
	geo.GetOutlinePoints(m_VertexList, m_VertexLength, m_FaceList, m_FaceLength, &GirthPT, GirthLen, m_ClickPoints[1].y);

	if (0 == GirthLen[1])
	{
		int ptLen = GirthLen[0];
		geo.DivideBasedOnPoint(&GirthPT, ptLen, GirthLen, VERTEX(0, 0, 0), 0);
	}
	else if (0 == GirthLen[3])
	{
		geo.GetMinMaxPosition(GirthPT, GirthLen[0], vxMinX, vxMaxX, vxMinY, vxMaxY, vxMinZ, vxMaxZ);
		vxTemp1.x = vxMinX.x + fabsf(vxMaxX.x - vxMinX.x) / 2.f;
		vxTemp1.y = m_ClickPoints[1].y;
		vxTemp1.z = vxMinX.z;
	}

	geo.GetOutlinePoints(m_VertexList, m_VertexLength, m_FaceList, m_FaceLength, &GirthPT, GirthLen, m_ClickPoints[2].y);

	if (0 == GirthLen[1])
	{
		int ptLen = GirthLen[0];
		geo.DivideBasedOnPoint(&GirthPT, ptLen, GirthLen, VERTEX(0, 0, 0), 0);
	}
	else if (0 == GirthLen[3])
	{
		geo.GetMinMaxPosition(GirthPT + GirthLen[0], GirthLen[1] - GirthLen[0], vxMinX, vxMaxX, vxMinY, vxMaxY, vxMinZ, vxMaxZ);
		vxTemp2.x = vxMinX.x + fabsf(vxMaxX.x - vxMinX.x) / 2.f;
		vxTemp2.y = m_ClickPoints[2].y;
		vxTemp2.z = vxMaxX.z;
	}

	m_HipKneePoint[2] = vxTemp1;
	m_HipKneePoint[3] = vxTemp2;

	//Calc Thigh Point
	geo.GetOutlinePoints(m_VertexList, m_VertexLength, m_FaceList, m_FaceLength, &GirthPT, GirthLen, m_ClickPoints[3].y);

	if (0 == GirthLen[1])
	{
		int ptLen = GirthLen[0];
		geo.GetMinMax(GirthPT, GirthLen[0], vxMin, vxMax);
		geo.DivideBasedOnPoint(&GirthPT, ptLen, GirthLen, VERTEX((vxMax.x - vxMin.x) / 2.f + vxMin.x, 0, 0), 0);

		geo.GetMinMax(GirthPT, GirthLen[0], vxMin, vxMax);
		geo.GetMinMaxPosition(GirthPT, GirthLen[0], vxMinX, vxMaxX, vxMinY, vxMaxY, vxMinZ, vxMaxZ);
		vxTemp1.x = vxMin.x + fabsf(vxMax.x - vxMin.x) / 2.f;
		vxTemp1.y = vxMin.y + fabsf(vxMax.y - vxMin.y) / 2.f;
		vxTemp1.z = vxMin.z + fabsf(vxMax.z - vxMin.z) / 3.f;

		geo.GetMinMax(GirthPT + GirthLen[0], GirthLen[1] - GirthLen[0], vxMin, vxMax);
		geo.GetMinMaxPosition(GirthPT + GirthLen[0], GirthLen[1] - GirthLen[0], vxMinX, vxMaxX, vxMinY, vxMaxY, vxMinZ, vxMaxZ);
		vxTemp2.x = vxMin.x + fabsf(vxMax.x - vxMin.x) / 2.f;
		vxTemp2.y = vxMin.y + fabsf(vxMax.y - vxMin.y) / 2.f;
		vxTemp2.z = vxMin.z + fabsf(vxMax.z - vxMin.z) / 3.f;
	}
	else if (0 == GirthLen[3])
	{
		geo.GetMinMax(GirthPT, GirthLen[0], vxMin, vxMax);
		geo.GetMinMaxPosition(GirthPT, GirthLen[0], vxMinX, vxMaxX, vxMinY, vxMaxY, vxMinZ, vxMaxZ);
		vxTemp1.x = vxMin.x + fabsf(vxMax.x - vxMin.x) / 2.f;
		vxTemp1.y = vxMin.y + fabsf(vxMax.y - vxMin.y) / 2.f;
		vxTemp1.z = vxMin.z + fabsf(vxMax.z - vxMin.z) / 3.f;

		geo.GetMinMax(GirthPT + GirthLen[0], GirthLen[1] - GirthLen[0], vxMin, vxMax);
		geo.GetMinMaxPosition(GirthPT + GirthLen[0], GirthLen[1] - GirthLen[0], vxMinX, vxMaxX, vxMinY, vxMaxY, vxMinZ, vxMaxZ);
		vxTemp2.x = vxMin.x + fabsf(vxMax.x - vxMin.x) / 2.f;
		vxTemp2.y = vxMin.y + fabsf(vxMax.y - vxMin.y) / 2.f;
		vxTemp2.z = vxMin.z + fabsf(vxMax.z - vxMin.z) / 3.f;
	}

	m_HipKneePoint[4] = vxTemp1;
	m_HipKneePoint[5] = vxTemp2;

	//손
	m_HipKneePoint[6] = m_ClickPoints[4];
	m_HipKneePoint[7] = m_ClickPoints[5];

	//calc DATA
	float temp1[2], temp2[2] = { 0.f };
	// Tibia Vs Mechanical Axis
	float TibiaR, TibiaL = 0.f;

	temp1[0] = m_HipKneePoint[0].x;
	temp1[1] = m_HipKneePoint[0].y;
	temp2[0] = m_HipKneePoint[2].x;
	temp2[1] = m_HipKneePoint[2].y;
	TibiaR = 90 - fabsf(geo.CalcLineAngle2D(temp1, temp2));

	temp1[0] = m_HipKneePoint[1].x;
	temp1[1] = m_HipKneePoint[1].y;
	temp2[0] = m_HipKneePoint[3].x;
	temp2[1] = m_HipKneePoint[3].y;
	TibiaL = 90 - fabsf(geo.CalcLineAngle2D(temp1, temp2));

	m_HipKneeData[0] = TibiaR;
	m_HipKneeData[1] = TibiaL;

	// Femur Vs Mechanical Axis
	float FemurR, FemurL = 0.f;

	temp1[0] = m_HipKneePoint[2].x;
	temp1[1] = m_HipKneePoint[2].y;
	temp2[0] = m_HipKneePoint[4].x;
	temp2[1] = m_HipKneePoint[4].y;
	FemurR = 90 - fabsf(geo.CalcLineAngle2D(temp1, temp2));

	temp1[0] = m_HipKneePoint[3].x;
	temp1[1] = m_HipKneePoint[3].y;
	temp2[0] = m_HipKneePoint[5].x;
	temp2[1] = m_HipKneePoint[5].y;
	FemurL = 90 - fabsf(geo.CalcLineAngle2D(temp1, temp2));

	m_HipKneeData[2] = FemurR;
	m_HipKneeData[3] = FemurL;

	// Tibia Vs Femur
	float TibiaFemurR, TibiaFemurL = 0.f;
	VERTEX Line1, Line2;
	vxTemp1 = m_HipKneePoint[0];
	vxTemp1.z = 0.f;
	vxTemp2 = m_HipKneePoint[2];
	vxTemp2.z = 0.f;
	Line1 = geo.CalcLineVector(vxTemp1, vxTemp2);

	vxTemp1 = m_HipKneePoint[2];
	vxTemp1.z = 0.f;
	vxTemp2 = m_HipKneePoint[4];
	vxTemp2.z = 0.f;
	Line2 = geo.CalcLineVector(vxTemp1, vxTemp2);

	TibiaFemurR = __RADIAN * geo.CalcVectorInnerProduct(Line1, Line2);

	vxTemp1 = m_HipKneePoint[1];
	vxTemp1.z = 0.f;
	vxTemp2 = m_HipKneePoint[3];
	vxTemp2.z = 0.f;
	Line1 = geo.CalcLineVector(vxTemp1, vxTemp2);

	vxTemp1 = m_HipKneePoint[3];
	vxTemp1.z = 0.f;
	vxTemp2 = m_HipKneePoint[5];
	vxTemp2.z = 0.f;
	Line2 = geo.CalcLineVector(vxTemp1, vxTemp2);

	TibiaFemurL = __RADIAN * geo.CalcVectorInnerProduct(Line1, Line2);

	m_HipKneeData[4] = TibiaFemurR;
	m_HipKneeData[5] = TibiaFemurL;

	// Sagittal Tibia Vs Mechanical Axis
	float STibiaR, STibiaL = 0.f;

	temp1[0] = m_HipKneePoint[0].z;
	temp1[1] = m_HipKneePoint[0].y;
	temp2[0] = m_HipKneePoint[2].z;
	temp2[1] = m_HipKneePoint[2].y;
	STibiaR = 90 - fabsf(geo.CalcLineAngle2D(temp1, temp2));

	temp1[0] = m_HipKneePoint[1].z;
	temp1[1] = m_HipKneePoint[1].y;
	temp2[0] = m_HipKneePoint[3].z;
	temp2[1] = m_HipKneePoint[3].y;
	STibiaL = 90 - fabsf(geo.CalcLineAngle2D(temp1, temp2));

	m_HipKneeData[6] = STibiaR;
	m_HipKneeData[7] = STibiaL;

	// Sagittal Femur Vs Mechanical Axis
	float SFemurR, SFemurL = 0.f;

	temp1[0] = m_HipKneePoint[2].z;
	temp1[1] = m_HipKneePoint[2].y;
	temp2[0] = m_HipKneePoint[4].z;
	temp2[1] = m_HipKneePoint[4].y;
	SFemurR = 90 - fabsf(geo.CalcLineAngle2D(temp1, temp2));

	temp1[0] = m_HipKneePoint[3].z;
	temp1[1] = m_HipKneePoint[3].y;
	temp2[0] = m_HipKneePoint[5].z;
	temp2[1] = m_HipKneePoint[5].y;
	SFemurL = 90 - fabsf(geo.CalcLineAngle2D(temp1, temp2));

	m_HipKneeData[8] = SFemurR;
	m_HipKneeData[9] = SFemurL;

	// 7. Sagittal Femur Vs Tibia
	float STibiaFemurR, STibiaFemurL = 0.f;

	vxTemp1 = m_HipKneePoint[0];
	vxTemp1.x = 0.f;
	vxTemp2 = m_HipKneePoint[2];
	vxTemp2.x = 0.f;
	Line1 = geo.CalcLineVector(vxTemp2, vxTemp1);

	vxTemp1 = m_HipKneePoint[2];
	vxTemp1.x = 0.f;
	vxTemp2 = m_HipKneePoint[4];
	vxTemp2.x = 0.f;
	Line2 = geo.CalcLineVector(vxTemp1, vxTemp2);

	STibiaFemurR = __RADIAN * geo.CalcVectorInnerProduct(Line1, Line2);

	vxTemp1 = m_HipKneePoint[1];
	vxTemp1.x = 0.f;
	vxTemp2 = m_HipKneePoint[3];
	vxTemp2.x = 0.f;
	Line1 = geo.CalcLineVector(vxTemp2, vxTemp1);

	vxTemp1 = m_HipKneePoint[3];
	vxTemp1.x = 0.f;
	vxTemp2 = m_HipKneePoint[5];
	vxTemp2.x = 0.f;
	Line2 = geo.CalcLineVector(vxTemp1, vxTemp2);

	STibiaFemurL = __RADIAN * geo.CalcVectorInnerProduct(Line1, Line2);

	m_HipKneeData[10] = STibiaFemurR;
	m_HipKneeData[11] = STibiaFemurL;

	// 손에서 발목 까지의 거리	
	m_HipKneeData[12] = fabsf(m_HipKneePoint[0].y - m_HipKneePoint[6].y);
	m_HipKneeData[13] = fabsf(m_HipKneePoint[1].y - m_HipKneePoint[7].y);

	//엉덩이 양쪽 튀어나온 점의 각도
	// test code
	float fTempScanEnd = 10.f; // (*m_ScanDataList)[m_SelectSD].Crouch.y
	geo.GetMinMaxOfZForY(m_VertexList, m_VertexLength, 0.f, fTempScanEnd, vxMin, vxMax);

	geo.GetOutlinePoints(m_VertexList, m_VertexLength, m_FaceList, m_FaceLength, &GirthPT, GirthLen, vxMin.y);

	VERTEX PT1, PT2;
	int Len1 = GirthLen[0];
	int Len2 = GirthLen[1] - GirthLen[0];
	int Len3 = GirthLen[2] - GirthLen[1];
	int Len4 = GirthLen[3] - GirthLen[2];
	int MaxLen = std::max(Len1, std::max(Len2, std::max(Len3, Len4)));

	VERTEX* GirthTemp = new VERTEX[MaxLen];

	if (MaxLen == Len1)
	{
		memcpy(GirthTemp, GirthPT, sizeof(VERTEX) * MaxLen);
	}
	else if (MaxLen == Len2)
	{
		memcpy(GirthTemp, GirthPT + GirthLen[0], sizeof(VERTEX) * MaxLen);
	}
	else if (MaxLen == Len3)
	{
		memcpy(GirthTemp, GirthPT + GirthLen[1], sizeof(VERTEX) * MaxLen);
	}
	else if (MaxLen == Len4)
	{
		memcpy(GirthTemp, GirthPT + GirthLen[2], sizeof(VERTEX) * MaxLen);
	}

	geo.GetMinMaxPosition(GirthTemp, MaxLen, vxMinX, vxMaxX, vxMinY, vxMaxY, vxMinZ, vxMaxZ);

	PT1 = vxMinZ;

	float fCenter = vxMinX.x + fabsf(vxMaxX.x - vxMinX.x) / 2.f;
	vxMin = VERTEX(1000, 1000, 1000);
	float LeftX = vxMaxX.x;
	float RightX = vxMinX.x;

	if (PT1.x < 0)
	{
		for (int i = 0; i < MaxLen; i++)
		{
			if (GirthTemp[i].x > fCenter && GirthTemp[i].x <= LeftX)
			{
				if (GirthTemp[i].z < vxMin.z && GirthTemp[i].x > fCenter + (-PT1.x) / 2)
				{
					vxMin = GirthTemp[i];
				}
			}
		}
	}
	else if (PT1.x > 0)
	{
		for (int i = (int)MaxLen - 1; i >= 0; i--)
		{
			if (GirthTemp[i].x < fCenter && GirthTemp[i].x >= RightX)
			{
				if (GirthTemp[i].z < vxMin.z && GirthTemp[i].x < fCenter + (-PT1.x) / 2)
				{
					vxMin = GirthTemp[i];
				}
			}
		}
	}
	else
	{
		vxMin = PT1;
	}

	PT2 = vxMin;

	m_HipKneePoint[8] = PT1;
	m_HipKneePoint[9] = PT2;
	m_HipKneeData[14] = geo.CalcLinePlaneAngle(PT1, PT2, VERTEX(0, 0, 1));

	//양 무릎의 간격
	geo.GetOutlinePoints(m_VertexList, m_VertexLength, m_FaceList, m_FaceLength, &GirthPT, GirthLen, (m_ClickPoints[1].y - m_ClickPoints[2].y) / 2 + m_ClickPoints[2].y);

	if (0 == GirthLen[1])
	{
		m_HipKneeData[15] = 0;
	}
	else if (0 == GirthLen[3])
	{
		geo.GetMinMax(GirthPT, GirthLen[0], vxMin, vxMax);
		m_HipKneeData[15] = vxMax.x;

		geo.GetMinMax(GirthPT + GirthLen[0], GirthLen[1] - GirthLen[0], vxMin, vxMax);

		m_HipKneeData[15] -= vxMin.x;
		m_HipKneeData[15] = fabsf(m_HipKneeData[15]);
	}

	//m_ReportView.m_HipKnee.SetMainData(m_HipKneeData);
	//m_ReportView.m_HipKnee.SetPoints(m_HipKneePoint);

	/*
	girth 허리둘레
	*/

	/*
	output : 43개
	//calc point
	VERTEX m_HipKneePoint[10];
	m_HipKneePoint[0] = Ankle Center 발목 center R [0, 0]
	m_HipKneePoint[1] = Ankle Center 발목 center L [1, 0]
	m_HipKneePoint[2] = Knee Center 무릎 center R [0, 1]
	m_HipKneePoint[3] = Knee Center 무릎 center L [1, 1]
	m_HipKneePoint[4] = Thigh Point 넓적다리 point 1 [0, 2]
	m_HipKneePoint[5] = Thigh Point 넓적다리 point 2 [1, 2]
	m_HipKneePoint[6] = 손 1 [0, 3]
	m_HipKneePoint[7] = 손 2 [1, 3]
	m_HipKneePoint[8] = 엉덩이 양쪽 튀어나온 점 R [0, 4]
	m_HipKneePoint[9] = 엉덩이 양쪽 튀어나온 점 L [1, 4]

	VERTEX vxTibiaR[3], vxTibiaL[3], vxFemurR[2], vxFemurL[2]
	vxTibiaR[0] = m_Point[0][0];
	vxTibiaR[1] = m_Point[0][1];
	vxTibiaR[2].x = (-vxTibiaR[0].y / ZeroDivisor(vxTibiaR[1].y - vxTibiaR[0].y)) * (vxTibiaR[1].x - vxTibiaR[0].x) + vxTibiaR[0].x;
	vxTibiaR[2].z = (-vxTibiaR[0].y / ZeroDivisor(vxTibiaR[1].y - vxTibiaR[0].y)) * (vxTibiaR[1].z - vxTibiaR[0].z) + vxTibiaR[0].z;

	vxTibiaL[0] = m_Point[1][0];
	vxTibiaL[1] = m_Point[1][1];
	vxTibiaL[2].x = (-vxTibiaL[0].y / ZeroDivisor(vxTibiaL[1].y - vxTibiaL[0].y)) * (vxTibiaL[1].x - vxTibiaL[0].x) + vxTibiaL[0].x;
	vxTibiaL[2].z = (-vxTibiaL[0].y / ZeroDivisor(vxTibiaL[1].y - vxTibiaL[0].y)) * (vxTibiaL[1].z - vxTibiaL[0].z) + vxTibiaL[0].z;

	vxFemurR[0] = m_Point[0][1];
	vxFemurR[1].x = (-vxFemurR[0].y / ZeroDivisor(m_Point[0][2].y - vxFemurR[0].y)) * (m_Point[0][2].x - vxFemurR[0].x) + vxFemurR[0].x;
	vxFemurR[1].z = (-vxFemurR[0].y / ZeroDivisor(m_Point[0][2].y - vxFemurR[0].y)) * (m_Point[0][2].z - vxFemurR[0].z) + vxFemurR[0].z;

	vxFemurL[0] = m_Point[1][1];
	vxFemurL[1].x = (-vxFemurL[0].y / ZeroDivisor(m_Point[1][2].y - vxFemurL[0].y)) * (m_Point[1][2].x - vxFemurL[0].x) + vxFemurL[0].x;
	vxFemurL[1].z = (-vxFemurL[0].y / ZeroDivisor(m_Point[0][2].y - vxFemurL[0].y)) * (m_Point[0][2].z - vxFemurL[0].z) + vxFemurL[0].z;

	Line[0] = VERTEX(m_Point[0][4].x, -m_Point[0][4].z, 0.f);
	Line[1] = VERTEX(m_Point[1][4].x, -m_Point[1][4].z, 0.f);

	//calc data
	float m_HipKneeData[17];
	m_HipKneeData[0] = Tibia Vs Mechanical Axis 정강이뼈, 넓적다리뼈 R
	m_HipKneeData[1] = Tibia Vs Mechanical Axis 정강이뼈 L
	m_HipKneeData[2] = Femur Vs Mechanical Axis 대퇴골 R
	m_HipKneeData[3] = Femur Vs Mechanical Axis 대퇴골 L
	m_HipKneeData[4] = Tibia Vs Femur R
	m_HipKneeData[5] = Tibia Vs Femur L
	m_HipKneeData[6] = Sagittal Tibia Vs Mechanical Axis R 시상 봉합(矢狀縫合)의
	m_HipKneeData[7] = Sagittal Tibia Vs Mechanical Axis L 시상 봉합(矢狀縫合)의
	m_HipKneeData[8] = Sagittal Femur Vs Mechanical Axis R
	m_HipKneeData[9] = Sagittal Femur Vs Mechanical Axis L
	m_HipKneeData[10] = Sagittal Femur Vs Tibia R
	m_HipKneeData[11] = Sagittal Femur Vs Tibia L
	m_HipKneeData[12] = 손에서 발목 까지의 거리 R - / 10.f 필요
	m_HipKneeData[13] = 손에서 발목 까지의 거리 L - / 10.f 필요
	m_HipKneeData[14] = 엉덩이 양쪽 튀어나온 점의 각도
	m_HipKneeData[15] = 양 무릎의 간격 - / 10.f 필요
	m_HipKneeData[16] = 양 발목의 간격 - / 10.f 필요
	*/

	delete[] GirthPT;
	GirthPT = nullptr;

	delete[] GirthTemp;
	GirthTemp = nullptr;

	/*
	{
		"anglTiAx": {"R": , "L": },
		"anglFeAx": {"R": , "L": },
		"anglTiFe": {"R": , "L": },
		"anglSTiAx": {"R": , "L": },
		"anglSFeAx": {"R": , "L": },
		"anglSFeSTi": {"R": , "L": },
		"distHandAnkle": {"R": , "L": },
		"anglHip": ,
		"distKnee": ,
		"distAnkle": ,
		"tibia_pt3": {"R": {"x": , "y": , "z": }, "L": {"x": , "y": , "z": }},
		"femur_pt3": {"R": {"x": , "y": , "z": }, "L": {"x": , "y": , "z": }},
	}
	*/

	m_HipKneeData[12] *= 0.1f;
	m_HipKneeData[13] *= 0.1f;
	m_HipKneeData[15] *= 0.1f;
	m_HipKneeData[16] *= 0.1f;

	//
	VERTEX vxTibiaR, vxTibiaL, vxFemurR, vxFemurL;
	vxTibiaR.x = (-m_HipKneePoint[0].y / ZeroDivisor(m_HipKneePoint[2].y - m_HipKneePoint[0].y)) * (m_HipKneePoint[2].x - m_HipKneePoint[0].x) + m_HipKneePoint[0].x;
	vxTibiaR.z = (-m_HipKneePoint[0].y / ZeroDivisor(m_HipKneePoint[2].y - m_HipKneePoint[0].y)) * (m_HipKneePoint[2].z - m_HipKneePoint[0].z) + m_HipKneePoint[0].z;
	vxTibiaL.x = (-m_HipKneePoint[1].y / ZeroDivisor(m_HipKneePoint[3].y - m_HipKneePoint[1].y)) * (m_HipKneePoint[3].x - m_HipKneePoint[1].x) + m_HipKneePoint[1].x;
	vxTibiaL.z = (-m_HipKneePoint[1].y / ZeroDivisor(m_HipKneePoint[3].y - m_HipKneePoint[1].y)) * (m_HipKneePoint[3].z - m_HipKneePoint[1].z) + m_HipKneePoint[1].z;

	vxFemurR.x = (-m_HipKneePoint[2].y / ZeroDivisor(m_HipKneePoint[4].y - m_HipKneePoint[2].y)) * (m_HipKneePoint[4].x - m_HipKneePoint[2].x) + m_HipKneePoint[2].x;
	vxFemurR.z = (-m_HipKneePoint[2].y / ZeroDivisor(m_HipKneePoint[4].y - m_HipKneePoint[2].y)) * (m_HipKneePoint[4].z - m_HipKneePoint[2].z) + m_HipKneePoint[2].z;
	vxFemurL.x = (-m_HipKneePoint[3].y / ZeroDivisor(m_HipKneePoint[5].y - m_HipKneePoint[3].y)) * (m_HipKneePoint[5].x - m_HipKneePoint[3].x) + m_HipKneePoint[3].x;
	vxFemurL.z = (-m_HipKneePoint[3].y / ZeroDivisor(m_HipKneePoint[4].y - m_HipKneePoint[3].y)) * (m_HipKneePoint[4].z - m_HipKneePoint[3].z) + m_HipKneePoint[3].z;

	//
	const int nCNT = 12;
	const int nCNTSUB = 2;

	std::string strKeys[nCNT] = {
		"\"anglTiAx\"", "\"anglFeAx\"", "\"anglTiFe\"", "\"anglSTiAx\"", "\"anglSFeAx\"",
		"\"anglSFeSTi\"", "\"distHandAnkle\"", "\"anglHip\"", "\"distKnee\"", "\"distAnkle\"",
		"\"tibia_pt3\"", "\"femur_pt3\""
	};
	std::string strSubKeys[nCNTSUB] = { "\"R\"", "\"L\"" };

	//
	std::string strJson = JSONOpen;

	int i = 0, j = 0;

	for (i = 0; i < 7; i++)
	{
		strJson += strKeys[i] + JSONColon + JSONOpen;

		for (j = 0; j < nCNTSUB; j++)
		{
			strJson += strSubKeys[j] + JSONColon + std::to_string(m_HipKneeData[(i * nCNTSUB) + j]);

			if (j < nCNTSUB - 1)
			{
				strJson += JSONComma;
			}
		}

		strJson += JSONClose + JSONComma;
	}

	strJson += strKeys[7] + JSONColon + std::to_string(m_HipKneeData[14]) + JSONComma;
	strJson += strKeys[8] + JSONColon + std::to_string(m_HipKneeData[15]) + JSONComma;
	strJson += strKeys[9] + JSONColon + std::to_string(m_HipKneeData[16]) + JSONComma;

	strJson += strKeys[10] + JSONColon + JSONOpen;
	strJson += strSubKeys[0] + JSONColon + XYZ_toJsonStr(vxTibiaR) + JSONComma;
	strJson += strSubKeys[1] + JSONColon + XYZ_toJsonStr(vxTibiaL);
	strJson += JSONClose + JSONComma;

	strJson += strKeys[11] + JSONColon + JSONOpen;
	strJson += strSubKeys[0] + JSONColon + XYZ_toJsonStr(vxFemurR) + JSONComma;
	strJson += strSubKeys[1] + JSONColon + XYZ_toJsonStr(vxFemurL);
	strJson += JSONClose;

	strJson += JSONClose;

	return strJson;
}

const std::string CGccAnalysis::XYZ_toJsonStr(float x, float y, float z)
{
	std::string strJson = JSONOpen;
	strJson += std::string("\"x\"") + JSONColon + std::to_string(x) + JSONComma;
	strJson += std::string("\"y\"") + JSONColon + std::to_string(y) + JSONComma;
	strJson += std::string("\"z\"") + JSONColon + std::to_string(z);
	strJson += JSONClose;

	return strJson;
}

const std::string CGccAnalysis::XYZ_toJsonStr(const VERTEX &v)
{
	return XYZ_toJsonStr(v.x, v.y, v.z);
}

const std::string CGccAnalysis::XY_toJsonStr(float x, float y)
{
	std::string strJson = JSONOpen;
	strJson += std::string("\"x\"") + JSONColon + std::to_string(x) + JSONComma;
	strJson += std::string("\"y\"") + JSONColon + std::to_string(y);
	strJson += JSONClose;

	return strJson;
}

const std::string CGccAnalysis::CalcSpineData()
{
	VERTEX GradientPoint[2];
	float fGradient = 0.f;
	float Volume[2] = { 0.f };
	float HeightX[10] = { 0.f };
	float HeightY[10] = { 0.f };
	VERTEX RipHump[4];
	float RipHumpData[2] = { 0.f };

	// test code
	InitClickPoints(5);
	// test code

	/*
	input : Img/Spine_F.png, Spine_M.png
	m_ClickPoints 5개
	1 - 턱 점 필요!!!
	2 - 16 몸통부위 어깨가쪽점 Lateral Shoulder R >>>> 어깨가쪽점R(Shoulder-R)
	3 - 16 몸통부위 어깨가쪽점 Lateral Shoulder L >>>> 어깨가쪽점L(Shoulder-L)
	4 - 18 몸통부위 겨드랑점* Axilla >>>> 겨드랑점R(Armpit-R)
	5 - 18 몸통부위 겨드랑점* Axilla >>>> 겨드랑점L(Armpit-L)
	*/

	CalcSpineDataGradient(GradientPoint, fGradient);
	CalcSpineDataHeight(HeightX, HeightY);
	CalcSpineVolumeData(std::max(GradientPoint[0].y, GradientPoint[1].y), Volume);
	CalcSpineRibHumpData(RipHump, RipHumpData, GradientPoint);

	//
	memcpy(m_CurSpineData, GradientPoint, sizeof(GradientPoint));
	m_CurSpineData[6] = fGradient;
	memcpy(m_CurSpineData + 7, Volume, sizeof(Volume));
	memcpy(m_CurSpineData + 9, HeightX, sizeof(HeightX));
	memcpy(m_CurSpineData + 19, HeightY, sizeof(HeightY));
	memcpy(m_CurSpineData + 29, RipHump, sizeof(RipHump));
	memcpy(m_CurSpineData + 41, RipHumpData, sizeof(RipHumpData));

	//if (m_SelectSD + 1 < *m_ScanDataLength)
	{
		//m_ReportView.m_Spine.SetPoints(m_SpinePrePoints, m_ClickPoints);
		//m_ReportView.m_Spine.SetMainData(m_PreSpineData, m_CurSpineData);
	}
	//else
	{
		//m_ReportView.m_Spine.SetPoints(m_ClickPoints, m_SpinePrePoints);
		//m_ReportView.m_Spine.SetMainData(m_CurSpineData, m_PreSpineData);
	}

	/*
	output : 43개
	m_CurSpineData[43]
	012 - gradient 1 xyz - [shoulder gradient volume - gradient - peak]
	345 - gradient 2 xyz
	//temp = m_vxGradient1st[0].y > m_vxGradient1st[1].y ? m_vxGradient1st[0] : m_vxGradient1st[1];
	//strTemp.Format(_T("%s"), temp.x > 0 ? _T("Left") : _T("Right"));
	6 - [shoulder gradient volume - gradient - angle]
	7 - volume data R - / 1000000.f 필요. [shoulder gradient volume - volume - right]
	8 - volume data L - / 1000000.f 필요. [shoulder gradient volume - volume - left]
	9~18 - height x 10개 - 중간 - shoulder height 1st 그림 그리기에 사용
	19~28 - height y 10개 - 중간 - shoulder height 1st 그림 그리기에 사용
	*19~23 - height y 5개 - [left shoulder] - strTemp.Format(_T("%.1f"), (m_HeightY1st[i] - fBottom) / 10.f);
	*24~28 - height y 5개 - [right shoulder] - 순서 거꾸로 - strTemp.Format(_T("%.1f"), (m_HeightY1st[10 - (i - 4)] - fBottom) / 10.f);
	29 30 31 - 늑골 돌출고 rib hump xyz - [rib hump - direction 1] - m_vxRibHump1st[0][0].x > 0 ? _T("Left") : _T("Right"));
	32 33 34 - rib hump xyz - [0][1] - 아래 - left top 그림 그리기에 사용
	35 36 37 - rib hump xyz - [1][0] - [rib hump - direction 2] - m_vxRibHump1st[1][0].x > 0 ? _T("Left") : _T("Right"));
	38 39 40 - rib hump xyz - [1][1] - 아래 - left bottom 그림 그리기에 사용 (아마도...)
	41 - rib hump data - [rib hump - gradient 1]
	42 - rib hump data - [rib hump - gradient 2]

	Data 데이터 순서
	| Gradient[7](0~6)      | Volume[2](7~8) | Height[20](9~28) | RibHump[12](29~40) + RipHumpData[2](41~42)|
	| Point(x,y,z)*2 + Data | Data[2]        | Point(x,y)*10    | (Point(x,y,z))*4   + Data[2]              |

	{
		"shoulder gradient": {
			"pt1": {"x": , "y": , "z": },
			"pt2": {"x": , "y": , "z": },
			"angle": ,
			"volume": {"R": , "L": }
			"shoulder": {
				"L": [{"x": , "y": }, {"x": , "y": }, {"x": , "y": }, {"x": , "y": }, {"x": , "y": }],
				"R": [{"x": , "y": }, {"x": , "y": }, {"x": , "y": }, {"x": , "y": }, {"x": , "y": }]
			}
		},
		"rib hump": {
			"1": {
				"gradient": ,
				"direction pt": {"x": , "y": , "z": },
				"img pt": {"x": , "y": , "z": },
			},
			"2": {
				"gradient": ,
				"direction pt": {"x": , "y": , "z": },
				"img pt": {"x": , "y": , "z": }
			}
		}
	}
	*/

	std::string strKeys[2] = { "\"shoulder_gradient\"", "\"rib_hump\"" };
	std::string strSub1Keys[5] = { "\"pt1\"", "\"pt2\"", "\"angle\"", "\"volume\"", "\"shoulder_pt2\"" };
	std::string strSub15Keys[2] = { "\"L\"", "\"R\"" };
	std::string strSub2Keys[2] = { "\"1\"", "\"2\"" };
	std::string strSub21Keys[3] = { "\"gradient\"", "\"direction_pt3\"", "\"img_pt3\"" };

	std::string strJson = JSONOpen;

	strJson += strKeys[0] + JSONColon + JSONOpen;
	strJson += strSub1Keys[0] + JSONColon + XYZ_toJsonStr(m_CurSpineData[0], m_CurSpineData[1], m_CurSpineData[2]) + JSONComma;
	strJson += strSub1Keys[1] + JSONColon + XYZ_toJsonStr(m_CurSpineData[3], m_CurSpineData[4], m_CurSpineData[5]) + JSONComma;

	strJson += strSub1Keys[2] + JSONColon + std::to_string(m_CurSpineData[6]) + JSONComma;

	strJson += strSub1Keys[3] + JSONColon + JSONOpen;
	strJson += std::string("\"R\"") + JSONColon + std::to_string(m_CurSpineData[7]) + JSONComma;
	strJson += std::string("\"L\"") + JSONColon + std::to_string(m_CurSpineData[8]) + JSONClose + JSONComma;

	strJson += strSub1Keys[4] + JSONColon + JSONOpen;
	strJson += strSub15Keys[0] + JSONColon + JSONAryOpen;

	int i = 0;

	for (i = 0; i < 5; i++)
	{
		strJson += XY_toJsonStr(m_CurSpineData[9 + i], m_CurSpineData[19 + i]);

		if (i < 4)
		{
			strJson += JSONComma;
		}
	}

	strJson += JSONAryClose + JSONComma;

	strJson += strSub15Keys[1] + JSONColon + JSONAryOpen;

	for (i = 0; i < 5; i++)
	{
		strJson += XY_toJsonStr(m_CurSpineData[14 + i], m_CurSpineData[24 + i]);

		if (i < 4)
		{
			strJson += JSONComma;
		}
	}

	strJson += JSONAryClose;
	strJson += JSONClose;
	strJson += JSONClose + JSONComma;

	strJson += strKeys[1] + JSONColon + JSONOpen;
	strJson += strSub2Keys[0] + JSONColon + JSONOpen;
	strJson += strSub21Keys[0] + JSONColon + std::to_string(m_CurSpineData[41]) + JSONComma;
	strJson += strSub21Keys[1] + JSONColon + XYZ_toJsonStr(m_CurSpineData[29], m_CurSpineData[30], m_CurSpineData[31]) + JSONComma;
	strJson += strSub21Keys[2] + JSONColon + XYZ_toJsonStr(m_CurSpineData[32], m_CurSpineData[33], m_CurSpineData[34]);
	strJson += JSONClose + JSONComma;
	strJson += strSub2Keys[1] + JSONColon + JSONOpen;
	strJson += strSub21Keys[0] + JSONColon + std::to_string(m_CurSpineData[42]) + JSONComma;
	strJson += strSub21Keys[1] + JSONColon + XYZ_toJsonStr(m_CurSpineData[35], m_CurSpineData[36], m_CurSpineData[37]) + JSONComma;
	strJson += strSub21Keys[2] + JSONColon + XYZ_toJsonStr(m_CurSpineData[38], m_CurSpineData[39], m_CurSpineData[40]);
	strJson += JSONClose;
	strJson += JSONClose;
	strJson += JSONClose;

	return strJson;
}

void CGccAnalysis::CalcSpineDataGradient(VERTEX GRPT[2], float& Data)
{
	CGccGeometry geo;

	VERTEX Points[2];
	float fGradient = 0.f;
	VERTEX* GirthPoints = nullptr;
	VERTEX vxMinX, vxMinY, vxMinZ, vxMaxX, vxMaxY, vxMaxZ;
	int GirthLength[4] = { 0 };

	int Len = 0;

	geo.GetOutlinePoints(m_VertexList, m_VertexLength, m_FaceList, m_FaceLength, &GirthPoints, GirthLength, m_ClickPoints[1].y);
	Len = std::max(GirthLength[0], std::max(GirthLength[1], std::max(GirthLength[2], GirthLength[3])));

	geo.GetMinMaxPosition(GirthPoints, Len, vxMinX, vxMaxX, vxMinY, vxMaxY, vxMinZ, vxMaxZ);
	m_ClickPoints[1] = Points[0] = vxMinX;

	geo.GetOutlinePoints(m_VertexList, m_VertexLength, m_FaceList, m_FaceLength, &GirthPoints, GirthLength, m_ClickPoints[2].y);
	Len = std::max(GirthLength[0], std::max(GirthLength[1], std::max(GirthLength[2], GirthLength[3])));

	geo.GetMinMaxPosition(GirthPoints, Len, vxMinX, vxMaxX, vxMinY, vxMaxY, vxMinZ, vxMaxZ);
	m_ClickPoints[2] = Points[1] = vxMaxX;

	delete[] GirthPoints;
	GirthPoints = nullptr;

	float GRP1[2], GRP2[2] = { 0.f };
	GRP1[0] = Points[0].x;
	GRP1[1] = Points[0].y;
	GRP2[0] = Points[1].x;
	GRP2[1] = Points[1].y;
	fGradient = fabsf(geo.CalcLineAngle2D(GRP1, GRP2));
	Data = fGradient;

	memcpy(GRPT, Points, sizeof(Points));
}

void CGccAnalysis::CalcSpineDataHeight(float HeightX[10], float HeightY[10])
{
	CGccGeometry geo;
	VERTEX* GirthPoints = nullptr;
	VERTEX vxMin, vxMax;
	int GirthLength[4] = { 0 };
	float LeftX[5] = { 0.f };
	float RightX[5] = { 0.f };
	float LeftY[5] = { 0.f };
	float RightY[5] = { 0.f };

	geo.GetOutlinePoints(m_VertexList, m_VertexLength, m_FaceList, m_FaceLength, &GirthPoints, GirthLength, m_ClickPoints[0].y);
	int len = std::max(GirthLength[0], std::max(GirthLength[1], std::max(GirthLength[2], GirthLength[3])));
	geo.GetMinMax(GirthPoints, len, vxMin, vxMax);
	RightX[4] = m_ClickPoints[3].x;

	float fWidth = 20.f;

	for (int i = 3; i >= 0; i--)
	{
		RightX[i] = RightX[i + 1] + fWidth;
	}

	LeftX[0] = m_ClickPoints[4].x;

	fWidth = 20.f;

	for (int i = 1; i < 5; i++)
	{
		LeftX[i] = LeftX[i - 1] - fWidth;
	}

	memcpy(HeightX, LeftX, sizeof(LeftX));
	memcpy(HeightX + 5, RightX, sizeof(RightX));

	for (int i = 0; i < 5; i++)
	{
		geo.GetVtcOutlinePoints(m_VertexList, m_VertexLength, m_FaceList, m_FaceLength, &GirthPoints, GirthLength, RightX[i]);
		geo.GetMinMax(GirthPoints, GirthLength[0], vxMin, vxMax);

		if (vxMax.y > m_ClickPoints[4].y && vxMax.y < m_ClickPoints[0].y)
		{
			RightY[i] = vxMax.y;
			continue;
		}

		if (0 == GirthLength[1])
		{
			if (vxMax.y > m_ClickPoints[0].y)
			{
				RightY[i] = m_ClickPoints[0].y;
			}
			else
			{
				RightY[i] = vxMax.y;
			}

			continue;
		}

		geo.GetMinMax(GirthPoints + GirthLength[0], GirthLength[1] - GirthLength[0], vxMin, vxMax);

		if (vxMax.y > m_ClickPoints[4].y && vxMax.y < m_ClickPoints[0].y)
		{
			RightY[i] = vxMax.y;
			continue;
		}

		if (0 == GirthLength[2])
		{
			if (vxMax.y > m_ClickPoints[0].y)
			{
				RightY[i] = m_ClickPoints[0].y;
			}
			else
			{
				RightY[i] = vxMax.y;
			}

			continue;
		}

		geo.GetMinMax(GirthPoints + GirthLength[1], GirthLength[2] - GirthLength[1], vxMin, vxMax);

		if (vxMax.y > m_ClickPoints[4].y && vxMax.y < m_ClickPoints[0].y)
		{
			RightY[i] = vxMax.y;
			continue;
		}

		if (0 == GirthLength[3])
		{
			if (vxMax.y > m_ClickPoints[0].y)
			{
				RightY[i] = m_ClickPoints[0].y;
			}
			else
			{
				RightY[i] = vxMax.y;
			}

			continue;
		}

		geo.GetMinMax(GirthPoints + GirthLength[2], GirthLength[3] - GirthLength[2], vxMin, vxMax);

		if (vxMax.y > m_ClickPoints[4].y && vxMax.y < m_ClickPoints[0].y)
		{
			RightY[i] = vxMax.y;
			continue;
		}

		if (vxMax.y > m_ClickPoints[0].y)
		{
			RightY[i] = m_ClickPoints[0].y;
		}
		else
		{
			RightY[i] = vxMax.y;
		}
	}

	for (int i = 0; i < 5; i++)
	{
		geo.GetVtcOutlinePoints(m_VertexList, m_VertexLength, m_FaceList, m_FaceLength, &GirthPoints, GirthLength, LeftX[i]);
		geo.GetMinMax(GirthPoints, GirthLength[0], vxMin, vxMax);

		if (vxMax.y > m_ClickPoints[4].y && vxMax.y < m_ClickPoints[0].y)
		{
			LeftY[i] = vxMax.y;
			continue;
		}

		if (0 == GirthLength[1])
		{
			if (vxMax.y > m_ClickPoints[0].y)
			{
				LeftY[i] = m_ClickPoints[0].y;
			}
			else
			{
				LeftY[i] = vxMax.y;
			}

			continue;
		}

		geo.GetMinMax(GirthPoints + GirthLength[0], GirthLength[1] - GirthLength[0], vxMin, vxMax);

		if (vxMax.y > m_ClickPoints[4].y && vxMax.y < m_ClickPoints[0].y)
		{
			LeftY[i] = vxMax.y;
			continue;
		}

		if (0 == GirthLength[2])
		{
			if (vxMax.y > m_ClickPoints[0].y)
			{
				LeftY[i] = m_ClickPoints[0].y;
			}
			else
			{
				LeftY[i] = vxMax.y;
			}

			continue;
		}

		geo.GetMinMax(GirthPoints + GirthLength[1], GirthLength[2] - GirthLength[1], vxMin, vxMax);

		if (vxMax.y > m_ClickPoints[4].y && vxMax.y < m_ClickPoints[0].y)
		{
			LeftY[i] = vxMax.y;
			continue;
		}

		if (0 == GirthLength[3])
		{
			if (vxMax.y > m_ClickPoints[0].y)
			{
				LeftY[i] = m_ClickPoints[0].y;
			}
			else
			{
				LeftY[i] = vxMax.y;
			}

			continue;
		}

		geo.GetMinMax(GirthPoints + GirthLength[2], GirthLength[3] - GirthLength[2], vxMin, vxMax);

		if (vxMax.y > m_ClickPoints[4].y && vxMax.y < m_ClickPoints[0].y)
		{
			LeftY[i] = vxMax.y;
			continue;
		}

		if (vxMax.y > m_ClickPoints[0].y)
		{
			LeftY[i] = m_ClickPoints[0].y;
		}
		else
		{
			LeftY[i] = vxMax.y;
		}
	}

	delete[] GirthPoints;
	GirthPoints = nullptr;

	memcpy(HeightY, LeftY, sizeof(LeftY));
	memcpy(HeightY + 5, RightY, sizeof(RightY));
}

void CGccAnalysis::CalcSpineVolumeData(float bottom, float Volume[2])
{
	CGccGeometry geo;
	VERTEX* GirthPoints = nullptr;
	int GirthLength[4] = { 0 };
	float volumeR = 0.f, volumeL = 0.f;
	int Len = 0;

	for (float i = bottom; i < m_ClickPoints[0].y; i += 1.0f)
	{
		geo.GetOutlinePoints(m_VertexList, m_VertexLength, m_FaceList, m_FaceLength, &GirthPoints, GirthLength, i);
		Len = std::max(GirthLength[0], std::max(GirthLength[1], std::max(GirthLength[2], GirthLength[3])));
		geo.DivideBasedOnPoint(&GirthPoints, Len, GirthLength, VERTEX(0, 0, 0), 0);
		volumeR += geo.CalcAreaOfPolygon(GirthPoints, GirthLength[0]);
		volumeL += geo.CalcAreaOfPolygon(GirthPoints + GirthLength[0], GirthLength[1] - GirthLength[0]);
	}

	Volume[0] = volumeR;
	Volume[1] = volumeL;

	delete[] GirthPoints;
	GirthPoints = nullptr;
}

void CGccAnalysis::CalcSpineRibHumpData(VERTEX Point[4], float Data[2], VERTEX GradientPT[2])
{
	CGccGeometry geo;
	VERTEX* GirthPoints = nullptr;
	int GirthLength[4] = { 0 };
	VERTEX vxScanTop = GradientPT[0].y < GradientPT[1].y ? GradientPT[0] : GradientPT[1];

	// test code
	vxScanTop.y = 0.f;

	int CheckLen = (int)(fabsf(-200.f - vxScanTop.y)) / 10;
	VERTEX* MaxPointList = new VERTEX[CheckLen];
	int nBottom = -200;
	int Len1, Len2, Len3, Len4 = 0;
	int MaxLen = 0;
	VERTEX vxMaxX, vxMaxY, vxMaxZ;
	VERTEX vxMinX, vxMinY, vxMinZ;

	for (int i = 0, j = 0; j < CheckLen; i += 10, j++)
	{
		geo.GetOutlinePoints(m_VertexList, m_VertexLength, m_FaceList, m_FaceLength, &GirthPoints, GirthLength, (float)((int)i + nBottom));

		Len1 = GirthLength[0];
		Len2 = GirthLength[1] - GirthLength[0];
		Len3 = GirthLength[2] - GirthLength[1];
		Len4 = GirthLength[3] - GirthLength[2];
		MaxLen = std::max(Len1, std::max(Len2, std::max(Len3, Len4)));

		if (Len1 == MaxLen)
		{
			geo.GetMinMaxPosition(GirthPoints, Len1, vxMinX, vxMaxX, vxMinY, vxMaxY, vxMinZ, vxMaxZ);
			MaxPointList[j] = vxMinZ;
		}
		else if (Len2 == MaxLen)
		{
			geo.GetMinMaxPosition(GirthPoints + GirthLength[0], Len2, vxMinX, vxMaxX, vxMinY, vxMaxY, vxMinZ, vxMaxZ);
			MaxPointList[j] = vxMinZ;
		}
		else if (Len3 == MaxLen)
		{
			geo.GetMinMaxPosition(GirthPoints + GirthLength[1], Len3, vxMinX, vxMaxX, vxMinY, vxMaxY, vxMinZ, vxMaxZ);
			MaxPointList[j] = vxMinZ;
		}
		else if (Len4 == MaxLen)
		{
			geo.GetMinMaxPosition(GirthPoints + GirthLength[2], Len4, vxMinX, vxMaxX, vxMinY, vxMaxY, vxMinZ, vxMaxZ);
			MaxPointList[j] = vxMinZ;
		}
		else
		{
			MaxPointList[j] = MaxPointList[j - 1];
		}
	}

	float Center = 0.f;
	float fCenterZ = -1000.f;

	for (int i = 0; i < CheckLen; i++)
	{
		if (MaxPointList[i].y <= 100 && MaxPointList[i].y >= -100)
		{
			if (MaxPointList[i].z > fCenterZ)
			{
				Center = MaxPointList[i].y;
				fCenterZ = MaxPointList[i].z;
			}
		}
	}

	Point[0].z = 1000.f;
	Point[2].z = 1000.f;

	for (int i = 0; i < CheckLen; i++)
	{
		if (MaxPointList[i].y >= Center)
		{
			Point[0] = MaxPointList[i].z < Point[0].z ? MaxPointList[i] : Point[0];
		}

		if (MaxPointList[i].y < Center)
		{
			Point[2] = MaxPointList[i].z < Point[2].z ? MaxPointList[i] : Point[2];
		}
	}

	geo.GetOutlinePoints(m_VertexList, m_VertexLength, m_FaceList, m_FaceLength, &GirthPoints, GirthLength, Point[0].y);

	Len1 = GirthLength[0];
	Len2 = GirthLength[1] - GirthLength[0];
	Len3 = GirthLength[2] - GirthLength[1];
	Len4 = GirthLength[3] - GirthLength[2];
	MaxLen = std::max(Len1, std::max(Len2, std::max(Len3, Len4)));

	VERTEX* GirthTemp = new VERTEX[MaxLen];

	if (MaxLen == Len1)
	{
		memcpy(GirthTemp, GirthPoints, sizeof(VERTEX) * MaxLen);
	}
	else if (MaxLen == Len2)
	{
		memcpy(GirthTemp, GirthPoints + GirthLength[0], sizeof(VERTEX) * MaxLen);
	}
	else if (MaxLen == Len3)
	{
		memcpy(GirthTemp, GirthPoints + GirthLength[1], sizeof(VERTEX) * MaxLen);
	}
	else if (MaxLen == Len4)
	{
		memcpy(GirthTemp, GirthPoints + GirthLength[2], sizeof(VERTEX) * MaxLen);
	}

	//두개일 경우 두개를 하나로 생각하여 회전
	if (GirthLength[1] > 0 && GirthLength[2] == 0)
	{
		delete[] GirthTemp;
		GirthTemp = nullptr;

		MaxLen = GirthLength[1];
		GirthTemp = new VERTEX[MaxLen];
		memcpy(GirthTemp, GirthPoints, sizeof(VERTEX) * MaxLen);
	}

	float fAngle = 0.f;

	//좌우 양끝 점을 잇는 직선이 x축과 평행이 되도록 회전	
	do
	{
		geo.GetMinMaxPosition(GirthTemp, MaxLen, vxMinX, vxMaxX, vxMinY, vxMaxY, vxMinZ, vxMaxZ);

		fAngle = geo.CalcLinePlaneAngle(vxMinX, vxMaxX, VERTEX(0, 0, 1));

		if (fabsf(fAngle) > 0.001f)
		{
			geo.RotateModelD(GirthTemp, MaxLen, 0.f, fAngle, 0.f);
		}
	} while (fabsf(fAngle) > 0.001f);

	VERTEX* GirthTemp2 = nullptr;

	if (GirthLength[1] > 0 && GirthLength[2] == 0)
	{
		MaxLen = std::max(Len1, std::max(Len2, std::max(Len3, Len4)));
		GirthTemp2 = new VERTEX[MaxLen];

		if (MaxLen == Len1)
		{
			memcpy(GirthTemp2, GirthTemp, sizeof(VERTEX) * MaxLen);
		}
		else if (MaxLen == Len2)
		{
			memcpy(GirthTemp2, GirthTemp + GirthLength[0], sizeof(VERTEX) * MaxLen);
		}

		delete[] GirthTemp;
		GirthTemp = nullptr;

		GirthTemp = GirthTemp2;
	}

	geo.GetMinMaxPosition(GirthTemp, MaxLen, vxMinX, vxMaxX, vxMinY, vxMaxY, vxMinZ, vxMaxZ);

	Point[0] = vxMinZ;

	float fCenter = vxMinX.x + fabsf(vxMaxX.x - vxMinX.x) / 2.f;
	VERTEX vxMin = VERTEX(1000, 1000, 1000);
	float LeftX = GradientPT[0].x > GradientPT[1].x ? GradientPT[0].x : GradientPT[1].x;
	float RightX = GradientPT[0].x < GradientPT[1].x ? GradientPT[0].x : GradientPT[1].x;

	if (Point[0].x < 0)
	{
		for (int i = 0; i < MaxLen; i++)
		{
			if (GirthTemp[i].x > fCenter && GirthTemp[i].x <= LeftX)
			{
				if (GirthTemp[i].z < vxMin.z && GirthTemp[i].x > fCenter + (-Point[0].x) / 2)
				{
					vxMin = GirthTemp[i];
				}
			}
		}
	}
	else if (Point[0].x > 0)
	{
		for (int i = (int)MaxLen - 1; i >= 0; i--)
		{
			if (GirthTemp[i].x < fCenter && GirthTemp[i].x >= RightX)
			{
				if (GirthTemp[i].z < vxMin.z && GirthTemp[i].x < fCenter + (-Point[0].x) / 2)
				{
					vxMin = GirthTemp[i];
				}
			}
		}
	}
	else
	{
		vxMin = Point[0];
	}

	Point[1] = vxMin;

	//if (m_SelectSD == *m_ScanDataLength - 1)
	{
		//Point[2] = Point[0];
		//Point[3] = Point[1];
	}
	//else
	{
		float height = m_ClickPoints[0].y - fabsf(m_SpinePrePoints[0].y - m_PreSpineData[30]);

		geo.GetOutlinePoints(m_VertexList, m_VertexLength, m_FaceList, m_FaceLength, &GirthPoints, GirthLength, height);

		Len1 = GirthLength[0];
		Len2 = GirthLength[1] - GirthLength[0];
		Len3 = GirthLength[2] - GirthLength[1];
		Len4 = GirthLength[3] - GirthLength[2];
		MaxLen = std::max(Len1, std::max(Len2, std::max(Len3, Len4)));

		if (GirthTemp)
		{
			delete[] GirthTemp;
			GirthTemp = nullptr;
		}

		GirthTemp = new VERTEX[MaxLen];

		if (MaxLen == Len1)
		{
			memcpy(GirthTemp, GirthPoints, sizeof(VERTEX) * MaxLen);
		}
		else if (MaxLen == Len2)
		{
			memcpy(GirthTemp, GirthPoints + GirthLength[0], sizeof(VERTEX) * MaxLen);
		}
		else if (MaxLen == Len3)
		{
			memcpy(GirthTemp, GirthPoints + GirthLength[1], sizeof(VERTEX) * MaxLen);
		}
		else if (MaxLen == Len4)
		{
			memcpy(GirthTemp, GirthPoints + GirthLength[2], sizeof(VERTEX) * MaxLen);
		}

		//좌우 양끝 점을 잇는 직선이 x축과 평행이 되도록 회전
		if (GirthLength[1] > 0 && GirthLength[2] == 0)
		{
			delete[] GirthTemp;
			GirthTemp = nullptr;

			MaxLen = GirthLength[1];
			GirthTemp = new VERTEX[MaxLen];
			memcpy(GirthTemp, GirthPoints, sizeof(VERTEX) * MaxLen);
		}

		do
		{
			geo.GetMinMaxPosition(GirthTemp, MaxLen, vxMinX, vxMaxX, vxMinY, vxMaxY, vxMinZ, vxMaxZ);

			fAngle = geo.CalcLinePlaneAngle(vxMinX, vxMaxX, VERTEX(0, 0, 1));

			if (fabsf(fAngle) > 0.001f)
			{
				geo.RotateModelD(GirthTemp, MaxLen, 0.f, fAngle, 0.f);
			}
		} while (fabsf(fAngle) > 0.001f);

		if (GirthLength[1] > 0 && GirthLength[2] == 0)
		{
			MaxLen = std::max(Len1, std::max(Len2, std::max(Len3, Len4)));
			GirthTemp2 = new VERTEX[MaxLen];

			if (MaxLen == Len1)
			{
				memcpy(GirthTemp2, GirthTemp, sizeof(VERTEX) * MaxLen);
			}
			else if (MaxLen == Len2)
			{
				memcpy(GirthTemp2, GirthTemp + GirthLength[0], sizeof(VERTEX) * MaxLen);
			}

			delete[] GirthTemp;
			GirthTemp = nullptr;

			GirthTemp = GirthTemp2;
		}

		geo.GetMinMaxPosition(GirthTemp, MaxLen, vxMinX, vxMaxX, vxMinY, vxMaxY, vxMinZ, vxMaxZ);

		Point[2] = vxMinZ;

		fCenter = vxMinX.x + fabsf(vxMaxX.x - vxMinX.x) / 2.f;
		VERTEX vxMin2 = VERTEX(1000, 1000, 1000);
		float LeftX2 = GradientPT[0].x > GradientPT[1].x ? GradientPT[0].x : GradientPT[1].x;
		float RightX2 = GradientPT[0].x < GradientPT[1].x ? GradientPT[0].x : GradientPT[1].x;

		if (Point[2].x < 0)
		{
			for (int i = 0; i < MaxLen; i++)
			{
				if (GirthTemp[i].x > fCenter && GirthTemp[i].x <= LeftX2)
				{
					if (GirthTemp[i].z < vxMin2.z && GirthTemp[i].x > fCenter + (-Point[2].x) / 2)
					{
						vxMin2 = GirthTemp[i];
					}
				}
			}
		}
		else if (Point[2].x > 0)
		{
			for (int i = (int)MaxLen - 1; i >= 0; i--)
			{
				if (GirthTemp[i].x < fCenter && GirthTemp[i].x >= RightX2)
				{
					if (GirthTemp[i].z < vxMin2.z && GirthTemp[i].x < fCenter + (-Point[2].x) / 2)
					{
						vxMin2 = GirthTemp[i];
					}
				}
			}
		}
		else
		{
			vxMin2 = Point[2];
		}

		Point[3] = vxMin2;
	}

	Data[0] = geo.CalcLinePlaneAngle(Point[0], Point[1], VERTEX(0, 0, 1));
	Data[1] = geo.CalcLinePlaneAngle(Point[2], Point[3], VERTEX(0, 0, 1));

	delete[] MaxPointList;
	MaxPointList = nullptr;

	delete[] GirthPoints;
	GirthPoints = nullptr;

	delete[] GirthTemp;
	GirthTemp = nullptr;
}

const std::string CGccAnalysis::CalcShapeData()
{
	CGccGeometry geo;
	VERTEX* Girthpt = nullptr;
	float Girth[4] = { 0.f };
	float Max = 0.f;
	int GirthLen[4] = { 0 };

	// test code
	InitClickPoints(3);
	// test code

	/*
	input : Img/Shape_F.png, Shape_M.png
	m_ClickPoints 3개
	1 - 27 몸통부위 젖꼭지점 Nipple >>>> 젖꼭지점RL 평균
	2 - 34 몸통부위 배꼽점* Anterior Waist >>>> 배꼽점(Navel)
	3 - 38 몸통부위 엉덩이돌출점 Buttock Protrusion >>>> 엉덩이돌출점(Hip)
	*/

	geo.GetOutlinePoints(m_VertexList, m_VertexLength, m_FaceList, m_FaceLength,
		&Girthpt, GirthLen, m_ClickPoints[m_ClickPointsCount - 1].y);

	// girth (특히 사람의) 허리둘레; 둘레 치수
	Girth[0] = geo.CalcGirth(Girthpt, GirthLen[0]) / 10.f;
	Girth[1] = (GirthLen[1] == 0 ? 0 : geo.CalcGirth(Girthpt + GirthLen[0], GirthLen[1] - GirthLen[0])) / 10.f;
	Girth[2] = (GirthLen[2] == 0 ? 0 : geo.CalcGirth(Girthpt + GirthLen[1], GirthLen[2] - GirthLen[1])) / 10.f;
	Girth[3] = (GirthLen[3] == 0 ? 0 : geo.CalcGirth(Girthpt + GirthLen[2], GirthLen[3] - GirthLen[2])) / 10.f;
	Max = std::max(Girth[0], std::max(Girth[1], std::max(Girth[2], Girth[3])));
	m_ShapeData[2 + m_ClickPointsCount] = Max;

	// test code
	float fTempScanWeightL = 10.f;
	float fTempScanWeightR = 10.f;
	float fTempScanStature = 10.f;
	float fTempScanWaist = 10.f;
	float fTempScanHip = 10.f;
	float fTempScanVolume = 10.f;
	float fTempScanSurface = 10.f;

	// bmi - body mass index 체질량 지수
	//m_ShapeData[8] = ((*m_ScanDataList)[m_SelectSD].WeightL + (*m_ScanDataList)[m_SelectSD].WeightR) /
	//	((*m_ScanDataList)[m_SelectSD].Stature / 100) /
	//	((*m_ScanDataList)[m_SelectSD].Stature / 100);
	m_ShapeData[8] = (fTempScanWeightL + fTempScanWeightR) /
		(fTempScanStature / 100) /
		(fTempScanStature / 100);

	// whr - waist to hip ratio
	m_ShapeData[4] = fTempScanWaist;
	m_ShapeData[5] = fTempScanHip;
	m_ShapeData[9] = m_ShapeData[4] / m_ShapeData[5];

	if (Girthpt)
	{
		delete Girthpt;
		Girthpt = nullptr;
	}

	// test code
	m_ShapeData[0] = fTempScanStature;
	m_ShapeData[1] = fTempScanWeightL;
	m_ShapeData[2] = fTempScanWeightR;
	//m_ShapeData[3] = breast 둘레
	//m_ShapeData[4] = waist 둘레
	//m_ShapeData[5] = hip 둘레
	m_ShapeData[6] = fTempScanVolume;
	m_ShapeData[7] = fTempScanSurface;
	//m_ShapeData[8] = bmi - body mass index 체질량 지수
	//m_ShapeData[9] = whr - waist to hip ratio

	//m_ReportView.m_Shape.SetMainData(m_ShapeData);

	/*
	output : 10개
	todo : 기본 정보 중복
	m_ShapeData[10]
	0 - stature
	1 - weight L
	2 - weight R
	3 - breast size
	4 - waist size
	5 - hip size
	6 - volume
	7 - surface
	8 - bmi
	9 - whr

	{
		"stature": ,
		"weight": {"R": , "L": },
		"breast size": ,
		"waist size": ,
		"hip size": ,
		"volume": ,
		"surface": ,
		"bmi": ,
		"whr": ,
	}
	*/

	const int nCNT = 9;
	std::string strKeys[nCNT] = {
		"\"stature\"", "\"weight\"", "\"breast_size\"",
		"\"waist_size\"", "\"hip_size\"", "\"volume\"",
		"\"surface\"", "\"bmi\"", "\"whr\""
	};

	//int i = 0;

	//
	std::string strJson = JSONOpen;

	strJson += strKeys[0] + JSONColon + std::to_string(m_ShapeData[0]) + JSONComma;
	strJson += strKeys[1] + JSONColon + JSONOpen;
	strJson += std::string("\"R\"") + JSONColon + std::to_string(m_ShapeData[2]) + JSONComma; // LR > RL
	strJson += std::string("\"L\"") + JSONColon + std::to_string(m_ShapeData[1]); // LR > RL
	strJson += JSONClose + JSONComma;
	strJson += strKeys[2] + JSONColon + std::to_string(m_ShapeData[3]) + JSONComma;
	strJson += strKeys[3] + JSONColon + std::to_string(m_ShapeData[4]) + JSONComma;
	strJson += strKeys[4] + JSONColon + std::to_string(m_ShapeData[5]) + JSONComma;
	strJson += strKeys[5] + JSONColon + std::to_string(m_ShapeData[6]) + JSONComma;
	strJson += strKeys[6] + JSONColon + std::to_string(m_ShapeData[7]) + JSONComma;
	strJson += strKeys[7] + JSONColon + std::to_string(m_ShapeData[8]) + JSONComma;
	strJson += strKeys[8] + JSONColon + std::to_string(m_ShapeData[9]);

	strJson += JSONClose;

	return strJson;
}
