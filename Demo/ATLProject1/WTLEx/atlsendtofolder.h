//
// Windows Template Library Extension for SendTo folder
//
// Written by Pete Kvitek <pete@kvitek.com>
//
// This file is NOT part of Windows Template Library.
// The code and information is provided "as-is" without
// warranty of any kind, either expressed or implied.
//

#ifndef __ATLSENDTOFOLDER_H__
#define __ATLSENDTOFOLDER_H__

#pragma once

#ifndef __cplusplus
	#error ATL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLAPP_H__
	#error atlsendtofolder.h requires atlapp.h to be included first
#endif

#ifndef __ATLWIN_H__
	#error atlsendtofolder.h requires atlwin.h to be included first
#endif

#include <shlobj.h>
#include <shlwapi.h>

/////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CSendToFolder<T>
//

namespace WTL
{

///////////////////////////////////////////////////////////////////////////////
// CSendToFolder class -- implements SendTo folder access

template <class T>
class CSendToFolder
{
public:

	///////////////////////////////////////////////////////////////////////////
	// Attributes

    IMalloc* m_pMalloc;

    IShellFolder* m_psfDesktop;

	///////////////////////////////////////////////////////////////////////////
	// Constructor/destructor

	CSendToFolder()
	{
		// Get shell allocator interface
		
		if (FAILED(::SHGetMalloc(&m_pMalloc))) {
			m_pMalloc = NULL;
		}

		// Get shell desktop folder interface

		if (FAILED(::SHGetDesktopFolder(&m_psfDesktop))) {
			m_psfDesktop = NULL;
		}
	}

	~CSendToFolder()
	{
		// Cleanup

		if (m_psfDesktop) {
			m_psfDesktop->Release();
			m_psfDesktop = NULL;
		}

		if (m_pMalloc) {
			m_pMalloc->Release();
			m_pMalloc = NULL;
		}
	}

	///////////////////////////////////////////////////////////////////////////
	// Operations

	IShellFolder* GetSendToFolder()
	{
		ASSERT(m_pMalloc != NULL); ASSERT(m_psfDesktop != NULL);

		// Get SendTo menu pidl and if ok, bind it to IShellFolder 
		
		LPITEMIDLIST pidl = NULL; IShellFolder* psf = NULL;
		if (SUCCEEDED(::SHGetSpecialFolderLocation(NULL, CSIDL_SENDTO, &pidl))) {
			m_psfDesktop->BindToObject(pidl, NULL, IID_IShellFolder, (LPVOID*)&psf);
			ShellFree(pidl);
		}

		return psf;
	}

	void EnumSendToFolder(IShellFolder* psf)
	{
		ASSERT(psf != NULL); ASSERT(m_pMalloc != NULL); ASSERT(m_psfDesktop != NULL);
		
		T* pT = static_cast<T*>(this);

		// Get shell fodler enumerator interface and check if ok

		IEnumIDList* peidl = NULL;
		if (SUCCEEDED(psf->EnumObjects(NULL, SHCONTF_FOLDERS | SHCONTF_NONFOLDERS, &peidl))) {

			pT->OnEnumSendToFolderStart(psf);

			// Enumerate SendTo folder

			LPITEMIDLIST pidl;
			while (peidl->Next(1, &pidl, NULL) == NOERROR) {

				// Get folder item attributes and check if ok

				SFGAOF sfgaof = SFGAO_LINK | SFGAO_FOLDER;
				if (SUCCEEDED(psf->GetAttributesOf(1, (LPCITEMIDLIST*)&pidl, &sfgaof))) {
					
					pT->OnEnumSendToFolderItem(psf, pidl, sfgaof);

					// Handle subfolder items

					if (sfgaof & SFGAO_FOLDER) {
						IShellFolder* psfSubFolder = NULL;
						if (SUCCEEDED(psf->BindToObject(pidl, NULL, IID_IShellFolder, (LPVOID*)&psfSubFolder))) {
							EnumSendToFolder(psfSubFolder);
							psfSubFolder->Release();
						}
					}
				}

				// Cleanup

				ShellFree(pidl);
			}

			pT->OnEnumSendToFolderEnd(psf);

			peidl->Release();
		}
	}

	void EnumSendToFolder()
	{
		// Get SendTo folder interface and check if ok

		IShellFolder* psf = GetSendToFolder();
		if (psf) {
			EnumSendToFolder(psf);
			psf->Release();
		}
	}

