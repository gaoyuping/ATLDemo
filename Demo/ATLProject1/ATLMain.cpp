
#include "stdafx.h"
#include "ATLMain.h"
#include <atltypes.h>
#include <WinUser.h>
#include "LogInfo.h"
#include "ATLMenu.h"
#include "Resource.h"

extern HMODULE g_hDll;
extern HINSTANCE g_hInstance;

ATLMain::ATLMain()
    :m_label(this), m_style(menu_ATL)
{

}

ATLMain::~ATLMain()
{
    for (; m_btnlist.size() > 0;)
    {
        std::vector<ATLButton*>::iterator iter = m_btnlist.begin();
        if (iter!= m_btnlist.end()) {
            ATLButton* ptr = *iter;
            m_btnlist.erase(iter);
            delete ptr;
        }
        
    }
}

LRESULT ATLMain::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    ATLTRACE(_T("ATLMain::OnCreate: wParam=%d size=(%d x %d)\n"), wParam, LOWORD(lParam), HIWORD(lParam));
    //m_label.Create();
    RECT rect;
    GetClientRect(&rect);
    rect.left = 100;
    rect.top = 50;
    rect.right = 200;
    rect.bottom = 100;

    HWND hwnd = nullptr;
    m_label.setBorderSize(1, 1, 1, 1);
    m_label.seText(_T("ATLLabel"));
    m_label.LoadImageW(IDB_BITMAP1);
    m_label.Create(m_hWnd, rect);
    m_label.setPos(rect);
    rect.top += 60;
    rect.bottom += 60;
    m_btn.Create(m_hWnd);
    m_btn.setPos(rect);
    m_btn.setBorderSize(2, 2, 2, 2);
    for (int i = 3; i < 11; i++)
    {
        ATLButton* ptr = new ATLButton(this);
        m_btnlist.push_back(ptr);
        rect.top += 60;
        rect.bottom += 60;
        ptr->Create(m_hWnd);
        ptr->setPos(rect);
        ptr->seText(_T("ATLButton"));
        ptr->setBorderSize(i, i, i, i);
    }

    bHandled = false;

    long lStyle;

    lStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
    //lStyle &= WS_DISABLED;
    lStyle |= WS_CAPTION;
    //lStyle |= WS_OVERLAPPEDWINDOW; 
    ::SetWindowLong(m_hWnd, GWL_STYLE, lStyle);
    m_hMenu = ::LoadMenu(g_hInstance,MAKEINTRESOURCE(IDR_MAINFRAME));
    ::SetMenu(m_hWnd, m_hMenu);
    m_hmenuHandle = m_hMenu;
    return 0;
}

LRESULT ATLMain::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    ATLTRACE(_T("ATLMain::OnDestroy: wParam=%d size=(%d x %d)\n"), wParam, LOWORD(lParam), HIWORD(lParam));


    bHandled = false;
    ::PostMessage(nullptr, WM_QUIT, 0, 0);
    return 0;
}

LRESULT ATLMain::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    ATLTRACE(_T("ATLMain::OnClose: wParam=%d size=(%d x %d)\n"), wParam, LOWORD(lParam), HIWORD(lParam));

    bHandled = false;
    return 0;
}

LRESULT ATLMain::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    ATLTRACE(_T("ATLMain::OnSize: wParam=%d size=(%d x %d)\n"), wParam, LOWORD(lParam), HIWORD(lParam));

    bHandled = false;
    return 0;
}

LRESULT ATLMain::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    bHandled = FALSE;

    return 0;
}
#include <time.h>
//#define Initdata
//#define sendmsg
#define showDialog
LRESULT ATLMain::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    if (menu_QT == (m_style & menu_QT))
    {
        int ierroecode;
        typedef bool(*FUNC)(HWND);
        TCHAR chCurDir[MAX_PATH] = { 0 };
        GetCurrentDirectory(MAX_PATH, chCurDir);

        //SetCurrentDirectory(_T("D:/code/Test/dll/"));
        g_hDll = ::LoadLibrary(L"qtdialog.dll");
        //HMODULE hDll = ::LoadLibraryEx(L"D:/code/Test/dll/qtdialog.dll", NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
        ierroecode = GetLastError();
        //SetCurrentDirectory(chCurDir);
        if (g_hDll != NULL) {
#ifdef showDialog
            ATLTRACE(_T(" %s in\n"), _T("showDialog"));
            FUNC displayQML = (FUNC)GetProcAddress(g_hDll, "showDialog");

            if (displayQML != NULL) {
                displayQML(m_hWnd);
                ATLTRACE(_T("  showDialog call end\n"));
            }
            else {
                ::MessageBox(NULL, L"can't found showDialog function", L"tips", MB_OK);
            }
            ATLTRACE(_T(" %s out\n"), _T("showDialog"));
#endif

#ifdef Initdata
            ATLTRACE(_T(" %s in\n"), _T("Initdata"));
            FUNC displayQML_InitData = (FUNC)GetProcAddress(g_hDll, "InitData");

            if (displayQML_InitData != NULL) {
                displayQML_InitData(m_hWnd);
                ATLTRACE(_T("  InitData call end\n"));
            }
            else {
                ::MessageBox(NULL, L"can't found InitData function", L"tips", MB_OK);
        }
            ATLTRACE(_T(" %s out\n"), _T("Initdata"));
#endif

#ifdef sendmsg
            ATLTRACE(_T(" %s in\n"), _T("sendmsg"));
            typedef bool(*FUNC_sendmsg)();
            FUNC_sendmsg displayQML_sendmsg = (FUNC_sendmsg)GetProcAddress(g_hDll, "sendmsg");

            if (displayQML_sendmsg != NULL) {
                displayQML_sendmsg();
                ATLTRACE(_T("  InitData call end\n"));
            }
            else {
                ::MessageBox(NULL, L"can't found sendmsg function", L"tips", MB_OK);
    }
            ATLTRACE(_T(" %s out\n"), _T("sendmsg"));
#endif

}
        else {
            ::MessageBox(NULL, L"can't found dll.", L"tips", MB_OK);
        }

    }
    else if(menu_ATL == (m_style & menu_ATL))
    {
        //ATL

    }

    bHandled = TRUE;
    return 0;
}

