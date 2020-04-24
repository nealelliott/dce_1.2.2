/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: commands.c,v $
 * Revision 1.1.14.1  1996/08/09  11:46:20  arvind
 * 	Merge changes.
 * 	[1996/06/10  17:40 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/1]
 *
 * 	Merge with latest HP fixes and enhancements.
 * 	[1996/03/20  21:53 UTC  truitt  /main/HPDCE02/26]
 *
 * 	Merge changes.
 * 	[1996/03/20  21:50 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf70/1]
 *
 * 	CHFts16688: Bring on-line help for resolve command into synch with
 * 	the way the code actually works.
 * 	[1996/03/18  18:33 UTC  truitt  /main/HPDCE02/25]
 *
 * 	Merge changes.
 * 	[1996/03/18  18:31 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf69/1]
 *
 * 	CHFts17225: Modify some on-line help messages to match current functionality.
 * 	CHFts17407: Condense some of the sam messages that are duplicates.
 * 	[1995/11/15  21:18 UTC  truitt  /main/HPDCE02/24]
 *
 * 	Merge changes.
 * 	[1995/11/15  21:15 UTC  truitt  /main/HPDCE02/truitt_dcecp_chfts52/1]
 *
 * 	CHFts16844: Disable the 'cdsalias set' command temporarily.
 *
 * 	Initial DCE_1.2.2 branch.
 * 	[1996/06/10  14:57 UTC  truitt  /main/DCE_1.2.2/1]
 *
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:20:15  root]
 *
 * 	Merge.
 * 	[1994/12/21  19:31 UTC  truitt  /main/HPDCE02/truitt_dcecp_chk_05/1]
 *
 * 	Back out password visibility work.
 * 	[1994/12/21  14:41 UTC  truitt  /main/HPDCE02/5]
 *
 * 	Merge.
 * 	[1994/12/21  14:40 UTC  truitt  /main/HPDCE02/truitt_dcecp_chk_04/1]
 *
 * 	Add a switch to the login command to allow the
 * 	password to be visible on the command line.
 * 	[1994/12/20  14:22 UTC  truitt  /main/HPDCE02/4]
 *
 * 	Merge.
 * 	[1994/12/20  14:20 UTC  truitt  /main/HPDCE02/truitt_dcecp_chk_03/1]
 *
 * 	Put registry checkpoint options in order.
 * 	[1994/12/08  17:28 UTC  hanfei  /main/HPDCE02/3]
 *
 * 	merge
 * 	[1994/12/07  19:30 UTC  hanfei  /main/HPDCE02/hanfei_cpi_bl2/1]
 *
 * 	merge
 * 	[1994/12/05  14:47 UTC  hanfei  /main/hanfei_cpi_bl1/1]
 *
 * 	work for rgy checkpoint to be configurable
 * 	[1994/12/07  18:18 UTC  truitt  /main/HPDCE02/2]
 *
 * 	Add new function for clock synchronize.
 * 	[1994/11/03  20:16 UTC  truitt  /main/HPDCE02/1]
 *
 * 	Initial HPDCE02 branch.
 * 	[1994/11/03  20:13 UTC  truitt  /main/truitt_dcecp_01/1]
 *
 * 	Fix ordering of options in a bunch of commands.
 * 	[1994/10/05  18:10:48  melman  1.1.5.81]
 *
 * Revision 1.1.9.3  1996/02/18  19:13:07  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:59:54  marty]
 * 
 * Revision 1.1.9.2  1995/12/08  21:20:15  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/23  1995/10/19  17:13 UTC  truitt
 * 	Merge local and login work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts50/1  1995/10/19  17:11 UTC  truitt
 * 	CHFts16529: Minor changes for new dced local functionality.
 * 
 * 	HP revision /main/HPDCE02/22  1995/10/10  15:08 UTC  truitt
 * 	Merge command changes.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts49/1  1995/10/05  15:03 UTC  truitt
 * 	CHFts16537: Split 'cdsmisc' command functionality into existing 'directory'
 * 	command and new 'cds' and 'cdsclient' commands.
 * 
 * 	HP revision /main/HPDCE02/21  1995/09/29  20:46 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/2  1995/09/23  00:53 UTC  tatsu_s
 * 	Added dcp_general_unbind_local().
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/1  1995/09/21  15:29 UTC  tatsu_s
 * 	Added -local option.
 * 
 * 	HP revision /main/HPDCE02/20  1995/07/26  19:26 UTC  truitt
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts43/1  1995/07/26  19:25 UTC  truitt
 * 	CHFts14274: Change 'destroy' to 'discard'.
 * 
 * 	HP revision /main/HPDCE02/19  1995/07/24  20:21 UTC  truitt
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts42/1  1995/07/24  20:19 UTC  truitt
 * 	Finish support for new 'login' option.
 * 
 * 	HP revision /main/HPDCE02/18  1995/07/21  13:08 UTC  truitt
 * 	Merge cdscache additions.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts40/1  1995/07/21  13:07 UTC  truitt
 * 	CHFts14274: Add new cdscache command.
 * 
 * 	HP revision /main/HPDCE02/17  1995/07/20  20:15 UTC  jrr
 * 	Add -keytab support to login.
 * 	[1995/07/20  20:14 UTC  jrr  /main/HPDCE02/jrr_mothra_pure/1]
 * 
 * 	HP revision /main/HPDCE02/16  1995/06/16  18:30 UTC  truitt
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts35/1  1995/06/16  18:28 UTC  truitt
 * 	CHFts15504: Correct build warnings in dcp_init() and dcp_cleanup().
 * 	CHFts15515: Make sure signal handler gets reset after interactive
 * 	read of password.
 * 
 * 	HP revision /main/HPDCE02/15  1995/05/26  18:03 UTC  jrr
 * 	Use better function names in rpccp.c
 * 	[1995/05/24  18:31 UTC  jrr  /main/HPDCE02/jrr_dcecp_chfts15321/1]
 * 
 * 	HP revision /main/HPDCE02/14  1995/05/24  18:54 UTC  jrr
 * 	Merge fixes for CHFts15142.
 * 
 * 	HP revision /main/HPDCE02/jrr_dcecp_chfts15142/2  1995/05/23  16:40 UTC  jrr
 * 	More -refresh changes to login
 * 
 * 	HP revision /main/HPDCE02/jrr_dcecp_chfts15142/1  1995/05/23  14:17 UTC  jrr
 * 	login should support -refresh (CHFts15142).
 * 
 * 	HP revision /main/HPDCE02/13  1995/05/15  17:21 UTC  truitt
 * 	Merge to HPDCE02.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts29/1  1995/05/15  17:19 UTC  truitt
 * 	CHFts15237: Fix the way some of the new _vhelp
 * 	message are being displayed.
 * 
 * 	HP revision /main/HPDCE02/12  1995/04/28  20:07 UTC  mdf
 * 	Fix based on review comments
 * 	[1995/04/27  19:05 UTC  mdf  /main/HPDCE02/mdf_mothra_defects_3/2]
 * 
 * 	Resolve CHFts15068
 * 	[1995/04/27  15:17 UTC  mdf  /main/HPDCE02/mdf_mothra_defects_3/1]
 * 
 * 	HP revision /main/HPDCE02/11  1995/04/04  17:28 UTC  mdf
 * 	Merge for CHFts13120
 * 
 * 	HP revision /main/HPDCE02/10  1995/03/30  17:53 UTC  truitt
 * 	Merge some warranty patch work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts23/1  1995/03/30  17:51 UTC  truitt
 * 	CHFts14675: New registry functionality to support warranty
 * 	patch work.
 * 
 * 	HP revision /main/HPDCE02/9  1995/02/21  16:22 UTC  truitt
 * 	Merge convenience variable work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts14/2  1995/02/21  16:21 UTC  truitt
 * 	Fix header error.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts14/1  1995/02/21  16:18 UTC  truitt
 * 	CHFts14391: Make sure all internal dcecp commands set
 * 	     the _o convenience variable.
 * 
 * 	HP revision /main/HPDCE02/8  1995/02/14  16:01 UTC  truitt
 * 	Merge _e work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts13/1  1995/02/14  16:00 UTC  truitt
 * 	CHFts14285: When setting error codes, use the right
 * 	macros, to ensure the _e var sets set correctly.
 * 
 * 	HP revision /main/HPDCE02/7  1995/01/13  18:07 UTC  truitt
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chk_07/1  1995/01/13  18:03 UTC  truitt
 * 	Add new commands to cover some cdscp functionality
 * 	that is currently not pres
 * 
 * Revision 1.1.5.81  1994/10/05  18:10:48  melman
 * 	fixed setting of auto_path
 * 	[1994/10/05  18:00:25  melman]
 * 
 * Revision 1.1.5.80  1994/10/01  00:42:12  rousseau
 * 	Add help support to login and resolve (CR 12441).
 * 	[1994/10/01  00:41:58  rousseau]
 * 
 * Revision 1.1.5.79  1994/09/29  22:25:40  melman
 * 	add -object option to resolve
 * 	[1994/09/29  22:19:36  melman]
 * 
 * Revision 1.1.5.78  1994/09/28  15:17:10  rousseau
 * 	Fix object w/o verb error message (CR 12387).
 * 	[1994/09/28  15:17:02  rousseau]
 * 
 * Revision 1.1.5.77  1994/09/27  19:09:10  kevins
 * 	OT 12220 modified cdsalias arg_table for help messages
 * 	[1994/09/27  18:28:51  kevins]
 * 
 * Revision 1.1.5.76  1994/09/26  20:23:25  kevins
 * 	OT 12334 added cdsalias command
 * 	[1994/09/26  20:18:05  kevins]
 * 
 * Revision 1.1.5.75  1994/09/21  20:40:41  kevins
 * 	OT 12288 clearinghouse help targeted wrong arg table entry
 * 	[1994/09/21  20:35:12  kevins]
 * 
 * Revision 1.1.5.74  1994/09/20  18:00:24  kevins
 * 	OT 12262 commands that have passwords in them now fail when called in -c mode
 * 	[1994/09/20  17:59:02  kevins]
 * 
 * Revision 1.1.5.73  1994/09/16  14:50:18  kevins
 * 	Merged with changes from 1.1.5.72
 * 	[1994/09/16  14:49:56  kevins]
 * 
 * 	OT 11270 Implemented clearinghouse catalog
 * 	[1994/09/16  14:47:30  kevins]
 * 
 * Revision 1.1.5.72  1994/09/14  19:32:22  coren
 * 	Wrapped call to Tcl_InitSignalHandling() in "#ifdef _DCECP_TEST"
 * 	(CR 11832).
 * 	[1994/09/13  18:27:43  coren]
 * 
 * Revision 1.1.5.71  1994/09/01  19:04:39  truitt
 * 	OT11897: commands in xattrschema were in wrong order.
 * 	[1994/09/01  18:06:58  truitt]
 * 
 * Revision 1.1.5.70  1994/09/01  14:16:47  rousseau
 * 	Binding authentication changes
 * 	[1994/09/01  14:16:07  rousseau]
 * 
 * Revision 1.1.5.69  1994/08/29  14:20:41  ajayb
 * 	Add account generate command.
 * 	[1994/08/26  20:31:52  ajayb]
 * 
 * Revision 1.1.5.68  1994/08/24  16:56:15  baum
 * 	Merged with changes from 1.1.5.67
 * 	[1994/08/24  16:56:04  baum]
 * 
 * 	Added secval status and string bindings to secval cmds
 * 	[1994/08/24  16:54:01  baum]
 * 
 * Revision 1.1.5.67  1994/08/24  15:04:46  salamone
 * 	CR11838 - Add support for "registry set" command.
 * 	[1994/08/23  21:23:59  salamone]
 * 
 * Revision 1.1.5.66  1994/08/21  19:45:26  melman
 * 	fix various compiler warnings
 * 	[1994/08/21  19:29:41  melman]
 * 
 * Revision 1.1.5.65  1994/08/19  21:09:57  zee
 * 	Add clearinghouse repair argument.
 * 	[1994/08/19  18:10:45  zee]
 * 
 * Revision 1.1.5.64  1994/08/18  17:43:55  rousseau
 * 	Merged with changes from 1.1.5.63
 * 	[1994/08/18  17:43:33  rousseau]
 * 
 * 	Implemented help -verbose (CR 11559).
 * 	[1994/08/18  17:42:44  rousseau]
 * 
 * Revision 1.1.5.63  1994/08/17  14:27:54  truitt
 * 	OT11693: Changed command from 'schema' to 'xattrschema'.
 * 	[1994/08/16  20:59:53  truitt]
 * 
 * Revision 1.1.5.62  1994/08/12  20:03:39  kevins
 * 	OT 9691 Add registry connect command
 * 	[1994/08/12  20:00:14  kevins]
 * 
 * Revision 1.1.5.61  1994/08/08  18:51:41  melman
 * 	Merged with changes from 1.1.5.60
 * 	[1994/08/08  18:51:07  melman]
 * 
 * 	change strdup to dce_strdup
 * 	[1994/08/08  17:52:07  melman]
 * 
 * Revision 1.1.5.60  1994/08/08  18:00:59  kevins
 * 	  OT 11071 - Modified the code that manipulates the registry site
 * 	        handle cache. When invalidating the cache the appropriate
 * 	        cache needs to be targeted.
 * 	[1994/08/08  15:43:19  kevins]
 * 
 * Revision 1.1.5.59  1994/08/01  15:07:18  zee
 * 	     Add clearinghouse verify and initiate args.
 * 	     Add cdsalias commands.
 * 	[1994/07/29  20:59:25  zee]
 * 
 * Revision 1.1.5.58  1994/07/26  18:54:15  kevins
 * 	OT 9691 Added the 'name get' command
 * 	[1994/07/26  18:51:27  kevins]
 * 
 * Revision 1.1.5.57  1994/07/20  20:30:35  nav
 * 	Merged with changes from 1.1.5.56
 * 	[1994/07/20  20:30:24  nav]
 * 
 * 	Added support for schema modify and schema rename.
 * 	[1994/07/20  19:27:37  nav]
 * 
 * Revision 1.1.5.56  1994/07/20  19:28:10  salamone
 * 	Add "registry verify" support.
 * 	[1994/07/20  19:17:30  salamone]
 * 
 * Revision 1.1.5.55  1994/07/19  18:21:32  truitt
 * 	Merged with changes from 1.1.5.54
 * 	[1994/07/19  18:21:10  truitt]
 * 
 * 	OT9691: Support for schema delete
 * 	[1994/07/19  17:28:05  truitt]
 * 
 * Revision 1.1.5.54  1994/07/19  18:09:52  rousseau
 * 	Merged with changes from 1.1.5.53
 * 	[1994/07/19  18:09:40  rousseau]
 * 
 * 	Added 'dts catalog' and 'clock compare' (CR 11249 11251).
 * 	[1994/07/19  18:08:52  rousseau]
 * 
 * Revision 1.1.5.53  1994/07/14  18:13:34  truitt
 * 	OT11186: Support for schema create
 * 	[1994/07/14  17:30:10  truitt]
 * 
 * Revision 1.1.5.51  1994/07/07  15:09:37  truitt
 * 	OT11185: Support for schema catalog.
 * 	[1994/07/07  14:31:39  truitt]
 * 
 * Revision 1.1.5.50  1994/07/05  19:33:53  truitt
 * 	OT11162: Implement schema show
 * 	[1994/07/05  18:18:58  truitt]
 * 
 * Revision 1.1.5.49  1994/07/01  14:25:27  rousseau
 * 	Merged with changes from 1.1.5.48
 * 	[1994/07/01  14:25:15  rousseau]
 * 
 * 	Implemented utc subcomponent.
 * 	[1994/07/01  14:24:32  rousseau]
 * 
 * Revision 1.1.5.48  1994/06/30  19:47:02  kevins
 * 	OT 11142 resolve command now i/o with tcl lists
 * 	[1994/06/30  19:46:53  kevins]
 * 
 * Revision 1.1.5.47  1994/06/23  19:22:07  kevins
 * 	OT 9691 Added name parse command
 * 	[1994/06/23  18:43:15  kevins]
 * 
 * Revision 1.1.5.46  1994/06/20  21:03:04  salamone
 * 	Merged with previous version.
 * 	[1994/06/20  21:01:14  salamone]
 * 
 * 	Added "registry show" and "registry dump" support.
 * 	[1994/06/20  19:41:28  salamone]
 * 
 * Revision 1.1.5.45  1994/06/20  19:44:44  coren
 * 	Checking in again.
 * 	[1994/06/13  20:13:01  coren]
 * 
 * 	Added endpoint commands.
 * 	[1994/06/10  21:48:53  coren]
 * 
 * Revision 1.1.5.44  1994/06/09  16:01:48  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:47:14  devsrc]
 * 
 * Revision 1.1.5.43  1994/06/06  15:58:02  rousseau
 * 	Better error checking for errtext (CR 10818).
 * 	[1994/06/06  15:57:23  rousseau]
 * 
 * Revision 1.1.5.42  1994/06/02  17:34:39  kevins
 * 	OT 10832 Fixed argument checking for resolve
 * 	[1994/06/02  12:02:39  kevins]
 * 
 * Revision 1.1.5.41  1994/06/02  15:55:18  salamone
 * 	Moved and renamed  rgy_modify() and rgy_show() functions in
 * 	rgy.c to rgy_sa_tclif.c
 * 	[1994/06/02  15:45:21  salamone]
 * 
 * Revision 1.1.5.40  1994/06/01  17:27:24  rousseau
 * 	Fixed problem in login, errtext and shell.
 * 	[1994/06/01  17:27:08  rousseau]
 * 
 * Revision 1.1.5.39  1994/05/27  15:36:23  kevins
 * 	added uuid compare and name compare commands.
 * 	[1994/05/27  15:35:27  kevins]
 * 
 * Revision 1.1.5.38  1994/05/26  17:53:04  kevins
 * 	OT9691 Implelmented resolve command.
 * 	[1994/05/26  17:42:25  kevins]
 * 
 * Revision 1.1.5.37  1994/05/18  21:57:01  pwang
 * 	Fixed a typo.
 * 	[1994/05/18  21:20:32  pwang]
 * 
 * Revision 1.1.5.36  1994/05/12  18:29:29  salamone
 * 	Added support for sec_admin thru the registry command.
 * 	[1994/05/12  18:20:18  salamone]
 * 
 * Revision 1.1.5.35  1994/05/10  10:00:04  kevins
 * 	Merged with changes from 1.1.5.34
 * 	[1994/05/10  09:59:54  kevins]
 * 
 * 	OT 9691 Add registry show/modify commands
 * 	[1994/05/09  19:16:04  kevins]
 * 
 * Revision 1.1.5.34  1994/05/09  14:53:56  coren
 * 	Re-merge after temporarily commenting out setting of convenience variables.
 * 	Added definition of "directory merge".
 * 	[1994/04/20  15:35:08  coren]
 * 
 * Revision 1.1.5.33  1994/05/05  13:48:12  salamone
 * 	CR10537 - For every dcecp command that is an object listed in
 * 		  the dcecp_cmd_table_t table[], set $_o accordingly.
 * 	[1994/05/05  13:47:03  salamone]
 * 
 * Revision 1.1.5.32  1994/05/03  16:11:45  kevins
 * 	OT 10466 rerodered group arg_table for alpha order.
 * 	[1994/05/03  14:34:49  kevins]
 * 
 * Revision 1.1.5.31  1994/04/26  20:03:43  pwang
 * 	Added secval object (OT#10262)
 * 	[1994/04/26  19:36:15  pwang]
 * 
 * Revision 1.1.5.30  1994/04/26  15:32:40  salamone
 * 	Added "log" command support for DCE serviceability.
 * 	[1994/04/26  14:25:15  salamone]
 * 
 * Revision 1.1.5.29  1994/04/22  19:59:21  pwang
 * 	Added dced commands support (OT#10262)
 * 	[1994/04/22  19:49:06  pwang]
 * 
 * Revision 1.1.5.28  1994/04/18  16:58:42  rousseau
 * 	Second drop of audit functionality (CR 9691).
 * 	[1994/04/18  16:55:18  rousseau]
 * 
 * Revision 1.1.5.27  1994/04/11  21:28:23  melman
 * 	Merged with changes from 1.1.5.26
 * 	[1994/04/11  21:28:01  melman]
 * 
 * 	move Tcl_CreateCommand calls into loop
 * 	[1994/04/11  19:14:00  melman]
 * 
 * 	Change Tcl_CreateCommand calls to loop over table.
 * 	[1994/04/01  20:16:48  melman]
 * 
 * Revision 1.1.5.26  1994/04/08  10:39:59  kevins
 * 	OT 9691 Implemented account commands
 * 	[1994/04/08  10:39:16  kevins]
 * 
 * Revision 1.1.5.25  1994/03/31  16:47:38  rousseau
 * 	Quick fix to allow building.
 * 	[1994/03/31  16:46:54  rousseau]
 * 
 * Revision 1.1.5.24  1994/03/31  16:34:30  rousseau
 * 	Fix handling of _c and _u (CR 10100 10101).
 * 	Implement dcp_audfilter and dcp_audtrail.
 * 	[1994/03/31  16:33:30  rousseau]
 * 
 * Revision 1.1.5.23  1994/03/22  20:01:26  melman
 * 	rpccp error message cleanup
 * 	[1994/03/22  19:45:41  melman]
 * 
 * Revision 1.1.5.22  1994/03/17  21:52:36  melman
 * 	rpc command cleanup
 * 	[1994/03/17  20:46:24  melman]
 * 
 * Revision 1.1.5.21  1994/03/14  17:39:41  kevins
 * 	CR9904 Added support for help system
 * 	[1994/03/14  13:44:25  kevins]
 * 
 * Revision 1.1.5.20  1994/03/08  15:49:38  melman
 * 	init.dcecp and verb commands
 * 	[1994/03/08  14:46:00  melman]
 * 
 * Revision 1.1.5.19  1994/02/28  23:57:24  rousseau
 * 	Fixed initilization of function pointer for each object (CR 10037).
 * 	[1994/02/28  23:56:50  rousseau]
 * 
 * Revision 1.1.5.18  1994/02/14  16:52:22  rousseau
 * 	Invalidate rgy cache on login and logout for CR 9943.
 * 	[1994/02/14  16:52:04  rousseau]
 * 
 * Revision 1.1.5.17  1994/02/10  21:47:00  rousseau
 * 	Fixes for help facility problems. CR's 9917 9918 9919 9920
 * 	[1994/02/10  21:43:43  rousseau]
 * 
 * Revision 1.1.5.16  1994/02/09  18:59:54  salamone
 * 	CR9883 "acl help" and "acl <operations> help" support new help functions.
 * 	[1994/02/09  18:35:57  salamone]
 * 
 * Revision 1.1.5.15  1994/02/03  17:24:22  kevins
 * 	Merged with changes from 1.1.5.14
 * 	[1994/02/03  17:24:02  kevins]
 * 
 * 	added support for group and org objects
 * 	[1994/02/03  17:16:48  kevins]
 * 
 * Revision 1.1.5.14  1994/02/02  20:00:39  rousseau
 * 	Help support for audit commands.
 * 	[1994/02/02  19:58:37  rousseau]
 * 
 * Revision 1.1.5.12  1994/01/10  19:54:53  rousseau
 * 	Fix rios build problems.
 * 	[1994/01/10  19:53:35  rousseau]
 * 
 * Revision 1.1.5.11  1994/01/06  15:41:40  rousseau
 * 	Merged with changes from 1.1.5.10
 * 	[1994/01/06  15:41:33  rousseau]
 * 
 * 	Added aud, audfilter, audevents and autrail commands.
 * 	[1994/01/06  15:40:38  rousseau]
 * 
 * Revision 1.1.5.10  1994/01/05  16:30:25  kevins
 * 	Merged with changes from 1.1.5.9
 * 	[1994/01/05  16:30:14  kevins]
 * 
 * 	Added rgy group commands
 * 	[1994/01/05  15:55:30  kevins]
 * 
 * Revision 1.1.5.9  1994/01/02  14:52:11  rousseau
 * 	Fixed argument checking in dcp_login.
 * 	[1994/01/02  14:52:01  rousseau]
 * 
 * Revision 1.1.5.8  1993/12/30  19:58:47  rousseau
 * 	Changed include of dcemsgmsg.h to dcesvcmsg.h
 * 	[1993/12/30  19:58:02  rousseau]
 * 
 * Revision 1.1.5.7  1993/12/27  20:49:10  salamone
 * 	Initial ACL support
 * 	[1993/12/27  20:42:07  salamone]
 * 
 * Revision 1.1.5.6  1993/12/27  16:26:35  rousseau
 * 	Fixed convenience variable support for dts and clock.
 * 	[1993/12/27  16:26:22  rousseau]
 * 
 * Revision 1.1.5.5  1993/12/27  15:05:53  kevins
 * 	Remove dce_error function call
 * 	[1993/12/27  15:03:27  kevins]
 * 
 * Revision 1.1.5.4  1993/12/22  19:17:17  rousseau
 * 	Merged with changes from 1.1.5.3
 * 	[1993/12/22  19:17:11  rousseau]
 * 
 * 	Updated use of $u to use general variable support.
 * 	[1993/12/22  19:15:10  rousseau]
 * 
 * Revision 1.1.5.3  1993/12/22  18:12:36  kevins
 * 	Message catalog cleanup
 * 	[1993/12/22  18:08:00  kevins]
 * 
 * Revision 1.1.5.2  1993/12/21  16:23:25  rousseau
 * 	Initial style guide changes.
 * 	Added signal command initilization to dcpInitInterp.
 * 	[1993/12/21  16:23:10  rousseau]
 * 
 * Revision 1.1.5.1  1993/12/20  20:06:28  kevins
 * 	Fixed to conform to style guide.
 * 	[1993/12/20  18:51:45  kevins]
 * 
 * Revision 1.1.2.17  1993/12/14  19:17:10  salamone
 * 	Support for dcecp convenience variables
 * 	[1993/12/14  18:46:00  salamone]
 * 
 * Revision 1.1.2.16  1993/12/08  19:17:12  rousseau
 * 	Really fixed echo this time.
 * 	[1993/12/08  19:17:05  rousseau]
 * 
 * Revision 1.1.2.15  1993/12/06  22:36:45  rousseau
 * 	Backed out echo changes.
 * 	[1993/12/06  22:36:30  rousseau]
 * 
 * Revision 1.1.2.14  1993/11/24  19:20:18  rousseau
 * 	Fixed echo command to put result in interp.
 * 	[1993/11/24  19:20:11  rousseau]
 * 
 * Revision 1.1.2.13  1993/11/19  22:43:58  rousseau
 * 	Wrapped call to dcp_initInterp() with dcp_init Tcl command.
 * 	[1993/11/19  22:43:52  rousseau]
 * 
 * Revision 1.1.2.12  1993/11/19  22:32:10  rousseau
 * 	Moved local setup out of main() into dcp_initInterp().
 * 	Created dcecpinit Tcl command.
 * 	[1993/11/19  22:32:04  rousseau]
 * 
 * Revision 1.1.2.11  1993/11/15  00:24:40  rousseau
 * 	Added clock commands. Removed obsolete dts commands.
 * 	[1993/11/15  00:24:34  rousseau]
 * 
 * Revision 1.1.2.10  1993/11/11  19:49:47  rousseau
 * 	Added dcp_cleanup() and dcecp_cleanup command.
 * 	Overloaded exit with our own to do cleanup.
 * 	[1993/11/11  19:49:41  rousseau]
 * 
 * Revision 1.1.2.9  1993/11/11  14:31:02  rousseau
 * 	Removed references to global_conetxt_list
 * 	[1993/11/11  14:29:08  rousseau]
 * 
 * Revision 1.1.2.8  1993/11/09  18:22:58  rousseau
 * 	Changed includes for dts prototypes.
 * 	Changed parameter list for dcp_initInterp().
 * 	[1993/11/09  18:22:53  rousseau]
 * 
 * Revision 1.1.2.7  1993/11/05  19:52:54  kevins
 * 	added some rgy commands
 * 	[1993/11/05  19:46:43  kevins]
 * 
 * Revision 1.1.2.6  1993/11/05  15:44:55  rousseau
 * 	Merged with changes from 1.1.2.5
 * 	[1993/11/05  15:44:43  rousseau]
 * 
 * 	More changes to login/logout.
 * 	Added routines for dts.
 * 	[1993/11/05  15:39:22  rousseau]
 * 
 * Revision 1.1.2.5  1993/10/25  19:51:54  melman
 * 	added sys/ to wait.h include
 * 	[1993/10/25  19:51:16  melman]
 * 
 * Revision 1.1.2.4  1993/10/22  17:07:24  rousseau
 * 	Fixes in login and shell.
 * 	Upgrade to Tcl7.0
 * 	[1993/10/22  17:07:16  rousseau]
 * 
 * Revision 1.1.2.3  1993/10/21  18:18:58  rousseau
 * 	Implemented login and logout commands.
 * 	[1993/10/21  18:18:52  rousseau]
 * 
 * Revision 1.1.2.2  1993/10/19  13:51:00  rousseau
 * 	Added all ArgvInfo help strings to message catalog.
 * 	[1993/10/19  13:50:51  rousseau]
 * 
 * Revision 1.1.2.1  1993/10/18  00:09:20  rousseau
 * 	Conversion from dsh to dcecp
 * 	[1993/10/18  00:09:08  rousseau]
 * 
 * $EndLog$
 */

/*
 * MODULE: commands.c
 *
 * DESCRIPTION:
 *   Code for all the object commands.
 *
 */

/* INCLUDE FILES  */

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h> 
#include <fcntl.h>
#include <unistd.h>             /* For execl() */
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcedcpmsg.h>
#include <dce/dcesvcmsg.h>
#include <dce/rpc.h>
#include <dce/dce_error.h>
#include <krb5/krb5.h>
#include <krb5/los-proto.h>     /* for krb5_read_password */
#include <dce/tcl.h>
#include <rpccp.h>
#include <cdscp.h>
#include <dts_proto.h>
#include <rgy.h>
#include <rgy_sa_tclif.h>
#include <dcecp.h>
#include <util_help.h>
#include <util_login.h>
#include <util_signal.h>
#include <util_var.h>
#include <util_binding.h>
#include <acl_tclif.h>
#include <dcp_dts.h>
#include <aud.h>
#include <dhd.h>
#include <log_tclif.h>
#include <dcp_utc.h>

/* DECLARATIONS */

typedef struct dcecp_cmd_table_s_t {
    char *cmdName;
    Tcl_CmdProc *proc;
} dcecp_cmd_table_t;
 
/* PROTOTYPES */

int dcp_init            (ClientData, Tcl_Interp *, int, char **);
int dcp_rpcentry        (ClientData, Tcl_Interp *, int, char **);
int dcp_rpcgroup        (ClientData, Tcl_Interp *, int, char **);
int dcp_rpcprofile      (ClientData, Tcl_Interp *, int, char **);
int dcp_endpoint        (ClientData, Tcl_Interp *, int, char **);
int dcp_uuid            (ClientData, Tcl_Interp *, int, char **);
int dcp_principal       (ClientData, Tcl_Interp *, int, char **);
int dcp_group           (ClientData, Tcl_Interp *, int, char **);
int dcp_org             (ClientData, Tcl_Interp *, int, char **);
int dcp_account         (ClientData, Tcl_Interp *, int, char **);
int dcp_registry        (ClientData, Tcl_Interp *, int, char **);
int dcp_xattrschema     (ClientData, Tcl_Interp *, int, char **);
int dcp_hostdata        (ClientData, Tcl_Interp *, int, char **);
int dcp_keytab          (ClientData, Tcl_Interp *, int, char **);
int dcp_acl             (ClientData, Tcl_Interp *, int, char **);
int dcp_object          (ClientData, Tcl_Interp *, int, char **);
int dcp_directory       (ClientData, Tcl_Interp *, int, char **);
int dcp_link            (ClientData, Tcl_Interp *, int, char **);
int dcp_clearinghouse   (ClientData, Tcl_Interp *, int, char **);
int dcp_cdscache        (ClientData, Tcl_Interp *, int, char **);
int dcp_dts             (ClientData, Tcl_Interp *, int, char **);
int dcp_clock           (ClientData, Tcl_Interp *, int, char **);
int dcp_aud             (ClientData, Tcl_Interp *, int, char **);
int dcp_audfilter       (ClientData, Tcl_Interp *, int, char **);
int dcp_audevents       (ClientData, Tcl_Interp *, int, char **);
int dcp_audtrail        (ClientData, Tcl_Interp *, int, char **);
int dcp_log             (ClientData, Tcl_Interp *, int, char **);
int dcp_secval          (ClientData, Tcl_Interp *, int, char **);
int dcp_server          (ClientData, Tcl_Interp *, int, char **);
int dcp_errtext         (ClientData, Tcl_Interp *, int, char **);
int dcp_name            (ClientData, Tcl_Interp *, int, char **);
int dcp_login           (ClientData, Tcl_Interp *, int, char **);
int dcp_logout          (ClientData, Tcl_Interp *, int, char **);
int dcp_quit            (ClientData, Tcl_Interp *, int, char **);
int dcp_cleanup         (ClientData, Tcl_Interp *, int, char **);
int dcp_resolve         (ClientData, Tcl_Interp *, int, char **);
int dcp_shell           (ClientData, Tcl_Interp *, int, char **);
int dcp_echo            (ClientData, Tcl_Interp *, int, char **);
int dcp_utc             (ClientData, Tcl_Interp *, int, char **);
int dcp_cdsalias        (ClientData, Tcl_Interp *, int, char **);
int dcp_cds             (ClientData, Tcl_Interp *, int, char **);
int dcp_cdsclient       (ClientData, Tcl_Interp *, int, char **);
extern void setup_signal_handling(void);

/* INTERNAL VARIABLES */

dcecp_cmd_table_t table[] = {
    { "dcecpinit",      dcp_init },
    { "rpcentry",       dcp_rpcentry },
    { "rpcgroup",       dcp_rpcgroup },
    { "rpcprofile",     dcp_rpcprofile },
    { "endpoint",       dcp_endpoint },
    { "uuid",           dcp_uuid },
    { "principal",      dcp_principal },
    { "group",          dcp_group },
    { "organization",   dcp_org },
    { "account",        dcp_account },
    { "registry",       dcp_registry },
    { "xattrschema",    dcp_xattrschema },
    { "hostdata",       dcp_hostdata },
    { "keytab",         dcp_keytab },
    { "acl",            dcp_acl },
    { "object",         dcp_object },
    { "directory",      dcp_directory },
    { "link",           dcp_link },
    { "clearinghouse",  dcp_clearinghouse },
    { "cdscache",       dcp_cdscache },
    { "dts",            dcp_dts },
    { "clock",          dcp_clock },
    { "aud",            dcp_aud },
    { "audfilter",      dcp_audfilter },
    { "audevents",      dcp_audevents },
    { "audtrail",       dcp_audtrail },
    { "log",            dcp_log },
    { "secval",         dcp_secval },
    { "server",         dcp_server },
    { "errtext",        dcp_errtext },
    { "name",           dcp_name },
    { "login",          dcp_login },
    { "logout",         dcp_logout },
    { "quit",           dcp_quit },
    { "dcecp_cleanup",  dcp_cleanup },
    { "resolve",        dcp_resolve },
    { "shell",          dcp_shell },
    { "echo",           dcp_echo },
    { "utc",            dcp_utc },
    { "cdsalias",       dcp_cdsalias },
    { "cds",            dcp_cds },
    { "cdsclient",      dcp_cdsclient }
};


int
dcp_initInterp(Tcl_Interp *interp)
{
    int                result;
    int                i;
    dcecp_cmd_table_t  *tp;
    void               *init_var_p;
    char               cmd[128];

    static char inittclCmd[] = "if [file exists [info library]/init.tcl] {source [info library]/init.tcl}";
    static char initdcpCmd[] = "if [file exists [set dcecp_library]/init.dcecp] {source [set dcecp_library]/init.dcecp}";
    static char setautopath[] = "if {[info globals auto_path] == \"auto_path\"} {set auto_path [linsert $auto_path 0 $dcecp_library]} else {set auto_path $dcecp_library}";
    static char dcp_exit[] = "rename exit exit.old; proc exit {{status 0}} {dcecp_cleanup; exit.old $status}";

    /* Allocate space for dcecp Convenience Variables */
    init_var_p = dcp_general_init_var(interp);
    if (init_var_p == NULL){
        DCP_SET_MISC_ERROR(dcp_s_no_memory);
    }

    /* Tell Tcl about all the commands */
    i = sizeof table / sizeof table[0];
    for (tp=table; --i >= 0; tp++) {
        Tcl_CreateCommand(interp, tp->cmdName, tp->proc, NULL, NULL);
    }

#ifdef _DCECP_TEST
    /*
     * Initialize signal handling support
     */
    Tcl_InitSignalHandling(interp);
#endif

    /* Overload exit with our own exit */
    result = Tcl_Eval(interp, dcp_exit);
    if (result != TCL_OK) {
        printf("%s\n", interp->result);
        exit(1);
    }

    /* Run the init.tcl file in the Tcl library */
    result = Tcl_Eval(interp, inittclCmd);
    if (result != TCL_OK) {
        printf("%s\n", interp->result);
        exit(1);
    }

    /*
     * Set $dcecp_library
     */
    sprintf(cmd, "set dcecp_library %s/%s", dcelocal_path, DCECP_SCRIPT_DIR);
    result = Tcl_Eval(interp, cmd);
    if (result != TCL_OK) {
        printf("%s\n", interp->result);
        exit(1);
    }

    /*
     * Run the init.dcecp file in the dcecp library, 
     * dcecp_library must be set 
     */
    result = Tcl_Eval(interp, initdcpCmd);
    if (result != TCL_OK) {
        printf("%s\n", interp->result);
        exit(1);
    }

    /* 
     * Set $auto_path to use dcecp library 
     * Autoloading allows dcecp to start up without 
     * loading any script files.  The files will be loaded when 
     * their procedures are invoked.
     */
    result = Tcl_Eval(interp, setautopath);
    if (result != TCL_OK) {
        printf("%s\n", interp->result);
        exit(1);
    }

    return TCL_OK;
}


int
dcp_init(ClientData clientData, Tcl_Interp *interp,int argc, char *argv[])
{
    dcp_initInterp(interp);
    return TCL_OK;
}


int
dcp_rpcentry(ClientData clientData, Tcl_Interp *interp, 
              int argc, char *argv[])
{
    boolean32                   help_flag = FALSE;
    static int (*func)(ClientData, Tcl_Interp *, int, char **);
    dcp_ArgvInfo arg_table[] = {
        {"create",DCP_ARGV_CONSTANT,(char *)dcp_rpcentry_create,(char *)&func,
             dcp_t_rpc_create_entry_help},
        {"delete",DCP_ARGV_CONSTANT,(char *)dcp_rpcentry_delete,(char *)&func,
             dcp_t_rpc_delete_entry_help},
        {"export",DCP_ARGV_CONSTANT,(char *)dcp_rpc_export,(char *)&func,
             dcp_t_rpc_export_entry_help},
        {"import",DCP_ARGV_CONSTANT,(char *)dcp_rpc_import,(char *)&func,
             dcp_t_rpc_import_entry_help},
        {"show",DCP_ARGV_CONSTANT,(char *)dcp_rpcentry_show,(char *)&func,
             dcp_t_rpc_show_entry_help},
        {"unexport",DCP_ARGV_CONSTANT,(char *)dcp_rpc_unexport,(char *)&func,
             dcp_t_rpc_unexport_entry_help},
        {"help", DCP_ARGV_CONSTANT, (char *)TRUE,
             NULL, dcp_t_help_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };
    
    func = NULL;
    arg_table[6].dst = (char *)&help_flag;

    if (argc < 2){
        DCP_SET_RESULT_CODE(dcp_s_object_needs_verb);
        return TCL_ERROR;
    }

    /* Reset convenience variables */
    DCP_GENERAL_SET_O_VAR("rpcentry");

    if (dcp_ParseArgv(interp, &argc, argv, arg_table,
                      DCP_ARGV_NO_LEFTOVERS | DCP_ARGV_FOUND_ONE |
                      DCP_ARGV_OPERATIONS) != TCL_OK)
        return TCL_ERROR;
    
    /* Handle requests for help */
    DCP_CHECK_OBJECT_HELP(dcp_t_rpcentry_vhelp);
    
    if (func) {
        return (*func)(clientData, interp, argc, argv);
    }
    else {
        return TCL_OK;
    }
}

int
dcp_rpcgroup(ClientData clientData, Tcl_Interp *interp,
              int argc, char *argv[])
{
    boolean32                   help_flag = FALSE;
    static int (*func)(ClientData, Tcl_Interp *, int, char **);
    dcp_ArgvInfo arg_table[] = {
        {"add",DCP_ARGV_CONSTANT,(char *)dcp_rpcgroup_add ,(char *)&func,
             dcp_t_rpc_add_group_help},
        {"create",DCP_ARGV_CONSTANT,(char *)dcp_rpcentry_create,(char *)&func,
             dcp_t_rpc_create_group_help},
        {"delete",DCP_ARGV_CONSTANT,(char *)dcp_rpcgroup_delete,(char *)&func,
             dcp_t_rpc_delete_group_help},
        {"import",DCP_ARGV_CONSTANT,(char *)dcp_rpc_import,(char *)&func,
             dcp_t_rpc_import_group_help},
        {"list",DCP_ARGV_CONSTANT,(char *)dcp_rpcgroup_list,(char *)&func,
             dcp_t_rpc_list_group_help},
        {"remove",DCP_ARGV_CONSTANT,(char *)dcp_rpcgroup_remove,(char *)&func,
             dcp_t_rpc_remove_group_help},
        {"help", DCP_ARGV_CONSTANT, (char *)TRUE,
             NULL, dcp_t_help_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    func = NULL;
    arg_table[6].dst = (char *)&help_flag;

    if (argc < 2){
        DCP_SET_RESULT_CODE(dcp_s_object_needs_verb);
        return TCL_ERROR;
    }

    /* Reset convenience variables */
    DCP_GENERAL_SET_O_VAR("rpcgroup");

    if (dcp_ParseArgv(interp, &argc, argv, arg_table,
                     DCP_ARGV_NO_LEFTOVERS | DCP_ARGV_FOUND_ONE |
                     DCP_ARGV_OPERATIONS) != TCL_OK)
        return TCL_ERROR;
    
    /* Handle requests for help */
    DCP_CHECK_OBJECT_HELP(dcp_t_rpcgroup_vhelp);

    if (func) {
        return (*func)(clientData, interp, argc, argv);
    }
    else {
        return TCL_OK;
    }
}

int
dcp_rpcprofile(ClientData clientData, Tcl_Interp *interp,
                int argc, char *argv[])
{
    boolean32                   help_flag = FALSE;
    static int (*func)(ClientData, Tcl_Interp *, int, char **);
    dcp_ArgvInfo arg_table[] = {
        {"add",DCP_ARGV_CONSTANT,(char *)dcp_rpcprofile_add,(char *)&func,
             dcp_t_rpc_add_profile_help},
        {"create",DCP_ARGV_CONSTANT,(char *)dcp_rpcentry_create,(char *)&func,
             dcp_t_rpc_create_profile_help},
        {"delete",DCP_ARGV_CONSTANT,(char *)dcp_rpcprofile_delete,(char *)&func,
             dcp_t_rpc_delete_profile_help},
        {"import",DCP_ARGV_CONSTANT,(char *)dcp_rpc_import,(char *)&func,
             dcp_t_rpc_import_profile_help},
        {"list",DCP_ARGV_CONSTANT,(char *)dcp_rpcprofile_list,(char *)&func,
             dcp_t_rpc_list_profile_help},
        {"remove",DCP_ARGV_CONSTANT,(char *)dcp_rpcprofile_remove,(char *)&func,
             dcp_t_rpc_remove_profile_help},
        {"show",DCP_ARGV_CONSTANT,(char *)dcp_rpcprofile_show,(char *)&func,
             dcp_t_rpc_show_profile_help},
        {"help", DCP_ARGV_CONSTANT, (char *)TRUE,
             NULL, dcp_t_help_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    func = NULL;
    arg_table[7].dst = (char *)&help_flag;

    if (argc < 2){
        DCP_SET_RESULT_CODE(dcp_s_object_needs_verb);
        return TCL_ERROR;
    }

    /* Reset convenience variables */
    DCP_GENERAL_SET_O_VAR("rpcprofile");

    if (dcp_ParseArgv(interp, &argc, argv, arg_table,
                     DCP_ARGV_NO_LEFTOVERS | DCP_ARGV_FOUND_ONE |
                     DCP_ARGV_OPERATIONS) != TCL_OK)
        return TCL_ERROR;
    
    /* Handle requests for help */
    DCP_CHECK_OBJECT_HELP(dcp_t_rpcprofile_vhelp);

    if (func) {
        return (*func)(clientData, interp, argc, argv);
    }
    else {
        return TCL_OK;
    }
}

int
dcp_endpoint(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    boolean32                   help_flag = FALSE;
    static int (*func)(ClientData, Tcl_Interp *, int, char **);
    dcp_ArgvInfo arg_table[] = {
        {"create",DCP_ARGV_CONSTANT,(char *)dcp_endpoint_create,(char *)&func,
             dcp_t_endpoint_create_help},
        {"delete",DCP_ARGV_CONSTANT,(char *)dcp_endpoint_delete,(char *)&func,
             dcp_t_endpoint_delete_help},
        {"show",DCP_ARGV_CONSTANT,(char *)dcp_endpoint_show,(char *)&func,
             dcp_t_endpoint_show_help},
        {"help", DCP_ARGV_CONSTANT, (char *)TRUE,
             NULL, dcp_t_help_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };
    
    func = NULL;
    arg_table[3].dst = (char *)&help_flag;
    
    if (argc < 2){
        DCP_SET_RESULT_CODE(dcp_s_object_needs_verb);
        return TCL_ERROR;
    }

    /* Reset convenience variables */
    DCP_GENERAL_SET_O_VAR("endpoint");
    
    if (dcp_ParseArgv(interp, &argc, argv, arg_table,
                      DCP_ARGV_NO_LEFTOVERS | DCP_ARGV_FOUND_ONE |
                      DCP_ARGV_OPERATIONS) != TCL_OK)
        return TCL_ERROR;
    
    /* Handle requests for help */
    DCP_CHECK_OBJECT_HELP(dcp_t_endpoint_vhelp);
    
    if (func) {
        return (*func)(clientData, interp, argc, argv);
    }
    else {
        return TCL_OK;
    }
}


/*
 * FUNCTION: dcp_resolve
 *
 * OVERVIEW: This takes a server partial binding handle and an
 *    interface identifier and resolves the partial binding handle.
 *
 * INPUTS:
 *  ClientData clientData
 *  int argc 
 *  char *argv[]
 *
 * INPUT/OUTPUTS:
 *  Tcl_Interp *interp   
 *
 * RETURNS:
 *  TCL_OK on success. 
 */
int dcp_resolve(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    rpc_binding_handle_t  binding_h, out_binding;
    char                  *if_h_string;
    rpc_ep_inq_handle_t   inquiry_context;
    rpc_if_id_t           if_id;
    char                  *object_uuid_string = NULL;
    uuid_t                object_uuid;
    uuid_t                obj_uuid;
    unsigned_char_p_t     elt;
    char                  *vers_minor;
    char                  *version;
    error_status_t        st;
    char                  *end;
    unsigned_char_t       *full_binding;
    boolean32             found = FALSE;
    int                   aargc;
    char                  **aargv;
    char                  *binding_string;
    char                  *resolve_string;
    int                   bargc;
    char                  **bargv;
    boolean32             have_obj_uuid = FALSE;
    boolean32             help_flag = FALSE;

    dcp_ArgvInfo arg_table[] = {
        {"-interface", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rpc_interface_help},
        {"-object", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rpc_object_help},
        {"help", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
            dcp_t_help_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    if_h_string = NULL;

    arg_table[0].dst = (char *)&if_h_string;
    arg_table[1].dst = (char *)&object_uuid_string;
    arg_table[2].dst = (char *)&help_flag;

    DCP_GENERAL_SET_O_VAR("resolve");

    if ((argc == 3) && (strcmp(argv[1], HELP_TOKEN) == 0) &&
        (strcmp(argv[2], "-verbose") == 0)) {
        end = (char *)dce_sprintf(dcp_t_resolve_vhelp);
        DCP_APPEND_RESULT(end);
        free(end);
        return TCL_OK;
    }

    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
        return TCL_ERROR;

    /* Handle requests for help */
    if (help_flag) {
        DCP_CHECK_MISC_HELP(2);
    }

    DCP_CHECK_EXTRA_ARGS();  
    if (argc <= 1) {
        DCP_SET_RESULT_CODE(dcp_s_resolve_need_binding);
        return TCL_ERROR;
    }

    if (!if_h_string) {
        DCP_SET_RESULT_CODE(dcp_s_resolve_need_if);
        return TCL_ERROR;
    }

    /* 
     * Parse the interface id string into a uuid, major number and 
     * minor number.
     */
    if (Tcl_SplitList(interp, if_h_string, &aargc, &aargv) == TCL_ERROR)
        return TCL_ERROR;

    if (aargc > 1)
        version = aargv[1];
    else {
        version =  strchr(aargv[0], (int )',');
        if (version)
            version++;
        else {
            DCP_SET_RESULT_CODE(dcp_s_resolve_bad_version);
            return TCL_ERROR;
        }
    }

    vers_minor = strchr(version, (int )'.');

    if (vers_minor)
        vers_minor++;
    else {
        DCP_SET_RESULT_CODE(dcp_s_resolve_bad_version);
        return TCL_ERROR;
    }

    if_id.vers_minor = strtol(vers_minor, &end, 16);
    vers_minor--;
    *vers_minor = '\0';

    if_id.vers_major = strtol(version, &end, 16);
    version--;
    *version = '\0';
    uuid_from_string((unsigned char *)aargv[0], &if_id.uuid, &st);
    DCP_CHECK_UUID_ERROR(st);

    if (object_uuid_string != NULL) {
        uuid_from_string((unsigned char *)object_uuid_string, &object_uuid, &st);
        if (st != rpc_s_ok) {
            DCP_SET_RESULT_CODE(st);
            return TCL_ERROR;
        }
        have_obj_uuid = TRUE;
    }

    if (Tcl_SplitList(interp, argv[1], &bargc, &bargv) == TCL_ERROR)
        return TCL_ERROR;

    if (bargc > 1) {
        binding_string = dcp_binding_tcl_to_string(bargc, bargv, &st);
    }
    else {
        binding_string = dce_strdup(argv[1]);
    }

    rpc_binding_from_string_binding((unsigned char *)binding_string, 
                                    &binding_h, 
                                    &st);
    DCP_CHECK_RPC_ERROR(st);

    if (have_obj_uuid) {
        rpc_mgmt_ep_elt_inq_begin(binding_h,
                                  rpc_c_ep_match_by_both,
                                  &if_id,
                                  rpc_c_vers_exact,
                                  &object_uuid,
                                  &inquiry_context,
                                  &st);
    } else {
        rpc_mgmt_ep_elt_inq_begin(binding_h,
                                  rpc_c_ep_match_by_if,
                                  &if_id,
                                  rpc_c_vers_exact,
                                  NULL,
                                  &inquiry_context,
                                  &st);
    }
    DCP_CHECK_RPC_ERROR(st);
    Tcl_ResetResult(interp);
    for (;;) {
        rpc_mgmt_ep_elt_inq_next(inquiry_context,
                                 &if_id,
                                 &out_binding,
                                 &obj_uuid,
                                 &elt,
                                 &st);
        if (st == rpc_s_no_more_elements) {
            rpc_mgmt_ep_elt_inq_done(&inquiry_context, &st);
            free(binding_string);
            DCP_CHECK_RPC_ERROR(st);
            if (!found) {
                DCP_SET_RESULT_CODE(dcp_s_resolve_not_found);
                return TCL_ERROR;
            }
            else 
                return TCL_OK;
        }
        DCP_CHECK_RPC_ERROR(st);
        rpc_binding_to_string_binding(out_binding, &full_binding, &st);
        DCP_CHECK_RPC_ERROR(st);
        if (!strncmp((char *)full_binding, binding_string, strlen(binding_string))) {
            found = TRUE;
            resolve_string = dcp_binding_string_to_tcl((char *)full_binding, &st);
            if (strlen(interp->result))
                Tcl_AppendResult(interp, "\n", (char *)resolve_string, NULL);
            else
                Tcl_AppendResult(interp, (char *)resolve_string, NULL);
            free(resolve_string);
        }
        rpc_binding_free(&out_binding, &st);
        DCP_CHECK_RPC_ERROR(st);
        rpc_string_free(&full_binding, &st);
        DCP_CHECK_RPC_ERROR(st);
    }
}


/*
 * FUNCTION: dcp_principal
 *
 * OVERVIEW: 
 *        This function controls manipulation of principals in the rgy 
 *        database. Principal functions are: create, delete, show,
 *        modify and catalog. 
 * 
 *
 * INPUTS:
 * argc    The number of command line arguments passed into function.
 * argv    The actual arguements.
 *
 * INPUT/OUTPUTS:
 * interp  Structure associated with the interpreter. This will hold
 *         the error message (if any) generated from the command. This
 *         is also the location of any convienence variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int
dcp_principal(ClientData clientData, Tcl_Interp *interp,
              int argc, char *argv[])
{
    static int  (*func)(ClientData, Tcl_Interp *, int, char **);
    int         result = TCL_OK;
    boolean32   help_flag = FALSE;

    dcp_ArgvInfo arg_table[] = {
        {"catalog",DCP_ARGV_CONSTANT,(char *)principal_catalog,(char *)&func,
             dcp_t_rgy_principal_catalog_help},
        {"create",DCP_ARGV_CONSTANT,(char *)principal_create,(char *)&func,
             dcp_t_rgy_principal_create_help},
        {"delete",DCP_ARGV_CONSTANT,(char *)principal_delete,(char *)&func,
             dcp_t_rgy_principal_delete_help},
        {"modify",DCP_ARGV_CONSTANT,(char *)principal_modify,(char *)&func,
             dcp_t_rgy_principal_modify_help},
        {"rename",DCP_ARGV_CONSTANT,(char *)principal_rename,(char *)&func,
             dcp_t_rgy_principal_rename_help},
        {"show",DCP_ARGV_CONSTANT,(char *)principal_show,(char *)&func,
             dcp_t_rgy_principal_show_help},
        {"help", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
             dcp_t_help_help},
        {NULL,DCP_ARGV_END,NULL,NULL,0}
    };

    func = NULL;

    arg_table[6].dst = (char *)&help_flag;

    if (argc < 2){
        DCP_SET_RESULT_CODE(dcp_s_object_needs_verb);
        return TCL_ERROR;
    }

    DCP_GENERAL_SET_O_VAR("principal");

    if(dcp_ParseArgv(interp,&argc,argv,arg_table,
                     DCP_ARGV_NO_LEFTOVERS | DCP_ARGV_FOUND_ONE |
                     DCP_ARGV_OPERATIONS) != TCL_OK)
        return TCL_ERROR;

    /* Handle requests for help */
    DCP_CHECK_OBJECT_HELP(dcp_t_principal_vhelp);

    if (func) {
        result =  (*func)(clientData, interp, argc, argv);
    }
    else {
        return TCL_OK;
    }

    return result;
}


/*
 * FUNCTION: dcp_group
 *
 * OVERVIEW: This function controls manipulation of the groups within 
 *           then rgy database.
 * 
 *
 * INPUTS:
 * argc    The number of command line arguments passed into function.
 * argv    The actual arguements.
 *
 * INPUT/OUTPUTS:
 * interp  Structure associated with the interpreter. This will hold
 *         the error message (if any) generated from the command. This
 *         is also the location of any convienience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */

int
dcp_group(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    static int (*func)(ClientData, Tcl_Interp *, int, char **);
    int           result = TCL_OK;
    boolean32     help_flag = FALSE;

    dcp_ArgvInfo arg_table[] = {
        {"add",DCP_ARGV_CONSTANT,(char *)rgy_group_add,(char *)&func,
             dcp_t_rgy_group_add_help},
        {"catalog",DCP_ARGV_CONSTANT,(char *)rgy_group_catalog,(char *)&func,
             dcp_t_rgy_group_catalog_help},
        {"create",DCP_ARGV_CONSTANT,(char *)rgy_group_create,(char *)&func,
             dcp_t_rgy_group_create_help},
        {"delete",DCP_ARGV_CONSTANT,(char *)rgy_group_delete,(char *)&func,
             dcp_t_rgy_group_delete_help},
        {"list",DCP_ARGV_CONSTANT,(char *)rgy_group_list,(char *)&func,
             dcp_t_rgy_group_list_help},
        {"modify",DCP_ARGV_CONSTANT,(char *)rgy_group_modify,(char *)&func,
             dcp_t_rgy_group_modify_help},
        {"remove",DCP_ARGV_CONSTANT,(char *)rgy_group_remove,(char *)&func,
             dcp_t_rgy_group_remove_help},
        {"rename",DCP_ARGV_CONSTANT,(char *)rgy_group_rename,(char *)&func,
             dcp_t_rgy_group_rename_help},
        {"show",DCP_ARGV_CONSTANT,(char *)rgy_group_show,(char *)&func,
             dcp_t_rgy_group_show_help},
        {"help", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
             dcp_t_help_help},
        {NULL,DCP_ARGV_END,NULL,NULL,0}
    };

    func = NULL;
    arg_table[9].dst = (char *)&help_flag;

    if (argc < 2){
        DCP_SET_RESULT_CODE(dcp_s_object_needs_verb);
        return TCL_ERROR;
    }

    DCP_GENERAL_SET_O_VAR("group");
 
    if(dcp_ParseArgv(interp,&argc,argv,arg_table,
                     DCP_ARGV_NO_LEFTOVERS | DCP_ARGV_FOUND_ONE |
                     DCP_ARGV_OPERATIONS) != TCL_OK)
        return TCL_ERROR;
    
   /* Handle requests for help */
    DCP_CHECK_OBJECT_HELP(dcp_t_group_vhelp);

    if (func) {
        result =  (*func)(clientData, interp, argc, argv);
    }
    else {
        return TCL_OK;
    }
    return result;

}

/*
 * FUNCTION: dcp_org
 *
 * OVERVIEW: This function controls manipulation of the organizations within 
 *           then rgy database.
 * 
 *
 * INPUTS:
 * argc    The number of command line arguments passed into function.
 * argv    The actual arguements.
 *
 * INPUT/OUTPUTS:
 * interp  Structure associated with the interpreter. This will hold
 *         the error message (if any) generated from the command. This
 *         is also the location of any convienience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */

int
dcp_org(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    static int (*func)(ClientData, Tcl_Interp *, int, char **);
    int result = TCL_OK;
    boolean32     help_flag = FALSE;
    dcp_ArgvInfo arg_table[] = {
        {"add",DCP_ARGV_CONSTANT,(char *)rgy_org_add,(char *)&func,
             dcp_t_rgy_organization_add_help},
        {"catalog",DCP_ARGV_CONSTANT,(char *)rgy_org_catalog,(char *)&func,
             dcp_t_rgy_organization_catalog_help},
        {"create",DCP_ARGV_CONSTANT,(char *)rgy_org_create,(char *)&func,
             dcp_t_rgy_organization_create_help},
        {"delete",DCP_ARGV_CONSTANT,(char *)rgy_org_delete,(char *)&func,
             dcp_t_rgy_organization_delete_help},
        {"list",DCP_ARGV_CONSTANT,(char *)rgy_org_list,(char *)&func,
             dcp_t_rgy_organization_list_help},
        {"modify",DCP_ARGV_CONSTANT,(char *)rgy_org_modify,(char *)&func,
             dcp_t_rgy_organization_modify_help}, 
        {"remove",DCP_ARGV_CONSTANT,(char *)rgy_org_remove,(char *)&func,
             dcp_t_rgy_organization_remove_help},
        {"rename",DCP_ARGV_CONSTANT,(char *)rgy_org_rename,(char *)&func,
             dcp_t_rgy_organization_rename_help},
        {"show",DCP_ARGV_CONSTANT,(char *)rgy_org_show,(char *)&func,
             dcp_t_rgy_organization_show_help},
        {"help", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
             dcp_t_help_help},
        {NULL,DCP_ARGV_END,NULL,NULL,0}
    };

    func = NULL;
    arg_table[9].dst = (char *)&help_flag;

    if (argc < 2){
        DCP_SET_RESULT_CODE(dcp_s_object_needs_verb);
        return TCL_ERROR;
    }
    
    DCP_GENERAL_SET_O_VAR("organization");
    if(dcp_ParseArgv(interp,&argc,argv,arg_table,
                     DCP_ARGV_NO_LEFTOVERS | DCP_ARGV_FOUND_ONE |
                     DCP_ARGV_OPERATIONS) != TCL_OK)
        return TCL_ERROR;
    

    /* Handle requests for help */
    DCP_CHECK_OBJECT_HELP(dcp_t_organization_vhelp);

    if (func) {
        result =  (*func)(clientData, interp, argc, argv);
    }
    else {
        return TCL_OK;
    }
    return result;
}

int
dcp_account(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    static int (*func)(ClientData, Tcl_Interp *, int, char **);
    int result = TCL_OK;
    boolean32     help_flag = FALSE;
    dcp_ArgvInfo arg_table[] = {
        {"catalog",DCP_ARGV_CONSTANT,(char *)rgy_account_catalog,(char *)&func,
             dcp_t_rgy_account_catalog_help},
        {"create",DCP_ARGV_CONSTANT,(char *)rgy_account_create,(char *)&func,
             dcp_t_rgy_account_create_help},
        {"delete",DCP_ARGV_CONSTANT,(char *)rgy_account_delete,(char *)&func,
             dcp_t_rgy_account_delete_help},
        {"generate",DCP_ARGV_CONSTANT,(char *)rgy_account_generate,(char *)&func,
             dcp_t_rgy_account_generate_help},
        {"modify",DCP_ARGV_CONSTANT,(char *)rgy_account_modify,(char *)&func,
             dcp_t_rgy_account_modify_help}, 
        {"show",DCP_ARGV_CONSTANT,(char *)rgy_account_show,(char *)&func,
             dcp_t_rgy_account_show_help},
        {"help", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
             dcp_t_help_help},
        {NULL,DCP_ARGV_END,NULL,NULL,0}
    };

    func = NULL;
    arg_table[6].dst = (char *)&help_flag;

    if (argc < 2){
        DCP_SET_RESULT_CODE(dcp_s_object_needs_verb);
        return TCL_ERROR;
    }
    
    DCP_GENERAL_SET_O_VAR("account");
    if(dcp_ParseArgv(interp,&argc,argv,arg_table,
                     DCP_ARGV_NO_LEFTOVERS | DCP_ARGV_FOUND_ONE |
                     DCP_ARGV_OPERATIONS) != TCL_OK)
        return TCL_ERROR;
    

    /* Handle requests for help */
    DCP_CHECK_OBJECT_HELP(dcp_t_account_vhelp);

    if (func) {
        result =  (*func)(clientData, interp, argc, argv);
    }
    else {
        return TCL_OK;
    }
    return result;


}


/*
 * FUNCTION:
 * dcp_registry()
 *
 * OVERVIEW:
 * This function is the main driver for the "registry" command to be
 * used for security replication configuration and for the 
 * manipulation of registry-wide attributes (a.k.a. properties),
 * policies, and registry ERAs.
 *
 * INPUTS:
 * clientData Client specific data.
 * argc       The number of command line arguments passed into function.
 * argv       The actual arguements.
 *
 * INPUT/OUTPUTS:
 * interp  Structure associated with the interpreter. This will hold
 *         the error message (if any) generated from the command. This
 *         is also the location of any convienience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int
dcp_registry(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    static int  (*func)(ClientData, Tcl_Interp *, int, char **);
    int         result = TCL_OK;
    boolean32   help_flag = FALSE;

    dcp_ArgvInfo arg_table[] = {
        {"catalog", DCP_ARGV_CONSTANT, (char *)dcp_rgy_sa_catalog, (char *)&func,
            dcp_t_rgy_sa_catalog_help}, 
        {"checkpoint", DCP_ARGV_CONSTANT, (char *)dcp_rgy_sa_checkpoint, (char *)&func,
            dcp_t_rgy_sa_checkpoint_help},
        {"connect", DCP_ARGV_CONSTANT, (char *)dcp_rgy_sa_connect, (char *)&func,
            dcp_t_rgy_sa_connect_help},
        {"delete", DCP_ARGV_CONSTANT, (char *)dcp_rgy_sa_delete, (char *)&func,
            dcp_t_rgy_sa_delete_help}, 
        {"designate", DCP_ARGV_CONSTANT, (char *)dcp_rgy_sa_designate, (char *)&func,
            dcp_t_rgy_sa_designate_help}, 
        {"destroy", DCP_ARGV_CONSTANT, (char *)dcp_rgy_sa_destroy, (char *)&func,
            dcp_t_rgy_sa_destroy_help}, 
        {"disable", DCP_ARGV_CONSTANT, (char *)dcp_rgy_sa_disable, (char *)&func,
            dcp_t_rgy_sa_disable_help}, 
        {"dump", DCP_ARGV_CONSTANT, (char *)dcp_rgy_sa_dump, (char *)&func,
            dcp_t_rgy_sa_dump_help}, 
        {"enable", DCP_ARGV_CONSTANT, (char *)dcp_rgy_sa_enable, (char *)&func,
            dcp_t_rgy_sa_enable_help}, 
        {"modify", DCP_ARGV_CONSTANT, (char *)dcp_rgy_sa_modify, (char *)&func,
            dcp_t_rgy_sa_modify_help}, 
        {"replace", DCP_ARGV_CONSTANT, (char *)dcp_rgy_sa_replace, (char *)&func,
            dcp_t_rgy_sa_replace_help}, 
        {"show", DCP_ARGV_CONSTANT, (char *)dcp_rgy_sa_show, (char *)&func,
            dcp_t_rgy_sa_show_help},
        {"stop", DCP_ARGV_CONSTANT, (char *)dcp_rgy_sa_stop, (char *)&func,
            dcp_t_rgy_sa_stop_help}, 
        {"synchronize", DCP_ARGV_CONSTANT, (char *)dcp_rgy_sa_synchronize, (char *)&func,
            dcp_t_rgy_sa_synchronize_help}, 
        {"verify", DCP_ARGV_CONSTANT, (char *)dcp_rgy_sa_verify, (char *)&func,
            dcp_t_rgy_sa_verify_help},
        {"help", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
            dcp_t_help_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    func = NULL;
    arg_table[15].dst = (char *)&help_flag;

    if (argc < 2) {
        DCP_SET_RESULT_CODE(dcp_s_object_needs_verb);
        return TCL_ERROR;
    }

    DCP_GENERAL_SET_O_VAR("registry");
    if(dcp_ParseArgv(interp,&argc,argv,arg_table,
                     DCP_ARGV_NO_LEFTOVERS | DCP_ARGV_FOUND_ONE |
                     DCP_ARGV_OPERATIONS) != TCL_OK)
        return TCL_ERROR;

    /* Handle requests for help */
    DCP_CHECK_OBJECT_HELP(dcp_t_registry_vhelp);

    if (func) {
        result = (*func)(clientData, interp, argc, argv);
    }
    else {
        return TCL_OK;
    }
    return result;
}


/*
 * FUNCTION:
 * dcp_hostdata()
 *
 * OVERVIEW:
 * This function is the main driver for the "hostdata" object related
 * commands to access or manipulate data in specified host thru dcecp.
 *
 * INPUTS:
 * argc    The number of command line arguments passed into function.
 * argv    The actual arguements.
 *
 * INPUT/OUTPUTS:
 * interp  Structure associated with the interpreter. This will hold
 *         the error message (if any) generated from the command. This
 *         is also the location of any convienience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int
dcp_hostdata(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    boolean32   help_flag = FALSE;
    static int  (*func)(ClientData, Tcl_Interp *, int, char **);

    dcp_ArgvInfo arg_table[] = {
        {"catalog", DCP_ARGV_CONSTANT, (char *) dcp_hd_catalog,
            (char *)&func, dcp_t_hd_catalog_help},
        {"create", DCP_ARGV_CONSTANT, (char *) dcp_hd_create,
            (char *)&func, dcp_t_hd_create_help},
        {"delete", DCP_ARGV_CONSTANT, (char *) dcp_hd_delete,
            (char *)&func, dcp_t_hd_delete_help},
        {"modify", DCP_ARGV_CONSTANT, (char *) dcp_hd_modify,
            (char *)&func, dcp_t_hd_modify_help},
        {"show", DCP_ARGV_CONSTANT, (char *) dcp_hd_show,
            (char *)&func, dcp_t_hd_show_help},
        {"help", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
            dcp_t_help_help},
        {NULL,DCP_ARGV_END,NULL,NULL,0}
    };

    func = NULL;

    arg_table[5].dst = (char *) &help_flag;

    if (argc < 2){
        DCP_SET_RESULT_CODE(dcp_s_object_needs_verb);
        return TCL_ERROR;
    }

    /* reset the failed, list and name variable*/
    DCP_GENERAL_SET_O_VAR("hostdata");

    if ((dcp_ParseArgv(interp, &argc, argv, arg_table,
        DCP_ARGV_NO_LEFTOVERS|DCP_ARGV_FOUND_ONE|DCP_ARGV_OPERATIONS))
        != TCL_OK) {
        return TCL_ERROR;
    }

    /* Handle requests for help */
    DCP_CHECK_OBJECT_HELP(dcp_t_hostdata_vhelp);

    if (func) {
        return (*func)(clientData, interp, argc, argv);
    } else {
        return TCL_OK;
    }

} /* End of dcp_hostdata() */


/*
 * FUNCTION:
 * dcp_keytab()
 *
 * OVERVIEW:
 * This function is the main driver for the "keytab" object related
 * commands to access or manipulate keys in specified host thru dcecp.
 *
 * INPUTS:
 * argc    The number of command line arguments passed into function.
 * argv    The actual arguements.
 *
 * INPUT/OUTPUTS:
 * interp  Structure associated with the interpreter. This will hold
 *         the error message (if any) generated from the command. This
 *         is also the location of any convienience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int
dcp_keytab(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    boolean32   help_flag = FALSE;
    static int  (*func)(ClientData, Tcl_Interp *, int, char **);

    dcp_ArgvInfo arg_table[] = {
        {"add", DCP_ARGV_CONSTANT, (char *) dcp_kt_add,
            (char *)&func, dcp_t_kt_add_help},
        {"catalog", DCP_ARGV_CONSTANT, (char *) dcp_kt_catalog,
            (char *)&func, dcp_t_kt_catalog_help},
        {"create", DCP_ARGV_CONSTANT, (char *) dcp_kt_create,
            (char *)&func, dcp_t_kt_create_help},
        {"delete", DCP_ARGV_CONSTANT, (char *) dcp_kt_delete,
            (char *)&func, dcp_t_kt_delete_help},
        {"list", DCP_ARGV_CONSTANT, (char *) dcp_kt_list,
            (char *)&func, dcp_t_kt_list_help},
        {"remove", DCP_ARGV_CONSTANT, (char *) dcp_kt_remove,
            (char *)&func, dcp_t_kt_remove_help},
        {"show", DCP_ARGV_CONSTANT, (char *) dcp_kt_show,
            (char *)&func, dcp_t_kt_show_help},
        {"help", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
            dcp_t_help_help},
        {NULL,DCP_ARGV_END,NULL,NULL,0}
    };

    func = NULL;
    arg_table[7].dst = (char *) &help_flag;

    if (argc < 2){
        DCP_SET_RESULT_CODE(dcp_s_object_needs_verb);
        return TCL_ERROR;
    }

    /* reset the failed, list and name variable*/
    DCP_GENERAL_SET_O_VAR("keytab");

    if ((dcp_ParseArgv(interp, &argc, argv, arg_table,
        DCP_ARGV_NO_LEFTOVERS|DCP_ARGV_FOUND_ONE|DCP_ARGV_OPERATIONS))
        != TCL_OK) {
        return TCL_ERROR;
    }

    /* Handle requests for help */
    DCP_CHECK_OBJECT_HELP(dcp_t_keytab_vhelp);

    if (func) {
        return (*func)(clientData, interp, argc, argv);
    } else {
        return TCL_OK;
    }

} /* End of dcp_keytab() */


/*
 * FUNCTION:
 * dcp_acl()
 *
 * OVERVIEW:
 * This function is the main driver for the "acl" command to edit or
 * list an objects ACLs thru dcecp.
 *
 * INPUTS:
 * clientData Client specific data.
 * argc       The number of command line arguments passed into function.
 * argv       The actual arguements.
 *
 * INPUT/OUTPUTS:
 * interp  Structure associated with the interpreter. This will hold
 *         the error message (if any) generated from the command. This
 *         is also the location of any convienience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int
dcp_acl(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    boolean32                   help_flag = FALSE;
    static int (*func)(ClientData, Tcl_Interp *, int, char **);

    dcp_ArgvInfo arg_table[] = {
	{"check",DCP_ARGV_CONSTANT,(char *)dcp_acl_check,
	    (char *)&func, dcp_t_acl_check_help},
	{"delete",DCP_ARGV_CONSTANT,(char *)dcp_acl_delete,
	    (char *)&func, dcp_t_acl_delete_help},
	{"modify",DCP_ARGV_CONSTANT,(char *)dcp_acl_modify,
	    (char *)&func, dcp_t_acl_modify_entry_help},
	{"permissions",DCP_ARGV_CONSTANT,(char *)dcp_acl_permissions,
	    (char *)&func, dcp_t_acl_permissions_help},
	{"replace",DCP_ARGV_CONSTANT,(char *)dcp_acl_replace,
	    (char *)&func, dcp_t_acl_replace_help},
	{"show",DCP_ARGV_CONSTANT,(char *)dcp_acl_show,
	    (char *)&func, dcp_t_acl_show_help},
        {"help", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
             dcp_t_help_help},
	{(char *)NULL, DCP_ARGV_END, (char *)NULL, (char *)NULL, 0}
    };

    func = NULL;

    arg_table[6].dst = (char *)&help_flag;

    if (argc < 2){
        DCP_SET_RESULT_CODE(dcp_s_object_needs_verb);
        return TCL_ERROR;
    }

    /* Reset convenience variables */
    DCP_GENERAL_SET_O_VAR("acl");

    if ((dcp_ParseArgv(interp, &argc, argv, arg_table,
	    DCP_ARGV_NO_LEFTOVERS|DCP_ARGV_FOUND_ONE|DCP_ARGV_OPERATIONS))
    	    != TCL_OK)
	return TCL_ERROR;

    /* Handle requests for help */
    DCP_CHECK_OBJECT_HELP(dcp_t_acl_vhelp);

    if (func) {
	return (*func)(clientData, interp, argc, argv);
    } else {
        return TCL_OK;
    }

} /* End of dcp_acl() */

/*
 * FUNCTION: dcp_object
 *
 * OVERVIEW:
 *      This function controls object entries within the cds 
 *      namespace. Operations available on objects are: create,
 *      delete, modify and show. 
 *
 * INPUTS:
 * argc    The number of command line arguments passed into function.
 * argv    The actual arguements.
 *
 * INPUT/OUTPUTS:
 * interp  Structure associated with the interpreter. This will hold
 *         the error message (if any) generated from the command. This
 *         is also the location of any convienience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
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

int
dcp_object(ClientData clientData, Tcl_Interp *interp, 
           int argc, char *argv[])
{
    static int (*func)(ClientData, Tcl_Interp *, int, char **);
    int result = TCL_OK;
    boolean32     help_flag = FALSE;
    dcp_ArgvInfo arg_table[] = {
        {"create",DCP_ARGV_CONSTANT,(char *)object_create,(char *)&func,
             dcp_t_cds_create_object_help},
        {"delete",DCP_ARGV_CONSTANT,(char *)object_delete,(char *)&func,
             dcp_t_cds_delete_object_help},
        {"modify",DCP_ARGV_CONSTANT,(char *)object_modify,(char *)&func,
             dcp_t_cds_modify_object_help},
        {"show",DCP_ARGV_CONSTANT,(char *)object_show,(char *)&func,
             dcp_t_cds_show_object_help},
        {"help", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
             dcp_t_help_help},
        {NULL,DCP_ARGV_END,NULL,NULL,0}
    };

    func = NULL;
    arg_table[4].dst = (char *)&help_flag;

    if (argc < 2){
        DCP_SET_RESULT_CODE(dcp_s_object_needs_verb);
        return TCL_ERROR;
    }   

    DCP_GENERAL_SET_O_VAR("object");

    if(dcp_ParseArgv(interp,&argc,argv,arg_table,
                     DCP_ARGV_NO_LEFTOVERS | DCP_ARGV_FOUND_ONE |
                     DCP_ARGV_OPERATIONS) != TCL_OK)
        return TCL_ERROR;

    /* Handle requests for help */
    DCP_CHECK_OBJECT_HELP(dcp_t_object_vhelp);
    
    if (func) {
        result =  (*func)(clientData, interp, argc, argv);
    }
    else {
        return TCL_OK;
    }
    return result;
} /* end dcp_object */

/*
 * FUNCTION: dcp_directory()
 *
 * OVERVIEW: This function controls operations to directory entries
 *           within the cds namespace. 
 *
 * INPUTS:
 * argc    The number of command line arguments passed into function.
 * argv    The actual arguements.
 *
 * INPUT/OUTPUTS:
 * interp  Structure associated with the interpreter. This will hold
 *         the error message (if any) generated from the command. This
 *         is also the location of any convienience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */

int
dcp_directory(ClientData clientData, Tcl_Interp *interp,
              int argc, char *argv[])
{
    static int (*func)(ClientData, Tcl_Interp *, int, char **);
    int result = TCL_OK;
    boolean32     help_flag = FALSE;    
    dcp_ArgvInfo arg_table[] = {
        {"add",DCP_ARGV_CONSTANT,(char *)directory_addto,(char *)&func,
             dcp_t_cds_addto_directory_help},
        {"create",DCP_ARGV_CONSTANT,(char *)directory_create,(char *)&func,
             dcp_t_cds_create_directory_help},
        {"delete",DCP_ARGV_CONSTANT,(char *)directory_delete,(char *)&func,
             dcp_t_cds_delete_directory_help},
        {"list",DCP_ARGV_CONSTANT,(char *)directory_list,(char *)&func,
             dcp_t_cds_list_directory_help},
        {"merge",DCP_ARGV_CONSTANT,(char *)directory_merge,(char *)&func,
             dcp_t_cds_merge_directory_help},
        {"modify",DCP_ARGV_CONSTANT,(char *)directory_modify,(char *)&func,
             dcp_t_cds_modify_directory_help},
        {"remove",DCP_ARGV_CONSTANT,(char *)directory_rmfrom,(char *)&func,
             dcp_t_cds_rmfrom_directory_help},
        {"show",DCP_ARGV_CONSTANT,(char *)directory_show,(char *)&func,
             dcp_t_cds_show_directory_help},
        {"synchronize",DCP_ARGV_CONSTANT,(char *)directory_update,
             (char *)&func,dcp_t_cds_update_directory_help},
        {"help", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
             dcp_t_help_help},
         {NULL,DCP_ARGV_END,NULL,NULL,0}
    };

    func = NULL;
    arg_table[9].dst = (char *)&help_flag;

    if (argc < 2){
        DCP_SET_RESULT_CODE(dcp_s_object_needs_verb);
        return TCL_ERROR;
    }

    /* Reset convenience variables */
    DCP_GENERAL_SET_O_VAR("directory");

    if(dcp_ParseArgv(interp,&argc,argv,arg_table,
                     DCP_ARGV_NO_LEFTOVERS | DCP_ARGV_FOUND_ONE |
                     DCP_ARGV_OPERATIONS) != TCL_OK)
        return TCL_ERROR;

    /* Handle requests for help */
    DCP_CHECK_OBJECT_HELP(dcp_t_directory_vhelp);

    if (func) {
        result = (*func)(clientData, interp, argc, argv);
    }
    else {
        return TCL_OK;
    }
    return result;
} /* end dcp_directory */

/*
 * FUNCTION: dcp_link()
 *
 * OVERVIEW: This cuntion controls operations to link object within 
 *            the cds namespace. 
 *
 * INPUTS:
 * argc    The number of command line arguments passed into function.
 * argv    The actual arguements.
 *
 * INPUT/OUTPUTS:
 * interp  Structure associated with the interpreter. This will hold
 *         the error message (if any) generated from the command. This
 *         is also the location of any convienience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */

int
dcp_link(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    
    static int (*func)(ClientData, Tcl_Interp *, int, char **);
    int  result = TCL_OK;
    boolean32     help_flag = FALSE;

    dcp_ArgvInfo arg_table[] = {
        {"create",DCP_ARGV_CONSTANT,(char *)link_create,(char *)&func,
         dcp_t_cds_create_link_help},
        {"delete",DCP_ARGV_CONSTANT,(char *)link_delete,(char *)&func,
         dcp_t_cds_delete_link_help},
        {"modify",DCP_ARGV_CONSTANT,(char *)link_modify,(char *)&func,
         dcp_t_cds_modify_link_help},
        {"show",DCP_ARGV_CONSTANT,(char *)link_show,(char *)&func,
	     dcp_t_cds_show_link_help},
        {"help", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
             dcp_t_help_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    func = NULL;
    arg_table[4].dst = (char *)&help_flag;

    if (argc < 2){
        DCP_SET_RESULT_CODE(dcp_s_object_needs_verb);
        return TCL_ERROR;
    }   

    DCP_GENERAL_SET_O_VAR("link");
    if(dcp_ParseArgv(interp,&argc,argv,arg_table,
                     DCP_ARGV_NO_LEFTOVERS | DCP_ARGV_FOUND_ONE |
                     DCP_ARGV_OPERATIONS) != TCL_OK)
        return TCL_ERROR;
    

    /* Handle requests for help */
    DCP_CHECK_OBJECT_HELP(dcp_t_link_vhelp);

    if (func) {
        result = (*func)(clientData, interp, argc, argv);
 
    }
    else {
        return TCL_OK;
    }
    return result;
}

/*
 * FUNCTION: cds_clearinghouse()
 *
 * OVERVIEW: This function controls clearinghouse operations within the
 *           cds namespace. 
 * 
 *
 * INPUTS:
 * argc    The number of command line arguments passed into function.
 * argv    The actual arguements.
 *
 * INPUT/OUTPUTS:
 * interp  Structure associated with the interpreter. This will hold
 *         the error message (if any) generated from the command. This
 *         is also the location of any convienience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */

int
dcp_clearinghouse(ClientData clientData, Tcl_Interp *interp,
                  int argc, char *argv[])
{
    
    static int (*func)(ClientData, Tcl_Interp *, int, char **);
    int result = TCL_OK;
    boolean32     help_flag = FALSE;
    dcp_ArgvInfo arg_table[] = {
        {"catalog",DCP_ARGV_CONSTANT,(char *)clringhouse_catalog,(char *)&func,
             dcp_t_cds_catalog_clearinghouse_help},
        {"create",DCP_ARGV_CONSTANT,(char *)clringhouse_create,(char *)&func,
             dcp_t_cds_create_clearinghouse_help},
        {"delete",DCP_ARGV_CONSTANT,(char *)clringhouse_delete,(char *)&func,
             dcp_t_cds_delete_clearinghouse_help},
        {"disable",DCP_ARGV_CONSTANT,(char *)clringhouse_disable,(char *)&func,
             dcp_t_cds_disable_clearinghouse_help},
        {"initiate",DCP_ARGV_CONSTANT,(char *)clringhouse_initiate,
             (char *)&func,dcp_t_cds_initiate_clearinghouse_help},
        {"repair",DCP_ARGV_CONSTANT,(char *)clringhouse_repair,(char *)&func,
             dcp_t_cds_repair_clearinghouse_help},
        {"show",DCP_ARGV_CONSTANT,(char *)clringhouse_show,(char *)&func,
             dcp_t_cds_show_clearinghouse_help},
        {"verify",DCP_ARGV_CONSTANT,(char *)clringhouse_verify,
             (char *)&func,dcp_t_cds_verify_clearinghouse_help},
        {"help", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
             dcp_t_help_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    func = NULL;
    arg_table[8].dst = (char *)&help_flag;

    if (argc < 2){
        DCP_SET_RESULT_CODE(dcp_s_object_needs_verb);
        return TCL_ERROR;
    }    
   
    /* reset the failed, list and name variable*/
    
    DCP_GENERAL_SET_O_VAR("clearinghouse");
    if(dcp_ParseArgv(interp,&argc,argv,arg_table,
                     DCP_ARGV_NO_LEFTOVERS | DCP_ARGV_FOUND_ONE |
                     DCP_ARGV_OPERATIONS) != TCL_OK)
        return TCL_ERROR;

    /* Handle requests for help */
    DCP_CHECK_OBJECT_HELP(dcp_t_clearinghouse_vhelp);
    
    if (func) {
        result = (*func)(clientData, interp, argc, argv);
    }
    else {
        return TCL_OK;
    }
    return result;
}

/*
 * FUNCTION: dcp_cdscache()
 *
 * OVERVIEW: This function controls cache operations within the
 *           cds namespace. 
 * 
 *
 * INPUTS:
 * argc    The number of command line arguments passed into function.
 * argv    The actual arguements.
 *
 * INPUT/OUTPUTS:
 * interp  Structure associated with the interpreter. This will hold
 *         the error message (if any) generated from the command. This
 *         is also the location of any convienience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */

int
dcp_cdscache(ClientData clientData, Tcl_Interp *interp,
             int argc, char *argv[])
{
    static int (*func)(ClientData, Tcl_Interp *, int, char **);
    int result = TCL_OK;
    boolean32     help_flag = FALSE;
    dcp_ArgvInfo arg_table[] = {
        {"create",DCP_ARGV_CONSTANT,(char *)cache_create,(char *)&func,
             dcp_t_cds_create_cds_cache_help},
        {"delete",DCP_ARGV_CONSTANT,(char *)cache_delete,(char *)&func,
             dcp_t_cds_delete_cds_cache_help},
        {"discard",DCP_ARGV_CONSTANT,(char *)cache_discard,(char *)&func,
             dcp_t_cds_discard_cds_cache_help},
        {"dump",DCP_ARGV_CONSTANT,(char *)cache_dump,(char *)&func,
             dcp_t_cds_dump_cds_cache_help},
        {"show",DCP_ARGV_CONSTANT,(char *)cache_show,(char *)&func,
             dcp_t_cds_show_cds_cache_help},
        {"help", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
             dcp_t_help_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    func = NULL;
    arg_table[5].dst = (char *)&help_flag;

    if (argc < 2) {
        DCP_SET_RESULT_CODE(dcp_s_object_needs_verb);
        return TCL_ERROR;
    }

    DCP_GENERAL_SET_O_VAR("cdscache");
    if (dcp_ParseArgv(interp,&argc,argv,arg_table,
                      DCP_ARGV_NO_LEFTOVERS | DCP_ARGV_FOUND_ONE |
                      DCP_ARGV_OPERATIONS) != TCL_OK)
        return TCL_ERROR;

    /* Handle requests for help */
    DCP_CHECK_OBJECT_HELP(dcp_t_cdscache_vhelp);

    if (func) {
        result =  (*func)(clientData, interp, argc, argv);
    }
    else {
        return TCL_OK;
    }
    return result;
}


/*
 * FUNCTION:    dcp_dts
 *
 * OVERVIEW:    Entry point for all dts commands.
 * 
 *
 * INPUTS:
 *  clientData
 *    Client specific data.
 *
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied arguments.
 *
 * INPUT/OUTPUTS:
 *  interp
 *    A pointer to the current TCL interpreter. 
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
dcp_dts(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    boolean32                   help_flag = FALSE;
    
    static int (*func)(ClientData, Tcl_Interp *, int, char **);
    dcp_ArgvInfo arg_table[] = {
        {"activate", DCP_ARGV_CONSTANT, (char *)dts_activate, 
             (char *)&func, dcp_t_dts_activate_help}, 
        {"catalog", DCP_ARGV_CONSTANT, (char *)dts_catalog, 
             (char *)&func, dcp_t_dts_catalog_help}, 
        {"configure", DCP_ARGV_CONSTANT, (char *)dts_configure, 
             (char *)&func, dcp_t_dts_configure_help}, 
        {"deactivate", DCP_ARGV_CONSTANT, (char *)dts_deactivate,
             (char *)&func, dcp_t_dts_deactivate_help}, 
        {"modify", DCP_ARGV_CONSTANT, (char *)dts_modify, 
             (char *)&func, dcp_t_dts_modify_help}, 
        {"show", DCP_ARGV_CONSTANT, (char *)dts_show, 
             (char *)&func, dcp_t_dts_show_help}, 
        {"stop", DCP_ARGV_CONSTANT, (char *)dts_stop, 
             (char *)&func, dcp_t_dts_stop_help}, 
        {"synchronize", DCP_ARGV_CONSTANT, (char *)dts_synchronize,
             (char *)&func, dcp_t_dts_synchronize_help}, 
        {"help", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
             dcp_t_help_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    func = NULL;
    
    arg_table[8].dst = (char *)&help_flag;

    if (argc < 2){
        DCP_SET_RESULT_CODE(dcp_s_object_needs_verb);
        return TCL_ERROR;
    }

    /* Reset convenience variables */
    DCP_GENERAL_SET_O_VAR("dts");

    if (dcp_ParseArgv(interp, &argc, argv, arg_table,
                     DCP_ARGV_NO_LEFTOVERS | DCP_ARGV_FOUND_ONE |
                     DCP_ARGV_OPERATIONS) != TCL_OK)
        return TCL_ERROR;
    
    /* Handle requests for help */
    DCP_CHECK_OBJECT_HELP(dcp_t_dts_vhelp);

    if (func) {
        return (*func)(clientData, interp, argc, argv);
    }
    else {
        return TCL_OK;
    }
}

/*
 * FUNCTION:    dcp_clock
 *
 * OVERVIEW:    Entry point for all dts commands.
 * 
 *
 * INPUTS:
 *  clientData
 *    Client specific data.
 *
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied arguments.
 *
 * INPUT/OUTPUTS:
 *  interp
 *    A pointer to the current TCL interpreter. 
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
dcp_clock(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    boolean32                   help_flag = FALSE;
    
    static int (*func)(ClientData, Tcl_Interp *, int, char **);
    dcp_ArgvInfo arg_table[] = {
        {"compare", DCP_ARGV_CONSTANT, (char *)clock_compare, 
             (char *)&func, dcp_t_clock_compare_help}, 
        {"set", DCP_ARGV_CONSTANT, (char *)clock_set, (char *)&func, 
             dcp_t_clock_set_help}, 
        {"show", DCP_ARGV_CONSTANT, (char *)clock_show, (char *)&func, 
             dcp_t_clock_show_help}, 
        {"synchronize", DCP_ARGV_CONSTANT, (char *)clock_synchronize, 
             (char *)&func, dcp_t_clock_synchronize_help}, 
        {"help", DCP_ARGV_CONSTANT, (char *)TRUE,
             NULL, dcp_t_help_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    func = NULL;

    arg_table[4].dst = (char *)&help_flag;

    if (argc < 2){
        DCP_SET_RESULT_CODE(dcp_s_object_needs_verb);
        return TCL_ERROR;
    }

    /* Reset convenience variables */
    DCP_GENERAL_SET_O_VAR("clock");

    if (dcp_ParseArgv(interp, &argc, argv, arg_table,
                     DCP_ARGV_NO_LEFTOVERS | DCP_ARGV_FOUND_ONE |
                     DCP_ARGV_OPERATIONS) != TCL_OK)
        return TCL_ERROR;
    
    /* Handle requests for help */
    DCP_CHECK_OBJECT_HELP(dcp_t_clock_vhelp);

    if (func) {
        return (*func)(clientData, interp, argc, argv);
    }
    else {
        return TCL_OK;
    }
}

/*
 * FUNCTION:    dcp_aud
 *
 * OVERVIEW:    Entry point for all aud commands.
 * 
 *
 * INPUTS:
 *  clientData
 *    Client specific data.
 *
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied arguments.
 *
 * INPUT/OUTPUTS:
 *  interp
 *    A pointer to the current TCL interpreter. 
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
dcp_aud(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    boolean32                   help_flag = FALSE;
    
    static int (*func)(ClientData, Tcl_Interp *, int, char **);
    dcp_ArgvInfo arg_table[] = {
        {"disable", DCP_ARGV_CONSTANT, (char *)aud_disable, (char *)&func, 
             dcp_t_aud_disable_help}, 
        {"enable", DCP_ARGV_CONSTANT, (char *)aud_enable, (char *)&func, 
             dcp_t_aud_enable_help}, 
        {"modify", DCP_ARGV_CONSTANT, (char *)aud_modify, (char *)&func, 
             dcp_t_aud_modify_help}, 
        {"rewind", DCP_ARGV_CONSTANT, (char *)aud_rewind, (char *)&func, 
             dcp_t_aud_rewind_help}, 
        {"show", DCP_ARGV_CONSTANT, (char *)aud_show, (char *)&func, 
             dcp_t_aud_show_help}, 
        {"stop", DCP_ARGV_CONSTANT, (char *)aud_stop, (char *)&func, 
             dcp_t_aud_stop_help}, 
        {"help", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
             dcp_t_help_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    func = NULL;
    
    arg_table[6].dst = (char *)&help_flag;

    if (argc < 2) {
        DCP_SET_RESULT_CODE(dcp_s_object_needs_verb);
        return TCL_ERROR;
    }

    /* Reset convenience variables */
    DCP_GENERAL_SET_O_VAR("aud");

    if (dcp_ParseArgv(interp, &argc, argv, arg_table,
                      DCP_ARGV_NO_LEFTOVERS | DCP_ARGV_FOUND_ONE |
                      DCP_ARGV_OPERATIONS) != TCL_OK) {
        return TCL_ERROR;
    }

    /* Handle requests for help */
    DCP_CHECK_OBJECT_HELP(dcp_t_aud_vhelp);

    if (func) {
        return (*func)(clientData, interp, argc, argv);
    }
    else {
        return TCL_OK;
    }
}

/*
 * FUNCTION:    dcp_audfilter
 *
 * OVERVIEW:    Entry point for all audfilter commands.
 * 
 *
 * INPUTS:
 *  clientData
 *    Client specific data.
 *
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied arguments.
 *
 * INPUT/OUTPUTS:
 *  interp
 *    A pointer to the current TCL interpreter. 
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
dcp_audfilter(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    boolean32                   help_flag = FALSE;
    
    static int (*func)(ClientData, Tcl_Interp *, int, char **);
    dcp_ArgvInfo arg_table[] = {
        {"catalog", DCP_ARGV_CONSTANT, (char *)audfilter_catalog, 
             (char *)&func, dcp_t_audfilter_catalog_help}, 
        {"create", DCP_ARGV_CONSTANT, (char *)audfilter_create, 
             (char *)&func, dcp_t_audfilter_create_help}, 
        {"delete", DCP_ARGV_CONSTANT, (char *)audfilter_delete, 
             (char *)&func, dcp_t_audfilter_delete_help}, 
        {"modify", DCP_ARGV_CONSTANT, (char *)audfilter_modify,
             (char *)&func, dcp_t_audfilter_modify_help}, 
        {"show", DCP_ARGV_CONSTANT, (char *)audfilter_show,
             (char *)&func, dcp_t_audfilter_show_help}, 
        {"help", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
             dcp_t_help_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    func = NULL;
    
    arg_table[5].dst = (char *)&help_flag;

    if (argc < 2) {
        DCP_SET_RESULT_CODE(dcp_s_object_needs_verb);
        return TCL_ERROR;
    }

    /* Reset convenience variables */
    DCP_GENERAL_SET_O_VAR("audfilter");

    if (dcp_ParseArgv(interp, &argc, argv, arg_table,
                     DCP_ARGV_NO_LEFTOVERS | DCP_ARGV_FOUND_ONE |
                     DCP_ARGV_OPERATIONS) != TCL_OK)
        return TCL_ERROR;
    
    /* Handle requests for help */
    DCP_CHECK_OBJECT_HELP(dcp_t_audfilter_vhelp);

    if (func) {
        return (*func)(clientData, interp, argc, argv);
    }
    else {
        return TCL_OK;
    }
}


/*
 * FUNCTION:    dcp_audevents
 *
 * OVERVIEW:    Entry point for all audevents commands.
 * 
 *
 * INPUTS:
 *  clientData
 *    Client specific data.
 *
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied arguments.
 *
 * INPUT/OUTPUTS:
 *  interp
 *    A pointer to the current TCL interpreter. 
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
dcp_audevents(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    boolean32                   help_flag = FALSE;
    
    static int (*func)(ClientData, Tcl_Interp *, int, char **);
    dcp_ArgvInfo arg_table[] = {
        {"catalog", DCP_ARGV_CONSTANT, (char *)audevents_catalog, 
             (char *)&func, dcp_t_audevents_catalog_help}, 
        {"show", DCP_ARGV_CONSTANT, (char *)audevents_show,
             (char *)&func, dcp_t_audevents_show_help}, 
        {"help", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
             dcp_t_help_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    func = NULL;
    
    arg_table[2].dst = (char *)&help_flag;

    if (argc < 2) {
        DCP_SET_RESULT_CODE(dcp_s_object_needs_verb);
        return TCL_ERROR;
    }

    /* Reset convenience variables */
    DCP_GENERAL_SET_O_VAR("audevents");

    if (dcp_ParseArgv(interp, &argc, argv, arg_table,
                     DCP_ARGV_NO_LEFTOVERS | DCP_ARGV_FOUND_ONE |
                     DCP_ARGV_OPERATIONS) != TCL_OK)
        return TCL_ERROR;
    
    /* Handle requests for help */
    DCP_CHECK_OBJECT_HELP(dcp_t_audevents_vhelp);

    if (func) {
        return (*func)(clientData, interp, argc, argv);
    }
    else {
        return TCL_OK;
    }
}

/*
 * FUNCTION:    dcp_audtrail
 *
 * OVERVIEW:    Entry point for all audtrail commands.
 * 
 *
 * INPUTS:
 *  clientData
 *    Client specific data.
 *
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied arguments.
 *
 * INPUT/OUTPUTS:
 *  interp
 *    A pointer to the current TCL interpreter. 
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
dcp_audtrail(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    boolean32                   help_flag = FALSE;
    
    static int (*func)(ClientData, Tcl_Interp *, int, char **);
    dcp_ArgvInfo arg_table[] = {
        {"show", DCP_ARGV_CONSTANT, (char *)audtrail_show,
             (char *)&func, dcp_t_audtrail_show_help}, 
        {"help", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
             dcp_t_help_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    func = NULL;
    
    arg_table[1].dst = (char *)&help_flag;

    if (argc < 2) {
        DCP_SET_RESULT_CODE(dcp_s_object_needs_verb);
        return TCL_ERROR;
    }

    /* Reset convenience variables */
    DCP_GENERAL_SET_O_VAR("audtrail");

    if (dcp_ParseArgv(interp, &argc, argv, arg_table,
                     DCP_ARGV_NO_LEFTOVERS | DCP_ARGV_FOUND_ONE |
                     DCP_ARGV_OPERATIONS) != TCL_OK)
        return TCL_ERROR;
    
    /* Handle requests for help */
    DCP_CHECK_OBJECT_HELP(dcp_t_audtrail_vhelp);

    if (func) {
        return (*func)(clientData, interp, argc, argv);
    }
    else {
        return TCL_OK;
    }
}

/*
 * FUNCTION:
 * dcp_log()
 *
 * OVERVIEW:
 * This function is the main driver for the "log" command to show 
 * and modify the routing specifications and list the serviceability 
 * components registered by the specified server(s) thru dcecp.
 *
 * INPUTS:
 * clientData Client specific data.
 * argc       The number of command line arguments passed into function.
 * argv       The actual arguements.
 *
 * INPUT/OUTPUTS:
 * interp  Structure associated with the interpreter. This will hold
 *         the error message (if any) generated from the command. This
 *         is also the location of any convienience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int
dcp_log(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    boolean32                   help_flag = FALSE;
    static int (*func)(ClientData, Tcl_Interp *, int, char **);

    dcp_ArgvInfo arg_table[] = {
	{"list",DCP_ARGV_CONSTANT,(char *)dcp_log_list,
	    (char *)&func, dcp_t_log_list_help},
	{"modify",DCP_ARGV_CONSTANT,(char *)dcp_log_modify,
	    (char *)&func, dcp_t_log_modify_help},
	{"show",DCP_ARGV_CONSTANT,(char *)dcp_log_show,
	    (char *)&func, dcp_t_log_show_help},
        {"help", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
             dcp_t_help_help},
	{(char *)NULL, DCP_ARGV_END, (char *)NULL, (char *)NULL, 0}
    };

    func = NULL;

    arg_table[3].dst = (char *)&help_flag;

    if (argc < 2) {
        DCP_SET_RESULT_CODE(dcp_s_object_needs_verb);
        return TCL_ERROR;
    }

    /* Reset convenience variables */
    DCP_GENERAL_SET_O_VAR("log");

    if ((dcp_ParseArgv(interp, &argc, argv, arg_table,
	    DCP_ARGV_NO_LEFTOVERS|DCP_ARGV_FOUND_ONE|DCP_ARGV_OPERATIONS))
    	    != TCL_OK)
	return TCL_ERROR;

    /* Handle requests for help */
    DCP_CHECK_OBJECT_HELP(dcp_t_log_vhelp);

    if (func) {
	return (*func)(clientData, interp, argc, argv);
    } else {
	return TCL_OK;
    }

} /* End of dcp_log() */


/*
 * FUNCTION:
 * dcp_secval()
 *
 * OVERVIEW:
 * This function is the main driver for the "secval" object related
 * commands to access dced secval service on a specified host thru dcecp.
 *
 * INPUTS:
 * argc    The number of command line arguments passed into function.
 * argv    The actual arguements.
 *
 * INPUT/OUTPUTS:
 * interp  Structure associated with the interpreter. This will hold
 *         the error message (if any) generated from the command. This
 *         is also the location of any convienience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int
dcp_secval(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    boolean32   help_flag = FALSE;
    static int  (*func)(ClientData, Tcl_Interp *, int, char **);

    dcp_ArgvInfo arg_table[] = {
        {"activate", DCP_ARGV_CONSTANT, (char *) dcp_sv_activate,
            (char *)&func, dcp_t_sv_activate_help},
        {"deactivate", DCP_ARGV_CONSTANT, (char *) dcp_sv_deactivate,
            (char *)&func, dcp_t_sv_deactivate_help},
        {"ping", DCP_ARGV_CONSTANT, (char *) dcp_sv_ping,
            (char *)&func, dcp_t_sv_ping_help},
        {"status", DCP_ARGV_CONSTANT, (char *) dcp_sv_status,
            (char *)&func, dcp_t_sv_status_help},
        {"update", DCP_ARGV_CONSTANT, (char *) dcp_sv_update,
            (char *)&func, dcp_t_sv_update_help},
        {"help", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
            dcp_t_help_help},
        {NULL,DCP_ARGV_END,NULL,NULL,0}
    };

    func = NULL;

    arg_table[5].dst = (char *) &help_flag;

    if (argc < 2) {
        DCP_SET_RESULT_CODE(dcp_s_object_needs_verb);
        return TCL_ERROR;
    }

    /* reset the failed, list and name variable*/
    DCP_GENERAL_SET_O_VAR("secval");

    if ((dcp_ParseArgv(interp, &argc, argv, arg_table,
        DCP_ARGV_NO_LEFTOVERS|DCP_ARGV_FOUND_ONE|DCP_ARGV_OPERATIONS))
        != TCL_OK) {
        return TCL_ERROR;
    }

    /* Handle requests for help */
    DCP_CHECK_OBJECT_HELP(dcp_t_secval_vhelp);

    if (func) {
        return (*func)(clientData, interp, argc, argv);
    } else {
        return TCL_OK;
    }

} /* End of dcp_secval() */


/*
 * FUNCTION:
 * dcp_server()
 *
 * OVERVIEW:
 * This function is the main driver for the "server" object related
 * commands to access or manipulate server's configuration and running
 * object in specified host thru dcecp.
 *
 * INPUTS:
 * argc    The number of command line arguments passed into function.
 * argv    The actual arguements.
 *
 * INPUT/OUTPUTS:
 * interp  Structure associated with the interpreter. This will hold
 *         the error message (if any) generated from the command. This
 *         is also the location of any convienience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int
dcp_server(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    boolean32   help_flag = FALSE;
    static int  (*func)(ClientData, Tcl_Interp *, int, char **);

    dcp_ArgvInfo arg_table[] = {
        {"catalog", DCP_ARGV_CONSTANT, (char *) dcp_sr_catalog,
            (char *)&func, dcp_t_sr_catalog_help},
        {"create", DCP_ARGV_CONSTANT, (char *) dcp_sr_create,
            (char *)&func, dcp_t_sr_create_help},
        {"delete", DCP_ARGV_CONSTANT, (char *) dcp_sr_delete,
            (char *)&func, dcp_t_sr_delete_help},
        {"disable", DCP_ARGV_CONSTANT, (char *) dcp_sr_disable,
            (char *)&func, dcp_t_sr_disable_help},
        {"enable", DCP_ARGV_CONSTANT, (char *) dcp_sr_enable,
            (char *)&func, dcp_t_sr_enable_help},
        {"modify", DCP_ARGV_CONSTANT, (char *) dcp_sr_modify,
            (char *)&func, dcp_t_sr_modify_help},
        {"ping", DCP_ARGV_CONSTANT, (char *) dcp_sr_ping,
            (char *)&func, dcp_t_sr_ping_help},
        {"show", DCP_ARGV_CONSTANT, (char *) dcp_sr_show,
            (char *)&func, dcp_t_sr_show_help},
        {"start", DCP_ARGV_CONSTANT, (char *) dcp_sr_start,
            (char *)&func, dcp_t_sr_start_help},
        {"stop", DCP_ARGV_CONSTANT, (char *) dcp_sr_stop,
            (char *)&func, dcp_t_sr_stop_help},
        {"help", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
            dcp_t_help_help},
        {NULL,DCP_ARGV_END,NULL,NULL,0}
    };

    func = NULL;

    arg_table[10].dst = (char *) &help_flag;

    if (argc < 2) {
        DCP_SET_RESULT_CODE(dcp_s_object_needs_verb);
        return TCL_ERROR;
    }

    /* reset the failed, list and name variable*/
    DCP_GENERAL_SET_O_VAR("server");

    if ((dcp_ParseArgv(interp, &argc, argv, arg_table,
        DCP_ARGV_NO_LEFTOVERS|DCP_ARGV_FOUND_ONE|DCP_ARGV_OPERATIONS))
        != TCL_OK) {
        return TCL_ERROR;
    }

    /* Handle requests for help */
    DCP_CHECK_OBJECT_HELP(dcp_t_server_vhelp);

    if (func) {
        return (*func)(clientData, interp, argc, argv);
    } else {
        return TCL_OK;
    }

} /* End of dcp_server() */


/*
 * FUNCTION:    dcp_errtext
 *
 * OVERVIEW:    Display a message from an installed message catalog. Use
 *              a supplied argument message code to find the message.
 * 
 *
 * INPUTS:
 *  clientData
 *    Client specific data.
 *
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied arguments.
 *
 * INPUT/OUTPUTS:
 *  interp
 *    A pointer to the current TCL interpreter. 
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
dcp_errtext(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    error_status_t              err_code;
    char                        *endptr;

    DCP_GENERAL_SET_O_VAR("errtext");

    if (strcmp(argv[1], HELP_TOKEN) == 0) {
        if (argc == 2) {
            return TCL_OK;
        }
        else if ((argc == 3) && (strcmp(argv[2], "-verbose") == 0)) {
            endptr = (char *)dce_sprintf(dcp_t_errtext_vhelp);
            DCP_APPEND_RESULT(endptr);
            free(endptr);
            return TCL_OK;
        }
    }

    /*
     * Check for extraneous arguments
     */
    DCP_CHECK_EXTRA_ARGS();  

    if (argc < 2) {
        DCP_SET_RESULT_CODE(dcp_s_need_arg);
        return TCL_ERROR;
    }

    err_code = strtoul(argv[1], &endptr, 0);

    if ((err_code == 0) || (endptr[0] != '\0')) {
        DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_errtext_invalid_code, argv[1]);
        return TCL_ERROR;
    }

    DCP_SET_RESULT_CODE(err_code);
    return TCL_OK;
}


