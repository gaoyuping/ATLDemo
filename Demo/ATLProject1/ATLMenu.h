#pragma once
#include <atlwin.h>
#include <atltypes.h>
#include <atlctl.h>
#include "ATLUIControl.h"
#include <vector>

class ATLMenu : public ATLControl
{
public:

    ATLMenu(ICallback* icallback = nullptr);
    virtual~ATLMenu();
    void setSpace(int ispace = 0);

    void show(RECT point);
protected:
    int m_iSpace;

private:
    ATLMenu(ATLUISTYLE style, ICallback* icallback = nullptr);

private:
    std::vector<ATLControl*> m_listCtrl;
public:
    BEGIN_MSG_MAP(ATLMenu); // 利用宏实现ProcessWindowMessage函数，用以分发消息
    CHAIN_MSG_MAP(ATLControl);
    MESSAGE_HANDLER(WM_ACTIVATE, OnActivate)
    END_MSG_MAP()

public:
    // Message handlers
    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;
    LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;
    LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;
    LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;

    LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;
    LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;

    virtual LRESULT OnActivate(UINT msg, WPARAM wp, LPARAM lp, BOOL& bHandled);
public:
    bool addItem(ATLControl* ptrItem, int ipos = -1);
    bool removeItem(const ATLControl* ptrItem);
};

