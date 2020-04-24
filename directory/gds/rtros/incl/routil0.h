/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: routil0.h,v $
 * Revision 1.1.4.2  1996/03/11  13:21:23  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:10:10  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:37:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:20:32  root]
 * 
 * Revision 1.1.2.5  1994/08/24  08:35:29  marrek
 * 	Fix warnings. (OT 11824)
 * 	[1994/08/23  14:35:09  marrek]
 * 
 * Revision 1.1.2.4  1994/08/22  15:22:00  keutel
 * 	OT 11620
 * 	[1994/08/19  13:49:17  keutel]
 * 
 * Revision 1.1.2.3  1994/06/13  15:06:10  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:51  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:46  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:59:16  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:36:59  keutel
 * 	creation
 * 	[1994/03/21  13:28:37  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROUTIL0_H
#define _ROUTIL0_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _routil0_rcsid[] = { "@(#) $RCSfile: routil0.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:21:23 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)                                                  */
/*  * Siemens Nixdorf Informationssysteme AG 1992                     */
/*  * All rights reserved                                             */
/*  *                                                                 */
/*  * This software is furnished under licence and may be used only   */
/*  * in accordance with the terms of that licence and with the       */
/*  * inclusion of the above copyright notice.                        */
/*  * This software may not be provided or otherwise made available   */
/*  * to, or used by, any other person. No title to or ownership of   */
/*  * the software is hereby  transferred.                            */
/*                                                                    */
/* ****************************************************************** */

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : INCLUDE                                             */
/*                                                                    */
/* NAME         : routil0.h                                           */
/*                                                                    */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : MAR-5th-1992                                        */
/*                                                                    */
/* COMPONENT    : RTS common utilities  external interface            */
/*                                                                    */
/* PRD#/VERS.   : RTS-V02.00                                          */
/*                                                                    */
/* DESCRIPTION  : RTS common utilities  external interface            */
/*                                                                    */
/*                - Function trace- , logging system faciltity        */
/*                - Memory Management facility                        */
/*                - Mapped functions                                  */
/*                                                                    */
/*                Compiler options :                                  */
/*                                                                    */
/*                - RC0000_TRACE                                      */
/*                - RC0007_CHECKMALLOC                                */
/*                                                                    */
/* SYSTEM DEPENDENCE :                                                */
/*                                                                    */
/* HISTORY      : sccsid  = "@(#)routil0.h  1.2 93/12/20"                         */
/*                                                                    */
/* Vers.Nr. | Date    |  changes                       |mark| CR# EM# */
/* 02.00    | 92-02-17|  original                      | gj |         */
/* 02.01    | 92-03-05|  Malloc-checks introduced      | gj |         */
/* 03.00    | 93-05-10|  New prototypes for file hdling| ma/|         */
/*          |         |  such as tmpnam remove etc.    |sss |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/* ****************************************************************** */
/* ********** Logging system-, function trace facility ************** */
/* ********** (routil2.c)                              ************** */
/* ****************************************************************** */

/* ****************** Function trace macros ************************* */
/* ******************                       ************************* */

#ifdef  RC0000_TRACE
#define RM0001_STRG(strName,string)     static char *strName = string;
#define RM0002_ENTRY(fnctName)          ro0010_entry(fnctName);
#define RM0003_RETURN(fnctName,type,va) {type __v = va;   \
                                        ro0011_return(fnctName,(long int)__v); \
                                        return (__v);}
#define RM0004_RETURN_VOID(fnctName)  { ro0011_return(fnctName,(long int)0); \
                                        return ;}

signed short int PASCAL ro0010_entry  PROTO ((char FAR *));
signed short int PASCAL ro0011_return PROTO ((char FAR *, long int));

#else
#define RM0001_STRG(strName,string)
#define RM0002_ENTRY(fnctName)
#define RM0003_RETURN(fnctName,type,va)  return (va);
#define RM0004_RETURN_VOID(fnctName)  return ;
#endif

/* ****************** Logging API *********************************** */
/* ******************             *********************************** */

#define RC0020_INTERFACE                2          /*   - ROS logging */
#define RC0021_TRACE                    1          /*     levels.     */
#define RC0022_ACTIVITY                 4
#define RC0023_CWAITTRACE               8

#define RC0024_ProvOptions              (RC0020_INTERFACE  | \
                                         RC0021_TRACE      | \
                                         RC0022_ACTIVITY   | \
                                         RC0023_CWAITTRACE )

#define RC0025_NewFile                  0          /*   - Logging file*/
#define RC0026_ExtendFile               1          /*     open mode.  */


