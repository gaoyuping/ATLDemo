#include "stdafx.h"
#include "ATLButton.h"
#include "LogInfo.h"

ATLButton::ATLButton(ICallback* icallback):
    ATLLabel(UIButton, icallback)
{
    InOutlog(__FUNCTION__);
    m_bMouseTracking = false;
    m_bMouseWithin = false;
}

ATLButton::ATLButton(ATLUISTYLE style, ICallback* icallback) :
    ATLLabel(style, icallback)
{
    InOutlog(__FUNCTION__);
    m_bMouseTracking = false;
    m_bMouseWithin = false;
}


ATLButton::~ATLButton()
{
}

bool ATLButton::IsWantedMessage(UINT uMsg)
{
    switch (uMsg) {
    case WM_SETFOCUS:
    case WM_KILLFOCUS:
//     case WM_LBUTTONDOWN:
//     case WM_LBUTTONDBLCLK:
//     case WM_LBUTTONUP:
//     case WM_RBUTTONDOWN:
//     case WM_RBUTTONDBLCLK:
//     case WM_RBUTTONUP:
//     case WM_MOUSEMOVE:
//     case WM_MOUSELEAVE:
//     case WM_CAPTURECHANGED:
//     case WM_KEYDOWN:
//     case WM_CONTEXTMENU:
//     case WM_COMMAND:
//     case WM_TIMER:
//     case WM_CREATE:
//     case WM_DESTROY:
//     case WM_CHAR:
        return true;
    }

    return false;
}

void ATLButton::StartTrack()
{
    InOutlog(__FUNCTION__);
    TRACKMOUSEEVENT time = { 0 };
    time.cbSize = sizeof(time);
    time.hwndTrack = m_hWnd;
    time.dwFlags = TME_HOVER | TME_LEAVE;
    time.dwHoverTime = HOVER_DEFAULT;
    _TrackMouseEvent(&time);
}

LRESULT ATLButton::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    bHandled = false;
    return 0;
}

LRESULT ATLButton::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    bHandled = false;

    return 0;
}

LRESULT ATLButton::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    bHandled = false;

    return 0;
}

LRESULT ATLButton::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    bHandled = false;
    GetClientRect(&m_rect);
    return 0;
}
#include <winuser.h>
#define RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
LRESULT ATLButton::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    CPaintDC dc(m_hWnd);
    POINT beginpoint;
    beginpoint.x = 0;
    beginpoint.y = 0;
    dc.MoveTo(beginpoint);
    dc.LineTo(m_rect.Width() - 1, 0);
    dc.LineTo(m_rect.Width() - 1, m_rect.Height() - 1);
    dc.LineTo(0, m_rect.Height() - 1);
    dc.LineTo(beginpoint);
    CRect rect = m_rect;
    rect.MoveToX(1);
    rect.MoveToY(1);
    rect.right = m_rect.right - 2;
    rect.bottom = m_rect.bottom - 2;
    int iret;
    iret= dc.DrawText(_T("BTN"), -1, rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

    bHandled = TRUE;
    return 0;
}

LRESULT ATLButton::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    bHandled = false;
    return 0;
}
static CPoint _mousepoint;
LRESULT ATLButton::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    if (_mousepoint.x == GET_X_LPARAM(lParam) && _mousepoint .y == GET_Y_LPARAM(lParam))
    {
        return TRUE;
    }
    //InOutlog(__FUNCTION__);
    _mousepoint.x = GET_X_LPARAM(lParam);
    _mousepoint.y = GET_Y_LPARAM(lParam);
    if (!m_bMouseWithin)
    {
        //OnMouseEnter(uMsg, wParam, lParam, bHandled);
    }
    if (!m_bMouseTracking)
    {
        //StartTrack();
        m_bMouseTracking = true;
    }

    //ATLTRACE(_T("  mouse point %d,%d\n"), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
    bHandled = TRUE;
    return 0;
}


LRESULT ATLButton::OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    m_bMouseTracking = false;
    bHandled = false;
    return 0;
}

LRESULT ATLButton::OnMouseEnter(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    m_bMouseWithin = true;
    bHandled = false;
    return 0;
}

LRESULT ATLButton::OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    bHandled = false;
    m_bMouseTracking = false;
    m_bMouseWithin = false;
    _mousepoint.x = -1;
    _mousepoint.y = -1;
    return 0;
}

LRESULT ATLButton::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    bHandled = false;
    return 0;
}

LRESULT ATLButton::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    bHandled = false;
    return 0;
}