int
dcp_name(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    boolean32   help_flag = FALSE;
    static int  (*func)(ClientData, Tcl_Interp *, int, char **);

    dcp_ArgvInfo arg_table[] = {
        {"compare",DCP_ARGV_CONSTANT,(char *)dcp_name_compare,(char *)&func,
            dcp_t_rgy_name_compare_help},
        {"expand",DCP_ARGV_CONSTANT,(char *)dcp_name_expand,(char *)&func,
            dcp_t_rgy_name_expand_help},
        {"get",DCP_ARGV_CONSTANT,(char *)name_get,(char *)&func,
            dcp_t_rgy_name_get_help},
        {"parse",DCP_ARGV_CONSTANT,(char *)name_parse,(char *)&func,
            dcp_t_rgy_name_parse_help},
        {"help", DCP_ARGV_CONSTANT, (char *)TRUE,
            NULL, dcp_t_help_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    func = NULL;
    arg_table[4].dst = (char *)&help_flag;

    if (argc < 2) {
        DCP_SET_RESULT_CODE(dcp_s_object_needs_verb);
        return TCL_ERROR;
    }

    DCP_GENERAL_SET_O_VAR("name");

    if (dcp_ParseArgv(interp, &argc, argv, arg_table,
                      DCP_ARGV_NO_LEFTOVERS | DCP_ARGV_FOUND_ONE |
                      DCP_ARGV_OPERATIONS) != TCL_OK)
        return TCL_ERROR;

    /* Handle requests for help */
    DCP_CHECK_OBJECT_HELP(dcp_t_name_vhelp);

    if (func) {
        return (*func)(clientData, interp, argc, argv);
    }
    else {
        return TCL_OK;
    }
}


/*
 * FUNCTION:    dcp_login
 *
 * OVERVIEW:    Login to the DCE security service. Maintain a stack of
 *              logins for 'su' behavior.
 * 
 *
 * INPUTS:
 *  clientData
 *    Client specific data.
 *
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied arguments.
 *
 * INPUT/OUTPUTS:
 *  interp
 *    A pointer to the current TCL interpreter. 
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
dcp_login(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    unsigned_char_p_t           user = NULL;
    boolean32                   pflag = FALSE;
    boolean32                   cflag = FALSE;
    boolean32                   rflag = FALSE;
    boolean32                   setup = FALSE;
    boolean32                   reset_passwd, valid;
    boolean32                   help_flag = FALSE;
    sec_login_auth_src_t        auth_src;
    char                        *parg = NULL;
    char                        *keytab = NULL;
    unsigned32                  kvno, asvc = rpc_c_authn_dce_secret;
    sec_passwd_str_t            pbuf;
    sec_passwd_rec_t            prec;
    sec_login_handle_t          lcontext;
    int                         len;
    error_status_t              st;
    extern boolean32            check_visible_pw;

    dcp_ArgvInfo arg_table[] = {
        {"-certify", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
             dcp_t_login_certify_help},
        {"-keytab", DCP_ARGV_STRING, NULL, NULL,
             dcp_t_login_keytab_help},
        {"-password", DCP_ARGV_STRING, NULL, NULL,
             dcp_t_login_password_help},
        {"-refresh", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
             dcp_t_login_refresh_help},
        {"help", DCP_ARGV_CONSTANT, (char *)TRUE, NULL, dcp_t_help_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    arg_table[0].dst = (char *)&cflag;
    arg_table[1].dst = (char *)&keytab;
    arg_table[2].dst = (char *)&parg;
    arg_table[3].dst = (char *)&rflag;
    arg_table[4].dst = (char *)&help_flag;

    DCP_GENERAL_SET_O_VAR("login");

    if ((argc == 3) && (strcmp(argv[1], HELP_TOKEN) == 0) &&
        (strcmp(argv[2], "-verbose") == 0)) {
        parg = (char *)dce_sprintf(dcp_t_login_vhelp);
        DCP_APPEND_RESULT(parg);
        free(parg);
        return TCL_OK;
    }

    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
        return TCL_ERROR;

    /* Handle requests for help */
    if (help_flag) {
        DCP_CHECK_MISC_HELP(4);
    }

    if (rflag) {
        if (argc > 1) {
            DCP_SET_RESULT_CODE(dcp_s_login_refresh_no_user);
            return TCL_ERROR;
        }
    }  
    else {
        if (argc < 2) {
            DCP_SET_RESULT_CODE(dcp_s_login_needs_user);
            return TCL_ERROR;
        }
    }

    /* Check for extraneous arguments */
    if (rflag) {
        DCP_CHECK_EXTRA_ARGS_NO_ARG();
    }
    else {
        DCP_CHECK_EXTRA_ARGS();
        user = (unsigned_char_p_t)argv[1];
    }

    if (parg != NULL) {
        if (check_visible_pw) {
            DCP_SET_RESULT_CODE(dcp_s_illegal_command);
            return TCL_ERROR;
        }
        strncpy((char *)pbuf, parg, sec_passwd_str_max_len);
        pbuf[sec_passwd_str_max_len] = '\0';
        memset(parg, 0, strlen((char *)parg));
        pflag = TRUE;
    }

    /* -password and -keytab are mutually exclusive */
    if ((pflag) && (keytab != NULL)) {
        DCP_SET_RESULT_CODE(dcp_s_login_no_ktb_and_pass);
        return TCL_ERROR;
    }

    if (rflag) {
        sec_login_get_current_context(&lcontext, &st);
        if (st != error_status_ok) {
            DCP_SET_RESULT_CODE(dcp_s_login_get_context_fail);
            return TCL_ERROR;
        }

        if (sec_login_refresh_identity(lcontext, &st)) {
            if (st != error_status_ok) {
                DCP_SET_RESULT_CODE(dcp_s_login_refresh_fail);
                return TCL_ERROR;
            }
        }
        setup = TRUE;
    }
    else {
        setup = sec_login_setup_identity((unsigned_char_p_t)user,
                                         sec_login_no_flags,
                                         &lcontext,
                                         &st);
    }

    if (setup) {
        if (keytab) {
            if (getuid() != geteuid()) {
                DCP_SET_RESULT_CODE(dcp_s_login_euid_not_uid);
                return TCL_ERROR;
            }
            sec_login_valid_from_keytable(lcontext,
                                          asvc,
                                          keytab,
                                          0,
                                          &kvno,
                                          &reset_passwd,
                                          &auth_src,
                                          &st);
            valid = (st == error_status_ok);
        }
        else {
            if (!pflag) {
                len = sizeof(pbuf) - 1;
                if ((krb5_read_password("Enter Password: ", 
                                        NULL, 
                                        (char *)pbuf, 
                                        &len)) != 0) {
                    setup_signal_handling();
                    sec_login_release_context(&lcontext, &st);
                    DCP_SET_RESULT_CODE(dcp_s_login_bad_pw_read);
                    return TCL_ERROR;
                }
            }
            setup_signal_handling();

            prec.key.key_type = sec_passwd_plain;
            prec.key.tagged_union.plain = (unsigned char *)pbuf;
            prec.pepper = NULL;
            prec.version_number = sec_passwd_c_version_none;

            if (cflag) {
                valid = sec_login_valid_and_cert_ident(lcontext, 
                                                       &prec,
                                                       &reset_passwd, 
                                                       &auth_src,
                                                       &st);
            } else {
                valid = sec_login_validate_identity(lcontext, 
                                                    &prec, 
                                                    &reset_passwd,
                                                    &auth_src, 
                                                    &st);
            }
        }

        if (valid) {
            if (st) {
                DCP_APPEND_RESULT_CODE(st);
            }
            if (reset_passwd) {
                DCP_APPEND_RESULT_CODE(dcp_s_login_reset_password);
            }
            if (auth_src == sec_login_auth_src_local) {
                DCP_APPEND_RESULT_CODE(dcp_s_login_local_creds);
            }
            if (auth_src == sec_login_auth_src_overridden) {
                DCP_APPEND_RESULT_CODE(dcp_s_login_from_local_override);
            } 
            else {
                if (!rflag) {
                    sec_login_set_context(lcontext, &st);
                    DCP_CHECK_SEC_ERROR(st);
                }
            }
        } else {
            if (st) {
                DCP_APPEND_RESULT_CODE(st);
            }
            else {
                DCP_APPEND_RESULT_CODE(dcp_s_login_invalid_pw);
            }
            return TCL_ERROR;
        }

        /* Invalidate various login dependent caches */
        site_cache[realtime_cache].valid = false;
        dcp_dts_valid_binding = FALSE;
        dcp_aud_valid_binding = FALSE;

        /* Create a new login context structure on the global list */
        if (!rflag) {
            if (add_login_context(interp, lcontext) != TCL_OK) {
                return TCL_ERROR;
            }
        }
        return TCL_OK;
    }
    else {
        if (st) {
            DCP_APPEND_RESULT_CODE(st);
        } 
        else {
            DCP_APPEND_RESULT_CODE(dcp_s_login_invalid_user);
        }
    }
    return TCL_ERROR;
}


/*
 * FUNCTION:    dcp_logout
 *
 * OVERVIEW:    Logout of the DCE security service. Activate the next login
 *              on the stack.
 * 
 *
 * INPUTS:
 *  clientData
 *    Client specific data.
 *
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied arguments.
 *
 * INPUT/OUTPUTS:
 *  interp
 *    A pointer to the current TCL interpreter. 
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
dcp_logout(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    char  *cmd;

    DCP_GENERAL_SET_O_VAR("logout");

    if (strcmp(argv[1], HELP_TOKEN) == 0) {
        if (argc == 2) {
            return TCL_OK;
        }
        else if ((argc == 3) && (strcmp(argv[2], "-verbose") == 0)) {
            cmd = (char *)dce_sprintf(dcp_t_logout_vhelp);
            DCP_APPEND_RESULT(cmd);
            free(cmd);
            return TCL_OK;
        }
    }

    /* Check for extraneous arguments */
    DCP_CHECK_EXTRA_ARGS_NO_ARG();

    /* Remove a login_context from the global list and set the new context. */
    if (delete_login_context(interp) != TCL_OK) {
        return TCL_ERROR;
    }

    /* Invalidate various login dependant caches */
    site_cache[realtime_cache].valid = false;
    dcp_dts_valid_binding = FALSE;
    dcp_aud_valid_binding = FALSE;

    return TCL_OK;
}


