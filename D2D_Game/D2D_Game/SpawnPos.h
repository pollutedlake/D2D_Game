#pragma once

#include "Vector2D.h"
#include <vector>
using namespace std;

#include <d2d1.h>

class SpawnPos
{
public:
	Vector2D m_Pos = { 0.0f,0.0f };
	float m_SpDelay = 0.1f;
	ID2D1Bitmap* m_SocketImg;		// �ִ� ����ó�� ����� ������
	/*if (0.0f < OldDelay && m_SpDelay <= 0.0f) {
		�޸� Ǯ���� ���� �Ѹ��� ������Ű�� �����ʿ� �ε����� ���� �Ѵ�.
	}
	���Ͱ� ���� �� �ش� �ε����� m_SpDelay �༭ ������ �ǰ� ���ش�.*/

public:
	SpawnPos() {}
	~SpawnPos() {}

	bool Update_SpPos(float a_DeltaTime) {
		static float OldDelay = 0.0f;
		OldDelay = m_SpDelay;
		m_SpDelay = m_SpDelay - a_DeltaTime;
		if (m_SpDelay <= 0.0f) {
			m_SpDelay = 0.0f;
			if (0.0f < OldDelay) {
				// �޸�Ǯ���� ���� �Ѹ��� ������Ű�� �����ʿ� �ε����� ���� �Ѵ�.
				return true;
			}
		}	// if (m_SpDelay <= 0.0f)

		return false;
	}	// bool Update_SpPos(float a_DeltaTime)

	void Render_SpPos(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush, VecINT2D& a_ScreenHalf, Vector2D& a_CamPos) {
		if (m_SocketImg == NULL) {
			return;
		}

		static Vector2D m_RenderPos;

		m_RenderPos.x = m_Pos.x - (int)a_CamPos.x;		// ���� ����� �� �� �ε巴�� �����δ�.
		m_RenderPos.y = m_Pos.y - (int)a_CamPos.y;

		static VecINT2D img_size;
		static VecINT2D img_Half;
		img_size.x = m_SocketImg->GetSize().width;
		img_size.y = m_SocketImg->GetSize().height;
		img_Half.x = img_size.x / 2.0f;
		img_Half.y = img_size.y / 2.0f;

		//------ ĳ���� Render
		a_pd2dRTarget->DrawBitmap(m_SocketImg, D2D1::RectF(m_RenderPos.x - img_Half.x, m_RenderPos.y - img_Half.y, m_RenderPos.x + img_Half.x, m_RenderPos.y + img_Half.y));
		//------ ĳ���� Render
	}
};

