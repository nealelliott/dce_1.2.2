/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: d27charvar.c,v $
 * Revision 1.1.4.2  1996/02/18  18:23:37  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:46:36  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:16:05  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:21:17  root]
 * 
 * Revision 1.1.2.4  1994/07/15  13:29:32  marrek
 * 	Remove empty comment line.
 * 	[1994/07/15  13:00:19  marrek]
 * 
 * Revision 1.1.2.3  1994/07/07  07:02:45  marrek
 * 	Add version string in July 1994 code drop.
 * 	[1994/07/07  06:59:02  marrek]
 * 
 * Revision 1.1.2.2  1994/06/10  21:18:47  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:55:15  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:16:05  marrek
 * 	Bug fixes.
 * 	[1994/04/29  09:10:02  marrek]
 * 
 * 	Created for April Submission.
 * 	[1994/04/26  12:52:53  marrek]
 * 
 * $EndLog$
 */
#if !defined lint && defined GDS_RCS_ID
static char d27charvar_rcsid[] = "@(#)$RCSfile: d27charvar.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:23:37 $";
#endif

/*module***********************************************************************
 *
 * PRODUCT NAME / SUBSYSTEM NAME:
 *    OCIS / ORGMAN
 * MODULE NAME / VERSION:
 *    omCharVar.c (%M%) / %I%
 * DATE / LAST MODIFICATION:
 *    %D% / %E%
 * DESCRIPTION:
 *    Global variables for OmChar functions.
 * OFFERED FUNCTIONS:
 *    -
 * OFFERED DATA:
 *    omCharConvSwitch
 *    omCharVal
 *    omCharExc
 *    omCharConv1
 *    omCharConv2
 *    omCharInd
 *    omCharConv3
 *    omCharPrs
 *    omCharErrC
 * AUTHOR:
 *    Fiedrich, AP 154
 *    vers.|   date   |                changes                  | KZ | ER#/CR#
 *    -------------------------------------------------------------------------
 *    1.00 | 10.09.92 | initial state                           | Fd |
 *    1.01 | 30.10.92 | array omCharEqual introduced            | Fd | internal
 *    1.02 | 20.11.92 | underline conversion changed            | Fd | internal
 *    1.03 | 05.04.93 | ISO8859/T.61 conversion deactivation    | Fd | internal
 *    1.07 | 28.04.93 | '\t' included in ISO 8859 character set | Fd | internal
 *
 *headerend*******************************************************************/

/*****************/
/* include files */
/*****************/

#include <d27chardef.h>

/***************************************************************************/
/* conversion switch                                                       */
/*                                                                         */
/* This variable determines whether conversion functions omCharIsoT61      */
/* and omCharT61Iso are to convert the passed string between ISO 8859 and  */
/* T.61 or leave it unchanged.                                             */
/***************************************************************************/

int omCharConvSwitch = FALSE;   /* default value: no conversion */

/***************************************************************************/
/* valid codes table                                                       */
/*                                                                         */
/* The index to this table is the ISO 8859 code of the character to be     */
/* tested. Only the lower half of this table is initialized, the upper     */
/* half is set at runtime according to table omCharConv1.                  */
/* Each byte contains the validity information of the respective character */
/* for 8 character sets. One bit each is allocated to a character set.     */
/* 0 means invalid, 1 means valid character. The character set masks       */
/* CHS1 ... CHS8 are defined in omCharDef.h.                               */
/***************************************************************************/

