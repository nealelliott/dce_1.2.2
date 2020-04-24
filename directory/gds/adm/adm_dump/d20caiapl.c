/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20caiapl.c,v $
 * Revision 1.1.10.2  1996/02/18  19:38:28  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:19:23  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:10:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:27  root]
 * 
 * Revision 1.1.8.3  1994/08/01  15:31:40  marrek
 * 	Bug fixes for OT 11295, 11456, 11458, 11471.
 * 	[1994/08/01  15:28:48  marrek]
 * 
 * Revision 1.1.8.2  1994/06/21  14:43:09  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:22:11  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  15:51:01  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:11:39  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  08:15:53  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:02:21  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  17:43:32  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:57:36  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:43:23  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:36:03  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20caiapl.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:38:28 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODUL                                               */
/*								      */
/* NAME         : d20caiapl.c                                         */
/*								      */
/* AUTHOR       : Volpers,D AP 11                                     */
/* DATE         : 26.04.88                                            */
/*								      */
/* KOMPONENTE	: DS						      */
/*								      */
/* DOK.-NR.	: DS-Design-Spez.				      */
/*			.					      */
/*			.					      */
/*								      */
/* PRD#/VERS.	: <Produktnr. aus Produktstruktur> - <Versionsnr.>    */
/*								      */
/* DESCRIPTION  :                                                     */
/*								      */
/* SYSTEMABHAENIGKEIT:                                                */
/*                      COMMON                                        */
/*								      */
/* HISTORIE	:						      */
/*								      */
/* Vers.Nr. |  Datum  |	 Aenderungen		       | KZ | CR# FM# */
/*      0.1 | 15.04.88| Erstellung                     | hv |         */
/*								      */
/*datoff **************************************************************/

/**********************************************************************/
/*								      */
/*		       Modulidentification			      */
/*								      */
/**********************************************************************/




/**********************************************************************/
/*								      */
/*			  I N C	L U D E	S			      */
/*								      */
/**********************************************************************/

#include <gds.h>

#include <stdio.h>

#include <dce/d2dir.h>
#include <d21iapl.h>
#include <dce/d27util.h>
#include <gdsext.h>
#include <d2shm.h>
#include <d2adm.h>
#include <d2dump.h>

/**********************************************************************/
/*								      */
/*			  D E F	I N E S				      */
/*								      */
/**********************************************************************/


/**********************************************************************/
/*								      */
/*		   T Y P  -  D E F I N I T I O N E N		      */
/*								      */
/**********************************************************************/



/**********************************************************************/
/*								      */
/*		   D E C L A R A T I O N S  	  DATEN		      */
/*								      */
/**********************************************************************/

extern D20_omask d20_qomask[];         /* Input fields for error mask */
extern char      *d20_errtab[];        /* Error mask                  */
extern char      *d20_errmess[];       /* Error mask                  */

extern  D2_c_arg    *d20_p_c_arg;    /* pointer to common arguments        */
extern  D2_credent  *p_usercred;
extern  short       d20_bid;         /* current bind-id                    */
extern  signed16    d20_dir_id;     /* directory identifier                */


/**********************************************************************/
/*								      */
/*		  D E C	L A R A	T I O N	S  	 FUNKTIONEN	      */
/*								      */
/**********************************************************************/

extern short d21_1_bind();
extern short d21_2_unbind();


 short d20_163_bind();

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNKTION                                            */
/*								      */
/* NAME       : d20_163_bind                                          */
/*								      */
/* AUTHOR:      Volpers   (D AP 11)                                   */
/* DATE:        26.04.88                                              */
/*								      */
/* SYNTAX:      short d20_163_bind(b_id, dsa_disname, h_fld)          */
/*                                                                    */
/*              short   *b_id;                                        */
/*              char    *dsa_disname;                                 */
/*              char    *h_fld;                                       */
/*                                                                    */
/* DESCRIPTION: This function make the bind to the special dsa        */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*								      */
/*       0      :       no error                                      */
/*      -1      :       break is selected                             */
/*								      */
/* STANDARD-HEADER: None                                              */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/*exoff ***************************************************************/

   short d20_163_bind(b_id, dsa_disname, h_fld, psap_len)

short   *b_id;
char    *dsa_disname;
char    *h_fld;
short   psap_len;

{                               /*  d20_163_bind                      */

/**********************************************************************/
/*								      */
/*		   T Y P  -  D E F I N I T I O N E N		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*		    D E	K L A R	A T I O	N E N			      */
/*								      */
/**********************************************************************/

   short ret_value = D2_NOERROR;
   char        *ptr;
   D21_p1      plbind;
   D2_a_info   address;
				     /* psap address of dsa                */
   D2_a_value  value;            /* value of psap address              */
   D2_pbhead   err_inf;         /* Fehler-Bereich                      */

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/
#ifdef TRACE
   fprintf(stderr,"in d20_163_bind\n");
   fflush(stderr);
#endif

		/* fill in bind-parameters */
plbind.d21_1pbhead = &err_inf ;
err_inf.d2_version = D21V02;
plbind.d21_1dir_id = d20_dir_id ;
plbind.d21_1u_credent = p_usercred ;

	     /* bind to a specific DSA -> put dsa address to bind block */
plbind.d21_1name_dsa = (D2_name_string) dsa_disname;
plbind.d21_1addr_dsa = &address ;
address.d2_a_val = &value ;
address.d2_a_next = NULL ;
address.d2_a_type.d2_typ_len = sizeof(D2_PSAP_ADDRESS)-1;
address.d2_a_type.d2_type = (Ob_id_string) D2_PSAP_ADDRESS;
address.d2_a_no_val = 1 ;
value.d2_a_v_len = psap_len ;
value.d2_at_value.d2_oct_a_v = (Octet_string) h_fld ;

if (d21_1_bind (&plbind) == D2_NO_ERR)
{                           /* error doing bind to default DSA */
			    /* save bind-id */
  *b_id = plbind.d21_1bind_id ;
  return(D2_NOERROR);
}
else
{
  Switch ((int) err_inf.d2_errvalue)
    case D2_DIR_UNKNOWN:
	 d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab[D20_E52IDX] ;
	 break ;
    case D2_CREDENTIALS_INVALID:
	 d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab[D20_E53IDX] ;
	 break ;
    case D2_DSA_UNKNOWN:
	 d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab[D20_E55IDX] ;
	 break;
    default:
	 d20_qomask[D20_Q1POS].d20_ioinfo = d20_errmess[0] ;
    }   
    d20_handlemask (d20_qomask) ;
#ifdef TRACE
    fprintf(stderr,"d20caiapl: bind fails\n");
    fflush(stderr);
#endif
    return(D2_ERROR);
} 

}                               /*  d20_163_bind                      */

