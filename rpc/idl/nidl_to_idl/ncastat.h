/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ncastat.h,v $
 * Revision 1.1.4.2  1996/02/18  23:46:06  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:44:52  marty]
 *
 * Revision 1.1.4.1  1995/12/07  22:36:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:18:20  root]
 * 
 * Revision 1.1.2.3  1993/01/03  22:15:04  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:48:14  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  19:16:00  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  01:11:23  zeliff]
 * 
 * Revision 1.1  1992/01/19  02:59:31  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#ifndef nca_status__included
#define nca_status__included
#include <idl_base.h>
#define NCASTAT_IDL_SUPPORTS_V1 ndr_$true
#define nca_status_$comm_failure 469827585
#define nca_status_$op_rng_error 469827586
#define nca_status_$unk_if 469827587
#define nca_status_$wrong_boot_time 469827590
#define nca_status_$you_crashed 469827593
#define nca_status_$proto_error 469827595
#define nca_status_$out_args_too_big 469827603
#define nca_status_$server_too_busy 469827604
#define nca_status_$string_too_long 469827605
#define nca_status_$unsupported_type 469827607
#define nca_status_$zero_divide 469762049
#define nca_status_$address_error 469762050
#define nca_status_$fp_div_zero 469762051
#define nca_status_$fp_underflow 469762052
#define nca_status_$fp_overflow 469762053
#define nca_status_$invalid_tag 469762054
#define nca_status_$invalid_bound 469762055
#define nca_status_$rpc_version_mismatch 469762056
#endif
