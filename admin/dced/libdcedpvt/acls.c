/* ______________________________________________________________________ */
/*
 * @OSF_COPYRIGHT@
 * Copyright (c) 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: acls.c,v $
 * Revision 1.1.4.2  1996/11/13  17:55:31  arvind
 * 	fix dced acl code returns wrong error on unauthorized
 * 	[1996/10/09  19:39 UTC  bartf  /main/DCE_1.2.2/bartf_pk_fix/1]
 *
 * 	fix incorrect invalid_permission error to be not_authorized
 * 	[1996/07/09  19:44 UTC  jrr  /main/DCE_1.2.2/1]
 *
 * Revision 1.1.4.1  1996/08/09  11:53:41  arvind
 * 	Fix memory leaks (OT13533).
 * 	[1996/07/09  19:40 UTC  jrr  /main/HPDCE02/jrr_perf_2/1]
 * 
 * Revision 1.1.2.2  1996/02/18  19:16:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:02:00  marty]
 * 
 * Revision 1.1.2.1  1995/12/08  20:53:05  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/5  1995/09/29  22:37 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/3  1995/09/23  20:20 UTC  tatsu_s
 * 	Don't overwrite the status.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/2  1995/09/23  17:49 UTC  tatsu_s
 * 	Added anonymous hostdata directory.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/1  1995/09/19  15:30 UTC  tatsu_s
 * 	Initial revision: dced splitting (libdcedpvt).
 * 
 * 	HP revision /main/HPDCE02/4  1995/08/29  22:26 UTC  tatsu_s
 * 	Submitted yet another (probably not last) fix for CHFts15852/OT12494.
 * 	Submitted the fix for CHFts16056.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.psock_timeout.b0/1  1995/08/23  20:58 UTC  tatsu_s
 * 	Tightened up ACLs.
 * 
 * 	HP revision /main/HPDCE02/3  1995/07/27  20:55 UTC  tatsu_s
 * 	Submitted the fix for CHFts15852.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.local_rpc.b5/1  1995/07/26  17:21 UTC  tatsu_s
 * 	Added perm. sets.
 * 	Added add_admin_group_acl() and patch_other_acl().
 * 
 * 	HP revision /main/HPDCE02/2  1995/05/31  13:25 UTC  tatsu_s
 * 	Submitted the fix for OT12494/CHFts13959 .
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_security.b0/5  1995/05/15  19:59 UTC  tatsu_s
 * 	Removed extra statements.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_security.b0/4  1995/05/13  03:38 UTC  tatsu_s
 * 	Added any_other in the default ALC.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_security.b0/3  1995/05/12  20:34 UTC  tatsu_s
 * 	Fixed a potential null pointer dereference and memory leak.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_security.b0/2  1995/05/11  21:32 UTC  tatsu_s
 * 	Closed a small security hole in bootstrap phase.
 * 	Added subsys/dce/dced-admin and a guessed cell admin in the default ACL.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_security.b0/1  1995/05/09  18:57 UTC  tatsu_s
 * 	Fixed the initial ACLs in acl_patcher().
 * 
 * 	HP revision /main/HPDCE02/1  1995/01/04  18:13 UTC  tatsu_s
 * 	Submitted dced shutdown fix.
 * 
 * 	HP revision /main/tatsu_s.mothra_cn_fix.b0/1  1995/01/04  16:34 UTC  tatsu_s
 * 	Fixed the shutdown.
 * 	[1995/12/08  20:48:48  root]
 * 
 * Revision 1.1.2.28  1994/10/06  18:11:56  rsalz
 * 	Undo the acl_patcher modification (OT CR 12536) for now.
 * 	[1994/10/06  17:02:01  rsalz]
 * 
 * Revision 1.1.2.27  1994/10/05  19:57:52  rsalz
 * 	acl_patches modifies any_other to machine_principal (OT CR 12494).
 * 	[1994/10/05  18:40:21  rsalz]
 * 
 * Revision 1.1.2.26  1994/09/20  21:25:54  bowe
 * 	In resolver, correct typo in sx name cache lookup. [CR 12272]
 * 	[1994/09/20  21:25:27  bowe]
 * 
 * Revision 1.1.2.25  1994/09/17  08:05:07  rsalz
 * 	Fix I bit in permstring and final SVC work (OT CR 11725).
 * 	[1994/09/17  07:56:22  rsalz]
 * 
 * Revision 1.1.2.24  1994/09/14  21:12:41  bowe
 * 	SVC work  [CR 11725]
 * 	[1994/09/14  21:09:06  bowe]
 * 
 * Revision 1.1.2.23  1994/09/12  15:37:51  rsalz
 * 	Add insert_priv ACL bit (part of OT CR 12032).
 * 	[1994/09/09  18:43:29  rsalz]
 * 
 * Revision 1.1.2.22  1994/08/26  20:15:11  bowe
 * 	Add acl bits for remote svc control  [CR 11725]
 * 	[1994/08/26  20:09:09  bowe]
 * 
 * Revision 1.1.2.21  1994/08/24  16:40:56  baum
 * 	Full dced service on bootup.  Secval on by default (OT 11726 & 11656)
 * 	[1994/08/24  16:34:56  baum]
 * 
 * Revision 1.1.2.20  1994/07/29  19:56:02  rsalz
 * 	Use official ACL manager names (OT CR 11306).
 * 	[1994/07/29  19:36:15  rsalz]
 * 
 * Revision 1.1.2.19  1994/07/25  19:10:17  rsalz
 * 	Change to new schema name (OT CR 11427).
 * 	[1994/07/25  19:08:14  rsalz]
 * 
 * Revision 1.1.2.18  1994/07/08  17:22:55  rsalz
 * 	Add an unauthenticated entry to all "-i" ACL's (OT CR 11214).
 * 	[1994/07/08  17:22:37  rsalz]
 * 
 * Revision 1.1.2.17  1994/07/06  19:24:31  rsalz
 * 	Make acl_patcher more robust, always call export (OT CR 11171).
 * 	[1994/07/06  19:22:58  rsalz]
 * 
 * Revision 1.1.2.16  1994/06/22  20:27:56  rsalz
 * 	Add "export into namespace" into acl_patcher.
 * 	[1994/06/22  20:14:12  rsalz]
 * 
 * Revision 1.1.2.15  1994/06/22  18:43:27  baum
 * 	Fixed acl_patcher() to not do store() in iterator loop
 * 	[1994/06/22  18:43:09  baum]
 * 
 * Revision 1.1.2.14  1994/06/06  19:54:17  rsalz
 * 	Finish ACL patcher (OT CR 10858).
 * 	[1994/06/06  19:52:20  rsalz]
 * 
 * Revision 1.1.2.13  1994/06/03  19:17:09  rsalz
 * 	Add "acl patcher" (OT CR 10858).
 * 	[1994/06/03  19:10:34  rsalz]
 * 
 * Revision 1.1.2.12  1994/05/27  16:31:10  rsalz
 * 	Fix bootstrap phase; cleanup EP and LLB compat code
 * 	[1994/05/27  16:30:18  rsalz]
 * 
 * Revision 1.1.2.11  1994/05/26  19:42:05  rsalz
 * 	EP functionality.
 * 	[1994/05/26  19:35:34  rsalz]
 * 
 * Revision 1.1.2.10  1994/05/10  20:15:06  rsalz
 * 	Cleanup and more functionality
 * 	[1994/05/10  20:10:34  rsalz]
 * 
 * Revision 1.1.2.9  1994/05/06  15:19:44  bowe
 * 	Rename some dce_acl_obj_xxx calls [cr 10556]
 * 	[1994/05/06  15:16:06  bowe]
 * 
 * Revision 1.1.2.8  1994/05/05  14:41:15  pwang
 * 	Rename _rkeytab_ to _keytab_.
 * 	[1994/05/05  14:37:10  pwang]
 * 
 * Revision 1.1.2.7  1994/04/26  19:25:54  bowe
 * 	New param to dce_acl_register_object_type()
 * 	[1994/04/26  19:13:52  bowe]
 * 
 * Revision 1.1.2.6  1994/04/06  21:17:17  bowe
 * 	Use global thishost_prin_uuid instead of local uuid
 * 	[1994/04/06  21:15:57  bowe]
 * 
 * Revision 1.1.2.5  1994/04/06  15:12:12  bowe
 * 	Added ACL name resolver for attr_schema. Fix ACL bits for srvrexec.
 * 	Misc cleanup.
 * 	[1994/04/06  15:11:32  bowe]
 * 
 * Revision 1.1.2.4  1994/03/29  22:31:31  bowe
 * 	Use new name->uuid cache functions to find UUIDs in resolver
 * 	functions.  Remove casts of pointers to resolver functions when calling
 * 	dce_acl_register_object_type().  Defined resolver functions correctly.
 * 	Free ACL [dce_acl_obj_free()] after writing it to ACL backing store.
 * 	[1994/03/29  22:30:25  bowe]
 * 
 * Revision 1.1.2.3  1994/03/25  18:49:47  bowe
 * 	Added UUIDs and printstring for config_type.  Adjusted resolver to
 * 	return UUID for config object.  Added dced_server_auth().
 * 	[1994/03/25  18:41:51  bowe]
 * 
 * Revision 1.1.2.2  1994/03/18  19:51:31  bowe
 * 	Added dced_perm_insert to default ACLs for any_other
 * 	[1994/03/18  19:39:37  bowe]
 * 
 * Revision 1.1.2.1  1994/03/09  14:25:40  bowe
 * 	Initial checkin
 * 	[1994/03/09  14:24:14  bowe]
 * 
 * $EndLog$
 */
