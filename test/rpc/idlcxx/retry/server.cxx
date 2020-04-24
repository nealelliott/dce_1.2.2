// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// 
// 
// HISTORY
// $Log: server.cxx,v $
// Revision 1.1.2.2  1996/03/09  20:49:50  marty
// 	Add OSF copyright
// 	[1996/03/09  20:28:09  marty]
//
// Revision 1.1.2.1  1995/12/11  19:35:42  root
// 	Submit OSF/DCE 1.2.1
// 	[1995/12/11  19:18:45  root]
// 
// $EndLog$
#include "test_com.h"

#include <iostream.h>
#include "server.hxx"

#include "objectid.h"
#include "numbermgr.h"

#ifdef _MSDOS
#define sleep(n) Sleep((n)*1000)
#endif

int     Value;
void exit_server();

int
main(int argc, char *argv[])
{
    char	*entryName;
    char	cmd[1000];
    void 	registerAnObject(char *);
    char    	*protocol_name  = "ncacn_ip_tcp";

    if (!strcmp(argv[1], protocol_name))
        Value = 1;
    else
        Value = 2;

    TRY
	// create a server
	Server s(Number_v0_0_s_ifspec, argv[1]);
	entryName = Server::buildEntryName("retry");   // build server's name
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
	Number *n = new numberMgr(Value);
	n->register_named_object((unsigned_char_t *) name);

}

void exit_server() {
	error_status_t st;
	struct timespec interval;
	interval.tv_sec = 10;
	interval.tv_nsec = 0;
	pthread_delay_np(&interval);
	cout << "exit_server" << endl;
	rpc_mgmt_stop_server_listening(
		NULL,
		&st
	);
}

void
numberMgr::die() { 
	pthread_t       t;		// thread causes server to exit

	cout << "server die" << endl;
	pthread_create(
		&t,
		pthread_attr_default,
		(pthread_startroutine_t) exit_server,
		(pthread_addr_t) 0
		);
}
