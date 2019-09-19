#pragma once
#include "ATLLabel.h"

class ATLButton :
    public ATLLabel
{
public:
    ATLButton();
    ~ATLButton();
    BEGIN_MSG_MAP(ATLButton); // 利用宏实现ProcessWindowMessage函数，用以分发消息
    CHAIN_MSG_MAP(ATLLabel)
    MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown);
    MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove);
    MESSAGE_HANDLER(WM_MOUSEHOVER, OnMouseHover);
    MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave);
    END_MSG_MAP()

private:
    bool m_bMouseWithin;
    bool m_bMouseTracking; //m_bMouseTracking = false && m_bMouseWithin = true 是hover状态
private:
    void StartTrack();
public:
    // Message handlers
    virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    virtual LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual LRESULT OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual LRESULT OnMouseEnter(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual LRESULT OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};

