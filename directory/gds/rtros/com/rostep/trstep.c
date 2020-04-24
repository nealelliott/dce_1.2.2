/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: trstep.c,v $
 * Revision 1.1.4.2  1996/03/11  13:19:57  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:05  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:31:59  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:18:10  root]
 * 
 * Revision 1.1.2.5  1994/08/24  08:35:24  marrek
 * 	Fix warnings. (OT 11824)
 * 	[1994/08/23  14:34:20  marrek]
 * 
 * Revision 1.1.2.4  1994/07/06  15:08:05  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:19:03  marrek]
 * 
 * Revision 1.1.2.3  1994/06/10  21:07:06  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:49:47  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:59:37  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:54:12  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:34:14  keutel
 * 	creation
 * 	[1994/03/21  13:23:53  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: trstep.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:19:57 $" } ;
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
/* NAME         : trstep.h                                            */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-05-04                                            */
/*                                                                    */
/* COMPONENT    : STEP evaluation program                             */
/*                                                                    */
/* PRD#/VERS.   : RORTS-V3.0                                          */
/*                                                                    */
/* DESCRIPTION  : main module of STEP                                 */
/*                                                                    */
/* SYSTEM DEPENDENCIES : None                                         */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)trstep.c	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* ----------|--------|--------------------------------|----|-------- */
/* 3.00   K0 |93-05-04|  original                      |kc  |         */
/*datoff **************************************************************/
#ifndef RP0000_MicV0600
static char *sccsid =    "@(#)trstep.c RTROS-V03.00";
#endif

#define STEP_VERSION   300

/* ******************************************************************* */
/*           I n c l u d e  F i l e s                                  */
/* ******************************************************************* */

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <stdio.h>
#include <rogenrl.h>
#include <rouply0.h>
#include <rouply1.h>
#include <routil0.h>
 
#include <routil6.h>      /* for login data definitions */

 
 
/*************************************************************************/
/*                     Local Constants and Macros                        */
/*************************************************************************/
#define MSGOPN   "STEP: CURRENT TRACE FILE: "
#define MSGCLS   "STEP: CLOSED TRACE FILE: "
#define MSGRER   "STEP: INCONSISTENT TRACE FILE\n"
#define MSGUSAGE "Usage:  %s [-d] [-l=nnn[k]] [-s=n/l/m/h] [-r=nnn]\n"
#define MSGSER   "step: invalid option '%s'\n"
#define MSGOER   "step: open error on trace file: %s\n"
#define MSGRERF  "step: inconsistent trace file: %s\n"
#define MSGMEMER "step: no memory for trace buffer\n"
#define MAXRECLEN  65345
 
/*************************************************************************/
/*                          Local Variables                              */
/*************************************************************************/
static char stars[] = "**************************************************\
*****************************";
/*************************************************************************/
/*************************************************************************/
/***                                                                   ***/
/***                        Global Functions                           ***/
/***                                                                   ***/
/*************************************************************************/
/*************************************************************************/
/* 
 * Prototyping
 */
signed short int sp1285_startUpStep (
	signed         char FAR  *lpszFileName,
	signed         char FAR  *fuMode,
	FILE           FAR * FAR *pulFileHandle);

int usr_ros (
    unsigned char FAR *rcrd,
    unsigned long	len);

signed short int sp1286_cleanUpStep (
	FILE          FAR *pulFileHandle);

int main(int argc, char *argv[]);

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: main            				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */


main(
	int   argc,             /* number of command arguments        */
	char *argv[])           /* pointer to command arguments       */


/*                                                                    */
/* DESCRIPTION  :  main function for STEP evaluation program          */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
/*                                                                    */
/* RETURN VALUES              :                                       */
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
rT0060_LogRecHead           stRecHd;
FILE                      * fp      = NULL;
signed short int            n       = NULL;
signed short int            rc      = NULL;
unsigned long int           hPtr    = NULL;
              char  FAR   * pPtr    = NULL;
