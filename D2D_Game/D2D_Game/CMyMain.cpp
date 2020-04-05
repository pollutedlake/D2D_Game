#include "framework.h"
#include "CMyMain.h"
#include "CBackGround.h"
#include "CHero.h"
#include "CMonster_Mgr.h"

CMyMain::CMyMain()
{
}

CMyMain::~CMyMain()
{
}

void CMyMain::MainInit(HWND a_hWnd)
{
	srand((unsigned)time(NULL));		// <--- rand

	m_hWnd = a_hWnd;

	//--- DeltaTime ��� ���� ���
	m_LastTime = timeGetTime();
	m_LastTick = timeGetTime();
	// g_vLastTime = m_LastTIme;
	//--- DeltaTime ��� ���� ���

	//------ Direct2D �ʱ�ȭ �κ� / �޸� Ȯ���� UpdateWindow() �ϱ� ���� �ؾ� �Ѵ�.
	HRESULT hr = E_FAIL;
	// D2D�� �ʱ�ȭ�Ѵ�.
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pd2dFactory);

	RECT rc;
	GetClientRect(a_hWnd, &rc);
	D2D1_SIZE_U uSize = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
	m_pd2dFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(a_hWnd, uSize, D2D1_PRESENT_OPTIONS_IMMEDIATELY), &m_pd2dRenderTarget);

	m_pd2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_Brush);

	// ��Ʈ ����
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(m_pDWriteFactory), reinterpret_cast<IUnknown**>(&m_pDWriteFactory));
	// &pTextFormat : Gulim(����ü)�� ��Ʈ ũ��� 50 ������ ���� ���� ������ �־��ش�.
	m_pDWriteFactory->CreateTextFormat(_T("Gulim"), NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 40, _T(""), &m_pTextFormat);

	//------ �̹��� �ε� �غ�
	CoInitialize(0);		// Com�ʱ�ȭ

	CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_pImageFactory));
	//------ �̹��� �ε� �غ�
	//------ Direct2D �ʱ�ȭ �κ� / �޸� Ȯ���� UpdateWindow() �ϱ� ���� �ؾ� �Ѵ�.

	//------ ��� �ʱ�ȭ
	g_BG_Mgr.BGMgrInit(D2DLoadImg);
	//------ ��� �ʱ�ȭ

	//------ ���ΰ� �̹��� �ε� �κ�
	g_Hero.Init_Unit(D2DLoadImg);
	//------ ���ΰ� �̹��� �ε� �κ�

	//------ ���� �Ŵ��� �ʱ�ȭ
	g_Mon_Mgr.MonMgr_Init(a_hWnd, D2DLoadImg);
	//------ ���� �Ŵ��� �ʱ�ȭ
}

void CMyMain::MainUpdate()
{
	//------ DeltaTime ������ ���
	timeBeginPeriod(1);		// ���е��� ���̱� ���� �Լ� ���
	m_CurTime = timeGetTime();
	m_DeltaTime = (float)((m_CurTime - m_LastTime) * 0.001f);
	m_LastTime = m_CurTime;
	// g_vLastTime = m_LastTime;
	timeEndPeriod(1);
	//------ DeltaTime ������ ���

	static RECT a_Crt;
	GetClientRect(m_hWnd, &a_Crt);		// MainDC�� ���� ���� ����� ������ �Լ�
	m_ScreenHalf.x = (int)(a_Crt.right / 2);
	m_ScreenHalf.y = (int)(a_Crt.bottom / 2);

	//------ ���ΰ� ������Ʈ �κ�
	g_Hero.Update_Unit(m_DeltaTime);
	//------ ���ΰ� ������Ʈ �κ�

	m_CamPos = g_Hero.m_CurPos;		// ī�޶� �̵�
	m_CamPos.x = m_CamPos.x - (float)m_ScreenHalf.x;
	m_CamPos.y = m_CamPos.y - (float)m_ScreenHalf.y;
	// m_CamPos : <--- ������ ���� ī�޶� ����Ʈ�� ����
	LimitMoveCam();
	g_Hero.Abs_Update(m_ScreenHalf, m_CamPos);

	//------ MousePick ó�� �κ�
	static bool a_IsCkLBtn = false;
	static bool m_IsUILock = false;
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
		// �����찡 ��Ƽ�� ������ ���� �����ϵ��� ����ó�� ����...
		if (GetFocus() != NULL) {		// �� �����찡 ��Ŀ���� ������ ���� ������ NULL�� ���ϵ�
			if (a_IsCkLBtn == false) {		// WM_LBUTTONDOWN: ���
				static POINT a_NewMMXZ;
				GetCursorPos(&a_NewMMXZ);		// ���� ���콺 ��ǥ�� ������ �Լ�
				ScreenToClient(m_hWnd, &a_NewMMXZ);		// Ŭ���̾�Ʈ ȭ�� ��ǥ�� ������ �ִ� �Լ�
				static Vector2D a_TargetV;
				// ��ŷ ��ǥ(��ũ����ǥ)�� ������ǥ(������ǥ)�� ȯ��
				a_TargetV.x = (int)a_NewMMXZ.x + m_CamPos.x;		// ���콺�� ��ǥ
				a_TargetV.y = (int)a_NewMMXZ.y + m_CamPos.y;

				g_Hero.MsPicking(a_TargetV);

				a_IsCkLBtn = true;
			}	// if (a_IsCkLBtn == false)
		}	// if (GetFocus() != NULL)
	}	// if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	else {
		m_IsUILock = false;
		a_IsCkLBtn = false;
	}
	//------ MousePick ó�� �κ�

	//------ ��� ������Ʈ
	g_BG_Mgr.BGMgrUpdate(m_DeltaTime, m_ScreenHalf, m_CamPos);
	//------ ��� ������Ʈ
}

