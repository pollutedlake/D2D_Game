#include "framework.h"
#include "CHero.h"
#include "GlobalValue.h"
#include "CAnimData.h"

CHero::CHero()
{
	m_CurHP = 150;
	m_MaxHP = 150;

	m_MoveSpeed = 200.0;

	//------ 애니메이션 관련 변수 초기화
	m_RefAniData = NULL;

	m_CurAniState = AS_None;
	m_NowImgCount = 0;
	m_CurAniInx = 0;		// 진행 Ani Index
	m_AniTickCount = 0;		// 다음 프레임까지 시간 Add
	m_EachAniDelay = 0.2f;		// 프레임 간격 시간
	//------ 애니메이션 관련 변수 초기화
}

CHero::~CHero()
{
}

void CHero::Init_Unit(ID2D1Bitmap* (*D2DLoadBmp)(LPCWSTR FName))
{
	if (D2DLoadBmp != NULL) {
		m_SocketImg = D2DLoadBmp(_T(".\\RscTrunk\\Nanami\\front_Idle\\n001.png"));
	}
}

void CHero::Update_Unit(double a_DeltaTime)
{
	//------ 마우스 피킹이 있는 경우 처리
	if (m_bMoveOnOff == true) {
		static double a_CalcStep;
		static Vector2D a_CalcEndVec;

		// 가야할 방향에 시간 값을 곱해서 조금씩 이동하게 한다.
		a_CalcStep = a_DeltaTime * m_MoveSpeed;		// 이번에 한걸음 길이 (보폭)

		a_CalcEndVec = m_TargetPos - m_CurPos;
		if (a_CalcEndVec.Magnitude() <= a_CalcStep) {		// 목표점까지의 거리보다 보폭이 크거나 같으면 도착으로 본다.
			m_bMoveOnOff = false;
			m_CurPos = m_TargetPos;		// <--- 이게 주인공의 좌표
		}
		else {
			m_DirVec = a_CalcEndVec;
			m_DirVec.Normalize();
			m_CurPos = m_CurPos + (m_DirVec * a_CalcStep);
		}
	}
	//------ 마우스 피킹이 있는 경우 처리

	//------ 캐릭터 키보드 이동처리
	static Vector2D a_KDirVec;
	a_KDirVec.x = 0.0f;
	a_KDirVec.y = 0.0f;

	if ((GetAsyncKeyState(VK_LEFT) & 0x8000) || (GetAsyncKeyState('A') & 0x8000)) {
		a_KDirVec.x -= 1.0f;
	}
	if ((GetAsyncKeyState(VK_RIGHT) & 0x8000) || (GetAsyncKeyState('D') & 0x8000)) {
		a_KDirVec.x += 1.0f;
	}
	if ((GetAsyncKeyState(VK_UP) & 0x8000) || (GetAsyncKeyState('W') & 0x8000)) {
		a_KDirVec.y -= 1.0f;
	}
	if ((GetAsyncKeyState(VK_DOWN) & 0x8000) || (GetAsyncKeyState('S') & 0x8000)) {
		a_KDirVec.y += 1.0f;
	}

	if (a_KDirVec.x != 0.0f || a_KDirVec.y != 0.0f) {
		m_bMoveOnOff = false;

		a_KDirVec.Normalize();		// 단위벡터
		m_CurPos = m_CurPos + (a_KDirVec * (a_DeltaTime * m_MoveSpeed));
	}
	//------ 캐릭터 키보드 이동처리

	//------ 캐릭터 애니메이션 상태 변경 처리 부분
	if (m_bMoveOnOff == true) {
		a_KDirVec.x = m_DirVec.x;
		a_KDirVec.y = m_DirVec.y;
	}

	if (a_KDirVec.x != 0.0f || a_KDirVec.y != 0.0f) {
		static Vector2D m_ZeroVec;
		m_ZeroVec.x = 0.0f;
		m_ZeroVec.y = 0.0f;
		float a_Angle = GetAngle(m_ZeroVec, a_KDirVec);
		if (50.0f < a_Angle && a_Angle < 130.0f) {
			ChangeState(Left_Walk);
		}
		else if (130.0f <= a_Angle && a_Angle <= 230.0f) {
			ChangeState(Back_Walk);
		}
		else if (230.0f < a_Angle && a_Angle < 310.0f) {
			ChangeState(Right_Walk);
		}
		else {
			ChangeState(Front_Walk);
		}
	}
	else {
		ChangeState(Idle);
	}
	//------ 캐릭터 애니메이션 상태 변경 처리 부분

	//------ 애니메이션 프레임 계산 부분
	AniFrameUpdate(a_DeltaTime);
	//------ 애니메이션 프레임 계산 부분
}

