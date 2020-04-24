/*
 *  @OSF_COPYRIGHT@
 *  COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 *  ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 *  src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: xt_parms.h,v $
 * Revision 1.1.13.2  1996/02/18  23:04:13  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:23:05  marty]
 *
 * Revision 1.1.13.1  1995/12/08  22:13:54  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:46:05  root]
 * 
 * Revision 1.1.11.5  1994/08/10  08:25:56  marrek
 * 	Fix for SAP errors, OT11467.
 * 	[1994/08/08  14:17:43  marrek]
 * 
 * Revision 1.1.11.4  1994/07/06  15:09:40  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:27:44  marrek]
 * 
 * Revision 1.1.11.3  1994/06/21  14:28:53  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  07:48:47  marrek]
 * 
 * Revision 1.1.11.2  1994/05/10  15:33:18  marrek
 * 	Bug fix for April 1994 submission.
 * 	[1994/05/09  08:41:23  marrek]
 * 
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/06  10:23:48  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  13:44:02  marrek]
 * 
 * Revision 1.1.11.1  1994/02/22  19:16:04  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/07  13:31:17  marrek]
 * 
 * Revision 1.1.8.1  1993/10/14  19:28:50  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  19:28:14  keutel]
 * 
 * 	October 1993 code drop
 * 	[1993/10/11  12:55:27  keutel]
 * 
 * Revision 1.1.6.2  1993/08/12  12:31:29  marrek
 * 	July 1993 code drop.
 * 	[1993/08/03  12:28:29  marrek]
 * 
 * Revision 1.1.4.3  1993/01/05  20:34:32  htf
 * 	Embedding copyright notice
 * 	[1993/01/05  18:48:49  htf]
 * 
 * Revision 1.1.4.2  1992/12/01  15:04:50  marrek
 * 	November 1992 code drop
 * 	[1992/12/01  14:09:54  marrek]
 * 
 * Revision 1.1.2.4  1992/07/17  19:59:37  weisman
 * 	Checked in at OSF for SNI
 * 	[1992/07/17  19:36:15  weisman]
 * 
 * $EndLog$
 */
#ifndef _XT_PARMS_H
#define _XT_PARMS_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xt_parms_rcsid[] = "@(#)$RCSfile: xt_parms.h,v $ $Revision: 1.1.13.2 $ $Date: 1996/02/18 23:04:13 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/
 
/****************************************************************************/
/*                                                                          */
/*   Static declarations for parameters to the XDS / XOM functions          */
/*                                                                          */
/****************************************************************************/
 
#include <xt_len.h>

 
/*** NOTE: dsaname file no longer exists ***/
 
/* DSA names are initialiased by the test-tool from the specified files: */
/*      $DIRINFBAS/dsa/dir?/dsaname    (? = 1,2,3,4,5)               */
 
OM_descriptor dsa1_name[XT_DSANAME_MAX];/*C=de/O=dbp/OU=dap11/CN=dsa/CN=dsa-m1*/
OM_descriptor dsa2_name[XT_DSANAME_MAX]; 
OM_descriptor dsa3_name[XT_DSANAME_MAX]; 
OM_descriptor dsa4_name[XT_DSANAME_MAX]; 
OM_descriptor dsa5_name[XT_DSANAME_MAX];
 
 
/* country */
 
static OM_descriptor    country0[] = {
    OM_OID_DESC(OM_CLASS, DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_COUNTRY_NAME),
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("us")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    country1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_COUNTRY_NAME),
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("ie")},
    OM_NULL_DESCRIPTOR
};
 
/* invalid value */
static OM_descriptor    country2[] = {
    OM_OID_DESC(OM_CLASS, DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_COUNTRY_NAME),
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("IRELAND")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    country3[] = {
    OM_OID_DESC(OM_CLASS, DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_COUNTRY_NAME),
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("de")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    country4[] = {
    OM_OID_DESC(OM_CLASS, DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_COUNTRY_NAME),
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("gb")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    country5[] = {
    OM_OID_DESC(OM_CLASS, DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_COUNTRY_NAME),
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("AD")},
    OM_NULL_DESCRIPTOR
};

static OM_descriptor    bad_country1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_TITLE),
    {DS_ATTRIBUTE_VALUES, OM_S_INTEGER, OM_STRING("ie0X32")},
    OM_NULL_DESCRIPTOR
};
 
 
/* organization */
 
static OM_descriptor    org1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_ORG_NAME),
    {DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("sni")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    org2[] = {
    OM_OID_DESC(OM_CLASS, DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_ORG_NAME),
    {DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("Siemens Nixdorf Informationssysteme AG")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    org3[] = {
    OM_OID_DESC(OM_CLASS, DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_ORG_NAME),
    {DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("siemens")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    org4[] = {
    OM_OID_DESC(OM_CLASS, DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_ORG_NAME),
    {DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("xxx")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    org5[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_ORG_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("dbp")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    org6[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_ORG_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("digital")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    org7[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_ORG_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("o1")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    org8[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_ORG_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING | OM_S_LOCAL_STRING,
	OM_STRING("Äthärische Ökölogische Überdrüß")},
    OM_NULL_DESCRIPTOR
};
 
 
/* organizational unit */
 
static OM_descriptor    org_unit1[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_ORG_UNIT_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("ap11")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    org_unit2[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_ORG_UNIT_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("sci")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    org_unit3[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_ORG_UNIT_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("Software Centre Ireland")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    org_unit4[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_ORG_UNIT_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("dap11")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    org_unit5[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_ORG_UNIT_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("ncsl")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    org_unit6[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_ORG_UNIT_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("ou1")},
    OM_NULL_DESCRIPTOR
};
 
/* common name */
 
static OM_descriptor    c_name1[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_COMMON_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("dsa")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    c_name2[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_COMMON_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("helmut")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    c_name3[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_COMMON_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("vinnie")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    c_name4[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_COMMON_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("marysia")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    c_name5[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_COMMON_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("dsa-m1")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    c_name6[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_COMMON_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("cahill")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    c_name7[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_COMMON_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("xxx")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    c_name8[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_COMMON_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("vincent")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    c_name9[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_COMMON_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("DBA")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    c_name10[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_COMMON_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("modem")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    c_name11[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_COMMON_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("XDS")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    c_name12[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_COMMON_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("brian")},
    OM_NULL_DESCRIPTOR
};
 
/* invalid value for specified syntax */
static OM_descriptor    c_name13[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_COMMON_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("\x0D\x0E\x0F")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    c_name14[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_COMMON_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("vin")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    c_name15[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_COMMON_NAME),
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    c_name16[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_COMMON_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("dsa-m2")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    c_name17[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_COMMON_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("dsa1")},
    OM_NULL_DESCRIPTOR
};
 
/* CN=m:uller */
static OM_descriptor    c_name18[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_COMMON_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("m\xC8\x75\x6C\x6C\x65\x72")},
    OM_NULL_DESCRIPTOR
};
 
/* CN=sekret:ar */
static OM_descriptor    c_name19[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_COMMON_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("sekret\xC8\x61\x72")},
    OM_NULL_DESCRIPTOR
};
 
/* MHS common names */
 
/* CN=project leaders */
static OM_descriptor    c_name20[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_COMMON_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("project leaders")},
    OM_NULL_DESCRIPTOR
};
 
/* CN=ms */
static OM_descriptor    c_name21[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_COMMON_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("ms")},
    OM_NULL_DESCRIPTOR
};
 
/* CN=ms1 */
static OM_descriptor    c_name22[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_COMMON_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("ms1")},
    OM_NULL_DESCRIPTOR
};
 
/* CN=mta */
static OM_descriptor    c_name23[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_COMMON_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("mta")},
    OM_NULL_DESCRIPTOR
};
 
/* CN=mta1 */
static OM_descriptor    c_name24[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_COMMON_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("mta1")},
    OM_NULL_DESCRIPTOR
};
 
/* CN=ua */
static OM_descriptor    c_name25[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_COMMON_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("ua")},
    OM_NULL_DESCRIPTOR
};
 
/* CN=ua1 */
static OM_descriptor    c_name26[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_COMMON_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("ua1")},
    OM_NULL_DESCRIPTOR
};
 
/* CN=MHS Administrator */
static OM_descriptor    c_name28[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_COMMON_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("MHS Administrator")},
    OM_NULL_DESCRIPTOR
};
 
/* CN=Patrick */
static OM_descriptor    c_name29[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_COMMON_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("Patrick")},
    OM_NULL_DESCRIPTOR
};
 
 
/* CN=nmo-agent */
static OM_descriptor    c_name30[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_COMMON_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("nmo-agent")},
    OM_NULL_DESCRIPTOR
};
 
 
static OM_descriptor    c_name31[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_COMMON_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("dsa7")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    c_name32[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_COMMON_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING | OM_S_LOCAL_STRING,
	OM_STRING("äÄöÖüÜß")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    c_name33[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_COMMON_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING, OM_STRING("sap")},
    OM_NULL_DESCRIPTOR
};
 
/* Certification Authority */

static OM_descriptor    ca_name1[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_ORG_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING, OM_STRING("ca")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    ca_name2[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_ORG_UNIT_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING, OM_STRING("ca")},
    OM_NULL_DESCRIPTOR
};
 
 
/* surname */
 
static OM_descriptor    surname1[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_SURNAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("Ryan")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    surname2[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_SURNAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("Cahill")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    surname3[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_SURNAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("Volpers")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    surname4[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_SURNAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("Keane")},
    OM_NULL_DESCRIPTOR
};
 
/* invalid syntax/value */
static OM_descriptor    surname5[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_SURNAME),
    {DS_ATTRIBUTE_VALUES,OM_S_OCTET_STRING,OM_STRING("\x02\x03\x04")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    surname6[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_SURNAME),
    OM_NULL_DESCRIPTOR
};
 
 
  /* invalid syntax */
static OM_descriptor    surname7[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_SURNAME),
    {DS_ATTRIBUTE_VALUES,((OM_syntax)99),OM_STRING("bad-surname")},
    OM_NULL_DESCRIPTOR
};
  
static OM_descriptor    surname8[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_SURNAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING, OM_STRING("sap")},
    OM_NULL_DESCRIPTOR
};
 

/* user password */
 
static OM_descriptor    user_pswd1[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_USER_PASSWORD),
    {DS_ATTRIBUTE_VALUES,OM_S_OCTET_STRING,OM_STRING("secret")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    user_pswd2[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_USER_PASSWORD),
    {DS_ATTRIBUTE_VALUES,OM_S_OCTET_STRING,OM_STRING("xxx")},
    OM_NULL_DESCRIPTOR
    };
 
/* invalid syntax */
static OM_descriptor    user_pswd3[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_USER_PASSWORD),
    {DS_ATTRIBUTE_VALUES,OM_S_PRINTABLE_STRING,OM_STRING("secret")},
    OM_NULL_DESCRIPTOR
    };
 
/* missing value */
static OM_descriptor    user_pswd4[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_USER_PASSWORD),
    OM_NULL_DESCRIPTOR
    };
 
 
/* locality name */
 
static OM_descriptor    loc_name1[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_LOCALITY_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("Dublin")},
    OM_NULL_DESCRIPTOR
    };
 
 
/* street address */
 
static OM_descriptor    str_addr1[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_STREET_ADDRESS),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("Fitzwilliam Court")},
    OM_NULL_DESCRIPTOR
    };
 
static OM_descriptor    str_addr2[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_STREET_ADDRESS),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("Leeson Close")},
    OM_NULL_DESCRIPTOR
    };
 
 
/* undefined attribute */
 
#define OMP_O_DS_A_UNDEFINED_ATTRIBUTE  "\x55\xFF\xFF"
OM_EXPORT(DS_A_UNDEFINED_ATTRIBUTE)
 
static OM_descriptor    undefined1[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_UNDEFINED_ATTRIBUTE),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("xxx")},
    OM_NULL_DESCRIPTOR
    };
 
/* typeless */
 
static OM_descriptor    typeless1[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DSX_TYPELESS_RDN),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("sales")},
    OM_NULL_DESCRIPTOR
    };
 
static OM_descriptor    typeless2[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DSX_TYPELESS_RDN),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("printer")},
    OM_NULL_DESCRIPTOR
    };
 
static OM_descriptor    typeless3[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DSX_TYPELESS_RDN),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("agfa")},
    OM_NULL_DESCRIPTOR
    };
 
static OM_descriptor    typeless4[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DSX_TYPELESS_RDN),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("philip")},
    OM_NULL_DESCRIPTOR
    };
 
static OM_descriptor    typeless6[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DSX_TYPELESS_RDN),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("admin")},
    OM_NULL_DESCRIPTOR
    };
 
/* CN=m:uller */
static OM_descriptor    typeless7[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DSX_TYPELESS_RDN),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("m\xC8\x75\x6C\x6C\x65\x72")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    typeless8[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DSX_TYPELESS_RDN),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("marysia")},
    OM_NULL_DESCRIPTOR
    };
 
static OM_descriptor    typeless9[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DSX_TYPELESS_RDN),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("vinnie")},
    OM_NULL_DESCRIPTOR
    };
 
static OM_descriptor    typeless10[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DSX_TYPELESS_RDN),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("adm")},
    OM_NULL_DESCRIPTOR
    };
 
#ifndef SNI_CELL_NAME
#define SNI_CELL_NAME "snidec.sni.com"
#endif /* SNI_CELL_NAME */

static OM_descriptor    typeless11[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DSX_TYPELESS_RDN),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING(SNI_CELL_NAME)},
    OM_NULL_DESCRIPTOR
    };
 
static OM_descriptor    bad_ava[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_ORG_NAME),
    OM_NULL_DESCRIPTOR
    };
 
 
 
/* rdn */
 
static OM_descriptor    rdn0[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, country0}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, country1}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn2[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, org1}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn3[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, org2}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn4[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, org_unit1}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn5[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, org_unit2}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn7[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, c_name1}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn8[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, c_name2}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn9[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, c_name3}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn10[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, c_name4}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn11[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, c_name5}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn12[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, c_name6}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn13[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, surname1}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn14[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, c_name31}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn16[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, org3}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn17[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, org_unit4}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn18[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, country1}},
    {DS_AVAS, OM_S_OBJECT, {0, org1}},
    {DS_AVAS, OM_S_OBJECT, {0, bad_ava}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn19[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, c_name7}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn20[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, c_name8}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn21[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, c_name9}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn22[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, c_name10}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn23[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, loc_name1}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn24[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, c_name11}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn25[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, c_name12}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn26[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, org4}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn27[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, c_name13}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn28[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, org_unit5}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn29[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, c_name14}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn30[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, country2}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn31[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, c_name15}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn32[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, country3}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn33[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, org5}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn34[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, c_name16}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn35[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, c_name17}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn36[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, country4}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn37[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, c_name18}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn38[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, c_name19}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    bad_rdn1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, bad_country1}},
    {DS_RDNS,OM_S_OBJECT, {0, rdn0}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn40[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, org6}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn41[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, typeless1}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn42[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, typeless2}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn43[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, typeless3}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn44[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, typeless4}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn46[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, typeless6}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn47[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, typeless7}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn48[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, typeless8}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn49[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, typeless9}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn50[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, typeless10}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn51[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, typeless11}},
    OM_NULL_DESCRIPTOR
};
 
 
/* MHS rdns */
 
static OM_descriptor    rdn52[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, c_name20}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn53[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, c_name21}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn54[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, c_name22}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn55[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, c_name23}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn56[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, c_name24}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn57[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, c_name25}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn58[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, c_name26}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn59[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, c_name28}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn60[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, c_name29}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn61[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, c_name12}},
    {DS_AVAS, OM_S_OBJECT, {0, str_addr2}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn62[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, str_addr2}},
    {DS_AVAS, OM_S_OBJECT, {0, c_name12}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn63[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, c_name4}},
    {DS_AVAS, OM_S_OBJECT, {0, str_addr1}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn64[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, str_addr1}},
    {DS_AVAS, OM_S_OBJECT, {0, c_name4}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn65[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, c_name30}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn66[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, country2}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn67[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, org7}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn68[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, org_unit6}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn69[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, c_name32}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn70[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, c_name33}},
    OM_NULL_DESCRIPTOR
};

static OM_descriptor    rdn71[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, ca_name1}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    rdn72[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, ca_name2}},
    OM_NULL_DESCRIPTOR
};
 

/* name */
 
static OM_descriptor top_name[] = {
    OM_OID_DESC(OM_CLASS,DS_C_DS_DN),
    OM_NULL_DESCRIPTOR
    };
 
/* C=us */
static OM_descriptor name0[] = {
    OM_OID_DESC(OM_CLASS,DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn0}},
    OM_NULL_DESCRIPTOR
    };
 
/* C=ie */
static OM_descriptor name1[] = {
    OM_OID_DESC(OM_CLASS,DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    OM_NULL_DESCRIPTOR
    };
 
/* C=ie/O=sni */
static OM_descriptor name2[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    OM_NULL_DESCRIPTOR
};
 
/* C=de/O=sni/OU=ap11 */
static OM_descriptor name3[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS, OM_S_OBJECT, {0, rdn32} },
    {DS_RDNS, OM_S_OBJECT, {0, rdn2} },
    {DS_RDNS, OM_S_OBJECT, {0, rdn4} },
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/O=sni/OU=sci */
static OM_descriptor name4[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS, OM_S_OBJECT, {0, rdn1} },
    {DS_RDNS, OM_S_OBJECT, {0, rdn2} },
    {DS_RDNS, OM_S_OBJECT, {0, rdn5} },
    OM_NULL_DESCRIPTOR
};
 
/* C=de/O=sni/OU=ap11/CN=helmut */
static OM_descriptor name5[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn32}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn4}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn8}},
    OM_NULL_DESCRIPTOR
};
 
/* C=us/O=sni/OU=ap11/CN=vinnie */
static OM_descriptor name6[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn0}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn4}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn9}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/O=sni/OU=sci/CN=marysia */
static OM_descriptor name7[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn5}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn10}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/O=sni/OU=sci/CN=vinnie */
static OM_descriptor name8[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn5}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn9}},
    OM_NULL_DESCRIPTOR
};
 
/* O=sni */
static OM_descriptor name9[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    OM_NULL_DESCRIPTOR
};
 
/* OU=ap11 */
static OM_descriptor name10[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn4}},
    OM_NULL_DESCRIPTOR
};
 
/* OU=sci */
static OM_descriptor name11[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn5}},
    OM_NULL_DESCRIPTOR
};
 
/* CN=helmut */
static OM_descriptor name12[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn8}},
    OM_NULL_DESCRIPTOR
};
 
/* CN=vinnie */
static OM_descriptor name13[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS, OM_S_OBJECT, {0, rdn9}},
    OM_NULL_DESCRIPTOR
};
 
/* CN=marysia */
static OM_descriptor name14[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn10}},
    OM_NULL_DESCRIPTOR
};
 
/* CN=cahill */
static OM_descriptor name15[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn12}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/O=sni/OU=sci/CN=cahill */
static OM_descriptor name16[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn5}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn12}},
    OM_NULL_DESCRIPTOR
};
 
/* C=de/O=dbp/OU=dap11/CN=dsa/CN=dsa-m1 */
static OM_descriptor name17[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn32}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn33}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn17}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn7}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn11}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/O=sni/OU=sci/CN=xxx */
static OM_descriptor name18[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn5}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn19}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/O=sni/OU=sci/CN=vincent */
static OM_descriptor name19[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn5}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn20}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/O=sni/OU=sci/CN=DBA */
static OM_descriptor name20[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn5}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn21}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/O=sni/OU=sci/CN=modem */
static OM_descriptor name21[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn5}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn22}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/L=dublin */
static OM_descriptor name22[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn23}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/L=dublin/CN=XDS */
static OM_descriptor name23[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn23}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn24}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/L=dublin/CN=marysia,SA=Fitzwilliam Court */
static OM_descriptor name24[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn23}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn63}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/L=dublin/CN=brian,SA=Leeson Close */
static OM_descriptor name25[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn23}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn61}},
    OM_NULL_DESCRIPTOR
};
 
/* C=gb/O=sni */
static OM_descriptor name26[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn36}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/O=xxx/OU=sci */
static OM_descriptor name27[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn26}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn5}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/O=sni/OU=sci/CN=vincent/CN=xxx */
static OM_descriptor name28[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn5}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn20}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn19}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/O=sni/C=us */
static OM_descriptor name29[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn0}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/O=sni/OU=sci/CN=xxx */
static OM_descriptor name30[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn5}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn19}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/O=sni/OU=sci/CN=\x0D\x0E\x0F */
static OM_descriptor name31[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn5}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn27}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/O=sni/OU=ncsl */
static OM_descriptor name32[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn28}},
    OM_NULL_DESCRIPTOR
};
 
/* CN=vin */
static OM_descriptor name33[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS, OM_S_OBJECT, {0, rdn29}},
    OM_NULL_DESCRIPTOR
};
 
/* O=siemens */
static OM_descriptor name34[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS, OM_S_OBJECT, {0, rdn16}},
    OM_NULL_DESCRIPTOR
};
 
/* CN=\x0D\x0E\x0F */
static OM_descriptor name35[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn27}},
    OM_NULL_DESCRIPTOR
};
 
/* C=IRELAND */
static OM_descriptor name36[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn30}},
    OM_NULL_DESCRIPTOR
};
 
/* SN=Ryan */
static OM_descriptor name37[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS, OM_S_OBJECT, {0, rdn13}},
    OM_NULL_DESCRIPTOR
};

/* CN= */ 
static OM_descriptor name38[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn31}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/O=sni/OU=sci/CN=vin */
static OM_descriptor name39[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn5}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn29}},
    OM_NULL_DESCRIPTOR
};
 
/* CN=vincent */
static OM_descriptor name40[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS, OM_S_OBJECT, {0, rdn20}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/O=sni/OU=sci/CN= */
static OM_descriptor name41[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn5}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn31}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/O=sni/OU=sci/CN=dsa */
static OM_descriptor name42[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn5}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn7}},
    OM_NULL_DESCRIPTOR
};
 
/* C=de */
static OM_descriptor name43[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn32}},
    OM_NULL_DESCRIPTOR
};
 
/* C=de/O=sni */
static OM_descriptor name44[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn32}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    OM_NULL_DESCRIPTOR
};
 
/* C=de */
static OM_descriptor name45[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn32}},
    OM_NULL_DESCRIPTOR
};
 
/* C=de/O=dbp/OU=dap11/CN=dsa/CN=dsa-m2 */
static OM_descriptor name46[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn32}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn33}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn17}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn7}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn34}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/O=sni/OU=sci/CN=dsa/CN=dsa1 */
static OM_descriptor name47[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn5}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn7}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn35}},
    OM_NULL_DESCRIPTOR
};
 
/* C=gb */
static OM_descriptor name48[] = {
    OM_OID_DESC(OM_CLASS,DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn36}},
    OM_NULL_DESCRIPTOR
    };
 
/* C=ie/O=sni/OU=sci/CN=m:uller */
static OM_descriptor name49[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn5}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn37}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/O=sni/OU=sci/CN=sekret:ar */
static OM_descriptor name50[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn5}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn38}},
    OM_NULL_DESCRIPTOR
};
 
/* C=de/O=dbp/OU=dap11/CN=dsa */
static OM_descriptor name51[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn32}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn33}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn17}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn7}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/O=digital/TY=sales/TY=printer */
static OM_descriptor name52[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn40}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn41}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn42}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/O=digital/TY=sales/TY=agfa */
static OM_descriptor name53[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn40}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn41}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn43}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/O=digital/TY=sales/TY=admin/TY=philip */
static OM_descriptor name54[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn40}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn41}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn46}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn44}},
    OM_NULL_DESCRIPTOR
};
 
/* TY=agfa */
static OM_descriptor name55[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn43}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/O=digital/TY=sales/TY=philip */
static OM_descriptor name56[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn40}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn41}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn44}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/O=digital/TY=sales */
static OM_descriptor name57[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn40}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn41}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/O=digital/TY=sales/TY=m:uller */
static OM_descriptor name58[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn40}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn41}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn47}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/O=digital/TY=sales/TY=admin/TY=marysia */
static OM_descriptor name59[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn40}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn41}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn46}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn48}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/O=digital/TY=sales/TY=admin/TY=vinnie */
static OM_descriptor name60[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn40}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn41}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn46}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn49}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/O=digital/TY=sales/TY=admin/TY=printer */
static OM_descriptor name61[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn40}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn41}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn46}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn42}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/O=digital/TY=sales/TY=admin/TY=agfa */
static OM_descriptor name62[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn40}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn41}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn46}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn43}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/O=digital/TY=sales/TY=agfa */
static OM_descriptor name63[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn40}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn41}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn46}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/O=digital/TY=sales/TY=adm */
static OM_descriptor name64[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn40}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn41}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn50}},
    OM_NULL_DESCRIPTOR
};
 
/* C=us/O=siemens/OU=sci/TY=sales/TY=philip */
static OM_descriptor name65[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn0}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn16}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn5}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn41}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn44}},
    OM_NULL_DESCRIPTOR
};
 
/* TY=sales/TY=philip */
static OM_descriptor name66[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn41}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn44}},
    OM_NULL_DESCRIPTOR
};
 
/* C=gb/O=siemens */
static OM_descriptor name67[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn36}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn16}},
    OM_NULL_DESCRIPTOR
};
 
/* TY=snidec.sni.com/TY=sales/TY=printer */
static OM_descriptor name68[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn51}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn41}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn42}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/O=digital/TY=sales/TY=admin */
static OM_descriptor name69[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn40}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn41}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn46}},
    OM_NULL_DESCRIPTOR
};
 
/* TY=snidec.sni.com/TY=sales/TY=admin */
static OM_descriptor name70[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn51}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn41}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn46}},
    OM_NULL_DESCRIPTOR
};
 
/* TY=snidec.sni.com/TY=sales/TY=admin/TY=vinnie */
static OM_descriptor name71[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn51}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn41}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn46}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn49}},
    OM_NULL_DESCRIPTOR
};
 
/* MHS names */
 
/* C=ie/O=sni/OU=sci/CN=project leaders */
static OM_descriptor name73[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn5}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn52}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/O=sni/OU=sci/CN=ms/CN=ms1 */
static OM_descriptor name74[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn5}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn53}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn54}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/O=sni/OU=sci/CN=mta/CN=mta1 */
static OM_descriptor name75[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn5}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn55}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn56}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/O=sni/OU=sci/CN=ua/CN=ua1 */
static OM_descriptor name76[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn5}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn57}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn58}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/O=sni/OU=sci/CN=MHS Administrator */
static OM_descriptor name77[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn5}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn59}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/O=sni/OU=sci/CN=Patrick */
static OM_descriptor name78[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn5}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn60}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/O=sni/OU=sci/CN=ms */
static OM_descriptor name79[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn5}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn53}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/O=sni/OU=sci/CN=ua */
static OM_descriptor name80[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn5}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn57}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/O=sni/OU=sci/CN=mta */
static OM_descriptor name81[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn5}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn55}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/L=dublin/SA=Fitzwilliam Court,CN=marysia */
static OM_descriptor name82[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn23}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn64}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/L=dublin/SA=Leeson Close,CN=brian */
static OM_descriptor name83[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn23}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn62}},
    OM_NULL_DESCRIPTOR
};
 
/* CN=brian,SA=Leeson Close */
static OM_descriptor name84[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn61}},
    OM_NULL_DESCRIPTOR
};
 
/* CN=brian,SA=Leeson Close */
static OM_descriptor name85[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn63}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/L=dublin/CN=brian */
static OM_descriptor name86[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn23}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn25}},
    OM_NULL_DESCRIPTOR
};

/* C=ie/O=sni/OU=sci/CN=dsa/CN=nmo-agent */
static OM_descriptor name87[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn5}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn7}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn65}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/CN=nmo-agent */
static OM_descriptor name88[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn65}},
    OM_NULL_DESCRIPTOR
};
 
/* C=de/O=sni/OU=dap11/CN=dsa/CN=dsa7 */
static OM_descriptor name89[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn32}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn4}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn7}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn14}},
    OM_NULL_DESCRIPTOR
};
 
/* C=AD/O=o1/OU=ou1 */
static OM_descriptor name90[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn66}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn67}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn68}},
    OM_NULL_DESCRIPTOR
};

/* C=ie/O=sni/OU=sci/CN=some umlauts */
static OM_descriptor name91[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn5}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn69}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/O=sni/OU=sci/CN=sap */
static OM_descriptor name92[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn5}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn70}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/O=ca */
static OM_descriptor name93[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn71}},
    OM_NULL_DESCRIPTOR
};
 
/* C=ie/O=sni/OU=ca */
static OM_descriptor name94[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn72}},
    OM_NULL_DESCRIPTOR
};
 
 
static OM_descriptor bad_name1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,bad_rdn1}},
    {DS_T_SELECTOR, OM_S_OCTET_STRING, OM_STRING("22010")}, 
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor bad_name2[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn18}},
    OM_NULL_DESCRIPTOR
};
 
 
/* address */
 
/* DSA #1 psapAddress */
static OM_descriptor    address1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_PRESENTATION_ADDRESS),
    {DS_N_ADDRESSES, OM_S_OCTET_STRING, OM_STRING("127.0.0.1")},
    {DS_T_SELECTOR, OM_S_OCTET_STRING, OM_STRING("Server")}, 
    OM_NULL_DESCRIPTOR
};
 
 
/* DSA #3 psapAddress */
static OM_descriptor    address5[] = {
    OM_OID_DESC(OM_CLASS, DS_C_PRESENTATION_ADDRESS),
    {DS_N_ADDRESSES, OM_S_OCTET_STRING, OM_STRING("127.0.0.1")},
    {DS_T_SELECTOR, OM_S_OCTET_STRING, OM_STRING("22030")}, 
    OM_NULL_DESCRIPTOR
};
 
 
 
/* BDCP objects */
 
static OM_descriptor    fax_number1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_FACSIMILE_PHONE_NBR),
    {DS_PHONE_NBR, OM_S_PRINTABLE_STRING, OM_STRING("+49 89 636 51")}, 
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    postal_address1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_POSTAL_ADDRESS),
    {DS_POSTAL_ADDRESS, OM_S_TELETEX_STRING, OM_STRING("SNI")}, 
    {DS_POSTAL_ADDRESS, OM_S_TELETEX_STRING, OM_STRING("Otto-Hahn-Ring 6")}, 
    {DS_POSTAL_ADDRESS, OM_S_TELETEX_STRING, OM_STRING("D-8000 Munich 83")}, 
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    search_criterion1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_SEARCH_CRITERION),
    {DS_FILTER_ITEM_TYPE, OM_S_ENUMERATION, {DS_EQUALITY, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_OBJECT_CLASS),
    {DS_FILTER_TYPE, OM_S_ENUMERATION, {DS_ITEM, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    search_guide1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_SEARCH_GUIDE),
    {DS_CRITERIA, OM_S_OBJECT, {0, search_criterion1}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    teletex_terminal_id1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_TELETEX_TERM_IDENT),
    {DS_TELETEX_TERM, OM_S_PRINTABLE_STRING, OM_STRING("898250=siemcp")}, 
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    telex_number1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_TELEX_NBR),
    {DS_ANSWERBACK, OM_S_PRINTABLE_STRING, OM_STRING("SNI")},
    {DS_COUNTRY_CODE, OM_S_PRINTABLE_STRING, OM_STRING("d")}, 
    {DS_TELEX_NBR, OM_S_PRINTABLE_STRING, OM_STRING("987654321")}, 
    OM_NULL_DESCRIPTOR
};
 
 
/* MHS objects */
 
/* OR addresses */
 
 
/* mnemonic */
static OM_descriptor    or_addr2[] = {
    OM_OID_DESC(OM_CLASS, MH_C_OR_ADDRESS),
    {MH_T_ADMD_NAME,    OM_S_PRINTABLE_STRING, OM_STRING("dbp") },
    {MH_T_COMMON_NAME,  OM_S_PRINTABLE_STRING, OM_STRING("ryan") },
    {MH_T_COUNTRY_NAME, OM_S_PRINTABLE_STRING, OM_STRING("de") },
    {MH_T_DOMAIN_TYPE_1,    OM_S_PRINTABLE_STRING, OM_STRING("internet") },
    {MH_T_DOMAIN_VALUE_1,   OM_S_PRINTABLE_STRING, OM_STRING("ryan at sni.org")},
    {MH_T_ORGANIZATION_NAME,OM_S_PRINTABLE_STRING, OM_STRING("sni") },
    {MH_T_ORGANIZATIONAL_UNIT_NAME_1,OM_S_PRINTABLE_STRING, OM_STRING("ap1") },
    {MH_T_ORGANIZATIONAL_UNIT_NAME_2,OM_S_PRINTABLE_STRING, OM_STRING("ap11") },
    {MH_T_ORGANIZATIONAL_UNIT_NAME_3,OM_S_PRINTABLE_STRING, OM_STRING("ap113")},
    {MH_T_GENERATION,   OM_S_PRINTABLE_STRING, OM_STRING("Jr") },
    {MH_T_GIVEN_NAME,   OM_S_PRINTABLE_STRING, OM_STRING("Vinnie") },
    {MH_T_INITIALS,     OM_S_PRINTABLE_STRING, OM_STRING("VR") },
    {MH_T_SURNAME,      OM_S_PRINTABLE_STRING, OM_STRING("Ryan") },
    {MH_T_PRMD_NAME,    OM_S_PRINTABLE_STRING, OM_STRING("sni") },
    OM_NULL_DESCRIPTOR
};
 
