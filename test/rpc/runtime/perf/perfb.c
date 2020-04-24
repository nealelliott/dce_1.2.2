/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: perfb.c,v $
 * Revision 1.1.6.2  1996/02/17  23:23:51  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:42:17  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:15:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:48:01  root]
 * 
 * Revision 1.1.4.2  1993/01/13  19:56:57  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:24:53  bbelch]
 * 
 * Revision 1.1.2.2  1992/05/01  15:41:19  rsalz
 * 	 05-feb-92 mishkin   Add perfb_null & perfb_null_idem
 * 	 22-jul-91 mishkin   eliminate use of globaldef
 * 	[1992/05/01  01:21:10  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:41:30  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME
**
**      perfb.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  Server manager routines for performance and system execiser auxiliary
**  interface.  This interface is dynamically registered by the server when
**  request by the client through a call to an operation in the "perf"
**  interface.
**
**
*/

#include <perf_c.h>
#include <perf_p.h>


perfb_v1_0_epv_t perfb_mgr_epv =
{
    perfb_init,
    perfb_in,
    perfb_brd,
    perfb_null,
    perfb_null_idem
};


/***************************************************************************/

void perfb_init 
#ifdef IDL_PROTOTYPES
(
    handle_t                h,
    idl_char                *name
)
#else
    (h, name)
    handle_t                h;
    idl_char                *name;
#endif
{
    print_binding_info ("perfb_init", h);
    gethostname(name, 256);
}

/***************************************************************************/

void perfb_in 
#ifdef IDL_PROTOTYPES
(
    handle_t                h,
    perf_data_t             d,
    unsigned long           l,
    idl_boolean             verify,
    unsigned long           *sum
)
#else
    (h, d, l, verify, sum)
    handle_t                h;
    perf_data_t             d;
    unsigned long           l;
    idl_boolean             verify;
    unsigned long           *sum;
#endif
{
    print_binding_info ("perfb_in", h);
    perf_in(h, d, l, verify, sum);
}

/***************************************************************************/

void perfb_brd 
#ifdef IDL_PROTOTYPES
(
    handle_t                h,
    idl_char                *name
)
#else
    (h, name)
    handle_t                h;
    idl_char                *name;
#endif
{
    print_binding_info ("perfb_brd", h);
    gethostname(name, 256);
}

/***************************************************************************/

void perfb_null
#ifdef IDL_PROTOTYPES
(
    handle_t                h
)
#else
    (h)
    handle_t                h;
#endif
{
}

/***************************************************************************/

void perfb_null_idem
#ifdef IDL_PROTOTYPES
(
    handle_t                h
)
#else
    (h)
    handle_t                h;
#endif
{
}


