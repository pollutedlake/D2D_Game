#include "framework.h"
#include "Vector2D.h"
#include <math.h>

Vector2D::Vector2D()
{
	x = 0.0f;
	y = 0.0f;
}

Vector2D::~Vector2D()
{
}

float Vector2D::Magnitude()
{
	float a_Len = sqrtf((x * x) + (y * y));

	return a_Len;
}

void Vector2D::Normalize()
{
	float a_Len = sqrtf((x * x) + (y * y));

	if (0.0f < a_Len)
	{
		x = x / a_Len;
		y = y / a_Len;
	}
}

Vector2D Vector2D::GetNormalFromRotation(float RotationYaw) //각도를 벡터로 환산하는 함수
{
	RotationYaw *= 0.0174532925194444f;  // 3.14 / 180
	Vector2D RET;
	RET.x = cos(RotationYaw);
	RET.y = sin(RotationYaw);

	return RET;
}

float Vector2D::GetAngle()
{
	static double angleInRadians = 0;
	angleInRadians = atan2(y, x);
	static double angleInDegrees = 0;
	angleInDegrees = (angleInRadians / 3.1415926535f) * 180.0;

	return angleInDegrees;
}

//------------ 전역 연산자 함수 정의 부분
Vector2D operator+(const Vector2D & T1, const Vector2D & T2)
{
	Vector2D R;
	R.x = T1.x + T2.x;
	R.y = T1.y + T2.y;
	return R;
}

Vector2D operator-(const Vector2D & T1, const Vector2D & T2)
{
	Vector2D R;
	R.x = T1.x - T2.x;
	R.y = T1.y - T2.y;
	return R;
}

Vector2D operator*(const Vector2D & T1, const float & T2)
{
	Vector2D R;
	R.x = T1.x * T2;
	R.y = T1.y * T2;
	return R;
}

Vector2D operator*(const float & T1, const Vector2D & T2)
{
	Vector2D R;
	R.x = T1 * T2.x;
	R.y = T1 * T2.y;
	return R;
}

float GetAngle(Vector2D & a_A, Vector2D & a_B)
{
	static double a_CacDx = 0.0;
	static double a_CacDy = 0.0;
	static double a_CacRad = 0.0;
	static double a_Degree = 0.0;

	a_CacDx = 0.0;
	a_CacDy = 0.0;
	a_CacRad = 0.0;
	a_Degree = 0.0;

	a_CacDx = (double)(a_B.x - a_A.x);  //double == 8바이트
	a_CacDy = (double)(a_B.y - a_A.y);

	a_CacRad = atan2(a_CacDx, a_CacDy);
	a_Degree = (a_CacRad * 180.0f) / 3.141592f;

	if (a_Degree < 0)
		a_Degree += 360;

	return (float)a_Degree;
}
//------------ 전역 연산자 함수 정의 부분
