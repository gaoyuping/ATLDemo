///////////////////////////////////////////////////////////////////////////////
//
// Common.h: Common declarations declarations header
//
// Copyright (c) 2003 EverNote Corporation - ALL RIGHTS RESERVED
//
// Confidential material of EverNote Corp, 221 South Road, Wurtsboro, NY 12790
//
// History: 14/Apr/2003 kvitekp - Picked up from AirBase code base
//
// $Id$
//
///////////////////////////////////////////////////////////////////////////////

#ifndef AFX_COMMON_H_INCLUDED
#define AFX_COMMON_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Provide Windows 8 compatibility

#ifndef WINAPI_FAMILY
#include "winapifamily_stub.h"
#endif

///////////////////////////////////////////////////////////////////////////////
// Define convenience macros
///////////////////////////////////////////////////////////////////////////////

// Endless loop until break

#ifndef loop
#define loop  for (;;)
#endif

// Number of elements in array

#ifndef numbof
#define numbof(a)  (sizeof(a)/sizeof(a[0]))
#endif

// Length of the const string

#ifndef lengof
#define lengof(s)  (numbof(s) - 1)
#endif

// Literal string length specification

#ifndef LITERAL
#define LITERAL(s) s, lengof(s)
#endif

// Structure field offset

#ifndef fieldoffs
#define fieldoffs(type, field)  ((int)&(((type *)0)->field))
#endif
  
// Structure field size

#ifndef fieldsize
#define fieldsize(type, field)  ((int)sizeof(*(&(((type *)1)->field))))
#endif

// Define bit manipulation macros

#ifndef HASFLAG
#define HASFLAG(f,b)  ((f) & (b))
#endif

#ifndef HASFLAGS
#define HASFLAGS(f,b)  (((f) & (b)) == (b))
#endif

#ifndef SETFLAG
#define SETFLAG(f,b)  (f)|= (b)
#endif

#ifndef CLRFLAG
#define CLRFLAG(f,b)  (f)&=~(b)
#endif

#ifndef SETFLAGTO
#define SETFLAGTO(f,b,c)  if (c) SETFLAG(f,b); else CLRFLAG(f,b);
#endif

// Generic boolean values

#ifndef FALSE
#define FALSE	0
#endif

#ifndef TRUE
#define TRUE	1
#endif

// Macros to round numbers
//
// ROUND2 = rounds a number up to a power of 2
// ROUND  = rounds a number up to any other number
//
// n = number to be rounded
// pow2 = must be a power of two value
// r = any number

#define ROUND2(n,pow2)	(((n) + (pow2) - 1) & ~((pow2) - 1))
#define ROUND(n,r)	 (((n)/(r) + (((n)%(r)) ? 1 : 0)) * (r))

// Check for the RFC complaint linear white space

#ifndef	ISLWSP
#define ISLWSP(ch) ((ch) == ' ' || (ch) == '\t')
#endif

// Data size helpers

#define KB (1024)
#define MB (1024 * KB)
#define GB (1024 * MB)

// No color constant

#define NO_COLOR ((COLORREF)-1)

// C++ specific declarations

#ifdef __cplusplus

// Abs value function

template<class T>
T AbsVal(T val)
{
	return val < 0 ? - val : val;
}

// Template to initialize sized structures common in Windows 

template <typename T>
struct InitT : public T
{
	InitT() { ::ZeroMemory(this, sizeof(T)); }
	inline operator const T*() const { return this; }
	inline operator T*() { return this; }
};

template <class T>
struct InitCbT : public InitT<T>
{
	InitCbT() { cb = sizeof(T); }
};

template <class T>
struct InitCbSizeT : public InitT<T>
{
	InitCbSizeT() { cbSize = sizeof(T); }
};

template <class T>
struct InitDwSizeT : public InitT<T>
{
	InitDwSizeT() { dwSize = sizeof(T); }
};

template <class T>
struct InitlengthT : public InitT<T>
{
	InitlengthT() { length = sizeof(T); }
};

///////////////////////////////////////////////////////////////////////////////
// Safe pointer class
///////////////////////////////////////////////////////////////////////////////

template<typename T>
class CSafePtr 
{
public:

	// Attributes
	
	T* m_pT;

	// Construction/destruction

	CSafePtr(T* pT = NULL)
	:	m_pT(pT)
	{
	}

	~CSafePtr()
	{
		delete m_pT;
	}

	// Operations
	
	operator T*() const { return m_pT; }

	T& operator*() const { return *m_pT; }

	T** operator&() { return &m_pT; }

	T* operator->() const { return m_pT; }
	
	T* operator=(T* pT) { if (m_pT && m_pT != pT) delete m_pT; return m_pT = pT; }

	T* Detach() { T* pT = m_pT; m_pT = NULL; return pT; }

private: // prevent copying
      CSafePtr( const CSafePtr& );
      const CSafePtr& operator=( const CSafePtr& );
};

///////////////////////////////////////////////////////////////////////////////
// Safe flag setting class
///////////////////////////////////////////////////////////////////////////////

class CSafeSetFlag
{
public:

	// Attributes
	
	bool& m_flag;
	bool  m_prev;

	// Construction/destruction

	CSafeSetFlag(bool& bFlag, bool bSet = true)
	:	m_flag(bFlag)
	,	m_prev(bFlag)
	{
		m_flag = bSet;
	}

	~CSafeSetFlag()
	{
		m_flag = m_prev;
	}

	bool Prev() const
	{
		return m_prev;
	}

};

///////////////////////////////////////////////////////////////////////////////
// WinRT only declarations
///////////////////////////////////////////////////////////////////////////////

#if !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)

#ifndef REFSTR
#define REFSTR(str) (ref new Platform::String(const_cast<wchar_t*>((const WCHAR*)str)))
#endif

#endif

#endif // #ifdef __cplusplus

#endif // #ifndef AFX_COMMON_H_INCLUDED

///////////////////////////////////////////////////////////////////////////////
// End of Common.h
///////////////////////////////////////////////////////////////////////////////
