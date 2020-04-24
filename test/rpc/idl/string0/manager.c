/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: manager.c,v $
 * Revision 1.1.4.2  1996/02/17  23:17:28  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:36:35  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:29:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:15:50  root]
 * 
 * Revision 1.1.2.2  1993/01/07  23:37:05  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:44:04  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:27:58  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <string0.h>
#include <test_common.h>

static idl_small_int op1
#ifdef NIDL_PROTOTYPES
(
    handle_t h,
    idl_char s[A],
    idl_long_int checksum
)
#else
(h,s, checksum)
    handle_t h;
    idl_char s[A];
    idl_long_int checksum;
#endif
{
    idl_long_int check = 0;
    int i, len = strlen(s);

#ifdef DEBUG
    printf("In op1: strlen(s) = %d\n", len);
#endif

    for (i = 0; i < len; i++)
        check += s[i];

    if (check == checksum) 
    {
#ifdef DEBUG
        printf("Checksum matches.\n");
#endif
        return 0;
    }
    else 
    {
        printf("Checksum doesn't match! (%d != %d)\n", check, checksum);
        return 1;
    }
}

static void op2
#ifdef NIDL_PROTOTYPES
(
    handle_t h,
    idl_char s[A]
)
#else
(h,s)
    handle_t h;
    idl_char s[A];
#endif
{
    int i, j, len = strlen(s);
    idl_char r[A];

#ifdef DEBUG
    printf("In op2: length is %d, reversing\n", len);
#endif

    j = len-1;

    for (i = 0; i < len; i++, j--)
        r[j] = s[i];
    
    r[len] = '\0';

    strcpy(s,r);
}

static idl_small_int op3
#ifdef NIDL_PROTOTYPES
(
    handle_t h,
    idl_char s[B][C],
    idl_long_int checksum
)
#else
(h, s, checksum)
    handle_t h;
    idl_char s[B][C];
    idl_long_int checksum;
#endif
{
    int i, j, len;
    idl_long_int check = 0;

#ifdef DEBUG
    printf("In op3: computing checksum;\n");
#endif

    for (i = 0; i < B; i++)
    {
        len = strlen(s[i]);
        for (j = 0; j < len; j++)
            check += s[i][j];
    }
    if (check == checksum)
    {
#ifdef DEBUG
        printf("Checksum matches.\n");
#endif
        return 0;
    }
    else
    {
        printf("Checksum doesn't match! (%d != %d)\n", check, checksum);
        return 1;
    }
}

globaldef string0_v0_0_epv_t string0_v0_0_m_epv = {op1, op2, op3};
