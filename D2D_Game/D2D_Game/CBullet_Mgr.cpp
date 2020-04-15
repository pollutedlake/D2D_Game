#include "framework.h"
#include "CBullet_Mgr.h"
#include "GlobalValue.h"

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
		m_RocketWidth = 100.0f;
		m_RocketHeight = 32.0f;
		// m_Radius = 40.0f;		// �Ѿ� �浹 �ݰ�
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

void CBullet_Mgr::SpawnPistol(Vector2D a_StartV, Vector2D a_TargetV)
{
	static Vector2D SpawnLocation;
	static float Ang = 0.0f;

	//-------------- void Weapon_Pistol::HandleShoot
	static Vector2D a_CacDirVec;
	a_CacDirVec = a_TargetV - a_StartV;
	a_CacDirVec.Normalize();

	SpawnLocation = a_StartV + a_CacDirVec * 40.0f; //50.0; //* 20.0f;
	Ang = a_CacDirVec.GetAngle(); //���͸� ������ ȯ��
	a_CacDirVec = Vector2D::GetNormalFromRotation(Ang); //������ ���ͷ� ȯ��
	//-------------- void Weapon_Pistol::HandleShoot

	for (int aii = 0; aii < m_BullList.size(); aii++)
	{
		if (m_BullList[aii]->m_BLActive == false)
		{
			m_BullList[aii]->WeaponType = EWeaponSlots::PISTOL;
			m_BullList[aii]->m_TexImgWidth = m_PistolWidth;
			m_BullList[aii]->m_TexImgHeight = m_PistolHeight;
			m_BullList[aii]->m_Radius = 10.0f; //�Ѿ� �浹 �ݰ�
			m_BullList[aii]->m_MoveSpeed = 1000.0f;

			m_BullList[aii]->m_CurPos.x = SpawnLocation.x;
			m_BullList[aii]->m_CurPos.y = SpawnLocation.y;
			m_BullList[aii]->m_RenderPos.x = SpawnLocation.x;
			m_BullList[aii]->m_RenderPos.y = SpawnLocation.y;

			m_BullList[aii]->m_DirVec = a_CacDirVec;
			m_BullList[aii]->m_DirRot = Ang;
			m_BullList[aii]->m_BLActive = true;
			m_BullList[aii]->m_Duration = 2.0f;

			return;
		}
	}

	static CBullet* a_BNode = NULL;
	a_BNode = new CBullet();

	a_BNode->WeaponType = EWeaponSlots::PISTOL;
	a_BNode->m_TexImgWidth = m_PistolWidth;
	a_BNode->m_TexImgHeight = m_PistolHeight;
	a_BNode->m_Radius = 10.0f; //�Ѿ� �浹 �ݰ�
	a_BNode->m_MoveSpeed = 1000.0f;

	a_BNode->m_CurPos.x = SpawnLocation.x;
	a_BNode->m_CurPos.y = SpawnLocation.y;
	a_BNode->m_RenderPos.x = SpawnLocation.x;
	a_BNode->m_RenderPos.y = SpawnLocation.y;

	a_BNode->m_DirVec = a_CacDirVec;
	a_BNode->m_DirRot = Ang;
	a_BNode->m_BLActive = true;
	a_BNode->m_Duration = 2.0f;

	m_BullList.push_back(a_BNode);
}

