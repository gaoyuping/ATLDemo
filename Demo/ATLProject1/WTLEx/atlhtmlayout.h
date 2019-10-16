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

#ifndef __ATLHTMLAYOUT_H__
#define __ATLHTMLAYOUT_H__

#pragma once

#ifndef __cplusplus
	#error ATL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLAPP_H__
	#error atlhtmlayout.h requires atlapp.h to be included first
#endif

#ifndef __ATLWIN_H__
	#error atlhtmlayout.h requires atlwin.h to be included first
#endif

#include <htmlayout.h>

/////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CHTMLayoutCtrlT<TBase>
// CHTMLayoutHost<T>
//

namespace WTL
{

/////////////////////////////////////////////////////////////////////////////
// CHTMLayoutCtrl - client side for a HTMLayout control

template <class TBase>
class CHTMLayoutCtrlT : public TBase
{
public:

	// Constructors

	CHTMLayoutCtrlT(HWND hWnd = NULL) : TBase(hWnd) { }

	CHTMLayoutCtrlT< TBase >& operator=(HWND hWnd)
	{
		m_hWnd = hWnd;
		return *this;
	}

	HWND Create(HWND hWndParent, _U_RECT rect = NULL, LPCTSTR szWindowName = NULL,
			DWORD dwStyle = 0, DWORD dwExStyle = 0,
			_U_MENUorID MenuOrID = 0U, LPVOID lpCreateParam = NULL)
	{
		CWindow::Create(GetWndClassName(), hWndParent, rect.m_lpRect, 
			szWindowName, dwStyle, dwExStyle, MenuOrID.m_hMenu, lpCreateParam);

		return m_hWnd;
	}
	
	// Attributes

	static LPCTSTR GetWndClassName()
	{
		return ::HTMLayoutClassNameT();
	}

	BOOL SetText(LPCTSTR lpszText)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return SendMessage(WM_SETTEXT, 0, (LPARAM)lpszText);
	}

	BOOL LoadHtml(LPCSTR lpszText)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return SendMessage(WM_HL_ACTION, WP_HL_ACTION_LOAD_HTML, (LPARAM)lpszText);
	}

	BOOL LoadHtml(LPCBYTE pb, DWORD nBytes)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return SendMessage(WM_HL_ACTION_LOAD_HTML_DATA, (WPARAM)pb, (LPARAM)nBytes);
	}

	BOOL OpenFile(LPCTSTR lpszFilePath)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return SendMessage(WM_HL_ACTION, WP_HL_ACTION_OPEN_FILE, (LPARAM)lpszFilePath);
	}

	BOOL SetCallback(const HTMLAYOUT_CALLBACK* pCallback)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return SendMessage(WM_HL_ACTION, WP_HL_ACTION_SET_CALLBACK, (LPARAM)pCallback);
	}

	int GetMinWidth()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return SendMessage(WM_HL_ACTION, WP_HL_ACTION_GET_MIN_DOCUMENT_WIDTH, 0);
	}
	
	int GetMinHeight(int width)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return SendMessage(WM_HL_ACTION, WP_HL_ACTION_GET_MIN_DOCUMENT_HEIGHT, width);
	}

	BOOL ForceRelayout()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return SendMessage(WM_HL_ACTION, WP_HL_ACTION_FORCE_RELAYOUT, 0);
	}
	
	// Methods

	BOOL CALLBACK HTMLayoutDataReady(LPCWSTR uri, LPBYTE data, DWORD dataLength)
	{
		return ::HTMLayoutDataReady(m_hWnd, uri, data, dataLength);
	}

	LRESULT CALLBACK HTMLayoutProc(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return ::HTMLayoutProc(m_hWnd, msg, wParam, lParam);
	}

};

typedef CHTMLayoutCtrlT<CWindow> CHTMLayoutCtrl;

/////////////////////////////////////////////////////////////////////////////
// CHTMLayoutHost - host side for a HTMLayout control

template <class T>
class CHTMLayoutHost : public HTMLAYOUT_CALLBACK
{
public:

	// HTMLayout callback interface methods
	
	virtual LRESULT __stdcall notification(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return OnHtmlNotify(uMsg, wParam, lParam);
	}

	virtual LRESULT __stdcall child_ctl_notification(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		// NOTE: This method is never called with current HTMLayout implementation if its 
		// window procedure has been subclassed. The only way to handle notifications from
		// embedded controls is to catch them in the subclassed code
		
		return OnCtrlNotify(uMsg, wParam, lParam);
	}

	virtual LRESULT __stdcall windowless_ctl_handler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	// Overridables

	virtual LRESULT OnHtmlNotify(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		ATLASSERT(uMsg == WM_NOTIFY);

		// Crack and call appropriate method

		switch(((NMHDR*)lParam)->code) {
			case HLN_CREATE_CONTROL:    return OnCreateControl((LPNMHL_CREATE_CONTROL) lParam);
			case HLN_CONTROL_CREATED:   return OnControlCreated((LPNMHL_CREATE_CONTROL) lParam);
			case HLN_HYPERLINK:         return OnHyperLink((LPNMHL_HYPERLINK) lParam);
			case HLN_LOAD_DATA:         return OnLoadData((LPNMHL_LOAD_DATA) lParam);
            case HLN_DATA_LOADED:       return OnDataLoaded((LPNMHL_DATA_LOADED)lParam);
            case HLN_DOCUMENT_COMPLETE: return OnDocumentComplete();
		}

		return 0;
	}

	virtual LRESULT OnCtrlNotify(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		T* pT = static_cast<T*>(this);
		
		ATLASSERT(::IsWindow(pT->m_hWnd));

		// Pass it to the parent window if any
		
		HWND hWndParent = pT->GetParent();
		if (!hWndParent) return 0;

		return ::SendMessage(hWndParent, uMsg, wParam, lParam);
	}

