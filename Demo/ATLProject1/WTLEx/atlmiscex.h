//
// Windows Template Library Extensions
//
// This file is NOT part of Windows Template Library.
// The code and information is provided "as-is" without
// warranty of any kind, either expressed or implied.
//

#ifndef __ATLMISCEX_H__
#define __ATLMISCEX_H__

#pragma once

#ifndef __cplusplus
	#error ATL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLAPP_H__
	#error atlmiscex.h requires atlapp.h to be included first
#endif

#ifndef __ATLWIN_H__
	#error atlmiscex.h requires atlwin.h to be included first
#endif

///////////////////////////////////////////////////////////////////////////////
// CScrollInfo - Wrapper for Windows SCROLLINFO structure

class CScrollInfo : public tagSCROLLINFO
{
public:

	CScrollInfo()
	{
		memset(this, 0, sizeof(SCROLLINFO)); cbSize = sizeof(SCROLLINFO);
	}
	
	CScrollInfo(UINT _fMask)
	{
		memset(this, 0, sizeof(SCROLLINFO)); cbSize = sizeof(SCROLLINFO); fMask = _fMask;
	}

	int SetScrollInfo(HWND hwnd, int fnBar, BOOL fRedraw = TRUE)
	{
		return ::SetScrollInfo(hwnd, fnBar, this, fRedraw);
	}

};

///////////////////////////////////////////////////////////////////////////////
// CWindowPlacement  - Wrapper for Windows WINDOWPLACEMENT structure

class CWindowPlacement : public tagWINDOWPLACEMENT
{
public:
	
	CWindowPlacement(UINT _showCmd = SW_NORMAL)
	{
		length = sizeof(WINDOWPLACEMENT); 
		flags = 0;
		showCmd = _showCmd;
		ptMinPosition.x = -1; ptMinPosition.y = -1;
		ptMaxPosition.x = -1; ptMaxPosition.y = -1;
		::SetRectEmpty(&rcNormalPosition);
	}

	const CWindowPlacement& operator=(const WINDOWPLACEMENT& wp)
	{
		length = sizeof(WINDOWPLACEMENT); 
		flags = wp.flags;
		showCmd = wp.showCmd;
		ptMinPosition = wp.ptMinPosition;
		ptMaxPosition = wp.ptMaxPosition;
		rcNormalPosition = wp.rcNormalPosition;

		return *this;
	} 

};

///////////////////////////////////////////////////////////////////////////////
// CBmpDC - variation of Viksoe CMemDC, but without autoblitting on destruction

class CBmpDC : public CDC
{
public:

	// Attributes

	CBitmap m_bitmap;				// Offscreen bitmap
	CBitmapHandle m_hOldBitmap;		// Originally selected bitmap
	CRect m_rc;						// Rectangle of the drawing area

	// Construction/destruction

	CBmpDC(HDC hDC, const LPRECT prc = NULL, bool bSetWindowOrg = true)
	{
		ATLASSERT(hDC != NULL);

		// Prepare drawing rectangle
		
		if (prc != NULL) m_rc = *prc; 
		else ::GetClipBox(hDC, &m_rc);

		// Create offscreen dc

		CreateCompatibleDC(hDC);
		::LPtoDP(hDC, (LPPOINT)&m_rc, sizeof(RECT) / sizeof(POINT));
		m_bitmap.CreateCompatibleBitmap(hDC, m_rc.Width(), m_rc.Height());
		m_hOldBitmap = SelectBitmap(m_bitmap);
		::DPtoLP(hDC, (LPPOINT)&m_rc, sizeof(RECT) / sizeof(POINT));
		if (bSetWindowOrg) { SetWindowOrg(m_rc.left, m_rc.top); }
	}

	~CBmpDC()
	{
		// Swap back the original bitmap

		SelectBitmap(m_hOldBitmap);
	}

	// Operations

	operator HDC() const { return m_hDC; }

	void BitBlt(CDCHandle dc)
	{
		// Copy offscreen bitmap onto the destination DC

		dc.BitBlt(m_rc.left, m_rc.top, m_rc.Width(), m_rc.Height(),
			m_hDC, m_rc.left, m_rc.top, SRCCOPY);
	}

	void BitBlt(CDCHandle dc, int x, int y)
	{
		// Copy offscreen bitmap onto the destination DC at the specified position

		dc.BitBlt(x, y, m_rc.Width(), m_rc.Height(),
			m_hDC, m_rc.left, m_rc.top, SRCCOPY);
	}

	void BitBlt(CDCHandle dc, const CPoint& pt)
	{
		// Copy offscreen bitmap onto the destination DC at the specified position

		dc.BitBlt(pt.x, pt.y, m_rc.Width(), m_rc.Height(),
			m_hDC, m_rc.left, m_rc.top, SRCCOPY);
	}

