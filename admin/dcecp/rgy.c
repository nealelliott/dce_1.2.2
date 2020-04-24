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
 * $Log: rgy.c,v $
 * Revision 1.1.11.1  1996/08/09  11:48:36  arvind
 * 	Merge changes.
 * 	[1996/06/11  17:06 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/2]
 *
 * 	Minor changes.
 * 	[1996/06/11  15:53 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/1]
 *
 * 	Merge with latest HP fixes and enhancements.
 * 	[1996/03/28  18:52 UTC  truitt  /main/HPDCE02/15]
 *
 * 	Merge changes.
 * 	[1996/03/28  18:50 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf72/1]
 *
 * 	CHFts17168: The account, pgo, registry and schema calls all have serious
 * 	problems when called with an argument of " ".  Note they work correctly if the
 * 	argument is {" "}.  Trap the first type of argument as illegal.
 * 	[1996/03/20  21:49 UTC  truitt  /main/HPDCE02/14]
 *
 * 	Merge changes.
 * 	[1996/03/20  21:47 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf70/1]
 *
 * 	CHFts17733: Some complication with registry cursors that currently cannot
 * 	be fixed is causing problems for dcecp in some rare occasions.  In order to
 * 	work around this problem until registry cursors are completely fixed, we'll watch
 * 	the output from the api calls that use the rgy cursor and make sure we trap
 * 	and handle the correct errors.
 * 	[1996/03/18  18:30 UTC  truitt  /main/HPDCE02/13]
 *
 * 	Merge changes.
 * 	[1996/03/18  18:29 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf69/1]
 *
 * 	CHFts17225: Modify some on-line help messages to match current functionality.
 * 	CHFts17407: Condense some of the sam messages that are duplicates.
 * 	[1996/02/15  18:36 UTC  truitt  /main/HPDCE02/12]
 *
 * 	Merge brief changes.
 * 	[1996/02/15  18:34 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf62/1]
 *
 * 	CHFts17451: Remove include of value.h so duplicate macro
 * 	warning does not appear during HP build.  The include is
 * 	required on other platforms.
 * 	[1996/02/06  21:23 UTC  truitt  /main/HPDCE02/11]
 *
 * 	Merge changes.
 * 	[1996/02/06  21:21 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf60/1]
 *
 * 	CHFts16677: Make sure the correct error message is displayed
 * 	when an invalid cellname is passed to a 'catalog' operation.
 * 	[1996/02/05  16:14 UTC  truitt  /main/HPDCE02/10]
 *
 * 	Merge changes.
 * 	[1996/02/05  16:02 UTC  truitt  /main/HPDCE02/truitt_dcecp_17281/1]
 *
 * 	CHFts17281: The calls to the internal parse_name function accept
 * 	a global name and return the cell name and the residual name, derived
 * 	from the global name.  Many places where this function is called reset
 * 	the returned cellname to '\0' just before the open site call.  We removed
 * 	the code that resets the cellname so the cell name returned from the
 * 	parse name function is used in the open calls.
 *
 * 	Initial DCE_1.2.2 branch.
 * 	[1996/06/11  15:37 UTC  truitt  /main/DCE_1.2.2/1]
 *
 * Revision 1.1.9.2  1996/02/18  19:14:08  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:00:31  marty]
 * 
 * Revision 1.1.9.1  1995/12/08  21:21:55  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/9  1995/06/16  18:16 UTC  truitt
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts35/1  1995/06/16  18:15 UTC  truitt
 * 	CHFts15504: Remove unneeded includes.
 * 
 * 	HP revision /main/HPDCE02/8  1995/05/01  19:00 UTC  truitt
 * 	Merge era work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts26/1  1995/05/01  18:59 UTC  truitt
 * 	CHFts14301: Allocate memory for the sec_attr_t structures
 * 	dynamically.
 * 
 * 	HP revision /main/HPDCE02/7  1995/04/05  20:12 UTC  truitt
 * 	Merge message work.
 * 
 * 	HP revision /main/HPDCE02/truitt_mothra_chfts24/1  1995/04/05  20:11 UTC  truitt
 * 	CHFts14652: Add improved error message.
 * 
 * 	HP revision /main/HPDCE02/6  1995/03/09  14:36 UTC  truitt
 * 	Check for valid cellname in catalog command.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts18/1  1995/03/09  14:35 UTC  truitt
 * 	CHFts14388: The catalog action was accepting a bogus value
 * 	as the registry to contact.
 * 
 * 	HP revision /main/HPDCE02/5  1995/03/02  18:44 UTC  truitt
 * 	Merge init changes.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts17/1  1995/03/02  18:42 UTC  truitt
 * 	CHFts14504: Initialize attr_info structure correctly.
 * 
 * 	HP revision /main/HPDCE02/4  1995/02/14  16:26 UTC  truitt
 * 	Merge _e work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts13/1  1995/02/14  16:07 UTC  truitt
 * 	CHFts14285: Replace direct set of error with call
 * 	to correct macro.
 * 
 * 	HP revision /main/HPDCE02/3  1995/02/10  19:27 UTC  truitt
 * 	Merge _e work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts11/1  1995/02/10  19:26 UTC  truitt
 * 	CHFts14285: Make sure the error code convenience
 * 	variable gets set when an error occurs.
 * 
 * 	HP revision /main/HPDCE02/2  1995/01/26  18:01 UTC  truitt
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts05/1  1995/01/26  17:59 UTC  truitt
 * 	CHFts14004: Make sure the name_get() function displays
 * 	the hostname correctly.
 * 
 * 	HP revision /main/HPDCE02/1  1994/11/03  20:32 UTC  truitt
 * 	Initial HPDCE02 branch.
 * 
 * 	HP revision /main/truitt_dcecp_01/1  1994/11/03  20:30 UTC  truitt
 * 	Fix principal command switches.
 * 	[1995/12/08  17:56:23  root]
 * 
 * Revision 1.1.4.71  1994/10/06  15:17:59  rousseau
 * 	Change use of status var (CR 12539).
 * 	[1994/10/06  15:17:41  rousseau]
 * 
 * Revision 1.1.4.70  1994/10/05  18:42:47  truitt
 * 	OT12492: Do not free output structures when call to
 * 	check_era_info() function fails.
 * 	[1994/10/05  18:35:14  truitt]
 * 
 * Revision 1.1.4.69  1994/09/30  15:26:19  truitt
 * 	OT12433: Properly chech the show_era() return status.
 * 	[1994/09/30  15:04:13  truitt]
 * 
 * Revision 1.1.4.68  1994/09/29  18:45:08  truitt
 * 	OT10967: Fix the display of multiple values per attribute.
 * 	Also, cleaned up some of the 'free' code in the necessary
 * 	files.
 * 	[1994/09/29  17:29:22  truitt]
 * 
 * Revision 1.1.4.67  1994/09/23  18:31:01  ajayb
 * 	Fixed several memory leaks.
 * 	[1994/09/22  20:38:48  ajayb]
 * 
 * Revision 1.1.4.66  1994/09/16  15:34:19  ajayb
 * 	principal_show(): Don't print leading newline before
 * 	ERAs if that is the only thing being displayed.
 * 	[1994/09/15  17:59:16  ajayb]
 * 
 * Revision 1.1.4.65  1994/09/14  15:10:02  kevins
 * 	OT 12175 - The code now continues onto the next registry instead
 * 	   of returning. Also the result code is reset prior to
 * 	   accessing the registry in account and pgo catalog
 * 	   commands. The new method for allocating the output
 * 	   buffer needed one more byte. The output buffer is
 * 	   allocated based on the size of the cell_name and
 * 	   the maximum size of the name allowed by the registry.
 * 	   It needed one more byte though to accommodate the
 * 	   '/' that goes between them. I also included an
 * 	   empty line between multiple registry outputs.
 * 	[1994/09/14  15:05:58  kevins]
 * 
 * Revision 1.1.4.64  1994/09/13  22:38:51  rousseau
 * 	Catalog performance changes.
 * 	[1994/09/13  22:37:32  rousseau]
 * 
 * Revision 1.1.4.63  1994/09/13  19:12:31  kevins
 * 	OT 11641 list syntax not supported in catalog commands.
 * 	[1994/09/13  18:46:29  kevins]
 * 
 * Revision 1.1.4.62  1994/09/12  17:57:53  kevins
 * 	11968 Modified the registry opening process to aquire the
 * 	      rgy site handle with authentication that corresponds
 * 	      to the users login context.
 * 	[1994/09/12  13:42:29  kevins]
 * 
 * Revision 1.1.4.61  1994/09/06  16:19:48  kevins
 * 	OT 11694 broke ip rgy.c into rgy_pgo_group.c and rgy_pco_account.c
 * 	[1994/09/06  16:18:33  kevins]
 * 
 * Revision 1.1.4.60  1994/09/01  19:04:44  truitt
 * 	move schema commands to separate files
 * 	[1994/09/01  18:08:39  truitt]
 * 
 * Revision 1.1.4.59  1994/09/01  13:12:58  kevins
 * 	OT 11973 now checks for maxtktrenew when creating account.
 * 	[1994/09/01  13:09:32  kevins]
 * 
 * Revision 1.1.4.58  1994/08/31  18:25:02  kevins
 * 	Modified the check from sec_rgy_auth_plcy_get_info in rgy_account_modify.
 * 	[1994/08/31  18:24:22  kevins]
 * 
 * Revision 1.1.4.57  1994/08/31  13:04:04  truitt
 * 	Merged with changes from 1.1.4.56
 * 	[1994/08/31  13:03:51  truitt]
 * 
 * 	OT11095: Allow proper removal of extended attributes.
 * 	[1994/08/30  20:50:05  truitt]
 * 
 * Revision 1.1.4.56  1994/08/30  17:22:33  melman
 * 	merge up
 * 	[1994/08/30  17:21:59  melman]
 * 
 * 	alphabetized the arg_tables
 * 	[1994/08/30  00:24:26  melman]
 * 
 * Revision 1.1.4.55  1994/08/30  15:18:56  kevins
 * 	OT 11597 uid errors are now reported correctly
 * 	[1994/08/30  15:16:55  kevins]
 * 
 * Revision 1.1.4.54  1994/08/29  14:20:55  ajayb
 * 	Merged with changes from 1.1.4.53
 * 	[1994/08/29  14:19:52  ajayb]
 * 
 * 	Add rgy_account_generate().
 * 	Change rgy_account_create() and rgy_account_modify()
 * 	now that password generation is done in
 * 	rgy_account_generate().
 * 	[1994/08/26  20:35:46  ajayb]
 * 
 * Revision 1.1.4.53  1994/08/26  18:12:08  rousseau
 * 	Changed sec_login_get_current_context() calls to use local context cache (CR 11848).
 * 	[1994/08/26  18:11:52  rousseau]
 * 
 * Revision 1.1.4.52  1994/08/25  18:14:08  nav
 * 	Fixed the merge statements.
 * 	[1994/08/25  18:13:15  nav]
 * 
 * Revision 1.1.4.51  1994/08/25  15:52:48  nav
 * 	Merged with changes from 1.1.4.50
 * 	[1994/08/25  15:52:23  nav]
 * 
 * 	Added support for generic schema.
 * 	~
 * 	[1994/08/25  15:43:02  nav]
 * 
 * Revision 1.1.4.50  1994/08/24  20:23:56  kevins
 * 	Merged with changes from 1.1.4.49
 * 	[1994/08/24  20:22:56  kevins]
 * 
 * 	OT 11820 Intercell communications are now consistent.
 * 	[1994/08/24  17:53:46  kevins]
 * 
 * Revision 1.1.4.49  1994/08/24  18:51:33  truitt
 * 	OT11709: Support attributes of type void correctly.
 * 	         Also moved some 'free' statements outside
 * 	         a 'for' loop in rgy_group_show().
 * 	[1994/08/24  18:46:30  truitt]
 * 
 * Revision 1.1.4.48  1994/08/23  21:20:30  kevins
 * 	Merged with changes from 1.1.4.47
 * 	[1994/08/23  21:19:30  kevins]
 * 
 * 	OT 11641 Added _n support for catalog commands
 * 	[1994/08/23  20:55:23  kevins]
 * 
 * Revision 1.1.4.47  1994/08/22  19:44:22  ajayb
 * 	Merged with changes from 1.1.4.46
 * 	[1994/08/22  19:43:47  ajayb]
 * 
 * 	schema_create/modify/rename/delete():
 * 
 * 	Explicitly bind to the master rgy before
 * 	doing the operation.
 * 	[1994/08/22  01:13:21  ajayb]
 * 
 * Revision 1.1.4.46  1994/08/22  15:29:12  coren
 * 	Made policy attributes work with -attribute for org create. (CR 11671)
 * 	[1994/08/19  22:05:16  coren]
 * 
 * Revision 1.1.4.45  1994/08/19  18:14:27  truitt
 * 	Merged with changes from 1.1.4.44
 * 	[1994/08/19  18:11:02  truitt]
 * 
 * 	Added missing security utilities include file.
 * 	[1994/08/19  16:13:21  truitt]
 * 
 * Revision 1.1.4.44  1994/08/19  16:55:30  ajayb
 * 	Fixed memory leaks in rgy_account_create/modify().
 * 	Memory obtained from parse_account_attributes()
 * 	was not being released from these functions.
 * 	[1994/08/18  21:46:42  ajayb]
 * 
 * Revision 1.1.4.43  1994/08/18  13:55:47  ajayb
 * 	rgy_account_modify():
 * 
 * 	Do password generation and pwd_val_type ERA checks only if
 * 	either -password or -randompwd was specified in the account
 * 	modify command.
 * 
 * 	Set passwd_valid to 1 in sec_rgy_acct_replace_all() call only
 * 	if we are changing the password.
 * 	[1994/08/17  18:07:54  ajayb]
 * 
 * Revision 1.1.4.42  1994/08/17  14:30:49  truitt
 * 	OT11691: Added support for -simplename option to the xattrschema
 * 	         catalog command.
 * 	[1994/08/16  21:05:17  truitt]
 * 
 * Revision 1.1.4.41  1994/08/15  18:19:13  ajayb
 * 	rgy_account_create/modify():
 * 
 * 	- Match the sequence for initializing arg_table[].dst with the rest
 * 	  of the fields in the table. A mismatch in this sequence was causing
 * 	  dcecp to coredump when using the random password generation
 * 	  functionality.
 * 
 * 	- Use 'user-select' semantics for a pwd_val_type of 'none', i.e.
 * 	  pre-1.1 behavior.
 * 
 * 	- Return an error if the pwd_val_type is of an unrecognized type.
 * 	[1994/08/15  14:39:51  ajayb]
 * 
 * Revision 1.1.4.40  1994/08/10  21:17:09  jane
 * 	Merged with changes from 1.1.4.39
 * 	[1994/08/10  21:16:47  jane]
 * 
 * 	added password generation functionality
 * 	[1994/08/10  20:58:17  jane]
 * 
 * Revision 1.1.4.39  1994/08/08  21:04:36  melman
 * 	put include dce_utils.h in dcecp.h
 * 	[1994/08/08  21:01:54  melman]
 * 
 * Revision 1.1.4.38  1994/08/08  18:51:52  melman
 * 	Merged with changes from 1.1.4.37
 * 	[1994/08/08  18:51:12  melman]
 * 
 * 	change strdup to dce_strdup
 * 	[1994/08/08  17:52:23  melman]
 * 
 * Revision 1.1.4.37  1994/08/08  18:00:46  kevins
 * 	        11071 - Reworded the rgy_handle cache so that we now
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
 * 
 * 	        11368 - pgo/account now take as an argument the name of the
 * 	        cell to catalog.
 * 
 * 	        11535 - The cellname obtained for the catalog commands is now
 * 	        the current cell that the user has opened. (ie the cell that
 * 	[1994/08/08  15:37:38  kevins]
 * 
 * Revision 1.1.4.36  1994/08/03  21:43:17  kevins
 * 	OT 9691 name parse should open default cell
 * 	[1994/08/03  21:36:05  kevins]
 * 
 * Revision 1.1.4.35  1994/07/29  20:37:17  kevins
 * 	OT 11451 mad 64 bit changes requested by a_ferris
 * 	[1994/07/29  20:19:09  kevins]
 * 
 * Revision 1.1.4.34  1994/07/26  18:54:12  kevins
 * 	Merged with changes from 1.1.4.33
 * 	[1994/07/26  18:53:50  kevins]
 * 
 * 	OT 9691 Added the 'name get' command
 * 	[1994/07/26  18:51:19  kevins]
 * 
 * Revision 1.1.4.33  1994/07/25  15:18:53  truitt
 * 	Change the default registry schema name.
 * 	[1994/07/25  14:07:22  truitt]
 * 
 * Revision 1.1.4.32  1994/07/20  20:36:12  nav
 * 	Added support for schema modify and schema rename.
 * 	[1994/07/20  19:27:01  nav]
 * 
 * Revision 1.1.4.31  1994/07/19  18:26:25  truitt
 * 	OT11186: Implement the rest of schema create.
 * 	OT9691: Implement schema delete.
 * 	[1994/07/19  17:30:05  truitt]
 * 
 * Revision 1.1.4.30  1994/07/14  18:34:02  truitt
 * 	Merged with changes from 1.1.4.29
 * 	[1994/07/14  18:33:44  truitt]
 * 
 * 	OT11186: Implement partial schema create
 * 	[1994/07/14  17:33:35  truitt]
 * 
 * Revision 1.1.4.29  1994/07/08  09:30:37  kevins
 * 	OT 11197 quota value perserved
 * 	[1994/07/07  20:38:52  kevins]
 * 
 * Revision 1.1.4.28  1994/07/07  15:18:30  truitt
 * 	OT11185: Implement schema catalog.
 * 	[1994/07/07  14:44:56  truitt]
 * 
 * Revision 1.1.4.27  1994/07/05  19:35:37  truitt
 * 	OT11162: Implement schema show
 * 	[1994/07/05  18:19:26  truitt]
 * 
 * Revision 1.1.4.26  1994/06/28  18:30:17  truitt
 * 	Fix bsubmit errors.
 * 	[1994/06/28  18:26:34  truitt]
 * 
 * Revision 1.1.4.25  1994/06/28  14:34:24  truitt
 * 	Merged with changes from 1.1.4.24
 * 	[1994/06/28  14:34:01  truitt]
 * 
 * 	OT11061: Support for pgo modify (all phases) of extended attributes.
 * 	The three major modify functions (principal, group, organization)
 * 	were all changed.
 * 	[1994/06/27  18:29:49  truitt]
 * 
 * Revision 1.1.4.24  1994/06/23  19:21:55  kevins
 * 	OT 9691 Added name parse command
 * 	[1994/06/23  18:43:20  kevins]
 * 
 * Revision 1.1.4.23  1994/06/21  14:57:42  kevins
 * 	OT 10916 modified group list for princ show command.
 * 	[1994/06/21  14:57:19  kevins]
 * 
 * Revision 1.1.4.22  1994/06/10  09:39:08  kevins
 * 	OT 10917 modify display output to provide extra line between objects
 * 	         in a list.
 * 	OT 10908 Change account modify api sequence.
 * 	OT 10916 Princ show now resets cursor correctly.
 * 	[1994/06/10  09:37:20  kevins]
 * 
 * Revision 1.1.4.21  1994/06/09  16:02:16  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:47:49  devsrc]
 * 
 * Revision 1.1.4.20  1994/06/07  19:16:35  kevins
 * 	OT 10784 modified integer evalutation functions
 * 	[1994/06/07  19:15:28  kevins]
 * 
 * Revision 1.1.4.19  1994/06/06  09:42:41  kevins
 * 	OT 10804 Pincipal show now displays group list
 * 	[1994/06/06  09:41:05  kevins]
 * 
 * Revision 1.1.4.18  1994/06/02  18:27:04  kevins
 * 	OT 10826 Fixed list processing in group/org add
 * 	[1994/06/02  17:23:10  kevins]
 * 
 * Revision 1.1.4.17  1994/06/02  15:55:13  salamone
 * 	Moved and renamed  rgy_modify() and rgy_show() functions in
 * 	rgy.c to rgy_sa_tclif.c
 * 	[1994/06/02  15:45:50  salamone]
 * 
 * Revision 1.1.4.16  1994/06/02  13:56:43  truitt
 * 	Merged with changes from 1.1.4.15
 * 	[1994/06/02  13:56:19  truitt]
 * 
 * 	OT 10800: Initial work for extended registry attribute
 * 	support.  Added support for principal/group/org
 * 	create/modify/show for all encoding types except
 * 	binding.  Actually, you can show binding types, but
 * 	you cannot enter them via create or modify yet.
 * 	[1994/05/27  18:36:05  truitt]
 * 
 * Revision 1.1.4.15  1994/05/27  13:31:10  kevins
 * 	OT 10783 Free arguments after calling TCL_AppendResult
 * 	[1994/05/27  13:30:52  kevins]
 * 
 * Revision 1.1.4.14  1994/05/20  11:26:25  kevins
 * 	10635 - Modified the routine that opens the registry site to allow
 * 		unauthenticated access.
 * 	10033 - Added the ability to add pgo/account objects via their
 * 		full path name.
 * 	[1994/05/20  08:57:06  kevins]
 * 
 * Revision 1.1.4.13  1994/05/12  19:35:03  kevins
 * 	OT 10618 modified input/output of acctlife pwdlife and
 * 	     pwdexpdate for UTC dates
 * 	[1994/05/12  19:34:47  kevins]
 * 
 * Revision 1.1.4.12  1994/05/12  13:04:05  kevins
 * 	OT 10612 removed redefined macros.
 * 	[1994/05/12  11:37:56  kevins]
 * 
 * Revision 1.1.4.11  1994/05/11  19:13:55  kevins
 * 	OT 10595 Fixed usage of _s(sec) var
 * 	[1994/05/11  19:13:25  kevins]
 * 
 * 	OT 10097 modified arg checking
 * 	[1994/05/11  16:12:23  kevins]
 * 
 * Revision 1.1.4.10  1994/05/10  10:00:13  kevins
 * 	OT 9691 Add registry show/modify commands
 * 	[1994/05/09  19:15:55  kevins]
 * 
 * Revision 1.1.4.9  1994/04/08  10:40:11  kevins
 * 	OT 9691 Implemented account commands
 * 	[1994/04/08  10:39:27  kevins]
 * 
 * Revision 1.1.4.8  1994/03/21  23:52:22  kevins
 * 	CR 10097 modify commands should not be able to alter unix_num
 * 	[1994/03/21  23:51:11  kevins]
 * 
 * Revision 1.1.4.7  1994/03/18  14:23:59  kevins
 * 	CR 10097 added pgo_item init prior to encoding attributes
 * 	[1994/03/18  00:10:49  kevins]
 * 
 * Revision 1.1.4.6  1994/03/14  17:39:44  kevins
 * 	CR9904 Added support for help system
 * 	[1994/03/14  13:44:28  kevins]
 * 
 * Revision 1.1.4.5  1994/03/07  16:08:30  kevins
 * 	CR10026 reorder argument checking for pgo commands
 * 	[1994/03/07  16:07:44  kevins]
 * 
 * Revision 1.1.4.4  1994/02/03  17:24:15  kevins
 * 	added support for group and org objects
 * 	[1994/02/03  17:16:46  kevins]
 * 
 * Revision 1.1.4.3  1994/01/05  16:30:30  kevins
 * 	Added rgy group commads.
 * 	[1994/01/05  16:03:15  kevins]
 * 
 * Revision 1.1.4.2  1993/12/22  18:12:40  kevins
 * 	Message catalo cleanup
 * 	[1993/12/22  18:07:58  kevins]
 * 
 * Revision 1.1.4.1  1993/12/20  20:07:53  kevins
 * 	Fixed and cleaned messages
 * 	[1993/12/20  18:50:04  kevins]
 * 
 * Revision 1.1.2.5  1993/12/13  12:48:01  kevins
 * 	Completed principal commands
 * 	[1993/12/13  12:43:32  kevins]
 * 
 * Revision 1.1.2.3  1993/11/23  13:22:34  kevins
 * 	included principal delete/catalog commands
 * 	[1993/11/23  13:18:24  kevins]
 * 
 * Revision 1.1.2.1  1993/11/05  19:52:55  kevins
 * 	new file for sec commands
 * 	[1993/11/05  19:40:33  kevins]
 * 
 * $EndLog$
 */

