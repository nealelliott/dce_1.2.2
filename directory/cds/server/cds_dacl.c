/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cds_dacl.c,v $
 * Revision 1.1.18.1  1996/10/03  14:35:48  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/03  13:19:38  arvind]
 *
 * Revision 1.1.14.2  1996/02/18  19:35:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:16:01  marty]
 * 
 * Revision 1.1.14.1  1995/12/08  15:29:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:38  root]
 * 
 * Revision 1.1.12.14  1994/09/15  19:36:52  mccann
 * 	change size to length OT12211
 * 	[1994/09/15  19:35:56  mccann]
 * 
 * Revision 1.1.12.13  1994/09/08  19:41:16  jd
 * 	Replaced the strdup with a malloc(sizeof) and an strcpy.
 * 	[1994/09/08  17:56:23  jd]
 * 
 * Revision 1.1.12.12  1994/09/07  20:02:13  mccann
 * 	fix version handling OT:12004
 * 	[1994/09/07  18:41:59  mccann]
 * 
 * Revision 1.1.12.11  1994/09/06  17:35:37  proulx
 * 	fix timestamps
 * 	[1994/09/06  17:30:15  proulx]
 * 
 * 	fix status check in rdacl_get_access
 * 
 * Revision 1.1.12.10  1994/08/29  21:17:48  zee
 * 	Fixes for OT 11934.
 * 	[1994/08/29  21:17:20  zee]
 * 
 * Revision 1.1.12.9  1994/08/26  21:55:28  zee
 * 	- Add check for DirectoryVersion >= 4.0 in rdacl_get_manager_types.
 * 	- Add sec_acl_type parameter to check_manager.
 * 	- In check_manager, allow two manager types if default object.
 * 	- In rdacl_lookup, replace the manager type if DirectoryVersion >= 4.0.
 * 	- In check_acl, change directory version requirement for delegation to
 * 	  be 4.0.
 * 	- Added routine update_dir_defobjacl_mgr_type.
 * 	[1994/08/26  21:48:59  zee]
 * 
 * Revision 1.1.12.6  1994/07/25  15:14:54  proulx
 * 	Use common ACL manager for subcomponent mgmt ACLs
 * 	[1994/07/25  13:57:04  proulx]
 * 
 * Revision 1.1.12.5  1994/06/23  18:30:50  mccann
 * 	sams cleanup drop1
 * 	[1994/06/23  18:13:41  mccann]
 * 
 * Revision 1.1.12.4  1994/06/09  18:42:58  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:15:17  devsrc]
 * 
 * Revision 1.1.12.3  1994/04/29  15:56:05  peckham
 * 	Use dcecdsmac.h macros for serviceability.
 * 	Use dcelocal_path in file names.
 * 	[1994/04/29  14:37:05  peckham]
 * 
 * Revision 1.1.12.2  1994/03/12  22:10:20  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:39:13  peckham]
 * 
 * Revision 1.1.12.1  1994/01/28  23:09:09  burati
 * 	Delegation/EPAC changes - use rdaclifv0 until new one integrated (dlg_bl1)
 * 	[1994/01/18  21:27:17  burati]
 * 
 * Revision 1.1.10.1  1993/10/08  20:07:42  pwang
 * 	Checked the acl data size in rdacl_replace() aganst
 * 	dns_attlist[index].max to prevent Update_u from
 * 	overflowed [OT#7851]
 * 	[1993/10/08  20:07:01  pwang]
 * 
 * Revision 1.1.5.8  1993/01/27  21:32:50  keegan
 * 	Backed out last 2 bug fixes relating to the permissions that are
 * 	displayed when editing the initial object ACL on a directory.
 * 	These fixes will cause interoperability problems between 1.0.2
 * 	and 1.0.1.
 * 	[1993/01/27  21:31:18  keegan]
 * 
 * Revision 1.1.7.2  1993/01/27  14:53:58  keegan
 * 	Fix 'dacl_lookup'. Check the ACL type and choose the appropriate
 * 	acl manager uuid to pass to 'sec_acl_bind'.
 * 
 * Revision 1.1.5.6  1992/12/30  20:24:08  keegan
 * 	Provide the 'catalog' manager instead of the 'directory' manager when
 * 	the initial object ACL is being accessed. Otherwise, the 'i' and 'a'
 * 	permissions are allowed when they shouldn't.
 * 	[1992/12/30  20:13:15  keegan]
 * 
 * Revision 1.1.5.5  1992/12/30  14:19:52  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:19:56  zeliff]
 * 
 * Revision 1.1.5.4  1992/12/21  18:59:32  mfox
 * 	Set timeout on rpc_ns_mgmt_set*() call to zero to bypass the
 * 	cds clerk cache when testing acls.
 * 	[1992/12/21  18:58:19  mfox]
 * 
 * Revision 1.1.5.3  1992/10/31  03:08:31  keegan
 * 	Use proper constant - 'sec_acl_printstring_len' or
 * 	'sec_acl_printstring_help_len' when copying strings to client's buffers
 * 	in 'rdacl_get_printstrings' rpc.
 * 	[1992/10/30  21:32:16  keegan]
 * 
 * Revision 1.1.5.2  1992/10/30  14:36:10  keegan
 * 	Fixed 'rdacl_get_referral'. Remove call to 'check_manager' (it fails
 * 	in this context and is only necessary at the master)
 * 	[1992/10/29  21:40:26  keegan]
 * 
 * Revision 1.1.2.6  1992/06/26  06:44:15  jim
 * 	 Added include of msgblk.h for AIX 3.2.  It is not being included for some
 * 	 reason.
 * 	[1992/06/23  15:19:03  jim]
 * 
 * Revision 1.1.2.5  1992/06/16  15:10:10  keegan
 * 	Fix OT 4173: remove pre-RPC6 hack
 * 	[1992/06/11  15:24:13  keegan]
 * 
 * Revision 1.1.2.4  1992/05/08  15:50:16  burati
 * 	Add rdacl_get_mgr_type_semantics support.
 * 	[1992/05/07  18:07:19  burati]
 * 
 * Revision 1.1.2.3  1992/04/29  18:08:36  keegan
 * 	Change the return status on 'rdacl_replace' from 'sec_acl_invalid_permission'
 * 	to 'sec_acl_not_authorized', if user trying to change ACL doesn't have
 * 	owner permission.
 * 	[1992/04/29  14:31:23  keegan]
 * 
 * Revision 1.1.2.2  1992/03/22  22:28:17  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:34:25  weisman]
 * 
 * Revision 1.1  1992/01/19  15:26:14  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* Module: cds_dacl.c
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
 * ACL's for namespace entries:
 *
 * The rdaclif is registered once for each clearinghouse at a server.
 * The acl editor then does an rpc_import on the CDS directory names.
 * The CDS server intercepts this request and returns the UUID of the
 * directory and a tower set to build a fully bound
 * handle to the directory.  This enables the acl_editor to contact the
 * server directory without using rpcd.  It also means that
 * we register with clearinghouse uuids, then expect directory uuids
 * in the handle.  This design was choosen so we didn't have
 * to register hundreds of directories with rpcd.  The RPC group
 * assures me this will work.
 * It also means when a server goes down, cached handles
 * become stale because we are using dynamic endpoints.
 * All clearinghouses are registered so we can return referalls to other
 * clearinghouses by contacting that rpcd that it is registed with.
 *
 * ACL's for nameserver management:
 *
 * The rdaclif is registered once for each CDS server.  The objectuuid is
 * fixed.
 *
 *
 * MODIFICATIONS:
 *  	Set CANCEL_OFF for these RPC server threads so that
 *	RPC can't pull the rug from under us. This is done in
 *	the beginning and end of each rdacl entry point.
 *
 * Gen 38, 24-Nov-1991, Alan Peckham
 *	Fix DEBUG displays. Lots of other mischief.
 * Gen 41, 12-dec-1991, Peter Keegan
 *      Change backstop admin group name to "subsys/dce/cds-admin"
 * Gen 43, 30-Dec-1991, Alan Peckham
 *	Fix DEB_ASCII_*() macros for -UDEBUG
 * Gen 44, 2-Jan-1992, P. Keegan
 *      Make security resources in cds.conf file optional.
 * Gen 53, 18-Jun-1992, Alan Peckham
 *	Merge Revision 1.1.2.4  1992/05/08  15:50:16  burati
 *		Add rdacl_get_mgr_type_semantics support.
 *
 *	Run through c89 -check, gcc -W, and lint.
 *	  Don't use index - lint complaints about conflict with index().
 *	Fix get_user_pac() status assignment bug in conditional
 *	in rdacl_get_access().
 * Gen 54, 24-Jun-1992, Alan Peckham
 *	The user_descriptor_t's on the stack need to be zeroed.
 * 
 * Gen 59, 30-Dec-1992, P. Keegan
 *      Wrong acl manager is used when accessing the initial container
 *      ACL. Also, fixed 'rdacl_get_referral'. Remove call to 
 *      'check_manager' (it fails in this context and is only necessary
 *      at the master)
 *
 * Gen 64, 4-may-1993, P. Keegan
 * For SIA, add 'root' principal to server management ACL 
 */

#if defined(DCE_SEC)

#include <server.h>
#include <unistd.h>
#include <back.h>
#include <dbdef.h>
#include <dns_cfg.h>
#include <dns_strings.h>
#include <dpeacl.h>
#include <find.h>
#include <names.h>
#include <security.h>
#include <sets.h>
#include <tlog.h>
#include <tower.h>

#include <dce/aclif.h>
#include <dce/dceacl.h>
#include <dce/rdaclifv0.h>
#include <dce/rdaclif.h>
#include <dce/binding.h>
#include <dce/daclif.h>
#include <dce/dce_cf.h>
#include <dce/pgo.h>
#include <dce/rgybase.h>
#include <dce/uuid.h>
#include <dce/sec_acl_encode.h>

extern struct nsgbl_ns dns_nameserver;
extern uuid_t server_mgmt_dacl_uuid;

boolean server_acl_ready;


/*
 * Handle to backing storage for this ACL 
 */
static dce_db_handle_t acl_db ;

/*
 * ACL print string and help string id table
 */
#define PERM_STRING_CNT 3
static sec_acl_printstring_t server_printstrings[PERM_STRING_CNT+1];

static long server_mgmt_help_ids[] = {
  /* general ACL help */
  EVT_DACLM_SERVER_MGMT, EVT_DACLMH_SERVER_MGMT, (sec_acl_permset_t)0,
  /* permission helps */
  EVT_DACLPREAD,         EVT_DACLH_R_SERVER, sec_acl_perm_read , 
  EVT_DACLPWRITE,        EVT_DACLH_W_SERVER, sec_acl_perm_write,
  EVT_DACLPCONTROL,      EVT_DACLHADMIN,     sec_acl_perm_control 
};

/*
 * Each entry in the following tables is 3 fields 
 */
#define EVT_ENTRY_SIZE (3*sizeof(long))

/* Array of print/help strings */
static long cat_evt[] = {
    EVT_DACLPREAD,    EVT_DACLHREAD,    sec_acl_perm_read,
    EVT_DACLPWRITE,   EVT_DACLHWRITE,   sec_acl_perm_write,
    EVT_DACLPDELETE,  EVT_DACLHDELETE,  sec_acl_perm_delete,
    EVT_DACLPTEST,    EVT_DACLHTEST,    sec_acl_perm_test,
    EVT_DACLPCONTROL, EVT_DACLHCONTROL, sec_acl_perm_owner};

/* Array of print/help strings */
static long dir_evt[] =   {
    EVT_DACLPREAD,       EVT_DACLHREAD,       sec_acl_perm_read,
    EVT_DACLPWRITE,      EVT_DACLHWRITE,      sec_acl_perm_write,
    EVT_DACLPDELETE,     EVT_DACLHDELETE,     sec_acl_perm_delete,
    EVT_DACLPTEST,       EVT_DACLHTEST,       sec_acl_perm_test,
    EVT_DACLPCONTROL,    EVT_DACLHCONTROL,    sec_acl_perm_owner,
    EVT_DACLPINSERT,     EVT_DACLHINSERTDIR,  sec_acl_perm_insert,
    EVT_DACLPADMIN,      EVT_DACLHADMIN,      sec_acl_perm_admin};

/* Array of print/help strings */
static long cle_evt[] =     {
    EVT_DACLPREAD,    EVT_DACLHREAD,    sec_acl_perm_read,
    EVT_DACLPWRITE,   EVT_DACLHWRITE,   sec_acl_perm_write,
    EVT_DACLPDELETE,  EVT_DACLHDELETE,  sec_acl_perm_delete,
    EVT_DACLPTEST,    EVT_DACLHTEST,    sec_acl_perm_test,
    EVT_DACLPCONTROL, EVT_DACLHCONTROL, sec_acl_perm_owner};

/* Used by wire<->flag routines */

typedef struct {
    const char *const mgr;
    const sec_acl_permset_t perms;
    const int evt_help;
    const int evt_print;
    const long *const evt;
    const int perm_cnt;
    uuid_t uuid;
} mgr_type_t;

/* thread flag for initalizing uuid_t below */
static pthread_once_t type_once = pthread_once_init;

/* DACL Manager types */
static mgr_type_t
  cat = {"c2e7e53c-4455-11ca-99bd-08002b1c8f1f",
	 sec_acl_perm_read | sec_acl_perm_write | sec_acl_perm_delete
	  | sec_acl_perm_test | sec_acl_perm_owner,
	 EVT_DACLMHCATALOG, EVT_DACLMCATALOG,
	 cat_evt, sizeof(cat_evt)/EVT_ENTRY_SIZE},
  dir = {"d1a74194-4455-11ca-b064-08002b1c8f1f",
	 sec_acl_perm_read | sec_acl_perm_write | sec_acl_perm_delete
	  | sec_acl_perm_test | sec_acl_perm_owner | sec_acl_perm_insert
	  | sec_acl_perm_admin,
	 EVT_DACLMHDIR, EVT_DACLMDIR,
	 dir_evt, sizeof(dir_evt)/EVT_ENTRY_SIZE},
  cle = {"d645d095-4455-11ca-b028-08002b1c8f1f",
	 sec_acl_perm_read | sec_acl_perm_write | sec_acl_perm_delete
	  | sec_acl_perm_test | sec_acl_perm_owner,
	 EVT_DACLMHCLE, EVT_DACLMCLE,
	 cle_evt, sizeof(cle_evt)/EVT_ENTRY_SIZE};


