/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: routil2.c,v $
 * Revision 1.1.4.2  1996/03/11  13:20:12  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:14  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:32:39  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:18:29  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:08:01  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:50:00  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:59:47  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:55:17  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:34:34  keutel
 * 	creation
 * 	[1994/03/21  13:25:00  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: routil2.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:20:12 $" } ;
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

/*daton	***************************************************************/
/*								      */
/* TYPE		: MODULE					      */
/*								      */
/* NAME         : routil2.c                                           */
/*								      */
/* AUTHOR       : Monika Krause/Gerhard Jahn                          */
/* DATE         : 14-JUN-1992                                         */
/*								      */
/* COMPONENT    : RTS utility functions                               */
/*								      */
/* PRD#/VERS.   : RTS-V2.0                                            */
/*								      */
/* DESCRIPTION	: This modul provides functions	which		      */
/*								      */
/*   - call the logging system and assign parameters to               */
/*     the corresponding parameter blocks.			      */
/*								      */
/* SYSTEM DEPENDENCE :	none					      */
/*								      */
/* HISTORY      :                                                     */
/*								      */
/* Vers.Nr. | Date    |	 Modification		       |mark| CR# EM# */
/*  1.00    |14-06-92 |  original                      | gj |         */
/*datoff **************************************************************/

/**********************************************************************/
/*		Module Identification				      */
/**********************************************************************/

static char *sccsid =    "@(#) routil2.c 1.2 93/12/20   RTS-V2.00";

/**********************************************************************/
/*		I N C L	U D E S					      */
/**********************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#define _WINDLL
/* #include <varargs.h>     *  Include of variable argument definitions */ 
#include <stdarg.h>
#include <string.h>

#include <rogenrl.h>
#include <roerval.h>      /* Include of RTS common utilites (extern)  */
#include <rolgval.h>      /* Include of RTS common utilites (extern)  */
#include <rodpat0.h>
#include <rospm00.h>
#include <rotpm00.h>

#include <rostor0.h>      /* Include of RTS Store prototypes(extern)  */
#include <routil0.h>      /* Include of RTS common utilites (extern)  */
#include <routil2.h>      /* Include of RTS common utilites (logging) */
#include <routil3.h>      /* Include of RTS common utilites (logging) */
#include <routil6.h>      /* Include the ROS definitions              */
#include <roloif9.h>      /* Include of RTS common utilites (intern)  */

/**********************************************************************/
/*		G L O B	A L   V	A R I A	B L E S			      */
/**********************************************************************/

  static rs0002_logpar rv0002_logpar;

/**********************************************************************/
/*		L O C A L   F U N C T I O N S			      */
/**********************************************************************/

  static signed short int PASCAL ro004b_SwitchLogFile PROTO((void));
  static signed short int PASCAL ro004f_newOpenLog PROTO ((
  unsigned short int  loglevel,
  unsigned short int    recnum,
  char FAR              *fname,
  unsigned short int     fmode));


/**********************************************************************/
/*		G L O B	A L   F	U N C T	I O N S			      */
/**********************************************************************/

#ifdef  RC0000_TRACE

/*exon ****************************************************************/
/*								      */
/* TYPE		: C-FUNCTION					      */
/*								      */
/* NAME		: ro0010_entry					      */
/*								      */
/* AUTHOR       : Monika Krause                                       */
/* DATE		: 90-10-29					      */
/*								      */
/* SYNTAX	:						      */

signed short int PASCAL	ro0010_entry(
       char FAR *fnctName)

/*								      */
/* DESCRIPTION	:   This function writes a logging record to the      */
/*		    logging-file whenever a function is	entered.      */
/*								      */
/* IN-PARAMETERS  :						      */
/*								      */
/*	fnctname  : Name of the	function calling the entry function.  */
/*								      */
/*								      */
/*								      */
/* OUT-PARAMETERS :		 none				      */
/*								      */
/* RETURN VALUES  :						      */
/*								      */
/*	  =  0	   if successful,				      */
/*	  <> 0	  if failure.					      */
/*								      */
/* GLOBAL DATA	 ( read	only ):	 none				      */
/*								      */
/* GLOBAL DATA	 ( modified ) :	 none				      */
/*								      */
/* REMARKS		      :					      */
/*								      */
/*inoff	***************************************************************/

/* ****************** l	o c a l	   v a r i a b l e s **************** */
/* ******************				     **************** */
{/*entr*/

signed int i;

/* ****************** S	T A R T	************************************* */
/* ******************		************************************* */

i = ro0053_strlen (fnctName);
return (ro0042_writeLog	(RC0021_TRACE,
	     RL0001_FctTraceEntry,
	     (i	<= RC0040_MaxStrLen) ? i : RC0040_MaxStrLen,
	     (char FAR *)fnctName));

}/*end*/


/*exon ****************************************************************/
/*								      */
/* TYPE		: C-FUNCTION					      */
/*								      */
/* NAME		: ro0011_return					      */
/*								      */
/* AUTHOR       : Monika Krause                                       */
/* DATE		: 90-10-29					      */
/*								      */
/* SYNTAX	:						      */

signed short int  PASCAL ro0011_return(
    char *fnctName,
    signed long int rc)

/*								      */
/* DESCRIPTION	:   This function writes a logging-record to the      */
/*		    logging-file whenever a function is	leaved.	      */
/*								      */
/* IN-PARAMETERS  :						      */
/*								      */
/*	fnctname  : Name of the	function calling the exit function.   */
/*								      */
/*	rc	  : Returncode of the function.			      */
/*								      */
/* OUT-PARAMETERS :		 none				      */
/*								      */
/* RETURN VALUES  :						      */
/*								      */
/*	  SUCES,  if successful,				      */
/*	  CALERR, if failure.					      */
/*								      */
/* GLOBAL DATA	 ( read	only ):	 none				      */
/*								      */
/* GLOBAL DATA	 ( modified ) :	 none				      */
/*								      */
/* REMARKS		      :					      */
/*								      */
/*inoff	***************************************************************/

/* ****************** l	o c a l	   v a r i a b l e s **************** */
/* ******************				     **************** */
{/*entr*/

signed short int i;

/* ****************** S	T A R T	************************************* */
/* ******************		************************************* */

i = ro0053_strlen (fnctName);

return (ro0043_pack10Log (RC0021_TRACE,
	     RL0002_FctTraceReturn,
	     2,
	     (i	<= RC0040_MaxStrLen) ? i : RC0040_MaxStrLen,
	     (char FAR *)fnctName,
	     sizeof (signed long int),
	     (char FAR *) &rc));
}/*end*/

#endif /* RC0000_TRACE  */


/*exon ****************************************************************/
/*                                                                    */
/* TYPE              : C-FUNCTION                                     */
/*                                                                    */
/* NAME              : ro004c_newOpenLog                              */
/*                                                                    */
/* AUTHOR            : Shivkumar                                      */
/* DATE              : 93-11-10                                       */
/*                                                                    */
/* SYNTAX            :                                                */

static signed short int PASCAL ro004f_newOpenLog ( 
  unsigned short int  loglevel,
  unsigned short int    recnum,
  char FAR              *fname,
  unsigned short int     fmode)


/*                                                                    */
/* DESCRIPTION       : This function does a OpenLog in a status where */
/*                     logging is OFF and a logging level has been    */
/*                     specified.                                     */
/*                                                                    */
/* IN-PARAMETERS     : none                                           */
/*                                                                    */
/* OUT-PARAMETERS    : none                                           */
/*                                                                    */
/* RETURN VALUES     : 0 if ok.                                       */
/*                    !0 if error.                                    */
/*                                                                    */
/* GLOBAL DATA        ( read       only ):        none                */
/*                                                                    */
/* GLOBAL DATA        ( modified ) :        rv0002_logpar             */
/*                                                                    */
/* REMARKS :                                                          */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr-ro004f_newOpenLog*/
/* *******************  local variables  **************************** */
char                 buffer1[130];           /* store for filename */
signed short int     rc = 0;
unsigned long int    hFile=LNULL;            /* for File handle */
/* ******************  Start of Function  *************************** */

ro0050_strcpy ((char FAR *)buffer1,fname);
ro0051_strcat ((char FAR *)buffer1, (char FAR *)".l1");

rc = ro0060_OpenLog ((signed   char            FAR *)buffer1, fmode, loglevel,
                     (unsigned long int        FAR *)&hFile,
                     (void               FAR * FAR *)&rv0002_logpar.r_fileinfo);

if (rc) return (rc);

/* session ok: update logging administration */
ro0050_strcpy (rv0002_logpar.r_fname1, fname);            /* log file #1 */
ro0051_strcat (rv0002_logpar.r_fname1, (char FAR *)".l1");
ro0050_strcpy (rv0002_logpar.r_fname2, fname);            /* log file #2 */
ro0051_strcat (rv0002_logpar.r_fname2, (char FAR *)".l2");
rv0002_logpar.r_recnum   = recnum;       /* max no. of records */
                                         /* before file #2 is used */
rv0002_logpar.r_logging  = RC0027_ON;
rv0002_logpar.r_logLevel = loglevel;
rv0002_logpar.r_fselect  = 1;            /* use 1st file */
rv0002_logpar.r_actnum   = 0;            /* counts no of written rcds */

rv0002_logpar.hFile      = hFile;

return (OK);
}/*end*/


