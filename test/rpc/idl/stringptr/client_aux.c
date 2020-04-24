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
 * Revision 1.1.4.2  1996/02/17  23:17:36  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:36:42  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:30:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:16:01  root]
 * 
 * Revision 1.1.2.2  1993/01/07  23:38:32  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:45:06  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:28:25  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <stringptr.h>
#include <test_common.h>

static ndr_boolean check_string
#ifdef NIDL_PROTOTYPES
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
#ifdef NIDL_PROTOTYPES
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

client_aux(h)
rpc_binding_handle_t h;
{
    int failures = 0;
    int i, cstrlen, fstrlen;
    cstring_t *cstringp;
    fstring_t fstring;
    ndr_long_int ccs, fcs;

    cstrlen = random()%MAXSTRINGLEN;
    cstringp = (cstring_t *)malloc(cstrlen + 1);
#ifdef DEBUG
    printf ("cstrlen == %d, cstringp == %lx\n", cstrlen, cstringp);
#endif

    set_string ((ndr_char *)cstringp, &ccs, cstrlen);

#ifdef DEBUG
    printf ("cstrlen == %d, strlen(*cstringp) == %d\n", cstrlen, strlen(*cstringp));
    printf ("Calling iref_to_cstring(h, %s, %d)\n", cstringp, ccs);
#endif

    if (!iref_to_cstring(h, cstringp, ccs)) {
        failures ++;
        printf ("***FAIL***: !iref_to_cstring()\n");
    }

#ifdef DEBUG
    printf ("Calling ioref_to_cstring(h, %s, %d)\n", cstringp, ccs);
#endif
    if (!ioref_to_cstring(h, cstringp, &ccs)) {
        failures ++;
        printf ("***FAIL***: !ioref_to_cstring()\n");
    } else if (!check_string ((ndr_char *)cstringp, ccs)) {
        failures ++;
        printf ("***FAIL***: !check_string() after ioref_to_cstring()\n");
    }


#ifdef DEBUG
    printf ("Calling iptr_to_cstring(h, %s, %d)\n", cstringp, ccs);
#endif
    if (!iptr_to_cstring(h, cstringp, ccs)) {
        failures ++;
        printf ("***FAIL***: !iptr_to_cstring()\n");
    }

#ifdef DEBUG
    printf ("Calling ioptr_to_cstring(h, %s, %d)\n", cstringp, ccs);
#endif
    if (!ioptr_to_cstring(h, cstringp, &ccs)) {
        failures ++;
        printf ("***FAIL***: !ioptr_to_cstring()\n");
    } else if (!check_string ((ndr_char *)cstringp, ccs)) {
        failures ++;
        printf ("***FAIL***: !check_string() after ioptr_to_cstring()\n");
    }

#ifdef DEBUG
    printf ("Calling iptr_to_cstring(h, NULL, 0)\n");
#endif
    if (!iptr_to_cstring(h, NULL, 0)) {
        failures ++;
        printf ("***FAIL***: !iptr_to_cstring(..., NULL, 0)\n");
    }

    fstrlen = random()%MAXSTRINGLEN;
    fcs = 0;
    for (i=0; i<fstrlen; i++)
        fcs += fstring[i] = 'a' + random()%26;
    fstring[fstrlen] = '\000';

#ifdef DEBUG
    printf ("fstrlen == %d, strlen(fstring) == %d\n", fstrlen, strlen(fstring));
    printf ("Calling iref_to_fstring(h, %s, %d)\n", fstring, fcs);
#endif

    if (!iref_to_fstring(h, (fstring_t *)fstring, fcs)) {
        failures ++;
        printf ("***FAIL***: !iref_to_fstring()\n");
    }

#ifdef DEBUG
    printf ("Calling iptr_to_fstring(h, %s, %d)\n", fstring, ccs);
#endif
    if (!iptr_to_fstring(h, (fstring_t *)fstring, fcs)) {
        failures ++;
        printf ("***FAIL***: !iptr_to_fstring()\n");
    }

#ifdef DEBUG
    printf ("Calling iptr_to_fstring(h, NULL, 0)\n");
#endif
    if (!iptr_to_fstring(h, NULL, 0)) {
        failures ++;
        printf ("***FAIL***: !iptr_to_fstring(..., NULL, 0)\n");
    }

#ifdef DEBUG
    printf ("Calling ioptr_to_fstring(h, %s, %d)\n", fstring, ccs);
#endif
    if (!ioptr_to_fstring(h, (fstring_t *)fstring, &fcs)) {
        failures ++;
        printf ("***FAIL***: !ioptr_to_fstring()\n");
    } else if (!check_string ((ndr_char *)fstring, fcs)) {
        failures ++;
        printf ("***FAIL***: !check_string() after ioptr_to_fstring()\n");
    }

#ifdef DEBUG
    printf ("cstrlen == %d, cstringp == %lx\n", cstrlen, cstringp);
#endif
    free (cstringp);
    return(failures);
}