signed short int PASCAL ro0040_openLog PROTO ((char FAR *,
                                               unsigned short int,
                                               unsigned short int,
                                               unsigned short int ));

/* Return value == 0:  if successful,  */
/*              <> 0:  if error.       */
/* Parameter1       :  FileName        */
/* Parameter2       :  FileMode        */
/* Parameter3       :  Recordnum       */
/* Parameter4       :  Options         */


signed short int  PASCAL ro0041_closeLog PROTO ((void));

/* Return value == 0:  if successful,  */
/*              <> 0:  if error.       */

signed short int  PASCAL ro0042_writeLog PROTO ((unsigned short int,
                                                 signed   long  int,
                                                 unsigned long  int,
                                                 char FAR *        ));

/* Return value == 0:  Succesfully, logging record written.            */
/*               > 0:  Succesfully, no logging record must be written. */
/*               < 0:  Error.                                          */
/* Parameter1       : Level                                            */
/* Parameter2       : Type                                             */
/* Parameter3       : Length                                           */
/* Parameter4       : Record                                           */

signed short int ro0043_pack10Log PROTO ((unsigned short int,
                                          signed   long  int,
                                          unsigned short int,
                                          ...));

/* RETURN VALUES :                                                          */
/*         0 : logging record was written successfully                      */
/*        >0 : no logging record written  (level didn't fit)                */
/*        <0 : error  (session couldn't write the record)                   */
/* Parameter1       : Level                                                 */
/* Parameter2       : Type                                                  */
/* Parameter3       : Count                                                 */
/* ...              : va_alist                                              */
/*                                                                          */
/* ro0043_pack10Log (p1, p2, p3, signed short int, char FAR *, .........)   */
/*                           <i> |------  i times----------------------|    */
/*                                                                          */

signed short int  PASCAL ro0044_AppendLog PROTO ((unsigned short int,
                                                 unsigned long  int,
                                                 char FAR *        ));

/* Return value == 0:  Succesfully, logging record written.            */
/*               > 0:  Succesfully, no logging record must be written. */
/*               < 0:  Error.                                          */
/* Parameter1       : Level                                            */
/* Parameter3       : Length                                           */
/* Parameter4       : Record                                           */

signed short int  PASCAL ro0045_PduLog PROTO ((signed short int,
                                                 unsigned long  int,
                                                 unsigned long  int));

/* Return value == 0:  Succesfully, logging record written.            */
/*               > 0:  Succesfully, no logging record must be written. */
/*               < 0:  Error.                                          */
/* Parameter1       : sPmNo                                            */
/* Parameter3       : ulEvent                                          */
/* Parameter4       : hStore                                           */

signed short int  PASCAL ro0046_writeHeader PROTO ((unsigned short int,
                                                 signed   long  int,
                                                 unsigned long  int
                                                 ));

/* Return value == 0:  Succesfully, logging record written.            */
/*               > 0:  Succesfully, no logging record must be written. */
/*               < 0:  Error.                                          */
/* Parameter1       : Level                                            */
/* Parameter2       : Type                                             */
/* Parameter3       : Length                                           */

/* ****************************************************************** */
/* ********** Memory management facility **************************** */
/* ********** (routil5.c)                **************************** */
/* ****************************************************************** */
/* When the switch RC0007_CHECKMALLOC is set, the standard alloc-,    */
/* lock-,unlock-,free-,and realloc functions are not used.            */
/* Instead substitutes are used, called like the old ones,            */
/* but providing a malloc-check facility.                             */
/* The variables __FILE__ and __LINE__ refer to the modul name and    */
/* line number of the calling function.                               */
/* ****************** Global memory functions *********************** */
/* ******************                         *********************** */

#ifdef RC0007_CHECKMALLOC

unsigned long int PASCAL ro002a_globalalloc PROTO ((unsigned long int size,
						    char *, int));
#define ro0020_globalalloc(size)                      \
	ro002a_globalalloc(size, __FILE__, __LINE__)
#else

unsigned long int PASCAL ro0020_globalalloc PROTO ((unsigned long int size));

#endif

       /*******************************************************/
       /* DESCRIP:  Allocates memory dynamically              */
       /* RETURN:    ==0  : If Error                          */
       /*             >0  : Handle to allocted memory         */
       /* INPUT:    size  : Size of Area in bytes             */
       /*******************************************************/

/* ------------------------------------------------------------------ */

#ifdef RC0007_CHECKMALLOC

