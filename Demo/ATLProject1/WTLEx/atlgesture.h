//
// Windows Template Library Extensions
//
// This file is NOT part of Windows Template Library.
// The code and information is provided "as-is" without
// warranty of any kind, either expressed or implied.
//
// History: 13/Jan/2010 kvitekp - Initially created
//

#ifndef __ATLGESTURE_H__
#define __ATLGESTURE_H__

#pragma once

#ifndef __cplusplus
	#error ATL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLAPP_H__
	#error atlgesture.h requires atlapp.h to be included first
#endif

#ifndef __ATLWIN_H__
	#error atlgesture.h requires atlwin.h to be included first
#endif

// Include Windows 7 gesture declarations

#ifndef WM_GESTURE
#include "WinGesture.h"
#endif

///////////////////////////////////////////////////////////////////////////////
// Classes in this file
//
// CGestureAPI
// CGestureHandler<T>
//

namespace WTL
{

///////////////////////////////////////////////////////////////////////////////
// CGestureAPI helper class
///////////////////////////////////////////////////////////////////////////////

class CGestureAPI
{
public:

	// Gesture API function prototypes

	typedef BOOL (WINAPI *PFNGetGestureInfo)(HGESTUREINFO hGestureInfo, PGESTUREINFO pGestureInfo);
	typedef BOOL (WINAPI *PFNCloseGestureInfoHandle)(HGESTUREINFO hGestureInfo);
	typedef BOOL (WINAPI *PFNSetGestureConfig)(HWND hwnd, DWORD dwReserved, UINT cIDs, PGESTURECONFIG pGestureConfig, UINT cbSize);

	// Gesture API function pointers

	static PFNGetGestureInfo pfnGetGestureInfo;
	static PFNCloseGestureInfoHandle pfnCloseGestureInfoHandle;
	static PFNSetGestureConfig pfnSetGestureConfig;

	static int nIsGestureSupported;

	// Load library helper class

	struct SafeLoadLibrary
	{
		HMODULE m_hModule;

		SafeLoadLibrary(LPCTSTR pszLibrary, UINT flags = 0)
		{
			m_hModule = ::LoadLibraryEx(pszLibrary, NULL, flags);
		}

		~SafeLoadLibrary()
		{
			if (m_hModule) ::FreeLibrary(m_hModule);
		}

		inline operator HMODULE() const { return m_hModule; }
	
	};

	// Gesture API initializer

	static bool IsGestureSupported()
	{
		if (nIsGestureSupported == -1) {

			CStaticDataInitCriticalSectionLock lock;
			if (FAILED(lock.Lock())) {
				ATLTRACE2(atlTraceUI, 0, _T("CGestureAPI::IsGestureSupported: Unable to lock critical section in CGestureHandler::IsGestureSupported\n"));
				ATLASSERT(FALSE);
				return false;
			}

			if (nIsGestureSupported == -1) {

				static SafeLoadLibrary userDll(_T("user32.dll"));

				#define SETPROCADDR(proc) pfn##proc = (PFN##proc)::GetProcAddress(userDll, #proc)

				nIsGestureSupported =
					(SETPROCADDR(GetGestureInfo)) != NULL &&
					(SETPROCADDR(CloseGestureInfoHandle)) != NULL &&
					(SETPROCADDR(SetGestureConfig)) != NULL
					? 1 : 0;

				#undef SETPROCADDR
			}

			lock.Unlock();
		}

		ATLASSERT(nIsGestureSupported != -1);

		return (nIsGestureSupported == 1);
	}

	// Gesture API function wrappers

	static BOOL GetGestureInfo(HGESTUREINFO hGestureInfo, PGESTUREINFO pGestureInfo)
	{
		return IsGestureSupported() ? pfnGetGestureInfo(hGestureInfo, pGestureInfo) : false;
	}

	static BOOL CloseGestureInfoHandle(HGESTUREINFO hGestureInfo)
	{
		return IsGestureSupported() ? pfnCloseGestureInfoHandle(hGestureInfo) : false;
	}

