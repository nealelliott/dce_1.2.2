/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: routil4.c,v $
 * Revision 1.1.4.2  1996/03/11  13:20:15  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:17  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:32:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:18:33  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:08:08  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:50:04  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:59:50  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:55:35  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:34:39  keutel
 * 	creation
 * 	[1994/03/21  13:25:16  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: routil4.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:20:15 $" } ;
#endif

/**********************************************************************\
*								       *
*  * Copyright (c) 1992 by					       *
*  * SIEMENS NIXDORF INFORMATIONSSYSTEME AG			       *
*  * All rights reserved					       *
*  *								       *
*  * This software is furnished under licence and may be used only in  *
*  * accordance with the terms of that licence and with the inclusion  *
*  * of the above copyright notice. This software may not be provided  *
*  * or otherwise made available to, or used by, any other person.     *
*  * No title to or ownership of the software is hereby transferred.   *	  *
*								       *
\**********************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODULE                                              */
/*                                                                    */
/* NAME         : routil4.c                                           */
/*                                                                    */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : MAR-8th-1992                                        */
/*                                                                    */
/* COMPONENT    : RTS-V2.0 Malloc Controlling Unit                    */
/*                                                                    */
/* DESCRIPTION  : Checks allocated area for:                          */
/*                  - Overwriting                                     */
/*                  - Global/Local mismatch                           */
/*                  - freeing only unlocked areas                     */
/*                    (fills freed area with 0-bytes)                 */
/*                  - non matching sizes during realloc               */
/*                  - Handle mismatch (by providing real handles)     */
/*                Writes two malloc-control-files:                    */
/*                  1) mallfile.bin   (binary/use mallshow to read)   */
/*                  2) mallfile.pcl   (ascii-protocol)                */
/*                Protocolling can be avoided by setting an           */
/*                environment variable like:  MALLPROTOCOL=OFF        */
/*                                                                    */
/* SYSTEM DEPENDENCE :  SINIX V5.4                                    */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  |mark| CR# EM# */
/* 1.00 K0  | 92-03-08|  original                      | gj |         */
/* 1.01 K0  | 92-03-12|  new: protocol-file logging    | gj |         */
/* 1.02 K0  | 92-03-14|  new: continue facility        | gj |         */
/* 1.03 K0  | 92-03-15|  new: hints                    | gj |         */
/* 1.04 K0  | 92-05-23|  changed: freed area will now  | gj |         */
/*          |         |           be filled with NULL  |    |         */
/* 1.05 K0  | 92-06-26|  new: protocol can be switched | gj |         */
/*          |         |  off by environment varibale   |    |         */
/*          |         |    MALLPROTOCOL=OFF            |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*              Module Identification                                 */
/**********************************************************************/

static char *sccsid =    "@(#) routil4.c 1.1 93/11/29   RTS-V2.00";

#ifdef RC0007_CHECKMALLOC            /* this module is used only when */
			  /* the RTS-malloc-routines shall be checked */
/**********************************************************************/
/*                      I N C L U D E S                               */
/**********************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <rogenrl.h>
#include <routil0.h>
#include <routil4.h>

/**********************************************************************/
/*              G L O B A L   V A R I A B L E S                       */
/**********************************************************************/
 
static MallInfo mall   [MAXMALL];      /* Info table for malloc calls */
static MallInfo mallold[MAXMALL];        /* Copy of Malloc Table used */
				    /* to check for undefined changes */
				    /* within the malloc-table itself */
                                         /* Default abort reason info */
static AbortInfo Abort = {
		    "  >>> M E M O R Y - E R R O R !!! <<<",
		    "No Error detected",0,"",99L    };

static FILE* mallfptr     = NULL;            /* Malloc/Free Info file */
static FILE* mallprotocol = NULL;             /* Malloc-Protocol file */

static unsigned long HdlCtr = HDLOFFSET;/* incremented by each malloc */
					    /* represents the MallHdl */
			     /* offset is choosen to avoid a 0-Handle */

				     /* default is: check the mallocs */
static int IgnoreMallocErrors = M_NO;

static int MallProtocol = M_YES;    /* default: write a protocol file */
static int MallAbort    = M_NO;    /* default: abort to stdout */

/**********************************************************************************/
/*                                                                                */
/*              P R O V I D E D   G L O B A L   F U N C T I O N S                 */
/*										  */
/* unsigned long int PASCAL ro002a_globalalloc	 ((unsigned long int size,	  */
/*					           char *filename, int lineno));  */
/* char FAR *	     PASCAL ro002b_globallock	 ((unsigned long int hMem,	  */
/*						   char *filename, int lineno));  */
/* unsigned long int PASCAL ro002c_globalunlock  ((unsigned long int hMem,	  */
/*						   char *filename, int lineno));  */
/* unsigned long int PASCAL ro002d_globalrealloc ((unsigned long int hMem,	  */
/*						   unsigned long int oldsize,	  */
/*						   unsigned long int newsize,	  */
/*						   char *filename, int lineno));  */
/* unsigned long int PASCAL ro002e_globalfree	 ((unsigned long int hMem,	  */
/*						   char *filename, int lineno));  */
/* unsigned long int PASCAL ro002f_localalloc	 ((unsigned long int hMem,	  */
/*						   char *filename, int lineno));  */
/* char NEAR *	     PASCAL ro002g_locallock	 ((unsigned long int hMem,	  */
/*						   char *filename, int lineno));  */
/* unsigned long int PASCAL ro002h_localunlock	 ((unsigned long int hMem,	  */
/*						   char *filename, int lineno));  */
/* unsigned long int PASCAL ro002i_localrealloc  ((unsigned long int hMem,	  */
/*						   unsigned long int oldsize,	  */
/*						   unsigned long int newsize,	  */
/*						   char *filename, int lineno));  */
/* unsigned long int PASCAL ro002j_localfree	 ((unsigned long int hMem,	  */
/*						   char *filename, int lineno));  */
/*										  */
/**********************************************************************************/

/**********************************************************************************/
/*              P R O V I D E D    L O C A L    F U N C T I O N S                 */
/**********************************************************************************/

 static void PASCAL ro002k_OpenMallFiles  PROTO ((void));
 static void PASCAL ro002l_CloseMallFiles PROTO ((void));
 static void PASCAL ro002m_MallErrReport  PROTO ((char * message,
						  char * filename,
						  unsigned int lineno,
						  unsigned long int hdl,
						  char * hint,
						  unsigned int diagnostic));
 static void PASCAL ro002n_CheckMallTable PROTO ((void));
 static void PASCAL ro002o_WriteMallTable PROTO ((void));
 static void PASCAL ro002p_IgnoreMallErr  PROTO ((void));


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro002a_globalalloc                                  */
/*                                                                    */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : Mar-6th-1992                                        */
/*                                                                    */
/* SYNTAX       :                                                     */

unsigned long int PASCAL ro002a_globalalloc ( 
  unsigned long int size,
  char             *filename,
  int               lineno)

/*                                                                    */
/* DESCRIPTION    :  Allocates memory area of size <size>.            */
/*                                                                    */
/* IN-PARAMETERS  :                                                   */
/*                                                                    */
/*      size      :  Size of memory to be allocated (>0).             */
/*      filename  :  Module name of calling function.                 */
/*      lineno    :  Line number of calling function.                 */
/*                                                                    */
/* OUT-PARAMETERS :  none                                             */
/*                                                                    */
/* RETURN VALUES  :                                                   */
/*                                                                    */
/*        >0  :  Handle of the memory area.                           */
/*         0  :  if an error occurs.                                  */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS:   Used for Malloc-Checking                                */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr-ro002a_globalalloc*/
  char FAR *ptr;
  int i;
  unsigned long int l;
/* **************** S T A R T   O F   F U N C T I O N *************** */

  if (size < 0)                    /* a size of this is not supported */
      ro002m_MallErrReport ("GlobalALLOC with size of < 0! ",
			     filename, lineno, 0L,
	     "It might be caused by a signed/unsigned-type mismatch",
	     M_NEGLIG );

  ro002n_CheckMallTable();       /* before new allocation takes place */
				     /* check the existing ones first */
				  /* allocate an area with some check */
					    /* bytes ahead and behind */
  ptr = (char * FAR) malloc (size + 2*CHECKSIZE);
  if (ptr == NULL)
      ro002m_MallErrReport ("Malloc returned NULL address in GlobalALLOC!",
			     filename, lineno, 0L,
			"Provide more system-memory",M_FATAL);

  ro0043_memset (ptr, 0,size + 2*CHECKSIZE);    
					 /* initializing with 0-bytes */
			      /* now update the Malloc-Table and File */
					   /* search for a free entry */
  for (i=0; (i<MAXMALL) && (mall[i].MallHdl != 0); i++);

  if (i==MAXMALL)            /* number of possible mallocs is limited */
      ro002m_MallErrReport ("Malloc-Table Overflow in GlobalALLOC!",
			     filename, lineno, 0L,
		  "Either expand the table or free unused areas",M_NEGLIG);

					      /* now fill a new entry */
  mall[i].MallHdl = HdlCtr;
  mall[i].LockCounter = 0;
  mall[i].Type = GLOBALTYPE;
  mall[i].address = ptr;
  mall[i].size = size;
  mall[i].line = lineno;
  l = strlen (filename);
  if (l>(MODULNAMLEN-1))
      l=MODULNAMLEN-1;
  ro0043_memset (mall[i].modul, 0, MODULNAMLEN);
  ro0041_memcpy (mall[i].modul,filename,l+1);

  for (i=0; i<CHECKSIZE; i++)  /* set the checkbytes to 1010101010... */
      {
       *(ptr+i) = CHECKBYTE;
       *(ptr+i+size+CHECKSIZE) = CHECKBYTE;
      }

  ro002o_WriteMallTable();                 /* Flush the Table to file */


  if (MallProtocol == M_YES)      /* Flush Entry to the protocol file */
     {
      fprintf(mallprotocol,
	  "Galloc of %ld bytes with handle:%ld in %s line %d\n",
	   size,HdlCtr,filename,lineno);
     }

  if (HdlCtr == MAXMALLHDL)               /* abort if Handle-Overflow */
      ro002m_MallErrReport ("Malloc-Handle-Overflow!",
			     filename, lineno, HdlCtr,
	    "Try to avoid so many allocations or expand the limit",M_NEGLIG);
  else
      HdlCtr++;        /* the next alloc shall get a different handle */

  return ( (unsigned long int) ( HdlCtr-1) );        /* a real handle */

/* **************** E N D   O F   F U N C T I O N ******************* */
}/*end-ro002a_globalalloc*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro002b_globallock                                   */
/*                                                                    */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : MAR-6th-1992                                        */
/*                                                                    */
/* SYNTAX       :                                                     */

