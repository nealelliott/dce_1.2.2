/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: clerk_bind.c,v $
 * Revision 1.1.14.2  1996/02/18  19:25:25  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:08:53  marty]
 *
 * Revision 1.1.14.1  1995/12/08  15:03:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:37:40  root]
 * 
 * Revision 1.1.11.7  1994/08/17  21:53:04  jd
 * 	Took out the instrumentation.
 * 	[1994/08/17  21:51:17  jd]
 * 
 * Revision 1.1.11.6  1994/08/09  15:21:06  jd
 * 	Temporary instrumentation for 11566.
 * 	[1994/08/09  15:16:29  jd]
 * 
 * Revision 1.1.11.5  1994/08/03  18:59:17  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:10:29  mccann]
 * 
 * Revision 1.1.11.4  1994/06/23  18:28:39  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:53:23  mccann]
 * 
 * Revision 1.1.11.3  1994/06/09  16:07:29  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:52:09  devsrc]
 * 
 * Revision 1.1.11.2  1994/04/14  14:41:04  peckham
 * 	New module clerk_socket.c to isolate SVR4 named pipes.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	Remove msgblk.h, cds_event_init() and evp references.
 * 	Remove v1v2 code (v1 never used for cds).
 * 	Remove VMS and UNIX conditionals, VMS code.
 * 	Remove last vestige of clerk_trace().
 * 	[1994/04/11  19:35:34  peckham]
 * 
 * Revision 1.1.11.1  1994/03/12  21:56:10  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:49:29  peckham]
 * 
 * Revision 1.1.9.2  1993/07/29  18:16:55  jd
 * 	FIxed mis-bracketed if statement.
 * 	[1993/07/29  18:09:53  jd]
 * 
 * Revision 1.1.6.4  1992/12/30  12:40:15  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:25:36  zeliff]
 * 
 * Revision 1.1.6.3  1992/11/04  20:21:54  keegan
 * 	The clerk was deleting all cached binding handles when the login
 * 	context changed, even if they were in use.  Now, the clerk calls
 * 	a new function (free_inuse_cached_handle) instead, which marks
 * 	the handle for deletion, but doesn't decrement the use count.
 * 	Also added some related instrumentation.
 * 	[1992/11/04  19:42:26  keegan]
 * 
 * Revision 1.1.6.2  1992/09/29  19:08:36  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:10:05  weisman]
 * 
 * Revision 1.1.2.4  1992/05/01  19:56:10  keegan
 * 	If binding handle is unauthenticated, null out the login context in
 * 	the handle cache, so the login context doesn't get released more than
 * 	once.
 * 	[1992/05/01  18:24:09  keegan]
 * 
 * Revision 1.1.2.3  1992/03/22  21:18:04  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:06:51  weisman]
 * 
 * Revision 1.1.2.2  1992/01/22  23:01:17  melman
 * 	Adding changes from defect_fixes archive.
 * 	[1992/01/22  22:15:56  melman]
 * 
 * $EndLog$
 */
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
 *
 */
#include <dce/dce.h>
#include <dce/dcecdsmsg.h>
#include <dce/rpc.h>
#include <dce/dce_error.h>

#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <string.h>
#include <stdio.h>

#include <dce/dns_config.h>
#include <uc_child.h>
#include <uc_clerk.h>
#include <dce/rpc_cvt.h> 
#include <tower.h>

#ifdef DCE_SEC
#include <dce/dnsclerk.h>
#include <dns_cfg.h>
#include <dce/rpc.h>
#include <dce/sec_login.h>
#include <dce/rgybase.h>
#include <dce/binding.h>
#include <dce/dce_cf.h>
#include <dce/secidmap.h>
#include <dce/pgo.h>
#endif


/*
 *  Local function prototypes.
 */
static int 
get_secure_handle (thread_t *, 
		   rpc_binding_handle_t *, 
		   error_status_t *);

static handle_cache_t *
find_handle (handle_hdr_t *,
	     rpc_binding_handle_t);

