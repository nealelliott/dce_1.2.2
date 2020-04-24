/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: xmhp.h,v $
 * Revision 1.1.10.2  1996/02/18  23:38:09  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:31:17  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:37:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:36  root]
 * 
 * Revision 1.1.8.1  1994/02/22  19:00:11  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:06:25  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  07:12:43  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:22:00  marrek]
 * 
 * Revision 1.1.4.2  1992/12/30  19:31:27  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  18:39:44  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  19:46:09  zeliff
 * 	Commenting out non-ANSI #ident
 * 	[1992/05/28  15:52:56  zeliff]
 * 
 * 	New files from GDS drop
 * 	[1992/05/27  19:45:01  zeliff]
 * 
 * $EndLog$
 */
#ifndef _XMHP_H
#define _XMHP_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xmhp_rcsid[] = "@(#)$RCSfile: xmhp.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:38:09 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/

/*===================================*/
/* MH package object identifier      */
/*===================================*/
#define	OMP_O_MH_PACKAGE "\x56\x06\x01\x02\x05\x0B"

/* BEGIN MH PORTION OF INTERFACE */

/* SYMBOLIC CONSTANTS */

/* Class */

#define OMP_O_MH_C_ALGORITHM		"\x56\x06\x01\x02\x05\x0B\x00"
#define OMP_O_MH_C_ALGORITHM_AND_RESULT	"\x56\x06\x01\x02\x05\x0B\x01"
#define OMP_O_MH_C_ASYMMETRIC_TOKEN	"\x56\x06\x01\x02\x05\x0B\x02"
#define OMP_O_MH_C_BILATERAL_INFO	"\x56\x06\x01\x02\x05\x0B\x03"
#define OMP_O_MH_C_COMMUNIQUE		"\x56\x06\x01\x02\x05\x0B\x04"
#define OMP_O_MH_C_CONTENT		"\x56\x06\x01\x02\x05\x0B\x05"
#define OMP_O_MH_C_DELIV_MESSAGE	"\x56\x06\x01\x02\x05\x0B\x06"
#define OMP_O_MH_C_DELIV_PER_RECIP_DR	"\x56\x06\x01\x02\x05\x0B\x07"
#define OMP_O_MH_C_DELIV_PER_RECIP_NDR	"\x56\x06\x01\x02\x05\x0B\x08"
#define OMP_O_MH_C_DELIV_PER_RECIP_REP	"\x56\x06\x01\x02\x05\x0B\x09"
#define OMP_O_MH_C_DELIV_REPORT		"\x56\x06\x01\x02\x05\x0B\x0A"
/* 
 * Note: the following two names are not in alphabetical order to retain
 * the value assignment as publised in X.400 API Version 2.0
 */
#define OMP_O_MH_C_DELIVERY_CONFIRM	"\x56\x06\x01\x02\x05\x0B\x0B"
#define OMP_O_MH_C_DELIVERY_ENVELOPE	"\x56\x06\x01\x02\x05\x0B\x0C"
#define OMP_O_MH_C_EITS			"\x56\x06\x01\x02\x05\x0B\x0D"
#define OMP_O_MH_C_EXPANSION_RECORD	"\x56\x06\x01\x02\x05\x0B\x0E"
#define OMP_O_MH_C_EXTENSIBLE_OBJECT	"\x56\x06\x01\x02\x05\x0B\x0F"
#define OMP_O_MH_C_EXTENSION		"\x56\x06\x01\x02\x05\x0B\x10"
#define OMP_O_MH_C_EXTERNAL_TRACE_ENTRY	"\x56\x06\x01\x02\x05\x0B\x11"
#define OMP_O_MH_C_G3_FAX_NBPS		"\x56\x06\x01\x02\x05\x0B\x12"
#define OMP_O_MH_C_GENERAL_CONTENT	"\x56\x06\x01\x02\x05\x0B\x13"
#define OMP_O_MH_C_INTERNAL_TRACE_ENTRY	"\x56\x06\x01\x02\x05\x0B\x14"
#define OMP_O_MH_C_LOCAL_DELIV_CONFIRM	"\x56\x06\x01\x02\x05\x0B\x15"
#define OMP_O_MH_C_LOCAL_DELIV_CONFIRMS	"\x56\x06\x01\x02\x05\x0B\x16"
#define OMP_O_MH_C_LOCAL_NDR		"\x56\x06\x01\x02\x05\x0B\x17"
#define OMP_O_MH_C_LOCAL_PER_RECIP_NDR	"\x56\x06\x01\x02\x05\x0B\x18"
#define OMP_O_MH_C_MESSAGE		"\x56\x06\x01\x02\x05\x0B\x19"
#define OMP_O_MH_C_MESSAGE_RD		"\x56\x06\x01\x02\x05\x0B\x1A"
#define OMP_O_MH_C_MTS_IDENTIFIER	"\x56\x06\x01\x02\x05\x0B\x1B"
#define OMP_O_MH_C_OR_ADDRESS		"\x56\x06\x01\x02\x05\x0B\x1C"
#define OMP_O_MH_C_OR_NAME		"\x56\x06\x01\x02\x05\x0B\x1D"
#define OMP_O_MH_C_PER_RECIP_DR		"\x56\x06\x01\x02\x05\x0B\x1E"
#define OMP_O_MH_C_PER_RECIP_NDR	"\x56\x06\x01\x02\x05\x0B\x1F"
#define OMP_O_MH_C_PER_RECIP_REPORT	"\x56\x06\x01\x02\x05\x0B\x20"
#define OMP_O_MH_C_PROBE		"\x56\x06\x01\x02\x05\x0B\x21"
#define OMP_O_MH_C_PROBE_RD		"\x56\x06\x01\x02\x05\x0B\x22"
#define OMP_O_MH_C_RD			"\x56\x06\x01\x02\x05\x0B\x23"
#define OMP_O_MH_C_REDIRECTION_RECORD	"\x56\x06\x01\x02\x05\x0B\x24"
#define OMP_O_MH_C_REPORT		"\x56\x06\x01\x02\x05\x0B\x25"
#define OMP_O_MH_C_SECURITY_LABEL	"\x56\x06\x01\x02\x05\x0B\x26"
#define OMP_O_MH_C_SESSION		"\x56\x06\x01\x02\x05\x0B\x27"
#define OMP_O_MH_C_SUBMISSION_RESULTS	"\x56\x06\x01\x02\x05\x0B\x28"
#define OMP_O_MH_C_SUBMITTED_COMMUNIQUE	"\x56\x06\x01\x02\x05\x0B\x29"
#define OMP_O_MH_C_SUBMITTED_MESSAGE	"\x56\x06\x01\x02\x05\x0B\x2A"
#define OMP_O_MH_C_SUBMITTED_MESSAGE_RD	"\x56\x06\x01\x02\x05\x0B\x2B"
#define OMP_O_MH_C_SUBMITTED_PROBE	"\x56\x06\x01\x02\x05\x0B\x2C"
#define OMP_O_MH_C_SUBMITTED_PROBE_RD	"\x56\x06\x01\x02\x05\x0B\x2D"
#define OMP_O_MH_C_TELETEX_NBPS		"\x56\x06\x01\x02\x05\x0B\x2E"
#define OMP_O_MH_C_DELIVERY_REPORT	"\x56\x06\x01\x02\x05\x0B\x2F"
#define OMP_O_MH_C_MT_PUBLIC_DATA	"\x56\x06\x01\x02\x05\x0B\x30"
#define OMP_O_MH_C_TOKEN_PUBLIC_DATA	"\x56\x06\x01\x02\x05\x0B\x31"

