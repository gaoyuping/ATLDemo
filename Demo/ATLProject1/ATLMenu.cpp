#include "stdafx.h"
#include "ATLMenu.h"
#include "LogInfo.h"
#include <atlwin.h>

ATLMenu::ATLMenu(ICallback* icallback) :
    ATLControl(UIMenu, icallback)
{
    InOutlog(__FUNCTION__);
    m_cText = "ATLMenu";
    m_iBorderBotton = 1;
    m_iBorderLeft = 1;
    m_iBorderTop = 1;
    m_iBorderRight = 1;
    m_iSpace = 0;
}

ATLMenu::ATLMenu(ATLUISTYLE style, ICallback* icallback) :
    ATLControl(style, icallback)
{
    InOutlog(__FUNCTION__);
    m_cText = "ATLMenu";
    m_iBorderBotton = 1;
    m_iBorderLeft = 1;
    m_iBorderTop = 1;
    m_iBorderRight = 1;
    m_iSpace = 0;
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

LRESULT ATLMenu::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    bHandled = false;
    DWORD wStyle = GetWndStyle(0);
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
    int iw = 0;
    int ih = 0;
    for (std::vector<ATLControl*>::iterator iter = m_listCtrl.begin(); iter != m_listCtrl.end(); iter++)
    {
        if (ih > 0)
        {
            ih += m_iSpace;
        }
        ATLTRACE(_T("%d  x=%d y=%d  out\n"), 0,0, ih);
        (*iter)->MoveWindow(m_iBorderLeft, m_iBorderTop +  ih, (*iter)->getWidth(), (*iter)->getHeight());
        iw = iw > (*iter)->getWidth()? iw : (*iter)->getWidth();
        ih += (*iter)->getHeight();
    }
    ih = ih + m_iBorderTop + m_iBorderBotton;
    iw = iw + m_iBorderRight + m_iBorderLeft;
    if (iw == 0 || ih == 0)
    {
        GetClientRect(&m_rect);
        m_iheight = m_rect.Height();
        m_iwidth = m_rect.Width();
    }
    else
    {
        if (m_iheight == ih && m_iwidth == iw)
        {
            ::PostMessage(m_hWnd, WM_PAINT, 0, 0);
            return 0;
        }
        m_iheight = ih;
        m_iwidth = iw;
        m_rect.right = m_rect.left + m_iwidth;
        m_rect.bottom = m_rect.top + m_iheight;
    }
    ResizeClient(m_iwidth, m_iheight);
 
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
        PostMessage(WM_CLOSE, 0, 0);
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
