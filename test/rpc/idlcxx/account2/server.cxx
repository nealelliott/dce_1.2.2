// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// 
// 
// HISTORY
// $Log: server.cxx,v $
// Revision 1.1.4.2  1996/09/10  18:20:55  bfc
// 	fix typo
// 	[1996/09/10  18:14:51  bfc]
//
// Revision 1.1.4.1  1996/09/09  20:58:41  bfc
// 	finish TETizing and cleanup
// 	[1996/09/09  19:48:54  bfc]
// 
// Revision 1.1.2.2  1996/03/09  20:48:05  marty
// 	Add OSF copyright
// 	[1996/03/09  20:26:48  marty]
// 
// Revision 1.1.2.1  1995/12/11  19:33:14  root
// 	Submit OSF/DCE 1.2.1
// 	[1995/12/11  19:17:30  root]
// 
// $EndLog$
#include "test_com.h"

#include <iostream.h>
#include "server.hxx"

#include "objectid.h"
#include "nowaccount.h"

Account *globalAccount;

int
main(int argc, char *argv[])
{
    char	*entryName;
    char	cmd[1000];
    void 	registerAnObject(char *);


    TRY
	// create a server
	Server s(Account_v0_0_s_ifspec);
	entryName = Server::buildEntryName("account2");   // build server's name
	s.exportName(entryName);

	// creat namespace entry and register an object
    	cout << "creating entry in name space with ..." << endl;
    	s.build_system_cmd(DCE_OBJECTID, cmd);
    	cout << cmd << endl;
    	system(cmd);
    	registerAnObject(entryName);

	// write binding information to a file
	s.printBindingToFile();

	// go into listening loop
	cout << "server is listening" << endl;
	Server::listen();
    FINALLY
	     cout << "server exiting" << endl;
    ENDTRY
    return(0);
}

void 
registerAnObject(char *name) {
	cout << "registering an object with name " << name << endl;
	globalAccount = new nowAccount(987.65);
	globalAccount->register_named_object( (unsigned_char_t *) name);
	cout << "registered global account, balance: ";
	cout << globalAccount->getAccountBalance() << endl;
	return;
}
