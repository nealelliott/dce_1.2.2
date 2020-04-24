/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xdssap.h,v $
 * Revision 1.1.20.2  1996/02/18  23:38:03  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:31:04  marty]
 *
 * Revision 1.1.20.1  1995/12/08  15:37:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:29  root]
 * 
 * Revision 1.1.18.4  1994/06/10  21:20:33  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:04  devsrc]
 * 
 * Revision 1.1.18.3  1994/05/10  16:04:13  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:15:55  marrek]
 * 
 * Revision 1.1.18.2  1994/03/23  15:49:15  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:37:15  keutel]
 * 
 * Revision 1.1.18.1  1994/02/22  18:59:57  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:11:40  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:03:42  marrek]
 * 
 * Revision 1.1.16.1  1993/10/14  19:23:53  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  19:22:49  keutel]
 * 
 * 	October 1993 code drop
 * 	[1993/10/11  09:54:30  keutel]
 * 
 * $EndLog$
 */
#ifndef _XDSSAP_H
#define _XDSSAP_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xdssap_rcsid[] = "@(#)$RCSfile: xdssap.h,v $ $Revision: 1.1.20.2 $ $Date: 1996/02/18 23:38:03 $";
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
/* NAME         : xdssap.h                                            */
/*                                                                    */
/* AUTHOR       :                                                     */
/* DATE         : 13. 08. 1990                                        */
/*                                                                    */
/* COMPONENT    : XDS                                                 */
/*                                                                    */
/* DOK.-REF.    : X/Open CAE Spec. API to Directory Services (XDS)    */
/*                X/Open CAE Spec. OSI-Abstract-Data Manip. API (XOM) */
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*                                                                    */
/* DESCRIPTION  : xdssap.h as specified in XDS API                    */
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCIES:                                               */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   1.0    | 93-08-13| Original                       | CC |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/


 
  #define OMP_O_DS_STRONG_AUTHENT_PKG      "\x2B\x0C\x02\x87\x73\x1c\x02"
 
  /* Intermediate object identifier macros                    */
  #ifndef dsP_attributeType
    #define dsP_attributeType(X) ("\x55\x4"#X) /* joint-iso-ccitt 5 4 */
  #endif
 
  #ifndef dsP_objectClass
    #define dsP_objectClass(X)   ("\x55\x6"#X) /* joint-iso-ccitt 5 6 */
  #endif
 
  #define dsP_sap_c(X)   (OMP_O_DS_STRONG_AUTHENT_PKG#X)
 
  /* OM class names (prefixed DS_C_),							*/
  /* Directory attribute types (prefixed DS_A_),				*/
  /* and Directory object classes (prefixed DS_O_)  			*/

  /* Every application program which makes use of a class or    */
  /* other Object Identifier must explicitly import it into     */
  /* every compilation unit (C source program) which uses it.   */
  /* Each such class or Object Identifier name must be          */
  /* explicitly exported from just one compilation unit.        */

  /* In the header file, OM class constants are prefixed with	*/
  /* the OMP_O prefix to denote that they are OM classes.		*/
  /* However, when using the OM_IMPORT and OM_EXPORT macros,	*/
  /* the base names (without the OMP_O prefix) should be used.	*/
  /*       For example:											*/
  /*         OM_IMPORT(DS_O_CERT_AUTHORITY)						*/
 
  /* Directory attribute types */
  
  #define OMP_O_DS_A_AUTHORITY_REVOC_LIST		dsP_attributeType(\x26)
  #define OMP_O_DS_A_CA_CERT					dsP_attributeType(\x25)
  #define OMP_O_DS_A_CERT_REVOC_LIST			dsP_attributeType(\x27)
  #define OMP_O_DS_A_CROSS_CERT_PAIR			dsP_attributeType(\x28)
  #define OMP_O_DS_A_USER_CERT					dsP_attributeType(\x24)
 

  /* Directory object classes */

  #define OMP_O_DS_O_CERT_AUTHORITY				dsP_objectClass(\x10)
  #define OMP_O_DS_O_STRONG_AUTHENT_USER		dsP_objectClass(\x0F)
 
  /* OM class names */

  #define OMP_O_DS_C_ALGORITHM_IDENT			dsP_sap_c(\x6\x35)
  #define OMP_O_DS_C_CERT						dsP_sap_c(\x6\x36)
  #define OMP_O_DS_C_CERT_LIST					dsP_sap_c(\x6\x37)
  #define OMP_O_DS_C_CERT_PAIR					dsP_sap_c(\x6\x38)
  #define OMP_O_DS_C_CERT_SUBLIST				dsP_sap_c(\x6\x39)
  #define OMP_O_DS_C_SIGNATURE					dsP_sap_c(\x6\x3A)
 
  /* OM attribute names */

  #define DS_ALGORITHM							((OM_type)821)
  #define DS_FORWARD							((OM_type)822)
  #define DS_ISSUER								((OM_type)823)
  #define DS_LAST_UPDATE						((OM_type)824)
  #define DS_ALGORITHM_PARAMETERS				((OM_type)825)
  #define DS_REVERSE							((OM_type)826)
  #define DS_REVOC_DATE							((OM_type)827)
  #define DS_REVOKED_CERTS						((OM_type)828)
  #define DS_SERIAL_NBR							((OM_type)829)
  #define DS_SERIAL_NBRS						((OM_type)830)
  #define DS_SIGNATURE							((OM_type)831)
  #define DS_SIGNATURE_VALUE					((OM_type)832)
  #define DS_SUBJECT							((OM_type)833)
  #define DS_SUBJECT_ALGORITHM					((OM_type)834)
  #define DS_SUBJECT_PUBLIC_KEY					((OM_type)835)
  #define DS_VALIDITY_NOT_AFTER					((OM_type)836)
  #define DS_VALIDITY_NOT_BEFORE				((OM_type)837)
  #define DS_VERSION							((OM_type)838)
 
  /* DS_Version */

  #define DS_V1988						((OM_enumeration)1)
 
  /* upper bounds on string lengths and number of repeated */
  /* OM attribute values                                   */

  #define DS_VL_LAST_UPDATE					((OM_value_length)17)
  #define DS_VL_REVOC_DATE					((OM_value_length)17)
  #define DS_VL_VALIDITY_NOT_AFTER			((OM_value_length)17)
  #define DS_VL_VALIDITY_NOT_BEFORE			((OM_value_length)17)
  #define DS_VN_REVOC_DATE					((OM_value_length)2)
 
#endif /* XDSSAP_H */