	virtual LRESULT OnCreateControl(LPNMHL_CREATE_CONTROL pnmcc)
	{
		ATLTRACE(_T("CHTMLayoutHost::OnCreateControl: type='%s' name='%s' id='%s' tagType=%u\n"), CString(pnmcc->type), CString(pnmcc->name), CString(pnmcc->id), pnmcc->tagType);

		// Try to create control and if failed, proceed with default processing.
		// Note that this code assumes that the host and control windows are the same. If
		// you are handling HTMLayout control notification in another window, you'll have
		// to override this method and provide proper hWnd.

		T* pT = static_cast<T*>(this);
		
		ATLASSERT(::IsWindow(pT->m_hWnd));

		return CreateControl(pT->m_hWnd, pnmcc);
	}

	virtual LRESULT OnControlCreated(LPNMHL_CREATE_CONTROL pnmcc)
	{
		ATLTRACE(_T("CHTMLayoutHost::OnControlCreated: type='%s' name='%s' id='%s' tagType=%u\n"), CString(pnmcc->type), CString(pnmcc->name), CString(pnmcc->id), pnmcc->tagType);

		ATLASSERT(pnmcc->outControlHwnd != NULL);

		return 0;
	}

	virtual LRESULT OnHyperLink(LPNMHL_HYPERLINK pnmhl)
	{
		ATLTRACE(_T("CHTMLayoutHost::OnHyperLink: name='%s' href='%s' target='%s' status=%u\n"), CString(pnmhl->name), CString(pnmhl->href), CString(pnmhl->target), pnmhl->status);

		switch (pnmhl->status) {
			case HL_HYPERLINK_ENTER: return OnHyperLinkEnter(pnmhl);
			case HL_HYPERLINK_LEAVE: return OnHyperLinkLeave(pnmhl);
			case HL_HYPERLINK_CLICK: return OnHyperLinkClick(pnmhl);
		}

		return HWND_TRY_DEFAULT;
	}

	virtual LRESULT OnHyperLinkEnter(LPNMHL_HYPERLINK pnmhl)
	{
		ATLTRACE(_T("CHTMLayoutHost::OnHyperLinkEnter: name='%s' href='%s' target='%s' status=%u\n"), CString(pnmhl->name), CString(pnmhl->href), CString(pnmhl->target), pnmhl->status);
		
		return HWND_TRY_DEFAULT;
	}

	virtual LRESULT OnHyperLinkLeave(LPNMHL_HYPERLINK pnmhl)
	{
		ATLTRACE(_T("CHTMLayoutHost::OnHyperLinkLeave: name='%s' href='%s' target='%s' status=%u\n"), CString(pnmhl->name), CString(pnmhl->href), CString(pnmhl->target), pnmhl->status);
		
		return HWND_TRY_DEFAULT;
	}

	virtual LRESULT OnHyperLinkClick(LPNMHL_HYPERLINK pnmhl)
	{
		ATLTRACE(_T("CHTMLayoutHost::OnHyperLinkClick: name='%s' href='%s' target='%s' status=%u\n"), CString(pnmhl->name), CString(pnmhl->href), CString(pnmhl->target), pnmhl->status);
		
		return HWND_TRY_DEFAULT;
	}

	virtual LRESULT OnLoadData(LPNMHL_LOAD_DATA pnmld)
	{
		ATLTRACE(_T("CHTMLayoutHost::OnLoadData: uri='%s'\n"), CString(pnmld->uri));

		// Try to load data from resource and if failed, proceed with default processing.
		// Note that this code assumes that the host and control windows are the same. If
		// you are handling HTMLayout control notification in another window, you'll have
		// to override this method and provide proper hWnd.

		T* pT = static_cast<T*>(this);
		
		ATLASSERT(::IsWindow(pT->m_hWnd));
#if 0
		return LoadResourceData(pT->m_hWnd, pnmld);
#else
		// FIXME: workaround...
		return LoadResourceData(pT->m_hWnd, pnmld) == LOAD_DATA_VALID ? 0 : LOAD_TRY_DEFAULT;
#endif
	}

	virtual LRESULT OnDataLoaded(LPNMHL_DATA_LOADED pnmld)
	{
		ATLTRACE(_T("CHTMLayoutHost::OnDataLoaded: uri='%s'\n"), CString(pnmld->uri));
        return 0;
	}

	virtual LRESULT OnDocumentComplete()
	{
		ATLTRACE(_T("CHTMLayoutHost::OnDocumentComplete\n"));
        return 0;
	}
  
	// Declare module manager that will free loaded module upon exit. Note that we can't
	// place this declaration within the static procedure were it belongs due to MSVC6.0
	// problem at link stage

	struct Module {
		HMODULE hModule;
		Module() : hModule(0) {}
		~Module() { if (hModule) ::FreeLibrary(hModule); }
		HMODULE Load(LPCTSTR pszModule, DWORD flags) { return hModule = ::LoadLibraryEx(pszModule, 0, flags); }
		operator HMODULE() const { return hModule; }
	};

	// Load Data helper routines