char FAR * PASCAL ro002b_globallock ( 
  unsigned long int hMem,
  char             *filename,
  int               lineno)

/*                                                                    */
/* DESCRIPTION  :  Lock of a memory area                              */
/*                                                                    */
/* IN-PARAMETERS  :                                                   */
/*                                                                    */
/*      hMem      :  Handle of the memory area                        */
/*      filename  :  Module name of calling function.                 */
/*      lineno    :  Line number of calling function.                 */
/*                                                                    */
/* OUT-PARAMETERS :    none                                           */
/*                                                                    */
/* RETURN VALUES  :                                                   */
/*                                                                    */
/*        >0   :  Pointer to the allocated memory area.               */
/*        NULL :  if an error occurs.                                 */
/*                                                                    */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS :  none                                                    */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr-ro002b_globallock*/
  int i;
/* **************** S T A R T   O F   F U N C T I O N *************** */

				      /* first check the input-handle */
  if (hMem < HDLOFFSET || hMem >= HdlCtr)
      ro002m_MallErrReport ("Invalid input handle in GlobalLOCK!",
			     filename, lineno, hMem,
		  "Look where you got this handle from",M_FATAL);

  ro002n_CheckMallTable();    /* check the Table for range violations */

			  /* check whether the incoming handle exists */
  for (i=0; (i<MAXMALL) && (mall[i].MallHdl != hMem); i++);
  if (i==MAXMALL)    /* an entry matching to the handle doesn't exist */
      ro002m_MallErrReport ("Handle not found in Malloc-Table in GlobalLOCK!",
			     filename, lineno, hMem,
		  "Look where you got this handle from",M_FATAL);

  if (mall[i].Type != GLOBALTYPE)
      ro002m_MallErrReport ("Global/Local mismatch in GlobalLOCK!",
			     filename, lineno, mall[i].MallHdl,
	       "Better try <locallock> in this case!",M_NEGLIG);

  mall[i].LockCounter++;

  ro002o_WriteMallTable();                 /* Flush the Table to file */

  if (MallProtocol == M_YES)      /* Flush Entry to the protocol file */
     {
      fprintf(mallprotocol,
	  "Glock with handle:%ld at address:%#10lx in %s line %d \n",
	   hMem,(unsigned long int)mall[i].address,filename,lineno);
     }

  return ( (char FAR *) mall[i].address + CHECKSIZE );

/* **************** E N D   O F   F U N C T I O N ******************* */
}/*end-ro002b_globallock*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro002c_globalunlock                                 */
/*                                                                    */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : MAR-6th-1992                                        */
/*                                                                    */
/* SYNTAX       :                                                     */

unsigned long int PASCAL ro002c_globalunlock (
  unsigned long int  hMem,
  char             *filename,
  int               lineno)

