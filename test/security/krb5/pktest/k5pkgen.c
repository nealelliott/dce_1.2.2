/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: k5pkgen.c,v $
 * Revision 1.1.2.1  1996/10/03  20:36:56  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/03  19:45:24  arvind]
 *
 * 	OSF DCE 1.2.2 Drop 4
 *
 * Revision /main/DCE_1.2.2/1  1996/09/26  23:32 UTC  sommerfeld
 * 	Quick & Dirty key generator
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <stdlib.h>
#include <dce/sec_pk.h>
#include <dce/sec_psm.h>

#include "pkcommon.h"

extern sec_pk_algorithm_id_t rsadsi; /* !!! */

int main(int argc, char **argv)
{
    sec_pk_gen_data_t seed;
    sec_pk_data_t pubkey;
    sec_pk_data_t privkey;
    error_status_t status;
    int i;
    
    seed.len = 0x42;
    seed.data = malloc(seed.len);

    strcpy((char *)seed.data,
        "Always know where your towel is.  Don't Panic!");
    for (i=0; i<3; i++) {
	status = sec_psm_gen_pub_key(&rsadsi, 384, &seed, &pubkey, &privkey);

	if (status != 0) {
	    printf("gen_pub_key failed: %lx\n", status);
	} else {
	    test_dump_data(&pubkey);
	    test_dump_data(&privkey);
	    sec_pk_data_free(&pubkey);
	    sec_pk_data_free(&privkey);	    
	}
	seed.data[i] += i+1;
    }
    sec_pk_data_free(&seed);
    return 0;
}
