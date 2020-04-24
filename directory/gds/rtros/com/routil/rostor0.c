/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rostor0.c,v $
 * Revision 1.1.4.2  1996/03/11  13:20:05  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:10  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:32:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:18:22  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:07:53  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:49:54  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:59:43  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:54:52  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:34:27  keutel
 * 	creation
 * 	[1994/03/21  13:24:36  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: rostor0.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:20:05 $" } ;
#endif

/***************************************************************************\
*                                                                           *
*  * Copyright (c) 1992 by                                                  *
*  * SIEMENS NIXDORF INFORMATIONSSYSTEME AG                                 *
*  * All rights reserved                                                    *
*  *                                                                        *
*  * This software is furnished under licence and may be used only in       *
*  * accordance with the terms of that licence and with the inclusion of    *
*  * the above copyright notice. This software may not be provided or       *
*  * otherwise made available to, or used by, any other person. No title    *
*  * to or ownership of the software is hereby transferred.                 *
*                                                                           *
\***************************************************************************/

/*daton	*********************************************************************/
/*		           					            */
/* TYPE		: MODULE					            */
/*								            */
/* NAME         : rostor0.c                                                 */
/*								            */
/* AUTHOR       : Gerhard Jahn                                              */
/* DATE         : FEB-25h-1992                                              */
/*								            */
/* COMPONENT    : RTS store utility functions                               */
/*								            */
/* PRD#/VERS.   : RTS-V2.0                                                  */
/*								            */
/* DESCRIPTION	: This modul provides functions	which		            */
/*								            */
/*   - read and write data to a buffered store                              */
/*								            */
/* SYSTEM DEPENDENCE :  SINIX V5.4 (no PROTO-check) / MS-DOS 3.30           */
/*								            */
/* HISTORY	:						            */
/*								            */
/* Vers.Nr. | Date     |	 Modification		   |mark| CR# EM#   */
/* 1.00 K0  | 91-09-27 |  original                         | gj |           */
/* 1.01 K0  | 91-10-18 |  new: stlink,stgetcp,stcat (RTS)  | gj |           */
/* 1.02 K0  | 91-10-31 |  new: seterror (for MAVROS)       | gj |           */
/* 1.03 K0  | 91-11-08 |  new: stmove (for MAVROS)         | gj |           */
/* 1.04 K0  | 91-11-12 |  changed: stcat (for MAVROS+RTS)  | gj |           */
/* 1.05 K0  | 91-11-14 |  changed: memory mapping fncts    | gj |           */
/* 1.05 K0  | 91-11-21 |  new: FileToStore                 | gj |           */
/* 1.06 K0  | 91-11-28 |  corrected: stmove                | gj |           */
/* 1.07 K0  | 91-11-28 |  changed: newpos in stmove        | gj |           */
/* 1.08 K0  | 92-02-24 |  new: stbufresize                 | gj |           */
/* 1.09 K0  | 92-02-25 |  corrected: stgetcp ->fseek       | gj |           */
/* 1.10 K0  | 92-03-02 |  corrected: stgets/stputs         | gj |           */
/*                     |  hs->brest will be updated now    |    |           */
/* 1.11 K0  | 92-03-05 |  new: stStoreResize for resize    | gj |           */
/*                     |  after a stgets/stputs only       |    |           */
/* 1.12 K0  | 92-03-25 |  useless stflush in deletestore   | gj |           */
/*                     |  removed to avoid system-calls    |    |           */
/* 1.13 K0  | 92-05-26 |  new: OpenStFile - storefile will | gj |           */
/*                     |  be a temporary file now          |    |           */
/* 1.14 K0  | 92-05-26 |  new: CloseStFile                 | gj |           */
/* 1.15 K0  | 92-05-26 |  newstore optimized for speed     | gj |           */
/* 2.00 K0  | 93-05-10 |  Use of utility functions for     | ma/|           */
/*          |          |  file handling 'C' library fns.   |sss |           */
/*          |          |  such as fopen/fclose/tmpnam etc. |    |           */
/*datoff ********************************************************************/

/****************************************************************************/
/*		       Module Identification				    */
/****************************************************************************/

static char *sccsid =    "@(#) rostor0.c 1.2 93/12/20   RTS-V2.00";

/****************************************************************************/
/*                    S T D - I N C L U D E S                               */
/****************************************************************************/

/* #include <stdio.h>
   #include <stdlib.h>
   #include <string.h> */
/* removed because store now uses util functions -sss 05/10/93              */

/*****************************************************************************/
/*        D E F I N E S,  T Y P E D E F S  &   P R O T O T Y P E S           */
/*****************************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>                             /* general defines for RTS */
#include <rostor8.h>                               /* private store include */
#include <rostor0.h>                                    /* Store Prototypes */
#include <routil0.h>                         /* utilitiy-prototypes+defines */
#include <rolgval.h>                         /* Logging definitions         */

/**********************************************************************************/
/*                                                                                */
/*              P R O V I D E D   G L O B A L   F U N C T I O N S                 */
/*                                                                                */
/* unsigned long int PASCAL ro0060_newstore (long length, int mode,               */
/*                                           signed short int FAR * error);       */
/* long PASCAL              ro0061_stbuflen (unsigned long int hdl);              */
/* long PASCAL              ro0062_storelength (unsigned long int hdl);           */
/* signed short int PASCAL  ro0063_sterror (unsigned long int hdl);               */
/* signed short int PASCAL  ro0064_stflush (unsigned long int hdl);               */
/* long PASCAL              ro0065_stbufread (unsigned long int hdl);             */
/* signed short int PASCAL  ro0066_storepos (unsigned long int hdl, long pos);    */
/* signed short int PASCAL  ro0067_stputc (unsigned long int hdl, unsigned char databyte); */
/* signed short int PASCAL  ro0068_stputm (unsigned long int hdl,                 */
/*                                         char FAR *madr, long length);          */
/* char FAR * PASCAL        ro0069_stputs (unsigned long int hdl, long length);   */
/* signed short int PASCAL  ro0070_stgetc (unsigned long int hdl);                */
/* long PASCAL              ro0071_stgetm (unsigned long int hdl,                 */
/*                                         char FAR *madr, long length);          */
/* char FAR * PASCAL        ro0072_stgets (unsigned long int hdl, long length);   */
/* signed short int PASCAL  ro0073_storeidle (unsigned long int hdl);             */
/* signed short int PASCAL  ro0074_storeactive (unsigned long int hdl, long pos); */
/* signed short int PASCAL  ro0075_deletestore (unsigned long int hdl);           */
/* long PASCAL              ro0076_stwhere (unsigned long int hdl);               */
/* signed short int PASCAL  ro0077_stgetcp (unsigned long int hdl);               */
/* int PASCAL               ro0078_stlink  (unsigned long int hdl);               */
/* signed short int PASCAL  ro0079_stcat (unsigned long int hdl1,                 */
/*                                       unsigned long int hdl2,                  */
/*                                       long offset, length);                    */
/* signed short int PASCAL  ro0080_seterror (unsigned long int hdl,               */
/*                                          signed short int error);              */
/* signed short int PASCAL  ro0081_stmove (unsigned long int hdl,                 */
/*                                        long destend,long sourceend             */
/*                                        long length, long newpos);              */
/* unsigned long int PASCAL ro0082_FileToStore (char FAR *name,long buflen);      */
/* long PASCAL              ro0083_stbufresize (unsigned long int hdl);           */
/* long PASCAL              ro0084_stStoreResize ( unsigned long int hdl,         */
/*                                                 long newsize );                */
/* signed short int PASCAL  ro0085_OpenStFile (unsigned long int hdl);            */
/* signed short int PASCAL  ro0086_CloseStFile (unsigned long int hdl);           */
/* signed short int PASCAL  ro0087_StoreToFile (unsigned long int hdl,
  */
/*                                                char FAR * name );          
  */
/**********************************************************************************/

/*exon **********************************************************************/
/*		     						            */
/* TYPE         : C-FUNCTION                                                */
/*								            */
/* NAME         : ro0060_newstore                                           */
/*								            */
/* AUTHOR       : Gerhard Jahn                                              */
/* DATE         : OCT-18th-1991                                             */
/*								            */
/* SYNTAX	:						            */

unsigned long int PASCAL ro0060_newstore (
   long length,
   int mode,
   signed short int FAR *error)

/*			    					            */
/* DESCRIPTION  :   This function creates a new object of type STORE. The   */
/*                  STORE-Components are initialized to well defined values.*/
/*                  A memory area of size of <length> is allocated for a    */
/*                  buffer. If <length> exceeds a predefined limit, this    */
/*                  limit will be used as buffer size. If the installation  */
/*                  fails, <NULL> is returned and no memory is allocated.   */
/*                  The type of error can be seen from the input variable   */
/*                  <error>, which is handed over by address.               */
/*								            */
/* INPUT-PARAMETERS :                                                       */
/*								            */
/*      length   : desired buffer size  (truncated if too long)             */
/*                 if ST_USEMAXBUF the maximum buffersize <MAXBULEN>        */
/*                 will be used.                                            */
/*      mode     : whether a file shall be opened at the                    */
/*                 beginning or not.                                        */
/*                 (ST_OPEN = always open file, ST_CLOSED= don't open)      */
/*      error    : success information of installation                      */
/*								            */
/* OUTPUT-PARAMETERS :                                                      */
/*								            */
/*      error    : success information of installation                      */
/*                        0 : no error                                      */
/*                     else : type of internal error                        */
/*								            */
/* RETURN VALUES :						            */
/*								            */
/*        NULL   : internal error (no store installed -> see error)         */
/*        else   : storehandle                                              */
/*								            */
/* GLOBAL DATA	 ( read	only ):	 none				            */
/*								            */
/* GLOBAL DATA	 ( modified ) :	 none				            */
/*								            */
/* REMARKS : Don't use any Store-functions without a successful             */
/*           installation of a Store!!                                      */
/*inoff	*********************************************************************/
{/*entr-ro0060_newstore*/
/* *******************   l o c a l   v a r i a b l e s   ****************** */
  STORE FAR * hs;
  unsigned long int reject,handle;
/* *****************   S T A R T   O F   F U N C T I O N   **************** */
  *error = OK;
  if ((mode != ST_OPEN) && (mode != ST_CLOSED))      /* mode must be 0 or 1 */
     {*error = STMODEER; return(NULL);}
  if (length < -1)                           /* no reason for such a length */
     {*error = STBULENE; return(NULL);}
  handle = ro0020_globalalloc (sizeof(STORE));              /* get a handle */
  if (!handle)                                /* no handle because of error */
     {*error = STGALLCE;  return (NULL); }            /* fatal memory error */
  hs = (STORE FAR *) ro0021_globallock (handle);  /* reference to the STORE */
  if (hs == NULL)
     {ro0024_globalfree(handle);                              /* free error */
      *error = STGLOCKE;
      return (NULL); }                                  /* fatal free error */
  hs->handle = handle;
  hs->lockhandle = (unsigned long int) hs;              /* fnct-call handle */
  hs->stlength = 0L;
		       /* limitation of buffer size if MAXBULEN is exceeded */
			     /* if the length-input is -1 then use MAXBULEN */
						  /* as default buffer size */
  length = (length>MAXBULEN || length == ST_USEMAXBUF)? MAXBULEN : length;

  hs->blength  = length;
  hs->offset = 0L;
  hs->links = 0;                                /* first user of this Store */
					      /* now allocating buffer area */
  hs->bufhandle = ro0020_globalalloc (length);
  if (hs->bufhandle == NULL)
     {*error = STGALLCE;                                     /* Alloc Error */
      ro0022_globalunlock(hs->handle);               /* unlock STORE struct */
      ro0024_globalfree  (hs->handle);                        /* free Store */
      return (NULL);                                        /* error signal */
     }
  hs->base = (unsigned  char FAR * ) ro0021_globallock (hs->bufhandle);
  if (hs->base == NULL)
     {*error = STGLOCKE;                                      /* Lock Error */
      ro0024_globalfree(hs->bufhandle);                      /* free buffer */
      ro0022_globalunlock(hs->handle);               /* unlock STORE struct */
      ro0024_globalfree  (hs->handle);                        /* free Store */
      return (NULL);                                        /* error signal */
     }
  hs->bptr = hs->base;
  hs->brest=length;
  hs->fp = NULL;
			   /* modify-flag must be set if openmode is <OPEN> */
			   /* otherwise stflush wouldn't open a file at all */
			      /* if openmode = 0 then clear the modify-flag */
  hs->modify=(mode == ST_OPEN)? ST_YES : ST_NO;
  hs->error=OK;                                        /* default no error  */

  if (mode == ST_OPEN)                       /* open file if wanted by user */
      *error = ro0064_stflush(hs->lockhandle);
  if (*error == OK)
      return ( (unsigned long int) hs);                /* return the handle */
						 /* release all allocations */
		       /* when an error comes from stflush ---- free buffer */
  reject = ro0022_globalunlock(hs->bufhandle);
  if (reject)
     {*error = STGULOKE;}
  reject = ro0024_globalfree  (hs->bufhandle);
  if (reject)
     {*error = STGFREEE;}
  reject = ro0022_globalunlock(hs->handle);                   /* free Store */
  if (reject)
     {*error = STGULOKE;}
  reject = ro0024_globalfree  (hs->handle);
  if (reject)
     {*error = STGFREEE;}
  return(NULL);

/* *****************   E N D   O F   F U N C T I O N   ******************** */
}/*end-ro0060_newstore*/

/*exon **********************************************************************/
/*			        				            */
/* TYPE         : C-FUNCTION                                                */
/*								            */
/* NAME         : ro0061_stbuflen                                           */
/*								            */
/* AUTHOR       : Gerhard Jahn                                              */
/* DATE         : SEP-27th-1991                                             */
/*								            */
/* SYNTAX	:						            */

long PASCAL ro0061_stbuflen ( 
   unsigned long int hdl)

/*								            */
/* DESCRIPTION  :   This function returns the size of the actual buffer.    */
/*								            */
/* INPUT-PARAMETERS :                                                       */
/*								            */
/*      hdl      : handle to the desired store                              */
/*								            */
/* OUTPUT-PARAMETERS :  none                                                */
/*								            */
/* RETURN VALUES :						            */
/*								            */
/*      >=0 : size of the buffer                                            */
/*       <0 : error status                                                  */
/*								            */
/* GLOBAL DATA	 ( read	only ):	 none				            */
/*								            */
/* GLOBAL DATA	 ( modified ) :	 none				            */
/*								            */
/* REMARKS		      :	 none				            */
/*								            */
/*inoff	*********************************************************************/

/* *******************   l o c a l   v a r i a b l e s   ****************** */
{/*entr-ro0061_stbuflen*/
  STORE FAR * hs;
/* *****************   S T A R T   O F   F U N C T I O N   **************** */

  hs = (STORE FAR *) hdl;
  if (hs->lockhandle != hdl)                    /* check for invalid handle */
      return ( (long) STHDLERR);

  if (hs->error != OK)
      return(hs->error);
  else
      return(hs->blength);                                    /* no comment */
/* *****************   E N D   O F   F U N C T I O N   ******************** */
}/*end-ro0061_stbuflen*/

/*exon **********************************************************************/
/*								            */
/* TYPE         : C-FUNCTION                                                */
/*								            */
/* NAME         : ro0062_storelength                                        */
/*								            */
/* AUTHOR       : Gerhard Jahn                                              */
/* DATE         : SEP-27th-1991                                             */
/*								            */
/* SYNTAX	:						            */

long PASCAL ro0062_storelength ( 
   unsigned long int hdl)

/*								            */
/* DESCRIPTION  :   This function updates the stlength component within the */
/*                  STORE and returns this new value.                       */
/*								            */
/* INPUT-PARAMETERS :                                                       */
/*								            */
/*      hdl      : handle to the desired store                              */
/*								            */
/* OUTPUT-PARAMETERS :   none                                               */
/*								            */
/* RETURN VALUES :						            */
/*							                    */
/*      >=0 : actual store-length                                           */
/*       <0 : error status                                                  */
/*								            */
/* GLOBAL DATA	 ( read	only ):	 none				            */
/*								            */
/* GLOBAL DATA	 ( modified ) :	 none				            */
/*								            */
/* REMARKS		      :	 none				            */
/*								            */
/*inoff	*********************************************************************/

/* ******************   l o c a l   v a r i a b l e s   ******************* */
{/*entr-ro0062_storelength*/
  STORE FAR * hs;
/* *****************   S T A R T   O F   F U N C T I O N   **************** */

  hs = (STORE FAR *) hdl;
  if (hs->lockhandle != hdl)                    /* check for invalid handle */
      return ( (long) STHDLERR);

			 /* stlength is only updated if the old position is */
			       /* lower than the actual read/write position */

  if (hs->offset + hs->blength - hs->brest > hs->stlength)
      hs->stlength = hs->offset + hs->blength - hs->brest;

  if (hs->error != OK)
      return(hs->error);
  else
      return(hs->stlength);
/* *****************   E N D   O F   F U N C T I O N   ******************** */
}/*end-ro0062_storelength*/

/*exon **********************************************************************/
/*								            */
/* TYPE         : C-FUNCTION                                                */
/*								            */
/* NAME         : ro0063_sterror                                            */
/*								            */
/* AUTHOR       : Gerhard Jahn                                              */
/* DATE         : OCT-31st-1991                                             */
/*								            */
/* SYNTAX	:						            */

signed short int PASCAL ro0063_sterror ( 
   unsigned long int hdl)

/*								            */
/* DESCRIPTION  :   This function reports the actual error status by reading*/
/*                  the error component of the STORE.                       */
/*								            */
/* INPUT-PARAMETERS :                                                       */
/*								            */
/*      hdl     :  handle to the desired store                              */
/*								            */
/* OUTPUT-PARAMETERS :   none                                               */
/*								            */
/* RETURN VALUES :						            */
/*								            */
/*      actual error status                                                 */
/*								            */
/* GLOBAL DATA	 ( read	only ):	 none				            */
/*								            */
/* GLOBAL DATA	 ( modified ) :	 none				            */
/*								            */
/* REMARKS		      :	 none				            */
/*								            */
/*inoff	*********************************************************************/

/* ******************   l o c a l   v a r i a b l e s   ******************* */
{/*entr-ro0063_sterror*/
  STORE FAR * hs;
/* *****************   S T A R T   O F   F U N C T I O N   **************** */

  hs = (STORE FAR *) hdl;
  if (hs->lockhandle != hdl)                    /* check for invalid handle */
     {hs->error = STHDLERR; return (STHDLERR);}

  return (hs->error);                                         /* no comment */
/* *****************   E N D   O F   F U N C T I O N   ******************** */
}/*end-ro0063_sterror*/

/*exon **********************************************************************/
/*								            */
/* TYPE         : C-FUNCTION                                                */
/*								            */
/* NAME         : ro0064_stflush                                            */
/*								            */
/* AUTHOR       : Gerhard Jahn                                              */
/* DATE         : SEP-27th-1991                                             */
/*								            */
/* SYNTAX	:						            */

signed short int PASCAL ro0064_stflush ( 
   unsigned long int hdl)

/*								            */
/* DESCRIPTION  :   This function is used by several other store-functions. */
/*                  It saves the actual buffer into the right place within  */
/*                  the store-file, but this is only performed when the     */
/*                  buffer has been modified since the last buffer read     */
/*                  action. The storelength will be updated in every case.  */
/*                  If an error appears during file access it will be       */
/*                  reported in the error component of the STORE and by the */
/*                  return value.                                           */
/*								            */
/* INPUT-PARAMETERS :                                                       */
/*								            */
/*      hdl      : handle to the desired store                              */
/*								            */
/* OUTPUT-PARAMETERS :   none                                               */
/*								            */
/* RETURN VALUES :						            */
/*								            */
/*      actual error status   ( 0 = no error )                              */
/*								            */
/* WORKING METHOD : Before saving the buffer area to the store file, it     */
/*                  first has to be calculated how many bytes are to save.  */
/*                  This is done by actualizing stlength. If there were no  */
/*                  modifications within the buffer there is no need to     */
/*                  save it, so all that's done is a return without any     */
/*                  modifications. If the buffer was modified, a file is    */
/*                  opened if it does not exist already. If a fatal file    */
/*                  error appeared like for e.g. 'disk full' or 'no writing */
/*                  permission' the whole action is cancelled and no buffer */
/*                  will be saved to the store. So the User is strictly     */
/*                  advised to check the returned error status to avoid     */
/*                  loss of data !! If writing was possible there is another*/
/*                  chance for a failure: not all data could be written.    */
/*                  An abort will be the consequence, because although data */
/*                  were written, the Store content isn't correct at all.   */
/*								            */
/* GLOBAL DATA	 ( read	only ):	 none				            */
/*								            */
/* GLOBAL DATA	 ( modified ) :	 none				            */
/*								            */
/* REMARKS: Please note that when a file is creted it will be a temporary   */
/*          file! This means it will be deleted by a close-action to this   */
/*          or by termination of the running process.                       */
/*inoff	*********************************************************************/

/* ******************   l o c a l   v a r i a b l e s   ******************* */
{/*entr-ro0064_stflush*/
  STORE FAR * hs;
  long wlen;
  signed short int status; 
  signed short int rc;
/* *****************   S T A R T   O F   F U N C T I O N   **************** */

  hs = (STORE FAR *) hdl;
  if (hs->lockhandle != hdl)                    /* check for invalid handle */
     {hs->error = STHDLERR; return (STHDLERR);}

  ro0062_storelength (hdl);                  /* at least update storelength */

			  /* only if modifications in buffer have been made */
		  /* safe actual buffer back into the store else do nothing */
  if (hs->modify == ST_YES)
     {
			/* now calculating how many bytes have to be stored */
					   /* stlength exceeds buffer limit */
     if (hs->stlength > hs->offset + hs->blength)
	 wlen = hs->blength;                           /* save whole buffer */
     else if (hs->stlength > hs->offset + hs->blength - hs->brest)
				    /* stlength exceeds actual r/w position */
	      wlen = hs->stlength - hs->offset;
					  /* save more than to r/w position */
	  else
	     wlen=hs->blength - hs->brest;        /* save 'til w/r position */

     if (hs->fp == NULL)                     /* when no file was opened yet */
	{
	 if (ro0085_OpenStFile(hdl) )                /* open the Store-file */
	     return (hs->error);            /* it will be a temporary file! */
	}
     status = ro00f5_fseek ( hs->fp, hs->offset,  RC0080_SEEK_SET );
				     /* seek for right position in file and */
				     /* check if file limit is not exceeded */
     if (status != OK)
	{ hs->error = STFSEEKE; return (hs->error); }

					/* try to save wlen data into store */
     rc = ro00f3_fwrite ( hs->base, sizeof(char), wlen, hs->fp );

     if (rc != OK)               /* not all of the bytes were written */
	{ hs->error = STFWRITE;
	  return (STFWRITE);
	}
     hs->offset += hs->bptr-hs->base;
     hs->bptr = hs->base;                                /* update pointers */
     hs->brest = hs->blength;
     hs->modify = ST_NO;          /* reset modification flag for new buffer */
     return (hs->error);                          /* give back error status */
  }
  else return (OK);                              /* no buffer modifications */
/* *****************   E N D   O F   F U N C T I O N   ******************** */
}/*end-ro0064_stflush*/

/*exon **********************************************************************/
/*								            */
/* TYPE         : C-FUNCTION                                                */
/*								            */
/* NAME         : ro0065_stbufread                                          */
/*								            */
/* AUTHOR       : Gerhard Jahn                                              */
/* DATE         : SEP-27th-1991                                             */
/*								            */
/* SYNTAX	:						            */

long PASCAL ro0065_stbufread ( 
   unsigned long int hdl)

/*			       					            */
/* DESCRIPTION  :   This function is used by several other store-functions. */
/*                  It reads a new buffer beginning at the value of the     */
/*                  STORE-offset component. This means that if no new       */
/*                  positioning has been made (for e.g. by stflush or       */
/*                  storepos) the old buffer would be read in again. If no  */
/*                  buffer-memory is allocated, nothing is done but an error*/
/*                  report. If all was o.k. the number of bytes which were  */
/*                  read from the store is returned.                        */
/*								            */
/* INPUT-PARAMETERS :                                                       */
/*							                    */
/*      hdl      : handle to the desired store                              */
/*								            */
/* OUTPUT-PARAMETERS :    none                                              */
/*								            */
/* RETURN VALUES :						            */
/*                                                                          */
/*       <0   :  if an error appeared --> see sterror for error type        */
/*      >=0   :  number of read bytes in buffer                             */
/*								            */
/* GLOBAL DATA	 ( read	only ):	 none			                    */
/*								            */
/* GLOBAL DATA	 ( modified ) :	 none				            */
/*								            */
/* REMARKS		      :	 none				            */
/*								            */
/*inoff	*********************************************************************/

/* ******************   l o c a l   v a r i a b l e s   ******************* */
{/*entr-ro0065_stbufread*/
  STORE FAR * hs;
  long number;
/* *****************   S T A R T   O F   F U N C T I O N   **************** */

  hs = (STORE FAR *) hdl;
  if (hs->lockhandle != hdl)                    /* check for invalid handle */
     {hs->error = STHDLERR; return ( (long) STHDLERR);}

				    /* if no buffer is allocated do nothing */
  if (hs->base == NULL || hs->bptr == NULL)      
     {hs->error = STBUMALE;                          /* but an error report */
       return( RC0083_EOF);  }                                  /* signal for error */

  if (ro00f5_fseek(hs->fp, hs->offset,  RC0080_SEEK_SET) != OK)
     {hs->error = STFSEEKE;
      return ( RC0083_EOF);                                     /* signal for error */
     }
  number = ro00fe_fread(hs->base, sizeof(char), hs->blength, hs->fp);
  hs->bptr  = hs->base;        /* **skh */
  hs->brest = hs->blength;     /* **skh */

  return (number);                                  /* number of read bytes */
/* *****************   E N D   O F   F U N C T I O N   ******************** */
}/*end-ro0065_stbufread*/

/*exon **********************************************************************/
/*								            */
/* TYPE         : C-FUNCTION                                                */
/*								            */
/* NAME         : ro0066_storepos                                           */
/*								            */
/* AUTHOR       : Gerhard Jahn                                              */
/* DATE         : SEP-27th-1991                                             */
/*								            */
/* SYNTAX	:						            */

signed short int PASCAL ro0066_storepos(
   unsigned long int hdl,
   long pos)

/*		     						            */
/* DESCRIPTION  :   This function is used to move to any position of the    */
/*                  store. If <pos> lies outside the store the noting is    */
/*                  done. When leaving the function, the buffer is filled   */
/*                  with the bytes following <pos>. The error status is     */
/*                  reported by the return value.                           */
/*								            */
/* INPUT-PARAMETERS :                                                       */
/*								            */
/*      hdl      : handle to the desired store                              */
/*      pos      : absolute store-position where to move to                 */
/*                                                                          */
/* OUTPUT-PARAMETERS :    none                                              */
/*								            */
/* RETURN VALUES :						            */
/*                                                                          */
/*      actual error status                                                 */
/*								            */
/* GLOBAL DATA	 ( read	only ):	 none				            */
/*								            */
/* GLOBAL DATA	 ( modified ) :	 none				            */
/*								            */
/* REMARKS :    ATTENTION!! the file numbering starts with zero !!          */
/*								            */
/*inoff	*********************************************************************/

/* ******************   l o c a l   v a r i a b l e s   ******************* */
{/*entr-ro0066_storepos*/
  STORE FAR * hs;
  signed short int status;
  signed short int err;
/* *****************   S T A R T   O F   F U N C T I O N   **************** */

  hs = (STORE FAR *) hdl;
  if (hs->lockhandle != hdl)                    /* check for invalid handle */
     {hs->error = STHDLERR; return(STHDLERR);}
				    /* if no buffer is allocated do nothing */
  if (hs->base == NULL || hs->bptr == NULL)      
     {hs->error = STBUMALE; return(STBUMALE);}       /* but an error report */
			    
  ro0062_storelength(hdl);           /* update length before moving pointer */
			       /* pos must be within the range of the store */
  if ( pos > hs->stlength || pos < 0 )
     { hs->error = STPOSERR; return (STPOSERR); }      /* don't do anything */

  if ((pos >= hs->offset) && pos <= (hs->offset + hs->blength))
     {                                /* pos lies within the present buffer */
       hs->bptr = hs->base + pos - hs->offset;
       hs->brest = hs->blength - pos + hs->offset;
       return (hs->error);
     }
  else                                    /* pos lies outside actual buffer */
     {
       err = ro0064_stflush(hdl);   /* don't change hs->error in good cases */
       if (err == OK)                                  /* safe buffer first */
	  {
	   status = ro00f5_fseek ( hs->fp, pos,  RC0080_SEEK_SET );
				     /* seek for right position in file and */
				     /* check if file limit is not exceeded */

	   if (status != OK)                       /* fatal error with file */
	      { hs->error = STFSEEKE;
		return (STFSEEKE);           /* don't do anything but leave */
	      }
				     /* try to read blength data into store */
	   ro00fe_fread ( hs->base, sizeof(char), hs->blength, hs->fp );
			       /* no error check needed because no problems */
	   hs->offset = pos;                    /* move to desired position */
	   hs->bptr = hs->base;                      /* and update pointers */
	   hs->brest = hs->blength;
	  }
       else
	   hs->error = err;                           /* error from stflush */
      }
  return (hs->error);                                       /* that's clear */
}/*end-ro0066_storepos*/
/* *****************   E N D   O F   F U N C T I O N   ******************** */

/*exon **********************************************************************/
/*								            */
/* TYPE         : C-FUNCTION                                                */
/*								            */
/* NAME         : ro0067_stputc                                             */
/*								            */
/* AUTHOR       : Gerhard Jahn                                              */
/* DATE         : SEP-27th-1991                                             */
/*								            */
/* SYNTAX	:						            */

signed short int PASCAL ro0067_stputc(
  unsigned long int hdl,
  unsigned char databyte)

/*								            */
/* DESCRIPTION  :   This function writes one <databyte> to the actual       */
/*                  position of the store. The error status is returned.    */
/*                  After writing the actual r/w pointer <bptr>, <brest> is */
/*                  increased or decreased respectively.                    */
/*								            */
/* INPUT-PARAMETERS :                                                       */
/*								            */
/*      hdl      : handle to the desired store                              */
/*      databyte : byte that shall be written to the store                  */
/*								            */
/* OUTPUT-PARAMETERS :  none                                                */
/*								            */
/* RETURN VALUES :						            */
/*                                                                          */
/*      actual error status                                                 */
/*								            */
/* GLOBAL DATA	 ( read	only ):	 none				            */
/*								            */
/* GLOBAL DATA	 ( modified ) :	 none				            */
/*								            */
/* REMARKS		      :	 none				            */
/*								            */
/*inoff	*********************************************************************/

/* ******************   l o c a l   v a r i a b l e s   ******************* */
{/*entr-ro0067_stputc*/
  STORE FAR * hs;
  long number;
  signed short int err;
/* *****************   S T A R T   O F   F U N C T I O N   **************** */

  hs = (STORE FAR *) hdl;
  if (hs->lockhandle != hdl)                    /* check for invalid handle */
     {hs->error = STHDLERR; return (STHDLERR);}

  if (hs->brest < 1)                         /* when no room left in buffer */
     {err=ro0064_stflush (hdl);                       /* clear buffer first */
      if (err == OK)
	 {
	  number = ro0065_stbufread(hdl);       /* take care that buffer is */
						/* filled with right values */
	  if (number ==  RC0083_EOF)                                 /* error check */
	      return (hs->error);                     /* hard error - leave */
	 }
      else                    /* fatal error from stflush --> leave at once */
	 {
	  hs->error = err;
	  return (hs->error);
	 }
     }
  hs->modify = ST_YES;                          /* set modification flag */
  *(hs->bptr)= databyte;                               /* store databyte */
  hs->bptr++;                                         /* adjust pointers */
  hs->brest--;
  return(hs->error);
/* *****************   E N D   O F   F U N C T I O N   ******************** */
}/*end-ro0067_stputc*/

/*exon **********************************************************************/
/*							      	            */
/* TYPE         : C-FUNCTION                                                */
/*								            */
/* NAME         : ro0068_stputm                                             */
/*								            */
/* AUTHOR       : Gerhard Jahn                                              */
/* DATE         : NOV-20th-1991                                             */
/*								            */
/* SYNTAX	:						            */

signed short int PASCAL ro0068_stputm ( 
   unsigned long int hdl,
   char FAR *madr,
   long length)

/*							                    */
/* DESCRIPTION  :   This function writes <length> data beginning at memory  */
/*                  address <madr> into the store. The error status is      */
/*                  returned.                                               */
/*							                    */
/* INPUT-PARAMETERS :                                                       */
/*								            */
/*      hdl      : handle to the desired store                              */
/*      madr     : start address where bytes shall be copied from           */
/*      length   : number of bytes to copy                                  */
/*								            */
/* OUTPUT-PARAMETERS :   none                                               */
/*								            */
/* RETURN VALUES :					                    */
/*                                                                          */
/*      actual error status                                                 */
/*								            */
/* GLOBAL DATA	 ( read	only ):	 none				            */
/*								            */
/* GLOBAL DATA	 ( modified ) :	 none				            */
/*								            */
/* REMARKS		      :	 none				            */
/*								            */
/*inoff	*********************************************************************/

{/*entr-ro0068_stputm*/
/* ******************   l o c a l   v a r i a b l e s   ******************* */
  STORE FAR * hs;
  signed short int err;
  long oldpos;
/* *****************   S T A R T   O F   F U N C T I O N   **************** */

  hs = (STORE FAR *) hdl;
  if (hs->lockhandle != hdl)                    /* check for invalid handle */
     {hs->error = STHDLERR; return (STHDLERR);}
  if (madr == NULL)                            /* check for invalid address */
     {hs->error = STMADRER; return (STMADRER);}
  if (length < 0)
     {hs->error = STLENERR; return (STLENERR);}  /* check for useful length */
			  /* retain the absolute r/w position for later use */
  oldpos = hs->bptr - hs->base + hs->offset;
  if (length <= hs->brest)              /* write to buffer when enough room */
     {                           /* copy length bytes from memory to buffer */
      ro0041_memcpy((char FAR *)hs->bptr,madr,length );   /* dont't need an error check */
						  /* either it works or not */
			     /* when this writing action enlarged the Store */
				       /* the Store limit has to be updated */
				   /* otherwise the storelength won't be ok */
      if ((oldpos + length) > hs->stlength)
	  hs->stlength = oldpos + length;
      hs->modify = ST_YES;                         /* set modification flag */
      hs->brest -= length;                               /* adjust pointers */
      hs->bptr += length;
      return (hs->error);                              /* everything's fine */
     }
  else             /* write directly to file when not enough room in buffer */
     {
      if (hs->fp == NULL)             /* when openmode was 0 then this will */
	  hs->modify = ST_YES;        /* force the 'stflush' to open a file */
				      /* which must be open for the 'fseek' */
      err = ro0064_stflush (hdl);
				       /* safe buffer to file because there */
			       /* could have been writing operations before */
      if (err == OK)
	 {
	  if (ro00f5_fseek (hs->fp, oldpos,  RC0080_SEEK_SET) != OK)
	     { hs->error = STFSEEKE;               /* fatal error with file */
	       return (hs->error);           /* don't do anything but leave */
	     }
	  err = ro00f3_fwrite ( madr, sizeof(char), length, hs->fp );
	  if (err == OK)              /* everything ok when writing to file */
	     {                       
			     /* when this writing action enlarged the Store */
				       /* the Store limit has to be updated */
				/* otherwise the storepos-action won't work */
	      if ((oldpos + length) > hs->stlength)
		   hs->stlength = oldpos + length;
	      ro0065_stbufread(hdl);               /* fill the buffer again */
						/* skip to the new position */
	      err=ro0066_storepos(hdl,oldpos+length);
	      if (err != OK)
		  hs->error =err;
	      return (hs->error);                     /* operation complete */
	     }
	  else
	     { hs->error = STFWRITE;
	       return (STFWRITE);                    /* error while writing */
	     }
	 }
      else                                            /* error from stflush */
	 { hs->error = err;  return (err); }
  }
/* *****************   E N D   O F   F U N C T I O N   ******************** */
}/*end-ro0068_stputm*/

/*exon **********************************************************************/
/*								            */
/* TYPE         : C-FUNCTION                                                */
/*								            */
/* NAME         : ro0069_stputs                                             */
/*								            */
/* AUTHOR       : Gerhard Jahn                                              */
/* DATE         : OCT-9th-1991                                              */
/*								            */
/* SYNTAX	:						            */

char FAR * PASCAL ro0069_stputs (
   unsigned long int hdl,
   long length)

/*								            */
/* DESCRIPTION  :   This function returns an address where the User can     */
/*                  write data to. This address lies inside the buffer.     */
/*                  <length> may not exceed the buffer-size. If an error    */
/*                  appears <NULL> is returned, else the storing address is */
/*                  returned. The read/write pointer of the buffer is       */
/*                  skipped <length> bytes forward, so that it points to    */
/*                  the first byte behind the reserved area. The content of */
/*                  the reserved area is not changed. The User has to take  */
/*                  care that he will write only <length> Bytes to the      */
/*                  store else data might be destroyed.                     */
/*								            */
/* INPUT-PARAMETERS :                                                       */
/*								            */
/*      hdl      : handle to the desired store                              */
/*      length   : number of bytes to reserve for writing                   */
/*							                    */
/* OUTPUT-PARAMETERS :   none                                               */
/*								            */
/* RETURN VALUES :					                    */
/*                                                                          */
/*      memory address inside buffer                                        */
/*								            */
/* GLOBAL DATA	 ( read	only ):	 none				            */
/*								            */
/* GLOBAL DATA	 ( modified ) :	 none				            */
/*								            */
/* REMARKS   :  The User is expected to write his data immediately          */
/*              after he' s got the address from this function.             */
/*              Otherwise data might be lost.                               */
/*inoff	*********************************************************************/

/* ******************   l o c a l   v a r i a b l e s   ******************* */
{/*entr-ro0069_stputs*/
  STORE FAR * hs;
  long  number,oldpos;
  signed short int err;
/* *****************   S T A R T   O F   F U N C T I O N   **************** */

  hs = (STORE FAR *) hdl;
  if (hs->lockhandle != hdl)                    /* check for invalid handle */
     {hs->error = STHDLERR; return (NULL);}
  if (length < 0)
     {hs->error = STLENERR; return (NULL);}      /* check for useful length */
  if (length == 0)
     {return((char FAR *)hs->bptr);}                  /* nothing else to do */
  if (length > hs->blength)               /* maximum of permitted length is */
     {hs->error = STBUEXCE; return (NULL);}         /* the length of buffer */
  if (length > hs->brest )            /* when too less bytes left in buffer */
     {                                  /* the actual buffer must be stored */
					    /* and blength data are read in */

					       /* positioning + save buffer */
					 /* absolute r/w position in buffer */
      oldpos = hs->offset + (hs->bptr-hs->base);

      err = ro0064_stflush(hdl);                 /* save the present buffer */
      if (err != OK)
	 {hs->error=err; return (NULL);}     /* fatal error in file - leave */
      hs->offset = oldpos;         /* read new buffer from old r/w position */
      number = ro0065_stbufread(hdl);
      if (number ==  RC0083_EOF)                                /* fatal file error */
	 {hs->error = STFEXCEE; return(NULL);}
      hs->brest = hs->blength-length;                 /* reset the pointers */
      hs->bptr = hs->base+length;
      hs->modify = ST_YES;      /* suppose that User will write immediately */
      return( (char FAR*) hs->base);               /* first buffer position */
     }
  else                                /* movement can be done in the buffer */
     {                               /* so just skip over the reserved area */
      hs->brest -= length;                              /* move r/w pointer */
      hs->bptr  += length;
      hs->modify = ST_YES;                         /* set modification flag */
      return ((char FAR *) (hs->bptr-length));           /* desired address */
     }
/* *****************   E N D   O F   F U N C T I O N   ******************** */
}/*end-ro0069_stputs*/

/*exon **********************************************************************/
/*								            */
/* TYPE         : C-FUNCTION                                                */
/*								            */
/* NAME         : ro0070_stgetc                                             */
/*								            */
/* AUTHOR       : Gerhard Jahn                                              */
/* DATE         : SEP-30th-1991                                             */
/*								            */
/* SYNTAX	:						            */

signed short int PASCAL ro0070_stgetc ( 
   unsigned long int hdl)

/*						                            */
/* DESCRIPTION  :   This function reads one byte from the actual r/w        */
/*                  position of the Store. The return value is either the   */
/*                  <byte> (>=0) or < RC0083_EOF> which announces an error.         */
/*								            */
/* INPUT-PARAMETERS :                                                       */
/*								            */
/*      hdl      : handle to the desired store                              */
/*								            */
/* OUTPUT-PARAMETERS :   none                                               */
/*								            */
/* RETURN VALUES :						            */
/*                                                                          */
/*       RC0083_EOF :  in case of error                                             */
/*	>=0 :  byte vale					            */
/*                                                                          */
/* GLOBAL DATA	 ( read	only ):	 none				            */
/*							                    */
/* GLOBAL DATA	 ( modified ) :	 none				            */
/*								            */
/* REMARKS : Before calling stgetc - stlength must have been updated        */
/*           by using the storelength function else data might be lost.     */
/*           This is only necessary before the first stgetc-call in a       */
/*           sequence of stgetc-calls when no other operation is            */
/*           performed in between.                                          */
/*inoff	*********************************************************************/

{/*entr-ro0070_stgetc*/
/* ******************   l o c a l   v a r i a b l e s   ******************* */
  STORE FAR * hs;
  signed short int databyte;
  long number;
  signed short int err;
/* *****************   S T A R T   O F   F U N C T I O N   **************** */

  hs = (STORE FAR *) hdl;
  if (hs->lockhandle != hdl)                    /* check for invalid handle */
     {hs->error = STHDLERR; return (STHDLERR);}

  if (hs->brest < 1)                         /* when no room left in buffer */
     {
      if (hs->modify == ST_YES)                    /* if buffer was changed */
	 {err = ro0064_stflush (hdl);                        /* safe it now */
	  if (err != OK)
	     {hs->error=err; return ( RC0083_EOF);}              /* EOF means error */
	 }               /* must be distinguished from normal return values */
				       /* no modification in present buffer */
      hs->offset += hs->blength;
      hs->bptr = hs->base;              /* adjust pointers for next buffer  */
					/* read next buffer size from store */
      number = ro0065_stbufread(hdl);

		  /* if the new buffer contains the last bytes of store and */
	      /* the end of the store lies within the buffer then it is not */
			/* allowed to set the r/w pointer beyond this limit */

      hs->brest = (number < hs->blength)? number : hs->blength;
     }
  databyte =(unsigned char) *hs->bptr;
			      /* it's not allowed to exceed the store limit */
		     /* in read mode, so the r/w pointer isn't moved across */
	       /* this border. Otherwise there would be a chance to blow up */
					       /* the store just by reading */
	   /* take care that the r/w pointer isn't moved beyond store limit */
  if ((hs->offset + hs->bptr - hs->base) < hs->stlength)
     {
       hs->bptr++;                                  /* increase r/w pointer */
       hs->brest--;
       return(databyte);
     }
  else                                               /* store limit reached */
     { hs->error = STFEXCEE;
       return( RC0083_EOF);
     }
/* *****************   E N D   O F   F U N C T I O N   ******************** */
}/*end-ro0070_stgetc*/

/*exon **********************************************************************/
/*								            */
/* TYPE         : C-FUNCTION                                                */
/*								            */
/* NAME         : ro0071_stgetm                                             */
/*								            */
/* AUTHOR       : Gerhard Jahn                                              */
/* DATE         : SEP-30th-1991                                             */
/*								            */
/* SYNTAX	:						            */

long PASCAL ro0071_stgetm ( 
   unsigned long int hdl,
   char FAR *madr,
   long length)

/*								            */
/* DESCRIPTION  :   This function reads <length> bytes from the actual r/w  */
/*                  position and copies them to the memory address <madr>.  */
/*                  The return value is either the number of copied bytes   */
/*                  or a negative value if an error has appeared.           */
/*                  If the returned number differs from the desired length  */
/*                  only a non critical error has appeared.                 */
/*								            */
/* INPUT-PARAMETERS :                                                       */
/*								            */
/*      hdl      :  handle to the desired store                             */
/*								            */
/* OUTPUT-PARAMETERS :  none                                                */
/*								            */
/* RETURN VALUES :						            */
/*                                                                          */
/*     >=0  : number of copied bytes                                        */
/*      <0  : fatal error  --> see sterror-function for details             */
/*								            */
/* GLOBAL DATA	 ( read	only ):	 none				            */
/*								            */
/* GLOBAL DATA	 ( modified ) :	 none				            */
/*								            */
/* REMARKS :    none                                                        */
/*inoff	*********************************************************************/

{/*entr-ro0071_stgetm*/
/* ******************   l o c a l   v a r i a b l e s   ******************* */
  STORE FAR * hs;
  long oldpos, rest, number;
  signed short int err;
/* *****************   S T A R T   O F   F U N C T I O N   **************** */

  hs = (STORE FAR *) hdl;
  if (hs->lockhandle != hdl)                    /* check for invalid handle */
     {hs->error = STHDLERR; return ( (long) STHDLERR);}
  if (madr == NULL)                            /* check for invalid address */
     {hs->error = STMADRER; return ( (long) STMADRER);}
  if (length < 0)                                /* check for useful length */
     {hs->error = STLENERR; return ( (long) STLENERR);}
  if (length == 0)                                         /* nothing to do */
     {return ( (long) 0);}
				      
  rest = hs->stlength - (hs->bptr - hs->base) - hs->offset;
  if (length > rest)           /* length may not exceed the length of store */
     {length = rest;               /* cut length if Store-limit is exceeded */
      hs->error = STFEXCEE; }    /* don't stop but copy as much as possible */

  if (hs->brest >= length)                    /* copy from buffer to memory */
     {
      ro0041_memcpy(madr, (char FAR *)hs->bptr, length );
      hs->bptr += length;                                 /* update pointer */
      hs->brest -= length;
      return(length);
     }
  else                                 /* read directly from file to memory */
     {              /* retain the absolute r/w position for the later fseek */
      oldpos = hs->bptr - hs->base + hs->offset;
      err=ro0064_stflush (hdl);        /* safe buffer to file because there */
			       /* could have been writing operations before */
      if (err == OK)
	 {
	  if (ro00f5_fseek (hs->fp, oldpos,  RC0080_SEEK_SET) != OK)
	     { hs->error = STFSEEKE;               /* fatal error with file */
	       return (STFSEEKE);            /* don't do anything but leave */
	     }
	  number = ro00fe_fread (madr, sizeof(char), length, hs->fp);
	  if (number == length)     /* everything ok when reading from file */
	     {		   /* As a read operation may not enlarge the Store */
		  /* like 'stputm' may do, so just skip to the new position */
	      err = ro0066_storepos(hdl,oldpos+length);
	      if (err != OK)                     /* if error in positioning */
		 { hs->error = err; return (err);}    /* operation complete */
	      else
		   return (number);      /* return the read number of bytes */
	     }
	  else
	     { hs->error = STFREADE;
	       return (STFREADE);                    /* error while writing */
	     }
	 }
      else                                            /* error from stflush */
	 {hs->error = err; return (hs->error);}
  }
/* *****************   E N D   O F   F U N C T I O N   ******************** */
}/*end-ro0071_stgetm*/

/*exon **********************************************************************/
/*								            */
/* TYPE         : C-FUNCTION                                                */
/*								            */
/* NAME         : ro0072_stgets                                             */
/*								            */
/* AUTHOR       : Gerhard Jahn                                              */
/* DATE         : OCT-9th-1991                                              */
/*								            */
/* SYNTAX	:						            */

char FAR * PASCAL ro0072_stgets ( 
   unsigned long int hdl,
   long length)

/*							                    */
/* DESCRIPTION  :   This function returns the memory address from where the */
/*                  User can read data out of the store. This address lies  */
/*                  within the buffer. <length> may not exceed the store    */
/*                  size and also not the buffer size. If an error appeared,*/
/*                  <NULL> is returned else the absolute mermory address is */
/*                  returned.                                               */
/*								            */
/* INPUT-PARAMETERS :                                                       */
/*								            */
/*      hdl      : handle to the desired store                              */
/*      length   : length of data the User wants to read from store         */
/*								            */
/* OUTPUT-PARAMETERS :   none                                               */
/*								            */
/* RETURN VALUES :						            */
/*                                                                          */
/*    NULL  : in case of error  --> see sterror for type                    */
/*    else  : memory address                                                */
/*								            */
/* GLOBAL DATA	 ( read	only ):	 none				            */
/*								            */
/* GLOBAL DATA	 ( modified ) :	 none				            */
/*								            */
/* REMARKS :    none                                                        */
/*inoff	*********************************************************************/

{/*entr-ro0072_stgets*/
/* ******************   l o c a l   v a r i a b l e s   ******************* */
  STORE FAR * hs;
  long oldpos,number;
  signed short int err;
/* *****************   S T A R T   O F   F U N C T I O N   **************** */

  hs = (STORE FAR *) hdl;
  if (hs->lockhandle != hdl)                    /* check for invalid handle */
     {hs->error = STHDLERR; return (NULL);}
			       /* length may not exceed the length of store */
  if (length < 0)
     {hs->error = STLENERR; return (NULL);}      /* check for useful length */

  if (length == 0)
     {return ((char FAR *)hs->bptr);}                 /* nothing else to do */

  if (length > (hs->stlength - ro0076_stwhere(hdl)))
     {hs->error = STFEXCEE;
      return (NULL); }                     /* error --- don't move pointers */

  if (length > hs->blength)            /* length may not exceed buffer size */
     {hs->error = STBUEXCE;
      return(NULL);  }                            /* don't move any pointer */

  if (hs->brest < length)              /* storelength exceeds actual buffer */
     {
      oldpos = hs->offset + (hs->bptr-hs->base);     /* actual r/w position */
      err = ro0064_stflush(hdl);                       /* save buffer first */
      if (err != OK)                                    /* fatal file error */
	 {hs->error=err; return (NULL);}   /* don't do anything but get out */

			   /*  update buffer start position even if stflush */
					 /* was executed in non-modify mode */
      hs->offset = oldpos;                         /* read old buffer again */
      number = ro0065_stbufread(hdl);                     /* get new buffer */
      if (number ==  RC0083_EOF)
	 {hs->error = STFEXCEE; return (NULL); }
						      /* updat pointers now */
      hs->bptr = hs->base+length;
      hs->brest = hs->blength-length;
      return ( (char FAR*) hs->base );
     }
  else                                       /* work only in present buffer */
     {
      hs->bptr  += length;                         /* skip over length area */
      hs->brest -= length;
					    /* desired address for the User */
      return ((char FAR *) (hs->bptr-length) );
     }
/* *****************   E N D   O F   F U N C T I O N   ******************** */
}/*end-ro0072_stgets*/

/*exon **********************************************************************/
/*								            */
/* TYPE         : C-FUNCTION                                                */
/*								            */
/* NAME         : ro0073_storeidle                                          */
/*								            */
/* AUTHOR       : Gerhard Jahn                                              */
/* DATE         : SEP-30th-1991                                             */
/*								            */
/* SYNTAX	:						            */

signed short int PASCAL ro0073_storeidle ( 
   unsigned long int hdl)

/*			     				                    */
/* DESCRIPTION  :   This function changes the Store-status to IDLE. This    */
/*                  means that the buffer is saved to the store, the buffer */
/*                  is freed and the file closed, but not wiped from disk.  */
/*                  The r/w pointer is set to <NULL>. The error status is   */
/*                  returned.                                               */
/*                  But if only a buffer was used until now, there will be  */
/*                  no action at all - to prevent data loss.                */
/*								            */
/* INPUT-PARAMETERS :                                                       */
/*								            */
/*      hdl      : handle to the desired store                              */
/*								            */
/* OUTPUT-PARAMETERS :  none                                                */
/*								            */
/* RETURN VALUES :						            */
/*                                                                          */
/*          actual error status                                             */
/*								            */
/* GLOBAL DATA	 ( read	only ):	 none				            */
/*								            */
/* GLOBAL DATA	 ( modified ) :	 none				            */
/*								            */
/* REMARKS :   Don't try to write to a Store which is in the IDLE           */
/*             state because a fatal memory error might appear!!            */
/*             Better try storeactive first.                                */
/*inoff	*********************************************************************/

{/*entr-ro0073_storeidle*/
/* ******************   l o c a l   v a r i a b l e s   ******************* */
  STORE FAR * hs;
  unsigned long int err;
  signed short int fail;
/* *****************   S T A R T   O F   F U N C T I O N   **************** */

  hs = (STORE FAR *) hdl;
  if (hs->lockhandle != hdl)                    /* check for invalid handle */
     {hs->error = STHDLERR; return (STHDLERR);}

/* **skh: Note: In case file is written to then stflush causes the update
          of stlength. Otherwise, the functions returns back at the
          (hs->fp == NULL) itself, in which case  stlength is not updated.
          If the next storeactive positions at a non-zero position, then it
          checks whether stlength is non-zero, which is not the case  because
          of no update !!
          Procedure: same  as in stflush !!
*/
  ro0062_storelength (hdl);                  /* at least update storelength */

   /* GJ.---------------  a double call of 'idle' should be possible

  if (hs->base == NULL && hs->bptr == NULL)      * prevent a multiple call *
     {                                           * of 'storeidle' to avoid *
      hs->error = STDBLIDL;                     * big problems with 'free' *
      return (STDBLIDL);     * this part can be removed when it is ensured *
     }                    * that no multiple 'idle' call appeared which is *
					    * actually a design problem !!*
   -------------------*/

  if (hs->base == NULL && hs->bptr == NULL)          /* double idle allowed */
      return (hs->error);
			   /* do nothing when only buffer was used 'til now */
  if (hs->fp == NULL)                          /* if no file was opened yet */
      return (hs->error);                         /* then don't do anything */

		    /* when a file was opened save buffer before freeing it */
  fail = ro0064_stflush (hdl);                     /* safe buffer into file */
  if (fail)                                      /* don't free buffer if it */
     {hs->error=fail; return (fail);}           /* couldn't be safed surely */

  err = ro0022_globalunlock(hs->bufhandle);                  /* free buffer */
  if (err)
     {hs->error = STGULOKE;}                                /* Unlock error */
  err = ro0024_globalfree  (hs->bufhandle);
  if (err)
     {hs->error = STGFREEE;}                                  /* Free error */
  hs->base      = LNULL;                    /* if base = NULL it shows that  */
  hs->bptr      = LNULL;                    /* no buffer is activated        */
  hs->bufhandle = LNULL;                    /* **skh: No buffer is activated */
  hs->brest     = hs->blength;              /* **skh: */
  hs->offset    = LNULL;                    /* **skh: */

  return(hs->error);                          /* report error status */

/* *****************   E N D   O F   F U N C T I O N   ******************** */
}/*end-ro0073_storeidle*/

/*exon **********************************************************************/
/*								            */
/* TYPE         : C-FUNCTION                                                */
/*								            */
/* NAME         : ro0074_storeactive                                        */
/*								            */
/* AUTHOR       : Gerhard Jahn                                              */
/* DATE         : SEP-30th-1991                                             */
/*								            */
/* SYNTAX	:						            */

signed short int PASCAL ro0074_storeactive ( 
   unsigned long int hdl,
   long pos)

/*								            */
/* DESCRIPTION  :   This function activates the Store again. This means     */
/*                  that the buffer is installed, the connection to the     */
/*                  disk file is reopened and the buffer is filled with the */
/*                  bytes beginning at <pos>. The error status is returned. */
/*								            */
/* INPUT-PARAMETERS :                                                       */
/*								            */
/*      hdl      : handle to the desired store                              */
/*      pos      : desired starting position of buffer                      */
/*								            */
/* OUTPUT-PARAMETERS :   none                                               */
/*								            */
/* RETURN VALUES :						            */
/*                                                                          */
/*    actual error status                                                   */
/*								            */
/* GLOBAL DATA	 ( read	only ):	 none				            */
/*								            */
/* GLOBAL DATA	 ( modified ) :	 none				            */
/*								            */
/* REMARKS :   A multiple call of this function - while the store is        */
/*             still in the ACTIVE state - doesn't change anything          */
/*inoff	*********************************************************************/

{/*entr-ro0074_storeactive*/
/* ******************   l o c a l   v a r i a b l e s   ******************* */
  STORE FAR * hs;
  signed short int err;
/* *****************   S T A R T   O F   F U N C T I O N   **************** */

  hs = (STORE FAR *) hdl;
  if (hs->lockhandle != hdl)                    /* check for invalid handle */
     {hs->error = STHDLERR; return (STHDLERR);}
  if (pos > hs->stlength || pos < 0)
     {hs->error = STPOSERR; return (STPOSERR);}         /* invalid position */
  if (hs->base == NULL)                          /* no buffer exists at all */
     {					      /* now allocating buffer area */
      hs->bufhandle = ro0020_globalalloc (hs->blength);
      if (hs->bufhandle == 0)
	 {hs->error = STGALLCE;                              /* Alloc Error */
	  return(STGALLCE);
	 }
      else                                       /* globalalloc worked well */
	 {hs->base = (unsigned  char FAR * ) ro0021_globallock (hs->bufhandle);
	  hs->bptr = hs->base;
	  if (hs->base == NULL)
	     {hs->error = STGLOCKE;                           /* Lock Error */
	      ro0024_globalfree(hs->bufhandle);              /* free buffer */
	      return(STGLOCKE);
	     }
	  hs->offset = pos;         /* fill the buffer with bytes from file */
	  ro0065_stbufread(hdl);                 /* and set the r/w pointer */
	 }
     }
  err=ro0066_storepos ( hdl, pos);  /* even if the Store is already active- */
  if (err)                            /* do the positioning, so storeactive */
      hs->error = err;                              /* works like storepos. */
  return(hs->error);
/* *****************   E N D   O F   F U N C T I O N   ******************** */
}/*end-ro0074_storeactive*/

/*exon **********************************************************************/
/*								            */
/* TYPE         : C-FUNCTION                                                */
/*								            */
/* NAME         : ro0075_deletestore                                        */
/*								            */
/* AUTHOR       : Gerhard Jahn                                              */
/* DATE         : NOV-1st-1991                                              */
/*								            */
/* SYNTAX	:						            */

signed short int PASCAL ro0075_deletestore ( 
   unsigned long int hdl)

/*								            */
/* DESCRIPTION  :   If there is no other link to this Store, this function  */
/*                  removes the Store physically. This means that both the  */
/*                  buffer and the STORE structure are released. The store  */
/*                  file on disk is removed. After this action no old store */
/*                  pointers are any longer available.                      */
/*								            */
/* INPUT-PARAMETERS :                                                       */
/*								            */
/*      hdl     : handle to the desired store                               */
/*								            */
/* OUTPUT-PARAMETERS :    none                                              */
/*								            */
/* RETURN VALUES :						            */
/*                                                                          */
/*    actual error status                                                   */
/*								            */
/* GLOBAL DATA	 ( read	only ):	 none				            */
/*								            */
/* GLOBAL DATA	 ( modified ) :	 none				            */
/*								            */
/* REMARKS :   After a use by mistake there is no chance to restore         */
/*             any data !!  So be careful.                                  */
/*inoff	*********************************************************************/

{/*entr-ro0075_deletestore*/
/* ******************   l o c a l   v a r i a b l e s   ******************* */
  STORE FAR * hs;
  unsigned long int reject;
  signed short int err;
/* *****************   S T A R T   O F   F U N C T I O N   **************** */

  hs = (STORE FAR *) hdl;
  if (hs->lockhandle != hdl)                    /* check for invalid handle */
     {hs->error = STHDLERR; return (STHDLERR);}

  if (hs->links != 0)             /* someone else is still using this store */
     {
      hs->links--;                                  /* decrease the counter */
      return (hs->error);
     }
  if (hs->fp)              /* close the Store-filestream - and because it's */
     {                            /* created by a tmpfile()-call it will be */
      err=ro0086_CloseStFile(hdl);  /* removed physically by a close()-call */
      if (err)                      /* if the file is a tmpfile .           */
	  hs->error = err;
     }
  if (hs->bnamed == TRUE)           /* remove physically a named file       */
     {                              /* added on 05/10/93 -sss               */  
     err = ro00fd_remove(hs->vnamed);
      if (err)
	  hs->error = STREMOVE;
     }

/* **skh: */
  if (hs->bufhandle) { /* **skh: */
      reject = ro0022_globalunlock(hs->bufhandle);
      if (reject)
          {hs->error = STGULOKE;}
      else {
          reject = ro0024_globalfree(hs->bufhandle);
          if (reject)
              {hs->error = STGFREEE;}
      }
  }

  err = hs->error;
  if (!err) { /* **skh: */
      reject = ro0022_globalunlock(hs->handle);		       /* free Store */
      if (reject)
          {err = STGULOKE;}
      else {
          reject = ro0024_globalfree(hs->handle);
          if (reject)
              {err = STGFREEE;}
      }
  }

  return (err);

/* *****************   E N D   O F   F U N C T I O N   ******************** */
}/*end-ro0075_deletestore*/

/*exon **********************************************************************/
/*								            */
/* TYPE         : C-FUNCTION                                                */
/*								            */
/* NAME         : ro0076_stwhere                                            */
/*								            */
/* AUTHOR       : Gerhard Jahn                                              */
/* DATE         : SEP-30th-1991                                             */
/*								            */
/* SYNTAX	:						            */

long PASCAL ro0076_stwhere ( 
   unsigned long int hdl)

/*								            */
/* DESCRIPTION  :   This function returns the absolute r/w position         */
/*                  of the store.                                           */
/*								            */
/* INPUT-PARAMETERS :                                                       */
/*								            */
/*      hdl      : handle to the desired store                              */
/*								            */
/* OUTPUT-PARAMETERS :   none                                               */
/*								            */
/* RETURN VALUES :						            */
/*                                                                          */
/*    >=0 : absolute r/w position of the store                              */
/*     <0 : error (invalid handle)                                          */
/*								            */
/* GLOBAL DATA	 ( read	only ):	 none				            */
/*								            */
/* GLOBAL DATA	 ( modified ) :	 none				            */
/*								            */
/* REMARKS :   ATTENTION !! -- As in files, counting starts with zero !     */
/*inoff	*********************************************************************/

{/*entr-ro0076_stwhere*/
/* ******************   l o c a l   v a r i a b l e s   ******************* */
  STORE FAR * hs;
  long pos;
/* *****************   S T A R T   O F   F U N C T I O N   **************** */

  hs = (STORE FAR *) hdl;
  if (hs->lockhandle != hdl)                    /* check for invalid handle */
     {hs->error = STHDLERR; return ( (long) STHDLERR);}

  pos = hs->offset + (long)(hs->bptr - hs->base);
  return(pos);
/* *****************   E N D   O F   F U N C T I O N   ******************** */
}/*end-ro0076_stwhere*/

/*exon **********************************************************************/
/*								            */
/* TYPE         : C-FUNCTION                                                */
/*								            */
/* NAME         : ro0077_stgetcp                                            */
/*								            */
/* AUTHOR       : Gerhard Jahn                                              */
/* DATE         : NOV-1st-1991                                              */
/*								            */
/* SYNTAX	:						            */

signed short int PASCAL ro0077_stgetcp ( 
   unsigned long int hdl)

/*								            */
/* DESCRIPTION  :   This function reads one byte from the actual r/w        */
/*                  position of the Store. No pointers are moved.           */
/*								            */
/* INPUT-PARAMETERS :                                                       */
/*								            */
/*      hdl      : handle to the desired store                              */
/*								            */
/* OUTPUT-PARAMETERS :    none                                              */
/*								            */
/* RETURN VALUES :						            */
/*                                                                          */
/*      >=0 : Byte-Value from the Store                                     */
/*      EOF : if error                                                      */
/*                                                                          */
/* GLOBAL DATA	 ( read	only ):	 none				            */
/*								            */
/* GLOBAL DATA	 ( modified ) :	 none				            */
/*								            */
/* REMARKS : --- stgetcp was especially installed for MAVROS ----           */
/*inoff	*********************************************************************/

{/*entr-ro0077_stgetcp*/
/* ******************   l o c a l   v a r i a b l e s   ******************* */
  STORE FAR * hs;
  unsigned char byte;
  signed short int status;
  long number;
/* *****************   S T A R T   O F   F U N C T I O N   **************** */

  hs = (STORE FAR *) hdl;
  if (hs->lockhandle != hdl)                    /* check for invalid handle */
     {hs->error = STHDLERR; return (STHDLERR);}

  ro0062_storelength ( hdl);                   /* always update storelength */
			       /* 'cause there might have been some writing */
  if (hs->brest < 1)     /* r/w position is beyond the last buffer position */
     {                         /* then read the byte directly from the file */
      if (hs->fp == NULL)          /* but read only if a file really exists */
	 return ( RC0083_EOF);
      status = ro00f5_fseek(hs->fp, hs->offset + hs->blength,  RC0080_SEEK_SET);
      if (status != OK)              /* not allowed at the end of the Store */
	  return ( RC0083_EOF);
      number = ro00fe_fread( (void *) &byte, sizeof(char), 1, hs->fp);
      if (number != 1)                               /* read exactly 1 byte */
	  return ( RC0083_EOF);
      return( (signed short int) byte);
     }
  else                                   /* get byte without pointer moving */
     {                  /* the real end of the store may not be exceeded !! */
      if ((hs->offset + (hs->bptr-hs->base)) >= hs->stlength)
	  return ( RC0083_EOF);
      else
	 {
	  byte = (unsigned char) *hs->bptr;
	  return( (signed short int) byte );
	 }
     }
/* *****************   E N D   O F   F U N C T I O N   ******************** */
}/*end-ro0077_stgetcp*/

/*exon **********************************************************************/
/*							                    */
/* TYPE         : C-FUNCTION                                                */
/*								            */
/* NAME         : ro0078_stlink                                             */
/*								            */
/* AUTHOR       : Gerhard Jahn                                              */
/* DATE         : OCT-17th-1991                                             */
/*								            */
/* SYNTAX	:						            */

int PASCAL ro0078_stlink ( 
   unsigned long int hdl)

/*								            */
/* DESCRIPTION  :   This function installs a new link to the desired store. */
/*								            */
/* INPUT-PARAMETERS :                                                       */
/*								            */
/*      hdl      : handle to the desired store                              */
/*								            */
/* OUTPUT-PARAMETERS :     none                                             */
/*								            */
/* RETURN VALUES :						            */
/*                                                                          */
/*    > 0 : number of installed links to this store                         */
/*    < 0 : error code                                                      */
/*								            */
/* GLOBAL DATA	 ( read	only ):	 none				            */
/*								            */
/* GLOBAL DATA	 ( modified ) :	 none				            */
/*								            */
/* REMARKS :   ATTENTION !! -- Before using an already installed            */
/*             Store this linking-function should be called to ensure       */
/*             that no other User can delete this Store before one          */
/*             has finished working.                                        */
/*								            */
/*inoff	*********************************************************************/

{/*entr-ro0078_stlink*/
/* ******************   l o c a l   v a r i a b l e s   ******************* */
  STORE FAR * hs;
/* *****************   S T A R T   O F   F U N C T I O N   **************** */

  hs = (STORE FAR *) hdl;
  if (hs->lockhandle != hdl)                    /* check for invalid handle */
     {hs->error = STHDLERR; return (STHDLERR);}

  hs->links++;                            /* just increase the link-counter */
  return (hs->links);
/* *****************   E N D   O F   F U N C T I O N   ******************** */
}/*end-ro0078_stlink*/

/*exon **********************************************************************/
/*								            */
/* TYPE         : C-FUNCTION                                                */
/*								            */
/* NAME         : ro0079_stcat                                              */
/*								            */
/* AUTHOR       : Gerhard Jahn                                              */
/* DATE         : OCT-18th-1991                                             */
/*								            */
/* SYNTAX	:						            */

signed short int PASCAL ro0079_stcat ( 
   unsigned long int hdl1,
   unsigned long int hdl2,
   long offset,
   long length)

/*		      						            */
/* DESCRIPTION  :   This function appends <length> bytes starting from      */
/*                  <offset> of the Store <hdl2> to the actual r/w position */
/*                  to the Store <hdl1>. The Store <hdl2> still exists      */
/*                  after the operation is completed.                       */
/*		      						            */
/* INPUT-PARAMETERS :                                                       */
/*								            */
/*      hdl1     : handle of destination Store                              */
/*      hdl2     : handle of source Store                                   */
/*      offset   : starting position within Store <hdl2>                    */
/*      length   : how many bytes shall be appended to Store <hdl1>         */
/*								            */
/* OUTPUT-PARAMETERS :     none                                             */
/*								            */
/* RETURN VALUES :						            */
/*                                                                          */
/*          0 : means no error                                              */
/*         <0 : error code                                                  */
/*								            */
/* GLOBAL DATA	 ( read	only ):	 none				            */
/*								            */
/* GLOBAL DATA	 ( modified ) :	 none				            */
/*								            */
/* REMARKS :   While Store #2 can be in <idle> mode -  Store #1 must be     */
/*             active !!                                                    */
/*inoff	*********************************************************************/

{/*entr-ro0079_stcat*/
/* ******************   l o c a l   v a r i a b l e s   ******************* */
  STORE FAR *hs;
  STORE FAR *gs;
  signed short int err;
  long wlen, number;
  char FAR *adr;
/* *****************   S T A R T   O F   F U N C T I O N   **************** */

  hs = (STORE FAR *) hdl1;
  gs = (STORE FAR *) hdl2;
					       /* check for invalid handles */
  if (hs->lockhandle != hdl1 || gs->lockhandle != hdl2)
     {hs->error = STHDLERR; return (STHDLERR);}

  if (offset < 0 || offset > gs->stlength)
     {hs->error = STPOSERR; return (STPOSERR);}           /* invalid offset */

  err = ro0074_storeactive(hdl2,offset);     /* hdl2 could be in idle state */
  if (err != OK)                             /* move to the offset position */
     {hs->error=err; return (err);}

  if (length < 0 || length > gs->stlength)
     {hs->error = STLENERR; return (STLENERR);}           /* invalid length */
  if (length == 0)                                         /* nothing to do */
      return (OK);
  while (length > 0)
	{                                            /* calc the buffersize */
	 wlen = (length < hs->blength) ? length : hs->blength;
	 adr  = ro0069_stputs (hdl1,wlen);                   /* get address */
	 if (adr == NULL)                                   /* error signal */
	     return (hs->error);                              /* error code */
	 number = ro0071_stgetm (hdl2,adr,wlen);      /* and copy wlen data */
	 if (number != wlen)
	     return (hs->error);
	 length -= wlen;
	}
  ro0062_storelength(hdl1);              /* always update the changed store */
  return (hs->error);
/* *****************   E N D   O F   F U N C T I O N   ******************** */
}/*end-ro0079_stcat*/

/*exon **********************************************************************/
/*								            */
/* TYPE         : C-FUNCTION                                                */
/*								            */
/* NAME         : ro0080_seterror                                           */
/*								            */
/* AUTHOR       : Gerhard Jahn                                              */
/* DATE         : OCT-28th-1991                                             */
/*								            */
/* SYNTAX	:						            */

signed short int PASCAL ro0080_seterror (
   unsigned long int hdl,
   signed short int error)

/*								            */
/* DESCRIPTION  :   This function sets the internal error to <error>        */
/*                  and returns the old error status.                       */
/*								            */
/* INPUT-PARAMETERS :                                                       */
/*								            */
/*      hdl      : handle to the Store                                      */
/*      error    : error that shall be set                                  */
/*								            */
/* OUTPUT-PARAMETERS :   none                                               */
/*								            */
/* RETURN VALUES :						            */
/*                                                                          */
/*      error status that was set before calling this function              */
/*								            */
/* GLOBAL DATA	 ( read	only ):	 none				            */
/*								            */
/* GLOBAL DATA	 ( modified ) :	 none				            */
/*								            */
/* REMARKS :   This function was designed for MAVROS                        */
/*								            */
/*inoff	*********************************************************************/

{/*entr-ro0080_seterror*/
/* ******************   l o c a l   v a r i a b l e s   ******************* */
  STORE FAR * hs;
  signed short int olderr;
/* *****************   S T A R T   O F   F U N C T I O N   **************** */

  hs = (STORE FAR *) hdl;
  if (hs->lockhandle != hdl)                    /* check for invalid handle */
     {hs->error = STHDLERR; return (STHDLERR);}

  olderr = hs->error;
  hs->error = error;
  return (olderr);

/* *****************   E N D   O F   F U N C T I O N   ******************** */
}/*end-ro0080_seterror*/

/*exon **********************************************************************/
/*								            */
/* TYPE         : C-FUNCTION                                                */
/*								            */
/* NAME         : ro0081_stmove                                             */
/*								            */
/* AUTHOR       : Gerhard Jahn                                              */
/* DATE         : NOV-28th-1991                                             */
/*								            */
/* SYNTAX	:						            */

signed short int PASCAL ro0081_stmove ( 
   unsigned long int hdl,
   long destend,
   long sourceend,
   long length,
   long newpos)

/*								            */
/* DESCRIPTION  :   This function copies an area of the store to an other   */
/*                  position within the store. Copying is performed from    */
/*                  the end to beginning of the area in order to avoid      */
/*                  conflicts when copying overlapping areas.               */
/*                  The only restriction on this function is, that the      */
/*                  destination-area has to lie above the source-area!      */
/*                  After the operation the r/w position is moved to        */
/*                  <newpos>.                                               */
/*								            */
/* INPUT-PARAMETERS :                                                       */
/*								            */
/*      hdl      : handle to the Store                                      */
/*	destend  : endposition, = 1 byte behind where the moved area        */
/*                 shall end                                                */
/*	sourceend: endposition of the area to be moved (also 1 byte behind  */
/*                 that area)                                               */
/*	length   : how many bytes shall be moved                            */
/*      newpos   : desired Store-position after the move-operation          */
/*								            */
/*  for example:                                                            */
/*	The <..source-area..> shall be moved to the <...dest-area...>       */
/*	in the range of the STORE by the call:				    */
/*        ro0081_stmove( hdl,destend,sourceend,length,newpos);              */
/*								            */
/*								            */
/*	  |    length     |		       |    length     |	    */
/*  ......<..source-area..>.........STORE......<...dest-area...>.........   */
/*    |                    |                                    |           */
/*    newpos               sourceend                            destend     */
/*								            */
/*								            */
/* OUTPUT-PARAMETERS :   none                                               */
/*								            */
/* RETURN VALUES :						            */
/*                                                                          */
/*      actual error status                                                 */
/*								            */
/* GLOBAL DATA	 ( read	only ):	 none				            */
/*								            */
/* GLOBAL DATA	 ( modified ) :	 none				            */
/*								            */
/* REMARKS :   This function was designed for MAVROS especially to unpack   */
/*	       constructed octet-strings                                    */
/*inoff	*********************************************************************/

{/*entr-ro0081_stmove*/
/* ******************   l o c a l   v a r i a b l e s   ******************* */
  STORE FAR * hs;
  char FAR *dptr, FAR *sptr;
  long wlen;
  signed short int status;
  int i;
  signed short int err;
/* *****************   S T A R T   O F   F U N C T I O N   **************** */

  hs = (STORE FAR *) hdl;
  if (hs->lockhandle != hdl)                    /* check for invalid handle */
     {hs->error = STHDLERR; return (STHDLERR);}
  if (destend == NULL || sourceend == NULL)
     {hs->error = STMADRER; return (STMADRER);}  /* invalid memory adresses */

  if (length < 0 || length > hs->stlength)
     {hs->error = STLENERR; return (STLENERR);}           /* invalid length */

  if (length == 0 || sourceend == destend)          /* nothing useful to do */
      return(hs->error);
				 /* first check the parameters for validity */
  if (destend   > hs->stlength || destend   < 0 ||
      sourceend > hs->stlength || sourceend < 0 ||
      sourceend > destend)             /* destend has to be above sourceend */
      return (STPOSERR);
			    /* newpos must be within the range of the store */
  if (newpos > hs->stlength || newpos < 0 )
     { hs->error = STPOSERR; return (STPOSERR); }      /* don't do anything */

  ro0062_storelength(hdl);                        /* update the real length */

  			  /* check if all can be done in the present buffer */
  if ((destend-length) >= hs->offset              /* both areas have to lie */
	&& (sourceend-length) >= hs->offset    /* within the current buffer */
	&& destend <= (hs->offset+hs->blength)
	&& sourceend <= (hs->offset+hs->blength))
     {
      dptr = (char FAR *)(hs->base + destend - hs->offset);          /* get the addresses */
      sptr = (char FAR *)(hs->base + sourceend - hs->offset);
      while (length--)
	     *(--dptr) = *(--sptr);           /* copy from buffer to buffer */
      hs->modify = ST_YES;                     /* buffer was really changed */
			/* and has to be saved before a new one can be read */
     }
  else           /* movement exceeds the buffer and is done within the file */
     {
       err= ro0064_stflush (hdl);                      /* first safe buffer */
       if (err != OK)
	  {hs->error=err; return (hs->error);}
       i=0;                             /* counts number of written buffers */
       while (length > 0)
	  {                           /* default copysize is the buffersize */
						    /* except the last time */
	   wlen = (length < hs->blength) ? length : hs->blength;
			    /* calc from where the next bytes shall be read */
	   hs->offset = sourceend - (i*hs->blength) - wlen;
					      /* read the bytes into buffer */
	   if (ro0065_stbufread(hdl) != hs->blength)
	       return (hs->error);
				/* seek for the right position in the Store */
	   status = ro00f5_fseek(hs->fp,destend-(i*hs->blength)-wlen, RC0080_SEEK_SET);
	   if (status != OK)
	      {hs->error = STFSEEKE; return (STFSEEKE); }
	   err = ro00f3_fwrite(hs->base,sizeof(char),wlen,hs->fp);
	   if (err != OK)
	      {hs->error = STFWRITE; return(STFWRITE); }
	   length -= wlen;
	   i++;                                              /* next buffer */
	  }
     }
  if (hs->modify == ST_YES)                        /* if buffer was changed */
     {err = ro0064_stflush (hdl);                            /* safe it now */
      if (err != OK)
	 {hs->error=err; return (hs->error);}
     }
  if (hs->offset != newpos)                   /* when i'm not already there */
     {
      hs->offset = newpos;               /* move to desired target position */
      ro0065_stbufread(hdl);
     }
  hs->bptr = hs->base;                                /* adjust r/w pointer */
  ro00f6_fflush(hs->fp);                 /* force to clear internal buffers */
  return (hs->error);

/* *****************   E N D   O F   F U N C T I O N   ******************** */
}/*end-ro0081_stmove*/

/*exon **********************************************************************/
/*								            */
/* TYPE         : C-FUNCTION                                                */
/*								            */
/* NAME         : ro0082_FileToStore                                        */
/*								            */
/* AUTHOR       : Gerhard Jahn                                              */
/* DATE         : NOV-28th-1991                                             */
/*								            */
/* SYNTAX	:						            */

unsigned long int PASCAL ro0082_FileToStore ( 
   char FAR * name,
   long buflen)

/*								            */
/* DESCRIPTION  :   This function creates a Store out of the file <name>,   */
/*                  which must already exist on disk. All Store operations  */
/*                  can then be used on this file. After the deletesore-    */
/*                  command only the file still exists.                     */
/*                  Note : deletestore function modified to remove named    */
/*                          files using remove() files.                     */
/*								            */
/* INPUT-PARAMETERS :                                                       */
/*								            */
/*      name     : Name of the file on disk that shall be converted         */
/*                 to a Store                                               */
/*      buflen   : size of buffer (will truncated if too long)                 */
/*								            */
/* OUTPUT-PARAMETERS :   none                                               */
/*								            */
/* RETURN VALUES :						            */
/*                                                                          */
/*      >0 : the Handle to the 'new' Storefile                              */
/*       0 : if an error appeared                                                 */
/*								            */
/* GLOBAL DATA	 ( read	only ):	 none				            */
/*								            */
/* GLOBAL DATA	 ( modified ) :	 none				            */
/*								            */
/* REMARKS :   This function was designed for MAVROS                        */
/*								            */
/*inoff	*********************************************************************/

{/*entr-ro0082_FileToStore*/
/* ******************   l o c a l   v a r i a b l e s   ******************* */
  STORE FAR * hs;
  unsigned long int hdl;
  signed short int err;
  signed short int status;
/* *****************   S T A R T   O F   F U N C T I O N   **************** */

  if (buflen < -1)                                  /* invalid bufferlength */
      return(0);
					  /* first create an ordinary Store */
						 /* without a file creation */
  hdl = ro0060_newstore( buflen, 0, (signed short int*) &err);  
  if (err != OK)
      return (err);                           /* fatal error -> don't go on */
  hs = (STORE FAR *) hdl;
  if (hs->lockhandle != hdl)                    /* check for invalid handle */
     {hs->error = STHDLERR; return (0);}
  hs->fp = ro00f1_fopen(name,"r+");  /* the filepointer to the desired file */
  if (hs->fp == NULL)                         /* no connection to that file */
     {hs->error = STFOPENE; return (0L);}
  hs->bnamed = TRUE;                           /* set bnamed to true because*/
                                               /* file is a named one.      */
  ro0050_strcpy (hs->vnamed,name);             /* copy file name to store   */
                                               /* structure                 */
  status = ro00f5_fseek(hs->fp,0, RC0082_SEEK_END);   /* move to the fileend */
  hs->stlength = ro00f7_ftell(hs->fp);      /* that's the length of the Store */
  if (status != OK || hs->stlength < 0)
     {hs->error = STFSEEKE; return (0L);}
  err= ro0066_storepos(hdl,0L);       /* read the first bytes to the buffer */
  if (err != OK)
      hs->error=err;
  ro0065_stbufread(hdl);
  return (hdl);

/* *****************   E N D   O F   F U N C T I O N   ******************** */
}/*end-ro0082_FileToStore*/

/*exon **********************************************************************/
/*								            */
/* TYPE         : C-FUNCTION                                                */
/*								            */
/* NAME         : ro0083_stbufresize                                        */
/*								            */
/* AUTHOR       : Gerhard Jahn                                              */
/* DATE         : FEB-24th-1992                                             */
/*								            */
/* SYNTAX	:						            */

long   PASCAL ro0083_stbufresize ( 
   unsigned long int hdl)

/*								            */
/* DESCRIPTION  :   This function resizes the internal store buffer if      */
/*                  no data were written to file and the buffer is filled   */
/*                  less than half. If these conditions are true, the new   */
/*                  buffersize will be the number of data in the old buffer.*/
/*								            */
/* INPUT-PARAMETERS :                                                       */
/*								            */
/*      hdl      : handle to the Store                                      */
/*								            */
/* OUTPUT-PARAMETERS :   none                                               */
/*								            */
/* RETURN VALUES :						            */
/*                                                                          */
/*      >=0 : The (may be) truncated new buffersize                         */
/*       <0 : Type of Error                                                                 */
/*								            */
/* GLOBAL DATA	 ( read	only ):	 none				            */
/*								            */
/* GLOBAL DATA	 ( modified ) :	 none				            */
/*								            */
/* REMARKS :   This function was designed for a better use of memory        */
/*								            */
/*inoff	*********************************************************************/

{/*entr-ro0083_stbufresize*/
/* ******************   l o c a l   v a r i a b l e s   ******************* */
  STORE FAR * hs;
  unsigned long int bhdl,dist;
  char FAR * nptr;
/* *****************   S T A R T   O F   F U N C T I O N   **************** */

  hs = (STORE FAR *) hdl;
  if (hs->lockhandle != hdl)                    /* check for invalid handle */
     {hs->error = STHDLERR; return (STHDLERR);}
  ro0062_storelength(hdl);             /* better update the real size first */
  if (hs->stlength < hs->blength)         /* only do the resize if no bytes */
     {                                     /* were written to the storefile */
      dist = hs->bptr - hs->base;               /* store this for later use */
      if ( ro0022_globalunlock (hs->bufhandle))
	 { hs->error = STGULOKE; return (STGULOKE); }
      bhdl = ro0023_globalrealloc ( hs->bufhandle, hs->blength, hs->stlength );
      if ( bhdl <= 0 )                                /* error with realloc */
	 { hs->error = STGRALLE; return (STGRALLE); }
      nptr = ro0021_globallock (bhdl);
      if ( nptr == NULL )
	 { hs->error = STGLOCKE; return (STGLOCKE); }
					/* realloc worked fine - now update */
					     /* internal control structures */
      hs->base = (unsigned char FAR *)nptr;                   /* where the new buffer starts now */
      hs->bufhandle = bhdl;               /* now we have a different handle */
      hs->blength = hs->stlength;       /* that's the reason for the change */
      hs->bptr = hs->base + dist;               /* r/w position not changed */
      hs->brest = hs->blength - dist;          /* the buffer is smaller now */
     }
  return (hs->blength);                        /* return the new buffersize */

/* *****************   E N D   O F   F U N C T I O N   ******************** */
}/*end-ro0083_stbufresize*/

/*exon **********************************************************************/
/*								            */
/* TYPE         : C-FUNCTION                                                */
/*								            */
/* NAME         : ro0084_stStoreResize                                      */
/*								            */
/* AUTHOR       : Gerhard Jahn                                              */
/* DATE         : MAR-5th-1992                                              */
/*								            */
/* SYNTAX	:						            */

long   PASCAL ro0084_stStoreResize ( 
   unsigned long int hdl,
   long newsize)

/*								            */
/* DESCRIPTION  :   This function truncates the Store after a previous      */
/*                  stgets/stputs-call to the value of <newsize>.           */
/*								            */
/* INPUT-PARAMETERS :                                                       */
/*								            */
/*      hdl      : handle to the Store                                      */
/*      newsize  : Desired length of truncated Store                        */
/*                 this <newsize>-Position has to be inside                 */
/*                 the current buffer and may not exceed the 'old'          */
/*                 storelength!!                                            */
/*								            */
/* OUTPUT-PARAMETERS :   none                                               */
/*								            */
/* RETURN VALUES :						            */
/*                                                                          */
/*      >=0 : The truncated new storesize                                   */
/*       <0 : Type of Error                                                 */
/*								            */
/* GLOBAL DATA	 ( read	only ):	 none				            */
/*								            */
/* GLOBAL DATA	 ( modified ) :	 none				            */
/*								            */
/* REMARKS :   Attention!!                                                  */
/*             This function provides a good behaviour only in a special    */
/*             circumstance:  After one or more stgets/stputs were executed */
/*             but no other storefunction were called in between the        */
/*             'stgets/stputs' and the 'stStoreResize' function ! ---       */
/*             Otherwise the whole store might lose its data !!!            */
/*	       So be well aware where you call this function                */
/*								            */
/*inoff	*********************************************************************/

{/*entr-ro0084_stStoreResize*/
/* ******************   l o c a l   v a r i a b l e s   ******************* */
  STORE FAR * hs;
  long dist;
/* *****************   S T A R T   O F   F U N C T I O N   **************** */

  hs = (STORE FAR *) hdl;
  if (hs->lockhandle != hdl)                    /* check for invalid handle */
     {hs->error = STHDLERR; return (STHDLERR);}
  ro0062_storelength(hdl);             /* better update the real size first */
				       /* for this is a dangerous change    */
				       /* first check the newsize-parameter */
				       /* this means that <newsize> must be */
				       /* within the current buffer range   */
  if ((newsize < hs->offset) || (newsize > hs->stlength))
     { hs->error = STNWSZER; return (STNWSZER); }
  hs->stlength = newsize;                       /* truncate the storelength */
  dist = newsize - hs->offset;         /* this will be the new r/w poistion */
  hs->bptr = hs->base + dist;                       /* update the positions */
  hs->brest = hs->blength - dist;
  return ( hs->stlength );                      /* return the new storesize */

/* *****************   E N D   O F   F U N C T I O N   ******************** */
}/*end-ro0084_stStoreResize*/

/*exon **********************************************************************/
/*								            */
/* TYPE         : C-FUNCTION                                                */
/*								            */
/* NAME         : ro0085_OpenStFile                                         */
/*								            */
/* AUTHOR       : Gerhard Jahn                                              */
/* DATE         : MAY-26th-1992                                             */
/*								            */
/* SYNTAX	:						            */

signed short int PASCAL ro0085_OpenStFile (
   unsigned long int hdl)

/*								            */
/* DESCRIPTION  :   This function creates a temporary storefile.            */
/*                  The file will be removed if either the process or the   */
/*                  program ends or the Close-Fct is called.                */
/*								            */
/* INPUT-PARAMETERS :                                                       */
/*                                                                          */
/*         hdl : Store-Handle                                               */
/*								            */
/* OUTPUT-PARAMETERS :   none                                               */
/*								            */
/* RETURN VALUES :						            */
/*                                                                          */
/*       0   : If the storefile could be installed successfully.            */
/*    <> 0   : Type of error that appeared during open-procedure.           */
/*								            */
/* GLOBAL DATA	 ( read	only ):	 none				            */
/*								            */
/* GLOBAL DATA	 ( modified ) :	 none				            */
/*								            */
/* REMARKS :  The descriptor is the only connection to the temporary file!  */
/*								            */
/*inoff	*********************************************************************/

{/*entr-ro0085_OpenStFile*/
/* ******************   l o c a l   v a r i a b l e s   ******************* */
  STORE FAR * hs;
/* *****************   S T A R T   O F   F U N C T I O N   **************** */

  hs = (STORE FAR *) hdl;
  if (hs->lockhandle != hdl)                   /* check for invalid handle */
     {hs->error = STHDLERR; return (STHDLERR);}

/* **skh:
  hs->fp = ro00fd_tmpfile();                   * create a temporary storefile *
  hs->bnamed = FALSE;                          * set bnamed to false  because * 
                                               * file is a temp file.         * 
*/

                                              /* create a temporary storefile */
  hs->fp = ro00ff_openTmpFile ((signed short int  FAR *)&hs->bnamed,
                               (signed         char FAR *)hs->vnamed);
  if (hs->fp == NULL)                                        
     { 
      hs->error = STFOPENE; 
      return (STFOPENE);
     }
  else
      return (OK);

/* *****************   E N D   O F   F U N C T I O N   ******************** */
}/*end-ro0085_OpenStFile*/

/*exon **********************************************************************/
/*								            */
/* TYPE         : C-FUNCTION                                                */
/*								            */
/* NAME         : ro0086_CloseStFile                                        */
/*								            */
/* AUTHOR       : Gerhard Jahn                                              */
/* DATE         : MAY-26th-1992                                             */
/*								            */
/* SYNTAX	:						            */

signed short int PASCAL ro0086_CloseStFile ( 
   unsigned long int hdl)

/*								            */
/* DESCRIPTION  :   This function closes the storefile logically.           */
/*                  According to the ANSI description the file is removed.  */
/*								            */
/* INPUT-PARAMETERS :                                                       */
/*                                                                          */
/*         hdl : Store-Handle                                               */
/*								            */
/* OUTPUT-PARAMETERS :   none                                               */
/*								            */
/* RETURN VALUES :						            */
/*                                                                          */
/*       0   : If the storefile was closed successfully.                    */
/*    <> 0   : If an error appeared during the close-procedure.             */
/*								            */
/* GLOBAL DATA	 ( read	only ):	 none				            */
/*								            */
/* GLOBAL DATA	 ( modified ) :	 none				            */
/*								            */
/* REMARKS :  If the close-action was performed successfully,               */
/*            the file descriptor is set to NULL.                           */
/*            Remember that under DOS the number of simultaiously opened    */
/*            files is limited (-->FILES).                                  */
/*inoff	*********************************************************************/

{/*entr-ro0086_CloseStFile*/
/* ******************   l o c a l   v a r i a b l e s   ******************* */
  STORE FAR * hs;
/* *****************   S T A R T   O F   F U N C T I O N   **************** */

  hs = (STORE FAR *) hdl;
  if (hs->lockhandle != hdl)                    /* check for invalid handle */
     {hs->error = STHDLERR; return (STHDLERR);}

  if (hs->fp == NULL)               /* if no file exists - the call to this */
      return (STFNOTOP);           		 /* function is not allowed */
  else
     {
      if (ro00f2_fclose(hs->fp))                            /* close the storefile */
	  return (STFCLOSE);
      hs->fp = NULL;                               /* forget the descriptor */
      return (OK);
     }

/* *****************   E N D   O F   F U N C T I O N   ******************** */
}/*end-ro0086_CloseStFile*/


/*exon **********************************************************************/
/*								            */
/* TYPE         : C-FUNCTION                                                */
/*								            */
/* NAME         : ro0087_StoreToFile                                        */
/*								            */
/* AUTHOR       : Shanbhag, Sanjay.                                         */
/* DATE         : 93-05-10                                                  */
/*								            */
/* SYNTAX	:						            */

signed short int PASCAL ro0087_StoreToFile (
   unsigned long int hdl,
   char FAR * name)

/*								            */
/* DESCRIPTION  :   This function converts a given store in to a named file */
/*                  The named file is created a per the filename returned   */
/*                  by the tmpnam() function.This is returned in the char   */
/*                  pointer 'name.'                                         */
/*								            */
/* INPUT-PARAMETERS :                                                       */
/*								            */
/*      hdl      : handle to the desired store                              */
/*      name     : A char pointer which will point to the file name after   */
/*                  the function returns.                                   */
/*								            */
/* OUTPUT-PARAMETERS :   none                                               */
/*								            */
/* RETURN VALUES :						            */
/*							                    */
/*       =0 : successfull conversion.                                       */
/*       <0 : error status                                                  */
/*								            */
/* GLOBAL DATA	 ( read	only ):	 none				            */
/*								            */
/* GLOBAL DATA	 ( modified ) :	 none				            */
/*								            */
/* REMARKS		      :	 none				            */
/*								            */
/*inoff	*********************************************************************/

/* ******************   l o c a l   v a r i a b l e s   ******************* */
{/*entr-ro0087_StoreToFile*/
  STORE FAR * hs;
  char FAR  *ptr;
  unsigned long int ulFd;
  long int lToBeCopied;
  long int lCopy;
  signed short int  rc = 0;
/* *****************   S T A R T   O F   F U N C T I O N   **************** */

  hs = (STORE FAR *) hdl;
  if (hs->lockhandle != hdl)                    /* check for invalid handle */
      return ( STHDLERR);

  if ((rc = ro00fc_tmpnam(name)) != 0)
     {hs->error = STTMPNME; return (STTMPNME);}

  ulFd = ro00f1_fopen(name,"w+");     /* the filepointer to the desired file */

  if (ulFd == NULL)                         /* no connection to that file */
     {hs->error = STFOPENE; return (STFOPENE);}
  
  ro0062_storelength (hdl);                  /* at least update storelength */

  lToBeCopied = hs->stlength ;
  rc = ro0066_storepos(hdl,0L);       /* read the first bytes to the buffer */
  if (rc != OK)
     {hs->error = rc; return (rc);}
  do {
     lCopy = (hs->blength < lToBeCopied) ? hs->blength : lToBeCopied;
     ptr = ro0072_stgets(hdl,lCopy);
     if (ptr == NULL) break;
     rc = ro00f3_fwrite(ptr,lCopy,1,ulFd);
     if (rc != OK)
        {hs->error = STFWRITE; break;}
     lToBeCopied -= lCopy;
  } while (lToBeCopied);
  rc = ro00f2_fclose(ulFd);
  if (rc != OK)
     hs->error = STFCLOSE; 
  if (hs->error != OK)
      return(hs->error);
  else
      return(0);
/* *****************   E N D   O F   F U N C T I O N   ******************** */
}/*end-ro0087_StoreToFile*/

/*exon **********************************************************************/
/*								            */
/* TYPE         : C-FUNCTION                                                */
/*								            */
/* NAME         : ro0088_StoreToLog                                         */
/*								            */
/* AUTHOR       : Chandrasekar.K                                            */
/* DATE         : 04-11-93                                                  */
/*								            */
/* SYNTAX	:						            */

signed short int PASCAL ro0088_StoreToLog (
   unsigned long int hdl)

/*								            */
/* DESCRIPTION  :   This function writes the STORE contents to LOG File     */
/*								            */
/* INPUT-PARAMETERS :                                                       */
/*								            */
/*      hdl      : handle to the desired store                              */
/*								            */
/* OUTPUT-PARAMETERS :   none                                               */
/*								            */
/* RETURN VALUES :						            */
/*							                    */
/*       =0 : successfull conversion.                                       */
/*       <0 : error status                                                  */
/*								            */
/* GLOBAL DATA	 ( read	only ):	 none				            */
/*								            */
/* GLOBAL DATA	 ( modified ) :	 none				            */
/*								            */
/* REMARKS		      :	 none				            */
/*								            */
/*inoff	*********************************************************************/

/* ******************   l o c a l   v a r i a b l e s   ******************* */
{/*entr-ro0088_StoreToLog*/
  STORE FAR * hs;
  char FAR  *ptr;
  long int lToBeCopied;
  long int lCopy;
  signed short int  rc = 0;
  signed long  int  lLen=0;

  static signed long int lPduLen = 0;
/* *****************   S T A R T   O F   F U N C T I O N   **************** */

  hs = (STORE FAR *) hdl;
  if (hs->lockhandle != hdl)                  /* check for invalid handle */
      return ( STHDLERR);

  rc = ro0074_storeactive(hdl,0);
  if (rc != OK) return(rc);

                                               /* at least update storelength */
  lLen=ro0062_storelength (hdl);

  if (lPduLen == 0 )
      lPduLen = ro0058_readPduLenEnv();
      
  lToBeCopied = (lPduLen < hs->stlength) ? lPduLen : hs->stlength ;

  rc = ro0066_storepos(hdl,0L);       /* read the first bytes to the buffer */
  if (rc != OK)
     {hs->error = rc; return (rc);}
 /*
  lCopy = (hs->blength < lToBeCopied) ? hs->blength : lToBeCopied;
  ptr = ro0072_stgets(hdl,lCopy);
  if(ptr == NULL) lCopy = NULL;
  */ 
  rc = ro0043_pack10Log ( RC0022_ACTIVITY,
                         RL0403_PduInfo,
                         1,
                         (signed short int)sizeof(signed long int),
                         (char FAR *) &lLen);
  
  if (rc != OK ) return (rc);
  /* rc = ro0042_writeLog   (RC0022_ACTIVITY,
                          RL0402_PduLog,
                         (signed short int)lCopy,
                         (char FAR *)ptr);  */

   rc = ro0046_writeHeader   (RC0022_ACTIVITY,
                          RL0402_PduLog,
                         (signed short int)lToBeCopied);
 /*  lToBeCopied -= lCopy; */
  
 /* After the Header simply append to that Header in the log file */ 
  while(lToBeCopied){
     lCopy = (hs->blength < lToBeCopied) ? hs->blength : lToBeCopied;
     ptr = ro0072_stgets(hdl,lCopy);
     if (ptr == NULL) break;
     /* by kc on 16-11-93 for testing */
     rc = ro0044_AppendLog(RC0022_ACTIVITY,lCopy,(char FAR *)ptr); 
/*
     rc = ro0042_writeLog   (RC0022_ACTIVITY,
                          RL0402_PduLog,
                         (signed short int)lCopy,
                         (char FAR *)ptr); */

     if (rc != OK)
        {hs->error = STFWRITE; break;}
     lToBeCopied -= lCopy;
  } ;
  if (hs->error != OK)
      return(hs->error);
  else
      return(0);
/* *****************   E N D   O F   F U N C T I O N   ******************** */
}/*end-ro0088_StoreToLog*/
/* ******************    E N D   O F   M O D U L E    ********************* */