/* ______________________________________________________________________ */

#include <libimpl.h>
#include <hdimpl.h>
#include <dce/dceacl.h>
#include <dce/binding.h>
#include <dce/secidmap.h>

#define dced_c_acl_chunk_size	10


/* ______________________________________________________________________ */

/* ----  Well-known UUIDs  ---- */

uuid_t dced_config_type = { /* 0080fde1-644a-1d8f-89d2-0000c09ce054 */
  0x0080fde1, 0x644a, 0x1d8f, 0x89, 0xd2, {0x00, 0x00, 0xc0, 0x9c, 0xe0, 0x54}
};

/* Manager type UUIDs for server objects */

uuid_t hd_obj_type = { /* 00735bd6-c9b5-1d6c-8bb7-0000c09ce054 */
  0x00735bd6, 0xc9b5, 0x1d6c, 0x8b, 0xb7, {0x00, 0x00, 0xc0, 0x9c, 0xe0, 0x54}
};
uuid_t km_obj_type = { /* 0085aadf-c9b5-1d6c-97da-0000c09ce054 */
  0x0085aadf, 0xc9b5, 0x1d6c, 0x97, 0xda, {0x00, 0x00, 0xc0, 0x9c, 0xe0, 0x54}
};
uuid_t sc_obj_type = { /* 004d3731-c9b5-1d6c-9adb-0000c09ce054 */
  0x004d3731, 0xc9b5, 0x1d6c, 0x9a, 0xdb, {0x00, 0x00, 0xc0, 0x9c, 0xe0, 0x54}
};
uuid_t sx_obj_type = { /* 005f8639-c9b5-1d6c-95d2-0000c09ce054 */
  0x005f8639, 0xc9b5, 0x1d6c, 0x95, 0xd2, {0x00, 0x00, 0xc0, 0x9c, 0xe0, 0x54}
};
uuid_t dced_sv_obj_type = { /* 0000e9fc-c9b6-1d6c-8d33-0000c09ce054 */
  0x0000e9fc, 0xc9b6, 0x1d6c, 0x8d, 0x33, {0x00, 0x00, 0xc0, 0x9c, 0xe0, 0x54}
};
uuid_t as_obj_type = { /* 00133905-c9b6-1d6c-bdaf-0000c09ce054 */
  0x00133905, 0xc9b6, 0x1d6c, 0xbd, 0xaf, {0x00, 0x00, 0xc0, 0x9c, 0xe0, 0x54}
};


/* Pseudo-types for containers */

uuid_t hd_cont_type = { /* 00340bde-7374-1d72-b39d-0000c09ce054 */
  0x00340bde, 0x7374, 0x1d72, 0xb3, 0x9d, {0x00, 0x00, 0xc0, 0x9c, 0xe0, 0x54}
};
uuid_t km_cont_type = { /* 0007be12-7379-1d72-a395-0000c09ce054 */
  0x0007be12, 0x7379, 0x1d72, 0xa3, 0x95, {0x00, 0x00, 0xc0, 0x9c, 0xe0, 0x54}
};
uuid_t sc_cont_type = { /* 00728b16-7379-1d72-89ab-0000c09ce054 */
  0x00728b16, 0x7379, 0x1d72, 0x89, 0xab, {0x00, 0x00, 0xc0, 0x9c, 0xe0, 0x54}
};
uuid_t sx_cont_type = { /* 00357fbf-737a-1d72-a0f0-0000c09ce054 */
  0x00357fbf, 0x737a, 0x1d72, 0xa0, 0xf0, {0x00, 0x00, 0xc0, 0x9c, 0xe0, 0x54}
};
uuid_t as_cont_type = { /* 0013e804-737c-1d72-b5b4-0000c09ce054 */
  0x0013e804, 0x737c, 0x1d72, 0xb5, 0xb4, {0x00, 0x00, 0xc0, 0x9c, 0xe0, 0x54}
};


/* ACLs for objects, initial objects */

uuid_t dced_config_acl = { /* 00219b67-650d-1d8f-b795-0000c09ce054 */
  0x00219b67, 0x650d, 0x1d8f, 0xb7, 0x95, {0x00, 0x00, 0xc0, 0x9c, 0xe0, 0x54}
};

