#pragma once

#include <d2d1.h>
#include "Vector2D.h"

using namespace std;

enum EWeaponSlots;

class CBullet
{
public:
	EWeaponSlots WeaponType;

	bool m_BLActive;		// 총알의 액티브 상태
	ID2D1Bitmap* m_BulletImg = NULL;
	float m_TexImgWidth = 40.0f;
	float m_TexImgHeight = 10.0f;

	float m_Duration;		// 총알의 라이프 타임(총알을 몇초당안 날아가게 할 것인지의 시간)
	Vector2D m_CurPos;		// 총알의 현재 위치
	Vector2D m_RenderPos;		// 총알 렌더링 연출을 위한 위치 계손용 변수
	Vector2D m_DirVec;		// 총알의 날아가는 방향벡터
	float m_DirRot = 0.0f;		// 총알의 날아가는 방향각도
	float m_MoveSpeed;		// 총알의 이동 속도
	float m_Radius;		// 총알 충돌 반경
	Vector2D a_CalcBVec;		// 계산용 변수
	float m_SmokeTime = 0.0f;

public:
	CBullet();
	~CBullet();

	bool BulletUpdate(float a_DwdDeltaTime,		// <--- 정밀한 기준의 DeltaTime
		DWORD a_wdLastTime,		// <--- 정밀한 현재 시간
		float a_WpDeltaTime,		// <--- 무기별 이동을 위한 DeltaTime
		void(*GenSmokePtc)(Vector2D a_StartV, DWORD a_CurTime, Vector2D a_DirVec) = NULL);		// <--- 미사일 Smoke 생성용 함수 포인터

	void BulletRender(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush, Vector2D a_CamPos = { 0, 0 });
};

