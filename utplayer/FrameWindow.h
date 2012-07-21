/* ï∂éöÉRÅ[ÉhÇÕÇrÇiÇhÇr â¸çsÉRÅ[ÉhÇÕÇbÇqÇkÇe */
/* $Id$ */

#pragma once

#include "resource.h"

class CUtPlayerFrameWindow : public CWindowImpl<CUtPlayerFrameWindow, CWindow, CFrameWinTraits>
{
public:
	CUtPlayerFrameWindow(void);
	~CUtPlayerFrameWindow(void);

public:
	static CWndClassInfo &GetWndClassInfo()
	{
		static CWndClassInfo wc =
		{
			{
				sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
				StartWindowProc, 0, 0, NULL, NULL, NULL, NULL,
				MAKEINTRESOURCE(IDC_UTPLAYER), "UtPlayerFrameWindow", NULL
			},
			NULL, NULL, IDC_ARROW, TRUE, 0, ""
		};
		return wc;
	}

private:
	BEGIN_MSG_MAP(CMyWindow)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
	END_MSG_MAP()

private:
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};