/*                                                                    */
/* DESCRIPTION  :  Unlocks the memory block                           */
/*                                                                    */
/* IN-PARAMETERS  :                                                   */
/*      hMem      :  Handle of the memory area                        */
/*      filename  :  Module name of calling function.                 */
/*      lineno    :  Line number of calling function.                 */
/*                                                                    */
/* OUT-PARAMETERS :  none                                             */
/*                                                                    */
/* RETURN VALUES  :                                                   */
/*                                                                    */
/*         0  :  if successful                                        */
/*       <>0  :  error  (invalid memory-handle)                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS :   none                                                   */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr-ro002c_globalunlock*/
  int i;
/* **************** S T A R T   O F   F U N C T I O N *************** */

				      /* first check the input-handle */
  if (hMem < HDLOFFSET || hMem >= HdlCtr)
      ro002m_MallErrReport ("Invalid input handle in GlobalUNLOCK!",
			     filename, lineno, hMem,
		  "Look where you got this handle from",M_FATAL);

  ro002n_CheckMallTable();    /* check the table for range violations */

  for (i=0; (i<MAXMALL) && (mall[i].MallHdl != hMem); i++);
  if (i==MAXMALL)    /* an entry matching to the handle doesn't exist */
      ro002m_MallErrReport ("Handle not found in Malloc-Table in GlobalUNLOCK!",
			     filename, lineno, hMem,
		  "Look where you got this handle from",M_NEGLIG);

  if (mall[i].Type != GLOBALTYPE)
      ro002m_MallErrReport ("Global/Local mismatch in GlobalUNLOCK!",
			     filename, lineno, mall[i].MallHdl,
	       "Better try <localunlock> in this case!",M_NEGLIG);

  if (mall[i].LockCounter > 0) /* Windows allows underflow of UNLOCKS */
      mall[i].LockCounter--;

  ro002o_WriteMallTable();                 /* Flush the Table to file */

  if (MallProtocol == M_YES)      /* Flush Entry to the protocol file */
     {
      fprintf(mallprotocol,"Gunlock with handle:%ld in %s line %d \n",
	  hMem,filename,lineno);
     }

  return (0);                                        /* everything ok */

/* **************** E N D   O F   F U N C T I O N ******************* */
}/*end-ro002c_globalunlock*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro002d_globalrealloc                                */
/*                                                                    */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : MAR-6th-1992                                        */
/*                                                                    */
/* SYNTAX       :                                                     */

unsigned long int PASCAL ro002d_globalrealloc (
  unsigned long int hMem,
  unsigned long int oldsize,
  unsigned long int newsize,
  char             *filename,
  int               lineno)

/*                                                                    */
/* DESCRIPTION  :  Reallocates the memory block by increasing         */
/*                 or decreasing its size. If the <newsize> is larger */
/*                 than the <oldsize> was, the extended area will be  */
/*                 filled with zero-bytes.                            */
/*                                                                    */
/* IN-PARAMETERS  :                                                   */
/*                                                                    */
/*      oldsize   :  Old size of the memory block (>0).               */
/*      newsize   :  New size of the memory block (>0).               */
/*      hMem      :  Handle of already allocated memory block.        */
/*      filename  :  Module name of calling function.                 */
/*      lineno    :  Line number of calling function.                 */
/*                                                                    */
/* OUT-PARAMETERS :  none                                             */
/*                                                                    */
/* RETURN VALUES  :                                                   */
/*                                                                    */
/*        >0  :  Handle of the memory block.                          */
/*         0  :  if an error occurs.                                  */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :                                       */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr-ro002d_globalrealloc*/
  char FAR* mptr;
  int i,j;
  unsigned long int l;
/* **************** S T A R T   O F   F U N C T I O N *************** */

				      /* first check the input-handle */
  if (hMem < HDLOFFSET || hMem >= HdlCtr)
      ro002m_MallErrReport ("Invalid input handle in GlobalREALLOC!",
			     filename, lineno, hMem,
		  "Look where you got this handle from",M_FATAL);

  ro002n_CheckMallTable();    /* check the table for range violations */

  for (i=0; (i<MAXMALL) && (mall[i].MallHdl != hMem); i++);
  if (i==MAXMALL)    /* an entry matching to the handle doesn't exist */
      ro002m_MallErrReport ("Handle not found in Malloc-Table in GlobalREALLOC!",
			     filename, lineno, hMem,
		  "Look where you got this handle from",M_FATAL);

  if (mall[i].Type != GLOBALTYPE)
      ro002m_MallErrReport ("Global/Local mismatch in GlobalREALLOC!",
			     filename, lineno, mall[i].MallHdl,
	       "Better try <localrealloc> in this case!",M_NEGLIG);

  if (mall[i].LockCounter > 0)      /* Reallocation only allowed when */
					     /* no locks are existing */
      ro002m_MallErrReport ("LockCounter not 0 in GlobalREALLOC!",
			     filename, lineno, mall[i].MallHdl,
	   "Always unlock the area before calling <realloc>!",M_NEGLIG);

  if (mall[i].size != oldsize)
      ro002m_MallErrReport ("Mismatch of oldsize GlobalREALLOC!",
			     filename, lineno, mall[i].MallHdl,
     "You gave the wrong oldsize-input, maybe the wrong variable?",M_NEGLIG); 

  if (mall[i].address == NULL)
      ro002m_MallErrReport ("Address is 0 in GlobalREALLOC!",
			     filename, lineno, mall[i].MallHdl,
       "There might have been some overwriting since allocation",M_FATAL);

  mptr = (char FAR*) realloc ( mall[i].address, newsize+2*CHECKSIZE );
  if (mptr == NULL)
      ro002m_MallErrReport ("Realloc returned address 0 in GlobalREALLOC!",
			     filename, lineno, mall[i].MallHdl,
	   "Provide more system-memory and check input parameter",M_FATAL);

  if (newsize > oldsize)
      ro0043_memset (mptr+CHECKSIZE+oldsize, 0, newsize-oldsize);

					      /* now update the entry */
  mall[i].MallHdl = HdlCtr;
  mall[i].LockCounter = 0;
  mall[i].Type = GLOBALTYPE;
  mall[i].address = mptr;
  mall[i].size = newsize;
  mall[i].line = lineno;
  l = ro0053_strlen ((char FAR *)filename);
  if (l>(MODULNAMLEN-1))
      l=MODULNAMLEN-1;
  ro0043_memset (mall[i].modul, 0, MODULNAMLEN);
  ro0041_memcpy (mall[i].modul,filename,l+1);

  for (j=0; j<CHECKSIZE; j++)  /* set the checkbytes to 1010101010... */
      {
       *(mptr+j) = CHECKBYTE;
       *(mptr+j+newsize+CHECKSIZE) = CHECKBYTE;
      }
  ro002o_WriteMallTable();                 /* Flush the Table to file */

  if (MallProtocol == M_YES)      /* Flush Entry to the protocol file */
     {
      fprintf(mallprotocol,
  "Grealloc with hdl:%ld newhdl:%ld oldsize:%ld newsize:%ld in %s line %d \n",
	  hMem,HdlCtr,oldsize,newsize,filename,lineno);
     }

  if (HdlCtr == MAXMALLHDL)               /* abort if Handle-Overflow */
      ro002m_MallErrReport ("Malloc-Handle-Overflow!",
			     filename, lineno, HdlCtr,
      "Try to avoid so many allocations or expand the limit",M_NEGLIG);
  else
      HdlCtr++;        /* the next alloc shall get a different handle */
  return ( (unsigned long int) ( HdlCtr-1) );        /* a real handle */

