/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tsh_ns.h,v $
 * Revision 1.1.4.2  1996/02/18  23:06:50  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:23  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:12:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:45:56  root]
 * 
 * Revision 1.1.2.4  1993/01/25  19:29:41  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:46:32  bbelch]
 * 
 * Revision 1.1.2.3  1993/01/13  19:49:10  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:16:02  bbelch]
 * 
 * Revision 1.1.2.2  1992/10/12  17:51:40  cuti
 * 	Remove hardcoded protocol seq and endpoint instead using all_protoseq.
 * 	[1992/10/09  21:27:51  cuti]
 * 
 * Revision 1.1  1992/01/19  03:39:12  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
#
#
# Copyright (c) 1991 by
#    Hewlett-Packard Company, Palo Alto, Ca.
#
# Permission to use, copy, modify, and distribute this
# software and its documentation for any purpose and without
# fee is hereby granted, provided that the above copyright
# notice appear in all copies and that both that copyright
# notice and this permission notice appear in supporting
# documentation, and that the name of Hewlett-Packard Company not
# be used in advertising or publicity pertaining to distribution
# of the software without specific, written prior permission.
# Hewlett-Packard Company makes no representations about the
# suitability of this software for any purpose.  It is provided
# "as is" without express or implied warranty.
#
#
# DCE API Test
#
*/



#include <tsh_common.h>
#include <dce/rpc.h>
#include <dce/stubbase.h>
                      
#define CONFIG_FILE "/opt/dcelocal/dce_cf.db"

/* %*START*% Start the body of code that will get translated into the FILE.tshrc */

#define PROFILE_ELT_ADD           0 
#define ORDERED_PROFILE_ELT_ADD   1 
#define REPEAT_PROFILE_ELT_ADD    2 
#define PROFILE_ELT_INQ_BEGIN     3
#define PROFILE_ELT_INQ_NEXT      4
#define PROFILE_ELT_INQ_DONE      5
#define PROFILE_ELT_REMOVE        6
#define PROFILE_REMOVE_ERRORS     7
#define PROFILE_DELETE            8


#define INVALID_BINDING     100
#define INVALID_OPNUM       101
#define INVALID_IF_SPEC     102
#define INVALID_CALL_HANDLE 103
#define MEMORY_ERROR        104

#define NORMAL_ENTRY_CREATE 200
#define ENTRY_EXISTS_CREATE 201

#define NORMAL_ENTRY_DELETE 300
#define ENTRY_DOESNT_EXIST_DELETE 301

#define NS_BINDING_IMPORT_BEGIN   400
#define NS_BINDING_IMPORT_NEXT    401
#define NS_BINDING_IMPORT_DONE    402
#define BINDING_LOOKUP_BEGIN      403
#define BINDING_LOOKUP_NEXT       404
#define BINDING_LOOKUP_DONE       405
#define NS_BINDING_EXPORT         406
#define NS_BINDING_UNEXPORT       407
#define NS_BINDING_SELECT_ONE	  408
#define NS_BINDING_SELECT_ALL	  409

#define NS_ENTRY_OBJECT_INQ_BEGIN 600
#define NS_ENTRY_OBJECT_INQ_NEXT  601
#define NS_ENTRY_OBJECT_INQ_DONE  602

#define NOTHING_TO_UNEXPORT        700
#define NO_INTERFACES_EXPORTED     701
#define NOT_ALL_OBJECTS_UNEXPORTED 702
#define REMOVE_ALL                 703
#define REMOVE_BINDINGS            704
#define REMOVE_COMPAT              705
#define REMOVE_MAJOR_ONLY          706
#define REMOVE_EXACT               707
#define REMOVE_UPTO                708
#define REMOVE_OBJECTS             709
#ifdef DNS_FNAME_MAX
#undef DNS_FNAME_MAX
#endif
#define DNS_FNAME_MAX	  		 1024	/* Largest name */


#define NULL_INTERFACE             99		/* nothing below 10 */

/* %*STOP*%  Stop translating for FILE.tshrc  */

#define  tsh_c_dummy_protseq   (unsigned_char_t *) "ncadg_ip_udp"
#define  tsh_c_dummy_endpoint  (unsigned_char_t *) "2020"
#define  tsh_c_test_profile    (unsigned_char_t *) "/.:/NSTEST_DIR/profile_test"
#define  tsh_c_max_calls       1


/*  Some defines for the name space setup and cleanup routines
 */
#define  TSH_EXPORT    0x00000000
#define  TSH_UNEXPORT  0x00000001


/* Some flags for ns_binding_import and export
*/

#define MAKE_GOOD_IF_SPEC 1
#define MAKE_NULL_IF_SPEC  0

#define MAKE_GOOD_UUID_VECTOR 1
#define MAKE_NULL_UUID_VECTOR  0

#define MAKE_GOOD_BINDING_VECTOR 1
#define MAKE_NULL_BINDING_VECTOR  0

/* define a standard ndr_syntax 
static rpc_syntax_id_t  NIDL_ndr_id = 
                       {{0x8a885d04, 0x1ceb, 0x11c9, 0x9f, 0xe8, 
                       {0x8, 0x0, 0x2b, 0x10, 0x48, 0x60}}, 2};
*/
    

/* 
 *  Test specific data 
 */

#define  PROFILE_TEST_IF_ID1 "15f4c640-726c-11c9-b27a-08002b102989"
#define  PROFILE_TEST_IF_ID2 "16d9a800-726c-11c9-b27a-08002b102989"

#define  rpc_c_ns_syntax_incorrect    = 99; /* invalid name, mbr syntax */

#ifdef BS3
#define rpc_ns_profile_elt_add  OUR_ns_profile_elt_add 
#endif