uuid_t hd_acl = { /* 0021e242-611a-1d6e-9ead-0000c09ce054 */
  0x0021e242, 0x611a, 0x1d6e, 0x9e, 0xad, {0x00, 0x00, 0xc0, 0x9c, 0xe0, 0x54}
};
uuid_t hd_do_acl = { /* 001f84df-e03a-1d6c-a6ce-0000c09ce054 */
  0x001f84df, 0xe03a, 0x1d6c, 0xa6, 0xce, {0x00, 0x00, 0xc0, 0x9c, 0xe0, 0x54}
};

uuid_t km_acl = { /* 0034314b-611a-1d6e-8e5e-0000c09ce054 */
  0x0034314b, 0x611a, 0x1d6e, 0x8e, 0x5e, {0x00, 0x00, 0xc0, 0x9c, 0xe0, 0x54}
};
uuid_t km_do_acl = { /* 0047301b-e03a-1d6c-a019-0000c09ce054 */
  0x0047301b, 0xe03a, 0x1d6c, 0xa0, 0x19, {0x00, 0x00, 0xc0, 0x9c, 0xe0, 0x54}
};

uuid_t sc_acl = { /* 0092cd87-6119-1d6e-a65e-0000c09ce054 */
  0x0092cd87, 0x6119, 0x1d6e, 0xa6, 0x5e, {0x00, 0x00, 0xc0, 0x9c, 0xe0, 0x54}
};
uuid_t sc_do_acl = { /* 006a4b7d-e039-1d6c-9c56-0000c09ce054 */
  0x006a4b7d, 0xe039, 0x1d6c, 0x9c, 0x56, {0x00, 0x00, 0xc0, 0x9c, 0xe0, 0x54}
};

uuid_t sx_acl = { /* 000e0ca4-611a-1d6e-983f-0000c09ce054 */
  0x000e0ca4, 0x611a, 0x1d6e, 0x98, 0x3f, {0x00, 0x00, 0xc0, 0x9c, 0xe0, 0x54}
};
uuid_t sx_do_acl = { /* 0091f6b8-e039-1d6c-8485-0000c09ce054 */
  0x0091f6b8, 0xe039, 0x1d6c, 0x84, 0x85, {0x00, 0x00, 0xc0, 0x9c, 0xe0, 0x54}
};

uuid_t dced_sv_acl = { /* 00498d7f-611a-1d6e-a093-0000c09ce054 */
  0x00498d7f, 0x611a, 0x1d6e, 0xa0, 0x93, {0x00, 0x00, 0xc0, 0x9c, 0xe0, 0x54}
};

uuid_t as_acl = { /* 005bdc87-611a-1d6e-85e3-0000c09ce054 */
  0x005bdc87, 0x611a, 0x1d6e, 0x85, 0xe3, {0x00, 0x00, 0xc0, 0x9c, 0xe0, 0x54}
};
uuid_t as_do_acl = { /* 00937967-e03a-1d6c-b8bb-0000c09ce054 */
  0x00937967, 0xe03a, 0x1d6c, 0xb8, 0xbb, {0x00, 0x00, 0xc0, 0x9c, 0xe0, 0x54}
};

/* ______________________________________________________________________ */

/* Printstrings and helpstrings for ACL management */

/* pseudo-containers for each manager - for who can create manager objects */
static sec_acl_printstring_t cont_info = {
    {"dced"},			{"DCED container ACLs"}
};
static sec_acl_printstring_t cont_prstr[] = {
    { {"c"},  {"control"},	dced_perm_control }, /* Modify the ACL */
    { {"r"},  {"read"},		dced_perm_read	  }, /* Read an item list */
    { {"i"},  {"insert"},	dced_perm_insert  },  /* Insert a new object */
    { {"I"},  {"insert_privileged"}, dced_perm_insertpriv  } /* Insert privileged object */
};

/* real objects for each manager */
static sec_acl_printstring_t hd_info = {
    {"hostdata"},		{"DCED host data object ACLs"}
};
static sec_acl_printstring_t hd_prstr[] = {
    { {"c"},  {"control"},	dced_perm_control }, /* Modify the ACL */
    { {"d"},  {"delete"},	dced_perm_delete  }, /* Delete item */
    { {"p"},  {"purge"},	dced_perm_purge	  }, /* Delete item & storage_tag */
    { {"r"},  {"read"},		dced_perm_read	  }, /* Read item */
    { {"w"},  {"write"},	dced_perm_write	  }  /* Modify item */
};

static sec_acl_printstring_t km_info = {
    {"keytab"},		{"DCED key management object ACLs"}
};
static sec_acl_printstring_t km_prstr[] = {
    { {"a"},  {"append"},	dced_perm_append  }, /* Add new item */
    { {"c"},  {"control"},	dced_perm_control }, /* Modify the ACL */
    { {"d"},  {"delete"},	dced_perm_delete  }, /* Delete item */
    { {"e"},  {"expunge"},	dced_perm_expunge }, /* Remove keys from item */
    { {"p"},  {"purge"},	dced_perm_purge   }, /* Delete item & storage_tag */
    { {"r"},  {"read"},		dced_perm_read	  }  /* Read keys in item */
};

static sec_acl_printstring_t sc_info = {
    {"srvrconf"},		{"DCED server configuration object ACLs"}
};
static sec_acl_printstring_t sc_prstr[] = {
    { {"c"},  {"control"},	dced_perm_control }, /* Modify the ACL */
    { {"d"},  {"delete"},	dced_perm_delete  }, /* Delete item */
    { {"f"},  {"flag"},		dced_perm_flag	  }, /* Start w/ custom flags */
    { {"r"},  {"read"},		dced_perm_read	  }, /* Read item */
    { {"w"},  {"write"},	dced_perm_write   }, /* Modify item's att's */
    { {"x"},  {"execute"},	dced_perm_execute }  /* Start server */
};

static sec_acl_printstring_t sx_info = {
    {"srvrexec"},		{"DCED server execution object ACLs"}
};
static sec_acl_printstring_t sx_prstr[] = {
    { {"c"},  {"control"},	dced_perm_control }, /* Modify ACL */
    { {"r"},  {"read"},		dced_perm_read	  }, /* Read item */
    { {"w"},  {"write"},	dced_perm_write	  }, /* Modify item's att's */
    { {"s"},  {"stop"},		dced_perm_stop	  }  /* Stop server */
};

/* secval a is pseudo-container only.  There are no objects. */
static sec_acl_printstring_t sv_info = {
    {"secval"},			{"security validation service"}
};
static sec_acl_printstring_t sv_prstr[] = {
    { {"c"},  {"control"},	dced_perm_control }, /* Modify ACL */
    { {"s"},  {"stop"},		dced_perm_stop	  }, /* Stop service */
    { {"u"},  {"update"},	dced_perm_update  }, /* Make ticket requests */
    { {"x"},  {"execute"},	dced_perm_execute }  /* Start service */
};


