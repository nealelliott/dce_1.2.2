/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: spstep0.h,v $
 * Revision 1.1.4.2  1996/03/11  13:21:34  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:10:21  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:37:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:20:50  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:06:19  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  15:00:00  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:55  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:59:41  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:37:12  keutel
 * 	creation
 * 	[1994/03/21  13:28:57  keutel]
 * 
 * $EndLog$
 */
#ifndef _SPSTEP0_H
#define _SPSTEP0_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _spstep0_rcsid[] = { "@(#) $RCSfile: spstep0.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:21:34 $" } ;
#endif

/**********************************************************************/
/*                                                                    */
/*    COPYRIGHT  (C)						      */
/*    Siemens Nixdorf Informationssysteme AG 1991		      */
/*    All rights reserved					      */
/*								      */
/*    This software is furnished under licence and may be used only   */
/*    in accordance with the terms of that licence and with the       */
/*    inclusion of the above copyright notice.			      */
/*    This software may not be provided or otherwise made available   */
/*    to, or used by, any other person. No title to or ownership of   */
/*    the software is hereby transferred.			      */
/*                                                                    */
/**********************************************************************/

/*daton ***************************************************************/
/*								      */
/* TYPE         : INCLUDE                                             */
/*								      */
/* NAME 	: spstep0.h					      */
/*								      */
/* AUTHOR	: Natalia Serra 				      */
/* DATE 	: 04-03-92					      */
/*								      */
/* COMPONENT	: RTS / STEP					      */
/*								      */
/* PRD#/VERS.	: RTS-V2.00					      */
/*								      */
/* DESCRIPTION	: RTS-STEP General definitions and functions	      */
/*		  prototyping					      */
/*								      */
/* SYSTEM DEPENDENCE :	none					      */
/*								      */
/* USED PRODUCTS:   RTS V2.00					      */
/*		    CES V5.2   C-Entwicklungssystem		      */
/*								      */
/*								      */
/* Vers.Nr. | Date    | Changes 		       |mark| CR# EM# */
/* 2.00 K0  | 04-03-92| original		       | ns |	      */
/* 2.00 K0  | 18-03-92| add. function prototyping      | ns |	      */
/*	    |	      | 			       |    |	      */
/*datoff **************************************************************/

#define EMPTY          "\0"

#define RC1200_OFFSET0	    0
#define RC1201_OFFSET1	    9
#define RC1202_OFFSET2	    RC1201_OFFSET1 +  4
#define RC1203_OFFSET3	    RC1201_OFFSET1 +  8
#define RC1204_OFFSET4	    RC1201_OFFSET1 + 10

#define RC1205_DEC	    0
#define RC1206_DEC_NL	    2
#define RC1207_HEX	    1
#define RC1208_HEX_NL	    3

/*--------------------------------------------------------------------*/
void sp1250_mainHead PROTO ((char FAR *));

/* P1 : name of the Login file to be showed			      */

/*--------------------------------------------------------------------*/
void sp1251_recordHead PROTO ((int,
			       char FAR *,
			       char FAR *,
			       long int,
			       long int,
			             int));

/* P1 : indenting offset					      */
/* P2 : calling module						      */
/* P3 : calling function					      */
/* P4 : record type						      */
/* P5 : time							      */
/* P6 : number of wrong elements in the record			      */

/*--------------------------------------------------------------------*/
void sp1252_blockBegin PROTO ((char FAR *));

/* P1 : block's name to be showed (max. 10 chars)                     */

/*--------------------------------------------------------------------*/
void sp1253_blockEnd PROTO ((char FAR *));

/* P1 : block's name to be showed (max. 10 chars)                     */

/*--------------------------------------------------------------------*/
void sp1273_wrSgShrt PROTO ((int,
			     char FAR *,
			     signed short int FAR *,
			     signed int));

void sp1254_wrSgInt PROTO ((int,
			    char FAR *,
			    signed int FAR *,
			    signed int));

void sp1255_wrSgLong PROTO ((int,
			     char FAR *,
			     signed long int FAR *,
			     signed int));

void sp1274_wrUnShrt PROTO ((int,
			     char FAR *,
			     unsigned short int FAR *,
			     signed int));

void sp1256_wrUnInt PROTO ((int,
			    char FAR *,
			    unsigned int FAR *,
			    signed int));

void sp1257_wrUnLong PROTO ((int,
			     char FAR *,
			     unsigned long int FAR *,
			     signed int));

/* P1 : indenting offset					      */
/* P2 : descriptive string					      */
/* P3 : pointer to the value to be printed			      */
/* P4 : write the value in decimal or in hexadecimal format	      */

/*--------------------------------------------------------------------*/
void sp1258_wrAddr PROTO ((int,
			   char FAR *,
			   char FAR *));

/* P1 : indenting offset					      */
/* P2 : descriptive string					      */
/* P3 : address to be printed					      */

/*--------------------------------------------------------------------*/
void sp1259_lstSgShrt PROTO ((int,
			      char FAR *,
			      short FAR *,
			      int));

