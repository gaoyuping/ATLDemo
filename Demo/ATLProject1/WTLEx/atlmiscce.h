//
// Windows Template Library Extensions
//
// This file is NOT part of Windows Template Library.
// The code and information is provided "as-is" without
// warranty of any kind, either expressed or implied.
//
// History: 22/Jun/2005 sgorshkov - Initially created
//

#ifndef __ATLMISCCE_H__
#define __ATLMISCCE_H__

#pragma once

#ifndef __cplusplus
	#error ATL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLAPP_H__
	#error atlmiscce.h requires atlapp.h to be included first
#endif

#ifndef __ATLWIN_H__
	#error atlmiscce.h requires atlwin.h to be included first
#endif

/////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CMenubar
// CLabel
// CStaticBtn
// CRecentDocumentListBase
// CSeparator
// CDlgHeader
//

namespace WTL
{

/////////////////////////////////////////////////////////////////////////////
// CMenubar - client side for a Windows Mobile for Smartphone MENUBAR control

struct MBBUTTON
{
	CString name;
	int nCmd;
};

class CMenubar
{
public:

	enum SoftButton {
		sbLeft  = 0,
		sbRight = 1,
	};

	///////////////////////////////////////////////////////////////////////////
	// Attributes

	HWND m_hWnd;				// Menubar handle
	HWND m_hWndParent;			// Parent window handle
	UINT m_nToolBar;			// Toolbar resource id

	///////////////////////////////////////////////////////////////////////////
	// Construction/destruction

	CMenubar()
	:	m_hWnd(NULL)
	,	m_hWndParent(NULL)
	,	m_nToolBar(0)
	{
	}

	~CMenubar()
	{
	}

	///////////////////////////////////////////////////////////////////////////
	// Operations

	bool Create(HWND hWndParent, UINT nToolBar)
	{
		ASSERT(hWndParent != NULL);
		ASSERT(m_hWnd == NULL);

		// Create menubar

		SHMENUBARINFO mbi; memset(&mbi, 0, sizeof(mbi));
		mbi.cbSize     = sizeof(mbi);
		mbi.hwndParent = hWndParent;
		mbi.nToolBarId = nToolBar;
		mbi.hInstRes   = _Module.GetResourceInstance();
		mbi.dwFlags |= SHCMBF_HMENU;

		if(!SHCreateMenuBar(&mbi))
			return false;

		// Set attributes

		m_hWnd       = mbi.hwndMB;
		m_hWndParent = hWndParent;
		m_nToolBar   = nToolBar;

		return true;
	}

	void DestroyWindow()
	{
		ASSERT(m_hWnd != NULL);
		BOOL bRet = ::DestroyWindow(m_hWnd);
		if (bRet)
			m_hWnd = NULL;
	}

	void OverrideKey(UINT nKey)
	{
		ASSERT(m_hWnd != NULL);
		::SendMessage(m_hWnd, SHCMBM_OVERRIDEKEY, nKey, MAKELPARAM (SHMBOF_NODEFAULT | SHMBOF_NOTIFY, SHMBOF_NODEFAULT | SHMBOF_NOTIFY));
	}

	operator HWND() const { return m_hWnd; }

	LRESULT SendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		ASSERT(m_hWnd != NULL);
		return ::SendMessage(m_hWnd, uMsg, wParam, lParam);
	}

	BOOL ShowWindow(int nCmdShow)
	{
		ASSERT(m_hWnd != NULL);
		return ::ShowWindow(m_hWnd, nCmdShow);
	}

	HMENU GetMenu()
	{
		return (HMENU) ::SendMessage(m_hWnd, SHCMBM_GETMENU, 0, 0);
	}

	HMENU GetSubMenu(UINT nMenuID)
	{
		return (HMENU) ::SendMessage(m_hWnd, SHCMBM_GETSUBMENU, 0, (LPARAM) nMenuID);
	}

	// Menu item operations

	bool DeleteItem(UINT nButton, UINT nCode)
	{
		ASSERT(m_hWnd != NULL);

		// Get menu handle

		HMENU hMenu = (HMENU) ::SendMessage(m_hWnd, SHCMBM_GETMENU, 0, 0);
		if (hMenu == NULL)
			return false;

		// Delete menu item

		HMENU hSubMenu = ::GetSubMenu(hMenu, nButton);

		int nPos;
		if (!GetMenuItemPos(hSubMenu, nCode, nPos))
			return false;

		if (!DeleteMenu(hSubMenu, nPos, MF_BYPOSITION))
			return false;

		return true;
	}

