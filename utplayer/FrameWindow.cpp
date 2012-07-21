/* 文字コードはＳＪＩＳ 改行コードはＣＲＬＦ */
/* $Id: UL00Codec.cpp 890 2012-05-10 10:33:13Z umezawa $ */

#include "stdafx.h"
#include "FrameWindow.h"

CUtPlayerFrameWindow::CUtPlayerFrameWindow(void)
{
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
