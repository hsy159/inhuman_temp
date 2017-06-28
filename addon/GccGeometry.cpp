#include "GccGeometry.h"

#include <string.h> // memcpy
#include <algorithm> // std::min
//#include <float.h> // FLT_MAX

const int POINT_ZERO = 0;
const int POINT_CROSS = 1;
const int POINT_CONTACT1 = 2;
const int POINT_CONTACT2 = 3;
const int POINT_INCLUDE = 4;

CGccGeometry::CGccGeometry()
{
}


CGccGeometry::~CGccGeometry()
{
}

//17
// 중복되지 않는 점만 삽입
int CGccGeometry::Insert(VERTEX* VertexList, VERTEX InsertVertex, int& Last)
{
	//VERTEX* VertexTemp = NULL;
	if (0 == Last)
	{
		VertexList[0] = InsertVertex;
		++Last;
	}
	else
	{
		for (int i = 0; i <= Last / 2; i++)
		{
			if (VertexList[i].x == InsertVertex.x)
			{
				if (VertexList[i].y == InsertVertex.y &&
					VertexList[i].z == InsertVertex.z)
				{
					return i;
				}
			}

			if (VertexList[Last - 1 - i].x == InsertVertex.x)
			{
				if (VertexList[Last - 1 - i].y == InsertVertex.y &&
					VertexList[Last - 1 - i].z == InsertVertex.z)
				{
					return Last - 1 - i;
				}
			}
		}

		VertexList[Last] = InsertVertex;
		++Last;
	}

	return Last - 1;
}

//50
int CGccGeometry::Insert(VERTEX* VertexList, float InsertVertex[3], int& Last)
{
	VERTEX vertex;
	vertex.x = InsertVertex[0];
	vertex.y = InsertVertex[1];
	vertex.z = InsertVertex[2];

	return Insert(VertexList, vertex, Last);
}

//61
bool CGccGeometry::STLtoVertexList(VERTEX **VertexList, int &vertexLength,
	FACE **FaceList, int &faceLength,
	BinarySTLtriangle *STLFaceList, int stlLenght)
{
	if (stlLenght <= 0)
	{
		return false;
	}

	//
	if (*FaceList)
	{
		delete[] * FaceList;
		*FaceList = nullptr;
	}

	if (*VertexList)
	{
		delete[] * VertexList;
		*VertexList = nullptr;
	}

	*FaceList = new FACE[stlLenght];

	VERTEX *VListTemp = nullptr;
	VListTemp = new VERTEX[stlLenght * 3];

	faceLength = stlLenght;
	vertexLength = 0;
	int i;

	for (i = 0; i < stlLenght; i++)
	{
		(*FaceList)[i].faceNormal.x = STLFaceList[i].Normal[0];
		(*FaceList)[i].faceNormal.y = STLFaceList[i].Normal[1];
		(*FaceList)[i].faceNormal.z = STLFaceList[i].Normal[2];

		(*FaceList)[i].List[0] = Insert(VListTemp, STLFaceList[i].Vertex1, vertexLength);
		(*FaceList)[i].List[1] = Insert(VListTemp, STLFaceList[i].Vertex2, vertexLength);
		(*FaceList)[i].List[2] = Insert(VListTemp, STLFaceList[i].Vertex3, vertexLength);

		if ((*FaceList)[i].faceNormal.x == 0.f &&
			(*FaceList)[i].faceNormal.y == 0.f &&
			(*FaceList)[i].faceNormal.z == 0.f)
		{
			CalcNormalVector(STLFaceList[i].Vertex1,
				STLFaceList[i].Vertex2,
				STLFaceList[i].Vertex3,
				(*FaceList)[i].faceNormal);
		}
	}

	*VertexList = new VERTEX[vertexLength];
	memcpy(*VertexList, VListTemp, sizeof(VERTEX) * vertexLength);

	delete[] VListTemp;
	VListTemp = nullptr;

	return true;
}

//136
void CGccGeometry::CalcVertexNormal(VERTEX* NormalList, int VertexLength, FACE* FaceList, int FaceLength)
{
	for (int i = 0; i < VertexLength; i++)
	{
		NormalList[i].x = 0.f;
		NormalList[i].y = 0.f;
		NormalList[i].z = 0.f;
	}

	for (int i = 0; i < FaceLength; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			NormalList[FaceList[i].List[j]].x += FaceList[i].faceNormal.x;
			NormalList[FaceList[i].List[j]].y += FaceList[i].faceNormal.y;
			NormalList[FaceList[i].List[j]].z += FaceList[i].faceNormal.z;
		}
	}

	for (int i = 0; i < VertexLength; i++)
	{
		ReduceToUnit(NormalList[i]);
	}
}

//159
void CGccGeometry::ReduceToUnit(VERTEX& vector)
{
	float length = sqrt((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));

	if (0.f == length)
	{
		length = 1.0f;
	}

	vector.x /= length;
	vector.y /= length;
	vector.z /= length;
}

//173
void CGccGeometry::RotateModel(VERTEX* VertexList, int VertexLength, float RadianX, float RadianY, float RadianZ)
{
	for (int i = 0; i < VertexLength; i++)
	{
		Rotate(VertexList[i], RadianX, RadianY, RadianZ);
	}
}

//181
void CGccGeometry::RotateModelD(VERTEX* VertexList, int VertexLength, float DegreeX, float DegreeY, float DegreeZ)
{
	RotateModel(VertexList, VertexLength, DegreeX * __DEGREE, DegreeY * __DEGREE, DegreeZ * __DEGREE);
}

//186
void CGccGeometry::Rotate(VERTEX& Vertex, float RadianX, float RadianY, float RadianZ)
{
	VERTEX temp;

	if (RadianY != 0)
	{
		temp.z = Vertex.z * cos(RadianY) - Vertex.x * sin(RadianY);
		temp.x = Vertex.z * sin(RadianY) + Vertex.x * cos(RadianY);
		temp.y = Vertex.y;
		Vertex = temp;
	}

	if (RadianX != 0)
	{
		temp.y = Vertex.y * cos(RadianX) - Vertex.z * sin(RadianX);
		temp.z = Vertex.y * sin(RadianX) + Vertex.z * cos(RadianX);
		temp.x = Vertex.x;
		Vertex = temp;
	}

	if (RadianZ != 0)
	{
		temp.x = Vertex.x * cos(RadianZ) - Vertex.y * sin(RadianZ);
		temp.y = Vertex.x * sin(RadianZ) + Vertex.y * cos(RadianZ);
		temp.z = Vertex.z;
		Vertex = temp;
	}
}

//238
void CGccGeometry::TranslatedModel(VERTEX* VertexList, int VertexLength, float X, float Y, float Z)
{
	for (int i = 0; i < VertexLength; i++)
	{
		Translated(VertexList[i], X, Y, Z);
	}
}

void CGccGeometry::Translated(VERTEX& Vertex, float X, float Y, float Z)
{
	Vertex.x += X;
	Vertex.y += Y;
	Vertex.z += Z;
}

//253
void CGccGeometry::GetMinMax(VERTEX* VertexList, int VertexLength, VERTEX& Min, VERTEX& Max)
{
	Min.x = Min.y = Min.z = 100000000000000000000.f;
	Max.x = Max.y = Max.z = -100000000000000000000.f;

	for (int i = 0; i < VertexLength; i++)
	{
		Min.x = std::min(VertexList[i].x, Min.x);
		Max.x = std::max(VertexList[i].x, Max.x);
		Min.y = std::min(VertexList[i].y, Min.y);
		Max.y = std::max(VertexList[i].y, Max.y);
		Min.z = std::min(VertexList[i].z, Min.z);
		Max.z = std::max(VertexList[i].z, Max.z);
	}
}

//267
void CGccGeometry::GetMinMaxPosition(VERTEX* vxList, int nLength, VERTEX& vxXmin, VERTEX& vxXmax, VERTEX& vxYmin, VERTEX& vxYmax, VERTEX& vxZmin, VERTEX& vxZmax)
{
	vxXmax = vxYmax = vxZmax = VERTEX(-100000000000000000000000.f, -100000000000000000000000.f, -100000000000000000000000.f);
	vxXmin = vxYmin = vxZmin = VERTEX(100000000000000000000000.f, 100000000000000000000000.f, 100000000000000000000000.f);

	for (int i = 0; i < nLength; i++)
	{
		vxXmax = vxList[i].x > vxXmax.x ? vxList[i] : vxXmax;
		vxXmin = vxList[i].x < vxXmin.x ? vxList[i] : vxXmin;
		vxYmax = vxList[i].y > vxYmax.y ? vxList[i] : vxYmax;
		vxYmin = vxList[i].y < vxYmin.y ? vxList[i] : vxYmin;
		vxZmax = vxList[i].z > vxZmax.z ? vxList[i] : vxZmax;
		vxZmin = vxList[i].z < vxZmin.z ? vxList[i] : vxZmin;
	}
}

//282
float CGccGeometry::CalcDistance(VERTEX P1, VERTEX P2)
{
	float Result = sqrtf((P2.x - P1.x) * (P2.x - P1.x) +
		(P2.y - P1.y) * (P2.y - P1.y) +
		(P2.z - P1.z) * (P2.z - P1.z));
	return Result;
}

//290
float CGccGeometry::CalcLinePlaneAngle(VERTEX P1, VERTEX P2, VERTEX PlaneNormal)
{
	VERTEX LineVector = CalcLineVector(P1, P2);

	float Result = 90.f - (CalcVectorInnerProduct(LineVector, PlaneNormal) * __RADIAN);
	return Result;
}

//300
float CGccGeometry::CalcLineAngle2D(float P1[2], float P2[2])
{
	float x = ZeroDivisor(P2[0] - P1[0]);
	float y = (P2[1] - P1[1]);

	float Result = atanf(y / x) * 180.f / (float)M_PI;
	return Result;
}

