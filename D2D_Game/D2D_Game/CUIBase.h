#pragma once

#pragma comment(lib, "d2d1.lib")		// <--- Direct2D 사용하기...
#include <d2d1.h>
#include <functional>		// function
using namespace std;		// function

class CUIBase
{
public:
	HWND m_hWnd = NULL;
	float m_PosXRate = 0.0f;		// UI의 위치 좌표 : DC 사이즈 기준 비율(중앙기준)
	float m_PosYRate = 0.0f;		// UI의 위치 좌표 : DC 사이즈 기준 비율(중앙기준)
	float m_SzXRate = 1.0f;		// 이미지 사이즈 스케일 X
	float m_SzYRate = 1.0f;		// 이미지 사이즈 스케일 Y
	float m_ImgSizeX = 100.0f;
	float m_ImgSizeY = 100.0f;
	RECT m_Crt;
	RECT m_CollBound = { 0, 0, 0, 0 };

	function<void(void)> PressCallback = NULL;		// <--- STL에서 제공해 주는 함수 포인터 변수 생성
	function<void(void)> ClickCallback = NULL;

public:
	CUIBase() {

	}
	~CUIBase() {

	}

	virtual void UI_Init(HWND a_hWnd) {};		// 초기화 함수
	virtual void UI_Update(float a_DeltaTime) = 0 {};
	virtual void UI_Render(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush) = 0 {};
	virtual void UI_Destroy() = 0 {};

	virtual bool OnLButtonDown(POINT a_MouseXY) = 0 {};
	virtual void OnLButtonUp() = 0 {};
	virtual bool IsPickLock(POINT a_MouseXY) = 0 {};

	virtual void OnPress_AddListener(function<void(void)> Callback = NULL);		// PointerDown
	virtual void OnClick_AddListener(function<void(void)> Callback = NULL);		// PointerUp
};

