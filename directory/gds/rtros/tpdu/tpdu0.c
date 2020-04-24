/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: tpdu0.c,v $
 * Revision 1.1.4.2  1996/03/11  13:23:44  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:15  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:45:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:24:25  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:14:46  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:53:48  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:02:53  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:09:48  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:46:36  keutel
 * 	creation
 * 	[1994/03/21  13:38:26  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: tpdu0.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:23:44 $" } ;
#endif

/***************************************************************************\
*                                                                           *
*  * Copyright (c) 1993 by                                                  *
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

/*daton	***************************************************************/
/*								      */
/* TYPE		: MODULE					      */
/*								      */
/* NAME         : tpdu.c                                              */
/*								      */
/* AUTHOR       : Shivkumar                                           */
/* DATE         : 02-12-93                                            */
/*								      */
/* COMPONENT    : ROS Tpdu functions                                  */
/*								      */
/* PRD#/VERS.   : ROS-V03.00                                          */
/*								      */
/* DESCRIPTION	: This module is an implementation of the             */
/*		  TPDU interface (Virtual File Access Method).        */
/*								      */
/*		  Uses the original TPDU implementation of CoCoNet.   */
/*								      */
/* SYSTEM DEPENDENCE :	none					      */
/*								      */
/* HISTORY      :                                                     */
/*								      */
/* Vers.Nr. | Date    |	 Modification		       |mark| CR# EM# */
/*  03.00   |02-12-93 |  original                      | skh|         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*		Module Identification				      */
/**********************************************************************/

static char *sccsid =    "@(#) tpdu0.c 1.1 94/01/19   ROS-V03.00";

/**********************************************************************/
/*		I N C L	U D E S					      */
/**********************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

/**************************/
#ifdef R_G000_Windows  /***/
/**************************/

/* #define WINDOWS */

#include        <windows.h>  /* WINDOWS system interface include         */
#include        <string.h>
#include        <stdlib.h>

#undef ERROR
#define WIN_INCLUDED
#define RP0002_WinIncl       /* for dos compilation -sss                 */

#define OP_READ         "rb"
#define OP_CREATE       "wb+"
#define OP_UPDATE       "rb+"

extern BOOL bDefiniteLengthTpdu;       /* TRUE: definite length in use   */
                                       /* Note: This variable is defined */
                                       /* in the DOS test program !      */
/**************************/
#else /* ! R_G000_Windows */
/**************************/

#define OP_READ         "r"
#define OP_CREATE       "w"
#define OP_UPDATE       "r+"

/**************************/
#endif /* R_G000_Windows  */
/**************************/




/**************************/
#ifdef TPDU_FILELENGTH  /**/
/**************************/

#define   DEFINITE_FILELENGTH             1
#define   INDEFINITE_FILELENGTH           2
#define   ALWAYS_DEFINITE_FILELENGTH      3
#define   ALWAYS_INDEFINITE_FILELENGTH    4

static    int               prompt_tpdu   = DEFINITE_FILELENGTH;

/**************************/
#endif /* TPDU_FILELENGTH */
/**************************/




#include        <stdio.h>
#include        <errno.h>
#include        <fcntl.h>

	/**** System -Includes  ****/

#include        <rogenrl.h>
#include        <tpdu.h>
#include        <tpdu1.h>



/********************************************************/
/*      D E F I N E S                                   */
/********************************************************/

#define         UNUSED          0
#define         USED            1

#ifndef TRUE
#define         TRUE            1
#endif

#ifndef FALSE
#define         FALSE           0
#endif




/********************************************************/
/*      TYPEDEFS                                        */
/********************************************************/

#define MAX_FILENAMELEN    128

typedef struct  {
	TP_char             filename [ MAX_FILENAMELEN ];
	TP_short            name_len;
	Tpdir               dir;
	TP_long             file_len;
	unsigned long int   file;
	TP_short            used;
} Filedata ;


/************************************************/
/*      L O C A L     D A T A                   */
/************************************************/

#define TP_TABLE_SIZE        100

extern int               list_length ;
extern Filedata          file_list [ TP_TABLE_SIZE ];


/************************************************/
/*      M A C R O S                             */
/************************************************/

#define VERSIONSTEST                                     \
	if (pb->tp_pbhead->tpversion != TP2V02)          \
	{                                                \
	  pb->tp_pbhead->tperr_value = TPNOSUPVS;        \
	  pb->tp_pbhead->tperr_class = TP_ILL_VERS;      \
	  return pb->tp_pbhead->tpret_code =TP_CALL_ERR; \
	}

#define RETURN( RC, CLASS, VAL)                          \
	pb->tp_pbhead->tperr_value = VAL;                \
	pb->tp_pbhead->tperr_class = CLASS;              \
	return pb->tp_pbhead->tpret_code = RC

void PASCAL tp_strcpy PROTO ((
char FAR *destination,
char FAR *source ));

unsigned long int PASCAL tp_fopen PROTO ((
char  FAR  *fname,
char  FAR  *fmode ));

signed short int  PASCAL tp_fclose PROTO ((
unsigned long int   fp ));

signed short int  PASCAL tp_fwrite PROTO ((
void     FAR       *  ptr,
unsigned long int     size,
unsigned long int     numb,
unsigned long int     fp));

signed long int PASCAL tp_fseek PROTO ((
unsigned long int   fp,
long int            offset,
int                 mapptr));

void PASCAL tp_fflush PROTO ((
unsigned long int   fp ));

signed long int  PASCAL tp_fread PROTO ((
void     FAR       *  ptr,
unsigned long int     size,
unsigned long int     numb,
unsigned long int     fp));

signed long int PASCAL tp_filelen PROTO ((
unsigned long int fp));



/********************************************************/
/* L O C A L   F U N C T I O N    D E F I N I T I O N S */
/********************************************************/

/**************************/
#ifdef RP0000_MicV0600 /**/
/**************************/


void PASCAL tp_strcpy (
	char FAR *destination,
	char FAR *source)
{
  _fstrcpy (destination, source);
}

unsigned long int PASCAL tp_fopen (
	char  FAR  *fname,
	char  FAR  *fmode)
{
    HFILE hFd;
    unsigned int mode;
    OFSTRUCT lpFileInfo;

    if      (!lstrcmp (fmode, OP_READ))
	mode = OF_READ;
    else if (!lstrcmp (fmode, OP_CREATE))
	mode = (OF_CREATE | OF_WRITE);
    else if (!lstrcmp (fmode, OP_UPDATE))
	mode = OF_READWRITE;
    else 
	return ((unsigned long int)0L);

    mode |= (OF_SHARE_COMPAT | OF_SHARE_DENY_NONE);
    hFd = OpenFile (fname, (LPOFSTRUCT)&lpFileInfo, mode);
    if (hFd == HFILE_ERROR)
        return ((unsigned long int) 0L);

    return ((unsigned long int) hFd);
}


signed short int  PASCAL tp_fclose(
	unsigned long int   fp)
{
    signed short int rc;

    if ((rc = _lclose ((HFILE)fp)) != 0 )
	rc = -1;

    return (rc);
}

signed long int PASCAL tp_fseek(
	unsigned long int   fp,
	long int            offset,
	int                 mapptr)
{
    return (_llseek ((HFILE)fp, offset, mapptr));
}

signed short int  PASCAL tp_fwrite (
	void     FAR       *  ptr,
	unsigned long int     size,
	unsigned long int     numb,
	unsigned long int     fp)
{

    signed short int rc             = OK;
    signed   long int writebytes    = 0L;
    signed   long int actualwritten = 0L;


    writebytes = size * numb;
    actualwritten = (unsigned long int)_lwrite((HFILE)fp, ptr, (int)writebytes);
    if ( actualwritten != writebytes ) rc = -1;
    else {
        rc = (signed short int)(actualwritten / size);
        if ( (actualwritten > 0L) && (rc == 0) ) rc = 1;/* at least 1 written */
    }

    return (rc);
}

void  PASCAL tp_fflush(
	unsigned long int   fp)
{
/*
    _flushall();
*/
}

signed long int     PASCAL tp_fread (
	void     FAR       *  ptr,
	unsigned long int     size,
	unsigned long int     numb,
	unsigned long int     fp)
{
    signed long int actualread = 0L;
    signed long int readsize   = 0L;
    signed long int ctr        = 0L;

    readsize = size * numb;

    actualread = _lread ((int)fp, ptr, (int)readsize);
    if   ( actualread == HFILE_ERROR ) ctr = 0;
    else {
         ctr = (actualread / size);
         if ( (actualread > 0L) && (ctr == 0L) ) ctr = 1L;/*at least 1 is read*/
    }

    return (ctr);
}


/**************************/
#else /* !RP0000_MicV0600 */
/**************************/


void PASCAL tp_strcpy (
	char FAR *destination,
	char FAR *source)
{
  strcpy (destination, source);
}

unsigned long int PASCAL tp_fopen (
	char  FAR  *fname,
	char  FAR  *fmode)
{
    return( (unsigned long int) fopen(fname,fmode));
}


signed short int  PASCAL tp_fclose(
	unsigned long int   fp)
{
    signed short int rc;
    if  ((rc = fclose ( (FILE *) fp)) != 0)
	rc = -1;
    return (rc);
}

signed long int PASCAL tp_fseek(
	unsigned long int   fp,
	long int            offset,
	int                 mapptr)
{
    if (fseek ((FILE *)fp, offset, mapptr) != 0) return (-1L);
    else                                 return (ftell ((FILE *)fp));
}

signed short int  PASCAL tp_fwrite(
	void     FAR       *  ptr,
	unsigned long int     size,
	unsigned long int     numb,
	unsigned long int     fp)
{

    signed short int rc = 0;
    unsigned long int writen;

    writen = fwrite(ptr,size,numb,(FILE *)fp);
    if (writen  != numb )
	rc = -1;
    if (!rc)
         tp_fflush(fp);
    return (rc);
}

void  PASCAL tp_fflush(
	unsigned long int   fp)
{
    fflush ((FILE*)fp);
    /* sync(); */
}

signed long int  PASCAL tp_fread(
	void     FAR       *  ptr,
	unsigned long int     size,
	unsigned long int     numb,
	unsigned long int     fp)
{
    return ((signed long int)fread(ptr,size,numb,(FILE *)fp));
}


/**************************/
#endif /* RP0000_MicV0600 */
/**************************/


signed long int PASCAL tp_filelen (
	unsigned long int fp)
{
  signed long int len = LNULL;
  signed long int oldlen = LNULL;

  oldlen = tp_fseek ( fp, 0,      1 ); /* auf das Dateiende */
  len    = tp_fseek ( fp, 0,      2 );
  oldlen = tp_fseek ( fp, oldlen, 0 ); /* auf den Dateianfang */
  return (len);
}

