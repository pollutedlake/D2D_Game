#pragma once

#include "SpawnPos.h"
#include <list>
using namespace std;

class CMonster;
class CHero;

class CMonster_Mgr
{
	list<CMonster*> m_ActMonList;		// Active 활동중인 몬스터 리스트	<--- 메모리 풀 방식
	list<CMonster*> m_InActMonList;		// InActive 활동하지 않는 몬스터 리스트		<--- 메모리 풀 방식
	list<CMonster*>::iterator a_iter;	// <--- 메모리 풀 순환용 변수

public:
	vector<ID2D1Bitmap*> m_ImgList;		// 몬스터 이미지 리스트
	vector<SpawnPos> _SpawnPos;			// 몬스터 스폰 위치

public:
	CMonster_Mgr();
	~CMonster_Mgr();

public:
	void MonMgr_Init(HWND a_hWnd, ID2D1Bitmap* (*D2DLoadBitmap)(LPCWSTR FName) = NULL);
	void MonMgr_Update(double a_DeltaTime, VecINT2D& a_Center, Vector2D& a_CamPos, CHero& a_Hero);
	void MonMgr_Render(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush);
	void MonEdit_Render(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush, VecINT2D& a_Center, Vector2D& a_CamPos);
	void MonMgr_Destroy();
};

extern CMonster_Mgr g_Mon_Mgr;

