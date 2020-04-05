#include "framework.h"
#include "CHero.h"

CHero::CHero()
{
	m_CurHP = 150;
	m_MaxHP = 150;

	m_MoveSpeed = 200.0;
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
	//------ ���콺 ��ŷ�� �ִ� ��� ó��
	if (m_bMoveOnOff == true) {
		static double a_CalcStep;
		static Vector2D a_CalcEndVec;

		// ������ ���⿡ �ð� ���� ���ؼ� ���ݾ� �̵��ϰ� �Ѵ�.
		a_CalcStep = a_DeltaTime * m_MoveSpeed;		// �̹��� �Ѱ��� ���� (����)

		a_CalcEndVec = m_TargetPos - m_CurPos;
		if (a_CalcEndVec.Magnitude() <= a_CalcStep) {		// ��ǥ�������� �Ÿ����� ������ ũ�ų� ������ �������� ����.
			m_bMoveOnOff = false;
			m_CurPos = m_TargetPos;		// <--- �̰� ���ΰ��� ��ǥ
		}
		else {
			m_DirVec = a_CalcEndVec;
			m_DirVec.Normalize();
			m_CurPos = m_CurPos + (m_DirVec * a_CalcStep);
		}
	}
	//------ ���콺 ��ŷ�� �ִ� ��� ó��

	//------ ĳ���� Ű���� �̵�ó��
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

		a_KDirVec.Normalize();		// ��������
		m_CurPos = m_CurPos + (a_KDirVec * (a_DeltaTime * m_MoveSpeed));
	}
	//------ ĳ���� Ű���� �̵�ó��
}

void CHero::Render_Unit(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush)
{
	// ������ǥ�� ��ũ�� ��ǥ�� ��ȯ�ؼ� �׷��� �Ѵ�.
	// m_RenderPos.x = m_CurPos.x - (int)m_CamPos.x + m_ScreenHalf.x;		// �̷���
	// m_RenderPos.y = m_CurPos.y - (int)m_CamPos.y + m_ScreenHalf.y;
	m_RenderPos.x = m_CurPos.x - m_CamPos.x;		// ���� ����� �� �� �ε巴�� �����δ�.
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

	//------ ĳ���� Render
	a_pd2dRTarget->DrawBitmap(m_SocketImg, D2D1::RectF(m_RenderPos.x - img_Half.x, m_RenderPos.y - img_Half.y, m_RenderPos.x + img_Half.x, m_RenderPos.y + img_Half.y));
	//------ ĳ���� Render
}

void CHero::Destroy_Unit()
{
}

void CHero::Abs_Update(VecINT2D& a_ScHalf, Vector2D& a_CamPos)
{
	m_ScreenHalf.x = a_ScHalf.x;		// MainDc�� ���� ���� �ݻ�����
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

	m_TargetPos = a_TgPos;		// ��ǥ��
}

CHero g_Hero;