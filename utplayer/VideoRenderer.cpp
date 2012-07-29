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
	m_pDIB = NULL;
}

CUtPlayerVideoRenderer::~CUtPlayerVideoRenderer(void)
{
	if (m_pDIB != NULL)
		free(m_pDIB);
}

HRESULT CUtPlayerVideoRenderer::CompleteConnect(IPin *pReceivePin)
{
	HRESULT hr;
	AM_MEDIA_TYPE mt;
	VIDEOINFOHEADER *pvih;

	hr = CBaseRenderer::CompleteConnect(pReceivePin);
	if (FAILED(hr))
		return hr;

	free(m_pDIB);
	m_pDIB = NULL;

	this->m_pInputPin->ConnectionMediaType(&mt);
	pvih = (VIDEOINFOHEADER *)mt.pbFormat;
	m_nWidth = pvih->bmiHeader.biWidth;
	m_nHeight = pvih->bmiHeader.biHeight;
	m_bmih = pvih->bmiHeader;
	FreeMediaType(mt);

	m_pDIB = (BYTE *)malloc(m_nWidth * m_nHeight * 4);
	memset(m_pDIB, 0, m_nWidth * m_nHeight * 4);

	return S_OK;
}

void CUtPlayerVideoRenderer::OnReceiveFirstSample(IMediaSample *pMediaSample)
{
	DoRenderSample(pMediaSample);
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
	HDC hdc;
	LONG lActualDataLength;
	RECT rc;

	pMediaSample->GetPointer(&pBuf);
	lActualDataLength = pMediaSample->GetActualDataLength();
	for (long i = 0; i < lActualDataLength; i += 4)
	{
		m_pDIB[i + 0] = pBuf[i + 0] * pBuf[i + 3] / 255;
		m_pDIB[i + 1] = pBuf[i + 1] * pBuf[i + 3] / 255;
		m_pDIB[i + 2] = pBuf[i + 2] * pBuf[i + 3] / 255;
		m_pDIB[i + 3] = pBuf[i + 3];
	}

	//InvalidateRect(m_hWnd, NULL, FALSE);
	hdc = GetDC(m_hWnd);
	GetClientRect(m_hWnd, &rc);
	SetStretchBltMode(hdc, COLORONCOLOR);
	StretchDIBits(hdc, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
		0, 0, m_nWidth, m_nHeight, m_pDIB, (BITMAPINFO *)&m_bmih, DIB_RGB_COLORS, SRCCOPY);
	ReleaseDC(m_hWnd, hdc);

	return S_OK;
}
