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
 * Revision 1.1.4.2  1996/09/10  18:21:07  bfc
 * 	finish TETizing and cleanup
 * 	[1996/09/10  18:16:01  bfc]
 *
 * Revision 1.1.4.1  1996/09/09  20:59:11  bfc
 * 	finish TETizing and cleanup
 * 	[1996/09/09  20:42:27  bfc]
 * 
 * Revision 1.1.2.2  1996/03/09  20:49:13  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:27:39  marty]
 * 
 * Revision 1.1.2.1  1995/12/11  19:34:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:18:19  root]
 * 
 * $EndLog$
 */

#ifdef TET
#define TET_ENV
#define main loc_main
#endif

#include "test_com.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dce/dce_error.h>
#include "objectid.h"
#include "matrix.h"

void print(Matrix *m) {
    printf("    [%2.2f,%2.2f]\n", Matrix_get(m,0,0),  Matrix_get(m,0,1));
    printf("    [%2.2f,%2.2f]\n", Matrix_get(m,1,0),  Matrix_get(m,1,1));
}

#ifndef TRUE
#define TRUE 1
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
	Matrix		*m;
	Matrix		*mx;
	unsigned_char_t *string_binding;
	rpc_binding_handle_t binding_handle;
	unsigned32      status;
	uuid_t          u;
	int             c, errflg = 0;
	int             use_bh = TRUE;
	FILE           *fp;
	char            buf[100], *s;
	char           *prot, *host, *endp;
	idl_long_int	x,z;
	int		y;
	char            name[100];
	char            hostname[100];
        unsigned_char_t *entry_name;
	unsigned long	ref;
	char		xch;

	READ_BINDING_FROM_FILE(&binding_handle);

	uuid_from_string((unsigned_char_t *) DCE_OBJECTID, &u, &status);
	CHECK_STATUS(status, "Can't convert uuid from string:", ABORT);

	rpc_binding_set_object(binding_handle, &u, &status);
	CHECK_STATUS(status, "Can't set object id into binding handle:", ABORT);

	printf("binding to matrix by handle\n");
	m = Matrix_bind_by_hndl(binding_handle);
	printf("done\n");
	print(m);

	uuid_from_string((unsigned_char_t *) DCE_OBJECTID, &u, &status);
	CHECK_STATUS(status, "Can't convert uuid from string:", ABORT);

	printf("binding to matrix by uuid\n");
	m = Matrix_bind_by_uuid(&u);
	printf("done\n");
	print(m);

	gethostname(hostname, sizeof(hostname));

        entry_name = (unsigned_char_t *)getenv("RPC_DEFAULT_ENTRY");
        if (entry_name == NULL)
          sprintf(name, "/.:/idltest/matrixc_%s", strtok(hostname, "."));
        else
          sprintf(name, "%s", entry_name);

	printf("binding to matrix by name\n");
	m = Matrix_bind_by_name((unsigned_char_t *) name);
	printf("done\n");
	print(m);
	printf("success!! on binding by name\n");
	printf("calling invert\n");
	mx = Matrix_invert(m);
	printf("success!! on invert\n");

	printf("trying bind by uuid\n");
	uuid_from_string((unsigned_char_t *) DCE_OBJECTID, &u, &status);
	CHECK_STATUS(status, "Can't convert uuid from string:", ABORT);

	m = Matrix_bind_by_uuid(&u);
	printf("done\n");
	print(m);
	Matrix_set(m, 1, 1, 33);
	print(m);

	printf("creating new matrix from client directly\n");
	printf("calling Matrix_new2x2\n");
	m = Matrix_new2x2(1, 2, 3, 4);
	printf(" built m, now printing\n");;
	print(m);
	printf("success!! on dynamic creator\n");
	printf("calling set from client\n");
	Matrix_set(m, 1, 1, 99);
	print(m);

	printf("binding by interface\n");
	printf("calling Matrix_bind_by_iface\n");
	mx = Matrix_bind_by_iface(m);
	printf(" built mx, now printing\n");;
	print(mx);
	printf("success!!\n");
	printf("calling set from client\n");
	Matrix_set(mx, 1, 1, 763);
	print(mx);

	printf("testing static function access\n");
	printf("calling staticFunction\n");
	Matrix_staticFunction(17, 28, &z);
	printf("17 + 28 = %d\n", z);

	printf("reference count: %ld\n", ref);

	printf("Done - bye\n");

	return 0;
}

/* for debugging - can call this from the debugger */
void
PrintBindingHandle(void * b)
{
        unsigned_char_t * str;
        unsigned32 st;

        rpc_binding_to_string_binding(
                (rpc_binding_handle_t) b,
                &str,
                &st);
        printf("str: %s\n", (char *) str);
}
