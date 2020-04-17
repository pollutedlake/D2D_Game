#pragma once

#include <vector>
using namespace std;

#pragma comment(lib, "d2d1.lib")		// <--- Direct2D 사용하기...
#include <d2d1.h>
#include <functional>		// function 함수포인터를 저장할 변수 선언을 지원하는 STL 헤더

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
	vector<ID2D1Bitmap*> m_ImgList;		// 프레임 로딩
	static function<ID2D1Bitmap * (LPCWSTR fileName)> CB_LoadImg;		// <--- STL에서 제공해 주는 함수 포인터 변수 생성

public:
	CMotion();
	~CMotion();

	void LoadTexImg(const TCHAR* a_FName);
};

