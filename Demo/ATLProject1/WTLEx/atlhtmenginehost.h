//
// Windows Template Library Extension for
// Terra Informatica Lightweight Embeddable HTMLayout control
// http://terra-informatica.org/htmlayout
//
// Written by Pete Kvitek <pete@kvitek.com>

//
// This file is NOT part of Windows Template Library.
// The code and information is provided "as-is" without
// warranty of any kind, either expressed or implied.
//
// (C) 2003, Pete Kvitek <pete@kvitek.com>
//	     and Andrew Fedoniouk <andrew@TerraInformatica.com>
//

#ifndef __ATLHTMENGINEHOST_H__
#define __ATLHTMENGINEHOST_H__

#pragma once

#ifndef __cplusplus
	#error ATL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLAPP_H__
	#error ATLHTMENGINEHOST.h requires atlapp.h to be included first
#endif

#ifndef __ATLWIN_H__
	#error ATLHTMENGINEHOST.h requires atlwin.h to be included first
#endif

#include <atlmisc.h>
#include <atlctrls.h>

#ifdef _WIN32_WCE
#include <commctrl.h>
#define stricmp _stricmp
#else
#include <richedit.h>
#endif

#include <htmengine.h>

/////////////////////////////////////////////////////////////////////////////
// Classes in this file
//
// CHtmEngineHost<T>
//

namespace WTL
{

// Command name handler. Used primarily for handling button clicks
// Mimics COMMAND_ID_HANDLER but uses html defined control name instead of ID

#define COMMAND_NAME_HANDLER(name, func) \
	if(uMsg == WM_COMMAND) \
	{ \
		LPCTSTR ctlName = GetDlgItemName((HWND)lParam); \
		if(ctlName && _tcsicmp(ctlName,name) == 0) \
		{ \
			bHandled = TRUE; \
			lResult = func(HIWORD(wParam), LOWORD(wParam), (HWND)lParam, bHandled); \
			if(bHandled) return TRUE; \
		} \
	}

// HYPERLINK_HANDLER
// Handles clicks on hyperlinks in html document
// To be able to catch this notifications class should provide
// following implementation of
/*
	virtual LRESULT OnHyperLinkClick(LPNMHE_HYPERLINK pnmhl)
	{
		CString href(pnmhl->href);
		// send it as a command to the message map to
		// be able to catch it by HYPERLINK_HANDLER macro
		return SendMessage(WM_COMMAND,WPARAM(0xAFAF0000), LPARAM((LPCTSTR)href));
	}
*/

#define HYPERLINK_HANDLER(href, func) \
	if(uMsg == WM_COMMAND) \
	{ \
		LPCTSTR hrefstr = ((LPCTSTR)lParam); \
		if(wParam == 0xAFAF0000 && _tcsicmp(hrefstr,href) == 0) \
		{ \
			bHandled = TRUE; \
			lResult = func(hrefstr, bHandled); \
			if(bHandled) return TRUE; \
		} \
	}

// NMHE_CREATE_CONTROL wrapper

struct CREATE_CONTROL_PARAMS: public NMHE_CREATE_CONTROL
{
	int AttrIndex(LPCSTR name)
	{
		for(int i = 0; i < attributeCount; i++)
		if(_stricmp(name,attributeNames[i]) == 0) return i;
		return -1;
	}

	bool HasAttr(LPCSTR name)
	{
		return AttrIndex(name) >= 0;
	}

	LPCWSTR GetAttr(LPCSTR name, LPCWSTR defaultValue = 0)
	{
		int idx = AttrIndex(name);
		if(idx < 0) return defaultValue;
		return attributeValues[idx];
	}

	int GetAttrInt(LPCSTR name, int defaultValue = 0 )
	{
		LPCWSTR lpw = GetAttr(name);
		if(lpw) return _wtoi(lpw);
		return defaultValue;
	}

#ifdef UNICODE

	int AttrIndex(LPCWSTR tname)
	{
		USES_CONVERSION;
		LPCSTR name = W2CA(tname);
		for(int i = 0; i < attributeCount; i++)
		if(_stricmp(name,attributeNames[i]) == 0) return i;
		return -1;
	}

