/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ta_create.c,v $
 * Revision 1.1.17.1  1996/08/09  12:01:13  arvind
 * 	Merge CDS changes from DEC into DCE_1.2.2
 * 	[1996/07/30  15:57 UTC  arvind  /main/DCE_1.2.2/arvind_cds122/1]
 *
 * 	Incorporate fix for OT 13085: CDSD memory leaks.
 * 	[1996/02/18  19:36:50  marty  1.1.15.2]
 *
 * Revision 1.1.15.2  1996/02/18  19:36:50  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:17:31  marty]
 * 
 * Revision 1.1.15.1  1995/12/08  15:32:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:02  root]
 * 
 * Revision 1.1.11.8  1994/08/26  21:55:34  zee
 * 	In create_directory_acs, if DirectoryVersion is >=4.0, set default object
 * 	acl to the catalog manager.
 * 	[1994/08/26  21:49:46  zee]
 * 
 * Revision 1.1.11.7  1994/08/25  19:30:57  proulx
 * 	     Delegation support.
 * 	     [1994/08/25  19:28:12  proulx]
 * 
 * Revision 1.1.11.5  1994/08/16  18:28:07  jd
 * 	drop from janet
 * 	[1994/08/13  16:07:38  jd]
 * 
 * Revision 1.1.11.4  1994/08/03  19:04:44  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:28:46  mccann]
 * 
 * Revision 1.1.11.3  1994/06/23  19:11:11  jd
 * 	Remove evtblk. It no longer exists
 * 	[1994/06/23  15:41:28  jd]
 * 
 * Revision 1.1.11.2  1994/06/09  18:43:58  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:16:32  devsrc]
 * 
 * Revision 1.1.11.1  1994/03/12  22:12:47  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:46:19  peckham]
 * 
 * Revision 1.1.9.1  1993/09/24  16:39:48  peckham
 * 	OT#5876: ta_create_dir uses pointers which have been freed
 * 	    Don't free net_info until end of routine.
 * 	[1993/09/24  16:39:19  peckham]
 * 
 * Revision 1.1.5.4  1993/02/12  21:09:25  peckham
 * 	 * Gen 89, 11-Feb-1993, Alan Peckham
 * 	 *      Several routine exits in ta_create_dir()
 * 	 *	do not close the stream.
 * 	[1993/02/12  19:19:51  peckham]
 * 
 * Revision 1.1.5.3  1992/12/30  14:23:45  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:24:23  zeliff]
 * 
 * Revision 1.1.5.2  1992/12/01  02:37:48  keegan
 * 	cdsd crashes (usually) when creating a directory on behalf of an
 * 	unauthenticated client.
 * 	In 'ta_create_dir', when creating the child entry, the client's identity
 * 	is passed as a parameter for generating the new ACL.  This identity
 * 	should be nil if the client is unauthenticated.
 * 	[1992/11/30  21:11:06  keegan]
 * 
 * Revision 1.1.2.8  1992/07/09  16:36:21  keegan
 * 	Fix authorization bug in directory creation. The bug occurs when the
 * 	parent directory is in the same clearinghouse.
 * 	[1992/07/09  13:41:03  keegan]
 * 
 * Revision 1.1.2.7  1992/06/30  19:42:55  peckham
 * 	OT#4471: release parent_stream_p->db_entry in ta_create_dir().
 * 	Remove unused variables. Formfeed cleanup.
 * 	[1992/06/30  19:41:35  peckham]
 * 
 * Revision 1.1.2.6  1992/06/04  19:06:20  mfox
 * 	Fix hang that occurs then background thread starts during directory creation.
 * 	[1992/06/04  13:44:25  mfox]
 * 
 * Revision 1.1.2.5  1992/05/20  18:22:53  peckham
 * 	CR#3383: Memory leak in create_child(). The db_entry of
 * 	parent_stream_p was not being cleaned up in ta_create_dir().
 * 	[1992/05/20  17:37:44  peckham]
 * 
 * Revision 1.1.2.4  1992/05/13  01:46:03  peckham
 * 	CR#3283: Optimization allowing directory creation with same name
 * 	as object.  Routine ta_create_dir() calls check_exists() directly
 * 	to simultate walktree(), but forgets to change DNS_SUCCESS
 * 	to DNS_ENTRYEXISTS. Fixes also found by lint.
 * 	[1992/05/12  21:12:41  peckham]
 * 
 * Revision 1.1.2.3  1992/04/21  21:07:43  grober
 * 	Modified ta_create_dir to ensure that db_close_stream is
 * 	called before you return.  Not calling db_close_stream could
 * 	cause a potential hang when checkpointing.
 * 	[1992/04/21  18:44:07  grober]
 * 
 * Revision 1.1.2.2  1992/03/22  22:35:39  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:36:36  weisman]
 * 
 * Revision 1.1  1992/01/19  15:25:56  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: ta_create.c
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
 * Abstract:
 *	Functions associated with creating directory or child pointer.
 *
 */ 

#include <server.h>
#include <dce/utc.h>
#include <dce/uuid.h>
#include <dce/assert.h>

#include <back.h>
#include <clerk.h>
#include <dbdef.h>
#include <find.h>
#include <names.h>
#include <replica.h>
#include <sets.h>
#include <security.h>
#include <tlog.h>
#include <uid.h>

#if defined(DCE_SEC)
# include <dce/rpc_cvt.h>
# include <dce/rgybase.h>
# include <dce/aclbase.h>
# include <dce/sec_login.h>
# include <dpeacl.h>
  /* for debugging */
# ifndef DEB_ASCII_BUF_sec_acl
#  define deb_ascii_sec_acl(b,p) deb_ascii_rec(b,p,"NULL sec_acl")
#  define DEB_ASCII_LEN_SEC_ACL DEB_ASCII_LEN_rec
#  define DEB_ASCII_BUF_sec_acl(b) DEB_ASCII_BUF_rec(b)
#  define LOG_ASCII_BUF_sec_acl(b) LOG_ASCII_BUF_rec(b)
# endif
# ifndef DEB_ASCII_BUF_sec_id
#  define deb_ascii_sec_id(b,p) deb_ascii_rec(b,p,"NULL sec_id")
#  define DEB_ASCII_LEN_SEC_ID DEB_ASCII_LEN_rec
#  define DEB_ASCII_BUF_sec_id(b) DEB_ASCII_BUF_rec(b)
#  define LOG_ASCII_BUF_sec_id(b) LOG_ASCII_BUF_rec(b)
# endif
# ifndef DEB_ASCII_BUF_sec_acl_entry
#  define deb_ascii_sec_acl_entry(b,p) deb_ascii_rec(b,p,"NULL sec_acl_entry")
#  define DEB_ASCII_LEN_SEC_ACL_ENTRY DEB_ASCII_LEN_rec
#  define DEB_ASCII_BUF_sec_acl_entry(b) DEB_ASCII_BUF_rec(b)
#  define LOG_ASCII_BUF_sec_acl_entry(b) LOG_ASCII_BUF_rec(b)
# endif
#endif /* DCE_SEC */

extern utc_t ppextend_deltatime_utc;
extern time_quad_t ppextend_deltatime;

#define INITIAL_RECORD_SIZE 4096

bytes_u RPC_ClassVersion[] = 
                    {0x7, 0x7, 0x6, 0x5, 0x2B, 0x16, 0x01, 0x01, 0x01};
unsigned char RPCversion[2] = {1,0};

#if defined(DCE_SEC)

/*
 * Local prototypes
 */
static void
get_all_ace (
    sec_acl_t           *,
    sec_id_t            *,
    sec_id_t            *,
    sec_acl_entry_t    	*,
    int                 );

static void
get_others (
     sec_acl_entry_t *,
     int             );

static void
get_unauth (
     sec_acl_entry_t *,
     int             );

static void
get_nameserver (
     sec_acl_t       *,
     nsgbl_ch_t      *,
     sec_acl_entry_t *,
     int             );

static dns_status_t
create_child_acs (
    user_descriptor_t	*,
    sec_id_foreign_t    *,
    dir_hash_entry_t	*,
    db_data_t		*,
    db_data_t		*,
    nsgbl_ch_t		*);

#endif

static dns_status_t
create (
    user_descriptor_t	*,
    Progress_t		*,
    unsigned int	,
    entrylist_t		*,
    int			,
    Timestamp_t		*);

static void
resolve_dirname (
    FullName_t		*,
    FullName_t		*);

static void
init_pp_timeout (
     ParentPointer_t *);

typedef enum {
    from_ta_create_child,
    from_ta_create_dir
} cre_child_caller_t;

static dns_status_t
create_child (
     dns_status_t       ,
     db_stream_t        *,
     user_descriptor_t	*,
     Progress_t		*,
     sec_id_foreign_t   *,
     ObjUID_t		*,
     Set_t		*,
     ObjUID_t		*,
     cre_child_caller_t  );


/*
 * Initialize parentpointer timeout 
 */
static void 
init_pp_timeout (ParentPointer_t *parent_p)
{
  Timeout_t *const to_p = (Timeout_t *)parent_p->pp_timeout;

  utc_t current;

  /* DTSS expects alignment */
  (void)utc_gettime(&current);
  COPY_utc(&current, to_p->to_expiration);
  COPY_utc(&ppextend_deltatime_utc, to_p->to_extension);

}
#if !defined(DCE_SEC)

/*
 * create_directory_acs
 *     This routine build the acs attribute for a directory
 *
 * Input:
 *     user_p = address of user descriptor
 *     stream_p = address of directory stream 
 *     isroot = is this the root directory being created??
 *
 * Value:
 *	DNS_SUCCESS on successful completion
 *      other dns error code otherwise
*/
/*******************
** not DCE_SEC on **
********************/
dns_status_t 
create_directory_acs (user_descriptor_t  *user_p,
                      db_stream_t        *stream_p,
                      int                isroot)
{
    return(DNS_SUCCESS);
}

/*
 * create_entry_acs
 *     This routine build the acs attribute for an object, soft link or
 * pseudo_directory.
 *
 * Input:
 *     	user_p = address of user descriptor
 *	hash_p = address to hash entry for directory
 *     	rec_p = address of record descriptor
 *     	dir_p = address of directory descriptor or null
 *	ch_p = address of nsgbl_ch for this clearinghouse
 *
 * Value:
 *	DNS_SUCCESS on successful completion
 *     	other dns error code otherwise
 */
dns_status_t 
create_entry_acs (user_descriptor_t  *user_p,
                  dir_hash_entry_t   *hash_p,
                  db_data_t          *rec_p,
                  db_data_t          *dir_p,
                  nsgbl_ch_t         *ch_p)
{
    return(DNS_SUCCESS);
}
/***************
** DCE_SEC on **
****************/
#else  /* DCE_SEC is on */

/*
 * get_all_ace
 *    This routine builds the dacl entry which gives the current
 *  client access to the entry.  Memory is dynamically allocated and
 *  needs to be cleaned up.
 *
 * Parameters:
 *	acl_p		(in)	-
 *	realm_p 	(in)	-
 *	user_p		(in)	- address of user descriptor
 *	sec_p		(in)	-
 *	rec_type	(in)	-
 */
