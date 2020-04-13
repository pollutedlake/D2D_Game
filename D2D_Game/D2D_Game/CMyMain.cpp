#include "framework.h"
#include "CMyMain.h"
#include "CBackGround.h"
#include "CHero.h"
#include "CMonster_Mgr.h"
#include "resource.h"
#pragma warning(disable: 4996)
#include "CBullet_Mgr.h"
#include "GlobalValue.h"

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

	//--- DeltaTime 얻어 오는 방법
	m_LastTime = timeGetTime();
	m_LastTick = timeGetTime();
	// g_vLastTime = m_LastTIme;
	//--- DeltaTime 얻어 오는 방법

	//------ Direct2D 초기화 부분 / 메모리 확보는 UpdateWindow() 하기 전에 해야 한다.
	HRESULT hr = E_FAIL;
	// D2D를 초기화한다.
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pd2dFactory);

	RECT rc;
	GetClientRect(a_hWnd, &rc);
	D2D1_SIZE_U uSize = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
	m_pd2dFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(a_hWnd, uSize, D2D1_PRESENT_OPTIONS_IMMEDIATELY), &m_pd2dRenderTarget);

	m_pd2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_Brush);

	// 폰트 생성
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(m_pDWriteFactory), reinterpret_cast<IUnknown**>(&m_pDWriteFactory));
	// &pTextFormat : Gulim(굴림체)와 폰트 크기는 50 글자의 정렬 등의 정보를 넣어준다.
	m_pDWriteFactory->CreateTextFormat(_T("Gulim"), NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 40, _T(""), &m_pTextFormat);

	//------ 이미지 로딩 준비
	CoInitialize(0);		// Com초기화

	CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_pImageFactory));
	//------ 이미지 로딩 준비
	//------ Direct2D 초기화 부분 / 메모리 확보는 UpdateWindow() 하기 전에 해야 한다.

	//------ 배경 초기화
	g_BG_Mgr.BGMgrInit(D2DLoadImg);
	//------ 배경 초기화

	//------ 주인공 이미지 로딩 부분
	g_Hero.Init_Unit(D2DLoadImg);
	//------ 주인공 이미지 로딩 부분

	//------ 몬스터 매니저 초기화
	g_Mon_Mgr.MonMgr_Init(a_hWnd, D2DLoadImg);
	//------ 몬스터 매니저 초기화

	//------ 총알 매니저 호출
	g_Bullet_Mgr.BLMgerInit(D2DLoadImg);
	//------ 총알 매니저 호출

	LoadMonSpPos();		// 몬스터 Pos정보 로딩
}

