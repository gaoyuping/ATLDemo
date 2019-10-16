//
// Windows Template Library Extensions
//
// This file is NOT part of Windows Template Library.
// The code and information is provided "as-is" without
// warranty of any kind, either expressed or implied.
//
// History: 02/Feb/2005 kvitekp - Initially created
//

#ifndef __ATLDIALOGIMPLEX_H__
#define __ATLDIALOGIMPLEX_H__

#pragma once

#ifndef __cplusplus
	#error ATL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLAPP_H__
	#error atldialogimplex.h requires atlapp.h to be included first
#endif

#ifndef __ATLWIN_H__
	#error atldialogimplex.h requires atlwin.h to be included first
#endif

#ifndef __ATLCTRLS_H__
	#error atldialogimplex.h requires atlctrls.h to be included first
#endif

///////////////////////////////////////////////////////////////////////////////
// Classes in this file
//
// CDialogImplEx<T>
//

namespace WTL
{

///////////////////////////////////////////////////////////////////////////////
// CDialogImplEx class: MI class for CDialogImpl derived classes
///////////////////////////////////////////////////////////////////////////////

template <class T>
class CDialogImplEx
{
public:

	///////////////////////////////////////////////////////////////////////////
	// Attributes

	///////////////////////////////////////////////////////////////////////////
	// Operations

	CString GetDlgItemText(UINT idCtrl)
	{
		T* pT = static_cast<T*>(this);

		CString str;

		CWindow wnd = pT->GetDlgItem(idCtrl);
		if (!wnd) return str;

		UINT cch = wnd.GetWindowTextLength();
		LPTSTR psz = str.GetBufferSetLength(cch);
		if (psz != NULL) {
			wnd.GetWindowText(psz, cch + 1);
			str.ReleaseBuffer();
		}
		
		return str;
	}

	bool GetDlgItemText(UINT idCtrl, CString& str)
	{
		T* pT = static_cast<T*>(this);

		CWindow wnd = pT->GetDlgItem(idCtrl);
		if (!wnd) return false;

		UINT cch = wnd.GetWindowTextLength();
		LPTSTR psz = str.GetBufferSetLength(cch);
		if (psz != NULL) {
			wnd.GetWindowText(psz, cch + 1);
			str.ReleaseBuffer();
		}
		
		return true;
	}

	UINT GetDlgItemTextLength(UINT idCtrl)
	{
		T* pT = static_cast<T*>(this);

		CWindow wnd = pT->GetDlgItem(idCtrl);
		if (!wnd) return false;

		return wnd.GetWindowTextLength();
	}

	bool GetDlgItemCheck(UINT idCtrl)
	{
		T* pT = static_cast<T*>(this);

		CButton bttn = pT->GetDlgItem(idCtrl);
		if (!bttn) return false;

		return bttn.GetCheck() ? true : false;
	}

	bool SetDlgItemCheck(UINT idCtrl, BOOL bCheck = true)
	{
		T* pT = static_cast<T*>(this);

		CButton bttn = pT->GetDlgItem(idCtrl);
		if (!bttn) return false;

		bttn.SetCheck(bCheck);

		return true;
	}

	bool IsDlgItemEnabled(UINT idCtrl)
	{
		T* pT = static_cast<T*>(this);

		CWindow wnd = pT->GetDlgItem(idCtrl);
		if (!wnd) return false;

		return wnd.IsWindowEnabled() ? true : false;
	}

	void EnableDlgItem(UINT idCtrl, bool bEnable = true)
	{
		T* pT = static_cast<T*>(this);

		CWindow wnd = pT->GetDlgItem(idCtrl);
		if (wnd) wnd.EnableWindow(bEnable);
	}

	bool SetDlgItemFocus(UINT idCtrl, bool bSetSel = true)
	{
		T* pT = static_cast<T*>(this);

		CWindow wnd = pT->GetDlgItem(idCtrl);
		if (!wnd) return false;

		wnd.SetFocus();
		if (bSetSel && (wnd.SendMessage(WM_GETDLGCODE) & DLGC_HASSETSEL)) {
			wnd.SendMessage(EM_SETSEL, 0, -1);
		}

		return true;
	}

	bool GetDlgItemRect(UINT idCtrl, CRect& rc)
	{
		T* pT = static_cast<T*>(this);

		CWindow wnd = pT->GetDlgItem(idCtrl);
		if (!wnd) return false;

		wnd.GetWindowRect(&rc);

		pT->ScreenToClient(&rc);

		return true;
	}

	bool SetDlgItemRect(UINT idCtrl, const CRect& rc, bool bRepaint = true)
	{
		T* pT = static_cast<T*>(this);

		CWindow wnd = pT->GetDlgItem(idCtrl);
		if (!wnd) return false;

		wnd.MoveWindow(rc, bRepaint);

		return true;
	}

