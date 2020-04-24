/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tsh_string.h,v $
 * Revision 1.1.4.3  1996/02/18  23:06:53  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:30  marty]
 *
 * Revision 1.1.4.2  1995/12/13  21:19:08  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	Revision /main/jrr_1.2_mothra/1  1995/12/12  22:44 UTC  dat
 * 	Add tsh_c_valid_UUID declaration
 * 	[1995/12/13  21:18:19  root]
 * 
 * Revision 1.1.2.3  1993/01/25  19:29:50  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:46:48  bbelch]
 * 
 * Revision 1.1.2.2  1993/01/13  19:49:29  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:16:14  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:37:55  devrcs
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
# Revision 1.1  91/09/11  16:15:18  marco
# Initial revision
# 
*/



#include <tsh_common.h>
#include <pthread.h>

/* %*START*% Start the body of code that will get translated into the FILE.tshrc */

#define NORMAL_TEST                   0
#define MEMORY_ERROR                  1
#define FREE                          2
#define FREE_AGAIN                    3
#define	NULL_UUID                     4
#define NULL_PROTSEQ                  5
#define	NULL_NET_ADDR                 6
#define NULL_ENDPOINT                 7
#define NULL_OPTIONS                  8
#define USE_BINDING_COPY              9
#define USE_BINDING_DUP              10
#define SRC_HANDLE_ZERO              18
#define SRC_HANDLE_MINUS_ONE         19
#define DATA_OFFSET_ERR              20
#define PROTOCOL_ID_ERR              21
#define REFCNT_ERR                   22
#define OBJ_ERR                      23
#define RPC_ADDR_ERR                 24
#define RPC_ADDR_RPC_PROTSEQ_ID_ERR  25
#define RPC_ADDR_LEN_ERR             26
#define RPC_ADDR_SA_FAMILY_ERR       27
#define RPC_ADDR_SA_DATA_ERR         28
#define TIMEOUT_ERR                  29
#define CALLS_IN_PROGRESS_ERR        30
#define NS_SPECIFIC_ERR              31

#define    NULL_TOKEN    "<NULL>"
#define    NIL_TOKEN       "<NIL>"
/* %*STOP*%  Stop translating for FILE.tshrc  */ 

/* in #include <dce/cmalib_crtlx.h>

#ifdef OSF
  extern void *malloc(
#else
  extern unsigned_char_p_t    malloc(
#endif
#ifdef _PROTOTYPE
int sz
#endif
);   */
#define GOBBLE_MEM_SIZE    10000


uuid_t     tsh_c_valid_UUID = 
               { 
               0x12345678,
               0x1234,
               0x1234,
               0xa2,
               0x34,
               0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc
               };
