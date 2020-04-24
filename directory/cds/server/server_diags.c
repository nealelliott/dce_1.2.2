/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: server_diags.c,v $
 * Revision 1.1.8.2  1996/02/18  19:36:33  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:17:09  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:31:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:42  root]
 * 
 * Revision 1.1.6.7  1994/08/24  20:14:43  mccann
 * 	fix server shutdown, add annotation strings, start message delay:OT11239,11245 and 11766
 * 	[1994/08/24  20:09:05  mccann]
 * 
 * Revision 1.1.6.6  1994/08/03  19:04:31  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:27:37  mccann]
 * 
 * Revision 1.1.6.5  1994/06/30  19:18:16  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:48:54  mccann]
 * 
 * Revision 1.1.6.4  1994/06/09  18:43:44  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:16:18  devsrc]
 * 
 * Revision 1.1.6.3  1994/04/14  14:51:28  peckham
 * 	Remove CMA, VMS and UNIX conditionals.
 * 	Remove dnstables dependencies.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	[1994/04/11  20:21:42  peckham]
 * 
 * Revision 1.1.6.2  1994/03/22  20:00:32  griffin
 * 	HCell BL3 support: Hooks for add/remove cellname diagnostic support.
 * 	[1994/03/22  18:12:32  griffin]
 * 
 * Revision 1.1.6.1  1994/03/12  22:12:18  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:45:05  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  14:23:11  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:23:41  zeliff]
 * 
 * Revision 1.1.2.2  1992/06/30  19:46:30  peckham
 * 	OT#4473: wrong number of arguments to dns_cle_create().
 * 	[1992/06/30  19:45:33  peckham]
 * 
 * Revision 1.1  1992/01/19  15:25:20  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * MODULE - server_diags.c
 *
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
 *
 * Module description:
 *
 * This module provides diagnostic access to the DECdns server.
 *
 */

#include <server.h>
#include <stdio.h>
#include <clerk.h>
#include <dbdef.h>
#include <dump.h>
#include <security.h>
#include <server_diags.h>
#include <hcell.h>
#include <sys_mbx.h>

#define NORMAL_RESPONSE \
    response_buffer.response_code = normal_response; \
    response_buffer.response_error = 0; \
    (void)sys_mbx_write(mbx, (unsigned char *)&response_buffer, sizeof(response_buffer))


static sys_mailbox diag_mbx_id;

static nsgbl_ns_t *diag_ns_p = 0;

extern nsgbl_ns_t dns_nameserver;
extern dthread_attr_t diag_thread_attr;

/*
 * Local prototypes
 */

static int
emit_diag_output (
    char         *,
    sys_mailbox   );

static void
vm_statistics (
    sys_mailbox );

static int
do_examine (
    sys_mailbox    ,
    diag_request_t *);

static int
do_enable (
    sys_mailbox    ,
    diag_request_t *);

static int
do_disable (
    sys_mailbox    ,
    diag_request_t *);

static int
do_close (
    sys_mailbox    ,
    diag_request_t *);

static int
do_set (
    sys_mailbox    ,
    diag_request_t *);

static int
do_mgmt (
    sys_mailbox    ,
    diag_request_t *);

static int
do_mark (
    sys_mailbox    ,
    diag_request_t *);

static int
do_signal (
    sys_mailbox    ,
    diag_request_t *);

static int
do_navigate (
    sys_mailbox    ,
    diag_request_t *);

static int
do_dump (
    sys_mailbox    ,
    diag_request_t *);

static int
server_diag_processor (
    sys_mailbox    ,
    diag_request_t *);

/* -------------------------------------------------------------------------- */

/*
 *
 */
static int 
emit_diag_output (char         *s,
                  sys_mailbox  mbx)
{
    diag_response_t response_buffer;

    (void)sprintf(response_buffer.response_text,
            "%.132s", s);
    NORMAL_RESPONSE;
    return (0);
}



/* -------------------------------------------------------------------------- */


/*
 * Virtual memory (system-oriented) statistics
 */
