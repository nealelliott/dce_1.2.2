/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: server_globals.c,v $
 * Revision 1.1.11.2  1996/02/18  19:36:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:17:12  marty]
 *
 * Revision 1.1.11.1  1995/12/08  15:32:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:44  root]
 * 
 * Revision 1.1.9.9  1994/09/15  15:21:43  mccann
 * 	add -v option OT12161
 * 	[1994/09/15  15:20:37  mccann]
 * 
 * Revision 1.1.9.8  1994/09/07  20:02:17  mccann
 * 	fix version handling OT:12004
 * 	[1994/09/07  18:42:14  mccann]
 * 
 * Revision 1.1.9.7  1994/08/26  21:55:31  zee
 * 	Increment server software version to 4.0, default directory version to 4.0.
 * 	Also add identifying timestamp for V4 replicas.
 * 	[1994/08/26  21:49:17  zee]
 * 
 * Revision 1.1.9.6  1994/08/24  20:14:49  mccann
 * 	fix server shutdown, add annotation strings, start message delay:OT11239,11245 and 11766
 * 	[1994/08/24  20:09:12  mccann]
 * 
 * Revision 1.1.9.5  1994/08/03  19:04:33  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:27:41  mccann]
 * 
 * Revision 1.1.9.4  1994/06/09  18:43:45  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:16:19  devsrc]
 * 
 * Revision 1.1.9.3  1994/05/06  16:04:25  zee
 * 	     Bump server software version to 3.1.
 * 	[1994/05/05  21:20:43  zee]
 * 
 * Revision 1.1.9.2  1994/04/14  14:51:29  peckham
 * 	Remove CMA, VMS and UNIX conditionals.
 * 	Remove dnstables dependencies.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	[1994/04/11  20:21:47  peckham]
 * 
 * Revision 1.1.9.1  1994/03/12  22:12:22  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:45:13  peckham]
 * 
 * Revision 1.1.7.2  1992/12/30  14:23:17  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:23:47  zeliff]
 * 
 * Revision 1.1.2.3  1992/04/14  20:25:28  grober
 * 	Modified the parameters to get_ckpt_parameters to be unsigned long
 * 	rather than longword_t since longword_t is really unsigned char.
 * 	[1992/04/14  20:24:48  grober]
 * 
 * Revision 1.1.2.2  1992/03/22  22:32:58  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:35:51  weisman]
 * 
 * Revision 1.1  1992/01/19  15:26:08  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: server_globals.c
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
 * Module Description
 *
 * Maintains server-global structures.
 *
 */

#include <server.h>
#include <clerk.h>
#include <dbdef.h>
#include <dns_cfg.h>

static int binary_second = 10*1000*1000;

time_quad_t bkgdonehour;
utc_t bkgdonehour_utc;
static int set_one_hour = 60*60;	/* convert 1 hour to seconds */

time_quad_t bkgdtimer;
utc_t bkgdtimer_utc;
static int set_back_timer = 12*60*60; /* convert 12 hours to seconds */

time_quad_t bkgdskulk;
static int set_back_skulk = 24*60*60;    /* convert 24 hours to seconds */

time_quad_t bkgdskulkhalf;
static int set_back_skulkhalf = 12*60*60;/* convert 24/2 hours to seconds */

time_quad_t ppextend_deltatime;
utc_t ppextend_deltatime_utc;
static int ppextend = 24*60*60;

/* Default DirectoryVersion for this server */
static byte_t directory_version_major = 3;
static byte_t directory_version_minor = 0;

/* Server Software Version */
static byte_t server_version_major = 4;
static byte_t server_version_minor = 0;

/* Database Version (checkpoint file) written by this server */
static byte_t database_version_major = DATABASE_VERSION_MAJOR;
static byte_t database_version_minor = DATABASE_VERSION_MINOR;

/* Database Checkpoint default parameters */
static unsigned long database_tlog_threshold = 5;   /*  5 Megabytes */
static unsigned long database_ckpt_interval =  24;  /* 24 Hours */

long data_limit = 512*1024*1024;	/* MAX data size = 512Meg */

/*
 * unix_net.c
 */
dthread_attr_t rpc_server_listen_attr = {"rpc_listener", 0};
/*
 * Stack sizes for server threads
 *
 * back_collect.c
 */
dthread_attr_t collect_data_task_attr = {"collect_data_task", 0};
/*
 * back_copy_updates.c
 */
dthread_attr_t send_async_task_attr = {"send_async_task", 10000};
/*
 * back_ground.c
 */