/***************
** DCE_SEC on **
****************/
static void 
get_all_ace (sec_acl_t        *acl_p,
             sec_id_t         *realm_p,
             sec_id_t         *user_p,
             sec_acl_entry_t  *sec_p,
             int              rec_type)
{
    DEB_ASCII_BUF_sec_acl(aclBuf)
    DEB_ASCII_BUF_sec_id(realmBuf)
    DEB_ASCII_BUF_sec_id(userBuf)
    DEB_ASCII_BUF_sec_acl_entry(secBuf)
    unsigned32   uidstatus;			 /* status from uuid fcn  */

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " >get_all_ace(%s,%s,%s,%p,%d)",
        deb_ascii_sec_acl(aclBuf,acl_p),
        deb_ascii_sec_id(realmBuf,realm_p),
        deb_ascii_sec_id(userBuf,user_p),
        sec_p,
        rec_type));

    /* Build acl for creater, Gets all access */
    sec_p->perms = dacl_manager_permset(rec_type);

    /* Make sure user is in same cell as acl */
    if (uuid_equal(&acl_p->default_realm.uuid, &realm_p->uuid, &uidstatus)) 
	{
	sec_p->entry_info.entry_type = sec_acl_e_type_user;
	sec_p->entry_info.tagged_union.id.uuid = user_p->uuid;
	sec_p->entry_info.tagged_union.id.name = user_p->name;
	} 
    else 
	{
	sec_p->entry_info.entry_type = sec_acl_e_type_foreign_user;
	/* Copy principal */
	sec_p->entry_info.tagged_union.foreign_id.id.uuid = user_p->uuid;
	sec_p->entry_info.tagged_union.foreign_id.id.name = user_p->name;
	sec_p->entry_info.tagged_union.foreign_id.realm.uuid = realm_p->uuid;
	sec_p->entry_info.tagged_union.foreign_id.realm.name = realm_p->name;
	}

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " <get_all_ace(%s,%s,%s,%s,%d)",
        deb_ascii_sec_acl(aclBuf,acl_p),
        deb_ascii_sec_id(realmBuf,realm_p),
        deb_ascii_sec_id(userBuf,user_p),
        deb_ascii_sec_acl_entry(secBuf,sec_p),
        rec_type));
}

/* 
 * get_others
 *
 * add all access for others 
 *
 * Parameters:
 *	entry_p (in)	-
 *	type	(in)	-
 */
static void 
get_others (sec_acl_entry_t  *entry_p,
            int              type)
{
    DEB_ASCII_BUF_sec_acl_entry(entryBuf)

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " >get_others(%p,%d)",
        entry_p,
        type));

    /* Give others in cell all access */
    entry_p->perms = dacl_manager_permset(type);
    entry_p->entry_info.entry_type = sec_acl_e_type_any_other;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " <get_others(%s,%d)",
        deb_ascii_sec_acl_entry(entryBuf,entry_p),
        type));
}     
/*
 * get_unauth
 *
 * allow all access with unauthenticated mask
 *
 * Parameters:
 *	entry_p	(in)	
 *	type	(in)
 */
static void 
get_unauth (sec_acl_entry_t  *entry_p,
            int              type)
{
    DEB_ASCII_BUF_sec_acl_entry(entryBuf)

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " >get_unauth(%p,%d)",
        entry_p,
        type));

    /* Allow all access through unauthenticated mask */
    entry_p->perms = dacl_manager_permset(type);
    entry_p->entry_info.entry_type = sec_acl_e_type_unauthenticated;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " <get_unauth(%s,%d)",
        deb_ascii_sec_acl_entry(entryBuf,entry_p),
        type));
}
/*
 * get_nameserver
 *
 * allow all access for this nameserver
 *
 * Parameters:
 *	acl_p	(in)	- address of 
 *	ch_p	(in)	- address of clearinghouse info (nsgbl_ch)
 *	entry_p	(in)	- address of entry
 *	type	(in)	- type of entry?
 */
static void 
get_nameserver (sec_acl_t        *acl_p,
                nsgbl_ch_t       *ch_p,
                sec_acl_entry_t  *entry_p,
                int              type)
{
    DEB_ASCII_BUF_sec_acl(aclBuf)
    DEB_ASCII_BUF_nsgbl_ch(chBuf)
    DEB_ASCII_BUF_sec_acl_entry(entryBuf)
    sec_id_t realm, server;
    nsgbl_ns_t *const ns_p = ch_p->ch_nameserver;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " >get_nameserver(%p,%s,%p,%d)",
        deb_ascii_sec_acl(aclBuf,acl_p),
        deb_ascii_nsgbl_ch(chBuf,ch_p),
        entry_p,
        type));

/* Wait for security initialization to complete, if necessary. */

    if ((thstatus = dthread_lock(&(ns_p->ns_sec_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

     /* Beginning of the 'protected code' */

    while (ns_p->ns_sec_state != cds_sec_On) 
      {

          int thstatus;

          if ((thstatus = dthread_wait(&(ns_p->ns_sec_state_changed),
                                       &(ns_p->ns_sec_mutex))) < 0)
          {
	    dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, (long)thstatus);
          }
      }
    realm.uuid = ns_p->ns_Realm_uuid; 
    realm.name = ns_p->ns_Realm; 
    server.uuid = ns_p->ns_Principal_uuid; 
    server.name = ns_p->ns_Principal;

    /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(ns_p->ns_sec_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    get_all_ace(acl_p, &realm, &server, entry_p, type);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " <get_nameserver(%s,%s,%s,%d)",
        deb_ascii_sec_acl(aclBuf,acl_p),
        deb_ascii_nsgbl_ch(chBuf,ch_p),
        deb_ascii_sec_acl_entry(entryBuf,entry_p),
        type));
}

/*
 * create_directory_acs
 *     This routine builds the initial dacl for a directory.
 *     Memory allocated by rpc_ss_allocate will be cleaned up in
 *     the stubs.
 *
 * Parameters:
 *	user_p		(in) 	- address of user descriptor
 *	stream_p	(in)	- address of stream information
 *	isroot		(in)	- flag set if have root directory
 *
 * Returns:
 *      DNS_SUCCESS on successful completion
 *      DNS_NONSRESOURCES
 *      other dns error code otherwise
 */
dns_status_t 
create_directory_acs (user_descriptor_t  *user_p,
                      db_stream_t        *stream_p,
                      int                isroot)
{
    DEB_ASCII_BUF_user_descriptor(userBuf)
    DEB_ASCII_BUF_db_stream(streamBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    Update_t		upd_buf;		 /* update buffer  */
    sec_acl_t           acl;
    sec_acl_entry_t     sec[5];
    byte_t              *flat_acl_p;
    int                 len;
    register dns_status_t status;		 /* current & exit status  */
    int thstatus;
    version_t           dir_version;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " >create_directory_acs(%s,%s,%d)",
        deb_ascii_user_descriptor(userBuf,user_p),
        deb_ascii_db_stream(streamBuf,stream_p),
        isroot));

    /* Initialize the acl */
    acl.sec_acl_manager_type = *(uuid_t *)dacl_manager_type(ET_directory);
    acl.num_entries = 0;
    acl.sec_acl_entries = sec;
    if (user_p->ud_is_pac)
	acl.default_realm = user_p->ud_pac.realm;
    else 
	{
	nsgbl_ns_t *const ns_p = stream_p->db_file_p->file_ch_p->ch_nameserver;
	/* Wait for security initialization to complete, if necessary.
	 * The realm name is initialized there.
	 */

      if ((thstatus = dthread_lock(&(ns_p->ns_sec_mutex))) < 0)
      {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
      }

       /* Beginning of the 'protected code' */

	while (ns_p->ns_sec_state != cds_sec_On) 
	    {
              if ((thstatus = dthread_wait(&(ns_p->ns_sec_state_changed),
                                           &(ns_p->ns_sec_mutex))) < 0)
              {
		dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, (long)thstatus);
	      }
	  }
	acl.default_realm.uuid =
		stream_p->db_file_p->file_ch_p->ch_nameserver->ns_Realm_uuid;
	acl.default_realm.name = (unsigned_char_p_t)
		stream_p->db_file_p->file_ch_p->ch_nameserver->ns_Realm;

	/* End of the 'protected code' */

      if ((thstatus = dthread_unlock(&(ns_p->ns_sec_mutex))) < 0)
      {
        dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
      }
    }  /* end of else */

    /* If this is the root directory or no user pac, add world everything  */
    if (isroot || !user_p->ud_is_pac) 
	{

	/* Give all users in same realm access  */
	get_others(&acl.sec_acl_entries[acl.num_entries++], ET_directory);

	/* Set unauthenticated mask */ 
	get_unauth(&acl.sec_acl_entries[acl.num_entries++], ET_directory);
	} 
    else if (user_p->ud_is_pac && !user_p->ud_pac.authenticated)
	/* Set unauthenticated mask */ 
	get_unauth(&acl.sec_acl_entries[acl.num_entries++], ET_directory);


    /* Add for self */
    if (user_p->ud_is_pac) 
	get_all_ace(&acl, &user_p->ud_pac.realm, &user_p->ud_pac.principal,
		    &acl.sec_acl_entries[acl.num_entries++], 
		    ET_directory);

    /* Next entry is for nameserver */
    get_nameserver(&acl, stream_p->db_file_p->file_ch_p,
		   &acl.sec_acl_entries[acl.num_entries++], 
		   ET_directory);

    if (!acl.num_entries) 
	{
	status = DNS_SUCCESS;
	goto leave_create_directory_acs;
	}

    /* convert acl to flat format for storage */
    if (dpe_acl_wire_to_flat(&acl, DPE_ACL_STDMEM, (char **)&flat_acl_p, &len) 
	== dpe_acl_failure) 
	{
	status = DNS_NONSRESOURCES;
	goto leave_create_directory_acs;
	}

    /* Build update buffer */
    (void)update_dns_build(&upd_buf, UD_present, NULL, 
				    dns_Attribute(DNS_OBJECTACL), BL_true,
				    len, flat_acl_p);
    /* Release dynamic memory */
    free((char *)flat_acl_p);

    /* Add object acl  to the record */
    status = set_insert(&stream_p->db_dir, &upd_buf,
		dns_Attribute(DNS_OBJECTACL), FALSE);

    /* If this is the root directory, add world everything for initials */
    if ((status == DNS_SUCCESS) && isroot) 
	{

	/* Already set up from get_others and get_unauth above */
	acl.num_entries = 2;

	/* Give all users in same realm access  */
	get_others(&acl.sec_acl_entries[0], ET_directory);

	/* Set unauthenticated mask */ 
	get_unauth(&acl.sec_acl_entries[1], ET_directory);

	/* convert acl to flat format for storage */
	if (dpe_acl_wire_to_flat(&acl, DPE_ACL_STDMEM,
			(char **)&flat_acl_p, &len) == dpe_acl_success) 
	    {
	    /* Build update buffer for initial container acl */
	    (void)update_dns_build(&upd_buf, UD_present, NULL, 
			     dns_Attribute(DNS_DEFCONTAINERACL), BL_true,
			     len, flat_acl_p);
	    
	    /* Add it to the record */
	    status = set_insert(&stream_p->db_dir, &upd_buf,
				dns_Attribute(DNS_DEFCONTAINERACL), FALSE);
	    free((char *)flat_acl_p);
	    
	    /* Default object acl has different access rights,
	     * Rebuild - count still 2
	     */
	    /* Give all users in same realm access  */
	    get_others(&acl.sec_acl_entries[0], ET_object);
	    
	    /* Set unauthenticated mask */ 
	    get_unauth(&acl.sec_acl_entries[1], ET_object);
	    
	    /*
	     * If DirectoryVersion is 4.0, acl manager type should be cat.uuid.
	     * However, for backward compatibility, if DirectoryVersion is 3.0,
	     * the acl manager type will remain the same, old, wrong
	     * ET_directory.
	     */
	    db_dir_get_version(stream_p, &dir_version);
	    if (EXT8(dir_version.vr_major) >= 4) {
		acl.sec_acl_manager_type = *(uuid_t *)dacl_manager_type(ET_object);
	    }

	    /* convert acl to flat format for storage */
	    if (dpe_acl_wire_to_flat(&acl, DPE_ACL_STDMEM,
			(char **)&flat_acl_p, &len) == dpe_acl_success) 
		{
		/* Build update buffer for initial object acl */
		(void)update_dns_build(&upd_buf, UD_present, NULL, 
				 dns_Attribute(DNS_DEFOBJECTACL), BL_true,
				 len, flat_acl_p);
		
		/* Add it to the record */
		status = set_insert(&stream_p->db_dir, &upd_buf,
				    dns_Attribute(DNS_DEFOBJECTACL), FALSE);
		    
		free((char *)flat_acl_p);
		} 
	    else 
		status = DNS_NONSRESOURCES;
	    }  /* End convert acl to flat format for storage */
	else 
	    status = DNS_NONSRESOURCES;
	} /* end not root dir, add world everything for initials */

    if (status == DNS_SUCCESS)
	{
        (void)set_timestamp_set(&stream_p->db_dir,
		(Timestamp_t *)upd_buf.ud_timestamp, 
		dns_Attribute(DNS_UTS), NULL, 0);
        (void)set_timestamp_set(&stream_p->db_dir,
		(Timestamp_t *)upd_buf.ud_timestamp, 
		dns_Attribute(DNS_LASTUPDATE), NULL, 0);
	}

