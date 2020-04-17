#pragma once

#include <vector>
using namespace std;

#pragma comment(lib, "d2d1.lib")		// <--- Direct2D ����ϱ�...
#include <d2d1.h>
#include <functional>		// function �Լ������͸� ������ ���� ������ �����ϴ� STL ���

enum AniState {
	AS_None,
	Idle,
	Front_Walk,
	Back_Walk,
	Left_Walk,
	Right_Walk,
	AniLength,
};

class CMotion
{
public:
	vector<ID2D1Bitmap*> m_ImgList;		// ������ �ε�
	static function<ID2D1Bitmap * (LPCWSTR fileName)> CB_LoadImg;		// <--- STL���� ������ �ִ� �Լ� ������ ���� ����

public:
	CMotion();
	~CMotion();

	void LoadTexImg(const TCHAR* a_FName);
};

