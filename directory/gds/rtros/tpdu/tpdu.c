/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: tpdu.c,v $
 * Revision 1.1.4.2  1996/03/11  13:23:43  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:14  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:45:39  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:24:23  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:14:44  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:53:47  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:02:52  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:09:44  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:46:35  keutel
 * 	creation
 * 	[1994/03/21  13:38:23  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: tpdu.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:23:43 $" } ;
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

static char *sccsid =    "@(#) tpdu.c 1.3 94/01/19   ROS-V03.00";

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

/********************************************************/
/*      E X P O R T S                                   */
/********************************************************/


TP_short FAR  PASCAL tp2_001_open     PROTO ((Ptp1001 FAR *));
TP_short FAR  PASCAL tp2_002_read     PROTO ((Ptp1002 FAR *));
TP_short FAR  PASCAL tp2_003_write    PROTO ((Ptp1003 FAR *));
TP_short FAR  PASCAL tp2_005_close    PROTO ((Ptp1005 FAR *));

void     NEAR PASCAL tp_clear_table   PROTO ((void));
void     NEAR PASCAL tp_create_table  PROTO ((TP_short));

/********************************************************/
/* T P D U   F U N C T I O N   D E F I N I T I O N S    */
/********************************************************/


/************************************************/
/*      TP-OPEN                                 */
/************************************************/

TP_short FAR PASCAL tp2_001_open (
	Ptp1001  FAR      * pb)
{
  int           i;

  VERSIONSTEST;

  /*********** freien Platz in der Liste suchen *************/

  if (list_length == NULL) {
      tp_create_table ( TP_TABLE_SIZE );
  }

  for (i=0; i < list_length ; i++ )
  {
    if (file_list[i].used == UNUSED)  break;
  }

  if ( i >= list_length )
  {
     pb->tp_pbhead->tperr_value = TPINTERR;
     pb->tp_pbhead->tperr_class = TP_NOT_SPEC;
     return pb->tp_pbhead->tpret_code = TP_TEMP_ERR;
  }

  /******* APDU Deskriptor als Dateinamen interpretieren *****/
  /******* SKH: WARNING !! Our TEST program provides an  *****/
  /*******      allocated memory of actual strlen + 1.   *****/
  /*******      Therefore, no problem in appending a     *****/
  /*******      '\0' to the string. Otherwise, in a real *****/
  /*******      situation, only the no. of binary bytes  *****/
  /*******      that specify the name of the TPDU file   *****/
  /*******      (and not ASCII bytes, as this impl.      *****/
  /*******      assumes) are allocated. In that case,    *****/
  /*******      this implementation wont work !!         *****/

  pb->tpdata_description[ (*pb->tplen_data_description)-1 ]  = '\0';

  /************ Listeneintrag vornehmen **********************/

  file_list[i] . dir            = * pb -> tpdir;
  file_list[i] . name_len       = * pb -> tplen_data_description;
/*
  file_list[i] . filename       = tp_malloc((* pb->tplen_data_description)+1);
*/
  tp_strcpy ( (char FAR *)file_list[i].filename, pb->tpdata_description);

  /************ Datei oeffnen ********************************/

  switch        ( * pb->tpdir ) {

  case  TPIN : {
    if((file_list[i].file = tp_fopen(pb->tpdata_description, OP_READ))
	==  NULL)
    {
      RETURN( TP_CALL_ERR, TP_PAR_ERR, TPFILENAME);
    }
    break;
  }

  case  TPOUT : {
    if((file_list[i].file = tp_fopen(pb->tpdata_description,OP_CREATE))
	==  NULL)
    {
      RETURN( TP_PERM_ERR, TP_NOT_SPEC, TPINTERR);
    }
    break;
  }

  default:
	RETURN( TP_CALL_ERR, TP_PAR_ERR, TPDIRUSE  );

  }/*switch-end*/

  file_list[i] . used = USED;

  file_list[i].file_len = tp_filelen ( file_list[i].file );

  /*************** Rueckgabeparameter setzen *****************/

  * pb->tpopenid        = i;

#if defined TPDU_FILELENGTH
{
  char string [1024];

  switch (prompt_tpdu) {
  case ALWAYS_DEFINITE_FILELENGTH   :
      *(pb->tpfilelength) = file_list[i].file_len;
      printf("Length \"%s\" is %d\n",file_list[i].filename,*(pb->tpfilelength));
      break;

  case ALWAYS_INDEFINITE_FILELENGTH :
      *(pb->tpfilelength) = -1;
      printf("Length \"%s\" is %d\n",file_list[i].filename,*(pb->tpfilelength));
      break;

  default :
      printf ("Indefinite tpdu filelength ? [y/n/xy/xn]: Default xn ? ");
      scanf ("%s", string);

      if (strncmp (string, "y", 1) == 0) {
	  prompt_tpdu = INDEFINITE_FILELENGTH;
	  *(pb->tpfilelength) = -1;
      }
      else if (strncmp (string, "n", 1) == 0) {
	  prompt_tpdu = DEFINITE_FILELENGTH;
	  *(pb->tpfilelength) = file_list[i].file_len;
      }
      else if (strncmp (string, "xy", 2) == 0) {
	  prompt_tpdu = ALWAYS_INDEFINITE_FILELENGTH;
	  *(pb->tpfilelength) = -1;
      }
      else {
	  prompt_tpdu = ALWAYS_DEFINITE_FILELENGTH;
	  *(pb->tpfilelength) = file_list[i].file_len;
      }
      scanf ("%c", string);
      printf("Length \"%s\" is %d\n",file_list[i].filename,*(pb->tpfilelength));
      break;
  }
}
#elif defined RP0000_MicV0600

  
  *(pb->tpfilelength)   = (bDefiniteLengthTpdu)
                        ? file_list[i].file_len
                        : -1;

#else

  *(pb->tpfilelength)   = file_list[i].file_len;

#endif

  RETURN( TP_NO_ERR, TP_NO_ERR, TP_NO_ERR );

}


/************************************************/
/*      TP-READ                                 */
/************************************************/

TP_short FAR PASCAL  tp2_002_read (
	Ptp1002  FAR       * pb)
{
  int   i       = * pb->tpopenid;

  if( (i >= list_length) || (file_list[i].used != USED) )
  {
    RETURN( TP_CALL_ERR, TP_PAR_ERR, TPOPENID);
  }

  * pb->tpread_data_length = (TP_long)
	tp_fread ( pb -> tpinbufferaddress,
                (unsigned long int)1,
		(unsigned long int)* pb-> tpinrequired_data_length,
		file_list[i].file );

  if ( *pb->tpread_data_length == LNULL )
  {
    RETURN( TP_PERM_ERR, TP_NOT_SPEC, TPINTERR);
  }

/* **SKH: Does not work if the no. of bytes read is exactly the same as that
	  in the file

  * pb->tpinfurther_data =
	feof( file_list[i].file )  ?
	TP_NO  :  TP_YES ;
*/

{ /* This section of code changed by SKH  to take care of the above error */

  signed long int     current_file_pos = 0L;
  signed long int     end_of_file_pos  = 0L;

  current_file_pos = tp_fseek ( file_list[i].file, 0L, SEEK_CUR );
  end_of_file_pos  = tp_fseek ( file_list[i].file, 0L, SEEK_END );

  if ( (current_file_pos < -1) || (end_of_file_pos < -1) )
  {
        RETURN( TP_PERM_ERR, TP_NOT_SPEC, TPINTERR);
  }

  * pb->tpinfurther_data =
	( current_file_pos < end_of_file_pos )   ?   TP_YES :  TP_NO ;

  current_file_pos = tp_fseek ( file_list[i].file, current_file_pos, SEEK_SET );
}

  RETURN( TP_NO_ERR, TP_NO_ERR, TP_NO_ERR);

}

/************************************************/
/*      TP-WRITE                                */
/************************************************/

TP_short FAR PASCAL tp2_003_write   (
	Ptp1003  FAR      * pb)
{
  int   i       = * pb->tpopenid;

  if( (i >= list_length) || (file_list[i].used != USED) )
  {
    RETURN( TP_CALL_ERR, TP_PAR_ERR, TPOPENID);
  }

  if( file_list[i].dir != TPOUT )
  {
    RETURN( TP_CALL_ERR, TP_ILL_STAT, TPDIRUSE);
  }

  * pb-> tpwritten_data_length =
	tp_fwrite ( pb-> tpoutbufferaddress,
                 (unsigned long int)1,
                 (unsigned long int)* pb-> tpoutrequired_data_length,
		 file_list[i].file );

  if ( *pb->tpwritten_data_length == -1 )
  {
    RETURN( TP_PERM_ERR, TP_NOT_SPEC, TPINTERR);
  }

  * pb -> tpoutfurtherdataspace = TP_YES;

  if ( * pb->tpsecure ) tp_fflush( file_list[i].file );

  RETURN( TP_NO_ERR, TP_NO_ERR, TP_NO_ERR );
}


/************************************************/
/*      TP-CLOSE                                */
/************************************************/

TP_short FAR PASCAL tp2_005_close   (
	Ptp1005  FAR      * pb)
{

  int   i       = * pb->tpopenid;

  if( (i >= list_length) || (file_list[i].used != USED) )
  {
    RETURN( TP_CALL_ERR, TP_PAR_ERR, TPOPENID);
  }

  if (tp_fclose( file_list[i].file ))
  {
    RETURN( TP_CALL_ERR, TP_PAR_ERR, TPOPENID);
  }

/*
  free( file_list[i].filename );
*/

  file_list[i].used  =  UNUSED;
  file_list[i].file  =  LNULL;

  RETURN( TP_NO_ERR, TP_NO_ERR, TP_NO_ERR );
}


/************************************************/
/*      TP-CLEAR_TABLE                          */
/************************************************/

void NEAR PASCAL tp_clear_table  (void )
{
  int   i;

  if ( file_list != (Filedata *)NULL )
  {
    for (i=0 ; i < list_length ; i++ )
    {
      file_list[i].used = UNUSED;
    }
  }
}

/************************************************/
/*      TP-CREATE-TABLE                         */
/************************************************/

void NEAR PASCAL tp_create_table ( 
	TP_short        anz)
{ 
/*
  if ( ! file_list )
  {
    file_list  =  (Filedata *) tp_malloc( anz * sizeof(Filedata) );
    list_length = file_list ? anz : 0 ;
  }
*/
list_length = TP_TABLE_SIZE;
tp_clear_table();
}