	bool HasAttr(LPCWSTR tname)
	{
		return AttrIndex(tname) >= 0;
	}

	LPCWSTR GetAttr(LPCWSTR tname, LPCWSTR defaultValue = 0)
	{
		int idx = AttrIndex(tname);
		if(idx < 0) return defaultValue;
		return attributeValues[idx];
	}

	int GetAttrInt(LPCWSTR tname, int defaultValue = 0 )
	{
		LPCWSTR lpw = GetAttr(tname);
		if(lpw) return _wtoi(lpw);
		return defaultValue;
	}

#endif // #ifdef UNICODE
};

/////////////////////////////////////////////////////////////////////////////
// CHtmEngineHost - host side implementation for a HTMEngine control

template <class T>
class CHtmEngineHost : public HTMENGINE_CALLBACK
{
public:

	// HTMLayout callback interface methods

	virtual LRESULT __stdcall notification(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return OnHtmlNotify(uMsg, wParam, lParam);
	}

	virtual LRESULT __stdcall child_ctl_notification(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return 0; //OnCtrlNotify(uMsg, wParam, lParam);
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
			case HEN_CREATE_CONTROL:    return OnCreateControl((CREATE_CONTROL_PARAMS*) lParam);
			case HEN_CONTROL_CREATED:   return OnControlCreated((CREATE_CONTROL_PARAMS*) lParam);
			case HEN_DESTROY_CONTROL:   return OnDestroyControl((LPNMHE_DESTROY_CONTROL) lParam);
			case HEN_HYPERLINK:         return OnHyperLink((LPNMHE_HYPERLINK) lParam);
			case HEN_LOAD_DATA:         return OnLoadData((LPNMHE_LOAD_DATA) lParam);
			case HEN_LOAD_DATA_ASYNC:   return OnLoadDataAsync((LPNMHE_LOAD_DATA_ASYNC)lParam);
			case HEN_DATA_LOADED:       return OnDataLoaded((LPNMHE_DATA_LOADED)lParam);
			case HEN_DOCUMENT_COMPLETE: return OnDocumentComplete();
			case HEN_REQUEST_RESIZE:    return OnResizeRequest((LPNMHE_REQUEST_RESIZE)lParam );
			case HEN_UPDATE_UI:         return OnUpdateUI((LPNMHE_UPDATE_UI)lParam );
			case HEN_HTML_INSERTED:     return OnHtmlInserted((LPNMHE_HTML_INSERTED)lParam );
			case HEN_SAVE_DATA:         return OnSaveData((LPNMHE_SAVE_DATA)lParam );
			case HEN_HTML_INSERTION:    return OnHtmlInsertion((LPNMHE_HTML_INSERTED)lParam );
			case HEN_SPELL_CHECK:		return OnSpellCheck((LPNMHE_SPELL_CHECK)lParam );
		}

		return OnHtmlGenericNotifications(uMsg,wParam,lParam);
	}

	virtual LRESULT OnHtmlGenericNotifications(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		// all generic notifications
		// are coming to the parent of HTMEngine
		T* pT = static_cast<T*>(this);

		ATLASSERT(::IsWindow(pT->m_hWnd));
		// Pass it to the parent window if any
		HWND hWndParent = pT->GetParent();
		if (!hWndParent) return 0;

		return ::SendMessage(hWndParent, uMsg, wParam, lParam);
	}

	virtual LRESULT OnCreateControl(CREATE_CONTROL_PARAMS* pnmcc)
	{
		ATLTRACE(_T("CHtmEngineHost::OnCreateControl: type='%s' name='%s' id='%s' tagType=%u\n"), CString(pnmcc->type), CString(pnmcc->name), CString(pnmcc->id), pnmcc->tagType);

		// Try to create control and if failed, proceed with default processing.
		// Note that this code assumes that the host and control windows are the same. If
		// you are handling HTMLayout control notification in another window, you'll have
		// to override this method and provide proper hWnd.

		T* pT = static_cast<T*>(this);

		ATLASSERT(::IsWindow(pT->m_hWnd));

		return CreateControl(pT->m_hWnd, pnmcc);
	}