/*exon ****************************************************************/
/*								      */
/* TYPE		: C-FUNCTION					      */
/*								      */
/* NAME		: ro004d_RefreshLog				      */
/*								      */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : 14-JUN-1992                                         */
/*								      */
/* SYNTAX	:						      */

signed short int PASCAL ro004d_RefreshLog( 
  unsigned short int  loglevel,
  unsigned short int    recnum,
  char FAR              *fname,
  unsigned short int     fmode)

/*								      */
/* DESCRIPTION	: This function	initializes the logging system.       */
/*		  The decision what to do depends on the current      */
/*		  state of logging:                                   */
/*		   1) logging ON  & loglevel > 0:                     */
/*			  ==> update only the loglevel                */
/*		   2) logging ON  & loglevel = 0:                     */
/*			  ==> switch logging off                      */
/*		   3) logging OFF & loglevel > 0:                     */
/*			  ==> turn on logging, set input values       */
/*		   4) logging OFF & loglevel = 0:                     */
/*			  ==> do nothing                              */
/*								      */
/* IN-PARAMETERS  :                                                   */
/*								      */
/*      loglevel  : logging level 				      */
/*        recnum  : max no. of records in the logging file            */
/*         fname  : logging file name                                 */
/*         fmode  : file creation mode                                */
/*								      */
/* OUT-PARAMETERS :  none                                             */
/*								      */
/* RETURN VALUES  :     0 if ok.                                      */
/*		       !0 if error.(Session-openLog failed)           */
/*								      */
/* GLOBAL DATA	 ( read	only ):	 none				      */
/*								      */
/* GLOBAL DATA	 ( modified ) :	 rv0002_logpar   		      */
/*								      */
/* REMARKS :					                      */
/*								      */
/*inoff	***************************************************************/