/* Enumeration */

	/* Action */
#define MH_AC_EXPANDED		( (OM_enumeration) -2 )
#define MH_AC_REDIRECTED	( (OM_enumeration) -1 )
#define MH_AC_RELAYED		( (OM_enumeration) 0 )
#define MH_AC_REROUTED		( (OM_enumeration) 1 )

	/* Builtin EIT */
#define MH_BE_UNDEFINED		( (OM_enumeration) 0 )
#define MH_BE_TELEX		( (OM_enumeration) 1 )
#define MH_BE_IA5_TEXT		( (OM_enumeration) 2 )
#define MH_BE_G3_FAX		( (OM_enumeration) 3 )
#define MH_BE_G4_CLASS1		( (OM_enumeration) 4 )
#define MH_BE_TELETEX		( (OM_enumeration) 5 )
#define MH_BE_VIDEOTEX		( (OM_enumeration) 6 )
#define MH_BE_MIXED_MODE	( (OM_enumeration) 9 )
#define MH_BE_ISO_6937_TEXT	( (OM_enumeration) 11 )

	/* Delivery Mode */
#define MH_DM_ANY		( (OM_enumeration) 0 )
#define MH_DM_MTS		( (OM_enumeration) 1 )
#define MH_DM_PDS		( (OM_enumeration) 2 )
#define MH_DM_TELEX		( (OM_enumeration) 3 )
#define MH_DM_TELETEX		( (OM_enumeration) 4 )
#define MH_DM_G3_FAX		( (OM_enumeration) 5 )
#define MH_DM_G4_FAX		( (OM_enumeration) 6 )
#define MH_DM_IA5_TERMINAL	( (OM_enumeration) 7 )
#define MH_DM_VIDEOTEX		( (OM_enumeration) 8 )
#define MH_DM_TELEPHONE		( (OM_enumeration) 9 )

	/* Delivery Point */
#define MH_DP_PUBLIC_UA		( (OM_enumeration) 0 )
#define MH_DP_PRIVATE_UA	( (OM_enumeration) 1 )
#define MH_DP_MS		( (OM_enumeration) 2 )
#define MH_DP_DL		( (OM_enumeration) 3 )
#define MH_DP_PDAU		( (OM_enumeration) 4 )
#define MH_DP_PDS_PATRON	( (OM_enumeration) 5 )
#define MH_DP_OTHER_AU		( (OM_enumeration) 6 )

	/* Diagnostic */