leave_create_directory_acs:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " <create_directory_acs(%s,%s,%d) return(%s)",
        deb_ascii_user_descriptor(userBuf,user_p),
        deb_ascii_db_stream(streamBuf,stream_p),
        isroot,
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

/*
 * create_entry_acs
 *     This routine build the acs attribute for a catlog entry or 
 *     pseudo_directory.  
 *     Memory allocated by rpc_ss_allocate will be cleaned up in
 *     the stubs.
 *
 * Parameters:
 *	user_p	(in)	- address of user descriptor
 *	hash_p	(in)	- address of directory hash entry
 *	rec_p	(in)	- address of record descirptor
 *	dir_p	(in)	- address of directory descriptor or null
 *	ch_p	(in)	- address of clearinghouse info (nsgbl_ch)
 *
 * Returns:
 *      DNS_SUCCESS on successful completion
 *      DNS_NONSRESOURCES
 *      other dns error code otherwise
 *
 */
dns_status_t 
create_entry_acs (user_descriptor_t  *user_p,
                  dir_hash_entry_t   *hash_p,
                  db_data_t          *rec_p,
                  db_data_t          *dir_p,
                  nsgbl_ch_t         *ch_p)
{
    DEB_ASCII_BUF_user_descriptor(userBuf)
    DEB_ASCII_BUF_dir_hash_entry(hashBuf)
    DEB_ASCII_BUF_db_data(recBuf)
    DEB_ASCII_BUF_db_data(dirBuf)
    DEB_ASCII_BUF_nsgbl_ch(chBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    Update_t            upd_buf;		 /* update buffer  */
    sec_acl_t           acl;
    sec_acl_entry_t     sec[6], entry;
    byte_t              *flat_acl_p;
    int                 len;
    register dns_status_t status;		 /* current & exit status  */
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " >create_entry_acs(%s,%s,%s,%s,%s)",
        deb_ascii_user_descriptor(userBuf,user_p),
        deb_ascii_dir_hash_entry(hashBuf,hash_p),
        deb_ascii_db_data(recBuf,rec_p),
        deb_ascii_db_data(dirBuf,dir_p),
        deb_ascii_nsgbl_ch(chBuf,ch_p)));

    /* Initialize the acl */
    acl.sec_acl_manager_type = *(uuid_t *)dacl_manager_type(rec_p->data_type);
    acl.num_entries = 0;
    acl.sec_acl_entries = (sec_acl_entry_t *)NULL;
    if (user_p->ud_is_pac)
	acl.default_realm = user_p->ud_pac.realm;
    else  
	{
	nsgbl_ns_t *const ns_p = ch_p->ch_nameserver;
	/* Wait for security initialization to complete, if necessary.
	 * The realm name is initialized there.
	 */

      if ((thstatus = dthread_lock(&(ns_p->ns_sec_mutex))) < 0)
      {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
      }

       /* Beginning of the 'protected code' */
	while (ns_p->ns_sec_state != cds_sec_On) 
	    {
              if ((thstatus = dthread_wait(&(ns_p->ns_sec_state_changed),
                                           &(ns_p->ns_sec_mutex))) < 0)
              {
		dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, (long)thstatus);
              }
	  }
	acl.default_realm.uuid = ch_p->ch_nameserver->ns_Realm_uuid;

	/* Get local realm and add it here */
	acl.default_realm.name =  
	  (unsigned_char_p_t)ch_p->ch_nameserver->ns_Realm;

     /* End of the 'protected code' */
     if ((thstatus = dthread_unlock(&(ns_p->ns_sec_mutex))) < 0)
     {
        dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
     }
    } /* end of else */

    if (rec_p->data_type == ET_clearinghouse) 
	{
	/* First entry is for creator */
	acl.sec_acl_entries = sec;

	if (user_p->ud_is_pac) 
	    {
	    get_all_ace(&acl, &user_p->ud_pac.realm, &user_p->ud_pac.principal,
			 &acl.sec_acl_entries[acl.num_entries++], 
			 rec_p->data_type);
	    if (!user_p->ud_pac.authenticated)
	      /* Set unauthenticated mask */ 
	      get_unauth(&acl.sec_acl_entries[acl.num_entries++], 
			 rec_p->data_type);
	    } 
	else 
	    {
	    /* Give all users in same realm access  */
	    get_others(&acl.sec_acl_entries[acl.num_entries++], 
		       rec_p->data_type);

	    /* Set unauthenticated mask */ 
	    get_unauth(&acl.sec_acl_entries[acl.num_entries++], 
		       rec_p->data_type);
	    }
	/* Next entry is for nameserver */
	get_nameserver(&acl, ch_p, &acl.sec_acl_entries[acl.num_entries++], 
		       ET_clearinghouse);

	if (!acl.num_entries) 
	    {
	    status = DNS_SUCCESS;
	    goto leave_create_entry_acs;
	    }

	/* convert acl to flat format for storage */
	if (dpe_acl_wire_to_flat(&acl, DPE_ACL_STDMEM,
			(char **)&flat_acl_p, &len) == dpe_acl_failure) 
	    {
	    status = DNS_NONSRESOURCES;
	    goto leave_create_entry_acs;
	    }

	/* Build update buffer */
	(void)update_dns_build(&upd_buf, UD_present, NULL, 
				    dns_Attribute(DNS_OBJECTACL), BL_true,
				    len, flat_acl_p);
	/* Release dynamic memory */
	free((char *)flat_acl_p);

	} 
    else 
	{ 
	sec_acl_t  *defacl_p;

	if (dacl_get(dir_p, &defacl_p, DNS_DEFOBJECTACL) != DNS_SUCCESS)
	  defacl_p = &acl;

	if (user_p->ud_is_pac) 
	    {
	    get_all_ace(defacl_p, &user_p->ud_pac.realm, 
			&user_p->ud_pac.principal, 
			&entry, rec_p->data_type);
	    dacl_add_acl_entry(&entry, defacl_p, FALSE);

	    if (!user_p->ud_pac.authenticated) 
		{
		/* Set unauthenticated mask */ 
		get_unauth(&entry, rec_p->data_type);
		dacl_add_acl_entry(&entry, defacl_p, FALSE);
		}

	    /* 
	     * Give softlinks and clearinghouse objects
	     * world read since this is what is probably wanted anyway
             */
	    if ((rec_p->data_type == ET_softlink) || 
		((rec_p->data_type == ET_object) &&
		(set_dnstest(rec_p->data_p,
		         LEN_ClassName(cds_classlist[DNS_CLEARINGHOUSE].cname),
			 cds_classlist[DNS_CLEARINGHOUSE].cname,
			 dns_Attribute(DNS_CLASS))))) 
		{
		/* Give all users in same realm read access  */
		get_others(&entry, rec_p->data_type);
		entry.perms = sec_acl_perm_read;
		dacl_add_acl_entry(&entry, defacl_p, TRUE);

		/* Set unauthenticated mask */ 
		get_unauth(&entry, rec_p->data_type);
		entry.perms = sec_acl_perm_read;
		dacl_add_acl_entry(&entry, defacl_p, TRUE);
		}

	    } 			    /* End have pac  */
	else 
	    {			    /* Give all users all access if no pac */
	    get_others(&entry, rec_p->data_type);
	    dacl_add_acl_entry(&entry, defacl_p, FALSE);

	    /* Set unauthenticated mask */ 
	    get_unauth(&entry, rec_p->data_type);
	    dacl_add_acl_entry(&entry, defacl_p, FALSE);
	    }
	
	if (defacl_p->num_entries == 0) 
	    {
	    status = DNS_SUCCESS;
	    goto leave_create_entry_acs;
	    }

	/* convert acl to flat format for storage */
	if (dpe_acl_wire_to_flat(defacl_p, DPE_ACL_STDMEM,
			(char **)&flat_acl_p, &len) ==  dpe_acl_failure) 
	    {
	    status = DNS_NONSRESOURCES;
	    goto leave_create_entry_acs;
	    }

	/* Build update buffer */
	(void)update_dns_build(&upd_buf, UD_present, 
			 NULL, 
			 dns_Attribute(DNS_OBJECTACL), BL_true,
			 len, flat_acl_p);
	free((char *)flat_acl_p);
	} 					 /* Not clearinghouse record */


    status = set_insert(rec_p, &upd_buf, dns_Attribute(DNS_OBJECTACL), FALSE);
    if (status == DNS_SUCCESS) 
	{
	(void)set_timestamp_set(rec_p, (Timestamp_t *)upd_buf.ud_timestamp,
			  dns_Attribute(DNS_UTS), NULL, 0);
	if (dir_p)
	    db_dir_set_lastupdate(hash_p, 
				(Timestamp_t *)upd_buf.ud_timestamp);
	}

