/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: rgy.h,v $
 * Revision 1.1.10.3  1996/11/13  17:53:49  arvind
 * 	Merge changes.
 * 	[1996/10/15  16:01 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk7/2]
 *
 * 	CHFts20086: Force multiple new passphrases to be identical when entered
 * 	on the same command line.  Also handle default generate key values correctly.
 * 	[1996/10/11  17:52 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk7/1]
 *
 * 	CHFts20138: Add the ability to have pk passwords prompted for as opposed
 * 	to forcing the user to enter them on the command line.
 * 	[1996/10/11  15:20 UTC  cuti  /main/DCE_1.2.2/cuti_pk_fix/2]
 *
 * 	Change pk_genkey default_value back to 1024
 * 	[1996/09/26  16:18 UTC  cuti  /main/DCE_1.2.2/cuti_pk_fix/1]
 *
 * 	Change  pk_genkey default_value to 0 from 1024 to let the library decide the value
 * 	[1996/07/08  17:26 UTC  truitt  /main/DCE_1.2.2/2]
 *
 * 	Merge changes.
 * 	[1996/07/02  17:30 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/3]
 *
 * 	Additional pk work.
 * 	[1996/06/27  18:52 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/2]
 *
 * 	Public key back-end work.
 * 	[1996/06/11  16:56 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/1]
 *
 * 	Some dcecp changes to support Public Key functionality.
 * 	Merge with latest HP fixes and enhancements.
 * 	[1996/02/05  16:10 UTC  truitt  /main/HPDCE02/8]
 *
 * 	Merge changes.
 * 	[1996/02/05  16:03 UTC  truitt  /main/HPDCE02/truitt_dcecp_17281/1]
 *
 * Revision 1.1.10.2  1996/08/09  11:48:52  arvind
 * 	Merge changes.
 * 	[1996/07/02  17:30 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/3]
 * 
 * 	Additional pk work.
 * 	[1996/06/27  18:52 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/2]
 * 
 * 	Public key back-end work.
 * 	[1996/06/11  16:56 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/1]
 * 
 * 	Some dcecp changes to support Public Key functionality.
 * 	Merge with latest HP fixes and enhancements.
 * 	[1996/02/05  16:10 UTC  truitt  /main/HPDCE02/8]
 * 
 * 	Merge changes.
 * 	[1996/02/05  16:03 UTC  truitt  /main/HPDCE02/truitt_dcecp_17281/1]
 * 
 * Revision 1.1.10.1  1996/06/04  21:41:37  arvind
 * 	Merge u2u changes from mb_u2u
 * 	[1996/05/06  21:53 UTC  burati  /main/DCE_1.2/2]
 * 
 * 	merge u2u work
 * 	[1996/05/02  17:05 UTC  burati  /main/DCE_1.2/mb_u2u/1]
 * 
 * 	u2u work off Mothra, before 1.2 branch is available
 * 	[1996/04/29  21:49 UTC  burati  /main/HPDCE02/mb_mothra8/1]
 * 
 * Revision 1.1.8.3  1996/02/18  23:31:44  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:50  marty]
 * 
 * Revision 1.1.8.2  1995/12/08  21:22:05  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/7  1995/05/15  16:56 UTC  truitt
 * 	Merge to HPDCE02.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts29/1  1995/05/15  16:53 UTC  truitt
 * 	Merge to HPDCE02.
 * 
 * 	HP revision /main/HPDCE02/SEC_migrate_merge/3  1995/05/13  02:40 UTC  greg
 * 	Mergeout from HPDCE02 fto SEC_migrate_merge.
 * 
 * 	HP revision /main/HPDCE02/6  1995/05/01  19:02 UTC  truitt
 * 	Merge era work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts26/1  1995/05/01  19:01 UTC  truitt
 * 	CHFts14301: Allocate memory for the sec_attr_t structures
 * 	dynamically.
 * 
 * 	HP revision /main/HPDCE02/SEC_migrate_merge/2  1995/04/26  18:01 UTC  greg
 * 	pick up chnages to signature of encode_pgo_item_attrs().
 * 
 * 	HP revision /main/HPDCE02/4  1995/02/10  19:25 UTC  truitt
 * 	Merge _e work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts11/1  1995/02/10  19:23 UTC  truitt
 * 	CHFts14285: Make sure the error code convenience
 * 	variable gets set when an error occurs.
 * 
 * 	HP revision /main/HPDCE02/3  1994/12/19  18:11 UTC  truitt
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chk_02/1  1994/12/19  18:10 UTC  truitt
 * 	Changed footprint for modify_era_info() call to
 * 	identify which schema (security or generic) the
 * 	function is to operate on.
 * 
 * 	HP revision /main/HPDCE02/2  1994/12/15  19:51 UTC  truitt
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chk_01/1  1994/12/15  19:49 UTC  truitt
 * 	Change the footprint of the print_attrs function to
 * 	pass in the encoding type.
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/08  17:29 UTC  hanfei
 * 	merge
 * 
 * 	HP revision /main/hanfei_cpi_bl2/1  1994/12/07  19:31 UTC  hanfei
 * 	merge
 * 
 * 	HP revision /main/hanfei_cpi_bl1/1  1994/12/07  17:40 UTC  hanfei
 * 	add dcp_rgy_checkpt_parse_cpi_time()
 * 	[1995/12/08  17:56:31  root]
 * 
 * Revision 1.1.4.50  1994/10/05  21:59:23  salamone
 * 	CR12447 - set _b(sec) to fullname
 * 	[1994/10/05  21:21:40  salamone]
 * 
 * 	Merge up.
 * 	[1994/10/05  19:56:42  salamone]
 * 
 * Revision 1.1.4.49  1994/10/05  19:53:51  kevins
 * 	OT 12464 add and remove protos
 * 	[1994/10/05  19:53:42  kevins]
 * 
 * Revision 1.1.4.48  1994/09/29  18:45:11  truitt
 * 	OT10967: Changed the print_attrs() function to support
 * 	multiple values display on each attribute.
 * 	[1994/09/29  17:36:03  truitt]
 * 
 * Revision 1.1.4.47  1994/09/26  18:41:11  salamone
 * 	CR11409 - Determine security registry group name on the fly.
 * 	[1994/09/26  18:27:23  salamone]
 * 
 * Revision 1.1.4.46  1994/09/23  18:30:08  ajayb
 * 	Define FREE_ATTR_PTRS
 * 	[1994/09/22  20:42:06  ajayb]
 * 
 * Revision 1.1.4.45  1994/09/13  19:12:49  kevins
 * 	OT 11641 list syntax not supported in catalog commands.
 * 	[1994/09/13  18:46:33  kevins]
 * 
 * Revision 1.1.4.44  1994/09/12  17:57:56  kevins
 * 	OT 11968 removed site_master_query declaration
 * 	[1994/09/12  17:57:33  kevins]
 * 
 * Revision 1.1.4.43  1994/09/07  13:24:25  kevins
 * 	OT 12057 removed duplicate attribute declaration 'uuid'
 * 	[1994/09/07  11:26:29  kevins]
 * 
 * Revision 1.1.4.42  1994/09/06  16:21:57  kevins
 * 	OT 11694 added global macros prototypes and variables
 * 	[1994/09/06  15:45:22  kevins]
 * 
 * Revision 1.1.4.41  1994/09/01  19:04:48  truitt
 * 	move schema commands to separate files
 * 	[1994/09/01  18:08:11  truitt]
 * 
 * Revision 1.1.4.40  1994/08/31  12:46:39  truitt
 * 	OT11095: Support for proper removal of extended attributes.
 * 	The check_era and modify_era functions had their prototypes
 * 	change.
 * 	[1994/08/30  20:45:45  truitt]
 * 
 * Revision 1.1.4.39  1994/08/29  14:21:00  ajayb
 * 	Add proto for rgy_account_generate().
 * 	Delete randompwd field from acct struct
 * 	and attriute list.
 * 	[1994/08/26  20:37:03  ajayb]
 * 
 * Revision 1.1.4.38  1994/08/24  20:24:23  kevins
 * 	OT 11659 supplied mutex locks for context modifcations
 * 	[1994/08/24  17:55:12  kevins]
 * 
 * Revision 1.1.4.37  1994/08/23  21:20:56  kevins
 * 	OT 11789 Added functions for registry connect
 * 	[1994/08/23  20:54:50  kevins]
 * 
 * 	merge up
 * 	[1994/08/22  19:45:52  kevins]
 * 
 * Revision 1.1.4.36  1994/08/22  19:11:55  pwang
 * 	Merged with changes from 1.1.4.35
 * 	[1994/08/22  19:11:25  pwang]
 * 
 * 	Added definitions of era_info_t and modify_era_info()
 * 	[OT#11424]
 * 
 * Revision 1.1.4.35  1994/08/19  18:14:31  truitt
 * 	Merged with changes from 1.1.4.34
 * 	[1994/08/19  18:13:44  truitt]
 * 
 * 	Added missing schema modify function prototype.
 * 	[1994/08/19  16:11:22  truitt]
 * 
 * Revision 1.1.4.34  1994/08/19  16:55:35  ajayb
 * 	Add prototype for free_account_ptrs(). This frees
 * 	memory obtained by parse_account_attributes()
 * 	[1994/08/18  21:49:08  ajayb]
 * 
 * Revision 1.1.4.33  1994/08/17  14:29:51  truitt
 * 	OT11691: Changed prototype for xattrschema catalog function to
 * 	         support -simplename option.
 * 	[1994/08/16  21:03:15  truitt]
 * 
 * Revision 1.1.4.32  1994/08/15  15:23:45  ajayb
 * 	Changed type of randompwd from char* to boolean32.
 * 	[1994/08/15  13:53:30  ajayb]
 * 
 * Revision 1.1.4.31  1994/08/12  20:03:42  kevins
 * 	OT 9691 Add registry connect command
 * 	[1994/08/12  20:00:18  kevins]
 * 
 * 	Merge up
 * 	[1994/08/11  10:05:29  kevins]
 * 
 * Revision 1.1.4.30  1994/08/10  21:19:38  jane
 * 	Merged with changes from 1.1.4.29
 * 	[1994/08/10  21:19:25  jane]
 * 
 * 	added random password generation functionality
 * 	[1994/08/10  20:59:17  jane]
 * 
 * Revision 1.1.4.29  1994/08/08  18:00:51  kevins
 * 	OT 11071 - Modified the registry cache so that it now stores two
 * 	           cache handles. One  for use with the sec_id_parse_name routine
 * 	           and one that is dynamically changing with the input from the
 * 	           user.
 * 	[1994/08/08  15:39:20  kevins]
 * 
 * Revision 1.1.4.28  1994/07/26  18:54:10  kevins
 * 	OT 9691 Added the 'name get' command
 * 	[1994/07/26  18:51:15  kevins]
 * 
 * Revision 1.1.4.27  1994/07/20  20:15:03  nav
 * 	Merged with changes from 1.1.4.26
 * 	[1994/07/20  20:14:53  nav]
 * 
 * 	Added support for schema modify and schema rename.
 * 	[1994/07/20  19:28:06  nav]
 * 
 * Revision 1.1.4.26  1994/07/20  19:27:58  salamone
 * 	Add new bind type; site_master_query.
 * 	[1994/07/20  19:19:51  salamone]
 * 
 * Revision 1.1.4.25  1994/07/19  18:28:00  truitt
 * 	OT9691: Support for schema delete.
 * 	[1994/07/19  17:32:09  truitt]
 * 
 * Revision 1.1.4.24  1994/07/14  18:30:25  truitt
 * 	OT11186: Support for schema create
 * 	[1994/07/14  17:32:30  truitt]
 * 
 * Revision 1.1.4.23  1994/07/07  20:53:42  coren
 * 	Corrected signature for encode_rgy_pgo_item_attr.
 * 	[1994/07/07  16:05:33  coren]
 * 
 * 	Added Tcl_Interp argument to get_attr_values
 * 	[1994/07/05  21:43:30  coren]
 * 
 * Revision 1.1.4.22  1994/07/07  13:50:38  truitt
 * 	OT11185: Support for schema catalog.  Added two
 * 	new function headers.
 * 	[1994/07/07  11:50:50  truitt]
 * 
 * Revision 1.1.4.21  1994/07/05  15:45:35  kevins
 * 	Merged with changes from 1.1.4.20
 * 	[1994/07/05  15:45:16  kevins]
 * 
 * 	OT11118 registry policy attributes are now show and modifyable
 * 	[1994/07/05  15:44:13  kevins]
 * 
 * Revision 1.1.4.20  1994/07/01  18:00:39  truitt
 * 	OT11162: Support for schema show.  Added two
 * 	new function headers.
 * 	[1994/07/01  17:28:35  truitt]
 * 
 * Revision 1.1.4.19  1994/06/28  13:42:37  truitt
 * 	Merged with changes from 1.1.4.18
 * 	[1994/06/28  13:42:24  truitt]
 * 
 * 	OT11061: Support for pgo modify (all phases) of extended
 * 	attributes.  Added "modify_type" variables and a define
 * 	for a new era function.
 * 	[1994/06/27  18:28:28  truitt]
 * 
 * Revision 1.1.4.18  1994/06/23  19:21:59  kevins
 * 	OT 9691 Added name parse command
 * 	[1994/06/23  18:43:24  kevins]
 * 
 * Revision 1.1.4.17  1994/06/09  16:02:19  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:47:52  devsrc]
 * 
 * Revision 1.1.4.16  1994/06/07  19:16:40  kevins
 * 	OT 10784 modified integer evalutation functions
 * 	[1994/06/07  19:16:03  kevins]
 * 
 * Revision 1.1.4.15  1994/06/02  15:55:11  salamone
 * 	Moved and renamed  rgy_modify() and rgy_show() functions in
 * 	rgy.c to rgy_sa_tclif.c
 * 	[1994/06/02  15:46:29  salamone]
 * 
 * Revision 1.1.4.14  1994/06/02  13:36:49  truitt
 * 	Merged with changes from 1.1.4.13
 * 	[1994/06/02  13:36:30  truitt]
 * 
 * 	OT 10800: Initial work for extended registry attribute
 * 	support.  Three new function prototypes added and two
 * 	existing prototypes changed.
 * 	[1994/05/27  18:34:01  truitt]
 * 
 * Revision 1.1.4.13  1994/05/26  17:56:31  salamone
 * 	Added another flag to open_rgy_site() to bind to a specific
 * 	registry site for update.
 * 	[1994/05/26  16:37:55  salamone]
 * 
 * Revision 1.1.4.12  1994/05/20  11:26:21  kevins
 * 	10663 - Provided the ability to pass a name for the registry to open.
 * 	[1994/05/20  08:58:57  kevins]
 * 
 * Revision 1.1.4.11  1994/05/12  13:04:10  kevins
 * 	OT 10612 removed redefined macros.
 * 	[1994/05/12  11:37:51  kevins]
 * 
 * Revision 1.1.4.10  1994/05/11  19:28:37  kevins
 * 	OT 10595 modified SITE_VAR for _s(sec)
 * 	[1994/05/11  19:28:23  kevins]
 * 
 * Revision 1.1.4.9  1994/05/10  10:00:16  kevins
 * 	OT 9691 Add registry show/modify commands
 * 	[1994/05/09  19:16:02  kevins]
 * 
 * 	[1994/05/09  19:16:02  kevins]
 * 
 * Revision 1.1.4.8  1994/04/19  16:27:03  kevins
 * 	CR 10381 added forwardabletkt and pwdvalid as account attributes
 * 	[1994/04/19  10:49:58  kevins]
 * 
 * Revision 1.1.4.7  1994/04/08  10:40:15  kevins
 * 	OT 9691 Implemented account commands
 * 	[1994/04/08  10:39:30  kevins]
 * 
 * Revision 1.1.4.6  1994/03/22  16:14:12  kevins
 * 	CR 10198 check the attribute name uid/gid/orgidd
 * 	[1994/03/22  16:07:46  kevins]
 * 
 * Revision 1.1.4.5  1994/03/21  23:52:20  kevins
 * 	CR 10097 modify commands should not be able to alter unix_num
 * 	[1994/03/21  23:51:09  kevins]
 * 
 * Revision 1.1.4.4  1994/03/14  17:39:46  kevins
 * 	CR9904 Added support for help system
 * 	[1994/03/14  13:44:30  kevins]
 * 
 * Revision 1.1.4.3  1994/02/14  15:34:10  kevins
 * 	Added _RGY_H to protect from multi inclusions
 * 	[1994/02/11  23:12:31  kevins]
 * 
 * Revision 1.1.4.2  1994/02/03  17:24:13  kevins
 * 	added support for group and org objects
 * 	[1994/02/03  17:20:06  kevins]
 * 
 * Revision 1.1.4.1  1994/01/05  16:30:32  kevins
 * 	Added rgy group commads.
 * 	[1994/01/05  16:03:13  kevins]
 * 
 * Revision 1.1.2.4  1993/12/13  12:48:03  kevins
 * 	Completed principal commands
 * 	[1993/12/13  12:43:29  kevins]
 * 
 * Revision 1.1.2.3  1993/11/23  13:22:35  kevins
 * 	cleaned file
 * 	[1993/11/23  13:20:51  kevins]
 * 
 * Revision 1.1.2.2  1993/11/15  15:01:51  kevins
 * 	 added protos for princ show command
 * 	[1993/11/15  15:00:05  kevins]
 * 
 * Revision 1.1.2.1  1993/11/05  19:52:56  kevins
 * 	new file for sec commands
 * 	[1993/11/05  19:40:49  kevins]
 * 
 * $EndLog$
 */
