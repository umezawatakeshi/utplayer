/* ï∂éöÉRÅ[ÉhÇÕÇrÇiÇhÇr â¸çsÉRÅ[ÉhÇÕÇbÇqÇkÇe */
/* $Id: UL00Codec.cpp 890 2012-05-10 10:33:13Z umezawa $ */

#pragma once

class CUtPlayerFrameWindow : public CWindowImpl<CUtPlayerFrameWindow, CWindow, CFrameWinTraits>
{
public:
	CUtPlayerFrameWindow(void);
	~CUtPlayerFrameWindow(void);

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
