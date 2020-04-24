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
 * $Log: rgy_utils.c,v $
 * Revision 1.1.9.3  1996/11/13  17:54:24  arvind
 * 	Merge changes.
 * 	[1996/10/15  15:57 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk7/1]
 *
 * 	CHFts20138: Add the ability to have pk passwords prompted for as opposed
 * 	to forcing the user to enter them on the command line.
 * 	[1996/07/08  17:19 UTC  truitt  /main/DCE_1.2.2/2]
 *
 * 	Merge changes.
 * 	[1996/07/02  17:29 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/3]
 *
 * 	Additional pk work.
 * 	[1996/06/27  18:54 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/2]
 *
 * 	Public key back-end work.
 * 	[1996/06/11  19:35 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/1]
 *
 * 	Some dcecp changes to support Public Key functionality.
 * 	Merge with latest HP fixes and enhancements.
 * 	[1996/03/28  18:21 UTC  truitt  /main/HPDCE02/23]
 *
 * 	Merge changes.
 * 	[1996/03/28  18:19 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf72/1]
 *
 * 	CHFts17975: The change to the access method is incorrect for a couple
 * 	commands.  We have to get the open_site and open_site_by_access
 * 	functions to work correctly, with the correct security api calls.  It would be
 * 	wonderful if the security api calls were returning the binding of an UP replica
 * 	each time, but supposedly there is no guarantee of that.  Unbelievable.  So,
 * 	we've got to jump through some hoops to make sure we make the correct call
 * 	to the correct replica (or cell)  under the correct circumstances, or forget it.
 * 	[1996/03/20  21:07 UTC  jrr  /main/HPDCE02/22]
 *
 * 	Merge changes.
 * 	[1996/02/15  18:17 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf62/1]
 *
 * 	CHFts17451: Remove include of value.h so duplicate macro
 * 	warning does not appear during HP build.  The include is
 * 	required on other platforms.
 * 	[1996/02/05  15:52 UTC  truitt  /main/HPDCE02/20]
 *
 * 	Merge changes.
 * 	[1996/02/05  15:49 UTC  truitt  /main/HPDCE02/truitt_dcecp_17281/1]
 *
 * 	CHFts17281: Fix problem between dcp_id_parse_name() and
 * 	sec_id_parse_name() so the correct parsing is done and the
 * 	correct cell is contacted, based on the input argument.
 * 	CHFts17406: Clean up some of the internal function names.
 * 	[1996/01/31  15:37 UTC  truitt  /main/HPDCE02/19]
 *
 * 	Merge for multiple tower bindings.
 * 	[1996/01/31  15:32 UTC  truitt  /main/HPDCE02/truitt_dcecp_17287/1]
 *
 * 	Merge changes.
 * 	[1995/11/15  21:02 UTC  truitt  /main/HPDCE02/truitt_dcecp_chfts52/1]
 *
 * 	CHFts16836: Make sure the correct instances are freed when
 * 	an error is encountered during an era modification.
 * 	[1995/11/02  21:43 UTC  kline_s  /main/HPDCE02/17]
 *
 * 	Submitted the fix for CHFts16512.
 * 	[1995/09/23  17:48 UTC  tatsu_s  /main/HPDCE02/tatsu_s.dced_local.b0/1]
 *
 * 	Added dhd__schema_ent_free_ptrs() kluge.
 * 	[1995/09/28  17:43 UTC  truitt  /main/HPDCE02/14]
 *
 * 	Merge bytes output change.
 * 	[1995/09/28  17:41 UTC  truitt  /main/HPDCE02/truitt_dcecp_chfts47/1]
 *
 * 	CHFts16476: Output the correct number of bytes per line for a
 * 	bytes-type era.
 * 	[1995/07/12  13:24 UTC  truitt  /main/HPDCE02/13]
 *
 * 	Merge i18n changes.
 * 	[1995/07/12  13:22 UTC  truitt  /main/HPDCE02/truitt_dcecp_chfts38/1]
 *
 * 	CHFts15275: Allow proper parsing of input and output for attributes
 * 	with encoding type i18n_data.
 * 	[1995/06/12  12:58 UTC  truitt  /main/HPDCE02/12]
 *
 * 	Merge.
 * 	[1995/06/12  12:57 UTC  truitt  /main/HPDCE02/truitt_dcecp_chfts34/1]
 *
 * 	CHFts15424: Fix braces in print attrs function.
 * 	[1995/05/23  19:16 UTC  truitt  /main/HPDCE02/11]
 *
 * 	Merge field work.
 * 	[1995/05/23  19:14 UTC  truitt  /main/HPDCE02/truitt_dcecp_chfts30/1]
 *
 * 	CHFts14679: Move string header info from code to sams file.
 * 	[1995/05/15  17:34 UTC  truitt  /main/HPDCE02/10]
 *
 * 	Merge warranty patch work.
 * 	[1995/05/15  17:30 UTC  truitt  /main/HPDCE02/truitt_dcecp_chfts29/1]
 *
 * 	Merge warranty patch work.
 * 	[1995/05/13  02:40 UTC  greg  /main/HPDCE02/SEC_migrate_merge/3]
 *
 * 	Mergeout from HPDCE02 fto SEC_migrate_merge.
 * 	[1995/05/01  19:14 UTC  truitt  /main/HPDCE02/9]
 *
 * 	Merge era changes.
 * 	[1995/05/01  19:08 UTC  truitt  /main/HPDCE02/truitt_dcecp_chfts26/1]
 *
 * 	CHFts14301: Allocate memory for the sec_attr_t structures
 * 	dynamically.
 * 	[1995/04/28  19:35 UTC  mdf  /main/HPDCE02/8]
 *
 * 	Changes for defect CHFts14561
 * 	[1995/04/13  21:22 UTC  mdf  /main/HPDCE02/mdf_mothra_defects_3/1]
 *
 * 	pick up jrr's memory leak fixes.
 * 	[1995/03/09
 *
 * Revision 1.1.9.2  1996/08/09  11:50:36  arvind
 * 	Merge changes.
 * 	[1996/07/02  17:29 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/3]
 * 
 * 	Additional pk work.
 * 	[1996/06/27  18:54 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/2]
 * 
 * 	Public key back-end work.
 * 	[1996/06/11  19:35 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/1]
 * 
 * 	Some dcecp changes to support Public Key functionality.
 * 	Merge with latest HP fixes and enhancements.
 * 	[1996/03/28  18:21 UTC  truitt  /main/HPDCE02/23]
 * 
 * 	Merge changes.
 * 	[1996/03/28  18:19 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf72/1]
 * 
 * 	CHFts17975: The change to the access method is incorrect for a couple
 * 	commands.  We have to get the open_site and open_site_by_access
 * 	functions to work correctly, with the correct security api calls.  It would be
 * 	wonderful if the security api calls were returning the binding of an UP replica
 * 	each time, but supposedly there is no guarantee of that.  Unbelievable.  So,
 * 	we've got to jump through some hoops to make sure we make the correct call
 * 	to the correct replica (or cell)  under the correct circumstances, or forget it.
 * 	[1996/03/20  21:07 UTC  jrr  /main/HPDCE02/22]
 * 
 * 	Modify open_rgy_site() to upgrade access to site_specific if
 * 	either a command line site or an _s(sec) site is specified.
 * 	Only upgrade site_query access. Leave site_update and (for
 * 	now) site_default alone.
 * 	[1996/03/20  21:05 UTC  jrr  /main/HPDCE02/jrr_davis_1/1]
 * 
 * 	Merge changes.
 * 	[1996/02/15  18:17 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf62/1]
 * 
 * 	CHFts17451: Remove include of value.h so duplicate macro
 * 	warning does not appear during HP build.  The include is
 * 	required on other platforms.
 * 	[1996/02/05  15:52 UTC  truitt  /main/HPDCE02/20]
 * 
 * 	Merge changes.
 * 	[1996/02/05  15:49 UTC  truitt  /main/HPDCE02/truitt_dcecp_17281/1]
 * 
 * 	CHFts17281: Fix problem between dcp_id_parse_name() and
 * 	sec_id_parse_name() so the correct parsing is done and the
 * 	correct cell is contacted, based on the input argument.
 * 	CHFts17406: Clean up some of the internal function names.
 * 	[1996/01/31  15:37 UTC  truitt  /main/HPDCE02/19]
 * 
 * 	Merge for multiple tower bindings.
 * 	[1996/01/31  15:32 UTC  truitt  /main/HPDCE02/truitt_dcecp_17287/1]
 * 
 * 	Merge changes.
 * 	[1995/11/15  21:02 UTC  truitt  /main/HPDCE02/truitt_dcecp_chfts52/1]
 * 
 * 	CHFts16836: Make sure the correct instances are freed when
 * 	an error is encountered during an era modification.
 * 	[1995/11/02  21:43 UTC  kline_s  /main/HPDCE02/17]
 * 
 * 	Fix for CHFts16023, CHFts16464, CHFts15919 - Remove bogus call to rep_string_free()
 * 	for codeset_name, which is returned from the dce_cs_rgy_to_loc() call.  This is
 * 	a pointer to shared memory and should not be freed.
 * 	[1995/10/24  20:24 UTC  kline_s  /main/HPDCE02/kline_s_mothra2/1]
 * 
 * 	merge-out
 * 	[1995/10/13  18:13 UTC  aha  /main/HPDCE02/aha_mothra10/2]
 * 
 * 	CHFts15769: dcecp can bind to stopped security server;
 * 	be more selective about when site is set from site_cache on update
 * 	[1995/10/13  17:21 UTC  aha  /main/HPDCE02/aha_mothra10/1]
 * 
 * 	Submitted the fix for CHFts16512.
 * 	[1995/09/23  17:48 UTC  tatsu_s  /main/HPDCE02/tatsu_s.dced_local.b0/1]
 * 
 * 	Added dhd__schema_ent_free_ptrs() kluge.
 * 	[1995/09/28  17:43 UTC  truitt  /main/HPDCE02/14]
 * 
 * 	Merge bytes output change.
 * 	[1995/09/28  17:41 UTC  truitt  /main/HPDCE02/truitt_dcecp_chfts47/1]
 * 
 * 	CHFts16476: Output the correct number of bytes per line for a
 * 	bytes-type era.
 * 	[1995/07/12  13:24 UTC  truitt  /main/HPDCE02/13]
 * 
 * 	Merge i18n changes.
 * 	[1995/07/12  13:22 UTC  truitt  /main/HPDCE02/truitt_dcecp_chfts38/1]
 * 
 * 	CHFts15275: Allow proper parsing of input and output for attributes
 * 	with encoding type i18n_data.
 * 	[1995/06/12  12:58 UTC  truitt  /main/HPDCE02/12]
 * 
 * 	Merge.
 * 	[1995/06/12  12:57 UTC  truitt  /main/HPDCE02/truitt_dcecp_chfts34/1]
 * 
 * 	CHFts15424: Fix braces in print attrs function.
 * 	[1995/05/23  19:16 UTC  truitt  /main/HPDCE02/11]
 * 
 * 	Merge field work.
 * 	[1995/05/23  19:14 UTC  truitt  /main/HPDCE02/truitt_dcecp_chfts30/1]
 * 
 * 	CHFts14679: Move string header info from code to sams file.
 * 	[1995/05/15  17:34 UTC  truitt  /main/HPDCE02/10]
 * 
 * 	Merge warranty patch work.
 * 	[1995/05/15  17:30 UTC  truitt  /main/HPDCE02/truitt_dcecp_chfts29/1]
 * 
 * 	Merge w
 * 
 * Revision 1.1.9.1  1996/06/04  21:42:18  arvind
 * 	Merge u2u changes from mb_u2u
 * 	[1996/05/06  21:55 UTC  burati  /main/DCE_1.2/3]
 * 
 * 	merge  u2u work
 * 	[1996/05/02  19:17 UTC  burati  /main/DCE_1.2/mb_u2u/1]
 * 
 * 	u2u work off Mothra, before 1.2 branch is available
 * 	[1996/04/29  21:49 UTC  burati  /main/HPDCE02/mb_mothra8/1]
 * 
 * Revision 1.1.6.4  1996/02/18  19:14:43  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:00:56  marty]
 * 
 * Revision 1.1.6.3  1996/02/07  17:47:22  parul
 * 	DCE 1.2.1 final drop from HP
 * 	[1996/02/07  16:13:03  parul]
 * 
 * 	DCE 1.2.1 final drop from HP
 * 
 * Revision 1.1.2.84  1994/10/05  21:59:29  salamone
 * 	CR12447 - set _b(sec) to fullname
 * 	[1994/10/05  21:21:51  salamone]
 * 
 * 	Merge up.
 * 	[1994/10/05  19:47:38  salamone]
 * 
 * Revision 1.1.2.83  1994/10/05  19:12:13  kevins
 * 	OT 12464 registry connect princ/account generate  modified.
 * 	[1994/10/05  18:09:15  kevins]
 * 
 * Revision 1.1.2.82  1994/10/03  20:46:41  rousseau
 * 	Check cache before rebinding in open_rgy_site (CR 12463).
 * 	[1994/10/03  20:46:21  rousseau]
 * 
 * Revision 1.1.2.81  1994/09/30  15:31:40  truitt
 * 	OT12434: Changed a few malloc's.
 * 	[1994/09/30  15:00:18  truitt]
 * 
 * Revision 1.1.2.80  1994/09/29  21:28:32  kevins
 * 	OT 12415 fixed suspect malloc lengths
 * 	[1994/09/29  21:27:51  kevins]
 * 
 * Revision 1.1.2.79  1994/09/29  18:45:21  truitt
 * 	Merged with changes from 1.1.2.78
 * 	[1994/09/29  18:44:48  truitt]
 * 
 * 	OT10967: Allow proper display of multiple valued attributes.
 * 	[1994/09/29  17:37:02  truitt]
 * 
 * Revision 1.1.2.78  1994/09/28  17:54:42  kevins
 * 	OT 12376 modified registry unavailable message delivery
 * 	[1994/09/28  17:51:06  kevins]
 * 
 * Revision 1.1.2.77  1994/09/27  19:24:07  kevins
 * 	OT 12361 modified deftktlife parsing
 * 	[1994/09/27  19:22:18  kevins]
 * 
 * Revision 1.1.2.76  1994/09/20  17:35:00  kevins
 * 	OT 12257 modified site_cache checking functions
 * 	[1994/09/20  17:34:46  kevins]
 * 
 * Revision 1.1.2.75  1994/09/19  17:30:32  truitt
 * 	OT12108: Incorrect usage of array index.
 * 	[1994/09/19  16:00:57  truitt]
 * 
 * Revision 1.1.2.74  1994/09/16  15:34:28  ajayb
 * 	Merged with changes from 1.1.2.73
 * 	[1994/09/16  15:33:53  ajayb]
 * 
 * 	print_attrs(): Print leading newline before ERAs
 * 	only if required.
 * 	[1994/09/15  18:03:32  ajayb]
 * 
 * Revision 1.1.2.73  1994/09/15  18:19:52  truitt
 * 	Because the match_index variable was being set only when
 * 	the user was doing a remove by type, a remove of the last
 * 	instance when done by value was failing.  Must set the
 * 	match_index variable whenever an exact match is found.
 * 	[1994/09/15  18:19:07  truitt]
 * 
 * Revision 1.1.2.72  1994/09/15  14:18:30  truitt
 * 	OT11682: Changed or added malloc's in the get_attr_values()
 * 	function to better match the way the security code seems to
 * 	want them done.
 * 	[1994/09/15  12:37:25  truitt]
 * 
 * Revision 1.1.2.71  1994/09/13  19:12:45  kevins
 * 	OT 11641 list syntax not supported in catalog commands.
 * 	[1994/09/13  18:46:50  kevins]
 * 
 * Revision 1.1.2.70  1994/09/12  17:58:03  kevins
 * 	OT 11968 Modified the registry opening process to aquire the
 * 	   rgy site handle with authentication that corresponds
 * 	   to the users login context. Removed site_master_query since
 * 	   it is no longer necessary.
 * 	[1994/09/12  13:48:32  kevins]
 * 
 * Revision 1.1.2.69  1994/09/06  16:19:58  kevins
 * 	OT 11694 broke out pgo stuff into rgy_pgo_utils.c
 * 	[1994/09/06  16:19:01  kevins]
 * 
 * Revision 1.1.2.68  1994/09/01  20:09:59  pwang
 * 	Checked null/empty attr_value and handled them accordingly
 * 	in case of remove_by_type in modify_era_info() [OT#11970]
 * 	[1994/09/01  20:09:09  pwang]
 * 
 * Revision 1.1.2.67  1994/09/01  19:04:50  truitt
 * 	move schema commands to separate files
 * 	[1994/09/01  18:09:33  truitt]
 * 
 * Revision 1.1.2.66  1994/09/01  14:21:19  ajayb
 * 	Merged with changes from 1.1.2.65
 * 	[1994/09/01  14:21:01  ajayb]
 * 
 * 	get_attr_values(): Handle tower bindings in input.
 * 	encode_schema_attr(): Handle tower bindings in trigbind values.
 * 	parse_schema_switch_list(): Handle tower bindings in trigbind values.
 * 	[1994/08/31  18:06:49  ajayb]
 * 
 * 	Various changes for input support of tower bindings.
 * 	[1994/08/31  15:00:14  ajayb]
 * 
 * Revision 1.1.2.65  1994/08/31  23:04:03  salamone
 * 	CR11916 - Add _b(sec) support.
 * 	[1994/08/31  23:02:30  salamone]
 * 
 * Revision 1.1.2.64  1994/08/31  19:08:18  kevins
 * 	OT 11972 Merge error
 * 	[1994/08/31  19:08:00  kevins]
 * 
 * Revision 1.1.2.63  1994/08/31  18:25:09  kevins
 * 	OT 11972 modified how group/org names are aquired for acct create
 * 	[1994/08/31  18:19:40  kevins]
 * 
 * Revision 1.1.2.62  1994/08/31  13:36:01  kevins
 * 	OT 11953 modified organization policy attr list error handling
 * 	[1994/08/30  19:38:41  kevins]
 * 
 * Revision 1.1.2.61  1994/08/31  12:50:16  truitt
 * 	Merged with changes from 1.1.2.60
 * 	[1994/08/31  12:49:58  truitt]
 * 
 * 	OT11095: Support proper removal of extended attributes.
 * 	OT11098: Handle multiple values per attribute correctly.
 * 	[1994/08/30  20:51:14  truitt]
 * 
 * Revision 1.1.2.60  1994/08/30  15:19:05  kevins
 * 	OT 11597 uid errors are now reported correctly
 * 	[1994/08/30  15:17:06  kevins]
 * 
 * Revision 1.1.2.59  1994/08/29  14:21:03  ajayb
 * 	Merged with changes from 1.1.2.58
 * 	[1994/08/29  14:20:16  ajayb]
 * 
 * 	Eliminate use of randompwd field in account struct.
 * 	[1994/08/26  20:38:03  ajayb]
 * 
 * Revision 1.1.2.58  1994/08/26  19:30:21  kevins
 * 	OT 11660 uses dce_cf_same_cell_name to check differences in cell name
 * 	[1994/08/26  15:11:28  kevins]
 * 
 * Revision 1.1.2.57  1994/08/25  15:59:51  kevins
 * 	Merged with changes from 1.1.2.56
 * 	[1994/08/25  15:59:18  kevins]
 * 
 * 	OT 11865 fix attrlist decoding process.
 * 	[1994/08/25  15:57:30  kevins]
 * 
 * Revision 1.1.2.56  1994/08/25  15:52:42  nav
 * 	Merged with changes from 1.1.2.55
 * 	[1994/08/25  15:52:07  nav]
 * 
 * 	Added support for generic schema.
 * 	[1994/08/25  15:42:40  nav]
 * 
 * Revision 1.1.2.55  1994/08/25  13:39:25  ajayb
 * 	Merged with changes from 1.1.2.54
 * 	[1994/08/25  13:38:50  ajayb]
 * 
 * 	print_attrs():
 * 
 * 	Use DCP_BINDING_MAX_BYTES_PER_LINE instead of
 * 	MAX_BYTES_PER_LINE while printing byte data.
 * 	[1994/08/24  20:42:09  ajayb]
 * 
 * 	get_attr_values(), print_attrs():
 * 
 * 	Input/Output ERA's of encoding type 'byte' correctly.
 * 	[1994/08/24  18:56:38  ajayb]
 * 
 * Revision 1.1.2.54  1994/08/24  20:35:08  kevins
 * 	Merge problems
 * 	[1994/08/24  20:34:44  kevins]
 * 
 * Revision 1.1.2.53  1994/08/24  20:24:10  kevins
 * 	Merged with changes from 1.1.2.52
 * 	[1994/08/24  20:23:07  kevins]
 * 
 * 	OT 11659 supplied mutex locks for context modifcations
 * 	[1994/08/24  17:55:20  kevins]
 * 
 * Revision 1.1.2.52  1994/08/24  18:52:38  truitt
 * 	OT11709: Finish support for attributes of type void.  Also,
 * 	         corrected a compiler warning and removed three
 * 	         instances where attributes of type 'any' were
 * 	         being incorrectly associated with 'void' attributes.
 * 	[1994/08/24  18:48:10  truitt]
 * 
 * Revision 1.1.2.51  1994/08/24  13:59:55  truitt
 * 	Merged with changes from 1.1.2.50
 * 	[1994/08/24  13:59:32  truitt]
 * 
 * 	Add a missing 'break' statement to ensure leaving an
 * 	endless loop.  Also realigned some code.
 * 	[1994/08/24  13:55:25  truitt]
 * 
 * Revision 1.1.2.50  1994/08/23  21:20:41  kevins
 * 	        11789 - Finished the "registry connect" command. I
 * 	                was passing in a bad sec_passwd_rec_t value to
 * 	                sec_key_mgmt_gen_rand_key. Added registry connect
 * 	                failure cleanup.
 * 	        11596 - Multiple definitions of attribute name "uuid' caused
 * 	                problems with parsing same.
 * 	        11394 - _s(sec) is being taken from the site variable returned
 * 	                from sec_rgy_site_get() after the registry is opened.
 * 	                This was in string binding format. I changed this to
 * 	                be tcl syntax.
 * 	[1994/08/23  20:53:42  kevins]
 * 
 * 	Merge up
 * 	[1994/08/22  20:01:55  kevins]
 * 
 * Revision 1.1.2.49  1994/08/22  19:11:57  pwang
 * 	Merged with changes from 1.1.2.48
 * 	[1994/08/22  19:11:30  pwang]
 * 
 * 	Changed check_era_info() into two routines, check_era_info()
 * 	and modify_era_info() [OT#11424]
 * 	[1994/08/22  19:09:30  pwang]
 * 
 * Revision 1.1.2.48  1994/08/22  19:08:29  melman
 * 	change NULL to 0
 * 	[1994/08/22  19:00:19  melman]
 * 
 * Revision 1.1.2.47  1994/08/19  18:14:33  truitt
 * 	Merged with changes from 1.1.2.46
 * 	[1994/08/19  18:13:55  truitt]
 * 
 * 	OT11016: Corrected error code reporting in print_attrs() function.
 * 	         Corrected display of printstrings with no whitespace.
 * 	OT11030: Handled null case in print_attrs() for encoding type
 * 	         printstring_array.
 * 	OT11644: Corrected error message coming back from an unauthenticated
 * 	         principal create command.
 * 	OT11709: Partial.  Support for new void format.
 * 	OT11095: Partial.  Additional work for era attribute delete.
 * 	[1994/08/19  16:16:39  truitt]
 * 
 * Revision 1.1.2.46  1994/08/19  16:55:38  ajayb
 * 	Added free_account_ptrs(), which frees memory
 * 	obtained by parse_account_attributes()
 * 	[1994/08/18  21:52:12  ajayb]
 * 
 * Revision 1.1.2.45  1994/08/17  15:37:02  melman
 * 	added 4th param to sec_att_util_alloc_copy
 * 	[1994/08/17  15:36:41  melman]
 * 
 * Revision 1.1.2.44  1994/08/17  14:32:28  truitt
 * 	OT11449: xattrschema create was incorrectly parsing fields
 * 	         passed via the -attribute option.  Some variables
 * 	         would produce an incorrect message, while others
 * 	         caused a core dump.  Now carefully check the
 * 	         parameters passed in via the -attribute option.
 * 	OT11691: Added support for the -simplename option to the
 * 	         xattrschema catalog command.
 * 	OT11692: Corrected output of the xattrschema show command.
 * 	[1994/08/16  21:07:42  truitt]
 * 
 * Revision 1.1.2.43  1994/08/12  20:14:04  kevins
 * 	fixed merge failure
 * 	[1994/08/12  20:13:51  kevins]
 * 
 * Revision 1.1.2.42  1994/08/12  20:03:46  kevins
 * 	Merged with changes from 1.1.2.41
 * 	[1994/08/12  20:03:25  kevins]
 * 
 * 	OT 9691 Add registry connect command
 * 	[1994/08/12  20:00:26  kevins]
 * 
 * Revision 1.1.2.41  1994/08/10  21:22:34  jane
 * 	Merged with changes from 1.1.2.40
 * 	[1994/08/10  21:22:12  jane]
 * 
 * 	added random password generation functionality
 * 	[1994/08/10  21:00:12  jane]
 * 
 * Revision 1.1.2.40  1994/08/08  21:04:48  melman
 * 	put include dce_utils.h in dcecp.h
 * 	[1994/08/08  21:02:03  melman]
 * 
 * Revision 1.1.2.39  1994/08/08  18:00:53  kevins
 * 	        11071 - Reworked the rgy_handle cache so that we now
 * 	        maintain two registry handles. One that is dynamic in
 * 	        that it always tracks the cell/registry that the user wants to access
 * 	        and another that is by default associated with unauthenticated
 * 	        access to the local registry. We now use the unauthenticated registry
 * 	        handle to pass into sec_id_parse_name so that we have quick and easy
 * 	        access to parsing object names.
 * 
 * 	        10664 - The code now also checks the name of the cell that is
 * 	        currently opened and does not reopen the registry if it was opened
 * 	        before for the same access type.
 * 	[1994/08/08  15:42:05  kevins]
 * 
 * Revision 1.1.2.38  1994/08/01  13:59:53  kevins
 * 	OT 11500 Added dce_utils.h
 * 	[1994/08/01  13:54:37  kevins]
 * 
 * Revision 1.1.2.37  1994/07/29  20:33:49  kevins
 * 	OT 11394 Now accepts tcl binding in _s(sec)
 * 	[1994/07/29  20:27:24  kevins]
 * 
 * Revision 1.1.2.36  1994/07/25  16:00:34  truitt
 * 	Change the default registry schema name
 * 	[1994/07/25  14:08:11  truitt]
 * 
 * Revision 1.1.2.35  1994/07/20  20:40:23  nav
 * 	Merged with changes from 1.1.2.34
 * 	[1994/07/20  20:39:57  nav]
 * 
 * 	Added support for schema modify and schema rename.
 * 	[1994/07/20  19:25:09  nav]
 * 
 * Revision 1.1.2.34  1994/07/20  19:28:05  salamone
 * 	Add new bind type; site_master_query.
 * 	[1994/07/20  19:20:44  salamone]
 * 
 * Revision 1.1.2.33  1994/07/19  18:28:36  truitt
 * 	OT11186: Implement the rest of schema create.
 * 	OT9691: Implement schema delete.
 * 	[1994/07/19  17:31:13  truitt]
 * 
 * Revision 1.1.2.32  1994/07/15  14:58:06  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/15  10:48:20  mdf]
 * 
 * Revision 1.1.2.31  1994/07/14  21:09:03  coren
 * 	Added ability to display tower-type bindings (CR10964).
 * 	[1994/07/14  18:44:04  coren]
 * 
 * Revision 1.1.2.30  1994/07/14  18:37:08  truitt
 * 	Merged with changes from 1.1.2.29
 * 	[1994/07/14  18:36:47  truitt]
 * 
 * 	OT11186: Implement partial schema create
 * 	[1994/07/14  17:34:35  truitt]
 * 
 * Revision 1.1.2.29  1994/07/11  13:28:05  coren
 * 	Added code to compare binding-type attributes in check_era_info.
 * 	[1994/07/08  20:52:48  coren]
 * 
 * Revision 1.1.2.28  1994/07/08  09:30:40  kevins
 * 	OT 11197 quota value perserved
 * 	[1994/07/07  20:38:59  kevins]
 * 
 * Revision 1.1.2.27  1994/07/07  20:53:45  coren
 * 	Changed MAX* constants to be derived from array size.
 * 	[1994/07/07  20:52:08  coren]
 * 
 * 	Bugfixes after merge.
 * 	[1994/07/07  16:05:38  coren]
 * 
 * 	Added code to handle bindings.
 * 	[1994/07/05  21:43:34  coren]
 * 
 * Revision 1.1.2.26  1994/07/07  15:24:53  truitt
 * 	Merged with changes from 1.1.2.25
 * 	[1994/07/07  15:24:14  truitt]
 * 
 * 	OT11185: Support for schema catalog.
 * 	OT11181: Change constant name.
 * 	OT11183: Have schema show accept fully qualified
 * 	schema attribute names.
 * 	[1994/07/07  14:46:32  truitt]
 * 
 * Revision 1.1.2.25  1994/07/07  14:00:38  kevins
 * 	OT 11118 reorder display of registry attribtues and policies
 * 	[1994/07/07  12:04:18  kevins]
 * 
 * Revision 1.1.2.24  1994/07/05  19:46:50  truitt
 * 	Merged with changes from 1.1.2.23
 * 	[1994/07/05  19:46:28  truitt]
 * 
 * 	OT11162: Implement schema show
 * 	[1994/07/05  18:20:10  truitt]
 * 
 * Revision 1.1.2.23  1994/07/05  15:45:31  kevins
 * 	OT11118 registry policy attributes are now show and modifyable
 * 	[1994/07/05  15:44:09  kevins]
 * 
 * Revision 1.1.2.22  1994/06/30  20:42:32  kevins
 * 	OT 11117 Changed registry attribute classifications.
 * 	[1994/06/30  20:15:50  kevins]
 * 
 * Revision 1.1.2.21  1994/06/28  15:01:13  truitt
 * 	Merged with changes from 1.1.2.20
 * 	[1994/06/28  15:01:04  truitt]
 * 
 * 	OT11061: Support for pgo modify (all phases) of extended
 * 	attributes.  One new era checking function was added.
 * 	[1994/06/27  18:32:52  truitt]
 * 
 * Revision 1.1.2.20  1994/06/23  19:23:21  kevins
 * 	OT 11060 Added checks for registry site opening
 * 	[1994/06/23  18:41:49  kevins]
 * 
 * Revision 1.1.2.19  1994/06/22  19:23:08  kevins
 * 	OT11037 changed registry policy modification error reporting technics
 * 	[1994/06/22  19:05:30  kevins]
 * 
 * Revision 1.1.2.18  1994/06/09  16:02:25  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:47:59  devsrc]
 * 
 * Revision 1.1.2.17  1994/06/08  15:44:59  kevins
 * 	OT 10663 Add ability to choose registry
 * 	[1994/06/08  15:44:44  kevins]
 * 
 * Revision 1.1.2.16  1994/06/07  19:16:41  kevins
 * 	OT 10784 modified integer evalutation functions
 * 	[1994/06/07  19:15:33  kevins]
 * 
 * Revision 1.1.2.15  1994/06/02  14:01:54  truitt
 * 	Merged with changes from 1.1.2.14
 * 	[1994/06/02  14:01:44  truitt]
 * 
 * 	OT 10800: Patched up a problem where encode policy was being
 * 	passed extended attribute information and was not ignoring
 * 	them properly.
 * 	[1994/06/01  16:55:37  truitt]
 * 
 * 	OT 10800: Initial work for extended registry attribute
 * 	support.  Three new functions added to handle basic
 * 	show capability, era display and storage of extended
 * 	attribute values.
 * 	[1994/05/27  18:37:39  truitt]
 * 
 * Revision 1.1.2.14  1994/05/26  17:56:39  salamone
 * 	Ensure binding to proper site for all regi cmds.  Added another
 * 	flag to open_rgy_site() to bind to a specific registry site for update.
 * 	Fixed the following core dump problem if user unsets SITE_VAR.
 * 	[1994/05/26  16:42:23  salamone]
 * 
 * Revision 1.1.2.13  1994/05/20  11:21:35  kevins
 * 	10635 - Modified the routine that opens the registry site to allow
 * 		unauthenticated access.
 * 	10033 - Added the ability to add pgo/account objects via their
 * 		full path name.
 * 	[1994/05/20  08:57:40  kevins]
 * 
 * Revision 1.1.2.12  1994/05/12  19:35:07  kevins
 * 	OT 10618 modified input/output of acctlife pwdlife and pwdexpdate for UTC dates
 * 	[1994/05/12  19:34:51  kevins]
 * 
 * Revision 1.1.2.11  1994/05/12  13:04:09  kevins
 * 	OT 10612 removed redefined macros.
 * 	[1994/05/12  11:37:53  kevins]
 * 
 * Revision 1.1.2.10  1994/05/11  19:13:57  kevins
 * 	OT 10595 Fixed usage of _s(sec) var
 * 	[1994/05/11  19:13:30  kevins]
 * 
 * Revision 1.1.2.9  1994/05/10  10:00:17  kevins
 * 	CR 9691 merge needed
 * 	[1994/05/10  09:54:38  kevins]
 * 
 * 	OT 9691 Add registry show/modify commands
 * 	[1994/05/09  19:16:00  kevins]
 * 
 * Revision 1.1.2.8  1994/04/19  16:27:04  kevins
 * 	CR 10381 added forwardabletkt and pwdvalid as account attributes
 * 	[1994/04/19  10:50:02  kevins]
 * 
 * Revision 1.1.2.7  1994/04/08  10:40:17  kevins
 * 	OT 9691 Implemented account commands
 * 	[1994/04/08  10:39:31  kevins]
 * 
 * Revision 1.1.2.6  1994/03/22  16:14:20  kevins
 * 	CR 10198 check the attribute name uid/gid/orgid
 * 	[1994/03/22  16:10:21  kevins]
 * 
 * 	CR 10198 check the attribute name uid/gid/orgidd
 * 
 * Revision 1.1.2.5  1994/03/21  23:52:24  kevins
 * 	CR 10097 modify commands should not be able to alter unix_num
 * 	[1994/03/21  23:51:14  kevins]
 * 
 * Revision 1.1.2.4  1994/03/18  14:24:01  kevins
 * 	CR 10096/10026 changed allocation of attribute decoding variables
 * 	[1994/03/18  00:12:04  kevins]
 * 
 * Revision 1.1.2.3  1994/03/10  19:10:22  kevins
 * 	Updated to synchronize with cds message cleanup
 * 	[1994/03/10  19:04:35  kevins]
 * 
 * Revision 1.1.2.2  1994/02/03  21:34:50  rousseau
 * 	Added #include <time.h> for CR 9886.
 * 	[1994/02/03  21:34:34  rousseau]
 * 
 * Revision 1.1.2.1  1994/02/03  17:24:17  kevins
 * 	added support for group and org objects
 * 	[1994/02/03  17:17:21  kevins]
 * 
 * $EndLog$
 */