unsigned char omCharVal[256] =
{  /* code        validity information for character set CHS1 ... CHS8 */

   /* 0x00 */    0,
   /* 0x01 */    0,
   /* 0x02 */    0,
   /* 0x03 */    0,
   /* 0x04 */    0,
   /* 0x05 */    0,
   /* 0x06 */    0,
   /* 0x07 */    0,
   /* 0x08 */    0,
   /* 0x09 */    0 +    0 +    0 +    0 +    0 +    0 + CHS7 +    0,
   /* 0x0a */    0,
   /* 0x0b */    0,
   /* 0x0c */    0,
   /* 0x0d */    0,
   /* 0x0e */    0,
   /* 0x0f */    0,

   /* 0x10 */    0,
   /* 0x11 */    0,
   /* 0x12 */    0,
   /* 0x13 */    0,
   /* 0x14 */    0,
   /* 0x15 */    0,
   /* 0x16 */    0,
   /* 0x17 */    0,
   /* 0x18 */    0,
   /* 0x19 */    0,
   /* 0x1a */    0,
   /* 0x1b */    0,
   /* 0x1c */    0,
   /* 0x1d */    0,
   /* 0x1e */    0,
   /* 0x1f */    0,

   /* 0x20 */    0 + CHS2 +    0 +    0 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x21 */    0 +    0 +    0 +    0 +    0 + CHS6 + CHS7 + CHS8,
   /* 0x22 */    0 +    0 +    0 +    0 +    0 + CHS6 + CHS7 + CHS8,
   /* 0x23 */    0 +    0 +    0 +    0 +    0 + CHS6 + CHS7 + CHS8,
   /* 0x24 */    0 +    0 +    0 +    0 +    0 + CHS6 + CHS7 + CHS8,
   /* 0x25 */    0 +    0 +    0 +    0 +    0 + CHS6 + CHS7 + CHS8,
   /* 0x26 */    0 +    0 +    0 +    0 +    0 + CHS6 + CHS7 + CHS8,
   /* 0x27 */    0 +    0 +    0 +    0 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x28 */    0 +    0 +    0 +    0 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x29 */    0 +    0 +    0 +    0 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x2a */    0 +    0 +    0 +    0 +    0 + CHS6 + CHS7 + CHS8,
   /* 0x2b */    0 +    0 +    0 +    0 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x2c */    0 +    0 +    0 +    0 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x2d */    0 +    0 +    0 +    0 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x2e */    0 +    0 +    0 +    0 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x2f */    0 +    0 +    0 +    0 + CHS5 + CHS6 + CHS7 + CHS8,

   /* 0x30 */ CHS1 + CHS2 + CHS3 +    0 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x31 */ CHS1 + CHS2 + CHS3 +    0 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x32 */ CHS1 + CHS2 + CHS3 +    0 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x33 */ CHS1 + CHS2 + CHS3 +    0 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x34 */ CHS1 + CHS2 + CHS3 +    0 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x35 */ CHS1 + CHS2 + CHS3 +    0 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x36 */ CHS1 + CHS2 + CHS3 +    0 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x37 */ CHS1 + CHS2 + CHS3 +    0 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x38 */ CHS1 + CHS2 + CHS3 +    0 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x39 */ CHS1 + CHS2 + CHS3 +    0 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x3a */    0 +    0 +    0 +    0 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x3b */    0 +    0 +    0 +    0 +    0 + CHS6 + CHS7 + CHS8,
   /* 0x3c */    0 +    0 +    0 +    0 +    0 + CHS6 + CHS7 + CHS8,
   /* 0x3d */    0 +    0 +    0 +    0 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x3e */    0 +    0 +    0 +    0 +    0 + CHS6 + CHS7 + CHS8,
   /* 0x3f */    0 +    0 +    0 +    0 + CHS5 + CHS6 + CHS7 + CHS8,

   /* 0x40 */    0 +    0 +    0 +    0 +    0 + CHS6 + CHS7 + CHS8,
   /* 0x41 */    0 +    0 +    0 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x42 */    0 +    0 +    0 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x43 */    0 +    0 +    0 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x44 */    0 +    0 +    0 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x45 */    0 +    0 +    0 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x46 */    0 +    0 +    0 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x47 */    0 +    0 +    0 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x48 */    0 +    0 +    0 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x49 */    0 +    0 +    0 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x4a */    0 +    0 +    0 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x4b */    0 +    0 +    0 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x4c */    0 +    0 +    0 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x4d */    0 +    0 +    0 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x4e */    0 +    0 +    0 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x4f */    0 +    0 +    0 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,

   /* 0x50 */    0 +    0 +    0 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x51 */    0 +    0 +    0 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x52 */    0 +    0 +    0 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x53 */    0 +    0 +    0 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x54 */    0 +    0 +    0 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x55 */    0 +    0 +    0 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x56 */    0 +    0 +    0 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x57 */    0 +    0 +    0 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x58 */    0 +    0 +    0 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x59 */    0 +    0 +    0 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x5a */    0 +    0 +    0 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x5b */    0 +    0 +    0 +    0 +    0 + CHS6 + CHS7 + CHS8,
   /* 0x5c */    0 +    0 +    0 +    0 +    0 + CHS6 + CHS7 +    0,
   /* 0x5d */    0 +    0 +    0 +    0 +    0 + CHS6 + CHS7 + CHS8,
   /* 0x5e */    0 +    0 +    0 +    0 +    0 + CHS6 + CHS7 + CHS8,
   /* 0x5f */    0 +    0 +    0 +    0 +    0 + CHS6 + CHS7 + CHS8,

   /* 0x60 */    0 +    0 +    0 +    0 +    0 + CHS6 + CHS7 + CHS8,
   /* 0x61 */    0 +    0 + CHS3 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x62 */    0 +    0 + CHS3 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x63 */    0 +    0 + CHS3 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x64 */    0 +    0 + CHS3 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x65 */    0 +    0 + CHS3 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x66 */    0 +    0 + CHS3 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x67 */    0 +    0 + CHS3 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x68 */    0 +    0 + CHS3 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x69 */    0 +    0 + CHS3 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x6a */    0 +    0 + CHS3 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x6b */    0 +    0 + CHS3 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x6c */    0 +    0 + CHS3 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x6d */    0 +    0 + CHS3 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x6e */    0 +    0 + CHS3 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x6f */    0 +    0 + CHS3 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,

   /* 0x70 */    0 +    0 + CHS3 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x71 */    0 +    0 + CHS3 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x72 */    0 +    0 + CHS3 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x73 */    0 +    0 + CHS3 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x74 */    0 +    0 + CHS3 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x75 */    0 +    0 + CHS3 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x76 */    0 +    0 + CHS3 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x77 */    0 +    0 + CHS3 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x78 */    0 +    0 + CHS3 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x79 */    0 +    0 + CHS3 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x7a */    0 +    0 + CHS3 + CHS4 + CHS5 + CHS6 + CHS7 + CHS8,
   /* 0x7b */    0 +    0 +    0 +    0 +    0 + CHS6 + CHS7 +    0,
   /* 0x7c */    0 +    0 +    0 +    0 +    0 + CHS6 + CHS7 + CHS8,
   /* 0x7d */    0 +    0 +    0 +    0 +    0 + CHS6 + CHS7 +    0,
   /* 0x7e */    0 +    0 +    0 +    0 +    0 + CHS6 + CHS7 + CHS8,
   /* 0x7f */    0 +    0 +    0 +    0 +    0 +    0 +    0 +    0
};

