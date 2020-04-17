#pragma once
#include "CMotion.h"

#include <vector>
using namespace std;

class CAnimData
{
public:
	vector<CMotion*> m_MotionList;

public:
	CAnimData();
	~CAnimData();

	void LoadAnimList(enum CT_Type a_CharicType);		// 캐릭터 종류가 "몬스터"인지? "주인공"인지?
};

