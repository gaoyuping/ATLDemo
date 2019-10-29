#include "stdafx.h"
#include "LogInfo.h"
#include <codecvt>


funlog::funlog(std::wstring datamsg)
{
    m_datamsg = datamsg;
    m_start = std::chrono::system_clock::now();
    ATLTRACE(_T("%s in\n"), m_datamsg.c_str());
};
funlog::funlog(std::string datamsg)
{
    std::wstring_convert<std::codecvt<wchar_t, char, mbstate_t>> converter_wtos(new std::codecvt<wchar_t, char, mbstate_t>("CHS"));
    std::wstring_convert<std::codecvt<char, wchar_t, mbstate_t>> converter_stow(new std::codecvt<char, wchar_t, mbstate_t>("CHS"));
    m_start = std::chrono::system_clock::now();
    m_datamsg = converter_wtos.from_bytes(datamsg.c_str());
    //vscolor information
    ATLTRACE(_T("  %s in\n"), m_datamsg.c_str());
};
funlog::~funlog()
{
    tmieclock entime = std::chrono::system_clock::now();
    ATLTRACE(_T("  %s out use time %f s\n"), m_datamsg.c_str(),double(std::chrono::duration_cast<std::chrono::microseconds>(entime - m_start).count())*std::chrono::microseconds::period::num / std::chrono::microseconds::period::den);
};