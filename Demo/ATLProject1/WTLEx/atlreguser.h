//
// Active Template Library Extensions
//
// This file is NOT part of Active Template Library.
// The code and information is provided "as-is" without
// warranty of any kind, either expressed or implied.
//
// History: 30/Mar/2007 kober - Initially created
//
// $Id$

#ifndef AFX_ATLREGUSER_H_INCLUDED
#define AFX_ATLREGUSER_H_INCLUDED

///////////////////////////////////////////////////////////////////////////////
// Those two fucntions first declared in Windows 6.0 SDK.
// When we will use this SDK, we should remove those declarations from here.
// Functions not exist in Windows 2000

// Registers a type library for use by the calling user.

WINOLEAUTAPI RegisterTypeLibForUser(ITypeLib * ptlib, OLECHAR  *szFullPath,
            OLECHAR  *szHelpDir);
typedef HRESULT (__stdcall * PfnRegisterTypeLibForUser)(ITypeLib * ptlib, OLECHAR  *szFullPath,
            OLECHAR  *szHelpDir);

// Removes type library information that was registered by using RegisterTypeLibForUser. 

WINOLEAUTAPI UnRegisterTypeLibForUser(REFGUID libID, WORD wVerMajor,
            WORD wVerMinor, LCID lcid, SYSKIND syskind);
typedef HRESULT (__stdcall * PfnUnRegisterTypeLibForUser)(REFGUID libID, WORD wVerMajor,
            WORD wVerMinor, LCID lcid, SYSKIND syskind);


namespace ATL
{
	// global flag; determines how interfaces and typelib should be registered;
	// should be set before any registering functions will be called

	static BOOL g_ForUser = FALSE;

