#pragma once

#include "Vector2D.h"
#pragma comment(lib, "d2d1.lib")		// <--- Direct2D ����ϱ�...

#include <d2d1.h>

#include<vector>
using namespace std;

class CHero
{
public:
	ID2D1Bitmap* m_SocketImg = NULL;		// �ִ� ����ó�� ����� ������

	int m_CurHP;
	int m_MaxHP;

	//------ ĳ���� �̵� ���� ����
	Vector2D m_CurPos = { 0.0f, 0.0f };		// ȭ�� ���� ��ġ
	double m_MoveSpeed;		// �̵� �ӵ�
	//------ ĳ���� �̵� ���� ����

	//------ ������ǥ �̵� ���� ����
	VecINT2D m_ScreenHalf;		// ȭ�� �� ������ ����� ����
	Vector2D m_CamPos = { 0.0f, 0.0f };		// ���� ��ġ
	Vector2D m_RenderPos;		// ������ ��ġ
	//------ ������ǥ �̵� ���� ����

	// ��ŷ ���� ������...
	Vector2D m_DirVec;
	bool m_bMoveOnOff = false;
	Vector2D m_TargetPos;
	// ��ŷ ���� ������...

	float m_HalfColl = 20.0f;		// ���ΰ��� �浹 �ݰ�

	//------ �ִϸ��̼� ���� ����
public:
	enum CT_Type m_CharicType;		// ĳ���� ������ "����"����? "���ΰ�"����?
	class CAnimData* m_RefAniData;		// ĳ���� Ÿ�Կ� ���� �ִϸ��̼� �����͸� �ٲ� �� ����

	enum AniState m_CurAniState;		// ���� �ִϸ��̼� ����
	int m_NowImgCount;		// �ִ� ����ó�� ����� �̹��� ī��Ʈ
	float m_EachAniDelay;		// ������ ���� �ð�
	float m_AniTickCount;		// ���� �����ӱ��� �ð� Add
	int m_CurAniInx;		// ���� Ani Index
	//------ �ִϸ��̼� ���� ����

public:
	CHero();
	~CHero();

public:
	void Init_Unit(ID2D1Bitmap* (*D2DLoadBmp)(LPCWSTR FName) = NULL);
	void Update_Unit(double a_DeltaTime);
	void Render_Unit(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush);
	void Destroy_Unit();

	void Abs_Update(VecINT2D& a_ScHalf, Vector2D& a_CamPos);
	void MsPicking(Vector2D a_TgPos);

	void TakeDamage(float a_Damage = 10.0f);

	void SetAni_Rsc(CT_Type a_CharType, vector<class CAnimData*>* refAnimList = nullptr);

	bool ChangeState(AniState newState);
	void AniFrameUpdate(double a_DeltaTime);
};

extern CHero g_Hero;

