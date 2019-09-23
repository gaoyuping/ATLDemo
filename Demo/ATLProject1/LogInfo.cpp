#include "stdafx.h"
#include "LogInfo.h"
#include <codecvt>

funlog::funlog(std::wstring datamsg)
{
    m_datamsg = datamsg;
    ATLTRACE(_T("%s in\n"), m_datamsg.c_str());
};
funlog::funlog(std::string datamsg)
{
    std::wstring_convert<std::codecvt<wchar_t, char, mbstate_t>> converter_wtos(new std::codecvt<wchar_t, char, mbstate_t>("CHS"));
    std::wstring_convert<std::codecvt<char, wchar_t, mbstate_t>> converter_stow(new std::codecvt<char, wchar_t, mbstate_t>("CHS"));

    m_datamsg = converter_wtos.from_bytes(datamsg.c_str());
    //vscolor information
    ATLTRACE(_T("  %s in\n"), m_datamsg.c_str());
};
funlog::~funlog()
{
    ATLTRACE(_T("  %s out\n"), m_datamsg.c_str());
};