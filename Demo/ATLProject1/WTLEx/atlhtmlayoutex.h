//
// Windows Template Library Extension for
// Lightweight Embeddable HTMLayout control
// by Andrew Fedoniouk of Terra Informatica
//
// Written by Pete Kvitek <pete@kvitek.com>
//
// This file is NOT part of Windows Template Library.
// The code and information is provided "as-is" without
// warranty of any kind, either expressed or implied.
//

#ifndef __ATLHTMLAYOUTEX_H__
#define __ATLHTMLAYOUTEX_H__

#pragma once

#ifndef __cplusplus
	#error ATL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLAPP_H__
	#error atlhtmlayoutex.h requires atlapp.h to be included first
#endif

#ifndef __ATLWIN_H__
	#error atlhtmlayoutex.h requires atlwin.h to be included first
#endif

#include <htmlayoutex.h>

/////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CHTMLayoutDibAPI
// CDibexT<t_bManaged>
//

namespace WTL
{

/////////////////////////////////////////////////////////////////////////////
// CHTMLayoutDibAPI - client side for a HTMLayout DIB API

class CHTMLayoutDibAPI
{
public:

	// Attributes

	HTMLAYOUT_HDIB_API* m_papi;

	// Declare module manager that will free loaded module upon exit. Note that we can't
	// place this declaration within the member procedure were it belongs due to MSVC6.0
	// problem at link stage

	struct Module {
		HMODULE hModule;
		Module() : hModule(0) {}
		~Module() { if (hModule) ::FreeLibrary(hModule); }
		HMODULE Load(LPCTSTR pszModule, DWORD flags) { return hModule = ::LoadLibraryEx(pszModule, 0, flags); }
		operator HMODULE() const { return hModule; }
	};

	// Construction

	CHTMLayoutDibAPI()
	{
		m_papi = (HTMLAYOUT_HDIB_API*)::HTMLayoutProc(NULL, WM_HL_GET_INTERFACE, HLINTERFACE_HDIB_API, 17246);
	}

	CHTMLayoutDibAPI(HTMLAYOUT_HDIB_API* papi)
	:	m_papi(papi)
	{
	}

	// Operations

	HDIBEX LoadDibex(LPCTSTR pszFile)
	{
		HLRESULT hlr; USES_CONVERSION;
		
		ATLASSERT(m_papi != NULL);
		ATLASSERT(pszFile != NULL);

		HDIBEX hdibex;
		if ((hlr = m_papi->loadHdibFromFile(T2CA(pszFile), &hdibex)) != HLR_OK) {
			ATLTRACE(_T("CHTMLayoutDibAPI::LoadDibex: loadHdibFromFile() failed, error=%d\n"), hlr);
			return NULL;
		}

		return hdibex;
	}

	HDIBEX LoadDibex(LPCBYTE pb, DWORD cb)
	{
		HLRESULT hlr;
		
		ATLASSERT(m_papi != NULL);
		ATLASSERT(pb != NULL);

		HDIBEX hdibex;
		if ((hlr = m_papi->loadHdibFromMemory((LPBYTE)pb, cb, &hdibex)) != HLR_OK) {
			ATLTRACE(_T("CHTMLayoutDibAPI::LoadDibex: loadHdibFromMemory() failed, error=%d\n"), hlr);
			return NULL;
		}

		return hdibex;
	}

	HDIBEX LoadDibexResource(LPCTSTR pszRes)
	{
		ATLASSERT(m_papi != NULL);
		ATLASSERT(pszRes != NULL);

		// Retrieve resource specification into a local storage since FindResource() expects 
		// to find its parameters on stack rather then on the heap under Win9x/Me

		TCHAR achRes[MAX_PATH]; lstrcpyn(achRes, pszRes, MAX_PATH);

		// Separate name and handle external resource module specification

		LPTSTR psz, pszName = achRes; Module module;
		if ((psz = _tcsrchr(pszName, '/')) != NULL) {
			LPTSTR pszModule = pszName; pszName = psz + 1; *psz = '\0';
			DWORD dwAttr = ::GetFileAttributes(pszModule);
			if (dwAttr != INVALID_FILE_ATTRIBUTES && !(dwAttr & FILE_ATTRIBUTE_DIRECTORY)) {
				module.Load(pszModule, LOAD_LIBRARY_AS_DATAFILE);
			}
		}

		// Separate extension if any

		LPTSTR pszExt = _tcsrchr(pszName, '.'); if (pszExt) *pszExt++ = '\0';

		// Find specified resource and leave if failed. Note that we use extension
		// as the custom resource type specification or assume standard HTML resource 
		// if no extension is specified

		HRSRC hrsrc = ::FindResource(module, pszName, pszExt ? pszExt : RT_HTML);
		if (!hrsrc) return NULL;

		// Load specified resource and check if ok

		HGLOBAL hgres = ::LoadResource(module, hrsrc);
		if (!hgres) return NULL;

		// Retrieve resource data and check if ok

		PBYTE pb = (PBYTE)::LockResource(hgres); if (!pb) return NULL;
		DWORD cb = ::SizeofResource(module, hrsrc); if (!cb) return NULL;

		// Load dibex from memory 
		
		return LoadDibex(pb, cb);
	}