void CGccGeometry::CalcNormalVector(VERTEX P1, VERTEX P2, VERTEX P3, VERTEX& Normal)
{
	float v1[3];
	float v2[3];

	v1[0] = P1.x - P2.x;
	v1[1] = P1.y - P2.y;
	v1[2] = P1.z - P2.z;

	v2[0] = P2.x - P3.x;
	v2[1] = P2.y - P3.y;
	v2[2] = P2.z - P3.z;

	Normal.x = v1[1] * v2[2] - v1[2] * v2[1];
	Normal.y = v1[2] * v2[0] - v1[0] * v2[2];
	Normal.z = v1[0] * v2[1] - v1[1] * v2[0];

	ReduceToUnit(Normal);
}

//341
VERTEX CGccGeometry::CalcLineVector(VERTEX V1, VERTEX V2)
{
	VERTEX Vector;
	Vector.x = V2.x - V1.x;
	Vector.y = V2.y - V1.y;
	Vector.z = V2.z - V1.z;
	return Vector;
}

//351
float CGccGeometry::CalcVectorInnerProduct(VERTEX vector1, VERTEX vector2)
{
	float vSum = vector1.x * vector2.x + vector1.y * vector2.y + vector1.z * vector2.z;
	float vMul = sqrtf(vector1.x * vector1.x +
		vector1.y * vector1.y +
		vector1.z * vector1.z) *
		sqrtf(vector2.x * vector2.x +
			vector2.y * vector2.y +
			vector2.z * vector2.z);

	float Result = acosf(vSum / ZeroDivisor(vMul));
	return Result;
}

//362
VERTEX CGccGeometry::CalcFootOfPerpendicular(VERTEX Point, float PlaneEquation[4])
{
	float temp = (PlaneEquation[3] -
		PlaneEquation[0] * Point.x -
		PlaneEquation[1] * Point.y -
		PlaneEquation[2] * Point.z) /
		ZeroDivisor(PlaneEquation[0] * PlaneEquation[0] +
			PlaneEquation[1] * PlaneEquation[1] +
			PlaneEquation[2] * PlaneEquation[2]);

	VERTEX Result;
	Result.x = PlaneEquation[0] * temp + Point.x;
	Result.y = PlaneEquation[1] * temp + Point.y;
	Result.z = PlaneEquation[2] * temp + Point.z;
	return Result;
}

//376
int CGccGeometry::IsCrossLinePlane(VERTEX LineP1, VERTEX LineP2, float PlaneEquation[4])
{
	bool cross1 = false;
	bool cross2 = false;

	if ((LineP1.x * PlaneEquation[0] +
		LineP1.y * PlaneEquation[1] +
		LineP1.z * PlaneEquation[2]) == PlaneEquation[3])
	{
		cross1 = true;
	}

	if ((LineP2.x * PlaneEquation[0] +
		LineP2.y * PlaneEquation[1] +
		LineP2.z * PlaneEquation[2]) == PlaneEquation[3])
	{
		cross2 = true;
	}

	if (cross1 && cross2)
	{
		return POINT_INCLUDE;
	}
	else if (cross1)
	{
		return POINT_CONTACT1;
	}
	else if (cross2)
	{
		return POINT_CONTACT2;
	}

	float deg = CalcLinePlaneAngle(LineP1, LineP2,
		VERTEX(PlaneEquation[0], PlaneEquation[1], PlaneEquation[2]));

	if (0.f == deg)
	{
		return POINT_ZERO;
	}

	VERTEX FOP1 = CalcFootOfPerpendicular(LineP1, PlaneEquation);
	VERTEX FOP2 = CalcFootOfPerpendicular(LineP2, PlaneEquation);

	VERTEX Vector1 = CalcLineVector(LineP1, FOP1);
	VERTEX Vector2 = CalcLineVector(LineP2, FOP2);

	if (Vector1.x * Vector2.x >= 0 &&
		Vector1.y * Vector2.y >= 0 &&
		Vector1.z * Vector2.z >= 0)
	{
		return POINT_ZERO;
	}

	return POINT_CROSS;
}

//420
bool CGccGeometry::GetIntersectionPointLinePlane(VERTEX LineP1, VERTEX LineP2, float PlaneEquation[4], VERTEX& IntersectionPoint)
{
	float deg = CalcLinePlaneAngle(LineP1, LineP2,
		VERTEX(PlaneEquation[0], PlaneEquation[1], PlaneEquation[2]));

	if (0.f == deg)
	{
		return false;
	}

	VERTEX lineVector = CalcLineVector(LineP1, LineP2);

	float temp = (PlaneEquation[3] -
		LineP1.x * PlaneEquation[0] -
		LineP1.y * PlaneEquation[1] -
		LineP1.z * PlaneEquation[2]) /
		(PlaneEquation[0] * lineVector.x +
			PlaneEquation[1] * lineVector.y +
			PlaneEquation[2] * lineVector.z);

	IntersectionPoint.x = lineVector.x * temp + LineP1.x;
	IntersectionPoint.y = lineVector.y * temp + LineP1.y;
	IntersectionPoint.z = lineVector.z * temp + LineP1.z;

	return true;
}

//439
void CGccGeometry::GetOutlinePoints(VERTEX* VertexList, int VertexLength, FACE* FaceList, int FaceLength, VERTEX** GirthList, int GirthLength[4], float Hight)
{
	if (0 == VertexLength)
	{
		return;
	}

	float PlaneEqu[4] = { 0, 1, 0, Hight };
	VERTEX* List = nullptr;
	VERTEX* ListTemp = nullptr;
	VERTEX Temp;
	VERTEX P1, P2;
	int Length = 0;
	VERTEX Min, Max;

	for (int i = 0; i < FaceLength; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			P1 = VertexList[FaceList[i].List[j]];
			P2 = VertexList[FaceList[i].List[3 == j + 1 ? 0 : j + 1]];

			switch (IsCrossLinePlane(P1, P2, PlaneEqu))
			{
			case POINT_ZERO:
				break;

			case POINT_CROSS:
			{
				bool bGet = GetIntersectionPointLinePlane(P1, P2, PlaneEqu, Temp);

				if (bGet)
				{
					//
				}

				if (List)
				{
					ListTemp = new VERTEX[Length];
					memcpy(ListTemp, List, sizeof(VERTEX) * Length);

					delete[] List;
					List = nullptr;

					List = new VERTEX[Length + 1];
					memcpy(List, ListTemp, sizeof(VERTEX) * Length);

					delete[] ListTemp;
					ListTemp = nullptr;
				}
				else
				{
					List = new VERTEX[Length + 1];
				}

				List[Length] = Temp;
				Length++;
			}
			break;

			case POINT_CONTACT1:
				if (List)
				{
					ListTemp = new VERTEX[Length];
					memcpy(ListTemp, List, sizeof(VERTEX) * Length);

					delete[] List;
					List = nullptr;

					List = new VERTEX[Length + 1];
					memcpy(List, ListTemp, sizeof(VERTEX) * Length);

					delete[] ListTemp;
					ListTemp = nullptr;
				}
				else
				{
					List = new VERTEX[Length + 1];
				}

				List[Length] = P1;
				Length++;
				break;

			case POINT_CONTACT2:
				if (List)
				{
					ListTemp = new VERTEX[Length];
					memcpy(ListTemp, List, sizeof(VERTEX) * Length);

					delete[] List;
					List = nullptr;

					List = new VERTEX[Length + 1];
					memcpy(List, ListTemp, sizeof(VERTEX) * Length);

					delete[] ListTemp;
					ListTemp = nullptr;
				}
				else
				{
					List = new VERTEX[Length + 1];
				}

				List[Length] = P2;
				Length++;
				break;

			case POINT_INCLUDE:
				if (List)
				{
					ListTemp = new VERTEX[Length];
					memcpy(ListTemp, List, sizeof(VERTEX) * Length);

					delete[] List;
					List = nullptr;

					List = new VERTEX[Length + 2];
					memcpy(List, ListTemp, sizeof(VERTEX) * Length);

					delete[] ListTemp;
					ListTemp = nullptr;
				}
				else
				{
					List = new VERTEX[Length + 2];
				}

				List[Length] = P1;
				List[Length + 1] = P2;
				Length += 2;

				break;
			}
		}
	}

	if (Length <= 1)
	{
		if (*GirthList)
		{
			delete[] * GirthList;
			*GirthList = nullptr;
		}

		for (int i = 0; i < 4; i++)
		{
			GirthLength[i] = 0;
		}

		if (List)
		{
			delete[] List;
			List = nullptr;
		}

		return;
	}

	SortOutlinePoints(&List, Length);

	if (*GirthList)
	{
		delete[] * GirthList;
		*GirthList = nullptr;
	}

	*GirthList = new VERTEX[Length];
	memcpy(*GirthList, List, sizeof(VERTEX) * Length);

	OutlineDivision(List, Length, GirthLength);

	SortOutlineList(*GirthList, GirthLength, Length);

	if (List)
	{
		delete[] List;
		List = nullptr;
	}
}

//564
void CGccGeometry::SortOutlinePoints(VERTEX** PointList, int& Length)
{
	if (0 == Length)
	{
		return;
	}

	int LenTemp = Length;
	VERTEX* List = new VERTEX[LenTemp];
	VERTEX* TempList = nullptr;
	memcpy(List, *PointList, sizeof(VERTEX) * LenTemp);
	VERTEX Temp;

	for (int i = 0; i < LenTemp; i++)
	{
		for (int j = 1; j < LenTemp - i; j++)
		{
			if ((List[j - 1].x == List[j].x &&
				List[j - 1].y == List[j].y &&
				List[j - 1].z == List[j].z))
			{
				TempList = new VERTEX[LenTemp - j];
				memcpy(TempList, List + j + 1, sizeof(VERTEX) * (LenTemp - j - 1));
				memcpy(List + j, TempList, sizeof(VERTEX) * (LenTemp - j - 1));

				delete[] TempList;
				TempList = nullptr;

				LenTemp--;
				j--;

				continue;
			}

			if (List[j - 1].x > List[j].x)
			{
				Swap(List[j - 1], List[j]);
			}
		}
	}

	if (*PointList != nullptr)
	{
		delete[] * PointList;
		*PointList = nullptr;
	}

	*PointList = new VERTEX[LenTemp];
	memcpy(*PointList, List, sizeof(VERTEX) * LenTemp);
	Length = LenTemp;

	delete[] List;
	List = nullptr;
}

