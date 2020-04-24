// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// 
// 
// HISTORY
// $Log: client.cxx,v $
// Revision 1.1.4.2  1996/09/10  18:21:15  bfc
// 	remove unecessary STDC definition
// 	[1996/09/10  18:16:44  bfc]
//
// Revision 1.1.4.1  1996/09/09  20:59:28  bfc
// 	finish TETizing and cleanup
// 	[1996/09/09  20:43:45  bfc]
// 
// Revision 1.1.2.2  1996/03/09  20:50:15  marty
// 	Add OSF copyright
// 	[1996/03/09  20:28:24  marty]
// 
// Revision 1.1.2.1  1995/12/11  19:36:12  root
// 	Submit OSF/DCE 1.2.1
// 	[1995/12/11  19:19:00  root]
// 
// $EndLog$
#ifdef TET
#define TET_ENV
#define main loc_main
#endif

#include "test_com.h"

#include <iostream.h>
#include <assert.h>

#include "savings.h"		// IDL generated header file
#include "checking.h"		// IDL generated header file
#include "nowaccount.h"		
#include "objectid.h"

#if defined(__osf__)
#include <unistd.h>
#elif defined (__VMS)
#include <socket.h>
#elif defined (_MSDOS)
#include <winsock.h>
#endif

void readHandle(rpc_binding_handle_t & );
void testNoServerStub(const rpc_binding_handle_t &);
void testNoClientStub(const rpc_binding_handle_t & );

int
main(int argc, char *argv[])
{
	rpc_binding_handle_t bh;

	readHandle(bh);

	testNoClientStub(bh);
	testNoServerStub(bh);

	cout << "All tests passed" << endl;

	return(0);
}

void
testNoClientStub(const rpc_binding_handle_t & bh)
{
	Account		*a = 0;
	double		balance;

	cout << "testing access with no client stub" << endl;

	// open an account to start with (no checking interface on oldAccount)
	a = new oldAccount(456.12);
	assert (a != 0);
	balance = a->getAccountBalance();
	assert(balance == 456.12);

	// pass an Account * as an [in] parameter to a remote procedure.
	// Server is not linked with account_cstub.o so that the call will 
	// raise an exception.
	TRY
		Account::balanceFromServer(bh, a);
	CATCH (rpc_x_no_client_stub)
		cout << "... passed" << endl;
	CATCH_ALL
		cout << "... failed" << endl;
		RERAISE;

	ENDTRY
}


void
testNoServerStub(const rpc_binding_handle_t & bh)
{
	Account		*a = 0;
	Checking	*c = 0;
	double		balance;

	cout << "testing access with no server stub" << endl;

	// open an account to start with and get the checking interface
	c = new nowAccount(456.12);
	assert (c != 0);
	balance = c->getCheckingBalance();
	assert(balance == 456.12);

	// pass a Checking * as an [in] parameter to a remote procedure.
	// Client is not linked with checking_sstub.o so that the call will 
	// raise an exception as the client tries to marshall the object ref.
	TRY
		Account::checkBalFromServer(bh, c);
	CATCH (rpc_x_no_server_stub)
		cout << "... passed" << endl;
	CATCH_ALL
		cout << "... failed" << endl;
		RERAISE;

	ENDTRY
}


void
readHandle(rpc_binding_handle_t & bh)
{
	unsigned32      status;
	uuid_t		u;

	READ_BINDING_FROM_FILE(&bh);

	uuid_from_string((unsigned_char_t *) DCE_OBJECTID, &u, &status);
	CHECK_STATUS(status, "Can't convert uuid from string:", ABORT);

	rpc_binding_set_object(bh, &u, &status);
	CHECK_STATUS(status, "Can't set object id into binding handle:", ABORT);
}