static sec_acl_printstring_t as_info = {
    {"xattrschema"},		{"DCED extended attribute schema object ACLs"}
};
static sec_acl_printstring_t as_prstr[] = {
    { {"c"},  {"control"},	dced_perm_control	}, /* Modify ACL */
    { {"r"},  {"read"},		dced_perm_read		}, /* Read item */
    { {"w"},  {"write"},	dced_perm_write		}, /* Modify item */
    { {"d"},  {"delete"},	dced_perm_delete	}  /* Delete item */
};

static sec_acl_printstring_t config_info = {
    {"dced"},			{"DCED server"}
};
static sec_acl_printstring_t config_prstr[] = {
    { {"c"},  {"control"},	dced_perm_control	}, /* Modify ACL */
    { {"r"},  {"read"},		dced_perm_read		}, /* Read svc info */
    { {"w"},  {"write"},	dced_perm_write		}, /* Modify svc info */
    { {"s"},  {"stop"},		dced_perm_stop		}  /* Stop DCED */
};

/* ______________________________________________________________________ */

const static manager_list_t managers[] = {
    { dced_c_service_hostdata, dced_c_service_hostdata_len, &hd_info, hd_prstr,
	&hd_obj_type, &hd_cont_type, &hd_acl, &hd_do_acl,
        dced_perm_read | dced_perm_insert,
        dced_perm_read | dced_perm_insert, dced_perm_read,
        dced_hd_all_perms, (dced_hd_all_perms & ~dced_perm_control),
        dced_perm_read
    },
    { dced_c_service_keytab,  dced_c_service_keytab_len,  &km_info, km_prstr,
	&km_obj_type, &km_cont_type, &km_acl, &km_do_acl,
        dced_cont_all_perms, (dced_cont_all_perms & ~dced_perm_control),
        dced_perm_read,
        dced_km_all_perms, (dced_km_all_perms & ~dced_perm_control), 0
    },
    { dced_c_service_srvrconf, dced_c_service_srvrconf_len, &sc_info, sc_prstr,
	&sc_obj_type, &sc_cont_type, &sc_acl, &sc_do_acl,
        dced_perm_read | dced_perm_insert,
        dced_perm_read | dced_perm_insert, dced_perm_read,
        (dced_sc_all_perms & ~(dced_perm_control | dced_perm_flag)),
        (dced_sc_all_perms & ~(dced_perm_control | dced_perm_flag)),
        dced_perm_read
    },
    { dced_c_service_srvrexec, dced_c_service_srvrexec_len, &sx_info, sx_prstr,
	&sx_obj_type, &sx_cont_type, &sx_acl, &sx_do_acl,
        dced_perm_read | dced_perm_insert,
        dced_perm_read | dced_perm_insert, dced_perm_read,
        dced_sx_all_perms, (dced_sx_all_perms & ~dced_perm_control),
        dced_perm_read
    },
    { dced_c_service_secval,   dced_c_service_secval_len,   &sv_info, sv_prstr,
	&dced_sv_obj_type, NULL,          NULL, &dced_sv_acl,
        0, 0, 0,
        dced_sv_all_perms, (dced_sv_all_perms & ~dced_perm_control),
        dced_perm_execute
    },
    { dced_c_service_attr_sch, dced_c_service_attr_sch_len, &as_info, as_prstr,
	&as_obj_type, &as_cont_type, &as_acl, &as_do_acl,
        dced_cont_all_perms, (dced_cont_all_perms & ~dced_perm_control),
        dced_perm_read,
        dced_as_all_perms, (dced_as_all_perms & ~dced_perm_control),
        dced_perm_read
     },
    { NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, 0, 0, 0, 0 }
};

dce_lock_t acl_lock;

/* ______________________________________________________________________ */
/* (Pseudo-)container resolver routine.
 */

void
dced_resolve_container(
    handle_t			h,
    sec_acl_component_name_t	name,
    sec_acl_type_t		sec_acl_type,
    uuid_t			*mgr_type,
    boolean32			writing,
    void			*resolver_arg,
    uuid_t			*acl_uuid,
    error_status_t		*st
)
{
    manager_list_t		*mp;

    if (sec_acl_type != sec_acl_type_object) {
	*st = sec_acl_invalid_acl_type;
	return;
    }

    if (writing && h != NULL
	&& (dced_is_bootstrap || dced_is_remote_disabled)) {
	*st = sec_acl_not_authorized;
	return;
    }

    *st = error_status_ok;
    for (mp = managers; mp->name; mp++) {
	if (strcmp((char *)name, mp->name) == 0
	 && mp->cont_type
	 && uuid_equal(mgr_type, mp->cont_type, st)
	 && mp->acl) {
	    *acl_uuid = *mp->acl;
	    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval, svc_c_debug1,
		"Resolver match (container): %s, type=%d", name,
		sec_acl_type));
	    return;
	}
    }

    /* try the config object for this DCED */
    if ((name == NULL || *name == '\0')
     && uuid_equal(mgr_type, &dced_config_type, st)) {
	*acl_uuid = dced_config_acl;
	return;
    }

    *st = sec_acl_no_acl_found;
}

/* ______________________________________________________________________ */
/* Object and default-object resolver routine.
 */

void
dced_find_entry_from_name(
    name_cache_t	cache,
    dce_db_handle_t	db,
    char		*name,
    dced_entry_data_t	*e,
    error_status_t	*st
)
{
    uuid_t		u;
    error_status_t	lst;

    if (!name_cache_find_uuid(cache, name, &u))
	*st = db_s_key_not_found;
    else {
	dce_lock_read(acl_lock, st);
	CHECK_STAT_RET(*st);
	dce_db_fetch_by_uuid(db, &u, e, st);
	dce_lock_unlock(acl_lock, &lst);
	CHECK_STAT_RET(*st);
    }
}

void
dced_find_server_from_name(
    name_cache_t	cache,
    dce_db_handle_t	db,
    char		*name,
    server_data_t	*s,
    error_status_t	*st
)
{
    uuid_t		u;
    error_status_t	lst;

    if (!name_cache_find_uuid(cache, name, &u))
	*st = db_s_key_not_found;
    else {
	dce_lock_read(acl_lock, st);
	CHECK_STAT_RET(*st);
	dce_db_fetch_by_uuid(db, &u, s, st);
	dce_lock_unlock(acl_lock, &lst);
	CHECK_STAT_RET(*st);
    }
}

void
dced_find_attr_schema_from_name(
    name_cache_t	cache,
    dce_db_handle_t	db,
    char		*name,
    attr_schema_data_t	*as,
    error_status_t	*st
)
{
    uuid_t		u;
    error_status_t	lst;

    if (!name_cache_find_uuid(cache, name, &u))
	*st = db_s_key_not_found;
    else {
	dce_lock_read(acl_lock, st);
	CHECK_STAT_RET(*st);
	dce_db_fetch_by_uuid(db, &u, as, st);
	dce_lock_unlock(acl_lock, &lst);
	CHECK_STAT_RET(*st);
    }
}

/* ______________________________________________________________________ */