/* numeric */
static OM_descriptor    or_addr3[] = {
    OM_OID_DESC(OM_CLASS, MH_C_OR_ADDRESS),
    {MH_T_ADMD_NAME,    OM_S_PRINTABLE_STRING, OM_STRING("TE") },
    {MH_T_COUNTRY_NAME, OM_S_PRINTABLE_STRING, OM_STRING("IE") },
    {MH_T_DOMAIN_TYPE_1,    OM_S_PRINTABLE_STRING, OM_STRING("domain1") },
    {MH_T_DOMAIN_TYPE_1,    OM_S_TELETEX_STRING,   OM_STRING("domain1") },
    {MH_T_DOMAIN_TYPE_2,    OM_S_PRINTABLE_STRING, OM_STRING("domain2") },
    {MH_T_DOMAIN_TYPE_3,    OM_S_PRINTABLE_STRING, OM_STRING("domain3") },
    {MH_T_DOMAIN_TYPE_4,    OM_S_PRINTABLE_STRING, OM_STRING("domain4") },
    {MH_T_DOMAIN_VALUE_1,   OM_S_PRINTABLE_STRING, OM_STRING("domain value1") },
    {MH_T_DOMAIN_VALUE_1,   OM_S_TELETEX_STRING,   OM_STRING("domain value1") },
    {MH_T_DOMAIN_VALUE_2,   OM_S_PRINTABLE_STRING, OM_STRING("domain value2") },
    {MH_T_DOMAIN_VALUE_3,   OM_S_PRINTABLE_STRING, OM_STRING("domain value3") },
    {MH_T_DOMAIN_VALUE_4,   OM_S_PRINTABLE_STRING, OM_STRING("domain value4") },
    {MH_T_NUMERIC_USER_IDENTIFIER, OM_S_NUMERIC_STRING, OM_STRING("999854") },
    {MH_T_PRMD_NAME,    OM_S_PRINTABLE_STRING, OM_STRING("sni") },
    OM_NULL_DESCRIPTOR
};
 
/* structured postal */
static OM_descriptor    or_addr4[] = {
    OM_OID_DESC(OM_CLASS, MH_C_OR_ADDRESS),
    {MH_T_ADMD_NAME,    OM_S_PRINTABLE_STRING, OM_STRING("TE") },
    {MH_T_COUNTRY_NAME, OM_S_PRINTABLE_STRING, OM_STRING("IE") },
    {MH_T_POSTAL_ADDRESS_DETAILS, OM_S_PRINTABLE_STRING,OM_STRING("4th. Floor") },
    {MH_T_POSTAL_CODE,  OM_S_PRINTABLE_STRING, OM_STRING("Dublin 2") },
    {MH_T_POSTAL_COUNTRY_NAME, OM_S_PRINTABLE_STRING, OM_STRING("IE") },
    {MH_T_POSTAL_DELIVERY_POINT_NAME, OM_S_PRINTABLE_STRING, OM_STRING("Dublin") },
    {MH_T_POSTAL_DELIV_SYSTEM_NAME, OM_S_PRINTABLE_STRING, OM_STRING("PDS-11") },
    {MH_T_POSTAL_GENERAL_DELIV_ADDR, OM_S_PRINTABLE_STRING, OM_STRING("box no. 1234") },
    {MH_T_POSTAL_LOCALE,    OM_S_PRINTABLE_STRING, OM_STRING("Building No.4") },
    {MH_T_POSTAL_OFFICE_BOX_NUMBER, OM_S_PRINTABLE_STRING, OM_STRING("1234") },
    {MH_T_POSTAL_OFFICE_NAME, OM_S_PRINTABLE_STRING, OM_STRING("Dublin") },
    {MH_T_POSTAL_OFFICE_NUMBER, OM_S_PRINTABLE_STRING, OM_STRING("42") },
    {MH_T_POSTAL_ORGANIZATION_NAME, OM_S_PRINTABLE_STRING, OM_STRING("An Post") },
    {MH_T_POSTAL_PATRON_DETAILS, OM_S_PRINTABLE_STRING, OM_STRING("Mail Dept.") },
    {MH_T_POSTAL_PATRON_NAME, OM_S_PRINTABLE_STRING, OM_STRING("Postmaster") },
    {MH_T_POSTAL_STREET_ADDRESS, OM_S_PRINTABLE_STRING, OM_STRING("Leeson Close") },
    {MH_T_PRMD_NAME,    OM_S_PRINTABLE_STRING, OM_STRING("sni") },
    OM_NULL_DESCRIPTOR
};
 
/* unstructured postal */
static OM_descriptor    or_addr5[] = {
    OM_OID_DESC(OM_CLASS, MH_C_OR_ADDRESS),
    {MH_T_ADMD_NAME,    OM_S_PRINTABLE_STRING, OM_STRING("TE") },
    {MH_T_COUNTRY_NAME, OM_S_PRINTABLE_STRING, OM_STRING("IE") },
    {MH_T_POSTAL_ADDRESS_IN_FULL, OM_S_TELETEX_STRING,OM_STRING("Siemens Nixdorf Information Systems, Fitzwilliam Court, Leeson Close, Dublin 2") },
    {MH_T_POSTAL_CODE,  OM_S_PRINTABLE_STRING, OM_STRING("Dublin 2") },
    {MH_T_POSTAL_COUNTRY_NAME, OM_S_PRINTABLE_STRING, OM_STRING("IE") },
    {MH_T_POSTAL_DELIV_SYSTEM_NAME, OM_S_PRINTABLE_STRING, OM_STRING("An Post") },
    {MH_T_PRMD_NAME,    OM_S_PRINTABLE_STRING, OM_STRING("sni") },
    OM_NULL_DESCRIPTOR
};
 
/* terminal */
static OM_descriptor    or_addr6[] = {
    OM_OID_DESC(OM_CLASS, MH_C_OR_ADDRESS),
    {MH_T_ADMD_NAME,    OM_S_PRINTABLE_STRING, OM_STRING("TE") },
    {MH_T_COUNTRY_NAME, OM_S_PRINTABLE_STRING, OM_STRING("IE") },
    {MH_T_DOMAIN_TYPE_1,    OM_S_PRINTABLE_STRING, OM_STRING("domain1") },
    {MH_T_DOMAIN_TYPE_2,    OM_S_PRINTABLE_STRING, OM_STRING("domain2") },
    {MH_T_DOMAIN_TYPE_3,    OM_S_PRINTABLE_STRING, OM_STRING("domain3") },
    {MH_T_DOMAIN_TYPE_4,    OM_S_PRINTABLE_STRING, OM_STRING("domain4") },
    {MH_T_DOMAIN_VALUE_1,   OM_S_PRINTABLE_STRING, OM_STRING("domain value1") },
    {MH_T_DOMAIN_VALUE_2,   OM_S_PRINTABLE_STRING, OM_STRING("domain value2") },
    {MH_T_DOMAIN_VALUE_3,   OM_S_PRINTABLE_STRING, OM_STRING("domain value3") },
    {MH_T_DOMAIN_VALUE_4,   OM_S_PRINTABLE_STRING, OM_STRING("domain value4") },
    {MH_T_ISDN_NUMBER,  OM_S_NUMERIC_STRING, OM_STRING("865434222") },
    {MH_T_ISDN_SUBADDRESS,  OM_S_NUMERIC_STRING, OM_STRING("11233255231") },
/*  {MH_T_PRESENTATION_ADDRESS, OM_S_OBJECT, { 0, address1 } }, */
    {MH_T_PRMD_NAME,    OM_S_PRINTABLE_STRING, OM_STRING("sni") },
    {MH_T_TERMINAL_IDENTIFIER, OM_S_PRINTABLE_STRING, OM_STRING("term. 54") },
    {MH_T_TERMINAL_TYPE,    OM_S_ENUMERATION, {MH_TT_IA5_TERMINAL, NULL} },
    {MH_T_X121_ADDRESS, OM_S_NUMERIC_STRING, OM_STRING("5542313131222") },
    OM_NULL_DESCRIPTOR
};
 
/* mnemonic - surname missing */
static OM_descriptor    or_addr7[] = {
    OM_OID_DESC(OM_CLASS, MH_C_OR_ADDRESS),
    {MH_T_ADMD_NAME,    OM_S_PRINTABLE_STRING, OM_STRING("dbp") },
    {MH_T_COMMON_NAME,  OM_S_PRINTABLE_STRING, OM_STRING("ryan") },
    {MH_T_COUNTRY_NAME, OM_S_PRINTABLE_STRING, OM_STRING("de") },
    {MH_T_DOMAIN_TYPE_1,    OM_S_PRINTABLE_STRING, OM_STRING("internet") },
    {MH_T_DOMAIN_VALUE_1,   OM_S_PRINTABLE_STRING, OM_STRING("ryan at sni.org") },
    {MH_T_ORGANIZATION_NAME,OM_S_PRINTABLE_STRING, OM_STRING("sni") },
    {MH_T_ORGANIZATIONAL_UNIT_NAME_1,OM_S_PRINTABLE_STRING, OM_STRING("ap1") },
    {MH_T_ORGANIZATIONAL_UNIT_NAME_2,OM_S_PRINTABLE_STRING, OM_STRING("ap11") },
    {MH_T_ORGANIZATIONAL_UNIT_NAME_3,OM_S_PRINTABLE_STRING, OM_STRING("ap113")},
    {MH_T_GENERATION,   OM_S_PRINTABLE_STRING, OM_STRING("Jr") },
    {MH_T_GIVEN_NAME,   OM_S_PRINTABLE_STRING, OM_STRING("Vinnie") },
    {MH_T_INITIALS,     OM_S_PRINTABLE_STRING, OM_STRING("VR") },
    {MH_T_PRMD_NAME,    OM_S_PRINTABLE_STRING, OM_STRING("sni") },
    OM_NULL_DESCRIPTOR
};
 
/* terminal - PSAP and ISDN */
static OM_descriptor    or_addr8[] = {
    OM_OID_DESC(OM_CLASS, MH_C_OR_ADDRESS),
    {MH_T_ADMD_NAME,    OM_S_PRINTABLE_STRING, OM_STRING("TE") },
    {MH_T_COUNTRY_NAME, OM_S_PRINTABLE_STRING, OM_STRING("IE") },
    {MH_T_ISDN_NUMBER,  OM_S_NUMERIC_STRING, OM_STRING("865434222") },
    {MH_T_ISDN_SUBADDRESS,  OM_S_NUMERIC_STRING, OM_STRING("11233255231") },
    {MH_T_PRESENTATION_ADDRESS, OM_S_OBJECT, { 0, address1 } },
    {MH_T_PRMD_NAME,    OM_S_PRINTABLE_STRING, OM_STRING("sni") },
    {MH_T_TERMINAL_IDENTIFIER, OM_S_PRINTABLE_STRING, OM_STRING("term. 54") },
    {MH_T_TERMINAL_TYPE,    OM_S_ENUMERATION, {MH_TT_IA5_TERMINAL, NULL} },
    {MH_T_X121_ADDRESS, OM_S_NUMERIC_STRING, OM_STRING("5542313131222") },
    OM_NULL_DESCRIPTOR
};
 
/* numeric - domain-values but no domain-types */
static OM_descriptor    or_addr9[] = {
    OM_OID_DESC(OM_CLASS, MH_C_OR_ADDRESS),
    {MH_T_ADMD_NAME,    OM_S_PRINTABLE_STRING, OM_STRING("TE") },
    {MH_T_COUNTRY_NAME, OM_S_PRINTABLE_STRING, OM_STRING("IE") },
    {MH_T_DOMAIN_VALUE_1,   OM_S_PRINTABLE_STRING, OM_STRING("domain value1") },
    {MH_T_DOMAIN_VALUE_2,   OM_S_PRINTABLE_STRING, OM_STRING("domain value2") },
    {MH_T_DOMAIN_VALUE_3,   OM_S_PRINTABLE_STRING, OM_STRING("domain value3") },
    {MH_T_DOMAIN_VALUE_4,   OM_S_PRINTABLE_STRING, OM_STRING("domain value4") },
    {MH_T_NUMERIC_USER_IDENTIFIER, OM_S_NUMERIC_STRING, OM_STRING("999854") },
    {MH_T_PRMD_NAME,    OM_S_PRINTABLE_STRING, OM_STRING("sni") },
    OM_NULL_DESCRIPTOR
};
 
 
 
/* AVAs (continued) */
 
static OM_descriptor    or_address1[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OR_ADDRESSES),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, or_addr2} },
    OM_NULL_DESCRIPTOR
    };
 
static OM_descriptor    or_address2[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OR_ADDRESSES),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, or_addr3} },
    OM_NULL_DESCRIPTOR
    };
 
 
 
/* OR names */
 
static OM_descriptor    or_name1[] = {
    OM_OID_DESC(OM_CLASS, MH_C_OR_NAME),
    {MH_T_DIRECTORY_NAME, OM_S_OBJECT, {0, name8} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    or_name2[] = {
    OM_OID_DESC(OM_CLASS, MH_C_OR_NAME),
    {MH_T_DIRECTORY_NAME, OM_S_OBJECT, {0, name7} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    or_name3[] = {
    OM_OID_DESC(OM_CLASS, MH_C_OR_NAME),
    {MH_T_DIRECTORY_NAME, OM_S_OBJECT, {0, name49} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    or_name4[] = {
    OM_OID_DESC(OM_CLASS, MH_C_OR_NAME),
    {MH_T_DIRECTORY_NAME, OM_S_OBJECT, {0, name8} },
    {MH_T_ADMD_NAME,    OM_S_PRINTABLE_STRING, OM_STRING("dbp") },
    {MH_T_COMMON_NAME,  OM_S_PRINTABLE_STRING, OM_STRING("ryan") },
    {MH_T_COUNTRY_NAME, OM_S_PRINTABLE_STRING, OM_STRING("de") },
    {MH_T_DOMAIN_TYPE_1,    OM_S_PRINTABLE_STRING, OM_STRING("internet") },
    {MH_T_DOMAIN_VALUE_1,   OM_S_PRINTABLE_STRING, OM_STRING("ryan at sni.org") },
    {MH_T_ORGANIZATION_NAME,OM_S_PRINTABLE_STRING, OM_STRING("sni") },
    {MH_T_ORGANIZATIONAL_UNIT_NAME_1,OM_S_PRINTABLE_STRING, OM_STRING("ap1") },
    {MH_T_ORGANIZATIONAL_UNIT_NAME_2,OM_S_PRINTABLE_STRING, OM_STRING("ap11") },
    {MH_T_ORGANIZATIONAL_UNIT_NAME_3,OM_S_PRINTABLE_STRING, OM_STRING("ap113")},
    {MH_T_GENERATION,   OM_S_PRINTABLE_STRING, OM_STRING("Jr") },
    {MH_T_GIVEN_NAME,   OM_S_PRINTABLE_STRING, OM_STRING("Vinnie") },
    {MH_T_INITIALS,     OM_S_PRINTABLE_STRING, OM_STRING("VR") },
    {MH_T_SURNAME,      OM_S_PRINTABLE_STRING, OM_STRING("Ryan") },
    {MH_T_PRMD_NAME,    OM_S_PRINTABLE_STRING, OM_STRING("sni") },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    or_name5[] = {
    OM_OID_DESC(OM_CLASS, MH_C_OR_NAME),
    {MH_T_DIRECTORY_NAME, OM_S_OBJECT, {0, name19} },
    OM_NULL_DESCRIPTOR
};
 
 
 
/* DL submit permission */
 
static OM_descriptor    dl_subm_perm1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DL_SUBMIT_PERMS),
    {DS_PERM_TYPE, OM_S_ENUMERATION, {DS_PERM_INDIVIDUAL, NULL} },
    {DS_INDIVIDUAL, OM_S_OBJECT, {0, or_name1} },
    OM_NULL_DESCRIPTOR
};
 
 
static OM_descriptor    dl_subm_perm3[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DL_SUBMIT_PERMS),
    {DS_PERM_TYPE, OM_S_ENUMERATION, {DS_PERM_PATTERN_MATCH, NULL} },
    {DS_PATTERN_MATCH, OM_S_OBJECT, {0, or_name4} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    dl_subm_perm4[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DL_SUBMIT_PERMS),
    {DS_PERM_TYPE, OM_S_ENUMERATION, {DS_PERM_MEMBER_OF_GROUP, NULL} },
    {DS_MEMBER_OF_GROUP, OM_S_OBJECT, {0, name8} },
    OM_NULL_DESCRIPTOR
};
 

/* SAP objects */

/* algorithm identifier */
static OM_descriptor	algo_id0[] = {
	OM_OID_DESC(OM_CLASS, DS_C_ALGORITHM_IDENT),
    {DS_ALGORITHM, OM_S_OBJECT_IDENTIFIER_STRING, OM_STRING("\x55\x08\x02\x01") },
	{ DS_ALGORITHM_PARAMETERS, OM_S_INTEGER, { 1234567, NULL } },
	OM_NULL_DESCRIPTOR
};

static OM_descriptor    algo_id1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ALGORITHM_IDENT),
    {DS_ALGORITHM, OM_S_OBJECT_IDENTIFIER_STRING, OM_STRING("\x55\x08\x01\x01") },
	{ DS_ALGORITHM_PARAMETERS, OM_S_INTEGER, { 456789, NULL } },
    OM_NULL_DESCRIPTOR
};

static OM_descriptor    algo_id2[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ALGORITHM_IDENT),
    {DS_ALGORITHM, OM_S_OBJECT_IDENTIFIER_STRING, OM_STRING("\x55\x08\x02\x01") },
    {DS_ALGORITHM_PARAMETERS, OM_S_INTEGER, {512, NULL} },
    OM_NULL_DESCRIPTOR
};

static OM_descriptor    algo_id3[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ALGORITHM_IDENT),
    {DS_ALGORITHM, OM_S_OBJECT_IDENTIFIER_STRING, OM_STRING("\x55\x08\x03\x01") },
    {DS_ALGORITHM_PARAMETERS, OM_S_INTEGER, {1024, NULL} },
    OM_NULL_DESCRIPTOR
};

static OM_descriptor    cert1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_CERT),
    {DS_ISSUER, OM_S_OBJECT, {0, name2} },
    {DS_SIGNATURE, OM_S_OBJECT, {0, algo_id1} },
    {DS_SIGNATURE_VALUE, OM_S_OCTET_STRING, OM_STRING("\x05\x04\x03\x02\x01") },
    {DS_SERIAL_NBR, OM_S_INTEGER, {11111, NULL} },
    {DS_SUBJECT, OM_S_OBJECT, {0, name92} },
    {DS_SUBJECT_ALGORITHM, OM_S_OBJECT, {0, algo_id1} },
    {DS_SUBJECT_PUBLIC_KEY, OM_S_BIT_STRING, {40,"\x01\x02\x03\x04\x05"}},
    {DS_VALIDITY_NOT_AFTER, OM_S_UTC_TIME_STRING, OM_STRING("9901011200Z") },
    {DS_VALIDITY_NOT_BEFORE, OM_S_UTC_TIME_STRING, OM_STRING("9001011200Z") },
    {DS_VERSION, OM_S_ENUMERATION, {DS_V1988, NULL} },
    OM_NULL_DESCRIPTOR
};

static OM_descriptor    cert2[] = {
    OM_OID_DESC(OM_CLASS, DS_C_CERT),
    {DS_ISSUER, OM_S_OBJECT, {0, name2} },
    {DS_SIGNATURE, OM_S_OBJECT, {0, algo_id2} },
    {DS_SIGNATURE_VALUE, OM_S_OCTET_STRING, OM_STRING("\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f\x20") },
    {DS_SERIAL_NBR, OM_S_INTEGER, {20202, NULL} },
    {DS_SUBJECT, OM_S_OBJECT, {0, name92} },
    {DS_SUBJECT_ALGORITHM, OM_S_OBJECT, {0, algo_id3} },
    {DS_SUBJECT_PUBLIC_KEY, OM_S_BIT_STRING, {64,"\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2a\x2b\x2c\x2d\x2e\x2f\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3a\x3b\x3c\x3d\x3e\x3f\x40"} },
    {DS_VALIDITY_NOT_AFTER, OM_S_UTC_TIME_STRING, OM_STRING("9901011200Z") },
    {DS_VALIDITY_NOT_BEFORE, OM_S_UTC_TIME_STRING, OM_STRING("9001011200Z") },
    {DS_VERSION, OM_S_ENUMERATION, {DS_V1988, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    cert3[] = {
    OM_OID_DESC(OM_CLASS, DS_C_CERT),
    {DS_ISSUER, OM_S_OBJECT, {0, name2} },
    {DS_SIGNATURE, OM_S_OBJECT, {0, algo_id2} },
    {DS_SIGNATURE_VALUE, OM_S_OCTET_STRING, OM_STRING("\x05\x04\x03\x02\x01") },
    {DS_SERIAL_NBR, OM_S_INTEGER, {12121, NULL} },
    {DS_SUBJECT, OM_S_OBJECT, {0, name93} },
    {DS_SUBJECT_ALGORITHM, OM_S_OBJECT, {0, algo_id1} },
    {DS_SUBJECT_PUBLIC_KEY, OM_S_BIT_STRING, {40,"\x01\x02\x03\x04\x05"}},
    {DS_VALIDITY_NOT_AFTER, OM_S_UTC_TIME_STRING, OM_STRING("9901061200Z") },
    {DS_VALIDITY_NOT_BEFORE, OM_S_UTC_TIME_STRING, OM_STRING("9001061200Z") },
    {DS_VERSION, OM_S_ENUMERATION, {DS_V1988, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    sig1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_SIGNATURE),
    {DS_ISSUER, OM_S_OBJECT, {0, name2} },
    {DS_SIGNATURE, OM_S_OBJECT, {0, algo_id1} },
    {DS_SIGNATURE_VALUE, OM_S_OCTET_STRING, OM_STRING("\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f\x20") },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    sig2[] = {
    OM_OID_DESC(OM_CLASS, DS_C_SIGNATURE),
    {DS_ISSUER, OM_S_OBJECT, {0, name2} },
    {DS_SIGNATURE, OM_S_OBJECT, {0, algo_id2} },
    {DS_SIGNATURE_VALUE, OM_S_OCTET_STRING, OM_STRING("\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f\x20") },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    cert_sublist1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_CERT_SUBLIST),
    {DS_ISSUER, OM_S_OBJECT, {0, name2} },
    {DS_SIGNATURE, OM_S_OBJECT, {0, algo_id1} },
    {DS_SIGNATURE_VALUE, OM_S_OCTET_STRING, OM_STRING("\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f\x20") },
    {DS_REVOC_DATE, OM_S_UTC_TIME_STRING, OM_STRING("9001011200Z") },
    {DS_SERIAL_NBRS, OM_S_INTEGER, {10101, NULL} },
    OM_NULL_DESCRIPTOR
};

static OM_descriptor    cert_sublist2[] = {
    OM_OID_DESC(OM_CLASS, DS_C_CERT_SUBLIST),
    {DS_ISSUER, OM_S_OBJECT, {0, name3} },
    {DS_SIGNATURE, OM_S_OBJECT, {0, algo_id3} },
    {DS_SIGNATURE_VALUE, OM_S_OCTET_STRING, OM_STRING("\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f\x20") },
    {DS_REVOC_DATE, OM_S_UTC_TIME_STRING, OM_STRING("9101011200Z") },
    {DS_REVOC_DATE, OM_S_UTC_TIME_STRING, OM_STRING("9201011200Z") },
    {DS_REVOC_DATE, OM_S_UTC_TIME_STRING, OM_STRING("9301011200Z") },
    {DS_REVOC_DATE, OM_S_UTC_TIME_STRING, OM_STRING("9401011200Z") },
    {DS_REVOC_DATE, OM_S_UTC_TIME_STRING, OM_STRING("9501011200Z") },
    {DS_REVOC_DATE, OM_S_UTC_TIME_STRING, OM_STRING("9601011200Z") },
    {DS_REVOC_DATE, OM_S_UTC_TIME_STRING, OM_STRING("9701011200Z") },
    {DS_REVOC_DATE, OM_S_UTC_TIME_STRING, OM_STRING("9801011200Z") },
    {DS_REVOC_DATE, OM_S_UTC_TIME_STRING, OM_STRING("9901011200Z") },
    {DS_SERIAL_NBRS, OM_S_INTEGER, {11111, NULL} },
    {DS_SERIAL_NBRS, OM_S_INTEGER, {22222, NULL} },
    {DS_SERIAL_NBRS, OM_S_INTEGER, {33333, NULL} },
    {DS_SERIAL_NBRS, OM_S_INTEGER, {44444, NULL} },
    {DS_SERIAL_NBRS, OM_S_INTEGER, {55555, NULL} },
    {DS_SERIAL_NBRS, OM_S_INTEGER, {66666, NULL} },
    {DS_SERIAL_NBRS, OM_S_INTEGER, {77777, NULL} },
    {DS_SERIAL_NBRS, OM_S_INTEGER, {88888, NULL} },
    {DS_SERIAL_NBRS, OM_S_INTEGER, {99999, NULL} },
    OM_NULL_DESCRIPTOR
};

static OM_descriptor    cert_list1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_CERT_LIST),
    {DS_ISSUER, OM_S_OBJECT, {0, name2} },
    {DS_SIGNATURE, OM_S_OBJECT, {0, algo_id1} },
    {DS_SIGNATURE_VALUE, OM_S_OCTET_STRING, OM_STRING("\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f\x20") },
    {DS_LAST_UPDATE, OM_S_UTC_TIME_STRING, OM_STRING("9001011200Z") },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    cert_list2[] = {
    OM_OID_DESC(OM_CLASS, DS_C_CERT_LIST),
    {DS_ISSUER, OM_S_OBJECT, {0, name2} },
    {DS_SIGNATURE, OM_S_OBJECT, {0, algo_id2} },
    {DS_SIGNATURE_VALUE, OM_S_OCTET_STRING, OM_STRING("\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f\x20") },
    {DS_LAST_UPDATE, OM_S_UTC_TIME_STRING, OM_STRING("9901011200Z") },
    {DS_REVOKED_CERTS, OM_S_OBJECT, {0, cert_sublist1} },
    {DS_REVOKED_CERTS, OM_S_OBJECT, {0, cert_sublist2} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    cert_list3[] = {
    OM_OID_DESC(OM_CLASS, DS_C_CERT_LIST),
    {DS_ISSUER, OM_S_OBJECT, {0, name2} },
    {DS_SIGNATURE, OM_S_OBJECT, {0, algo_id2} },
    {DS_SIGNATURE_VALUE, OM_S_OCTET_STRING, OM_STRING("\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f\x20") },
    {DS_LAST_UPDATE, OM_S_UTC_TIME_STRING, OM_STRING("9901011200Z") },
    {DS_REVOKED_CERTS, OM_S_OBJECT, {0, cert_sublist2} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    cert_pair1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_CERT_PAIR),
    {DS_FORWARD, OM_S_OBJECT, {0, cert1} },
    OM_NULL_DESCRIPTOR
};

static OM_descriptor    cert_pair2[] = {
    OM_OID_DESC(OM_CLASS, DS_C_CERT_PAIR),
    {DS_REVERSE, OM_S_OBJECT, {0, cert2} },
    OM_NULL_DESCRIPTOR
};


 
 
 
/* attributes */
 
static OM_descriptor    att1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_PHONE_NBR),
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("+49 89 636 0")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att2[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_PHONE_NBR),
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("+353 1 767551")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att3[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_PHONE_NBR),
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("+353 1 767551 x 401")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att4[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_PHONE_NBR),
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("+353 1 767551 x 454")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att5[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_PHONE_NBR),
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("+49 89 636 46713")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att6[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_BUSINESS_CATEGORY),
    {DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("computer systems")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att7[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_TITLE),
    {DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("Software Engineer")},
    OM_NULL_DESCRIPTOR
};
 
 
static OM_descriptor    att9[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_ALIASED_OBJECT_NAME),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, name8}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att10[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OBJECT_CLASS),
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att11[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DSX_A_MASTER_KNOWLEDGE),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, name17 }},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att12[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OBJECT_CLASS),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_COUNTRY),
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att13[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OBJECT_CLASS),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_ORG),
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att14[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OBJECT_CLASS),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_ORG_UNIT),
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att15[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OBJECT_CLASS),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_ORG_PERSON),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_PERSON),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_TOP),
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att16[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OBJECT_CLASS),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_ALIAS),
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att17[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_USER_PASSWORD),
    {DS_ATTRIBUTE_VALUES, OM_S_OCTET_STRING, OM_STRING("secret")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att18[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OBJECT_CLASS),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_ORG_ROLE),
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att19[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OBJECT_CLASS),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_DEVICE),
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att20[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OBJECT_CLASS),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_LOCALITY),
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att21[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OBJECT_CLASS),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_GROUP_OF_NAMES),
    OM_NULL_DESCRIPTOR
};
 
 
static OM_descriptor    att23[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OBJECT_CLASS),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_RESIDENTIAL_PERSON),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_PERSON),
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att24[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_DESCRIPTION),
    {DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("Database Administrator")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att25[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_ROLE_OCCUPANT),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, name8}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att26[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OWNER),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, name8}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att27[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_SERIAL_NBR),
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("M0001")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att28[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_DESCRIPTION),
    {DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("XDS SIG")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att29[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_MEMBER),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, name8}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att30[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_MEMBER),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, name5}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att31[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_LOCALITY_NAME),
    {DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("Dublin")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att32[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_PHONE_NBR),
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("123456789012345678901234567890123")},
    OM_NULL_DESCRIPTOR
};
 
 
static OM_descriptor    att34[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_ALIASED_OBJECT_NAME),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, name4}},
    OM_NULL_DESCRIPTOR
};
 
 
/* ACL in string format:
 
     interpretation (single object)
     |
     | disting.name   D2_IGNORE
     | |          |
     v v              v
     0 dist-name NULL 3 NULL     <- modify public
     0 dist-name NULL 3 NULL     <- standard
     0 dist-name NULL 3 NULL     <- modify standard
     0 dist-name NULL 3 NULL     <- read sensitive
     0 dist-name NULL 3 NULL     <- modify sensitive
 
static OM_descriptor    att35[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DSX_A_ACL),
    {DS_ATTRIBUTE_VALUES, DSX_S_ACL, 
    OM_STRING("085.4.6ie85.4.10sni85.4.11sci85.4.3vinnie\x00"
              "\x33\x00\x33\x00\x33\x00"
              "085.4.6ie85.4.10sni85.4.11sci85.4.3vinnie\x00"
              "\x33\x00\x33\x00\x33\x00"
              "085.4.6ie85.4.10sni85.4.11sci85.4.3vinnie\x00"
              "\x33\x00\x33\x00\x33\x00"
              "085.4.6ie85.4.10sni85.4.11sci85.4.3vinnie\x00"
              "\x33\x00\x33\x00\x33\x00"
              "085.4.6ie85.4.10sni85.4.11sci85.4.3vinnie\x00"
              "\x33\x00\x33\x00\x33\x00") },
    OM_NULL_DESCRIPTOR
};
*/
 
