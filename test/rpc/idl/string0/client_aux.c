/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: client_aux.c,v $
 * Revision 1.1.4.2  1996/02/17  23:17:26  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:36:34  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:29:54  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:15:49  root]
 * 
 * Revision 1.1.2.2  1993/01/07  23:37:01  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:44:00  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:27:57  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <string0.h>
#include <test_common.h>

extern int Pass;
int client_aux (h)
handle_t h;
{
    int i, j, len, failures;
    idl_char s[A], r[A], b[B][C];
    idl_long_int checksum;
    idl_boolean failed;

    failures = 0;
    checksum = 0;
    len = random()%A - 1;
    for (i = 0; i < len; i++)
        checksum += s[i] = 'A' + random()%('z'-'A');
    
    s[len] = '\0';

#ifdef DEBUG
    printf("Calling op1 (len = %d, checksum = %d)\n", len, checksum);
#endif

    if (op1(h, s, checksum))
    {
        failures++;
        printf("op1 failed!\n");
    }

    strcpy(r, s);

#ifdef DEBUG
    printf("Calling op2 (len = %d)\n", len);
#endif
    op2(h, r);

    j = len-1; failed = false;
    for (i = 0; i < len; i++, j--)
        if (r[j] != s[i]) failed = true;

    if (failed)
    {
        failures++;
        printf("op2 failed\n");
    }

    checksum = 0;
    for (i =  0; i < B; i++)
    {
        do len = random()%C - 1; while (len < 0);
        for (j = 0; j < len; j++) 
            checksum += b[i][j] = 'A' + random()%('z'-'A');
        b[i][len] = '\0';
    }

#ifdef DEBUG
    printf("Calling op3\n");
#endif
    if (op3(h, b, checksum))
    {
        failures++;
        printf("op1 failed!\n");
    }
    return failures;
}
