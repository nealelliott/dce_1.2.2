/*
 * @OSF_COPYRIGHT@
 * Copyright (c) 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dced_data.c,v $
 * Revision 1.1.4.2  1996/02/18  19:15:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:01:40  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:26:12  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/06/26  19:45 UTC  mk
 * 	Merge in OT12573 fix from mk_dced_hostdata_init/1.
 * 
 * 	HP revision /main/mk_dced_hostdata_init/1  1995/06/26  18:37 UTC  mk
 * 	Fix OT12573:  Add more hostdata files.
 * 	[1995/12/08  17:58:52  root]
 * 
 * Revision 1.1.2.9  1994/09/22  18:00:59  bowe
 * 	Added cde session xattr definitions. [CR 12293]
 * 	[1994/09/22  17:35:34  bowe]
 * 
 * Revision 1.1.2.8  1994/08/26  20:13:00  bowe
 * 	Added pe_site and svc_routing hd objects. [CR 11986]
 * 	[1994/08/26  20:11:31  bowe]
 * 
 * Revision 1.1.2.7  1994/05/31  21:08:41  pwang
 * 	Added dced_g_uuid_all_servers for all dce services/servers.
 * 	[1994/05/31  20:50:57  pwang]
 * 
 * Revision 1.1.2.6  1994/05/10  15:39:18  rsalz
 * 	Add dtsconf and envadd attributes.
 * 	[1994/05/10  15:38:46  rsalz]
 * 
 * Revision 1.1.2.5  1994/04/07  19:27:25  bowe
 * 	Fix name of "dced_g_uuid_binfileattr" (mistyped)
 * 	[1994/04/07  19:26:29  bowe]
 * 
 * Revision 1.1.2.4  1994/03/09  19:00:14  rsalz
 * 	Remove host_type uuid; add bincontents.
 * 	Replace lengths with sizeof.
 * 	Use ANSI string concatenation.
 * 	[1994/03/09  18:51:56  rsalz]
 * 
 * Revision 1.1.2.3  1994/03/04  21:38:12  bowe
 * 	Fixed botched spelling of dced_g_uuid_host_keyfile and dced_g_uuid_dce_cf_db
 * 	[1994/03/04  21:36:40  bowe]
 * 
 * Revision 1.1.2.2  1994/03/04  15:29:27  bowe
 * 	Added dced_g_host_keyfile and dced_g_dce_cf_db
 * 	[1994/03/04  14:29:27  bowe]
 * 
 * 	Added dced_c_uuid_codeset_registry and dced_c_uuid_hostdata_post_proc UUIDs.
 * 	[1994/02/28  17:57:51  bowe]
 * 
 * Revision 1.1.2.1  1994/02/22  22:04:18  bowe
 * 	Initial checkin
 * 	[1994/02/22  22:02:29  bowe]
 * 
 * $EndLog$
 */

/*
**  Declarations for global data for DCED, both server and client sides.
*/
#include <dce/dced_data.h>

/*
 * Well-known UUIDs.
 * These must exactly match the corresponding #define's in dced_data.h
 */