// 605
void CGccGeometry::OutlineDivision(VERTEX* GirthList, int PointLength, int GirthLength[4])
{
	int Length = ZeroDivisor(PointLength);

	for (int i = 0; i < 4; i++)
	{
		GirthLength[i] = 0;
	}

	if (Length < 20)
	{
		GirthLength[0] = Length;
		GirthLength[1] = 0;
		GirthLength[2] = 0;
		GirthLength[3] = 0;
		return;
	}

	float Max[4] = { 5.f };
	Max[0] = 5.f;
	Max[1] = 5.f;
	Max[2] = 5.f;
	Max[3] = 5.f;
	float Dis = 0.f;
	float Temp = 0.f;

	for (int i = 1; i < Length; i++)
	{
		Dis = fabsf(GirthList[i].x - GirthList[i - 1].x);

		for (int j = 3; j >= 0; j--)
		{
			if (Max[j] < Dis)
			{
				Temp = Max[j];
				Max[j] = Dis;
				Dis = Temp;
			}
		}
	}

	Dis = fabsf(GirthList[Length - 1].x - GirthList[0].x) / (Length / 3.f);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 1; j < 4 - i; j++)
		{
			if (fabs(Max[j] - Max[j - 1]) < Dis)
			{
				Max[j - 1] = Max[j];
			}
		}
	}

	int Count = 0;

	for (int i = 1; i < Length; i++)
	{
		if (fabs(GirthList[i].x - GirthList[i - 1].x) > Max[0])
		{
			GirthLength[Count] = i;
			Count++;

			if (4 == Count)
			{
				break;
			}
		}
	}

	if (Count != 4)
	{
		GirthLength[Count] = Length;
	}
}

//664
void CGccGeometry::SortOutlineList(VERTEX* GirthList, int* GirthLength, int TotalLength)
{
	VERTEX* vList[4] = { nullptr };
	int Length[4] = { 0 };
	Length[0] = GirthLength[0];
	Length[1] = GirthLength[1] == 0 ? 0 : GirthLength[1] - GirthLength[0];
	Length[2] = GirthLength[2] == 0 ? 0 : GirthLength[2] - GirthLength[1];
	Length[3] = GirthLength[3] == 0 ? 0 : GirthLength[3] - GirthLength[2];
	int nLastGirth = 4;

	for (int i = 0; i < 4; i++)
	{
		if (Length[i] != 0)
		{
			vList[i] = new VERTEX[Length[i]];
			memcpy(vList[i], GirthList + (0 == i ? 0 : GirthLength[i - 1]), sizeof(VERTEX) * Length[i]);
		}
		else
		{
			vList[i] = nullptr;
		}
	}

	for (int i = 0; i < 4; i++)
	{
		if (0 == Length[i])
		{
			break;
		}

		SortByNearPoint(vList[i], Length[i]);
	}

	for (int i = 3; i >= 0; i--)
	{
		if (0 == Length[i])
		{
			nLastGirth = i - 1;
		}
	}

	int temp = 0;
	int addLength = 0;

	if (nLastGirth < 3)
	{
		for (int i = 10; i < Length[nLastGirth] - 1; i++)
		{
			if (CalcDistance(vList[nLastGirth][0], vList[nLastGirth][i]) <
				CalcDistance(vList[nLastGirth][i], vList[nLastGirth][i + 1]))
			{
				temp = Length[nLastGirth];
				Length[nLastGirth] = i + 1;
				addLength = temp - Length[nLastGirth];

				if (addLength > 5)
				{
					vList[nLastGirth + 1] = new VERTEX[addLength];
					Length[nLastGirth + 1] = addLength;
					memcpy(vList[nLastGirth + 1], vList[nLastGirth] + i + 1, sizeof(VERTEX) * addLength);

					GirthLength[nLastGirth + 1] = GirthLength[nLastGirth];
					GirthLength[nLastGirth] = GirthLength[nLastGirth] - Length[nLastGirth + 1];

					break;
				}
				else
				{
					GirthLength[nLastGirth] = GirthLength[nLastGirth] - addLength;

					break;
				}
			}
		}
	}

	for (int i = 0; i < 4; i++)
	{
		if (Length[i] != 0)
		{
			memcpy(GirthList + (0 == i ? 0 : GirthLength[i - 1]), vList[i], sizeof(VERTEX) * Length[i]);
		}
	}

	for (int i = 0; i < 4; i++)
	{
		if (vList[i])
		{
			delete[] vList[i];
			vList[i] = nullptr;
		}
	}
}

//780
void CGccGeometry::Swap(VERTEX& A, VERTEX& B)
{
	VERTEX Temp;
	Temp = A;
	A = B;
	B = Temp;
}

//796
void CGccGeometry::SortByNearPoint(VERTEX* List, int Length)
{
	VERTEX* Temp = nullptr;
	VERTEX Key;
	int lastLength = Length - 1;
	int Near = 0;
	VERTEX* Copy = new VERTEX[lastLength];
	memcpy(Copy, List + 1, sizeof(VERTEX) * lastLength);

	for (int i = 1; i < Length; i++)
	{
		Near = 0;
		Key = List[i - 1];

		for (int j = 1; j < lastLength; j++)
		{
			if (CalcDistance(Key, Copy[Near]) > CalcDistance(Key, Copy[j]))
			{
				Near = j;
			}
		}

		List[i] = Copy[Near];

		if (lastLength - 1 != Near)
		{
			Temp = new VERTEX[lastLength - Near];
			memcpy(Temp, Copy + Near + 1, sizeof(VERTEX) * (lastLength - Near - 1));
			memcpy(Copy + Near, Temp, sizeof(VERTEX) * (lastLength - Near - 1));

			delete[] Temp;
			Temp = nullptr;
		}

		lastLength--;
	}

	delete[] Copy;
	Copy = nullptr;

	delete[] Temp;
	Temp = nullptr;
}

//831
float CGccGeometry::CalcGirth(VERTEX* List, int Length)
{
	if (nullptr == List)
	{
		return 0.f;
	}

	float Result = 0.f;

	if (0 == Length)
	{
		return 0.f;
	}
	else if (1 == Length)
	{
		return (float)M_PI;
	}

	for (int i = 0; i < Length - 1; i++)
	{
		Result += CalcDistance(List[i], List[i + 1]);
	}

	Result += CalcDistance(List[0], List[Length - 1]);

	return Result;
}

//851
float CGccGeometry::CalcAreaOfPolygon(VERTEX* List, int Length)
{
	if (nullptr == List)
	{
		return 0.f;
	}

	float Result = 0.f;

	if (0 == Length)
	{
		return 0.f;
	}
	else if (1 == Length)
	{
		return (float)M_PI;
	}

	VERTEX Mid, Max, Min;
	GetMinMax(List, Length, Min, Max);

	Mid.x = Min.x + fabs(Max.x - Min.x) / 2.f;
	Mid.y = Min.y + fabs(Max.y - Min.y) / 2.f;
	Mid.z = Min.z + fabs(Max.z - Min.z) / 2.f;

	for (int i = 0; i < Length - 1; i++)
	{
		Result += (List[i + 1].x - List[i].x) * (((List[i].z - Mid.z) + (List[i + 1].z - Mid.z)) / 2);
	}

	Result += (List[0].x - List[Length - 1].x) * (((List[Length - 1].z - Mid.z) + (List[0].z - Mid.z)) / 2);

	return fabs(Result);
}