#define MH_DG_NO_DIAGNOSTIC		( (OM_enumeration) -1 )
#define MH_DG_OR_NAME_UNRECOGNIZED	( (OM_enumeration) 0 )
#define MH_DG_OR_NAME_AMBIGUOUS		( (OM_enumeration) 1 )
#define MH_DG_MTS_CONGESTED		( (OM_enumeration) 2 )
#define MH_DG_LOOP_DETECTED		( (OM_enumeration) 3 )
#define MH_DG_RECIPIENT_UNAVAILABLE	( (OM_enumeration) 4 )
#define MH_DG_MAXIMUM_TIME_EXPIRED	( (OM_enumeration) 5 )
#define MH_DG_EITS_UNSUPPORTED		( (OM_enumeration) 6 )
#define MH_DG_CONTENT_TOO_LONG		( (OM_enumeration) 7 )
#define MH_DG_IMPRACTICAL_TO_CONVERT	( (OM_enumeration) 8 )
#define MH_DG_PROHIBITED_TO_CONVERT	( (OM_enumeration) 9 )
#define MH_DG_CONVERSION_UNSUBSCRIBED	( (OM_enumeration) 10 )
#define MH_DG_PARAMETERS_INVALID	( (OM_enumeration) 11 )
#define MH_DG_CONTENT_SYNTAX_IN_ERROR	( (OM_enumeration) 12 )
#define MH_DG_LENGTH_CONSTRAINT_VIOLATD	( (OM_enumeration) 13 )
#define MH_DG_NUMBER_CONSTRAINT_VIOLATD	( (OM_enumeration) 14 )
#define MH_DG_CONTENT_TYPE_UNSUPPORTED	( (OM_enumeration) 15 )
#define MH_DG_TOO_MANY_RECIPIENTS	( (OM_enumeration) 16 )
#define MH_DG_NO_BILATERAL_AGREEMENT	( (OM_enumeration) 17 )
#define MH_DG_CRITICAL_FUNC_UNSUPPORTED	( (OM_enumeration) 18 )
#define MH_DG_CONVERSION_LOSS_PROHIB	( (OM_enumeration) 19 )
#define MH_DG_LINE_TOO_LONG		( (OM_enumeration) 20 )
#define MH_DG_PAGE_TOO_LONG		( (OM_enumeration) 21 )
#define MH_DG_PICTORIAL_SYMBOL_LOST	( (OM_enumeration) 22 )
#define MH_DG_PUNCTUATION_SYMBOL_LOST	( (OM_enumeration) 23 )
#define MH_DG_ALPHABETIC_CHARACTER_LOST	( (OM_enumeration) 24 )
#define MH_DG_MULTIPLE_INFO_LOSSES	( (OM_enumeration) 25 )
#define MH_DG_REASSIGNMENT_PROHIBITED	( (OM_enumeration) 26 )
#define MH_DG_REDIRECTION_LOOP_DETECTED	( (OM_enumeration) 27 )
#define MH_DG_EXPANSION_PROHIBITED	( (OM_enumeration) 28 )
#define MH_DG_SUBMISSION_PROHIBITED	( (OM_enumeration) 29 )
#define MH_DG_EXPANSION_FAILED		( (OM_enumeration) 30 )
#define MH_DG_RENDITION_UNSUPPORTED	( (OM_enumeration) 31 )
#define MH_DG_MAIL_ADDRESS_INCORRECT	( (OM_enumeration) 32 )
#define MH_DG_MAIL_OFFICE_INCOR_OR_INVD	( (OM_enumeration) 33 )
#define MH_DG_MAIL_ADDRESS_INCOMPLETE	( (OM_enumeration) 34 )
#define MH_DG_MAIL_RECIPIENT_UNKNOWN	( (OM_enumeration) 35 )
#define MH_DG_MAIL_RECIPIENT_DECEASED	( (OM_enumeration) 36 )
#define MH_DG_MAIL_ORGANIZATION_EXPIRED	( (OM_enumeration) 37 )
#define MH_DG_MAIL_REFUSED		( (OM_enumeration) 38 )
#define MH_DG_MAIL_UNCLAIMED		( (OM_enumeration) 39 )
#define MH_DG_MAIL_RECIPIENT_MOVED	( (OM_enumeration) 40 )
#define MH_DG_MAIL_RECIPIENT_TRAVELLING	( (OM_enumeration) 41 )
#define MH_DG_MAIL_RECIPIENT_DEPARTED	( (OM_enumeration) 42 )
#define MH_DG_MAIL_NEW_ADDRESS_UNKNOWN	( (OM_enumeration) 43 )
#define MH_DG_MAIL_FORWARDING_UNWANTED	( (OM_enumeration) 44 )
#define MH_DG_MAIL_FORWARDING_PROHIB	( (OM_enumeration) 45 )
#define MH_DG_SECURE_MESSAGING_ERROR	( (OM_enumeration) 46 )
#define MH_DG_DOWNGRADING_IMPOSSIBLE	( (OM_enumeration) 47 )

	/* Explicit Conversion */
#define MH_EC_NO_CONVERSION		( (OM_enumeration) -1 )
#define MH_EC_IA5_TEXT_TO_TELETEX	( (OM_enumeration) 0 )
#define MH_EC_TELETEX_TO_TELEX		( (OM_enumeration) 1 )
#define MH_EC_TELEX_TO_IA5_TEXT		( (OM_enumeration) 2 )
#define MH_EC_TELEX_TO_TELETEX		( (OM_enumeration) 3 )
#define MH_EC_TELEX_TO_G4_CLASS1	( (OM_enumeration) 4 )
#define MH_EC_TELEX_TO_VIDEOTEX		( (OM_enumeration) 5 )
#define MH_EC_IA5_TEXT_TO_TELEX		( (OM_enumeration) 6 )
#define MH_EC_TELEX_TO_G3_FAX		( (OM_enumeration) 7 )
#define MH_EC_IA5_TEXT_TO_G3_FAX	( (OM_enumeration) 8 )
#define MH_EC_IA5_TEXT_TO_G4_CLASS1	( (OM_enumeration) 9 )
#define MH_EC_IA5_TEXT_TO_VIDEOTEX	( (OM_enumeration) 10 )
#define MH_EC_TELETEX_TO_IA5_TEXT	( (OM_enumeration) 11 )
#define MH_EC_TELETEX_TO_G3_FAX		( (OM_enumeration) 12 )
#define MH_EC_TELETEX_TO_G4_CLASS1	( (OM_enumeration) 13 )
#define MH_EC_TELETEX_TO_VIDEOTEX	( (OM_enumeration) 14 )
#define MH_EC_VIDEOTEX_TO_TELEX		( (OM_enumeration) 15 )
#define MH_EC_VIDEOTEX_TO_IA5_TEXT	( (OM_enumeration) 16 )
#define MH_EC_VIDEOTEX_TO_TELETEX	( (OM_enumeration) 17 )

	/* Postal Mode */
