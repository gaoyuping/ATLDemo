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
private:
    ATLMenu(ATLUISTYLE style, ICallback* icallback = nullptr);
protected:
    CRect m_rect;
private:
    std::vector<ATLControl*> m_listCtrl;
public:
    BEGIN_MSG_MAP(ATLMenu); // 利用宏实现ProcessWindowMessage函数，用以分发消息
    CHAIN_MSG_MAP(ATLControl);
    END_MSG_MAP()

public:
    // Message handlers
    virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    virtual LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
public:
    bool addItem(ATLControl* ptrItem, int ipos = -1);
    bool removeItem(const ATLControl* ptrItem);
};