//1029
void CGccGeometry::CalcPartsVolumeNSurface(VERTEX* VertexList, int VertexLength, FACE* FaceList, int FaceLength, VERTEX PartsPt[8], float Volume[6][2], float Surface[6][2])
{
	VERTEX Min, Max;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			Volume[j][i] = 0.f;
			Surface[j][i] = 0.f;
		}
	}

	if (nullptr == VertexList || 0 == VertexLength)
	{
		return;
	}

	int UnitLength[4] = { 0 };

	GetMinMax(VertexList, VertexLength, Min, Max);

	Max.y = std::min(Max.y, PartsPt[0].y);
	int HeightMin = (int)(Min.y + 0.5f);
	int HeightMax = (int)(Max.y - 0.5f);
	int TotalListLength = (int)(HeightMax - HeightMin);

	VERTEX P1, P2, Vector, insert;
	int U1, U2 = 0;

	VERTEX** GirthList = nullptr;
	GirthList = new VERTEX*[TotalListLength];

	int* GirthLength = nullptr;
	GirthLength = new int[TotalListLength];

	memset(GirthLength, 0, sizeof(int) * TotalListLength);

	for (int i = 0; i < FaceLength; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			P1 = VertexList[FaceList[i].List[j]];
			P2 = VertexList[FaceList[i].List[3 == j + 1 ? 0 : j + 1]];

			if (!((P1.y >= HeightMin && P1.y <= HeightMax) || (P2.y >= HeightMin && P2.y <= HeightMax)))
			{
				continue;
			}

			Vector = CalcLineVector(P1, P2);

			U1 = (int)(P1.y >= P2.y ? P1.y : P2.y) - HeightMin;
			U2 = (int)(P1.y <= P2.y ? P1.y : P2.y) - HeightMin;

			if (U2 == U1)
			{
				if (U1 < 0 || U1 > TotalListLength)
				{
					continue;
				}
			}

			if (U2 < 0)
			{
				U2 = 0;
			}

			if (U1 > TotalListLength)
			{
				U1 = TotalListLength;
			}

			for (int k = U2; k < U1; k++)
			{
				insert = GetPointOnLineForY(Vector, P1, (float)(k + HeightMin));
				InsertSortX(&GirthList[k], insert, GirthLength[k]);
			}
		}
	}

	int KneeL, KneeR, Croutch, Waist, Chest, KneeMin, KneeMax = 0;

	Chest = (int)(PartsPt[3].y - HeightMin);
	Waist = (int)(PartsPt[4].y - HeightMin);
	Croutch = (int)(PartsPt[5].y - HeightMin);
	KneeR = (int)(PartsPt[6].y - HeightMin);
	KneeL = (int)(PartsPt[7].y - HeightMin);

	KneeMin = std::min(KneeL, KneeR);
	KneeMax = std::max(KneeL, KneeR);

	int GirthCount = 0;

	for (int i = 0; i < TotalListLength; i++)
	{
		if (GirthLength[i] == 0)
		{
			continue;
		}

		OutlineDivision(GirthList[i], GirthLength[i], UnitLength);
		SortOutlineList(GirthList[i], UnitLength, GirthLength[i]);

		GirthCount = 0;

		for (int j = 0; j < 4; j++)
		{
			GirthCount += UnitLength[j] != 0 ? 1 : 0;
		}

		if (i < KneeR)
		{
			Volume[4][0] += UnitLength[0] == 0 ? 0 : CalcAreaOfPolygon(GirthList[i], UnitLength[0]);
			Surface[4][0] += UnitLength[0] == 0 ? 0 : CalcGirth(GirthList[i], UnitLength[0]);
		}

		if (i < KneeL)
		{
			Volume[4][1] += UnitLength[1] == 0 ? 0 : CalcAreaOfPolygon(GirthList[i] + UnitLength[1 - 1], UnitLength[1] - UnitLength[1 - 1]);
			Surface[4][1] += UnitLength[1] == 0 ? 0 : CalcGirth(GirthList[i] + UnitLength[1 - 1], UnitLength[1] - UnitLength[1 - 1]);
		}

		if (i > KneeMin && i < KneeMax)
		{
			if (KneeMax == KneeR)
			{
				Volume[3][0] += UnitLength[0] == 0 ? 0 : CalcAreaOfPolygon(GirthList[i], UnitLength[0]);
				Surface[3][0] += UnitLength[0] == 0 ? 0 : CalcGirth(GirthList[i], UnitLength[0]);
			}

			if (KneeMax == KneeL)
			{
				Volume[3][1] += UnitLength[1] == 0 ? 0 : CalcAreaOfPolygon(GirthList[i] + UnitLength[1 - 1], UnitLength[1] - UnitLength[1 - 1]);
				Surface[3][1] += UnitLength[1] == 0 ? 0 : CalcGirth(GirthList[i] + UnitLength[1 - 1], UnitLength[1] - UnitLength[1 - 1]);
			}
		}

		if (i >= KneeMax && i < Croutch)
		{
			if (1 == GirthCount)
			{
				VERTEX vMax, vMin, vMid;

				GetMinMax(GirthList[i], UnitLength[0], vMin, vMax);

				vMid.x = vMin.x + (vMax.x - vMin.x) / 2.f;

				DivideBasedOnPoint(&GirthList[i], GirthLength[i], UnitLength, vMid, 0);

				Surface[3][0] += UnitLength[0] == 0 ? 0 : CalcGirth(GirthList[i], UnitLength[0]);
				Volume[3][0] += UnitLength[0] == 0 ? 0 : CalcAreaOfPolygon(GirthList[i], UnitLength[0]);

				Surface[3][1] += UnitLength[1] == 0 ? 0 : CalcGirth(GirthList[i] + UnitLength[1 - 1], UnitLength[1] - UnitLength[1 - 1]);
				Volume[3][1] += UnitLength[1] == 0 ? 0 : CalcAreaOfPolygon(GirthList[i] + UnitLength[1 - 1], UnitLength[1] - UnitLength[1 - 1]);

				continue;
			}
			else if (2 == GirthCount)
			{
				float v1, v2;
				v1 = CalcGirth(GirthList[i], UnitLength[0]);
				v2 = CalcGirth(GirthList[i] + UnitLength[1 - 1], UnitLength[1] - UnitLength[1 - 1]);
				float tempMax = std::max(v1, v2);
				float tempMin = std::min(v1, v2);

				if (tempMax > tempMin * 1.5) //두덩이의 차이가 클때
				{
					if (tempMax == v1) //첫번째가 클때
					{
						VERTEX vMax, vMin, vMid;
						GetMinMax(GirthList[i], UnitLength[0], vMin, vMax);
						vMid.x = vMin.x + (vMax.x - vMin.x) / 2.f;
						DivideBasedOnPoint(&GirthList[i], GirthLength[i], UnitLength, vMid, 0);

						Surface[3][0] += UnitLength[0] == 0 ? 0 : CalcGirth(GirthList[i], UnitLength[0]);
						Volume[3][0] += UnitLength[0] == 0 ? 0 : CalcAreaOfPolygon(GirthList[i], UnitLength[0]);

						Surface[3][1] += UnitLength[1] == 0 ? 0 : CalcGirth(GirthList[i] + UnitLength[1 - 1], UnitLength[1] - UnitLength[1 - 1]);
						Volume[3][1] += UnitLength[1] == 0 ? 0 : CalcAreaOfPolygon(GirthList[i] + UnitLength[1 - 1], UnitLength[1] - UnitLength[1 - 1]);

						Surface[5][1] += UnitLength[2] == 0 ? 0 : CalcGirth(GirthList[i] + UnitLength[2 - 1], UnitLength[2] - UnitLength[2 - 1]);
						Volume[5][1] += UnitLength[2] == 0 ? 0 : CalcAreaOfPolygon(GirthList[i] + UnitLength[2 - 1], UnitLength[2] - UnitLength[2 - 1]);

						continue;
					}
					else //두번째가 클때
					{
						//두번째 분할
						VERTEX vMax, vMin, vMid;

						GetMinMax(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0], vMin, vMax);

						vMid.x = vMin.x + (vMax.x - vMin.x) / 2.f;

						DivideBasedOnPoint(&GirthList[i], GirthLength[i], UnitLength, vMid, 1);

						Surface[5][0] += UnitLength[0] == 0 ? 0 : CalcGirth(GirthList[i], UnitLength[0]);
						Volume[5][0] += UnitLength[0] == 0 ? 0 : CalcAreaOfPolygon(GirthList[i], UnitLength[0]);

						Surface[3][0] += UnitLength[1] == 0 ? 0 : CalcGirth(GirthList[i] + UnitLength[1 - 1], UnitLength[1] - UnitLength[1 - 1]);
						Volume[3][0] += UnitLength[1] == 0 ? 0 : CalcAreaOfPolygon(GirthList[i] + UnitLength[1 - 1], UnitLength[1] - UnitLength[1 - 1]);

						Surface[3][1] += UnitLength[2] == 0 ? 0 : CalcGirth(GirthList[i] + UnitLength[2 - 1], UnitLength[2] - UnitLength[2 - 1]);
						Volume[3][1] += UnitLength[2] == 0 ? 0 : CalcAreaOfPolygon(GirthList[i] + UnitLength[2 - 1], UnitLength[2] - UnitLength[2 - 1]);

						continue;
					}
				}
				else
				{
					Surface[3][0] += UnitLength[0] == 0 ? 0 : CalcGirth(GirthList[i], UnitLength[0]);
					Volume[3][0] += UnitLength[0] == 0 ? 0 : CalcAreaOfPolygon(GirthList[i], UnitLength[0]);

					Surface[3][1] += UnitLength[1] == 0 ? 0 : CalcGirth(GirthList[i] + UnitLength[1 - 1], UnitLength[1] - UnitLength[1 - 1]);
					Volume[3][1] += UnitLength[1] == 0 ? 0 : CalcAreaOfPolygon(GirthList[i] + UnitLength[1 - 1], UnitLength[1] - UnitLength[1 - 1]);

					continue;
				}
			}
			else if (3 == GirthCount)
			{
				float g1, g2, g3, gmin, m1, m2, m3, mmin, mm1, mm2, mm3, mmmin = 0.;
				g1 = UnitLength[0] == 0 ? 0 : CalcGirth(GirthList[i], UnitLength[0]);
				g2 = UnitLength[1] == 0 ? 0 : CalcGirth(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0]);
				g3 = UnitLength[2] == 0 ? 0 : CalcGirth(GirthList[i] + UnitLength[1], UnitLength[2] - UnitLength[1]);
				m1 = fabs(g1 - g2);
				m2 = fabs(g2 - g3);
				m3 = fabs(g1 - g3);
				mm1 = fabs(m1 - m2);
				mm2 = fabs(m2 - m3);
				mm3 = fabs(m1 - m3);
				mmin = std::min(m1, std::min(m2, m3));
				gmin = std::min(g1, std::min(g2, g3));
				mmmin = std::min(mm1, std::min(mm2, mm3));

				if (mmin == m3 && mmmin == mm1) //큰거하나 작은거 둘
				{
					VERTEX vMax, vMin, vMid;

					GetMinMax(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0], vMin, vMax);

					vMid.x = vMin.x + (vMax.x - vMin.x) / 2.f;

					DivideBasedOnPoint(&GirthList[i], GirthLength[i], UnitLength, vMid, 1);

					Surface[5][0] += UnitLength[0] == 0 ? 0 : CalcGirth(GirthList[i], UnitLength[0]);
					Volume[5][0] += UnitLength[0] == 0 ? 0 : CalcAreaOfPolygon(GirthList[i], UnitLength[0]);

					Surface[3][0] += UnitLength[1] == 0 ? 0 : CalcGirth(GirthList[i] + UnitLength[1], UnitLength[1] - UnitLength[0]);
					Volume[3][0] += UnitLength[1] == 0 ? 0 : CalcAreaOfPolygon(GirthList[i] + UnitLength[1 - 1], UnitLength[1] - UnitLength[0]);

					Surface[3][1] += UnitLength[2] == 0 ? 0 : CalcGirth(GirthList[i] + UnitLength[1], UnitLength[2] - UnitLength[1]);
					Volume[3][1] += UnitLength[2] == 0 ? 0 : CalcAreaOfPolygon(GirthList[i] + UnitLength[2 - 1], UnitLength[2] - UnitLength[1]);

					Surface[5][1] += UnitLength[3] == 0 ? 0 : CalcGirth(GirthList[i] + UnitLength[2], UnitLength[3] - UnitLength[2]);
					Volume[5][1] += UnitLength[3] == 0 ? 0 : CalcAreaOfPolygon(GirthList[i] + UnitLength[2], UnitLength[3] - UnitLength[2]);

					continue;
				}
				else //작은거 하나 큰거둘
				{
					if (gmin == g1)
					{
						Surface[5][0] += g1;
						Volume[5][0] += UnitLength[0] == 0 ? 0 : CalcAreaOfPolygon(GirthList[i], UnitLength[0]);

						Surface[3][0] += g2;
						Volume[3][0] += UnitLength[1] == 0 ? 0 : CalcAreaOfPolygon(GirthList[i] + UnitLength[1 - 1], UnitLength[1] - UnitLength[1 - 1]);

						Surface[3][1] += g3;
						Volume[3][1] += UnitLength[2] == 0 ? 0 : CalcAreaOfPolygon(GirthList[i] + UnitLength[2 - 1], UnitLength[2] - UnitLength[2 - 1]);

						continue;
					}
					else
					{
						Surface[3][0] += g1;
						Volume[3][0] += UnitLength[0] == 0 ? 0 : CalcAreaOfPolygon(GirthList[i], UnitLength[0]);

						Surface[3][1] += g2;
						Volume[3][1] += UnitLength[1] == 0 ? 0 : CalcAreaOfPolygon(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0]);

						Surface[5][1] += g3;
						Volume[5][1] += UnitLength[2] == 0 ? 0 : CalcAreaOfPolygon(GirthList[i] + UnitLength[1], UnitLength[2] - UnitLength[1]);

						continue;
					}
				}
			}
			else if (4 == GirthCount)
			{
				Surface[5][0] += UnitLength[0] == 0 ? 0 : CalcGirth(GirthList[i], UnitLength[0]);
				Volume[5][0] += UnitLength[0] == 0 ? 0 : CalcAreaOfPolygon(GirthList[i], UnitLength[0]);

				Surface[3][0] += UnitLength[1] == 0 ? 0 : CalcGirth(GirthList[i] + UnitLength[1 - 1], UnitLength[1] - UnitLength[1 - 1]);
				Volume[3][0] += UnitLength[1] == 0 ? 0 : CalcAreaOfPolygon(GirthList[i] + UnitLength[1 - 1], UnitLength[1] - UnitLength[1 - 1]);

				Surface[3][1] += UnitLength[2] == 0 ? 0 : CalcGirth(GirthList[i] + UnitLength[2 - 1], UnitLength[2] - UnitLength[2 - 1]);
				Volume[3][1] += UnitLength[2] == 0 ? 0 : CalcAreaOfPolygon(GirthList[i] + UnitLength[2 - 1], UnitLength[2] - UnitLength[2 - 1]);

				Surface[5][1] += UnitLength[3] == 0 ? 0 : CalcGirth(GirthList[i] + UnitLength[3 - 1], UnitLength[3] - UnitLength[3 - 1]);
				Volume[5][1] += UnitLength[3] == 0 ? 0 : CalcAreaOfPolygon(GirthList[i] + UnitLength[3 - 1], UnitLength[3] - UnitLength[3 - 1]);

				continue;
			}
		}

		if (i >= Croutch && i < Waist)
		{
			if (1 == GirthCount)
			{
				VERTEX vMax, vMin, vMid;
				GetMinMax(GirthList[i], UnitLength[0], vMin, vMax);
				vMid.x = vMin.x + (vMax.x - vMin.x) / 2.f;
				DivideBasedOnPoint(&GirthList[i], GirthLength[i], UnitLength, vMid, 0);

				Surface[2][0] += CalcGirth(GirthList[i], UnitLength[0]);
				Volume[2][0] += CalcAreaOfPolygon(GirthList[i], UnitLength[0]);

				Surface[2][1] += CalcGirth(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0]);
				Volume[2][1] += CalcAreaOfPolygon(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0]);

				continue;
			}
			else if (2 == GirthCount)
			{
				float v1, v2;
				v1 = CalcGirth(GirthList[i], UnitLength[0]);
				v2 = CalcGirth(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0]);
				float tempMax = std::max(v1, v2);
				float tempMin = std::min(v1, v2);

				if (tempMax > tempMin * 1.5) //두덩이의 차이가 클때
				{
					if (tempMax == v1) //첫번째가 클때
					{
						VERTEX vMax, vMin, vMid;

						GetMinMax(GirthList[i], UnitLength[0], vMin, vMax);

						vMid.x = vMin.x + (vMax.x - vMin.x) / 2;

						DivideBasedOnPoint(&GirthList[i], GirthLength[i], UnitLength, vMid, 0);

						Surface[2][0] += CalcGirth(GirthList[i], UnitLength[0]);
						Volume[2][0] += CalcAreaOfPolygon(GirthList[i], UnitLength[0]);

						Surface[2][1] += CalcGirth(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0]);
						Volume[2][1] += CalcAreaOfPolygon(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0]);

						Surface[5][1] += CalcGirth(GirthList[i] + UnitLength[1], UnitLength[2] - UnitLength[1]);
						Volume[5][1] += CalcAreaOfPolygon(GirthList[i] + UnitLength[1], UnitLength[2] - UnitLength[1]);

						continue;
					}
					else //두번째가 클때
					{
						//두번째 분할
						VERTEX vMax, vMin, vMid;

						GetMinMax(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0], vMin, vMax);

						vMid.x = vMin.x + (vMax.x - vMin.x) / 2;

						DivideBasedOnPoint(&GirthList[i], GirthLength[i], UnitLength, vMid, 1);

						Surface[5][0] += CalcGirth(GirthList[i], UnitLength[0]);
						Volume[5][0] += CalcAreaOfPolygon(GirthList[i], UnitLength[0]);

						Surface[2][0] += CalcGirth(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0]);
						Volume[2][0] += CalcAreaOfPolygon(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0]);

						Surface[2][1] += CalcGirth(GirthList[i] + UnitLength[1], UnitLength[2] - UnitLength[1]);
						Volume[2][1] += CalcAreaOfPolygon(GirthList[i] + UnitLength[1], UnitLength[2] - UnitLength[1]);

						continue;
					}
				}
				else
				{
					Surface[2][0] += CalcGirth(GirthList[i], UnitLength[0]);
					Volume[2][0] += CalcAreaOfPolygon(GirthList[i], UnitLength[0]);

					Surface[2][1] += CalcGirth(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0]);
					Volume[2][1] += CalcAreaOfPolygon(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0]);

					continue;
				}
			}
			else if (3 == GirthCount)
			{
				float g1, g2, g3, gmin, m1, m2, m3, mmin, mm1, mm2, mm3, mmmin;
				g1 = (CalcGirth(GirthList[i], UnitLength[0]));
				g2 = (CalcGirth(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0]));
				g3 = (CalcGirth(GirthList[i] + UnitLength[1], UnitLength[2] - UnitLength[1]));

				m1 = fabs(g1 - g2);
				m2 = fabs(g2 - g3);
				m3 = fabs(g1 - g3);
				mm1 = fabs(m1 - m2);
				mm2 = fabs(m2 - m3);
				mm3 = fabs(m1 - m3);
				mmin = std::min(m1, std::min(m2, m3));
				gmin = std::min(g1, std::min(g2, g3));
				mmmin = std::min(mm1, std::min(mm2, mm3));

				if (mmin == m3 && mmmin == mm1) //큰거하나 작은거 둘
				{
					VERTEX vMax, vMin, vMid;

					GetMinMax(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0], vMin, vMax);

					vMid.x = vMin.x + (vMax.x - vMin.x) / 2;

					DivideBasedOnPoint(&GirthList[i], GirthLength[i], UnitLength, vMid, 1);

					Surface[5][0] += CalcGirth(GirthList[i], UnitLength[0]);
					Volume[5][0] += CalcAreaOfPolygon(GirthList[i], UnitLength[0]);

					Surface[2][0] += CalcGirth(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0]);
					Volume[2][0] += CalcAreaOfPolygon(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0]);

					Surface[2][1] += CalcGirth(GirthList[i] + UnitLength[1], UnitLength[2] - UnitLength[1]);
					Volume[2][1] += CalcAreaOfPolygon(GirthList[i] + UnitLength[1], UnitLength[2] - UnitLength[1]);

					Surface[5][1] += CalcGirth(GirthList[i] + UnitLength[2], UnitLength[3] - UnitLength[2]);
					Volume[5][1] += CalcAreaOfPolygon(GirthList[i] + UnitLength[2], UnitLength[3] - UnitLength[2]);

					continue;
				}
				else //작은거 하나 큰거둘
				{
					if (gmin == g1)
					{
						Surface[5][0] += g1;
						Volume[5][0] += CalcAreaOfPolygon(GirthList[i], UnitLength[0]);

						Surface[2][0] += g2;
						Volume[2][0] += CalcAreaOfPolygon(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0]);

						Surface[2][1] += g3;
						Volume[2][1] += CalcAreaOfPolygon(GirthList[i] + UnitLength[1], UnitLength[2] - UnitLength[1]);

						continue;
					}
					else
					{
						Surface[2][0] += g1;
						Volume[2][0] += CalcAreaOfPolygon(GirthList[i], UnitLength[0]);

						Surface[2][1] += g2;
						Volume[2][1] += CalcAreaOfPolygon(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0]);

						Surface[5][1] += g3;
						Volume[5][1] += CalcAreaOfPolygon(GirthList[i] + UnitLength[1], UnitLength[2] - UnitLength[1]);

						continue;
					}
				}
			}
			else if (4 == GirthCount)
			{
				Surface[5][0] += CalcGirth(GirthList[i], UnitLength[0]);
				Volume[5][0] += CalcAreaOfPolygon(GirthList[i], UnitLength[0]);

				Surface[2][0] += CalcGirth(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0]);
				Volume[2][0] += CalcAreaOfPolygon(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0]);

				Surface[2][1] += CalcGirth(GirthList[i] + UnitLength[1], UnitLength[2] - UnitLength[1]);
				Volume[2][1] += CalcAreaOfPolygon(GirthList[i] + UnitLength[1], UnitLength[2] - UnitLength[1]);

				Surface[5][1] += CalcGirth(GirthList[i] + UnitLength[2], UnitLength[3] - UnitLength[2]);
				Volume[5][1] += CalcAreaOfPolygon(GirthList[i] + UnitLength[2], UnitLength[3] - UnitLength[2]);

				continue;
			}
		}

		if (i >= Waist && i < Chest)
		{
			if (1 == GirthCount)
			{
				VERTEX vMax, vMin, vMid;

				GetMinMax(GirthList[i], UnitLength[0], vMin, vMax);

				vMid.x = vMin.x + (vMax.x - vMin.x) / 2;

				DivideBasedOnPoint(&GirthList[i], GirthLength[i], UnitLength, vMid, 0);

				Surface[1][0] += CalcGirth(GirthList[i], UnitLength[0]);
				Volume[1][0] += CalcAreaOfPolygon(GirthList[i], UnitLength[0]);

				Surface[1][1] += CalcGirth(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0]);
				Volume[1][1] += CalcAreaOfPolygon(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0]);

				continue;
			}
			else if (2 == GirthCount)
			{
				float v1, v2;
				v1 = CalcGirth(GirthList[i], UnitLength[0]);
				v2 = CalcGirth(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0]);

				float tempMax = std::max(v1, v2);

				if (tempMax == v1) //첫번째가 클때
				{
					VERTEX vMax, vMin, vMid;
					GetMinMax(GirthList[i], UnitLength[0], vMin, vMax);
					vMid.x = vMin.x + (vMax.x - vMin.x) / 2;
					DivideBasedOnPoint(&GirthList[i], GirthLength[i], UnitLength, vMid, 0);

					Surface[1][0] += CalcGirth(GirthList[i], UnitLength[0]);
					Volume[1][0] += CalcAreaOfPolygon(GirthList[i], UnitLength[0]);

					Surface[1][1] += CalcGirth(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0]);
					Volume[1][1] += CalcAreaOfPolygon(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0]);

					Surface[5][1] += CalcGirth(GirthList[i] + UnitLength[1], UnitLength[2] - UnitLength[1]);
					Volume[5][1] += CalcAreaOfPolygon(GirthList[i] + UnitLength[1], UnitLength[2] - UnitLength[1]);

					continue;
				}
				else //두번째가 클때
				{
					VERTEX vMax, vMin, vMid;

					GetMinMax(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0], vMin, vMax);

					vMid.x = vMin.x + (vMax.x - vMin.x) / 2;

					DivideBasedOnPoint(&GirthList[i], GirthLength[i], UnitLength, vMid, 1);

					Surface[5][0] += CalcGirth(GirthList[i], UnitLength[0]);
					Volume[5][0] += CalcAreaOfPolygon(GirthList[i], UnitLength[0]);

					Surface[1][0] += CalcGirth(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0]);
					Volume[1][0] += CalcAreaOfPolygon(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0]);

					Surface[1][1] += CalcGirth(GirthList[i] + UnitLength[1], UnitLength[2] - UnitLength[1]);
					Volume[1][1] += CalcAreaOfPolygon(GirthList[i] + UnitLength[1], UnitLength[2] - UnitLength[1]);

					continue;
				}
			}
			else if (3 == GirthCount)
			{
				VERTEX vMax, vMin, vMid;

				GetMinMax(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0], vMin, vMax);

				vMid.x = vMin.x + (vMax.x - vMin.x) / 2;

				DivideBasedOnPoint(&GirthList[i], GirthLength[i], UnitLength, vMid, 1);

				Surface[5][0] += CalcGirth(GirthList[i], UnitLength[0]);
				Volume[5][0] += CalcAreaOfPolygon(GirthList[i], UnitLength[0]);

				Surface[1][0] += CalcGirth(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0]);
				Volume[1][0] += CalcAreaOfPolygon(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0]);

				Surface[1][1] += CalcGirth(GirthList[i] + UnitLength[1], UnitLength[2] - UnitLength[1]);
				Volume[1][1] += CalcAreaOfPolygon(GirthList[i] + UnitLength[1], UnitLength[2] - UnitLength[1]);

				Surface[5][1] += CalcGirth(GirthList[i] + UnitLength[2], UnitLength[3] - UnitLength[2]);
				Volume[5][1] += CalcAreaOfPolygon(GirthList[i] + UnitLength[2], UnitLength[3] - UnitLength[2]);

				continue;
			}
			else if (4 == GirthCount)
			{
				Surface[5][0] += CalcGirth(GirthList[i], UnitLength[0]);
				Volume[5][0] += CalcAreaOfPolygon(GirthList[i], UnitLength[0]);

				Surface[1][0] += CalcGirth(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0]);
				Volume[1][0] += CalcAreaOfPolygon(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0]);

				Surface[1][1] += CalcGirth(GirthList[i] + UnitLength[1], UnitLength[2] - UnitLength[1]);
				Volume[1][1] += CalcAreaOfPolygon(GirthList[i] + UnitLength[1], UnitLength[2] - UnitLength[1]);

				Surface[5][1] += CalcGirth(GirthList[i] + UnitLength[2], UnitLength[3] - UnitLength[2]);
				Volume[5][1] += CalcAreaOfPolygon(GirthList[i] + UnitLength[2], UnitLength[3] - UnitLength[2]);

				continue;
			}
		}

		if (i >= Chest)
		{
			if (1 == GirthCount)
			{
				VERTEX vMax, vMin, vMid;
				bool left = false, right = false;

				GetMinMax(GirthList[i], UnitLength[0], vMin, vMax);

				if (vMin.x < PartsPt[1].x)
				{
					DivideBasedOnPoint(&GirthList[i], GirthLength[i], UnitLength, PartsPt[1], 0);
					left = true;
				}

				if (vMax.x > PartsPt[2].x)
				{
					if (left)
					{
						DivideBasedOnPoint(&GirthList[i], GirthLength[i], UnitLength, PartsPt[2], 1);
					}
					else
					{
						DivideBasedOnPoint(&GirthList[i], GirthLength[i], UnitLength, PartsPt[2], 0);
					}

					right = true;
				}

				if (left)
				{
					GetMinMax(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0], vMin, vMax);
				}
				else
				{
					GetMinMax(GirthList[i], UnitLength[0], vMin, vMax);
				}

				vMid.x = vMin.x + (vMax.x - vMin.x) / 2;

				if (left)
				{
					DivideBasedOnPoint(&GirthList[i], GirthLength[i], UnitLength, vMid, 1);
				}
				else
				{
					DivideBasedOnPoint(&GirthList[i], GirthLength[i], UnitLength, vMid, 0);
				}

				if (left)
				{
					Surface[5][0] += CalcGirth(GirthList[i], UnitLength[0]);
					Volume[5][0] += CalcAreaOfPolygon(GirthList[i], UnitLength[0]);

					Surface[0][0] += CalcGirth(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0]);
					Volume[0][0] += CalcAreaOfPolygon(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0]);

					Surface[0][1] += CalcGirth(GirthList[i] + UnitLength[1], UnitLength[2] - UnitLength[1]);
					Volume[0][1] += CalcAreaOfPolygon(GirthList[i] + UnitLength[1], UnitLength[2] - UnitLength[1]);

					if (right)
					{
						Surface[5][1] += CalcGirth(GirthList[i] + UnitLength[2], UnitLength[3] - UnitLength[2]);
						Volume[5][1] += CalcAreaOfPolygon(GirthList[i] + UnitLength[2], UnitLength[3] - UnitLength[2]);
					}
				}
				else
				{
					Surface[0][0] += CalcGirth(GirthList[i], UnitLength[0]);
					Volume[0][0] += CalcAreaOfPolygon(GirthList[i], UnitLength[0]);

					Surface[0][1] += CalcGirth(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0]);
					Volume[0][1] += CalcAreaOfPolygon(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0]);

					if (right)
					{
						Surface[5][1] += CalcGirth(GirthList[i] + UnitLength[1], UnitLength[2] - UnitLength[1]);
						Volume[5][1] += CalcAreaOfPolygon(GirthList[i] + UnitLength[1], UnitLength[2] - UnitLength[1]);
					}
				}

				continue;
			}
			else if (2 == GirthCount)
			{
				float v1, v2;
				v1 = CalcGirth(GirthList[i], UnitLength[0]);
				v2 = CalcGirth(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0]);

				float tempMax = std::max(v1, v2);

				if (tempMax == v1) //첫번째가 클때
				{
					VERTEX vMax, vMin, vMid;
					bool left = false;

					GetMinMax(GirthList[i], UnitLength[0], vMin, vMax);

					if (vMin.x < PartsPt[1].x)
					{
						DivideBasedOnPoint(&GirthList[i], GirthLength[i], UnitLength, PartsPt[1], 0);
						left = true;
					}

					if (left)
					{
						GetMinMax(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0], vMin, vMax);
					}
					else
					{
						GetMinMax(GirthList[i], UnitLength[0], vMin, vMax);
					}

					vMid.x = vMin.x + (vMax.x - vMin.x) / 2;

					if (left)
					{
						DivideBasedOnPoint(&GirthList[i], GirthLength[i], UnitLength, vMid, 1);
					}
					else
					{
						DivideBasedOnPoint(&GirthList[i], GirthLength[i], UnitLength, vMid, 0);
					}

					if (left)
					{
						Surface[5][0] += CalcGirth(GirthList[i], UnitLength[0]);
						Volume[5][0] += CalcAreaOfPolygon(GirthList[i], UnitLength[0]);

						Surface[0][0] += CalcGirth(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0]);
						Volume[0][0] += CalcAreaOfPolygon(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0]);

						Surface[0][1] += CalcGirth(GirthList[i] + UnitLength[1], UnitLength[2] - UnitLength[1]);
						Volume[0][1] += CalcAreaOfPolygon(GirthList[i] + UnitLength[1], UnitLength[2] - UnitLength[1]);

						Surface[5][1] += CalcGirth(GirthList[i] + UnitLength[2], UnitLength[3] - UnitLength[2]);
						Volume[5][1] += CalcAreaOfPolygon(GirthList[i] + UnitLength[2], UnitLength[3] - UnitLength[2]);
					}
					else
					{
						Surface[0][0] += CalcGirth(GirthList[i], UnitLength[0]);
						Volume[0][0] += CalcAreaOfPolygon(GirthList[i], UnitLength[0]);

						Surface[0][1] += CalcGirth(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0]);
						Volume[0][1] += CalcAreaOfPolygon(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0]);
						Surface[5][1] += CalcGirth(GirthList[i] + UnitLength[1], UnitLength[2] - UnitLength[1]);
						Volume[5][1] += CalcAreaOfPolygon(GirthList[i] + UnitLength[1], UnitLength[2] - UnitLength[1]);
					}

					continue;
				}
				else //두번째가 클때
				{
					VERTEX vMax, vMin, vMid;
					bool right = false;

					GetMinMax(GirthList[i], UnitLength[0], vMin, vMax);

					if (vMax.x > PartsPt[2].x)
					{
						DivideBasedOnPoint(&GirthList[i], GirthLength[i], UnitLength, PartsPt[2], 1);

						right = true;
					}

					GetMinMax(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0], vMin, vMax);

					vMid.x = vMin.x + (vMax.x - vMin.x) / 2;
					DivideBasedOnPoint(&GirthList[i], GirthLength[i], UnitLength, vMid, 1);

					Surface[5][0] += CalcGirth(GirthList[i], UnitLength[0]);
					Volume[5][0] += CalcAreaOfPolygon(GirthList[i], UnitLength[0]);

					Surface[0][0] += CalcGirth(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0]);
					Volume[0][0] += CalcAreaOfPolygon(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0]);

					Surface[0][1] += CalcGirth(GirthList[i] + UnitLength[1], UnitLength[2] - UnitLength[1]);
					Volume[0][1] += CalcAreaOfPolygon(GirthList[i] + UnitLength[1], UnitLength[2] - UnitLength[1]);

					if (right)
					{
						Surface[5][1] += CalcGirth(GirthList[i] + UnitLength[2], UnitLength[3] - UnitLength[2]);
						Volume[5][1] += CalcAreaOfPolygon(GirthList[i] + UnitLength[2], UnitLength[3] - UnitLength[2]);
					}

					continue;
				}
			}
			else if (3 == GirthCount)
			{
				VERTEX vMax, vMin, vMid;

				GetMinMax(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0], vMin, vMax);

				vMid.x = vMin.x + (vMax.x - vMin.x) / 2;

				DivideBasedOnPoint(&GirthList[i], GirthLength[i], UnitLength, vMid, 1);

				Surface[5][0] += CalcGirth(GirthList[i], UnitLength[0]);
				Volume[5][0] += CalcAreaOfPolygon(GirthList[i], UnitLength[0]);

				Surface[0][0] += CalcGirth(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0]);
				Volume[0][0] += CalcAreaOfPolygon(GirthList[i] + UnitLength[0], UnitLength[1] - UnitLength[0]);

				Surface[0][1] += CalcGirth(GirthList[i] + UnitLength[1], UnitLength[2] - UnitLength[1]);
				Volume[0][1] += CalcAreaOfPolygon(GirthList[i] + UnitLength[1], UnitLength[2] - UnitLength[1]);

				Surface[5][1] += CalcGirth(GirthList[i] + UnitLength[2], UnitLength[3] - UnitLength[2]);
				Volume[5][1] += CalcAreaOfPolygon(GirthList[i] + UnitLength[2], UnitLength[3] - UnitLength[2]);

				continue;
			}
		}
	}

	for (int i = 0; i < TotalListLength; i++)
	{
		// 경고	C6001	초기화되지 않은 메모리 '*GirthList'을(를) 사용하고 있습니다.
		delete[] GirthList[i];
		GirthList[i] = nullptr;
	}

	delete[] GirthList;
	GirthList = nullptr;

	delete[] GirthLength;
	GirthLength = nullptr;
}