/* INCLUDES */
#include <stdio.h>
#include <locale.h>
#include <errno.h>
/*
 * Don't inherit stubbase.h's use of the exception based pthrads API.
 */
#ifndef NCK
#  define NCK
#endif
#include <dce/rpc.h>
#include <dce/stubbase.h>

#ifndef SNI_SVR4_POSIX
#include <strings.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <dce/dce_error.h>
#include <sys/types.h>
#include <unistd.h>
#include <dce/dce.h>
#include <dce/binding.h>
#include <dce/pgo.h>
#include <dce/policy.h>
#include <dce/rgynbase.h>
#include <dce/dce_msg.h>
#include <dce/dcedcpmsg.h>
#include <dce/dce_svc.h>
#include <dce/misc.h>
#include <dcecp.h>
#include <dce/tcl.h>
#include <dce/acct.h>
#include <dce/utc.h>
#include <dce/secidmap.h>
#include <rgy.h>
#include <util_var.h>
#include <util_general.h>
#include <util_help.h>
#include <util_binding.h>
#include <values.h>

#include <dce/dced.h>
#include <dce/sec_attr_base.h>
#include <dce/sec_attr_util.h>
#include <dce/sec_attr_tools.h>
#include <dce/sec_rgy_attr.h>
#include <dce/sec_rgy_attr_sch.h>
#include <dce/sec_pwd_mgmt.h>
#include <dce/macros.h>
#include <dhd.h>
#include <util_login.h>

