/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: d27char.h,v $
 * Revision 1.1.4.2  1996/02/18  23:35:49  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:00  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:00:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:13:52  root]
 * 
 * Revision 1.1.2.4  1994/07/15  13:29:28  marrek
 * 	Remove empty comment line.
 * 	[1994/07/15  10:02:43  marrek]
 * 
 * Revision 1.1.2.3  1994/07/07  07:02:40  marrek
 * 	Add version string in July 1994 code drop.
 * 	[1994/07/07  06:58:48  marrek]
 * 
 * Revision 1.1.2.2  1994/06/09  18:46:16  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:19:19  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:14:51  marrek
 * 	Bug fixes.
 * 	[1994/04/29  09:09:45  marrek]
 * 
 * 	Created for April Submission.
 * 	[1994/04/26  12:49:56  marrek]
 * 
 * $EndLog$
 */

/*module***********************************************************************
 *
 * PRODUCT NAME / SUBSYSTEM NAME:
 *    OCIS / ORGMAN
 * MODULE NAME / VERSION:
 *    omChar.h (%M%) / %I%
 * DATE / LAST MODIFICATION:
 *    %D% / %E%
 * DESCRIPTION:
 *    Include file with definitions of parameter values and functions
 *    for OmChar function calls.
 * AUTHOR:
 *    Fiedrich, AP 154
 *    vers.|   date   |                changes                  | KZ | ER#/CR#
 *    -------------------------------------------------------------------------
 *    1.00 | 05.08.92 | initial state                           | Fd | 
 *    1.01 | 28.09.92 | new definition OmCHAR_ASCII_NOT_COLON   | Fd | A0158768
 *    1.02 | 13.10.92 | new character set OmCHAR_ASCII_NOT_COSE | Fd | internal
 *    1.03 | 28.04.93 | new charcter sets ('\t', ',', '-')      | Fd | internal
 *    1.06 | 03.05.93 | charcter '-' allowed                    | Fd | internal
 *
 *headerend*******************************************************************/

#ifndef _OMCHAR_H
#define _OMCHAR_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char d27char_rcsid[] = "@(#)$RCSfile: d27char.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:35:49 $";
#endif

/********************/
/* parameter values */
/********************/

#ifndef TRUE                    /* logic TRUE */
#define TRUE 1
#endif

#ifndef FALSE                   /* logic FALSE */
#define FALSE 0
#endif

typedef enum                   /* character sets                      */
{  OmCHAR_DIGIT,               /* digits                              */
   OmCHAR_DIGIT_BLANK,         /* digits and blank                    */
   OmCHAR_DIGIT_LOWER_ALPHA,   /* digits and lower case letters       */
   OmCHAR_ALPHA,               /* lower and upper case letters        */
   OmCHAR_PRINTABLE,           /* Printable String                    */
   OmCHAR_ASCII,               /* ASCII                               */
   OmCHAR_ASCII_NOT_COLON,     /* ASCII without ':'                   */
   OmCHAR_ASCII_NOT_COSE,      /* ASCII without ':' and ';'           */
   OmCHAR_ASCII_V3,            /* ASCII without ':', ';' and ','      */
   OmCHAR_ISO,                 /* ISO 8859 + '\t' without '\t'        */
   OmCHAR_ISO_NOT_SLASH,       /* ISO 8859 + '\t'                     */
			       /* without '\t' and '/'                */
   OmCHAR_ISO_V2,              /* ISO 8859 + '\t'                     */
   OmCHAR_T61,                 /* T.61                                */
   OmCHAR_T61_NOT_SEMICOLON,   /* T.61 without ';' and '!'            */
   OmCHAR_ANY                  /* all codes allowed                   */
} OmCharCharSet;

/*************/
/* functions */
/*************/

void OmCharInit(void);
int  OmCharRemoveBlanks(int, const char *, char *);
int  OmCharValidCodes(OmCharCharSet, int, const char *,
		      int, char *, int *, int);
int  OmCharT61Length(const char *, int);
int  OmCharAdaptPrs(const char *, char *);
int  OmCharIsoT61(const char *, int, char *, int *);
int  OmCharT61Iso(const char *, int, char *, int *);

#endif