#ifndef _RGY_H
#define _RGY_H

#include <dce/passwd.h>
#include <dce/rplbase.h>
#include <dce/sec_pk_base.h>
#include <dce/sec_pvtkey.h>
#include <dce/sec_psm.h>
#include <dce/sec_psm_base.h>
#include <dce/sec_pk.h>

/* PROTOTYPES */
int  principal_create(ClientData, Tcl_Interp *, int , char **);
int  principal_delete(ClientData, Tcl_Interp *, int , char **);
int  principal_catalog(ClientData, Tcl_Interp *, int , char **);
int  principal_modify(ClientData, Tcl_Interp *, int , char **);
int  principal_rename(ClientData, Tcl_Interp *, int , char **);
int  principal_show(ClientData, Tcl_Interp *, int , char **);
int  rgy_group_add(ClientData, Tcl_Interp *, int , char **);
int  rgy_group_create(ClientData, Tcl_Interp *, int , char **);
int  rgy_group_modify(ClientData, Tcl_Interp *, int , char **);
int  rgy_group_delete(ClientData, Tcl_Interp *, int , char **);
int  rgy_group_list(ClientData, Tcl_Interp *, int , char **);
int  rgy_group_catalog(ClientData, Tcl_Interp *, int , char **);
int  rgy_group_show(ClientData, Tcl_Interp *, int , char **);
int  rgy_group_rename(ClientData, Tcl_Interp *, int , char **);
int  rgy_group_remove(ClientData, Tcl_Interp *, int , char **);
int  rgy_org_add(ClientData, Tcl_Interp *, int , char **);
int  rgy_org_create(ClientData, Tcl_Interp *, int , char **);
int  rgy_org_modify(ClientData, Tcl_Interp *, int , char **);
int  rgy_org_delete(ClientData, Tcl_Interp *, int , char **);
int  rgy_org_list(ClientData, Tcl_Interp *, int , char **);
int  rgy_org_catalog(ClientData, Tcl_Interp *, int , char **);
int  rgy_org_show(ClientData, Tcl_Interp *, int , char **);
int  rgy_org_rename(ClientData, Tcl_Interp *, int , char **);
int  rgy_org_remove(ClientData, Tcl_Interp *, int , char **);
int  rgy_account_catalog(ClientData, Tcl_Interp *, int , char **);
int  rgy_account_show(ClientData, Tcl_Interp *, int , char **);
int  rgy_account_create(ClientData, Tcl_Interp *, int , char **);
int  rgy_account_delete(ClientData, Tcl_Interp *, int , char **);
int  rgy_account_modify(ClientData, Tcl_Interp *, int , char **);
int  rgy_account_generate(ClientData, Tcl_Interp *, int , char **);
int  schema_show(ClientData, Tcl_Interp *, int, char **);
int  schema_catalog(ClientData, Tcl_Interp *, int, char **);
int  schema_create(ClientData, Tcl_Interp *, int, char **);
int  schema_delete(ClientData, Tcl_Interp *, int, char **);
int  schema_modify(ClientData, Tcl_Interp *, int, char **);
int  schema_rename(ClientData, Tcl_Interp *, int, char **);
int  name_parse(ClientData, Tcl_Interp *, int , char **);
int  name_get(ClientData, Tcl_Interp *, int , char **);

