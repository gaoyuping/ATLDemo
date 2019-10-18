#include "stdafx.h"
#include "ATLLabel.h"
#include "LogInfo.h"
#include <atlwin.h>
#include <atlgdiraii.h>
#include <Render.h>
extern HINSTANCE g_hInstance;
ATLLabel::ATLLabel(ICallback* icallback) :
    ATLControl(UILabel, icallback),
    m_image(nullptr),
    m_cTextSize(0,0)
{
    InOutlog(__FUNCTION__);
    m_cText = "ATLLabel";
}

ATLLabel::ATLLabel(ATLUISTYLE style, ICallback* icallback):
    ATLControl(style, icallback),
    m_image(nullptr),
    m_cTextSize(0, 0)
{
    InOutlog(__FUNCTION__);
    m_cText = "ATLLabel";
}


ATLLabel::~ATLLabel()
{
    InOutlog(__FUNCTION__);
    m_cText = "ATLLabel";
}

bool ATLLabel::LoadImage(int Resourceid, int index, int imageCount) {
    ASSERT(index < imageCount);

    FreeImage(); 
    ASSERT(!m_image);

    m_image = new ATL::CImage;
    m_image->LoadFromResource(g_hInstance, Resourceid);

    return true;
}

void ATLLabel::FreeImage() {
    if (m_image) {
        delete m_image;
        m_image = NULL;
    }
}

LRESULT ATLLabel::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    bHandled = false;
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

    return FALSE;
}


LRESULT ATLLabel::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    bHandled = false;
    GetClientRect(&m_rect);
    return 0;
}

LRESULT ATLLabel::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    CPaintDC dc(m_hWnd);

    DrawBackgroundColor(dc);
    DrawBorder(dc);
    if (m_image) 
        DrawPic(dc);
    DrawText(dc);
    bHandled = True;
    return bHandled;
}

LRESULT ATLLabel::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    bHandled = false;

    return FALSE;
}

LRESULT ATLLabel::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    bHandled = false;

    return FALSE;
}

void ATLLabel::DrawText(CPaintDC &dc)
{
    int iimageWidth = 0;

    if (m_image) {
        iimageWidth = m_image->GetWidth();
    }
    else {
        iimageWidth = 0;
    }

    CRect rect;
    rect.top = m_rect.top + m_iBorderTop;
    rect.left = m_rect.left + m_iBorderLeft + iimageWidth;
    rect.bottom = m_rect.bottom - m_iBorderBotton;
    rect.right = m_rect.right - m_iBorderRight;
    int iret;
    CSetBkMode setBkMode(dc, TRANSPARENT);
    CSetTextColor setTextColor(dc, m_textColor);
    CSize size;
    dc.GetTextExtent(m_cText, m_cText.GetLength(), &size);
    iret = dc.DrawText(m_cText, -1, rect, DT_SINGLELINE | DT_VCENTER | DT_LEFT);
    m_cTextSize = size;
}

void ATLLabel::DrawPic(CPaintDC &dc)
{
    if (m_image)
    {
        CRect rect;
        rect.top = m_rect.top + m_iBorderTop;
        rect.left = m_rect.left + m_iBorderLeft;
        rect.bottom = m_rect.top + m_image->GetWidth() + 1;
        rect.right = m_rect.left + m_image->GetWidth() + 1;
        if (rect.bottom >= m_rect.Height() - m_iBorderBotton)
        {
            rect.bottom = m_rect.Height() - m_iBorderBotton - 1;
        }
        if (rect.Width() > m_rect.Width() - m_iBorderRight) {
            rect.right = m_rect.Width() - m_iBorderRight - 1;
        }
        CRender::Image(dc.m_hDC, rect.top, rect.left, rect.right, rect.bottom, *m_image, 0, 0, m_image->GetWidth(), m_image->GetHeight());
    }
}