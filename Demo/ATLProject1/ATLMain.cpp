
#include "stdafx.h"
#include "ATLMain.h"
#include <atltypes.h>
#include <WinUser.h>
#include "LogInfo.h"



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
    HWND hwnd = m_label.Create(m_hWnd, rect, _T("ATLLabel"), WS_CHILD | WS_VISIBLE);
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

LRESULT ATLMain::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	InOutlog(__FUNCTION__);
	int ierroecode;
	typedef int(*FUNC)(HWND);

	HMODULE hDll = ::LoadLibrary(L"D:/code/Test/dll/qtdialog.dll");
	ierroecode = GetLastError();
	if (hDll != NULL) {
		FUNC displayQML = (FUNC)GetProcAddress(hDll, "ShowDialog");

		if (displayQML != NULL) {
			displayQML(m_hWnd);
		}
		else {
			::MessageBox(NULL, L"can't found function", L"tips",MB_OK);
		}
		FreeLibrary(hDll);
	}
	else {
		::MessageBox(NULL, L"can't found dll.", L"tips", MB_OK);
	}

	bHandled = TRUE;


          
	return 0;
}


void ATLMain::OnCtrlCallback(ATLLabel* pCtrl)
{

}