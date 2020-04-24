/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: spstep4.c,v $
 * Revision 1.1.4.2  1996/03/11  13:19:54  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:02  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:31:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:18:06  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:07:02  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:49:44  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:59:34  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:54:00  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:34:09  keutel
 * 	creation
 * 	[1994/03/21  13:23:43  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: spstep4.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:19:54 $" } ;
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
/* TYPE 	: MODULE					      */
/*								      */
/* NAME 	: spstep4.c					      */
/*								      */
/* AUTHOR	: Natalia Serra 				      */
/*								      */
/* DATE 	: 19-02-92					      */
/*								      */
/* COMPONENT	: RTS - Step					      */
/*								      */
/* PRD#/VERS.	: RTS V2.0					      */
/*								      */
/* DESCRIPTION	: This file contains primitives for writing	      */
/*	 - parameter block headers				      */
/*	 - parameter block trailers				      */
/*	 - type : value constructs				      */
/*	 - type : length : pointer : value constructs		      */
/*	 - binary and integer strings				      */
/*	 - searching through the Test File			      */
/*								      */
/* SYSTEM DEPENDENCE :	none					      */
/*								      */
/* USED PRODUCTS:   RTS V2.0					      */
/*		    CES V5.2   C-Entwicklungssystem		      */
/*								      */
/* HISTORY	:						      */
/*								      */
/* Vers.Nr. | Date    | Changes 		       | KZ | CR# FM# */
/*  2.50K010| 17.07.91| original		       | gj |	      */
/* 2.0A00K00| 19.02.92| adaptation from ROS to RTS     | ns |	      */
/*	    |	      | 			       |    |	      */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                     Module Identification                          */
/*                                                                    */
/**********************************************************************/

static char *sccsid =    "@(#) spstep4.c 1.2 93/12/20   RTS-V2.00";

/**********************************************************************/
/*                                                                    */
/*            I N C L U D E S                                         */
/*                                                                    */
/**********************************************************************/

#ifdef RP0000_MicV0600    /* -------------- start ifdef ----------------------*/

#ifdef R_G000_Windows
#define WINDOWS
#endif

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <windows.h>

#undef ERROR
#define WIN_INCLUDED
#define RP0002_WinIncl

#endif                    /* -------------- end   ifdef ----------------------*/


#include <stdio.h>
#include <string.h>
#include <time.h>
/* #include <varargs.h> */
#include <stdarg.h>


#include <rogenrl.h>	  /* RTS General definitions		      */
#include <rolgval.h>	  /* RTS General definitions for LOGGING	      */
#include <routil0.h>	  /* RTS utility prototype definitions        */

#include <spstep0.h>	  /* RTS-STEP General definitions	      */

/**********************************************************************/
/*                                                                    */
/*              D E F I N E S                                         */
/*                                                                    */
/**********************************************************************/

#define SP_MAXELEM	 100	 /* Max num of elems to be showed     */
#define SP_MAXTAB	  60	 /* Max blank chars in the left side  */
#define SP_INITINDENT	   0	 /* Initial left margin indent	      */
#define SP_MAXLINELEN	  80	 /* Max num of chars in one text line */
#define SP_CHAR 	"%c"	 /* ASCII character		      */
#define SP_NOCHAR	 "."	 /* No ASCII character		      */

/* Used in sp1268_hexElemDump() function */
#define CLRBF(bf) { ro0043_memset ((char FAR *)bf, (int)'.', (unsigned long int)16); bf [16] = 0; }

#ifdef RP0000_MicV0600    /* -------------- start ifdef ----------------------*/

#include <roerval.h>


FILE FAR *ulFp = (FILE FAR *)LNULL;
#define sprintf wsprintf
/*
#define fprintf wfprintf
*/
#else                     /* -------------- else  ifdef ----------------------*/

#define ulFp    stdout

#endif                    /* -------------- end   ifdef ----------------------*/

/**********************************************************************/
/*                                                                    */
/*              L O C A L  V A R I A B L E S                          */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*              L O C A L  F U N C T I O N S                          */
/*                                                                    */
/**********************************************************************/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1250_mainHead()				      */
/*                                                                    */
/* DESCRIPTION	: Main header of the generated output. It writes as   */
/*		  well the "filename" name of the login file.	      */
/*                                                                    */
/*inoff ***************************************************************/

void sp1250_mainHead (
	char FAR  *filename)

{       
   char *stern ="*******************************************************************************\n";

   fprintf(ulFp, "%s", stern);
   fprintf(ulFp, "*     LOGGING FILE-NAME:  %-40s              *\n",filename);
   fprintf(ulFp, "%s", stern);
   fprintf(ulFp, "\n");
}


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1251_recordHead()				      */
/*                                                                    */
/* DESCRIPTION	: Header which is printed for every showed record.    */
/*		  It writes, starting at the "offset" position, the   */
/*		  "module" and "function" names which have written    */
/*		  the record, the "rectype" record type and the       */
/*		  time ("wrttime") when the login record was written. */
/*                                                                    */
/*inoff ***************************************************************/

void sp1251_recordHead (
	int	    offset,
	char  FAR  *module,
	char  FAR  *function,
	long  int   rectype,
	long  int   wrttime,
	      int   errcount)

{
   struct tm FAR *cnv_time;

   cnv_time = localtime ((signed long int FAR *)&wrttime);

   if ((!wrttime) || (!cnv_time)) {
      struct tm st_cnv_time;
      
      cnv_time = &st_cnv_time; 
      cnv_time->tm_hour = 0;
      cnv_time->tm_min	= 0;
      cnv_time->tm_sec	= 0;
   }
   

  /*  sp1270_separator(); */
   sp1265_indent(offset);
   fprintf(ulFp, "%-15s%-30s RecTyp: 0x%.8lx   %.2d:%.2d:%.2d\n\n",
		      module,
		      function,
		      rectype,
		      cnv_time->tm_hour,
		      cnv_time->tm_min,
		      cnv_time->tm_sec);


   if (errcount > 0) {
      sp1265_indent(offset);
      fprintf(ulFp, "ERROR IN %u ELEMENT(S) (IN LOGGING FILE)\n\n", errcount);
   }
}  


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1252_blockBegin()				      */
/*		  sp1252_blockEnd()				      */
/*                                                                    */
/* DESCRIPTION	: Begin and end text for every parameter block.       */
/*                                                                    */
/*inoff ***************************************************************/

void sp1252_blockBegin (
	char FAR  *blockname)
{
   sp1265_indent(RC1201_OFFSET1);
   fprintf(ulFp, "%s: Begin of parameter block\n", blockname);
}


void sp1253_blockEnd (
	char FAR  *blockname)

{
   sp1265_indent(RC1201_OFFSET1);
   fprintf(ulFp, "%s: End of parameter block\n", blockname);
   fprintf(ulFp, "\n");
}


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/*		  sp1273_wrSgShrt()				      */
/*		  sp1254_wrSgInt()				      */
/*		  sp1255_wrSgLong()				      */
/*		  sp1274_wrUnShrt()				      */
/*		  sp1256_wrUnInt()				      */
/*		  sp1257_wrUnLong()				      */
/*		  sp1258_wrAddr()				      */
/*                                                                    */
/* DESCRIPTION	: Functions to write data.			      */
/*		  They write, in the "offset" position, the value     */
/*		  contained in the "p?value" pointer, preceded by     */
/*		  the descriptive string "text".		      */
/*                                                                    */
/*inoff ***************************************************************/

void sp1273_wrSgShrt (
	int		       offset,
	char              FAR *text,
	signed short int  FAR *psvalue,
	signed int	       bDec)
{
   sp1265_indent(offset);
   switch (bDec) {
      case RC1205_DEC:
	 fprintf(ulFp, "%s%d", text, *psvalue);
	 break;
      case RC1206_DEC_NL:
	 fprintf(ulFp, "%s%d\n", text, *psvalue);
	 break;
      case RC1207_HEX:
	 fprintf(ulFp, "%s0x%.4x", text, *psvalue);
	 break;
      case RC1208_HEX_NL:
      default:
	 fprintf(ulFp, "%s0x%.4x\n", text, *psvalue);
   }
}


void sp1254_wrSgInt (
	int	         offset,
	char        FAR *text,
	signed int  FAR *pivalue,
	signed int       bDec)
{
   sp1265_indent(offset);
   switch (bDec) {
      case RC1205_DEC:
	 fprintf(ulFp, "%s%d", text, *pivalue);
	 break;
      case RC1206_DEC_NL:
	 fprintf(ulFp, "%s%d\n", text, *pivalue);
	 break;
      case RC1207_HEX:
	 fprintf(ulFp, "%s0x%.4x", text, *pivalue);
	 break;
      case RC1208_HEX_NL:
      default:
	 fprintf(ulFp, "%s0x%.4x\n", text, *pivalue);
   }
}


void sp1255_wrSgLong (
	int		      offset,
	char             FAR *text,
	signed long int  FAR *plvalue,
	signed int	      bDec)
{
   sp1265_indent(offset);
   switch (bDec) {
      case RC1205_DEC:
	 fprintf(ulFp, "%s%ld", text, *plvalue);
	 break;
      case RC1206_DEC_NL:
	 fprintf(ulFp, "%s%ld\n", text, *plvalue);
	 break;
      case RC1207_HEX:
	 fprintf(ulFp, "%s0x%.8x", text, *plvalue);
	 break;
      case RC1208_HEX_NL:
      default:
	 fprintf(ulFp, "%s0x%.8x\n", text, *plvalue);
   }
}


void sp1274_wrUnShrt (
	int		         offset,
	char                FAR	*text,
	unsigned short int  FAR *psvalue,
	signed	 int	         bDec)
{
   sp1265_indent(offset);
   switch (bDec) {
      case RC1205_DEC:
	 fprintf(ulFp, "%s%u", text, *psvalue);
	 break;
      case RC1206_DEC_NL:
	 fprintf(ulFp, "%s%u\n", text, *psvalue);
	 break;
      case RC1207_HEX:
	 fprintf(ulFp, "%s0x%.4x", text, *psvalue);
	 break;
      case RC1208_HEX_NL:
      default:
	 fprintf(ulFp, "%s0x%.4x\n", text, *psvalue);
   }
}


void sp1256_wrUnInt (
	int	           offset,
	char          FAR *text,
	unsigned int  FAR *pivalue,
	signed	 int       bDec)

{
   sp1265_indent(offset);
   switch (bDec) {
      case RC1205_DEC:
	 fprintf(ulFp, "%s%u", text, *pivalue);
	 break;
      case RC1206_DEC_NL:
	 fprintf(ulFp, "%s%u\n", text, *pivalue);
	 break;
      case RC1207_HEX:
	 fprintf(ulFp, "%s0x%.4x", text, *pivalue);
	 break;
      case RC1208_HEX_NL:
      default:
	 fprintf(ulFp, "%s0x%.4x\n", text, *pivalue);
   }
}


void sp1257_wrUnLong (
	int		        offset,
	char               FAR *text,
	unsigned long int  FAR *plvalue,
	signed int	        bDec)

{
   sp1265_indent(offset);
   switch (bDec) {
      case RC1205_DEC:
	 fprintf(ulFp, "%s%lu", text, *plvalue);
	 break;
      case RC1206_DEC_NL:
	 fprintf(ulFp, "%s%lu\n", text, *plvalue);
	 break;
      case RC1207_HEX:
	 fprintf(ulFp, "%s0x%.8x", text, *plvalue);
	 break;
      case RC1208_HEX_NL:
      default:
	 fprintf(ulFp, "%s0x%.8x\n", text, *plvalue);
   }
}


void sp1258_wrAddr (
	int	   offset,
	char FAR  *text,
	char FAR  *addr)

{
   sp1265_indent(offset);
   fprintf(ulFp, "%s0x%.8x\n", text, (unsigned long int)addr);
}


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1259_lstSgShrt()				      */
/*		  sp1260_lstSgLong()				      */
/*		  sp1261_lstSgInt()				      */
/*                                                                    */
/* DESCRIPTION	: Functions to write data.			      */
/*		  They write, in the "offset" position, a list of     */
/*		  "len" or "SP_MAXELEM" signed long/int values	      */
/*		  pointed by "ptr", and preceded by the descriptive   */
/*		  string "text".				      */
/*                                                                    */
/*inoff ***************************************************************/

void sp1259_lstSgShrt (
	int	    offset,
	char  FAR  *text,
	short FAR  *ptr,
	int	    len)

{
   int i, textOff, cont;

   sp1265_indent(offset);
   fprintf(ulFp, "%s", text);
   textOff = cont = SP_INITINDENT + offset + ro0053_strlen (text);
   for (i = 0; i < (len > SP_MAXELEM ? SP_MAXELEM : len); i++, ptr++) {
      cont += 7;
      if (cont > SP_MAXLINELEN) {
	 fprintf(ulFp, "\n");
	 sp1265_indent(offset + ro0053_strlen (text));
	 cont = textOff + 7;
      }
      fprintf(ulFp, " %6d", *ptr);
   }
   fprintf(ulFp, "\n");
}


void sp1260_lstSgLong (
	int	     offset,
	char FAR    *text,
	long FAR    *ptr,
	int	     len,
	signed int   bDec)

{
   int i, textOff, cont;

   sp1265_indent(offset);
   fprintf(ulFp, "%s", text);
   textOff = cont = SP_INITINDENT + offset + ro0053_strlen (text);
   for (i = 0; i < (len > SP_MAXELEM ? SP_MAXELEM : len); i++, ptr++) {
      cont += 11;
      if (cont > SP_MAXLINELEN) {
	 fprintf(ulFp, "\n");
	 sp1265_indent(offset + ro0053_strlen (text));
	 cont = textOff + 11;
      }
      if (bDec)
	 fprintf(ulFp, " %10ld", *ptr);
      else
	 fprintf(ulFp, " 0x%.8x", *ptr);
   }
   fprintf(ulFp, "\n");
}


void sp1261_lstSgInt (
	int	   offset,
	char FAR  *text,
	int  FAR  *ptr,
	int	   len)

{
   int i, textOff, cont;

   sp1265_indent(offset);
   fprintf(ulFp, "%s", text);
   textOff = cont = SP_INITINDENT + offset + ro0053_strlen (text);
   for (i = 0; i < (len > SP_MAXELEM ? SP_MAXELEM : len); i++, ptr++) {
      cont += 11;
      if (cont > SP_MAXLINELEN) {
	 fprintf(ulFp, "\n");
	 sp1265_indent(offset + ro0053_strlen (text));
	 cont = textOff + 11;
      }
      fprintf(ulFp, " %10d", *ptr);
   }
   fprintf(ulFp, "\n");
}


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1262_asciiOrHexDump()			      */
/*                                                                    */
/* DESCRIPTION	: Function to write data.			      */
/*		  It writes, in the "offset" position, a list of      */
/*		  "len" bytes pointed by "ptr", and preceded by the   */
/*		  descriptive string "text". If "bascii" is TRUE,     */
/*		  the output is written in ASCII format, and if not,  */
/*		  it is written in hexadecimal format.		      */
/*                                                                    */
/*inoff ***************************************************************/

void sp1262_asciiOrHexDump (
	int	               offset,
	char              FAR *text,
	char              FAR *ptr,
	unsigned long int      len,
	signed        int      bascii)

{
   unsigned long int i = 0;
   int textOff, cont, inc, extr;

   sp1265_indent(offset);
   fprintf(ulFp, "%s", text);
   if (bascii == TRUE) {
      inc  = 1;
      extr = 0;
   } else {
      inc = extr = 3;
      fprintf(ulFp, "0x ");
   }
   textOff = cont = SP_INITINDENT + offset + ro0053_strlen (text) + extr;
   for (i = 0; i < len; i++, ptr++) {
      cont += inc;
      if (cont > SP_MAXLINELEN) {
	 fprintf(ulFp, "\n");
	 sp1265_indent(offset + ro0053_strlen (text) + extr);
	 cont = textOff + inc;
      }
      if (bascii == TRUE) {
	 if (*ptr > 0x20 && *ptr < 0x7f)
	    fprintf(ulFp, SP_CHAR, *ptr);
	 else
	    fprintf(ulFp, SP_NOCHAR);
      } else {
	 fprintf(ulFp, "%.2x ", (unsigned char)*ptr);
      }
   }
   fprintf(ulFp, "\n");
}


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1263_hexDump()				      */
/*                                                                    */
/* DESCRIPTION	: Hexadecimal memory dump.			      */
/*		  Memory dump of "len" memory bytes (max. 200)	      */
/*		  starting at "ptr" position. The "textline"	      */
/*		  parameter is written in the first line.	      */
/*		  Dump: 16 bytes are showed in each line, in 4 groups */
/*		  of 4 bytes each, and on the right side, the bytes   */
/*		  which are ASCII characters are written again.       */
/*                                                                    */
/*inoff ***************************************************************/

void sp1263_hexDump (
	int	                 offset,
	char               FAR  *textline,
	char               FAR  *ptr,
	unsigned long int        len)

{

/*   short      size1, size2; by kc on 16-11-93 for dos warning */
   unsigned  long int size1, size2;

/*   short      newsize = (signed short int)(len > 200 ? 200 : len); */
   unsigned  long int newsize = len; /* by kc on 16-11-93 */
   char FAR  *p1 = ptr;
   char FAR  *p2 = ptr + newsize;
   char       buf[80];
   char  FAR *buf2;

   sp1265_indent(offset);
   fprintf(ulFp, "%s\n", textline);

   buf2 = (char FAR *)buf;
   if ((size2 = newsize % 16) != 0)
      newsize += 16 - size2;
   sprintf(buf2, (char FAR *)"%6x", 0);
   buf2 += ro0053_strlen (buf2);
   for (size1 = 0; size1 <= newsize; size1++) {
      if (size1 > 0 && (size1 % 16) == 0) {
	 sprintf(buf2, (char FAR *)" |");
         buf2 += ro0053_strlen (buf2);
	 for (size2 = 0; size2 < 16; size2++, p1++) {
            if (p1 < p2 && *p1 > 0x20 && *p1 < 0x7f)
	       sprintf(buf2, (char FAR *)SP_CHAR, *p1);
            else
	       sprintf(buf2, (char FAR *)SP_NOCHAR);
	    buf2 += ro0053_strlen (buf2);
         }
	 sprintf(buf2, (char FAR *)"|\n");
	 buf2 += ro0053_strlen (buf2);
	 sp1265_indent(offset + 4);
	 fprintf(ulFp, (char FAR *)"%s", (char FAR *)buf);
	 buf2 = (char FAR *)buf;
	 if (size1 == newsize)
	    break;

	 sprintf(buf2, (char FAR *)"0x%.4x", size1);
	 buf2 += ro0053_strlen (buf2);
      }
      if ((size1 % 4) == 0) {
	 sprintf(buf2, (char FAR *)" ");
	 buf2 += ro0053_strlen (buf2);
      }
      if (ptr < p2)
	 sprintf(buf2, (char FAR *)"%02x", *ptr++ & 0xff);
      else
	 sprintf(buf2, (char FAR *)"  ");
      buf2 += ro0053_strlen (buf2);
   }
}


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1264_write()				      */
/*                                                                    */
/* DESCRIPTION	: Call "printf()" function with the same parameters.  */
/*                                                                    */
/*inoff ***************************************************************/

void sp1264_write (
	int	   offset,
	char FAR  *fmt,
	char FAR  *arg1,
	char FAR  *arg2,
	char FAR  *arg3,
	char FAR  *arg4)

{
   sp1265_indent(offset);
   fprintf(ulFp, fmt, arg1, arg2, arg3, arg4);
}


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1265_indent()				      */
/*                                                                    */
/* DESCRIPTION	: Writes "offset" blanks after the SP_INITINDENT      */
/*		  position.					      */
/*                                                                    */
/*inoff ***************************************************************/

void sp1265_indent (       /* Write the left margin indent.*/
	int   offset)

{
   if ((SP_INITINDENT + offset) != 0)
/*    fprintf(ulFp, "%*c\0",
	  ((SP_INITINDENT + offset) > SP_MAXTAB ? SP_MAXTAB
						: (SP_INITINDENT + offset)),
	  ' '); */
    fprintf(ulFp, "%*c",
	  ((SP_INITINDENT + offset) > SP_MAXTAB ? SP_MAXTAB
						: (SP_INITINDENT + offset)),
	  ' '); 
}


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1266_hexDumpAll()				      */
/*                                                                    */
/* DESCRIPTION	: Hexadecimal memory dump.			      */
/*		  Memory dump of "argn" parameters, which are	      */
/*		  available through an array of pointers "argv".      */
/*		  The length of each element is stored in "argl",     */
/*		  which is an array of long values		      */
/*		  (argl[i] = length of argv[i]).		      */
/*                                                                    */
/*inoff ***************************************************************/

void sp1266_hexDumpAll(
	unsigned       int             argn,
	char               FAR * FAR * argv,
	unsigned long  int       FAR * argl,
	unsigned	               flag)

{
   unsigned short int i;

   fprintf(ulFp, "    RECORD WITH (%u) ELEMENTS\n\n", argn);
   for (i = 0; i < argn; i++) {
      if (argl[i] > 0) {
	 fprintf(ulFp, "*** Element (%u), length (%lu) ***\n\n", i, argl[i]);
	 sp1268_hexElemDump((unsigned char FAR *)argv[i], (unsigned)argl[i],
			    flag);
	 fprintf(ulFp, "\n");
      } else {
	 fprintf(ulFp, "*** Element (%u) is empty ***\n\n", i);
      }
   }
   sp1270_separator();
}


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1267_hexRecDump()				      */
/*                                                                    */
/* DESCRIPTION	: Hexadecimal memory dump.			      */
/*		  Dump of a Login Record pointed by "ptr" and which   */
/*		  length is "len". It shows all the elements of the   */
/*		  record, if it is a packed one.		      */
/*                                                                    */
/*inoff ***************************************************************/

void sp1267_hexRecDump (
	unsigned char FAR  *ptr,
	unsigned long	    len,
	unsigned int	    shownull,
	signed long int     isPacked)

{
   unsigned short int	i;
   unsigned short int	n_recs;
   unsigned short int	el_len;
   unsigned short int	c_cnt = 0;

   if (isPacked == RLro01_PackedRecord) {
      n_recs = *ptr++;
      fprintf(ulFp, "----Packed Record with (%u) elements----\n", n_recs);
      c_cnt++;
      for (i = 1; i <= n_recs; i++) {
         el_len = *ptr;
	 fprintf(ulFp, "----Packed Element (%u), length (%u)----\n", i, el_len);
	 ptr++;
	 c_cnt++;
         if (c_cnt <= len)
	    sp1268_hexElemDump(ptr, (unsigned)el_len, shownull);
         else
            return;
         ptr += el_len; c_cnt += el_len;
      }
   } else
      sp1268_hexElemDump(ptr, (unsigned)len, shownull);
}



/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1269_isAscii()				      */
/*		  sp1270_separator()				      */
/*		  sp1271_wrLine()				      */
/*		  sp1272_strcpy()				      */
/*                                                                    */
/* DESCRIPTION	: Other general functions			      */
/*                                                                    */
/*inoff ***************************************************************/

int sp1269_isAscii (
	char  c)

{
   return((c > ' ') && (c < '~'));
}


void sp1270_separator (void )

{
   char *line ="--------------------------------------------------------------------------------\n";

   fprintf(ulFp, "%s", line);
   fprintf(ulFp, "\n");
}


void sp1271_wrLine (void )

{
   fprintf(ulFp, "\n");
}


/* **skh: For DOS port: 11/11/93
void sp1272_strcpy (
	char  *dest,
	char  *sour)

{
   strcpy(dest, sour);
}

int sp1275_strlen (
	char  *string)

{
   return(strlen(string));
}
*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1268_hexElemDump()				      */
/*                                                                    */
/* DESCRIPTION	: Hexadecimal memory dump.			      */
/*		  Dump of a single element in a Login Record.	      */
/*		  It dumps the memory contents in hexadecimal groups  */
/*		  of 16 bytes in the left side and the characters     */
/*		  that can be showed in ASCII format in the right     */
/*		  side. 					      */
/*                                                                    */
/*inoff ***************************************************************/

void sp1268_hexElemDump (
	unsigned char FAR  *ptr,
	unsigned	    len,
	unsigned	    shownull)

{
   unsigned            i = 0;
   int		       count = 0;
   char 	       buf[40];
   char FAR	      *auxPtr = buf;
   unsigned char FAR  *q = ptr;
   unsigned int        l;

   if (shownull)
      q = 0;
   CLRBF (buf);
   fprintf(ulFp, "------ Hex Dump: %d bytes starting at 0x%08x: ------\n", len,  (unsigned long int)q);
   for (i = 1; i <= len; i++, ptr++, q++) {
      if (i % 16 == 1) 
	 fprintf(ulFp, "%.8x   ", (unsigned long int)q);
      l = (unsigned int)*ptr;
      fprintf(ulFp, "%.2x ", l);

/* **skh: Removal of warning: Possible conflict in order of operator.
      *(auxPtr++) = sp1269_isAscii(*ptr) ? *ptr : '.';  
*/
      *(auxPtr) = sp1269_isAscii(*ptr) ? *ptr : '.';  
      auxPtr++;
      count ++;
      if (i % 16 == 0) {
	 fprintf(ulFp, (char FAR *)"\t%s\n", (char FAR *)buf);
	 auxPtr = buf;
         count = 0;
         CLRBF (buf);
      }
   }
   if (count) {
      fprintf(ulFp, (char FAR *)"%*c", (16 - count) * 3, ' ');
      fprintf(ulFp, (char FAR *)"\t%s\n", (char FAR *)buf);
   }
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1280_writeBool                                    */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 27-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp1280_writeBool (
	signed int          iOffset,
	char FAR        *pStr,
	signed int        iValue)
/*                                                                    */
/* DESCRIPTION  :write Boolean TRUE of FALSE(0) to the screen         */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
/*                                                                    */
/* RETURN VALUES              :   Void                                */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/
/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
char vStr[16]="TRUE";
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
 if(iValue == FALSE)
      ro0050_strcpy((char FAR *)vStr,(char FAR *)"FALSE");
 sp1264_write(iOffset,"%s%s\n",pStr,vStr,NULL,NULL);

}/*end*/
 

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1281_writePduHdr                                  */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 29-11-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp1281_writePduHdr( 
	char    FAR *pStr1,
	char    FAR *pStr2)
/*                                                                    */
/* DESCRIPTION  :write the PDU header                                 */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
/*                                                                    */
/* RETURN VALUES              :   Void                                */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/
/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
 sp1264_write(RC1200_OFFSET0,"%s%s%s%s\n",
"--------------------",
pStr1,
                                 "---------------------",pStr2);

}/*end*/

 
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1282_WrObjId  				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