//1593
void CGccGeometry::GetMinMaxOfZForY(VERTEX* VertexList, int VertexLength, float Start, float End, VERTEX& Min, VERTEX& Max)
{
	Min.x = Min.y = Min.z = 100000000000000000000.f;
	Max.x = Max.y = Max.z = -100000000000000000000.f;

	float Top, Bottom = 0.f;

	if (Start > End)
	{
		Top = Start;
		Bottom = End;
	}
	else
	{
		Top = End;
		Bottom = Start;
	}

	for (int i = 0; i < VertexLength; i++)
	{
		if (VertexList[i].y <= Top && VertexList[i].y >= Bottom)
		{
			if (VertexList[i].z < Min.z)
			{
				Min = VertexList[i];
			}

			if (VertexList[i].z > Max.z)
			{
				Max = VertexList[i];
			}
		}
	}
}

//1901
VERTEX CGccGeometry::GetPointOnLineForY(VERTEX Vector, VERTEX P1, float Y)
{
	VERTEX Result;
	Result.x = ((Y - P1.y) / Vector.y) * Vector.x + P1.x;
	Result.y = Y;
	Result.z = ((Y - P1.y) / Vector.y) * Vector.z + P1.z;
	return Result;
}

//1910
int CGccGeometry::InsertSortX(VERTEX** List, VERTEX In, int& Length)
{
	int Result = 0;

	if (0 == Length)
	{
		*List = new VERTEX[Length + 1];
		(*List)[Result] = In;
		Length++;
		return Result;
	}

	VERTEX* ListTemp = nullptr;

	for (Result = 0; Result < Length; Result++)
	{
		if ((*List)[Result].x == In.x && (*List)[Result].y == In.y && (*List)[Result].z == In.z)
		{
			return Result;
		}

		if ((*List)[Result].x > In.x)
		{
			ListTemp = new VERTEX[Length];
			memcpy(ListTemp, *List, sizeof(VERTEX) * Length);

			delete[] * List;
			*List = nullptr;

			*List = new VERTEX[Length + 1];
			memcpy(*List, ListTemp, sizeof(VERTEX) * Result);
			(*List)[Result] = In;
			memcpy((*List) + Result + 1, ListTemp + Result, sizeof(VERTEX) * (Length - Result));
			Length++;

			delete[] ListTemp;
			ListTemp = nullptr;

			return Result;
		}
	}

	ListTemp = new VERTEX[Length];
	memcpy(ListTemp, *List, sizeof(VERTEX) * Length);

	delete[] * List;
	*List = nullptr;

	*List = new VERTEX[Length + 1];
	memcpy(*List, ListTemp, sizeof(VERTEX) * Length);
	(*List)[Length] = In;
	Length++;

	delete[] ListTemp;
	ListTemp = nullptr;

	return Result;
}