	static LRESULT LoadResourceData(HWND hWnd, LPNMHL_LOAD_DATA pnmld)
	{
		USES_CONVERSION;

		ATLASSERT(::IsWindow(hWnd));
		ATLASSERT(pnmld != NULL);

		// Check for trivial case

		if (!pnmld->uri || !pnmld->uri[0]) return LOAD_DISCARD;

		// Retrieve url specification into a local storage since FindResource() expects 
		// to find its parameters on stack rather then on the heap under Win9x/Me

		TCHAR achURL[MAX_PATH]; lstrcpyn(achURL, W2CT(pnmld->uri), MAX_PATH);

		// Separate name and handle external resource module specification

		LPTSTR psz, pszName = achURL;  Module module;
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
		if (!hrsrc) return LOAD_TRY_DEFAULT;

		// Load specified resource and check if ok

		HGLOBAL hgres = ::LoadResource(module, hrsrc);
		if (!hgres) return LOAD_DISCARD;

		// Retrieve resource data and check if ok

		PBYTE pb = (PBYTE)::LockResource(hgres); if (!pb) return LOAD_DISCARD;
		DWORD cb = ::SizeofResource(module, hrsrc); if (!cb) return LOAD_DISCARD;

		// Report data ready

		if (!HTMLayoutDataReady(hWnd, pnmld->uri, pb,  cb)) return LOAD_DISCARD;
		
		return LOAD_DATA_VALID;
	}

	LRESULT LoadResourceData(LPNMHL_LOAD_DATA pnmld)
	{
		// This code assumes that the host and control windows are the same

		T* pT = static_cast<T*>(this);
		
		ATLASSERT(::IsWindow(pT->m_hWnd));

		return LoadResourceData(pT->m_hWnd, pnmld);
	}

	static PBYTE GetHtmlResource(LPTSTR pszName, HMODULE hModule = NULL)
	{
		ATLASSERT(pszName != NULL);

		// Find specified resource and check if ok
		
		HRSRC hrsrc = ::FindResource(hModule, pszName, MAKEINTRESOURCE(RT_HTML));
		if(!hrsrc) return false;

		// Load specified resource and check if ok
		
		HGLOBAL hgres = ::LoadResource(hModule, hrsrc);
		if(!hgres) return false;

		// Retrieve resource data and check if ok

		PBYTE pb = (PBYTE)::LockResource(hgres); if (!pb) return false;
		DWORD cb = ::SizeofResource(hModule, hrsrc); if (!cb) return false;

		return pb;
	}

	static bool LoadHtmlResource(HWND hWnd, LPTSTR pszName, HMODULE hModule = NULL)
	{
		ATLASSERT(::IsWindow(hWnd));
		ATLASSERT(pszName != NULL);

		// Get specified html resource and check if ok

		PBYTE pb = GetHtmlResource(pszName, hModule);
		if (!pb) return false;

		// Load html data info control and check if ok

		if (!::SendMessage(hWnd, WM_HL_ACTION, WP_HL_ACTION_LOAD_HTML, (LPARAM)pb)) return false;

		return true;
	}

	bool LoadHtmlResource(LPTSTR pszName, HMODULE hModule = NULL)
	{
		// This code assumes that the host and control windows are the same

		T* pT = static_cast<T*>(this);
		
		ATLASSERT(::IsWindow(pT->m_hWnd));

		return LoadHtmlResource(pT->m_hWnd, pszName, hModule);
	}

	// Create Control helper routines

	static LPCWSTR GetAttr(LPNMHL_CREATE_CONTROL pnmcc, LPCSTR pszName)
	{
		ATLASSERT(pnmcc != NULL);
		ATLASSERT(pszName != NULL);

		// Scan attribute table looking for the specified name and
		// return associated value if found, null otherwise
		
		for (int n = 0; n < pnmcc->attributeCount; n++) {
			if (stricmp(pszName, pnmcc->attributeNames[n]) == 0) {
				return pnmcc->attributeValues[n];
			}
		}
		
		return NULL;
	}

	static int GetAttrInt(LPNMHL_CREATE_CONTROL pnmcc, LPCSTR pszName)
	{
		ATLASSERT(pnmcc != NULL);
		ATLASSERT(pszName != NULL);

		// Get attribute string value and if ok, convert it to interger

		LPCWSTR psz = GetAttr(pnmcc, pszName);
		
		return psz ? _wtoi(psz) : 0;
	}

	static bool HasAttr(LPNMHL_CREATE_CONTROL pnmcc, LPCSTR pszName)
	{
		ATLASSERT(pnmcc != NULL);
		ATLASSERT(pszName != NULL);

		// Return positive if specified attribute is present

		return GetAttr(pnmcc, pszName) != NULL;
	}

	virtual CString GetCtrlText(LPNMHL_CREATE_CONTROL pnmcc)
	{
		ATLASSERT(pnmcc != NULL);

		// Return control window text

		LPCWSTR psz = GetAttr(pnmcc, "value");

		CString text; if (psz) text = psz;

		return text;
	}

	virtual UINT GetCtrlID(LPNMHL_CREATE_CONTROL pnmcc)
	{
		ATLASSERT(pnmcc != NULL);

		// In order to provide symbolic control id translation, override 
		// this method in your host code with something like:

//		static CTRLIDTABLE aCtrlID[] = {
//			CTRLIDTABENTRY(IDC_RICHEDIT),
//			CTRLIDTABENTRY(IDC_DATETIME),
//			...
//			CTRLIDTABLEEND
//		};
//
//		return MapCtrlID(pnmcc, aCtrlID);

		// Try to convert numeric id specification

		return GetAttrInt(pnmcc, "id");
	}

