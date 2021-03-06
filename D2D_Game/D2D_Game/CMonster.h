#pragma once

#include "Vector2D.h"
#include <d2d1.h>
#include "GlobalValue.h"

enum MonAIState {
	MAI_Idle,		// 숨쉬기 상태
	MAI_Patrol,		// 패트롤 상태
	MAI_AggroTrace,		// 적으로부터 공격을 당했을 때 추적 상태
	MAI_NormalTrace,		// 일반 추적 상태
	MAI_ReturnPos,		// 추적 놓쳤을 때 제자리로 돌아오는 상태
	MAI_Attack,		// 공격 상태
};

class CHero;

class CMonster
{
public:
	CT_Type m_CharicType;		// 캐릭터 종류가 "몬스터"인지? "주인공"인지?
	int m_SpawnIdx;		// vector<SpawnPos> m_SpawnPos;의 인덱스

	ID2D1Bitmap* m_SocketImg;		// 애니 소켓처럼 사용할 포인터
	bool m_isActive;
	float m_MaxHP = 100.0f;
	float m_CurHP = 100.0f;

	//--- 몬스터 좌표 계산용 변수들...
	Vector2D m_CurPos;
	float m_Speed = 150.0f;
	Vector2D m_DirVec;
	float m_HalfColl = 17;		// 몬스터의 충돌 반경
	//--- 몬스터 좌표 계산용 변수들...

	//------ 절대좌표 이동 계산용 변수
	VecINT2D m_ScreenHalf;		// 화면 반 사이즈 저장용 변수
	Vector2D m_CamPos = { 0.0f, 0.0f };		// 현재 위치
	Vector2D m_RenderPos;		// 렌더링 위치
	//------ 절대좌표 이동 계산용 변수

	bool m_InScRect = false;

	//------ 몬스터 AI 변수들...
	CHero* m_AggroTarget = NULL;
	MonAIState m_AIState = MAI_Patrol;

	bool m_bMvPtOnOff = false;		// Patrol Move
	float m_WaitTime = 0.0f;		// Patrol시에 목표점에 도착하면 잠시 대기시키기 위한 랜덤 시간변수
	Vector2D m_BasePos = { 0.0f,0.0f };		// 몬스터의 초기 스폰 위치(기준점이 된다.)

	Vector2D m_PatrolTarget = { 0.0f,0.0f };		// Patrol시 움직여야될 다음 목표 좌표
	double m_AddTimeCount = 0.0;		// 누적시간 카운트
	double m_MoveDurTime = 0.0;		// 목표점까지 도착하는데 걸리는 시간

	float m_AttackTick = 0.0f;
	//------ 몬스터 AI 변수들...

public:
	CMonster();
	~CMonster();

public:
	void Init_Unit();
	void Update_Unit(double a_DeltaTime, VecINT2D& a_Center, Vector2D& a_CamPos, class CHero& a_Hero);
	void Render_Unit(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush);
	void Destroy_Unit();

	void Spawn(float a_XX, float a_YY, ID2D1Bitmap* a_RefImg = NULL);
	void Monster_AI(float a_DeltaTime, CHero& a_Hero);
	bool CheckAABB(Vector2D& AACen, Vector2D& AAHLen, Vector2D& BBCen, Vector2D& BBHLen);
};