	virtual LRESULT OnControlCreated(CREATE_CONTROL_PARAMS* pnmcc)
	{
		ATLTRACE(_T("CHtmEngineHost::OnControlCreated: type='%s' name='%s' id='%s' tagType=%u\n"), CString(pnmcc->type), CString(pnmcc->name), CString(pnmcc->id), pnmcc->tagType);

		return 0;
	}

	virtual LRESULT OnDestroyControl(LPNMHE_DESTROY_CONTROL pnmhl)
	{
		ATLTRACE(_T("CHtmEngineHost::OnDestroyControl: HWND=%x\n"), pnmhl->inoutControlHwnd);

		// Use this if you don't want this child to be destroyed:
		// pnmhl->inoutControlHwnd = 0;

		// If you will not change pnmhl->inoutControlHwnd field then HTMEngine
		// will call ::DestroyWindow by itself.

		return 0;
	}

	virtual LRESULT OnHyperLink(LPNMHE_HYPERLINK pnmhl)
	{
		ATLTRACE(_T("CHtmEngineHost::OnHyperLink: name='%s' href='%s' target='%s' status=%u\n"), CString(pnmhl->name), CString(pnmhl->href), CString(pnmhl->target), pnmhl->status);

		switch (pnmhl->status) {
			case HE_HYPERLINK_ENTER: return OnHyperLinkEnter(pnmhl);
			case HE_HYPERLINK_LEAVE: return OnHyperLinkLeave(pnmhl);
			case HE_HYPERLINK_CLICK: return OnHyperLinkClick(pnmhl);
		}

		return 0;
	}

	virtual LRESULT OnHyperLinkEnter(LPNMHE_HYPERLINK pnmhl)
	{
		ATLTRACE(_T("CHtmEngineHost::OnHyperLinkEnter: name='%s' href='%s' target='%s' status=%u\n"), CString(pnmhl->name), CString(pnmhl->href), CString(pnmhl->target), pnmhl->status);
		return 0;
	}

	virtual LRESULT OnHyperLinkLeave(LPNMHE_HYPERLINK pnmhl)
	{
		ATLTRACE(_T("CHtmEngineHost::OnHyperLinkLeave: name='%s' href='%s' target='%s' status=%u\n"), CString(pnmhl->name), CString(pnmhl->href), CString(pnmhl->target), pnmhl->status);
		return 0;
	}

	virtual LRESULT OnHyperLinkClick(LPNMHE_HYPERLINK pnmhl)
	{
		ATLTRACE(_T("CHtmEngineHost::OnHyperLinkClick: name='%s' href='%s' target='%s' status=%u\n"), CString(pnmhl->name), CString(pnmhl->href), CString(pnmhl->target), pnmhl->status);
		return 0;
	}

	virtual LRESULT OnHtmlInserted(LPNMHE_HTML_INSERTED pnmhl)
	{
		ATLTRACE(_T("CHtmEngineHost::OnHtmlInserted: HTML has been pasted from: url='%s' title='%s'\n"), CString(pnmhl->uri), CString(pnmhl->title));
		return 0;
	}

	virtual LRESULT OnHtmlInsertion(LPNMHE_HTML_INSERTED pnmhl)
	{
		ATLTRACE(_T("CHtmEngineHost::OnHtmlInsertion: Is about to paste HTML from: url='%s' title='%s'\n"), CString(pnmhl->uri), CString(pnmhl->title));
		// pnmhl->reject = TRUE; // use this if you want to reject the operation
		return 0;
	}

