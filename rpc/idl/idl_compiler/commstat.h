/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: commstat.h,v $
 * Revision 1.1.8.2  1996/02/18  23:45:02  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:44:00  marty]
 *
 * Revision 1.1.8.1  1995/12/07  22:15:05  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:10:32  root]
 * 
 * Revision 1.1.4.2  1993/07/07  19:56:54  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:30:38  ganni]
 * 
 * $EndLog$
 */
/*
**
**  Copyright (c) 1990, 1993 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**  Digital Equipment Corporation, Maynard, Mass.
**  All Rights Reserved.  Unpublished rights reserved
**  under the copyright laws of the United States.
**
**  The software contained on this media is proprietary
**  to and embodies the confidential technology of
**  Digital Equipment Corporation.  Possession, use,
**  duplication or dissemination of the software and
**  media is authorized only pursuant to a valid written
**  license from Digital Equipment Corporation.
**
**  RESTRICTED RIGHTS LEGEND   Use, duplication, or
**  disclosure by the U.S. Government is subject to
**  restrictions as set forth in Subparagraph (c)(1)(ii)
**  of DFARS 252.227-7013, or in FAR 52.227-19, as
**  applicable.
**
**
**  NAME:
**
**      commstat.h
**
**  FACILITY:
**
**      Interface Definition Language (IDL) Compiler
**
**  ABSTRACT:
**
**      Data types and function prototypes for commstat.c
**
**  VERSION: DCE 1.0
**
*/

#ifndef COMMSTAT_H
#define COMMSTAT_H

typedef enum {BE_stat_param_k, BE_stat_result_k, BE_stat_addl_k,
              BE_stat_except_k} BE_stat_k_t;

typedef struct BE_stat_info_t {
    BE_stat_k_t type;
    NAMETABLE_id_t name;
} BE_stat_info_t;

void BE_get_comm_stat_info
(
#ifdef PROTO
    AST_operation_n_t *p_operation,
    BE_stat_info_t *p_comm_stat_info
#endif
);

void BE_get_fault_stat_info
(
#ifdef PROTO
    AST_operation_n_t *p_operation,
    BE_stat_info_t *p_fault_stat_info
#endif
);

void CSPELL_receive_fault
(
#ifdef PROTO
    FILE *fid
#endif
);

void CSPELL_return_status
(
#ifdef PROTO
    FILE *fid,
    BE_stat_info_t *p_comm_stat_info,
    BE_stat_info_t *p_fault_stat_info,
    char *status_var_name,
    char *result_param_name,
    int num_user_exceptions,
    char *IDL_msp_name
#endif
);

#endif