	BOOL DestroyDibex(HDIBEX hdibex)
	{
		HLRESULT hlr;
		
		ATLASSERT(m_papi != NULL);
		ATLASSERT(hdibex != NULL);

		if ((hlr = m_papi->destroyHdib(hdibex)) != HLR_OK) {
			ATLTRACE(_T("CHTMLayoutDibAPI::DestroyDibex: destroyHdib() failed, error=%d\n"), hlr);
			return FALSE;
		}

		return TRUE;
	}

	BOOL SetDibexMargins(HDIBEX hdibex, int cxLeft, int cyTop, int cxRight, int cyBottom)
	{
		HLRESULT hlr;
		
		ATLASSERT(m_papi != NULL);
		ATLASSERT(hdibex != NULL);

		if ((hlr = m_papi->setHdibMargins(hdibex, cxLeft, cyTop, cxRight, cyBottom)) != HLR_OK) {
			ATLTRACE(_T("CHTMLayoutDibAPI::SetDibexMargins: setHdibMargins() failed, error=%d\n"), hlr);
			return FALSE;
		}

		return TRUE;
	}

	BOOL GetDibexInfo(HDIBEX hdibex, LPBITMAPINFO pBitmapInfo)
	{
		HLRESULT hlr;
		
		ATLASSERT(m_papi != NULL);
		ATLASSERT(hdibex != NULL); ATLASSERT(pBitmapInfo != NULL);

		if ((hlr = m_papi->getHdibInfo(hdibex, pBitmapInfo)) != HLR_OK) {
			ATLTRACE(_T("CHTMLayoutDibAPI::GetDibexInfo: getHdibInfo() failed, error=%d\n"), hlr);
			return FALSE;
		}

		return TRUE;
	}

	BOOL GetDibexBits(HDIBEX hdibex, LPBYTE& pb, DWORD& cb)
	{
		HLRESULT hlr;
		
		ATLASSERT(m_papi != NULL);
		ATLASSERT(hdibex != NULL);

		if ((hlr = m_papi->getHdibBits(hdibex, &pb, &cb)) != HLR_OK) {
			ATLTRACE(_T("CHTMLayoutDibAPI::GetDibexBits: getHdibBits() failed, error=%d\n"), hlr);
			return FALSE;
		}

		return TRUE;
	}

	BOOL RenderDibex(HDC hdc, HDIBEX hdibex, int x, int y, int cx, int cy)
	{
		HLRESULT hlr;
		
		ATLASSERT(m_papi != NULL);
		ATLASSERT(hdc != NULL); ATLASSERT(hdibex != NULL);

		if ((hlr = m_papi->renderHdib(hdc, x, y, cx, cy, hdibex)) != HLR_OK) {
			ATLTRACE(_T("CHTMLayoutDibAPI::RenderDibex: renderHdib() failed, error=%d\n"), hlr);
			return FALSE;
		}

		return TRUE;
	}
		
	BOOL RenderDibex(HDC hdc, HDIBEX hdibex, int x, int y, int cx, int cy, int xSrc, int ySrc)
	{
		HLRESULT hlr;
		
		ATLASSERT(m_papi != NULL);
		ATLASSERT(hdc != NULL); ATLASSERT(hdibex != NULL);

		if ((hlr = m_papi->renderHdib(hdc, x, y, cx, cy, xSrc, ySrc, hdibex)) != HLR_OK) {
			ATLTRACE(_T("CHTMLayoutDibAPI::RenderDibex: renderHdib() failed, error=%d\n"), hlr);
			return FALSE;
		}

		return TRUE;
	}
		
	BOOL RenderDibex(HDC hdc, HDIBEX hdibex, int x, int y, int cx, int cy, int xSrc, int ySrc, int cxSrc, int cySrc)
	{
		HLRESULT hlr;
		
		ATLASSERT(m_papi != NULL);
		ATLASSERT(hdc != NULL); ATLASSERT(hdibex != NULL);

		if ((hlr = m_papi->renderHdib(hdc, x, y, cx, cy, xSrc, ySrc, cxSrc, cySrc, hdibex)) != HLR_OK) {
			ATLTRACE(_T("CHTMLayoutDibAPI::RenderDibex: renderHdib() failed, error=%d\n"), hlr);
			return FALSE;
		}

		return TRUE;
	}
		
