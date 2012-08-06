/* ï∂éöÉRÅ[ÉhÇÕÇrÇiÇhÇr â¸çsÉRÅ[ÉhÇÕÇbÇqÇkÇe */
/* $Id$ */

#include "stdafx.h"
#include "Util.h"

LPTSTR FormatErrorAllocateBuffer(DWORD dwErrorCode)
{
	LPTSTR psz;
	DWORD ret;

	ret = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, dwErrorCode, LANG_USER_DEFAULT, (LPTSTR)&psz, 0, NULL);
	if (ret == 0)
		return NULL;
	else
		return psz;
}

LPTSTR FormatStringResourceAllocateBuffer(HINSTANCE hInstance, UINT uID, ...)
{
	va_list args;
	LPTSTR ret;

	va_start(args, uID);
	ret = FormatStringResourceAllocateBufferV(hInstance, uID, args);
	va_end(args);
	return ret;
}

LPTSTR FormatStringResourceAllocateBufferV(HINSTANCE hInstance, UINT uID, va_list args)
{
	TCHAR szString[1024];
	LPTSTR psz;
	DWORD ret;

	if (LoadString(hInstance, uID, szString, _countof(szString)) == 0)
		return NULL;
	ret = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_STRING,
		szString, 0, LANG_USER_DEFAULT, (LPTSTR)&psz, 0, &args);
	if (ret == 0)
		return NULL;
	else
		return psz;
}

int MessageBox(HWND hWnd, HINSTANCE hInstance, UINT uID, UINT uType, ...)
{
	va_list args;
	int ret;

	va_start(args, uType);
	ret = MessageBoxV(hWnd, hInstance, uID, uType, args);
	va_end(args);
	return ret;
}

int MessageBoxV(HWND hWnd, HINSTANCE hInstance, UINT uID, UINT uType, va_list args)
{
	LPTSTR psz;
	int ret;

	psz = FormatStringResourceAllocateBufferV(hInstance, uID, args);
	if (psz == NULL)
		return 0;
	ret = MessageBox(hWnd, psz, "", uType);
	va_end(args);
	LocalFree(psz);
	return ret;
}