/*
 * We will create/use the DACL for the cds-server, and allow the common
 * ACL manager for DCE 1.1 the privilege of managing it.
 *
 * Here we get/store the necessary info for creating the default DACL.
 */
char Server_Realm[100];     /* Our Cell       */
uuid_t Server_Realm_uuid;   /* Our CEll UUID */
char HostPrincipal[100];   /* host (machine) principal name */
uuid_t HostPrincipal_uuid; /* host (machine) principal uuid */
char AdminGroup[100];      /* cds-admin group name          */
uuid_t AdminGroup_uuid;    /* cds-admin group uuid          */


/* Local prototypes */
static error_status_t
check_manager (
    uuid_t         *,
    int             ,
    sec_acl_type_t  );

static error_status_t
check_acl (
    int            ,
    sec_acl_list_t *,
    int            *,
    db_stream_t       *);

static unsigned32
dacl_resolve_ep (
    DBSet_t             *,
    sec_acl_tower_set_t *);

static void
type_init (void);

static db_stream_t *
ch_from_handle (
    handle_t   );

static unsigned32
get_entry  (
    handle_t                 ,
    sec_acl_component_name_t ,
    db_stream_t              **,
    db_data_t                **,
    int                      );

idl_void_p_t
client_malloc (
    idl_size_t );

void
client_free (
    idl_void_p_t );

static void
build_printstrings (
    void);

static void 
server_acl_resolve (handle_t, sec_acl_component_name_t, sec_acl_type_t,
		    uuid_t *, boolean32, void *, uuid_t *, error_status_t *);

static int 
  create_def_server_mgmt_acl (sec_acl_t *);

static void
  get_cell_and_host_info (error_status_t *st_p);

#define DEB_ASCII_LEN_HANDLE 12
#define DEB_ASCII_BUF_handle(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_HANDLE)
#define LOG_ASCII_BUF_handle(nam) \
	LOG_ASCII_BUF(nam,DEB_ASCII_LEN_HANDLE)

#define DEB_ASCII_LEN_SEC_ACL_PERMSET 20
#define DEB_ASCII_BUF_sec_acl_permset(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_SEC_ACL_PERMSET)
#define LOG_ASCII_BUF_sec_acl_permset(nam) \
	LOG_ASCII_BUF(nam,DEB_ASCII_LEN_SEC_ACL_PERMSET)

#define DEB_ASCII_LEN_SEC_ACL_TYPE 20
#define DEB_ASCII_BUF_sec_acl_type(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_SEC_ACL_TYPE)
#define LOG_ASCII_BUF_sec_acl_type(nam) \
	LOG_ASCII_BUF(nam,DEB_ASCII_LEN_SEC_ACL_TYPE)

#ifdef DCE_CDS_DEBUG

static char *
deb_ascii_handle (
    char	*,
    handle_t	);

static char *
deb_ascii_sec_acl_permset (
    char	      *,
    sec_acl_permset_t  );

static char *
deb_ascii_sec_acl_type (
    char	   *,
    sec_acl_type_t  );

static void
print_acl (
     sec_acl_p_t );

static void
print_accessor_info (
     handle_t );

#else

# define print_acl(a)
# define print_accessor_info(h)

#endif /* #ifdef DCE_CDS_DEBUG */


/*
 * Prototypes for end point vector functions
 */
void rdacl_lookup_v0 (handle_t, sec_acl_component_name_t, uuid_t *, 
		      sec_acl_type_t, sec_acl_result_t *);
void rdacl_lookup_v1 (handle_t, sec_acl_component_name_t, uuid_t *, 
		      sec_acl_type_t, sec_acl_result_t *);
void rdacl_replace_v0 (handle_t, sec_acl_component_name_t, uuid_t *, sec_acl_type_t,
		       sec_acl_list_t *, error_status_t *);
void rdacl_replace_v1 (handle_t, sec_acl_component_name_t, uuid_t *, sec_acl_type_t,
		       sec_acl_list_t *, error_status_t *);
void rdacl_get_access (handle_t, sec_acl_component_name_t, uuid_t *,
		       sec_acl_permset_t *, error_status_t *);
boolean32 rdacl_test_access (handle_t, sec_acl_component_name_t, uuid_t *,
			     sec_acl_permset_t, error_status_t *);
boolean32 rdacl_test_access_on_behalf (handle_t, sec_acl_component_name_t, uuid_t *,
				       sec_id_pac_t *, sec_acl_permset_t, error_status_t *);
void rdacl_get_manager_types (handle_t, sec_acl_component_name_t, sec_acl_type_t,
			      unsigned32, unsigned32 *, unsigned32 *, uuid_t *, error_status_t *);
void rdacl_get_mgr_types_semantics (handle_t, sec_acl_component_name_t, sec_acl_type_t,
				    unsigned32, unsigned32 *, unsigned32 *, uuid_t *,
				    sec_acl_posix_semantics_t *, error_status_t *);
void rdacl_get_printstring (handle_t, uuid_t *, unsigned32, uuid_t *,
                       sec_acl_printstring_t *, boolean32 *, unsigned32 *,
                       unsigned32 *, sec_acl_printstring_t *, error_status_t *);
void rdacl_get_referral (handle_t, sec_acl_component_name_t, uuid_t *,
			 sec_acl_type_t, sec_acl_tower_set_t *, error_status_t *);


/*
 * End point vector for v0 rdacl support
 */
rdaclif_v0_0_epv_t server_acl_v0_epv = {
   rdacl_lookup_v0,
   rdacl_replace_v0,
   rdacl_get_access,
   rdacl_test_access,
   rdacl_test_access_on_behalf,
   rdacl_get_manager_types,
   rdacl_get_printstring,
   rdacl_get_referral,
   rdacl_get_mgr_types_semantics 
};

/*
 * End point vector for v1 rdacl support
 */
rdaclif_v1_0_epv_t server_acl_v1_epv = {
   rdacl_lookup_v1,
   rdacl_replace_v1,
   rdacl_get_access,
   rdacl_test_access,
   rdacl_test_access_on_behalf,
   rdacl_get_manager_types,
   rdacl_get_printstring,
   rdacl_get_referral,
   rdacl_get_mgr_types_semantics 
};

idl_void_p_t 
client_malloc (idl_size_t size)
{
    return((idl_void_p_t)malloc(size));
}

void 
client_free (idl_void_p_t ptr)
{
    free(ptr);
}

/********************************************************************/
/*
 * dacl_get
 *        Retrieve acl in wire format
 */
dns_status_t 
dacl_get (db_data_t    *rec_p,
          sec_acl_p_t  *acl_pp,
          int          type)
{
    DBSet_t       *set_p;
    int           idx;
    dns_status_t	status;

    /* Locate attribute set */
    if (find_attribute(rec_p->data_p,
		   dns_AttributeName(type), &set_p, &idx) == DNS_SUCCESS) {
      /* Get last value and use it */
      DBSetMember_t *const invalue_p = find_last_value(set_p);
      if (invalue_p) {
	struct {
	    union {
		long            align;
		bytes_u         buf[MAXATTRIBUTE];
	    } flat;
	} flatbuf;

	/* This has to be aligned */
	COPY_bytes(invalue_p->dm_data, flatbuf.flat.buf,
		   invalue_p->dm_length);
	if (dpe_acl_flat_to_wire((char *)flatbuf.flat.buf,
				    DPE_ACL_RPCMEM, acl_pp) != dpe_acl_failure)
	  status = DNS_SUCCESS;
	else
	  status = DNS_NONSRESOURCES;
      } else
	status = DNS_UNKNOWNENTRY;
    } else
      status = DNS_UNKNOWNENTRY;

    return(status);
}

/*
 * Add/replace 1 sec_acl_entry_t for a specific user in a sec_acl_t
 * The input acl_entry is devoured.
 * Since dynamic memory was allocated using rpc_ss_allocate it will
 * be freed by the stubs.
 */
void 
dacl_add_acl_entry (sec_acl_entry_t  *new_entry_p,
                    sec_acl_t        *acl_p,
                    int              merge)
{

    int i;
    sec_acl_entry_t *entry_p;
    unsigned32 status;
    int done = FALSE;
    sec_acl_permset_t oldrights;

    /* Initalize type uuids*/
    (void)pthread_once(&type_once, type_init);

    /*
     * Add an acl_entry to the acl_t.  A specific user can have only
     * one entry so need to check this out and replace the existing extry
     * if required
     */
    for (i=0; i<acl_p->num_entries; ++i) {
	entry_p = &acl_p->sec_acl_entries[i];
	if (entry_p->entry_info.entry_type !=
	    new_entry_p->entry_info.entry_type) continue;

	/* Save existing rights in-case they must be merged */
	oldrights = entry_p->perms;
	switch (entry_p->entry_info.entry_type) {
	  case sec_acl_e_type_user_obj:
	  case sec_acl_e_type_group_obj:
	  case sec_acl_e_type_other_obj:
	  case sec_acl_e_type_any_other:
	  case sec_acl_e_type_mask_obj:
	  case sec_acl_e_type_unauthenticated:
	  case sec_acl_e_type_user_obj_deleg:
	  case sec_acl_e_type_group_obj_deleg:
	  case sec_acl_e_type_other_obj_deleg:
	  case sec_acl_e_type_any_other_deleg:
	    /* Replace permissions - only one allowed */
	    *entry_p  = *new_entry_p;
	    done = TRUE;
	    break;
	  case sec_acl_e_type_user:
	  case sec_acl_e_type_group:
	  case sec_acl_e_type_foreign_other:
	  case sec_acl_e_type_user_deleg:
	  case sec_acl_e_type_group_deleg:
	  case sec_acl_e_type_for_other_deleg:
	    if (uuid_equal(&entry_p->entry_info.tagged_union.id.uuid,
	      &new_entry_p->entry_info.tagged_union.id.uuid, &status)) {
		/* Replace permissions - only one allowed */
		done = TRUE;
		*entry_p = *new_entry_p;
	    }
	    break;
	  case sec_acl_e_type_foreign_user:
	  case sec_acl_e_type_foreign_group:
	  case sec_acl_e_type_for_user_deleg:
	  case sec_acl_e_type_for_group_deleg:
	    if (
	      uuid_equal(&entry_p->entry_info.tagged_union.foreign_id.id.uuid,
     	      &new_entry_p->entry_info.tagged_union.foreign_id.id.uuid,
	      &status) &&
              uuid_equal(&entry_p->entry_info.tagged_union.foreign_id.realm.uuid,
     	      &new_entry_p->entry_info.tagged_union.foreign_id.realm.uuid,
			 &status)) {
	      done = TRUE;
	      *entry_p = *new_entry_p;
	    }
	    break;
	  case sec_acl_e_type_extended:
	  default:
	    break;
	} /* end switch */
	if (done == TRUE) {
	    if (merge)
	      entry_p->perms = entry_p->perms | oldrights;
	    break;
	}
    } /* End for statement */

    /* Still need to add it */
    if (done == FALSE) {
	/*
	 * Add a new entry
	 */
	entry_p = (sec_acl_entry_t *)rpc_ss_allocate(sizeof(sec_acl_entry_t) *
				   (acl_p->num_entries+1));

	if (entry_p) {
	    /* Copy the ones we are adding first */
	    COPY_bytes(acl_p->sec_acl_entries, entry_p,
		       sizeof(sec_acl_entry_t) * acl_p->num_entries);

	    acl_p->sec_acl_entries = entry_p;

	    /* Now add the new one */
	    acl_p->sec_acl_entries[acl_p->num_entries] = *new_entry_p;
	    acl_p->num_entries++;
	}
    }
}

/*
 * Return permission bits allowed for entry type
 */
sec_acl_permset_t 
dacl_manager_permset (int type)
{
    DEB_ASCII_BUF_EntryType(typeBuf)
    DEB_ASCII_BUF_sec_acl_permset(retBuf)

#ifdef DCE_CDS_DEBUG

    byte_t	entryType = type;

#endif

    mgr_type_t	*mgr_p;


    /* Initalize type uuids*/
    (void)pthread_once(&type_once, type_init);

    if (type == ET_directory)
      mgr_p = &dir;
    else if (type == ET_clearinghouse)
      mgr_p = &cle;
    else
      mgr_p = &cat;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " <dacl_manager_permset(%s) return(%s)",
        deb_ascii_EntryType(typeBuf,&entryType),
        deb_ascii_sec_acl_permset(retBuf,mgr_p->perms)));

    return(mgr_p->perms);
}

/*
 * Return a pointer to  the manager type for an entry
 */
uuid_t 
*dacl_manager_type (int type)
{
    DEB_ASCII_BUF_EntryType(typeBuf)

#ifdef DCE_CDS_DEBUG

    byte_t	entryType = type;

#endif

    mgr_type_t	*mgr_p;


    /* Initalize type uuids*/
    (void)pthread_once(&type_once, type_init);

    if (type == ET_directory)
      mgr_p = &dir;
    else if (type == ET_clearinghouse)
      mgr_p = &cle;
    else
      mgr_p = &cat;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " <dacl_manager_type(%s) return(%s)",
        deb_ascii_EntryType(typeBuf,&entryType),
        mgr_p->mgr));

    return(&mgr_p->uuid);
}

/*
 * Make the call to the sec_acl api interface to check whether
 * a user and the server have the appropriate access .
 * Bind to CDS directory.
 */
