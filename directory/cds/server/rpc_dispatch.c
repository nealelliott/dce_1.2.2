/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rpc_dispatch.c,v $
 * Revision 1.1.11.2  1996/02/18  19:36:25  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:16:59  marty]
 *
 * Revision 1.1.11.1  1995/12/08  15:31:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:33  root]
 * 
 * Revision 1.1.9.8  1994/10/04  15:31:40  zee
 * 	OT 12333: Remove calls to db_begin_operation and db_end_operation
 * 	in cp_AddCellname and cp_RemoveCellname.
 * 	[1994/10/03  21:03:04  zee]
 * 
 * Revision 1.1.9.7  1994/08/25  19:30:54  proulx
 * 	Delegation support.
 * 	[1994/08/25  19:24:49  proulx]
 * 
 * Revision 1.1.9.6  1994/08/03  19:04:25  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:27:08  mccann]
 * 
 * Revision 1.1.9.5  1994/06/23  19:11:04  jd
 * 	Remove evtblk. It no longer exists
 * 	[1994/06/23  15:41:24  jd]
 * 
 * Revision 1.1.9.4  1994/06/09  18:43:38  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:16:12  devsrc]
 * 
 * Revision 1.1.9.3  1994/05/06  16:04:23  zee
 * 	     HCell BL4 support: hcell namespace connection support.
 * 	[1994/05/05  21:20:32  zee]
 * 
 * Revision 1.1.9.2  1994/03/22  20:00:29  griffin
 * 	HCell BL3 support: Add add/remove cellname entry points.
 * 	[1994/03/22  18:08:56  griffin]
 * 
 * Revision 1.1.9.1  1994/03/12  22:12:06  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:44:25  peckham]
 * 
 * Revision 1.1.7.2  1992/12/30  14:22:47  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:23:15  zeliff]
 * 
 * Revision 1.1.2.7  1992/07/17  18:31:16  peckham
 * 	CR#2681: Avoid checkpoint deadly embrace.
 * 	   9-JUL-1992 22:50  by Dave Griffin
 * 	              Added support for distributed deadlock detection.
 * 	[1992/07/17  18:29:26  peckham]
 * 
 * Revision 1.1.2.6  1992/06/26  06:44:34  jim
 * 	 Merge AIX 3.2 changes with latest version.
 * 	[1992/06/23  15:41:45  jim]
 * 
 * Revision 1.1.2.5  1992/06/04  17:04:28  mfox
 * 	Fix bugs in instrumentation that potentially cause thread exceptions.
 * 	[1992/06/03  19:21:56  mfox]
 * 	Revision 1.1.3.2  1992/05/29  18:58:24  jim
 * 	Added include msgblk.h. This is not getting included on AIX 3.2
 * 	for some reason.
 * 
 * Revision 1.1.2.4  1992/04/16  18:24:23  peckham
 * 	OT 2517: Roll back cancellability if failure in get_user_ctx().
 * 	Extra credit: Release ud_resp_p if get_user_pac() fails in get_user_ctx().
 * 	Fix endian for status return if get_user_ctx() fails.
 * 	Pay attention to error return from rpc_binding_inq_object().
 * 	Export exception in cds_CreateChild() if get_user_ctx() fails.
 * 	[1992/04/14  18:55:53  peckham]
 * 
 * Revision 1.1.2.3  1992/04/14  20:51:51  mfox
 * 	Fixes to allow CDS to compile cleanly with RIOS ANSI C compiler
 * 	[1992/04/14  19:43:19  mfox]
 * 
 * Revision 1.1.2.2  1992/03/22  22:32:21  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:35:40  weisman]
 * 
 * Revision 1.1  1992/01/19  15:26:18  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: rpc_dispatch.c
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
 * MODULE DESCRIPTION:
 *
 * Networks & Communications Software Engineering
 *
 */ 

#include <server.h>
#include <dce/rpc.h>
#include <dbdef.h>
#include <find.h>
#include <tlog.h>
#include <dce/cds_clerkserver.h>
#include <dce/cprpc_hcell.h>
#include <hcell.h>
#include <dce/rpc_cvt.h>
#include <back.h>
#include <dce/dnsmanpar.h>

#if defined(DCE_SEC)
#include <dce/sec_cred.h>
/* #include <dce/rpcsts.h>*/
#endif

#ifndef	RC_ONE_ATTRIBUTE
#define	RC_ONE_ATTRIBUTE 1
#define	RC_WHOLE_ENTRY	2
#endif



extern struct nsgbl_ns dns_nameserver;


/*
 * Internal data structures
 */
typedef struct {
    user_descriptor_t	d_user;
    handle_t d_h_p;
    cds_visa_t		d_visa;
    FullName_t		d_name;
    int			d_op;
    int			d_cancel;
    longword_u		d_status;
} data_t;

static protocolVersion_u RPC_ProtocolVersion = {3,0,0,0};

/*
 * Local prototypes
 */

static int 
bldout (
    data_t		*,
    cds_Progress_t	*,
    int  		);

static int 
chk_EntryType (
      int    ,
      int    );

/*
 * static int
 * chk_Tower (
 *     byte_t *,
 *     int     );
 * (not currently used)
 * 
 * static int
 * chk_AtomicValue (
 *     cds_AtomicValue_t *,
 *     int                );
 * (not currently used)
 * 
 * static int
 * chk_ReplicaPointer (
 *     cds_ReplicaPointer_t *);
 * (not currently used)
 */
 
static dns_status_t
get_user_ctx (
     data_t		*,
     handle_t           ,
     int                );

static void
release_user_ctx (
     data_t		*);

void 
setup_return_status (
     cds_status_t	*);


/* Define these dummy entry points for OSF */
void 
trans_dispatcher (user_descriptor_t  *user_p,
                  byte_t             *spare_msg_p)
{
}
void 
back_dispatcher (user_descriptor_t  *user_p,
                 byte_t             *spare_msg_p)
{
}

/* dummy display for handle_t */

#define DEB_ASCII_BUF_rpc_binding_handle(nam)
#define deb_ascii_rpc_binding_handle(str,h) ""

#define DEB_ASCII_LEN_RPTYPE 42
#define DEB_ASCII_BUF_rptype(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_RPTYPE)

static char 
*deb_ascii_rptype (char  *str,
                   int   rptype)
{
    switch (rptype) {
    case RT_master:
	return("master");
    case RT_secondary:
	return("secondary");
    case RT_readOnly:
	return("readOnly");
#ifdef RT_gda
    case RT_gda:
	return("GDA");
#endif
    default:
	sprintf(str, "*bad ReplicaType(%d)*", rptype);
	return(str);
    }
}

#define DEB_ASCII_BUF_type(nam) DEB_ASCII_BUF_EntryType(nam)

static char 
*deb_ascii_type (char  *str,
                 int   type)
{
    byte_t et = type;

    return deb_ascii_EntryType(str, &et);
}

/*
 * Caller must call done user pac to free dynamicly allocated memory when
 * done
 */
error_status_t
server_get_client_creds (handle_t        client_handle,
			 rpc_authz_cred_handle_t *rpc_cred_h_p,
			 sec_id_pac_t    *client_pac_p,
			 boolean32       *auth_p) {
  boolean32 got_privs = TRUE; /* assume we can get them */
  rpc_authz_cred_handle_t temp_rpc_cred_h;
  error_status_t status;
  unsigned32 authn_level, authn_svc, authz_svc;
  

  DCE_SVC_DEBUG((
		 cds__svc_handle,
		 cds_svc_server,
		 svc_c_debug7,
		 " >server_get_client_creds"));
  
#ifdef DCE_SEC
  /* Inquire about client's authentication parameters and get the 
   * Privilege Attribute Certificate (PAC)
   */
  dce_acl_inq_client_creds (client_handle,
			    &temp_rpc_cred_h,
			    &status);
  
  if (status != rpc_s_ok) {
    dce_svc_printf(CDS_S_RPCBINDINGINQAUTH_MSG, (long)status);
    return (status);
  }

  if (dce_acl_is_unauthenticated (temp_rpc_cred_h)) {
    /* 
     * Unauthenticated RPC. 
     * Set defaults and construct an anonymous PAC 
     */
    DCE_SVC_DEBUG((cds__svc_handle,
		   cds_svc_server,
		   svc_c_debug7,
		   "  server_get_client_creds: Unauthenticated RPC"));
    got_privs = FALSE;
  }
      
  if ( rpc_cred_h_p != NULL )
    COPY_bytes (&temp_rpc_cred_h, rpc_cred_h_p, sizeof(temp_rpc_cred_h));

  /*
   * If authenticated, get the V1 PAC also
   */
  if (got_privs) {
    sec_id_pac_t *pac_p;
    sec_cred_pa_handle_t cred_h;
    
    cred_h = sec_cred_get_initiator (temp_rpc_cred_h, &status);
    if (status != error_status_ok) {
    DCE_SVC_DEBUG((cds__svc_handle,
		   cds_svc_server,
		   svc_c_debug5,
		   "  server_get_client_creds: get_initiator returns %d",
		   status));
      return (status);
    }
    pac_p = sec_cred_get_v1_pac (cred_h, &status);
    if (status != error_status_ok) {
    DCE_SVC_DEBUG((cds__svc_handle,
		   cds_svc_server,
		   svc_c_debug5,
		   "  server_get_client_creds: get_v1_pac returns %d",
		   status));
      return (status);
    }
    if ( client_pac_p != NULL )
      COPY_bytes (pac_p, client_pac_p, sizeof(sec_id_pac_t));
    
#ifdef DCE_CDS_DEBUG

    view_pac (client_pac_p);
    
#endif
    if ( auth_p != NULL )
      *auth_p = TRUE;
  }
  else 
    if ( auth_p != NULL )
      *auth_p = FALSE;
  
#endif
    
  DCE_SVC_DEBUG((
		 cds__svc_handle,
		 cds_svc_server,
		 svc_c_debug7,
		 " <server_get_client_creds: return(error_status_ok)"));
  
  return(error_status_ok);
}

/*
 * Fill in the user descriptor data structure with the 
 * pre-v1.1 pac, the v1.1 credentials, and an exists indicator
 */
#ifdef DCE_SEC
error_status_t
get_user_creds (handle_t           client_handle,
		user_descriptor_t  *user_p)
{
    error_status_t  status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " >get_user_creds"));
    
    status = server_get_client_creds (client_handle, 
				      &user_p->ud_cred,
				      &user_p->ud_pac,
				      &user_p->ud_is_pac);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " <get_user_creds: return(%d)", status));

    return(status);
}
#endif

void
view_pac(
	 sec_id_pac_t *pac_p)
{
  int                i;
  unsigned_char_p_t  uuid_string;
  char *name;
  error_status_t     lst;

  if (!(DCE_SVC_DEBUG_ATLEAST(cds__svc_handle, cds_svc_server, svc_c_debug5)))
      return;
  if (pac_p->realm.name) {
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        "Realm:     %s",
        pac_p->realm.name));
  }

  uuid_to_string(&(pac_p->principal.uuid), &uuid_string, &lst);
  name = (pac_p->principal.name) ? (char *)pac_p->principal.name : "";

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug5,
      "Principal: %s <%s>",
      name,
      uuid_string));

  rpc_string_free(&uuid_string, &lst);
  uuid_to_string(&(pac_p->group.uuid), &uuid_string, &lst);
  name = (pac_p->group.name) ? (char *)pac_p->group.name : "";

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug5,
      "Group    : %s <%s>",
      name,
      uuid_string));

  rpc_string_free(&uuid_string, &lst);

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug5,
      "Local groupset :"));
  
  for (i = 0; i < pac_p->num_groups; i++) {

    name = (pac_p->groups[i].name) ? (char *)pac_p->groups[i].name : "";

    uuid_to_string(&(pac_p->groups[i].uuid), &uuid_string, &lst);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        "     %s <%s>",
        name,
        uuid_string));
    rpc_string_free(&uuid_string, &lst);
  }
  
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug5,
      "Foreign groupset :"));
  
  for (i = 0; i < pac_p->num_foreign_groups; i++) {

    if (pac_p->foreign_groups[i].realm.name) {

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_server,
          svc_c_debug5,
          "     %s",
	  pac_p->foreign_groups[i].realm.name));
    }
    name = (pac_p->foreign_groups[i].id.name) ? (char *)pac_p->foreign_groups[i].id.name : "";
    uuid_to_string(&(pac_p->foreign_groups[i].id.uuid), &uuid_string, &lst);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        "     %s <%s>",
        name,
        uuid_string));
    rpc_string_free(&uuid_string, &lst);
  }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug5,
      " * * * * * * * * * * * * * *"));
}
/*
 * Initialize user descriptor
 */