static void 
vm_statistics (sys_mailbox mbx)
{
    diag_response_t response_buffer;

#if _DNS_OS_ == _DNS__VMS
    int get_vm_calls;
    int free_vm_calls;
    int vm_allocated;
    int available_vm;
    int pagfilcnt;
    int page_faults;
    int wssize;

#include <pragma_nostandard.h>
    LIB$STAT_VM(&1, &get_vm_calls);
    LIB$STAT_VM(&2, &free_vm_calls);
    LIB$STAT_VM(&3, &vm_allocated);
    LIB$GETJPI(&JPI$_FREPTECNT, 0, 0, &available_vm, 0, 0);
    LIB$GETJPI(&JPI$_PAGFILCNT, 0, 0, &pagfilcnt, 0, 0);
    LIB$GETJPI(&JPI$_PAGEFLTS,  0, 0, &page_faults, 0, 0);
    LIB$GETJPI(&JPI$_WSSIZE,    0, 0, &wssize, 0, 0);
#include <pragma_standard.h>

    (void)sprintf(response_buffer.response_text,
            "GET_VM calls: %d  FREE_VM calls: %d  VM  allocated: %d bytes",
            get_vm_calls, free_vm_calls, vm_allocated);
    NORMAL_RESPONSE;
    (void)sprintf(response_buffer.response_text,
            "FreePTECount: %d  PageFileCnt: %d  PageFaults: %d  WSSize: %d",
            available_vm, pagfilcnt, page_faults, wssize);
    NORMAL_RESPONSE;
#endif /* VMS */

    (void)sprintf(response_buffer.response_text,
            "Sorry, no system memory statistics for UNIX yet...");
    NORMAL_RESPONSE;


}


/* -------------------------------------------------------------------------- */


/*
 * Virtual memory (system-oriented) audit (to stdout)
 */

#if _DNS_OS_ == _DNS__VMS
# include <pragma_nostandard.h>

static 
vm_audit_output (struct dsc $descriptor  *output_dsc_p,
                 int                     zone)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug2,
        " >vm_audit_output"));

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug4,
        "vm_audit_output: %.*s\n", 
        (int)output_dsc_p->dsc$w_length, 
        output_dsc_p->dsc$a_pointer));

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug2,
        " <vm_audit_output"));
    return (0);
}

#endif /* VMS */

#if _DNS_OS_ == _DNS__VMS
static 
vm_audit_output (struct dsc $descriptor  *output_dsc_p,
                 int                     zone)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug2,
        " >vm_audit_output"));

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug4,
        "vm_audit_output: %.*s\n", 
        (int)output_dsc_p->dsc$w_length, 
        output_dsc_p->dsc$a_pointer));

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug2,
        " <vm_audit_output"));
    return (0);
}

#endif /* VMS */

static void 
vm_audit (void)
{
#if _DNS_OS_ == _DNS__VMS
    int current_zone = 0;
    int zone_context = 0;
    int status;

    globalvalue LIB$_NOTFOU;
#endif /* VMS */

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug4,
        "=-=-=-=-=-=-=-=-=-= Virtual Memory Audit =-=-=-=-=-=-=-=-=-="));

#if _DNS_OS_ == _DNS__VMS

    while (1) {
        status = lib$find_vm_zone(&zone_context, &current_zone);
        if (status == LIB$_NOTFOU) {

	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_gda,
                svc_c_debug4,
                "No more zones..."));

            break;
        } else if (!(status & 1)) {

	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_gda,
                svc_c_debug4,
                "find_vm_zone returned 0x%x.  Audit cancelled.", 
                status));

            break;
        }

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug4,
            "VM Audit zone: %d (0x%x)\n", 
            current_zone, 
            current_zone));

        status = lib$show_vm_zone(&current_zone, &2, 
                                  &vm_audit_output, current_zone);

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug4,
            "show_vm_zone(%d) returned: 0x%x\n", 
            current_zone, 
	    status));

        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug4,
            "--------------------------------------------------------------------\n"));

    }

# include <pragma_standard.h>
#elif _DNS_OS_ == _DNS__UNIX

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug4,
        "Sorry, no system memory audit for UNIX yet..."));

#else

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug4,
        "Sorry, no system memory audit for UNKNOWN SYSTEM yet...\n"));

#endif

}



/* -------------------------------------------------------------------------- */

/*
 *
 */