/* **************** E N D   O F   F U N C T I O N ******************* */
}/*end-ro002d_globalrealloc*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro002e_globalfree                                   */
/*                                                                    */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : MAR-6th-1992                                        */
/*                                                                    */
/* SYNTAX       :                                                     */

unsigned long int PASCAL ro002e_globalfree ( 
  unsigned long int hMem,
  char             *filename,
  int               lineno)

/*                                                                    */
/* DESCRIPTION    :  Frees the allocated memory block                 */
/*                   connected to <hMem>.                             */
/*                                                                    */
/* IN-PARAMETERS  :                                                   */
/*                                                                    */
/*      hMem      :  Handle to the memory block to be free            */
/*      filename  :  Module name of calling function.                 */
/*      lineno    :  Line number of calling function.                 */
/*                                                                    */
/* OUT-PARAMETERS :              none                                 */
/*                                                                    */
/* RETURN VALUES  :                                                   */
/*                                                                    */
/*         0  : if successful                                         */
/*        !0  : if error  (invalid memory-handle)                     */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS : none                                                     */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr-ro002e_globalfree*/
  int i;
/* **************** S T A R T   O F   F U N C T I O N *************** */

				      /* first check the input-handle */
  if (hMem < HDLOFFSET || hMem >= HdlCtr)
      ro002m_MallErrReport ("Invalid input handle in GlobalFREE!",
			     filename, lineno, hMem,
		  "Look where you got this handle from",M_FATAL);

  ro002n_CheckMallTable();    /* check the table for range violations */

  for (i=0; (i<MAXMALL) && (mall[i].MallHdl != hMem); i++);
  if (i==MAXMALL)    /* an entry matching to the handle doesn't exist */
      ro002m_MallErrReport ("Handle not found in Malloc-Table in GlobalFREE!",
			     filename, lineno, hMem,
		  "Look where you got this handle from",M_FATAL);

  if (mall[i].Type != GLOBALTYPE)
      ro002m_MallErrReport ("Global/Local mismatch in GlobalFREE!",
			     filename, lineno, mall[i].MallHdl,
	       "Better try <localfree> in this case!",M_NEGLIG);

  if (mall[i].LockCounter != 0)
      ro002m_MallErrReport ("LockCounter not 0 in GlobalFREE!",
			     filename, lineno, mall[i].MallHdl,
	   "Always unlock the area before calling <free>!",M_NEGLIG);

  if (mall[i].address == NULL)
      ro002m_MallErrReport ("Address is NULL in GlobalFREE!",
			     filename, lineno, mall[i].MallHdl,
       "There might have been some overwriting since allocation",M_FATAL);

		/* fill the occupied area with 0-bytes before release */
  ro0043_memset (mall[i].address, 0,mall[i].size + 2*CHECKSIZE);
		  /* seems to be a good handle so execute the freeing */
  free(mall[i].address);
						    /* free the entry */
  mall[i].MallHdl = 0L;
  mall[i].LockCounter = 0;
  mall[i].Type = NOTYPE;
  mall[i].address = (char FAR *) 0;
  mall[i].size = 0L;
  mall[i].line = 0;
  ro0043_memset (mall[i].modul, 0, MODULNAMLEN);

  ro002o_WriteMallTable();                 /* Flush the Table to file */

  if (MallProtocol == M_YES)      /* Flush Entry to the protocol file */
     {
      fprintf(mallprotocol,"Gfree with handle:%ld in %s line %d \n",
	  hMem,filename,lineno);
     }

  return ( (unsigned long int) 0 );                       /* means ok */

/* **************** E N D   O F   F U N C T I O N ******************* */
}/*end-ro002e_globalfree*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro002f_localalloc                                   */
/*                                                                    */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : Mar-6th-1992                                        */
/*                                                                    */
/* SYNTAX       :                                                     */

unsigned long int PASCAL ro002f_localalloc (
  unsigned long int size,
  char             *filename,
  int               lineno)

/*                                                                    */
/* DESCRIPTION    :  Allocates memory area of size <size>.            */
/*                                                                    */
/* IN-PARAMETERS  :                                                   */
/*                                                                    */
/*      size      :  Size of memory to be allocated (>0).             */
/*      filename  :  Module name of calling function.                 */
/*      lineno    :  Line number of calling function.                 */
/*                                                                    */
/* OUT-PARAMETERS :  none                                             */
/*                                                                    */
/* RETURN VALUES  :                                                   */
/*                                                                    */
/*        >0  :  Handle of the memory area.                           */
/*         0  :  if an error occurs.                                  */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS:   Used for Malloc-Checking                                */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr-ro002f_localalloc*/
  char NEAR *ptr;
  int i,l;
/* **************** S T A R T   O F   F U N C T I O N *************** */

  if (size < 0)                    /* a size of this is not supported */
      ro002m_MallErrReport ("LocalALLOC with size of < 0! ",
			     filename, lineno, 0L,
       "It might be caused by a signed/unsigned-type mismatch",M_FATAL);

  ro002n_CheckMallTable();         /* before new allocation check the */
					       /* existing ones first */
				  /* allocate an area with some check */
					    /* bytes ahead and behind */
  ptr = (char * NEAR) malloc (size + 2*CHECKSIZE);
  if (ptr == NULL)
      ro002m_MallErrReport ("Malloc returned NULL address in LocalALLOC!",
			     filename, lineno, 0L,
			"Provide more system-memory",M_FATAL);

					 /* initializing with 0-bytes */
  ro0043_memset (ptr, 0,size + 2*CHECKSIZE);  
			      /* now update the Malloc-Table and File */
			      /* search for a free entry */
  for (i=0; (i<MAXMALL) && (mall[i].MallHdl != 0); i++);

  if (i==MAXMALL)            /* number of possible mallocs is limited */
      ro002m_MallErrReport ("Malloc-Table Overflow in LocalALLOC!",
			     filename, lineno, 0L,
		  "Either expand the table or free unused areas",M_NEGLIG);
					      /* now fill a new entry */
  mall[i].MallHdl = HdlCtr;
  mall[i].LockCounter = 0;
  mall[i].Type = LOCALTYPE;
  mall[i].address = ptr;
  mall[i].size = size;
  mall[i].line = lineno;
  l = ro0053_strlen ((char FAR *)filename);
  if (l>(MODULNAMLEN-1))
      l=MODULNAMLEN-1;
  ro0043_memset (mall[i].modul, 0, MODULNAMLEN);
  ro0041_memcpy (mall[i].modul,filename,l+1);

  for (i=0; i<CHECKSIZE; i++)  /* set the checkbytes to 1010101010... */
      {
       *(ptr+i) = CHECKBYTE;
       *(ptr+i+size+CHECKSIZE) = CHECKBYTE;
      }

  ro002o_WriteMallTable();                 /* Flush the Table to file */

  if (MallProtocol == M_YES)      /* Flush Entry to the protocol file */
     {
      fprintf(mallprotocol,
	  "Lalloc of %ld bytes with handle:%ld in %s line %d\n",
	   size,HdlCtr,filename,lineno);
     }

  if (HdlCtr == MAXMALLHDL)               /* abort if Handle-Overflow */
      ro002m_MallErrReport ("Malloc-Handle-Overflow!",
			     filename, lineno, HdlCtr,
      "Try to avoid so many allocations or expand the limit",M_NEGLIG);
  else
      HdlCtr++;        /* the next alloc shall get a different handle */

  return ( (unsigned long int) ( HdlCtr-1) );        /* a real handle */

