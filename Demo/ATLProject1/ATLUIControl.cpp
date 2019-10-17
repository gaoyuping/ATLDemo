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
    m_iwidth(-1),
    m_iheight(-1),
    m_iBorderTop(DefaultBroder),
    m_iBorderRight(DefaultBroder),
    m_iBorderBotton(DefaultBroder),
    m_iBorderLeft(DefaultBroder),
    m_cText("ATLControl"),
    m_textColor(RGB(0,255,0)),
    m_borderColor(RGB(0,0, 0)),
    m_backgroundColor(RGB(255, 0, 255))
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

void ATLControl::setBorderSize(int itop, int iright, int ibotton, int ileft)
{
    m_iBorderTop = itop;
    m_iBorderRight = itop;
    m_iBorderBotton = itop;
    m_iBorderLeft = itop;
}

void ATLControl::setTextColor(COLORREF bordercolor)
{
    m_textColor = bordercolor;
}

void ATLControl::setBorderColor(COLORREF bordercolor)
{
    m_borderColor = bordercolor;
}

void ATLControl::setBackgroundColor(COLORREF bordercolor) {
    m_backgroundColor = bordercolor;
}

void ATLControl::seText(ATL::CString ctext)
{
    m_cText = ctext;
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

    DrawBackgroundColor(dc);
    DrawBorder(dc);
    DrawText(dc);

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

void ATLControl::DrawBorder(CPaintDC &dc)
{
    int irightHelf = 0;
    int ibottonHelf = 0;
    if (m_iBorderTop > 0)
    {
        CPen penLt;
        penLt.CreatePen(PS_SOLID, m_iBorderTop, m_borderColor);
        CSelectPen selectLtPen(dc, penLt);
        dc.MoveTo(0 , m_iBorderTop / 2);
        dc.LineTo(m_rect.Width(), m_iBorderTop / 2);
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
        dc.MoveTo(m_rect.Width() - irightHelf, 0);
        dc.LineTo(m_rect.Width() - irightHelf, m_rect.Height());
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
        dc.MoveTo(0, m_rect.Height() - ibottonHelf);
        dc.LineTo(m_rect.Width(), m_rect.Height() - ibottonHelf);
    }

    if (m_iBorderLeft > 0)
    {
        CPen penLt;
        penLt.CreatePen(PS_SOLID, m_iBorderLeft, m_borderColor);
        CSelectPen selectLtPen(dc, penLt);
        dc.MoveTo(m_iBorderLeft / 2, 0);
        dc.LineTo(m_iBorderLeft / 2, m_rect.Height());
    }
}

void ATLControl::DrawBackgroundColor(CPaintDC &dc)
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

void ATLControl::DrawText(CPaintDC &dc) {
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