	virtual LRESULT CreateControl(HWND hWnd, LPNMHL_CREATE_CONTROL pnmcc)
	{
		ATLASSERT(::IsWindow(hWnd));
		ATLASSERT(pnmcc != NULL);

		// Create control of the specified type
		
		if (!wcscmp(pnmcc->type, L"stdbutton")) return CreateButton(hWnd, pnmcc);
		if (!wcscmp(pnmcc->type, L"stdcheck")) return CreateButton(hWnd, pnmcc);
		if (!wcscmp(pnmcc->type, L"stdradio")) return CreateButton(hWnd, pnmcc);
		if (!wcscmp(pnmcc->type, L"stdedit"))  return CreateEdit(hWnd, pnmcc);
		if (!wcscmp(pnmcc->type, L"stdlist"))  return CreateListBox(hWnd, pnmcc);
		if (!wcscmp(pnmcc->type, L"stdcombo")) return CreateComboBox(hWnd, pnmcc);
		
		if (!wcscmp(pnmcc->type, L"richedit")) return CreateRichEdit(hWnd, pnmcc);
		if (!wcscmp(pnmcc->type, L"datetime")) return CreateDateTime(hWnd, pnmcc);
		if (!wcscmp(pnmcc->type, L"calendar")) return CreateCalendar(hWnd, pnmcc);
		if (!wcscmp(pnmcc->type, L"listview")) return CreateListView(hWnd, pnmcc);
		if (!wcscmp(pnmcc->type, L"treeview")) return CreateTreeView(hWnd, pnmcc);

		return HWND_TRY_DEFAULT;
	}

	// Control id mapping table declarations

	#define CTRLIDTABENTRY(id) { id, L#id }
	#define CTRLIDTABLEEND { 0, NULL }

	typedef struct _CTRLIDTABLE {
		DWORD id; LPWSTR psz;
	} CTRLIDTABLE;

	static UINT MapCtrlID(LPNMHL_CREATE_CONTROL pnmcc, const CTRLIDTABLE* pCtrlID)
	{
		ATLASSERT(pnmcc != NULL);
		ATLASSERT(pCtrlID != NULL);

		// Get control id string and check if ok

		LPCWSTR psz = GetAttr(pnmcc, "id");
		if (!psz) return false;

		// Scan specified control id table looking for the match

		for ( ; pCtrlID->psz != NULL; pCtrlID++) {
			if (!wcscmp(psz, pCtrlID->psz)) {
				return pCtrlID->id;
			}
		}
		
		// Finally try to convert numeric id specification
		
		return _wtoi(psz);
	}

	// Style table declarations

	#define STYLETABENTRY(prefix, style) { prefix##style, #style }
	#define STYLETABLEEND { 0, NULL }

	typedef struct _STYLETABLE {
		DWORD dwStyle; LPCSTR pszStyle;
	} STYLETABLE;

	static DWORD GetCtrlStyle(LPNMHL_CREATE_CONTROL pnmcc, const STYLETABLE* pStyle, DWORD dwDefStyle = 0)
	{
		ATLASSERT(pnmcc != NULL);
		ATLASSERT(pStyle != NULL);

		// Scan specified style table collecting styles

		for (DWORD dwStyle = 0; pStyle->pszStyle != NULL; pStyle++) {
			if (HasAttr(pnmcc, pStyle->pszStyle)) {
				dwStyle|= pStyle->dwStyle;
			}
		}
		
		// Return collected styles or default style if none specified
		
		return dwStyle ? dwStyle : dwDefStyle;
	}

	// Generic window style handling

	static DWORD GetCtrlStyle(LPNMHL_CREATE_CONTROL pnmcc, DWORD dwDefStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN)
	{
		// Window style table

		static STYLETABLE aStyles[] = {
			STYLETABENTRY(WS_, OVERLAPPED),
			STYLETABENTRY(WS_, POPUP),
			STYLETABENTRY(WS_, CHILD),
			STYLETABENTRY(WS_, MINIMIZE),
			STYLETABENTRY(WS_, VISIBLE),
			STYLETABENTRY(WS_, DISABLED),
			STYLETABENTRY(WS_, CLIPSIBLINGS),
			STYLETABENTRY(WS_, CLIPCHILDREN),
			STYLETABENTRY(WS_, MAXIMIZE),
			STYLETABENTRY(WS_, CAPTION),
			STYLETABENTRY(WS_, BORDER),
			STYLETABENTRY(WS_, DLGFRAME),
			STYLETABENTRY(WS_, VSCROLL),
			STYLETABENTRY(WS_, HSCROLL),
			STYLETABENTRY(WS_, SYSMENU),
			STYLETABENTRY(WS_, THICKFRAME),
			STYLETABENTRY(WS_, GROUP),
			STYLETABENTRY(WS_, TABSTOP),
			STYLETABENTRY(WS_, MINIMIZEBOX),
			STYLETABENTRY(WS_, MAXIMIZEBOX),
			STYLETABLEEND
		};

		return GetCtrlStyle(pnmcc, aStyles, dwDefStyle);
	}

	static DWORD GetCtrlExStyle(LPNMHL_CREATE_CONTROL pnmcc, DWORD dwDefExStyle = 0)
	{
		// Extended window style table

		static STYLETABLE aStyles[] = {
			STYLETABENTRY(WS_EX_, DLGMODALFRAME),
			STYLETABENTRY(WS_EX_, NOPARENTNOTIFY),
			STYLETABENTRY(WS_EX_, TOPMOST),
			STYLETABENTRY(WS_EX_, ACCEPTFILES),
			STYLETABENTRY(WS_EX_, TRANSPARENT),
		#if (WINVER >= 0x0400)
			STYLETABENTRY(WS_EX_, MDICHILD),
			STYLETABENTRY(WS_EX_, TOOLWINDOW),
			STYLETABENTRY(WS_EX_, WINDOWEDGE),
			STYLETABENTRY(WS_EX_, CLIENTEDGE),
			STYLETABENTRY(WS_EX_, CONTEXTHELP),
		#endif
		#if (WINVER >= 0x0400)
			STYLETABENTRY(WS_EX_, RIGHT),
			STYLETABENTRY(WS_EX_, LEFT),
			STYLETABENTRY(WS_EX_, RTLREADING),
			STYLETABENTRY(WS_EX_, LTRREADING),
			STYLETABENTRY(WS_EX_, LEFTSCROLLBAR),
			STYLETABENTRY(WS_EX_, RIGHTSCROLLBAR),
			STYLETABENTRY(WS_EX_, CONTROLPARENT),
			STYLETABENTRY(WS_EX_, STATICEDGE),
			STYLETABENTRY(WS_EX_, APPWINDOW),
		#endif
		#if (_WIN32_WINNT >= 0x0500)
			STYLETABENTRY(WS_EX_, LAYERED),
		#endif
		#if (WINVER >= 0x0500)
			STYLETABENTRY(WS_EX_, NOINHERITLAYOUT),
			STYLETABENTRY(WS_EX_, LAYOUTRTL),
		#endif
		#if (_WIN32_WINNT >= 0x0500)
			STYLETABENTRY(WS_EX_, COMPOSITED),
			STYLETABENTRY(WS_EX_, NOACTIVATE),
		#endif
			STYLETABLEEND
		};

		return GetCtrlStyle(pnmcc, aStyles, dwDefExStyle);
	}

