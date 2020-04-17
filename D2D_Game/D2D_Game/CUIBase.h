#pragma once

#pragma comment(lib, "d2d1.lib")		// <--- Direct2D ����ϱ�...
#include <d2d1.h>
#include <functional>		// function
using namespace std;		// function

class CUIBase
{
public:
	HWND m_hWnd = NULL;
	float m_PosXRate = 0.0f;		// UI�� ��ġ ��ǥ : DC ������ ���� ����(�߾ӱ���)
	float m_PosYRate = 0.0f;		// UI�� ��ġ ��ǥ : DC ������ ���� ����(�߾ӱ���)
	float m_SzXRate = 1.0f;		// �̹��� ������ ������ X
	float m_SzYRate = 1.0f;		// �̹��� ������ ������ Y
	float m_ImgSizeX = 100.0f;
	float m_ImgSizeY = 100.0f;
	RECT m_Crt;
	RECT m_CollBound = { 0, 0, 0, 0 };

	function<void(void)> PressCallback = NULL;		// <--- STL���� ������ �ִ� �Լ� ������ ���� ����
	function<void(void)> ClickCallback = NULL;

public:
	CUIBase() {

	}
	~CUIBase() {

	}

	virtual void UI_Init(HWND a_hWnd) {};		// �ʱ�ȭ �Լ�
	virtual void UI_Update(float a_DeltaTime) = 0 {};
	virtual void UI_Render(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush) = 0 {};
	virtual void UI_Destroy() = 0 {};

	virtual bool OnLButtonDown(POINT a_MouseXY) = 0 {};
	virtual void OnLButtonUp() = 0 {};
	virtual bool IsPickLock(POINT a_MouseXY) = 0 {};

	virtual void OnPress_AddListener(function<void(void)> Callback = NULL);		// PointerDown
	virtual void OnClick_AddListener(function<void(void)> Callback = NULL);		// PointerUp
};