#define MH_PM_ORDINARY_MAIL		( (OM_enumeration) 0 )
#define MH_PM_SPECIAL_DELIVERY		( (OM_enumeration) 1 )
#define MH_PM_EXPRESS_MAIL		( (OM_enumeration) 2 )
#define MH_PM_CC			( (OM_enumeration) 3 )
#define MH_PM_CC_WITH_TELEPHONE_ADVICE	( (OM_enumeration) 4 )
#define MH_PM_CC_WITH_TELEX_ADVICE	( (OM_enumeration) 5 )
#define MH_PM_CC_WITH_TELETEX_ADVICE	( (OM_enumeration) 6 )

	/* Postal Report */
#define MH_PR_UNDELIVBLE_MAIL_VIA_PDS	( (OM_enumeration) 0 )
#define MH_PR_NOTIFICN_VIA_PDS		( (OM_enumeration) 1 )
#define MH_PR_NOTIFICN_VIA_MTS		( (OM_enumeration) 2 )
#define MH_PR_NOTIFICN_VIA_MTS_AND_PDS	( (OM_enumeration) 3 )

	/* Priority */
#define MH_PTY_NORMAL			( (OM_enumeration) 0 )
#define MH_PTY_LOW			( (OM_enumeration) 1 )
#define MH_PTY_URGENT			( (OM_enumeration) 2 )

	/* Reason */
#define MH_RE_TRANSFER_FAILED		( (OM_enumeration) 0 )
#define MH_RE_TRANSFER_IMPOSSIBLE	( (OM_enumeration) 1 )
#define MH_RE_CONVERSION_NOT_PERFORMED	( (OM_enumeration) 2 )
#define MH_RE_PHYSICAL_RENDITN_NOT_DONE	( (OM_enumeration) 3 )
#define MH_RE_PHYSICAL_DELIV_NOT_DONE	( (OM_enumeration) 4 )
#define MH_RE_RESTRICTED_DELIVERY	( (OM_enumeration) 5 )
#define MH_RE_DIRECTORY_OPERATN_FAILED	( (OM_enumeration) 6 )

	/* Redirection Reason */
#define MH_RR_RECIPIENT_ASSIGNED	( (OM_enumeration) 0 )
#define MH_RR_ORIGINATOR_REQUESTED	( (OM_enumeration) 1 )
#define MH_RR_RECIPIENT_DOMAIN_ASSIGNED	( (OM_enumeration) 2 )

	/* Registration */
#define MH_RG_UNREGISTERED_MAIL		( (OM_enumeration) 0 )
#define MH_RG_REGISTERED_MAIL		( (OM_enumeration) 1 )
#define MH_RG_REGISTERED_MAIL_IN_PERSON	( (OM_enumeration) 2 )

	/* Report Request */
#define MH_RQ_NEVER			( (OM_enumeration) 0 )
#define MH_RQ_NON_DELIVERY		( (OM_enumeration) 1 )
#define MH_RQ_ALWAYS			( (OM_enumeration) 2 )
#define MH_RQ_ALWAYS_AUDITED		( (OM_enumeration) 3 )

	/* Security Classification */
#define MH_SC_UNMARKED			( (OM_enumeration) 0 )
#define MH_SC_UNCLASSIFIED		( (OM_enumeration) 1 )
#define MH_SC_RESTRICTED		( (OM_enumeration) 2 )
#define MH_SC_CONFIDENTIAL		( (OM_enumeration) 3 )
#define MH_SC_SECRET			( (OM_enumeration) 4 )
#define MH_SC_TOP_SECRET		( (OM_enumeration) 5 )

	/* Terminal Type */
#define MH_TT_TELEX			( (OM_enumeration) 3 )
#define MH_TT_TELETEX			( (OM_enumeration) 4 )
#define MH_TT_G3_FAX			( (OM_enumeration) 5 )
#define MH_TT_G4_FAX			( (OM_enumeration) 6 )
#define MH_TT_IA5_TERMINAL		( (OM_enumeration) 7 )
#define MH_TT_VIDEOTEX			( (OM_enumeration) 8 )

/* Integer */

	/* Content Type */
#define MH_CTI_UNIDENTIFIED		( (OM_integer) 0 )
#define MH_CTI_EXTERNAL			( (OM_integer) 1 )
#define MH_CTI_P2_1984			( (OM_integer) 2 )
#define MH_CTI_P2_1988			( (OM_integer) 22 )

/* Object Identifier (Elements component) */

	/* Content Type */
#define OMP_O_MH_CTO_INNER_MESSAGE	"\x56\x03\x03\x01"
#define OMP_O_MH_CTO_UNIDENTIFIED	"\x56\x03\x03\x00"

	/* External EITs */
