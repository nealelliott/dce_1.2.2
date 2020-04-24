/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: gda_rpcint.c,v $
 * Revision 1.1.11.2  1996/02/18  19:31:18  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:12:25  marty]
 *
 * Revision 1.1.11.1  1995/12/08  15:13:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:41:33  root]
 * 
 * Revision 1.1.8.9  1994/08/23  19:57:37  mccann
 * 	add annotation strings OT:11766
 * 	[1994/08/23  19:52:01  mccann]
 * 
 * Revision 1.1.8.8  1994/08/03  19:01:41  mccann
 * 	includes cleanup
 * 	[1994/08/03  18:44:07  mccann]
 * 
 * Revision 1.1.8.7  1994/07/25  15:13:06  proulx
 * 	Use common ACL manager for subcomponent mgmt ACLs
 * 	[1994/07/25  14:01:51  proulx]
 * 
 * Revision 1.1.8.6  1994/06/23  19:10:03  jd
 * 	Removed extern evtblk. It no longer exists
 * 	[1994/06/23  18:59:08  jd]
 * 
 * Revision 1.1.8.5  1994/06/23  18:30:07  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  19:28:04  mccann]
 * 
 * Revision 1.1.8.4  1994/06/09  18:38:09  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:10:21  devsrc]
 * 
 * Revision 1.1.8.3  1994/05/20  22:03:17  sommerfeld
 * 	rdaclif.h -> dce/rdaclif.h
 * 	[1994/05/17  19:36:11  sommerfeld]
 * 
 * Revision 1.1.8.2  1994/05/12  21:11:15  peckham
 * 	Change _DNS_DEFAULT_STACK_ reference to _CDS_DEFAULT_STACK_
 * 	[1994/05/12  19:15:09  peckham]
 * 
 * Revision 1.1.8.1  1994/03/12  22:01:43  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:09:41  peckham]
 * 
 * Revision 1.1.6.5  1993/01/22  20:35:23  mfox
 * 	Fix CR 6969 - gdad marshalls freed memory by using rpc_ss_allocate()
 * 	[1993/01/22  20:32:18  mfox]
 * 
 * Revision 1.1.6.4  1992/12/30  13:27:25  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:44:10  zeliff]
 * 
 * Revision 1.1.6.3  1992/12/07  14:33:59  mfox
 * 	Initialize a structure with zeros so the length field for a resolved name
 * 	doesn't have trash in it causing gdad to core dump on some platforms.
 * 	[1992/12/07  14:32:58  mfox]
 * 
 * Revision 1.1.6.2  1992/09/29  19:12:57  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:12:19  weisman]
 * 
 * Revision 1.1.2.5  1992/06/25  19:02:43  mfox
 * 	Was returning an uninitialized variable instead of rpc_s_ok. This caused
 * 	an rpc exception to be returned to a clerk, causing cdsd to erase knowledge
 * 	of a gda.
 * 	[1992/06/25  19:01:34  mfox]
 * 
 * Revision 1.1.2.4  1992/05/22  14:01:23  mfox
 * 	Cleanup logging bugs when running with -d7. Display things that weren't.
 * 	[1992/05/21  23:44:33  mfox]
 * 
 * Revision 1.1.2.3  1992/04/16  17:13:31  mfox
 * 	Replace NULLs with 0's to make RIOS ANSI C compiler happy
 * 	[1992/04/16  14:01:57  mfox]
 * 
 * Revision 1.1.2.2  1992/03/22  21:57:39  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:24:28  weisman]
 * 
 * Revision 1.1  1992/01/19  15:24:27  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * gda_rpcint.c
 *
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
 *
 */
/*
 * Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA USA
 * All Rights Reserved
 */

/*
        rpc_int.c includes those routines which implement the rpc interface
        to the GDA.
*/
#include <stdio.h>

#include <gda.h>
#include <dce/dce_cf.h>
#include <dce/rpc.h>
#include <dce/dce_error.h>
#include <dce/cds_basic_types.h>

#include <rpcserver.h>
#include <errno.h>

#include <dce/cds_clerkserver.h>
#include <dce/rpc_cvt.h>

#include <dce/sys_time.h>
#include <gda_main.h>
#if defined(DCE_SEC)
# include <dce/rdaclifv0.h>
# include <dce/rdaclif.h>
#endif

/*
 * index of the following functions in the conduit func_c array 
 */
#define CDS_CREATE_SOFTLINK_INDEX               0
#define CDS_CREATEOBJECT_INDEX                  1
#define CDS_DELETEOBJECT_INDEX                  2
#define CDS_DELETESOFTLINK_INDEX                3
#define CDS_DELETEDIRECTORY_INDEX               4
#define CDS_ENUMERATEATTRIBUTES_INDEX           5
#define CDS_ENUMERATECHILDREN_INDEX             6
#define CDS_ENUMERATEOBJECTS_INDEX              7
#define CDS_ENUMERATESOFTLINKS_INDEX            8
#define CDS_MODIFYATTRIBUTE_INDEX               9
#define CDS_READATTRIBUTE_INDEX                 10
#define CDS_RESOLVENAME_INDEX                   11
#define CDS_SKULK_INDEX                         12
#define CDS_TESTATTRIBUTE_INDEX                 13
#define CDS_TESTGROUP_INDEX                     14
#define CDS_ALLOWCLEARINGHOUSES_INDEX           15
#define CDS_DISALLOWCLEARINGHOUSES_INDEX        16
#define CDS_DOUPDATE_INDEX                      17
#define CDS_COMBINE_INDEX                       18
#define CDS_LINKREPLICA_INDEX                   19
#define CDS_MODIFYREPLICA_INDEX                 20
#define CDS_ADDREPLICA_INDEX                    21
#define CDS_REMOVEREPLICA_INDEX                 22
#define CDS_NEWEPOCH_INDEX                      23
#define CDS_DELETECHILD_INDEX                   24
#define CDS_CREATECHILD_INDEX                   25
#define CDS_CREATEDIRECTORY_INDEX               26

/*
 * Local Function Prototypes..
 */

static int 
print_objuid (int     , 
              char    *,        
              byte_t  *,        
              int     , 
              char    *);

static int 
print_rp (cds_ReplicaPointer_t  *,
          int                   ,
          char                  *);

static int 
build_progress (question_el_t   *,
                cds_Progress_t  *,
                int             , 
                cds_status_t    *,
                int             status);

static error_status_t 
get_rpc_answer (int     ,
                cds_Progress_t  *,
                int     ,
                cds_status_t    *);

static int 
print_progress (cds_Progress_t  *,
                cds_status_t    *,
                int             ,
                char            *);
static void 
rpc_listener (
              void);

/*
 * The gdad's binding vectors (aquired in 'InitializeRPC'):
 */

rpc_binding_vector_p_t gdad_bind_vector;              /* binding vectors*/
extern int x;
extern uuid_t   null_uuid;

#if defined(DCE_SEC)
extern char gda_principal_name[];
extern rdaclif_v0_0_epv_t dce_acl_v0_0_epv; 
extern rdaclif_v1_0_epv_t dce_acl_v1_0_epv; 
#endif

/*
 * This uuid unambiguously identifies the gda's clerk-server interface,
 * (from the cdsd's clerk-server interface), so that we can determine
 * whether or not a gdad is already running. It is used for nothing
 * else.
 */
static char * gda_uuid_string = "2293230D-DFC4-11CA-A53C-08002B1C8A62";
uuid_t gda_uuid;

extern   serviceability_v1_0_epv_t dce_svc_ep;

/* Define object uuid for management and serviceability */
static char *gda_mgmt_dacl_string = "fa620458-924f-11cc-840e-08002b1c8a62";
uuid_t gda_mgmt_dacl_uuid;

/*
 *      rpc_listener thread
 */
static void 
rpc_listener (void)
{
    unsigned32          rpc_status;
    unsigned char 	inq_error_string[dce_c_error_string_len];
    int			inq_sts;
    

    inq_error_string[0]='\0';
    
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug2,
        " >rpc_listener"));

    /*
     * initialize the thread specific data 
     */
    if (pthread_setspecific(thread_num,(pthread_addr_t)&rpc_listener_th_num))
    {
	dce_svc_printf(CDS_S_PTHREADSETSPECIFIC_MSG, (long)errno);
        error_exit(-1);
    }

    /*
     * listen for requests.  This function does not return unless
     * an error occurs 
     */
    rpc_server_listen(rpc_c_listen_max_calls_default, &rpc_status);
    if (rpc_status != rpc_s_ok)
    {
        dce_svc_printf(CDS_S_RPCSERVERLISTEN_MSG, (long)rpc_status);
	error_exit(-1);
    }
    
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug2,
        " <rpc_listener"));
}

/*
 *      InitializeRPC: initializes the rpc interface and creates the
 *      rpc_listener_th thread
 */
