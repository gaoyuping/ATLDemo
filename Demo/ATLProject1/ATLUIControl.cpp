#include "stdafx.h"
#include "ATLUIControl.h"
#include "LogInfo.h"
#include <atlwin.h>
#include <winuser.h>
#include <atlgdiraii.h>

#define DefaultBroder 1
ATLControl::ATLControl(ATLUISTYLE style, ICallback* icallback) :
    m_icallback(icallback),
    m_style(style),
    m_iWidth(-1),
    m_iHeight(-1),
    m_iMinWidth(-1),
    m_iMinHeight(-1),
    m_iMaxWidth(-1),
    m_iMaxHeight(-1),
    m_iFixWidth(-1),
    m_iFixHeight(-1),
    m_iBorderTop(DefaultBroder),
    m_iBorderRight(DefaultBroder),
    m_iBorderBotton(DefaultBroder),
    m_iBorderLeft(DefaultBroder),
    m_cText("ATLControl"),
    m_textColor(RGB(0,255,0)),
    m_borderColor(RGB(0,0, 255)),
    m_backgroundColor(RGB(255, 0, 255))
    , m_bNeedRePaint(true)
    , m_bcreate(false)
{
    InOutlog(__FUNCTION__);
    m_bshow = false;
}

ATLControl::~ATLControl()
{
    InOutlog(__FUNCTION__);
}

bool ATLControl::IsWantedMessage(UINT uMsg) {
    switch (uMsg) {
    case WM_SETFOCUS:
    case WM_KILLFOCUS:
    //case WM_SIZE:
    case WM_MOUSEWHEEL:
    case WM_LBUTTONUP:
    case WM_LBUTTONDOWN:
    //case WM_PAINT:
        return true;
    }

    return false;
}

ATLUISTYLE ATLControl::getStyle()
{
    return m_style;
}

void ATLControl::setHwnd(HWND hwnd) {
    m_hWnd = hwnd;
}

void ATLControl::setPos(CRect rect) {
    m_bNeedRePaint = true;
    if (m_bcreate) {
        MoveWindow(rect.left, rect.top, rect.Width(), rect.Height());
    }
    else {
        m_rect = rect;
    }
}

void ATLControl::setSize(int iw, int ih)
{
    m_iWidth = iw;
    m_iHeight = ih;
    m_bNeedRePaint = true;
}

void ATLControl::setMinSize(int iw, int ih) {
    m_iMinWidth = iw;
    m_iMinHeight = ih;
    m_bNeedRePaint = true;
}

void ATLControl::setMaxSize(int iw, int ih) {
    m_iMaxWidth = iw;
    m_iMaxHeight = ih;
    m_bNeedRePaint = true;
}

void ATLControl::setFixSize(int iw, int ih) {
    m_iFixWidth = iw;
    m_iFixHeight = ih;
    m_bNeedRePaint = true;
}

void ATLControl::setBorderSize(int itop, int iright, int ibotton, int ileft)
{
    m_iBorderTop = itop;
    m_iBorderRight = itop;
    m_iBorderBotton = itop;
    m_iBorderLeft = itop;
    m_bNeedRePaint = true;
}

void ATLControl::setTextColor(COLORREF bordercolor)
{
    m_textColor = bordercolor;
    m_bNeedRePaint = true;
}

void ATLControl::setBorderColor(COLORREF bordercolor)
{
    m_borderColor = bordercolor;
    m_bNeedRePaint = true;
}

void ATLControl::setBackgroundColor(COLORREF bordercolor) {
    m_backgroundColor = bordercolor;
    m_bNeedRePaint = true;
}

void ATLControl::seText(CString ctext)
{
    m_cText = ctext;
    m_bNeedRePaint = true;
}

void ATLControl::SetWidth(int iw) {
    m_iWidth = iw;
    if (m_bcreate) {
        GetClientRect(&m_rect);
    }
    m_rect.right = m_rect.left + m_iWidth;
    m_bNeedRePaint = true;
    //MoveWindow(m_rect, true);
}

void ATLControl::SetHeight(int ih) {
    m_iHeight = ih;
    if (m_bcreate) {
        GetClientRect(&m_rect);
    }
    m_rect.bottom = m_rect.top + m_iHeight;
    m_bNeedRePaint = true;
    //MoveWindow(m_rect, true);
}