/* INCLUDES */
#include <stdio.h>
#include <locale.h>
#include <errno.h>
/*
 * Don't inherit stubbase.h's use of the exception based pthreads API.
 */
#ifndef NCK
#  define NCK
#endif
#include <dce/rpc.h>
#include <dce/stubbase.h>

#ifndef SNI_SVR4_POSIX
#include <strings.h>
#endif

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <dce/macros.h>
#include <dce/dce_error.h>
#include <sys/types.h>
#include <unistd.h>
#include <dce/dce.h>
#include <dce/binding.h>
#include <dce/pgo.h>
#include <dce/policy.h>
#include <dce/rgynbase.h>
#include <dce/secidmap.h>
#include <dce/dce_msg.h>
#include <dce/dcedcpmsg.h>
#include <dce/dce_svc.h>
#include <dce/misc.h>
#include <dcecp.h>
#include <dce/tcl.h>
#include <dce/utc.h>
#include <dce/passwd.h>
#include <rgy.h>
#include <wchar.h>
#include <values.h>

#include <util_var.h>
#include <util_general.h>
#include <util_binding.h>
#include <dce/sec_attr_base.h>
#include <dce/sec_attr_util.h>
#include <dce/sec_attr_tools.h>
#include <dce/sec_rgy_attr.h>
#include <dce/sec_rgy_attr_sch.h>
#include <dce/aclbase.h>
#include <dce/acct.h>
#include <ctype.h>
#include <dhd.h>
#include <util_login.h>
#include <rgy_sa_replist.h>

unsigned char st_txt[dce_c_error_string_len];
int dce_status;
int skip_era_newline = 0;

#define MAX_ATTR_SHOW      10
#define MAX_BYTES_PER_LINE 12 
#define MAX_PROT_LEV       ((sizeof(prot_lev_val)) / (sizeof(unsigned32)))
#define MAX_AUTHN          ((sizeof(authn_val)) / (sizeof(unsigned32)))
#define MAX_AUTHZ          ((sizeof(authz_val)) / (sizeof(unsigned32)))

#define APPEND_VALUE(buffer)                        \
    if(interp->result[0] == '\0')                   \
        Tcl_AppendResult(interp,buffer,NULL);       \
    else                                            \
        Tcl_AppendResult(interp,"\n",buffer,NULL);  \

/* PROTOTYPES */

void check_string_binding(char *, unsigned32 *);
unsigned32 get_current_time(void);
int add_yes_no_flag(char *, signed32 *, int );
int add_maxuid(char *, sec_rgy_properties_t * );
int add_minuid(char *, sec_rgy_properties_t * );
int add_mingid(char *, sec_rgy_properties_t * );
int add_minorgid(char *, sec_rgy_properties_t * );
boolean32 match_bindings(sec_attr_bind_info_t *, sec_attr_bind_info_t *);
void get_hex_bytes(char *, char *, unsigned32 *, unsigned32 *);

/* INTERNAL VARIABLES */

typedef struct dcecp_rgy_attr_index_table_t {
    char *attribute_name;
    int  index;
} dcecp_rgy_attr_index_table_t;

typedef struct era_check_table_s_t {
    boolean32  same_found;
    boolean32  other_found;
    boolean32  used;
    boolean32  special_case;
    boolean32  any_case;
    uuid_t     attr_id;
} era_check_table_t;

typedef struct any_list_s_t {
    int  index;
} any_list_t;

static sec_attr_schema_entry_t  s_null_sch_entry;

dcecp_rgy_attr_index_table_t rgy_attr[] = {
    {rgy_attr_name,rgy_pgo_name },
    {rgy_attr_uuid,rgy_pgo_uuid},
    {rgy_attr_uid,rgy_pgo_unix_num},
    {rgy_attr_gid,rgy_pgo_unix_num},
    {rgy_attr_orgid,rgy_pgo_unix_num},
    {rgy_attr_fullname,rgy_pgo_fullname},
    {rgy_attr_alias,rgy_pgo_is_an_alias},
    {rgy_attr_quota,rgy_pgo_quota},
    {rgy_attr_inprojlist,rgy_pgo_inprojlist},
    {rgy_attr_pwdlife,rgy_plcy_pwdlife},
    {rgy_attr_pwdminlen,rgy_plcy_pwdminlen },
    {rgy_attr_pwdexpdate,rgy_plcy_pwdexpdate},
    {rgy_attr_pwdspaces,rgy_plcy_pwdspaces},
    {rgy_attr_pwdalpha,rgy_plcy_pwdalpha},
    {rgy_attr_acctlife,rgy_plcy_acctlife},
    {rgy_attr_version,rgy_plcy_version},
    {rgy_attr_acctvalid,rgy_acct_acctvalid},
    {rgy_attr_client,rgy_acct_client},
    {rgy_attr_created,rgy_acct_created},
    {rgy_attr_description,rgy_acct_description},
    {rgy_attr_dupkey,rgy_acct_dupkey },
    {rgy_attr_expdate,rgy_acct_expdate},
    {rgy_attr_goodsince,rgy_acct_goodsince },
    {rgy_attr_group,rgy_acct_group},
    {rgy_attr_home,rgy_acct_home },
    {rgy_attr_lastchange,rgy_acct_lastchange},
    {rgy_attr_organization,rgy_acct_organization },
    {rgy_attr_postdatedtkt,rgy_acct_postdatedtkt},
    {rgy_attr_proxiabletkt,rgy_acct_proxiabletkt},
    {rgy_attr_renewabletkt,rgy_acct_renewabletkt},
    {rgy_attr_server,rgy_acct_server},
    {rgy_attr_shell,rgy_acct_shell},
    {rgy_attr_stdtgtauth,rgy_acct_stdtgtauth },
    {rgy_attr_maxtktlife,rgy_acct_plcy_maxtktlife },
    {rgy_attr_maxtktrenew,rgy_acct_plcy_maxtktrenew},
    {rgy_attr_password,rgy_acct_password},
    {rgy_attr_mypwd,rgy_acct_mypwd },
    {rgy_attr_forwardabletkt,rgy_acct_forwardabletkt},
    {rgy_attr_pwdvalid,rgy_acct_pwdvalid},
    {rgy_attr_deftktlife,rgy_deftktlife},
    {rgy_attr_mingid,rgy_mingid},
    {rgy_attr_minorgid,rgy_minorgid},
    {rgy_attr_minuid,rgy_minuid},
    {rgy_attr_maxuid,rgy_maxuid},
    {rgy_attr_mintktlife,rgy_mintktlife},
    {rgy_attr_usertouser,rgy_acct_usertouser},
    {rgy_attr_pksignatureusage,rgy_acct_pksignatureusage},
    {rgy_attr_pkkeycipherusage,rgy_acct_pkkeycipherusage},
    {rgy_attr_pkgenprivkey,rgy_acct_pkgenprivkey},
    {rgy_attr_pkmechanism,rgy_acct_pkmechanism},
    {rgy_attr_hidepwd,rgy_hidepwd}
};

static char *prot_lev_name[] = {"default",
                                "none",
                                "connect",
                                "call",
                                "pkt",
                                "pktinteg",
                                "pktprivacy"};

static unsigned32 prot_lev_val[] = {rpc_c_protect_level_default,
                                    rpc_c_protect_level_none,
                                    rpc_c_protect_level_connect,
                                    rpc_c_protect_level_call,
                                    rpc_c_protect_level_pkt,
                                    rpc_c_protect_level_pkt_integ,
                                    rpc_c_protect_level_pkt_privacy};

static char *authn_name[] = {"none",
                             "secret",
                             "public",
                             "default"};

static unsigned32 authn_val[] = {rpc_c_authn_none,
                                 rpc_c_authn_dce_secret,
                                 rpc_c_authn_dce_public,
                                 rpc_c_authn_default};

static char *authz_name[] = {"none",
                             "name",
                             "dce"};

static unsigned32 authz_val[] = {rpc_c_authz_none,
                                 rpc_c_authz_name,
                                 rpc_c_authz_dce};


/*
 * FUNCTION: open_rgy_site
 *
 * OVERVIEW:
 *   This function is used by just about every dcecp registry-related
 *   command.  It is called directly by most account, principal, group,
 *   organization and xattrschema commands, and is called indirectly,
 *   via dcp_rgy_sa_bind_named_object() and dcp_rgy_sa_do_modify_key()
 *   for the registry commands.
 *
 *   The real binding work is done in the function that open_rgy_site()
 *   calls, however, open_site_by_access().  But you need the set-up code
 *   from here, so you should not call open_site_by_access() directly.
 *
 *   Primarily, this function accepts the name of a site to bind to and
 *   the method in which to bind.  The presence or absence of a site,
 *   and the access method all figure into which security api call is
 *   eventually used to bind to a registry server.
 *
 *   There are a number of horrible 'features' in some of the security
 *   binding api calls that cause dcecp some problems, not the least of
 *   which is the fact that an api call can return a success status and
 *   the binding of a DOWN replica.  The next time the binding is used
 *   in real code an error message like 'Communications failure' or
 *   'Not registered in endpoint map' appears.  Very useful.
 *
 *   Due to the problems we were having when a replica in the cell was
 *   down, we needed to do a careful check of each of the commands that
 *   call open_rgy_site() and what the commands expected in return.
 *   Please use the information in the following summary before modifying
 *   this function to ensure no other command is broken.
 *
 *   1. Because dcecp supports pgo name input in a variety of formats
 *      (name, /.:/name, /.../cell/name, name@cell), all the account,
 *      group, organization and principal operations (*except* 'catalog')
 *      require a name to operate on as an argument.  The name is parsed
 *      using dcp_id_parse_name(), which returns a cell name and a
 *      residual name to operate on.  The cell name is passed to the
 *      open_rgy_site() function, along with an access type of site_update
 *      for most operations, or an access type of site_query for any
 *      'show' and 'list' operation.  The main thing is, the cell name
 *      IS passed in for ALL of these operations.
 *
 *   2. For the 'catalog' operations in the above commands, the argument
 *      is an optional cell name, and whatever the user enters is passed
 *      as the cell name to the open_rgy_site() call.  The name entered
 *      is expected to be a cell name, not a replica name, and the access
 *      type is always site_query.
 *
 *   3. The xattrschema commands all require one or more schema names on
 *      input, so dcp_id_parse_name() is called and the cell name and
 *      an access type of site_update is passed to open_rgy_site() for
 *      all operations.
 *
 *   4. The registry commands differ greatly, depending upon their
 *      functionality.  They fall into three basic categories:
 *
 *      A. Show-type commands.  The 'catalog' and 'show' operations will
 *         use an access type of site_query if no registry name is passed
 *         as an argument, or if the argument is the local cell name.  If
 *         an argument is passed that represents a registry replica, or
 *         the name of a foreign cell, site_specific is used.  In 'show',
 *         if the -master option is used, the access type used will be
 *         site_update.
 *
 *      B. Master-type commands.  The 'delete', 'designate', 'replace'
 *         and 'synchronize' operations all require the name of a replica
 *         on the command line, but bind to the master using an access type
 *         of site_update.  Depending on the options given to the 'designate'
 *         operation, an access type of site_specific might be used instead.
 *         The 'connect' operation falls into this category because it uses
 *         site_update to bind to the local cell.
 * 
 *      C. Other commands.  These should be looked at individually.  The
 *         'checkpoint' option, for example, requires a replica name on
 *         the command line and binds using an access type of site_specific.
 *         Same with 'destroy' and 'stop'.  The 'enable' and 'disable'
 *         operations accept a replica, but bind to the master using an
 *         access of site_update.
 *
 * INPUTS:
 *   site_name_t name: site to bind to.  Can be anything from "" to the
 *                     shorthand for the cell (/.:) to a foreign cell
 *                     name to a full replica name.
 *
 *   site_access_t access: type of access required:
 *      site_update             Bind to master replica for update
 *      site_query              Bind to arbitrary replica for query
 *      site_specific           Bind to specific replica for update
 *      site_default            Bind to the local cell unauthenticated
 *
 * INPUT/OUTPUTS:
 *   Tcl_Interp *interp: Tcl Interpreter to use.
 *
 * OUTPUTS:
 *   None
 *
 * RETURNS:
 *   TCL_OK or TCL_ERROR
 */
int open_rgy_site(Tcl_Interp *interp, site_name_t name, site_access_t access)
{
    char            *s_variable;
    sec_rgy_name_t  site;
    sec_rgy_name_t  temp_site;
    char            *expand_site;
    char            *temp_binding;
    int             argc = 1;
    char            **argv;
    error_status_t  result = error_status_ok;

    /*
     * Start with an empty, clean site.  Get the value of the _s(sec)
     * convenience variable just in case we need it to help figure out
     * a binding.
     */
    memset((char *)site, 0, sizeof(site));
    s_variable = DCP_GENERAL_GET_S_VAR(SITE_VAR);

    /*
     * Process the replica or cell name entered.
     */
    if (strlen((char *)name)) {
        /*
         * A site name was entered.  Take it on faith that this
         * is the right name for the right operation.
         */
        strcpy((char *)site, (char *)name);
    }
    else if (s_variable && strlen(s_variable)) {
        /*
         * A name hasn't been passed in but the _s(sec) variable does
         * have a name, so assign site the value of the _s(sec) variable.
         */
        strcpy((char *)site, s_variable);
        if (access == site_query) {
            /*
             * Hmm.  The user is asking for site_query, but has named
             * a specific replica or cell to talk to in the _s(sec)
             * variable.  In this case, we better upgrade the access
             * type to site_specific and hope we did the right thing.
             * Note this isn't necessarily the right thing to do, but
             * at least will cause security to bind to the replica
             * that the user specified.
             */
            access = site_specific;
        }
    }
    else {
        /*
         * We don't have to get fancy here.  The user is making a
         * security binding request, but provided no name on the
         * command line, and does not have the _s(sec) variable set.
         * So, since the security code keeps its own internal cache
         * of bindings, let security be responsible for getting us
         * a binding.
         */
        strcpy((char *)site, "/.:");
    }

    if (Tcl_SplitList(interp, (char *)site, &argc, &argv) != TCL_OK)
        return TCL_ERROR;

    /*
     * Now that we have something in 'site', check if its a cell name,
     * string binding, or short replica name.
     */
    if (argc > 1) {
        /*
         * Hmm.  Too many arguments to be a straight replica name, so
         * it must be a tcl syntax string binding.  Convert it into a
         * normal string binding.
         */
        temp_binding = dcp_binding_tcl_to_string(argc, argv, &result);
        if (temp_binding) {
            strcpy((char *)site, temp_binding);
            rpc_string_free((unsigned char **)&temp_binding, &result);
        }
    }
    else if (*site == '/') {
        /*
         * This must be a full or partial pathame, of either a cell or
         * a specific replica.  Either way, processing is the same...
         */ 
        expand_site = expand_cell_name((char *)site, &result);
        if (result == rpc_s_incomplete_name) {
            DCP_SET_SEC_ERROR(dcp_s_rgy_con_bad_cell, site);
        }
        else {
            strcpy((char *)site, expand_site);
            free(expand_site);
        }
    }
    else if (!strchr((char *)site, ':')) {
        /*
         * Not a cell name, not a replica name, not a string binding.
         * Our last chance is a short replica name (subsys/dce/...).
         * Stick the local cell name in front of it.
         */
        strcpy((char *)temp_site, "/.:/");
        strcat((char *)temp_site, (char *)site);
        strcpy((char *)site, (char *)temp_site);
    }

    free((char *)argv);

    /*
     * Using the site we computed and the access type passed in, get
     * a binding to a registry server.
     */
    result = open_site_by_access(interp, site, &access);
    if (result == sec_rgy_server_unavailable) {
        DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_rgy_server_unavailable, site);
        return TCL_ERROR;
    }
    DCP_CHECK_SEC_ERROR(result);

    return TCL_OK;
}


/*
 * FUNCTION: open_site_by_access
 *
 * OVERVIEW:
 *
 * INPUTS:
 *
 * INPUT/OUTPUTS:
 *   Tcl_Interp *interp: Tcl Interpreter to use.
 *
 * OUTPUTS:
 *   None
 *
 * RETURNS:
 *   TCL_OK or TCL_ERROR
 *
 */
int open_site_by_access(Tcl_Interp *interp, site_name_t site, site_access_t *access)
{
    error_status_t            status, local_status;
    extern sec_rgy_handle_t   rgy_context; 
    unsigned_char_p_t         lsite;
    sec_rgy_bind_auth_info_t  auth_info;
    sec_rgy_handle_t          new_context;
    site_cache_type_t         cache_entry = realtime_cache;
    boolean32                 is_same_cell = FALSE;

    if (rgy_context == NULL) {
        if (pthread_mutex_init(&site_list, pthread_mutexattr_default) == -1)
            return dcp_s_mutex_init_fail;
    }

    if (*access == site_default) {
        cache_entry = default_cache;
        if (site_cache[cache_entry].valid) {
            rgy_default_context = site_cache[cache_entry].cache_context;
            return error_status_ok;
        }
    }

    /*
     * Check to see if the site that is requested is the same site 
     * that is already opened and stored in the site_cache structure. 
     * When checking the access type of the structure and the access 
     * requested, allow positive evaluation if the access type stored
     * is site_update and the access type requested is site_query. 
     */
    if (site_cache[cache_entry].valid) {
        if (site_cache[cache_entry].access_type == *access ||
            ((site_cache[cache_entry].access_type == site_update) &&
            *access == site_query)) {
            if (*access != site_specific) {
                dce_cf_same_cell_name((char *)site,
                                      (char *)site_cache[cache_entry].cell_name,
                                      &is_same_cell,
                                      &status);
                if (!strcmp((char *)site, (char *)site_cache[cache_entry].site) || is_same_cell)
                    return error_status_ok;
            }
            else
                if (!strcmp((char *)site, (char *)site_cache[cache_entry].site))
                    return error_status_ok;
        }
    }

    if (dcp_is_logged_in()) 
        auth_info.info_type = sec_rgy_bind_auth_dce;
    else
        auth_info.info_type = sec_rgy_bind_auth_none;
    auth_info.tagged_union.dce_info.authn_level= rpc_c_protect_level_pkt_integ;
    auth_info.tagged_union.dce_info.authn_svc = rpc_c_authn_dce_secret;
    auth_info.tagged_union.dce_info.authz_svc = rpc_c_authz_dce;
    auth_info.tagged_union.dce_info.identity  = NULL;

    status = error_status_ok;

    if (!strlen((char *)site))
        strcpy((char *)site, "");

    /*
     * Based on the access type requested, call one of the security binding
     * api's.  These api calls do not return the name of the site that was
     * bound to, so the only way to set the _b(sec) variable is to make a
     * call to sec_rgy_site_get() using the binding retrieved below.
     */
    switch (*access) 
    {
        case site_query:
            sec_rgy_site_bind_query((unsigned_char_t *)site,
                                    &auth_info, 
                                    &new_context,
                                    &status);
            break;

        case site_update:
            sec_rgy_site_bind_update((unsigned_char_t *)site,
                                     &auth_info, 
                                     &new_context,
                                     &status);
            break;

        case site_default:
            sec_rgy_cell_bind((unsigned char *)LOCAL_CELL,
                              &auth_info,
                              &new_context,
                              &status);
            rgy_default_context = new_context;
            return update_site_cache_contents(interp, site, *access, rgy_default_context);

        case site_specific:
            sec_rgy_site_bind((unsigned_char_t *)site, 
                              &auth_info, 
                              &new_context, 
                              &status);
            break;

        default:
            status = sec_rgy_cant_authenticate;
            break;
    }

    if (status == error_status_ok || status == sec_rgy_read_only ||
        status == sec_rgy_cant_authenticate) {
        if (rgy_context)
            site_close(cache_entry);

        /*
         * Lock the write operation
         */
        if (pthread_mutex_lock(&site_list) != 0)
            return dcp_s_mutex_lock_fail;

        rgy_context = new_context;

        if (pthread_mutex_unlock(&site_list) != 0)
            return dcp_s_mutex_unlock_fail;

        /*
         * What site have we opened?
         */
        sec_rgy_site_get(rgy_context, &lsite, &local_status); 
        if (local_status == error_status_ok) {
            strncpy((char *)site, (char *)lsite, sec_rgy_name_max_len);
            free(lsite);

            /*
             * We're willing to give up an update site even if they only
             * request a query site.
             */
            if ((*access == site_query) && (!sec_rgy_site_is_readonly(rgy_context)))
                *access = site_update;
            status = update_site_cache_contents(interp, site, *access, rgy_context);
            if (status != error_status_ok)
                return status;
        }
    }
    else if ((status == sec_rgy_server_unavailable) && (*access == site_update))
        /*
         * The server we tried to bind to was not available.
         */
        status =  sec_rgy_server_unavailable;

    return status;
}


/*
 * FUNCTION: update_site_cache_contents
 *
 * OVERVIEW:
 * Open the site for the specified operation and update the current 
 * the site cache. 
 *
 * INPUTS:
 *  None
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  void
 */
int update_site_cache_contents(Tcl_Interp *interp,
                               site_name_t site, 
                               site_access_t access,
                               sec_rgy_handle_t new_cache_context)
{
    unsigned32                result;
    error_status_t            tmp_st = TCL_OK;
    site_cache_type_t         cache_entry = realtime_cache;
    unsigned_char_t           *cell_name;
    rs_replica_info_t         rgy_info;
    rs_replica_item_t         *item_p;
    char                      output_string[BUFSIZ];
    char                      *bind_site = (char *)NULL;
    char                      *temp_binding;
    sec_rgy_bind_auth_info_t  auth_info;

    auth_info.info_type = sec_rgy_bind_auth_none;
    if (access == site_default)
        cache_entry = default_cache;
    strcpy((char *)site_cache[cache_entry].site, (char *) site);
    site_cache[cache_entry].access_type = access;
    site_cache[cache_entry].valid = true;
    site_cache[cache_entry].cache_context = new_cache_context;
    sec_rgy_site_binding_get_info(new_cache_context,
                                  &cell_name,
                                  NULL, 
                                  NULL,
                                  &auth_info,
                                  &result);
    if (GOOD_STATUS(&result)) {
        strcpy((char *)site_cache[cache_entry].cell_name, (char *)cell_name);
        free(cell_name);
        if (access != site_default) {
            /*
             * Determine if we have a fully qualified replica name
             */
            if (strcmp((char *)site_cache[cache_entry].cell_name,
                (char *)site_cache[cache_entry].site) == 0) {
                /*
                 * Obtain UUID of replica
                 */
                sec_rgy_rep_admin_info(new_cache_context, 
                                       &rgy_info,
                                       &result);
                /*
                 * Here new_cache_context is actually "rgy_context"
                 * Obtain entire replist and find replicas UUID 
                 * from the replist
                 */
                if (GOOD_STATUS(&result)) {
                    result = dcp__rgy_sa_obtain_replist();
                    if (GOOD_STATUS(&result)) {
                        if (dcp__rgy_sa_find_rep_item_by_uuid(&rgy_info.rep_id, &item_p)) {
                            sprintf(output_string, "%s/%s",
                                    (char *)site_cache[cache_entry].cell_name,
                                    (char *)item_p->rep_name);
                            bind_site = dce_strdup(output_string);
                        }
		    }
                }
            }

	    /*
	     * Set _b(sec) only when real-time cache is updated.
	     */
	    if (GOOD_STATUS(&result)) {
	        if (bind_site == NULL) {
                   bind_site = dce_strdup((char *)site_cache[cache_entry].site);
                }

                temp_binding = dcp_binding_string_to_tcl((char *)bind_site,
							 &tmp_st);
                free(bind_site);

		if (temp_binding) {
		    bind_site = dce_strdup(temp_binding);
		    rpc_string_free((unsigned char **)&temp_binding,
				    &tmp_st);
		}
                DCP_GENERAL_SET_B_VAR(SITE_VAR, bind_site); 
                free(bind_site);
	    }
        }
    }

    return result;
}


/*
 * FUNCTION: site_close
 *
 * OVERVIEW:
 * 
 *
 * INPUTS:
 *
 * INPUT/OUTPUTS:
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *    
 */
int site_close(site_cache_type_t cache_entry)
{
    error_status_t           status = 0;
    extern sec_rgy_handle_t  rgy_context;

    if (SITE_CACHE_VALID(site_cache[cache_entry]))
        sec_rgy_site_close(rgy_context, &status);
    else 
        status = error_status_ok;

    return status;
}