char FAR * PASCAL ro002b_globallock PROTO ((unsigned long int hMem,
					    char *, int));
#define ro0021_globallock(handle)                      \
	ro002b_globallock(handle, __FILE__, __LINE__)
#else

char FAR * PASCAL ro0021_globallock PROTO ((unsigned long int hMem));

#endif

       /*******************************************************/
       /* DESCRIP:  Locks an allocated Area (->Windows)       */
       /* RETURN:   NULL  : If Error                          */
       /*           else  : FAR-Pointer to the allocated area */
       /* INPUT:    hMem  : Memory Handle (->from 'alloc')    */
       /*******************************************************/

/* ------------------------------------------------------------------ */

#ifdef RC0007_CHECKMALLOC

unsigned long int PASCAL ro002c_globalunlock PROTO ((unsigned long int hMem, char *, int));
#define ro0022_globalunlock(hdl)                      \
	ro002c_globalunlock(hdl, __FILE__, __LINE__)
#else

unsigned long int PASCAL ro0022_globalunlock PROTO ((unsigned long int hMem));

#endif

       /*******************************************************/
       /* DESCRIP:  Unlocks a locked Area (->Windows)         */
       /* RETURN:    0  : If successful                       */
       /*           !0  : If Error                            */
       /* INPUT:    hMem  : Memory Handle (->from 'alloc')    */
       /*******************************************************/

/* ------------------------------------------------------------------ */

#ifdef RC0007_CHECKMALLOC

unsigned long int PASCAL ro002d_globalrealloc PROTO 
				       ((unsigned long int hMem,
					 unsigned long int oldsize,
					 unsigned long int newsize,
					 char             *filename,
					 int               lineno));
#define ro0023_globalrealloc(hdl, oldsize, newsize)    \
	ro002d_globalrealloc(hdl, oldsize, newsize, __FILE__, __LINE__)
#else

unsigned long int PASCAL ro0023_globalrealloc PROTO 
				       ((unsigned long int hMem,
					 unsigned long int oldsize,
					 unsigned long int newsize));

#endif

       /*******************************************************/
       /* DESCRIP:  Reallocs an already existing Area         */
       /* RETURN:      0  : If Error                          */
       /*             !0  : Handle to reallocated Area        */
       /* INPUT:    hMem  : Memory Handle (->from 'alloc')    */
       /*          oldsize: Old Size (used in previous alloc) */
       /*          newsize: Desired new Size                  */
       /*******************************************************/

/* ------------------------------------------------------------------ */

#ifdef RC0007_CHECKMALLOC

unsigned long int PASCAL ro002e_globalfree   PROTO ((unsigned long int hMem, char *, int));
#define ro0024_globalfree(hdl)         \
	ro002e_globalfree(hdl, __FILE__, __LINE__)
#else

unsigned long int PASCAL ro0024_globalfree   PROTO ((unsigned long int hMem));

#endif

       /*******************************************************/
       /* DESCRIP:  Reallocs an existing Area                 */
       /* RETURN:      0  : If successful                     */
       /*             !0  : If Error appeared                 */
       /* INPUT:    hMem  : Memory Handle (->from 'alloc')    */
       /*******************************************************/

/* ****************** Local memory functions ************************ */
/* ******************                        ************************ */

#ifdef RC0007_CHECKMALLOC

unsigned long int PASCAL ro002f_localalloc  PROTO ((unsigned long int hMem, char *, int));
#define ro0030_localalloc(size)      \
	ro002f_localalloc(size, __FILE__, __LINE__)
#else

unsigned long int PASCAL ro0030_localalloc  PROTO ((unsigned long int hMem));

#endif

       /*******************************************************/
       /* DESCRIP:  Allocates memory dynamically              */
       /* RETURN:    ==0  : If Error                          */
       /*             >0  : Handle to allocted memory         */
       /* INPUT:    size  : Size of Area in bytes             */
       /*******************************************************/

/* ------------------------------------------------------------------ */

#ifdef RC0007_CHECKMALLOC

char NEAR * PASCAL ro002g_locallock PROTO ((unsigned long int hMem, char *, int));
#define ro0031_locallock(hdl)   \
	ro002g_locallock(hdl, __FILE__, __LINE__)
#else

char NEAR * PASCAL ro0031_locallock PROTO ((unsigned long int hMem));

#endif

       /*******************************************************/
       /* DESCRIP:  Locks an allocated Area (->Windows)       */
       /* RETURN:   NULL  : If Error                          */
       /*           else  : NEAR-Pointer to allocated area    */
       /* INPUT:    hMem  : Memory Handle (->from 'alloc')    */
       /*******************************************************/

