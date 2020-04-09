#include "framework.h"
#include "CMonster.h"
#include "CHero.h"

CMonster::CMonster()
{
	m_isActive = false;
	m_SpawnIdx = -1;		// ���� �� -1�� �ʱ�ȭ

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
	m_ScreenHalf.x = a_Center.x;		// MainDC�� ���� ���� �ݻ�����
	m_ScreenHalf.y = a_Center.y;

	m_CamPos.x = a_CamPos.x;
	m_CamPos.y = a_CamPos.y;

	Monster_AI(a_DeltaTime, a_Hero);

	static Vector2D a_Sc_HLen;
	static Vector2D a_Obj_HLen;

	a_Sc_HLen.x = m_ScreenHalf.x;		// +40;
	a_Sc_HLen.y = m_ScreenHalf.y;		// +40;

	if (m_SocketImg != NULL) {
		a_Obj_HLen.x = m_SocketImg->GetSize().width / 2.0f;		// ���� �� ������
		a_Obj_HLen.y = m_SocketImg->GetSize().height / 2.0f;		// ���� �� ������
	}
	else {
		a_Obj_HLen.x = 50.0f;
		a_Obj_HLen.y = 50.0f;
	}

	m_InScRect = CheckAABB(a_CamPos, a_Sc_HLen, m_CurPos, a_Obj_HLen);
}

void CMonster::Render_Unit(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush)
{
	if (m_SocketImg == NULL) {
		return;
	}

	m_RenderPos.x = m_CurPos.x - (int)m_CamPos.x;		// ���� ����� �� �� �ε巴�� �����δ�.
	m_RenderPos.y = m_CurPos.y - (int)m_CamPos.y;

	static VecINT2D img_size;
	static VecINT2D img_Half;
	img_size.x = m_SocketImg->GetSize().width;
	img_size.y = m_SocketImg->GetSize().height;
	img_Half.x = img_size.x / 2.0f;
	img_Half.y = img_size.y / 2.0f;

	static float a_iXX = 0;		// ������ �ݿø� ȯ��(�׸���� ���� ��ǥ�̱� ������...)
	static float a_iYY = 0;
	a_iXX = m_RenderPos.x;
	a_iYY = m_RenderPos.y;

	//------ ĳ���� Render
	a_pd2dRTarget->DrawBitmap(m_SocketImg, D2D1::RectF(a_iXX - img_Half.x, a_iYY - img_Half.y, a_iXX + img_Half.x, a_iYY + img_Half.y));
	//------ ĳ���� Render
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

	m_BasePos = m_CurPos;
	m_bMvPtOnOff = false;		// Patrol Move
	m_WaitTime = ((rand() % 251) + 50) / 100.0f;
}

