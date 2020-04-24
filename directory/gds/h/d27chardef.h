/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: d27chardef.h,v $
 * Revision 1.1.4.2  1996/02/18  23:35:50  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:01  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:00:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:13:53  root]
 * 
 * Revision 1.1.2.4  1994/07/15  13:29:29  marrek
 * 	Remove empty comment line.
 * 	[1994/07/15  10:02:45  marrek]
 * 
 * Revision 1.1.2.3  1994/07/07  07:02:41  marrek
 * 	Add version string in July 1994 code drop.
 * 	[1994/07/07  06:58:50  marrek]
 * 
 * Revision 1.1.2.2  1994/06/09  18:46:17  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:19:20  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:14:52  marrek
 * 	Bug fixes.
 * 	[1994/04/29  09:09:47  marrek]
 * 
 * 	Created for April Submission.
 * 	[1994/04/26  12:49:58  marrek]
 * 
 * $EndLog$
 */

/*module***********************************************************************
 *
 * PRODUCT NAME / SUBSYSTEM NAME:
 *    OCIS / ORGMAN
 * MODULE NAME / VERSION:
 *    omCharDef.h (%M%) / %I%
 * DATE / LAST MODIFICATION:
 *    %D% / %E%
 * DESCRIPTION:
 *    Include file with definitions of names and types
 *    for internal use of OmChar functions.
 * AUTHOR:
 *    Fiedrich, AP 154
 *    vers.|   date   |                changes                  | KZ | ER#/CR#
 *    -------------------------------------------------------------------------
 *    1.00 | 17.09.92 | initial state                           | Fd | 
 *    1.01 | 20.11.92 | conversion file paths changed           | Fd | internal
 *    1.02 | 05.04.93 | ISO8859/T.61 conversion deactivation    | Fd | internal
 *    1.03 | 14.04.93 | PATH_MAX for SUN defined                | Fd | internal
 *    1.04 | 19.05.93 | changes due to shell variable concept   | Fd | internal
 *
 *headerend*******************************************************************/

#ifndef _D27CHARDEF_H
#define _D27CHARDEF_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char d27chardef_rcsid[] = "@(#)$RCSfile: d27chardef.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:35:50 $";
#endif

/*****************/
/* include files */
/*****************/

#include <stdio.h>
#include <string.h>
#include <limits.h>

/********************/
/* conversion files */
/********************/

#define OmCHAR_FILE_T61 "/adm/orgConvT61"
		/* T.61 conversion file name, relative to root */
#define OmCHAR_FILE_PRS "/adm/orgConvPrs"
		/* Printable String conversion file name, relative to root */
#define OmCHAR_FILE_SWITCH "/adm/orgConvTrue"
		/* conversion switch file name, relative to root */

/********************/
/* parameter values */
/********************/

#ifndef FALSE                           /* logic FALSE */
#define FALSE 0
#endif

#ifndef TRUE                            /* logic TRUE */
#define TRUE 1
#endif

typedef enum                            /* conversion file selector */
{  OmCHAR_SEL_T61,                      /* T.61 conversion file */
   OmCHAR_SEL_PRS                       /* Printable String conversion file */
} OmCharSelFile;

					/* symbol types */
#define OmCHAR_EOF -1                   /* end of file */
#define OmCHAR_EOC -2                   /* end of conversion */
#define OmCHAR_INV -3                   /* invalid symbol */
					/* >0: number value */

#ifndef OCIS_RC_ADMIN                   /* logging ret. value for adm error */
#define OCIS_RC_ADMIN -2
#endif

/****************************************************************************/
/* control characters in conversion files and invalid character designation */
/****************************************************************************/

#define OmCHAR_BLK ' '                  /* blank */
#define OmCHAR_NLN '\n'                 /* newline */
#define OmCHAR_HT  '\t'                 /* horizonatal TAB */
#define OmCHAR_COM '#'                  /* start of comment marking */
#define OmCHAR_WC1 '*'                  /* first wildcard character */
#define OmCHAR_WC2 '%'                  /* second wildcard character */
#define OmCHAR_DV  '\002'               /* devaluation char. (theoretical) */
#define OmCHAR_SUB '?'                  /* substitution for invalid char. */

/*********************************/
/* character set codes and masks */
/*********************************/

#define CHS1 0x01 /* character set 0-9                  */
#define CHS2 0x02 /* character set 0-9 & BLANK          */
#define CHS3 0x04 /* character set 0-9 & a-z            */
#define CHS4 0x08 /* character set A-Z & a-z            */
#define CHS5 0x10 /* character set Printable String     */
#define CHS6 0x20 /* character set ASCII                */
#define CHS7 0x40 /* character set ISO 8859             */
#define CHS8 0x80 /* character set T.61                 */

/*****************************/
/* internally used functions */
/*****************************/

int OmCharReadFile(char *, OmCharSelFile);
int OmCharReadSymbol(FILE *);
void OmCharError(char *, int, char *);

#endif