/*
 * FUNCTION: rgy_attribute
 *
 * OVERVIEW: This function looks up an attribute using a 
 *    Tcl Interpreter where the known attributes are stored. 
 *    All known attributes are assigned an integer value
 *    this value is returned. 
 * 
 * INPUTS:
 * char *attribute ascii representation of the attribute. 
 *
 * INPUT/OUTPUTS:
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *    The attribute integer value. 
 */
int rgy_attribute(char *attribute)
{
    extern Tcl_Interp  *attr_interp;
    char               command[1024];
    char               *command_format = "lindex [set [ attr_name %s ]] 0";
    int                result = TCL_OK;

    sprintf(command,command_format,attribute);
    result = Tcl_Eval(attr_interp, command);
    return atoi((const char *)attr_interp->result);
}


/*
 * FUNCTION: sec_to_date_string
 *
 * OVERVIEW: This function takes the number of seconds since the epoch and
 *            converts it into a string of the form 1994/12/01.00:00.
 *
 * INPUTS: 
 *     sec_timeval_sec_t secs - Actual number of seconds since the epoch to
 *                              convert.
 *
 * INPUT/OUTPUTS:
 *     char *dt_str - pointer allocatedd with enough space to hold the 
 *                    resultant string;
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  void
 */
void sec_to_date_string(sec_timeval_sec_t  secs,char *dt_str)
{
    timespec_t  unix_time;
    utc_t       utc;

    unix_time.tv_sec = secs;
    unix_time.tv_nsec = 0;

    utc_mkbintime(&utc, &unix_time, NULL, 0);
    if (!utc_asclocaltime(dt_str, UTC_MAX_STR_LEN, &utc))
        return;
    else
        *dt_str = '\0';
}


/*
 * FUNCTION: sec_to_rel_date_string
 *
 * OVERVIEW: This function takes the number of seconds since the epoch and
 *            converts it into a string of the form 1994/12/01.00:00.
 *
 * INPUTS: 
 *     sec_timeval_sec_t secs - Actual number of seconds since the epoch to
 *                              convert.
 *
 * INPUT/OUTPUTS:
 *     char *dt_str - pointer allocatedd with enough space to hold the 
 *                    resultant string;
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  void
 */
void sec_to_rel_date_string(sec_timeval_sec_t  secs,char *dt_str)
{
    reltimespec_t  unix_time;
    utc_t          utc;

    unix_time.tv_sec = secs;
    unix_time.tv_nsec = 0;

    utc_mkbinreltime(&utc, &unix_time, NULL);
    if (!utc_ascreltime(dt_str, UTC_MAX_STR_LEN, &utc))
        return;
    else
        *dt_str = '\0';
}


/*
 * FUNCTION: is_number()
 *
 * OVERVIEW: This function returns an indication of weather or not the 
 *              supplied string is an integer. 
 * 
 *
 * INPUTS:
 *  char *string - character string that may or may not represent a string 
 *                 value
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int   TRUE/FALSE
 */
int is_number(char *string)
{
    int  i;
    int  j = 0;
    int  value;

    i = strlen(string);

    if (*string == '-')
        j++;

    for (; j < i; j++) {
        if (!isdigit(*(string + j)))
            return dcp_rgy_not_int;
    }

    errno = 0;

    value = strtol(string, (char **)NULL, 10);
    if (value == MAXINT  && errno == ERANGE)
        return dcp_rgy_int_out_of_range;

    return dcp_rgy_ok_int;
}


/*
 * FUNCTION: print_attrs
 *
 * OVERVIEW:
 *  This function takes as input a pointer to the structure containing the
 *  information to print out, the attribute name, and the tcl interpreter,
 *  and outputs a formatted string, ready to be printed in tcl fashion.
 *
 * INPUTS:
 *  sec_attr_t *attr_p - Pointer to the structure containing the pertinent information.
 *  sec_attr_encoding_t encoding_type - The encoding type of the attribute to print.
 *
 * INPUT/OUTPUTS:
 *  Tcl_Interp *interp - Structure associated with the interpreter.
 *
 * OUTPUTS:
 *
 * RETURNS:
 *  int result - TCL_OK or some error to notify the interpreter of successful
 *               completion or command failure.
 *
 */
int print_attrs(Tcl_Interp *interp, sec_attr_t *attr_p, sec_attr_encoding_t encode_type)
{
   int              i = 0, j, n;
   int              result = TCL_OK;
   char             *tmp_str;
   char             *tcl_str;
   char             *present_svc;
   char             *unavail_svc;
   char             *unknown_svc;
   unsigned_char_t  *uuid_string;
   unsigned_char_t  *codeset_name;
   error_status_t   tmp_status;

   /*
    * The processing of the attribute value is based on the encoding
    * type of the extended attribute.  Figure out the encoding type
    * and then process the information accordingly.
    */
   switch (encode_type) {
      case sec_attr_enc_void:
         /*
          * No real value here, just output the attribute
          * name and the fact that it is 'present'.
          */
         present_svc = (char *)dce_sprintf(dcp_t_rgy_present);
         if (present_svc == NULL)
            return dcp_s_no_memory;
         Tcl_AppendResult(interp, " ", present_svc, NULL);
         free(present_svc);
         break;

      case sec_attr_enc_type_status:
         /*
          * The attribute is attached to the object, but the 
	  * security server part of the trigger is unavailable.
          */
         tmp_str = (char *)malloc(128);
         if (tmp_str == NULL)
            return dcp_s_no_memory;
	 dce_error_inq_text(SA_ATTR_STATUS(attr_p), st_txt, &dce_status);
         unavail_svc = (char *)dce_sprintf(dcp_t_rgy_unavailable);
         if (unavail_svc == NULL)
            return dcp_s_no_memory;
         sprintf(tmp_str, "{%s: 0x%x, %s}", unavail_svc, SA_ATTR_STATUS(attr_p), st_txt);
         Tcl_AppendResult(interp, " ", tmp_str, NULL);
         free(tmp_str);
         free(unavail_svc);
         break;


      case sec_attr_enc_integer:
         /*
          * There is one integer value associated with an integer type.
          */
         tmp_str = (char *)malloc(128);
         if (tmp_str == NULL)
            return dcp_s_no_memory;
         sprintf(tmp_str, "%d", (SA_ATTR_INTEGER(attr_p)));
         Tcl_AppendResult(interp, " ", tmp_str, NULL);
         free(tmp_str);
         break;

      case sec_attr_enc_printstring:
         /*
          * There is one string associated with a printstring type.
          * The string is put in braces, because we cannot use quotes.
          */
         Tcl_AppendElement(interp, (char *)(SA_ATTR_PRINTSTRING_P(attr_p)));
         break;

      case sec_attr_enc_printstring_array:
         /*
          * There can be multiple strings associated with a printstring_array
          * type.  Each string is put in its own set of braces.  The entire
          * list of strings is also put in a set of braces.
          */
         if (SA_ATTR_STR_ARRAY_P(attr_p)) {
            if (SA_ATTR_STR_ARRAY_NUM(attr_p) == 0) {
               Tcl_AppendResult(interp, " {}", NULL);
            }
            else {
               for (i = 0; i < SA_ATTR_STR_ARRAY_NUM(attr_p); i++) {
                  if (SA_ATTR_STR_ARRAY_NUM(attr_p) > 1) {
                     Tcl_AppendResult(interp, "\n", NULL);
                  }
                  Tcl_AppendElement(interp, (char *)(SA_ATTR_STR_ARRAY_ELT_P(attr_p, i)));
               }
            }
         }
         else
            Tcl_AppendResult(interp, " {}", NULL);
         break;

      case sec_attr_enc_uuid:
         /*
          * There is one uuid string associated with a uuid type.
          * Verify it is legal before printing it out.
          */
         uuid_to_string(&SA_ATTR_UUID(attr_p), &uuid_string, &tmp_status);
         if (tmp_status != uuid_s_ok)
            Tcl_AppendResult(interp, " {}", NULL);
         else {
            Tcl_AppendResult(interp, " ", uuid_string, NULL);
            rpc_string_free(&uuid_string, &tmp_status);
         }
         break;

      case sec_attr_enc_attr_set:
         /*
          * There can be multiple uuids associated with the attr_set
          * type.  Each one is printed on a separate line, and the
          * entire set is enclosed in braces.
          */
         if (SA_ATTR_SET_P(attr_p)) {
            if (SA_ATTR_SET_NUM(attr_p) > 1)
               Tcl_AppendResult(interp, "\n", NULL);
            for (i = 0; i < SA_ATTR_SET_NUM(attr_p); i++) {
               uuid_to_string(&SA_ATTR_SET_MEMBERS(attr_p, i), &uuid_string, &tmp_status);
               if (tmp_status != uuid_s_ok)
                  Tcl_AppendResult(interp, "{}", NULL);
               else {
                  Tcl_AppendResult(interp, " ", uuid_string, NULL);
                  rpc_string_free(&uuid_string, &tmp_status);
               }
               if (i != (SA_ATTR_SET_NUM(attr_p) - 1))
                  Tcl_AppendResult(interp, "\n", NULL);
            }
         }
         else
            Tcl_AppendResult(interp, " {}", NULL);
         break;

      case sec_attr_enc_bytes:
      case sec_attr_enc_confidential_bytes:
         /*
          * The byte string associated with these types must be
          * printed as one continuous string, represented as hex
          * values (\x).  These are formatted with only a certain
          * number of bytes per line, then a continuation marker
          * and a new line are inserted.  This is done to try and
          * produce clean output.
          */
         if (SA_ATTR_BYTES_P(attr_p)) {
            if (SA_ATTR_BYTES_LEN(attr_p) > 0) {
               Tcl_AppendResult(interp, " {", NULL);
               j = 1;
               tmp_str = (char *)malloc(256);
               if (tmp_str == NULL)
                  return dcp_s_no_memory;
               for (i = 0; i < SA_ATTR_BYTES_LEN(attr_p); i++) {
                  sprintf(tmp_str, "%.2x", (SA_ATTR_BYTES_DATA(attr_p, i)));
                  if (i < (SA_ATTR_BYTES_LEN(attr_p) - 1))
                     strcat(tmp_str, " ");
                  Tcl_AppendResult(interp, tmp_str, NULL);
                  if ((j == DCP_BINDING_MAX_BYTES_PER_LINE) && 
                      (i < (SA_ATTR_BYTES_LEN(attr_p) - 1))) {
                     Tcl_AppendResult(interp, "\\\n  ", NULL);
                     j = 1;
                  }
                  else if (i == (SA_ATTR_BYTES_LEN(attr_p) - 1)) {
                     Tcl_AppendResult(interp, "}", NULL);
                     j++;
                  }
                  else
                     j++;
               }
               free(tmp_str);
            }
            else
               Tcl_AppendResult(interp, " {}", NULL);
         }
         else
            Tcl_AppendResult(interp, " {}", NULL);
         break;

      case sec_attr_enc_i18n_data:
         /*
          * The data for this type is treated the same as the byte
          * data type.  The codeset is read from the internal record,
          * and displayed as a string.  The data is also read from
          * the internal record, and is displayed as a continuous byte
          * string, with continuation markers and newlines inserted
          * as needed.  The byte string is enclosed in braces.
          */
         if (SA_ATTR_IDATA_P(attr_p)) {
            dce_cs_rgy_to_loc((SA_ATTR_IDATA_CODESET(attr_p)),
                              &codeset_name,
                              NULL,
                              NULL,
                              &tmp_status);
            if (tmp_status != dce_cs_c_ok) {
               codeset_name = (unsigned_char_t *)dce_sprintf(dcp_t_rgy_unknown);
               if (codeset_name == NULL)
                  return dcp_s_no_memory;
            }
            tmp_str = (char *)malloc(256);
            if (tmp_str == NULL)
               return dcp_s_no_memory;
            if (SA_ATTR_IDATA_LEN(attr_p)) {
               sprintf(tmp_str, " {%s {", codeset_name);
               Tcl_AppendResult(interp, tmp_str, NULL);
               j = (strlen(tmp_str) - 1) / 4;
               if (j > MAX_BYTES_PER_LINE)
                  j = MAX_BYTES_PER_LINE;
               for (i = 0; i < SA_ATTR_IDATA_LEN(attr_p); i++) {
                  sprintf(tmp_str, "\\x%.2x", (SA_ATTR_IDATA_DATA(attr_p, i)));
                  Tcl_AppendResult(interp, tmp_str, NULL);
                  if ((j == MAX_BYTES_PER_LINE) && (i < (SA_ATTR_IDATA_LEN(attr_p) - 1))) {
                     Tcl_AppendResult(interp, "\\\n", NULL);
                     j = 0;
                  }
                  else if (i == (SA_ATTR_IDATA_LEN(attr_p) - 1)) {
                     Tcl_AppendResult(interp, "}}", NULL);
                     j++;
                  }
                  else
                     j++;
               }
            }
            else {
               Tcl_AppendResult(interp, " {", codeset_name, " {}}", NULL);
            }
            free(tmp_str);
         }
         else {
            unknown_svc = (char *)dce_sprintf(dcp_t_rgy_unknown);
            if (unknown_svc == NULL)
               return dcp_s_no_memory;
            Tcl_AppendResult(interp, " {", unknown_svc, " {}}", NULL);
            free(unknown_svc);
         }
         break;

      case sec_attr_enc_binding:
         /*
          * The data for this type is output in a complicated
          * format.  String bindings and name bindings are
          * displayed the same way, but internally stored in
          * a different fashion.  The binding information is
          * enclosed in braces.  The authorization information
          * is also enclosed in braces if a list.
          */
         if (SA_ATTR_BND_INFO_P(attr_p)) {
            if (SA_ATTR_BND_AUTH_INFO_TYPE(attr_p) == sec_attr_bind_auth_dce) {
               Tcl_AppendResult(interp, " {{", authz_name[2], " ", (SA_ATTR_BND_AUTH_SVR_PNAME_P(attr_p)), NULL);
               Tcl_AppendResult(interp, " ", prot_lev_name[SA_ATTR_BND_AUTH_PROT_LEVEL(attr_p)], NULL);
               for (i = 0; i < MAX_AUTHN; i++) {
                  if (SA_ATTR_BND_AUTH_AUTHN_SVC(attr_p) == authn_val[i]) {
                     Tcl_AppendResult(interp, " ", authn_name[i], NULL);
                     break;
                  }
               }
               Tcl_AppendResult(interp, " ", authz_name[SA_ATTR_BND_AUTH_AUTHZ_SVC(attr_p)], "}", NULL);
            }
            else if (SA_ATTR_BND_AUTH_INFO_TYPE(attr_p) == sec_attr_bind_auth_none) {
               Tcl_AppendResult(interp, " {", authz_name[0], NULL);
            }
            else {
               unknown_svc = (char *)dce_sprintf(dcp_t_rgy_unknown);
               if (unknown_svc == NULL)
                  return dcp_s_no_memory;
               Tcl_AppendResult(interp, " {", unknown_svc, NULL);
               free(unknown_svc);
            }

            if (SA_ATTR_BND_NUM(attr_p) == 0)
               Tcl_AppendResult(interp, " {}}", NULL);
            else if (SA_ATTR_BND_NUM(attr_p) == 1)
               Tcl_AppendResult(interp, "\n  ", NULL);
            else
               Tcl_AppendResult(interp, "\n  {", NULL);
            for (i = 0; i < SA_ATTR_BND_NUM(attr_p); i++) {
               switch ((int)SA_ATTR_BND_TYPE(attr_p, i)) {
                  case sec_attr_bind_type_string:
                     tcl_str = (char *)dcp_binding_string_to_tcl(
                        (char *)(SA_ATTR_BND_STRING_P(attr_p, i)), &tmp_status);
                     if (tmp_status == rpc_s_ok) {
                        Tcl_AppendResult(interp, "{", tcl_str, "}", NULL);
                        free(tcl_str);
                     }
                     else
                        Tcl_AppendResult(interp, "{}", NULL);
                     break;

                  case sec_attr_bind_type_svrname:
                     if (SA_ATTR_BND_SVRNAME_P(attr_p, i)->name)
                        Tcl_AppendResult(interp, (SA_ATTR_BND_SVRNAME_P(attr_p, i))->name, NULL);
                     else {
                        unknown_svc = (char *)dce_sprintf(dcp_t_rgy_unknown);
                        if (unknown_svc == NULL)
                           return dcp_s_no_memory;
                        Tcl_AppendResult(interp, unknown_svc, NULL);
                        free(unknown_svc);
                     }
                     break;

                  case sec_attr_bind_type_twrs:
                     tmp_str = (char *)malloc(256);
                     if (tmp_str == NULL)
                        return dcp_s_no_memory;
                     for (j = 0; j < SA_ATTR_BND_TWRSET_COUNT(attr_p, i); j++) {
                        Tcl_AppendResult(interp, "{", NULL);
                        for (n = 0; n < SA_ATTR_BND_TWR_LEN(attr_p, i, j); n++) {
                           sprintf(tmp_str, "%.2x", SA_ATTR_BND_TWR_OCTETS(attr_p, i, j)[n]);
                           Tcl_AppendResult(interp, tmp_str, NULL);
                           if (n < SA_ATTR_BND_TWR_LEN(attr_p, i, j) - 1) {
                              Tcl_AppendResult(interp, " ", NULL);
                           }
                        }
                        Tcl_AppendResult(interp, "}", NULL);
                        if ((SA_ATTR_BND_TWRSET_COUNT(attr_p, i) > 1) &&
                            (j < SA_ATTR_BND_TWRSET_COUNT(attr_p, i) - 1)) {
                           Tcl_AppendResult(interp, "\n   ", NULL);
                        }
                     }
                     free(tmp_str);
                     break;

                  default:
                     /*
                      * If the code gets here, something is wrong.  An unknown
                      * binding type was found.  This is either an internal
                      * error or a data entry error.
                      */
                     result = dcp_s_rgy_era_unknown_binding_type;
                     break;
               }
               if ((i == (SA_ATTR_BND_NUM(attr_p)) - 1) && (i > 0))
                  Tcl_AppendResult(interp, "}}", NULL);
               else if (i == (SA_ATTR_BND_NUM(attr_p)) - 1)
                  Tcl_AppendResult(interp, "}", NULL);
               else
                  Tcl_AppendResult(interp, "\n   ", NULL);
            }
         }
         else
            Tcl_AppendResult(interp, " {}", NULL);
         break;

      default:
         /*
          * If the code gets here, something is wrong.  An unknown
          * encoding type was found.  This might be an internal error.
          */
         result = dcp_s_rgy_era_unknown_encoding_type;
         break;
   }
   return result;
}


/*
 * FUNCTION: show_era
 *
 * OVERVIEW: This function will take as input a domain type (typically
 *  a principal, group, or org) and a name, and display any extended
 *  attribute information assosicated with that name.
 *
 * INPUTS:
 *  signed32 domain - The registry domain in which name belongs.
 *  sec_rgy_name_t name - The name of the object for which extended
 *                        attribute information should be displayed.
 *
 * INPUT/OUTPUTS:
 *  Tcl_Interp *interp - Structure associated with the interpreter.
 *
 * OUTPUTS:
 *
 * RETURNS:
 *
 */
error_status_t show_era(Tcl_Interp *interp, signed32 domain, sec_rgy_name_t name)
{
   int                        i, j;
   error_status_t             status;
   error_status_t             tmp_status;
   unsigned32                 attr_count;
   unsigned32                 cur_num_attrs = 0;
   unsigned32                 num_returned = 0;
   unsigned32                 num_left = 0;
   int                        *used;
   sec_attr_t                 attr_keys[MAX_ATTR_SHOW];
   sec_attr_t                 *out_attrs = NULL;
   sec_attr_cursor_t          attr_cursor;
   sec_attr_schema_entry_t    schema_entry;
   sec_attr_component_name_t  schema_name = NULL;

   /*
    * Initialize the ERA attr cursor.  This will allow us to
    * search through the domain and see how many attributes
    * are attached to this object at the time of the call.
    */
   CLEAR_STATUS(&status);
   sec_rgy_attr_cursor_init(rgy_context,
                            domain,
                            name,
                            &cur_num_attrs,
                            &attr_cursor,
                            &status);
   if (!GOOD_STATUS(&status)) {
      return status;
   }

   if (cur_num_attrs > 0) {
      /*
       * Set aside enough space for the output structures.
       */
      out_attrs = malloc((cur_num_attrs + 2) * sizeof(sec_attr_t));
      if (out_attrs == NULL) {
         sec_rgy_attr_cursor_release(&attr_cursor, &tmp_status);
         return dcp_s_no_memory;
      }
      used = malloc((cur_num_attrs + 2) * sizeof(int));
      if (used == NULL) {
         free((sec_attr_t *)out_attrs);
         sec_rgy_attr_cursor_release(&attr_cursor, &tmp_status);
         return dcp_s_no_memory;
      }

      /*
       * We know there are extra attributes attached to this
       * object.  We will fetch the attribute information all
       * at once, using the knowledge of how many attributes
       * exist.  By setting the number of query attr keys to
       * 0, the 'lookup' function will return all attributes
       * and put them in the out_attrs structure.
       *
       * By fetching the information all at once, we eliminate
       * the need to loop through the 'lookup' call.  If by
       * some chance after the call num_left is not what we
       * expected, it is possible the database changed somewhere
       * between the 'cursor init' call and the 'lookup' call.
       * We will not take this possibility into account.
       */
      sec_rgy_attr_lookup_by_id(rgy_context,
                                domain,
                                name,
                                &attr_cursor,
                                0,
                                cur_num_attrs,
                                attr_keys,
                                &num_returned,
                                out_attrs,
                                &num_left,
                                &status);
      if (status != sec_rgy_status_ok) {
         free((sec_attr_t *)out_attrs);
         free((int *)used);
         sec_rgy_attr_cursor_release(&attr_cursor, &tmp_status);
         return status;
      }

      if ((num_returned > 0) && (num_returned <= cur_num_attrs)) {
         /*
          * We have retrieved some attributes for this object.
          * They are stored, by attr_id, in the out_attrs structure.
          */
         for (j = 0; j < num_returned; j++) {
            used[j] = 0;
         }

         for (i = 0; i < num_returned; i++) {
            /*
             * Check if this value has already been processed.
             */
            if (used[i] == 1) {
               continue;
            }

            /*
             * We need to know how many of this attribute type are
             * going to be processed during this iteration of the
             * loop, so we need a count of all like attributes left
             * in the output structure.
             */
            attr_count = 0;
            for (j = i; j < num_returned; j++) {
               if ((uuid_equal(&out_attrs[i].attr_id, &out_attrs[j].attr_id, &status)) &&
                   (SA_ATTR_ENCODING(&out_attrs[i]) == SA_ATTR_ENCODING(&out_attrs[j])))
                  attr_count++;
            }

            /*
             * Go through the schema and match the attr_id to a name.
             */
            sec_rgy_attr_sch_lookup_by_id(rgy_context,
                                          schema_name,
                                          &out_attrs[i].attr_id,
                                          &schema_entry,
                                          &status);
            if (status != sec_rgy_status_ok) {
               for (j = 0; j < num_returned; j++) {
                  sec_attr_util_inst_free_ptrs(&out_attrs[j]);
               }
               free((sec_attr_t *)out_attrs);
               free((int *)used);
               sec_rgy_attr_cursor_release(&attr_cursor, &tmp_status);
               return status;
            }

            for (j = i; j < num_returned; j++) {
               /*
                * Check if the attribute id's match.
                */
               if ((uuid_equal(&out_attrs[i].attr_id, &out_attrs[j].attr_id, &status)) &&
                   (SA_ATTR_ENCODING(&out_attrs[i]) == SA_ATTR_ENCODING(&out_attrs[j]))) {
                  /*
                   * Check if this is the first occurance of this attribute name.
                   */
                  if (j == i) {
                     /*
                      * Print the attribute name part of the display.
                      */
                     if (skip_era_newline) {
                        skip_era_newline = 0;
                        Tcl_AppendResult(interp, "{", schema_entry.attr_name, NULL);
                     }
                     else {
                        Tcl_AppendResult(interp, "\n{", schema_entry.attr_name, NULL);
                     }
                  }

                  /*
                   * Check if we should print a leading newline.  This
                   * only applies to certain encoding types, and depends
                   * on the number of values to display.
                   */
                  if ((attr_count > 1) &&
                      ((SA_ATTR_ENCODING(&out_attrs[j]) == sec_attr_enc_printstring) ||
                       (SA_ATTR_ENCODING(&out_attrs[j]) == sec_attr_enc_uuid) ||
                       (SA_ATTR_ENCODING(&out_attrs[j]) == sec_attr_enc_bytes) ||
                       (SA_ATTR_ENCODING(&out_attrs[j]) == sec_attr_enc_confidential_bytes) ||
                       (SA_ATTR_ENCODING(&out_attrs[j]) == sec_attr_enc_i18n_data) ||
                       (SA_ATTR_ENCODING(&out_attrs[j]) == sec_attr_enc_binding))) {
                     Tcl_AppendResult(interp, "\n", NULL);
                  }

                  /*
                   * Check for the void special case.  If we've already done one void,
                   * and found others, they should be ignored.
                   */
                  if ((j != i) && (SA_ATTR_ENCODING(&out_attrs[j]) == sec_attr_enc_void)) {
                     used[j] = 1;
                     continue;
                  }

                  /*
                   * Output the value part of the display.
                   */
                  status = print_attrs(interp, &out_attrs[j], SA_ATTR_ENCODING(&out_attrs[j]));
                  if (status != sec_rgy_status_ok) {
                     for (j = 0; j < num_returned; j++) {
                        sec_attr_util_inst_free_ptrs(&out_attrs[j]);
                     }
                     free((sec_attr_t *)out_attrs);
                     free((int *)used);
                     sec_attr_util_sch_ent_free_ptrs(&schema_entry);
                     sec_rgy_attr_cursor_release(&attr_cursor, &tmp_status);
                     return status;
                  }

                  /*
                   * Mark the value as used.
                   */
                  used[j] = 1;
               }
            }

            /*
             * We are back from the inside loop.  Put in the
             * closing bracket for this attribute type.
             */
            Tcl_AppendResult(interp, "}", NULL);

            /*
             * Free the stuff allocated in the schema_entry structure.
             */
            sec_attr_util_sch_ent_free_ptrs(&schema_entry);
         }
      }
      else {
         /*
          * If we get here, there is some problem.  This means that
          * the call to sec_rgy_attr_lookup_by_id() returned a
          * good status and num_returned is either less than one
          * or just not what we expected.  This is some kind of
          * internal software error.
          */
         free((sec_attr_t *)out_attrs);
         free((int *)used);
         sec_rgy_attr_cursor_release(&attr_cursor, &tmp_status);
         return dcp_s_rgy_era_internal_error;
      }

      /*
       * Free the stuff allocated for the attribute structure.
       */
      for (j = 0; j < num_returned; j++) {
         sec_attr_util_inst_free_ptrs(&out_attrs[j]);
      }
      free((sec_attr_t *)out_attrs);
      free((int *)used);
   }

   /*
    * Release the attribute cursor.
    */
   sec_rgy_attr_cursor_release(&attr_cursor, &status);
   if (!GOOD_STATUS(&status)) {
      return status;
   }
   return sec_rgy_status_ok;
}


/*
 * FUNCTION: check_era_info
 *
 * OVERVIEW: This function will take as input a domain type (typically
 *  a principal, group, or org) an object name, the attribute information
 *  structure and the era information and perform various checks to
 *  ensure the data is processed correctly.
 *
 * INPUTS:
 *  signed32 domain - The registry domain in which name belongs.
 *  sec_rgy_name_t name - The name of the object for which extended
 *                        attribute information should be displayed.
 *  sec_attr_t *in_attrs  - Holds the attribute values for any era's specified.
 *
 * INPUT/OUTPUTS:
 *  Tcl_Interp *interp - Structure associated with the interpreter.
 *  rgy_attribute_info_t *attr_info - Holds attribute information, including
 *                                    the count of extended attributes specified.
 *
 * OUTPUTS:
 *  sec_attr_t *out_attrs - Holds the attribute values for any era's that will
 *                          be processed via update.
 *  sec_attr_t *del_attrs - Holds the attribute values for any era's that will
 *                          be processed via delete.
 *
 * RETURNS:
 *  sec_rgy_status_ok on success.
 */
