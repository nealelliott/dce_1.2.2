/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: mhsdef.h,v $
 * Revision 1.1.22.2  1996/02/18  23:37:44  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:30:06  marty]
 *
 * Revision 1.1.22.1  1995/12/08  15:35:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:13  root]
 * 
 * Revision 1.1.20.3  1994/09/29  15:56:06  keutel
 * 	OT 12404
 * 	[1994/09/29  15:55:40  keutel]
 * 
 * Revision 1.1.20.2  1994/06/10  21:20:24  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:00  devsrc]
 * 
 * Revision 1.1.20.1  1994/02/22  19:23:39  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:10:36  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:01:28  marrek]
 * 
 * Revision 1.1.18.1  1993/10/14  19:19:51  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  19:18:14  keutel]
 * 
 * 	October 1993 code drop
 * 	[1993/10/11  09:54:17  keutel]
 * 
 * $EndLog$
 */
#ifndef _MHSDEF_H
#define _MHSDEF_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char mhsdef_rcsid[] = "@(#)$RCSfile: mhsdef.h,v $ $Revision: 1.1.22.2 $ $Date: 1996/02/18 23:37:44 $";
#endif
/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/
/* @(#)mhsdef.h	10.2.1.1 91/12/19 (SNI) */

/**********************************************************************/
/*               COPYRIGHT (C) BULL S.A. 1990 1991 1992               */
/*                        All Rights Reserved                         */
/**********************************************************************/
/*                                                                    */
/*   * This software is furnished under licence and may be used only  */
/*   * in accordance with the terms of that licence and with the      */
/*   * inclusion of the above copyright notice.                       */
/*   * This software may not be provided or otherwise made available  */
/*   * to, or used by, any other person. No title to or ownership of  */
/*   * the software is hereby transferred.                            */
/*                                                                    */
/**********************************************************************/
/*	@(#)mhsdef.h	1.3 (Bull S.A) 4/23/92	*/

