/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id$ */

#pragma once

LPTSTR FormatErrorAllocateBuffer(DWORD dwErrorCode);
LPTSTR FormatStringResourceAllocateBuffer(HINSTANCE hInstance, UINT uID, ...);
LPTSTR FormatStringResourceAllocateBufferV(HINSTANCE hInstance, UINT uID, va_list args);

int MessageBox(HWND hWnd, HINSTANCE hInstance, UINT uID, UINT uType, ...);
int MessageBoxV(HWND hWnd, HINSTANCE hInstance, UINT uID, UINT uType, va_list args);