dthread_attr_t background_attr = {"background", 64000};
dthread_attr_t background_activator_attr = {"background_activator", 7000};
/*
 * back_propagate.c
 */
dthread_attr_t back_propagate_fork_attr = {"back_propagate_fork", 10000};
/*
 * db_checkpoint.c
 */
dthread_attr_t db_checkpoint_memory_attr = {"db_checkpoint_memory", 0};
/*
 * dns_ncl_mgmt_ultrix.c
 */
dthread_attr_t dns_server_mgmt_attr = {"dns_server_mgmt", 32000};
/*
 * ta_adver.c
 */
dthread_attr_t ta_ping_advertiser_attr = {"ta_ping_advertiser", 17500};

#ifdef DCE_SEC
dthread_attr_t sec_init_attr = {"sec_init_thread", 32000};
#endif

/*
 * *_mbx.c
 */
dthread_attr_t diag_thread_attr = {"diagnostics", 0};

/* Configuration parameter table */
static dthread_attr_t *attrp[] = {
    &collect_data_task_attr,
    &send_async_task_attr,
    &background_attr,
    &background_activator_attr,
    &back_propagate_fork_attr,
    &db_checkpoint_memory_attr,
    &dns_server_mgmt_attr,
    &ta_ping_advertiser_attr,
    &rpc_server_listen_attr,
    0
};

/*
 *  V4.0 replica-indicating timestamp used in ta_replica_add and
 *  ta_link_replica calls.
 */
static Timestamp_t v4_replica_ts = {
    {0,0,0,0,0,0},			         /* ts_node */
    {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x40}};  /* ts_time */

void 
init_global_structures (void)
{
    timespec_t inacc;
    reltimespec_t time;
    register dthread_attr_t **attr_pp = attrp, *attr_p;
    register char *handle, *chr_p;

    /* Get defaults from configuration file */
    handle = dns_cfgopen(NULL, NULL);
    while (attr_p = *attr_pp++) {
	char parm[256];

	strcpy(parm, attr_p->name);
	strcat(parm, ".stack");
	attr_p->stacksize = dns_cfgtol(handle, parm, attr_p->stacksize);
    }

    data_limit = dns_cfgtol(handle, "data_limit", data_limit);



    dns_cfgclose(handle);

    /* Convert times to relative delta times */
    dns_emul(&ppextend, &binary_second, &ppextend_deltatime);
    dns_emul(&set_one_hour, &binary_second, &bkgdonehour);
    dns_emul(&set_back_timer, &binary_second, &bkgdtimer);
    dns_emul(&set_back_skulkhalf, &binary_second, &bkgdskulkhalf);
    dns_emul(&set_back_skulk, &binary_second, &bkgdskulk);

    /* Setup UTC times */
    inacc.tv_sec = 0;
    inacc.tv_nsec = 0;
    time.tv_sec = ppextend;
    time.tv_nsec = 0;
    utc_mkbinreltime(&ppextend_deltatime_utc, &time, &inacc);

    time.tv_sec = set_one_hour;
    utc_mkbinreltime(&bkgdonehour_utc, &time, &inacc);

    time.tv_sec = set_back_timer;
    utc_mkbinreltime(&bkgdtimer_utc, &time, &inacc);

}


void 
get_database_version (VersionNumber_t *version_p)
{
    COPY_byte(&database_version_major, version_p->vn_major);
    COPY_byte(&database_version_minor, version_p->vn_minor);
}

void 
get_directory_version (VersionNumber_t *version_p)
{
    COPY_byte(&directory_version_major, version_p->vn_major);
    COPY_byte(&directory_version_minor, version_p->vn_minor);
}

void 
set_directory_version (byte_t major, byte_t minor)
{
    directory_version_major = major;
    directory_version_minor = minor;
}


void 
get_server_version (VersionNumber_t *version_p)
{
    COPY_byte(&server_version_major, version_p->vn_major);
    COPY_byte(&server_version_minor, version_p->vn_minor);
}


void 
get_ckpt_parameters (unsigned long  *tlog_threshold_p,
                     unsigned long  *ckpt_interval_p)
{
    if (tlog_threshold_p)
	*tlog_threshold_p = database_tlog_threshold;

    if (ckpt_interval_p)
	*ckpt_interval_p = database_ckpt_interval;
}


void
get_v4_replica_ts (Timestamp_t *ts_p)
{
    COPY_Timestamp(&v4_replica_ts, ts_p);
}
