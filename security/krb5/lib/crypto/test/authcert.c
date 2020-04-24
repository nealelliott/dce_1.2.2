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
 * $Log: authcert.c,v $
 * Revision 1.1.2.2  1996/03/09  20:45:37  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:25:03  marty]
 *
 * Revision 1.1.2.1  1995/12/08  17:50:28  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/08/21  17:36 UTC  sommerfeld
 * 	crypto regression test.
 * 	[1995/08/17  21:50 UTC  sommerfeld  /main/sommerfeld_mothpatch/1]
 * 
 * $EndLog$
 */

/*
 * This file, and the certdata file, were shamelessly stolen from xntp,
 * from whence it was stolen from from Phil Karn''s DES implementation.
 *
 * [1995/07/31  13:17:39  sommerfeld]
 * Ported to the DES implementation used in DCE 
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
    mit_des_cblock key, plain, cipher, answer;
    int i;
    int test;
    int fail;
    int anyfail;
    int code;

    anyfail=0;
    for(test=0;!feof(stdin);test++){
	if (get8(key) == EOF) break;
	if (get8(plain) == EOF) break;
	if (get8(answer) == EOF) break;

	printf(" K: "); put8(key);

	code = make_key_sched(key, keysched);
	if (code != 0) 
	{
	    printf(" FAILED: %d\n", code);
	}

	printf(" P: "); put8(plain);

	printf(" C: "); put8(answer);


	mit_des_ecb_encrypt (plain, cipher,
			     keysched, MIT_DES_ENCRYPT);

	for(i=0;i<8;i++)
	    if(cipher[i] != answer[i])
		break;
	fail = 0;
	if(i != 8){
	    printf(" Encrypt FAIL ");
	    put8(cipher);
	    fail++;
	}
	mit_des_ecb_encrypt (cipher, cipher,
			     keysched, MIT_DES_DECRYPT);
	for(i=0;i<8;i++)
	    if(cipher[i] != plain[i])
		break;
	if(i != 8){
	    printf(" Decrypt FAIL ");
	    put8(cipher);
	    fail++;
	}
	if(fail == 0)
	    printf(" OK");
	anyfail += fail;
	printf("\n");
    }
    if (anyfail != 0) 
    {
	printf("authcert FAILED with %d failures\n", anyfail);
	return 1;
    } else {
	printf("authcert PASSED: no errors\n");
	return 0;
    }
}
