/* •¶šƒR[ƒh‚Í‚r‚i‚h‚r ‰üsƒR[ƒh‚Í‚b‚q‚k‚e */
/* $Id$ */

#include "stdafx.h"
#include "VideoRenderer.h"

#define INITGUID
#include <guiddef.h>
// {C7016167-796F-4459-876E-EA2CB1C5D52F}
DEFINE_GUID(CLSID_UtPlayerVideoRenderer, 0xc7016167, 0x796f, 0x4459, 0x87, 0x6e, 0xea, 0x2c, 0xb1, 0xc5, 0xd5, 0x2f);

CUtPlayerVideoRenderer::CUtPlayerVideoRenderer(HWND hWnd, HRESULT *phr) : CBaseRenderer(CLSID_UtPlayerVideoRenderer, "CUtPlayerVideoRenderer", NULL, phr), m_hWnd(hWnd)
{
}

CUtPlayerVideoRenderer::~CUtPlayerVideoRenderer(void)
{
}

HRESULT CUtPlayerVideoRenderer::CheckMediaType(const CMediaType *pmt)
{
	if (!IsEqualGUID(*pmt->Type(), MEDIATYPE_Video) || !IsEqualGUID(*pmt->FormatType(), FORMAT_VideoInfo))
		return S_FALSE;

	if (IsEqualGUID(*pmt->Subtype(), MEDIASUBTYPE_RGB32) || IsEqualGUID(*pmt->Subtype(), MEDIASUBTYPE_ARGB32))
		return S_OK;
	else
		return S_FALSE;
}

HRESULT CUtPlayerVideoRenderer::DoRenderSample(IMediaSample *pMediaSample)
{
	BYTE *pBuf;
	BYTE *pDIB;
	AM_MEDIA_TYPE mt;
	VIDEOINFOHEADER *pvih;
	HBITMAP hbm;
	HBITMAP hbmOld;
	HDC hdc;
	HDC hdcBitmap;
	LONG lActualDataLength;

	pMediaSample->GetPointer(&pBuf);
	//pMediaSample->GetMediaType(&pmt); // ‚È‚º‚© SEGV ‚·‚é‚±‚Æ‚ª‚ ‚éB
	this->m_pInputPin->ConnectionMediaType(&mt);
	pvih = (VIDEOINFOHEADER *)mt.pbFormat;
	hdc = GetDC(m_hWnd);
	hbm = CreateDIBSection(hdc, (BITMAPINFO *)&pvih->bmiHeader, DIB_RGB_COLORS, (void **)&pDIB, NULL, 0);
	ReleaseDC(m_hWnd, hdc);
	lActualDataLength = pMediaSample->GetActualDataLength();
	for (long i = 0; i < lActualDataLength; i += 4)
	{
		pDIB[i + 0] = pBuf[i + 0] * pBuf[i + 3] / 255;
		pDIB[i + 1] = pBuf[i + 1] * pBuf[i + 3] / 255;
		pDIB[i + 2] = pBuf[i + 2] * pBuf[i + 3] / 255;
		pDIB[i + 3] = pBuf[i + 3];
	}

	//InvalidateRect(m_hWnd, NULL, FALSE);
	hdc = GetDC(m_hWnd);
	hdcBitmap = CreateCompatibleDC(hdc);
	hbmOld = (HBITMAP)SelectObject(hdcBitmap, hbm);
	BitBlt(hdc, 0, 0, pvih->bmiHeader.biWidth, pvih->bmiHeader.biHeight, hdcBitmap, 0, 0, SRCCOPY);
	SelectObject(hdcBitmap, hbmOld);
	DeleteDC(hdcBitmap);
	ReleaseDC(m_hWnd, hdc);

	DeleteObject(hbm);

	FreeMediaType(mt);

	_RPT0(_CRT_WARN, "hoge\n");
	return S_OK;
}
