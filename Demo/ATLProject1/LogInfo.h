#pragma once
#include <string>
#include <chrono>

using tmieclock = std::chrono::time_point<std::chrono::system_clock>;
class funlog
{
public:
    funlog(std::wstring datamsg);
    funlog(std::string datamsg);
    ~funlog();
private:
    std::wstring m_datamsg;
    tmieclock m_start;
};
#define InOutlog(str) funlog logfun(str);