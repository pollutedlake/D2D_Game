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
	//------ 몬스터 이미지 로딩하기
	ID2D1Bitmap* a_ImgNode = NULL;

	if (D2DLoadBmp != NULL) {
		a_ImgNode = D2DLoadBmp(_T(".\\MonsterImg\\Zombi1.png"));
	}

	m_ImgList.push_back(a_ImgNode);
	//------ 몬스터 이미지 로딩하기

	//--- 몬스터 메모리풀로 미리 만들어 놓기
	CMonster* a_Node = NULL;
	for (int aii = 0; aii < 20; aii++) {
		a_Node = new CMonster();
		a_Node->Init_Unit();
		m_InActMonList.push_back(a_Node);
	}
	//--- 몬스터 메모리풀로 미리 만들어 놓기
}

void CMonster_Mgr::MonMgr_Update(double a_DeltaTime, VecINT2D& a_Center, Vector2D& a_CamPos, CHero& a_Hero)
{
	m_ScCollRadius = sqrt(a_Center.x * a_Center.x + a_Center.y * a_Center.y);
	m_ScCollRadius = m_ScCollRadius + 50.0f;

	m_ScreenHalf = a_Center;		// MainDC의 가로 세로 반사이즈
	m_CamPos = a_CamPos;

	//------ 주기적인 Monster Spawn
	SpawnMonster(a_DeltaTime);
	//------ 주기적인 Monster Spawn

	//------ Monster AI
	static CMonster* a_OwnMon = NULL;
	for (a_iter = m_ActMonList.begin(); a_iter != m_ActMonList.end(); a_iter++) {
		a_OwnMon = (*a_iter);

		a_OwnMon->Update_Unit(a_DeltaTime, a_Center, a_CamPos, a_Hero);
	}	// for (a_iter = m_ActMonList.begin(); a_iter != m_ActMonList.end(); a_iter++)
	//------ Monster AI
}

void CMonster_Mgr::MonMgr_Render(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush)
{
	//--- 몬스터 이미지 렌더링...
	for (a_iter = m_ActMonList.begin(); a_iter != m_ActMonList.end(); a_iter++) {
		if ((*a_iter)->m_InScRect == false) {		// 컬링
			continue;
		}

		(*a_iter)->Render_Unit(a_pd2dRTarget, a_pBrush);
	}
	//--- 몬스터 이미지 렌더링...
}

void CMonster_Mgr::MonEdit_Render(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush, VecINT2D& a_Center, Vector2D& a_CamPos)
{
	static Vector2D a_CalcVec;
	for (int ii = 0; ii < m_SpawnPos.size(); ii++) {
		a_CalcVec.x = -(float)a_Center.x;
		a_CalcVec.y = -(float)a_Center.y;

		a_CalcVec = m_SpawnPos[ii].m_Pos - a_CamPos + a_CalcVec;		// 주인공 위치와의 거리...
		if (m_ScCollRadius < a_CalcVec.Magnitude()) {
			continue;
		}
		m_SpawnPos[ii].Render_SpPos(a_pd2dRTarget, a_pBrush, a_Center, a_CamPos);
	}
}

void CMonster_Mgr::MonMgr_Destroy()
{
	//------ 몬스터 이미지 제거하기
	for (int aii = 0; aii < m_ImgList.size(); aii++) {
		if (m_ImgList[aii] != NULL) {
			m_ImgList[aii]->Release();
			m_ImgList[aii] = NULL;
		}
	}
	m_ImgList.clear();
	//------ 몬스터 이미지 제거하기

	// 몬스터 메모리풀 제거하기...
	for (a_iter = m_ActMonList.begin(); a_iter != m_ActMonList.end(); a_iter++) {
		if ((*a_iter) != NULL) {
			(*a_iter)->Destroy_Unit();

			delete (*a_iter);
			(*a_iter) = NULL;
		}
	}	// for (a_iter = m_ActMonList.begin(); a_iter != m_ActMonList.end(); a_iter++)
	m_ActMonList.clear();

	for (a_iter = m_InActMonList.begin(); a_iter != m_InActMonList.end(); a_iter++) {
		if ((*a_iter) != NULL) {
			(*a_iter)->Destroy_Unit();

			delete (*a_iter);
			(*a_iter) = NULL;
		}
	}	// for (a_iter = m_InActMonList.begin(); a_iter != m_InActMonList.end(); a_iter++)
	m_InActMonList.clear();
	// 몬스터 메모리풀 제거하기...

	m_SpawnPos.clear();		// <--- 스폰 리스트 지우기
}

void CMonster_Mgr::SpawnMonster(float a_DeltaTime)
{
	//--- 몬스터 Active 감시 및 마리수 카운트
	static int g_MonCount = 0;
	g_MonCount = m_ActMonList.size();
	//--- 몬스터 Active 감시 및 마리수 카운트
	CMonster* a_Node = NULL;
	for (int ii = 0; ii < m_SpawnPos.size(); ii++) {
		if (m_SpawnPos[ii].Update_SpPos(a_DeltaTime) == false) {
			continue;
		}

		a_Node = NULL;
		if (0 < m_InActMonList.size()) {
			a_iter = m_InActMonList.begin();		// 활동하지 않는 몬스터 첫번째 노드를 꺼내와서
			a_Node = (*a_iter);
			m_InActMonList.erase(a_iter);		// <--- 활동하지 않는 몬스터 리스트에서는 제거
			m_ActMonList.push_back(a_Node);		// 활동 노드 몬스터 리스트로 추가해 준다.
		}

		if (a_Node == NULL) {
			a_Node = new CMonster();		// 새로 추가
			a_Node->Init_Unit();
			m_ActMonList.push_back(a_Node);
		}

		if (a_Node != NULL) {
			a_Node->m_CharicType = CT_Zombie1;
			a_Node->Spawn(m_SpawnPos[ii].m_Pos.x, m_SpawnPos[ii].m_Pos.y, m_ImgList[0]);
			a_Node->m_SpawnIdx = ii;
		}
	}	// for (int ii = 0; ii < m_SpawnPos.size(); ii++)
}

void CMonster_Mgr::ReSrcClear()
{
	static CMonster* a_MonNode = NULL;
	// 활동 하고 있는 모든 몬스터들의 리스트를 휴식 리스트로 옮긴다.
	for (a_iter = m_ActMonList.begin(); a_iter != m_ActMonList.end(); a_iter++) {
		a_MonNode = (*a_iter);
		// a_iter = m_ActMonList.erase(a_iter);

		if (a_MonNode != NULL) {
			a_MonNode->m_isActive = false;
			a_MonNode->m_SpawnIdx = -1;
			m_InActMonList.push_back(a_MonNode);
		}
	}
	m_ActMonList.clear();
}

void CMonster_Mgr::AddMonSpPos(Vector2D& a_SpPos)
{
	Vector2D a_CalcVec;
	for (int ii = 0; ii < m_SpawnPos.size(); ii++) {
		a_CalcVec = a_SpPos - m_SpawnPos[ii].m_Pos;

		if (a_CalcVec.Magnitude() < 40) {
			m_SpawnPos.erase(m_SpawnPos.begin() + ii);		// 이미 설치되어 있는 위치면 삭제
			return;
		}
	}

	SpawnPos a_Temp;
	a_Temp.m_Pos = a_SpPos;
	a_Temp.m_SocketImg = g_Mon_Mgr.m_ImgList[0];
	m_SpawnPos.push_back(a_Temp);
}

CMonster_Mgr g_Mon_Mgr;