void
dced_resolve_object(
    handle_t			h,
    sec_acl_component_name_t	name,
    sec_acl_type_t		sec_acl_type,
    uuid_t			*mgr_type,
    boolean32			writing,
    void			*resolver_arg,
    uuid_t			*acl_uuid,
    error_status_t		*st
)
{
    char			*o_name;
    manager_list_t		*mp;
    dced_entry_data_t		entry_d;
    server_data_t		server_d;
    attr_schema_data_t		schema_d;

    if (writing && h != NULL
	&& (dced_is_bootstrap || dced_is_remote_disabled)) {
	*st = sec_acl_not_authorized;
	return;
    }

    *st = error_status_ok;

    /* First, check if caller wants default object */
    /* secval is an object only. (not a pseudo-container of objects.) */
    if (sec_acl_type == sec_acl_type_default_object
	&& strcmp((char *)name, dced_c_service_secval) != 0) {
	for (mp = managers; mp->name; mp++) {
	    if (strcmp((char *)name, mp->name) == 0
	     && mp->obj_type
	     && uuid_equal(mgr_type, mp->obj_type, st)
	     && mp->do_acl) {
		*acl_uuid = *mp->do_acl;
		DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval, svc_c_debug1,
			"Resolver match (object): %s, type=%d", name,
			sec_acl_type));
		return;
	    }
	}
    }

    if (sec_acl_type != sec_acl_type_object) {
	*st = sec_acl_no_acl_found;
	return;
    }

    /* At this point, we know we're looking for an object */

    /* secval is an object only. (not a pseudo-container of objects.) */
    if (strcmp((char *)name, dced_c_service_secval) == 0
     && uuid_equal(mgr_type, &dced_sv_obj_type, st)) {
	*acl_uuid = dced_sv_acl;
	return;
    }

    /* Name must have a slash for an individual object. */
    if ((o_name = strchr((char *)name, '/')) == NULL) {
	*st = sec_acl_no_acl_found;
	return;
    }
    o_name++;	

    if (strncmp((char *)name, dced_c_service_hostdata,
	    dced_c_service_hostdata_len) == 0
     && uuid_equal(mgr_type, &hd_obj_type, st)) {
	dced_find_entry_from_name(hd_name_cache, hd_db, o_name, &entry_d, st);
	CHECK_STAT_RET(*st);
	*acl_uuid = entry_d.h.tagged_union.h.acl_uuid;
	return;
    }
    if (strncmp((char *)name, dced_c_service_srvrconf,
	    dced_c_service_srvrconf_len) == 0
     && uuid_equal(mgr_type, &sc_obj_type, st)) {
	dced_find_server_from_name(sc_name_cache, sc_db, o_name, &server_d, st);
	CHECK_STAT_RET(*st);
	*acl_uuid = server_d.h.tagged_union.h.acl_uuid;
	return;
    }
    if (strncmp((char *)name, dced_c_service_srvrexec,
	    dced_c_service_srvrexec_len) == 0
     && uuid_equal(mgr_type, &sx_obj_type, st)) {
	dced_find_server_from_name(sx_name_cache, sx_db, o_name, &server_d, st);
	CHECK_STAT_RET(*st);
	*acl_uuid = server_d.h.tagged_union.h.acl_uuid;
	return;
    }
    if (strncmp((char *)name, dced_c_service_keytab,
	    dced_c_service_keytab_len) == 0
     && uuid_equal(mgr_type, &km_obj_type, st)) {
	dced_find_entry_from_name(km_name_cache, km_db, o_name, &entry_d, st);
	CHECK_STAT_RET(*st);
	*acl_uuid = entry_d.h.tagged_union.h.acl_uuid;
	return;
    }
    if (strncmp((char *)name, dced_c_service_attr_sch,
	    dced_c_service_attr_sch_len) == 0
     && uuid_equal(mgr_type, &as_obj_type, st)) {
	dced_find_attr_schema_from_name(as_name_cache, as_db, o_name,
	    &schema_d, st);
	CHECK_STAT_RET(*st);
	*acl_uuid = schema_d.h.tagged_union.h.acl_uuid;
	return;
    }

    *st = sec_acl_no_acl_found;
}

/* ______________________________________________________________________ */
/* register all the object types for the ACLs we manage with the ACL lib. */

void
dced_register_acls(
    error_status_t	*st
)
{
    *st = error_status_ok;

    dce_lock_justwrite(acl_lock, st);
    CHECK_STAT_RET(*st);

    /* Register pseudo-container types */

    dce_acl_register_object_type(acl_db, &hd_cont_type,
	sizeof cont_prstr / sizeof cont_prstr[0],
	cont_prstr, &cont_info, dced_perm_control, 0,
	dced_resolve_container, NULL, 0, st);
    CHECK_STAT_RET(*st);

    dce_acl_register_object_type(acl_db, &km_cont_type,
	sizeof cont_prstr / sizeof cont_prstr[0],
	cont_prstr, &cont_info, dced_perm_control, 0,
	dced_resolve_container, NULL, 0, st);
    CHECK_STAT_RET(*st);

    dce_acl_register_object_type(acl_db, &sc_cont_type,
	sizeof cont_prstr / sizeof cont_prstr[0],
	cont_prstr, &cont_info, dced_perm_control, 0,
	dced_resolve_container, NULL, 0, st);
    CHECK_STAT_RET(*st);

    dce_acl_register_object_type(acl_db, &sx_cont_type,
	sizeof cont_prstr / sizeof cont_prstr[0],
	cont_prstr, &cont_info, dced_perm_control, 0,
	dced_resolve_container, NULL, 0, st);
    CHECK_STAT_RET(*st);

    dce_acl_register_object_type(acl_db, &as_cont_type,
	sizeof cont_prstr / sizeof cont_prstr[0],
	cont_prstr, &cont_info, dced_perm_control, 0,
	dced_resolve_container, NULL, 0, st);
    CHECK_STAT_RET(*st);

    /* Register object types */

    dce_acl_register_object_type(acl_db, &hd_obj_type,
	sizeof hd_prstr / sizeof hd_prstr[0],
	hd_prstr, &hd_info, dced_perm_control, 0,
	dced_resolve_object, NULL, dce_acl_c_has_owner, st);
    CHECK_STAT_RET(*st);

    dce_acl_register_object_type(acl_db, &km_obj_type,
	sizeof km_prstr / sizeof km_prstr[0],
	km_prstr, &km_info, dced_perm_control, 0,
	dced_resolve_object, NULL, dce_acl_c_has_owner, st);
    CHECK_STAT_RET(*st);

    dce_acl_register_object_type(acl_db, &sc_obj_type,
	sizeof sc_prstr / sizeof sc_prstr[0],
	sc_prstr, &sc_info, dced_perm_control, 0,
	dced_resolve_object, NULL, dce_acl_c_has_owner, st);
    CHECK_STAT_RET(*st);

    dce_acl_register_object_type(acl_db, &sx_obj_type,
	sizeof sx_prstr / sizeof sx_prstr[0],
	sx_prstr, &sx_info, dced_perm_control, 0,
	dced_resolve_object, NULL, dce_acl_c_has_owner, st);
    CHECK_STAT_RET(*st);

    dce_acl_register_object_type(acl_db, &as_obj_type,
	sizeof as_prstr / sizeof as_prstr[0],
	as_prstr, &as_info, dced_perm_control, 0,
	dced_resolve_object, NULL, dce_acl_c_has_owner, st);
    CHECK_STAT_RET(*st);

    /* secval is different from the others - it's just an object */

    dce_acl_register_object_type(acl_db, &dced_sv_obj_type,
	sizeof sv_prstr / sizeof sv_prstr[0],
	sv_prstr, &sv_info, dced_perm_control, 0,
	dced_resolve_object, NULL, 0, st);
    CHECK_STAT_RET(*st);

    /* Register container/obj for dced */

    dce_acl_register_object_type(acl_db, &dced_config_type,
	sizeof config_prstr / sizeof config_prstr[0],
	config_prstr, &config_info, dced_perm_control, 0,
	dced_resolve_container, NULL, 0, st);
    CHECK_STAT_RET(*st);

    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_aclmgr, svc_c_debug1,
		   "ACL enabled."));

    dced_is_acl_enabled = TRUE;

    dce_lock_unlock(acl_lock, st);
}

