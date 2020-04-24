// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// 
// 
// HISTORY
// $Log: statics.cxx,v $
// Revision 1.1.2.2  1996/03/09  20:48:06  marty
// 	Add OSF copyright
// 	[1996/03/09  20:26:49  marty]
//
// Revision 1.1.2.1  1995/12/11  19:33:15  root
// 	Submit OSF/DCE 1.2.1
// 	[1995/12/11  19:17:31  root]
// 
// $EndLog$
#include <dce/idlbase.h>
#include <dce/rpc.h>

/* %dec_begin%msrpc */
#if defined(_MSDOS) || defined(WIN32)
#include <dce/stubbase.h>
#define UUID_DEFINED
#define UUID uuid_t
#define UUID_VECTOR_DEFINED
#define UUID_VECTOR uuid_vector_t
#include <rpc.h>
#include <rpcndr.h>
#endif
/* %dec_end%msrpc */


#include <assert.h>
#include <iostream.h>
#include <dce/trace.hxx>
#include "savings.h"
#include "checking.h"
#include "nowaccount.h"

extern idl_long_int rpc_ss_get_num_ot_entries();

Savings *
openNOW(handle_t h, double amt)
{
	Trace("openNOW()");
	return new nowAccount(amt);
}

Savings *
openOld(handle_t h, double amt)
{
	Trace("openOld()");
	return new oldAccount(amt);
}

idl_long_int
getNumOTentries(handle_t h) {
	Trace("getNumOTentries()");
	return rpc_ss_get_num_ot_entries();
}

void
remote_local_test(handle_t h, Account *a)
{
	Trace("remote_local_test()");

	Savings         *s = 0;
	Checking	*c = 0;
	double		bal;
	double		balance;
	unsigned32	st;

	assert (a != 0);

	bal = a->getAccountBalance();

	// verify balance
	balance = a->getAccountBalance();
	assert(balance == bal);

	// switch to a different supported interface
	delete s;
	s = Savings::bind(a);
	assert (s != 0);
	balance = s->getSavingsBalance();
	assert(balance == bal);

	// call a base class operation from a derived interface pointer
	balance = s->getAccountBalance();
	assert(balance == bal);

	// switch to a different supported interface
	delete c;
	c = Checking::bind(a);
	assert (c != 0);

	c->withDraw(5.00);
	balance = c->getCheckingBalance();
	assert(balance == bal - 5.00);


	delete a;
	delete c;
	delete s;

	////////////////////////////////////////////////////////////////

	cout << "... passed" << endl;

	return;
}