int is_simple_name(int, char **);
void sec_to_date_string(sec_timeval_sec_t, char *);
void sec_to_rel_date_string(sec_timeval_sec_t, char *);
int add_acctlife(char *, sec_rgy_plcy_t *);
void init_pgo_item(sec_rgy_pgo_item_t *);
void init_policy(sec_rgy_plcy_t *);
void copy_policy_with_restrict(sec_rgy_plcy_t *, sec_rgy_plcy_t *);

int dcp_id_parse_name(Tcl_Interp *, 
                      sec_rgy_name_t, 
                      sec_rgy_name_t, 
                      sec_rgy_name_t);
                             
int add_pwdlife(char *, sec_rgy_plcy_t *);
int add_pgo_flag(char *, sec_rgy_pgo_item_t *, int );
int add_policy_flag(char *, sec_rgy_plcy_t *, int );
int add_pwdminlen(char *, sec_rgy_plcy_t *);
int add_pwdexpdate(char *, sec_rgy_plcy_t *);

int is_number(char *);

void parse_policy_switch_list(char *, char *, char *, char *, char *, 
                           char *, sec_rgy_plcy_t *, unsigned32 *);

int check_uid_validity(sec_rgy_domain_t,
                       unsigned32,
                       char *);
int add_pgo_entry(void);
int date_to_seconds(sec_timeval_sec_t *, char *);
int reldate_to_seconds(sec_timeval_sec_t *, char *);
int format_registry_prop(Tcl_Interp *,
                         boolean32,
                         boolean32,
                         sec_rgy_properties_t *,
                         rs_sw_version_t *,
                         sec_rgy_plcy_t *,
                         sec_rgy_plcy_auth_t *);
