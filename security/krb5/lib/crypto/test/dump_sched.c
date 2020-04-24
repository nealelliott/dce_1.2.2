/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * @HP_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: dump_sched.c,v $
 * Revision 1.1.2.2  1996/03/09  20:45:39  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:25:05  marty]
 *
 * Revision 1.1.2.1  1995/12/08  17:50:39  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/08/21  17:36 UTC  sommerfeld
 * 	Debugging aid.
 * 	[1995/08/17  21:50 UTC  sommerfeld  /main/sommerfeld_mothpatch/1]
 * 
 * $EndLog$
 */

/*
 * Dump out the guts of a key schedule.  For debugging purposes only.
 */

#include <stdio.h>
#include <sys/types.h>

#include <krb5/base-defs.h>
#include <krb5/encryption.h>
#include <krb5/des_int.h>

mit_des_key_schedule keysched;

int get8(mit_des_cblock lp)
{
    int t;
    int i;

    memset(lp,0,8);
    for(i=0;i<8;i++){
	scanf("%2x",&t);
	if(feof(stdin))
	    return EOF;
	lp[i] = t;
    }
    return 0;
}

void put8(mit_des_cblock lp)
{
    int i;

    for(i=0;i<8;i++){
	printf("%02x",lp[i]);
    }
    fflush(stdout);
}

int main(char argc, char **argv)
{
    int i,j;
    mit_des_cblock key;
    mit_des_key_schedule sched;


    while (get8(key) != EOF) 
    {
	put8(key);
	printf("\n");
	make_key_sched (key, sched);
	for (i=0; i<16; i+=4) 
	{
	    printf("   ");
	    for (j=0; j<4; j++) 
	    {
		printf(" ");
		put8(&((unsigned char *)sched)[(j+i)*8]);
	    }
	    printf("\n");	    
	}

    }
    return 0;
}

