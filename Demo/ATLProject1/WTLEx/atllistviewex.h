//
// Windows Template Library Extensions
//
// This file is NOT part of Windows Template Library.
// The code and information is provided "as-is" without
// warranty of any kind, either expressed or implied.
//
// History: 18/Dec/2003 kvitekp - Initially created
//

#ifndef __ATLLISTVIEWEX_H__
#define __ATLLISTVIEWEX_H__

#pragma once

#ifndef __cplusplus
	#error ATL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLAPP_H__
	#error atllistviewex.h requires atlapp.h to be included first
#endif

#ifndef __ATLWIN_H__
	#error atllistviewex.h requires atlwin.h to be included first
#endif

#ifndef __ATLCTRLS_H__
	#error atllistviewex.h requires atlctrls.h to be included first
#endif

///////////////////////////////////////////////////////////////////////////////
// Classes in this file
//
// CListViewEx<T>
//

namespace WTL
{

///////////////////////////////////////////////////////////////////////////////
// CListViewEx class: MI class for CListViewCtrl derived classes
///////////////////////////////////////////////////////////////////////////////

template <class T>
class CListViewEx
{
public:
	
	///////////////////////////////////////////////////////////////////////////
	// Attributes

	///////////////////////////////////////////////////////////////////////////
	// Operations

	int GetColumnCount()
	{
		T* pT = static_cast<T*>(this);

		return pT->GetHeader().GetItemCount();
	}

	void FixLastColumnWidth()
	{
		T* pT = static_cast<T*>(this);

		int nLastCol = pT->GetColumnCount() - 1;
		pT->SetColumnWidth(nLastCol, LVSCW_AUTOSIZE_USEHEADER);
		pT->Scroll(CSize(0, 0));
	}
	
	int AddColumnSize(LPCTSTR pszText, int cx = LVSCW_AUTOSIZE_USEHEADER, 
			int nItem = -1, int nSubItem = -1,
			int nMask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM,
			int nFmt = LVCFMT_LEFT)
	{
		T* pT = static_cast<T*>(this);

		if (nItem == -1) nItem = pT->GetColumnCount();

		LV_COLUMN lvc;
		lvc.mask = nMask;
		lvc.fmt = nFmt;
		lvc.pszText = (LPTSTR)pszText;
		lvc.cx = cx;
		if (nMask & LVCF_SUBITEM) lvc.iSubItem = (nSubItem != -1) ? nSubItem : nItem;
		
		return pT->InsertColumn(nItem, &lvc);
	}

	BOOL SetColumnText(int nCol, LPCTSTR pszText)
	{
		T* pT = static_cast<T*>(this);

		LV_COLUMN lvc;
		lvc.mask = LVCF_FMT | LVCF_TEXT;
		lvc.fmt = LVCFMT_LEFT;
		lvc.pszText = (LPTSTR)pszText;
		
		return pT->SetColumn(nCol, &lvc);
	}

	bool SelItem(int nIndex)
	{
		T* pT = static_cast<T*>(this);

		UINT mask = LVIS_SELECTED | LVIS_FOCUSED;
		if (!pT->SetItemState(nIndex, mask, mask))
			return false;
		
		pT->EnsureVisible(nIndex, FALSE);

		return true;
	}
	
	bool ShowPopupMenu(const CPoint& ptMenu)
	{
		ATLTRACE(_T("CListViewEx::ShowPopupMenu: ptMenu=(%d,%d)\n"), ptMenu.x, ptMenu.y);
		
		// Override this method to show popup menu

		return false;
	}

	///////////////////////////////////////////////////////////////////////////
	// Message map
	
	BEGIN_MSG_MAP(CListViewEx<T>)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_NOTIFY, OnNotify)
		MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenu)
	END_MSG_MAP()

	///////////////////////////////////////////////////////////////////////////
	// Message handlers
	
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		T* pT = static_cast<T*>(this);

		pT->FixLastColumnWidth();
		
		bHandled = FALSE;
		
		return 0;
	}
	
	LRESULT OnNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		T* pT = static_cast<T*>(this);

		if (((LPNMHDR)lParam)->hwndFrom == pT->GetHeader().m_hWnd) {

			switch (((LPNMHDR)lParam)->code) {

				// Resize last column to fill the header
			
				case HDN_ITEMCHANGED: 
					int nItem = ((LPNMHEADER)lParam)->iItem;
					int nLastItem = pT->GetColumnCount() - 1;
					if (nItem != nLastItem) pT->FixLastColumnWidth();
					break;
			}
		}

		bHandled = FALSE;
		
		return 0;
	}

	LRESULT OnContextMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		T* pT = static_cast<T*>(this);

		// List control sends WM_CONTEXTMENU on button down so filter it out

		if (::GetKeyState(VK_RBUTTON) < 0) return 0;

		// Check if from us and leave otherwise

		if ((HWND)wParam != pT->m_hWnd) return 0;

		// Position the popup menu if necessary

		CPoint ptMenu(lParam);
		
		if (lParam == -1) {

			// Check if there is a selected item and position popup menu next to it,
			// otherwise position popup menu at the top left of the list view window

			CRect rc;
			int nSel = pT->GetNextItem(-1, LVNI_SELECTED);
			if (nSel != -1) {
				pT->GetItemRect(nSel, &rc, LVIR_LABEL);
				ptMenu.SetPoint(rc.left, rc.bottom);
			} else {
				pT->GetClientRect(&rc);
				ptMenu.SetPoint(rc.left, rc.top);
			}

			// Map menu position to screen coordiantes
			
			pT->ClientToScreen(&ptMenu);
		}

		// Call base class method to show popup menu

		pT->ShowPopupMenu(ptMenu);
		
		return 0;
	}

};

}; // namespace WTL

#endif // __ATLLISTVIEWEX_H__