	void BitBlt(CDCHandle dc, const CRect& rc)
	{
		// Copy part of offscreen bitmap onto the destination DC

		dc.BitBlt(rc.left, rc.top, rc.Width(), rc.Height(),
			m_hDC, rc.left, rc.top, SRCCOPY);
	}

	void BitBlt(CDCHandle dc, const CRect& rc, int xSrc, int ySrc)
	{
		// Copy part of offscreen bitmap onto the destination DC

		dc.BitBlt(rc.left, rc.top, rc.Width(), rc.Height(),
			m_hDC, xSrc, ySrc, SRCCOPY);
	}

	void BitBlt(CDCHandle dc, const CRect& rc, const CPoint& ptSrc)
	{
		// Copy part of offscreen bitmap onto the destination DC

		dc.BitBlt(rc.left, rc.top, rc.Width(), rc.Height(),
			m_hDC, ptSrc.x, ptSrc.y, SRCCOPY);
	}

#ifndef _WIN32_WCE

	void AlphaBlend(CDCHandle dc, const CRect& rc, const BLENDFUNCTION& bf)
	{
		// Alpha blend the offscreen bitmap onto the destination DC
		
		dc.AlphaBlend(rc.left, rc.top, rc.Width(), rc.Height(),
			m_hDC, m_rc.left, m_rc.top, m_rc.Width(), m_rc.Height(), bf);
	}

#endif

};

///////////////////////////////////////////////////////////////////////////////
// Bitmap info header wraooer

class CBitmapInfoHeader : public BITMAPINFOHEADER
{
public:

	CBitmapInfoHeader(int cx, int cy, UINT bitCount = 32, UINT compression = BI_RGB)
	{ 
		memset(this, 0, sizeof(BITMAPINFOHEADER)); 
		biSize          = sizeof(BITMAPINFOHEADER);
		biWidth         = (LONG)cx;
		biHeight        = (LONG)cy;
		biPlanes        = 1;
		biBitCount      = (WORD)bitCount;
		biCompression   = (DWORD)compression;
	}

	CBitmapInfoHeader(const CSize& size, UINT bitCount = 32, UINT compression = BI_RGB)
	{ 
		memset(this, 0, sizeof(BITMAPINFOHEADER)); 
		biSize          = sizeof(BITMAPINFOHEADER);
		biWidth         = (LONG)size.cx;
		biHeight        = (LONG)size.cy;
		biPlanes        = 1;
		biBitCount      = (WORD)bitCount;
		biCompression   = (DWORD)compression;
	}

};

///////////////////////////////////////////////////////////////////////////////
// CTriVertex - Wrapper for Windows TRIVERTEX structure

#ifndef GetAValue
#define GetAValue(rgb) ((BYTE)((rgb)>>24))
#endif

class CTriVertex : public TRIVERTEX
{
public:

	CTriVertex(LONG X = 0, LONG Y = 0, COLORREF rgb = 0)
	{
		x = X;
		y = Y;
		SetColor(rgb);
	}

	CTriVertex(const POINT& pt, COLORREF rgb = 0)
	{
		x = pt.x;
		y = pt.y;
		SetColor(rgb);
	}

	void SetColor(COLORREF rgb)
	{
		Red   = (COLOR16)(GetRValue(rgb) << 8);
		Green = (COLOR16)(GetGValue(rgb) << 8);
		Blue  = (COLOR16)(GetBValue(rgb) << 8);
		Alpha = (COLOR16)(GetAValue(rgb) << 8);
	}

	CTriVertex& operator=(COLORREF rgb)
	{
		SetColor(rgb);
		return *this;
	}

	CTriVertex& operator=(const POINT& pt)
	{
		x = pt.x;
		y = pt.y;
		return *this;
	}

};

///////////////////////////////////////////////////////////////////////////////
// CGradientRect - Wrapper for Windows GRADIENT_RECT structure.

class CGradientRect : public GRADIENT_RECT
{
public:

	CGradientRect(ULONG uUpperLeftIndex, ULONG uBottomRightIndex)
	{
		UpperLeft = uUpperLeftIndex;
		LowerRight = uBottomRightIndex;
	}

};

#if !defined(_WIN32_WCE) || _WIN32_WCE >= 0x500

//////////////////////////////////////////////////////////////////////////
// CBlendFunction - Wrapper for Windows BLENDFUNCTION structure

class CBlendFunction : public BLENDFUNCTION
{
public:

	CBlendFunction(BYTE byConstAlpha = 255, bool bPerPixelAlpha = false)
	{
		BlendOp = AC_SRC_OVER; BlendFlags = 0;
		AlphaFormat = (BYTE)((bPerPixelAlpha)? AC_SRC_ALPHA: 0);
		SourceConstantAlpha = byConstAlpha;
	}

};

#endif

///////////////////////////////////////////////////////////////////////////////
// Rectangle helpers