static OM_descriptor    acl_item0[] = {
    OM_OID_DESC(OM_CLASS, DSX_C_GDS_ACL_ITEM),
    {DSX_INTERPRETATION, OM_S_ENUMERATION, {DSX_ROOT_OF_SUBTREE, 0}},
    {DSX_USER, OM_S_OBJECT, {0, top_name}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    acl_item1[] = {
    OM_OID_DESC(OM_CLASS, DSX_C_GDS_ACL_ITEM),
    {DSX_INTERPRETATION, OM_S_ENUMERATION, {DSX_SINGLE_OBJECT, 0}},
    {DSX_USER, OM_S_OBJECT, {0, name8}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    acl_item2[] = {
    OM_OID_DESC(OM_CLASS, DSX_C_GDS_ACL_ITEM),
    {DSX_INTERPRETATION, OM_S_ENUMERATION, {DSX_SINGLE_OBJECT, 0}},
    {DSX_USER, OM_S_OBJECT, {0, name7}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    acl_item3[] = {
    OM_OID_DESC(OM_CLASS, DSX_C_GDS_ACL_ITEM),
    {DSX_INTERPRETATION, OM_S_ENUMERATION, {DSX_SINGLE_OBJECT, 0}},
    {DSX_USER, OM_S_OBJECT, {0, name20}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    acl0[] = {
    OM_OID_DESC(OM_CLASS, DSX_C_GDS_ACL),
    {DSX_MODIFY_PUBLIC, OM_S_OBJECT, {0, acl_item0}},
    {DSX_READ_STANDARD, OM_S_OBJECT, {0, acl_item0}},
    {DSX_MODIFY_STANDARD, OM_S_OBJECT, {0, acl_item0}},
    {DSX_READ_SENSITIVE, OM_S_OBJECT, {0, acl_item0}},
    {DSX_MODIFY_SENSITIVE, OM_S_OBJECT, {0, acl_item0}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    acl1[] = {
    OM_OID_DESC(OM_CLASS, DSX_C_GDS_ACL),
    {DSX_MODIFY_PUBLIC, OM_S_OBJECT, {0, acl_item1}},
    {DSX_READ_STANDARD, OM_S_OBJECT, {0, acl_item1}},
    {DSX_MODIFY_STANDARD, OM_S_OBJECT, {0, acl_item1}},
    {DSX_READ_SENSITIVE, OM_S_OBJECT, {0, acl_item1}},
    {DSX_MODIFY_SENSITIVE, OM_S_OBJECT, {0, acl_item1}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    acl2[] = {
    OM_OID_DESC(OM_CLASS, DSX_C_GDS_ACL),
    {DSX_MODIFY_PUBLIC, OM_S_OBJECT, {0, acl_item1}},
    {DSX_MODIFY_PUBLIC, OM_S_OBJECT, {0, acl_item2}},
    {DSX_MODIFY_PUBLIC, OM_S_OBJECT, {0, acl_item3}},
    {DSX_READ_STANDARD, OM_S_OBJECT, {0, acl_item1}},
    {DSX_READ_STANDARD, OM_S_OBJECT, {0, acl_item2}},
    {DSX_READ_STANDARD, OM_S_OBJECT, {0, acl_item3}},
    {DSX_MODIFY_STANDARD, OM_S_OBJECT, {0, acl_item1}},
    {DSX_MODIFY_STANDARD, OM_S_OBJECT, {0, acl_item2}},
    {DSX_MODIFY_STANDARD, OM_S_OBJECT, {0, acl_item3}},
    {DSX_READ_SENSITIVE, OM_S_OBJECT, {0, acl_item3}},
    {DSX_MODIFY_SENSITIVE, OM_S_OBJECT, {0, acl_item3}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att35[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DSX_A_ACL),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, acl1}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att36[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OBJECT_CLASS),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_APPLIC_PROCESS),
    OM_NULL_DESCRIPTOR
};
 
 
static OM_descriptor    att38[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OBJECT_CLASS),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_DSA),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_APPLIC_ENTITY),
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att39[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_PRESENTATION_ADDRESS),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, address1}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att40[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DSX_A_ACL),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, acl2}},
    OM_NULL_DESCRIPTOR
};
 
/* static OM_descriptor    att41[]; */  /* unused */
 
/* invalid syntax/value for telephone-number */
static OM_descriptor    att42[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_PHONE_NBR),
    {DS_ATTRIBUTE_VALUES, OM_S_OCTET_STRING, OM_STRING("\x02\x03\x04")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att43[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_FACSIMILE_PHONE_NBR),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, fax_number1}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att44[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_POSTAL_ADDRESS),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, postal_address1}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att45[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_SEARCH_GUIDE),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, search_guide1}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att46[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_TELETEX_TERM_IDENT),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, teletex_terminal_id1}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att47[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_TELEX_NBR),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, telex_number1}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att48[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_DESCRIPTION),
    {DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("Abteilungssekret\xC8\x61\x72")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att49[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_ROLE_OCCUPANT),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, name49}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att50[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DSX_A_CDS_CTS),
    {DS_ATTRIBUTE_VALUES, OM_S_OCTET_STRING, OM_STRING("\x02\x03\x04")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att51[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DSX_A_CDS_Address),
    {DS_ATTRIBUTE_VALUES, OM_S_OCTET_STRING, OM_STRING("\x04\x08\x02")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att52[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DSX_A_CDS_AllUpTo),
    {DS_ATTRIBUTE_VALUES, OM_S_OCTET_STRING, OM_STRING("\x06\x02")},
    OM_NULL_DESCRIPTOR
};
 
 
static OM_descriptor    att54[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_ALIASED_OBJECT_NAME),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, name61}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att55[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_ALIASED_OBJECT_NAME),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, name52}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att56[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_ALIASED_OBJECT_NAME),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, name63}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att57[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DSX_A_CDS_Address),
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att58[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_MEMBER),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, name8}},
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, name5}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att59[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_PHONE_NBR),
    {DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("+353 1 767551 x346")},
    {DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("+49 89 636 46713")},
    OM_NULL_DESCRIPTOR
};
 
/* MHS attributes */
 
static OM_descriptor    att60[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_DELIV_CONTENT_LENGTH),
    {DS_ATTRIBUTE_VALUES, OM_S_INTEGER, { 2048, NULL } },
    OM_NULL_DESCRIPTOR
};
 
/* content types - MH_CTO_INNER_MESSAGE, MH_CTO_UNIDENTIFIED              */
static OM_descriptor    att61[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_DELIV_CONTENT_TYPES),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, MH_CTO_INNER_MESSAGE),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, MH_CTO_UNIDENTIFIED),
    OM_NULL_DESCRIPTOR
};
 
/* external EITs - MH_EE_G3_FAX, MH_EE_G4_CLASS_1, MH_EE_IA5_TEXT,
                   MH_EE_MIXED_MODE, MH_EE_TELETEX, MH_EE_TELEX,
                   MH_EE_UNDEFINED, MH_EE_VIDEOTEX                        */
static OM_descriptor    att62[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_DELIV_EITS),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, MH_EE_G3_FAX),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, MH_EE_TELEX),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, MH_EE_TELETEX),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, MH_EE_IA5_TEXT),
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att63[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_DL_MEMBERS),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, or_name2}},
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, or_name3}},
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, or_name4}},
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, or_name5}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att64[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_DL_SUBMIT_PERMS),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, dl_subm_perm1}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att65[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_MESSAGE_STORE),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, name74}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att66[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OR_ADDRESSES),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, or_addr2} },
    OM_NULL_DESCRIPTOR
};
 
 
/* delivery mode - MH_DM_ANY, MH_DM_MTS, MH_DM_PDS, MH_DM_TELEX,
                   MH_DM_TELETEX, MH_DM_G3_FAX, MH_DM_G4_FAX, 
                   MH_DM_IA5_TERMINAL, MH_DM_VIDEOTEX, MH_DM_TELEPHONE    */
static OM_descriptor    att67[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_PREF_DELIV_METHODS),
    {DS_ATTRIBUTE_VALUES, OM_S_ENUMERATION, {MH_DM_MTS, NULL} },
/*{DS_ATTRIBUTE_VALUES, DSX_S_INTEGER_LIST, {sizeof(dm_mts),(void *)dm_mts} },*/
    OM_NULL_DESCRIPTOR
};
 
/* automatic actions - auto-forward, auto-alert */
static OM_descriptor    att68[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_SUPP_AUTO_ACTIONS),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT_IDENTIFIER_STRING, {4,"\x56\x4\x4\x0"}},
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT_IDENTIFIER_STRING, {4,"\x56\x4\x4\x1"}},
    OM_NULL_DESCRIPTOR
};
 
/* content types */
static OM_descriptor    att69[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_SUPP_CONTENT_TYPES),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, MH_CTO_INNER_MESSAGE),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, MH_CTO_UNIDENTIFIED),
    OM_NULL_DESCRIPTOR
};
 
/* optional attributes */
static OM_descriptor    att70[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_SUPP_OPT_ATTRIBUTES),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, MS_A_CONTENT),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, MS_A_CONTENT_CONFIDENTL_ALGM_ID),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, MS_A_CONTENT_CORRELATOR),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, MS_A_CONTENT_IDENTIFIER),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, MS_A_CONTENT_INTEGRITY_CHECK),
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att71[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OBJECT_CLASS),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_MHS_DISTRIBUTION_LIST),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_TOP),
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att72[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OBJECT_CLASS),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_MHS_MESSAGE_STORE),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_APPLIC_ENTITY),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_TOP),
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att73[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OBJECT_CLASS),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_MHS_MESSAGE_TRANS_AG),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_APPLIC_ENTITY),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_TOP),
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att74[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OBJECT_CLASS),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_MHS_USER),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_ORG_PERSON),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_PERSON),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_TOP),
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att75[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OBJECT_CLASS),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_MHS_USER_AG),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_APPLIC_ENTITY),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_TOP),
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att76[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_DESCRIPTION),
    {DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("SNI Distribution List")}, 
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att77[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_DESCRIPTION),
    {DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("SNI Message Store")}, 
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att78[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_DESCRIPTION),
    {DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("SNI Message Transfer Agent")}, 
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att79[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OWNER),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, name77}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att80[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OR_ADDRESSES),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, or_addr3} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att81[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OR_ADDRESSES),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, or_addr4} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att82[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OR_ADDRESSES),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, or_addr5} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att83[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OR_ADDRESSES),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, or_addr6} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att84[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_DL_SUBMIT_PERMS),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, dl_subm_perm3}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att85[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_DL_SUBMIT_PERMS),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, dl_subm_perm4}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att86[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OR_ADDRESSES),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, or_addr7} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att87[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OR_ADDRESSES),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, or_addr8} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att88[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OR_ADDRESSES),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, or_addr9} },
    OM_NULL_DESCRIPTOR
};
 
 
static OM_descriptor    att90[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_PHONE_NBR),
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("+353 1 767441 x401")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att91[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_PHONE_NBR),
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("+353 1 767441 x401")},
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("+353 1 767441 x999")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att92[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_PHONE_NBR),
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("+353 1 767551")},
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("+49 89 636 41094")},
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("+353 1 772222")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att93[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DSX_A_ACL),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, acl0}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att94[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OBJECT_CLASS),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DSX_O_DME_NMO_AGENT),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_APPLIC_ENTITY),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_TOP),
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att95[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DSX_A_ALTERNATE_ADDRESS),
    {DS_ATTRIBUTE_VALUES, OM_S_OCTET_STRING, OM_STRING("\x08\x09\x0A\x0B\x0C")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att96[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OBJECT_CLASS),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_STRONG_AUTHENT_USER),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_ORG_PERSON),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_PERSON),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_TOP),
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att97[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OBJECT_CLASS),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_CERT_AUTHORITY),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_ORG_PERSON),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_PERSON),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_TOP),
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att98[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_USER_CERT),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, cert1}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att100[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_USER_CERT),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, cert2}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att101[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_CA_CERT),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, cert3}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att102[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_CERT_REVOC_LIST),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, cert_list2}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att103[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_AUTHORITY_REVOC_LIST),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, cert_list1}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att104[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_CROSS_CERT_PAIR),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, cert_pair2}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att105[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_SURNAME),
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att106[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_COMMON_NAME),
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att107[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_USER_CERT),
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att108[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_ORG_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING, OM_STRING("ca")},
    OM_NULL_DESCRIPTOR
};
 

/* attribute list */
 
static OM_descriptor    att_list0[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att6} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att13} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list2[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att1} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att14} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list3[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att2} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att14} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list4[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname1} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, user_pswd1} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list5[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att4} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att7} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname2} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list6[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att5} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname3} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, user_pswd1} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list7[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att16} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att9} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list8[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att12} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list9[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att13} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list10[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att14} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list11[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list12[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att16} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list13[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att17} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list14[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att18} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att24} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att25} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list15[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att19} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att26} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att27} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list16[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att20} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list17[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att21} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att28} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att29} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att30} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list18[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att23} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att31} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname2} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, user_pswd1} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list19[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att23} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att31} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname4} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, user_pswd1} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list20[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname1} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att42} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list21[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname1} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, undefined1} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list22[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att14} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att32} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list23[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att14} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname1} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list24[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname1} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, user_pswd4} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list25[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att16} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att34} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list26[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att36} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list27[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att38} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att39} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list28[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att12} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att35} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list29[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att13} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att43} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att44} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att45} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att46} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att47} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list30[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att13} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att43} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list31[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att13} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att44} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list32[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att13} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att45} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list33[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att13} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att46} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list34[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att13} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att47} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list35[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att18} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att48} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att49} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list36[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att16} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att54} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list37[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att50} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list38[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att51} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list39[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att2} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname1} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, user_pswd2} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att24} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list40[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att16} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att55} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list41[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att16} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att56} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list42[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname1} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att51} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list43[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att52} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list44[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att59} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list45[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att12} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att40} },
    OM_NULL_DESCRIPTOR
};
 
 
/* MHS attribute list */
 
static OM_descriptor    att_list46[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att71} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att64} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att66} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list47[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att71} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att64} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att66} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att61} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att62} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att63} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att67} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att76} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att79} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list48[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att71} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att64} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att66} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att61} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att62} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att63} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att67} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att76} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att79} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att84} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att80} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list49[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att71} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att64} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att66} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att61} }, 
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att62} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att63} },
/* TBD    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att67} }, */
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att76} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att79} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att84} },  
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att80} }, 
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att81} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att82} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att83} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att85} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list50[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att72} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att70} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att69} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att68} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att77} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att79} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att39} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list51[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att73} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att60} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att78} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att79} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att39} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list52[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att74} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname4} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att66} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list53[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att74} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname4} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att66} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att60} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att61} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att62} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att65} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att67} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list54[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att75} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att60} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att61} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att62} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att66} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att39} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list55[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att74} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname4} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att86} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list56[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att74} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname4} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att87} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list57[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att74} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname4} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att88} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list58[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att71} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att66} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list59[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att74} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname4} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att66} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att60} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att61} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att62} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att65} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att67} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att64} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list60[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att94} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att95} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att39} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list61[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att94} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att39} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list62[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att96} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att98} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname8} },
    OM_NULL_DESCRIPTOR
};
 
 
static OM_descriptor    att_list63[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att96} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att98} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname8} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att100} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list64[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname8} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att97} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att101} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att102} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att103} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list65[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname8} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att97} },
	{DS_ATTRIBUTES, OM_S_OBJECT, {0, att101} },
	{DS_ATTRIBUTES, OM_S_OBJECT, {0, att102} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att103} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att104} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list66[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname8} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att97} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att101} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list67[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname8} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att97} },
	{DS_ATTRIBUTES, OM_S_OBJECT, {0, att102} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list68[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname8} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att97} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att103} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_list69[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname8} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att97} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att104} },
    OM_NULL_DESCRIPTOR
};
 
/* entry information */
 
static OM_descriptor entry_info1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name6} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att3} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att7} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname1} },
    OM_NULL_DESCRIPTOR
};
 
 
static OM_descriptor entry_info3[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name5} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att5} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname3} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, user_pswd1} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info4[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name1} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info5[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name1} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att11} },
    OM_NULL_DESCRIPTOR
};
 
 
static OM_descriptor entry_info9[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name1} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att10} },
/* op. attrs not returned
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att33} },*/
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info10[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name1} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att12} },
/* op. attrs not returned
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att11} },*/
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info11[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name1} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att12} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info12[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name8} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname1} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, user_pswd1} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info13[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name19} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att16} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att9} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info14[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name2} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att13} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info15[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name4} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att14} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info16[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name32} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att16} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info17[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name2} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att13} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att6} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info18[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name4} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att14} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att2} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info19[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name32} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att16} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att34} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info20[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name22} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att20} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info21[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name20} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att18} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att24} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att25} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info22[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name25} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att23} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att31} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname4} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, user_pswd1} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info23[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name24} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att23} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname2} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, user_pswd1} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info24[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name8} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname1} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info25[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name8} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info26[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name8} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info27[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name8} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname1} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, user_pswd1} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info28[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name8} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname1} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, user_pswd1} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att11} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info29[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name43} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att12} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info30[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name46} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att38} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info31[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name2} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att13} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att43} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att44} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att45} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att46} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att47} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info32[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name50} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att18} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att48} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att49} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info33[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name52} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att51} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info34[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name61} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att51} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info35[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name62} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att16} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att54} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info36[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name60} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname1} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, user_pswd1} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info37[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name60} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att10} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname6} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att57} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info38[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name52} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att21} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att28} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att58} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info39[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name60} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info40[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name60} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att10} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname6} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info41[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name60} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname1} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info42[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name56} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att59} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info43[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name65} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att24} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info44[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name66} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname1} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att51} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info45[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name17} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att38} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info46[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name46} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att38} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info47[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name8} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname1} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info48[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name71} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname1} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, user_pswd1} },
    OM_NULL_DESCRIPTOR
};
 
/* UNUSED?
 
static OM_descriptor entry_info49[] = { 
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name68} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname1} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, user_pswd1} },
    OM_NULL_DESCRIPTOR
};
*/
 
 
/* MHS entry information */
 
 
static OM_descriptor entry_info49[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name73} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att71} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att64} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att66} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info50[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name73} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att71} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att64} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att66} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att61} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att62} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att63} },
/* TBD   {DS_ATTRIBUTES, OM_S_OBJECT, {0, att67} }, */
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att76} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att79} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att84} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att80} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att81} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att82} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att83} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att85} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info51[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name74} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att72} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att70} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att69} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att68} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att77} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att79} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att39} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info52[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name75} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att73} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att60} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att78} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att79} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att39} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info53[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name78} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname4} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att66} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info54[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name76} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att75} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att60} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att61} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att62} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att66} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att39} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info55[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name8} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname4} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att66} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info56[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name7} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname4} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att66} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info57[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name16} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname4} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att66} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info58[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name18} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname4} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att66} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info59[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name19} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname4} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att66} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info60[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name20} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname4} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att66} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info61[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name49} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname4} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att66} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info62[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name50} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname4} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att66} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info63[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name77} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname4} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att66} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info64[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name78} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname4} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att66} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info65[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname4} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att81} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att60} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att61} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att62} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info66[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name78} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname4} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att66} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info67[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name8} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname1} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, user_pswd1} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att90} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info68[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name8} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname1} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, user_pswd1} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att91} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info69[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name8} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname1} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att92} },
    OM_NULL_DESCRIPTOR
};

static OM_descriptor entry_info70[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name39} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname1} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, user_pswd1} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info71[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name39} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname1} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, user_pswd1} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, c_name14} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info72[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name8} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, user_pswd1} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname1} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, c_name3} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info73[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name8} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att16} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att9} },
    OM_NULL_DESCRIPTOR
};

static OM_descriptor entry_info74[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name8} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname1} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, user_pswd1} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att35} },
    OM_NULL_DESCRIPTOR
};

static OM_descriptor entry_info75[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name8} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att15} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname1} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, user_pswd1} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att93} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info76[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name25} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att23} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att31} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname4} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, user_pswd1} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att90} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info77[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name24} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att23} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att31} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname2} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, user_pswd1} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info78[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name25} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att23} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att31} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname2} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, user_pswd1} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info79[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name46} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att38} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info80[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name87} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att39} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att94} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att95} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info81[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name92} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att10} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att105} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att106} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att107} },
    OM_NULL_DESCRIPTOR
};

static OM_descriptor entry_info82[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name8} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname8} },
	/*
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, c_name3} },
	*/
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att97} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att101} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att102} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att103} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att104} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info83[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name8} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att98} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info84[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name8} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att98} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor entry_info85[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name92} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att98} },
    OM_NULL_DESCRIPTOR
};

static OM_descriptor entry_info86[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name8} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname8} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att97} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att101} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att102} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att103} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att104} },
    OM_NULL_DESCRIPTOR
};
static OM_descriptor entry_info87[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name8} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname8} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att97} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att101} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att102} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att103} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att104} },
    OM_NULL_DESCRIPTOR
};
static OM_descriptor entry_info88[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name8} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname8} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att97} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att101} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att102} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att103} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att104} },
    OM_NULL_DESCRIPTOR
};
static OM_descriptor entry_info89[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name8} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname8} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att97} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att101} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att102} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att103} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att104} },
    OM_NULL_DESCRIPTOR
};
static OM_descriptor entry_info90[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO),
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name8} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname8} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att97} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att101} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att102} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att103} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att104} },
    OM_NULL_DESCRIPTOR
};
 
 
 
 
 
 

/* extension */
 
static OM_descriptor    extension1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_EXT),
    {DS_CRIT, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_IDENT, OM_S_INTEGER, {1, NULL} },
    {DS_ITEM_PARAMETERS, OM_S_PRINTABLE_STRING, OM_STRING("p1=3, p2=2, p3=1")},
    OM_NULL_DESCRIPTOR
};
 
 
/* operation_progress */
 
static OM_descriptor    operation_prog1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_OPERATION_PROGRESS),
    {DS_NAME_RESOLUTION_PHASE, OM_S_ENUMERATION, {DS_COMPLETED, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    operation_prog2[] = {
    OM_OID_DESC(OM_CLASS, DS_C_OPERATION_PROGRESS),
    {DS_NAME_RESOLUTION_PHASE, OM_S_ENUMERATION, {DS_PROCEEDING, NULL} },
    {DS_NEXT_RDN_TO_BE_RESOLVED, OM_S_INTEGER, {5, NULL} },
    OM_NULL_DESCRIPTOR
};
    
static OM_descriptor    operation_prog3[] = {
    OM_OID_DESC(OM_CLASS, DS_C_OPERATION_PROGRESS),
    {DS_NAME_RESOLUTION_PHASE, OM_S_ENUMERATION, {DS_NOT_STARTED, NULL} },
    OM_NULL_DESCRIPTOR
};
    
 
/* session */
 
/*
static OM_descriptor session0[] = {
    {DS_DEFAULT_SESSION}
};
*/
 
static OM_descriptor session1[] = {
    OM_OID_DESC(OM_CLASS, DSX_C_GDS_SESSION),
    {DS_DSA_NAME, OM_S_OBJECT, {0, name17 }},
    {DSX_DIR_ID, OM_S_INTEGER, {1, NULL} },
    {DSX_AUTH_MECHANISM, OM_S_ENUMERATION, {DSX_NONE_AT_ALL, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor session2[] = {
    OM_OID_DESC(OM_CLASS, DSX_C_GDS_SESSION),
    {DS_DSA_ADDRESS, OM_S_OBJECT, {0, address1} },
    {DS_DSA_NAME, OM_S_OBJECT, {0, name17 }},
    {DS_REQUESTOR, OM_S_OBJECT, {0, name8} },
    {DSX_PASSWORD, OM_S_OCTET_STRING, OM_STRING("secret")},
    {DSX_DIR_ID, OM_S_INTEGER, {1, NULL} },
    {DSX_AUTH_MECHANISM, OM_S_ENUMERATION, {DSX_SIMPLE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor session3[] = {
    OM_OID_DESC(OM_CLASS, DSX_C_GDS_SESSION),
    {DS_DSA_ADDRESS, OM_S_OBJECT, {0, address1} },
    {DS_DSA_NAME, OM_S_OBJECT, {0, name17 }},
    {DS_REQUESTOR, OM_S_OBJECT, {0, name18} },
    {DSX_PASSWORD, OM_S_OCTET_STRING, OM_STRING("secret")},
    {DSX_DIR_ID, OM_S_INTEGER, {1, NULL} },
    {DSX_AUTH_MECHANISM, OM_S_ENUMERATION, {DSX_SIMPLE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor session4[] = {
    OM_OID_DESC(OM_CLASS, DSX_C_GDS_SESSION),
    {DS_DSA_NAME, OM_S_OBJECT, {0, name46 }},
    {DSX_DIR_ID, OM_S_INTEGER, {1, NULL} },
    {DSX_AUTH_MECHANISM, OM_S_ENUMERATION, {DSX_NONE_AT_ALL, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor session5[] = {
    OM_OID_DESC(OM_CLASS, DSX_C_GDS_SESSION),
    {DS_DSA_ADDRESS, OM_S_OBJECT, {0, address5} },
    {DS_DSA_NAME, OM_S_OBJECT, {0, dsa3_name }},
    {DSX_DIR_ID, OM_S_INTEGER, {3, NULL} },
    {DSX_AUTH_MECHANISM, OM_S_ENUMERATION, {DSX_NONE_AT_ALL, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor session6[] = {
    OM_OID_DESC(OM_CLASS, DSX_C_GDS_SESSION),
    {DS_DSA_ADDRESS, OM_S_OBJECT, {0, address1} },
    {DS_DSA_NAME, OM_S_OBJECT, {0, name17 }},
    {DS_REQUESTOR, OM_S_OBJECT, {0, name8} },
    {DSX_PASSWORD, OM_S_OCTET_STRING, OM_STRING("secret")},
    {DSX_DIR_ID, OM_S_INTEGER, {1, NULL} },
    {DSX_AUTH_MECHANISM, OM_S_ENUMERATION, {DSX_SIMPLE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor session7[] = {
    OM_OID_DESC(OM_CLASS, DSX_C_GDS_SESSION),
    {DS_DSA_NAME, OM_S_OBJECT, {0, name20 }},
    {DSX_DIR_ID, OM_S_INTEGER, {1234, NULL} },
    {DSX_AUTH_MECHANISM, OM_S_ENUMERATION, {DSX_NONE_AT_ALL, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor session8[] = {
    OM_OID_DESC(OM_CLASS, DSX_C_GDS_SESSION),
    {DSX_DIR_ID, OM_S_INTEGER, {1, NULL} },
    {DSX_AUTH_MECHANISM, OM_S_ENUMERATION, {DSX_SIMPLE, NULL} },
    OM_NULL_DESCRIPTOR
};

static OM_descriptor session9[] = {
    OM_OID_DESC(OM_CLASS, DSX_C_GDS_SESSION),
    {DS_DSA_NAME, OM_S_OBJECT, {0, name89 }},
    {DSX_DIR_ID, OM_S_INTEGER, {7, NULL} },
    {DSX_AUTH_MECHANISM, OM_S_ENUMERATION, {DSX_NONE_AT_ALL, NULL} },
    OM_NULL_DESCRIPTOR
};
/* context */
 
 
static OM_descriptor context1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_CONTEXT),
    {DS_EXT, OM_S_OBJECT, {0, extension1} },
    {DS_OPERATION_PROGRESS, OM_S_OBJECT, {0, operation_prog3} },
    {DS_CHAINING_PROHIB, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_DONT_DEREFERENCE_ALIASES, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_DONT_USE_COPY, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_LOCAL_SCOPE, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_PREFER_CHAINING, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_PRIORITY, OM_S_ENUMERATION, {DS_MEDIUM, NULL} },
    {DS_SCOPE_OF_REFERRAL, OM_S_ENUMERATION, {DS_COUNTRY, NULL} },
    {DS_SIZE_LIMIT, OM_S_INTEGER, {(OM_uint32)-1, NULL} },
    {DS_TIME_LIMIT, OM_S_INTEGER, {(OM_uint32)-1, NULL} },
    {DS_ASYNCHRONOUS, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_AUTOMATIC_CONTINUATION, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor context_mod1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_CONTEXT),
    {DSX_DUA_CACHE, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DSX_USEDSA, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor context_mod2[] = {
    OM_OID_DESC(OM_CLASS, DS_C_CONTEXT),
    {DS_CHAINING_PROHIB, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor context_mod3[] = {
    OM_OID_DESC(OM_CLASS, DS_C_CONTEXT),
    {DS_DONT_DEREFERENCE_ALIASES, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor context_mod4[] = {
    OM_OID_DESC(OM_CLASS, DS_C_CONTEXT),
    {DS_DONT_USE_COPY, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor context_mod5[] = {
    OM_OID_DESC(OM_CLASS, DS_C_CONTEXT),
    {DS_LOCAL_SCOPE, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor context_mod6[] = {
    OM_OID_DESC(OM_CLASS, DS_C_CONTEXT),
    {DS_PREFER_CHAINING, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor context_mod7[] = {
    OM_OID_DESC(OM_CLASS, DS_C_CONTEXT),
    {DS_PRIORITY, OM_S_ENUMERATION, {DS_LOW, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor context_mod8[] = {
    OM_OID_DESC(OM_CLASS, DS_C_CONTEXT),
    {DS_PRIORITY, OM_S_ENUMERATION, {DS_MEDIUM, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor context_mod9[] = {
    OM_OID_DESC(OM_CLASS, DS_C_CONTEXT),
    {DS_PRIORITY, OM_S_ENUMERATION, {DS_HIGH, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor context_mod10[] = {
    OM_OID_DESC(OM_CLASS, DS_C_CONTEXT),
    {DS_SCOPE_OF_REFERRAL, OM_S_ENUMERATION, {DS_COUNTRY, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor context_mod11[] = {
    OM_OID_DESC(OM_CLASS, DS_C_CONTEXT),
    {DS_SCOPE_OF_REFERRAL, OM_S_ENUMERATION, {DS_DMD, NULL} },
    OM_NULL_DESCRIPTOR
};
 
/* unused */
static OM_descriptor context_mod12[] = {
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor context_mod13[] = {
    OM_OID_DESC(OM_CLASS, DS_C_CONTEXT),
    {DSX_DONT_STORE, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DSX_NORMAL_CLASS, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor context_mod14[] = {
    OM_OID_DESC(OM_CLASS, DS_C_CONTEXT),
    {DSX_DONT_STORE, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DSX_PRIV_CLASS, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor context_mod15[] = {
    OM_OID_DESC(OM_CLASS, DS_C_CONTEXT),
    {DSX_DONT_STORE, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DSX_RESIDENT_CLASS, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor context_mod16[] = {
    OM_OID_DESC(OM_CLASS, DS_C_CONTEXT),
    {DS_TIME_LIMIT, OM_S_INTEGER, {0, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor context_mod17[] = {
    OM_OID_DESC(OM_CLASS, DS_C_CONTEXT),
    {DS_AUTOMATIC_CONTINUATION, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor context_mod18[] = {
    OM_OID_DESC(OM_CLASS, DS_C_CONTEXT),
    {DS_DONT_USE_COPY, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DSX_DUAFIRST, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DSX_DUA_CACHE, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DSX_USEDSA, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor context_mod19[] = {
    OM_OID_DESC(OM_CLASS, DS_C_CONTEXT),
    {DS_DONT_USE_COPY, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DSX_DUAFIRST, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DSX_DUA_CACHE, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DSX_DONT_STORE, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DSX_NORMAL_CLASS, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor context_mod20[] = {
    OM_OID_DESC(OM_CLASS, DS_C_CONTEXT),
    {DS_CHAINING_PROHIB, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_PREFER_CHAINING, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor context_mod21[] = {
    OM_OID_DESC(OM_CLASS, DS_C_CONTEXT),
    {DS_SIZE_LIMIT, OM_S_INTEGER, {1, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor context_mod22[] = {
    OM_OID_DESC(OM_CLASS, DS_C_CONTEXT),
    {DSX_PREFER_ADM_FUNCS, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
/* entry information selection */
 
static  OM_descriptor entry_info_sel0[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO_SELECTION),
    {DS_ALL_ATTRIBUTES, OM_S_BOOLEAN, OM_FALSE},
    {DS_INFO_TYPE, OM_S_ENUMERATION, {DS_TYPES_ONLY, NULL} },
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_info_sel1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO_SELECTION),
    {DS_ALL_ATTRIBUTES, OM_S_BOOLEAN, OM_TRUE},
    {DS_INFO_TYPE, OM_S_ENUMERATION, {DS_TYPES_AND_VALUES, NULL} },
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_info_sel2[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO_SELECTION),
    {DS_ALL_ATTRIBUTES, OM_S_BOOLEAN, OM_TRUE},
    {DS_INFO_TYPE, OM_S_ENUMERATION, {DS_TYPES_ONLY, NULL} },
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_info_sel3[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO_SELECTION),
    {DS_ALL_ATTRIBUTES, OM_S_BOOLEAN, OM_FALSE},
    OM_OID_DESC(DS_ATTRIBUTES_SELECTED, DS_A_OBJECT_CLASS),
    {DS_INFO_TYPE, OM_S_ENUMERATION, {DS_TYPES_ONLY, NULL} },
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_info_sel4[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO_SELECTION),
    {DS_ALL_ATTRIBUTES, OM_S_BOOLEAN, OM_FALSE},
    OM_OID_DESC(DS_ATTRIBUTES_SELECTED, DS_A_OBJECT_CLASS),
    {DS_INFO_TYPE, OM_S_ENUMERATION, {DS_TYPES_AND_VALUES, NULL} },
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_info_sel5[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO_SELECTION),
    {DS_ALL_ATTRIBUTES, OM_S_BOOLEAN, OM_FALSE},
    OM_OID_DESC(DS_ATTRIBUTES_SELECTED, DS_A_COUNTRY_NAME),
    {DS_INFO_TYPE, OM_S_ENUMERATION, {DS_TYPES_AND_VALUES, NULL} },
    OM_NULL_DESCRIPTOR
    };
 
/* invalid */
static  OM_descriptor entry_info_sel6[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO_SELECTION),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_ADD_ATTRIBUTE, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_USER_PASSWORD),
    {DS_ATTRIBUTE_VALUES, OM_S_OCTET_STRING, OM_STRING("secret")},
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_info_sel7[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO_SELECTION),
    {DS_ALL_ATTRIBUTES, OM_S_BOOLEAN, OM_FALSE},
    OM_OID_DESC(DS_ATTRIBUTES_SELECTED, DSX_A_CDS_Towers),
    {DS_INFO_TYPE, OM_S_ENUMERATION, {DS_TYPES_AND_VALUES, NULL} },
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_info_sel8[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO_SELECTION),
    {DS_ALL_ATTRIBUTES, OM_S_BOOLEAN, OM_FALSE},
    OM_OID_DESC(DS_ATTRIBUTES_SELECTED, DS_A_OBJECT_CLASS),
    OM_OID_DESC(DS_ATTRIBUTES_SELECTED, DS_A_SURNAME),
    {DS_INFO_TYPE, OM_S_ENUMERATION, {DS_TYPES_AND_VALUES, NULL} },
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_info_sel9[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO_SELECTION),
    {DS_ALL_ATTRIBUTES, OM_S_BOOLEAN, OM_FALSE},
    OM_OID_DESC(DS_ATTRIBUTES_SELECTED, DS_A_OBJECT_CLASS),
    OM_OID_DESC(DS_ATTRIBUTES_SELECTED, DS_A_SURNAME),
    {DS_INFO_TYPE, OM_S_ENUMERATION, {DS_TYPES_ONLY, NULL} },
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_info_sel10[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO_SELECTION),
    {DS_ALL_ATTRIBUTES, OM_S_BOOLEAN, OM_FALSE},
    OM_OID_DESC(DS_ATTRIBUTES_SELECTED, DS_A_POSTAL_ADDRESS),
    {DS_INFO_TYPE, OM_S_ENUMERATION, {DS_TYPES_AND_VALUES, NULL} },
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_info_sel11[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO_SELECTION),
    {DS_ALL_ATTRIBUTES, OM_S_BOOLEAN, OM_FALSE},
    OM_OID_DESC(DS_ATTRIBUTES_SELECTED, DS_A_OBJECT_CLASS),
    OM_OID_DESC(DS_ATTRIBUTES_SELECTED, DS_A_SURNAME),
    {DS_INFO_TYPE, OM_S_ENUMERATION, {DS_TYPES_AND_VALUES, NULL} },
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_info_sel12[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO_SELECTION),
    {DS_ALL_ATTRIBUTES, OM_S_BOOLEAN, OM_FALSE},
    OM_OID_DESC(DS_ATTRIBUTES_SELECTED, DS_A_OBJECT_CLASS),
    OM_OID_DESC(DS_ATTRIBUTES_SELECTED, DS_A_OR_ADDRESSES),
    OM_OID_DESC(DS_ATTRIBUTES_SELECTED, DS_A_SURNAME),
    {DS_INFO_TYPE, OM_S_ENUMERATION, {DS_TYPES_AND_VALUES, NULL} },
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_info_sel13[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO_SELECTION),
    {DS_ALL_ATTRIBUTES, OM_S_BOOLEAN, OM_FALSE},
    OM_OID_DESC(DS_ATTRIBUTES_SELECTED, DS_A_USER_CERT),
    {DS_INFO_TYPE, OM_S_ENUMERATION, {DS_TYPES_AND_VALUES, NULL} },
    OM_NULL_DESCRIPTOR
    };
 

/* entry modification */
 
static  OM_descriptor entry_mod1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_ADD_ATTRIBUTE, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_USER_PASSWORD),
    {DS_ATTRIBUTE_VALUES, OM_S_OCTET_STRING, OM_STRING("secret")},
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod2[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_ADD_VALUES, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_USER_PASSWORD),
    {DS_ATTRIBUTE_VALUES, OM_S_OCTET_STRING, OM_STRING("secret")},
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod3[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_REMOVE_ATTRIBUTE, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_USER_PASSWORD),
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod4[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_REMOVE_VALUES, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_USER_PASSWORD),
    {DS_ATTRIBUTE_VALUES, OM_S_OCTET_STRING, OM_STRING("secret")},
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod5[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_ADD_ATTRIBUTE, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_DESCRIPTION),
    {DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("Ireland")},
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod6[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_ADD_ATTRIBUTE, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_PHONE_NBR),
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("+353 1 767441 x401")},
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod7[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_REMOVE_ATTRIBUTE, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_PHONE_NBR),
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod8[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_ADD_VALUES, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_PHONE_NBR),
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("+353 1 767441 x999")},
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod9[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_REMOVE_VALUES, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_PHONE_NBR),
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("+353 1 767441 x999")},
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod10[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_REMOVE_ATTRIBUTE, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_ALIASED_OBJECT_NAME),
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod11[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_ADD_ATTRIBUTE, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_ALIASED_OBJECT_NAME),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, name20} },
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod12[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_ADD_ATTRIBUTE, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_PHONE_NBR),
    {DS_ATTRIBUTE_VALUES,OM_S_OCTET_STRING,OM_STRING("\x02\x03\x04")},
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod13[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_ADD_ATTRIBUTE, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_UNDEFINED_ATTRIBUTE),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("xxx")},
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod14[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_ADD_ATTRIBUTE, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_PHONE_NBR),
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("123456789012345678901234567890123")},
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod15[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_ADD_ATTRIBUTE, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_SURNAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("Ryan")},
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod16[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_REMOVE_ATTRIBUTE, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_COMMON_NAME),
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod17[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_ADD_ATTRIBUTE, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_PHONE_NBR),
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod18[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_REMOVE_ATTRIBUTE, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_COMMON_NAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("vinnie")},
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod19[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_REMOVE_VALUES, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DSX_A_ACL),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, acl0}},
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod20[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_ADD_VALUES, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DSX_A_ACL),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, acl1}},
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod21[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_REMOVE_VALUES, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_PHONE_NBR),
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("+353 1 767441 x401")},
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod22[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_ADD_ATTRIBUTE, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DSX_A_CDS_Address),
    {DS_ATTRIBUTE_VALUES, OM_S_OCTET_STRING, OM_STRING("\x07\x17")},
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod23[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_REMOVE_ATTRIBUTE, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DSX_A_CDS_Address),
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod24[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_ADD_VALUES, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DSX_A_CDS_AllUpTo),
    {DS_ATTRIBUTE_VALUES, OM_S_OCTET_STRING, OM_STRING("\x55")},
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod25[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_REMOVE_VALUES, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DSX_A_CDS_AllUpTo),
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("\x06\x02")},
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod26[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_ADD_VALUES, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_PHONE_NBR),
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("+49 89 636 41094")},
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod27[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_ADD_VALUES, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_PHONE_NBR),
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("+353 1 767551")},
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("+49 89 636 41094")},
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("+353 1 772222")},
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod28[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_REMOVE_VALUES, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_PHONE_NBR),
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("+353 1 767551")},
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod29[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_REMOVE_VALUES, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DSX_A_ACL),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, acl1}},
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod30[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_ADD_VALUES, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DSX_A_ACL),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, acl0}},
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod31[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_ADD_ATTRIBUTE, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_PHONE_NBR),
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("+49.89.636.1234")},
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod32[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_ADD_VALUES, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_PHONE_NBR),
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("+49.89.636.5678")},
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod33[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_REMOVE_VALUES, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_PHONE_NBR),
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("+49.89.636.1234")},
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod34[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_REMOVE_ATTRIBUTE, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_PHONE_NBR),
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod35[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_ADD_VALUES, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OR_ADDRESSES),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, or_addr4}},
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod36[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_REMOVE_VALUES, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OR_ADDRESSES),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, or_addr2}},
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod37[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_ADD_ATTRIBUTE, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_DELIV_CONTENT_LENGTH),
    {DS_ATTRIBUTE_VALUES, OM_S_INTEGER, { 2048, NULL } },
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod38[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_ADD_ATTRIBUTE, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_DELIV_CONTENT_TYPES),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, MH_CTO_INNER_MESSAGE),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, MH_CTO_UNIDENTIFIED),
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod39[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_ADD_ATTRIBUTE, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_DELIV_EITS),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, MH_EE_G3_FAX),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, MH_EE_TELEX),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, MH_EE_TELETEX),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, MH_EE_IA5_TEXT),
    OM_NULL_DESCRIPTOR
    };
 
 
/* entry modification list */
 
static  OM_descriptor entry_mod_list1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD_LIST),
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod1} },
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod_list2[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD_LIST),
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod2} },
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod_list3[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD_LIST),
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod3} },
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod_list4[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD_LIST),
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod4} },
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod_list5[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD_LIST),
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod5} },
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod_list6[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD_LIST),
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod6} },
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod_list7[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD_LIST),
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod7} },
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod_list8[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD_LIST),
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod8} },
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod_list9[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD_LIST),
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod9} },
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod_list10[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD_LIST),
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod9} },
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod3} },
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod21} },
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod27} },
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod_list11[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD_LIST),
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod10} },
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod11} },
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod_list12[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD_LIST),
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod12} },
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod_list13[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD_LIST),
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod13} },
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod_list14[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD_LIST),
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod14} },
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod_list15[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD_LIST),
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod15} },
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod_list16[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD_LIST),
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod16} },
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod_list17[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD_LIST),
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod17} },
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod_list18[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD_LIST),
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod18} },
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod_list19[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD_LIST),
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod19} },
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod20} },
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod_list20[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD_LIST),
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod22} },
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod_list21[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD_LIST),
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod23} },
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod_list22[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD_LIST),
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod24} },
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod_list23[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD_LIST),
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod25} },
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod_list24[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD_LIST),
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod26} },
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod_list25[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD_LIST),
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod6} },
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod8} },
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod9} },
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod7} },
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod_list26[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD_LIST),
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod6} },
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod27} },
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod_list27[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD_LIST),
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod28} },
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod26} },
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod_list28[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD_LIST),
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod29} },
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod30} },
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod_list29[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD_LIST),
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod31} },
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod32} },
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod33} },
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod34} },
    OM_NULL_DESCRIPTOR
    };
 
