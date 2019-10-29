#pragma once
#include <atlwin.h>
#include <atltypes.h>
#include <atlctl.h>
#include "ATLUIControl.h"
#include <atlimage.h>
enum IMAGENUMER {
    NO_IMAGE,
};
class ATLLabel : public ATLControl
{
public:
    ATLLabel(ICallback* icallback = nullptr);
     virtual~ATLLabel();

     bool LoadImage(int Resourceid, int index = 0, int imageCount = 1);

     void FreeImage();

protected:
    ATLLabel(ATLUISTYLE style, ICallback* icallback = nullptr);

    ATL::CImage* m_image;
    CSize m_cTextSize;
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

    virtual void Draw(CMemoryDC &mdc) override;
public:
    void DrawText(CMemoryDC &dc) override;

    virtual void DrawPic(CMemoryDC &dc);
};