error_status_t check_era_info(Tcl_Interp *interp,
                              signed32 domain,
                              sec_rgy_name_t name,
                              sec_attr_t *in_attrs,
                              rgy_attribute_info_t *attr_info,
                              sec_attr_t *out_attrs,
                              sec_attr_t *del_attrs)
{
    int                        i, j;
    int                        new_count = 0;
    int                        del_count = 0;
    unsigned32                 cur_num_attrs = 0;
    unsigned32                 num_returned = 0;
    unsigned32                 num_left = 0;
    boolean32                  called_from = 1;
    sec_attr_cursor_t          attr_cursor;
    sec_attr_schema_entry_t    schema_entry;
    sec_attr_t                 *temp_attrs = NULL;
    era_info_t                 *era_info = NULL;
    error_status_t             status;
    error_status_t             tmp_status;
    sec_attr_component_name_t  schema_name = NULL;
    any_list_t                 *any_list = NULL;
    boolean32                  any_changed;

    CLEAR_STATUS(&status);
    /*
     * Initialize the ERA attr cursor.  This will allow us to
     * search through the domain and check how many attributes
     * are attached to this object at the time of the call.
     */
    sec_rgy_attr_cursor_init(rgy_context,
                             domain,
                             name,
                             &cur_num_attrs,
                             &attr_cursor,
                             &status);
    if (!GOOD_STATUS(&status)) {
        return status;
    }

    /*
     * Based on the number of existing attributes, assign enough
     * space for the temp_attrs structure.  Add a little bit more
     * space to be safe.
     */
    temp_attrs = malloc((cur_num_attrs + 5) * sizeof(sec_attr_t));
    if (temp_attrs == NULL) {
        sec_rgy_attr_cursor_release(&attr_cursor, &tmp_status);
        return dcp_s_no_memory;
    }

    if (cur_num_attrs > 0) {
        /*
         * We now know there are extra attributes attached to this
         * object.  Using the attribute cursor, get a record of the
         * extended attributes that are present.
         *
         * Because the upcomming call to lookup_by_id does not
         * handle individual sec_attr_t records that contain
         * an encoding type of 'any', we may have to play with
         * the records in in_attrs, making sure no any's exist.
         * The lookup_by_id function will return the records
         * we want (with respect to encoding type any regardless
         * of the encoding type passed since it keys on the id.
         * So, change records of encoding type 'any' to 'void',
         * make the lookup call, then change them back, since the
         * modify era function will need this information.
         */
        any_changed = FALSE;
        if (attr_info->modify_type == modify_type_remove) {
            any_list = (any_list_t *)malloc(attr_info->era_count * sizeof(any_list_t));
            if (any_list == NULL) {
                sec_rgy_attr_cursor_release(&attr_cursor, &tmp_status);
                free((sec_attr_t *)temp_attrs);
                return dcp_s_no_memory;
            }
            memset(any_list, -1, attr_info->era_count*sizeof(any_list_t));
            for (i = 0; i < attr_info->era_count; i++) {
                if (SA_ATTR_ENCODING(&in_attrs[i]) == sec_attr_enc_any) {
                    any_changed = TRUE;
                    any_list[i].index = sec_attr_enc_any;
                    in_attrs[i].attr_value.attr_encoding = sec_attr_enc_void;
                }
            }
        }

        /*
         * The variable cur_num_attrs represents the number of
         * attributes that exist on this object.  We only have
         * to call the lookup function once, passing this number
         * as the 'space available' parameter.  If the num_left
         * parameter is greater than zero after this call, it means
         * the database changed between the cursor init call and
         * the lookup call.  We can safely ignore these extras,
         * since this is a dynamic environment.  The next call
         * to this function would pick up the new information.
         */
        sec_rgy_attr_lookup_by_id(rgy_context,
                                  domain,
                                  name,
                                  &attr_cursor,
                                  attr_info->era_count,
                                  cur_num_attrs,
                                  in_attrs,
                                  &num_returned,
                                  temp_attrs,
                                  &num_left,
                                  &status);
        if (STATUS_EQUAL(&status, sec_attr_no_more_entries)) {
            CLEAR_STATUS(&status);
        }

        /*
         * Reset records back to encoding type any, if necessary.
         */
        if ((GOOD_STATUS(&status)) && (any_changed == TRUE)) {
            for (i = 0; i < attr_info->era_count; i++) {
                if (any_list[i].index == sec_attr_enc_any) {
                    in_attrs[i].attr_value.attr_encoding = sec_attr_enc_any;
                }
            }
        }
    }
 
    /*
     * Release the attribute cursor and check the results of the lookup call.
     */
    sec_rgy_attr_cursor_release(&attr_cursor, &tmp_status);
    if ((!GOOD_STATUS(&status)) || (!GOOD_STATUS(&tmp_status))) {
        /*
         * Free the internal pointers and the structure pointers and return.
         */
        for (i = 0; i < num_returned; i++) {
            sec_attr_util_inst_free_ptrs(&temp_attrs[i]);
        }
        free((sec_attr_t *)temp_attrs);
        if (attr_info->modify_type == modify_type_remove) {
            free((any_list_t *)any_list);
        }
        if (!GOOD_STATUS(&status))
            return status;
        else
            return tmp_status;
    }
 
    /*
     * If the request is modify_type_add, the era_info array is needed
     * to perform modification.
     */
    if (attr_info->modify_type == modify_type_add) {
        era_info = (era_info_t *) malloc(attr_info->era_count * sizeof(era_info_t));
        if (era_info == NULL) {
            for (i = 0; i < num_returned; i++) {
                sec_attr_util_inst_free_ptrs(&temp_attrs[i]);
            }
            free((sec_attr_t *)temp_attrs);
            return dcp_s_no_memory;
        }
        memset(era_info, 0, attr_info->era_count*sizeof(era_info_t));
 
        for (i = 0; i < attr_info->era_count; i++) {
            /*
             * Get more information about this attribute type.  The
             * 'multi_valued' field of the schema structure will help
             * control further processing.
             */
            schema_entry = s_null_sch_entry;
            sec_rgy_attr_sch_lookup_by_id(rgy_context,
                                          schema_name,
                                          &in_attrs[i].attr_id,
                                          &schema_entry,
                                          &status);
            if (!GOOD_STATUS(&status)) {
                for (j = 0; j < num_returned; j++) {
                    sec_attr_util_inst_free_ptrs(&temp_attrs[j]);
                }
                free((sec_attr_t *)temp_attrs);
                free((era_info_t *)era_info);
                return status;
            }
            era_info[i].attr_id = schema_entry.attr_id;
            era_info[i].schema_entry_flags = schema_entry.schema_entry_flags;
            sec_attr_util_sch_ent_free_ptrs(&schema_entry);
        }
    }
 
    /*
     * Ask the modify function to do the work
     */
    status = modify_era_info(interp,
                             called_from,
                             (unsigned32)attr_info->modify_type,
                             (unsigned32)attr_info->era_count,
                             (boolean32)attr_info->types,
                             in_attrs,
                             era_info,
                             num_returned,
                             (num_returned) ? temp_attrs : NULL,
                             (unsigned32 *)&new_count,
                             out_attrs,
                             (unsigned32 *)&del_count,
                             del_attrs);
 
    /*
     * At this point, all processing is done.  Clean up whatever we
     * allocated prior to the major loop, including internally allocated
     * sec_attr_t attribute stuff.
     */
    for (i = 0; i < num_returned; i++) {
        sec_attr_util_inst_free_ptrs(&temp_attrs[i]);
    }
    free((sec_attr_t *)temp_attrs);
    if (attr_info->modify_type == modify_type_remove) {
        free((any_list_t *)any_list);
    }
    else if (attr_info->modify_type == modify_type_add) {
        free((era_info_t *)era_info);
    }
 
    /*
     * Update the era count and return.
     */
    if (GOOD_STATUS(&status)) {
        attr_info->era_count = new_count;
        attr_info->del_count = del_count;
    }
    return status;
 }


/*
 * FUNCTION: modify_era_info
 *
 * OVERVIEW: This function will take as input a group of new attributes and
 * their era information, a group of existing attributes and modification type,
 * and perform requested modification to existing attributes to produce a
 * group of attributes-after-modification.
 *
 * INPUTS:
 *  boolean32 called_from - The source of the request, 1 for security, 0 for generic.
 *  unsigned32 modify_type - The requested modification type.
 *  unsigned32 in_count  - The number of new attributes.
 *  boolean32 remove_by_type - Whether the remove is by value, or by entire attribute.
 *  sec_attr_t *in_attrs  - The new attribute values for any era's specified.
 *  era_info_t *era_info - The new era attributes info.
 *
 *  unsigned32 exist_count  - The number of existing attributes.
 *  sec_attr_t*exist_attrs  - The existing attribute values.
 *
 * INPUT/OUTPUTS:
 *  Tcl_Interp *interp - Structure associated with the interpreter.
 *
 * OUTPUTS:
 *  unsigned32 *out_count - The number of attributes-after-modification to update.
 *  sec_attr_t *out_attrs - Holds the attribute values for any era's that will
 *                          be processed via update.
 *  unsigned32 *del_count - The number of attributes-after-modification to delete.
 *  sec_attr_t *del_attrs - Holds the attribute values for any era's that will
 *                          be processed via delete.
 *
 * RETURNS:
 *  sec_rgy_status_ok on success.
 *
 */
error_status_t modify_era_info(Tcl_Interp *interp,
                               boolean32  called_from,
                               unsigned32 modify_type,
                               unsigned32 in_count,
                               boolean32  remove_by_type,
                               sec_attr_t *in_attrs,
                               era_info_t *era_info,
                               unsigned32 exist_count,
                               sec_attr_t *exist_attrs,
                               unsigned32 *out_count,
                               sec_attr_t *out_attrs,
                               unsigned32 *delete_count,
                               sec_attr_t *del_attrs)
{
    int                           i, j, k, index;
    int                           pass = 0;
    int                           new_count = 0;
    int                           del_count = 0;
    int                           match_count = 0;
    boolean32                     same_found;
    boolean32                     other_found;
    boolean32                     match_found;
    boolean32                     special_case = FALSE;
    boolean32                     multiple_found = FALSE;
    unsigned32                    special_index;
    unsigned32                    match_index = exist_count;
    era_check_table_t             *check_table;
    error_status_t                status;
    error_status_t                tmp_status;
    sec_attr_value_t              *in_ptr, *out_ptr, *exist_ptr;
    sec_attr_schema_entry_t       schema_entry;
    sec_attr_component_name_t     schema_name = NULL;
    extern dce_attr_sch_handle_t  attr_schema_binding;
    char                          *str_ptr1, *str_ptr2;
    int                           size1, size2;
    int                           existing_members = 0;
    int                           new_members = 0;

    CLEAR_STATUS(&status);

    /*
     * Check if there are extended attributes associated with this object.
     * If not, and the modify type is '-change' or '-remove', then report
     * an error; if the modify type is '-add', just process them all.
     */
    if ((!exist_attrs) || (!exist_count)) {
        switch ((int)modify_type) {
            case modify_type_add:
                /*
                 * There are only new attributes available.
                 */
                for (i = 0; i < in_count; i++) {
                    sec_attr_util_alloc_copy(NULL, &in_attrs[i], &out_attrs[i], &status);
                    if (!GOOD_STATUS(&status)) {
                        for (j = 0; j < i; j++) {
                            sec_attr_util_inst_free_ptrs(&out_attrs[j]);
                        }
                        return status;
                    }
                }
                *out_count = in_count;
                break;
            case modify_type_change:
            case modify_type_remove:
                status = dcp_s_rgy_era_no_attribute_exists;
                break;
            default:
                /*
                 * If we get here, that means we are in the modify routine,
                 * but the modify type is somehow set incorrectly.  This is
                 * an internal error.
                 */
                status = dcp_s_rgy_era_internal_error;
                break;
        }
        return status;
    }

    /*
     * If we get here, it means that we need to perform the requested
     * modification on the existing attribute.  Check to see that the
     * era_info array is defined only when a modify_type_add is requested.
     */
    if ((modify_type == modify_type_add) && (era_info == NULL)) {
        status = dcp_s_rgy_era_internal_error;
        return status;
    }

    /*
     * Malloc space for the check_table structure, based on
     * the current number of existing attributes.
     */
    check_table = malloc(exist_count * sizeof(era_check_table_t));
    if (check_table == NULL) {
        return dcp_s_no_memory;
    }

    /*
     * Clear the fixed part of the check table.  Assign
     * the type_id's of the existing attributes.
     */
    for (i = 0; i < exist_count; i++) {
        check_table[i].used = FALSE;
        check_table[i].special_case = FALSE;
        check_table[i].any_case = FALSE;
        check_table[i].attr_id = exist_attrs[i].attr_id;
    }

    /*
     * We need to compare each of the attributes the user is trying
     * to modify with the existing attribute information found.
     */
    for (i = 0; i < in_count; i++) {
        /*
         * Clear the variable part of the check table.
         */
        for (j = 0; j < exist_count; j++) {
            check_table[j].same_found = FALSE;
            check_table[j].other_found = FALSE;
        }
        same_found = FALSE;
        other_found = FALSE;
        special_case = FALSE;
        special_index = exist_count;

        /*
         * Go thru the schema and match the attr_id, so we can
         * get the real encoding type of the input attribute.
         */
        if (called_from == 0) {
            /*
             * Use the generic schema.
             */
            dce_attr_sch_lookup_by_id(attr_schema_binding,
                                      &in_attrs[i].attr_id,
                                      &schema_entry,
                                      &status);
            if (status != sec_rgy_status_ok) {
                free((era_check_table_t *)check_table);
                DCP_SET_MISC_ERROR(status);
            }
        }
        else if (called_from == 1) {
            /*
             * Use the security schema.
             */
            sec_rgy_attr_sch_lookup_by_id(rgy_context,
                                          schema_name,
                                          &in_attrs[i].attr_id,
                                          &schema_entry,
                                          &status);
            if (status != sec_rgy_status_ok) {
                free((era_check_table_t *)check_table);
                DCP_SET_MISC_ERROR(status);
            }
        }
        else {
            /*
             * This function was called incorrectly, so
             * it must be some kind of internal error.
             */
            free((era_check_table_t *)check_table);
            DCP_SET_MISC_ERROR(dcp_s_rgy_era_internal_error);
        }

        /*
         * If the encoding type of the input attribute being processed
         * is encoding type any, then set the any_case flag to true.
         */
        if (schema_entry.attr_encoding == sec_attr_enc_any) {
            check_table[i].any_case = TRUE;
        }

        /*
         * Free the stuff allocated in the schema_entry structure.
         */
	dhd__schema_ent_free_ptrs(&schema_entry);

        /*
         * Search through each attribute found in the database (i.e.
         * exist_attrs) and see if an attribute-value pair that already
         * exists matches the new one.
         */
        for (j = 0; j < exist_count; j++) {
            /*
             * Do the value comparison only if the attribute type ids
             * and the attribute encoding types match.
             */
            if ((uuid_equal(&in_attrs[i].attr_id, &exist_attrs[j].attr_id, &tmp_status)) &&
                (SA_ATTR_ENCODING(&in_attrs[i]) == SA_ATTR_ENCODING(&exist_attrs[j]))) {

                /*
                 * If the input case we just matched is an encoding type any
                 * in real life, propagate the any_case flag being set down to
                 * this exist case as well.
                 */
                if ((check_table[i].any_case == TRUE) && (i != j)) {
                    check_table[j].any_case = TRUE;
                }

                /*
                 * Each encoding type must be handled differently.
                 */
                switch (SA_ATTR_ENCODING(&exist_attrs[j])) {
                    case sec_attr_enc_void:
                        if (modify_type == modify_type_add) {
                            status = dcp_s_rgy_era_void_already_exists;
                        }
                        else {
                            same_found = TRUE;
                            check_table[j].same_found = TRUE;
                            match_index = j;
                        }
                        break;
                    case sec_attr_enc_integer:
                        if (remove_by_type == FALSE) {
                            if (in_attrs[i].attr_value.tagged_union.signed_int ==
                                exist_attrs[j].attr_value.tagged_union.signed_int) {
                                same_found = TRUE;
                                check_table[j].same_found = TRUE;
                                match_index = j;
                            }
                            else {
                                other_found = TRUE;
                                check_table[j].other_found = TRUE;
                            }
                        }
                        else {
                            same_found = TRUE;
                            match_index = j;
                            if (check_table[j].any_case == TRUE) {
                                check_table[j].same_found = TRUE;
                            }
                        }
                        break;
                    case sec_attr_enc_printstring:
                        if (remove_by_type == FALSE) {
                            str_ptr1 = (char *)in_attrs[i].attr_value.tagged_union.printstring;
                            str_ptr2 = (char *)exist_attrs[j].attr_value.tagged_union.printstring;
                            if (strcmp(str_ptr1, str_ptr2) == 0) {
                                same_found = TRUE;
                                check_table[j].same_found = TRUE;
                                match_index = j;
                            }
                            else {
                                other_found = TRUE;
                                check_table[j].other_found = TRUE;
                            }
                        }
                        else {
                            same_found = TRUE;
                            match_index = j;
                            if (check_table[j].any_case == TRUE) {
                                check_table[j].same_found = TRUE;
                            }
                        }
                        break;
                    case sec_attr_enc_bytes:
                    case sec_attr_enc_confidential_bytes:
                        if (remove_by_type == FALSE) {
                            if ((!in_attrs[i].attr_value.tagged_union.bytes) ||
                                (!exist_attrs[j].attr_value.tagged_union.bytes)) {
                                status = dcp_s_rgy_era_illegal_attribute_list;
                                break;
                            }
                            size1 = in_attrs[i].attr_value.tagged_union.bytes->length;
                            size2 = exist_attrs[j].attr_value.tagged_union.bytes->length;
                            str_ptr1 = (char *)in_attrs[i].attr_value.tagged_union.bytes->data;
                            str_ptr2 = (char *)exist_attrs[j].attr_value.tagged_union.bytes->data;
                            if ((size1 == size2) && (strncmp(str_ptr1, str_ptr2, size1) == 0)) {
                                same_found = TRUE;
                                check_table[j].same_found = TRUE;
                                match_index = j;
                            }
                            else {
                                other_found = TRUE;
                                check_table[j].other_found = TRUE;
                            }
                        }
                        else {
                            same_found = TRUE;
                            match_index = j;
                            if (check_table[j].any_case == TRUE) {
                                check_table[j].same_found = TRUE;
                            }
                        }
                        break;
                    case sec_attr_enc_i18n_data:
                        if (remove_by_type == FALSE) {
                            if ((!in_attrs[i].attr_value.tagged_union.idata) ||
                                (!exist_attrs[j].attr_value.tagged_union.idata)) {
                                status = dcp_s_rgy_era_illegal_attribute_list;
                                break;
                            }
                            size1 = in_attrs[i].attr_value.tagged_union.idata->length;
                            size2 = exist_attrs[j].attr_value.tagged_union.idata->length;
                            str_ptr1 = (char *)in_attrs[i].attr_value.tagged_union.idata->data;
                            str_ptr2 = (char *)exist_attrs[j].attr_value.tagged_union.idata->data;
                            if ((in_attrs[i].attr_value.tagged_union.idata->codeset ==
                                exist_attrs[j].attr_value.tagged_union.idata->codeset) &&
                                (size1 == size2) &&
                                (strncmp(str_ptr1, str_ptr2, size1) == 0)) {
                                same_found = TRUE;
                                check_table[j].same_found = TRUE;
                                match_index = j;
                            }
                            else {
                                other_found = TRUE;
                                check_table[j].other_found = TRUE;
                            }
                        }
                        else {
                            same_found = TRUE;
                            match_index = j;
                            if (check_table[j].any_case == TRUE) {
                                check_table[j].same_found = TRUE;
                            }
                        }
                        break;
                    case sec_attr_enc_uuid:
                        if (remove_by_type == FALSE) {
                            if (uuid_equal(&in_attrs[i].attr_value.tagged_union.uuid,
                                           &exist_attrs[j].attr_value.tagged_union.uuid,
                                           &tmp_status)) {
                                same_found = TRUE;
                                check_table[j].same_found = TRUE;
                                match_index = j;
                            }
                            else {
                                other_found = TRUE;
                                check_table[j].other_found = TRUE;
                            }
                        }
                        else {
                            same_found = TRUE;
                            match_index = j;
                            if (check_table[j].any_case == TRUE) {
                                check_table[j].same_found = TRUE;
                            }
                        }
                        break;
                    case sec_attr_enc_printstring_array:
                        if (remove_by_type == FALSE) {
                            if ((!in_attrs[i].attr_value.tagged_union.string_array) ||
                                (!exist_attrs[j].attr_value.tagged_union.string_array)) {
                                status = dcp_s_rgy_era_illegal_attribute_list;
                                break;
                            }
                            special_case = TRUE;
                            special_index = j;
                            check_table[j].special_case = TRUE;
                        }
                        else {
                            same_found = TRUE;
                            match_index = j;
                            if (check_table[j].any_case == TRUE) {
                                check_table[j].same_found = TRUE;
                            }
                        }
                        break;
                    case sec_attr_enc_attr_set:
                        if (remove_by_type == FALSE) {
                            if ((!in_attrs[i].attr_value.tagged_union.attr_set) ||
                                (!exist_attrs[j].attr_value.tagged_union.attr_set)) {
                                status = dcp_s_rgy_era_illegal_attribute_list;
                                break;
                            }
                            special_case = TRUE;
                            special_index = j;
                            check_table[j].special_case = TRUE;
                        }
                        else {
                            same_found = TRUE;
                            match_index = j;
                            if (check_table[j].any_case == TRUE) {
                                check_table[j].same_found = TRUE;
                            }
                        }
                        break;
                    case sec_attr_enc_binding:
                        if (remove_by_type == FALSE) {
                            if (match_bindings(in_attrs[i].attr_value.tagged_union.binding,
                                               exist_attrs[j].attr_value.tagged_union.binding)) {
                                same_found = TRUE;
                                check_table[j].same_found = TRUE;
                                match_index = j;
                            }
                            else {
                                other_found = TRUE;
                                check_table[j].other_found = TRUE;
                            }
                        }
                        else {
                            same_found = TRUE;
                            match_index = j;
                            if (check_table[j].any_case == TRUE) {
                                check_table[j].same_found = TRUE;
                            }
                        }
                        break;
                    default:
                        /*
                         * The internal encoding type is unknown, must be
                         * some kind of fatal internal error.
                         */
                        status = dcp_s_rgy_era_unknown_encoding_type;
                        break;
                }

                /*
                 * If we just processed an attribute of encoding type any, and we
                 * are doing a delete, either by encoding type or by value, then we
                 * have to check if more of this attribute exist, but with a different
                 * encoding type.  If so, only the attributes of the given encoding
                 * type (or value) should be removed, the others have to stay.
                 */
                if ((check_table[j].any_case == TRUE) && (modify_type == modify_type_remove)) {
                    for (k = 0; k < exist_count; k++) {
                        if ((uuid_equal(&in_attrs[i].attr_id, &exist_attrs[k].attr_id, &tmp_status)) &&
                            (SA_ATTR_ENCODING(&in_attrs[i]) != SA_ATTR_ENCODING(&exist_attrs[k]))) {
                            other_found = TRUE;
                            check_table[k].any_case = TRUE;
                            check_table[k].other_found = TRUE;
                        }
                    }
                }
            }
            else if ((uuid_equal(&in_attrs[i].attr_id, &exist_attrs[j].attr_id, &tmp_status)) &&
                     (SA_ATTR_ENCODING(&in_attrs[i]) == sec_attr_enc_any)) {
                /*
                 * This is the signal to delete all instances of the attribute
                 * with encoding type any, regardless of the encoding type used
                 * in the existing 'any' attributes.
                 */
                 same_found = TRUE;
                 match_index = j;
                 break;
            }

            if (!GOOD_STATUS(&status)) {
                break;
            }
        }

        /*
         * If an error has been seen, don't need go any further
         */
        if (!GOOD_STATUS(&status)) {
            break;
        }

        /*
         * When we get here, the current extended attribute has been
         * compared to each existing extended attribute.  Processing now
         * depends on which modify operation was selected, the status
         * of the 'found' variables, the schema fields, the phase of the
         * moon, and which direction the wind is blowing.
         */
        switch ((int)modify_type) {
            case modify_type_remove:
                if (special_case == TRUE) {
                    /*
                     * We have an attribute type that, by its definition,
                     * can have multiple values stored in a single instance.
                     * Any work the user want's done to this attribute will
                     * be done in this instance.  Currently, this applies to
                     * the encoding types "printstring_array" and "attr_set".
                     * A remove here means the user is trying to remove one
                     * or more of the values stored in that single instance.
                     * If each value being removed is a legal value, then the
                     * entire input structure can be sent to the remove function.
                     * Unless it is an encoding type 'any'.  Then, we will have
                     * to check if other instances exist under a different
                     * encoding type.
                     */

                    /*
                     * Assign pointers to the value portions of the attribute
                     * structures.
                     */
                    in_ptr = &in_attrs[i].attr_value;
                    exist_ptr = &exist_attrs[special_index].attr_value;
                    check_table[special_index].used = TRUE;

                    /*
                     * Process the values based on the encoding type of the
                     * attribute.
                     */
                    switch (in_ptr->attr_encoding) {
                        case sec_attr_enc_attr_set:
                            new_members = in_ptr->tagged_union.attr_set->num_members;
                            existing_members = exist_ptr->tagged_union.attr_set->num_members;

                            /*
                             * Do a quick check to ensure that no values are
                             * repeated on the removal line.  This is an error.
                             */
                            j = 0;
                            match_found = FALSE;
                            while ((j < (new_members - 1)) && (match_found == FALSE)) {
                                for (k = j + 1; k < new_members; k++) {
                                    if (uuid_equal(&in_ptr->tagged_union.attr_set->members[j],
                                                   &in_ptr->tagged_union.attr_set->members[k],
                                                   &tmp_status)) {
                                        match_found = TRUE;
                                        break;
                                    }
                                }
                                j++;
                            }
                            if (match_found == TRUE) {
                                status = dcp_s_rgy_era_illegal_attribute_list;
                                break;
                            }

                            /*
                             * Do a quick check to ensure that the values specified for
                             * removal all are existing values.  If not, this is an 
                             * error.
                             */
                            match_count = 0;
                            for (j = 0; j < new_members; j++) {
                                match_found = FALSE;
                                for (k = 0; k < existing_members; k++) {
                                    if (uuid_equal(&in_ptr->tagged_union.attr_set->members[j],
                                                   &exist_ptr->tagged_union.attr_set->members[k],
                                                   &tmp_status)) {
                                        match_found = TRUE;
                                        match_count++;
                                    }
                                }
                                if (match_found == FALSE) {
                                    status = dcp_s_rgy_era_no_attribute_exists;
                                    break;
                                }
                            }
                            if (!GOOD_STATUS(&status)) {
                                break;
                            }
      
                            if ((existing_members - match_count) < 0) {
                                /*
                                 * We are somehow trying to remove more elements than
                                 * actually exist.  This is an internal error.
                                 */
                                status = dcp_s_rgy_era_internal_error;
                                break;
                            }
                            else if ((existing_members - match_count) == 0) {
                                /*
                                 * The user is trying to remove every value from the
                                 * current instance.  Submit the entry for removal.
                                 * If we are removing all instances of a specified
                                 * encoding type from an encoding type 'any', then
                                 * we have to check for the existence of other records.
                                 */
                                if (check_table[special_index].any_case == TRUE) {
                                    for (j = 0; j < exist_count; j++) {
                                        if (check_table[j].other_found == TRUE) {
                                            break;
                                        }
                                    }
                                    if (j == exist_count) {
                                        sec_attr_util_alloc_copy(NULL,
                                                                 &in_attrs[i],
                                                                 &del_attrs[del_count++],
                                                                 &status);
                                        if (!GOOD_STATUS(&status)) {
                                            del_count--;
                                            break;
                                        }
                                    }
                                }
                                else {
                                    sec_attr_util_alloc_copy(NULL,
                                                             &in_attrs[i],
                                                             &del_attrs[del_count++],
                                                             &status);
                                    if (!GOOD_STATUS(&status)) {
                                        del_count--;
                                        break;
                                    }
                                }
                            }
                            else {
                                /*
                                 * Due to limitations in the security api, we have
                                 * to micky-mouse the removal by building a new
                                 * structure for this attribute, containing only
                                 * the values the user is not trying to remove.
                                 */
                                out_ptr = &out_attrs[new_count].attr_value;
                                out_attrs[new_count] = in_attrs[i];
                                size1 = (existing_members - match_count) * 
                                        sizeof(uuid_t) +
                                        sizeof(in_ptr->tagged_union.attr_set->num_members);
                                out_ptr->tagged_union.attr_set = 
                                        (sec_attr_enc_attr_set_t *) malloc(size1);
                                if (out_ptr->tagged_union.attr_set == NULL) {
                                    status = dcp_s_no_memory;
                                    break;
                                }
                                memset(out_ptr->tagged_union.attr_set, 0, size1);
 
                                index = 0;
                                for (j = 0; j < existing_members; j++) {
                                    match_found = FALSE;
                                    for (k = 0; k < new_members; k++) {
                                        if (uuid_equal(&exist_ptr->tagged_union.attr_set->members[j],
                                                       &in_ptr->tagged_union.attr_set->members[k],
                                                       &tmp_status)) {
                                            match_found = TRUE;
                                            break;
                                        }
                                    }
                                    if (match_found == FALSE) {
                                        out_ptr->tagged_union.attr_set->members[index++] =
                                            exist_ptr->tagged_union.attr_set->members[j];
                                    }
                                }
                                out_ptr->tagged_union.attr_set->num_members = index;
                                new_count++;
                            }
                            break;

                        case sec_attr_enc_printstring_array:
                            new_members = in_ptr->tagged_union.string_array->num_strings;
                            existing_members = exist_ptr->tagged_union.string_array->num_strings;

                            /*
                             * Do a quick check to ensure that no values are repeated
                             * on the removal line.  This is an error.
                             */
                            j = 0;
                            match_found = FALSE;
                            while ((j < (new_members - 1)) && (match_found == FALSE)) {
                                str_ptr1 = (char *)in_ptr->tagged_union.string_array->strings[j];
                                for (k = j + 1; k < new_members; k++) {
                                    str_ptr2 = (char *)
                                        in_ptr->tagged_union.string_array->strings[k];
                                    if (strcmp(str_ptr1, str_ptr2) == 0) {
                                        match_found = TRUE;
                                        break;
                                    }
                                }
                                j++;
                            }
                            if (match_found == TRUE) {
                                status = dcp_s_rgy_era_illegal_attribute_list;
                                break;
                            }

                            /*
                             * Do a quick check to ensure that the values specified for
                             * removal all are existing values.  If not, This is an 
                             * error.
                             */
                            match_count = 0;
                            for (j = 0; j < new_members; j++) {
                                match_found = FALSE;
                                str_ptr1 = (char *)
                                    in_ptr->tagged_union.string_array->strings[j];
                                for (k = 0; k < existing_members; k++) {
                                    str_ptr2 = (char *)
                                        exist_ptr->tagged_union.string_array->strings[k];
                                    if (strcmp(str_ptr1, str_ptr2) == 0) {
                                        match_found = TRUE;
                                        match_count++;
                                    }
                                }
                                if (match_found == FALSE) {
                                    status = dcp_s_rgy_era_no_attribute_exists;
                                    break;
                                }
                            }
                            if (!GOOD_STATUS(&status)) {
                                break;
                            }

                            if ((existing_members - match_count) < 0) {
                                /*
                                 * We are somehow trying to remove more elements than
                                 * actually exist.  This is an internal error.
                                 */
                                status = dcp_s_rgy_era_internal_error;
                                break;
                            }
                            else if ((existing_members - match_count) == 0) {
                                /*
                                 * The user is trying to remove every value from the
                                 * current instance.  Submit the entry for removal.
                                 * If we are removing all instances of a specified
                                 * encoding type from an encoding type 'any', then
                                 * we have to check for the existence of other records.
                                 */
                                if (check_table[special_index].any_case == TRUE) {
                                    for (j = 0; j < exist_count; j++) {
                                        if (check_table[j].other_found == TRUE) {
                                            break;
                                        }
                                    }
                                    if (j == exist_count) {
                                        sec_attr_util_alloc_copy(NULL,
                                                                 &in_attrs[i],
                                                                 &del_attrs[del_count++],
                                                                 &status);
                                        if (!GOOD_STATUS(&status)) {
                                            del_count--;
                                            break;
                                        }
                                    }
                                }
                                else {
                                    sec_attr_util_alloc_copy(NULL,
                                                             &in_attrs[i],
                                                             &del_attrs[del_count++],
                                                             &status);
                                    if (!GOOD_STATUS(&status)) {
                                        del_count--;
                                        break;
                                    }
                                }
                            }
                            else {
                                /*
                                 * Due to limitations in the security api, we have
                                 * to micky-mouse the removal by building a new
                                 * structure for this attribute, containing only
                                 * the values the user is not trying to remove.
                                 */
                                out_ptr = &out_attrs[new_count].attr_value;
                                out_attrs[new_count] = in_attrs[i];
                                size1 = (existing_members - match_count) *
                                        sizeof(sec_attr_enc_printstring_p_t) +
                                        sizeof(in_ptr->tagged_union.string_array->num_strings);
                                out_ptr->tagged_union.string_array = 
                                    (sec_attr_enc_str_array_t *) malloc(size1);
                                if (out_ptr->tagged_union.string_array == NULL) {
                                    status = dcp_s_no_memory;
                                    break;
                                }
                                memset(out_ptr->tagged_union.string_array, 0, size1);

                                index = 0;
                                for (j = 0; j < existing_members; j++) {
                                    match_found = FALSE;
                                    str_ptr1 = (char *)
                                        exist_ptr->tagged_union.string_array->strings[j];
                                    for (k = 0; k < new_members; k++) {
                                        str_ptr2 = (char *)
                                            in_ptr->tagged_union.string_array->strings[k];
                                        if (strcmp(str_ptr1, str_ptr2) == 0) {
                                            match_found = TRUE;
                                            break;
                                        }
                                    }
                                    if (match_found == FALSE) {
                                        out_ptr->tagged_union.string_array->strings[index++] =
                                            (unsigned_char_t *)dce_strdup(str_ptr1);
                                    }
                                }
                                out_ptr->tagged_union.string_array->num_strings = index;
                                new_count++;
                            }
                            break;

                        default:
                            /*
                             * We have either an encoding type that is marked as a
                             * special case and shouldn't be, or simply an unknown
                             * encoding type.  Regardless, this is an internal error.
                             */
                            status = dcp_s_rgy_era_internal_error;
                            break;
                    }
                }
                else if (same_found == FALSE) {
                    /*
                     * Trying to remove an attribute that does not exist.  Error.
                     */
                    status = dcp_s_rgy_era_no_attribute_exists;
                }
                else if ((same_found == TRUE) && (other_found == FALSE)) {
                    /*
                     * Trying to remove an attribute that does exist, and
                     * it is the only attribute of it's kind in the database.
                     * Pass the information along in the delete structure.
                     * In the case of removal_by_type, or in the case of
                     * remove by value (the last value), we copy the matched
                     * existed era to del_attrs just to make the
                     * sec_attr_util_alloc_copy() function happy.
                     */
                    sec_attr_util_alloc_copy(NULL,
                                             &exist_attrs[match_index],
                                             &del_attrs[del_count++],
                                             &status);
                    if (!GOOD_STATUS(&status)) {
                        del_count--;
                        break;
                    }
                }
                else {
                    /*
                     * Trying to remove one instance of an attribute when other 
                     * ones also exist.  We can do this by sending all the like 
                     * attributes, except for the one being removed, to the 
                     * update call.  However, to guard against multiple '-remove'
                     * requests on the same command line interferring with one 
                     * another, for now we will just mark the attribute for 
                     * deletion and then process the multiple deletes all at 
                     * once after we leave the main loop.
                     */
                    multiple_found = TRUE;
                    for (j = 0; j < exist_count; j++) {
                        if (check_table[j].same_found == TRUE) {
                            check_table[j].used = TRUE;
                        }
                    }
                }
                break;

            case modify_type_add:
                if (special_case == TRUE) {
                    /*
                     * We have an attribute type that, by its definition, can
                     * have multiple values stored in a single instance.  Any
                     * work the user want's done to this attribute will be done
                     * in this instance.  Currently, this applies to the encoding
                     * types "printstring_array" and "attr_set".  An add here
                     * means the user is trying to stick whatever new values
                     * specified onto the end of the current list of values.
                     * When the existing instance is found, build the new
                     * instance to include those currently in the database,
                     * plus the new ones.
                     */

                    /*
                     * Assign pointers to the value portions of the attribute 
                     * structures.
                     */
                    in_ptr = &in_attrs[i].attr_value;
                    exist_ptr = &exist_attrs[special_index].attr_value;
                    out_ptr = &out_attrs[new_count].attr_value;

                    switch (in_ptr->attr_encoding) {
                        case sec_attr_enc_attr_set:
                            /*
                             * Tack the new attr_set info onto the existing info.
                             */
                            out_attrs[new_count] = in_attrs[i];
                            existing_members = exist_ptr->tagged_union.attr_set->num_members;
                            new_members = in_ptr->tagged_union.attr_set->num_members;
                            size1 = (existing_members + new_members) * sizeof(uuid_t) +
                                sizeof(in_ptr->tagged_union.attr_set->num_members);
                            out_ptr->tagged_union.attr_set =
                                (sec_attr_enc_attr_set_t *) malloc(size1);
                            if (out_ptr->tagged_union.attr_set == NULL) {
                                status = dcp_s_no_memory;
                                break;
                            }
                            memset(out_ptr->tagged_union.attr_set, 0, size1); 

                            index = 0;
                            for (j = 0; j < existing_members; j++) {
                                out_ptr->tagged_union.attr_set->members[index++] =
                                    exist_ptr->tagged_union.attr_set->members[j];
                            }
                            for (j = 0; j < new_members; j++) {
                                out_ptr->tagged_union.attr_set->members[index++] =
                                    in_ptr->tagged_union.attr_set->members[j];
                            }
                            out_ptr->tagged_union.attr_set->num_members = index; 
                            new_count++;
                            check_table[special_index].used = TRUE;
                            break;

                        case sec_attr_enc_printstring_array:
                            /*
                             * Tack the new printstring_array info onto the existing 
                             * info.
                             */
                            out_attrs[new_count] = in_attrs[i];
                            existing_members = exist_ptr->tagged_union.string_array->num_strings;
                            new_members = in_ptr->tagged_union.string_array->num_strings;
                            size1 = (existing_members + new_members) * 
                                    sizeof(sec_attr_enc_printstring_p_t) +
                                    sizeof(out_ptr->tagged_union.string_array->num_strings);
                            out_ptr->tagged_union.string_array = 
                                (sec_attr_enc_str_array_t *) malloc(size1);
                            if (out_ptr->tagged_union.string_array == NULL) {
                                status = dcp_s_no_memory;
                                break;
                            }
                            memset(out_ptr->tagged_union.string_array, 0, size1);

                            index = 0;
                            for (j = 0; j < existing_members; j++) {
                                out_ptr->tagged_union.string_array->strings[index++] =
                                    (unsigned_char_t *) dce_strdup((char *)
                                    exist_ptr->tagged_union.string_array->strings[j]);
                            }
                            for (j = 0; j < new_members; j++) {
                                out_ptr->tagged_union.string_array->strings[index++] =
                                    (unsigned_char_t *) dce_strdup((char *)
                                    in_ptr->tagged_union.string_array->strings[j]);
                            }
                            out_ptr->tagged_union.string_array->num_strings = index;
                            new_count++;
                            check_table[special_index].used = TRUE;
                            break;

                        default:
                            /*
                             * If we get here, that means we have an encoding type that
                             * is marked as a special case, or simply an unknown 
                             * encoding type. Either way, this is some kind of 
                             * internal error.
                             */
                            status = dcp_s_rgy_era_internal_error;
                            break;
                    }
                }
                else if (((same_found == TRUE) || (other_found == TRUE)) &&
                         !(SA_SCH_FLAG_IS_SET_MULTI_INST(&era_info[i]))) {
                    /*
                     * Trying to add an attribute that already exists, either as 
                     * a same or as an other, but the attribute type is not marked
                     * to handle multiple values.  This is an error.
                     */
                    status = dcp_s_rgy_era_not_multi_valued;
                }
                else if ((same_found == TRUE) || (other_found == TRUE)) {
                    /*
                     * Trying to add an instance of an attribute that already 
                     * exists, and the schema flags allow repeats.  Since there 
                     * is at least one instance of the attribute present, we must
                     * play with the attribute structure to ensure all instances 
                     * get processed.
                     */
                    for (j = 0; j < exist_count; j++) {
                        if (((check_table[j].same_found == TRUE) ||
                            (check_table[j].other_found == TRUE)) &&
                            (check_table[j].used == FALSE)) {
                            sec_attr_util_alloc_copy(NULL,
                                                     &exist_attrs[j],
                                                     &out_attrs[new_count++],
                                                     &status);
                            if (!GOOD_STATUS(&status)) {
                                new_count--;
                                break;
                            }
                            check_table[j].used = TRUE;
                        }
                    }
                    if (!GOOD_STATUS(&status)) {
                        break;
                    }
                    sec_attr_util_alloc_copy(NULL,
                                             &in_attrs[i],
                                             &out_attrs[new_count++],
                                             &status);
                    if (!GOOD_STATUS(&status)) {
                        new_count--;
                        break;
                    }
                }
                else {
                    /*
                     * Trying to add a new instance of an attribute, of which no 
                     * other type_id's currently exist.  Here we can just add the
                     * new attribute.
                     */
                    sec_attr_util_alloc_copy(NULL,
                                             &in_attrs[i], 
                                             &out_attrs[new_count++], 
                                             &status);
                    if (!GOOD_STATUS(&status)) {
                        new_count--;
                        break;
                    }
                }
                break;

            case modify_type_change:
                if (special_case == TRUE) {
                    /*
                     * We have an attribute type that, by its definition, can have
                     * multiple values stored in a single instance.  Any work the
                     * user want's done to this attribute will be done in this
                     * instance.  Currently, this applies to the encoding types
                     * "printstring_array" and "attr_set".  A change here
                     * means the user is trying to simply replace whatever is 
                     * currently stored in that instance with the new instance 
                     * specified.  All we have to do is pass the new one along 
                     * and let update handle things.
                     */
                    check_table[special_index].used = TRUE;
                    sec_attr_util_alloc_copy(NULL,
                                             &in_attrs[i],
                                             &out_attrs[new_count++],
                                             &status);
                    if (!GOOD_STATUS(&status)) {
                        new_count--;
                        break;
                    }
                }
                else if ((same_found == FALSE) && (other_found == FALSE)) {
                    /*
                     * Trying to change an attribute that does not exist.  
                     * This is an error.
                     */
                    status = dcp_s_rgy_era_no_attribute_exists;
                }
                else {
                    /*
                     * Trying to change an attribute that does exist.  We know 
                     * that there is at least one entry that is 'same' or 'other'.
                     * In either case, we can just process the change.
                     */
                    for (j = 0; j < exist_count; j++) {
                        if ((check_table[j].same_found == TRUE) || (check_table[j].other_found == TRUE)) {
                            check_table[j].used = TRUE;
                        }
                    }
                    sec_attr_util_alloc_copy(NULL,
                                             &in_attrs[i],
                                             &out_attrs[new_count++],
                                             &status);
                    if (!GOOD_STATUS(&status)) {
                        new_count--;
                        break;
                    }
                }
                break;

            default:
                /*
                 * If we get here, that means we are in the modify routine,
                 * but the modify type is somehow set incorrectly.  This is
                 * an internal error.
                 */
                status = dcp_s_rgy_era_internal_error;
                break;
        }

        /*
         * If we had an error during the processing of this attribute, don't
         * bother processing the rest in the loop.  Break out and finish
         * cleaning before we leave.
         */
        if (!GOOD_STATUS(&status)) {
            break;
        }
    }

    /*
     * Now handle those pesky attributes the user wants to 'remove', but
     * still have other instances that are not being removed.  If we
     * have multiple attributes of the same type_id, and the user is
     * trying to remove some of them, add the rest to the output
     * attribute structure.  If it turns out the user is trying to
     * remove them all, put them in the delete structure.
     */
    if (GOOD_STATUS(&status) && (modify_type == modify_type_remove) &&
       ((multiple_found == TRUE) || (special_case == TRUE))) {
        pass = 0;
        for (j = 0; j < exist_count; j++) {
            if ((check_table[j].used == FALSE) && 
                (check_table[j].special_case == FALSE)) {
                pass++;
            }
        }
        for (j = 0; j < exist_count; j++) {
            if ((check_table[j].used == FALSE) && 
                (check_table[j].special_case == FALSE)) {
                if (pass != 0) {
                    sec_attr_util_alloc_copy(NULL,
                                             &exist_attrs[j],
                                             &out_attrs[new_count++],
                                             &status);
                }
                else {
                    sec_attr_util_alloc_copy(NULL,
                                             &exist_attrs[j],
                                             &del_attrs[del_count++],
                                             &status);
                }
                if (!GOOD_STATUS(&status)) {
                    if (pass != 0)
                        new_count--;
                    else
                        del_count--;
                    break;
                }
                check_table[j].used = TRUE;
            }
        }
    }

    /*
     * If the input attribute we are processing is an encoding type
     * any, we need to look through the existing attributes and make
     * sure that any other attribute of the same id but of a different
     * encoding type is processed as well.
     */
    if (GOOD_STATUS(&status) && ((modify_type == modify_type_add) ||
        (modify_type == modify_type_change))) {
        for (i = 0; i < in_count; i++) {
            if (check_table[i].any_case == TRUE) {
                for (j = 0; j < exist_count; j++) {
                    if ((uuid_equal(&in_attrs[i].attr_id, &exist_attrs[j].attr_id, &tmp_status)) &&
                        (SA_ATTR_ENCODING(&in_attrs[i]) != SA_ATTR_ENCODING(&exist_attrs[j])) &&
                        (check_table[j].used == FALSE)) {
                        sec_attr_util_alloc_copy(NULL,
                                                 &exist_attrs[j],
                                                 &out_attrs[new_count++],
                                                 &status);
                        if (!GOOD_STATUS(&status)) {
                            new_count--;
                            break;
                        }
                        check_table[j].used = TRUE;
                    }
                }
            }
        }
    }

    /*
     * If we failed up to now, cleanup and return
     */
    if (!GOOD_STATUS(&status)) {
        for (i = 0; i < new_count; i++) {
            sec_attr_util_inst_free_ptrs(&out_attrs[i]);		
        }
        for (i = 0; i < del_count; i++) {
            sec_attr_util_inst_free_ptrs(&del_attrs[i]);		
        }
        *out_count = 0;
        *delete_count = 0;
        free((era_check_table_t *)check_table);
        return status;
    }

    /*
     * Update the era count and return.
     */
    *out_count = new_count;
    *delete_count = del_count;
    free((era_check_table_t *)check_table);
    return status;
}