error_status_t 
InitializeRPC (char  *myname_p,
               int   noFork)
{
    pthread_t           rpc_listener_th;
    pthread_attr_t      pattr;
    unsigned int        mode;
    int                 n, if_count=0;
    rpc_if_descriptor_t if_descriptors[4];
    error_status_t      rpc_status;
    int                 status;
    dce_error_string_t  error_str;
    uuid_vector_t       *uuids;
    uuid_vector_t       svc_uuids;
#if defined(DCE_SEC)
    uuid_vector_t       sec_uuids;
#endif

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug2,
        " >InitializeRPC"));
    
    /*
     * Fork a child, if we're not running in debug mode 
     */
    mode = (noFork) ? (rpc_c_server_mode_debug | rpc_c_server_mode_verify) : 0;
    if (noFork == 0) 
    {
        if (cds_server_fork (myname_p, mode, &rpc_status) != 0) 
        {
	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_gda,
                svc_c_debug2,
		" <InitializeRPC: return(CDS_ERROR)"));

            return (CDS_ERROR);
        }
    }

    /*
     * Get my clearinghouse uuid
     */
    if ((status = get_ObjUID (&ch_id)) != CDS_SUCCESS) 
    {
	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug2,
	    " <InitializeRPC: return(FALSE)"));

        return (status);
    }
    
    /*
     * Set up the management Object UUID for the gda to 
     * export to the RPC end point map, and to be used in the
     * ACL database.
     */
    uuid_from_string ((unsigned char *)gda_mgmt_dacl_string,
                      &gda_mgmt_dacl_uuid, &rpc_status);
    
    if (rpc_status != uuid_s_ok) 
    {
      dce_svc_printf(CDS_S_UUIDFROMSTR_MSG, (long)rpc_status);
      DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug2,
	    " <InitializeRPC: return(rpc_status which is not uuid_s_ok)"));
      return (rpc_status);
    }

    /* 
     * Call rpc_server_init to check for server already running,
     * and register our interfaces/endpoints.
     */

    ZERO_bytes (if_descriptors, sizeof (if_descriptors));

    /*
     * Get the gdad's unique id 
     */
    uuid_from_string ((unsigned char *)gda_uuid_string,
                      &gda_uuid, &rpc_status);
    if (rpc_status != uuid_s_ok) 
    {
      dce_svc_printf(CDS_S_UUIDFROMSTR_MSG,(long)rpc_status);
      DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug2,
	    " <InitializeRPC: return(rpc_status which is not uuid_s_ok)"));

      return (rpc_status);
    }   
    /*
     * malloc a vector for 2 uuids 
     */
    uuids = (uuid_vector_t *)malloc(sizeof(uuid_vector_t) + sizeof(uuid_t));
    uuids->count = 2;
    uuids->uuid[0] = &gda_uuid;
    uuids->uuid[1] = &ch_id;
    if_descriptors[if_count].if_handle = cds_clerkserver_v1_0_s_ifspec;
    if_descriptors[if_count].if_epv    = NULL;
    if_descriptors[if_count].object_ids = uuids;
    if_descriptors[if_count].annotation = 
	(char *)dce_msg_get_msg(cds_s_gda_clerkserverif, &rpc_status);
    if_count++;
    /*
     * Set up the servicibility interface 
     */
    if_descriptors[if_count].if_handle = serviceability_v1_0_s_ifspec;
    if_descriptors[if_count].if_epv    = (rpc_mgr_epv_t *)&dce_svc_ep;
    svc_uuids.count = 1;
    svc_uuids.uuid[0] = &gda_mgmt_dacl_uuid;
    if_descriptors[if_count].object_ids = &svc_uuids;
    if_descriptors[if_count].annotation = 
	(char *)dce_msg_get_msg(cds_s_gda_svcif, &rpc_status);
    if_count++;

#if defined(DCE_SEC)
    /*
     * Remote DACL interface, V0. 
     */
    sec_uuids.count = 1;
    sec_uuids.uuid[0] = &gda_mgmt_dacl_uuid;

    if_descriptors[if_count].if_handle = rdaclif_v0_0_s_ifspec;
    if_descriptors[if_count].object_ids = &sec_uuids;
    if_descriptors[if_count].if_epv    = (rpc_mgr_epv_t *)&dce_acl_v0_0_epv;
    if_descriptors[if_count].annotation = 
	(char *)dce_msg_get_msg(cds_s_gda_aclifv0, &rpc_status);
    ++if_count;

    /*
     * Remote DACL interface, V1.
     */
    if_descriptors[if_count].if_handle = rdaclif_v1_0_s_ifspec;
    if_descriptors[if_count].object_ids = &sec_uuids;
    if_descriptors[if_count].if_epv    = (rpc_mgr_epv_t *)&dce_acl_v1_0_epv;
    if_descriptors[if_count].annotation = 
	(char *)dce_msg_get_msg(cds_s_gda_aclifv1, &rpc_status);
    ++if_count;
#endif /* DCE_SEC */

    if (cds_server_init ( myname_p,
                          NULL,
                          if_count,
                          if_descriptors,
                          mode,
                          &gdad_bind_vector,
                          &rpc_status) != 0) 
    {
	free ((char *)uuids);

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug2,
	    " <InitializeRPC: rpc_server_init failure: return(CDS_ERROR)"));

	return (CDS_ERROR);
    }

    /*
     * Finished with the uuids now.
     */
    free ((char *)uuids);
    
    for (n=0; n<if_count; ++n)
	free(if_descriptors[n].annotation);

    for (n = 0; n < gdad_bind_vector->count; n++) 
    {
	/*
	 * Clear out the port no. from the binding
	 */
	rpc_binding_reset (gdad_bind_vector->binding_h[n], &rpc_status);
	if (rpc_status != rpc_s_ok) 
	{
	    dce_svc_printf(CDS_S_RPCBINDINGRESET_MSG, (long)rpc_status);
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug2,
		" <InitializeRPC: return(CDS_ERROR)"));

	    return(CDS_ERROR);
	}
    }

    /*
     * create the listener thread 
     */
    if (pthread_attr_create(&pattr) < 0) {
      dce_svc_printf(CDS_S_PTHREADATTRCREATE_MSG, (long)errno);
      return(CDS_ERROR);
    }

    if (pthread_attr_setstacksize(&pattr, _CDS_DEFAULT_STACK_) < 0) {
      dce_svc_printf(CDS_S_PTHREADATTRSETSTACKSIZE_MSG, (long)errno);
      return(CDS_ERROR);
    }

    if (pthread_create(&rpc_listener_th, pattr,
		      (pthread_startroutine_t)rpc_listener, 
		      (pthread_addr_t)NULL))
    {
	dce_svc_printf(CDS_S_PTHREADCREATE_MSG, (long)errno);
	return(CDS_ERROR);
    }
    
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug2,
        " <InitializeRPC"));

    return (CDS_SUCCESS);
}


/*
	build_progress builds a progress record, progress_p, from the
	data returned by process inside el.
*/
static int 
build_progress (question_el_t   *el, /* elm't which contains answer to a qu. */
                cds_Progress_t  *progress_p, /* ptr to progress rec'   */
		                             /* build_progress builds */
                int             type,    /* entry type */
                cds_status_t    *cds_status_p, /* ptr to an rpc routine's */
		                               /* status record */
                int             status) /* if == DNS_SUCCESS, then a name was*/
                                    /* resolved, otherwise, no name was found*/
{
	cds_Set_t		*outset_p;
	int			looper;
	cds_SetMember_t		*member;


	DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_gda,
	    svc_c_debug2,
	    " >build_progress"));

	/*
	 * Initialize returned pointers 
	 */

	cds_status_p->er_name = (cds_FullName_t *)NULL;

	if (status == DNS_NAMESERVERERRORS)
	{
	    /*
	     * an error occurred 
	     */
	    cds_status_p->er_status = PDU_Bug;

	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug2,
		" <build_progress: return(OK)"));

	    return(OK);
	}

	if (status == DNS_UNDERSPECIFIEDNAME) 
	{
	    /*
	     * the global name did not overlap a cell 
	     */
	    cds_status_p->er_status = PDU_UnderSpecifiedName;

	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug2,
		" <build_progress: return(OK)"));

	    return(OK);
	}

	if ((status == DNS_SUCCESS) && (el->unresolved_out_q.fn_length <= 1))
	    switch (type) {
	    case ET_directory:
	    case ET_dirOrObj:
		break;
	    default:
		status = DNS_UNKNOWNENTRY;
	    }

	if (status == DNS_UNKNOWNENTRY) 
	{
	    
	    /*
	     * the global name did not exist 
	     */
	    cds_status_p->er_status = PDU_UnknownEntry;

	    /*
	     * allocate and format error parameter 
	     */
	    cds_status_p->er_name =
		(cds_FullName_t *)rpc_ss_allocate(sizeof(cds_FullName_t));

	    if (cds_status_p->er_name == NULL) 
		dce_svc_printf(CDS_S_ALLOC_A_MSG);
		
	    ALLOC(cds_FullName_t, 1)

	    /* 
	     * set the namespace uuid 
	     */
	     COPY_ObjUID(cdsWildRoot, &cds_status_p->er_name->fn_root);
	      
	    /*
	     * next copy the name 
	     */
	    COPY_bytes("/...", cds_status_p->er_name->fn_name, 4);
	      
	    if (el->resolved_out_q.fn_length > 0) 
		{
		  COPY_bytes(el->resolved_out_q.fn_name,
			     cds_status_p->er_name->fn_name + 4,
			     el->resolved_out_q.fn_length);
		}
	    
	    /*
	     * next set length 
	     */
	    cds_status_p->er_name->fn_length =
		el->resolved_out_q.fn_length + 4;

	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug2,
		" <build_progress: return(OK)"));

	    return(OK);
	}

	/*
	 * status = DNS_SUCCESS  or DNS_NOTSUPPORTED 
	 */

	cds_status_p->er_status = PDU_Success;

	/*
	 * clear flags 
	 */

	progress_p->pr_flags &= ~PR_done;
	progress_p->pr_flags &= ~PR_up;
	progress_p->pr_flags &= ~PR_hitLink;

	if(status == DNS_NOTSUPPORTED)
	{
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug2,
		" <build_progress: return(OK)"));

	    return(OK);
	}