static error_status_t 
do_sec_login (byte_t *,
              int,
              sec_login_handle_t *);

static void
get_sec_params(void);


#ifdef DCE_SEC

static error_status_t 
do_sec_login (byte_t *, 
	      int, 
	      sec_login_handle_t *);
#endif

/*
 * Turn a tower member into a binding handle
 * Error status maximized in th_p->clerkStatus
 */
int 
get_rpc_handle (rpc_binding_handle_t  *h_p,
                MemberValue_t         *member_p,
                ObjUID_t              *id_p,
                thread_t              *th_p,
                int                   *getout)
{
    error_status_t status = FALSE;
    error_status_t rpc_status;
    uuid_t binding_uuid;
    char bind_string[100];
    unsigned_char_t *string_binding;

#if defined(DCE_CDS_DEBUG)

    DEB_ASCII_BUF_uuid(IDBuf)

#endif

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug6,
        " >get_rpc_handle(%lx,%lx,%lx,%lx)",
        h_p,
        member_p,
        id_p,
        th_p));

    *getout=0;
    COPY_ObjUID(id_p, &binding_uuid);	  /* Align uuid for rpc routines */

    /* first try to use RPC to convert the complete tower for us.	*/

    rpc_tower_to_binding((unsigned char *)member_p->mv_value,h_p,&rpc_status);
    if (rpc_status != rpc_s_ok) 
    {
	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_child,
            svc_c_debug6,
            "get_rpc_handle: rpc_tower_to_binding rtd. %d", 
            rpc_status));
    }

    /*
     * If RPC fails because the tower was not an RPC tower, see if
     * it might be our own two-floor protocol tower... crock of ages... 
     */

    if ( rpc_status == rpc_s_not_rpc_tower ) 
    {
	if ( get_cds_binding_string ( member_p->mv_value, bind_string )) 
	{
	    rpc_binding_from_string_binding ((unsigned char *)bind_string,
					     h_p, &rpc_status);
	}
    }

    if (rpc_status == rpc_s_ok) 
    {
	
#if defined(DCE_CDS_DEBUG)

        rpc_binding_to_string_binding (*h_p, &string_binding,&rpc_status);

        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_child,
            svc_c_debug6,
            "get_rpc_handle: string binding: %s, uuid: %s", 
            string_binding, 
            deb_ascii_uuid(IDBuf, &binding_uuid))); 

        rpc_string_free (&string_binding, &rpc_status);

#endif

        rpc_binding_set_object (*h_p, &binding_uuid, &rpc_status);
        if (rpc_status == rpc_s_ok) 
	{
	    /*
	     * Setup for secure calls 
	     */
            status = get_secure_handle(th_p, h_p, &rpc_status);
	}
	else 
	{
	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_child,
                svc_c_debug6,
                "get_rpc_handle: rpc_binding_set_object rtd %d", 
                rpc_status));
	}
    }
    else 
    {
	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_child,
            svc_c_debug6,
            "get_rpc_handle: rpc_binding_from_string_binding(%s) rtd %d",
            bind_string, 
	    rpc_status));
    }

    /* Return most descriptive RPC error */
    if (rpc_status != rpc_s_ok)
    {
	maximize_rpc_error(th_p, rpc_status, getout);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug6,
        " <get_rpc_handle(status=%d, getout=%d)",
        status,
        *getout));

    return ( status );
}


#ifdef DCE_SEC
#define SERVER_GROUP_NAME "subsys/dce/cds-server"
#define LOCAL_CELL "/.:"

static pthread_once_t get_sec_params_once = pthread_once_init;

/*
 *
 * Get authentication, security parameters from config file,
 * if they exist.  The parameters obtained are:
 * 1) the authentication level (default = rpc_c_authn_level_default)
 * 2) the cds servers group name (default = subsys/dce/cds-server)
 *
 */

static unsigned32 authn_level = rpc_c_authn_level_default;
static char ClerkCellName[256];
static char group_name[256];


