#include "stdafx.h"
#include "ATLMenu.h"
#include "LogInfo.h"
#include <atlwin.h>

int iScrollBarWidth = 20;
ATLMenu::ATLMenu(ICallback* icallback) :
    ATLControl(UIMenu, icallback)
    ,m_sBar(this)
{
    InOutlog(__FUNCTION__);
    m_cText = "ATLMenu";
    m_iBorderBotton = 1;
    m_iBorderLeft = 1;
    m_iBorderTop = 1;
    m_iBorderRight = 1;
    m_iSpace = 0;
    m_iBeginTop = 0;
    m_sizechange = false;
}

ATLMenu::ATLMenu(ATLUISTYLE style, ICallback* icallback) :
    ATLControl(style, icallback)
    ,m_sBar(this)
{
    InOutlog(__FUNCTION__);
    m_cText = "ATLMenu";
    m_iBorderBotton = 1;
    m_iBorderLeft = 1;
    m_iBorderTop = 1;
    m_iBorderRight = 1;
    m_iSpace = 0;
    m_iBeginTop = 0;
    m_sizechange = false;
}

ATLMenu::~ATLMenu()
{
    InOutlog(__FUNCTION__);
    std::vector<ATLControl*>::iterator iter;
    ATLControl*ptrctrl;
    while (m_listCtrl.size() > 0)
    {
        iter = m_listCtrl.begin();
        ptrctrl = *iter;
        m_listCtrl.erase(iter);
        ptrctrl->setHwnd(nullptr);
        delete ptrctrl;
    }
    m_sBar.setHwnd(nullptr);
}

bool ATLMenu::Relayout() {
    InOutlog(__FUNCTION__);
//     if (m_sizechange) {
//         if (!m_sizechangwait) {
//             m_sizechangwait = true;
//             ::PostMessage(m_hWnd, WM_RELAYOUT, 0, 0);
//         }
//         return false;
//     }
//     m_sizechange = true;
    int iw = 0;
    int ih = 0;
    CRect rect;
    for (std::vector<ATLControl*>::iterator iter = m_listCtrl.begin(); iter != m_listCtrl.end(); iter++) {
        if (ih > 0) {
            ih += m_iSpace;
        }
        //ATLTRACE(_T("%d  x=%d y=%d  out\n"), 0, 0, m_iBorderTop + ih - m_iBeginTop);
        rect.left = m_iBorderLeft;
        rect.top = m_iBorderTop + ih - m_iBeginTop;
        rect.bottom = rect.top + (*iter)->getHeight();
        rect.right = rect.left + (*iter)->getWidth();

        (*iter)->setPos(rect);
        iw = iw > (*iter)->getWidth() ? iw : (*iter)->getWidth();
        ih += (*iter)->getHeight();
    }
    ih = ih + m_iBorderTop + m_iBorderBotton;
    iw = iw + m_iBorderRight + m_iBorderLeft;
    if (iw == 0 || ih == 0) {
        GetClientRect(&m_rect);
        m_iHeight = m_rect.Height();
        m_iWidth = m_rect.Width();
    }
    else {
        if (iUnDefine != m_iMaxWidth && ih > m_iMaxWidth) {
            iw = m_iMaxWidth;
        }
        else if (iUnDefine != m_iMinWidth && ih < m_iMinWidth) {
            iw = m_iMinWidth;
        }
        if (iUnDefine != m_iMaxHeight && ih > m_iMaxHeight) {
            m_sBar.SetRange(0, ih - m_iMaxHeight);
            ih = m_iMaxHeight;
            RECT rect;
            rect.top = m_iBorderTop;
            rect.left = iw - m_iBorderLeft - iScrollBarWidth;
            rect.right = iw - m_iBorderRight;
            rect.bottom = ih - m_iBorderBotton;
            //m_sBar.Show(rect);
            m_sBar.setPos(rect);
            m_sBar.Show(rect);
            for (std::vector<ATLControl*>::iterator iter = m_listCtrl.begin(); iter != m_listCtrl.end(); iter++) {
                (*iter)->SetWidth(iw - iScrollBarWidth - 2);
            }
        }
        else if (iUnDefine != m_iMinHeight && ih < m_iMinHeight) {
            ih = m_iMinHeight;
            m_sBar.Hide();
        }
        else {
            m_sBar.Hide();
        }

        m_iHeight = ih;
        m_iWidth = iw;
        m_rect.right = m_rect.left + m_iWidth;
        m_rect.bottom = m_rect.top + m_iHeight;
    }
    m_bNeedRePaint = true;
    ResizeClient(m_iWidth, m_iHeight);
    Invalidate();
    UpdateWindow();
    m_sizechange = false;
    return true;
}