void init_rgy_attribute_table(Tcl_Interp *);
void init_schema_attribute_table(Tcl_Interp *);
int add_new_group_org(sec_rgy_handle_t,
                      sec_rgy_login_name_t,
                      sec_rgy_domain_t,
                      boolean32 *,
                      error_status_t *);
int add_cell_account(sec_rgy_handle_t,
                     sec_rgy_login_name_t,
                     sec_passwd_rec_t,
                     sec_passwd_rec_t *);
int replace_cell_account(sec_rgy_handle_t,
                         sec_rgy_login_name_t,
                         sec_passwd_rec_t,
                         sec_passwd_rec_t *);

/*
 * MACRO definitions
 */
#define SITE_VAR "sec"

#define DCP_SET_SEC_ERROR(code,param) {                                     \
        Tcl_SetResult(interp,(char *)dce_sprintf(code,param), TCL_DYNAMIC); \
        DCP_GENERAL_SET_E_VAR(code);                                        \
        return TCL_ERROR;                                                   \
}

#define DCP_CHECK_RGY_NO_ARG(type)                                  \
    if(argc < 2){                                                   \
        char *temp;                                                 \
        temp = (char *)dce_sprintf(dcp_s_rgy_no_ ## type ## _name); \
        Tcl_SetResult(interp, temp, TCL_DYNAMIC);                   \
        DCP_GENERAL_SET_E_VAR(dcp_s_rgy_no_ ## type ## _name);      \
        return TCL_ERROR;                                           \
    }                                                               \

#define FREE_ACCT_PTRS(acct) {                                      \
    if (rls_mypwd_mem) {                                            \
        free ((char *) (acct)->mypwd);                              \
        (acct)->mypwd = NULL;                                       \
        }                                                           \
    if (attribute) free_account_ptrs ((acct));                      \
    }
   
#define FREE_ATTR_PTRS(era_count,attrs) {                           \
    int i;                                                          \
    if ((era_count))                                                \
        for (i=0;i<(era_count);i++)                                 \
            sec_attr_util_inst_free_ptrs(&attrs[i]);                \
    }

