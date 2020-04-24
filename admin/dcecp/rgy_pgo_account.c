/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: rgy_pgo_account.c,v $
 * Revision 1.1.10.4  1996/11/13  17:54:05  arvind
 * 	Merge changes.
 * 	[1996/10/21  18:27 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk7/3]
 *
 * 	Minor code change.
 * 	[1996/10/15  16:00 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk7/2]
 *
 * 	CHFts20086: Force multiple new passphrases to be identical when entered
 * 	on the same command line.  Also handle default generate key values correctly.
 * 	[1996/10/11  17:52 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk7/1]
 *
 * 	CHFts20138: Add the ability to have pk passwords prompted for as opposed
 * 	to forcing the user to enter them on the command line.
 * 	[1996/10/11  17:53 UTC  cuti  /main/DCE_1.2.2/cuti_pk_fix/2]
 *
 * 	check status of put_pub_key
 * 	[1996/10/10  12:24 UTC  cuti  /main/DCE_1.2.2/cuti_pk_fix/1]
 *
 * 	Make default key gen works
 * 	[1996/09/16  20:30 UTC  aha  /main/DCE_1.2.2/4]
 *
 * 	CHFts19896: public key version support
 * 	[1996/09/13  23:45 UTC  aha  /main/DCE_1.2.2/aha_pk9_3/1]
 *
 * 	Merge Registry support for KDC private key storage
 * 	[1996/07/16  13:50 UTC  aha  /main/DCE_1.2.2/aha_pk6/1]
 *
 * 	Changes for Public Key Login; link in sec_pubkey (libdce) functions for
 * 	reading public key keyfiles.
 * 	[1996/07/08  17:23 UTC  truitt  /main/DCE_1.2.2/2]
 *
 * 	Merge changes.
 * 	[1996/07/03  15:59 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/7]
 *
 * 	Minor variable change.
 * 	[1996/07/03  15:28 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/6]
 *
 * 	Minor name change.
 * 	[1996/07/03  14:26 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/5]
 *
 * 	Minor name change.
 * 	[1996/07/02  17:34 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/4]
 *
 * 	Additional pk work.
 * 	[1996/06/28  13:57 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/3]
 *
 * 	Continue public key work.
 * 	[1996/06/27  18:53 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/2]
 *
 * 	Public key back-end work.
 * 	[1996/06/11  19:11 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/1]
 *
 * 	Some dcecp changes to support Public Key functionality.
 * 	Merge with latest HP fixes and enhancements.
 * 	[1996/03/28  18:49 UTC  truitt  /main/HPDCE02/19]
 *
 * 	Merge changes.
 * 	[1996/03/28  18:47 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf72/1]
 *
 * 	CHFts17168: The account, pgo, registry and schema calls all have serious
 * 	problems when called with an argument of " ".  Note they work correctly if the
 * 	argument is {" "}.  Trap the first type of argument as illegal.
 * 	[1996/03/20  21:46 UTC  truitt  /main/HPDCE02/18]
 *
 * 	Merge changes.
 * 	[1996/03/20  21:45 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf70/1]
 *
 * 	CHFts17733: Some complication with registry cursors that currently cannot
 * 	be fixed is causing problems for dcecp in some rare occasions.  In order to
 * 	work around this problem until registry cursors are completely fixed, we'll watch
 * 	the output from the api calls that use the rgy cursor and make sure we trap
 * 	and handle the correct errors.
 * 	[1996/03/18  18:28 UTC  truitt  /main/HPDCE02/17]
 *
 * 	Merge changes.
 * 	[1996/03/18  18:26 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf69/1]
 *
 * 	CHFts17225: Modify some on-line help messages to match current functionality.
 * 	CHFts17407: Condense some of the sam messages that are duplicates.
 * 	[1996/02/15  18:30 UTC  truitt  /main/HPDCE02/16]
 *
 * 	Merge brief changes.
 * 	[1996/02/15  18:26 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf62/1]
 *
 * 	CHFts17451: Remove include of value.h so duplicate macro
 * 	warning does not appear during HP build.  The include is
 * 	required on other platforms.
 * 	[1996/02/06  21:15 UTC  truitt  /main/HPDCE02/15]
 *
 * 	Merge changes.
 * 	[1996/02/06  21:13 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf60/1]
 *
 * 	CHFts16677: Make sure the correct error message is displayed
 * 	when an invalid cellname is passed to a 'catalog' operation.
 * 	[1996/02/05  16:08 UTC  truitt  /main/HPDCE02/14]
 *
 * 	Merge changes.
 * 	[1996/02/05  16:03 UTC  truitt  /main/HPDCE02/truitt_dcecp_17281/1]
 *
 * 	CHFts17281: The calls to the internal parse_name function accept
 * 	a global name and return the cell name and the residual name, derived
 * 	from the global name.  Many places where this function is called reset
 * 	the returned cellname to '\0' just before the open site call.  We removed
 * 	the code that resets the cellname so the cell name returned from the
 * 	parse name function is used in the open calls.
 * 	[1996/05/2
 *
 * Revision 1.1.10.3  1996/10/03  17:49:58  arvind
 * 	CHFts19896: public key version support
 * 	[1996/09/13  23:45 UTC  aha  /main/DCE_1.2.2/aha_pk9_3/1]
 * 
 * 	Use new pubkey domain name
 * 	[1996/09/10  21:08 UTC  aha  /main/DCE_1.2.2/aha_pk9_2/2]
 * 
 * 	Support key version numbers in sec_pubkey keyfiles
 * 	[1996/09/06  18:20 UTC  aha  /main/DCE_1.2.2/aha_pk9_2/1]
 * 
 * 	Merge Registry support for KDC private key storage
 * 	[1996/07/16  13:50 UTC  aha  /main/DCE_1.2.2/aha_pk6/1]
 * 
 * 	Changes for Public Key Login; link in sec_pubkey (libdce) functions for
 * 	reading public key keyfiles.
 * 	[1996/07/08  17:23 UTC  truitt  /main/DCE_1.2.2/2]
 * 
 * 	Merge changes.
 * 	[1996/07/03  15:59 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/7]
 * 
 * 	Minor variable change.
 * 	[1996/07/03  15:28 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/6]
 * 
 * 	Minor name change.
 * 	[1996/07/03  14:26 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/5]
 * 
 * 	Minor name change.
 * 	[1996/07/02  17:34 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/4]
 * 
 * 	Additional pk work.
 * 	[1996/06/28  13:57 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/3]
 * 
 * 	Continue public key work.
 * 	[1996/06/27  18:53 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/2]
 * 
 * 	Public key back-end work.
 * 	[1996/06/11  19:11 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/1]
 * 
 * 	Some dcecp changes to support Public Key functionality.
 * 	Merge with latest HP fixes and enhancements.
 * 	[1996/03/28  18:49 UTC  truitt  /main/HPDCE02/19]
 * 
 * 	Merge changes.
 * 	[1996/03/28  18:47 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf72/1]
 * 
 * 	CHFts17168: The account, pgo, registry and schema calls all have serious
 * 	problems when called with an argument of " ".  Note they work correctly if the
 * 	argument is {" "}.  Trap the first type of argument as illegal.
 * 	[1996/03/20  21:46 UTC  truitt  /main/HPDCE02/18]
 * 
 * 	Merge changes.
 * 	[1996/03/20  21:45 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf70/1]
 * 
 * 	CHFts17733: Some complication with registry cursors that currently cannot
 * 	be fixed is causing problems for dcecp in some rare occasions.  In order to
 * 	work around this problem until registry cursors are completely fixed, we'll watch
 * 	the output from the api calls that use the rgy cursor and make sure we trap
 * 	and handle the correct errors.
 * 	[1996/03/18  18:28 UTC  truitt  /main/HPDCE02/17]
 * 
 * 	Merge changes.
 * 	[1996/03/18  18:26 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf69/1]
 * 
 * 	CHFts17225: Modify some on-line help messages to match current functionality.
 * 	CHFts17407: Condense some of the sam messages that are duplicates.
 * 	[1996/02/15  18:30 UTC  truitt  /main/HPDCE02/16]
 * 
 * 	Merge brief changes.
 * 	[1996/02/15  18:26 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf62/1]
 * 
 * 	CHFts17451: Remove include of value.h so duplicate macro
 * 	warning does not appear during HP build.  The include is
 * 	required on other platforms.
 * 	[1996/02/06  21:15 UTC  truitt  /main/HPDCE02/15]
 * 
 * 	Merge changes.
 * 	[1996/02/06  21:13 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf60/1]
 * 
 * 	CHFts16677: Make sure the correct error message is displayed
 * 	when an invalid cellname is passed to a 'catalog' operation.
 * 	[1996/02/05  16:08 UTC  truitt  /main/HPDCE02/14]
 * 
 * 	Merge changes.
 * 	[1996/02/05  16:03 UTC  truitt  /main/HPDCE02/truitt_dcecp_17281/1]
 * 
 * 	CHFts17281: The calls to the internal parse_name function accept
 * 	a global name and return the cell name and the residual name, derived
 * 	from the global name.  Many places where this function is called reset
 * 	the returned cellname to '\0' just before the open site call.  We removed
 * 	the code that resets the cellname so the cell name returned from the
 * 	parse name function is used in the open calls.
 * 	[1996/05/24  23:22 UTC  sommerfeld  /main/DCE_1.2.2/1]
 * 
 * Revision 1.1.10.2  1996/08/09  11:49:06  arvind
 * 	Merge Registry support for KDC private key storage
 * 	[1996/07/16  13:50 UTC  aha  /main/DCE_1.2.2/aha_pk6/1]
 * 
 * 	Changes for Public Key Login; link in sec_pubkey (libdce) functions for
 * 	reading public key keyfiles.
 * 	[1996/07/08  17:23 UTC  truitt  /main/DCE_1.2.2/2]
 * 
 * 	Merge changes.
 * 	[1996/07/03  15:59 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/7]
 * 
 * 	Minor variable change.
 * 	[1996/07/03  15:28 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/6]
 * 
 * 	Minor name change.
 * 	[1996/07/03  14:26 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/5]
 * 
 * 	Minor name change.
 * 	[1996/07/02  17:34 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/4]
 * 
 * 	Additional pk work.
 * 	[1996/06/28  13:57 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/3]
 * 
 * 	Continue public key work.
 * 	[1996/06/27  18:53 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/2]
 * 
 * 	Public key back-end work.
 * 	[1996/06/11  19:11 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/1]
 * 
 * 	Some dcecp changes to support Public Key functionality.
 * 	Merge with latest HP fixes and enhancements.
 * 	[1996/03/28  18:49 UTC  truitt  /main/HPDCE02/19]
 * 
 * 	Merge changes.
 * 	[1996/03/28  18:47 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf72/1]
 * 
 * 	CHFts17168: The account, pgo, registry and schema calls all have serious
 * 	problems when called with an argument of " ".  Note they work correctly if the
 * 	argument is {" "}.  Trap the first type of argument as illegal.
 * 	[1996/03/20  21:46 UTC  truitt  /main/HPDCE02/18]
 * 
 * 	Merge changes.
 * 	[1996/03/20  21:45 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf70/1]
 * 
 * 	CHFts17733: Some complication with registry cursors that currently cannot
 * 	be fixed is causing problems for dcecp in some rare occasions.  In order to
 * 	work around this problem until registry cursors are completely fixed, we'll watch
 * 	the output from the api calls that use the rgy cursor and make sure we trap
 * 	and handle the correct errors.
 * 	[1996/03/18  18:28 UTC  truitt  /main/HPDCE02/17]
 * 
 * 	Merge changes.
 * 	[1996/03/18  18:26 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf69/1]
 * 
 * 	CHFts17225: Modify some on-line help messages to match current functionality.
 * 	CHFts17407: Condense some of the sam messages that are duplicates.
 * 	[1996/02/15  18:30 UTC  truitt  /main/HPDCE02/16]
 * 
 * 	Merge brief changes.
 * 	[1996/02/15  18:26 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf62/1]
 * 
 * 	CHFts17451: Remove include of value.h so duplicate macro
 * 	warning does not appear during HP build.  The include is
 * 	required on other platforms.
 * 	[1996/02/06  21:15 UTC  truitt  /main/HPDCE02/15]
 * 
 * 	Merge changes.
 * 	[1996/02/06  21:13 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf60/1]
 * 
 * 	CHFts16677: Make sure the correct error message is displayed
 * 	when an invalid cellname is passed to a 'catalog' operation.
 * 	[1996/02/05  16:08 UTC  truitt  /main/HPDCE02/14]
 * 
 * 	Merge changes.
 * 	[1996/02/05  16:03 UTC  truitt  /main/HPDCE02/truitt_dcecp_17281/1]
 * 
 * 	CHFts17281: The calls to the internal parse_name function accept
 * 	a global name and return the cell name and the residual name, derived
 * 	from the global name.  Many places where this function is called reset
 * 	the returned cellname to '\0' just before the open site call.  We removed
 * 	the code that resets the cellname so the cell name returned from the
 * 	parse name function is used in the open calls.
 * 	[1996/05/24  23:22 UTC  sommerfeld  /main/DCE_1.2.2/1]
 * 
 * Revision 1.1.10.1  1996/06/04  21:41:59  arvind
 * 	Merge u2u changes from mb_u2u
 * 	[1996/05/06  21:57 UTC  burati  /main/DCE_1.2/2]
 * 
 * 	merge u2u work
 * 	[1996/05/02  19:17 UTC  burati  /main/DCE_1.2/mb_u2u/1]
 * 
 * 	u2u work off Mothra, before 1.2 branch is available
 * 	[1996/04/29  21:50 UTC  burati  /main/HPDCE02/mb_mothra8/1]
 * 
 * Revision 1.1.8.3  1996/02/18  19:14:15  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:00:36  marty]
 * 
 * Revision 1.1.8.2  1995/12/08  21:22:10  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/13  1995/06/16  18:19 UTC  truitt
 * 	Merge sigint work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts35/1  1995/06/16  18:17 UTC  truitt
 * 	CHFts15115: Reset sigint signal handler after call to interactive
 * 	read password function.
 * 
 * 	HP revision /main/HPDCE02/12  1995/05/15  17:27 UTC  truitt
 * 	Merge org work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts29/1  1995/05/15  17:26 UTC  truitt
 * 	CHFts15220: When showing policy information for an org,
 * 	the effective policy information is not being correctly
 * 	reported for the account life, password life and password
 * 	expiration date parameters under certain conditions.
 * 
 * 	HP revision /main/HPDCE02/11  1995/05/01  19:04 UTC  truitt
 * 	Merge era work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts26/1  1995/05/01  19:03 UTC  truitt
 * 	CHFts14301: Allocate memory for the sec_attr_t structures
 * 	dynamically.
 * 
 * 	HP revision /main/HPDCE02/10  1995/04/05  19:53 UTC  truitt
 * 	Merge schema message work.
 * 
 * 	HP revision /main/HPDCE02/truitt_mothra_chfts24/1  1995/04/05  19:52 UTC  truitt
 * 	CHFts14652: Add improved error message.
 * 
 * 	HP revision /main/HPDCE02/9  1995/03/28  15:05 UTC  jrr
 * 	Make org list and group list more efficient (CR12822).
 * 
 * 	HP revision /main/HPDCE02/jrr_dcecp_cr12822/1  1995/03/28  14:59 UTC  jrr
 * 	Make org list and group list more efficient (CR12822).
 * 
 * 	HP revision /main/HPDCE02/8  1995/03/09  14:34 UTC  truitt
 * 	Check for valid cellname in catalog commands.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts18/1  1995/03/09  14:33 UTC  truitt
 * 	CHFts14388: Fix the check for a proper cell name being passed
 * 	to the catalog command.
 * 
 * 	HP revision /main/HPDCE02/7  1995/03/02  18:45 UTC  truitt
 * 	Merge init changes.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts17/1  1995/03/02  18:44 UTC  truitt
 * 	CHFts14504: Initialize attr_info structure correctly.
 * 
 * 	HP revision /main/HPDCE02/6  1995/01/26  17:58 UTC  truitt
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts05/1  1995/01/26  17:57 UTC  truitt
 * 	Fix a couple free's in the show function.  Also, change
 * 	the way the options are processed in the show function.
 * 
 * 	HP revision /main/HPDCE02/5  1994/12/21  19:30 UTC  truitt
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chk_05/1  1994/12/21  19:29 UTC  truitt
 * 	Back out password visibility work.
 * 
 * 	HP revision /main/HPDCE02/4  1994/12/21  14:40 UTC  truitt
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chk_04/1  1994/12/21  14:39 UTC  truitt
 * 	Add a switch to the account create and the account
 * 	modify commands to allow the passwords to be visible
 * 	on the command line.
 * 
 * 	HP revision /main/HPDCE02/3  1994/11/18  14:58 UTC  truitt
 * 	Merge unintegrated
 * 	[1995/12/08  17:56:35  root]
 * 
 * Revision 1.1.4.1  1994/10/13  17:12:32  kevins
 * 	OT 12579 fixed formating in rgy_org_show
 * 	[1994/10/13  17:11:42  kevins]
 * 
 * 	HP revision /main/HPDCE02/2  1994/11/03  20:35 UTC  truitt
 * 	Fix messages in some of the account commands.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_01/1  1994/11/03  20:33 UTC  truitt
 * 	Fix help messages in the account create, modify, and show commands.
 * 
 * 	HP revision /main/HPDCE02/1  1994/10/20  17:38 UTC  truitt
 * 	Initial HPDCE02 branch.
 * 
 * 	HP revision /main/truitt_era_dcecp/1  1994/10/20  17:29 UTC  truitt
 * 	The open_rgy_site() call in rgy_org_list() and rgy_org_show()
 * 	used site_update when it should have been using site_query.
 * 
 * Revision 1.1.2.14  1994/10/05  18:42:54  truitt
 * 	OT12492: Do not free output structures when call to
 * 	check_era_info() function fails.
 * 	[1994/10/05  18:36:32  truitt]
 * 
 * Revision 1.1.2.13  1994/10/01  00:23:51  melman
 * 	change msg code for -mypwd help
 * 	[1994/10/01  00:23:33  melman]
 * 
 * Revision 1.1.2.12  1994/09/30  15:26:24  truitt
 * 	OT12433: Properly chech the show_era() return status.
 * 	[1994/09/30  15:04:35  truitt]
 * 
 * Revision 1.1.2.11  1994/09/29  18:45:13  truitt
 * 	OT10967: Allow proper display of multiple valued attributes.
 * 	Also, cleaned up some of the 'free' code.
 * 	[1994/09/29  17:32:37  truitt]
 * 
 * Revision 1.1.2.10  1994/09/23  18:34:05  ajayb
 * 	Merged with changes from 1.1.2.9
 * 	[1994/09/23  18:33:57  ajayb]
 * 
 * 	Fixed several memory leaks.
 * 	[1994/09/22  20:39:01  ajayb]
 * 
 * Revision 1.1.2.9  1994/09/20  18:00:28  kevins
 * 	OT 12262 commands that have passwords in them now fail when called in -c mode
 * 	[1994/09/20  17:59:07  kevins]
 * 
 * Revision 1.1.2.8  1994/09/16  15:34:22  ajayb
 * 	rgy_org_show(): Don't print leading newline before ERAs,
 * 	if that is the first thing being displayed.
 * 	[1994/09/15  18:00:54  ajayb]
 * 
 * Revision 1.1.2.7  1994/09/15  13:06:35  kevins
 * 	OT 12197 modified list cell_name aquisition.
 * 	[1994/09/15  13:05:51  kevins]
 * 
 * Revision 1.1.2.6  1994/09/14  15:10:11  kevins
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
 * 	[1994/09/14  15:08:54  kevins]
 * 
 * Revision 1.1.2.5  1994/09/13  22:54:04  rousseau
 * 	Catalog performance changes.
 * 	[1994/09/13  22:53:19  rousseau]
 * 
 * Revision 1.1.2.4  1994/09/13  22:38:57  rousseau
 * 	Catalog performance changes.
 * 	[1994/09/13  22:37:38  rousseau]
 * 
 * Revision 1.1.2.3  1994/09/13  19:12:35  kevins
 * 	OT 11641 list syntax not supported in catalog commands.
 * 	[1994/09/13  18:46:36  kevins]
 * 
 * Revision 1.1.2.2  1994/09/12  17:57:57  kevins
 * 	OT 12076 Added two flags -all and -xattrs to aquire all
 * 	  attribute/policy and era info or to aquire only
 * 	  the attribute information.
 * 	[1994/09/12  13:44:55  kevins]
 * 
 * Revision 1.1.2.1  1994/09/06  16:19:53  kevins
 * 	12050 - Now aquires current login context from login cache.
 * 	11694 - Broke up rgy.c into
 * 	                        rgy_pgo_group.c
 * 	                        rgy_pgo_account.c
 * 	                        rgy.c
 * 	[1994/09/06  15:47:12  kevins]
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
#include <krb5/krb5.h>
#include <krb5/los-proto.h>     /* for krb5_read_password */
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
#include <dce/sec_pubkey.h>
#include <dce/dcepsmmsg.h>