static void 
get_sec_params (void)
{
    char *handle, *p, *cellname;
    error_status_t st;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug8,
	" >get_sec_params"));

    /*
     * Get our cell and host name 
     */
    dce_cf_get_cell_name (&cellname, &st);
    if (st != error_status_ok) 
    {
        dce_svc_printf(CDS_S_CFGETCELLNAME_MSG, (long)st);
    }
    else 
    {
	strcpy (ClerkCellName, cellname);
        free (cellname);
    }

    handle = dns_cfgopen((char *)0, (char *)0);
    authn_level = (int)dns_cfgtol(handle, "security.authn_level",
			          rpc_c_authn_level_default);

    if ((p = dns_cfgtostr(handle, "security.server_group_name", (char *)0))
	!= NULL) 
    {
	strcpy (group_name, p);
    }
    else 
    {
	strcpy (group_name, SERVER_GROUP_NAME);
    }

    dns_cfgclose (handle); /* close cds.conf */

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug8,
	" <get_sec_params"));
}
#endif


/*
 * Get and setup authentication info. for binding handle.  
 * Check out server if client requests it
 */
static int 
get_secure_handle (thread_t              *th_p,
                   rpc_binding_handle_t  *h_p,
                   error_status_t        *rpc_status_p)
{

#ifndef DCE_SEC

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug6,
        " >get_secure_handle(%lx,%lx)"));

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug6,
        " <get_secure_handle: return(TRUE)"));

    return(TRUE);

