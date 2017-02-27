#pragma once

#include "Gcc3dType.h"

#include <string> // std::string

const int SIZE_VNS = 24;
const int SIZE_LATERAL = 32;
const int SIZE_HIPKNEE = 17;
const int SIZE_SHAPE = 10;
const int SIZE_SPINE = 43;

class CGccAnalysis
{
public:
	CGccAnalysis();
	virtual ~CGccAnalysis();

protected:
	void InitClickPoints(int nCnt = 0);
	const std::string XYZ_toJsonStr(float x, float y, float z);
	const std::string XYZ_toJsonStr(const VERTEX &v);
	const std::string XY_toJsonStr(float x, float y);

protected:
	VERTEX* m_VertexList;
	int m_VertexLength;

	FACE* m_FaceList;
	int m_FaceLength;

	VERTEX* m_ClickPoints;
	int m_ClickPointsCount;

	//
	float m_VnSData[SIZE_VNS];
	//
	float m_LateralData[SIZE_LATERAL];
	//
	float m_HipKneeData[SIZE_HIPKNEE];
	VERTEX m_HipKneePoint[10];
	//
	float m_CurSpineData[SIZE_SPINE];
	float m_PreSpineData[SIZE_SPINE];
	VERTEX m_SpinePrePoints[5];
	/* m_SpineData 데이터 순서
	| Gradient[7](0~6)      | Volume[2](7~8) | Height[20](9~28) | RibHump[8](29~36)           + RipHumpData[2](37~38)|
	| Point(x,y,z)*2 + Data | Data[2]        | Point(x,y)*10    | (Point(x,y,z) + Point(z))*2 + Data[2]              |
	*/
	//
	float m_ShapeData[SIZE_SHAPE];

	//
	//ScanData** m_ScanDataList;
	//int* m_ScanDataLength;
	//int m_SelectSD;

public:
	const std::string CalcVnSData(); // volume
	const std::string CalcLateralData(); // lateral
	const std::string CalcHipKneeData(); // hip&knee
	const std::string CalcSpineData(); // scoliosis
protected:
	void CalcSpineDataGradient(VERTEX GRPT[2], float& Data);
	void CalcSpineDataHeight(float HeightX[10], float HeightY[10]);
	void CalcSpineVolumeData(float bottom, float Volume[2]);
	void CalcSpineRibHumpData(VERTEX Point[4], float Data[2], VERTEX GradientPT[2]);
public:
	const std::string CalcShapeData(); // shape
};