dns_status_t 
dacl_test_access (user_descriptor_t  *user_p,
                  FullName_t         *name_p,
                  sec_acl_permset_t  yourperms,
                  sec_acl_permset_t  myperms)
{
    DEB_ASCII_BUF_user_descriptor(userBuf)
    DEB_ASCII_BUF_FullName(nameBuf)
    DEB_ASCII_BUF_sec_acl_permset(yourpermsBuf)
    DEB_ASCII_BUF_sec_acl_permset(mypermsBuf)
    unsigned char    dirname[DNS_STR_FNAME_MAX];
    int              len = sizeof(dirname);
    dns_status_t     status = DNS_NOCOMMUNICATION;

    /* Initalize type uuids*/
    (void)pthread_once(&type_once, type_init);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " >dacl_test_access(%s,%s,%s,%s)",
        deb_ascii_user_descriptor(userBuf,user_p),
        deb_ascii_FullName(nameBuf,name_p),
        deb_ascii_sec_acl_permset(yourpermsBuf,yourperms),
        deb_ascii_sec_acl_permset(mypermsBuf,myperms)));

    /* Get the string format of the directory name */
    if (dnsCvtCDSFullToStr((dns_full_name_t *)name_p, dirname, &len)
		== DNS_SUCCESS) {
	sec_acl_handle_t h;
	sec_id_pac_t     *pac_p;
	error_status_t   secstatus;
	unsigned32       def_expire;
	unsigned32       rpc_status;
	unsigned32       prev_auth;

	/* Get current expiratation for re-set */
	rpc_ns_mgmt_inq_exp_age(&def_expire, &rpc_status);

	/*
	 * Bypass the cache. Handles to
	 * to directories contain dynamic endpoints and will
	 * be invalid as servers go up and down
	 */

	rpc_ns_mgmt_set_exp_age(0, &rpc_status);

	rpc_ns_set_authn(dns_nameserver.Authenticated, &prev_auth, &rpc_status);

	/* Get a binding handle to the directory */
	sec_acl_bind(dirname, FALSE, &h, &secstatus);

	/* Set it back to what it was */
	rpc_ns_mgmt_set_exp_age(def_expire, &rpc_status);

	if (secstatus == error_status_ok) {
	    error_status_t   relstatus;

	    /* See if I - the server have access.  */
	    if (sec_acl_test_access(h, &dir.uuid, myperms, &secstatus) ==
		TRUE) {
		/* See if caller has access */
		pac_p = (user_p->ud_is_pac) ? &user_p->ud_pac : NULL;
		if (sec_acl_test_access_on_behalf(h, &dir.uuid, pac_p,
					   yourperms, &secstatus) == TRUE)
		  status = DNS_SUCCESS;
		else if (secstatus == error_status_ok)
		  status = DNS_ACCESSDENIED;
	    }
	    else if (secstatus == error_status_ok)
	      status = DNS_UNTRUSTEDCH;

	    /* Need to free this handle */
	    sec_acl_release_handle(&h, &relstatus);
	} else
	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_server,
                svc_c_debug3,
                "dacl_test_access(%s,%s,%s,%s) sec_acl_bind(%s,0,,%p->%d) failed\n",
	        deb_ascii_user_descriptor(userBuf,user_p),
       	        deb_ascii_FullName(nameBuf,name_p),
	        deb_ascii_sec_acl_permset(yourpermsBuf,yourperms),
	        deb_ascii_sec_acl_permset(mypermsBuf,myperms),
	        dirname, 
                &secstatus, 
                secstatus));

	/*
	 * Handle sec_acl_bind and test error cases here 
	 */
	if ((status == DNS_NOCOMMUNICATION) &&
	    (secstatus == sec_acl_object_not_found))
	  status = DNS_UNKNOWNENTRY;
    } else
      status = DNS_NAMESERVERBUG;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " <dacl_test_access(%s,%s,%s,%s) return(%d)",
        deb_ascii_user_descriptor(userBuf,user_p),
        deb_ascii_FullName(nameBuf,name_p),
        deb_ascii_sec_acl_permset(yourpermsBuf,yourperms),
        deb_ascii_sec_acl_permset(mypermsBuf,myperms),
        status));

    return(status);
}

/*
 * dacl_lookup
 *      Retrieve a dacl from another server
 */
dns_status_t 
dacl_lookup (FullName_t      *name_p,
             byte_t          **flat_pp,
             int             *len_p,
             sec_acl_type_t  type)
{
    DEB_ASCII_BUF_FullName(nameBuf)
    DEB_ASCII_BUF_sec_acl_type(typeBuf)
    unsigned char    dirname[DNS_STR_FNAME_MAX];
    int              len = sizeof(dirname);
    dns_status_t     status = DNS_NOCOMMUNICATION;

    /* Initalize type uuids*/
    (void)pthread_once(&type_once, type_init);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " >dacl_loookup(%s,,,%s)",
        deb_ascii_FullName(nameBuf,name_p),
        deb_ascii_sec_acl_type(typeBuf,type)));

    /* Get the string format of the directory name */
    if (dnsCvtCDSFullToStr((dns_full_name_t *)name_p, dirname, &len)
		== DNS_SUCCESS) {
      DEB_ASCII_BUF_handle(hBuf)
      sec_acl_handle_t h;
      error_status_t   secstatus;
      sec_acl_list_t   list;
      unsigned32       def_expire;
      unsigned32       rpc_status;
      unsigned32       prev_auth;

      /* Get current expiratation for re-set */
      rpc_ns_mgmt_inq_exp_age(&def_expire, &rpc_status);

      /*
       * Bypass the cache. Handles to
       * to directories contain dynamic endpoints and will
       * be invalid as servers go up and down
       */

      rpc_ns_mgmt_set_exp_age(0, &rpc_status);

      rpc_ns_set_authn(dns_nameserver.Authenticated, &prev_auth, &rpc_status);

      /* Get a binding handle to the directory */
      sec_acl_bind(dirname, FALSE, &h, &secstatus);

      /* Set it back to what it was */
      rpc_ns_mgmt_set_exp_age(def_expire, &rpc_status);

      if (secstatus == error_status_ok) {
	  error_status_t   relstatus;

	  sec_acl_lookup(h, &dir.uuid, type, &list, &secstatus);
	  if (secstatus == error_status_ok)
	    if (list.num_acls) {
	      if (dpe_acl_wire_to_flat(list.sec_acls[0], DPE_ACL_RPCMEM,
				(char **)flat_pp, len_p) == dpe_acl_success)
		status = DNS_SUCCESS;
	      else
		status = DNS_NONSRESOURCES;

	      /*
	       * Since we are a server thread, the allocate/deallocator is
	       * is rpc_ss_allocate so we cannot use sec_acl_release
	       * memory will be released when the server stub exits
	       */

	    }
	  else
	  {

#ifdef DCE_CDS_DEBUG

	    const char *type_p = deb_ascii_sec_acl_type(typeBuf,type);

#endif
	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_server,
                svc_c_debug3,
	        "dacl_loookup(%s,,,%s) sec_acl_lookup(%s,%s,%s,,%p->%d) failed\n",
		deb_ascii_FullName(nameBuf,name_p), type_p,
	        deb_ascii_handle(hBuf,(handle_t)h), dir.mgr, type_p,
	        &secstatus, 
		secstatus));
	}

	  sec_acl_release_handle(&h, &relstatus);
      } else
	 DCE_SVC_DEBUG((
             cds__svc_handle,
             cds_svc_server,
             svc_c_debug3,
	     "dacl_loookup(%s,,,%s) sec_acl_bind(%s,0,,%p->%d) failed\n",
             deb_ascii_FullName(nameBuf,name_p),
	     deb_ascii_sec_acl_type(typeBuf,type),
	     dirname, 
             &secstatus, 
             secstatus));

      if (status != DNS_SUCCESS)
	  if (secstatus == sec_acl_object_not_found)
	      status = DNS_UNKNOWNENTRY;
	  else if (secstatus == sec_acl_no_acl_found)
	      status = DNS_NOTFND;
  } else
      status = DNS_NAMESERVERBUG;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " <dacl_loookup(%s,,,%s) return(%d)",
        deb_ascii_FullName(nameBuf,name_p),
        deb_ascii_sec_acl_type(typeBuf,type),
        status));
    return(status);
}

/*
 * type_init
 *     Convert static string uuid to uuid_t
 */
static void 
type_init (void)
{
    unsigned32 status;


    /* Convert static manager type uuids from string to uuid_t */
    uuid_from_string((unsigned char *)cat.mgr, &cat.uuid, &status);
    if (status != uuid_s_ok)
      uuid_create_nil(&cat.uuid, &status);

    uuid_from_string((unsigned char *)dir.mgr, &dir.uuid, &status);
    if (status != uuid_s_ok)
      uuid_create_nil(&dir.uuid, &status);

    uuid_from_string((unsigned char *)cle.mgr, &cle.uuid, &status);
    if (status != uuid_s_ok)
      uuid_create_nil(&cle.uuid, &status);
}

/*
 * ch_from_handle
 *   Locate the clearinghouse storing the uuid in the rpc binding
 * handle.  If multiple clearinghouse, the one containing the master,
 * wins.
 */
static db_stream_t 
*ch_from_handle (rpc_binding_handle_t h)
{
    uuid_t              objuid;
    unsigned32          status;
    db_stream_t         *stream_p,*savestream_p = (db_stream_t *)NULL;
    nsgbl_ch_t		*ch_p;
    dir_hash_entry_t    *hash_p;

    /* Get object uuid out of handle */
    rpc_binding_inq_object(h, &objuid, &status);
    if (status == rpc_s_ok) {
	/*
	 * Would like to lock the memory section keeping list of clearinghouses
         * consistant but on a create clearinghouse/add_replica/dacl_lookup
         * we could end up in a deadlock.
	 * SERVER_LOCK(dns_nameserver.ns_mutex)
	 * SERVER_END_LOCK(dns_nameserver.ns_mutex)
	 */
	/* Loop through clearinghouses */
	for (ch_p = (nsgbl_ch_t *)dns_nameserver.ns_clearinghouses.next_p;
	    ch_p != (nsgbl_ch_t *)&dns_nameserver.ns_clearinghouses;
	     ch_p = (nsgbl_ch_t *)ch_p->ch_clearinghouses.next_p) {
	    /* Open stream, which locks cle state */
	    status = db_open_stream(&ch_p->ch_uid, &stream_p,
				    OP_ReadAttribute);
	    if (status == DNS_SUCCESS) {
		hash_p = db_hash_uid(stream_p->db_file_p, &objuid);
		if (hash_p) {
		    if (db_dir_get_replica_state(hash_p) == RS_on) {
			if (db_dir_get_replica_type(hash_p) == RT_master) {
			    /* return this one */
			    if (savestream_p)
			      (void)db_close_stream(savestream_p);
			    savestream_p = stream_p;
			    savestream_p->db_dir_hash_p = hash_p;
			    break;
			} else if (!savestream_p) {
			  /* Haven't found anything yet. Save this
                           * open stream but keep looking for master.
			   */
			    savestream_p = stream_p;
			    savestream_p->db_dir_hash_p = hash_p;
			    continue;
			}
		    } /* entry is on */
		} /* end got a hash entry */
		(void)db_close_stream(stream_p);
	    } /* End got a stream */
	} /* End loop through ch list */
      }
    return(savestream_p);
}

/*
 * check the manager type again what we know it should be
 */
static error_status_t 
check_manager (uuid_t          *manager_type_p,
               int             type,
	       sec_acl_type_t  acl_type)
{
    unsigned32        uuid_status;
    uuid_t            *mgr_p;

    /* Set up based on type */
    switch (type) {
      case ET_directory:
	if (acl_type == sec_acl_type_default_object) {
	    /*
	     * Return OK if:
	     *    manager_type == dir.uuid (for pre-4.0 directories)
	     *    manager_type == cat.uuid (for >= 4.0 directories)
	     */
	    if (uuid_equal(manager_type_p, &dir.uuid, &uuid_status) ||
		uuid_equal(manager_type_p, &cat.uuid, &uuid_status))
		  return(error_status_ok);
	    else
		return(sec_acl_invalid_manager_type);
	}
	mgr_p = &dir.uuid;
	break;

      case ET_clearinghouse:
	mgr_p = &cle.uuid;
	break;

      default:
	/* Pre v.0 directories returned directory manager.  Allow */
	if ((uuid_equal(manager_type_p, &dir.uuid, &uuid_status)) ||
	   (uuid_equal(manager_type_p, &cat.uuid, &uuid_status)))
	    return(error_status_ok);
	else
	    return(sec_acl_invalid_manager_type);
	break;
    }

    if (!uuid_equal(manager_type_p, mgr_p, &uuid_status) ||
	(uuid_status != uuid_s_ok))
      return(sec_acl_invalid_manager_type);
    else
      return(error_status_ok);


}

/*
 * check the entry types in the acl list to ensure the are compatible, 
 * and validate directory version number for *_deleg entries
 */