	bool EnableItem(UINT nButton, UINT nCode, bool bEnable = true)
	{
		ASSERT(m_hWnd != NULL);

		// Get menu handle

		HMENU hMenu = (HMENU) ::SendMessage(m_hWnd, SHCMBM_GETMENU, 0, 0);
		if (hMenu == NULL)
			return false;

		// Delete menu item

		HMENU hSubMenu = ::GetSubMenu(hMenu, nButton);

		UINT fEnable = bEnable ? MF_ENABLED : MF_GRAYED;
		if (!::EnableMenuItem(hMenu, nCode, fEnable | MF_BYCOMMAND))
			return false;

		return true;
	}

	bool ModifyItem(UINT nButton, UINT nCode, UINT nNewCode, LPCTSTR pszName)
	{
		ASSERT(m_hWnd != NULL);

		// Get menu handle

		HMENU hMenu = (HMENU) ::SendMessage(m_hWnd, SHCMBM_GETMENU, 0, 0);
		if (hMenu == NULL)
			return false;

		// Get submenu

		HMENU hSubMenu = ::GetSubMenu(hMenu, nButton);
		if (hSubMenu == NULL)
			return false;

		return ModifyMenuItem(hSubMenu, nCode, nNewCode, pszName);
	}

	bool SetButton(int nCmd, MBBUTTON* pMBButton)
	{
		TBBUTTONINFO tbinfo; memset(&tbinfo, 0, sizeof(tbinfo));
		tbinfo.cbSize = sizeof(tbinfo);
		tbinfo.dwMask = TBIF_COMMAND | TBIF_TEXT;
		tbinfo.idCommand = pMBButton->nCmd;
		tbinfo.pszText = (LPTSTR)(LPCTSTR) pMBButton->name;
		tbinfo.cchText = pMBButton->name.GetLength();

		return !!::SendMessage(m_hWnd, TB_SETBUTTONINFO, nCmd, (LPARAM)&tbinfo);
	}

	static int GetMenuItemID(HMENU hMenu, int nPos)
	{
		MENUITEMINFO mii; memset(&mii, 0, sizeof(mii));
		mii.cbSize = sizeof(mii);
		mii.fMask  = MIIM_ID;
		if (!GetMenuItemInfo(hMenu, nPos, TRUE, &mii))
			return -1;

		return mii.wID;
	}

	static bool GetMenuItemPos(HMENU hMenu, int nCode, int& nPos)
	{
		int nMax = 9, nCount = 0;
		while (nCount < nMax) {
			if (nCode == GetMenuItemID(hMenu, nCount)) {
				nPos = nCount;
				return true;
			}

			nCount++;
		}

		return false;
	}

	static bool ModifyMenuItem(HMENU hMenu, UINT nCode, UINT nNewCode, LPCTSTR pszName)
	{
		if (hMenu == NULL)
			return false;

		// Get item pos to be modified

		int nPos;
		if (!GetMenuItemPos(hMenu, nCode, nPos))
			return false;

		// Delete item

		if (!DeleteMenu(hMenu, nPos, MF_BYPOSITION))
			return false;

		// Add new item

		if (!InsertMenu(hMenu, nPos, MF_BYPOSITION | MF_STRING, nNewCode, pszName))
			return false;

		return true;
	}

	static bool InsertMenuItem(HMENU hMenu, UINT nPosInsert, UINT nCode, LPCTSTR pszName)
	{
		if (hMenu == NULL)
			return false;

		if (!InsertMenu(hMenu, nPosInsert, MF_BYPOSITION | MF_STRING, nCode, pszName))
			return false;

		return true;
	}

	static bool EnableMenuItem(HMENU hMenu, UINT nCode, BOOL bEnable = TRUE)
	{
		// Get item pos to be modified

		int nPos;
		if (!GetMenuItemPos(hMenu, nCode, nPos))
			return false;

		// Get menuitem info

		UINT fEnable = bEnable ? MF_ENABLED : MF_GRAYED;
		if (!::EnableMenuItem(hMenu, nCode, fEnable | MF_BYCOMMAND))
			return false;

		return true;
	}

