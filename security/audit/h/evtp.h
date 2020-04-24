/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * (c) Copyright International Business Machines Corp. 1994, 1996
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 *
 */
/*
 * HISTORY
 * $Log: evtp.h,v $
 * Revision 1.1.6.1  1996/07/08  18:27:44  arvind
 * 	Audit for 1.2.2
 * 	[1996/06/21  20:25 UTC  arvind  /main/arvind_audit122/1]
 *
 * 	Audit for 1.2.2
 * 	[1996/02/18  22:57:37  marty  1.1.4.2]
 *
 * Revision 1.1.4.2  1996/02/18  22:57:37  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:16:29  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  17:40:06  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:17:53  root]
 * 
 * Revision 1.1.2.11  1994/09/22  21:09:02  weisz
 * 	CR 10703: handling of trail full situation.
 * 	[1994/09/22  21:07:30  weisz]
 * 
 * Revision 1.1.2.10  1994/08/17  19:56:50  weisz
 * 	CR 11443: added PENDING as a new valid outcome.
 * 	[1994/08/17  19:53:58  weisz]
 * 
 * Revision 1.1.2.8  1994/07/27  14:54:13  weisz
 * 	CR 11372: added status parameter to audit_pickle_dencode_ev_info.
 * 	[1994/07/27  14:53:20  weisz]
 * 
 * Revision 1.1.2.7  1994/07/25  16:24:45  weisz
 * 	CR 11166: initialize own_id_t next pointer to NULL.
 * 	[1994/07/25  16:24:09  weisz]
 * 
 * Revision 1.1.2.6  1994/07/25  14:04:28  weisz
 * 	CR 11166: cache own id, to prevent trail reading loop.
 * 	[1994/07/25  14:03:37  weisz]
 * 
 * Revision 1.1.2.5  1994/03/11  22:48:32  luan
 * 	CR 10115: Added data structures for event class table.
 * 	[1994/03/11  22:04:12  luan]
 * 
 * Revision 1.1.2.4  1994/03/03  18:58:22  luan
 * 	CR 9773: Make consistent prior-inclusion checks in header files.
 * 	[1994/03/03  18:57:59  luan]
 * 
 * Revision 1.1.2.3  1994/02/22  16:47:27  luan
 * 	CR 9775 9776 9860:  Defined DCE_AUD_DEBUG in audit.h.  Removed
 * 	debug_mode and PRE_BETA_DEBUG or replaced it with DCE_DEBUG.
 * 	[1994/02/22  16:39:39  luan]
 * 
 * Revision 1.1.2.2  1994/02/18  19:47:00  luan
 * 	CR 9777: Changed to use <dce/audit.h> instead of <audit.h>.
 * 	[1994/02/11  03:04:37  luan]
 * 
 * Revision 1.1.2.1  1993/12/22  12:39:08  zeliff
 * 	First version of code
 * 	[1993/12/21  15:01:21  zeliff]
 * 
 * $EndLog$
 */

#ifndef _EVTP_H
#define _EVTP_H

#include <stdarg.h>
#include <time.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <dce/dce.h>
#include <dce/nbase.h>
#include <dce/idlbase.h>
#include <dce/stubbase.h>
#include <dce/binding.h>
#include <dce/policy.h>
#include <dce/sec_login.h>
#include <dce/idl_es.h>
#include <dce/secidmap.h>
#include <dce/utc.h>

#include "audit_log.h"
#include <dce/audit.h>
#include <dce/dceaudmsg.h>
#include <dce/dceaudmac.h>
#include <dce/dceaudsvc.h>

#include <eslp.h>
#include <esl_filep.h>
#include <error_statusp.h>
#include <pathsp.h>

#define MAX_CONDITIONS                 5
#define MAX_ACTIONS                    4
#define TRAIL_DAEMON                   0
#define TRAIL_FILE                     1
#define SMALL_SIZE                    16
#define MEDIUM_SIZE                  256
#define FILE_NAME_LENGTH             256
#define REFRESH_ID_LIMIT             300
#define LARGE_SIZE                  4096
#define EVENT_BUF_SIZE              4096
#define DEFAULT_TRAIL_SIZE_LIMIT 2000000

extern pthread_mutex_t esl_mutex;

extern time_t   aud_esl_input_file
                (
		                 aud_esl_type_t,
		                 unsigned32,
		                 unsigned32 *,
		                 void ***,
		                 aud_esl_guides_p_t *,
		                 aud_esl_guides_p_t *
);