leave_create_entry_acs:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " <create_entry_acs(%s,%s,%s,%s,%s) return(%s)",
        deb_ascii_user_descriptor(userBuf,user_p),
        deb_ascii_dir_hash_entry(hashBuf,hash_p),
        deb_ascii_db_data(recBuf,rec_p),
        deb_ascii_db_data(dirBuf,dir_p),
        deb_ascii_nsgbl_ch(chBuf,ch_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

/*
 * create_child_acs
 *     This routine build the acs attribute for a child pointer.
 *     Memory allocated by rpc_ss_allocate will be cleaned up in
 *     the stubs.
 *
 * Parameters:
 *	user_p		(in)	 - address of user descriptor 
 *	onbehalf_p	(in)	 - address of client 
 *	hash_p		(in)	 - address of directory hash entry 
 *	rec_p		(in)	 - address of record descirptor
 *	dir_p		(in)	 - address of directory descriptor or null
 *	ch_p		(in)	 - address of clearinghouse info (nsgbl_ch)
 *
 * Returns:
 *	DNS_SUCCESS on successful completion
 *	DNS_NONSRESOURCES
 * 	other dns error code otherwise
 */
static dns_status_t 
create_child_acs (user_descriptor_t  *user_p,
                  sec_id_foreign_t   *onbehalf_p,
                  dir_hash_entry_t   *hash_p,
                  db_data_t          *rec_p,
                  db_data_t          *dir_p,
                  nsgbl_ch_t         *ch_p)
{
    DEB_ASCII_BUF_user_descriptor(userBuf)
    DEB_ASCII_BUF_dir_hash_entry(hashBuf)
    DEB_ASCII_BUF_db_data(recBuf)
    DEB_ASCII_BUF_db_data(dirBuf)
    DEB_ASCII_BUF_nsgbl_ch(chBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    Update_t              upd_buf;		 /* update block   */
    sec_acl_t             acl;			 
    sec_acl_entry_t       entry;
    sec_acl_t             *defacl_p;
    byte_t                *flat_acl_p;
    nsgbl_ns_t            *ns_p;
    int                   len;
    register dns_status_t status;		 /* current & exit status  */
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " >create_child_acs(%s,,%s,%s,%s,%s)",
        deb_ascii_user_descriptor(userBuf,user_p),
        deb_ascii_dir_hash_entry(hashBuf,hash_p),
        deb_ascii_db_data(recBuf,rec_p),
        deb_ascii_db_data(dirBuf,dir_p),
        deb_ascii_nsgbl_ch(chBuf,ch_p)));

    /* Initialize the acl */
    acl.sec_acl_manager_type = *(uuid_t *)dacl_manager_type(rec_p->data_type);
    acl.num_entries = 0;
    acl.sec_acl_entries = (sec_acl_entry_t *)NULL;
    if (user_p->ud_is_pac)
	acl.default_realm = user_p->ud_pac.realm;
    else  if (onbehalf_p) 
	acl.default_realm = onbehalf_p->realm;
    else 
	{
	/* Wait for security initialization to complete, if necessary.
	 * The realm name is initialized there.
	 */
	ns_p = ch_p->ch_nameserver;

      if ((thstatus = dthread_lock(&(ns_p->ns_sec_mutex))) < 0)
      {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
      }

	/* Beginning of the 'protected code' */

	while (ns_p->ns_sec_state != cds_sec_On) 
	    {
              if ((thstatus = dthread_wait(&(ns_p->ns_sec_state_changed),
                                           &(ns_p->ns_sec_mutex))) < 0)
              {
		dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, (long)thstatus);
              }
	  }
	acl.default_realm.uuid = ch_p->ch_nameserver->ns_Realm_uuid;
      
	/* Get local realm and add it here */
	acl.default_realm.name =  
	  (unsigned_char_p_t)ch_p->ch_nameserver->ns_Realm;

	/* End of the 'protected code' */

	if ((thstatus = dthread_unlock(&(ns_p->ns_sec_mutex))) < 0)
	{
	  dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
	}
    } /* end of else */

    if (dacl_get(dir_p, &defacl_p, DNS_DEFOBJECTACL) != DNS_SUCCESS)
	defacl_p = &acl;

    /* 
     * If their is not a default entry for other_user and
     * this is a clearinghouse object,
     * give cell members read access 
     */
    
    if (user_p->ud_is_pac) 
	{
	get_all_ace(defacl_p, &user_p->ud_pac.realm, &user_p->ud_pac.principal, 
		     &entry, rec_p->data_type);
	dacl_add_acl_entry(&entry, defacl_p, FALSE);
	if (!user_p->ud_pac.authenticated) 
	    {
	    /* Set unauthenticated mask */ 
	    get_unauth(&entry, rec_p->data_type);
	    dacl_add_acl_entry(&entry, defacl_p, FALSE);
	    }
	if (onbehalf_p) 
	    {
	    get_all_ace(defacl_p, &onbehalf_p->realm, &onbehalf_p->id, &entry,
			ET_childPointer);
	    dacl_add_acl_entry(&entry, defacl_p, FALSE);

	    }
	
	} 
    else if (onbehalf_p) 
	{
	get_all_ace(defacl_p, &onbehalf_p->realm, &onbehalf_p->id, &entry,
		     ET_childPointer);
	dacl_add_acl_entry(&entry, defacl_p, FALSE);
	} 
    else 
	{
	/* Give all users all access if no pac */
	get_others(&entry, rec_p->data_type);
	dacl_add_acl_entry(&entry, defacl_p, FALSE);
	
	/* Set unauthenticated mask */ 
	get_unauth(&entry, rec_p->data_type);
	dacl_add_acl_entry(&entry, defacl_p, FALSE);
	}

    if (defacl_p->num_entries == 0) 
	{
	status = DNS_SUCCESS;
	goto leave_create_child_acs;
	}
    
    /* convert acl to flat format for storage */
    if (dpe_acl_wire_to_flat(defacl_p, DPE_ACL_STDMEM,
				(char **)&flat_acl_p, &len) == dpe_acl_failure)
	status = DNS_NONSRESOURCES;
    else 
	{

	/* Build update buffer */
	(void)update_dns_build(&upd_buf, UD_present, 
			 NULL, 
			 dns_Attribute(DNS_OBJECTACL), BL_true,
			 len, flat_acl_p);
	free((char *)flat_acl_p);
	status = set_insert(rec_p, &upd_buf, 
			    dns_Attribute(DNS_OBJECTACL), FALSE);
	}
    if (status == DNS_SUCCESS) 
	{
	(void)set_timestamp_set(rec_p, (Timestamp_t *)upd_buf.ud_timestamp,
			  dns_Attribute(DNS_UTS), NULL, 0);
	db_dir_set_lastupdate(hash_p, 
				(Timestamp_t *)upd_buf.ud_timestamp);
	}
    
leave_create_child_acs:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " <create_child_acs(%s,,%s,%s,%s,%s) return(%s)",
        deb_ascii_user_descriptor(userBuf,user_p),
        deb_ascii_dir_hash_entry(hashBuf,hash_p),
        deb_ascii_db_data(recBuf,rec_p),
        deb_ascii_db_data(dirBuf,dir_p),
        deb_ascii_nsgbl_ch(chBuf,ch_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}
#endif /* DCE_SEC */


/*
 * create_record 
 *     This routine receives a list of entries to be placed in a new
 * record.  No order is assumed.
 *
 * Input
 *     rec_p = address of data descriptor
 *     numentries = count of attributes to build
 *     elist_p = attribute descriptors
 *
 * Value:
 *	DNS_SUCCESS on successful completion
 *      other dns error code otherwise
 */
dns_status_t 
create_record (db_data_t    *rec_p,
               int          numentries,
               entrylist_t  *elist_p)
{
    DEB_ASCII_BUF_db_data(recBuf)
    DEB_ASCII_BUF_entrylist(elistBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    Timestamp_u		ts;			 /* current timestamp  */
    DBSet_t		*out_set_p, 		 /* working sets  */
			*set_p;
    int			i,cnt;			 /* indicies  */
    int                 next,			 /* index to next entry  */
			last;			 /* CDS specific?  */
    dns_status_t	status;			 /* current & exit status  */

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug4,
        " >create_record(%s,%d,%s)",
        deb_ascii_db_data(recBuf,rec_p),
        numentries,
        deb_ascii_entrylist(elistBuf,elist_p)));

    out_set_p = (DBSet_t *)rec_p->data_p->dbr_data;
    ALIGN_CHECK(out_set_p);

    /* Initialize header */
    INS32(out_set_p->ds_casemask, 0);
    INS8(out_set_p->ds_flag, DS_present);
    COPY_Timestamp(NullTimestamp, out_set_p->ds_ts);
    INS8(out_set_p->ds_type, AT_set);
    out_set_p->ds_ocnt = numentries;
    out_set_p->ds_hcnt = 0;		/* no delete holders */

     /* Set to null in case nothing generated */
    COPY_Timestamp(NullTimestamp, ts);

    set_p = (DBSet_t *)DATA_DBSet(out_set_p);

    last = -1;

    /* Insert remaining attributes - add in lexicalorder */
    for (cnt = 0; cnt < numentries; cnt++)
	{
	attribute_descriptor_t *dns_p;

	/* 
	 * Ordering is not assumed within elist (cause of object-id)
         * but the value of the type fields => lexical order
         */
	 next = -1;
	 for (i=0; i<numentries; i++) 
	    {
	    if (last != -1)
		if (elist_p[i].el_type <= last) continue;
	    if (next == -1)
		next = i;
	    else if (elist_p[i].el_type < elist_p[next].el_type) 
		next=i;
	    }
	last = elist_p[next].el_type;
	dns_p = dns_Attribute(elist_p[next].el_type);

	/* Insure that the DBSet following the name is aligned */
	set_p = (DBSet_t *)ALIGN_PTR(SKIP_bytes(set_p,
				LEN_AttributeName(dns_p->aname)));
	set_p = (DBSet_t *)SKIP_bytes(set_p, -LEN_AttributeName(dns_p->aname));

	/* this is the right offset */
	out_set_p->ds_offp[cnt] = DIFF_bytes(set_p, out_set_p);

        if (elist_p[next].el_type == DNS_ALLUPTO)
	    (void)set_member(&set_p,
		       (AttributeName_t *)dns_p->aname, dns_p->casemask,
		       NullTimestamp, elist_p[next].el_value_p, 
		       elist_p[next].el_len, 
		       dns_p->vtype, BL_true, DM_present, dns_p->type);
	else if (dns_Attribute(elist_p[next].el_type)->rep_rec)
	    (void)set_member(&set_p,
		       (AttributeName_t *)dns_p->aname, dns_p->casemask,
		       NullTimestamp, elist_p[next].el_value_p, 
		       elist_p[next].el_len,
		       dns_p->vtype, BL_true, DM_present, dns_p->type);
        else if (elist_p[next].el_type == DNS_CTS) 
	    {
	    /* With CDS CTS may be the only attribute, it will be there */
	    if (EQ_Timestamp(ts, NullTimestamp))
		COPY_Timestamp(elist_p[next].el_value_p, ts);
	    (void)set_member(&set_p,
		       (AttributeName_t *)dns_p->aname, dns_p->casemask,
		       (Timestamp_t *)elist_p[next].el_value_p,	
		       elist_p[next].el_value_p, elist_p[next].el_len, 
		       dns_p->vtype, BL_true, DM_present, dns_p->type);
	    }
	else
            {
            status = ts_new((Timestamp_t *)ts);
	    if (status != DNS_SUCCESS)
		goto leave_create_record;
	    (void)set_member(&set_p,
		       (AttributeName_t *)dns_p->aname, dns_p->casemask,
		       (Timestamp_t *)ts, elist_p[next].el_value_p, 
		       elist_p[next].el_len,
		       dns_p->vtype, BL_true, DM_present, dns_p->type);
            }
	} /* End adding remaining attributes in lexical order */

    /* update set length */
    out_set_p->ds_setl = DIFF_bytes(set_p, out_set_p);
    (void)set_timestamp_set(rec_p, (Timestamp_t *)ts, dns_Attribute(DNS_UTS),
	NULL, 0);

    status = DNS_SUCCESS;