/*
 * FUNCTION:    dcp_quit
 *
 * OVERVIEW:    Exit dcecp. Cleanup as necessary.
 * 
 *
 * INPUTS:
 *  clientData
 *    Client specific data.
 *
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied arguments.
 *
 * INPUT/OUTPUTS:
 *  interp
 *    A pointer to the current TCL interpreter. 
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
dcp_quit(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    char  *cmd;

    DCP_GENERAL_SET_O_VAR("quit");

    /*
     * Check for help request first.
     */
    if (strcmp(argv[1], HELP_TOKEN) == 0) {
        if (argc == 2) {
            return TCL_OK;
        }
        else if ((argc == 3) && (strcmp(argv[2], "-verbose") == 0)) {
            cmd = (char *)dce_sprintf(dcp_t_quit_vhelp);
            DCP_APPEND_RESULT(cmd);
            free(cmd);
            return TCL_OK;
        }
    }

    /*
     * Just call the Tcl command exit, regardless of
     * what else is on the command line.
     */
    Tcl_Eval(interp, "exit");
    return TCL_OK;
}


/*
 * FUNCTION:    dcp_cleanup
 *
 * OVERVIEW:    Do exit cleanup work.
 * 
 *
 * INPUTS:
 *  clientData
 *    Client specific data.
 *
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied arguments.
 *
 * INPUT/OUTPUTS:
 *  interp
 *    A pointer to the current TCL interpreter. 
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
dcp_cleanup(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    dcp_general_unbind_local(interp);
    dcp_general_unset_var(interp, "_local", (char *)NULL, 0);
    dcp_general_cleanup_var(interp);
    delete_all_login_contexts(interp);
    return TCL_OK;
}


/*
 * FUNCTION:    dcp_shell
 *
 * OVERVIEW:    With no arguments, spawns a shell. With arguments, runs
 *              the specified command and returns.
 * 
 *
 * INPUTS:
 *  clientData
 *    Client specific data.
 *
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied arguments.
 *
 * INPUT/OUTPUTS:
 *  interp
 *    A pointer to the current TCL interpreter. 
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
dcp_shell(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    char *command, *shell;
    pid_t pid;
    int w, status;

    DCP_GENERAL_SET_O_VAR("shell");

    if ((argc == 2) && (strcmp(argv[1], HELP_TOKEN) == 0)) {
        return TCL_OK;
    }

    if ((argc == 3) && (strcmp(argv[1], HELP_TOKEN) == 0) &&
        (strcmp(argv[2], "-verbose") == 0)) {
        DCP_SET_RESULT_CODE(dcp_t_shell_vhelp);
        return TCL_OK;
    }

    /* Get the correct shell */    
    shell = getenv("SHELL");
    if (shell == NULL) {
        shell = DEFAULT_SHELL;
    }

    /* If no args, just spawn $SHELL */
    if (argc == 1) {
        if ((pid = fork()) == 0) {
            /* In Child */
            execl(DEFAULT_SHELL, "sh", "-c", shell, (char *)NULL);
            exit(0);
            /* NOTREACHED */
        }
        
        if (pid == -1) {
            DCP_SET_RESULT_CODE(dcp_s_shell_fork_fail);
            return TCL_ERROR;
        }
        
        do {
            w = waitpid(pid, &status, 0);
        } while (w == -1 && errno == EINTR);
    }
    else {
        /* Execute a single command */
        command = Tcl_Merge(argc - 1, argv + 1);  
        
        status = Tcl_VarEval(interp, 
                             "exec ", 
                             command, 
                             (char *)NULL);
        free(command);
        return status;
    }
    return TCL_OK;
}

