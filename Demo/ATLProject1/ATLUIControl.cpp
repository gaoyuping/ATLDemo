#include "stdafx.h"
#include "ATLUIControl.h"
#include "LogInfo.h"
#include <atlwin.h>
#include <winuser.h>

ATLControl::ATLControl(ATLUISTYLE style,ICallback* icallback) :
    m_icallback(icallback),
    m_style(style),
    m_iwidth(-1),
    m_iheight(-1)
{
    InOutlog(__FUNCTION__);
}

ATLControl::~ATLControl()
{
    InOutlog(__FUNCTION__);
}

ATLUISTYLE ATLControl::getStyle()
{
    return m_style;
}

void ATLControl::setSize(int iw, int ih)
{
    m_iwidth = iw;
    m_iheight = 50;
}

int ATLControl::getWidth()
{
    return m_iwidth;
}
int ATLControl::getHeight()
{
    return m_iheight;
}

LRESULT ATLControl::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    bHandled = false;
    return 0;
}

LRESULT ATLControl::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    bHandled = false;
    return 0;
}

LRESULT ATLControl::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    bHandled = false;

    return FALSE;
}

LRESULT ATLControl::OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    bHandled = false;
    PostMessage(WM_SIZE);
    return FALSE;
}

LRESULT ATLControl::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    bHandled = false;
    if (m_iheight == -1 && m_iwidth == -1)
    {
        GetClientRect(&m_rect);
        m_iwidth = m_rect.Width();
        m_iheight = m_rect.Height();
    }
    return 0;
}

LRESULT ATLControl::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
    iret = dc.DrawText(_T("ATLControl"), -1, rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
    
    bHandled = TRUE;
    return 0;
}

LRESULT ATLControl::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    bHandled = false;

    return FALSE;
}

LRESULT ATLControl::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    bHandled = false;

    return FALSE;
}

LRESULT ATLControl::OnDeleteCtrl(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    bHandled = false;

    return FALSE;
}
