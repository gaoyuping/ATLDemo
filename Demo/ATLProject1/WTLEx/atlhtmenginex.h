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

#include <htmenginex.h>

// This declarations is missing from WinCE headers

#ifndef INVALID_FILE_ATTRIBUTES
#define INVALID_FILE_ATTRIBUTES 0xFFFFFFFF
#endif

/////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CHtmEngineDibAPI
// CDibexT<t_bManaged>
//

namespace WTL
{

/////////////////////////////////////////////////////////////////////////////
// CHtmEngineDibAPI - client side for a HTMLayout DIB API

class CHtmEngineDibAPI
{
public:

	// Attributes

	HTMENGINE_HDIB_API* m_papi;

	// Declare module manager that will free loaded module upon exit. Note that we can't
	// place this declaration within the member procedure were it belongs due to MSVC6.0
	// problem at link stage. Also note that hModule could not be null on WinCE.

	struct Module {
		HMODULE hModule;
		Module() : hModule(0) {}
		~Module() { if (hModule) ::FreeLibrary(hModule); }
		HMODULE Load(LPCTSTR pszModule, DWORD flags) { return hModule = ::LoadLibraryEx(pszModule, 0, flags); }
#ifdef _WIN32_WCE
		operator HMODULE() const { return hModule ? hModule : _Module.GetResourceInstance(); }
#else
		operator HMODULE() const { return hModule; }
#endif
	};

	// Construction

	CHtmEngineDibAPI()
	{
		m_papi = (HTMENGINE_HDIB_API*)::HTMEngineProc(NULL, WM_HE_GET_INTERFACE, HEINTERFACE_HDIB_API, 17246);
	}

	CHtmEngineDibAPI(HTMENGINE_HDIB_API* papi)
	:	m_papi(papi)
	{
	}

	// Operations

	HDIBEX LoadDibex(LPCTSTR pszFile)
	{
		HERESULT hlr; USES_CONVERSION;
		
		ATLASSERT(m_papi != NULL);
		ATLASSERT(pszFile != NULL);

		HDIBEX hdibex;
		if ((hlr = m_papi->loadHdibFromFile(T2CA(pszFile), &hdibex)) != HER_OK) {
			ATLTRACE(_T("CHtmEngineDibAPI::LoadDibex: loadHdibFromFile() failed, error=%d\n"), hlr);
			return NULL;
		}

		return hdibex;
	}

	HDIBEX LoadDibex(LPCBYTE pb, DWORD cb)
	{
		HERESULT hlr;
		
		ATLASSERT(m_papi != NULL);
		ATLASSERT(pb != NULL);

		HDIBEX hdibex;
		if ((hlr = m_papi->loadHdibFromMemory((LPBYTE)pb, cb, &hdibex)) != HER_OK) {
			ATLTRACE(_T("CHtmEngineDibAPI::LoadDibex: loadHdibFromMemory() failed, error=%d\n"), hlr);
			return NULL;
		}

		return hdibex;
	}

	HDIBEX LoadDibexResource(LPCTSTR pszRes, bool bFileOverride = true)
	{
		ATLASSERT(m_papi != NULL);
		ATLASSERT(pszRes != NULL);

		// Check for file override and try to load it

		if (bFileOverride) {
			HDIBEX hdibex = LoadDibexResourceOverride(pszRes);
			if (hdibex) return hdibex;
		}

		// Otherwise get resource data and check if ok

		LPCBYTE pb; DWORD cb;
		if (!_GetResource(pszRes, pb, cb))
			return NULL;

		// Load dibex from memory 
		
		return LoadDibex(pb, cb);
	}

