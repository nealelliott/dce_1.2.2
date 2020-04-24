/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: client.c,v $
 * Revision 1.1.4.2  1996/09/10  18:20:56  bfc
 * 	finish TETizing and cleanup
 * 	[1996/09/10  18:14:57  bfc]
 *
 * Revision 1.1.4.1  1996/09/09  20:58:44  bfc
 * 	finish TETizing and cleanup
 * 	[1996/09/09  19:49:05  bfc]
 * 
 * Revision 1.1.2.2  1996/03/09  20:48:13  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:26:54  marty]
 * 
 * Revision 1.1.2.1  1995/12/11  19:33:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:17:36  root]
 * 
 * $EndLog$
 */
/*
** this is a test for the c to c++ mapping of functions
** its the same test as account except that the client is written in c
*/

#ifdef TET
#define TET_ENV
#define __STDC__
#define main loc_main
#endif

#include "test_com.h"
#include <stdio.h>

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

#include "objectid.h"		/* IDL generated header file */
#include "savings.h"		/* IDL generated header file */
#include "checking.h"		/* IDL generated header file */

void readHandle(rpc_binding_handle_t * );
void testOldServer(rpc_binding_handle_t );
void testBind(rpc_binding_handle_t  );
void testDynamic(rpc_binding_handle_t );
void testRelations(rpc_binding_handle_t );

/* this will be initialized to number of named objects before we start testing */
idl_long_int BASE_NUMBER_OF_ENTRIES; 

int
main(int argc, char *argv[])
{
	rpc_binding_handle_t bh;

	readHandle(&bh);

	/* get the number of objects before we start */
	BASE_NUMBER_OF_ENTRIES = Account_getNumOTentries(bh);

	testOldServer(bh);
	testBind(bh);
	testDynamic(bh);
	testRelations(bh);


	printf("All tests passed\n");

	return(0);
}

void
testOldServer(rpc_binding_handle_t bh)
{
	Savings         *s = 0;
	Account		*a = 0;
	Checking	*c = 0;
	double		balance;

	printf("testing binding to an old object\n");

	assert(Account_getNumOTentries(bh) == BASE_NUMBER_OF_ENTRIES);

	/* open an account to start with */
	s = Savings_openOld(bh, 456.12);
	assert (s != 0);
	balance = Savings_getSavingsBalance(s);
	assert(balance == 456.12);

	/* switch to a different supported interface */
	a = Account_bind_by_iface(s);
	assert (a != 0);
	balance = Account_getAccountBalance(a);
	assert(balance == 456.12);

	/* switch to a different supported interface */
	s = Savings_bind_by_iface(a);
	assert (s != 0);
	balance = Savings_getSavingsBalance(s);
	assert(balance == 456.12);

	balance = Savings_getAccountBalance(s);
	assert(balance == 456.12);

	/* Bind the savings inteface to the checking interface on the object. */
	/* But this object does not support the checking interface so the */
	/* bind() call should return zero. */
	c = Checking_bind_by_iface(s);
	assert(c == 0);

	if (a) Account_destroy(a);
	if (s) Savings_destroy(s);
	if (c) Checking_destroy(c);

	assert(Account_getNumOTentries(bh) == BASE_NUMBER_OF_ENTRIES);

	printf("... passed\n");
}

void
testBind(rpc_binding_handle_t bh)
{
	Savings         *s;
	Account		*a;
	Checking	*c;
	double		balance;
	char		name[100];
	char		hostname[100];
        unsigned_char_t *entry_name = (unsigned_char_t *)getenv("RPC_DEFAULT_ENTRY");

	printf("testing binding by different interfaces\n");

	assert(Account_getNumOTentries(bh) == BASE_NUMBER_OF_ENTRIES);

	a = Account_bind_by_hndl(bh);
	balance = Account_getAccountBalance(a);
	assert(balance == 987.65);

	s = Savings_bind_by_hndl(bh);
	balance = Savings_getSavingsBalance(s);
	assert(balance == 987.65);

	balance = Savings_getAccountBalance(s);
	assert(balance == 987.65);

	c = Checking_bind_by_hndl(bh);
	balance = Checking_getCheckingBalance(c);
	assert(balance == 987.65);
	
	gethostname(hostname, sizeof(hostname));

        if (entry_name == NULL)
          sprintf(name, "/.:/idltest/accountc_%s", strtok(hostname, "."));
        else
          sprintf(name, "%s", entry_name);

	a = Account_bind_by_name((unsigned_char_t *) name);
	balance = Account_getAccountBalance(a);
	assert(balance == 987.65);

	s = Savings_bind_by_name((unsigned_char_t *) name);
	balance = Savings_getSavingsBalance(s);
	assert(balance == 987.65);

	balance = Savings_getAccountBalance(s);
	assert(balance == 987.65);

	c = Checking_bind_by_name((unsigned_char_t *) name);
	balance = Checking_getCheckingBalance(c);
	assert(balance == 987.65);

	a = Account_bind_by_iface(c);
	assert(a != 0);
	balance = Account_getAccountBalance(a);
	assert(balance == 987.65);

	if (a) Account_destroy(a);
	if (s) Savings_destroy(s);
	if (c) Checking_destroy(c);

	assert(Account_getNumOTentries(bh) == BASE_NUMBER_OF_ENTRIES);

	printf("... passed\n");
}

