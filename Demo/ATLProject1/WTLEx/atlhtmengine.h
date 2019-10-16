//
// Windows Template Library Extension for
// Terra Informatica Lightweight Embeddable HTMEngine control
// http://terra-informatica.org/HTMEngine
//
// Written by Andrew Fedoniouk / <andrew@TerraInformatica.com>
// Portions: Pete Kvitek <pete@kvite.com>
//
// This file is NOT part of Windows Template Library.
// The code and information is provided "as-is" without
// warranty of any kind, either expressed or implied.
//
// (C) 2003, Andrew Fedoniouk <andrew@TerraInformatica.com>
//

#ifndef __ATLHTMENGINE_H__
#define __ATLHTMENGINE_H__

#pragma once

#ifndef __cplusplus
	#error ATL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLAPP_H__
	#error atlHTMEngine.h requires atlapp.h to be included first
#endif

#ifndef __ATLWIN_H__
	#error atlHTMEngine.h requires atlwin.h to be included first
#endif

#include <atlmisc.h>

#include "HTMEngine.h"
#include "atlvalue.h"

/////////////////////////////////////////////////////////////////////////////
// Classes in this file
//
// CHtmEngineCtrlT<TBase>
// CHtmEngineEditorCtrlT<TBase>
//

namespace WTL
{

/////////////////////////////////////////////////////////////////////////////
// CHtmEngineCtrl - client side for a HTMEngine control

template <class TBase>
class CHtmEngineCtrlT : public TBase
{
public:

	// Constructors

	CHtmEngineCtrlT(HWND hWnd = NULL) : TBase(hWnd) { }

	CHtmEngineCtrlT< TBase >& operator=(HWND hWnd)
	{
		m_hWnd = hWnd;
		return *this;
	}

	HWND Create(HWND hWndParent, _U_RECT rect = NULL,
      LPCTSTR szWindowName = NULL,
			DWORD dwStyle = 0, DWORD dwExStyle = 0,
			_U_MENUorID MenuOrID = 0U, LPVOID lpCreateParam = NULL)
	{
		CWindow::Create(GetWndClassName(), hWndParent, rect.m_lpRect,
			szWindowName, dwStyle | WS_CHILD, dwExStyle, MenuOrID.m_hMenu, lpCreateParam);

		return m_hWnd;
	}

	// Attributes

	static LPCTSTR GetWndClassName()
	{
		return ::HTMEngineClassNameT();
	}

	bool LoadHtml(LPCBYTE pb, DWORD nBytes)
	{
		ATLASSERT(::IsWindow(m_hWnd));
    return HTMEngineProc(WM_HE_LOAD_HTML,WPARAM(nBytes),LPARAM(pb)) != 0;
	}

  void SetCallback(HTMENGINE_CALLBACK* pCallback)
  {
    HTMEngineProc(WM_HE_CALLBACK,0x80000000,LPARAM(pCallback));
  }


	bool LoadHtmlResource(LPCTSTR pszName, HMODULE hModule = NULL)
	{
		// This code assumes that the host and control windows are the same

		ATLASSERT(::IsWindow(m_hWnd));

    LPBYTE pb;
    DWORD  cb;

		if(!GetHtmlResource(pszName, pb, cb, hModule))
      return false;

    return LoadHtml(pb, cb);
	}

	bool LoadHtmlResource(DWORD resID, HMODULE hModule = NULL)
  {
    return LoadHtmlResource(MAKEINTRESOURCE(resID), hModule);
  }

	bool OpenFile(LPCTSTR lpszFilePath)
	{
		ATLASSERT(::IsWindow(m_hWnd));
    return HTMEngineProc(WM_HE_LOAD_FILE,0,LPARAM(lpszFilePath)) != 0;
	}

  int     GetDocumentMinWidth()           { return HTMEngineProc(WM_HE_GET_MIN_WIDTH,0,0); }
  int     GetDocumentMinHeight(int width)	{ return HTMEngineProc(WM_HE_GET_MIN_HEIGHT,0,LPARAM(width)); }