	HDIBEX LoadDibexResourceOverride(LPCTSTR pszRes)
	{
		ATLASSERT(m_papi != NULL);
		ATLASSERT(pszRes != NULL);

		// Get executable module path and check if ok

		TCHAR achPath[MAX_PATH];
		if (::GetModuleFileName(NULL, achPath, MAX_PATH)) {
			::PathRemoveFileSpec(achPath);
		} else
			return false;

		// Build resource override file specification

		TCHAR achFile[MAX_PATH];
		if (!::PathCombine(achFile, achPath, pszRes))
			return false;

		// Check if resourse override file exists and leave if not

		if (::GetFileAttributes(achFile) == INVALID_FILE_ATTRIBUTES)
			return false;

		// Load dibex file and return handle
			
		return LoadDibex(achFile);
	}

	BOOL DestroyDibex(HDIBEX hdibex)
	{
		HERESULT hlr;
		
		ATLASSERT(m_papi != NULL);
		ATLASSERT(hdibex != NULL);

		if ((hlr = m_papi->destroyHdib(hdibex)) != HER_OK) {
			ATLTRACE(_T("CHtmEngineDibAPI::DestroyDibex: destroyHdib() failed, error=%d\n"), hlr);
			return FALSE;
		}

		return TRUE;
	}

	BOOL SetDibexMargins(HDIBEX hdibex, int cxLeft, int cyTop, int cxRight, int cyBottom, int stretchFlags)
	{
		HERESULT hlr;
		
		ATLASSERT(m_papi != NULL);
		ATLASSERT(hdibex != NULL);

		if ((hlr = m_papi->setHdibMargins(hdibex, cxLeft, cyTop, cxRight, cyBottom, stretchFlags)) != HER_OK) {
			ATLTRACE(_T("CHtmEngineDibAPI::SetDibexMargins: setHdibMargins() failed, error=%d\n"), hlr);
			return FALSE;
		}

		return TRUE;
	}

	BOOL GetDibexInfo(HDIBEX hdibex, LPBITMAPINFO pBitmapInfo)
	{
		HERESULT hlr;
		
		ATLASSERT(m_papi != NULL);
		ATLASSERT(hdibex != NULL); ATLASSERT(pBitmapInfo != NULL);

		if ((hlr = m_papi->getHdibInfo(hdibex, pBitmapInfo)) != HER_OK) {
			ATLTRACE(_T("CHtmEngineDibAPI::GetDibexInfo: getHdibInfo() failed, error=%d\n"), hlr);
			return FALSE;
		}

		return TRUE;
	}

	BOOL GetDibexBits(HDIBEX hdibex, LPBYTE& pb, DWORD& cb)
	{
		HERESULT hlr;
		
		ATLASSERT(m_papi != NULL);
		ATLASSERT(hdibex != NULL);

		if ((hlr = m_papi->getHdibBits(hdibex, &pb, &cb)) != HER_OK) {
			ATLTRACE(_T("CHtmEngineDibAPI::GetDibexBits: getHdibBits() failed, error=%d\n"), hlr);
			return FALSE;
		}

		return TRUE;
	}

	BOOL RenderDibex(HDC hdc, HDIBEX hdibex, int x, int y, int cx, int cy)
	{
		HERESULT hlr;
		
		ATLASSERT(m_papi != NULL);
		ATLASSERT(hdc != NULL); ATLASSERT(hdibex != NULL);

		if ((hlr = m_papi->renderHdib(hdc, x, y, cx, cy, hdibex)) != HER_OK) {
			ATLTRACE(_T("CHtmEngineDibAPI::RenderDibex: renderHdib() failed, error=%d\n"), hlr);
			return FALSE;
		}

		return TRUE;
	}
		
	BOOL RenderDibex(HDC hdc, HDIBEX hdibex, int x, int y, int cx, int cy, int xSrc, int ySrc)
	{
		HERESULT hlr;
		
		ATLASSERT(m_papi != NULL);
		ATLASSERT(hdc != NULL); ATLASSERT(hdibex != NULL);

		if ((hlr = m_papi->renderHdib(hdc, x, y, cx, cy, xSrc, ySrc, hdibex)) != HER_OK) {
			ATLTRACE(_T("CHtmEngineDibAPI::RenderDibex: renderHdib() failed, error=%d\n"), hlr);
			return FALSE;
		}

		return TRUE;
	}
		