static dns_status_t 
get_user_ctx (data_t    *data_p,
              handle_t  h_p,
              int       op)
{
    register user_descriptor_t *user_p = &data_p->d_user;
    unsigned32 rpc_status;
    dns_status_t status;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " >get_user_ctx"));

    ZERO_bytes(data_p, sizeof(*data_p));
    dthread_assign(dthread_null, &data_p->d_user.ud_mutex);
    data_p->d_cancel = dthread_setcancel(CANCEL_OFF);
    user_p->ud_ns_p = &dns_nameserver;
    COPY_FullName(NullFullName, &data_p->d_name);
    user_p->ud_protocol_p = (protocolVersion_t *)RPC_ProtocolVersion;
    user_p->ud_chuid_p = (ObjUID_t *)user_p->ud_Clearinghouse;
    rpc_binding_inq_object(h_p, (uuid_t *)user_p->ud_Clearinghouse, &rpc_status);
    if (rpc_status == rpc_s_ok) {
	user_p->ud_operation = op;
	user_p->ud_resp_p = (byte_t *)dns_malloc(NSMAXSIZE);
	if (user_p->ud_resp_p) {
	    user_p->ud_resp_end_p = SKIP_bytes(user_p->ud_resp_p, NSMAXSIZE);
	    user_p->ud_respLength = NSMAXSIZE;
	    user_p->ud_protocol_version =
		    EXT8(user_p->ud_protocol_p->pv_majorVersion);
	    if (get_user_creds(h_p, user_p) == error_status_ok) {
	      DCE_SVC_DEBUG((
			     cds__svc_handle,
			     cds_svc_server,
			     svc_c_debug7,
			     " <get_user_ctx: return(status=%d)",status));
	      return(DNS_SUCCESS);
	    } 
	    else
	      status = DNS_CANNOTAUTHENTICATE;

	    dns_free((char *)user_p->ud_resp_p);
	} else
	    status = DNS_NONSRESOURCES;
    } else
	status = rpc_status;

    dthread_setcancel(data_p->d_cancel);
    INS32(data_p->d_status, status);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " <get_user_ctx: return(status=%d)",status));

    return(status);
}

/*
 * Cleanup user context block
 */
static void 
release_user_ctx (data_t *data_p)
{
    user_descriptor_t *user_p = &data_p->d_user;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " >release_user_ctx"));

    cds_visa_destroy(&data_p->d_visa);

    if (user_p->ud_resp_p)
	dns_free((char *)user_p->ud_resp_p);

    dthread_setcancel(data_p->d_cancel);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " <release_user_ctx"));
}

/***************************************************************************/

/*
 * Setup return status as success
 */
void 
setup_return_status (cds_status_t *status_p)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " >setup_return_status"));

  status_p->er_status = PDU_Success;
  status_p->er_name = (cds_FullName_t *)0;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " <setup_return_status"));
}

/*      
 *  cds_DoUpdate
 *                                       
 * This routine dispatches a message to do_updates
 * It must take apart the message and pass the arguments to the routine
 * then after the call, it will assemble a return message
 */               
error_status_t 
cds_DoUpdate (handle_t         h,
              uuid_t           *epoch_p,
              cds_UpdatePkt_t  *updates_p,
              cds_status_t     *cds_status_p)
{
  DEB_ASCII_BUF_rpc_binding_handle(hBuf)
  DEB_ASCII_BUF_uuid(epochBuf)
  DEB_ASCII_BUF_cds_status(cds_statusBuf)
  data_t		data;
  int			status;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " >cds_DoUpdate(%s,%s,)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_uuid(epochBuf,epoch_p)));

  setup_return_status(cds_status_p);

  /* Convert RPC structures to local compresed buffer */
  status = get_user_ctx(&data, h, OP_DoUpdate);
  if (status == DNS_SUCCESS) {
      cdsImport_UpdatePkt(&data.d_visa,
		updates_p, (UpdatePacket_t *)data.d_user.ud_resp_p, FALSE);
      status = db_begin_operation(&data.d_user);
      if (status == DNS_SUCCESS) {
          status = do_updates(&data.d_user, epoch_p,
			(FullName_t *)data.d_user.ud_resp_p,
			(UpdatePacket_t *)SKIP_FullName(data.d_user.ud_resp_p), 
			LEN_UpdatePacket(data.d_user.ud_resp_p));
          (void)db_end_operation(&data.d_user);
      }
      status = bldout(&data, NULL, status);
      if (status != DNS_SUCCESS) 
	cdsExport_Exception(&data.d_visa, data.d_user.ud_resp_p, cds_status_p);
      release_user_ctx(&data);
    } else
      cdsExport_Exception(&data.d_visa, data.d_status, cds_status_p);

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " <cds_DoUpdate(%s,%s,) %s return(rpc_s_ok)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_uuid(epochBuf,epoch_p),
      deb_ascii_cds_status(cds_statusBuf,cds_status_p)));

  return(rpc_s_ok);
}

/*      
 *  cds_Combine
 *                                       
 * This routine dispatches a message to back_combine
 * It must take apart the message and pass the arguments to the routine
 * then after the call, it will assemble a return message.
 *
 */               
error_status_t 
cds_Combine (handle_t         h,
             uuid_t           *directory_p,
             cds_Timestamp_t  *skulk_p,
             cds_Timestamp_t  *allupto_p,
             uuid_t           *epoch_p,
             cds_FullName_t   *to_p,
             uuid_t           *ring_p,
             cds_status_t     *cds_status_p)
{
  DEB_ASCII_BUF_rpc_binding_handle(hBuf)
  DEB_ASCII_BUF_uuid(directoryBuf)
  DEB_ASCII_BUF_uuid(epochBuf)
  DEB_ASCII_BUF_cds_FullName(toBuf)
  DEB_ASCII_BUF_cds_status(cds_statusBuf)
  data_t		data;
  int			status;
  Timestamp_u		skulk, allupto;
  FullName_u		to;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " >cds_Combine(%s,%s,,,%s,%s,)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_uuid(directoryBuf,directory_p),
      deb_ascii_uuid(epochBuf,epoch_p),
      deb_ascii_cds_FullName(toBuf,to_p)));

  setup_return_status(cds_status_p);

  /* Convert RPC structures to local compresed buffer */
  status = get_user_ctx(&data, h, OP_Combine);
  if (status == DNS_SUCCESS) {
      cdsImport_Timestamp(&data.d_visa,
		skulk_p, (Timestamp_t *)skulk, FALSE);
      cdsImport_Timestamp(&data.d_visa,
		allupto_p, (Timestamp_t *)allupto, FALSE);
      cdsImport_FullName(&data.d_visa, to_p, (FullName_t *)to,  FALSE);
      status = db_begin_operation(&data.d_user);
      if (status == DNS_SUCCESS) {
          status = combine(&data.d_user, directory_p, (Timestamp_t *)skulk,
		       (Timestamp_t *)allupto, epoch_p, 
		       (FullName_t *)to, ring_p);
          (void)db_end_operation(&data.d_user);
      }
      status = bldout(&data, NULL, status);
      if (status != DNS_SUCCESS) 
	cdsExport_Exception(&data.d_visa, data.d_user.ud_resp_p, cds_status_p);
      release_user_ctx(&data);
    } else
      cdsExport_Exception(&data.d_visa, data.d_status, cds_status_p);

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " <cds_Combine(%s,%s,,,%s,%s,) %s return(rpc_s_ok)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_uuid(directoryBuf,directory_p),
      deb_ascii_uuid(epochBuf,epoch_p),
      deb_ascii_cds_FullName(toBuf,to_p),
      deb_ascii_cds_status(cds_statusBuf,cds_status_p)));
  
  return(rpc_s_ok);
}

/*      
 *  cds_LinkReplica
 *                                       
 * This routine dispatches a message to ta_LinkReplica
 * It must take apart the message and pass the arguments to the routine
 * then after the call, it will assemble a return message.
 *
 */               
error_status_t 
cds_LinkReplica (handle_t        h,
                 cds_Progress_t  *progress_p,
                 uuid_t          *directory_p,
                 cds_Update_t    *update_p,
                 uuid_t          *epoch_p,
                 uuid_t          *ring_p,
                 cds_Version_t   *rpversion_p,
                 cds_status_t    *cds_status_p)
{
  DEB_ASCII_BUF_rpc_binding_handle(hBuf)
  DEB_ASCII_BUF_cds_Progress(progressBuf)
  DEB_ASCII_BUF_uuid(directoryBuf)
  DEB_ASCII_BUF_cds_Update(updateBuf)
  DEB_ASCII_BUF_cds_status(cds_statusBuf)
  data_t		data;
  Update_u		update;
  byte_t		*msg_p;
  int			status;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " >cds_LinkReplica(%s,%s,%s,%s,,,)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_Progress(progressBuf,progress_p),
      deb_ascii_uuid(directoryBuf,directory_p),
      deb_ascii_cds_Update(updateBuf,update_p)));
  
  setup_return_status(cds_status_p);

  /* Convert RPC structures to local compresed buffer */
  status = get_user_ctx(&data, h, OP_LinkReplica);
  if (status == DNS_SUCCESS) {
    cdsImport_Progress(&data.d_visa,
		progress_p,(Progress_t *)data.d_user.ud_resp_p, FALSE);
    cdsImport_Update(&data.d_visa, update_p, (Update_t *)update, FALSE);

    status = db_begin_operation(&data.d_user);
    if (status == DNS_SUCCESS) {
       status = ta_link_replica(&data.d_user, &data.d_user,
			(Progress_t *)data.d_user.ud_resp_p,
			directory_p, (Update_t *)update,
			LEN_Update(update), epoch_p,
			ring_p, (version_t *)rpversion_p);
       (void)db_end_operation(&data.d_user);
    }
    status = bldout(&data, progress_p, status);
    if (status == DNS_SUCCESS) {
      msg_p = cdsExport_Progress(&data.d_visa,
		(Progress_t *)data.d_user.ud_resp_p, progress_p, cds_status_p);
    } else
      cdsExport_Exception(&data.d_visa, data.d_user.ud_resp_p, cds_status_p);
    release_user_ctx(&data);
  } else
    cdsExport_Exception(&data.d_visa, data.d_status, cds_status_p);
  
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " <cds_LinkReplica(%s,%s,%s,%s,,,) %s return(rpc_s_ok)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_Progress(progressBuf,progress_p),
      deb_ascii_uuid(directoryBuf,directory_p),
      deb_ascii_cds_Update(updateBuf,update_p),
      deb_ascii_cds_status(cds_statusBuf,cds_status_p)));

  return(rpc_s_ok);
}

/*      
 *  cds_ModifyReplica
 *                                       
 * This routine dispatches a message to ta_modify_replica
 * It must take apart the message and pass the arguments to the routine
 * then after the call, it will assemble a return message.
 *
 */               
