#include "framework.h"
#include "CAnimData.h"
#include "GlobalValue.h"

CAnimData::CAnimData()
{
	for (int ii = 0; ii < AniLength; ii++) {
		CMotion* a_Node = new CMotion();
		m_MotionList.push_back(a_Node);
	}
}

CAnimData::~CAnimData()
{
	for (int ii = 0; ii < m_MotionList.size(); ii++) {
		if (m_MotionList[ii] != NULL) {
			delete m_MotionList[ii];
			m_MotionList[ii] = NULL;
		}
	}
	m_MotionList.clear();
}

void CAnimData::LoadAnimList(CT_Type a_CharicType)
{
    if (a_CharicType == CT_Nanami)
    {
        m_MotionList[(int)Idle]->LoadTexImg(_T(".\\RscTrunk\\Nanami\\front_Idle\\n001.png"));
        m_MotionList[(int)Idle]->LoadTexImg(_T(".\\RscTrunk\\Nanami\\front_Idle\\n002.png"));

        m_MotionList[(int)Front_Walk]->LoadTexImg(_T(".\\RscTrunk\\Nanami\\front_walk\\n010.png"));
        m_MotionList[(int)Front_Walk]->LoadTexImg(_T(".\\RscTrunk\\Nanami\\front_walk\\n011.png"));
        m_MotionList[(int)Front_Walk]->LoadTexImg(_T(".\\RscTrunk\\Nanami\\front_walk\\n012.png"));
        m_MotionList[(int)Front_Walk]->LoadTexImg(_T(".\\RscTrunk\\Nanami\\front_walk\\n013.png"));

        m_MotionList[(int)Back_Walk]->LoadTexImg(_T(".\\RscTrunk\\Nanami\\back_walk\\n040.png"));
        m_MotionList[(int)Back_Walk]->LoadTexImg(_T(".\\RscTrunk\\Nanami\\back_walk\\n041.png"));
        m_MotionList[(int)Back_Walk]->LoadTexImg(_T(".\\RscTrunk\\Nanami\\back_walk\\n042.png"));
        m_MotionList[(int)Back_Walk]->LoadTexImg(_T(".\\RscTrunk\\Nanami\\back_walk\\n043.png"));

        m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Nanami\\left_walk\\n030.png"));
        m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Nanami\\left_walk\\n031.png"));
        m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Nanami\\left_walk\\n032.png"));
        m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Nanami\\left_walk\\n033.png"));

        m_MotionList[(int)Right_Walk]->LoadTexImg(_T(".\\RscTrunk\\Nanami\\right_walk\\n020.png"));
        m_MotionList[(int)Right_Walk]->LoadTexImg(_T(".\\RscTrunk\\Nanami\\right_walk\\n021.png"));
        m_MotionList[(int)Right_Walk]->LoadTexImg(_T(".\\RscTrunk\\Nanami\\right_walk\\n022.png"));
        m_MotionList[(int)Right_Walk]->LoadTexImg(_T(".\\RscTrunk\\Nanami\\right_walk\\n023.png"));
    }
}
