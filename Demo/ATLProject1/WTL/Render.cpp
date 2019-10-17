///////////////////////////////////////////////////////////////////////////////
//
// Render.cpp : Implementation for CRender and related classes
//
// Copyright 2009 Evernote Corp. All rights reserved.
// 
// History: 11/25/2009 kvitekp - Initially created.
//
// $Id$
//
///////////////////////////////////////////////////////////////////////////////

// Include additional headers

#include "Render.h"
#include "DCCache.h"
#include <atlmiscex.h>
#include "atlgdiraii.h"

///////////////////////////////////////////////////////////////////////////////
// CRender class
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Generic dc to dc rendering with aplha channel support

bool CRender::Image(HDC dc, int x, int y, int cx, int cy, HDC dcSrc, int xSrc, int ySrc, int cxSrc, int cySrc)
{
	// Prepare bitmap info header

	CBitmapInfoHeader bmi(cx, cy);

	// Create source DIB section in shared memory and check if ok

	CBitmap src; BYTE* pSrcBits = NULL;
	if (!src.CreateDIBSection(dcSrc, (BITMAPINFO*)&bmi, DIB_RGB_COLORS, (void**)&pSrcBits, NULL, 0))
		return false;

	// Create destination DIB section in shared memory and check if ok

	CBitmap dst; BYTE* pDstBits = NULL;
	if (!dst.CreateDIBSection(dc, (BITMAPINFO*)&bmi, DIB_RGB_COLORS, (void**)&pDstBits, NULL, 0))
		return false;

	// Copy source and destination bitmaps onto our DIBSections, so we can get access to 
	// their bits using the BYTE pointers we passed into CreateDIBSection

	{	CCachedDC tmp;

		CSelectBitmap selectBitmap(tmp, src);

		if (!tmp.StretchBlt(0, 0, cx, cy, dcSrc, xSrc, ySrc, cxSrc, cySrc, SRCCOPY))
			return false;

		tmp.SelectBitmap(dst);

		if (!tmp.StretchBlt(0, 0, cx, cy, dc, x, y, cx, cy, SRCCOPY))
			return false;
	}

	// Alpha blend bitmaps onto source DIB section surface

	for (int j = 0; j < cy; ++j) {

		LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[j * cx];
		LPBYTE pbDstRGB = (LPBYTE)&((DWORD*)pDstBits)[j * cx];

		for (int i = 0; i < cx; ++i) {

			int alpha = pbSrcRGB[3];

			pbSrcRGB[0] = (BYTE)((pbDstRGB[0] * (255 - alpha) + pbSrcRGB[0] * alpha) >> 8);
			pbSrcRGB[1] = (BYTE)((pbDstRGB[1] * (255 - alpha) + pbSrcRGB[1] * alpha) >> 8);
			pbSrcRGB[2] = (BYTE)((pbDstRGB[2] * (255 - alpha) + pbSrcRGB[2] * alpha) >> 8);

			pbSrcRGB+= sizeof(DWORD);
			pbDstRGB+= sizeof(DWORD);
		}
	}

	// Copy source DIB section surface onto destination dc

	{	CCachedDC tmp;

		CSelectBitmap selectBitmap(tmp, src);

		if (!::BitBlt(dc, x, y, cx, cy, tmp, 0, 0, SRCCOPY))
			return false;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////
// ATL ATL::CImage to dc rendering

bool CRender::Image(HDC hdc, int x, int y, int cx, int cy, ATL::CImage& imageSrc, int xSrc, int ySrc, int cxSrc, int cySrc)
{

	// Render alpha channel image

	if (imageSrc.GetBPP() == 32) {
		CSafeImageDC hdcSrc(imageSrc);
		return Image(hdc, x, y, cx, cy, hdcSrc, xSrc, ySrc, cxSrc, cySrc);
	}

	// Otherwise use stock rendering method

	return !!imageSrc.Draw(hdc, x, y, cx, cy, xSrc, ySrc, cxSrc, cySrc);
}
#define VERIFYRET(x,y) x;y;
bool CRender::Slice(CDCHandle dc, int x, int y, int cx, int cy, ATL::CImage& imageSrc, int cut_l, int cut_t, int cut_r, int cut_b)
{
	// Sanity check

	VERIFYRET(dc != NULL, false);
	VERIFYRET(!imageSrc.IsNull(), false);
	VERIFYRET(imageSrc.GetBPP() == 32, false);
	VERIFYRET(cut_l + cut_r <= cx, false);
	VERIFYRET(cut_t + cut_b <= cy, false);
	const int img_w = imageSrc.GetWidth();
	const int img_h = imageSrc.GetHeight();
	VERIFYRET(cut_l + cut_r <= img_w, false);
	VERIFYRET(cut_t + cut_b <= img_h, false);

	// Prepare bitmap header info

	BITMAPINFOHEADER bmi = {};
	bmi.biSize			= sizeof(BITMAPINFOHEADER);
	bmi.biWidth			= cx;
	bmi.biHeight		= cy;
	bmi.biPlanes		= 1;
	bmi.biBitCount		= 32;
	bmi.biCompression	= BI_RGB;

	// Create source DIB section in shared memory and check if ok

	CBitmap srcBmp;
	BYTE* pSrcBits = NULL;
	const CSafeImageDC hdcSrc(imageSrc);
	if (!srcBmp.CreateDIBSection(hdcSrc, reinterpret_cast<BITMAPINFO*>(&bmi), DIB_RGB_COLORS, (void**)&pSrcBits, NULL, 0))
		return false;

	// Create destination DIB section in shared memory and check if ok

	CBitmap dstBmp;
	BYTE* pDstBits = NULL;
	if (!dstBmp.CreateDIBSection(dc, reinterpret_cast<BITMAPINFO*>(&bmi), DIB_RGB_COLORS, (void**)&pDstBits, NULL, 0))
		return false;

	// Copy source and destination bitmaps onto our DIBSections, so we can get access to 
	// their bits using the BYTE pointers we passed into CreateDIBSection

	{
		CCachedDC tmp;
		const CSelectBitmap selectBitmap(tmp, srcBmp);

		// Blt corners

		if (cut_l > 0 && cut_t > 0 && !tmp.BitBlt(0, 0, cut_l, cut_t, hdcSrc, 0, 0, SRCCOPY)) // TopLeft
			return false;
		if (cut_r > 0 && cut_t > 0 && !tmp.BitBlt(cx - cut_r, 0, cut_r, cut_t, hdcSrc, img_w - cut_r, 0, SRCCOPY)) // TopRight
			return false;
		if (cut_l > 0 && cut_b > 0 && !tmp.BitBlt(0, cy - cut_b, cut_l, cut_b, hdcSrc, 0, img_h - cut_b, SRCCOPY)) // BottomLeft
			return false;
		if (cut_r > 0 && cut_b > 0 && !tmp.BitBlt(cx - cut_r, cy - cut_b, cut_r, cut_b, hdcSrc, img_w - cut_r, img_h - cut_b, SRCCOPY)) // BottomRight
			return false;

		// Blt edges

		const int cut_w = cut_l + cut_r;
		const int cut_h = cut_t + cut_b;
		if (cut_l > 0 && !tmp.StretchBlt(0, cut_t, cut_l, cy - cut_h, hdcSrc, 0, cut_t, cut_l, img_h - cut_h, SRCCOPY)) // Left
			return false;
		if (cut_t > 0 && !tmp.StretchBlt(cut_l, 0, cx - cut_w, cut_t, hdcSrc, cut_l, 0, img_w - cut_w, cut_t, SRCCOPY)) // Top
			return false;
		if (cut_r > 0 && !tmp.StretchBlt(cx - cut_r, cut_t, cut_r, cy - cut_h, hdcSrc, img_w - cut_r, cut_t, cut_r, img_h - cut_h, SRCCOPY)) // Right
			return false;
		if (cut_b > 0 && !tmp.StretchBlt(cut_l, cy - cut_b, cx - cut_w, cut_b, hdcSrc, cut_l, img_h - cut_b, img_w - cut_w, cut_b, SRCCOPY)) // Bottom
			return false;

		// Blt center

		if (!tmp.StretchBlt(cut_l, cut_t, cx - cut_w, cy - cut_h, hdcSrc, cut_l, cut_t, img_w - cut_w, img_h - cut_h, SRCCOPY)) // Bottom
			return false;
	}

	{
		CCachedDC tmp;
		const CSelectBitmap selectBitmap(tmp, dstBmp);
		if (!tmp.StretchBlt(0, 0, cx, cy, dc, x, y, cx, cy, SRCCOPY))
			return false;
	}

	// Alpha blend bitmaps onto source DIB section surface

	for (int j = 0; j < cy; ++j) {

		LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[j * cx];
		LPBYTE pbDstRGB = (LPBYTE)&((DWORD*)pDstBits)[j * cx];

		for (int i = 0; i < cx; ++i) {

			const int alpha = pbSrcRGB[3];

			pbSrcRGB[0] = (BYTE)((pbDstRGB[0] * (255 - alpha) + pbSrcRGB[0] * alpha) >> 8);
			pbSrcRGB[1] = (BYTE)((pbDstRGB[1] * (255 - alpha) + pbSrcRGB[1] * alpha) >> 8);
			pbSrcRGB[2] = (BYTE)((pbDstRGB[2] * (255 - alpha) + pbSrcRGB[2] * alpha) >> 8);

			pbSrcRGB += sizeof(DWORD);
			pbDstRGB += sizeof(DWORD);
		}
	}

	// Copy source DIB section surface onto destination dc

	CCachedDC tmp;
	const CSelectBitmap selectBitmap(tmp, srcBmp);
	return !!dc.BitBlt(x, y, cx, cy, tmp, 0, 0, SRCCOPY);
}

///////////////////////////////////////////////////////////////////////////////
// End of Render.cpp
///////////////////////////////////////////////////////////////////////////////
