#pragma once
#include "CPtcEmitter.h"

class CParticle_Mgr
{
public:
	vector<CPtcEmitter*> m_ParticleList;		// <--- ��ƼŬ ������ Ǯ ���
	vector<CPtcEmitter*> m_MissileExpList;		// <--- �̻��� ���� �޸� Ǯ�� ���� �����ϴ� �͵� ����Ѵ�.

public:
	static ID2D1Bitmap* m_Ptc_MGun_Img;
	static ID2D1Bitmap* m_Smoke_Img;
	static ID2D1Bitmap* m_MissileExp_Img;

public:
	CParticle_Mgr();
	~CParticle_Mgr();

public:
	void PtcMgrInit(ID2D1Bitmap* (D2DLoadBmp)(LPCWSTR FName) = NULL);
	void PtcMgrUpdate(DWORD a_CurTime, VecINT2D& a_Center, Vector2D& a_CamPos);
	void PtcMgrRender(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush);
	void PtcMgrDestroy();
	//--- Missile ���� ���� ������
	void MslExpMgrRender(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush);
	//--- Missile ���� ���� ������

	void Explosion_MGun(Vector2D a_StartV, DWORD a_CurTime);
	void Explosion_Missile(Vector2D a_StartV, DWORD a_CurTime, float a_RocketRot = 0.0f);
	void Explosion_Die(Vector2D a_StartV, DWORD a_CurTime);
	void static GenSmokeParticle(Vector2D a_StartV, DWORD a_CurTime, Vector2D a_DirVec);		// ���� ��ƼŬ �����...
};

extern CParticle_Mgr g_Particle_Mgr;