static error_status_t 
check_acl (int             type,
           sec_acl_list_t  *list_p,
           int             *isowner_p,
	   db_stream_t       *stream_p)
{
  db_stream_t       *pp_stream_p;
  unsigned32        status;
  const sec_acl_permset_t *perm_p;
  sec_acl_entry_t   *entry_p;
  int               i;
  version_t         dir_version;
  
  /*
   * Set up based on type 
   */
  switch (type) {
  case ET_directory:
    perm_p = &dir.perms;
    break;
  case ET_clearinghouse:
    perm_p = &cle.perms;
    break;
  default:
    perm_p = &cat.perms;
    break;
  }
  
  /*
   * If not list to check, return success 
   */
  if (list_p == NULL)
    return(error_status_ok);
  
  /* 
   * Make sure the acl list is supported 
   */
  if (list_p->num_acls != 1)
    return(sec_acl_not_implemented);
  
  /*
   * Loop through acl_entries.  Reject if permissions are not supported
   * or there is a group_obj, other_obj or mask_obj or there
   * is no owner
   */
  *isowner_p = FALSE;
  for (i=0; i<list_p->sec_acls[0]->num_entries; ++ i) {
      entry_p = &list_p->sec_acls[0]->sec_acl_entries[i];
      if ((entry_p->entry_info.entry_type == sec_acl_e_type_user_obj) ||
	  (entry_p->entry_info.entry_type == sec_acl_e_type_group_obj) ||
	  (entry_p->entry_info.entry_type == sec_acl_e_type_mask_obj) ||
	  (entry_p->entry_info.entry_type == sec_acl_e_type_user_obj_deleg) ||
	  (entry_p->entry_info.entry_type == sec_acl_e_type_group_obj_deleg))
	return(sec_acl_invalid_entry_type);
      if ((*perm_p & entry_p->perms) != entry_p->perms)
	return(sec_acl_invalid_permission);
      if (sec_acl_perm_owner & entry_p->perms)
	*isowner_p = TRUE;
      /*
       * Check version of directory when using a v1.1 delegation type
       */
      if ((entry_p->entry_info.entry_type == sec_acl_e_type_user_deleg  ) ||
	  (entry_p->entry_info.entry_type == sec_acl_e_type_for_user_deleg ) ||
	  (entry_p->entry_info.entry_type == sec_acl_e_type_group_deleg ) ||
	  (entry_p->entry_info.entry_type == sec_acl_e_type_for_group_deleg ) ||
	  (entry_p->entry_info.entry_type == sec_acl_e_type_any_other_deleg ) ||
	  (entry_p->entry_info.entry_type == sec_acl_e_type_for_other_deleg ) ||
	  (entry_p->entry_info.entry_type == sec_acl_e_type_user_obj_deleg ) ||
	  (entry_p->entry_info.entry_type == sec_acl_e_type_other_obj_deleg ) ||
	  (entry_p->entry_info.entry_type == sec_acl_e_type_group_obj_deleg )) {
	  /* Doesn't matter what type is */
	  if (!stream_p->db_dir.data_p) {
	      db_dir_read(stream_p);
	  }
	  db_dir_get_version(stream_p, &dir_version);

	  if (EXT8(dir_version.vr_major) < 4) {
	      return(cds_s_delegdirversion); /* need >= 4.0 version directory
					      * to use delegation */
	  }
      }
  } /* end for */
  return(error_status_ok);  
}

/*
 * Locate the record of interest.  Return stream and record read.
 */
static unsigned32 
get_entry (handle_t                  h,
           sec_acl_component_name_t  component_name,
           db_stream_t               **stream_pp,
           db_data_t                 **rec_pp,
           int                       optype)
{
    DEB_ASCII_BUF_handle(hBuf)
    DEB_ASCII_BUF_sec_acl_type(typeBuf)
    unsigned32   rpcstatus;
    dns_status_t status;

    /* Initalize type uuids*/
    (void)pthread_once(&type_once, type_init);

    /*
     * First locate the clearinghouse the dacl is in
     */
    /* Locate clearinghouse that stores the uuid in the handle */
    *stream_pp = ch_from_handle(h);

    if (!*stream_pp) return(sec_acl_object_not_found);

    /* If the component name is null, we are looking for the directory */
    if (!strlen((char *)component_name)) {
	if (optype == WALKTREE_LOOKUP_OP)
	  status = db_dir_read(*stream_pp);
	else
	  status = db_dir_read_for_update(*stream_pp);
	*rec_pp = &(*stream_pp)->db_dir;
    } else {
	uuid_t         root;
	dns_opq_sname  Iname;
	int            Ilen = sizeof(Iname);

	/* Set up db_key with component name */
	rpc_binding_inq_object(h, &root, &rpcstatus);
	/* convert name to a internal simplename */
	if ((rpcstatus == rpc_s_ok) &&
	    (dnsCvtCDSSimpleToOpq(component_name, FALSE,
				 &Iname, &Ilen) == DNS_SUCCESS)) {
	    names_new(&root, (SimpleName_t *)&Iname,
		      (FullName_t *)(*stream_pp)->db_key);
	    status = check_exists(*stream_pp, optype);
	    if ((status == DNS_SUCCESS) && (optype == WALKTREE_UPDATE_OP))
	      status = db_dir_read(*stream_pp);
	} else
	  status = DNS_UNKNOWNENTRY;
	*rec_pp = &(*stream_pp)->db_entry;
    }

    /* Return results */
    if (status != DNS_SUCCESS)
      return(sec_acl_object_not_found);
    else if (operationallowed(optype,
	      db_dir_get_replica_type((*stream_pp)->db_dir_hash_p),
			     (*rec_pp)->data_type) == DNS_SUCCESS)
      return(error_status_ok);
    else
      return(sec_acl_site_read_only);
}

/*
 * *** rdacl procedures ***
 */
/*
 * Locate a dacl for a namespace entry and return it
 *
 * For version 0, redirect processing for the CDS server mgmt acl
 * entry to the common ACL manager.
 */
void 
rdacl_lookup_v0 (handle_t                  h,
		 sec_acl_component_name_t  component_name,
		 uuid_t                    *manager_type_p,
		 sec_acl_type_t            sec_acl_type,
		 sec_acl_result_t          *result_p)
{
   DEB_ASCII_BUF_handle(hBuf)
   DEB_ASCII_BUF_sec_acl_type(typeBuf)
   uuid_t            obj_uuid;
   int               old_cancel;
   error_status_t st;


   DCE_SVC_DEBUG((
		  cds__svc_handle,
		  cds_svc_server,
		  svc_c_debug2,
		  " >rdacl_lookup_v0(%s,,,%s,%p)",
		  deb_ascii_handle(hBuf,h),
		  deb_ascii_sec_acl_type(typeBuf,sec_acl_type),
		  result_p));

   old_cancel = dthread_setcancel(CANCEL_OFF);
   
   rpc_binding_inq_object (h,
			   &obj_uuid,
			   &result_p->st);

   if ( result_p->st == rpc_s_ok && uuid_equal (&obj_uuid, &server_mgmt_dacl_uuid, &st)) {
      /*
       * server management dacl request
       */
      dce_rdacl_lookup_v0(h, component_name, manager_type_p, sec_acl_type, result_p);
   }
   else if ( result_p->st == rpc_s_ok) {
     rdacl_lookup(h, component_name, manager_type_p, sec_acl_type, result_p);
     
     /*
      * Convert the delegate entries to extended ones (where apropriate). 
      */
     sec_acl_encode_v0_entries(rpc_ss_allocate, rpc_ss_free, FALSE,
			       result_p->tagged_union.sec_acl_list, &result_p->st);
   }   
   
   (void)dthread_setcancel(old_cancel);

   DCE_SVC_DEBUG((cds__svc_handle,
		  cds_svc_server,
		  svc_c_debug2,
		  " <rdacl_lookup_v0(%s,,,%s,%p)",
		   deb_ascii_handle(hBuf,h),
		  deb_ascii_sec_acl_type(typeBuf,sec_acl_type),
		  result_p));
}
/*
 * Locate a dacl for a namespace entry and return it
 *
 * For version 1, redirect processing for the CDS server mgmt acl
 * entry to the common ACL manager.
 */
void 
rdacl_lookup_v1 (handle_t                  h,
		 sec_acl_component_name_t  component_name,
		 uuid_t                    *manager_type_p,
		 sec_acl_type_t            sec_acl_type,
		 sec_acl_result_t          *result_p)
{
   DEB_ASCII_BUF_handle(hBuf)
   DEB_ASCII_BUF_sec_acl_type(typeBuf)
   uuid_t            obj_uuid;
   int               old_cancel;
   error_status_t st;


   DCE_SVC_DEBUG((
		  cds__svc_handle,
		  cds_svc_server,
		  svc_c_debug2,
		  " >rdacl_lookup_v1(%s,,,%s,%p)",
		  deb_ascii_handle(hBuf,h),
		  deb_ascii_sec_acl_type(typeBuf,sec_acl_type),
		  result_p));

   old_cancel = dthread_setcancel(CANCEL_OFF);
   
   rpc_binding_inq_object (h,
			   &obj_uuid,
			   &result_p->st);

   if (result_p->st == rpc_s_ok && uuid_equal (&obj_uuid, &server_mgmt_dacl_uuid, &st)) {
      /*
       * server management dacl request
       */
      dce_rdacl_lookup(h, component_name, manager_type_p, sec_acl_type, result_p);
   }
   else if (result_p->st == rpc_s_ok) {
      rdacl_lookup(h, component_name, manager_type_p, sec_acl_type, result_p);
   }

   (void)dthread_setcancel(old_cancel);

   DCE_SVC_DEBUG((
		  cds__svc_handle,
		  cds_svc_server,
		  svc_c_debug2,
		  " <rdacl_lookup_v1(%s,,,%s,%p)",
		   deb_ascii_handle(hBuf,h),
		  deb_ascii_sec_acl_type(typeBuf,sec_acl_type),
		  result_p));
}

/*
 * Common rdacl_lookup code namespace entries
 */
void 
rdacl_lookup (handle_t                  h,
              sec_acl_component_name_t  component_name,
              uuid_t                    *manager_type_p,
              sec_acl_type_t            sec_acl_type,
              sec_acl_result_t          *result_p)
{
    DEB_ASCII_BUF_handle(hBuf)
    DEB_ASCII_BUF_sec_acl_type(typeBuf)
    dns_status_t      status;
    db_data_t         *rec_p;
    int               idx;
    error_status_t    mgmt_status;
    error_status_t st;
    int thstatus;
    db_stream_t       *stream_p;
	  
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " >rdacl_lookup(%s,,,%s,%p)",
        deb_ascii_handle(hBuf,h),
        deb_ascii_sec_acl_type(typeBuf,sec_acl_type),
        result_p));

    /* namespace dacl request */
    /* Get entry to operate on  */
    result_p->st =get_entry(h, component_name, &stream_p, &rec_p,
			     WALKTREE_LOOKUP_OP);

    /* If we have the entry in question, make sure manager is valid */
    if (result_p->st == error_status_ok)
      result_p->st = check_manager(manager_type_p, rec_p->data_type, sec_acl_type);

    /* Now make sure use has read access */
    if (result_p->st == error_status_ok) {
      user_descriptor_t user;

      ZERO_bytes(&user, sizeof(user));

      if ((result_p->st = get_user_creds(h, &user)) == error_status_ok) {
	  /* Check for any access */
	  status = security_any(&user, rec_p);
	  if (status == DNS_UNKNOWNENTRY)
	    result_p->st = sec_acl_object_not_found;
	  else
	    result_p->st = error_status_ok;
      }
    }

    if (result_p->st == error_status_ok) {
	switch (sec_acl_type) {

	  case sec_acl_type_object:
	    idx = DNS_OBJECTACL;
	    break;
	  case sec_acl_type_default_object:
	    idx = DNS_DEFOBJECTACL;
	    break;
	  case sec_acl_type_default_container:
	    idx = DNS_DEFCONTAINERACL;
	    break;
	  default:
	    result_p->st = sec_acl_invalid_acl_type;
	}

	/* If we have valid index, find the attribute */
	if (result_p->st == error_status_ok) {
	    result_p->tagged_union.sec_acl_list =
	      (sec_acl_list_t *)rpc_ss_allocate(sizeof(sec_acl_list_t));
	    /* make sure we have valid memory */
	    if (!result_p->tagged_union.sec_acl_list)
	      result_p->st = sec_acl_cant_allocate_memory;
	    else {

	      status = dacl_get(rec_p,
			    &result_p->tagged_union.sec_acl_list->sec_acls[0],
				idx);
	      if (status != DNS_SUCCESS) {
		result_p->st = sec_acl_no_acl_found;
		/* Let stubs clean it up */
		result_p->tagged_union.sec_acl_list = (sec_acl_list_t *)NULL;
	      } else {
		  version_t  dir_version;

		  result_p->tagged_union.sec_acl_list->num_acls = 1;
		  if (sec_acl_type == sec_acl_type_object) {
		      if (!stream_p->db_dir.data_p) {
			  db_dir_read(stream_p);
		      }
		      db_dir_get_version(stream_p, &dir_version);
		      if (EXT8(dir_version.vr_major) >= 4) {
			  if (rec_p->data_type == ET_object ||
			      rec_p->data_type == ET_childPointer ||
			      rec_p->data_type == ET_softlink) {
			      result_p->tagged_union.sec_acl_list->sec_acls[0]->sec_acl_manager_type = cat.uuid;
			  }
		      }
		  }
	    }
	  }
	}
      }
    if (stream_p)
      (void)db_close_stream(stream_p);
  
    DCE_SVC_DEBUG((
		   cds__svc_handle,
		   cds_svc_server,
		   svc_c_debug2,
		   " <rdacl_lookup(%s,,,%s,%p->%d)",
		   deb_ascii_handle(hBuf,h),
		   deb_ascii_sec_acl_type(typeBuf,sec_acl_type),
		   result_p,
		   result_p->st));
  }

/*
 * Filter out replace requests for the server mgmt ACL and pass
 * them on to the common ACL manager.
 */
void 
rdacl_replace_v0 (handle_t                  h,
		  sec_acl_component_name_t  component_name,
		  uuid_t                    *manager_type_p,
		  sec_acl_type_t            sec_acl_type,
		  sec_acl_list_t            *sec_acl_list_p,
		  error_status_t            *st_p)
{
    DEB_ASCII_BUF_handle(hBuf)
    uuid_t            obj_uuid;
    int               old_cancel;


    DCE_SVC_DEBUG((
		   cds__svc_handle,
		   cds_svc_server,
		   svc_c_debug2,
		   " >rdacl_replace_v0(%s,,,,,%p)",
		   deb_ascii_handle(hBuf,h),
		   st_p));
    
    old_cancel = dthread_setcancel(CANCEL_OFF);
    
    /*
     * Determine which DACl manager this is for 
     */
    rpc_binding_inq_object (h,
			    &obj_uuid,
			    st_p);
    if (*st_p == rpc_s_ok && uuid_equal (&obj_uuid, &server_mgmt_dacl_uuid, st_p)) {
       /* 
	* server management dacl request
	*/
       dce_rdacl_replace_v0 (h, component_name, manager_type_p, 
			     sec_acl_type, sec_acl_list_p, st_p);
    } 
    else if (*st_p == rpc_s_ok) {
       rdacl_replace (h, component_name, manager_type_p, 
		      sec_acl_type, sec_acl_list_p, st_p);
    }

    DCE_SVC_DEBUG((
		   cds__svc_handle,
		   cds_svc_server,
		   svc_c_debug2,
		   " <rdacl_replace_v0(%s,,,,,%p)",
		   deb_ascii_handle(hBuf,h),
		   st_p));
}