/* ------------------------------------------------------------------ */

#ifdef RC0007_CHECKMALLOC

unsigned long int PASCAL ro002h_localunlock PROTO ((unsigned long int hMem, char *, int));
#define ro0032_localunlock(hdl)   \
	ro002h_localunlock(hdl, __FILE__, __LINE__)
#else

unsigned long int PASCAL ro0032_localunlock PROTO ((unsigned long int hMem));

#endif

       /*******************************************************/
       /* DESCRIP:  Unlocks a locked Area (->Windows)         */
       /* RETURN:    0  : If successful                       */
       /*           !0  : If Error                            */
       /* INPUT:    hMem  : Memory Handle (->from 'alloc')    */
       /*******************************************************/

/* ------------------------------------------------------------------ */

#ifdef RC0007_CHECKMALLOC

unsigned long int PASCAL ro002i_localrealloc PROTO
				       ((unsigned long int hMem,
					 unsigned long int oldsize,
					 unsigned long int newsize,
					 char             *filename,
					 int               lineno));
#define ro0033_localrealloc(hdl, oldsize, newsize)  \
	ro002i_localrealloc(hdl, oldsize, newsize, __FILE__, __LINE__)
#else

unsigned long int PASCAL ro0033_localrealloc PROTO 
				       ((unsigned long int hMem,
					 unsigned long int oldsize,
					 unsigned long int newsize));

#endif

       /*******************************************************/
       /* DESCRIP:  Reallocs an already existing Area         */
       /* RETURN:      0  : If Error                          */
       /*             !0  : Handle to reallocated Area        */
       /* INPUT:    hMem  : Memory Handle (->from 'alloc')    */
       /*          oldsize: Old Size (used in previous alloc) */
       /*          newsize: Desired new Size                  */
       /*******************************************************/

/* ------------------------------------------------------------------ */

#ifdef RC0007_CHECKMALLOC

unsigned long int PASCAL ro002j_localfree  PROTO ((unsigned long int hMem, char *, int));
#define ro0034_localfree(hdl)   \
	ro002j_localfree(hdl, __FILE__, __LINE__)
#else

unsigned long int PASCAL ro0034_localfree  PROTO ((unsigned long int hMem));

#endif

       /*******************************************************/
       /* DESCRIP:  Reallocs an existing Area                 */
       /* RETURN:      0  : If successful                     */
       /*             !0  : If Error appeared                 */
       /* INPUT:    hMem  : Memory Handle (->from 'alloc')    */
       /*******************************************************/

/* ****************************************************************** */
/* ********** Mapped functions ************************************** */
/* ********** (routil9.c)      ************************************** */
/* ****************************************************************** */


unsigned long int PASCAL ro0040_time      PROTO ((void));

       /*******************************************************/
       /* DESCRIP:  Gets the System Time                      */
       /* RETURN:      0  : If Error                          */
       /*             !0  : System time in seconds            */
       /*******************************************************/

/* ------------------------------------------------------------------ */

void PASCAL ro0041_memcpy PROTO ((char FAR *        target,
				  char FAR *        source,
				  unsigned long int length));

       /*******************************************************/
       /* DESCRIP:  Copy a Memory Area to Memory again        */
       /* RETURN:   ---                                       */
       /* INPUT:    target: Where to copy to                  */
       /*           source: Where to copy from                */
       /*           length: Number of bytes to copy           */
       /*******************************************************/


/* ------------------------------------------------------------------ */

void PASCAL ro0056_memmove PROTO ((char FAR *        target,
				   char FAR *        source,
				   unsigned long int length));

       /*******************************************************/
       /* DESCRIP:  Copy a Memory Area to Memory again        */
       /*           taking  overlaps into consideration.      */
       /* RETURN:   ---                                       */
       /* INPUT:    target: Where to copy to                  */
       /*           source: Where to copy from                */
       /*           length: Number of bytes to copy           */
       /*******************************************************/

/* ------------------------------------------------------------------ */

unsigned long int PASCAL ro0042_memcmp PROTO ((char FAR * area1,
					       char FAR * area2,
					       unsigned long int length));

       /*******************************************************/
       /* DESCRIP:  Compares two Areas in Memory              */
       /* RETURN:      0  : If Area1 equal to Area2           */
       /*             !0  : Area1 differs from Area2          */
       /* INPUT:    area1 : Start of Area1                    */
       /*           area2 : Start of Area2                    */
       /*           length: Number of bytes to compare        */
       /*******************************************************/

