:*		
:* @OSF_COPYRIGHT@
:* COPYRIGHT NOTICE
:* Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
:* ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
:* src directory for the full copyright text.
:* HISTORY
:* $Log: Remove_object.sv,v $
:* Revision 1.1.6.2  1996/03/11  02:41:16  marty
:* 	Update OSF copyright years
:* 	[1996/03/10  20:06:22  marty]
:*
:* 	Ported to DCE 1.0.2 GDS.
:*
:* 	Defect 7071.
:* 	[1993/03/24  19:30:49  mhickey]
:*
:* Revision 1.1.6.1  1995/12/11  21:57:39  root
:* 	Submit OSF/DCE 1.2.1
:* 	[1995/12/11  20:57:22  root]
:* 
:* Revision 1.1.4.1  1994/07/21  20:19:53  mcg
:* 	Changes to gdsditadm required changes
:* 	to the .sv screen drivers
:* 	This Fixes OT 10240
:* 	[1994/07/21  20:19:41  mcg]
:* 
:* Revision 1.1.2.1  1993/09/20  22:40:38  pellis
:* 	Moved from ./systest/directory/gds.
:* 	[1993/09/20  22:36:53  pellis]
:* 
:* Revision 1.1.2.2  1993/03/31  21:56:51  mhickey
:* 	Initial checkin for defect 7071.
:* 	[1993/03/31  21:52:34  mhickey]
:* 
:* Revision 1.1.6.2  1993/02/05  15:18:34  cjd
:* 	Embedded copyright notice
:* 	[1993/02/05  14:37:51  cjd]
:* 
:* Revision 1.1.4.2  1993/02/04  21:46:39  cjd
:* 	Embedded copyright notice
:* 
:* Revision 1.1.2.2  1992/07/24  20:18:28  mhickey
:* 	Ported to latest GSD, fixed systax errors
:* 
:* 	Defect 4817.
:* 	[1992/07/24  20:15:56  mhickey]
:* 
:* Revision 1.1  1992/01/19  19:09:44  devrcs
:* 	Initial revision
:* 
:* $EndLog$
:*
:*	FILE_NAME:  Remove_object.sv
:*
:*	COMPONENT_NAME:  dce.systest
:*
:*	FUNCTIONS:  none
:*
:*	ORIGINS:  27
:*
:*	(C) COPYRIGHT International Business Machines Corp. 1991
:*      All Rights Reserved
:*
:*      FOR_TEST:  dcedgshd
:*
:*      USAGE:  dcegdsh1
:*	
:*	Overview:
:*		This file will be piped into the gdsditadm tool.  This
:*		file emulates the keystrokes a user would enter at the
:*		GDS admin menu.  This particular modifies file removes an 
:*		object from the specified DSA.  The file is first edited 
:*		with sed (editing done by the shell driver dcegdsh1) to 
:*		remove dummy entries.
:*
:*	Change History:
:*		6/17/91		Gerald Cantor 		Initial Design
:*              7/22/92         Mark Hickey     ported to latest GDS,
:*                                              fixed syntax errors
:*
:directory id:2
:Authentication mechanism:Anonymous
:options:Logon to the Default DSA
:**** Object Administration ****:
:Function:1
:**** Remove Object ****:
:Operation:2
:Objekttypnummer:5
:country:dummy_dsa_country
:organization:dummy_dsa_org
:OrganizationalUnit:dummy_dsa_org_unit
:Common-Name:dummy_object_name
:**** Return ****:
:operation:00
:**** Administration ****:
:Function:0
