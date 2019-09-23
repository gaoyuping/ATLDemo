// ATLProject1.cpp: WinMain 的实现


#include "stdafx.h"
#include "resource.h"
#include "ATLProject1_i.h"
#include "ATLMain.h"
#include <time.h>

extern "C" int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/,
                                LPTSTR /*lpCmdLine*/, int nShowCmd)
{
    MSG msg;
    bool nCmdShow = true;
    //hInst = hInstance; // 将实例句柄存储在全局变量中

    // TODO: 初始化窗口
    // 加载菜单资源
    //HMENU hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDC_MYATLWINDOWTEST));

    // 创建窗口
    ATLMain wnd;
    wnd.Create(NULL, ATL::CWindow::rcDefault, _T("My Window"), WS_OVERLAPPEDWINDOW, WS_EX_CLIENTEDGE, nullptr);

    // 显示并更新窗口
    wnd.ShowWindow(nCmdShow);
    wnd.UpdateWindow();

    // 主消息循环:
    int iret;
    while (iret = GetMessage(&msg, NULL, 0, 0)) // 消息循环 - 等待消息
    {
        if (iret < 1)
        {
            ATLTRACE(_T("iret=%d\n"), iret);
        }
        TranslateMessage(&msg); // 消息循环 - 翻译键盘消息
        DispatchMessage(&msg); // 消息循环 - 分发消息
    }

    return (int)msg.wParam;
}

