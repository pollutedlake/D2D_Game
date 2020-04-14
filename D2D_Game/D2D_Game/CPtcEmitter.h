#pragma once
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "WindowsCodecs.lib")

#include <d2d1.h>
#include<wincodec.h>
#include "Vector2D.h"
#include <vector>

using namespace std;

class CPtcEmitter
{
public:
	bool m_EmtActive;		// ��ƼŬ �������� ��Ƽ�� ����

	ID2D1Bitmap* m_PtcImage = NULL;		// ��ƼŬ ������ �̹���

	float m_DirRot = 0.0f;		// ��ƼŬ �������� ���ư��� ���Ⱒ��
	float LifeTime;		// ��ƼŬ ������ ������ Ÿ��
	Vector2D SpawnPosition;		// ��ƼŬ ������ �ʱ� ���� ��ġ
	Vector2D StartSize;		// ��ƼŬ ������ �ʱ� ������
	Vector2D EndSize;		// ��ƼŬ ������ ������ ������
	Vector2D Velocity;		// ��ƼŬ �������� �̵� �ӵ�
	float StartAlpha = 1.0f;
	float EndAlpha = 1.0f;

	DWORD CreatedTime = 0.0f;
	DWORD m_LastTime = 0.0f;

	//--- Missile ���� ���� �̹��� ����
	ID2D1Bitmap* m_MslExp_Img = NULL;		// Missiile ���� ���� �̹���
	int UVX = 1;
	int UVY = 1;
	float UVPer = 0.0f;
	float ImgRealWidth = 256.0f;
	float ImgRealHeight = 256.0f;
	D2D1_RECT_F UVRect;
	//--- Missile ���� ���� �̹��� ����

	//------ ������ǥ �̵� ���� ����
	VecINT2D m_ScreenHalf;		// �ִ� �� ������ ����� ����
	Vector2D m_CamPos = { 0.0f, 0.0f };		// ���� ��ġ
	Vector2D m_RenderPos;		// ������ ��ġ
	//------ ������ǥ �̵� ���� ����

public:
	CPtcEmitter();
	~CPtcEmitter();

	void Emt_Update(DWORD a_LastTime, VecINT2D& a_Center, Vector2D& a_CamPos);
	void Emt_Render(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush);

	//--- Missile ���� ���� �̹��� ����
	void SetUV(int UV_X, int UV_Y) {
		UVX = UV_X;
		UVY = UV_Y;
	}
	// Index��° UV�簢�� ��ǥ
	void CalculateUVRect(float per);
	void MslExpRender(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush);		// �̻��� ���� ���� ����Ʈ ������ �Լ�
};

