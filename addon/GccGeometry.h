#pragma once

#include "Gcc3dType.h"

#define _USE_MATH_DEFINES // M_PI
#include <math.h> // sqrtf
const float __RADIAN = (float)(180.f / M_PI);
const float __DEGREE = (float)(M_PI / 180.f);


class CGccGeometry
{
public:
	CGccGeometry();
	virtual ~CGccGeometry();

public:
	// stlFacet 리스트 형식을 Vertex 리스트 형식으로 변환
	bool STLtoVertexList(VERTEX** VertexList, int& vertexLength,
		FACE** FaceList, int& faceLength,
		BinarySTLtriangle* STLFaceList, int stlLenght);
	void CalcVertexNormal(VERTEX* NormalList, int VertexLength, FACE* FaceList, int FaceLength);
	void TranslatedModel(VERTEX* VertexList, int VertexLength, float X, float Y, float Z);
protected:
	// Y축으로 정렬하여 삽입
	int Insert(VERTEX* VertexList, VERTEX InsertVertex, int& Last);
	int Insert(VERTEX* VertexList, float InsertVertex[3], int& Last);
	void CalcNormalVector(VERTEX V1, VERTEX V2, VERTEX V3, VERTEX& Normal);
	void ReduceToUnit(VERTEX& Vertex);
	void Translated(VERTEX& Vertex, float X, float Y, float Z);

	// {{ CGccAnalysis::CalcVnSData() 에서 사용
public:
	/*
	Img/VnS_F.png, VnS_M.png
	PartsPt[0] - 목 ?
	PartsPt[1] - 겨드랑이R ?
	PartsPt[2] - 겨드랑이L ?
	PartsPt[3] - chest - 가슴, 흉부
	PartsPt[4] - waist - 허리
	PartsPt[5] - crotch - 가랑이, 사타구니
	PartsPt[6] - knee R - 무릎R
	PartsPt[7] - knee L - 무릎L
	*/
	void CalcPartsVolumeNSurface(VERTEX* VertexList, int VertexLength, FACE* FaceList, int FaceLength, VERTEX PartsPt[8], float Volume[6][2], float Surface[6][2]);
protected:
	VERTEX GetPointOnLineForY(VERTEX Vector, VERTEX P1, float Y);
	int InsertSortX(VERTEX** List, VERTEX In, int& Length);
	// }} CGccAnalysis::CalcVnSData() 에서 사용

	// {{ CGccAnalysis::CalcHipKneeData() 에서 사용
public:
	void GetOutlinePoints(VERTEX* VertexList, int VertexLength, FACE* FaceList, int FaceLength, VERTEX** GirthList, int GirthLength[4], float Hight);
protected:
	int IsCrossLinePlane(VERTEX LineP1, VERTEX LineP2, float PlaneEquation[4]);
	// 한 점에서 aX+bY+cZ=d 의 평면에 내린 수선의 발
	VERTEX CalcFootOfPerpendicular(VERTEX Point, float PlaneEquation[4]);
	bool GetIntersectionPointLinePlane(VERTEX LineP1, VERTEX LineP2, float PlaneEquation[4], VERTEX& IntersectionPoint);
	void OutlineDivision(VERTEX* GirthList, int PointLength, int GirthLength[4]);
	void SortOutlinePoints(VERTEX** PointList, int& Length);
	void Swap(VERTEX& A, VERTEX& B);
	void SortOutlineList(VERTEX* GirthList, int* GirthLength, int TotalLength);
public:
	// 이미 분리된 Girth포인트들을 지정된 Girth부위를 지정된 점으로 분할
	void DivideBasedOnPoint(VERTEX** GirthList, int& PointLength, int GirthLength[4], VERTEX BasePoint, int DvideGirthNum);
protected:
	void SortByNearPoint(VERTEX* List, int Length);
	float CalcDistance(VERTEX P1, VERTEX P2);
public:
	void GetMinMax(VERTEX* VertexList, int VertexLength, VERTEX& Min, VERTEX& Max);
	void GetMinMaxPosition(VERTEX* vxList, int nLength, VERTEX& vxXmin, VERTEX& vxXmax, VERTEX& vxYmin, VERTEX& vxYmax, VERTEX& vxZmin, VERTEX& vxZmax);
	void GetMinMaxOfZForY(VERTEX* VertexList, int VertexLength, float Start, float End, VERTEX& Min, VERTEX& Max);
	float CalcLineAngle2D(float P1[2], float P2[2]);
	VERTEX CalcLineVector(VERTEX V1, VERTEX V2);
	// 라디안 값 반환
	float CalcVectorInnerProduct(VERTEX vector1, VERTEX vector2);
	float CalcLinePlaneAngle(VERTEX P1, VERTEX P2, VERTEX PlaneNormal);
	// }} CGccAnalysis::CalcHipKneeData() 에서 사용

	// {{ CGccAnalysis::CalcSpineData() 에서 사용
public:
	void GetVtcOutlinePoints(VERTEX* VertexList, int VertexLength, FACE* FaceList, int FaceLength, VERTEX** GirthList, int GirthLength[4], float fPoinsX);
protected:
	void SortOutlinePointsY(VERTEX** PointList, int& Length);
	void OutlineDivisionY(VERTEX* GirthList, int PointLength, int GirthLength[4]);
public:
	float CalcAreaOfPolygon(VERTEX* List, int Length);
	void RotateModelD(VERTEX* VertexList, int VertexLength, float DegreeX, float DegreeY, float DegreeZ);
protected:
	void RotateModel(VERTEX* VertexList, int VertexLength, float RadianX, float RadianY, float RadianZ);
	void Rotate(VERTEX& Vertex, float RadianX, float RadianY, float RadianZ);
	// }} CGccAnalysis::CalcSpineData() 에서 사용

	// {{ CGccAnalysis::CalcShapeData() 에서 사용
public:
	float CalcGirth(VERTEX* List, int Length);
	// }} CGccAnalysis::CalcShapeData() 에서 사용
};
