#include "framework.h"
#include "CUIBase.h"

void CUIBase::OnPress_AddListener(function<void(void)> Callback)		// PointerDown
{
	PressCallback = Callback;
}

void CUIBase::OnClick_AddListener(function<void(void)> Callback)		// PointerUp
{
	ClickCallback = Callback;
}