/*
 * This is necessary to find the current global login context. This
 * is defined in util_login.h 
 */
extern dcp_login_context_p_t global_context_list;
extern int skip_era_newline;


/*
 * FUNCTION: principal_create
 *
 * OVERVIEW:
 * 
 *
 * INPUTS:
 *  int argc - The number of command line arguments passed into function.
 *  char **argv - The actual arguements.
 *
 * INPUT/OUTPUTS:
 * Tcl_Interp *interp - Structure associated with the interpreter. This         
 *                      will hold the error message (if any) generated from the
 *                      command. This is also the location of any convienience
 *                      variables.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 *    
 */
int principal_create(ClientData clientData, 
                     Tcl_Interp *interp, 
                     int argc, 
                     char **argv)
{
    unsigned32            result = TCL_OK;
    char                  *alias = NULL;
    rgy_attribute_info_t  attr_info;
    char                  **cargv;
    int                   cargc = 0;
    char                  **jargv;
    int                   jargc = 0;
    char                  **targv;
    int                   targc = 0;
    int                   arg_count = 0;
    int                   i;
    boolean32             is_simple;
    sec_rgy_name_t        global_name;
    sec_rgy_name_t        cell_name;
    sec_rgy_name_t        princ_name;
    char                  *fullname = NULL;
    char                  *quota = NULL;
    char                  *uuid = NULL;
    char                  *attribute = NULL;
    char                  *unix_num = NULL;
    sec_rgy_pgo_item_t    pgo_item;
    sec_attr_t            *attrs = NULL;
    sec_attr_t            *out_attrs = NULL;
    unsigned32            num_returned = 0;
    unsigned32            num_left = 0;
    unsigned32            space_avail = 0;
    signed32              failure_index;

    dcp_ArgvInfo arg_table[] = {
        {"-alias", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_princ_alias_help},
        {"-attribute", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_princ_attr_help},
        {"-fullname", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_princ_fname_help},
        {"-quota", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_princ_quota_help},
        {"-uid", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_princ_uid_help},
        {"-uuid", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_princ_uuid_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    /*
     * Set the variables for the Parser. We need to cast them all as 
     * char pointers. 
     */
    memset((char *)&pgo_item,0,sizeof(pgo_item));
    pgo_item.unix_num = -1;
    arg_table[0].dst = (char *)&alias;
    arg_table[1].dst = (char *)&attribute;
    arg_table[2].dst = (char *)&fullname;
    arg_table[3].dst = (char *)&quota;
    arg_table[4].dst = (char *)&unix_num;
    arg_table[5].dst = (char *)&uuid;

    if ((result = dcp_ParseArgv(interp, &argc, argv, arg_table, 0)) == TCL_ERROR)
        return TCL_ERROR;

    /*
     * Verify that the user provided a principal name with the command. 
     * If so initialize the pgo name parameter.
     */
    DCP_CHECK_RGY_NO_ARG(princ);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*   
     * Check the argument list to verify the principal create conditions 
     * have been met. If the user is creating an alias then the uid must 
     * be set as the principal the user wishes to alias to.  Also if 
     * alias is set the uuid_no must not be specified as the principal 
     * uuid already exists. 
     */
    if (attribute != NULL && (alias != NULL || fullname  != NULL || 
        quota != NULL || uuid != NULL || unix_num != NULL)) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_list_and_switches);
    }

    /*
     * Set the initial flags and attribute.  By default, a person
     * can have a concurrent group set.
     */
    pgo_item.flags = 0;
    pgo_item.flags |= sec_rgy_pgo_projlist_ok;
    pgo_item.quota = -1;
    attr_info.domain = sec_rgy_domain_person;
    attr_info.policy = FALSE;
    attr_info.pgo = FALSE;
    attr_info.types = 0;
    attr_info.era_count = 0;
    attr_info.del_count = 0;
    attr_info.modify_type = modify_type_none;
    attr_info.flags = pgo_create;
    attr_info.pgo_item = &pgo_item;
    attr_info.fullname = NULL;

    parse_item_switch_list(quota, fullname, uuid, unix_num, alias, NULL,
                           &pgo_item, sec_rgy_domain_person, &attr_info,
                           &result);

    if (result == dcp_s_rgy_uid_out_of_range || result == dcp_s_rgy_quota_out_of_range) {
        DCP_SET_SEC_ERROR(result, MAXINT);
    }
    DCP_CHECK_SEC_ERROR(result);

    /*
     * Check size of attribute input to get a rough estimate of
     * how large the attrs structures will have to be.  Even if
     * no attribute are specified, malloc some space so the
     * following functions do not have any problems.
     */
    if (attribute != NULL) {
        if (Tcl_SplitList(interp, attribute, &jargc, &jargv) != TCL_OK) {
            return TCL_ERROR;
        }
        for (i = 0; i < jargc; i++) {
            if (Tcl_SplitList(interp, jargv[i], &targc, &targv) == TCL_OK) {
               arg_count = arg_count + targc;
               free((char *)targv);
            }
        }
        free((char *)jargv);
    }
    attrs = malloc((arg_count + 5) * sizeof(sec_attr_t));
    if (attrs == NULL) {
        DCP_SET_MISC_ERROR(dcp_s_no_memory);
    }
    out_attrs = malloc((arg_count + 5) * sizeof(sec_attr_t));
    if (out_attrs == NULL) {
        free((sec_attr_t *)attrs);
        DCP_SET_MISC_ERROR(dcp_s_no_memory);
    }

    if (parse_attribute_list(interp, attribute, &pgo_item, NULL,
                             &attr_info, attrs) != TCL_OK) {
        FREE_ATTR_PTRS(attr_info.era_count, attrs);
        free((sec_attr_t *)attrs);
        free((sec_attr_t *)out_attrs);
        return TCL_ERROR;
    }

    /* 
     * Check to see if the correct number of switches have been set. 
     * If the user wants to generate an alias he/she must provide 
     * both the alias and a appropriate uid of an existing principal.
     * If the user wants to specify an uuid then she/he cannot be specifying
     * a request for an alias. 
     */
    if ((pgo_item.unix_num == -1) && 
        (pgo_item.flags & sec_rgy_pgo_is_an_alias)) {
        FREE_ATTR_PTRS(attr_info.era_count, attrs);
        free((sec_attr_t *)attrs);
        free((sec_attr_t *)out_attrs);
        DCP_SET_MISC_ERROR(dcp_s_rgy_no_uid); 
    }
    if ((pgo_item.flags & sec_rgy_pgo_is_an_alias) && 
        !uuid_is_nil(&pgo_item.id,&result)) {
        FREE_ATTR_PTRS(attr_info.era_count, attrs);
        free((sec_attr_t *)attrs);
        free((sec_attr_t *)out_attrs);
        DCP_SET_MISC_ERROR(dcp_s_rgy_uuid_and_alias);
    }

    /*
     * Split the argument list into individual principal names.
     */
    if (Tcl_SplitList(interp, argv[1], &cargc, &cargv) != TCL_OK) {
        FREE_ATTR_PTRS(attr_info.era_count, attrs);
        free((sec_attr_t *)attrs);
        free((sec_attr_t *)out_attrs);
        return TCL_ERROR;
    }
    else {
        /*
         * Check that we did not receive an empty argument list.
         */
        if ((cargc == 0) && (cargv[0] == NULL)) {
            FREE_ATTR_PTRS(attr_info.era_count, attrs);
            free((sec_attr_t *)attrs);
            free((sec_attr_t *)out_attrs);
            DCP_SET_MISC_ERROR(dcp_s_null_arg);
        }
    }

    if ((is_simple = is_simple_name(cargc, cargv)) == -1) {
        free((char *)cargv);
        FREE_ATTR_PTRS(attr_info.era_count, attrs);
        free((sec_attr_t *)attrs);
        free((sec_attr_t *)out_attrs);
        DCP_SET_MISC_ERROR(dcp_s_rgy_full_or_simple);
    }

    for (i = 0; i < cargc ; i++) {
        /*
         * Initialize the name parameter. Assure that the user is 
         * only using either full or simple names. 
         */
        strcpy((char *)global_name, cargv[i]);
        if (dcp_id_parse_name(interp, global_name, cell_name, princ_name) != TCL_OK) {
            free((char *)cargv);
            FREE_ATTR_PTRS(attr_info.era_count, attrs);
            free((sec_attr_t *)attrs);
            free((sec_attr_t *)out_attrs);
            return TCL_ERROR;
        }

        if (open_rgy_site(interp, cell_name, site_update) != TCL_OK) {
            free((char *)cargv);
            FREE_ATTR_PTRS(attr_info.era_count, attrs);
            free((sec_attr_t *)attrs);
            free((sec_attr_t *)out_attrs);
            return TCL_ERROR;
        }

        sec_rgy_pgo_add(rgy_context,
                        sec_rgy_domain_person,
                        princ_name,
                        &pgo_item,
                        &result);
        if (result != sec_rgy_status_ok) {
            free((char *)cargv);
            FREE_ATTR_PTRS(attr_info.era_count, attrs);
            free((sec_attr_t *)attrs);
            free((sec_attr_t *)out_attrs);
            DCP_SET_MISC_ERROR(result);
        }

        /*
         * Check if any extended attributes were specified during the
         * create.  If so, they are stored in the attrs structure. Call
         * the proper function to get them processed.
         */
        if (attr_info.era_count > 0) {
            sec_rgy_attr_update(rgy_context,
                                sec_rgy_domain_person,
                                princ_name,
                                attr_info.era_count,
                                space_avail,
                                attrs,
                                &num_returned,
                                out_attrs,
                                &num_left,
                                &failure_index,
                                &result);
            if (result != sec_rgy_status_ok) {
                free((char *)cargv);
                FREE_ATTR_PTRS(attr_info.era_count, attrs);
                free((sec_attr_t *)attrs);
                free((sec_attr_t *)out_attrs);
                DCP_SET_MISC_ERROR(result);
            }
        }
    }
    free((char *)cargv);
    FREE_ATTR_PTRS(attr_info.era_count, attrs);
    free((sec_attr_t *)attrs);
    free((sec_attr_t *)out_attrs);
    return TCL_OK;
}