extern time_t   aud_esl_input_update_file
                (
		                 aud_esl_type_t,
		                 unsigned32,
		                 unsigned32 *,
		                 void ***
);

extern void     audit_pickle_dencode_ev_info
                (
		                 idl_es_handle_t,
		                 dce_aud_hdr_t *,
		                 aud_log_ev_info_list_t **,
		                 error_status_t *
);

void            aud_cli_bind_to_dmn
                (
		                 rpc_binding_handle_t *,
		                 error_status_t *
);

extern unsigned32         aud_g_cli_num_of_ecs;
extern unsigned32        *aud_g_cli_evt_classes;
extern boolean32          aud_g_cli_audit_on;
extern boolean32          aud_g_cli_filter_on;
extern aud_esl_guides_p_t aud_g_cli_world_guides;
extern aud_esl_guides_p_t aud_g_cli_world_overridable_guides;

extern void **aud_g_cli_esl[];

/*
 * The following table, once established, tells us to which event classes
 * each auditable event belongs. The size of the event table is determined 
 * at run time and equals to the number of auditable events defined in
 * the calling audit client program; i.e., table size = aud_g_cli_num_of_evts.
 */

typedef struct {
	unsigned32      evt_number;
	int             evt_class_cnt;	/* number of event classes         */
	unsigned32     *evt_classes;	/* array of event classes to which */
				        /* this event belongs              */
} evt_t;

extern evt_t *evt_table;

/*
 * The following table (ec_table), once established, lists auditable events in
 * each event class. The size of the event class table is determined 
 * at run time, and equals to the number of valid event class files in the
 * event class directory.
 */

typedef struct _ec_evt_t {
	unsigned32	  evt_number;
	struct _ec_evt_t *next;
} ec_evt_t;

typedef struct _ec_t {
	unsigned32        ec_number;
	char		 *ec_name;
	ec_evt_t     	 *evts;		/* linked list of events in */
					/* this event class         */
	struct _ec_t     *next;
} ec_t;

extern ec_t *ec_table;

typedef struct _e_t {
        char             *ec_name;
        unsigned32        e_number;
        char             *e_name;
        char             *e_text;
        struct _e_t      *next;
} e_t;
 
extern struct e_t *e_table;

extern time_t               ESL_File_Timestamp[];
extern time_t               ESL_UPDATE_File_Timestamp[];
extern boolean32            cell_uuid_known;
extern sec_rgy_handle_t     rgy_context;
extern sec_rgy_properties_t properties;
extern unsigned32           aud_g_cli_first_evt_number;
extern unsigned32           aud_g_cli_num_of_evts;

struct act_cond_text {
    char           *name;
    unsigned32      number;
};

extern struct act_cond_text audit_condition[];
extern struct act_cond_text audit_action[];

struct _dce_aud_trail_handle {
    int                  location;	  /* 0: trail daemon, 1: trail file   */
    rpc_binding_handle_t auditd_binding;  /* audit daemon                     */
    char                *trail_file;	  /* trail file name, if location = 0 */
    int                  trail_fd;	  /* descriptor of trail file (open)  */
    FILE	        *trail_fp;	  /* descriptor of trail file (fopen  */
    char                *index_file;	  /* index file name, if location = 0 */
    int                  index_fd;	  /* descriptor of trail file (open)  */
    FILE                *index_fp;        /* descriptor of trail file (fopen) */
    int                  md_index_fd;     /* descriptor of index file (open)  */
    FILE                *md_index_fp;     /* descriptor of index file (fopen) */
    unsigned32           index_cursor;    /* index cursor                     */
    unsigned32           trail_cursor;    /* trail cursor                     */
    unsigned32           flags;	          /* I/O flags parameter              */
    boolean32            file_size_limit;
    unsigned32           file_size_limit_value;
    pthread_mutex_t      mutex; 
};


struct _dce_aud_rec_handle {
    dce_aud_hdr_t   header;
    aud_log_ev_info_list_t *tail;
    unsigned32      tail_read_cursor;
    unsigned32      options;
    aud_esl_act_t *as;	/* action specifiers */
};

typedef struct own_id {
    struct own_id   *next; 
    uuid_t          group_uuid;
    sec_rgy_name_t  global_group_name;
} own_id_t;

#endif /* _EVTP_H */
