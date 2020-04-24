/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: xmsga.h,v $
 * Revision 1.1.10.2  1996/02/18  23:38:11  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:31:20  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:37:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:38  root]
 * 
 * Revision 1.1.8.1  1994/02/22  19:00:13  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:06:34  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  07:12:58  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:22:15  marrek]
 * 
 * Revision 1.1.4.2  1992/12/30  19:31:35  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  18:39:48  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  19:46:15  zeliff
 * 	Commenting out non-ANSI #ident
 * 	[1992/05/28  15:54:17  zeliff]
 * 
 * 	New files from GDS drop
 * 	[1992/05/27  19:45:07  zeliff]
 * 
 * $EndLog$
 */
#ifndef _XMSGA_H
#define _XMSGA_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xmsga_rcsid[] = "@(#)$RCSfile: xmsga.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:38:11 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/

/* MS General Attributes Package object identifier */

#define OMP_O_MS_GENERAL_ATTRIBUTES_PACKAGE     "\x56\x06\x01\x02\x06\x02"


/* MS General Attributes Types */
/*
 * Note: Every client program must explicitly import into 
 * every compilation unit (C source program) the classes or
 * Object Identifiers that it uses. Each of these classes or
 * Object Identifier names must then be explicitly exported from
 * just one compilation unit.
 * Importing and exporting can be done using the OM_IMPORT and
 * OM_EXPORT macros respectively (see [OM API]).
 * For instance, the client program uses
 *                OM_IMPORT( MS_A_CHILD_SEQUENCE_NUMBERS )
 * which in turn will make use of
 *                OMP_O_MS_A_CHILD_SEQUENCE_NUMBERS 
 * defined below.
 */

