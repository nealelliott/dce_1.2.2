/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: routil8.c,v $
 * Revision 1.1.4.2  1996/03/11  13:20:19  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:20  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:33:05  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:18:41  root]
 * 
 * Revision 1.1.2.5  1994/08/24  08:35:26  marrek
 * 	Fix warnings. (OT 11824)
 * 	[1994/08/23  14:34:42  marrek]
 * 
 * Revision 1.1.2.4  1994/08/22  15:21:50  keutel
 * 	OT 11620
 * 	[1994/08/19  13:48:28  keutel]
 * 
 * Revision 1.1.2.3  1994/06/10  21:08:18  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:50:10  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:59:54  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:55:53  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:34:47  keutel
 * 	creation
 * 	[1994/03/21  13:25:31  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: routil8.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:20:19 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)  1991 BY SIEMENS NIXDORF INFORMATIONS AG	      */
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
/* TYPE         : MODULE                                              */
/*                                                                    */
/* NAME         : routil8.c                                           */
/*                                                                    */
/* AUTHOR       : Hubert Trieb                                        */
/* DATE         : 92-03-23                                            */
/*                                                                    */
/* COMPONENT    : RTS  store utility functions                        */
/*                                                                    */
/* PRD#/VERS.	: RTS-V02.00					      */
/*                                                                    */
/* DESCRIPTION  : This modul provides functions which mapps the       */
/*                C - filefunctions to utility functions              */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  |mark| CR# EM# */
/* V02.00 K0| 92-03-23|  original                      | ht |         */
/*          | 92-05-04|  file lock functions inserted  | ht |         */
/*          | 93-05-10|  New file handling functions   |sss |         */
/*          |         |  such as tmpnam/remove etc.    |    |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                     Module Identification                          */
/**********************************************************************/

static char *sccsid =    "@(#) routil8.c 1.2 93/12/20   RTS-V2.00";

/*********************************************************************/
/*              I N C L U D E S                                      */
/*********************************************************************/

#ifndef  RP0000_MicV0600  /* for compilation on dos -sss 14/06/93  */

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <stdio.h>
#include <errno.h>
#include <unistd.h>    
#include <rogenrl.h>	 /* RTS general definitions		     */
#include <roerval.h>
#include <routil0.h>

signed short int PASCAL ro00fl_fgets (
    char    FAR *    pStr,
    signed int       Num,
    unsigned long int fp);

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro00f1_fopen                                        */
/*                ro00f2_fclose                                       */
/*                ro00f3_fwrite                                       */
/*                ro00f4_freadAndLenChk                               */
/*                ro00f5_fseek                                        */
/*                ro00f6_fflush                                       */
/*                ro00f7_ftell                                        */
/*                ro00f8_tmpfile                                      */
/*                ro00fc_tmpnam                                       */
/*                ro00fd_remove                                       */
/*                ro00fe_fread                                        */
/*                ro00ff_openTmpFile                                  */
/*                ro00fk_feof                                         */
/*                ro00fl_fgets                                        */
/*                                                                    */
/* AUTHOR       : Hubert Trieb , Chandrasekar.K                       */
/* DATE         : 92-02-20                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
/*                                                                    */
/* DESCRIPTION  : mapping file access to standard C functions         */
/*                                                                    */
/* IN-PARAMETERS:             : same as standard C functions except   */
/*                              the parameter of type FILE pointer,   */
/*                              which is used as handle.              */
/*                                                                    */
/* RETURN VALUES              : errors mapped to values in roerval.h  */
/*                                                                    */
/*                                                                    */
/*inoff ***************************************************************/

unsigned long int PASCAL ro00f1_fopen (
	char  FAR  *fname,
	char  FAR  *fmode)
{
    FILE	*fp;
    if ((fp = fopen(fname,fmode)) != (FILE *)NULL)
	setbuf(fp, (char *) NULL) ;
    return( (unsigned long int) fp);
}


signed short int  PASCAL ro00f2_fclose(
	unsigned long int   fp)
{
    signed short int rc;
    if  ((rc = fclose( (FILE *) fp)) != 0)
	rc = -R_FCLERR;
    return (rc);
}

signed short int  PASCAL ro00f3_fwrite(
	void     FAR       *  ptr,
	unsigned long int     size,
	unsigned long int     numb,
	unsigned long int     fp)
{

    signed short int rc = 0;
    unsigned long int writen;

    writen = fwrite(ptr,size,numb,(FILE *)fp);
/* **** Error commented by kc on 3/5/93 
    if ( writen != size * numb)
 end of comment */
    if (writen  != numb )
	rc = -R_FWRERR;
    return (rc);
}