{/*entr-ro004d_RefreshLog*/
/* *******************  local variables  **************************** */
  signed short int     rc = 0;
  signed short int     bCloseAll = FALSE;

/* ******************  Start of Function  *************************** */

  if (rv0002_logpar.r_logging == RC0028_OFF) {   /* if logging is off */
      if (loglevel) {                /* input says: switch on logging */
                       /* use the input values for assignment + open */
          rc = ro004f_newOpenLog (loglevel, recnum, fname, fmode);
      }
      else             /* do nothing if logging is OFF + loglevel = 0 */
         ;
  }
  else {                                     /* logging is already ON */
      if (!loglevel) {              /* input says: switch off logging */
          bCloseAll = FALSE;
          rc = ro0061_CloseLog ( rv0002_logpar.hFile,
                                 rv0002_logpar.r_fileinfo,
                                 (signed short int FAR *)&bCloseAll);
          if (!rc && bCloseAll) {               /* closed file well   */
              rv0002_logpar.r_logLevel= 0;      /* switch logging off */
              rv0002_logpar.r_logging = RC0028_OFF;
              rv0002_logpar.hFile     = LNULL;
              rv0002_logpar.r_fileinfo= (void FAR *)LNULL;
          }
      }
      else {   /* if logging is ON + loglevel !0 just reset the value */
#ifndef RP0000_MicV0600
          bCloseAll = TRUE;
          rc = ro0061_CloseLog ( rv0002_logpar.hFile,
                                 rv0002_logpar.r_fileinfo,
                                 (signed short int FAR *)&bCloseAll);
          rv0002_logpar.r_fileinfo = (void FAR *)LNULL;
          if (!rc && bCloseAll) {
                       /* use the input values for assignment + open */
              rc = ro004f_newOpenLog (loglevel, recnum, fname, fmode);
          }
#else
          rc = ro0060_OpenLog (
                   (signed   char            FAR *)LNULL,
                   (unsigned short int            )0,
                   (unsigned short int            )0,
                   (unsigned long  int       FAR *)&(rv0002_logpar.hFile),
                   (void               FAR * FAR *)&(rv0002_logpar.r_fileinfo));
          rv0002_logpar.r_logLevel = loglevel;
#endif
      }
  }
  return (rc);

}/*end-ro004d_RefreshLog*/
/* *******************  End of Function  **************************** */

