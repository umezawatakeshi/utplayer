/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id$ */

#pragma once

class CUtPlayerVideoRenderer : public CBaseRenderer
{
private:
	const HWND m_hWnd;

public:
	CUtPlayerVideoRenderer(HWND hWnd, HRESULT *phr);
	~CUtPlayerVideoRenderer(void);

public:
	virtual HRESULT CheckMediaType(const CMediaType *pmt);
	virtual HRESULT DoRenderSample(IMediaSample *pMediaSample);
};
