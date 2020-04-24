/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: pkcommon.c,v $
 * Revision 1.1.2.1  1996/10/03  20:37:27  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/03  19:45:50  arvind]
 *
 * 	OSF DCE 1.2.2 Drop 4
 *
 * Revision /main/DCE_1.2.2/1  1996/09/26  23:32 UTC  sommerfeld
 * 	simple hex dump/load for tests
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <stdlib.h>
#include <dce/sec_pk_base.h>
#include <dce/sec_pk.h>

#include "pkcommon.h"

void test_dump_data(sec_pk_data_t *d)
{
    int i;
    printf("(%x) ", (int)d->len);
    for (i=0; i<d->len; i++) {
	printf(" %02x", d->data[i]);
	if ((i%24) == 23)
	    printf("\n   ");
    }
    printf("\n");
}

/* this is a test.  were this a real program, we wouldn't use scanf.. */

int test_load_data(sec_pk_data_t *dload)
{
    int len;
    int i;
    unsigned char *buf;
    
    dload->len = 0;
    dload->data = NULL;

    if (scanf(" (%x)", &len) != 1) {
	printf("Bad len");
	return -1;
    }
    

    if ((buf = malloc(len)) == NULL)
	return -1;

    for (i=0; i<len; i++) {
	unsigned int n;
	if (scanf("%x", &n) != 1) {
	    printf("Bad data idx %d\n", i);
	    free(buf);
	    return -1;
	}
	buf[i] = n;
    }
    
    dload->len = len;
    dload->data = buf;
    return 0;
}

