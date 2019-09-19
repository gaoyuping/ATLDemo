// ATLProject1.cpp: WinMain 的实现


#include "stdafx.h"
#include "resource.h"
#include "ATLProject1_i.h"
#include "ATLMain.h"
#include <time.h>


class CATLProject1Module : public ATL::CAtlExeModuleT< CATLProject1Module >
{
public :
	DECLARE_LIBID(LIBID_ATLProject1Lib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_ATLPROJECT1, "{4f1ff4c3-d526-43e3-a82a-5846c4b9cf6e}")
};

CATLProject1Module _AtlModule;


void pass1()
{
    int  xxxx = 0;
    int * iptr = &xxxx;
    *iptr = 4;
    *(iptr + 80) = 5;
}

extern "C" int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/,
								LPTSTR /*lpCmdLine*/, int nShowCmd)
{
//     time_t now = time(0);
//     time_t nownoeday = 24 * 60 * 60 ;
//     // 把 now 转换为字符串形式
//     char* dt = ctime(&now);
// 
//     // 把 now 转换为 tm 结构
//     tm *gmtm = gmtime(&now);
// 
//     now = now - gmtm->tm_hour * 60 * 60  - gmtm->tm_min * 60  - gmtm->tm_sec ;
//     now += nownoeday;
//     gmtm = gmtime(&now);
// 
//     int x = 123;
//     pass1();
//     x = 4;
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