int filter = 0 ;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

 if (argc < 2){
	filter = 1 ;
	fp = stdin ;
	setbuf (stdin, NULL) ;
	setbuf (stdout, NULL) ;
/* ### */
/*
       fprintf(stderr,MSGUSAGE, argv[0]); */    /* print help text      */
/*
       exit(1);
*/
/* $$$ */
 }
/* ********** Input is OK then evaluate ALL the input files ******** */

 for (n=1 ; n < argc || filter; ++n) {
    if (!filter && (fp = fopen (argv[n],"r") ) == NULL )
        fprintf (stderr,MSGOER, argv[n]);
    else {
	if (!filter) {
	    /* print comment box with opened tracefile name               */
	    printf("%s\n",stars);
	    printf("* %s%-*s *\n", MSGOPN, 76-sizeof(MSGOPN), argv[n]);
	    printf("%74.74s%d**\n",stars,STEP_VERSION);
	}
        while (fread ( &stRecHd , sizeof(rT0060_LogRecHead), 1 , fp ) ==1 ) {
              if (  (stRecHd.lLength < NULL ) ||
                    (stRecHd.lLength > MAXRECLEN  )   ) {
	          printf(MSGRER);                          /* read error */
	          fprintf(stderr, MSGRERF, argv[n]);       /* read error */
                  break;
              }
              else {/* allocate mem for Length + sizeof (logRechead) */
                   if  ((hPtr = ro0020_globalalloc ( stRecHd.lLength +
                                              sizeof(rT0060_LogRecHead) ))
                                != NULL ) 
                        if( (pPtr = (char FAR  *)ro0021_globallock (hPtr) )  
                                != NULL ){
                          ro0041_memcpy(   pPtr            ,
                                           (char *)&stRecHd         ,
                                           (unsigned long int) 
                                              sizeof(rT0060_LogRecHead));
                          if (fread ( pPtr+sizeof(rT0060_LogRecHead),
                                      sizeof (char)                 ,
                                      stRecHd.lLength               ,
                                      fp                            ) <
                                    stRecHd.lLength                    ){
	                      printf(MSGRER);                    /*read error*/
	                      fprintf(stderr, MSGRERF, argv[n]); /*read error*/
                           }
                           else { /* successful read */
				FILE FAR * FAR *pulFileHandle;
                                rc= sp1285_startUpStep(
					(signed char FAR *)"logfile.l1",
                                        (signed char FAR *)"w+",
					pulFileHandle);
                                if (!rc) {
                                    rc =usr_ros ( (unsigned char FAR *)pPtr, 
					      (unsigned long)(stRecHd.lLength +
                                                   sizeof(rT0060_LogRecHead)));
                                }
                                if (!rc) {
				    FILE FAR *pulFileHandle;
                                    rc = sp1286_cleanUpStep(pulFileHandle);
                                }
                                if (rc) {
                                    printf(MSGRER);              
                                    fprintf(stderr, MSGRERF, argv[n]); 
                                }
                 
                            }
                            ro0022_globalunlock(hPtr);
                            ro0024_globalfree  (hPtr);
                          }
                          else   /* memory error */
	                       fprintf(stderr,MSGMEMER);             
                    else /*memory error */
	                  fprintf(stderr,MSGMEMER);             
                  }/*end of else of legal length */
		  if (filter) {
			/* print filter operation termination character */
			fputc ('\0', stdout) ;
		  }
        }/*end while loop      */
    }/*end of else of successful open */
    if (!filter) {
	fclose(fp);
	/* print comment box with closed tracefile name               */
	printf("%s\n",stars);
	printf("* %s%-*s *\n", MSGCLS, 76-sizeof(MSGCLS), argv[n]);
	printf("%s\n",stars);
	/* print empty line if there are further files to evaluate    */
	if (n !=  argc-1)
            printf("\n");
    }
    filter = 0 ;
  } /*end of for loop */

}/*end*/                                                                        
