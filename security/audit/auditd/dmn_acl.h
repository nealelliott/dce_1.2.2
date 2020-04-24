/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * (c) Copyright International Business Machines Corp. 1994, 1996
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dmn_acl.h,v $
 * Revision 1.1.6.1  1996/07/08  18:24:45  arvind
 * 	Audit for 1.2.2
 * 	[1996/06/21  20:23 UTC  arvind  /main/arvind_audit122/1]
 *
 * 	Audit for 1.2.2
 * 	[1996/02/18  22:57:29  marty  1.1.4.2]
 *
 * Revision 1.1.4.2  1996/02/18  22:57:29  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:16:22  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  17:38:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:17:30  root]
 * 
 * Revision 1.1.2.1  1994/07/07  19:46:02  rsalz
 * 	Created from dce/utils/svc/svcacl.h (part of OT CR 10856).
 * 	[1994/07/07  19:04:50  rsalz]
 * 
 * $EndLog$
 */
#ifndef _DMN_ACL_H
#define _DMN_ACL_H

#define dce_svc_acl_c_no_permissions	((sec_acl_permset_t)0)

extern void dce_svc_acl_client_get_pac(
    handle_t			/* h */,
    sec_id_pac_t**		/* pacp */,
    error_status_t*		/* st */
);

extern boolean32 dce_svc_acl_client_is_authorized(
    sec_acl_p_t			/* ap */,
    handle_t			/* h */,
    sec_acl_permset_t 		/* wanted */,
    error_status_t*		/* st */
);

extern void dce_svc_acl_client_get_access(
    sec_acl_p_t			/* ap */,
    handle_t			/* h */,
    sec_acl_permset_t* 		/* perms */,
    error_status_t*		/* st */
);


/*
**  The following routines implement the server side of the rdacl interface.
*/

extern void dce_svc_acl_get_manager_types(
    uuid_t			/* my_manager_type */,
    unsigned32			/* size_avail */,
    unsigned32*			/* size_used */,
    unsigned32*			/* num_types */,
    uuid_t*			/* manager_types */,
    error_status_t*		/* st */
);


/*
**  The first entry in each table must contain the the acl manager print
**  and help strings (this will have a zero permission entry).  Following
**  that must be one entry for each permission supported for this
**  interface.
*/
extern void dce_svc_acl_get_printstring(
    sec_acl_printstring_t* 	/* idp */,
    unsigned32			/* idp_size */,
    unsigned32			/* size_avail */,
    uuid_t*			/* manager_type_chain */,
    sec_acl_printstring_t* 	/* manager_info */,
    unsigned32*			/* total_num_printstrings */,
    unsigned32*			/* size_used */,
    sec_acl_printstring_t*	/* psp */,
    error_status_t*		/* st */
);

extern void dce_svc_acl_lookup(
    sec_acl_p_t			/* ap */,
    handle_t			/* h */,
    sec_acl_result_t*		/* result */,
    error_status_t*		/* st */
);

extern void dce_svc_acl_replace(
    char*			/* name */,
    handle_t			/* h */,
    sec_acl_list_t*		/* newlist */,
    sec_acl_p_t*		/* ap */,
    error_status_t*		/* st */
);

extern boolean32 dce_svc_acl_test_access_on_behalf(
    sec_acl_p_t			/* ap */,
    handle_t			/* h */,
    sec_id_pac_t*		/* subject */,
    sec_acl_permset_t 		/* wanted */,
    error_status_t*		/* st */
);

extern void dce_svc_acl_storage_read(
    char*			/* file */,
    sec_acl_p_t*		/* ap */,
    error_status_t*		/* st */
);

extern void dce_svc_acl_storage_write(
    char*			/* file */,
    sec_acl_p_t			/* ap */,
    error_status_t*		/* st */
);

extern void dce_svc_acl_free_acl(
    sec_acl_p_t			/* ap */
);

extern boolean32 dce_svc_acl_mgr_is_authorized(
    sec_acl_t*			/* ap */,
    sec_acl_permset_t		/* wanted */,
    sec_id_pac_t*		/* pac */,
    error_status_t*		/* st */
);

extern boolean32 dce_svc_acl_mgr_get_access(
    sec_acl_t*			/* ap */,
    sec_id_pac_t*		/* pac */,
    sec_acl_permset_t*		/* perms */,
    error_status_t*		/* st */
);
#endif	/* _DMN_ACL_H */