static inline void SetRect(CRect& rc, int x, int y, const CSize& size)
{
	rc.left = x; rc.right = x + size.cx;
	rc.top = y; rc.bottom = y + size.cy;
}

static inline void SetRect(CRect& rc, const CPoint& pt, int x, int y)
{
	rc.left = pt.x; rc.right = x;
	rc.top = pt.y; rc.bottom = y;
	rc.NormalizeRect();
}

static inline void SetRect(CRect& rc, const CPoint& pt, const CPoint& pt2)
{
	rc.left = pt.x; rc.right = pt2.x;
	rc.top = pt.y; rc.bottom = pt2.y;
	rc.NormalizeRect();
}

static inline int HCenter(const CRect& rc, int width = 0)
{
	if (rc.Width() > width) {
		return rc.left + (rc.Width() / 2 - width / 2);
	} else {
		return rc.left;
	}
}

static inline int VCenter(const CRect& rc, int height = 0)
{
	if (rc.Height() > height) {
		return rc.top + (rc.Height() / 2 - height / 2);
	} else {
		return rc.top;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Image rectangle helpers

static inline void CopyImageRect(HDC hdc, CRect& rc, HDC hdcSrc, int xSrc = 0, int ySrc = 0)
{
	::BitBlt(hdc, rc.left, rc.top, rc.Width(), rc.Height(), hdcSrc, xSrc, ySrc, SRCCOPY);
}

static inline void CopyImageRect(HDC hdc, CRect& rc, HDC hdcSrc, CPoint& ptSrc)
{
	::BitBlt(hdc, rc.left, rc.top, rc.Width(), rc.Height(), hdcSrc, ptSrc.x, ptSrc.y, SRCCOPY);
}

static inline void CopyImageRect(HDC hdc, HDC hdcSrc, CRect& rc)
{
	::BitBlt(hdc, 0, 0, rc.Width(), rc.Height(), hdcSrc, rc.left, rc.top, SRCCOPY);
}

static inline void CopyImageRect(HDC hdc, HDC hdcSrc, int cx, int cy)
{
	::BitBlt(hdc, 0, 0, cx, cy, hdcSrc, 0, 0, SRCCOPY);
}

static inline void BlendImageRect(HDC hdc, CRect& rc, HDC hdcSrc, BYTE alpha, int xSrc = 0, int ySrc = 0)
{
	::AlphaBlend(hdc, rc.left, rc.top, rc.Width(), rc.Height(), hdcSrc, xSrc, ySrc, rc.Width(), rc.Height(), CBlendFunction(alpha));
}

static inline void BlendImageRect(HDC hdc, CRect& rc, HDC hdcSrc, BYTE alpha, CPoint& ptSrc)
{
	::AlphaBlend(hdc, rc.left, rc.top, rc.Width(), rc.Height(), hdcSrc, ptSrc.x, ptSrc.y, rc.Width(), rc.Height(), CBlendFunction(alpha));
}

static inline void BlendImageRect(HDC hdc, HDC hdcSrc, CRect& rc, BYTE alpha)
{
	::AlphaBlend(hdc, 0, 0, rc.Width(), rc.Height(), hdcSrc, rc.left, rc.top, rc.Width(), rc.Height(), CBlendFunction(alpha));
}

static inline void BlendImageRect(HDC hdc, HDC hdcSrc, int cx, int cy, BYTE alpha)
{
	::AlphaBlend(hdc, 0, 0, cx, cy, hdcSrc, 0, 0, cx, cy, CBlendFunction(alpha));
}

///////////////////////////////////////////////////////////////////////////////
// Color manipulation helpers

inline COLORREF AdjustColor(COLORREF clr, int rDelta, int gDelta, int bDelta)
{
	int r = GetRValue(clr) + rDelta;
	int g = GetGValue(clr) + gDelta;
	int b = GetBValue(clr) + bDelta;

	if (r < 0) r = 0; else if (r > 255) r = 255;
	if (g < 0) g = 0; else if (g > 255) g = 255;
	if (b < 0) b = 0; else if (b > 255) b = 255;

	return RGB(r, g, b);
}

inline COLORREF AdjustColor(COLORREF clr, int delta)
{
	return AdjustColor(clr, delta, delta, delta);
}

///////////////////////////////////////////////////////////////////////////////
// Miscellaneous helper routines

static inline bool TrimString(CString& str)
{
	str.TrimLeft(); str.TrimRight();

	return !str.IsEmpty();
}

static inline CString GetWindowClassName(HWND hwnd)
{
	CString name;

	static UINT cch = 256;
	LPTSTR psz = name.GetBufferSetLength(cch);
	if (psz != NULL) {
		::GetClassName(hwnd, psz, cch);
		name.ReleaseBuffer();
	}
	
	return name;
}

static inline bool HasStyle(HWND hwnd, DWORD style)
{
	return ::GetWindowLong(hwnd, GWL_STYLE) & style ? true : false;
}

#endif // __ATLMISCEX_H__