static int 
do_examine (sys_mailbox     mbx,
            diag_request_t  *request_p)
{
    diag_response_t response_buffer;

    switch (request_p->subcode) {
    case ATP_STRUCT :
        (void)dthread_diag_threads(8, request_p->args.type_1.arg1, (dthread_t *)0,
			     (dthread_mutex_t *)0, (dthread_cond_t *)0,
                             emit_diag_output, (void *)mbx);
        break;

    case TRQ_STRUCT :
        (void)dthread_diag_threads(3, request_p->args.type_1.arg1, (dthread_t *)0,
			     (dthread_mutex_t *)0, (dthread_cond_t *)0,
                             emit_diag_output, (void *)mbx);
        break;

    case TTQ_STRUCT :
        (void)dthread_diag_threads(7, request_p->args.type_1.arg1, (dthread_t *)0,
			     (dthread_mutex_t *)0, (dthread_cond_t *)0,
                             emit_diag_output, (void *)mbx);
        break;

    case THLOG_STRUCT :
        (void)dthread_diag_threads(4, request_p->args.type_1.arg1, (dthread_t *)0,
			     (dthread_mutex_t *)0, (dthread_cond_t *)0,
                             emit_diag_output, (void *)mbx);
        break;

    case TSTAT_STRUCT :
        (void)dthread_diag_threads(2, request_p->args.type_1.arg1, (dthread_t *)0,
			     (dthread_mutex_t *)0, (dthread_cond_t *)0,
                             emit_diag_output, (void *)mbx);
        break;

    case TCB_STRUCT :
        (void)dthread_diag_threads(1, request_p->args.type_1.arg1,
                             (dthread_t *)&request_p->args.type_1.arg2,
			     (dthread_mutex_t *)0, (dthread_cond_t *)0,
                             emit_diag_output, (void *)mbx);
        break;

    case MUTEX_STRUCT :
        (void)dthread_diag_threads(5, request_p->args.type_1.arg1, (dthread_t *)0,
			     (dthread_mutex_t *)&request_p->args.type_1.arg2,
			     (dthread_cond_t *)0,
			     emit_diag_output, (void *)mbx);
        break;

    case CV_STRUCT :
        (void)dthread_diag_threads(6, request_p->args.type_1.arg1,
			     (dthread_t *)0, (dthread_mutex_t *)0,
			     (dthread_cond_t *)&request_p->args.type_1.arg2,
                             emit_diag_output, (void *)mbx);
        break;

    case NCB_STRUCT :
        (void)net_diag_dump(1, request_p->args.type_1.arg1,
                      request_p->args.type_1.arg2, emit_diag_output, mbx);
        break;

    case AOB_STRUCT :
        (void)sys_diag_async_op(1, request_p->args.type_1.arg1,
                          request_p->args.type_1.arg2, emit_diag_output, mbx);
        break;

    case AOQ_STRUCT :
        (void)sys_diag_async_op(2, request_p->args.type_1.arg1,
                          request_p->args.type_1.arg2, emit_diag_output, mbx);
        break;

    case AOS_STRUCT :
        (void)sys_diag_async_op(3, request_p->args.type_1.arg1,
                          NULL, emit_diag_output, mbx);
        break;

    case VMSTAT_STRUCT :
        dns_diag_vm_tot(emit_diag_output, mbx);
        vm_statistics(mbx);
        break;

    case FIP_STRUCT :
        (void)db_diag_dump_file_info_list(request_p->args.type_1.arg1,
                                    emit_diag_output, mbx);

        break;

    case FIB_STRUCT :
        (void)db_diag_dump_file_info(request_p->args.type_1.arg1,
                               (file_info_t *)request_p->args.type_1.arg2,
                               emit_diag_output, mbx);
        break;

    case DHE_STRUCT :
        (void)db_diag_dump_dir_hash_entry(request_p->args.type_1.arg1,
                               (dir_hash_entry_t *)request_p->args.type_1.arg2,
                               emit_diag_output, mbx);
        break;


    case NSB_STRUCT :
        (void)db_diag_dump_nsgbl_ns(request_p->args.type_1.arg1,
                              &dns_nameserver,
                              emit_diag_output, mbx);
        break;

    case CHB_STRUCT :
        (void)db_diag_dump_nsgbl_ch(request_p->args.type_1.arg1,
                              (nsgbl_ch_t *)request_p->args.type_1.arg2,
                              emit_diag_output, mbx);
        break;


    case DHT_STRUCT :
        (void)db_diag_dump_dir_hash_table(request_p->args.type_1.arg1,
                               (file_info_t *)request_p->args.type_1.arg2,
                               emit_diag_output, mbx);
        break;

    case STM_STRUCT :
        (void)db_diag_dump_stream(request_p->args.type_1.arg1,
                               (db_stream_t *)request_p->args.type_1.arg2,
                               emit_diag_output, mbx);
        break;

    case MEM_STRUCT :
        dns_diag_vm_find((int)request_p->args.type_1.arg2,
                         emit_diag_output, mbx);
        break;

    case ADV_STRUCT :
        (void)adver_diag_dump_control_block(request_p->args.type_1.arg1,
            request_p->args.type_1.arg2,
            emit_diag_output, mbx);
        break;

    case PORT_STRUCT :
        (void)net_diag_dump(2, request_p->args.type_1.arg1,
                      request_p->args.type_1.arg2, emit_diag_output, mbx);
        break;

    case RQST_STRUCT :
        (void)net_diag_dump(3, request_p->args.type_1.arg1,
                      request_p->args.type_1.arg2, emit_diag_output, mbx);
        break;

    case RPOOL_STRUCT :
        (void)net_diag_dump(4, request_p->args.type_1.arg1,
                      request_p->args.type_1.arg2, emit_diag_output, mbx);
        break;

    default :
        (void)sprintf(response_buffer.response_text,
                "Structure code %d is not supported by this server_diags.",
                request_p->subcode);
        NORMAL_RESPONSE;
	return (0);
        break;
    }
    return (0);
}