/*
 * FUNCTION: match_bindings
 *
 * OVERVIEW: Compares two "fully-structured" bindings (data of type
 *      sec_attr_bind_info_t) to see if they are equal, which depends on 
 *      all their components matching in type and value. Used by the
 *      check era function to find out if the attribute passed in for
 *      a modify operation matches one that is already on the object.
 *
 * INPUTS:
 *  in_binding - The binding provided as input
 *  comp_binding - The existing binding to which it is to be compared
 *
 * OUTPUTS: None
 *
 * RETURNS:
 *  boolean32 - TRUE if they match, FALSE otherwise.
 */
boolean32 match_bindings(sec_attr_bind_info_t *in_binding,
	                 sec_attr_bind_info_t *comp_binding)
{
    int                 i;
    sec_attr_binding_t  *in_member, *comp_member;

    if (in_binding->num_bindings != comp_binding->num_bindings) {
	return FALSE;
    }
    if (in_binding->auth_info.info_type !=
	comp_binding->auth_info.info_type) {
	return FALSE;
    }
    if (in_binding->auth_info.info_type == sec_attr_bind_auth_dce) {
	if ((in_binding->auth_info.tagged_union.dce_info.protect_level !=
	     comp_binding->auth_info.tagged_union.dce_info.protect_level) ||
	    (in_binding->auth_info.tagged_union.dce_info.authn_svc !=
	     comp_binding->auth_info.tagged_union.dce_info.authn_svc) ||
	    (in_binding->auth_info.tagged_union.dce_info.authz_svc !=
	     comp_binding->auth_info.tagged_union.dce_info.authz_svc) ||
	    (strcmp((char *)in_binding->auth_info.tagged_union.dce_info.svr_princ_name,
		    (char *)comp_binding->auth_info.tagged_union.dce_info.svr_princ_name) != 0)) {
	    return FALSE;
	}
    }
    for (i = 0; i < in_binding->num_bindings; i++) {
	in_member = &(in_binding->bindings[i]);
	comp_member = &(comp_binding->bindings[i]);
	if (in_member->bind_type != comp_member->bind_type) {
	    return FALSE;
	}
	if (in_member->bind_type == sec_attr_bind_type_string) {
	    if (strcmp((char *)in_member->tagged_union.string_binding,
		       (char *)comp_member->tagged_union.string_binding) != 0) {
		return FALSE;
	    }
	}
	else if (in_member->bind_type == sec_attr_bind_type_svrname) {
	    if ((in_member->tagged_union.svrname->name_syntax !=
		comp_member->tagged_union.svrname->name_syntax) ||
		(strcmp((char *)in_member->tagged_union.svrname->name,
			(char *)comp_member->tagged_union.svrname->name) != 0)) {
		return FALSE;
	    }
	}
	else {
	    /*
             * Unrecognized/unsupported type; should
             * probably be treated as an error.
             */
	    return FALSE;
	}
    }

    /*
     * Didn't find anything that didn't match.
     */
    return TRUE;
}


/*
 * FUNCTION: get_attr_values
 *
 * OVERVIEW: This function will take as input the encoding type
 *      and id of an extended attribute and try to store the value
 *      information in the proper structure, suitable for passing
 *      in to the update function.
 *
 * INPUTS:
 *  Tcl_Interp *interp - The tcl interpreter structure for this invocation
 *  sec_attr_encoding_t encode_type - The encoding type of the extended
 *                                    attribute to process.
 *  uuid_t attr_id - The attribute id of the extended attribute to
 *                   process.
 *  int *num_val - The number of entries in the structure which holds
 *                 the attribute / value information.
 *  char **attr_string - The attribute name and value information.
 *
 * OUTPUTS:
 *  sec_attr_t *attrs - The structure which holds the attribute value
 *                      information.
 *
 * RETURNS:
 *  int - The status of this call, can range from sec_rgy_status_ok to
 *        some hideous error code.
 */

#define CAN_BE_NAME_BINDING(x) \
    *(x) == '/'

#define CAN_BE_STRING_BINDING(x) \
    strchr((x), ':') != NULL

#define CAN_BE_TWR_BINDING(x) \
    isxdigit(*(x))

