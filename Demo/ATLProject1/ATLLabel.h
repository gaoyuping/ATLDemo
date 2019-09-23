#pragma once
#include <atlwin.h>
#include <atltypes.h>
#include <atlctl.h>

class ATLLabel;
class ICallback
{
public:
    // This is called when creating thumb zoom control. Host application is
    // supposed to perform additional initialization if required...

    virtual void OnCtrlCallback(ATLLabel* pCtrl) = 0;
};

class ATLLabel : public ATL::CWindowImpl<ATLLabel>,public ATL::CComControlBase
{
public:
    ATLLabel(ICallback* icallback = nullptr);
     virtual~ATLLabel();
protected:
    ICallback* m_icallback;

    CRect m_rect;
public:
    BEGIN_MSG_MAP(ATLLabel); // 利用宏实现ProcessWindowMessage函数，用以分发消息
    MESSAGE_HANDLER(WM_CREATE, OnCreate);
    MESSAGE_HANDLER(WM_DESTROY, OnDestroy);
    MESSAGE_HANDLER(WM_CLOSE, OnClose);
    MESSAGE_HANDLER(WM_SIZE, OnSize);
    MESSAGE_HANDLER(WM_PAINT, OnPaint);
    MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus);
    MESSAGE_HANDLER(WM_KILLFOCUS, OnKillFocus);
    END_MSG_MAP()

public:
    // CComControlBase
    HWND CreateControlWindow(        _In_ HWND hWndParent,        _In_ RECT& rcPos);
    virtual HRESULT ControlQueryInterface(        _In_ const IID& iid,        _Outptr_ void** ppv);
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