/* **************** E N D   O F   F U N C T I O N ******************* */
}/*end-ro002f_localalloc*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro002g_locallock                                    */
/*                                                                    */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : MAR-6th-1992                                        */
/*                                                                    */
/* SYNTAX       :                                                     */

char NEAR * PASCAL ro002g_locallock ( 
  unsigned long int hMem,
  char             *filename,
  int               lineno)

/*                                                                    */
/* DESCRIPTION  :  Lock of a memory area                              */
/*                                                                    */
/* IN-PARAMETERS  :                                                   */
/*                                                                    */
/*      hMem      :  Handle of the memory area                        */
/*      filename  :  Module name of calling function.                 */
/*      lineno    :  Line number of calling function.                 */
/*                                                                    */
/* OUT-PARAMETERS :    none                                           */
/*                                                                    */
/* RETURN VALUES  :                                                   */
/*                                                                    */
/*        >0   :  NEAR-Pointer to the allocated memory area.          */
/*        NULL :  if an error occurs.                                 */
/*                                                                    */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS :  none                                                    */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr-ro002g_locallock*/
  int i;
/* **************** S T A R T   O F   F U N C T I O N *************** */

				      /* first check the input-handle */
  if (hMem < HDLOFFSET || hMem >= HdlCtr)
      ro002m_MallErrReport ("Invalid input handle in LocalLOCK!",
			     filename, lineno, hMem,
		  "Look where you got this handle from",M_FATAL);

  ro002n_CheckMallTable();    /* check the Table for range violations */

			  /* check whether the incoming handle exists */
  for (i=0; (i<MAXMALL) && (mall[i].MallHdl != hMem); i++);
  if (i==MAXMALL)    /* an entry matching to the handle doesn't exist */
      ro002m_MallErrReport ("Handle not found in Malloc-Table in LocalLOCK!",
			     filename, lineno, hMem,
		  "Look where you got this handle from",M_FATAL);

  if (mall[i].Type != LOCALTYPE)
      ro002m_MallErrReport ("Global/Local mismatch in LocalLOCK!",
			     filename, lineno, mall[i].MallHdl,
	       "Better try <globallock> in this case!",M_NEGLIG);

  mall[i].LockCounter++;

  ro002o_WriteMallTable();                 /* Flush the Table to file */

  if (MallProtocol == M_YES)      /* Flush Entry to the protocol file */
     {
      fprintf(mallprotocol,
	  "Llock with handle:%ld at address:%#10lx in %s line %d \n",
	   hMem,(unsigned long int)mall[i].address,filename,lineno);
     }
  return ( (char NEAR *) mall[i].address + CHECKSIZE );

/* **************** E N D   O F   F U N C T I O N ******************* */
}/*end-ro002g_locallock*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro002h_localunlock                                  */
/*                                                                    */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : MAR-6th-1992                                        */
/*                                                                    */
/* SYNTAX       :                                                     */

unsigned long int PASCAL ro002h_localunlock (
  unsigned long int  hMem,
  char             *filename,
  int               lineno)

/*                                                                    */
/* DESCRIPTION  :  Unlocks the memory block                           */
/*                                                                    */
/* IN-PARAMETERS  :                                                   */
/*      hMem      :  Handle of the memory area                        */
/*      filename  :  Module name of calling function.                 */
/*      lineno    :  Line number of calling function.                 */
/*                                                                    */
/* OUT-PARAMETERS :  none                                             */
/*                                                                    */
/* RETURN VALUES  :                                                   */
/*                                                                    */
/*         0  :  if successful                                        */
/*       <>0  :  error  (invalid memory-handle)                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS :   none                                                   */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr-ro002h_localunlock*/
  int i;
/* **************** S T A R T   O F   F U N C T I O N *************** */

				      /* first check the input-handle */
  if (hMem < HDLOFFSET || hMem >= HdlCtr)
      ro002m_MallErrReport ("Invalid input handle in LocalUNLOCK!",
			     filename, lineno, hMem,
		  "Look where you got this handle from",M_FATAL);

  ro002n_CheckMallTable();    /* check the table for range violations */

  for (i=0; (i<MAXMALL) && (mall[i].MallHdl != hMem); i++);
  if (i==MAXMALL)    /* an entry matching to the handle doesn't exist */
      ro002m_MallErrReport ("Handle not found in Malloc-Table in LocalUNLOCK!",
			     filename, lineno, hMem,
		  "Look where you got this handle from",M_NEGLIG);

  if (mall[i].Type != LOCALTYPE)
      ro002m_MallErrReport ("Global/Local mismatch in LocalUNLOCK!",
			     filename, lineno, mall[i].MallHdl,
	       "Better try <globalunlock> in this case!",M_NEGLIG);

  if (mall[i].LockCounter > 0) /* Windows allows underflow of UNLOCKS */
      mall[i].LockCounter--;

  ro002o_WriteMallTable();                 /* Flush the Table to file */

  if (MallProtocol == M_YES)      /* Flush Entry to the protocol file */
     {
      fprintf(mallprotocol,"Lunlock with handle:%ld in %s line %d \n",
	  hMem,filename,lineno);
     }

  return (0);                                        /* everything ok */

/* **************** E N D   O F   F U N C T I O N ******************* */
}/*end-ro002h_localunlock*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro002i_localrealloc                                 */
/*                                                                    */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : MAR-6th-1992                                        */
/*                                                                    */
/* SYNTAX       :                                                     */

unsigned long int PASCAL ro002i_localrealloc ( 
  unsigned long int hMem,
  unsigned long int oldsize,
  unsigned long int newsize,
  char             *filename,
  int               lineno)

