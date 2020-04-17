#include "framework.h"
#include "CUIManager.h"
#include "CButton.h"
#include "GlobalValue.h"

CUIManager::CUIManager()
{
}

CUIManager::~CUIManager()
{
}

void CUIManager::UIMgrInit(HWND a_hWnd, ID2D1Bitmap* (*D2DLoadBmp)(LPCWSTR FName))
{
	m_hWnd = a_hWnd;

	CButton* a_BNode = new CButton();
	a_BNode->m_pBtnUpImg = D2DLoadBmp(_T(".\\RscTrunk\\UI\\ETCButtonNormal.png"));
	a_BNode->m_pBtnDownImg = D2DLoadBmp(_T(".\\RscTrunk\\UI\\ETCButtonPressed.png"));
	a_BNode->UI_Init(a_hWnd, 0.94f, 0.88f, 0.35f, 0.35f);

	ID2D1Bitmap* a_pImg = NULL;
	a_pImg = D2DLoadBmp(_T(".\\RscTrunk\\UI\\_Pistol.png"));
	a_BNode->AddSubImg(a_pImg, 0.45f, 0.45f, 0.5f, 0.56f);
	a_pImg = D2DLoadBmp(_T(".\\RscTrunk\\UI\\_machinegun.png"));
	a_BNode->AddSubImg(a_pImg, 0.45f, 0.41f, 0.5f, 0.57f);
	a_pImg = D2DLoadBmp(_T(".\\RscTrunk\\UI\\_RocketLauncher.png"));
	a_BNode->AddSubImg(a_pImg, 0.5f, 0.5f, 0.5f, 0.56f);
	a_BNode->ChangeSubImg(1);

	// 이벤트 함수 등록... 람다식으로...
	a_BNode->OnClick_AddListener([this, a_BNode]() {
		int a_WpIdx = ChangeWeapon();		// 실제 무기 교체
		a_BNode->ChangeSubImg(a_WpIdx);		// 이미지 교체
		});

	m_InGameUIList.push_back(a_BNode);
}

void CUIManager::UIMgrUpdate(double a_DeltaTime)
{
	for (int aii = 0; aii < m_InGameUIList.size(); aii++) {
		m_InGameUIList[aii]->UI_Update(a_DeltaTime);
	}
}

void CUIManager::UIMgrRender(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush)
{
	for (int aii = 0; aii < m_InGameUIList.size(); aii++) {
		m_InGameUIList[aii]->UI_Render(a_pd2dRTarget, a_pBrush);
	}	// for (int aii = 0; aii < m_InGameUIList.size(); aii++)
}

void CUIManager::UIMgrDestroy()
{
	for (int aii = 0; aii < m_InGameUIList.size(); aii++) {
		if (m_InGameUIList[aii] != NULL) {
			delete m_InGameUIList[aii];
			m_InGameUIList[aii] = NULL;
		}
	}
	m_InGameUIList.clear();
}

bool CUIManager::OnLButtonDown(HWND hWnd)		// 마우스를 누르는 순간 한번 발생한다.
{
	static POINT a_MouseXY;
	GetCursorPos(&a_MouseXY);		// 현재 마우스 좌표를 얻어오는 함수
	ScreenToClient(hWnd, &a_MouseXY);		// 클라이언트 화면 좌표로 변경해 주는 함수

	static bool a_IsPickLock = false;
	a_IsPickLock = false;
	for (int aii = 0; aii < m_InGameUIList.size(); aii++) {
		a_IsPickLock = m_InGameUIList[aii]->OnLButtonDown(a_MouseXY);
		if (a_IsPickLock == true) {
			break;
		}
	}

	return a_IsPickLock;
}

void CUIManager::OnLButtonUp()		// 마우스 떼는 순간 한번 발생한다.
{
	for (int aii = 0; aii < m_InGameUIList.size(); aii++) {
		m_InGameUIList[aii]->OnLButtonUp();
	}
}

bool CUIManager::IsPickLock(HWND hWnd)
{
	static POINT a_MouseXY;
	GetCursorPos(&a_MouseXY);		// 현재 마우스 좌표를 얻어오는 함수
	ScreenToClient(hWnd, &a_MouseXY);		// 클라이언트 화면 좌표로 변경해 주는 함수

	static bool a_IsPickLock = false;
	a_IsPickLock = false;
	for (int aii = 0; aii < g_UI_Mgr.m_InGameUIList.size(); aii++) {
		a_IsPickLock = g_UI_Mgr.m_InGameUIList[aii]->IsPickLock(a_MouseXY);
		if (a_IsPickLock == true) {
			break;
		}
	}

	return a_IsPickLock;
}

CUIManager g_UI_Mgr;