/*
 * This is necessary to find the current global login context. This
 * is defined in util_login.h 
 */
extern dcp_login_context_p_t global_context_list;
extern int skip_era_newline;
extern void setup_signal_handling(void);

#define FREE_ACCT_LABELS() {  \
   free(acctvalid_svc);       \
   free(client_svc);          \
   free(created_svc);         \
   free(description_svc);     \
   free(dupkey_svc);          \
   free(expdate_svc);         \
   free(forwardabletkt_svc);  \
   free(goodsince_svc);       \
   free(group_svc);           \
   free(home_svc);            \
   free(lastchange_svc);      \
   free(organization_svc);    \
   free(postdatedtkt_svc);    \
   free(proxiabletkt_svc);    \
   free(pwdvalid_svc);        \
   free(renewabletkt_svc);    \
   free(server_svc);          \
   free(shell_svc);           \
   free(stdtgtauth_svc);      \
   free(maxtktlife_svc);      \
   free(maxtktrenew_svc);     \
   free(effective_svc);       \
   free(yes_svc);             \
   free(no_svc);              \
   free(usertouser_svc);      \
}


/*
 * FUNCTION: rgy_org_add()
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
int rgy_org_add(ClientData clientData, 
                Tcl_Interp *interp, 
                int argc, 
                char **argv)
{
    error_status_t  result = TCL_OK;
    char            *member = NULL;
    sec_rgy_name_t  global_name;
    sec_rgy_name_t  cell_name;
    sec_rgy_name_t  org_name;
    sec_rgy_name_t  member_name;
    char            **aargv;
    int             aargc = 0;
    char            **margv;
    int             margc = 0;
    int             i = 0;
    int             j = 0;
    boolean32       is_simple;

    dcp_ArgvInfo arg_table[] = {
        {"-member", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_org_add_mem_help},
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
    arg_table[0].dst = (char *)&member;

    if ((result = dcp_ParseArgv(interp, &argc, argv, arg_table, 0)) == TCL_ERROR)
        return TCL_ERROR;

    /*
     * Verify that the user provided an organization name with the command. 
     * If so initialize the pgo name parameter.
     */
    DCP_CHECK_RGY_NO_ARG(org);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    if (!member) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_no_member_name);
    }

    if (Tcl_SplitList(interp, argv[1], &aargc, &aargv) != TCL_OK) {
        return TCL_ERROR;
    }
    else {
        /*
         * Check that we did not receive an empty argument list.
         */
        if ((aargc == 0) && (aargv[0] == NULL)) {
            DCP_SET_MISC_ERROR(dcp_s_null_arg);
        }
    }

    if (Tcl_SplitList(interp, member, &margc, &margv) != TCL_OK) {
        free((char *)aargv);
        return TCL_ERROR;
    }
    else {
        /*
         * Check that we did not receive an empty member list.
         */
        if ((margc == 0) && (margv[0] == NULL)) {
            free((char *)aargv);
            DCP_SET_MISC_ERROR(dcp_s_null_arg);
        }
    }

    if ((is_simple = is_simple_name(aargc, aargv)) == -1) {
        free((char *)aargv);
        free((char *)margv);
        DCP_SET_MISC_ERROR(dcp_s_rgy_full_or_simple);
    }

    for(; i < aargc; i++) {
        /*
         * Initialize the name parameter
         */
        strcpy((char *)global_name, aargv[i]);
        if (dcp_id_parse_name(interp, global_name, cell_name, org_name) != TCL_OK) {
            free((char *)aargv);
            free((char *)margv);
            return TCL_ERROR;
        }

        if (open_rgy_site(interp, cell_name, site_update) != TCL_OK) {
            free((char *)aargv);
            free((char *)margv);
            return TCL_ERROR;
        }

        /* 
         * Cycle through all members and initialize the member
         * parameter 
         */
        for (j = 0; j < margc; j++) {
            strcpy((char *)global_name, margv[j]);
            if (dcp_id_parse_name(interp, global_name, cell_name, member_name) != TCL_OK) {
                free((char *)aargv);
                free((char *)margv);
                return TCL_ERROR;
            }

            sec_rgy_pgo_add_member(rgy_context,
                                   sec_rgy_domain_org,
                                   org_name,
                                   member_name,
                                   &result);
            if (result != sec_rgy_status_ok) {
                free((char *)aargv);
                free((char *)margv);
                DCP_SET_MISC_ERROR(result);
            }
        }
    }
    free((char *)aargv);
    free((char *)margv);
    return TCL_OK;
}


/*
 * FUNCTION: rgy_org_create()
 *
 * OVERVIEW:
 * 
 *
 * INPUTS:
 *  int argc - The number of command line arguments passed into function.
 *  char **argv - The actual arguements.
 *
 * INPUT/OUTPUTS:
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
 */
int rgy_org_create(ClientData clientData, 
                   Tcl_Interp *interp, 
                   int argc, 
                   char **argv)
{
    error_status_t        result = TCL_OK;
    sec_rgy_name_t        global_name;
    sec_rgy_name_t        cell_name;
    sec_rgy_name_t        org_name;
    rgy_attribute_info_t  attr_info;
    char                  **cargv;
    int                   cargc = 0;
    char                  **jargv;
    int                   jargc = 0;
    char                  **targv;
    int                   targc = 0;
    int                   arg_count = 0;
    int                   i;
    boolean32             policy_found = FALSE;
    char                  *attribute = NULL;
    char                  *fullname = NULL;
    char                  *uuid = NULL;
    char                  *orgid = NULL;
    char                  *acctlife = NULL;
    char                  *pwdalpha = NULL;
    char                  *pwdexpdate = NULL;
    char                  *pwdlife = NULL;
    char                  *pwdminlen = NULL;
    char                  *pwdspaces = NULL;
    sec_rgy_pgo_item_t    pgo_item;
    sec_rgy_plcy_t        policy;
    sec_attr_t            *attrs;
    sec_attr_t            *out_attrs;
    unsigned32            num_returned = 0;
    unsigned32            num_left = 0;
    unsigned32            space_avail = 0;
    signed32              failure_index;
    error_status_t        status;
    boolean32             is_simple;

    dcp_ArgvInfo arg_table[] = {
        {"-acctlife", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_org_pol_acctlife_help},
        {"-attribute", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_org_attr_help},
        {"-fullname", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_org_fname_help},
        {"-orgid", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_org_orgid_help},
        {"-pwdalpha", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_org_pol_pwdalph_help},
        {"-pwdexpdate", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_org_pol_pwdexpdt_help},
        {"-pwdlife", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_org_pol_pwdlife_help},
        {"-pwdminlen", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_org_pol_pwdminln_help},
        {"-pwdspaces", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_org_pol_pwdspace_help},
        {"-uuid", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_org_uuid_help},
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
    memset((char *)&pgo_item, 0, sizeof(pgo_item));
    memset((char *)&policy, 0, sizeof(policy));

    pgo_item.unix_num = -1;
    arg_table[0].dst = (char *)&acctlife;
    arg_table[1].dst = (char *)&attribute;
    arg_table[2].dst = (char *)&fullname;
    arg_table[3].dst = (char *)&orgid;
    arg_table[4].dst = (char *)&pwdalpha;
    arg_table[5].dst = (char *)&pwdexpdate;
    arg_table[6].dst = (char *)&pwdlife;
    arg_table[7].dst = (char *)&pwdminlen;
    arg_table[8].dst = (char *)&pwdspaces;
    arg_table[9].dst = (char *)&uuid;

    if ((result = dcp_ParseArgv(interp, &argc, argv, arg_table, 0)) == TCL_ERROR)
        return TCL_ERROR;

    /*
     * Verify that the user provided an organization name with the command. 
     * If so initialize the pgo name parameter.
     */
    DCP_CHECK_RGY_NO_ARG(org);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    if (acctlife != NULL || pwdalpha != NULL || pwdexpdate != NULL ||
        pwdlife != NULL || pwdspaces != NULL || pwdminlen != NULL)
        policy_found = TRUE;

    if ((policy_found || fullname != NULL || uuid != NULL || 
        orgid  != NULL) && attribute != NULL) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_list_and_switches);
    }

    /*
     * Add the fields to the pgo_item. 
     * By default, an org can have concurrent org set. Unless otherwise 
     * prohibited from command line option. 
     */
    pgo_item.flags |= sec_rgy_pgo_projlist_ok;
    pgo_item.quota = -1;
    pgo_item.flags = 0;

    attr_info.domain = sec_rgy_domain_org;
    attr_info.policy = FALSE;
    attr_info.pgo = FALSE;
    attr_info.types = 0;
    attr_info.era_count = 0;
    attr_info.del_count = 0;
    attr_info.modify_type = modify_type_none;
    attr_info.flags = pgo_create;
    attr_info.pgo_item = &pgo_item;
    attr_info.fullname = NULL;

    parse_item_switch_list(NULL, fullname, uuid, orgid, NULL, NULL, 
                           &pgo_item, sec_rgy_domain_org, 
                           &attr_info, &result);
    if (result == dcp_s_rgy_uid_out_of_range) {
        DCP_SET_SEC_ERROR(dcp_s_rgy_orgid_out_of_range,MAXINT);
    }
    DCP_CHECK_SEC_ERROR(result);

    parse_policy_switch_list(acctlife, pwdalpha, pwdexpdate, pwdlife, 
                             pwdminlen, pwdspaces, &policy, &result);
    if (result == dcp_s_rgy_pwdminlen_out_of_range) {
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

    if (parse_attribute_list(interp, attribute, &pgo_item, &policy,
                             &attr_info, attrs) != TCL_OK) {
        FREE_ATTR_PTRS(attr_info.era_count, attrs);
        return TCL_ERROR;
    }
    if (attr_info.policy) {
        policy_found = TRUE;
    }

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
         * Initialize the name parameter
         */
        strcpy((char *)global_name, cargv[i]);
        if (dcp_id_parse_name(interp, global_name, cell_name, org_name) != TCL_OK) {
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
                        sec_rgy_domain_org,
                        org_name,
                        &pgo_item,
                        &result);
        if (result != sec_rgy_status_ok) {
            free((char *)cargv);
            FREE_ATTR_PTRS(attr_info.era_count, attrs);
            free((sec_attr_t *)attrs);
            free((sec_attr_t *)out_attrs);
            DCP_SET_MISC_ERROR(result);
        }
        if (policy_found) {
            policy.passwd_flags&=~rgy_plcy_alpha_found;
            policy.passwd_flags&=~rgy_plcy_spaces_found;
            sec_rgy_plcy_set_info(rgy_context,
                                  org_name,
                                  &policy,
                                  &result);
            if (result != sec_rgy_status_ok) {
                free((char *)cargv);
                FREE_ATTR_PTRS(attr_info.era_count, attrs);
                free((sec_attr_t *)attrs);
                free((sec_attr_t *)out_attrs);
                DCP_SET_MISC_ERROR(result);
            }
        }

        /*
         * Check if any extended attributes were specified during the
         * create.  If so, they are stored in the attrs structure.  Call
         * the proper function to get them processed.
         */
        if (attr_info.era_count > 0) {
            sec_rgy_attr_update(rgy_context,
                                sec_rgy_domain_org,
                                org_name,
                                attr_info.era_count,
                                space_avail,
                                attrs,
                                &num_returned,
                                out_attrs,
                                &num_left,
                                &failure_index,
                                &status);
            if (status != sec_rgy_status_ok) {
                free((char *)cargv);
                FREE_ATTR_PTRS(attr_info.era_count, attrs);
                free((sec_attr_t *)attrs);
                free((sec_attr_t *)out_attrs);
                DCP_SET_MISC_ERROR(status);
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
 * FUNCTION: rgy_org_delete()
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
int rgy_org_delete(ClientData clientData, 
                   Tcl_Interp *interp, 
                   int argc, 
                   char **argv)
{
    error_status_t  result;
    int             oargc;
    char            **oargv;
    int             i = 0;
    sec_rgy_name_t  global_name;
    sec_rgy_name_t  cell_name;
    sec_rgy_name_t  org_name;
    boolean32       is_simple;

    dcp_ArgvInfo arg_table[] = { 
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    DCP_CHECK_RGY_NO_ARG(org);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    if (Tcl_SplitList(interp, argv[1], &oargc, &oargv) == TCL_ERROR) {
        return TCL_ERROR;
    }
    else {
        /*
         * Check that we did not receive an empty argument list.
         */
        if ((oargc == 0) && (oargv[0] == NULL)) {
            DCP_SET_MISC_ERROR(dcp_s_null_arg);
        }
    }

    if ((is_simple = is_simple_name(oargc, oargv)) == -1) {
        free((char *)oargv);
        DCP_SET_MISC_ERROR(dcp_s_rgy_full_or_simple);
    }

    for (; i < oargc; i++) {
        strcpy((char *)global_name, oargv[i]);
        if (dcp_id_parse_name(interp, global_name, cell_name, org_name) != TCL_OK) {
            free((char *)oargv);
            return TCL_ERROR;
        }

        if (open_rgy_site(interp, cell_name, site_update) != TCL_OK) {
            free((char *)oargv);
            return TCL_ERROR;
        }
        sec_rgy_pgo_delete(rgy_context,
                           sec_rgy_domain_org,
                           org_name,
                           &result);
        if (result != sec_rgy_status_ok) {
            free((char *)oargv);
            DCP_SET_MISC_ERROR(result);
        }
    }
    free((char *)oargv);
    return TCL_OK;
}


/*
 * FUNCTION: rgy_org_catalog
 *
 * OVERVIEW:
 * 
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
int rgy_org_catalog(ClientData clientData, 
                    Tcl_Interp *interp, 
                    int argc, 
                    char **argv)
{
    error_status_t      result = TCL_OK;
    sec_rgy_pgo_item_t  pgo_item;
    sec_rgy_cursor_t    item_cursor;
    char                *output;
    sec_rgy_name_t      global_name;
    sec_rgy_name_t      name;
    sec_rgy_name_t      name1;
    boolean32           simplename = FALSE;
    sec_rgy_name_t      cell_name;
    size_t              org_name_size;
    int                 oargc = 0;
    char                **oargv;
    int                 i = 0;
    int                 saved_length;
    int                 bad_rgy_cursor_count = 0;
    char                *expand_global_name;

    dcp_ArgvInfo arg_table[] = { 
        {"-simplename", DCP_ARGV_CONSTANT, (char *)1, NULL,
            dcp_t_rgy_org_cat_simple_help},
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
        if (dcp_general_split_list(interp, argv[1], &oargc, &oargv) != TCL_OK) {
            return TCL_ERROR;
        }
        else {
            /*
             * Check that we did not receive an empty argument list.
             */
            if ((oargc == 0) && (oargv[0] == NULL)) {
                DCP_SET_MISC_ERROR(dcp_s_null_arg);
            }
        }
    }
    else {
        oargc = 1;
        oargv = (char **)malloc(2 * sizeof(char *));
        if(oargv == NULL) {
            DCP_SET_RESULT_CODE(dcp_s_no_memory);
            return TCL_ERROR;
        }
        oargv[0] = (char *)global_name;
        oargv[1] = NULL;
    }

    for (; i < oargc; i++) {
        strcpy((char *)global_name, oargv[i]);
        if (open_rgy_site(interp, global_name, site_query) != TCL_OK) {
            free((char *)oargv);
            DCP_SET_SEC_ERROR(dcp_s_rgy_invalid_cell, global_name);
        }
        strcpy((char *)cell_name,(char *)site_cache[realtime_cache].cell_name);
        if (*oargv[i] != '\0') {
            if (global_name[0] == '/') {
                expand_global_name = expand_cell_name((char *)global_name, &result);
                if (result != rpc_s_incomplete_name) {
                    strcpy((char *)global_name, expand_global_name);
                    free(expand_global_name);
                }
            }
            if (strcmp((char *)global_name, (char *)cell_name) != 0) {
                free((char *)oargv);
                DCP_SET_SEC_ERROR(dcp_s_rgy_invalid_cell, global_name);
            }
        }
        strcpy((char *)name, "");
        strcpy((char *)name1, "");
        sec_rgy_cursor_reset(&item_cursor);

        org_name_size = strlen((char *)cell_name) + sec_rgy_pname_t_size + 1;
        output = (char *)malloc(org_name_size);
        if (output == NULL) {
            free((char *)oargv);
            DCP_SET_RESULT_CODE(dcp_s_no_memory);
            return TCL_ERROR;
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
                                 sec_rgy_domain_org,
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
                free((char *)oargv);
                free(output);
                DCP_SET_MISC_ERROR(result);
            }

            if (!simplename)
                sprintf(output, "%s/%s", cell_name, name1);
            else
                sprintf(output, "%s", name1);
            if (strlen(interp->result))
                Tcl_AppendResult(interp, "\n", NULL);
            Tcl_AppendResult(interp, output, NULL);
        }
        free(output);
    }

    free((char *)oargv);
    return TCL_OK;
}


/*
 * FUNCTION: rgy_org_list()
 *
 * OVERVIEW:
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
 *
 */