int get_attr_values(Tcl_Interp          *interp,
                    sec_attr_encoding_t encode_type,
                    uuid_t              attr_id,
                    sec_attr_t          *attrs,
                    int                 *num_val,
                    char                **attr_string)
{
   int                        i;
   int                        string_len;
   sec_attr_value_t           *attr;
   unsigned32                 result;
   int                        era_int;
   int                        list_length, num_bindings, num_elements;
   boolean32                  one_tcl_binding = FALSE;
   char                       **full_binding;
   char                       **auth_info_values;
   char                       **binding_str;
   char                       **binding_elements;
   char                       *temp_string;
   int                        iargc;
   char                       **iargv;
   sec_attr_bind_auth_info_t  *auth_info_p;
   sec_attr_binding_t         *binding_p;
   int                        twr_binding = 0;
   unsigned32                 codeset = 0;
   unsigned_char_t            *codeset_name;
   error_status_t             status;

   /*
    * Define a short-hand pointer to make the rest of
    * the code easier to read.
    */
   attr = &attrs->attr_value;

   /*
    * If the value for the attribute is NULL, then just store
    * the attribute id and the encoding type and return.
    */
   if (attr_string[1] == NULL) {
      attrs->attr_id = attr_id;
      attr->attr_encoding = encode_type;
      return sec_rgy_status_ok;
   }

   /*
    * Store the value information based on the encoding
    * type of the extended attribute being processed.
    * Be careful to store the value correctly.
    */
   switch (encode_type) {
      case sec_attr_enc_void:
         attrs->attr_id = attr_id;
         attr->attr_encoding = encode_type;
         if (strcmp(attr_string[1], "present") != 0)
            return dcp_s_rgy_era_not_present;
         break;

      case sec_attr_enc_integer:
         attrs->attr_id = attr_id;
         attr->attr_encoding = encode_type;
         era_int = is_number(attr_string[1]);
         if (era_int == dcp_rgy_not_int)
            return dcp_s_rgy_era_not_int;
         else if (era_int == dcp_rgy_int_out_of_range)
            return dcp_s_rgy_era_out_of_range;
         attr->tagged_union.signed_int = (signed32)atoi(attr_string[1]);
         break;

      case sec_attr_enc_printstring:
         attrs->attr_id = attr_id;
         attr->attr_encoding = encode_type;
         attr->tagged_union.printstring = (unsigned_char_p_t) malloc(
            sizeof(unsigned_char_t) * (strlen(attr_string[1]) + 1));
         if (attr->tagged_union.printstring == NULL)
            return dcp_s_no_memory;
         strcpy((char *)attr->tagged_union.printstring, attr_string[1]);
         break;

      case sec_attr_enc_bytes:
      case sec_attr_enc_confidential_bytes:
         attrs->attr_id = attr_id;
         attr->attr_encoding = encode_type;
         if (attr_string[1] != NULL) {
            attr->tagged_union.bytes = (sec_attr_enc_bytes_t *) malloc(
               sizeof(sec_attr_enc_bytes_t) +
               ((strlen(attr_string[1]) + 1) * sizeof(idl_byte)));
            if (attr->tagged_union.bytes == NULL)
               return dcp_s_no_memory;
            get_hex_bytes((char *)attr_string[1],
                          (char *)attr->tagged_union.bytes->data,
                          (unsigned32 *)&attr->tagged_union.bytes->length,
                          (unsigned32 *)&result);
            if (result != error_status_ok) {
               free(attr->tagged_union.bytes);
               return (result);
            }
         }
         else {
            attr->tagged_union.bytes = NULL;
         }
         break;

      case sec_attr_enc_i18n_data:
         attrs->attr_id = attr_id;
         attr->attr_encoding = encode_type;
         if (Tcl_SplitList(interp, attr_string[1], &iargc, &iargv) != TCL_OK) {
            return TCL_ERROR;
         }
         if (iargc != 2) {
            return dcp_s_rgy_era_invalid_i18ndata_syntax;
         }
         era_int = is_number(iargv[0]);
         if (era_int != dcp_rgy_not_int) {
            codeset = (unsigned32)atoi(iargv[0]);
            dce_cs_rgy_to_loc(codeset,
                              &codeset_name,
                              NULL,
                              NULL,
                              &status);
            if (status != dce_cs_c_ok) {
               return dcp_s_rgy_era_codeset_not_valid;
            }
         }
         else {
            dce_cs_loc_to_rgy((unsigned_char_p_t)iargv[0],
                              &codeset,
                              NULL,
                              NULL,
                              &status);
            if (status != dce_cs_c_ok) {
               return dcp_s_rgy_era_codeset_not_valid;
            }
         }
         attr->tagged_union.idata = (sec_attr_i18n_data_t *)malloc(
            sizeof(sec_attr_i18n_data_t) +
            ((strlen(iargv[1]) + 1) * sizeof(idl_byte)));
         if (attr->tagged_union.idata == NULL)
            return dcp_s_no_memory;
         attr->tagged_union.idata->codeset = codeset;
         attr->tagged_union.idata->length = strlen(iargv[1]);
         strcpy((char *)attr->tagged_union.idata->data, iargv[1]);
         break;

      case sec_attr_enc_uuid:
         attrs->attr_id = attr_id;
         attr->attr_encoding = encode_type;
         uuid_from_string((unsigned char *)attr_string[1],
                          &attr->tagged_union.uuid,
                          &result);
         if (result != sec_rgy_status_ok)
            return dcp_s_rgy_era_not_uuid;
         break;

      case sec_attr_enc_printstring_array:
         attrs->attr_id = attr_id;
         attr->attr_encoding = encode_type;
         attr->tagged_union.string_array = (sec_attr_enc_str_array_t *) malloc(
            sizeof(attr->tagged_union.string_array) +
            ((*num_val - 1) * sizeof(attr->tagged_union.string_array->strings)));
         if (attr->tagged_union.string_array == NULL)
            return dcp_s_no_memory;
         attr->tagged_union.string_array->num_strings = *num_val - 1;
         for (i = 1; i < *num_val; i++) {
            attr->tagged_union.string_array->strings[i-1] =
               (sec_attr_enc_printstring_p_t) malloc(sizeof(char) * (strlen(attr_string[i]) + 1));
            if (attr->tagged_union.string_array->strings[i-1] == NULL) {
               free(attr->tagged_union.string_array);
               return dcp_s_no_memory;
            }
            strcpy((char *)attr->tagged_union.string_array->strings[i-1], attr_string[i]);
         }
         break;

      case sec_attr_enc_attr_set:
         attrs->attr_id = attr_id;
         attr->attr_encoding = encode_type;
         attr->tagged_union.attr_set =
            (sec_attr_enc_attr_set_t *) malloc(((*num_val - 1) * sizeof(uuid_t)) +
            sizeof(sec_attr_enc_attr_set_t));
         if (attr->tagged_union.attr_set == NULL)
            return dcp_s_no_memory;
         attr->tagged_union.attr_set->num_members = *num_val - 1;
         for (i = 1; i < *num_val; i++) {
            uuid_from_string((unsigned char *)attr_string[i],
                             &attr->tagged_union.attr_set->members[i-1], &result);
            if (result != sec_rgy_status_ok) {
               free(attr->tagged_union.attr_set);
               return dcp_s_rgy_era_not_uuid;
            }
         }
         break;

      case sec_attr_enc_binding:
         attrs->attr_id = attr_id;
         attr->attr_encoding = encode_type;
         if (attr_string[1] != NULL) {
            if (Tcl_SplitList(interp, attr_string[1], &list_length, &full_binding)
                != TCL_OK || list_length != 2) {
                return dcp_s_rgy_era_invalid_binding_syntax;
            }
            if (CAN_BE_TWR_BINDING(full_binding[1])) {
                twr_binding = 1;
                num_bindings = 1;
                binding_str = &full_binding[1];
            }
            else {
                if (Tcl_SplitList(interp, full_binding[1], &num_bindings, &binding_str) != TCL_OK) {
                    free((char *)full_binding);
                    return dcp_s_rgy_era_invalid_binding_syntax;
                }
                if (CAN_BE_TWR_BINDING(binding_str[0])) {
                    twr_binding = 1;
                }
            }
            one_tcl_binding = FALSE;
            if (num_bindings == 3 || num_bindings == 4) {
                /*
                 * Check to see if it's a single tcl binding. (We need to
                 * know this now so we can allocate the right number of bindings.
                 * If the first element doesn't match a possible syntax
                 * for a server name or a string binding, see if it can be
                 * interpreted as a tcl binding. If it can't, don't worry about
                 * it now; it will be dealt with when we process all the
                 * bindings later.
                 */
                if (!(CAN_BE_NAME_BINDING(binding_str[0])) &&
                    !(CAN_BE_STRING_BINDING(binding_str[0])) &&
                    !(CAN_BE_TWR_BINDING(binding_str[0]))) {
                    temp_string = dcp_binding_tcl_to_string(num_bindings,
                                                            binding_str,
                                                            &result);
                    if (result == rpc_s_ok) {
                        check_string_binding(temp_string, &result);
                        if (result == rpc_s_ok) {
                            num_bindings = 1;
                            one_tcl_binding = TRUE;
                        }
                    }
                    else {
                        rpc_string_free((unsigned char **)&temp_string, &result);
                    }
                }
            }

            attr->tagged_union.binding = (sec_attr_bind_info_t *) malloc(
                sizeof(sec_attr_bind_info_t) +
                (num_bindings * sizeof(sec_attr_binding_t)));
            if (attr->tagged_union.binding == NULL)
               return dcp_s_no_memory;
            attr->tagged_union.binding->num_bindings = num_bindings;

            auth_info_p = &(attr->tagged_union.binding->auth_info);
            if (Tcl_SplitList(interp, full_binding[0], &list_length, &auth_info_values) != TCL_OK) {
                free((char *)full_binding);
                return dcp_s_rgy_era_invalid_binding_syntax;
            }
            if (list_length == 1) {
                if (strcmp(auth_info_values[0], "none") != 0) {
                    free((char *)auth_info_values);
                    free((char *)binding_str);
                    free((char *)attr->tagged_union.binding);
                    free((char *)full_binding);
                    return dcp_s_rgy_era_invalid_binding_syntax;
                }
                auth_info_p->info_type = sec_attr_bind_auth_none;
            } else {
                if ((list_length != 5) || (strcmp(auth_info_values[0], "dce") != 0)) {
                    free((char *)auth_info_values);
                    free((char *)binding_str);
                    free((char *)attr->tagged_union.binding);
                    free((char *)full_binding);
                    return dcp_s_rgy_era_invalid_binding_syntax;
                }
                auth_info_p->info_type = sec_attr_bind_auth_dce;
                auth_info_p->tagged_union.dce_info.svr_princ_name =
                    (unsigned_char_p_t) malloc(sizeof(unsigned_char_t) * (strlen(auth_info_values[1]) + 1));
                if (auth_info_p->tagged_union.dce_info.svr_princ_name == NULL)
                    return dcp_s_no_memory;
                strcpy((char *)auth_info_p->tagged_union.dce_info.svr_princ_name, auth_info_values[1]);

                /*
                 * Get protection value
                 */
                for (i = 0; i < MAX_PROT_LEV; i++) {
                    if (strcmp(auth_info_values[2], prot_lev_name[i]) == 0) {
                        auth_info_p->tagged_union.dce_info.protect_level = prot_lev_val[i];
                        break;
                    }
                }
                if (i >= MAX_PROT_LEV) {
                    free((char *)auth_info_p->tagged_union.dce_info.svr_princ_name);
                    free((char *)auth_info_values);
                    free((char *)binding_str);
                    free((char *)attr->tagged_union.binding);
                    free((char *)full_binding);
                    return dcp_s_rgy_era_invalid_protection;
                }

                /*
                 * Get authentication value
                 */
                for (i = 0; i < MAX_AUTHN; i++) {
                    if (strcmp(auth_info_values[3], authn_name[i]) == 0) {
                        auth_info_p->tagged_union.dce_info.authn_svc = authn_val[i];
                        break;
                    }
                }
                if (i >= MAX_AUTHN) {
                    free((char *)auth_info_p->tagged_union.dce_info.svr_princ_name);
                    free((char *)auth_info_values);
                    free((char *)binding_str);
                    free((char *)attr->tagged_union.binding);
                    free((char *)full_binding);
                    return dcp_s_rgy_era_invalid_authentication;
                }

                /*
                 * Get authorization value
                 */
                for (i = 0; i < MAX_AUTHZ; i++) {
                    if (strcmp(auth_info_values[4], authz_name[i]) == 0) {
                        auth_info_p->tagged_union.dce_info.authz_svc = authz_val[i];
                        break;
                    }
                }
                if (i >= MAX_AUTHZ) {
                    free((char *)auth_info_p->tagged_union.dce_info.svr_princ_name);
                    free((char *)auth_info_values);
                    free((char *)binding_str);
                    free((char *)attr->tagged_union.binding);
                    free((char *)full_binding);
                    return dcp_s_rgy_era_invalid_authorization;
                }
                free((char *)auth_info_values);
            }

            binding_p = &(attr->tagged_union.binding->bindings[0]);
            if (one_tcl_binding) {
                /*
                 * It's already been checked for validity
                 */
                binding_p->bind_type = sec_attr_bind_type_string;
                binding_p->tagged_union.string_binding = (unsigned_char_p_t) malloc(
                    sizeof(unsigned_char_t) * (strlen(temp_string) + 1));
                if (binding_p->tagged_union.string_binding == NULL)
                    return dcp_s_no_memory;
                strcpy((char *)binding_p->tagged_union.string_binding, temp_string);
                rpc_string_free((unsigned char **)&temp_string, &result);
            } else {
                for (i = 0; i < num_bindings; i++) {
                    binding_p = &(attr->tagged_union.binding->bindings[i]);
                    if (twr_binding) {
                        binding_p->bind_type = sec_attr_bind_type_twrs;
                        binding_p->tagged_union.twr_set =
                            (sec_attr_twr_set_t *) malloc(sizeof(sec_attr_twr_set_t));
                        if (binding_p->tagged_union.twr_set == NULL)
                            return dcp_s_no_memory;
                        binding_p->tagged_union.twr_set->count = 1;
                        string_len = strlen(binding_str[i]);
                        binding_p->tagged_union.twr_set->towers[0] = (twr_t *) malloc(
                            sizeof(binding_p->tagged_union.twr_set->towers[0]->tower_length) +
                            string_len + 1);
                        if (binding_p->tagged_union.twr_set->towers[0] == NULL)
                            return dcp_s_no_memory;
                        get_hex_bytes((char*)binding_str[i],
                                      (char*)binding_p->tagged_union.twr_set->towers[0]->tower_octet_string,
                                      &binding_p->tagged_union.twr_set->towers[0]->tower_length,
                                      &result);
                        if (result != error_status_ok) {
                            free((char *)binding_p->tagged_union.twr_set->towers[0]);
                            free((char *)binding_p->tagged_union.twr_set);
                            if (auth_info_p->info_type == sec_attr_bind_auth_dce)
                                free((char *)auth_info_p->tagged_union.dce_info.svr_princ_name);
                            if (num_bindings > 1)
                                free((char *)binding_str);
                            free((char *)attr->tagged_union.binding);
                            free((char *)full_binding);
                            return result;
                        }
                    } else {
                        if (Tcl_SplitList(interp, binding_str[i], &num_elements,
                                          &binding_elements) != TCL_OK) {
                            if (auth_info_p->info_type == sec_attr_bind_auth_dce)
                                free((char *)auth_info_p->tagged_union.dce_info.svr_princ_name);
                            free((char *)binding_str);
                            free((char *)attr->tagged_union.binding);
                            free((char *)full_binding);
                            return dcp_s_rgy_era_invalid_binding_syntax;
                        }
                        if (num_elements == 1) {
                            /*
                             * Single string, don't need result of Tcl_SplitList
                             */
                            free((char *)binding_elements);
                            if (CAN_BE_NAME_BINDING(binding_str[i])) {
                                binding_p->bind_type = sec_attr_bind_type_svrname;
                                binding_p->tagged_union.svrname =
                                    (sec_attr_bind_svrname *) malloc(sizeof(sec_attr_bind_svrname));
                                if (binding_p->tagged_union.svrname == NULL)
                                    return dcp_s_no_memory;
                                binding_p->tagged_union.svrname->name_syntax = rpc_c_ns_syntax_dce;
                                binding_p->tagged_union.svrname->name =
                                    (unsigned_char_p_t) malloc(sizeof(char) *
                                    (strlen(binding_str[i]) + 1));
                                if (binding_p->tagged_union.svrname->name == NULL)
                                    return dcp_s_no_memory;
                                strcpy((char *)binding_p->tagged_union.svrname->name, binding_str[i]);
                            } else if (CAN_BE_STRING_BINDING(binding_str[i])) {
                                check_string_binding(binding_str[i], &result);
                                if (result == rpc_s_ok) {
                                    binding_p->bind_type = sec_attr_bind_type_string;
                                    binding_p->tagged_union.string_binding =
                                       (unsigned_char_p_t) malloc(sizeof(unsigned_char_t) *
                                       (strlen(binding_str[i]) + 1));
                                    if (binding_p->tagged_union.string_binding == NULL)
                                        return dcp_s_no_memory;
                                    strcpy((char *)binding_p->tagged_union.string_binding, binding_str[i]);
                                }
                                else {
                                    if (auth_info_p->info_type == sec_attr_bind_auth_dce)
                                        free((char *)auth_info_p->tagged_union.dce_info.svr_princ_name);
                                    free((char *)binding_str);
                                    free((char *)attr->tagged_union.binding);
                                    free((char *)full_binding);
                                    return result;
                                }
                            } else {
                                if (auth_info_p->info_type == sec_attr_bind_auth_dce)
                                    free((char *)auth_info_p->tagged_union.dce_info.svr_princ_name);
                                free((char *)binding_str);
                                free((char *)attr->tagged_union.binding);
                                free((char *)full_binding);
                                return dcp_s_rgy_era_invalid_binding_syntax;
                            }
                        }
                        else {
                            temp_string = dcp_binding_tcl_to_string(num_elements,
                                                                    binding_elements,
                                                                    &result);
                            if (result == rpc_s_ok) {
                                check_string_binding(temp_string, &result);
                                if (result == rpc_s_ok) {
                                    binding_p->bind_type = sec_attr_bind_type_string;
                                    binding_p->tagged_union.string_binding =
                                        (unsigned_char_p_t) malloc(sizeof(unsigned_char_t) *
                                        (strlen(temp_string) + 1));
                                    if (binding_p->tagged_union.string_binding == NULL)
                                        return dcp_s_no_memory;
                                    strcpy((char *)binding_p->tagged_union.string_binding, temp_string);
                                }
                                rpc_string_free((unsigned char **)&temp_string, &result);
                            }
                            if (result != rpc_s_ok) {
                                free((char *)binding_elements);
                                if (auth_info_p->info_type == sec_attr_bind_auth_dce)
                                    free((char *)auth_info_p->tagged_union.dce_info.svr_princ_name);
                                free((char *)binding_str);
                                free((char *)attr->tagged_union.binding);
                                free((char *)full_binding);
                                return result;
                            }
                        }
                    }
                }
            }
            if (!twr_binding || (twr_binding && num_bindings > 1))
                free((char *)binding_str);
            free((char *)full_binding);
         }
         else {
            attr->tagged_union.binding = NULL;
         }
         break;

      default:
         return dcp_s_rgy_era_unknown_encoding_type;
   }
   return sec_rgy_status_ok;
}


void get_hex_bytes(char *from,
                   char *to,
                   unsigned32 *length,
                   unsigned32 *result)
{
   int   i;
   int   j;
   int   high_bits;
   char  *hex = "0123456789abcdef";

   *result = error_status_ok;
   for (i = 0, j = 0, high_bits = 1; i < strlen(from); i++) {
      if (isspace(from[i]))
         continue;
      if (!isxdigit(from[i])) {
         *result = dcp_s_rgy_era_invalid_byte;
         return;
      }
      if (high_bits) {
         to[j] = strchr(hex, tolower(from[i])) - hex;
         high_bits = 0;
      } else {
         to[j] = to[j]*16 + strchr(hex, tolower(from[i])) - hex;
         j++;
         high_bits = 1;
      }
   }
   *length = j;
   return;
}


/*
 * FUNCTION: check_string_binding
 *
 * OVERVIEW: Check a string binding for validity by seeing if it can be
 *      converted to a binding handle.
 *
 * INPUTS:
 *  char *string_binding - The string binding being tested.
 *
 * OUTPUTS:
 *  unsigned32 *result - Status code (returned by
 *                       rpc_binding_from_string_binding)
 *
 * RETURNS:
 *  void
 */
void
check_string_binding(char *string_binding,
                     unsigned32 *result)
{
    rpc_binding_handle_t  temp_handle;
    unsigned32            temp_result;

    rpc_binding_from_string_binding((unsigned_char_t *)string_binding,
                                    &temp_handle,
                                    result);
    if (result == rpc_s_ok) {
        rpc_binding_free(&temp_handle, &temp_result);
    }
    return;
}


/*
 * FUNCTION: initialize_account_attr
 *
 * OVERVIEW: The procedure inializes the acccount attribute parameters
 *           with the default values for an account create. 
 * 
 *
 * INPUTS: rgy_acct_info_t account. A structure  holding the all necessary 
 *         structures for account creation. 
 *  None
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  void
 */
void initialize_account_attr(rgy_acct_info_t *account)
{
    account->acct_key = sec_rgy_acct_key_person;
    account->acct_user.flags = sec_rgy_acct_user_passwd_valid;
    account->acct_user.gecos[0] = '\0';
    strcpy((char *)account->acct_user.homedir,"/\0");;
    account->acct_user.shell[0] = '\0';
    account->acct_user.passwd[0] = '\0';
    account->acct_admin.good_since_date = get_current_time();
    account->acct_admin.expiration_date = 0;
    account->acct_admin.flags |= sec_rgy_acct_admin_valid;
    account->acct_admin.flags |= sec_rgy_acct_admin_server;
    account->acct_admin.flags |= sec_rgy_acct_admin_client;
    account->acct_admin.authentication_flags |= sec_rgy_acct_auth_forwardable;
    account->acct_admin.authentication_flags |= sec_rgy_acct_auth_tgt;
    account->acct_admin.authentication_flags |= sec_rgy_acct_auth_renewable;
    account->account_name.pname[0] = '\0';
    account->account_name.gname[0] = '\0';
    account->account_name.oname[0] = '\0';
    account->passwd.version_number = 0;
    account->adm_passwd.version_number = 0;
    account->passwd.key.key_type = sec_passwd_plain;
    account->adm_passwd.key.key_type = sec_passwd_plain;
    account->pkgenprivkey_value = 0;
}


/*
 * FUNCTION: initialize_pk_attr
 *
 * OVERVIEW:
 *
 * INPUTS: 
 *
 * INPUT/OUTPUTS:
 *
 * OUTPUTS:
 *
 * RETURNS:
 */
void initialize_pk_attr(rgy_pubkey_info_t *pkinfo)
{
    pkinfo->generatekey = 0;
    pkinfo->publickeyfile = '\0';
    pkinfo->privatekeyfile = '\0';
    pkinfo->oldpassphrase = '\0';
    pkinfo->newpassphrase = '\0';
}


/*
 * FUNCTION: parse_account_switches
 * 
 * OVERVIEW: This takes all of the switches provided on the command line and 
 *      enters them into the correct account structure. 
 * 
 *
 * INPUTS:
 *  None
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  void
 */
int parse_account_switches(Tcl_Interp *interp, rgy_acct_info_t *account)
{
    error_status_t  result = sec_rgy_status_ok;
    char            *none_svc;
    sec_rgy_name_t  global_name;
    sec_rgy_name_t  cell_name;
    char            *end;
    int             plen;
    int             genkey_int;
    boolean32       default_privkey = FALSE;

    if (account->group) {
        strcpy((char *)global_name,account->group);
        if (dcp_id_parse_name(interp,
                              global_name,
                              cell_name,
                              account->account_name.gname) != TCL_OK)
            return TCL_ERROR;
    }
    if (account->organization) {
        strcpy((char *)global_name,account->organization);
        if (dcp_id_parse_name(interp,
                              global_name,
                              cell_name,
                              account->account_name.oname) != TCL_OK)
            return TCL_ERROR;
    }
    if (account->password) {
        if(strlen(account->password) > sec_passwd_str_max_len){
            DCP_SET_SEC_ERROR(dcp_s_rgy_acct_passwd_long,
                              sec_passwd_str_max_len);
        }
        account->passwd.key.tagged_union.plain = 
            (unsigned char *)account->password;
    }

    if(account->mypwd){
        if(strlen((char *)account->mypwd) > sec_passwd_str_max_len)
            return TCL_ERROR;
        account->adm_passwd.key.tagged_union.plain = 
            (unsigned char *)account->mypwd;
    }

    if(account->acctvalid){
        result = add_acct_admin_flag(&(account->acct_admin.flags),
                                     sec_rgy_acct_admin_valid,
                                     account->acctvalid);
        DCP_CHECK_SEC_ERROR(result);
    }
    if(account->client){
       result = add_acct_admin_flag(&(account->acct_admin.flags),
                                    sec_rgy_acct_admin_client,
                                    account->client);
       DCP_CHECK_SEC_ERROR(result);
    }

    if (account->pkgenprivkey) {
        genkey_int = -1;
        plen = strlen((char *)account->pkgenprivkey);
        if (strncmp(pk_genkey_default_string, account->pkgenprivkey, plen) == 0) {
            account->pkgenprivkey_value = pk_genkey_default_value;
            default_privkey = TRUE;
        }
        else {
            genkey_int = is_number(account->pkgenprivkey);
            if (genkey_int == dcp_rgy_not_int) {
                DCP_SET_SEC_ERROR(dcp_s_rgy_pk_genkey_not_int, rgy_attr_pkgenprivkey);
            }
            else if (genkey_int == dcp_rgy_int_out_of_range) {
                DCP_SET_SEC_ERROR(dcp_s_rgy_pk_genkey_out_of_range, MAXINT);
            }
            genkey_int = strtol(account->pkgenprivkey, &end, 10);
            if (genkey_int < 0) {
                DCP_SET_SEC_ERROR(dcp_s_rgy_pk_genkey_is_negative, rgy_attr_pkgenprivkey);
            }
            account->pkgenprivkey_value = genkey_int;
        }
        if ((account->pkgenprivkey_value != 0) || (default_privkey)) {
            account->passwd.key.key_type = sec_passwd_genprivkey;
            account->passwd.key.tagged_union.modulus_size = account->pkgenprivkey_value;
        }
    }

    if (account->pkmechanism) {
        if ((strncmp((char *)pk_attr_mechanism_file, account->pkmechanism,
                strlen((char *)account->pkmechanism)) != 0) &&
            (strncmp((char *)pk_attr_mechanism_pkss, account->pkmechanism,
                strlen((char *)account->pkmechanism)) != 0)) {
            DCP_SET_MISC_ERROR(dcp_s_rgy_pk_invalid_storage);
        }
    }

    if(account->description){
        if(strlen(account->description) > sec_rgy_pname_max_len){
            DCP_SET_SEC_ERROR(dcp_s_rgy_acct_desc_long,sec_rgy_pname_max_len);
        }
        else
            strcpy((char *)account->acct_user.gecos,account->description);
    }
    if(account->dupkey){
        result = add_acct_auth_flag(&(account->acct_admin.authentication_flags),
                                     sec_rgy_acct_auth_dup_skey,
                                     account->dupkey);
        DCP_CHECK_SEC_ERROR(result);
    }
    if(account->usertouser){
        result = add_acct_auth_flag(&(account->acct_admin.authentication_flags),
                                     sec_rgy_acct_auth_user_to_user,
                                     account->usertouser);
        DCP_CHECK_SEC_ERROR(result);
    }
    if(account->expdate){
        none_svc = (char *)dce_sprintf(dcp_t_rgy_none);
        if(!strcmp(account->expdate,none_svc)){
            account->acct_admin.expiration_date = 0;
            free(none_svc);
        }
        else {
            free(none_svc);
            result = date_to_seconds(&(account->acct_admin.expiration_date),
                                     account->expdate);
            DCP_CHECK_SEC_ERROR(result);
        }
    }
    if(account->forwardabletkt){
       result = add_acct_auth_flag(&(account->acct_admin.authentication_flags),
                                     sec_rgy_acct_auth_forwardable,
                                     account->forwardabletkt);
        DCP_CHECK_SEC_ERROR(result);
     }
    if(account->goodsince){
        result = date_to_seconds(&(account->acct_admin.good_since_date),
                          account->goodsince);
        DCP_CHECK_SEC_ERROR(result);
    }
    if(account->home){
        if(strlen(account->home) > sec_rgy_pname_max_len){
            DCP_SET_SEC_ERROR(dcp_s_rgy_acct_home_long,sec_rgy_pname_max_len);
        }
        else
            strcpy((char *)account->acct_user.homedir,account->home);
    }
    if(account->postdatedtkt){
       result = add_acct_auth_flag(&(account->acct_admin.authentication_flags),
                                     sec_rgy_acct_auth_post_dated,
                                     account->postdatedtkt);
       DCP_CHECK_SEC_ERROR(result);
     }  
    if(account->proxiabletkt){
       result = add_acct_auth_flag(&(account->acct_admin.authentication_flags),
                                     sec_rgy_acct_auth_proxiable,
                                     account->proxiabletkt);
       DCP_CHECK_SEC_ERROR(result);
     }  
    if(account->pwdvalid){
        result = add_acct_user_flag(&(account->acct_user.flags),
                                     sec_rgy_acct_user_passwd_valid,
                                     account->pwdvalid);
       DCP_CHECK_SEC_ERROR(result);
     } 
    if(account->renewabletkt){
       result = add_acct_auth_flag(&(account->acct_admin.authentication_flags),
                                     sec_rgy_acct_auth_renewable,
                                     account->renewabletkt);
       DCP_CHECK_SEC_ERROR(result);
     }  
    if(account->server){
        result = add_acct_admin_flag(&(account->acct_admin.flags),
                                     sec_rgy_acct_admin_server,
                                     account->server);
        DCP_CHECK_SEC_ERROR(result);
     }  
    if(account->shell){
        if(strlen(account->shell) > sec_rgy_pname_max_len){
           DCP_SET_SEC_ERROR(dcp_s_rgy_acct_shell_long,sec_rgy_pname_max_len);
       }
        else
            strcpy((char *)account->acct_user.shell,account->shell);
    }
    if(account->stdtgtauth){
       result = add_acct_auth_flag(&(account->acct_admin.authentication_flags),
                                     sec_rgy_acct_auth_tgt,
                                     account->stdtgtauth);
       DCP_CHECK_SEC_ERROR(result);
    }
    if(account->maxtktlife){
        result = reldate_to_seconds(&(account->policy.max_ticket_lifetime),
                  account->maxtktlife);
        DCP_CHECK_SEC_ERROR(result);
    }

    if(account->maxtktrenew){
        result = reldate_to_seconds(&(account->policy.max_renewable_lifetime),
                  account->maxtktrenew);
        DCP_CHECK_SEC_ERROR(result);
    }

    return error_status_ok;
}


unsigned32 get_current_time(void)
{
    utc_t       utc;
    timespec_t  timesp;
    timespec_t  inacc;
    long        tdf;

    if (!utc_gettime(&utc))
        if (!utc_bintime(&timesp,&inacc,&tdf,&utc))
            return (unsigned32)timesp.tv_sec;

    return 0;
}

int add_acct_admin_flag(sec_rgy_acct_admin_flags_t *holder,
                        sec_rgy_acct_admin_flags_t flag,
                        char *value)
{
    int  yes_no = -1;

    yes_no = is_yes(value);
    if (yes_no == TRUE) 
        *holder |= flag;
    else if (yes_no == FALSE)
        *holder &= ~flag;
    else
        return(yes_no);

    return sec_rgy_status_ok;
}

int add_acct_auth_flag(sec_rgy_acct_auth_flags_t *holder,
                       sec_rgy_acct_auth_flags_t flag,
                       char *value)
{
    int  yes_no = -1;

    yes_no = is_yes(value);
    if (yes_no == TRUE) 
        *holder |= flag;
    else if (yes_no == FALSE)
        *holder &= ~flag;
    else
        return(yes_no);

    return sec_rgy_status_ok;
}

int add_acct_user_flag(sec_rgy_acct_user_flags_t *holder,
                       sec_rgy_acct_user_flags_t flag,
                       char *value)
{
    int  yes_no = -1;

    yes_no = is_yes(value);
    if (yes_no == TRUE) 
        *holder |= flag;
    else if (yes_no == FALSE)
        *holder &= ~flag;
    else
        return(yes_no);

    return sec_rgy_status_ok;
}


/*
 * FUNCTION: parse_account_attributes
 *
 * OVERVIEW:
 *
 * INPUTS:
 *
 * INPUT/OUTPUTS:
 *
 * RETURNS:
 */
int parse_account_attributes(Tcl_Interp *interp, 
                             rgy_acct_info_t *account, 
                             char *pkdigital,
                             char *pkcipher,
                             char *attr_list)
{
    int        i = 0;
    boolean32  list_list = FALSE;
    char       **pargv;
    int        pargc = 0;
    char       **aargv;
    int        aargc = 0;

    if (attr_list != NULL) {
        list_list = isllist(attr_list);
        if (list_list) {
            if (Tcl_SplitList(interp, attr_list, &pargc, &pargv) != TCL_OK)
                return TCL_ERROR;
        }
        else {
            pargc = 1;
            pargv = (char **)malloc(2*sizeof(char *));
            if (pargv == NULL) {
                DCP_SET_MISC_ERROR(dcp_s_no_memory);
            }
            pargv[0] = attr_list;
            pargv[1] = NULL;
        }

        for (; i < pargc; i++ ) {
            if (Tcl_SplitList(interp, pargv[i], &aargc, &aargv) != TCL_OK) {
                free((char *)pargv);
                return TCL_ERROR;
            }
            if (aargc < 2) {
                free((char *)pargv);
                DCP_SET_MISC_ERROR(dcp_s_rgy_bad_attribute_list);
            }

            switch (rgy_attribute(aargv[0])) {
                case rgy_acct_acctvalid:
                    account->acctvalid = dce_strdup(aargv[1]);
                    break;
                case rgy_acct_client:
                    account->client = dce_strdup(aargv[1]);
                    break;
                case rgy_acct_created:
                    account->created = dce_strdup(aargv[1]);
                    break;
                case rgy_acct_description: 
                    account->description = dce_strdup(aargv[1]);
                    break;
                case rgy_acct_dupkey: 
                    account->dupkey = dce_strdup(aargv[1]);
                    break;
                case rgy_acct_expdate:
                    account->expdate = dce_strdup(aargv[1]);
                    break;
                case rgy_acct_forwardabletkt:
                    account->forwardabletkt = dce_strdup(aargv[1]);
                    break;
                case rgy_acct_goodsince:
                    account->goodsince = dce_strdup(aargv[1]);
                    break;
                case rgy_acct_group:
                    account->group = dce_strdup(aargv[1]);
                    break;
                case rgy_acct_home:
                    account->home = dce_strdup(aargv[1]);
                    break;
                case rgy_acct_lastchange:
                    account->lastchange = dce_strdup(aargv[1]);
                    break;
                case rgy_acct_organization:
                    account->organization = dce_strdup(aargv[1]);
                    break;
                case rgy_acct_postdatedtkt:
                    account->postdatedtkt = dce_strdup(aargv[1]);
                    break;
                case rgy_acct_proxiabletkt:
                    account->proxiabletkt = dce_strdup(aargv[1]);
                    break;
                case rgy_acct_pwdvalid:
                    account->pwdvalid = dce_strdup(aargv[1]);
                    break;
                case rgy_acct_renewabletkt:
                    account->renewabletkt = dce_strdup(aargv[1]);
                    break;
                case rgy_acct_server:
                    account->server = dce_strdup(aargv[1]);
                    break;
                case rgy_acct_shell:
                    account->shell = dce_strdup(aargv[1]);
                    break;
                case rgy_acct_stdtgtauth:
                    account->stdtgtauth = dce_strdup(aargv[1]);
                    break;
                case rgy_acct_plcy_maxtktlife:
                    account->maxtktlife = dce_strdup(aargv[1]);
                    break;
                case rgy_acct_plcy_maxtktrenew:
                    account->maxtktrenew = dce_strdup(aargv[1]);
                    break;
                case rgy_acct_password:
                    account->password = dce_strdup(aargv[1]);
                    break;
                case rgy_acct_mypwd:
                    account->mypwd = dce_strdup(aargv[1]);
                    break;
                case rgy_acct_usertouser: 
                    account->usertouser = dce_strdup(aargv[1]);
                    break;
                case rgy_acct_pksignatureusage:
                    strcpy(pkdigital, (char *)aargv[1]);
                    break;
                case rgy_acct_pkkeycipherusage:
                    strcpy(pkcipher, (char *)aargv[1]);
                    break;
                case rgy_acct_pkgenprivkey:
                    account->pkgenprivkey = dce_strdup(aargv[1]);
                    break;
                case rgy_acct_pkmechanism:
                    account->pkmechanism = dce_strdup(aargv[1]);
                    break;
                default:
                    free((char *)pargv);
                    DCP_SET_SEC_ERROR(dcp_s_rgy_illegal_attr,aargv[0]);
            }
            free((char *)aargv);
        }
        free((char *)pargv);
    }
    return error_status_ok;
}


/*
 * FUNCTION: parse_pk_switches
 * 
 * OVERVIEW:
 *
 * INPUTS:
 *
 * INPUT/OUTPUTS:
 *
 * OUTPUTS:
 *
 * RETURNS:
 */
