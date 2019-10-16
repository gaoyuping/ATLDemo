//
// Windows Template Library Extension for
// EverNote Ink Control control
//
// Written by Pete Kvitek <pete@kvitek.com>
//
// This file is NOT part of Windows Template Library.
// The code and information is provided "as-is" without
// warranty of any kind, either expressed or implied.
//

#ifndef __ATLENINKCTRL_H__
#define __ATLENINKCTRL_H__

#pragma once

#ifndef __cplusplus
	#error ATL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLAPP_H__
	#error atleninkctrl.h requires atlapp.h to be included first
#endif

#ifndef __ATLWIN_H__
	#error atleninkctrl.h requires atlwin.h to be included first
#endif

#include <ENInkCtrl.h>

/////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CENInkCtrlBaseT<TBase>
// CENInkCtrlT<TBase>
// CENFatInkCtrlT<TBase>
//

namespace WTL
{

/////////////////////////////////////////////////////////////////////////////
// CENInkCtrlBase - client side base class
// for ENInkCtrl and ENFatInkCtrl controls

template <class TBase>
class CENInkCtrlBaseT : public TBase
{
public:

	// Constructors

	CENInkCtrlBaseT(HWND hWnd = NULL) : TBase(hWnd) { }

	CENInkCtrlBaseT< TBase >& operator=(HWND hWnd)
	{
		m_hWnd = hWnd;
		return *this;
	}

	// Operations

	UINT SetPenType(UINT penType)
	{
		return (UINT)SendMessage(ENIM_SETPENTYPE, (WPARAM)penType);
	}

	UINT GetPenType()
	{
		return (UINT)SendMessage(ENIM_GETPENTYPE);
	}

	UINT SetPenWidth(UINT penWidth)
	{
		return (UINT)SendMessage(ENIM_SETPENWIDTH, (WPARAM)penWidth);
	}

	UINT GetPenWidth()
	{
		return (UINT)SendMessage(ENIM_GETPENWIDTH);
	}

	COLORREF SetPenColor(COLORREF cr)
	{
		return (UINT)SendMessage(ENIM_SETPENCOLOR, (WPARAM)cr);
	}

	COLORREF GetPenColor()
	{
		return (COLORREF)SendMessage(ENIM_GETPENCOLOR);
	}

	UINT SetRecoMode(UINT recoMode)
	{
		return (UINT)SendMessage(ENIM_SETRECOMODE, (WPARAM)recoMode);
	}

	UINT GetRecoMode()
	{
		return (UINT)SendMessage(ENIM_GETRECOMODE);
	}

	BOOL SetPaperInfo(ENINKPAPERINFO* penpi)
	{
		ATLASSERT(penpi != NULL);
        penpi->cbSize = sizeof(ENINKPAPERINFO);
		return (BOOL)SendMessage(ENIM_SETPAPERINFO, 0, (LPARAM)penpi);
	}

	BOOL GetPaperInfo(ENINKPAPERINFO* penpi)
	{
		ATLASSERT(penpi != NULL);
        penpi->cbSize = sizeof(ENINKPAPERINFO);
		return (BOOL)SendMessage(ENIM_GETPAPERINFO, 0, (LPARAM)penpi);
	}

	BOOL SetModify(BOOL bModified = TRUE)
	{
		return (BOOL)SendMessage(ENIM_SETMODIFY, (WPARAM)bModified);
	}

	BOOL GetModify()
	{
		return (BOOL)SendMessage(ENIM_GETMODIFY);
	}

	BOOL SetReadOnly(BOOL bReadOnly = TRUE)
	{
		return (BOOL)SendMessage(ENIM_SETREADONLY, (WPARAM)bReadOnly);
	}

	BOOL GetReadOnly()
	{
		return (BOOL)SendMessage(ENIM_GETREADONLY);
	}

    BOOL SetZoom(UINT nZoomPercent)
    {
        return (BOOL)SendMessage(ENIM_SETZOOM, nZoomPercent);
    }

    UINT GetZoom()
    {
        return (UINT)SendMessage(ENIM_GETZOOM);
    }

	BOOL RequestResize()
	{
		return (BOOL)SendMessage(ENIM_REQUESTRESIZE);
	}

	LONG StreamIn(UINT uFormat, ENINKSTREAM& stream)
	{
		return (LONG)SendMessage(ENIM_STREAMIN, uFormat, (LPARAM)&stream);
	}

	LONG StreamOut(UINT uFormat, ENINKSTREAM& stream)
	{
		return (LONG)SendMessage(ENIM_STREAMOUT, uFormat, (LPARAM)&stream);
	}

	BOOL ClearContent()
	{
		return (BOOL)SendMessage(ENIM_CLEARCONTENT);
	}

	BOOL IsContentEmpty()
	{
		return (BOOL)SendMessage(ENIM_ISCONTENTEMPTY);
	}

	BOOL ResetControl()
	{
		return (BOOL)SendMessage(ENIM_RESETCONTROL);
	}

	UINT GetPrintHeight(ENINKPRINTMEASURE* penpm)
	{
		return (UINT)SendMessage(ENIM_GETPRINTHEIGHT, 0, (LPARAM)penpm);
	}

	UINT PrintPage(ENINKPRINTSETTINGS* penps)
	{
		return (UINT)SendMessage(ENIM_PRINTPAGE, 0, (LPARAM)penps);
	}

    BOOL SetRecognizer(const ENINKRECOSETTINGS* preco)
    {
		ATLASSERT(preco != NULL);
        return (BOOL)SendMessage(ENIM_SETRECOGNIZER, 0, (LPARAM)preco);
    }