/*
 * Filter out replace requests for the server mgmt ACL and pass
 * them on to the common ACL manager.
 */
void 
rdacl_replace_v1 (handle_t                  h,
		  sec_acl_component_name_t  component_name,
		  uuid_t                    *manager_type_p,
		  sec_acl_type_t            sec_acl_type,
		  sec_acl_list_t            *sec_acl_list_p,
		  error_status_t            *st_p)
{
    DEB_ASCII_BUF_handle(hBuf)
    uuid_t            obj_uuid;
    int               old_cancel;


    DCE_SVC_DEBUG((
		   cds__svc_handle,
		   cds_svc_server,
		   svc_c_debug2,
		   " >rdacl_replace_v1(%s,,,,,%p)",
		   deb_ascii_handle(hBuf,h),
		   st_p));
    
    old_cancel = dthread_setcancel(CANCEL_OFF);
    
    /*
     * Determine which DACl manager this is for 
     */
    rpc_binding_inq_object (h,
			    &obj_uuid,
			    st_p);
    if (*st_p == rpc_s_ok && uuid_equal (&obj_uuid, &server_mgmt_dacl_uuid, st_p)) {
       /* 
	* server management dacl request
	*/
       dce_rdacl_replace (h, component_name, manager_type_p, 
			     sec_acl_type, sec_acl_list_p, st_p);
    } 
    else if (*st_p == rpc_s_ok) {
       rdacl_replace (h, component_name, manager_type_p, 
		      sec_acl_type, sec_acl_list_p, st_p);
    }

    DCE_SVC_DEBUG((
		   cds__svc_handle,
		   cds_svc_server,
		   svc_c_debug2,
		   " <rdacl_replace_v1(%s,,,,,%p)",
		   deb_ascii_handle(hBuf,h),
		   st_p));
}

/*
 * Locate a dacl for a namespace entry and replace it.
 */
void 
rdacl_replace (handle_t                  h,
               sec_acl_component_name_t  component_name,
               uuid_t                    *manager_type_p,
               sec_acl_type_t            sec_acl_type,
               sec_acl_list_t            *sec_acl_list_p,
               error_status_t            *st_p)
{
    DEB_ASCII_BUF_handle(hBuf)
    db_stream_t       *stream_p;
    dns_status_t      status;
    int               len;
    db_data_t         *rec_p;
    Update_u          upd;
    Update_t          *update_p = (Update_t *)upd;
    tlog_dsc_t        tlog;
    byte_t            *data_p;
    int               isowner;
    uuid_t            obj_uuid;
    int               old_cancel;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " >rdacl_replace(%s,,,,,%p)",
        deb_ascii_handle(hBuf,h),
        st_p));
    old_cancel = dthread_setcancel(CANCEL_OFF);

/* namespace management dacl request */

        *st_p = get_entry(h, component_name, &stream_p, &rec_p,
		      WALKTREE_UPDATE_OP);

       /*
        * If we have the entry in question, make sure manager is valid...
        */
	if (*st_p == error_status_ok) {
	  *st_p = check_manager(manager_type_p, rec_p->data_type, sec_acl_type);
	  /* 
	   * ...and make sure the acl is valid 
	   */
	  if (*st_p == error_status_ok)
	    *st_p = check_acl(rec_p->data_type,
			      sec_acl_list_p, 
			      &isowner,
			      stream_p );
	}

    /* Now make sure user has owner permission */
	if (*st_p == error_status_ok) {
	    user_descriptor_t user;

	    ZERO_bytes(&user, sizeof(user));

	    if ((*st_p = get_user_creds(h, &user)) == error_status_ok) {
	      status = security_dacl(&user, rec_p, sec_acl_perm_owner,
				     ALLPerms);
	      if (status == DNS_UNKNOWNENTRY)
		  *st_p = sec_acl_object_not_found;
	      else if (status != DNS_SUCCESS)
		  *st_p  = sec_acl_not_authorized;
	      else *st_p = error_status_ok;
	    }
	}

    /* If all is okay, replace that value */
	if (*st_p == error_status_ok) {
	  attribute_descriptor_t *dns_p;

	  switch (sec_acl_type) {
	  case sec_acl_type_object:
	    if (!isowner)
	      *st_p = sec_acl_no_owner;
	    else
	      dns_p = dns_Attribute(DNS_OBJECTACL);
	    break;
	  case sec_acl_type_default_object:
	    dns_p = dns_Attribute(DNS_DEFOBJECTACL);
	    break;
	      case sec_acl_type_default_container:
	    dns_p = dns_Attribute(DNS_DEFCONTAINERACL);
	    break;
	  default:
	    *st_p = sec_acl_invalid_acl_type;
	  }

	/* If we have valid index, find the attribute */
	  if (*st_p == error_status_ok) {
	    tlog.cnt = 1;
	    tlog.buf_p = (unsigned char *)update_p;
	    /* Make sure their are entries to replace */
	    if (sec_acl_list_p->sec_acls[0]->num_entries) {
	      /* convert from wire to flag */
	      if (dpe_acl_wire_to_flat(sec_acl_list_p->sec_acls[0],
		  DPE_ACL_STDMEM, (char **)&data_p, &len) == dpe_acl_success) {
		/* If the size exceeds the max size allowed, free memory */
		if ((len > dns_p->max) && (data_p)) {
			free((char *)data_p);
			*st_p = sec_acl_mgr_no_space;
		} else {
		    tlog.len = update_dns_build(update_p, UD_present, NULL,
					    dns_p, BL_true, len, data_p);

		    free((char *)data_p);
		    /* Insert member and log it */
		    (void)applyupdate(stream_p, rec_p->data_type, update_p,
				dns_p, TRUE, &tlog);
		}
	      } else
		  *st_p = sec_acl_cant_allocate_memory;
	    } else {
	      /* Remove the attribute */
	      tlog.len = update_dns_build(update_p, UD_absent, NULL,
					  dns_p, BL_false, 0, NULL);
	      /* Insert member and log it */
	      (void)applyupdate(stream_p, rec_p->data_type, update_p,
			  dns_p, TRUE, &tlog);

	    }
	  }
	}
	if (stream_p)
	    (void)db_close_stream(stream_p);
    (void)dthread_setcancel(old_cancel);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " <rdacl_replace(%s,,,,,%p->%d)",
        deb_ascii_handle(hBuf,h),
        st_p,
        *st_p));
 }


/*
 * Locate a dacl and return the cummulative access for a user.
 */
void 
rdacl_get_access (handle_t                  h,
                  sec_acl_component_name_t  component_name,
                  uuid_t                    *manager_type_p,
                  sec_acl_permset_t         *net_rights_p,
                  error_status_t            *st_p)
{
  DEB_ASCII_BUF_handle(hBuf)
  dns_status_t      status;
  db_data_t         *rec_p;
  uuid_t            obj_uuid;
  sec_acl_t        *acl_p;
  error_status_t    aclstatus;
  int               old_cancel;
  int thstatus;
  db_stream_t       *stream_p = NULL;
  
  DCE_SVC_DEBUG((
		 cds__svc_handle,
		 cds_svc_server,
		 svc_c_debug2,
		 " >rdacl_get_access(%s,,,,%p)",
		 deb_ascii_handle(hBuf,h),
		 st_p));

  old_cancel = dthread_setcancel(CANCEL_OFF);
  
  /* Determine which DACl manager this is for */
  rpc_binding_inq_object (h,
			  &obj_uuid,
			  st_p);
  if (*st_p == rpc_s_ok) {
    
    if (uuid_equal (&obj_uuid, &server_mgmt_dacl_uuid, st_p)) {
      /*
       * server management dacl request
       */
      dce_rdacl_get_access (h, component_name, manager_type_p,
			    net_rights_p, st_p);
    }
    else {
      *st_p = error_status_ok;
      *net_rights_p = dce_acl_c_no_permissions;	/* default perms - none */
      
      /*
       * Find the entry 
       */
      *st_p = get_entry(h, component_name, &stream_p, &rec_p,
			WALKTREE_LOOKUP_OP);
      if (*st_p == error_status_ok) {
	
	/*
	 * If we have the entry in question, make sure mgr is valid 
	 */
	*st_p = check_manager(manager_type_p, rec_p->data_type, sec_acl_type_object);
	if (*st_p == error_status_ok) {
	  
	  /* 
	   * Now get the ACL
	   */
	  status = dacl_get(rec_p, &acl_p, DNS_OBJECTACL);
	  if (status != CDS_SUCCESS) {
	    *st_p = sec_acl_object_not_found;
	    }
	  else {
	    rpc_authz_cred_handle_t cred_h;
	    rpc_authz_cred_handle_t *cred_h_p = NULL;
	    /* 
	     * Get the access information
	     */
	    dce_acl_inq_client_creds(h, &cred_h, st_p);
	    if (*st_p == error_status_ok) {
	      if (dpe_acl_mgr_get_access (acl_p, &cred_h, 
					  net_rights_p, 
					  &aclstatus) == FALSE) 
		*st_p = sec_acl_object_not_found;
	      else if (!*net_rights_p)
		*st_p = sec_acl_object_not_found;
	    } 
	  } /* got dacl */
	} /* checked manager */
      } /* got entry */
    } /* uuid_equal */
  } /* got binding object */
  
  if (stream_p)
    (void)db_close_stream(stream_p);
    
  (void)dthread_setcancel(old_cancel);
  
  DCE_SVC_DEBUG((
		 cds__svc_handle,
		 cds_svc_server,
		 svc_c_debug2,
		 " <rdacl_get_access(%s,,,,%p->%d)",
		 deb_ascii_handle(hBuf,h),
		 st_p,
		 *st_p));
}

/*
 * Locate a dacl and test if the caller has the requested permissions
 */
boolean32 
rdacl_test_access (handle_t                  h,
                   sec_acl_component_name_t  component_name,
                   uuid_t                    *manager_type_p,
                   sec_acl_permset_t         desired_permset,
                   error_status_t            *st_p)
{
    DEB_ASCII_BUF_handle(hBuf)
    db_stream_t       *stream_p;
    dns_status_t      status;
    db_data_t         *rec_p;
    boolean32         result = FALSE;
    uuid_t            obj_uuid;
    int	 	      old_cancel;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " >rdacl_test_access(%s,,,,%p)",
        deb_ascii_handle(hBuf,h),
        st_p));

    old_cancel = dthread_setcancel(CANCEL_OFF);
/* Determine which DACl manager this is for */

    rpc_binding_inq_object (h,
			    &obj_uuid,
			    st_p);
    if (*st_p == rpc_s_ok) {
      if (uuid_equal (&obj_uuid, &server_mgmt_dacl_uuid, st_p)) {
	 /*
	  * server management dacl request
	  */
	 result = dce_rdacl_test_access (h, component_name, manager_type_p, 
					 desired_permset, st_p);
      }
      else if (*st_p == rpc_s_ok) {

/* namespace dacl request */

	*st_p = get_entry(h, component_name, &stream_p, &rec_p,
			  WALKTREE_LOOKUP_OP);

    /* If we have the entry in question, make sure manager is valid */
	if (*st_p == error_status_ok)
	    *st_p = check_manager(manager_type_p, rec_p->data_type, sec_acl_type_object);

    /* Get user pac setup */
	if (*st_p == error_status_ok) {
	  user_descriptor_t user;

	  ZERO_bytes(&user, sizeof(user));

	  if ((*st_p = get_user_creds(h, &user)) == error_status_ok) {
	    /* check for permissions user wants */
	    status = security_dacl(&user, rec_p, desired_permset, ALLPerms);
	    if (status == DNS_UNKNOWNENTRY)
	      *st_p = sec_acl_object_not_found;
	    else {
	      *st_p = error_status_ok;
	      if (status == DNS_SUCCESS)
		result = TRUE;
	    }
	  }
	}
	if (stream_p)
	    (void)db_close_stream(stream_p);
      }
    }
    (void)dthread_setcancel(old_cancel);
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " <rdacl_test_access(%s,,,,%p->%d) return(%d)",
        deb_ascii_handle(hBuf,h),
        st_p,
        *st_p,
        result));
    return(result);

}
/*
 * Locate a dacl and test if a user (not the caller) has access
 */