/*exon ****************************************************************/
/*								      */
/* TYPE		: C-FUNCTION					      */
/*								      */
/* NAME		: ro0042_writeLog				      */
/*								      */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : 03-JUN-1992                                         */
/*								      */
/* SYNTAX	:						      */

signed short int  PASCAL ro0042_writeLog (
  unsigned short int    level,
  signed long int       type,
  unsigned long int     length,
  char FAR             *record)

/*								      */
/* DESCRIPTION  : This function writes a logging record using         */
/*                ro0062_WriteLog                                     */
/*								      */
/* IN-PARAMETERS  :						      */
/*								      */
/*    level  : Logging level desired by caller.	                      */
/*    type   : Type of logging record.                                */
/*    length : Length of logging information.                         */
/*    record : Pointer to logging record.                             */
/*								      */
/* OUT-PARAMETERS :  none		        		      */
/*								      */
/* RETURN VALUES  :						      */
/*								      */
/*         =  0  sucessfully  logging                                 */
/*         <> 0  if error                                             */
/*								      */
/* GLOBAL DATA	 ( read	only ):	 none				      */
/*								      */
/* GLOBAL DATA	 ( modified ) :	 none				      */
/*								      */
/* REMARKS		      :					      */
/*								      */
/*inoff	***************************************************************/

{/*entr-ro0042_writeLog*/
/* *******************  local variables  **************************** */
  rT0060_LogRecHead       h;
  signed short int        rc = 0;
/* ******************  Start of Function  *************************** */

               /* if the limit for a new reading of the config-file */
                     /* is reached, then read the config-file again */
      /* NOTE: because all the RTROS-logging functionality is redirected to */
      /* DCE-serviceability only the write logging function has to be called  */
/*
  if (rv0002_logpar.r_confreadnum <= 0)
     {
      rc = ro004a_ReadConf();
      if (rc) return (rc);
     }

  if (level & rv0002_logpar.r_logLevel)  #* check whether this record *#
     {                          #* fits the the current logging level *#
                   #* prepare the header for the later session call *#

                            #* avoid misbehaviour by NULL pointer *#
*/
      h.lLength = (record == NULL)?  0 : length;
      h.lRecordType = type;
      h.lWriteTime = (signed long int)ro0040_time (); /* write the record */
      
      rc = ro0062_WriteLog (rv0002_logpar.hFile, 
                            rv0002_logpar.r_fileinfo,
                            (rT0060_LogRecHead FAR *)&h,
                            record);

/*
      rv0002_logpar.r_actnum++;               #* update the counters *#
                                   #* change the logging file if *#
                            #* the number of records gets too big *#

      if (rv0002_logpar.r_actnum >= rv0002_logpar.r_recnum)
        {
         rc = ro004b_SwitchLogFile();
        }
     }
  rv0002_logpar.r_confreadnum--;
*/
  return (rc);
/* *******************  End of Function  **************************** */
}/*end-ro0042_writeLog*/


/*exon ****************************************************************/
/*								      */
/* TYPE		: C-FUNCTION					      */
/*								      */
/* NAME		: ro0043_pack10Log				      */
/*								      */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : 10-JUN-1992                                         */
/*								      */
/* SYNTAX	:						      */

/* signed short int ro0043_pack10Log (
  unsigned short int usLevel,
  signed long int      lType,
  unsigned short int     usN,
  va_dcl) */
signed short int ro0043_pack10Log (
  unsigned short int usLevel,
  signed long int      lType,
  unsigned short int     usN,
  ...)

