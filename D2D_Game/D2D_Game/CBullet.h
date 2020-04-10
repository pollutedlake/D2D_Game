#pragma once

#include <d2d1.h>
#include "Vector2D.h"

using namespace std;

enum EWeaponSlots;

class CBullet
{
public:
	EWeaponSlots WeaponType;

	bool m_BLActive;		// �Ѿ��� ��Ƽ�� ����
	ID2D1Bitmap* m_BulletImg = NULL;
	float m_TexImgWidth = 40.0f;
	float m_TexImgHeight = 10.0f;

	float m_Duration;		// �Ѿ��� ������ Ÿ��(�Ѿ��� ���ʴ�� ���ư��� �� �������� �ð�)
	Vector2D m_CurPos;		// �Ѿ��� ���� ��ġ
	Vector2D m_RenderPos;		// �Ѿ� ������ ������ ���� ��ġ ��տ� ����
	Vector2D m_DirVec;		// �Ѿ��� ���ư��� ���⺤��
	float m_DirRot = 0.0f;		// �Ѿ��� ���ư��� ���Ⱒ��
	float m_MoveSpeed;		// �Ѿ��� �̵� �ӵ�
	float m_Radius;		// �Ѿ� �浹 �ݰ�
	Vector2D a_CalcBVec;		// ���� ����
	float m_SmokeTime = 0.0f;

public:
	CBullet();
	~CBullet();

	bool BulletUpdate(float a_DwdDeltaTime,		// <--- ������ ������ DeltaTime
		DWORD a_wdLastTime,		// <--- ������ ���� �ð�
		float a_WpDeltaTime,		// <--- ���⺰ �̵��� ���� DeltaTime
		void(*GenSmokePtc)(Vector2D a_StartV, DWORD a_CurTime, Vector2D a_DirVec) = NULL);		// <--- �̻��� Smoke ������ �Լ� ������

	void BulletRender(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush, Vector2D a_CamPos = { 0, 0 });
};

