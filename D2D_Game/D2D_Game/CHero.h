#pragma once

#include "Vector2D.h"
#pragma comment(lib, "d2d1.lib")		// <--- Direct2D 사용하기...

#include <d2d1.h>

#include<vector>
using namespace std;

class CHero
{
public:
	ID2D1Bitmap* m_SocketImg = NULL;		// 애니 소켓처럼 사용할 포인터

	int m_CurHP;
	int m_MaxHP;

	//------ 캐릭터 이동 계산용 변수
	Vector2D m_CurPos = { 0.0f, 0.0f };		// 화면 중점 위치
	double m_MoveSpeed;		// 이동 속도
	//------ 캐릭터 이동 계산용 변수

	//------ 절대좌표 이동 계산용 변수
	VecINT2D m_ScreenHalf;		// 화면 반 사이즈 저장용 변수
	Vector2D m_CamPos = { 0.0f, 0.0f };		// 현재 위치
	Vector2D m_RenderPos;		// 렌더링 위치
	//------ 절대좌표 이동 계산용 변수

	// 피킹 관련 변수들...
	Vector2D m_DirVec;
	bool m_bMoveOnOff = false;
	Vector2D m_TargetPos;
	// 피킹 관련 변수들...

	float m_HalfColl = 20.0f;		// 주인공의 충돌 반경

	//------ 애니메이션 관련 변수
public:
	enum CT_Type m_CharicType;		// 캐릭터 종류가 "몬스터"인지? "주인공"인지?
	class CAnimData* m_RefAniData;		// 캐릭터 타입에 따라 애니메이션 데이터를 바꿔 낄 것임

	enum AniState m_CurAniState;		// 현재 애니메이션 상태
	int m_NowImgCount;		// 애니 소켓처럼 사용할 이미지 카운트
	float m_EachAniDelay;		// 프레임 간격 시간
	float m_AniTickCount;		// 다음 프레임까지 시간 Add
	int m_CurAniInx;		// 진행 Ani Index
	//------ 애니메이션 관련 변수

public:
	CHero();
	~CHero();

public:
	void Init_Unit(ID2D1Bitmap* (*D2DLoadBmp)(LPCWSTR FName) = NULL);
	void Update_Unit(double a_DeltaTime);
	void Render_Unit(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush);
	void Destroy_Unit();

	void Abs_Update(VecINT2D& a_ScHalf, Vector2D& a_CamPos);
	void MsPicking(Vector2D a_TgPos);

	void TakeDamage(float a_Damage = 10.0f);

	void SetAni_Rsc(CT_Type a_CharType, vector<class CAnimData*>* refAnimList = nullptr);

	bool ChangeState(AniState newState);
	void AniFrameUpdate(double a_DeltaTime);
};

extern CHero g_Hero;