/* -------------------------------------------------------------------------- */

/*
 *  Enable command processing
 */
static int 
do_enable (sys_mailbox     mbx,
           diag_request_t  *request_p)
{
    diag_response_t response_buffer;
    int prev_setting;

    switch (request_p->subcode) {
    case EVENT_SUBCODE :
        (void)sprintf(response_buffer.response_text,
                "Obsolete: %s", request_p->args.type_2.arg1);
        NORMAL_RESPONSE;
        break;
    case FAST_ASTS_SUBCODE :
        prev_setting = sys_diag_ast_speed_selector(1);
        (void)sprintf(response_buffer.response_text,
                "Enabled fast AST deliveries (prior setting was %d)", prev_setting);
        NORMAL_RESPONSE;
        break;
    case EMAA_LOGGING_SUBCODE :
        /* dns_enable_emaa_logging();*/
        (void)sprintf(response_buffer.response_text, "Enabled EMAA logging");
        NORMAL_RESPONSE;
        break;
    case MESSAGE_LOG_SUBCODE :
        prev_setting = net_diag_set_logging_state(request_p->args.type_1.arg2, 1);
        (void)sprintf(response_buffer.response_text,
                "Enabled message logging for NCB %x (prior setting was %d)",
                request_p->args.type_1.arg2, prev_setting);
        NORMAL_RESPONSE;
        break;
    case ACS_OVERRIDE_SUBCODE :
        prev_setting = security_diag_set_override(1);
        (void)sprintf(response_buffer.response_text,
                "Enabled ACS override (prior setting was %d)", prev_setting);
        NORMAL_RESPONSE;
        break;
    case JANITOR_INHIBIT_SUBCODE :
	/* WARNING - handle required now */
        prev_setting = net_diag_set_janitor_inhibit(request_p->args.type_1.arg2, 0);
        (void)sprintf(response_buffer.response_text, "Enabled idle link janitors");
        NORMAL_RESPONSE;
        break;
    case DB_VM_MONITOR_SUBCODE :
        (void)sprintf(response_buffer.response_text, "The database VM Monitor is obsolete.");
        NORMAL_RESPONSE;
        break;
    case RANDOMIZE_CH_SUBCODE :
        prev_setting = ta_diag_set_random(0);
        (void)sprintf(response_buffer.response_text, "Randomize CH enabled (prev setting was %d)", prev_setting);
        NORMAL_RESPONSE;
        break;
    case NODE_VERIF_SUBCODE :
        prev_setting = security_diag_set_node_verif(1);
        (void)sprintf(response_buffer.response_text,
                "Enabled node verification (prior setting was %d)", prev_setting);
        NORMAL_RESPONSE;
        break;
    case MEM_SWEEP_SUBCODE :
        prev_setting = dns_diag_set_memory_check(1);
        (void)sprintf(response_buffer.response_text,
                "Enabled continuous memory check (prior setting was %d)", prev_setting);
        NORMAL_RESPONSE;
        break;
    case MEM_FULL_ABORT_SUBCODE :
        prev_setting = dns_diag_set_abort_on_full_mem(1);
        (void)sprintf(response_buffer.response_text,
                "Enabled full memory abort (prior setting was %d)", prev_setting);
        NORMAL_RESPONSE;
        break;
    case SUPERCHUNK_SUBCODE :
        prev_setting = dns_diag_set_superchunk_mode(1);
        (void)sprintf(response_buffer.response_text,
                "Enabled superchunk memory mode (prior setting was %d)", prev_setting);
        NORMAL_RESPONSE;
        break;
    default :
        (void)sprintf(response_buffer.response_text,
                "server_diags: Unsupported enable subcode (%d)", request_p->subcode);
        NORMAL_RESPONSE;
        break;
    }
    return (0);
}



/* -------------------------------------------------------------------------- */

/*
 *   Disable command processing
 */