static  OM_descriptor entry_mod_list30[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD_LIST),
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod35} },
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod36} },
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod37} },
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod38} },
    {DS_CHANGES, OM_S_OBJECT, {0, entry_mod39} },
    OM_NULL_DESCRIPTOR
    };
 
 

 
/* results */
 
 
/* compare result */
 
static OM_descriptor compare_result1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_COMPARE_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_MATCHED, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor compare_result2[] = {
    OM_OID_DESC(OM_CLASS, DS_C_COMPARE_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_MATCHED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor compare_result3[] = {
    OM_OID_DESC(OM_CLASS, DS_C_COMPARE_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_MATCHED, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name8} },
    OM_NULL_DESCRIPTOR
};
 
 
/* access_points */
 
#if 0 /* CC 20.1.94 not used */
static OM_descriptor access_points1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ACCESS_POINT),
    {DS_AE_TITLE, OM_S_OBJECT, { 0, name46} },
    {DS_ADDRESS, OM_S_OBJECT, {0, address1} },
    OM_NULL_DESCRIPTOR
};
#endif /* 0 */
 
/* continuation reference */
 
 
static OM_descriptor    continuation_ref2[] = {
    OM_OID_DESC(OM_CLASS, DS_C_CONTINUATION_REF),
/*    {DS_ACCESS_POINTS, OM_S_OBJECT, {0, access_points1} }, */
    {DS_ALIASED_RDNS, OM_S_INTEGER, {0, NULL} },
    {DS_OPERATION_PROGRESS, OM_S_OBJECT, {0, operation_prog1} },
    {DS_TARGET_OBJECT, OM_S_OBJECT, {0, name51} },
    OM_NULL_DESCRIPTOR
};
 
 
/* partial outcome qual */
 
 
static OM_descriptor    partial_outcome_qual3[] = {
    OM_OID_DESC(OM_CLASS, DS_C_PARTIAL_OUTCOME_QUAL),
    {DS_LIMIT_PROBLEM, OM_S_ENUMERATION, {DS_SIZE_LIMIT_EXCEEDED, NULL} },
    {DS_UNAVAILABLE_CRIT_EXT, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_UNEXPLORED, OM_S_OBJECT, {0, continuation_ref2}},
    OM_NULL_DESCRIPTOR
};
 
 
/* relative name */
 
 
 
/* list info item */
 