boolean32 
rdacl_test_access_on_behalf (handle_t                  h,
                             sec_acl_component_name_t  component_name,
                             uuid_t                    *manager_type_p,
                             sec_id_pac_t              *subject_p,
                             sec_acl_permset_t         desired_permset,
                             error_status_t            *st_p)
{
  DEB_ASCII_BUF_handle(hBuf)
  db_stream_t       *stream_p;
  dns_status_t      status;
  db_data_t         *rec_p;
  boolean32         result = FALSE;
  uuid_t            obj_uuid;
  int               old_cancel;
  int thstatus;

  DCE_SVC_DEBUG((
		 cds__svc_handle,
		 cds_svc_server,
		 svc_c_debug2,
		 " >rdacl_test_access_on_behalf(%s,,,,,%p)",
		 deb_ascii_handle(hBuf,h),
		 st_p));

  /* Disallow the thread to be cancelled underneath us */
  old_cancel = dthread_setcancel(CANCEL_OFF);


  /* Determine which DACl manager this is for */
  
  rpc_binding_inq_object (h,
			  &obj_uuid,
			  st_p);
  if (*st_p == rpc_s_ok) {
    if (uuid_equal (&obj_uuid, &server_mgmt_dacl_uuid, st_p)) {
      
      print_accessor_info (h); /* print client info for debug */
      /*
       * server management dacl request
       */
      result = dce_rdacl_test_access_on_behalf (h, component_name, manager_type_p, 
						subject_p, desired_permset, st_p);
    }
    else if (*st_p == rpc_s_ok) {

      /* namespace dacl request */

      *st_p = get_entry(h, component_name, &stream_p, &rec_p,
			WALKTREE_LOOKUP_OP);

      /* If we have the entry in question, make sure manager is valid */
      if (*st_p == error_status_ok)
	*st_p = check_manager(manager_type_p, rec_p->data_type, sec_acl_type_object);

    /* Get user pac setup */
      if (*st_p == error_status_ok) {
	user_descriptor_t user;
	
	ZERO_bytes(&user, sizeof(user));
	
	if ((*st_p = get_user_creds(h, &user)) == error_status_ok) {

	  /* Now make sure caller has some permissions */
	  status = security_dacl(&user, rec_p, desired_permset,
				 ALLPerms);
	  if (status == DNS_UNKNOWNENTRY)
	    *st_p = sec_acl_object_not_found;
	  else {
	    /* Now make sure onbehalf has desired permission */
	    if (subject_p != (sec_id_pac_t *)NULL) {
	      user.ud_pac = *subject_p;
	      user.ud_is_pac = 1;
	      user.ud_cred.data = NULL; /* cause a v0 PAC-based check */
	    } else
	      user.ud_is_pac = 0;
	    
	    status = security_dacl (&user, rec_p, desired_permset,
				    ALLPerms);
	    if (status == DNS_UNKNOWNENTRY)
	      *st_p = sec_acl_object_not_found;
	    else {
	      *st_p = error_status_ok;
	      if (status == DNS_SUCCESS)
		result = TRUE;
	    }
	  } /* caller has permissions */
	}
      } /* manager checked */

    if (stream_p)
      (void)db_close_stream(stream_p);

    } /* namespace dacl request */       
  } /* found object */

  (void)dthread_setcancel(old_cancel);
  DCE_SVC_DEBUG((
		 cds__svc_handle,
		 cds_svc_server,
		 svc_c_debug2,
		 " <rdacl_test_access_on_behalf(%s,,,,,%p->%d) return(%d)",
		 deb_ascii_handle(hBuf,h),
		 st_p,
		 *st_p,
		 result));
  
  return(result);
}

/*
 * Locate an entry and return its manager type.  Type of manager
 * depends on the entry type.  Space is preallocate by the stubs.
 * CDS supports 3 manager types, but only 1 per entry.
 */
void 
rdacl_get_manager_types (handle_t                  h,
                         sec_acl_component_name_t  component_name,
                         sec_acl_type_t            sec_acl_type,
                         unsigned32                size_avail,
                         unsigned32                *size_used_p,
                         unsigned32                *num_types_p,
                         uuid_t                    manager_types[],
                         error_status_t            *st_p)
{
    DEB_ASCII_BUF_handle(hBuf)
    db_stream_t       *stream_p;
    db_data_t         *rec_p;
    int               status;
    uuid_t            obj_uuid;
    int               old_cancel;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " >rdacl_get_manager_types(%s,,,,,,,%p)",
        deb_ascii_handle(hBuf,h),
        st_p));

    /* Prevent thread from being cancelled from under us */
    old_cancel = dthread_setcancel(CANCEL_OFF);

    /* Determine which DACl manager this is for */

    /* Initialize return values */
    *size_used_p = 0;
    *num_types_p = 0;

    rpc_binding_inq_object (h,
			    &obj_uuid,
			    st_p);
    if (*st_p == rpc_s_ok) {
      if (uuid_equal (&obj_uuid, &server_mgmt_dacl_uuid, st_p)) {
	 /*
	  * server management dacl request
	  */
	 dce_rdacl_get_manager_types (h, component_name, sec_acl_type, size_avail,
				      size_used_p, num_types_p, manager_types, st_p);
      }
      else if (*st_p == rpc_s_ok) {

	/* namespace management dacl request */

	/* Locate entry */
	*st_p  = get_entry(h, component_name, &stream_p, &rec_p,
			   WALKTREE_LOOKUP_OP);
	
	if (*st_p == error_status_ok) {
	    user_descriptor_t user;

	    ZERO_bytes(&user, sizeof(user));

	    if ((*st_p = get_user_creds(h, &user)) == error_status_ok) {
	      /* Now make sure caller has any permission */
	      status = security_any(&user, rec_p);
	      if (status == DNS_UNKNOWNENTRY)
		*st_p = sec_acl_object_not_found;
	      else {
		*st_p = error_status_ok;
		/* We only support one manger type/entry */
		
		if (size_avail < 1) {
		  /* Caller will have to try again with more space allocated */
		  *size_used_p = 0;
		  *num_types_p = 1;
		  
		} else {
		  /* its gonna fit, fill it in */
		  *size_used_p = *num_types_p = 1;
		  switch (sec_acl_type) {
		  case sec_acl_type_object:
                      if (rec_p->data_type == ET_directory)
                        *manager_types = dir.uuid;
		      else if (rec_p->data_type == ET_clearinghouse)
			*manager_types = cle.uuid;
		      else {
			  version_t  dir_version;

			  if (!stream_p->db_dir.data_p) {
			      db_dir_read(stream_p);
			  }
			  db_dir_get_version(stream_p, &dir_version);
			  if (EXT8(dir_version.vr_major) >= 4) {
			      /* looking for cataloged entry */
			      *manager_types = cat.uuid;
			  }
			  else
			    *manager_types = dir.uuid;
		      }
		      break;

		  case sec_acl_type_default_object:
		    if (rec_p->data_type == ET_directory) {
			version_t  dir_version;

			/*
			 * If DirectoryVersion is > 3.0, acl manager type
			 * should be ET_object.  However, for backward
			 * compatibility, if DirectoryVersion is 3.0,
			 * the acl manager type will remain the same, old,
			 * wrong ET_directory.
			 */
			if (!stream_p->db_dir.data_p) {
			    db_dir_read(stream_p);
			}
			db_dir_get_version(stream_p, &dir_version);
			if (EXT8(dir_version.vr_major) < 4)
			  *manager_types = dir.uuid;
			else
			  *manager_types = cat.uuid;
		    }
		    break;

		  case sec_acl_type_default_container:
		    
		    /* These types are only supported on directores */
		    if (rec_p->data_type == ET_directory) {
		      *manager_types = dir.uuid;
		    }
		    else
		      /* looking for cataloged entry */
		      *st_p = sec_acl_not_implemented;
		    break;
		    
		  default:
		    *st_p = sec_acl_invalid_acl_type;
		    
		  } /* end case */
		} /* end have enough room in manger_types */
	      } /* End have access */
	    } /* end have user info */
	  } /* end Found a clearinghouse */
	if (stream_p)
	  (void)db_close_stream(stream_p);
      }
    }
    (void)dthread_setcancel(old_cancel);
    DCE_SVC_DEBUG((
		   cds__svc_handle,
		   cds_svc_server,
		   svc_c_debug2,
		   " <rdacl_get_manager_types(%s,,,,,,,%p->%d)",
		   deb_ascii_handle(hBuf,h),
		   st_p,
		   *st_p));
  }


/*
 * Locate an entry and return its manager type.  Type of manager
 * depends on the entry type.  Space is preallocate by the stubs.
 * CDS supports 3 manager types, but only 1 per entry.
 * Let caller know we don't support any of the *optional* POSIX
 * semantics.  Just call rdacl_get_manager_types to do the rest.
 */
void 
rdacl_get_mgr_types_semantics (handle_t                   h,
                               sec_acl_component_name_t   component_name,
                               sec_acl_type_t             sec_acl_type,
                               unsigned32                 size_avail,
                               unsigned32                 *size_used_p,
                               unsigned32                 *num_types_p,
                               uuid_t                     manager_types[],
                               sec_acl_posix_semantics_t  semantics[],
                               error_status_t             *st_p)
{
    uuid_t	obj_uuid;
    int i;

    rpc_binding_inq_object (h,
			    &obj_uuid,
			    st_p);
    if (*st_p == rpc_s_ok) {
      if (uuid_equal (&obj_uuid, &server_mgmt_dacl_uuid, st_p)) {
	 /*
	  * server management rdacl request
	  */
	 dce_rdacl_get_mgr_types_semantics (h, component_name, sec_acl_type, size_avail, 
					    size_used_p, num_types_p, manager_types, 
					    semantics, st_p);
      }
      else {
	 rdacl_get_manager_types(h, component_name, sec_acl_type, size_avail,
				 size_used_p, num_types_p, manager_types, st_p);
	 if (*st_p == rpc_s_ok) {
	    for (i = 0; i < *size_used_p; i++) {
	       semantics[i] = sec_acl_posix_no_semantics;
	    }
	 }
      }
   }
}

/*
 * Locate a dacl manager type and return printstings.
 * Space is preallocated by the stub.
 */
void 
rdacl_get_printstring (handle_t               h,
                       uuid_t                 *manager_type_p,
                       unsigned32             size_avail,
                       uuid_t                 *manager_type_chain_p,
                       sec_acl_printstring_t  *manager_info_p,
                       boolean32              *tokenize_p,
                       unsigned32             *num_printstrings_p,
                       unsigned32             *size_used_p,
                       sec_acl_printstring_t  printstrings[],
                       error_status_t         *st_p)
{
    DEB_ASCII_BUF_handle(hBuf)
    unsigned char *tmpbuf;
    mgr_type_t	*mgr_p = NULL;
    unsigned32  status;
    int		j;
    int		old_cancel;
    
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " >rdacl_get_printstring(%s,,,,,,,,,%p)",
        deb_ascii_handle(hBuf,h),
        st_p));

    /*
     * Redirect request for management ACL printstring 
     */
    if (uuid_equal (manager_type_p, &server_mgmt_dacl_uuid, st_p)) {
       dce_rdacl_get_printstring (h,
				  manager_type_p,
				  size_avail,
				  manager_type_chain_p,
				  manager_info_p,
				  tokenize_p,
				  num_printstrings_p,
				  size_used_p,
				  printstrings,
				  st_p	  );
       return;
    }
    
    /* Prevent thread from being cancelled from under us */
    old_cancel = dthread_setcancel(CANCEL_OFF);

    *tokenize_p = 0; /* permissions are unambiguous and may be concatenated */
    *num_printstrings_p = 0;
    *size_used_p = 0;

    /* permissions do not extend beyond 32 bits => no managers in chain */
    uuid_create_nil(manager_type_chain_p, &status);

    *st_p = error_status_ok;

    /* Determine which DACl manager this is for */
    if (uuid_equal(manager_type_p, &cat.uuid, &status)) 
	mgr_p = &cat;
    else if (uuid_equal(manager_type_p, &dir.uuid, &status)) 
	mgr_p = &dir;
    else if (uuid_equal(manager_type_p, &cle.uuid, &status)) 
	mgr_p = &cle;
    else
	/* someone is very confused */
	*st_p = sec_acl_unknown_manager_type;
    
    if (*st_p == error_status_ok) {
      tmpbuf = dce_msg_get_msg(mgr_p->evt_help, &status);
      strcpy((char *)manager_info_p->helpstring, (char *)tmpbuf);
      free(tmpbuf);
      
      tmpbuf = dce_msg_get_msg(mgr_p->evt_print, &status);
      strcpy((char *)manager_info_p->printstring, (char *)tmpbuf);
      free(tmpbuf);
      
      *num_printstrings_p = mgr_p->perm_cnt;
      if (mgr_p->perm_cnt <= size_avail)  {
	const long *evt_p = mgr_p->evt;
	
	*size_used_p = mgr_p->perm_cnt;
	for (j=0; j<mgr_p->perm_cnt; ++j) {
	  tmpbuf = dce_msg_get_msg(*evt_p++, &status);
	  (void)strcpy((char *)printstrings[j].printstring, (char *)tmpbuf);
	  free(tmpbuf);
	  
	  tmpbuf = dce_msg_get_msg(*evt_p++, &status);
	  (void)strcpy((char *)printstrings[j].helpstring, (char *)tmpbuf);
	  free(tmpbuf);
	  
	  printstrings[j].permissions = *evt_p++;
	}
      } else
	  *size_used_p = 0;
    }
    
  (void)dthread_setcancel(old_cancel);
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " <rdacl_get_printstring(%s,,,,,,,,,%p->%d)",
      deb_ascii_handle(hBuf,h),
      st_p,
      *st_p));
}


/*
 * Locate a dacl and build a referral for another server.
 */