/*****************************************************************************/
/* transformation exception table                                            */
/*                                                                           */
/* Normally, when the ISO 8859 code of a character is in the lower half      */
/* of the code value area, the respective T.61 code has the same value.      */
/* However, there are some exceptions from this rule. The following table    */
/* contains the transformations for these exceptions.                        */
/*****************************************************************************/

unsigned char omCharExc[][3] =
{    /* character   ISO 8859   T.61/1st   T.61/2nd */

   { /* '#' */      0x23,      0xa6,      0         },
   { /* '$' */      0x24,      0xa4,      0         },
   { /* '^' */      0x5e,      0xc3,      0x20      },
   { /* '`' */      0x60,      0xc1,      0x20      },
   { /* '~' */      0x7e,      0xc4,      0x20      },
   { /* end */      0,         0,         0         }
};

/*****************************************************************************/
/* conversion table 1 (ISO 8859 to T.61)                                     */
/*                                                                           */
/* This table contains the information for the conversion of ISO 8859 codes  */
/* in the range from 0xa0 up. The index to this table is the ISO 8859 code   */
/* of the character, minus 0xa0.                                             */
/* In each array element the first character has the following meaning:      */
/* 0 = code not defined in ISO 8859; 1 = code defined in ISO 8859 but not in */
/* T.61; >1 = code defined in T.61, value is first byte of the T.61          */
/* representation.                                                           */
/* The second character has the following meaning: 0 = no second byte in     */
/* T.61 representation, >0 = value is second byte in T.61 representation.    */
/* At runtime the program completes array omCharVal with information taken   */
/* from conversion table 1. Hereafter, the 1s in the first characters        */
/* (indication that the code is defined in ISO 8859 but not in T.61) is no   */
/* longer needed and is, therefore, replaced by value 0.                     */
/*****************************************************************************/