	// Control factory routines

	virtual LRESULT CreateButton(HWND hwndParent, LPNMHL_CREATE_CONTROL pnmcc)
	{
		ATLASSERT(::IsWindow(hwndParent));
		ATLASSERT(pnmcc != NULL);
		
		// Control style table

		static STYLETABLE aStyles[] = {
			STYLETABENTRY(BS_, PUSHBUTTON),
			STYLETABENTRY(BS_, DEFPUSHBUTTON),
			STYLETABENTRY(BS_, CHECKBOX),
			STYLETABENTRY(BS_, AUTOCHECKBOX),
			STYLETABENTRY(BS_, RADIOBUTTON),
			STYLETABENTRY(BS_, 3STATE),
			STYLETABENTRY(BS_, AUTO3STATE),
			STYLETABENTRY(BS_, GROUPBOX),
			STYLETABENTRY(BS_, USERBUTTON),
			STYLETABENTRY(BS_, AUTORADIOBUTTON),
			STYLETABENTRY(BS_, PUSHBOX),
			STYLETABENTRY(BS_, OWNERDRAW),
			STYLETABENTRY(BS_, LEFTTEXT),
		#if(WINVER >= 0x0400)
			STYLETABENTRY(BS_, TEXT),
			STYLETABENTRY(BS_, ICON),
			STYLETABENTRY(BS_, BITMAP),
			STYLETABENTRY(BS_, LEFT),
			STYLETABENTRY(BS_, RIGHT),
			STYLETABENTRY(BS_, CENTER),
			STYLETABENTRY(BS_, TOP),
			STYLETABENTRY(BS_, BOTTOM),
			STYLETABENTRY(BS_, VCENTER),
			STYLETABENTRY(BS_, PUSHLIKE),
			STYLETABENTRY(BS_, MULTILINE),
			STYLETABENTRY(BS_, NOTIFY),
			STYLETABENTRY(BS_, FLAT),
			STYLETABENTRY(BS_, RIGHTBUTTON),
		#endif
			STYLETABLEEND
		};

		// Set default button type

		DWORD dwDefStyle = 0;
		if (!wcscmp(pnmcc->type, L"stdbutton")) dwDefStyle = BS_PUSHBUTTON; else
		if (!wcscmp(pnmcc->type, L"stdcheck"))  dwDefStyle = BS_AUTOCHECKBOX; else
		if (!wcscmp(pnmcc->type, L"stdradio"))  dwDefStyle = BS_AUTORADIOBUTTON;
		
		// Make up the control window style

		DWORD dwStyle = GetCtrlStyle(pnmcc) | GetCtrlStyle(pnmcc, aStyles, dwDefStyle);
		DWORD dwExStyle = GetCtrlExStyle(pnmcc);

		// Create control and check if ok

		pnmcc->outControlHwnd = ::CreateWindowEx(
			dwExStyle, CButton::GetWndClassName(), GetCtrlText(pnmcc), dwStyle, 
			0, 0, 0, 0, hwndParent, (HMENU)GetCtrlID(pnmcc), _Module.GetModuleInstance(), pnmcc);

		if (!pnmcc->outControlHwnd) return HWND_DISCARD_CREATION;

		// Handle checked attribute

		if (HasAttr(pnmcc, "checked")) {
			::SendMessage(pnmcc->outControlHwnd, BM_SETCHECK, BST_CHECKED, 0);
		}
		
		return 0;
	}

	virtual LRESULT CreateEdit(HWND hwndParent, LPNMHL_CREATE_CONTROL pnmcc)
	{
		ATLASSERT(::IsWindow(hwndParent));
		ATLASSERT(pnmcc != NULL);
		
		// Control style table

		static STYLETABLE aStyles[] = {
			STYLETABENTRY(ES_, LEFT),
			STYLETABENTRY(ES_, CENTER),
			STYLETABENTRY(ES_, RIGHT),
			STYLETABENTRY(ES_, MULTILINE),
			STYLETABENTRY(ES_, UPPERCASE),
			STYLETABENTRY(ES_, LOWERCASE),
			STYLETABENTRY(ES_, PASSWORD),
			STYLETABENTRY(ES_, AUTOVSCROLL),
			STYLETABENTRY(ES_, AUTOHSCROLL),
			STYLETABENTRY(ES_, NOHIDESEL),
			STYLETABENTRY(ES_, OEMCONVERT),
			STYLETABENTRY(ES_, READONLY),
			STYLETABENTRY(ES_, WANTRETURN),
		#if(WINVER >= 0x0400)
			STYLETABENTRY(ES_, NUMBER),
		#endif
			STYLETABLEEND
		};

		// Make up the control window style

		DWORD dwStyle = GetCtrlStyle(pnmcc) | GetCtrlStyle(pnmcc, aStyles);
		DWORD dwExStyle = GetCtrlExStyle(pnmcc);

		// Create control and check if ok

		pnmcc->outControlHwnd = ::CreateWindowEx(
			dwExStyle, CEdit::GetWndClassName(), GetCtrlText(pnmcc), dwStyle, 
			0, 0, 0, 0, hwndParent, (HMENU)GetCtrlID(pnmcc), _Module.GetModuleInstance(), pnmcc);

		if (!pnmcc->outControlHwnd) return HWND_DISCARD_CREATION;

		return 0;
	}

