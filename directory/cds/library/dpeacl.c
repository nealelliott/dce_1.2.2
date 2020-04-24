/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dpeacl.c,v $
 * Revision 1.1.8.2  1996/02/18  19:34:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:14:59  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:26:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:47:11  root]
 * 
 * Revision 1.1.6.5  1994/08/25  19:30:45  proulx
 * 	Delegation support.
 * 	[1994/08/25  19:16:28  proulx]
 * 
 * Revision 1.1.6.4  1994/06/30  19:17:45  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:03:38  mccann]
 * 
 * Revision 1.1.6.3  1994/06/09  18:42:24  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:14:35  devsrc]
 * 
 * Revision 1.1.6.2  1994/04/29  15:55:19  peckham
 * 	Eliminate dependency on cds_events.c routines.
 * 	[1994/04/29  14:30:27  peckham]
 * 
 * Revision 1.1.6.1  1994/03/12  22:08:14  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:33:48  peckham]
 * 
 * Revision 1.1.4.3  1992/12/30  14:16:41  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:16:08  zeliff]
 * 
 * Revision 1.1.4.2  1992/12/07  13:18:46  zeliff
 * 	Removed duplicate OSF copyright markers
 * 	[1992/12/06  00:42:32  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  22:24:00  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:32:42  weisman]
 * 
 * Revision 1.1  1992/01/19  15:18:44  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*  Copyright (c) 1991 by
 *	Hewlett-Packard Company, Palo Alto, Ca. &
 *      Digital Equipment Corporation, Maynard, Mass.
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
 *  NAME:
 *
 *      dpeacl.c
 *
 *  FACILITY:
 *
 *      DPE acl library (libdpeacl.a)
 *
 *  ABSTRACT:
 *
 *  	This module contains routines that implement an acl manager for DPE
 *	applications that are DCE RPC servers.
 *
 */

#include <dce/dce.h>
#include <dce/dcecdsmsg.h>
#include <dce/dcesvcmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <pthread.h>
#include <stdio.h>
#include <dce/rpc.h>
#include <dce/dce_error.h>
#include <dce/dce_msg.h>
#include <dpeacl.h>
#include <dce/sec_cred.h>

/*
 * Function Prototype(s)
 */

#if defined(DCE_CDS_DEBUG)

static void 
print_pac(
	  sec_id_pac_t *);

#endif


/*
**++
**
**  ROUTINE NAME:	dpe_acl_client_is_authorized ()
**
**  SCOPE:		PUBLIC - declared in dpeacl.h
**
**  DESCRIPTION:
**
**	Checks the acl for a given RPC interface to see that the client
**	(described by the given RPC binding) has the desired access.  This
**	routine should be called by each RPC server routine before the
**	server function is performed.  This routine is also called by the
**	rdacl interface routines and other routines that implement the acl
**	manager for the interface.
**
**	We use the RPC binding handle to obtain the client's credentials info
**	(PAC).  We then check the acl provided to see if the requested access is
**	granted for the client.  If the client has no credentials, then we
**	assume that the client is unauthenticated and check access accordingly.
**
**	WARNING:
**		The acl passed to this routine must be locked for the duration
**		of the call if it can be modified by another thread (e.g. by a
**		call to rdacl_replace).
**	
**
**  INPUTS:
**
**	acl_p		the acl protecting the RPC interface the client is
**			calling.
**
**	client_handle	RPC binding handle for the client making the management
**			call.
**
**	desired_permset	mask representing the set of permissions requested on
**			behalf of the client - only those needed to perform the
**			server function that called this routine.
**
**  INPUTS/OUTPUTS:	none
**
**  OUTPUTS:
**
**	status		error_status_ok if we succeed.
**
**
**  IMPLICIT INPUTS:	none
**
**  IMPLICIT OUTPUTS:	none
**
**  FUNCTION VALUE:
**
**	is_authorized	is TRUE if the client is authorized for the requested
**			access, FALSE otherwise.
**
**  Side Effects:	none (I hope)
**
**--
**/
boolean32 
dpe_acl_client_is_authorized (sec_acl_p_t        acl_p,
                              handle_t           client_handle,
                              sec_acl_permset_t  desired_permset,
                              error_status_t     *status)
{
  sec_id_pac_t	         *client_pac_p;
  boolean32	          is_authorized = FALSE;
  sec_acl_permset_t       perms;
  
  rpc_authz_cred_handle_t cred_h;
    
  dce_acl_inq_client_creds(client_handle, &cred_h, status);
  if (status != error_status_ok) {
    return(is_authorized);
  }
  
  dce_acl_inq_permset_for_creds (cred_h, acl_p, NULL, NULL, 
				 sec_acl_posix_no_semantics,
				 &perms, status );  
  
  if ( (desired_permset & perms) == desired_permset ) {
    is_authorized = TRUE;
  }
  
  return(is_authorized);  

} /* end of routine dpe_acl_client_is_authorized */

#if defined(DCE_CDS_DEBUG)
/*
 * p r i n t _ p a c 
 */
static void 
print_pac (sec_id_pac_t *p)
{
    int                i;
    unsigned_char_p_t  uuid_string;
    error_status_t     lst;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
	" >print_pac"));
    
    if (p->realm.name) 
    {
	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_library,
            svc_c_debug5,
	    "print_pac: Realm:     %s", 
	    p->realm.name));
    }
    if (p->principal.name) {
	uuid_to_string(&(p->principal.uuid), &uuid_string, &lst);

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_library,
            svc_c_debug5,
	    "print_pac:	Principal: %s <%s>",
	    p->principal.name, uuid_string));

	rpc_string_free(&uuid_string, &lst);
    }
    if (p->group.name) 
    {
	uuid_to_string(&(p->group.uuid), &uuid_string, &lst);

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_library,
            svc_c_debug5,
	    "print_pac: Group: %s <%s>", 
	    p->group.name, 
	    uuid_string));

	rpc_string_free(&uuid_string, &lst);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug5,
	"print_pac: Local groupset :"));
    
    for (i = 0; i < p->num_groups; i++) 
    {
        if (p->groups[i].name) 
	{
	    uuid_to_string(&(p->groups[i].uuid), &uuid_string, &lst);

	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_library,
                svc_c_debug5,
                "print_pac: %s <%s>", 
		p->groups[i].name, 
	        uuid_string));

	    rpc_string_free(&uuid_string, &lst);
        }
    }
    
    for (i = 0; i < p->num_foreign_groups; i++) 
    {
	if (p->foreign_groups[i].realm.name) 
	{
	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_library,
                svc_c_debug5,
                "print_pac: %s", 
		p->foreign_groups[i].realm.name));
	}
	if (p->foreign_groups[i].id.name) 
	{
	    uuid_to_string(&(p->foreign_groups[i].id.uuid), 
			   &uuid_string, &lst);
	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_library,
                svc_c_debug5,
                "print_pac: %s <%s>", 
		p->foreign_groups[i].id.name, 
		uuid_string));

	    rpc_string_free(&uuid_string, &lst);
	}
    }
}
#endif /* DCE_CDS_DEBUG */
