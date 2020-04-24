// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// 
// 
// HISTORY
// $Log: client.cxx,v $
// Revision 1.1.4.2  1996/09/10  18:21:06  bfc
// 	remove unecessary STDC definition
// 	[1996/09/10  18:15:51  bfc]
//
// Revision 1.1.4.1  1996/09/09  20:59:07  bfc
// 	finish TETizing and cleanup
// 	[1996/09/09  20:42:12  bfc]
// 
// Revision 1.1.2.2  1996/03/09  20:49:05  marty
// 	Add OSF copyright
// 	[1996/03/09  20:27:34  marty]
// 
// Revision 1.1.2.1  1995/12/11  19:34:37  root
// 	Submit OSF/DCE 1.2.1
// 	[1995/12/11  19:18:14  root]
// 
// $EndLog$
#ifdef TET
#define TET_ENV
#define main loc_main
#endif

#include "test_com.h"
#include <stdio.h>
#include <stdlib.h>
#ifndef _MSDOS
#ifdef __VMS
extern "C" {
    extern char *optarg;
    extern int getopt(int, char **, const char *);
}
#else
#ifndef AIX32
#include <unistd.h>
#endif
#endif
#endif
#include <dce/dce_error.h>
#include <dce/trace.hxx>
#include "objectid.h"		// IDL generated header file
#include "matrix.h"		// IDL generated header file
#include "matriximpl.h"		// Matrix implementation file
#ifndef _MSDOS
#if defined(VMS) || defined(__VMS)
#include <in.h>
#include <inet.h>
#else
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#endif

#if defined(__osf__)
#include <unistd.h>
#elif defined (__VMS)
#include <socket.h>
#elif defined (_MSDOS)
#include <winsock.h>
#endif