unsigned char omCharConv1[96][2] =
{    /* ISO code, 1st , 2nd  code of T.61 representation */

   { /* 0xa0 */    0,    0 },
   { /* 0xa1 */ 0xa1,    0 },
   { /* 0xa2 */ 0xa2,    0 },
   { /* 0xa3 */ 0xa3,    0 },
   { /* 0xa4 */ 0xa8,    0 },
   { /* 0xa5 */ 0xa5,    0 },
   { /* 0xa6 */    1,    0 },
   { /* 0xa7 */ 0xa7,    0 },
   { /* 0xa8 */ 0xc8, 0x20 },
   { /* 0xa9 */    1,    0 },
   { /* 0xaa */ 0xe3,    0 },
   { /* 0xab */ 0xab,    0 },
   { /* 0xac */    1,    0 },
   { /* 0xad */    1,    0 },
   { /* 0xae */    1,    0 },
   { /* 0xaf */ 0xc5, 0x20 },

   { /* 0xb0 */ 0xb0,    0 },
   { /* 0xb1 */ 0xb1,    0 },
   { /* 0xb2 */ 0xb2,    0 },
   { /* 0xb3 */ 0xb3,    0 },
   { /* 0xb4 */ 0xc2, 0x20 },
   { /* 0xb5 */ 0xb5,    0 },
   { /* 0xb6 */ 0xb6,    0 },
   { /* 0xb7 */ 0xb7,    0 },
   { /* 0xb8 */ 0xcb, 0x20 },
   { /* 0xb9 */    1,    0 },
   { /* 0xba */ 0xeb,    0 },
   { /* 0xbb */ 0xbb,    0 },
   { /* 0xbc */ 0xbc,    0 },
   { /* 0xbd */ 0xbd,    0 },
   { /* 0xbe */ 0xbe,    0 },
   { /* 0xbf */ 0xbf,    0 },

   { /* 0xc0 */ 0xc1, 0x41 },
   { /* 0xc1 */ 0xc2, 0x41 },
   { /* 0xc2 */ 0xc3, 0x41 },
   { /* 0xc3 */ 0xc4, 0x41 },
   { /* 0xc4 */ 0xc8, 0x41 },
   { /* 0xc5 */ 0xca, 0x41 },
   { /* 0xc6 */ 0xe1,    0 },
   { /* 0xc7 */ 0xcb, 0x43 },
   { /* 0xc8 */ 0xc1, 0x45 },
   { /* 0xc9 */ 0xc2, 0x45 },
   { /* 0xca */ 0xc3, 0x45 },
   { /* 0xcb */ 0xc8, 0x45 },
   { /* 0xcc */ 0xc1, 0x49 },
   { /* 0xcd */ 0xc2, 0x49 },
   { /* 0xce */ 0xc3, 0x49 },
   { /* 0xcf */ 0xc8, 0x49 },

   { /* 0xd0 */ 0xe2,    0 },
   { /* 0xd1 */ 0xc4, 0x4e },
   { /* 0xd2 */ 0xc1, 0x4f },
   { /* 0xd3 */ 0xc2, 0x4f },
   { /* 0xd4 */ 0xc3, 0x4f },
   { /* 0xd5 */ 0xc4, 0x4f },
   { /* 0xd6 */ 0xc8, 0x4f },
   { /* 0xd7 */ 0xb4,    0 },
   { /* 0xd8 */ 0xe9,    0 },
   { /* 0xd9 */ 0xc1, 0x55 },
   { /* 0xda */ 0xc2, 0x55 },
   { /* 0xdb */ 0xc3, 0x55 },
   { /* 0xdc */ 0xc8, 0x55 },
   { /* 0xdd */ 0xc2, 0x59 },
   { /* 0xde */ 0xec,    0 },
   { /* 0xdf */ 0xfb,    0 },

   { /* 0xe0 */ 0xc1, 0x61 },
   { /* 0xe1 */ 0xc2, 0x61 },
   { /* 0xe2 */ 0xc3, 0x61 },
   { /* 0xe3 */ 0xc4, 0x61 },
   { /* 0xe4 */ 0xc8, 0x61 },
   { /* 0xe5 */ 0xca, 0x61 },
   { /* 0xe6 */ 0xf1,    0 },
   { /* 0xe7 */ 0xcb, 0x63 },
   { /* 0xe8 */ 0xc1, 0x65 },
   { /* 0xe9 */ 0xc2, 0x65 },
   { /* 0xea */ 0xc3, 0x65 },
   { /* 0xeb */ 0xc8, 0x65 },
   { /* 0xec */ 0xc1, 0x69 },
   { /* 0xed */ 0xc2, 0x69 },
   { /* 0xee */ 0xc3, 0x69 },
   { /* 0xef */ 0xc8, 0x69 },

   { /* 0xf0 */ 0xf3,    0 },
   { /* 0xf1 */ 0xc4, 0x6e },
   { /* 0xf2 */ 0xc1, 0x6f },
   { /* 0xf3 */ 0xc2, 0x6f },
   { /* 0xf4 */ 0xc3, 0x6f },
   { /* 0xf5 */ 0xc4, 0x6f },
   { /* 0xf6 */ 0xc8, 0x6f },
   { /* 0xf7 */ 0xb8,    0 },
   { /* 0xf8 */ 0xf9,    0 },
   { /* 0xf9 */ 0xc1, 0x75 },
   { /* 0xfa */ 0xc2, 0x75 },
   { /* 0xfb */ 0xc3, 0x75 },
   { /* 0xfc */ 0xc8, 0x75 },
   { /* 0xfd */ 0xc2, 0x79 },
   { /* 0xfe */ 0xfc,    0 },
   { /* 0xff */ 0xc8, 0x79 }
};