/* P1 : indenting offset					      */
/* P2 : descriptive string					      */
/* P3 : pointer to a list of long elements			      */
/* P4 : number of elements in the list				      */

/*--------------------------------------------------------------------*/
void sp1260_lstSgLong PROTO ((int,
			      char FAR *,
			      long FAR *,
			      int,
			      signed int));

/* P1 : indenting offset					      */
/* P2 : descriptive string					      */
/* P3 : pointer to a list of long elements			      */
/* P4 : number of elements in the list				      */
/* P5 : write the value in decimal format (TRUE) or in hexadecimal    */

/*--------------------------------------------------------------------*/
void sp1261_lstSgInt PROTO ((int,
			     char FAR *,
			     int FAR *,
			     int));

/* P1 : indenting offset					      */
/* P2 : descriptive string					      */
/* P3 : pointer to a list of int elements			      */
/* P4 : number of elements in the list				      */

/*--------------------------------------------------------------------*/
void sp1262_asciiOrHexDump PROTO ((int,
				   char FAR *,
				   char FAR *,
				   unsigned long int,
				   signed int));

/* P1 : indenting offset					      */
/* P2 : descriptive string					      */
/* P3 : ptr to a memory area which will be showed as Ascii bytes      */
/* P4 : length of the area to be showed 			      */
/* P4 : dump in ascii (TRUE) or in hexadecimal			      */

/*--------------------------------------------------------------------*/
void sp1263_hexDump PROTO ((int,
			    char FAR *,
			    char FAR *,
			    unsigned long int));

/* P1 : indenting offset					      */
/* P2 : descriptive string					      */
/* P3 : ptr to a memory area to be showed			      */
/* P4 : length of the area					      */

/*--------------------------------------------------------------------*/
void sp1264_write PROTO ((int,
			  char FAR *,
			  char FAR *,
			  char FAR *,
			  char FAR *,
			  char FAR *));

/* P1 : indenting offset					      */
/* P2 : format for a "printf()" call				      */
/* P3, P4, P5, P6 : parameters for a "printf()" call		      */

/*--------------------------------------------------------------------*/
void sp1265_indent PROTO ((int));

/* P1 : blank chars in the left margin				      */

/*--------------------------------------------------------------------*/
void sp1266_hexDumpAll PROTO ((unsigned      int            ,
			       char              FAR * FAR *,
			       unsigned long int       FAR *,
			       unsigned      int            ));

/* P1 : number of parameters to be showed (max. 16)		      */
/* P2 : array of pointers to the parameters to be showed	      */
/* P3 : array of parameters's lengths (P3[i] == length of P2[i])      */
/* P4 : flag: show parameter's pointer or NULL                        */

/*--------------------------------------------------------------------*/
void sp1267_hexRecDump PROTO ((unsigned char FAR *,
			       unsigned long,
			       unsigned int,
			       signed long int));

/* P1 : pointer to a memory area to be showed			      */
/* P2 : area length						      */
/* P3 : flag: show P1 pointer or NULL				      */
/* P4 : record type (packed or not)				      */

/*--------------------------------------------------------------------*/
void sp1268_hexElemDump PROTO ((unsigned char FAR *,
				unsigned,
				unsigned));

/* P1 : pointer to a memory area to be showed			      */
/* P2 : area length						      */
/* P3 : flag: show P1 pointer or NULL				      */

/*--------------------------------------------------------------------*/
int sp1269_isAscii PROTO ((char));

/* P1 : char to be tested					      */
/* return value: != 0 : "c" is an ascii char			      */
/*		 == 0 : it is not				      */

/*--------------------------------------------------------------------*/
void sp1270_separator PROTO ((void));

/*--------------------------------------------------------------------*/
void sp1271_wrLine PROTO ((void));

/*--------------------------------------------------------------------*/
/* **skh: For DOS port: 11/11/93
void sp1272_strcpy PROTO ((char *, char *));
int  sp1275_strlen PROTO ((char *));
*/
void sp1280_writeBool PROTO ((signed int       iOffset,
                              char FAR *pStr,
                              signed int       iValue));

void sp1290_writeAbortReason PROTO ((
unsigned long int          ulVal));                

signed short int wfprintf PROTO ((
unsigned long int        ulFileHandle,
signed        char  FAR *lpszFormat,
...));


#ifndef _STDIO_H
#include <stdio.h>
#endif

signed short int sp1285_startUpStep PROTO ((
signed         char FAR  *lpszFileName,
signed         char FAR  *fuMode,
FILE            FAR * FAR*pulFileHandle));

signed short int sp1286_cleanUpStep PROTO ((FILE         FAR  *pulFileHandle));
void sp1281_writePduHdr PROTO ((
char    FAR *pStr1,                   
char    FAR *pStr2));                   

void sp1282_WrObjId PROTO (( 
int                    offset,                
char              FAR *text,                  
char              FAR *p,                     
unsigned long int      ulLen));                 

#endif	/* _SPSTEP0_H */