void CBullet_Mgr::SpawnMachineGun(Vector2D a_StartV, Vector2D a_TargetV)
{
	static Vector2D SpawnLocation;
	static float Ang = 0.0f;

	//------- void Weapon_HeavyMachineGun::HandleShoot
	static Vector2D a_CalcDirVec;
	a_CalcDirVec = a_TargetV - a_StartV;
	a_CalcDirVec.Normalize();

	SpawnLocation = a_StartV + a_CalcDirVec * 50.0;

	Ang = a_CalcDirVec.GetAngle();		// ���͸� ������ ȯ��
	Ang += RandInt(-1000, 1000) / 500.0f;		// +- 2��
	a_CalcDirVec = Vector2D::GetNormalFromRotation(Ang);		// ������ ���ͷ� ȯ��
	a_CalcDirVec.Normalize();

	SpawnLocation = SpawnLocation + Vector2D(RandInt(-5, 5), RandInt(-5, 5));
	//------- void Weapon_HeavyMachineGun::HandleShoot

	for (int aii = 0; aii < m_BullList.size(); aii++) {
		if (m_BullList[aii]->m_BLActive == false) {
			m_BullList[aii]->WeaponType = EWeaponSlots::HEAVY_MACHINE_GUN;
			m_BullList[aii]->m_BulletImg = m_MachineGunImg;
			m_BullList[aii]->m_TexImgWidth = m_McGunWidth;
			m_BullList[aii]->m_TexImgHeight = m_McGunHeight;
			m_BullList[aii]->m_Radius = 25.0f;		// �Ѿ� �浹 �ݰ�
			m_BullList[aii]->m_MoveSpeed = 1700.0f;

			m_BullList[aii]->m_CurPos.x = SpawnLocation.x;
			m_BullList[aii]->m_CurPos.y = SpawnLocation.y;
			m_BullList[aii]->m_RenderPos.x = SpawnLocation.x;
			m_BullList[aii]->m_RenderPos.y = SpawnLocation.y;

			m_BullList[aii]->m_DirVec = a_CalcDirVec;
			m_BullList[aii]->m_DirRot = Ang;
			m_BullList[aii]->m_BLActive = true;
			m_BullList[aii]->m_Duration = 2.0f;
			
			return;
		}	// if (m_BullList[aii]->m_BLActive == false)
	}	// for (int aii = 0; aii < m_BullList.size(); aii++)

	static CBullet* m_BLNode = NULL;
	m_BLNode = new CBullet();

	m_BLNode->WeaponType = EWeaponSlots::HEAVY_MACHINE_GUN;
	m_BLNode->m_BulletImg = m_MachineGunImg;
	m_BLNode->m_TexImgWidth = m_McGunWidth;
	m_BLNode->m_TexImgHeight = m_McGunHeight;
	m_BLNode->m_Radius = 25.0f;		// �Ѿ� �浹 �ݰ�
	m_BLNode->m_MoveSpeed = 1700.0f;

	m_BLNode->m_CurPos.x = SpawnLocation.x;
	m_BLNode->m_CurPos.y = SpawnLocation.y;
	m_BLNode->m_RenderPos.x = SpawnLocation.x;
	m_BLNode->m_RenderPos.y = SpawnLocation.y;

	m_BLNode->m_DirVec = a_CalcDirVec;
	m_BLNode->m_DirRot = Ang;
	m_BLNode->m_BLActive = true;
	m_BLNode->m_Duration = 2.0f;

	m_BullList.push_back(m_BLNode);
}

void CBullet_Mgr::SpawnRocket(Vector2D a_StartV, Vector2D a_TargetV)
{
	static Vector2D SpawnLocation;
	static float Ang = 0.0f;

	//------ void Weapon_RocketLauncher::HandleShoot
	static Vector2D a_CalcDirVec;
	a_CalcDirVec = a_TargetV - a_StartV;
	a_CalcDirVec.Normalize();

	SpawnLocation = a_StartV + a_CalcDirVec * 50.0;

	Ang = a_CalcDirVec.GetAngle();		// ���͸� ������ ȯ��

	SpawnLocation = SpawnLocation + Vector2D(RandInt(-5, 5), RandInt(-5, 5));
	//------ void Weapon_RocketLauncher::HandleShoot

	for (int aii = 0; aii < m_BullList.size(); aii++) {
		if (m_BullList[aii]->m_BLActive == false) {
			m_BullList[aii]->WeaponType = EWeaponSlots::ROCKET_LAUNCHER;
			m_BullList[aii]->m_BulletImg = m_RocketImg;
			m_BullList[aii]->m_TexImgWidth = m_RocketWidth;
			m_BullList[aii]->m_TexImgHeight = m_RocketHeight;
			m_BullList[aii]->m_Radius = 40.0f;
			m_BullList[aii]->m_MoveSpeed = 1700.0f;

			m_BullList[aii]->m_CurPos.x = SpawnLocation.x;
			m_BullList[aii]->m_CurPos.y = SpawnLocation.y;
			m_BullList[aii]->m_RenderPos.x = SpawnLocation.x;
			m_BullList[aii]->m_RenderPos.y = SpawnLocation.y;

			m_BullList[aii]->m_DirVec = a_CalcDirVec;
			m_BullList[aii]->m_DirRot = Ang;
			m_BullList[aii]->m_BLActive = true;
			m_BullList[aii]->m_Duration = 2.0f;

			return;
		}	// if (m_BullList[aii]->m_BLActive == false)
	}	// for (int aii = 0; aii < m_BullList.size(); aii++)

	static CBullet* m_BLNode = NULL;
	m_BLNode = new CBullet();

	m_BLNode->WeaponType = EWeaponSlots::ROCKET_LAUNCHER;
	m_BLNode->m_BulletImg = m_RocketImg;
	m_BLNode->m_TexImgWidth = m_RocketWidth;
	m_BLNode->m_TexImgHeight = m_RocketHeight;
	m_BLNode->m_Radius = 40.0f;		// �Ѿ� �浹 �ݰ�
	m_BLNode->m_MoveSpeed = 1700.0f;

	m_BLNode->m_CurPos.x = SpawnLocation.x;
	m_BLNode->m_CurPos.y = SpawnLocation.y;
	m_BLNode->m_RenderPos.x = SpawnLocation.x;
	m_BLNode->m_RenderPos.y = SpawnLocation.y;

	m_BLNode->m_DirVec = a_CalcDirVec;
	m_BLNode->m_DirRot = Ang;
	m_BLNode->m_BLActive = true;
	m_BLNode->m_Duration = 2.0f;

	m_BullList.push_back(m_BLNode);
}

CBullet_Mgr g_Bullet_Mgr;