	virtual LRESULT CreateListBox(HWND hwndParent, LPNMHL_CREATE_CONTROL pnmcc)
	{
		ATLASSERT(::IsWindow(hwndParent));
		ATLASSERT(pnmcc != NULL);
		
		// Control style table

		static STYLETABLE aStyles[] = {
			STYLETABENTRY(LBS_, NOTIFY),
			STYLETABENTRY(LBS_, SORT),
			STYLETABENTRY(LBS_, NOREDRAW),
			STYLETABENTRY(LBS_, MULTIPLESEL),
			STYLETABENTRY(LBS_, OWNERDRAWFIXED),
			STYLETABENTRY(LBS_, OWNERDRAWVARIABLE),
			STYLETABENTRY(LBS_, HASSTRINGS),
			STYLETABENTRY(LBS_, USETABSTOPS),
			STYLETABENTRY(LBS_, NOINTEGRALHEIGHT),
			STYLETABENTRY(LBS_, MULTICOLUMN),
			STYLETABENTRY(LBS_, WANTKEYBOARDINPUT),
			STYLETABENTRY(LBS_, EXTENDEDSEL),
			STYLETABENTRY(LBS_, DISABLENOSCROLL),
			STYLETABENTRY(LBS_, NODATA),
		#if(WINVER >= 0x0400)
			STYLETABENTRY(LBS_, NOSEL),
		#endif
			STYLETABENTRY(LBS_, COMBOBOX),
			STYLETABLEEND
		};

		// Make up the control window style

		DWORD dwStyle = GetCtrlStyle(pnmcc) | GetCtrlStyle(pnmcc, aStyles, LBS_STANDARD);
		DWORD dwExStyle = GetCtrlExStyle(pnmcc);

		// Create control and check if ok

		pnmcc->outControlHwnd = ::CreateWindowEx(
			dwExStyle, CListBox::GetWndClassName(), GetCtrlText(pnmcc), dwStyle, 
			0, 0, 0, 0, hwndParent, (HMENU)GetCtrlID(pnmcc), _Module.GetModuleInstance(), pnmcc);

		if (!pnmcc->outControlHwnd) return HWND_DISCARD_CREATION;

		return 0;
	}

	virtual LRESULT CreateComboBox(HWND hwndParent, LPNMHL_CREATE_CONTROL pnmcc)
	{
		ATLASSERT(::IsWindow(hwndParent));
		ATLASSERT(pnmcc != NULL);
		
		// Control style table

		static STYLETABLE aStyles[] = {
			STYLETABENTRY(CBS_, SIMPLE),
			STYLETABENTRY(CBS_, DROPDOWN),
			STYLETABENTRY(CBS_, DROPDOWNLIST),
			STYLETABENTRY(CBS_, OWNERDRAWFIXED),
			STYLETABENTRY(CBS_, OWNERDRAWVARIABLE),
			STYLETABENTRY(CBS_, AUTOHSCROLL),
			STYLETABENTRY(CBS_, OEMCONVERT),
			STYLETABENTRY(CBS_, SORT),
			STYLETABENTRY(CBS_, HASSTRINGS),
			STYLETABENTRY(CBS_, NOINTEGRALHEIGHT),
			STYLETABENTRY(CBS_, DISABLENOSCROLL),
		#if(WINVER >= 0x0400)
			STYLETABENTRY(CBS_, UPPERCASE),
			STYLETABENTRY(CBS_, LOWERCASE),
		#endif
			STYLETABLEEND
		};

		// Make up the control window style

		DWORD dwStyle = GetCtrlStyle(pnmcc) | GetCtrlStyle(pnmcc, aStyles);
		DWORD dwExStyle = GetCtrlExStyle(pnmcc);

		// Create control and check if ok

		pnmcc->outControlHwnd = ::CreateWindowEx(
			dwExStyle, CComboBox::GetWndClassName(), GetCtrlText(pnmcc), dwStyle, 
			0, 0, 0, 0, hwndParent, (HMENU)GetCtrlID(pnmcc), _Module.GetModuleInstance(), pnmcc);

		if (!pnmcc->outControlHwnd) return HWND_DISCARD_CREATION;

		return 0;
	}