/* ------------------------------------------------------------------ */

void PASCAL ro0043_memset PROTO ((char FAR * area,
				  int  mybyte,
				  unsigned long int length));

       /*******************************************************/
       /* DESCRIP:  Fills a memory area with a value          */
       /* RETURN:    ---                                      */
       /* INPUT:     area : Start of the Area                 */
       /*          mybyte : Byte to overwrite the area with   */
       /*           length: Number of bytes to fill with byte */
       /*******************************************************/

/* ------------------------------------------------------------------ */

void PASCAL ro0050_strcpy PROTO ((char FAR *        target,
				  char FAR *        source));

       /*******************************************************/
       /* DESCRIP:  Copy a String from from source to target  */
       /* RETURN:   ---                                       */
       /* INPUT:    target: Where to copy to                  */
       /*           source: Where to copy from                */
       /*******************************************************/

/* ------------------------------------------------------------------ */

void PASCAL ro0051_strcat PROTO ((char FAR *        target,
				  char FAR *        source));

       /***************************************************************/
       /* DESCRIP:  Concatenates a String from from source to target  */
       /* RETURN:   ---                                               */
       /* INPUT:    target: Where to copy to                          */
       /*           source: Where to copy from                        */
       /***************************************************************/


/* ------------------------------------------------------------------ */

unsigned int PASCAL ro0053_strlen PROTO ((char  FAR * source));

       /*******************************************************/
       /* DESCRIP:  find out the length of the string         */
       /* RETURN:   length of the string.                     */
       /* INPUT:    source: The  input string                 */
       /*******************************************************/

/* ------------------------------------------------------------------ */

signed short int PASCAL ro0057_strcmp PROTO ((
char            FAR     *pString1,           /*  -> :               */
char            FAR     *pString2));         /*  -> :               */

       /*******************************************************/
       /* DESCRIP:  Compares two strings for equality.        */
       /* RETURN:   OK      : If equal.                       */
       /*          !OK      : If unequal.                     */
       /* INPUT:    pString1: First  string to be compared    */
       /*           pString2: Second string to be compared    */
       /*******************************************************/

/* ------------------------------------------------------------------ */
signed long int PASCAL ro0058_readPduLenEnv PROTO (( void )); 

       /*******************************************************/
       /* DESCRIP:  Gets Env variable for Pdu Length          */
       /* RETURN:   Length od PDU                             */
       /* INPUT:    None                                      */
       /*******************************************************/

/* ------------------------------------------------------------------ */


/* ------------------------------------------------------------------ */

int PASCAL ro0054_atoi PROTO ((char  * source));

       /*******************************************************/
       /* DESCRIP:  converts a string to a integer            */
       /* RETURN:   integer value of a string.                */
       /* INPUT:    source: The  input string                 */
       /*******************************************************/

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */

int  PASCAL ro0055_itoa PROTO ((int num,char * source,int radix));

       /*******************************************************/
       /* DESCRIP:  converts a integer to a string            */
       /* RETURN:   String form of an integer.                */
       /* INPUT:    number: The  input integer                 */
       /*           source: The  output string                */
       /*           radix : The  base of the value.           */
       /*******************************************************/

/* ------------------------------------------------------------------ */

unsigned long int FAR PASCAL ro0060_GetCurrentTaskId PROTO ((void));

       /*******************************************************/
       /* DESCRIP : Gets the ID of the task that has currently*/
       /*           invoked the ROS DLL.                      */
       /* RETURN  : != 0    : Current Task ID.                */
       /*           == 0    : Error.                          */
       /* INPUT   : None.                                     */
       /* OUTPUT  : None.                                     */
       /*******************************************************/

/* ------------------------------------------------------------------ */

#define RC0040_FormT61			1	   /*	- String      */

#define RC0040_FormT61			1	   /*	- String      */

#define RC0040_FormT61			1	   /*	- String      */
#define RC0041_FormOCTET		2	   /*	  types       */
#define RC0042_FormUTC			3
#define RC0043_FormINTEGER		4

signed short int PASCAL ro0044_formString PROTO ((signed short int  type,
						  char FAR * target,
						  unsigned long int FAR * length,
						  unsigned long int value ));

       /*******************************************************/
       /* DESCRIP:  Encodes the value as a valid ASN1 string  */
       /*           using Basic Encoding Rules                */
       /* RETURN:      0  : If successful                     */
       /*             !0  : If Error                          */
       /* INPUT:     type : Determines which type of ASN1     */
       /*                   string shall be created           */
       /*                   (T61,OCTET,UTC or INTEGER)        */
       /*           target: Where the created ASN1-string     */
       /*                   shall be stored to                */
       /*           length: Length of reserved target area    */
       /*                   (may not be exceeded)             */
       /*            value: Value that shall be encoded       */
       /*******************************************************/