void
dced_unregister_acls(
    error_status_t	*st
)
{
    error_status_t lst;

    *st = error_status_ok;

    dce_lock_justwrite(acl_lock, st);
    CHECK_STAT_RET(*st);

#define	CHECK_STAT_UNLCK_RET(st)	if ((st) != error_status_ok) { \
dce_lock_unlock(acl_lock, &lst); \
return; \
}

    /* Unregister pseudo-container types */

    dce_acl_unregister_object_type(&hd_cont_type, st);
    CHECK_STAT_UNLCK_RET(*st);

    dce_acl_unregister_object_type(&km_cont_type, st);
    CHECK_STAT_UNLCK_RET(*st);

    dce_acl_unregister_object_type(&sc_cont_type, st);
    CHECK_STAT_UNLCK_RET(*st);

    dce_acl_unregister_object_type(&sx_cont_type, st);
    CHECK_STAT_UNLCK_RET(*st);

    dce_acl_unregister_object_type(&as_cont_type, st);
    CHECK_STAT_UNLCK_RET(*st);

    /* Unregister object types */

    dce_acl_unregister_object_type(&hd_obj_type, st);
    CHECK_STAT_UNLCK_RET(*st);

    dce_acl_unregister_object_type(&km_obj_type, st);
    CHECK_STAT_UNLCK_RET(*st);

    dce_acl_unregister_object_type(&sc_obj_type, st);
    CHECK_STAT_UNLCK_RET(*st);

    dce_acl_unregister_object_type(&sx_obj_type, st);
    CHECK_STAT_UNLCK_RET(*st);

    dce_acl_unregister_object_type(&as_obj_type, st);
    CHECK_STAT_UNLCK_RET(*st);

    /* secval is different from the others - it's just an object */

    dce_acl_unregister_object_type(&dced_sv_obj_type, st);
    CHECK_STAT_UNLCK_RET(*st);

    /* Unregister container/obj for dced */

    dce_acl_unregister_object_type(&dced_config_type, st);
    CHECK_STAT_UNLCK_RET(*st);
#undef CHECK_STAT_UNLCK_RET

    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_aclmgr, svc_c_debug1,
		   "ACL disabled."));

    dced_is_acl_enabled = FALSE;

    dce_lock_unlock(acl_lock, st);
}

void
dced_setup_acls(
    error_status_t	*st
)
{
    dce_lock_create(&acl_lock, st);
    CHECK_STAT_RET(*st);

    dced_register_acls(st);
}

/* ______________________________________________________________________ */
/*  Write (hard-coded) acls for the objects we manage.
 */
static void
write_acl(
    uuid_t		*acl_uuid,
    uuid_t		*mgr_type,
    sec_acl_permset_t	self_perms,
    sec_acl_permset_t	other_perms,
    error_status_t	*st
)
{
    static sec_acl_t	nil_acl;
    sec_acl_t		acl;
    sec_acl_entry_t	entries[2];

    entries[0].perms = self_perms;
    if (uuid_is_nil(&dced_g_info.thishost_prin_uuid, st))
	entries[0].entry_info.entry_type = sec_acl_e_type_any_other;
    else {
	entries[0].entry_info.entry_type = sec_acl_e_type_user;
	entries[0].entry_info.tagged_union.id.uuid =
	    dced_g_info.thishost_prin_uuid;
	entries[0].entry_info.tagged_union.id.name = NULL;
    }
    entries[1].entry_info.entry_type = sec_acl_e_type_unauthenticated;
    entries[1].perms = other_perms;
    acl = nil_acl;
    acl.sec_acl_manager_type = *mgr_type;
    acl.default_realm.uuid = dced_g_info.this_cell_uuid;
    acl.num_entries = 2;
    acl.sec_acl_entries = entries;
    dce_db_store_by_uuid(acl_db, acl_uuid, &acl, st);
    if (*st != error_status_ok)
	dce_svc_printf(DCED_S_CANT_STORE_ACL_MSG, "write_acl", (long)*st);
}

void
init_acl_database(
    error_status_t	*st
)
{
    manager_list_t		*mp;

    dce_lock_justwrite(acl_lock, st);
    CHECK_STAT_RET(*st);

    for (mp = managers; mp->name; mp++) {
	/* pseudo-container ACLs */
	if (mp->cont_type != NULL) {
	    write_acl(mp->acl, mp->cont_type,
		      mp->self_perms, mp->other_perms, st);
	    if (*st != error_status_ok) {
		dce_svc_printf(DCED_S_INIT_ACL_FAIL_MSG, mp->name,
			       "container", (long)*st);
		return;
	    }
	}

	/* Initial object ACLs */
	if (mp->obj_type != NULL) {
	    write_acl(mp->do_acl, mp->obj_type,
		      mp->do_self_perms, mp->do_other_perms, st);
	    if (*st != error_status_ok) {
		dce_svc_printf(DCED_S_INIT_ACL_FAIL_MSG, mp->name,
			       "init. object", (long)*st);
		return;
	    }
	}
    }

    /* For the DCED server itself (eg, for rpc_mgmt_xxx) */
    write_acl(&dced_config_acl, &dced_config_type,
	      dced_sx_all_perms, dced_perm_read, st);
    if (*st != error_status_ok) {
	dce_svc_printf(DCED_S_INIT_ACL_FAIL_MSG, "dced", "server", (long)*st);
	return;
    }

    dce_lock_unlock(acl_lock, st);
}

/* ______________________________________________________________________ */
/*  Check permissions when someone does an rpc_mgmt_call.
 *  This function is passed to rpc_mgmt_set_authorization_fn() before
 *  we (rpc runtime) start listening.
 */