#ifdef PR_directory

	/*
	 * we only match directories 
	 */
	progress_p->pr_flags |= PR_directory;
#endif

	/*
	 * leave timeout alone 
	 */

	/*
	 * format resolved name 
	 */

	/*
	 * first copy namespace cts timestamp 
	 */
	COPY_ObjUID(cdsWildRoot, &progress_p->pr_resolved.fn_root);

	/*
	 * next copy the name 
	 */
	COPY_bytes("/...", progress_p->pr_resolved.fn_name, 4);
	COPY_bytes(el->resolved_out_q.fn_name,
		progress_p->pr_resolved.fn_name + 4,
		el->resolved_out_q.fn_length);

	/*
	 * next set length 
	 */
	progress_p->pr_resolved.fn_length =
					el->resolved_out_q.fn_length + 4;

	/*
	 * format unresolved name 
	 */

	/*
	 * first copy cts timestamp 
	 */
	progress_p->pr_unresolved.fn_root = el->nsp_uuid_q;

	/*
	 * next set length 
	 */
	progress_p->pr_unresolved.fn_length = el->unresolved_out_q.fn_length;

	/*
	 * next copy the name 
	 */
	COPY_bytes(el->unresolved_out_q.fn_name,
		progress_p->pr_unresolved.fn_name,
		progress_p->pr_unresolved.fn_length);



	/*
	 * include the replicas 
	 */

	/*
	 * allocate space for replicas and set type and length fields 
	 */
	if(el->num_replicas_q)
	{
		outset_p = (cds_Set_t *)rpc_ss_allocate(sizeof(cds_Set_t) +
				el->num_replicas_q * sizeof(cds_SetMember_t));
		ALLOC(ss_cds_Set_t, 1)

		if (outset_p == (cds_Set_t *)NULL)
		{
		    /*
		     * dealloc memory 
		     */
		  dce_svc_printf(CDS_S_ALLOC_A_MSG);
		}
		else
		{
		    outset_p->set_type = AT_set;
		    outset_p->set_length = el->num_replicas_q;
		}
	}
	else
		outset_p = (cds_Set_t *)NULL;

	/*
	 * copy the replica pointers to the member structures 
	 */
	for (looper = 0; looper < el->num_replicas_q; looper++)
	      {
		member = &outset_p->set_members[looper];

		/*
		 * set member flag 
		 */
		member->sm_flag = MV_present;

		/*
		 * copy null timestamp 
		 */
		ZERO_Timestamp(&member->sm_ts);

		member->sm_value.av_valuetype = VT_ReplicaPointer;
		member->sm_value.av_val.av_rp_p = el->replicas_q[looper];
	}
	free((void *)el->replicas_q);
	FREE(replicas_q, 1)

        /*
	 * Set up replica pointer 
	 */
	progress_p->pr_replicas_p = outset_p;

	DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_gda,
	    svc_c_debug2,
	    " <build_progress"));

	return(OK);
}

/*
	get_rpc_answer serves as an interface between process and the
	rpc interface routines.  get_rpc_answer calls process in order to
	answer the question asked and then calls build_progress in order
	to format the parameters returned by process into a progress
	record.  The altered progress record is returned by the rpc
	interface routine.
*/
static error_status_t 
get_rpc_answer (int routine, /* routine from which get_rpc_answer was called */
                cds_Progress_t  *progress_p, /* pointer to an rpc */
		                             /* routine's progress record */
                int type,	/* entry type */
                cds_status_t    *cds_status_p) 	/* pointer to an rpc */
                                                /* routine's status record */
{
#define	EL		LIST_REF(question_el_t, el)
#define LOCAL_BUF_LEN	2048
	question_el_t	*el;
	int		status;
	char		buffer[LOCAL_BUF_LEN];
	int		len;
	int		len_left;
	int		temp_len;
	int		looper;

	
	DCE_SVC_DEBUG((
	    cds__svc_handle,
            cds_svc_gda,
            svc_c_debug2,
            " >get_rpc_answer"));
 
	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug3,
            "get_rpc_answer: Entry type = %d",
            type));

	len_left = LOCAL_BUF_LEN;
	len = 0;

	strcpy(buffer, "\nPROGRESS RECORD IN:");
	len_left -= strlen(buffer);
	len += strlen(buffer);

	if((temp_len = print_progress(progress_p, cds_status_p,
				      len_left, buffer + len)) == -1)
	{
	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_gda,
                svc_c_debug1,
                "get_rpc_answer: print progress record in"));
	}
	else
	{
	    len += temp_len;
	    buffer[len] = '\0';

	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_gda,
                svc_c_debug1,
                "get_rpc_answer: print progress returned error.\n\t%s",
                buffer));
	}

	/*
	 * produce an element which process can use to answer the question 
	 * asked. zero it out to initialize all of the length fields 
	 */
	if((el = (question_el_t *)calloc(1, sizeof(question_el_t)))
			== (question_el_t *)NULL)
	{
	    dce_svc_printf(CDS_S_ALLOC_A_MSG);
	}
	el->resolved_out_q.fn_length = 0;
	el->unresolved_out_q.fn_length = 0;
	el->num_replicas_q = 0;
	ALLOC(question_el_t, 1)

	/*
	 * Strip off any /... 
	 */
	if(EQ_bytes("/...", progress_p->pr_unresolved.fn_name, 4))
	{
	    /*
	     * copy the timestamp 
	     */
		el->unresolved_in_q.fn_root =
				progress_p->pr_unresolved.fn_root;
		/*
		 * next copy the name 
		 */
		COPY_bytes(progress_p->pr_unresolved.fn_name + 4,
			el->unresolved_in_q.fn_name,
			progress_p->pr_unresolved.fn_length - 4);

		/*
		 * next set the length 
		 */
		el->unresolved_in_q.fn_length =
			progress_p->pr_unresolved.fn_length - 4;
	}
	else
	{
		el->unresolved_in_q	= progress_p->pr_unresolved;
	}

	el->routine_q		= routine;

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug5,
          "get_rpc_answer: before process"));

	len_left = LOCAL_BUF_LEN;
	len = 0;

	strcpy(buffer,"\nINPUT TO GLOBAL CONDUIT:\n\tunresolved name:\n\t\t");
	len_left -= strlen(buffer);
	len += strlen(buffer);

	if(len_left -  (el->unresolved_in_q.fn_length + 1) < 0)
	{
	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_gda,
                svc_c_debug3,
                "get_rpc_answer: buf len exceeded: unresolved_in_q"));
	}
	memmove( buffer + len,el->unresolved_in_q.fn_name,
	      el->unresolved_in_q.fn_length);
	len_left -= el->unresolved_in_q.fn_length;
	len += el->unresolved_in_q.fn_length;

	buffer[len] = '\0';
		
        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug3,
            "get_rpc_answer: %s",
	    buffer));

	/*
	 * call process function in order to answer the question 	
	 */
	status = process(el);

	len_left = LOCAL_BUF_LEN;
	len = 0;

	strcpy(buffer, "\nOUTPUT FROM GLOBAL CONDUIT:\n\tstatus:\n\t\t");

	len += strlen(buffer);
	len_left -= strlen(buffer);

	if(status == DNS_NAMESERVERERRORS)
	{
	    memmove( buffer + len,"DNS_NAMESERVERERRORS\n\0", 19);
	    len += 19;
	    len_left -= 19;
	    goto print;
	}
	else 
	    if(status == DNS_UNKNOWNENTRY)
	    {
		memmove(
		      buffer + len,"DNS_UNKNOWNENTRY\n\tresolved name:\n\t\t", 35);
		len += 35;
		len_left -= 35;
	    }
	    else 
		if(status == DNS_UNDERSPECIFIEDNAME)
		{
		    memmove(
			  buffer + len,"DNS_UNDERSPECIFIEDNAME\n\tresolved name:\n\t\t", 41);
		    len += 41;
		    len_left -= 41;
		}
		else 
		    if(status == DNS_SUCCESS)
		    {
			memmove(
			      buffer + len,"DNS_SUCCESS\n\tresolved name:\n\t\t", 30);
			len += 30;
			len_left -= 30;
		    }
	if(len_left -  (el->resolved_out_q.fn_length + 1) < 0)
	{
	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_gda,
                svc_c_debug1,
                "get_rpc_answer: buf len exceeded: resolved_out_q"));

	    goto end_print;
	}

	memmove( buffer + len,el->resolved_out_q.fn_name,
	      el->resolved_out_q.fn_length);
	len += el->resolved_out_q.fn_length;
	len_left -= el->resolved_out_q.fn_length;
	
	memmove( buffer + len,"\n\tunresolved name:\n\t\t", 21);
	
	len += 21;
	len_left -= 21;
	
	if(len_left -  (el->unresolved_out_q.fn_length + 1) < 0)
	{
	    DCE_SVC_DEBUG((
	        cds__svc_handle,
		cds_svc_gda,
		svc_c_debug1,
		"get_rpc_answer: buf len exceeded: unresolved_out_q"));

	    goto end_print;
	}
	
	memmove( buffer + len,el->unresolved_out_q.fn_name,
	      el->unresolved_out_q.fn_length);
	
	len += el->unresolved_out_q.fn_length;
	len_left -= el->unresolved_out_q.fn_length;
	
	if(status == DNS_UNDERSPECIFIEDNAME ||
	   status == DNS_UNKNOWNENTRY)
	{
	    memmove( buffer + len,"\n\0", 2);
	    len += 2;
	    len_left -= 2;
	    
	    goto print;
	}
	
	memmove( buffer + len,"\n\tnamespace uuid:\n\t\t", 20);
	
	len += 20;
	len_left -= 20;
	
	if((temp_len = print_objuid((int)0, (char *)0,
				    (byte_t *)&el->nsp_uuid_q, len_left,
				    buffer + len)) == -1)
	{
	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_gda,
                svc_c_debug1,
                "get_rpc_answer: print_objuid: buf len exceeded: nsp_uuid_q"));

	    goto end_print;
	}
	len += temp_len;
	len_left -= temp_len;
	
	memmove( buffer + len,"\n\tdirectory uuid:\n\t\t", 20);
	
	len += 20;
	len_left -= 20;
	
	if((temp_len = print_objuid((int)0, (char *)0,
				    (byte_t *)&el->dir_uuid_q, len_left,
				    buffer + len)) == -1)
	{
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug1,
		"get_rpc_answer: print_objuid: buf len exceeded: dir_uuid_q"));

	    goto end_print;
	}
	len += temp_len;
	len_left -= temp_len;

	if(len_left - 12 < 0)
	{
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug2,
		" <get_rpc_answer: return(0)"));

	    return(0);
	}
	
	memmove( buffer + len,"\n\treplicas:", 14);
	len_left -= 12;
	len += 12;
	
	/*
	 * display each replica 
	 */
	for(looper = 0; looper < el->num_replicas_q; looper++)
	{
	    if((temp_len = print_rp(el->replicas_q[looper],
				    len_left, buffer + len)) == -1)
	    {
		DCE_SVC_DEBUG((
		    cds__svc_handle,
		    cds_svc_gda,
		    svc_c_debug1,
		    "get_rpc_answer: print_rp:\n buf len exceeded: replicas_q"));
		goto end_print;
	    }
	    else
	    {
		len_left -= temp_len;
		len += temp_len;
	    }
	}
	*(buffer + len) = '\0';