  // this function will return width of the HTMEngine window needed to
  // show document in full width - which means without horizontal scrollbar
  int     GetWindowWidth()
  {
    CRect rc(0,0,0,0);
    AdjustWindowRectEx(&rc,GetWindowLong(GWL_STYLE),FALSE,GetWindowLong(GWL_EXSTYLE));
    return GetDocumentMinWidth() + rc.Width();
  }

  // this function will return height of HTMEngine window for proposed width
  // needed to show the document in full thus without vertical scrollbar.
  int     GetWindowHeight(int windowWidth)
  {
    CRect rc(0,0,0,0);
    AdjustWindowRectEx(&rc,GetWindowLong(GWL_STYLE),FALSE,GetWindowLong(GWL_EXSTYLE));
    int clientWidth = windowWidth - rc.Width();
    int h = GetDocumentMinHeight(clientWidth);
    h += rc.Height(); // add borders extra
    if(clientWidth < GetDocumentMinWidth()) // horz scrollbar will appear, add its height
      h += GetSystemMetrics(SM_CYHSCROLL);
    return h;
  }



	// Methods

	bool CALLBACK HTMEngineGetImageByPoint(int x, int y, const char** pstr, RECT* rcOut)
	{
		return !!::HTMEngineGetImageByPoint(m_hWnd, x, y, pstr, rcOut);
	}

	bool CALLBACK HTMEngineChangeHBITMAP(LPCWSTR uri, HBITMAP hBmp)
	{
		return !!::HTMEngineChangeHBITMAP(m_hWnd, uri, hBmp);
	}

	bool CALLBACK HTMEngineHBITMAPReady(LPCWSTR uri, HBITMAP hBmp, BOOL managed)
	{
		return !!::HTMEngineHBITMAPReady(m_hWnd, uri, hBmp, managed);
	}

	bool CALLBACK HTMEngineDataReady(LPCWSTR uri, LPCBYTE data, DWORD dataLength)
	{
		return !!::HTMEngineDataReady(m_hWnd, uri, data, dataLength);
	}

	LRESULT CALLBACK HTMEngineProc(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return ::HTMEngineProc(m_hWnd, msg, wParam, lParam);
	}

#ifndef RT_HTML
#define RT_HTML         MAKEINTRESOURCE(23)
#endif

	static bool GetHtmlResource(LPCTSTR pszName, /*out*/PBYTE& pb, /*out*/DWORD& cb, HMODULE hModule = NULL)
	{
		ATLASSERT(pszName != NULL);

		// Find specified resource and check if ok

    if(!hModule)
      hModule = _Module.GetResourceInstance();

//#ifndef _WIN32_WCE
		HRSRC hrsrc = ::FindResource(hModule, pszName, MAKEINTRESOURCE(RT_HTML));
//#else
//    HRSRC hrsrc = ::FindResource(hModule, pszName, TEXT("HTM"));
//#endif
		if(!hrsrc)
      return false;

		// Load specified resource and check if ok

		HGLOBAL hgres = ::LoadResource(hModule, hrsrc);
		if(!hgres) return false;

		// Retrieve resource data and check if ok

		pb = (PBYTE)::LockResource(hgres); if (!pb) return false;
		cb = ::SizeofResource(hModule, hrsrc); if (!cb) return false;

		return true;
	}

  // Find HWND of child ctl by its name.
  // nameTemplate could be literal name or template like "margin*" - to get
  // control which name looks like "margin..."
  // If there are multiple ctls which names are matching given criteria
  // you may use parameter index to select particular ctl in the name group.
  // To find max value of the index use GetDlgItemNameCount function.
  HWND    GetDlgItemByName(LPCTSTR nameTemplate, int index = 0) { return HTMEngineGetDlgItemByNameT(m_hWnd,nameTemplate,index);  }
  // Get number of ctls which names are matching nameTemplate criteria
  int     GetDlgItemNameCount(LPCTSTR nameTemplate) { return HTMEngineGetDlgNameItemsCountT(m_hWnd,nameTemplate); }
  // Get HTML name of ctl by its HWND
  LPCTSTR GetDlgItemName(HWND hwnd) { return HTMEngineGetDlgItemNameT(m_hWnd,hwnd);  }

  // get/set control value
  CValue  GetDlgItemValue(HWND hwndControl);
  void    SetDlgItemValue(HWND hwndControl, CValue v);

