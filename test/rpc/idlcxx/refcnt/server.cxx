// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// 
// 
// HISTORY
// $Log: server.cxx,v $
// Revision 1.1.2.2  1996/03/09  20:49:36  marty
// 	Add OSF copyright
// 	[1996/03/09  20:27:56  marty]
//
// Revision 1.1.2.1  1995/12/11  19:35:23  root
// 	Submit OSF/DCE 1.2.1
// 	[1995/12/11  19:18:34  root]
// 
// $EndLog$
#include "test_com.h"

#include <iostream.h>
#include "server.hxx"

#include "matriximpl.h"

extern idl_long_int rpc_ss_get_num_ot_entries();

int
main(int argc, char *argv[])
{
    char	*entryName;
    char	cmd[1000];
    void 	registerAnObject(char *);
    unsigned32	st;

    TRY
	// create a server
	Server s(Matrix_v0_0_s_ifspec);
	entryName = Server::buildEntryName("refcnt");   // build server's name
	s.exportName(entryName);
	delete [] entryName;
	rpc_mgmt_set_server_stack_size(100000, &st);

	// go into listening loop
	cout << "server is listening" << endl;
	Server::listen();
    FINALLY
	     cout << "server exiting" << endl;
    ENDTRY
    return(0);
}

idl_long_int
Matrix::getNumOTentries() {
	return rpc_ss_get_num_ot_entries();
}