error_status_t 
cds_ModifyReplica (handle_t      h,
                   uuid_t        *directory_p,
                   cds_Update_t  *update_p,
                   cds_status_t  *cds_status_p)
{
  DEB_ASCII_BUF_rpc_binding_handle(hBuf)
  DEB_ASCII_BUF_uuid(directoryBuf)
  DEB_ASCII_BUF_cds_Update(updateBuf)
  DEB_ASCII_BUF_cds_status(cds_statusBuf)
  data_t		data;
  Update_u		update;
  int			status;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " >cds_ModifyReplica(%s,%s,%s)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_uuid(directoryBuf,directory_p),
      deb_ascii_cds_Update(updateBuf,update_p)));

  setup_return_status(cds_status_p);

  /* Convert RPC structures to local compresed buffer */
  status = get_user_ctx(&data, h, OP_ModifyReplica);
  if (status == DNS_SUCCESS) {
      cdsImport_Update(&data.d_visa, update_p, (Update_t *)update,  FALSE);
      status = db_begin_operation(&data.d_user);
      if (status == DNS_SUCCESS) {
          status = ta_modify_replica(&data.d_user, directory_p, 
				 (Update_t *)update, LEN_Update(update));
          (void)db_end_operation(&data.d_user);
      }

      status = bldout(&data, NULL, status);
      if (status != DNS_SUCCESS) 
	cdsExport_Exception(&data.d_visa, data.d_user.ud_resp_p, cds_status_p);
      release_user_ctx(&data);
    } else
      cdsExport_Exception(&data.d_visa, data.d_status, cds_status_p);

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " <cds_ModifyReplica(%s,%s,%s) %s return(rpc_s_ok)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_uuid(directoryBuf,directory_p),
      deb_ascii_cds_Update(updateBuf,update_p),
      deb_ascii_cds_status(cds_statusBuf,cds_status_p)));

  return(rpc_s_ok);
}

/*      
 *  cds_AddReplica
 *                                       
 * This routine dispatches a message to ta_addreplica
 * It must take apart the message and pass the arguments to the routine
 * then after the call, it will assemble a return message.
 *
 */               
error_status_t 
cds_AddReplica (handle_t        h,
                cds_FullName_t  *directory_p,
                ndr_usmall_int  rptype,
                cds_status_t    *cds_status_p)
{
  DEB_ASCII_BUF_rpc_binding_handle(hBuf)
  DEB_ASCII_BUF_cds_FullName(directoryBuf)
  DEB_ASCII_BUF_rptype(rptypeBuf)
  DEB_ASCII_BUF_cds_status(cds_statusBuf)
  data_t		data;
  FullName_u		directory;
  int			status;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " >cds_AddReplica(%s,%s,%s)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_FullName(directoryBuf,directory_p),
      deb_ascii_rptype(rptypeBuf,rptype)));

  setup_return_status(cds_status_p);

  /* Convert RPC structures to local compresed buffer */
  status = get_user_ctx(&data, h, OP_AddReplica);
  if (status == DNS_SUCCESS) {
      cdsImport_FullName(&data.d_visa,
		directory_p, (FullName_t *)directory, FALSE);
      status = db_begin_operation(&data.d_user);
      if (status == DNS_SUCCESS) {
          status = ta_replica_add(&data.d_user, (FullName_t *)directory, rptype);
          (void)db_end_operation(&data.d_user);
      }

      status = bldout(&data, NULL, status);
      if (status != DNS_SUCCESS)
	cdsExport_Exception(&data.d_visa, data.d_user.ud_resp_p, cds_status_p);
      release_user_ctx(&data);
    } else
      cdsExport_Exception(&data.d_visa, data.d_status, cds_status_p);

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " <cds_AddReplica(%s,%s,%s) %s return(rpc_s_ok)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_FullName(directoryBuf,directory_p),
      deb_ascii_rptype(rptypeBuf,rptype),
      deb_ascii_cds_status(cds_statusBuf,cds_status_p)));

  return(rpc_s_ok);
}

/*      
 *  cds_RemoveReplica
 *                                       
 * This routine dispatches a message to ta_delete_replica
 * It must take apart the message and pass the arguments to the routine
 * then after the call, it will assemble a return message.
 *
 */               
error_status_t 
cds_RemoveReplica (handle_t        h,
                   cds_FullName_t  *directory_p,
                   cds_status_t    *cds_status_p)
{
  DEB_ASCII_BUF_rpc_binding_handle(hBuf)
  DEB_ASCII_BUF_cds_FullName(directoryBuf)
  DEB_ASCII_BUF_cds_status(cds_statusBuf)
  data_t		data;
  FullName_u		directory;
  int			status;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " >cds_RemoveReplica(%s,%s)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_FullName(directoryBuf,directory_p)));

  setup_return_status(cds_status_p);

  /* Convert RPC structures to local compresed buffer */
  status = get_user_ctx(&data, h, OP_RemoveReplica);
  if (status == DNS_SUCCESS) {
      cdsImport_FullName(&data.d_visa,
		directory_p, (FullName_t *)directory, FALSE);
      status = db_begin_operation(&data.d_user);
      if (status == DNS_SUCCESS) {
          status = ta_delete_replica(&data.d_user, (FullName_t *)directory);
          (void)db_end_operation(&data.d_user);
      }

      status = bldout(&data, NULL, status);
      if (status != DNS_SUCCESS)
	cdsExport_Exception(&data.d_visa, data.d_user.ud_resp_p, cds_status_p);
      release_user_ctx(&data);
    } else
      cdsExport_Exception(&data.d_visa, data.d_status, cds_status_p);

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " <cds_RemoveReplica(%s,%s) %s return(rpc_s_ok)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_FullName(directoryBuf,directory_p),
      deb_ascii_cds_status(cds_statusBuf,cds_status_p)));

  return(rpc_s_ok);
}

/*      
 *  cds_NewEpoch
 *                                       
 * This routine dispatches a message to ta_epoch
 * It must take apart the message and pass the arguments to the routine
 * then after the call, it will assemble a return message.
 *
 */               
error_status_t 
cds_NewEpoch (handle_t        h,
              cds_FullName_t  *directory_p,
              cds_Set_t       *readonlies_p,
              cds_Set_t       *secondaries_p,
              cds_status_t    *cds_status_p)
{
  DEB_ASCII_BUF_rpc_binding_handle(hBuf)
  DEB_ASCII_BUF_cds_FullName(directoryBuf)
  DEB_ASCII_BUF_cds_status(cds_statusBuf)
  data_t		data;
  FullName_u		directory;
  Set_t			*second_p, *read_p;
  int			status;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " >cds_NewEpoch(%s,%s,,)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_FullName(directoryBuf,directory_p)));
  
  setup_return_status(cds_status_p);

  /* Convert RPC structures to local compresed buffer */
  status = get_user_ctx(&data, h, OP_NewEpoch);
  if (status == DNS_SUCCESS) {
    cdsImport_FullName(&data.d_visa,
		directory_p, (FullName_t *)directory, FALSE);
    second_p = (Set_t *)data.d_user.ud_resp_p;
    read_p = (Set_t *)cdsImport_Set(&data.d_visa,
		secondaries_p, (Set_t *)second_p, FALSE);
    cdsImport_Set(&data.d_visa, readonlies_p, (Set_t *)read_p, FALSE); 
      status = db_begin_operation(&data.d_user);
      if (status == DNS_SUCCESS) {
         status = ta_epoch(&data.d_user, (FullName_t *)directory, second_p, read_p);
         (void)db_end_operation(&data.d_user);
      }
    status = bldout(&data, NULL, status);
    if (status != DNS_SUCCESS) 
      cdsExport_Exception(&data.d_visa, data.d_user.ud_resp_p, cds_status_p);
    release_user_ctx(&data);
  } else
    cdsExport_Exception(&data.d_visa, data.d_status, cds_status_p);
  
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " <cds_NewEpoch(%s,%s,,) %s return(rpc_s_ok)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_FullName(directoryBuf,directory_p),
      deb_ascii_cds_status(cds_statusBuf,cds_status_p)));

  return(rpc_s_ok);
}

/*      
 *  cds_AllowClearinghousse
 *                                       
 * This routine dispatches a message to ta_allow
 * It must take apart the message and pass the arguments to the routine
 * then after the call, it will assemble a return message.
 *
 */               
error_status_t 
cds_AllowClearinghouses (handle_t        h,
                         cds_Progress_t  *progress_p,
                         cds_status_t    *cds_status_p)
{

  
  setup_return_status(cds_status_p);
  cds_status_p->er_status = PDU_NotSupported;

  return(rpc_s_ok);
}

/*      
 *  cds_DisallowClearinghousse
 *                                       
 * This routine dispatches a message to ta_disallow
 * It must take apart the message and pass the arguments to the routine
 * then after the call, it will assemble a return message.
 *
 */               
error_status_t 
cds_DisallowClearinghouses (handle_t        h,
                            cds_Progress_t  *progress_p,
                            cds_status_t    *cds_status_p)
{

  
  setup_return_status(cds_status_p);
  cds_status_p->er_status = PDU_NotSupported;

  return(rpc_s_ok);
}

/*      
 *  cds_DeleteChild
 *                                       
 * This routine dispatches a message to ta_delete_child
 * It must take apart the message and pass the arguments to the routine
 * then after the call, it will assemble a return message.
 *
 */               
error_status_t 
cds_DeleteChild (handle_t        h,
                 cds_Progress_t  *progress_p,
                 cds_status_t    *cds_status_p)
{
  DEB_ASCII_BUF_rpc_binding_handle(hBuf)
  DEB_ASCII_BUF_cds_Progress(progressBuf)
  DEB_ASCII_BUF_cds_status(cds_statusBuf)
  data_t		data;
  byte_t		*msg_p;
  int			status;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " >cds_DeleteChild(%s,%s)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_Progress(progressBuf,progress_p)));
  
  setup_return_status(cds_status_p);

  /* Convert RPC structures to local compresed buffer */
  status = get_user_ctx(&data, h, OP_DeleteChildEntry);
  if (status == DNS_SUCCESS) {
    cdsImport_Progress(&data.d_visa,
		progress_p, (Progress_t *)data.d_user.ud_resp_p, FALSE);
    status = db_begin_operation(&data.d_user);
    if (status == DNS_SUCCESS) {
        status = ta_delete(&data.d_user, (Progress_t *)data.d_user.ud_resp_p,
		       ET_childPointer);
        (void)db_end_operation(&data.d_user);
    }
    status = bldout(&data, progress_p, status);
    if (status == DNS_SUCCESS) {
      msg_p = cdsExport_Progress(&data.d_visa,
		(Progress_t *)data.d_user.ud_resp_p, progress_p, cds_status_p);
    } else
      cdsExport_Exception(&data.d_visa, data.d_user.ud_resp_p, cds_status_p);
    release_user_ctx(&data);
  } else
    cdsExport_Exception(&data.d_visa, data.d_status, cds_status_p);
  
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " <cds_DeleteChild(%s,%s) %s return(rpc_s_ok)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_Progress(progressBuf,progress_p),
      deb_ascii_cds_status(cds_statusBuf,cds_status_p)));

  return(rpc_s_ok);
}

/*      
 *  cds_DeleteObject
 *                                       
 * This routine dispatches a message to ta_delete
 * It must take apart the message and pass the arguments to the routine
 * then after the call, it will assemble a return message.
 *
 */               
error_status_t 
cds_DeleteObject (handle_t        h,
                  cds_Progress_t  *progress_p,
                  cds_status_t    *cds_status_p)
{
  DEB_ASCII_BUF_rpc_binding_handle(hBuf)
  DEB_ASCII_BUF_cds_Progress(progressBuf)
  DEB_ASCII_BUF_cds_status(cds_statusBuf)
  data_t		data;
  byte_t		*msg_p;
  int			status;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " >cds_DeleteObject(%s,%s)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_Progress(progressBuf,progress_p)));
  
  setup_return_status(cds_status_p);

  /* Convert RPC structures to local compresed buffer */
  status = get_user_ctx(&data, h, OP_DeleteObject);
  if (status == DNS_SUCCESS) {
    cdsImport_Progress(&data.d_visa,
		progress_p, (Progress_t *)data.d_user.ud_resp_p, FALSE);
    status = db_begin_operation(&data.d_user);
    if (status == DNS_SUCCESS) {
        status = ta_delete(&data.d_user,
      		(Progress_t *)data.d_user.ud_resp_p, ET_object);
        (void)db_end_operation(&data.d_user);
    }
    status = bldout(&data, progress_p, status);
    if (status == DNS_SUCCESS) {
      msg_p = cdsExport_Progress(&data.d_visa,
		(Progress_t *)data.d_user.ud_resp_p, progress_p, cds_status_p);
    } else
      cdsExport_Exception(&data.d_visa, data.d_user.ud_resp_p, cds_status_p);
    release_user_ctx(&data);
  } else
    cdsExport_Exception(&data.d_visa, data.d_status, cds_status_p);
  
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " <cds_DeleteObject(%s,%s) %s return(rpc_s_ok)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_Progress(progressBuf,progress_p),
      deb_ascii_cds_status(cds_statusBuf,cds_status_p)));

  return(rpc_s_ok);
}

