/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tsh_uuid.h,v $
 * Revision 1.1.5.2  1996/02/18  23:06:55  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:34  marty]
 *
 * Revision 1.1.5.1  1995/12/11  20:12:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:46:02  root]
 * 
 * Revision 1.1.3.4  1993/01/20  21:12:27  bbelch
 * 	Fix crunched log
 * 	[1993/01/20  21:10:41  bbelch]
 * 
 * Revision 1.1.3.3  1993/01/13  19:49:39  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:16:21  bbelch]
 * 
 * Revision 1.1.3.2  1993/01/12  15:46:02  markar
 * 	 *      OT CR 2153 fix: uuid strings must be in lower case (per AES)
 * 	[1993/01/12  15:44:33  markar]
 * 
 * Revision 1.1  1992/01/19  03:37:57  devrcs
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


/* %*START*% Start the body of code that will get translated into the FILE.tshrc */

#define Valid_UUID            0
#define Invalid_UUID          1
#define Invalid_UUID_Format   2
#define NULL_UUID             3
#define NIL_UUID              4

#define CREATE                0
#define CREATE_NIL            1
#define EQUAL                 2
#define FROM_STRING           3
#define HASH                  4
#define IS_NIL                5
#define COMPARE               6
#define TO_STRING             7
#define MEMORY_ERROR          100

/* %*STOP*%  Stop translating for FILE.tshrc  */

#define UUID_INIT { \
                  error_status_t st; \
                  rpc_network_is_protseq_valid((unsigned_char_p_t)"ncadg_ip_udp", &st); \
                  }


unsigned16 tsh_c_valid_UUID_hash = 0x6cae;
char      *tsh_c_valid_UUID_string = "12345678-1234-1234-a234-123456789abc";
uuid_t     tsh_c_valid_UUID = 
               { 
               0x12345678,
               0x1234,
               0x1234,
               0xa2,
               0x34,
               0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc
               };
 
unsigned16 tsh_c_invalid_UUID_hash = 0;
char      *tsh_c_invalid_UUID_string = "ffffffff-ffff-ffff-ffff-ffffffffffff";
uuid_t     tsh_c_invalid_UUID =
               {
               0xffffffff,
               0xffff,
               0xffff,
               0xff,
               0xff,
               0xff, 0xff, 0xff, 0xff, 0xff, 0xff
               };

unsigned16 tsh_c_nil_UUID_hash = 0;
char      *tsh_c_nil_UUID_string = "00000000-0000-0000-0000-000000000000";
uuid_t     tsh_c_nil_UUID =
               {
               0x00000000,
               0x0000,
               0x0000,
               0x00,
               0x00,
               0x00, 0x00, 0x00, 0x00, 0x00, 0x00
               };
 
unsigned16 tsh_c_NULL_UUID_hash = 0;

char       *tsh_c_invalid_UUID_format_string = "bad-bad-bad-bad-bad-bad-bad-bad-bad";
  