//1984
void CGccGeometry::DivideBasedOnPoint(VERTEX** GirthList, int& PointLength, int GirthLength[4], VERTEX BasePoint, int GirthNum)
{
	int count1 = 0;
	int count2 = 0;
	VERTEX Contect1, Contect2, Contect3, Contect4;
	int len = GirthLength[GirthNum] - (0 == GirthNum ? 0 : GirthLength[GirthNum - 1]);
	VERTEX* temp1 = new VERTEX[len + 4];
	VERTEX* temp2 = new VERTEX[len + 4];
	float fMin, fMax = 0.f;
	int x = 0;

	for (x = (0 == GirthNum ? 0 : GirthLength[GirthNum - 1]); x < GirthLength[GirthNum] - 1; x++)
	{
		fMin = std::min((*GirthList)[x].x, (*GirthList)[x + 1].x);
		fMax = std::max((*GirthList)[x].x, (*GirthList)[x + 1].x);

		if (fMin <= BasePoint.x && fMax >= BasePoint.x)
		{
			Contect1.x = BasePoint.x;
			Contect1.y = (*GirthList)[x].y;
			Contect1.z = (*GirthList)[x].z +
				((Contect1.x - (*GirthList)[x].x) / ZeroDivisor((*GirthList)[x + 1].x - (*GirthList)[x].x)) *
				((*GirthList)[x + 1].z - (*GirthList)[x].z);
			break;
		}
	}

	for (x++; x < GirthLength[GirthNum] - 1; x++)
	{
		fMin = std::min((*GirthList)[x].x, (*GirthList)[x + 1].x);
		fMax = std::max((*GirthList)[x].x, (*GirthList)[x + 1].x);

		if (fMin <= BasePoint.x && fMax >= BasePoint.x)
		{
			Contect2.x = BasePoint.x;
			Contect2.y = (*GirthList)[x].y;
			Contect2.z = (*GirthList)[x].z +
				((Contect2.x - (*GirthList)[x].x) / ZeroDivisor((*GirthList)[x + 1].x - (*GirthList)[x].x)) *
				((*GirthList)[x + 1].z - (*GirthList)[x].z);
			break;
		}
	}

	count1 = 0;
	count2 = 0;

	for (int i = (0 == GirthNum ? 0 : GirthLength[GirthNum - 1]); i < GirthLength[GirthNum]; i++)
	{
		if (BasePoint.x > (*GirthList)[i].x)
		{
			temp1[count1] = (*GirthList)[i];
			count1++;
		}
		else
		{
			temp2[count2] = (*GirthList)[i];
			count2++;
		}
	}

	Contect3 = Contect2;
	Contect3.z = std::min(Contect1.z, Contect2.z) +
		(std::max(Contect1.z, Contect2.z) - std::min(Contect1.z, Contect2.z)) / 3.f;
	Contect4 = Contect2;
	Contect4.z = std::min(Contect1.z, Contect2.z) +
		(std::max(Contect1.z, Contect2.z) - std::min(Contect1.z, Contect2.z)) / 3.f * 2.f;

	temp1[count1] = Contect1;
	temp1[count1 + 1] = Contect2;
	temp1[count1 + 2] = Contect3;
	temp1[count1 + 3] = Contect4;

	temp2[count2] = Contect1;
	temp2[count2 + 1] = Contect2;
	temp2[count2 + 2] = Contect3;
	temp2[count2 + 3] = Contect4;

	count1 += 4;
	count2 += 4;
	SortByNearPoint(temp1, count1);
	SortByNearPoint(temp2, count2);

	VERTEX* TempGirth = nullptr;

	TempGirth = new VERTEX[PointLength + 8];

	for (int i = 0; i < GirthNum; i++)
	{
		memcpy(TempGirth + (0 == i ? 0 : GirthLength[i - 1]),
			(*GirthList) + (0 == i ? 0 : GirthLength[i - 1]),
			sizeof(VERTEX) * (GirthLength[i] - (0 == i ? 0 : GirthLength[i - 1])));
	}

	memcpy(TempGirth + (0 == GirthNum ? 0 : GirthLength[GirthNum - 1]), temp1, sizeof(VERTEX) * count1);
	memcpy(TempGirth + (0 == GirthNum ? 0 : GirthLength[GirthNum - 1]) + count1, temp2, sizeof(VERTEX) * count2);

	if (GirthNum < 2 && GirthLength[GirthNum + 1] != 0)
	{
		memcpy(TempGirth + (0 == GirthNum ? 0 : GirthLength[GirthNum - 1]) + count1 + count2,
			(*GirthList) + GirthLength[GirthNum],
			sizeof(VERTEX) * (GirthLength[GirthNum + 1] - GirthLength[GirthNum]));
	}

	for (int i = 3; i > GirthNum; i--)
	{
		if (GirthLength[i - 1] != 0)
		{
			GirthLength[i] = GirthLength[i - 1] + 8;
		}
	}

	GirthLength[GirthNum] = (0 == GirthNum ? 0 : GirthLength[GirthNum - 1]) + count1;
	GirthLength[GirthNum + 1] = GirthLength[GirthNum] + count2;

	if (*GirthList)
	{
		delete[] * GirthList;
		*GirthList = nullptr;
	}

	*GirthList = TempGirth;
	PointLength += 8;

	delete[] temp1;
	temp1 = nullptr;

	delete[] temp2;
	temp2 = nullptr;
}

