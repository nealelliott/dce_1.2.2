/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: server_diags.h,v $
 * Revision 1.1.8.1  1996/08/09  11:58:33  arvind
 * 	Merge CDS changes from DEC into DCE_1.2.2
 * 	[1996/07/30  15:37 UTC  arvind  /main/DCE_1.2.2/arvind_cds122/1]
 *
 * 	Add SVC_SUBCODE set subcode.
 * 	[1996/02/18  23:34:21  marty  1.1.6.2]
 *
 * Revision 1.1.6.2  1996/02/18  23:34:21  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:23:23  marty]
 * 
 * Revision 1.1.6.1  1995/12/08  15:18:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:43:43  root]
 * 
 * Revision 1.1.4.4  1994/08/03  19:02:40  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:59:49  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:39:30  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:12:11  devsrc]
 * 
 * Revision 1.1.4.2  1994/03/22  20:00:08  griffin
 * 	HCell BL3 support: New diagnostic command opcodes.
 * 	[1994/03/22  17:56:51  griffin]
 * 
 * Revision 1.1.4.1  1994/03/12  22:04:12  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:19:17  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:34:07  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:53:46  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:16:00  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _SERVER_DIAGS_H
#define _SERVER_DIAGS_H
/*
 * Module server_diags.h
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990-1994. ALL RIGHTS RESERVED.
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
 * AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 *
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
 *
 * Module Description
 *
 * Data structures for the server diagnostic interface.
 *
 */
#define DIAG_MAX_MESSAGE_SIZE 255

typedef enum {
    fault_op = 0,
    examine_op,
    enable_op,
    disable_op,
    load_op,
    set_op,
    show_op,
    close_op,
    mgmt_op,
    mark_op,
    signal_op,
    navigate_op,
    dump_op
} req_op_code_t;

typedef enum {
    normal_response = 1,
    end_of_responses,
    error_response
} res_code_t;


/*
 * Structure ID constants
 */

#define ATP_STRUCT      1
#define TRQ_STRUCT      2
#define TTQ_STRUCT      3
#define NCB_STRUCT      4
#define TCB_STRUCT      5
#define MUTEX_STRUCT    6
#define CV_STRUCT       7
#define THLOG_STRUCT    8
#define TSTAT_STRUCT    9
#define AOB_STRUCT     10
#define AOQ_STRUCT     11
#define AOS_STRUCT     12
#define VMSTAT_STRUCT  13
#define FIP_STRUCT     14
#define FIB_STRUCT     15
#define DHE_STRUCT     16
#define NSB_STRUCT     17
#define CHB_STRUCT     18
#define DHT_STRUCT     19
#define STM_STRUCT     20
#define MEM_STRUCT     21
#define ADV_STRUCT     22
#define PORT_STRUCT    23
#define RQST_STRUCT    24
#define RPOOL_STRUCT   25
#define BTSTAT_STRUCT  26


/*
 * Enable/Disable/Close/Set subcodes
 */
#define EVENT_SUBCODE           1
#define FAST_ASTS_SUBCODE       2
#define EMAA_LOGGING_SUBCODE    3
#define MESSAGE_LOG_SUBCODE     4
#define ACS_OVERRIDE_SUBCODE    5
#define JANITOR_INHIBIT_SUBCODE 6
#define ALT_EVENT_DB_SUBCODE    7
#define DB_VM_MONITOR_SUBCODE   8  /* OBSOLETE */
#define FORCE_CKPT_SUBCODE      9
#define FORCE_BACK_SUBCODE     10
#define RANDOMIZE_CH_SUBCODE   11
#define MEM_SWEEP_SUBCODE      12
#define MEM_FULL_ABORT_SUBCODE 13
#define INHIBIT_BACK_SUBCODE   14
#define NODE_VERIF_SUBCODE     15
#define SUPERCHUNK_SUBCODE     16
#define COMPRESSION_SUBCODE    17
#define ADV_PING_SUBCODE       18
#define DHE_STATE_SUBCODE      19
#define SVC_SUBCODE            20

/*
 * Management subcodes
 */

#define CREATE_SERVER_SUBCODE    1
#define ENABLE_SERVER_SUBCODE    2
#define DISABLE_SERVER_SUBCODE   3
#define INIT_NS_SUBCODE          4
#define CREATE_CLE_SUBCODE       5
#define ENABLE_CLE_SUBCODE       6
#define DISABLE_CLE_SUBCODE      7
#define REMOVE_CLE_SUBCODE       8
#define CLEAR_CLE_SUBCODE        9
#define DELETE_SERVER_SUBCODE   10
#define ADD_CELL_ALIAS_SUBCODE  11
#define REM_CELL_ALIAS_SUBCODE  12


/*
 * Signal subcodes 
 */

#define SIGNAL_SUBCODE           1
#define BROADCAST_SUBCODE        2

/*
 * Dump subcodes
 */
#define PERM_MEM_SUBCODE         1
#define TEMP_MEM_SUBCODE         2
#define UNK_MEM_SUBCODE          3
#define IGN_MEM_SUBCODE          4
#define ALL_MEM_SUBCODE          5
#define HISTORY_MEM_SUBCODE      6
#define AUDIT_MEM_SUBCODE        7
#define V1_DEF_FILE_SUBCODE      8
#define INV_ARG_BUFFER_SUBCODE   9

/*
 * Diag Mailbox Request/Response Structure
 */
typedef struct diag_request {
    req_op_code_t opcode;
    int           subcode;
    union {
        struct { int arg1; void *arg2; int arg3; } type_1; 
        struct { char arg1[80]; } type_2;
        struct { char arg1[51]; char arg2[51]; char arg3[51]; } type_3;
    } args;
} diag_request_t;

typedef struct diag_response {
    res_code_t  response_code;
    int         response_error;
    char        response_text[DIAG_MAX_MESSAGE_SIZE - sizeof(int) -
                              sizeof(res_code_t)];
} diag_response_t;

#endif  /* #ifndef _SERVER_DIAGS_H */
