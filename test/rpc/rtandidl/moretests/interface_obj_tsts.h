/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: interface_obj_tsts.h,v $
 * Revision 1.1.4.2  1996/02/18  23:06:23  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:35  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:05:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:43:26  root]
 * 
 * Revision 1.1.2.2  1993/01/11  22:14:52  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:06:59  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:38:43  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**  %a%copyright(,**  )
**
**  NAME
**
**      interface_obj_tsts.h
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**  
**  This module contains definitions and preinitialized
**  objects used by test routines comtest_if_inq_id and
**  comtest_if_id_vector_free.
**
**
**  %a%private_begin
**
**  MODIFICATION HISTORY:
**
**   5-Nov-90     burton   add support for routine
**                         comtest_if_assemb_args
**  15-oct-90     burton   original
**
**  %a%private_end  
**
*/

#include <dce/stubbase.h>

/*
 * COMTST_endpoints,  is a preinitialized endpoint vector
 * containing three supported network adress families.  Its
 * address is used as a default component by the preinitialized
 * interface specification, COMTST_ifspec, which is used for
 * default test data by test routine comtest_if_inq_id.
 */

static rpc_endpoint_vector_elt_t COMTST_endpoints[3] = 

{
   (unsigned_char_p_t)"ncacn_ip_tcp", (unsigned_char_p_t)"2002",
   (unsigned_char_p_t)"ncadg_ip_udp", (unsigned_char_p_t)"2002",
   (unsigned_char_p_t)"ncadg_dnet_nsp", (unsigned_char_p_t)"dntst"
};

/*
 * COMTST_stx_id, is a preinitialized syntax representation.
 * Its address is used as a default component by the preinitialized
 * interface specification, COMTST_ifspec, which is used for
 * default test data by test routine comtest_if_inq_id.
 */

static rpc_syntax_id_t COMTST_stx_id =

{
   {0x8a885d04, 0x1ceb, 0x11c9, 0x9f, 0xe8, {0x8, 0x0, 0x2b, 0x10, 0x48, 0x60}},
   2
};

/*
 *  COMTST_ifspec is the address passed in the interface
 *  handle, 'comtest_ifspec' to test routine, 'comtest_if_inq_id'
 *  which will extract the 'id' field.  All the data initialized
 *  here serves as a default test case.  Each data field,
 *  ifspec rep version, op count, if version, id, and stub/rt
 *  if version, are replaced by the respective data fields
 *  passed in the input testscript file.
 */

static rpc_if_rep_t COMTST_ifspec =

{
   0,                      /* ifspec rep version */
   2,                      /* op count */
   1,                      /* if version */
   {0x8dc3af80, 0x3791,    /* id */
    0x11c9, 0xb6, 0x6b,
    {0x8, 0x0, 0x2b, 0xa,
     0x76, 0x16}},
   2,                      /* stub/rt if version */
   {3, COMTST_endpoints},  /* endpoint vector */
   {1, &COMTST_stx_id}     /* syntax vector */
};

/*
 * comtest_ifspec, serves as the ifspec handle argument passed
 * to test routine comtest_if_inq_id
 */

rpc_if_handle_t comtest_ifspec = (rpc_if_handle_t)&COMTST_ifspec;

/*
 * COMTST_const_if_id is a preinitialized interface id used
 * to initialize the dynamically allocated interface id
 * vector, used as test data by routine 'comtest_if_id_vector_free.
 */

static rpc_if_id_t COMTST_const_if_id =

{
   {0x8c863a04, 0x13ec, 0x11c9, 0x9f, 0xe8, {0x8, 0x0, 0x2b, 0x10, 0x48, 0x60}},
   2,
   3
};







