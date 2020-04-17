#pragma once

#pragma comment(lib, "d2d1.lib")		// <--- Direct2D 사용하기...
#include <d2d1.h>

class SubImage
{
public:
	ID2D1Bitmap* m_pImg = NULL;
	float m_PosXReRt = 0.5;		// SubImage 만의 사이즈 조정에 따른 위치 조정 값
	float m_PosYReRt = 0.5;		// SubImage 만의 사이즈 조정에 따른 위치 조정 값
	float m_SzXRate = 1.0f;		// 이미지 사이즈 스케일 X
	float m_SzYRate = 1.0f;		// 이미지 사이즈 스케일 Y
	float m_ImgSizeX = 100.0f;
	float m_ImgSizeY = 100.0f;

public:
	SubImage();
	~SubImage();
};