/*
 * FUNCTION:    dcp_echo
 *
 * OVERVIEW:    Evaluate and display the supplied arguments.
 * 
 *
 * INPUTS:
 *  clientData
 *    Client specific data.
 *
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied arguments.
 *
 * INPUT/OUTPUTS:
 *  interp
 *    A pointer to the current TCL interpreter. 
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
dcp_echo(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    char *cmd;

    if (argc == 1) {
        return TCL_OK;
    }

    DCP_GENERAL_SET_O_VAR("echo");

    if (strcmp(argv[1], HELP_TOKEN) == 0) {
        if (argc == 2) {
            return TCL_OK;
        }
        else if ((argc == 3) && (strcmp(argv[2], "-verbose") == 0)) {
            cmd = (char *)dce_sprintf(dcp_t_echo_vhelp);
            DCP_APPEND_RESULT(cmd);
            free(cmd);
            return TCL_OK;
        }
    }

    cmd = Tcl_Merge(argc-1, argv+1);
    puts(cmd);
    free((char *)cmd);

    return TCL_OK;
}


/*
 * FUNCTION: dcp_cdsalias()
 *
 * OVERVIEW: This function controls operations to create/delete/set/catalog
 *           cell aliases for a given cell in the cds namespace.
 *
 * INPUTS:
 * argc    The number of command line arguments passed into function.
 * argv    The actual arguments.
 *
 * INPUT/OUTPUTS:
 * interp  Structure associated with the interpreter. This will hold
 *         the error message (if any) generated from the command. This
 *         is also the location of any convenience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */

