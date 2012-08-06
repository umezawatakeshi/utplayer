/* 文字コードはＳＪＩＳ 改行コードはＣＲＬＦ */
/* $Id$ */

#include "stdafx.h"
#include "utplayer.h"
#include "FrameWindow.h"
#include "VideoRenderer.h"
#include "Util.h"

CUtPlayerFrameWindow::CUtPlayerFrameWindow(void)
{
	m_pMediaControl = NULL;
	m_pMediaSeeking = NULL;
	m_pVideoRenderer = NULL;
}

CUtPlayerFrameWindow::~CUtPlayerFrameWindow(void)
{
}

LRESULT CUtPlayerFrameWindow::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DragAcceptFiles();

	OnViewBackground(0, ID_VIEW_BACKGROUND_TRANSPARENT, NULL, bHandled);

	ResizeClient(320, 240);

	return 0;
}

LRESULT CUtPlayerFrameWindow::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CloseMedia();
	PostQuitMessage(0);
	return 0;
}

LRESULT CUtPlayerFrameWindow::OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CUtPlayerFrameWindow::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rc;

	hdc = BeginPaint(&ps);
	GetClientRect(&rc);
	if (m_pVideoRenderer != NULL)
	{
		SetStretchBltMode(hdc, COLORONCOLOR);
		StretchDIBits(hdc, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
			0, 0, m_pVideoRenderer->GetWidth(), m_pVideoRenderer->GetHeight(),
			m_pVideoRenderer->GetDIBits(), (BITMAPINFO *)m_pVideoRenderer->GetBitmapInfoHeader(),
			DIB_RGB_COLORS, SRCCOPY);
	}
	else
	{
		HBRUSH hBrush;
		hBrush = CreateSolidBrush(0x00000000); // GetStockObject(BLACK_BRUSH) だとアルファ値が 0 であることが保証されない
		FillRect(hdc, &rc, hBrush);
		DeleteObject(hBrush);
	}
	EndPaint(&ps);

	return 0;
}

LRESULT CUtPlayerFrameWindow::OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HDROP hDrop = (HDROP)wParam;
	char szFile[MAX_PATH];

	if (!DragQueryFile(hDrop, 0, szFile, sizeof(szFile)))
	{
		DragFinish(hDrop);
		return 0;
	}

	OpenMediaFile(szFile);

	DragFinish(hDrop);

	return 0;
}

LRESULT CUtPlayerFrameWindow::OnFileOpen(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OPENFILENAME ofn;
	char szFile[MAX_PATH] = "";

	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFilter = "AVI Files (*.avi)\0*.avi\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST;
	if (!GetOpenFileName(&ofn))
		return 0;

	OpenMediaFile(szFile);

	return 0;
}

LRESULT CUtPlayerFrameWindow::OnFileClose(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CloseMedia();
	Invalidate();

	return 0;
}

LRESULT CUtPlayerFrameWindow::OnFileExit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	return SendMessage(WM_CLOSE);
}

LRESULT CUtPlayerFrameWindow::OnViewSize(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	AM_MEDIA_TYPE mt;
	VIDEOINFOHEADER *pvih;
	int width, height;

	if (m_pMediaControl == NULL)
		return 0;

	m_pVideoRenderer->GetPin(0)->ConnectionMediaType(&mt);
	pvih = (VIDEOINFOHEADER *)mt.pbFormat;
	width = pvih->bmiHeader.biWidth;
	height = pvih->bmiHeader.biHeight;
	switch (wID)
	{
	case ID_VIEW_SIZE_50:
		width  = (width  + 1) / 2;
		height = (height + 1) / 2;
		break;
	case ID_VIEW_SIZE_200:
		width  *= 2;
		height *= 2;
		break;
	}
	ResizeClient(width, height);
	FreeMediaType(mt);

	return 0;
}

LRESULT CUtPlayerFrameWindow::OnViewBackground(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	DWM_BLURBEHIND bb;
	HRGN hrgn;

	if (wID == ID_VIEW_BACKGROUND_TRANSPARENT)
		hrgn = CreateRectRgn(-1, -1, 0, 0);
	else
		hrgn = NULL;
	bb.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
	bb.fEnable = (wID != ID_VIEW_BACKGROUND_BLACK);
	bb.hRgnBlur = hrgn;
	DwmEnableBlurBehindWindow(m_hWnd, &bb);
	if (hrgn != NULL)
		DeleteObject(hrgn);

	CheckMenuRadioItem(GetMenu(), ID_VIEW_BACKGROUND_FIRST, ID_VIEW_BACKGROUND_LAST, wID, MF_BYCOMMAND);

	return 0;
}

LRESULT CUtPlayerFrameWindow::OnPlayPlayPause(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	OAFilterState fs;

	if (m_pMediaControl == NULL)
		return 0;

	if (FAILED(m_pMediaControl->GetState(500, &fs)))
		return 0;

	switch (fs)
	{
	case State_Stopped:
	case State_Paused:
		m_pMediaControl->Run();
		break;
	case State_Running:
		m_pMediaControl->Pause();
		break;
	}

	return 0;
}