/*								      */
/* DESCRIPTION	: This function	checks the logging level and returns  */
/*		  immediately if the level is not set. Otherwise it   */
/*		  flattens the logging information it gets in form of */
/*		  variable stack of parameters to an array of charac- */
/*                ter. Afterwards it calls the ro0342_writeLog        */
/*                function to write the record.                       */
/*								      */
/* IN-PARAMETERS  :						      */
/*								      */
/*     usLevel  : Desired logging level (Interface/Activity/C+W/...)  */
/*      ulType  : Logging type identifier                             */
/*         usN  : Number of following logging records                 */
/*     va_alist : Stack of logging records (variable argumet list)    */
/*                each single record consists of:                     */
/*                   -  signed short int  (= length)                  */
/*                   -  char FAR *        (= reference to values)     */
/*  Shape:                                                            */
/*   pack10Log (p1, p2, p3, signed short int, char FAR *, .........)  */
/*                      <i> |------  i times----------------------|   */
/*								      */
/* OUT-PARAMETERS   :       none                                      */
/*								      */
/* CALLING FUNCTIONS:  ro0342_writelog                                */
/*								      */
/* RETURN VALUES :                                                    */
/*          0 : logging record was written successfully               */
/*         >0 : no logging record written  (level didn't fit)         */
/*         <0 : error  (session couldn't write the record)            */
/*								      */
/* GLOBAL DATA	 ( read	only ):	 none				      */
/*								      */
/* GLOBAL DATA	 ( modified ) :	 none				      */
/*								      */
/* REMARKS		      :					      */
/*								      */
/*inoff	***************************************************************/

{/*entr-ro0043_pack10Log*/
/* *******************  local variables  **************************** */
  va_list              v;  /* define va_list                          */
  signed short int    l0;  /* lenght of one subrecord                 */
  char           FAR *p0;  /* reference for content of a subrecord    */
  unsigned short int   l;  /* total length of packed record           */
  unsigned char       *p;  /* reference to intermediate store         */
  static unsigned char    store[CONSTmax];       /* store for packing */
  rT0060_LogRecHead    h;  /* session record header                   */
  signed short int  *pus;
  signed short int    rc = 0;
/* ******************  Start of Function  *************************** */

		 /* if the limit for a new reading of the config-file */
		       /* is reached, then read the config-file again */
      /* NOTE: because all the RTROS-logging functionality is redirected to */
      /* DCE-serviceability configuration handling is ignored here */
/*
  if (rv0002_logpar.r_confreadnum <= 0)
     {
      rc = ro004a_ReadConf();
      if (rc) return (rc);
     }

  if (usLevel & rv0002_logpar.r_logLevel)#* check whether this record *#
     {                              #* fits the current logging level *#
*/

      if (usN < CONSTlow)                 /* correct range violations */
	  usN = 0;
      if (usN > CONSThigh)
	  usN = CONSThigh;

      va_start(v,usN);                 /* initializing the parameter list */

				       /* now build the packed string */
      p = store;
      *p++ = (unsigned char) usN;
      l    = 1;
				   /* as long as there are parameters */
      while (usN--)
	{                     /* pop in the parameters from the stack */
	 l0 = (signed short int) va_arg(v, int);            /* length */
	 p0 = va_arg(v, char FAR *);           /* reference to values */

		 /* First check whether the store would be overfilled */
	      /* by writing this entry. There must be at least enough */
      /* space to write all the <length> fields of the entire record! */
     /* If the values exceed the store-limit then just write <0xffff> */
					/* as length (but no values). */
	/* This special value will be evaluated by <rtstep> later on. */
	 if (CONSTmax < (l + l0 + sizeof(l0) + sizeof(l0)*usN))
	    {
	     pus  = (signed short int *) p;
	     *pus = (signed short int) RLro20_FIELDERR;
	     p+=sizeof(signed short int);
	     l0 = 0;
	    }
	 else
	    {                                 /* copy length to store */
	     if (p0 == NULL)                    /* avoid NULL-Pointer */
		 l0 = 0;
	     ro0041_memcpy ((char FAR *)p,(char FAR *)&l0, sizeof(l0));
	     p += sizeof(l0);                       /* next parameter */
					     /* copy content to store */
	     ro0041_memcpy ((char FAR *)p,(char FAR *)p0, l0);
	     p += l0;
	    }
	 l += l0 + sizeof(l0);                 /* update total length */
	} /* while */

      va_end(v);
		     /* prepare the header for the later session call */
      h.lLength = l;
      h.lRecordType = lType;
      h.lWriteTime = (signed long int)ro0040_time (); /* write the record */

      rc = ro0062_WriteLog (rv0002_logpar.hFile, 
                            rv0002_logpar.r_fileinfo,
                            (rT0060_LogRecHead FAR *)&h,
                            (char FAR *) store );
      if (rc) return (rc);

/*
      rv0002_logpar.r_actnum++;                #* update the counters *#

					#* change the logging file if *#
				#* the number of records gets too big *#
      if (rv0002_logpar.r_actnum >= rv0002_logpar.r_recnum)
	 {
	  rc = ro004b_SwitchLogFile();
	 }
     }
  else                   #* if logging level didn't fit current level *#
      rc = 1;
  rv0002_logpar.r_confreadnum--;
*/

  return (rc);
/* *******************  End of Function  **************************** */
}/*end-ro0043_pack10Log*/

