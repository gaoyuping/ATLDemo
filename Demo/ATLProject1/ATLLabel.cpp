#include "stdafx.h"
#include "ATLLabel.h"
#include "LogInfo.h"
#include <atlwin.h>

ATLLabel::ATLLabel(ICallback* icallback)
{
    ATLLabel(UILabel, icallback);
}

ATLLabel::ATLLabel(ATLUISTYLE style, ICallback* icallback):
    ATLControl(UILabel, icallback)
{
    InOutlog(__FUNCTION__);
    m_cName = "ATLLabel";
}


ATLLabel::~ATLLabel()
{
    InOutlog(__FUNCTION__);
    m_cName = "ATLLabel";
}

LRESULT ATLLabel::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    bHandled = false;
    return 0;
}


LRESULT ATLLabel::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    bHandled = false;
    return 0;
}


LRESULT ATLLabel::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    bHandled = false;

    return FALSE;
}


LRESULT ATLLabel::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    bHandled = false;
    GetClientRect(&m_rect);
    return 0;
}

LRESULT ATLLabel::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);

    return __super::OnPaint(uMsg, wParam, lParam, bHandled);
}

LRESULT ATLLabel::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    bHandled = false;

    return FALSE;
}

LRESULT ATLLabel::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InOutlog(__FUNCTION__);
    bHandled = false;

    return FALSE;
}