void 
rdacl_get_referral (handle_t                  h,
                    sec_acl_component_name_t  component_name,
                    uuid_t                    *manager_type_p,
                    sec_acl_type_t            sec_acl_type,
                    sec_acl_tower_set_t       *towers_p,
                    error_status_t            *st_p)
{
    DEB_ASCII_BUF_handle(hBuf)
    db_stream_t       *stream_p;
    dns_status_t      status;
    db_data_t         *rec_p;
    uuid_t            obj_uuid;
    int               old_cancel;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " >rdacl_get_referral(%s,,,,,%p)",
        deb_ascii_handle(hBuf,h),
        st_p));

    *towers_p = (sec_acl_tower_set_t )NULL;

    /* Prevent thread from being cancelled from under us */
    old_cancel = dthread_setcancel(CANCEL_OFF);

    /* Determine which DACl manager this is for */

    rpc_binding_inq_object (h,
			    &obj_uuid,
			    st_p);
    if (*st_p == rpc_s_ok) {
	if (uuid_equal (&obj_uuid, &server_mgmt_dacl_uuid, st_p)) {
	   /* 
	    * server management dacl request
	    * (no referrals in this manager.)
	    */
	   dce_rdacl_get_referral (h, component_name, manager_type_p, 
				   sec_acl_type, towers_p, st_p);

	} else if (*st_p == rpc_s_ok) {
	    /* Locate master of directory => ignore component name */
	    *st_p = get_entry(h, (sec_acl_component_name_t)"",
			      &stream_p, &rec_p, WALKTREE_LOOKUP_OP);

	    if (*st_p == error_status_ok) {
	      user_descriptor_t user;

	      ZERO_bytes(&user, sizeof(user));

	      if ((*st_p = get_user_creds(h, &user)) == error_status_ok) {
		/* Now make sure caller has any permission */
		status = security_any(&user, rec_p);
		if (status == DNS_UNKNOWNENTRY)
		  *st_p = sec_acl_object_not_found;
		else {
		  DBSet_t *set_p;
		  int idx;
		  if (find_attribute(rec_p->data_p,
				     dns_AttributeName(DNS_REPLICAS),
				     &set_p, &idx) != DNS_SUCCESS)
		    *st_p = DNS_NAMESERVERBUG;
		  else {
		    /*
		     * Turn address into towers with port numbers -
		     * will produce fully bound handles
		     * Have to since directories not registered with rpcd
		     *
		     */
		    *st_p = dacl_resolve_ep(set_p, towers_p);
		  } /* End found replica set */
		} /* End have access */
	      }
	    }
	    if (stream_p)
	      (void)db_close_stream(stream_p);
	  } /* Got binding */
      }
    (void)dthread_setcancel(old_cancel);
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " <rdacl_get_referral(%s,,,,,%p->%d)",
        deb_ascii_handle(hBuf,h),
        st_p,
        *st_p));
}

/*
 * Look for master then resolve binding handle so we can return it to caller
 * as towers that include a port so caller doesn't have to interact
 * with rpcd - which doesn't know about directories.
 */
static unsigned32 
dacl_resolve_ep (DBSet_t              *set_p,
                 sec_acl_tower_set_t  *tower_p)
{
    DBSetMember_t *val_p;
    int idx;
    int cnt;
    ReplicaPointer_t *rp_p;
    unsigned32 rpcstatus, status, freestatus;
    rpc_binding_handle_t h;
    byte_t      *binding_p;
    byte_t      **array_p;
    unsigned_char_t   *protseq;
    rpc_tower_vector_p_t tower_vector_p;
    uuid_t          rpuuid;
    /* Loop through replica set */
    for (idx=0; idx<NUM_DBSet(set_p); ++idx) {
	val_p = (DBSetMember_t *)INDEX_DBSet(set_p, idx);
	if (!EXT8(val_p->dm_valid) ||
	    !EXT8(val_p->dm_value_present)) continue;
	/* Locate the master */
	rp_p = (ReplicaPointer_t *)val_p->dm_data;
	if (EXT8(FLD_ReplicaPointer_replicaType(rp_p)) !=
	    RT_master) continue;

	/* Convert to an array of string bindings */
	if (dnsCvtTowerToStr(rp_p->rp_addressHint, &binding_p) != DNS_SUCCESS)
	  return(sec_acl_cant_allocate_memory);

	/*
	 * Fully resolve each binding so the editor bypasses
	 * rpcd
	 */
	array_p = (byte_t **)binding_p;
	for (cnt=0, idx=0; array_p[idx] != NULL; ++idx)
	  ++cnt;

	/* Each address will produce 1 binding handle */
	*tower_p = (sec_acl_tower_set_t )rpc_ss_allocate(sizeof(*tower_p) +
				   cnt*sizeof(sec_acl_twr_ref_t));
	if (!*tower_p) {
	    free(binding_p);
	    return(sec_acl_cant_allocate_memory);
	}
	(*tower_p)->count = 0;
	cnt = 0;
	status = rpc_s_ok;
	for (idx=0; array_p[idx] != NULL; ++idx) {

	    rpc_string_binding_parse(array_p[idx], NULL, &protseq,
				     NULL, NULL, NULL, &rpcstatus);
	    if (rpcstatus != rpc_s_ok) continue;

	    rpc_string_free(&protseq, &freestatus);

	    rpc_binding_from_string_binding(array_p[idx],
					    &h, &rpcstatus);
	    if (rpcstatus != rpc_s_ok) {
		rpc_string_free(&protseq, &freestatus);
		continue;
	    }

	    /* Set object to clearinghouse - that is registered */
	    COPY_ObjUID(rp_p->rp_CHID, &rpuuid);
	    rpc_binding_set_object(h, &rpuuid, &rpcstatus);
	    if (rpcstatus == rpc_s_ok) {
		/* Since this request makes an outgoing RPC and assumes itself
		  * to be a client( uses free) we must swap memory management
		  */
		rpc_ss_set_client_alloc_free(client_malloc, client_free);
		rpc_ep_resolve_binding(h, rdaclif_v0_0_s_ifspec,
				       &rpcstatus);
		rpc_ss_set_client_alloc_free(rpc_ss_allocate, rpc_ss_free);
		if (rpcstatus != rpc_s_ok)
		  /* Save this one */
		  status = rpcstatus;
		else {
		    rpc_tower_vector_from_binding(rdaclif_v0_0_s_ifspec, h,
						&tower_vector_p, &rpcstatus);

		    if (rpcstatus == rpc_s_ok) {
			if (tower_vector_p->count >= 1) {
			    /* copy it to stub memory */
			    (*tower_p)->towers[cnt] =
				  (sec_acl_twr_ref_t)rpc_ss_allocate(
				     tower_vector_p->tower[0]->tower_length +
							     sizeof(twr_t));
			    if ((*tower_p)->towers[cnt]) {
				COPY_bytes(tower_vector_p->tower[0],
					   (*tower_p)->towers[cnt],
			               tower_vector_p->tower[0]->tower_length +
				       sizeof(twr_t));
				++cnt;
			    }
			}
		    } /* End got tower vector */
		    rpc_tower_vector_free(&tower_vector_p, &rpcstatus);
		} /* End resolve handle */
	    } /* End set object */
	    rpc_binding_free(&h, &rpcstatus);
	} /* End for loop through string bindings */
	free(binding_p);
	(*tower_p)->count = cnt;
	/* Make sure we found some */
	if (cnt != 0) return(error_status_ok);
	if (status != rpc_s_ok) return(status);
	return(sec_acl_cant_allocate_memory);
    } /* End for loop looking for masters */

    return(DNS_NAMESERVERBUG);
}

/* 
 * server_mgmt_acl_init: initialize server mgmt DACL.
 * Called once at startup, before the rdacl and cdscp  interface
 * is registered.
*/
int 
server_mgmt_acl_init (void)
{
   boolean32    must_create_default_acl = FALSE;
   boolean32	must_store_acl = FALSE;
   boolean32 	have_old_acl_file = FALSE;
   unsigned32   flags = 0;
   error_status_t      status, file_status;
   static sec_acl_t    theAcl ;
   sec_acl_p_t	       temp_acl_p = &theAcl ;    
   char		mgt_acl_filename[CDS_STR_FNAME_MAX];
   char * 	lastchp = 0;
   char * 	new_acl_filename_p = 0 ;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " >server_mgmt_acl_init()"));

   /*
    * Use old file name to build new file name
    */
   sprintf (mgt_acl_filename, CDS_SERVER_MGMT_ACL_FILENAME, dcelocal_path);
  
   new_acl_filename_p = malloc(sizeof(mgt_acl_filename));
   strcpy(new_acl_filename_p,mgt_acl_filename);
   if((lastchp = strrchr( new_acl_filename_p,  '/')) != 0)
     lastchp++;
   strcpy( lastchp, CDS_SERVER_MGMT_V1_ACL_FILENAME);

   /* 
    * See if v1.1-type file already exists 
    */
   flags = db_c_index_by_uuid ;       

   if ( access( new_acl_filename_p, R_OK) == 0) {
      
      /*
       * open the server acl database - assume that it exists.
       * if it fails, then add the create flag, and try again 
       */
      dce_db_open( new_acl_filename_p, 
		  (const char *)0, /* backend type */
		  flags,           /* index by uuid */
		  (dce_db_convert_func_t)dce_rdacl_convert, /* cnvrt functn*/ 
		  &acl_db,         /* database handle */
		  &status) ; 
      if (status != error_status_ok) {
	 dce_svc_printf(CDS_S_ACLINIT_MSG, (long)status); 
	 return status ;
      }

      /* 
       * need to get the new acl from the database
       */
      temp_acl_p = &theAcl ;

      dce_db_fetch_by_uuid (acl_db, 
			    &server_mgmt_dacl_uuid,
			    temp_acl_p, 
			    &status) ;

      if (status != error_status_ok) { 
	/*
	 * Have new file, but it doesn't have the acl
	 * we need.  Create default
	 */
	dce_svc_printf(CDS_S_NOACL_MSG, (long)status); 
	must_create_default_acl = TRUE;
      }
   }
   else {		/* new file doesn't exists */	
      /* 
       * Check the old file  
       */
      if ( access ( mgt_acl_filename, R_OK) == 0) {

	 must_store_acl = TRUE ;
 	 
	 /* 
	  * Try to read it 
	  */
	 dpe_acl_storage_read (mgt_acl_filename, 
			       &temp_acl_p, 
			       &file_status);

	 if (file_status != dpe_acl_s_ok) {
	    must_create_default_acl = TRUE;
	    dce_svc_printf(CDS_S_NOACL_MSG, (long)status); 
	                          /* failed to read, creating */
	 }
	 else {
	    have_old_acl_file = TRUE ;
	 }
      }
      else {
	 must_create_default_acl = must_store_acl = TRUE;
      }

      /* 
       * since the new db doesn't exist, create one 
       */
      flags |= db_c_create ; /* create it  */

      dce_db_open (new_acl_filename_p,    /* filename */
		   (const char *)0,       /* backend type */
		   flags,                 /* db_c_create */
		   (dce_db_convert_func_t)dce_rdacl_convert,  
		   &acl_db,               /* database handle */
		   &status);
      
      if (status != error_status_ok) {
	 dce_svc_printf(CDS_S_ACLWRITE_MSG, (long)status); 
	 return status ;
      }

      if (! have_old_acl_file) 
	must_create_default_acl = must_store_acl = TRUE;
   }

   /*
    * Now we have an open v1.0 acl file
    */
   {
      /*
       * enable stub management memory allocation 
       * (for backing store code: rdacl_lookup ) 
       */
      rpc_sm_enable_allocate(&status);
      if (status != error_status_ok) {
	 dce_svc_printf(CDS_S_BAD_RPC_ALLOC_MSG, (long)status); 
	 return status ;
      }
   }

   
   {
     /*
      * register object types for this database 
      * N.B. we also register a private resolver function 
      * to map input parms to a uuid 
      */
     sec_acl_printstring_t *server_mgmt_acl_help = &server_printstrings[0];
     sec_acl_printstring_t *server_mgmt_perm_help = &server_printstrings[1];
     
     build_printstrings ();

     dce_acl_register_object_type (acl_db,               /* database handle */
				   &server_mgmt_dacl_uuid,  /* server uuid  */
				   PERM_STRING_CNT,        
				   server_mgmt_perm_help,     /* "rwc" perms */
				   server_mgmt_acl_help,      /* "cdsserver" */
				   sec_acl_perm_control,  
				   0, 
				   server_acl_resolve, (void *)NULL, 
				    0, /* flags */
				   &status);
     if (status != error_status_ok) {
       dce_svc_printf(CDS_S_BAD_REGISTER_OBJ_MSG, (long)status); 
       return status ;
     }
   }

   if (must_create_default_acl) { 
      /* 
       * Either corrupt or non-existent 
       */
      if ( (status = create_def_server_mgmt_acl (temp_acl_p)) != error_status_ok ) {
	 dce_svc_printf(CDS_S_ACLFAIL_MSG, (long)status);
	 return status;
      }
   }

   if (must_store_acl) {
      /*
       * Save our ACL in database
       */
      dce_db_store_by_uuid (acl_db, 
			    &server_mgmt_dacl_uuid,
			    temp_acl_p, 
			    &status);
      if (status != error_status_ok) {
	 dce_svc_printf(CDS_S_ACLWRITE_MSG, (long)status);
	 return status;
      }
   }

   if (have_old_acl_file) {
     dpe_acl_free_acl (temp_acl_p);
     (void)remove (mgt_acl_filename);
   }
   
   server_acl_ready = TRUE;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " <server_mgmt_acl_init() return(%ld)",
      status));

   return (status);
}

/*
 * build_printstrings - build structure for printstrings needed
 * for the common acl manager out of our arrays.  Basicly, do
 * conversion from constants to sams file text strings and
 * move the strings in the printstring structure
 */
static void
build_printstrings (void) 
{
  unsigned char *tmpbuf;
  int  string_index,id_index;
  error_status_t status;

  for (id_index = 0, string_index = 0; 
       id_index < sizeof(server_mgmt_help_ids)/sizeof(long); 
       id_index += EVT_ENTRY_SIZE/sizeof(long), string_index++) {
    tmpbuf = dce_msg_get_msg(server_mgmt_help_ids[id_index], &status);
    strcpy ((char *)server_printstrings[string_index].printstring, (char *)tmpbuf);
    free (tmpbuf);
    tmpbuf = dce_msg_get_msg(server_mgmt_help_ids[id_index+1], &status);    
    strcpy ((char *)server_printstrings[string_index].helpstring, (char *)tmpbuf);
    free (tmpbuf);
    server_printstrings[string_index].permissions = server_mgmt_help_ids[id_index+2];
  }  
}

/*
 * server_acl_resolve - 
 *
 * resolver function for the new DCE 1.1 ACL library. 
 * see manpages for the dce_acl_resolve_by_{name,uuid}
 * for the interface to this function 
 * 
 * In our simple case, using the common ACL manager for only the server 
 * management ACL, we have only one possible component.  Since we used
 * our management uuid to identify this acl, just return that to the caller.
 * All this provided the caller is bright enough not to ask for an 
 * object default or container default acl type... we don't support those
 * for server management.
 */