/*exon ****************************************************************/
/*								      */
/* TYPE		: C-FUNCTION					      */
/*								      */
/* NAME		: ro0044_AppendLog				      */
/*								      */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 04-11-93                                            */
/*								      */
/* SYNTAX	:						      */

signed short int  PASCAL ro0044_AppendLog (
  unsigned short int    level,
  unsigned long int     length,
  char FAR             *record)

/*								      */
/* DESCRIPTION  : This function appends to a record using             */
/*                ro00f3_fwrite                                       */
/*								      */
/* IN-PARAMETERS  :						      */
/*								      */
/*    level  : Logging level desired by caller.	                      */
/*    length : Length of logging information.                         */
/*    record : Pointer to logging record.                             */
/*								      */
/* OUT-PARAMETERS :  none		        		      */
/*								      */
/* RETURN VALUES  :						      */
/*								      */
/*         =  0  sucessfully  logging                                 */
/*         <> 0  if error                                             */
/*								      */
/* GLOBAL DATA	 ( read	only ):	 none				      */
/*								      */
/* GLOBAL DATA	 ( modified ) :	 none				      */
/*								      */
/* REMARKS		      :					      */
/*								      */
/*inoff	***************************************************************/

{/*entr-ro0044_AppendLog*/
/* *******************  local variables  **************************** */
  signed short int        rc = 0;
/* ******************  Start of Function  *************************** */

  if (level & rv0002_logpar.r_logLevel)  /* check whether this record */
     {                          /* fits the the current logging level */
      if (record == NULL) length = 0;
/*
      rc = ro00f3_fwrite (record, sizeof(char),length,rv0002_logpar.hFile);
*/
    rc = ro00fi_Shfwrite ((char FAR *)record,
                          (unsigned long int)sizeof(char),
                          (unsigned long int)length,
                          rv0002_logpar.hFile, rv0002_logpar.r_fileinfo);
     }

  return (rc);
/* *******************  End of Function  **************************** */
}/*end-ro0044_AppendLog*/


/*exon ****************************************************************/
/*								      */
/* TYPE		: C-FUNCTION					      */
/*								      */
/* NAME		: ro0045_PduLog  				      */
/*								      */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 04-11-93                                            */
/*								      */
/* SYNTAX	:						      */

signed short int  PASCAL ro0045_PduLog (
  signed   short int      sPmNo,
  unsigned long int       ulEvent,
  unsigned long int       hStore)

/*								      */
/* DESCRIPTION  : This function writes the Pdu in the Store to Log    */
/*								      */
/* IN-PARAMETERS  :						      */
/*								      */
/*    sPmNo  : Protocol Machine Number         	                      */
/*    ulEvent: Event value                                            */
/*    hStore : Store handle                                           */
/*								      */
/* OUT-PARAMETERS :  none		        		      */
/*								      */
/* RETURN VALUES  :						      */
/*								      */
/*         =  0  sucessfully  logging                                 */
/*         <> 0  if error                                             */
/*								      */
/* GLOBAL DATA	 ( read	only ):	 none				      */
/*								      */
/* GLOBAL DATA	 ( modified ) :	 none				      */
/*								      */
/* REMARKS		      :					      */
/*								      */
/*inoff	***************************************************************/