/* ------------------------------------------------------------------ */

unsigned long int PASCAL ro0045_memAndLenCmp PROTO ((char FAR * area1,
						     char FAR * area2,
						     unsigned long int len1,
						     unsigned long int len2));

       /*******************************************************/
       /* DESCRIP:  Checks whether the length as well as the  */
       /*           memory contents of two areas match        */
       /* RETURN:      0  : If both areas are equal           */
       /*             !0  : If either length are unequal      */
       /*                   or memory data are unequal        */
       /* INPUT:    area1 : Start address of area1            */
       /*           area2 : Start address of area2            */
       /*            len1 : Length of area1                   */
       /*            len2 : Length of area2                   */
       /*******************************************************/

/* ------------------------------------------------------------------ */

#define RC0040_FormT61			1	   /*	- String      */
#define RC0041_FormOCTET		2	   /*	  types       */
#define RC0042_FormUTC			3
#define RC0043_FormINTEGER		4

signed short int PASCAL ro0046_formString PROTO ((signed short int  type,
						  char FAR * target,
						  unsigned long int FAR * length,
						  unsigned long int value ));

       /*******************************************************/
       /* DESCRIP:  Encodes the value to the V-Field of the   */
       /*           corresponding ASN1 string using Basic     */
       /*           Encoding Rules.                           */
       /* RETURN:      0  : If successful                     */
       /*             !0  : If Error                          */
       /* INPUT:     type : Determines which type of ASN1     */
       /*                   string shall be created           */
       /*                   (T61,OCTET,UTC or INTEGER)        */
       /*           target: Where the created string shall    */
       /*                   be stored to                      */
       /*           length: Length of reserved target area    */
       /*                   (may not be exceeded)             */
       /*            value: Value that shall be encoded       */
       /* OUTPUT:   length: Length of transformed string      */
       /*******************************************************/

/* ------------------------------------------------------------------ */

signed short int PASCAL ro0047_formEncodedString
				      PROTO ((signed short int  type,
					      char FAR * target,
					      unsigned long int FAR * length,
					      char FAR * inpstring,
					      unsigned long int inplength));

       /*******************************************************/
       /* DESCRIP:  Encodes the value as a valid ASN1 string  */
       /*           using Basic Encoding Rules (TLV)          */
       /* RETURN:      0  : If successful                     */
       /*             !0  : If Error                          */
       /* INPUT:     type : Determines which type of ASN1     */
       /*                   string shall be created           */
       /*                   (T61,OCTET,UTC or INTEGER)        */
       /*           target: Where the created ASN1-string     */
       /*                   shall be stored to                */
       /*           length: Length of reserved target area    */
       /*                   (may not be exceeded)             */
       /*        inpstring: Input string reference            */
       /*        inplength: Length of input String            */
       /* OUTPUT:   length: Length of transformed string      */
       /*******************************************************/

/* ------------------------------------------------------------------ */

signed short int PASCAL ro0048_formDecodedString
				      PROTO ((signed short int  type,
					      char FAR * target,
					      unsigned long int FAR * length,
					      char FAR * inpstring,
					      unsigned long int inplength));

       /*******************************************************/
       /* DESCRIP:  Decodes the V-Field of a valid ASN1       */
       /*           string to the target address in unpacked  */
       /*           form.                                     */
       /* RETURN:      0  : If successful                     */
       /*             <0  : If serious error                  */
       /*             >0  : If decoding error                 */
       /* INPUT:     type : Determines which type of ASN1     */
       /*                   string shall be created           */
       /*                   (T61,OCTET,UTC or INTEGER)        */
       /*           target: Where the created string shall    */
       /*                   be stored to                      */
       /*           length: Length of reserved target area    */
       /*                   (may not be exceeded)             */
       /*        inpstring: Input string reference            */
       /*        inplength: Length of input String            */
       /* OUTPUT:   length: Length of transformed string      */
       /*******************************************************/

/* ------------------------------------------------------------------ */

