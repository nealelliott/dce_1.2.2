/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rologset.c,v $
 * Revision 1.1.4.2  1996/02/18  18:22:23  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:45:12  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:09:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:18:23  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:07:30  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  15:02:10  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:15  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:47:25  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:30:34  keutel
 * 	creation
 * 	[1994/03/21  13:16:54  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: rologset.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:22:23 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1990     */
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
/* TYPE         : PROGRAM                                             */
/*                                                                    */
/* NAME         : rtlogset.c                                          */
/*                                                                    */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : JUN-8th-1992                                        */
/*                                                                    */
/* COMPONENT    : LOGGING                                             */
/*                                                                    */
/* PRD#/VERS.   : RTS-V2.0                                            */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* SYSTEM DEPENDENCE :  MAKRO konstant BS2000 defined    : for BS2000 */
/*                      MAKRO konstant BS2000 not defined: for SINIX  */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  |mark| CR# EM# */
/* 1.00     | 92-06-08|  original                      | gj |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                     Module Identification                          */
/**********************************************************************/

static char *sccsid =    "@(#) rologset.c 1.1 93/11/29   RTS-V2.00";

/*********************************************************************/
/*              I N C L U D E S                                      */
/*********************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <stdio.h>                                   /* STD-Includes */
#include <stdlib.h>
#include <string.h>

#include <rogenrl.h>                                 /* RTS-Includes */
#include <routil3.h>

#ifdef __MSDOS__
#include <conio.h>
#endif

/*********************************************************************/
/*              D E F I N E S                                        */
/*********************************************************************/

#define END                 99
#define MAX_LOG             16
#define MIN_LOG             0

#ifdef __MSDOS__
#define CLS                 clrscr()
#else
#define CLS                 printf("\x1B[2J")        /* clear screen */
#endif


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : main                                                */
/*                                                                    */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : JUN-8th-1992                                        */
/*                                                                    */
/* SYNTAX       :                                                     */

  void main (void)

/*                                                                    */
/* DESCRIPTION  :   Setup of the Logging config-file                  */
/*                                                                    */
/* IN-PARAMETERS:                                                     */
/*                                                                    */
/* OUT-PARAMETERS:                                                    */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :                                       */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/
/* ****************** l o c a l    v a r i a b l e s **************** */
  FILE *                                  file_ptr;
  int                                            i;
  static unsigned short     read_option[MAX_LOG+1];
  int                                        input;
  rs0004_configpar                rv0004_configpar;
  char                                    buf[120];
/* ******************                                **************** */

  char *log_file = RTS_LOGCFGNAME;

/* ***************    Read an existing config-file    *************** */
/* ***************       and show the settings        *************** */

  CLS;
  printf("\n");
  printf("\t========================================================\n");
  printf("\t             SETUP for RTS-V2.0 Logging                 \n");
  printf("\t========================================================\n\n");


  if ((file_ptr = fopen(log_file,"rb") ) == NULL )
     {
      printf("No config-file <%s> found! \n",log_file);
      printf("All values assigned with defaults!!");
      rv0004_configpar.loglevel=0;
      rv0004_configpar.recnummax=256;
      rv0004_configpar.confreadmax=64;
      strcpy((char FAR *)rv0004_configpar.filename,(char FAR *)"rtslog");
     }
  else
     {
      fread ((void*)&rv0004_configpar,sizeof(rv0004_configpar),1,file_ptr);
      fclose (file_ptr);
      printf ("Current configuration was read from <%s>! \n\n",log_file);
     }
  printf ("\n\tCurrent Configuration for RTS-Logging is:\n");
  printf ("\n\tLogging Level: %u",rv0004_configpar.loglevel);
  printf ("\n\tMax number of records in one logging file: %u",
	       rv0004_configpar.recnummax);
  printf ("\n\tNumber of records to reread the config-file: %u",
	       rv0004_configpar.confreadmax);
  printf ("\n\tName of the logging file: %s\n",
	       rv0004_configpar.filename);
  printf("\n\tPlease press [RETURN] to continue!");
  getc(stdin);

/* ******************   Build up the bit-mask        **************** */

    for (i = 0;i < MAX_LOG; i++)
	 if (rv0004_configpar.loglevel & (1 << i))
	     read_option[i] = 1;

/* ******************  Change Logging levels         **************** */

do  {
    CLS;
    printf ("\nThe following logging levels are available: \n\n");
    printf ("\tBit [1] = TRACE logging        |   Bit [9]  = user level  5\n");
    printf ("\tBit [2] = INTERFACE  logging   |   Bit [10] = user level  6\n");
    printf ("\tBit [3] = ACTIVITY   logging   |   Bit [11] = user level  7\n");
    printf ("\tBit [4] = CHECK&WAIT logging   |   Bit [12] = user level  8\n");
    printf ("\tBit [5] = user level  1        |   Bit [13] = user level  9\n");
    printf ("\tBit [6] = user level  2        |   Bit [14] = user level 10\n");
    printf ("\tBit [7] = user level  3        |   Bit [15] = user level 11\n");
    printf ("\tBit [8] = user level  4        |   Bit [16] = user level 12\n");
    printf ("\t\t [0] =  Reset all bits to 0! (switch off logging) \n\n");
    printf ("\t\t [99] =  END of Selection \n\n");
    printf ("Current Bit-Mask:\n");
    printf ("\tBit-No.  :   1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16\n");
    printf ("\t-----------------------------------------------------------\n");
    printf ("\tLevel set: ");
    for (i = MIN_LOG ;i < MAX_LOG; i++)
	 printf ("  %.1d",read_option[i]);
    printf ("\n");
    printf ("\n\tPlease insert the Bit number to set or unset! ");
    printf ("\n\t(Example: 3 sets activity level,-3 resets activity level) ");
    gets((char*)buf);
    if (strlen((char*)buf) !=0)
       {
	input = atoi((char*)buf);
	if (input == 0)                        /* reset all bits to 0 */
	   {
	    for (i = 0;i < MAX_LOG; i++)
		 read_option[i] = 0;
		 rv0004_configpar.loglevel=0;   /* switch off logging */
	   }
	if (input > 0)                               /* set the level */
	   {
	    if ((input > MIN_LOG) && (input <= MAX_LOG))
	       {
		read_option[--input]= 1;
		rv0004_configpar.loglevel |= (1 << input);
	       }
	   }
	if (input < 0)                                /* clear level */
	   {
	    input *= -1;                     /* indexes are positive */
	    if ((input > MIN_LOG) && (input <= MAX_LOG))
	       {
		read_option[--input] = 0;
		rv0004_configpar.loglevel &= (~(1<<input));
	       }
	   }
       }
    } while(input != END);

/* **********  Number of records to reread configuration  *********** */

    CLS;
    printf ("\n\tPlease enter number of records after which\n ");
    printf ("\tthe configuration-file shall be read again \n ");
    printf ("\t[Current value = %hd] : ",rv0004_configpar.confreadmax);
    gets((char*)buf);
    if (strlen((char*)buf) !=0)
	rv0004_configpar.confreadmax = atoi((char*)buf);
    else
	printf("value left unchanged!");

/* ***************  Change number of records in file   ************** */

    printf ("\n\n\tPlease enter max number of records within\n ");
    printf ("\tone logging file: \n");
    printf ("\t[Current value = %hd] : ",rv0004_configpar.recnummax);
    gets((char*)buf);
    if (strlen((char*)buf) !=0)
	rv0004_configpar.recnummax = atoi((char*)buf);
    else
	printf("value left unchanged!");

/* ***************  Change number of records in file   ************** */

    printf ("\n\n\tPlease enter name of logfile ( <120 Bytes ):\n");
    printf ("\t[Current name = %s] : ",rv0004_configpar.filename);
    gets((char*)buf);
    if (strlen((char*)buf) !=0)
	strcpy((char FAR *)rv0004_configpar.filename,(char FAR *)buf);
    else
	printf("name left unchanged!");

/* ***************  Write configuration and exit   ****************** */

    if ((file_ptr = fopen(log_file,"wb+") ) == NULL)
	printf ("Sorry! File %s couldn't be opened!\n",log_file);
    else
       {
	fwrite((void*)&rv0004_configpar,sizeof(rs0004_configpar),1,file_ptr);
	fclose(file_ptr);
	printf("\n\tConfiguration written to <%s>!",log_file);
	printf ("\n\nConfiguration for RTS-Logging will be:\n");
	printf ("\n\tLogging Level: %u",rv0004_configpar.loglevel);
	printf ("\n\tMax number of records in one logging file: %u",
		     rv0004_configpar.recnummax);
	printf ("\n\tNumber of records to reread the config-file: %u",
		     rv0004_configpar.confreadmax);
	printf ("\n\tName of the logging file: %s\n",
		     rv0004_configpar.filename);
       }
    printf ("\nE N D  of Program.\n");

}/*end*/

