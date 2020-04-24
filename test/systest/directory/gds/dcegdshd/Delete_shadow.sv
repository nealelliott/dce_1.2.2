:*		
:* @OSF_COPYRIGHT@
:* COPYRIGHT NOTICE
:* Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
:* ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
:* src directory for the full copyright text.
:* HISTORY
:* $Log: Delete_shadow.sv,v $
:* Revision 1.1.6.2  1996/03/11  02:41:10  marty
:* 	Update OSF copyright years
:* 	[1996/03/10  20:06:20  marty]
:*
:* Revision 1.1.6.1  1995/12/11  21:57:37  root
:* 	Submit OSF/DCE 1.2.1
:* 	[1995/12/11  20:57:20  root]
:* 
:* Revision 1.1.4.1  1994/07/21  20:19:04  mcg
:* 	Changes to gdsditadm required changes
:* 	to the .sv screen drivers
:* 	This Fixes OT 10240
:* 	[1994/07/21  20:18:49  mcg]
:* 
:* Revision 1.1.2.1  1993/09/20  22:40:36  pellis
:* 	Moved from ./systest/directory/gds.
:* 	[1993/09/20  22:36:45  pellis]
:* 
:* Revision 1.1.2.3  1993/04/03  04:27:06  mhickey
:* 	Putting in Helmut's fix for operation exit.
:* 	defect 7071
:* 	[1993/04/03  04:20:21  mhickey]
:* 
:* Revision 1.1.2.2  1993/03/31  21:56:38  mhickey
:* 	Initial checkin for defect 7071.
:* 	[1993/03/31  21:52:25  mhickey]
:* 
:* Revision 1.1.6.2  1993/02/05  15:18:25  cjd
:* 	Embedded copyright notice
:* 	[1993/02/05  14:37:42  cjd]
:* 
:* Revision 1.1.4.2  1993/02/04  21:46:29  cjd
:* 	Embedded copyright notice
:* 
:* Revision 1.1.2.2  1992/07/24  20:18:23  mhickey
:* 	Ported to latest GSD, fixed systax errors
:* 
:* 	Defect 4817.
:* 	[1992/07/24  20:15:37  mhickey]
:* 
:* Revision 1.1  1992/01/19  19:09:44  devrcs
:* 	Initial revision
:* 
:* $EndLog$
:*
:*	FILE_NAME:  Delete_shadow.sv
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
:*		GDS admin menu.  This particular file deletes a shadow and
:*		its associated job in the DSA specified.  The file is first 
:*		edited with sed (editing done by the shell driver dcegdsh1) 
:*		to remove dummy entries.
:*
:*	Change History:
:*		6/17/91		Gerald Cantor 		Initial Design
:*              7/22/92         Mark Hickey     ported to latest GDS,
:*                                              fixed syntax errors
:*
:directory id:2
:Authentication mechanism:Anonymous
:options:Logon to the Default DSA
:**** Shadow Administration ****:
:Function:3
:**** Delete shadow/job ****:
:Function:3
:Objekttypnummer:05
:object type:SINGLE OBJECT
:country:dummy_mstr_dsa_country
:organization:dummy_mstr_dsa_org
:OrganizationalUnit:dummy_mstr_dsa_org_unit
:common name:dummy_object_name
:*** The following is DSA info ***:
:country:dummy_dsa_country
:organization:dummy_dsa_org
:organizationalunit:dummy_dsa_org_unit
:common-Name:dummy_dsa_com_name
:common-Name:dummy_dsa_name
:Options:None
:operation:00
:operation:0