	bool EnableButton(UINT nCmd, BOOL bEnable = TRUE)
	{
		TBBUTTONINFO tbinfo; memset(&tbinfo, 0, sizeof(tbinfo));
		tbinfo.cbSize = sizeof(tbinfo);
		tbinfo.dwMask = TBIF_STATE;
		tbinfo.fsState = bEnable ? TBSTATE_ENABLED : TBSTATE_INDETERMINATE;
		return !!::SendMessage(m_hWnd, TB_SETBUTTONINFO, nCmd, (LPARAM)&tbinfo);
	}

};

/////////////////////////////////////////////////////////////////////////////
// CLabel - static control with ability to set font face, weight and italic

typedef CWinTraits<
	WS_CHILD | WS_VISIBLE,
	0> CLabelTraits;

class CLabel : public CWindowImpl<CLabel, CStatic, CLabelTraits>
{
public:

	///////////////////////////////////////////////////////////////////////////
	// Attributes

	CFont m_font;

	// Font settings

	CString m_face;
	UINT m_pt;
	bool m_bBold;
	bool m_bItalic;
	COLORREF m_clr;

	///////////////////////////////////////////////////////////////////////////
	// Construction/destruction

	CLabel(LPCTSTR pszFace = NULL, UINT pt = 0, bool bBold = false, bool bItalic = false, COLORREF clr = RGB(0, 0, 0))
	:	m_face(pszFace)
	,	m_pt(pt)
	,	m_bBold(bBold)
	,	m_bItalic(bItalic)
	,	m_clr(clr)
	{
	}

	~CLabel()
	{
	}

	///////////////////////////////////////////////////////////////////////////
	// Operations

	bool InitFont(HFONT hFont = NULL)
	{
		CPaintDC dc(m_hWnd);

		// Get curent font settings

		HFONT hOldFont;
		if (hFont)
			hOldFont = hFont;
		else
			hOldFont = GetFont(); 

		LOGFONT logFont; memset(&logFont, 0, sizeof(logFont));
		if (hOldFont) {
		
			CFontHandle oldFont(hOldFont);
			oldFont.GetLogFont(&logFont);

			// Set new font settings

			if (m_face.GetLength() > 0)
				_tcscpy(logFont.lfFaceName, (LPCTSTR) m_face);

			logFont.lfWeight = (LONG)(m_bBold ? FW_BOLD : FW_NORMAL);
			logFont.lfItalic = (BYTE)(m_bItalic ? TRUE : FALSE);
			if (m_pt) logFont.lfHeight = -::MulDiv(m_pt, GetDeviceCaps(dc, LOGPIXELSY), 72);
		}
		else {

			if (m_face.GetLength() > 0)
				_tcscpy(logFont.lfFaceName, (LPCTSTR) m_face);

			logFont.lfWeight = (LONG)(m_bBold ? FW_BOLD : FW_NORMAL);
			logFont.lfItalic = (BYTE)(m_bItalic ? TRUE : FALSE);
			if (m_pt) logFont.lfHeight = -::MulDiv(m_pt, GetDeviceCaps(dc, LOGPIXELSY), 72);
		}

		// Create new font

		if (m_font) m_font.DeleteObject();
		m_font.CreateFontIndirect(&logFont);

		// Set new font

		SetFont(m_font);

		// Set font color

		dc.SetTextColor(m_clr);

		InvalidateRect(NULL);

		return true;
	}

	bool Init()
	{
		InitFont();

		return true;
	}

	///////////////////////////////////////////////////////////////////////////
	// Message map

	BEGIN_MSG_MAP(CLabel)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
	END_MSG_MAP()

	///////////////////////////////////////////////////////////////////////////
	// Message handlers

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// Let default window procedure do its chores

		LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);

		// Init font

		InitFont();

		return lRes;
	}

};

/////////////////////////////////////////////////////////////////////////////
// CStaticBtn - static control with button behavior

typedef CWinTraits<
	WS_CHILD | WS_VISIBLE | WS_TABSTOP,
	0> CStaticBtnTraits;

class CStaticBtn : public CWindowImpl<CStaticBtn, CStatic, CStaticBtnTraits>
{
public:

	///////////////////////////////////////////////////////////////////////////
	// Attributes

	CString m_title;

	// Graphics objects

	CFont m_fntNormal;
	CFont m_fntUnderlined;

	// Command notification code

	UINT m_nCode;

	///////////////////////////////////////////////////////////////////////////
	// Construction/destruction

	CStaticBtn(UINT nCode)
	:	m_nCode(nCode)
	{
	}

	~CStaticBtn()
	{
	}

