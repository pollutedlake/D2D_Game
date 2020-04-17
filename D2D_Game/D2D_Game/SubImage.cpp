#include "framework.h"
#include "SubImage.h"

SubImage::SubImage()
{
}

SubImage::~SubImage()
{
	if (m_pImg != NULL) {
		m_pImg->Release();
		m_pImg = NULL;
	}
}