#else

    sec_rgy_handle_t context, context_foreign, context_p;
    error_status_t ignore_status;
    unsigned_char_t *global_server_princ_name = NULL;
    unsigned_char_t server_princ_name[1025];
    unsigned_char_t server_cell_name[1025];
    sec_rgy_bind_auth_info_t auth_info;
    unsigned char       inq_error_string[dce_c_error_string_len];
    int                 inq_sts;



    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug6,
        " >get_secure_handle(%lx,%lx)",
        th_p,
        h_p));

    /* 
     * Register authentication info. Get parameters from config file,
     * if we haven't yet. If client requested unauthenticated access,
     * bypass this.
     */
    pthread_once (&get_sec_params_once, get_sec_params);

    if (UCP_EXTbyte(th_p->clerkReq.UnAuth)) 
    {

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_child,
            svc_c_debug6,
            " <get_secure_handle: return(TRUE)"));

	return(TRUE);
    }

    rpc_mgmt_inq_server_princ_name(*h_p, rpc_c_authn_dce_secret,
				   &global_server_princ_name, rpc_status_p);
    if (*rpc_status_p != rpc_s_ok) 
    {
	if (*rpc_status_p == rpc_s_comm_failure) 
	{
	    rpc_ep_resolve_binding(*h_p, cds_clerkserver_v1_0_c_ifspec, 
				   &ignore_status);
	    if (ignore_status != rpc_s_ok)
	    {
		*rpc_status_p = ignore_status;
	    }
	}

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_child,
            svc_c_debug6,
            " <get_secure_handle: rpc_mgmt_inq_server_princ_name failed - return(FALSE)"));

	return(FALSE);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug6,
        "  global server_princ_name: %s",
        global_server_princ_name));

    /* 
     * Get current login context 
     */

    *rpc_status_p = 
      do_sec_login(th_p->link_p->ll_opq_login_context+sizeof(field16),
		   EXT16(th_p->link_p->ll_opq_login_context),
		   &th_p->login_context);

    /* If cannot get context, get out of here */
    if (*rpc_status_p != error_status_ok) 
    {
	rpc_string_free(&global_server_princ_name, &ignore_status);

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_child,
            svc_c_debug6,
            " <get_secure_handle: could not get context - return(FALSE)"));

	return(FALSE);
    }


    rpc_binding_set_auth_info (*h_p, 
			       global_server_princ_name,
			       authn_level,
			       rpc_c_authn_dce_secret,
			       (rpc_auth_identity_handle_t)th_p->login_context,
			       rpc_c_authz_dce, 
			       rpc_status_p);
    
    if (*rpc_status_p != rpc_s_ok) 
    {
	rpc_string_free(&global_server_princ_name, &ignore_status);


        dce_svc_printf(CDS_S_RPCSETAUTHINFO_MSG, (long)*rpc_status_p);

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_child,
            svc_c_debug6,
            " <get_secure_handle: rpc_binding_set_auth_info failed - return(FALSE)"));
	

	return (FALSE);
    }

    /*
     * See if the server's principal name is registered in the group and
     * organization we expect. Bypass this if client requested total trust.
     */
    
    if (!UCP_EXTbyte(th_p->clerkReq.TrustAll)) 
    {
	/* 
	 * Bind to the local registry site so we can parse the cell name from
	 * the cds server's principal name.
	 * Use the client's login context for authentication with the security 
	 * server.
	 */
	auth_info.info_type = sec_rgy_bind_auth_dce;
	auth_info.tagged_union.dce_info.authn_level = authn_level;
	auth_info.tagged_union.dce_info.authn_svc   = rpc_c_authn_dce_secret;
	auth_info.tagged_union.dce_info.authz_svc   = rpc_c_authz_dce;
	auth_info.tagged_union.dce_info.identity    = th_p->login_context;
	sec_rgy_cell_bind ((unsigned char *)LOCAL_CELL, &auth_info,
			   &context, rpc_status_p);

	if (*rpc_status_p != error_status_ok) 
	{
	    /*
	     * Release dynamic memory 
	     */
	    if (global_server_princ_name)
	    {
	        rpc_string_free(&global_server_princ_name, &ignore_status);
	    }

            dce_svc_printf(CDS_S_SECRGYCELLBIND_MSG, 
			   (unsigned char *)LOCAL_CELL, (long)*rpc_status_p);

	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_child,
                svc_c_debug6,
		" <get_secure_handle: sec_rgy_cell_bind failed return(FALSE)"));
	    		
	    return (FALSE);
	}

	/*
	 * Strip off the global part of the server's principal name.
	 * This is necessary because the security server doesn't currently
	 * handle global names.
	 */
	sec_id_parse_name(context, global_server_princ_name,
			  server_cell_name, NULL,
			  server_princ_name, NULL, rpc_status_p);

	/*
	 * Release dynamic memory 
	 */
	rpc_string_free(&global_server_princ_name, &ignore_status);
	if (*rpc_status_p != error_status_ok) 
	{
            dce_svc_printf(CDS_S_SECIDPARSENAME_MSG, (long)*rpc_status_p);

	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_child,
                svc_c_debug6,
		" <get_secure_handle: rpc_string_free failed - return(FALSE)"));

	    return(FALSE);
	}

	/*
	 * If the cds server is in a different cell, then bind to its registry
	 * server.
	 */
	context_foreign = NULL;
	if (strcmp ((char *)server_cell_name, (char *)ClerkCellName) != 0)
	{
	    sec_rgy_cell_bind ((unsigned char *)server_cell_name, 
			       &auth_info,
			       &context_foreign, 
			       rpc_status_p);

	    if (*rpc_status_p != error_status_ok) 
	    {
                dce_svc_printf(CDS_S_SECRGYCELLBIND_MSG, server_cell_name, *rpc_status_p);

		DCE_SVC_DEBUG((
                    cds__svc_handle,
                    cds_svc_child,
                    svc_c_debug6,
	            " <get_secure_handle: sec_rgy_cell_bind no.2 failed - return(FALSE)")); 

		return (FALSE);
	    }
	    context_p = context_foreign;
	}
	else
	{
	    context_p = context;
	}

        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_child,
            svc_c_debug6,
            "  Check membership of '%s' in '%s'",
            server_princ_name,
            group_name));

   	if ((!sec_rgy_pgo_is_member(context_p, 
  				   sec_rgy_domain_group, 
  				   (unsigned char *)group_name,
  				   server_princ_name,
  				   rpc_status_p) || 
	     (*rpc_status_p != error_status_ok))) 
	{
            DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_child,
                svc_c_debug6,
                "  get_secure_handle: SECURITY ERROR: Server principal: %s is not a member of group: %s, status: %ld",
                server_princ_name,
                group_name,
                *rpc_status_p));

	    if (*rpc_status_p != error_status_ok)
		dce_svc_printf(CDS_S_SECRGYPGOISMEMBER_MSG, 
			       (long)*rpc_status_p);
	    else
		dce_svc_printf(CDS_S_CHILD_SERVERNOTMEMBER_MSG, 
			       server_princ_name, group_name);
  
  	    sec_rgy_site_close (context, &ignore_status);
	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_child,
                svc_c_debug6,
	        " <get_secure_handle: sec_rgy_site_close failed - return(FALSE)"));

	    return (FALSE);
	}

	if (context_foreign)
	{
	    sec_rgy_site_close (context_foreign, &ignore_status);
	
	}

	sec_rgy_site_close (context, &ignore_status);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug6,
        " <get_secure_handle(): at end: return(TRUE)"));

    return(TRUE);

