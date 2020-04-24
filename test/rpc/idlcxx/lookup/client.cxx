// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// 
// 
// HISTORY
// $Log: client.cxx,v $
// Revision 1.1.4.2  1996/09/10  18:21:04  bfc
// 	remove unecessary STDC definition
// 	[1996/09/10  18:15:39  bfc]
//
// Revision 1.1.4.1  1996/09/09  20:59:04  bfc
// 	finish TETizing and cleanup
// 	[1996/09/09  20:41:55  bfc]
// 
// Revision 1.1.2.2  1996/03/09  20:48:56  marty
// 	Add OSF copyright
// 	[1996/03/09  20:27:27  marty]
// 
// Revision 1.1.2.1  1995/12/11  19:34:27  root
// 	Submit OSF/DCE 1.2.1
// 	[1995/12/11  19:18:08  root]
// 
// $EndLog$
#ifdef TET
#define TET_ENV
#define main loc_main
#endif

#include "test_com.h"
#include <iostream.h>
#include <fstream.h>
#include <stdlib.h>
#include <assert.h>
#include <dce/rpc.h>
#include <dce/rpcexc.h>
#include <dce/dce_error.h>
#include "matrix.h"	// This file contains class declaration for

// following line needed by older versions of OSF/1 (pre v3.0)
extern "C" {
int     gethostname ( char *, int );
}

void	print(Matrix *);
int	testBindByName();

int 
main(int argc, char *argv[])
{
	int ret;

	cout << "*** start of remote object binding test ***" << endl;

	cout << "should raise an object-not-found exception" << endl;
	ret = testBindByName();
	assert(ret == -1);
	cout << "test passed" << endl;

	cout << "should be able to print it now" << endl;
	ret = testBindByName();
	assert(ret == 0);
	cout << "test passed" << endl;

	cout << "all tests passed" << endl;
	cout << "*** end of remote object binding test ***" << endl;
	return 0;
}

int
testBindByName()
{
	char		name[100];
	char		hostname[100];
	Matrix		*m;

#ifdef _MSDOS
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD( 1, 1 );
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
 	   /* Tell the user that we couldn't find a useable */
 	   /* winsock.dll.     */
	   cout << "Couldn't access a useable winsock.dll" << endl;
 	   return -1; 
	}
#endif
	gethostname(hostname, sizeof(hostname));

        unsigned_char_t *entry_name = (unsigned_char_t *)getenv("RPC_DEFAULT_ENTRY");
        if (entry_name == NULL)
          sprintf(name, "/.:/idltest/lookup_%s", strtok(hostname, "."));
        else
          sprintf(name, "%s", entry_name);

//	cout << "testing bind by name" << name << endl;
//	cout << "name: " << name << endl;

	m = Matrix::bind((unsigned_char_t *) name);
	cout << "m:" << endl;
	TRY
	    print(m);
	CATCH (rpc_x_object_not_found)
	    cout << "client caught rpc_x_object_not_found exception" << endl;
	    return -1;
	CATCH_ALL
	    cout << "client caught unexpected exception" << endl;
	    exit(1);
	ENDTRY
	return 0;
}

void
print(Matrix *m) 
{
	cout << "    [" << m->get(0,0) << "," << m->get(0,1) << "]" << endl;
	cout << "    [" << m->get(1,0) << "," << m->get(1,1) << "]" << endl;
}
