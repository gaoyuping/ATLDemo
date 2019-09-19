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
    END_MSG_MAP()
public:
    // Message handlers
    virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};