#define OMP_O_MH_EE_G3_FAX		"\x56\x03\x04\x03"
#define OMP_O_MH_EE_G4_CLASS_1		"\x56\x03\x04\x04"
#define OMP_O_MH_EE_IA5_TEXT		"\x56\x03\x04\x02"
#define OMP_O_MH_EE_MIXED_MODE		"\x56\x03\x04\x09"
#define OMP_O_MH_EE_TELETEX		"\x56\x03\x04\x05"
#define OMP_O_MH_EE_TELEX		"\x56\x03\x04\x01"
#define OMP_O_MH_EE_UNDEFINED		"\x56\x03\x04\x00"
#define OMP_O_MH_EE_VIDEOTEX		"\x56\x03\x04\x06"

	/* Rendition Attributes */
#define OMP_O_MH_RA_BASIC_RENDITION	"\x56\x03\x05\x00"

/* Type */

#define MH_T_A3_WIDTH 			( (OM_type) 200 )
#define MH_T_ACTION			( (OM_type) 201 )
#define MH_T_ACTUAL_RECIPIENT_NAME	( (OM_type) 202 )
#define MH_T_ADMD_NAME			( (OM_type) 203 )
#define MH_T_ALGORITHM_DATUM		( (OM_type) 204 )
#define MH_T_ALGORITHM_ID		( (OM_type) 205 )
#define MH_T_ALGORITHM_RESULT		( (OM_type) 206 )
#define MH_T_ALTERNATE_RECIP_ALLOWED	( (OM_type) 207 )
#define MH_T_ALTERNATE_RECIPIENT_NAME	( (OM_type) 208 )
#define MH_T_ARRIVAL_TIME		( (OM_type) 209 )
#define MH_T_ATTEMPTED_ADMD_NAME	( (OM_type) 210 )
#define MH_T_ATTEMPTED_COUNTRY_NAME	( (OM_type) 211 )
#define MH_T_ATTEMPTED_MTA_NAME		( (OM_type) 212 )
#define MH_T_ATTEMPTED_PRMD_IDENTIFIER	( (OM_type) 213 )
#define MH_T_B4_LENGTH			( (OM_type) 214 )
#define MH_T_B4_WIDTH			( (OM_type) 215 )
#define MH_T_BILATERAL_INFORMATION	( (OM_type) 216 )
#define MH_T_BINARY_CONTENT		( (OM_type) 217 )
#define MH_T_BUILTIN_EITS		( (OM_type) 218 )
#define MH_T_BUREAU_FAX_DELIVERY	( (OM_type) 219 )
#define MH_T_COMMON_NAME		( (OM_type) 220 )
#define MH_T_CONFIDENTIALITY_ALGORITHM	( (OM_type) 221 )
#define MH_T_CONFIDENTIALITY_KEY	( (OM_type) 222 )
#define MH_T_CONTENT			( (OM_type) 223 )
#define MH_T_CONTENT_CORRELATOR		( (OM_type) 224 )
#define MH_T_CONTENT_EXTENSIONS		( (OM_type) 225 )
#define MH_T_CONTENT_IDENTIFIER		( (OM_type) 226 )
#define MH_T_CONTENT_LENGTH		( (OM_type) 227 )
#define MH_T_CONTENT_RETURN_REQUESTED	( (OM_type) 228 )
#define MH_T_CONTENT_TYPE		( (OM_type) 229 )
#define MH_T_CONTROL_CHARACTER_SETS	( (OM_type) 230 )
#define MH_T_CONVERSION_LOSS_PROHIBITED	( (OM_type) 231 )
#define MH_T_CONVERSION_PROHIBITED	( (OM_type) 232 )
#define MH_T_CONVERTED_EITS		( (OM_type) 233 )
#define MH_T_COUNTRY_NAME		( (OM_type) 234 )
#define MH_T_CRITICAL_FOR_DELIVERY 	( (OM_type) 235 )
#define MH_T_CRITICAL_FOR_SUBMISSION 	( (OM_type) 236 )
#define MH_T_CRITICAL_FOR_TRANSFER 	( (OM_type) 237 )
#define MH_T_DEFERRED_DELIVERY_TIME	( (OM_type) 238 )
#define MH_T_DEFERRED_TIME		( (OM_type) 239 )
#define MH_T_DELIVERY_CONFIRMATIONS	( (OM_type) 240 )
#define MH_T_DELIVERY_POINT		( (OM_type) 241 )
#define MH_T_DELIVERY_TIME		( (OM_type) 242 )
#define MH_T_DIRECTORY_NAME		( (OM_type) 243 )
#define MH_T_DISCLOSURE_ALLOWED		( (OM_type) 244 )
#define MH_T_DISTINGUISHED_RECIP_ADDR	( (OM_type) 245 )
#define MH_T_DOMAIN_TYPE_1		( (OM_type) 246 )
#define MH_T_DOMAIN_TYPE_2		( (OM_type) 247 )
#define MH_T_DOMAIN_TYPE_3		( (OM_type) 248 )
#define MH_T_DOMAIN_TYPE_4		( (OM_type) 249 )
#define MH_T_DOMAIN_VALUE_1		( (OM_type) 250 )
#define MH_T_DOMAIN_VALUE_2		( (OM_type) 251 )
#define MH_T_DOMAIN_VALUE_3		( (OM_type) 252 )
#define MH_T_DOMAIN_VALUE_4		( (OM_type) 253 )
#define MH_T_ENVELOPES			( (OM_type) 254 )
#define MH_T_EVENT_HANDLE		( (OM_type) 255 )
#define MH_T_EXPANSION_HISTORY		( (OM_type) 256 )
#define MH_T_EXPANSION_PROHIBITED	( (OM_type) 257 )
#define MH_T_EXPLICIT_CONVERSION	( (OM_type) 258 )
#define MH_T_EXTENSION_TYPE 		( (OM_type) 259 )
#define MH_T_EXTENSION_VALUE 		( (OM_type) 260 )
#define MH_T_EXTENSIONS 		( (OM_type) 261 )
#define MH_T_EXTERNAL_EITS		( (OM_type) 262 )
#define MH_T_EXTERNAL_TRACE_INFO	( (OM_type) 263 )
#define MH_T_FINE_RESOLUTION		( (OM_type) 264 )
#define MH_T_FORWARDING_ADDRESS		( (OM_type) 265 )
#define MH_T_FORWARDING_ADDR_REQUESTED	( (OM_type) 266 )
#define MH_T_FORWARDING_PROHIBITED	( (OM_type) 267 )
#define MH_T_G3_FAX_NBPS		( (OM_type) 268 )
#define MH_T_G4_FAX_NBPS		( (OM_type) 269 )
#define MH_T_GENERATION			( (OM_type) 270 )
#define MH_T_GIVEN_NAME			( (OM_type) 271 )
#define MH_T_GRAPHIC_CHARACTER_SETS	( (OM_type) 272 )
#define MH_T_INFORMATION		( (OM_type) 273 )
#define MH_T_INITIALS			( (OM_type) 274 )
#define MH_T_INTEGRITY_CHECK		( (OM_type) 275 )
#define MH_T_INTENDED_RECIPIENT_NAME	( (OM_type) 276 )
#define MH_T_INTENDED_RECIPIENT_NUMBER	( (OM_type) 277 )
#define MH_T_INTERNAL_TRACE_INFO	( (OM_type) 278 )
#define MH_T_ISDN_NUMBER		( (OM_type) 279 )
#define MH_T_ISDN_SUBADDRESS		( (OM_type) 280 )
#define MH_T_LATEST_DELIVERY_TIME	( (OM_type) 281 )
#define MH_T_LOCAL_IDENTIFIER		( (OM_type) 282 )
#define MH_T_MESSAGE_SEQUENCE_NUMBER	( (OM_type) 283 )
#define MH_T_MISCELLANEOUS_CAPABILITIES	( (OM_type) 284 )
#define MH_T_MTA_CERTIFICATE		( (OM_type) 285 )
#define MH_T_MTA_NAME			( (OM_type) 286 )
#define MH_T_MTA_REPORT_REQUEST		( (OM_type) 287 )
#define MH_T_MTA_RESPONSIBILITY		( (OM_type) 288 )
#define MH_T_MTS_IDENTIFIER		( (OM_type) 289 )
#define MH_T_NAME			( (OM_type) 290 )
#define MH_T_NON_DELIVERY_DIAGNOSTIC	( (OM_type) 291 )
#define MH_T_NON_DELIVERY_REASON	( (OM_type) 292 )
#define MH_T_NUMERIC_USER_IDENTIFIER	( (OM_type) 293 )
#define MH_T_ORGANIZATION_NAME		( (OM_type) 294 )
#define MH_T_ORGANIZATIONAL_UNIT_NAME_1	( (OM_type) 295 )
#define MH_T_ORGANIZATIONAL_UNIT_NAME_2	( (OM_type) 296 )
#define MH_T_ORGANIZATIONAL_UNIT_NAME_3	( (OM_type) 297 )
#define MH_T_ORGANIZATIONAL_UNIT_NAME_4	( (OM_type) 298 )
#define MH_T_ORIG_AND_EXPANSION_HISTORY	( (OM_type) 299 )
#define MH_T_ORIGIN_CHECK		( (OM_type) 300 )
#define MH_T_ORIGINAL_EITS		( (OM_type) 301 )
#define MH_T_ORIGINALLY_INTENDED_RECIP	( (OM_type) 302 )
#define MH_T_ORIGINATOR_CERTIFICATE	( (OM_type) 303 )
#define MH_T_ORIGINATOR_NAME		( (OM_type) 304 )
#define MH_T_ORIGINATOR_REPORT_REQUEST	( (OM_type) 305 )
#define MH_T_ORIGINATOR_RETURN_ADDRESS	( (OM_type) 306 )
#define MH_T_OTHER_RECIPIENT_NAMES	( (OM_type) 307 )
#define MH_T_PAGE_FORMATS		( (OM_type) 308 )
#define MH_T_PER_RECIPIENT_REPORTS	( (OM_type) 309 )
#define MH_T_POSTAL_ADDRESS_DETAILS	( (OM_type) 310 )
#define MH_T_POSTAL_ADDRESS_IN_FULL	( (OM_type) 311 )
#define MH_T_POSTAL_ADDRESS_IN_LINES	( (OM_type) 312 )
#define MH_T_POSTAL_CODE		( (OM_type) 313 )
#define MH_T_POSTAL_COUNTRY_NAME	( (OM_type) 314 )
#define MH_T_POSTAL_DELIVERY_POINT_NAME	( (OM_type) 315 )
#define MH_T_POSTAL_DELIV_SYSTEM_NAME	( (OM_type) 316 )
#define MH_T_POSTAL_GENERAL_DELIV_ADDR	( (OM_type) 317 )
#define MH_T_POSTAL_LOCALE		( (OM_type) 318 )
#define MH_T_POSTAL_MODE		( (OM_type) 319 )
#define MH_T_POSTAL_OFFICE_BOX_NUMBER	( (OM_type) 320 )
#define MH_T_POSTAL_OFFICE_NAME		( (OM_type) 321 )
#define MH_T_POSTAL_OFFICE_NUMBER	( (OM_type) 322 )
#define MH_T_POSTAL_ORGANIZATION_NAME	( (OM_type) 323 )
#define MH_T_POSTAL_PATRON_DETAILS	( (OM_type) 324 )
#define MH_T_POSTAL_PATRON_NAME		( (OM_type) 325 )
#define MH_T_POSTAL_REPORT		( (OM_type) 326 )
#define MH_T_POSTAL_STREET_ADDRESS	( (OM_type) 327 )
#define MH_T_PREFERRED_DELIVERY_MODES	( (OM_type) 328 )
#define MH_T_PRESENTATION_ADDRESS	( (OM_type) 329 )
#define MH_T_PRIORITY			( (OM_type) 330 )
#define MH_T_PRIVACY_MARK		( (OM_type) 331 )
#define MH_T_PRIVATE_USE		( (OM_type) 332 )
#define MH_T_PRMD_IDENTIFIER		( (OM_type) 333 )
#define MH_T_PRMD_NAME			( (OM_type) 334 )
#define MH_T_PROOF_OF_DELIVERY		( (OM_type) 335 )
#define MH_T_PROOF_OF_DELIV_REQUESTED	( (OM_type) 336 )
#define MH_T_PROOF_OF_SUBMISSION	( (OM_type) 337 )
#define MH_T_PROOF_OF_SUBMISN_REQUEST	( (OM_type) 338 )
#define MH_T_PUBLIC_INFORMATION		( (OM_type) 339 )
#define MH_T_RANDOM_NUMBER		( (OM_type) 340 )
#define MH_T_REASON			( (OM_type) 341 )
#define MH_T_REASSIGNMENT_PROHIBITED	( (OM_type) 342 )
#define MH_T_RECIPIENT_CERTIFICATE	( (OM_type) 343 )
#define MH_T_RECIPIENT_DESCRIPTORS	( (OM_type) 344 )
#define MH_T_RECIPIENT_NAME		( (OM_type) 345 )
#define MH_T_RECIPIENT_NUMBER		( (OM_type) 346 )
#define MH_T_RECIP_NUMBER_FOR_ADVICE	( (OM_type) 347 )
#define MH_T_REDIRECTION_HISTORY	( (OM_type) 348 )
#define MH_T_REGISTRATION		( (OM_type) 349 )
#define MH_T_RENDITION_ATTRIBUTES	( (OM_type) 350 )
#define MH_T_REPORT_ADDITIONAL_INFO	( (OM_type) 351 )
#define MH_T_REPORT_DESTINATION		( (OM_type) 352 )
#define MH_T_REPORTING_DL_NAME		( (OM_type) 353 )
#define MH_T_REPORTING_MTA_CERTIFICATE	( (OM_type) 354 )
#define MH_T_SECRET_INFORMATION		( (OM_type) 355 )
#define MH_T_SECURITY_CATEGORY_DATA	( (OM_type) 356 )
#define MH_T_SECURITY_CATEGORY_IDS	( (OM_type) 357 )
#define MH_T_SECURITY_CLASSIFICATION	( (OM_type) 358 )
#define MH_T_SECURITY_LABEL		( (OM_type) 359 )
#define MH_T_SECURITY_POLICY_ID		( (OM_type) 360 )
#define MH_T_SIGNATURE			( (OM_type) 361 )
#define MH_T_SUBJECT_EXT_TRACE_INFO	( (OM_type) 362 )
#define MH_T_SUBJECT_MTS_IDENTIFIER	( (OM_type) 363 )
#define MH_T_SUBMISSION_TIME		( (OM_type) 364 )
#define MH_T_SUPPLEMENTARY_INFO		( (OM_type) 365 )
#define MH_T_SURNAME			( (OM_type) 366 )
#define MH_T_TELETEX_NBPS		( (OM_type) 367 )
#define MH_T_TEMPORARY			( (OM_type) 368 )
#define MH_T_TERMINAL_IDENTIFIER	( (OM_type) 369 )
#define MH_T_TERMINAL_TYPE		( (OM_type) 370 )
#define MH_T_TIME			( (OM_type) 371 )
#define MH_T_TOKEN			( (OM_type) 372 )
#define MH_T_TWO_DIMENSIONAL		( (OM_type) 373 )
#define MH_T_UNCOMPRESSED		( (OM_type) 374 )
#define MH_T_UNLIMITED_LENGTH		( (OM_type) 375 )
#define MH_T_WORKSPACE			( (OM_type) 376 )
#define MH_T_X121_ADDRESS		( (OM_type) 377 )