    BOOL GetRecognizer(ENINKRECOSETTINGS* preco)
    {
		ATLASSERT(preco != NULL);
        preco->cbSize = sizeof(ENINKRECOSETTINGS);
        return (BOOL)SendMessage(ENIM_GETRECOGNIZER, 0, (LPARAM)preco);
    }

	BOOL Recognize(UINT recoMode)
	{
		return (BOOL)SendMessage(ENIM_RECOGNIZE, (WPARAM)recoMode);
	}

	BOOL HighlightWords(bool bUpdate, LPCTSTR pszWords)
	{
		return (BOOL)SendMessage(ENIM_HIGHLIGHTWORDS,
                                 (WPARAM)bUpdate,
                                 (LPARAM)pszWords);
	}

	BOOL HighlightWords(LPCTSTR pszWords, bool bUpdate = true)
	{
		return (BOOL)SendMessage(ENIM_HIGHLIGHTWORDS,
                                 (WPARAM)bUpdate,
                                 (LPARAM)pszWords);
	}

	BOOL RemoveHighlighting(bool bUpdate = true)
	{
		return HighlightWords(NULL, bUpdate);
	}

    BOOL CanUndo()
    {
        return (BOOL)SendMessage(EM_CANUNDO);
    }

    BOOL CanRedo()
    {
        return (BOOL)SendMessage(EM_CANREDO);
    }

    void Undo()
	{
		SendMessage(EM_UNDO);
	}

	void Redo()
	{
		SendMessage(EM_REDO);
	}

	void Clear()
	{
		SendMessage(WM_CLEAR);
	}

	void Copy()
	{
		SendMessage(WM_COPY);
	}

	void Cut()
	{
		SendMessage(WM_CUT);
	}

	void Paste()
	{
		SendMessage(WM_PASTE);
	}

	void SelectAll()
	{
		SendMessage(ENIM_SELECTALL);
	}

	BOOL HasSelection()
	{
		return (BOOL)SendMessage(ENIM_HASSELECTION);
	}

};

/////////////////////////////////////////////////////////////////////////////
// CENInkCtrl - client side for a ENInkCtrl control

template <class TBase>
class CENInkCtrlT : public CENInkCtrlBaseT<TBase>
{
public:

	// Constructors

	CENInkCtrlT(HWND hWnd = NULL) : CENInkCtrlBaseT<TBase>(hWnd) { }

	CENInkCtrlT< TBase >& operator=(HWND hWnd)
	{
		m_hWnd = hWnd;
		return *this;
	}

	HWND Create(HWND hWndParent, _U_RECT rect = NULL, LPCTSTR szWindowName = NULL,
			DWORD dwStyle = 0, DWORD dwExStyle = 0,
			_U_MENUorID MenuOrID = 0U, LPVOID lpCreateParam = NULL)
	{
        m_hWnd = ::CreateWindowEx(dwExStyle,
                                  GetWndClassName(),
                                  szWindowName,
			                      dwStyle,
                                  rect.m_lpRect->left,
                                  rect.m_lpRect->top,
                                  rect.m_lpRect->right - rect.m_lpRect->left,
			                      rect.m_lpRect->bottom - rect.m_lpRect->top,
                                  hWndParent,
                                  MenuOrID.m_hMenu,
			                      NULL,
                                  lpCreateParam);

		return m_hWnd;
	}
	
	// Attributes

	static LPCTSTR GetWndClassName()
	{
		return ::ENInkCtrlClassName();
	}

	// Methods

	LRESULT CALLBACK ENInkCtrlWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return ::ENInkCtrlWindowProc(m_hWnd, uMsg, wParam, lParam);
	}

};

typedef CENInkCtrlT<CWindow> CENInkCtrl;

/////////////////////////////////////////////////////////////////////////////
// CENFatInkCtrl - client side for a ENFatInkCtrl control

template <class TBase>
class CENFatInkCtrlT : public CENInkCtrlBaseT<TBase>
{
public:

	// Constructors

	CENFatInkCtrlT(HWND hWnd = NULL) : CENInkCtrlBaseT<TBase>(hWnd) { }

	CENFatInkCtrlT< TBase >& operator=(HWND hWnd)
	{
		m_hWnd = hWnd;
		return *this;
	}

	HWND Create(HWND hWndParent, _U_RECT rect = NULL, LPCTSTR szWindowName = NULL,
			DWORD dwStyle = 0, DWORD dwExStyle = 0,
			_U_MENUorID MenuOrID = 0U, LPVOID lpCreateParam = NULL)
	{
        m_hWnd = ::CreateWindowEx(dwExStyle,
                                  GetWndClassName(),
                                  szWindowName,
			                      dwStyle,
                                  rect.m_lpRect->left,
                                  rect.m_lpRect->top,
                                  rect.m_lpRect->right - rect.m_lpRect->left,
			                      rect.m_lpRect->bottom - rect.m_lpRect->top,
                                  hWndParent,
                                  MenuOrID.m_hMenu,
			                      NULL,
                                  lpCreateParam);

		return m_hWnd;
	}
	
	// Attributes

	static LPCTSTR GetWndClassName()
	{
		return ::ENFatInkCtrlClassName();
	}

	BOOL ShowToolbar(BOOL bShow = TRUE)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, ENIM_SHOWTOOLBAR, (WPARAM)bShow, 0L);
	}

	// Methods

	LRESULT CALLBACK ENFatInkCtrlWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return ::ENFatInkCtrlWindowProc(m_hWnd, uMsg, wParam, lParam);
	}

};

typedef CENFatInkCtrlT<CWindow> CENFatInkCtrl;

}; //namespace WTL

#endif // __ATLENINKCTRL_H__