	bool ResizeDlgItem(UINT idCtrl, int dx, int dy, bool bRepaint = true)
	{
		T* pT = static_cast<T*>(this);

		CWindow wnd = pT->GetDlgItem(idCtrl);
		if (!wnd) return false;

		CRect rc; wnd.GetWindowRect(&rc);
		::MapWindowPoints(HWND_DESKTOP, pT->m_hWnd, (POINT*)&rc, 2);

		rc.right+= dx; rc.bottom+= dy;

		wnd.MoveWindow(rc, bRepaint);

		return true;
	}

	bool OffsetDlgItem(UINT idCtrl, int dx, int dy, bool bRepaint = true)
	{
		T* pT = static_cast<T*>(this);

		CWindow wnd = pT->GetDlgItem(idCtrl);
		if (!wnd) return false;

		CRect rc; wnd.GetWindowRect(&rc);
		::MapWindowPoints(HWND_DESKTOP, pT->m_hWnd, (POINT*)&rc, 2);

		rc.OffsetRect(dx, dy);

		wnd.MoveWindow(rc, bRepaint);

		return true;
	}

	bool ShowDlgItem(UINT idCtrl, bool bShow = true)
	{
		T* pT = static_cast<T*>(this);

		CWindow wnd = pT->GetDlgItem(idCtrl);
		if (!wnd) return false;

		wnd.SetWindowPos(NULL, 0, 0, 0, 0, 
			SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOACTIVATE | (bShow ? SWP_SHOWWINDOW : SWP_HIDEWINDOW));

		return true;
	}

	bool KillDlgItem(UINT idCtrl)
	{
		T* pT = static_cast<T*>(this);

		CWindow wnd = pT->GetDlgItem(idCtrl);
		if (!wnd) return false;

		wnd.DestroyWindow();

		return true;
	}

	bool SetDlgItemFont(UINT idCtrl, HFONT hFont, bool bRedraw = true)
	{
		T* pT = static_cast<T*>(this);

		CWindow wnd = pT->GetDlgItem(idCtrl);
		if (!wnd) return false;

		wnd.SetFont(hFont, bRedraw);

		return true;
	}

	HFONT GetDlgItemFont(UINT idCtrl)
	{
		T* pT = static_cast<T*>(this);

		CWindow wnd = pT->GetDlgItem(idCtrl);
		if (!wnd) return false;

		return wnd.GetFont();
	}

	CString GetDlgTitle()
	{
		T* pT = static_cast<T*>(this);

		CString str;

		UINT cch = pT->GetWindowTextLength();
		LPTSTR psz = str.GetBufferSetLength(cch);
		if (psz != NULL) {
			pT->GetWindowText(psz, cch + 1);
			str.ReleaseBuffer();
		}
		
		return str;
	}

	void SetDlgTitle(LPCTSTR pszTitle)
	{
		T* pT = static_cast<T*>(this);

		pT->SetWindowText(pszTitle);
	}

	void SetDlgTitle(UINT idTitle)
	{
		T* pT = static_cast<T*>(this);

		CString title; title.LoadStringW(idTitle);

		pT->SetDlgTitle((LPCTSTR)title);
	}

	void SetDlgIcons(UINT nID)
	{
		T* pT = static_cast<T*>(this);

		pT->SetIcon(AtlLoadIconImage(nID, LR_DEFAULTCOLOR, 
			::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON)), TRUE);

		pT->SetIcon(AtlLoadIconImage(nID, LR_DEFAULTCOLOR, 
			::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CXSMICON)), FALSE);
	}

	HWND GetDefDlgButton()
	{
		T* pT = static_cast<T*>(this);

		for (HWND hwnd = pT->GetWindow(GW_CHILD); hwnd != NULL; hwnd = ::GetWindow(hwnd, GW_HWNDNEXT)) {
			if (::SendMessage(hwnd, WM_GETDLGCODE, 0, 0) & DLGC_DEFPUSHBUTTON)
				return hwnd;
		}

		return false;
	}

	void SetDefDlgButton(UINT idCtrl, bool bSetFocus = true)
	{
		T* pT = static_cast<T*>(this);

		HWND hwndDefBttn = idCtrl ? pT->GetDlgItem(idCtrl) : NULL;

		for (HWND hwnd = pT->GetWindow(GW_CHILD); hwnd != NULL; hwnd = ::GetWindow(hwnd, GW_HWNDNEXT)) {
			UINT dlgCode = ::SendMessage(hwnd, WM_GETDLGCODE, 0, 0);
			if ((dlgCode & DLGC_DEFPUSHBUTTON) && hwnd != hwndDefBttn) {
				::SendMessage(hwnd, BM_SETSTYLE, BS_PUSHBUTTON, TRUE);
			} else
			if ((dlgCode & DLGC_UNDEFPUSHBUTTON) && hwnd == hwndDefBttn) {
				::SendMessage(hwnd, BM_SETSTYLE, BS_DEFPUSHBUTTON, TRUE);
				if (bSetFocus) ::SetFocus(hwnd);
			}
		}
	}

};

}; // namespace WTL

#endif // __ATLDIALOGIMPLEX_H__
