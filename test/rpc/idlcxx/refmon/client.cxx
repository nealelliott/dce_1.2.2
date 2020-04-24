// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// 
// 
// HISTORY
// $Log: client.cxx,v $
// Revision 1.1.4.2  1996/09/10  18:21:11  bfc
// 	remove unecessary STDC definition
// 	[1996/09/10  18:16:22  bfc]
//
// Revision 1.1.4.1  1996/09/09  20:59:19  bfc
// 	finish TETizing and cleanup
// 	[1996/09/09  20:43:03  bfc]
// 
// Revision 1.1.2.2  1996/03/09  20:49:38  marty
// 	Add OSF copyright
// 	[1996/03/09  20:27:58  marty]
// 
// Revision 1.1.2.1  1995/12/11  19:35:27  root
// 	Submit OSF/DCE 1.2.1
// 	[1995/12/11  19:18:36  root]
// 
// $EndLog$
#ifdef TET
#define TET_ENV
#define main loc_main
#endif

#include "test_com.h"
#include <assert.h>
#include <dce/dce_error.h>
#include <iostream.h>
#include "matrix.h"		// IDL generated header file

int
main(int argc, char *argv[])
{
	Matrix         *m;

	cout << "creating a remote matrix" << endl;
	m = Matrix::new2x2(1, 2, 3, 4);
	assert(m->get(0,0) == 1);
	assert(m->get(0,1) == 2);
	assert(m->get(1,0) == 3);
	assert(m->get(1,1) == 4);

	cout << "calling set() operation without authorization" << endl;
	m->set(0,0,99);

	// since set() was called with authorization, operation should
	// have not changed anything.
	assert(m->get(0,0) == 1);
	assert(m->get(0,1) == 2);
	assert(m->get(1,0) == 3);
	assert(m->get(1,1) == 4);

#if 0
	// can call secure with no parameters but that is not
	// the recommended way since defaults may vary.
	m->secure();			
#else

	cout << "setting security privileges on object" << endl;
	m->secure(
		(unsigned char *) "refmon_test", // server principal name
		rpc_c_protect_level_pkt_integ,     // protection level
		rpc_c_authn_dce_secret,            // authentication protocol
		NULL,                              // inherited login context
		rpc_c_authz_name                   // authorization policy
	);
#endif
	// since we now have set security flags, the set() operation should work
	m->set(0,0,99);
	m->set(0,1,200);
	m->set(1,0,13);
	m->set(1,1,879);
	assert(m->get(0,0) == 99);
	assert(m->get(0,1) == 200);
	assert(m->get(1,0) == 13);
	assert(m->get(1,1) == 879);

	cout << "all tests passed" << endl;

	return 0;
}