/* Value Length */

#define MH_VL_ADMD_NAME			( (OM_value_length) 16 )
#define MH_VL_ATTEMPTED_ADMD_NAME	( (OM_value_length) 16 )
#define MH_VL_ATTEMPTED_COUNTRY_NAME	( (OM_value_length) 3 )
#define MH_VL_ATTEMPTED_PRMD_IDENTIFIER	( (OM_value_length) 16 )
#define MH_VL_COMMON_NAME		( (OM_value_length) 64 )
#define MH_VL_CONTENT_CORRELATOR	( (OM_value_length) 512 )
#define MH_VL_CONTENT_IDENTIFIER	( (OM_value_length) 16 )
#define MH_VL_COUNTRY_NAME		( (OM_value_length) 3 )
#define MH_VL_DOMAIN_TYPE		( (OM_value_length) 8 )
#define MH_VL_DOMAIN_VALUE		( (OM_value_length) 128 )
#define MH_VL_GENERATION		( (OM_value_length) 3 )
#define MH_VL_GIVEN_NAME		( (OM_value_length) 16 )
#define MH_VL_INFORMATION		( (OM_value_length) 1024 )
#define MH_VL_INITIALS			( (OM_value_length) 5 )
#define MH_VL_ISDN_NUMBER		( (OM_value_length) 15 )
#define MH_VL_ISDN_SUBADDRESS		( (OM_value_length) 40 )
#define MH_VL_LATEST_DELIVERY_TIME	( (OM_value_length) 7 )
#define MH_VL_LOCAL_IDENTIFIER		( (OM_value_length) 32 )
#define MH_VL_MSG_CONTENT_CORRELATOR	( (OM_value_length) 16 )
#define MH_VL_MTA_NAME			( (OM_value_length) 32 )
#define MH_VL_NUMERIC_USER_IDENTIFIER	( (OM_value_length) 32 )
#define MH_VL_ORGANIZATION_NAME		( (OM_value_length) 64 )
#define MH_VL_ORGANIZATIONAL_UNIT_NAMES	( (OM_value_length) 32 )
#define MH_VL_POSTAL_ADDRESS_DETAILS	( (OM_value_length) 30 )
#define MH_VL_POSTAL_ADDRESS_IN_FULL	( (OM_value_length) 185 )
#define MH_VL_POSTAL_ADDRESS_IN_LINES	( (OM_value_length) 30 )
#define MH_VL_POSTAL_CODE		( (OM_value_length) 16 )
#define MH_VL_POSTAL_COUNTRY_NAME	( (OM_value_length) 32 )
#define MH_VL_POSTAL_DELIV_POINT_NAME	( (OM_value_length) 30 )
#define MH_VL_POSTAL_DELIV_SYSTEM_NAME	( (OM_value_length) 16 )
#define MH_VL_POSTAL_GENERAL_DELIV_ADDR	( (OM_value_length) 30 )
#define MH_VL_POSTAL_LOCALE		( (OM_value_length) 30 )
#define MH_VL_POSTAL_OFFICE_BOX_NUMBER	( (OM_value_length) 30 )
#define MH_VL_POSTAL_OFFICE_NAME	( (OM_value_length) 30 )
#define MH_VL_POSTAL_OFFICE_NUMBER	( (OM_value_length) 30 )
#define MH_VL_POSTAL_ORGANIZATION_NAME	( (OM_value_length) 30 )
#define MH_VL_POSTAL_PATRON_DETAILS	( (OM_value_length) 30 )
#define MH_VL_POSTAL_PATRON_NAME	( (OM_value_length) 30 )
#define MH_VL_POSTAL_STREET_ADDRESS	( (OM_value_length) 30 )
#define MH_VL_PRIVACY_MARK		( (OM_value_length) 128 )
#define MH_VL_PRIVATE_USE		( (OM_value_length) 126 )
#define MH_VL_PRMD_IDENTIFIER		( (OM_value_length) 16 )
#define MH_VL_PRMD_NAME			( (OM_value_length) 16 )
#define MH_VL_RECIP_NUMBER_FOR_ADVICE	( (OM_value_length) 32 )
#define MH_VL_REDIRECTION_TIME		( (OM_value_length) 7 )
#define MH_VL_REPORT_ADDITIONAL_INFO	( (OM_value_length) 1024 )
#define MH_VL_SUPPLEMENTARY_INFO	( (OM_value_length) 64 )
#define MH_VL_SURNAME			( (OM_value_length) 40 )
#define MH_VL_TERMINAL_IDENTIFIER	( (OM_value_length) 24 )
#define MH_VL_TIME			( (OM_value_length) 17 )
#define MH_VL_X121_ADDRESS		( (OM_value_length) 15 )

/* Value Number */

#define MH_VN_BILATERAL_INFORMATION	( (OM_value_number) 8 )
#define MH_VN_ENCODED_INFORMATION_TYPES	( (OM_value_number) 8 )
#define MH_VN_EXPANSION_HISTORY		( (OM_value_number) 512 )
#define MH_VN_OTHER_RECIPIENT_NAMES	( (OM_value_number) 32767 )
#define MH_VN_PREFERRED_DELIVERY_MODES	( (OM_value_number) 10 )
#define MH_VN_RECIPIENT_DESCRIPTORS	( (OM_value_number) 32767 )
#define MH_VN_REDIRECTION_HISTORY	( (OM_value_number) 512 )
#define MH_VN_REPORT_SUBSTANCE		( (OM_value_number) 32767 )
#define MH_VN_SECURITY_CATEGORY_DATA	( (OM_value_number) 64 )
#define MH_VN_SECURITY_CATEGORY_IDS	( (OM_value_number) 64 )
#define MH_VN_TRACE_INFO		( (OM_value_number) 512 )

/* END MH PORTION OF INTERFACE */

#endif /* _XMHP_H */
