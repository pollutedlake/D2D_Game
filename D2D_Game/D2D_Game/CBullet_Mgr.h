#pragma once

#include <vector>
using namespace std;
#include "CBullet.h"

class CBullet_Mgr
{
public:
	float m_PistolWidth = 30.0f;
	float m_PistolHeight = 5.0f;

	ID2D1Bitmap* m_MachineGunImg = NULL;
	float m_McGunWidth = 40.0f;
	float m_McGunHeight = 10.0f;
	ID2D1Bitmap* m_RocketImg = NULL;
	float m_RocketWidth = 100.0f;
	float m_RocketHeight = 32.0f;

	vector<CBullet*> m_BullList;		// <--- 총알 풀 방식

	VecINT2D m_CenterPos;
	Vector2D m_CamPos = { 0.0f, 0.0f };

public:
	CBullet_Mgr();
	~CBullet_Mgr();

public:
	void BLMgerInit(ID2D1Bitmap* (*D2DLoadBmp)(LPCWSTR FName) = NULL);
	void BLMgerUpdate(float a_DwdDeltaTime,		// <--- 정밀한 기준의 델타 타임
		DWORD a_DwdLastTime,		// <--- 정밀한 기준의 현재 시간
		float(*GetDelta)(int WpType) = NULL,		// <--- 무기별 이동을 위한 DeltaTime 얻어오기
		bool(*CheckColl)(CBullet* a_RefBull, DWORD a_CurTime) = NULL,		// <--- 충돌 처리를 위한 함수 포인터
		void(*GenSmokePtc)(Vector2D a_StartV, DWORD a_CurTime, Vector2D a_DirVec) = NULL);		// <--- 미사일 Smoke 생성용 함수 포인터
	void BLMgerRender(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush);
	void BLMgerDestroy();
};

extern CBullet_Mgr g_Bullet_Mgr;