	virtual LRESULT OnLoadData(LPNMHE_LOAD_DATA pnmld)
	{
		ATLTRACE(_T("CHtmEngineHost::OnLoadData: uri='%s'\n"), CString(pnmld->uri));

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

	virtual LRESULT OnLoadDataAsync(LPNMHE_LOAD_DATA_ASYNC pnmlda)
	{
		ATLTRACE(_T("CHtmEngineHost::OnLoadDataAsync: uri='%s'\n"), CString(pnmlda->uri));
		return 0;
	}

	virtual LRESULT OnDataLoaded(LPNMHE_DATA_LOADED pnmld)
	{
		ATLTRACE(_T("CHtmEngineHost::OnDataLoaded: uri='%s'\n"), CString(pnmld->uri));
		return 0;
	}

	virtual LRESULT OnSaveData(LPNMHE_SAVE_DATA pnmld)
	{
		ATLTRACE(_T("CHtmEngineHost::OnSaveData: uri='%s'\n"), CString(pnmld->uriIn));

		// use something like this:
		//    wcscpy(pnmld->uriOut,L"db:image-record-id");
		// if you want to change resource location.
		return 0;
	}

	virtual LRESULT OnUpdateUI(LPNMHE_UPDATE_UI pnmld)
	{
		//ATLTRACE(_T("CHtmEngineHost::OnUpdateUI: anchor=%d,%d%,%d,%d caret=%d,%d,%d,%d\n"),
		//	pnmld->coorAnchor.left, pnmld->coorAnchor.top, pnmld->coorAnchor.right, pnmld->coorAnchor.bottom,
		//	pnmld->coorCaret.left, pnmld->coorCaret.top, pnmld->coorCaret.right, pnmld->coorCaret.bottom);

		return 0;
	}

	virtual LRESULT OnDocumentComplete()
	{
		ATLTRACE(_T("CHtmEngineHost::OnDocumentComplete\n"));

		return 0;
	}

	virtual LRESULT OnResizeRequest( LPNMHE_REQUEST_RESIZE pnmld)
	{
		ATLTRACE(_T("CHtmEngineHost::OnResizeRequest\n"));

		// Default implementation.
		// Send EN_REQUESTRISZE to the parent.

		T* pT = static_cast<T*>(this);

		ATLASSERT(::IsWindow(pT->m_hWnd));

		// Pass it to the parent window if any

		HWND hWndParent = pT->GetParent();
		if (!hWndParent) return 0;

		return ::SendMessage(hWndParent, WM_NOTIFY, pnmld->nmhdr.idFrom, LPARAM(pnmld));
	}

	virtual LRESULT OnSpellCheck(LPNMHE_SPELL_CHECK pnmsch)
	{
		CString str; str.Format(_T("CHtmEngineHost::OnSpellCheck [%s]\n"), CString(pnmsch->word, pnmsch->wordlen));
		ATLTRACE(str);

		return true;
	}

	// Declare module manager that will free loaded module upon exit

	struct Module
	{
		HMODULE hModule;
		inline Module() : hModule(0) {}
		inline ~Module() { if (hModule) ::FreeLibrary(hModule); }
		HMODULE Load(LPCTSTR pszModule, DWORD flags) { return hModule = ::LoadLibraryEx(pszModule, 0, flags); }
		operator HMODULE() const { return hModule; }
	};

	// Load Data helper routines

	#ifndef INVALID_FILE_ATTRIBUTES
	#define INVALID_FILE_ATTRIBUTES 0xFFFFFFFF
	#endif

	static LRESULT LoadResourceData(HWND hWnd, LPNMHE_LOAD_DATA pnmld)
	{
		USES_CONVERSION;

		ATLASSERT(::IsWindow(hWnd));
		ATLASSERT(pnmld != NULL);

		// Check for trivial case

		if (!pnmld->uri || !pnmld->uri[0]) return LOAD_DISCARD;

		// Retrieve url specification into a local storage since FindResource() expects
		// to find its parameters on stack rather then on the heap under Win9x/Me

		TCHAR achURL[MAX_PATH]; lstrcpyn(achURL, W2CT(pnmld->uri), MAX_PATH);

		Module module;

		// Separate name and handle external resource module specification

		LPTSTR psz, pszName = achURL;
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

		HRSRC hrsrc;
#ifdef _WIN32_WCE
		hrsrc = ::FindResource(module, pszName, pszExt);
#else
		if (!pszExt || _tcsicmp(pszExt, _T("HTML")) == 0)
			hrsrc = ::FindResource(module, pszName, RT_HTML);
		else
			hrsrc = ::FindResource(module, pszName, pszExt);
#endif
		if (!hrsrc) return LOAD_TRY_DEFAULT;

		// Load specified resource and check if ok

		HGLOBAL hgres = ::LoadResource(module, hrsrc);
		if (!hgres) return LOAD_DISCARD;

		// Retrieve resource data and check if ok

		PBYTE pb = (PBYTE)::LockResource(hgres); if (!pb) return LOAD_DISCARD;
		DWORD cb = ::SizeofResource(module, hrsrc); if (!cb) return LOAD_DISCARD;

		// Report data ready

		if (!::HTMEngineDataReady(hWnd, pnmld->uri, pb,  cb)) return LOAD_DISCARD;

		return LOAD_DATA_VALID;
	}

	LRESULT LoadResourceData(LPNMHE_LOAD_DATA pnmld)
	{
		// This code assumes that the host and control windows are the same

		T* pT = static_cast<T*>(this);

		ATLASSERT(::IsWindow(pT->m_hWnd));

		return LoadResourceData(pT->m_hWnd, pnmld);
	}

	virtual LRESULT CreateControl(HWND hWnd, CREATE_CONTROL_PARAMS* pnmcc)
	{
		ATLASSERT(::IsWindow(hWnd));
		ATLASSERT(pnmcc != NULL);

		// Create control of the specified type

		if (!wcscmp(pnmcc->type, L"richedit")) return CreateRichEdit(hWnd, pnmcc);
		if (!wcscmp(pnmcc->type, L"datetime")) return CreateDateTime(hWnd, pnmcc);
		if (!wcscmp(pnmcc->type, L"calendar")) return CreateCalendar(hWnd, pnmcc);
		if (!wcscmp(pnmcc->type, L"listview")) return CreateListView(hWnd, pnmcc);
		if (!wcscmp(pnmcc->type, L"treeview")) return CreateTreeView(hWnd, pnmcc);
		if (!wcscmp(pnmcc->type, L"htmledit")) return CreateHtmlEdit(hWnd, pnmcc);

		return 0;

	}

	// Style table declarations

	#define STYLETABENTRY(prefix, style) { prefix##style, #style }
	#define STYLETABLEEND { 0, NULL }

	typedef struct _STYLETABLE {
		DWORD	dwStyle;
		LPCSTR	pszStyle;
	} STYLETABLE;

	static DWORD GetCtlStyle(CREATE_CONTROL_PARAMS* pnmcc, const STYLETABLE* pStyles, DWORD dwDefStyle = 0)
	{
		ATLASSERT(pnmcc != NULL);

		// Scan specified style table collecting styles

		DWORD dwStyle = 0;

		for (; pStyles->pszStyle != NULL; pStyles++) {
			if (pnmcc->HasAttr(pStyles->pszStyle)) {
				dwStyle|= pStyles->dwStyle;
			}
		}

		// Return collected styles or default style if none specified

		return dwStyle ? dwStyle : dwDefStyle;
	}

	// Generic window style handling

	static DWORD GetCtlStyle(CREATE_CONTROL_PARAMS* pnmcc, DWORD dwDefStyle = WS_CHILD | WS_TABSTOP | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN)
	{
		// Window style table

		static STYLETABLE aStyles[] = {
			STYLETABENTRY(WS_, OVERLAPPED),
			STYLETABENTRY(WS_, POPUP),
			STYLETABENTRY(WS_, CHILD),
			STYLETABENTRY(WS_, VISIBLE),
			STYLETABENTRY(WS_, DISABLED),
			STYLETABENTRY(WS_, CLIPSIBLINGS),
			STYLETABENTRY(WS_, CLIPCHILDREN),
#ifndef _WIN32_WCE
			STYLETABENTRY(WS_, MAXIMIZE),
			STYLETABENTRY(WS_, MINIMIZE),
#endif // #ifndef _WIN32_WCE
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

		return GetCtlStyle(pnmcc, aStyles, dwDefStyle);
	}

	static DWORD GetCtlExStyle(CREATE_CONTROL_PARAMS* pnmcc, DWORD dwDefExStyle = 0)
	{
		// Extended window style table
#ifdef _WIN32_WCE
		return dwDefExStyle;
#else
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
		#if (_WIN32_WINNT >= 0x0501)
			STYLETABENTRY(WS_EX_, COMPOSITED),
		#endif
		#if (_WIN32_WINNT >= 0x0500)
			STYLETABENTRY(WS_EX_, NOACTIVATE),
		#endif
			STYLETABLEEND
		};
		return GetCtlStyle(pnmcc, aStyles, dwDefExStyle);
#endif // #ifndef _WIN32_WCE
	}

	// Control factory routines

	virtual LRESULT CreateRichEdit(HWND hwndParent, CREATE_CONTROL_PARAMS* pnmcc)
	{
#ifndef _RICHEDIT_
		return 0;
#else
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

		// Default richedit style

		const DWORD dwDefStyle = ES_MULTILINE
							   | ES_WANTRETURN
//							   | ES_AUTOVSCROLL
//							   | ES_AUTOHSCROLL
							   ;

		// Make up the control window style

		DWORD dwStyle = GetCtlStyle(pnmcc) | GetCtlStyle(pnmcc, aStyles, dwDefStyle);
		DWORD dwExStyle = GetCtlExStyle(pnmcc);

		// Create control and check if ok

		pnmcc->outControlHwnd = ::CreateWindowEx(
			dwExStyle, CRichEditCtrl::GetWndClassName(), NULL, dwStyle,
			0, 0, 0, 0, hwndParent, HMENU(pnmcc->GetAttrInt("id")), _Module.GetModuleInstance(), pnmcc);

		if (!pnmcc->outControlHwnd) {
			pnmcc->outControlHwnd = HWND_DISCARD_CREATION;
			return 0;
		}

		// Handle bottomless RichEdit controls

		if (pnmcc->HasAttr("autoheight")) {
			::SendMessage(pnmcc->outControlHwnd, EM_SETEVENTMASK, 0, ENM_REQUESTRESIZE);
		}

		return 0;
#endif // #ifdef _RICHEDIT_
	}

	virtual LRESULT CreateHtmlEdit(HWND hwndParent, CREATE_CONTROL_PARAMS* pnmcc)
	{
		ATLASSERT(::IsWindow(hwndParent));
		ATLASSERT(pnmcc != NULL);

		// Make up the control window style

		DWORD dwStyle = GetCtlStyle(pnmcc);
		DWORD dwExStyle = GetCtlExStyle(pnmcc);

		// Create control and check if ok

		pnmcc->outControlHwnd = ::CreateWindowEx(
			dwExStyle, CHtmEngineCtrl::GetWndClassName(), NULL, dwStyle,
			0, 0, 0, 0, hwndParent, HMENU(pnmcc->GetAttrInt("id")), _Module.GetModuleInstance(), pnmcc);

		if (!pnmcc->outControlHwnd) {
			pnmcc->outControlHwnd = HWND_DISCARD_CREATION;
			return 0;
		}

		CHtmEngineEditorCtrl ctl = pnmcc->outControlHwnd;

		// Set inital text with 1 pix margins

		static const char achText[] = "<html><body leftmargin=1 topmargin=1 rightmargin=1 bottommargin=1>&nbsp;</body></html>";
		ctl.LoadHtml((LPCBYTE)achText,sizeof(achText));

		// Handle attributes

		if (pnmcc->HasAttr("autoheight"))
			ctl.SetBottomlessMode(true);
		else
			ctl.SetBottomlessMode(false);

		if (pnmcc->HasAttr("readonly"))
			ctl.SetEditMode(HEM_READ_ONLY);
		else
			ctl.SetEditMode(HEM_EDITOR);

		return 0;
	}

	virtual LRESULT CreateDateTime(HWND hwndParent, CREATE_CONTROL_PARAMS* pnmcc)
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

		DWORD dwStyle = GetCtlStyle(pnmcc) | GetCtlStyle(pnmcc, aStyles);
		DWORD dwExStyle = GetCtlExStyle(pnmcc);

		// Create control and check if ok

		pnmcc->outControlHwnd = ::CreateWindowEx(
			dwExStyle, CDateTimePickerCtrl::GetWndClassName(), NULL, dwStyle,
			0, 0, 0, 0, hwndParent, HMENU(pnmcc->GetAttrInt("id")), _Module.GetModuleInstance(),
			pnmcc);

		if (!pnmcc->outControlHwnd)
			pnmcc->outControlHwnd = HWND_DISCARD_CREATION;

		return 0;
	}

