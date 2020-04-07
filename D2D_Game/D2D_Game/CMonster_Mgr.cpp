#include "framework.h"
#include "CMonster_Mgr.h"
#include "CMonster.h"
#include "CHero.h"

CMonster_Mgr::CMonster_Mgr()
{
}

CMonster_Mgr::~CMonster_Mgr()
{
}

void CMonster_Mgr::MonMgr_Init(HWND a_hWnd, ID2D1Bitmap* (*D2DLoadBmp)(LPCWSTR FName))
{
	//------ ���� �̹��� �ε��ϱ�
	ID2D1Bitmap* a_ImgNode = NULL;

	if (D2DLoadBmp != NULL) {
		a_ImgNode = D2DLoadBmp(_T(".\\MonsterImg\\Zombi1.png"));
	}

	m_ImgList.push_back(a_ImgNode);
	//------ ���� �̹��� �ε��ϱ�
}

void CMonster_Mgr::MonMgr_Update(double a_DeltaTime, VecINT2D& a_Center, Vector2D& a_CamPos, CHero& a_Hero)
{
	m_ScCollRadius = sqrt(a_Center.x * a_Center.x + a_Center.y * a_Center.y);
	m_ScCollRadius = m_ScCollRadius + 50.0f;

	m_ScreenHalf = a_Center;		// MainDC�� ���� ���� �ݻ�����
	m_CamPos = a_CamPos;
}

void CMonster_Mgr::MonMgr_Render(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush)
{
}

void CMonster_Mgr::MonEdit_Render(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush, VecINT2D& a_Center, Vector2D& a_CamPos)
{
	static Vector2D a_CalcVec;
	for (int ii = 0; ii < m_SpawnPos.size(); ii++) {
		a_CalcVec.x = -(float)a_Center.x;
		a_CalcVec.y = -(float)a_Center.y;

		a_CalcVec = m_SpawnPos[ii].m_Pos - a_CamPos + a_CalcVec;		// ���ΰ� ��ġ���� �Ÿ�...
		if (m_ScCollRadius < a_CalcVec.Magnitude()) {
			continue;
		}
		m_SpawnPos[ii].Render_SpPos(a_pd2dRTarget, a_pBrush, a_Center, a_CamPos);
	}
}

void CMonster_Mgr::MonMgr_Destroy()
{
	//------ ���� �̹��� �����ϱ�
	for (int aii = 0; aii < m_ImgList.size(); aii++) {
		if (m_ImgList[aii] != NULL) {
			m_ImgList[aii]->Release();
			m_ImgList[aii] = NULL;
		}
	}
	m_ImgList.clear();
	//------ ���� �̹��� �����ϱ�

	m_SpawnPos.clear();		// <--- ���� ����Ʈ �����
}

void CMonster_Mgr::ReSrcClear()
{

}

void CMonster_Mgr::AddMonSpPos(Vector2D& a_SpPos)
{
	Vector2D a_CalcVec;
	for (int ii = 0; ii < m_SpawnPos.size(); ii++) {
		a_CalcVec = a_SpPos - m_SpawnPos[ii].m_Pos;

		if (a_CalcVec.Magnitude() < 40) {
			m_SpawnPos.erase(m_SpawnPos.begin() + ii);		// �̹� ��ġ�Ǿ� �ִ� ��ġ�� ����
			return;
		}
	}

	SpawnPos a_Temp;
	a_Temp.m_Pos = a_SpPos;
	a_Temp.m_SocketImg = g_Mon_Mgr.m_ImgList[0];
	m_SpawnPos.push_back(a_Temp);
}

CMonster_Mgr g_Mon_Mgr;