static int 
do_disable (sys_mailbox     mbx,
            diag_request_t  *request_p)
{
    diag_response_t response_buffer;
    int prev_setting;

    switch (request_p->subcode) {
    case EVENT_SUBCODE :
        (void)sprintf(response_buffer.response_text,
                "Obsolete: %s", request_p->args.type_2.arg1);
        NORMAL_RESPONSE;
        break;
    case FAST_ASTS_SUBCODE :
        prev_setting = sys_diag_ast_speed_selector(0);
        (void)sprintf(response_buffer.response_text,
                "Disabled fast AST deliveries (prior setting was %d)", prev_setting);
        NORMAL_RESPONSE;
        break;
    case EMAA_LOGGING_SUBCODE :
        /* dns_disable_emaa_logging(); */
        (void)sprintf(response_buffer.response_text, "Disabled EMAA logging");
        NORMAL_RESPONSE;
        break;
    case MESSAGE_LOG_SUBCODE :
        prev_setting = net_diag_set_logging_state(request_p->args.type_1.arg2, 0);
        (void)sprintf(response_buffer.response_text,
                "Disabled message logging for NCB %x (prior setting was %d)",
                request_p->args.type_1.arg2, prev_setting);
        NORMAL_RESPONSE;
        break;
    case ACS_OVERRIDE_SUBCODE :
        prev_setting = security_diag_set_override(0);
        (void)sprintf(response_buffer.response_text,
                "Disabled ACS override (prior setting was %d)", prev_setting);
        NORMAL_RESPONSE;
        break;
    case JANITOR_INHIBIT_SUBCODE :
	/* WARNING - handle required now */
        prev_setting = net_diag_set_janitor_inhibit(request_p->args.type_1.arg2, 1);
        (void)sprintf(response_buffer.response_text, "Disabled idle link janitors");
        NORMAL_RESPONSE;
        break;
    case DB_VM_MONITOR_SUBCODE :
        (void)sprintf(response_buffer.response_text, "The database VM monitor is obsolete");
        NORMAL_RESPONSE;
        break;
    case RANDOMIZE_CH_SUBCODE :
        prev_setting = ta_diag_set_random(1);
        (void)sprintf(response_buffer.response_text, "Randomize CH disabled (prev setting was %d)", prev_setting);
        NORMAL_RESPONSE;
        break;
    case NODE_VERIF_SUBCODE :
        prev_setting = security_diag_set_node_verif(0);
        (void)sprintf(response_buffer.response_text,
                "Disabled node verification (prior setting was %d)", prev_setting);
        NORMAL_RESPONSE;
        break;
    case MEM_SWEEP_SUBCODE :
        prev_setting = dns_diag_set_memory_check(0);
        (void)sprintf(response_buffer.response_text,
                "Disabled continuous memory check (prior setting was %d)", prev_setting);
        NORMAL_RESPONSE;
        break;
    case MEM_FULL_ABORT_SUBCODE :
        prev_setting = dns_diag_set_abort_on_full_mem(0);
        (void)sprintf(response_buffer.response_text,
                "Disabled full memory abort (prior setting was %d)", prev_setting);
        NORMAL_RESPONSE;
        break;
    case SUPERCHUNK_SUBCODE :
        prev_setting = dns_diag_set_superchunk_mode(0);
        (void)sprintf(response_buffer.response_text,
                "Disabled superchunk memory mode (prior setting was %d)", prev_setting);
        NORMAL_RESPONSE;
        break;
    default :
        (void)sprintf(response_buffer.response_text,
                "server_diags: Unsupported disable subcode (%d)", request_p->subcode);
        NORMAL_RESPONSE;
        break;
    }
    return (0);
}



/* -------------------------------------------------------------------------- */

/*
 *   Close command processing
 */
static int 
do_close (sys_mailbox     mbx,
          diag_request_t  *request_p)
{
    diag_response_t response_buffer;

    switch (request_p->subcode) {
    case MESSAGE_LOG_SUBCODE :
        net_diag_close_logging_file();
        (void)sprintf(response_buffer.response_text, "Closed message logging file.");
        NORMAL_RESPONSE;
        break;
    default :
        (void)sprintf(response_buffer.response_text,
                "server_diags: Unsupported close subcode (%d)", request_p->subcode);
        NORMAL_RESPONSE;
        break;
    }
    return (0);
}



/* -------------------------------------------------------------------------- */

/*
 *   Set command processing
 */
