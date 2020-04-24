// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// 
// 
// HISTORY
// $Log: client.cxx,v $
// Revision 1.1.4.2  1996/09/10  18:20:51  bfc
// 	remove unecessary STDC definition
// 	[1996/09/10  18:14:33  bfc]
//
// Revision 1.1.4.1  1996/09/09  20:58:34  bfc
// 	finish TETizing and cleanup
// 	[1996/09/09  19:48:32  bfc]
// 
// Revision 1.1.2.2  1996/03/09  20:47:49  marty
// 	Add OSF copyright
// 	[1996/03/09  20:26:37  marty]
// 
// Revision 1.1.2.1  1995/12/11  19:32:56  root
// 	Submit OSF/DCE 1.2.1
// 	[1995/12/11  19:17:19  root]
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

#if defined(__osf__)
#include <unistd.h>
#elif defined (__VMS)
#include <socket.h>
#elif defined (_MSDOS)
#include <winsock.h>
#endif

#include "objectid.h"		// IDL generated header file
#include "savings.h"		// IDL generated header file
#include "checking.h"		// IDL generated header file

void readHandle(rpc_binding_handle_t & );
void testOldServer(const rpc_binding_handle_t &);
void testBind(const rpc_binding_handle_t & );
void testDynamic(const rpc_binding_handle_t &);
void testRelations(const rpc_binding_handle_t &);

// this will be initialized to number of named objects before we start testing
idl_long_int BASE_NUMBER_OF_ENTRIES; 

int
main(int argc, char *argv[])
{
	rpc_binding_handle_t bh;

	readHandle(bh);

	// get the number of objects before we start
	BASE_NUMBER_OF_ENTRIES = Account::getNumOTentries(bh);

	testOldServer(bh);
	testBind(bh);
	testDynamic(bh);
	testRelations(bh);

	assert(Account::getNumOTentries(bh) == BASE_NUMBER_OF_ENTRIES);

	cout << "All tests passed" << endl;

	return(0);
}

void
testOldServer(const rpc_binding_handle_t & bh)
{
	Savings         *s = 0;
	Account		*a = 0;
	Checking	*c = 0;
	double		balance;

	cout << "testing binding to an old object" << endl;

	assert(Account::getNumOTentries(bh) == BASE_NUMBER_OF_ENTRIES);

	// open an account to start with
	s = Savings::openOld(bh, 456.12);
	assert (s != 0);
	balance = s->getSavingsBalance();
	assert(balance == 456.12);

	// switch to a different supported interface

	a = Account::bind(s);
	assert (a != 0);
	balance = a->getAccountBalance();
	assert(balance == 456.12);

	// switch to a different supported interface

	s = Savings::bind(a);
	assert (s != 0);
	balance = s->getSavingsBalance();
	assert(balance == 456.12);

	balance = s->getAccountBalance();
	assert(balance == 456.12);

	// Bind the savings inteface to the checking interface on the object.
	// But this object does not support the checking interface so the
	// bind() call should return zero.
	c = Checking::bind(s);
	assert(c == 0);

	delete a;
	delete c;
	delete s;

	assert(Account::getNumOTentries(bh) == BASE_NUMBER_OF_ENTRIES);

	cout << "... passed" << endl;
}

void
testBind(const rpc_binding_handle_t & bh)
{
	Savings         *s;
	Account		*a;
	Checking	*c;
	double		balance;
	char		name[100];
	char		hostname[100];

	cout << "testing binding by different interfaces" << endl;

	assert(Account::getNumOTentries(bh) == BASE_NUMBER_OF_ENTRIES);

	a = Account::bind(bh);
	balance = a->getAccountBalance();
	assert(balance == 987.65);

	s = Savings::bind(bh);
	balance = s->getSavingsBalance();
	assert(balance == 987.65);

	balance = s->getAccountBalance();
	assert(balance == 987.65);

	c = Checking::bind(bh);
	balance = c->getCheckingBalance();
	assert(balance == 987.65);
	
	gethostname(hostname, sizeof(hostname));

        unsigned_char_t *entry_name = (unsigned_char_t *)getenv("RPC_DEFAULT_ENTRY");
        if (entry_name == NULL)
          sprintf(name, "/.:/idltest/account_%s", strtok(hostname, "."));
        else
          sprintf(name, "%s", entry_name);

	a = Account::bind((unsigned_char_t *) name);
	balance = a->getAccountBalance();
	assert(balance == 987.65);

	s = Savings::bind((unsigned_char_t *) name);
	balance = s->getSavingsBalance();
	assert(balance == 987.65);

	balance = s->getAccountBalance();
	assert(balance == 987.65);

	c = Checking::bind((unsigned_char_t *) name);
	balance = c->getCheckingBalance();
	assert(balance == 987.65);

	a = Account::bind(c);
	assert(a != 0);
	balance = a->getAccountBalance();
	assert(balance == 987.65);

	assert(Account::getNumOTentries(bh) == BASE_NUMBER_OF_ENTRIES);

	cout << "... passed" << endl;
}

