#include "framework.h"
#include "CButton.h"

void CButton::UI_Init(HWND a_hWnd, float a_PosXRate, float a_PosYRate, float a_SzXRate, float a_SzYRate)
{
	m_hWnd = a_hWnd;

	m_PosXRate = a_PosXRate;
	m_PosYRate = a_PosYRate;

	m_SzXRate = a_SzXRate;		// 이미지 사이즈 스케일 X
	m_SzYRate = a_SzYRate;		// 이미지 사이즈 스케일 Y

	if (m_pBtnUpImg != NULL) {
		m_ImgSizeX = m_pBtnUpImg->GetSize().width * a_SzXRate;
		m_ImgSizeY = m_pBtnUpImg->GetSize().height * a_SzYRate;
	}
}

void CButton::UI_Update(float a_DeltaTime)
{
	if (m_pBtnUpImg == NULL) {
		return;
	}

	GetClientRect(m_hWnd, &m_Crt);

	m_CollBound.left = (LONG)(m_Crt.right * m_PosXRate) - (LONG)(m_ImgSizeX / 2);
	m_CollBound.top = (LONG)(m_Crt.bottom * m_PosYRate) - (LONG)(m_ImgSizeY / 2);
	m_CollBound.right = m_CollBound.left + m_ImgSizeX;
	m_CollBound.bottom = m_CollBound.top + m_ImgSizeY;
}

void CButton::UI_Render(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush)
{
	if (m_bPushBtnDU == true) {
		if (m_pBtnDownImg != NULL) {
			a_pd2dRTarget->DrawBitmap(m_pBtnDownImg, D2D1::RectF(m_CollBound.left, m_CollBound.top, m_CollBound.right, m_CollBound.bottom));
		}
	}
	else {
		if (m_pBtnUpImg != NULL) {
			a_pd2dRTarget->DrawBitmap(m_pBtnUpImg, D2D1::RectF(m_CollBound.left, m_CollBound.top, m_CollBound.right, m_CollBound.bottom));
		}
	}

	if (0 <= m_SubImgIdx && m_SubImgIdx < m_SubImgList.size()) {
		if (m_SubImgList[m_SubImgIdx]->m_pImg != NULL) {
			static RECT a_CalcRect = { 0,0,0,0 };
			a_CalcRect.left = (LONG)(m_Crt.right * m_PosXRate) - (LONG)(m_SubImgList[m_SubImgIdx]->m_ImgSizeX * m_SubImgList[m_SubImgIdx]->m_PosXReRt);
			a_CalcRect.top = (LONG)(m_Crt.bottom * m_PosYRate) - (LONG)(m_SubImgList[m_SubImgIdx]->m_ImgSizeY * m_SubImgList[m_SubImgIdx]->m_PosYReRt);
			a_CalcRect.right = a_CalcRect.left + m_SubImgList[m_SubImgIdx]->m_ImgSizeX;
			a_CalcRect.bottom = a_CalcRect.top + m_SubImgList[m_SubImgIdx]->m_ImgSizeY;

			a_pd2dRTarget->DrawBitmap(m_SubImgList[m_SubImgIdx]->m_pImg, D2D1::RectF(a_CalcRect.left, a_CalcRect.top, a_CalcRect.right, a_CalcRect.bottom));
		}
	}
}

void CButton::UI_Destroy()
{
	for (int aii = 0; aii < m_SubImgList.size(); aii++) {
		if (m_SubImgList[aii] != NULL) {
			delete m_SubImgList[aii];
			m_SubImgList[aii] = NULL;
		}
	}

	if (m_pBtnDownImg != NULL) {
		m_pBtnDownImg->Release();
		m_pBtnDownImg = NULL;
	}

	if (m_pBtnUpImg != NULL) {
		m_pBtnUpImg->Release();
		m_pBtnUpImg = NULL;
	}
}

bool CButton::OnLButtonDown(POINT a_MouseXY)
{
	if (m_pBtnUpImg == NULL) {
		return false;
	}

	if (m_bPushBtnDU == false) {
		if (m_CollBound.left <= a_MouseXY.x && a_MouseXY.x <= m_CollBound.right && m_CollBound.top <= a_MouseXY.y && a_MouseXY.y <= m_CollBound.bottom) {
			if (PressCallback != NULL) {
				PressCallback();
			}

			m_bPushBtnDU = true;
			return true;
		}
	}	// if (m_bPushBtnDU == false)

	return false;
}

void CButton::OnLButtonUp()
{
	if (m_pBtnUpImg == NULL) {
		return;
	}

	if (m_bPushBtnDU == true) {
		if (ClickCallback != NULL) {
			ClickCallback();
		}

		m_bPushBtnDU = false;
	}	// if (m_bPushBtnDU == true)
}

bool CButton::IsPickLock(POINT a_MouseXY)
{
	if (m_pBtnUpImg == NULL) {
		return false;
	}

	if (m_bPushBtnDU == true) {
		return true;
	}

	if (m_CollBound.left <= a_MouseXY.x && a_MouseXY.x <= m_CollBound.right && m_CollBound.top <= a_MouseXY.y && a_MouseXY.y <= m_CollBound.bottom) {
		return true;
	}

	return false;
}

void CButton::AddSubImg(ID2D1Bitmap* a_pImg, float a_SzXRate, float a_SzYRate, float a_PosXAdjust, float a_PosYAdjust)
{
	SubImage* a_BNode = new SubImage();

	a_BNode->m_pImg = a_pImg;

	a_BNode->m_SzXRate = a_SzXRate;		// 이미지 사이즈 스케일 X
	a_BNode->m_SzXRate = a_SzXRate;

	a_BNode->m_PosXReRt = a_PosXAdjust;		// SubImage 만의 사이즈 조정에 따른 위치 조정 값
	a_BNode->m_PosYReRt = a_PosYAdjust;		// SubImage 만의 사이즈 조정에 따른 위치 조정 값

	if (a_BNode->m_pImg != NULL) {
		a_BNode->m_ImgSizeX = a_BNode->m_pImg->GetSize().width * a_SzXRate;
		a_BNode->m_ImgSizeY = a_BNode->m_pImg->GetSize().height * a_SzYRate;
	}

	m_SubImgList.push_back(a_BNode);
}

void CButton::ChangeSubImg(int a_Idx)
{
	if (0 <= a_Idx && a_Idx < m_SubImgList.size()) {
		m_SubImgIdx = a_Idx;
	}
}