leave_create_record:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug4,
        " <create_record(%s,%d,%s) return(%s)",
        deb_ascii_db_data(recBuf,rec_p),
        numentries,
        deb_ascii_entrylist(elistBuf,elist_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

/*
 * create
 *   This routine makes sure an entry does not exist and then creates it.
 * It receives a description of attributes with one value each, to put into the
 * entry.  An acl is created.  The entries must be in alphabetical order.
 * The lastupdate attribute of the parent is updated in memory.  A propagate
 * message is sent to the background so the new entry is distributed
 * to all clearinghouses where the parent directory is replicated.
 *
 * Input:
 *      user_p = address of user descriptor
 *      progress_p = address of progress record
 *      numentries = number of sets to create
 *      entrylist_p = address of entry descriptors
 *      type = entry type to create
 *
 * Output:
 *      actual_cts_p = address to return actual cts
 *
 * Side effects: 
 *      Progress record updated
 *
 * Function value:
 *	DNS_SUCCESS on successful create
 *	DNS_NONSRESOURCES
 *	DNS_UNKNOWNENTRY
 *      other dns error codes otherwise
 *
 */
static dns_status_t 
create (user_descriptor_t  *user_p,
        Progress_t         *progress_p,
        unsigned int       numentries,
        entrylist_t        *entrylist_p,
        int                type,
        Timestamp_t        *actual_cts_p)
{
    DEB_ASCII_BUF_user_descriptor(userBuf)
    DEB_ASCII_BUF_Progress(progressBuf)
    DEB_ASCII_BUF_entrylist(entrylistBuf)
    DEB_ASCII_BUF_Timestamp(actual_ctsBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    db_stream_t *stream_p;			 /* stream information  */
    dns_status_t status;			 /* current & exit status */

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug4,
        " >create(%s,%s,%d,%s,%d,%p)",
        deb_ascii_user_descriptor(userBuf,user_p),
        deb_ascii_Progress(progressBuf,progress_p),
        numentries,
        deb_ascii_entrylist(entrylistBuf,entrylist_p),
        type,
        actual_cts_p));

    if (type == ET_object)
	status = db_open_stream(user_p->ud_chuid_p, &stream_p, OP_CreateObject);
    else
	status = db_open_stream(user_p->ud_chuid_p, &stream_p, OP_CreateLink);
    if (status != DNS_SUCCESS)
	goto leave_create;

    /* 
     * On success, the directory is returned locked, so no other 
     * thread can sneak in and use the same name.  It must remain locked 
     * until the entry itself is created.
     */
    status = walktree(user_p, type, WALKTREE_CREATE_OP, progress_p, stream_p);
    if (status == DNS_ENTRYEXISTS)
	(void)db_close_stream(stream_p);
    if (status != DNS_SUCCESS)
	goto leave_create;

    /* Need Insert or Owner access to directory */
    status = security_check(user_p, stream_p, Create_Entry_check, ET_directory,
			    NULL); 
    if (status == DNS_SUCCESS) 
        {
	/* Set up stream descriptor */
	stream_p->db_entry.data_buf_len = INITIAL_RECORD_SIZE;
	stream_p->db_entry.data_p = 
		(data_record_t *)dns_malloc_temp(INITIAL_RECORD_SIZE);
	if (!stream_p->db_entry.data_p)
	    status = DNS_NONSRESOURCES;
	else
	    {
	    status = ts_new(actual_cts_p);
	    if (status == DNS_SUCCESS) 
                status = create_record(&stream_p->db_entry, numentries, 
				entrylist_p);
	    if (status == DNS_SUCCESS) 
                {
		/* Add access control */
	        stream_p->db_entry.data_type = type;
                status = create_entry_acs(user_p, stream_p->db_dir_hash_p,
			&stream_p->db_entry, &stream_p->db_dir, 
			stream_p->db_file_p->file_ch_p);
		if (status == DNS_SUCCESS) 
		    {
		    SimpleName_t *name_p = names_pointlast(
				(FullName_t *)progress_p->pr_unresolvedName);
		    /* Do the create */
                    status = db_entry_create(stream_p, name_p, type);
		    }
                if (status == DNS_SUCCESS) 
                    {
		    /* Complete resolveing the name */
		    (void)names_swap((FullName_t *)progress_p->pr_unresolvedName);
		    if (!low_convergence(stream_p->db_dir.data_p))
                        back_propagate_sched(stream_p, type, actual_cts_p);
                    }
		}
            } /* End allocation succeeded */
        } /* End access allowed */
    else if (status == DNS_UNKNOWNENTRY)
        {
        /* Directory not resolved, need to fix name for exception */
        resolved_null(progress_p);  
        }

    (void)db_close_stream(stream_p);

leave_create:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug4,
        " <create(%s,%s,%d,%s,%d,%p->%s) return(%s)",
        deb_ascii_user_descriptor(userBuf,user_p),
        deb_ascii_Progress(progressBuf,progress_p),
        numentries,
        deb_ascii_entrylist(entrylistBuf,entrylist_p),
        type,
        actual_cts_p,
        deb_ascii_Timestamp(actual_ctsBuf,actual_cts_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

/*
 * ta_create_obj
 *   This routine sets up the entry descriptor list for a new object
 * and calls create to do the actual work.
 *
 *
 * Input:
 *      user_p = address of user descriptor
 *      progress_p = address of progress record
 *      class_p = address of class name (opt in v3.0)
 *      version_p = address of class version (opt in v3.0)
 *      objectid_p = address of objectuid attribute (opt in 3.0)
 *
 * Output:
 *      actual_cts_p = address to return actual cts
 *
 * Side effects: 
 *      Progress record updated
 *
 * Function value:
 *      status returned from create function
 *
 */
/*********************
**  DNS_CDS VERSION **
**********************/
dns_status_t 
ta_create_obj (user_descriptor_t  *user_p,
               Progress_t         *progress_p,
               ClassName_t        *class_p,
               ClassVersion_t     *version_p,
               uuid_t             *objectid_p,
               Timestamp_t        *actual_cts_p)
{
    DEB_ASCII_BUF_user_descriptor(userBuf)
    DEB_ASCII_BUF_ClassName(classBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    entrylist_t  elist[4], 			 /* entry list buffer  */
		 *elist_p = elist;		 /* entry list pointer  */
    dns_status_t status;			 /* exit status  */
    entrylist_t  *temp;				 /* used by dce_assert */

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " >ta_create_obj(%s,,%s,,,)",
        deb_ascii_user_descriptor(userBuf,user_p),
        deb_ascii_ClassName(classBuf,class_p)));

    if (class_p) 
	{
        /* set up attribute descriptors */
        elist_p->el_type = DNS_CLASS;
        elist_p->el_len = LEN_ClassName(class_p);
        elist_p->el_value_p = (unsigned char *)class_p;
        elist_p++;
    
	if (version_p) 
	    {
	    elist_p->el_type = DNS_CLASSVERSION;
	    elist_p->el_len = sizeof(*version_p);
	    elist_p->el_value_p = (unsigned char *)version_p;
	    elist_p++;
	    }
	}

    elist_p->el_type = DNS_CTS;
    elist_p->el_len = sizeof(*actual_cts_p);
    elist_p->el_value_p = (unsigned char *)actual_cts_p;
    elist_p++;

    /* Optional objectuid value */
    if (objectid_p) 
	{
        elist_p->el_type = DNS_OBJECTUID;
        elist_p->el_len = sizeof(*objectid_p);
        elist_p->el_value_p = (unsigned char *)objectid_p;
        elist_p++;
	}
    temp = (entrylist_t *)SKIP_bytes(elist, sizeof(elist));
    dce_assert(cds__svc_handle,
	       (elist_p <= temp));

    status = create(user_p, progress_p, elist_p-elist, elist,
			ET_object, actual_cts_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " <ta_create_obj(%s,,%s,,,) return(%s)",
        deb_ascii_user_descriptor(userBuf,user_p),
        deb_ascii_ClassName(classBuf,class_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

/*
 * ta_create_link
 *   This routine sets up the entry descriptor list for a new soft link
 * and calls create to do the actual work.
 *
 *
 * Input:
 *      user_p = address of user descriptor
 *      progress_p = address of progress record
 *      tartget_p = address of target fullname
 *      timeout_p = address of timeout
 *
 * Output:
 *      actual_cts_p = address to return actual cts
 *
 * Side effects: 
 *      Progress record updated
 *
 * Function value:
 *      status returned from create function
 */
dns_status_t 
ta_create_link (user_descriptor_t  *user_p,
                Progress_t         *progress_p,
                FullName_t         *target_p,
                Timeout_t          *timeout_p,
                Timestamp_t        *actual_cts_p)
{
    DEB_ASCII_BUF_user_descriptor(userBuf)
    DEB_ASCII_BUF_FullName(targetBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    entrylist_t elist[3];			 /* entry list buffer  */
    int i=0;					 /* index  */
    dns_status_t status;			 /* current & exit status  */

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " >ta_create_link(%s,,%s,,)",
        deb_ascii_user_descriptor(userBuf,user_p),
        deb_ascii_FullName(targetBuf,target_p)));

    /* set up attribute descriptors */
    elist[i].el_type = DNS_CTS;
    elist[i].el_len = sizeof(*actual_cts_p);
    elist[i++].el_value_p = (unsigned char *)actual_cts_p;

    elist[i].el_type = DNS_LINKTARGET;
    elist[i].el_len = LEN_FullName(target_p);
    elist[i++].el_value_p = (unsigned char *)target_p;

    /* Optional in CDS */
    if (timeout_p != NULL) 
	{
	elist[i].el_type = DNS_LINKTIMEOUT;
	elist[i].el_len = sizeof(*timeout_p);
	elist[i++].el_value_p = (unsigned char *)timeout_p;
	}

    status = create(user_p, progress_p, i, elist, ET_softlink, actual_cts_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " <ta_create_link(%s,,%s,,) return(%s)",
        deb_ascii_user_descriptor(userBuf,user_p),
        deb_ascii_FullName(targetBuf,target_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

/*
 * resolve_dirname
 *    This routine will resolve the directory name.  Since the directory
 * doesn't exist yet, the input to resolve name is actually the parent
 * name.
 *
 * Input:
 *    directory_p = address of new directory fullname
 *
 * Output:
 *    result_p = address to store resolved name into
 *
 */
static void 
resolve_dirname (FullName_t  *directory_p,
                 FullName_t  *result_p)
{
    SimpleName_u saved_name;			 /* dir last simple name */
    SimpleName_t *last_p, 			 /* ptr to last simple name  */
		 *newlast_p;			 /* resolved last sname  */
    dns_status_t status;			 /* current status  */
     
    /* Assume it won't work */
    COPY_FullName(directory_p, result_p);

    /* Peal off last directory simple name */
    last_p = (SimpleName_t *)names_pointlast(directory_p);
    COPY_SimpleName(last_p, saved_name);
    COPY_SimpleName(NullSimpleName, last_p);
    INS16(directory_p->fn_length,
	EXT16(directory_p->fn_length) - LEN_SimpleName(saved_name));

    status = clerk_resolve_name(directory_p, result_p);

    /* Restore directory simple name */
    COPY_SimpleName(saved_name, last_p);
    INS16(directory_p->fn_length,
	EXT16(directory_p->fn_length) + LEN_SimpleName(last_p));

    /* Not linked, use original name */
    if (status != DNS_SUCCESS) return;

    /* Append original directory simple name to resolved name */
    if (EMPTY_FullName(result_p))
        newlast_p = (SimpleName_t *)result_p->fn_name;  /* no-op, already here*/
    else 
	newlast_p = (SimpleName_t *)SKIP_SimpleName(names_pointlast(result_p));

    /* Copy directory simple name and terminating name */
    COPY_SimpleName(last_p, newlast_p);
    COPY_SimpleName(NullSimpleName, SKIP_SimpleName(newlast_p));

    /* Update length, terminating name already included */
    INS16(result_p->fn_length,
	EXT16(result_p->fn_length) + LEN_SimpleName(last_p));
}

/*
 * ta_create_dir_record
 *	Build the directory record 
 *
 * Input:
 *	user_p = address of user descriptor
 *	stream_p = address of stream descriptor 
 *	version_p = address of version number to use
 *      actual_cts_p = buffer containing directory cts
 *	actual_id_p = buffer containing directory id
 *	replica_p = address of buffer to receive replica pointer or NULL
 *	parent_p = address of parent pointer or NULL
 *
 * Returns:
 *	DNS_SUCCESS on successful completion
 *	other dns error code otherwise
 */
dns_status_t 
ta_create_dir_record (user_descriptor_t  *user_p,
                      db_stream_t        *stream_p,
                      version_t          *version_p,
                      Timestamp_t        *actual_cts_p,
                      ObjUID_t           *actual_id_p,
                      ReplicaPointer_t   *replica_p,
                      ParentPointer_t    *parent_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    DEB_ASCII_BUF_Timestamp(actual_ctsBuf)
    DEB_ASCII_BUF_ObjUID(actual_idBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    entrylist_t		elist[16], 		 /* entry list storage  */
			*elist_p = elist;	 /* entry list pointer  */
    Timestamp_u		skulkcts;		 /* DNS$LASTSKULK value  */
    ObjUID_u		epochuid;		 /* current epoch timestamp  */
    ReplicaPointer_u	replica;		 /* replica data buffer  */
    ReplicaState_u	statevalue;		 /* DNS$REPLICASTATE value  */
    ReplicaType_u	typevalue;		 /* DNS$REPLICATYPE value  */
    byte_u		convalue;		 /* DNS$CONVERGENCE value  */
    byte_u		inchvalue;		 /* DNS$INCHNAME value  */
    nsgbl_ch_t		*ch_p 			 /* clearinghouse information */
			    = stream_p->db_file_p->file_ch_p;
    dns_status_t	status;			 /* current & exit status  */
    entrylist_t  *temp;				 /* used by dce_assert */

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug4,
        " >ta_create_dir_record(,%s,,%s,%s,,)",
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_Timestamp(actual_ctsBuf,actual_cts_p),
        deb_ascii_ObjUID(actual_idBuf,actual_id_p)));

    INS8(convalue, CG_medium);
    INS8(statevalue, RS_on);
    INS8(typevalue, RT_master);

    if (!replica_p) replica_p = (ReplicaPointer_t *)replica;

    /* Build directory attributes in alphabetic order */
    elist_p->el_type = DNS_ALLUPTO;
    elist_p->el_len = sizeof(NullTimestamp);
    elist_p->el_value_p = (byte_t *)NullTimestamp;
    elist_p++;

    elist_p->el_type = DNS_CONVERGENCE;
    elist_p->el_len = sizeof(convalue);
    elist_p->el_value_p = convalue;
    elist_p++;

    elist_p->el_type = DNS_CTS;
    elist_p->el_len = sizeof(*actual_cts_p);
    elist_p->el_value_p = (byte_t *)actual_cts_p;
    elist_p++;

    elist_p->el_type = DNS_DIRECTORYVERSION;
    elist_p->el_len = sizeof(*version_p);
    elist_p->el_value_p = (byte_t *)version_p;
    elist_p++;

    status = id_new((ObjUID_t *)epochuid);
    if (status != DNS_SUCCESS)
	goto leave_ta_create_dir_record;
    elist_p->el_type = DNS_EPOCH;
    elist_p->el_len = sizeof(epochuid);
    elist_p->el_value_p = epochuid;
    elist_p++;

    if (!parent_p) 
	{
	INS8(inchvalue, TRUE);
	elist_p->el_type = DNS_INCHNAME;
	elist_p->el_len = sizeof(inchvalue);
	elist_p->el_value_p = (unsigned char *)inchvalue;
	elist_p++;
	}

    status = ts_new((Timestamp_t *)skulkcts);
    if (status != DNS_SUCCESS)
	goto leave_ta_create_dir_record;
    elist_p->el_type = DNS_LASTSKULK;
    elist_p->el_len = sizeof(skulkcts);
    elist_p->el_value_p = skulkcts;
    elist_p++;

    elist_p->el_type = DNS_LASTUPDATE;
    elist_p->el_len = sizeof(NullTimestamp);
    elist_p->el_value_p = (byte_t *)NullTimestamp;
    elist_p++;

    elist_p->el_type = DNS_OBJECTUID;
    elist_p->el_len = sizeof(*actual_id_p);
    elist_p->el_value_p = (byte_t *)actual_id_p;
    elist_p++;

    if (parent_p) 
	{
	elist_p->el_type = DNS_PARENTPOINTER;
	elist_p->el_len = LEN_ParentPointer(parent_p);
	elist_p->el_value_p = (byte_t *)parent_p;
	elist_p++;
	INS8(statevalue, RS_newDir);
	INS8(convalue, CG_high);
	}

    elist_p->el_type = DNS_REPLICAS;
    elist_p->el_len = replica_build(replica_p, &ch_p->ch_uid,
				NULL, EXT8(typevalue), &ch_p->ch_name);
    elist_p->el_value_p = (byte_t *)replica_p;
    elist_p++;

    elist_p->el_type = DNS_REPLICASTATE;
    elist_p->el_len = sizeof(statevalue);
    elist_p->el_value_p = statevalue;
    elist_p++;

    elist_p->el_type = DNS_REPLICATYPE;
    elist_p->el_len = sizeof(typevalue);
    elist_p->el_value_p = typevalue;
    elist_p++;

    elist_p->el_type = DNS_REPLICAVERSION;
    elist_p->el_len = sizeof(*version_p);
    elist_p->el_value_p = (byte_t *)version_p;
    elist_p++;

    temp = (entrylist_t *)SKIP_bytes(elist, sizeof(elist));
    dce_assert(cds__svc_handle,
	       (elist_p <= temp));

    stream_p->db_dir.data_p = (data_record_t *)
			dns_malloc_temp(INITIAL_RECORD_SIZE);
    if (!stream_p->db_dir.data_p) 
	{
	status = DNS_NONSRESOURCES;
	goto leave_ta_create_dir_record;
	}

    stream_p->db_dir.data_buf_len = INITIAL_RECORD_SIZE;
    status = create_record(&stream_p->db_dir, elist_p-elist, elist);
    if (status == DNS_SUCCESS)
	{
	/* Add access control to the directory */
	status = create_directory_acs(user_p, stream_p, FALSE);
	if (status == DNS_SUCCESS) 
	    status = db_dir_create(stream_p);
	}

leave_ta_create_dir_record:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug4,
        " <ta_create_dir_record(,%s,,%s,%s,,) return(%s)",
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_Timestamp(actual_ctsBuf,actual_cts_p),
        deb_ascii_ObjUID(actual_idBuf,actual_id_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

/*
 * ta_create_dir
 *   This routine creates a new directory and writes it to the database.
 * It also creates a child directory pointer at the parent.  
 * The parent id is not known until the child directory pointer is
 * created so the directory is unreachable until the remote operation
 * completes and the directory updated.
 * A skulk is started to turn actually turn the directory on.
 *
 * Input:
 *      user_p = address of user descriptor
 *      directory_p = address of directory full name
 *
 * Output:
 *      actual_uid_p = address to return actual uid
 *
 * Side Effects: 
 *   The count of directories in this clearinghouse is updated in the
 * global section.
 *
 * Function value:
 *	DNS_SUCCESS on successful completion
 *	DNS_NONSRESOURCES
 *      other dns status indicating failure 
 *
 */
dns_status_t 
ta_create_dir (user_descriptor_t  *user_p,
               Progress_t         *progress_p,
               Timestamp_t        *actual_cts_p)
{
  FullName_t            *directory_p = (FullName_t *)progress_p->pr_unresolvedName;

    DEB_ASCII_BUF_user_descriptor(userBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    user_descriptor_t	self_user_desc;
    sec_id_foreign_t onbehalfof, *on_p = &onbehalfof;
    boolean32 optimize = FALSE;

#if defined(DCE_SEC)
    sec_login_net_info_t net_info;
    error_status_t sec_st;
#endif /* DCE_SEC */

    ReplicaPointer_u 	replica;		 /* replica data storage  */
    tlog_dsc_t 		tlog;			 /* transaction log update  */
    version_u 		version;		 /* pseudo dir version buf  */
    db_stream_t 	*stream_p;		 /* pointer to record read */
    db_stream_t 	*parent_stream_p = NULL; /* parent stream info  */
    ObjUID_t 		dirid;			 /* id for directory  */
    nsgbl_ch_t 		*ch_p;			 /* clearinghouse information */
    Update_t 		*update_p;		 /* alloc'd update block  */
    byte_t 		*upd_p;			 /* for filling in update_p */
    ParentPointer_t 	*parent_p;		 /* parent pointer data  */
    dns_status_t 	status, 		 /* current & exit status  */
			exists_status;	 	 /* existence of parent stat */
    FullName_t   	parent;			 /* name of parent dir  */
    Timestamp_t  	*ts_p;			 /* update timestamp  */

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " >ta_create_dir(%s,,)",
        deb_ascii_user_descriptor(userBuf,user_p)));

    ZERO_bytes (&self_user_desc, sizeof(user_descriptor_t));

   /* 
    * Since the parentpointer fields are output from clerk calls
    * they must be kept off the stack.  Since the parentpointer is also
    * part of an update structure allocate it all at once.
    * Also allocate space for the RPC_ClassVersion attribute
    * which must be added to directories for sec_acl functions.
    */
    update_p = (Update_t *)dns_malloc_temp(2*sizeof(Update_u)
			- 2*sizeof(MAXATTRIBUTE) + sizeof(ParentPointer_u) +
					   2*sizeof(byte_u));
    if (!update_p) 
	{
	status = DNS_NONSRESOURCES;
	goto leave_ta_create_dir;
	}

    status = db_open_stream(user_p->ud_chuid_p, &stream_p, OP_CreateDirectory);
    if (status != DNS_SUCCESS) 
	goto leave_ta_create_dir;

    /* Generate CTS */
    status = ts_new(actual_cts_p);
    if (status != DNS_SUCCESS) 
	{
	goto close_ta_create_dir;
	}
    (void)id_new(&dirid);
    /* Check pseudo directory for access */
    status = db_pseudo_read(stream_p);
    if (status == DNS_SUCCESS) 
	{
	status = security_check(user_p, stream_p, Create_Dir_check, 
			      ET_clearinghouse, NULL);

	/* Get pseudo directory version number */
	db_dir_get_version(stream_p, (version_t *)version);
	dns_free((char *)stream_p->db_dir.data_p);
        stream_p->db_dir.data_p = NULL;
	}

    if (status == DNS_SUCCESS) 
	{
	upd_p = SKIP_bytes(update_p, update_dns_build(update_p, UD_present, 
		     NullTimestamp, dns_Attribute(DNS_PARENTPOINTER),
		     BL_true, 0, NULL));

	/* Parent pointer is the update value */
        parent_p = (ParentPointer_t *)((AtomicValue_t *)upd_p)->av_value;
	init_pp_timeout(parent_p);

	/* 
	 * We need to resolve any links in the directory name before
	 * checking the length against the clearinghouse name.
	 * The result is put directly into the parent pointer.
	 */
	resolve_dirname(directory_p, (FullName_t *)parent_p->pp_myName);
        INS16(((AtomicValue_t *)upd_p)->av_length, LEN_ParentPointer(parent_p));
	/* Parent ID not known yet, returned from create_child */
	ZERO_ObjUID(parent_p->pp_parentID);


        /* 
         * Directories must be stored in at least one clearinghouse
         * whose name is less then or equal the directory name length
         */
	ch_p = stream_p->db_file_p->file_ch_p;
        if (dnsCountSimple((dns_opq_fname *)&ch_p->ch_name) >
		dnsCountSimple((dns_opq_fname *)parent_p->pp_myName))
            status = DNS_CANTPUTHERE;
	else 
	/* 
	 * Onbehalf of check is done seperately from the create child
	 * call in CDS.  Servers trust each other to have done this
	 * check.  Caller is passed as a parm. for default acl on create
	 * child operations.
	 */
	names_stripright((FullName_t *)parent_p->pp_myName, &parent);

	/*
	 * If the parent directory is in this clearinghouse, don't bother
	 * binding to the parent to test access - we can put our hands
	 * right on the parent's dacl.   Note that we use the get_stream_igore
	 * call here because we are using a nested access to the clearinghouse
	 * and we want to avoid deadlocks with checkpoints firing off.  The
	 * lock of the open_stream above holds the checkpoint file in a
	 * steady state whilst this "sub-stream" does its work.
	 */

	status = db_get_stream_ignore(stream_p->db_file_p,
				 &parent_stream_p,
				 OP_CreateDirectory); 
	if (status != DNS_SUCCESS) 
	    goto close_ta_create_dir;

	COPY_FullName (&parent, parent_stream_p->db_key);
	/* See if we can create a directory here. */
	if ((status = 
		db_dir_read_for_update (parent_stream_p)) == DNS_SUCCESS &&
	    (status = operationallowed(WALKTREE_CREATE_OP,
			db_dir_get_replica_type(parent_stream_p->db_dir_hash_p),
			ET_directory)) == DNS_SUCCESS) 
	    {
#if !defined(DCE_SEC)
	    (void)db_dir_release (parent_stream_p);
	    /* fall through acl check */
	    }
# else
	    nsgbl_ns_t *const ns_p = user_p->ud_ns_p;

	    /* Optimize the access tests */
	    /* Construct a user_p structure containing our (server) PAC */
          DCE_SVC_DEBUG((
              cds__svc_handle,
              cds_svc_server,
              svc_c_debug3,
              " >ta_create_dir(%s,,) Optimize access test",
              deb_ascii_user_descriptor(userBuf,user_p)));
	    optimize = TRUE;
	    sec_login_inquire_net_info (ns_p->ns_login_context,
				       &net_info,
				       &sec_st);
	    /*
	     * Ignore 'context not certified' error 
	     */
	    if ((sec_st != error_status_ok) && 
		 (sec_st != sec_login_s_not_certified)) 
	    {
		DCE_SVC_DEBUG((
                   cds__svc_handle,
                   cds_svc_server,
                   svc_c_debug3,
                   "ta_create_dir: sec_login_inquire_net_info returned error status: %d", 
		   sec_st));

		status = sec_st;
		goto close_ta_create_dir;
	    } 
	    COPY_bytes (&net_info.pac, &self_user_desc.ud_pac, sizeof(sec_id_pac_t));
	    self_user_desc.ud_is_pac = 1;
	    self_user_desc.ud_cred.data = NULL; /* force PAC-style check */
	    /* Test my access first (read and insert),
	       using my pre-v1.1 PAC, on_behalf() style */
	    status = security_dacl(&self_user_desc, 
				   &parent_stream_p->db_dir, 
				   sec_acl_perm_insert | sec_acl_perm_read,
				   ALLPerms);
	    if (status == DNS_SUCCESS) 
		{
		/* Test user's access (insert) */
		status = security_dacl(user_p, 
				     &parent_stream_p->db_dir, 
				     sec_acl_perm_insert,
				     ALLPerms);
		}
	    } /* End ACL check */
	  else 
	    {
	    /* Test access the long, hard way */
	    status = dacl_test_access(user_p, &parent, 
				      sec_acl_perm_insert,
				      sec_acl_perm_insert | sec_acl_perm_read);
	    }
#endif	/* DCE_SEC */
	if (status == DNS_SUCCESS) 
	    {
	    Set_t *const rpset_p = (Set_t *)replica;
	    MemberValue_t *member_p;
	    /* 
	     * Build the key and create the directory.
	     * The case of the directory name is kept in the parent pointer.
	     */
	    INS8(rpset_p->st_flag, ST_present);
	    INS16(rpset_p->st_offsetLength, sizeof(word_u));
            member_p = (MemberValue_t *)DATA_Set(rpset_p);
	    INS16(OFFSET_Set(rpset_p, 0), DIFF_bytes(member_p, rpset_p));
	    INS16(member_p->mv_length,
		replica_build((ReplicaPointer_t *)member_p->mv_value,
		&ch_p->ch_uid, NULL, RT_master, &ch_p->ch_name));
	    /* 
	     * The replica set of the directory is also the replica set
	     * used as input to the create_child call.  Pass in the
	     * buffer used for both.
	     */
	    status = ta_create_dir_record(user_p, stream_p,
			(version_t *)version, actual_cts_p, &dirid,
			(ReplicaPointer_t *)member_p->mv_value,
			(ParentPointer_t *)((AtomicValue_t *)upd_p)->av_value);
	    if (status == DNS_SUCCESS)
		{
	        /* Compete building the replica set for over the wire */
	        INS8(member_p->mv_flag, MV_present);
	        status = ts_new((Timestamp_t *)member_p->mv_timestamp);
		INS8(member_p->mv_valuetype, VT_ReplicaPointer);
	        INS16(rpset_p->st_valueLength, LEN_MemberValue(member_p));
		}
            if (status == DNS_SUCCESS) 
		{
#if defined(DCE_SEC)
	        if (user_p->ud_is_pac) 
		    {
		    onbehalfof.id = user_p->ud_pac.principal;
		    onbehalfof.realm = user_p->ud_pac.realm;
		    } 
		else
		    on_p = (sec_id_foreign_t *)NULL;
#else /* !DCE_SEC */

		    on_p = (sec_id_foreign_t *)NULL;

#endif /* !DCE_SEC */
		if (optimize) 
		    {
		    /* short circuit the clerk create child call */
		    Progress_t progress;

		    ZERO_bytes(&progress, sizeof(progress));
		    COPY_FullName(parent_p->pp_myName, 
				  progress.pr_unresolvedName);
		    COPY_FullName(NullFullName, 
				  SKIP_FullName(progress.pr_unresolvedName)); 
		    
		    COPY_FullName ((FullName_t *)parent_p->pp_myName,
                                   parent_stream_p->db_key);
		    exists_status = check_exists (parent_stream_p, 
						WALKTREE_LOOKUP_OP);
		    if (exists_status == DNS_SUCCESS)
			exists_status = DNS_ENTRYEXISTS;
		    else if (exists_status == DNS_UNKNOWNENTRY)
			exists_status = DNS_SUCCESS;
		    parent_stream_p->db_function = OP_CreateChildEntry;
		    status = create_child (exists_status, 
					   parent_stream_p,
					   &self_user_desc,
					   &progress,
					   on_p,
					   &dirid,
					   rpset_p,
					   (ObjUID_t *)parent_p->pp_parentID,
                                           from_ta_create_dir);
		    }
		else
		    {
		    status = clerk_create_child((FullName_t *)parent_p->pp_myName,
						(byte_t *)on_p, &dirid, rpset_p, 
						(ObjUID_t *)parent_p->pp_parentID);
		    }

	        /* Check status from create child */
	        if (status != DNS_SUCCESS) 
	            db_dir_delete(stream_p);
		else
		    {
		   /* 
  		    * Output from create child was the parent ID so the parent 
 	  	    * pointer is complete and can be inserted 
		    */
		    ts_p = (Timestamp_t *)update_p->ud_timestamp;
		    (void)ts_new(ts_p);

		    (void)set_insert(&stream_p->db_dir, 
			 update_p, dns_Attribute(DNS_PARENTPOINTER), TRUE);
		    tlog.buf_p = (byte_t *)update_p;
		    tlog.len = LEN_Update(update_p);
		    /* Build update to add classversion for RPC */
		    tlog.cnt = 2;
		    upd_p = SKIP_bytes(update_p, tlog.len);
		    tlog.len += update_build((Update_t *)upd_p,
				UD_present, NULL,
				AT_set, (AttributeName_t *)RPC_ClassVersion, 
				(unsigned int *)NULL, BL_true,
				sizeof(RPCversion), VT_byte, RPCversion);
		    (void)set_insert(&stream_p->db_dir, (Update_t *)upd_p,
				(attribute_descriptor_t *)NULL, TRUE);
		    ts_p = (Timestamp_t *)((Update_t *)upd_p)->ud_timestamp;
		   /* 
		    * At this point the directory is commited, so
		    * any further problems will be fixed up later.
  		    * User sees success
		    */

		    /* Maximize UTS and LASTUPDATE */
		    (void)set_timestamp_set(&stream_p->db_dir, ts_p,
				dns_Attribute(DNS_UTS), NULL, 0);
		    db_dir_set_lastupdate(stream_p->db_dir_hash_p, ts_p); 
		    
		    (void)db_dir_update(stream_p, &tlog);

		    /* Get parent acs and convergence */
		    stream_p->db_function = OP_ModifyAttribute;
                    (void)db_dir_release(parent_stream_p);
		    (void)skulk_sched(stream_p);
		    } /* End create child success */
		} /* End create directory success */
	    } /*End security check successful */
        } /* End pseudo directory read successful */

    /* Fix up progress record */
    if (status == DNS_SUCCESS) 
	{
	  FullName_t *full_p;
	  Set_t *set_p;

	SET_FLAG(progress_p->pr_flags, PR_done);

	if (NE_FullName(directory_p, parent_p->pp_myName))
	    SET_FLAG(progress_p->pr_flags, PR_linked);
      
	/* Build unresolved name */
	names_new(&dirid, NULL, 
		(FullName_t *)progress_p->pr_unresolvedName);

	full_p = (FullName_t *)SKIP_FullName(progress_p->pr_unresolvedName);
      
	/* Build resolved name */
	COPY_FullName(parent_p->pp_myName, full_p);

        set_p = (Set_t *)SKIP_FullName(full_p);

        COPY_Set(NullSet, set_p);
	}

    dns_free((char *)update_p);

    /*
     * Close up nested stream (if we opened one), then the primary one.
     */


close_ta_create_dir:

#if defined(DCE_SEC)
    if (self_user_desc.ud_is_pac)
	sec_login_free_net_info (&net_info);/* cleanup */
#endif

    if (parent_stream_p)
        (void)db_close_stream(parent_stream_p);

    (void)db_close_stream(stream_p);

leave_ta_create_dir:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " <ta_create_dir(%s,,) return(%s)",
        deb_ascii_user_descriptor(userBuf,user_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

/*
 * ta_create_child_check
 *	This routine makes sure the create_child call completed during
 *	a create_dir operation.  We would be in this state if the system
 *	crashed before the call completed. If not, we will attempt to 
 *	create it.  The directory is read and locked on input.
 *
 * Input:
 *	stream_p = pointer to stream descriptor, hash entry valid
 *
 * Returns:
 *	DNS_SUCCESS on successful completion
 *	DNS_NAMESERVERBUG
 *	DNS_UNKNOWNENTRY
 *	other dns error or informational codes
 */
dns_status_t 
ta_create_child_check (db_stream_t *stream_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    sec_id_foreign_t     onbehalfof, *on_p = &onbehalfof;
    Update_u		update;			 /* update block  */
    ReplicaPointer_u 	replica;		 /* replica data buffer  */
    int 		idx;			 /* used for find_attribute  */
    DBSet_t 		*set_p;			 /* current set  */
    ParentPointer_t 	*pp_p;			 /* parent pointer data  */
    Set_t 		*rpset_p;		 /* pointer to replica data  */
    MemberValue_t 	*member_p;		 /* current member value  */
    dns_status_t 	status;			 /* current status  */
    tlog_dsc_t		tlog;			 /* transaction log update  */

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug4,
        " >ta_create_child_check(%s)",
        deb_ascii_db_stream(streamBuf,stream_p)));

    /* 
     * Check parentpointer first.  If parent ID is not null, completed
     * create child call 
     */
    if (find_attribute(stream_p->db_dir.data_p,
		dns_AttributeName(DNS_PARENTPOINTER),
		&set_p, &idx) == DNS_SUCCESS)
	{
	DBSetMember_t *const value_p = find_last_value(set_p);

	if (value_p)
	    {
	    pp_p = (ParentPointer_t *)value_p->dm_data;
	    if (!ObjUID_is_nil(pp_p->pp_parentID))
		{
		status = DNS_SUCCESS;
		goto leave_ta_create_child_check;
		}
	    }
	else
	    {
	    status = DNS_NAMESERVERBUG;  /* ERROR */
	    goto leave_ta_create_child_check;
	    }
	}
    else
	{
	status = DNS_SUCCESS;  /* Must be root */
	goto leave_ta_create_child_check;
	}
    on_p = (sec_id_foreign_t *)NULL;
    /* Set up replica set */
    rpset_p = (Set_t *)replica;
    INS8(rpset_p->st_flag, ST_present);
    INS16(rpset_p->st_offsetLength, sizeof(word_u));
    member_p = (MemberValue_t *)DATA_Set(rpset_p);
    INS16(OFFSET_Set(rpset_p, 0), DIFF_bytes(member_p, rpset_p));
    INS8(member_p->mv_flag, MV_present);
    (void)ts_new((Timestamp_t *)member_p->mv_timestamp);
    
    if (find_attribute(stream_p->db_dir.data_p,
	    dns_AttributeName(DNS_REPLICAS), &set_p, &idx) != DNS_SUCCESS)
	{
	status = DNS_UNKNOWNENTRY;
	/* Something is very wrong here, get rid of directory */
	tlog.cnt = 1;
	tlog.buf_p = update;
	(void)db_dir_set_state(stream_p, RS_dyingDir,
				  (Update_t *)tlog.buf_p, &tlog.len);
	(void)db_dir_update(stream_p, &tlog);
	goto leave_ta_create_child_check;
	}
    else
	{
      /* Get first and only replica */
	DBSetMember_t *const value_p = (DBSetMember_t *)DATA_DBSet(set_p);

	INS8(member_p->mv_valuetype, EXT8(value_p->dm_valuetype));

	INS16(member_p->mv_length, LEN_DBSetMemberData(value_p));
	COPY_bytes(value_p->dm_data, member_p->mv_value,
	    EXT16(member_p->mv_length));
	INS16(rpset_p->st_valueLength, LEN_SetMember(member_p));
	}

    status = clerk_create_child((FullName_t *)pp_p->pp_myName,
		(byte_t *)on_p, (ObjUID_t *)stream_p->db_dir.data_uid,
		rpset_p, (ObjUID_t *)pp_p->pp_parentID);
    if ((status == DNS_ENTRYEXISTS) || (status == DNS_UNKNOWNENTRY)
	|| (status == DNS_ACCESSDENIED)) 
	{
	status = DNS_UNKNOWNENTRY;
	/* Cannot complete the operation */
	db_dir_delete(stream_p);
	}

leave_ta_create_child_check:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug4,
        " <ta_create_child_check(%s) return(%s)",
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

/*
 * ta_create_child
 *    This routine is called by skulk to create the child directory for
 * a new directory.
 *
 * Input:
 *     user_p = address of skulk user descriptor
 *     progress_p = address of progress record
 *     onbehalf_p = address of client 
 *     uid_p = address of uid of corresponding directory
 *     rp_p = replica set
 *
 * Output:
 *     parentuid_p = parent directory uid
 *
 * Value: 
 *     dns status as returned from db_open_stream or create_child
 *
 * Side Effects:
 *     progress record updated
 */
dns_status_t 
ta_create_child (user_descriptor_t  *user_p,
                 Progress_t         *progress_p,
                 sec_id_foreign_t   *onbehalf_p,
                 ObjUID_t           *uid_p,
                 Set_t              *rp_p,
                 ObjUID_t           *parentuid_p)
{
    DEB_ASCII_BUF_user_descriptor(userBuf)
    DEB_ASCII_BUF_ObjUID(uidBuf)
    DEB_ASCII_BUF_ObjUID(parentuidBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    db_stream_t  *stream_p;			 /* stream information ptr  */
    dns_status_t status, 			 /* current status  */
		 exists_status;			 /* stat from create_child fcn*/

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " >ta_create_child(%s,,,%s,,%s)",
        deb_ascii_user_descriptor(userBuf,user_p),
        deb_ascii_ObjUID(uidBuf,uid_p),
        deb_ascii_ObjUID(parentuidBuf,parentuid_p)));
  
    status = db_open_stream(user_p->ud_chuid_p, &stream_p, OP_CreateChildEntry);
    if (status == DNS_SUCCESS) 
	{
	exists_status = walktree(user_p, ET_childPointer, WALKTREE_CREATE_OP, 
		    progress_p, stream_p);
	status = create_child (exists_status, 
			   stream_p,
			   user_p, 
			   progress_p, 
			   onbehalf_p, 
			   uid_p, 
			   rp_p, 
			   parentuid_p,
                           from_ta_create_child);
	}

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " <ta_create_child(%s,,,%s,,%s) return(%s)",
        deb_ascii_user_descriptor(userBuf,user_p),
        deb_ascii_ObjUID(uidBuf,uid_p),
        deb_ascii_ObjUID(parentuidBuf,parentuid_p),
        deb_ascii_dns_status(statusBuf,status)));
    return (status);
}

/*
 * create_child
 *    This routine creates the child directory for a new directory.
 *
 * Input:
 *     exists_status = return value from 'walktree' on the child pointer
 *     stream_p = stream to clearinghouse
 *     user_p = address of skulk user descriptor
 *     progress_p = address of progress record
 *     onbehalf_p = address of client 
 *     uid_p = address of uid of corresponding directory
 *     rp_p = replica set
 *     parentuid_p = parent directory uid
 *
 * Output:
 *     parentuid_p = parent directory uid
 *
 * Value: 
 *	DNS_SUCCESS on successful completion
 *     	other dns status otherwise
 *
 * Side Effects:
 *     progress record updated
 */
static dns_status_t 
create_child (dns_status_t        exists_status,
              db_stream_t         *stream_p,
              user_descriptor_t   *user_p,
              Progress_t          *progress_p,
              sec_id_foreign_t    *onbehalf_p,
              ObjUID_t            *uid_p,
              Set_t               *rp_p,
              ObjUID_t            *parentuid_p,
              cre_child_caller_t  caller)
{
    DEB_ASCII_BUF_dns_status(exists_statusBuf)
    DEB_ASCII_BUF_db_stream(streamBuf)
    DEB_ASCII_BUF_user_descriptor(userBuf)
    DEB_ASCII_BUF_ObjUID(uidBuf)
    DEB_ASCII_BUF_ObjUID(parentuidBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    entrylist_t elist[3];			 /* entry attr list  */
    Timestamp_u child_cts;			 /* timestamp of child  */
    MemberValue_t *sm_p;			 /* replicas member value  */
    dns_status_t status;			 /* current status  */
  
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug4,
        " >create_child(%s ,%s,%s,,%s,,%s)",
        deb_ascii_dns_status(exists_statusBuf,exists_status),
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_user_descriptor(userBuf,user_p),
        deb_ascii_ObjUID(uidBuf,uid_p),
        deb_ascii_ObjUID(parentuidBuf,parentuid_p)));

    if ((status = exists_status) == DNS_ENTRYEXISTS)
	{
	if (stream_p->db_entry.data_type == ET_childPointer)
	    {

	    if (set_dnstest(stream_p->db_entry.data_p, sizeof(*uid_p), 
			      (byte_t *)uid_p, dns_Attribute(DNS_OBJECTUID)))
		status = DNS_SUCCESS;

	    if (status == DNS_SUCCESS)
		COPY_ObjUID(stream_p->db_dir.data_uid, parentuid_p);
	    }
	goto close_create_child;
	}	
    if (status != DNS_SUCCESS)
	goto leave_create_child;
  
    COPY_ObjUID(stream_p->db_dir.data_uid, parentuid_p);
  
    /*
     * Server making the request must have access 
     */
    if (security_check(user_p, stream_p, Create_Entry_check, 
		     ET_directory, NULL) != DNS_SUCCESS) 
	{
        status = DNS_UNTRUSTEDCH;
        security_log(user_p,
		     (FullName_t *)SKIP_FullName(progress_p->pr_unresolvedName),
		     stream_p->db_file_p->file_ch_p);
	}


    if (status == DNS_SUCCESS) 
	{

	/* 
	 * We trust that the calling server has already checked the user
	 * access by calling sec_acl_test_access_on_behalf
	 */
	elist[0].el_type = DNS_OBJECTUID;
	elist[0].el_value_p = (unsigned char *)uid_p;
	elist[0].el_len = sizeof(*uid_p);


	status = ts_new((Timestamp_t *)child_cts);
	elist[1].el_type = DNS_CTS;
	elist[1].el_len = sizeof(child_cts);
	elist[1].el_value_p = child_cts;
    
	sm_p = (MemberValue_t *)DATA_Set(rp_p);
	elist[2].el_type = DNS_REPLICAS;
	elist[2].el_value_p = sm_p->mv_value;
	elist[2].el_len = EXT16(sm_p->mv_length);
    
	if (status == DNS_SUCCESS) 
	    {
	    stream_p->db_entry.data_p = 
	      (data_record_t *)dns_malloc_temp(INITIAL_RECORD_SIZE);
	    if (!stream_p->db_entry.data_p)
		status = DNS_NONSRESOURCES;
	    else
		{
		stream_p->db_entry.data_buf_len = INITIAL_RECORD_SIZE;
		status = create_record(&stream_p->db_entry, 3, elist);

#if defined(DCE_SEC)
		if (status == DNS_SUCCESS) 
		    /* Add access control */
		    status = create_child_acs(user_p, onbehalf_p,
					      stream_p->db_dir_hash_p,
					      &stream_p->db_entry,
					      &stream_p->db_dir, 
					      stream_p->db_file_p->file_ch_p);
#endif /* DCE_SEC */

		}
	    if (status == DNS_SUCCESS) 
		{
		SimpleName_t *const name_p = names_pointlast(
				(FullName_t *)progress_p->pr_unresolvedName);
		/* Do the create */
		status = db_entry_create(stream_p, name_p, ET_childPointer);
		if (status == DNS_SUCCESS) 
		    {
		    DBSet_t *set_p;
		    int idx;

		    /* Update last update */
		    (void)names_swap((FullName_t *)progress_p->pr_unresolvedName);
		    if (find_attribute(stream_p->db_entry.data_p, 
					dns_AttributeName(DNS_UTS),
					&set_p, &idx) == DNS_SUCCESS)
			db_dir_set_lastupdate(stream_p->db_dir_hash_p, 
						(Timestamp_t *)set_p->ds_ts);
		    if (!low_convergence(stream_p->db_dir.data_p))
			(void)back_propagate_sched(stream_p,
				    ET_childPointer, (Timestamp_t *)child_cts);
		    } /* End create entry success */
		} /* End got memory for record */
	    } /* End created a timestamp */
	} /* End access okay */

close_create_child:
    if (caller == from_ta_create_child)
        {
        db_close_stream(stream_p);
        }

leave_create_child:
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug4,
      " <create_child(%s,%s,%s,,%s,,%s) return(%s)",
      deb_ascii_dns_status(exists_statusBuf,exists_status),
      deb_ascii_db_stream(streamBuf,stream_p),
      deb_ascii_user_descriptor(userBuf,user_p),
      deb_ascii_ObjUID(uidBuf,uid_p),
      deb_ascii_ObjUID(parentuidBuf,parentuid_p),
      deb_ascii_dns_status(statusBuf,status)));
  return(status);
}