#endif  /* DCE_SEC */
}

/*
 * Get the best rpc error to return
 */
void 
maximize_rpc_error (thread_t        *th_p,
                    error_status_t  rpcStatus,
                    int             *getout)
{
    dns_status_t oldstatus;


    oldstatus = th_p->clerkStatus;
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug8,
        " >maximize_rpc_error(%d, %d)",
        oldstatus,
        rpcStatus));

    /* 
     * Check for errors that won't be relieved by trying different 
     * towers or clearinghouses.
     */
    *getout = 0;

    switch (rpcStatus) 
    {
	  case (rpc_s_auth_tkt_expired):
	  case (sec_rgy_server_unavailable):
	  *getout = 1;  /* signal caller to give up */
    } /* end switch */

    /*
     * Anything is better then these 
     */
    if ((oldstatus == DNS_SUCCESS) || (oldstatus == DNS_NOCOMMUNICATION) ||
	(oldstatus == DNS_UNKNOWNENTRY) || (oldstatus == rpc_s_comm_failure)) 
    {
	th_p->clerkStatus = rpcStatus;

	DCE_SVC_DEBUG((
           cds__svc_handle,
           cds_svc_child,
           svc_c_debug8,
	   " <maximize_rpc_error: oldstatus no good"));

        return;
    }

    /*
     * Want to know if node is down so clerk_find_ch can be called
     * intelligently
     */
    if ((rpcStatus == ept_s_not_registered) ||
	(rpcStatus == rpc_s_endpoint_not_found))
    {
	th_p->clerkStatus = rpcStatus;
    }
    
    /*	
     * Keep oldstatus around 
     */
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug8,
        " <maximize_rpc_error(%d,%d)",
        th_p->clerkStatus,
        *getout));

    return;
}



#define REMOVE_HANDLE(cache_p) \
{ \
  cache_p->hdr.prev_p->hdr.next_p = cache_p->hdr.next_p; \
  cache_p->hdr.next_p->hdr.prev_p = cache_p->hdr.prev_p;\
}


#define INSERT_HANDLE(hdr_p, cache_p) \
{ \
  cache_p->hdr.next_p = (handle_cache_t *)((hdr_p)->next_p); \
  cache_p->hdr.prev_p = (handle_cache_t *)(hdr_p); \
  (hdr_p)->next_p->hdr.prev_p = cache_p; \
  (hdr_p)->next_p = cache_p; \
}



/*
 * delete a cached handle
 */
void 
delete_cache_handle (link_t          *link_p,
                     handle_cache_t  *cache_p)
{
    unsigned32 rpcStatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug8,
        " >delete_cache_handle()"));

#ifdef DCE_SEC

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug8,
        "UnAuth=%d,TrustAll=%d,rpc_hdl:%lx,login_hdl:%lx",
        cache_p->UnAuth,
        cache_p->TrustAll,
        cache_p->h,
        cache_p->login_context));

