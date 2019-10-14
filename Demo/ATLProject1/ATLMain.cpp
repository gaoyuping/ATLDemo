
#include "stdafx.h"
#include "ATLMain.h"
#include <atltypes.h>
#include <WinUser.h>
#include "LogInfo.h"
#include "ATLMenu.h"
extern HMODULE g_hDll;
ATLMain::ATLMain()
    :m_label(this)
{

}
ATLMain::~ATLMain()
{

}
LRESULT ATLMain::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    ATLTRACE(_T("ATLMain::OnCreate: wParam=%d size=(%d x %d)\n"), wParam, LOWORD(lParam), HIWORD(lParam));
    //m_label.Create();
    RECT rect;
    GetClientRect(&rect);
    rect.left = rect.right / 8;
    rect.top = rect.bottom / 8;
    rect.right = rect.right / 4;
    rect.bottom = rect.bottom / 4;

    HWND hwnd = nullptr;
    hwnd = m_label.Create(m_hWnd, rect, _T("ATLLabel"), WS_CHILD | WS_VISIBLE);

    rect.left += 100;
    rect.top += 100;
    rect.right += 100;
    rect.bottom +=  100;
    hwnd = m_btn.Create(m_hWnd, rect, _T("ATLButton"), WS_CHILD | WS_VISIBLE);

    bHandled = false;
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
    int ierroecode;
    typedef bool (*FUNC)(HWND);
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

    bHandled = TRUE;
    return 0;
}

LRESULT ATLMain::OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    ATLMenu *ptrmenu = new ATLMenu(this);
    CPoint point;
    GetCursorPos(&point);
    RECT rect;
    rect.top = point.y;
    rect.left = point.x;
    rect.bottom = point.y + 50;
    rect.right = point.x + 100;
    ptrmenu->Create(m_hWnd, rect, _T("ATLMenu"), WS_POPUP);
    {
        ATLLabel *ptrctrl = new ATLLabel(this);
        ptrctrl->setSize(100, 50);
        ptrmenu->addItem(ptrctrl);
    }
    ATLTRACE(_T(" %s in\n"), _T("hhhhhhhhhhhhhhhhhhhhhhhhh"));
    {
        ATLButton *ptrctrl = new ATLButton(this);
        ptrctrl->setSize(100, 50);
        ptrmenu->addItem(ptrctrl);
    }
    ATLTRACE(_T(" %s in\n"), _T("ssssssssss"));
    ptrmenu->ShowWindow(true);
    ptrmenu->UpdateWindow();
    //
    bHandled = TRUE;
    return 0;
}
LRESULT ATLMain::OnDeleteCtrl(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    bHandled = false;
    if (WM_deleteCtrl == uMsg)
    {
        //delete (ATLControl*)wParam;
    }
    return FALSE;
}

LRESULT ATLMain::OnTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
    InOutlog(__FUNCTION__);
    bHandled = false;

    return FALSE;
}

LRESULT ATLMain::OnCmdNewNotebook(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)     {
    InOutlog(__FUNCTION__);
    bHandled = false;

    return FALSE;
}
void ATLMain::OnCtrlCallback(ATLControl* pCtrl)
{
    InOutlog(__FUNCTION__);
    if (UIMenu == pCtrl->getStyle())
    {
        pCtrl->DestroyWindow();
        ::PostMessage(m_hWnd, WM_deleteCtrl,(WPARAM)pCtrl,0 );
    }
}