  CValue  GetDlgItemValue(LPCTSTR name);
  void    SetDlgItemValue(LPCTSTR name, CValue v);

  void    GetDlgItemValues(CSimpleMap<CString,CValue>& bag);
  void    SetDlgItemValues(const CSimpleMap<CString,CValue>& bag);

  // get list of control attributes defined in HTML
  // e.g. for <INPUT type=text id=28 width=10> it will return
  // collection from three attributes(type,id,width) and their values
  void    GetDlgItemAttributes(HWND hwndControl, CSimpleMap<CString,CValue>& attributes)
  {
    struct _callback: public HTMENGINE_ENUM_PROPERTIES_CALLBACK
    {
      CSimpleMap<CString,CValue>* pCollection;
      virtual BOOL __stdcall ObjectProperty(LPCSTR lpszPropName, LPCWSTR lpwszPropValue)
      {
        pCollection->Add(CString(lpszPropName),CValue(lpwszPropValue));
        return TRUE;
      }
    } callback;
    attributes.RemoveAll();
    callback.pCollection = &attributes;
    HTMEngineProc(WM_HE_ENUM_CONTROL_ATTRIBUTES, WPARAM(hwndControl), LPARAM(&callback));
  }

  // get control attribute value
  CValue  GetDlgItemAttribute(HWND hwndControl, LPCTSTR attributeName)
  {

    struct _callback: public HTMENGINE_ENUM_PROPERTIES_CALLBACK
    {
      LPCSTR  attributeName;
      CValue  attributeValue;
      virtual BOOL __stdcall ObjectProperty(LPCSTR lpszPropName, LPCWSTR lpwszPropValue)
      {
        if(stricmp(attributeName,lpszPropName) == 0)
        {
          attributeValue = lpwszPropValue;
          return FALSE;
        }
        return TRUE;
      }
    } callback;

    USES_CONVERSION;
    callback.attributeName = T2CA(attributeName);
    HTMEngineProc(WM_HE_ENUM_CONTROL_ATTRIBUTES, WPARAM(hwndControl), LPARAM(&callback));
    return callback.attributeValue;
  }


  void ScrollToView(LPRECT pRect)
  {
    HTMEngineProc(WM_HE_SCROLL_TO_VIEW, 0, LPARAM(pRect));
  }


  // get list of <META> attributes of HTML document
  void    GetMetaInfo(CSimpleMap<CString,CString>& metas)
  {
    struct _callback: public HTMENGINE_ENUM_PROPERTIES_CALLBACK
    {
      CSimpleMap<CString,CString>* pCollection;
      virtual BOOL __stdcall ObjectProperty(LPCSTR lpszPropName, LPCWSTR lpwszPropValue)
      {
        pCollection->Add(CString(lpszPropName),CString(lpwszPropValue));
        return TRUE;
      }
    } callback;
    metas.RemoveAll();
    callback.pCollection = &metas;
    HTMEngineProc(WM_HE_ENUM_META_INFO, 0, LPARAM(&callback));
  }

  void HighlightText(HTMENGINE_HIGHLIGHT_TEXT_CB *pcbf)
  {
    HTMEngineProc(WM_HE_TEXT_HIGHLIGHT, 0, LPARAM(pcbf));
  }

  void HighlightImage(LPBYTE pImgSrcUri, LPRECT pRECT)
  {
    HTMEngineProc(WM_HE_IMGBOX_HIGHLIGHT, WPARAM(pImgSrcUri), LPARAM(pRECT));
  }

  BOOL SelectHighlight( BOOL first, BOOL backward )
  {
    return HTMEngineProc(WM_HE_HIGHLIGHTED_TEXT_SELECT, first, backward);
  }

  void ProcessKey(UINT vkey)
  {
	  HTMEngineProc(WM_HE_KEYDOWN, vkey, 0);
  }

};

typedef CWinTraits<WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, WS_EX_CLIENTEDGE> CHtmEngineWinTraits;
typedef CHtmEngineCtrlT<CWindow> CHtmEngineCtrl;


//
// Implementation of HtmlEngine Editor methods
//
template <class TBase>
class CHtmEngineEditorCtrlT : public CHtmEngineCtrlT<TBase>
{
public:

  CHtmEngineEditorCtrlT(HWND hWnd = NULL) : CHtmEngineCtrlT<TBase>(hWnd)
  #if defined(WIN32_PLATFORM_WFSP)
    ,m_inputMode(IM_SPELL)
  #endif
  { }

  CHtmEngineEditorCtrlT< TBase >& operator=(HWND hWnd)
	{
		m_hWnd = hWnd;
		return *this;
	}

  //|
  //| Calculate size of the buffer to save. Number of bytes
  //|
	DWORD SaveHtmlSize()
	{
		ATLASSERT(::IsWindow(m_hWnd));
    return (DWORD)::SendMessage(m_hWnd,WM_HE_SAVE_HTML,0,0);
	}

  //|
  //| Save current HTML doc to the buffer. Use SaveHtmlSize to get size needed for saving.
  //|
	DWORD SaveHtml(LPBYTE pb, DWORD nBytes)
	{
		ATLASSERT(::IsWindow(m_hWnd));
    return (DWORD)::SendMessage(m_hWnd,WM_HE_SAVE_HTML,WPARAM(nBytes),LPARAM(pb));
	}

  //|
  //| save current HTML doc to the file.
  //|
	bool SaveFile(LPCTSTR lpszFilePath)
	{
		ATLASSERT(::IsWindow(m_hWnd));
    return ::SendMessage(m_hWnd,WM_HE_SAVE_FILE,0,LPARAM(lpszFilePath)) == 0;
	}


  //|
  //| Calculate size of the buffer to save selection. Number of bytes
  //| Returns zero if there is no selection
  //|
	DWORD GetSelectionSize()
	{
		ATLASSERT(::IsWindow(m_hWnd));
    return (DWORD)::SendMessage(m_hWnd,WM_HE_GET_SELECTION,0,0);
	}

  //|
  //| Copy HTML in current selection to the buffer.
  //| Use GetSelectionSize to get size of buffer needed for saving.
  //|
	DWORD GetSelection(LPBYTE pb, DWORD nBytes)
	{
		ATLASSERT(::IsWindow(m_hWnd));
    return (DWORD)::SendMessage(m_hWnd,WM_HE_GET_SELECTION,WPARAM(nBytes),LPARAM(pb));
	}

  //|
  //| Test if point is in current selection.
  //|
	bool IsPointInSelection(LPARAM lParam)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (bool)!!::SendMessage(m_hWnd, WM_HE_TEST_SELECTION, WPARAM(0), lParam);
	}

	bool IsPointInSelection(CPoint pt)
	{
		IsPointInSelection(MAKELPARAM(pt.x, pt.y));
	}

	bool OpenSpellCheckSession()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (bool)!!::SendMessage(m_hWnd, WM_HE_SPELL_CHECK, WPARAM(0), LPARAM(1));
	}

	bool CheckSpelling()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (bool)!!::SendMessage(m_hWnd, WM_HE_SPELL_CHECK, WPARAM(0), LPARAM(0));
	}

	bool ReplaceCurrentSelection(LPCTSTR pszNew)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (bool)!!::SendMessage(m_hWnd, WM_HE_REPLACE, WPARAM(0), LPARAM(pszNew));
	}

	bool CloseSpellCheckSession()
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return (bool)!!::SendMessage(m_hWnd, WM_HE_SPELL_CHECK, WPARAM(1), LPARAM(1));
	}

private:
  // auxiliary getter helpers
    bool get_bool(int msg, bool init_value = false)
    {
		  ATLASSERT(::IsWindow(m_hWnd));
      return ::SendMessage(m_hWnd, msg, 0, 0) != 0;
    }
    int get_int(int msg, int value = 0)
    {
		  ATLASSERT(::IsWindow(m_hWnd));
      return ::SendMessage(m_hWnd, msg, 0, value);
    }
    bool get_str(int msg, LPTSTR str)
    {
		  ATLASSERT(::IsWindow(m_hWnd));
      return ::SendMessage(m_hWnd, msg, 0, LPARAM(str)) != 0;
    }
    // auxiliary setter helpers
    void set_bool(int msg, bool value)
    {
		  ATLASSERT(::IsWindow(m_hWnd));
      ::SendMessage(m_hWnd, msg, 0x80000000, LPARAM(value));
    }
    void set_int(int msg, int value)
    {
		  ATLASSERT(::IsWindow(m_hWnd));
      ::SendMessage(m_hWnd, msg, 0x80000000, LPARAM(value));
    }
    void set_str(int msg, LPCTSTR value)
    {
		  ATLASSERT(::IsWindow(m_hWnd));
      ::SendMessage(m_hWnd, msg, 0x80000000, LPARAM(value));
    }
    // send command without parameters
    void send(int msg, LPARAM lParam = 0)
    {
		  ATLASSERT(::IsWindow(m_hWnd));
      ::SendMessage(m_hWnd, msg, 0x80000000, lParam);
    }