/*      
 *  cds_DeleteSoftLink
 *                                       
 * This routine dispatches a message to ta_delete
 * It must take apart the message and pass the arguments to the routine
 * then after the call, it will assemble a return message.
 *
 */               
error_status_t 
cds_DeleteSoftLink (handle_t        h,
                    cds_Progress_t  *progress_p,
                    cds_status_t    *cds_status_p)
{
  DEB_ASCII_BUF_rpc_binding_handle(hBuf)
  DEB_ASCII_BUF_cds_Progress(progressBuf)
  DEB_ASCII_BUF_cds_status(cds_statusBuf)
  data_t		data;
  byte_t		*msg_p;
  int			status;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " >cds_DeleteSoftLink(%s,%s)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_Progress(progressBuf,progress_p)));
  
  setup_return_status(cds_status_p);

  /* Convert RPC structures to local compresed buffer */
  status = get_user_ctx(&data, h, OP_DeleteLink);
  if (status == DNS_SUCCESS) {
    cdsImport_Progress(&data.d_visa,
		progress_p, (Progress_t *)data.d_user.ud_resp_p, FALSE);
    status = db_begin_operation(&data.d_user);
    if (status == DNS_SUCCESS) {
      status = ta_delete(&data.d_user,
		(Progress_t *)data.d_user.ud_resp_p, ET_softlink);
      (void)db_end_operation(&data.d_user);
    }
    status = bldout(&data, progress_p, status);
    if (status == DNS_SUCCESS) {
      msg_p = cdsExport_Progress(&data.d_visa,
		(Progress_t *)data.d_user.ud_resp_p, progress_p, cds_status_p);
    } else
      cdsExport_Exception(&data.d_visa, data.d_user.ud_resp_p, cds_status_p);
    release_user_ctx(&data);
  } else
    cdsExport_Exception(&data.d_visa, data.d_status, cds_status_p);
  
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " <cds_DeleteSoftLink(%s,%s) %s return(rpc_s_ok)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_Progress(progressBuf,progress_p),
      deb_ascii_cds_status(cds_statusBuf,cds_status_p)));

  return(rpc_s_ok);
}

/*      
 *  cds_DeleteDirectory
 *                                       
 * This routine dispatches a message to ta_delete_dir
 * It must take apart the message and pass the arguments to the routine
 * then after the call, it will assemble a return message.
 *
 */               
error_status_t 
cds_DeleteDirectory (handle_t        h,
                     cds_Progress_t  *progress_p,
                     cds_status_t    *cds_status_p)
{
  DEB_ASCII_BUF_rpc_binding_handle(hBuf)
  DEB_ASCII_BUF_cds_Progress(progressBuf)
  DEB_ASCII_BUF_cds_status(cds_statusBuf)
  data_t		data;
  byte_t		*msg_p;
  int			status;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " >cds_DeleteDirectory(%s,%s)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_Progress(progressBuf,progress_p)));
  
  setup_return_status(cds_status_p);

  /* Convert RPC structures to local compresed buffer */
  status = get_user_ctx(&data, h, OP_DeleteDirectory);
  if (status == DNS_SUCCESS) {
    cdsImport_Progress(&data.d_visa,
		progress_p, (Progress_t *)data.d_user.ud_resp_p, FALSE);
    status = db_begin_operation(&data.d_user);
    if (status == DNS_SUCCESS) {
        status = ta_delete_dir(&data.d_user, (Progress_t *)data.d_user.ud_resp_p);
        (void)db_end_operation(&data.d_user);
    }
    status = bldout(&data, progress_p, status);
    if (status == DNS_SUCCESS) {
      msg_p = cdsExport_Progress(&data.d_visa,
		(Progress_t *)data.d_user.ud_resp_p, progress_p, cds_status_p);
    } else
      cdsExport_Exception(&data.d_visa, data.d_user.ud_resp_p, cds_status_p);
    release_user_ctx(&data);
  } else
    cdsExport_Exception(&data.d_visa, data.d_status, cds_status_p);
  
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " <cds_DeleteDirectory(%s,%s) %s return(rpc_s_ok)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_Progress(progressBuf,progress_p),
      deb_ascii_cds_status(cds_statusBuf,cds_status_p)));

  return(rpc_s_ok);
}

/*      
 *  cds_Skulk
 *                                       
 * This routine dispatches a message to ta_skulk
 * It must take apart the message and pass the arguments to the routine
 * then after the call, it will assemble a return message.
 *
 */               
error_status_t 
cds_Skulk (handle_t        h,
           cds_Progress_t  *progress_p,
           cds_status_t    *cds_status_p)
{
  DEB_ASCII_BUF_rpc_binding_handle(hBuf)
  DEB_ASCII_BUF_cds_Progress(progressBuf)
  DEB_ASCII_BUF_cds_status(cds_statusBuf)
  data_t		data;
  byte_t		*msg_p;
  int			status;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " >cds_Skulk(%s,%s)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_Progress(progressBuf,progress_p)));
  
  setup_return_status(cds_status_p);

  /* Convert RPC structures to local compresed buffer */
  status = get_user_ctx(&data, h, OP_Skulk);
  if (status == DNS_SUCCESS) {
    cdsImport_Progress(&data.d_visa,
		progress_p, (Progress_t *)data.d_user.ud_resp_p, FALSE);
    status = db_begin_operation(&data.d_user);
    if (status == DNS_SUCCESS) {
        status = ta_skulk(&data.d_user, (Progress_t *)data.d_user.ud_resp_p);
        (void)db_end_operation(&data.d_user);
    }
    status = bldout(&data, progress_p, status);
    if (status == DNS_SUCCESS) {
      msg_p = cdsExport_Progress(&data.d_visa,
		(Progress_t *)data.d_user.ud_resp_p, progress_p, cds_status_p);
    } else
      cdsExport_Exception(&data.d_visa, data.d_user.ud_resp_p, cds_status_p);
    release_user_ctx(&data);
  } else
    cdsExport_Exception(&data.d_visa, data.d_status, cds_status_p);
  
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " <cds_Skulk(%s,%s) %s return(rpc_s_ok)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_Progress(progressBuf,progress_p),
      deb_ascii_cds_status(cds_statusBuf,cds_status_p)));

  return(rpc_s_ok);
}

/*      
 *  cds_ResolveName
 *                                       
 * This routine dispatches a message to ta_resolvename
 * It must take apart the message and pass the arguments to the routine
 * then after the call, it will assemble a return message.
 *
 */               
error_status_t 
cds_ResolveName (handle_t        h,
                 cds_Progress_t  *progress_p,
                 cds_status_t    *cds_status_p)
{
  DEB_ASCII_BUF_rpc_binding_handle(hBuf)
  DEB_ASCII_BUF_cds_Progress(progressBuf)
  DEB_ASCII_BUF_cds_status(cds_statusBuf)
  data_t		data;
  byte_t		*msg_p;
  int			status;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " >cds_ResolveName(%s,%s)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_Progress(progressBuf,progress_p)));

  setup_return_status(cds_status_p);
  
  /*	
   * Convert RPC structures to local compresed buffer 
   */

  status = get_user_ctx(&data, h, OP_ResolveName);
  if (status == DNS_SUCCESS) 
  {
      cdsImport_Progress(&data.d_visa,
	                 progress_p,(Progress_t *)data.d_user.ud_resp_p, 
			 FALSE);

      status = db_begin_operation(&data.d_user);
      if (status == DNS_SUCCESS) 
      {
	  status = ta_resolve_name(&data.d_user,	
				   (Progress_t *)data.d_user.ud_resp_p);
	  (void)db_end_operation(&data.d_user);
      }

      status = bldout(&data, progress_p, status);
      if (status == DNS_SUCCESS) 
      {
	  msg_p = cdsExport_Progress(&data.d_visa,
				     (Progress_t *)data.d_user.ud_resp_p,
				     progress_p, 
				     cds_status_p);
      } 
      else
      {
	  cdsExport_Exception(&data.d_visa, data.d_user.ud_resp_p, 	
			      cds_status_p);
      }
     release_user_ctx(&data);
  }
  else
  {
      cdsExport_Exception(&data.d_visa, data.d_status, cds_status_p);
  }
  
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " <cds_ResolveName(%s,%s) (result: %s) return(rpc_s_ok)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_Progress(progressBuf,progress_p),
      deb_ascii_cds_status(cds_statusBuf,cds_status_p)));

  return(rpc_s_ok);
}

/*      
 *  cds_ReadAttribute
 *                                       
 * This routine dispatches a message to ta_read_att.
 * It must take apart the message and pass the arguments to the routine
 * then after the call, it will assemble a return message.
 *
 */               
