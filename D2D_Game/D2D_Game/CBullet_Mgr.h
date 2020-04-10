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

	vector<CBullet*> m_BullList;		// <--- �Ѿ� Ǯ ���

	VecINT2D m_CenterPos;
	Vector2D m_CamPos = { 0.0f, 0.0f };

public:
	CBullet_Mgr();
	~CBullet_Mgr();

public:
	void BLMgerInit(ID2D1Bitmap* (*D2DLoadBmp)(LPCWSTR FName) = NULL);
	void BLMgerUpdate(float a_DwdDeltaTime,		// <--- ������ ������ ��Ÿ Ÿ��
		DWORD a_DwdLastTime,		// <--- ������ ������ ���� �ð�
		float(*GetDelta)(int WpType) = NULL,		// <--- ���⺰ �̵��� ���� DeltaTime ������
		bool(*CheckColl)(CBullet* a_RefBull, DWORD a_CurTime) = NULL,		// <--- �浹 ó���� ���� �Լ� ������
		void(*GenSmokePtc)(Vector2D a_StartV, DWORD a_CurTime, Vector2D a_DirVec) = NULL);		// <--- �̻��� Smoke ������ �Լ� ������
	void BLMgerRender(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush);
	void BLMgerDestroy();
};

extern CBullet_Mgr g_Bullet_Mgr;

