#include "framework.h"
#include "CPtcEmitter.h"
#include "GlobalValue.h"

CPtcEmitter::CPtcEmitter()
{
	m_EmtActive = false;
}

CPtcEmitter::~CPtcEmitter()
{
}

void CPtcEmitter::Emt_Update(DWORD a_LastTime, VecINT2D& a_Center, Vector2D& a_CamPos)
{
	m_ScreenHalf.x = a_Center.x;		// MainDC�� ���� ���� �ݻ�����
	m_ScreenHalf.y = a_Center.y;

	m_CamPos.x = a_CamPos.x;
	m_CamPos.y = a_CamPos.y;

	m_LastTime = a_LastTime;
}

void CPtcEmitter::Emt_Render(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush)
{
	if (m_MslExp_Img != NULL) {
		// MslExpRender(a_pd2dRTarget, a_pBrush);
		return;
	}		// �̻��� ���� ���� ����Ʈ �������� ��� ��ŷ

	if (m_PtcImage == NULL) {
		return;
	}

	m_RenderPos.x = SpawnPosition.x - (int)m_CamPos.x;
	m_RenderPos.y = SpawnPosition.y - (int)m_CamPos.y;

	float PER = (m_LastTime - CreatedTime) / LifeTime;

	Vector2D SimulatedPos = m_RenderPos + (Velocity * PER);
	Vector2D SimulatedSize = StartSize + (EndSize - StartSize) * PER;

	SimulatedPos.x = m_RenderPos.x + (Velocity.x * PER);
	SimulatedPos.y = m_RenderPos.y + (Velocity.y * PER);

	SimulatedSize.x = StartSize.x + (EndSize.x - StartSize.x) * PER;
	SimulatedSize.y = StartSize.y + (EndSize.y - StartSize.y) * PER;

	float Alpha_Per = StartAlpha + ((EndAlpha - StartAlpha) * PER);		// 1.0f -> 0.0f

	float x = SimulatedPos.x - SimulatedSize.x * 0.5f;
	float y = SimulatedPos.y - SimulatedSize.y * 0.5f;

	AdjustRenderPosForShake(x);
	AdjustRenderPosForShake(y);

	D2D1_POINT_2F center_pos = { x + SimulatedSize.x * 0.5f, y + SimulatedSize.y * 0.5f };
	a_pd2dRTarget->SetTransform(D2D1::Matrix3x2F::Rotation(m_DirRot, center_pos));

	a_pd2dRTarget->DrawBitmap(m_PtcImage, D2D1::RectF(x, y, x + SimulatedSize.x, y + SimulatedSize.y), Alpha_Per, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);

	if (PER > 1.0f) {
		m_EmtActive = false;		// ����
	}

	a_pd2dRTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

//--- Missile ���� ���� �̹��� ����
void CPtcEmitter::CalculateUVRect(float per)
{
	UVPer = per;

	int Index = (int)(UVPer * (UVX * UVY));

	int RWidth = ImgRealWidth / UVX;
	int RHeight = ImgRealHeight / UVY;

	// �ε��� 0���� ����

	// 4 X 4 ���� 4���? ����� 5
	/*
	a	a	a	a
	a	a	a	a
	a	a	a	a
	a	a	a	a
	*/

	int XX = Index % UVX;
	int YY = 0;
	if (Index != 0) {
		YY = Index / UVX;
	}
	UVRect.left = XX * RWidth;
	UVRect.top = YY * RHeight;

	UVRect.right = (XX + 1) * RWidth;
	UVRect.bottom = (YY + 1) * RHeight;
}

// ������ �������� ������...
// �������� ���̵��� ������... (���� �������� ���� �����ϴ� �� �������...)
void CPtcEmitter::MslExpRender(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush)
{
	// �̻��� ���� ���� ����Ʈ ������ �Լ�
	if (m_MslExp_Img == NULL) {
		return;
	}

	// GetGameTimeSinceCreation() == m_LastTime
	static float a_PassTime = 0.0f;
	a_PassTime = (float)((m_LastTime - CreatedTime) * 0.001);

	CalculateUVRect(a_PassTime);

	m_RenderPos.x = SpawnPosition.x - (int)m_CamPos.x;
	m_RenderPos.y = SpawnPosition.y - (int)m_CamPos.y;

	static float x = 0.0f;
	static float y = 0.0f;
	static float width = 0.0f;
	static float height = 0.0f;
	x = m_RenderPos.x - 110.0f;
	y = m_RenderPos.y - 110.0f;
	width = 220.0f;
	height = 220.0f;

	AdjustRenderPosForShake(x);
	AdjustRenderPosForShake(y);
	//------ �̹��� ������
	a_pd2dRTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	a_pd2dRTarget->DrawBitmap(m_MslExp_Img, D2D1::RectF(x, y, x + width, y + height), 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, UVRect);
	//------ �̹��� ������

	if (a_PassTime > 1.0f) {
		m_EmtActive = false;		// ����
	}
}
//--- Missile ���� ���� �̹��� ����