LRESULT CUtPlayerFrameWindow::OnPlayStop(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if (m_pMediaControl == NULL)
		return 0;

	m_pMediaControl->Stop();

	return 0;
}

LRESULT CUtPlayerFrameWindow::OnPlayRewind(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	LONGLONG llPos = 0;

	if (m_pMediaControl == NULL)
		return 0;

	m_pMediaSeeking->SetPositions(&llPos, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning);

	return 0;
}

LRESULT CUtPlayerFrameWindow::OnPlayStepBackward(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	LONGLONG llPos;

	if (m_pMediaControl == NULL)
		return 0;

	//m_pMediaSeeking->SetPositions(&llPos, AM_SEEKING_RelativePositioning, NULL, AM_SEEKING_NoPositioning); // AM_SEEKING_RelativePositioning で過去にシークすることはできない…？
	if (FAILED(m_pMediaSeeking->GetCurrentPosition(&llPos)))
		return 0;
	llPos = max(llPos - 50000000LL, 0LL);
	m_pMediaSeeking->SetPositions(&llPos, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning);

	return 0;
}

LRESULT CUtPlayerFrameWindow::OnPlayStepForward(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	LONGLONG llPos = +50000000LL;

	if (m_pMediaControl == NULL)
		return 0;

	m_pMediaSeeking->SetPositions(&llPos, AM_SEEKING_RelativePositioning, NULL, AM_SEEKING_NoPositioning);

	return 0;
}

LRESULT CUtPlayerFrameWindow::OnPlayPlaySpeed(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	double rate = 1.0;

	if (m_pMediaControl == NULL)
		return 0;

	switch (wID)
	{
	case ID_PLAY_PLAYSPEED_50:
		rate = 0.5;
		break;
	case ID_PLAY_PLAYSPEED_100:
		rate = 1.0;
		break;
	case ID_PLAY_PLAYSPEED_200:
		rate = 2.0;
		break;
	}

	m_pMediaSeeking->SetRate(rate);

	return 0;
}

LRESULT CUtPlayerFrameWindow::OnHelpAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	char buf[256];

	wsprintf(buf,
		"Ut Alpha Video Player, version %s\n"
		"Copyright (C) 2012  UMEZAWA Takeshi\n\n"
		"Licensed under GNU General Public License version 2 or later.",
		UTPLAYER_VERSION_STR);
	MessageBox(buf);

	return 0;
}

HRESULT CUtPlayerFrameWindow::OpenMediaFile(LPCSTR pszFile)
{
	HRESULT hr = S_OK;
	IGraphBuilder *pGraphBuilder;
	CUtPlayerVideoRenderer *pVideoRenderer;
	WCHAR wszFile[MAX_PATH];
	BOOL bHandled;

	CloseMedia();

	CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (LPVOID *)&pGraphBuilder);

	pVideoRenderer = new CUtPlayerVideoRenderer(m_hWnd, &hr);
	pVideoRenderer->AddRef(); // new した時点では参照カウントが 0
	pGraphBuilder->AddFilter(pVideoRenderer, L"Video Renderer");

	wsprintfW(wszFile, L"%S", pszFile);
	hr = pGraphBuilder->RenderFile(wszFile, NULL);
	if (!SUCCEEDED(hr))
	{
		LPTSTR pszError;
		pGraphBuilder->Release();
		pVideoRenderer->Release();
		pszError = FormatErrorAllocateBuffer(hr);
		::MessageBox(m_hWnd, hInstance, IDS_FAILED_TO_BUILD_GRAPH, MB_ICONERROR,
			hr, pszError ? pszError : "");
		LocalFree(pszError);
		return hr;
	}

	if (!pVideoRenderer->GetPin(0)->IsConnected())
	{
		pGraphBuilder->Release();
		pVideoRenderer->Release();
		::MessageBox(m_hWnd, hInstance, IDS_FAILED_TO_CONNECT_TO_VIDEO_RENDERER, MB_ICONERROR);
		return E_FAIL;
	}

	pGraphBuilder->QueryInterface(IID_IMediaControl, (LPVOID *)&m_pMediaControl);
	pGraphBuilder->QueryInterface(IID_IMediaSeeking, (LPVOID *)&m_pMediaSeeking);
	pGraphBuilder->Release();

	m_pVideoRenderer = pVideoRenderer;

	OnViewSize(0, ID_VIEW_SIZE_100, NULL, bHandled);

	m_pMediaControl->Run();

	return S_OK;
}

HRESULT CUtPlayerFrameWindow::CloseMedia()
{
	if (m_pMediaControl != NULL)
	{
		_ASSERT(m_pMediaSeeking != NULL);
		_ASSERT(m_pVideoRenderer != NULL);
		m_pMediaControl->Stop();
		m_pMediaControl->Release();
		m_pMediaControl = NULL;
		m_pMediaSeeking->Release();
		m_pMediaSeeking = NULL;
		m_pVideoRenderer->Release();
		m_pVideoRenderer = NULL;
	}
	else
	{
		_ASSERT(m_pMediaSeeking == NULL);
		_ASSERT(m_pVideoRenderer == NULL);
	}

	return S_OK;
}
