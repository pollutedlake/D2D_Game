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
	//------ �Ѿ� �̹��� �ε�
	if (m_MachineGunImg == NULL) {
		m_MachineGunImg = D2DLoadBmp(_T(".\\RscTrunk\\Effect\\HeavyMachineGunBullet.png"));
		m_McGunWidth = 40.0f;
		m_McGunHeight = 10.0f;
		// m_Radius = 25.0f;		// �Ѿ� �浹 �ݰ�
	}

	if (m_RocketImg == NULL) {
		m_RocketImg = D2DLoadBmp(_T(".\\RscTrunk\\Weapon\\Missile.png"));
		m_RocketWidth = 40.0f;
		m_RocketHeight = 10.0f;
		// m_Radius = 25.0f;		// �Ѿ� �浹 �ݰ�
	}
	//------ �Ѿ� �̹��� �ε�

	//--- �Ѿ� �޸�Ǯ�� �̸� ����� ����
	CBullet* a_BLNode = NULL;
	for (int aii = 0; aii < 200; aii++) {
		a_BLNode = new CBullet();
		m_BullList.push_back(a_BLNode);
	}
	//--- �Ѿ� �޸�Ǯ�� �̸� ����� ����
}

void CBullet_Mgr::BLMgerUpdate(float a_DwdDeltaTime, DWORD a_DwdLastTime, float(*GetDelta)(int WpType), bool(*CheckColl)(CBullet* a_RefBull, DWORD a_CurTime), void(*GenSmokePtc)(Vector2D a_StartV, DWORD a_CurTime, Vector2D a_DirVec))
{
	//------ ��� �Ѿ� �̵����� ������Ʈ ó��
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

		// �Ѿ��� ���Ϳ� �¾����� ������ �ش�. ���� ���� �Ѿ� ����
		a_IsColl = CheckColl(m_BullList[aii], a_DwdLastTime);		// �浹üũ �Լ� ������
		if (a_IsColl == true) {
			m_BullList[aii]->m_BLActive = false;		// �Ѿ� ����
		}
	}
	//------ ��� �Ѿ� �̵����� ������Ʈ ó��
}

void CBullet_Mgr::BLMgerRender(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush)
{
	//--- �Ѿ� ������
	for (int aii = 0; aii < m_BullList.size(); aii++) {
		if (m_BullList[aii]->m_BLActive == false) {
			continue;
		}

		m_BullList[aii]->BulletRender(a_pd2dRTarget, a_pBrush, m_CamPos);
	}	// for (int aii = 0; aii < m_BullList.size(); aii++)
	//--- �Ѿ� ������
}

void CBullet_Mgr::BLMgerDestroy()
{
	// �Ѿ� �޸�Ǯ �����ϱ�...
	for (int aii = 0; aii < m_BullList.size(); aii++) {
		if (m_BullList[aii] != NULL) {
			delete m_BullList[aii];
			m_BullList[aii] = NULL;
		}
	}
	m_BullList.clear();
	// �Ѿ� �޸�Ǯ �����ϱ�...

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