void CHero::Render_Unit(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush)
{
	// 절대좌표를 스크린 좌표로 변환해서 그려야 한다.
	// m_RenderPos.x = m_CurPos.x - (int)m_CamPos.x + m_ScreenHalf.x;		// 이론적
	// m_RenderPos.y = m_CurPos.y - (int)m_CamPos.y + m_ScreenHalf.y;
	m_RenderPos.x = m_CurPos.x - m_CamPos.x;		// 이쪽 계산이 좀 더 부드럽게 움직인다.
	m_RenderPos.y = m_CurPos.y - m_CamPos.y;

	if (m_SocketImg == NULL) {
		return;
	}

	static D2D1_SIZE_F img_size;
	static Vector2D img_Half;
	static Vector2D Calc_Pos;
	img_size = m_SocketImg->GetSize();
	img_Half.x = img_size.width / 2.0f;
	img_Half.y = img_size.height / 2.0f;
	Calc_Pos.x = m_CurPos.x - img_Half.x;
	Calc_Pos.y = m_CurPos.y - img_Half.y;

	//---- HP Bar Render
	static float FigureAlpha = 1.0f;  //도형 투명도
	static float a_CalcMXX = 0.0f;
	static float a_CalcMYY = (int)(img_Half.x * 0.8f);
	static float a_HpBSize = 50;
	static float a_CurHpSize = 0;
	a_CalcMXX = a_HpBSize / 2.0f;
	a_CurHpSize = a_HpBSize * ((float)m_CurHP / (float)m_MaxHP);

	a_CalcMYY = (int)(img_Half.y * 0.83f);

	static float a_iXX = 0; //정수로 반올림 환산(그리기는 정수 좌표이기 때문에...)
	static float a_iYY = 0;
	a_iXX = m_RenderPos.x;
	a_iYY = m_RenderPos.y;
	AdjustRenderPosForShake(a_iXX);
	AdjustRenderPosForShake(a_iYY);

	a_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Red, FigureAlpha));
	a_pd2dRTarget->FillRectangle(D2D1::RectF(a_iXX - a_CalcMXX, a_iYY - a_CalcMYY, a_iXX - a_CalcMXX + a_CurHpSize, a_iYY - (a_CalcMYY + 9.0f)), a_pBrush);

	a_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White, FigureAlpha));
	a_pd2dRTarget->DrawRectangle(D2D1::RectF(a_iXX - a_CalcMXX, a_iYY - a_CalcMYY, a_iXX - a_CalcMXX + a_HpBSize, a_iYY - (a_CalcMYY + 9.0f)), a_pBrush);

	a_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	////---- HP Bar Render

	//------ 캐릭터 Render
	a_pd2dRTarget->DrawBitmap(m_SocketImg, D2D1::RectF(a_iXX - img_Half.x, a_iYY - img_Half.y, a_iXX + img_Half.x, a_iYY + img_Half.y));
	//------ 캐릭터 Render
}

void CHero::Destroy_Unit()
{
}

void CHero::Abs_Update(VecINT2D& a_ScHalf, Vector2D& a_CamPos)
{
	m_ScreenHalf.x = a_ScHalf.x;		// MainDc의 가로 세로 반사이즈
	m_ScreenHalf.y = a_ScHalf.y;

	m_CamPos.x = a_CamPos.x;
	m_CamPos.y = a_CamPos.y;
}

void CHero::MsPicking(Vector2D a_TgPos)
{
	Vector2D a_CalcVec = a_TgPos - m_CurPos;
	if (a_CalcVec.Magnitude() < 4.0f) {
		return;
	}

	m_bMoveOnOff = true;

	m_TargetPos = a_TgPos;		// 목표점
}

void CHero::TakeDamage(float a_Damage)
{
	// 몬스터가 주인공 공격
	m_CurHP = m_CurHP - (int)a_Damage;

	if (m_CurHP <= 0) {		// 사망처리
		// 남은 몬스터와 총알 제거
		// g_GameState = GAME_OVER;

		m_CurHP = 0;
	}
}

void CHero::SetAni_Rsc(CT_Type a_CharType, vector<class CAnimData*>* refAnimList)
{
	if (refAnimList == nullptr) {
		return;
	}

	if (a_CharType <= CT_None || CTT_Length <= a_CharType) {
		return;
	}

	m_CharicType = a_CharType;

	if (a_CharType < refAnimList->size()) {
		m_RefAniData = (*refAnimList)[(int)a_CharType];
	}

	if (m_RefAniData != NULL) {
		if (m_SocketImg != NULL) {
			m_SocketImg->Release();
			m_SocketImg = NULL;
		}
	}

	ChangeState(Idle);
}

bool CHero::ChangeState(AniState newState)
{
	if (m_CurAniState == newState) {
		return false;
	}

	if (m_RefAniData == NULL) {
		return false;
	}

	if (newState <= AS_None || AniLength <= newState) {
		return false;
	}

	static int a_NowImgCount = 0;
	a_NowImgCount = m_RefAniData->GetFrameCount(newState);
	if (a_NowImgCount <= 0) {
		return false;
	}

	m_NowImgCount = a_NowImgCount;

	m_CurAniInx = 0;
	m_AniTickCount = 0;

	if (newState == Idle) {
		m_EachAniDelay = 0.4f;
	}
	else {
		m_EachAniDelay = 0.12f;
	}

	m_SocketImg = m_RefAniData->GetFrameImg(newState, 0);		// 첫동작 대입

	m_CurAniState = newState;

	return true;
}

void CHero::AniFrameUpdate(double a_DeltaTime)
{
	//------ 애니메이션 프레임 계산 부분
	if (m_RefAniData == NULL) {
		return;
	}

	if (m_CurAniState == AS_None) {
		return;
	}

	if (m_NowImgCount <= 0) {		// 애니 소켓에 뭔가 꼽혀 있는지 확인해 보는 안전장치
		return;
	}

	m_AniTickCount = m_AniTickCount + a_DeltaTime;
	if (m_EachAniDelay < m_AniTickCount) {		// 다음 프레임
		m_CurAniInx++;
		if (m_NowImgCount <= m_CurAniInx) {
			m_CurAniInx = 0;
		}

		m_SocketImg = m_RefAniData->GetFrameImg(m_CurAniState, m_CurAniInx);

		m_AniTickCount = 0;
	}
	//------ 애니메이션 프레임 계산 부분
}

CHero g_Hero;