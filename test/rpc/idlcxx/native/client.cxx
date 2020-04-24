// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// 
// 
// HISTORY
// $Log: client.cxx,v $
// Revision 1.1.4.2  1996/09/10  18:21:09  bfc
// 	remove unecessary STDC definition
// 	[1996/09/10  18:16:12  bfc]
//
// Revision 1.1.4.1  1996/09/09  20:59:14  bfc
// 	finish TETizing and cleanup
// 	[1996/09/09  20:42:42  bfc]
// 
// Revision 1.1.2.2  1996/03/09  20:49:22  marty
// 	Add OSF copyright
// 	[1996/03/09  20:27:46  marty]
// 
// Revision 1.1.2.1  1995/12/11  19:34:54  root
// 	Submit OSF/DCE 1.2.1
// 	[1995/12/11  19:18:24  root]
// 
// $EndLog$
// This test demonstrates how to pass native C++ objects as parameters
// in RPC calls. The test deals with String objects, where the String
// class is defined in a C++ class library.
//
// A dynamic remote text object is created on the server using a String
// object to initialize it. Then a couple more String objects are 
// concatenated to the remote object. The server's text is fetched
// and printed to stdout, along with an all upper case and an all lower
// case version of the same text.
//

#ifdef TET
#define TET_ENV
#define main loc_main
#endif

#ifdef _WIN32
#include "afx.h"  // MFC must appear before windows.h
#endif

#include "test_com.h"

#include <iostream.h>

#ifdef _WIN32
#include <afx.h>                        // NT CString Class
#else
#include <String.h>			// OSF String Class
#endif

#include "text.h"			// IDL generated header file

int
main(int argc, char *argv[])
{
	Text	*t;			// text interface pointer

	//
	// Test passing a String object as an RPC argument 
	//
	String	s1("Hello");		// a String object
	String	s2(",");		// a String object
	String	s3("World");		// a String object

	t = Text::newText(s1);		// create remote object and init with s1
	t->concat(s2);			// concatenate s2 onto remote object
	t->concat("");			// concatenate s3 onto remote object
	t->concat(s3);			// concatenate s3 onto remote object

	// next 3 lines contact the server to get the text to print
	cout << "text:  " << t->getText() << endl;	// fetch and print
	cout << "upper: " << t->upper() << endl;	// fetch and print
	cout << "lower: " << t->lower() << endl;	// fetch and print

	delete t;

	return 0;
}
