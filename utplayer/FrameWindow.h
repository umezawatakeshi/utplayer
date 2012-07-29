/* ï∂éöÉRÅ[ÉhÇÕÇrÇiÇhÇr â¸çsÉRÅ[ÉhÇÕÇbÇqÇkÇe */
/* $Id$ */

#pragma once

#include "resource.h"
#include "VideoRenderer.h"

class CUtPlayerFrameWindow : public CWindowImpl<CUtPlayerFrameWindow, CWindow, CFrameWinTraits>
{
private:
	IMediaControl *m_pMediaControl;
	IMediaSeeking *m_pMediaSeeking;
	CUtPlayerVideoRenderer *m_pVideoRenderer;

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
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_DROPFILES, OnDropFiles);
		COMMAND_ID_HANDLER(ID_FILE_OPEN, OnFileOpen);
		COMMAND_ID_HANDLER(ID_FILE_CLOSE, OnFileClose);
		COMMAND_ID_HANDLER(ID_FILE_EXIT, OnFileExit);
		COMMAND_RANGE_HANDLER(ID_VIEW_SIZE_50, ID_VIEW_SIZE_200, OnViewSize);
		COMMAND_RANGE_HANDLER(ID_VIEW_BACKGROUND_FIRST, ID_VIEW_BACKGROUND_LAST, OnViewBackground);
		COMMAND_ID_HANDLER(ID_PLAY_PLAYPAUSE, OnPlayPlayPause);
		COMMAND_ID_HANDLER(ID_PLAY_STOP, OnPlayStop);
		COMMAND_ID_HANDLER(ID_PLAY_REWIND, OnPlayRewind);
		COMMAND_ID_HANDLER(ID_PLAY_STEP_BACKWARD, OnPlayStepBackward);
		COMMAND_ID_HANDLER(ID_PLAY_STEP_FORWARD, OnPlayStepForward);
		COMMAND_RANGE_HANDLER(ID_PLAY_PLAYSPEED_50, ID_PLAY_PLAYSPEED_200, OnPlayPlaySpeed);
		COMMAND_ID_HANDLER(ID_HELP_ABOUT, OnHelpAbout);
	END_MSG_MAP()

private:
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnFileOpen(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnFileClose(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnFileExit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnViewSize(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnViewBackground(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnPlayPlayPause(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnPlayStop(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnPlayRewind(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnPlayStepBackward(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnPlayStepForward(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnPlayPlaySpeed(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnHelpAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

private:
	HRESULT OpenMediaFile(LPCSTR pszFile);
	HRESULT CloseMedia(void);
};