int ATLControl::getWidth()
{
    return m_iWidth;
}
int ATLControl::getHeight()
{
    return m_iHeight;
}

LRESULT ATLControl::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    bHandled = false;
    m_bcreate = true;
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
    if (m_iHeight == -1 && m_iWidth == -1)
    {
        if (m_bcreate) {
            GetClientRect(&m_rect);
        }
        m_iWidth = m_rect.Width();
        m_iHeight = m_rect.Height();
        m_bNeedRePaint = true;
    }
    return 0;
}
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")
using namespace Gdiplus;

LRESULT ATLControl::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
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
//     CPaintDC pdc(m_hWnd);
//     CRect rc = pdc.m_ps.rcPaint;
// 
//     CMemoryDC mdc(pdc.m_hDC, rc);
//     DrawBackgroundColor(mdc);
//     DrawBorder(mdc);
//     DrawText(mdc);

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

void ATLControl::Draw(CMemoryDC &mdc) {
    DrawBackgroundColor(mdc);
    DrawBorder(mdc);
    DrawText(mdc);
}

void ATLControl::DrawBorder(CMemoryDC &dc)
{
    int irightHelf = 0;
    int ibottonHelf = 0;
    if (m_iBorderTop > 0)
    {
        CPen penLt;
        penLt.CreatePen(PS_SOLID, m_iBorderTop, m_borderColor);
        CSelectPen selectLtPen(dc, penLt);
        dc.MoveTo(m_rect.left, m_rect.top + m_iBorderTop / 2);
        dc.LineTo(m_rect.right, m_rect.top + m_iBorderTop / 2);
    }

    if (m_iBorderRight > 0)
    {
        if (m_iBorderRight % 2) {
            irightHelf = m_iBorderRight / 2 + 1;
        }
        else {
            irightHelf = m_iBorderRight / 2;
        }

        CPen penLt;
        penLt.CreatePen(PS_SOLID, m_iBorderRight, m_borderColor);
        CSelectPen selectLtPen(dc, penLt);
        int ii = m_rect.right;
        dc.MoveTo(m_rect.right - irightHelf, m_rect.top);
        dc.LineTo(m_rect.right - irightHelf, m_rect.bottom);
    }

    if (m_iBorderBotton > 0)
    {
        if (m_iBorderBotton % 2) {
            ibottonHelf = m_iBorderBotton / 2 + 1;
        }
        else {
            ibottonHelf = m_iBorderBotton / 2;
        }
        CPen penLt;
        penLt.CreatePen(PS_SOLID, m_iBorderBotton, m_borderColor);
        CSelectPen selectLtPen(dc, penLt);
        dc.MoveTo(m_rect.left, m_rect.bottom - ibottonHelf);
        dc.LineTo(m_rect.right, m_rect.bottom - ibottonHelf);
    }

    if (m_iBorderLeft > 0)
    {
        CPen penLt;
        penLt.CreatePen(PS_SOLID, m_iBorderLeft, m_borderColor);
        CSelectPen selectLtPen(dc, penLt);
        dc.MoveTo(m_rect.left + m_iBorderLeft / 2, m_rect.top);
        dc.LineTo(m_rect.left + m_iBorderLeft / 2, m_rect.bottom);
    }
}

void ATLControl::DrawBackgroundColor(CMemoryDC &dc)
{
    CBrush brush;
    brush.CreateSolidBrush(m_backgroundColor);
    CRect rect;
    rect.top = m_rect.top + m_iBorderTop ;
    rect.left = m_rect.left + m_iBorderLeft;
    rect.bottom = m_rect.bottom - m_iBorderBotton;
    rect.right = m_rect.right - m_iBorderRight;
    dc.FillRect(&rect, brush);
}

void ATLControl::DrawText(CMemoryDC &dc) {
    CRect rect;
    rect.top = m_rect.top + m_iBorderTop;
    rect.left = m_rect.left + m_iBorderLeft;
    rect.bottom = m_rect.bottom - m_iBorderBotton;
    rect.right = m_rect.right - m_iBorderRight;
    int iret;
    CSetBkMode setBkMode(dc, TRANSPARENT);
    CSetTextColor setTextColor(dc, m_textColor);
    iret = dc.DrawText(m_cText, -1, rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
}

