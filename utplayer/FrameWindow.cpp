/* ï∂éöÉRÅ[ÉhÇÕÇrÇiÇhÇr â¸çsÉRÅ[ÉhÇÕÇbÇqÇkÇe */
/* $Id$ */

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
	HRGN hrgn = CreateRectRgn(-1, -1, 0, 0); // ÉNÉâÉCÉAÉìÉgóÃàÊëSëÃÇ™ìßñæ

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

	MessageBox(szFile);
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
