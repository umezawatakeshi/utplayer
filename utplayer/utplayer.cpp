/* 文字コードはＳＪＩＳ 改行コードはＣＲＬＦ */
/* $Id: UL00Codec.cpp 890 2012-05-10 10:33:13Z umezawa $ */

// utplayer.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "utplayer.h"
#include "FrameWindow.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	CUtPlayerFrameWindow frame;
	MSG msg;

	frame.Create(NULL, CWindow::rcDefault, "Ut Alpha Video Player");
	frame.ShowWindow(nCmdShow);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}
