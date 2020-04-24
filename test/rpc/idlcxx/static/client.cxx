// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// 
// 
// HISTORY
// $Log: client.cxx,v $
// Revision 1.1.4.2  1996/09/10  18:21:14  bfc
// 	remove unecessary STDC definition
// 	[1996/09/10  18:16:38  bfc]
//
// Revision 1.1.4.1  1996/09/09  20:59:26  bfc
// 	finish TETizing and cleanup
// 	[1996/09/09  20:43:35  bfc]
// 
// Revision 1.1.2.2  1996/03/09  20:50:04  marty
// 	Add OSF copyright
// 	[1996/03/09  20:28:17  marty]
// 
// Revision 1.1.2.1  1995/12/11  19:36:01  root
// 	Submit OSF/DCE 1.2.1
// 	[1995/12/11  19:18:53  root]
// 
// $EndLog$
#ifdef TET
#define TET_ENV
#define main loc_main
#endif

#include "test_com.h"

#include <iostream.h>
#include <assert.h>
#include "static.h"

void	testIDLstatic();
void	testACFstatic();
void	testACFnameStatic();

int 
main(int argc, char *argv[])
{
	cout << "*** start of remote object static function test ***" << endl;

	testIDLstatic();
	testACFstatic();
	testACFnameStatic();

	cout << "all tests passed" << endl;
	cout << "*** end of remote object binding test ***" << endl;

        return 0;
}

void
testIDLstatic()
{
	assert(Static::foo() == 1);
}

void
testACFstatic()
{
	assert(Static::fee() == 2);
}

void
testACFnameStatic()
{
	assert(Static::fii() == 3);
}