	// extends DECLARE_REGISTRY_RESOURCE define for per-machine/per-user case
	// x - string identifier of registry resource with %ROOTKEY% root handle

#define DECLARE_REGISTRY_RESOURCE_EX(x)\
	static HRESULT WINAPI UpdateRegistry(BOOL bRegister) throw()\
	{\
		__if_exists(_GetMiscStatus) \
		{ \
			ATL::_ATL_REGMAP_ENTRY regMapEntries[3]; \
			memset(&regMapEntries[2], 0, sizeof(ATL::_ATL_REGMAP_ENTRY)); \
			regMapEntries[0].szKey = L"OLEMISC"; \
			TCHAR szOleMisc[32]; \
			ATL::Checked::itot_s(_GetMiscStatus(), szOleMisc, _countof(szOleMisc), 10); \
			USES_CONVERSION_EX; \
			regMapEntries[0].szData = T2OLE_EX(szOleMisc, _ATL_SAFE_ALLOCA_DEF_THRESHOLD); \
			if (regMapEntries[0].szData == NULL) \
				return E_OUTOFMEMORY; \
			regMapEntries[1].szKey = L"ROOTKEY"; \
			regMapEntries[1].szData = g_ForUser ? L"HKCU" : L"HKLM"; \
			__if_exists(_Module) \
			{ \
				return _Module.UpdateRegistryFromResource(_T(#x), bRegister, regMapEntries); \
			} \
			__if_not_exists(_Module) \
			{ \
				return ATL::_pAtlModule->UpdateRegistryFromResource(_T(#x), bRegister, regMapEntries); \
			} \
		} \
		__if_not_exists(_GetMiscStatus) \
		{ \
			ATL::_ATL_REGMAP_ENTRY regMapEntries[2]; \
			memset(&regMapEntries[1], 0, sizeof(ATL::_ATL_REGMAP_ENTRY)); \
			regMapEntries[0].szKey = L"ROOTKEY"; \
			regMapEntries[0].szData = g_ForUser ? L"HKCU" : L"HKLM"; \
			__if_exists(_Module) \
			{ \
				return _Module.UpdateRegistryFromResource(_T(#x), bRegister, regMapEntries); \
			} \
			__if_not_exists(_Module) \
			{ \
				return ATL::_pAtlModule->UpdateRegistryFromResource(_T(#x), bRegister, regMapEntries); \
			} \
		} \
	}

	// extends DECLARE_REGISTRY_RESOURCEID define for per-machine/per-user case
	// x - string identifier of registry resource with %ROOTKEY% root handle

#define DECLARE_REGISTRY_RESOURCEID_EX(x)\
	static HRESULT WINAPI UpdateRegistry(BOOL bRegister) throw()\
	{\
		__if_exists(_GetMiscStatus) \
		{ \
			ATL::_ATL_REGMAP_ENTRY regMapEntries[3]; \
			memset(&regMapEntries[2], 0, sizeof(ATL::_ATL_REGMAP_ENTRY)); \
			regMapEntries[0].szKey = L"OLEMISC"; \
			TCHAR szOleMisc[32]; \
			ATL::Checked::itot_s(_GetMiscStatus(), szOleMisc, _countof(szOleMisc), 10); \
			USES_CONVERSION_EX; \
			regMapEntries[0].szData = T2OLE_EX(szOleMisc, _ATL_SAFE_ALLOCA_DEF_THRESHOLD); \
			if (regMapEntries[0].szData == NULL) \
				return E_OUTOFMEMORY; \
			regMapEntries[1].szKey = L"ROOTKEY"; \
			regMapEntries[1].szData = g_ForUser ? L"HKCU" : L"HKLM"; \
			__if_exists(_Module) \
			{ \
				return _Module.UpdateRegistryFromResource(x, bRegister, regMapEntries); \
			} \
			__if_not_exists(_Module) \
			{ \
				return ATL::_pAtlModule->UpdateRegistryFromResource(x, bRegister, regMapEntries); \
			} \
		} \
		__if_not_exists(_GetMiscStatus) \
		{ \
			ATL::_ATL_REGMAP_ENTRY regMapEntries[2]; \
			memset(&regMapEntries[1], 0, sizeof(ATL::_ATL_REGMAP_ENTRY)); \
			regMapEntries[0].szKey = L"ROOTKEY"; \
			regMapEntries[0].szData = g_ForUser ? L"HKCU" : L"HKLM"; \
			__if_exists(_Module) \
			{ \
				return _Module.UpdateRegistryFromResource(x, bRegister, regMapEntries); \
			} \
			__if_not_exists(_Module) \
			{ \
				return ATL::_pAtlModule->UpdateRegistryFromResource(x, bRegister, regMapEntries); \
			} \
		} \
	}

// extends AtlUnRegisterTypeLib function for per-machine/per-user case

ATLINLINE ATLAPI AtlUnRegisterTypeLibEx(HINSTANCE hInstTypeLib, LPCOLESTR lpszIndex)
{
	CComBSTR bstrPath;
	CComPtr<ITypeLib> pTypeLib;
	HRESULT hr = AtlLoadTypeLib(hInstTypeLib, lpszIndex, &bstrPath, &pTypeLib);
	if (SUCCEEDED(hr))
	{
		TLIBATTR* ptla;
		hr = pTypeLib->GetLibAttr(&ptla);
		if (SUCCEEDED(hr))
		{
			if (!g_ForUser) {

				// for all users - use well-known function

				hr = ::UnRegisterTypeLib(ptla->guid, ptla->wMajorVerNum, ptla->wMinorVerNum, ptla->lcid, ptla->syskind);
			} else {

				// for current user - get special function
				
				HINSTANCE hLib = ::LoadLibrary(_T("oleaut32.dll"));
				if (!hLib) 
					hr = HRESULT_FROM_WIN32(GetLastError());
				else {
					PfnUnRegisterTypeLibForUser pfnUnRegisterTypeLibForUser = 
						(PfnUnRegisterTypeLibForUser)::GetProcAddress(hLib, "UnRegisterTypeLibForUser");
					if (pfnUnRegisterTypeLibForUser) {

						// special function exists - use it

						hr = pfnUnRegisterTypeLibForUser(ptla->guid, ptla->wMajorVerNum, ptla->wMinorVerNum, ptla->lcid, ptla->syskind);
					} else {

						// special function not exists - try to map HKLM to HKCU

						::RegOverridePredefKey(HKEY_LOCAL_MACHINE, HKEY_CURRENT_USER);
						hr = ::UnRegisterTypeLib(ptla->guid, ptla->wMajorVerNum, ptla->wMinorVerNum, ptla->lcid, ptla->syskind);
						::RegOverridePredefKey(HKEY_LOCAL_MACHINE, NULL);
					}
					::FreeLibrary(hLib);
				}
			}
			pTypeLib->ReleaseTLibAttr(ptla);
		}
	}
	return hr;
}

// extends AtlRegisterTypeLib function for per-machine/per-user case

ATLINLINE ATLAPI AtlRegisterTypeLibEx(HINSTANCE hInstTypeLib, LPCOLESTR lpszIndex)
{
	CComBSTR bstrPath;
	CComPtr<ITypeLib> pTypeLib;
	HRESULT hr = AtlLoadTypeLib(hInstTypeLib, lpszIndex, &bstrPath, &pTypeLib);
	if (SUCCEEDED(hr))
	{
		OLECHAR szDir[MAX_PATH];
		Checked::wcsncpy_s(szDir, MAX_PATH, bstrPath.m_str, bstrPath.Length());

		szDir[MAX_PATH - 1] = 0;
		// If index is specified remove it from the path
		if (lpszIndex != NULL)
		{
			size_t nLenPath = ocslen(szDir);
			size_t nLenIndex = ocslen(lpszIndex);
			if (memcmp(szDir + nLenPath - nLenIndex, lpszIndex, nLenIndex) == 0)
				szDir[nLenPath - nLenIndex] = 0;
		}
		szDir[AtlGetDirLen(szDir)] = 0;
		if (!g_ForUser) {
			
			// for all users - use well-known function

			hr = ::RegisterTypeLib(pTypeLib, bstrPath, szDir);
		} else {

			// for current user - get special function

			HINSTANCE hLib = ::LoadLibrary(_T("oleaut32.dll"));
			if (!hLib) 
				hr = HRESULT_FROM_WIN32(GetLastError());
			else {
				PfnRegisterTypeLibForUser pfnRegisterTypeLibForUser = 
					(PfnRegisterTypeLibForUser)::GetProcAddress(hLib, "RegisterTypeLibForUser");
				if (pfnRegisterTypeLibForUser) {

					// special function exists - use it

					hr = pfnRegisterTypeLibForUser(pTypeLib, bstrPath, szDir);
				} else {

					// special function not exists - try to map HKLM to HKCU

					::RegOverridePredefKey(HKEY_LOCAL_MACHINE, HKEY_CURRENT_USER);
					hr = ::RegisterTypeLib(pTypeLib, bstrPath, szDir);
					::RegOverridePredefKey(HKEY_LOCAL_MACHINE, NULL);
				}
				::FreeLibrary(hLib);
			}
		}
	}
	return hr;
}

} // end of namespace ATL

#endif //  AFX_ATLREGUSER_H_INCLUDED
