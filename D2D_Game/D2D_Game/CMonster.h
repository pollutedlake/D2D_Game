#pragma once

#include "Vector2D.h"
#include <d2d1.h>

class CMonster
{
	ID2D1Bitmap* m_SocketImg;		// �ִ� ����ó�� ����� ������
	bool m_isActive;
	float m_MaxHP = 100.0f;
	float m_CurHP = 100.0f;

	//--- ���� ��ǥ ���� ������...
	Vector2D m_CurPos;
	float m_Speed = 150.0f;
	Vector2D m_DirVec;
	float m_HalfColl = 17;		// ������ �浹 �ݰ�
	//--- ���� ��ǥ ���� ������...

	//------ ������ǥ �̵� ���� ����
	VecINT2D m_ScreenHalf;		// ȭ�� �� ������ ����� ����
	Vector2D m_CamPos = { 0.0f, 0.0f };		// ���� ��ġ
	Vector2D m_RenderPos;		// ������ ��ġ
	//------ ������ǥ �̵� ���� ����

public:
	CMonster();
	~CMonster();

public:
	void Init_Unit();
	void Update_Unit(double a_DeltaTime, VecINT2D& a_Center, Vector2D& a_CamPos, class CHero& a_Hero);
	void Render_Unit(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush);
	void Destroy_Unit();
};

