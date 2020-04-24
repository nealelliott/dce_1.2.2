// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// 
// 
// HISTORY
// $Log: server.cxx,v $
// Revision 1.1.2.2  1996/03/09  20:49:03  marty
// 	Add OSF copyright
// 	[1996/03/09  20:27:32  marty]
//
// Revision 1.1.2.1  1995/12/11  19:34:34  root
// 	Submit OSF/DCE 1.2.1
// 	[1995/12/11  19:18:11  root]
// 
// $EndLog$
#include "test_com.h"

#include <iostream.h>
#include "server.hxx"

#include "matriximpl.h"

Matrix * identityMatrix;
char	*entryName;

int
main(int argc, char *argv[])
{
    char	cmd[1000];

    TRY
	// create a server
	Server s(Matrix_v0_0_s_ifspec);
	entryName = Server::buildEntryName("lookup");   // build server's name
	s.exportName(entryName);

	// creat namespace entry and register an object
    	cout << "creating entry in name space with ..." << endl;
    	s.build_system_cmd(cmd);
    	cout << cmd << endl;
    	system(cmd);

	// go into listening loop
	cout << "server is listening" << endl;
	Server::listen();
    FINALLY
	     cout << "server exiting" << endl;
    ENDTRY
    return(0);
}
