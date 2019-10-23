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
        delete ptrctrl;
    }
}

void ATLMenu::setSpace(int ispace)
{
    m_iSpace = ispace;
}

void ATLMenu::show(RECT point) {
    MoveWindow(point.left, point.top, point.right, point.bottom);
    ShowWindow(true);
    UpdateWindow();
}

LRESULT ATLMenu::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    bHandled = false;
    m_sBar.Create(m_hWnd);
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
    m_sizechange = true;
    int iw = 0;
    int ih = 0;
    for (std::vector<ATLControl*>::iterator iter = m_listCtrl.begin(); iter != m_listCtrl.end(); iter++)
    {
        if (ih > 0)
        {
            ih += m_iSpace;
        }
        ATLTRACE(_T("%d  x=%d y=%d  out\n"), 0,0, m_iBorderTop + ih - m_iBeginTop);
        (*iter)->MoveWindow(m_iBorderLeft, m_iBorderTop +  ih - m_iBeginTop, (*iter)->getWidth(), (*iter)->getHeight());
        iw = iw > (*iter)->getWidth()? iw : (*iter)->getWidth();
        ih += (*iter)->getHeight();
    }
    ih = ih + m_iBorderTop + m_iBorderBotton;
    iw = iw + m_iBorderRight + m_iBorderLeft;
    if (iw == 0 || ih == 0)
    {
        GetClientRect(&m_rect);
        m_iHeight = m_rect.Height();
        m_iWidth = m_rect.Width();
    }
    else
    {
        if (iUnDefine != m_iMaxHeight && ih > m_iMaxHeight) {
            m_sBar.SetRange(0, ih - m_iMaxHeight);
            ih = m_iMaxHeight;
            RECT rect;
            rect.top = m_iBorderTop;
            rect.left = m_iWidth - m_iBorderLeft - iScrollBarWidth;
            rect.right = m_iWidth - m_iBorderRight;
            rect.bottom = m_iHeight - m_iBorderBotton;
            m_sBar.Show(rect);
            for (std::vector<ATLControl*>::iterator iter = m_listCtrl.begin(); iter != m_listCtrl.end(); iter++) {
                (*iter)->SetWidth(m_iWidth - iScrollBarWidth - 2);
                (*iter)->Invalidate();
            }
        }
        else if (iUnDefine != m_iMinHeight && ih < m_iMinHeight) {
            ih = m_iMinHeight;
            m_sBar.Hide();
        }
        else {
            m_sBar.Hide();
        }

        if (iUnDefine != m_iMaxWidth && ih > m_iMaxWidth) {
            iw = m_iMaxWidth;
        }
        else if (iUnDefine != m_iMinWidth && ih < m_iMinWidth) {
            iw = m_iMinWidth;
        }

        if (m_iHeight == ih && m_iWidth == iw)
        {
            Invalidate();
            m_sizechange = false;
            return 0;
        }
        m_iHeight = ih;
        m_iWidth = iw;
        m_rect.right = m_rect.left + m_iWidth;
        m_rect.bottom = m_rect.top + m_iHeight;
    }
    ResizeClient(m_iWidth, m_iHeight);
 
    return 0;
}

LRESULT ATLMenu::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);

    return __super::OnPaint(uMsg, wParam, lParam, bHandled);;
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

            ::PostMessage(m_hWnd, WM_SIZE, 0, 0);
        }
    }
}
