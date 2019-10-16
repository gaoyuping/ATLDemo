#pragma once
#include <atlwin.h>
#include <atltypes.h>
#include <atlctl.h>
#include "ATLUIControl.h"

class ATLLabel : public ATLControl
{
public:
    ATLLabel(ICallback* icallback = nullptr);
     virtual~ATLLabel();

protected:
    ATLLabel(ATLUISTYLE style, ICallback* icallback = nullptr);
public:
    BEGIN_MSG_MAP(ATLLabel); // 利用宏实现ProcessWindowMessage函数，用以分发消息
    CHAIN_MSG_MAP(ATLControl)
    END_MSG_MAP()

public:
    // CComControlBase
//     HWND CreateControlWindow(        _In_ HWND hWndParent,        _In_ RECT& rcPos);
//     virtual HRESULT ControlQueryInterface(        _In_ const IID& iid,        _Outptr_ void** ppv);
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

};

