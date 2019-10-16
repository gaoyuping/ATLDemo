//
// Windows Template Library Extensions
//
// This file is NOT part of Windows Template Library.
// The code and information is provided "as-is" without
// warranty of any kind, either expressed or implied.
//

#ifndef __ATLIMAGEFORMAT_H__
#define __ATLIMAGEFORMAT_H__

#pragma once

#ifndef __cplusplus
	#error ATL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLAPP_H__
	#error atlmiscex.h requires atlapp.h to be included first
#endif

#ifndef __ATLWIN_H__
	#error atlmiscex.h requires atlwin.h to be included first
#endif

///////////////////////////////////////////////////////////////////////////////
// CImageFormatHelper - GDI to Mime conversion helper

class CImageFormatHelper
{
public:

	static bool GetGUIDByMimeType(const CString& mime, GUID& guid)
	{
		InitMap();

		std::map<CString, GUID>::const_iterator it = m_mimeGuidMap.find(mime);
		if (it == m_mimeGuidMap.end())
			return false;

		guid = it->second;
		return true;
	}

private:
	
	static std::map<CString, GUID> m_mimeGuidMap;
	
	static void InitMap()
	{
		// Check if maps has already been initialized

		if (!m_mimeGuidMap.empty())
			return;

		// Initialize map with GUIDs for known data types

		m_mimeGuidMap[_T("image/jpeg")]	= Gdiplus::ImageFormatJPEG;
		m_mimeGuidMap[_T("image/jpg")]	= Gdiplus::ImageFormatJPEG;
		m_mimeGuidMap[_T("image/png")]	= Gdiplus::ImageFormatPNG;	
		m_mimeGuidMap[_T("image/gif")]	= Gdiplus::ImageFormatGIF;
		m_mimeGuidMap[_T("image/bmp")]	= Gdiplus::ImageFormatBMP;
	}
};

__declspec(selectany) std::map<CString, GUID> CImageFormatHelper::m_mimeGuidMap;

#endif // __ATLIMAGEFORMAT_H__