char FAR * PASCAL ro0049_OctMove PROTO((char FAR *z,char FAR *zm,
					char FAR *s, unsigned long int FAR *l));

       /*******************************************************/
       /* DESCRIP:  Unpacks a packed Octet-String limited by  */
       /*           <z> and <zm> to the target <s>            */
       /*                                                     */
       /* RETURN:    NULL : If Error                          */
       /*             !0  : If ok                             */
       /* INPUT:       z  : Start of optionally packed        */
       /*                   ASN1-Octet-String                 */
       /*              zm : End of optionally packed          */
       /*                   ASN1-Octet-String                 */
       /*              s  : Target where to store to          */
       /*              l  : Length of packed string           */
       /* OUTPUT:      l  : Length of unpacked string ->s     */
       /*******************************************************/

/* ------------------------------------------------------------------ */


/* ****************************************************************** */
/*                                                                    */
/*                    ROUTIL8.C functions                             */
/*                                                                    */
/* Interface to the File functions                                    */
/* fopen, fclose, fwrite, fread, fseek, ftell, fflush                 */
/*                                                                    */
/* ****************************************************************** */

/* ***************** A T T E N T I O N ****************************** */
/*                                                                    */
/* All functions have the same input parameter as the equivalent      */
/* C-functions, exept for the FILE pointer. The util functions use    */
/* instead of the FILE pointer a handle, which is here only a casted  */
/* FILE pointer.                                                      */
/*                                                                    */
/* The Returncode is mapped in some functions to a local error code   */
/* and therefore not equal to the error code of the original C-fuction*/
/*                                                                    */
/* ****************************************************************** */

/* ****************************************************************** */
/*                                                                    */
/*                  D E F I N E S                                     */
/*                                                                    */
/* for mapping defines in stdio.h to local defines.                   */
/*                                                                    */
/* ****************************************************************** */

#define RC0080_SEEK_SET    1
#define RC0081_SEEK_CUR    2
#define RC0082_SEEK_END    3
#define RC0083_EOF        (-1)


/* ****************** fopen  **************************************** */

unsigned long int  PASCAL ro00f1_fopen  PROTO ((char  FAR  *   fname,
						char  FAR  *   fmode));
/*
   Return value   : Same as fopen(); FILE pointer casted to a handle.
   Inputparameter : Same as fopen();
*/

/* ****************** fclose  **************************************** */

signed short int  PASCAL ro00f2_fclose PROTO ((unsigned long int fp));
/*
   Return value   : == 0         : OK.
		    == -R_FCLERR : Error
		       local error mapping
   Inputparameter : Same as fclose(); FILE pointer casted to a handle.
*/

/* ****************** fwrite  **************************************** */

signed short int  PASCAL ro00f3_fwrite PROTO ((
				       void     FAR       *  ptr,
				       unsigned long int     size,
				       unsigned long int     nnumb,
				       unsigned long int     fp));
/*
   Return value   : == 0         : OK.
		    == -R_FWRERR : Error
		       local error mapping, if (size * nnumb) !=
		       returnvalue of fwrite;
   Inputparameter : Same as fwrite(); FILE pointer casted to a handle.
*/

/* ****************** fread  **************************************** */

signed short int  PASCAL ro00f4_freadAndLenChk PROTO ((
				               void        FAR    *  ptr,
	        			       unsigned long int     size,
	        			       unsigned long int     nnumb,
	        			       unsigned long int     fp));
/*
   Return value   : == 0         : OK.
		    == -R_FREERR : Error
		       local error mapping
		       local error mapping, if (size * nnumb) !=
		       returnvalue of fread;
   Inputparameter : Same as fread(); FILE pointer casted to a handle.
*/


/* ****************** fseek   **************************************** */

signed short int  PASCAL ro00f5_fseek  PROTO ((unsigned long int fp,
					       long int          offset,
					       int               ptrname));
/*
   Return value   : == 0         : OK.
		    == -R_FSEERR : Error
		       local error mapping
   Inputparameter : Same as fseek();
		    for ptrname use only the local defines
		    RC0080_SEEK_SET, RC0081_SEEK_CUR and RC0082_SEEK_END,
		    otherwise the result is undefined;
*/


/* ****************** fflush  **************************************** */

signed short int  PASCAL ro00f6_fflush PROTO ((unsigned long int fp));
/*
   Return value   : == 0         : OK.
		    == -R_FFLERR : Error
		       local error mapping
   Inputparameter : Same as fflush();
*/

/* ****************** ftell  **************************************** */

long int  PASCAL ro00f7_ftell  PROTO ((unsigned long int fp));
/*
   Return value   : Same as ftell();
   Inputparameter : Same as ftell();
*/

/* ****************** tmpfile  ************************************** */

