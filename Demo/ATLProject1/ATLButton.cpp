#include "stdafx.h"
#include "ATLButton.h"
#include "LogInfo.h"

#include <winuser.h>
#define RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))

static CPoint _mousepoint;

ATLButton::ATLButton(ICallback* icallback):
    ATLLabel(UIButton, icallback)
{
    InOutlog(__FUNCTION__);
    m_bMouseTracking = false;
    m_bMouseWithin = false;
    m_cText = "ATLButton";
}

ATLButton::ATLButton(ATLUISTYLE style, ICallback* icallback) :
    ATLLabel(style, icallback)
{
    InOutlog(__FUNCTION__);
    m_bMouseTracking = false;
    m_bMouseWithin = false;
    m_cText = "ATLButton";
}


ATLButton::~ATLButton()
{
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
    m_bcreate = true;
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

LRESULT ATLButton::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    //InOutlog(__FUNCTION__);
//     CPaintDC pdc(m_hWnd);
//     CRect rc = pdc.m_ps.rcPaint;
// 
//     CMemoryDC mdc(pdc.m_hDC, rc);
//     DrawBackgroundColor(mdc);
//     DrawBorder(mdc);
//     DrawText(mdc);
// 
//     bHandled = TRUE;
//     return bHandled;
    CPaintDC pdc(m_hWnd);

    if (m_bNeedRePaint) {
        m_bNeedRePaint = false;
        hDCMem = CreateCompatibleDC(pdc.m_hDC);
        hBmpMem = CreateCompatibleBitmap(pdc.m_hDC, pdc.m_ps.rcPaint.right, pdc.m_ps.rcPaint.bottom);
        hPreBmp = (HBITMAP)SelectObject(hDCMem, hBmpMem);
        CMemoryDC mdc(hDCMem, pdc.m_ps.rcPaint);
        DrawBackgroundColor(mdc);
        DrawBorder(mdc);
        DrawText(mdc);
    }
    BitBlt(pdc.m_hDC, 0, 0, pdc.m_ps.rcPaint.right, pdc.m_ps.rcPaint.bottom, hDCMem, 0, 0, SRCCOPY);
    bHandled = TRUE;
    return bHandled;
}

LRESULT ATLButton::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    if (m_icallback) {
        m_icallback->OnCtrlCallback(this);
    }
    bHandled = false;
    return 0;
}

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
        //do nothing
    }
    if (!m_bMouseTracking)
    {
        m_bMouseTracking = true;
    }

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