public:
  // getters
  bool    CanCut()                        { return get_bool(WM_HE_CUT); }
  bool    CanCopy()                       { return get_bool(WM_HE_COPY); }
  bool    CanUndo()                       { return get_bool(WM_HE_UNDO); }
  bool    CanRedo()                       { return get_bool(WM_HE_REDO); }
  bool    CanPaste()                      { return get_bool(WM_HE_PASTE); }

  // returns TRUE if document was modified by user since last SetModified(false);
  bool    GetModified()                   { return get_bool(WM_HE_IS_MODIFIED); }
  // send 'modified' flag. Setting this flag does not change undo/redo stack.
  void    SetModified(bool on)            { set_bool(WM_HE_IS_MODIFIED,on); }

  bool    GetFontBold()                   { return get_bool(WM_HE_FONT_BOLD); }
  bool    GetFontItalic()                 { return get_bool(WM_HE_FONT_ITALIC); }
  bool    GetFontUnderline()              { return get_bool(WM_HE_FONT_UNDERLINE); }
  bool    GetFontSubscript()              { return get_bool(WM_HE_FONT_SUBSCRIPT); }
  bool    GetFontSuperscript()            { return get_bool(WM_HE_FONT_SUPERSCRIPT); }
  bool    GetFontStrikethrough()          { return get_bool(WM_HE_FONT_STRIKETHROUGH); }
  int     GetFontSize()                   { return get_int(WM_HE_FONT_SIZE); }
  COLORREF GetFontColor()                 { return (COLORREF)get_int(WM_HE_FONT_COLOR); }
  bool    GetFontName(LPTSTR buf)         { return get_str(WM_HE_FONT_NAME,buf); }
          // size of the buffer here shall be not less then LF_FACESIZE



  bool    GetParaAlignLeft()              { return get_bool(WM_HE_PARA_ALIGN_LEFT); }
  bool    GetParaAlignCenter()            { return get_bool(WM_HE_PARA_ALIGN_CENTER); }
  bool    GetParaAlignRight()             { return get_bool(WM_HE_PARA_ALIGN_RIGHT); }
  bool    GetParaAlignJustify()           { return get_bool(WM_HE_PARA_ALIGN_JUSTIFY); }

  bool    GetParaIndentMinus()            { return get_bool(WM_HE_PARA_INDENT_MINUS); }
  bool    GetParaIndentPlus()             { return get_bool(WM_HE_PARA_INDENT_PLUS); }
  bool    GetParaOrderedList()            { return get_bool(WM_HE_PARA_ORDERED_LIST); }
  bool    GetParaUnorderedList()          { return get_bool(WM_HE_PARA_UNORDERED_LIST); }

  int     GetParaType()                   { return get_int(WM_HE_PARA_TYPE); }

  int     GetEditMode()                   { return get_int(WM_HE_EDIT_MODE); }
  bool    GetBottomlessMode()             { return get_bool(WM_HE_BOTTOMLESS_MODE); }

  bool    IsWholeDocSelected()            { return get_bool(WM_HE_SELECT_ALL); }
  bool    IsCaretInLink()                 { return get_bool(WM_HE_IS_CARET_IN_LINK); }

  // setters
  void    SetEditMode(int mode)           { set_int(WM_HE_EDIT_MODE,mode); }
  void    SetBottomlessMode(bool on)      { set_bool(WM_HE_BOTTOMLESS_MODE,on); }

  void    SetFontSize(int size1to7)       { set_int(WM_HE_FONT_SIZE,size1to7); }
  void    SetFontName(LPCTSTR name)       { set_str(WM_HE_FONT_NAME,name); }

  // dec/inc font size in selection by 1
  void    FontSizeIncrease()             { set_int(WM_HE_FONT_SIZE,-1); }
  void    FontSizeDecrease()             { set_int(WM_HE_FONT_SIZE,-2); }

  // togglers (commands)
  void    ToggleFontBold()                { send(WM_HE_FONT_BOLD); }
  void    ToggleFontItalic()              { send(WM_HE_FONT_ITALIC); }
  void    ToggleFontUnderline()           { send(WM_HE_FONT_UNDERLINE); }
  void    ToggleFontSubscript()           { send(WM_HE_FONT_SUBSCRIPT); }
  void    ToggleFontSuperscript()         { send(WM_HE_FONT_SUPERSCRIPT); }
  void    ToggleFontStrikethrough()       { send(WM_HE_FONT_STRIKETHROUGH); }

  void    ToggleParaAlignLeft()           { send(WM_HE_PARA_ALIGN_LEFT); }
  void    ToggleParaAlignCenter()         { send(WM_HE_PARA_ALIGN_CENTER); }
  void    ToggleParaAlignRight()          { send(WM_HE_PARA_ALIGN_RIGHT); }
  void    ToggleParaAlignJustify()        { send(WM_HE_PARA_ALIGN_JUSTIFY); }

  void    ToggleParaIndentMinus()         { send(WM_HE_PARA_INDENT_MINUS); }
  void    ToggleParaIndentPlus()          { send(WM_HE_PARA_INDENT_PLUS); }
  void    ToggleParaOrderedList()         { send(WM_HE_PARA_ORDERED_LIST); }
  void    ToggleParaUnorderedList()       { send(WM_HE_PARA_UNORDERED_LIST); }

  void    SetParaType(int type)           { set_int(WM_HE_PARA_TYPE,type); }
  void    SetFontColor(COLORREF clr = 0xFF)  { set_int(WM_HE_FONT_COLOR, (int) clr); }

  void    Undo()                          { send(WM_HE_UNDO); }
  void    Redo()                          { send(WM_HE_REDO); }
  void    Copy()                          { send(WM_HE_COPY); }
  void    Cut()                           { send(WM_HE_CUT); }
  void    Clear()                         { send(WM_HE_CLEAR); }
  void    Paste()                         { send(WM_HE_PASTE); }

  void    PastePlainText()                { send(WM_HE_PASTE_PLAIN_TEXT_ONLY); }
  //paste HTML but ignore tables
  void    PasteNoTables()                 { send(WM_HE_PASTE_HTML_WITHOUT_TABLES); }

  void    SelectAll()                     { send(WM_HE_SELECT_ALL); }
  void    ClearAll()                      { send(WM_HE_CLEAR_ALL); }
  void    SetCaretPos(int pos)			  { send(WM_HE_SETCARETPOS, pos); }

  bool    InsertHTML(LPCBYTE utf8encodedHTML, DWORD cb = 0)
  {
    ATLASSERT(::IsWindow(m_hWnd));
    if(utf8encodedHTML == 0)
      return false;
    if(cb == 0)
      cb = strlen((const char *)utf8encodedHTML);
    return SendMessage(WM_HE_INSERT_HTML, WPARAM(cb), LPARAM(utf8encodedHTML)) != 0;
  }

  // clears undo stack without actual undoing.
  void    ClearUndoHistory()              { send(WM_HE_CLEAR_UNDO_STACK); }

  bool    GetShowSelection()              { return get_bool(WM_HE_SHOW_SELECTION); }
  void    SetShowSelection(bool show)     { set_bool(WM_HE_SHOW_SELECTION,show); }


  struct _enum_obj_callback: public HTMENGINE_ENUM_OBJECTS_CALLBACK
  {
      CSimpleArray<CString>& collection;

	  _enum_obj_callback(CSimpleArray<CString>& _collection) : collection(_collection)  {}

      virtual BOOL __stdcall CurrentObject(LPCSTR lpszObjectTypeName)
      { CString str = lpszObjectTypeName;
        collection.Add(str);
        return TRUE;
      }
  };

  // get list of current object names
  void    GetCurrentObjects(CSimpleArray<CString>& objects)
  {
    _enum_obj_callback callback(objects);

    objects.RemoveAll();
    SendMessage(WM_HE_ENUM_CURRENT_OBJECTS, 0, LPARAM(&callback));
  }

  struct _enum_prop_callback: public HTMENGINE_ENUM_PROPERTIES_CALLBACK
  {
    CSimpleMap<CString,CString>& collection;

	_enum_prop_callback(CSimpleMap<CString,CString>& _collection) : collection(_collection)  {}

    virtual BOOL __stdcall ObjectProperty(LPCSTR lpszPropName, LPCWSTR lpwszPropValue)
    {
      collection.Add(CString(lpszPropName),CString(lpwszPropValue));
      return TRUE;
    }
  };

  // get list of current object properties
  void    GetCurrentObjectProperties(LPCTSTR objectName, CSimpleMap<CString,CString>& props)
  {
	_enum_prop_callback callback(props);

    props.RemoveAll();
#ifdef UNICODE
    USES_CONVERSION;
    SendMessage(WM_HE_ENUM_CURRENT_OBJECT_PROPERTIES, WPARAM(W2CA(objectName)), LPARAM(&callback));
#else
    SendMessage(WM_HE_ENUM_CURRENT_OBJECT_PROPERTIES, WPARAM(objectName), LPARAM(&callback));
#endif
  }

  struct _set_prop_callback: public HTMENGINE_SET_PROPERTIES_CALLBACK
  {
      CSimpleMap<CString,CString>& collection;

	  _set_prop_callback(CSimpleMap<CString,CString>& _collection) : collection(_collection) {}

      virtual LPCSTR __stdcall GetNthPropertyName(UINT n)
      {
        if(int(n) >= collection.GetSize())
          return 0;
        CString tname = collection.GetKeyAt(n);
#ifdef UNICODE
        USES_CONVERSION;
        static char name[64]; strncpy(name,W2CA(LPCWSTR(tname)),64);
        return name;
#else
        return tname;
#endif
      }
      virtual LPCWSTR __stdcall GetNthPropertyValue(UINT n)
      {
        if(int(n) >= collection.GetSize())
          return 0;
        CString tvalue = collection.GetValueAt(n);
#ifdef UNICODE
        return tvalue;
#else
        USES_CONVERSION;
        static wchar_t value[512]; wcsncpy(value,A2CW(LPCSTR(tvalue)),512);
        return value;
#endif
      }
  };

  // set current object properties from the list
  void SetCurrentObjectProperties(LPCTSTR objectName, CSimpleMap<CString,CString>& props)
  {
    _set_prop_callback callback(props);

#ifdef UNICODE
    USES_CONVERSION;
    SendMessage(WM_HE_SET_CURRENT_OBJECT_PROPERTIES, WPARAM(W2CA(objectName)), LPARAM(&callback));
#else
    SendMessage(WM_HE_SET_CURRENT_OBJECT_PROPERTIES, WPARAM(objectName), LPARAM(&callback));
#endif
  }

  DWORD GetPlainText(LPCBYTE utf8buffer, DWORD cbBuffer)
  {
    ATLASSERT(::IsWindow(m_hWnd));
    return SendMessage(WM_HE_SAVE_PLAIN_TEXT, WPARAM(cbBuffer), LPARAM(utf8buffer));
  }


#if defined(WIN32_PLATFORM_WFSP) | defined(WIN32_PLATFORM_PSPC)
  DWORD m_inputMode;
  DWORD GetInputMode() { return m_inputMode; }
  DWORD GetDefaultInputMode() { return IM_SPELL; }
  void  SetInputMode(DWORD im = IM_SPELL);

  void  CharComposition(WCHAR wc)
  {
    SendMessage(WM_HE_CHAR_COMPOSITION, WPARAM(wc), 0);
  }

#endif

};

typedef CHtmEngineEditorCtrlT<CWindow>    CHtmEngineEditorCtrl;

} //namespace WTL

#include "atlhtmenginevalues.h"


// Link against HTMEngine library
#ifdef _WIN32_WCE
#pragma comment(lib, "HTMEngineCE.lib")
#else
#pragma comment(lib, "HTMEngine.lib")
#endif

#endif // __ATLHTMEngine_H__
