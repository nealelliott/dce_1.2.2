/* ______________________________________________________________________ */
/*
 * @OSF_COPYRIGHT@
 * Copyright (c) 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dced_data.h,v $
 * Revision 1.1.4.2  1996/02/18  23:31:57  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:08  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:26:16  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/06/26  19:01 UTC  mk
 * 	Merge in OT12573 fix from mk_dced_hostdata_init/1.
 * 
 * 	HP revision /main/mk_dced_hostdata_init/1  1995/06/26  18:35 UTC  mk
 * 	Fix OT12573:  Add more hostdata files.
 * 	[1995/12/08  17:58:54  root]
 * 
 * Revision 1.1.2.17  1994/09/26  17:49:16  rsalz
 * 	Fix names of CDECONFIG xattr object (OT CR 12293).
 * 	[1994/09/26  17:11:38  rsalz]
 * 
 * Revision 1.1.2.16  1994/09/22  18:01:01  bowe
 * 	Added cde session xattr definitions. [CR 12293]
 * 	[1994/09/22  17:35:37  bowe]
 * 
 * Revision 1.1.2.15  1994/09/21  21:49:33  pwang
 * 	Adjusted dced attr_schema entry names [OT#12146]
 * 	[1994/09/21  21:46:38  pwang]
 * 
 * Revision 1.1.2.14  1994/08/26  20:13:02  bowe
 * 	Added pe_site and svc_routing hd objects. [CR 11986]
 * 	[1994/08/26  20:11:34  bowe]
 * 
 * Revision 1.1.2.13  1994/08/23  19:23:58  rsalz
 * 	Change names of hostdata ERA attributes (OT CR 11711).
 * 	[1994/08/22  18:20:17  rsalz]
 * 
 * Revision 1.1.2.12  1994/07/25  19:10:21  rsalz
 * 	Change to new schema name (OT CR 11427).
 * 	[1994/07/25  19:08:34  rsalz]
 * 
 * Revision 1.1.2.11  1994/05/31  21:08:44  pwang
 * 	Added dced_g_uuid_all_servers for all dce services/servers.
 * 	[1994/05/31  20:51:37  pwang]
 * 
 * Revision 1.1.2.10  1994/05/10  15:39:20  rsalz
 * 	Add dtsconf and envadd attributes.
 * 	[1994/05/10  15:38:47  rsalz]
 * 
 * Revision 1.1.2.9  1994/05/05  14:41:10  pwang
 * 	Rename _rkeytab_ to _keytab_.
 * 	[1994/05/05  14:33:23  pwang]
 * 
 * Revision 1.1.2.8  1994/04/26  13:53:29  bowe
 * 	Added #define's for well-known filenames.
 * 	[1994/04/26  13:53:12  bowe]
 * 
 * Revision 1.1.2.7  1994/04/11  15:34:06  bowe
 * 	dced_c_uuid_binfileattr_string was mis-defined
 * 	[1994/04/11  15:33:51  bowe]
 * 
 * Revision 1.1.2.6  1994/03/09  19:00:15  rsalz
 * 	Remove host_type uuid; add bincontents.
 * 	Replace lengths with sizeof.
 * 	Use ANSI string concatenation.
 * 	[1994/03/09  18:51:57  rsalz]
 * 
 * Revision 1.1.2.5  1994/03/04  21:38:13  bowe
 * 	Fixed botched spelling of dced_g_uuid_host_keyfile and dced_g_uuid_dce_cf_db
 * 	[1994/03/04  21:36:41  bowe]
 * 
 * Revision 1.1.2.4  1994/03/04  18:00:38  pwang
 * 	Merged with changes from 1.1.2.3
 * 	[1994/03/04  18:00:32  pwang]
 * 
 * 	Added dced_c_filecontents_name
 * 	[1994/03/04  17:59:18  pwang]
 * 
 * Revision 1.1.2.3  1994/03/04  16:55:20  bowe
 * 	Moved symbols for dced service names from dced.h here.
 * 	Changed the name from "rkeytab" to "keytab".
 * 	[1994/03/04  16:54:07  bowe]
 * 
 * Revision 1.1.2.2  1994/03/04  15:29:28  bowe
 * 	Added dced_g_host_keyfile and dced_g_dce_cf_db
 * 	[1994/03/04  14:29:29  bowe]
 * 
 * 	Added dced_c_uuid_codeset_registry and dced_c_uuid_hostdata_post_proc UUIDs.
 * 	[1994/02/28  17:58:05  bowe]
 * 
 * Revision 1.1.2.1  1994/02/22  22:04:21  bowe
 * 	Initial checkin
 * 	[1994/02/22  22:02:30  bowe]
 * 
 * $EndLog$
 */
