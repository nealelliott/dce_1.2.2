/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsTrace.c,v $
 * Revision 1.1.78.2  1996/02/18  18:25:10  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:47:42  marty]
 *
 * Revision 1.1.78.1  1995/12/08  15:37:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:33  root]
 * 
 * Revision 1.1.76.5  1994/08/10  08:25:47  marrek
 * 	Fix for SAP errors, OT11467.
 * 	[1994/08/08  14:16:14  marrek]
 * 
 * Revision 1.1.76.4  1994/07/06  15:09:13  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:25:18  marrek]
 * 
 * Revision 1.1.76.3  1994/06/21  14:48:52  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:48:55  marrek]
 * 
 * Revision 1.1.76.2  1994/03/23  15:48:54  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:37:50  keutel]
 * 
 * Revision 1.1.76.1  1994/02/22  19:22:27  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:05:21  marrek]
 * 
 * Revision 1.1.74.1  1993/10/14  19:19:41  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  19:18:00  keutel]
 * 
 * 	October 1993 code drop
 * 	[1993/10/11  09:45:01  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  07:08:03  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:18:01  marrek]
 * 
 * Revision 1.1.4.7  1993/02/02  08:59:56  marrek
 * 	*** empty log message ***
 * 
 * Revision 1.1.4.6  1993/02/02  08:14:27  marrek
 * 	Insert <pthread.h> and add thread_id to log-file
 * 	additional flag in dsP_trace_msg().
 * 	[1993/02/01  14:07:27  marrek]
 * 
 * Revision 1.1.4.5  1992/12/30  23:29:39  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  23:19:51  bbelch]
 * 
 * Revision 1.1.4.4  1992/12/17  09:44:10  marrek
 * 	New object id's added for mhs
 * 	[1992/12/16  09:05:37  marrek]
 * 
 * Revision 1.1.4.3  1992/12/04  17:36:28  marrek
 * 	November 1992 code drop
 * 	improved tracing messages
 * 	[1992/12/04  12:13:46  marrek]
 * 
 * Revision 1.1.4.2  1992/11/30  08:09:11  marrek
 * 	November 1992 code drop
 * 	[1992/11/26  13:23:21  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:15:13  melman
 * 	Added #include <stdlib.h> so prototype of getenv() is established.
 * 	Added it just after the include of stdio.h.
 * 	[1992/06/01  19:04:55  melman]
 * 
 * 	New GDS merged drop
 * 	[1992/05/29  12:42:27  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: dsTrace.c,v $ $Revision: 1.1.78.2 $ $Date: 1996/02/18 18:25:10 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYP          : MODUL                                               */
/*                                                                    */
/* NAME         : dsTrace.c                                           */
/*                                                                    */
/* AUTHOR       : V. Ryan                                             */
/* DATE         : 1. 10. 1990                                         */
/*                                                                    */
/* COMPONENT    : XDS                                                 */
/*                                                                    */
/* DOK.-REF.    :                                                     */
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*                                                                    */
/* DESCRIPTION  : dsP_trace_init() function                           */
/*                dsP_display_object() function                       */
/*                dsP_display_feature() function                      */
/*                dsP_display_integer() function                      */
/*                dsP_display_boolean() function                      */
/*                dsP_print_descrs() function                         */
/*                dsP_print_oid() function                            */
/*                d27_one_digit_more() function                       */
/*                d27_oid_digits() function                           */
/*                dsP_trace_msg() function                            */
/*                dsP_compare_typ_index() function                    */
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

#ifdef TRACE

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <xom.h>
#include <xds.h>
#include <xdsbdcp.h>
#include <xdsmdup.h>
#include <xdsgds.h>
#include <xdscds.h>
#include <xdsdme.h>
#include <xdssap.h>
#include <xomip.h>
#include <xdstrace.h>
#include <d2dir.h>
#include <dsTrace.h>
#include <d27util.h>
#include <dsUtils.h>

/* internal TRACE functions */

static OM_sint dsP_compare_typ_index(const void *pkey, const void *pentry);

static OM_sint d27_one_digit_more (OM_sint *t, OM_sint n, OM_sint x, OM_sint m1, OM_sint m2);
static OM_sint d27_oid_digits (unsigned char * z, OM_sint l, char * buf);


static FILE *dsPtfile = 0;
static OM_sint  dsP_t_prtobj;
static OM_sint  dsP_t_prires;
static OM_sint  dsP_t_oidint;
static OM_sint  dsP_t_oiddot;
static OM_sint  dsP_t_stxint;
static OM_sint  dsP_t_typint;
static OM_sint  dsP_t_prthex;


#define L_FINAL_SYNTAX  OM_S_GENERAL_STRING

const char *syntaxes[] = {
    "",
    "OM_S_BOOLEAN",
    "OM_S_INTEGER",
    "OM_S_BIT_STRING",
    "OM_S_OCTET_STRING",
    "OM_S_NULL",
    "OM_S_OBJECT_ID_STRING",
    "OM_S_OBJECT_DESCRIPTOR_STRING",
    "OM_S_ENCODING_STRING",
    "",
    "OM_S_ENUMERATION",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "OM_S_NUMERIC_STRING",
    "OM_S_PRINTABLE_STRING",
    "OM_S_TELETEX_STRING",
    "OM_S_VIDEOTEX_STRING",
    "OM_S_IA5_STRING",
    "OM_S_UTC_TIME_STRING",
    "OM_S_GENERALISED_TIME_STRING",
    "OM_S_GRAPHIC_STRING",
    "OM_S_VISIBLE_STRING",
    "OM_S_GENERAL_STRING",
};

/* DSP_ERROR_STRING_LEN defined in dsTrace.h */
const char *dsP_error_string[DSP_ERROR_STRING_LEN] = {
        "DS_E_ADMIN_LIMIT_EXCEEDED",
        "DS_E_AFFECTS_MULTIPLE_DSAS",
        "DS_E_ALIAS_DEREFERENCING_PROBLEM",
        "DS_E_ALIAS_PROBLEM",
        "DS_E_ATTRIBUTE_OR_VALUE_EXISTS",
        "DS_E_BAD_ARGUMENT",
        "DS_E_BAD_CLASS",
        "DS_E_BAD_CONTEXT",
        "DS_E_BAD_NAME",
        "DS_E_BAD_SESSION",
        "DS_E_BAD_WORKSPACE",
        "DS_E_BUSY",
        "DS_E_CANNOT_ABANDON",
        "DS_E_CHAINING_REQUIRED",
        "DS_E_COMMUNICATIONS_PROBLEM",
        "DS_E_CONSTRAINT_VIOLATION",
        "DS_E_DIT_ERROR",
        "DS_E_ENTRY_EXISTS",
        "DS_E_INAPPROP_AUTHENTICATION",
        "DS_E_INAPPROP_MATCHING",
        "DS_E_INSUFFICIENT_ACCESS_RIGHTS",
        "DS_E_INVALID_ATTRIBUTE_SYNTAX",
        "DS_E_INVALID_ATTRIBUTE_VALUE",
        "DS_E_INVALID_CREDENTIALS",
        "DS_E_INVALID_REF",
        "DS_E_INVALID_SIGNATURE",
        "DS_E_LOOP_DETECTED",
        "DS_E_MISCELLANEOUS",
        "DS_E_MISSING_TYPE",
        "DS_E_MIXED_SYNCHRONOUS",
        "DS_E_NAMING_VIOLATION",
        "DS_E_NO_INFO",
        "DS_E_NO_SUCH_ATTRIBUTE_OR_VALUE",
        "DS_E_NO_SUCH_OBJECT",
        "DS_E_NO_SUCH_OPERATION",
        "DS_E_NOT_ALLOWED_ON_NON_LEAF",
        "DS_E_NOT_ALLOWED_ON_RDN",
        "DS_E_NOT_SUPPORTED",
        "DS_E_OBJECT_CLASS_MOD_PROHIB",
        "DS_E_OBJECT_CLASS_VIOLATION",
        "DS_E_OUT_OF_SCOPE",
        "DS_E_PROTECTION_REQUIRED",
        "DS_E_TIME_LIMIT_EXCEEDED",
        "DS_E_TOO_LATE",
        "DS_E_TOO_MANY_OPERATIONS",
        "DS_E_TOO_MANY_SESSIONS",
        "DS_E_UNABLE_TO_PROCEED",
        "DS_E_UNAVAILABLE",
        "DS_E_UNAVAILABLE_CRIT_EXT",
        "DS_E_UNDEFINED_ATTRIBUTE_TYPE",
        "DS_E_UNWILLING_TO_PERFORM",
};


struct dsP_typ_entry {
    const char    *sym_typ;
          OM_type  int_typ;
} dsP_typ_tab[] = {
 
 /* xom.h */
 { "OM_NO_MORE_TYPES",                 ((OM_type) 0) },
 { "OM_ARBITRARY_ENCODING",            ((OM_type) 1) },
 { "OM_ASN1_ENCODING",                 ((OM_type) 2) },
 { "OM_CLASS",                         ((OM_type) 3) },
 { "OM_DATA_VALUE_DESCRIPTOR",         ((OM_type) 4) },
 { "OM_DIRECT_REFERENCE",              ((OM_type) 5) },
 { "OM_INDIRECT_REFERENCE",            ((OM_type) 6) },
 { "OM_OBJECT_CLASS",                  ((OM_type) 7) },
 { "OM_OBJECT_ENCODING",               ((OM_type) 8) },
 { "OM_OCTET_ALIGNED_ENCODING",        ((OM_type) 9) },
 { "OM_PRIVATE_OBJECT",                ((OM_type) 10) },
 { "OM_RULES",                         ((OM_type) 11) },
 
 /* xmhp.h */
 { "MH_T_A3_WIDTH",                    ((OM_type) 200) },
 { "MH_T_ACTION",                      ((OM_type) 201) },
 { "MH_T_ACTUAL_RECIPIENT_NAME",       ((OM_type) 202) },
 { "MH_T_ADMD_NAME",                   ((OM_type) 203) },
 { "MH_T_ALGORITHM_DATUM",             ((OM_type) 204) },
 { "MH_T_ALGORITHM_ID",                ((OM_type) 205) },
 { "MH_T_ALGORITHM_RESULT",            ((OM_type) 206) },
 { "MH_T_ALTERNATE_RECIP_ALLOWED",     ((OM_type) 207) },
 { "MH_T_ALTERNATE_RECIPIENT_NAME",    ((OM_type) 208) },
 { "MH_T_ARRIVAL_TIME",                ((OM_type) 209) },
 { "MH_T_ATTEMPTED_ADMD_NAME",         ((OM_type) 210) },
 { "MH_T_ATTEMPTED_COUNTRY_NAME",      ((OM_type) 211) },
 { "MH_T_ATTEMPTED_MTA_NAME",          ((OM_type) 212) },
 { "MH_T_ATTEMPTED_PRMD_IDENTIFIER",   ((OM_type) 213) },
 { "MH_T_B4_LENGTH",                   ((OM_type) 214) },
 { "MH_T_B4_WIDTH",                    ((OM_type) 215) },
 { "MH_T_BILATERAL_INFORMATION",       ((OM_type) 216) },
 { "MH_T_BINARY_CONTENT",              ((OM_type) 217) },
 { "MH_T_BUILTIN_EITS",                ((OM_type) 218) },
 { "MH_T_BUREAU_FAX_DELIVERY",         ((OM_type) 219) },
 { "MH_T_COMMON_NAME",                 ((OM_type) 220) },
 { "MH_T_CONFIDENTIALITY_ALGORITHM",   ((OM_type) 221) },
 { "MH_T_CONFIDENTIALITY_KEY",         ((OM_type) 222) },
 { "MH_T_CONTENT",                     ((OM_type) 223) },
 { "MH_T_CONTENT_CORRELATOR",          ((OM_type) 224) },
 { "MH_T_CONTENT_EXTENSIONS",          ((OM_type) 225) },
 { "MH_T_CONTENT_IDENTIFIER",          ((OM_type) 226) },
 { "MH_T_CONTENT_LENGTH",              ((OM_type) 227) },
 { "MH_T_CONTENT_RETURN_REQUESTED",    ((OM_type) 228) },
 { "MH_T_CONTENT_TYPE",                ((OM_type) 229) },
 { "MH_T_CONTROL_CHARACTER_SETS",      ((OM_type) 230) },
 { "MH_T_CONVERSION_LOSS_PROHIBITED",  ((OM_type) 231) },
 { "MH_T_CONVERSION_PROHIBITED",       ((OM_type) 232) },
 { "MH_T_CONVERTED_EITS",              ((OM_type) 233) },
 { "MH_T_COUNTRY_NAME",                ((OM_type) 234) },
 { "MH_T_CRITICAL_FOR_DELIVERY",       ((OM_type) 235) },
 { "MH_T_CRITICAL_FOR_SUBMISSION",     ((OM_type) 236) },
 { "MH_T_CRITICAL_FOR_TRANSFER",       ((OM_type) 237) },
 { "MH_T_DEFERRED_DELIVERY_TIME",      ((OM_type) 238) },
 { "MH_T_DEFERRED_TIME",               ((OM_type) 239) },
 { "MH_T_DELIVERY_CONFIRMATIONS",      ((OM_type) 240) },
 { "MH_T_DELIVERY_POINT",              ((OM_type) 241) },
 { "MH_T_DELIVERY_TIME",               ((OM_type) 242) },
 { "MH_T_DIRECTORY_NAME",              ((OM_type) 243) },
 { "MH_T_DISCLOSURE_ALLOWED",          ((OM_type) 244) },
 { "MH_T_DISTINGUISHED_RECIP_ADDR",    ((OM_type) 245) },
 { "MH_T_DOMAIN_TYPE_1",               ((OM_type) 246) },
 { "MH_T_DOMAIN_TYPE_2",               ((OM_type) 247) },
 { "MH_T_DOMAIN_TYPE_3",               ((OM_type) 248) },
 { "MH_T_DOMAIN_TYPE_4",               ((OM_type) 249) },
 { "MH_T_DOMAIN_VALUE_1",              ((OM_type) 250) },
 { "MH_T_DOMAIN_VALUE_2",              ((OM_type) 251) },
 { "MH_T_DOMAIN_VALUE_3",              ((OM_type) 252) },
 { "MH_T_DOMAIN_VALUE_4",              ((OM_type) 253) },
 { "MH_T_ENVELOPES",                   ((OM_type) 254) },
 { "MH_T_EVENT_HANDLE",                ((OM_type) 255) },
 { "MH_T_EXPANSION_HISTORY",           ((OM_type) 256) },
 { "MH_T_EXPANSION_PROHIBITED",        ((OM_type) 257) },
 { "MH_T_EXPLICIT_CONVERSION",         ((OM_type) 258) },
 { "MH_T_EXTENSION_TYPE",              ((OM_type) 259) },
 { "MH_T_EXTENSION_VALUE",             ((OM_type) 260) },
 { "MH_T_EXTENSIONS",                  ((OM_type) 261) },
 { "MH_T_EXTERNAL_EITS",               ((OM_type) 262) },
 { "MH_T_EXTERNAL_TRACE_INFO",         ((OM_type) 263) },
 { "MH_T_FINE_RESOLUTION",             ((OM_type) 264) },
 { "MH_T_FORWARDING_ADDRESS",          ((OM_type) 265) },
 { "MH_T_FORWARDING_ADDR_REQUESTED",   ((OM_type) 266) },
 { "MH_T_FORWARDING_PROHIBITED",       ((OM_type) 267) },
 { "MH_T_G3_FAX_NBPS",                 ((OM_type) 268) },
 { "MH_T_G4_FAX_NBPS",                 ((OM_type) 269) },
 { "MH_T_GENERATION",                  ((OM_type) 270) },
 { "MH_T_GIVEN_NAME",                  ((OM_type) 271) },
 { "MH_T_GRAPHIC_CHARACTER_SETS",      ((OM_type) 272) },
 { "MH_T_INFORMATION",                 ((OM_type) 273) },
 { "MH_T_INITIALS",                    ((OM_type) 274) },
 { "MH_T_INTEGRITY_CHECK",             ((OM_type) 275) },
 { "MH_T_INTENDED_RECIPIENT_NAME",     ((OM_type) 276) },
 { "MH_T_INTENDED_RECIPIENT_NUMBER",   ((OM_type) 277) },
 { "MH_T_INTERNAL_TRACE_INFO",         ((OM_type) 278) },
 { "MH_T_ISDN_NUMBER",                 ((OM_type) 279) },
 { "MH_T_ISDN_SUBADDRESS",             ((OM_type) 280) },
 { "MH_T_LATEST_DELIVERY_TIME",        ((OM_type) 281) },
 { "MH_T_LOCAL_IDENTIFIER",            ((OM_type) 282) },
 { "MH_T_MESSAGE_SEQUENCE_NUMBER",     ((OM_type) 283) },
 { "MH_T_MISCELLANEOUS_CAPABILITIES",  ((OM_type) 284) },
 { "MH_T_MTA_CERTIFICATE",             ((OM_type) 285) },
 { "MH_T_MTA_NAME",                    ((OM_type) 286) },
 { "MH_T_MTA_REPORT_REQUEST",          ((OM_type) 287) },
 { "MH_T_MTA_RESPONSIBILITY",          ((OM_type) 288) },
 { "MH_T_MTS_IDENTIFIER",              ((OM_type) 289) },
 { "MH_T_NAME",                        ((OM_type) 290) },
 { "MH_T_NON_DELIVERY_DIAGNOSTIC",     ((OM_type) 291) },
 { "MH_T_NON_DELIVERY_REASON",         ((OM_type) 292) },
 { "MH_T_NUMERIC_USER_IDENTIFIER",     ((OM_type) 293) },
 { "MH_T_ORGANIZATION_NAME",           ((OM_type) 294) },
 { "MH_T_ORGANIZATIONAL_UNIT_NAME_1",  ((OM_type) 295) },
 { "MH_T_ORGANIZATIONAL_UNIT_NAME_2",  ((OM_type) 296) },
 { "MH_T_ORGANIZATIONAL_UNIT_NAME_3",  ((OM_type) 297) },
 { "MH_T_ORGANIZATIONAL_UNIT_NAME_4",  ((OM_type) 298) },
 { "MH_T_ORIG_AND_EXPANSION_HISTORY",  ((OM_type) 299) },
 { "MH_T_ORIGIN_CHECK",                ((OM_type) 300) },
 { "MH_T_ORIGINAL_EITS",               ((OM_type) 301) },
 { "MH_T_ORIGINALLY_INTENDED_RECIP",   ((OM_type) 302) },
 { "MH_T_ORIGINATOR_CERTIFICATE",      ((OM_type) 303) },
 { "MH_T_ORIGINATOR_NAME",             ((OM_type) 304) },
 { "MH_T_ORIGINATOR_REPORT_REQUEST",   ((OM_type) 305) },
 { "MH_T_ORIGINATOR_RETURN_ADDRESS",   ((OM_type) 306) },
 { "MH_T_OTHER_RECIPIENT_NAMES",       ((OM_type) 307) },
 { "MH_T_PAGE_FORMATS",                ((OM_type) 308) },
 { "MH_T_PER_RECIPIENT_REPORTS",       ((OM_type) 309) },
 { "MH_T_POSTAL_ADDRESS_DETAILS",      ((OM_type) 310) },
 { "MH_T_POSTAL_ADDRESS_IN_FULL",      ((OM_type) 311) },
 { "MH_T_POSTAL_ADDRESS_IN_LINES",     ((OM_type) 312) },
 { "MH_T_POSTAL_CODE",                 ((OM_type) 313) },
 { "MH_T_POSTAL_COUNTRY_NAME",         ((OM_type) 314) },
 { "MH_T_POSTAL_DELIVERY_POINT_NAME",  ((OM_type) 315) },
 { "MH_T_POSTAL_DELIV_SYSTEM_NAME",    ((OM_type) 316) },
 { "MH_T_POSTAL_GENERAL_DELIV_ADDR",   ((OM_type) 317) },
 { "MH_T_POSTAL_LOCALE",               ((OM_type) 318) },
 { "MH_T_POSTAL_MODE",                 ((OM_type) 319) },
 { "MH_T_POSTAL_OFFICE_BOX_NUMBER",    ((OM_type) 320) },
 { "MH_T_POSTAL_OFFICE_NAME",          ((OM_type) 321) },
 { "MH_T_POSTAL_OFFICE_NUMBER",        ((OM_type) 322) },
 { "MH_T_POSTAL_ORGANIZATION_NAME",    ((OM_type) 323) },
 { "MH_T_POSTAL_PATRON_DETAILS",       ((OM_type) 324) },
 { "MH_T_POSTAL_PATRON_NAME",          ((OM_type) 325) },
 { "MH_T_POSTAL_REPORT",               ((OM_type) 326) },
 { "MH_T_POSTAL_STREET_ADDRESS",       ((OM_type) 327) },
 { "MH_T_PREFERRED_DELIVERY_MODES",    ((OM_type) 328) },
 { "MH_T_PRESENTATION_ADDRESS",        ((OM_type) 329) },
 { "MH_T_PRIORITY",                    ((OM_type) 330) },
 { "MH_T_PRIVACY_MARK",                ((OM_type) 331) },
 { "MH_T_PRIVATE_USE",                 ((OM_type) 332) },
 { "MH_T_PRMD_IDENTIFIER",             ((OM_type) 333) },
 { "MH_T_PRMD_NAME",                   ((OM_type) 334) },
 { "MH_T_PROOF_OF_DELIVERY",           ((OM_type) 335) },
 { "MH_T_PROOF_OF_DELIV_REQUESTED",    ((OM_type) 336) },
 { "MH_T_PROOF_OF_SUBMISSION",         ((OM_type) 337) },
 { "MH_T_PROOF_OF_SUBMISN_REQUEST",    ((OM_type) 338) },
 { "MH_T_PUBLIC_INFORMATION",          ((OM_type) 339) },
 { "MH_T_RANDOM_NUMBER",               ((OM_type) 340) },
 { "MH_T_REASON",                      ((OM_type) 341) },
 { "MH_T_REASSIGNMENT_PROHIBITED",     ((OM_type) 342) },
 { "MH_T_RECIPIENT_CERTIFICATE",       ((OM_type) 343) },
 { "MH_T_RECIPIENT_DESCRIPTORS",       ((OM_type) 344) },
 { "MH_T_RECIPIENT_NAME",              ((OM_type) 345) },
 { "MH_T_RECIPIENT_NUMBER",            ((OM_type) 346) },
 { "MH_T_RECIP_NUMBER_FOR_ADVICE",     ((OM_type) 347) },
 { "MH_T_REDIRECTION_HISTORY",         ((OM_type) 348) },
 { "MH_T_REGISTRATION",                ((OM_type) 349) },
 { "MH_T_RENDITION_ATTRIBUTES",        ((OM_type) 350) },
 { "MH_T_REPORT_ADDITIONAL_INFO",      ((OM_type) 351) },
 { "MH_T_REPORT_DESTINATION",          ((OM_type) 352) },
 { "MH_T_REPORTING_DL_NAME",           ((OM_type) 353) },
 { "MH_T_REPORTING_MTA_CERTIFICATE",   ((OM_type) 354) },
 { "MH_T_SECRET_INFORMATION",          ((OM_type) 355) },
 { "MH_T_SECURITY_CATEGORY_DATA",      ((OM_type) 356) },
 { "MH_T_SECURITY_CATEGORY_IDS",       ((OM_type) 357) },
 { "MH_T_SECURITY_CLASSIFICATION",     ((OM_type) 358) },
 { "MH_T_SECURITY_LABEL",              ((OM_type) 359) },
 { "MH_T_SECURITY_POLICY_ID",          ((OM_type) 360) },
 { "MH_T_SIGNATURE",                   ((OM_type) 361) },
 { "MH_T_SUBJECT_EXT_TRACE_INFO",      ((OM_type) 362) },
 { "MH_T_SUBJECT_MTS_IDENTIFIER",      ((OM_type) 363) },
 { "MH_T_SUBMISSION_TIME",             ((OM_type) 364) },
 { "MH_T_SUPPLEMENTARY_INFO",          ((OM_type) 365) },
 { "MH_T_SURNAME",                     ((OM_type) 366) },
 { "MH_T_TELETEX_NBPS",                ((OM_type) 367) },
 { "MH_T_TEMPORARY",                   ((OM_type) 368) },
 { "MH_T_TERMINAL_IDENTIFIER",         ((OM_type) 369) },
 { "MH_T_TERMINAL_TYPE",               ((OM_type) 370) },
 { "MH_T_TIME",                        ((OM_type) 371) },
 { "MH_T_TOKEN",                       ((OM_type) 372) },
 { "MH_T_TWO_DIMENSIONAL",             ((OM_type) 373) },
 { "MH_T_UNCOMPRESSED",                ((OM_type) 374) },
 { "MH_T_UNLIMITED_LENGTH",            ((OM_type) 375) },
 { "MH_T_WORKSPACE",                   ((OM_type) 376) },
 { "MH_T_X121_ADDRESS",                ((OM_type) 377) },
 
 /* xds.h */
 { "DS_ACCESS_POINTS",                 ((OM_type) 701) },
 { "DS_ADDRESS",                       ((OM_type) 702) },
 { "DS_AE_TITLE",                      ((OM_type) 703) },
 { "DS_ALIASED_RDNS",                  ((OM_type) 704) },
 { "DS_ALIAS_DEREFERENCED",            ((OM_type) 705) },
 { "DS_ALIAS_ENTRY",                   ((OM_type) 706) },
 { "DS_ALL_ATTRIBUTES",                ((OM_type) 707) },
 { "DS_ASYNCHRONOUS",                  ((OM_type) 708) },
 { "DS_ATTRIBUTES",                    ((OM_type) 709) },
 { "DS_ATTRIBUTES_SELECTED",           ((OM_type) 710) },
 { "DS_ATTRIBUTE_TYPE",                ((OM_type) 711) },
 { "DS_ATTRIBUTE_VALUE",               ((OM_type) 712) },
 { "DS_ATTRIBUTE_VALUES",              ((OM_type) 713) },
 { "DS_AUTOMATIC_CONTINUATION",        ((OM_type) 714) },
 { "DS_AVAS",                          ((OM_type) 715) },
 { "DS_CHAINING_PROHIB",               ((OM_type) 716) },
 { "DS_CHANGES",                       ((OM_type) 717) },
 { "DS_CRIT",                          ((OM_type) 718) },
 { "DS_DONT_DEREFERENCE_ALIASES",      ((OM_type) 719) },
 { "DS_DONT_USE_COPY",                 ((OM_type) 720) },
 { "DS_DSA_ADDRESS",                   ((OM_type) 721) },
 { "DS_DSA_NAME",                      ((OM_type) 722) },
 { "DS_ENTRIES",                       ((OM_type) 723) },
 { "DS_ENTRY",                         ((OM_type) 724) },
 { "DS_EXT",                           ((OM_type) 725) },
 { "DS_FILE_DESCRIPTOR",               ((OM_type) 726) },
 { "DS_FILTERS",                       ((OM_type) 727) },
 { "DS_FILTER_ITEMS",                  ((OM_type) 728) },
 { "DS_FILTER_ITEM_TYPE",              ((OM_type) 729) },
 { "DS_FILTER_TYPE",                   ((OM_type) 730) },
 { "DS_FINAL_SUBSTRING",               ((OM_type) 731) },
 { "DS_FROM_ENTRY",                    ((OM_type) 732) },
 { "DS_IDENT",                         ((OM_type) 733) },
 { "DS_INFO_TYPE",                     ((OM_type) 734) },
 { "DS_INITIAL_SUBSTRING",             ((OM_type) 735) },
 { "DS_ITEM_PARAMETERS",               ((OM_type) 736) },
 { "DS_LIMIT_PROBLEM",                 ((OM_type) 737) },
 { "DS_LIST_INFO",                     ((OM_type) 738) },
 { "DS_LOCAL_SCOPE",                   ((OM_type) 739) },
 { "DS_MATCHED",                       ((OM_type) 740) },
 { "DS_MOD_TYPE",                      ((OM_type) 741) },
 { "DS_NAME_RESOLUTION_PHASE",         ((OM_type) 742) },
 { "DS_NEXT_RDN_TO_BE_RESOLVED",       ((OM_type) 743) },
 { "DS_N_ADDRESSES",                   ((OM_type) 744) },
 { "DS_OBJECT_NAME",                   ((OM_type) 745) },
 { "DS_OPERATION_PROGRESS",            ((OM_type) 746) },
 { "DS_PARTIAL_OUTCOME_QUAL",          ((OM_type) 747) },
 { "DS_PERFORMER",                     ((OM_type) 748) },
 { "DS_PREFER_CHAINING",               ((OM_type) 749) },
 { "DS_PRIORITY",                      ((OM_type) 750) },
 { "DS_PROBLEM",                       ((OM_type) 751) },
 { "DS_PROBLEMS",                      ((OM_type) 752) },
 { "DS_P_SELECTOR",                    ((OM_type) 753) },
 { "DS_RDN",                           ((OM_type) 754) },
 { "DS_RDNS",                          ((OM_type) 755) },
 { "DS_RDNS_RESOLVED",                 ((OM_type) 756) },
 { "DS_REQUESTOR",                     ((OM_type) 757) },
 { "DS_SCOPE_OF_REFERRAL",             ((OM_type) 758) },
 { "DS_SEARCH_INFO",                   ((OM_type) 759) },
 { "DS_SIZE_LIMIT",                    ((OM_type) 760) },
 { "DS_SUBORDINATES",                  ((OM_type) 761) },
 { "DS_S_SELECTOR",                    ((OM_type) 762) },
 { "DS_TARGET_OBJECT",                 ((OM_type) 763) },
 { "DS_TIME_LIMIT",                    ((OM_type) 764) },
 { "DS_T_SELECTOR",                    ((OM_type) 765) },
 { "DS_UNAVAILABLE_CRIT_EXT",          ((OM_type) 766) },
 { "DS_UNCORRELATED_LIST_INFO",        ((OM_type) 767) },
 { "DS_UNCORRELATED_SEARCH_INFO",      ((OM_type) 768) },
 { "DS_UNEXPLORED",                    ((OM_type) 769) },
 
 /* xdsbdcp.h */
 { "DS_ANSWERBACK",                    ((OM_type) 801) },
 { "DS_COUNTRY_CODE",                  ((OM_type) 802) },
 { "DS_CRITERIA",                      ((OM_type) 803) },
 { "DS_OBJECT_CLASS",                  ((OM_type) 804) },
 { "DS_PARAMETERS",                    ((OM_type) 805) },
 { "DS_POSTAL_ADDRESS",                ((OM_type) 806) },
 { "DS_PHONE_NBR",                     ((OM_type) 807) },
 { "DS_TELETEX_TERM",                  ((OM_type) 808) },
 { "DS_TELEX_NBR",                     ((OM_type) 809) },
 
 /* xdsap.h */
 { "DS_ALGORITHM",                     ((OM_type) 821) },
 { "DS_FORWARD",                       ((OM_type) 822) },
 { "DS_ISSUER",                        ((OM_type) 823) },
 { "DS_LAST_UPDATE",                   ((OM_type) 824) },
 { "DS_ALGORITHM_PARAMETERS",          ((OM_type) 825) },
 { "DS_REVERSE",                       ((OM_type) 826) },
 { "DS_REVOCATION_DATE",               ((OM_type) 827) },
 { "DS_REVOKED_CERTS",                 ((OM_type) 828) },
 { "DS_SERIAL_NUMBER",                 ((OM_type) 829) },
 { "DS_SERIAL_NUMBERS",                ((OM_type) 830) },
 { "DS_SIGNATURE",                     ((OM_type) 831) },
 { "DS_SIGNATURE_VALUE",               ((OM_type) 832) },
 { "DS_SUBJECT",                       ((OM_type) 833) },
 { "DS_SUBJECT_ALGORITHM",             ((OM_type) 834) },
 { "DS_SUBJECT_PUBLIC_KEY",            ((OM_type) 835) },
 { "DS_VALIDITY_NOT_AFTER",            ((OM_type) 836) },
 { "DS_VALIDITY_NOT_BEFORE",           ((OM_type) 837) },
 { "DS_VERSION",                       ((OM_type) 838) },
 
 /* xdsgds.h */
 { "DSX_PASSWORD",                     ((OM_type) 850) },
 { "DSX_DIR_ID",                       ((OM_type) 851) },
 { "DSX_DUAFIRST",                     ((OM_type) 852) },
 { "DSX_DONT_STORE",                   ((OM_type) 853) },
 { "DSX_NORMAL_CLASS",                 ((OM_type) 854) },
 { "DSX_PRIV_CLASS",                   ((OM_type) 855) },
 { "DSX_RESIDENT_CLASS",               ((OM_type) 856) },
 { "DSX_USEDSA",                       ((OM_type) 857) },
 { "DSX_DUA_CACHE",                    ((OM_type) 858) },
 { "DSX_MODIFY_PUBLIC",                ((OM_type) 859) },
 { "DSX_READ_STANDARD",                ((OM_type) 860) },
 { "DSX_MODIFY_STANDARD",              ((OM_type) 861) },
 { "DSX_READ_SENSITIVE",               ((OM_type) 862) },
 { "DSX_MODIFY_SENSITIVE",             ((OM_type) 863) },
 { "DSX_INTERPRETATION",               ((OM_type) 864) },
 { "DSX_USER",                         ((OM_type) 865) },
 { "DSX_PREFER_ADM_FUNCS",             ((OM_type) 866) },
 { "DSX_AUTH_MECHANISM",               ((OM_type) 867) },
 { "DSX_AUTH_INFO",                    ((OM_type) 868) },
 { "DSX_SIGN_MECHANISM",               ((OM_type) 869) },
 { "DSX_PROT_REQUEST",                 ((OM_type) 870) },
 
 /* xdsmdup.h */
 { "DS_PERM_TYPE",                     ((OM_type) 901) },
 { "DS_INDIVIDUAL",                    ((OM_type) 902) },
 { "DS_MEMBER_OF_DL",                  ((OM_type) 903) },
 { "DS_PATTERN_MATCH",                 ((OM_type) 904) },
 { "DS_MEMBER_OF_GROUP",               ((OM_type) 905) },
};

#define DSP_TYP_TAB_SIZE    (sizeof(dsP_typ_tab) / sizeof(dsP_typ_tab[0]))


#define DSP_FILL_OID_TAB(oid)   OMP_O_##oid, sizeof(OMP_O_##oid) -1, #oid

/* 						N O T E ! ! ! !
	the following entries are numerically sorted by object id,
	so any changes and additions must be done carefully 
*/

struct dsP_oid_entry {
	const		char *hex_oid;
	OM_sint		hex_len;
	const char	*symbol_oid;
} dsP_oid_tab[] = {

 /* directory service package (OM classes) */
 { DSP_FILL_OID_TAB (DS_SERVICE_PKG) },
 { DSP_FILL_OID_TAB (DS_C_ABANDON_FAILED) },
 { DSP_FILL_OID_TAB (DS_C_ACCESS_POINT) },
 { DSP_FILL_OID_TAB (DS_C_ADDRESS) },
 { DSP_FILL_OID_TAB (DS_C_ATTRIBUTE) },
 { DSP_FILL_OID_TAB (DS_C_ATTRIBUTE_ERROR) },
 { DSP_FILL_OID_TAB (DS_C_ATTRIBUTE_LIST) },
 { DSP_FILL_OID_TAB (DS_C_ATTRIBUTE_PROBLEM) },
 { DSP_FILL_OID_TAB (DS_C_AVA) },
 { DSP_FILL_OID_TAB (DS_C_COMMON_RESULTS) },
 { DSP_FILL_OID_TAB (DS_C_COMMUNICATIONS_ERROR) },
 { DSP_FILL_OID_TAB (DS_C_COMPARE_RESULT) },
 { DSP_FILL_OID_TAB (DS_C_CONTEXT) },
 { DSP_FILL_OID_TAB (DS_C_CONTINUATION_REF) },
 { DSP_FILL_OID_TAB (DS_C_DS_DN) },
 { DSP_FILL_OID_TAB (DS_C_DS_RDN) },
 { DSP_FILL_OID_TAB (DS_C_ENTRY_INFO) },
 { DSP_FILL_OID_TAB (DS_C_ENTRY_INFO_SELECTION) },
 { DSP_FILL_OID_TAB (DS_C_ENTRY_MOD) },
 { DSP_FILL_OID_TAB (DS_C_ENTRY_MOD_LIST) },
 { DSP_FILL_OID_TAB (DS_C_ERROR) },
 { DSP_FILL_OID_TAB (DS_C_EXT) },
 { DSP_FILL_OID_TAB (DS_C_FILTER) },
 { DSP_FILL_OID_TAB (DS_C_FILTER_ITEM) },
 { DSP_FILL_OID_TAB (DS_C_LIBRARY_ERROR) },
 { DSP_FILL_OID_TAB (DS_C_LIST_INFO) },
 { DSP_FILL_OID_TAB (DS_C_LIST_INFO_ITEM) },
 { DSP_FILL_OID_TAB (DS_C_LIST_RESULT) },
 { DSP_FILL_OID_TAB (DS_C_NAME) },
 { DSP_FILL_OID_TAB (DS_C_NAME_ERROR) },
 { DSP_FILL_OID_TAB (DS_C_OPERATION_PROGRESS) },
 { DSP_FILL_OID_TAB (DS_C_PARTIAL_OUTCOME_QUAL) },
 { DSP_FILL_OID_TAB (DS_C_PRESENTATION_ADDRESS) },
 { DSP_FILL_OID_TAB (DS_C_READ_RESULT) },
 { DSP_FILL_OID_TAB (DS_C_REFERRAL) },
 { DSP_FILL_OID_TAB (DS_C_RELATIVE_NAME) },
 { DSP_FILL_OID_TAB (DS_C_SEARCH_INFO) },
 { DSP_FILL_OID_TAB (DS_C_SEARCH_RESULT) },
 { DSP_FILL_OID_TAB (DS_C_SECURITY_ERROR) },
 { DSP_FILL_OID_TAB (DS_C_SERVICE_ERROR) },
 { DSP_FILL_OID_TAB (DS_C_SESSION) },
 { DSP_FILL_OID_TAB (DS_C_SYSTEM_ERROR) },
 { DSP_FILL_OID_TAB (DS_C_UPDATE_ERROR) },
 
 /* basic directory contents package (OM classes) */
 { DSP_FILL_OID_TAB (DS_BASIC_DIR_CONTENTS_PKG) },
 { DSP_FILL_OID_TAB (DS_C_FACSIMILE_PHONE_NBR) },
 { DSP_FILL_OID_TAB (DS_C_POSTAL_ADDRESS) },
 { DSP_FILL_OID_TAB (DS_C_SEARCH_CRITERION) },
 { DSP_FILL_OID_TAB (DS_C_SEARCH_GUIDE) },
 { DSP_FILL_OID_TAB (DS_C_TELETEX_TERM_IDENT) },
 { DSP_FILL_OID_TAB (DS_C_TELEX_NBR) },
 
 /* strong authentication package SAP (OM classes) */
 { DSP_FILL_OID_TAB (DS_STRONG_AUTHENT_PKG) },
 { DSP_FILL_OID_TAB (DS_C_ALGORITHM_IDENT) },
 { DSP_FILL_OID_TAB (DS_C_CERT) },
 { DSP_FILL_OID_TAB (DS_C_CERT_LIST) },
 { DSP_FILL_OID_TAB (DS_C_CERT_PAIR) },
 { DSP_FILL_OID_TAB (DS_C_CERT_SUBLIST) },
 { DSP_FILL_OID_TAB (DS_C_SIGNATURE) },

 /* MHS directory user package (OM classes) */
 { DSP_FILL_OID_TAB (DS_MHS_DIR_USER_PKG) },
 { DSP_FILL_OID_TAB (DS_C_DL_SUBMIT_PERMS) },

 /* global directory service attribute types */
 { DSP_FILL_OID_TAB (DSX_A_MASTER_KNOWLEDGE) },
 { DSP_FILL_OID_TAB (DSX_A_ACL) },
 { DSP_FILL_OID_TAB (DSX_A_TIME_STAMP) },
 { DSP_FILL_OID_TAB (DSX_A_SHADOWED_BY) },
 { DSP_FILL_OID_TAB (DSX_A_SRT) },
 { DSP_FILL_OID_TAB (DSX_A_OCT) },
 { DSP_FILL_OID_TAB (DSX_A_AT) },
 { DSP_FILL_OID_TAB (DSX_A_DEFAULT_DSA) },
 { DSP_FILL_OID_TAB (DSX_A_LOCAL_DSA) },
 { DSP_FILL_OID_TAB (DSX_A_CLIENT) },
 { DSP_FILL_OID_TAB (DSX_A_DNLIST) },
 { DSP_FILL_OID_TAB (DSX_A_SHADOWING_JOB) },
 { DSP_FILL_OID_TAB (DSX_A_CDS_CELL) },
 { DSP_FILL_OID_TAB (DSX_A_CDS_REPLICA) },
 
 /* global directory service object classes */
 { DSP_FILL_OID_TAB (DSX_O_SCHEMA) },
 
 /* global directory service package (OM classes) */
 { DSP_FILL_OID_TAB (DSX_GDS_PKG) },
 { DSP_FILL_OID_TAB (DSX_C_GDS_SESSION) },
 { DSP_FILL_OID_TAB (DSX_C_GDS_CONTEXT) },
 { DSP_FILL_OID_TAB (DSX_C_GDS_ACL) },
 { DSP_FILL_OID_TAB (DSX_C_GDS_ACL_ITEM) },
 
 /* cell directory service */
 { DSP_FILL_OID_TAB (DSX_UUID) },
 { DSP_FILL_OID_TAB (DSX_TYPELESS_RDN) },

 /* cell directory service attribute types */
 { DSP_FILL_OID_TAB (DSX_A_CDS_Members) },
 { DSP_FILL_OID_TAB (DSX_A_CDS_GroupRevoke) },
 { DSP_FILL_OID_TAB (DSX_A_CDS_CTS) },
 { DSP_FILL_OID_TAB (DSX_A_CDS_UTS) },
 { DSP_FILL_OID_TAB (DSX_A_CDS_Class) },
 { DSP_FILL_OID_TAB (DSX_A_CDS_ClassVersion) },
 { DSP_FILL_OID_TAB (DSX_A_CDS_ObjectUUID) },
 { DSP_FILL_OID_TAB (DSX_A_CDS_Address) },
 { DSP_FILL_OID_TAB (DSX_A_CDS_Replicas) },
 { DSP_FILL_OID_TAB (DSX_A_CDS_AllUpTo) },
 { DSP_FILL_OID_TAB (DSX_A_CDS_Convergence) },
 { DSP_FILL_OID_TAB (DSX_A_CDS_InCHName) },
 { DSP_FILL_OID_TAB (DSX_A_CDS_ParentPointer) },
 { DSP_FILL_OID_TAB (DSX_A_CDS_DirectoryVersion) },
 { DSP_FILL_OID_TAB (DSX_A_CDS_UpgradeTo) },
 { DSP_FILL_OID_TAB (DSX_A_CDS_LinkTarget) },
 { DSP_FILL_OID_TAB (DSX_A_CDS_LinkTimeout) },
 { DSP_FILL_OID_TAB (DSX_A_CDS_Towers) },
 { DSP_FILL_OID_TAB (DSX_A_CDS_CHName) },
 { DSP_FILL_OID_TAB (DSX_A_CDS_CHLastAddress) },
 { DSP_FILL_OID_TAB (DSX_A_CDS_CHUpPointers) },
 { DSP_FILL_OID_TAB (DSX_A_CDS_CHState) },

 /* X.500 standard attribute types */
 { DSP_FILL_OID_TAB (DS_A_OBJECT_CLASS) },
 { DSP_FILL_OID_TAB (DS_A_ALIASED_OBJECT_NAME) },
 { DSP_FILL_OID_TAB (DS_A_KNOWLEDGE_INFO) },
 { DSP_FILL_OID_TAB (DS_A_COMMON_NAME) },
 { DSP_FILL_OID_TAB (DS_A_SURNAME) },
 { DSP_FILL_OID_TAB (DS_A_SERIAL_NBR) },
 { DSP_FILL_OID_TAB (DS_A_COUNTRY_NAME) },
 { DSP_FILL_OID_TAB (DS_A_LOCALITY_NAME) },
 { DSP_FILL_OID_TAB (DS_A_STATE_OR_PROV_NAME) },
 { DSP_FILL_OID_TAB (DS_A_STREET_ADDRESS) },
 { DSP_FILL_OID_TAB (DS_A_ORG_NAME) },
 { DSP_FILL_OID_TAB (DS_A_ORG_UNIT_NAME) },
 { DSP_FILL_OID_TAB (DS_A_TITLE) },
 { DSP_FILL_OID_TAB (DS_A_DESCRIPTION) },
 { DSP_FILL_OID_TAB (DS_A_SEARCH_GUIDE) },
 { DSP_FILL_OID_TAB (DS_A_BUSINESS_CATEGORY) },
 { DSP_FILL_OID_TAB (DS_A_POSTAL_ADDRESS) },
 { DSP_FILL_OID_TAB (DS_A_POSTAL_CODE) },
 { DSP_FILL_OID_TAB (DS_A_POST_OFFICE_BOX) },
 { DSP_FILL_OID_TAB (DS_A_PHYS_DELIV_OFF_NAME) },
 { DSP_FILL_OID_TAB (DS_A_PHONE_NBR) },
 { DSP_FILL_OID_TAB (DS_A_TELEX_NBR) },
 { DSP_FILL_OID_TAB (DS_A_TELETEX_TERM_IDENT) },
 { DSP_FILL_OID_TAB (DS_A_FACSIMILE_PHONE_NBR) },
 { DSP_FILL_OID_TAB (DS_A_X121_ADDRESS) },
 { DSP_FILL_OID_TAB (DS_A_INTERNAT_ISDN_NBR) },
 { DSP_FILL_OID_TAB (DS_A_REGISTERED_ADDRESS) },
 { DSP_FILL_OID_TAB (DS_A_DEST_INDICATOR) },
 { DSP_FILL_OID_TAB (DS_A_PREF_DELIV_METHOD) },
 { DSP_FILL_OID_TAB (DS_A_PRESENTATION_ADDRESS) },
 { DSP_FILL_OID_TAB (DS_A_SUPPORT_APPLIC_CONTEXT) },
 { DSP_FILL_OID_TAB (DS_A_MEMBER) },
 { DSP_FILL_OID_TAB (DS_A_OWNER) },
 { DSP_FILL_OID_TAB (DS_A_ROLE_OCCUPANT) },
 { DSP_FILL_OID_TAB (DS_A_SEE_ALSO) },
 { DSP_FILL_OID_TAB (DS_A_USER_PASSWORD) },

 /* SAP directory attribute types */
 { DSP_FILL_OID_TAB (DS_A_USER_CERT) },
 { DSP_FILL_OID_TAB (DS_A_CA_CERT) },
 { DSP_FILL_OID_TAB (DS_A_AUTHORITY_REVOC_LIST) },
 { DSP_FILL_OID_TAB (DS_A_CERT_REVOC_LIST) },
 { DSP_FILL_OID_TAB (DS_A_CROSS_CERT_PAIR) },
 
 /* X.500 standard object classes */
 { DSP_FILL_OID_TAB (DS_O_TOP) },
 { DSP_FILL_OID_TAB (DS_O_ALIAS) },
 { DSP_FILL_OID_TAB (DS_O_COUNTRY) },
 { DSP_FILL_OID_TAB (DS_O_LOCALITY) },
 { DSP_FILL_OID_TAB (DS_O_ORG) },
 { DSP_FILL_OID_TAB (DS_O_ORG_UNIT) },
 { DSP_FILL_OID_TAB (DS_O_PERSON) },
 { DSP_FILL_OID_TAB (DS_O_ORG_PERSON) },
 { DSP_FILL_OID_TAB (DS_O_ORG_ROLE) },
 { DSP_FILL_OID_TAB (DS_O_GROUP_OF_NAMES) },
 { DSP_FILL_OID_TAB (DS_O_RESIDENTIAL_PERSON) },
 { DSP_FILL_OID_TAB (DS_O_APPLIC_PROCESS) },
 { DSP_FILL_OID_TAB (DS_O_APPLIC_ENTITY) },
 { DSP_FILL_OID_TAB (DS_O_DSA) },
 { DSP_FILL_OID_TAB (DS_O_DEVICE) },

 /* SAP object classes */
 { DSP_FILL_OID_TAB (DS_O_STRONG_AUTHENT_USER) },
 { DSP_FILL_OID_TAB (DS_O_CERT_AUTHORITY) },

 /* X.400 standard object classes */
 { DSP_FILL_OID_TAB (DS_O_MHS_DISTRIBUTION_LIST) },
 { DSP_FILL_OID_TAB (DS_O_MHS_MESSAGE_STORE) },
 { DSP_FILL_OID_TAB (DS_O_MHS_MESSAGE_TRANS_AG) },
 { DSP_FILL_OID_TAB (DS_O_MHS_USER) },
 { DSP_FILL_OID_TAB (DS_O_MHS_USER_AG) },

 /* X.400 standard attribute types */
 { DSP_FILL_OID_TAB (DS_A_DELIV_CONTENT_LENGTH) },
 { DSP_FILL_OID_TAB (DS_A_DELIV_CONTENT_TYPES) },
 { DSP_FILL_OID_TAB (DS_A_DELIV_EITS) },
 { DSP_FILL_OID_TAB (DS_A_DL_MEMBERS) },
 { DSP_FILL_OID_TAB (DS_A_DL_SUBMIT_PERMS) },
 { DSP_FILL_OID_TAB (DS_A_MESSAGE_STORE) },
 { DSP_FILL_OID_TAB (DS_A_OR_ADDRESSES) },
 { DSP_FILL_OID_TAB (DS_A_PREF_DELIV_METHODS) },
 { DSP_FILL_OID_TAB (DS_A_SUPP_AUTO_ACTIONS) },
 { DSP_FILL_OID_TAB (DS_A_SUPP_CONTENT_TYPES) },
 { DSP_FILL_OID_TAB (DS_A_SUPP_OPT_ATTRIBUTES) },

 /* MHS OM classes */
 { DSP_FILL_OID_TAB (MH_C_OR_ADDRESS) },
 { DSP_FILL_OID_TAB (MH_C_OR_NAME) },

 /* DME attribute types */
 { DSP_FILL_OID_TAB (DSX_A_ALTERNATE_ADDRESS) },

 /* DME object classes */
 { DSP_FILL_OID_TAB (DSX_O_DME_NMO_AGENT) },

};
 
#define DSP_OID_TAB_SIZE    (sizeof(dsP_oid_tab) / sizeof(dsP_oid_tab[0]))


static int 
/****************************************************************/
		compare_oid_entry(
				const void * parameter1,
				const void * parameter2)
/****************************************************************/
{
	#define entry1 ((const struct dsP_oid_entry *)parameter1)
	#define entry2 ((const struct dsP_oid_entry *)parameter2)
	OM_sint index,length = MIN(entry1->hex_len, entry2->hex_len);

	for ( index = 0; index < length; index++) {
		if ( entry1->hex_oid[index] < entry2->hex_oid[index]) {
			return (-1);
		} else if ( entry1->hex_oid[index] > entry2->hex_oid[index]) {
			return (1);
		} /* endif */
	} /* endfor */

	if ( entry1->hex_len < entry2->hex_len) {
		return (-1);
	} else if (entry1->hex_len > entry2->hex_len){
		return (1);
	} else {
		return (0);
	} /* endif */

} /* end compare_oid_entry */

void 
dsP_trace_init(
    void)
{
     OM_sint     xds_log;
     mode_t  m;
    long    t;
    char    *xds_log_str;
    char    *path;
    char    logname[D27_LFILE_NAME];

	qsort(dsP_oid_tab,DSP_OID_TAB_SIZE,sizeof(dsP_oid_tab[0]),compare_oid_entry);
 
    if ((xds_log_str = getenv("XDS_LOG")) && (*xds_log_str)) {

        xds_log = atoi(xds_log_str);

        dsP_t_prtobj = xds_log & 0x1;
        dsP_t_prires = xds_log & 0x2;
        dsP_t_oidint = xds_log & 0x4;
        dsP_t_oiddot = xds_log & 0x8;
        dsP_t_stxint = xds_log & 0x10;
        dsP_t_typint = xds_log & 0x20;
        dsP_t_prthex = xds_log & 0x40;

        if ((path = getenv("D2_LOG_DIR")) == (char *) NULL)
            path = getenv("HOME");

        (void) sprintf(logname, "%s/%s.%ld", path, DEFAULT_TRACEFILE, (long)getpid());

        m = umask(0066);
        dsPtfile = fopen(logname, "a");
        umask(m);

        if (dsPtfile == (FILE *)0) {
            char buffer[D27_LFILE_NAME + 30];
            (void) sprintf(buffer, "ERROR: cannot open tracefile: %s ",logname);
            perror(buffer);
            return;
        } /* endif */

        time(&t);
#ifdef THREADSAFE
        {
             OM_sint tid;

            tid = dsP_thread_id();
            (void) fprintf(dsPtfile, "[%d] " HEADERT, tid);
            (void) fprintf(dsPtfile, 
                           "\n[%d] %.20s         PROCESS-ID: %d          "
                           "USER-ID: %s\n[%d] \n", tid, ctime(&t), getpid(),
                           cuserid((char *)0), tid);
        }
#else
        (void) fprintf(dsPtfile, HEADERT);
        (void) fprintf(dsPtfile, "\n%.20s         PROCESS-ID: %d          "
               "USER-ID: %s\n\n", ctime(&t), getpid(), cuserid((char *)0));
#endif
        (void) fflush(dsPtfile);
    } /* endif */

}/* end dsP_trace_init */
 
 

void 
dsP_trace_msg (
    const OM_sint  tid,
    const char *fmt,
    ...)
{
    char    *xds_log_str;
    long    t;
    va_list ap;


    if ((tid == 3) && (! dsP_t_prtobj))
        return;

    if (tid == 2) {
        if ((xds_log_str = getenv("XDS_LOG")) && (*xds_log_str)) {

            if (! dsPtfile) {
                dsP_trace_init();
			} /* endif */
        } else {
            if (dsPtfile) {
                time(&t);
#ifdef THREADSAFE
                {
                     OM_sint thid;

                    thid = dsP_thread_id();
                    (void) fprintf(dsPtfile, 
                            "[%d] \n[%d] %.20s         PROCESS-ID: %d          "
                            "USER-ID: %s\n", thid, thid, ctime(&t), getpid(),
                            cuserid((char *)0));
                    (void) fprintf(dsPtfile, "[%d] " HEADERT "\n", thid);
                }
#else
                (void) fprintf(dsPtfile, "\n%.20s         PROCESS-ID: %d          "
                      "USER-ID: %s\n", ctime(&t), getpid(), cuserid((char *)0));
                (void) fprintf(dsPtfile, HEADERT "\n");
#endif
                (void) fclose(dsPtfile);
                dsPtfile = 0;
            } /* endif */
		} /* endif */
    } /* endif */

    if (dsPtfile != 0) {
        va_start(ap, fmt);

#ifdef THREADSAFE
        if (tid != 0)
            (void) fprintf(dsPtfile, "[%d] ", dsP_thread_id());
#endif
        (void) vfprintf(dsPtfile, fmt, ap);
        (void) fflush(dsPtfile);

        va_end(ap);
    } /* endif */
} /* end dsP_trace_msg */



OM_sint
dsP_display_object(
	const OM_sint       tid,
	const OM_object arg,
	const char     *arg_str)
{
   OM_public_object   tmp;
   OM_value_position  tn;

   if (dsP_t_prtobj) {
   
       dsP_trace_msg(1,"\n");
       dsP_trace_msg(1,"%s = %s\n", arg_str, OPEN);
       if ((! arg) || (! OMP_PRIVATE(arg)))
           dsP_print_descrs(TAB, arg);
       else {
           if ((!dsP_t_prires) && (tid != 0)) {
               dsP_trace_msg(1,"---WARNING: converting to public object for display purposes only---\n");
               if (om_get(arg, OM_NO_EXCLUSIONS, 0, OM_FALSE, 0, 0, &tmp, &tn)
                                                                  != OM_SUCCESS)
                   dsP_trace_msg(1,"---ERROR: cannot convert to public object---\n");
               else {
                   dsP_print_descrs(TAB, tmp);
                   if (om_delete(tmp) != OM_SUCCESS)
                       dsP_trace_msg(1,"---ERROR: cannot delete temporary public object---\n");
               } /* endif */
           } else 
               dsP_print_descrs(TAB, arg);
       } /* endif */
       dsP_trace_msg(1,"%s; /* %s */\n", CLOSE, arg_str);
       dsP_trace_msg(1,"\n");
    } /* endif */
} /* end dsP_display_object */



OM_sint
dsP_display_feature(
	const  DS_feature *f,
	const  char *arg)
{
   if (dsP_t_prtobj) {

       dsP_trace_msg(1,"\n");
       dsP_trace_msg(1,"%s = %s\n", arg, OPEN);

       if (f)
           while (f->feature.length && f->feature.elements) {
               dsP_trace_msg(1,"  %s ",OPEN);
               dsP_print_oid(&f->feature);
               dsP_trace_msg(0,", %s %s,\n",f->activated ? "OM_TRUE" : "OM_FALSE",
                                                                         CLOSE);
               f++;
       } else {
           dsP_trace_msg (1,"  ---WARNING: NULL pointer encountered---\n");
		} /* endif */

       dsP_trace_msg(1,"%s; /* %s */\n", CLOSE, arg);
       dsP_trace_msg(1,"\n");
    } /* endif */
} /* end dsP_display_feature */



OM_sint
dsP_display_integer(
	const  OM_sint i,
	const  char *arg)
{
    if (dsP_t_prtobj) {
        dsP_trace_msg(1,"\n");
        dsP_trace_msg(1,"%s = %d\n", arg, i);
        dsP_trace_msg(1,"\n");
    } /* endif */
} /* end dsP_display_integer */



OM_sint
dsP_display_boolean(
 const OM_sint b,
 const char *arg)
{
    if (dsP_t_prtobj) {
        dsP_trace_msg(1,"\n");
        dsP_trace_msg(1,"%s = %s\n", arg, b ? "OM_TRUE" : "OM_FALSE");
        dsP_trace_msg(1,"\n");
    } /* endif */
} /* end dsP_display_boolean */



OM_sint
dsP_print_descrs(
	const OM_sint       indent_param,
	const OM_object desc_param)
{
    OM_sint i;
    OM_sint nullp;
    OM_sint syntax;
    OM_sint pri_obj = 0;
    OM_sint found = 0;
    char typbuf[MAXTOKLEN];
    char *typ;
    char hi_bits[11];
    OM_syntax s;
	OM_sint indent = indent_param;
	OM_object desc = desc_param;
    struct dsP_typ_entry *entry;
    struct dsP_typ_entry key;

    if ((desc >= ((OM_object)0)) && (desc < ((OM_object)3))) {
        dsP_trace_msg (1,"---WARNING: descriptor pointer is %d ---\n", desc);
        return(1);
    } /* endif */

    for (i=0, nullp=0; ((! pri_obj) && (desc->type != OM_NO_MORE_TYPES));
                                                                  i++, desc++) {

        dsP_trace_msg(1,"%*s", indent, "");
        syntax = desc->syntax & OM_S_SYNTAX;
        s      = desc->syntax;

        /* only local-strings are flagged */
        sprintf(hi_bits, "%s%s%s%s%s", (s & OM_S_PRIVATE) ? "" : "",
                (s & OM_S_SERVICE_GENERATED) ? "" : "",
                (s & OM_S_LOCAL_STRING) ? "+L" : "",
                (s & OM_S_NO_VALUE) ? "" : "",
                (s & OM_S_LONG_STRING) ? "" : "");

        if (desc->type == OM_PRIVATE_OBJECT)
            pri_obj = 1;

        if (! dsP_t_typint) {
            key.int_typ = desc->type;
            if ((entry = (struct dsP_typ_entry *) bsearch(
                              (void *)&key,
                              (void *)dsP_typ_tab,
                              DSP_TYP_TAB_SIZE,
                              sizeof(struct dsP_typ_entry),
                              dsP_compare_typ_index)) != NULL) {
                typ = (char *)entry->sym_typ;
                found = 1;
            } /* endif */
        } /* endif */

        if ((dsP_t_typint) || (! found)) {
            (void) sprintf(typbuf, "%3d", desc->type);
            typ = typbuf;
        } /* endif */

        switch (syntax) {
            case OM_S_OBJECT_IDENTIFIER_STRING:
                if (! dsP_t_stxint) {
                    dsP_trace_msg(0,"%s %s, %s%s, ", OPEN, typ, syntaxes[syntax], hi_bits);
                } else {
                    dsP_trace_msg(0,"%s %s, %d%s, ", OPEN, typ, syntax, hi_bits);
				} /* endif */
                if (desc->value.string.elements) {
                    dsP_print_oid(&desc->value.string);
                    if (pri_obj) {
                        dsP_trace_msg(0," %s ...\n", CLOSE);
                    } else {
                        dsP_trace_msg(0," %s,\n", CLOSE);
					} /* endif */
                } else {
                    nullp=1;
				} /* endif */
                break;

            case OM_S_PRINTABLE_STRING:
            case OM_S_NUMERIC_STRING:
            case OM_S_IA5_STRING:
            case OM_S_VISIBLE_STRING:
            case OM_S_UTC_TIME_STRING:
            case OM_S_GENERALISED_TIME_STRING:
                if (! dsP_t_stxint) {
                    dsP_trace_msg(0,"%s %s, %s%s, ", OPEN, typ, syntaxes[syntax], hi_bits);
                } else {
                    dsP_trace_msg(0,"%s %s, %d%s, ", OPEN, typ, syntax, hi_bits);
				} /* endif */

                if (desc->value.string.elements) {
                    if (desc->value.string.length == OM_LENGTH_UNSPECIFIED) {
                        dsP_trace_msg(0,"\"%s\" %s,\n", desc->value.string.elements,
                                                                         CLOSE);
                    } else {
                        dsP_trace_msg(0,"\"%.*s\" %s,\n",
						desc->value.string.length,
						desc->value.string.elements, CLOSE);
					} /* endif */
                } else {
                    nullp=1;
				} /* endif */
                break;

            case OM_S_OBJECT:
                if (! dsP_t_stxint) {
                    dsP_trace_msg(0,"%s %s, OM_S_OBJECT%s, ", OPEN, typ, hi_bits);
                } else {
                    dsP_trace_msg(0,"%s %s, %d%s, ", OPEN, typ, syntax, hi_bits);
				} /* endif */
                if (desc->value.object.object) {
                    indent+=TAB;
                    dsP_trace_msg(0,"\n");
                    dsP_trace_msg(1,"%*s%s\n", indent, "", OPEN);
                    indent+=TAB;
                    dsP_print_descrs(indent, desc->value.object.object);
                    indent-=TAB;
                    dsP_trace_msg(1,"%*s%s,\n", indent, "", CLOSE);
                    indent-=TAB;
                    dsP_trace_msg(1,"%*s%s,\n", indent, "", CLOSE);
                } else {
                    nullp=1;
				} /* endif */
                break;

            case OM_S_TELETEX_STRING: /* TBD: display escapes as hex */
            {
                 OM_sint            i = 0;
                 unsigned char *c = 
                                 ((unsigned char *)desc->value.string.elements);

                if (c) {
                    if (! dsP_t_stxint) {
                        dsP_trace_msg(0,"%s %s, %s%s, \"", OPEN, typ, syntaxes[syntax], hi_bits);
                        for (i=0; i < desc->value.string.length; i++)
                            if (dsP_t_prthex)
                                dsP_trace_msg(0,"\\x%-2.2X", c[i]);
                            else {
                                if (isprint(c[i]) && (c[i] != '"')) {
                                    dsP_trace_msg(0,"%c", c[i]);
                                } else {
                                    dsP_trace_msg(0,".");
								} /* endif */
                            } /* endif */
                        dsP_trace_msg(0,"\" %s,\n", CLOSE);
                    } else {
                        dsP_trace_msg(0,"%s %s, %d%s, \"%.*s\" %s,\n",
						OPEN, typ, syntax, hi_bits, desc->value.string.length,
						desc->value.string.elements, CLOSE);
					} /* endif */
                } else {
                    nullp=1;
				} /* endif */
                break;
            }

            case OM_S_OCTET_STRING:
            {
                 OM_sint            i = 0;
                 unsigned char *c = 
                                 ((unsigned char *)desc->value.string.elements);

                if (! dsP_t_stxint) {
                    dsP_trace_msg(0,"%s %s, %s%s, ", OPEN, typ,
					syntaxes[syntax], hi_bits);
                } else {
                    dsP_trace_msg(0,"%s %s, %d%s, ", OPEN, typ,
					syntax, hi_bits);
				} /* endif */

                if (c) {
                    if (desc->value.string.length == OM_LENGTH_UNSPECIFIED)
                        dsP_trace_msg(0,"\"%s\" %s,\n", c, CLOSE);
                    else {
                        if (desc->syntax & OM_S_LOCAL_STRING) {
                            dsP_trace_msg(0,"\"%.*s\" %s,\n", 
                                          desc->value.string.length,
                                          desc->value.string.elements, CLOSE);
                        } else {
                            dsP_trace_msg(0,"\"");
                            for (i=0; i < desc->value.string.length; i++)
#if 0
                                if (dsP_t_prthex)
#endif /* 0 */
                                    dsP_trace_msg(0,"\\x%-2.2X", c[i]);
#if 0
                                else {
                                    if (isprint(c[i]) && (c[i] != '"')) {
                                        dsP_trace_msg(0,"%c", c[i]);
                                    } else {
                                        dsP_trace_msg(0,".");
									} /* endif */
                                } /* endif */
#endif /* 0 */
                            dsP_trace_msg(0,"\" %s,\n", CLOSE);
                        } /* endif */
                    } /* endif */
                } else {
                    nullp=1;
				} /* endif */
                break;
            }
            case OM_S_OBJECT_DESCRIPTOR_STRING:
            case OM_S_VIDEOTEX_STRING:
            case OM_S_GRAPHIC_STRING:
            case OM_S_GENERAL_STRING:
            case OM_S_ENCODING_STRING:
            {
                 OM_sint            i = 0;
                 unsigned char *c = 
                                 ((unsigned char *)desc->value.string.elements);

                if (! dsP_t_stxint) {
                    dsP_trace_msg(0,"%s %s, %s%s, ", OPEN, typ,
					syntaxes[syntax], hi_bits);
                } else {
                    dsP_trace_msg(0,"%s %s, %d%s, ", OPEN, typ,
					syntax, hi_bits);
				} /* endif */

                if (c) {
                    if (desc->value.string.length == OM_LENGTH_UNSPECIFIED)
                        dsP_trace_msg(0,"\"%s\" %s,\n", c, CLOSE);
                    else {
                        if (desc->syntax & OM_S_LOCAL_STRING) {
                            dsP_trace_msg(0,"\"%.*s\" %s,\n", 
                                          desc->value.string.length,
                                          desc->value.string.elements, CLOSE);
                        } else {
                            dsP_trace_msg(0,"\"");
                            for (i=0; i < desc->value.string.length; i++)
                                if (dsP_t_prthex)
                                    dsP_trace_msg(0,"\\x%-2.2X", c[i]);
                                else {
                                    if (isprint(c[i]) && (c[i] != '"')) {
                                        dsP_trace_msg(0,"%c", c[i]);
                                    } else {
                                        dsP_trace_msg(0,".");
									} /* endif */
                                } /* endif */
                            dsP_trace_msg(0,"\" %s,\n", CLOSE);
                        } /* endif */
                    } /* endif */
                } else {
                    nullp=1;
				} /* endif */
                break;
            }
            case OM_S_BIT_STRING:
            {
                 OM_sint            i = 0;
                 unsigned char *c = 
				 ((unsigned char *)desc->value.string.elements);

                if (! dsP_t_stxint) {
                    dsP_trace_msg(0,"%s %s, %s%s, ", OPEN, typ,
					syntaxes[syntax], hi_bits);
                } else {
                    dsP_trace_msg(0,"%s %s, %d%s, ", OPEN, typ,
					syntax, hi_bits);
				} /* endif */

                if (c) {
                    if (desc->value.string.length == OM_LENGTH_UNSPECIFIED)
                        dsP_trace_msg(0,"\"%s\" %s,\n", c, CLOSE);
                    else {
                        if (desc->syntax & OM_S_LOCAL_STRING) {
                            dsP_trace_msg(0,"\"%.*s\" %s,\n", 
                                          ((desc->value.string.length + 7)& -8 )/8,
                                          desc->value.string.elements, CLOSE);
                        } else {
                            dsP_trace_msg(0,"\"");
                            for (i=0; i < desc->value.string.length/8; i++)
#if 0
                                if (dsP_t_prthex)
#endif /* 0 */
                                    dsP_trace_msg(0,"\\x%-2.2X", c[i]);
#if 0
                                else {
                                    if (isprint(c[i]) && (c[i] != '"')) {
                                        dsP_trace_msg(0,"%c", c[i]);
                                    } else {
                                        dsP_trace_msg(0,".");
									} /* endif */
                                } /* endif */
#endif /* 0 */
                            dsP_trace_msg(0,"\" %s,\n", CLOSE);
                        } /* endif */
                    } /* endif */
                } else {
                    nullp=1;
				} /* endif */
                break;
            }


            case OM_S_BOOLEAN:
                    if (! dsP_t_stxint) {
                        dsP_trace_msg(0,"%s %s, %s%s, %s %s,\n", OPEN, typ,
             syntaxes[syntax], hi_bits, desc->value.boolean?"OM_TRUE":"OM_FALSE", CLOSE);
                    } else {
                        dsP_trace_msg(0,"%s %s, %d%s, %s %s,\n", OPEN, typ,
						syntax, hi_bits, 
						desc->value.boolean?"OM_TRUE":"OM_FALSE", CLOSE);
					} /* endif */
                break;

            case OM_S_ENUMERATION:
            case OM_S_INTEGER:
                    if (! dsP_t_stxint) {
                        dsP_trace_msg(0,"%s %s, %s%s, %d %s,\n", OPEN, typ,
					  syntaxes[syntax], hi_bits, desc->value.integer, CLOSE);
                    } else {
                        dsP_trace_msg(0,"%s %s, %d%s, %d %s,\n", OPEN, typ,
						syntax, hi_bits, desc->value.integer, CLOSE);
					} /* endif */
                break;

            default:
                dsP_trace_msg(0,"%s %d,\n---WARNING: unrecognised syntax encountered--- %s\n", OPEN, desc->type, CLOSE);
                break;
        } /* endswitch */
        if (nullp)
           nullp=0, dsP_trace_msg (0,"---WARNING: NULL pointer encountered--- },\n");
    }
} /* end dsP_print_descrs */



/* adds the new item "x" within the int array "t";
 * "n": the number of elem within "t"
 * "x": the new item to be introduced
 * "m1": the decimal base of "x"
 * "m2": the decimal base of the result "t"
 * the array "t" contains integer representation in the "m2" decimal base,
 * but in reverse order.
 * ex:  (from base 10 to base 128)
 *      input-> t:      85
                n:      1
                x:      7
                m1:     10
                m2:     128
        output-> t:     89      6
        returned  :     2

        (6 * 128 + 89 = 857)
 */
static OM_sint
d27_one_digit_more(
    OM_sint *t,
    OM_sint n,
    OM_sint x,
    OM_sint m1,
    OM_sint m2)
{
		OM_sint i;

        if (n == 0){
                n = 1;
                t[0] = x;
        }else{  for (i=0;i<n;i++)       t[i] *= m1;
                t[0] += x;
        } /* endif */

        i =0;
        while (i<n){
                if (t[i] >= m2){
                        if (i == (n-1))
                                t[n++] = t[i]/m2;
                        else    t[i+1] += t[i]/m2;
                        t[i] = t[i]%m2;
                } /* endif */
                i++;
        } /* endwhile */
        return(n);
} /* end d27_one_digit_more */



/* from  "D2_obj_id" to digital string representation.
 * "l" bytes from the input parameter "z" will be interpreted.
 */
static OM_sint
d27_oid_digits(
    unsigned char * z,
    OM_sint l,
    char * buf)
{
        OM_sint i = 0, n;
        OM_sint t[32];

        while (i<l){
                *buf++ = '.';
                n = 0;
                while(i<l){
                        n = d27_one_digit_more(t,n,z[i]&0x7f,128,10);
                        if ((z[i++]&0x80) == 0) break;
                } /* endwhile */
                while (n)
                        *buf++ = '0' + t[--n];
        } /* endwhile */
        *buf = 0;

        return(0);
}/* end d27_oid_digits */


OM_sint
dsP_print_oid(
	const OM_string *str)
{
   static unsigned char    buf[64];
    OM_sint            i;
    OM_uint		   len;
    OM_uint		   found = 0;
    struct dsP_oid_entry     *entry;
    struct dsP_oid_entry      key;
    unsigned char  *c = ((unsigned char *)str->elements);

	if (str->length == OM_LENGTH_UNSPECIFIED) {
       len = strlen(str->elements);
	} else {
       len = str->length;
	} /* endif */

   if (len < 2) {
       dsP_trace_msg(0,"---ERROR: invalid object identifier--- ");
       return(0);
   } /* endif */

   if (! dsP_t_oidint) {
        key.hex_oid = str->elements;
        key.hex_len = len;
        if ((entry = (struct dsP_oid_entry *)
                      bsearch((void *)&key, 
                              (void *)dsP_oid_tab,
                              DSP_OID_TAB_SIZE,
                              sizeof(struct dsP_oid_entry),
                              compare_oid_entry)) != NULL) {
            dsP_trace_msg(0,"%s", (char *)entry->symbol_oid);
            found = 1;
         } /* endif */
   } /* endif */

   if ((dsP_t_oidint) || (! found)) {
       if (dsP_t_oiddot) {
           dsP_trace_msg(0,"%d.%d", *c/40, *c-(*c/40*40));
           d27_oid_digits (c+1, len-1, (char *)buf);
           dsP_trace_msg (0,"%s", buf);
       } else {
           dsP_trace_msg(0,"\"");
           for (i=0; i < len; i++)
               dsP_trace_msg(0,"\\x%-2.2X", c[i]);
           dsP_trace_msg(0,"\"");
       } /* endif */
   } /* endif */
} /* end dsP_print_oid */


static
OM_sint
dsP_compare_typ_index(
    register const void *pkey,
    register const void *pentry)
{
    if (((struct dsP_typ_entry *)pkey)->int_typ > 
	((struct dsP_typ_entry *)pentry)->int_typ) {
        return (1);
    } else {
        if (((struct dsP_typ_entry *)pkey)->int_typ < 
		((struct dsP_typ_entry *)pentry)->int_typ) {
            return (-1);
        } else {
            return (0);
		} /* endif */
	} /* endif */
} /* end dsP_compare_typ_index */

#endif  /* TRACE */