/*
 * FUNCTION: principal_delete
 *
 * OVERVIEW:
 *
 * principal_delete will remove the named principal from the 
 * registry. 
 *
 * INPUTS:
 *  int argc - The number of command line arguments passed into function.
 *  char **argv - The actual arguements.
 *
 * INPUT/OUTPUTS:
 * Tcl_Interp *interp - Structure associated with the interpreter. This         
 *                      will hold the error message (if any) generated from the
 *                      command. This is also the location of any convienience
 *                      variables.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 *    
 */
int principal_delete(ClientData clientData,
                     Tcl_Interp *interp,
                     int argc,
                     char **argv)
{
    int             result = TCL_OK;
    int             pargc;
    char            **pargv;
    int             i = 0;
    sec_rgy_name_t  global_name;
    sec_rgy_name_t  cell_name;
    sec_rgy_name_t  princ_name;
    boolean32       is_simple;

    dcp_ArgvInfo arg_table[] = { 
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    DCP_CHECK_RGY_NO_ARG(princ);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    if (Tcl_SplitList(interp, argv[1], &pargc, &pargv) == TCL_ERROR) {
        return TCL_ERROR;
    }
    else {
        /*
         * Check that we did not receive an empty argument list.
         */
        if ((pargc == 0) && (pargv[0] == NULL)) {
            DCP_SET_MISC_ERROR(dcp_s_null_arg);
        }
    }

    if ((is_simple = is_simple_name(pargc, pargv)) == -1) {
        free((char *)pargv);
        DCP_SET_MISC_ERROR(dcp_s_rgy_full_or_simple);
    }

    for (; i < pargc; i++) {
        strcpy((char *)global_name, pargv[i]);
        if (dcp_id_parse_name(interp, global_name, cell_name, princ_name) != TCL_OK) {
            free((char *)pargv);
            return TCL_ERROR;
        }

        if (open_rgy_site(interp, cell_name, site_update) != TCL_OK) {
            free((char *)pargv);
            return TCL_ERROR;
        }

        sec_rgy_pgo_delete(rgy_context,
                           sec_rgy_domain_person,
                           princ_name,
                           (error_status_t *)&result);
        if (result != sec_rgy_status_ok) {
            free((char *)pargv);
            DCP_SET_MISC_ERROR(result);
        }
    }

    free((char *)pargv);
    return TCL_OK;
}


/*
 * FUNCTION: principal_catalog
 *
 * OVERVIEW:
 *
 * principal_catalog will list all principal entries within the 
 * registry. 
 *
 * INPUTS:
 *  int argc - The number of command line arguments passed into function.
 *  char **argv - The actual arguements.
 *
 * INPUT/OUTPUTS:
 *  Tcl_Interp *interp - Structure associated with the interpreter. This         
 *                       will hold the error message (if any) generated from the
 *                       command. This is also the location of any convienience
 *                       variables.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 *    
 */
int principal_catalog(ClientData clientData, 
                      Tcl_Interp *interp, 
                      int argc, 
                      char **argv)
{
    error_status_t      result = TCL_OK;
    sec_rgy_pgo_item_t  pgo_item;
    sec_rgy_cursor_t    item_cursor;
    char                *output;
    sec_rgy_name_t      name;
    sec_rgy_name_t      name1;
    sec_rgy_name_t      global_name;
    sec_rgy_name_t      cell_name;
    size_t              princ_name_size;
    int                 pargc = 0;
    char                **pargv;
    int                 i = 0;
    int                 bad_rgy_cursor_count = 0;
    int                 saved_length;
    boolean32           simplename = FALSE;
    char                *expand_global_name = NULL;

    dcp_ArgvInfo arg_table[] = { 
        {"-simplename", DCP_ARGV_CONSTANT, (char *)1, NULL,
            dcp_t_rgy_princ_cat_simple_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    arg_table[0].dst = (char *)&simplename; 

    if ((result = dcp_ParseArgv(interp, &argc, argv, arg_table, 0)) == TCL_ERROR)
        return TCL_ERROR;

    global_name[0] = '\0';
    if (argc > 1) {
        DCP_CHECK_EXTRA_ARGS();
        DCP_GENERAL_SET_NAMELIST(argv[1]);
        if (dcp_general_split_list(interp, argv[1], &pargc, &pargv) != TCL_OK) {
            return TCL_ERROR;
        }
        else {
            /*
             * Check that we did not receive an empty argument list.
             */
            if ((pargc == 0) && (pargv[0] == NULL)) {
                DCP_SET_MISC_ERROR(dcp_s_null_arg);
            }
        }
    }
    else {
        pargc = 1;
        pargv = (char **)malloc(2 * sizeof(char *));
        if (pargv == NULL) {
            DCP_SET_RESULT_CODE(dcp_s_no_memory);
            return TCL_ERROR;
        }
        pargv[0] = (char *)global_name;
        pargv[1] = NULL;
    }

    for (; i < pargc; i++) {
        strcpy((char *)global_name, pargv[i]);
        if (open_rgy_site(interp, global_name, site_query) != TCL_OK) {
            free((char *)pargv);
            DCP_SET_SEC_ERROR(dcp_s_rgy_invalid_cell, global_name);
        }
        strcpy((char *)cell_name, (char *)site_cache[realtime_cache].cell_name);
        if (*pargv[i] != '\0') {
            if (global_name[0] == '/') {
                expand_global_name = expand_cell_name((char *)global_name, &result);
                if (result != rpc_s_incomplete_name) {
                    strcpy((char *)global_name, expand_global_name);
                    free(expand_global_name);
                }
            }
            if (strcmp((char *)global_name, (char *)cell_name) != 0) {
                free((char *)pargv);
                DCP_SET_SEC_ERROR(dcp_s_rgy_invalid_cell, global_name);
            }
        }
        strcpy((char *)name, "");
        strcpy((char *)name1, "");
        sec_rgy_cursor_reset(&item_cursor);

        princ_name_size = strlen((char *)cell_name) + sec_rgy_pname_t_size + 1;
        output = (char *)malloc(princ_name_size);
        if (output == NULL) {
            free((char *)pargv);
            DCP_SET_MISC_ERROR(dcp_s_no_memory);
        }

        if (strlen(interp->result)) {
            Tcl_AppendResult(interp, "\n", NULL);
            saved_length = strlen(interp->result);
        }
        else {
            saved_length = 0;
        }

        for (result = error_status_ok; result == error_status_ok;) {
            sec_rgy_pgo_get_next(rgy_context,
                                 sec_rgy_domain_person,
                                 name,
                                 &item_cursor,
                                 &pgo_item,
                                 name1,
                                 &result);
            if (result == sec_rgy_no_more_entries)
                continue;
            if ((result == sec_rgy_object_not_found) && (bad_rgy_cursor_count < 5)) {
                /*
                 * This is a very unexpected result, but if it occurs, the
                 * registry cursor has been trashed somehow, probably
                 * because the element it was pointing to was deleted out
                 * from under it.  The recommended recovery is to start
                 * again, but to avoid a possible endless loop, we'll only
                 * re-try a few times.  If we still cannot complete the
                 * command without the registry cursor being hosed, just
                 * forget it and report the error.
                 */
                bad_rgy_cursor_count++;
                strcpy((char *)name, "");
                strcpy((char *)name1, "");
                sec_rgy_cursor_reset(&item_cursor);
                interp->result[saved_length] = '\0';
                result = error_status_ok;
                continue;
            }
            if (result != sec_rgy_status_ok) {
                free((char *)pargv);
                free(output);
                DCP_SET_MISC_ERROR(result);
            }

            if (strlen(interp->result))
                Tcl_AppendResult(interp, "\n", NULL);
            if (!simplename)
                sprintf(output, "%s/%s", cell_name, name1);
            else
                sprintf(output, "%s", name1);
            Tcl_AppendResult(interp, output, NULL);
        }
        free(output);
    }

    free((char *)pargv);
    return TCL_OK;
}


/*
 * FUNCTION: principal_modify
 *
 * OVERVIEW:
 * 
 *
 * INPUTS:
 *  int argc - The number of command line arguments passed into function.
 *  char **argv - The actual arguements.
 *
 * INPUT/OUTPUTS:
 * Tcl_Interp *interp - Structure associated with the interpreter. This         
 *                      will hold the error message (if any) generated from the
 *                      command. This is also the location of any convienience
 *                      variables.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 *    
 */
int principal_modify(ClientData clientData,
                     Tcl_Interp *interp,
                     int argc,
                     char **argv)
{
    error_status_t        result = TCL_OK;
    sec_rgy_name_t        global_name;
    sec_rgy_name_t        cell_name;
    sec_rgy_name_t        princ_name;
    char                  *attribute = NULL;
    char                  *attribute1 = NULL;
    char                  *attribute2 = NULL;
    char                  *attribute3 = NULL;
    char                  *alias = NULL;
    char                  *quota = NULL;
    char                  *types = NULL;
    char                  *fullname = NULL;
    sec_rgy_cursor_t      item_cursor;
    sec_rgy_pgo_item_t    pgo_item;
    sec_rgy_pgo_item_t    temp_pgo_item;
    rgy_attribute_info_t  attr_info;
    int                   i;
    int                   era_count_hold = 0;
    char                  **oargv;
    int                   oargc;
    char                  **jargv;
    int                   jargc = 0;
    char                  **targv;
    int                   targc = 0;
    int                   arg_count = 0;
    sec_attr_t            *attrs;
    sec_attr_t            *tmp_attrs;
    sec_attr_t            *out_attrs;
    sec_attr_t            *del_attrs;
    unsigned32            num_returned = 0;
    unsigned32            num_left = 0;
    unsigned32            space_avail = 0;
    signed32              failure_index;
    boolean32             is_simple;
    unsigned32            cur_num_attrs;
    sec_attr_cursor_t     attr_cursor;

    /*
     * Set up the parser argument table and initialize the variables.
     */
    dcp_ArgvInfo arg_table[] = { 
        {"-add",DCP_ARGV_STRING,NULL,NULL,
             dcp_t_rgy_princ_mod_add_help},
        {"-alias",DCP_ARGV_STRING,NULL,NULL,
             dcp_t_rgy_princ_alias_help},
        {"-change",DCP_ARGV_STRING,NULL,NULL,
             dcp_t_rgy_princ_mod_change_help},
        {"-fullname",DCP_ARGV_STRING,NULL,NULL,
             dcp_t_rgy_princ_fname_help},
        {"-quota",DCP_ARGV_STRING,NULL,NULL,
             dcp_t_rgy_princ_quota_help},
        {"-remove",DCP_ARGV_STRING,NULL,NULL,
             dcp_t_rgy_princ_mod_remove_help},
        {"-types",DCP_ARGV_CONSTANT,(char *)1,NULL,
             dcp_t_rgy_princ_mod_types_help},
        {NULL,DCP_ARGV_END,NULL,NULL,0} 
    };

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    arg_table[0].dst = (char *)&attribute1;
    arg_table[1].dst = (char *)&alias;
    arg_table[2].dst = (char *)&attribute2;
    arg_table[3].dst = (char *)&fullname;
    arg_table[4].dst = (char *)&quota;
    arg_table[5].dst = (char *)&attribute3;
    arg_table[6].dst = (char *)&types;

    if ((result = dcp_ParseArgv(interp, &argc, argv, arg_table, 0)) == TCL_ERROR)
        return TCL_ERROR;

    /* 
     * The user cannot both promote and demote a principal in the same 
     * operation. Also verify that a principal name has been supplied.
     */
    DCP_CHECK_RGY_NO_ARG(princ);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * The user cannot specify combinations of add, change and remove
     * on the same command line.  Only one at a time is allowed.
     */
    if ((attribute1 != NULL && attribute2 != NULL) ||
        (attribute1 != NULL && attribute3 != NULL) ||
        (attribute2 != NULL && attribute3 != NULL)) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_era_illegal_switches);
    }

    /*
     * The user also cannot specify an attribute list and a variety of 
     * switches. (ie "principal modify foo -alias yes -change {name foob}"
     * is illegal).
     */
    if ((alias != NULL || fullname != NULL || quota != NULL) &&
        (attribute1 != NULL || attribute2 != NULL || attribute3 != NULL)) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_list_and_switches);
    }

    /*
     * If the user didn't specify any type of modify command,
     * then this is an error.
     */
    if (alias == NULL && fullname == NULL && quota == NULL && 
        attribute1 == NULL && attribute2 == NULL && attribute3 == NULL) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_no_modify_operation);
    }

    /*
     * If the user specified the types option with anything but
     * a remove option, then this is an error.
     */
    if (types != NULL && attribute3 == NULL) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_bad_use_of_types_option);
    }

    /*
     * Initialize the pgo_item to a known value.
     */
    init_pgo_item(&temp_pgo_item);

    /* 
     * Determine which elements are being modified and update the 
     * temp_pgo_item accordingly. 
     */
    attr_info.domain = sec_rgy_domain_person;
    attr_info.policy = FALSE;
    attr_info.pgo = FALSE;
    attr_info.era_count = 0;
    attr_info.del_count = 0;
    attr_info.flags = pgo_modify;
    attr_info.pgo_item = &temp_pgo_item;
    attr_info.fullname = NULL;

    if (types == NULL)
       attr_info.types = FALSE;
    else
       attr_info.types = TRUE;

    if (attribute1 != NULL) {
       attr_info.modify_type = modify_type_add;
       attribute = attribute1;
    }
    else if (attribute2 != NULL) {
       attr_info.modify_type = modify_type_change;
       attribute = attribute2;
    }
    else if (attribute3 != NULL) {
       attr_info.modify_type = modify_type_remove;
       attribute = attribute3;
    }
    else {
       attr_info.modify_type = modify_type_none;
    }

    parse_item_switch_list(quota, fullname, NULL, NULL, alias, NULL, &temp_pgo_item,
                           sec_rgy_domain_person, &attr_info, &result);
    if (result == dcp_s_rgy_quota_out_of_range) {
        DCP_SET_SEC_ERROR(result, MAXINT);
    }
    DCP_CHECK_SEC_ERROR(result);

    /*
     * Check size of attribute input to get a rough estimate of
     * how large the attrs structures will have to be.  Even if
     * no attribute are specified, malloc some space so the
     * following functions do not have any problems.
     */
    if (attribute != NULL) {
        if (Tcl_SplitList(interp, attribute, &jargc, &jargv) != TCL_OK) {
            return TCL_ERROR;
        }
        for (i = 0; i < jargc; i++) {
            if (Tcl_SplitList(interp, jargv[i], &targc, &targv) == TCL_OK) {
               arg_count = arg_count + targc;
               free((char *)targv);
            }
        }
        free((char *)jargv);
    }
    attrs = malloc((arg_count + 5) * sizeof(sec_attr_t));
    if (attrs == NULL) {
        DCP_SET_MISC_ERROR(dcp_s_no_memory);
    }
    tmp_attrs = malloc((arg_count + 5) * sizeof(sec_attr_t));
    if (tmp_attrs == NULL) {
        free((sec_attr_t *)attrs);
        DCP_SET_MISC_ERROR(dcp_s_no_memory);
    }

    /*
     * Parse the attribute list.  The list will apply to each
     * object being operated on if there are more than one.
     */
    if (parse_attribute_list(interp, attribute, &temp_pgo_item, NULL,
                             &attr_info, attrs) != TCL_OK) {
        FREE_ATTR_PTRS(attr_info.era_count, attrs);
        free((sec_attr_t *)attrs);
        free((sec_attr_t *)tmp_attrs);
        return TCL_ERROR;
    }

    /*
     * Modify each principal object that was specified on the command line.
     */
    if (Tcl_SplitList(interp, argv[1], &oargc, &oargv) != TCL_OK) {
        FREE_ATTR_PTRS(attr_info.era_count, attrs);
        free((sec_attr_t *)attrs);
        free((sec_attr_t *)tmp_attrs);
        return TCL_ERROR;
    }
    else {
        /*
         * Check that we did not receive an empty argument list.
         */
        if ((oargc == 0) && (oargv[0] == NULL)) {
            FREE_ATTR_PTRS(attr_info.era_count, attrs);
            free((sec_attr_t *)attrs);
            free((sec_attr_t *)tmp_attrs);
            DCP_SET_MISC_ERROR(dcp_s_null_arg);
        }
    }

    if ((is_simple = is_simple_name(oargc, oargv)) == -1) {
        free((char *)oargv);
        FREE_ATTR_PTRS(attr_info.era_count, attrs);
        free((sec_attr_t *)attrs);
        free((sec_attr_t *)tmp_attrs);
        DCP_SET_MISC_ERROR(dcp_s_rgy_full_or_simple);
    }

    for (i = 0; i < oargc; i++) {
        /*
         * Copy the principal name and attempt to aquire it from the registry.
         */
        strcpy((char *)global_name, oargv[i]);
        if (dcp_id_parse_name(interp, global_name, cell_name, princ_name) != TCL_OK) {
            free((char *)oargv);
            FREE_ATTR_PTRS(attr_info.era_count, attrs);
            free((sec_attr_t *)attrs);
            free((sec_attr_t *)tmp_attrs);
            return TCL_ERROR;
        }

        if (open_rgy_site(interp, cell_name, site_update) != TCL_OK) {
            free((char *)oargv);
            FREE_ATTR_PTRS(attr_info.era_count, attrs);
            free((sec_attr_t *)attrs);
            free((sec_attr_t *)tmp_attrs);
            return TCL_ERROR;
        }
        sec_rgy_pgo_get_by_name(rgy_context,
                                sec_rgy_domain_person,
                                princ_name,
                                &item_cursor,
                                &pgo_item,
                                &result);
        if (result != sec_rgy_status_ok) {
            free((char *)oargv);
            FREE_ATTR_PTRS(attr_info.era_count, attrs);
            free((sec_attr_t *)attrs);
            free((sec_attr_t *)tmp_attrs);
            DCP_SET_MISC_ERROR(result);
        }

        /* 
         * Copy the temporary pgo item into the pgo item aquired from
         * the registry for this principal
         */
        copy_pgo_item_with_restrict(&pgo_item, &temp_pgo_item, &attr_info);

        sec_rgy_pgo_replace(rgy_context,
                            sec_rgy_domain_person,
                            princ_name,
                            &pgo_item,
                            (error_status_t *)&result);
        if (result != sec_rgy_status_ok) {
            free((char *)oargv);
            FREE_ATTR_PTRS(attr_info.era_count, attrs);
            free((sec_attr_t *)attrs);
            free((sec_attr_t *)tmp_attrs);
            DCP_SET_MISC_ERROR(result);
        }

        /*
         * Check if any extended attributes were specified during the
         * modify.  If so, they are stored in the attrs structure.
         */
        if (attr_info.era_count > 0) {
            era_count_hold = attr_info.era_count;

            /*
             * We need to get a rough count of the existing attributes for
             * the given object.  To do this, we can initialize the attribute
             * attribute cursor, get the number, then release the cursor.
             */
            cur_num_attrs = 0;
            sec_rgy_attr_cursor_init(rgy_context,
                                     sec_rgy_domain_person,
                                     princ_name,
                                     &cur_num_attrs,
                                     &attr_cursor,
                                     &result);
            if (result != sec_rgy_status_ok) {
                free((char *)oargv);
                FREE_ATTR_PTRS(attr_info.era_count, attrs);
                free((sec_attr_t *)attrs);
                free((sec_attr_t *)tmp_attrs);
                DCP_SET_MISC_ERROR(result);
            }
            sec_rgy_attr_cursor_release(&attr_cursor, &result);
            if (result != sec_rgy_status_ok) {
                free((char *)oargv);
                FREE_ATTR_PTRS(attr_info.era_count, attrs);
                free((sec_attr_t *)attrs);
                free((sec_attr_t *)tmp_attrs);
                DCP_SET_MISC_ERROR(result);
            }

            /*
             * Based on the number of existing attributes, plus the number
             * of attributes the user is trying to modify, allocate memory
             * for the out_attrs and del_attrs structures.
             */
            out_attrs = malloc((attr_info.era_count + cur_num_attrs + 5) * sizeof(sec_attr_t));
            if (out_attrs == NULL) {
                free((char *)oargv);
                FREE_ATTR_PTRS(era_count_hold, attrs);
                free((sec_attr_t *)attrs);
                free((sec_attr_t *)tmp_attrs);
                DCP_SET_MISC_ERROR(dcp_s_no_memory);
            }
            del_attrs = malloc((attr_info.era_count + cur_num_attrs + 5) * sizeof(sec_attr_t));
            if (del_attrs == NULL) {
                free((char *)oargv);
                FREE_ATTR_PTRS(era_count_hold, attrs);
                free((sec_attr_t *)attrs);
                free((sec_attr_t *)tmp_attrs);
                free((sec_attr_t *)out_attrs);
                DCP_SET_MISC_ERROR(dcp_s_no_memory);
            }

            /*
             * The attrs structure must be changed to reflect the real
             * processing that will go on with the extended attributes.
             * Multiple instances, multiple values, and the differences
             * between -add, -change, and -remove make further processing
             * of the extended attributes a requirement.
             */
            result = check_era_info(interp,
                                    sec_rgy_domain_person,
                                    princ_name,
                                    attrs,
                                    &attr_info,
                                    out_attrs,
                                    del_attrs);
            if (result != sec_rgy_status_ok) {
                free((char *)oargv);
                FREE_ATTR_PTRS(era_count_hold, attrs);
                free((sec_attr_t *)attrs);
                free((sec_attr_t *)tmp_attrs);
                free((sec_attr_t *)out_attrs);
                free((sec_attr_t *)del_attrs);
                DCP_SET_MISC_ERROR(result);
            }

            /*
             * If we managed to get through the check function without any
             * errors, and there are still attributes to process, do so.
             */
            if (attr_info.era_count > 0) {
                /*
                 * The out_attrs structure is filled with information
                 * to add or change, so use the update function.
                 */
                sec_rgy_attr_update(rgy_context,
                                    sec_rgy_domain_person,
                                    princ_name,
                                    attr_info.era_count,
                                    space_avail,
                                    out_attrs,
                                    &num_returned,
                                    tmp_attrs,
                                    &num_left,
                                    &failure_index,
                                    &result);
                if (result != sec_rgy_status_ok) {
                    free((char *)oargv);
                    FREE_ATTR_PTRS(era_count_hold, attrs);
                    FREE_ATTR_PTRS(attr_info.era_count, out_attrs);
                    FREE_ATTR_PTRS(attr_info.del_count, del_attrs);
                    free((sec_attr_t *)attrs);
                    free((sec_attr_t *)tmp_attrs);
                    free((sec_attr_t *)out_attrs);
                    free((sec_attr_t *)del_attrs);
                    DCP_SET_MISC_ERROR(result);
                }
            }
            if (attr_info.del_count > 0) {
                /*
                 * The del_attrs structure is filled with information
                 * to remove from the database, so use the remove function.
                 */
                sec_rgy_attr_delete(rgy_context,
                                    sec_rgy_domain_person,
                                    princ_name,
                                    attr_info.del_count,
                                    del_attrs,
                                    &failure_index,
                                    &result);
                if (result != sec_rgy_status_ok) {
                    free((char *)oargv);
                    FREE_ATTR_PTRS(era_count_hold, attrs);
                    FREE_ATTR_PTRS(attr_info.era_count, out_attrs);
                    FREE_ATTR_PTRS(attr_info.del_count, del_attrs);
                    free((sec_attr_t *)attrs);
                    free((sec_attr_t *)tmp_attrs);
                    free((sec_attr_t *)out_attrs);
                    free((sec_attr_t *)del_attrs);
                    DCP_SET_MISC_ERROR(result);
                }
            }

            /*
             * We successfully processed some era attributes.  Since we
             * did not run into any errors, we have some free'ing to do
             * of the sec_attr_t structures.  The next pass through the
             * loop will malloc the required space.
             */
            FREE_ATTR_PTRS(attr_info.era_count, out_attrs);
            FREE_ATTR_PTRS(attr_info.del_count, del_attrs);
            free((sec_attr_t *)out_attrs);
            free((sec_attr_t *)del_attrs);

            /*
             * Reset the era count.
             */
            attr_info.era_count = era_count_hold;
        }
    }
    free((char *)oargv);
    FREE_ATTR_PTRS(attr_info.era_count, attrs);
    free((sec_attr_t *)attrs);
    free((sec_attr_t *)tmp_attrs);
    return TCL_OK;
}