//2095
void CGccGeometry::SortOutlinePointsY(VERTEX** PointList, int& Length)
{
	if (0 == Length)
	{
		return;
	}

	int LenTemp = Length;
	VERTEX* List = new VERTEX[LenTemp];
	VERTEX* TempList = nullptr;
	memcpy(List, *PointList, sizeof(VERTEX) * LenTemp);
	VERTEX Temp;

	for (int i = 0; i < LenTemp; i++)
	{
		for (int j = 1; j < LenTemp - i; j++)
		{
			if ((List[j - 1].x == List[j].x &&
				List[j - 1].y == List[j].y &&
				List[j - 1].z == List[j].z))
			{
				TempList = new VERTEX[LenTemp - j];
				memcpy(TempList, List + j + 1, sizeof(VERTEX) * (LenTemp - j - 1));
				memcpy(List + j, TempList, sizeof(VERTEX) * (LenTemp - j - 1));

				delete[] TempList;
				TempList = nullptr;

				LenTemp--;
				j--;

				continue;
			}

			if (List[j - 1].y > List[j].y)
			{
				Swap(List[j - 1], List[j]);
			}
		}
	}

	if (*PointList != nullptr)
	{
		delete[] * PointList;
		*PointList = nullptr;
	}

	*PointList = new VERTEX[LenTemp];
	memcpy(*PointList, List, sizeof(VERTEX) * LenTemp);
	Length = LenTemp;

	delete[] List;
	List = nullptr;
}