print:

        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug3,
            "get_rpc_answer: %s",
	    buffer));

end_print:

	/*
	 * produce a progress record 	
	 */
	status = build_progress(el, progress_p, type, cds_status_p, status);

	len_left = LOCAL_BUF_LEN;
	len = 0;

	strcpy(buffer, "\nPROGRESS RECORD OUT:");
	len_left -= strlen(buffer);
	len += strlen(buffer);

	if((temp_len = print_progress(progress_p, cds_status_p, len_left,
				      buffer + len)) == -1)
	{
	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_gda,
                svc_c_debug1,
                "get_rpc_answer: print_progress failed"));
	}

	len += temp_len;
	buffer[len] = '\0';

        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug3,
            "get_rpc_answer: %s",
	    buffer));

	/*
	 * dealloc element 
	 */
	free((void *)el);
	FREE(question_el_t, 1);

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " <get_rpc_answer"));

	return(rpc_s_ok);

#undef	EL
#undef	LOCAL_BUF_LEN
}	

error_status_t 
cds_CreateSoftLink (handle_t         h,			/* [in] */
                    cds_Progress_t   *Progress_p,	/* [in, out] */
                    cds_FullName_t   *target_p,	 	/* [in] */
                    cds_Timeout_t    *linkTimeout_p, 	/* [in] */
                    cds_Timestamp_t  *actual_ts_p,	/* [out] */
                    cds_status_t     *cds_status_p)	/* [out] */
{
	error_status_t	answ;

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " >cds_CreateSoftLink"));

	/*	
	 * initialize thread specific data 
	 */
	if (pthread_setspecific(thread_num,
			(pthread_addr_t)&cds_createsoftlink_th_num))
	{
	  dce_svc_printf(CDS_S_PTHREADSETSPECIFIC_MSG, (long)errno);
	  error_exit_th(-1);
	}

	/*
	 * get the answer 
	 */
	answ = get_rpc_answer(CDS_CREATE_SOFTLINK_INDEX,
			Progress_p, ET_softlink, cds_status_p);

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " <cds_CreateSoftLink"));

	return(answ);
}

error_status_t 
cds_CreateObject (handle_t         h,			/* [in] */
                  cds_Progress_t   *Progress_p,		/* [in, out] */
                  cds_Name_t       *class_p,		/* [in] */
                  cds_Version_t    *version_p,		/* [in] */
                  uuid_t           *uuid_p,		/* [in] */
                  cds_Timestamp_t  *acutal_ts_p,	/* [out] */
                  cds_status_t     *cds_status_p)	/* [out] */
{
	error_status_t	answ;

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " >cds_CreateObject"));

	/*
	 * initialize thread specific data 
	 */
	if (pthread_setspecific(thread_num,
			(pthread_addr_t)&cds_createobject_th_num))
	{
	    dce_svc_printf(CDS_S_PTHREADSETSPECIFIC_MSG, (long)errno);
	    error_exit_th(-1);
	}

	/*
	 * get the answer 
	 */
	answ = get_rpc_answer(CDS_CREATEOBJECT_INDEX,
			Progress_p, ET_object, cds_status_p);

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " <cds_CreateObject"));

	return(answ);
}

error_status_t 
cds_DeleteObject (handle_t        h,			/* [in] */
                  cds_Progress_t  *Progress_p,		/* [in, out] */
                  cds_status_t    *cds_status_p)	/* [out] */
{
	error_status_t	answ;

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " >cds_DeleteObject"));

	/*
	 * initialize thread specific data 
	 */
	if(pthread_setspecific(thread_num,
			(pthread_addr_t)&cds_deleteobject_th_num))
	{
	    dce_svc_printf(CDS_S_PTHREADSETSPECIFIC_MSG, (long)errno);
	    error_exit_th(-1);
	}

	/*
	 * get the answer 
	 */
	answ = get_rpc_answer(CDS_DELETEOBJECT_INDEX,
			Progress_p, ET_object, cds_status_p);

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " <cds_DeleteObject:"));

	return(answ);
}

error_status_t 
cds_DeleteSoftLink (handle_t        h,			/* [in] */
                    cds_Progress_t  *Progress_p,	/* [in, out] */
                    cds_status_t    *cds_status_p)	/* [out] */
{
	error_status_t	answ;

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " >cds_DeleteSoftLink:"));

	/*
	 * initialize thread specific data 
	 */
	if(pthread_setspecific(thread_num,
			(pthread_addr_t)&cds_deletesoftlink_th_num))
	{
	    dce_svc_printf(CDS_S_PTHREADSETSPECIFIC_MSG, (long)errno);
	    error_exit_th(-1);
	}

	/*
	 * get the answer 	
	 */
	answ = get_rpc_answer(CDS_DELETESOFTLINK_INDEX,
			Progress_p, ET_softlink, cds_status_p);

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " <cds_DeleteSoftlink"));

	return(answ);
}

error_status_t 
cds_DeleteDirectory (handle_t        h,			/* [in] */
                     cds_Progress_t  *Progress_p,	/* [in, out] */
                     cds_status_t    *cds_status_p)	/* [out] */
{
	error_status_t	answ;

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " >cds_DeleteDirectory"));

	/*
	 * initialize thread specific data 
	 */
	if(pthread_setspecific(thread_num,
			(pthread_addr_t)&cds_deletedirectory_th_num))
	{
	    dce_svc_printf(CDS_S_PTHREADSETSPECIFIC_MSG, (long)errno);
	    error_exit_th(-1);
	}

	/*
	 * get the answer 
	 */
	answ = get_rpc_answer(CDS_DELETEDIRECTORY_INDEX,
			Progress_p, ET_directory, cds_status_p);

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " <cds_DeleteDirectory"));

	return(answ);
}