	BOOL RenderDibex(HDC hdc, HDIBEX hdibex, int x, int y, int cx, int cy, int xSrc, int ySrc, int cxSrc, int cySrc)
	{
		HERESULT hlr;
		
		ATLASSERT(m_papi != NULL);
		ATLASSERT(hdc != NULL); ATLASSERT(hdibex != NULL);

		if ((hlr = m_papi->renderHdib(hdc, x, y, cx, cy, xSrc, ySrc, cxSrc, cySrc, hdibex)) != HER_OK) {
			ATLTRACE(_T("CHtmEngineDibAPI::RenderDibex: renderHdib() failed, error=%d\n"), hlr);
			return FALSE;
		}

		return TRUE;
	}
		
	BOOL SetColorSchema(HDIBEX hdibex, COLORREF DkShadow, COLORREF Shadow, COLORREF Face, COLORREF Light, COLORREF HighLight)
	{
		HERESULT hlr;
		
		ATLASSERT(m_papi != NULL);
		ATLASSERT(hdibex != NULL);

		if ((hlr = m_papi->setColorSchema(hdibex, DkShadow, Shadow, Face, Light, HighLight)) != HER_OK) {
			ATLTRACE(_T("CHtmEngineDibAPI::SetColorSchema: setColorSchema() failed, error=%d\n"), hlr);
			return FALSE;
		}

		return TRUE;
	}
		
	static bool _GetResource(LPCTSTR pszRes, LPCBYTE& pb, DWORD& cb)
	{
		ATLASSERT(pszRes != NULL);

		// Retrieve resource specification into local storage since FindResource() expects 
		// to find its parameters on stack rather then on heap under Win9x/Me

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
		if (!hrsrc) return false;

		// Load specified resource and check if ok

		HGLOBAL hgres = ::LoadResource(module, hrsrc);
		if (!hgres) return false;

		// Retrieve resource data and check if ok

		pb = (LPCBYTE)::LockResource(hgres); if (!pb) return false;
		cb = ::SizeofResource(module, hrsrc); if (!cb) return false;

		return true;
	}

};

/////////////////////////////////////////////////////////////////////////////
// CDibex - Wrapper for a HTMLayout DIBEX object

template <bool t_bManaged>
class CDibexT
{
public:

	// Data members

	CHtmEngineDibAPI m_api;		// DibEx API reference object

	HDIBEX m_hdibex;			// DibEx handle or NULL

	// Constructor/destructor/operators

	CDibexT(HDIBEX hDibex = NULL) : m_hdibex(hDibex)
	{
		ATLASSERT(m_api.m_papi != NULL);
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
		if (t_bManaged && m_hdibex != NULL) m_api.DestroyDibex(m_hdibex);

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

		m_hdibex = m_api.LoadDibex(pszFile);
		
		return m_hdibex;
	}

	HDIBEX Load(LPCBYTE pb, DWORD cb)
	{
		ATLASSERT(m_hdibex == NULL);
		
		m_hdibex = m_api.LoadDibex(pb, cb);
		
		return m_hdibex;
	}

	HDIBEX LoadResource(LPCTSTR pszRes, bool bFileOverride = true)
	{
		ATLASSERT(m_hdibex == NULL);

		m_hdibex = m_api.LoadDibexResource(pszRes, bFileOverride);
		
		return m_hdibex;
	}

	BOOL DeleteObject()
	{
		ATLASSERT(m_hdibex != NULL);

		BOOL bRet = m_api.DestroyDibex(m_hdibex);
		if (bRet) m_hdibex = NULL;

		return bRet;
	}

