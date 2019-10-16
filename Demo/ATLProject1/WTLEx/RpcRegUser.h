
// This file is extension of RPC proxy registration for current user only.
// Usage: include it into "dlldatax.c" after #include "xxx_p.c", following with
//
// extern HINSTANCE hProxyDll;
// STDAPI PrxDllRegisterServerForUser(void)
// { 
//     return NdrDllRegisterProxyForUser(hProxyDll, aProxyFileList, GET_DLL_CLSID);
// }
// STDAPI PrxDllUnregisterServerForUser(void)
// {
//     return NdrDllUnregisterProxyForUser(hProxyDll, aProxyFileList, GET_DLL_CLSID);
// }

// Created by akobern 02\Apr\2007


#ifndef AFX_RPCREGUSER_H_INCLUDED
#define AFX_RPCREGUSER_H_INCLUDED

#include <ObjIdl.h>
#include <RpcProxy.h>

// ported from http://www.reactos.org/generated/doxygen/d1/d82/cpsf_8c-source.html#l00185

__inline HRESULT RPC_ENTRY NdrDllRegisterProxyForUser(
  HMODULE hDll,
  const ProxyFileInfo** pProxyFileList,
  const CLSID* pclsid
)
{
	WCHAR achKey[128];
	WCHAR achClsid[64];
	LONG err;
	HRESULT hr = S_OK;
	HKEY hKey;
	WCHAR achModule[MAX_PATH];
	HKEY hSubKey;

	LPOLESTR pstrClsid;
	StringFromCLSID(pclsid, &pstrClsid);
	wsprintf(achClsid, L"%ls", pstrClsid);

	// register interfaces to point to clsid

	while (*pProxyFileList) {
		
		UINT u;
		for (u = 0; u < (*pProxyFileList)->TableSize; u++) {
			LPOLESTR pstrPiid;
			WCHAR achValue[64];
			
			CInterfaceStubVtbl *proxy = (*pProxyFileList)->pStubVtblList[u];
			PCInterfaceName name = (*pProxyFileList)->pNamesArray[u];

			// create key and value names
			
			StringFromCLSID(proxy->header.piid, &pstrPiid);
			wsprintf(achKey, L"Software\\Classes\\Interface\\%ls", pstrPiid);
			CoTaskMemFree(pstrPiid);
			wsprintf(achValue, L"%u", proxy->header.DispatchTableCount);
			
			// register

			err = RegCreateKeyExW(HKEY_CURRENT_USER, achKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);
			if (err != ERROR_SUCCESS) {
				hr = HRESULT_FROM_WIN32(err);
				goto End;
			}

			if (name) {
				RegSetValueExA(hKey, NULL, 0, REG_SZ, (const BYTE*)name, strlen(name) + 1);
			}

			err = RegCreateKeyExW(hKey, L"ProxyStubClsid32", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hSubKey, NULL);
			if (err != ERROR_SUCCESS) {
				RegCloseKey(hKey);
				hr = HRESULT_FROM_WIN32(err);
				goto End;
			}
			RegSetValueExW(hSubKey, NULL, 0, REG_SZ, (const BYTE*)achClsid, (wcslen(achClsid) + 1) * sizeof(WCHAR));
			RegCloseKey(hSubKey);

			err = RegCreateKeyExW(hKey, L"NumMethods", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hSubKey, NULL);
			if (err != ERROR_SUCCESS) {
				RegCloseKey(hKey);
				hr = HRESULT_FROM_WIN32(err);
				goto End;
			}

			RegSetValueExW(hSubKey, NULL, 0, REG_SZ, (const BYTE*)achValue, (wcslen(achValue) + 1) * sizeof(WCHAR));
			RegCloseKey(hSubKey);

			RegCloseKey(hKey);
		}
		pProxyFileList++;
	}

	// register clsid to point to module

	wsprintf(achKey, L"Software\\Classes\\CLSID\\%ls", pstrClsid);
	err = RegCreateKeyExW(HKEY_CURRENT_USER, achKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);
	if (err != ERROR_SUCCESS) {
		hr = HRESULT_FROM_WIN32(err);
		goto End;
	}
	RegSetValueExW(hKey, NULL, 0, REG_SZ, (const BYTE*)L"PSFactoryBuffer", (wcslen(L"PSFactoryBuffer") + 1) * sizeof(WCHAR));
	
	err = RegCreateKeyExW(hKey, L"InProcServer32", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hSubKey, NULL);
	if (err != ERROR_SUCCESS) {
				RegCloseKey(hKey);
				hr = HRESULT_FROM_WIN32(err);
				goto End;
	}
	GetModuleFileNameW(hDll, achModule, MAX_PATH);
	RegSetValueExW(hSubKey, NULL, 0, REG_SZ, (const BYTE*)achModule, (wcslen(achModule) + 1) * sizeof(WCHAR));
	RegSetValueExW(hSubKey, L"ThreadingModel", 0, REG_SZ, (const BYTE*)L"Both", (wcslen(L"Both") + 1) * sizeof(WCHAR));
	
	RegCloseKey(hSubKey);
	RegCloseKey(hKey);

End:	
	CoTaskMemFree(pstrClsid);
	return hr;
}

__inline HRESULT RPC_ENTRY NdrDllUnregisterProxyForUser(
  HMODULE hDll,
  const ProxyFileInfo** pProxyFileList,
  const CLSID* pclsid
)
{
	WCHAR achKey[128];
	LONG err;
	
	LPOLESTR pstrClsid;

	// unregister interfaces

	while (*pProxyFileList) {
		
		UINT u;
		for (u = 0; u < (*pProxyFileList)->TableSize; u++) {
			LPOLESTR pstrPiid;
			CInterfaceStubVtbl *proxy = (*pProxyFileList)->pStubVtblList[u];
			StringFromCLSID(proxy->header.piid, &pstrPiid);
			wsprintf(achKey, L"Software\\Classes\\Interface\\%ls", pstrPiid);
			CoTaskMemFree(pstrPiid);
			err = RegDeleteKeyW(HKEY_CURRENT_USER, achKey);
			if (err != ERROR_SUCCESS)
				return HRESULT_FROM_WIN32(err);

		}
	}

	// unregister clsid

	StringFromCLSID(pclsid, &pstrClsid);
	wsprintf(achKey, L"Software\\Classes\\CLSID\\%ls", pstrClsid);
	CoTaskMemFree(pstrClsid);
	err = RegDeleteKeyW(HKEY_CURRENT_USER, achKey);
	if (err != ERROR_SUCCESS)
		return HRESULT_FROM_WIN32(err);

	return S_OK;
}

#endif // AFX_RPCREGUSER_H_INCLUDED