/*
 * cds_EnumerateAttributes: an rpc interface routine 
 */

error_status_t 
cds_EnumerateAttributes (handle_t        h,		/* [in] */
                         cds_Progress_t  *Progress_p,	/* [in, out] */
                         ndr_usmall_int  type,		/* [in] */
                         cds_Name_t      *context_p,	/* [in] */
                         ndr_ulong_int   max_size,	/* [in] */
                         cds_SetP_t      *attr_set,	/* [in, out] */
                         ndr_usmall_int  *wholeset_p,	/* [out] */
                         cds_status_t    *cds_status_p)	/* [out] */
{
	error_status_t	answ;

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " >cds_EnumerateAttributes"));

	/*
	 * initialize thread specific data 
	 */
	if(pthread_setspecific(thread_num,
			(pthread_addr_t)&cds_enumerateattributes_th_num))
	{
	    dce_svc_printf(CDS_S_PTHREADSETSPECIFIC_MSG, (long)errno);
	    error_exit_th(-1);
	}

	/*
	 * get the answer 
	 */
	answ = get_rpc_answer(CDS_ENUMERATEATTRIBUTES_INDEX,
			Progress_p, type, cds_status_p);

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " <cds_EnumerateAttributes"));

	return(answ);
}

error_status_t 
cds_EnumerateChildren (handle_t        h,		/* [in] */
                       cds_Progress_t  *Progress_p,	/* [in, out] */
                       cds_Name_t      *wild_p,		/* [in] */
                       cds_Name_t      *context_p,	/* [in] */
                       ndr_ulong_int   max_size,	/* [in] */
                       cds_SetP_t      *name_set,	/* [in, out] */
                       ndr_usmall_int  *wholeset_p,	/* [out] */
                       cds_status_t    *cds_status_p)	/* [out] */
{
	error_status_t	answ;

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " >cds_EnumerateChildren"));

	/*
	 * initialize thread specific data 
	 */
	if(pthread_setspecific(thread_num,
			(pthread_addr_t)&cds_enumeratechildren_th_num))
	{
	    dce_svc_printf(CDS_S_PTHREADSETSPECIFIC_MSG, (long)errno);
	    error_exit_th(-1);
	}

	/*
	 * get the answer 
	 */
	answ = get_rpc_answer(CDS_ENUMERATECHILDREN_INDEX,
			Progress_p, ET_directory, cds_status_p);

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " <cds_EnumerateChildren"));

	return(answ);
}

error_status_t 
cds_EnumerateObjects (handle_t        h,		/* [in] */
                      cds_Progress_t  *Progress_p,	/* [in, out] */
                      cds_Name_t      *wild_p,		/* [in] */
                      cds_Name_t      *context_p,	/* [in] */
                      cds_Name_t      *class_p,		/* [in] */
                      ndr_ulong_int   max_size,		/* [in] */
                      cds_SetP_t      *name_set,	/* [in, out] */
                      ndr_usmall_int  *wholeset_p,	/* [out] */
                      ndr_usmall_int  *returnClass_p,	/* [in, out] */
                      cds_status_t    *cds_status_p)	/* [out] */
{
	error_status_t	answ;

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " >cds_Enumerateobjects"));

	/*
	 * initialize thread specific data 
	 */
	if(pthread_setspecific(thread_num,
			(pthread_addr_t)&cds_enumerateobjects_th_num))
	{
	    dce_svc_printf(CDS_S_PTHREADSETSPECIFIC_MSG, (long)errno);
	    error_exit_th(-1);
	}	

	/*
	 * get the answer 
	 */
	answ = get_rpc_answer(CDS_ENUMERATEOBJECTS_INDEX,
			Progress_p, ET_directory, cds_status_p);

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " <cds_EnumerateObjects"));

	return(answ);
}

error_status_t 
cds_EnumerateSoftLinks (handle_t        h,		/* [in] */
                        cds_Progress_t  *Progress_p,	/* [in, out] */
                        cds_Name_t      *wild_p,	/* [in] */
                        cds_Name_t      *context_p,	/* [in] */
                        ndr_ulong_int   max_size,	/* [in] */
                        cds_SetP_t      *name_set,	/* [in, out] */
                        ndr_usmall_int  *wholeset_p,	/* [out] */
                        cds_status_t    *cds_status_p)	/* [out] */
{
	error_status_t	answ;

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " >cds_EnumerateSoftLinks"));

	/*
	 * initialize thread specific data 
	 */
	if(pthread_setspecific(thread_num,
			(pthread_addr_t)&cds_enumeratesoftlinks_th_num))
	{
	    dce_svc_printf(CDS_S_PTHREADSETSPECIFIC_MSG, (long)errno);
	    error_exit_th(-1);
	}

	/*
	 * get the answer 
	 */
	answ = get_rpc_answer(CDS_ENUMERATESOFTLINKS_INDEX,
			Progress_p, ET_directory, cds_status_p);

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " <cds_EnumerateSoftLinks"));

	return(answ);
}

/*
 * cds_ModifyAttribute: an rpc interface routine 
 */

error_status_t 
cds_ModifyAttribute (handle_t        h,			/* [in] */
                     cds_Progress_t  *Progress_p,	/* [in, out] */
                     ndr_usmall_int  type,		/* [in] */
                     cds_Update_t    *update_p,		/* [in] */
                     cds_status_t    *cds_status_p)	/* [out] */
{
	error_status_t	answ;

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " >cds_ModifyAttrubute"));

	/*
	 * initialize thread specific data 
	 */
	if(pthread_setspecific(thread_num,
			(pthread_addr_t)&cds_modifyattribute_th_num))
	{
	    dce_svc_printf(CDS_S_PTHREADSETSPECIFIC_MSG, (long)errno);
	    error_exit_th(-1);
	}

	/*
	 * get the answer 
	 */
	answ = get_rpc_answer(CDS_MODIFYATTRIBUTE_INDEX,
			Progress_p, type, cds_status_p);

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " <cds_ModifyAttrubute"));

	return(answ);
}

/*
 * cds_ReadAttribute: an rpc interface routine 
 */

error_status_t 
cds_ReadAttribute (handle_t         h, 			/* [in] */
                   cds_Progress_t   *Progress_p,	/* [in, out] */
                   ndr_usmall_int   type,		/* [in] */
                   cds_Name_t       *att_p,		/* [in] */
                   cds_Timestamp_t  *context_p,		/* [in] */
                   ndr_ulong_int    max_size,		/* [in] */
                   ndr_ulong_int    maybemore,		/* [in] */
                   cds_RA_value_t   *value_p, 		/* [in, out] */
                   ndr_usmall_int   *wholeset_p, 	/* [out] */
                   cds_status_t     *cds_status_p)	/* [out] */
{
	error_status_t	answ;

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " >cds_ReadAttribute"));

	/*
	 * initialize thread specific data 
	 */
	if(pthread_setspecific(thread_num,
			(pthread_addr_t)&cds_readattribute_th_num))
	{
	    dce_svc_printf(CDS_S_PTHREADSETSPECIFIC_MSG, (long)errno);
	    error_exit_th(-1);
	}

	/*
	 * get the answer 
	 */
	answ = get_rpc_answer(CDS_READATTRIBUTE_INDEX,
			Progress_p, type, cds_status_p);

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " <cds_ReadAttribute"));

	value_p->returningToClerk = RA_none;
	return(answ);
}

error_status_t 
cds_ResolveName (handle_t        h,		/* [in] */
                 cds_Progress_t  *Progress_p,	/* [in, out] */
                 cds_status_t    *cds_status_p)	/* [out] */
{
	error_status_t	answ;

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " >cds_ResolveName"));

	/*
	 * initialize thread specific data 
	 */
	if(pthread_setspecific(thread_num,
			(pthread_addr_t)&cds_resolvename_th_num))
	{
	    dce_svc_printf(CDS_S_PTHREADSETSPECIFIC_MSG, (long)errno);
	    error_exit_th(-1);
	}

	/*
	 * get the answer 
	 */
	answ = get_rpc_answer(CDS_RESOLVENAME_INDEX,
			Progress_p, ET_softlink, cds_status_p);

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " <cds_ResolveName"));

	return(answ);
}

error_status_t 
cds_Skulk (handle_t        h,			/* [in] */
           cds_Progress_t  *Progress_p,		/* [in, out] */
           cds_status_t    *cds_status_p)	/* [out] */
{
	error_status_t	answ;

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " >cds_Skulk"));

	/*
	 * initialize thread specific data 
	 */
	if(pthread_setspecific(thread_num,
			(pthread_addr_t)&cds_skulk_th_num))
	{
	    dce_svc_printf(CDS_S_PTHREADSETSPECIFIC_MSG, (long)errno);
	    error_exit_th(-1);
	}

	/*
	 * get the answer 
	 */
	answ = get_rpc_answer(CDS_SKULK_INDEX,
			Progress_p, ET_directory, cds_status_p);

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " <cds_Skulk"));

	return(answ);
}

/*
 * cds_TestAttribute: an rpc interface routine 
 */