#define FREE_PK_PTRS(pkinfo) {                                          \
    if (pkinfo.publickeyfile) free((char *)pkinfo.publickeyfile);       \
    if (pkinfo.privatekeyfile) free((char *)pkinfo.privatekeyfile);     \
    if (pkinfo.oldpassphrase) free((char *)pkinfo.oldpassphrase);       \
    if (pkinfo.newpassphrase) free((char *)pkinfo.newpassphrase);       \
}

typedef unsigned_char_t  site_name_t[1025];

/*
 * represents possible password policies contained in pwd_val_type ERA
 */
typedef enum {
    none,
    user_select,
    user_can_select,
    generation_required
} ek_pwd_val_type_t;

sec_rgy_handle_t   rgy_context;
sec_rgy_handle_t   rgy_default_context;

/*
 * The "site_list" mutex is to prorvide a mutex lock on the  
 * site cache.
 * 
 * site_access_t refers to binding to the proper registry site:
 *     site_update             Bind to master replica for update
 *     site_query              Bind to arbitrary replica for query
 *     site_specific           Bind to specific replica for update
 *     site_default            Bind to the local cell unauthenticated
 *
 * site_cache_type_t defines the registry site cache we maintain.
 *     default_cache           contains info on the local cell regi unauth
 *     realtime_cache          contains info on the current open registry 
 *
 * For the dcecp registry commands:
 *     registry catalog        Bind to arbitrary replica for query
 *     registry delete         Bind to master replica for update
 *     registry delete -force  Bind to master replica for update
 *     registry delete -only   Bind to specific replica for update
 *     registry disable        Bind to master replica for update
 *     registry dump           Bind to arbitrary replica for query
 *     registry enable         Bind to master replica for update
 *     registry modify         Bind to master replica for update
 *     registry modify -key    Bind to specific replica for update
 *     registry show           Bind to arbitrary replica for query
 *     registry show -master   Bind to master replica for query
 *     registry stop           Bind to specific replica for update
 *     registry synchronize    Bind to master replica for update
 *     registry verify         Bind to arbitrary replica for query
 */
pthread_mutex_t site_list;

typedef enum { 
    site_update, 
    site_query, 
    site_specific,
    site_default
} site_access_t;

typedef enum {default_cache, realtime_cache} site_cache_type_t;

typedef struct rgy_site_cache_t {
    site_name_t      site;
    site_name_t      cell_name;
    site_access_t    access_type;
    sec_rgy_handle_t cache_context;
    boolean32        valid;         /* has a site been opened? */
} rgy_site_cache_t;

rgy_site_cache_t site_cache[2];

#define LOCAL_CELL "/.:"
#define XATTRSCHEMA "xattrschema"

/*
 * expands to true if the specified cache is valid, false otherwise
 */
#define SITE_CACHE_VALID(cache) ((cache).valid == true)

typedef struct rgy_attribute_info_t {
    sec_rgy_domain_t domain;
    sec_rgy_pgo_item_t *pgo_item;
    boolean32        policy;
    boolean32        pgo;
    boolean32        types;
    int              era_count;
    int              del_count;
    int              modify_type;
    unsigned32       flags;
    char             *fullname;
} rgy_attribute_info_t; 

typedef struct rgy_policy_info_t {
    sec_rgy_properties_t *properties;
    sec_rgy_plcy_t       *policy;
    sec_rgy_plcy_auth_t  *auth_policy;
    char                 *list;
    char                 *deftktlife;
    char                 *mingid;
    char                 *minorgid;
    char                 *minuid;
    char                 *maxuid;
    char                 *mintktlife;
    char                 *hidepwd;
    char                 *version;
    char                 *acctlife;
    char                 *pwdminlen;
    char                 *pwdlife;
    char                 *expdate;
    char                 *pwdspaces;
    char                 *pwdalpha;
    char                 *maxtktlife;
    char                 *maxtktrenew;
} rgy_policy_info_t;

typedef struct rgy_pubkey_info_t {
    int            generatekey;
    char           *publickeyfile;
    char           *privatekeyfile;
    char           *oldpassphrase;
    char           *newpassphrase;
} rgy_pubkey_info_t;

