#include "framework.h"
#include "CMotion.h"

function<ID2D1Bitmap * (LPCWSTR fileName)> CMotion::CB_LoadImg = NULL;

CMotion::CMotion()
{
}

CMotion::~CMotion()
{
	for (int ii = 0; ii < m_ImgList.size(); ii++) {
		//--- D2D Img 해제
		if (m_ImgList[ii] != NULL) {
			m_ImgList[ii]->Release();
			m_ImgList[ii] = NULL;
		}
		//--- D2D Img 해제
	}
	m_ImgList.clear();
}

void CMotion::LoadTexImg(const TCHAR* a_FName)
{
	if (CB_LoadImg == NULL) {
		return;
	}

	ID2D1Bitmap* a_pBitmap = CB_LoadImg(a_FName);
	m_ImgList.push_back(a_pBitmap);
}
