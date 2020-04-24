/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: textimpl.h,v $
 * Revision 1.1.2.2  1996/03/09  20:49:28  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:27:51  marty]
 *
 * Revision 1.1.2.1  1995/12/11  19:35:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:18:29  root]
 * 
 * $EndLog$
 */
#ifndef textimpl_included
#define textimpl_included

#ifdef _WIN32
#include <afx.h>        // NT CString Class
#define String CString
#else
#include <String.h>   // OSF String Class
#endif

#include <stdio.h>
#include "text_mgr.h"

// This class implements the Text interface with some simple operations
// to demonstrate how C++ native objects can be passed as parameters

class TextImpl : public virtual TextMgr {

private:
	String  *s;			// TextImpl is really a String object

public:
	// null constructor
	TextImpl() : s(0) {};

	// String constructor
	TextImpl(String t) { s = new String(t); };

	// return the String object
	String IDL_STD_STDCALL getText() { return *s; };

	// return an upper case version of the String object
	String IDL_STD_STDCALL upper()
        {
#ifdef _WIN32
            s->MakeUpper();
            return *s;
#elif __GNUG__
            s->upcase();
	    return *s;
#else
            return s->upper();
#endif
        };

	// return a lower case version of the String object
	String IDL_STD_STDCALL lower() {
#ifdef _WIN32
            s->MakeLower();
            return *s;
#elif __GNUG__
	    s->downcase();
	    return *s;
#else
            return s->lower();
#endif
};

	// concatenate a String object onto the String
	void IDL_STD_STDCALL concat(String t) { *s += t; };
};

#endif