/****************************************************************************/
/* conversion table 2 (T.61 to ISO 8859, 1 byte T.61 codes)                 */
/*                                                                          */
/* This table contains the information for the conversion of the 1 byte     */
/* T.61 codes in the range from 0xa0 up. The index to this table is the     */
/* first T.61 code of the character, minus 0xa0. The codes 0xc0 ... 0xcf    */
/* belong to 2 byte T.61 representations and have no meaning in this        */
/* table. This table is filled by the program at runtime, using information */
/* from array omCharConv1.                                                  */
/* In each array element the number has the following meaning:              */
/* 0 = code not defined in T.61; >1 = code defined in T.61, value is the    */
/* ISO 8859 representation.                                                 */
/****************************************************************************/

unsigned char omCharConv2[96];

/****************************************************************************/
/* index table (indexes to omCharConv3)                                     */
/*                                                                          */
/* This table contains part of the information for the conversion of the    */
/* 2 bytes T.61 codes (first byte in the range 0xc0 ... 0xcf). The index to */
/* this table is the code of the first byte of the T.61 representation in   */
/* the range 0xc0 ... 0xcf, minus 0xc0. This table is filled by the program */
/* at runtime, using information from array omCharConv1.                    */
/* In each array element there is an index to table omCharConv3. From this  */
/* index up array omCharConv3 contains conversion information for the       */
/* respective 2 byte T.61 codes.                                            */
/****************************************************************************/