typedef struct rgy_acct_info_t {
    sec_rgy_login_name_t account_name;
    sec_rgy_sid_t        sid;
    sec_rgy_unix_sid_t   unix_sid;
    sec_rgy_acct_key_t   acct_key;
    sec_rgy_acct_user_t  acct_user;
    sec_rgy_acct_admin_t acct_admin;
    sec_rgy_plcy_auth_t  policy;
    sec_passwd_rec_t     passwd;
    sec_passwd_rec_t     adm_passwd;
    char                 *acctvalid;
    char                 *client;
    char                 *created;
    char                 *description;
    char                 *dupkey;
    char                 *expdate;
    char                 *forwardabletkt;
    char                 *goodsince;
    char                 *group;
    char                 *home;
    char                 *lastchange;
    char                 *mypwd;
    char                 *organization;
    char                 *password;
    char                 *postdatedtkt;
    char                 *proxiabletkt;
    char                 *pwdvalid;
    char                 *renewabletkt;
    char                 *server;
    char                 *shell;
    char                 *stdtgtauth;
    char                 *maxtktlife;
    char                 *maxtktrenew;
    char                 *usertouser;
    char                 *pkmechanism;
    char                 *pkgenprivkey;
    int                  pkgenprivkey_value;
} rgy_acct_info_t;

typedef struct rgy_connect_info_t {
    char    *foreign_principal;
    char    *local_principal;
    char    *expdate;
    char    *facct;
    char    *fgroup;
    char    *facctpwd;
    char    *forg;
    char    *group;
    char    *mypwd;
    char    *org;
} rgy_connect_info_t;

#define pgo_modify  0x2
#define pgo_create  0x4

#define modify_type_none 0
#define modify_type_add 1
#define modify_type_change 2
#define modify_type_remove 3

#define pk_genkey_default_value 0
#define pk_generate_default_value 9999

/*
 * rgy attribute definition
 */
#define rgy_attr_name             "name"
#define rgy_attr_uuid             "uuid"
#define rgy_attr_uid              "uid" 
#define rgy_attr_gid              "gid" 
#define rgy_attr_orgid            "orgid"
#define rgy_attr_fullname         "fullname" 
#define rgy_attr_alias            "alias" 
#define rgy_attr_quota            "quota" 
#define rgy_attr_inprojlist       "inprojlist" 
#define rgy_attr_pwdlife          "pwdlife"
#define rgy_attr_pwdminlen        "pwdminlen"
#define rgy_attr_pwdexpdate       "pwdexpdate"
#define rgy_attr_pwdspaces        "pwdspaces"
#define rgy_attr_pwdalpha         "pwdalpha"
#define rgy_attr_acctlife         "acctlife"
#define rgy_attr_acctvalid        "acctvalid"
#define rgy_attr_client           "client"
#define rgy_attr_created          "created"
#define rgy_attr_description      "description"
#define rgy_attr_dupkey           "dupkey"
#define rgy_attr_expdate          "expdate"
#define rgy_attr_goodsince        "goodsince"
#define rgy_attr_group            "group"
#define rgy_attr_home             "home"
#define rgy_attr_lastchange       "lastchange"
#define rgy_attr_organization     "organization"
#define rgy_attr_postdatedtkt     "postdatedtkt"
#define rgy_attr_proxiabletkt     "proxiabletkt"
#define rgy_attr_renewabletkt     "renewabletkt"
#define rgy_attr_server           "server"
#define rgy_attr_shell            "shell"
#define rgy_attr_stdtgtauth       "stdtgtauth"
#define rgy_attr_maxtktlife       "maxtktlife"
#define rgy_attr_maxtktrenew      "maxtktrenew"
#define rgy_attr_password         "password"
#define rgy_attr_mypwd            "mypwd"
#define rgy_attr_forwardabletkt   "forwardabletkt"
#define rgy_attr_pwdvalid         "pwdvalid"
#define rgy_attr_deftktlife       "deftktlife"
#define rgy_attr_mingid           "mingid"
#define rgy_attr_minorgid         "minorgid"
#define rgy_attr_minuid           "minuid"
#define rgy_attr_maxuid           "maxuid"
#define rgy_attr_mintktlife       "mintktlife"
#define rgy_attr_hidepwd          "hidepwd"
#define rgy_attr_version          "version"
#define rgy_attr_usertouser       "usertouser"
#define rgy_attr_pksignatureusage "pksignatureusage"
#define rgy_attr_pkkeycipherusage "pkkeycipherusage"
#define rgy_attr_pkgenprivkey     "pkgenprivkey"
#define rgy_attr_pkmechanism      "pkmechanism"
#define pk_attr_generatekey       "generatekey"
#define pk_attr_privatekeyfile    "privatekeyfile"
#define pk_attr_publickeyfile     "publickeyfile"
#define pk_attr_oldpassphrase     "oldpassphrase"
#define pk_attr_newpassphrase     "newpassphrase"
#define pk_attr_mechanism_file    "file"
#define pk_attr_mechanism_pkss    "pkss"
#define pk_genkey_default_string  "default"
#define schema_attr_encoding      "encoding"
#define schema_attr_aclmgr        "aclmgr"
#define schema_attr_unique        "unique"
#define schema_attr_multivalued   "multivalued"
#define schema_attr_reserved      "reserved"
#define schema_attr_applydefs     "applydefs"
#define schema_attr_intercell     "intercell"
#define schema_attr_trigtype      "trigtype"
#define schema_attr_trigbind      "trigbind"
#define schema_attr_scope         "scope"
#define schema_attr_annotation    "annotation"

/*
 * Person, Group, Org and account attributes
 */