static OM_descriptor list_info_item1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIST_INFO_ITEM),
    {DS_ALIAS_ENTRY, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_RDN, OM_S_OBJECT, {0, rdn9} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor list_info_item2[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIST_INFO_ITEM),
    {DS_ALIAS_ENTRY, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_RDN, OM_S_OBJECT, {0, rdn21} },
    OM_NULL_DESCRIPTOR
};
 
 
static OM_descriptor list_info_item4[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIST_INFO_ITEM),
    {DS_ALIAS_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_RDN, OM_S_OBJECT, {0, rdn20} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor list_info_item5[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIST_INFO_ITEM),
    {DS_ALIAS_ENTRY, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_RDN, OM_S_OBJECT, {0, rdn48} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor list_info_item6[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIST_INFO_ITEM),
    {DS_ALIAS_ENTRY, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_RDN, OM_S_OBJECT, {0, rdn49} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor list_info_item7[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIST_INFO_ITEM),
    {DS_ALIAS_ENTRY, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_RDN, OM_S_OBJECT, {0, rdn42} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor list_info_item8[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIST_INFO_ITEM),
    {DS_ALIAS_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_RDN, OM_S_OBJECT, {0, rdn43} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor list_info_item9[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIST_INFO_ITEM),
    {DS_ALIAS_ENTRY, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_RDN, OM_S_OBJECT, {0, rdn46} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor list_info_item10[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIST_INFO_ITEM),
    {DS_ALIAS_ENTRY, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_RDN, OM_S_OBJECT, {0, rdn11} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor list_info_item11[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIST_INFO_ITEM),
    {DS_ALIAS_ENTRY, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_RDN, OM_S_OBJECT, {0, rdn34} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor list_info_item12[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIST_INFO_ITEM),
    {DS_ALIAS_ENTRY, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_RDN, OM_S_OBJECT, {0, rdn1} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor list_info_item13[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIST_INFO_ITEM),
    {DS_ALIAS_ENTRY, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_RDN, OM_S_OBJECT, {0, rdn32} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor list_info_item14[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIST_INFO_ITEM),
    {DS_ALIAS_ENTRY, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_RDN, OM_S_OBJECT, {0, rdn61} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor list_info_item15[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIST_INFO_ITEM),
    {DS_ALIAS_ENTRY, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_FROM_ENTRY, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_RDN, OM_S_OBJECT, {0, rdn63} },
    OM_NULL_DESCRIPTOR
};
 
 
/* list info */
 
static OM_descriptor list_info1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIST_INFO),
    {DS_SUBORDINATES, OM_S_OBJECT, {0, list_info_item1} },
    {DS_SUBORDINATES, OM_S_OBJECT, {0, list_info_item2} },
    {DS_SUBORDINATES, OM_S_OBJECT, {0, list_info_item4} },
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor list_info2[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIST_INFO),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor list_info3[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIST_INFO),
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name4} },
    {DS_SUBORDINATES, OM_S_OBJECT, {0, list_info_item1} },
    {DS_SUBORDINATES, OM_S_OBJECT, {0, list_info_item2} },
    {DS_SUBORDINATES, OM_S_OBJECT, {0, list_info_item4} },
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor list_info4[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIST_INFO),
    {DS_SUBORDINATES, OM_S_OBJECT, {0, list_info_item5} },
    {DS_SUBORDINATES, OM_S_OBJECT, {0, list_info_item6} },
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor list_info5[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIST_INFO),
    {DS_SUBORDINATES, OM_S_OBJECT, {0, list_info_item7} },
    {DS_SUBORDINATES, OM_S_OBJECT, {0, list_info_item8} },
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor list_info6[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIST_INFO),
    {DS_SUBORDINATES, OM_S_OBJECT, {0, list_info_item7} },
    {DS_SUBORDINATES, OM_S_OBJECT, {0, list_info_item8} },
    {DS_SUBORDINATES, OM_S_OBJECT, {0, list_info_item9} },
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor list_info7[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIST_INFO),
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name69} },
    {DS_SUBORDINATES, OM_S_OBJECT, {0, list_info_item5} },
    {DS_SUBORDINATES, OM_S_OBJECT, {0, list_info_item6} },
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor list_info8[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIST_INFO),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor list_info9[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIST_INFO),
    {DS_SUBORDINATES, OM_S_OBJECT, {0, list_info_item10} },
    {DS_SUBORDINATES, OM_S_OBJECT, {0, list_info_item11} },
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor list_info10[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIST_INFO),
    {DS_SUBORDINATES, OM_S_OBJECT, {0, list_info_item10} },
    {DS_PARTIAL_OUTCOME_QUAL, OM_S_OBJECT, {0, partial_outcome_qual3} },
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor list_info11[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIST_INFO),
    {DS_SUBORDINATES, OM_S_OBJECT, {0, list_info_item12} },
    {DS_SUBORDINATES, OM_S_OBJECT, {0, list_info_item13} },
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor list_info12[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIST_INFO),
    {DS_SUBORDINATES, OM_S_OBJECT, {0, list_info_item6} },
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor list_info13[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIST_INFO),
    {DS_SUBORDINATES, OM_S_OBJECT, {0, list_info_item14} },
    {DS_SUBORDINATES, OM_S_OBJECT, {0, list_info_item15} },
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
 
/* list result */
 
static OM_descriptor list_result1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIST_RESULT),
    {DS_LIST_INFO, OM_S_OBJECT, {0, list_info1} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor list_result2[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIST_RESULT),
    {DS_LIST_INFO, OM_S_OBJECT, {0, list_info2} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor list_result3[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIST_RESULT),
    {DS_LIST_INFO, OM_S_OBJECT, {0, list_info3} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor list_result4[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIST_RESULT),
    {DS_LIST_INFO, OM_S_OBJECT, {0, list_info4} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor list_result5[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIST_RESULT),
    {DS_LIST_INFO, OM_S_OBJECT, {0, list_info5} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor list_result6[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIST_RESULT),
    {DS_LIST_INFO, OM_S_OBJECT, {0, list_info6} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor list_result7[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIST_RESULT),
    {DS_LIST_INFO, OM_S_OBJECT, {0, list_info7} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor list_result8[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIST_RESULT),
    {DS_LIST_INFO, OM_S_OBJECT, {0, list_info8} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor list_result9[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIST_RESULT),
    {DS_LIST_INFO, OM_S_OBJECT, {0, list_info9} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor list_result10[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIST_RESULT),
    {DS_LIST_INFO, OM_S_OBJECT, {0, list_info10} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor list_result11[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIST_RESULT),
    {DS_LIST_INFO, OM_S_OBJECT, {0, list_info11} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor list_result12[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIST_RESULT),
    {DS_LIST_INFO, OM_S_OBJECT, {0, list_info12} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor list_result13[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIST_RESULT),
    {DS_LIST_INFO, OM_S_OBJECT, {0, list_info13} },
    OM_NULL_DESCRIPTOR
};
 
 
/* search info */
 
static OM_descriptor search_info1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_SEARCH_INFO),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor search_info2[] = {
    OM_OID_DESC(OM_CLASS, DS_C_SEARCH_INFO),
    {DS_ENTRIES, OM_S_OBJECT, {0, entry_info14} },
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor search_info3[] = {
    OM_OID_DESC(OM_CLASS, DS_C_SEARCH_INFO),
    {DS_ENTRIES, OM_S_OBJECT, {0, entry_info15} },
    {DS_ENTRIES, OM_S_OBJECT, {0, entry_info16} },
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor search_info4[] = {
    OM_OID_DESC(OM_CLASS, DS_C_SEARCH_INFO),
    {DS_ENTRIES, OM_S_OBJECT, {0, entry_info11} },
    {DS_ENTRIES, OM_S_OBJECT, {0, entry_info17} },
    {DS_ENTRIES, OM_S_OBJECT, {0, entry_info18} },
    {DS_ENTRIES, OM_S_OBJECT, {0, entry_info19} },
    {DS_ENTRIES, OM_S_OBJECT, {0, entry_info20} },
    {DS_ENTRIES, OM_S_OBJECT, {0, entry_info21} },
    {DS_ENTRIES, OM_S_OBJECT, {0, entry_info12} },
    {DS_ENTRIES, OM_S_OBJECT, {0, entry_info13} },
    {DS_ENTRIES, OM_S_OBJECT, {0, entry_info22} },
    {DS_ENTRIES, OM_S_OBJECT, {0, entry_info23} },
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor search_info5[] = {
    OM_OID_DESC(OM_CLASS, DS_C_SEARCH_INFO),
    {DS_ENTRIES, OM_S_OBJECT, {0, entry_info12} },
    {DS_ENTRIES, OM_S_OBJECT, {0, entry_info22} },
    {DS_ENTRIES, OM_S_OBJECT, {0, entry_info23} },
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor search_info6[] = {
    OM_OID_DESC(OM_CLASS, DS_C_SEARCH_INFO),
    {DS_ENTRIES, OM_S_OBJECT, {0, entry_info21} },
    {DS_ENTRIES, OM_S_OBJECT, {0, entry_info12} },
    {DS_ENTRIES, OM_S_OBJECT, {0, entry_info22} },
    {DS_ENTRIES, OM_S_OBJECT, {0, entry_info23} },
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor search_info7[] = {
    OM_OID_DESC(OM_CLASS, DS_C_SEARCH_INFO),
    {DS_ENTRIES, OM_S_OBJECT, {0, entry_info32} },
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor search_info8[] = {
    OM_OID_DESC(OM_CLASS, DS_C_SEARCH_INFO),
    {DS_ENTRIES, OM_S_OBJECT, {0, entry_info45} },
    {DS_ENTRIES, OM_S_OBJECT, {0, entry_info46} },
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor search_info9[] = {
    OM_OID_DESC(OM_CLASS, DS_C_SEARCH_INFO),
    {DS_ENTRIES, OM_S_OBJECT, {0, entry_info55} },
    {DS_ENTRIES, OM_S_OBJECT, {0, entry_info56} },
    {DS_ENTRIES, OM_S_OBJECT, {0, entry_info57} },
    {DS_ENTRIES, OM_S_OBJECT, {0, entry_info58} },
    {DS_ENTRIES, OM_S_OBJECT, {0, entry_info59} },
    {DS_ENTRIES, OM_S_OBJECT, {0, entry_info60} },
    {DS_ENTRIES, OM_S_OBJECT, {0, entry_info61} },
    {DS_ENTRIES, OM_S_OBJECT, {0, entry_info62} },
    {DS_ENTRIES, OM_S_OBJECT, {0, entry_info63} },
    {DS_ENTRIES, OM_S_OBJECT, {0, entry_info64} },
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor search_info10[] = {
    OM_OID_DESC(OM_CLASS, DS_C_SEARCH_INFO),
    {DS_ENTRIES, OM_S_OBJECT, {0, entry_info22} },
    {DS_ENTRIES, OM_S_OBJECT, {0, entry_info77} },
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor search_info11[] = {
    OM_OID_DESC(OM_CLASS, DS_C_SEARCH_INFO),
    {DS_ENTRIES, OM_S_OBJECT, {0, entry_info83} },
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor search_info12[] = {
    OM_OID_DESC(OM_CLASS, DS_C_SEARCH_INFO),
    {DS_ENTRIES, OM_S_OBJECT, {0, entry_info84} },
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor search_info13[] = {
    OM_OID_DESC(OM_CLASS, DS_C_SEARCH_INFO),
    {DS_ENTRIES, OM_S_OBJECT, {0, entry_info83} },
    {DS_ENTRIES, OM_S_OBJECT, {0, entry_info84} },
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
 
/* search result */
 
static OM_descriptor search_result1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_SEARCH_RESULT),
    {DS_SEARCH_INFO, OM_S_OBJECT, {0, search_info1} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor search_result2[] = {
    OM_OID_DESC(OM_CLASS, DS_C_SEARCH_RESULT),
    {DS_SEARCH_INFO, OM_S_OBJECT, {0, search_info2} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor search_result3[] = {
    OM_OID_DESC(OM_CLASS, DS_C_SEARCH_RESULT),
    {DS_SEARCH_INFO, OM_S_OBJECT, {0, search_info3} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor search_result4[] = {
    OM_OID_DESC(OM_CLASS, DS_C_SEARCH_RESULT),
    {DS_SEARCH_INFO, OM_S_OBJECT, {0, search_info4} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor search_result5[] = {
    OM_OID_DESC(OM_CLASS, DS_C_SEARCH_RESULT),
    {DS_SEARCH_INFO, OM_S_OBJECT, {0, search_info5} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor search_result6[] = {
    OM_OID_DESC(OM_CLASS, DS_C_SEARCH_RESULT),
    {DS_SEARCH_INFO, OM_S_OBJECT, {0, search_info6} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor search_result7[] = {
    OM_OID_DESC(OM_CLASS, DS_C_SEARCH_RESULT),
    {DS_SEARCH_INFO, OM_S_OBJECT, {0, search_info7} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor search_result8[] = {
    OM_OID_DESC(OM_CLASS, DS_C_SEARCH_RESULT),
    {DS_SEARCH_INFO, OM_S_OBJECT, {0, search_info8} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor search_result9[] = {
    OM_OID_DESC(OM_CLASS, DS_C_SEARCH_RESULT),
    {DS_SEARCH_INFO, OM_S_OBJECT, {0, search_info9} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor search_result10[] = {
    OM_OID_DESC(OM_CLASS, DS_C_SEARCH_RESULT),
    {DS_SEARCH_INFO, OM_S_OBJECT, {0, search_info10} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor search_result11[] = {
    OM_OID_DESC(OM_CLASS, DS_C_SEARCH_RESULT),
    {DS_SEARCH_INFO, OM_S_OBJECT, {0, search_info11} },
    OM_NULL_DESCRIPTOR
};

static OM_descriptor search_result12[] = {
    OM_OID_DESC(OM_CLASS, DS_C_SEARCH_RESULT),
    {DS_SEARCH_INFO, OM_S_OBJECT, {0, search_info12} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor search_result13[] = {
    OM_OID_DESC(OM_CLASS, DS_C_SEARCH_RESULT),
    {DS_SEARCH_INFO, OM_S_OBJECT, {0, search_info13} },
    OM_NULL_DESCRIPTOR
};
 
 
/* read result */
 
static OM_descriptor read_result1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info1} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor read_result2[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info4} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor read_result3[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info5} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor read_result4[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info4} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor read_result5[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info9} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor read_result6[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info10} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor read_result7[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info11} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor read_result8[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info12} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor read_result9[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info12} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor read_result10[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info13} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor read_result11[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info24} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor read_result12[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info25} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor read_result13[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info26} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor read_result14[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info27} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor read_result15[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info28} },
    OM_NULL_DESCRIPTOR
};
 
OM_descriptor read_result16[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info29} },
    OM_NULL_DESCRIPTOR
};
 
OM_descriptor read_result17[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info3} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor read_result18[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info30} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor read_result19[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info31} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor read_result20[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info33} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor read_result21[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info34} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor read_result22[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info35} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor read_result23[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info36} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor read_result24[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info37} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor read_result25[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info38} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor read_result26[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info39} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor read_result27[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info40} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor read_result28[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info41} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor read_result29[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info42} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor read_result30[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info43} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor read_result31[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info44} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor read_result32[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info47} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor read_result33[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info48} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor read_result34[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info49} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor read_result35[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info50} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor read_result36[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info51} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor read_result37[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info52} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor read_result38[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info53} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor read_result39[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info54} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor read_result40[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info65} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor read_result41[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info66} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor read_result42[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info67} },
    OM_NULL_DESCRIPTOR
};

static OM_descriptor read_result43[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info68} },
    OM_NULL_DESCRIPTOR
};

static OM_descriptor read_result44[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info69} },
    OM_NULL_DESCRIPTOR
};

static OM_descriptor read_result45[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info70} },
    OM_NULL_DESCRIPTOR
};

static OM_descriptor read_result46[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info71} },
    OM_NULL_DESCRIPTOR
};

static OM_descriptor read_result47[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info72} },
    OM_NULL_DESCRIPTOR
};

static OM_descriptor read_result48[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_TRUE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info73} },
    OM_NULL_DESCRIPTOR
};

static OM_descriptor read_result49[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info74} },
    OM_NULL_DESCRIPTOR
};

static OM_descriptor read_result50[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info75} },
    OM_NULL_DESCRIPTOR
};

static OM_descriptor read_result51[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info18} },
    OM_NULL_DESCRIPTOR
};

static OM_descriptor read_result52[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info17} },
    OM_NULL_DESCRIPTOR
};

static OM_descriptor read_result53[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info76} },
    OM_NULL_DESCRIPTOR
};

static OM_descriptor read_result54[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info22} },
    OM_NULL_DESCRIPTOR
};

static OM_descriptor read_result55[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info78} },
    OM_NULL_DESCRIPTOR
};

static OM_descriptor read_result56[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info77} },
    OM_NULL_DESCRIPTOR
};

static OM_descriptor read_result57[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info79} },
    OM_NULL_DESCRIPTOR
};

static OM_descriptor read_result58[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info80} },
    OM_NULL_DESCRIPTOR
};

static OM_descriptor read_result59[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info81} },
    OM_NULL_DESCRIPTOR
};

static OM_descriptor read_result60[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info82} },
    OM_NULL_DESCRIPTOR
};

static OM_descriptor read_result61[] = {
    OM_OID_DESC(OM_CLASS, DS_C_READ_RESULT),
    {DS_ALIAS_DEREFERENCED, OM_S_BOOLEAN, {OM_FALSE, NULL} },
    {DS_ENTRY, OM_S_OBJECT, {0, entry_info85} },
    OM_NULL_DESCRIPTOR
};

 
/* filter item */
 
static OM_descriptor filter_item1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_FILTER_ITEM),
    {DS_FILTER_ITEM_TYPE, OM_S_ENUMERATION, {DS_EQUALITY, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_USER_PASSWORD),
    {DS_ATTRIBUTE_VALUES,OM_S_OCTET_STRING, OM_STRING("secret")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor filter_item2[] = {
    OM_OID_DESC(OM_CLASS, DS_C_FILTER_ITEM),
    {DS_FILTER_ITEM_TYPE, OM_S_ENUMERATION, {DS_EQUALITY, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_TITLE),
    {DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("software engineer")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor filter_item3[] = {
    OM_OID_DESC(OM_CLASS, DS_C_FILTER_ITEM),
    {DS_FILTER_ITEM_TYPE, OM_S_ENUMERATION, {DS_PRESENT, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OBJECT_CLASS),
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor filter_item4[] = {
    OM_OID_DESC(OM_CLASS, DS_C_FILTER_ITEM),
    {DS_FILTER_ITEM_TYPE, OM_S_ENUMERATION, {DS_SUBSTRINGS, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_COMMON_NAME),
    {DS_FINAL_SUBSTRING, OM_S_TELETEX_STRING, OM_STRING("T")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor filter_item5[] = {
    OM_OID_DESC(OM_CLASS, DS_C_FILTER_ITEM),
    {DS_FILTER_ITEM_TYPE, OM_S_ENUMERATION, {DS_PRESENT, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_SURNAME),
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor filter_item6[] = {
    OM_OID_DESC(OM_CLASS, DS_C_FILTER_ITEM),
    {DS_FILTER_ITEM_TYPE, OM_S_ENUMERATION, {DS_SUBSTRINGS, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_DESCRIPTION),
    {DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("admin")},
    {DS_INITIAL_SUBSTRING, OM_S_TELETEX_STRING, OM_STRING("  DATABASE  ")},
    OM_NULL_DESCRIPTOR
};
 
/* invalid syntax for Surname */
static OM_descriptor filter_item7[] = {
    OM_OID_DESC(OM_CLASS, DS_C_FILTER_ITEM),
    {DS_FILTER_ITEM_TYPE, OM_S_ENUMERATION, {DS_EQUALITY, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_SURNAME),
    {DS_ATTRIBUTE_VALUES,OM_S_OCTET_STRING,OM_STRING("\x02\x03\x04")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor filter_item8[] = {
    OM_OID_DESC(OM_CLASS, DS_C_FILTER_ITEM),
    {DS_FILTER_ITEM_TYPE, OM_S_ENUMERATION, {DS_EQUALITY, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_PHONE_NBR),
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("123456789012345678901234567890123")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor filter_item9[] = {
    OM_OID_DESC(OM_CLASS, DS_C_FILTER_ITEM),
    {DS_FILTER_ITEM_TYPE, OM_S_ENUMERATION, {DS_SUBSTRINGS, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_COUNTRY_NAME),
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("de")},
    OM_NULL_DESCRIPTOR
};
 
/* SUBSTRINGS: "*:a*"  */
static OM_descriptor filter_item10[] = {
    OM_OID_DESC(OM_CLASS, DS_C_FILTER_ITEM),
    {DS_FILTER_ITEM_TYPE, OM_S_ENUMERATION, {DS_SUBSTRINGS, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_COMMON_NAME),
    {DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("\xC8\x61")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor filter_item11[] = {
    OM_OID_DESC(OM_CLASS, DS_C_FILTER_ITEM),
    {DS_FILTER_ITEM_TYPE, OM_S_ENUMERATION, {DS_EQUALITY, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OR_ADDRESSES),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, or_addr2} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor filter_item12[] = {
    OM_OID_DESC(OM_CLASS, DS_C_FILTER_ITEM),
    {DS_FILTER_ITEM_TYPE, OM_S_ENUMERATION, {DS_EQUALITY, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OR_ADDRESSES),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, or_addr3} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor filter_item13[] = {
    OM_OID_DESC(OM_CLASS, DS_C_FILTER_ITEM),
    {DS_FILTER_ITEM_TYPE, OM_S_ENUMERATION, {DS_PRESENT, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_USER_CERT),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_A_USER_CERT),
    OM_NULL_DESCRIPTOR
};
static OM_descriptor filter_item14[] = {
    OM_OID_DESC(OM_CLASS, DS_C_FILTER_ITEM),
    {DS_FILTER_ITEM_TYPE, OM_S_ENUMERATION, {DS_PRESENT, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_CA_CERT),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_A_CA_CERT),
    OM_NULL_DESCRIPTOR
};
 
 
 
/* filter */
 
static OM_descriptor no_filter[] = {
    OM_OID_DESC(OM_CLASS, DS_C_FILTER),
    {DS_FILTER_TYPE, OM_S_ENUMERATION, {DS_AND, NULL} },
    OM_NULL_DESCRIPTOR
};
 
/*
static OM_descriptor no_filter[] = {
    {DS_NO_FILTER}
};
 */
 
static OM_descriptor filter1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_FILTER),
    {DS_FILTER_ITEMS, OM_S_OBJECT, {0, filter_item1} },
    {DS_FILTER_TYPE, OM_S_ENUMERATION, {DS_AND, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor filter2[] = {
    OM_OID_DESC(OM_CLASS, DS_C_FILTER),
    {DS_FILTER_ITEMS, OM_S_OBJECT, {0, filter_item2} },
    {DS_FILTERS, OM_S_OBJECT, {0, filter1} },
    {DS_FILTER_TYPE, OM_S_ENUMERATION, {DS_AND, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor filter3[] = {
    OM_OID_DESC(OM_CLASS, DS_C_FILTER),
    {DS_FILTER_ITEMS, OM_S_OBJECT, {0, filter_item3} },
    {DS_FILTER_TYPE, OM_S_ENUMERATION, {DS_AND, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor filter4[] = {
    OM_OID_DESC(OM_CLASS, DS_C_FILTER),
    {DS_FILTER_ITEMS, OM_S_OBJECT, {0, filter_item4} },
    {DS_FILTER_TYPE, OM_S_ENUMERATION, {DS_NOT, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor filter5[] = {
    OM_OID_DESC(OM_CLASS, DS_C_FILTER),
    {DS_FILTER_ITEMS, OM_S_OBJECT, {0, filter_item5} },
    {DS_FILTER_ITEMS, OM_S_OBJECT, {0, filter_item6} },
    {DS_FILTER_TYPE, OM_S_ENUMERATION, {DS_OR, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor filter6[] = {
    OM_OID_DESC(OM_CLASS, DS_C_FILTER),
    {DS_FILTER_ITEMS, OM_S_OBJECT, {0, filter_item7} },
    {DS_FILTER_TYPE, OM_S_ENUMERATION, {DS_AND, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor filter7[] = {
    OM_OID_DESC(OM_CLASS, DS_C_FILTER),
    {DS_FILTER_ITEMS, OM_S_OBJECT, {0, filter_item8} },
    {DS_FILTER_TYPE, OM_S_ENUMERATION, {DS_AND, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor filter8[] = {
    OM_OID_DESC(OM_CLASS, DS_C_FILTER),
    {DS_FILTER_ITEMS, OM_S_OBJECT, {0, filter_item9} },
    {DS_FILTER_TYPE, OM_S_ENUMERATION, {DS_AND, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor filter9[] = {
    OM_OID_DESC(OM_CLASS, DS_C_FILTER),
    {DS_FILTER_ITEMS, OM_S_OBJECT, {0, filter_item10} },
    {DS_FILTER_TYPE, OM_S_ENUMERATION, {DS_AND, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor filter10[] = {
    OM_OID_DESC(OM_CLASS, DS_C_FILTER),
    {DS_FILTER_ITEMS, OM_S_OBJECT, {0, filter_item12} },
    {DS_FILTER_TYPE, OM_S_ENUMERATION, {DS_NOT, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor filter11[] = {
    OM_OID_DESC(OM_CLASS, DS_C_FILTER),
    {DS_FILTER_ITEMS, OM_S_OBJECT, {0, filter_item11} },
/* TBD    {DS_FILTERS, OM_S_OBJECT, {0, filter10} }, */
    {DS_FILTER_TYPE, OM_S_ENUMERATION, {DS_AND, NULL} },
    OM_NULL_DESCRIPTOR
};

static OM_descriptor filter12[] = {
    OM_OID_DESC(OM_CLASS, DS_C_FILTER),
    {DS_FILTER_ITEMS, OM_S_OBJECT, {0, filter_item13} },
    {DS_FILTER_TYPE, OM_S_ENUMERATION, {DS_AND, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor filter13[] = {
    OM_OID_DESC(OM_CLASS, DS_C_FILTER),
    {DS_FILTER_ITEMS, OM_S_OBJECT, {0, filter_item14} },
    {DS_FILTER_TYPE, OM_S_ENUMERATION, {DS_AND, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor filter14[] = {
    OM_OID_DESC(OM_CLASS, DS_C_FILTER),
    {DS_FILTER_ITEMS, OM_S_OBJECT, {0, filter_item13} },
    {DS_FILTER_ITEMS, OM_S_OBJECT, {0, filter_item14} },
    {DS_FILTER_TYPE, OM_S_ENUMERATION, {DS_OR, NULL} },
    OM_NULL_DESCRIPTOR
};
 
 
/* feature_list */
 
#define SERVICE_PACKAGE "\x2b\x0c\x02\x87\x73\x1c\x00"
#define BDCP_PACKAGE    "\x2b\x0c\x02\x87\x73\x1c\x01"
#define SAP_PACKAGE     "\x2b\x0c\x02\x87\x73\x1c\x02"
#define MDUP_PACKAGE    "\x2b\x0c\x02\x87\x73\x1c\x03"
#define GDS_PACKAGE     "\x2b\x0c\x02\x88\x53\x01\x03\x64\x00"
 
DS_feature  feature_list0[] = {
    { 0 }
};
 
DS_feature  feature_list1[] = {
    { OM_STRING(GDS_PACKAGE), 1 },
    { OM_STRING(BDCP_PACKAGE), 1 },
    { 0 }
};
 
DS_feature  feature_list2[] = {
    { OM_STRING(GDS_PACKAGE), 1 },
    { 0 }
};
 
DS_feature  feature_list3[] = {
    { OM_STRING(GDS_PACKAGE), 0 },
    { 0 }
};
 
DS_feature  feature_list4[] = {
    { OM_STRING(GDS_PACKAGE), 1 },
    { OM_STRING(BDCP_PACKAGE), 1 },
    { OM_STRING(MDUP_PACKAGE), 1 },
    { 0 }
};
 
DS_feature  feature_list5[] = {
    { OM_STRING(GDS_PACKAGE), 1 },
    { OM_STRING(BDCP_PACKAGE), 1 },
    { OM_STRING(MDUP_PACKAGE), 1 },
    { OM_STRING(SAP_PACKAGE), 1 },
    { 0 }
};
 
 
 
/* DS_status !! commented out 8.9.92 CC
 
static OM_descriptor ds_success[] = {
    (OM_descriptor *) DS_SUCCESS
};
*/
 
static OM_descriptor abandon_failed1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ABANDON_FAILED),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_TOO_LATE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor att_problem1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_PROBLEM),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_ATTRIBUTE_OR_VALUE_EXISTS, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_TITLE),
    {DS_ATTRIBUTE_VALUE, OM_S_TELETEX_STRING, OM_STRING("software engineer")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor att_problem2[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_PROBLEM),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_INVALID_ATTRIBUTE_SYNTAX, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_COMMON_NAME),
    {DS_ATTRIBUTE_VALUE, OM_S_PRINTABLE_STRING, OM_STRING("vinnie")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor att_problem3[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_PROBLEM),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_NO_SUCH_ATTRIBUTE_OR_VALUE, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_COUNTRY_NAME),
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor att_problem4[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_PROBLEM),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_INVALID_ATTRIBUTE_SYNTAX, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_PHONE_NBR),
    {DS_ATTRIBUTE_VALUE, OM_S_OCTET_STRING, OM_STRING("\x02\x03\x04")},
/*  {DS_ATTRIBUTE_VALUE, OM_S_PRINTABLE_STRING, OM_STRING("\x02\x03\x04")}, */
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor att_problem5[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_PROBLEM),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_UNDEFINED_ATTRIBUTE_TYPE, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_UNDEFINED_ATTRIBUTE),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("xxx")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor att_problem6[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_PROBLEM),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_CONSTRAINT_VIOLATION, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_PHONE_NBR),
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("123456789012345678901234567890123")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor att_problem7[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_PROBLEM),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_CONSTRAINT_VIOLATION, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_COUNTRY_NAME),
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("IRELAND")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor att_problem8[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_PROBLEM),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_INAPPROP_MATCHING, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_COUNTRY_NAME),
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("de")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor att_problem9[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_PROBLEM),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_INVALID_ATTRIBUTE_SYNTAX, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_SURNAME),
    {DS_ATTRIBUTE_VALUE, OM_S_OCTET_STRING, OM_STRING("\x02\x03\x04")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor att_problem10[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_PROBLEM),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_ATTRIBUTE_OR_VALUE_EXISTS, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_SURNAME),
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor att_problem11[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_PROBLEM),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_NO_SUCH_ATTRIBUTE_OR_VALUE, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DSX_A_CDS_Towers),
    OM_NULL_DESCRIPTOR
};
 
 
static OM_descriptor att_problem12[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_PROBLEM),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_CONSTRAINT_VIOLATION, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_POSTAL_ADDRESS),
/* DS_ATTRIBUTE_VALUE is not returned in this case
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, postal_address1}},
 */
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor att_problem13[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_PROBLEM),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_UNDEFINED_ATTRIBUTE_TYPE, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE,DS_A_SURNAME),
    {DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("Ryan")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor att_problem14[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_PROBLEM),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_INVALID_ATTRIBUTE_SYNTAX, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_PHONE_NBR),
    {DS_ATTRIBUTE_VALUE, OM_S_OCTET_STRING,OM_STRING("\x02\x03\x04")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor att_problem15[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_PROBLEM),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_INVALID_ATTRIBUTE_SYNTAX, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OR_ADDRESSES),
    {DS_ATTRIBUTE_VALUE, OM_S_OBJECT, {0, or_addr7} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor att_problem16[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_PROBLEM),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_INVALID_ATTRIBUTE_SYNTAX, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OR_ADDRESSES),
    {DS_ATTRIBUTE_VALUE, OM_S_OBJECT, {0, or_addr8} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor att_problem17[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_PROBLEM),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_INVALID_ATTRIBUTE_SYNTAX, NULL} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OR_ADDRESSES),
    {DS_ATTRIBUTE_VALUE, OM_S_OBJECT, {0, or_addr9} },
    OM_NULL_DESCRIPTOR
};
 
 
static OM_descriptor att_err1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_ERROR),
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name6} },
    {DS_PROBLEMS, OM_S_OBJECT, {0, att_problem1} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor att_err2[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_ERROR),
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name31} },
    {DS_PROBLEMS, OM_S_OBJECT, {0, att_problem2} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor att_err3[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_ERROR),
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name8} },
    {DS_PROBLEMS, OM_S_OBJECT, {0, att_problem3} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor att_err4[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_ERROR),
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name8} },
    {DS_PROBLEMS, OM_S_OBJECT, {0, att_problem4} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor att_err5[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_ERROR),
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name8} },
    {DS_PROBLEMS, OM_S_OBJECT, {0, att_problem5} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor att_err6[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_ERROR),
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name4} },
    {DS_PROBLEMS, OM_S_OBJECT, {0, att_problem6} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor att_err7[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_ERROR),
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name20} },
    {DS_PROBLEMS, OM_S_OBJECT, {0, att_problem2} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor att_err8[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_ERROR),
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name1} },
    {DS_PROBLEMS, OM_S_OBJECT, {0, att_problem7} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor att_err9[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_ERROR),
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name1} },
    {DS_PROBLEMS, OM_S_OBJECT, {0, att_problem8} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor att_err10[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_ERROR),
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name8} },
    {DS_PROBLEMS, OM_S_OBJECT, {0, att_problem9} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor att_err11[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_ERROR),
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name8} },
    {DS_PROBLEMS, OM_S_OBJECT, {0, att_problem10} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor att_err12[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_ERROR),
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name52} },
    {DS_PROBLEMS, OM_S_OBJECT, {0, att_problem11} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor att_err13[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_ERROR),
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name56} },
    {DS_PROBLEMS, OM_S_OBJECT, {0, att_problem12} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor att_err14[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_ERROR),
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name4} },
    {DS_PROBLEMS, OM_S_OBJECT, {0, att_problem13} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor att_err15[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_ERROR),
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name8} },
    {DS_PROBLEMS, OM_S_OBJECT, {0, att_problem14} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor att_err16[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_ERROR),
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name78} },
    {DS_PROBLEMS, OM_S_OBJECT, {0, att_problem15} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor att_err17[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_ERROR),
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name78} },
    {DS_PROBLEMS, OM_S_OBJECT, {0, att_problem16} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor att_err18[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_ERROR),
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name78} },
    {DS_PROBLEMS, OM_S_OBJECT, {0, att_problem17} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor att_err19[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_ERROR),
    {DS_OBJECT_NAME, OM_S_OBJECT, {0, name2} },
    {DS_PROBLEMS, OM_S_OBJECT, {0, att_problem6} },
    OM_NULL_DESCRIPTOR
};
 
 
static OM_descriptor communications_err1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_COMMUNICATIONS_ERROR),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_COMMUNICATIONS_PROBLEM, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor library_err1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIBRARY_ERROR),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_BAD_SESSION, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor library_err2[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIBRARY_ERROR),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_MISCELLANEOUS, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor library_err3[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIBRARY_ERROR),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_BAD_ARGUMENT, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor library_err4[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIBRARY_ERROR),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_BAD_CLASS, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor library_err5[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIBRARY_ERROR),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_BAD_CONTEXT, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor library_err6[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIBRARY_ERROR),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_BAD_NAME, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor library_err7[] = { 0 }; /* unused */
 
static OM_descriptor library_err8[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIBRARY_ERROR),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_MISSING_TYPE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor library_err9[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIBRARY_ERROR),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_TOO_MANY_SESSIONS, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor library_err10[] = {
    OM_OID_DESC(OM_CLASS, DS_C_LIBRARY_ERROR),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_NOT_SUPPORTED, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor name_err1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_NAME_ERROR),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_NO_SUCH_OBJECT, NULL} },
    {DS_MATCHED, OM_S_OBJECT, {0, name1} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor name_err2[] = {
    OM_OID_DESC(OM_CLASS, DS_C_NAME_ERROR),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_NO_SUCH_OBJECT, NULL} },
    {DS_MATCHED, OM_S_OBJECT, {0, name4} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor name_err3[] = {
    OM_OID_DESC(OM_CLASS, DS_C_NAME_ERROR),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_ALIAS_DEREFERENCING_PROBLEM, NULL} },
    {DS_MATCHED, OM_S_OBJECT, {0, name19} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor name_err4[] = {
    OM_OID_DESC(OM_CLASS, DS_C_NAME_ERROR),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_INVALID_ATTRIBUTE_VALUE, NULL} },
    {DS_MATCHED, OM_S_OBJECT, {0, name4} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor name_err5[] = {
    OM_OID_DESC(OM_CLASS, DS_C_NAME_ERROR),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_NO_SUCH_OBJECT, NULL} },
    {DS_MATCHED, OM_S_OBJECT, {0, name51} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor name_err6[] = {
    OM_OID_DESC(OM_CLASS, DS_C_NAME_ERROR),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_NO_SUCH_OBJECT, NULL} },
    {DS_MATCHED, OM_S_OBJECT, {0, name57} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor name_err7[] = {
    OM_OID_DESC(OM_CLASS, DS_C_NAME_ERROR),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_NO_SUCH_OBJECT, NULL} },
    {DS_MATCHED, OM_S_OBJECT, {0, name57} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor name_err8[] = {
    OM_OID_DESC(OM_CLASS, DS_C_NAME_ERROR),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_NO_SUCH_OBJECT, NULL} },
    {DS_MATCHED, OM_S_OBJECT, {0, name70} },
    OM_NULL_DESCRIPTOR
};
 
 
static OM_descriptor referral1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_REFERRAL),
/*  {DS_ACCESS_POINTS, OM_S_OBJECT, {0, access_points1} }, */
    {DS_ALIASED_RDNS, OM_S_INTEGER, {0, NULL} },
    {DS_OPERATION_PROGRESS, OM_S_OBJECT, {0, operation_prog2} },
    {DS_TARGET_OBJECT, OM_S_OBJECT, {0, name46} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor security_err1[] = { 
    OM_OID_DESC(OM_CLASS, DS_C_SECURITY_ERROR), 
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_INSUFFICIENT_ACCESS_RIGHTS, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor security_err2[] = { 
    OM_OID_DESC(OM_CLASS, DS_C_SECURITY_ERROR), 
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_INVALID_CREDENTIALS, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor service_err1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_SERVICE_ERROR),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_OUT_OF_SCOPE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor service_err2[] = {
    OM_OID_DESC(OM_CLASS, DS_C_SERVICE_ERROR),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_TIME_LIMIT_EXCEEDED, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor service_err3[] = {
    OM_OID_DESC(OM_CLASS, DS_C_SERVICE_ERROR),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_CHAINING_REQUIRED, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor service_err4[] = {
    OM_OID_DESC(OM_CLASS, DS_C_SERVICE_ERROR),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_UNAVAILABLE, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor service_err5[] = {
    OM_OID_DESC(OM_CLASS, DS_C_SERVICE_ERROR),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_UNWILLING_TO_PERFORM, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor system_err1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_SYSTEM_ERROR),
    {DS_PROBLEM, OM_S_ENUMERATION, {10, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor update_err1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_UPDATE_ERROR),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_OBJECT_CLASS_VIOLATION, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor update_err2[] = {
    OM_OID_DESC(OM_CLASS, DS_C_UPDATE_ERROR),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_ENTRY_EXISTS, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor update_err3[] = {
    OM_OID_DESC(OM_CLASS, DS_C_UPDATE_ERROR),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_NAMING_VIOLATION, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor update_err4[] = {
    OM_OID_DESC(OM_CLASS, DS_C_UPDATE_ERROR),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_NOT_ALLOWED_ON_RDN, NULL} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor update_err5[] = {
    OM_OID_DESC(OM_CLASS, DS_C_UPDATE_ERROR),
    {DS_PROBLEM, OM_S_ENUMERATION, {DS_E_NOT_ALLOWED_ON_NON_LEAF, NULL} },
    OM_NULL_DESCRIPTOR
};
 
 
 
 
/* bound session */
 
OM_descriptor bound_session1[] = { OM_NULL_DESCRIPTOR };
OM_descriptor bound_session2[] = { OM_NULL_DESCRIPTOR };
OM_descriptor bound_session3[] = { OM_NULL_DESCRIPTOR };
OM_descriptor bound_session4[] = { OM_NULL_DESCRIPTOR };
OM_descriptor bound_session5[] = { OM_NULL_DESCRIPTOR };
OM_descriptor bound_session6[] = { OM_NULL_DESCRIPTOR };
OM_descriptor bound_session7[] = { OM_NULL_DESCRIPTOR };
OM_descriptor bound_session8[] = { OM_NULL_DESCRIPTOR };
OM_descriptor bound_session9[] = { OM_NULL_DESCRIPTOR };
OM_descriptor bound_session10[] = { OM_NULL_DESCRIPTOR };
OM_descriptor bound_session11[] = { OM_NULL_DESCRIPTOR };
OM_descriptor bound_session12[] = { OM_NULL_DESCRIPTOR };
OM_descriptor bound_session13[] = { OM_NULL_DESCRIPTOR };
OM_descriptor bound_session14[] = { OM_NULL_DESCRIPTOR };
OM_descriptor bound_session15[] = { OM_NULL_DESCRIPTOR };
OM_descriptor bound_session16[] = { OM_NULL_DESCRIPTOR };
OM_descriptor bound_session17[] = { OM_NULL_DESCRIPTOR };
 
 
/* workspace */
 
OM_workspace workspace1[] = { (OM_workspace) NULL };
 
OM_workspace workspace2[] = { (OM_workspace) NULL };
 
/* rules */
 
/* mac 23/9/91, won't compile for Philip so remove *
OM_object_identifier *rules1[] = { (OM_object_identifier *) NULL };
 */
/* cc 8.9.92, won't compile for anyone, so restore the * before rules1 */
OM_object_identifier *rules1[] = { (OM_object_identifier *) NULL };
 
/* type list */
 
/*
static OM_type type_list1[] = { (OM_type_list) 0 };
 */
 
static OM_type type_list2[] = {
    DS_ATTRIBUTE_TYPE,
    DS_ATTRIBUTE_VALUES,
    OM_NO_MORE_TYPES
};
 
static OM_type type_list3[] = {
    DS_DSA_NAME,
    OM_NO_MORE_TYPES
};
 
static OM_type type_list4[] = {
    DSX_DIR_ID,
    OM_NO_MORE_TYPES
};
 
static OM_type type_list5[] = {
    DS_DSA_NAME,
    DSX_DIR_ID,
    OM_NO_MORE_TYPES
};
 
static OM_type type_list6[] = {
    DS_ATTRIBUTE_TYPE,
    OM_NO_MORE_TYPES
};
 
static OM_type type_list7[] = {
    DS_RDNS,
    OM_NO_MORE_TYPES
};
 
static OM_type type_list8[] = {
    OM_CLASS,
    DS_RDNS,
    OM_NO_MORE_TYPES
};
 
static OM_type type_list9[] = {
    DS_ATTRIBUTE_VALUES,
    OM_NO_MORE_TYPES
};


/* navigation path for omX_extract */
static OM_type nav_path1[] = {
	DS_SEARCH_INFO,
    OM_NO_MORE_TYPES
};


static OM_type nav_path2[] = {
	DS_SEARCH_INFO,
	DS_ENTRIES,
    OM_NO_MORE_TYPES
};

static OM_type nav_path3[] = {
	DS_SEARCH_INFO,
	DS_ENTRIES,
	DS_ATTRIBUTES,
    OM_NO_MORE_TYPES
};

static OM_type nav_path4[] = {
	DS_SEARCH_INFO,
	DS_ENTRIES,
	DS_ATTRIBUTES,
	DS_ATTRIBUTE_VALUES,
    OM_NO_MORE_TYPES
};

static OM_type nav_path5[] = {
	DS_SEARCH_INFO,
	DS_OBJECT_NAME,
    OM_NO_MORE_TYPES
};

static OM_type nav_path6[] = {
	DS_SEARCH_INFO,
	DS_OBJECT_NAME,
	DS_RDNS,
    OM_NO_MORE_TYPES
};


static OM_type nav_path7[] = {
	DS_ENTRY,
	DS_ATTRIBUTES,
	DS_ATTRIBUTE_TYPE,
    OM_NO_MORE_TYPES
};


static OM_type nav_path8[] = {
	DS_ENTRY,
	DS_ATTRIBUTES,
    OM_NO_MORE_TYPES
};



/* private object */
 
OM_descriptor private_obj1[] = { OM_NULL_DESCRIPTOR };
OM_descriptor private_obj2[] = { OM_NULL_DESCRIPTOR };
OM_descriptor private_obj3[] = { OM_NULL_DESCRIPTOR };
OM_descriptor private_obj4[] = { OM_NULL_DESCRIPTOR };
OM_descriptor private_obj5[] = { OM_NULL_DESCRIPTOR };
OM_descriptor private_obj6[] = { OM_NULL_DESCRIPTOR };
OM_descriptor private_obj7[] = { OM_NULL_DESCRIPTOR };
OM_descriptor private_obj8[] = { OM_NULL_DESCRIPTOR };
OM_descriptor private_obj9[] = { OM_NULL_DESCRIPTOR };
OM_descriptor private_obj10[] = { OM_NULL_DESCRIPTOR };
OM_descriptor private_obj11[] = { OM_NULL_DESCRIPTOR };
OM_descriptor private_obj12[] = { OM_NULL_DESCRIPTOR };
OM_descriptor private_obj13[] = { OM_NULL_DESCRIPTOR };
OM_descriptor private_obj14[] = { OM_NULL_DESCRIPTOR };
OM_descriptor private_obj15[] = { OM_NULL_DESCRIPTOR };
OM_descriptor private_obj16[] = { OM_NULL_DESCRIPTOR };
OM_descriptor private_obj17[] = { OM_NULL_DESCRIPTOR };
OM_descriptor private_obj18[] = { OM_NULL_DESCRIPTOR };
OM_descriptor private_obj19[] = { OM_NULL_DESCRIPTOR };
OM_descriptor private_obj20[] = { OM_NULL_DESCRIPTOR };
OM_descriptor private_obj21[] = { OM_NULL_DESCRIPTOR };
OM_descriptor private_obj22[] = { OM_NULL_DESCRIPTOR };
OM_descriptor private_obj23[] = { OM_NULL_DESCRIPTOR };
OM_descriptor private_obj24[] = { OM_NULL_DESCRIPTOR };
OM_descriptor private_obj25[] = { OM_NULL_DESCRIPTOR };
OM_descriptor private_obj26[] = { OM_NULL_DESCRIPTOR };
OM_descriptor private_obj27[] = { OM_NULL_DESCRIPTOR };
OM_descriptor private_obj28[] = { OM_NULL_DESCRIPTOR };
OM_descriptor private_obj29[] = { OM_NULL_DESCRIPTOR };
OM_descriptor private_obj30[] = { OM_NULL_DESCRIPTOR };
OM_descriptor private_result[] = { OM_NULL_DESCRIPTOR };
 
static OM_descriptor not_private[] = {
    OM_OID_DESC(OM_CLASS, DS_C_SESSION),
    {DS_DSA_NAME, OM_S_OBJECT, {0, name1} },
    {DS_FILE_DESCRIPTOR, OM_S_INTEGER, {1, NULL} },
    OM_NULL_DESCRIPTOR
};
 
/* public object */
 
OM_descriptor public_obj1[] = { OM_NULL_DESCRIPTOR };
OM_descriptor public_obj2[] = { OM_NULL_DESCRIPTOR };
OM_descriptor public_obj3[] = { OM_NULL_DESCRIPTOR };
OM_descriptor public_obj4[] = { OM_NULL_DESCRIPTOR };
OM_descriptor public_obj5[] = { OM_NULL_DESCRIPTOR };
OM_descriptor public_obj6[] = { OM_NULL_DESCRIPTOR };
OM_descriptor public_obj7[] = { OM_NULL_DESCRIPTOR };
OM_descriptor public_obj8[] = { OM_NULL_DESCRIPTOR };
OM_descriptor public_obj9[] = { OM_NULL_DESCRIPTOR };
OM_descriptor public_obj10[] = { OM_NULL_DESCRIPTOR };
 
/*
 * data from example.c for extract test
 * Public Object ("Descriptor List") for Name argument to ds_read().
 * Build the Distinguished-Name of Peter Piper.
 */

static OM_descriptor	country_extract[] = {
OM_OID_DESC(OM_CLASS, DS_C_AVA),
OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_COUNTRY_NAME),
{ DS_ATTRIBUTE_VALUES,OM_S_PRINTABLE_STRING,OM_STRING("US") },
OM_NULL_DESCRIPTOR
};
static OM_descriptor	organization_extract[] = {
OM_OID_DESC(OM_CLASS, DS_C_AVA),
OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_ORG_NAME),
{ DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("Acme Pepper Co") },
OM_NULL_DESCRIPTOR
};
static OM_descriptor	organizational_unit_extract[] = {
OM_OID_DESC(OM_CLASS, DS_C_AVA),
OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_ORG_UNIT_NAME),
{ DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("Research") },
OM_NULL_DESCRIPTOR
};
static OM_descriptor	common_name_extract[] = {
OM_OID_DESC(OM_CLASS, DS_C_AVA),
OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_COMMON_NAME),
{ DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("Peter Piper") },
OM_NULL_DESCRIPTOR
};

static OM_descriptor	rdn1_extract[] = {
OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
{ DS_AVAS, OM_S_OBJECT, { 0, country_extract } },
OM_NULL_DESCRIPTOR
};
static OM_descriptor	rdn2_extract[] = {
OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
{ DS_AVAS, OM_S_OBJECT, { 0, organization_extract } },
OM_NULL_DESCRIPTOR
};
static OM_descriptor	rdn3_extract[] = {
OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
{ DS_AVAS, OM_S_OBJECT, { 0, organizational_unit_extract } },
OM_NULL_DESCRIPTOR
};
static OM_descriptor	rdn4_extract[] = {
OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
{ DS_AVAS, OM_S_OBJECT, { 0, common_name_extract } },
OM_NULL_DESCRIPTOR
};

OM_descriptor	name_extract[] = {
OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
{ DS_RDNS, OM_S_OBJECT, { 0, rdn1_extract } },
{ DS_RDNS, OM_S_OBJECT, { 0, rdn2_extract } },
{ DS_RDNS, OM_S_OBJECT, { 0, rdn3_extract } },
{ DS_RDNS, OM_S_OBJECT, { 0, rdn4_extract } },
OM_NULL_DESCRIPTOR
};

/*
 * Public Object ("Descriptor List") for Entry-Information-Selection
 * argument to ds_read().
 */
OM_descriptor selection_extract[] = {
OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO_SELECTION),
{ DS_ALL_ATTRIBUTES, OM_S_BOOLEAN, { OM_FALSE, NULL } },
OM_OID_DESC(DS_ATTRIBUTES_SELECTED, DS_A_PHONE_NBR),
{ DS_INFO_TYPE,OM_S_ENUMERATION, { DS_TYPES_AND_VALUES,NULL } },
OM_NULL_DESCRIPTOR
};


/* parameter tables */
 
/* NOTE - XT_LEN_SESSION_TBL must agree with value in xt_len.h */
const OM_descriptor *master_session_tbl[XT_LEN_SESSION_TBL] = {
    DS_DEFAULT_SESSION,
    session1,
    session2,
    session3,
    session4,
    session5,
    session6,
    session7,
    session8,
    session9,
    bound_session1,
    bound_session2, 
    bound_session3,
    bound_session4,
    bound_session5,
    bound_session6,
    bound_session7,
    bound_session8,
    bound_session9,
    bound_session10,
    bound_session11,
    bound_session12,
    bound_session13,
    bound_session14,
    bound_session15,
    bound_session16,
    bound_session17
};
 
 
OM_descriptor *context_tbl[] = {
    DS_DEFAULT_CONTEXT,
    context1,
    context_mod1,
    context_mod2,
    context_mod3,
    context_mod4,
    context_mod5,
    context_mod6,
    context_mod7,
    context_mod8,
    context_mod9,
    context_mod10,
    context_mod11,
    context_mod12,
    context_mod13,
    context_mod14,
    context_mod15,
    context_mod16,
    context_mod17,
    context_mod18,
    context_mod19,
    context_mod20,
    context_mod21,
    context_mod22
};
 
 
OM_descriptor *name_tbl[] = {
    name0,
    name1,
    name2,
    name3,
    name4,
    name5,
    name6,
    name7,
    name8,
    name9,
    name10,
    name11,
    name12,
    name13,
    name14,
    name15,
    name16,
    name17,
    name18,
    name19,
    name20,
    name21,
    name22,
    name23,
    name24,
    name25,
    name26,
    name27,
    name28,
    name29,
    name30,
    name31,
    name32,
    name33,
    name34,
    name35,
    name36,
    name37,
    name38,
    name39,
    name40,
    name41,
    name42,
    name43,
    name44,
    name45,
    name46,
    name47,
    name48,
    name49,
    name50,
    name51,
    name52,
    name53,
    name54,
    name55,
    name56,
    name57,
    name58,
    name59,
    name60,
    name61,
    name62,
    name63,
    name64,
    name65,
    name66,
    name67,
    name68,
    name69,
    name70,
    name71,
    name73,
    name74,
    name75,
    name76,
    name77,
    name78,
    name79,
    name80,
    name81,
    name82,
    name83,
    name84,
    name85,
    name86,
    name87,
    name88,
    name89,
    name90,
    name91,
    name92,
    name93,
    name94,
    top_name,
    bad_name1,
    bad_name2
};
 
 
OM_descriptor *entry_tbl[] = {
    att_list0,
    att_list1,
    att_list2,
    att_list3,
    att_list4,
    att_list5,
    att_list6,
    att_list7,
    att_list8,
    att_list9,
    att_list10,
    att_list11,
    att_list12,
    att_list13,
    att_list14,
    att_list15,
    att_list16,
    att_list17,
    att_list18,
    att_list19,
    att_list20,
    att_list21,
    att_list22,
    att_list23,
    att_list24,
    att_list25,
    att_list26,
    att_list27,
    att_list28,
    att_list29,
    att_list30,
    att_list31,
    att_list32,
    att_list33,
    att_list34,
    att_list35,
    att_list36,
    att_list37,
    att_list38,
    att_list39,
    att_list40,
    att_list41,
    att_list42,
    att_list43,
    att_list44,
    att_list45,
    att_list46,
    att_list47,
    att_list48,
    att_list49,
    att_list50,
    att_list51,
    att_list52,
    att_list53,
    att_list54,
    att_list55,
    att_list56,
    att_list57,
    att_list58,
    att_list59,
    att_list60,
    att_list61,
    att_list62,
    att_list63,
    att_list64,
    att_list65,
    att_list66,
    att_list67,
    att_list68,
    att_list69,
};
 
 
OM_descriptor *ava_tbl[] = {
    country1,
    org1,
    org2,
    org3,
    org8,
    org_unit1,
    org_unit2,
    org_unit3,
    org_unit4,
    c_name1,
    c_name2,
    c_name3,
    c_name4,
    c_name5,
    c_name6,
    c_name16,
    c_name32,
    surname1,
    surname2,
    surname3,
    surname4,
    surname5,
    surname6,
    surname7,
    surname8,
    user_pswd1,
    user_pswd2,
    user_pswd3,
    user_pswd4,
    typeless2,
    att28,
    att29,
    att51,
    or_address1,
    or_address2
};
 
 
OM_descriptor *rdn_tbl[] = {
    rdn0,
    rdn1,
    rdn2,
    rdn3,
    rdn9,
    rdn13,
    rdn16,
    rdn18,
    rdn20,
    rdn29,
    rdn43,
    rdn61,
    rdn63,
    rdn69
};
 
 
OM_descriptor *entry_modification_list_tbl[] = {
    entry_mod_list1,
    entry_mod_list2,
    entry_mod_list3,
    entry_mod_list4,
    entry_mod_list5,
    entry_mod_list6,
    entry_mod_list7,
    entry_mod_list8,
    entry_mod_list9,
    entry_mod_list10,
    entry_mod_list11,
    entry_mod_list12,
    entry_mod_list13,
    entry_mod_list14,
    entry_mod_list15,
    entry_mod_list16,
    entry_mod_list17,
    entry_mod_list18,
    entry_mod_list19,
    entry_mod_list20,
    entry_mod_list21,
    entry_mod_list22,
    entry_mod_list23,
    entry_mod_list24,
    entry_mod_list25,
    entry_mod_list26,
    entry_mod_list27,
    entry_mod_list28,
    entry_mod_list29,
    entry_mod_list30,
};
 
 
OM_descriptor *filter_tbl[] = {
    no_filter,
    filter1,
    filter2,
    filter3,
    filter4,
    filter5,
    filter6,
    filter7,
    filter8,
    filter9,
    filter10,
    filter11,
    filter12,
    filter13,
    filter14
};
 
 
 
OM_descriptor *entry_info_selection_tbl[] = {
    entry_info_sel0,
    entry_info_sel1,
    entry_info_sel2,
    entry_info_sel3,
    entry_info_sel4,
    entry_info_sel5,
    entry_info_sel6,
    entry_info_sel7,
    entry_info_sel8,
    entry_info_sel9,
    entry_info_sel10,
    entry_info_sel11,
    entry_info_sel12,
    entry_info_sel13,
};
 
 
OM_descriptor *compare_result_tbl[] = {
    compare_result1,
    compare_result2,
    compare_result3
};
 
OM_descriptor *read_result_tbl[] = {
    read_result1,
    read_result2,
    read_result3,
    read_result4,
    read_result5,
    read_result6,
    read_result7,
    read_result8,
    read_result9,
    read_result10,
    read_result11,
    read_result12,
    read_result13,
    read_result14,
    read_result15,
    read_result16,
    read_result17,
    read_result18,
    read_result19,
    read_result20,
    read_result21,
    read_result22,
    read_result23,
    read_result24,
    read_result25,
    read_result26,
    read_result27,
    read_result28,
    read_result29,
    read_result30,
    read_result31,
    read_result32,
    read_result33,
    read_result34,
    read_result35,
    read_result36,
    read_result37,
    read_result38,
    read_result39,
    read_result40,
    read_result41,
    read_result42,
    read_result43,
    read_result44,
    read_result45,
    read_result46,
    read_result47,
    read_result48,
    read_result49,
    read_result50,
    read_result51,
    read_result52,
    read_result53,
    read_result54,
    read_result55,
    read_result56,
    read_result57,
    read_result58,
    read_result59,
    read_result60,
    read_result61,
};
 
 
OM_descriptor *bdcp_tbl[] = {
    fax_number1,
    postal_address1,
    search_guide1,
    teletex_terminal_id1,
    telex_number1
};
 
OM_descriptor *sap_tbl[] = {
		algo_id0,
        cert2,
        cert_list2,
        cert_list3,
        cert_pair2,
        cert_sublist2,
        sig1,
        sig2,
};
 
 
DS_status *status_tbl[] = {
    (DS_status *)DS_SUCCESS,
    (DS_status *)abandon_failed1,
    (DS_status *)att_err1,
    (DS_status *)att_err2,
    (DS_status *)att_err3,
    (DS_status *)att_err4,
    (DS_status *)att_err5,
    (DS_status *)att_err6,
    (DS_status *)att_err7,
    (DS_status *)att_err8,
    (DS_status *)att_err9,
    (DS_status *)att_err10,
    (DS_status *)att_err11,
    (DS_status *)att_err12,
    (DS_status *)att_err13,
    (DS_status *)att_err14,
    (DS_status *)att_err15,
    (DS_status *)att_err16,
    (DS_status *)att_err17,
    (DS_status *)att_err18,
    (DS_status *)att_err19,
    (DS_status *)communications_err1,
    (DS_status *)library_err1,
    (DS_status *)library_err2,
    (DS_status *)library_err3,
    (DS_status *)library_err4,
    (DS_status *)library_err5,
    (DS_status *)library_err6,
    (DS_status *)library_err7,
    (DS_status *)library_err8,
    (DS_status *)library_err9,
    (DS_status *)library_err10,
    (DS_status *)name_err1,
    (DS_status *)name_err2,
    (DS_status *)name_err3,
    (DS_status *)name_err4,
    (DS_status *)name_err5,
    (DS_status *)name_err6,
    (DS_status *)name_err7,
    (DS_status *)name_err8,
    (DS_status *)referral1,
    (DS_status *)security_err1,
    (DS_status *)security_err2,
    (DS_status *)service_err1,
    (DS_status *)service_err2,
    (DS_status *)service_err3,
    (DS_status *)service_err4,
    (DS_status *)service_err5,
    (DS_status *)system_err1,
    (DS_status *)update_err1,
    (DS_status *)update_err2,
    (DS_status *)update_err3,
    (DS_status *)update_err4,
    (DS_status *)update_err5
};
 
 
OM_descriptor *list_result_tbl[] = {
    list_result1,
    list_result2,
    list_result3,
    list_result4,
    list_result5,
    list_result6,
    list_result7,
    list_result8,
    list_result9,
    list_result10,
    list_result11,
    list_result12,
    list_result13
};
 
 
OM_sint subset_tbl[] = {
    DS_BASE_OBJECT,
    DS_ONE_LEVEL,
    DS_WHOLE_SUBTREE
};
 
 
OM_uint32 boolean_tbl[] = {
    OM_TRUE,
    OM_FALSE
};
 
 
OM_descriptor *search_result_tbl[] = {
    search_result1,
    search_result2,
    search_result3,
    search_result4,
    search_result5,
    search_result6,
    search_result7,
    search_result8,
    search_result9,
    search_result10,
    search_result11,
    search_result12,
    search_result13,
};
 
 
DS_feature *feature_list_tbl[] = {
    feature_list0,
    feature_list1,
    feature_list2,
    feature_list3,
    feature_list4,
    feature_list5,
};
 
#define OMP_O_DS_C_INVALID_CLASS  "\x2B\xFF\xFF"
OM_EXPORT(DS_C_INVALID_CLASS)
 
struct class_tbl_struct class_tbl[] = {
    {"DS_C_ABANDON_FAILED",     &DS_C_ABANDON_FAILED},
    {"DS_C_ACCESS_POINT",       &DS_C_ACCESS_POINT},
    {"DS_C_ADDRESS",            &DS_C_ADDRESS},
    {"DS_C_ATTRIBUTE",          &DS_C_ATTRIBUTE},
    {"DS_C_ATTRIBUTE_ERROR",    &DS_C_ATTRIBUTE_ERROR},
    {"DS_C_ATTRIBUTE_LIST",     &DS_C_ATTRIBUTE_LIST},
    {"DS_C_ATTRIBUTE_PROBLEM",  &DS_C_ATTRIBUTE_PROBLEM},
    {"DS_C_AVA",                &DS_C_AVA},
    {"DS_C_COMMON_RESULTS",     &DS_C_COMMON_RESULTS},
    {"DS_C_COMMUNICATIONS_ERROR",   &DS_C_COMMUNICATIONS_ERROR},
    {"DS_C_COMPARE_RESULT",     &DS_C_COMPARE_RESULT},
    {"DS_C_CONTEXT",            &DS_C_CONTEXT},
    {"DS_C_CONTINUATION_REF", &DS_C_CONTINUATION_REF},
    {"DS_C_DS_DN",              &DS_C_DS_DN},
    {"DS_C_DS_RDN",             &DS_C_DS_RDN},
    {"DS_C_ENTRY_INFO",  &DS_C_ENTRY_INFO},
    {"DS_C_ENTRY_INFO_SELECTION",    &DS_C_ENTRY_INFO_SELECTION},
    {"DS_C_ENTRY_MOD", &DS_C_ENTRY_MOD},
    {"DS_C_ENTRY_MOD_LIST",&DS_C_ENTRY_MOD_LIST},
    {"DS_C_ERROR",              &DS_C_ERROR},
    {"DS_C_EXT",          &DS_C_EXT},
    {"DS_C_FILTER",             &DS_C_FILTER},
    {"DS_C_FILTER_ITEM",        &DS_C_FILTER_ITEM},
    {"DS_C_LIBRARY_ERROR",      &DS_C_LIBRARY_ERROR},
    {"DS_C_LIST_INFO",          &DS_C_LIST_INFO},
    {"DS_C_LIST_INFO_ITEM",     &DS_C_LIST_INFO_ITEM},
    {"DS_C_LIST_RESULT",        &DS_C_LIST_RESULT},
    {"DS_C_NAME",               &DS_C_NAME},
    {"DS_C_NAME_ERROR",         &DS_C_NAME_ERROR},
    {"DS_C_OPERATION_PROGRESS", &DS_C_OPERATION_PROGRESS},
    {"DS_C_PARTIAL_OUTCOME_QUAL",  &DS_C_PARTIAL_OUTCOME_QUAL},
    {"DS_C_PRESENTATION_ADDRESS",   &DS_C_PRESENTATION_ADDRESS},
    {"DS_C_READ_RESULT",        &DS_C_READ_RESULT},
    {"DS_C_REFERRAL",           &DS_C_REFERRAL},
    {"DS_C_RELATIVE_NAME",      &DS_C_RELATIVE_NAME},
    {"DS_C_SEARCH_INFO",        &DS_C_SEARCH_INFO},
    {"DS_C_SEARCH_RESULT",      &DS_C_SEARCH_RESULT},
    {"DS_C_SECURITY_ERROR",     &DS_C_SECURITY_ERROR},
    {"DS_C_SERVICE_ERROR",      &DS_C_SERVICE_ERROR},
    {"DS_C_SESSION",            &DS_C_SESSION},
    {"DS_C_SYSTEM_ERROR",       &DS_C_SYSTEM_ERROR},
    {"DS_C_UPDATE_ERROR",       &DS_C_UPDATE_ERROR},
/* xds_siemens */
    {"DSX_C_GDS_SESSION",       &DSX_C_GDS_SESSION},
    {"DSX_C_GDS_CONTEXT",       &DSX_C_GDS_CONTEXT},
    {"DSX_C_GDS_ACL",           &DSX_C_GDS_ACL},
    {"DSX_C_GDS_ACL_ITEM",      &DSX_C_GDS_ACL_ITEM},
/* bdcp */
    {"DS_C_FACSIMILE_PHONE_NBR", &DS_C_FACSIMILE_PHONE_NBR},
    {"DS_C_POSTAL_ADDRESS",     &DS_C_POSTAL_ADDRESS},
    {"DS_C_SEARCH_CRITERION",   &DS_C_SEARCH_CRITERION},
    {"DS_C_SEARCH_GUIDE",       &DS_C_SEARCH_GUIDE},
    {"DS_C_TELETEX_TERM_IDENT",&DS_C_TELETEX_TERM_IDENT},
    {"DS_C_TELEX_NBR",       &DS_C_TELEX_NBR},
/* xom */
    {"OM_C_ENCODING",           &OM_C_ENCODING},
    {"OM_C_EXTERNAL",           &OM_C_EXTERNAL},
/* sap */
    { "DS_C_ALGORITHM_IDENT",   &DS_C_ALGORITHM_IDENT},
    { "DS_C_CERT",              &DS_C_CERT},
    { "DS_C_CERT_LIST",        	&DS_C_CERT_LIST},
    { "DS_C_CERT_PAIR",       	&DS_C_CERT_PAIR},
    { "DS_C_CERT_SUBLIST",    	&DS_C_CERT_SUBLIST},
    { "DS_C_SIGNATURE",      	&DS_C_SIGNATURE},

    {"INVALID_CLASS",           &DS_C_INVALID_CLASS},
};
 
 
OM_object_identifier *attr_type_tbl[] = {
	&DS_A_ALIASED_OBJECT_NAME,
	&DS_A_BUSINESS_CATEGORY,
	&DS_A_COMMON_NAME,
	&DS_A_COUNTRY_NAME,
	&DS_A_DESCRIPTION,
	&DS_A_DEST_INDICATOR,
	&DS_A_FACSIMILE_PHONE_NBR,
	&DS_A_INTERNAT_ISDN_NBR,
	&DS_A_KNOWLEDGE_INFO,
	&DS_A_LOCALITY_NAME,
	&DS_A_MEMBER,
	&DS_A_OBJECT_CLASS,
	&DS_A_ORG_NAME,
	&DS_A_ORG_UNIT_NAME,
	&DS_A_OWNER,
	&DS_A_PHONE_NBR,
	&DS_A_PHYS_DELIV_OFF_NAME,
	&DS_A_POST_OFFICE_BOX,
	&DS_A_POSTAL_ADDRESS,
	&DS_A_POSTAL_CODE,
	&DS_A_PREF_DELIV_METHOD,
	&DS_A_PRESENTATION_ADDRESS,
	&DS_A_REGISTERED_ADDRESS,
	&DS_A_ROLE_OCCUPANT,
	&DS_A_SEARCH_GUIDE,
	&DS_A_SEE_ALSO,
	&DS_A_SERIAL_NBR,
	&DS_A_STATE_OR_PROV_NAME,
	&DS_A_STREET_ADDRESS,
	&DS_A_SUPPORT_APPLIC_CONTEXT,
	&DS_A_SURNAME,
	&DS_A_TELETEX_TERM_IDENT,
	&DS_A_TELEX_NBR,
	&DS_A_TITLE,
	&DS_A_USER_PASSWORD,
	&DS_A_X121_ADDRESS
};

OM_type type_tbl[] = {
    OM_CLASS,
    DS_ACCESS_POINTS,
    DS_ADDRESS,
    DS_AE_TITLE,
    DS_ALIASED_RDNS,
    DS_ALIAS_DEREFERENCED,
    DS_ALIAS_ENTRY,
    DS_ALL_ATTRIBUTES,
    DS_ASYNCHRONOUS,
    DS_ATTRIBUTES,
    DS_ATTRIBUTES_SELECTED,
    DS_ATTRIBUTE_TYPE,
    DS_ATTRIBUTE_VALUE,
    DS_ATTRIBUTE_VALUES,
    DS_AUTOMATIC_CONTINUATION,
    DS_AVAS,
    DS_CHAINING_PROHIB,
    DS_CHANGES,
    DS_CRIT,
    DS_DONT_DEREFERENCE_ALIASES,
    DS_DONT_USE_COPY,
    DS_DSA_ADDRESS,
    DS_DSA_NAME,
    DS_ENTRIES,
    DS_ENTRY,
    DS_EXT,
    DS_FILE_DESCRIPTOR,
    DS_FILTERS,
    DS_FILTER_ITEMS,
    DS_FILTER_ITEM_TYPE,
    DS_FILTER_TYPE,
    DS_FINAL_SUBSTRING,
    DS_FROM_ENTRY,
    DS_IDENT,
    DS_INFO_TYPE,
    DS_INITIAL_SUBSTRING,
    DS_ITEM_PARAMETERS,
    DS_LIMIT_PROBLEM,
    DS_LIST_INFO,
    DS_LOCAL_SCOPE,
    DS_MATCHED,
    DS_MOD_TYPE,
    DS_NAME_RESOLUTION_PHASE,
    DS_NEXT_RDN_TO_BE_RESOLVED,
    DS_N_ADDRESSES,
    DS_OBJECT_NAME,
    DS_OPERATION_PROGRESS,
    DS_PARTIAL_OUTCOME_QUAL,
    DS_PERFORMER,
    DS_PREFER_CHAINING,
    DS_PRIORITY,
    DS_PROBLEM,
    DS_PROBLEMS,
    DS_P_SELECTOR,
    DS_RDN,
    DS_RDNS,
    DS_RDNS_RESOLVED,
    DS_REQUESTOR,
    DS_SCOPE_OF_REFERRAL,
    DS_SEARCH_INFO,
    DS_SIZE_LIMIT,
    DS_SUBORDINATES,
    DS_S_SELECTOR,
    DS_TARGET_OBJECT,
    DS_TIME_LIMIT,
    DS_T_SELECTOR,
    DS_UNAVAILABLE_CRIT_EXT,
    DS_UNCORRELATED_LIST_INFO ,
    DS_UNCORRELATED_SEARCH_INFO,
    DS_UNEXPLORED,
    DSX_PASSWORD,
    DSX_DIR_ID,
    DSX_DUAFIRST,
    DSX_DONT_STORE,
    DSX_NORMAL_CLASS,
    DSX_PRIV_CLASS,
    DSX_RESIDENT_CLASS,
    DSX_USEDSA,
    DSX_DUA_CACHE,
    ((OM_type) 0)
};
 
 
OM_exclusions exclusions_tbl[] = {
    OM_NO_EXCLUSIONS,
    OM_EXCLUDE_ALL_BUT_THESE_TYPES,
    OM_EXCLUDE_ALL_BUT_THESE_VALUES,
    OM_EXCLUDE_MULTIPLES,
    OM_EXCLUDE_SUBOBJECTS,
    OM_EXCLUDE_VALUES,
    OM_EXCLUDE_DESCRIPTORS,
    ((OM_exclusions) 64)
};
 
OM_modification modification_tbl[] = {
    OM_INSERT_AT_BEGINNING, 
    OM_INSERT_AT_CERTAIN_POINT,
    OM_INSERT_AT_END,
    OM_REPLACE_ALL,
    OM_REPLACE_CERTAIN_VALUES,
    ((OM_modification) 9)
};
 
OM_syntax syntax_tbl[] = {
    OM_S_NO_MORE_SYNTAXES,  
    OM_S_BIT_STRING,    
    OM_S_BOOLEAN,
    OM_S_ENCODING_STRING,
    OM_S_ENUMERATION,
    OM_S_GENERAL_STRING,
    OM_S_GENERALISED_TIME_STRING,
    OM_S_GRAPHIC_STRING,    
    OM_S_IA5_STRING,    
    OM_S_INTEGER,
    OM_S_NULL,
    OM_S_NUMERIC_STRING,
    OM_S_OBJECT,    
    OM_S_OBJECT_DESCRIPTOR_STRING,
    OM_S_OBJECT_IDENTIFIER_STRING,
    OM_S_OCTET_STRING,  
    OM_S_PRINTABLE_STRING,
    OM_S_TELETEX_STRING,
    OM_S_UTC_TIME_STRING,
    OM_S_VIDEOTEX_STRING,   
    OM_S_VISIBLE_STRING,    
    OM_S_LONG_STRING,
    OM_S_NO_VALUE,
    OM_S_LOCAL_STRING,
    OM_S_SERVICE_GENERATED,
    OM_S_PRIVATE,   
    OM_S_SYNTAX,
    ((OM_syntax) 27)
};
 
 
/* NOTE - XT_LEN_WORKSPACE_TBL must agree with value in xt_len.h */
const OM_workspace  *master_workspace_tbl[XT_LEN_WORKSPACE_TBL] = {
    NULL,
    workspace1,
    workspace2
};
 
 
OM_object_identifier  *rules_tbl[] = {
    (OM_object_identifier *)rules1
};
 
OM_type_list  type_list_tbl[] = {
    0,                      /* type_list1 */
    type_list2,
    type_list3,
    type_list4,
    type_list5,
    type_list6,
    type_list7,
    type_list8,
    type_list9
};


OM_type_list  nav_path_tbl[] = {
	nav_path1,
	nav_path2,
	nav_path3,
	nav_path4,
	nav_path5,
	nav_path6,
	nav_path7,
	nav_path8,
};
 

OM_return_code  return_code_tbl[] = {
    OM_SUCCESS,
    OM_ENCODING_INVALID,
    OM_FUNCTION_DECLINED,
    OM_FUNCTION_INTERRUPTED,
    OM_MEMORY_INSUFFICIENT,
    OM_NETWORK_ERROR,
    OM_NO_SUCH_CLASS,
    OM_NO_SUCH_EXCLUSION,
    OM_NO_SUCH_MODIFICATION,
    OM_NO_SUCH_OBJECT,
    OM_NO_SUCH_RULES,
    OM_NO_SUCH_SYNTAX,
    OM_NO_SUCH_TYPE,
    OM_NO_SUCH_WORKSPACE,
    OM_NOT_AN_ENCODING,
    OM_NOT_CONCRETE,
    OM_NOT_PRESENT,
    OM_NOT_PRIVATE,
    OM_NOT_THE_SERVICES,
    OM_PERMANENT_ERROR,
    OM_POINTER_INVALID,
    OM_SYSTEM_ERROR,
    OM_TEMPORARY_ERROR,
    OM_TOO_MANY_VALUES,
    OM_VALUES_NOT_ADJACENT,
    OM_WRONG_VALUE_LENGTH,
    OM_WRONG_VALUE_MAKEUP,
    OM_WRONG_VALUE_NUMBER,
    OM_WRONG_VALUE_POSITION,
    OM_WRONG_VALUE_SYNTAX,
    OM_WRONG_VALUE_TYPE
};
 
/* renamed by CC 27.8.92 */
/* NOTE - XT_LEN_PRIVATE_OBJECT_TBL must agree with value in xt_len.h */
/*        and private_result must be at private_result_index in table */
/*		  where private_result_index is defined in xt_len.h as 31*/
const OM_private_object master_private_object_tbl[XT_LEN_PRIVATE_OBJECT_TBL] = {
    private_obj1,
    private_obj2,
    private_obj3,
    private_obj4,
    private_obj5,
    private_obj6,
    private_obj7,
    private_obj8,
    private_obj9,
    private_obj10,
    private_obj11,
    private_obj12,
    private_obj13,
    private_obj14,
    private_obj15,
    private_obj16,
    private_obj17,
    private_obj18,
    private_obj19,
    private_obj20,
    private_obj21,
    private_obj22,
    private_obj23,
    private_obj24,
    private_obj25,
    private_obj26,
    private_obj27,
    private_obj28,
    private_obj29,
    private_obj30,
	private_result,
    not_private,
};
 
 
/* NOTE - XT_LEN_PUBLIC_OBJECT_TBL must agree with value in xt_len.h */
const OM_public_object master_public_object_tbl[XT_LEN_PUBLIC_OBJECT_TBL] = {
    public_obj1,
    public_obj2,
    public_obj3,
    public_obj4,
    public_obj5,
    public_obj6,
    public_obj7,
    public_obj8,
    public_obj9,
    public_obj10,
	selection_extract
};
 
 
OM_object object_tbl[] = {
    private_obj1,
    private_obj2,
    public_obj1,
    public_obj2
};
 
/* array needed for passing to om_read(), will have string read on return */
char str[100];
 
/* NOTE - XT_LEN_STRING_TBL must agree with value in xt_len.h */
const OM_string master_string_tbl[XT_LEN_STRING_TBL] = {
    { 3, str },                 /* string3,  string of length 3    */
    { 8, str },                 /* string8,  string of length 8    */
    { 10, str },                /* string10, string of length 10   */
    { 11, str },                /* string11, string of length 11   */
    { 15, str },                /* string15, string of length 15   */
    { 31, str },                /* string31, string of length 31   */
    OM_STRING("Siemens Nixdorf"),   /* substring1                      */
    OM_STRING("ens Nixdorf"),   /* substring2                      */
    OM_STRING("SNI-test"),      /* substring3                      */
    OM_STRING("herrmann"),      /* substring4                      */
    OM_STRING("vinnie"),        /* substring5                      */
	OM_STRING("Ökölogische Überdrüß") /* substring6					   */ 
};
 

/****************************************************************************/
/*   parameter_tbl                                                          */
/****************************************************************************/
 
struct parm_tbl_struct parameter_tbl[] = {
 
/* session */
    { "default_session",    0,  XT_SESSION, NULL },
    { "session1",       	1,  XT_SESSION, NULL },
    { "session2",       	2,  XT_SESSION, NULL },
    { "session3",       	3,  XT_SESSION, NULL },
    { "session4",       	4,  XT_SESSION, NULL },
    { "session5",       	5,  XT_SESSION, NULL },
    { "session6",       	6,  XT_SESSION, NULL },
    { "session7",       	7,  XT_SESSION, NULL },
    { "session8",       	8,  XT_SESSION, NULL },
    { "session9",       	9,  XT_SESSION, NULL },
    { "bound_session1", 	10,  XT_SESSION, NULL },
    { "bound_session2",		11, XT_SESSION, NULL },
    { "bound_session3", 	12, XT_SESSION, NULL },
    { "bound_session4", 	13, XT_SESSION, NULL },
    { "bound_session5", 	14, XT_SESSION, NULL },
    { "bound_session6", 	15, XT_SESSION, NULL },
    { "bound_session7", 	16, XT_SESSION, NULL },
    { "bound_session8", 	17, XT_SESSION, NULL },
    { "bound_session9", 	18, XT_SESSION, NULL },
    { "bound_session10",    19, XT_SESSION, NULL },
    { "bound_session11",    20, XT_SESSION, NULL },
    { "bound_session12",    21, XT_SESSION, NULL },
    { "bound_session13",    22, XT_SESSION, NULL },
    { "bound_session14",    23, XT_SESSION, NULL },
    { "bound_session15",    24, XT_SESSION, NULL },
    { "bound_session16",    25, XT_SESSION, NULL },
    { "bound_session17",    26, XT_SESSION, NULL },
 
/* context */
    { "default_context",    0,   XT_CONTEXT, context_tbl },
    { "context1",       1,   XT_CONTEXT, context_tbl },
    { "context_mod1",   2,   XT_CONTEXT, context_tbl },
    { "context_mod2",   3,   XT_CONTEXT, context_tbl },
    { "context_mod3",   4,   XT_CONTEXT, context_tbl },
    { "context_mod4",   5,   XT_CONTEXT, context_tbl },
    { "context_mod5",   6,   XT_CONTEXT, context_tbl },
    { "context_mod6",   7,   XT_CONTEXT, context_tbl },
    { "context_mod7",   8,   XT_CONTEXT, context_tbl },
    { "context_mod8",   9,   XT_CONTEXT, context_tbl },
    { "context_mod9",   10,  XT_CONTEXT, context_tbl },
    { "context_mod10",  11,  XT_CONTEXT, context_tbl },
    { "context_mod11",  12,  XT_CONTEXT, context_tbl },
    { "context_mod12",  13,  XT_CONTEXT, context_tbl },
    { "context_mod13",  14,  XT_CONTEXT, context_tbl },
    { "context_mod14",  15,  XT_CONTEXT, context_tbl },
    { "context_mod15",  16,  XT_CONTEXT, context_tbl },
    { "context_mod16",  17,  XT_CONTEXT, context_tbl },
    { "context_mod17",  18,  XT_CONTEXT, context_tbl },
    { "context_mod18",  19,  XT_CONTEXT, context_tbl },
    { "context_mod19",  20,  XT_CONTEXT, context_tbl },
    { "context_mod20",  21,  XT_CONTEXT, context_tbl },
    { "context_mod21",  22,  XT_CONTEXT, context_tbl },
    { "context_mod22",  23,  XT_CONTEXT, context_tbl },
 
/* name */
    { "name0",      0,  XT_NAME,    name_tbl    },
    { "name1",      1,  XT_NAME,    name_tbl    },
    { "name2",      2,  XT_NAME,    name_tbl    },
    { "name3",      3,  XT_NAME,    name_tbl    },
    { "name4",      4,  XT_NAME,    name_tbl    },
    { "name5",      5,  XT_NAME,    name_tbl    },
    { "name6",      6,  XT_NAME,    name_tbl    },
    { "name7",      7,  XT_NAME,    name_tbl    },
    { "name8",      8,  XT_NAME,    name_tbl    },
    { "name9",      9,  XT_NAME,    name_tbl    },
    { "name10",     10, XT_NAME,    name_tbl    },
    { "name11",     11, XT_NAME,    name_tbl    },
    { "name12",     12, XT_NAME,    name_tbl    },
    { "name13",     13, XT_NAME,    name_tbl    },
    { "name14",     14, XT_NAME,    name_tbl    },
    { "name15",     15, XT_NAME,    name_tbl    },
    { "name16",     16, XT_NAME,    name_tbl    },
    { "name17",     17, XT_NAME,    name_tbl    },
    { "name18",     18, XT_NAME,    name_tbl    },
    { "name19",     19, XT_NAME,    name_tbl    },
    { "name20",     20, XT_NAME,    name_tbl    },
    { "name21",     21, XT_NAME,    name_tbl    },
    { "name22",     22, XT_NAME,    name_tbl    },
    { "name23",     23, XT_NAME,    name_tbl    },
    { "name24",     24, XT_NAME,    name_tbl    },
    { "name25",     25, XT_NAME,    name_tbl    },
    { "name26",     26, XT_NAME,    name_tbl    },
    { "name27",     27, XT_NAME,    name_tbl    },
    { "name28",     28, XT_NAME,    name_tbl    },
    { "name29",     29, XT_NAME,    name_tbl    },
    { "name30",     30, XT_NAME,    name_tbl    },
    { "name31",     31, XT_NAME,    name_tbl    },
    { "name32",     32, XT_NAME,    name_tbl    },
    { "name33",     33, XT_NAME,    name_tbl    },
    { "name34",     34, XT_NAME,    name_tbl    },
    { "name35",     35, XT_NAME,    name_tbl    },
    { "name36",     36, XT_NAME,    name_tbl    },
    { "name37",     37, XT_NAME,    name_tbl    },
    { "name38",     38, XT_NAME,    name_tbl    },
    { "name39",     39, XT_NAME,    name_tbl    },
    { "name40",     40, XT_NAME,    name_tbl    },
    { "name41",     41, XT_NAME,    name_tbl    },
    { "name42",     42, XT_NAME,    name_tbl    },
    { "name43",     43, XT_NAME,    name_tbl    },
    { "name44",     44, XT_NAME,    name_tbl    },
    { "name45",     45, XT_NAME,    name_tbl    },
    { "name46",     46, XT_NAME,    name_tbl    },
    { "name47",     47, XT_NAME,    name_tbl    },
    { "name48",     48, XT_NAME,    name_tbl    },
    { "name49",     49, XT_NAME,    name_tbl    },
    { "name50",     50, XT_NAME,    name_tbl    },
    { "name51",     51, XT_NAME,    name_tbl    },
    { "name52",     52, XT_NAME,    name_tbl    },
    { "name53",     53, XT_NAME,    name_tbl    },
    { "name54",     54, XT_NAME,    name_tbl    },
    { "name55",     55, XT_NAME,    name_tbl    },
    { "name56",     56, XT_NAME,    name_tbl    },
    { "name57",     57, XT_NAME,    name_tbl    },
    { "name58",     58, XT_NAME,    name_tbl    },
    { "name59",     59, XT_NAME,    name_tbl    },
    { "name60",     60, XT_NAME,    name_tbl    },
    { "name61",     61, XT_NAME,    name_tbl    },
    { "name62",     62, XT_NAME,    name_tbl    },
    { "name63",     63, XT_NAME,    name_tbl    },
    { "name64",     64, XT_NAME,    name_tbl    },
    { "name65",     65, XT_NAME,    name_tbl    },
    { "name66",     66, XT_NAME,    name_tbl    },
    { "name67",     67, XT_NAME,    name_tbl    },
    { "name68",     68, XT_NAME,    name_tbl    },
    { "name69",     69, XT_NAME,    name_tbl    },
    { "name70",     70, XT_NAME,    name_tbl    },
    { "name71",     71, XT_NAME,    name_tbl    },
    { "name73",     72, XT_NAME,    name_tbl    },
    { "name74",     73, XT_NAME,    name_tbl    },
    { "name75",     74, XT_NAME,    name_tbl    },
    { "name76",     75, XT_NAME,    name_tbl    },
    { "name77",     76, XT_NAME,    name_tbl    },
    { "name78",     77, XT_NAME,    name_tbl    },
    { "name79",     78, XT_NAME,    name_tbl    },
    { "name80",     79, XT_NAME,    name_tbl    },
    { "name81",     80, XT_NAME,    name_tbl    },
    { "name82",     81, XT_NAME,    name_tbl    },
    { "name83",     82, XT_NAME,    name_tbl    },
    { "name84",     83, XT_NAME,    name_tbl    },
    { "name85",     84, XT_NAME,    name_tbl    },
    { "name86",     85, XT_NAME,    name_tbl    },
    { "name87",     86, XT_NAME,    name_tbl    },
    { "name88",     87, XT_NAME,    name_tbl    },
    { "name89",     88, XT_NAME,    name_tbl    },
    { "name90",     89, XT_NAME,    name_tbl    },
    { "name91",     90, XT_NAME,    name_tbl    },
    { "name92",     91, XT_NAME,    name_tbl    },
    { "name93",     92, XT_NAME,    name_tbl    },
    { "name94",     93, XT_NAME,    name_tbl    },
    { "top_name",   94, XT_NAME,    name_tbl    },
    { "bad_name1",  95, XT_NAME,    name_tbl    },
    { "bad_name2",  96, XT_NAME,    name_tbl    },
 
/* entry */
    { "attribute_list0",    0,  XT_ENTRY,   entry_tbl   },
    { "attribute_list1",    1,  XT_ENTRY,   entry_tbl   },
    { "attribute_list2",    2,  XT_ENTRY,   entry_tbl   },
    { "attribute_list3",    3,  XT_ENTRY,   entry_tbl   },
    { "attribute_list4",    4,  XT_ENTRY,   entry_tbl   },
    { "attribute_list5",    5,  XT_ENTRY,   entry_tbl   },
    { "attribute_list6",    6,  XT_ENTRY,   entry_tbl   },
    { "attribute_list7",    7,  XT_ENTRY,   entry_tbl   },
    { "attribute_list8",    8,  XT_ENTRY,   entry_tbl   },
    { "attribute_list9",    9,  XT_ENTRY,   entry_tbl   },
    { "attribute_list10",   10, XT_ENTRY,   entry_tbl   },
    { "attribute_list11",   11, XT_ENTRY,   entry_tbl   },
    { "attribute_list12",   12, XT_ENTRY,   entry_tbl   },
    { "attribute_list13",   13, XT_ENTRY,   entry_tbl   },
    { "attribute_list14",   14, XT_ENTRY,   entry_tbl   },
    { "attribute_list15",   15, XT_ENTRY,   entry_tbl   },
    { "attribute_list16",   16, XT_ENTRY,   entry_tbl   },
    { "attribute_list17",   17, XT_ENTRY,   entry_tbl   },
    { "attribute_list18",   18, XT_ENTRY,   entry_tbl   },
    { "attribute_list19",   19, XT_ENTRY,   entry_tbl   },
    { "attribute_list20",   20, XT_ENTRY,   entry_tbl   },
    { "attribute_list21",   21, XT_ENTRY,   entry_tbl   },
    { "attribute_list22",   22, XT_ENTRY,   entry_tbl   },
    { "attribute_list23",   23, XT_ENTRY,   entry_tbl   },
    { "attribute_list24",   24, XT_ENTRY,   entry_tbl   },
    { "attribute_list25",   25, XT_ENTRY,   entry_tbl   },
    { "attribute_list26",   26, XT_ENTRY,   entry_tbl   },
    { "attribute_list27",   27, XT_ENTRY,   entry_tbl   },
    { "attribute_list28",   28, XT_ENTRY,   entry_tbl   },
    { "attribute_list29",   29, XT_ENTRY,   entry_tbl   },
    { "attribute_list30",   30, XT_ENTRY,   entry_tbl   },
    { "attribute_list31",   31, XT_ENTRY,   entry_tbl   },
    { "attribute_list32",   32, XT_ENTRY,   entry_tbl   },
    { "attribute_list33",   33, XT_ENTRY,   entry_tbl   },
    { "attribute_list34",   34, XT_ENTRY,   entry_tbl   },
    { "attribute_list35",   35, XT_ENTRY,   entry_tbl   },
    { "attribute_list36",   36, XT_ENTRY,   entry_tbl   },
    { "attribute_list37",   37, XT_ENTRY,   entry_tbl   },
    { "attribute_list38",   38, XT_ENTRY,   entry_tbl   },
    { "attribute_list39",   39, XT_ENTRY,   entry_tbl   },
    { "attribute_list40",   40, XT_ENTRY,   entry_tbl   },
    { "attribute_list41",   41, XT_ENTRY,   entry_tbl   },
    { "attribute_list42",   42, XT_ENTRY,   entry_tbl   },
    { "attribute_list43",   43, XT_ENTRY,   entry_tbl   },
    { "attribute_list44",   44, XT_ENTRY,   entry_tbl   },
    { "attribute_list45",   45, XT_ENTRY,   entry_tbl   },
    { "attribute_list46",   46, XT_ENTRY,   entry_tbl   },
    { "attribute_list47",   47, XT_ENTRY,   entry_tbl   },
    { "attribute_list48",   48, XT_ENTRY,   entry_tbl   },
    { "attribute_list49",   49, XT_ENTRY,   entry_tbl   },
    { "attribute_list50",   50, XT_ENTRY,   entry_tbl   },
    { "attribute_list51",   51, XT_ENTRY,   entry_tbl   },
    { "attribute_list52",   52, XT_ENTRY,   entry_tbl   },
    { "attribute_list53",   53, XT_ENTRY,   entry_tbl   },
    { "attribute_list54",   54, XT_ENTRY,   entry_tbl   },
    { "attribute_list55",   55, XT_ENTRY,   entry_tbl   },
    { "attribute_list56",   56, XT_ENTRY,   entry_tbl   },
    { "attribute_list57",   57, XT_ENTRY,   entry_tbl   },
    { "attribute_list58",   58, XT_ENTRY,   entry_tbl   },
    { "attribute_list59",   59, XT_ENTRY,   entry_tbl   },
    { "attribute_list60",   60, XT_ENTRY,   entry_tbl   },
    { "attribute_list61",   61, XT_ENTRY,   entry_tbl   },
    { "attribute_list62",   62, XT_ENTRY,   entry_tbl   },
    { "attribute_list63",   63, XT_ENTRY,   entry_tbl   },
    { "attribute_list64",   64, XT_ENTRY,   entry_tbl   },
    { "attribute_list65",   65, XT_ENTRY,   entry_tbl   },
    { "attribute_list66",   66, XT_ENTRY,   entry_tbl   },
    { "attribute_list67",   67, XT_ENTRY,   entry_tbl   },
    { "attribute_list68",   68, XT_ENTRY,   entry_tbl   },
    { "attribute_list69",   69, XT_ENTRY,   entry_tbl   },
 
/* ava */
    { "country1",           0,  XT_AVA, ava_tbl     },
    { "organization1",      1,  XT_AVA, ava_tbl     },
    { "organization2",      2,  XT_AVA, ava_tbl     },
    { "organization3",      3,  XT_AVA, ava_tbl     },
    { "organization8",      4,  XT_AVA, ava_tbl     },
    { "organizational_unit1",   5,  XT_AVA, ava_tbl     },
    { "organizational_unit2",   6,  XT_AVA, ava_tbl     },
    { "organizational_unit3",   7,  XT_AVA, ava_tbl     },
    { "organizational_unit4",   8,  XT_AVA, ava_tbl     },
    { "common_name1",       9,  XT_AVA, ava_tbl     },
    { "common_name2",       10,  XT_AVA, ava_tbl     },
    { "common_name3",       11, XT_AVA, ava_tbl     },
    { "common_name4",       12, XT_AVA, ava_tbl     },
    { "common_name5",       13, XT_AVA, ava_tbl     },
    { "common_name6",       14, XT_AVA, ava_tbl     },
    { "common_name16",      15, XT_AVA, ava_tbl     },
    { "common_name32",      16, XT_AVA, ava_tbl     },
    { "surname1",           17, XT_AVA, ava_tbl     },
    { "surname2",           18, XT_AVA, ava_tbl     },
    { "surname3",           19, XT_AVA, ava_tbl     },
    { "surname4",           20, XT_AVA, ava_tbl     },
    { "surname5",           21, XT_AVA, ava_tbl     },
    { "surname6",           22, XT_AVA, ava_tbl     },
    { "surname7",           23, XT_AVA, ava_tbl     },
    { "surname8",           24, XT_AVA, ava_tbl     },
    { "user_password1",     25, XT_AVA, ava_tbl     },
    { "user_password2",     26, XT_AVA, ava_tbl     },
    { "user_password3",     27, XT_AVA, ava_tbl     },
    { "user_password4",     28, XT_AVA, ava_tbl     },
    { "typeless2",          29, XT_AVA, ava_tbl     },
    { "attribute28",        30, XT_AVA, ava_tbl     },
    { "attribute29",        31, XT_AVA, ava_tbl     },
    { "attribute51",        32, XT_AVA, ava_tbl     },
    { "or_address1",        33, XT_AVA, ava_tbl     },
    { "or_address2",        34, XT_AVA, ava_tbl     },
    
/* rdn */
    { "rdn0",       0,  XT_RDN, rdn_tbl },
    { "rdn1",       1,  XT_RDN, rdn_tbl },
    { "rdn2",       2,  XT_RDN, rdn_tbl },
    { "rdn3",       3,  XT_RDN, rdn_tbl },
    { "rdn9",       4,  XT_RDN, rdn_tbl },
    { "rdn13",      5,  XT_RDN, rdn_tbl },
    { "rdn16",      6,  XT_RDN, rdn_tbl },
    { "rdn18",      7,  XT_RDN, rdn_tbl },
    { "rdn20",      8,  XT_RDN, rdn_tbl },
    { "rdn29",      9,  XT_RDN, rdn_tbl },
    { "rdn43",     10,  XT_RDN, rdn_tbl },
    { "rdn61",     11,  XT_RDN, rdn_tbl },
    { "rdn63",     12,  XT_RDN, rdn_tbl },
    { "rdn69",     13,  XT_RDN, rdn_tbl },
 
/* changes */
    { "entry_modification_list1",  0, XT_CHANGES, entry_modification_list_tbl },
    { "entry_modification_list2",  1, XT_CHANGES, entry_modification_list_tbl },
    { "entry_modification_list3",  2, XT_CHANGES, entry_modification_list_tbl },
    { "entry_modification_list4",  3, XT_CHANGES, entry_modification_list_tbl },
    { "entry_modification_list5",  4, XT_CHANGES, entry_modification_list_tbl },
    { "entry_modification_list6",  5, XT_CHANGES, entry_modification_list_tbl },
    { "entry_modification_list7",  6, XT_CHANGES, entry_modification_list_tbl },
    { "entry_modification_list8",  7, XT_CHANGES, entry_modification_list_tbl },
    { "entry_modification_list9",  8, XT_CHANGES, entry_modification_list_tbl },
    { "entry_modification_list10", 9, XT_CHANGES, entry_modification_list_tbl },
    { "entry_modification_list11",10, XT_CHANGES, entry_modification_list_tbl },
    { "entry_modification_list12",11, XT_CHANGES, entry_modification_list_tbl },
    { "entry_modification_list13",12, XT_CHANGES, entry_modification_list_tbl },
    { "entry_modification_list14",13, XT_CHANGES, entry_modification_list_tbl },
    { "entry_modification_list15",14, XT_CHANGES, entry_modification_list_tbl },
    { "entry_modification_list16",15, XT_CHANGES, entry_modification_list_tbl },
    { "entry_modification_list17",16, XT_CHANGES, entry_modification_list_tbl },
    { "entry_modification_list18",17, XT_CHANGES, entry_modification_list_tbl },
    { "entry_modification_list19",18, XT_CHANGES, entry_modification_list_tbl },
    { "entry_modification_list20",19, XT_CHANGES, entry_modification_list_tbl },
    { "entry_modification_list21",20, XT_CHANGES, entry_modification_list_tbl },
    { "entry_modification_list22",21, XT_CHANGES, entry_modification_list_tbl },
    { "entry_modification_list23",22, XT_CHANGES, entry_modification_list_tbl },
    { "entry_modification_list24",23, XT_CHANGES, entry_modification_list_tbl },
    { "entry_modification_list25",24, XT_CHANGES, entry_modification_list_tbl },
    { "entry_modification_list26",25, XT_CHANGES, entry_modification_list_tbl },
    { "entry_modification_list27",26, XT_CHANGES, entry_modification_list_tbl },
    { "entry_modification_list28",27, XT_CHANGES, entry_modification_list_tbl },
    { "entry_modification_list29",28, XT_CHANGES, entry_modification_list_tbl },
    { "entry_modification_list30",29, XT_CHANGES, entry_modification_list_tbl },
 
/* filter */
    { "no_filter",  0,  XT_FILTER,  filter_tbl  },
    { "filter1",    1,  XT_FILTER,  filter_tbl  },
    { "filter2",    2,  XT_FILTER,  filter_tbl  },
    { "filter3",    3,  XT_FILTER,  filter_tbl  },
    { "filter4",    4,  XT_FILTER,  filter_tbl  },
    { "filter5",    5,  XT_FILTER,  filter_tbl  },
    { "filter6",    6,  XT_FILTER,  filter_tbl  },
    { "filter7",    7,  XT_FILTER,  filter_tbl  },
    { "filter8",    8,  XT_FILTER,  filter_tbl  },
    { "filter9",    9,  XT_FILTER,  filter_tbl  },
    { "filter10",  10,  XT_FILTER,  filter_tbl  },
    { "filter11",  11,  XT_FILTER,  filter_tbl  },
    { "filter12",  12,  XT_FILTER,  filter_tbl  },
    { "filter13",  13,  XT_FILTER,  filter_tbl  },
    { "filter14",  14,  XT_FILTER,  filter_tbl  },
 
/* selection */
    { "entry_info_selection0",  0,  XT_SELECTION,   entry_info_selection_tbl},
    { "entry_info_selection1",  1,  XT_SELECTION,   entry_info_selection_tbl},
    { "entry_info_selection2",  2,  XT_SELECTION,   entry_info_selection_tbl},
    { "entry_info_selection3",  3,  XT_SELECTION,   entry_info_selection_tbl},
    { "entry_info_selection4",  4,  XT_SELECTION,   entry_info_selection_tbl},
    { "entry_info_selection5",  5,  XT_SELECTION,   entry_info_selection_tbl},
    { "entry_info_selection6",  6,  XT_SELECTION,   entry_info_selection_tbl},
    { "entry_info_selection7",  7,  XT_SELECTION,   entry_info_selection_tbl},
    { "entry_info_selection8",  8,  XT_SELECTION,   entry_info_selection_tbl},
    { "entry_info_selection9",  9,  XT_SELECTION,   entry_info_selection_tbl},
    { "entry_info_selection10", 10, XT_SELECTION,   entry_info_selection_tbl},
    { "entry_info_selection11", 11, XT_SELECTION,   entry_info_selection_tbl},
    { "entry_info_selection12", 12, XT_SELECTION,   entry_info_selection_tbl},
    { "entry_info_selection13", 13, XT_SELECTION,   entry_info_selection_tbl},
    { "entry_info_selection14", 14, XT_SELECTION,   entry_info_selection_tbl},
    { "entry_info_selection15", 15, XT_SELECTION,   entry_info_selection_tbl},
    { "entry_info_selection16", 16, XT_SELECTION,   entry_info_selection_tbl},
 
/* bdcp */
    { "fax_number1",       0,  XT_BDCP,    bdcp_tbl},
    { "postal_address1",   1,  XT_BDCP,    bdcp_tbl},
    { "search_guide1",     2,  XT_BDCP,    bdcp_tbl},
    { "teletex_terminal_id1",  3,  XT_BDCP,    bdcp_tbl},
    { "telex_number1",     4,  XT_BDCP,    bdcp_tbl},
 
/* DS_status */
    { "DS_SUCCESS",     0,  XT_STATUS,  ((OM_descriptor **) NULL)},
    { "abandon_failed1",    1,  XT_STATUS,  ((OM_descriptor **) NULL)},
    { "attribute_error1",   2,  XT_STATUS,  ((OM_descriptor **) NULL)},
    { "attribute_error2",   3,  XT_STATUS,  ((OM_descriptor **) NULL)},
    { "attribute_error3",   4,  XT_STATUS,  ((OM_descriptor **) NULL)},
    { "attribute_error4",   5,  XT_STATUS,  ((OM_descriptor **) NULL)},
    { "attribute_error5",   6,  XT_STATUS,  ((OM_descriptor **) NULL)},
    { "attribute_error6",   7,  XT_STATUS,  ((OM_descriptor **) NULL)},
    { "attribute_error7",   8,  XT_STATUS,  ((OM_descriptor **) NULL)},
    { "attribute_error8",   9,  XT_STATUS,  ((OM_descriptor **) NULL)},
    { "attribute_error9",   10, XT_STATUS,  ((OM_descriptor **) NULL)},
    { "attribute_error10",  11, XT_STATUS,  ((OM_descriptor **) NULL)},
    { "attribute_error11",  12, XT_STATUS,  ((OM_descriptor **) NULL)},
    { "attribute_error12",  13, XT_STATUS,  ((OM_descriptor **) NULL)},
    { "attribute_error13",  14, XT_STATUS,  ((OM_descriptor **) NULL)},
    { "attribute_error14",  15, XT_STATUS,  ((OM_descriptor **) NULL)},
    { "attribute_error15",  16, XT_STATUS,  ((OM_descriptor **) NULL)},
    { "attribute_error16",  17, XT_STATUS,  ((OM_descriptor **) NULL)},
    { "attribute_error17",  18, XT_STATUS,  ((OM_descriptor **) NULL)},
    { "attribute_error18",  19, XT_STATUS,  ((OM_descriptor **) NULL)},
    { "attribute_error19",  20, XT_STATUS,  ((OM_descriptor **) NULL)},
    { "communications_error1",  21,  XT_STATUS,  ((OM_descriptor **) NULL)},
    { "library_error1", 22, XT_STATUS,  ((OM_descriptor **) NULL)},
    { "library_error2", 23, XT_STATUS,  ((OM_descriptor **) NULL)},
    { "library_error3", 24, XT_STATUS,  ((OM_descriptor **) NULL)},
    { "library_error4", 25, XT_STATUS,  ((OM_descriptor **) NULL)},
    { "library_error5", 26, XT_STATUS,  ((OM_descriptor **) NULL)},
    { "library_error6", 27, XT_STATUS,  ((OM_descriptor **) NULL)},
    { "library_error7", 28, XT_STATUS,  ((OM_descriptor **) NULL)},
    { "library_error8", 29, XT_STATUS,  ((OM_descriptor **) NULL)},
    { "library_error9", 30, XT_STATUS,  ((OM_descriptor **) NULL)},
    { "library_error10",    31, XT_STATUS,  ((OM_descriptor **) NULL)},
    { "name_error1",    32, XT_STATUS,  ((OM_descriptor **) NULL)},
    { "name_error2",    33, XT_STATUS,  ((OM_descriptor **) NULL)},
    { "name_error3",    34, XT_STATUS,  ((OM_descriptor **) NULL)},
    { "name_error4",    35, XT_STATUS,  ((OM_descriptor **) NULL)},
    { "name_error5",    36, XT_STATUS,  ((OM_descriptor **) NULL)},
    { "name_error6",    37, XT_STATUS,  ((OM_descriptor **) NULL)},
    { "name_error7",    38, XT_STATUS,  ((OM_descriptor **) NULL)},
    { "name_error8",    39, XT_STATUS,  ((OM_descriptor **) NULL)},
    { "referral1",      40, XT_STATUS,  ((OM_descriptor **) NULL)},
    { "security_error1",    41, XT_STATUS,  ((OM_descriptor **) NULL)},
    { "security_error2",    42, XT_STATUS,  ((OM_descriptor **) NULL)},
    { "service_error1", 43, XT_STATUS,  ((OM_descriptor **) NULL)},
    { "service_error2", 44, XT_STATUS,  ((OM_descriptor **) NULL)},
    { "service_error3", 45, XT_STATUS,  ((OM_descriptor **) NULL)},
    { "service_error4", 46, XT_STATUS,  ((OM_descriptor **) NULL)},
    { "service_error5", 47, XT_STATUS,  ((OM_descriptor **) NULL)},
    { "system_error1",  48, XT_STATUS,  ((OM_descriptor **) NULL)},
    { "update_error1",  49, XT_STATUS,  ((OM_descriptor **) NULL)},
    { "update_error2",  50, XT_STATUS,  ((OM_descriptor **) NULL)},
    { "update_error3",  51, XT_STATUS,  ((OM_descriptor **) NULL)},
    { "update_error4",  52, XT_STATUS,  ((OM_descriptor **) NULL)},
    { "update_error5",  53, XT_STATUS,  ((OM_descriptor **) NULL)},

/* compare_result */
    { "compare_result_return1", 0,  XT_COMPARE_RESULT, compare_result_tbl },
    { "compare_result_return2", 1,  XT_COMPARE_RESULT, compare_result_tbl },
    { "compare_result_return3", 2,  XT_COMPARE_RESULT, compare_result_tbl },
 
/* read_result */
    { "read_result_return1",    0,  XT_READ_RESULT, read_result_tbl },
    { "read_result_return2",    1,  XT_READ_RESULT, read_result_tbl },
    { "read_result_return3",    2,  XT_READ_RESULT, read_result_tbl },
    { "read_result_return4",    3,  XT_READ_RESULT, read_result_tbl },
    { "read_result_return5",    4,  XT_READ_RESULT, read_result_tbl },
    { "read_result_return6",    5,  XT_READ_RESULT, read_result_tbl },
    { "read_result_return7",    6,  XT_READ_RESULT, read_result_tbl },
    { "read_result_return8",    7,  XT_READ_RESULT, read_result_tbl },
    { "read_result_return9",    8,  XT_READ_RESULT, read_result_tbl },
    { "read_result_return10",   9,  XT_READ_RESULT, read_result_tbl },
    { "read_result_return11",   10, XT_READ_RESULT, read_result_tbl },
    { "read_result_return12",   11, XT_READ_RESULT, read_result_tbl },
    { "read_result_return13",   12, XT_READ_RESULT, read_result_tbl },
    { "read_result_return14",   13, XT_READ_RESULT, read_result_tbl },
    { "read_result_return15",   14, XT_READ_RESULT, read_result_tbl },
    { "read_result_return16",   15, XT_READ_RESULT, read_result_tbl },
    { "read_result_return17",   16, XT_READ_RESULT, read_result_tbl },
    { "read_result_return18",   17, XT_READ_RESULT, read_result_tbl },
    { "read_result_return19",   18, XT_READ_RESULT, read_result_tbl },
    { "read_result_return20",   19, XT_READ_RESULT, read_result_tbl },
    { "read_result_return21",   20, XT_READ_RESULT, read_result_tbl },
    { "read_result_return22",   21, XT_READ_RESULT, read_result_tbl },
    { "read_result_return23",   22, XT_READ_RESULT, read_result_tbl },
    { "read_result_return24",   23, XT_READ_RESULT, read_result_tbl },
    { "read_result_return25",   24, XT_READ_RESULT, read_result_tbl },
    { "read_result_return26",   25, XT_READ_RESULT, read_result_tbl },
    { "read_result_return27",   26, XT_READ_RESULT, read_result_tbl },
    { "read_result_return28",   27, XT_READ_RESULT, read_result_tbl },
    { "read_result_return29",   28, XT_READ_RESULT, read_result_tbl },
    { "read_result_return30",   29, XT_READ_RESULT, read_result_tbl },
    { "read_result_return31",   30, XT_READ_RESULT, read_result_tbl },
    { "read_result_return32",   31, XT_READ_RESULT, read_result_tbl },
    { "read_result_return33",   32, XT_READ_RESULT, read_result_tbl },
    { "read_result_return34",   33, XT_READ_RESULT, read_result_tbl },
    { "read_result_return35",   34, XT_READ_RESULT, read_result_tbl },
    { "read_result_return36",   35, XT_READ_RESULT, read_result_tbl },
    { "read_result_return37",   36, XT_READ_RESULT, read_result_tbl },
    { "read_result_return38",   37, XT_READ_RESULT, read_result_tbl },
    { "read_result_return39",   38, XT_READ_RESULT, read_result_tbl },
    { "read_result_return40",   39, XT_READ_RESULT, read_result_tbl },
    { "read_result_return41",   40, XT_READ_RESULT, read_result_tbl },
    { "read_result_return42",   41, XT_READ_RESULT, read_result_tbl },
    { "read_result_return43",   42, XT_READ_RESULT, read_result_tbl },
    { "read_result_return44",   43, XT_READ_RESULT, read_result_tbl },
    { "read_result_return45",   44, XT_READ_RESULT, read_result_tbl },
    { "read_result_return46",   45, XT_READ_RESULT, read_result_tbl },
    { "read_result_return47",   46, XT_READ_RESULT, read_result_tbl },
    { "read_result_return48",   47, XT_READ_RESULT, read_result_tbl },
    { "read_result_return49",   48, XT_READ_RESULT, read_result_tbl },
    { "read_result_return50",   49, XT_READ_RESULT, read_result_tbl },
    { "read_result_return51",   50, XT_READ_RESULT, read_result_tbl },
    { "read_result_return52",   51, XT_READ_RESULT, read_result_tbl },
    { "read_result_return53",   52, XT_READ_RESULT, read_result_tbl },
    { "read_result_return54",   53, XT_READ_RESULT, read_result_tbl },
    { "read_result_return55",   54, XT_READ_RESULT, read_result_tbl },
    { "read_result_return56",   55, XT_READ_RESULT, read_result_tbl },
    { "read_result_return57",   56, XT_READ_RESULT, read_result_tbl },
    { "read_result_return58",   57, XT_READ_RESULT, read_result_tbl },
    { "read_result_return59",   58, XT_READ_RESULT, read_result_tbl },
    { "read_result_return60",   59, XT_READ_RESULT, read_result_tbl },
    { "read_result_return61",   60, XT_READ_RESULT, read_result_tbl },
 
/* list_result */
    { "list_result_return1",    0,  XT_LIST_RESULT, list_result_tbl },
    { "list_result_return2",    1,  XT_LIST_RESULT, list_result_tbl },
    { "list_result_return3",    2,  XT_LIST_RESULT, list_result_tbl },
    { "list_result_return4",    3,  XT_LIST_RESULT, list_result_tbl },
    { "list_result_return5",    4,  XT_LIST_RESULT, list_result_tbl },
    { "list_result_return6",    5,  XT_LIST_RESULT, list_result_tbl },
    { "list_result_return7",    6,  XT_LIST_RESULT, list_result_tbl },
    { "list_result_return8",    7,  XT_LIST_RESULT, list_result_tbl },
    { "list_result_return9",    8,  XT_LIST_RESULT, list_result_tbl },
    { "list_result_return10",   9,  XT_LIST_RESULT, list_result_tbl },
    { "list_result_return11",  10,  XT_LIST_RESULT, list_result_tbl },
    { "list_result_return12",  11,  XT_LIST_RESULT, list_result_tbl },
    { "list_result_return13",  12,  XT_LIST_RESULT, list_result_tbl },
 
/* search_result */
    { "search_result_return1",  0,  XT_SEARCH_RESULT, search_result_tbl },
    { "search_result_return2",  1,  XT_SEARCH_RESULT, search_result_tbl },
    { "search_result_return3",  2,  XT_SEARCH_RESULT, search_result_tbl },
    { "search_result_return4",  3,  XT_SEARCH_RESULT, search_result_tbl },
    { "search_result_return5",  4,  XT_SEARCH_RESULT, search_result_tbl },
    { "search_result_return6",  5,  XT_SEARCH_RESULT, search_result_tbl },
    { "search_result_return7",  6,  XT_SEARCH_RESULT, search_result_tbl },
    { "search_result_return8",  7,  XT_SEARCH_RESULT, search_result_tbl },
    { "search_result_return9",  8,  XT_SEARCH_RESULT, search_result_tbl },
    { "search_result_return10", 9,  XT_SEARCH_RESULT, search_result_tbl },
    { "search_result_return11",10,  XT_SEARCH_RESULT, search_result_tbl },
    { "search_result_return12",11,  XT_SEARCH_RESULT, search_result_tbl },
    { "search_result_return13",12,  XT_SEARCH_RESULT, search_result_tbl },
 
 
/* feature_list */
    { "feature_list0",  0,  XT_FEATURE_LIST, ((OM_descriptor **) NULL)},
    { "feature_list1",  1,  XT_FEATURE_LIST, ((OM_descriptor **) NULL)},
    { "feature_list2",  2,  XT_FEATURE_LIST, ((OM_descriptor **) NULL)},
    { "feature_list3",  3,  XT_FEATURE_LIST, ((OM_descriptor **) NULL)},
    { "feature_list4",  4,  XT_FEATURE_LIST, ((OM_descriptor **) NULL)},
    { "feature_list5",  5,  XT_FEATURE_LIST, ((OM_descriptor **) NULL)},
 
/* workspace */
    { "NULL",       0,  XT_WORKSPACE,    ((OM_descriptor **) NULL)},
    { "workspace1", 1,  XT_WORKSPACE,    ((OM_descriptor **) NULL)},
    { "workspace2", 2,  XT_WORKSPACE,    ((OM_descriptor **) NULL)},
 
/* rules */
    { "rules1",     0,  XT_RULES,    ((OM_descriptor **) NULL)},
 
/* boolEan - delete_old_RDN, search_aliases, initialize */
    { "TRUE",       0,  XT_BOOLEAN,  ((OM_descriptor **) NULL)},
    { "FALSE",      1,  XT_BOOLEAN,  ((OM_descriptor **) NULL)},
 
/* subset */
    { "DS_BASE_OBJECT",  0, XT_SUBSET,   ((OM_descriptor **) NULL)},
    { "DS_ONE_LEVEL",    1, XT_SUBSET,   ((OM_descriptor **) NULL)},
    { "DS_WHOLE_SUBTREE",2, XT_SUBSET,   ((OM_descriptor **) NULL)},
 
/* class */
    { "DS_C_ABANDON_FAILED",        0,  XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_ACCESS_POINT",          1,  XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_ADDRESS",               2,  XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_ATTRIBUTE",             3,  XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_ATTRIBUTE_ERROR",       4,  XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_ATTRIBUTE_LIST",        5,  XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_ATTRIBUTE_PROBLEM",     6,  XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_AVA",                   7,  XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_COMMON_RESULTS",        8,  XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_COMMUNICATIONS_ERROR",  9,  XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_COMPARE_RESULT",        10, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_CONTEXT",               11, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_CONTINUATION_REF",    	12, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_DS_DN",                 13, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_DS_RDN",                14, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_ENTRY_INFO",     		15, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_ENTRY_INFO_SELECTION",	16,XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_ENTRY_MOD",    			17, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_ENTRY_MOD_LIST",   		18, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_ERROR",                 19, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_EXT",             		20, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_FILTER",                21, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_FILTER_ITEM",           22, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_LIBRARY_ERROR",         23, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_LIST_INFO",             24, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_LIST_INFO_ITEM",        25, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_LIST_RESULT",           26, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_NAME",                  27, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_NAME_ERROR",            28, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_OPERATION_PROGRESS",    29, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_PARTIAL_OUTCOME_QUAL",	30, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_PRESENTATION_ADDRESS",  31, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_READ_RESULT",           32, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_REFERRAL",              33, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_RELATIVE_NAME",         34, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_SEARCH_INFO",           35, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_SEARCH_RESULT",         36, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_SECURITY_ERROR",        37, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_SERVICE_ERROR",         38, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_SESSION",               39, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_SYSTEM_ERROR",          40, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_UPDATE_ERROR",          41, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DSX_C_GDS_SESSION",          42, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DSX_C_GDS_CONTEXT",          43, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DSX_C_GDS_ACL",              44, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DSX_C_GDS_ACL_ITEM",         45, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_FACSIMILE_PHONE_NBR",	46, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_POSTAL_ADDRESS",        47, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_SEARCH_CRITERION",      47, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_SEARCH_GUIDE",          49, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_TELETEX_TERM_IDENT",	50,XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_TELEX_NBR",          	51, XT_CLASS, ((OM_descriptor **) NULL)},
    { "OM_C_ENCODING",              52, XT_CLASS, ((OM_descriptor **) NULL)},
    { "OM_C_EXTERNAL",              53, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_ALGORITHM_IDENT",       54, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_CERT",              	55, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_CERT_LIST",             56, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_CERT_PAIR",             57, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_CERT_SUBLIST",          58, XT_CLASS, ((OM_descriptor **) NULL)},
    { "DS_C_SIGNATURE",             59, XT_CLASS, ((OM_descriptor **) NULL)},
    { "INVALID_CLASS",              60, XT_CLASS, ((OM_descriptor **) NULL)},
 
/* attr type */

	{ "OMP_O_DS_A_ALIASED_OBJECT_NAME",		0,	XT_ATTR_TYPE, ((OM_descriptor **)NULL)},
	{ "OMP_O_DS_A_BUSINESS_CATEGORY",		1,	XT_ATTR_TYPE, ((OM_descriptor **)NULL)},
	{ "OMP_O_DS_A_COMMON_NAME",				2,	XT_ATTR_TYPE, ((OM_descriptor **)NULL)},
	{ "OMP_O_DS_A_COUNTRY_NAME",			3,	XT_ATTR_TYPE, ((OM_descriptor **)NULL)},
	{ "OMP_O_DS_A_DESCRIPTION",				4,	XT_ATTR_TYPE, ((OM_descriptor **)NULL)},
	{ "OMP_O_DS_A_DEST_INDICATOR",			5,	XT_ATTR_TYPE, ((OM_descriptor **)NULL)},
	{ "OMP_O_DS_A_FACSIMILE_PHONE_NBR",		6,	XT_ATTR_TYPE, ((OM_descriptor **)NULL)},
	{ "OMP_O_DS_A_INTERNAT_ISDN_NBR",		7,	XT_ATTR_TYPE, ((OM_descriptor **)NULL)},
	{ "OMP_O_DS_A_KNOWLEDGE_INFO",			8,	XT_ATTR_TYPE, ((OM_descriptor **)NULL)},
	{ "OMP_O_DS_A_LOCALITY_NAME",			9,	XT_ATTR_TYPE, ((OM_descriptor **)NULL)},
	{ "OMP_O_DS_A_MEMBER",					10,	XT_ATTR_TYPE, ((OM_descriptor **)NULL)},
	{ "OMP_O_DS_A_OBJECT_CLASS",			11,	XT_ATTR_TYPE, ((OM_descriptor **)NULL)},
	{ "OMP_O_DS_A_ORG_NAME",				12,	XT_ATTR_TYPE, ((OM_descriptor **)NULL)},
	{ "OMP_O_DS_A_ORG_UNIT_NAME",			13,	XT_ATTR_TYPE, ((OM_descriptor **)NULL)},
	{ "OMP_O_DS_A_OWNER",					14,	XT_ATTR_TYPE, ((OM_descriptor **)NULL)},
	{ "OMP_O_DS_A_PHONE_NBR",				15,	XT_ATTR_TYPE, ((OM_descriptor **)NULL)},
	{ "OMP_O_DS_A_PHYS_DELIV_OFF_NAME",		16,	XT_ATTR_TYPE, ((OM_descriptor **)NULL)},
	{ "OMP_O_DS_A_POST_OFFICE_BOX",			17,	XT_ATTR_TYPE, ((OM_descriptor **)NULL)},
	{ "OMP_O_DS_A_POSTAL_ADDRESS",			18,	XT_ATTR_TYPE, ((OM_descriptor **)NULL)},
	{ "OMP_O_DS_A_POSTAL_CODE",				19,	XT_ATTR_TYPE, ((OM_descriptor **)NULL)},
	{ "OMP_O_DS_A_PREF_DELIV_METHOD",		20,	XT_ATTR_TYPE, ((OM_descriptor **)NULL)},
	{ "OMP_O_DS_A_PRESENTATION_ADDRESS",	21,	XT_ATTR_TYPE, ((OM_descriptor **)NULL)},
	{ "OMP_O_DS_A_REGISTERED_ADDRESS",		22,	XT_ATTR_TYPE, ((OM_descriptor **)NULL)},
	{ "OMP_O_DS_A_ROLE_OCCUPANT",			23,	XT_ATTR_TYPE, ((OM_descriptor **)NULL)},
	{ "OMP_O_DS_A_SEARCH_GUIDE",			24,	XT_ATTR_TYPE, ((OM_descriptor **)NULL)},
	{ "OMP_O_DS_A_SEE_ALSO",				25,	XT_ATTR_TYPE, ((OM_descriptor **)NULL)},
	{ "OMP_O_DS_A_SERIAL_NBR",				26,	XT_ATTR_TYPE, ((OM_descriptor **)NULL)},
	{ "OMP_O_DS_A_STATE_OR_PROV_NAME",		27,	XT_ATTR_TYPE, ((OM_descriptor **)NULL)},
	{ "OMP_O_DS_A_STREET_ADDRESS",			28,	XT_ATTR_TYPE, ((OM_descriptor **)NULL)},
	{ "OMP_O_DS_A_SUPPORT_APPLIC_CONTEXT",	29,	XT_ATTR_TYPE, ((OM_descriptor **)NULL)},
	{ "OMP_O_DS_A_SURNAME",					30,	XT_ATTR_TYPE, ((OM_descriptor **)NULL)},
	{ "OMP_O_DS_A_TELETEX_TERM_IDENT",		31,	XT_ATTR_TYPE, ((OM_descriptor **)NULL)},
	{ "OMP_O_DS_A_TELEX_NBR",				32,	XT_ATTR_TYPE, ((OM_descriptor **)NULL)},
	{ "OMP_O_DS_A_TITLE",					33,	XT_ATTR_TYPE, ((OM_descriptor **)NULL)},
	{ "OMP_O_DS_A_USER_PASSWORD",			34,	XT_ATTR_TYPE, ((OM_descriptor **)NULL)},
	{ "OMP_O_DS_A_X121_ADDRESS",			35,	XT_ATTR_TYPE, ((OM_descriptor **)NULL)},
/* type */
    { "OM_CLASS",                   0,   XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_ACCESS_POINTS",           1,   XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_ADDRESS",                 2,   XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_AE_TITLE",                3,   XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_ALIASED_RDNS",            4,   XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_ALIAS_DEREFERENCED",      5,   XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_ALIAS_ENTRY",             6,   XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_ALL_ATTRIBUTES",          7,   XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_ASYNCHRONOUS",            8,   XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_ATTRIBUTES",              9,   XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_ATTRIBUTES_SELECTED",     10,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_ATTRIBUTE_TYPE",          11,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_ATTRIBUTE_VALUE",         12,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_ATTRIBUTE_VALUES",        13,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_AUTOMATIC_CONTINUATION",  14,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_AVAS",                    15,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_CHAINING_PROHIB",     16,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_CHANGES",                 17,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_CRIT",                18,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_DONT_DEREFERENCE_ALIASES",    19,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_DONT_USE_COPY",           20,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_DSA_ADDRESS",             21,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_DSA_NAME",                22,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_ENTRIES",                 23,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_ENTRY",                   24,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_EXT",              25,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_FILE_DESCRIPTOR",         26,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_FILTERS",                 27,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_FILTER_ITEMS",            28,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_FILTER_ITEM_TYPE",        29,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_FILTER_TYPE",             30,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_FINAL_SUBSTRING",         31,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_FROM_ENTRY",              32,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_IDENT",              33,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_INFO_TYPE",        34,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_INITIAL_SUBSTRING",       35,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_ITEM_PARAMETERS",         36,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_LIMIT_PROBLEM",           37,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_LIST_INFO",               38,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_LOCAL_SCOPE",             39,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_MATCHED",                 40,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_MOD_TYPE",       41,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_NAME_RESOLUTION_PHASE",   42,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_NEXT_RDN_TO_BE_RESOLVED", 43,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_N_ADDRESSES",             44,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_OBJECT_NAME",             45,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_OPERATION_PROGRESS",      46,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_PARTIAL_OUTCOME_QUAL",   47,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_PERFORMER",               48,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_PREFER_CHAINING",         49,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_PRIORITY",                50,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_PROBLEM",                 51,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_PROBLEMS",                52,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_P_SELECTOR",              53,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_RDN",                     54,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_RDNS",                    55,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_RDNS_RESOLVED",           56,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_REQUESTOR",               57,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_SCOPE_OF_REFERRAL",       58,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_SEARCH_INFO",             59,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_SIZE_LIMIT",              60,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_SUBORDINATES",            61,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_S_SELECTOR",              62,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_TARGET_OBJECT",           63,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_TIME_LIMIT",              64,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_T_SELECTOR",              65,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_UNAVAILABLE_CRIT_EXT",66,XT_TYPE,((OM_descriptor **)NULL)},
    { "DS_UNCORRELATED_LIST_INFO",  67,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_UNCORRELATED_SEARCH_INFO",    68,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DS_UNEXPLORED",              69,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DSX_PASSWORD",               70,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DSX_DIR_ID",                 71,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DSX_DUAFIRST",               72,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DSX_DONT_STORE",             73,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DSX_NORMAL_CLASS",           74,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DSX_PRIV_CLASS",             75,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DSX_RESIDENT_CLASS",         76,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DSX_USEDSA",                 77,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "DSX_DUA_CACHE",              78,  XT_TYPE, ((OM_descriptor **)NULL)},
    { "INVALID_TYPE",               79,  XT_TYPE, ((OM_descriptor **)NULL)},
 
/* exclusions */
    {"OM_NO_EXCLUSIONS",      0, XT_EXCLUSIONS,   ((OM_descriptor **) NULL)},
    {"OM_EXCLUDE_ALL_BUT_THESE_TYPES", 1, XT_EXCLUSIONS, ((OM_descriptor **) NULL)},
    {"OM_EXCLUDE_ALL_BUT_THESE_VALUES",2, XT_EXCLUSIONS, ((OM_descriptor **) NULL)},
    {"OM_EXCLUDE_MULTIPLES",  3, XT_EXCLUSIONS,   ((OM_descriptor **) NULL)},
    {"OM_EXCLUDE_SUBOBJECTS", 4, XT_EXCLUSIONS,   ((OM_descriptor **) NULL)},
    {"OM_EXCLUDE_VALUES",     5, XT_EXCLUSIONS,   ((OM_descriptor **) NULL)},
    {"OM_EXCLUDE_DESCRIPTORS" ,   6, XT_EXCLUSIONS,   ((OM_descriptor **) NULL)},
    {"INVALID_EXCLUSION",     7, XT_EXCLUSIONS,   ((OM_descriptor **) NULL)},
 
/* modification */
    {"OM_INSERT_AT_BEGINNING",    0, XT_MODIFICATION, ((OM_descriptor **) NULL)},
    {"OM_INSERT_AT_CERTAIN_POINT",1, XT_MODIFICATION, ((OM_descriptor **) NULL)},
    {"OM_INSERT_AT_END",      2, XT_MODIFICATION, ((OM_descriptor **) NULL)},
    {"OM_REPLACE_ALL",        3, XT_MODIFICATION, ((OM_descriptor **) NULL)},
    {"OM_REPLACE_CERTAIN_VALUES", 4, XT_MODIFICATION, ((OM_descriptor **) NULL)},
    {"INVALID_MODIFICATION",  5, XT_MODIFICATION, ((OM_descriptor **) NULL)},
 
/* syntax */
    {"OM_S_NO_MORE_SYNTAXES",    0,   XT_SYNTAX, ((OM_descriptor **) NULL)},
    {"OM_S_BIT_STRING",          1,   XT_SYNTAX, ((OM_descriptor **) NULL)},
    {"OM_S_BOOLEAN",             2,   XT_SYNTAX, ((OM_descriptor **) NULL)},
    {"OM_S_ENCODING_STRING",     3,   XT_SYNTAX, ((OM_descriptor **) NULL)},
    {"OM_S_ENUMERATION",         4,   XT_SYNTAX, ((OM_descriptor **) NULL)},
    {"OM_S_GENERAL_STRING",      5,   XT_SYNTAX, ((OM_descriptor **) NULL)},
    {"OM_S_GENERALISED_TIME_STRING", 6,   XT_SYNTAX, ((OM_descriptor **) NULL)},
    {"OM_S_GRAPHIC_STRING",      7,   XT_SYNTAX, ((OM_descriptor **) NULL)},
    {"OM_S_IA5_STRING",          8,   XT_SYNTAX, ((OM_descriptor **) NULL)},
    {"OM_S_INTEGER",             9,   XT_SYNTAX, ((OM_descriptor **) NULL)},
    {"OM_S_NULL",                10,  XT_SYNTAX, ((OM_descriptor **) NULL)},
    {"OM_S_NUMERIC_STRING",      11,  XT_SYNTAX, ((OM_descriptor **) NULL)},
    {"OM_S_OBJECT",              12,  XT_SYNTAX, ((OM_descriptor **) NULL)},
    {"OM_S_OBJECT_DESCRIPTOR_STRING",13,  XT_SYNTAX, ((OM_descriptor **) NULL)},
    {"OM_S_OBJECT_IDENTIFIER_STRING",14,  XT_SYNTAX, ((OM_descriptor **) NULL)},
    {"OM_S_OCTET_STRING",        15,  XT_SYNTAX, ((OM_descriptor **) NULL)},
    {"OM_S_PRINTABLE_STRING",    16,  XT_SYNTAX, ((OM_descriptor **) NULL)},
    {"OM_S_TELETEX_STRING",      17,  XT_SYNTAX, ((OM_descriptor **) NULL)},
    {"OM_S_UTC_TIME_STRING",     18,  XT_SYNTAX, ((OM_descriptor **) NULL)},
    {"OM_S_VIDEOTEX_STRING",     19,  XT_SYNTAX, ((OM_descriptor **) NULL)},
    {"OM_S_VISIBLE_STRING",      20,  XT_SYNTAX, ((OM_descriptor **) NULL)},
    {"OM_S_LONG_STRING",         21,  XT_SYNTAX, ((OM_descriptor **) NULL)},
    {"OM_S_NO_VALUE",            22,  XT_SYNTAX, ((OM_descriptor **) NULL)},
    {"OM_S_LOCAL_STRING",        23,  XT_SYNTAX, ((OM_descriptor **) NULL)},
    {"OM_S_SERVICE_GENERATED",   24,  XT_SYNTAX, ((OM_descriptor **) NULL)},
    {"OM_S_PRIVATE",             25,  XT_SYNTAX, ((OM_descriptor **) NULL)},
    {"OM_S_SYNTAX",              26,  XT_SYNTAX, ((OM_descriptor **) NULL)},
    {"INVALID_SYNTAX",           27,  XT_SYNTAX, ((OM_descriptor **) NULL)},
 
/* type list */
    {"type_list1",     0,  XT_TYPE_LIST,  ((OM_descriptor **) NULL)},
    {"type_list2",     1,  XT_TYPE_LIST,  ((OM_descriptor **) NULL)},
    {"type_list3",     2,  XT_TYPE_LIST,  ((OM_descriptor **) NULL)},
    {"type_list4",     3,  XT_TYPE_LIST,  ((OM_descriptor **) NULL)},
    {"type_list5",     4,  XT_TYPE_LIST,  ((OM_descriptor **) NULL)},
    {"type_list6",     5,  XT_TYPE_LIST,  ((OM_descriptor **) NULL)},
    {"type_list7",     6,  XT_TYPE_LIST,  ((OM_descriptor **) NULL)},
    {"type_list8",     7,  XT_TYPE_LIST,  ((OM_descriptor **) NULL)},
    {"type_list9",     8,  XT_TYPE_LIST,  ((OM_descriptor **) NULL)},
 
/* navigation path */
    {"navigation_path1",   0,  XT_NAV_PATH,  ((OM_descriptor **) NULL)},
    {"navigation_path2",   1,  XT_NAV_PATH,  ((OM_descriptor **) NULL)},
    {"navigation_path3",   2,  XT_NAV_PATH,  ((OM_descriptor **) NULL)},
    {"navigation_path4",   3,  XT_NAV_PATH,  ((OM_descriptor **) NULL)},
    {"navigation_path5",   4,  XT_NAV_PATH,  ((OM_descriptor **) NULL)},
    {"navigation_path6",   5,  XT_NAV_PATH,  ((OM_descriptor **) NULL)},
    {"navigation_path7",   6,  XT_NAV_PATH,  ((OM_descriptor **) NULL)},
    {"navigation_path8",   7,  XT_NAV_PATH,  ((OM_descriptor **) NULL)},

/* OM_return_code */
    { "OM_SUCCESS",            0, XT_RETURN_CODE, ((OM_descriptor **) NULL)},
    { "OM_ENCODING_INVALID",   1, XT_RETURN_CODE, ((OM_descriptor **) NULL)},
    { "OM_FUNCTION_DECLINED",  2, XT_RETURN_CODE, ((OM_descriptor **) NULL)},
    { "OM_FUNCTION_INTERRUPTED",   3, XT_RETURN_CODE, ((OM_descriptor **) NULL)},
    { "OM_MEMORY_INSUFFICIENT",    4, XT_RETURN_CODE, ((OM_descriptor **) NULL)},
    { "OM_NETWORK_ERROR",      5, XT_RETURN_CODE, ((OM_descriptor **) NULL)},
    { "OM_NO_SUCH_CLASS",      6, XT_RETURN_CODE, ((OM_descriptor **) NULL)},
    { "OM_NO_SUCH_EXCLUSION",  7, XT_RETURN_CODE, ((OM_descriptor **) NULL)},
    { "OM_NO_SUCH_MODIFICATION",   8, XT_RETURN_CODE, ((OM_descriptor **) NULL)},
    { "OM_NO_SUCH_OBJECT",     9, XT_RETURN_CODE, ((OM_descriptor **) NULL)},
/*
    { "OM_NO_SUCH_REPRESENTATION",10, XT_RETURN_CODE, ((OM_descriptor **) NULL)},
 */
    { "OM_NO_SUCH_RULES",     10, XT_RETURN_CODE, ((OM_descriptor **) NULL)},
    { "OM_NO_SUCH_SYNTAX",    11, XT_RETURN_CODE, ((OM_descriptor **) NULL)},
    { "OM_NO_SUCH_TYPE",      12, XT_RETURN_CODE, ((OM_descriptor **) NULL)},
    { "OM_NO_SUCH_WORKSPACE", 13, XT_RETURN_CODE, ((OM_descriptor **) NULL)},
    { "OM_NOT_AN_ENCODING",   14, XT_RETURN_CODE, ((OM_descriptor **) NULL)},
    { "OM_NOT_CONCRETE",      15, XT_RETURN_CODE, ((OM_descriptor **) NULL)},
    { "OM_NOT_PRESENT",       16, XT_RETURN_CODE, ((OM_descriptor **) NULL)},
    { "OM_NOT_PRIVATE",       17, XT_RETURN_CODE, ((OM_descriptor **) NULL)},
    { "OM_NOT_THE_SERVICES",  18, XT_RETURN_CODE, ((OM_descriptor **) NULL)},
    { "OM_PERMANENT_ERROR",   19, XT_RETURN_CODE, ((OM_descriptor **) NULL)},
    { "OM_POINTER_INVALID",   20, XT_RETURN_CODE, ((OM_descriptor **) NULL)},
    { "OM_SYSTEM_ERROR",      21, XT_RETURN_CODE, ((OM_descriptor **) NULL)},
    { "OM_TEMPORARY_ERROR",   22, XT_RETURN_CODE, ((OM_descriptor **) NULL)},
    { "OM_TOO_MANY_VALUES",   23, XT_RETURN_CODE, ((OM_descriptor **) NULL)},
    { "OM_VALUES_NOT_ADJACENT",   24, XT_RETURN_CODE, ((OM_descriptor **) NULL)},
    { "OM_WRONG_VALUE_LENGTH",    25, XT_RETURN_CODE, ((OM_descriptor **) NULL)},
    { "OM_WRONG_VALUE_MAKEUP",    26, XT_RETURN_CODE, ((OM_descriptor **) NULL)},
    { "OM_WRONG_VALUE_NUMBER",    27, XT_RETURN_CODE, ((OM_descriptor **) NULL)},
    { "OM_WRONG_VALUE_POSITION",  28, XT_RETURN_CODE, ((OM_descriptor **) NULL)},
    { "OM_WRONG_VALUE_SYNTAX",    29, XT_RETURN_CODE, ((OM_descriptor **) NULL)},
    { "OM_WRONG_VALUE_TYPE",  30, XT_RETURN_CODE, ((OM_descriptor **) NULL)},
 
 /* OM extended return codes */

	{ "OMX_SUCCESS",	           (OM_integer)  0,XT_XRETURN_CODE,NULL},
	{ "OMX_CANNOT_READ_SCHEMA",    (OM_integer) -1,XT_XRETURN_CODE,NULL},
	{ "OMX_SCHEMA_NOT_READ",	   (OM_integer) -2,XT_XRETURN_CODE,NULL},
	{ "OMX_NO_START_OBJ_BLOCK",	   (OM_integer) -3,XT_XRETURN_CODE,NULL},
	{ "OMX_NO_END_OBJ_BLOCK",	   (OM_integer) -4,XT_XRETURN_CODE,NULL},
	{ "OMX_EMPTY_OBJ_BLOCK",	   (OM_integer) -5,XT_XRETURN_CODE,NULL},
	{ "OMX_OBJ_FORMAT_ERROR",	   (OM_integer) -6,XT_XRETURN_CODE,NULL},
	{ "OMX_DUPLICATE_OBJ_ABBRV",   (OM_integer) -7,XT_XRETURN_CODE,NULL},
	{ "OMX_DUPLICATE_OBJ_OBJ_ID",  (OM_integer) -8,XT_XRETURN_CODE,NULL},
	{ "OMX_NO_START_ATTR_BLOCK",   (OM_integer) -9,XT_XRETURN_CODE,NULL},
	{ "OMX_NO_END_ATTR_BLOCK",	   (OM_integer) -10,XT_XRETURN_CODE,NULL},
	{ "OMX_EMPTY_ATTR_BLOCK",	   (OM_integer) -11,XT_XRETURN_CODE,NULL},
	{ "OMX_ATTR_FORMAT_ERROR",	   (OM_integer) -12,XT_XRETURN_CODE,NULL},
	{ "OMX_DUPLICATE_ATTR_ABBRV",  (OM_integer) -13,XT_XRETURN_CODE,NULL},
	{ "OMX_DUPLICATE_ATTR_OBJ_ID", (OM_integer) -14,XT_XRETURN_CODE,NULL},
	{ "OMX_NO_START_CLASS_BLOCK",  (OM_integer) -15,XT_XRETURN_CODE,NULL},
	{ "OMX_NO_END_CLASS_BLOCK",	   (OM_integer) -16,XT_XRETURN_CODE,NULL},
	{ "OMX_EMPTY_CLASS_BLOCK",	   (OM_integer) -17,XT_XRETURN_CODE,NULL},
	{ "OMX_CLASS_FORMAT_ERROR",    (OM_integer) -18,XT_XRETURN_CODE,NULL},
	{ "OMX_NO_CLASS_NAME",     (OM_integer) -19,XT_XRETURN_CODE,NULL},
	{ "OMX_DUPLICATE_CLASS_BLOCK", (OM_integer) -20,XT_XRETURN_CODE,NULL},
	{ "OMX_CLASS_BLOCK_UNDEFINED", (OM_integer) -21,XT_XRETURN_CODE,NULL},
	{ "OMX_INVALID_ABBRV",     (OM_integer) -22,XT_XRETURN_CODE,NULL},
	{ "OMX_INVALID_OBJ_ID",	       (OM_integer) -23,XT_XRETURN_CODE,NULL},
	{ "OMX_INVALID_CLASS_NAME",    (OM_integer) -24,XT_XRETURN_CODE,NULL},
	{ "OMX_INVALID_SYNTAX",    (OM_integer) -25,XT_XRETURN_CODE,NULL},
	{ "OMX_MEMORY_INSUFFICIENT",   (OM_integer) -26,XT_XRETURN_CODE,NULL},
	{ "OMX_INVALID_PARAMETER",	   (OM_integer) -27,XT_XRETURN_CODE,NULL},
	{ "OMX_UNKNOWN_ABBRV",	   	(OM_integer) -28,XT_XRETURN_CODE,NULL},
	{ "OMX_UNKNOWN_OBJ_ID",	   (OM_integer) -29,XT_XRETURN_CODE,NULL},
	{ "OMX_UNKNOWN_OMTYPE",	   (OM_integer) -30,XT_XRETURN_CODE,NULL},

	{ "OMX_MISSING_AVA",  	   (OM_integer) -101,XT_XRETURN_CODE,NULL},
	{ "OMX_MISSING_ABBRV",	   (OM_integer) -102,XT_XRETURN_CODE,NULL},
	{ "OMX_FORMAT_ERROR", 	   (OM_integer) -103,XT_XRETURN_CODE,NULL},
	{ "OMX_UNKNOWN_ERROR",	   (OM_integer) -104,XT_XRETURN_CODE,NULL},
	{ "OMX_MISSING_RDN_DELIMITER",  (OM_integer) -105,XT_XRETURN_CODE,NULL},
	{ "OMX_MISMATCHED_QUOTES",  (OM_integer) -106,XT_XRETURN_CODE,NULL},
	{ "OMX_MISSING_EQUAL_OPERATOR", (OM_integer) -107,XT_XRETURN_CODE,NULL},
	{ "OMX_MISSING_ATTR_VALUE", (OM_integer) -108,XT_XRETURN_CODE,NULL},
	{ "OMX_MISSING_ATTR_INFO",  (OM_integer) -109,XT_XRETURN_CODE,NULL},
	{ "OMX_MISSING_CLASS_START_OP", (OM_integer) -110,XT_XRETURN_CODE,NULL},
	{ "OMX_MISSING_CLASS_END_OP",   (OM_integer) -111,XT_XRETURN_CODE,NULL},
	{ "OMX_MISSING_CLASS_VALUE",    (OM_integer) -112,XT_XRETURN_CODE,NULL},
	{ "OMX_MISSING_COMP_VALUE", (OM_integer) -113,XT_XRETURN_CODE,NULL},
	{ "OMX_MISMATCHED_BRACKETS",    (OM_integer) -114,XT_XRETURN_CODE,NULL},
	{ "OMX_UNEXPECTED_OPERATOR",    (OM_integer) -115,XT_XRETURN_CODE,NULL},
	{ "OMX_WRONG_VALUE",		   (OM_integer) -116,XT_XRETURN_CODE,NULL},
	{ "OMX_UNKNOWN_KEYWORD",	   (OM_integer) -117,XT_XRETURN_CODE,NULL},
	{ "OMX_MISSING_OPERATOR",   (OM_integer) -118,XT_XRETURN_CODE,NULL},
	{ "OMX_MISSING_COMPOUND_OP",    (OM_integer) -119,XT_XRETURN_CODE,NULL},

/* OM_private_object */
    { "private_object1",    0,   XT_PRIVATE_OBJECT, NULL },
    { "private_object2",    1,   XT_PRIVATE_OBJECT, NULL },
    { "private_object3",    2,   XT_PRIVATE_OBJECT, NULL },
    { "private_object4",    3,   XT_PRIVATE_OBJECT, NULL },
    { "private_object5",    4,   XT_PRIVATE_OBJECT, NULL },
    { "private_object6",    5,   XT_PRIVATE_OBJECT, NULL },
    { "private_object7",    6,   XT_PRIVATE_OBJECT, NULL },
    { "private_object8",    7,   XT_PRIVATE_OBJECT, NULL },
    { "private_object9",    8,   XT_PRIVATE_OBJECT, NULL },
    { "private_object10",   9,   XT_PRIVATE_OBJECT, NULL },
    { "private_object11",   10,  XT_PRIVATE_OBJECT, NULL },
    { "private_object12",   11,  XT_PRIVATE_OBJECT, NULL },
    { "private_object13",   12,  XT_PRIVATE_OBJECT, NULL },
    { "private_object14",   13,  XT_PRIVATE_OBJECT, NULL },
    { "private_object15",   14,  XT_PRIVATE_OBJECT, NULL },
    { "private_object16",   15,  XT_PRIVATE_OBJECT, NULL },
    { "private_object17",   16,  XT_PRIVATE_OBJECT, NULL },
    { "private_object18",   17,  XT_PRIVATE_OBJECT, NULL },
    { "private_object19",   18,  XT_PRIVATE_OBJECT, NULL },
    { "private_object20",   19,  XT_PRIVATE_OBJECT, NULL },
    { "private_object21",   20,  XT_PRIVATE_OBJECT, NULL },
    { "private_object22",   21,  XT_PRIVATE_OBJECT, NULL },
    { "private_object23",   22,  XT_PRIVATE_OBJECT, NULL },
    { "private_object24",   23,  XT_PRIVATE_OBJECT, NULL },
    { "private_object25",   24,  XT_PRIVATE_OBJECT, NULL },
    { "private_object26",   25,  XT_PRIVATE_OBJECT, NULL },
    { "private_object27",   26,  XT_PRIVATE_OBJECT, NULL },
    { "private_object28",   27,  XT_PRIVATE_OBJECT, NULL },
    { "private_object29",   28,  XT_PRIVATE_OBJECT, NULL },
    { "private_object30",   29,  XT_PRIVATE_OBJECT, NULL },
    { "not_private",    30,  XT_PRIVATE_OBJECT, NULL },
    { "not_an_object",   0,  XT_PRIVATE_OBJECT, (OM_descriptor **)boolean_tbl },
 
/* OM_public_object */
    { "public_object1", 0,  XT_PUBLIC_OBJECT, NULL },
    { "public_object2", 1,  XT_PUBLIC_OBJECT, NULL },
    { "public_object3", 2,  XT_PUBLIC_OBJECT, NULL },
    { "public_object4", 3,  XT_PUBLIC_OBJECT, NULL },
    { "public_object5", 4,  XT_PUBLIC_OBJECT, NULL },
    { "public_object6", 5,  XT_PUBLIC_OBJECT, NULL },
    { "public_object7", 6,  XT_PUBLIC_OBJECT, NULL },
    { "public_object8", 7,  XT_PUBLIC_OBJECT, NULL },
    { "public_object9", 8,  XT_PUBLIC_OBJECT, NULL },
    { "public_object10",    9,  XT_PUBLIC_OBJECT, NULL },
    { "selection_extract",  10, XT_PUBLIC_OBJECT, NULL },
 
/* OM_string */
    { "string3",    0,  XT_STRING,  ((OM_descriptor **) NULL)},
    { "string8",    1,  XT_STRING,  ((OM_descriptor **) NULL)},
    { "string10",   2,  XT_STRING,  ((OM_descriptor **) NULL)},
    { "string11",   3,  XT_STRING,  ((OM_descriptor **) NULL)},
    { "string15",   4,  XT_STRING,  ((OM_descriptor **) NULL)},
    { "string31",   5,  XT_STRING,  ((OM_descriptor **) NULL)},
    { "substring1", 6,  XT_STRING,  ((OM_descriptor **) NULL)},
    { "substring2", 7,  XT_STRING,  ((OM_descriptor **) NULL)},
    { "substring3", 8,  XT_STRING,  ((OM_descriptor **) NULL)},
    { "substring4", 9,  XT_STRING,  ((OM_descriptor **) NULL)},
    { "substring5", 10, XT_STRING,  ((OM_descriptor **) NULL)},
    { "substring6", 11, XT_STRING,  ((OM_descriptor **) NULL)},

/* SAP */
/* algorithm id */
	{ "algo_id0",		0,  XT_SAP,  sap_tbl},
	{ "cert2",			1,  XT_SAP,  sap_tbl},
	{ "cert_list2",		2,  XT_SAP,  sap_tbl},
	{ "cert_list3",		3,  XT_SAP,  sap_tbl},
	{ "cert_pair2",		4,  XT_SAP,  sap_tbl},
	{ "cert_sublist2",	5,  XT_SAP,  sap_tbl},
	{ "sig1",			6,  XT_SAP,  sap_tbl},
	{ "sig2",			7,  XT_SAP,  sap_tbl},
 
};
 
#define XT_LEN_PARAMETER_TBL \
        (sizeof(parameter_tbl) / sizeof(parameter_tbl[0]))
 
#endif /* _XT_PARMS_H */