	BOOL Render(HDC hdc, int x, int y)
	{
		ATLASSERT(m_hdibex != NULL);

		SIZE size = {0, 0};
		if (!GetSize(size)) 
			return FALSE;

		return m_api.RenderDibex(hdc, m_hdibex, x, y, size.cx, size.cy);
	}

	BOOL Render(HDC hdc, POINT& pt)
	{	
		ATLASSERT(m_hdibex != NULL);

		return Render(hdc, pt.x, pt.y); 
	}

	BOOL Render(HDC hdc, int x, int y, int cx, int cy)
	{
		ATLASSERT(m_hdibex != NULL);

		return m_api.RenderDibex(hdc, m_hdibex, x, y, cx, cy);
	}

	BOOL Render(HDC hdc, RECT& rc)
	{	
		ATLASSERT(m_hdibex != NULL);

		return Render(hdc, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top); 
	}

	BOOL Render(HDC hdc, int x, int y, int cx, int cy, int xSrc, int ySrc)
	{
		ATLASSERT(m_hdibex != NULL);

		return m_api.RenderDibex(hdc, m_hdibex, x, y, cx, cy, xSrc, ySrc);
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

		return m_api.RenderDibex(hdc, m_hdibex, x, y, cx, cy, xSrc, ySrc, cxSrc, cySrc);
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

	BOOL GetInfo(LPBITMAPINFO pBitmapInfo)
	{
		ATLASSERT(m_hdibex != NULL);

		return m_api.GetDibexInfo(m_hdibex, pBitmapInfo);
	}

	BOOL GetBits(LPBYTE& pb, DWORD& cb)
	{
		ATLASSERT(m_hdibex != NULL);

		return m_api.GetDibexBits(m_hdibex, pb, cb);
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

	BOOL GetSize(int& cx, int& cy)
	{
		ATLASSERT(m_hdibex != NULL);

		BITMAPINFO bmi; if (!GetInfo(&bmi)) return FALSE;

		cx = bmi.bmiHeader.biWidth;
		cy = bmi.bmiHeader.biHeight;

		return TRUE;
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

	BOOL SetMargins(int cx, int cy, int stretchFlags = 0)
	{
		ATLASSERT(m_hdibex != NULL);

		return m_api.SetDibexMargins(m_hdibex, cx, cy, cx, cy,stretchFlags);
	}

	BOOL SetMargins(const SIZE& size, int stretchFlags = 0)
	{
		ATLASSERT(m_hdibex != NULL);

		return m_api.SetDibexMargins(m_hdibex, size.cx, size.cy, size.cx, size.cy, stretchFlags);
	}

	BOOL SetMargins(int cxLeft, int cyTop, int cxRight, int cyBottom, int stretchFlags = 0)
	{
		ATLASSERT(m_hdibex != NULL);

		return m_api.SetDibexMargins(m_hdibex, cxLeft, cyTop, cxRight, cyBottom, stretchFlags);
	}

	BOOL SetMargins(const RECT& rc, int stretchFlags = 0)
	{
		ATLASSERT(m_hdibex != NULL);

		return m_api.SetDibexMargins(m_hdibex, rc.left, rc.top, rc.right, rc.bottom, stretchFlags);
	}

	BOOL SetColorSchema(COLORREF DkShadow, COLORREF Shadow, COLORREF Face, COLORREF Light, COLORREF HighLight)
	{
		ATLASSERT(m_hdibex != NULL);

		return m_api.SetColorSchema(m_hdibex, DkShadow, Shadow, Face, Light, HighLight);
	}

};

typedef CDibexT<false> CDibexHandle;
typedef CDibexT<true>  CDibex;

}; //namespace WTL

// Link against appropriate HTMEngine library

#ifdef _WIN32_WCE
#pragma comment(lib, "htmenginece.lib")
#else
#pragma comment(lib, "htmengine.lib")
#endif

#endif // __ATLHTMLAYOUTEX_H__
