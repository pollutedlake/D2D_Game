#include "framework.h"
#include "CParticle_Mgr.h"
#include "GlobalValue.h"

ID2D1Bitmap* CParticle_Mgr::m_Ptc_MGun_Img = NULL;
ID2D1Bitmap* CParticle_Mgr::m_Smoke_Img = NULL;
ID2D1Bitmap* CParticle_Mgr::m_MissileExp_Img = NULL;

CParticle_Mgr::CParticle_Mgr()
{
}

CParticle_Mgr::~CParticle_Mgr()
{
}

void CParticle_Mgr::PtcMgrInit(ID2D1Bitmap* (D2DLoadBmp)(LPCWSTR FName))
{
	if (CParticle_Mgr::m_Ptc_MGun_Img == NULL) {
		CParticle_Mgr::m_Ptc_MGun_Img = D2DLoadBmp(_T(".\\RscTrunk\\Effect\\Glow.png"));
	}

	if (CParticle_Mgr::m_Smoke_Img == NULL) {
		CParticle_Mgr::m_Smoke_Img = D2DLoadBmp(_T(".\\RscTrunk\\Effect\\Fog1.png"));
	}

	if (CParticle_Mgr::m_MissileExp_Img == NULL) {
		CParticle_Mgr::m_MissileExp_Img = D2DLoadBmp(_T(".\\RscTrunk\\Effect\\ExplosionSheet.png"));
	}

	//--- ��ƼŬ ������ �޸�Ǯ�� �̸� ����� ����
	CPtcEmitter* a_BLNode = NULL;
	for (int aii = 0; aii < 500; aii++) {
		a_BLNode = new CPtcEmitter();
		m_ParticleList.push_back(a_BLNode);
	}
	//--- ��ƼŬ ������ �޸�Ǯ�� �̸� ����� ����

	m_MissileExpList.reserve(500);
}

void CParticle_Mgr::PtcMgrUpdate(DWORD a_CurTime, VecINT2D& a_Center, Vector2D& a_CamPos)
{
	for (int aii = 0; aii < m_ParticleList.size(); aii++) {
		if (m_ParticleList[aii]->m_EmtActive == false) {
			continue;
		}

		m_ParticleList[aii]->Emt_Update(a_CurTime, a_Center, a_CamPos);
	}
}

void CParticle_Mgr::PtcMgrRender(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush)
{
	//--- ��ƼŬ ������ ������
	for (int aii = 0; aii < m_ParticleList.size(); aii++) {
		if (m_ParticleList[aii]->m_EmtActive == false) {
			continue;
		}
		
		if (m_ParticleList[aii]->m_MslExp_Img != NULL) {
			continue;
		}

		m_ParticleList[aii]->Emt_Render(a_pd2dRTarget, a_pBrush);
	}	// for (int aii = 0; aii < m_ParticleList.size(); aii++)
	//--- ��ƼŬ ������ ������
}

void CParticle_Mgr::PtcMgrDestroy()
{
	// ��ƼŬ ������ �޸�Ǯ �����ϱ�...
	m_MissileExpList.clear();

	for (int aii = 0; aii < m_ParticleList.size(); aii++) {
		if (m_ParticleList[aii] != NULL) {
			delete m_ParticleList[aii];
			m_ParticleList[aii] = NULL;
		}
	}
	// ��ƼŬ ������ �޸�Ǯ �����ϱ�...

	if (CParticle_Mgr::m_Ptc_MGun_Img != NULL) {
		DeleteObject(CParticle_Mgr::m_Ptc_MGun_Img);		// brush object ����
		CParticle_Mgr::m_Ptc_MGun_Img = NULL;
	}

	if (CParticle_Mgr::m_Smoke_Img != NULL) {
		DeleteObject(CParticle_Mgr::m_Smoke_Img);		// brush object ����
		CParticle_Mgr::m_Smoke_Img = NULL;
	}

	if (CParticle_Mgr::m_MissileExp_Img != NULL) {
		DeleteObject(CParticle_Mgr::m_MissileExp_Img);		// brush object ����
		CParticle_Mgr::m_MissileExp_Img = NULL;
	}
}