void sp1282_WrObjId ( 
	int                    offset,
	char              FAR *text,
	char              FAR *p,
	unsigned long int      ulLen)


/*                                                                    */
/* DESCRIPTION  : prints the Objid which is passed with first elemsent*/
/*                as th elength of Objid.                             */
/*                                                                    */
/* IN-PARAMETERS              :   pointer to ObjId                    */
/*                                                                    */
/* OUT-PARAMETERS             :   none                                */
/*                                                                    */
/* RETURN VALUES              :   none                                */
/*                                                                    */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/
/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
unsigned long int   FAR * pInt = NULL;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
      sp1264_write(offset,"%s",text,NULL,NULL,NULL);
      if (ulLen > 0){
          ulLen /= sizeof(unsigned long int);
          pInt = (unsigned long int FAR *) p;
          while(ulLen --){
            sp1257_wrUnLong(NULL, "",
                      (unsigned long int  FAR *)pInt++ , RC1205_DEC);
            sp1265_indent(2);
         }
    }
      sp1271_wrLine();
     
}/*end*/                                                                        

        
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1285_startUpStep                                  */
/*                sp1286_cleanUpStep                                  */
/*                                                                    */
/* DESCRIPTION	: Performs the startUp and cleanUp related STEP       */
/*                activities.                                         */
/*                                                                    */
/*inoff ***************************************************************/