	///////////////////////////////////////////////////////////////////////////
	// Operations

	// Initializations

	bool InitFonts()
	{
		// Get curent font

		HFONT hOldFont = GetFont(); ASSERT(hOldFont != NULL);
		CFontHandle oldFont(hOldFont);
		LOGFONT logFont; memset(&logFont, 0, sizeof(logFont));
		oldFont.GetLogFont(&logFont);

		// Create normal font

		if (m_fntNormal.m_hFont == NULL)
			if (!m_fntNormal.CreateFontIndirect(&logFont))
				return false;

		// Create underlined font

		logFont.lfUnderline = TRUE;
		if (m_fntUnderlined.m_hFont == NULL)
			if (!m_fntUnderlined.CreateFontIndirect(&logFont))
				return false;

		return true;
	}

	bool Init()
	{
		// Initialize

		VERIFY(InitFonts());

		if (::GetFocus() == m_hWnd)
			SetUnderlinedFont(true);
		else
			SetUnderlinedFont(false);

		// Update window

		InvalidateRect(NULL);

		return true;
	}

	// Operations

	void SetText(const CString& text)
	{
		CString btnText = text;

		// Get dc

		CPaintDC dc(m_hWnd);

		// Get client rect

		CRect rcClient; GetClientRect(rcClient);

		// Calc text rect

		CRect rcText(rcClient);
		dc.DrawText(btnText, btnText.GetLength(), rcText, DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_NOPREFIX | DT_CALCRECT);

		// Fit text to client rect

		if (rcText.Width() > rcClient.Width()) {

			// Calc pixels needed for one simbol

			int cxSymbol = rcText.Width() / text.GetLength();

			// Calc count of symbols that may be displayed

			int nSymbols = rcClient.Width() / cxSymbol;

			// Fit text

			btnText = btnText.Left(nSymbols); btnText += _T("...");
		}

		SetWindowText(btnText);
	}

	void SetUnderlinedFont(bool bUnderlined = true)
	{
		// Set font

		SetFont(bUnderlined ? m_fntUnderlined.m_hFont : m_fntNormal.m_hFont, TRUE);
	}

	bool ProcKeyDown(UINT vkey, UINT nRepeat, UINT uFlags)
	{
		// Handle keyboard events

		while (nRepeat--) {
			switch (vkey) {
				case VK_RETURN: ::PostMessage(GetParent(), WM_COMMAND, m_nCode, 0); break;
				case VK_UP:     ::PostMessage(GetParent(), WM_NEXTDLGCTL, TRUE, FALSE); break;
				case VK_DOWN:   ::PostMessage(GetParent(), WM_NEXTDLGCTL, FALSE, FALSE); break;
				default: return false;
			}
		}

		return true;
	}

	///////////////////////////////////////////////////////////////////////////
	// Message map

	BEGIN_MSG_MAP(CStaticBtn)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
		MESSAGE_HANDLER(WM_KILLFOCUS, OnKillFocus)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	END_MSG_MAP()

	///////////////////////////////////////////////////////////////////////////
	// Message handlers

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// Let default window procedure do its chores

		LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);

		// Initialize

		VERIFY(InitFonts());

		// Set font

		SetUnderlinedFont(false);

		return lRes;
	}

	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// Allow further processing

		bHandled = FALSE;

		return 0;
	}

	LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// Set font

		SetUnderlinedFont();

		// Allow further processing

		bHandled = FALSE;

		return 0;
	}

	LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// Set font

		SetUnderlinedFont(false);

		// Allow further processing

		bHandled = FALSE;

		return 0;
	}

	LRESULT OnGetDlgCode(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// Request all keyboard input

		return DLGC_WANTALLKEYS;
	}

	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// Process keydown events

		if (ProcKeyDown((UINT)wParam, LOWORD(lParam), HIWORD(lParam))) {
			// Processed
		} else
			bHandled = FALSE;

		return 0;
	}

};

///////////////////////////////////////////////////////////////////////////////
// CRecentDocumentList - MRU List Support

#ifndef _WTL_MRUEMPTY_TEXT
#define _WTL_MRUEMPTY_TEXT  _T("(empty)")
#endif

// forward declaration
inline bool AtlCompactPath(LPTSTR lpstrOut, LPCTSTR lpstrIn, int cchLen);