unsigned long int  PASCAL ro00f8_tmpfile  PROTO (( void ));
/*
   Return value   : Same as fopen(); FILE pointer casted to a handle.
   Inputparameter : None. 
*/

/* ****************** feof  ************************************** */
signed short int PASCAL ro00fk_feof PROTO ((
     unsigned long int ));
/* ****************** fgets  ************************************** */
signed short int PASCAL fo00fl_fgets PROTO ((
     char        FAR * ,
     signed int        ,
     unsigned long int ));
/* ****************** flock  **************************************** */

signed short int PASCAL ro00fa_flock PROTO ((unsigned long int  hfp));
/*
   Return value   : Error code  ( 0 = ok).
   Inputparameter : Handle to file.
*/

/* ****************** funlock  **************************************** */

signed short int PASCAL ro00fb_funlock PROTO (( unsigned long int  hfp));
/*
   Return value   : Error code  ( 0 = ok).
   Inputparameter : Handle to file.
*/
/* ****************** tmpnam  **************************************** */

signed short int  PASCAL ro00fc_tmpnam PROTO ((char FAR * name));
/*
   Return value   : == 0         : OK.
		    == -R_FNMERR : Error
		       local error mapping
   Inputparameter : A char array of L_tmpnam bytes (defined as 25)
*/
/* ****************** remove  **************************************** */

signed short int  PASCAL ro00fd_remove PROTO ((char FAR * name));
/*
   Return value   : == 0         : OK.
		    == -R_FRMERR : Error
		       local error mapping
   Inputparameter : A char array of L_tmpnam bytes (defined as 25)
*/

/* ****************** openTmpFile *********************************** */

unsigned long int  PASCAL ro00ff_openTmpFile PROTO ((
signed short int   FAR  * pNamed,
signed       char  FAR  * pFileName));
/*
   Return value   : == Handle to tempfile that was opened.
   Outputparameter: pFileName   : Pointer to filename where name is to
                                  be copied.
   Outputparameter: pNamed      : Pointer to boolean where info is to be copied
                                  whether a physical file was created or not.
*/

/* ****************** fread  **************************************** */

unsigned long int  PASCAL ro00fe_fread PROTO ((
				      void        FAR    *  ptr,
	        		      unsigned long int     size,
	        		      unsigned long int     nnumb,
	        		      unsigned long int     fp));
/*
   Return value   : == number of bytes actually read.
   Inputparameter : Same as fread(); FILE pointer casted to a handle.
*/


/* ****************************************************************** */
/*                                                                    */
/*    E X T E N D E D      F I L E     I / O    I N T E R F A C E     */
/*                                                                    */
/* The following is an interface that provides the normal file I/O    */
/* interface + allows the implementor of the interface to provide     */
/* additional internal info back and forth the interface for any      */
/* additional processing to be done on the files.                     */
/*                                                                    */
/* For e.g.: Under Windows, when a file is opened, a structure is     */
/*           returned that can be reused after a file close to        */
/*           reopen the file with the same file-pointer position etc. */
/*           by passing this structure back.                          */
/*                                                                    */
/* ****************************************************************** */


/* ****************** Shfopen *************************************** */


signed short int PASCAL ro00ff_Shfopen PROTO ((
char                     FAR * lpszFileName,
char                     FAR * fuMode,
unsigned long int        FAR * phFileHandle,
void              FAR *  FAR * lpFileInfo));


signed short int PASCAL ro00fg_Shfread PROTO ((
void              FAR  * lpBuffer,
unsigned long int        ulSize,
unsigned long int        ulNumb,
unsigned long int        hFileHandle,
void              FAR  * lpFileInfo,
unsigned long int FAR  * pulOutNumb));


signed short int  PASCAL ro00fi_Shfwrite PROTO ((
void              FAR * lpBuffer,
unsigned long int       ulSize,
unsigned long int       ulNumb,
unsigned long int       hFileHandle,
void              FAR * lpFileInfo));


signed short int  PASCAL ro00fj_Shfclose PROTO ((
unsigned long  int        hFileHandle,
void                FAR * lpFileInfo,
signed   short int  FAR * pbCloseAll));


/* ****************************************************************** */
/*                                                                    */
/*   Other Logging related calls.                                     */
/*                                                                    */
/* ****************************************************************** */

signed short int PASCAL ro004a_ReadConf      PROTO((void));

signed short int PASCAL ro004d_RefreshLog    PROTO((                 
                                      unsigned short int  loglevel,  
                                      unsigned short int    recnum,  
                                      char FAR              *fname,  
                                      unsigned short int     fmode));

#endif	/* _ROUTIL0_H */
