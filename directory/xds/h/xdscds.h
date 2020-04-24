/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: xdscds.h,v $
 * Revision 1.1.10.2  1996/02/18  23:37:50  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:30:24  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:36:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:19  root]
 * 
 * Revision 1.1.8.1  1994/02/22  19:23:50  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:10:57  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:02:12  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  07:10:07  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:19:42  marrek]
 * 
 * Revision 1.1.4.2  1992/12/30  23:30:15  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  23:20:24  bbelch]
 * 
 * Revision 1.1.2.3  1992/06/30  21:00:31  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:41:49  melman]
 * 
 * Revision 1.1.2.2  1992/06/01  23:16:10  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:43:13  melman]
 * 
 * $EndLog$
 */
#ifndef _XDSCDS_H
#define _XDSCDS_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xdscds_rcsid[] = "@(#)$RCSfile: xdscds.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:37:50 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYP          : INCLUDE                                             */
/*                                                                    */
/* NAME         : xdscds.h                                            */
/*                                                                    */
/* AUTHOR       : V. Ryan                                             */
/* DATE         : 1. 10. 1990                                         */
/*                                                                    */
/* COMPONENT    : XDS                                                 */
/*                                                                    */
/* DOK.-REF.    : Use of ISO Object Identifiers in DCE Naming,        */
/*                David Oran, Digital Equipment Corporation.          */
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*                                                                    */
/* DESCRIPTION  : OM attributes for the Cell Directory Service        */
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCIES:                                               */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   1.0    | 90-10-01| Original                       | VR |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/


#ifndef XDSCDS_HEADER
#define XDSCDS_HEADER


/*      {iso(1) identified-org(3) osf(22) dce(1) cds(3)
          = "\x2B\x16\x01\x03" */

/* Cell Directory Service attribute types */

#define OMP_O_DSX_A_CDS_Members          "\x2B\x16\x01\x03\x0A"
#define OMP_O_DSX_A_CDS_GroupRevoke      "\x2B\x16\x01\x03\x0B"
#define OMP_O_DSX_A_CDS_CTS              "\x2B\x16\x01\x03\x0C"
#define OMP_O_DSX_A_CDS_UTS              "\x2B\x16\x01\x03\x0D"
#define OMP_O_DSX_A_CDS_Class            "\x2B\x16\x01\x03\x0F"
#define OMP_O_DSX_A_CDS_ClassVersion     "\x2B\x16\x01\x03\x10"
#define OMP_O_DSX_A_CDS_ObjectUUID       "\x2B\x16\x01\x03\x11"
#define OMP_O_DSX_A_CDS_Address          "\x2B\x16\x01\x03\x12"
#define OMP_O_DSX_A_CDS_Replicas         "\x2B\x16\x01\x03\x13"
#define OMP_O_DSX_A_CDS_AllUpTo          "\x2B\x16\x01\x03\x14"
#define OMP_O_DSX_A_CDS_Convergence      "\x2B\x16\x01\x03\x15"
#define OMP_O_DSX_A_CDS_InCHName         "\x2B\x16\x01\x03\x16"
#define OMP_O_DSX_A_CDS_ParentPointer    "\x2B\x16\x01\x03\x17"
#define OMP_O_DSX_A_CDS_DirectoryVersion "\x2B\x16\x01\x03\x18"
#define OMP_O_DSX_A_CDS_UpgradeTo        "\x2B\x16\x01\x03\x19"
#define OMP_O_DSX_A_CDS_LinkTarget       "\x2B\x16\x01\x03\x1B"
#define OMP_O_DSX_A_CDS_LinkTimeout      "\x2B\x16\x01\x03\x1C"
#define OMP_O_DSX_A_CDS_Towers           "\x2B\x16\x01\x03\x1E"
#define OMP_O_DSX_A_CDS_CHName           "\x2B\x16\x01\x03\x20"
#define OMP_O_DSX_A_CDS_CHLastAddress    "\x2B\x16\x01\x03\x22"
#define OMP_O_DSX_A_CDS_CHUpPointers     "\x2B\x16\x01\x03\x23"
#define OMP_O_DSX_A_CDS_CHState          "\x2B\x16\x01\x03\x24"


/*      {iso(1) identified-org(3) osf(22) dce(1) gds(2)
          = "\x2B\x16\x01\x02" */

#define OMP_O_DSX_UUID                   "\x2B\x16\x01\x02\x01"
#define OMP_O_DSX_TYPELESS_RDN           "\x2B\x16\x01\x02\x02"


#define OMP_O_DSX_NORMAL_SIMPLE_NAME     "\x2B\x16\x01\x03\x00"
#define OMP_O_DSX_BINARY_SIMPLE_NAME     "\x2B\x16\x01\x03\x02"


#endif  /* XDSCDS_HEADER */

#endif /* _XDSCDS_H */
