#pragma once

#include "CUIBase.h"
#include "SubImage.h"

#include <vector>
using namespace std;

class CButton : public CUIBase
{
public:
	bool m_bPushBtnDU = false;
	ID2D1Bitmap* m_pBtnDownImg = NULL;
	ID2D1Bitmap* m_pBtnUpImg = NULL;

	int m_SubImgIdx = -1;
	vector<SubImage*> m_SubImgList;

public:
	CButton() {

	}
	~CButton() {

	}

	void UI_Init(HWND a_hWnd, float a_PosXRate, float a_PosYRate, float a_SzXRate = 1.0f, float a_SzYRate = 1.0f);		// 초기화 함수
	virtual void UI_Update(float a_DeltaTime);
	virtual void UI_Render(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush);
	virtual void UI_Destroy();

	virtual bool OnLButtonDown(POINT a_MouseXY);
	virtual void OnLButtonUp();
	virtual bool IsPickLock(POINT a_MouseXY);

	void AddSubImg(ID2D1Bitmap* a_pImg, float a_SzXRate = 1.0f, float a_SzYRate = 1.0f, float a_PosXAdjust = 0.5f, float a_PosYAdjust = 0.5f);
	void ChangeSubImg(int a_Idx);

};

