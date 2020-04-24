/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d23svcserv.c,v $
 * Revision 1.1.6.3  1996/02/18  18:19:53  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:42:38  marty]
 *
 * Revision 1.1.6.2  1995/12/08  16:03:48  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/DCE1_1_WP/1  1995/04/26  19:42 UTC  jrr
 * 	Include pthread.h for AIX.
 * 	[1995/12/08  15:15:20  root]
 * 
 * Revision 1.1.2.2  1994/09/06  12:25:36  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:16:15  keutel]
 * 
 * Revision 1.1.2.1  1994/06/21  14:39:42  marrek
 * 	Created in June 94 code submission.
 * 	[1994/06/10  16:04:32  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d23svcserv.c,v $ $Revision: 1.1.6.3 $ $Date: 1996/02/18 18:19:53 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*exon ***********************************************************************/
/*                                                                           */
/*   TYPE:    module                                                         */
/*                                                                           */
/*   NAME:    d23svcserv.c                                                   */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*                                                                           */
/*   This module represents the RPC-server for serviceability. The following */
/*   functions are contained:                                                */
/*                                                                           */
/*        - d23_initsvcrpc          initialize and establish RPC-server      */
/*        - d23_svcserver           RPC-server thread (RPC-listener)         */
/*        - d23_perform_svc_op      perform serviceability operation         */
/*                                                                           */
/*        - dce_svc_set_route                serviceability RPC's            */
/*        - dce_svc_set_dbg_route                     "                      */
/*        - dce_svc_set_dbg_levels                    "                      */
/*        - dce_svc_inq_components     *)             "                      */
/*        - dce_svc_inq_routings       *)             "                      */
/*        - dce_svc_inq_table          *)             "                      */
/*        - dce_svc_filter_control     *)             "                      */
/*        - dce_svc_inq_stats          *)             "                      */
/*                                                                           */
/*       *) = function is not supported                                      */
/*                                                                           */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 2.5.94      */
/*exoff **********************************************************************/

#include <gds.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <service.h>
#include <dce/svcremote.h>
#include <dce/dcesvcmsg.h>
#include <d2dir.h>
#include <d23ipc.h>

/* --------------------- DEFINITION OF CONSTANTS --------------------------- */

					/* GDS component names */
#define MONITOR		"monitor"
#define DUACACHE	"cache"
#define CSTUB		"cstub"
#define SSTUB		"sstub"
#define DSA		"dsa"

#define SVC_SETROUTE	1
#define SVC_DBGROUTE	2
#define SVC_DBGLEVEL	3

#define SVC_DEBUGCHAR	'v'		/* svc debug specification flag */
#define SVC_ROUTECHAR	'w'		/* svc routing specification flag */
#define SVC_TMOUT	30		/* distributed command timeout value */

/* --------------------- DECLARATION OF GLOBAL DATA ------------------------ */

extern uuid_t	uuid_nil ;

/* --------------------- DECLARATION OF LOCAL DATA ------------------------- */

static rpc_binding_vector_p_t	bvect ;

/* --------------------- PROTOTYPING OF LOCAL FUNCTIONS -------------------- */

void d23_svcserver (unsigned_char_t *) ;
error_status_t d23_perform_svc_op (signed32, unsigned char *) ;


/* ***************** initialize RPC-server ********************************* */

signed32 d23_init_svcserver (unsigned_char_t *ename) {

    pthread_t		d23_handle ;
    pthread_attr_t	d23_attr ;
    unsigned32		status ;
    idl_char		*string_binding ;
    int			i ;

    /* tell RPC-runtime to use all supported protocols for communication */
    rpc_server_use_all_protseqs (rpc_c_protseq_max_reqs_default, &status) ;
    if (status != rpc_s_ok) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
			     svc_c_sev_warning, GDS_S_RPC_RUNTIME_ERR, status) ;
	return (-1) ;
    }

    /* register server interface with the RPC-runtime */
    rpc_server_register_if (serviceability_v1_0_s_ifspec, &uuid_nil, NULL,
								      &status) ;
    if (status != rpc_s_ok) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
			     svc_c_sev_warning, GDS_S_RPC_RUNTIME_ERR, status) ;
	return (-1) ;
    }

    /* get server binding information */
    rpc_server_inq_bindings (&bvect, &status) ;
    if (status != rpc_s_ok) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
			     svc_c_sev_warning, GDS_S_RPC_RUNTIME_ERR, status) ;
	return (-1) ;
    }
    if (ename == (unsigned_char_t *) NULL) {
	/* evaluate binding information because no global name was specified */
	for (i = 0; i < bvect->count; i++) {
	    rpc_binding_to_string_binding (bvect->binding_h[i],
						     &string_binding, &status) ;
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%s"), GDS_S_GENERAL,
			 svc_c_sev_notice, GDS_S_RPC_BINDINFO, string_binding) ;
	    rpc_string_free (&string_binding, &status) ;
	    if (status != rpc_s_ok) {
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
			     svc_c_sev_warning, GDS_S_RPC_RUNTIME_ERR, status) ;
		return (-1) ;
	    }
	}
    }

    /* register server binding information in the local endpoint map */
    rpc_ep_register (serviceability_v1_0_s_ifspec, bvect, (uuid_vector_p_t)
					NULL, (unsigned_char_t) NULL, &status) ;
    if (status != rpc_s_ok) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
			     svc_c_sev_warning, GDS_S_RPC_RUNTIME_ERR, status) ;
	return (-1) ;
    }

    if (ename != (unsigned_char_t *) NULL) {
	/* export binding information into name service data base */
	rpc_ns_binding_export (rpc_c_ns_syntax_default, ename,
	 serviceability_v1_0_s_ifspec, bvect, (uuid_vector_t *) NULL, &status) ;
	if (status != rpc_s_ok) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
			     svc_c_sev_warning, GDS_S_RPC_RUNTIME_ERR, status) ;
	    return (-1) ;
	}
    }

    /* initialize thread specific attributes object */
    /* create thread attributes object */	
    if (pthread_attr_create (&d23_attr) == -1) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
			      svc_c_sev_warning, GDS_S_RPC_THREADS_ERR, errno) ;
	return (-1) ;
    }
    /* change inherit scheduling attribute */
    if (pthread_attr_setinheritsched (&d23_attr,
						 PTHREAD_DEFAULT_SCHED) == -1) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
			      svc_c_sev_warning, GDS_S_RPC_THREADS_ERR, errno) ;
	return (-1) ;
    }
    /* change scheduling policy attribute */
    if (pthread_attr_setsched (&d23_attr, SCHED_RR) == -1) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
			      svc_c_sev_warning, GDS_S_RPC_THREADS_ERR, errno) ;
	return (-1) ;
    }
    /* change scheduling priority attribute */
    if (pthread_attr_setprio (&d23_attr, PRI_RR_MAX) == -1) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
			      svc_c_sev_warning, GDS_S_RPC_THREADS_ERR, errno) ;
	return (-1) ;
    }
    /* create RPC-server listener thread */
    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL, svc_c_debug3,
						     GDS_S_IPCMON_RPC_THREAD)) ;
    if (pthread_create (&d23_handle, d23_attr, (pthread_startroutine_t)
				   d23_svcserver, (pthread_addr_t) ename) < 0) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
			      svc_c_sev_warning, GDS_S_RPC_THREADS_ERR, errno) ;
        return (-1) ;
    }
    /* delete thread attributes object */
    if (pthread_attr_delete (&d23_attr) == -1) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
			      svc_c_sev_warning, GDS_S_RPC_THREADS_ERR, errno) ;
	return (-1) ;
    }
    return (0) ;
}

