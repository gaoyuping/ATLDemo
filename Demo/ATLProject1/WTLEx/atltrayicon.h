//
// Windows Template Library Extensions
//
// This file is NOT part of Windows Template Library.
// The code and information is provided "as-is" without
// warranty of any kind, either expressed or implied.
//
// History: 12/Mar/2000 kvitekp - Initially created
//

#ifndef __ATLTRAYICON_H__
#define __ATLTRAYICON_H__

#pragma once

#ifndef __cplusplus
	#error ATL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLAPP_H__
	#error atltrayicon.h requires atlapp.h to be included first
#endif

#ifndef __ATLWIN_H__
	#error atltrayicon.h requires atlwin.h to be included first
#endif

///////////////////////////////////////////////////////////////////////////////
// Classes in this file
//
// CTrayIcon<T>
//

namespace WTL
{

///////////////////////////////////////////////////////////////////////////////
// CTrayIcon class: MI class for CWindowImpl derived classes
///////////////////////////////////////////////////////////////////////////////

template <class T>
class CTrayIcon
{
public:
	
	///////////////////////////////////////////////////////////////////////////
	// Attributes

	UINT WM_TRAYICONEVENT;

	NOTIFYICONDATA m_nid;

	bool m_bTrayIconShown;

	///////////////////////////////////////////////////////////////////////////
	// Construction/destruction

	CTrayIcon()
	:	m_bTrayIconShown(false)
	{
		WM_TRAYICONEVENT = ::RegisterWindowMessage(_T("WM_TRAYICONEVENT"));

		ZeroMemory(&m_nid, sizeof(m_nid)); m_nid.cbSize = NOTIFYICONDATA_V2_SIZE; // Win2K
	}

	~CTrayIcon()
	{
		if (m_bTrayIconShown) DelTrayIcon();
	}

	///////////////////////////////////////////////////////////////////////////
	// Operations

	bool AddTrayIcon(UINT nID, HICON hIcon, LPCTSTR pszToolTip = NULL)
	{
		T* pT = static_cast<T*>(this);

		if (m_bTrayIconShown) DelTrayIcon(); ATLASSERT(!m_bTrayIconShown);

		ATLASSERT(hIcon != NULL);

		m_nid.hWnd = pT->m_hWnd;
		m_nid.uID = nID;
		m_nid.hIcon = hIcon;
		m_nid.uFlags = NIF_MESSAGE | NIF_ICON;
		m_nid.uCallbackMessage = WM_TRAYICONEVENT;

		if (pszToolTip) {
			lstrcpyn(m_nid.szTip, pszToolTip, sizeof(m_nid.szTip) / sizeof(TCHAR));
			m_nid.uFlags|= NIF_TIP;
		}

		if (Shell_NotifyIcon(NIM_ADD)) m_bTrayIconShown = true;
		
		return m_bTrayIconShown;
	}

	bool DelTrayIcon()
	{
		if (m_bTrayIconShown) 
			m_bTrayIconShown = false;
		else
			return false;
		
		m_nid.uFlags = 0;

		return Shell_NotifyIcon(NIM_DELETE);
	}

	bool SetTrayIcon(HICON hIcon)
	{
		if (!m_bTrayIconShown) return false;
		
		m_nid.hIcon = hIcon;
		m_nid.uFlags = NIF_ICON;

		return Shell_NotifyIcon(NIM_MODIFY);
	}

	bool SetTrayIconToolTip(LPCTSTR pszToolTip)
	{
		if (!m_bTrayIconShown) return false;
		
		m_nid.uFlags = NIF_TIP;

		if (!pszToolTip) pszToolTip = _T("");
		
		lstrcpyn(m_nid.szTip, pszToolTip, sizeof(m_nid.szTip) / sizeof(TCHAR));

		return Shell_NotifyIcon(NIM_MODIFY);
	}

	bool ShowTrayIconBalloon(LPCTSTR pszInfo, LPCTSTR pszInfoTitle, UINT flags = NIIF_INFO, UINT timeout = 0)
	{
		if (!m_bTrayIconShown) return false;
		
		m_nid.uFlags = NIF_INFO;
		m_nid.dwInfoFlags = flags;
		m_nid.uTimeout = timeout;

		lstrcpyn(m_nid.szInfo, pszInfo, sizeof(m_nid.szInfo) / sizeof(TCHAR));
		lstrcpyn(m_nid.szInfoTitle, pszInfoTitle, sizeof(m_nid.szInfoTitle) / sizeof(TCHAR));

		if (!Shell_NotifyIcon(NIM_MODIFY))
			return false;
		
		return true;
	}

	bool Shell_NotifyIcon(UINT cmd)
	{
		return ::Shell_NotifyIcon(cmd, &m_nid) ? true : false;
	}

	///////////////////////////////////////////////////////////////////////////
	// Message map
	
	BEGIN_MSG_MAP(CTrayIcon<T>)
		MESSAGE_HANDLER(WM_TRAYICONEVENT, OnTrayIconEvent)
	END_MSG_MAP()

	///////////////////////////////////////////////////////////////////////////
	// Message handlers
	
	LRESULT OnTrayIconEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
//		ATLTRACE(_T("CTrayIcon::OnTrayIconEvent: wParam=%u lParam=%08x\n"), wParam, lParam);

		if (wParam != m_nid.uID) return 0;

		T* pT = static_cast<T*>(this);

		switch (LOWORD(lParam)) {
			case WM_MOUSEMOVE: pT->OnTrayIconMouseMove(wParam, lParam); break;
			case WM_LBUTTONUP: pT->OnTrayIconLButtonUp(wParam, lParam); break;
			case WM_LBUTTONDOWN: pT->OnTrayIconLButtonDown(wParam, lParam); break;
			case WM_LBUTTONDBLCLK: pT->OnTrayIconLButtonDblClk(wParam, lParam); break;
			case WM_RBUTTONUP: pT->OnTrayIconRButtonUp(wParam, lParam); break;
			case WM_RBUTTONDOWN: pT->OnTrayIconRButtonDown(wParam, lParam); break;
			case WM_RBUTTONDBLCLK: pT->OnTrayIconRButtonDblClk(wParam, lParam); break;
		}
		
		return 0;
	}

	///////////////////////////////////////////////////////////////////////////
	// Overridables

	void OnTrayIconMouseMove(WPARAM wParam, LPARAM lParam) {}
	void OnTrayIconLButtonUp(WPARAM wParam, LPARAM lParam) {}
	void OnTrayIconLButtonDown(WPARAM wParam, LPARAM lParam) {}
	void OnTrayIconLButtonDblClk(WPARAM wParam, LPARAM lParam) {}
	void OnTrayIconRButtonUp(WPARAM wParam, LPARAM lParam) {}
	void OnTrayIconRButtonDown(WPARAM wParam, LPARAM lParam) {}
	void OnTrayIconRButtonDblClk(WPARAM wParam, LPARAM lParam) {}

};

}; //namespace WTL

#endif __ATLTRAYICON_H__
