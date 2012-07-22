/* 文字コードはＳＪＩＳ 改行コードはＣＲＬＦ */
/* $Id$ */

#include "stdafx.h"
#include "utplayer.h"
#include "FrameWindow.h"
#include "VideoRenderer.h"

CUtPlayerFrameWindow::CUtPlayerFrameWindow(void)
{
	m_pMediaControl = NULL;
	m_pVideoRenderer = NULL;
}

CUtPlayerFrameWindow::~CUtPlayerFrameWindow(void)
{
}

LRESULT CUtPlayerFrameWindow::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DWM_BLURBEHIND bb;
	HRGN hrgn = CreateRectRgn(-1, -1, 0, 0); // クライアント領域全体が透明

	bb.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
	bb.fEnable = TRUE;
	bb.hRgnBlur = hrgn;
	DwmEnableBlurBehindWindow(m_hWnd, &bb);
	DeleteObject(hrgn);

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
		// 白いブラシならアルファ値に関わらずちゃんと白くなる。（乗算済みアルファなので）
		FillRect(hdc, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));
	}
	EndPaint(&ps);

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

LRESULT CUtPlayerFrameWindow::OnHelpAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	char buf[256];

	sprintf(buf,
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
	IMediaControl *pMediaControl;
	CUtPlayerVideoRenderer *pVideoRenderer;
	WCHAR wszFile[MAX_PATH];
	BOOL bHandled;

	CloseMedia();

	CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (LPVOID *)&pGraphBuilder);

	pVideoRenderer = new CUtPlayerVideoRenderer(m_hWnd, &hr);
	pVideoRenderer->AddRef(); // new した時点では参照カウントが 0
	pGraphBuilder->AddFilter(pVideoRenderer, L"Video Renderer");

	swprintf(wszFile, L"%S", pszFile);
	hr = pGraphBuilder->RenderFile(wszFile, NULL);
	if (!SUCCEEDED(hr))
	{
		pGraphBuilder->Release();
		pVideoRenderer->Release();
		return hr;
	}

	if (!pVideoRenderer->GetPin(0)->IsConnected())
	{
		pGraphBuilder->Release();
		pVideoRenderer->Release();
		return E_FAIL;
	}

	pGraphBuilder->QueryInterface(IID_IMediaControl, (LPVOID *)&pMediaControl);
	pGraphBuilder->Release();

	m_pMediaControl = pMediaControl;
	m_pVideoRenderer = pVideoRenderer;

	OnViewSize(0, ID_VIEW_SIZE_100, NULL, bHandled);

	pMediaControl->Run();
//	pMediaControl->Release(); // フィルタグラフマネージャの最後の参照が Release() されるとグラフ自体が消滅する。

	return S_OK;
}

HRESULT CUtPlayerFrameWindow::CloseMedia()
{
	if (m_pMediaControl != NULL)
	{
		_ASSERT(m_pVideoRenderer != NULL);
		m_pMediaControl->Stop();
		m_pMediaControl->Release();
		m_pMediaControl = NULL;
		m_pVideoRenderer->Release();
		m_pVideoRenderer = NULL;
	}
	else
		_ASSERT(m_pVideoRenderer == NULL);

	return S_OK;
}
