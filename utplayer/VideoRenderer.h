/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
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
	virtual HRESULT CompleteConnect(IPin *pReceivePin);
	virtual HRESULT CheckMediaType(const CMediaType *pmt);
	virtual HRESULT DoRenderSample(IMediaSample *pMediaSample);
};
