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

	// �̺�Ʈ �Լ� ���... ���ٽ�����...
	a_BNode->OnClick_AddListener([this, a_BNode]() {
		int a_WpIdx = ChangeWeapon();		// ���� ���� ��ü
		a_BNode->ChangeSubImg(a_WpIdx);		// �̹��� ��ü
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

bool CUIManager::OnLButtonDown(HWND hWnd)		// ���콺�� ������ ���� �ѹ� �߻��Ѵ�.
{
	static POINT a_MouseXY;
	GetCursorPos(&a_MouseXY);		// ���� ���콺 ��ǥ�� ������ �Լ�
	ScreenToClient(hWnd, &a_MouseXY);		// Ŭ���̾�Ʈ ȭ�� ��ǥ�� ������ �ִ� �Լ�

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

void CUIManager::OnLButtonUp()		// ���콺 ���� ���� �ѹ� �߻��Ѵ�.
{
	for (int aii = 0; aii < m_InGameUIList.size(); aii++) {
		m_InGameUIList[aii]->OnLButtonUp();
	}
}

bool CUIManager::IsPickLock(HWND hWnd)
{
	static POINT a_MouseXY;
	GetCursorPos(&a_MouseXY);		// ���� ���콺 ��ǥ�� ������ �Լ�
	ScreenToClient(hWnd, &a_MouseXY);		// Ŭ���̾�Ʈ ȭ�� ��ǥ�� ������ �ִ� �Լ�

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