/*
 * FUNCTION: principal_rename
 *
 * OVERVIEW:
 * 
 *
 * INPUTS:
 *  int argc - The number of command line arguments passed into function.
 *  char **argv - The actual arguements.
 *
 * INPUT/OUTPUTS:
 * Tcl_Interp *interp - Structure associated with the interpreter. This         
 *                      will hold the error message (if any) generated from the
 *                      command. This is also the location of any convienience
 *                      variables.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 *    
 */
int principal_rename(ClientData clientData,
                     Tcl_Interp *interp,
                     int argc,
                     char **argv)
{
    error_status_t  result = TCL_OK;
    char            *new_name_ptr = NULL;
    sec_rgy_name_t  global_name;
    sec_rgy_name_t  cell_name;
    sec_rgy_name_t  princ_name;
    sec_rgy_name_t  new_name;
    boolean32       is_simple;

    dcp_ArgvInfo arg_table[] = {
        {"-to", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_princ_rename_to_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    /* 
     * Set the variables for the Parser. We need to cast them all as 
     * char pointers. 
     */
    arg_table[0].dst = (char *)&new_name_ptr;

    if ((result = dcp_ParseArgv(interp, &argc, argv, arg_table, 0)) == TCL_ERROR)
        return TCL_ERROR;

    /* 
     * Terminate the command if not enough arguments are passed in. 
     * If the correct number of args have been supplied, set the
     * namelist accordingly.
     */
    DCP_CHECK_RGY_NO_ARG(princ);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    if (new_name_ptr == NULL) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_no_name_to_rename);
    }

    result = error_status_ok;
    strcpy((char *)global_name, argv[1]);
    is_simple = is_simple_name(1, &argv[1]);

    if (dcp_id_parse_name(interp, global_name, cell_name, princ_name) != TCL_OK)
        return TCL_ERROR;

    if (open_rgy_site(interp, cell_name, site_update) != TCL_OK)
        return TCL_ERROR;

    strcpy((char *)global_name, new_name_ptr);
    if (dcp_id_parse_name(interp, global_name, cell_name, new_name) != TCL_OK)
        return TCL_ERROR;

    sec_rgy_pgo_rename(rgy_context, sec_rgy_domain_person, princ_name, new_name, &result);
    DCP_CHECK_SEC_ERROR(result);

    return TCL_OK;
}