/* **************************************************************** */
/* The name of the function changed to ro00f4_freadAndLenChk from   */
/* ro00f4_fread.A new function  ro00fe_fread added to return the    */
/* number of elements read. -sss 10/05/93.                          */
/* **************************************************************** */

signed short int  PASCAL ro00f4_freadAndLenChk(
	void     FAR       *  ptr,
	unsigned long int     size,
	unsigned long int     numb,
	unsigned long int     fp)
{
    signed short int rc = 0;
    unsigned long int read;

    read = fread(ptr,size,numb,(FILE *)fp);
    if ( read != numb)
	rc = -R_FREERR;
    return (rc);
}

signed short int PASCAL ro00f5_fseek(
	unsigned long int   fp,
	long int            offset,
	int                 ptrname)
{
    int mapptr;
    signed short int rc;

    if      (ptrname == RC0080_SEEK_SET) mapptr = SEEK_SET;
    else if (ptrname == RC0081_SEEK_CUR) mapptr = SEEK_CUR;
    else if (ptrname == RC0082_SEEK_END) mapptr = SEEK_END;
    else return (-R_FFLERR);

    if ((rc = fseek((FILE *)fp,offset,mapptr)) != 0)
	rc = R_FSEERR;
    return (rc);
}

signed short int  PASCAL ro00f6_fflush(
	unsigned long int   fp)
{
    signed short int rc;
    if ((rc = fflush((FILE*)fp)) != 0) rc = -R_FFLERR;
    /* sync();  not needed after flush  warning by GCC for PROTO */
    return (rc);
}

long int  PASCAL ro00f7_ftell(
	unsigned long int   fp)
{
    return (ftell((FILE *)fp));
}

unsigned long int PASCAL ro00f8_tmpfile(void)
{
    return( (unsigned long int) tmpfile()); 
}

signed short int PASCAL ro00fc_tmpnam(
	char FAR * filename)
{
    signed short int rc = 0;
    if (tmpnam(filename) == NULL) rc = -R_FNMERR;
    return (rc);
}

signed short int  PASCAL ro00fd_remove(
	char FAR * filename)
{
    signed short int rc;
    if ((rc = remove(filename)) != 0) rc = -R_FRMERR;
    return (rc);
}

unsigned long int PASCAL ro00ff_openTmpFile (
	signed short int   FAR  * pNamed,
	signed       char  FAR  * pFileName)
{

    pFileName [0] = '\0';
    *pNamed       = FALSE;
    return (ro00f8_tmpfile());
}

signed short int PASCAL ro00fk_feof (
     unsigned long int fp)
{
 return ( (signed short int) feof ( (FILE *) fp));
}

/* returns 0 on success Non zero on eof or failure */
signed short int PASCAL ro00fl_fgets (
    char    FAR *    pStr,
    signed int       Num,
    unsigned long int fp)
{
   if (fgets (pStr, Num , (FILE *) fp ) != NULL )
      return (0);
   else return (1);
}
/* **************************************************************** */
/* this function added to return the actual bytes read as required  */
/* in the store functions. -sss 10/05/93                            */
/* **************************************************************** */

unsigned long int  PASCAL ro00fe_fread(
	void     FAR       *  ptr,
	unsigned long int     size,
	unsigned long int     numb,
	unsigned long int     fp)
{
    unsigned long int read;

    read = fread(ptr,size,numb,(FILE *)fp);
    return (read);
}


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         :                                                     */
/*                ro00fa_flock                                        */
/*                ro00fb_funlock                                      */
/*                                                                    */
/* AUTHOR       : Hubert Trieb                                        */
/* DATE         : 92-05-04                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
/*                                                                    */
/* DESCRIPTION  : Functions for locking and unlocking files           */
/*                ro00fa_flock :                                      */
/*                Locks a file indicated by the file handle           */
/*                for the calling process.                            */
/*                After a file is locked by a process, further calls  */
/*                to ro00fa_flock of other processes are waiting      */
/*                untill all locks are removed before they return.    */
/*                                                                    */
/*                ro00fb_funlock :                                    */
/*                removes all locks of the calling process to the     */
/*                file indicated by the file handle.                  */
/*                                                                    */
/*                After locking or unlocking a file the position      */
/*                pointer for reading and writing within the file     */
/*                is set to NULL.                                     */
/*                                                                    */
/*                All locks of a process are removed after the process*/
/*                terminates regulary, or is stopped (killed) by the  */
/*                system.                                             */
/*                                                                    */
/* IN-PARAMETERS: handle to a FILE pointer returned by ro00f1_fopen.  */
/*                                                                    */
/* RETURN VALUES              : errors mapped to values in roerval.h  */
/*                                                                    */
/*                                                                    */
/*inoff ***************************************************************/