	BOOL SetColorSchema(HDIBEX hdibex, COLORREF DkShadow, COLORREF Shadow, COLORREF Face, COLORREF Light, COLORREF HighLight)
	{
		HLRESULT hlr;
		
		ATLASSERT(m_papi != NULL);
		ATLASSERT(hdibex != NULL);

		if ((hlr = m_papi->setColorSchema(hdibex, DkShadow, Shadow, Face, Light, HighLight)) != HLR_OK) {
			ATLTRACE(_T("CHTMLayoutDibAPI::SetColorSchema: setColorSchema() failed, error=%d\n"), hlr);
			return FALSE;
		}

		return TRUE;
	}
		
};

/////////////////////////////////////////////////////////////////////////////
// CDibex - Wrapper for a HTMLayout DIBEX object

template <bool t_bManaged>
class CDibexT
{
public:

	// Data members

	HDIBEX m_hdibex;

	// Constructor/destructor/operators

	CDibexT(HDIBEX hDibex = NULL) : m_hdibex(hDibex)
	{
	}

	~CDibexT()
	{
		if (t_bManaged && m_hdibex != NULL) DeleteObject();
	}

	CDibexT<t_bManaged>& operator=(HDIBEX hDibex)
	{
		m_hdibex = hDibex;
		
		return *this;
	}

	void Attach(HDIBEX hDibex)
	{
		if (t_bManaged && m_hdibex != NULL) _HTMLayoutDibAPI.DestroyDibex(m_hdibex);

		m_hdibex = hDibex;
	}

	HDIBEX Detach()
	{
		HDIBEX hDibex = m_hdibex;
		
		m_hdibex = NULL;

		return hDibex;
	}

	operator HDIBEX() const { return m_hdibex; }

	bool IsNull() const { return (m_hdibex == NULL); }

	// Operations

	HDIBEX Load(LPCTSTR pszFile)
	{
		ATLASSERT(m_hdibex == NULL);

		m_hdibex = _HTMLayoutDibAPI.LoadDibex(pszFile);
		
		return m_hdibex;
	}

	HDIBEX Load(LPCBYTE pb, DWORD cb)
	{
		ATLASSERT(m_hdibex == NULL);
		
		m_hdibex = _HTMLayoutDibAPI.LoadDibex(pb, cb);
		
		return m_hdibex;
	}

	HDIBEX LoadResource(LPCTSTR pszRes)
	{
		ATLASSERT(m_hdibex == NULL);

		m_hdibex = _HTMLayoutDibAPI.LoadDibexResource(pszRes);
		
		return m_hdibex;
	}

	BOOL DeleteObject()
	{
		ATLASSERT(m_hdibex != NULL);

		BOOL bRet = _HTMLayoutDibAPI.DestroyDibex(m_hdibex);
		if (bRet) m_hdibex = NULL;

		return bRet;
	}

	BOOL Render(HDC hdc, int x, int y)
	{
		ATLASSERT(m_hdibex != NULL);

		SIZE size; if (!GetSize(size)) return FALSE;

		return _HTMLayoutDibAPI.RenderDibex(hdc, m_hdibex, x, y, size.cx, size.cy);
	}

	BOOL Render(HDC hdc, POINT& pt)
	{	
		ATLASSERT(m_hdibex != NULL);

		return Render(hdc, pt.x, pt.y); 
	}

	BOOL Render(HDC hdc, int x, int y, int cx, int cy)
	{
		ATLASSERT(m_hdibex != NULL);

		return _HTMLayoutDibAPI.RenderDibex(hdc, m_hdibex, x, y, cx, cy);
	}

	BOOL Render(HDC hdc, RECT& rc)
	{	
		ATLASSERT(m_hdibex != NULL);

		return Render(hdc, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top); 
	}

	BOOL Render(HDC hdc, int x, int y, int cx, int cy, int xSrc, int ySrc)
	{
		ATLASSERT(m_hdibex != NULL);

		return _HTMLayoutDibAPI.RenderDibex(hdc, m_hdibex, x, y, cx, cy, xSrc, ySrc);
	}

	BOOL Render(HDC hdc, RECT& rc, int xSrc, int ySrc)
	{	
		ATLASSERT(m_hdibex != NULL);

		return Render(hdc, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, xSrc, ySrc); 
	}

