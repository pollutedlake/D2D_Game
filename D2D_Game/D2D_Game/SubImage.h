#pragma once

#pragma comment(lib, "d2d1.lib")		// <--- Direct2D ����ϱ�...
#include <d2d1.h>

class SubImage
{
public:
	ID2D1Bitmap* m_pImg = NULL;
	float m_PosXReRt = 0.5;		// SubImage ���� ������ ������ ���� ��ġ ���� ��
	float m_PosYReRt = 0.5;		// SubImage ���� ������ ������ ���� ��ġ ���� ��
	float m_SzXRate = 1.0f;		// �̹��� ������ ������ X
	float m_SzYRate = 1.0f;		// �̹��� ������ ������ Y
	float m_ImgSizeX = 100.0f;
	float m_ImgSizeY = 100.0f;

public:
	SubImage();
	~SubImage();
};

