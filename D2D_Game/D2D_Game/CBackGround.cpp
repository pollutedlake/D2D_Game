#include "framework.h"
#include "CBackGround.h"

CBackGround::CBackGround()
{
}

CBackGround::~CBackGround()
{
}

void CBackGround::BGMgrInit(ID2D1Bitmap* (*MyLoadBitmap)(LPCWSTR fileName))
{
	if (MyLoadBitmap != NULL) {
		m_pBGBmp = MyLoadBitmap(_T(".\\RscTrunk\\Background\\BackGround3.png"));

		if (m_pBGBmp != NULL) {
			m_ImgSize.x = m_pBGBmp->GetSize().width * 4;
			m_ImgSize.y = m_pBGBmp->GetSize().height * 4;

			m_WorldHfSize.x = (int)(m_ImgSize.x * (X_Tile / 2.0f));
			m_WorldHfSize.y = (int)(m_ImgSize.y * (Y_Tile / 2.0f));
		}
	}
}

void CBackGround::BGMgrUpdate(float a_DeltaTime, VecINT2D& a_Center, Vector2D& a_CamPos)
{
	m_ScreenHalf.x = a_Center.x;		// MainDC의 가로 세로 반사이즈
	m_ScreenHalf.y = a_Center.y;

	m_CamPos.x = a_CamPos.x;
	m_CamPos.y = a_CamPos.y;
}

void CBackGround::BGMgrRender(ID2D1HwndRenderTarget* a_pd2dRTarget)
{
	// 이쪽 계사닝 좀 더 부드럽게 움직인다.
	m_TileStPos.x = - m_WorldHfSize.x;
	m_TileStPos.y = - m_WorldHfSize.y;

	static int a_SetPosX = 0;
	static int a_SetPosY = 0;
	static int a_CalcPosX = 0;
	static int a_CalcPosY = 0;

	for (int yy = 0; yy < Y_Tile; yy++) {
		a_SetPosY = m_TileStPos.y + (m_ImgSize.y * yy);
		for (int xx = 0; xx < X_Tile; xx++) {
			a_SetPosX = m_TileStPos.x + (m_ImgSize.x * xx);

			a_CalcPosX = a_SetPosX - (int)m_CamPos.x;
			a_CalcPosY = a_SetPosY - (int)m_CamPos.y;

			a_pd2dRTarget->DrawBitmap(m_pBGBmp, D2D1::RectF(a_CalcPosX, a_CalcPosY, a_CalcPosX + m_ImgSize.x, a_CalcPosY + m_ImgSize.y));
		}
	}
}

void CBackGround::BGMgrDestroy()
{
}

CBackGround g_BG_Mgr;