#endif
    /*	
     * Remove from linked list 
     */

    REMOVE_HANDLE(cache_p);

    /*
     * Free RPC and CDS memory 
     */
    rpc_binding_free(&cache_p->h, &rpcStatus);

    /*	
     * release prior context, if any 
     */

#ifdef DCE_SEC

    /*
     * Note: don't release the opaque (exported) login context here,
     * because we reimport it when we create a new rpc binding
     * (i.e. on comm errors).
     * Release it when the connection to the client is released (link_free).
     */
    if (cache_p->login_context) 
    {
	sec_login_release_context(&cache_p->login_context, &rpcStatus);
    }
    
#endif

    dns_free((char *)cache_p);

    /*
     * Update count 
     */
    --link_p->ll_binding_count;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug8,
        " <delete_cache_handle()"));
}


/* 
 * find the cached entry for a specific handle address 
 */
static handle_cache_t 
*find_handle (handle_hdr_t         *hdr_p,
              rpc_binding_handle_t  h)
{
    handle_cache_t *cache_p; 


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug8,
        " >find_handle"));

    cache_p = hdr_p->next_p;
    while ((handle_hdr_t *)cache_p != hdr_p) 
    {
	if (h == cache_p->h)
	{
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_child,
		svc_c_debug8,
		" <find_handle: return(cache_p)"));

	    return(cache_p);
	}
	cache_p = cache_p->hdr.next_p;
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug8,
        " <find_handle: at end: return NULL"));

    return((handle_cache_t *)NULL);
}  


/*
 * A cached handle is not longer useful.  Free it and its resources.
 */
void 
free_cached_handle (thread_t              *th_p,
                    rpc_binding_handle_t  handle)
{
    handle_cache_t *cache_p;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug8,
        " >free_cached_handle()"));

    if ((thstatus = dthread_lock(&(th_p->link_p->ll_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

     /* Beginning of the 'protected code' */

    cache_p=find_handle(&th_p->link_p->ll_binding_hdr,handle);
    if (cache_p) 
    {
	/*
	 * If there are no other users of this handle, free it now 
	 */
	if (--cache_p->usecount == 0)
	{
	    delete_cache_handle(th_p->link_p, cache_p); 
	}
	else
	{
	    ++cache_p->killit;
	}
    }


    /* End of the 'protected code' */
    if ((thstatus = dthread_unlock(&(th_p->link_p->ll_mutex))) < 0)
    {
	dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }
      
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug8,
        " <free_cached_handle()"));
}

/*
 * A cached handle is not longer useful, but caller is not a "user" of it.
 * So, DON'T decrement the usecount, just mark if for deletion (or if
 * usecount is already 0, delete it)
 */
void 
free_inuse_cached_handle (thread_t        *th_p,
                          handle_cache_t  *cache_p)
{
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug8,
        " >free_inuse_cached_handle()"));

    if ((thstatus = dthread_lock(&(th_p->link_p->ll_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }


     /* Beginning of the 'protected code' */

    if(cache_p)
    {
	/*
	 * If there are no other users of this handle, free it now     
	 */
	if (cache_p->usecount == 0)
	{
            delete_cache_handle(th_p->link_p, cache_p); 
	}
	else
	{
	    /* 
	     * Otherwise, mark it to be deleted when no longer in use 
	     */
	    ++cache_p->killit;
	}
    }


    /* End of the 'protected code' */
    if ((thstatus = dthread_unlock(&(th_p->link_p->ll_mutex))) < 0)
    {
	dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }
      
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug8,
        " <free_inuse_cached_handle()"));
}


/*
 * Decrement cached handle use count
 */