typedef enum {
    rgy_pgo_name,
    rgy_pgo_uuid,
    rgy_pgo_unix_num,
    rgy_pgo_fullname,
    rgy_pgo_reserved,
    rgy_pgo_is_an_alias,
    rgy_pgo_quota,
    rgy_pgo_inprojlist,
    rgy_plcy_pwdlife,
    rgy_plcy_pwdminlen,
    rgy_plcy_pwdexpdate,
    rgy_plcy_pwdspaces,
    rgy_plcy_pwdalpha,
    rgy_plcy_acctlife,
    rgy_plcy_version,
    rgy_acct_acctvalid,
    rgy_acct_client,
    rgy_acct_created,
    rgy_acct_description,
    rgy_acct_dupkey,
    rgy_acct_expdate,
    rgy_acct_goodsince,
    rgy_acct_group,
    rgy_acct_home,
    rgy_acct_lastchange,
    rgy_acct_organization,
    rgy_acct_postdatedtkt,
    rgy_acct_proxiabletkt,
    rgy_acct_renewabletkt,
    rgy_acct_server,
    rgy_acct_shell,
    rgy_acct_stdtgtauth,
    rgy_acct_plcy_maxtktlife,
    rgy_acct_plcy_maxtktrenew,
    rgy_acct_password,
    rgy_acct_mypwd,
    rgy_acct_forwardabletkt,
    rgy_acct_pwdvalid,
    rgy_deftktlife,
    rgy_mingid,
    rgy_minorgid,
    rgy_minuid,
    rgy_maxuid,
    rgy_mintktlife,
    rgy_acct_usertouser,
    rgy_acct_pksignatureusage,
    rgy_acct_pkkeycipherusage,
    rgy_acct_pkgenprivkey,
    rgy_acct_pkmechanism,
    rgy_hidepwd
} rgy_attributes;

/*
 * xattrschema attributes  The attributes must start enum sequence at 50 to 
 * differentiate them from the pgo and account attrs. This is because of one 
 * attribute name overlap "uuid".
 */
typedef enum {
    schema_encoding = 50,
    schema_aclmgr,
    schema_unique,
    schema_multivalued,
    schema_reserved,
    schema_applydefs,
    schema_intercell,
    schema_trigtype,
    schema_trigbind,
    schema_scope,
    schema_annotation
} schema_attributes;
 
/*
 * Legal schema attribute encoding types.
 */
#define MAX_ENCODING 11

static char *enc_type_name[] = {"any",
                                "void",
                                "integer",
                                "printstring",
                                "stringarray",
                                "byte",
                                "confidential",
                                "i18ndata",
                                "uuid",
                                "attrset",
                                "binding"};

/*
 * Account data fields
 */
#define rgy_acct_pname               0   /* login name */
#define rgy_acct_gname               1   /* group name */
#define rgy_acct_oname               2   /* organization name */
#define rgy_acct_key_parts           3   /* key info */
#define rgy_acct_sid_person          4   /* person uuid */
#define rgy_acct_sid_group           5   /* group uuid */
#define rgy_acct_sid_org             6   /* org uuid */
#define rgy_acct_unix_sid_person     7   /* person unix id */
#define rgy_acct_unix_sid_group      8   /* group unix id */
#define rgy_acct_unix_sid_org        9   /* org unix id */

#define dcp_rgy_not_int              0
#define dcp_rgy_ok_int               1
#define dcp_rgy_int_out_of_range     2

/*
 * KERNEL OPERATIONS AND DATA FIELDS
 *
 * Data field ids are actually zero origin offsets to a vector of fields stored
 * in an operation handle.  By convention, a key field definition is always the
 * last field in every set of data fields, so it can be used as a symbolic
 * sentinel value.   Do not change the definitions of field ids unless you know
 * exactly what you're doing,  otherwise, things could get real ugly.  The "n"
 * in the "rgy_" prefix indicates that  the definitions apply to data/operations
 * in/on the network registry database.
 */
#define rgy_pgo_alias_found      0x40
#define rgy_pgo_inprojlist_found 0x20
#define rgy_pgo_fullname_found   0x10
#define rgy_plcy_spaces_found    0x10
#define rgy_plcy_alpha_found     0x20

/*
 * Default Values for properties
 */
#define org_pol_never_expire 0        /* a clockh_t value that the kernel interprets
                                         as a null expiration date */
#define org_pol_live_forever 0        /* infinite lifespan */

/*
 * Person specific fields
 */
#define rgy_pgo_is_an_alias       5   /* ek_flag_buf_t */
#define rgy_pgo_quota             6

/*
 * Policy Field Ids
 */
#define rgy_plcy_max_date_len      16
#define rgy_plcy_null_date         "0"

/*
 * Registry properties operations and data - no key field required because
 * there's only logically) one registry
 */
#define rgy_prop_is_readonly                    0
#define rgy_prop_auth_certificates_unbounded    1
#define rgy_prop_shadow_passwd                  2
#define rgy_prop_embedded_unix_id               3
#define rgy_prop_minimum_certificate_lifetime   4
#define rgy_prop_default_certificate_lifetime   5
#define rgy_prop_low_unix_id_person             6
#define rgy_prop_low_unix_id_group              7
#define rgy_prop_low_unix_id_org                8
#define rgy_prop_max_unix_id                    9
#define rgy_prop_realm                          10
#define rgy_prop_last_field                     rgy_prop_realm
#define rgy_prop_num_fields                     (rgy_prop_last_field + 1)

/*
 * PROPERTY operations
 */
#define  rgy_prop_rgy_get 70
#define  rgy_prop_rgy_set 71

/*
 * Local registry properties operations and data
 */
#define rgy_prop_lrgy_max_entries               0
#define rgy_prop_lrgy_num_entries               1
#define rgy_prop_lrgy_num_tgt_entries           2
#define rgy_prop_lrgy_lifespan                  3
#define rgy_prop_lrgy_last_field                rgy_prop_lrgy_lifespan
#define rgy_prop_lrgy_num_fields                (rgy_prop_lrgy_last_field + 1)

/*
 * Local registry property operations
 */
#define  rgy_prop_lrgy_get 72
#define  rgy_prop_lrgy_set 73

/*
 * bit masks for operation field attributes
 */
#define attribute_readonly_mask   0x1  /* field not writable by interface */
#define attribute_readwrite_mask  0x2  /* ok for interface to write */
#define attribute_required_mask   0x4  /* can't perform operation unless the field was written */
#define attribute_unused_mask     0x8  /* field not used by this operation */  

/*
 * era_info_t contains the information about a ERA
 * attribute defined and will be used for the
 * communication among the internal routines so that
 * the number of server invocation can be kept in
 * minimum.   Currently it only contains attr uuid
 * and schema_entry_flags.
 */