int
main(int argc, char *argv[])
{
	Trace("main");
	char           *answer;
	Matrix         *m;
	Matrix         *mx;
	unsigned_char_t *string_binding;
	rpc_binding_handle_t binding_handle;
	unsigned32      status;
	uuid_t          u;
	int             c, errflg = 0;
	int             readers = 1, writers = 1;
	int             freeBufs = 1;
	int             doStats = FALSE;
	int             use_bh = TRUE;
	FILE           *fp;
	char            buf[100], *s;
	char           *prot, *host, *endp;
	void            usage(char *);
	idl_long_int	x,z;
	Struct1		struct1;
	Union1		union1;
	int		y;
	Matrix          *myarr[2];
	Struct2    	 struct2;
	double		d1, d2, d3, d4;
	char            name[100];
	char            hostname[100];

	READ_BINDING_FROM_FILE(&binding_handle);

	uuid_from_string((unsigned_char_t *) DCE_OBJECTID, &u, &status);
	CHECK_STATUS(status, "Can't convert uuid from string:", ABORT);

	rpc_binding_set_object(binding_handle, &u, &status);
	CHECK_STATUS(status, "Can't set object id into binding handle:", ABORT);

	m = Matrix::bind(binding_handle);
	cout << "done" << endl;
	print(m);

	cout << "trying bind by uuid" << endl;
	uuid_from_string((unsigned_char_t *) DCE_OBJECTID, &u, &status);
	CHECK_STATUS(status, "Can't convert uuid from string:", ABORT);

	m = Matrix::bind(u);
	cout << "done" << endl;
	print(m);

	cout << "trying bind by name" << endl;
	gethostname(hostname, sizeof(hostname));

        unsigned_char_t *entry_name = (unsigned_char_t *)getenv("RPC_DEFAULT_ENTRY");
        if (entry_name == NULL)
          sprintf(name, "/.:/idltest/matrix_%s", strtok(hostname, "."));
        else
          sprintf(name, "%s", entry_name);

	m = Matrix::bind((unsigned_char_t *) name);
	cout << "done" << endl;
	print(m);
	cout << "success!! on binding by name" << endl;
	cout << "calling invert" << endl;
	mx = m->invert();
	cout << "success!! on invert" << endl;

	cout << "trying bind by uuid" << endl;
	uuid_from_string((unsigned_char_t *) DCE_OBJECTID,
		  	  &u,
			  &status
	);
	if (status != rpc_s_ok) {
		cout << "Can't convert uuid from string" << endl;
	}

	m = Matrix::bind(u);
	cout << "done" << endl;
	print(m);
	m->set(1, 1, 33);
	print(m);

	cout << "creating new matrix from client" << endl;
	cout << "calling new2x2" << endl;
	d1 = 1;
	d2 = 2;
	d3 = 3;
	d4 = 4;
	m = Matrix::new2x2(d1, d2, d3, d4);
	cout << " built m, now printing" << endl;
	print(m);
	cout << "success!! on dynamic creator" << endl;
	cout << "calling set from client" << endl;
	m->set(1, 1, 99);
	print(m);

	cout << "calling new3x3" << endl;
	m = Matrix::new3x3(1, 2, 3,
			       4, 5, 6,
	       		7, 8, 9);
	print(m);
	cout << "success!! on dynamic creator for 3x3" << endl;

	Matrix         *m2, *m3, *m4;

	cout << "Creating Matrix *m = new matrixImpl(1,0,0,1)" << endl;
	m2 = new matrixImpl(1, 0, 0, 1);

	//Utilizing automatic binding, a
	// server is found and a 2
	// dimensional matrix is created.
	cout << "calling m3 = m->add(m2) " << endl;
	print(m2);
	m3 = m->add(m2);
	print(m3);

	cout << "calling m3 = m->add2(m2, &m3) " << endl;
	print(m);
	print(m2);
	m->add2(m2, &m3);
	print(m3);

	cout << "calling m4 = m->addArray(array) " << endl;
	print(m2);
	cout << "    +" << endl;
	print(m3);
	myarr[0] = m2;
	myarr[1] = m3;
	m4 = m->addArray(myarr);
	cout << "    =" << endl;
	print(m4);

	cout << "calling m3 = m->addStruct1(struct1) " << endl;
	print(m);
	cout << "    +" << endl;
	print(m2);
	cout << "    5" << endl;
	struct1.m = m2;
	struct1.x = 5;
	m3 = m->addStruct1(struct1);
	cout << "    =" << endl;
	print(m3);

	cout << "calling m4 = m->addUnion1(Union1) with added = 5 " << endl;
	print(m);
	cout << "    +" << endl;
	cout << "    5" << endl;
	union1.s = 0;
	union1.unionPart.x = 5;
	m4 = m->addUnion1(union1);
	cout << "    =" << endl;
	print(m4);

	cout << "calling m4 = m->addUnion1(Union1)  with added = m2" << endl;
	print(m);
	cout << "    +" << endl;
	print(m2);
	union1.s = 1;
	union1.unionPart.m = m2;
	m4 = m->addUnion1(union1);
	cout << "    =" << endl;
	print(m4);

	cout << "calling m4 = m->addUnion1(Union1)  with added = 0" << endl;
	print(m);
	cout << "    + 0" << endl;
	union1.s = 1;
	union1.unionPart.m = 0;
	m4 = m->addUnion1(union1);
	cout << "    =" << endl;
	if (m4 == 0)
		cout << "result is null" << endl;
	else {
		cout << "test failed!" << endl;
		exit(1);
	}


	cout << "calling m4 = m->addStruct2(Struct2)" << endl;
	print(m);
	cout << "    +" << endl;
	cout << "    3 + 7"  << endl;
	struct2.x = &z;
	struct2.y = 7;
	z = 3;
	m4 = m->addStruct2(struct2);
	cout << "    =" << endl;
	print(m4);

	cout << "calling m3->sum(&m2, 5, 35, &z ) " << endl;
	x = 5;
	m3->sum(&m2, &x, 35, &z);
	print(m3);
	print(m2);
	cout << "z is " << z << endl;

	cout << "Createing m2 = Matrix::new2x2(2,3,4,5)" << endl;
        d1 = 2; d2 = 3; d3 = 4; d4 = 5;
	m2 = Matrix::new2x2(d1, d2, d3, d4);
	//Utilizing automatic binding, a
	// server is found and a 2
	// dimensional matrix is created.

	 cout << "Calling m3 = m->add(m2)" << endl;
	m3 = m->add(m2);

	print(m2);

	//Invert(executes on the compute server)
	Matrix * inverted_m = m2->invert();

	//Another remote matrix l
        d1 = 2; d2 = 7; d3 = 3; d4 = 2;
	Matrix * l = Matrix::new2x2(d1, d2, d4, d4);

	//Add inverted_m to l and return sum(executes locally)
	 Matrix * sum = l->add(inverted_m);

	//Perform the multiplication of sum and m(executes on server)
	 Matrix * result = m->multiply(sum);

	//Display result
	 print(result);
	return 0;
}

void
usage(char *name)
{
	cout << "usage: " << name << " -h" << endl;
}

// for debugging - can call this from the debugger
void
PrintBindingHandle(void * b)
{
        unsigned_char_t * str;
        unsigned32 st;

        rpc_binding_to_string_binding(
                (rpc_binding_handle_t) b,
                &str,
                &st);
        cout << "str: " << (char *) str << endl;
}
