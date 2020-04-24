/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xdsdme.h,v $
 * Revision 1.1.16.2  1996/02/18  23:37:53  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:30:35  marty]
 *
 * Revision 1.1.16.1  1995/12/08  15:36:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:21  root]
 * 
 * Revision 1.1.14.2  1994/06/10  21:20:29  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:02  devsrc]
 * 
 * Revision 1.1.14.1  1994/02/22  19:23:56  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:11:10  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:02:38  marrek]
 * 
 * Revision 1.1.12.2  1993/10/19  15:02:12  keutel
 * 	CR 9188
 * 	[1993/10/19  15:01:51  keutel]
 * 
 * Revision 1.1.12.1  1993/10/14  19:23:46  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  19:22:39  keutel]
 * 
 * 	October 1993 code drop
 * 	[1993/10/11  09:54:32  keutel]
 * 
 * $EndLog$
 */
#ifndef _XDSDME_H
#define _XDSDME_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xdsdme_rcsid[] = "@(#)$RCSfile: xdsdme.h,v $ $Revision: 1.1.16.2 $ $Date: 1996/02/18 23:37:53 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1993   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYP          : INCLUDE                                             */
/*                                                                    */
/* NAME         : xdsdme.h                                            */
/*                                                                    */
/* AUTHOR       : CC                                                     */
/* DATE         : 13. 09. 1993                                        */
/*                                                                    */
/* COMPONENT    : XDS                                                 */
/*                                                                    */
/* DOK.-REF.    : X/Open CAE Spec. API to Directory Services (XDS)    */
/*                X/Open CAE Spec. OSI-Abstract-Data Manip. API (XOM) */
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*                                                                    */
/* DESCRIPTION  : definitions for Distributed Management Environment  */
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCIES:                                               */
/*                                                                    */
/*datoff **************************************************************/


/* Intermediate object identifier macros */

/* iso(1) identified-organization(3) osf(22) dme(2) components(1) nmo(2) ... */

#define dsP_NMOattributeType(X)         "\x2B\x16\x02\x01\x02\x01" #X

#define dsP_NMOobjectClass(X)           "\x2B\x16\x02\x01\x02\x02" #X
 
/* Directory attribute types (prefixed by DSX_A_)
   Directory object classes (prefixed by DSX_O_)
*/


/* Directory attribute types */

#define OMP_O_DSX_A_ALTERNATE_ADDRESS	dsP_NMOattributeType(\x01)
 


/* Directory object classes */

#define OMP_O_DSX_O_DME_NMO_AGENT	dsP_NMOobjectClass(\x01)
 

#endif /* XDSDME_H */