/* *********************** RPC-server listener thread ********************** */

void d23_svcserver (unsigned_char_t *ename) {

    unsigned32			status ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL, svc_c_debug3,
						     GDS_S_IPCMON_RPC_LISTEN)) ;
    /* tell RPC-runtime to listen for remote procedure calls */
    rpc_server_listen (rpc_c_listen_max_calls_default, &status) ;
    if (status != rpc_s_ok) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
			     svc_c_sev_warning, GDS_S_RPC_RUNTIME_ERR, status) ;
	exit (1) ;
    }

    if (ename != (unsigned_char_t *) NULL) {
	/* remove server binding information from the name service database */
	rpc_ns_binding_unexport (rpc_c_ns_syntax_default, ename,
		serviceability_v1_0_s_ifspec, (uuid_vector_t *) NULL, &status) ;
	if (status != rpc_s_ok) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
			     svc_c_sev_warning, GDS_S_RPC_RUNTIME_ERR, status) ;
	    exit (1) ;
	}
    }

    /* remove server binding information from local endpoint map */
    rpc_ep_unregister (serviceability_v1_0_s_ifspec, bvect, (uuid_vector_p_t)
								NULL, &status) ;
    if (status != rpc_s_ok) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
			     svc_c_sev_warning, GDS_S_RPC_RUNTIME_ERR, status) ;
	exit (1) ;
    }
    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
					  svc_c_sev_notice, GDS_S_EXITMONITOR) ;
    exit (0) ;
}