void CMyMain::MainUpdate()
{
	//------ DeltaTime 얻어오는 방법
	timeBeginPeriod(1);		// 정밀도를 높이기 위한 함수 사용
	m_CurTime = timeGetTime();
	m_DeltaTime = (float)((m_CurTime - m_LastTime) * 0.001f);
	m_LastTime = m_CurTime;
	// g_vLastTime = m_LastTime;
	timeEndPeriod(1);
	//------ DeltaTime 얻어오는 방법

	static RECT a_Crt;
	GetClientRect(m_hWnd, &a_Crt);		// MainDC의 가로 세로 사이즈를 얻어오는 함수
	m_ScreenHalf.x = (int)(a_Crt.right / 2);
	m_ScreenHalf.y = (int)(a_Crt.bottom / 2);

	//------ 주인공 업데이트 부분
	g_Hero.Update_Unit(m_DeltaTime);
	//------ 주인공 업데이트 부분

	m_CamPos = g_Hero.m_CurPos;		// 카메라 이동
	m_CamPos.x = m_CamPos.x - (float)m_ScreenHalf.x;
	m_CamPos.y = m_CamPos.y - (float)m_ScreenHalf.y;
	// m_CamPos : <--- 지형에 따른 카메라 리미트값 제한
	LimitMoveCam();
	g_Hero.Abs_Update(m_ScreenHalf, m_CamPos);

	//------ MousePick 처리 부분
	static bool a_IsCkLBtn = false;
	static bool m_IsUILock = false;
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
		// 윈도우가 액티브 상태일 때만 반응하도록 예외처리 고려...
		if (GetFocus() != NULL) {		// 내 윈도우가 포커스를 가지고 있지 않으면 NULL이 리턴됨
			if (a_IsCkLBtn == false) {		// WM_LBUTTONDOWN: 대신
				static POINT a_NewMMXZ;
				GetCursorPos(&a_NewMMXZ);		// 현재 마우스 좌표를 얻어오는 함수
				ScreenToClient(m_hWnd, &a_NewMMXZ);		// 클라이언트 화면 좌표로 변경해 주는 함수
				static Vector2D a_TargetV;
				// 피킹 좌표(스크린좌표)를 절대좌표(월드좌표)로 환산
				a_TargetV.x = (int)a_NewMMXZ.x + m_CamPos.x;		// 마우스의 좌표
				a_TargetV.y = (int)a_NewMMXZ.y + m_CamPos.y;

				if (m_IsMonEdit == true) {
					g_Mon_Mgr.AddMonSpPos(a_TargetV);
					static TCHAR str[128];
					_stprintf_s(str, _T("몬스터 에디트 모드 : %d"), g_Mon_Mgr.m_SpawnPos.size());
					SetWindowText(m_hWnd, (LPTSTR)str);
				}
				else {
					g_Hero.MsPicking(a_TargetV);
				}

				a_IsCkLBtn = true;
			}	// if (a_IsCkLBtn == false)
		}	// if (GetFocus() != NULL)
	}	// if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	else {
		m_IsUILock = false;
		a_IsCkLBtn = false;
	}
	//------ MousePick 처리 부분

	//------ 총알 발사
	static float g_Ticktime = 0.0f;

	//------ MachineGun Timer
	static int g_AddCount = 0;
	static float g_MgEcTick = 0.0f;
	static float g_MgTickTm = 0.0f;

	g_MgEcTick = g_MgEcTick - m_DeltaTime;
	if (g_MgEcTick < -10.0f) {
		g_MgEcTick = -10.0f;
	}

	g_MgTickTm = g_MgTickTm - m_DeltaTime;
	if (g_MgTickTm < -10.0f) {
		g_MgTickTm = -10.0f;
	}
	//------ MachineGun Timer

	if (GetFocus() != NULL) {		// 내 윈도우가 포커스를 가지고 있다는 뜻
		if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {		// 마우스 우측 버튼 클릭
			if (WeaponSlot == PISTOL) {
				if (g_MgEcTick <= 0.0f) {
					static POINT a_MPos;
					GetCursorPos(&a_MPos);
					ScreenToClient(m_hWnd, &a_MPos);

					static Vector2D a_TargetV;
					a_TargetV.x = (float)a_MPos.x + g_Hero.m_CamPos.x;		// 마우스의 좌표
					a_TargetV.y = (float)a_MPos.y + g_Hero.m_CamPos.y;

					g_Bullet_Mgr.SpawnPistol(g_Hero.m_CurPos, a_TargetV);

					g_MgEcTick = 0.2f;
				}	// if (g_MgEcTick <= 0.0f)
			}	// if (WeaponSlot == PISTOL)

			else if (WeaponSlot == HEAVY_MACHINE_GUN) {
				if (g_MgTickTm <= 0.0f) {
					g_MgEcTick = 0.0f;
					g_AddCount = 0;

					g_MgTickTm = 0.064f;
				}

				// 이전 총알이 움직이기 전에 몇개의 총알이 발사되게 할 것인지? 설정하는 코드
				if (g_MgEcTick <= 0.0f && g_AddCount < 4) {		// <--- 쿨타임 적용 3발 연속 사격
					// g_AddCount 한번에 발사되는 총알의 갯수를 의미함
					static POINT a_MPos;
					GetCursorPos(&a_MPos);
					ScreenToClient(m_hWnd, &a_MPos);

					static Vector2D a_TargetV;
					a_TargetV.x = (float)a_MPos.x + g_Hero.m_CamPos.x;
					a_TargetV.y = (float)a_MPos.y + g_Hero.m_CamPos.y;

					g_Bullet_Mgr.SpawnMachineGun(g_Hero.m_CurPos, a_TargetV);

					g_MgEcTick = 0.02f;		// 머신건의 쿨타임은 0.02초로 되어 있음
					g_AddCount++;
				}
			}	// else if (WeaponSlot == HEAVY_MACHINE_GUN)

			else if (WeaponSlot == ROCKET_LAUNCHER) {
				if (g_MgEcTick <= 0.0f) {
					static POINT a_MPos;
					GetCursorPos(&a_MPos);
					ScreenToClient(m_hWnd, &a_MPos);

					static Vector2D a_TargetV;
					a_TargetV.x = (float)a_MPos.x + g_Hero.m_CamPos.x;
					a_TargetV.y = (float)a_MPos.y + g_Hero.m_CamPos.y;

					g_Bullet_Mgr.SpawnRocket(g_Hero.m_CurPos, a_TargetV);

					g_MgEcTick = 0.2f;
				}	// if (g_MgEcTick <= 0.0f)
			}	// else if (WeaponSlot == ROCKET_LAUNCHER)
		}	// if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	}
	//------ 총알 발사

	//------ 배경 업데이트
	g_BG_Mgr.BGMgrUpdate(m_DeltaTime, m_ScreenHalf, m_CamPos);
	//------ 배경 업데이트

	//------ 몬스터 매니저 업데이트
	if (m_IsMonEdit == false) {
		g_Mon_Mgr.MonMgr_Update(m_DeltaTime, m_ScreenHalf, m_CamPos, g_Hero);
	}
	//------ 몬스터 매니저 업데이트

	//------ 총알 업데이트
	g_Bullet_Mgr.m_CenterPos = m_ScreenHalf;
	g_Bullet_Mgr.m_CamPos = g_Hero.m_CamPos;
	GDeltaUpdate(m_DeltaTime);
	g_Bullet_Mgr.BLMgerUpdate(m_DeltaTime, m_LastTime, GetMvDelta, NULL, NULL);
	//------ 총알 업데이트
}	// void CMyMain::MainUpdate()