static void server_acl_resolve (handle_t handle,		/* [in] parameters  */
			       sec_acl_component_name_t component_name,
			       sec_acl_type_t sec_acl_type,
			       uuid_t * mgr_type_p,
			       boolean32 writing,
			       void * resolver_arg_p,
			       uuid_t * acl_uuid_p,	/* [out] parameters */
			       error_status_t * stp )
{
    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_server, svc_c_debug2,
		   " >server_acl_resolve(,,,,,,,%ld)",
		   *stp));

    if ( sec_acl_type == sec_acl_type_object ) {
       *acl_uuid_p = server_mgmt_dacl_uuid ;
       *stp = error_status_ok ;
    }
    else {
       *stp = sec_acl_invalid_acl_type;
       DCE_SVC_DEBUG((cds__svc_handle, cds_svc_server, svc_c_debug2,
		      "Call to server_acl_resolve with non-object ACL type",
		      *stp));
    }

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_server, svc_c_debug2,
		   " <server_acl_resolve(,,,,,,,%ld)",
		   *stp));
    return ;
}

/*
 * Generate a default ACL for server management consisting
 * of:
 *   an entry containing all permissions for the machine principal,
 *   an entry containing all permissions for the cds-admin group,
 *   an entry containing read permission for 'any_other' 
 *   an entry containing read permission for unauthenticated users.
 *
 * Returns error_status_ok on success; other on failure
*/
static int 
  create_def_server_mgmt_acl (sec_acl_t *acl_p)
{
  sec_acl_entry_t      *sec_p;
  error_status_t 	status;
  boolean32 		orig_sec_nsi_state; 	/* original nsi state for   */
                                                /* security bindings  	    */
                                                /* (formally 'BIND_PE_SITE')*/ 
  sec_acl_permset_t   permset = (sec_acl_permset_t) 0;
  

  DCE_SVC_DEBUG((cds__svc_handle, cds_svc_server, svc_c_debug8,
		 " >create_def_server_mgmt_acl"));

  /* Get the Cell and host names and their UUID.
   * This requires that we be able to talk to the registry server,
   * which means that we must have a valid login context.
   * Also, disable the use of nsi binding by the security code while
   * we do this because we don't want it to try to make cds api calls
   * here.
   */
  orig_sec_nsi_state = sec_rgy_enable_nsi (0); /* disable */

  get_cell_and_host_info(&status);

  sec_rgy_enable_nsi(orig_sec_nsi_state);     /* restore regardless of status*/

  if (status != error_status_ok) 
  {
      dce_svc_printf(CDS_S_ACLFAIL_MSG, (long)status);
      return(status);
  }

  /*
   * Create this ACL for server object in the acl database
   */
  dce_acl_obj_init( &server_mgmt_dacl_uuid, acl_p, &status);
  if (status != error_status_ok) {
     dce_svc_printf(CDS_S_BAD_ACL_INIT_OBJ_MSG, (long)status);
     return(status);
  }

  /*
   * Get local realm and add it here 
   */

  /*
   * 1st entry = "rwc" access for machine principal 
   */
  permset = (sec_acl_perm_control | sec_acl_perm_read | sec_acl_perm_write);
  dce_acl_obj_add_user_entry (acl_p, permset, &HostPrincipal_uuid, &status);
  if (status != error_status_ok) {
     dce_svc_printf(CDS_S_BAD_ADD_USER_ACL_MSG, (long)status);
     return (status);
  }

  /*
   * 2nd entry = "rwc" access for cds-admin group    
   */
  permset = (sec_acl_perm_control | sec_acl_perm_read | sec_acl_perm_write);
  dce_acl_obj_add_group_entry( acl_p, permset, &AdminGroup_uuid, &status);
  if (status != error_status_ok) {
     dce_svc_printf(CDS_S_BAD_ADD_GROUP_ACL_MSG, (long)status);
     return (status);
  }
  
  /*
   * 3rd entry = "r" access to any other principals 
   */
  permset = sec_acl_perm_read ;
  dce_acl_obj_add_any_other_entry( acl_p, permset, &status);
  if (status != error_status_ok) {
     dce_svc_printf(CDS_S_BAD_ADD_ANY_ACL_MSG, (long)status);
     return (status);
  }
  
  /*
   * 4th entry = "r" access for the unauthenticated users 
   */
  permset = sec_acl_perm_read ;
  dce_acl_obj_add_unauth_entry (acl_p, permset, &status);
  if (status != error_status_ok) {
     dce_svc_printf(CDS_S_BAD_ADD_UNAUTH_ACL_MSG, (long)status);
     return (status);
  }
  
  DCE_SVC_DEBUG((cds__svc_handle, cds_svc_server, svc_c_debug8,
		 " <create_def_server_mgmt_acl"));

  return(error_status_ok);
}

/*
 * Get cell name, host principal, cds-admin group name from
 * configuration file(s).  Let values in cds.conf override any
 * defaults. Also lookup their UUIDS. (makes call to registry server, 
 * using unathenticated requests)
 */
#define NULL_STRING "\0"

static void
  get_cell_and_host_info (error_status_t *st_p)
{
  char *cellname, *hostprinc;
  unsigned_char_p_t uuid_string;
  sec_rgy_handle_t rgy_context; 
  sec_rgy_bind_auth_info_t auth_info;
  unsigned32 status;
  char *cfg_h, *p;
  error_status_t st;

  /*
   * Get the cell name from the dce config file 
   */
  dce_cf_get_cell_name (&cellname, st_p);
  if (*st_p == dce_cf_st_ok) {
    strcpy (Server_Realm, cellname);
    free(cellname);
  }

  /*
   * Get the cell uuid 
   */

  /* Open a connection with the local registry site.
   * 
   * Bind to it without authentication, since the advertiser
   * does not have a login context.
   * NOTE: this will change. Adver will run as machine principal, so that
   * its identity will be refreshed automatically by the sec_clientd.
   */
  auth_info.info_type = sec_rgy_bind_auth_none;
  sec_rgy_site_bind ((unsigned char *)NULL_STRING, &auth_info, &rgy_context, st_p);
  if (*st_p != error_status_ok) 
  {
      dce_svc_printf(CDS_S_SECRGYSITEBIND_MSG, (long)*st_p);
      return;
  }
  sec_id_parse_name(rgy_context, (unsigned char *)Server_Realm,
		      NULL, &Server_Realm_uuid, NULL, NULL, st_p);
  if (*st_p != error_status_ok) 
  {
     dce_svc_printf(CDS_S_SECIDPARSENAME_MSG, (long)*st_p); 
     return;
  }

  /*
   * Get host principal name (check cds.conf first, then dce_cf.db) 
   */
  cfg_h = dns_cfgopen(NULL, NULL);
  if ((p = dns_cfgtostr(cfg_h, 
			"security.host_princ_name",
			NULL)) == NULL) {
    dce_cf_prin_name_from_host (NULL,&hostprinc,&st);
    if (st == dce_cf_st_ok) {
      strcpy(HostPrincipal, hostprinc);
      free(hostprinc);
    }
  }
  else strcpy (HostPrincipal, p);

  DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug8,
		 " Host principal: %s", HostPrincipal));

  /*
   * Get the host uuid 
   */
  sec_rgy_pgo_name_to_id (rgy_context, 
			  sec_rgy_domain_person,
			  (unsigned char *)HostPrincipal,
			  &HostPrincipal_uuid,
			  st_p);
  
  if (*st_p != rpc_s_ok) {
    dce_svc_printf(CDS_S_HOSTFAIL_MSG, (long)*st_p);
    return;
  }


  /*
   * Check cds.conf for cds-admin group name 
   */
  if ((p = dns_cfgtostr(cfg_h, 
			"security.admin_group_name",
			NULL)) == NULL) {
     /*
      * TEMPORARY: hard-wired admin group name 
      */
     strcpy (AdminGroup, "subsys/dce/cds-admin");
  }
  else strcpy (AdminGroup, p);

  /*
   * Get the admin group uuid 
   */
  sec_rgy_pgo_name_to_id (rgy_context, 
			  sec_rgy_domain_group,
			  (unsigned char *)AdminGroup,
			  &AdminGroup_uuid,
			  st_p);
  if (*st_p != rpc_s_ok) {
     dce_svc_printf(CDS_S_GROUPFAIL_MSG, (long)*st_p);
     return;
  }

  dns_cfgclose (cfg_h);
  sec_rgy_site_close(rgy_context, st_p);

  if (DCE_SVC_DEBUG_ATLEAST(cds__svc_handle, cds_svc_adver, svc_c_debug8)) {
    uuid_to_string (&Server_Realm_uuid, &uuid_string, st_p);
    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug7,
		   " Cell name: %s, uuid: %s",
		   Server_Realm, uuid_string));
    rpc_string_free(&uuid_string, &status);
    uuid_to_string (&HostPrincipal_uuid, &uuid_string, st_p);
    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug8,
		   " Host principal: %s, uuid: %s",
		   HostPrincipal,  uuid_string));
    rpc_string_free(&uuid_string, &status);
    uuid_to_string (&AdminGroup_uuid, &uuid_string, st_p);
    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug8,
		   " Admin group: %s, uuid: %s",
		   AdminGroup, uuid_string));
    rpc_string_free(&uuid_string, &status);
  }
}


/*
 * Update a directory's DEFOBJECTACL manager type.
 * Assumes update_p has already been allocated by caller. 
 */
dns_status_t 
update_dir_defobjacl_mgr_type (db_data_t *db_dir_p,
			       Update_t  *update_p)
{
    sec_acl_t    *defacl_p;
    byte_t       *flat_acl_p;
    int           len;
    dns_status_t  status = CDS_SUCCESS;

    if (dacl_get(db_dir_p, &defacl_p, DNS_DEFOBJECTACL) ==
	DNS_SUCCESS) {
	defacl_p->sec_acl_manager_type = cat.uuid;

	/* convert acl to flat format for storage */
	if (dpe_acl_wire_to_flat(defacl_p,
				 DPE_ACL_STDMEM,
				 (char **)&flat_acl_p,
				 &len) == dpe_acl_failure) {
	    DCE_SVC_DEBUG((
			   cds__svc_handle,
			   cds_svc_server,
			   svc_c_debug1,
			   " E update_dir_defobjacl_mgr_type: NONSRESOURCES"));
	    return(DNS_NONSRESOURCES);
	}
	else {
	    /* Build update buffer */
	    (void)update_dns_build(update_p,
				   UD_present, 
				   NULL, 
				   dns_Attribute(DNS_DEFOBJECTACL),
				   BL_true,
				   len,
				   flat_acl_p);
	    free((char *)flat_acl_p);
	    status = set_replace_value(db_dir_p,
				       update_p,
				       dns_Attribute(DNS_DEFOBJECTACL));
	    if (status != CDS_SUCCESS) {
		DCE_SVC_DEBUG((
		    cds__svc_handle,
		    cds_svc_server,
		    svc_c_debug1,
		    " E update_dir_defobjacl_mgr_type: set_replace_value: %d",
		    status));
	    }
	}
    }
    return(DNS_SUCCESS);
}

#ifdef DCE_CDS_DEBUG

static void 
print_acl (sec_acl_p_t acl_p)
{
  int i;
  char *name, *name_uuid;
  unsigned_char_t *uuid_string;
  error_status_t st;
  sec_acl_entry_t *sec_p;

  if (!(DCE_SVC_DEBUG_ATLEAST(cds__svc_handle, cds_svc_server, svc_c_debug5)))
      return;
  if (acl_p == NULL) return;
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug5,
      "   ACL:"));
  uuid_to_string (&acl_p->default_realm.uuid, &uuid_string, &st);
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug5,
      "   Realm: %s, <%s>",
      acl_p->default_realm.name,
      uuid_string));
  rpc_string_free (&uuid_string, &st);
  for (i=0;i<acl_p->num_entries;i++) {
    sec_p = &acl_p->sec_acl_entries[i];
    if (sec_p->entry_info.entry_type == sec_acl_e_type_user ||
	sec_p->entry_info.entry_type == sec_acl_e_type_group) {
      uuid_to_string (&sec_p->entry_info.tagged_union.id.uuid, &uuid_string, &st);
      name = (char *)sec_p->entry_info.tagged_union.id.name;
      name_uuid = (char *)uuid_string;
    }
    else {
      name = name_uuid = "n/a";
      uuid_string = NULL;
    }
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        "   Name: %s, <%s>",
        name,
        name_uuid));

    if (uuid_string) rpc_string_free (&uuid_string, &st);
  }

}

/**********************************************************************/
/* Display info about the accessor described in the rpc handle.
*/
static void 
print_accessor_info (handle_t h)
{
  sec_id_pac_t pac;
  error_status_t st;

  if (!(DCE_SVC_DEBUG_ATLEAST(cds__svc_handle, cds_svc_server, svc_c_debug5)))
      return;

  st = server_get_client_creds (h,NULL,&pac,NULL); /* get client's PAC */
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug5,
      "   Accessor's PAC:"));
  view_pac (&pac);  /* display it */

}

static char 
*deb_ascii_handle (char      *str_1,
                   handle_t  arg)
{
    if (arg) {
      (void)sprintf(str_1, "%p", arg);
      return(str_1);
    } else
	return("NULL handle_t");
}

static char 
*deb_ascii_sec_acl_permset (char               *str_1,
                            sec_acl_permset_t  arg)
{
    (void)sprintf(str_1, "0x%X", arg);
    return(str_1);
}

static char 
*deb_ascii_sec_acl_type (char            *str_1,
                         sec_acl_type_t  arg)
{
    switch (arg) {
    case sec_acl_type_object:
	return("sec_acl_type_object");
    case sec_acl_type_default_object:
	return("sec_acl_type_default_object");
    case sec_acl_type_default_container:
	return("sec_acl_type_default_container");
    default:
	(void)sprintf(str_1, "unknown(%d)", arg);
	return(str_1);
    }
}

#endif  /* #ifdef DCE_CDS_DEBUG */

#endif  /* #ifdef DCE_SEC  right at top of file, just below copyright */