int parse_pk_switches(Tcl_Interp *interp,
                      rgy_pubkey_info_t *pkdinfo,
                      rgy_pubkey_info_t *pkcinfo,
                      char *pkdigital,
                      char *pkcipher)
{
    int        i;
    int        paramlen;
    int        genkey_int;
    char       *end;
    int        dargc, cargc, pargc;
    char       **dargv, **cargv, **pargv;
    boolean32  list_list = FALSE;
    boolean32  found_gen;
    boolean32  found_privkey;
    boolean32  found_pubkey;
    boolean32  found_oldpass;
    boolean32  found_newpass;

    if ((pkdigital != NULL) && (strlen(pkdigital) != 0)) {
        list_list = isllist(pkdigital);
        if (list_list) {
            if (Tcl_SplitList(interp, pkdigital, &dargc, &dargv) != TCL_OK) {
                return TCL_ERROR;
            }
        }
        else {
            dargc = 1;
            dargv = (char **)malloc(2 * sizeof(char *));
            if (dargv == NULL) {
                DCP_SET_MISC_ERROR(dcp_s_no_memory);
            }
            dargv[0] = pkdigital;
            dargv[1] = NULL;
        }

        found_gen = FALSE;
        found_privkey = FALSE;
        found_pubkey = FALSE;
        found_oldpass = FALSE;
        found_newpass = FALSE;
        for (i = 0; i < dargc; i++) {
            if (Tcl_SplitList(interp, dargv[i], &pargc, &pargv) != TCL_OK) {
                free((char *)dargv);
                return TCL_ERROR;
            }
            if (pargc < 2) {
                free((char *)dargv);
                free((char *)pargv);
                DCP_SET_MISC_ERROR(dcp_s_rgy_bad_attribute_list);
            }
            paramlen = strlen((char *)pargv[0]);
            if (strncmp(pk_attr_generatekey, pargv[0], paramlen) == 0) {
                /*
                 * Process generatekey value.
                 */
                if (found_gen == TRUE) {
                    free((char *)dargv);
                    free((char *)pargv);
                    DCP_SET_SEC_ERROR(dcp_s_rgy_pk_too_many_attr, pk_attr_generatekey);
                }
                genkey_int = -1;
                if (strncmp(pk_genkey_default_string, pargv[1], strlen(pargv[1])) == 0) {
                    pkdinfo->generatekey = pk_generate_default_value;
                }
                else {
                    genkey_int = is_number(pargv[1]);
                    if (genkey_int == dcp_rgy_not_int) {
                        free((char *)dargv);
                        free((char *)pargv);
                        DCP_SET_SEC_ERROR(dcp_s_rgy_pk_genkey_not_int, pk_attr_generatekey);
                    }
                    else if (genkey_int == dcp_rgy_int_out_of_range) {
                        free((char *)dargv);
                        free((char *)pargv);
                        DCP_SET_SEC_ERROR(dcp_s_rgy_pk_genkey_out_of_range, MAXINT);
                    }
                    genkey_int = strtol(pargv[1], &end, 10);
                    if (genkey_int < -1) {
                        free((char *)dargv);
                        free((char *)pargv);
                        DCP_SET_SEC_ERROR(dcp_s_rgy_pk_genkey_is_negative, pk_attr_generatekey);
                    }
                    pkdinfo->generatekey = genkey_int;
                }
                found_gen = TRUE;
            }
            else if (strncmp(pk_attr_privatekeyfile, pargv[0], paramlen) == 0) {
                /*
                 * Process privatekeyfile value.
                 */
                if (found_privkey == TRUE) {
                    free((char *)dargv);
                    free((char *)pargv);
                    DCP_SET_SEC_ERROR(dcp_s_rgy_pk_too_many_attr, pk_attr_privatekeyfile);
                }
                pkdinfo->privatekeyfile = dce_strdup((char *)pargv[1]);
                found_privkey = TRUE;
            }
            else if (strncmp(pk_attr_publickeyfile, pargv[0], paramlen) == 0) {
                /*
                 * Process publickeyfile value.
                 */
                if (found_pubkey == TRUE) {
                    free((char *)dargv);
                    free((char *)pargv);
                    DCP_SET_SEC_ERROR(dcp_s_rgy_pk_too_many_attr, pk_attr_publickeyfile);
                }
                pkdinfo->publickeyfile = dce_strdup((char *)pargv[1]);
                found_pubkey = TRUE;
            }
            else if (strncmp(pk_attr_oldpassphrase, pargv[0], paramlen) == 0) {
                /*
                 * Process oldpassphrase value.
                 */
                if (found_oldpass == TRUE) {
                    free((char *)dargv);
                    free((char *)pargv);
                    DCP_SET_SEC_ERROR(dcp_s_rgy_pk_too_many_attr, pk_attr_oldpassphrase);
                }
                pkdinfo->oldpassphrase = dce_strdup((char *)pargv[1]);
                found_oldpass = TRUE;
            }
            else if (strncmp(pk_attr_newpassphrase, pargv[0], paramlen) == 0) {
                /*
                 * Process newpassphrase value.
                 */
                if (found_newpass == TRUE) {
                    free((char *)dargv);
                    free((char *)pargv);
                    DCP_SET_SEC_ERROR(dcp_s_rgy_pk_too_many_attr, pk_attr_newpassphrase);
                }
                pkdinfo->newpassphrase = dce_strdup((char *)pargv[1]);
                found_newpass = TRUE;
            }
            else {
                /*
                 * Bad parameter found.
                 */
                free((char *)dargv);
                DCP_SET_SEC_ERROR(dcp_s_rgy_pk_unknown_attr, pargv[0]);
            }
            free((char *)pargv);
        }
        free((char *)dargv);
    }

    if ((pkcipher != NULL) && (strlen(pkcipher) != 0)) {
        list_list = isllist(pkcipher);
        if (list_list) {
            if (Tcl_SplitList(interp, pkcipher, &cargc, &cargv) != TCL_OK) {
                return TCL_ERROR;
            }
        }
        else {
            cargc = 1;
            cargv = (char **)malloc(2 * sizeof(char *));
            if (cargv == NULL) {
                DCP_SET_MISC_ERROR(dcp_s_no_memory);
            }
            cargv[0] = pkcipher;
            cargv[1] = NULL;
        }

        found_gen = FALSE;
        found_privkey = FALSE;
        found_pubkey = FALSE;
        found_oldpass = FALSE;
        found_newpass = FALSE;
        for (i = 0; i < cargc; i++) {
            if (Tcl_SplitList(interp, cargv[i], &pargc, &pargv) != TCL_OK) {
                free((char *)cargv);
                return TCL_ERROR;
            }
            if (pargc < 2) {
                free((char *)cargv);
                free((char *)pargv);
                DCP_SET_MISC_ERROR(dcp_s_rgy_bad_attribute_list);
            }
            paramlen = strlen((char *)pargv[0]);
            if (strncmp(pk_attr_generatekey, pargv[0], paramlen) == 0) {
                /*
                 * Process generatekey value.
                 */
                if (found_gen == TRUE) {
                    free((char *)cargv);
                    free((char *)pargv);
                    DCP_SET_SEC_ERROR(dcp_s_rgy_pk_too_many_attr, pk_attr_generatekey);
                }
                genkey_int = -1;
                if (strncmp(pk_genkey_default_string, pargv[1], strlen(pargv[1])) == 0) {
                    pkcinfo->generatekey = pk_generate_default_value;
                }
                else {
                    genkey_int = is_number(pargv[1]);
                    if (genkey_int == dcp_rgy_not_int) {
                        free((char *)dargv);
                        free((char *)pargv);
                        DCP_SET_SEC_ERROR(dcp_s_rgy_pk_genkey_not_int, pk_attr_generatekey);
                    }
                    else if (genkey_int == dcp_rgy_int_out_of_range) {
                        free((char *)dargv);
                        free((char *)pargv);
                        DCP_SET_SEC_ERROR(dcp_s_rgy_pk_genkey_out_of_range, MAXINT);
                    }
                    genkey_int = strtol(pargv[1], &end, 10);
                    if (genkey_int < -1) {
                        free((char *)dargv);
                        free((char *)pargv);
                        DCP_SET_SEC_ERROR(dcp_s_rgy_pk_genkey_is_negative, pk_attr_generatekey);
                    }
                    pkcinfo->generatekey = genkey_int;
                }
                found_gen = TRUE;
            }
            else if (strncmp(pk_attr_privatekeyfile, pargv[0], paramlen) == 0) {
                /*
                 * Process privatekeyfile value.
                 */
                if (found_privkey == TRUE) {
                    free((char *)cargv);
                    free((char *)pargv);
                    DCP_SET_SEC_ERROR(dcp_s_rgy_pk_too_many_attr, pk_attr_privatekeyfile);
                }
                pkcinfo->privatekeyfile = dce_strdup((char *)pargv[1]);
                found_privkey = TRUE;
            }
            else if (strncmp(pk_attr_publickeyfile, pargv[0], paramlen) == 0) {
                /*
                 * Process publickeyfile value.
                 */
                if (found_pubkey == TRUE) {
                    free((char *)cargv);
                    free((char *)pargv);
                    DCP_SET_SEC_ERROR(dcp_s_rgy_pk_too_many_attr, pk_attr_publickeyfile);
                }
                pkcinfo->publickeyfile = dce_strdup((char *)pargv[1]);
                found_pubkey = TRUE;
            }
            else if (strncmp(pk_attr_oldpassphrase, pargv[0], paramlen) == 0) {
                /*
                 * Process oldpassphrase value.
                 */
                if (found_oldpass == TRUE) {
                    free((char *)cargv);
                    free((char *)pargv);
                    DCP_SET_SEC_ERROR(dcp_s_rgy_pk_too_many_attr, pk_attr_oldpassphrase);
                }
                pkcinfo->oldpassphrase = dce_strdup((char *)pargv[1]);
                found_oldpass = TRUE;
            }
            else if (strncmp(pk_attr_newpassphrase, pargv[0], paramlen) == 0) {
                /*
                 * Process newpassphrase value.
                 */
                if (found_newpass == TRUE) {
                    free((char *)cargv);
                    free((char *)pargv);
                    DCP_SET_SEC_ERROR(dcp_s_rgy_pk_too_many_attr, pk_attr_newpassphrase);
                }
                pkcinfo->newpassphrase = dce_strdup((char *)pargv[1]);
                found_newpass = TRUE;
            }
            else {
                /*
                 * Bad parameter found.
                 */
                free((char *)cargv);
                free((char *)pargv);
                DCP_SET_SEC_ERROR(dcp_s_rgy_pk_unknown_attr, pargv[0]);
            }
            free((char *)pargv);
        }
        free((char *)cargv);
    }
    return error_status_ok;
}


/*
 * FUNCTION: check_pk_create_arguments()
 * 
 * OVERVIEW:
 *
 * INPUTS:
 *
 * INPUT/OUTPUTS:
 *
 * OUTPUTS:
 *
 * RETURNS:
 */
int check_pk_create_arguments(Tcl_Interp *interp,
                              rgy_pubkey_info_t *pkinfo,
                              char *keytype)
{
    if (!pkinfo->newpassphrase) {
        char  pwprompt[128];
        int   passwd_len = sec_passwd_str_max_len;

        sprintf(pwprompt, "Enter passphrase for %s key: ", keytype);
        pkinfo->newpassphrase = (char *)malloc(sec_passwd_str_max_len);
        if (!(pkinfo->newpassphrase)) {
            DCP_SET_MISC_ERROR(dcp_s_no_memory);
        }
        memset(pkinfo->newpassphrase, 0, sec_passwd_str_max_len - 1);
        if (krb5_read_password(pwprompt,
                               "Again: ",
                               (char *)pkinfo->newpassphrase,
                               &passwd_len) != 0) {
            setup_signal_handling();
            free(pkinfo->newpassphrase);
            pkinfo->newpassphrase = NULL;
            DCP_SET_MISC_ERROR(dcp_s_rgy_pk_missing_newpass);
        }
        setup_signal_handling();
    }

    if (pkinfo->oldpassphrase) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_pk_illegal_oldpass);
    }

    if ((pkinfo->generatekey > 0) && ((pkinfo->privatekeyfile) || (pkinfo->publickeyfile))) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_pk_gen_and_key);
    }

    if ((pkinfo->generatekey <= 0) && (!pkinfo->privatekeyfile) && (!pkinfo->publickeyfile)) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_pk_missing_key);
    }

    if (((pkinfo->privatekeyfile) && (!pkinfo->publickeyfile)) ||
        ((!pkinfo->privatekeyfile) && (pkinfo->publickeyfile))) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_pk_missing_key);
    }

    if ((pkinfo->privatekeyfile) && (pkinfo->publickeyfile)) {
        if (strcmp((char *)pkinfo->privatekeyfile,
                   (char *)pkinfo->publickeyfile) == 0) {
            DCP_SET_MISC_ERROR(dcp_s_rgy_pk_same_filename);
        }
    }
    return error_status_ok;
}


/*
 * FUNCTION: check_pk_modify_arguments()
 * 
 * OVERVIEW:
 *
 * INPUTS:
 *
 * INPUT/OUTPUTS:
 *
 * OUTPUTS:
 *
 * RETURNS:
 */
int check_pk_modify_arguments(Tcl_Interp *interp,
                              rgy_pubkey_info_t *pkinfo,
                              char *keytype)
{
    if ((pkinfo->generatekey > 0) && ((pkinfo->privatekeyfile) || (pkinfo->publickeyfile))) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_pk_gen_and_key);
    }

    if ((pkinfo->oldpassphrase) && (pkinfo->oldpassphrase[0] == '\0')) {
        char  pwprompt[128];
        int   passwd_len = sec_passwd_str_max_len;

        sprintf(pwprompt, "Enter old passphrase for %s key: ", keytype);
        pkinfo->oldpassphrase = (char *)malloc(sec_passwd_str_max_len);
        if (!(pkinfo->oldpassphrase)) {
            DCP_SET_MISC_ERROR(dcp_s_no_memory);
        }
        memset(pkinfo->oldpassphrase, 0, sec_passwd_str_max_len - 1);
        if (krb5_read_password(pwprompt,
                               "Again: ",
                               (char *)pkinfo->oldpassphrase,
                               &passwd_len) != 0) {
            setup_signal_handling();
            free(pkinfo->oldpassphrase);
            pkinfo->oldpassphrase = NULL;
            DCP_SET_MISC_ERROR(dcp_s_rgy_pk_missing_oldpass);
        }
        setup_signal_handling();
    }

    if ((pkinfo->newpassphrase) && (pkinfo->newpassphrase[0] == '\0')) {
        char  pwprompt[128];
        int   passwd_len = sec_passwd_str_max_len;

        sprintf(pwprompt, "Enter new passphrase for %s key: ", keytype);
        pkinfo->newpassphrase = (char *)malloc(sec_passwd_str_max_len);
        if (!(pkinfo->newpassphrase)) {
            DCP_SET_MISC_ERROR(dcp_s_no_memory);
        }
        memset(pkinfo->newpassphrase, 0, sec_passwd_str_max_len - 1);
        if (krb5_read_password(pwprompt,
                               "Again: ",
                               (char *)pkinfo->newpassphrase,
                               &passwd_len) != 0) {
            setup_signal_handling();
            free(pkinfo->newpassphrase);
            pkinfo->newpassphrase = NULL;
            DCP_SET_MISC_ERROR(dcp_s_rgy_pk_missing_newpass);
        }
        setup_signal_handling();
    }

    if ((!pkinfo->oldpassphrase) && (!pkinfo->newpassphrase)) {
        if ((pkinfo->generatekey > 0) || (pkinfo->privatekeyfile) || (pkinfo->publickeyfile)) {
            DCP_SET_MISC_ERROR(dcp_s_rgy_pk_missing_eitherpass);
        }
        else {
            DCP_SET_MISC_ERROR(dcp_s_rgy_pk_missing_argument);
        }
    }

    if (((pkinfo->privatekeyfile) && (!pkinfo->publickeyfile)) ||
        ((!pkinfo->privatekeyfile) && (pkinfo->publickeyfile))) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_pk_missing_key);
    }

    if ((pkinfo->generatekey <= 0) && (!pkinfo->oldpassphrase) && (!pkinfo->privatekeyfile)) {
        char  pwprompt[128];
        int   passwd_len = sec_passwd_str_max_len;

        sprintf(pwprompt, "Enter old passphrase for %s key: ", keytype);
        pkinfo->oldpassphrase = (char *)malloc(sec_passwd_str_max_len);
        if (!(pkinfo->oldpassphrase)) {
            DCP_SET_MISC_ERROR(dcp_s_no_memory);
        }
        memset(pkinfo->oldpassphrase, 0, sec_passwd_str_max_len - 1);
        if (krb5_read_password(pwprompt,
                               "Again: ",
                               (char *)pkinfo->oldpassphrase,
                               &passwd_len) != 0) {
            setup_signal_handling();
            free(pkinfo->oldpassphrase);
            pkinfo->oldpassphrase = NULL;
            DCP_SET_MISC_ERROR(dcp_s_rgy_pk_missing_oldpass);
        }
        setup_signal_handling();
    }

    if ((pkinfo->generatekey <= 0) && (!pkinfo->newpassphrase) && (!pkinfo->privatekeyfile)) {
        char  pwprompt[128];
        int   passwd_len = sec_passwd_str_max_len;

        sprintf(pwprompt, "Enter new passphrase for %s key: ", keytype);
        pkinfo->newpassphrase = (char *)malloc(sec_passwd_str_max_len);
        if (!(pkinfo->newpassphrase)) {
            DCP_SET_MISC_ERROR(dcp_s_no_memory);
        }
        memset(pkinfo->newpassphrase, 0, sec_passwd_str_max_len - 1);
        if (krb5_read_password(pwprompt,
                               "Again: ",
                               (char *)pkinfo->newpassphrase,
                               &passwd_len) != 0) {
            setup_signal_handling();
            free(pkinfo->newpassphrase);
            pkinfo->newpassphrase = NULL;
            DCP_SET_MISC_ERROR(dcp_s_rgy_pk_missing_newpass);
        }
        setup_signal_handling();
    }

    if ((pkinfo->privatekeyfile) && (pkinfo->publickeyfile)) {
        if (strcmp((char *)pkinfo->privatekeyfile, (char *)pkinfo->publickeyfile) == 0) {
            DCP_SET_MISC_ERROR(dcp_s_rgy_pk_same_filename);
        }
    }
    return error_status_ok;
}


/*
 * FUNCTION: check_pk_file_contents()
 * 
 * OVERVIEW:
 *
 * INPUTS:
 *
 * INPUT/OUTPUTS:
 *
 * OUTPUTS:
 *
 * RETURNS:
 */
int check_pk_file_contents(Tcl_Interp *interp,
                           rgy_pubkey_info_t *pkinfo)
{
    FILE        *fp = 0;
    char        buff[256];
    unsigned32  result;

    if (pkinfo->publickeyfile) {
        buff[0] = '\0';
        if ((fp = fopen(pkinfo->publickeyfile, "r")) == NULL) {
            DCP_SET_SEC_ERROR(dcp_s_rgy_pk_file_open_fail, pkinfo->publickeyfile);
        }
        if (fgets(buff, sizeof(buff), fp) == NULL) {
            fclose(fp);
            DCP_SET_SEC_ERROR(dcp_s_rgy_pk_file_read_fail, pkinfo->publickeyfile);
        }
        buff[strlen(buff)-1] = '\0';
        fclose(fp);
    }

    if (pkinfo->privatekeyfile) {
        buff[0] = '\0';
        if ((fp = fopen(pkinfo->privatekeyfile, "r")) == NULL) {
            DCP_SET_SEC_ERROR(dcp_s_rgy_pk_file_open_fail, pkinfo->privatekeyfile);
        }
        if (fgets(buff, sizeof(buff), fp) == NULL) {
            fclose(fp);
            DCP_SET_SEC_ERROR(dcp_s_rgy_pk_file_read_fail, pkinfo->privatekeyfile);
        }
        buff[strlen(buff)-1] = '\0';
        fclose(fp);
    }

    return error_status_ok;
}


/*
 * FUNCTION: free_account_ptrs
 *
 * OVERVIEW: Free the memory allocated by parse_account_attributes().
 *
 * INPUTS:
 *
 * INPUT/OUTPUTS:
 *
 * RETURNS:
 */
void free_account_ptrs (rgy_acct_info_t *account)
{
    if (account->acctvalid) free((char*) account->acctvalid);
    if (account->client) free((char*) account->client);
    if (account->created) free((char*) account->created);
    if (account->description) free((char*) account->description);
    if (account->dupkey) free((char*) account->dupkey);
    if (account->expdate) free((char*) account->expdate);
    if (account->forwardabletkt) free((char*) account->forwardabletkt);
    if (account->goodsince) free((char*) account->goodsince);
    if (account->group) free((char*) account->group);
    if (account->home) free((char*) account->home);
    if (account->lastchange) free((char*) account->lastchange);
    if (account->organization) free((char*) account->organization);
    if (account->postdatedtkt) free((char*) account->postdatedtkt);
    if (account->proxiabletkt) free((char*) account->proxiabletkt);
    if (account->pwdvalid) free((char*) account->pwdvalid);
    if (account->renewabletkt) free((char*) account->renewabletkt);
    if (account->server) free((char*) account->server);
    if (account->shell) free((char*) account->shell);
    if (account->stdtgtauth) free((char*) account->stdtgtauth);
    if (account->maxtktlife) free((char*) account->maxtktlife);
    if (account->maxtktrenew) free((char*) account->maxtktrenew);
    if (account->password) free((char*) account->password);
    if (account->mypwd) free((char*) account->mypwd);
    if (account->usertouser) free((char*) account->usertouser);
    if (account->pkgenprivkey) free((char*) account->pkgenprivkey);
    if (account->pkmechanism) free((char*) account->pkmechanism);
}

int date_to_seconds(sec_timeval_sec_t *secs,char *string)
{
    int         result;
    utc_t       utc;
    timespec_t  timesp;
    timespec_t  inaccsp;
    long        tdf;

    result = utc_mkasctime(&utc,string);
    if (result)
        return dcp_s_rgy_bad_time_string;

    result = utc_bintime(&timesp,&inaccsp,&tdf,&utc);
    if (result)
        return dcp_s_rgy_bad_time_string;

    *secs = timesp.tv_sec;
    
    return error_status_ok;
}

int reldate_to_seconds(sec_timeval_sec_t *secs,char *string)
{
    int            result;
    utc_t          utc;
    reltimespec_t  timesp;
    timespec_t     inaccsp;

    result = utc_mkascreltime(&utc,string);
    if (result)
        return dcp_s_rgy_bad_reltime_string;

    result = utc_binreltime(&timesp,&inaccsp,&utc);
    if (result)
        return dcp_s_rgy_bad_reltime_string;

    *secs = timesp.tv_sec;

    return error_status_ok;
}


/*
 * FUNCTION: format_registry_prop
 *
 * OVERVIEW: This function will format and display the attributes and 
 *           policies of a registry. 
 * 
 *
 * INPUTS:
 *  boolean32, - format only the attributes. 
 *  boolean32, - format the policy information.
 *  sec_rgy_properties_t *, - structure containing the property information.
 *  rs_sw_version_t *, - the character string containing the version information.
 *  sec_rgy_plcy_t *, - policy information for the registry.
 *  sec_rgy_plcy_auth_t *, - registry wide policy information for accounts.
 *
 * INPUT/OUTPUTS:
 *  Tcl_Interp *interp, - interpreter to store information
 *
 *
 * RETURNS:
 *  TCL_OK on success. 
 */
int format_registry_prop(Tcl_Interp *interp, 
                         boolean32 attributes,
                         boolean32 policies,
                         sec_rgy_properties_t *properties,
                         rs_sw_version_t *version,
                         sec_rgy_plcy_t *reg_policy,
                         sec_rgy_plcy_auth_t *auth_policy)
{
    char          *deftktlife_svc;
    char          *mingid_svc;
    char          *minorgid_svc;
    char          *minuid_svc;
    char          *maxuid_svc;
    char          *mintktlife_svc;
    char          *hidepwd_svc;
    char          *acclife_svc;    /* account lifetime */
    char          *pwdminlen_svc;  /* passwd min length */
    char          *pwdlife_svc;    /* pwdlifetime */
    char          *version_svc;    /* version */
    char          *expdate_svc;    /* psswd expiration */
    char          *spaces_svc;     /* passwd spaces ? */
    char          *alphan_svc;     /* passwd alphanumeric ? */
    char          *maxtktlife_svc;
    char          *maxtktrenew_svc;
    char          *unlimited_svc;
    char          *none_svc;
    char          *yes_svc;
    char          *no_svc;
    char          actual_date[UTC_MAX_STR_LEN];
    char          buffer[1024];
    reltimespec_t timespec;
    utc_t         utc;
    char          dt_string[UTC_MAX_STR_LEN];

    yes_svc = (char *)dce_sprintf(dcp_t_rgy_yes);
    no_svc = (char *)dce_sprintf(dcp_t_rgy_no);
    if (attributes) {
        mingid_svc = (char *)dce_sprintf(dcp_t_rgy_mingid);
        minorgid_svc = (char *)dce_sprintf(dcp_t_rgy_minorgid);
        minuid_svc = (char *)dce_sprintf(dcp_t_rgy_minuid);
        maxuid_svc = (char *)dce_sprintf(dcp_t_rgy_maxuid);
        deftktlife_svc = (char *)dce_sprintf(dcp_t_rgy_deftktlife);
        mintktlife_svc = (char *)dce_sprintf(dcp_t_rgy_mintktlife);
        version_svc = (char *)dce_sprintf(dcp_t_rgy_version);
        hidepwd_svc = (char *)dce_sprintf(dcp_t_rgy_hidepwd);
        timespec.tv_sec = properties->default_certificate_lifetime;
        timespec.tv_nsec = 0;
        utc_mkbinreltime(&utc, &timespec, NULL);
        if (utc_ascreltime(dt_string, UTC_MAX_STR_LEN, &utc))
            strcpy(dt_string,"{}");
        sprintf(buffer, "{%s %s}", deftktlife_svc, dt_string);
        APPEND_VALUE(buffer);
        if(properties->flags & sec_rgy_prop_shadow_passwd)
            sprintf(buffer, "{%s %s}", hidepwd_svc, yes_svc);
        else
            sprintf(buffer, "{%s %s}", hidepwd_svc, no_svc);
        APPEND_VALUE(buffer);
        sprintf(buffer, "{%s %d}", maxuid_svc, properties->max_unix_id);
        APPEND_VALUE(buffer);
        sprintf(buffer, "{%s %d}", mingid_svc, properties->low_unix_id_group);
        APPEND_VALUE(buffer);
        sprintf(buffer, "{%s %d}", minorgid_svc, properties->low_unix_id_org);
        APPEND_VALUE(buffer);
        timespec.tv_sec = properties->minimum_ticket_lifetime;
        timespec.tv_nsec = 0;
        utc_mkbinreltime(&utc, &timespec, NULL);
        if (utc_ascreltime(dt_string, UTC_MAX_STR_LEN, &utc))
            strcpy(dt_string, "{}");
        sprintf(buffer, "{%s %s}", mintktlife_svc, dt_string);
        APPEND_VALUE(buffer);
        sprintf(buffer, "{%s %d}", minuid_svc, properties->low_unix_id_person);
        APPEND_VALUE(buffer);
        sprintf(buffer,"{%s %s}", version_svc, version);
        APPEND_VALUE(buffer);
        free(deftktlife_svc);
        free(mintktlife_svc);
        free(hidepwd_svc);
        free(version_svc);
        free(mingid_svc);
        free(minorgid_svc);
        free(minuid_svc);
        free(maxuid_svc);
    }
    if (policies) {
       acclife_svc = (char *)dce_sprintf(dcp_t_rgy_acctlife);
       pwdminlen_svc = (char *)dce_sprintf(dcp_t_rgy_pwdminlen);
       pwdlife_svc = (char *)dce_sprintf(dcp_t_rgy_pwdlife);
       expdate_svc = (char *)dce_sprintf(dcp_t_rgy_pwdexpdate);
       spaces_svc = (char *)dce_sprintf(dcp_t_rgy_pwdspaces);
       alphan_svc = (char *)dce_sprintf(dcp_t_rgy_pwdalpha); 
       maxtktlife_svc = (char *)dce_sprintf(dcp_t_rgy_maxtktlife);
       maxtktrenew_svc = (char *)dce_sprintf(dcp_t_rgy_maxtktrenew);
       none_svc = (char *)dce_sprintf(dcp_t_rgy_none);
       unlimited_svc = (char *)dce_sprintf(dcp_t_rgy_unlimited);
       if (reg_policy->acct_lifespan == org_pol_live_forever)
           sprintf(buffer, "{%s %s}", acclife_svc, unlimited_svc);
       else {
           sec_to_rel_date_string(reg_policy->acct_lifespan, actual_date);
           sprintf(buffer, "{%s %s}", acclife_svc, actual_date);
       }  
       APPEND_VALUE(buffer);
       sec_to_rel_date_string(auth_policy->max_ticket_lifetime,actual_date);
       sprintf(buffer, "{%s %s}", maxtktlife_svc, actual_date);
       APPEND_VALUE(buffer);
       sec_to_rel_date_string(auth_policy->max_renewable_lifetime,actual_date);
       sprintf(buffer, "{%s %s}", maxtktrenew_svc, actual_date);
       APPEND_VALUE(buffer);
       if (reg_policy->passwd_flags & sec_rgy_plcy_pwd_non_alpha)
           sprintf(buffer, "{%s %s}", alphan_svc, no_svc);
       else 
           sprintf(buffer, "{%s %s}", alphan_svc, yes_svc);
       APPEND_VALUE(buffer);
       if (reg_policy->passwd_exp_date == org_pol_never_expire)
           sprintf(buffer, "{%s %s}", expdate_svc, none_svc);
       else {
           sec_to_date_string(reg_policy->passwd_exp_date,actual_date);
           sprintf(buffer, "{%s %s}", expdate_svc, actual_date);
       }
       APPEND_VALUE(buffer);
       if (reg_policy->passwd_lifetime == org_pol_live_forever) {
           sprintf(buffer, "{%s %s}", pwdlife_svc, unlimited_svc);
       }
       else {
           sec_to_rel_date_string(reg_policy->passwd_lifetime, actual_date);
           sprintf(buffer, "{%s %s}", pwdlife_svc, actual_date);
       }
       APPEND_VALUE(buffer);
       sprintf(buffer, "{%s %d}", pwdminlen_svc, reg_policy->passwd_min_len);
       APPEND_VALUE(buffer);
       if (reg_policy->passwd_flags & sec_rgy_plcy_pwd_no_spaces)
           sprintf(buffer, "{%s %s}", spaces_svc, no_svc);
       else 
           sprintf(buffer, "{%s %s}", spaces_svc, yes_svc);
       APPEND_VALUE(buffer);
       free(acclife_svc);
       free(pwdminlen_svc);
       free(pwdlife_svc);
       free(expdate_svc);
       free(spaces_svc);
       free(alphan_svc);
       free(maxtktlife_svc);
       free(maxtktrenew_svc);
       free(none_svc);
       free(unlimited_svc);
     }
    free(yes_svc);
    free(no_svc);
    return TCL_OK;
}