	virtual LRESULT CreateCalendar(HWND hwndParent, CREATE_CONTROL_PARAMS* pnmcc)
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

		DWORD dwStyle = GetCtlStyle(pnmcc) | GetCtlStyle(pnmcc, aStyles);
		DWORD dwExStyle = GetCtlExStyle(pnmcc);

		// Create control and check if ok

		pnmcc->outControlHwnd = ::CreateWindowEx(
			dwExStyle, CMonthCalendarCtrl::GetWndClassName(), NULL, dwStyle,
			0, 0, 0, 0, hwndParent, HMENU(pnmcc->GetAttrInt("id")), _Module.GetModuleInstance(),
			pnmcc);

		if (!pnmcc->outControlHwnd)
			pnmcc->outControlHwnd = HWND_DISCARD_CREATION;

		return 0;
	}

	virtual LRESULT CreateListView(HWND hwndParent, CREATE_CONTROL_PARAMS* pnmcc)
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
#ifndef _WIN32_WCE
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
#endif // #ifndef _WIN32_WCE
			STYLETABLEEND
		};

		// Make up the control window style

		DWORD dwStyle = GetCtlStyle(pnmcc) | GetCtlStyle(pnmcc, aStyles);
		DWORD dwExStyle = GetCtlExStyle(pnmcc) | GetCtlStyle(pnmcc, aExStyles);

		// Create control and check if ok

		pnmcc->outControlHwnd = ::CreateWindowEx(
			dwExStyle, CListViewCtrl::GetWndClassName(), NULL, dwStyle,
			0, 0, 0, 0, hwndParent, HMENU(pnmcc->GetAttrInt("id")), _Module.GetModuleInstance(),
			pnmcc);

		if (!pnmcc->outControlHwnd)
			pnmcc->outControlHwnd = HWND_DISCARD_CREATION;

		return 0;
	}

	virtual LRESULT CreateTreeView(HWND hwndParent, CREATE_CONTROL_PARAMS* pnmcc)
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
		#endif
		#if (_WIN32_IE >= 0x0400)
			STYLETABENTRY(TVS_, SINGLEEXPAND),
