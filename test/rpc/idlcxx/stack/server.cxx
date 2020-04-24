// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// 
// 
// HISTORY
// $Log: server.cxx,v $
// Revision 1.1.2.2  1996/03/09  20:50:00  marty
// 	Add OSF copyright
// 	[1996/03/09  20:28:15  marty]
//
// Revision 1.1.2.1  1995/12/11  19:35:57  root
// 	Submit OSF/DCE 1.2.1
// 	[1995/12/11  19:18:51  root]
// 
// $EndLog$
#include "test_com.h"

#include <iostream.h>
#include "server.hxx"
#include "icalculate.h"

int
main(int argc, char *argv[])
{
	char	*entryName;
	unsigned32 st;

    TRY
	// create a server
	Server s(ICalculate_v0_0_s_ifspec);
	entryName = Server::buildEntryName("stack");   // build server's name
	s.exportName(entryName);
	delete [] entryName;

	// go into listening loop
	cout << "server is listening" << endl;
	Server::listen();
    FINALLY
	     cout << "server exiting" << endl;
    ENDTRY
    return(0);
}