LRESULT ATLMain::OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    if (menu_QT == (m_style & menu_QT))
    {
    }
    else if (menu_ATL == (m_style & menu_ATL))
    {
        //ATL
        SetFocus();
        if (m_ptr.get()) {
            if (m_ptr->m_hWnd) {
                m_ptr->DestroyWindow();
            }
        }
        m_ptr.reset(new ATLMenu(this));
        CPoint point;
        GetCursorPos(&point);
        //ScreenToClient(&point);
        RECT rect;
        rect.top = point.y;
        rect.left = point.x;
        rect.bottom = point.y + 50;
        rect.right = point.x + 100;
        m_ptr->setMaxSize(100, 300);
        m_ptr->Create(m_hWnd, rect, _T("ATLMenu"), WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
        {
            ATLLabel *ptrctrl = new ATLLabel(this);
            ptrctrl->setHwnd(m_ptr->m_hWnd);
            //ptrctrl->Create(m_ptr->m_hWnd);
            ptrctrl->setSize(100, 50);
            m_ptr->addItem(ptrctrl);
        }
        ATLTRACE(_T(" %s in\n"), _T("hhhhhhhhhhhhhhhhhhhhhhhhh"));
        {
            for (int i =0; i < 10; i++)
            {
                ATLButton *ptrctrl = new ATLButton(this);
                ptrctrl->setHwnd(m_ptr->m_hWnd);
                //ptrctrl->Create(m_ptr->m_hWnd);
                ptrctrl->setSize(100, 50);
                ptrctrl->LoadImageW(IDB_BITMAP1);
                m_ptr->addItem(ptrctrl);
            }
        }
        ATLTRACE(_T(" %s in\n"), _T("ssssssssss"));
        m_ptr->show(rect);
    }

    //
    bHandled = TRUE;
    return 0;
}
LRESULT ATLMain::OnDeleteCtrl(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    bHandled = false;
    switch (uMsg) {
    case WM_deleteCtrl:
        delete (ATLControl*)wParam;
        break;
    default:
        break;
    }
    return FALSE;
}

LRESULT ATLMain::OnTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
    InOutlog(__FUNCTION__);
    bHandled = false;

    return FALSE;
}

LRESULT ATLMain::OnCmdNewNotebook(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    bHandled = false;

    return FALSE;
}

LRESULT ATLMain::OnInitMenuPopup(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InitTopMenuPopup((HMENU)wParam, LOWORD(lParam));
    return true;
}

LRESULT ATLMain::OnMenuClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    switch (wID) {
    case ID_STYLE_QT:
        m_style = menu_QT;
        break;
    case ID_STYLE_ATL:
        m_style = menu_ATL;
        break;
    default:
        break;
    }

    return true;
}

void ATLMain::OnCtrlCallback(ATLControl* pCtrl)
{
    InOutlog(__FUNCTION__);
    switch (pCtrl->getStyle()) {
    case UIMenu:
        //pCtrl->DestroyWindow();
        //::PostMessage(m_hWnd, WM_deleteCtrl, (WPARAM)pCtrl, 0);
        break;
    case UIButton:
        break;
    default:
        break;
    }
}

void ATLMain::InitTopMenuPopup(CMenuHandle popup, UINT index)
{
    m_hmenuHandle.CheckMenuItem(ID_STYLE_QT, MF_BYCOMMAND | (menu_QT == (m_style & menu_QT) ? MF_CHECKED : MF_UNCHECKED));
    m_hmenuHandle.CheckMenuItem(ID_STYLE_ATL, MF_BYCOMMAND | (menu_ATL == (m_style & menu_ATL) ? MF_CHECKED : MF_UNCHECKED));
}