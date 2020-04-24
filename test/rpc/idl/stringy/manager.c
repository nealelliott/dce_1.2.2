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
 * Revision 1.1.4.2  1996/02/17  23:17:45  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:36:50  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:30:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:16:09  root]
 * 
 * Revision 1.1.2.2  1993/01/07  23:39:10  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:45:39  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:28:40  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stringy.h>
#include <test_common.h>


static void reverse(s)
char *s;
{
    int i, len = strlen(s);
    char *buf = (char *)malloc(len+1);

    strcpy(buf, s);

    for (i = 0; i < len; i++)
        s[i] = buf[len-i-1];
    
    s[len] = '\0';

    free(buf);
}

static void palindrome(s)
char *s;
{
    int i, len = strlen(s);

    for (i = 0; i < len; i++)
        s[len+i-1] = s[len-i-1];
	s[2*len-1] = '\0';
}

static void op0
#ifdef NIDL_PROTOTYPES
(
    /* [in] */ handle_t h,
    /* [in, out] */ ndr_char *p
)
#else
(h, p)
    /* [in] */ handle_t h;
    /* [in, out] */ ndr_char *p;
#endif
{
    reverse(p);
}

static void op1
#ifdef NIDL_PROTOTYPES
(
    /* [in] */ handle_t h,
    /* [in, out] */ ndr_char *p,
    /* [in] */ ndr_long_int s
)
#else
(h, p, s)
    /* [in] */ handle_t h;
    /* [in, out] */ ndr_char *p;
    /* [in] */ ndr_long_int s;
#endif
{
    palindrome(p);
}

static void op2
#ifdef NIDL_PROTOTYPES
(
    /* [in] */ handle_t h,
    /* [in, out] */ ndr_char *p
)
#else
(h, p)
    /* [in] */ handle_t h;
    /* [in, out] */ ndr_char *p;
#endif
{
    reverse(p);
}

static void op3
#ifdef NIDL_PROTOTYPES
(
    /* [in] */ handle_t h,
    /* [in, out] */ ndr_char *p,
    /* [in] */ ndr_long_int s
)
#else
(h, p, s)
    /* [in] */ handle_t h;
    /* [in, out] */ ndr_char *p;
    /* [in] */ ndr_long_int s;
#endif
{
    palindrome(p);
}

static void op4
#ifdef NIDL_PROTOTYPES
(
    /* [in] */ handle_t h,
    /* [in, out] */ ndr_char a[]
)
#else
(h, a)
    /* [in] */ handle_t h;
    /* [in, out] */ ndr_char a[];
#endif
{
    reverse(a);
}

static void op5
#ifdef NIDL_PROTOTYPES
(
    /* [in] */ handle_t h,
    /* [in, out] */ ndr_char a[],
    /* [in] */ ndr_long_int s
)
#else
(h, a, s)
    /* [in] */ handle_t h;
    /* [in, out] */ ndr_char a[];
    /* [in] */ ndr_long_int s;
#endif
{
    palindrome(a);
}

globaldef stringy_v0_0_epv_t stringy_v0_0_m_epv = 
	{op0, op1, op2, op3, op4, op5};