{/*entr-ro0045_PduLog*/
/* *******************  local variables  **************************** */
/* ******************  Start of Function  *************************** */
switch ( sPmNo ) {                                                            
/*                                                                              
case RC0214_Session      :                                                    
case RC0215_Presentation :                                                     
 */
  /*   if ( ((ulEvent & 0x0f00) == RC0800_SpmEvent) && (hStore) ) {                   */
case RC0202_Lower:
case RC0213_Transport: 
case RC0214_Session      :                                                    
     if ( ((ulEvent & 0x0f00) == RC0900_TpmEvent) && (hStore) ) {
                   
         /*-----------------------------------------------------------------*/
         /* Call the PduLog function only if :                              */
         /* - Event passing through SPM-PPM boundary, and                   */
         /* - A store exists in P1 (user data of SPM).                      */
         /*-----------------------------------------------------------------*/
         ro0088_StoreToLog(hStore); 
     }                                                                        
     break;                                                                   
                                                                              
 default                  :                                                   
     break;                                                                   
}                                                                             

return (0);

/* *******************  End of Function  **************************** */
}/*end-ro0045_PduLog*/

/*exon ****************************************************************/
/*								      */
/* TYPE		: C-FUNCTION					      */
/*								      */
/* NAME		: ro0046_writeHeader				      */
/*								      */
/* AUTHOR       : Chandrasekar.K                                      */
/* DATE         : 18-11-93                                            */
/*								      */
/* SYNTAX	:						      */

signed short int  PASCAL ro0046_writeHeader (
  unsigned short int    level,
  signed long int       type,
  unsigned long int     length)

/*								      */
/* DESCRIPTION  : This function writes a logging record header        */
/*								      */
/* IN-PARAMETERS  :						      */
/*								      */
/*    level  : Logging level desired by caller.	                      */
/*    type   : Type of logging record.                                */
/*    length : Length of logging information.                         */
/*								      */
/* OUT-PARAMETERS :  none		        		      */
/*								      */
/* RETURN VALUES  :						      */
/*								      */
/*         =  0  sucessfully  logging                                 */
/*         <> 0  if error                                             */
/*								      */
/* GLOBAL DATA	 ( read	only ):	 none				      */
/*								      */
/* GLOBAL DATA	 ( modified ) :	 none				      */
/*								      */
/* REMARKS		      :					      */
/*								      */
/*inoff	***************************************************************/

{/*entr-ro0046_writeHeader*/
/* *******************  local variables  **************************** */
  rT0060_LogRecHead       h;
  signed short int        rc = 0;
/* ******************  Start of Function  *************************** */

               /* if the limit for a new reading of the config-file */
                     /* is reached, then read the config-file again */
  if (rv0002_logpar.r_confreadnum <= 0)
     {
      rc = ro004a_ReadConf();
      if (rc) return (rc);
     }

  if (level & rv0002_logpar.r_logLevel)  /* check whether this record */
     {                          /* fits the the current logging level */
                   /* prepare the header for the later session call */

                            /* avoid missbehaviour by NULL pointer */
      h.lLength = length;
      h.lRecordType = type;
      h.lWriteTime = (signed long int)ro0040_time (); /* write the record */
      
      rc = ro0062_WriteLog (rv0002_logpar.hFile, 
                            rv0002_logpar.r_fileinfo,
                            (rT0060_LogRecHead FAR *)&h,
                            NULL);

      rv0002_logpar.r_actnum++;                /* update the counters */

                                   /* change the logging file if */
                            /* the number of records gets too big */
      if (rv0002_logpar.r_actnum >= rv0002_logpar.r_recnum)
        {
         rc = ro004b_SwitchLogFile();
        }
     }
  rv0002_logpar.r_confreadnum--;

  return (rc);
/* *******************  End of Function  **************************** */
}/*end-ro0046_writeHeader*/

/*exon ****************************************************************/
/*								      */
/* TYPE		: C-FUNCTION					      */
/*								      */
/* NAME         : ro004a_ReadConf                                     */
/*								      */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : 02-JUN-1992                                         */
/*								      */
/* SYNTAX	:						      */

  signed short int PASCAL ro004a_ReadConf (void)

