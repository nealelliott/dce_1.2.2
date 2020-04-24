/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: xdsmdup.h,v $
 * Revision 1.1.89.2  1996/02/18  23:37:59  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:30:55  marty]
 *
 * Revision 1.1.89.1  1995/12/08  15:36:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:26  root]
 * 
 * Revision 1.1.87.2  1994/09/19  08:50:15  marrek
 * 	Fixes for OT 12198.
 * 	[1994/09/19  08:49:45  marrek]
 * 
 * Revision 1.1.87.1  1994/02/22  18:59:52  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:11:28  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:03:14  marrek]
 * 
 * Revision 1.1.85.1  1993/10/14  19:23:52  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  19:22:47  keutel]
 * 
 * 	October 1993 code drop
 * 	[1993/10/11  10:02:25  keutel]
 * 
 * Revision 1.1.7.2  1993/08/10  07:11:20  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:20:50  marrek]
 * 
 * Revision 1.1.5.2  1992/12/30  23:30:47  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  23:20:43  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:16:41  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:43:40  melman]
 * 
 * $EndLog$
 */
#ifndef _XDSMDUP_H
#define _XDSMDUP_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xdsmdup_rcsid[] = "@(#)$RCSfile: xdsmdup.h,v $ $Revision: 1.1.89.2 $ $Date: 1996/02/18 23:37:59 $";
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
/* NAME         : xdsmdup.h                                           */
/*                                                                    */
/* AUTHOR       : V. Ryan                                             */
/* DATE         : 1. 08. 1991                                         */
/*                                                                    */
/* COMPONENT    : XDS                                                 */
/*                                                                    */
/* DOK.-REF.    : X/Open CAE Spec. API to Directory Services (XDS)    */
/*                X/Open CAE Spec. OSI-Abstract-Data Manip. API (XOM) */
/*                SNI XDS/XOM Design Specification November 1991      */
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*                                                                    */
/* DESCRIPTION  : definitions for Global Directory Service Package    */
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCIES:                                               */
/*                                                                    */
/* HISTORY      :                                                     */ 
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   1.0    | 91-08-01| Original                       | VR |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/


#ifndef XDSMDUP_HEADER
#define XDSMDUP_HEADER
 

#ifndef XMHP_HEADER
#include <xmhp.h>
#endif  /* XMHP_HEADER */

/* MDUP package object identifier */
/* { iso(1) identifier-organization(3) icd-ecma(12) member-company(2) dec(1011) 
     xopen(28) mdup(3) } */

#define OMP_O_DS_MHS_DIR_USER_PKG               "\x2B\x0C\x02\x87\x73\x1C\x03"
 
/* Intermediate object identifier macros */

#define dsP_MHSattributeType(X) ("\x56\x5\x2" #X) /* joint-iso-ccitt 6 5 2 */
 
#define dsP_MHSobjectClass(X)   ("\x56\x5\x1" #X) /* joint-iso-ccitt 6 5 1 */
 
#define dsP_mdup_c(X)      (OMP_O_DS_MHS_DIR_USER_PKG #X)


/* OM class names (prefixed DS_C_),                             */
/* Directory attribute types (prefixed DS_A_),                  */
/* and Directory object classes (prefixed DS_O_)                */

/* Every application program which makes use of a class or      */
/* other Object Identifier must explicitly import it into       */
/* every compilation unit (C source program) which uses it.     */
/* Each such class or Object Identifier name must be            */
/* explicitly exported from just one compilation unit.          */

/* In the header file, OM class constants are prefixed with     */
/* the OMP_O prefix to denote that they are OM classes.         */
/* However, when using the OM_IMPORT and OM_EXPORT macros,      */
/* the base names (without the OMP_O prefix) should be used.    */
/* For example:                                                 */
/*        OM_IMPORT(DS_O_CERT_AUTHORITY)                        */


/* Directory attribute types */

#define OMP_O_DS_A_DELIV_CONTENT_LENGTH         dsP_MHSattributeType(\x00)
#define OMP_O_DS_A_DELIV_CONTENT_TYPES          dsP_MHSattributeType(\x01)
#define OMP_O_DS_A_DELIV_EITS                   dsP_MHSattributeType(\x02)
#define OMP_O_DS_A_DL_MEMBERS                   dsP_MHSattributeType(\x03)
#define OMP_O_DS_A_DL_SUBMIT_PERMS              dsP_MHSattributeType(\x04)
#define OMP_O_DS_A_MESSAGE_STORE                dsP_MHSattributeType(\x05)
#define OMP_O_DS_A_OR_ADDRESSES                 dsP_MHSattributeType(\x06)
#define OMP_O_DS_A_PREF_DELIV_METHODS           dsP_MHSattributeType(\x07)
#define OMP_O_DS_A_SUPP_AUTO_ACTIONS            dsP_MHSattributeType(\x08)
#define OMP_O_DS_A_SUPP_CONTENT_TYPES           dsP_MHSattributeType(\x09)
#define OMP_O_DS_A_SUPP_OPT_ATTRIBUTES          dsP_MHSattributeType(\x0A)

 
/* Directory object classes */

#define OMP_O_DS_O_MHS_DISTRIBUTION_LIST        dsP_MHSobjectClass(\x00)
#define OMP_O_DS_O_MHS_MESSAGE_STORE            dsP_MHSobjectClass(\x01)
#define OMP_O_DS_O_MHS_MESSAGE_TRANS_AG         dsP_MHSobjectClass(\x02)
#define OMP_O_DS_O_MHS_USER                     dsP_MHSobjectClass(\x03)
#define OMP_O_DS_O_MHS_USER_AG                  dsP_MHSobjectClass(\x04)

 
/* OM class names */

#define OMP_O_DS_C_DL_SUBMIT_PERMS      dsP_mdup_c(\x86\x21)

 
/* OM attribute names */

#define DS_PERM_TYPE            ( (OM_type) 901 )
#define DS_INDIVIDUAL           ( (OM_type) 902 )
#define DS_MEMBER_OF_DL         ( (OM_type) 903 )
#define DS_PATTERN_MATCH        ( (OM_type) 904 )
#define DS_MEMBER_OF_GROUP      ( (OM_type) 905 )
 

/* DS_Permission_Type */

enum DS_Permission_Type {
        DS_PERM_INDIVIDUAL      = 0,
        DS_PERM_MEMBER_OF_DL    = 1,
        DS_PERM_PATTERN_MATCH   = 2,
        DS_PERM_MEMBER_OF_GROUP = 3
};
 

#endif /* XDSMDUP_HEADER */

#endif /* _XDSMDUP_H */