/*                                                                    */
/* DESCRIPTION  :  Reallocates the memory block by increasing         */
/*                 or decreasing its size. If the <newsize> is larger */
/*                 than the <oldsize> was, the extended area will be  */
/*                 filled with zero-bytes.                            */
/*                                                                    */
/* IN-PARAMETERS  :                                                   */
/*                                                                    */
/*      oldsize   :  Old size of the memory block (>0).               */
/*      newsize   :  New size of the memory block (>0).               */
/*      hMem      :  Handle of already allocated memory block.        */
/*      filename  :  Module name of calling function.                 */
/*      lineno    :  Line number of calling function.                 */
/*                                                                    */
/* OUT-PARAMETERS :  none                                             */
/*                                                                    */
/* RETURN VALUES  :                                                   */
/*                                                                    */
/*        >0  :  Handle of the memory block.                          */
/*         0  :  if an error occurs.                                  */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :                                       */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr-ro002i_localrealloc*/
  char NEAR* mptr;
  int i,j,l;
/* **************** S T A R T   O F   F U N C T I O N *************** */

				      /* first check the input-handle */
  if (hMem < HDLOFFSET || hMem >= HdlCtr)
      ro002m_MallErrReport ("Invalid input handle in LocalREALLOC!",
			     filename, lineno, hMem,
		  "Look where you got this handle from",M_FATAL);

  ro002n_CheckMallTable();    /* check the table for range violations */

  for (i=0; (i<MAXMALL) && (mall[i].MallHdl != hMem); i++);
  if (i==MAXMALL)    /* an entry matching to the handle doesn't exist */
      ro002m_MallErrReport ("Handle not found in Malloc-Table in LocalREALLOC!",
			     filename, lineno, hMem,
		  "Look where you got this handle from",M_FATAL);

  if (mall[i].Type != LOCALTYPE)
      ro002m_MallErrReport ("Global/Local mismatch in LocalREALLOC!",
			     filename, lineno, mall[i].MallHdl,
	       "Better try <globalrealloc> in this case!",M_NEGLIG);

  if (mall[i].LockCounter > 0)      /* Reallocation only allowed when */
					     /* no locks are existing */
      ro002m_MallErrReport ("LockCounter not 0 in LocalREALLOC!",
			     filename, lineno, mall[i].MallHdl,
	   "Always unlock the area before calling <realloc>!",M_NEGLIG);

  if (mall[i].size != oldsize)
      ro002m_MallErrReport ("Mismatch of oldsize in LocalREALLOC!",
			     filename, lineno, mall[i].MallHdl,
     "You gave the wrong oldsize-input, maybe the wrong variable?",M_NEGLIG); 

  if (mall[i].address == NULL)
      ro002m_MallErrReport ("Address is 0 in LocalREALLOC!",
			     filename, lineno, mall[i].MallHdl,
       "There might have been some overwriting since allocation",M_FATAL);

  mptr = (char NEAR *) realloc ( mall[i].address, newsize+2*CHECKSIZE );
  if (mptr == NULL)
      ro002m_MallErrReport ("Realloc returned address 0 in LocalREALLOC!",
			     filename, lineno, mall[i].MallHdl,
		"Provide more system-memory and check input parameter",M_FATAL);

  if (newsize > oldsize)
      ro0043_memset (mptr+CHECKSIZE+oldsize, 0, newsize-oldsize);

					      /* now update the entry */
  mall[i].MallHdl = HdlCtr;
  mall[i].LockCounter = 0;
  mall[i].Type = LOCALTYPE;
  mall[i].address = mptr;
  mall[i].size = newsize;
  mall[i].line = lineno;
  l = ro0053_strlen ((char FAR *)filename);
  if (l>(MODULNAMLEN-1))
      l=MODULNAMLEN-1;
  ro0043_memset (mall[i].modul, 0, MODULNAMLEN);
  ro0041_memcpy (mall[i].modul,filename,l+1);

  for (j=0; j<CHECKSIZE; j++)  /* set the checkbytes to 1010101010... */
      {
       *(mptr+j) = CHECKBYTE;
       *(mptr+j+newsize+CHECKSIZE) = CHECKBYTE;
      }
  ro002o_WriteMallTable();                 /* Flush the Table to file */

  if (MallProtocol == M_YES)      /* Flush Entry to the protocol file */
     {
      fprintf(mallprotocol,
  "Lrealloc with hdl:%ld newhdl:%ld oldsize:%ld newsize:%ld in %s line %d \n",
	  hMem,HdlCtr,oldsize,newsize,filename,lineno);
     }

  if (HdlCtr == MAXMALLHDL)               /* abort if Handle-Overflow */
      ro002m_MallErrReport ("Malloc-Handle-Overflow!",
			     filename, lineno, HdlCtr,
	    "Try to avoid so many allocations or expand the limit",M_NEGLIG);
  else
      HdlCtr++;        /* the next alloc shall get a different handle */

  return ( (unsigned long int) ( HdlCtr-1) );       /* a real handle */

/* **************** E N D   O F   F U N C T I O N ******************* */
}/*end-ro002i_localrealloc*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro002j_localfree                                    */
/*                                                                    */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : MAR-6th-1992                                        */
/*                                                                    */
/* SYNTAX       :                                                     */

unsigned long int PASCAL ro002j_localfree ( 
  unsigned long int hMem,
  char             *filename,
  int               lineno)

/*                                                                    */
/* DESCRIPTION    :  Frees the allocated memory block                 */
/*                   connected to <hMem>.                             */
/*                                                                    */
/* IN-PARAMETERS  :                                                   */
/*                                                                    */
/*      hMem      :  Handle to the memory block to be free            */
/*      filename  :  Module name of calling function.                 */
/*      lineno    :  Line number of calling function.                 */
/*                                                                    */
/* OUT-PARAMETERS :              none                                 */
/*                                                                    */
/* RETURN VALUES  :                                                   */
/*                                                                    */
/*         0  : if successful                                         */
/*        !0  : if error  (invalid memory-handle)                     */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS : none                                                     */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr-ro002j_localfree*/
  int i;
