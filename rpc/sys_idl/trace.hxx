// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
//
//
// HISTORY
// $Log: trace.hxx,v $
// Revision 1.1.3.2  1996/03/09  20:45:32  marty
// 	Add OSF copyright
// 	[1996/03/09  20:25:00  marty]
//
// Revision 1.1.3.1  1995/12/08  00:23:48  root
// 	Submit OSF/DCE 1.2.1
// 	[1995/12/08  00:07:17  root]
// 
// $EndLog$
//
//  Copyright (c) Digital Equipment Corporation, 1995
//  All Rights Reserved.  Unpublished rights reserved
//  under the copyright laws of the United States.
//  
//  The software contained on this media is proprietary
//  to and embodies the confidential technology of 
//  Digital Equipment Corporation.  Possession, use,
//  duplication or dissemination of the software and
//  media is authorized only pursuant to a valid written
//  license from Digital Equipment Corporation.
//
//  RESTRICTED RIGHTS LEGEND   Use, duplication, or 
//  disclosure by the U.S. Government is subject to
//  restrictions as set forth in Subparagraph (c)(1)(ii)
//  of DFARS 252.227-7013, or in FAR 52.227-19, as
//  applicable.
//

#ifndef trace_included
#define trace_included

#ifdef TRACE

#include <iostream.h>
#include <string.h>

class Trace {
private:
	char *func_name;
	unsigned line_number;
	char *file_name;
public:
	static int On;
	static int Level;
	Trace(char *name, char *file, unsigned line) {
	    if (On) {
		file_name = new char [strlen(name) + 1];
		strcpy(file_name, name);
		for (int i = 0; i < Level; i++) cerr << "    ";
		cerr << "Entered: " << name << " <file: " << file
			<< ", line: " << line << '>' << endl;
		Level++;
	    }
	};
	~Trace() {
	    if (On) {
		for (int i = 1; i < Level; i++) cerr << "    ";
		cerr << "Exiting: " << file_name << endl;
		Level--;
		delete file_name;
	    }
	};
};
		
#define Trace(f) Trace t((f), __FILE__,  __LINE__)

#else
#define Trace(s)
#endif

#endif
