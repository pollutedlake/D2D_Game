#pragma once

#include "CUIBase.h"
#include <vector>
using namespace std;

class CUIManager
{
public:
	HWND m_hWnd = NULL;
	bool m_PickLock = false;
	vector<CUIBase*> m_InGameUIList;		// <--- �ΰ��� UI
	int(*RefChangeWeapon)() = NULL;		// <--- ���� ���� ��ü �Լ� ������

public:
	CUIManager();
	~CUIManager();

	void UIMgrInit(HWND a_hWnd, ID2D1Bitmap* (*D2DLoadBmp)(LPCWSTR FName) = NULL);
	void UIMgrUpdate(double a_DeltaTime);
	void UIMgrRender(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush);
	void UIMgrDestroy();

	bool OnLButtonDown(HWND hWnd);
	void OnLButtonUp();
	static bool IsPickLock(HWND hWnd);
};

extern CUIManager g_UI_Mgr;

