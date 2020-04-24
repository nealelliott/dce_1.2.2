// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// 
// 
// HISTORY
// $Log: server.cxx,v $
// Revision 1.1.2.2  1996/03/09  20:49:25  marty
// 	Add OSF copyright
// 	[1996/03/09  20:27:49  marty]
//
// Revision 1.1.2.1  1995/12/11  19:35:03  root
// 	Submit OSF/DCE 1.2.1
// 	[1995/12/11  19:18:27  root]
// 
// $EndLog$
#ifdef _WIN32
#include "afx.h"  // MFC must appear before windows.h
#endif

#include "test_com.h"

#include <iostream.h>
#include "server.hxx"

#include "textimpl.h"

int
main(int argc, char *argv[])
{
    char	*entryName;
    char	cmd[1000];
    void 	registerAnObject(char *);


    TRY
	// create a server
	Server s(Text_v0_0_s_ifspec);
	entryName = Server::buildEntryName("native");   // build server's name
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