/*								      */
/* DESCRIPTION  : This function reads the settings for logging from   */
/*                the config-file.                                    */
/*								      */
/* IN-PARAMETERS  :   none                                            */
/*								      */
/* OUT-PARAMETERS :   none                                            */
/*								      */
/* RETURN VALUES    : 0 = if a config-file exists                     */
/*                    1 = if no config-file was found                 */
/*								      */
/* GLOBAL DATA	 ( read	only ):	 none				      */
/*								      */
/* GLOBAL DATA	 ( modified ) :	 none				      */
/*								      */
/* REMARKS		      :					      */
/*								      */
/*inoff	***************************************************************/

{/*entr-ro004a_ReadConf*/
/* *******************  local variables  **************************** */
  unsigned short int     loglevel;
  unsigned short int     recnum;
  unsigned short int     confnum;
  char FAR              *fname;
  signed short int       rc = 0;
/* ******************  Start of Function  *************************** */

  rc = ro004c_GetLogParams (&loglevel,&recnum,&confnum,&fname);

  if (rc)                                        /* no cfg-file found */
     {                        /* always set to default if no cfg-file */
      rv0002_logpar.r_confreadnum=RC0029_MAXCONF;
      return (OK);
/*    return (rc);     **skh: */
     }
                                    /* cfg-found - assign values */
  rc = ro004d_RefreshLog (loglevel, recnum, fname, RC0025_NewFile);

  rv0002_logpar.r_confreadnum = confnum;
  return (rc);
/* *******************  End of Function  **************************** */
}/*end-ro004a_ReadConf*/


/*exon ****************************************************************/
/*								      */
/* TYPE		: C-FUNCTION					      */
/*								      */
/* NAME		: ro004b_SwitchLogFile				      */
/*								      */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : 08-JUN-1992                                         */
/*								      */
/* SYNTAX	:						      */

  static signed short int PASCAL ro004b_SwitchLogFile (void)

/*								      */
/* DESCRIPTION	: This function	exchanges the actual filename for all */
/*		  logging records. Switching is done between          */
/*                the two filenames provided by the controlling       */
/*                structure (of type rs0002_logpar).                  */
/*								      */
/* IN-PARAMETERS  :  none	                        	      */
/*								      */
/* OUT-PARAMETERS :  none                                             */
/*								      */
/* RETURN VALUES  :     0 if ok.                                      */
/*		       !0 if error.				      */
/*								      */
/* GLOBAL DATA	 ( read	only ):	 none				      */
/*								      */
/* GLOBAL DATA	 ( modified ) :	 rv0002_logpar   		      */
/*								      */
/* REMARKS :					                      */
/*								      */
/*inoff	***************************************************************/

{/*entr-ro004b_SwitchLogFile*/
/* *******************  local variables  **************************** */
  signed char      FAR *fname;
  signed short int      rc        = OK;
  signed short int      bCloseAll = TRUE;
/* ******************  Start of Function  *************************** */

  if (rv0002_logpar.r_fselect == 1)        /* switch the logging file */
     {
      rv0002_logpar.r_fselect = 2;         /* use the second filename */
      fname = (signed char FAR *)rv0002_logpar.r_fname2;
     }
  else
     {
      rv0002_logpar.r_fselect = 1;          /* use the first filename */
      fname = ( signed char FAR *)rv0002_logpar.r_fname1;
     }

  bCloseAll = TRUE;                 /* fist close the old file */
  rc = ro0061_CloseLog ( rv0002_logpar.hFile,
                         rv0002_logpar.r_fileinfo,
                         (signed short int FAR *)&bCloseAll);
  if (rc) return (rc);
  rv0002_logpar.hFile      = LNULL;
  rv0002_logpar.r_fileinfo = (void FAR *)LNULL;

                                    /* open the new logging file */
  rc = ro0060_OpenLog (
                   fname,
                   (unsigned short int)RC0025_NewFile,
                   rv0002_logpar.r_logLevel,
                   (unsigned long int        FAR *)&(rv0002_logpar.hFile),
                   (void              FAR  * FAR *)&(rv0002_logpar.r_fileinfo));

  rv0002_logpar.r_actnum = 0;                    /* reset the counter */

  return (rc);
/* *******************  End of Function  **************************** */
}/*end-ro004b_SwitchLogFile*/

/* ********************  END of MODULE  ***************************** */