	void ShellFree(void* p)
	{
		if (m_pMalloc) {
			m_pMalloc->Free(p);
		}
	}

	///////////////////////////////////////////////////////////////////////////
	// Helper routines

	static bool GetName(IShellFolder* psf, LPITEMIDLIST pidl, CString& name, UINT flags = SHGDN_NORMAL)
	{
		ASSERT(pidl != NULL);

		// Get display name and check if ok

		STRRET strret;
		if (psf->GetDisplayNameOf(pidl, flags, &strret) != NOERROR)
			return false;

		// Allocate target buffer

		LPTSTR pch = name.GetBufferSetLength(MAX_PATH);
		if (!pch) return false;

		// Copy returned string to beuffer and check if ok

		if (FAILED(::StrRetToBuf(&strret, pidl, pch, MAX_PATH))) {
			name.ReleaseBuffer(0);
			return false;
		} else
			name.ReleaseBuffer();

		return !name.IsEmpty();
	}

	static CString GetName(IShellFolder* psf, LPITEMIDLIST pidl, UINT flags = SHGDN_NORMAL | SHGDN_FORPARSING)
	{
		CString name;
		
		GetName(psf, pidl, name, flags);
		
		return name;
	}

	static HICON GetIcon(IShellFolder* psf, LPITEMIDLIST pidl, bool bLargeIcon = false)
	{
		HICON hIcon = NULL;
		
		// Get IExtractIcon interface poiner and check if ok

		IExtractIcon* pei;
		if (FAILED(psf->GetUIObjectOf(NULL, 1, (LPCITEMIDLIST*)&pidl, IID_IExtractIcon, 0, (LPVOID*)&pei)))
			return false;

		// Get icon location and check if ok

		TCHAR achFile[MAX_PATH]; int iIndex; UINT flags;
		if (SUCCEEDED(pei->GetIconLocation(0, achFile, numbof(achFile), &iIndex, &flags))) {
			
//			TRACE(_T("CSendToFolder::GetIcon: file='%s' index=%d flags=%08x\n"), achFile, iIndex, flags);
			
			// Extract icon and check if ok
			
			HICON hIconLarge = NULL, hIconSmall = NULL;
			if (pei->Extract(achFile, iIndex, &hIconLarge, &hIconSmall, 0) == NOERROR) {
				hIcon = bLargeIcon ? hIconLarge : hIconSmall;
			}
		}

		// Cleanup
		
		pei->Release();

		return hIcon;
	}

	///////////////////////////////////////////////////////////////////////////
	// Overridable routines

	void OnEnumSendToFolderStart(IShellFolder* psf)
	{
		TRACE(_T("CSendToFolder::OnEnumSendToFolderStart: \n"));
	}

	void OnEnumSendToFolderItem(IShellFolder* psf, LPITEMIDLIST pidl, SFGAOF sfgaof)
	{
		SHFILEINFO shfi; ZeroMemory(&shfi, sizeof(shfi));
		static const UINT flags = SHGFI_PIDL | SHGFI_DISPLAYNAME | SHGFI_TYPENAME | SHGFI_ICON | SHGFI_SHELLICONSIZE;
		if (::SHGetFileInfo((LPCTSTR)pidl, 0, &shfi, sizeof(shfi), flags)) {
			if (shfi.hIcon) ::DestroyIcon(shfi.hIcon);
		}

		TRACE(_T("CSendToFolder::OnEnumSendToFolderItem: szDisplayName='%s' szTypeName='%s' dwAttributes=%08x sfgaof=%08x %s%s path='%s'\n"), 
			shfi.szDisplayName, shfi.szTypeName, shfi.dwAttributes, sfgaof,
			sfgaof & SFGAO_LINK	  ? _T("SFGAO_LINK")   : _T(""),
			sfgaof & SFGAO_FOLDER ? _T("SFGAO_FOLDER") : _T(""),
			GetPath(pidl));
	}

	void OnEnumSendToFolderEnd(IShellFolder* psf)
	{
		TRACE(_T("CSendToFolder::OnEnumSendToFolderEnd: \n"));
	}

};

}; // namespace WTL

// Force linking against shlwapi library

#pragma comment(lib, "shlwapi")

#endif // __ATLSENDTOFOLDER_H__