boolean32
dced_server_auth(
    rpc_binding_handle_t	h,
    unsigned32			op,
    error_status_t		*st
)
{
    boolean32			ok;

    /* Check ACL for permission only if someone is trying to stop dced. */
    if (op == rpc_c_mgmt_stop_server_listen) {
	if (!dced_is_acl_enabled) {
	    *st = sec_acl_not_authorized;
	    return FALSE;
	}
	dce_lock_read(acl_lock, st);
	dce_acl_is_client_authorized(h, &dced_config_type, &dced_config_acl,
		NULL, NULL, dced_perm_stop, &ok, st);
	dce_lock_unlock(acl_lock, st);
	if (!ok) {
	    *st = sec_acl_not_authorized;
	    return FALSE;
	}
    }
    return TRUE;
}

/* ______________________________________________________________________ */
/*  Add admin group acls for the objects we manage.
**
** Note: This routine is intended to be called by
** acl_patcher() only. It does not check if an admin
** group acl already exists.
*/
static void
add_admin_group_acl(
    uuid_t		*admin_group_uuid,
    error_status_t	*st
)
{
    error_status_t		st2;
    uuid_t			*u;
    sec_acl_t			acl;
    int				i;
    int				j;
    manager_list_t		*mp;
    dced_entry_list_t		items;
    dced_entry_data_t		id_d;
    boolean32			thishost_only = TRUE;

    if (admin_group_uuid != NULL
	&& !uuid_is_nil(admin_group_uuid, &st2)
	&& st2 == error_status_ok)
	thishost_only = FALSE;

    for (mp = managers; !thishost_only && mp->name; mp++) {
	/* pseudo-container ACLs */
	if (mp->cont_type != NULL) {
	    dce_db_fetch_by_uuid(acl_db, mp->acl, &acl, st);
	    if (*st != error_status_ok) {
		dce_svc_printf(DCED_S_CANT_FETCH_ACL_MSG,
			       "add_admin_group_acl", (long)*st);
		goto io_acls;
	    }

	    dce_acl_obj_add_group_entry(&acl, mp->admin_perms,
					admin_group_uuid, &st2);
	    if (st2 != error_status_ok) {
		dce_svc_printf(DCED_S_INIT_ACL_FAIL_MSG, "add_admin_group_acl",
			       "admin group container ACL", (long)st2);
	    }
	    else {
		dce_db_store_by_uuid(acl_db, mp->acl, &acl, st);
		if (*st != error_status_ok) {
		    dce_svc_printf(DCED_S_CANT_STORE_ACL_MSG,
				   "add_admin_group_acl", (long)*st);
		}
		else {
		    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_aclmgr,
				   svc_c_debug1,
				   "Added admin group container ACL: %s",
				   mp->name));
		}
	    }
	    dce_acl_obj_free_entries(&acl, &st2);
	}
io_acls:
	/* Initial object ACLs */
	if (mp->obj_type != NULL) {
	    dce_db_fetch_by_uuid(acl_db, mp->do_acl, &acl, st);
	    if (*st != error_status_ok) {
		dce_svc_printf(DCED_S_CANT_FETCH_ACL_MSG,
			       "add_admin_group_acl", (long)*st);
		continue;
	    }

	    dce_acl_obj_add_group_entry(&acl, mp->do_admin_perms,
					admin_group_uuid, &st2);
	    if (st2 != error_status_ok) {
		dce_svc_printf(DCED_S_INIT_ACL_FAIL_MSG, "add_admin_group_acl",
			       "admin group init. object ACL", (long)st2);
	    }
	    else {
		dce_db_store_by_uuid(acl_db, mp->do_acl, &acl, st);
		if (*st != error_status_ok) {
		    dce_svc_printf(DCED_S_CANT_STORE_ACL_MSG,
				   "add_admin_group_acl", (long)*st);
		}
		else {
		    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_aclmgr,
				   svc_c_debug1,
				   "Added admin group init. object ACL: %s",
				   mp->name));
		}
	    }
	    dce_acl_obj_free_entries(&acl, &st2);
	}
    }

    /* Patch already created objects' ACLs. */
    (*dced_hostdata_v1_0_epv.hostdata_get_list)(NULL, &items, st);
    if (*st != error_status_ok)
	goto rkeytab_acls;

    for (i = 0; i < items.count; i++) {
	dce_db_fetch_by_uuid(hd_db, &items.list[i].id, &id_d, st);
	if (*st != error_status_ok)
	    continue;

	dce_db_fetch_by_uuid(acl_db,
			     &id_d.h.tagged_union.h.acl_uuid,
			     &acl, st);
	if (*st != error_status_ok) {
	    dce_svc_printf(DCED_S_CANT_FETCH_ACL_MSG,
			   "add_admin_group_acl", (long)*st);
	    continue;
	}

	st2 = error_status_ok;

	if (strcmp((char *)items.list[i].name,
		   dced_c_passwd_override) == 0
	    || strcmp((char *)items.list[i].name,
		      dced_c_group_override) == 0) {
	    /*
	     * passwd_override and group_override can't be remotely
	     * accessible.
	     */
	    for (j = 0; j < acl.num_entries; j++) {
		if ((acl.sec_acl_entries[j].entry_info.entry_type
		     == sec_acl_e_type_user) &&
		    uuid_equal(&acl.sec_acl_entries[j].entry_info.tagged_union.id.uuid,
			       &dced_g_info.thishost_prin_uuid, &st2)) {
		    acl.sec_acl_entries[j].perms = 0;
		}
		else if ((acl.sec_acl_entries[j].entry_info.entry_type
			  == sec_acl_e_type_unauthenticated)
			 || (acl.sec_acl_entries[j].entry_info.entry_type
			  == sec_acl_e_type_any_other)) {
		    acl.sec_acl_entries[j].perms = 0;
		}
	    }
	    if (!thishost_only) {
		dce_acl_obj_add_group_entry(&acl, 0,
					    admin_group_uuid, &st2);
	    }
	}
	else if (strcmp((char *)items.list[i].name,
			dced_c_hostdata_post_proc) == 0) {
	    /*
	     * post_processors can't be remotely editable.
	     */
	    for (j = 0; j < acl.num_entries; j++) {
		if ((acl.sec_acl_entries[j].entry_info.entry_type
		     == sec_acl_e_type_user) &&
		    uuid_equal(&acl.sec_acl_entries[j].entry_info.tagged_union.id.uuid,
			       &dced_g_info.thishost_prin_uuid, &st2)) {
		    acl.sec_acl_entries[j].perms =
			dced_perm_purge | dced_perm_read;
		}
	    }
	    if (!thishost_only) {
		dce_acl_obj_add_group_entry(&acl,
					    dced_perm_purge
					    | dced_perm_read,
					    admin_group_uuid, &st2);
	    }
	}
	else if (!thishost_only) {
	    dce_acl_obj_add_group_entry(&acl,
				      (dced_hd_all_perms & ~dced_perm_control),
					admin_group_uuid, &st2);
	}

	if (st2 != error_status_ok) {
	    dce_svc_printf(DCED_S_INIT_ACL_FAIL_MSG,
			   "add_admin_group_acl",
			   items.list[i].name, (long)st2);
	}
	else {
	    dce_db_store_by_uuid(acl_db,
				 &id_d.h.tagged_union.h.acl_uuid,
				 &acl, st);
	    if (*st != error_status_ok) {
		dce_svc_printf(DCED_S_CANT_STORE_ACL_MSG,
			       "add_admin_group_acl", (long)*st);
	    }
	    else if (!thishost_only) {
		DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_aclmgr,
			       svc_c_debug1,
			       "Added admin group ACL: %s",
			       items.list[i].name));
	    }
	}
	dce_acl_obj_free_entries(&acl, &st2);
    }

