﻿// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef STRICT
#define STRICT
#endif

#include "targetver.h"

#define _ATL_APARTMENT_THREADED

#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS    // 某些 CString 构造函数将是显式的

#define _WTL_NO_CSTRING
#define _WTL_NO_WTYPES
#include <atltypes.h>
#include <atlfile.h>
#include <comsvcs.h>
#include <atlapp.h>
#include <atlgdi.h>
#define ATL_NO_ASSERT_ON_DESTROY_NONEXISTENT_WINDOW
#include <atlbase.h>
#include <atlapp.h>
#include <atlgdi.h>
#undef _WINDOWS_


#include "resource.h"
//#include <afxwin.h>
#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>
#include <afx.h>

#pragma warning (disable:4100)