int rgy_org_list(ClientData clientData, 
                 Tcl_Interp *interp, 
                 int argc, 
                 char **argv)
{
    error_status_t    result = TCL_OK;
    sec_rgy_cursor_t  item_cursor;
    char              *output;
    sec_rgy_name_t    global_name;
    sec_rgy_name_t    cell_name;
    sec_rgy_name_t    org_name;
    int               largc = 0;
    char              **largv;
    int               i = 0;
    int               j = 0;
    signed32          members_supplied = 0;
    signed32          members_avail = 0;
    signed32          max_members = 1024;
    signed32          total_supplied;
    sec_rgy_member_t  *member_list;
    boolean32         simplename = FALSE;
    boolean32         is_simple;
    size_t            org_name_size = 0;

    dcp_ArgvInfo arg_table[] = { 
        {"-simplename", DCP_ARGV_CONSTANT, (char *)1, NULL,
            dcp_t_rgy_org_list_simple_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    arg_table[0].dst = (char *)&simplename;

    if ((result = dcp_ParseArgv(interp, &argc, argv, arg_table, 0)) == TCL_ERROR)
        return TCL_ERROR;

    DCP_CHECK_RGY_NO_ARG(org);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * Verify that the user provided an organization name with the command. 
     * Also check that no extra arguments were passed in. 
     * If so initialize the pgo name parameter and the namelist.
     */
    if (Tcl_SplitList(interp, argv[1], &largc, &largv) != TCL_OK) {
        return TCL_ERROR;
    }
    else {
        /*
         * Check that we did not receive an empty argument list.
         */
        if ((largc == 0) && (largv[0] == NULL)) {
            DCP_SET_MISC_ERROR(dcp_s_null_arg);
        }
    }

    if ((is_simple = is_simple_name(largc, largv)) == -1) {
        free((char *)largv);
        DCP_SET_MISC_ERROR(dcp_s_rgy_full_or_simple);
    }

    member_list = (sec_rgy_member_t *)malloc(sizeof(sec_rgy_member_t) * max_members);
    if (member_list == NULL) {
        free((char *)largv);
        DCP_SET_RESULT_CODE(dcp_s_no_memory);
        return TCL_ERROR;
    }

    for (; i < largc; i++) {
        strcpy((char *)global_name, largv[i]);
        if (dcp_id_parse_name(interp, global_name, cell_name, org_name) != TCL_OK) {
            free((char *)largv);
            free(member_list);
            return TCL_ERROR;
        }

        if (open_rgy_site(interp, cell_name, site_query) != TCL_OK) {
            free((char *)largv);
            free(member_list);
            return TCL_ERROR;
        }

        sec_rgy_cursor_reset(&item_cursor);
        total_supplied = 0;
        do {
            sec_rgy_pgo_get_members(rgy_context,
                                    sec_rgy_domain_org,
                                    org_name,
                                    &item_cursor,
                                    max_members,
                                    member_list,
                                    &members_supplied,
                                    &members_avail,
                                    &result);
            if (result != sec_rgy_status_ok) {
                free((char *)largv);
                free(member_list);
                DCP_SET_MISC_ERROR(result);
            }

            if ((strlen(interp->result)) && (total_supplied ==0)) {
                Tcl_AppendResult(interp,"\n",NULL);
            }

            org_name_size = strlen((char *)cell_name) + sec_rgy_pname_t_size + 1;
            output = (char *)malloc(org_name_size);
            if (output == NULL) {
                free((char *)largv);
                free(member_list);
                DCP_SET_MISC_ERROR(dcp_s_no_memory);
            }
            total_supplied += members_supplied;

            for (j = 0; j <  members_supplied; j++) {
                if (!simplename)
                    sprintf(output, "%s/%s", cell_name, member_list[j]);
                else
                    sprintf(output, "%s", member_list[j]);
                if (strlen(interp->result))
                    Tcl_AppendResult(interp, "\n", NULL);
                Tcl_AppendResult(interp, output, NULL);
            }
            free(output);
        } while (members_avail > total_supplied);
    }
    free((char *)largv);
    free(member_list);
    return TCL_OK;
}


/*
 * FUNCTION: rgy_org_modify()
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
int rgy_org_modify(ClientData clientData,
                   Tcl_Interp *interp,
                   int argc,
                   char **argv)
{
    error_status_t        result = TCL_OK;
    sec_rgy_name_t        global_name;
    sec_rgy_name_t        cell_name;
    sec_rgy_name_t        org_name;
    rgy_attribute_info_t  attr_info;
    char                  *attribute = NULL;
    char                  *attribute1 = NULL;
    char                  *attribute2 = NULL;
    char                  *attribute3 = NULL;
    boolean32             org_switches = FALSE;
    boolean32             policy_switches = FALSE;
    char                  *acctlife = NULL;
    char                  *pwdalpha = NULL;
    char                  *pwdexpdate = NULL;
    char                  *pwdlife = NULL;
    char                  *pwdminlen = NULL;
    char                  *pwdspaces = NULL;
    char                  *fullname = NULL;
    char                  *types = NULL;
    sec_rgy_cursor_t      item_cursor;
    sec_rgy_pgo_item_t    pgo_item;
    sec_rgy_pgo_item_t    temp_pgo_item;
    sec_rgy_plcy_t        policy;
    sec_rgy_plcy_t        temp_policy;
    int                   era_count_hold = 0;
    char                  **oargv;
    int                   oargc;
    char                  **jargv;
    int                   jargc = 0;
    char                  **targv;
    int                   targc = 0;
    int                   arg_count = 0;
    int                   i, j;
    sec_attr_t            *attrs;
    sec_attr_t            *out_attrs;
    sec_attr_t            *del_attrs;
    sec_attr_t            *tmp_attrs;
    unsigned32            num_returned = 0;
    unsigned32            num_left = 0;
    unsigned32            space_avail = 0;
    signed32              failure_index;
    error_status_t        status;
    boolean32             is_simple;
    unsigned32            cur_num_attrs;
    sec_attr_cursor_t     attr_cursor;

    /*
     * Set up the parser argument table and initialize the variables.
     */
    dcp_ArgvInfo arg_table[] = { 
        {"-acctlife", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_org_pol_acctlife_help},
        {"-add", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_org_mod_add_help},
        {"-change", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_org_mod_change_help},
        {"-fullname", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_org_fname_help},
        {"-pwdalpha", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_org_pol_pwdalph_help},
        {"-pwdexpdate", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_org_pol_pwdexpdt_help},
        {"-pwdlife", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_org_pol_pwdlife_help},
        {"-pwdminlen", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_org_pol_pwdminln_help},
        {"-pwdspaces", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_org_pol_pwdspace_help},
        {"-remove", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_org_mod_remove_help},
        {"-types", DCP_ARGV_CONSTANT, (char *)1, NULL,
            dcp_t_rgy_org_mod_types_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}     
    };

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    arg_table[0].dst = (char *)&acctlife;
    arg_table[1].dst = (char *)&attribute1;
    arg_table[2].dst = (char *)&attribute2;
    arg_table[3].dst = (char *)&fullname;
    arg_table[4].dst = (char *)&pwdalpha;
    arg_table[5].dst = (char *)&pwdexpdate;
    arg_table[6].dst = (char *)&pwdlife;
    arg_table[7].dst = (char *)&pwdminlen;
    arg_table[8].dst = (char *)&pwdspaces;
    arg_table[9].dst = (char *)&attribute3;
    arg_table[10].dst = (char *)&types;
    
    if ((result = dcp_ParseArgv(interp, &argc, argv, arg_table, 0)) == TCL_ERROR)
        return TCL_ERROR;

    /* 
     * The user cannot both promote and demote an org in the same 
     * operation. Also verify that an org name has been supplied.
     */
    DCP_CHECK_RGY_NO_ARG(org);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * The user cannot specify combinations of add, change, and remove
     * on the same command line.  Only one at a time is allowed.
     */
    if ((attribute1 != NULL && attribute2 != NULL) ||
        (attribute1 != NULL && attribute3 != NULL) ||
        (attribute2 != NULL && attribute3 != NULL)) {
       DCP_SET_MISC_ERROR(dcp_s_rgy_era_illegal_switches);
    }

    if (pwdalpha != NULL || pwdexpdate != NULL || pwdlife != NULL || 
        pwdminlen != NULL || pwdspaces != NULL || acctlife != NULL)
        policy_switches = TRUE;

    if (fullname != NULL)
        org_switches = TRUE;

    /*
     * The user also cannot specify an attribute list and a variety of
     * switches.  (ie "org modify foo -acctlife 10 -change {name foob}"
     * is illegal).
     */
    if ((policy_switches || org_switches) && (attribute1 != NULL ||
        attribute2 != NULL || attribute3 != NULL)) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_list_and_switches);
    }

    /*
     * If the user didn't specify any type of modify command,
     * then this is an error.
     */
    if (pwdalpha == NULL && pwdexpdate == NULL && pwdlife == NULL &&
        pwdminlen == NULL && pwdspaces == NULL && acctlife == NULL &&
        fullname == NULL && attribute1 == NULL && attribute2 == NULL &&
        attribute3 == NULL) {
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
     * Perform the necessary operation. For the attributes the only allowed
     * changes are for the name. Initialize the temporary pgo_item and 
     * policy_item.
     */
    init_pgo_item(&temp_pgo_item);
    init_policy(&temp_policy);

    attr_info.domain = sec_rgy_domain_org;
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

    /*
     * process the command line switches. 
     */
    parse_item_switch_list(NULL, fullname, NULL, NULL, NULL, NULL, &temp_pgo_item,
                           sec_rgy_domain_org, &attr_info, &result);
    DCP_CHECK_SEC_ERROR(result);

    parse_policy_switch_list(acctlife, pwdalpha, pwdexpdate, pwdlife, pwdminlen,
                             pwdspaces, &temp_policy, &result);
    if (result == dcp_s_rgy_pwdminlen_out_of_range) {
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
    if (parse_attribute_list(interp, attribute, &temp_pgo_item, &temp_policy,
                             &attr_info, attrs) != TCL_OK) {
        FREE_ATTR_PTRS(attr_info.era_count, attrs);
        free((sec_attr_t *)attrs);
        free((sec_attr_t *)tmp_attrs);
        return TCL_ERROR;
    }

    if (attr_info.policy)
        policy_switches = TRUE;
    if (attr_info.pgo)
        org_switches = TRUE;

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

    for (j = 0; j < oargc; j++) {
        /*
         * Copy the org name and attempt to aquire it from the 
         * registry. 
         */
        strcpy((char *)global_name, oargv[j]);
        if (dcp_id_parse_name(interp, global_name, cell_name, org_name) != TCL_OK) {
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
                                sec_rgy_domain_org,
                                org_name,
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
        sec_rgy_plcy_get_info(rgy_context,
                              org_name,
                              &policy,
                              &result);
        if (result == sec_rgy_object_not_found) {
            memset((char *)&policy, 0, sizeof(policy));
            result = error_status_ok;
        }
        if (result != sec_rgy_status_ok) {
            free((char *)oargv);
            FREE_ATTR_PTRS(attr_info.era_count, attrs);
            free((sec_attr_t *)attrs);
            free((sec_attr_t *)tmp_attrs);
            DCP_SET_MISC_ERROR(result);
        }

        if (org_switches) {
            copy_pgo_item_with_restrict(&pgo_item, &temp_pgo_item, &attr_info);
            sec_rgy_pgo_replace(rgy_context,
                                sec_rgy_domain_org,
                                org_name,
                                &pgo_item,
                                (error_status_t *)&result);
            if (result != sec_rgy_status_ok) {
                free((char *)oargv);
                FREE_ATTR_PTRS(attr_info.era_count, attrs);
                free((sec_attr_t *)attrs);
                free((sec_attr_t *)tmp_attrs);
                DCP_SET_MISC_ERROR(result);
            }
        }
        
        if (policy_switches) {
            copy_policy_with_restrict(&policy, &temp_policy);
            sec_rgy_plcy_set_info(rgy_context,
                                  org_name,
                                  &policy, 
                                  (error_status_t *)&result);
            if (result != sec_rgy_status_ok) {
                free((char *)oargv);
                FREE_ATTR_PTRS(attr_info.era_count, attrs);
                free((sec_attr_t *)attrs);
                free((sec_attr_t *)tmp_attrs);
                DCP_SET_MISC_ERROR(result);
            }
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
                                     sec_rgy_domain_org,
                                     org_name,
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
             * of the extended attributes a requriement.
             */
            result = check_era_info(interp,
                                    sec_rgy_domain_org,
                                    org_name,
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
                                    sec_rgy_domain_org,
                                    org_name,
                                    attr_info.era_count,
                                    space_avail,
                                    out_attrs,
                                    &num_returned,
                                    tmp_attrs,
                                    &num_left,
                                    &failure_index,
                                    &status);
                if (status != sec_rgy_status_ok) {
                    free((char *)oargv);
                    FREE_ATTR_PTRS(era_count_hold, attrs);
                    FREE_ATTR_PTRS(attr_info.era_count, out_attrs);
                    FREE_ATTR_PTRS(attr_info.del_count, del_attrs);
                    free((sec_attr_t *)attrs);
                    free((sec_attr_t *)tmp_attrs);
                    free((sec_attr_t *)out_attrs);
                    free((sec_attr_t *)del_attrs);
                    DCP_SET_MISC_ERROR(status);
                }
            }

            if (attr_info.del_count > 0) {
                /*
                 * The del_attrs structure is filled with information
                 * to remove from the database, so use the remove function.
                 */
                sec_rgy_attr_delete(rgy_context,
                                    sec_rgy_domain_org,
                                    org_name,
                                    attr_info.del_count,
                                    del_attrs,
                                    &failure_index,
                                    &status);
                if (status != sec_rgy_status_ok) {
                    free((char *)oargv);
                    FREE_ATTR_PTRS(era_count_hold, attrs);
                    FREE_ATTR_PTRS(attr_info.era_count, out_attrs);
                    FREE_ATTR_PTRS(attr_info.del_count, del_attrs);
                    free((sec_attr_t *)attrs);
                    free((sec_attr_t *)tmp_attrs);
                    free((sec_attr_t *)out_attrs);
                    free((sec_attr_t *)del_attrs);
                    DCP_SET_MISC_ERROR(status);
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
 * FUNCTION: rgy_org_remove()
 *
 * OVERVIEW:
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
int rgy_org_remove(ClientData clientData, 
                   Tcl_Interp *interp, 
                   int argc, 
                   char **argv)
{
    error_status_t  result = TCL_OK;
    char            *member = NULL;
    sec_rgy_name_t  global_name;
    sec_rgy_name_t  cell_name;
    sec_rgy_name_t  org_name;
    sec_rgy_name_t  member_name;
    char            **aargv;
    int             aargc = 0;
    char            **margv;
    int             margc = 0;
    int             i = 0;
    int             j = 0;
    boolean32       is_simple;

    dcp_ArgvInfo arg_table[] = {
        {"-member", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_org_remove_mem_help},
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
    arg_table[0].dst = (char *)&member;

    if ((result = dcp_ParseArgv(interp, &argc, argv, arg_table, 0)) == TCL_ERROR)
        return TCL_ERROR;

    /*
     * Verify that the user provided an organization name with the command. 
     * Also check that no extra arguments were passed in. 
     * If so initialize the pgo name parameter and the namelist.
     */
    DCP_CHECK_RGY_NO_ARG(org);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    if (!member) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_no_member_name);
    }

    /* 
     * Split both the member list and the list of objects (orgs) to 
     * operate on. 
     */
    if (Tcl_SplitList(interp, argv[1], &aargc, &aargv) != TCL_OK) {
        return TCL_ERROR;
    }
    else {
        /*
         * Check that we did not receive an empty argument list.
         */
        if ((aargc == 0) && (aargv[0] == NULL)) {
            DCP_SET_MISC_ERROR(dcp_s_null_arg);
        }
    }

    if (Tcl_SplitList(interp, member, &margc, &margv) != TCL_OK) {
        free((char *)aargv);
        return TCL_ERROR;
    }
    else {
        /*
         * Check that we did not receive an empty member list.
         */
        if ((margc == 0) && (margv[0] == NULL)) {
            free((char *)aargv);
            DCP_SET_MISC_ERROR(dcp_s_null_arg);
        }
    }

    if ((is_simple = is_simple_name(aargc, aargv)) == -1) {
        free((char *)aargv);
        free((char *)margv);
        DCP_SET_MISC_ERROR(dcp_s_rgy_full_or_simple);
    }

    for (; i < aargc; i++) {
        /*
         * Initialize the name parameter
         */
        strcpy((char *)global_name, aargv[i]);
        if (dcp_id_parse_name(interp, global_name, cell_name, org_name) != TCL_OK) {
            free((char *)aargv);
            free((char *)margv);
            return TCL_ERROR;
        }

        if (open_rgy_site(interp, cell_name, site_update) != TCL_OK) {
            free((char *)aargv);
            free((char *)margv);
            return TCL_ERROR;
        }

        /*
         * Cycle through all members and initialize the member
         * parameter 
         */
        for (j = 0; j < margc; j++) {
            strcpy((char *)global_name, margv[j]);
            if (dcp_id_parse_name(interp, global_name, cell_name, member_name) != TCL_OK) {
                free((char *)aargv);
                free((char *)margv);
                return TCL_ERROR;
            }

            sec_rgy_pgo_delete_member(rgy_context,
                                      sec_rgy_domain_org,
                                      org_name,
                                      member_name,
                                      &result);
            if (result != sec_rgy_status_ok) {
                free((char *)aargv);
                free((char *)margv);
                DCP_SET_MISC_ERROR(result);
            }
        }
    }
    free((char *)aargv);
    free((char *)margv);
    return TCL_OK;
}


/*
 * FUNCTION: rgy_org_rename
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
int rgy_org_rename(ClientData clientData, 
                   Tcl_Interp *interp, 
                   int argc, 
                   char **argv)
{
    error_status_t  result = TCL_OK;
    char            *new_name_ptr = NULL;
    sec_rgy_name_t  global_name;
    sec_rgy_name_t  cell_name;
    sec_rgy_name_t  org_name;
    sec_rgy_name_t  new_name;
    boolean32       is_simple;

    dcp_ArgvInfo arg_table[] = {
        {"-to", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_org_rename_to_help},
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
     * Set $n to "" . If the correct number of args have been supplied
     * set the namelist accordingly.
     */
    DCP_CHECK_RGY_NO_ARG(org);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    if (new_name_ptr == NULL) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_no_name_to_rename);
    }

    result = error_status_ok;
    strcpy((char *)global_name, argv[1]);
    is_simple = is_simple_name(1, &argv[1]);
    if (dcp_id_parse_name(interp, global_name, cell_name, org_name) != TCL_OK)
        return TCL_ERROR;

    if (open_rgy_site(interp, cell_name, site_update) != TCL_OK)
        return TCL_ERROR;

    strcpy((char *)global_name, new_name_ptr);
    if (dcp_id_parse_name(interp, global_name, cell_name, new_name) != TCL_OK)
        return TCL_ERROR;

    sec_rgy_pgo_rename(rgy_context, 
                       sec_rgy_domain_org, 
                       org_name,
                       new_name, 
                       &result);
    DCP_CHECK_SEC_ERROR(result);
    return TCL_OK;
}


/*
 * FUNCTION: rgy_org_show
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
int rgy_org_show(ClientData clientData, 
                 Tcl_Interp *interp, 
                 int argc, 
                 char **argv)
{
    int                 result = TCL_OK;
    error_status_t      status;
    sec_rgy_name_t      global_name;
    sec_rgy_name_t      cell_name;
    sec_rgy_name_t      org_name;
    char                *output;
    char                *orgid_svc = NULL;      /* unix id */
    char                *uuid_svc = NULL;       /* uuid value */
    char                *fullname_svc = NULL;   /* administrative data */
    char                *acclife_svc = NULL;    /* account lifetime */
    char                *pwdminlen_svc = NULL;  /* passwd min length */
    char                *pwdlife_svc = NULL;    /* pwdlifetime    */
    char                *expdate_svc = NULL;    /* psswd expiration */
    char                *spaces_svc = NULL;     /* passwd spaces ? */
    char                *alphan_svc = NULL;     /* passwd alphanumeric ?*/
    char                *unlimited_svc = NULL;  /* unlimited label */
    char                *none_svc;
    char                *yes_svc;
    char                *no_svc;
    char                acclife[1024];
    char                pwdlife[1024];
    char                pwdminlen[1024];
    char                spaces[1024];
    char                alphan[1024];
    char                expdate[1024];
    char                *effective_svc = NULL;
    char                effect_date[UTC_MAX_STR_LEN];
    char                actual_date[UTC_MAX_STR_LEN];
    sec_rgy_pgo_item_t  pgo_item;
    sec_rgy_cursor_t    item_cursor;
    sec_rgy_plcy_t      effective_policy;
    sec_rgy_plcy_t      policy;
    int                 oargc;
    char                **oargv;
    boolean32           effective = FALSE;
    boolean32           all = FALSE;
    char                uuid[UUID_STRING_LEN];
    int                 i = 0;
    int                 output_size = 0;
    boolean32           is_simple;
    boolean32           xattrs = FALSE;

    dcp_ArgvInfo arg_table[] = {
        {"-all", DCP_ARGV_CONSTANT, (char *)1, NULL,
            dcp_t_rgy_org_show_all_help},
        {"-policies", DCP_ARGV_CONSTANT, (char *)1, NULL,
            dcp_t_rgy_org_show_pol_help},
        {"-xattrs", DCP_ARGV_CONSTANT, (char *)1, NULL,
            dcp_t_rgy_org_show_era_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    arg_table[0].dst = (char *)&all;
    arg_table[1].dst = (char *)&effective;
    arg_table[2].dst = (char *)&xattrs;

    if ((result = dcp_ParseArgv(interp, &argc, argv, arg_table, 0)) == TCL_ERROR)
        return TCL_ERROR;

    /*
     * Check that the user has specified the options correctly.
     * The all options logically should not be used with the others.
     */
    if ((all == TRUE) && ((effective == TRUE) || (xattrs == TRUE))) {
        DCP_SET_MISC_ERROR(dcp_s_mut_excl_opts);
    }

    /* 
     * Terminate the command if not enough arguments are passed in. 
     * Set $n to "" . If the correct number of args have been supplied
     * set the namelist accordingly.
     */
    DCP_CHECK_RGY_NO_ARG(org);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * The user is allowed to specify a list of orgs to show.
     */
    if (Tcl_SplitList(interp, argv[1], &oargc, &oargv) == TCL_ERROR) {
        return TCL_ERROR;
    }
    else {
        /*
         * Check that we did not receive an empty argument list.
         */
        if ((oargc == 0) && (oargv[0] == NULL)) {
            DCP_SET_MISC_ERROR(dcp_s_null_arg);
        }
    }

    if ((is_simple = is_simple_name(oargc, oargv)) == -1) {
        free((char *)oargv);
        DCP_SET_MISC_ERROR(dcp_s_rgy_full_or_simple);
    }

    output_size = sec_rgy_pname_t_size + sec_rgy_name_t_size + UUID_STRING_LEN + 256;

    /*
     * Set up the field labels.
     */
    if ((effective == FALSE) && (xattrs == FALSE)) {
        orgid_svc = (char *)dce_sprintf(dcp_t_rgy_orgid);
        uuid_svc = (char *)dce_sprintf(dcp_t_rgy_uuid);
        fullname_svc = (char *)dce_sprintf(dcp_t_rgy_fullname);
        if (orgid_svc == NULL || uuid_svc == NULL || fullname_svc == NULL) {
            free((char *)oargv);
            DCP_SET_MISC_ERROR(dcp_s_no_memory);
        }
    }
    if ((effective == TRUE) || (all == TRUE)) {
        acclife_svc = (char *)dce_sprintf(dcp_t_rgy_acctlife);
        pwdminlen_svc = (char *)dce_sprintf(dcp_t_rgy_pwdminlen);
        pwdlife_svc = (char *)dce_sprintf(dcp_t_rgy_pwdlife);
        expdate_svc = (char *)dce_sprintf(dcp_t_rgy_pwdexpdate);
        spaces_svc = (char *)dce_sprintf(dcp_t_rgy_pwdspaces);
        alphan_svc = (char *)dce_sprintf(dcp_t_rgy_pwdalpha);
        effective_svc = (char *)dce_sprintf(dcp_t_rgy_effective);
        unlimited_svc = (char *)dce_sprintf(dcp_t_rgy_unlimited);
        none_svc = (char *)dce_sprintf(dcp_t_rgy_none);
        yes_svc = (char *)dce_sprintf(dcp_t_rgy_yes);
        no_svc = (char *)dce_sprintf(dcp_t_rgy_no);
        if (acclife_svc == NULL || pwdminlen_svc == NULL || pwdlife_svc == NULL ||
            expdate_svc == NULL || spaces_svc == NULL || alphan_svc == NULL ||
            effective_svc == NULL || unlimited_svc == NULL || none_svc == NULL ||
            yes_svc == NULL || no_svc == NULL) {
            free((char *)oargv);
            if (all == TRUE) {
                free(orgid_svc);
                free(uuid_svc);
                free(fullname_svc);
            }
            DCP_SET_MISC_ERROR(dcp_s_no_memory);
        }
    }

    /* 
     * Cycle through each of the specified org names and show 
     * all of the available information. 
     */
    for (i = 0; i < oargc; i++) {
        if (strlen(interp->result))
            Tcl_AppendResult(interp, "\n\n", NULL);
        strcpy((char *)global_name, oargv[i]);
        if (dcp_id_parse_name(interp, global_name, cell_name, org_name) != TCL_OK) {
            free((char *)oargv);
            if ((effective == FALSE) && (xattrs == FALSE)) {
                free(orgid_svc);
                free(uuid_svc);
                free(fullname_svc);
            }
            if ((effective == TRUE) || (all == TRUE)) {
                free(acclife_svc);
                free(pwdminlen_svc);
                free(pwdlife_svc);
                free(expdate_svc);
                free(spaces_svc);
                free(alphan_svc);
                free(effective_svc);
                free(unlimited_svc);
                free(none_svc);
                free(yes_svc);
                free(no_svc);
            }
            return TCL_ERROR;
        }

        if (open_rgy_site(interp, cell_name, site_query) != TCL_OK) {
            free((char *)oargv);
            if ((effective == FALSE) && (xattrs == FALSE)) {
                free(orgid_svc);
                free(uuid_svc);
                free(fullname_svc);
            }
            if ((effective == TRUE) || (all == TRUE)) {
                free(acclife_svc);
                free(pwdminlen_svc);
                free(pwdlife_svc);
                free(expdate_svc);
                free(spaces_svc);
                free(alphan_svc);
                free(effective_svc);
                free(unlimited_svc);
                free(none_svc);
                free(yes_svc);
                free(no_svc);
            }
            return TCL_ERROR;
        }

        if ((effective == FALSE) && (xattrs == FALSE)) {
            sec_rgy_pgo_get_by_name(rgy_context, 
                                    sec_rgy_domain_org, 
                                    org_name,
                                    &item_cursor, 
                                    &pgo_item, 
                                    &status);
            if (status != sec_rgy_status_ok) {
                free((char *)oargv);
                free(orgid_svc);
                free(uuid_svc);
                free(fullname_svc);
                if (all == TRUE) {
                    free(acclife_svc);
                    free(pwdminlen_svc);
                    free(pwdlife_svc);
                    free(expdate_svc);
                    free(spaces_svc);
                    free(alphan_svc);
                    free(effective_svc);
                    free(unlimited_svc);
                    free(none_svc);
                    free(yes_svc);
                    free(no_svc);
                }
                DCP_SET_MISC_ERROR(status);
            }
            status = decode_uuid((uuid_t *)&pgo_item.id, uuid);

            /* 
             * Set up the output buffer.
             */
            output = (char *)malloc(output_size);
            if (!output) {
                free((char *)oargv);
                free(orgid_svc);
                free(uuid_svc);
                free(fullname_svc);
                if (all == TRUE) {
                    free(acclife_svc);
                    free(pwdminlen_svc);
                    free(pwdlife_svc);
                    free(expdate_svc);
                    free(spaces_svc);
                    free(alphan_svc);
                    free(effective_svc);
                    free(unlimited_svc);
                    free(none_svc);
                    free(yes_svc);
                    free(no_svc);
                }
                DCP_SET_MISC_ERROR(dcp_s_no_memory);
            }

            /* 
             * Generate the output buffer. Install it in the interp result.
             */
            if (strlen((char *)pgo_item.fullname))
                Tcl_AppendResult(interp, "{", fullname_svc, " {",
                                 pgo_item.fullname, "}}\n", NULL);
            else
                Tcl_AppendResult(interp, "{", fullname_svc, " {}}\n", NULL);
            sprintf(output, "{%s %d}\n{%s %s}", orgid_svc, pgo_item.unix_num,
                    uuid_svc, uuid);
            Tcl_AppendResult(interp, output, NULL);
            free(output);
        }
        if ((xattrs == TRUE) || (all == TRUE)) {
            /*
             * Display any era information associated with this name.
             */
            if (!all)
                skip_era_newline = 1;
            status = show_era(interp, sec_rgy_domain_org, org_name);
            if ((status != sec_rgy_status_ok) && (status != rpc_s_unknown_if) &&
                (status != ept_s_not_registered)) {
                free((char *)oargv);
                if (all == TRUE) {
                    free(acclife_svc);
                    free(pwdminlen_svc);
                    free(pwdlife_svc);
                    free(expdate_svc);
                    free(spaces_svc);
                    free(alphan_svc);
                    free(effective_svc);
                    free(unlimited_svc);
                    free(none_svc);
                    free(yes_svc);
                    free(no_svc);
                }
                DCP_SET_MISC_ERROR(status);
            }
        }
        if ((effective == TRUE) || (all == TRUE)) {
            if (strlen(interp->result) && (xattrs || all))
                Tcl_AppendResult(interp,"\n",NULL);

            sec_rgy_plcy_get_effective(rgy_context, 
                                       org_name,
                                       &effective_policy, 
                                       &status);
            if (status != sec_rgy_status_ok) {
                free((char *)oargv);
                if (all == TRUE) {
                    free(orgid_svc);
                    free(uuid_svc);
                    free(fullname_svc);
                }
                free(acclife_svc);
                free(pwdminlen_svc);
                free(pwdlife_svc);
                free(expdate_svc);
                free(spaces_svc);
                free(alphan_svc);
                free(effective_svc);
                free(unlimited_svc);
                free(none_svc);
                free(yes_svc);
                free(no_svc);
                DCP_SET_MISC_ERROR(status);
            }

            sec_rgy_plcy_get_info(rgy_context,
                                  org_name,
                                  &policy,
                                  &status);
            if (status == sec_rgy_object_not_found) {
                Tcl_AppendResult(interp, (char *)dce_sprintf(dcp_t_rgy_no_org_policy), NULL);
                continue;
            }
            if (status != sec_rgy_status_ok) {
                free((char *)oargv);
                if (all == TRUE) {
                    free(orgid_svc);
                    free(uuid_svc);
                    free(fullname_svc);
                }
                free(acclife_svc);
                free(pwdminlen_svc);
                free(pwdlife_svc);
                free(expdate_svc);
                free(spaces_svc);
                free(alphan_svc);
                free(effective_svc);
                free(unlimited_svc);
                free(none_svc);
                free(yes_svc);
                free(no_svc);
                DCP_SET_MISC_ERROR(status);
            }

            /*
             * Account lifetime field.
             */
            if (effective_policy.acct_lifespan < policy.acct_lifespan ||
                (policy.acct_lifespan == org_pol_live_forever && 
                effective_policy.acct_lifespan != org_pol_live_forever)) {
                sec_to_rel_date_string(effective_policy.acct_lifespan, effect_date);
                if (policy.acct_lifespan == org_pol_live_forever) {
                    sprintf(acclife, "{%s %s %s %s}\n", acclife_svc, unlimited_svc,
                            effective_svc, effect_date);
                }
                else {
                    sec_to_rel_date_string(policy.acct_lifespan, actual_date);
                    sprintf(acclife, "{%s %s %s %s}\n", acclife_svc, actual_date,
                            effective_svc, effect_date);
                }
            }
            else if (policy.acct_lifespan == org_pol_live_forever)
                sprintf(acclife, "{%s %s}\n", acclife_svc, unlimited_svc);
            else {
                sec_to_rel_date_string(policy.acct_lifespan, actual_date);
                sprintf(acclife, "{%s %s}\n", acclife_svc, actual_date);
            }

            /*
             * Password lifetime field.
             */
            if (effective_policy.passwd_lifetime < policy.passwd_lifetime ||
                (policy.passwd_lifetime == org_pol_live_forever && 
                effective_policy.passwd_lifetime != org_pol_live_forever)) {
                sec_to_rel_date_string(effective_policy.passwd_lifetime, effect_date);
                if (policy.passwd_lifetime == org_pol_live_forever) {
                    sprintf(pwdlife, "{%s %s %s %s}\n", pwdlife_svc, unlimited_svc,
                            effective_svc, effect_date);
                }
                else {
                    sec_to_rel_date_string(policy.passwd_lifetime, actual_date);
                    sprintf(pwdlife, "{%s %s %s %s}\n", pwdlife_svc, actual_date,
                            effective_svc, effect_date);
                }
            }
            else if (policy.passwd_lifetime == org_pol_live_forever)
                sprintf(pwdlife, "{%s %s}\n", pwdlife_svc, unlimited_svc);
            else {
                sec_to_rel_date_string(policy.passwd_lifetime, actual_date);
                sprintf(pwdlife, "{%s %s}\n", pwdlife_svc, actual_date);
            }

            /*
             * Password minimum length field.
             */
            if (effective_policy.passwd_min_len > policy.passwd_min_len) {
                sprintf(pwdminlen, "{%s %d %s %d}\n", pwdminlen_svc, policy.passwd_min_len,
                        effective_svc, effective_policy.passwd_min_len);
            }
            else 
                sprintf(pwdminlen, "{%s %d}\n", pwdminlen_svc, policy.passwd_min_len);

            /*
             * Password expiration date field.
             */
            if (effective_policy.passwd_exp_date != org_pol_never_expire)
                sec_to_date_string(effective_policy.passwd_exp_date, effect_date);

            if (policy.passwd_exp_date != org_pol_never_expire)
                sec_to_date_string(policy.passwd_exp_date, actual_date);

            if (effective_policy.passwd_exp_date < policy.passwd_exp_date) {
                sprintf(expdate, "{%s %s %s %s}\n", expdate_svc, actual_date,
                        effective_svc, effect_date);
            }
            else if ((policy.passwd_exp_date == org_pol_never_expire) &&
                     (effective_policy.passwd_exp_date != org_pol_never_expire)) {
                sprintf(expdate, "{%s %s %s %s}\n", expdate_svc, none_svc,
                        effective_svc, effect_date);
            }
            else {
                if (policy.passwd_exp_date == org_pol_never_expire)
                    sprintf(expdate, "{%s %s}\n", expdate_svc, none_svc);
                else 
                    sprintf(expdate, "{%s %s}\n", expdate_svc, actual_date);
            }

            /*
             * Password spaces allowed field.
             */
            if ((effective_policy.passwd_flags
                & sec_rgy_plcy_pwd_no_spaces) == sec_rgy_plcy_pwd_no_spaces && 
                (policy.passwd_flags & sec_rgy_plcy_pwd_no_spaces) != sec_rgy_plcy_pwd_no_spaces)
                sprintf(spaces, "{%s %s %s %s}", spaces_svc, yes_svc, effective_svc, no_svc);
            else if ((policy.passwd_flags & sec_rgy_plcy_pwd_no_spaces) ==
                    sec_rgy_plcy_pwd_no_spaces)
                sprintf(spaces, "{%s %s}", spaces_svc, no_svc);
            else
                sprintf(spaces, "{%s %s}", spaces_svc, yes_svc);

            /*
             * Password alpha character field.
             */
            if ((effective_policy.passwd_flags & sec_rgy_plcy_pwd_non_alpha) ==
                sec_rgy_plcy_pwd_non_alpha && 
                (policy.passwd_flags & sec_rgy_plcy_pwd_non_alpha) !=
                sec_rgy_plcy_pwd_non_alpha)
                sprintf(alphan, "{%s %s %s %s}\n", alphan_svc, yes_svc,
                        effective_svc, no_svc);
            else if ((policy.passwd_flags & sec_rgy_plcy_pwd_non_alpha) ==
                     sec_rgy_plcy_pwd_non_alpha)
                sprintf(alphan, "{%s %s}\n", alphan_svc, no_svc);
            else 
                sprintf(alphan, "{%s %s}\n", alphan_svc, yes_svc);

            Tcl_AppendResult(interp, acclife, alphan, expdate, pwdlife, pwdminlen,
                             spaces, NULL);
        }
    }

    /* 
     * Free allocated memory.
     */
    free((char *)oargv);
    if ((effective == FALSE) && (xattrs == FALSE)) {
        free(fullname_svc);
        free(uuid_svc);
        free(orgid_svc);
    }
    if ((effective == TRUE) || (all == TRUE)) {
        free(acclife_svc);
        free(pwdminlen_svc);
        free(pwdlife_svc);
        free(expdate_svc);
        free(spaces_svc);
        free(unlimited_svc);
        free(none_svc);
        free(yes_svc);
        free(no_svc);
    }
    return TCL_OK;
}


/*
 * FUNCTION: rgy_account_show
 *
 * OVERVIEW: This function shows the the current attributes of the 
 *            specified account. This takes two switches -policies to show the 
 *            policy attributes of the account or -all to show all 
 *            attributes. 
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
 *  int  either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 *    
 */
int rgy_account_show(ClientData clientData, 
                     Tcl_Interp *interp, 
                     int argc, 
                     char **argv)
{
    unsigned32            result = TCL_OK;
    boolean32             effective = FALSE;
    boolean32             all = FALSE;
    sec_rgy_name_t        global_name;
    sec_rgy_name_t        cell_name;
    sec_rgy_cursor_t      cursor;
    sec_rgy_login_name_t  key_name;
    sec_rgy_login_name_t  account_name;
    sec_rgy_sid_t         sid;
    sec_rgy_unix_sid_t    unix_sid;
    sec_rgy_acct_key_t    acct_key;
    sec_rgy_acct_user_t   acct_user;
    sec_rgy_acct_admin_t  acct_admin;
    sec_rgy_plcy_auth_t   policy;
    sec_rgy_plcy_auth_t   default_policy;
    char                  *acctvalid_svc = NULL;
    char                  *client_svc = NULL;
    char                  *created_svc = NULL;
    char                  *description_svc = NULL;
    char                  *dupkey_svc = NULL;
    char                  *expdate_svc = NULL;
    char                  *forwardabletkt_svc = NULL;
    char                  *goodsince_svc = NULL;
    char                  *group_svc = NULL;
    char                  *home_svc = NULL;
    char                  *lastchange_svc = NULL;
    char                  *organization_svc = NULL;
    char                  *postdatedtkt_svc = NULL;
    char                  *proxiabletkt_svc = NULL;
    char                  *pwdvalid_svc = NULL;
    char                  *renewabletkt_svc = NULL;
    char                  *server_svc = NULL;
    char                  *shell_svc = NULL;
    char                  *stdtgtauth_svc = NULL;
    char                  *maxtktlife_svc = NULL;
    char                  *maxtktrenew_svc = NULL;
    char                  *effective_svc = NULL;
    char                  *no_svc = NULL;
    char                  *yes_svc = NULL;
    char                  *none_svc = NULL;
    char                  *usertouser_svc = NULL;
    int                   oargc;
    char                  **oargv;
    int                   i; 
    char                  dt_str[UTC_MAX_STR_LEN];
    char                  default_dt_str[UTC_MAX_STR_LEN];
    sec_rgy_name_t        principal;
    sec_rgy_name_t        cell;
    sec_rgy_name_t        group;
    sec_rgy_name_t        org;
    boolean32             is_simple;
    
    dcp_ArgvInfo arg_table[] = {
        {"-all", DCP_ARGV_CONSTANT, (char *)1, NULL,
            dcp_t_rgy_acct_show_all_help},
        {"-policies", DCP_ARGV_CONSTANT, (char *)1, NULL,
            dcp_t_rgy_acct_show_pol_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    arg_table[0].dst = (char *)&all;
    arg_table[1].dst = (char *)&effective;

    if ((result = dcp_ParseArgv(interp, &argc, argv, arg_table, 0)) == TCL_ERROR)
        return TCL_ERROR;

    /*
     * Terminate the command if not enough arguments are passed in. 
     * Set $n to "" . If the correct number of args have been supplied
     * set the namelist accordingly.
     */
    DCP_CHECK_RGY_NO_ARG(acct);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * The user is allowed to specify a list of accounts to show.
     */
    if (Tcl_SplitList(interp, argv[1], &oargc, &oargv) == TCL_ERROR) {
        return TCL_ERROR; 
    }
    else {
        /*
         * Check that we did not receive an empty argument list.
         */
        if ((oargc == 0) && (oargv[0] == NULL)) {
            DCP_SET_MISC_ERROR(dcp_s_null_arg);
        }
    }

    if ((is_simple = is_simple_name(oargc, oargv)) == -1) {
        free((char *)oargv);
        DCP_SET_MISC_ERROR(dcp_s_rgy_full_or_simple);
    }

    acctvalid_svc = (char *)dce_sprintf(dcp_t_rgy_acctvalid);
    client_svc = (char *)dce_sprintf(dcp_t_rgy_client);
    created_svc = (char *)dce_sprintf(dcp_t_rgy_created);
    description_svc = (char *)dce_sprintf(dcp_t_rgy_description);
    dupkey_svc = (char *)dce_sprintf(dcp_t_rgy_dupkey);
    expdate_svc = (char *)dce_sprintf(dcp_t_rgy_expdate);
    forwardabletkt_svc = (char *)dce_sprintf(dcp_t_rgy_forwardabletkt);
    goodsince_svc = (char *)dce_sprintf(dcp_t_rgy_goodsince);
    group_svc = (char *)dce_sprintf(dcp_t_rgy_group);
    home_svc = (char *)dce_sprintf(dcp_t_rgy_home);
    lastchange_svc = (char *)dce_sprintf(dcp_t_rgy_lastchange);
    organization_svc = (char *)dce_sprintf(dcp_t_rgy_organization);
    postdatedtkt_svc = (char *)dce_sprintf(dcp_t_rgy_postdatedtkt);
    proxiabletkt_svc = (char *)dce_sprintf(dcp_t_rgy_proxiabletkt);
    pwdvalid_svc = (char *)dce_sprintf(dcp_t_rgy_pwdvalid);
    renewabletkt_svc = (char *)dce_sprintf(dcp_t_rgy_renewabletkt);
    server_svc = (char *)dce_sprintf(dcp_t_rgy_server);
    shell_svc = (char *)dce_sprintf(dcp_t_rgy_shell);
    stdtgtauth_svc = (char *)dce_sprintf(dcp_t_rgy_stdtgtauth);
    maxtktlife_svc = (char *)dce_sprintf(dcp_t_rgy_maxtktlife);
    maxtktrenew_svc = (char *)dce_sprintf(dcp_t_rgy_maxtktrenew);
    effective_svc = (char *)dce_sprintf(dcp_t_rgy_effective);
    yes_svc = (char *)dce_sprintf(dcp_t_rgy_yes);
    no_svc = (char *)dce_sprintf(dcp_t_rgy_no);
    usertouser_svc = (char *)dce_sprintf(dcp_t_rgy_usertouser);
    
    if (!acctvalid_svc || !client_svc || !created_svc || !description_svc || !dupkey_svc ||
        !expdate_svc || !forwardabletkt_svc || !goodsince_svc || !group_svc || !home_svc ||
        !lastchange_svc || !organization_svc || !postdatedtkt_svc || !proxiabletkt_svc ||
        !pwdvalid_svc || !renewabletkt_svc || !server_svc || !shell_svc || !server_svc ||
        !stdtgtauth_svc || !maxtktlife_svc || !maxtktrenew_svc || !no_svc ||
        !yes_svc || !effective_svc || !usertouser_svc) {
        free((char *)oargv);
        DCP_SET_MISC_ERROR(dcp_s_no_memory);
    }

    for (i = 0; i < oargc; i++) {
        strcpy((char *)global_name, oargv[i]);
        if (dcp_id_parse_name(interp, global_name, cell_name, key_name.pname) != TCL_OK) {
            free((char *)oargv);
            FREE_ACCT_LABELS();
            return TCL_ERROR;
        }

        if (open_rgy_site(interp, cell_name, site_query) != TCL_OK) {
            free((char *)oargv);
            FREE_ACCT_LABELS();
            return TCL_ERROR;
        }
        strcpy((char *)key_name.gname,"\0");
        strcpy((char *)key_name.oname,"\0");;
        sec_rgy_cursor_reset(&cursor);
        sec_rgy_acct_lookup(rgy_context,
                            &key_name,
                            &cursor,
                            &account_name,
                            &sid,
                            &unix_sid,
                            &acct_key,
                            &acct_user,
                            &acct_admin,
                            &result);
        if (result != sec_rgy_status_ok) {
            free((char *)oargv);
            FREE_ACCT_LABELS();
            DCP_SET_MISC_ERROR(result);
        }

        if (all || !effective) {
            if (strlen(interp->result))
                Tcl_AppendResult(interp,"\n\n",NULL);  

            if (acct_admin.flags & sec_rgy_acct_admin_valid)
                Tcl_AppendResult(interp, "{", acctvalid_svc, " ", yes_svc, "}\n", NULL);
            else
                Tcl_AppendResult(interp, "{", acctvalid_svc, " ", no_svc, "}\n", NULL);

            if (acct_admin.flags & sec_rgy_acct_admin_client)
                Tcl_AppendResult(interp, "{", client_svc, " ", yes_svc, "}\n", NULL);
            else
                Tcl_AppendResult(interp, "{", client_svc, " ", no_svc, "}\n", NULL);

            sec_id_gen_name(rgy_context,
                            &(acct_admin.creator.cell),
                            &(acct_admin.creator.principal),
                            NULL,
                            cell,
                            principal,
                            &result);
            if (result != sec_rgy_status_ok) {
                free((char *)oargv);
                FREE_ACCT_LABELS();
                DCP_SET_MISC_ERROR(result);
            }
            dt_str[0] = '\0';
            sec_to_date_string(acct_admin.creation_date, dt_str);
            Tcl_AppendResult(interp, "{", created_svc, " ", cell, "/", principal, " ",
                             dt_str, "}\n", NULL);

            if (strlen((char *)acct_user.gecos))
                Tcl_AppendResult(interp, "{", description_svc, " {", acct_user.gecos,
                                 "}}\n", NULL);
            else
                Tcl_AppendResult(interp, "{", description_svc, " {}}\n", NULL);

            if (acct_admin.authentication_flags & sec_rgy_acct_auth_dup_skey)
                Tcl_AppendResult(interp, "{", dupkey_svc, " ", yes_svc, "}\n", NULL);
            else
                Tcl_AppendResult(interp, "{", dupkey_svc, " ", no_svc, "}\n", NULL);

            dt_str[0] = '\0';
            if (!acct_admin.expiration_date) {
                none_svc = (char *)dce_sprintf(dcp_t_rgy_none);
                strcpy(dt_str, none_svc);
                free(none_svc);
            }
            else
                sec_to_date_string(acct_admin.expiration_date, dt_str);
            if (!strlen(dt_str))
                strcpy(dt_str, "{}");
            Tcl_AppendResult(interp, "{", expdate_svc, " ", dt_str, "}\n", NULL);

            if (acct_admin.authentication_flags & sec_rgy_acct_auth_forwardable)
                Tcl_AppendResult(interp, "{", forwardabletkt_svc, " ", yes_svc, "}\n", NULL);
            else
                Tcl_AppendResult(interp, "{", forwardabletkt_svc, " ", no_svc, "}\n", NULL);

            dt_str[0] = '\0';
            sec_to_date_string(acct_admin.good_since_date, dt_str);
            if (!strlen(dt_str))
                strcpy(dt_str, "{}");
            Tcl_AppendResult(interp, "{", goodsince_svc, " ", dt_str, "}\n", NULL);

            sec_rgy_pgo_id_to_name(rgy_context,
                                   sec_rgy_domain_group,
                                   &(sid.group),
                                   group,
                                   &result);
            if (result != sec_rgy_status_ok) {
                free((char *)oargv);
                FREE_ACCT_LABELS();
                DCP_SET_MISC_ERROR(result);
            }
            Tcl_AppendResult(interp, "{", group_svc, " ", group, "}\n", NULL);

            if (strlen((char *)acct_user.homedir))
                Tcl_AppendResult(interp, "{", home_svc, " ", acct_user.homedir, "}\n", NULL);
            else
                Tcl_AppendResult(interp, "{", home_svc, " {}}\n", NULL);

            sec_id_gen_name(rgy_context,
                            &(acct_admin.last_changer.cell),
                            &(acct_admin.last_changer.principal),
                            NULL,
                            cell,
                            principal,
                            &result);
            if (result != sec_rgy_status_ok) {
                free((char *)oargv);
                FREE_ACCT_LABELS();
                DCP_SET_MISC_ERROR(result);
            }
            dt_str[0] = '\0';
            sec_to_date_string(acct_admin.change_date, dt_str);
            Tcl_AppendResult(interp, "{", lastchange_svc, " ", cell, "/", principal,
                             " ", dt_str, "}\n", NULL);

            sec_rgy_pgo_id_to_name(rgy_context,
                                   sec_rgy_domain_org,
                                   &(sid.org),
                                   org,
                                   &result);
            if (result != sec_rgy_status_ok) {
                free((char *)oargv);
                FREE_ACCT_LABELS();
                DCP_SET_MISC_ERROR(result);
            }
            Tcl_AppendResult(interp, "{", organization_svc, " ", org, "}\n", NULL);

            if (acct_admin.authentication_flags & sec_rgy_acct_auth_post_dated)
                Tcl_AppendResult(interp, "{", postdatedtkt_svc, " ", yes_svc, "}\n", NULL);
            else
                Tcl_AppendResult(interp, "{", postdatedtkt_svc, " ", no_svc, "}\n", NULL);

            if (acct_admin.authentication_flags & sec_rgy_acct_auth_proxiable)
                Tcl_AppendResult(interp, "{", proxiabletkt_svc, " ", yes_svc, "}\n", NULL);
            else
                Tcl_AppendResult(interp, "{", proxiabletkt_svc, " ", no_svc, "}\n", NULL);

            if (acct_user.flags & sec_rgy_acct_user_passwd_valid)
                Tcl_AppendResult(interp, "{", pwdvalid_svc, " ", yes_svc, "}\n", NULL);
            else
                Tcl_AppendResult(interp, "{", pwdvalid_svc, " ", no_svc, "}\n", NULL);

            if (acct_admin.authentication_flags & sec_rgy_acct_auth_renewable)
                Tcl_AppendResult(interp, "{", renewabletkt_svc, " ", yes_svc, "}\n", NULL);
            else
                Tcl_AppendResult(interp, "{", renewabletkt_svc, " ", no_svc, "}\n", NULL);

            if (acct_admin.flags & sec_rgy_acct_admin_server)
                Tcl_AppendResult(interp, "{", server_svc, " ", yes_svc, "}\n", NULL);
            else
                Tcl_AppendResult(interp, "{", server_svc, " ", no_svc, "}\n", NULL);

            if (strlen((char *)acct_user.shell))
                Tcl_AppendResult(interp, "{", shell_svc, " ", acct_user.shell, "}\n", NULL);
            else
                Tcl_AppendResult(interp, "{", shell_svc, " {}}\n", NULL);

            if (acct_admin.authentication_flags & sec_rgy_acct_auth_tgt)
                Tcl_AppendResult(interp, "{", stdtgtauth_svc, " ", yes_svc, "}\n", NULL);
            else
                Tcl_AppendResult(interp, "{", stdtgtauth_svc, " ", no_svc, "}\n", NULL);

            if (acct_admin.authentication_flags & sec_rgy_acct_auth_user_to_user)
                Tcl_AppendResult(interp, "{", usertouser_svc, " ", yes_svc, "}", NULL);
            else
                Tcl_AppendResult(interp, "{", usertouser_svc, " ", no_svc, "}", NULL);
        }

        if (all || effective) {
            key_name.pname[0] = '\0';
            sec_rgy_auth_plcy_get_effective(rgy_context,
                                            &key_name,
                                            &default_policy,
                                            &result);
            if (result != sec_rgy_status_ok) {
                free((char *)oargv);
                FREE_ACCT_LABELS();
                DCP_SET_MISC_ERROR(result);
            }

            sec_rgy_auth_plcy_get_info(rgy_context,
                                       &account_name,
                                       &policy,
                                       &result);
            if (strlen(interp->result)) {
                if (effective && !all)
                    Tcl_AppendResult(interp, "\n\n", NULL); 
                else
                    Tcl_AppendResult(interp, "\n", NULL); 
            }
            if (result == sec_rgy_object_not_found) {
                Tcl_AppendResult(interp, (char *)dce_sprintf(dcp_t_rgy_no_org_policy), NULL);
                result = error_status_ok;
            }
            else {
                if (result != sec_rgy_status_ok) {
                    free((char *)oargv);
                    FREE_ACCT_LABELS();
                    DCP_SET_MISC_ERROR(result);
                }

                sec_to_rel_date_string(policy.max_ticket_lifetime, dt_str);
                sec_to_rel_date_string(default_policy.max_ticket_lifetime, default_dt_str);
                if (policy.max_ticket_lifetime > default_policy.max_ticket_lifetime)
                    Tcl_AppendResult(interp, "{", maxtktlife_svc, " ", dt_str, " ",
                                     effective_svc, " ", default_dt_str, "}\n", NULL);
                else  
                    Tcl_AppendResult(interp, "{", maxtktlife_svc, " ", dt_str, "}\n", NULL);

                sec_to_rel_date_string(policy.max_renewable_lifetime, dt_str);
                sec_to_rel_date_string(default_policy.max_renewable_lifetime, default_dt_str);
                if (policy.max_renewable_lifetime > default_policy.max_renewable_lifetime)
                    Tcl_AppendResult(interp, "{", maxtktrenew_svc, " ", dt_str, " ",
                                     effective_svc, " ", default_dt_str, "}", NULL);
                else          
                    Tcl_AppendResult(interp, "{", maxtktrenew_svc, " ", dt_str, "}", NULL);
            }
        }
    }   
    free((char *)oargv);
    FREE_ACCT_LABELS();
    return TCL_OK;
}


/*
 * FUNCTION: rgy_account_create
 *
 * OVERVIEW: This function creates the specified account. 
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
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int  either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 *    
 */
int rgy_account_create(ClientData clientData, 
                       Tcl_Interp *interp, 
                       int argc, 
                       char **argv)
{
    unsigned32             result = TCL_OK;
    rgy_acct_info_t        account;
    rgy_pubkey_info_t      pkdattrs;
    rgy_pubkey_info_t      pkcattrs;
    char                   *attribute = NULL;
    char                   *pkdigital = NULL;
    char                   *pkcipher = NULL;
    char                   **cargv;
    int                    cargc = 0;
    int                    i = 0;
    sec_rgy_name_t         global_name;
    sec_rgy_name_t         cell_name;
    int                    passwd_len;
    sec_login_handle_t     login_context;
    unsigned32             rls_mypwd_mem = 0;
    boolean32              is_simple;
    extern boolean32       check_visible_pw;
    error_status_t         status;
    sec_psm_handle_t       psm_handle;
    sec_pk_domain_t        domain_id;
    sec_pk_algorithm_id_t  *alg_id;
    sec_pk_gen_data_t      *seed = NULL;
    sec_pk_usage_flags_t   usages;
    sec_pk_pubkey_t        pubkey_d;
    sec_pk_pvtkey_t        pvtkey_d;
    sec_pk_pubkey_t        pubkey_c;
    sec_pk_pvtkey_t        pvtkey_c;

    dcp_ArgvInfo arg_table[] = {
        {"-acctvalid", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_valid_help},
        {"-attribute", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_attribute_help},
        {"-client", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_client_help},
        {"-description", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_descr_help},
        {"-dupkey", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_dupkey_help},
        {"-expdate", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_expdate_help},
        {"-forwardabletkt", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_forward_help},
        {"-goodsince", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_goodsince_help}, 
        {"-group", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_group_help},
        {"-home", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_home_help},
        {"-maxtktlife", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_tktlife_help},
        {"-maxtktrenew", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_tktrenew_help},
        {"-mypwd", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_mypwd_help},
        {"-organization", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_org_help},
        {"-password", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_passwd_help},
        {"-pkkeycipherusage", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_pkcipher_help},
        {"-pkmechanism", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_pkmechanism_help},
        {"-pksignatureusage", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_pkdigital_help},
        {"-postdatedtkt", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_posttkt_help},
        {"-proxiabletkt", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_proxtkt_help},
        {"-pwdvalid", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_pwdvalid_help},
        {"-renewabletkt", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_renewtkt_help},
        {"-server", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_server_help},
        {"-shell" ,DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_shell_help},
        {"-stdtgtauth", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_auth_help},
        {"-usertouser", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_usertouser_help},
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
    memset(&account,0,sizeof(account));
    arg_table[0].dst = (char *)&account.acctvalid;
    arg_table[1].dst = (char *)&attribute;
    arg_table[2].dst = (char *)&account.client;
    arg_table[3].dst = (char *)&account.description;
    arg_table[4].dst = (char *)&account.dupkey;
    arg_table[5].dst = (char *)&account.expdate;
    arg_table[6].dst = (char *)&account.forwardabletkt;
    arg_table[7].dst = (char *)&account.goodsince;
    arg_table[8].dst = (char *)&account.group;
    arg_table[9].dst = (char *)&account.home;
    arg_table[10].dst = (char *)&account.maxtktlife;
    arg_table[11].dst = (char *)&account.maxtktrenew;
    arg_table[12].dst = (char *)&account.mypwd;
    arg_table[13].dst = (char *)&account.organization;
    arg_table[14].dst = (char *)&account.password;
    arg_table[15].dst = (char *)&pkcipher;
    arg_table[16].dst = (char *)&account.pkmechanism;
    arg_table[17].dst = (char *)&pkdigital;
    arg_table[18].dst = (char *)&account.postdatedtkt;
    arg_table[19].dst = (char *)&account.proxiabletkt;
    arg_table[20].dst = (char *)&account.pwdvalid;
    arg_table[21].dst = (char *)&account.renewabletkt;
    arg_table[22].dst = (char *)&account.server;
    arg_table[23].dst = (char *)&account.shell;
    arg_table[24].dst = (char *)&account.stdtgtauth;
    arg_table[25].dst = (char *)&account.usertouser;

    if ((result = dcp_ParseArgv(interp, &argc, argv, arg_table, 0)) == TCL_ERROR)
        return TCL_ERROR;

    /*
     * Verify that the user provided an account name with the command. 
     * If so, initialize the pgo name parameter.
     */
    DCP_CHECK_RGY_NO_ARG(acct);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * Check to see if attribute list and switches were provided.
     */
    if ((account.acctvalid || account.client || account.description || account.dupkey ||
        account.expdate || account.forwardabletkt || account.goodsince || account.group ||
        account.home || account.mypwd || account.organization || account.password ||
        account.postdatedtkt || account.proxiabletkt || account.pwdvalid ||
        account.renewabletkt || account.server || account.shell || account.stdtgtauth ||
        account.maxtktlife || account.maxtktrenew || account.pkmechanism ||
        account.usertouser || pkdigital || pkcipher) && attribute) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_list_and_switches);
    }

    if (Tcl_SplitList(interp, argv[1], &cargc, &cargv) == TCL_ERROR) {
        return TCL_ERROR;
    }
    else {
        /*
         * Check that we did not receive an empty argument list.
         */
        if ((cargc == 0) && (cargv[0] == NULL)) {
            DCP_SET_MISC_ERROR(dcp_s_null_arg);
        }
    }

    if ((is_simple = is_simple_name(cargc, cargv)) == -1) {
        free((char *)cargv);
        DCP_SET_MISC_ERROR(dcp_s_rgy_full_or_simple);
    }

    initialize_account_attr(&account);
    initialize_pk_attr(&pkdattrs);
    initialize_pk_attr(&pkcattrs);
    pubkey_d.data = NULL;
    pvtkey_d.data = NULL;
    pubkey_c.data = NULL;
    pvtkey_c.data = NULL;
    pubkey_d.len = 0;
    pvtkey_d.len = 0;
    pubkey_c.len = 0;
    pvtkey_c.len = 0;

    /*
     * Parse the attribute list if one is supplied. 
     * Simply fill out the account structure and parse_account_switches 
     * will fill out the account structures accordingly.
     */
    if (attribute) {
        pkdigital = (char *)malloc(strlen(attribute) * sizeof(char *));
        pkcipher = (char *)malloc(strlen(attribute) * sizeof(char *));
        memset((char *)pkdigital, 0, sizeof(pkdigital));
        memset((char *)pkcipher, 0, sizeof(pkcipher));
        result = parse_account_attributes(interp, &account, pkdigital, pkcipher, attribute);
        if (result != error_status_ok) {
            free((char *)cargv);
            FREE_ACCT_PTRS(&account);
            if (pkdigital)
                free((char *)pkdigital);
            if (pkcipher)
                free((char *)pkcipher);
            return TCL_ERROR;
        }
    }

    result = parse_account_switches(interp, &account);
    if (result != error_status_ok) {
        free((char *)cargv);
        FREE_ACCT_PTRS(&account);
        return TCL_ERROR;
    }

    result = parse_pk_switches(interp, &pkdattrs, &pkcattrs, pkdigital, pkcipher);
    if (result != error_status_ok) {
        free((char *)cargv);
        FREE_ACCT_PTRS(&account);
        FREE_PK_PTRS(&pkdattrs);
        FREE_PK_PTRS(&pkcattrs);
        return TCL_ERROR;
    }

    /*
     * Check pksignatureusage parameters.
     */
    if ((pkdigital != NULL) && (strlen(pkdigital) > 0)) {
        result = check_pk_create_arguments(interp, &pkdattrs, "signature");
        if (result != error_status_ok) {
            free((char *)cargv);
            FREE_ACCT_PTRS(&account);
            FREE_PK_PTRS(&pkdattrs);
            FREE_PK_PTRS(&pkcattrs);
            return TCL_ERROR;
        }
    }

    /*
     * Check pkkeycipherusage parameters.
     */
    if ((pkcipher != NULL) && (strlen(pkcipher) > 0)) {
        result = check_pk_create_arguments(interp, &pkcattrs, "key encryption");
        if (result != error_status_ok) {
            free((char *)cargv);
            FREE_ACCT_PTRS(&account);
            FREE_PK_PTRS(&pkdattrs);
            FREE_PK_PTRS(&pkcattrs);
            return TCL_ERROR;
        }
    }

    /*
     * Check the consistency of the two newpassphrases.
     */
    if ((pkdattrs.newpassphrase) && (pkcattrs.newpassphrase)) {
        if (strcmp(pkdattrs.newpassphrase, pkcattrs.newpassphrase) != 0) {
            DCP_SET_RESULT_CODE(dcp_s_rgy_pk_inconsistent_passphrase);
            return TCL_ERROR;
        }
    }

    /*
     * If any public key files were specified, check that the
     * files can be opened and read.  This allows dcecp to do
     * some basic checking of the user files before the security
     * api call is made.
     */
    if ((pkdattrs.publickeyfile) || (pkdattrs.privatekeyfile)) {
        result = check_pk_file_contents(interp, &pkdattrs);
        if (result != error_status_ok) {
            free((char *)cargv);
            FREE_ACCT_PTRS(&account);
            FREE_PK_PTRS(&pkdattrs);
            FREE_PK_PTRS(&pkcattrs);
            return TCL_ERROR;
        }
    }
    if ((pkcattrs.publickeyfile) || (pkcattrs.privatekeyfile)) {
        result = check_pk_file_contents(interp, &pkcattrs);
        if (result != error_status_ok) {
            free((char *)cargv);
            FREE_ACCT_PTRS(&account);
            FREE_PK_PTRS(&pkdattrs);
            FREE_PK_PTRS(&pkcattrs);
            return TCL_ERROR;
        }
    }

    /*
     * If public or private key information was specified via file, the
     * sec_pk_data_t structure (idl_byte and length) must be used.
     */
    if (pkdattrs.publickeyfile) {
        status = sec_pubkey_read_keyfile((idl_char *)pkdattrs.publickeyfile, NULL, NULL, &pubkey_d);
        if (status != error_status_ok) {
            free((char *)cargv);
            FREE_ACCT_PTRS(&account);
            FREE_PK_PTRS(&pkdattrs);
            FREE_PK_PTRS(&pkcattrs);
            DCP_SET_MISC_ERROR(status);
        }
    }

    if (pkdattrs.privatekeyfile) {
        status = sec_pubkey_read_keyfile((idl_char *)pkdattrs.privatekeyfile, NULL, NULL, &pvtkey_d);
        if (status != error_status_ok) {
            free((char *)cargv);
            FREE_ACCT_PTRS(&account);
            FREE_PK_PTRS(&pkdattrs);
            FREE_PK_PTRS(&pkcattrs);
            DCP_SET_MISC_ERROR(status);
        }
    }

    if (pkcattrs.publickeyfile) {
        status = sec_pubkey_read_keyfile((idl_char *)pkcattrs.publickeyfile, NULL, NULL, &pubkey_c);
        if (status != error_status_ok) {
            free((char *)cargv);
            FREE_ACCT_PTRS(&account);
            FREE_PK_PTRS(&pkdattrs);
            FREE_PK_PTRS(&pkcattrs);
            DCP_SET_MISC_ERROR(status);
        }
    }

    if (pkcattrs.privatekeyfile) {
        status = sec_pubkey_read_keyfile((idl_char *)pkcattrs.privatekeyfile, NULL, NULL, &pvtkey_c);
        if (status != error_status_ok) {
            free((char *)cargv);
            FREE_ACCT_PTRS(&account);
            FREE_PK_PTRS(&pkdattrs);
            FREE_PK_PTRS(&pkcattrs);
            DCP_SET_MISC_ERROR(status);
        }
    }

    /*
     * Check for basic required parameters.
     */
    if (!account.group) {
        free((char *)cargv);
        FREE_ACCT_PTRS(&account);
        FREE_PK_PTRS(&pkdattrs);
        FREE_PK_PTRS(&pkcattrs);
        DCP_SET_MISC_ERROR(dcp_s_rgy_acct_no_group);
    }
    if (!account.organization) {
        free((char *)cargv);
        FREE_ACCT_PTRS(&account);
        FREE_PK_PTRS(&pkdattrs);
        FREE_PK_PTRS(&pkcattrs);
        DCP_SET_MISC_ERROR(dcp_s_rgy_acct_no_org);
    }

    if (!account.password) {
        account.password = (char *)malloc(sec_passwd_str_max_len);
        passwd_len = sec_passwd_str_max_len;
        if (!account.password) {
            free((char *)cargv);
            FREE_ACCT_PTRS(&account);
            FREE_PK_PTRS(&pkdattrs);
            FREE_PK_PTRS(&pkcattrs);
            DCP_SET_MISC_ERROR(dcp_s_no_memory);
        }
        memset(account.password, 0, sec_passwd_str_max_len - 1);
        if (krb5_read_password("Enter Account Password: ",
                               "Again: ",
                               (char *)account.password,
                               &passwd_len) != 0) {
            free((char *)cargv);
            setup_signal_handling();
            FREE_ACCT_PTRS(&account);
            FREE_PK_PTRS(&pkdattrs);
            FREE_PK_PTRS(&pkcattrs);
            DCP_SET_MISC_ERROR(dcp_s_rgy_acct_no_passwd);
        }
        setup_signal_handling();
        account.passwd.key.tagged_union.plain = (unsigned char *)account.password;
    }
    else if (check_visible_pw) {
        /*
         * If this is being called from the command line it's an error.
         */
        free((char *)cargv);
        FREE_ACCT_PTRS(&account);
        FREE_PK_PTRS(&pkdattrs);
        FREE_PK_PTRS(&pkcattrs);
        DCP_SET_MISC_ERROR(dcp_s_illegal_command);
    }

    if ((account.password) && !(account.mypwd)) {
        account.mypwd = (char *)malloc(sec_passwd_str_max_len);
        passwd_len = sec_passwd_str_max_len;
        if (!account.mypwd) {
            free((char *)cargv);
            FREE_ACCT_PTRS(&account);
            FREE_PK_PTRS(&pkdattrs);
            FREE_PK_PTRS(&pkcattrs);
            DCP_SET_MISC_ERROR(dcp_s_no_memory);
        }
        rls_mypwd_mem = 1;
        memset(account.mypwd, 0, sec_passwd_str_max_len - 1);
        if (krb5_read_password("Enter Your Password: ",
                               NULL,
                               (char *)account.mypwd,
                               &passwd_len) != 0) {
            free((char *)cargv);
            setup_signal_handling();
            FREE_ACCT_PTRS(&account);
            FREE_PK_PTRS(&pkdattrs);
            FREE_PK_PTRS(&pkcattrs);
            DCP_SET_RESULT_CODE(dcp_s_login_bad_pw_read);
            return TCL_ERROR;
        }
        setup_signal_handling();
        account.adm_passwd.key.key_type = sec_passwd_plain;
        account.adm_passwd.key.tagged_union.plain = (unsigned char *)account.mypwd;
        account.adm_passwd.version_number = sec_passwd_c_version_none;
    }
    else if (check_visible_pw) {
        /*
         * If this is being called from the command line it's an error.
         */
        free((char *)cargv);
        FREE_ACCT_PTRS(&account);
        FREE_PK_PTRS(&pkdattrs);
        FREE_PK_PTRS(&pkcattrs);
        DCP_SET_MISC_ERROR(dcp_s_illegal_command);
    }

    if (global_context_list == NULL) {
        login_context = NULL;
    }
    else {
        login_context = global_context_list->context;
    }

    for (i = 0; i < cargc; i++) {
        strcpy((char *)global_name,cargv[i]);
        if(dcp_id_parse_name(interp,
                             global_name,
                             cell_name,
                             account.account_name.pname) != TCL_OK) {
            free((char *)cargv);
            FREE_ACCT_PTRS(&account);
            FREE_PK_PTRS(&pkdattrs);
            FREE_PK_PTRS(&pkcattrs);
            return TCL_ERROR;
        }
        if (open_rgy_site(interp, cell_name, site_update) != TCL_OK) {
            free((char *)cargv);
            FREE_ACCT_PTRS(&account);
            FREE_PK_PTRS(&pkdattrs);
            FREE_PK_PTRS(&pkcattrs);
            return TCL_ERROR;
        }
        sec_rgy_acct_add(rgy_context,
                         &(account.account_name),
                         &(account.acct_key),
                         &(account.acct_user),
                         &(account.acct_admin),
                         &(account.adm_passwd),
                         &(account.passwd),
                         sec_passwd_des,
                         &(account.passwd.version_number),
                         &result);
        if (result != sec_rgy_status_ok) {
            free((char *)cargv);
            FREE_ACCT_PTRS(&account);
            FREE_PK_PTRS(&pkdattrs);
            FREE_PK_PTRS(&pkcattrs);
            DCP_SET_MISC_ERROR(result);
        }
        account.passwd.version_number = 0;
        if (account.maxtktlife || account.maxtktrenew) {
            sec_rgy_auth_plcy_set_info(rgy_context,
                                       &(account.account_name),
                                       &(account.policy),
                                       &result);
            if (result != sec_rgy_status_ok) {
                free((char *)cargv);
                FREE_ACCT_PTRS(&account);
                FREE_PK_PTRS(&pkdattrs);
                FREE_PK_PTRS(&pkcattrs);
                DCP_SET_MISC_ERROR(result);
            }
        }

        /*
         * If pk information was specified, handle it.
         */
        if ((pkdattrs.newpassphrase) || (pkcattrs.newpassphrase)) {
            /*
             * Open the pk domain.
             */
            domain_id = sec_pk_domain_dce_general;
            status = sec_psm_open((char *)account.account_name.pname,
                                  (unsigned char *)account.passwd.key.tagged_union.plain,
                                  &domain_id,
                                  &psm_handle);
            if (status != error_status_ok) {
                free((char *)cargv);
                FREE_ACCT_PTRS(&account);
                FREE_PK_PTRS(&pkdattrs);
                FREE_PK_PTRS(&pkcattrs);
                DCP_SET_MISC_ERROR(status);
            }

            /*
             * If key information was not specified via files, then the
             * user wants the key information generated automatically.
             * If the current generate key value in either structure is
             * equal to the default value (in other words, the user
             * specified 'default' as input), change that value to '0'
             * so the sec_psm call handles it correctly.
             */
            if (pkdattrs.generatekey != 0) {
                alg_id = RSA_PKCS;
                if (pkdattrs.generatekey == pk_generate_default_value) {
                    pkdattrs.generatekey = 0;
                }
                fprintf(stderr, "Warning: Generating pksignature public key; this may take a few minutes.\n");
                status = sec_psm_gen_pub_key(alg_id,
                                             pkdattrs.generatekey,
                                             seed,
                                             &pubkey_d,
                                             &pvtkey_d);
                if (status != error_status_ok) {
                    free((char *)cargv);
                    FREE_ACCT_PTRS(&account);
                    FREE_PK_PTRS(&pkdattrs);
                    FREE_PK_PTRS(&pkcattrs);
                    sec_psm_close(psm_handle);
                    DCP_SET_MISC_ERROR(status);
                }
            }

            if (pkcattrs.generatekey != 0) {
                alg_id = RSA_PKCS;
                if (pkcattrs.generatekey == pk_generate_default_value) {
                    pkcattrs.generatekey = 0;
                }
                fprintf(stderr, "Warning: Generating pkkeycipher public key; this may take a few minutes.\n");
                status = sec_psm_gen_pub_key(alg_id,
                                             pkcattrs.generatekey,
                                             seed,
                                             &pubkey_c,
                                             &pvtkey_c);
                if (status != error_status_ok) {
                    free((char *)cargv);
                    FREE_ACCT_PTRS(&account);
                    FREE_PK_PTRS(&pkdattrs);
                    FREE_PK_PTRS(&pkcattrs);
                    sec_psm_close(psm_handle);
                    DCP_SET_MISC_ERROR(status);
                }
            }

            /*
             * Store the pk information.  If during the 'put' operation an error
             * occurs indicating the key already exists, try again using an
             * 'update' operation.
             */
            if (pkdattrs.newpassphrase) {
                usages = sec_pk_usage_digitalSignature;
                status = sec_psm_put_pub_key(psm_handle,
                                             (unsigned char *)pkdattrs.newpassphrase,
                                             usages,
                                             &pubkey_d,
                                             &pvtkey_d);
                if (status == sec_pvtkey_same_domain_and_usage_key_already_exist) {
                    /*
                     * Key exists, try update.
                     */
                    status = sec_psm_update_pub_key(psm_handle,
                                                    NULL,
                                                    (unsigned char *)pkdattrs.newpassphrase,
                                                    usages,
                                                    &pubkey_d,
                                                    &pvtkey_d);
                }
                if (status != error_status_ok) {
                    free((char *)cargv);
                    FREE_ACCT_PTRS(&account);
                    FREE_PK_PTRS(&pkdattrs);
                    FREE_PK_PTRS(&pkcattrs);
                    sec_psm_close(psm_handle);
                    DCP_SET_MISC_ERROR(status);
                }
            }

            if (pkcattrs.newpassphrase) {
                usages = sec_pk_usage_keyEncipherment;
                status = sec_psm_put_pub_key(psm_handle,
                                             (unsigned char *)pkcattrs.newpassphrase,
                                             usages,
                                             &pubkey_c,
                                             &pvtkey_c);
                if (status == sec_pvtkey_same_domain_and_usage_key_already_exist) {
                    /*
                     * Key exists, try update.
                     */
                    status = sec_psm_update_pub_key(psm_handle,
                                                    NULL,
                                                    (unsigned char *)pkcattrs.newpassphrase,
                                                    usages,
                                                    &pubkey_c,
                                                    &pvtkey_c);
                }
                if (status != error_status_ok) {
                    free((char *)cargv);
                    FREE_ACCT_PTRS(&account);
                    FREE_PK_PTRS(&pkdattrs);
                    FREE_PK_PTRS(&pkcattrs);
                    sec_psm_close(psm_handle);
                    DCP_SET_MISC_ERROR(status);
                }
            }

            /*
             * Close the pk domain.
             */
            status = sec_psm_close(psm_handle);
            if (status != error_status_ok) {
                free((char *)cargv);
                FREE_ACCT_PTRS(&account);
                FREE_PK_PTRS(&pkdattrs);
                FREE_PK_PTRS(&pkcattrs);
                DCP_SET_MISC_ERROR(status);
            }
        }

        /*
         * If a pk mechanism is specified, add it.
         */
        if (account.pkmechanism) {
            sec_attr_t                 attrs;
            sec_attr_t                 out_attrs;
            unsigned32                 num_attr, num_left;
            signed32                   failed_index;
            sec_attr_component_name_t  schema_name = NULL;
            sec_attr_schema_entry_t    schema_entry;

            sec_rgy_attr_sch_lookup_by_name(rgy_context,
                                            schema_name,
                                            (idl_char *)"DCEPKPrivateKeyStorage",
                                            &schema_entry,
                                            &result);
            if (result != sec_rgy_status_ok) {
                /*
                 * The schema entry name is unknown, return an error.
                 */
                free((char *)cargv);
                FREE_ACCT_PTRS(&account);
                FREE_PK_PTRS(&pkdattrs);
                FREE_PK_PTRS(&pkcattrs);
                DCP_SET_MISC_ERROR(result);
            }

            attrs.attr_id = schema_entry.attr_id;
            attrs.attr_value.attr_encoding = schema_entry.attr_encoding;
            if (strncmp("file", account.pkmechanism, strlen(account.pkmechanism)) == 0) {
                attrs.attr_value.tagged_union.uuid = sec_psm_file_mechanism;
            }
            else {
                attrs.attr_value.tagged_union.uuid = sec_psm_pkss_mechanism;
            }
            sec_rgy_attr_update(rgy_context,
                                sec_rgy_domain_person,
                                account.account_name.pname,
                                1,
                                0,
                                &attrs,
                                &num_attr,
                                &out_attrs,
                                &num_left,
                                &failed_index,
                                &result);
            if (result != sec_rgy_status_ok) {
                sec_attr_util_sch_ent_free_ptrs(&schema_entry);
                free((char *)cargv);
                FREE_ACCT_PTRS(&account);
                FREE_PK_PTRS(&pkdattrs);
                FREE_PK_PTRS(&pkcattrs);
                DCP_SET_MISC_ERROR(result);
            }
            sec_attr_util_sch_ent_free_ptrs(&schema_entry);
        }
    }
    free((char *)cargv);
    FREE_ACCT_PTRS(&account);
    FREE_PK_PTRS(&pkdattrs);
    FREE_PK_PTRS(&pkcattrs);
    sec_pk_data_free(&pubkey_d);
    sec_pk_data_free(&pubkey_c);
    sec_pk_data_zero_and_free(&pvtkey_d);
    sec_pk_data_zero_and_free(&pvtkey_c);
    return TCL_OK;
}


/*
 * FUNCTION: rgy_account_catalog
 *
 * OVERVIEW: This function displays all of the currently existing accounts 
 *    in the registry.
 * 
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
 *  int  either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 *    
 */
int rgy_account_catalog(ClientData clientData, 
                        Tcl_Interp *interp, 
                        int argc, 
                        char **argv)
{
    unsigned32            result;
    sec_rgy_cursor_t      cursor;
    sec_rgy_login_name_t  key_name;
    sec_rgy_name_t        global_name;
    sec_rgy_login_name_t  account_name;
    sec_rgy_sid_t         sid;
    sec_rgy_unix_sid_t    unix_sid;
    sec_rgy_acct_key_t    acct_key;
    sec_rgy_acct_user_t   acct_user;
    sec_rgy_acct_admin_t  acct_admin;
    char                  *output;
    boolean32             simplename = FALSE;
    sec_rgy_name_t        cell_name;
    size_t                account_name_size;
    char                  **aargv;
    int                   aargc = 0;
    int                   i = 0;
    int                   saved_length;
    int                   bad_rgy_cursor_count = 0;
    char                  *expand_global_name;

    dcp_ArgvInfo arg_table[] = {
        {"-simplename", DCP_ARGV_CONSTANT, (char *)1, NULL,
            dcp_t_rgy_acct_simple_help},
        {NULL, DCP_ARGV_END, NULL, NULL,0} 
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
        if (dcp_general_split_list(interp, argv[1], &aargc, &aargv) != TCL_OK) {
            return TCL_ERROR;
        }
        else {
            /*
             * Check that we did not receive an empty argument list.
             */
            if ((aargc == 0) && (aargv[0] == NULL)) {
                DCP_SET_MISC_ERROR(dcp_s_null_arg);
            }
        }
    }
    else {
        aargc = 1;
        aargv = (char **)malloc(2 * sizeof(char *));
        if (aargv == NULL) {
            DCP_SET_RESULT_CODE(dcp_s_no_memory);
            return TCL_ERROR;
        }
        aargv[0] = (char *)global_name;
        aargv[1] = NULL;
    }

    for (; i < aargc; i++) {
        strcpy((char *)global_name, aargv[i]);
        if (open_rgy_site(interp, global_name, site_query) != TCL_OK) {
            free((char *)aargv);
            DCP_SET_SEC_ERROR(dcp_s_rgy_invalid_cell, global_name);
        }
        strcpy((char *)cell_name, (char *)site_cache[realtime_cache].cell_name);    
        if (*aargv[i] != '\0') {
            if (global_name[0] == '/') {
                expand_global_name = expand_cell_name((char *)global_name, &result);
                if (result != rpc_s_incomplete_name) {
                    strcpy((char *)global_name, expand_global_name);
                    free(expand_global_name);
                }
            }
            if (strcmp((char *)global_name, (char *)cell_name) != 0) {
                free((char *)aargv);
                DCP_SET_SEC_ERROR(dcp_s_rgy_invalid_cell, global_name);
            }
        }
        strcpy((char *)key_name.pname, "\0");
        strcpy((char *)key_name.gname, "\0");
        strcpy((char *)key_name.oname, "\0");
        sec_rgy_cursor_reset(&cursor);

        account_name_size = strlen((char *)cell_name) + sec_rgy_pname_t_size + 1;
        output = (char *)malloc(account_name_size);
        if (output == NULL) {
            free((char *)aargv);
            DCP_SET_RESULT_CODE(dcp_s_no_memory);
            return TCL_ERROR;
        }

        if (strlen(interp->result)) {
            Tcl_AppendResult(interp, "\n", NULL);
            saved_length = strlen(interp->result);
        }
        else {
            saved_length = 0;
        }

        for (result = error_status_ok; result == error_status_ok;) {
            sec_rgy_acct_lookup(rgy_context,
                                &key_name,
                                &cursor,
                                &account_name,
                                &sid,
                                &unix_sid,
                                &acct_key,
                                &acct_user,
                                &acct_admin,
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
                strcpy((char *)key_name.pname, "\0");
                strcpy((char *)key_name.gname, "\0");
                strcpy((char *)key_name.oname, "\0");
                sec_rgy_cursor_reset(&cursor);
                interp->result[saved_length] = '\0';
                result = error_status_ok;
                continue;
            }
            if (result != sec_rgy_status_ok) {
                free((char *)aargv);
                free(output);
                DCP_SET_MISC_ERROR(result);
            }

            if (strlen(interp->result))
                Tcl_AppendResult(interp, "\n", NULL);
            if (!simplename) {
                sprintf(output, "%s/%s", cell_name, account_name.pname);
            }
            else {
                sprintf(output, "%s", account_name.pname);
            }
            Tcl_AppendResult(interp, output, NULL);
        }
        free(output);
    }
    free((char *)aargv);
    return TCL_OK;
}


/*
 * FUNCTION: rgy_account_delete
 *
 * OVERVIEW: This function delete the current attributes of the 
 *            specified account.
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
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int  either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 *    
 */
int rgy_account_delete(ClientData clientData, 
                       Tcl_Interp *interp, 
                       int argc, 
                       char **argv)
{
    error_status_t             result = TCL_OK;
    int                        pargc;
    char                       **pargv;
    int                        i = 0;
    sec_rgy_login_name_t       name;
    sec_rgy_name_t             global_name;
    sec_rgy_name_t             cell_name;
    boolean32                  is_simple;
    sec_attr_t                 *del_attrs;
    signed32                   failed_index;
    sec_attr_component_name_t  schema_name = NULL;
    sec_attr_schema_entry_t    schema_entry;
    char                       *conical_command = "principal modify {%s} -remove {%s} -types";
    char                       *command;

    dcp_ArgvInfo arg_table[] = { 
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    DCP_CHECK_RGY_NO_ARG(acct);
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

    name.gname[0] = '\0';
    name.oname[0] = '\0';

    for (; i < pargc; i++) {
        strcpy((char *)global_name, pargv[i]);
        if (dcp_id_parse_name(interp, global_name, cell_name, name.pname) != TCL_OK) {
            free((char *)pargv);
            return TCL_ERROR;
        }

        if (open_rgy_site(interp, cell_name, site_update) != TCL_OK) {
            free((char *)pargv);
            return TCL_ERROR;
        }
        sec_rgy_acct_delete(rgy_context, &name, (error_status_t *)&result);
        if (result != sec_rgy_status_ok)
            free((char *)pargv);
        DCP_CHECK_SEC_ERROR(result);

        /*
         * For public key work, because the public key ERA's are assigned
         * to the principal and not the account, when the account is deleted
         * the ERA's must be removed from the principal.  If the ERA's don't
         * exist on the principal, it's no problem.
         */
        command = malloc(strlen(conical_command) + strlen((char *)name.pname) + 20);
        sprintf(command, conical_command, name.pname, "DCEPKAuthentication");
        Tcl_Eval(interp, command);
        Tcl_ResetResult(interp);
        free(command);
        command = malloc(strlen(conical_command) + strlen((char *)name.pname) + 21);
        sprintf(command, conical_command, name.pname, "DCEPKKeyEncipherment");
        Tcl_Eval(interp, command);
        Tcl_ResetResult(interp);
        free(command);
    }

    free((char *)pargv);
    return TCL_OK;
}


/*
 * FUNCTION: rgy_account_generate
 *
 * OVERVIEW: This function generates a random password for the
 *           specified account.
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
 *  Generated password.
 *
 * RETURNS:
 *  int  either TCL_OK or TCL_ERROR to notify the interpreter of successful
 *      completion or command failure.
 *   
 */
int rgy_account_generate(ClientData clientData, 
                       Tcl_Interp *interp, 
                       int argc, 
                       char **argv)
{
    unsigned32             result = TCL_OK;
    char                   **margv;
    int                    margc = 0;
    sec_login_handle_t     login_context;
    sec_rgy_login_name_t   key_name;
    sec_pwd_mgmt_handle_t  pwd_mgmt_h;
    ek_pwd_val_type_t      pwd_val_type;
    unsigned32             num_pwds = 1;
    unsigned32             num_returned;
    sec_passwd_rec_t       passwd;
    sec_rgy_name_t         global_name;
    sec_rgy_name_t         cell_name;

    dcp_ArgvInfo arg_table[] = { 
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    /*
     * Verify that the user provided an account name with the command. 
     */
    DCP_CHECK_RGY_NO_ARG(acct);
    DCP_CHECK_EXTRA_ARGS();

    if (Tcl_SplitList(interp, argv[1], &margc, &margv) == TCL_ERROR) {
        return TCL_ERROR;
    }
    else {
        /*
         * Check that we did not receive an empty argument list.
         */
        if ((margc == 0) && (margv[0] == NULL)) {
            DCP_SET_MISC_ERROR(dcp_s_null_arg);
        }
    }

    /*
     * Get current context
     */
    if (global_context_list == NULL) {
        login_context = NULL;
    }
    else {
        login_context = global_context_list->context;
    }

    strcpy((char *)key_name.pname,"\0");
    strcpy((char *)key_name.gname,"\0");
    strcpy((char *)key_name.oname,"\0");
    strcpy((char *)global_name, margv[0]);
    if (dcp_id_parse_name(interp,
                          global_name,
                          cell_name,
                          key_name.pname) != TCL_OK) {
        return TCL_ERROR;
    }
    sec_pwd_mgmt_setup(&pwd_mgmt_h, 
                       rgy_context,
                       key_name, 
                       login_context, 
                       NULL, 
                       &result);
    if (result != sec_rgy_status_ok) {
        free((char *)margv);
        DCP_SET_MISC_ERROR(result);
    }

    sec_pwd_mgmt_get_val_type(pwd_mgmt_h,
                              (signed32 *)&pwd_val_type, 
                              &result);
    if (result != sec_rgy_status_ok) {
        free((char *)margv);
        DCP_SET_MISC_ERROR(result);
    }

    switch (pwd_val_type) {
        case generation_required:
        case user_can_select:
            sec_pwd_mgmt_gen_pwd(pwd_mgmt_h, 
                                 num_pwds, 
                                 &num_returned,
                                 &(passwd), 
                                 &result);
            if (result != sec_rgy_status_ok) {
                free((char *)margv);
                DCP_SET_MISC_ERROR(result);
            }
            break;
        case user_select:
        case none:
            sec_pwd_mgmt_free_handle(&pwd_mgmt_h, &result);
            free((char *)margv);
            DCP_CHECK_SEC_ERROR(result);
            DCP_SET_RESULT_CODE(dcp_s_rgy_no_random_password);
            return TCL_ERROR;
        default:
            sec_pwd_mgmt_free_handle(&pwd_mgmt_h, &result);
            free((char *)margv);
            DCP_CHECK_SEC_ERROR(result);
            DCP_SET_RESULT_CODE(dcp_s_rgy_invalid_pwd_val_type);
            return TCL_ERROR;
    }
    sec_pwd_mgmt_free_handle(&pwd_mgmt_h, &result);
    if (result != sec_rgy_status_ok) {
        free((char *)margv);
        DCP_SET_MISC_ERROR(result);
    }
    Tcl_AppendResult(interp, passwd.key.tagged_union.plain, NULL);
    free((char *)margv);
    return TCL_OK;
}


/*
 * FUNCTION: rgy_account_modify
 *
 * OVERVIEW: This function modifies the current attributes of the 
 *           specified account. 
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
 *  int  either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 *    
 */
int rgy_account_modify(ClientData clientData, 
                       Tcl_Interp *interp, 
                       int argc, 
                       char **argv)
{
    unsigned32             result = TCL_OK;
    rgy_acct_info_t        account;
    rgy_pubkey_info_t      pkdattrs;
    rgy_pubkey_info_t      pkcattrs;
    char                   *pkdigital = NULL;
    char                   *pkcipher = NULL;
    char                   *attribute = NULL;
    char                   **margv;
    int                    margc = 0;
    sec_rgy_name_t         global_name;
    sec_rgy_name_t         cell_name;
    sec_rgy_acct_key_t     key = sec_rgy_acct_key_person;
    sec_rgy_login_name_t   key_name;
    sec_rgy_cursor_t       cursor;
    int                    i = 0;
    int                    passwd_len;
    char                   *current_group;
    char                   *current_org;
    sec_login_handle_t     login_context;
    unsigned32             rls_mypwd_mem = 0;
    boolean32              is_simple;
    extern boolean32       check_visible_pw;
    boolean32              change_pwd = FALSE;
    boolean32              default_privkey = FALSE;
    char                   *end;
    int                    plen;
    int                    genkey_int;
    sec_passwd_type_t      pwd_keytype = sec_passwd_des;
    error_status_t         status;
    sec_psm_handle_t       psm_handle;
    sec_pk_domain_t        domain_id;
    sec_pk_algorithm_id_t  *alg_id;
    sec_pk_gen_data_t      *seed = NULL;
    sec_pk_usage_flags_t   usages;
    sec_pk_pubkey_t        pubkey_d;
    sec_pk_pvtkey_t        pvtkey_d;
    sec_pk_pubkey_t        pubkey_c;
    sec_pk_pvtkey_t        pvtkey_c;

    dcp_ArgvInfo arg_table[] = {
        {"-acctvalid", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_valid_help},
        {"-change", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_change_help},
        {"-client", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_client_help},
        {"-description", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_descr_help},
        {"-dupkey", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_dupkey_help},
        {"-expdate", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_expdate_help},
        {"-forwardabletkt", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_forward_help},
        {"-goodsince", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_goodsince_help}, 
        {"-group", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_group_help},
        {"-home", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_home_help},
        {"-maxtktlife", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_tktlife_help},
        {"-maxtktrenew", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_tktrenew_help},
        {"-mypwd", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_mypwd_help},
        {"-organization", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_org_help},
        {"-password", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_passwd_help},
        {"-pkgenprivkey", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_pkgenpriv_help},
        {"-pkkeycipherusage", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_pkcipher_help},
        {"-pkmechanism", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_pkmechanism_help},
        {"-pksignatureusage", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_pkdigital_help},
        {"-postdatedtkt", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_posttkt_help},
        {"-proxiabletkt", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_proxtkt_help},
        {"-pwdvalid", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_pwdvalid_help},
        {"-renewabletkt", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_renewtkt_help},
        {"-server", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_server_help},
        {"-shell", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_shell_help},
        {"-stdtgtauth", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_auth_help},
        {"-usertouser", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_acct_usertouser_help},
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
    memset(&account,0,sizeof(account));
    arg_table[0].dst = (char *)&account.acctvalid;
    arg_table[1].dst = (char *)&attribute;
    arg_table[2].dst = (char *)&account.client;
    arg_table[3].dst = (char *)&account.description;
    arg_table[4].dst = (char *)&account.dupkey;
    arg_table[5].dst = (char *)&account.expdate;
    arg_table[6].dst = (char *)&account.forwardabletkt;
    arg_table[7].dst = (char *)&account.goodsince;
    arg_table[8].dst = (char *)&account.group;
    arg_table[9].dst = (char *)&account.home;
    arg_table[10].dst = (char *)&account.maxtktlife;
    arg_table[11].dst = (char *)&account.maxtktrenew;
    arg_table[12].dst = (char *)&account.mypwd;
    arg_table[13].dst = (char *)&account.organization;
    arg_table[14].dst = (char *)&account.password;
    arg_table[15].dst = (char *)&account.pkgenprivkey;
    arg_table[16].dst = (char *)&pkcipher;
    arg_table[17].dst = (char *)&account.pkmechanism;
    arg_table[18].dst = (char *)&pkdigital;
    arg_table[19].dst = (char *)&account.postdatedtkt;
    arg_table[20].dst = (char *)&account.proxiabletkt;
    arg_table[21].dst = (char *)&account.pwdvalid;
    arg_table[22].dst = (char *)&account.renewabletkt;
    arg_table[23].dst = (char *)&account.server;
    arg_table[24].dst = (char *)&account.shell;
    arg_table[25].dst = (char *)&account.stdtgtauth;
    arg_table[26].dst = (char *)&account.usertouser;

    if ((result = dcp_ParseArgv(interp, &argc, argv, arg_table, 0)) == TCL_ERROR)
        return TCL_ERROR;

    /*
     * Verify that the user provided an account name with the command. 
     * If so initialize the pgo name parameter.
     */
    DCP_CHECK_RGY_NO_ARG(acct);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * Check to see if attribute list and switches were provided.
     */
    if ((account.acctvalid || account.client || account.description || account.dupkey ||
        account.expdate || account.forwardabletkt || account.goodsince || account.group ||
        account.home || account.mypwd || account.organization || account.password ||
        account.postdatedtkt || account.proxiabletkt || account.pwdvalid ||
        account.renewabletkt || account.server || account.shell || account.stdtgtauth ||
        account.maxtktlife || account.maxtktrenew || account.usertouser ||
        account.pkgenprivkey || account.pkmechanism || pkdigital || pkcipher) && attribute) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_list_and_switches);
    }

    if (Tcl_SplitList(interp, argv[1], &margc, &margv) == TCL_ERROR) {
        return TCL_ERROR;
    }
    else {
        /*
         * Check that we did not receive an empty argument list.
         */
        if ((margc == 0) && (margv[0] == NULL)) {
            DCP_SET_MISC_ERROR(dcp_s_null_arg);
        }
    }

    if ((is_simple = is_simple_name(margc, margv)) == -1) {
        free((char *)margv);
        DCP_SET_MISC_ERROR(dcp_s_rgy_full_or_simple);
    }

    initialize_account_attr(&account);
    initialize_pk_attr(&pkdattrs);
    initialize_pk_attr(&pkcattrs);
    pubkey_d.data = NULL;
    pvtkey_d.data = NULL;
    pubkey_c.data = NULL;
    pvtkey_c.data = NULL;
    pubkey_d.len = 0;
    pvtkey_d.len = 0;
    pubkey_c.len = 0;
    pvtkey_c.len = 0;

    /*
     * Parse the attribute list. If one is supplied. 
     * Simply fill out the account structure and parse_account_switches 
     * will fill out the account structures accordingly.
     */
    if (attribute) {
        pkdigital = (char *)malloc(strlen(attribute) * sizeof(char *));
        pkcipher = (char *)malloc(strlen(attribute) * sizeof(char *));
        memset((char *)pkdigital, 0, sizeof(pkdigital));
        memset((char *)pkcipher, 0, sizeof(pkcipher));
        result = parse_account_attributes(interp, &account, pkdigital, pkcipher, attribute);
        if (result != error_status_ok) {
            free((char *)margv);
            FREE_ACCT_PTRS(&account);
            if (pkdigital)
                free((char *)pkdigital);
            if (pkcipher)
                free((char *)pkcipher);
            return TCL_ERROR;
        }
    }

    if (account.pkgenprivkey) {
        genkey_int = -1;
        plen = strlen((char *)account.pkgenprivkey);
        if (strncmp(pk_genkey_default_string, account.pkgenprivkey, plen) == 0) {
            account.pkgenprivkey_value = pk_genkey_default_value;
            default_privkey = TRUE;
        }
        else {
            genkey_int = is_number(account.pkgenprivkey);
            if ((genkey_int != dcp_rgy_not_int) && (genkey_int != dcp_rgy_int_out_of_range)) {
                genkey_int = strtol(account.pkgenprivkey, &end, 10);
                account.pkgenprivkey_value = genkey_int;
            }
        }
    }

    if ((account.password) && (account.password[0] == '\0')) {
        account.password = (char *)malloc(sec_passwd_str_max_len);
        passwd_len = sec_passwd_str_max_len;
        if (!account.password) {
            free((char *)margv);
            FREE_ACCT_PTRS(&account);
            DCP_SET_MISC_ERROR(dcp_s_no_memory);
        }
        memset(account.password, 0, sec_passwd_str_max_len - 1);
        if (krb5_read_password("Enter Account Password: ",
                               "Again: ",
                               (char *)account.password,
                               &passwd_len) != 0) {
            free((char *)margv);
            setup_signal_handling();
            FREE_ACCT_PTRS(&account);
            DCP_SET_MISC_ERROR(dcp_s_rgy_acct_no_passwd);
        }
        setup_signal_handling();
        account.passwd.key.tagged_union.plain = (unsigned char *)account.password;
    }

    if ((account.password) && ((account.pkgenprivkey_value > 0) || (default_privkey))) {
        /*
         * Cannot specify the password and request the generation
         * of a priv key at the same time.
         */
        free((char *)margv);
        FREE_ACCT_PTRS(&account);
        DCP_SET_MISC_ERROR(dcp_s_rgy_pk_gen_and_pass);
    }

    if ((account.password) && (check_visible_pw)) {
        /*
         * If this is being called from the command line it's an error.
         */
        free((char *)margv);
        FREE_ACCT_PTRS(&account);
        DCP_SET_MISC_ERROR(dcp_s_illegal_command);
    }

    if (((account.password) || (account.pkgenprivkey_value > 0) || (default_privkey)) && !(account.mypwd)) {
        account.mypwd = (char *)malloc(sec_passwd_str_max_len);
        passwd_len = sec_passwd_str_max_len;
        if (!account.mypwd) {
            free((char *)margv);
            FREE_ACCT_PTRS(&account);
            DCP_SET_MISC_ERROR(dcp_s_no_memory);
        }
        rls_mypwd_mem = 1;
        memset(account.mypwd, 0, sec_passwd_str_max_len - 1);
        if (krb5_read_password("Enter Your Password: ", 
                               NULL, 
                               (char *)account.mypwd,
                               &passwd_len) != 0) {
            free((char *)margv);
            setup_signal_handling();
            FREE_ACCT_PTRS(&account);
            DCP_SET_MISC_ERROR(dcp_s_login_bad_pw_read);
        }
        setup_signal_handling();
        account.adm_passwd.key.key_type = sec_passwd_plain;
        account.adm_passwd.key.tagged_union.plain = (unsigned char *)account.mypwd;
        account.adm_passwd.version_number = sec_passwd_c_version_none;
    }
    else if (check_visible_pw) {
        /*
         * If this is being called from the command line it's an error.
         */
        free((char *)margv);
        FREE_ACCT_PTRS(&account);
        DCP_SET_MISC_ERROR(dcp_s_illegal_command);
    }

    if (global_context_list == NULL) {
        login_context = NULL;
    }
    else {
        login_context = global_context_list->context;
    }

    /*
     * Regardless of the number of accounts being modified (margc),
     * the public key information gets applied to each one in the
     * same manner.  Parse the public key information supplied by
     * the user so the information will be available to whatever
     * security API calls need it.
     */
    result = parse_pk_switches(interp, &pkdattrs, &pkcattrs, pkdigital, pkcipher);
    if (result != error_status_ok) {
        free((char *)margv);
        FREE_ACCT_PTRS(&account);
        FREE_PK_PTRS(&pkdattrs);
        FREE_PK_PTRS(&pkcattrs);
        return TCL_ERROR;
    }

    /*
     * Check pksignatureusage parameters.
     */
    if ((pkdigital != NULL) && (strlen(pkdigital) > 0)) {
        result = check_pk_modify_arguments(interp, &pkdattrs, "signature");
        if (result != error_status_ok) {
            free((char *)margv);
            FREE_ACCT_PTRS(&account);
            FREE_PK_PTRS(&pkdattrs);
            FREE_PK_PTRS(&pkcattrs);
            return TCL_ERROR;
        }
    }

    /*
     * Check pkkeycipherusage parameters.
     */
    if ((pkcipher != NULL) && (strlen(pkcipher) > 0)) {
        result = check_pk_modify_arguments(interp, &pkcattrs, "key encryption");
        if (result != error_status_ok) {
            free((char *)margv);
            FREE_ACCT_PTRS(&account);
            FREE_PK_PTRS(&pkdattrs);
            FREE_PK_PTRS(&pkcattrs);
            return TCL_ERROR;
        }
    }

    /*
     * Check the consistency of the two newpassphrases.
     */
    if ((pkdattrs.newpassphrase) && (pkcattrs.newpassphrase)) {
        if (strcmp(pkdattrs.newpassphrase, pkcattrs.newpassphrase) != 0) {
            DCP_SET_RESULT_CODE(dcp_s_rgy_pk_inconsistent_passphrase);
            return TCL_ERROR;
        }
    }

    /*
     * If any public key files were specified, check that the
     * files can be opened and read.  This allows dcecp to do
     * some basic checking of the user files before the security
     * api call is made.
     */
    if ((pkdattrs.publickeyfile) || (pkdattrs.privatekeyfile)) {
        result = check_pk_file_contents(interp, &pkdattrs);
        if (result != error_status_ok) {
            free((char *)margv);
            FREE_ACCT_PTRS(&account);
            FREE_PK_PTRS(&pkdattrs);
            FREE_PK_PTRS(&pkcattrs);
            return TCL_ERROR;
        }
    }
    if ((pkcattrs.publickeyfile) || (pkcattrs.privatekeyfile)) {
        result = check_pk_file_contents(interp, &pkcattrs);
        if (result != error_status_ok) {
            free((char *)margv);
            FREE_ACCT_PTRS(&account);
            FREE_PK_PTRS(&pkdattrs);
            FREE_PK_PTRS(&pkcattrs);
            return TCL_ERROR;
        }
    }

    /*
     * If public or private key information was specified via file, the
     * sec_pk_data_t structure (idl_byte and length) must be used.
     */
    if (pkdattrs.publickeyfile) {
        status = sec_pubkey_read_keyfile((idl_char *)pkdattrs.publickeyfile, NULL, NULL, &pubkey_d);
        if (status != error_status_ok) {
            free((char *)margv);
            FREE_ACCT_PTRS(&account);
            FREE_PK_PTRS(&pkdattrs);
            FREE_PK_PTRS(&pkcattrs);
            DCP_SET_MISC_ERROR(status);
        }
    }

    if (pkdattrs.privatekeyfile) {
        status = sec_pubkey_read_keyfile((idl_char *)pkdattrs.privatekeyfile, NULL, NULL, &pvtkey_d);
        if (status != error_status_ok) {
            free((char *)margv);
            FREE_ACCT_PTRS(&account);
            FREE_PK_PTRS(&pkdattrs);
            FREE_PK_PTRS(&pkcattrs);
            DCP_SET_MISC_ERROR(status);
        }
    }

    if (pkcattrs.publickeyfile) {
        status = sec_pubkey_read_keyfile((idl_char *)pkcattrs.publickeyfile, NULL, NULL, &pubkey_c);
        if (status != error_status_ok) {
            free((char *)margv);
            FREE_ACCT_PTRS(&account);
            FREE_PK_PTRS(&pkdattrs);
            FREE_PK_PTRS(&pkcattrs);
            DCP_SET_MISC_ERROR(status);
        }
    }

    if (pkcattrs.privatekeyfile) {
        status = sec_pubkey_read_keyfile((idl_char *)pkcattrs.privatekeyfile, NULL, NULL, &pvtkey_c);
        if (status != error_status_ok) {
            free((char *)margv);
            FREE_ACCT_PTRS(&account);
            FREE_PK_PTRS(&pkdattrs);
            FREE_PK_PTRS(&pkcattrs);
            DCP_SET_MISC_ERROR(status);
        }
    }

    /*
     * Loop through each account name.
     */
    for (i = 0; i < margc; i++) {
        strcpy((char *)key_name.pname,"\0");
        strcpy((char *)key_name.gname,"\0");
        strcpy((char *)key_name.oname,"\0");
        sec_rgy_cursor_reset(&cursor);
        strcpy((char *)global_name, margv[i]);
        if (dcp_id_parse_name(interp, global_name, cell_name, key_name.pname) != TCL_OK) {
            free((char *)margv);
            FREE_ACCT_PTRS(&account);
            FREE_PK_PTRS(&pkdattrs);
            FREE_PK_PTRS(&pkcattrs);
            return TCL_ERROR;
        }

        if (open_rgy_site(interp, cell_name, site_update) != TCL_OK) {
            free((char *)margv);
            return TCL_ERROR;
        }
        sec_rgy_acct_lookup(rgy_context,
                            &key_name,
                            &cursor,
                            &(account.account_name),
                            &(account.sid),
                            &(account.unix_sid),
                            &(account.acct_key),
                            &(account.acct_user),
                            &(account.acct_admin),
                            &result);
        if (result != sec_rgy_status_ok) {
            free((char *)margv);
            FREE_ACCT_PTRS(&account);
            FREE_PK_PTRS(&pkdattrs);
            FREE_PK_PTRS(&pkcattrs);
            DCP_SET_MISC_ERROR(result);
        }

        /*
         * Save local copies of the current group and org names.
         */
        current_group = malloc(strlen((char *)account.account_name.gname) + 1);
        current_org = malloc(strlen((char *)account.account_name.oname) + 1);
        if (!current_group || !current_org) {
            free((char *)margv);
            FREE_ACCT_PTRS(&account);
            FREE_PK_PTRS(&pkdattrs);
            FREE_PK_PTRS(&pkcattrs);
            DCP_SET_MISC_ERROR(dcp_s_no_memory);
        }
        strcpy(current_group, (char *)account.account_name.gname);
        strcpy(current_org, (char *)account.account_name.oname);

        if (account.maxtktlife || account.maxtktrenew) {
            sec_rgy_auth_plcy_get_info(rgy_context,    
                                       &key_name,
                                       &(account.policy),
                                       &result);
            if (result != sec_rgy_object_not_found && result != error_status_ok) {
                if (result != sec_rgy_status_ok) {
                    free((char *)margv);
                    FREE_ACCT_PTRS(&account);
                    FREE_PK_PTRS(&pkdattrs);
                    FREE_PK_PTRS(&pkcattrs);
                    free(current_group);
                    free(current_org);
                    DCP_SET_MISC_ERROR(result);
                }
            }
        }
        result = parse_account_switches(interp, &account);
        if (result != error_status_ok) {
            free((char *)margv);
            FREE_ACCT_PTRS(&account);
            FREE_PK_PTRS(&pkdattrs);
            FREE_PK_PTRS(&pkcattrs);
            free(current_group);
            free(current_org);
            return TCL_ERROR;
        }

        if (account.group || account.organization) {
            if ((strcmp(current_group, (char *)account.account_name.gname) != 0) ||
                (strcmp(current_org, (char *)account.account_name.oname) != 0)) {
                sec_rgy_acct_rename(rgy_context,
                                    &key_name,
                                    &(account.account_name),
                                    &key,
                                    &result);
                if (result != sec_rgy_status_ok) {
                    free((char *)margv);
                    FREE_ACCT_PTRS(&account);
                    FREE_PK_PTRS(&pkdattrs);
                    FREE_PK_PTRS(&pkcattrs);
                    free(current_group);
                    free(current_org);
                    DCP_SET_MISC_ERROR(result);
                }
            }
        }
        free(current_group);
        free(current_org);

        if (account.password || account.pkgenprivkey_value > 0 || default_privkey) {
            change_pwd = TRUE;
            if (account.pkgenprivkey_value > 0 || default_privkey) {
                pwd_keytype = sec_passwd_genprivkey;
            }
        }

        if (account.acctvalid || account.client || account.dupkey || account.goodsince ||
            account.forwardabletkt || account.expdate || account.postdatedtkt ||
            account.proxiabletkt || account.renewabletkt || account.server ||
            account.stdtgtauth || account.description || account.pwdvalid || account.home ||
            account.shell || account.password || account.usertouser || account.pkgenprivkey_value > 0 ||
            default_privkey) {
            sec_rgy_acct_replace_all(rgy_context,
                                     &key_name,
                                     &key,
                                     &(account.acct_user),
                                     &(account.acct_admin),
                                     change_pwd,
                                     &(account.adm_passwd),
                                     &(account.passwd),
                                     pwd_keytype,
                                     &(account.passwd.version_number),
                                     &result);
            if (result != sec_rgy_status_ok) {
                free((char *)margv);
                FREE_ACCT_PTRS(&account);
                FREE_PK_PTRS(&pkdattrs);
                FREE_PK_PTRS(&pkcattrs);
                DCP_SET_MISC_ERROR(result);
            }
        }
        if (account.maxtktlife || account.maxtktrenew) {
            sec_rgy_auth_plcy_set_info(rgy_context,
                                       &key_name,
                                       &(account.policy),
                                       &result);
            if (result != sec_rgy_status_ok) {
                free((char *)margv);
                FREE_ACCT_PTRS(&account);
                FREE_PK_PTRS(&pkdattrs);
                FREE_PK_PTRS(&pkcattrs);
                DCP_SET_MISC_ERROR(result);
            }
        }

        /*
         * If pk information was specified, handle it.
         */

        if ((pkdattrs.newpassphrase) || (pkcattrs.newpassphrase) ||
            (pkdattrs.oldpassphrase) || (pkcattrs.oldpassphrase)) {
            /*
             * Open the pk domain.
             */
            domain_id = sec_pk_domain_dce_general;
            status = sec_psm_open((char *)account.account_name.pname,
                                  (unsigned char *)pkdattrs.oldpassphrase,
                                  &domain_id,
                                  &psm_handle);
            if (status != error_status_ok) {
                free((char *)margv);
                FREE_ACCT_PTRS(&account);
                FREE_PK_PTRS(&pkdattrs);
                FREE_PK_PTRS(&pkcattrs);
                DCP_SET_MISC_ERROR(status);
            }

            /*
             * If key information was not specified via files, then the
             * user wants the key information generated automatically.
             * If the current generate key value in either structure is
             * equal to the default value (in other words, the user
             * specified 'default' as input), change that value to '0'
             * so the sec_psm call handles it correctly.
             */
            if (pkdattrs.generatekey != 0) {
                alg_id = RSA_PKCS;
                if (pkdattrs.generatekey == pk_generate_default_value) {
                    pkdattrs.generatekey = 0;
                }
                fprintf(stderr, "Warning: Generating pksignature public key; this may take a few minutes.\n");
                status = sec_psm_gen_pub_key(alg_id,
                                             pkdattrs.generatekey,
                                             seed,
                                             &pubkey_d,
                                             &pvtkey_d);
                if (status != error_status_ok) {
                    free((char *)margv);
                    FREE_ACCT_PTRS(&account);
                    FREE_PK_PTRS(&pkdattrs);
                    FREE_PK_PTRS(&pkcattrs);
                    sec_psm_close(psm_handle);
                    DCP_SET_MISC_ERROR(status);
                }
            }

            if (pkcattrs.generatekey != 0) {
                alg_id = RSA_PKCS;
                if (pkcattrs.generatekey == pk_generate_default_value) {
                    pkcattrs.generatekey = 0;
                }
                fprintf(stderr, "Warning: Generating pkkeycipher public key; this may take a few minutes.\n");
                status = sec_psm_gen_pub_key(alg_id,
                                             pkcattrs.generatekey,
                                             seed,
                                             &pubkey_c,
                                             &pvtkey_c);
                if (status != error_status_ok) {
                    free((char *)margv);
                    FREE_ACCT_PTRS(&account);
                    FREE_PK_PTRS(&pkdattrs);
                    FREE_PK_PTRS(&pkcattrs);
                    sec_psm_close(psm_handle);
                    DCP_SET_MISC_ERROR(status);
                }
            }

            /*
             * Store the pk information.  If during the 'update' operation an
             * error occurs indicating the key already exists, try again using
             * a 'put' operation.
             */
            if ((pkdattrs.newpassphrase) || (pkdattrs.oldpassphrase)) {
                usages = sec_pk_usage_digitalSignature;
                status = sec_psm_update_pub_key(psm_handle,
                                                (unsigned char *)pkdattrs.oldpassphrase,
                                                (unsigned char *)pkdattrs.newpassphrase,
                                                usages,
                                                &pubkey_d,
                                                &pvtkey_d);
                if (status == sec_pvtkey_no_matched_private_key) {
                    /*
                     * Key doesn't exist, try put.
                     */
                    status = sec_psm_put_pub_key(psm_handle,
                                                 (unsigned char *)pkdattrs.newpassphrase,
                                                 usages,
                                                 &pubkey_d,
                                                 &pvtkey_d);
                }
                if (status != error_status_ok) {
                    free((char *)margv);
                    FREE_ACCT_PTRS(&account);
                    FREE_PK_PTRS(&pkdattrs);
                    FREE_PK_PTRS(&pkcattrs);
                    sec_psm_close(psm_handle);
                    DCP_SET_MISC_ERROR(status);
                }
            }

            if ((pkcattrs.newpassphrase) || (pkcattrs.oldpassphrase)) {
                usages = sec_pk_usage_keyEncipherment;
                status = sec_psm_update_pub_key(psm_handle,
                                                (unsigned char *)pkcattrs.oldpassphrase,
                                                (unsigned char *)pkcattrs.newpassphrase,
                                                usages,
                                                &pubkey_c,
                                                &pvtkey_c);
                if (status == sec_pvtkey_no_matched_private_key) {
                    /*
                     * Key doesn't exist, try put.
                     */
                    status = sec_psm_put_pub_key(psm_handle,
                                                 (unsigned char *)pkcattrs.newpassphrase,
                                                 usages,
                                                 &pubkey_c,
                                                 &pvtkey_c);
                }
                if (status != error_status_ok) {
                    free((char *)margv);
                    FREE_ACCT_PTRS(&account);
                    FREE_PK_PTRS(&pkdattrs);
                    FREE_PK_PTRS(&pkcattrs);
                    sec_psm_close(psm_handle);
                    DCP_SET_MISC_ERROR(status);
                }
            }

            /*
             * Close the pk domain.
             */
            status = sec_psm_close(psm_handle);
            if (status != error_status_ok) {
                free((char *)margv);
                FREE_ACCT_PTRS(&account);
                FREE_PK_PTRS(&pkdattrs);
                FREE_PK_PTRS(&pkcattrs);
                DCP_SET_MISC_ERROR(status);
            }
        }

        /*
         * If a pk mechanism is specified, add it.
         */
        if (account.pkmechanism) {
            sec_attr_t                 attrs;
            sec_attr_t                 out_attrs;
            unsigned32                 num_attr, num_left;
            signed32                   failed_index;
            sec_attr_component_name_t  schema_name = NULL;
            sec_attr_schema_entry_t    schema_entry;

            sec_rgy_attr_sch_lookup_by_name(rgy_context,
                                            schema_name,
                                            (idl_char *)"DCEPKPrivateKeyStorage",
                                            &schema_entry,
                                            &result);
            if (result != sec_rgy_status_ok) {
                /*
                 * The schema entry name is unknown, return an error.
                 */
                free((char *)margv);
                FREE_ACCT_PTRS(&account);
                FREE_PK_PTRS(&pkdattrs);
                FREE_PK_PTRS(&pkcattrs);
                DCP_SET_MISC_ERROR(result);
            }

            attrs.attr_id = schema_entry.attr_id;
            attrs.attr_value.attr_encoding = schema_entry.attr_encoding;
            if (strncmp("file", account.pkmechanism, strlen(account.pkmechanism)) == 0) {
                attrs.attr_value.tagged_union.uuid = sec_psm_file_mechanism;
            }
            else {
                attrs.attr_value.tagged_union.uuid = sec_psm_pkss_mechanism;
            }
            sec_rgy_attr_update(rgy_context,
                                sec_rgy_domain_person,
                                account.account_name.pname,
                                1,
                                0,
                                &attrs,
                                &num_attr,
                                &out_attrs,
                                &num_left,
                                &failed_index,
                                &result);
            if (result != sec_rgy_status_ok) {
                sec_attr_util_sch_ent_free_ptrs(&schema_entry);
                free((char *)margv);
                FREE_ACCT_PTRS(&account);
                FREE_PK_PTRS(&pkdattrs);
                FREE_PK_PTRS(&pkcattrs);
                DCP_SET_MISC_ERROR(result);
            }
            sec_attr_util_sch_ent_free_ptrs(&schema_entry);
        }
    }
    free((char *)margv);
    FREE_ACCT_PTRS(&account);
    FREE_PK_PTRS(&pkdattrs);
    FREE_PK_PTRS(&pkcattrs);
    sec_pk_data_free(&pubkey_d);
    sec_pk_data_free(&pubkey_c);
    sec_pk_data_zero_and_free(&pvtkey_d);
    sec_pk_data_zero_and_free(&pvtkey_c);
    return TCL_OK;
}