//--- Missile ���� ���� ������
void CParticle_Mgr::MslExpMgrRender(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush)
{
	if (!m_MissileExpList.empty()) {
		static vector<CPtcEmitter*>::iterator a_iter;
		for (a_iter = m_MissileExpList.begin(); a_iter != m_MissileExpList.end();) {
			(*a_iter)->MslExpRender(a_pd2dRTarget, a_pBrush);

			if ((*a_iter)->m_EmtActive == false) {		// �̷��� ������ �ش�.
				a_iter = m_MissileExpList.erase(a_iter);
			}
			else {
				a_iter++;
			}
		}
	}	// if (!m_MissileExpList.empty())
}
//--- Missile ���� ���� ������

void CParticle_Mgr::Explosion_MGun(Vector2D a_StartV, DWORD a_CurTime)
{
	static CPtcEmitter* a_PtcEmtMchGun = NULL;
	static int aii = 0;
	for (int i = 0; i < 10; i++) {
		//------ Find ParticleEmitter
		a_PtcEmtMchGun = NULL;
		aii = 0;
		for (aii = 0; aii < m_ParticleList.size(); aii++) {
			if (m_ParticleList[aii]->m_EmtActive == false) {
				a_PtcEmtMchGun = m_ParticleList[aii];
				break;
			}
		}

		if (a_PtcEmtMchGun == NULL) {
			a_PtcEmtMchGun = new CPtcEmitter();
			m_ParticleList.push_back(a_PtcEmtMchGun);
		}

		if (a_PtcEmtMchGun != NULL) {
			a_PtcEmtMchGun->m_PtcImage = CParticle_Mgr::m_Ptc_MGun_Img;		// �ӽŰ� ��ƼŬ �̹��� ����
			a_PtcEmtMchGun->m_MslExp_Img = NULL;
			a_PtcEmtMchGun->SpawnPosition = a_StartV;
			a_PtcEmtMchGun->StartSize = Vector2D(3.0f, 1.0f);
			a_PtcEmtMchGun->EndSize = Vector2D(RandInt(5, 10), RandInt(2, 4));
			a_PtcEmtMchGun->LifeTime = RandInt(5.0f, 9.0f) * 20.0f;
			a_PtcEmtMchGun->Velocity = Vector2D::GetNormalFromRotation(RandInt(0, 360)) * RandInt(20.0f, 60.0f);
			// a_PtcEmtMchGun->ParticleColor = Direct2DHelper::RGBA(1.0f, 0.0f, 0.0f);
			a_PtcEmtMchGun->m_DirRot = a_PtcEmtMchGun->Velocity.GetAngle();		// <--- �ӵ� �������� ȸ�� �ϵ���...
			a_PtcEmtMchGun->StartAlpha = 1.0f;
			a_PtcEmtMchGun->EndAlpha = 1.0f;
			a_PtcEmtMchGun->CreatedTime = a_CurTime;
			a_PtcEmtMchGun->m_EmtActive = true;
		}
		//------ Find ParticleEmitter
	}	// for (int i = 0; i < 10; i++)
}

