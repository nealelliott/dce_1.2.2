/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: tpdu1.c,v $
 * Revision 1.1.4.2  1996/03/11  13:23:45  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:16  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:45:47  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:24:27  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:15:33  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:53:50  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:02:54  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:09:52  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:46:39  keutel
 * 	creation
 * 	[1994/03/21  13:38:29  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: tpdu1.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:23:45 $" } ;
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
/* NAME         : tpdu1.c                                             */
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

static char *sccsid =    "@(#) tpdu1.c 1.1 94/01/19   ROS-V03.00";

/**********************************************************************/
/*		I N C L	U D E S					      */
/**********************************************************************/


#ifdef THREADSAFE
#include <pthread.h>
#endif

#include        <stdio.h>
#include        <errno.h>
#include        <fcntl.h>
#include        <unistd.h>

	/**** System -Includes  ****/

#include        <rogenrl.h>
#include        <tpdu.h>
#include        <tpdu1.h>
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
#define MAX_FILENAMELEN    128                           
                                                         
typedef struct  {                                        
        TP_char             filename [ MAX_FILENAMELEN ];
        TP_short            name_len;                    
        Tpdir               dir;                         
        TP_long             file_len;                    
        unsigned long int   file;                        
        TP_short            used;                        
} Filedata ;                                             

#define TP_TABLE_SIZE        100 

int  list_length = NULL;
Filedata file_list[TP_TABLE_SIZE];

#define         UNUSED          0  
#define         USED            1  
                                   
#ifndef TRUE                       
#define         TRUE            1  
#endif                             
                                   
#ifndef FALSE                      
#define         FALSE           0  
#endif                             

#define RETURN( RC, CLASS, VAL)                          \
        pb->tp_pbhead->tperr_value = VAL;                \
        pb->tp_pbhead->tperr_class = CLASS;              \
        return pb->tp_pbhead->tpret_code = RC
              
                                                           

TP_short FAR  PASCAL tp2_004_position PROTO ((Ptp1004 FAR *));
TP_short FAR  PASCAL tp2_006_delete   PROTO ((Ptp1006 FAR *)); 

/************************************************/
/*      TP-POSITION                             */
/************************************************/

TP_short FAR PASCAL tp2_004_position( 
	Ptp1004  FAR      * pb)
{
  int   i       = * pb->tpopenid;
  signed long int   rc  = LNULL;

  if( (i >= list_length) || (file_list[i].used != USED) )
  {
    RETURN( TP_CALL_ERR, TP_PAR_ERR, TPOPENID);
  }

    /* rel.zum Dateianfang */
  rc = tp_fseek( file_list[i].file, * pb-> tpoffset, SEEK_SET );
  if (rc == -1)
  {
      RETURN( TP_CALL_ERR, TP_ILL_STAT, TPDIRUSE );
  }

  RETURN( TP_NO_ERR, TP_NO_ERR, TP_NO_ERR );
}


/************************************************/
/*      TP-DELETE                       */
/************************************************/

TP_short FAR PASCAL tp2_006_delete  (
	Ptp1006  FAR      * pb)
{
/**************************/
#ifndef RP0000_MicV0600 /**/
/**************************/

  signed       int  i               = * pb->tpopenid;
  signed short int  bDeleteFilePart =   FALSE;


  if( (i >= list_length) || (file_list[i].used != USED) )
  {
    RETURN( TP_CALL_ERR, TP_PAR_ERR, TPOPENID);
  }

  if( file_list[i].dir != TPOUT )
  {
    RETURN( TP_CALL_ERR, TP_ILL_STAT, TPDIRUSE);
  }

  /**** **SKH: Code for deleting file introduced *************/

  /**** update the file length for the subsequent ************/
  /**** analysis of the parameters.               ************/
  /**** Note: File lengths are not updated during ************/
  /****       reads and writes. They are updated  ************/
  /****       only on opens and deletes.          ************/

  /**** close the file first *********************************/


  tp_fseek( file_list[i].file, 0, SEEK_END );      /* auf das Dateiende */
  file_list[i].file_len = tp_filelen( file_list[i].file );

/*   **** commented for GCC  **************
{*entr-deleteBlock*

  int fd = 0;
  flock_t  delete_info;

  **** set up amount of data to be deleted ******************

  if ( * pb->tprequired_data_length <= 0 ) {
      **** all bytes to be deleted ***********************

      if (tp_fclose( file_list[i].file )) {
	RETURN( TP_CALL_ERR, TP_ILL_STAT, TPINTERR);
      }
      bDeleteFilePart       = TRUE;

      delete_info.l_whence  = SEEK_SET;
      delete_info.l_start   = 0;
      delete_info.l_len     = 0; * 0 => free space upto end of file *
      file_list[i].file_len = 0;

  }
  else if ( * pb->tprequired_data_length < file_list [i].file_len ) {
      **** delete part of file ***************************

      if (tp_fclose( file_list[i].file )) {
	RETURN( TP_CALL_ERR, TP_ILL_STAT, TPINTERR);
      }
      bDeleteFilePart       = TRUE;

      delete_info.l_whence  = SEEK_SET;
      delete_info.l_start   = *pb->tprequired_data_length;
      delete_info.l_len     = 0; * 0 => free space upto end of file *
      file_list[i].file_len = *pb->tprequired_data_length;
  }
  else {
      **** no bytes to be deleted  ***********************
      bDeleteFilePart       = FALSE;
  }

  if (bDeleteFilePart) {

      **** open file, delete data, close file *******************
      **** WARNING: fcntl (F_FREESP) may (?!) *******************
      **** not function under DOS or OS/2.    *******************
      **** In that case that function has to  *******************
      **** be replaced with an equivalent     *******************
      **** delete function.                   *******************

      fd = open (file_list [i].filename, O_RDWR);
      if (fd < 0) {
	RETURN( TP_CALL_ERR, TP_ILL_STAT, TPOPENID);
      }

      if (fcntl (fd, F_FREESP, &delete_info) < 0) {
	RETURN( TP_CALL_ERR, TP_ILL_STAT, TPINTERR);
      }

      if (close (fd) < 0) {
	RETURN( TP_CALL_ERR, TP_ILL_STAT, TPINTERR);
      }


     **** eopen the file **************************************

      file_list[i].file = tp_fopen( file_list [i].filename, OP_UPDATE);
      if (file_list[i].file ==  NULL) {
	RETURN( TP_PERM_ERR, TP_NOT_SPEC, TPINTERR);
      }

      tp_fseek ( file_list[i].file, 0, SEEK_END );  * auf das Dateiende *
      file_list[i].file_len = tp_filelen( file_list[i].file );

  }*if-end*

}*end-deleteBlock*
   end of GCC ********/
{/* delete block starts */
  int fd = 0;
   signed long int lLen=0;

  /**** set up amount of data to be deleted ******************/

  if ( * pb->tprequired_data_length <= 0 ) {
      /**** all bytes to be deleted ***********************/

      if (tp_fclose( file_list[i].file )) {
	RETURN( TP_CALL_ERR, TP_ILL_STAT, TPINTERR);
      }
      bDeleteFilePart       = TRUE;
      lLen = file_list[i].file_len; /* full length */
        file_list[i].file_len = 0;
  } /* delete all */
  else if ( * pb->tprequired_data_length < file_list [i].file_len ) {
      /**** delete part of file ***************************/

      if (tp_fclose( file_list[i].file )) {
	RETURN( TP_CALL_ERR, TP_ILL_STAT, TPINTERR);
      }
      bDeleteFilePart       = TRUE;

      lLen   = *pb->tprequired_data_length;
      file_list[i].file_len = *pb->tprequired_data_length;
  }
  else {
      /**** no bytes to be deleted  ***********************/
      bDeleteFilePart       = FALSE;
  }


  if (bDeleteFilePart) {

      fd = open (file_list [i].filename, O_RDWR);
      if (fd < 0) {
	RETURN( TP_CALL_ERR, TP_ILL_STAT, TPOPENID);
      }
      
      if (ftruncate (fd , lLen) < 0 )
  	RETURN( TP_CALL_ERR, TP_ILL_STAT, TPINTERR);
      if (close (fd) < 0) {
	RETURN( TP_CALL_ERR, TP_ILL_STAT, TPINTERR);
      }




     /**** reopen the file **************************************/

      file_list[i].file = tp_fopen( file_list [i].filename, OP_UPDATE);
      if (file_list[i].file ==  NULL) {
	RETURN( TP_PERM_ERR, TP_NOT_SPEC, TPINTERR);
      }

      tp_fseek ( file_list[i].file, 0, SEEK_END );  /* auf das Dateiende */
      file_list[i].file_len = tp_filelen( file_list[i].file );
 }/* end if */
}/* end-deleteblock */


  *pb->tpdata_length    = file_list[i].file_len;

/***************************/
#endif /* !RP0000_MicV0600 */
/***************************/


  RETURN( TP_NO_ERR, TP_NO_ERR, TP_NO_ERR );
}