/*
 * FUNCTION: update_registry_properties
 *
 * OVERVIEW: This function updates registry properties from switch values.
 *
 * INPUT/OUTPUTS:
 *  Tcl_Interp *interp - interpreter to store resultant error messages in.
 *  rgy_policy_info_t *policy - structure holding the properties and ascii 
 *      valies to modify the properties. 
 *
 * RETURNS:
 *  TCL_OK on success. 
 */
int update_registry_properties(Tcl_Interp *interp,
                               rgy_policy_info_t *policy)

{
    unsigned32                 result = sec_rgy_status_ok;
    sec_timeval_sec_t          seconds;
    int                        i;
    unsigned32                 num_versions = 0;
    unsigned32                 current_version = 0;
    rs_replica_sw_vers_info_t  *ver_info;

    if(policy->deftktlife){
        result = reldate_to_seconds(&seconds,policy->deftktlife);
        DCP_CHECK_SEC_ERROR(result);
        policy->properties->default_certificate_lifetime = seconds;
    }
    if (policy->mintktlife) {
        result = reldate_to_seconds(&seconds,policy->mintktlife);
        DCP_CHECK_SEC_ERROR(result);
        policy->properties->minimum_ticket_lifetime = seconds;
    }
    if (policy->hidepwd) {
        result = add_yes_no_flag(policy->hidepwd,
                                 (signed32 *)&policy->properties->flags,
                                 sec_rgy_prop_shadow_passwd);
        if (result == dcp_s_rgy_invalid_pnvalue) {
            DCP_SET_SEC_ERROR(dcp_s_bad_arg,policy->hidepwd);
        }
        DCP_CHECK_SEC_ERROR(result);
    }
    if (policy->maxuid) {
        result = add_maxuid(policy->maxuid,policy->properties);
        if (result == dcp_s_maxuid_out_of_range) {
            DCP_SET_SEC_ERROR(result,MAXINT);
        }
        DCP_CHECK_SEC_ERROR(result);
    }
    if (policy->mingid) {
        result = add_mingid(policy->mingid,policy->properties);
        if (result == dcp_s_mingid_out_of_range) {
            DCP_SET_SEC_ERROR(result,MAXINT);
        }
        DCP_CHECK_SEC_ERROR(result);
    }
    if (policy->minorgid) {
        result = add_minorgid(policy->minorgid,policy->properties);
        if (result == dcp_s_minorgid_out_of_range) {
            DCP_SET_SEC_ERROR(result,MAXINT);
        }
        DCP_CHECK_SEC_ERROR(result);
    }
    if (policy->minuid) {
        result = add_minuid(policy->minuid,policy->properties);
        if(result == dcp_s_minuid_out_of_range) {
            DCP_SET_SEC_ERROR(result,MAXINT);
        }
        DCP_CHECK_SEC_ERROR(result);
    }
    if (policy->pwdlife) {
        result = add_pwdlife(policy->pwdlife,policy->policy);
        DCP_CHECK_SEC_ERROR(result);
    }
    if (policy->acctlife) {
        result = add_acctlife(policy->acctlife,policy->policy);
        DCP_CHECK_SEC_ERROR(result);
    }
    if (policy->pwdminlen) {
        result = add_pwdminlen(policy->pwdminlen,policy->policy);
        if (result == dcp_s_rgy_pwdminlen_out_of_range)
            DCP_SET_SEC_ERROR(result,MAXINT);
        DCP_CHECK_SEC_ERROR(result);
    }
    if (policy->expdate) {
        result = add_pwdexpdate(policy->expdate,policy->policy);
        DCP_CHECK_SEC_ERROR(result);
    }
    if (policy->pwdspaces) {
        result = add_policy_flag(policy->pwdspaces,
                                 policy->policy,
                                 sec_rgy_plcy_pwd_no_spaces);
        DCP_CHECK_SEC_ERROR(result);
    }
    if (policy->pwdalpha) {
        result = add_policy_flag(policy->pwdalpha,
                                 policy->policy,
                                 sec_rgy_plcy_pwd_non_alpha);
        DCP_CHECK_SEC_ERROR(result);
    }
    if (policy->maxtktlife) {
        result = reldate_to_seconds(&(policy->auth_policy->max_ticket_lifetime),
                  policy->maxtktlife);
        DCP_CHECK_SEC_ERROR(result);
    }
    if (policy->maxtktrenew) {
        result = reldate_to_seconds(&(policy->auth_policy->max_renewable_lifetime),
                  policy->maxtktrenew);
        DCP_CHECK_SEC_ERROR(result);
    }
    if (policy->version) {
        /*
         * Check that the version the user entered is legal.
         * If so, make the call to set the new version.
         */
        sec_rgy_rep_admin_get_sw_vers(rgy_context,
                                      &num_versions,
                                      &ver_info,
                                      &current_version,
                                      &result);
        DCP_CHECK_SEC_ERROR(result);
        for (i = 0; i < num_versions; i++) {
            if ((char *)strcmp(policy->version, (char *)ver_info[i].sw_vers_printstring) == 0) {
                break;
            }
        }
        if (num_versions == 0) {
            DCP_SET_MISC_ERROR(dcp_s_rgy_no_versions_found);
        }
        if (i == num_versions) {
            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_rgy_illegal_version, policy->version);
            return TCL_ERROR;
        }
        /*
         * Set the new version.
         */
        sec_rgy_rep_admin_set_sw_vers(rgy_context,
                                      ver_info[i].sw_vers,
                                      &result);
        DCP_CHECK_SEC_ERROR(result);
        free((rs_replica_sw_vers_info_t *)ver_info);
    }

    return TCL_OK;
}


/*
 * FUNCTION: add_yes_no_flag
 *
 * OVERVIEW: This function will update a flag given a yes/no ascii 
 *    string. 
 * 
 * INPUTS:
 *  char *value - char constant either yes/no
 *  int flag    - flag value
 *
 * INPUT/OUTPUTS:
 *  signed32 flag_list = flag buffer  
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  sec_rgy_status_ok on success
 */

int add_yes_no_flag(char *value, signed32 *flag_list, int flag)
{
    int  yes_no = -1;

    if(*flag_list == (unsigned32)-1)
        *flag_list = 0;

    yes_no = is_yes(value);
    if(yes_no == TRUE) 
        *flag_list |= flag;
    else if(yes_no == FALSE)
        *flag_list = *flag_list & ~flag;
    else
        return(yes_no);

    return sec_rgy_status_ok;
}


/*
 * FUNCTION: add_maxuid
 *
 * OVERVIEW: This function will take an ascii string for an integer and 
 *     modify the sec_rgy_properties_t structure to update the 
 *     maximum available uid. 
 * 
 *
 * INPUTS:
 *  char *maxuid - ascii string representing an integer. 
 *
 * INPUT/OUTPUTS:
 *  sec_rgy_properties_t *properties - pointer to a structure containing 
 *  registry attributes.
 *
 * RETURNS:
 *  sec_rgy_status_ok on success.... 
 */
int add_maxuid(char *maxuid, sec_rgy_properties_t *properties )
{
    int maxuid_no = 0;
    int status;
    char *end;

    status = is_number(maxuid);
    if(status == dcp_rgy_not_int)
        return dcp_s_maxuid_not_int;
    else if(status == dcp_rgy_int_out_of_range)
        return dcp_s_maxuid_out_of_range;

    maxuid_no = strtol(maxuid,&end,10);
    if(maxuid_no < 0) 
        return dcp_s_maxuid_is_negative;
    else
        properties->max_unix_id = maxuid_no;
    
    return sec_rgy_status_ok;
}


/*
 * FUNCTION: add_minorgid
 *
 * OVERVIEW: This function encodes an ascii string as a sec_rgy_properties_t
 *      attribute for low_unix_id_org and stores the result in the 
 *      attribute structure. 
 * 
 *
 * INPUTS:
 *  char *minorgid -  ascii string containing attribute value.
 *
 * INPUT/OUTPUTS:
 *  sec_rgy_properties_t *properties - pointer to structure containing
 *         registry attributes. 
 *
 * RETURNS:
 *  sec_rgy_status_ok - on success. 
 */
int add_minorgid(char *minorgid, sec_rgy_properties_t *properties )
{
    int minorgid_no = 0;
    int status;
    char *end;

    status = is_number(minorgid);
    if(status == dcp_rgy_not_int)
        return dcp_s_minorgid_not_int;
    else if(status == dcp_rgy_int_out_of_range)
        return dcp_s_minorgid_out_of_range;

    minorgid_no = strtol(minorgid,&end,10);
    if(minorgid_no < 0) 
        return dcp_s_minorgid_is_negative;
    else
        properties->low_unix_id_org = minorgid_no;
    
    return sec_rgy_status_ok;
}


/*
 * FUNCTION: add_minuid
 *
 * OVERVIEW: This function encodes an ascii string as a 
 *     sec_rgy_properties_t low_unix_id_person attribute 
 *     for the master registry.
 * 
 *
 * INPUTS:
 *   char minuid  - ascii string representing the integer
 *
 * INPUT/OUTPUTS:
 *  sec_rgy_properties_t *properties - pointer to structure containing 
 *          registry properties. 
 *
 * RETURNS:
 *  sec_rgy_status_ok on success
 */
int add_minuid(char *minuid, sec_rgy_properties_t *properties )
{
    int minuid_no = 0;
    int status; 
    
    status = is_number(minuid);
    if(status == dcp_rgy_not_int)
        return dcp_s_minuid_not_int;
    else if (status == dcp_rgy_int_out_of_range)
        return dcp_s_minuid_out_of_range;
    
    minuid_no = atoi(minuid);
    if(minuid_no < 0) 
        return dcp_s_minuid_is_negative;
    else
        properties->low_unix_id_person = minuid_no;
    
    return sec_rgy_status_ok;
}


/*
 * FUNCTION: add_mingid
 *
 * OVERVIEW: This function encodes an ascii integer and stores it 
 *           sec_rgy_properties_t structure as the low_unix_id_group
 *           attribute
 * 
 *
 * INPUTS:
 *  char *mingid - ascii integer 
 *
 * INPUT/OUTPUTS:
 *  sec_rgy_properties_t *properties - registry properties 
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  sec_rgy_status_ok on success
 */
int add_mingid(char *mingid, sec_rgy_properties_t *properties )
{
    int mingid_no = 0;
    int status; 

    status = is_number(mingid);
    if(status == dcp_rgy_not_int)
        return dcp_s_mingid_not_int;
    else if (status == dcp_rgy_int_out_of_range)
        return dcp_s_mingid_out_of_range;
    mingid_no = atoi(mingid);
    if(mingid_no < 0) 
        return dcp_s_mingid_is_negative;
    else
        properties->low_unix_id_group = mingid_no;
    
    return sec_rgy_status_ok;
}


/*
 * FUNCTION: parse_registry_prop_list
 *
 * OVERVIEW: This function will parse the attributes of a registry 
 *     TCL attribute list.
 *
 * INPUT/OUTPUTS:
 *  Tcl_Interp *interp - for errors
 *  rgy_policy_info_t *policy - this contains the list to be parsed and 
 *      the resultant attribute values. 
 *
 * RETURNS:
 *  TCL_OK on success. 
 */
int parse_registry_prop_list(Tcl_Interp *interp,
                             rgy_policy_info_t *policy)
{
    boolean32  list_list = FALSE;
    int        i = 0;
    char       **pargv;
    int        pargc;
    char       **aargv;
    int        aargc;

    if (!policy->list)
        return TCL_OK;
    list_list = isllist(policy->list);
    if (list_list) {
        if (Tcl_SplitList(interp, policy->list, &pargc, &pargv) != TCL_OK)
            return TCL_ERROR;
    }
    else {
        pargc = 1;
        pargv = (char **)malloc(2 * sizeof(char *));
        if (pargv == NULL) {
            DCP_SET_MISC_ERROR(dcp_s_no_memory);
        }
        pargv[0] = policy->list;
        pargv[1] = NULL;
    }
    for(; i < pargc; i++ ){
        if (Tcl_SplitList(interp, pargv[i], &aargc, &aargv) != TCL_OK) {
            free((char *)pargv);
            return TCL_ERROR;
        }
        if (aargc < 2 ) {
            free((char *)pargv);
            DCP_SET_MISC_ERROR(dcp_s_rgy_bad_attribute_list);
        }
        if (aargc > 2) {
            free((char *)pargv);
            free((char *)aargv);
            DCP_SET_MISC_ERROR(dcp_s_rgy_bad_attribute_value);
        }

        switch (rgy_attribute(aargv[0])) {
            case rgy_deftktlife:
                policy->deftktlife = dce_strdup(aargv[1]);
                break;
            case rgy_mingid:
                policy->mingid = dce_strdup(aargv[1]);
                break;
            case rgy_minorgid:
                policy->minorgid = dce_strdup(aargv[1]);
                break;
            case rgy_minuid:
                policy->minuid = dce_strdup(aargv[1]);
                break;
            case rgy_maxuid:
                policy->maxuid = dce_strdup(aargv[1]);
                break;
            case rgy_mintktlife:
                policy->mintktlife = dce_strdup(aargv[1]);
                break;
            case rgy_hidepwd:
                policy->hidepwd = dce_strdup(aargv[1]);
                break;
            case rgy_acct_plcy_maxtktlife:
                policy->maxtktlife = dce_strdup(aargv[1]);
                break;
            case rgy_acct_plcy_maxtktrenew:
                policy->maxtktrenew = dce_strdup(aargv[1]);
                break;
            case rgy_plcy_acctlife:
                policy->acctlife = dce_strdup(aargv[1]);
                break;
            case rgy_plcy_pwdlife:
                policy->pwdlife = dce_strdup(aargv[1]);
                break;
            case rgy_plcy_pwdminlen:
                policy->pwdminlen = dce_strdup(aargv[1]);
                break;
            case rgy_plcy_pwdexpdate:
                policy->expdate = dce_strdup(aargv[1]);
                break;
            case rgy_plcy_pwdspaces:
                policy->pwdspaces = dce_strdup(aargv[1]);
                break;
            case rgy_plcy_pwdalpha:
                policy->pwdalpha = dce_strdup(aargv[1]);
                break;
	    case rgy_plcy_version:
		policy->version = dce_strdup(aargv[1]);
		break;
            default:
                free((char *)pargv);
                DCP_SET_SEC_ERROR(dcp_s_rgy_illegal_attr,aargv[0]);
        }
        free((char *)aargv);
    }
    free((char *)pargv);
    return TCL_OK;
}


/*
 * FUNCTION: init_rgy_attribute_table
 *
 * OVERVIEW: This function initializes the attribute table for the 
 *    pgo account and registry objects. 
 * 
 * INPUT/OUTPUTS:
 *  Tcl_Interp *interp
 *
 * RETURNS:
 *  void
 */
void init_rgy_attribute_table(Tcl_Interp *interp)
{

    int     attribute_index, i;
    char    command[1024];
    extern  Tcl_Interp  *attr_interp;

    attribute_index = sizeof rgy_attr / sizeof rgy_attr[0];
    for (i = 0; i < attribute_index; i++) {
        sprintf(command,"set %s %d",rgy_attr[i].attribute_name,
                rgy_attr[i].index);
        Tcl_Eval(attr_interp,command);
    }
}


/*
 * FUNCTION: free_policy_attr
 *
 * OVERVIEW: This function frees the char * pointers in the rgy_policy_info_t 
 *       structure.
 *
 * INPUTS:
 *  rgy_policy_info_t *policy pointer to the policy structure.
 *
 * RETURNS:
 *  void
 */
void free_policy_attr(rgy_policy_info_t *policy)
{
    if(policy->deftktlife)
        free(policy->deftktlife);
    if(policy->mingid)
        free(policy->mingid);
    if(policy->minorgid)
        free(policy->minorgid);
    if(policy->minuid)
        free(policy->minuid);
    if(policy->maxuid)
        free(policy->maxuid);
    if(policy->mintktlife)
        free(policy->mintktlife);
    if(policy->version)
        free(policy->version);
    if(policy->hidepwd)
        free(policy->hidepwd);
    if(policy->acctlife)
        free(policy->acctlife);
    if(policy->expdate)
        free(policy->expdate);
    if(policy->maxtktlife)
        free(policy->maxtktlife);
    if(policy->maxtktrenew)
        free(policy->maxtktrenew);
    if(policy->pwdlife)
        free(policy->pwdlife);
    if(policy->pwdminlen)
        free(policy->pwdminlen);
    if(policy->pwdspaces)
        free(policy->pwdspaces);
    if(policy->pwdalpha)
        free(policy->pwdalpha);
}


/*
 * FUNCTION: dcp_rgy_checkpt_parse_cpi_time
 *
 * OVERVIEW: This function will parse a string in the following
 *	 format into integer of seconds
 *	 <int>		: number of seconds, e.g. 3600
 *	 <int>m		: number of minutes, e.g. 60m
 *	 <int>h		: number of hours, e.g. 1h
 * 
 *
 * INPUTS:
 *  A string of the format described above
 *
 * INPUT/OUTPUTS:
 *  Tcl_Interp *interp - for error reporting
 *
 * OUTPUTS:
 *  unsigned32		new_interval  
 *
 * RETURNS:
 *  TCL_OK on success
 */
int dcp_rgy_checkpt_parse_cpi_time(Tcl_Interp *interp,
                                   char *cpi_time_str,
                                   unsigned32 *new_interval_p)
{
    char  *time_str;
    int   i;
 
    if ( (time_str=strchr(cpi_time_str, 'm')) ) {
	if ( *(time_str+1) != '\0') {
	/* m should be the last character */
	    return(TCL_ERROR);
	}
    	*time_str = '\0';
    	if ( sscanf(cpi_time_str, "%lu", &i) == 0 ) {
	    return(TCL_ERROR);
        }
	*new_interval_p = i * 60;
    } else 
    if ( (time_str=strchr(cpi_time_str, 'h')) ) {
	if ( *(time_str+1) != '\0') {
	/* h should be the last character */
	    return(TCL_ERROR);
	}
    	*time_str = '\0';
    	if ( sscanf(cpi_time_str, "%lu", &i) == 0 ) {
	    return(TCL_ERROR);
        }
	*new_interval_p = i * 3600;
    } else {
	/* check if the string is digits */
	time_str = cpi_time_str;
	while ( *time_str != '\0') {
	    if ( isdigit(*time_str) == 0 ) {
		return(TCL_ERROR);
	    }
	    time_str++;
	}
    	if ( sscanf(cpi_time_str, "%lu", &i) == 0 ) {
	    return(TCL_ERROR);
        }
	*new_interval_p = i;
    }
    return(TCL_OK);
}


/*
 * FUNCTION: dcp_id_parse_name
 *
 * OVERVIEW: This function will parse a name into individual components -
 *       the cell name and a residual name. The function only binds to the 
 *       local cell because the local registry will parse a name for a foriegn
 *       cell as well.
 *
 * INPUTS:
 *  sec_rgy_name_t global - name to parse
 *
 * INPUT/OUTPUTS:
 *  Tcl_Interp *interp - for error reporting
 *
 * OUTPUTS:
 *  sec_rgy_name_t cell     - cell name
 *  sec_rgy_name_t residual - residual name
 *
 * RETURNS:
 *  TCL_OK on success
 */
int dcp_id_parse_name(Tcl_Interp *interp,
                      sec_rgy_name_t global,
                      sec_rgy_name_t cell,
                      sec_rgy_name_t residual)
{
    unsigned32 status;

    if (!site_cache[default_cache].valid) {
        status = open_rgy_site(interp,
                               (unsigned char *)LOCAL_CELL,
                               site_default);
        DCP_CHECK_SEC_ERROR(status);
    }

    do {
        if (status == sec_rgy_server_unavailable) {
            site_cache[default_cache].valid = false;
            status = open_rgy_site(interp,
                                   (unsigned char *)LOCAL_CELL,
                                   site_default);
            DCP_CHECK_SEC_ERROR(status);
        }
        sec_id_parse_name(rgy_default_context,
                          global,
                          cell,
                          NULL,
                          residual,
                          NULL,
                          &status);

    } while (status == sec_rgy_server_unavailable);

    DCP_CHECK_SEC_ERROR(status);
    return TCL_OK;
}


/*
 * FUNCTION: add_new_group_org
 *
 * OVERVIEW: This function adds a new group or organization if necessary 
 *      and adds the designated principal as a member of that group or
 *      organization. 
 * 
 *
 * INPUTS:
 *  sec_rgy_handle_t     registry_handle - registry site handle operate on.
 *  sec_rgy_login_name_t name - pgo defining necessary princ, group and org
 *  sec_rgy_domain_t     domain - group or organization to operate on. 
 * 
 * INPUT/OUTPUTS:
 *  Tcl_Interp *interp - error handling
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK on success.
 */
int add_new_group_org(sec_rgy_handle_t registry_handle,
                      sec_rgy_login_name_t name,
                      sec_rgy_domain_t domain,
                      boolean32 *created, 
                      error_status_t *result)

{
    sec_rgy_pgo_item_t  pgo_item;

    memset((char *)&pgo_item,0,sizeof(pgo_item));
    pgo_item.unix_num = -1;

    /* quota for a group is always -1 */
    pgo_item.quota = -1;

    /*
     * Attempt to add the group/organization to the specified site. 
     * If this fails, because the name already exists that is not an
     * error. 
     */
    sec_rgy_pgo_add(registry_handle,
                    domain,
                    name.gname,
                    &pgo_item,
                    result);

    if(*result != sec_rgy_name_exists &&
       *result != error_status_ok) {
        return *result;
    } else if (*result != sec_rgy_name_exists) {
        *created = TRUE;
    } else 
        *created = FALSE;

    /* Add the principal to the group/organization. */
    sec_rgy_pgo_add_member(registry_handle,
                           domain,
                           name.gname,
                           name.pname,
                           result);
    return *result;
}


/*
 * FUNCTION: add_cell_account
 *
 * OVERVIEW: This function adds an account specificly for inter-cell 
 *     communications. The account is not a valid login account and 
 *     the password is a rand key. 
 * 
 *
 * INPUTS:
 *  sec_rgy_handle_t registry_handle - handle for site to modify
 *  sec_rgy_login_name_t name - pgo name of account to add.
 *  sec_passwd_rec_t admin_pwd - password for individual adding account
 *  sec_passwd_rec_t *acct_pwd - random key passwd
 *
 * INPUT/OUTPUTS:
 *  Tcl_Interp *interp - for error handling
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK on success. 
 */

int add_cell_account(sec_rgy_handle_t registry_handle,
                    sec_rgy_login_name_t name,
                    sec_passwd_rec_t admin_pwd,
                    sec_passwd_rec_t *acct_pwd)
{
    sec_rgy_acct_key_t    acct_key;
    sec_rgy_acct_user_t   acct_user;
    sec_rgy_acct_admin_t  acct_admin;
    error_status_t        result;
    sec_passwd_version_t  version_number;
    boolean32             org_created = FALSE;
    boolean32             group_created = FALSE;
    error_status_t        tmp_result;

    memset((char *)&acct_key,0,sizeof(acct_key));
    memset((char *)&acct_user,0,sizeof(acct_user));
    memset((char *)&acct_admin,0,sizeof(acct_admin));

    /* Set up the account information for the new account. */

    acct_key = sec_rgy_acct_key_person;
    acct_user.flags = sec_rgy_acct_user_passwd_valid;
    acct_user.gecos[0] = '\0';
    acct_user.homedir[0] = '\0';
    acct_user.shell[0] = '\0';
    acct_user.passwd[0] = '\0';
    acct_admin.good_since_date = get_current_time();
    acct_admin.expiration_date = 0;
    acct_admin.flags |= sec_rgy_acct_admin_server;
    acct_admin.authentication_flags |= sec_rgy_acct_auth_forwardable;
    acct_admin.authentication_flags |= sec_rgy_acct_auth_tgt;
    acct_admin.authentication_flags |= sec_rgy_acct_auth_renewable; 
    acct_admin.authentication_flags |= sec_rgy_acct_auth_post_dated;
    acct_admin.authentication_flags |= sec_rgy_acct_auth_proxiable;
    acct_admin.authentication_flags |= sec_rgy_acct_auth_dup_skey;
    admin_pwd.version_number = 0;
    admin_pwd.key.key_type = sec_passwd_plain;

    /* 
     * Add the new group and organization. The function also sets the 
     * associated principal as a member. Return a flag that states 
     * whether or not the group/org was actually created. This is 
     * in case of a failure. 
     */
    if(add_new_group_org(registry_handle,
                         name,
                         sec_rgy_domain_group,
                         &group_created,
                         &result) != error_status_ok){
        if(group_created)
            sec_rgy_pgo_delete(registry_handle,
                               sec_rgy_domain_group,
                               name.gname,
                               &tmp_result);
        return result;
    }

    if(add_new_group_org(registry_handle,
                         name,
                         sec_rgy_domain_org,
                         &org_created,
                         &result) != error_status_ok){
        if(org_created)
            sec_rgy_pgo_delete(registry_handle,
                               sec_rgy_domain_org,
                               name.oname,
                               &tmp_result);
        if(group_created)
            sec_rgy_pgo_delete(registry_handle,
                               sec_rgy_domain_group,
                               name.gname,
                               &tmp_result);
        return result;
    }

    /*
     * Add the new account. In case of a failure clean up the work 
     * already done. 
     */
    sec_rgy_acct_add(registry_handle,
                     &name,
                     &acct_key,
                     &acct_user,
                     &acct_admin,
                     &admin_pwd,
                     acct_pwd,
                     sec_passwd_des,
                     &version_number,
                     &result);
    if(result != error_status_ok){
        if(group_created)
            sec_rgy_pgo_delete(registry_handle,
                               sec_rgy_domain_group,
                               name.gname,
                               &tmp_result);
        if(org_created)
            sec_rgy_pgo_delete(registry_handle,
                               sec_rgy_domain_org,
                               name.oname,
                               &tmp_result);
    }
    return result;
}


/*
 * FUNCTION: replace_cell_account
 *
 * OVERVIEW: This function replaces a cell account specificly for inter-cell 
 *     communications. The account is not a valid login account and 
 *     the password is a rand key. 
 * 
 *
 * INPUTS:
 *  sec_rgy_handle_t registry_handle - handle for site to modify
 *  sec_rgy_login_name_t name - pgo name of account to add.
 *  sec_passwd_rec_t admin_pwd - password for individual adding account
 *  sec_passwd_rec_t *acct_pwd - random key passwd
 *
 * INPUT/OUTPUTS:
 *  Tcl_Interp *interp - for error handling
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK on success. 
 */
int replace_cell_account(sec_rgy_handle_t registry_handle,
                         sec_rgy_login_name_t name,
                         sec_passwd_rec_t admin_pwd,
                         sec_passwd_rec_t *acct_pwd)
{
    sec_rgy_acct_key_t   acct_key;
    sec_rgy_acct_user_t  acct_user;
    sec_rgy_acct_admin_t acct_admin;
    sec_rgy_cursor_t     marker;
    sec_rgy_login_name_t tmp_name, old_name;
    error_status_t       result;
    sec_passwd_version_t version_number;
    boolean32            group_created = FALSE;
    boolean32            org_created = FALSE;
    error_status_t       tmp_result;
    sec_rgy_acct_key_t   domain = sec_rgy_acct_key_person;

    memset((char *)&acct_key,0,sizeof(acct_key));
    memset((char *)&acct_user,0,sizeof(acct_user));
    memset((char *)&acct_admin,0,sizeof(acct_admin));

    strcpy((char *)tmp_name.pname, (char *)name.pname);
    tmp_name.gname[0] = '\0';
    tmp_name.oname[0] = '\0';

    /* lookup the original entry to see if the group or org has changed. */

    sec_rgy_cursor_reset(&marker);
    sec_rgy_acct_lookup(registry_handle,
                        &tmp_name,
                        &marker,
                        &old_name,
                        NULL,NULL,NULL,NULL,NULL,&result);

    /*
     * If the group or org has changed add the principal to the new 
     * group or new org. Keep track of whether or not a new 
     * group/org was created. In case of a failure we need to nuke them.
     */

    if(result == error_status_ok){
        if(strcmp((char *)name.gname,(char *)old_name.gname) ||
           strcmp((char *)name.oname,(char *)old_name.oname)){
            if(add_new_group_org(registry_handle,
                                 name,
                                 sec_rgy_domain_group,
                                 &group_created,
                                 &result) != error_status_ok)
                return result;
        
            if(add_new_group_org(registry_handle,
                                 name,
                                 sec_rgy_domain_org,
                                 &org_created,
                                 &result) != error_status_ok)
                return result;
            /*
             * If a new group/org exists for the account we need
             * to rename the account with the appropriate info.
             */
            sec_rgy_acct_rename(registry_handle,
                                &old_name,
                                &name,
                                &domain,
                                &result);
            if(result != error_status_ok){
                if(org_created)
                    sec_rgy_pgo_delete(registry_handle,
                                       sec_rgy_domain_org,
                                       name.oname,
                                       &tmp_result);
                if(group_created)
                    sec_rgy_pgo_delete(registry_handle,
                                       sec_rgy_domain_group,
                                       name.gname,
                                       &tmp_result);
                return result;
            }
        }
    }
               
    /* 
     * Set up the information so that the account can be updated with the
     * correct new password. 
     */

    acct_key = sec_rgy_acct_key_person;
    acct_user.flags = sec_rgy_acct_user_passwd_valid;
    acct_user.gecos[0] = '\0';
    acct_user.homedir[0] = '\0';
    acct_user.shell[0] = '\0';
    acct_user.passwd[0] = '\0';
    admin_pwd.version_number = 0;
    admin_pwd.key.key_type = sec_passwd_plain;

    sec_rgy_acct_user_replace(registry_handle,
                              &name,
                              &acct_user,
                              TRUE,
                              &admin_pwd,
                              acct_pwd,
                              sec_passwd_des,
                              &version_number,
                              &result);
    return result;
}
