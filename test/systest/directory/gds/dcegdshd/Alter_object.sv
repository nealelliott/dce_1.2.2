:*
:* @OSF_COPYRIGHT@
:* COPYRIGHT NOTICE
:* Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
:* ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
:* src directory for the full copyright text.
:* HISTORY
:* $Log: Alter_object.sv,v $
:* Revision 1.1.6.2  1996/03/11  02:40:56  marty
:* 	Update OSF copyright years
:* 	[1996/03/10  20:06:18  marty]
:*
:* Revision 1.1.6.1  1995/12/11  21:57:32  root
:* 	Submit OSF/DCE 1.2.1
:* 	[1995/12/11  20:57:16  root]
:* 
:* 	Fixed un matched comment terminator.
:* 	[1993/03/30  03:14:43  mhickey]
:* 
:* 	Ported to DCE 1.0.2 GDS.
:* 
:* 	Defect 7071.
:* 	[1993/03/24  19:30:29  mhickey]
:* 
:* Revision 1.1.4.1  1994/07/21  20:16:23  mcg
:* 	Changes to gdsditadm required changes
:* 	to the .sv screen drivers
:* 	This Fixes OT 10240
:* 	[1994/07/21  20:16:12  mcg]
:* 
:* Revision 1.1.2.1  1993/09/20  22:40:33  pellis
:* 	Moved from ./systest/directory/gds.
:* 	[1993/09/20  22:36:21  pellis]
:* 
:* Revision 1.1.2.2  1993/03/31  21:56:18  mhickey
:* 	Initial checkin for defect 7071.
:* 	[1993/03/31  21:52:11  mhickey]
:* 
:* Revision 1.1.4.2  1993/02/05  15:18:08  cjd
:* 	Embedded copyright notice
:* 	[1993/02/05  14:37:29  cjd]
:* 
:* Revision 1.1.2.2  1993/02/04  21:46:13  cjd
:* 	Embedded copyright notice
:* 
:* Revision 1.1  1992/01/19  19:09:42  devrcs
:* 	Initial revision
:* 
:* $EndLog$
:*		
:*	FILE_NAME:  Alter_object.sv
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
:*		This file will be piped into the gdsadm tool.  This
:*		file emulates the keystrokes a user would enter at the
:*		GDS admin menu.  This particular file modifies an object's
:*		attributes (phone number in this case).  The file is first 
:*		edited with sed (editing done by the shell driver dcegdsh1) 
:*		to remove dummy entries.
:*
:*	Change History:
:*		6/17/91		Gerald Cantor 		Initial Design
:* *:
:directory id:2
:Authentication mechanism:Anonymous
:options:Logon to the Default DSA
:**** Object Administration ****:
:Function:1
:**** Modify Object ***:
:Operation:07
:Objekttypnummber:05
:country:dummy_dsa_country
:organization:dummy_dsa_org
:OrganizationalUnit:dummy_dsa_org_unit
:User:dummy_object_name
:ObjectClass:Organizational-Person
:Auxiliary Object Class:NO
:Name:Telephone-Number
:old value:dummy_old_phone'
:old value:
:new value:dummy_new_phone'
:new value:
:**** Return ****:
:operation:00
:**** Administration ****:
:Function:0