template <class T, int t_cchItemLen = MAX_PATH, int t_nFirstID = ID_FILE_MRU_FIRST, int t_nLastID = ID_FILE_MRU_LAST>
class CRecentDocumentListBase
{
public:
// Declarations
	struct _DocEntry
	{
		TCHAR szDocName[t_cchItemLen];
		bool operator ==(const _DocEntry& de) const
		{ return (lstrcmpi(szDocName, de.szDocName) == 0); }
	};

	enum
	{
		m_nMaxEntries_Min = 2,
		m_nMaxEntries_Max = t_nLastID - t_nFirstID + 1,
		m_cchMaxItemLen_Min = 6,
		m_cchMaxItemLen_Max = t_cchItemLen
	};

// Data members
	ATL::CSimpleArray<_DocEntry> m_arrDocs;
	int m_nMaxEntries;   // default is 4
	HMENU m_hMenu;

	TCHAR m_szNoEntries[t_cchItemLen];

	int m_cchMaxItemLen;

// Constructor
	CRecentDocumentListBase() : m_hMenu(NULL), m_nMaxEntries(4), m_cchMaxItemLen(-1)
	{
		// These ASSERTs verify values of the template arguments
		ATLASSERT(t_cchItemLen > m_cchMaxItemLen_Min);
		ATLASSERT(m_nMaxEntries_Max > m_nMaxEntries_Min);
	}

// Attributes
	HMENU GetMenuHandle() const
	{
		return m_hMenu;
	}

	void SetMenuHandle(HMENU hMenu)
	{
		ATLASSERT(hMenu == NULL || ::IsMenu(hMenu));
		m_hMenu = hMenu;
		if(m_hMenu == NULL || (::GetMenuString(m_hMenu, t_nFirstID, m_szNoEntries, t_cchItemLen, MF_BYCOMMAND) == 0))
			lstrcpy(m_szNoEntries, _WTL_MRUEMPTY_TEXT);
	}

	int GetMaxEntries() const
	{
		return m_nMaxEntries;
	}

	void SetMaxEntries(int nMaxEntries)
	{
		ATLASSERT(nMaxEntries >= m_nMaxEntries_Min && nMaxEntries <= m_nMaxEntries_Max);
		if(nMaxEntries < m_nMaxEntries_Min)
			nMaxEntries = m_nMaxEntries_Min;
		else if(nMaxEntries > m_nMaxEntries_Max)
			nMaxEntries = m_nMaxEntries_Max;
		m_nMaxEntries = nMaxEntries;
	}

	int GetMaxItemLength() const
	{
		return m_cchMaxItemLen;
	}

	void SetMaxItemLength(int cchMaxLen)
	{
		ATLASSERT((cchMaxLen >= m_cchMaxItemLen_Min && cchMaxLen <= m_cchMaxItemLen_Max) || cchMaxLen == -1);
		if(cchMaxLen != -1)
		{
			if(cchMaxLen < m_cchMaxItemLen_Min)
				cchMaxLen = m_cchMaxItemLen_Min;
			else if(cchMaxLen > m_cchMaxItemLen_Max)
				cchMaxLen = m_cchMaxItemLen_Max;
		}
		m_cchMaxItemLen = cchMaxLen;
		T* pT = static_cast<T*>(this);
		pT->UpdateMenu();
	}

// Operations
	BOOL AddToList(LPCTSTR lpstrDocName)
	{
		_DocEntry de;
		if(lstrcpy(de.szDocName, lpstrDocName) == NULL)
			return FALSE;

		for(int i = 0; i < m_arrDocs.GetSize(); i++)
		{
			if(lstrcmpi(m_arrDocs[i].szDocName, lpstrDocName) == 0)
			{
				m_arrDocs.RemoveAt(i);
				break;
			}
		}

		if(m_arrDocs.GetSize() == m_nMaxEntries)
			m_arrDocs.RemoveAt(0);

		BOOL bRet = m_arrDocs.Add(de);
		if(bRet)
		{
			T* pT = static_cast<T*>(this);
			bRet = pT->UpdateMenu();
		}
		return bRet;
	}