void CParticle_Mgr::Explosion_Missile(Vector2D a_StartV, DWORD a_CurTime, float a_RocketRot)
{
	static CPtcEmitter* a_PtcEmtMsl = NULL;
	static int aa = 0;
	a_PtcEmtMsl = NULL;
	aa = 0;
	for (aa = 0; aa < m_ParticleList.size(); aa++) {
		if (m_ParticleList[aa]->m_EmtActive == false) {
			a_PtcEmtMsl = m_ParticleList[aa];
			break;
		}
	}

	if (a_PtcEmtMsl == NULL) {
		a_PtcEmtMsl = new CPtcEmitter();
		m_ParticleList.push_back(a_PtcEmtMsl);
	}

	if (a_PtcEmtMsl != NULL) {
		a_PtcEmtMsl->m_PtcImage = NULL;		// �ӽŰ� ��ƼŬ �̹��� ����
		a_PtcEmtMsl->m_MslExp_Img = CParticle_Mgr::m_MissileExp_Img;		// �̻��� ���� �̹��� ����
		a_PtcEmtMsl->SetUV(4, 4);
		a_PtcEmtMsl->ImgRealWidth = a_PtcEmtMsl->m_MslExp_Img->GetSize().width;
		a_PtcEmtMsl->ImgRealHeight = a_PtcEmtMsl->m_MslExp_Img->GetSize().height;
		a_PtcEmtMsl->SpawnPosition = a_StartV;
		// a_PtcEmtMsl->StartSize = Vector2D(10.0f, 10.0f);
		// a_PtcEmtMsl->EndSize = Vector2D(RandInt(7, 15), RandInt(2, 3));
		a_PtcEmtMsl->LifeTime = 1.0f;		// <--- 1�� �ڿ� �ڵ� ���� �ǵ��� �Ѵ�.
		// a_PtcEmtMsl->Velocity = Vector2D::GetNormalFromRotation(a_RocketRot + RandInt(-150, 150) / 10.0f) * RandInt(50.0f, 500.0f);		// ������ ���ͷ� ȯ��
		// a_PtcEmtMsl->m_DirRot = a_PtcEmtMsl->Velocity.GetAngle();		// <--- �ӵ� �������� ȸ�� �ϵ���...
		// a_PtcEmtMsl->StartAlpha = 1.0f;
		// a_PtcEmtMsl->EndAlpha = 1.0f;
		a_PtcEmtMsl->CreatedTime = a_CurTime;		// timeGetTime() / 1000.0f;
		a_PtcEmtMsl->m_EmtActive = true;

		m_MissileExpList.push_back(a_PtcEmtMsl);
	}

	static CPtcEmitter* a_PtcEmtMchGun = NULL;
	static int aii = 0;
	a_PtcEmtMchGun = NULL;
	aii = 0;
	for (int i = 0; i < 10; i++) {
		//------ Find ParticleEmitter
		a_PtcEmtMchGun = NULL;
		aii = 0;
		for (aii = 0; aii < m_ParticleList.size(); aii++) {
			if (m_ParticleList[aii]->m_EmtActive == false) {
				a_PtcEmtMchGun = m_ParticleList[aii];
				break;
			}
		}

		if (a_PtcEmtMchGun == NULL) {
			a_PtcEmtMchGun = new CPtcEmitter();
			m_ParticleList.push_back(a_PtcEmtMchGun);
		}

		if (a_PtcEmtMchGun != NULL) {
			a_PtcEmtMchGun->m_PtcImage = CParticle_Mgr::m_Ptc_MGun_Img;		// �ӽŰ� ��ƼŬ �̹��� ����
			a_PtcEmtMchGun->m_MslExp_Img = NULL;
			a_PtcEmtMchGun->SpawnPosition = a_StartV;
			a_PtcEmtMchGun->StartSize = Vector2D(10.0f, 10.0f);
			a_PtcEmtMchGun->EndSize = Vector2D(RandInt(7, 15), RandInt(2, 3));
			a_PtcEmtMchGun->LifeTime = RandInt(5.0f, 9.0f) * 100.0f;
			a_PtcEmtMchGun->Velocity = Vector2D::GetNormalFromRotation(a_RocketRot + RandInt(-150, 150) / 10.0f) * RandInt(50.0f, 500.0f);		// ������ ���ͷ� ȯ��
			// a_PtcEmtMchGun->ParticleColor = Direct2DHelper::RGBA(1.0f, 0.0f, 0.0f);
			a_PtcEmtMchGun->m_DirRot = a_PtcEmtMchGun->Velocity.GetAngle();		// <--- �ӵ� �������� ȸ�� �ϵ���...
			a_PtcEmtMchGun->StartAlpha = 1.0f;
			a_PtcEmtMchGun->EndAlpha = 1.0f;
			a_PtcEmtMchGun->CreatedTime = a_CurTime;		// timeGetTime() / 1000.0f;
			a_PtcEmtMchGun->m_EmtActive = true;
		}
		//------ Find ParticleEmitter
	}	// for (int i = 0; i < 10; i++)
}

