#include "stdafx.h"
#include "ATLMenu.h"
#include "LogInfo.h"
#include <atlwin.h>

ATLMenu::ATLMenu(ICallback* icallback) :
    ATLControl(UIMenu, icallback)
{
    InOutlog(__FUNCTION__);
}

ATLMenu::ATLMenu(ATLUISTYLE style, ICallback* icallback) :
    ATLControl(style, icallback)
{
    InOutlog(__FUNCTION__);
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
        ptrctrl->DestroyWindow();
        delete ptrctrl;
    }
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
        (*iter)->MoveWindow(0, iw, (*iter)->getWidth(), (*iter)->getHeight());
        iw = iw > (*iter)->getWidth()? iw : (*iter)->getWidth();
        ih += (*iter)->getHeight();
    }
//     if (iw == 0 || ih == 0)
//     {
//         GetClientRect(&m_rect);
//         m_iheight = m_rect.Height();
//         m_iwidth = m_rect.Width();
//     }
//     else
//     {
//         if (m_iheight == ih && m_iwidth == iw)
//         {
//             return 0;
//         }
//         m_iheight = ih;
//         m_iwidth = iw;
//     }
    //ResizeClient(m_iwidth, m_iheight);
    return 0;
}

LRESULT ATLMenu::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    CPaintDC dc(m_hWnd);
    POINT beginpoint;
    beginpoint.x = 0;
    beginpoint.y = 0;

    CRect rect = m_rect;
    rect.MoveToX(1);
    rect.MoveToY(1);
    rect.right = m_rect.right - 2;
    rect.bottom = m_rect.bottom - 2;
    int iret;
    iret = dc.DrawText(_T("ATLMenu"), -1, rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

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