	BOOL GetFromList(int nItemID, LPTSTR lpstrDocName)
	{
		int nIndex = m_arrDocs.GetSize() - (nItemID - t_nFirstID) - 1;
		if(nIndex < 0 || nIndex >= m_arrDocs.GetSize())
			return FALSE;
		return (lstrcpy(lpstrDocName, m_arrDocs[nIndex].szDocName) != NULL);
	}

#if defined(_WTL_USE_CSTRING) || defined(__ATLSTR_H__)
	BOOL GetFromList(int nItemID, _CSTRING_NS::CString& strDocName)
	{
		int nIndex = m_arrDocs.GetSize() - (nItemID - t_nFirstID) - 1;
		if(nIndex < 0 || nIndex >= m_arrDocs.GetSize())
			return FALSE;
		strDocName = m_arrDocs[nIndex].szDocName;
		return TRUE;
	}
#endif //defined(_WTL_USE_CSTRING) || defined(__ATLSTR_H__)

	BOOL RemoveFromList(int nItemID)
	{
		int nIndex = m_arrDocs.GetSize() - (nItemID - t_nFirstID) - 1;
		BOOL bRet = m_arrDocs.RemoveAt(nIndex);
		if(bRet)
		{
			T* pT = static_cast<T*>(this);
			bRet = pT->UpdateMenu();
		}
		return bRet;
	}

	BOOL MoveToTop(int nItemID)
	{
		int nIndex = m_arrDocs.GetSize() - (nItemID - t_nFirstID) - 1;
		if(nIndex < 0 || nIndex >= m_arrDocs.GetSize())
			return FALSE;
		_DocEntry de;
		de = m_arrDocs[nIndex];
		m_arrDocs.RemoveAt(nIndex);
		BOOL bRet = m_arrDocs.Add(de);
		if(bRet)
		{
			T* pT = static_cast<T*>(this);
			bRet = pT->UpdateMenu();
		}
		return bRet;
	}

	BOOL ReadFromRegistry(LPCTSTR lpstrRegKey)
	{
		T* pT = static_cast<T*>(this);
		ATL::CRegKey rkParent;
		ATL::CRegKey rk;

		LONG lRet = rkParent.Open(HKEY_CURRENT_USER, lpstrRegKey);
		if(lRet != ERROR_SUCCESS)
			return FALSE;
		lRet = rk.Open(rkParent, pT->GetRegKeyName());
		if(lRet != ERROR_SUCCESS)
			return FALSE;

		DWORD dwRet = 0;
#if (_ATL_VER >= 0x0700)
		lRet = rk.QueryDWORDValue(_T("DocumentCount"), dwRet);
#else
		lRet = rk.QueryValue(dwRet, _T("DocumentCount"));
#endif
		if(lRet != ERROR_SUCCESS)
			return FALSE;
		SetMaxEntries(dwRet);

		m_arrDocs.RemoveAll();

		TCHAR szRetString[t_cchItemLen] = { 0 };
		_DocEntry de;

		for(int nItem = m_nMaxEntries; nItem > 0; nItem--)
		{
			const int cchBuff = 11;
			TCHAR szBuff[cchBuff] = { 0 };
			wsprintf(szBuff, _T("Document%i"), nItem);
#if (_ATL_VER >= 0x0700)
			ULONG ulCount = t_cchItemLen;
			lRet = rk.QueryStringValue(szBuff, szRetString, &ulCount);
#else
			DWORD dwCount = t_cchItemLen * sizeof(TCHAR);
			lRet = rk.QueryValue(szRetString, szBuff, &dwCount);
#endif
			if(lRet == ERROR_SUCCESS && (lstrcpy(de.szDocName, szRetString) != NULL))
				m_arrDocs.Add(de);
		}

		rk.Close();
		rkParent.Close();

		return pT->UpdateMenu();
	}

