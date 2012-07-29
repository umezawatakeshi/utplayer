/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id$ */

// utplayer.cpp : �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "utplayer.h"
#include "FrameWindow.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	CUtPlayerFrameWindow frame;
	MSG msg;
	HACCEL hAccel;

	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_UTPLAYER));

	frame.Create(NULL, CWindow::rcDefault, "Ut Alpha Video Player");
	frame.ShowWindow(nCmdShow);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(frame.m_hWnd, hAccel, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}