/* **************** S T A R T   O F   F U N C T I O N *************** */

				      /* first check the input-handle */
  if (hMem < HDLOFFSET || hMem >= HdlCtr)
      ro002m_MallErrReport ("Invalid input handle in LocalFREE!",
			     filename, lineno, hMem,
		  "Look where you got this handle from",M_FATAL);

  ro002n_CheckMallTable();    /* check the table for range violations */

  for (i=0; (i<MAXMALL) && (mall[i].MallHdl != hMem); i++);
  if (i==MAXMALL)    /* an entry matching to the handle doesn't exist */
      ro002m_MallErrReport ("Handle not found in Malloc-Table in LocalFREE!",
			     filename, lineno, hMem,
		  "Look where you got this handle from",M_FATAL);

  if (mall[i].Type != LOCALTYPE)
      ro002m_MallErrReport ("Global/Local mismatch in LocalFREE!",
			     filename, lineno, mall[i].MallHdl,
	       "Better try <globalfree> in this case!",M_NEGLIG);

  if (mall[i].LockCounter != 0)
      ro002m_MallErrReport ("LockCounter not 0 in LocalFREE!",
			     filename, lineno, mall[i].MallHdl,
	   "Always unlock the area before calling <realloc>!",M_NEGLIG);

  if (mall[i].address == NULL)
      ro002m_MallErrReport ("Address is NULL in LocalFREE!",
			     filename, lineno, mall[i].MallHdl,
       "There might have been some overwriting since allocation",M_FATAL);

		/* fill the occupied area with 0-bytes before release */
  ro0043_memset (mall[i].address, 0,mall[i].size + 2*CHECKSIZE);

		  /* seems to be a good handle so execute the freeing */
  free(mall[i].address);
						    /* free the entry */
  mall[i].MallHdl = 0L;
  mall[i].LockCounter = 0;
  mall[i].Type = NOTYPE;
  mall[i].address = (char FAR * ) 0;
  mall[i].size = 0L;
  mall[i].line = 0;
  ro0043_memset (mall[i].modul, 0, MODULNAMLEN);

  ro002o_WriteMallTable();                         /* update the file */

  if (MallProtocol == M_YES)      /* Flush Entry to the protocol file */
     {
      fprintf(mallprotocol,"Lfree with handle:%ld in %s line %d \n",
	  hMem,filename,lineno);
     }

  return ( (unsigned long int) 0 );                       /* means ok */

/* **************** E N D   O F   F U N C T I O N ******************* */
}/*end-ro002j_localfree*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro002k_OpenMallFiles                                */
/*                                                                    */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : MAR-5th-1992                                        */
/*                                                                    */
/* SYNTAX       :                                                     */

  static void PASCAL ro002k_OpenMallFiles (void)

/* DESCRIPTION  :   This function opens two files:                    */
/*                    1) a file called <mallfile.bin>, where all      */
/*                       existing entries of the malloc-table         */
/*                       are listed.                                  */
/*                    2) a file called <mallptkl.txt>, where each     */
/*                       single alloc-,lock-,unlock- or realloc call  */
/*                       is listed.                                   */
/*                                                                    */
/* INPUT-PARAMETERS :   none                                          */
/*                                                                    */
/* OUTPUT-PARAMETERS :  none                                          */
/*                                                                    */
/* RETURN VALUES :  File-pointer to the mallfile                      */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS : none                                                     */
/*inoff ***************************************************************/

{/*entr-ro002k_OpenMallFile*/
  time_t timer;
  static char zz[28];
/* **************** S T A R T  of  F U N C T I O N ****************** */

  if (mallfptr == NULL)
     {
      mallfptr=fopen(MALLFILEBIN,"wb+");/* open the binary Table file */
      if (mallfptr == NULL)
	  ro002m_MallErrReport ("Couldn't open Mallfile !",
				  "", 0, 0L,
	   "Maybe file system full or no write permission",M_FATAL);
      setbuf (mallfptr, (char *) NULL);          /* switch off buffer */
     }

  if (strcmp("OFF",getenv("MALLPROTOCOL")) == 0)
      MallProtocol = M_NO;

  if (strcmp("ON",getenv("MALLABORT")) == 0)
      MallAbort = M_YES;

  if (mallprotocol == NULL && MallProtocol == M_YES)
     {
      mallprotocol=fopen(MALLPTKLTXT,"w+"); /* open the protocol file */
      if (mallprotocol == NULL)
	  ro002m_MallErrReport ("Couldn't open Protocol-file!",
				  "", 0, 0L,
	   "Maybe file system full or no write permission",M_FATAL);
      setbuf (mallprotocol, (char *) NULL);      /* switch off buffer */
      timer = ro0040_time();
      ro0041_memcpy ((char FAR*)zz,(char FAR*)asctime(localtime((time_t*)&timer)),24L);
      fprintf(mallprotocol,
      "    MALLOC - Protocol - File       [%24s]\n",zz);
      fprintf(mallprotocol,
      "   ============================================================\n");
     }
  return;                                   /* return the Filepointer */

/* **************** E N D   O F   F U N C T I O N ******************* */
}/*end-ro002k_OpenMallFile*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro002l_CloseMallFile                                */
/*                                                                    */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : FEB-22th-1992                                       */
/*                                                                    */
/* SYNTAX       :                                                     */

  static void PASCAL ro002l_CloseMallFiles ( void )

/* DESCRIPTION  :   This function closes the malloc listfile and the  */
/*                  protocol-file.                                    */
/*                                                                    */
/* INPUT-PARAMETERS :   none                                          */
/*                                                                    */
/* OUTPUT-PARAMETERS :  none                                          */
/*                                                                    */
/* RETURN VALUES :  none                                              */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS : none                                                     */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
{/*entr-ro002l_CloseMallFiles*/
/* **************** S T A R T  of  F U N C T I O N ****************** */

			                     /* append the Abort-Info */
  fseek(mallfptr, 0, SEEK_END);
  fwrite(&Abort,sizeof(AbortInfo),1,mallfptr);
  fclose(mallfptr);                            /* close the list file */
  if (MallProtocol == M_YES)
      fclose(mallprotocol);                /* close the protocol file */
  return;                                   /* return the Filepointer */

/* **************** E N D   O F   F U N C T I O N ******************* */
}/*end-ro002l_CloseMallFiles*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro002m_MallErrReport                                */
/*                                                                    */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : FEB-22th-1992                                       */
/*                                                                    */
/* SYNTAX       :                                                     */

static void PASCAL ro002m_MallErrReport ( 
  char * message,
  char * filename,
  unsigned int line,
  unsigned long int hdl,
  char * hint,
  unsigned int diagnostic)

