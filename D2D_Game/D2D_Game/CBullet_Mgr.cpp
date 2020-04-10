#include "framework.h"
#include "CBullet_Mgr.h"

CBullet_Mgr::CBullet_Mgr()
{
}

CBullet_Mgr::~CBullet_Mgr()
{
}

void CBullet_Mgr::BLMgerInit(ID2D1Bitmap* (*D2DLoadBmp)(LPCWSTR FName))
{
	//------ 총알 이미지 로딩
	if (m_MachineGunImg == NULL) {
		m_MachineGunImg = D2DLoadBmp(_T(".\\RscTrunk\\Effect\\HeavyMachineGunBullet.png"));
		m_McGunWidth = 40.0f;
		m_McGunHeight = 10.0f;
		// m_Radius = 25.0f;		// 총알 충돌 반경
	}

	if (m_RocketImg == NULL) {
		m_RocketImg = D2DLoadBmp(_T(".\\RscTrunk\\Weapon\\Missile.png"));
		m_RocketWidth = 40.0f;
		m_RocketHeight = 10.0f;
		// m_Radius = 25.0f;		// 총알 충돌 반경
	}
	//------ 총알 이미지 로딩

	//--- 총알 메모리풀로 미리 만들어 놓기
	CBullet* a_BLNode = NULL;
	for (int aii = 0; aii < 200; aii++) {
		a_BLNode = new CBullet();
		m_BullList.push_back(a_BLNode);
	}
	//--- 총알 메모리풀로 미리 만들어 놓기
}

void CBullet_Mgr::BLMgerUpdate(float a_DwdDeltaTime, DWORD a_DwdLastTime, float(*GetDelta)(int WpType), bool(*CheckColl)(CBullet* a_RefBull, DWORD a_CurTime), void(*GenSmokePtc)(Vector2D a_StartV, DWORD a_CurTime, Vector2D a_DirVec))
{
	//------ 모든 총알 이동관련 업데이트 처리
	static bool a_IsColl = false;
	static float a_WDelta = 0.0f;
	for (int aii = 0; aii < m_BullList.size(); aii++) {
		if (m_BullList[aii]->m_BLActive == false) {
			continue;
		}

		a_WDelta = a_DwdDeltaTime;
		if (GetDelta != NULL) {
			a_WDelta = GetDelta(m_BullList[aii]->WeaponType);
		}

		m_BullList[aii]->BulletUpdate(a_DwdDeltaTime, a_DwdLastTime, a_WDelta, GenSmokePtc);

		if (CheckColl == NULL) {
			continue;
		}

		// 총알이 몬스터에 맞았으면 제거해 준다. 몬스터 제거 총알 제거
		a_IsColl = CheckColl(m_BullList[aii], a_DwdLastTime);		// 충돌체크 함수 포인터
		if (a_IsColl == true) {
			m_BullList[aii]->m_BLActive = false;		// 총알 제거
		}
	}
	//------ 모든 총알 이동관련 업데이트 처리
}

void CBullet_Mgr::BLMgerRender(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush)
{
	//--- 총알 렌더링
	for (int aii = 0; aii < m_BullList.size(); aii++) {
		if (m_BullList[aii]->m_BLActive == false) {
			continue;
		}

		m_BullList[aii]->BulletRender(a_pd2dRTarget, a_pBrush, m_CamPos);
	}	// for (int aii = 0; aii < m_BullList.size(); aii++)
	//--- 총알 렌더링
}

void CBullet_Mgr::BLMgerDestroy()
{
	// 총알 메모리풀 제거하기...
	for (int aii = 0; aii < m_BullList.size(); aii++) {
		if (m_BullList[aii] != NULL) {
			delete m_BullList[aii];
			m_BullList[aii] = NULL;
		}
	}
	m_BullList.clear();
	// 총알 메모리풀 제거하기...

	if (m_MachineGunImg != NULL) {
		m_MachineGunImg->Release();
		m_MachineGunImg = NULL;
	}

	if (m_RocketImg != NULL) {
		m_RocketImg->Release();
		m_RocketImg = NULL;
	}
}

CBullet_Mgr g_Bullet_Mgr;