/**********************************************************************/
/*                                                                    */
/* TYP          : INCLUDE                                             */
/*                                                                    */
/* NAME         : mhsdef.h                                            */
/*                                                                    */
/* AUTHOR       : Bull S.A.                                           */
/* DATE         : 1. 10. 1990                                         */
/*                                                                    */
/* COMPONENT    : XOM/XDS                                             */
/*                                                                    */
/* DOC.-REF.    : X/Open Directory Services API Specification ver. 1.0*/
/*              : OSI Object Management API Specification ver. 2.0    */
/*              : X.400 API Version 2.0                               */
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*                                                                    */
/* DESCRIPTION  : Definition, for OM classes of MHS package used by   */
/*                XDS, of all OM attributes that may be found         */
/*                in an instance of an OM class.                      */
/*                X400 definitions (temporarily)                      */
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCIES:                                               */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)@(#)mhsdef.h	1.2 (Bull S.A) 12/3/91 */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       |    |         */
/*   1.0    | 90-10-01| Original                       |    |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/**********************************************************************/

/************* X400 include file ***************/
#include <xmhp.h>
/*******************End X400 definitions **********************/


/*	X400 MH object types			*/
#define	XMH_OR_ADDRESS	 	28
#define	XMH_OR_NAME	 	29
#define MHS_DLSP                801

#define	DL_SP_TYPE_LIST							\
		{{OM_CLASS,1,0,0},					\
		{DS_PERM_TYPE,1,0,0},				\
		{DS_INDIVIDUAL,1,0,0},					\
		{DS_MEMBER_OF_DL,1,0,0},				\
		{DS_PATTERN_MATCH,1,0,0},				\
		{DS_MEMBER_OF_GROUP,1,0,0},				\
		{OM_NO_MORE_TYPES,0,0,0}}		

#define	OR_NAME_TYPE_LIST						\
    {{OM_CLASS,1,0,0},							\
    {MH_T_DIRECTORY_NAME,1,0,0,0},					\
    {MH_T_ADMD_NAME,1,0,MH_VL_ADMD_NAME,0},				\
    {MH_T_COMMON_NAME,2,0,MH_VL_COMMON_NAME,0},				\
    {MH_T_COUNTRY_NAME,1,0,MH_VL_COUNTRY_NAME,0},			\
    {MH_T_DOMAIN_TYPE_1,2,0,MH_VL_DOMAIN_TYPE,0},			\
    {MH_T_DOMAIN_TYPE_2,2,0,MH_VL_DOMAIN_TYPE,0},			\
    {MH_T_DOMAIN_TYPE_3,2,0,MH_VL_DOMAIN_TYPE,0},			\
    {MH_T_DOMAIN_TYPE_4,2,0,MH_VL_DOMAIN_TYPE,0},			\
    {MH_T_DOMAIN_VALUE_1,2,0,MH_VL_DOMAIN_VALUE,0},			\
    {MH_T_DOMAIN_VALUE_2,2,0,MH_VL_DOMAIN_VALUE,0},			\
    {MH_T_DOMAIN_VALUE_3,2,0,MH_VL_DOMAIN_VALUE,0},			\
    {MH_T_DOMAIN_VALUE_4,2,0,MH_VL_DOMAIN_VALUE,0},			\
    {MH_T_GENERATION,2,0,MH_VL_GENERATION,0},				\
    {MH_T_GIVEN_NAME,2,0,MH_VL_GIVEN_NAME,0},				\
    {MH_T_INITIALS,2,0,MH_VL_INITIALS,0},				\
    {MH_T_ISDN_NUMBER,1,0,MH_VL_ISDN_NUMBER,0},				\
    {MH_T_ISDN_SUBADDRESS,1,0,MH_VL_ISDN_SUBADDRESS,0},			\
    {MH_T_NUMERIC_USER_IDENTIFIER,1,0,MH_VL_NUMERIC_USER_IDENTIFIER,0},	\
    {MH_T_ORGANIZATION_NAME,2,0,MH_VL_ORGANIZATION_NAME,0},		\
    {MH_T_ORGANIZATIONAL_UNIT_NAME_1,2,0,MH_VL_ORGANIZATIONAL_UNIT_NAMES,0},\
    {MH_T_ORGANIZATIONAL_UNIT_NAME_2,2,0,MH_VL_ORGANIZATIONAL_UNIT_NAMES,0},\
    {MH_T_ORGANIZATIONAL_UNIT_NAME_3,2,0,MH_VL_ORGANIZATIONAL_UNIT_NAMES,0},\
    {MH_T_ORGANIZATIONAL_UNIT_NAME_4,2,0,MH_VL_ORGANIZATIONAL_UNIT_NAMES,0},\
    {MH_T_POSTAL_ADDRESS_DETAILS,2,0,MH_VL_POSTAL_ADDRESS_DETAILS,0},	\
    {MH_T_POSTAL_ADDRESS_IN_FULL,1,0,MH_VL_POSTAL_ADDRESS_IN_FULL,0},	\
    {MH_T_POSTAL_CODE,1,0,MH_VL_POSTAL_CODE,0},				\
    {MH_T_POSTAL_COUNTRY_NAME,1,0,MH_VL_POSTAL_COUNTRY_NAME,0},		\
    {MH_T_POSTAL_DELIVERY_POINT_NAME,2,0,MH_VL_POSTAL_DELIV_POINT_NAME,0},\
    {MH_T_POSTAL_DELIV_SYSTEM_NAME,1,0,MH_VL_POSTAL_DELIV_SYSTEM_NAME,0},\
    {MH_T_POSTAL_GENERAL_DELIV_ADDR,2,0,MH_VL_POSTAL_GENERAL_DELIV_ADDR,0},\
    {MH_T_POSTAL_LOCALE,2,0,MH_VL_POSTAL_LOCALE,0},			\
    {MH_T_POSTAL_OFFICE_BOX_NUMBER,2,0,MH_VL_POSTAL_OFFICE_BOX_NUMBER,0},\
    {MH_T_POSTAL_OFFICE_NAME,2,0,MH_VL_POSTAL_OFFICE_NAME,0},		\
    {MH_T_POSTAL_OFFICE_NUMBER,2,0,MH_VL_POSTAL_OFFICE_NUMBER,0},	\
    {MH_T_POSTAL_ORGANIZATION_NAME,2,0,MH_VL_POSTAL_ORGANIZATION_NAME,0},\
    {MH_T_POSTAL_PATRON_DETAILS,2,0,MH_VL_POSTAL_PATRON_DETAILS,0},	\
    {MH_T_POSTAL_PATRON_NAME,2,0,MH_VL_POSTAL_PATRON_NAME,0},		\
    {MH_T_POSTAL_STREET_ADDRESS,2,0,MH_VL_POSTAL_STREET_ADDRESS,0},	\
    {MH_T_PRESENTATION_ADDRESS,1,0,0,0},				\
    {MH_T_PRMD_NAME,1,0,MH_VL_PRMD_NAME,0},				\
    {MH_T_SURNAME,2,0,MH_VL_SURNAME,0},					\
    {MH_T_TERMINAL_IDENTIFIER,1,0,MH_VL_TERMINAL_IDENTIFIER,0},		\
    {MH_T_TERMINAL_TYPE,1,0,0,0},					\
    {MH_T_X121_ADDRESS,1,0,MH_VL_X121_ADDRESS,0},			\
    {MH_T_POSTAL_ADDRESS_IN_LINES,6,0,MH_VL_POSTAL_ADDRESS_IN_LINES,0},	\
    {OM_NO_MORE_TYPES,0,0,0,0}}		

#define	OR_ADDR_TYPE_LIST						\
    {{OM_CLASS,1,0,0},					\
    {MH_T_ADMD_NAME,1,0,MH_VL_ADMD_NAME,0},				\
    {MH_T_COMMON_NAME,2,0,MH_VL_COMMON_NAME,0},				\
    {MH_T_COUNTRY_NAME,1,0,MH_VL_COUNTRY_NAME,0},			\
    {MH_T_DOMAIN_TYPE_1,2,0,MH_VL_DOMAIN_TYPE,0},			\
    {MH_T_DOMAIN_TYPE_2,2,0,MH_VL_DOMAIN_TYPE,0},			\
    {MH_T_DOMAIN_TYPE_3,2,0,MH_VL_DOMAIN_TYPE,0},			\
    {MH_T_DOMAIN_TYPE_4,2,0,MH_VL_DOMAIN_TYPE,0},			\
    {MH_T_DOMAIN_VALUE_1,2,0,MH_VL_DOMAIN_VALUE,0},			\
    {MH_T_DOMAIN_VALUE_2,2,0,MH_VL_DOMAIN_VALUE,0},			\
    {MH_T_DOMAIN_VALUE_3,2,0,MH_VL_DOMAIN_VALUE,0},			\
    {MH_T_DOMAIN_VALUE_4,2,0,MH_VL_DOMAIN_VALUE,0},			\
    {MH_T_GENERATION,2,0,MH_VL_GENERATION,0},				\
    {MH_T_GIVEN_NAME,2,0,MH_VL_GIVEN_NAME,0},				\
    {MH_T_INITIALS,2,0,MH_VL_INITIALS,0},				\
    {MH_T_ISDN_NUMBER,1,0,MH_VL_ISDN_NUMBER,0},				\
    {MH_T_ISDN_SUBADDRESS,1,0,MH_VL_ISDN_SUBADDRESS,0},			\
    {MH_T_NUMERIC_USER_IDENTIFIER,1,0,MH_VL_NUMERIC_USER_IDENTIFIER,0},	\
    {MH_T_ORGANIZATION_NAME,2,0,MH_VL_ORGANIZATION_NAME,0},		\
    {MH_T_ORGANIZATIONAL_UNIT_NAME_1,2,0,MH_VL_ORGANIZATIONAL_UNIT_NAMES,0},\
    {MH_T_ORGANIZATIONAL_UNIT_NAME_2,2,0,MH_VL_ORGANIZATIONAL_UNIT_NAMES,0},\
    {MH_T_ORGANIZATIONAL_UNIT_NAME_3,2,0,MH_VL_ORGANIZATIONAL_UNIT_NAMES,0},\
    {MH_T_ORGANIZATIONAL_UNIT_NAME_4,2,0,MH_VL_ORGANIZATIONAL_UNIT_NAMES,0},\
    {MH_T_POSTAL_ADDRESS_DETAILS,2,0,MH_VL_POSTAL_ADDRESS_DETAILS,0},	\
    {MH_T_POSTAL_ADDRESS_IN_FULL,1,0,MH_VL_POSTAL_ADDRESS_IN_FULL,0},	\
    {MH_T_POSTAL_CODE,1,0,MH_VL_POSTAL_CODE,0},				\
    {MH_T_POSTAL_COUNTRY_NAME,1,0,MH_VL_POSTAL_COUNTRY_NAME,0},		\
    {MH_T_POSTAL_DELIVERY_POINT_NAME,2,0,MH_VL_POSTAL_DELIV_POINT_NAME,0},\
    {MH_T_POSTAL_DELIV_SYSTEM_NAME,1,0,MH_VL_POSTAL_DELIV_SYSTEM_NAME,0},\
    {MH_T_POSTAL_GENERAL_DELIV_ADDR,2,0,MH_VL_POSTAL_GENERAL_DELIV_ADDR,0},\
    {MH_T_POSTAL_LOCALE,2,0,MH_VL_POSTAL_LOCALE,0},			\
    {MH_T_POSTAL_OFFICE_BOX_NUMBER,2,0,MH_VL_POSTAL_OFFICE_BOX_NUMBER,0},\
    {MH_T_POSTAL_OFFICE_NAME,2,0,MH_VL_POSTAL_OFFICE_NAME,0},		\
    {MH_T_POSTAL_OFFICE_NUMBER,2,0,MH_VL_POSTAL_OFFICE_NUMBER,0},	\
    {MH_T_POSTAL_ORGANIZATION_NAME,2,0,MH_VL_POSTAL_ORGANIZATION_NAME,0},\
    {MH_T_POSTAL_PATRON_DETAILS,2,0,MH_VL_POSTAL_PATRON_DETAILS,0},	\
    {MH_T_POSTAL_PATRON_NAME,2,0,MH_VL_POSTAL_PATRON_NAME,0},		\
    {MH_T_POSTAL_STREET_ADDRESS,2,0,MH_VL_POSTAL_STREET_ADDRESS,0},	\
    {MH_T_PRESENTATION_ADDRESS,1,0,0,0},				\
    {MH_T_PRMD_NAME,1,0,MH_VL_PRMD_NAME,0},				\
    {MH_T_SURNAME,2,0,MH_VL_SURNAME,0},					\
    {MH_T_TERMINAL_IDENTIFIER,1,0,MH_VL_TERMINAL_IDENTIFIER,0},		\
    {MH_T_TERMINAL_TYPE,1,0,0,0},					\
    {MH_T_X121_ADDRESS,1,0,MH_VL_X121_ADDRESS,0},			\
    {MH_T_POSTAL_ADDRESS_IN_LINES,6,0,MH_VL_POSTAL_ADDRESS_IN_LINES,0},	\
    {OM_NO_MORE_TYPES,0,0,0,0}}		

#define	PERM_TYPE_LIMITS {					\
    {DS_PERM_INDIVIDUAL},					\
    {DS_PERM_MEMBER_OF_GROUP}					\
    }

#define	PERM_TYPE_LIST {					\
    DS_PERM_INDIVIDUAL,						\
    DS_PERM_MEMBER_OF_DL,					\
    DS_PERM_INDIVIDUAL,						\
    DS_PERM_MEMBER_OF_GROUP					\
    }

#define	TERMINAL_TYPE_LIMITS {					\
    {MH_TT_TELEX},						\
    {MH_TT_VIDEOTEX}						\
    }

#define	TERMINAL_TYPE_LIST {					\
    MH_TT_TELEX,						\
    MH_TT_TELETEX,						\
    MH_TT_G3_FAX,						\
    MH_TT_G4_FAX,						\
    MH_TT_IA5_TERMINAL,						\
    MH_TT_VIDEOTEX						\
    }

/* specific minimum string value length		*/
/* other are MIN_VL_1 (defined equal to 1)	*/
#define	MIN_VL_ADMD_NAME		((OM_value_length) 0)
#define	MIN_VL_COUNTRY_NAME		((OM_value_length) 2)
#define	MIN_VL_POSTAL_COUNTRY_NAME	((OM_value_length) 2)

#endif /* _MHSDEF_H */