error_status_t 
cds_ReadAttribute (handle_t         h,
                   cds_Progress_t   *progress_p,
                   ndr_usmall_int   type,
                   cds_Name_t       *att_p,
                   cds_Timestamp_t  *context_p,
                   ndr_ulong_int    max_size,
                   ndr_ulong_int    maybemore,
                   cds_RA_value_t   *value_p,
                   unsigned char    *wholeset_p,
                   cds_status_t     *cds_status_p)
{
  DEB_ASCII_BUF_rpc_binding_handle(hBuf)
  DEB_ASCII_BUF_cds_Progress(progressBuf)
  DEB_ASCII_BUF_type(typeBuf)
  DEB_ASCII_BUF_cds_Name(attBuf)
  DEB_ASCII_BUF_cds_status(cds_statusBuf)
  data_t		data;
  AttributeName_u	attribute;
  Timestamp_u		context;
  byte_t		*msg_p;
  int			retLen = NSMAXSIZE;
  int			status;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " >cds_ReadAttribute(%s,%s,%s,%s,,,,,)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_Progress(progressBuf,progress_p),
      deb_ascii_type(typeBuf,type),
      deb_ascii_cds_Name(attBuf,att_p)));

  value_p->returningToClerk = RA_none;
  setup_return_status(cds_status_p);

  /* check values types */
  if (!chk_EntryType(type, FALSE))
    cds_status_p->er_status = PDU_InvalidArgument;
  else 
  {
    /*
     * Convert RPC structures to local compresed buffer 
     */
    status = get_user_ctx(&data, h, OP_ReadAttribute);
    if (status == DNS_SUCCESS) 
    {
      cdsImport_Progress(&data.d_visa,
		progress_p, (Progress_t *)data.d_user.ud_resp_p, FALSE);
      cdsImport_Timestamp(&data.d_visa,
		context_p, (Timestamp_t *)context, FALSE);
      cdsImport_AttributeName(&data.d_visa,
		att_p, (AttributeName_t *)attribute, FALSE);
      status = db_begin_operation(&data.d_user);
      if (status == DNS_SUCCESS) 
      {
          status = ta_read_att(&data.d_user,
			(Progress_t *)data.d_user.ud_resp_p, type, maybemore,
			(SimpleName_t *)attribute, 
			(Timestamp_t *)context, max_size, &retLen);
          (void)db_end_operation(&data.d_user);
      }

      status = bldout(&data, progress_p, status);
      if (status == DNS_SUCCESS) 
      {
	msg_p = cdsExport_Progress(&data.d_visa,
		(Progress_t *)data.d_user.ud_resp_p, progress_p, cds_status_p);

	/* skip wholenetry/oneattribute - only return one */     
	if (msg_p) 
	{
	    if (FLAG_IS_SET(&progress_p->pr_flags, PR_done)) {
		register int i;

	    i = GET8(msg_p);

	    switch (i) {
	    case RC_ONE_ATTRIBUTE:
	      dce_assert(cds__svc_handle,
			 (AT_null == RA_none));
	      dce_assert(cds__svc_handle,
			 (AT_single == RA_single));
	      dce_assert(cds__svc_handle,
			 (AT_set == RA_set));
	      value_p->returningToClerk = GET8(msg_p);
	      switch (value_p->returningToClerk) 
	      {
	      case AT_null:
		break;
	      case AT_single:
		value_p->tagged_union.value_single_p =
		  (cds_SetMember_t *)rpc_ss_allocate(sizeof(cds_SetMember_t));
		if (!value_p->tagged_union.value_single_p) 
		{
		    msg_p = NULL;
		    break;
		}
		msg_p = cdsExport_SetMember(&data.d_visa,
		  (MemberValue_t *)msg_p,
		  value_p->tagged_union.value_single_p, cds_status_p);
		break;
	      case AT_set:
		msg_p = cdsExport_Set(&data.d_visa, (Set_t *)msg_p,
			&value_p->tagged_union.value_set_p, cds_status_p);
		break;
	      }
	      if (msg_p)
		*wholeset_p = EXT8(msg_p);
	      break;
	    case RC_WHOLE_ENTRY:
	      value_p->returningToClerk = RA_wholeSet;
	      i = GET8(msg_p);
	      value_p->tagged_union.wholeEntry_p = (cds_WholeEntry_t *)
		rpc_ss_allocate(sizeof(cds_WholeEntry_t)
			- sizeof(value_p->tagged_union.wholeEntry_p->entry)
			+ i*sizeof(cds_WE_entry_t));
	      if (!value_p->tagged_union.wholeEntry_p) break;
	      value_p->tagged_union.wholeEntry_p->numberOfAttributes = i;
	      for (i = 0; 
		   i < value_p->tagged_union.wholeEntry_p->numberOfAttributes;
		   i++) 
	      {
		register cds_WE_entry_t *we_p =
		    &value_p->tagged_union.wholeEntry_p->entry[i];
		msg_p = cdsExport_AttributeName(&data.d_visa,
			(AttributeName_t *)msg_p, &we_p->name);
		we_p->value.type = GET8(msg_p);
		switch (we_p->value.type) {
		case AT_null:
		  continue;
		case AT_single:
		  we_p->value.tagged_union.single_p =
		    (cds_SetMember_t *)rpc_ss_allocate(sizeof(cds_SetMember_t));
		  if (!we_p->value.tagged_union.single_p) {
		    msg_p = NULL;
		    break;
		  }
		  msg_p = cdsExport_SetMember(&data.d_visa,
		    (MemberValue_t *)msg_p,
		    we_p->value.tagged_union.single_p, cds_status_p);
		  continue;
		case AT_set:
		  msg_p = cdsExport_Set(&data.d_visa, (Set_t *)msg_p,
			&we_p->value.tagged_union.set_p, cds_status_p);
		  if (!msg_p)
		    break;
		  continue;
		}
		break;
	      }
	      break;
	    }
	  }
	}
      } 
      else
      {
	  cdsExport_Exception(&data.d_visa, 
			      data.d_user.ud_resp_p, 
			      cds_status_p);
      }
      release_user_ctx(&data);
    } 
    else
    {
	cdsExport_Exception(&data.d_visa, data.d_status, cds_status_p);
    }
  }
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " <cds_ReadAttribute(%s,%s,%s,%s,,,,,) %s return(rpc_s_ok)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_Progress(progressBuf,progress_p),
      deb_ascii_type(typeBuf,type),
      deb_ascii_cds_Name(attBuf,att_p),
      deb_ascii_cds_status(cds_statusBuf,cds_status_p)));

  return(rpc_s_ok);
}

/*      
 *  cds_EnumerateAttribute
 *                                       
 * This routine dispatches a message to ta_enumerate_att
 * It must take apart the message and pass the arguments to the routine
 * then after the call, it will assemble a return message.
 *
 */               
error_status_t 
cds_EnumerateAttributes (handle_t        h,
                         cds_Progress_t  *progress_p,
                         ndr_usmall_int  type,
                         cds_Name_t      *context_p,
                         ndr_ulong_int   max_size,
                         cds_SetP_t      *valueset_p,
                         unsigned char   *wholeset_p,
                         cds_status_t    *cds_status_p)
{
  DEB_ASCII_BUF_rpc_binding_handle(hBuf)
  DEB_ASCII_BUF_cds_Progress(progressBuf)
  DEB_ASCII_BUF_type(typeBuf)
  DEB_ASCII_BUF_cds_Name(contextBuf)
  DEB_ASCII_BUF_cds_status(cds_statusBuf)
  data_t		data;
  AttributeName_u	context;
  byte_t		*msg_p;
  int			retLen = NSMAXSIZE;
  int			status;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " >cds_EnumerateAttributes(%s,%s,%s,%s,,,)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_Progress(progressBuf,progress_p),
      deb_ascii_type(typeBuf,type),
      deb_ascii_cds_Name(contextBuf,context_p)));

  setup_return_status(cds_status_p);

  /* check values types */
  if (!chk_EntryType(type, FALSE))
    cds_status_p->er_status = PDU_InvalidArgument;
  else {

    /* Convert RPC structures to local compresed buffer */
    status = get_user_ctx(&data, h, OP_EnumerateAttributes);
    if (status == DNS_SUCCESS) {
      cdsImport_Progress(&data.d_visa,
		progress_p, (Progress_t *)data.d_user.ud_resp_p, FALSE);
      cdsImport_AttributeName(&data.d_visa,
		context_p, (AttributeName_t *)context, FALSE);
      status = db_begin_operation(&data.d_user);
      if (status == DNS_SUCCESS) {
          status = ta_enumerate_att(&data.d_user,
			(Progress_t *)data.d_user.ud_resp_p, type, 
			(SimpleName_t *)context, max_size, &retLen);
          (void)db_end_operation(&data.d_user);
      }
      status = bldout(&data, progress_p, status);
      if (status == DNS_SUCCESS) {
	msg_p = cdsExport_Progress(&data.d_visa,
		(Progress_t *)data.d_user.ud_resp_p, progress_p, cds_status_p);
	if (msg_p) {
	  if (FLAG_IS_SET(&progress_p->pr_flags, PR_done)) {
	    msg_p = cdsExport_Set(&data.d_visa,
		(Set_t *)msg_p, valueset_p, cds_status_p);
	    if (msg_p)
	      *wholeset_p = EXT8(msg_p);
	    
	  }
	}
      } else
	cdsExport_Exception(&data.d_visa, data.d_user.ud_resp_p, cds_status_p);
      release_user_ctx(&data);
    } else
      cdsExport_Exception(&data.d_visa, data.d_status, cds_status_p);
  }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " <cds_EnumerateAttributes(%s,%s,%s,%s,,,) %s return(rpc_s_ok)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_Progress(progressBuf,progress_p),
      deb_ascii_type(typeBuf,type),
      deb_ascii_cds_Name(contextBuf,context_p),
      deb_ascii_cds_status(cds_statusBuf,cds_status_p)));

  return(rpc_s_ok);
}

/*      
 *  cds_ModifyAttribute
 *                                       
 * This routine dispatches a message to ta_modify_att.
 * It must take apart the message and pass the arguments to the routine
 * then after the call, it will assemble a return message.
 *
 */               
error_status_t 
cds_ModifyAttribute (handle_t        h,
                     cds_Progress_t  *progress_p,
                     ndr_usmall_int  type,
                     cds_Update_t    *update_p,
                     cds_status_t    *cds_status_p)
{
  DEB_ASCII_BUF_rpc_binding_handle(hBuf)
  DEB_ASCII_BUF_cds_Progress(progressBuf)
  DEB_ASCII_BUF_type(typeBuf)
  DEB_ASCII_BUF_cds_Update(updateBuf)
  DEB_ASCII_BUF_cds_status(cds_statusBuf)
  data_t		data;
  Update_u		update;
  byte_t		*msg_p;
  int			status;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " >cds_ModifyAttribute(%s,%s,%s,%s)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_Progress(progressBuf,progress_p),
      deb_ascii_type(typeBuf,type),
      deb_ascii_cds_Update(updateBuf,update_p)));

  setup_return_status(cds_status_p);

  /* check values types */
  if (!chk_EntryType(type, FALSE))
    cds_status_p->er_status = PDU_InvalidArgument;
  else {

    /* Convert RPC structures to local compresed buffer */
    status = get_user_ctx(&data, h, OP_ModifyAttribute);
    if (status == DNS_SUCCESS) {
      cdsImport_Progress(&data.d_visa,
		progress_p, (Progress_t *)data.d_user.ud_resp_p, FALSE);
      cdsImport_Update(&data.d_visa, update_p, (Update_t *)update, FALSE);
      status = db_begin_operation(&data.d_user);
      if (status == DNS_SUCCESS) {
          status = ta_modify_attribute(&data.d_user,
			(Progress_t *)data.d_user.ud_resp_p, type,
			(Update_t *)update, LEN_Update(update));
          (void)db_end_operation(&data.d_user);
      }
      status = bldout(&data, progress_p, status);
      if (status == DNS_SUCCESS) {
	msg_p = cdsExport_Progress(&data.d_visa,
		(Progress_t *)data.d_user.ud_resp_p, progress_p, cds_status_p);
      } else
	cdsExport_Exception(&data.d_visa, data.d_user.ud_resp_p, cds_status_p);
      release_user_ctx(&data);
    } else
      cdsExport_Exception(&data.d_visa, data.d_status, cds_status_p);
  }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " <cds_ModifyAttribute(%s,%s,%s,%s) %s return(rpc_s_ok)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_Progress(progressBuf,progress_p),
      deb_ascii_type(typeBuf,type),
      deb_ascii_cds_Update(updateBuf,update_p),
      deb_ascii_cds_status(cds_statusBuf,cds_status_p)));

  return(rpc_s_ok);
}

/*      
 *  cds_TestAttribute
 *                                       
 * This routine dispatches a message to ta_test_att.
 * It must take apart the message and pass the arguments to the routine
 * then after the call, it will assemble a return message.
 *
 */               
error_status_t 
cds_TestAttribute (handle_t           h,
                   cds_Progress_t     *progress_p,
                   ndr_usmall_int     type,
                   cds_Name_t         *attribute_p,
                   cds_AtomicValue_t  *value_p,
                   unsigned char      *result_p,
                   cds_status_t       *cds_status_p)
{
  DEB_ASCII_BUF_rpc_binding_handle(hBuf)
  DEB_ASCII_BUF_cds_Progress(progressBuf)
  DEB_ASCII_BUF_type(typeBuf)
  DEB_ASCII_BUF_cds_Name(attributeBuf)
  DEB_ASCII_BUF_cds_status(cds_statusBuf)
  data_t		data;
  AtomicValue_u		value;
  AttributeName_u	attribute;
  byte_t		*msg_p;
  int			status;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " >cds_TestAttribute(%s,%s,%s,%s,,)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_Progress(progressBuf,progress_p),
      deb_ascii_type(typeBuf,type),
      deb_ascii_cds_Name(attributeBuf,attribute_p)));

  setup_return_status(cds_status_p);

  /* check values types */
  if (!chk_EntryType(type, FALSE))
    cds_status_p->er_status = PDU_InvalidArgument;
  else {

    /* Convert RPC structures to local compresed buffer */
    status = get_user_ctx(&data, h, OP_TestAttribute);
    if (status == DNS_SUCCESS) {
      cdsImport_Progress(&data.d_visa,
		progress_p,(Progress_t *)data.d_user.ud_resp_p, FALSE);
      cdsImport_AtomicValue(&data.d_visa,
		value_p, (AtomicValue_t *)value, FALSE);
      cdsImport_AttributeName(&data.d_visa,
		attribute_p, (AttributeName_t *)attribute, FALSE);
      status = db_begin_operation(&data.d_user);
      if (status == DNS_SUCCESS) {
          status = ta_test_att(&data.d_user, (Progress_t *)data.d_user.ud_resp_p,
				   type, (SimpleName_t *)attribute,
				   (AtomicValue_t *)value, result_p);
          (void)db_end_operation(&data.d_user);
      }

      status = bldout(&data, progress_p, status);
      if (status == DNS_SUCCESS) {
	msg_p = cdsExport_Progress(&data.d_visa,
		(Progress_t *)data.d_user.ud_resp_p, progress_p, cds_status_p);
      } else
	cdsExport_Exception(&data.d_visa, data.d_user.ud_resp_p, cds_status_p);
      release_user_ctx(&data);
    } else
      cdsExport_Exception(&data.d_visa, data.d_status, cds_status_p);
  }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " <cds_TestAttribute(%s,%s,%s,%s,,) %s return(rpc_s_ok)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_Progress(progressBuf,progress_p),
      deb_ascii_type(typeBuf,type),
      deb_ascii_cds_Name(attributeBuf,attribute_p),
      deb_ascii_cds_status(cds_statusBuf,cds_status_p)));

  return(rpc_s_ok);
}