uuid_t dced_g_uuid_fileattr = { /* 008b47dd-6ec9-1d6a-9ac7-0000c09ce054 */
    0x008b47dd, 0x6ec9, 0x1d6a, 0x9a, 0xc7,
    {0x00, 0x00, 0xc0, 0x9c, 0xe0, 0x54}
};
uuid_t dced_g_uuid_binfileattr = { /* 764fd860-3b6f-11cd-b254-08000925634b */
    0x764fd860, 0x3b6f, 0x11cd, 0xb2, 0x54,
    {0x08, 0x00, 0x09, 0x25, 0x63, 0x4b}
};
uuid_t dced_g_uuid_dtsconfattr = { /* b574524e-6b37-11cd-8ec2-08000925634b */
    0xb574524eU, 0x6b37, 0x11cd, 0x8e, 0xc2, 
    {0x08, 0x00, 0x09, 0x25, 0x63, 0x4b}
};
uuid_t dced_g_uuid_cdeses = { /* 64311416-d613-11cd-a37f-0000c09ce054 */
    0x64311416, 0xd613, 0x11cd, 0xa3, 0x7f,
    { 0x00, 0x00, 0xc0, 0x9c, 0xe0, 0x54 }
};
uuid_t dced_g_uuid_envaddattr = { /* 041f9efc-6b39-11cd-8848-08000925634b */
    0x041f9efc, 0x6b39, 0x11cd, 0x88, 0x48, 
    {0x08, 0x00, 0x09, 0x25, 0x63, 0x4b}
};
uuid_t dced_g_uuid_host_name = { /* 00050065-6eca-1d6a-894b-0000c09ce054 */
    0x00050065, 0x6eca, 0x1d6a, 0x89, 0x4b,
    {0x00, 0x00, 0xc0, 0x9c, 0xe0, 0x54}
};
uuid_t dced_g_uuid_cell_name = { /* 00174f6c-6eca-1d6a-bf90-0000c09ce054 */
    0x00174f6c, 0x6eca, 0x1d6a, 0xbf, 0x90,
    {0x00, 0x00, 0xc0, 0x9c, 0xe0, 0x54}
};
uuid_t dced_g_uuid_cell_aliases = { /* 002817df-6eca-1d6a-b08a-0000c09ce054 */
    0x002817df, 0x6eca, 0x1d6a, 0xb0, 0x8a,
    {0x00, 0x00, 0xc0, 0x9c, 0xe0, 0x54}
};
uuid_t dced_g_uuid_codeset_registry = { /* 0014ab37-b292-1d6b-90a1-0000c09ce054 */
    0x0014ab37, 0xb292, 0x1d6b, 0x90, 0xa1,
    {0x00, 0x00, 0xc0, 0x9c, 0xe0, 0x54}
};
uuid_t dced_g_uuid_hostdata_post_proc = { /* 004270cb-b292-1d6b-894b-0000c09ce054 */
    0x004270cb, 0xb292, 0x1d6b, 0x89, 0x4b,
    {0x00, 0x00, 0xc0, 0x9c, 0xe0, 0x54}
};
uuid_t dced_g_uuid_host_keyfile = { /* 00096565-4301-1d77-9108-0000c09ce054 */
    0x00096565, 0x4301, 0x1d77, 0x91, 0x08, 
    {0x00, 0x00, 0xc0, 0x9c, 0xe0, 0x54}
};
uuid_t dced_g_uuid_dce_cf_db = { /* 002c7cdf-4301-1d77-af44-0000c09ce054 */
    0x002c7cdf, 0x4301, 0x1d77, 0xaf, 0x44, 
    {0x00, 0x00, 0xc0, 0x9c, 0xe0, 0x54}
};
uuid_t dced_g_uuid_pe_site = { /* 4bed6d84-c0d1-11cd-975f-0000c09ce054 */
    0x4bed6d84, 0xc0d1, 0x11cd, 0x97, 0x5f,
    { 0x00, 0x00, 0xc0, 0x9c, 0xe0, 0x54 }
};
uuid_t dced_g_uuid_svc_routing = { /* 4bf81be4-c0d1-11cd-975f-0000c09ce054 */
    0x4bf81be4, 0xc0d1, 0x11cd, 0x97, 0x5f,
    { 0x00, 0x00, 0xc0, 0x9c, 0xe0, 0x54 }
};

uuid_t dced_g_uuid_all_servers = { /* 0045d2b0-727f-1deb-95bb-0000c0ba4944 */
    0x0045d2b0, 0x727f, 0x1deb, 0x95, 0xbb, 
    {0x00, 0x00, 0xc0, 0xba, 0x49, 0x44}
};

#ifndef HPDCE_OT12573_FIX
uuid_t dced_g_uuid_krb_conf = { /* 66436fbe-ed48-11cd-8c44-08000925634b */
    0x66436fbe, 0xed48, 0x11cd, 0x8c, 0x44,
    {0x08, 0x00, 0x09, 0x25, 0x63, 0x4b}
};

uuid_t dced_g_uuid_dfs_cache_info = { /* 67c2dec4-ed48-11cd-8cb1-08000925634b */
    0x67c2dec4, 0xed48, 0x11cd, 0x8c, 0xb1,
    {0x08, 0x00, 0x09, 0x25, 0x63, 0x4b}
};

uuid_t dced_g_uuid_cds_conf = { /* 693af3d6-ed48-11cd-a5b3-08000925634b */
    0x693af3d6, 0xed48, 0x11cd, 0xa5, 0xb3,
    {0x08, 0x00, 0x09, 0x25, 0x63, 0x4b}
};

uuid_t dced_g_uuid_cds_attributes = { /* 6aa6d06e-ed48-11cd-8392-08000925634b */
    0x6aa6d06e, 0xed48, 0x11cd, 0x83, 0x92,
    {0x08, 0x00, 0x09, 0x25, 0x63, 0x4b}
};

uuid_t dced_g_uuid_cds_globalnames = { /* 6bfd884a-ed48-11cd-9e69-08000925634b */
    0x6bfd884a, 0xed48, 0x11cd, 0x9e, 0x69,
    {0x08, 0x00, 0x09, 0x25, 0x63, 0x4b}
};

uuid_t dced_g_uuid_passwd_override = { /* 7017e62e-f1c5-11cd-9369-08000925634b */
    0x7017e62e, 0xf1c5, 0x11cd, 0x93, 0x69,
    {0x08, 0x00, 0x09, 0x25, 0x63, 0x4b}
};

uuid_t dced_g_uuid_group_override = { /* 714c22c6-f1c5-11cd-9e58-08000925634b */
    0x714c22c6, 0xf1c5, 0x11cd, 0x9e, 0x58,
    {0x08, 0x00, 0x09, 0x25, 0x63, 0x4b}
};
#endif /* HPDCE_OT12573_FIX */
