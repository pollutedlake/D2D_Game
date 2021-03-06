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

	inline CMotion* GetMotion(AniState a_State) {
		return m_MotionList[(int)a_State];
	}

	inline int GetFrameCount(AniState a_State) {
		return m_MotionList[(int)a_State]->m_ImgList.size();
	}

	inline ID2D1Bitmap* GetFrameImg(AniState a_State, int a_AniInx) {
		return m_MotionList[(int)a_State]->m_ImgList[a_AniInx];
	}
};

