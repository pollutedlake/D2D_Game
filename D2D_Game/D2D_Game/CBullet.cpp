#include "framework.h"
#include "CBullet.h"
#include "GlobalValue.h"

CBullet::CBullet()
{
	m_BLActive = false;

	m_Duration = 4.0f;		// �Ѿ��� ���� ���� �ð�
	m_DirRot = 0.0f;		// �Ѿ��� ���ư��� ���Ⱒ��
	m_MoveSpeed = 1700.0f;		// 1700�ȼ� / 1��
	m_Radius = 25.0f;		// �Ѿ� �浹 �ݰ�
}		

CBullet::~CBullet()
{
}

bool CBullet::BulletUpdate(float a_DwdDeltaTime, DWORD a_DwdLastTime, float a_WpDeltaTime, void(*GenSmokePtc)(Vector2D a_StartV, DWORD a_CurTime, Vector2D a_DirVec))
{
	m_Duration = m_Duration - a_DwdDeltaTime;
	if (m_Duration < 0.0f) {
		m_BLActive = false;
		return false;		// ���� �ð� �ڿ� ������ �ش�.
	}

	//--- ����� ����
	if (WeaponSlot == EWeaponSlots::ROCKET_LAUNCHER) {
		m_SmokeTime = m_SmokeTime - a_WpDeltaTime;
		if (m_SmokeTime <= 0.0f) {
			// �ֱ������� ����ũ ����
			if (GenSmokePtc != NULL) {
				GenSmokePtc(m_CurPos, a_DwdLastTime, m_DirVec);
			}
			m_SmokeTime = 0.01;		// �� �ֱ��
		}
	}	// if (WeaponSlot == EWeaponSlots::ROCKET_LAUNCHER)
	//--- ����� ����

	m_CurPos = m_CurPos + (m_DirVec * (a_DwdDeltaTime * m_MoveSpeed));		// �Ѿ� �̵�ó��
	m_RenderPos = m_RenderPos + (m_DirVec * (a_WpDeltaTime * m_MoveSpeed));		// �Ѿ� �̵�ó��

	return true;
}

void CBullet::BulletRender(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush, Vector2D a_CamPos)
{
	static float a_iXX = 0;		// ������ �ݿø� ȯ��(�׸���� ���� ��ǥ�̱� ������...)
	static float a_iYY = 0;
	a_iXX = (float)m_RenderPos.x - (int)a_CamPos.x;
	a_iYY = (float)m_RenderPos.y - (int)a_CamPos.y;
	// AdjustRenderPosForShake(a_iXX);
	// AdjustRenderPosForShake(a_iYY);

	int x = a_iXX - m_TexImgWidth * 0.5f;
	int y = a_iYY - m_TexImgHeight * 0.5f;
	float Rotation = m_DirVec.GetAngle();
	float Alpha = 1.0f;	

	D2D1_POINT_2F center_pos = { a_iXX, a_iYY };
	a_pd2dRTarget->SetTransform(D2D1::Matrix3x2F::Rotation(Rotation, center_pos));

	//------ �̹��� ������
	if (WeaponType == EWeaponSlots::PISTOL) {
		a_pBrush->SetColor(D2D1::ColorF(0x00ffff));
		a_pd2dRTarget->FillRectangle(D2D1::RectF(x, y, x + m_TexImgWidth, y + m_TexImgHeight), a_pBrush);
		a_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	}
	else {
		a_pd2dRTarget->DrawBitmap(m_BulletImg, D2D1::RectF(x, y, x + m_TexImgWidth, y + m_TexImgHeight), Alpha, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
	}
	//------ �̹��� ������

	a_pd2dRTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}