error_status_t 
cds_TestAttribute (handle_t           h,		/* [in] */
                   cds_Progress_t     *Progress_p,	/* [in, out] */ 
                   ndr_usmall_int     type,		/* [in] */
                   cds_Name_t         *att_p,		/* [in] */
                   cds_AtomicValue_t  *value_p,		/* [in] */
                   ndr_usmall_int     *result_p,	/* [out] */
                   cds_status_t       *cds_status_p)	/* [out] */
{
	error_status_t	answ;

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " >cds_TestAttrubute"));

	/*
	 * initialize thread specific data 
	 */
	if(pthread_setspecific(thread_num,
			(pthread_addr_t)&cds_testattribute_th_num))
	{
	    dce_svc_printf(CDS_S_PTHREADSETSPECIFIC_MSG, (long)errno);
	    error_exit_th(-1);
	}

	/*
	 * get the answer 
	 */
	answ = get_rpc_answer(CDS_TESTATTRIBUTE_INDEX,
			Progress_p, type, cds_status_p);

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " <cds_TestAttrubute"));

      return(answ);
}

/*
 * cds_TestGroup: an rpc interface routine 
 */

error_status_t 
cds_TestGroup (handle_t        h,		/* [in] */
               cds_Progress_t  *Progress_p,	/* [in, out] */
               cds_FullName_t  *member_p,	/* [in] */
               uuid_t          *loop_p,		/* [in, out] */
               ndr_usmall_int  *direct_p,	/* [in, out] */
               ndr_usmall_int  *result_p,	/* [out] */
               cds_TimeoutP_t  *outTimeout_p,	/* [out] */
               cds_status_t    *cds_status_p)	/* [out] */
{
	error_status_t	answ;

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " >cds_TestGroup"));

	/*
	 * initialize thread specific data 
	 */
	if(pthread_setspecific(thread_num,
			(pthread_addr_t)&cds_testgroup_th_num))
	{
	    dce_svc_printf(CDS_S_PTHREADSETSPECIFIC_MSG, (long)errno);
	    error_exit_th(-1);
	}

	/*
	 * get the answer 
	 */
	answ = get_rpc_answer(CDS_TESTGROUP_INDEX,
			Progress_p, ET_object, cds_status_p);

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " <cds_TestGroup"));

	return(answ);
}

error_status_t 
cds_AllowClearinghouses (handle_t        h, 		/* [in] */
                         cds_Progress_t  *Progress_p,	/* [in, out] */
                         cds_status_t    *cds_status_p)	/* [out] */
{
	error_status_t	answ;

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " >cds_AllowClearinghouses"));

	/*
	 * initialize thread specific data 
	 */
	if(pthread_setspecific(thread_num,
			(pthread_addr_t)&cds_allowclearinghouses_th_num))
	{
	    dce_svc_printf(CDS_S_PTHREADSETSPECIFIC_MSG, (long)errno);
	    error_exit_th(-1);
	}

	/*
	 * get the answer 
	 */
	answ = get_rpc_answer(CDS_ALLOWCLEARINGHOUSES_INDEX,
			Progress_p, ET_directory, cds_status_p);

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " <cds_AllowClearinghouses"));

	return(answ);
}

error_status_t 
cds_DisallowClearinghouses (handle_t        h,			/* [in] */
                            cds_Progress_t  *Progress_p,	/* [in, out] */
                            cds_status_t    *cds_status_p)	/* [out] */
{
	error_status_t	answ;

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " >cds_DisallowClearinghouses"));

	/*
	 * initialize thread specific data 
	 */
	if(pthread_setspecific(thread_num,
			(pthread_addr_t)&cds_disallowclearinghouses_th_num))
	{
	    dce_svc_printf(CDS_S_PTHREADSETSPECIFIC_MSG, (long)errno);
	    error_exit_th(-1);
	}

	/*
	 * get the answer 
	 */
	answ = get_rpc_answer(CDS_DISALLOWCLEARINGHOUSES_INDEX,
			Progress_p, ET_directory, cds_status_p);

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " <cds_DisallowClearinghouses"));

	return(answ);
}

error_status_t 
cds_DoUpdate (handle_t         h,		/* [in] */
              uuid_t           *epoch_p,
              cds_UpdatePkt_t  *updates_p,
              cds_status_t     *cds_status_p)
{
      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " >cds_DoUpdate"));

      /*
       * initialize thread specific data 
       */
      if(pthread_setspecific(thread_num,
			(pthread_addr_t)&cds_doupdate_th_num))
      {
	    dce_svc_printf(CDS_S_PTHREADSETSPECIFIC_MSG, (long)errno);
	    error_exit_th(-1);
	}

      cds_status_p->er_name = (cds_FullName_t *)NULL;
      cds_status_p->er_status = PDU_NotImplemented;

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " <cds_DoUpdate"));

      return(rpc_s_ok);
}

error_status_t 
cds_Combine (handle_t         h,		/* [in] */
             uuid_t           *dir_p,
             cds_Timestamp_t  *skulk_p,
             cds_Timestamp_t  *allupto_p,
             uuid_t           *epoch_p,
             cds_FullName_t   *to_p,
             uuid_t           *next_p,
             cds_status_t     *cds_status_p)
{
      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " >cds_Combine"));

      /*
       * initialize thread specific data 
       */
	if(pthread_setspecific(thread_num,
			(pthread_addr_t)&cds_combine_th_num))
	{
	    dce_svc_printf(CDS_S_PTHREADSETSPECIFIC_MSG, (long)errno);
	    error_exit_th(-1);
	}

      cds_status_p->er_name = (cds_FullName_t *)NULL;
      cds_status_p->er_status = PDU_NotImplemented;

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " <cds_Combine"));

      return(rpc_s_ok);
}

error_status_t
cds_LinkReplica (handle_t        h,		/* [in] */
                 cds_Progress_t  *progress_p,
                 uuid_t          *directory_p,
                 cds_Update_t    *update_p,
                 uuid_t          *epoch_p,
                 uuid_t          *ring_p,
                 cds_Version_t   *rpversion_p,
                 cds_status_t    *cds_status_p)
{
	error_status_t	answ;

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " >cds_LinkReplica"));

	/*
	 * initialize thread specific data 
	 */
	if(pthread_setspecific(thread_num,
			(pthread_addr_t)&cds_linkreplica_th_num))
	{
	    dce_svc_printf(CDS_S_PTHREADSETSPECIFIC_MSG, (long)errno);
	    error_exit_th(-1);
	}

	/* get the answer */
	answ = get_rpc_answer(CDS_LINKREPLICA_INDEX,
			progress_p, ET_directory, cds_status_p);

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " <cds_LinkReplica"));

	return(answ);
}

error_status_t 
cds_ModifyReplica (handle_t      h,
                   uuid_t        *dir_p,
                   cds_Update_t  *update_p,
                   cds_status_t  *cds_status_p)
{
        DCE_SVC_DEBUG((
           cds__svc_handle,
           cds_svc_gda,
           svc_c_debug2,
           " >cds_ModifyReplica"));

        /*
	 * initialize thread specific data 
	 */
        if(pthread_setspecific(thread_num,
			(pthread_addr_t)&cds_modifyreplica_th_num))
	{
	    dce_svc_printf(CDS_S_PTHREADSETSPECIFIC_MSG, (long)errno);
	    error_exit_th(-1);
        }

        cds_status_p->er_name = (cds_FullName_t *)NULL;
        cds_status_p->er_status = PDU_NotImplemented;

        DCE_SVC_DEBUG((
           cds__svc_handle,
           cds_svc_gda,
           svc_c_debug2,
           " <cds_ModifyReplica"));

	return(rpc_s_ok);
}

/*
 * cds_AddReplica: an rpc interface routine 
 */

error_status_t 
cds_AddReplica (handle_t        h,
                cds_FullName_t  *directory_p,
                ndr_usmall_int  rptype,
                cds_status_t    *cds_status_p)
{
      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " >cds_AddReplica"));

      /*
       * initialize thread specific data 
       */
	if(pthread_setspecific(thread_num,
			(pthread_addr_t)&cds_addreplica_th_num))
	{
	    dce_svc_printf(CDS_S_PTHREADSETSPECIFIC_MSG, (long)errno);
	    error_exit_th(-1);
	}

	cds_status_p->er_name = (cds_FullName_t *)NULL;
	cds_status_p->er_status = PDU_NotImplemented;

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " <cds_AddReplica"));

	return(rpc_s_ok);
}

error_status_t 
cds_RemoveReplica (handle_t        h,
                   cds_FullName_t  *directory_p,
                   cds_status_t    *cds_status_p)
{
      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " >cds_RemoveReplica"));

      /*
       * initialize thread specific data 
       */
	if(pthread_setspecific(thread_num,
			(pthread_addr_t)&cds_removereplica_th_num))
	{
	    dce_svc_printf(CDS_S_PTHREADSETSPECIFIC_MSG, (long)errno);
	    error_exit_th(-1);
	}

	cds_status_p->er_name = (cds_FullName_t *)NULL;
	cds_status_p->er_status = PDU_NotImplemented;

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " <cds_RemoveReplica"));

	return(rpc_s_ok);
}