int omCharInd[17];

/****************************************************************************/
/* conversion table 3 (T.61 to ISO 8859, 2 byte T.61 codes)                 */
/*                                                                          */
/* This table contains part of the information for the conversion of the    */
/* 2 bytes T.61 codes (first byte in the range 0xc0 ... 0xcf). The index to */
/* this table does not refer to the T.61 characters. Table omCharInd        */
/* gives the starting indexes of ranges belonging to the respective 2 byte  */
/* T.61 codes. Each range ends below the start of the next range.           */
/* In the array elements of each range the values have the following        */
/* meaning: The first value ist the second byte of a T.61 character. The    */
/* second value is the respective ISO 8859 code.                            */
/****************************************************************************/

unsigned char omCharConv3[96][2];

/****************************************************************************/
/* ISO 8859 / T.61 equality table                                           */
/*                                                                          */
/* This principally redundant table was introduced to time optimize the     */
/* conversions betwenn ISO 8859 and T.61. It  is filled by function         */
/* OmCharInit.                                                              */
/* Index to this table is the ISO 8859 or T.61 code of a character. The     */
/* contents have the following meanings:                                    */
/* '0' = respective characters in ISO 8859 and T.61 are unequal             */
/* '1' = respective characters in ISO and T.61 are equal                    */
/*                                                                          */
/****************************************************************************/

char omCharEqual[256];

/*****************************************************************************/
/* substitution table (for non Printable String characters)                  */
/*                                                                           */
/* This table contains the information for the substitution of ISO 8859      */
/* characters in the range from 0xa0 up by a sequence of 1 or 2 Printable    */
/* String characters. The index to this table is the ISO 8859 code of the    */
/* character to be substituted, minus 0xa0.                                  */
/* In each array element the first character is the code of the first        */
/* substitution character. If a second substitution character is defined,    */
/* it is in the second character of the element, otherwise this character    */
/* is 0.                                                                     */
/*****************************************************************************/