void CMyMain::MainRender(HWND a_hWnd)
{
	if (m_pd2dRenderTarget == NULL) {
		return;
	}

	//------ FPS 출력용 계산
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
	//------ FPS 출력용 계산

	m_pd2dRenderTarget->BeginDraw();

	m_pd2dRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

	// ToDo...
	//------ 배경 렌더링
	g_BG_Mgr.BGMgrRender(m_pd2dRenderTarget);
	//------ 배경 렌더링

	//--- 몬스터 이미지 렌더링...
	if (m_IsMonEdit == false) {		// 게임모드 몬스터 이미지 렌더링...
		g_Mon_Mgr.MonMgr_Render(m_pd2dRenderTarget, m_Brush);
	}
	else if (m_IsMonEdit == true) {		// 몬스터 에디터모드 이미지 렌더링...
		g_Mon_Mgr.MonEdit_Render(m_pd2dRenderTarget, m_Brush, m_ScreenHalf, m_CamPos);
	}
	//--- 몬스터 이미지 렌더링...

	//------ 주인공 렌더링
	g_Hero.Render_Unit(m_pd2dRenderTarget, m_Brush);
	//------ 주인공 렌더링

	//--- 총알 렌더링
	g_Bullet_Mgr.BLMgerRender(m_pd2dRenderTarget, m_Brush);
	//--- 총알 렌더링

	m_Brush->SetColor(D2D1::ColorF(0xff00ff));
	D2D1_SIZE_F renderTargetSize = m_pd2dRenderTarget->GetSize();
	m_pd2dRenderTarget->DrawText(strFPS, wcslen(strFPS), m_pTextFormat, D2D1::RectF(0, 0, renderTargetSize.width, renderTargetSize.height), m_Brush);

	m_pd2dRenderTarget->EndDraw();
}

void CMyMain::MainDestroy()
{
	//--- 몬스터 제거...
	g_Mon_Mgr.MonMgr_Destroy();
	//--- 몬스터 제거...

	//--- 주인공 제거
	g_Hero.Destroy_Unit();
	//--- 주인공 제거

	//------ 배경 제거
	g_BG_Mgr.BGMgrDestroy();
	//------ 배경 제거

	//--- 총알 리소스 제거
	g_Bullet_Mgr.BLMgerDestroy();
	//--- 총알 리소스 제거

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

	//------ 캐릭터 지형 밖으로 벗어날 수 없게 제한 걸기...
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
	//------ 캐릭터 지형 밖으로 벗어날 수 없게 제한 걸기...
}