/* DESCRIPTION  :   This function prints out the info-message when a  */
/*                  malloc/free failure was detected.                 */
/*                                                                    */
/* INPUT-PARAMETERS :                                                 */
/*    message : User message to be printed out                        */
/*   filename : Filename where the error occurred                     */
/*       line : Line number where the error occurred                  */
/*        hdl : Memory Handle connected to the Error                  */
/*              (if 0 then internal error type like fopen-error)      */
/*       hint : Gives some hints to remove the error                  */
/*                                                                    */
/* OUTPUT-PARAMETERS :  none                                          */
/*                                                                    */
/* RETURN VALUES :  none                                              */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS : none                                                     */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
{/*entr-ro002m_MallErrReport*/
  char cont = 'x';
/* **************** S T A R T  of  F U N C T I O N ****************** */

  if (MallAbort && MallProtocol)
     {
      fprintf(mallprotocol,"\n  >>> M E M O R Y - E R R O R !!! <<<");
      fprintf(mallprotocol,"\n  %s",message);
      fprintf(mallprotocol,"\n  in File: %s",filename);
      fprintf(mallprotocol,"\n  in Line: %d",line);
      fprintf(mallprotocol,"\n  MemHdl : %ld  (hex: %#lx)",hdl,hdl);
      fprintf(mallprotocol,"\n  Hint   : %s",hint);
      fprintf(mallprotocol,"\n  Error Diagnostics:");
      switch (diagnostic)
	{
	 case M_NEGLIG:
	      fprintf(mallprotocol,"negligible error --- continuation might succeed.");
	      break;
	 case M_FATAL:
	      fprintf(mallprotocol,"fatal error --- continuation will fail!");
	      break;
	}
      abort();
      return;
     }
  else
     {	                       /* Print an error report to the screen */
      printf("\n  >>> M E M O R Y - E R R O R !!! <<<");
      printf("\n  %s",message);
      printf("\n  in File: %s",filename);
      printf("\n  in Line: %d",line);
      printf("\n  MemHdl : %ld  (hex: %#lx)",hdl,hdl);
      printf("\n  Hint   : %s",hint);
      printf("\n  Error Diagnostics:");
      switch (diagnostic)
	{
	 case M_NEGLIG:
	      printf("negligible error --- continuation might succeed.");
	      break;
	 case M_FATAL:
	      printf("fatal error --- continuation will fail!");
	      break;
	}
      printf("\n  Shall I:");
      printf("\n  switch off error-checking and continue ");
      printf("\n  or shall I abort?");
      do               	                    /* switch off checking ?? */
	{
	 if (cont != '\n')
	     printf("  [c|a] ");
	 cont = getchar();                               
	}
      while (cont !='a' && cont !='c');
      if (cont == 'c')           /* continue without further checking */
	 {
	  ro002p_IgnoreMallErr();           
	  if (MallProtocol == M_YES)
	     {
	      fprintf(mallprotocol,
	      "------- Malloc-Checking switched off -------\n");
	     }
	  return;
	 }
      else
	 {			/* fill the error report for flushing */
	  strcpy(Abort.message,message);
	  strcpy(Abort.modul,filename);
	  Abort.line=line;
	  Abort.hdl=hdl;
	  ro002l_CloseMallFiles();            /* Close the mall-files */
	  abort();                       /* enforce a STOP + Coredump */
	 }
     }
/* **************** E N D   O F   F U N C T I O N ******************* */
}/*end-ro002m_MallErrReport*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro002n_CheckMallTable                               */
/*                                                                    */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : FEB-22th-1992                                       */
/*                                                                    */
/* SYNTAX       :                                                     */

  static void PASCAL ro002n_CheckMallTable ( void )

/* DESCRIPTION  :   This function checks all existing malloc-areas    */
/*                  for well behaviour.                               */
/*                                                                    */
/* INPUT-PARAMETERS :  none                                           */
/*                                                                    */
/* OUTPUT-PARAMETERS :  none                                          */
/*                                                                    */
/* RETURN VALUES :  none                                              */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS : none                                                     */
/*inoff ***************************************************************/

{/*entr-ro002n_CheckMallTable*/
  int i,j;
  unsigned long int rc = 0L;
/* **************** S T A R T  of  F U N C T I O N ****************** */
			       /* check whether the table has changed */
			   /* since the last malloc-controller action */
  rc = ro0042_memcmp((char FAR *)mall,(char FAR *)mallold,
		       MAXMALL*sizeof(MallInfo));
  if (rc)
      ro002m_MallErrReport ("Malloc Table was changed outside Mall-Controller!",
			     "unknown", 0L, 0L,
	"Someone writes to unknown area (Pointer mismatch)",M_FATAL);

  for (i=0; i<MAXMALL; i++)       /* test all existing malloc entries */
      if (mall[i].MallHdl != 0L)                       /* entry found */
	 for (j=0; j<CHECKSIZE; j++)    /* test for overwritten areas */
	     if ((*(mall[i].address+j)& 0xff) != CHECKBYTE ||
		 (*(mall[i].address+mall[i].size+CHECKSIZE+j)
		   & 0xff) != CHECKBYTE )
		{
		 ro002m_MallErrReport("Overwritten Malloc Area! This area was created",
				       mall[i].modul,
				       mall[i].line,
				       mall[i].MallHdl,
		   "Examine your last mem-copy operations",M_FATAL);
		}
  return;                                        /* no error detected */

/* **************** E N D   of   F U N C T I O N ******************** */
}/*end-ro002n_CheckMallTable*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro002o_WriteMallTable                               */
/*                                                                    */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : MAR-9th-1992                                        */
/*                                                                    */
/* SYNTAX       :                                                     */

  static void PASCAL ro002o_WriteMallTable ( void )

/* DESCRIPTION  :   This function flushes the Malloc-table to disc    */
/*                                                                    */
/* INPUT-PARAMETERS :  none                                           */
/*                                                                    */
/* OUTPUT-PARAMETERS :  none                                          */
/*                                                                    */
/* RETURN VALUES :  none                                              */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS : none                                                     */
/*inoff ***************************************************************/

{/*entr-ro002o_WriteMallTable*/
/* **************** S T A R T  of  F U N C T I O N ****************** */

					   /* store the updated table */
  ro0041_memcpy((char FAR *)mallold,(char FAR*)mall,
		 MAXMALL*sizeof(MallInfo));

  if (mallfptr == NULL)       /* if no Malloc-File exists - create it */
      ro002k_OpenMallFiles();
					 /* flush whole table to disk */
  fseek (mallfptr, 0, SEEK_SET);
  fwrite ( mall, sizeof (MallInfo), MAXMALL, mallfptr);
  return;

/* **************** E N D   of   F U N C T I O N ******************** */
}/*end-ro002o_WriteMallTable*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro002p_IgnoreMallErr                                */
/*                                                                    */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : MAR-15th-1992                                       */
/*                                                                    */
/* SYNTAX       :                                                     */

  static void PASCAL ro002p_IgnoreMallErr ( void )

/* DESCRIPTION  :   This function switches off malloc checking.       */
/*                                                                    */
/* INPUT-PARAMETERS :  none                                           */
/*                                                                    */
/* OUTPUT-PARAMETERS :  none                                          */
/*                                                                    */
/* RETURN VALUES :  none                                              */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  IgnoreMallocErrors                   */
/*                                                                    */
/* REMARKS : Sets the static Variable 'IgnoreMallocErrors' to M_YES.  */
/*           This provides the possibility to continue, but if the    */
/*           detected error is too serious a fatal error can appear.  */
/*inoff ***************************************************************/

{/*entr-ro002p_IgnoreMallErr*/
/* **************** S T A R T  of  F U N C T I O N ****************** */

  IgnoreMallocErrors = M_YES;     /* ignore malloc-errors from now on */

  printf("\n Malloc-Error-Checking OFF now ! \n");

  return;

/* **************** E N D   of   F U N C T I O N ******************** */
}/*end-ro002p_IgnoreMallErr*/


#endif /*RC0007_CHECKMALLOC*/

/* ****************************************************************** */
/* ****************  E N D   O F   M O D U L E    ******************* */
/* ****************************************************************** */