unsigned char omCharPrs[96][2] =
{    /* ISO code, 1st , 2nd  code of substitution in Printable String */

   { /* 0xa0 */    0,    0 },
   { /* 0xa1 */    0,    0 },
   { /* 0xa2 */    0,    0 },
   { /* 0xa3 */    0,    0 },
   { /* 0xa4 */    0,    0 },
   { /* 0xa5 */    0,    0 },
   { /* 0xa6 */    0,    0 },
   { /* 0xa7 */    0,    0 },
   { /* 0xa8 */    0,    0 },
   { /* 0xa9 */    0,    0 },
   { /* 0xaa */    0,    0 },
   { /* 0xab */    0,    0 },
   { /* 0xac */    0,    0 },
   { /* 0xad */    0,    0 },
   { /* 0xae */    0,    0 },
   { /* 0xaf */    0,    0 },

   { /* 0xb0 */    0,    0 },
   { /* 0xb1 */    0,    0 },
   { /* 0xb2 */    0,    0 },
   { /* 0xb3 */    0,    0 },
   { /* 0xb4 */    0,    0 },
   { /* 0xb5 */    0,    0 },
   { /* 0xb6 */    0,    0 },
   { /* 0xb7 */    0,    0 },
   { /* 0xb8 */    0,    0 },
   { /* 0xb9 */    0,    0 },
   { /* 0xba */    0,    0 },
   { /* 0xbb */    0,    0 },
   { /* 0xbc */    0,    0 },
   { /* 0xbd */    0,    0 },
   { /* 0xbe */    0,    0 },
   { /* 0xbf */    0,    0 },

   { /* 0xc0 */ 0x41,    0 },
   { /* 0xc1 */ 0x41,    0 },
   { /* 0xc2 */ 0x41,    0 },
   { /* 0xc3 */ 0x41,    0 },
   { /* 0xc4 */ 0x41, 0x65 },
   { /* 0xc5 */ 0x41,    0 },
   { /* 0xc6 */ 0x41, 0x45 },
   { /* 0xc7 */ 0x43,    0 },
   { /* 0xc8 */ 0x45,    0 },
   { /* 0xc9 */ 0x45,    0 },
   { /* 0xca */ 0x45,    0 },
   { /* 0xcb */ 0x45,    0 },
   { /* 0xcc */ 0x49,    0 },
   { /* 0xcd */ 0x49,    0 },
   { /* 0xce */ 0x49,    0 },
   { /* 0xcf */ 0x49,    0 },

   { /* 0xd0 */ 0x44,    0 },
   { /* 0xd1 */ 0x4e,    0 },
   { /* 0xd2 */ 0x4f,    0 },
   { /* 0xd3 */ 0x4f,    0 },
   { /* 0xd4 */ 0x4f,    0 },
   { /* 0xd5 */ 0x4f,    0 },
   { /* 0xd6 */ 0x4f, 0x65 },
   { /* 0xd7 */    0,    0 },
   { /* 0xd8 */ 0x4f,    0 },
   { /* 0xd9 */ 0x55,    0 },
   { /* 0xda */ 0x55,    0 },
   { /* 0xdb */ 0x55,    0 },
   { /* 0xdc */ 0x55, 0x65 },
   { /* 0xdd */ 0x59,    0 },
   { /* 0xde */    0,    0 },
   { /* 0xdf */ 0x73, 0x73 },

   { /* 0xe0 */ 0x61,    0 },
   { /* 0xe1 */ 0x61,    0 },
   { /* 0xe2 */ 0x61,    0 },
   { /* 0xe3 */ 0x61,    0 },
   { /* 0xe4 */ 0x61, 0x65 },
   { /* 0xe5 */ 0x61,    0 },
   { /* 0xe6 */ 0x61, 0x65 },
   { /* 0xe7 */ 0x63,    0 },
   { /* 0xe8 */ 0x65,    0 },
   { /* 0xe9 */ 0x65,    0 },
   { /* 0xea */ 0x65,    0 },
   { /* 0xeb */ 0x65,    0 },
   { /* 0xec */ 0x69,    0 },
   { /* 0xed */ 0x69,    0 },
   { /* 0xee */ 0x69,    0 },
   { /* 0xef */ 0x69,    0 },

   { /* 0xf0 */ 0x64,    0 },
   { /* 0xf1 */ 0x6e,    0 },
   { /* 0xf2 */ 0x6f,    0 },
   { /* 0xf3 */ 0x6f,    0 },
   { /* 0xf4 */ 0x6f,    0 },
   { /* 0xf5 */ 0x6f,    0 },
   { /* 0xf6 */ 0x6f, 0x65 },
   { /* 0xf7 */    0,    0 },
   { /* 0xf8 */ 0x6f,    0 },
   { /* 0xf9 */ 0x75,    0 },
   { /* 0xfa */ 0x75,    0 },
   { /* 0xfb */ 0x75,    0 },
   { /* 0xfc */ 0x75, 0x65 },
   { /* 0xfd */ 0x79,    0 },
   { /* 0xfe */    0,    0 },
   { /* 0xff */ 0x79,    0 }
};

/******************/
/* error messages */
/******************/

char *omCharErrC[] =
{  /* 0 */ "invalid number",
   /* 1 */ "too many numbers",
   /* 2 */ "2nd number not 0xa0...0xbf/0xd0...0xff",
   /* 3 */ "2nd number not 0xc0...0xcf or 3rd not 0x01...0x7f",
   /* 4 */ "2nd number not Printable String",
   /* 5 */ "2nd or 3rd number not Printable String",
   /* 6 */ "1st number not 0xa0...0xff"
};
