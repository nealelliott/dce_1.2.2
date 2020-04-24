/*
 * @OSF_COPYRIGHT@
 * Copyright (c) 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: acldefs.h,v $
 * Revision 1.1.2.2  1996/02/18  23:32:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:15  marty]
 *
 * Revision 1.1.2.1  1995/12/08  20:53:04  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/09/29  22:37 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/1  1995/09/19  15:30 UTC  tatsu_s
 * 	Initial revision: dced splitting (libdcedpvt).
 * 
 * 	HP revision /main/HPDCE02/1  1995/07/27  20:55 UTC  tatsu_s
 * 	Submitted the fix for CHFts15852.
 * 
 * 	HP revision /main/tatsu_s.local_rpc.b5/1  1995/07/26  17:18 UTC  tatsu_s
 * 	Added perm. sets.
 * 	Added patch_other_acl().
 * 	[1995/12/08  20:48:47  root]
 * 
 * Revision 1.1.2.3  1994/03/25  18:49:46  bowe
 * 	Added uuid_t's for config_type
 * 	[1994/03/25  18:40:06  bowe]
 * 
 * Revision 1.1.2.2  1994/03/18  19:57:16  rsalz
 * 	Put permset bits in dced_aclbits.idl (OT CR 9768)
 * 	[1994/03/18  19:56:08  rsalz]
 * 
 * Revision 1.1.2.1  1994/03/09  14:25:38  bowe
 * 	Initial checkin
 * 	[1994/03/09  14:24:12  bowe]
 * 
 * $EndLog$
 */

#ifndef _ACLDEFS_H
#define _ACLDEFS_H

/* For convenience - to hold a list of all ALC types we managers */
typedef struct mgr_list_s_t {
    char			*name;
    int				name_len;
    sec_acl_printstring_t	*info;
    sec_acl_printstring_t	*printstr;
    uuid_t			*obj_type;
    uuid_t			*cont_type;
    uuid_t			*acl;
    uuid_t			*do_acl;
    sec_acl_permset_t		self_perms;
    sec_acl_permset_t		admin_perms;
    sec_acl_permset_t		other_perms;
    sec_acl_permset_t		do_self_perms;
    sec_acl_permset_t		do_admin_perms;
    sec_acl_permset_t		do_other_perms;
} manager_list_t;


/* manager type UUIDs */
extern uuid_t dced_config_type;
extern uuid_t hd_obj_type, hd_cont_type;
extern uuid_t km_obj_type, km_cont_type;
extern uuid_t sc_obj_type, sc_cont_type;
extern uuid_t sx_obj_type, sx_cont_type;
extern uuid_t dced_sv_obj_type;
extern uuid_t as_obj_type, as_cont_type;

/* object and default object ACL UUIDs */
extern uuid_t dced_config_acl;
extern uuid_t hd_acl, hd_do_acl;
extern uuid_t km_acl, km_do_acl;
extern uuid_t sc_acl, sc_do_acl;
extern uuid_t sx_acl, sx_do_acl;
extern uuid_t dced_sv_acl;
extern uuid_t as_acl, as_do_acl;

void	init_acl_database(error_status_t *);

#endif
