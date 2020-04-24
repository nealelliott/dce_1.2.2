/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tty_conf.c,v $
 * Revision 1.1.8.2  1996/03/11  13:27:10  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:05  marty]
 *
 * Revision 1.1.8.1  1995/12/08  18:01:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:30:29  root]
 * 
 * Revision 1.1.6.2  1994/09/09  19:12:06  mob
 * 	Removing the temporary, incomplete ERA support from rgy_edit (OT 11652,11262)
 * 	[1994/09/09  00:38:01  mob]
 * 
 * Revision 1.1.6.1  1994/03/02  17:54:30  hanfei
 * 	Initial ERA drop
 * 	[1994/02/24  21:10:32  hanfei]
 * 
 * Revision 1.1.4.3  1992/12/29  12:41:21  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:36:19  zeliff]
 * 
 * Revision 1.1.4.2  1992/10/12  22:43:09  burati
 * 	CR5444 Don't ask for fullname/misc info in quotes
 * 	[1992/10/12  22:03:38  burati]
 * 
 * Revision 1.1.2.2  1992/04/03  20:30:14  burati
 * 	CR2419 Add support for local registry.
 * 	[1992/03/31  20:58:25  burati]
 * 
 * Revision 1.1  1992/01/19  14:45:51  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  tty_conf.c V=17 03/02/92 //littl/prgy/src/edrgy
**
** Copyright (c) Hewlett-Packard Company 1991,1992
** Unpublished work. All Rights Reserved.
**
*/
/*
**      rgy_edit tty interface configuration
*/

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)tty_conf.c	17 - 03/02/92";
#endif

#include <macros.h>

PUBLIC char *Pgo_chg_prompts[] = {
    "Change Principal=> Enter name: ",
    "Change Group=> Enter name: ",
    "Change Org=> Enter name: ",
    "Enter new UNIX number: ",
    "Enter new full name: ",
    "Enter new password:  ",
    "Include group on PROJLIST [y/n]? ",
    "Enter new name: ",
    "Enter object creation quota: "
    };

PUBLIC char *Pgo_add_prompts[] = {
    "Add Principal=> Enter name: ",
    "Add Group=> Enter name: ",
    "Add Org=> Enter name: ",
    "Enter UNIX number: ",
    "Enter full name: ",
    "Enter password:  ",
    "Include group on PROJLIST [y/n]? ",
    "",
    "Enter object creation quota: "
    };

PUBLIC char *Policy_prompts[] = {
    "Enter acct lifespan in days or 'forever': ",
    "Enter minimum password length: ",
    "Enter password lifespan in days or 'forever': ",
    "Enter password expiration date [yy/mm/dd or 'none']: ",
    "May passwords be all spaces [y/n]? ",
    "May passwords be all alphanumeric [y/n]? ",
    "Do you wish to enter policy information"
    };

PUBLIC char *Auth_policy_prompts[] = {
    "Enter maximum certificate lifetime in hours or 'forever': ",
    "Enter maximum certificate-renewable lifetime in hours or 'forever': "
    };


PUBLIC char *Property_prompts[] = {
    "Stamp registry read-only [y/n]? ",
    "Do you wish to make changes",
    "Should encrypted passwords be hidden [y/n]? ",
    "Lower bound on principal unix id for automatic UID assignment: ",
    "Lower bound on group     unix id for automatic UID assignment: ",
    "Lower bound on org       unix id for automatic UID assignment: ",
    "Maximum allowable unix id: ",
    "Minimum certificate lifetime (minutes): ",
    "Default certificate lifetime (hours): "
    };

PUBLIC char *Lrgy_property_prompts[] = {
    "Enter local registry capacity: ",
    "Enter acct lifespan in days or 'forever': "
    };

PUBLIC char *Acct_add_prompts[] = {
    "Add Account=> Enter account id [pname]: ",
    "Enter account group [gname]: ",
    "Enter account organization [oname]: ",
    "Enter abbreviation type [p/pg/pgo]: ",
    "Enter password:   ",
    "Retype password:  ",
    "Enter misc info: ",
    "Enter home directory: ",
    "Enter shell: ",
    "Password valid [y/n]? ",
    "Enter expiration date [yy/mm/dd or 'none']: ",
    "Allow account to be server principal [y/n]? ",
    "Allow account to be client principal [y/n]? ",
    "Account valid for login [y/n]? ",
    "",
    "",
    "",
    "Good since date [yy/mm/dd]: ",
    "Allow account to obtain post-dated certificates [y/n]? ",
    "Allow account to obtain forwardable certificates [y/n]? ",
    "Allow certificates to this account to be issued via TGT authentication [y/n]? ",
    "Allow account to obtain renewable certificates [y/n]? ",
    "Allow account to obtain proxiable certificates [y/n]? ",
    "Allow account to obtain duplicate session keys [y/n]? ",
    "Enter your password:   "
    };

PUBLIC char  *Acct_chg_prompts[] = {
    "Change Account=> Enter account id [pname]: ",
    "Enter account group [gname]: ",
    "Enter account organization [oname]: ",
    "Enter new abbreviation type [p/pg/pgo]: ",
    "Enter new password:   ",
    "Retype new password:  ",
    "Enter new misc info: ",
    "Enter new home directory: ",
    "Enter new shell: ",
    "Password valid [y/n]? ",
    "Enter new expiration date [yy/mm/dd or 'none']: ",
    "Allow account to be server principal [y/n]? ",
    "Allow account to be client principal [y/n]? ",
    "Account valid for login [y/n]? ",
    "Enter new account id [pname]: ",
    "Enter new account group [gname]: ",
    "Enter new account org [oname]: ",
    "Good since date [yy/mm/dd or 'now']: ",
    "Allow account to obtain post-dated certificates [y/n]? ",
    "Allow account to obtain forwardable certificates [y/n]? ",
    "Allow certificates to this account to be issued via TGT authentication [y/n]? ",
    "Allow account to obtain renewable certificates [y/n]? ",
    "Allow account to obtain proxiable certificates [y/n]? ",
    "Allow account to obtain duplicate session keys [y/n]? ",
    "Enter your password:   "
    };

PUBLIC char *Cell_add_prompts[] = {
    "Enter group name of the local account for the foreign cell: ",
    "Enter group name of the foreign account for the local cell: ",
    "Enter org name of the local account for the foreign cell: ",
    "Enter org name of the foreign account for the local cell: ",
    "Enter your password:   ",
    "Enter expiration date [yy/mm/dd or 'none']: ",
    "Enter account id to log into foreign cell with: ",
    "Enter password for foreign account: "
    };

PUBLIC char *Override_prompts[] = {
    "Change Override=> Enter override category name: ",
    "Allow password exclusion [y/n]?: ",
    "Allow root password overrides [y/n]?: ",
    "Allow non-root password overrides [y/n]?: ",
    "Allow non-password data overrides [y/n]?: ",
    "Do you wish to change override policy"
    };

PUBLIC  char *Member_update_prompts[] = {
    "Enter name to add: ",
    "Enter name to remove: ",
    "Enter group name: ",
    "Enter org name: "
    };

PUBLIC char *Default_chg_prompts[] = {
    "Enter new default abbreviation type [p/pg/pgo]: ",
    "Enter new default home directory: ",
    "Enter new default shell: ",
    "Password valid [y/n]? ",
    "Enter new default expiration date [yy/mm/dd or 'none']: ",
    "Account valid [y/n]? "
    };
