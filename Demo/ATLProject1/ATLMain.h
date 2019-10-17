#pragma once
#include "ATLLabel.h"
#include "ATLButton.h"
#include <atlwin.h>
#include <WinUser.h>
#include <atlframe.h>
#include <vector>


#define WM_test WM_USER + 1000
enum menuStyle{
    menu_None = 0,
    menu_QT = 1,
    menu_ATL = 2,

};
class ATLMain : public ATL::CWindowImpl<ATLMain>
    ,public ICallback
{
public:
    //DECLARE_FRAME_WND_CLASS_EX(_T("ATLMenu"), IDR_MAINFRAME, CS_VREDRAW | CS_HREDRAW, COLOR_WINDOW);
    //DECLARE_FRAME_WND_CLASS(_T("ATLMenu"), IDR_MAINFRAME)
private:
    ATLLabel m_label;
    ATLButton m_btn;
    std::vector<ATLButton*> m_btnlist;
    HMENU m_hMenu;
    CMenuHandle m_hmenuHandle;
    menuStyle m_style;
public:
    ATLMain();
    ~ATLMain();
public:
    BEGIN_MSG_MAP(ATLMain); // 利用宏实现ProcessWindowMessage函数，用以分发消息
     MESSAGE_HANDLER(WM_CREATE, OnCreate);
    MESSAGE_HANDLER(WM_DESTROY, OnDestroy);
    MESSAGE_HANDLER(WM_CLOSE, OnClose);
    MESSAGE_HANDLER(WM_SIZE, OnSize);
    MESSAGE_HANDLER(WM_PAINT, OnPaint);
    MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown);
    MESSAGE_HANDLER(WM_RBUTTONDOWN, OnRButtonDown);
    MESSAGE_HANDLER(WM_deleteCtrl, OnDeleteCtrl);
    MESSAGE_HANDLER(WM_test, OnTest);
    MESSAGE_HANDLER(WM_INITMENUPOPUP, OnInitMenuPopup);
    COMMAND_ID_HANDLER(ID_STYLE_QT, OnMenuClick);
    COMMAND_ID_HANDLER(ID_STYLE_ATL, OnMenuClick);
    COMMAND_ID_HANDLER(WM_test, OnCmdNewNotebook);
    END_MSG_MAP()

public:
    // Message handlers
    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDeleteCtrl(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnCmdNewNotebook(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    //menu
    LRESULT OnInitMenuPopup(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnMenuClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

    //ICallback
    void OnCtrlCallback(ATLControl* pCtrl);


    //////////////////////////////////////////////////////////////////////
    void InitTopMenuPopup(CMenuHandle popup, UINT index);
};