/*
 * FUNCTION: principal_show
 *
 * OVERVIEW:
 * 
 *
 * INPUTS:
 *  int argc - The number of command line arguments passed into function.
 *  char **argv - The actual arguements.
 *
 * INPUT/OUTPUTS:
 * Tcl_Interp *interp - Structure associated with the interpreter. This         
 *                      will hold the error message (if any) generated from the
 *                      command. This is also the location of any convienience
 *                      variables.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 *    
 */
int principal_show(ClientData clientData, 
		   Tcl_Interp *interp, 
		   int argc, 
		   char **argv)
{
    int                 result = TCL_OK;
    error_status_t      status;
    sec_rgy_name_t      global_name;
    sec_rgy_name_t      cell_name;
    sec_rgy_name_t      princ_name;
    char                *quota_svc;     /* quota label */
    char                *quota_ptr;     /* actual quota */
    char                *uid_ptr;       /* actual uid */
    char                *uid_svc;       /* unix id */
    char                *uuid_svc;      /* uuid value */
    char                *fullname_svc;  /* administrative data */
    char                *alias_svc;     /* alias label */
    char                *groups_svc;    /* group label */
    char                *yes_no;
    sec_rgy_pgo_item_t  pgo_item;
    sec_rgy_cursor_t    item_cursor;
    sec_rgy_cursor_t    member_cursor;
    signed32            supplied_members;
    signed32            avail_members;
    signed32            found_members;
    int                 pargc;
    char                **pargv;
    char                uuid[UUID_STRING_LEN];
    int                 i = 0, j = 0;
    sec_rgy_member_t    member_list[50];
    boolean32           is_simple;
    boolean32           all = FALSE;
    boolean32           xattrs = FALSE;

    dcp_ArgvInfo arg_table[] = { 
        {"-all", DCP_ARGV_CONSTANT, (char *)1, NULL,
            dcp_t_rgy_princ_show_all_help},
        {"-xattrs", DCP_ARGV_CONSTANT, (char *)1, NULL,
            dcp_t_rgy_princ_show_era_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    arg_table[0].dst = (char *)&all;
    arg_table[1].dst = (char *)&xattrs;

    if ((result = dcp_ParseArgv(interp, &argc, argv, arg_table, 0)) == TCL_ERROR)
        return TCL_ERROR;

    /*
     * Check that the user has specified the options correctly.
     * Only one of the options is allowed at a time.
     */
    if ((all == TRUE) && (xattrs == TRUE)) {
        DCP_SET_MISC_ERROR(dcp_s_mut_excl_opts);
    }

    /*
     * Terminate the command if not enough arguments are passed in. 
     * If the correct number of args have been supplied, set the
     * namelist accordingly.
     */
    DCP_CHECK_RGY_NO_ARG(princ);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * The user is allowed to specify a list of principals to show.
     */
    if (Tcl_SplitList(interp, argv[1], &pargc, &pargv) == TCL_ERROR) {
        return TCL_ERROR;
    }
    else {
        /*
         * Check that we did not receive an empty argument list.
         */
        if ((pargc == 0) && (pargv[0] == NULL)) {
            DCP_SET_MISC_ERROR(dcp_s_null_arg);
        }
    }

    if ((is_simple = is_simple_name(pargc, pargv)) == -1) {
        free((char *)pargv);
        DCP_SET_MISC_ERROR(dcp_s_rgy_full_or_simple);
    }

    /*
     * Set up the field labels.
     */
    uid_svc = (char *)dce_sprintf(dcp_t_rgy_uid);
    uuid_svc = (char *)dce_sprintf(dcp_t_rgy_uuid);
    quota_svc = (char *)dce_sprintf(dcp_t_rgy_quota);
    alias_svc = (char *)dce_sprintf(dcp_t_rgy_alias);
    fullname_svc = (char *)dce_sprintf(dcp_t_rgy_fullname);
    groups_svc = (char *)dce_sprintf(dcp_t_rgy_groups);
    if (!uid_svc || !uuid_svc || !quota_svc || !alias_svc || !fullname_svc || !groups_svc) {
        free((char *)pargv);
        DCP_SET_MISC_ERROR(dcp_s_no_memory);
    }

    /* 
     * Cycle through each of the specified principal names and show 
     * all of the available information. 
     */
    for (; i < pargc; i++) {
        if (strlen(interp->result))
            Tcl_AppendResult(interp,"\n\n",NULL);
        strcpy((char *)global_name, pargv[i]);
        sec_rgy_cursor_reset(&item_cursor);
        if (dcp_id_parse_name(interp, global_name, cell_name, princ_name) != TCL_OK) {
            free((char *)pargv);
            free(uid_svc);
            free(uuid_svc);
            free(quota_svc);
            free(alias_svc);
            free(fullname_svc);
            free(groups_svc);
            return TCL_ERROR;
        }

        if (open_rgy_site(interp, cell_name, site_query) != TCL_OK) {
            free((char *)pargv);
            free(uid_svc);
            free(uuid_svc);
            free(quota_svc);
            free(alias_svc);
            free(fullname_svc);
            free(groups_svc);
            return TCL_ERROR;
        }

        if (xattrs == FALSE) {
            sec_rgy_pgo_get_by_name(rgy_context,
                                    sec_rgy_domain_person,
                                    princ_name,
                                    &item_cursor,
                                    &pgo_item,
                                    &status);
            if (status != sec_rgy_status_ok) {
                free((char *)pargv);
                free(uid_svc);
                free(uuid_svc);
                free(quota_svc);
                free(alias_svc);
                free(fullname_svc);
                free(groups_svc);
                DCP_SET_MISC_ERROR(status);
            }
            sec_rgy_pgo_get_members(rgy_context,
                                    sec_rgy_domain_person,
                                    princ_name,
                                    &member_cursor,
                                    50,
                                    member_list,
                                    &supplied_members,
                                    &avail_members,
                                    &status);
            if (status != sec_rgy_status_ok) {
                free((char *)pargv);
                free(uid_svc);
                free(uuid_svc);
                free(quota_svc);
                free(alias_svc);
                free(fullname_svc);
                free(groups_svc);
                DCP_SET_MISC_ERROR(status);
            }
            status = decode_uuid((uuid_t *)&pgo_item.id, uuid);
            if (pgo_item.quota == -1) {
                quota_ptr = (char *)dce_sprintf(dcp_t_rgy_unlimited);
            } else {
                quota_ptr = malloc(1024);
            }
            if (quota_ptr == NULL) {
                free((char *)pargv);
                free(uid_svc);
                free(uuid_svc);
                free(quota_svc);
                free(alias_svc);
                free(fullname_svc);
                free(groups_svc);
                DCP_SET_MISC_ERROR(dcp_s_no_memory);
            }
            if (pgo_item.quota != -1)
                sprintf(quota_ptr, "%d", pgo_item.quota);

            uid_ptr =  malloc(1024);
            if (uid_ptr == NULL) {
                free((char *)pargv);
                free(uid_svc);
                free(uuid_svc);
                free(quota_svc);
                free(alias_svc);
                free(fullname_svc);
                free(groups_svc);
                free(quota_ptr);
                DCP_SET_MISC_ERROR(dcp_s_no_memory);
            }
            sprintf(uid_ptr, "%d", pgo_item.unix_num);

            /* 
             * Generate the output buffers.  Install it in the interp result.
             * Check to see if the principal is an alias. 
             */
            if (pgo_item.flags & sec_rgy_pgo_is_an_alias) {
                yes_no = (char *)dce_sprintf(dcp_t_rgy_yes);
            } else {
                yes_no = (char *)dce_sprintf(dcp_t_rgy_no);
            }
            if (yes_no == NULL) {
                free((char *)pargv);
                free(uid_svc);
                free(uuid_svc);
                free(quota_svc);
                free(alias_svc);
                free(fullname_svc);
                free(groups_svc);
                free(quota_ptr);
                free(uid_ptr);
                DCP_SET_MISC_ERROR(dcp_s_no_memory);
            }

            if (strlen((char *)pgo_item.fullname))
                Tcl_AppendResult(interp, "{", fullname_svc, " {", pgo_item.fullname, "}}\n", NULL);
            else
                Tcl_AppendResult(interp, "{", fullname_svc, " {}}\n", NULL);
            Tcl_AppendResult(interp, "{", uid_svc, " ", uid_ptr, "}\n", NULL);
            Tcl_AppendResult(interp, "{", uuid_svc, " ", uuid, "}\n", NULL);
            Tcl_AppendResult(interp, "{", alias_svc, " ", yes_no, "}\n", NULL);
            Tcl_AppendResult(interp, "{", quota_svc, " ", quota_ptr, "}", NULL);
            free(uid_ptr);
            free(quota_ptr);
            free(yes_no);

            sec_rgy_cursor_reset(&member_cursor);
            found_members = 0;
            avail_members = 1;
            while (found_members < avail_members) {
                sec_rgy_pgo_get_members(rgy_context,
                                        sec_rgy_domain_person,
                                        princ_name,
                                        &member_cursor,
                                        50,
                                        member_list,
                                        &supplied_members,
                                        &avail_members,
                                        &status);
                if (status != sec_rgy_status_ok) {
                    free((char *)pargv);
                    free(uid_svc);
                    free(uuid_svc);
                    free(quota_svc);
                    free(alias_svc);
                    free(fullname_svc);
                    free(groups_svc);
                    DCP_SET_MISC_ERROR(status);
                }
                if (!found_members && !supplied_members)
                    break;
                if (!found_members && supplied_members)
                    Tcl_AppendResult(interp, "\n{", groups_svc, NULL);
                found_members += supplied_members;
                if (supplied_members) {
                    for (j = 0; j < supplied_members; j++)
                        Tcl_AppendResult(interp, " ", member_list[j], NULL);
                }
                if (found_members == supplied_members)
                    Tcl_AppendResult(interp, "}", NULL);
            }
        }
        if (xattrs || all) {
            /*
             * Display any era information associated with this name.
             */
            if (!all)
                skip_era_newline = 1;
            status = show_era(interp, sec_rgy_domain_person, princ_name);
            if ((status != sec_rgy_status_ok) && (status != rpc_s_unknown_if) &&
                (status != ept_s_not_registered)) {
                free((char *)pargv);
                free(uid_svc);
                free(uuid_svc);
                free(quota_svc);
                free(alias_svc);
                free(fullname_svc);
                free(groups_svc);
                DCP_SET_MISC_ERROR(status);
            }
        }
    }

    /* 
     * Free allocated memory. 
     */
    free((char *)pargv);
    free(uid_svc);
    free(uuid_svc);
    free(quota_svc);
    free(alias_svc);
    free(fullname_svc);
    free(groups_svc);
    return TCL_OK;
}


/*
 * FUNCTION: is_simple_name
 *
 * OVERVIEW: verifies that all names passed in are of either type simple or 
 *           full. 
 *
 * INPUTS:
 *  int argc - number of arguments to check. 
 *  char **argv - arguments to check.
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TRUE  if all names are the simple.
 *  FLASE if all names are full.
 *  -1    if there is a mix
 */
int is_simple_name(int argc, char **argv)
{
    int        i = 0;
    boolean32  simple = TRUE;

    if (argv[i][0] == '/')
        simple = FALSE;

    for (i = 1; i < argc; i++) {
        if (argv[i][0] == '/' && simple == TRUE)
            return -1;
        else if (argv[i][0] != '/' && simple == FALSE)
            return -1;
    }
    return simple;
}


/*
 * FUNCTION: name_parse
 *
 * OVERVIEW: This command takes a full pathname and parses it into  a 
 *     cell name and a residual name. 
 *
 * INPUTS:
 *  int argc - number of arguments passed in. 
 *  char **argv - pointers to arguments. 
 *
 * INPUT/OUTPUTS:
 *  Tcl_Interp *interp - contains the results of the parsing.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK on success
 */
int name_parse(ClientData clientData,
               Tcl_Interp *interp,
               int argc,
               char **argv)
{
    sec_rgy_name_t  global_name;
    sec_rgy_name_t  cell_name;
    sec_rgy_name_t  residual_name;

    dcp_ArgvInfo arg_table[] = { 
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    /*
     * Check for extraneous arguments.
     */
    DCP_CHECK_EXTRA_ARGS();
    if (argc < 2) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_no_name_to_parse);
    }
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    memset((char *)residual_name, 0, sizeof(residual_name));
    memset((char *)cell_name, 0, sizeof(cell_name));
    strcpy((char *)global_name, argv[1]);

    if (dcp_id_parse_name(interp, global_name, cell_name, residual_name) != TCL_OK)
        return TCL_ERROR;

    Tcl_AppendResult(interp, cell_name, " ", residual_name, NULL);
    return TCL_OK;
}


/*
 * FUNCTION: name_get
 *
 * OVERVIEW: returns the machine name given a protocol sequence and 
 *     IP address. 
 *
 * INPUTS:
 *  ClientData clientData
 *  int argc
 *  char **argv
 *
 * INPUT/OUTPUTS:
 *  Tcl_Interp *interp - contains the result of the transformation
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK on successful completion.
 */
int name_get(ClientData clientData,
             Tcl_Interp *interp,
             int argc,
             char **argv)
{
    unsigned32             status;
    int                    aargc;
    char                   **aargv;
    char                   *binding_string;
    char                   *tmp;
    uuid_t                 entry_uuid;
    uuid_t                 attr_uuid;
    dced_binding_handle_t  dced_binding;
    handle_t               rpc_binding;
    sec_attr_t             *data;
    char                   host_name[1024];

    dcp_ArgvInfo arg_table[] = { 
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();
 
    /*
     * Check for extraneous arguments.
     */
    DCP_CHECK_EXTRA_ARGS();
    if (argc < 2) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_no_binding_to_parse);
    }

    /* 
     * Check the string binding that was provided. If it is simple string 
     * binding formation do nothing. Otherwise try the transformation from
     * tcl syntax.
     */
    if (Tcl_SplitList(interp, argv[1], &aargc, &aargv) != TCL_OK) {
        return TCL_ERROR;
    }

    if (aargc > 1) {
        tmp = dcp_binding_tcl_to_string(aargc, aargv, &status);
        DCP_CHECK_RPC_ERROR_FREE_ONE(status, aargv);
        binding_string = dce_strdup(tmp);
        rpc_string_free((unsigned_char_t **)&tmp, &status);
    }
    else
        binding_string = dce_strdup(argv[1]);

    /* 
     * Obtain an rpc string binding from the string. Then transform it
     * into a dced binding.
     */
    rpc_binding_from_string_binding((unsigned_char_t *)binding_string,
                                    &rpc_binding,
                                    &status);
    if (status != rpc_s_ok) {
        free((char *)aargv);
        free(binding_string);
        DCP_SET_MISC_ERROR(status);
    }

    dced_binding_from_rpc_binding(dced_e_service_type_hostdata,
                                  rpc_binding,
                                  &dced_binding,
                                  &status);
    if (status != rpc_s_ok) {
        free((char *)aargv);
        free(binding_string);
        DCP_SET_MISC_ERROR(status);
    }
    free(binding_string);

    /* 
     * Set up the uuid to read the hostdata infomation from the bound 
     * dced. Then make the hostdata read call. 
     */
    entry_uuid = dced_g_uuid_host_name;
    attr_uuid = dced_g_uuid_fileattr;

    dced_hostdata_read(dced_binding,
                       &entry_uuid,
                       &attr_uuid,
                       &data,
                       (error_status_t *)&status);
    DCP_CHECK_RPC_ERROR_FREE_ONE(status, aargv);

    /*
     * Save the name in interp->result.
     */
    strcpy(host_name, (char *)SA_ATTR_STR_ARRAY_ELT_P(data, 0));
    Tcl_AppendResult(interp, host_name, NULL);
    dced_binding_free(dced_binding, &status);
    DCP_CHECK_RPC_ERROR_FREE_ONE(status, aargv);

    free((char *)aargv);
    return TCL_OK;
}