/* ************************************************************************* */
/* ********************** serviceability RPC's ***************************** */
/* ************************************************************************* */

void dce_svc_set_route (handle_t handle, unsigned char *where,
						       error_status_t *status) {
    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%s%s"), GDS_S_GENERAL, svc_c_debug3,
			   GDS_S_IPCMON_RPCENTRY, "dce_svc_set_route", where)) ;
    *status = d23_perform_svc_op (SVC_SETROUTE, where) ;
    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%s%d"), GDS_S_GENERAL, svc_c_debug3,
			  GDS_S_IPCMON_RPCEXIT, "dce_svc_set_route", *status)) ;
    return ;
}

/* ------------------------------------------------------------------------- */

void dce_svc_set_dbg_route (handle_t handle, unsigned char *where,
						       error_status_t *status) {
    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%s%s"), GDS_S_GENERAL, svc_c_debug3,
		       GDS_S_IPCMON_RPCENTRY, "dce_svc_set_dbg_route", where)) ;
    *status = d23_perform_svc_op (SVC_DBGROUTE, where) ;
    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%s%d"), GDS_S_GENERAL, svc_c_debug3,
		      GDS_S_IPCMON_RPCEXIT, "dce_svc_set_dbg_route", *status)) ;
    return ;
}

/* ------------------------------------------------------------------------- */

void dce_svc_set_dbg_levels (handle_t handle, unsigned char *flags,
						       error_status_t *status) {
    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%s%s"), GDS_S_GENERAL, svc_c_debug3,
		      GDS_S_IPCMON_RPCENTRY, "dce_svc_set_dbg_levels", flags)) ;
    *status = d23_perform_svc_op (SVC_DBGLEVEL, flags) ;
    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%s%d"), GDS_S_GENERAL, svc_c_debug3,
		     GDS_S_IPCMON_RPCEXIT, "dce_svc_set_dbg_levels", *status)) ;
    return ;
}

/* ------------------------------------------------------------------------- */

void dce_svc_filter_control (handle_t handle, dce_svc_string_t component,
			long arg_size, byte *argument, error_status_t *status) {
    /* function not supported */
    *status = svc_s_no_perm ;
    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
				     svc_c_sev_warning, GDS_S_RPC_UNSUPPORTED) ;
    return ;
}

/* ------------------------------------------------------------------------- */

void dce_svc_inq_stats (handle_t handle, dce_svc_stats_t *stats,
						       error_status_t *status) {
    /* function not supported */
    *status = svc_s_no_perm ;
    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
				     svc_c_sev_warning, GDS_S_RPC_UNSUPPORTED) ;
    return ;
}

/* ------------------------------------------------------------------------- */