/*      
 *  cds_TestGroup
 *                                       
 * This routine dispatches a message to ta_testgroup
 * It must take apart the message and pass the arguments to the routine
 * then after the call, it will assemble a return message.
 *
 */               
error_status_t 
cds_TestGroup (handle_t        h,
               cds_Progress_t  *progress_p,
               cds_FullName_t  *member_p,
               uuid_t          *loop_p,
               ndr_usmall_int  *direct_p,
               ndr_usmall_int  *result_p,
               cds_TimeoutP_t  *outTimeout_p,
               cds_status_t    *cds_status_p)
{
  DEB_ASCII_BUF_rpc_binding_handle(hBuf)
  DEB_ASCII_BUF_cds_Progress(progressBuf)
  DEB_ASCII_BUF_cds_FullName(memberBuf)
  DEB_ASCII_BUF_cds_status(cds_statusBuf)
  data_t		data;
  FullName_u		member;
  byte_t		*msg_p;
  int			retLen;
  int			status;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " >cds_TestGroup(%s,%s,%s,,,,)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_Progress(progressBuf,progress_p),
      deb_ascii_cds_FullName(memberBuf,member_p)));

  setup_return_status(cds_status_p);

  /* Convert RPC structures to local compresed buffer */
  status = get_user_ctx(&data, h, OP_TestGroup);
  if (status == DNS_SUCCESS) {
      cdsImport_Progress(&data.d_visa,
		progress_p,(Progress_t *)data.d_user.ud_resp_p, FALSE);
      cdsImport_FullName(&data.d_visa, member_p, (FullName_t *)member, FALSE);

      status = db_begin_operation(&data.d_user);
      if (status == DNS_SUCCESS) {
          status = ta_testgroup(&data.d_user, (Progress_t *)data.d_user.ud_resp_p,
			    (FullName_t *)member, loop_p,
			    *direct_p, &retLen);
          (void)db_end_operation(&data.d_user);
      }
      status = bldout(&data, progress_p, status);
      if (status == DNS_SUCCESS) {
	msg_p = cdsExport_Progress(&data.d_visa,
		(Progress_t *)data.d_user.ud_resp_p, progress_p, cds_status_p);
	if (msg_p) {
	  if (FLAG_IS_SET(&progress_p->pr_flags, PR_done)) {
	    msg_p = cdsExport_Timeout(&data.d_visa,
		(Timeout_t *)msg_p, outTimeout_p);
	    *direct_p = GET8(msg_p);
	    COPY_bytes(msg_p, loop_p, sizeof(*loop_p));
	    msg_p = SKIP_bytes(msg_p, sizeof(*loop_p));
	    *result_p = GET8(msg_p);
	  }
	}
      } else
	cdsExport_Exception(&data.d_visa, data.d_user.ud_resp_p, cds_status_p);
      release_user_ctx(&data);
    } else
      cdsExport_Exception(&data.d_visa, data.d_status, cds_status_p);

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " <cds_TestGroup(%s,%s,%s,,,,) %s return(rpc_s_ok)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_Progress(progressBuf,progress_p),
      deb_ascii_cds_FullName(memberBuf,member_p),
      deb_ascii_cds_status(cds_statusBuf,cds_status_p)));

  return(rpc_s_ok);
}

/*      
 *  cds_EnumerateChildren
 *                                       
 * This routine dispatches a message to ta_enumerate
 * It must take apart the message and pass the arguments to the routine
 * then after the call, it will assemble a return message.
 *
 */               
error_status_t 
cds_EnumerateChildren (handle_t        h,
                       cds_Progress_t  *progress_p,
                       cds_Name_t      *wild_p,
                       cds_Name_t      *context_p,
                       ndr_ulong_int   max_size,
                       cds_SetP_t      *valueset_p,
                       unsigned char   *wholeset_p,
                       cds_status_t    *cds_status_p)
{
  DEB_ASCII_BUF_rpc_binding_handle(hBuf)
  DEB_ASCII_BUF_cds_Progress(progressBuf)
  DEB_ASCII_BUF_cds_Name(wildBuf)
  DEB_ASCII_BUF_cds_Name(contextBuf)
  DEB_ASCII_BUF_cds_status(cds_statusBuf)
  data_t		data;
  SimpleName_u		wild, context;
  byte_t		*msg_p;
  int			retLen = NSMAXSIZE;
  int			status;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " >cds_EnumerateChildren(%s,%s,%s,%s,,,)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_Progress(progressBuf,progress_p),
      deb_ascii_cds_Name(wildBuf,wild_p),
      deb_ascii_cds_Name(contextBuf,context_p)));

  setup_return_status(cds_status_p);

  /* Convert RPC structures to local compresed buffer */
  status = get_user_ctx(&data, h, OP_EnumerateChildren);
  if (status == DNS_SUCCESS) {
    cdsImport_Progress(&data.d_visa,
		progress_p,(Progress_t *)data.d_user.ud_resp_p, FALSE);
    cdsImport_SimpleName(&data.d_visa, wild_p, (SimpleName_t *)wild, FALSE);
    cdsImport_SimpleName(&data.d_visa,
		context_p, (SimpleName_t *)context, FALSE);
    status = db_begin_operation(&data.d_user);
    if (status == DNS_SUCCESS) {
       status = ta_enumerate(&data.d_user, (Progress_t *)data.d_user.ud_resp_p,
			  (SimpleName_t *)wild, (SimpleName_t *)context, 
			  NULL, max_size, ET_childPointer,
			  &retLen, BL_false);
       (void)db_end_operation(&data.d_user);
    }
    status = bldout(&data, progress_p, status);
    if (status == DNS_SUCCESS) {
      msg_p = cdsExport_Progress(&data.d_visa,
		(Progress_t *)data.d_user.ud_resp_p, progress_p, cds_status_p);
      if (msg_p) {
	if (FLAG_IS_SET(&progress_p->pr_flags, PR_done)) {
	  msg_p = cdsExport_Set(&data.d_visa,
		(Set_t *)msg_p, valueset_p, cds_status_p);
	  if (msg_p)
	    *wholeset_p = EXT8(msg_p);
	}
      }
    } else
      cdsExport_Exception(&data.d_visa, data.d_user.ud_resp_p, cds_status_p);
    release_user_ctx(&data);
  } else
    cdsExport_Exception(&data.d_visa, data.d_status, cds_status_p);

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " <cds_EnumerateChildren(%s,%s,%s,%s,,,) %s return(rpc_s_ok)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_Progress(progressBuf,progress_p),
      deb_ascii_cds_Name(wildBuf,wild_p),
      deb_ascii_cds_Name(contextBuf,context_p),
      deb_ascii_cds_status(cds_statusBuf,cds_status_p)));

  return(rpc_s_ok);
}

/*      
 *  cds_EnumerateSoftLinks
 *                                       
 * This routine dispatches a message to ta_enumerate
 * It must take apart the message and pass the arguments to the routine
 * then after the call, it will assemble a return message.
 *
 */               
error_status_t 
cds_EnumerateSoftLinks (handle_t        h,
                        cds_Progress_t  *progress_p,
                        cds_Name_t      *wild_p,
                        cds_Name_t      *context_p,
                        ndr_ulong_int   max_size,
                        cds_SetP_t      *valueset_p,
                        unsigned char   *wholeset_p,
                        cds_status_t    *cds_status_p)
{
  DEB_ASCII_BUF_rpc_binding_handle(hBuf)
  DEB_ASCII_BUF_cds_Progress(progressBuf)
  DEB_ASCII_BUF_cds_Name(wildBuf)
  DEB_ASCII_BUF_cds_Name(contextBuf)
  DEB_ASCII_BUF_cds_status(cds_statusBuf)
  data_t		data;
  SimpleName_u		wild, context;
  byte_t		*msg_p;
  int			retLen = NSMAXSIZE;
  int			status;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " >cds_EnumerateSoftLinks(%s,%s,%s,%s,,,)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_Progress(progressBuf,progress_p),
      deb_ascii_cds_Name(wildBuf,wild_p),
      deb_ascii_cds_Name(contextBuf,context_p)));

  setup_return_status(cds_status_p);

  /* Convert RPC structures to local compresed buffer */
  status = get_user_ctx(&data, h, OP_EnumerateLinks);
  if (status == DNS_SUCCESS) {
    cdsImport_Progress(&data.d_visa,
		progress_p,(Progress_t *)data.d_user.ud_resp_p, FALSE);
    cdsImport_SimpleName(&data.d_visa, wild_p, (SimpleName_t *)wild, FALSE);
    cdsImport_SimpleName(&data.d_visa,
		context_p, (SimpleName_t *)context, FALSE);
    status = db_begin_operation(&data.d_user);
    if (status == DNS_SUCCESS) {
      status = ta_enumerate(&data.d_user, (Progress_t *)data.d_user.ud_resp_p,
			  (SimpleName_t *)wild, (SimpleName_t *)context, 
			  NULL, max_size, ET_softlink,
			  &retLen, BL_false);
      (void)db_end_operation(&data.d_user);
    }
    status = bldout(&data, progress_p, status);
    if (status == DNS_SUCCESS) {
      msg_p = cdsExport_Progress(&data.d_visa,
		(Progress_t *)data.d_user.ud_resp_p, progress_p, cds_status_p);
      if (msg_p) {
	if (FLAG_IS_SET(&progress_p->pr_flags, PR_done)) {
	  msg_p = cdsExport_Set(&data.d_visa,
		(Set_t *)msg_p, valueset_p, cds_status_p);
	  if (msg_p)
	    *wholeset_p = EXT8(msg_p);
	}
      }
    } else
      cdsExport_Exception(&data.d_visa, data.d_user.ud_resp_p, cds_status_p);
    release_user_ctx(&data);
  } else
    cdsExport_Exception(&data.d_visa, data.d_status, cds_status_p);

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " <cds_EnumerateSoftLinks(%s,%s,%s,%s,,,) %s return(rpc_s_ok)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_Progress(progressBuf,progress_p),
      deb_ascii_cds_Name(wildBuf,wild_p),
      deb_ascii_cds_Name(contextBuf,context_p),
      deb_ascii_cds_status(cds_statusBuf,cds_status_p)));

  return(rpc_s_ok);
}

/*      
 *  cds_EnumerateObjects
 *                                       
 * This routine dispatches a message to ta_enumerate
 * It must take apart the message and pass the arguments to the routine
 * then after the call, it will assemble a return message.
 *
 */               