void
testDynamic(rpc_binding_handle_t bh)
{
	Savings         *s = 0;
	Account		*a = 0;
	Checking	*c = 0;
	double		balance;

	printf("testing binding to dynamic interfaces\n");

	assert(Account_getNumOTentries(bh) == BASE_NUMBER_OF_ENTRIES);

	/* open an account to start work with */
	s = Savings_openNOW(bh, 123.45);
	assert (s != 0);
	balance = Savings_getSavingsBalance(s);
	assert(balance == 123.45);

	/* call a base class operation from a derived interface pointer */
	balance = Savings_getAccountBalance(s);
	assert(balance == 123.45);

	/* switch to a different supported interface */
	a = Account_bind_by_iface(s);
	assert (a != 0);
	balance = Account_getAccountBalance(a);
	assert(balance == 123.45);

	/* switch to a different supported interface */
	s = Savings_bind_by_iface(a);
	assert (s != 0);

	/* call a base class operation from a derived interface pointer */
	balance = Savings_getAccountBalance(s);
	assert(balance == 123.45);

	/* switch to a different supported interface */
	c = Checking_bind_by_iface(a);
	assert (c != 0);

	Checking_withDraw(c, 5.00);
	balance = Checking_getCheckingBalance(c);
	assert(balance == 123.45 - 5.00);

	printf("... passed\n");


	if (a) Account_destroy(a);
	if (c) Checking_destroy(c);
	if (s) Savings_destroy(s);


	assert(Account_getNumOTentries(bh) == BASE_NUMBER_OF_ENTRIES);

	return;
}

void
readHandle(rpc_binding_handle_t * bh)
{
	unsigned32      status;
	uuid_t		u;

	READ_BINDING_FROM_FILE(bh);

	uuid_from_string((unsigned_char_t *) DCE_OBJECTID, &u, &status);
	CHECK_STATUS(status, "Can't convert uuid from string:", ABORT);

	rpc_binding_set_object(*bh, &u, &status);
	CHECK_STATUS(status, "Can't set object id into binding handle:", ABORT);

}

void
testRelations(rpc_binding_handle_t bh)
{
	Account		*a1 = 0;
	Account		*a2 = 0;
	Account		*a = 0;
	Savings         *s = 0;
	Checking	*c = 0;
	uuid_t		u;
	double		balance;
	char            name[100];
	char            hostname[100];
	unsigned_char_t *entry_name;

	unsigned32	st;

	printf("testing bind relation properties\n");

	assert(Account_getNumOTentries(bh) == BASE_NUMBER_OF_ENTRIES);

	/***************************************************************/

	/* bind to same named object by name gets same result */
	a1 = Account_bind_by_name((unsigned_char_t *) "/.:/abc");
	a2 = Account_bind_by_name((unsigned_char_t *) "/.:/abc");
	assert(a1 == a2);
	assert(a1 == 0);

	/***************************************************************/

	/* bind to same named object by name gets same result */
	entry_name = (unsigned_char_t *) getenv("RPC_DEFAULT_ENTRY");
	if (entry_name == NULL) {
		gethostname(hostname, sizeof(hostname));
		sprintf(name, "/.:/idltest/accountc_%s",strtok(hostname, "."));
	} else {
		sprintf(name, "%s", entry_name);
	}

	a1 = Account_bind_by_name((unsigned_char_t *) name);
	a2 = Account_bind_by_name((unsigned_char_t *) name);
	assert(a1 == a2);

	/***************************************************************/

	/* bind to same named object by uuid gets same result */
	uuid_create(&u, &st);
	assert(st == uuid_s_ok);

	a1 = Account_bind_by_uuid(&u);
	a2 = Account_bind_by_uuid(&u);
	assert(a1 == a2);
	assert(a1 == 0);

	/***************************************************************/

	/* bind to same named object by uuid gets same result */
	uuid_from_string((unsigned_char_t *) DCE_OBJECTID, &u, &st);
	assert(st == uuid_s_ok);

	a1 = Account_bind_by_uuid(&u);
	a2 = Account_bind_by_uuid(&u);
	assert(a1 == a2);

	/**************************************************************/

	/* bind to same named object by handle gets same result */
	a1 = Account_bind_by_hndl(bh);
	a2 = Account_bind_by_hndl(bh);
	assert(a1 == a2);

	/**************************************************************/

	/* test that bind is reflexive */
	s = Savings_openNOW(bh, 321.23);		/* start with an s */

	s = Savings_bind_by_iface(s);			/* s to s */
	assert(s != 0);

	/* balance should be the same */
	assert(Savings_getAccountBalance(s) == 321.23);
	if (s) Savings_destroy(s);			/* delete s; */
	/**************************************************************/

	/* test that bind is symetric */
	s = Savings_openNOW(bh, 321.23);		/* start with an s */
	assert(s != 0);

	c = Checking_bind_by_iface(s);			/* s to c */
	assert(c != 0);

	s = Savings_bind_by_iface(c);			/* c to s */
	assert(s != 0);

	/* all balances should be the same */
	assert(Savings_getSavingsBalance(s) == 321.23);
	assert(Checking_getCheckingBalance(c) == 321.23);

	if (s) Savings_destroy(s);			/* delete s; */
	if (c) Checking_destroy(c);			/* delete c; */

	/**************************************************************/

	/* test that bind is transitive */
	s = Savings_openNOW(bh, 321.23);		/* start with an s */
	assert(s != 0);

	c = Checking_bind_by_iface(s);			/* s to c */
	assert(c != 0);

	a = Account_bind_by_iface(c);			/* c to a */
	assert(a != 0);

	s = Savings_bind_by_iface(a);			/* a to s */
	assert(s != 0);

	/* all balances should be the same */
	assert(Savings_getSavingsBalance(s) == 321.23);
	assert(Checking_getCheckingBalance(c) == 321.23);
	assert(Account_getAccountBalance(a) == 321.23);
	if (s) Savings_destroy(s);			/* delete s; */
	if (c) Checking_destroy(c);			/* delete c; */
	if (a) Account_destroy(a);			/* delete a; */

	return;
}