	BOOL WriteToRegistry(LPCTSTR lpstrRegKey)
	{
		T* pT = static_cast<T*>(this);
		pT;   // avoid level 4 warning
		ATL::CRegKey rkParent;
		ATL::CRegKey rk;

		LONG lRet = rkParent.Create(HKEY_CURRENT_USER, lpstrRegKey);
		if(lRet != ERROR_SUCCESS)
			return FALSE;
		lRet = rk.Create(rkParent, pT->GetRegKeyName());
		if(lRet != ERROR_SUCCESS)
			return FALSE;

#if (_ATL_VER >= 0x0700)
		lRet = rk.SetDWORDValue(_T("DocumentCount"), m_nMaxEntries);
#else
		lRet = rk.SetValue(m_nMaxEntries, _T("DocumentCount"));
#endif
		ATLASSERT(lRet == ERROR_SUCCESS);

		// set new values
		int nItem;
		for(nItem = m_arrDocs.GetSize(); nItem > 0; nItem--)
		{
			const int cchBuff = 11;
			TCHAR szBuff[cchBuff] = { 0 };
			wsprintf(szBuff, _T("Document%i"), nItem);
			TCHAR szDocName[t_cchItemLen] = { 0 };
			GetFromList(t_nFirstID + nItem - 1, szDocName);
#if (_ATL_VER >= 0x0700)
			lRet = rk.SetStringValue(szBuff, szDocName);
#else
			lRet = rk.SetValue(szDocName, szBuff);
#endif
			ATLASSERT(lRet == ERROR_SUCCESS);
		}

		// delete unused keys
		for(nItem = m_arrDocs.GetSize() + 1; nItem < m_nMaxEntries_Max; nItem++)
		{
			const int cchBuff = 11;
			TCHAR szBuff[cchBuff] = { 0 };
			wsprintf(szBuff, _T("Document%i"), nItem);
			rk.DeleteValue(szBuff);
		}

		rk.Close();
		rkParent.Close();

		return TRUE;
	}

// Implementation
	BOOL UpdateMenu()
	{
/*
		if(m_hMenu == NULL)
			return FALSE;
		ATLASSERT(::IsMenu(m_hMenu));

		int nItems = ::GetMenuItemCount(m_hMenu);
		int nInsertPoint;
		for(nInsertPoint = 0; nInsertPoint < nItems; nInsertPoint++)
		{
			CMenuItemInfo mi;
			mi.fMask = MIIM_ID;
			::GetMenuItemInfo(m_hMenu, nInsertPoint, TRUE, &mi);
			if (mi.wID == t_nFirstID)
				break;
		}
		ATLASSERT(nInsertPoint < nItems && "You need a menu item with an ID = t_nFirstID");

		int nItem;
		for(nItem = t_nFirstID; nItem < t_nFirstID + m_nMaxEntries; nItem++)
		{
			// keep the first one as an insertion point
			if (nItem != t_nFirstID)
				::DeleteMenu(m_hMenu, nItem, MF_BYCOMMAND);
		}

		TCHAR szItemText[t_cchItemLen + 6] = { 0 };   // add space for &, 2 digits, and a space
		int nSize = m_arrDocs.GetSize();
		nItem = 0;
		if(nSize > 0)
		{
			for(nItem = 0; nItem < nSize; nItem++)
			{
				if(m_cchMaxItemLen == -1)
				{
					wsprintf(szItemText, _T("&%i %s"), nItem + 1, m_arrDocs[nSize - 1 - nItem].szDocName);
				}
				else
				{
					TCHAR szBuff[t_cchItemLen] = { 0 };
					T* pT = static_cast<T*>(this);
					pT;   // avoid level 4 warning
					bool bRet = pT->CompactDocumentName(szBuff, m_arrDocs[nSize - 1 - nItem].szDocName, m_cchMaxItemLen);
					bRet;   // avoid level 4 warning
					ATLASSERT(bRet);
					wsprintf(szItemText, _T("&%i %s"), nItem + 1, szBuff);
				}
				::InsertMenu(m_hMenu, nInsertPoint + nItem, MF_BYPOSITION | MF_STRING, t_nFirstID + nItem, szItemText);
			}
		}
		else    // empty
		{
			::InsertMenu(m_hMenu, nInsertPoint, MF_BYPOSITION | MF_STRING, t_nFirstID, m_szNoEntries);
			::EnableMenuItem(m_hMenu, t_nFirstID, MF_GRAYED);
			nItem++;
		}
		::DeleteMenu(m_hMenu, nInsertPoint + nItem, MF_BYPOSITION);
*/
		return TRUE;
	}

// Overrideables
	// override to provide a different method of compacting document names
	static bool CompactDocumentName(LPTSTR lpstrOut, LPCTSTR lpstrIn, int cchLen)
	{
		return AtlCompactPath(lpstrOut, lpstrIn, cchLen);
	}

	static LPCTSTR GetRegKeyName()
	{
		return _T("Recent Document List");
	}
};

class CRecentDocumentList : public CRecentDocumentListBase<CRecentDocumentList>
{
public:
// nothing here
};

/////////////////////////////////////////////////////////////////////////////
// CSeparator - black horizontal line

typedef CWinTraits<
	WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
	0> CSeparatorTraits;

class CSeparator : public CWindowImpl<CSeparator, CWindow, CSeparatorTraits>
{
public:

	DECLARE_WND_CLASS(_T("ENSeparator"))

