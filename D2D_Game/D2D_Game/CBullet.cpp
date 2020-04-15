#include "framework.h"
#include "CBullet.h"
#include "GlobalValue.h"

CBullet::CBullet()
{
	m_BLActive = false;

	m_Duration = 4.0f;		// 총알의 생존 지속 시간
	m_DirRot = 0.0f;		// 총알의 날아가는 방향각도
	m_MoveSpeed = 1700.0f;		// 1700픽셀 / 1초
	m_Radius = 25.0f;		// 총알 충돌 반경
}		

CBullet::~CBullet()
{
}

bool CBullet::BulletUpdate(float a_DwdDeltaTime, DWORD a_DwdLastTime, float a_WpDeltaTime, void(*GenSmokePtc)(Vector2D a_StartV, DWORD a_CurTime, Vector2D a_DirVec))
{
	m_Duration = m_Duration - a_DwdDeltaTime;
	if (m_Duration < 0.0f) {
		m_BLActive = false;
		return false;		// 일정 시간 뒤에 제거해 준다.
	}

	//--- 스모그 스폰
	if (WeaponSlot == EWeaponSlots::ROCKET_LAUNCHER) {
		m_SmokeTime = m_SmokeTime - a_WpDeltaTime;
		if (m_SmokeTime <= 0.0f) {
			// 주기적으로 스모크 생성
			if (GenSmokePtc != NULL) {
				GenSmokePtc(m_CurPos, a_DwdLastTime, m_DirVec);
			}
			m_SmokeTime = 0.01;		// 이 주기로
		}
	}	// if (WeaponSlot == EWeaponSlots::ROCKET_LAUNCHER)
	//--- 스모그 스폰

	m_CurPos = m_CurPos + (m_DirVec * (a_DwdDeltaTime * m_MoveSpeed));		// 총알 이동처리
	m_RenderPos = m_RenderPos + (m_DirVec * (a_WpDeltaTime * m_MoveSpeed));		// 총알 이동처리

	return true;
}

void CBullet::BulletRender(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush, Vector2D a_CamPos)
{
	static float a_iXX = 0;		// 정수로 반올림 환산(그리기는 정수 좌표이기 때문에...)
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

	//------ 이미지 렌더링
	if (WeaponType == EWeaponSlots::PISTOL) {
		a_pBrush->SetColor(D2D1::ColorF(0x00ffff));
		a_pd2dRTarget->FillRectangle(D2D1::RectF(x, y, x + m_TexImgWidth, y + m_TexImgHeight), a_pBrush);
		a_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	}
	else {
		a_pd2dRTarget->DrawBitmap(m_BulletImg, D2D1::RectF(x, y, x + m_TexImgWidth, y + m_TexImgHeight), Alpha, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
	}
	//------ 이미지 렌더링

	a_pd2dRTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}