void dce_svc_inq_table (handle_t handle, dce_svc_string_t component,
			dce_svc_subcomparray_t *table, error_status_t *status) {
    /* function not supported */
    *status = svc_s_no_perm ;
    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
				     svc_c_sev_warning, GDS_S_RPC_UNSUPPORTED) ;
    return ;
}

/* ------------------------------------------------------------------------- */

void dce_svc_inq_routings (handle_t handle, boolean32 debug_values,
			 dce_svc_stringarray_t *table, error_status_t *status) {
    /* function not supported */
    *status = svc_s_no_perm ;
    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
				     svc_c_sev_warning, GDS_S_RPC_UNSUPPORTED) ;
    return ;
}

/* ------------------------------------------------------------------------- */

void dce_svc_inq_components (handle_t handle, dce_svc_stringarray_t *table,
						       error_status_t *status) {
    /* function not supported */
    *status = svc_s_no_perm ;
    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
				     svc_c_sev_warning, GDS_S_RPC_UNSUPPORTED) ;
    return ;
}

/* **************** perform serviceability operation *********************** */

error_status_t d23_perform_svc_op (signed32 func, unsigned char *svc_info) {

    char		*cspecptr, compname[16], svcbuf[D23_DPARSIZE] ;
    signed32		dirid = 0, svc_length, tmppos, position = 0 ;
    error_status_t	status ;
    D23_dwritepb	dwrpb ;

    /* search GDS component specification */
    /* format: <serviceability info>:[<directory-id>,]<gds component info> */
    if ((cspecptr = strrchr ((char *) svc_info, ':')) == (char *) NULL) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%s"), GDS_S_GENERAL,
			   svc_c_sev_warning, GDS_S_RPC_GDSINFO_ERR, svc_info) ;
	return (svc_s_unknown_component) ;
    }
    *(cspecptr++) = '\0' ;

    /* copy serviceability information into a buffer (the first character */
    /* specifies the type of information */
    if ((svc_length = strlen ((char *) svc_info)) > D23_DPARSIZE - 2) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_GENERAL,
					svc_c_sev_warning, GDS_S_RPC_SVCINFO,
					svc_length, 1, D23_DPARSIZE - 2) ;
	return (svc_s_unknown_component) ;
    }
    if (func != SVC_DBGLEVEL) {
	svcbuf[0] = (func == SVC_DBGROUTE) ? SVC_DEBUGCHAR : SVC_ROUTECHAR ;
	strcpy (&svcbuf[1], (char *) svc_info) ;
	svc_length += 2 ;
    } else {
	strcpy (svcbuf, (char *) svc_info) ;
	svc_length ++ ;
    }
 
    /* evaluate GDS component specification */
    if (sscanf (cspecptr, "%d%n", &dirid, &position) == 1) {
        /* directory-ID specified */ 
	if (dirid < 1 || dirid > D2MAXDIR_ID) {
	    /* invalid directory-ID specified */
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_GENERAL,
				svc_c_sev_warning, GDS_S_RPC_GDSINFO_DIR_ERR,
				dirid, 1, D2MAXDIR_ID) ;
	    return (svc_s_unknown_component) ;
	}
    } 
    if (!*(cspecptr + position)) {
	/* no GDS component specified */
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%s"), GDS_S_GENERAL,
			   svc_c_sev_warning, GDS_S_RPC_GDSINFO_ERR, svc_info) ;
	return (svc_s_unknown_component) ;
    }
    while (*(cspecptr + position)) {
        /* evaluate GDS component name(s) */ 
	tmppos = 0 ;
	/* skip delimiter value */
	sscanf (cspecptr + position, "%*[, ]%n", &tmppos) ;
	position += tmppos ;
        if (sscanf (cspecptr + position, "%[^ ,]%n", compname, &tmppos) == 1) {
	    /* component name specified */
	    position += tmppos ;

	    if (strcmp (compname, MONITOR) == 0) {
		/* component is IPC-monitoring process */
		/* operation is performed by a local serviceability */
		/* function call */
		if (func == SVC_SETROUTE) {
		    dce_svc_routing (svc_info, &status) ;
		    if (status != svc_s_ok) {
			dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"),
				GDS_S_GENERAL, svc_c_sev_warning,
				GDS_S_RPC_SVC_FAILED, status) ;
			return (status) ;
		    }
		    continue ;
		}
		if (func == SVC_DBGROUTE) {
		    dce_svc_debug_routing (svc_info, &status) ;
		    if (status != svc_s_ok) {
			dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"),
				GDS_S_GENERAL, svc_c_sev_warning,
				GDS_S_RPC_SVC_FAILED, status) ;
			return (status) ;
		    }
		    continue ;
		}
		if (func == SVC_DBGLEVEL) {
		    dce_svc_debug_set_levels (svc_info, &status) ;
		    if (status != svc_s_ok) {
			dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"),
				GDS_S_GENERAL, svc_c_sev_warning,
				GDS_S_RPC_SVC_FAILED, status) ;
			return (status) ;
		    }
		    continue ;
		}
		continue ;
	    }

	    /* for the following GDS components the serviceability operation */
	    /* is performed by means of a distributed command */
	    if (strcmp (compname, DUACACHE) == 0) {
		/* component is DUA-cache */
		dwrpb.d23_Jsv_prid = D23_DCASID ;
	    } else {
		if (strcmp (compname, CSTUB) == 0) {
		    dwrpb.d23_Jsv_prid = D23_CSTSID ;
		} else {
		    if (strcmp (compname, SSTUB) == 0) {
			if (!dirid) {
			    /* no directory-ID specified */
	    		    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"),
				GDS_S_GENERAL, svc_c_sev_warning,
				GDS_S_RPC_GDSINFO_DIR_ERR, 0, 1, D2MAXDIR_ID) ;
			    return (svc_s_unknown_component) ;
			}
			dwrpb.d23_Jsv_prid = D23_SSTSID + dirid ;
		    } else {
			if (strcmp (compname, DSA) == 0) {
			    if (!dirid) {
				/* no directory-ID specified */
	    			dce_svc_printf (DCE_SVC (gds_svc_handle,
				 "%d%d%d"), GDS_S_GENERAL, svc_c_sev_warning,
				 GDS_S_RPC_GDSINFO_DIR_ERR, 0, 1, D2MAXDIR_ID) ;
				return (svc_s_unknown_component) ;
			    }
			    dwrpb.d23_Jsv_prid = D23_DS0SID + dirid ;
			} else {
			    /* illegal component specification */
			    dce_svc_printf (DCE_SVC (gds_svc_handle, "%s"),
					GDS_S_GENERAL, svc_c_sev_warning,
					GDS_S_RPC_GDSINFO_ERR, cspecptr) ;
			    return (svc_s_unknown_component) ;
			}
		    }
		}
	    }
	    /*initialize distributed command parameter block */
	    dwrpb.d23_Jhdinfo.d2_version = D23_V02 ;
	    dwrpb.d23_Jcmd = (func == SVC_DBGLEVEL) ? D23_SVCLOGLEVEL :
								    D23_ENALOG ;
	    dwrpb.d23_Jtmout = SVC_TMOUT ;
	    dwrpb.d23_Jparsize = svc_length ;
	    dwrpb.d23_Jparref = (byte *) svcbuf ;
	    if (d23_write_dist_cmd (&dwrpb) != D23_SUCES) {
	        dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"),
			GDS_S_GENERAL, svc_c_sev_warning, GDS_S_RPC_DISTCMD_ERR,
			dwrpb.d23_Jcmd, dwrpb.d23_Jsv_prid,
			dwrpb.d23_Jhdinfo.d2_errvalue) ;
		return (svc_s_unknown_component) ;
	    }
	} else {
	    /* illegal component specification */
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%s"), GDS_S_GENERAL,
			   svc_c_sev_warning, GDS_S_RPC_GDSINFO_ERR, cspecptr) ;
	    return (svc_s_unknown_component) ;
	}
    }
    return (svc_s_ok) ;
}