void CMyMain::MainRender(HWND a_hWnd)
{
	if (m_pd2dRenderTarget == NULL) {
		return;
	}

	//------ FPS ��¿� ���
	static DWORD FPSdelay = 1000;
	static DWORD thisTick;

	thisTick = timeGetTime();

	if ((thisTick - m_LastTick) >= FPSdelay) {
		m_LastTick = thisTick;
		m_OldFlipCount = m_FlipCount;
		m_FlipCount = 0;
	}
	else {
		m_FlipCount++;
	}

	static TCHAR strFPS[128];
	_stprintf_s(strFPS, 128, _T("FPS : %d"), m_OldFlipCount);
	//------ FPS ��¿� ���

	m_pd2dRenderTarget->BeginDraw();

	m_pd2dRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

	// ToDo...
	//------ ��� ������
	g_BG_Mgr.BGMgrRender(m_pd2dRenderTarget);
	//------ ��� ������

	//------ ���ΰ� ������
	g_Hero.Render_Unit(m_pd2dRenderTarget, m_Brush);
	//------ ���ΰ� ������

	m_Brush->SetColor(D2D1::ColorF(0xff00ff));
	D2D1_SIZE_F renderTargetSize = m_pd2dRenderTarget->GetSize();
	m_pd2dRenderTarget->DrawText(strFPS, wcslen(strFPS), m_pTextFormat, D2D1::RectF(0, 0, renderTargetSize.width, renderTargetSize.height), m_Brush);

	m_pd2dRenderTarget->EndDraw();
}

void CMyMain::MainDestroy()
{
	//------ ��� ����
	g_BG_Mgr.BGMgrDestroy();
	//------ ��� ����

	if (m_pImageFactory != NULL) {
		m_pImageFactory->Release();
		m_pImageFactory = NULL;
	}

	if (m_pTextFormat != NULL) {
		m_pTextFormat->Release();
		m_pTextFormat = NULL;
	}

	if (m_pDWriteFactory != NULL) {
		m_pDWriteFactory->Release();
		m_pDWriteFactory = NULL;
	}

	if (m_Brush != NULL) {
		m_Brush->Release();
		m_Brush = NULL;
	}

	if (m_pd2dRenderTarget != nullptr) {
		m_pd2dRenderTarget->Release();
		m_pd2dRenderTarget = nullptr;
	}

	if (m_pd2dFactory != NULL) {
		m_pd2dFactory->Release();
		m_pd2dFactory = nullptr;
	}
}

