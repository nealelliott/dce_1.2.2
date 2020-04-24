// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// 
// 
// HISTORY
// $Log: util.cxx,v $
// Revision 1.1.2.2  1996/03/09  20:48:39  marty
// 	Add OSF copyright
// 	[1996/03/09  20:27:15  marty]
//
// Revision 1.1.2.1  1995/12/11  19:34:02  root
// 	Submit OSF/DCE 1.2.1
// 	[1995/12/11  19:17:55  root]
// 
// $EndLog$
#ifdef _WIN32
#include "afx.h"  // MFC must appear before windows.h
#endif

#include "test_com.h"
#include "card.h"

// These are the utility routines necessitated by the use
// of the [represent_as] attribute on a net_string in the ACF.


// convert a local String object to a net_string
// so that it can be passed across the wire.
void
net_string_from_local(
	String		*s,		// pointer to String object 
	net_string 	**x		// data that gets marshalled
) {
#ifdef _WIN32
	**x = new char[s->GetLength() + 1];
	strcpy(**x, *s);
#else
	**x = new char[s->length() + 1];
	strcpy(**x, (char *) *s);
#endif
}

// convert a marshalled net_string to a local String object
void
net_string_to_local(
	net_string	*x,		// data that was marshalled
	String		*s		// what the user sees
) {
	*s = *x;			// String assignment op is overloaded
}

// free memory used to hold the String object
void
net_string_free_local(String*x)
{
	//delete x;
}


// free the memory used to hold the marshalled data
void
net_string_free_inst(net_string *x)
{
	delete [] *x;
}