#define OMP_O_MS_A_CHILD_SEQUENCE_NUMBERS               "\x56\x04\x03\x00"
#define OMP_O_MS_A_CONTENT                              "\x56\x04\x03\x01"
#define OMP_O_MS_A_CONTENT_CONFIDENTL_ALGM_ID           "\x56\x04\x03\x02"
#define OMP_O_MS_A_CONTENT_CORRELATOR                   "\x56\x04\x03\x03"
#define OMP_O_MS_A_CONTENT_IDENTIFIER                   "\x56\x04\x03\x04"
#define OMP_O_MS_A_CONTENT_INTEGRITY_CHECK              "\x56\x04\x03\x05"
#define OMP_O_MS_A_CONTENT_LENGTH                       "\x56\x04\x03\x06"
#define OMP_O_MS_A_CONTENT_RETURNED                     "\x56\x04\x03\x07"
#define OMP_O_MS_A_CONTENT_TYPE                         "\x56\x04\x03\x08"
#define OMP_O_MS_A_CONVERSION_LOSS_PROHIBITED           "\x56\x04\x03\x09"
#define OMP_O_MS_A_CONVERTED_EITS                       "\x56\x04\x03\x0A"
#define OMP_O_MS_A_CREATION_TIME                        "\x56\x04\x03\x0B"
#define OMP_O_MS_A_DELIVERED_EITS                       "\x56\x04\x03\x0C"
#define OMP_O_MS_A_DELIVERY_FLAGS                       "\x56\x04\x03\x0D"
#define OMP_O_MS_A_DL_EXPANSION_HISTORY                 "\x56\x04\x03\x0E"
#define OMP_O_MS_A_ENTRY_STATUS                         "\x56\x04\x03\x0F"
#define OMP_O_MS_A_ENTRY_TYPE                           "\x56\x04\x03\x10"
#define OMP_O_MS_A_INTENDED_RECIPIENT_NAME              "\x56\x04\x03\x11"
#define OMP_O_MS_A_MESSAGE_DELIVERY_ENVELOPE            "\x56\x04\x03\x12"
#define OMP_O_MS_A_MESSAGE_DELIVERY_ID                  "\x56\x04\x03\x13"
#define OMP_O_MS_A_MESSAGE_DELIVERY_TIME                "\x56\x04\x03\x14"
#define OMP_O_MS_A_MESSAGE_ORIGIN_AUTHEN_CHK            "\x56\x04\x03\x15"
#define OMP_O_MS_A_MESSAGE_SECURITY_LABEL               "\x56\x04\x03\x16"
#define OMP_O_MS_A_MESSAGE_SUBMISSION_TIME              "\x56\x04\x03\x17"
#define OMP_O_MS_A_MESSAGE_TOKEN                        "\x56\x04\x03\x18"
#define OMP_O_MS_A_ORIGINAL_EITS                        "\x56\x04\x03\x19"
#define OMP_O_MS_A_ORIGINATOR_CERTIFICATE               "\x56\x04\x03\x1A"
#define OMP_O_MS_A_ORIGINATOR_NAME                      "\x56\x04\x03\x1B"
#define OMP_O_MS_A_OTHER_RECIPIENT_NAMES                "\x56\x04\x03\x1C"
#define OMP_O_MS_A_PARENT_SEQUENCE_NUMBER               "\x56\x04\x03\x1D"
#define OMP_O_MS_A_PERRECIP_REPORT_DELIV_FLDS           "\x56\x04\x03\x1E"
#define OMP_O_MS_A_PRIORITY                             "\x56\x04\x03\x1F"
#define OMP_O_MS_A_PROOF_OF_DELIVERY_REQUEST            "\x56\x04\x03\x20"
#define OMP_O_MS_A_REDIRECTION_HISTORY                  "\x56\x04\x03\x21"
#define OMP_O_MS_A_REPORT_DELIVERY_ENVELOPE             "\x56\x04\x03\x22"
#define OMP_O_MS_A_REPORT_ORIGIN_AUTHEN_CHK             "\x56\x04\x03\x23"
#define OMP_O_MS_A_REPORTING_DL_NAME                    "\x56\x04\x03\x24"
#define OMP_O_MS_A_REPORTING_MTA_CERTIFICATE            "\x56\x04\x03\x25"
#define OMP_O_MS_A_SECURITY_CLASSIFICATION              "\x56\x04\x03\x26"
#define OMP_O_MS_A_SEQUENCE_NUMBER                      "\x56\x04\x03\x27"
#define OMP_O_MS_A_SUBJECT_SUBMISSION_ID                "\x56\x04\x03\x28"
#define OMP_O_MS_A_THIS_RECIPIENT_NAME                  "\x56\x04\x03\x29"


/* Enumeration Constants */

/* for MS_A_ENTRY_STATUS */

#define MS_ES_NEW                       ((OM_enumeration) 0)
#define MS_ES_LISTED                    ((OM_enumeration) 1)
#define MS_ES_PROCESSED                 ((OM_enumeration) 2)


/* for MS_A_ENTRY_TYPE */

#define MS_ET_DELIVERED_MESSAGE         ((OM_enumeration) 0)
#define MS_ET_DELIVERED_REPORT          ((OM_enumeration) 1)
#define MS_ET_RETURNED_CONTENT          ((OM_enumeration) 2)


/* for MS_A_PRIORITY */

#define MS_PTY_NORMAL                   ((OM_enumeration) 0)
#define MS_PTY_LOW                      ((OM_enumeration) 1)
#define MS_PTY_URGENT                   ((OM_enumeration) 2)


/* for MS_A_SECURITY_CLASSIFICATION */

#define MS_SC_UNMARKED                  ((OM_enumeration) 0)
#define MS_SC_UNCLASSIFIED              ((OM_enumeration) 1)
#define MS_SC_RESTRICTED                ((OM_enumeration) 2)
#define MS_SC_CONFIDENTIAL              ((OM_enumeration) 3)
#define MS_SC_SECRET                    ((OM_enumeration) 4)
#define MS_SC_TOP_SECRET                ((OM_enumeration) 5)

#endif /* _XMSGA_H */