#ifdef RP0000_MicV0600

signed short int sp1285_startUpStep (
	signed         char FAR  *lpszFileName,
	signed         char FAR  *fuMode,
	FILE                FAR * FAR *pulFileHandle)
{
/*
*pulFileHandle = ulFp = ro00f1_fopen (lpszFileName, fuMode);
if (ulFp == LNULL) return (-R_FOPERR);
else               return (OK);
*/
*pulFileHandle = ulFp = fopen (lpszFileName, fuMode);
if (ulFp == LNULL) return (-R_FOPERR);
else               return (OK);
}

signed short int sp1286_cleanUpStep (
	FILE           FAR *pulFileHandle)
{
/*
return (ro00f2_fclose (ulFileHandle));
*/
return (fclose (pulFileHandle));
}

signed short int wfprintf (
	unsigned long int        ulFileHandle,
	signed        char  FAR *lpszFormat,
	va_dcl)
{
    char buf [1024];
    char FAR *buf2;

    buf2 = (char FAR *)buf;

    sprintf (buf2, lpszFormat, va_alist);
    return (ro00f3_fwrite (buf2,
                           (unsigned long int)ro0053_strlen (buf2),
                           (unsigned long int)1,
                           ulFileHandle));
}
#else
signed short int sp1285_startUpStep (
	signed         char FAR  *lpszFileName,
	signed         char FAR  *fuMode,
	FILE           FAR * FAR *pulFileHandle)
{
return (OK);
}

signed short int sp1286_cleanUpStep (
	FILE          FAR *pulFileHandle)
{
return (OK);
}

signed short int wfprintf (
	unsigned long int        ulFileHandle,
	signed        char  FAR *lpszFormat,
      ...)
{
    va_list v;

    va_start(v,lpszFormat) ; 
    vfprintf (ulFp,(char FAR *)lpszFormat, v); 
    va_end(v) ; 
    return (OK);
}  
#endif

/*********************	    END OF SPSTEP4.C	  *********************/