#ifndef _WIN32_WCE
			STYLETABENTRY(TVS_, INFOTIP),
			STYLETABENTRY(TVS_, FULLROWSELECT),
			STYLETABENTRY(TVS_, NOSCROLL),
			STYLETABENTRY(TVS_, NONEVENHEIGHT),
#endif // #ifndef _WIN32_WCE
		#endif
		#if (_WIN32_IE >= 0x500)
			STYLETABENTRY(TVS_, NOHSCROLL),
		#endif
			STYLETABLEEND
		};



		// Make up the control window style

		DWORD dwStyle = GetCtlStyle(pnmcc) | GetCtlStyle(pnmcc, aStyles);
		DWORD dwExStyle = GetCtlExStyle(pnmcc);

		// Create control and check if ok

		pnmcc->outControlHwnd = ::CreateWindowEx(
			dwExStyle, CTreeViewCtrl::GetWndClassName(), NULL, dwStyle,
			0, 0, 0, 0, hwndParent, HMENU(pnmcc->GetAttrInt("id")), _Module.GetModuleInstance(),
			pnmcc);

		if (!pnmcc->outControlHwnd)
			pnmcc->outControlHwnd = HWND_DISCARD_CREATION;

		return 0;
	}

	// Cleanup local defines

	#undef STYLETABENTRY
	#undef STYLETABLEEND

};

}; //namespace WTL

#endif // __ATLHTMENGINEHOST_H__
