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

	void LoadAnimList(enum CT_Type a_CharicType);		// ĳ���� ������ "����"����? "���ΰ�"����?
};

