/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: gda.h,v $
 * Revision 1.1.6.2  1996/02/18  23:33:59  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:44  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:16:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:43:13  root]
 * 
 * Revision 1.1.4.6  1994/08/16  18:27:53  jd
 * 	drop from janet
 * 	[1994/08/13  16:05:58  jd]
 * 
 * Revision 1.1.4.5  1994/08/03  19:02:25  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:58:10  mccann]
 * 
 * Revision 1.1.4.4  1994/06/23  18:30:33  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  19:18:29  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:39:11  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:11:49  devsrc]
 * 
 * Revision 1.1.4.2  1994/05/12  21:11:55  peckham
 * 	Remove NBPG and _DNS_DEFAULT_STACK_ definitions.
 * 	[1994/05/12  19:16:42  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:03:26  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:16:35  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:31:58  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:51:01  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:16:37  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _GDA_H
#define _GDA_H
/*
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1991-1994. ALL RIGHTS RESERVED.
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
 */

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dce_error.h>
#include <dce/dcecdsmsg.h>
#include <dcecdssvc.h>
#include <dcecdsmac.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dce/dns_config.h>
#include <sys/param.h>
#include <sys/types.h>
#include <dce/rpc.h>
#include <netinet/in.h>
#include <arpa/nameser.h>
#include <pthread.h>
#include <threads_lib.h>
#include <dce/dns_record.h>
#include <tower.h>
#include <dce/cdsclerk.h>
#include <dce/cds_types.h>
#include <gda_common.h>
#include <gda_rwl.h>
#include <gda_list.h>
#include <gda_bind.h>
#include <gda_debug.h>

#ifndef OK
# define OK 0
#endif

/* the debug_el_t structure describes where PRINT statements should actually
	be printed by the gda.  This can vary by what type of thread is
	running, thread_d, what function is being executed, func_d,
	how bad the error is, severity_d, what the error name is, error_d,
	or what the error code is, errnum_d. */
typedef struct debug_el_t {
	int	thread_d;	/* thread type */
	int	func_d;		/* function type */
	int	severity_d;	/* severity of the error */
	int	error_d;	/* error name */
	int	errnum_d;	/* error code */
} debug_el_t;

enum ci_tag	{ e_bind, e_x500 };

/* x500_t is used to store x500 data specific to a query within the 
	question_el_t structure. */
typedef struct x500_t {
	int	temp;
} x500_t;

/* must declare type of conduit_el_t before it's usage in question_el_t */
typedef struct conduit_el_t conduit_el_t;

typedef struct question_el_t {
	/* data common to all conduits */

	int			routine_q;

		/* input only.  routine gives the number of the cds_* routine,
			see the _INDEX constants, that was called */

	cds_FullName_t		unresolved_in_q;

		/* input only. unresolved_in_q stores the unresolved name
			input to the cds_* routine. */

	cds_FullName_t		resolved_out_q;

		/* output only. resolved_out_q stores the name resolved by
			the conduit code of the gda. */

	cds_FullName_t		unresolved_out_q;

		/* output only. unresolved_out_q stores the name resolved by
			the conduit code of the gda. */

	uuid_t			nsp_uuid_q;

		/* output only. nsp_uuid_q stores the namespace uuid
			of the namespace identified by resolved_out_q. */

	cds_FullName_t		dir_name_q;

		/* output only. dir_name_q stores the directory name associated
			with the namespace identified by resolved_out_q. */

	uuid_t			dir_uuid_q;

		/* output only. nsp_uuid_q stores the directory uuid
			of the namespace identified by resolved_out_q.  If
			dir_uuid_q is not available to the conduit code,
			dir_uuid_q must be set equal to nsp_uuid_q. */
			
	int			num_replicas_q;

		/* output only. num_replicas_q stores the number of replicas
			pointed to by replicas_q. */
	cds_ReplicaPointer_t	**replicas_q;
		/* output only. replicas_q points to memory allocated by
			rpc_ss_allocate which stores the replicas associated
			with the namespace identified by resolved_out_q. */

	conduit_el_t		*conduit_q;

		/* input only. conduit_q points to the conduit structure 
			associated with the conduit which is attempting to
			resolve the name unresolved_in_q. */

	/* cd_q stores all of the information specific to the conduit yet
		which is associated with the name being resolved. */
	enum ci_tag		tag_q;
	union {
		bind_t		bind;
		x500_t		x500;
	} cd_q;
} question_el_t;

#define	NUM_CDS_ROUTINES		17
#define	NUM_CONDUIT_ROUTINES_PER_CDS	3
#define	INIT_FUNCTION			0
#define	PROCESS_FUNCTION		1
#define	DESTROY_FUNCTION		2

/* the conduit_el_t structure describes the aspects of each conduit that
	the gda can use to answer a question.  Bind and x500 are conduits.
	All conduit structures are initialized by the conduit itself. */
struct conduit_el_t {
	int	(*init_conduit)(void);
			/* init_conduit is the initialization routine for
				the conduit code */

	int (*func_c[NUM_CDS_ROUTINES][NUM_CONDUIT_ROUTINES_PER_CDS])(question_el_t *);
			/* func_c stores the addresses of the routines used
				by the conduit code to answer each of the
				cds calls the gda can receive.  Each conduit
				processing function is paired with its
				initialization and destruction routine. */
	int	single_threaded_c;
			/* if single_threaded_c is set to TRUE, then the
				conduit is considered to be capable of only
				single threaded actions.  Only one call to
				the conduit will proceed at one time. */
	rwl_t	rwl_c;
			/* rwl_c is used to ensure that single threaded
				conduit code runs unobstructed. */
};

typedef struct ReplicaInfo {
    int    clh_type;        /* clearinghouse type (RT_Master or RT_Readonly) */
    char   clh_uuid[50];    /* clearinghouse uuid */
    char   clh_name[CDS_STR_SNAME_MAX+6];    
		           /* clearinghouse name ("/.../" + simplename)*/
    char   *towers;        /* malloc'd buffer containing a list of pointers 
		            * to towers followed by the towers, themselves */
} ReplicaInfo_t;


/**
 ** FUNCTION PROTOTYPES.... 
 **/

/* 
 * gda_update_parent.c
 */
int
get_ObjUID (
    ObjUID_t *);
int
update_parent (
   uuid_t *);
/* 
 * gda_rpcint.c
 */
error_status_t
InitializeRPC (
    char  *,
    int   );
/* 
 * gda_cell.c
 */
int 
get_cell_info (
    char           *,
    ReplicaInfo_t  *,
    int            *);
/* 
 * gda_process.c
 */
int
process (
    question_el_t *);
/*
 * gda_dacl.c
 */
int
gda_mgmt_acl_init(
    void);

#endif  /* #ifndef _GDA_H */
