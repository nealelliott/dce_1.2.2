/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: security.h,v $
 * Revision 1.1.6.2  1996/02/18  23:34:19  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:23:20  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:17:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:43:40  root]
 * 
 * Revision 1.1.4.4  1994/09/07  20:02:05  mccann
 * 	fix version handling OT:12004
 * 	[1994/09/07  18:41:13  mccann]
 * 
 * Revision 1.1.4.3  1994/08/25  19:30:43  proulx
 * 	Delegation support
 * 	[1994/08/25  19:14:47  proulx]
 * 
 * Revision 1.1.4.1  1994/03/12  22:04:07  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:19:00  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:33:55  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:53:34  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:15:58  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _SECURITY_H
#define _SECURITY_H
/*
 * Module security.h
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
 * MODIFICATIONS:
 *
 * Fri Mar  8 16:47:46 1991, by Tamar R. Wexler
 *	Shortened the names of security_diag_set_node_verification and
 * 	security_diag_read_node_verif to less than 31 characters so they
 *	would compile on vms.
 *
 * Gen 23, 15-May-1992, Alan Peckham
 *	Missing prototypes: server_mgmt_acl_init().
 *	Use _DNS_PROTOTYPE_() instead of _().
 *	Use dns_status_t instead of unsigned int where status values are used.
 *
 */

#include <dce/dns_record.h>

#ifdef DCE_SEC
# include <dce/rpc.h>
# include <dce/aclbase.h>
# define sec_acl_perm_admin sec_acl_perm_unused_00000080
#endif

/*
 * security_lib.c
 */
/*
 * The type of operation being performed .  This is used to determine what
 * access is required for both CDS and DNS
 */
typedef enum {
      Modify_Del_check,			/* Delete an attribute */
      Modify_check,			/* Modify attribute */
      Modify_Rptype_check,              /* Change replica type */
      Super_check,			/* Used for owner operations */
      Create_Entry_check,		/* Create an object, link or child */
      Create_Dir_check,			/* Create a directory */
      Delete_Entry_check,		/* Delete an object,link child,cle */
      Delete_Dir_check,			/* Delete a directory */
      Enum_Att_check,			/* Enumerate attributes */
      Enum_Entry_check,                 /* Enumerate object, link and child */
      Skulk_check,                      /* Skulk */
      Resolve_check,                    /* Resolve a link */
      Test_check,                       /* Test an attribute value */
      Read_check,                       /* Read and attribute */
      Replica_Add_check,                /* add replica */
      Replica_Remove_check              /* remove Replica */
} SecurityOpCodes;

void
security_log (
    struct user_descriptor *,
    FullName_t		   *,
    struct nsgbl_ch	   *);

#if DCE_SEC

# define ANYPerms  1			/* Test for one permission set */
# define ALLPerms  2			/* Test that all permissions are set */

dns_status_t
security_any (
    user_descriptor_t      *,
    struct db_data         *);

dns_status_t
security_dacl (
    user_descriptor_t      *,
    struct db_data	   *,
    sec_acl_permset_t      ,
    int                    );

boolean32
security_is_locksmith (
    sec_id_pac_t *,
    nsgbl_ns_t *
    );

#endif

dns_status_t
security_check (
    user_descriptor_t      *,
    db_stream_t            *,
    SecurityOpCodes        ,
    int                    ,
    ObjUID_t               *);

dns_status_t
security_diag_set_accden_evl (
    diag_state_t	,
    diag_state_t	*);

int
security_diag_set_override (
    int			);

int
security_diag_read_override (
    void);

int
security_diag_set_node_verif (
    int			);

int
security_diag_read_node_verif (
    void);

#ifdef DCE_SEC

/* cds_dacl.c */

dns_status_t 
update_dir_defobjacl_mgr_type (db_data_t *,
			       Update_t  *);

dns_status_t
dacl_get (
     struct db_data  *,
     sec_acl_p_t     *,
     int             );

void 
dacl_add_acl_entry (
     sec_acl_entry_t *,
     sec_acl_t       *,
     int             );

sec_acl_permset_t
dacl_manager_permset (
     int    );

uuid_t *
dacl_manager_type (
     int    );

dns_status_t
dacl_test_access (
     user_descriptor_t *,
     FullName_t        *,
     sec_acl_permset_t ,
     sec_acl_permset_t );

dns_status_t 
dacl_lookup (
     FullName_t     *, 
     byte_t         **, 
     int            *,
     sec_acl_type_t );

int
server_mgmt_acl_init (
    void);

/* static create_def_server_mgmt_acl */

/*
 * dns_server_rpc.c
 */
int 
TestServerUserAccess (
    handle_t           ,
    sec_acl_permset_t   );

#endif

#endif  /* #ifndef _SECURITY_H */