static int 
do_set (sys_mailbox     mbx,
        diag_request_t  *request_p)
{
    diag_response_t response_buffer;
    int prev_setting;
    int status;

    switch (request_p->subcode) {
    case ALT_EVENT_DB_SUBCODE :
	(void)sprintf(response_buffer.response_text,
                "Obsolete");
	NORMAL_RESPONSE;
        break;

    case FORCE_BACK_SUBCODE :
        status = db_diag_set_force_back((file_info_t *) request_p->args.type_1.arg2,
                                        request_p->args.type_1.arg1);
        (void)sprintf(response_buffer.response_text,
                "set_force_back returns: %d (0x%x)", status, status);

        NORMAL_RESPONSE;
        break;

    case FORCE_CKPT_SUBCODE :
        status = db_diag_set_force_ckpt((file_info_t *) request_p->args.type_1.arg2,
                                        request_p->args.type_1.arg1);
        (void)sprintf(response_buffer.response_text,
                "set_force_ckpt returns: %d (0x%x)", status, status);

        NORMAL_RESPONSE;
        break;

    case INHIBIT_BACK_SUBCODE :
        status = db_diag_set_inhibit_back((file_info_t *) request_p->args.type_1.arg2,
                                        request_p->args.type_1.arg1);
        (void)sprintf(response_buffer.response_text,
                "set_inhibit_back returns: %d (0x%x)", status, status);

        NORMAL_RESPONSE;
        break;

    case COMPRESSION_SUBCODE :
        status = db_diag_set_ckpt_comp_level((file_info_t *) request_p->args.type_1.arg2,
                                        request_p->args.type_1.arg1);
        (void)sprintf(response_buffer.response_text,
                "Previous compression level: %d", status);

        NORMAL_RESPONSE;
        break;

    case ADV_PING_SUBCODE :
        {
        diag_state_t old_state;
        status = adver_diag_set_message_flow(
            request_p->args.type_1.arg2,
            (diag_state_t)request_p->args.type_1.arg1,
            &old_state);
        if (status == DNS_SUCCESS) {
        (void)sprintf(response_buffer.response_text,
                "Previous state was: %s", server_diag_setting_ascii(old_state));
        } else {
        (void)sprintf(response_buffer.response_text,
                "adver_diag_set_message_flow call failed: 0x%x", status);
        }
        NORMAL_RESPONSE;
        break;
        }
    default :
        (void)sprintf(response_buffer.response_text,
                "server_diags: Unsupported set subcode (%d)", request_p->subcode);
        NORMAL_RESPONSE;
        break;
    }
    return (0);
}


/* -------------------------------------------------------------------------- */


/*
 *   Management command processing (minimal management via diagnostics)
 */
static int 
do_mgmt (sys_mailbox     mbx,
         diag_request_t  *request_p)
{
    diag_response_t response_buffer;
    SimpleName_u    simple_ch;
    int             simple_ch_len;
    FullName_u      full_ch;
    int             full_ch_len;
    FullName_u      full_replica;
    FullName_t      *full_replica_p = (FullName_t *)full_replica;
    int             full_replica_len;
    ObjUID_t	    newuid;
    int status;
    int cell_op_code;

    switch (request_p->subcode) {
    case DISABLE_SERVER_SUBCODE :
	kill(getpid(), SIGUSR1);
        (void)sprintf(response_buffer.response_text,
                "Disable server returned: ");
        NORMAL_RESPONSE;
        break;
    case INIT_NS_SUBCODE :
        simple_ch_len = sizeof(simple_ch);
	status = clerk_cvt_simple_to_opaque(request_p->args.type_3.arg2,
			strlen(request_p->args.type_3.arg2),
			(char *)simple_ch, &simple_ch_len, NULL);
        if (status != DNS_SUCCESS) {
            (void)sprintf(response_buffer.response_text, "Gack! (root name error)");
            NORMAL_RESPONSE;
            break;
        }
	status = dns_initialize(diag_ns_p,
				(SimpleName_t *)simple_ch, 
				&newuid);
        (void)sprintf(response_buffer.response_text,
                "Initialize namespace returned: 0x%x (%d)", status, status);
        NORMAL_RESPONSE;
        break;
    case ADD_CELL_ALIAS_SUBCODE:
    case REM_CELL_ALIAS_SUBCODE:
        full_ch_len = sizeof(full_replica);
	status = clerk_cvt_full_to_opaque(request_p->args.type_3.arg2,
			strlen(request_p->args.type_3.arg2),
			(char *)full_replica_p, &full_ch_len, NULL);
        if (status != DNS_SUCCESS) {
            (void)sprintf(response_buffer.response_text, "Gack! (cell name error)");
            NORMAL_RESPONSE;
            break;
        }

        if (request_p->subcode == ADD_CELL_ALIAS_SUBCODE) {
            cell_op_code = UD_present;
        } else {
            cell_op_code = UD_absent;
        }
/*****************************
        status = modify_cell_alias((file_info_t *)request_p->args.type_1.arg2,
                                   cell_op_code, request_p->args.type_1.arg1,
                                   full_replica_p);

        (void)sprintf(response_buffer.response_text, "Modify alias returns: %d",
                      status);
*****************************/
        NORMAL_RESPONSE;
        break;
    default :
        (void)sprintf(response_buffer.response_text,
                "server_diags: Unsupported mgmt subcode (%d)", request_p->subcode);
        NORMAL_RESPONSE;
        break;
    }
    return (0);
}