	virtual LRESULT CreateRichEdit(HWND hwndParent, LPNMHL_CREATE_CONTROL pnmcc)
	{
		ATLASSERT(::IsWindow(hwndParent));
		ATLASSERT(pnmcc != NULL);
		
		// Control style table

		static STYLETABLE aStyles[] = {
			STYLETABENTRY(ES_, LEFT),
			STYLETABENTRY(ES_, CENTER),
			STYLETABENTRY(ES_, RIGHT),
			STYLETABENTRY(ES_, MULTILINE),
			STYLETABENTRY(ES_, PASSWORD),
			STYLETABENTRY(ES_, AUTOVSCROLL),
			STYLETABENTRY(ES_, AUTOHSCROLL),
			STYLETABENTRY(ES_, NOHIDESEL),
			STYLETABENTRY(ES_, READONLY),
			STYLETABENTRY(ES_, WANTRETURN),
		#if(WINVER >= 0x0400)
			STYLETABENTRY(ES_, NUMBER),
		#endif
			STYLETABENTRY(ES_, SAVESEL),
			STYLETABENTRY(ES_, SUNKEN),
			STYLETABENTRY(ES_, DISABLENOSCROLL),
			STYLETABENTRY(ES_, SELECTIONBAR),
			STYLETABENTRY(ES_, NOOLEDRAGDROP),
			STYLETABENTRY(ES_, VERTICAL),
			STYLETABENTRY(ES_, NOIME),
			STYLETABENTRY(ES_, SELFIME),
			STYLETABLEEND
		};

		// Make up the control window style

		DWORD dwStyle = GetCtrlStyle(pnmcc) | GetCtrlStyle(pnmcc, aStyles, ES_MULTILINE | ES_WANTRETURN);
		DWORD dwExStyle = GetCtrlExStyle(pnmcc);

		// Create control and check if ok

		pnmcc->outControlHwnd = ::CreateWindowEx(
			dwExStyle, CRichEditCtrl::GetWndClassName(), GetCtrlText(pnmcc), dwStyle, 
			0, 0, 0, 0, hwndParent, (HMENU)GetCtrlID(pnmcc), _Module.GetModuleInstance(), pnmcc);

		if (!pnmcc->outControlHwnd) return HWND_DISCARD_CREATION;

		// Handle bottomless RichEdit controls

		if (HasAttr(pnmcc, "autoheight")) {
			::SendMessage(pnmcc->outControlHwnd, EM_SETEVENTMASK, 0, ENM_REQUESTRESIZE);
		}
		
		return 0;
	}

	virtual LRESULT CreateDateTime(HWND hwndParent, LPNMHL_CREATE_CONTROL pnmcc)
	{
		ATLASSERT(::IsWindow(hwndParent));
		ATLASSERT(pnmcc != NULL);

		// Control style table

		static STYLETABLE aStyles[] = {
			STYLETABENTRY(DTS_, UPDOWN),
			STYLETABENTRY(DTS_, SHOWNONE),
			STYLETABENTRY(DTS_, SHORTDATEFORMAT),
			STYLETABENTRY(DTS_, LONGDATEFORMAT),
		#if (_WIN32_IE >= 0x500)
			STYLETABENTRY(DTS_, SHORTDATECENTURYFORMAT),
		#endif
			STYLETABENTRY(DTS_, TIMEFORMAT),
			STYLETABENTRY(DTS_, APPCANPARSE),
			STYLETABENTRY(DTS_, RIGHTALIGN),
			STYLETABLEEND
		};

		// Make up the control window style

		DWORD dwStyle = GetCtrlStyle(pnmcc) | GetCtrlStyle(pnmcc, aStyles);
		DWORD dwExStyle = GetCtrlExStyle(pnmcc);

		// Create control and check if ok

		pnmcc->outControlHwnd = ::CreateWindowEx(
			dwExStyle, CDateTimePickerCtrl::GetWndClassName(), GetCtrlText(pnmcc), dwStyle, 
			0, 0, 0, 0, hwndParent, (HMENU)GetCtrlID(pnmcc), _Module.GetModuleInstance(), pnmcc);

		if (!pnmcc->outControlHwnd) return HWND_DISCARD_CREATION;
		
		return 0;
	}

	virtual LRESULT CreateCalendar(HWND hwndParent, LPNMHL_CREATE_CONTROL pnmcc)
	{
		ATLASSERT(::IsWindow(hwndParent));
		ATLASSERT(pnmcc != NULL);
		
		// Control style table

		static STYLETABLE aStyles[] = {
			STYLETABENTRY(MCS_, DAYSTATE),
			STYLETABENTRY(MCS_, MULTISELECT),
			STYLETABENTRY(MCS_, WEEKNUMBERS),
		#if (_WIN32_IE >= 0x0400)
			STYLETABENTRY(MCS_, NOTODAYCIRCLE),
			STYLETABENTRY(MCS_, NOTODAY),
		#else
			STYLETABENTRY(MCS_, NOTODAY),
		#endif
			STYLETABLEEND
		};

		// Make up the control window style

		DWORD dwStyle = GetCtrlStyle(pnmcc) | GetCtrlStyle(pnmcc, aStyles);
		DWORD dwExStyle = GetCtrlExStyle(pnmcc);

		// Create control and check if ok

		pnmcc->outControlHwnd = ::CreateWindowEx(
			dwExStyle, CMonthCalendarCtrl::GetWndClassName(), GetCtrlText(pnmcc), dwStyle, 
			0, 0, 0, 0, hwndParent, (HMENU)GetCtrlID(pnmcc), _Module.GetModuleInstance(), pnmcc);

		if (!pnmcc->outControlHwnd) return HWND_DISCARD_CREATION;
		
		return 0;
	}