int
dcp_cdsalias(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    static int  (*func)(ClientData, Tcl_Interp *, int, char **);
    int         result = TCL_OK;
    boolean32   help_flag = FALSE;

    dcp_ArgvInfo arg_table[] = {
        {"catalog",DCP_ARGV_CONSTANT,(char *)cdsalias_catalog,(char *)&func,
             dcp_t_cds_catalog_cdsalias_help},
        {"connect",DCP_ARGV_CONSTANT,(char *)cdsalias_connect,(char *)&func,
             dcp_t_cds_connect_cdsalias_help},
        {"create",DCP_ARGV_CONSTANT,(char *)cdsalias_create,(char *)&func,
             dcp_t_cds_create_cdsalias_help},
        {"delete",DCP_ARGV_CONSTANT,(char *)cdsalias_delete,(char *)&func,
             dcp_t_cds_delete_cdsalias_help},
        {"help", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
             dcp_t_help_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    func = NULL;

    arg_table[4].dst = (char *)&help_flag;

    if (argc < 2) {
        DCP_SET_RESULT_CODE(dcp_s_object_needs_verb);
        return TCL_ERROR;
    }

    DCP_GENERAL_SET_O_VAR("cdsalias");
    if(dcp_ParseArgv(interp,&argc,argv,arg_table,
                     DCP_ARGV_NO_LEFTOVERS | DCP_ARGV_FOUND_ONE |
                     DCP_ARGV_OPERATIONS) != TCL_OK)
        return TCL_ERROR;

    /* Handle requests for help */
    DCP_CHECK_OBJECT_HELP(dcp_t_cdsalias_vhelp);

    if (func) {
        result = (*func)(clientData, interp, argc, argv);
    }
    else {
        return TCL_OK;
    }
    return result;
}


/*
 * FUNCTION: dcp_cds()
 *
 * OVERVIEW: This function controls miscellaneous cds operations
 *           that are directed to a cds server and are not covered
 *           by other dcecp commands.
 *
 * INPUTS:
 * argc    The number of command line arguments passed into function.
 * argv    The actual arguements.
 *
 * INPUT/OUTPUTS:
 * interp  Structure associated with the interpreter. This will hold
 *         the error message (if any) generated from the command. This
 *         is also the location of any convienience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */

int
dcp_cds(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    static int  (*func)(ClientData, Tcl_Interp *, int, char **);
    int         result = TCL_OK;
    boolean32   help_flag = FALSE;

    dcp_ArgvInfo  arg_table[] = {
        {"disable", DCP_ARGV_CONSTANT,(char *)cds_disable,(char *)&func,
            dcp_t_cds_disable_cds_help},
        {"show",DCP_ARGV_CONSTANT,(char *)cds_show,(char *)&func,
            dcp_t_cds_show_cds_help},
        {"help", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
            dcp_t_help_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    func = NULL;
    arg_table[2].dst = (char *)&help_flag;

    if (argc < 2) {
        DCP_SET_RESULT_CODE(dcp_s_object_needs_verb);
        return TCL_ERROR;
    }

    DCP_GENERAL_SET_O_VAR("cds");
    if (dcp_ParseArgv(interp, &argc, argv, arg_table,
                      DCP_ARGV_NO_LEFTOVERS | DCP_ARGV_FOUND_ONE |
                      DCP_ARGV_OPERATIONS) != TCL_OK)
        return TCL_ERROR;

    /*
     * Handle requests for help.
     */
    DCP_CHECK_OBJECT_HELP(dcp_t_cds_vhelp);

    if (func) {
        result = (*func)(clientData, interp, argc, argv);
    }
    else {
        return TCL_OK;
    }
    return result;
}


/*
 * FUNCTION: dcp_cdsclient()
 *
 * OVERVIEW: This function controls miscellaneous cds operations
 *           that are directed to a cds client and are not covered
 *           by other dcecp commands.
 *
 * INPUTS:
 * argc    The number of command line arguments passed into function.
 * argv    The actual arguements.
 *
 * INPUT/OUTPUTS:
 * interp  Structure associated with the interpreter. This will hold
 *         the error message (if any) generated from the command. This
 *         is also the location of any convienience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */

int
dcp_cdsclient(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[])
{
    static int  (*func)(ClientData, Tcl_Interp *, int, char **);
    int         result = TCL_OK;
    boolean32   help_flag = FALSE;

    dcp_ArgvInfo  arg_table[] = {
        {"disable", DCP_ARGV_CONSTANT,(char *)cdsclient_disable,(char *)&func,
            dcp_t_cds_disable_cdsclient_help},
        {"show",DCP_ARGV_CONSTANT,(char *)cdsclient_show,(char *)&func,
            dcp_t_cds_show_cdsclient_help},
        {"help", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
            dcp_t_help_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    func = NULL;
    arg_table[2].dst = (char *)&help_flag;

    if (argc < 2) {
        DCP_SET_RESULT_CODE(dcp_s_object_needs_verb);
        return TCL_ERROR;
    }

    DCP_GENERAL_SET_O_VAR("cdsclient");
    if (dcp_ParseArgv(interp, &argc, argv, arg_table,
                      DCP_ARGV_NO_LEFTOVERS | DCP_ARGV_FOUND_ONE |
                      DCP_ARGV_OPERATIONS) != TCL_OK)
        return TCL_ERROR;

    /*
     * Handle requests for help.
     */
    DCP_CHECK_OBJECT_HELP(dcp_t_cdsclient_vhelp);

    if (func) {
        result = (*func)(clientData, interp, argc, argv);
    }
    else {
        return TCL_OK;
    }
    return result;
}


/*
 * FUNCTION: dcp_utc
 *
 * OVERVIEW:
 *  Entry point for all utc commands.
 * 
 * INPUTS:
 *  clientData
 *    Client specific data.
 *
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied arguments.
 *
 * INPUT/OUTPUTS:
 *  interp
 *    A pointer to the current TCL interpreter. 
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int dcp_utc(ClientData clientData, Tcl_Interp *interp,
            int argc, char *argv[])
{
    boolean32   help_flag = FALSE;
    static int  (*func)(ClientData, Tcl_Interp *, int, char **);

    dcp_ArgvInfo arg_table[] = {
        {"add", DCP_ARGV_CONSTANT, (char *)utc_add, (char *)&func,
             dcp_t_utc_add_help},
        {"compare", DCP_ARGV_CONSTANT, (char *)utc_compare, (char *)&func,
             dcp_t_utc_compare_help},
        {"convert", DCP_ARGV_CONSTANT, (char *)utc_convert, (char *)&func,
             dcp_t_utc_convert_help},
        {"multiply", DCP_ARGV_CONSTANT, (char *)utc_multiply, (char *)&func,
             dcp_t_utc_multiply_help},
        {"subtract", DCP_ARGV_CONSTANT, (char *)utc_subtract, (char *)&func,
             dcp_t_utc_subtract_help},
        {"help", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
             dcp_t_help_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    func = NULL;
    arg_table[5].dst = (char *)&help_flag;

    if (argc < 2) {
        DCP_SET_RESULT_CODE(dcp_s_object_needs_verb);
        return TCL_ERROR;
    }

    DCP_GENERAL_SET_O_VAR("utc");

    if (dcp_ParseArgv(interp, &argc, argv, arg_table,
                     DCP_ARGV_NO_LEFTOVERS | DCP_ARGV_FOUND_ONE |
                     DCP_ARGV_OPERATIONS) != TCL_OK)
        return TCL_ERROR;
    
    /*
     * Handle requests for help.
     */
    DCP_CHECK_OBJECT_HELP(dcp_t_utc_vhelp);

    if (func) {
        return (*func)(clientData, interp, argc, argv);
    }
    else {
        return TCL_OK;
    }
}


/*
 * FUNCTION: dcp_uuid
 *
 * OVERVIEW:
 *  This function controls manipulation of uuids.
 *
 * INPUTS:
 *  argc - The number of command line arguments passed into function.
 *  argv - The actual arguements.
 *
 * INPUT/OUTPUTS:
 *  interp  Structure associated with the interpreter. This will hold
 *          the error message (if any) generated from the command. This
 *          is also the location of any convienience variables.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK - Notify the interpreter of successful completion
 *  TCL_ERROR - Notify the interpreter of command failure
 */
int dcp_uuid(ClientData clientData, Tcl_Interp *interp,
             int argc, char *argv[])
{
    boolean32   help_flag = FALSE;
    static int  (*func)(ClientData, Tcl_Interp *, int, char **);

    dcp_ArgvInfo arg_table[] = {
        {"compare", DCP_ARGV_CONSTANT, (char *)dcp_rpc_uuid_compare, (char *)&func,
             dcp_t_rpc_compare_uuid_help},
        {"create", DCP_ARGV_CONSTANT, (char *)dcp_rpc_uuid_create, (char *)&func,
             dcp_t_rpc_create_uuid_help},
        {"help", DCP_ARGV_CONSTANT, (char *)TRUE,
             NULL, dcp_t_help_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    func = NULL;
    arg_table[2].dst = (char *)&help_flag;

    if (argc < 2) {
        DCP_SET_RESULT_CODE(dcp_s_object_needs_verb);
        return TCL_ERROR;
    }

    DCP_GENERAL_SET_O_VAR("uuid");

    if (dcp_ParseArgv(interp, &argc, argv, arg_table,
                     DCP_ARGV_NO_LEFTOVERS | DCP_ARGV_FOUND_ONE |
                     DCP_ARGV_OPERATIONS) != TCL_OK)
        return TCL_ERROR;

    /*
     * Handle requests for help
     */
    DCP_CHECK_OBJECT_HELP(dcp_t_uuid_vhelp);

    if (func) {
        return (*func)(clientData, interp, argc, argv);
    }
    else {
        return TCL_OK;
    }
}


/*
 * FUNCTION: dcp_xattrschema
 *
 * OVERVIEW:
 *  This function controls manipulation of schema entries.
 *
 * INPUTS:
 *  argc - The number of command line arguments passed into function.
 *  argv - The actual arguements.
 *
 * INPUT/OUTPUTS:
 *  interp  Structure associated with the interpreter. This will hold
 *          the error message (if any) generated from the command. This
 *          is also the location of any convienience variables.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK - Notify the interpreter of successful completion
 *  TCL_ERROR - Notify the interpreter of command failure
 */
int dcp_xattrschema(ClientData clientData, Tcl_Interp *interp,
                    int argc, char *argv[])
{
    boolean32   help_flag = FALSE;
    static int  (*func)(ClientData, Tcl_Interp *, int, char **);

    dcp_ArgvInfo arg_table[] = {
        {"catalog", DCP_ARGV_CONSTANT, (char *)schema_catalog, (char *)&func,
            dcp_t_rgy_xattrschema_catalog_help},
        {"create", DCP_ARGV_CONSTANT, (char *)schema_create, (char *)&func,
            dcp_t_rgy_xattrschema_create_help},
        {"delete", DCP_ARGV_CONSTANT, (char *)schema_delete, (char *)&func,
            dcp_t_rgy_xattrschema_delete_help},
        {"modify", DCP_ARGV_CONSTANT, (char *)schema_modify, (char *)&func,
            dcp_t_rgy_xattrschema_modify_help},
        {"rename", DCP_ARGV_CONSTANT, (char *)schema_rename, (char *)&func,
            dcp_t_rgy_xattrschema_rename_help},
        {"show", DCP_ARGV_CONSTANT, (char *)schema_show, (char *)&func,
            dcp_t_rgy_xattrschema_show_help},
        {"help", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
            dcp_t_help_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    func = NULL;
    arg_table[6].dst = (char *)&help_flag;

    if (argc < 2) {
        DCP_SET_RESULT_CODE(dcp_s_object_needs_verb);
        return TCL_ERROR;
    }

    DCP_GENERAL_SET_O_VAR("xattrschema");

    if (dcp_ParseArgv(interp, &argc, argv, arg_table,
                      DCP_ARGV_NO_LEFTOVERS | DCP_ARGV_FOUND_ONE |
                      DCP_ARGV_OPERATIONS) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * Handle requests for help.
     */
    DCP_CHECK_OBJECT_HELP(dcp_t_xattrschema_vhelp);

    if (func) {
        return (*func)(clientData, interp, argc, argv);
    }
    else {
        return TCL_OK;
    }
}