/* ______________________________________________________________________ */

#ifndef _DCED_DATA_H
#define	_DCED_DATA_H

/* Declarations for global data for DCED, both server and client sides. */

#include <dce/nbase.h>
#include <dce/dce.h>

/*
** dced service names (and lengths) - for the "namespace"
*/
#define dced_c_service_hostdata		"hostdata"
#define dced_c_service_keytab		"keytab"
#define dced_c_service_srvrconf		"srvrconf"
#define dced_c_service_srvrexec		"srvrexec"
#define dced_c_service_secval		"secval"
#define dced_c_service_attr_sch		"xattrschema"
#define dced_c_service_hostdata_len	(sizeof dced_c_service_hostdata - 1)
#define dced_c_service_keytab_len	(sizeof dced_c_service_keytab - 1)
#define dced_c_service_srvrconf_len	(sizeof dced_c_service_srvrconf - 1)
#define dced_c_service_srvrexec_len	(sizeof dced_c_service_srvrexec - 1)
#define dced_c_service_secval_len	(sizeof dced_c_service_secval - 1)
#define dced_c_service_attr_sch_len	(sizeof dced_c_service_attr_sch - 1)


/*
 * Well-known UUIDs, both as strings and C code (so your code does not have
 * to use uuid_from_string()).
 */
#define dced_c_uuid_fileattr_string	"008b47dd-6ec9-1d6a-9ac7-0000c09ce054"
#define dced_c_uuid_binfileattr_string	"764fd860-3b6f-11cd-b254-08000925634b"
#define dced_c_uuid_dtsconfattr_string	"b574524e-6b37-11cd-8ec2-08000925634b"
#define dced_c_uuid_cdeses_string	"64311416-d613-11cd-a37f-0000c09ce054"
#define dced_c_uuid_envaddattr_string	"041f9efc-6b39-11cd-8848-08000925634b"
#define dced_c_uuid_host_name_string	"00050065-6eca-1d6a-894b-0000c09ce054"
#define dced_c_uuid_cell_name_string	"00174f6c-6eca-1d6a-bf90-0000c09ce054"
#define dced_c_uuid_cell_aliases_string	"002817df-6eca-1d6a-b08a-0000c09ce054"
#define dced_c_uuid_codeset_registry	"0014ab37-b292-1d6b-90a1-0000c09ce054"
#define dced_c_uuid_hostdata_post_proc	"004270cb-b292-1d6b-894b-0000c09ce054"
#define dced_c_uuid_host_keyfile	"00096565-4301-1d77-9108-0000c09ce054"
#define dced_c_uuid_dce_cf_db		"002c7cdf-4301-1d77-af44-0000c09ce054"
#define dced_c_uuid_pe_site		"4bed6d84-c0d1-11cd-975f-0000c09ce054"
#define dced_c_uuid_svc_routing		"4bf81be4-c0d1-11cd-975f-0000c09ce054"
#define	dced_c_uuid_all_servers		"0045d2b0-727f-1deb-95bb-0000c0ba4944"

#ifndef HPDCE_OT12573_FIX
#define dced_c_uuid_krb_conf		"66436fbe-ed48-11cd-8c44-08000925634b"
#define dced_c_uuid_dfs_cache_info	"67c2dec4-ed48-11cd-8cb1-08000925634b"
#define dced_c_uuid_cds_conf		"693af3d6-ed48-11cd-a5b3-08000925634b"
#define dced_c_uuid_cds_attributes	"6aa6d06e-ed48-11cd-8392-08000925634b"
#define dced_c_uuid_cds_globalnames	"6bfd884a-ed48-11cd-9e69-08000925634b"
#define dced_c_uuid_passwd_override	"7017e62e-f1c5-11cd-9369-08000925634b"
#define dced_c_uuid_group_override	"714c22c6-f1c5-11cd-9e58-08000925634b"
#endif /* HPDCE_OT12573_FIX */

