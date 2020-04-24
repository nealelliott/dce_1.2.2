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
 * Revision 1.1.4.2  1996/02/17  23:14:16  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:45  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:06:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:31:35  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:57:18  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:33:58  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:24:48  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <test_common.h>
#include <fpe.h>

#ifdef VMS
extern LIB$FLT_UNDER(),VAXC$ESTABLISH(),LIB$SIM_TRAP();
#define ENABLE_EXCEPTIONS \
    LIB$FLT_UNDER(&1);VAXC$ESTABLISH(LIB$SIM_TRAP)
#else
#define ENABLE_EXCEPTIONS 
#endif


error_status_t _setup
#ifdef NIDL_PROTOTYPES
(
    handle_t h
)
#else
(h)
    handle_t h;
#endif
{
    return error_status_ok;
}

void _float_overflow
#ifdef NIDL_PROTOTYPES
(
    handle_t h,
    ndr_short_float *a,
    ndr_short_float *b
)
#else
(h, a, b)
    handle_t h;
    ndr_short_float *a, *b;
#endif
{
    int i = 100;

    ENABLE_EXCEPTIONS;
    while (i--)
        *a *= *b;

#ifdef DEBUG
    printf("Warning: No hardware float overflow, generating exception\n");
#endif
    RAISE(exc_e_fltovf);
}

void _float_underflow
#ifdef NIDL_PROTOTYPES
(
    handle_t h,
    ndr_short_float *a,
    ndr_short_float *b
)
#else
(h, a, b)
    handle_t h;
    ndr_short_float *a, *b;
#endif
{
    int i = 100;

    ENABLE_EXCEPTIONS;
    while (i--)
        *a *= *b;

#ifdef DEBUG
    printf("Warning: No hardware float underflow, generating exception\n");
#endif
    RAISE(exc_e_fltund);
}

void _float_divbyzero
#ifdef NIDL_PROTOTYPES
(
    handle_t h,
    ndr_short_float *a,
    ndr_short_float *b
)
#else
(h, a, b)
    handle_t h;
    ndr_short_float *a, *b;
#endif
{
    ENABLE_EXCEPTIONS;
    *a /= *b;

#ifdef DEBUG
    printf("Warning: No hardware float divide by zero, generating exception\n");
#endif
    RAISE(exc_e_fltdiv);
}

void _double_overflow
#ifdef NIDL_PROTOTYPES
(
    handle_t h,
    ndr_long_float *a,
    ndr_long_float *b
)
#else
(h, a, b)
    handle_t h;
    ndr_long_float *a, *b;
#endif
{
    int i = 100;

    ENABLE_EXCEPTIONS;
    while (i--)
        *a *= *b;

#ifdef DEBUG
    printf("Warning: No hardware double overflow, generating exception\n");
#endif
    RAISE(exc_e_fltovf);
}

void _double_underflow
#ifdef NIDL_PROTOTYPES
(
    handle_t h,
    ndr_long_float *a,
    ndr_long_float *b
)
#else
(h, a, b)
    handle_t h;
    ndr_long_float *a, *b;
#endif
{
    int i = 100;

    ENABLE_EXCEPTIONS;
    while (i--)
        *a *= *b;

#ifdef DEBUG
    printf("Warning: No hardware float underflow, generating exception\n");
#endif
    RAISE(exc_e_fltund);
}

void _double_divbyzero
#ifdef NIDL_PROTOTYPES
(
    handle_t h,
    ndr_long_float *a,
    ndr_long_float *b
)
#else
(h, a, b)
    handle_t h;
    ndr_long_float *a, *b;
#endif
{
    ENABLE_EXCEPTIONS;
    *a /= *b;

#ifdef DEBUG
    printf("Warning: No hardware double divide by zero, generating exception\n");
#endif
    RAISE(exc_e_fltdiv);
}

globaldef fpe_v0_0_epv_t fpe_v0_0_m_epv = {
    _setup,
    _float_overflow, _float_underflow, _float_divbyzero,
    _double_overflow, _double_underflow, _double_divbyzero
};