signed short int PASCAL  ro00fa_flock(
	unsigned long int   hfp)
{
    long int            offset;
    signed short int    rc;
    int                 err;
    FILE              * fp;

    fp = (FILE *) hfp;
    offset = 0;                            /* used for fseek and lockf     */
    ro00f5_fseek(hfp,                      /* positionize to begin of file */
		 offset,
		 RC0080_SEEK_SET);

    err = lockf(fp->_file,          /* lockf must be called with         */
				    /* unix file descriptor              */
	       F_LOCK,
	       offset);             /* lock to end of file; offset = 0   */

    if (err != 0) rc = -R_FLOCKE;
    else rc = 0;

    return (rc);
}

signed short int PASCAL ro00fb_funlock(
	unsigned long int   hfp)
{
    long int            offset;
    signed short int    rc;
    int                 err;
    FILE              * fp;

    fp = (FILE *) hfp;

    offset = 0;                            /* used for fseek and lockf     */
    ro00f5_fseek(hfp,                    /* positionize to begin of file */
		 offset,
		 RC0080_SEEK_SET);

    err = lockf(fp->_file,          /* lockf must be called with         */
				    /* unix file descriptor              */
	       F_ULOCK,
	       offset);             /* unlock to end of file; Offset = 0 */

    if (err != 0) rc = -R_FULOCK;
    else rc = 0;

    return (rc);
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         :                                                     */
/*                ro00ff_Shfopen                                      */
/*                ro00fg_Shfread                                      */
/*                ro00fi_Shfwrite                                     */
/*                ro00fj_Shfclose                                     */
/*                                                                    */
/* AUTHOR       : Shivkumar                                           */
/* DATE         : 93-11-09                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
/*                                                                    */
/* DESCRIPTION  : Functions for shared operations on files.           */
/*                ro00ff_Shfopen  :                                   */
/*                                                                    */
/* IN-PARAMETERS:                                                     */
/*                                                                    */
/* RETURN VALUES:               errors mapped to values in roerval.h  */
/*                                                                    */
/*                                                                    */
/*inoff ***************************************************************/

signed short int PASCAL ro00ff_Shfopen (
	char                     FAR * lpszFileName,
	char                     FAR * fuMode,
	unsigned long int        FAR * phFileHandle,
	void              FAR *  FAR * lpOpenBuff)
{
    *lpOpenBuff = LNULL;

    *phFileHandle = (unsigned long int)ro00f1_fopen (lpszFileName,
                                                     (char FAR *)fuMode);
    if (*phFileHandle == LNULL)
	return (-R_FOPERR);

    return ((signed short int) OK);
}

signed short int PASCAL ro00fg_Shfread (
	void              FAR  * lpBuffer,
	unsigned long int        ulSize,
	unsigned long int        ulNumb,
	unsigned long int        hFileHandle,
	void              FAR  * lpOpenBuff,
	unsigned long int FAR  * pulOutNumb)
{
/*  read = fread (ptr,(size_t)size,(size_t)numb,(FILE *)fp);   */

    *pulOutNumb = ro00fe_fread (lpBuffer, ulSize, ulNumb, hFileHandle);
    return (OK);
}

signed short int  PASCAL ro00fi_Shfwrite (
	void              FAR * lpBuffer,
	unsigned long int       ulSize,
	unsigned long int       ulNumb,
	unsigned long int       hFileHandle,
	void              FAR * lpOpenBuff)
{
/*  writen = (ulong) fwrite (ptr, (size_t)size, (size_t)numb, (FILE *)fp); */

    return (ro00f3_fwrite (lpBuffer, ulSize, ulNumb, hFileHandle));
}

signed short int  PASCAL ro00fj_Shfclose (
	unsigned long  int       hFileHandle,
	void               FAR * lpOpenBuff,
	signed   short int FAR * pbCloseAll)
{
/*  if ((sRetCode = fclose ((FILE *) fp)) != 0)   sss ** */
    *pbCloseAll = TRUE;
    return (ro00f2_fclose (hFileHandle));
}

#endif
