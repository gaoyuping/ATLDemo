///////////////////////////////////////////////////////////////////////////////
//
// Render.h: Interface for CRender and related classes
//
// Copyright 2009 Evernote Corp. All rights reserved.
// 
// History: 11/25/2009 kvitekp - Initially created.
//
// $Id$
//
///////////////////////////////////////////////////////////////////////////////

#ifndef AFX_RENDER_H_INCLUDED
#define AFX_RENDER_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Include additional headers
#include <atlbase.h>
#include <atlapp.h>
#include <atlimage.h>
#include <atlgdi.h>

///////////////////////////////////////////////////////////////////////////////
// CRender class
///////////////////////////////////////////////////////////////////////////////
class CSafeImageDC {
    ATL::CImage& m_image;
    HDC m_hdc;

public:

    CSafeImageDC(ATL::CImage& image)
        : m_image(image) {
        m_hdc = m_image.GetDC();
    }

    ~CSafeImageDC() {
        m_image.ReleaseDC();
    }

    operator HDC() const { return m_hdc; }

};

class CRender
{
public:

	// Generic dc to dc rendering with aplha channel support

	static bool Image(HDC dc, int x, int y, int cx, int cy, HDC dcSrc, int xSrc, int ySrc, int cxSrc, int cySrc);

	static bool Image(HDC dc, const CRect& rc, HDC dcSrc, const CRect& rcSrc)
	{
		return Image(dc, rc.left, rc.top, rc.Width(), rc.Height(), 
			dcSrc, rcSrc.left, rcSrc.top, rcSrc.Width(), rcSrc.Height()); 
	}

	static bool Image(HDC dc, int x, int y, HDC dcSrc, int xSrc, int ySrc, int cxSrc, int cySrc)
	{
		return Image(dc, x, y, cxSrc, cySrc, 
			dcSrc, xSrc, ySrc, cxSrc, cySrc); 
	}

	static bool Image(HDC dc, int x, int y, HDC dcSrc, const CRect& rcSrc)
	{
		return Image(dc, x, y, rcSrc.Width(), rcSrc.Height(), 
			dcSrc, rcSrc.left, rcSrc.top, rcSrc.Width(), rcSrc.Height()); 
	}

	// ATL ATL::CImage to dc rendering

	static bool Image(HDC hdc, int x, int y, int cx, int cy, ATL::CImage& imageSrc, int xSrc, int ySrc, int cxSrc, int cySrc);

	static bool Image(HDC dc, const CRect& rc, ATL::CImage& imageSrc, int xSrc, int ySrc, int cxSrc, int cySrc)
	{
		return Image(dc, rc.left, rc.top, rc.Width(), rc.Height(), 
			imageSrc, xSrc, ySrc, cxSrc, cySrc); 
	}

	static bool Image(HDC dc, const CRect& rc, ATL::CImage& imageSrc)
	{
		return Image(dc, rc.left, rc.top, rc.Width(), rc.Height(), 
			imageSrc, 0, 0, imageSrc.GetWidth(), imageSrc.GetHeight()); 
	}

	static bool Image(HDC dc, const CRect& rc, ATL::CImage& imageSrc, const CRect& rcSrc)
	{
		return Image(dc, rc.left, rc.top, rc.Width(), rc.Height(), 
			imageSrc, rcSrc.left, rcSrc.top, rcSrc.Width(), rcSrc.Height()); 
	}

	static bool Image(HDC dc, int x, int y, ATL::CImage& imageSrc, const CRect& rcSrc)
	{
		return Image(dc, x, y, rcSrc.Width(), rcSrc.Height(), 
			imageSrc, rcSrc.left, rcSrc.top, rcSrc.Width(), rcSrc.Height()); 
	}

	static bool Image(HDC dc, int x, int y, ATL::CImage& imageSrc, int xSrc, int ySrc, int cxSrc, int cySrc)
	{
		return Image(dc, x, y, cxSrc, cySrc, 
			imageSrc, xSrc, ySrc, cxSrc, cySrc); 
	}

	static bool Image(HDC dc, int x, int y, ATL::CImage& imageSrc)
	{
		return Image(dc, x, y, imageSrc.GetWidth(), imageSrc.GetHeight(), 
			imageSrc, 0, 0, imageSrc.GetWidth(), imageSrc.GetHeight()); 
	}

	static bool Slice(CDCHandle dc, int x, int y, int cx, int cy, ATL::CImage& imageSrc, int cut_l = 0, int cut_t = 0, int cut_r = 0, int cut_b = 0);

	static bool Slice(CDCHandle dc, const CRect rc, ATL::CImage& imageSrc, int cut_l = 0, int cut_t = 0, int cut_r = 0, int cut_b = 0)
	{
		return Slice(dc, rc.left, rc.top, rc.Width(), rc.Height(), imageSrc, cut_l, cut_t, cut_r, cut_b);
	}

};

#endif // #ifndef AFX_RENDER_H_INCLUDED

///////////////////////////////////////////////////////////////////////////////
// End of Render.h
///////////////////////////////////////////////////////////////////////////////