void CParticle_Mgr::Explosion_Die(Vector2D a_StartV, DWORD a_CurTime)
{
	static CPtcEmitter* a_PtcEmtMchGun = NULL;
	static int aii = 0;
	a_PtcEmtMchGun = NULL;
	aii = 0;
	for (int i = 0; i < 40; i++) {
		//------ Find ParticleEmitter
		a_PtcEmtMchGun = NULL;
		aii = 0;
		for (aii = 0; aii < m_ParticleList.size(); aii++) {
			if (m_ParticleList[aii]->m_EmtActive == false) {
				a_PtcEmtMchGun = m_ParticleList[aii];
				break;
			}
		}

		if (a_PtcEmtMchGun == NULL) {
			a_PtcEmtMchGun = new CPtcEmitter();
			m_ParticleList.push_back(a_PtcEmtMchGun);
		}

		if (a_PtcEmtMchGun != NULL) {
			a_PtcEmtMchGun->m_PtcImage = CParticle_Mgr::m_Ptc_MGun_Img;		// �ӽŰ� ��ƼŬ �̹��� ����
			a_PtcEmtMchGun->m_MslExp_Img = NULL;
			a_PtcEmtMchGun->SpawnPosition = a_StartV;
			a_PtcEmtMchGun->StartSize = Vector2D(10.0f, 10.0f);
			a_PtcEmtMchGun->EndSize = Vector2D(RandInt(20, 50), RandInt(2, 5));
			a_PtcEmtMchGun->LifeTime = RandInt(5.0f, 9.0f) * 100.0f;
			a_PtcEmtMchGun->Velocity = Vector2D::GetNormalFromRotation(i * 9) * RandInt(50.0f, 300.0f);
			// a_PtcEmtMchGun->ParticleColor = Direct2DHelper::RGBA(1.0f, 0.0f, 0.0f);
			a_PtcEmtMchGun->m_DirRot = a_PtcEmtMchGun->Velocity.GetAngle();		// <--- �ӵ� �������� ȸ�� �ϵ���...
			a_PtcEmtMchGun->StartAlpha = 1.0f;
			a_PtcEmtMchGun->EndAlpha = 1.0f;
			a_PtcEmtMchGun->CreatedTime = a_CurTime;		// timeGetTime() / 1000.0f;
			a_PtcEmtMchGun->m_EmtActive = true;
		}
		//------ Find ParticleEmitter
	}
}

// �̻��Ͽ��� �ֱ������� ȣ���Ѵ�.
void CParticle_Mgr::GenSmokeParticle(Vector2D a_StartV, DWORD a_CurTime, Vector2D a_DirVec)
{
	static CPtcEmitter* a_PtcEmtMchGun = NULL;
	static int aii = 0;
	a_PtcEmtMchGun = NULL;
	aii = 0;
	for (aii = 0; aii < g_Particle_Mgr.m_ParticleList.size(); aii++) {
		if (g_Particle_Mgr.m_ParticleList[aii]->m_EmtActive == false) {
			a_PtcEmtMchGun = g_Particle_Mgr.m_ParticleList[aii];
			break;
		}
	}

	if (a_PtcEmtMchGun == NULL) {
		a_PtcEmtMchGun = new CPtcEmitter();
		g_Particle_Mgr.m_ParticleList.push_back(a_PtcEmtMchGun);
	}

	if (a_PtcEmtMchGun != NULL) {
		a_PtcEmtMchGun->m_PtcImage = CParticle_Mgr::m_Smoke_Img;		// �ӽŰ� ��ƼŬ �̹��� ����
		a_PtcEmtMchGun->m_MslExp_Img = NULL;
		a_PtcEmtMchGun->SpawnPosition = a_StartV;
		a_PtcEmtMchGun->StartSize = Vector2D(10.0f, 1.0f);
		a_PtcEmtMchGun->EndSize = Vector2D(RandInt(30, 50), RandInt(30, 50));
		a_PtcEmtMchGun->LifeTime = 1.0f * 1000.0f;
		/*
		if (0.0f <= a_DirVec.x) {
			a_PtcEmtMchGun->Velocity = Vector2D(-RandInt(30, 60), RandInt(-20, 30));
		}
		else {
			a_PtcEmtMchGun->Velocity = Vector2D(RandInt(30, 60), RandInt(-20, 30));
		}
		*/
		a_PtcEmtMchGun->Velocity = Vector2D(-RandInt(30, 60), RandInt(-20, 30));
		a_PtcEmtMchGun->StartAlpha = 1.0f;
		a_PtcEmtMchGun->EndAlpha = 0.0f;
		a_PtcEmtMchGun->CreatedTime = a_CurTime;		// timeGetTime() / 1000.0f;
		a_PtcEmtMchGun->m_EmtActive = true;
	}
}

CParticle_Mgr g_Particle_Mgr;