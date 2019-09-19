#pragma once
#include <string>

class funlog
{
public:
    funlog(std::wstring datamsg);
    funlog(std::string datamsg);
    ~funlog();
private:
    std::wstring m_datamsg;
};
#define InOutlog(str) funlog logfun(str);