void ATLMenu::setSpace(int ispace)
{
    m_iSpace = ispace;
}

void ATLMenu::show(RECT point) {
    MoveWindow(point.left, point.top, point.right, point.bottom);
    ShowWindow(true);
    Relayout();
    ShowWindow(true);
    UpdateWindow();
}

LRESULT ATLMenu::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    bHandled = false;
    m_sBar.setHwnd(m_hWnd);
    DWORD wStyle = GetWndStyle(0);
    wStyle = wStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
    //::SetWindowLong(m_hWnd, GWL_STYLE, wStyle);
    return 0;
}


LRESULT ATLMenu::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    m_hWnd;
    InOutlog(__FUNCTION__);
    bHandled = false;
    return 0;
}


LRESULT ATLMenu::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    bHandled = false;

    return FALSE;
}


LRESULT ATLMenu::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    bHandled = false;

    return FALSE;
}

LRESULT ATLMenu::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    CPaintDC pdc(m_hWnd);

    if (m_bNeedRePaint)
    {
        m_bNeedRePaint = false;
        hDCMem = CreateCompatibleDC(pdc.m_hDC);
        hBmpMem = CreateCompatibleBitmap(pdc.m_hDC, pdc.m_ps.rcPaint.right, pdc.m_ps.rcPaint.bottom);
        hPreBmp = (HBITMAP)SelectObject(hDCMem, hBmpMem);
        CMemoryDC mdc(hDCMem, pdc.m_ps.rcPaint);  
        DrawBackgroundColor(mdc);
        DrawBorder(mdc);
        DrawText(mdc);
        if (m_sBar.IsWindow() && m_sBar.IsShow()) {
            m_sBar.Draw(mdc);
        }
        for (std::vector<ATLControl*>::iterator iter = m_listCtrl.begin(); iter != m_listCtrl.end(); iter++) {
            (*iter)->Draw(mdc);
        }
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

LRESULT ATLMenu::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    bHandled = false;

    return FALSE;
}

LRESULT ATLMenu::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    bHandled = false;
    if (m_icallback)
    {
        m_icallback->OnCtrlCallback(this);
    }
    return FALSE;
}

LRESULT ATLMenu::OnActivate(UINT msg, WPARAM wp, LPARAM lp, BOOL& bHandled)
{
    if (wp == WA_INACTIVE) {
        //PostMessage(WM_CLOSE, 0, 0);
    }
    bHandled = FALSE;
    return bHandled;
}

LRESULT ATLMenu::OnReLayout(UINT msg, WPARAM wp, LPARAM lp, BOOL& bHandled)     {
    InOutlog(__FUNCTION__);
    m_sizechangwait = !Relayout();
    return FALSE;
}

bool ATLMenu::addItem(ATLControl* ptrItem, int ipos)
{
    if ( -1 == ipos)
    {
        m_listCtrl.push_back(ptrItem);
    }
    else
    {
        if (m_listCtrl.size() < ipos)
        {
            m_listCtrl.push_back(ptrItem);
        }
        else
        {
            m_listCtrl.insert(m_listCtrl.begin()+ ipos, ptrItem);
        }
    }
    return true;
}

bool ATLMenu::removeItem(const ATLControl* ptrItem)
{
    //数据量不会太大 简单处理
    for (std::vector<ATLControl*>::iterator iter = m_listCtrl.begin(); iter != m_listCtrl.end(); iter++)
    {
        if (ptrItem == *iter)
        {
            m_listCtrl.erase(iter);
            delete ptrItem;
            return true;
        }
    }
    return false;
}

void ATLMenu::OnCtrlCallback(ATLControl* pCtrl)     {
    if (ATLScrollBar *ptr = dynamic_cast<ATLScrollBar*>(pCtrl)) {
        if (!m_sizechange) {
            m_iBeginTop = ptr->GetValue();
            Relayout();
        }
    }
}