extern uuid_t dced_g_uuid_fileattr;
extern uuid_t dced_g_uuid_binfileattr;
extern uuid_t dced_g_uuid_dtsconfattr;
extern uuid_t dced_g_uuid_cdeses;
extern uuid_t dced_g_uuid_envaddattr;
extern uuid_t dced_g_uuid_host_name;
extern uuid_t dced_g_uuid_cell_name;
extern uuid_t dced_g_uuid_cell_aliases;
extern uuid_t dced_g_uuid_codeset_registry;
extern uuid_t dced_g_uuid_hostdata_post_proc;
extern uuid_t dced_g_uuid_host_keyfile;
extern uuid_t dced_g_uuid_dce_cf_db;
extern uuid_t dced_g_uuid_pe_site;
extern uuid_t dced_g_uuid_svc_routing;
extern uuid_t dced_g_uuid_all_servers;

#ifndef HPDCE_OT12573_FIX
extern uuid_t dced_g_uuid_krb_conf;
extern uuid_t dced_g_uuid_dfs_cache_info;
extern uuid_t dced_g_uuid_cds_conf;
extern uuid_t dced_g_uuid_cds_attributes;
extern uuid_t dced_g_uuid_cds_globalnames;
extern uuid_t dced_g_uuid_passwd_override;
extern uuid_t dced_g_uuid_group_override;
#endif /* HPDCE_OT12573_FIX */

/* These names go in CDS. */
#define dced_c_host_name		"host_name"
#define dced_c_cell_name		"cell_name"
#define dced_c_cell_aliases		"cell_aliases"
#define dced_c_hostdata_post_proc	"post_processors"
#define dced_c_dce_cf_db		"dce_cf.db"
#define dced_c_pe_site			"pe_site"
#define dced_c_svc_routing		"svc_routing"

#ifndef HPDCE_OT12573_FIX
#define dced_c_krb_conf			"krb.conf"
#define dced_c_dfs_cache_info		"dfs-cache-info"
#define dced_c_cds_conf			"cds.conf"
#define dced_c_cds_attributes		"cds_attributes"
#define dced_c_cds_globalnames		"cds_globalnames"
#define dced_c_passwd_override		"passwd_override"
#define dced_c_group_override		"group_override"
#endif /* HPDCE_OT12573_FIX */

/* These are the filenames relative to dcelocal_path. */
#define dced_c_pe_site_file		"etc/security/pe_site"
#define dced_c_svc_routing_file		"var/svc/routing"

#ifndef HPDCE_OT12573_FIX
/* This filename is absolute. */
#define dced_c_krb_conf_file		"/krb5/krb.conf"

/* These filenames are relative to dcelocal_path. */
#define dced_c_dfs_cache_info_file	"etc/CacheInfo"
#define dced_c_cds_conf_file		"etc/cds.conf"
#define dced_c_cds_attributes_file	"etc/cds_attributes"
#define dced_c_cds_globalnames_file	"etc/cds_globalnames"
#define dced_c_passwd_override_file	"etc/passwd_override"
#define dced_c_group_override_file	"etc/group_override"
#endif /* HPDCE_OT12573_FIX */

/*
 * Predefined decd attribute names.
 */
#define	dced_c_filecontents_name	dced_c_service_hostdata "/data"
#define	dced_c_bincontents_name		dced_c_service_hostdata "/bindata"
#define dced_c_dtsconfattr_name		dced_c_service_srvrconf "/dtsconfig"
#define	dced_c_cdeses_name		dced_c_service_srvrconf "/cde_ses"
#define dced_c_envaddattr_name		\
				dced_c_service_srvrconf "/additional_environ"

#endif
