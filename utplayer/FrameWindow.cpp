/* 文字コードはＳＪＩＳ 改行コードはＣＲＬＦ */
/* $Id$ */

#include "stdafx.h"
#include "FrameWindow.h"
#include "VideoRenderer.h"

CUtPlayerFrameWindow::CUtPlayerFrameWindow(void)
{
	m_pMediaControl = NULL;
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
	HDC hdc = (HDC) wParam;
	HBRUSH hbr;

	hbr = CreateSolidBrush(0);
	CRect rect;
	GetClientRect( &rect );
	FillRect( hdc, &rect, hbr );
	DeleteObject(hbr);

	//bHandled = true;
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

LRESULT CUtPlayerFrameWindow::OnFileExit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	return SendMessage(WM_CLOSE);
}

LRESULT CUtPlayerFrameWindow::OnHelpAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	MessageBox("Ut Alpha Video Player\n"
		"Copyright (C) 2012  UMEZAWA Takeshi\n\n"
		"Licensed under GNU General Public License version 2 or later.");
	return 0;
}

HRESULT CUtPlayerFrameWindow::OpenMediaFile(LPCSTR pszFile)
{
	HRESULT hr = S_OK;
	IGraphBuilder *pGraphBuilder;
	IMediaControl *pMediaControl;
	CUtPlayerVideoRenderer *pVideoRenderer;
	WCHAR wszFile[MAX_PATH];

	CloseMedia();

	CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (LPVOID *)&pGraphBuilder);

	pVideoRenderer = new CUtPlayerVideoRenderer(m_hWnd, &hr);
	pGraphBuilder->AddFilter(pVideoRenderer, L"Video Renderer");
	//pVideoRenderer->Release(); // new した時点で参照カウントが 0 なので Release() してはいけない

	swprintf(wszFile, L"%S", pszFile);
	hr = pGraphBuilder->RenderFile(wszFile, NULL);
	if (!SUCCEEDED(hr))
	{
		pGraphBuilder->Release();
		return hr;
	}

	if (!pVideoRenderer->GetPin(0)->IsConnected())
	{
		pGraphBuilder->Release();
		return E_FAIL;
	}

	pGraphBuilder->QueryInterface(IID_IMediaControl, (LPVOID *)&pMediaControl);
	pGraphBuilder->Release();

	pMediaControl->Run();
//	pMediaControl->Release(); // フィルタグラフマネージャの最後の参照が Release() されるとグラフ自体が消滅する。

	m_pMediaControl = pMediaControl;

	return S_OK;
}

HRESULT CUtPlayerFrameWindow::CloseMedia()
{
	if (m_pMediaControl != NULL)
	{
		m_pMediaControl->Stop();
		m_pMediaControl->Release();
		m_pMediaControl = NULL;
	}

	return S_OK;
}