void 
release_cached_handle (thread_t              *th_p,
                       rpc_binding_handle_t  handle)
{
    handle_cache_t *cache_p;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug8,
        " >release_cached_handle()"));

    if ((thstatus = dthread_lock(&(th_p->link_p->ll_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

     /* Beginning of the 'protected code' */

    cache_p = find_handle(&th_p->link_p->ll_binding_hdr, handle);
    if (cache_p) 
    {

#ifdef DCE_SEC

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_child,
            svc_c_debug8,
            "UnAuth=%d,TrustAll=%d,rpc_hdl:%lx,login_hdl:%lx",
            cache_p->UnAuth,
            cache_p->TrustAll,
            cache_p->h,
            cache_p->login_context));

#endif
	/*
	 * If there are no other users of this handle and it is marked to be
         * freed, to it now 
	 */
	if ((--cache_p->usecount == 0) && (cache_p->killit))
	{
	    delete_cache_handle(th_p->link_p, cache_p);
	}
    }

    /* End of the 'protected code' */
    if ((thstatus = dthread_unlock(&(th_p->link_p->ll_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug8,
        " <release_cached_handle()"));
}


/*
 * See if a clearinghouse already has a cached handle.  This routine
 * plays the same role as the decnet routine clerk_connected
 */
int 
is_handle_cached (thread_t  *th_p,
                  uuid_t    *uuid_p)
{
    handle_cache_t *cache_p;
    unsigned32     rpcStatus;
    int            fromcache = FALSE;
    uuid_t         handleid;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug8,
        " >is_handle_cached()"));

    if ((thstatus = dthread_lock(&(th_p->link_p->ll_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

     /* Beginning of the 'protected code' */

    /*	
     * Find a good handle
     */
    cache_p=th_p->link_p->ll_binding_hdr.next_p;

    while ((handle_hdr_t *)cache_p != &th_p->link_p->ll_binding_hdr) 
    {
        if (!cache_p->killit && 
	    (cache_p->TrustAll == UCP_EXTbyte(th_p->clerkReq.TrustAll)) &&
	    (cache_p->UnAuth == UCP_EXTbyte(th_p->clerkReq.UnAuth))) 
	{
	    rpc_binding_inq_object(cache_p->h, &handleid, &rpcStatus);
		if ((rpcStatus == rpc_s_ok) && 
		    (uuid_equal(uuid_p, &handleid, &rpcStatus))) 
		{
		    fromcache = TRUE;
		    break;
		}
	}
	cache_p = cache_p->hdr.next_p;
    }

    /* End of the 'protected code' */
    if ((thstatus = dthread_unlock(&(th_p->link_p->ll_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug8,
        " <is_handle_cached(%d)",
        fromcache));

    return(fromcache);
}


/*
 * Look for a cached handle to this clearinghouse 
 */
int 
get_cached_handle (rpc_binding_handle_t  *handle_p,
                   uuid_t                *uuid_p,
                   thread_t              *th_p)
{
    handle_cache_t *cache_p;
    unsigned32     rpcStatus;
    int            fromcache = FALSE;
    uuid_t         handleid;
    int 	   thstatus;

#if defined(DCE_CDS_DEBUG)

    DEB_ASCII_BUF_uuid(IDBuf)

#endif


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug7,
        " >get_cached_handle(%s)",
        deb_ascii_uuid(IDBuf,uuid_p)));

    if ((thstatus = dthread_lock(&(th_p->link_p->ll_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

     /* Beginning of the 'protected code' */

    /*
     *Find a good handle
     */
    cache_p=th_p->link_p->ll_binding_hdr.next_p;

    while ((handle_hdr_t *)cache_p != &th_p->link_p->ll_binding_hdr) 
    {
	if (!cache_p->killit && 
	    (cache_p->TrustAll == UCP_EXTbyte(th_p->clerkReq.TrustAll)) &&
	    (cache_p->UnAuth == UCP_EXTbyte(th_p->clerkReq.UnAuth))) 
	{
	    rpc_binding_inq_object(cache_p->h, &handleid, &rpcStatus);
	    if ((rpcStatus == rpc_s_ok) && 
	        (uuid_equal(uuid_p, &handleid, &rpcStatus))) 
	    {
                 ++cache_p->usecount;
		 /*
		  * Move to front of LRU list 
		  */
		 REMOVE_HANDLE(cache_p);
		 INSERT_HANDLE(&th_p->link_p->ll_binding_hdr, cache_p);
		 *handle_p = cache_p->h;
		 fromcache = TRUE;
		 break;
	     }
	}
	cache_p = cache_p->hdr.next_p;
    }

    /* End of the 'protected code' */
    if ((thstatus = dthread_unlock(&(th_p->link_p->ll_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }	

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug7,
        " <get_cached_handle(%s, %d)",
        deb_ascii_uuid(IDBuf,uuid_p),
        fromcache));

    return(fromcache);
}


/*
 * Cache a new handle
 */
void 
cache_handle (thread_t              *th_p,
              rpc_binding_handle_t  handle)
{
    handle_cache_t *cache_p,*prev_p,*last_p;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug8,
        " >cache_handle()"));

    /*
     * See if handle is already cached 
     */
    if ((cache_p = find_handle(&th_p->link_p->ll_binding_hdr, handle))) 
    {
        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_child,
            svc_c_debug8,	
            " <cache_handle: handle already cached: %lx", 
            cache_p));

	return;
    }
    cache_p = (handle_cache_t *)dns_malloc(sizeof(handle_cache_t));

    if (cache_p) 
    {
	cache_p->killit = 0;
	cache_p->usecount = 0;
	cache_p->h = handle;
	cache_p->TrustAll = UCP_EXTbyte(th_p->clerkReq.TrustAll);
	cache_p->UnAuth = UCP_EXTbyte(th_p->clerkReq.UnAuth);

#ifdef DCE_SEC

	if (!cache_p->UnAuth)
	{
	    /*
	     * Move from temporary place 
	     */
	    cache_p->login_context = th_p->login_context;
	}
	else 
	{
	    cache_p->login_context = NULL;
	}

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_child,
            svc_c_debug8,
            "UnAuth=%d,TrustAll=%d,rpc_hdl:%lx,login_hdl:%lx",
            cache_p->UnAuth,
            cache_p->TrustAll,
            cache_p->h,
	    cache_p->login_context));

#endif

	/*
	 * Insert into linked list 	
	 */

      if ((thstatus = dthread_lock(&(th_p->link_p->ll_mutex))) < 0)
      {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
      }

	/* Beginning of the 'protected code' */
	prev_p = th_p->link_p->ll_binding_hdr.prev_p;

	while (((handle_hdr_t *)prev_p != &th_p->link_p->ll_binding_hdr) &&
	       (th_p->link_p->ll_binding_count >= MAX_HANDLES)) 
	{
	    if (prev_p->usecount == 0) 
	    {
		last_p = prev_p->hdr.prev_p;
		delete_cache_handle(th_p->link_p, prev_p);
		prev_p = last_p;
	    } 
	    else
	    {
		prev_p = prev_p->hdr.prev_p;
	    }
	}
	INSERT_HANDLE(&th_p->link_p->ll_binding_hdr, cache_p);
	++th_p->link_p->ll_binding_count;

	/* End of the 'protected code' */
	if ((thstatus = dthread_unlock(&(th_p->link_p->ll_mutex))) < 0)
	{
        dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
	}
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug8,
        " <cache_handle()"));

}


#ifdef DCE_SEC

/* Do a secure login.  Convert the linearized login context to
 * internal format, and set our current context to it.
 * Since we don't now when context has change, we do this for
 * each new binding handle.
 */

static error_status_t 
do_sec_login (byte_t              *import_context,
              int                 import_length,
              sec_login_handle_t  *login_context_p)
{
    error_status_t sec_status;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug7,
        " >do_sec_login()"));


    sec_login_import_context(import_length, import_context,
	login_context_p, &sec_status);

    if (sec_status != error_status_ok) 
    {
        dce_svc_printf(CDS_S_SECIMPORTCTX_MSG, (long)sec_status);

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_child,
            svc_c_debug7,
            " <sec_login_set_context(): return (sec_status)"));

        return (sec_status);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug7,
        " <sec_login_set_context(): return (error_status_ok)"));

    return (error_status_ok);

}

#endif


