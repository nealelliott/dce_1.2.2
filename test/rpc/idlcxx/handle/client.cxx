// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// 
// 
// HISTORY
// $Log: client.cxx,v $
// Revision 1.1.4.2  1996/09/10  18:21:02  bfc
// 	remove unecessary STDC definition
// 	[1996/09/10  18:15:29  bfc]
//
// Revision 1.1.4.1  1996/09/09  20:58:58  bfc
// 	finish TETizing and cleanup
// 	[1996/09/09  20:41:35  bfc]
// 
// Revision 1.1.2.2  1996/03/09  20:48:42  marty
// 	Add OSF copyright
// 	[1996/03/09  20:27:16  marty]
// 
// Revision 1.1.2.1  1995/12/11  19:34:06  root
// 	Submit OSF/DCE 1.2.1
// 	[1995/12/11  19:17:57  root]
// 
// $EndLog$

#ifdef TET
#define TET_ENV
#define main loc_main
#endif

#include "test_com.h"
#include <iostream.h>
#include "matrix.h"

void	print(Matrix *);
void	getHandle(handle_t *);

int 
main(int argc, char *argv[])
{
	Matrix		*m;
	unsigned32	st;
	handle_t	bh;

	cout << "*** start of remote object creator handle test ***" << endl;

	cout << "testing explicit handle" << endl;

	// get the server's handle
	getHandle(&bh);		

	m = Matrix::new2x2EXPLICIT(bh, 1,2,3,4);
	print(m);

	cout << "testing implicit handle" << endl;
	m = Matrix::new2x2IMPLICIT(1,2,3,4);
	print(m);

	cout << "*** end of remote object creator test ***" << endl;

	return 0;
}

void
getHandle(handle_t *bh)
{
	unsigned32      st;

	READ_BINDING_FROM_FILE(bh);

	// initialize the implicit binding handle
	rpc_binding_copy(*bh, &global_binding_h, &st);
	CHECK_STATUS(st, "Can't copy binding handle:", ABORT);
	
	return;
}

void
print(Matrix *m) {
    cout << "    [" << m->get(0,0) << "," << m->get(0,1) << "]" << endl;
    cout << "    [" << m->get(1,0) << "," << m->get(1,1) << "]" << endl;
}