void CMonster::Monster_AI(float a_DeltaTime, CHero& a_Hero)
{
	static Vector2D a_CalcVLen = { 0.0f,0.0f };
	static float a_CalcDist = 0.0f;
	static float m_TraceDist = 200.0f;		// �����Ÿ�
	static float m_NowStep = 0.0f;
	static float m_MoveVelocity = 50.0f;
	static Vector2D a_CalcEndVec = { 0.0f, 0.0f };
	static Vector2D m_DirMvVec = { 0.0f, 0.0f };		// Patrol�� �������ߵ� ���� ����
	static int a_AngleRan;
	static int a_LengthRan;
	static Vector2D a_DirPtVec = { 0.0f, 0.0f };
	static float a_CalcAngle = 0.0f;
	static Vector2D a_Vect;
	static Vector2D a_MoveNextStep = { 0.0f, 0.0f };
	static Vector2D m_MoveDir = { 0.0f, 0.0f };		// ��� ���� ����

	if (0.0f < m_AttackTick) {
		m_AttackTick = m_AttackTick - a_DeltaTime;
	}

	if (m_AIState == MAI_Patrol) {
		// ��Ʈ�� ���¶�� �ϴ��� 200 �ȼ� �������� ���� �����ϸ� �����ϰڴٴ� �ڵ�
		a_CalcVLen = a_Hero.m_CurPos - m_CurPos;
		a_CalcDist = a_CalcVLen.Magnitude();
		if (a_CalcDist < m_TraceDist) {		// �����Ÿ�
			m_AIState = MAI_NormalTrace;		// �Ϲ� �������� ���ư��� ���ݹ����ȿ� �ֱ� ������, �ٷ� ������ ���̴�.
			m_AggroTarget = &a_Hero;

			return;
		}
		// ��Ʈ�� ���¶�� �ϴ��� 200 �ȼ� �������� ���� �����ϸ� �����ϰڴٴ� �ڵ�

		if (m_bMvPtOnOff == true) {
			m_NowStep = a_DeltaTime * m_MoveVelocity;		// �̹��� �Ѱ��� ����(����)

			a_CalcEndVec = m_PatrolTarget - m_CurPos;
			m_DirMvVec = a_CalcEndVec;
			m_DirMvVec.Normalize();		// ���ͳ��� �浹�Ǵ��� Ÿ������ �̵��ϱ� ���� ó��
			// ���ͳ��� �浹�Ǽ� ��ǥ���� �������� ���ϴ� ������ �ذ��ϱ� ���ؼ��� ��ǥ������ �����ϴ� �ð��� ���� �� ���� �ð���ŭ�� �̵���Ű�� �������� �����Ѵ�.

			m_AddTimeCount = m_AddTimeCount + a_DeltaTime;
			if (m_MoveDurTime <= m_AddTimeCount) {		// ��ǥ���� ������ ������ �����Ѵ�.
				m_bMvPtOnOff = false;
			}
			else {
				m_CurPos = m_CurPos + (m_DirMvVec * m_NowStep);
			}
		}
		else {
			m_WaitTime = m_WaitTime - a_DeltaTime;
			if (0.0f < m_WaitTime) {
				// ������ �ִϸ��̼����� �ٲ��ִ� �κ�
			}
			else {
				m_WaitTime = 0.0f;
				a_AngleRan = (rand() % 271) + 30;
				a_LengthRan = (rand() % 70) + 30;		// a_LengthRan = Random.Range(30, 99);

				a_DirPtVec = m_CurPos - m_BasePos;

				// ���͸� ȸ�� ������... a_AngleRan
				if (a_DirPtVec.Magnitude() < 1.0f) {
					a_CalcAngle = 0.0f;
				}
				else {
					a_CalcAngle = a_DirPtVec.GetAngle();
				}

				a_CalcAngle = a_CalcAngle + (float)a_AngleRan;
				a_Vect = Vector2D::GetNormalFromRotation(a_CalcAngle);		// ������ ���ͷ� ȯ��

				m_PatrolTarget = m_BasePos + (a_Vect * (float)a_LengthRan);

				m_DirMvVec = m_PatrolTarget - m_CurPos;
				m_MoveDurTime = m_DirMvVec.Magnitude() / m_MoveVelocity;		// �����ϴµ� �ɸ��� �ð�
				m_AddTimeCount = 0.0;
				m_DirMvVec.Normalize();

				m_WaitTime = ((rand() % 301) + 20) / 100.0f;		// Random.Range(0.2f, 3.0f);
				m_bMvPtOnOff = true;
			}	// else
		}	// else
	}	// if (m_AIState == MonAIState.MAI_Patrol)

	if (m_AIState == MAI_AggroTrace) {
		if (m_AggroTarget != NULL) {
			a_CalcVLen = m_AggroTarget->m_CurPos - m_CurPos;

			a_CalcDist = a_CalcVLen.Magnitude();
			m_MoveDir = a_CalcVLen;
			m_MoveDir.Normalize();

			if (a_CalcDist <= (a_Hero.m_HalfColl + m_HalfColl + 10.0f)) {		// ���ݰŸ�
				m_AIState = MAI_NormalTrace;
			}

			if ((a_Hero.m_HalfColl + m_HalfColl) < a_CalcDist) {		// ���ݰŸ�
				m_NowStep = m_MoveVelocity * 3.5f * a_DeltaTime;		// �Ѱ��� ũ��
				a_MoveNextStep = m_MoveDir * m_NowStep;		// �Ѱ��� ����

				m_CurPos = m_CurPos + a_MoveNextStep;
			}
		}
		else {
			m_AIState = MAI_Patrol;
			m_bMvPtOnOff = false;
		}
	}	// if (m_AIState == MAI_AggroTrace)

	if (m_AIState == MAI_NormalTrace) {
		if (m_AggroTarget != NULL) {
			a_CalcVLen = m_AggroTarget->m_CurPos - m_CurPos;
			a_CalcDist = a_CalcVLen.Magnitude();

			if (a_CalcDist < m_TraceDist) {		// �����Ÿ�
				m_MoveDir = a_CalcVLen;
				m_MoveDir.Normalize();

				if (a_CalcDist < (a_Hero.m_HalfColl + m_HalfColl + 100.0f)) {		// ���ݰŸ�
					if (m_AttackTick <= 0.0f) {		// ���� ���� �ȿ� ������ ��� ����
						// ���Ͱ� ���ΰ� ����
						// a_Hero.TackDamage(10);

						m_AttackTick = 1.0f;
					}
				}
				if ((a_Hero.m_HalfColl + m_HalfColl) < a_CalcDist) {		// ���ݰŸ�
					m_NowStep = m_MoveVelocity * 3.0f * a_DeltaTime;		// �Ѱ��� ũ��
					a_MoveNextStep = m_MoveDir * m_NowStep;		// �Ѱ��� ����
					m_CurPos = m_CurPos + a_MoveNextStep;
				}
			}	// if (a_CalcDist < m_TraceDist)	// �����Ÿ�
			else {
				m_AIState = MAI_Patrol;
				m_bMvPtOnOff = false;
			}
		}	// if (m_AggroTarget != NULL)
		else {
			m_AIState = MAI_Patrol;
			m_bMvPtOnOff = false;
		}
	}	// if (m_AIState == MAI_NormalTrace)
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
