#pragma once
#include "stdafx.h"

#define  WM_deleteCtrl WM_USER+1000

#define RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
class ATLControl;
class ICallback
{
public:
    // This is called when creating thumb zoom control. Host application is
    // supposed to perform additional initialization if required...

    virtual void OnCtrlCallback(ATLControl* pCtrl) = 0;
};

enum ATLUISTYLE
{
    UIControl = 0,
    UILabel,
    UIButton,

    UIMenu,
    UILayout,
};

class ATLControl : public ATL::CWindowImpl<ATLControl>
{
public:
    ATLControl(ATLUISTYLE style = UIControl, ICallback* icallback = nullptr);
     virtual~ATLControl();
protected:
    ICallback* m_icallback;
    ATL::CString m_cName;
    CRect m_rect;
protected:
    ATLUISTYLE m_style;

    int m_iwidth;
    int m_iheight;

    int m_iBorderTop;
    int m_iBorderRight;
    int m_iBorderBotton;
    int m_iBorderLeft;
    COLORREF m_borderColor;
    COLORREF m_backgroundColor;
public:
    ATLUISTYLE getStyle();
    
    void setSize(int iw, int ih);

    void setBorderSize(int itop, int iright, int ibotton, int ileft);
    
    void setBorderColor(COLORREF bordercolor);

    void setBackgroundColor(COLORREF bordercolor);

    int getWidth();
    
    int getHeight();
public:
    BEGIN_MSG_MAP(ATLControl); // 利用宏实现ProcessWindowMessage函数，用以分发消息
    MESSAGE_HANDLER(WM_CREATE, OnCreate);
    MESSAGE_HANDLER(WM_DESTROY, OnDestroy);
    MESSAGE_HANDLER(WM_CLOSE, OnClose);
    MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow);
    MESSAGE_HANDLER(WM_SIZE, OnSize);
    MESSAGE_HANDLER(WM_PAINT, OnPaint);
    MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus);
    MESSAGE_HANDLER(WM_KILLFOCUS, OnKillFocus);
    MESSAGE_HANDLER(WM_deleteCtrl, OnDeleteCtrl);
    END_MSG_MAP()


public:
    // Message handlers
    virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual LRESULT OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    virtual LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual LRESULT OnDeleteCtrl(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    

protected:
    inline void DrawBorder(CPaintDC &dc);
    inline void DrawBackgroundColor(CPaintDC &dc);
};