	static BOOL SetGestureConfig(HWND hwnd, DWORD dwReserved, UINT cIDs, PGESTURECONFIG pGestureConfig, UINT cbSize)
	{
		return IsGestureSupported() ? pfnSetGestureConfig(hwnd, dwReserved, cIDs, pGestureConfig, cbSize) : false;
	}

};

// Gesture API static initializers

__declspec(selectany) CGestureAPI::PFNGetGestureInfo CGestureAPI::pfnGetGestureInfo;
__declspec(selectany) CGestureAPI::PFNCloseGestureInfoHandle CGestureAPI::pfnCloseGestureInfoHandle;
__declspec(selectany) CGestureAPI::PFNSetGestureConfig CGestureAPI::pfnSetGestureConfig;

__declspec(selectany) int CGestureAPI::nIsGestureSupported = -1;

///////////////////////////////////////////////////////////////////////////////
// CGestureHandler class: MI class for CWindowImpl derived classes
///////////////////////////////////////////////////////////////////////////////

template <class T>
class CGestureHandler
{
public:

	///////////////////////////////////////////////////////////////////////////
	// Attributes

	CPoint m_ptFirst;
	CPoint m_ptSecond;
    DWORD m_dwArguments;

	///////////////////////////////////////////////////////////////////////////
	// Construction/destruction

	CGestureHandler()
	:	m_ptFirst(0)
	,	m_ptSecond(0)
	,	m_dwArguments(0)
	{
		CGestureAPI::IsGestureSupported();
	}

	~CGestureHandler()
	{
	}

	///////////////////////////////////////////////////////////////////////////
	// Operations

	///////////////////////////////////////////////////////////////////////////
	// Message map
	
	BEGIN_MSG_MAP(CGestureHandler<T>)
		MESSAGE_HANDLER(WM_GESTURE, OnGesture)
		MESSAGE_HANDLER(WM_GESTURENOTIFY, OnGestureNotify)
	END_MSG_MAP()

	///////////////////////////////////////////////////////////////////////////
	// Message handlers
	
	LRESULT OnGesture(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
//		ATLTRACE(_T("CGestureHandler::OnGesture: wParam=%u lParam=%08x\n"), wParam, lParam);

		T* pT = static_cast<T*>(this);

		// Retrieve the gesture info and check if ok

		GESTUREINFO gi; ZeroMemory(&gi, sizeof(GESTUREINFO)); gi.cbSize = sizeof(GESTUREINFO);

		if (!CGestureAPI::GetGestureInfo((HGESTUREINFO)lParam, &gi)) {
			bHandled = false;
			return false;
		}

		// Handle gesture

		switch (gi.dwID) {
		case GID_BEGIN: bHandled = pT->OnBeginGesture(gi); break;
		case GID_END: bHandled = pT->OnEndGesture(gi); break;
		case GID_ZOOM: bHandled = pT->OnZoomGesture(gi); break;
		case GID_PAN:  bHandled = pT->OnPanGesture(gi); break;
		case GID_ROTATE: bHandled = pT->OnRotateGesture(gi); break;
		case GID_TWOFINGERTAP: bHandled = pT->OnTwoFingerTapGesture(gi); break;
		case GID_PRESSANDTAP: bHandled = pT->OnPressAndTapGesture(gi); break;
		default: bHandled = false; break;
		}

		// Check if handled and close gesture handle

		if (bHandled) CGestureAPI::CloseGestureInfoHandle((HGESTUREINFO)lParam);

		return 0;
	}

	LRESULT OnGestureNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
//		ATLTRACE(_T("CGestureHandler::OnGestureNotify: wParam=%u lParam=%08x\n"), wParam, lParam);

		T* pT = static_cast<T*>(this);

		// Handle gesture notification

		pT->OnGestureNotify(*((GESTURENOTIFYSTRUCT*)lParam));

		// Always pass it to default window procedure

		bHandled = false;

		return 0;
	}

	///////////////////////////////////////////////////////////////////////////
	// Overridables

	bool OnBeginGesture(GESTUREINFO& gi)
	{
//		TRACE(_T("CGestureHandler::OnBeginGesture: \n"));

		return false;
	}

	bool OnEndGesture(GESTUREINFO& gi)
	{
//		TRACE(_T("CGestureHandler::OnEndGesture: \n"));

		return false;
	}

