/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: xdsgds.h,v $
 * Revision 1.1.77.2  1996/02/18  23:37:56  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:30:40  marty]
 *
 * Revision 1.1.77.1  1995/12/08  15:36:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:23  root]
 * 
 * Revision 1.1.75.2  1994/05/10  16:04:09  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:15:41  marrek]
 * 
 * Revision 1.1.75.1  1994/02/22  19:24:00  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:11:16  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:02:47  marrek]
 * 
 * Revision 1.1.73.1  1993/10/14  19:23:48  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  19:22:41  keutel]
 * 
 * 	October 1993 code drop
 * 	[1993/10/11  10:02:22  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  07:10:43  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:20:17  marrek]
 * 
 * Revision 1.1.4.3  1992/12/30  23:30:24  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  23:20:31  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/30  08:11:09  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  14:47:22  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:16:21  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:43:23  melman]
 * 
 * $EndLog$
 */
#ifndef _XDSGDS_H
#define _XDSGDS_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xdsgds_rcsid[] = "@(#)$RCSfile: xdsgds.h,v $ $Revision: 1.1.77.2 $ $Date: 1996/02/18 23:37:56 $";
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
/* NAME         : xdsgds.h                                            */
/*                                                                    */
/* AUTHOR       : V. Ryan                                             */
/* DATE         : 1. 10. 1990                                         */
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
/*   1.0    | 90-10-01| Original                       | VR |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/


#ifndef XDSGDS_HEADER
#define XDSGDS_HEADER


/* GDS package object identifier */
/* iso(1) identified-organization(3) icd-ecma(0012) member-company(2)
   siemens-units(1107) sni(1) directory(3) xds-api(100) gdsp(0) */

#define OMP_O_DSX_GDS_PKG     "\x2B\x0C\x02\x88\x53\x01\x03\x64\x00"



/* Intermediate object identifier macros */

/* iso(1) identified-organization(3) icd-ecma(0012) member-company(2)
   siemens-units(1107) sni(1) directory(3) attribute-type(4) ... */

#define dsP_GDSattributeType(X)         "\x2B\x0C\x02\x88\x53\x01\x03\x04" #X

/* iso(1) identified-organization(3) icd-ecma(0012) member-company(2)
   siemens-units(1107) sni(1) directory(3) object-class(6) ... */

#define dsP_GDSobjectClass(X)           "\x2b\x0c\x02\x88\x53\x01\x03\x06" #X

#define dsP_gdsp_c(X)                   OMP_O_DSX_GDS_PKG #X


/* OM class names (prefixed DSX_C_)
   Directory attribute types (prefixed DSX_A_)
   Directory object classes (prefixed DSX_O_)
*/


/* Directory attribute types */

#define OMP_O_DSX_A_MASTER_KNOWLEDGE    dsP_GDSattributeType(\x00)
#define OMP_O_DSX_A_ACL                 dsP_GDSattributeType(\x01)
#define OMP_O_DSX_A_TIME_STAMP          dsP_GDSattributeType(\x02)
#define OMP_O_DSX_A_SHADOWED_BY         dsP_GDSattributeType(\x03)
#define OMP_O_DSX_A_SRT                 dsP_GDSattributeType(\x04)
#define OMP_O_DSX_A_OCT                 dsP_GDSattributeType(\x05)
#define OMP_O_DSX_A_AT                  dsP_GDSattributeType(\x06)
#define OMP_O_DSX_A_DEFAULT_DSA         dsP_GDSattributeType(\x08)
#define OMP_O_DSX_A_LOCAL_DSA           dsP_GDSattributeType(\x09)
#define OMP_O_DSX_A_CLIENT              dsP_GDSattributeType(\x0A)
#define OMP_O_DSX_A_DNLIST              dsP_GDSattributeType(\x0B)
#define OMP_O_DSX_A_SHADOWING_JOB       dsP_GDSattributeType(\x0C)
#define OMP_O_DSX_A_CDS_CELL            dsP_GDSattributeType(\x0D)
#define OMP_O_DSX_A_CDS_REPLICA         dsP_GDSattributeType(\x0E)



/* directory object classes */

#define OMP_O_DSX_O_SCHEMA              dsP_GDSobjectClass(\x00)



/* OM class names */

#define OMP_O_DSX_C_GDS_SESSION         dsP_gdsp_c(\x00)
#define OMP_O_DSX_C_GDS_CONTEXT         dsP_gdsp_c(\x01)
#define OMP_O_DSX_C_GDS_ACL             dsP_gdsp_c(\x02)
#define OMP_O_DSX_C_GDS_ACL_ITEM        dsP_gdsp_c(\x03)



/* OM attribute names */

#define DSX_PASSWORD            ((OM_type) 850)
#define DSX_DIR_ID              ((OM_type) 851)
#define DSX_DUAFIRST            ((OM_type) 852)
#define DSX_DONT_STORE          ((OM_type) 853)
#define DSX_NORMAL_CLASS        ((OM_type) 854)
#define DSX_PRIV_CLASS          ((OM_type) 855)
#define DSX_RESIDENT_CLASS      ((OM_type) 856)
#define DSX_USEDSA              ((OM_type) 857)
#define DSX_DUA_CACHE           ((OM_type) 858)
#define DSX_MODIFY_PUBLIC       ((OM_type) 859)
#define DSX_READ_STANDARD       ((OM_type) 860)
#define DSX_MODIFY_STANDARD     ((OM_type) 861)
#define DSX_READ_SENSITIVE      ((OM_type) 862)
#define DSX_MODIFY_SENSITIVE    ((OM_type) 863)
#define DSX_INTERPRETATION      ((OM_type) 864)
#define DSX_USER                ((OM_type) 865)
#define DSX_PREFER_ADM_FUNCS    ((OM_type) 866)
#define DSX_AUTH_MECHANISM      ((OM_type) 867)
#define DSX_AUTH_INFO           ((OM_type) 868)
#define DSX_SIGN_MECHANISM      ((OM_type) 869)
#define DSX_PROT_REQUEST        ((OM_type) 870)



/* DSX_Interpretation */

enum DSX_Interpretation {
        DSX_SINGLE_OBJECT    = 0,
        DSX_ROOT_OF_SUBTREE  = 1
};


/* DSX_Auth_Mechanism */

enum DSX_Auth_Mechanism {
        DSX_NONE_AT_ALL  = 0,
        DSX_DEFAULT      = 1,
        DSX_SIMPLE       = 2,
        DSX_SIMPLE_PROT1 = 3,
        DSX_SIMPLE_PROT2 = 4,
        DSX_DCE_AUTH     = 5,
        DSX_STRONG       = 6
};


/* DSX_Prot_Request */

enum DSX_Prot_Request {
        DSX_NONE   = 0,
        DSX_SIGNED = 1
};



/* upper bound on string lengths */

#define DSX_VL_PASSWORD         ((OM_value_length) 16)



#endif  /* XDSGDS_HEADER */

#endif /* _XDSGDS_H */