error_status_t 
cds_EnumerateObjects (handle_t        h,
                      cds_Progress_t  *progress_p,
                      cds_Name_t      *wild_p,
                      cds_Name_t      *context_p,
                      cds_Name_t      *class_p,
                      ndr_ulong_int   max_size,
                      cds_SetP_t      *valueset_p,
                      ndr_usmall_int  *wholeset_p,
                      ndr_usmall_int  *returnClass_p,
                      cds_status_t    *cds_status_p)
{
  DEB_ASCII_BUF_rpc_binding_handle(hBuf)
  DEB_ASCII_BUF_cds_Progress(progressBuf)
  DEB_ASCII_BUF_cds_Name(wildBuf)
  DEB_ASCII_BUF_cds_Name(contextBuf)
  DEB_ASCII_BUF_cds_Name(classBuf)
  DEB_ASCII_BUF_cds_status(cds_statusBuf)
  data_t		data;
  SimpleName_u		wild, context;
  ClassName_u		class;
  byte_t		*msg_p;
  int			retLen = NSMAXSIZE;
  int			status;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " >cds_EnumerateObjects(%s,%s,%s,%s,%s,,,,)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_Progress(progressBuf,progress_p),
      deb_ascii_cds_Name(wildBuf,wild_p),
      deb_ascii_cds_Name(contextBuf,context_p),
      deb_ascii_cds_Name(classBuf,class_p)));

  setup_return_status(cds_status_p);

  /* Convert RPC structures to local compresed buffer */
  status = get_user_ctx(&data, h, OP_EnumerateLinks);
  if (status == DNS_SUCCESS) {
    cdsImport_Progress(&data.d_visa,
		progress_p,(Progress_t *)data.d_user.ud_resp_p, FALSE);
    cdsImport_SimpleName(&data.d_visa, wild_p, (SimpleName_t *)wild, FALSE);
    cdsImport_SimpleName(&data.d_visa,
		context_p, (SimpleName_t *)context, FALSE);
    cdsImport_ClassName(&data.d_visa, class_p, (ClassName_t *)class, FALSE);
    status = db_begin_operation(&data.d_user);
    if (status == DNS_SUCCESS) {
       status = ta_enumerate(&data.d_user, (Progress_t *)data.d_user.ud_resp_p,
			  (SimpleName_t *)wild, (SimpleName_t *)context, 
			  (SimpleName_t *)class, max_size, ET_object,
			  &retLen, *returnClass_p);
      (void)db_end_operation(&data.d_user);
    }
    status = bldout(&data, progress_p, status);
    if (status == DNS_SUCCESS) {
      msg_p = cdsExport_Progress(&data.d_visa,
		(Progress_t *)data.d_user.ud_resp_p, progress_p, cds_status_p);
      if (msg_p) {
	if (FLAG_IS_SET(&progress_p->pr_flags, PR_done)) {
	  msg_p = cdsExport_Set(&data.d_visa,
		(Set_t *)msg_p, valueset_p, cds_status_p);
	  if (msg_p) {
	    *wholeset_p = GET8(msg_p);
	    *returnClass_p = EXT8(msg_p);
	  }
	}
      }
    } else
      cdsExport_Exception(&data.d_visa, data.d_user.ud_resp_p, cds_status_p);
    release_user_ctx(&data);
  } else
    cdsExport_Exception(&data.d_visa, data.d_status, cds_status_p);

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " <cds_EnumerateObjects(%s,%s,%s,%s,%s,,,,) %s return(rpc_s_ok)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_Progress(progressBuf,progress_p),
      deb_ascii_cds_Name(wildBuf,wild_p),
      deb_ascii_cds_Name(contextBuf,context_p),
      deb_ascii_cds_Name(classBuf,class_p),
      deb_ascii_cds_status(cds_statusBuf,cds_status_p)));

  return(rpc_s_ok);
}

/*      
 *  cds_CreateObject
 *                                       
 * This routine dispatches a message to ta_create_obj.
 * It must take apart the message and pass the arguments to the routine
 * then after the call, it will assemble a return message.
 *
 */               
error_status_t 
cds_CreateObject (handle_t         h,
                  cds_Progress_t   *progress_p,
                  cds_Name_t       *class_p,
                  cds_Version_t    *version_p,
                  uuid_t           *objectid_p,
                  cds_Timestamp_t  *actualcts_p,
                  cds_status_t     *cds_status_p)
{
  DEB_ASCII_BUF_rpc_binding_handle(hBuf)
  DEB_ASCII_BUF_cds_Progress(progressBuf)
  DEB_ASCII_BUF_cds_status(cds_statusBuf)
  data_t		data;
  ClassName_u		class;
  ClassVersion_u	classversion;
  Timestamp_u		outTS;
  ClassName_t		*cl_p;
  ClassVersion_t	*ver_p;
  byte_t		*msg_p;
  int			status;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " >cds_CreateObject(%s,%s,,,,)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_Progress(progressBuf,progress_p)));

  setup_return_status(cds_status_p);

  /* Convert RPC structures to local compresed buffer */
  status = get_user_ctx(&data, h, OP_CreateObject);
  if (status == DNS_SUCCESS) {
    cdsImport_Progress(&data.d_visa,
		progress_p,(Progress_t *)data.d_user.ud_resp_p, FALSE);
    if (class_p == (cds_Name_t *)NULL) {
      cl_p = NULL;
      ver_p = NULL;
    } else {
      cl_p = (ClassName_t *)class;
      cdsImport_ClassName(&data.d_visa, class_p, cl_p, FALSE);
      if (version_p) {
	ver_p = (ClassVersion_t *)classversion; 
	INS8(ver_p->cv_major, version_p->ver_major);
	INS8(ver_p->cv_minor, version_p->ver_minor);
      } else
	ver_p = NULL;
    }

    status = db_begin_operation(&data.d_user);
    if (status == DNS_SUCCESS) {
       status = ta_create_obj(&data.d_user, (Progress_t *)data.d_user.ud_resp_p, 
			   cl_p, ver_p, objectid_p, (Timestamp_t *)outTS);
       (void)db_end_operation(&data.d_user);
    }
    status = bldout(&data, progress_p, status);
    if (status == DNS_SUCCESS) {
      msg_p = cdsExport_Progress(&data.d_visa,
		(Progress_t *)data.d_user.ud_resp_p, progress_p, 
			      cds_status_p);
      if (msg_p) {
	if (FLAG_IS_SET(&progress_p->pr_flags, PR_done))
	  cdsExport_Timestamp(&data.d_visa,
		(Timestamp_t *)outTS, actualcts_p);
      }
    } else
      cdsExport_Exception(&data.d_visa, data.d_user.ud_resp_p, cds_status_p);
    release_user_ctx(&data);
  } else
    cdsExport_Exception(&data.d_visa, data.d_status, cds_status_p);

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " <cds_CreateObject(%s,%s,,,,) %s return(rpc_s_ok)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_Progress(progressBuf,progress_p),
      deb_ascii_cds_status(cds_statusBuf,cds_status_p)));

  return(rpc_s_ok);
}

/*      
 *  cds_CreateSoftLink
 *                                       
 * This routine dispatches a message to ta_create_link.
 * It must take apart the message and pass the arguments to the routine
 * then after the call, it will assemble a return message.
 *
 */               
error_status_t 
cds_CreateSoftLink (handle_t         h,
                    cds_Progress_t   *progress_p,
                    cds_FullName_t   *target_p,
                    cds_Timeout_t    *timeout_p,
                    cds_Timestamp_t  *actualcts_p,
                    cds_status_t     *cds_status_p)
{
  DEB_ASCII_BUF_rpc_binding_handle(hBuf)
  DEB_ASCII_BUF_cds_Progress(progressBuf)
  DEB_ASCII_BUF_cds_FullName(targetBuf)
  DEB_ASCII_BUF_cds_status(cds_statusBuf)
  data_t		data;
  FullName_u		Target;
  Timestamp_u		outTS;
  byte_t		*msg_p;
  int			status;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " >cds_CreateLink(%s,%s,%s,,)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_Progress(progressBuf,progress_p),
      deb_ascii_cds_FullName(targetBuf,target_p)));

  setup_return_status(cds_status_p);

  /* Convert RPC structures to local compresed buffer */
  status = get_user_ctx(&data, h, OP_CreateLink);
  if (status == DNS_SUCCESS) {
    cdsImport_Progress(&data.d_visa,
		progress_p,(Progress_t *)data.d_user.ud_resp_p, FALSE);
    cdsImport_FullName(&data.d_visa, target_p, (FullName_t *)Target, FALSE);
    status = db_begin_operation(&data.d_user);
    if (status == DNS_SUCCESS) {
       status = ta_create_link(&data.d_user,
		(Progress_t *)data.d_user.ud_resp_p, (FullName_t *)Target,
		(Timeout_t *)timeout_p, (Timestamp_t *)outTS);
       (void)db_end_operation(&data.d_user);
    }
    status = bldout(&data, progress_p, status);
    if (status == DNS_SUCCESS) {
      msg_p = cdsExport_Progress(&data.d_visa,
		(Progress_t *)data.d_user.ud_resp_p, progress_p, cds_status_p);
      if (msg_p) {
	if (FLAG_IS_SET(&progress_p->pr_flags, PR_done))
	  cdsExport_Timestamp(&data.d_visa, (Timestamp_t *)outTS, actualcts_p);
      }
    } else
      cdsExport_Exception(&data.d_visa, data.d_user.ud_resp_p, cds_status_p);
    release_user_ctx(&data);
  } else
    cdsExport_Exception(&data.d_visa, data.d_status, cds_status_p);

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " <cds_CreateLink(%s,%s,%s,,) %s return(rpc_s_ok)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_Progress(progressBuf,progress_p),
      deb_ascii_cds_FullName(targetBuf,target_p),
      deb_ascii_cds_status(cds_statusBuf,cds_status_p)));

  return(rpc_s_ok);
}

/*      
 *  cds_CreateChild
 *                                       
 * This routine dispatches a message to ta_create_child.
 * It must take apart the message and pass the arguments to the routine
 * then after the call, it will assemble a return message.
 *
 */               
error_status_t 
cds_CreateChild (handle_t          h,
                 cds_Progress_t    *progress_p,
                 sec_id_foreign_t  *user_p,
                 uuid_t            *childID_p,
                 cds_Set_t         *rpset_p,
                 uuid_t            *parentID_p,
                 cds_status_t      *cds_status_p)
{
  DEB_ASCII_BUF_rpc_binding_handle(hBuf)
  DEB_ASCII_BUF_cds_Progress(progressBuf)
  DEB_ASCII_BUF_uuid(childIDBuf)
  DEB_ASCII_BUF_uuid(parentIDBuf)
  DEB_ASCII_BUF_cds_status(cds_statusBuf)
  data_t		data;
  Set_t			*rpset;  
  byte_t		*msg_p;
  int			status;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " >cds_CreateChild(%s,%s,%s)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_Progress(progressBuf,progress_p),
      deb_ascii_uuid(childIDBuf,childID_p)));

  setup_return_status(cds_status_p);

  status = get_user_ctx(&data, h, OP_CreateChildEntry);
  if (status == DNS_SUCCESS) {
    /* Allocate memory for set conversion */
    rpset = (Set_t *)dns_malloc(NSMAXSIZE);
    if (rpset) {
      /* Convert RPC structures to local compresed byte streams */
      cdsImport_Progress(&data.d_visa,
		progress_p,(Progress_t *)data.d_user.ud_resp_p, FALSE);
      cdsImport_Set(&data.d_visa, rpset_p, (Set_t *)rpset, FALSE);
      status = db_begin_operation(&data.d_user);
      if (status == DNS_SUCCESS) {
          status = ta_create_child(&data.d_user,
			       (Progress_t *)data.d_user.ud_resp_p,
			       (sec_id_foreign_t *)user_p,
			       childID_p, (Set_t *)rpset, parentID_p);
          (void)db_end_operation(&data.d_user);
      }
      status = bldout(&data, progress_p, status);
      if (status == DNS_SUCCESS) {
	msg_p = cdsExport_Progress(&data.d_visa,
		(Progress_t *)data.d_user.ud_resp_p, progress_p, cds_status_p);
      } else
	cdsExport_Exception(&data.d_visa, data.d_user.ud_resp_p, cds_status_p);
      dns_free((char *)rpset);
    } else {
      cds_status_p->er_status = PDU_NoLocalResources;
      cdsExport_Exception(&data.d_visa, (byte_t *)&status, cds_status_p);
    }
    release_user_ctx(&data);
  } else
    cdsExport_Exception(&data.d_visa, data.d_status, cds_status_p);

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " <cds_CreateChild(%s,%s,%s,,%s) %s return(rpc_s_ok)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_Progress(progressBuf,progress_p),
      deb_ascii_uuid(childIDBuf,childID_p),
      deb_ascii_uuid(parentIDBuf,parentID_p),
      deb_ascii_cds_status(cds_statusBuf,cds_status_p)));

  return(rpc_s_ok);

}