	bool OnZoomGesture(GESTUREINFO& gi)
	{
//		TRACE(_T("CGestureHandler::OnZoomGesture: \n"));

		T* pT = static_cast<T*>(this);

		// Save the first point of the gesture

		if (gi.dwFlags & GF_BEGIN) {
			m_dwArguments = (DWORD)gi.ullArguments;
			m_ptFirst.x = gi.ptsLocation.x;
			m_ptFirst.y = gi.ptsLocation.y;
			pT->ScreenToClient(&m_ptFirst);
		} else {

			// Save the second point of the gesture. This is middle point between 
			// fingers in this new position.

			m_ptSecond.x = gi.ptsLocation.x;
			m_ptSecond.y = gi.ptsLocation.y;
			pT->ScreenToClient(&m_ptSecond);

			// Calculate zoom center point 

			CPoint ptZoomCenter;
			ptZoomCenter.x = (m_ptFirst.x + m_ptSecond.x) / 2;
			ptZoomCenter.y = (m_ptFirst.y + m_ptSecond.y) / 2;           

			// The zoom factor is the ratio between the new and the old distance. 
			// The new distance between two fingers is stored in gi.ullArguments 
			// (lower DWORD) and the old distance is stored in _dwArguments.

			double zoomFactor = (double)((DWORD)gi.ullArguments) / (double)(m_dwArguments);

			// Now we process zooming in/out of the object

			if (zoomFactor != 1.0) {
				pT->HandleZoomGesture(ptZoomCenter, zoomFactor);
			}
			
			// Save new information as a starting information 
			// for the next step in this gesture.

			m_ptFirst = m_ptSecond;
			m_dwArguments = (DWORD)gi.ullArguments;
		}

		return true;
	}

	bool OnPanGesture(GESTUREINFO& gi)
	{
//		TRACE(_T("CGestureHandler::OnPanGesture: \n"));

		T* pT = static_cast<T*>(this);

		// Save the first point of the gesture

		if (gi.dwFlags & GF_BEGIN) {
			m_ptFirst.x = gi.ptsLocation.x;
			m_ptFirst.y = gi.ptsLocation.y;
			pT->ScreenToClient(&m_ptFirst);
		} else {

			// Save the second point of the gesture. This is middle point between 
			// fingers in this new position.

			m_ptSecond.x = gi.ptsLocation.x;
			m_ptSecond.y = gi.ptsLocation.y;
			pT->ScreenToClient(&m_ptSecond);

			// Calculate delta

			int dx = m_ptSecond.x - m_ptFirst.x;
			int dy = m_ptSecond.y - m_ptFirst.y;
			if (dx || dy) {
				pT->HandlePanGesture(m_ptFirst, dx, dy);
			}

			// Save new information as a starting information 
			// for the next step in this gesture.

			m_ptFirst = m_ptSecond;
		}

		return true;
	}

	bool OnRotateGesture(GESTUREINFO& gi)
	{
//		TRACE(_T("CGestureHandler::OnRotateGesture: \n"));

		return false;
	}

	bool OnTwoFingerTapGesture(GESTUREINFO& gi)
	{
//		TRACE(_T("CGestureHandler::OnTwoFingerTapGesture: \n"));

		return false;
	}

	bool OnPressAndTapGesture(GESTUREINFO& gi)
	{
//		TRACE(_T("CGestureHandler::OnPressAndTapGesture: \n"));

		return false;
	}

	void OnGestureNotify(GESTURENOTIFYSTRUCT& gn)
	{
	}

	// Cooked gesture handlers

	bool HandleZoomGesture(const CPoint& ptZoomCenter, double zoomFactor)
	{
		TRACE(_T("CGestureHandler::HandleZoomGesture: (%d,%d) %f\n"), ptZoomCenter.x, ptZoomCenter.y, zoomFactor);

		return false;
	}

	bool HandlePanGesture(const CPoint& ptStart, int dx, int dy)
	{
		TRACE(_T("CGestureHandler::HandlePanGesture: (%d,%d) (%d,%d)\n"), ptStart.x, ptStart.y, dx, dy);

		return true;
	}

};

}; //namespace WTL

#endif __ATLGESTURE_H__