	BOOL Render(HDC hdc, RECT& rc, POINT& ptSrc)
	{	
		ATLASSERT(m_hdibex != NULL);

		return Render(hdc, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, ptSrc.x, ptSrc.y); 
	}

	BOOL Render(HDC hdc, int x, int y, int cx, int cy, int xSrc, int ySrc, int cxSrc, int cySrc)
	{
		ATLASSERT(m_hdibex != NULL);

		return _HTMLayoutDibAPI.RenderDibex(hdc, m_hdibex, x, y, cx, cy, xSrc, ySrc, cxSrc, cySrc);
	}

	BOOL Render(HDC hdc, RECT& rc, int xSrc, int ySrc, int cxSrc, int cySrc)
	{	
		ATLASSERT(m_hdibex != NULL);

		return Render(hdc, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, xSrc, ySrc, cxSrc, cySrc); 
	}

	BOOL Render(HDC hdc, RECT& rc, RECT& rcSrc)
	{	
		ATLASSERT(m_hdibex != NULL);

		return Render(hdc, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, rcSrc.left, rcSrc.top, rcSrc.right - rcSrc.left, rcSrc.bottom - rcSrc.top); 
	}

	// Attributes

	BOOL GetInfo(LPBITMAPINFO pBitmapInfo) const
	{
		ATLASSERT(m_hdibex != NULL);

		return _HTMLayoutDibAPI.GetDibexInfo(m_hdibex, pBitmapInfo);
	}

	BOOL GetBits(LPBYTE& pb, DWORD& cb) const
	{
		ATLASSERT(m_hdibex != NULL);

		return _HTMLayoutDibAPI.GetDibexBits(m_hdibex, pb, cb);
	}

	int GetWidth()
	{
		ATLASSERT(m_hdibex != NULL); BITMAPINFO bmi;

		return GetInfo(&bmi) ? bmi.bmiHeader.biWidth : 0;
	}

	int GetHeight()
	{
		ATLASSERT(m_hdibex != NULL); BITMAPINFO bmi;

		return GetInfo(&bmi) ? bmi.bmiHeader.biHeight : 0;
	}

	BOOL GetSize(SIZE& size)
	{
		ATLASSERT(m_hdibex != NULL);

		BITMAPINFO bmi; if (!GetInfo(&bmi)) return FALSE;

		size.cx = bmi.bmiHeader.biWidth;
		size.cy = bmi.bmiHeader.biHeight;

		return TRUE;
	}

	BOOL GetRect(RECT& rc)
	{
		ATLASSERT(m_hdibex != NULL);

		BITMAPINFO bmi; if (!GetInfo(&bmi)) return FALSE;

		rc.left = 0;
		rc.top = 0;
		rc.right = bmi.bmiHeader.biWidth;
		rc.bottom = bmi.bmiHeader.biHeight;

		return TRUE;
	}

	BOOL SetMargins(int cx, int cy)
	{
		ATLASSERT(m_hdibex != NULL);

		return _HTMLayoutDibAPI.SetDibexMargins(m_hdibex, cx, cy, cx, cy);
	}

	BOOL SetMargins(const SIZE& size)
	{
		ATLASSERT(m_hdibex != NULL);

		return _HTMLayoutDibAPI.SetDibexMargins(m_hdibex, size.cx, size.cy, size.cx, size.cy);
	}

	BOOL SetMargins(int cxLeft, int cyTop, int cxRight, int cyBottom)
	{
		ATLASSERT(m_hdibex != NULL);

		return _HTMLayoutDibAPI.SetDibexMargins(m_hdibex, cxLeft, cyTop, cxRight, cyBottom);
	}

	BOOL SetMargins(const RECT& rc)
	{
		ATLASSERT(m_hdibex != NULL);

		return _HTMLayoutDibAPI.SetDibexMargins(m_hdibex, rc.left, rc.top, rc.right, rc.bottom);
	}

	BOOL SetColorSchema(COLORREF DkShadow, COLORREF Shadow, COLORREF Face, COLORREF Light, COLORREF HighLight)
	{
		ATLASSERT(m_hdibex != NULL);

		return _HTMLayoutDibAPI.SetColorSchema(m_hdibex, DkShadow, Shadow, Face, Light, HighLight);
	}

};

typedef CDibexT<false> CDibexHandle;
typedef CDibexT<true>  CDibex;

}; //namespace WTL

// _HTMLayoutDibAPI is supposed to be defined in some other place

extern CHTMLayoutDibAPI _HTMLayoutDibAPI;

// Link against HTMLayout library

#pragma comment(lib, "htmlayout.lib")

#endif // __ATLHTMLAYOUTEX_H__