rkeytab_acls:
    (*dced_rkeytab_v1_0_epv.rkeytab_get_list)(NULL, &items, st);
    if (*st != error_status_ok || thishost_only)
	return;

    for (i = 0; i < items.count; i++) {
	dce_db_fetch_by_uuid(km_db, &items.list[i].id, &id_d, st);
	if (*st != error_status_ok)
	    continue;

	dce_db_fetch_by_uuid(acl_db,
			     &id_d.h.tagged_union.h.acl_uuid,
			     &acl, st);
	if (*st != error_status_ok) {
	    dce_svc_printf(DCED_S_CANT_FETCH_ACL_MSG,
			   "add_admin_group_acl", (long)*st);
	    continue;
	}

	dce_acl_obj_add_group_entry(&acl,
				    (dced_km_all_perms & ~dced_perm_control),
				    admin_group_uuid, &st2);
	if (st2 != error_status_ok) {
	    dce_svc_printf(DCED_S_INIT_ACL_FAIL_MSG,
			   "add_admin_group_acl",
			   items.list[i].name, (long)st2);
	}
	else {
	    dce_db_store_by_uuid(acl_db,
				 &id_d.h.tagged_union.h.acl_uuid,
				 &acl, st);
	    if (*st != error_status_ok) {
		dce_svc_printf(DCED_S_CANT_STORE_ACL_MSG,
			       "add_admin_group_acl", (long)*st);
	    }
	    else {
		DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_aclmgr,
			       svc_c_debug1,
			       "Added admin group ACL: %s",
			       items.list[i].name));
	    }
	}
	dce_acl_obj_free_entries(&acl, &st2);
    }

}

/* ______________________________________________________________________ */
/*
**  ACLs were created in bootstrap phase with a nil uuid for the realm.
**  Patch them to have the current realm and change any_other to
**  machine_principal.
*/
void
dced_acl_patcher(
    uuid_t			*admin_uuid,
    uuid_t			*admin_group_uuid,
    error_status_t              *st
)
{
    error_status_t		st2;
    uuid_t			*u;
    sec_acl_t			acl;
    unsigned32			count;
    uuid_t			*patches;
    int				i;
    int				j;
    sec_acl_permset_t		other_perms;

    /* Can't do DB store operations within a DB iteration (our position
     * gets lost).  So first loop to find all suspects, then loop and
     * fix them. */

    rpc_ss_enable_allocate();
    dce_db_iter_start(acl_db, st);
    if (*st != error_status_ok) {
	dce_svc_printf(DCED_S_ITER_FAIL_MSG, "acl_patcher", (long)*st);
	return;
    }

    dce_lock_riw(acl_lock, st);
    for (count = 0, patches = malloc(1); ; ) {
	dce_db_iter_next_by_uuid(acl_db, &u, st);
	if (*st == db_s_no_more) {
	    *st = error_status_ok;
	    break;
	}
	if (*st != error_status_ok) {
	    dce_svc_printf(DCED_S_CANT_FETCH_ACL_MSG, "acl_patcher", (long)*st);
	    goto Unlock;
	}

	dce_db_fetch_by_uuid(acl_db, u, &acl, st);
	if (*st != error_status_ok) {
	    dce_svc_printf(DCED_S_CANT_FETCH_ACL_MSG, "acl_patcher", (long)*st);
	    goto Unlock;
	}

	if (uuid_is_nil(&acl.default_realm.uuid, st)) {
	    if (count % dced_c_acl_chunk_size == 0)
		patches = realloc(patches,
			    (count + dced_c_acl_chunk_size) * sizeof (uuid_t));
	    patches[count++] = *u;
	}
	dce_acl_obj_free_entries(&acl, &st2);
    }
    dce_db_iter_done(acl_db, st);
    if (*st != error_status_ok) {
	dce_svc_printf(DCED_S_ITER_FAIL_MSG, "acl_patcher", (long)*st);
	goto Unlock;
    }

    if (count) {
	dce_lock_write(acl_lock, st);
	if (*st != error_status_ok) {
	    dce_svc_printf(DCED_S_LOCK_FAIL_MSG, "acl_lock", "acl_patcher", (long)*st);
	    goto Unlock;
	}
	for (i = 0; i < count; i++) {
	    dce_db_fetch_by_uuid(acl_db, &patches[i], &acl, st);
	    if (*st != error_status_ok) {
		dce_svc_printf(DCED_S_CANT_FETCH_ACL_MSG, "acl_patcher", (long)*st);
		continue;
	    }
	    acl.default_realm.uuid = dced_g_info.this_cell_uuid;
	    for (other_perms = 0, j = 0; j < acl.num_entries; j++) {
		if (acl.sec_acl_entries[j].entry_info.entry_type
		    == sec_acl_e_type_any_other) {
		    acl.sec_acl_entries[j].entry_info.entry_type =
			sec_acl_e_type_user;
		    acl.sec_acl_entries[j].entry_info.tagged_union.id.uuid =
			dced_g_info.thishost_prin_uuid;
		    acl.sec_acl_entries[j].entry_info.tagged_union.id.name =
			NULL;
		}
		else if (acl.sec_acl_entries[j].entry_info.entry_type
			 == sec_acl_e_type_unauthenticated) {
		    other_perms = acl.sec_acl_entries[j].perms;
		}
	    }
	    dce_acl_obj_add_any_other_entry(&acl, other_perms, &st2);
	    if (st2 != error_status_ok) {
		dce_svc_printf(DCED_S_INIT_ACL_FAIL_MSG, "acl_patcher",
			       "any_other ACL", (long)st2);
	    }
	    else {
		DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_aclmgr,
			       svc_c_debug1,
			       "Added any_other ACL"));
	    }
	    dce_db_store_by_uuid(acl_db, &patches[i], &acl, st);
	    if (*st != error_status_ok) {
		dce_svc_printf(DCED_S_CANT_STORE_ACL_MSG, "acl_patcher", (long)*st);
	    }
	    dce_acl_obj_free_entries(&acl, &st2);
	}
	/* Not using admin_uuid... */
	add_admin_group_acl(admin_group_uuid, st);
    }
Unlock:
    dce_lock_unlock(acl_lock, &st2);
    if (st2 != error_status_ok)
	dce_svc_printf(DCED_S_UNLOCK_FAIL_MSG, "acl_lock", "acl_patcher", (long)st2);
    free(patches);
    rpc_ss_disable_allocate();
}