	virtual LRESULT CreateListView(HWND hwndParent, LPNMHL_CREATE_CONTROL pnmcc)
	{
		ATLASSERT(::IsWindow(hwndParent));
		ATLASSERT(pnmcc != NULL);
		
		// Control style table

		static STYLETABLE aStyles[] = {
			STYLETABENTRY(LVS_, ICON),
			STYLETABENTRY(LVS_, REPORT),
			STYLETABENTRY(LVS_, SMALLICON),
			STYLETABENTRY(LVS_, LIST),
			STYLETABENTRY(LVS_, SINGLESEL),
			STYLETABENTRY(LVS_, SHOWSELALWAYS),
			STYLETABENTRY(LVS_, SORTASCENDING),
			STYLETABENTRY(LVS_, SORTDESCENDING),
			STYLETABENTRY(LVS_, SHAREIMAGELISTS),
			STYLETABENTRY(LVS_, NOLABELWRAP),
			STYLETABENTRY(LVS_, AUTOARRANGE),
			STYLETABENTRY(LVS_, EDITLABELS),
		#if (_WIN32_IE >= 0x0300)
			STYLETABENTRY(LVS_, OWNERDATA),
		#endif
			STYLETABENTRY(LVS_, NOSCROLL),
			STYLETABENTRY(LVS_, ALIGNTOP),
			STYLETABENTRY(LVS_, ALIGNLEFT),
			STYLETABENTRY(LVS_, OWNERDRAWFIXED),
			STYLETABENTRY(LVS_, NOCOLUMNHEADER),
			STYLETABENTRY(LVS_, NOSORTHEADER),
			STYLETABLEEND
		};

		// Extended control style table

		static STYLETABLE aExStyles[] = {
			STYLETABENTRY(LVS_EX_, GRIDLINES),
			STYLETABENTRY(LVS_EX_, SUBITEMIMAGES),
			STYLETABENTRY(LVS_EX_, CHECKBOXES),
			STYLETABENTRY(LVS_EX_, TRACKSELECT),
			STYLETABENTRY(LVS_EX_, HEADERDRAGDROP),
			STYLETABENTRY(LVS_EX_, FULLROWSELECT),
			STYLETABENTRY(LVS_EX_, ONECLICKACTIVATE),
			STYLETABENTRY(LVS_EX_, TWOCLICKACTIVATE),
		#if (_WIN32_IE >= 0x0400)
			STYLETABENTRY(LVS_EX_, FLATSB),
			STYLETABENTRY(LVS_EX_, REGIONAL),
			STYLETABENTRY(LVS_EX_, INFOTIP),
			STYLETABENTRY(LVS_EX_, UNDERLINEHOT),
			STYLETABENTRY(LVS_EX_, UNDERLINECOLD),
			STYLETABENTRY(LVS_EX_, MULTIWORKAREAS),
		#endif
			#if (_WIN32_IE >= 0x0500)
			STYLETABENTRY(LVS_EX_, LABELTIP),
			STYLETABENTRY(LVS_EX_, BORDERSELECT),
		#endif
		#if (_WIN32_WINNT >= 0x501)
			STYLETABENTRY(LVS_EX_, DOUBLEBUFFER),
			STYLETABENTRY(LVS_EX_, HIDELABELS),
			STYLETABENTRY(LVS_EX_, SINGLEROW),
			STYLETABENTRY(LVS_EX_, SNAPTOGRID),
			STYLETABENTRY(LVS_EX_, SIMPLESELECT),
		#endif
			STYLETABLEEND
		};
		
		// Make up the control window style

		DWORD dwStyle = GetCtrlStyle(pnmcc) | GetCtrlStyle(pnmcc, aStyles);
		DWORD dwExStyle = GetCtrlExStyle(pnmcc) | GetCtrlStyle(pnmcc, aExStyles);

		// Create control and check if ok

		pnmcc->outControlHwnd = ::CreateWindowEx(
			dwExStyle, CListViewCtrl::GetWndClassName(), GetCtrlText(pnmcc), dwStyle, 
			0, 0, 0, 0, hwndParent, (HMENU)GetCtrlID(pnmcc), _Module.GetModuleInstance(), pnmcc);

		if (!pnmcc->outControlHwnd) return HWND_DISCARD_CREATION;

		return 0;
	}

	virtual LRESULT CreateTreeView(HWND hwndParent, LPNMHL_CREATE_CONTROL pnmcc)
	{
		ATLASSERT(::IsWindow(hwndParent));
		ATLASSERT(pnmcc != NULL);
		
		// Control style table

		static STYLETABLE aStyles[] = {
			STYLETABENTRY(TVS_, HASBUTTONS),
			STYLETABENTRY(TVS_, HASLINES),
			STYLETABENTRY(TVS_, LINESATROOT),
			STYLETABENTRY(TVS_, EDITLABELS),
			STYLETABENTRY(TVS_, DISABLEDRAGDROP),
			STYLETABENTRY(TVS_, SHOWSELALWAYS),
		#if (_WIN32_IE >= 0x0300)
			STYLETABENTRY(TVS_, RTLREADING),
			STYLETABENTRY(TVS_, NOTOOLTIPS),
			STYLETABENTRY(TVS_, CHECKBOXES),
			STYLETABENTRY(TVS_, TRACKSELECT),
		#if (_WIN32_IE >= 0x0400)
			STYLETABENTRY(TVS_, SINGLEEXPAND),
			STYLETABENTRY(TVS_, INFOTIP),
			STYLETABENTRY(TVS_, FULLROWSELECT),
			STYLETABENTRY(TVS_, NOSCROLL),
			STYLETABENTRY(TVS_, NONEVENHEIGHT),
		#endif
		#if (_WIN32_IE >= 0x500)
			STYLETABENTRY(TVS_, NOHSCROLL),
		#endif
		#endif
			STYLETABLEEND
		};



		// Make up the control window style

		DWORD dwStyle = GetCtrlStyle(pnmcc) | GetCtrlStyle(pnmcc, aStyles);
		DWORD dwExStyle = GetCtrlExStyle(pnmcc);

		// Create control and check if ok

		pnmcc->outControlHwnd = ::CreateWindowEx(
			dwExStyle, CTreeViewCtrl::GetWndClassName(), GetCtrlText(pnmcc), dwStyle, 
			0, 0, 0, 0, hwndParent, (HMENU)GetCtrlID(pnmcc), _Module.GetModuleInstance(), pnmcc);

		if (!pnmcc->outControlHwnd) return HWND_DISCARD_CREATION;
		
		return 0;
	}

	// Cleanup local defines

	#undef STYLETABENTRY
	#undef STYLETABLEEND

};

}; //namespace WTL

// Link against HTMLayout library

#pragma comment(lib, "htmlayout.lib")

#endif // __ATLHTMLAYOUT_H__