//2136
void CGccGeometry::OutlineDivisionY(VERTEX* GirthList, int PointLength, int GirthLength[4])
{
	int Length = PointLength;

	for (int i = 0; i < 4; i++)
	{
		GirthLength[i] = 0;
	}

	if (Length < 20)
	{
		GirthLength[0] = Length;
		GirthLength[1] = 0;
		GirthLength[2] = 0;
		GirthLength[3] = 0;
		return;
	}

	float Max[4] = { 5.f }, Dis, Temp = 0.f;
	Max[0] = 5.f;
	Max[1] = 5.f;
	Max[2] = 5.f;
	Max[3] = 5.f;

	for (int i = 1; i < Length; i++)
	{
		Dis = fabs(GirthList[i].y - GirthList[i - 1].y);

		for (int j = 3; j >= 0; j--)
		{
			if (Max[j] < Dis)
			{
				Temp = Max[j];
				Max[j] = Dis;
				Dis = Temp;
			}
		}
	}

	Dis = fabs(GirthList[Length - 1].y - GirthList[0].y) / (Length / 3.f);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 1; j < 4 - i; j++)
		{
			if (fabs(Max[j] - Max[j - 1]) < Dis)
			{
				Max[j - 1] = Max[j];
			}
		}
	}

	int Count = 0;

	for (int i = 1; i < Length; i++)
	{
		if (fabs(GirthList[i].y - GirthList[i - 1].y) > Max[0])
		{
			GirthLength[Count] = i;
			Count++;

			if (Count == 4)
			{
				break;
			}
		}
	}

	if (Count != 4)
	{
		GirthLength[Count] = Length;
	}
}

//2210
void CGccGeometry::GetVtcOutlinePoints(VERTEX* VertexList, int VertexLength, FACE* FaceList, int FaceLength, VERTEX** GirthList, int GirthLength[4], float fPoinsX)
{
	if (0 == VertexLength)
	{
		return;
	}

	float PlaneEqu[4] = { 1, 0, 0, fPoinsX };
	VERTEX* List = nullptr;
	VERTEX* ListTemp = nullptr;
	VERTEX Temp;
	VERTEX P1, P2;
	int Length = 0;
	VERTEX Min, Max;

	for (int i = 0; i < FaceLength; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			P1 = VertexList[FaceList[i].List[j]];
			P2 = VertexList[FaceList[i].List[3 == j + 1 ? 0 : j + 1]];

			switch (IsCrossLinePlane(P1, P2, PlaneEqu))
			{
			case POINT_ZERO:
				break;

			case POINT_CROSS:
			{
				bool bGet = GetIntersectionPointLinePlane(P1, P2, PlaneEqu, Temp);

				if (bGet)
				{
					//
				}

				if (List)
				{
					ListTemp = new VERTEX[Length];
					memcpy(ListTemp, List, sizeof(VERTEX) * Length);

					delete[] List;
					List = nullptr;

					List = new VERTEX[Length + 1];
					memcpy(List, ListTemp, sizeof(VERTEX) * Length);

					delete[] ListTemp;
					ListTemp = nullptr;
				}
				else
				{
					List = new VERTEX[Length + 1];
				}

				List[Length] = Temp;
				Length++;
			}
			break;

			case POINT_CONTACT1:
				if (List)
				{
					ListTemp = new VERTEX[Length];
					memcpy(ListTemp, List, sizeof(VERTEX) * Length);

					delete[] List;
					List = nullptr;

					List = new VERTEX[Length + 1];
					memcpy(List, ListTemp, sizeof(VERTEX) * Length);

					delete[] ListTemp;
					ListTemp = nullptr;
				}
				else
				{
					List = new VERTEX[Length + 1];
				}

				List[Length] = P1;
				Length++;
				break;

			case POINT_CONTACT2:
				if (List)
				{
					ListTemp = new VERTEX[Length];
					memcpy(ListTemp, List, sizeof(VERTEX) * Length);

					delete[] List;
					List = nullptr;

					List = new VERTEX[Length + 1];
					memcpy(List, ListTemp, sizeof(VERTEX) * Length);

					delete[] ListTemp;
					ListTemp = nullptr;
				}
				else
				{
					List = new VERTEX[Length + 1];
				}

				List[Length] = P2;
				Length++;
				break;

			case POINT_INCLUDE:
				if (List)
				{
					ListTemp = new VERTEX[Length];
					memcpy(ListTemp, List, sizeof(VERTEX) * Length);

					delete[] List;
					List = nullptr;

					List = new VERTEX[Length + 2];
					memcpy(List, ListTemp, sizeof(VERTEX) * Length);

					delete[] ListTemp;
					ListTemp = nullptr;
				}
				else
				{
					List = new VERTEX[Length + 2];
				}

				List[Length] = P1;
				List[Length + 1] = P2;
				Length += 2;
				break;
			}
		}
	}

	if (Length <= 1)
	{
		if (*GirthList)
		{
			delete[] * GirthList;
			*GirthList = nullptr;
		}

		for (int i = 0; i < 4; i++)
		{
			GirthLength[i] = 0;
		}

		if (List)
		{
			delete[] List;
			List = nullptr;
		}

		return;
	}

	SortOutlinePointsY(&List, Length);

	if (*GirthList)
	{
		delete[] * GirthList;
		*GirthList = nullptr;
	}

	*GirthList = new VERTEX[Length];
	memcpy(*GirthList, List, sizeof(VERTEX) * Length);

	OutlineDivisionY(List, Length, GirthLength);

	SortOutlineList(*GirthList, GirthLength, Length);

	if (List)
	{
		delete[] List;
		List = nullptr;
	}
}