/*      
 *  cds_CreateDirectory
 *                                       
 * This routine dispatches a message to ta_create_dir
 * It must take apart the message and pass the arguments to the routine
 * then after the call, it will assemble a return message.
 *
 */               
error_status_t 
cds_CreateDirectory (handle_t         h,
                     cds_Progress_t   *progress_p,
                     cds_Timestamp_t  *actualcts_p,
                     cds_status_t     *cds_status_p)
{
  DEB_ASCII_BUF_rpc_binding_handle(hBuf)
  DEB_ASCII_BUF_cds_Progress(progressBuf)
  DEB_ASCII_BUF_Timestamp(actualctsBuf)
  DEB_ASCII_BUF_cds_status(cds_statusBuf)
  data_t		data;
  Timestamp_u		actualcts;
  byte_t		*msg_p;
  int			status;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " >cds_CreateDirectory(%s,%s)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_Progress(progressBuf,progress_p)));

  setup_return_status(cds_status_p);

  /* Convert RPC structures to local compresed buffer */
  status = get_user_ctx(&data, h, OP_CreateDirectory);
  if (status == DNS_SUCCESS) {
    cdsImport_Progress(&data.d_visa,
		progress_p,(Progress_t *)data.d_user.ud_resp_p, FALSE);
    status = db_begin_operation(&data.d_user);
    if (status == DNS_SUCCESS) {
       status = ta_create_dir(&data.d_user,
		(Progress_t *)data.d_user.ud_resp_p, (Timestamp_t *)actualcts);
       (void)db_end_operation(&data.d_user);
    }
    status = bldout(&data, progress_p, status);
    if (status == DNS_SUCCESS) {
      msg_p = cdsExport_Progress(&data.d_visa,
		(Progress_t *)data.d_user.ud_resp_p, progress_p, cds_status_p);
      if (msg_p) {
	if (FLAG_IS_SET(&progress_p->pr_flags, PR_done))
	  cdsExport_Timestamp(&data.d_visa,
		(Timestamp_t *)actualcts, actualcts_p);
      }
    } else
      cdsExport_Exception(&data.d_visa, data.d_user.ud_resp_p, cds_status_p);
    release_user_ctx(&data);
  } else
    cdsExport_Exception(&data.d_visa, data.d_status, cds_status_p);

  DCE_SVC_DEBUG((
      cds__svc_handle,
		 cds_svc_server,
      svc_c_debug2,
      " <cds_CreateDirectory(%s,%s,%s) %s return(rpc_s_ok)",
      deb_ascii_rpc_binding_handle(hBuf,h),
      deb_ascii_cds_Progress(progressBuf,progress_p),
      deb_ascii_Timestamp(actualctsBuf,actualcts),
      deb_ascii_cds_status(cds_statusBuf,cds_status_p)));

  return(rpc_s_ok);
}

/*
**++
**
**  ROUTINE NAME:       cp_AddCellname
**
**  SCOPE:              PUBLIC - declared in cprpc_hcell.idl
**
**  DESCRIPTION:
**
**  This routine is responsible for dispatching CDS server hierarchical
**  cell commands from the CDS Control Program.  These commands are received
**  via the RPC mechanism.
**
**  INPUTS:
**
**      h                 handle_t for this routine
**      cellname_p        name of cell alias to add
**      how               add as/remove
**      chid_p            UUID of clearinghouse holding master of root
**      cmd_status_p      status of local call
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUT:
**
**      command_status  pointer to return status
**
**  IMPLICIT INPUTS:
**
**      nsgbl_p         pointer to nameserver data
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:
**
**      return          success/failure status code
**                      0 = success
**                      other = failure
**
**  SIDE EFFECTS:       none
**
**--
*/

error_status_t
cp_AddCellname (
                handle_t          h,
                cds_FullName_t   *cellname_p,
                unsigned32        how,
                uuid_t           *chid_p,
                command_status_t *cmd_status_p)
{
    DEB_ASCII_BUF_FullName(cellnameBuf)
    DEB_ASCII_BUF_ObjUID(chidBuf)
    DEB_ASCII_BUF_cds_status(cds_statusBuf)
    data_t            data;
    FullName_t        alias_name;
    command_status_t  status;
    db_stream_t      *stream_p;
    file_info_t      *file_p;
    AddAliasAs_t      as;

    DCE_SVC_DEBUG((
		   cds__svc_handle,
		   cds_svc_server,
		   svc_c_debug2,
		   " >cp_AddCellname(...)\n"));

    status = CDS_SUCCESS;

    /* Convert RPC structures to local compresed buffer */
    status = get_user_ctx(&data, h, how);
    if (status == CDS_SUCCESS) {
        /*
         * do the remote to local conversion(s)
         */
        cdsImport_FullName(&data.d_visa, cellname_p, &alias_name, FALSE);
        
	DCE_SVC_DEBUG((
		       cds__svc_handle,
		       cds_svc_server,
		       svc_c_debug2,
		       " >  cp_AddCellname(%p,%s,%d,%s)\n",
		       h, deb_ascii_FullName(cellnameBuf,&alias_name),
		       how, deb_ascii_ObjUID(chidBuf,chid_p)));
        
        if (status == DNS_SUCCESS) {
            switch (how) {

              case ADDCELLPREFERCMD:
		as = AsPreferred;
		break;

              case ADDCELLALIASCMD:
		as = AsNormal;
		break;
                
              case ADDCELLCHILDCMD:
		as = AsChild;
		break;
                
	      case ADDCELLINHERITCMD:
		as = AsInherit;
		break;

              default:
                status = CDS_ERROR;
                break;
            } /* end switch */
            
	    status = add_cell_alias(&data.d_user,
				    chid_p,
				    as,
				    &alias_name);
        }
    }

    DCE_SVC_DEBUG((
		   cds__svc_handle,
		   cds_svc_server,
		   svc_c_debug2,
                   " <cp_AddCellname(%p) return(%d)\n",
		   h, status));
                 
    *cmd_status_p = status;
    return (rpc_s_ok);
}

/*
 *  cp_RemoveCellname    (similar to cp_AddCellname in structure)
 */
error_status_t
cp_RemoveCellname (
                   handle_t          h,
                   cds_FullName_t   *cellname_p,
                   uuid_t           *chid_p,
                   command_status_t *cmd_status_p)

{
    DEB_ASCII_BUF_FullName(cellnameBuf)
    DEB_ASCII_BUF_ObjUID(chidBuf)
    DEB_ASCII_BUF_cds_status(cds_statusBuf)
    data_t            data;
    FullName_t        alias_name;
    command_status_t  status;
    db_stream_t      *stream_p;
    file_info_t      *file_p;

    DCE_SVC_DEBUG((
		   cds__svc_handle,
		   cds_svc_server,
		   svc_c_debug2,
		   " >cp_RemoveCellname(...)\n"));

    status = CDS_SUCCESS;

    status = get_user_ctx(&data, h, REMCELLCMD);
    if (status == CDS_SUCCESS) {
        /*
         * do the remote to local conversion(s)
         */
        cdsImport_FullName(&data.d_visa, cellname_p, &alias_name, FALSE);

	DCE_SVC_DEBUG((
		       cds__svc_handle,
		       cds_svc_server,
		       svc_c_debug2,
		       " >  cp_RemoveCellname(%p,%s,%s)\n",
		       h, deb_ascii_FullName(cellnameBuf,&alias_name),
		       deb_ascii_ObjUID(chidBuf,chid_p)));
        
        if (status == DNS_SUCCESS) {
            if (status == CDS_SUCCESS) {
                status = remove_cell_alias(&data.d_user,
                                           chid_p,
                                           &alias_name);
            }
        }
    }

    DCE_SVC_DEBUG((
		   cds__svc_handle,
		   cds_svc_server,
		   svc_c_debug2,
                   " <cp_RemoveCellname(%p) return(%d)\n",
		   h, status));

    *cmd_status_p = status;
    return (rpc_s_ok);
}


/* ------------------------------------------------------------------------- */

/*
 * bldout
 *     This routine is called to package up return calls.
 *
 */
static int 
bldout (data_t          *data_p,
        cds_Progress_t  *cds_progress_p,
        int             status)
{
    Progress_t *progress_p = (Progress_t *)data_p->d_user.ud_resp_p;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " >bldout"));

  if (cds_progress_p) {
    if (status == DNS_SUCCESS)
      {
	SET_FLAG(progress_p->pr_flags, PR_done);
      }
    else if (status == DNS_NOTFND)
      {
	CLEAR_FLAG(progress_p->pr_flags, PR_done);
	status = DNS_SUCCESS;
      }
  }

  if (status != DNS_SUCCESS) {
      byte_t *msg_p = data_p->d_user.ud_resp_p;

      PUT32(msg_p, status);
      /* Return partial name on unknown entry */
      if (status == DNS_UNKNOWNENTRY) {
	if (cds_progress_p) {
	
	  FullName_t *resolvedName_p;
	
	  resolvedName_p = 
	    (FullName_t *)SKIP_FullName(progress_p->pr_unresolvedName);

	  /* Be careful, the progress_p is in the output buffer */
	  COPY_FullName(resolvedName_p, msg_p);
	} else
	  COPY_FullName(NullFullName, msg_p);
      }
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " <bldout: return(status=%d)",
        status));

  return(status);
}

/*
 * Validate the format of dna$tower value   (not currently used)
 *
 * Note:
 *	A tower consists of a count of floors, followed by two counted
 *	'strings' of data for each floor.  The count fields are field16
 *	and the data 'string' sizes are counted in bytes.
 *
 * Returns 1 if valid, 0 otherwise
 */
/* static int 
 * chk_Tower (byte_t  *tower_p,
 *            int     size)
 * {
 *     byte_t		*next_p = tower_p;
 *     byte_t              *end_p = tower_p + size;
 *     int			len, i, num_floors;
 * 
 *      * 
 *      * check that there is enough room for the floor count
 *      *
 *     if (end_p < SKIP_word(next_p))
 *       return(0);
 * 
 *     if (next_p[1]) return(0);
 *      *
 *      * Get count of tower floors
 *      *
 *     num_floors = GETL16(next_p);
 * 
 *     for (i = 0; i < num_floors*2; ++i)
 * 	{
 * 	if (end_p < SKIP_word(next_p))
 * 	  return(0);
 * 
 * 	len = GETL16(next_p);
 * 
 * 	if (end_p < SKIP_bytes(next_p, len))
 * 	  return(0);
 * 
 * 	next_p = SKIP_bytes(next_p, len);
 * 	}
 * 
 *   return(1);
 * }
 */

/*
 * Check the values of an atomic value (not currently used)
 *
 * Returns 1 if valid, 0 ptherwise
 */
/*
 * static int 
 * chk_AtomicValue (cds_AtomicValue_t  *value_p,
 *                  int                len)
 * {
 *   switch (value_p->av_valuetype) {
 *   case VT_char:
 *     if (value_p->av_val.av_char_p->op_length > MAXATTRIBUTE)
 *       return(0);
 *     break;
 *   case VT_byte:
 *     if (value_p->av_val.av_byte_p->op_length > MAXATTRIBUTE)
 *       return(0);
 *     break;
 *   }
 *   return(1);
 * }
 */

/* 
 *  Validate tower in replicapointer (not currently used)
 *
 * Return 1 if valid, 0 otherwise
 */
/*
 * static int 
 * chk_ReplicaPointer (cds_ReplicaPointer_t *rp_p)
 * {
 *   return(1);
 * 
 * }
 */

/*
 * Make sure the entry type is valid 
 * 
 * Returns 1 if valid, 0 otherwise
 */
static int 
chk_EntryType (int  type,
               int  flag)
{
  switch (type) {
  case ET_anyDirectName:
      if (flag == FALSE)
      {
	  return(0);
      }
      
  case ET_directory:
  case ET_object:
  case ET_dirOrObj:
  case ET_childPointer:
  case ET_softlink:
  case ET_clearinghouse:
      return(1);
  }
  return(0);
}   
