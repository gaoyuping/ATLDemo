///////////////////////////////////////////////////////////////////////////////
//
// DCCache.cpp : Implementation for CDCCache and related classes
//
// Copyright 2009 Evernote Corp. All rights reserved.
// 
// History: 11/25/2009 kvitekp - Initially created.
//
// $Id$
//
///////////////////////////////////////////////////////////////////////////////

// Include additional headers
#include "DCCache.h"
#include "Common.h"
// Instantiate dc cache

CDCCache CCachedDC::_dcCache;

///////////////////////////////////////////////////////////////////////////////
// CDCCache class
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// These routines manage dc cache

CDCCache::CDCCache()
{
	for (UINT n = 0; n < numbof(m_ahDCs); n++) {
		m_ahDCs[n] = NULL;
	}
}

CDCCache::~CDCCache()
{
	for (UINT n = 0; n < numbof(m_ahDCs); n++) {
		if (m_ahDCs[n] != NULL) {
			::DeleteDC(m_ahDCs[n]);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// Return cached or create overflow dc if no cahced dc is available

HDC CDCCache::GetDC()
{
	HDC hDC;

	// Scan cached dcs table looking for the available dc and return it if found

	for (UINT n = 0; n < numbof(m_ahDCs); n++) {
		hDC = static_cast<HDC>(InterlockedExchangePointer(reinterpret_cast<void**>(&m_ahDCs[n]), NULL));
		if (hDC)
			return hDC;
	}

	// Otherwise allocated overflow dc

	hDC = ::CreateCompatibleDC(NULL);

	return hDC;
}

///////////////////////////////////////////////////////////////////////////////
// Return specified dc to the cache deleting overflow dcs

void CDCCache::ReleaseDC(HDC hDC)
{
	// Scan cached dcs table looking for the available slot and cache the callers dc there

	for (UINT n = 0; n < numbof(m_ahDCs); n++) {
		HDC hOldDC = static_cast<HDC>(InterlockedExchangePointer(reinterpret_cast<void**>(&m_ahDCs[n]), hDC));
		if (hOldDC == NULL) {
			return;
		} else
			hDC = hOldDC;
	}

	// Delete overflow dc

	if (hDC != NULL) {
		::DeleteDC(hDC);
	}
}

///////////////////////////////////////////////////////////////////////////////
// End of DCCache.cpp
///////////////////////////////////////////////////////////////////////////////