error_status_t 
cds_NewEpoch (handle_t        h,
              cds_FullName_t  *directory_p,
              cds_Set_t       *readonlies_p,
              cds_Set_t       *secondaries_p,
              cds_status_t    *cds_status_p)
{
      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " >cds_NewEpoch"));

      /*
       * initialize thread specific data 
       */
	if(pthread_setspecific(thread_num,
			(pthread_addr_t)&cds_newepoch_th_num))
	{
	    dce_svc_printf(CDS_S_PTHREADSETSPECIFIC_MSG, (long)errno);
	    error_exit_th(-1);
	}

	cds_status_p->er_name = (cds_FullName_t *)NULL;
	cds_status_p->er_status = PDU_NotImplemented;

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " <cds_NewEpoch"));

	return(rpc_s_ok);
}

error_status_t 
cds_DeleteChild (handle_t        h,			/* [in] */
                 cds_Progress_t  *Progress_p,		/* [in, out] */
                 cds_status_t    *cds_status_p)		/* [out] */
{
	error_status_t	answ;

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " >cds_DeleteChild"));

	/*
	 * initialize thread specific data 
	 */
	if(pthread_setspecific(thread_num,
			(pthread_addr_t)&cds_deletechild_th_num))
	{
	    dce_svc_printf(CDS_S_PTHREADSETSPECIFIC_MSG, (long)errno);
	    error_exit_th(-1);
	}

	/*
	 * get the answer 
	 */
	answ = get_rpc_answer(CDS_DELETECHILD_INDEX,
			Progress_p, ET_childPointer, cds_status_p);

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " <cds_DeleteChild"));

	return(answ);
}

error_status_t 
cds_CreateChild (handle_t          h,			/* [in] */	
                 cds_Progress_t    *Progress_p,		/* [in, out] */
                 sec_id_foreign_t  *user_p,
                 uuid_t            *childID_p,
                 cds_Set_t         *rpset_p,
                 uuid_t            *parentID_p,
                 cds_status_t      *cds_status_p)
{
	error_status_t	answ;

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " >cds_CreateChild"));

	/*
	 * initialize thread specific data 
	 */
	if(pthread_setspecific(thread_num,
			(pthread_addr_t)&cds_createchild_th_num))
	{
	    dce_svc_printf(CDS_S_PTHREADSETSPECIFIC_MSG, (long)errno);
	    error_exit_th(-1);
	}

	/*
	 * get the answer 
	 */
	answ = get_rpc_answer(CDS_CREATECHILD_INDEX,
			Progress_p, ET_childPointer, cds_status_p);

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " <cds_CreateChild"));

	return(answ);
}

error_status_t 
cds_CreateDirectory (handle_t         h,
                     cds_Progress_t   *progress_p,
                     cds_Timestamp_t  *actualcts_p,
                     cds_status_t     *cds_status_p)
{
	error_status_t	answ;

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " >cds_CreateDirectory"));

	/*
	 * initialize thread specific data 
	 */
	if(pthread_setspecific(thread_num,
			(pthread_addr_t)&cds_createdirectory_th_num))
	{
	    dce_svc_printf(CDS_S_PTHREADSETSPECIFIC_MSG, (long)errno);
	    error_exit_th(-1);
	}

	/*
	 * get the answer 
	 */
	answ = get_rpc_answer(CDS_CREATEDIRECTORY_INDEX,
			progress_p, ET_directory, cds_status_p);

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " <cds_CreateDirectory"));

	return(answ);
}

static int 
print_objuid (int     indent,	/*IN indent level */
              char    *name_p, 	/*IN name of field */
              byte_t  *next_p,	/*IN data to interpret */
              int     len_left,	/* len of buffer */
              char    *buf)	/* buffer for printed data */
{
	unsigned32 status;
	unsigned_char_t *string;
	uuid_t uuid;
	int buf_len = 0;


      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " >print_objuid"));

	COPY_bytes(next_p, &uuid, sizeof(uuid));
	uuid_to_string(&uuid, &string, &status);
	if (status == uuid_s_ok) {
	    if(len_left -  strlen((char *)string) < 0)
	    {
		
		return(-1);
	    }
	    
	    memmove( buf + buf_len,(char *)string, strlen((char *)string));
	    len_left -= strlen((char *)string);
	    buf_len += strlen((char *)string);
		
	    rpc_string_free(&string, &status);

	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug2,
		" <print_objuid: return(buf_len = %d)",
		buf_len));

	    return(buf_len);
	}
	else
	{   
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug2,
		" <print_objuid: return(-1)"));

	    return(-1);
	}
	
}

static int 
print_rp (cds_ReplicaPointer_t  *rp_p,
          int                   len_left,
          char                  *buf)
{
	unsigned char string[100];  
	dns_full_name_t Iname;
	int tlen,len,cnt;
	int status;
	unsigned char *tower_p;
	int buf_len = 0;
	char	temp_buf[250];
	int	temp_len;


	DCE_SVC_DEBUG((
            cds__svc_handle,
	    cds_svc_gda,
	    svc_c_debug2,
	    " >print_rp"));

	if ((rp_p->rp_type < 1) || (rp_p->rp_type > 3))
	{
	    if(len_left - 26 < 0)
	    {
		DCE_SVC_DEBUG((
		    cds__svc_handle,
		    cds_svc_gda,
		    svc_c_debug2,
		    " <print_rp: return(-1)"));

		return(-1);
	    }
	    
	    memmove( buf + buf_len,"\t\t***INVALID REPLICA TYPE", 26);
	    len_left -= 26;
	    buf_len += 26;
	}

	/*
	 * Print Clearinghouse name 
	 */
	R_to_L_FullName(&rp_p->rp_chname, (FullName_t *)&Iname, 1);
	len = sizeof(string);
	status = dnsCvtCDSFullToStr(&Iname, string, &len);

	if (status != DNS_SUCCESS)
	{
	    if(len_left - 32 < 0)
	    {
		DCE_SVC_DEBUG((
		    cds__svc_handle,
		    cds_svc_gda,
		    svc_c_debug2,
		    " <print_rp: return(-1)"));

		return(-1);
	    }
	    
	    memmove(
		  buf + buf_len,"\t\t***INVALID CLEARINGHOUSE NAME", 32);
	    len_left -= 32;
	    buf_len += 32;
	}
	else
	{
	    sprintf(temp_buf, "\t\tclearinghouse name:\n\t\t\t%s", string);

	    if(len_left -  strlen(temp_buf) < 0)
	    {
		DCE_SVC_DEBUG((
		    cds__svc_handle,
		    cds_svc_gda,
		    svc_c_debug2,
		    " <print_rp: return(-1)"));

		return(-1);
	    }
	    
	    memmove( buf + buf_len,temp_buf, strlen(temp_buf));
	    len_left -= strlen(temp_buf);
	    buf_len += strlen(temp_buf);
	}

	/*
	 * Print clearinghouse uuid 
	 */
	if(len_left - 25 < 0)
	{
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug2,
		" <print_rp: return(-1)"));

	    return(-1);
	}
	
	memmove( buf + buf_len,"\t\tclearinghouse uuid:\n\t\t\t", 25);
	len_left -= 25;
	buf_len += 25;

	if((temp_len = print_objuid((int)0, (char *)0,
		(byte_t *)&rp_p->rp_chid, len_left,
		buf + buf_len)) == -1)
	{
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug2,
		" <print_rp: return(-1)"));

	    return(-1);
	}
	else
	{
	    len_left -= temp_len;
	    buf_len += temp_len;
	}

	if(len_left - 10 < 0)
	{
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug2,
		" <print_rp: return(-1)"));

	    return(-1);
	}
	
	memmove( buf + buf_len,"\n\t\ttowers:", 10);
	len_left -= 10;
	buf_len += 10;

	tower_p = rp_p->rp_towers;

	cnt = GETL16(tower_p);		/* No. of towers */

	for (/* cnt as assigned above */ ; cnt > 0; --cnt) 
	{
	    if(len_left - 4 < 0)
	    {
		DCE_SVC_DEBUG((
		    cds__svc_handle,
		    cds_svc_gda,
		    svc_c_debug2,
		    " <print_rp: return(-1)"));

		return(-1);
	    }
	    
	    memmove( buf + buf_len,"\n\t\t\t", 4);
	    len_left -= 4;
	    buf_len += 4;

	    tlen = GETL16(tower_p);	/* Binary tower length */
	    len = get_binding_string(tower_p, buf + buf_len) - 1;

	    /*
	     * String binding length less trailing NUL 
	     */
	    if (len <= 0)
	    {
		DCE_SVC_DEBUG((
		    cds__svc_handle,
		    cds_svc_gda,
		    svc_c_debug2,
		    " <print_rp: return(-1)"));

		return(-1);
	    }
	    else 
	    {
		len_left -= len;
		buf_len += len;
	    }
	    tower_p += tlen;
	}

	if(len_left - 1 < 0)
	{
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug2,
		" <print_rp: return(-1)"));

	    return(-1);
	}

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug2,
            " <print_rp: return(buf_len = %d)",
	    buf_len));

	return(buf_len);
}