	///////////////////////////////////////////////////////////////////////////
	// Attributes

	///////////////////////////////////////////////////////////////////////////
	// Construction/destruction

	CSeparator()
	{
	}

	~CSeparator()
	{
	}

	///////////////////////////////////////////////////////////////////////////
	// Operations

	void DoPaint(CDCHandle dc)
	{
		// Get client rect

		CRect rc; GetClientRect(rc);

		// Set pen

		HPEN hOldPen = dc.SelectPen((HPEN) ::GetStockObject(BLACK_PEN));

		// Draw horizontal line

		CPoint pt[2] = { CPoint(rc.left, rc.top), CPoint(rc.right, rc.top) };
		dc.Polyline(pt, 2);

		// Restore context

		dc.SelectPen(hOldPen);
	}

	///////////////////////////////////////////////////////////////////////////
	// Message map

	BEGIN_MSG_MAP(CSeparator)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
	END_MSG_MAP()

	///////////////////////////////////////////////////////////////////////////
	// Message handlers

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// Let default window procedure do its chores

		LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);

		return lRes;
	}

	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
	//  TRACE(_T("CNoteBar::OnPaint: \n"));

		// Paint separator control

		CPaintDC dc(m_hWnd);
		DoPaint(dc.m_hDC);

		return 0;
	}

};

/////////////////////////////////////////////////////////////////////////////
// CDlgHeader - dialog header wih label and separator below

typedef CWinTraits<
	WS_CHILD | WS_VISIBLE,
	0> CDlgHeaderTraits;

class CDlgHeader : public CWindowImpl<CDlgHeader, CWindow, CDlgHeaderTraits>
{
public:

	DECLARE_WND_CLASS(_T("ENDialogHeader"))

	///////////////////////////////////////////////////////////////////////////
	// Attributes

	CString m_title;

	// GDI object

	CFont m_font;

	///////////////////////////////////////////////////////////////////////////
	// Construction/destruction

	CDlgHeader()
	{
	}

	~CDlgHeader()
	{
	}

	///////////////////////////////////////////////////////////////////////////
	// Operations

	void SetTitle(CString title)
	{
		m_title = title;
	}

	LPCTSTR GetTitle() const
	{
		return m_title;
	}

	// Initialization

	bool InitFont()
	{
		ASSERT(m_font == NULL);

		// Create new font

		CLogFont logFont;
		logFont.lfCharSet = DEFAULT_CHARSET;
		logFont.lfHeight = 10 * 10;
		logFont.lfWeight = FW_BOLD;
		lstrcpy(logFont.lfFaceName, _T("Tahoma"));
		m_font.CreatePointFontIndirect(&logFont);

		// Set new font

		SetFont(m_font);

		return true;
	}

	// Painting

	void DoPaint(CDCHandle dc)
	{
		// Get client rect

		CRect rcClient; GetClientRect(rcClient);

		// Drow header test

		COLORREF clr = dc.SetTextColor(RGB(0x0, 0x0, 0x93));

		CRect rcText = rcClient; rcText.left = 8; // Defolt left offset define by Microsoft UI Guidlines
		dc.DrawText(m_title, m_title.GetLength(), rcText, DT_LEFT | DT_NOCLIP | DT_NOPREFIX | DT_VCENTER);

		dc.SetTextColor(clr);

		// Drow bottom line

		HPEN hOldPen = dc.SelectPen((HPEN) ::GetStockObject(BLACK_PEN));

		CRect rcBtmLine = rcClient; rcBtmLine.bottom -= 1;
		CPoint pt[2] = { CPoint(rcBtmLine.left, rcBtmLine.bottom), CPoint(rcBtmLine.right, rcBtmLine.bottom) };
		dc.Polyline(pt, 2);

		dc.SelectPen(hOldPen);
	}

	///////////////////////////////////////////////////////////////////////////
	// Message map

	BEGIN_MSG_MAP(CDlgHeader)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
	END_MSG_MAP()

	///////////////////////////////////////////////////////////////////////////
	// Message handlers

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// Let default window procedure do its chores

		LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);

		// Init control

		VERIFY(InitFont());

		return 0;
	}

	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
	//  TRACE(_T("CNoteBar::OnPaint: \n"));

		// Paint separator control

		CPaintDC dc(m_hWnd); DoPaint(dc.m_hDC);

		return 0;
	}

};

}; // namespace WTL

#endif // __ATLMISCCE_H__
