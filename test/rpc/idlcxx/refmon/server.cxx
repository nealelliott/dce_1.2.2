// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// 
// 
// HISTORY
// $Log: server.cxx,v $
// Revision 1.1.2.2  1996/03/09  20:49:44  marty
// 	Add OSF copyright
// 	[1996/03/09  20:28:04  marty]
//
// Revision 1.1.2.1  1995/12/11  19:35:35  root
// 	Submit OSF/DCE 1.2.1
// 	[1995/12/11  19:18:41  root]
// 
// $EndLog$
#include "test_com.h"

#include <iostream.h>
#include <ctype.h>
#include "server.hxx"

#include "matriximpl.h"

// DCE Authentication Service Protocols
struct Auth_names {
	char *name;
	unsigned32 svc;
} authn_names[] = {
	{"none",	rpc_c_authn_none},
	{"private",	rpc_c_authn_dce_secret},
	{"public",	rpc_c_authn_dce_public},
	{"default",	rpc_c_authn_default},
	{NULL,		rpc_c_authn_default}
};

unsigned32 lookup_name(struct Auth_names *, char *s);

int
main(int argc, char *argv[])
{
    char	*entryName;
    char	cmd[1000];
    void 	registerAnObject(char *);
    idl_boolean	auth = true;
    char	*auth_principal;
    unsigned32	authn_protocol;
    unsigned32	st;


    // use these as defaults since the test suites are executed
    // normally without parameters
    auth_principal = "refmon_test";
    authn_protocol = rpc_c_authn_dce_secret;

    if (auth) {
	cout << "using protocol " << authn_protocol;
	cout << ", name \"" << auth_principal << "\"" << endl;
    } else {
	cout << "running server authenticated" << endl;
    }

    TRY
	// create a server
	Server s(Matrix_v0_0_s_ifspec);
	entryName = Server::buildEntryName("refmon");   // build server's name
	s.exportName(entryName);
	delete [] entryName;

    	if (auth) {
		rpc_server_register_auth_info(
			(unsigned_char_p_t) auth_principal, 
			authn_protocol, 
			NULL,
			"refmon.pwd",
			&st
		);
        	if (st != rpc_s_ok) {
            		cout << "Error setting identity " << auth_principal;
	    		cout << " for authn protocol " << authn_protocol;
	    		cout << " - " << error_text(st) << endl;
	    		exit(1);
		}
    	}
	// go into listening loop
	cout << "server is listening" << endl;
	Server::listen();
    FINALLY
	     cout << "server exiting" << endl;
    ENDTRY
    return(0);
}


/*
 * Lookup a name in a table.
 */

unsigned32
lookup_name(
	struct Auth_names table[],
	char *s
) {
	int i;

    	if (isdigit(s[0]))
        	return (atoi(s));

    	for (i = 0; table[i].name != NULL; i++)
        	if (strcmp(table[i].name, s) == 0)
	            	return (table[i].svc);

	cout << "bad authentication protocol - bye" << endl;
	exit(1);
	return(0);
}

