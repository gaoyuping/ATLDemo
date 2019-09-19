#include "stdafx.h"
#include "ATLLabel.h"
#include "LogInfo.h"
#include <atlwin.h>

ATLLabel::ATLLabel(ICallback* icallback)
    :m_icallback(icallback)
{
    InOutlog(__FUNCTION__);
}


ATLLabel::~ATLLabel()
{
    InOutlog(__FUNCTION__);
}

LRESULT ATLLabel::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    bHandled = false;
    RECT rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = 100;
    rect.bottom = 50;
    //ATL::CWindowImpl<ATLLabel>::Create(m_hWnd, rect, _T("ATLLabel"));
    return 0;
}


LRESULT ATLLabel::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    bHandled = false;

    return 0;
}


LRESULT ATLLabel::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    bHandled = false;

    return 0;
}


LRESULT ATLLabel::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    bHandled = false;
    GetClientRect(&m_rect);
    return 0;
}
#include <winuser.h>
#define RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
LRESULT ATLLabel::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    CPaintDC dc(m_hWnd);
    POINT beginpoint;
    beginpoint.x = 0;
    beginpoint.y = 0;
    dc.MoveTo(beginpoint);
    dc.LineTo(m_rect.Width()-1, 0);
    dc.LineTo(m_rect.Width()-1, m_rect.Height()-1);
    dc.LineTo(0, m_rect.Height() - 1);
    dc.LineTo(beginpoint);
    CRect rect = m_rect;
    rect.MoveToX(1);
    rect.MoveToY(1);
    rect.right = m_rect.right - 2;
    rect.bottom = m_rect.bottom - 2;
    int iret;
    iret = dc.DrawText(_T("ATLLabel"), -1, rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
    
    bHandled = TRUE;
    return 0;
}