typedef struct era_info_s_t {
    uuid_t                      attr_id;
    sec_attr_sch_entry_flags_t  schema_entry_flags;
} era_info_t;

/*
 * PROTOTYPES
 */
int print_attrs(Tcl_Interp *, sec_attr_t *, sec_attr_encoding_t);
int get_attr_values(Tcl_Interp *, sec_attr_encoding_t, uuid_t, sec_attr_t *,
                    int *, char **);
error_status_t show_era(Tcl_Interp *, signed32, sec_rgy_name_t);
int show_schema_info(Tcl_Interp *, char *);
int catalog_schema_info(Tcl_Interp *, char *, char *);
void split_schema_name(char *, char *, char *, error_status_t *);
void assign_permset(char *, int, sec_acl_permset_t *, unsigned32 *);
error_status_t check_era_info(Tcl_Interp *, signed32, sec_rgy_name_t,
                              sec_attr_t *, rgy_attribute_info_t *,
                              sec_attr_t *, sec_attr_t *);
error_status_t modify_era_info(Tcl_Interp *, boolean32, unsigned32, unsigned32,
                               boolean32, sec_attr_t *, era_info_t *, unsigned32,
                               sec_attr_t *, unsigned32 *, sec_attr_t *,
                               unsigned32 *, sec_attr_t *);

int update_site_cache_contents(Tcl_Interp *,
                               site_name_t , 
                               site_access_t,
                               sec_rgy_handle_t);
int open_site_by_access(Tcl_Interp *, site_name_t , site_access_t *);
int mod_rgy_quota(char *, sec_rgy_pgo_item_t *);
int change_rgy_name(sec_rgy_name_t , char *, sec_rgy_domain_t);
int add_uid(char *, sec_rgy_pgo_item_t *);
int is_yes(char *);
int rgy_attribute(char *);
int schema_attribute(char *);
void encode_rgy_plcy_attr(char *, char *, sec_rgy_plcy_t *,
                          rgy_attribute_info_t *, unsigned32 *);
void encode_rgy_pgo_item_attr(Tcl_Interp *, char **, int *,
                              sec_rgy_pgo_item_t *,
                              rgy_attribute_info_t *, sec_attr_t *, int *, unsigned32 *);
int parse_attribute_list(Tcl_Interp *, char *, sec_rgy_pgo_item_t *,
                         sec_rgy_plcy_t *, rgy_attribute_info_t *, sec_attr_t *);
void initialize_account_attr(rgy_acct_info_t *);
int parse_account_switches(Tcl_Interp *, rgy_acct_info_t *);
int add_acct_admin_flag(sec_rgy_acct_admin_flags_t *,
                        sec_rgy_acct_admin_flags_t,
                        char *);
int add_acct_auth_flag(sec_rgy_acct_auth_flags_t *,
                       sec_rgy_acct_auth_flags_t,
                       char *);
int add_acct_user_flag(sec_rgy_acct_user_flags_t *,
                       sec_rgy_acct_user_flags_t,
                       char *);
int parse_account_attributes(Tcl_Interp *, 
                             rgy_acct_info_t *, 
                             char *, char *,
                             char *);
void copy_pgo_item_with_restrict(sec_rgy_pgo_item_t *, 
                                 sec_rgy_pgo_item_t *,
                                 rgy_attribute_info_t *);
void parse_item_switch_list(char *, char *, char *, char *, char *, 
                            char *, sec_rgy_pgo_item_t *,
                            sec_rgy_domain_t,
                            rgy_attribute_info_t *,
                            unsigned32 *);
void encode_schema_attr(Tcl_Interp *, char **, int *, sec_attr_schema_entry_t *,
                        unsigned32 *);
void encode_modify_schema_attr(Tcl_Interp *, char **, int *, sec_attr_schema_entry_t *,
                               sec_attr_schema_entry_parts_t *, unsigned32 *);
void parse_schema_attr_list(Tcl_Interp *, char *, sec_attr_schema_entry_t *,
                            unsigned32 *);
void parse_schema_switch_list(Tcl_Interp *, char *, char *, char *, char *,
                              char *, char *, char *, char *, char *, char *,
                              char *, char *, sec_attr_schema_entry_t *,
                              unsigned32 *);
 
void parse_modify_schema_switch_list(Tcl_Interp *, char *, char *, char *,
                              char *, char *, char *,
                              sec_attr_schema_entry_t *,
                              sec_attr_schema_entry_parts_t *,
                              unsigned32 *);
void parse_modify_schema_attr_list(Tcl_Interp *, char *,
                              sec_attr_schema_entry_t *,
                              sec_attr_schema_entry_parts_t *,
                              unsigned32 *);
void free_account_ptrs (rgy_acct_info_t *);
int site_close(site_cache_type_t);
int update_registry_properties(Tcl_Interp *, rgy_policy_info_t *);
int parse_registry_prop_list(Tcl_Interp *, rgy_policy_info_t *);
void free_policy_attr(rgy_policy_info_t *);
int open_rgy_site(Tcl_Interp *, site_name_t, site_access_t);
int build_proper_aclmgr_set(Tcl_Interp *, char *, char *, char *, sec_attr_schema_entry_t *);
int dcp_schema_read_group_name(Tcl_Interp *, unsigned_char_p_t *);
int dcp_rgy_checkpt_parse_cpi_time(Tcl_Interp *, char *, unsigned32 *);

void initialize_pk_attr(rgy_pubkey_info_t *);
int parse_pk_switches(Tcl_Interp *, rgy_pubkey_info_t *, rgy_pubkey_info_t *, char *, char *);
int check_pk_create_arguments(Tcl_Interp *, rgy_pubkey_info_t *, char *);
int check_pk_modify_arguments(Tcl_Interp *, rgy_pubkey_info_t *, char *);
int check_pk_file_contents(Tcl_Interp *, rgy_pubkey_info_t *);

#endif
