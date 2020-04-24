// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// 
// 
// HISTORY
// $Log: client.cxx,v $
// Revision 1.1.4.2  1996/09/10  18:20:58  bfc
// 	remove unecessary STDC definition
// 	[1996/09/10  18:15:08  bfc]
//
// Revision 1.1.4.1  1996/09/09  20:58:50  bfc
// 	finish TETizing and cleanup
// 	[1996/09/09  19:49:21  bfc]
// 
// Revision 1.1.2.2  1996/03/09  20:48:19  marty
// 	Add OSF copyright
// 	[1996/03/09  20:27:00  marty]
// 
// Revision 1.1.2.1  1995/12/11  19:33:31  root
// 	Submit OSF/DCE 1.2.1
// 	[1995/12/11  19:17:41  root]
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
#ifndef _MSDOS
#ifndef AIX32
#include <unistd.h>
#endif
#endif
#include <dce/rpc.h>
#include <dce/dce_error.h>
#include "matrix.h"	// This file contains class declaration for
#include "objectid.h"	// This file contains the object uuid

void	print(Matrix *);
void	readHandle(rpc_binding_handle_t & bh);
void    getUuidString(uuid_t *);
void	testBindByName();
void	testBindByUuid();
void	testBindByHandle();

// following line needed by older versions of OSF/1
#ifndef _MSDOS
extern "C" {
int	gethostname ( char *, int );
}
#endif

void
error(char *s1, char *s2 = "")
{
	cerr << "*** error: " << s1 << endl << s2 << endl;
	exit(1);
}

void
error(char *s, unsigned32 st)
{
	unsigned char	text[dce_c_error_string_len];
	int		status;

	dce_error_inq_text(st, text, &status);
	error(s, (char *) text);
}

int 
main(int argc, char *argv[])
{
	cout << "*** start of remote object binding test ***" << endl;
	testBindByName();
	testBindByUuid();
	testBindByHandle();
	cout << "*** end of remote object binding test ***" << endl;
	return 0;
}

void
testBindByName()
{
	char            name[100];
	char            hostname[100];
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
 	   return;
	}
#endif
	gethostname(hostname, sizeof(hostname));

        unsigned_char_t *entry_name = (unsigned_char_t *)getenv("RPC_DEFAULT_ENTRY");
        if (entry_name == NULL)
          sprintf(name, "/.:/idltest/bind_%s", strtok(hostname, "."));
        else
          sprintf(name, "%s", entry_name);

	m = Matrix::bind((unsigned_char_t *) name);
	assert(m != 0);

	cout << "m:" << endl;
	print(m);
	if (m) delete m;
}

void
testBindByUuid()
{
	unsigned_char_t		string_uuid[100];
	uuid_t			id;
	Matrix         		*m;

	cout << "testing bind by uuid" << endl;
	getUuidString(&id);
	cout << "uuid: " << DCE_OBJECTID << endl;
	m = Matrix::bind(id);
	cout << "m:" << endl;
	assert(m != 0);
	print(m);
	if (m) delete m;
}

void
testBindByHandle()
{
	rpc_binding_handle_t	binding_handle;
	unsigned32      	status;
	Matrix         		*m;

	readHandle(binding_handle);
	cout << "testing bind by binding handle" << endl;
	m = Matrix::bind(binding_handle);
	assert(m != 0);
	cout << "m:" << endl;
	print(m);
	if (m) delete m;
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
getUuidString(uuid_t *u)
{
	unsigned32	status;
	ifstream	from;

	uuid_from_string((unsigned_char_t *) DCE_OBJECTID, u, &status);
	CHECK_STATUS(status, "Can't convert uuid from string:", ABORT);
}

void
print(Matrix *m) 
{
	cout << "    [" << m->get(0,0) << "," << m->get(0,1) << "]" << endl;
	cout << "    [" << m->get(1,0) << "," << m->get(1,1) << "]" << endl;
}
