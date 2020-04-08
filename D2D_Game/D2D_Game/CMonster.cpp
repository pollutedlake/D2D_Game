#include "framework.h"
#include "CMonster.h"

CMonster::CMonster()
{
	m_isActive = false;
	m_SpawnIdx = -1;		// 죽을 때 -1로 초기화

	m_MaxHP = 400.0f;
	m_CurHP = 400.0f;

	m_Speed = 150.0f;
	m_HalfColl = 20;
}

CMonster::~CMonster()
{
}

void CMonster::Init_Unit()
{
}

void CMonster::Update_Unit(double a_DeltaTime, VecINT2D& a_Center, Vector2D& a_CamPos, CHero& a_Hero)
{
	m_ScreenHalf.x = a_Center.x;		// MainDC의 가로 세로 반사이즈
	m_ScreenHalf.y = a_Center.y;

	m_CamPos.x = a_CamPos.x;
	m_CamPos.y = a_CamPos.y;

	Monster_AI(a_DeltaTime, a_Hero);

	static Vector2D a_Sc_HLen;
	static Vector2D a_Obj_HLen;

	a_Sc_HLen.x = m_ScreenHalf.x;		// +40;
	a_Sc_HLen.y = m_ScreenHalf.y;		// +40;

	if (m_SocketImg != NULL) {
		a_Obj_HLen.x = m_SocketImg->GetSize().width / 2.0f;		// 가로 반 사이즈
		a_Obj_HLen.y = m_SocketImg->GetSize().height / 2.0f;		// 세로 반 사이즈
	}

	m_InScRect = CheckAABB(a_CamPos, a_Sc_HLen, m_CurPos, a_Obj_HLen);
}

void CMonster::Render_Unit(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush)
{
	if (m_SocketImg == NULL) {
		return;
	}

	m_RenderPos.x = m_CurPos.x - (int)m_CamPos.x;		// 이쪽 계산이 좀 더 부드럽게 움직인다.
	m_RenderPos.y = m_CurPos.y - (int)m_CamPos.y;

	static VecINT2D img_size;
	static VecINT2D img_Half;
	img_size.x = m_SocketImg->GetSize().width;
	img_size.y = m_SocketImg->GetSize().height;
	img_Half.x = img_size.x / 2.0f;
	img_Half.y = img_size.y / 2.0f;

	static float a_iXX = 0;		// 정수로 반올림 환산(그리기는 정수 좌표이기 때문에...)
	static float a_iYY = 0;
	a_iXX = m_RenderPos.x;
	a_iYY = m_RenderPos.y;

	//------ 캐릭터 Render
	a_pd2dRTarget->DrawBitmap(m_SocketImg, D2D1::RectF(a_iXX - img_Half.x, a_iYY - img_Half.y, a_iXX + img_Half.x, a_iYY + img_Half.y));
	//------ 캐릭터 Render
}

void CMonster::Destroy_Unit()
{
}

void CMonster::Spawn(float a_XX, float a_YY, ID2D1Bitmap* a_RefImg)
{
	m_CurPos.x = a_XX;
	m_CurPos.y = a_YY;

	m_isActive = true;

	m_SocketImg = a_RefImg;
}

void CMonster::Monster_AI(float a_DeltaTime, CHero& a_Hero)
{
}

bool CMonster::CheckAABB(Vector2D& AACen, Vector2D& AAHLen, Vector2D& BBCen, Vector2D& BBHLen)
{
	static Vector2D a_AA_Min;
	static Vector2D a_AA_Max;
	static Vector2D a_BB_Min;
	static Vector2D a_BB_Max;

	a_AA_Min.x = AACen.x;
	a_AA_Min.y = AACen.y;
	a_AA_Max.x = AACen.x + (AAHLen.x * 2.0f);
	a_AA_Max.y = AACen.y + (AAHLen.y * 2.0f);

	a_BB_Min.x = BBCen.x - BBHLen.x;
	a_BB_Min.y = BBCen.y - BBHLen.y;
	a_BB_Max.x = BBCen.x + BBHLen.x;
	a_BB_Max.y = BBCen.y + BBHLen.y;

	if (a_AA_Min.x <= a_BB_Max.x && a_AA_Max.x >= a_BB_Min.x && a_AA_Min.y <= a_BB_Max.y && a_AA_Max.y >= a_BB_Min.y) {
		return true;
	}

	return false;
}