void
testDynamic(const rpc_binding_handle_t & bh)
{
	Savings         *s = 0;
	Account		*a = 0;
	Checking	*c = 0;
	double		balance;

	cout << "testing binding to dynamic interfaces" << endl;

	assert(Account::getNumOTentries(bh) == BASE_NUMBER_OF_ENTRIES);

	// open an account to start work with

	s = Savings::openNOW(bh, 123.45);
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

	delete a;
	delete c;
	delete s;

	assert(Account::getNumOTentries(bh) == BASE_NUMBER_OF_ENTRIES);

	return;
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

void
testRelations(const rpc_binding_handle_t & bh)
{
	Account		*a1 = 0;
	Account		*a2 = 0;
	Account		*a = 0;
	Savings         *s = 0;
	Checking	*c = 0;
	uuid_t		u;
	double		balance;
	char		name[100];
	char            hostname[100];
	unsigned_char_t *entry_name;
	unsigned32	st;

	cout << "testing bind relation properties" << endl;

	assert(Account::getNumOTentries(bh) == BASE_NUMBER_OF_ENTRIES);

	////////////////////////////////////////////////////////////////

	a1 = Account::bind((unsigned_char_t *) "/.:/abc");
	a2 = Account::bind((unsigned_char_t *) "/.:/abc");
	assert(a1 == a2);
	assert(a1 == 0);

	////////////////////////////////////////////////////////////////

	// bind to same named object by name gets same result
	entry_name = (unsigned_char_t *) getenv("RPC_DEFAULT_ENTRY");
	if (entry_name == NULL) {
		gethostname(hostname, sizeof(hostname));
		sprintf(name, "/.:/idltest/account_%s",strtok(hostname, "."));
	} else {
		sprintf(name, "%s", entry_name);
	}

	a1 = Account::bind((unsigned_char_t *) name);
	a2 = Account::bind((unsigned_char_t *) name);
	assert(a1 == a2);

	////////////////////////////////////////////////////////////////

	// bind to same named object by uuid gets same result
	uuid_create(&u, &st);
	assert(st == uuid_s_ok);

	a1 = Account::bind(u);
	a2 = Account::bind(u);
	assert(a1 == a2);
	assert(a1 == 0);

	////////////////////////////////////////////////////////////////
 
	// bind to same named object by uuid gets same result
	uuid_from_string((unsigned_char_t *) DCE_OBJECTID, &u, &st);
	assert(st == uuid_s_ok);

	a1 = Account::bind(u);
	a2 = Account::bind(u);
	assert(a1 == a2);

	////////////////////////////////////////////////////////////////

	// bind to same named object by handle gets same result
	a1 = Account::bind(bh);
	a2 = Account::bind(bh);
	assert(a1 == a2);

	////////////////////////////////////////////////////////////////

	// test that bind is reflexive
	s = Savings::openNOW(bh, 321.23);		// start with an s

	s = Savings::bind(s);			// s to s
	assert(s != 0);

	// balance should be the same
	assert(s->getAccountBalance() == 321.23);

	delete s;

	////////////////////////////////////////////////////////////////

	// test that bind is symetric
	s = Savings::openNOW(bh, 321.23);		// start with an s
	assert(s != 0);

	c = Checking::bind(s);			// s to c
	assert(c != 0);

	s = Savings::bind(c);			// c to s
	assert(s != 0);

	// all balances should be the same
	assert(s->getSavingsBalance() == 321.23);
	assert(c->getCheckingBalance() == 321.23);

	delete s;
	delete c;

	////////////////////////////////////////////////////////////////

	// test that bind is transitive
	s = Savings::openNOW(bh, 321.23);		// start with an s
	assert(s != 0);

	c = Checking::bind(s);			// s to c
	assert(c != 0);

	a = Account::bind(c);			// c to a
	assert(a != 0);

	s = Savings::bind(a);			// a to s
	assert(s != 0);

	// all balances should be the same
	assert(s->getSavingsBalance() == 321.23);
	assert(c->getCheckingBalance() == 321.23);
	assert(a->getAccountBalance() == 321.23);
	delete s;
	delete c;
	delete a;

	assert(Account::getNumOTentries(bh) == BASE_NUMBER_OF_ENTRIES);
	return;
}