/* -------------------------------------------------------------------------- */

/*
 *   Mark command processing
 */
static int 
do_mark (sys_mailbox     mbx,
         diag_request_t  *request_p)
{
    diag_response_t response_buffer;

    dce_svc_printf(CDS_S_DIAG_MARK_MSG, request_p->args.type_2.arg1);
    (void)sprintf(response_buffer.response_text, "Mark event logged.");
    NORMAL_RESPONSE;
    return (0);
}



/* -------------------------------------------------------------------------- */

/*
 *   Signal command processing
 */
static int 
do_signal (sys_mailbox     mbx,
           diag_request_t  *request_p)
{
    diag_response_t response_buffer;
    int status;

    switch (request_p->subcode) {
    case SIGNAL_SUBCODE :
        status = dthread_signal((dthread_cond_t *)&request_p->args.type_1.arg2);
        (void)sprintf(response_buffer.response_text, "dthread_signal returns 0x%x(%d)",
                status, status);
        NORMAL_RESPONSE;
        break;
    case BROADCAST_SUBCODE :
        status = dthread_broadcast((dthread_cond_t *)&request_p->args.type_1.arg2);
        (void)sprintf(response_buffer.response_text, "dthread_broadcast returns 0x%x(%d)",
                status, status);
        NORMAL_RESPONSE;
        break;
    default :
        (void)sprintf(response_buffer.response_text,
                "server_diags: Unsupported signal subcode (%d)", request_p->subcode);
        NORMAL_RESPONSE;
        break;
    }
    return (0);
}


/* -------------------------------------------------------------------------- */

/*
 *   Navigate command processing
 */
static int 
do_navigate (sys_mailbox     mbx,
             diag_request_t  *request_p)
{
    diag_response_t response_buffer;
    char *result_p;

    result_p = (char *) navigate((char *)request_p->args.type_1.arg2,
                                 request_p->subcode,
                                 0);
    (void)sprintf(response_buffer.response_text,
            "navigate() returned 0x%x", result_p);
    NORMAL_RESPONSE;
    return (0);
}


/* -------------------------------------------------------------------------- */

/*
 *   Dump command processing
 */
static int 
do_dump (sys_mailbox     mbx,
         diag_request_t  *request_p)
{
    diag_response_t response_buffer;

    switch (request_p->subcode) {
    case PERM_MEM_SUBCODE :
        dns_diag_vm_dump(mem_permanent);
        (void)sprintf(response_buffer.response_text,
                "Permanent memory list dumped to disk.");
        NORMAL_RESPONSE;
        break;
    case TEMP_MEM_SUBCODE :
        dns_diag_vm_dump(mem_temporary);
        (void)sprintf(response_buffer.response_text,
                "Temporary memory list dumped to disk.");
        NORMAL_RESPONSE;
        break;
    case UNK_MEM_SUBCODE :
        dns_diag_vm_dump(mem_unknown);
        (void)sprintf(response_buffer.response_text,
                "Unknown memory list dumped to disk.");
        NORMAL_RESPONSE;
        break;
    case IGN_MEM_SUBCODE :
        dns_diag_vm_dump(mem_ignored);
        (void)sprintf(response_buffer.response_text,
                "Ignored memory list dumped to disk.");
        NORMAL_RESPONSE;
        break;
    case ALL_MEM_SUBCODE :
        dns_diag_vm_dumpall();
        (void)sprintf(response_buffer.response_text,
                "Memory list (all types) dumped to disk.");
        NORMAL_RESPONSE;
        break;

    case HISTORY_MEM_SUBCODE :
        dns_diag_vm_lastmallocs();
        (void)sprintf(response_buffer.response_text,
                "Recent memory allocation history dumped to disk.");
        NORMAL_RESPONSE;
        break;

    case AUDIT_MEM_SUBCODE :
        vm_audit();
        sprintf(response_buffer.response_text,
                "Virtual memory zone audit has been completed.");
        NORMAL_RESPONSE;
        break;

    default :
        (void)sprintf(response_buffer.response_text,
                "Dump subcode %d is not supported by this server_diags.",
                request_p->subcode);
        NORMAL_RESPONSE;
        break;
    }
    return (0);
}



