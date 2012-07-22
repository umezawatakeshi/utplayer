/* •¶šƒR[ƒh‚Í‚r‚i‚h‚r ‰üsƒR[ƒh‚Í‚b‚q‚k‚e */
/* $Id$ */

#pragma once

class CUtPlayerVideoRenderer : public CBaseRenderer
{
private:
	const HWND m_hWnd;
	BYTE *m_pDIB;
	LONG m_nWidth;
	LONG m_nHeight;
	BITMAPINFOHEADER m_bmih;

public:
	CUtPlayerVideoRenderer(HWND hWnd, HRESULT *phr);
	~CUtPlayerVideoRenderer(void);

public:
	const void *GetDIBits(void) { return m_pDIB; }
	LONG GetWidth(void) { return m_nWidth; }
	LONG GetHeight(void) { return m_nHeight; }
	const BITMAPINFOHEADER *GetBitmapInfoHeader(void) { return &m_bmih; }

public:
	virtual HRESULT CompleteConnect(IPin *pReceivePin);
	virtual HRESULT CheckMediaType(const CMediaType *pmt);
	virtual HRESULT DoRenderSample(IMediaSample *pMediaSample);
};
