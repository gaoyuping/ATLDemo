///////////////////////////////////////////////////////////////////////////////
//
// DCCache.h: Interface for CDCCache and related classes
//
// Copyright 2009 Evernote Corp. All rights reserved.
// 
// History: 11/25/2009 kvitekp - Initially created.
//
// $Id$
//
///////////////////////////////////////////////////////////////////////////////

#ifndef AFX_DCCACHE_H_INCLUDED
#define AFX_DCCACHE_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Include additional headers
#include <atlbase.h>
#include <atlapp.h>
#include <atlgdi.h>

///////////////////////////////////////////////////////////////////////////////
// CDCCache class
///////////////////////////////////////////////////////////////////////////////

class CDCCache
{
public:

	// Attributes

	HDC m_ahDCs[4];

	// Construction/destruction

	CDCCache();

	~CDCCache();

	// Operations

	HDC GetDC();

	void ReleaseDC(HDC hDC);

};

///////////////////////////////////////////////////////////////////////////////
// CCachedDC class
///////////////////////////////////////////////////////////////////////////////

class CCachedDC : public CDCHandle
{
	// Attributes

	static CDCCache _dcCache;

public:

	// Construction/destruction

	CCachedDC()
	{
		Attach(_dcCache.GetDC());
	}

	~CCachedDC()
	{
		_dcCache.ReleaseDC(Detach());
	}

	// Operations

	operator HDC() const { return m_hDC; }

};

#endif // #ifndef AFX_DCCACHE_H_INCLUDED

///////////////////////////////////////////////////////////////////////////////
// End of DCCache.h
///////////////////////////////////////////////////////////////////////////////