/* -------------------------------------------------------------------------- */

/*
 *
 */
static int 
server_diag_processor (sys_mailbox     mbx,
                       diag_request_t  *request_p)
{
    diag_response_t response_buffer;

    switch (request_p->opcode) {

    case examine_op:  (void)do_examine(mbx, request_p);  break;

    case enable_op:   (void)do_enable(mbx, request_p);   break;

    case disable_op:  (void)do_disable(mbx, request_p);  break;

    case set_op:      (void)do_set(mbx, request_p);      break;

    case close_op:    (void)do_close(mbx, request_p);    break;

    case mgmt_op:     (void)do_mgmt(mbx, request_p);     break;

    case mark_op:     (void)do_mark(mbx, request_p);     break;

    case signal_op:   (void)do_signal(mbx, request_p);   break;

    case navigate_op: (void)do_navigate(mbx, request_p); break;

    case dump_op:     (void)do_dump(mbx, request_p);     break;

    default:
        (void)sprintf(response_buffer.response_text,
                "server_diags received an unimplemented opcode: %d", request_p->opcode);
        NORMAL_RESPONSE;
        break;
    }

    response_buffer.response_code = end_of_responses;
    response_buffer.response_error = 0;
    (void)sys_mbx_write(mbx, (unsigned char *)&response_buffer, sizeof(res_code_t));

    return (DNS_SUCCESS);
}



/* -------------------------------------------------------------------------- */

/*
 * This routine starts up the diagnostic thread via the opening of the
 * diagnostic port (socket/mailbox).
 */

void * 
server_diag_start (nsgbl_ns_t *ns_p)
{
    diag_ns_p = ns_p;

    if (sys_openmbx((unsigned char *) ns_p->ns_diag_name_p,
		         255,
                         (mbx_user_routine) server_diag_processor,
			 1,		/* set block mode */
                         &diag_thread_attr,
                         &diag_mbx_id) != DNS_SUCCESS)
	ns_p = 0;

    return((void *)ns_p);
}


/* -------------------------------------------------------------------------- */

/*
 * This routine closes the diagnostic port which, in turn, shuts down
 * the diagnostic thread.
 */

void 
server_diag_stop (void *handle)
{
    if (diag_mbx_id)
        (void)sys_closembx(&diag_mbx_id);

}



/* -------------------------------------------------------------------------- */

/*
 * This routine accepts a generic diagnostic switch setting and returns
 * a general interpretation of it (ala strcmp).
 *
 * Returns:
 *     -1 indicates that operation inhibiting is desired (inhibit).
 *      0 indicates that normal operation is desired (normal/auto/off).
 *      1 indicates that special operation is desired (force/on/strobe*).
 *
 *      If the switch passed in was a strobe, it is modified to the base
 *      setting (e.g., strobe_normal -> normal).
 */

int 
server_diag_check_setting (diag_state_t *switch_p)
{
    switch(*switch_p) {
    case diag_inhibit:
        return -1;
    case diag_normal:
    case diag_auto:
    case diag_off:
        return  0;
    case diag_force:
    case diag_on:
        return  1;
    case diag_strobe_off:
        *switch_p = diag_off;             /* Strobe fired, revert to off */
        return  1;
    case diag_strobe_on:
        *switch_p = diag_on;              /* Strobe fired, revert to on */
        return  1;
    case diag_strobe_normal:
        *switch_p = diag_normal;          /* Strobe fired, revert to normal */
        return  1;
    case diag_strobe_auto:
        *switch_p = diag_auto;            /* Strobe fired, revert to auto */
        return  1;
    default:
        dce_fprintf(stderr, cds_s_diag_switch, (int)*switch_p);
        return  0;                       /* Bad switch goes to normal */
    }
    /* NOTREACHED */
}


/* -------------------------------------------------------------------------- */

char 
*server_diag_setting_ascii (diag_state_t switch_value)
{
    switch(switch_value) {
    case diag_inhibit:
        return "Inhibit";
    case diag_normal:
        return "Normal";
    case diag_auto:
        return "Auto";
    case diag_off:
        return "Off";
    case diag_force:
        return "Force";
    case diag_on:
        return "On";
    case diag_strobe_on:
    case diag_strobe_off:
    case diag_strobe_normal:
    case diag_strobe_auto:
        return "Strobe";
    default:
        return "*INVALID*";
    }
    /* NOTREACHED */
}