void CMyMain::MonEditMenu()
{
	m_IsMonEdit = !m_IsMonEdit;
	HMENU hMainMenu = GetMenu(m_hWnd);
	HMENU hMenu = GetSubMenu(hMainMenu, 1);		// 서브메뉴
	if (m_IsMonEdit == true) {
		TCHAR str[128];
		_stprintf_s(str, _T("몬스터 에디트 모드 : %d"), g_Mon_Mgr.m_SpawnPos.size());
		SetWindowText(m_hWnd, (LPTSTR)str);
		CheckMenuItem((HMENU)hMenu, ID_MONEDITMODE, MF_BYCOMMAND | MF_CHECKED);
	}
	else {
		SetWindowText(m_hWnd, (LPTSTR)m_szTitle);
		CheckMenuItem((HMENU)hMenu, ID_MONEDITMODE, MF_BYCOMMAND | MF_UNCHECKED);

		g_Mon_Mgr.ReSrcClear();
		for (int ii = 0; ii < g_Mon_Mgr.m_SpawnPos.size(); ii++) {
			g_Mon_Mgr.m_SpawnPos[ii].m_SpDelay = 0.1f;		// 다시 생성해 준다.
		}

		if (MessageBox(NULL, _T("지금 편집한 상태를 저장하시겠습니까?"), _T("확인"), MB_YESNO) == IDYES) {
			SaveMonSpPos();		// 자동 저장
		}
	}	// else
}

void CMyMain::LoadMonSpPos()
{
	//------ 파일 로딩
	FILE* a_rPF = fopen("MySave.abc", "rb");
	if (a_rPF != NULL) {
		int Count = 0;
		fread(&Count, sizeof(int), 1, a_rPF);
		static Vector2D a_SpPosV;
		int a_XX = 0;
		int a_YY = 0;
		for (int aa = 0; aa < Count; aa++) {
			fread(&a_XX, sizeof(int), 1, a_rPF);
			fread(&a_YY, sizeof(int), 1, a_rPF);
			a_SpPosV.x = a_XX;
			a_SpPosV.y = a_YY;
			g_Mon_Mgr.AddMonSpPos(a_SpPosV);
		}

		fclose(a_rPF);
	}
	//------ 파일 로딩
}

void CMyMain::SaveMonSpPos()
{
	//------ 파일 저장
	FILE* a_wPF = fopen("MySave.abc", "wb");
	if (a_wPF != NULL) {
		int Count = g_Mon_Mgr.m_SpawnPos.size();
		fwrite(&Count, sizeof(int), 1, a_wPF);
		int a_XX = 0;
		int a_YY = 0;
		for (int aa = 0; aa < Count; aa++) {
			a_XX = g_Mon_Mgr.m_SpawnPos[aa].m_Pos.x;
			a_YY = g_Mon_Mgr.m_SpawnPos[aa].m_Pos.y;
			fwrite(&a_XX, sizeof(int), 1, a_wPF);
			fwrite(&a_YY, sizeof(int), 1, a_wPF);
		}

		fclose(a_wPF);		// fopen() 짝
	}
	//------ 파일 저장
}

void CMyMain::On_MessageHook(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 메뉴 선택을 구문 분석합니다.
		switch (wmId) {
		case ID_MONEDITMODE:		// 몬스터 에디터 모드
		{
			MonEditMenu();
		}
		break;
		}	// switch (wmId)
	}	// case WM_COMMAND:
	break;

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

// 이미지 로딩 함수 정의
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

	// 컨버트된 데이터를 기반으로 해서 실제 렌더링 가능한 형태의 메모리를 생성해야 합니다.
	hr = a_pRenderTarget->CreateBitmapFromWicBitmap(a_pConverter, nullptr, bitmap);

	// 생성했던 자원 해제
	a_pDecoder->Release();
	a_pFrameDecoder->Release();
	a_pConverter->Release();

	return hr;
}

CMyMain g_MyMain;