void CMyMain::LimitMoveCam()
{
	static int a_LmtBdLeft = 0;
	static int a_LmtBdTop = 0;
	static int a_LmtBdRight = 0;
	static int a_LmtBdBottom = 0;

	if (g_BG_Mgr.m_WorldHfSize.x <= 0 || g_BG_Mgr.m_WorldHfSize.y <= 0) {
		return;
	}

	a_LmtBdLeft = -g_BG_Mgr.m_WorldHfSize.x + 4;
	a_LmtBdTop = -g_BG_Mgr.m_WorldHfSize.y + 4;
	a_LmtBdRight = g_BG_Mgr.m_WorldHfSize.x - 4 - (m_ScreenHalf.x * 2);
	a_LmtBdBottom = g_BG_Mgr.m_WorldHfSize.y - 4 - (m_ScreenHalf.y * 2);

	if (a_LmtBdRight <= a_LmtBdLeft) {
		m_CamPos.x -= m_ScreenHalf.x;
	}
	else {
		if (m_CamPos.x < (float)a_LmtBdLeft) {
			m_CamPos.x = (float)a_LmtBdLeft;
		}
		if ((float)a_LmtBdRight < m_CamPos.x) {
			m_CamPos.x = (float)a_LmtBdRight;
		}
	}

	if (a_LmtBdBottom <= a_LmtBdTop) {
		m_CamPos.y = -m_ScreenHalf.y;
	}
	else {
		if (m_CamPos.y < (float)a_LmtBdTop) {
			m_CamPos.y = (float)a_LmtBdTop;
		}

		if ((float)a_LmtBdBottom < m_CamPos.y) {
			m_CamPos.y = (float)a_LmtBdBottom;
		}
	}

	//------ ĳ���� ���� ������ ��� �� ���� ���� �ɱ�...
	a_LmtBdLeft = -g_BG_Mgr.m_WorldHfSize.x + 30;
	a_LmtBdTop = -g_BG_Mgr.m_WorldHfSize.y + 40;
	a_LmtBdRight = g_BG_Mgr.m_WorldHfSize.x - 30;
	a_LmtBdBottom = g_BG_Mgr.m_WorldHfSize.y + 40;

	if (g_Hero.m_CurPos.x < (float)a_LmtBdLeft) {
		g_Hero.m_CurPos.x = (float)a_LmtBdLeft;
	}

	if ((float)a_LmtBdRight < g_Hero.m_CurPos.x) {
		g_Hero.m_CurPos.x = (float)a_LmtBdRight;
	}

	if (g_Hero.m_CurPos.y < (float)a_LmtBdTop) {
		g_Hero.m_CurPos.y = (float)a_LmtBdTop;
	}

	if ((float)a_LmtBdBottom < g_Hero.m_CurPos.y) {
		g_Hero.m_CurPos.y = (float)a_LmtBdBottom;
	}
	//------ ĳ���� ���� ������ ��� �� ���� ���� �ɱ�...
}

void CMyMain::On_MessageHook(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_SIZE:
	{
		if (m_pd2dRenderTarget != NULL) {
			static RECT rc;
			GetClientRect(m_hWnd, &rc);
			D2D1_SIZE_U uSize = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
			m_pd2dRenderTarget->Resize(uSize);
		}
	}
	break;
	}	// switch (message)
}

// �̹��� �ε� �Լ� ����
ID2D1Bitmap* CMyMain::D2DLoadImg(LPCWSTR fileName)
{
	ID2D1Bitmap* a_pBGBmp = NULL;

	if (g_MyMain.m_pd2dRenderTarget == NULL) {
		return NULL;
	}

	if (g_MyMain.m_pImageFactory == NULL) {
		return NULL;
	}

	D2DLoadBitmap(fileName, g_MyMain.m_pd2dRenderTarget, g_MyMain.m_pImageFactory, &a_pBGBmp);

	return a_pBGBmp;
}

HRESULT CMyMain::D2DLoadBitmap(LPCWSTR fileName, ID2D1RenderTarget* a_pRenderTarget, IWICImagingFactory* a_pImgFactory, ID2D1Bitmap** bitmap)
{
	HRESULT hr = E_FAIL;

	IWICBitmapDecoder* a_pDecoder = NULL;
	hr = a_pImgFactory->CreateDecoderFromFilename(fileName, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &a_pDecoder);

	if (FAILED(hr)) {
		return hr;
	}

	IWICBitmapFrameDecode* a_pFrameDecoder = NULL;
	hr = a_pDecoder->GetFrame(0, &a_pFrameDecoder);

	if (FAILED(hr)) {
		a_pDecoder->Release();
		return hr;
	}

	IWICFormatConverter* a_pConverter = NULL;
	hr = a_pImgFactory->CreateFormatConverter(&a_pConverter);

	if (FAILED(hr)) {
		a_pDecoder->Release();
		a_pFrameDecoder->Release();
		return hr;
	}

	hr = a_pConverter->Initialize(a_pFrameDecoder, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.0f, WICBitmapPaletteTypeCustom);

	if (FAILED(hr)) {
		a_pDecoder->Release();
		return hr;
	}

	// ����Ʈ�� �����͸� ������� �ؼ� ���� ������ ������ ������ �޸𸮸� �����ؾ� �մϴ�.
	hr = a_pRenderTarget->CreateBitmapFromWicBitmap(a_pConverter, nullptr, bitmap);

	// �����ߴ� �ڿ� ����
	a_pDecoder->Release();
	a_pFrameDecoder->Release();
	a_pConverter->Release();

	return hr;
}

CMyMain g_MyMain;