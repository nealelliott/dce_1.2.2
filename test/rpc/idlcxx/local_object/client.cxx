// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// 
// 
// HISTORY
// $Log: client.cxx,v $
// Revision 1.1.4.2  1996/09/10  18:21:03  bfc
// 	remove unecessary STDC definition
// 	[1996/09/10  18:15:34  bfc]
//
// Revision 1.1.4.1  1996/09/09  20:59:01  bfc
// 	finish TETizing and cleanup
// 	[1996/09/09  20:41:45  bfc]
// 
// Revision 1.1.2.2  1996/03/09  20:48:51  marty
// 	Add OSF copyright
// 	[1996/03/09  20:27:24  marty]
// 
// Revision 1.1.2.1  1995/12/11  19:34:22  root
// 	Submit OSF/DCE 1.2.1
// 	[1995/12/11  19:18:04  root]
// 
// $EndLog$
#ifdef TET
#define TET_ENV
#define main loc_main
#endif

#include "test_com.h"
#include <iostream.h>

#include <assert.h>
#include <stdlib.h>
#include <dce/dce_error.h>

#include "savings.h"		// IDL generated header file
#include "checking.h"		// IDL generated header file
#include "nowaccount.h"		// local class definition file

// following line needed by older versions of OSF/1 (pre 3.0)
extern "C" {
extern int     gethostname ( char *, int );
}

void testLocalObject();

int
main(int argc, char *argv[])
{
	testLocalObject();

	cout << "All tests passed" << endl;

	return(0);
}

void
testLocalObject()
{
	Savings         *s = 0;
	Account		*a = 0;
	Checking	*c = 0;
	double		balance;

	cout << "testing binding to local object" << endl;

	// open an account to start work with
	s = new nowAccount(123.45);
	assert (s != 0);

	balance = s->getSavingsBalance();
	assert(balance == 123.45);

	// call a base class operation from a derived interface pointer
	balance = s->getAccountBalance();
	assert(balance == 123.45);

	// switch to a different supported interface
	a = Account::bind(s);
	assert (a != 0);
	balance = a->getAccountBalance();
	assert(balance == 123.45);

	// switch to a different supported interface
	s = Savings::bind(a);
	assert (s != 0);

	// call a base class operation from a derived interface pointer
	balance = s->getAccountBalance();
	assert(balance == 123.45);

	// switch to a different supported interface
	c = Checking::bind(a);
	assert (c != 0);

	c->withDraw(5.00);
	balance = c->getCheckingBalance();
	assert(balance == 123.45 - 5.00);

	cout << "... passed" << endl;

	return;
}

