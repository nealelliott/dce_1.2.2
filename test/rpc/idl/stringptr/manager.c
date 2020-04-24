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
 * Revision 1.1.6.2  1996/02/17  23:17:38  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:36:44  marty]
 *
 * Revision 1.1.6.1  1995/12/11  19:30:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:16:02  root]
 * 
 * Revision 1.1.4.1  1994/02/01  22:00:28  cbrooks
 * 	OT 9858 - add dce.h to manager.c files
 * 	[1994/02/01  21:59:20  cbrooks]
 * 
 * Revision 1.1.2.2  1993/01/07  23:38:36  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:45:10  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:28:26  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <dce/dce.h>
#include <stdio.h>
#include <stringptr.h>
#include <test_common.h>

static ndr_boolean check_string
#ifdef _DCE_PROTO_
(ndr_char *sp, ndr_long_int cs)
#else
(sp, cs)
ndr_char *sp;
ndr_long_int cs;
#endif
{
    for (; *sp != '\000'; sp++)
        cs -= *sp;
    return (cs == 0);
}

static void set_string
#ifdef _DCE_PROTO_
(ndr_char *sp, ndr_long_int *cs, ndr_long_int maxlen)
#else
(sp, cs, maxlen)
ndr_char *sp;
ndr_long_int *cs;
ndr_long_int maxlen;
#endif
{
    ndr_long_int len, i;


    len = (maxlen == 0) ? 0 : (random() % maxlen);
    *cs = 0;
    for (i=0; i<len; i++)
        *cs += *sp++ = 'a' + random()%26;
    *sp = '\000';
}

static ndr_boolean iref_to_cstring_m
#ifdef _DCE_PROTO_
(handle_t h, cstring_t *sp, ndr_long_int checksum)
#else
(h, sp, checksum)
handle_t h;
cstring_t *sp;
ndr_long_int checksum;
#endif
{
    int i;
    ndr_boolean answer;

    if (!(answer = check_string((ndr_char *) *sp, checksum)))
        printf ("***FAIL***: in iref_to_cstring_m, checksum(%d) didn't match\n", checksum);

    return (answer);
}

static ndr_boolean ioref_to_cstring_m
#ifdef _DCE_PROTO_
(handle_t h, cstring_t *sp, ndr_long_int *checksum)
#else
(h, sp, checksum)
handle_t h;
cstring_t *sp;
ndr_long_int *checksum;
#endif
{
    int i;
    ndr_long_int cs2 = 0;
    ndr_boolean answer;

    if (!(answer = check_string((ndr_char *) *sp, *checksum)))
        printf ("***FAIL***: in ioref_to_cstring_m, cs2 (%d) != *checksum(%d)\n", cs2, *checksum);
    else 
        set_string ((ndr_char *) sp, checksum, strlen(sp));

    return (answer);
}

static ndr_boolean iptr_to_cstring_m
#ifdef _DCE_PROTO_
(handle_t h, cstring_t *sp, ndr_long_int checksum)
#else
(h, sp, checksum)
handle_t h;
cstring_t *sp;
ndr_long_int checksum;
#endif
{
    ndr_boolean answer;

    if (sp == NULL)
        return checksum == 0;
    else
        if (!(answer = check_string((ndr_char *)*sp, checksum)))
            printf ("***FAIL***: in iptr_to_cstring_m, !check_stringcs\n");

    return answer;
}

static ndr_boolean ioptr_to_cstring_m
#ifdef _DCE_PROTO_
(handle_t h, cstring_t *sp, ndr_long_int *checksum)
#else
(h, sp, checksum)
handle_t h;
cstring_t *sp;
ndr_long_int *checksum;
#endif
{
    ndr_boolean answer;

    if (sp == NULL)
        return checksum == 0;
    else
        if (!(answer = check_string((ndr_char *)*sp, *checksum)))
            printf ("***FAIL***: in ioptr_to_cstring_m, !check_stringcs\n");
        else
            set_string ((ndr_char *) sp, checksum, strlen(sp));

    return answer;
}

static ndr_boolean iref_to_fstring_m
#ifdef _DCE_PROTO_
(handle_t h, fstring_t *sp, ndr_long_int checksum)
#else
(h, sp, checksum)
handle_t h;
fstring_t *sp;
ndr_long_int checksum;
#endif
{
    ndr_boolean answer;

    if (!(answer = check_string((ndr_char *) sp, checksum)))
        printf ("***FAIL***: in iref_to_fstring_m, !check_string\n");

    return (answer);
}

static ndr_boolean ioref_to_fstring_m
#ifdef _DCE_PROTO_
(handle_t h, fstring_t *sp, ndr_long_int *checksum)
#else
(h, sp, checksum)
handle_t h;
fstring_t *sp;
ndr_long_int *checksum;
#endif
{
    ndr_boolean answer;

    if (!(answer = check_string((ndr_char *) sp, *checksum)))
        printf ("***FAIL***: in iref_to_fstring_m, !check_string\n");
    else
        set_string ((ndr_char *)sp, checksum, strlen(sp));

    return (answer);
}

static ndr_boolean iptr_to_fstring_m
#ifdef _DCE_PROTO_
(handle_t h, fstring_t *sp, ndr_long_int checksum)
#else
(h, sp, checksum)
handle_t h;
fstring_t *sp;
ndr_long_int checksum;
#endif
{
    ndr_boolean answer;

    if (sp == NULL)
        return checksum == 0;
    else
        if (!(answer = check_string((ndr_char *)sp, checksum)))
            printf ("***FAIL***: in iptr_to_fstring_m, !check_string\n");

    return (answer);
}

static ndr_boolean ioptr_to_fstring_m
#ifdef _DCE_PROTO_
(handle_t h, fstring_t *sp, ndr_long_int *checksum)
#else
(h, sp, checksum)
handle_t h;
fstring_t *sp;
ndr_long_int *checksum;
#endif
{
    ndr_boolean answer;

    if (sp == NULL)
        return checksum == 0;
    else
        if (!(answer = check_string((ndr_char *)sp, *checksum)))
            printf ("***FAIL***: in iptr_to_fstring_m, !check_string\n");
        else
            set_string ((ndr_char *)sp, checksum, strlen(sp));

    return (answer);
}

static ndr_boolean stringified_iref_m
#ifdef _DCE_PROTO_
(handle_t h, ndr_char *cp, ndr_long_int checksum)
#else
(h, cp, checksum)
handle_t h;
ndr_long_int checksum;
#endif
{
}

static ndr_boolean stringified_null_iptr_m
#ifdef _DCE_PROTO_
(handle_t h, ndr_char *cp, ndr_long_int checksum)
#else
(h, cp, checksum)
handle_t h;
ndr_long_int checksum;
#endif
{
}

static ndr_boolean stringified_iptr_m
#ifdef _DCE_PROTO_
(handle_t h, ndr_char *cp, ndr_long_int checksum)
#else
(h, cp, checksum)
handle_t h;
ndr_long_int checksum;
#endif
{
}

globaldef stringptr_v0_0_epv_t stringptr_v0_0_m_epv = {
    iref_to_cstring_m,
    ioref_to_cstring_m,
    iptr_to_cstring_m,
    ioptr_to_cstring_m,
    iref_to_fstring_m,
    ioref_to_fstring_m,
    iptr_to_fstring_m,
    ioptr_to_fstring_m,
    stringified_iref_m,
    stringified_null_iptr_m,
    stringified_iptr_m
};