static int 
print_progress (cds_Progress_t  *progress_p,
                cds_status_t    *cds_status,
                int             len_left,
                char            *buf)
{
	int len = 0;
	char temp_buf[250];
	int i;
	cds_SetMember_t *member_p;
	cds_Timestamp_t context;
	int temp_len;


	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug2,
            " >print_progress"));

	if (cds_status->er_status != 0 &&
		cds_status->er_status != DNS_UNKNOWNENTRY &&
		cds_status->er_status != DNS_UNDERSPECIFIEDNAME)
	{
	    if(len_left - 17 < 0)
	    {
		DCE_SVC_DEBUG((
		    cds__svc_handle,
		    cds_svc_gda,
		    svc_c_debug2,
		    " <print_progress: return(0)"));

		return(0);
	    }
	    
	    memmove( buf + len,"CDS error Status ", 17);
	    len_left -= 17;
	    len += 17;

	    sprintf(temp_buf, "%d",
		    cds_status->er_status);
	    if(len_left -  strlen(temp_buf) < 0)
	    {
		DCE_SVC_DEBUG((
		    cds__svc_handle,
		    cds_svc_gda,
		    svc_c_debug2,
		    " <print_progress: return(0)"));

		return(0);
	    }

	    memmove( buf + len,temp_buf, strlen(temp_buf));
	    len_left -= strlen(temp_buf);
	    len += strlen(temp_buf);
	
	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_gda,
                svc_c_debug2,
                " <print_progress: return(len = %d)",
                len));

	    return(len);
	}

	/*
	 * Print out returned Progress record 
	 */
	if(len_left - 10 < 0)
	{
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug2,
		" <print_progress: return(0)"));

	    return(0);
	}

	memmove( buf + len,"\tflags:\n\t\t", 10);
	len_left -= 10;
	len += 10;

	if (progress_p->pr_flags & 1)
	{
		if(len_left - 4 < 0)
		{
		    DCE_SVC_DEBUG((
			cds__svc_handle,
			cds_svc_gda,
			svc_c_debug2,
			" <print_progress: return(0)"));

		    return(0);
		}

		memmove( buf + len,"done", 4);
		len_left -= 4;
		len += 4;
	}
	if (progress_p->pr_flags & 2)
	{
		if(len_left - 2 < 0)
		{
		    DCE_SVC_DEBUG((
			cds__svc_handle,
			cds_svc_gda,
			svc_c_debug2,
			" <print_progress: return(0)"));

		    return(0);
		}
		memmove( buf + len,"up", 2);
		len_left -= 2;
		len += 2;
	}
	if (progress_p->pr_flags & 4)
	{
		if(len_left - 6 < 0)
		{
		    DCE_SVC_DEBUG((
			cds__svc_handle,
			cds_svc_gda,
			svc_c_debug2,
			" <print_progress: return(0)"));

		    return(0);
		}
		memmove( buf + len,"linked", 6);
		len_left -= 6;
		len += 6;
	}
	if (progress_p->pr_flags & 8)
	{
		if(len_left - 7 < 0)
		{
		    DCE_SVC_DEBUG((
			cds__svc_handle,
			cds_svc_gda,
			svc_c_debug2,
			" <print_progress: return(0)"));

		    return(0);
		}
		memmove( buf + len,"hitlink", 7);
		len_left -= 7;
		len += 7;
	}
	if (progress_p->pr_flags & 16)
	{
		if(len_left - 11 < 0)
		{
		    DCE_SVC_DEBUG((
			cds__svc_handle,
			cds_svc_gda,
			svc_c_debug2,
			" <print_progress: return(0)"));

		    return(0);
		}
		memmove( buf + len,"ignorestate", 11);
		len_left -= 11;
		len += 11;
	}

	if(len_left - 19 < 0)
	{
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug2,
		" <print_progress: return(0)"));

	    return(0);
	}
	memmove( buf + len,"\n\tresolved name:\n\t\t", 19);
	len_left -= 19;
	len += 19;
	
	if(len_left - progress_p->pr_resolved.fn_length < 0)
	{
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug2,
		" <print_progress: return(0)"));

	    return(0);
	}
	memmove( buf + len,progress_p->pr_resolved.fn_name,
		progress_p->pr_resolved.fn_length);
	len_left -= progress_p->pr_resolved.fn_length;
	len += progress_p->pr_resolved.fn_length;

	if(len_left - 21 < 0)
	{
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug2,
		" <print_progress: return(0)"));

	    return(0);
	}
	memmove( buf + len,"\n\tunresolved name:\n\t\t", 21);
	len_left -= 21;
	len += 21;
	
	if(len_left - progress_p->pr_unresolved.fn_length < 0)
	{
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug2,
		" <print_progress: return(0)"));

	    return(0);
	}
	memmove( buf + len,progress_p->pr_unresolved.fn_name,
		progress_p->pr_unresolved.fn_length);
	len_left -= progress_p->pr_unresolved.fn_length;
	len += progress_p->pr_unresolved.fn_length;

	if(len_left - 1 < 0)
	{
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug2,
		" <print_progress: return(0)"));

	    return(0);
	}
	memmove( buf + len,"", 1);
	len_left -= 1;
	len += 1;

	if (progress_p->pr_replicas_p != (cds_Set_t *)NULL) {
		if (progress_p->pr_replicas_p->set_type != 3)
		{
			sprintf(temp_buf, "\tunexpected set_type:\n\t\t%d",
				progress_p->pr_replicas_p->set_type);
			if(len_left -  strlen(temp_buf) < 0)
			{
			    DCE_SVC_DEBUG((
				cds__svc_handle,
				cds_svc_gda,
				svc_c_debug2,
				" <print_progress: return(0)"));

			    return(0);
			} 

			memmove( buf + len,temp_buf, strlen(temp_buf));
			len_left -= strlen(temp_buf);
			len += strlen(temp_buf);
		}

		/*
		 * display each replica 
		 */
		if(len_left -  11 < 0)
		{
		    DCE_SVC_DEBUG((
			cds__svc_handle,
			cds_svc_gda,
			svc_c_debug2,
			" <print_progress: return(0)"));

		    return(0);
		}
		memmove( buf + len,"\treplicas:", 11);
		len_left -= 11;
		len += 11;

		for (i=0; i<progress_p->pr_replicas_p->set_length; ++i) {
			member_p = &progress_p->pr_replicas_p->set_members[i];
			if (member_p->sm_flag != 1)
			{
				sprintf(temp_buf, "\t\tbad set member flag for member %d", i);
				if(len_left -  strlen(temp_buf) < 0)
				{
				    DCE_SVC_DEBUG((
					cds__svc_handle,
					cds_svc_gda,
					svc_c_debug2,
					" <print_progress: return(0)"));

				    return(0);
				}

				memmove( buf + len,temp_buf, strlen(temp_buf));
				len_left -= strlen(temp_buf);
				len += strlen(temp_buf);
			}

			if (memcmp(&member_p->sm_ts, &context, sizeof(context)) !=0)
			{
				sprintf(temp_buf, "\t\tnon-null member timestamp for member %d", i);
				if(len_left -  strlen(temp_buf) < 0)
				{
				    DCE_SVC_DEBUG((
					cds__svc_handle,
					cds_svc_gda,
					svc_c_debug2,
					" <print_progress: return(0)"));

				    return(0);
				}
				memmove( buf + len,temp_buf, strlen(temp_buf));
				len_left -= strlen(temp_buf);
				len += strlen(temp_buf);
			}

			if (member_p->sm_value.av_valuetype != VT_ReplicaPointer)
			{
				sprintf(temp_buf, "\t\tbad value type(%d) for member %d", member_p->sm_value.av_valuetype, i);

				if(len_left -  strlen(temp_buf) < 0)
				{
				    DCE_SVC_DEBUG((
					cds__svc_handle,
					cds_svc_gda,
					svc_c_debug2,
					" <print_progress: return(0)"));

				    return(0);
				}
				memmove( buf + len,temp_buf, strlen(temp_buf));
				len_left -= strlen(temp_buf);
				len += strlen(temp_buf);
			}
			else
			{
				if((temp_len = print_rp(
					member_p->sm_value.av_val.av_rp_p,
					len_left, buf + len)) == -1)
				{
				    DCE_SVC_DEBUG((
					cds__svc_handle,
					cds_svc_gda,
					svc_c_debug2,
					" <print_progress: return(0)"));

				    return(0);
				}
				else
				{
					len_left -= temp_len;
					len += temp_len;
				}
			}
		}
	}
	else
	{
		if(len_left - 15 < 0)
		{
		    DCE_SVC_DEBUG((
			cds__svc_handle,
			cds_svc_gda,
			svc_c_debug2,
			" <print_progress: return(0)"));

		    return(0);
		}
		memmove( buf + len,"\tNo replica set", 15);
		len_left -= 15;
		len += 15;
	}

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug2,
            " <print_progress: return(len = %d)",
	    len));

	return(len);
}
