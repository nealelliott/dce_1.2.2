/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20spec_msk.c,v $
 * Revision 1.1.729.2  1996/02/18  19:41:27  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:22:08  marty]
 *
 * Revision 1.1.729.1  1995/12/08  15:17:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:52:13  root]
 * 
 * Revision 1.1.727.8  1994/08/23  15:27:36  keutel
 * 	OT 10919: remove gda
 * 	[1994/08/23  14:20:08  keutel]
 * 
 * Revision 1.1.727.7  1994/08/10  08:25:21  marrek
 * 	Fix for SAP errors, OT11467.
 * 	[1994/08/08  14:13:25  marrek]
 * 
 * Revision 1.1.727.6  1994/08/02  07:22:38  marrek
 * 	Fix double quotes around Z.
 * 	[1994/08/02  07:22:10  marrek]
 * 
 * Revision 1.1.727.5  1994/08/01  15:31:49  marrek
 * 	Bug fixes for OT 11295, 11456, 11458, 11471.
 * 	[1994/08/01  15:30:15  marrek]
 * 
 * Revision 1.1.727.4  1994/07/06  15:06:29  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:11:49  marrek]
 * 
 * Revision 1.1.727.3  1994/05/10  15:49:51  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:13:14  marrek]
 * 
 * Revision 1.1.727.2  1994/03/23  15:05:39  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:11:16  keutel]
 * 
 * Revision 1.1.727.1  1994/02/22  15:58:59  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:32:44  marrek]
 * 
 * Revision 1.1.725.2  1993/10/14  16:46:37  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  16:45:16  keutel]
 * 
 * Revision 1.1.725.1  1993/10/13  17:29:51  keutel
 * 	October 1993 code drop
 * 	[1993/10/11  07:36:29  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  10:25:24  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:33:55  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:09:24  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:05:19  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  13:19:13  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  14:14:10  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  22:01:56  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:17:16  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20spec_msk.c,v $ $Revision: 1.1.729.2 $ $Date: 1996/02/18 19:41:27 $";
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
/* NAME         : spec_msk.c                                          */
/*								      */
/* AUTHOR       : V. Mueller, SNI AP 11                               */
/* DATE         : 21.03.1991                                          */
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
/* SYSTEM-DEPENDENCIES:                                               */
/*                      COMMON                                        */
/*								      */
/* HISTORY 	:						      */
/*								      */
/* Vers.Nr. |  Date   |	 Updates    		       | KZ | CR# FM# */
/*      0.1 | 21.03.91| Erstellung                     | ws |         */
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
#include <stdlib.h>
#include <string.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <gdsext.h>
#include <d2adm.h>
#include <d2msk.h>
#include <dce/x500_name_utils.h>
#include <d21iapl.h>
#include <d2spec_msk.h>
#include <cget.h>
#include <d20proto.h>

/**********************************************************************/
/*								      */
/*			  D E F	I N E S				      */
/*								      */
/**********************************************************************/

#define D20_ONLY_SH_MASK1	4
#define MAX_RCERT_BODY		10	/* Limit of revoked certificate	*/
					/* attributes that can be add.	*/


/**********************************************************************/
/*								      */
/*		   D E C L A R A T I O N S       DATA		      */
/*								      */
/**********************************************************************/
static   signed32  fax_bits[] = { D2_A3_WIDTH, D2_B4_LENGTH, D2_B4_WIDTH,
				 D2_FINE_RESOLUTION, D2_TWO_DIMENSIONAL,
				 D2_UNCOMPRESSED, D2_UNLIMITED_LENGTH };

extern  char       d20_actopmsg[] ;
extern  char       d20_attr_name[] ;     /* attribute name in mask     */
extern  D20_omask  d20_qomask[];         /* input fields of error mask */
extern  char	  *d20_errtab[];         /* error table                */
extern  char      *d20_err_table[];      /* error table                */

					 /* mask fields                */
extern  char      *d20_ttxidifields[] ;
extern  char      *d20_telexifields[] ;
extern  char      *d20_t61ifields[] ;
extern  char      *d20_shifields[] ;
extern  char      *d20_faxifields[] ;
extern  char      *d20_dnscellifields[] ;
extern  char      *d20_dnsrepifields[] ;
					/* DME alternate address	*/
static	char      d20_dme_addr[(DME_ADDRESS_LEN * D20_T61_CONV_FACT) + 1];
extern	char      d20_dme_addr1[
		(DME_ADDRESS_FIRST_LINE_LEN * D20_T61_CONV_FACT) + 1];/*Line 1*/
extern	char      d20_dme_addr2[
		(DME_ADDRESS_LINE_LEN * D20_T61_CONV_FACT) + 1];      /*Line 2*/
extern	char      d20_dme_addr3[
		(DME_ADDRESS_LINE_LEN * D20_T61_CONV_FACT) + 1];      /*Line 3*/
extern	char      d20_dme_addr4[
		(DME_ADDRESS_LINE_LEN * D20_T61_CONV_FACT) + 1];      /*Line 4*/
extern	char      d20_dme_addr5[
		(DME_ADDRESS_LINE_LEN * D20_T61_CONV_FACT) + 1];      /*Line 5*/
					/* DME object identifiers	*/
extern	char      d20_dme_object_id[][DME_OBJECT_ID_LEN + 1];
extern  char      *d20_certifields[];	/* Certificate attribute fields	*/
extern  char      *d20_certpifields[];	/* Cert_pair attribute fields	*/
extern  char      *d20_certlifields[];	/* Cert_list attribute fields	*/
extern  char      *d20_rcertifields[];	/* Revoked Certificate attribute*/

extern  D20_omask  d20_ttxidomask[];
extern  D20_omask  d20_telexomask[];
extern  D20_omask  d20_t61omask[];
extern  D20_omask  d20_shomask[];
extern  D20_omask  d20_faxomask[];
extern  D20_omask  d20_dnscellomask[];
extern  D20_omask  d20_dnsrepomask[];
extern	D20_omask  d20_certomask[];	/* SAP Certificate mask		*/
extern	D20_omask  d20_certpomask[];	/* SAP Certificate Pair mask	*/
extern	D20_omask  d20_certlomask[];	/* SAP Certificate List mask	*/
extern	D20_omask  d20_rcertomask[];	/* SAP Revoke Certificate mask	*/
extern  D20_omask  d20_dmeomask[];	/* DME mask			*/
extern  Bool  	   d20_printable;

/**********************************************************************/
/*								      */
/*		  D E C L A R A T I O N S      FUNCTIONS	      */
/*								      */
/**********************************************************************/

/******** LOCAL  ********/
  static signed32 d2_cp_from_ttxid(D2_ttxid *ttxid);
  static signed32 d2_cp_from_telex(D2_telex_number *telex);
  static signed32 d2_cp_from_fax(D2_faxnumber *fax);
  static signed32 d2_cp_from_t61(D2_T61_seq *t61);
  static signed32 d2_cp_from_str_list( D2_str_list *str_list);
  static signed32 d2_cp_from_dnscell(x500_dns_cell_t *dns_cell);
  static signed32 d2_cp_from_dnsrep(x500_replica_t *dns_rep);
  static signed32 d2_cp_to_ttxid(D2_ttxid *ttxid);
  static signed32 d2_cp_to_telex(D2_telex_number *telex);
  static signed32 d2_cp_to_fax(D2_faxnumber *fax);
  static signed32 d2_cp_to_t61(D2_T61_seq *t61);
  static signed32 d2_cp_to_str_list(D2_str_list  *str_list);
  static signed32 d2_cp_to_dnscell(x500_dns_cell_t *dns_cell);
  static signed32 d2_cp_to_dnsrep(x500_replica_t *dns_rep);
  static signed32 DateToX208(signed32, signed32, signed32, signed32, signed32,
			signed32, UTC_time_string *);
  static signed32 X208ToDate(UTC_time_string, signed32*, signed32*,  signed32*,
					       signed32*, signed32*, signed32*);
  static signed32 algTypeAdmValueToX509Value(signed32, Ob_id_string *,
					       signed32 *);
  static signed32 X509ToAlgTypeAdm(Ob_id_string *, signed32, signed32 *);

/*inon ****************************************************************/
/*								      */
/* TYPE:        signed32                                              */
/*								      */
/* NAME  :      d20_ttxid()                                           */
/*								      */
/* AUTHOR:      V. Mueller  DI AP 11                                  */
/* DATE:        05.03.1991                                            */
/*                                                                    */
/* DESCRIPTION: signed32 d20_ttxid(op, attr_name, ttxid)              */
/*              signed32   op;                                        */
/*              char      *attr_name                                  */
/*              D2_ttxid  *ttxid;                                     */
/*								      */
/*              This function analyses the ttxid-structure and fills  */
/*              the fields of the mask.                               */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              signed32   op;                                        */
/*              char      *attr_name                                  */
/*              D2_ttxid  *ttxid;                                     */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*              D2_ttxid  *ttxid;                                     */
/*								      */
/* RETURNVALUE:         fkt      funktionkey                          */
/*								      */
/* STANDARD-HEADER:     None                                          */
/*								      */
/* GLOBAL  DATA  (read only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*                       d20_save_mask()                              */
/*                       d20_restore_mask()                           */
/*                       d2_cp_from_ttxid()                           */
/*                       d2_cp_to_ttxid()                             */
/*								      */
/* NOTES:                                                             */
/*								      */
/* SYNTAX       :                                                     */
/*								      */
/*inoff ***************************************************************/

 signed32 d20_ttxid(
   signed32  op,
   char     *attr_name,
   D2_ttxid *ttxid)

{                               /*  d20_ttxid                         */

/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*		    D E	K L A R	A T I O	N E N			      */
/*								      */
/**********************************************************************/

signed32           fkt, i;
D20_omask          sav_ttxidomask[D20_M_SIZE];

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

    for (i=0; i <= DS_TT_PRIV; i++)
	  *d20_ttxidifields[i] = D2_EOS;

    strcpy(d20_attr_name, attr_name);

    switch ((int) op) {
	 case DS_DELETE_ATTRIBUTE:
	       fkt = FKTCR;
	       break;

	 case DS_DISPLAY_OBJECTS:
	 case DS_DISP_OBJ:
	       d20_save_mask(d20_ttxidomask, sav_ttxidomask);

	       d2_cp_from_ttxid(ttxid);

			/* display ttxid-mask */
	       fkt = d20_handlemask(d20_ttxidomask);

	       d20_restore_mask(d20_ttxidomask, sav_ttxidomask);
	       break;

	 case DS_MODIFY_ATTRIBUTE:
	       *((signed32 *)d20_ttxidifields[DS_TT_CHG]) = D2_REP_AV;
	       d2_cp_from_ttxid(ttxid);

			/* display ttxid-mask */
	       for (;;) {
		     fkt = d20_handlemask(d20_ttxidomask);
		     if ((d20_ttxidifields[DS_TT_TERM][0] == '\0' ||
			d20_ttxidifields[DS_TT_TERM][0] == DS_DEFAULT_VALUE) &&
			fkt != FKTDEL) {
				d20_qomask[D20_Q1POS].d20_ioinfo=
					d20_err_table[D20_101IDX];
				d20_handlemask(d20_qomask);
				continue;
		     } else
			   	break;
	       }

	       if (fkt != FKTDEL)
		    d2_cp_to_ttxid(ttxid);
	       break;

	 case DS_ADD_OBJECT:
	 case DS_ADD_ATTRIBUTE:
	       for (;;) {
		     fkt = d20_handlemask(d20_ttxidomask);
		     if ((d20_ttxidifields[DS_TT_TERM][0] == '\0' ||
			d20_ttxidifields[DS_TT_TERM][0] == DS_DEFAULT_VALUE) &&
			fkt != FKTDEL) {
				d20_qomask[D20_Q1POS].d20_ioinfo=
					d20_err_table[D20_101IDX];
				d20_handlemask(d20_qomask);
				continue;
		     } else 
		   		break;
	       }

	       if (fkt != FKTDEL)
		    d2_cp_to_ttxid(ttxid);
	       break;

	 default:
	       return(D2_ERROR);
    }

return(fkt);

}                               /*  d20_ttxid()                       */


/*inon ****************************************************************/
/*								      */
/* TYPE:        signed32                                              */
/*								      */
/* NAME  :      d20_telex()                                           */
/*								      */
/* AUTHOR:      V. Mueller  DI AP 11                                  */
/* DATE:        05.03.1991                                            */
/*                                                                    */
/* DESCRIPTION: signed32 d20_telex(op, attr_name, telex)              */
/*              signed32          op;                                 */
/*              char             *attr_name                           */
/*              D2_telex_number  *telex;                              */
/*								      */
/*              This function analyses the telex-structure and fills  */
/*              the fields of the mask.                               */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              signed32          op;                                 */
/*              char             *attr_name                           */
/*              D2_telex_number  *telex;                              */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*              D2_telex_number  *telex;                              */
/*								      */
/* RETURNVALUE:         fkt      funktionkey                          */
/*								      */
/* STANDARD-HEADER:     None                                          */
/*								      */
/* GLOBAL  DATA  (read only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*                       d20_save_mask()                              */
/*                       d20_restore_mask()                           */
/*                       d2_cp_from_telex()                           */
/*                       d2_cp_to_telex()                             */
/*								      */
/* NOTES:                                                             */
/*								      */
/* SYNTAX       :                                                     */
/*								      */
/*inoff ***************************************************************/

 signed32 d20_telex(
   signed32         op,
   char            *attr_name,
   D2_telex_number *telex)

{                               /*  d20_telex                         */

/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*		    D E	K L A R	A T I O	N E N			      */
/*								      */
/**********************************************************************/

signed32           fkt, i;
Bool               telex_err;
D20_omask          sav_telexomask[D20_M_SIZE];

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

    telex_err = FALSE;
    for (i=0; i <= DS_TL_ANSW; i++)
	  *d20_telexifields[i] = D2_EOS;

    strcpy(d20_attr_name, attr_name);

    switch ((int) op) {
	 case DS_DELETE_ATTRIBUTE:
	       fkt = FKTCR;
	       break;

	 case DS_DISPLAY_OBJECTS:
	 case DS_DISP_OBJ:
	       d20_save_mask(d20_telexomask, sav_telexomask);

	       d2_cp_from_telex(telex);

	       fkt = d20_handlemask(d20_telexomask);

	       d20_restore_mask(d20_telexomask, sav_telexomask);
	       break;

	 case DS_MODIFY_ATTRIBUTE:
	       *((signed32 *)d20_telexifields[DS_TL_CHG]) = D2_REP_AV;
	       d2_cp_from_telex(telex);

				/* display telex-mask */
	       for(;;) {
	           fkt = d20_handlemask(d20_telexomask);
	           for (i=0; i <= DS_TL_ANSW; i++) {
			      if ((d20_telexifields[i][0] == '\0' ||
			           d20_telexifields[i][0] == DS_DEFAULT_VALUE) &&
			           fkt != FKTDEL) {
				        d20_qomask[D20_Q1POS].d20_ioinfo=
					        d20_err_table[D20_102IDX];
				        d20_handlemask(d20_qomask);
				        telex_err = TRUE;
				        break;
			       }
	           }
	           if (telex_err == TRUE) {
		        telex_err = FALSE;
		        continue;
	           } else
		        break;
	       }
	       if (fkt != FKTDEL)
			d2_cp_to_telex(telex);
	       break;

	 case DS_ADD_OBJECT:
	 case DS_ADD_ATTRIBUTE:
	       for (;;) {		/* display telex-mask */
		     fkt = d20_handlemask(d20_telexomask);
	 	     for (i=0; i <= DS_TL_ANSW; i++) {
			   if ((d20_telexifields[i][0] == '\0' ||
				d20_telexifields[i][0] == DS_DEFAULT_VALUE) &&
				fkt != FKTDEL) {
				    d20_qomask[D20_Q1POS].d20_ioinfo=
					    d20_err_table[D20_102IDX];
				    d20_handlemask(d20_qomask);
				    telex_err = TRUE;
				    break;
			   }
		     }
		     if (telex_err == TRUE) {
			      telex_err = FALSE;
			      continue;
		     } else
			      break;
	       }

	       if (fkt != FKTDEL)
		    d2_cp_to_telex(telex);
	       break;

	 default:
	       return(D2_ERROR);
    }

return(fkt);

}                               /*  d20_telex()                       */


/*inon ****************************************************************/
/*								      */
/* TYPE:        signed32                                              */
/*								      */
/* NAME  :      d20_fax()                                             */
/*								      */
/* AUTHOR:      V. Mueller  DI AP 11                                  */
/* DATE:        05.03.1991                                            */
/*                                                                    */
/* DESCRIPTION: signed32 d20_fax(op, attr_name, fax)                  */
/*              signed32       op;                                    */
/*              char          *attr_name                              */
/*              D2_faxnumber  *fax;                                   */
/*								      */
/*              This function analyses the fax-structure and fills    */
/*              the fields of the mask.                               */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              signed32       op;                                    */
/*              char          *attr_name                              */
/*              D2_faxnumber  *fax;                                   */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*              D2_faxnumber  *fax;                                   */
/*								      */
/* RETURNVALUE:         fkt      funktionkey                          */
/*								      */
/* STANDARD-HEADER:     None                                          */
/*								      */
/* GLOBAL  DATA  (read only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*                       d20_save_mask()                              */
/*                       d20_restore_mask()                           */
/*                       d2_cp_from_fax()                             */
/*                       d2_cp_to_fax()                               */
/*								      */
/* NOTES:                                                             */
/*								      */
/* SYNTAX       :                                                     */
/*								      */
/*inoff ***************************************************************/

 signed32 d20_fax(
   signed32      op,
   char         *attr_name,
   D2_faxnumber *fax)

{                               /*  d20_fax                           */

/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*		    D E	K L A R	A T I O	N E N			      */
/*								      */
/**********************************************************************/

signed32           fkt, i;
D20_omask          sav_faxomask[D20_M_SIZE];

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

    *d20_faxifields[DS_FX_NR] = D2_EOS;
    for (i=DS_FX_A3W; i <= DS_FX_UNLIML; i++)
	  *((signed32 *)d20_faxifields[i]) = FALSE;

    strcpy(d20_attr_name, attr_name);

    switch((int) op) {
	 case DS_DELETE_ATTRIBUTE:
	       fkt = FKTCR;
	       break;

	 case DS_DISPLAY_OBJECTS:
	 case DS_DISP_OBJ:
	       d20_save_mask(d20_faxomask, sav_faxomask);

	       d2_cp_from_fax(fax);

				/* display fax-mask */
	       fkt = d20_handlemask(d20_faxomask);

	       d20_restore_mask(d20_faxomask, sav_faxomask);
	       break;

	 case DS_MODIFY_ATTRIBUTE:
	       *((signed32 *)d20_faxifields[DS_FX_CHG]) = D2_REP_AV;
	       d2_cp_from_fax(fax);

				/* display fax-mask */
	       for (;;) {
		     fkt = d20_handlemask(d20_faxomask);
		     if ((d20_faxifields[DS_FX_NR][0] == '\0' ||
			d20_faxifields[DS_FX_NR][0] == DS_DEFAULT_VALUE) &&
			fkt != FKTDEL) {
				d20_qomask[D20_Q1POS].d20_ioinfo=
					d20_err_table[D20_103IDX];
				d20_handlemask(d20_qomask);
				continue;
		     } else
				break;
	       }

	       if (fkt != FKTDEL)
		    d2_cp_to_fax(fax);
	       break;

	 case DS_ADD_OBJECT:
	 case DS_ADD_ATTRIBUTE:
				/* display fax-mask */
	       for (;;) {
		     fkt = d20_handlemask(d20_faxomask);
		     if ((d20_faxifields[DS_FX_NR][0] == '\0' ||
			d20_faxifields[DS_FX_NR][0] == DS_DEFAULT_VALUE) &&
			fkt != FKTDEL) {
				d20_qomask[D20_Q1POS].d20_ioinfo=
					d20_err_table[D20_103IDX];
				d20_handlemask(d20_qomask);
				continue;
		     } else
				break;
	       }

	       if (fkt != FKTDEL)
		    d2_cp_to_fax(fax);
	       break;

	 default:
	       return(D2_ERROR);
    }

return(fkt);

}                               /*  d20_fax()                         */


/*inon ****************************************************************/
/*								      */
/* TYPE:        signed32                                              */
/*								      */
/* NAME  :      d20_t61()                                             */
/*								      */
/* AUTHOR:      V. Mueller  DI AP 11                                  */
/* DATE:        05.03.1991                                            */
/*                                                                    */
/* DESCRIPTION: signed32 d20_t61(op, attr_name, t61)                  */
/*              signed32     op;                                      */
/*              char        *attr_name                                */
/*              D2_T61_seq  *t61;                                     */
/*								      */
/*              This function analyses the t61-structure and fills    */
/*              the fields of the mask.                               */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              signed32     op;                                      */
/*              char        *attr_name                                */
/*              D2_T61_seq  *t61;                                     */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:         fkt      funktionkey                          */
/*								      */
/* STANDARD-HEADER:     None                                          */
/*								      */
/* GLOBAL  DATA  (read only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*                       d20_save_mask()                              */
/*                       d20_restore_mask()                           */
/*                       d2_cp_from_t61()                             */
/*                       d2_cp_to_t61()                               */
/*								      */
/* NOTES:                                                             */
/*								      */
/* SYNTAX       :                                                     */
/*								      */
/*inoff ***************************************************************/

 signed32 d20_t61(
   signed32    op,
   char       *attr_name,
   D2_T61_seq *t61)

{                               /*  d20_t61                           */

/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*		    D E	K L A R	A T I O	N E N			      */
/*								      */
/**********************************************************************/

signed32           fkt, i;
D20_omask          sav_t61omask[D20_M_SIZE];

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

    for (i=0; i < D20_MAX_NB_ITEM; i++)
	  *d20_t61ifields[i] = D2_EOS;

    strcpy(d20_attr_name, attr_name);

    switch ((int) op) {
	 case DS_DELETE_ATTRIBUTE:
	       fkt = FKTCR;
	       break;

	 case DS_DISPLAY_OBJECTS:
	 case DS_DISP_OBJ:
	       d20_save_mask(d20_t61omask, sav_t61omask);

	       d2_cp_from_t61(t61);

				/* display telex-mask */
	       fkt = d20_handlemask(d20_t61omask);

	       d20_restore_mask(d20_t61omask, sav_t61omask);
	       break;

	 case DS_MODIFY_ATTRIBUTE:
	       *((signed32 *)d20_t61ifields[DS_T61_CHG]) = D2_REP_AV;
	       d2_cp_from_t61(t61);

	       for (;;) {                        /* display t61-mask */
		     fkt = d20_handlemask(d20_t61omask);
		     if ((d20_t61ifields[0][0] == '\0' ||
			d20_t61ifields[0][0] == DS_DEFAULT_VALUE) &&
			fkt != FKTDEL) {
				d20_qomask[D20_Q1POS].d20_ioinfo=
					d20_err_table[D20_100IDX];
				d20_handlemask(d20_qomask);
				continue;
		     } else
				break;
	       }

	       if (fkt != FKTDEL)
		    d2_cp_to_t61(t61);
	       break;

	 case DS_ADD_OBJECT:
	 case DS_ADD_ATTRIBUTE:
	       for (;;) {                        /* display t61-mask */
		     fkt = d20_handlemask(d20_t61omask);
		     if ((d20_t61ifields[0][0] == '\0' ||
			d20_t61ifields[0][0] == DS_DEFAULT_VALUE) &&
			fkt != FKTDEL) {
				d20_qomask[D20_Q1POS].d20_ioinfo=
					d20_err_table[D20_100IDX];
				d20_handlemask(d20_qomask);
				continue;
		     } else
				break;
	       }

	       if (fkt != FKTDEL)
		    d2_cp_to_t61(t61);
	       break;

	 default:
	       return(D2_ERROR);
    }

return(fkt);

}                               /*  d20_t61()                         */

/*inon ****************************************************************/
/*								      */
/* TYPE:        signed32                                              */
/*								      */
/* NAME  :      d20_postal()                                          */
/*								      */
/* AUTHOR:      Sanjay jain DI AP 11                                  */
/* DATE:        05.03.1991                                            */
/*                                                                    */
/* DESCRIPTION: signed32 d20_postal(op, attr_name, str_list)          */
/*              signed32     op;                                      */
/*              char        *attr_name                                */
/*              D2_str_list  *str_list;                               */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              signed32     op;                                      */
/*              char        *attr_name                                */
/*              D2_str_list  *str_list;                               */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:         fkt      funktionkey                          */
/*								      */
/* STANDARD-HEADER:     None                                          */
/*								      */
/* GLOBAL  DATA  (read only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*                       d20_save_mask()                              */
/*                       d20_restore_mask()                           */
/*                       d2_cp_from_str_list()                        */
/*                       d2_cp_to_str_list()                          */
/*								      */
/* NOTES:                                                             */
/*								      */
/* SYNTAX       :                                                     */
/*								      */
/*inoff ***************************************************************/

 signed32 d20_postal(
   signed32     op,
   char        *attr_name,
   D2_str_list *str_list)

{                               /*  d20_postal                           */

/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*		    D E	K L A R	A T I O	N E N			      */
/*								      */
/**********************************************************************/

signed32           fkt, i;
D20_omask          sav_t61omask[D20_M_SIZE];

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

    for (i=0; i < D20_MAX_NB_ITEM; i++)
	  *d20_t61ifields[i] = D2_EOS;

    strcpy(d20_attr_name, attr_name);

    switch ((int) op) {
	 case DS_DELETE_ATTRIBUTE:
	       fkt = FKTCR;
	       break;

	 case DS_DISPLAY_OBJECTS:
	 case DS_DISP_OBJ:
	       d20_save_mask(d20_t61omask, sav_t61omask);

	       d2_cp_from_str_list(str_list);

				/* display telex-mask */
	       fkt = d20_handlemask(d20_t61omask);

	       d20_restore_mask(d20_t61omask, sav_t61omask);
	       break;

	 case DS_MODIFY_ATTRIBUTE:
	       *((signed32 *)d20_t61ifields[DS_T61_CHG]) = D2_REP_AV;
	       d2_cp_from_str_list(str_list);

	       for (;;) {                        /* display t61-mask */
		     fkt = d20_handlemask(d20_t61omask);
		     if ((d20_t61ifields[0][0] == '\0' ||
			d20_t61ifields[0][0] == DS_DEFAULT_VALUE) &&
			fkt != FKTDEL) {
				d20_qomask[D20_Q1POS].d20_ioinfo=
					d20_err_table[D20_100IDX];
				d20_handlemask(d20_qomask);
				continue;
		     } else
				break;
	       }

	       if (fkt != FKTDEL)
		    d2_cp_to_str_list(str_list);
	       break;

	 case DS_ADD_OBJECT:
	 case DS_ADD_ATTRIBUTE:
	       for (;;) {                        /* display t61-mask */
		     fkt = d20_handlemask(d20_t61omask);
		     if ((d20_t61ifields[0][0] == '\0' ||
			d20_t61ifields[0][0] == DS_DEFAULT_VALUE) &&
			fkt != FKTDEL) {
				d20_qomask[D20_Q1POS].d20_ioinfo=
					d20_err_table[D20_100IDX];
				d20_handlemask(d20_qomask);
				continue;
		     } else
				break;
	       }

	       if (fkt != FKTDEL)
		    d2_cp_to_str_list(str_list);
	       break;

	 default:
	       return(D2_ERROR);
    }

return(fkt);

}                               /*  d20_postal()                         */

/*inon ****************************************************************/
/*								      */
/* TYPE:        signed32                                              */
/*								      */
/* NAME  :      d20_dnscell()                                         */
/*								      */
/* AUTHOR:      V. Mueller  DI AP 11                                  */
/* DATE:        05.03.1991                                            */
/*                                                                    */
/* DESCRIPTION: signed32 d20_dnscell(op, attr_name, cell_str, c_len)  */
/*              signed32          op;                                 */
/*              char             *attr_name                           */
/*              Octet_string      cell_str;                           */
/*              signed16         *c_len;                              */
/*								      */
/*              This function analyses the cell-structure and fills   */
/*              the fields of the mask.                               */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              signed32          op;                                 */
/*              char             *attr_name                           */
/*              Octet_string      cell_str;                           */
/*              signed16         *c_len;                              */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*              Octet_string      cell_str;                           */
/*              signed16         *c_len;                              */
/*								      */
/* RETURNVALUE:         fkt      funktionkey                          */
/*								      */
/* STANDARD-HEADER:     None                                          */
/*								      */
/* GLOBAL  DATA  (read only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*                       d20_save_mask()                              */
/*                       d20_restore_mask()                           */
/*                       d2_cp_from_dnscell()                         */
/*                       d2_cp_to_dnscell()                           */
/*								      */
/* NOTES:                                                             */
/*								      */
/* SYNTAX       :                                                     */
/*								      */
/*inoff ***************************************************************/

 signed32 d20_dnscell(
   signed32      op,
   char         *attr_name,
   Octet_string  cell_str,
   signed16     *c_len)

{                               /*  d20_dnscell                       */

/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*		    D E	K L A R	A T I O	N E N			      */
/*								      */
/**********************************************************************/

D20_omask          sav_dnscellomask[D20_M_SIZE];

signed32           fkt, i;
char               nsp_uuid[CTSLEN+1];
char               root_uuid[CTSLEN+1];
char               root_name[DNS_STR_FNAME_MAX+1];
int           ret_code;
unsigned char     *cell_ucp;       /* pointer to the 'cell'-string */
signed16           cell_len;       /* length of the 'cell'-string  */

x500_dns_cell_t    dns_cell;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

    dns_cell.num_replicas = 1;
    cell_ucp = (unsigned char *) NULL;

    for(i=0; i <= DS_DCELL_RNM; i++)
	  *d20_dnscellifields[i] = D2_EOS;

    strcpy(d20_attr_name, attr_name);

    switch ((int) op) {
	 case DS_DELETE_ATTRIBUTE:
	       *c_len = 0;
	       fkt = FKTCR;
	       break;

	 case DS_DISPLAY_OBJECTS:
	 case DS_DISP_OBJ:
	       d20_save_mask(d20_dnscellomask, sav_dnscellomask);

				      /* provide dns_cell structure */
	       ret_code = x500_str_to_cell(cell_str, *c_len, &dns_cell);
	       if (ret_code == 1)
		    d2_cp_from_dnscell(&dns_cell);
	       else
		    {
		    d20_restore_mask(d20_dnscellomask, sav_dnscellomask);
		    return(D2_ERROR);
		    }

				/* display dnscell-mask */
	       fkt = d20_handlemask(d20_dnscellomask);

	       d20_restore_mask(d20_dnscellomask, sav_dnscellomask);
	       break;

	 case DS_MODIFY_ATTRIBUTE:
	       *((signed32 *)d20_dnscellifields[DS_DCELL_CHG]) = D2_REP_AV;

				       /* provide dns_cell structure */
	       ret_code = x500_str_to_cell(cell_str, *c_len, &dns_cell);
	       if (ret_code == 1)
		    d2_cp_from_dnscell(&dns_cell);
	       else
		    return(D2_ERROR);

	       for (;;) {              /* display dnscell-mask */
		     fkt = d20_handlemask(d20_dnscellomask);
		     if ((d20_dnscellifields[DS_DCELL_NSPCTS][0]==DS_DEFAULT_VALUE ||
			d20_dnscellifields[DS_DCELL_NSPCTS][0]=='\0') &&
			fkt != FKTDEL) {
				d20_qomask[D20_Q1POS].d20_ioinfo=
					d20_err_table[D20_E98IDX];
				d20_handlemask(d20_qomask);
			    	continue;
		     } else
				break;
	       }

	       if (fkt != FKTDEL) {
		    dns_cell.nsp_uuid = nsp_uuid;
		    dns_cell.root_uuid = root_uuid;
		    dns_cell.root_name = root_name;

		    d2_cp_to_dnscell(&dns_cell);

		    ret_code = x500_cell_to_str(&dns_cell, &cell_ucp, &cell_len);
	       } else
		     return(fkt);

	       if (ret_code == 1) {
		    memcpy(cell_str, cell_ucp, cell_len);
		    *c_len = cell_len;
	       }
	       else
		    return(D2_ERROR);
	       break;

	 case DS_ADD_OBJECT:
	 case DS_ADD_ATTRIBUTE:

	       for (;;) {              /* display dnscell-mask */
		     fkt = d20_handlemask(d20_dnscellomask);
		     if ((d20_dnscellifields[DS_DCELL_NSPCTS][0]==DS_DEFAULT_VALUE ||
			d20_dnscellifields[DS_DCELL_NSPCTS][0]=='\0') &&
			fkt != FKTDEL) {
				d20_qomask[D20_Q1POS].d20_ioinfo=
					d20_err_table[D20_E98IDX];
				d20_handlemask(d20_qomask);
			    	continue;
		     } else
				break;
	       }


	       if (fkt != FKTDEL) {
		    dns_cell.nsp_uuid = nsp_uuid;
		    dns_cell.root_uuid = root_uuid;
		    dns_cell.root_name = root_name;

		    d2_cp_to_dnscell(&dns_cell);
		    ret_code = x500_cell_to_str(&dns_cell, &cell_ucp, &cell_len);
	       } else
		     return(fkt);

	       if (ret_code == 1) {
		    memcpy(cell_str, cell_ucp, cell_len);
		    *c_len = cell_len;
	       } else
		    return(D2_ERROR);
	       break;

	 default:
	       return(D2_ERROR);
    }

return(fkt);

}                               /*  d20_dnscell()                       */

/*inon ****************************************************************/
/*								      */
/* TYPE:        signed32                                              */
/*								      */
/* NAME  :      d20_dnsrep()                                          */
/*								      */
/* AUTHOR:      V. Mueller  DI AP 11                                  */
/* DATE:        05.03.1991                                            */
/*                                                                    */
/* DESCRIPTION: signed32 d20_dnsrep(op, attr_name, rep_str, r_len)    */
/*              signed32          op;                                 */
/*              char             *attr_name                           */
/*              Octet_string      rep_str;                            */
/*              signed16         *r_len;                              */
/*								      */
/*              This function analyses the replica-structure and fills*/
/*              the fields of the mask.                               */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              signed32          op;                                 */
/*              char             *attr_name                           */
/*              Octet_string      rep_str;                            */
/*              signed16         *r_len;                              */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*              Octet_string      rep_str;                            */
/*              signed16         *r_len;                              */
/*								      */
/* RETURNVALUE:         fkt      funktionkey                          */
/*								      */
/* STANDARD-HEADER:     None                                          */
/*								      */
/* GLOBAL  DATA  (read only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*                       d20_save_mask()                              */
/*                       d20_restore_mask()                           */
/*                       d2_cp_from_dnsrep()                          */
/*                       d2_cp_to_dnsrep()                            */
/*								      */
/* NOTES:                                                             */
/*								      */
/* SYNTAX       :                                                     */
/*								      */
/*inoff ***************************************************************/

 signed32 d20_dnsrep(
   signed32      op,
   char         *attr_name,
   Octet_string  rep_str,
   signed16     *r_len)

{                               /*  d20_dnsrep                           */

/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*		    D E	K L A R	A T I O	N E N			      */
/*								      */
/**********************************************************************/

D20_omask          sav_dnsrepomask[D20_M_SIZE];

signed32           fkt, j;
char               ch_uuid[CTSLEN+1];
char               ch_name[DNS_STR_FNAME_MAX+1];
/*
char               tower[X500_TOWER_LEN_MAX];
*/
char               tower[DNS_STR_FNAME_MAX+1];
int           ret_code;
signed16           rep_lens_sp;     /* length of the 'replica'-string */
unsigned char     *replica_ucpp;    /* 'replica'-string               */

x500_replica_t     replica;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

    for(j=1; j <= DS_REP_TW5; j++)
	  *d20_dnsrepifields[j] = D2_EOS;
    *((signed32 *)d20_dnsrepifields[DS_REP_TYP]) = 1;

    strcpy(d20_attr_name, attr_name);

    switch ((int) op) {
	 case DS_DELETE_ATTRIBUTE:
	       *r_len = 0;
	       fkt = FKTCR;
	       break;

	 case DS_DISPLAY_OBJECTS:
	 case DS_DISP_OBJ:
	       d20_save_mask(d20_dnsrepomask, sav_dnsrepomask);

	       ret_code = x500_str_to_replica(rep_str, *r_len, &replica);
	       if (ret_code == 1)
		    d2_cp_from_dnsrep(&replica);
	       else
		    {
		    d20_restore_mask(d20_dnsrepomask, sav_dnsrepomask);
		    return(D2_ERROR);
		    }

				/* display dnsrep-mask */
	       fkt = d20_handlemask(d20_dnsrepomask);

	       d20_restore_mask(d20_dnsrepomask, sav_dnsrepomask);
	       break;

	 case DS_MODIFY_ATTRIBUTE:
	       *((signed32 *)d20_dnsrepifields[DS_REP_CHG]) = D2_REP_AV;

	       ret_code = x500_str_to_replica(rep_str, *r_len, &replica);
	       if (ret_code == 1)
		    d2_cp_from_dnsrep(&replica);
	       else
		    return(D2_ERROR);

	       for (;;) {              /* display dnsrep-mask  */
		     fkt = d20_handlemask(d20_dnsrepomask);
		     if ((d20_dnsrepifields[DS_REP_CHCTS][0] == DS_DEFAULT_VALUE || d20_dnsrepifields[DS_REP_CHCTS][0] == '\0') || 
		        ((d20_dnsrepifields[DS_REP_CHNMA][0] == DS_DEFAULT_VALUE || d20_dnsrepifields[DS_REP_CHNMA][0] == '\0') && 
		        (d20_dnsrepifields[DS_REP_CHNMB][0] == DS_DEFAULT_VALUE || d20_dnsrepifields[DS_REP_CHNMB][0] == '\0')) || 
		         (d20_dnsrepifields[DS_REP_TW1][0] == DS_DEFAULT_VALUE || d20_dnsrepifields[DS_REP_TW1][0] == '\0') && (fkt != FKTDEL)) {
				    d20_qomask[D20_Q1POS].d20_ioinfo=
					    d20_err_table[D20_E99IDX];
				    d20_handlemask(d20_qomask);
				    continue;
		     }
		     else
			    break;
	       }

	       if (fkt != FKTDEL) {
	       	   replica.ch_uuid = ch_uuid;
	           replica.ch_name = ch_name;
	           replica.tower = tower;

		   d2_cp_to_dnsrep(&replica);

		   ret_code = x500_replica_to_str(&replica,&replica_ucpp,
						  &rep_lens_sp);
	       } else
		   return(fkt);

	       if (ret_code == 1) {
		   memcpy(rep_str, replica_ucpp, rep_lens_sp);
		   *r_len = rep_lens_sp;
	       } else
		   return(D2_ERROR);
	       break;

	 case DS_ADD_OBJECT:
	 case DS_ADD_ATTRIBUTE:
	       for (;;) {              /* display dnsrep-mask  */
		     fkt = d20_handlemask(d20_dnsrepomask);
		     if ((d20_dnsrepifields[DS_REP_CHCTS][0] == DS_DEFAULT_VALUE || d20_dnsrepifields[DS_REP_CHCTS][0] == '\0') || 
		        ((d20_dnsrepifields[DS_REP_CHNMA][0] == DS_DEFAULT_VALUE || d20_dnsrepifields[DS_REP_CHNMA][0] == '\0') && 
		        (d20_dnsrepifields[DS_REP_CHNMB][0] == DS_DEFAULT_VALUE || d20_dnsrepifields[DS_REP_CHNMB][0] == '\0')) || 
		         (d20_dnsrepifields[DS_REP_TW1][0] == DS_DEFAULT_VALUE || d20_dnsrepifields[DS_REP_TW1][0] == '\0') && (fkt != FKTDEL)) {
				    d20_qomask[D20_Q1POS].d20_ioinfo=
					    d20_err_table[D20_E99IDX];
				    d20_handlemask(d20_qomask);
				    continue;
		     }
		     else
			    break;
	       }

	       if (fkt != FKTDEL) {
	           replica.ch_uuid = ch_uuid;
	           replica.ch_name = ch_name;
	           replica.tower = tower;

		   d2_cp_to_dnsrep(&replica);

		   ret_code = x500_replica_to_str(&replica,&replica_ucpp,
						  &rep_lens_sp);
	       } else
		   return(fkt);

	       if (ret_code == 1) {
		   memcpy(rep_str, replica_ucpp, rep_lens_sp);
		   *r_len = rep_lens_sp;
	       } else
		   return(D2_ERROR);
	       break;

	 default:
	       return(D2_ERROR);
    }

return(fkt);

}                               /*  d20_dnsrep()                        */


/**********************************************************************/
/*								      */
/* NAME             : d20_cert()                                      */
/* AUTHOR           : Jean-Marc Bonnaudet - SNI BU BA NM 123          */
/* DATE             : 23.07.1993                                      */
/* DESCRIPTION      : Display the SAP mask with certificate attribute */
/* INPUT-PARAMETERS : op Operator                                     */
/* OUTPUT-PARAMETERS:                                                 */
/* RETURNVALUE      : signed32 fkt funktionkey                        */
/* CALLING FUNCTIONS:                                                 */
/* CALLED FUNCTIONS :                                                 */
/* NOTES            :                                                 */
/*								      */
/**********************************************************************/
signed32 d20_cert(	signed32 object_counter,/* Object Counter	*/
			signed32 op,		/* Operator		*/
			char *attr_name,	/* Attribute name	*/
			Certificate *rep_str,
			signed16 *r_len )	/* Used for add only	*/
{
signed32	fkt;			/* Returned function key	*/
signed32	len_to_copy;		/* Length which should be copied*/
Bool		error_occured;		/* Boolean value		*/
Bool		mem_allocated;		/* is mem already allocated ? 	*/
D20_omask	sav_certomask[D20_M_SIZE]; /* Temp. copy of CGET fields	*/
Certificate 	*new_cert;		/* Certificate to add / to display */
Ob_id_string	sig_alg_id_string, key_alg_id_string;
signed32	alg_id_len;		/* Length of a algorithm to add	*/

    /* Common initialization */
    new_cert = rep_str;

    switch ((int) op)
	{
	case DS_DISPLAY_OBJECTS:
	case DS_MODIFY_ATTRIBUTE:

	    /* Initialization */
	    fkt = FKTCR;

	    /* enable SCROLL_UP */
	    d20_certomask[D20_CERTSCU].d20_fkt_id = D20_FKTX;
	    if (object_counter != 1)
		{
		/*  enable F1 and			*/
		/*  SCROLL_DOWN in Certificate mask	*/
		d20_certomask[D20_CERTSCD].d20_fkt_id =
		d20_certomask[D20_CERTF1].d20_fkt_id = D20_FKTX;
		}

	    /* Change CGET fields in CSAY fields	*/
	    d20_save_mask(d20_certomask, sav_certomask);

	    /* Put the values in the fields of the mask	*/

	    strcpy(d20_attr_name, attr_name);
	    *(signed32 *)(d20_certifields[0]) = 0; /* Not used for cert.*/
	    *(signed32 *)(d20_certifields[1]) = new_cert->to_be_signed.version;
						/* Version		*/
	    *(signed32 *)(d20_certifields[2]) = 
			    new_cert->to_be_signed.serial_number;
						/* Serial Number	*/
	    if ( X509ToAlgTypeAdm(		/* Signature		*/
		 &new_cert->to_be_signed.signature.algorithm.d2_type,
		 new_cert->to_be_signed.signature.algorithm.d2_typ_len,
		 (signed32 *)(d20_certifields[3])) == D2_ERROR )
	    {
		d20_qomask[D20_Q1POS].d20_ioinfo = d20_err_table[D20_E116IDX];
		d20_handlemask(d20_qomask);
		fkt = D2_ERROR;
		break;
	    }
	    if( new_cert->to_be_signed.signature.parameters != NULL )
		*(signed32 *)(d20_certifields[4]) =
		    *(new_cert->to_be_signed.signature.parameters);
	    else
		*(char *)(d20_certifields[4]) = D2_EOS;
						/* Parameter		*/

	    /* Convert the DN in issuer to a printable string		*/
	    d20_gen_dnintf((String)new_cert->to_be_signed.issuer);
	    /* Copy value of issuer 					*/
	    len_to_copy = MIN(
		(int)strlen((char *)new_cert->to_be_signed.issuer),
		(D20_CERT_LINE_LEN * D20_T61_CONV_FACT));
	    strncpy(d20_certifields[5],		/* Issuer line 1	*/
		    (char *)new_cert->to_be_signed.issuer,
		    len_to_copy);
	    d20_certifields[5][len_to_copy] = D2_EOS;
	    d20_certifields[6][0] = D2_EOS;
	    if ((int)strlen((char *)new_cert->to_be_signed.issuer) >
			(D20_CERT_LINE_LEN * D20_T61_CONV_FACT))
	    {
		len_to_copy = MIN(
			(int)(strlen((char *)new_cert->to_be_signed.issuer)
			- (D20_CERT_LINE_LEN * D20_T61_CONV_FACT)),
			(D20_CERT_LINE_LEN * D20_T61_CONV_FACT) );
		strncpy(d20_certifields[6],	/* Issuer line 2	*/
			(char *)new_cert->to_be_signed.issuer +
			(D20_CERT_LINE_LEN * D20_T61_CONV_FACT),
			len_to_copy);
		d20_certifields[6][len_to_copy] = D2_EOS;
	    }

	    if ( ( X208ToDate( new_cert->to_be_signed.validity.not_before,
			    (signed32 *)(d20_certifields[7]),
			    (signed32 *)(d20_certifields[8]),
			    (signed32 *)(d20_certifields[9]),
			    (signed32 *)(d20_certifields[10]),
			    (signed32 *)(d20_certifields[11]),
			    (signed32 *)(d20_certifields[12]) ) == D2_ERROR
		 )
	       ||
		 ( X208ToDate( new_cert->to_be_signed.validity.not_after,
			    (signed32 *)(d20_certifields[13]),
			    (signed32 *)(d20_certifields[14]),
			    (signed32 *)(d20_certifields[15]),
			    (signed32 *)(d20_certifields[16]),
			    (signed32 *)(d20_certifields[17]),
			    (signed32 *)(d20_certifields[18]) ) == D2_ERROR
		 )
	       )
	    {
		d20_qomask[D20_Q1POS].d20_ioinfo = d20_err_table[D20_E116IDX];
		d20_handlemask(d20_qomask);
		fkt = D2_ERROR;
		break;
	    }

	    /* Convert DN in subject in a printable string		*/
	    d20_gen_dnintf((String)new_cert->to_be_signed.subject);
	    /* Copy value of subject 					*/
	    len_to_copy = MIN(
			(int)strlen((char *)new_cert->to_be_signed.subject),
			(D20_CERT_LINE_LEN * D20_T61_CONV_FACT) );
	    strncpy(d20_certifields[19],	/* Subject line 1	*/
		    (char *)new_cert->to_be_signed.subject,
		    len_to_copy);
	    d20_certifields[19][len_to_copy] = D2_EOS;
	    d20_certifields[20][0] = D2_EOS;
	    if ((int)strlen((char *)new_cert->to_be_signed.subject) >
				(D20_CERT_LINE_LEN * D20_T61_CONV_FACT))
	    {
		len_to_copy = MIN(
			    (int)strlen((char *)new_cert->to_be_signed.subject)
			    - (D20_CERT_LINE_LEN * D20_T61_CONV_FACT),
			    (D20_CERT_LINE_LEN * D20_T61_CONV_FACT) );
		strncpy(d20_certifields[20],	/* Subject line 2	*/
			(char *)new_cert->to_be_signed.subject +
			(D20_CERT_LINE_LEN * D20_T61_CONV_FACT),
			len_to_copy);
		d20_certifields[20][len_to_copy] = D2_EOS;
	    }

	    /* Algorithm	*/
	    if( X509ToAlgTypeAdm(		/* Algorithm		*/
		&new_cert->to_be_signed.subj_public_key_info.
			algorithm.algorithm.d2_type,
		new_cert->to_be_signed.subj_public_key_info.
			algorithm.algorithm.d2_typ_len,
		(signed32 *)(d20_certifields[21])) == D2_ERROR )
	    {
		d20_qomask[D20_Q1POS].d20_ioinfo = d20_err_table[D20_E124IDX];
		d20_handlemask(d20_qomask);
		fkt = D2_ERROR;
	    }

	    if( new_cert->to_be_signed.subj_public_key_info.
		algorithm.parameters != NULL )
		*(signed32 *)(d20_certifields[22]) =
		    *(new_cert->to_be_signed.subj_public_key_info.
			algorithm.parameters);
	    else
		*(char *)(d20_certifields[22]) = D2_EOS;
						/* Parameter		*/
	    *(signed32 *)(d20_certifields[23]) = 0; /* Not used: Modif.	*/

	    if( fkt != D2_ERROR )
	    {
		/* Display the mask now			*/
		fkt = d20_handlemask(d20_certomask);
	    }

	    /* Restore the CGET fields		*/
	    d20_restore_mask(d20_certomask, sav_certomask);

	    /* disable SCROLL_UP and F1 and	*/
	    /* SCROLL_DOWN in Certificate mask	*/
	    d20_certomask[D20_CERTSCD].d20_fkt_id =
	    d20_certomask[D20_CERTSCU].d20_fkt_id =
	    d20_certomask[D20_CERTF1].d20_fkt_id = D20_NO_OP;

	    break;

	case DS_ADD_OBJECT:

	    /* Initializations of the fields	*/
	    strcpy(d20_attr_name, attr_name);
	    *(signed32 *)(d20_certifields[0]) = 0; /* Not used for cert.*/
	    *(signed32 *)(d20_certifields[1]) = 0; /* Version		*/
	    *(signed32 *)(d20_certifields[2]) = 0; /* Serial Number	*/
	    *(signed32 *)(d20_certifields[3]) = 0; /* Signature		*/
	    *(signed32 *)(d20_certifields[4]) = 0; /* Parameter		*/
	    d20_certifields[5][0] = D2_EOS;	/* Issuer line 1	*/
	    d20_certifields[6][0] = D2_EOS;	/* Issuer line 2	*/
	    *(signed32 *)(d20_certifields[7]) = 0; /* Not before year	*/
	    *(signed32 *)(d20_certifields[8]) = 0; /* Not before month	*/
	    *(signed32 *)(d20_certifields[9]) = 0; /* Not before day	*/
	    *(signed32 *)(d20_certifields[10]) = 0; /* Not before hours	*/
	    *(signed32 *)(d20_certifields[11]) = 0; /* Not before min.	*/
	    *(signed32 *)(d20_certifields[12]) = 0; /* Not before sec.	*/
	    *(signed32 *)(d20_certifields[13]) = 0; /* Not after year	*/
	    *(signed32 *)(d20_certifields[14]) = 0; /* Not after month	*/
	    *(signed32 *)(d20_certifields[15]) = 0; /* Not after day	*/
	    *(signed32 *)(d20_certifields[16]) = 0; /* Not after hours	*/
	    *(signed32 *)(d20_certifields[17]) = 0; /* Not after min.	*/
	    *(signed32 *)(d20_certifields[18]) = 0; /* Not after sec.	*/
	    d20_certifields[19][0] = D2_EOS;	/* Subject line 1	*/
	    d20_certifields[20][0] = D2_EOS;	/* Subject line 2	*/
	    *(signed32 *)(d20_certifields[21]) = 0; /* Algorithm	*/
	    *(signed32 *)(d20_certifields[22]) = 0; /* Parameter	*/
	    *(signed32 *)(d20_certifields[23]) = 0; /* Not used: Modif.	*/

	    mem_allocated = FALSE;

	    for (;;)
		{
		/* Initializations */
		error_occured = FALSE;

		fkt = d20_handlemask(d20_certomask);

		if( fkt != FKTDEL )		/* Save values		*/
		    {
		    if( !mem_allocated )
		    {
			/* Initialize pointers / Allocate memory	*/
			if( (
			      (sig_alg_id_string =
			       (Ob_id_string)malloc(sizeof(Ob_id_string)
						* CERT_ALG_ID_MAX_LEN))
			       == (Ob_id_string)NULL
			    )
			  ||
			    (
			      (key_alg_id_string =
			       (Ob_id_string)malloc(sizeof(Ob_id_string)
						* CERT_ALG_ID_MAX_LEN))
			       == (Ob_id_string)NULL
			    )
			  ||
			    (
			      (new_cert->to_be_signed.
					      signature.algorithm.d2_type =
			       (Ob_id_string)malloc(sizeof(Ob_id_string) * 20))
			       == (Ob_id_string)NULL
			    )
			  ||
			    (
			      (new_cert->to_be_signed.signature.parameters =
			       (signed32 *)malloc(sizeof(signed32)))
			       == (signed32 *)NULL
			    )
			  ||
			    (
			      (new_cert->to_be_signed.validity.not_before =
			       (UTC_time_string)malloc(
					       sizeof(UTC_time_string)*20))
			       == (UTC_time_string)NULL
			    )
			  ||
			    (
			      (new_cert->to_be_signed.validity.not_after =
			       (UTC_time_string)malloc(
					       sizeof(UTC_time_string)*20))
			       == (UTC_time_string)NULL
			    )
			  ||
			    (
			      (new_cert->to_be_signed.issuer =
			       (D2_name_string)malloc(D2_DNL_MAX))
			       == (D2_name_string)NULL
			    )
			  ||
			    (
			      (new_cert->to_be_signed.subject =
			       (D2_name_string)malloc(D2_DNL_MAX))
			       == (D2_name_string)NULL
			    )
			  ||
			    (
			      (new_cert->to_be_signed.subj_public_key_info.
				   algorithm.algorithm.d2_type =
			       (Ob_id_string)malloc(sizeof(Ob_id_string) * 20))
			       == (Ob_id_string)NULL
			    )
			  ||
			    (
			      (new_cert->to_be_signed.subj_public_key_info.
				   subject_key.d2_value =
			       (unsigned char *)malloc(
					       sizeof(unsigned char *)*20))
			       == (unsigned char *)NULL
			    )
			  ||
			    (
			      (new_cert->to_be_signed.subj_public_key_info.
				    algorithm.parameters =
			       (signed32 *)malloc(sizeof(signed32)))
			       == (signed32 *)NULL
			    )
			  ||
			    (
			      (new_cert->alg.algorithm.d2_type =
			       (Ob_id_string)malloc(sizeof(Ob_id_string) * 20))
			       == (Ob_id_string)NULL
			    )
			  ||
			    (
			      (new_cert->encr_str.d2_value =
			       (unsigned char *)malloc(
					       sizeof(unsigned char *)*20))
			       == (unsigned char *)NULL
			    )
			  ||
			    (
			      (new_cert->alg.parameters =
			       (signed32 *)malloc(sizeof(signed32)))
			       == (signed32 *)NULL
			    )
			  )
			{
			    /* Error: not enough memory     */
			    d20_qomask[D20_Q1POS].d20_ioinfo =
						d20_err_table[D20_E04IDX];
			    d20_handlemask(d20_qomask);
			    error_occured = TRUE;
			    break;
			}
		    }

		    /* Save values	*/
		    new_cert->to_be_signed.version =
			    *(signed32 *)(d20_certifields[1]);
		    new_cert->to_be_signed.serial_number =
			    *(signed32 *)(d20_certifields[2]);
		    if( algTypeAdmValueToX509Value(
			    *(signed32 *)(d20_certifields[3]),
			    &sig_alg_id_string,
			    &alg_id_len) == D2_ERROR )
		    {
			    error_occured = TRUE;
			    break;
		    }
		    memcpy(new_cert->to_be_signed.signature.algorithm.d2_type,
			    sig_alg_id_string, alg_id_len);
		    new_cert->to_be_signed.signature.algorithm.d2_typ_len =
			    alg_id_len;
		    *(new_cert->to_be_signed.signature.parameters) =
			    *(signed32 *)(d20_certifields[4]);
		    strcpy((char *)new_cert->to_be_signed.issuer,
			    d20_certifields[5]);
		    strcat((char *)new_cert->to_be_signed.issuer,
			    d20_certifields[6]);
		    if(ds0_508_check_name((char *)new_cert->to_be_signed.issuer)
			    == D2_NMSYNTAX_ERROR)
			{
			error_occured = TRUE;
			d20_qomask[D20_Q1POS].d20_ioinfo =
					    d20_errtab[D20_E106IDX];
			d20_handlemask(d20_qomask);
			fkt = D2_ERROR;
			}
		    if (
			(DateToX208(
			 *(signed32 *)(d20_certifields[7]),
			 *(signed32 *)(d20_certifields[8]),
			 *(signed32 *)(d20_certifields[9]),
			 *(signed32 *)(d20_certifields[10]),
			 *(signed32 *)(d20_certifields[11]),
			 *(signed32 *)(d20_certifields[12]),
			 &new_cert->to_be_signed.validity.not_before)
			 == D2_ERROR
			)
			||
			(DateToX208(
			 *(signed32 *)(d20_certifields[13]),
			 *(signed32 *)(d20_certifields[14]),
			 *(signed32 *)(d20_certifields[15]),
			 *(signed32 *)(d20_certifields[16]),
			 *(signed32 *)(d20_certifields[17]),
			 *(signed32 *)(d20_certifields[18]),
			 &new_cert->to_be_signed.validity.not_after)
			 == D2_ERROR
			 )
			)
		    {
			    error_occured = TRUE;
			    d20_qomask[D20_Q1POS].d20_ioinfo =
						d20_err_table[D20_E122IDX];
			    d20_handlemask(d20_qomask);
			    fkt = D2_ERROR;
		    }
		    strcpy((char *)new_cert->to_be_signed.subject,
			    d20_certifields[19]);
		    strcat((char *)new_cert->to_be_signed.subject,
			    d20_certifields[20]);
		    if(ds0_508_check_name(
			    (char *)new_cert->to_be_signed.subject)
			    == D2_NMSYNTAX_ERROR)
		    {
			error_occured = TRUE;
			d20_qomask[D20_Q1POS].d20_ioinfo =
					    d20_errtab[D20_E106IDX];
			d20_handlemask(d20_qomask);
			fkt = D2_ERROR;
		    }
		    if( algTypeAdmValueToX509Value(
			    *(signed32 *)(d20_certifields[21]),
			    &key_alg_id_string,
			    &alg_id_len) == D2_ERROR )
		    {
			    error_occured = TRUE;
		    }
		    memcpy(new_cert->to_be_signed.subj_public_key_info.
			algorithm.algorithm.d2_type,
			key_alg_id_string,
			alg_id_len);
		    new_cert->to_be_signed.subj_public_key_info.
			algorithm.algorithm.d2_typ_len =
			    alg_id_len;
		    *(new_cert->to_be_signed.subj_public_key_info.
			algorithm.parameters) =
			*(signed32 *)(d20_certifields[22]);
		    new_cert->to_be_signed.subj_public_key_info.
			subject_key.d2_size = 0;
		    (new_cert->to_be_signed.subj_public_key_info.
			subject_key.d2_value)[0] = D2_EOS;

		    memcpy(new_cert->alg.algorithm.d2_type, CERT_RSA,
				sizeof(CERT_RSA) - 1);
		    new_cert->alg.algorithm.d2_typ_len = sizeof(CERT_RSA) - 1;
		    *(new_cert->alg.parameters) = 0;

		    (new_cert->encr_str.d2_value)[0] = D2_EOS;
		    new_cert->encr_str.d2_size = 0;

		    *r_len = sizeof(*new_cert);

		    }

		if( !error_occured ) break;
		}

	    break;

	case DS_DELETE_ATTRIBUTE:
		*r_len = 0;
		fkt = FKTCR;
	    break;

	default:	/* Programming error! Operation not allowed!	*/
	    fkt = D2_ERROR;
        }

return( fkt );
}



/**********************************************************************/
/*								      */
/* NAME             : algTypeAdmValueToX509Value                      */
/* AUTHOR           : Jean-Marc Bonnaudet - SNI BU BA KP 13           */
/* DATE             : 29.11.1993                                      */
/* DESCRIPTION      : Convert the value of the mask field to the real */
/*                    value according to the X.509 recommendation     */
/* INPUT-PARAMETERS : value in the administration mask field          */
/* OUTPUT-PARAMETERS: X.509 value - memory should be allocated before */
/* RETURNVALUE      : D2_ERROR for unvalid input parameters,          */
/*                    D2_NOERROR else.                                */
/* CALLING FUNCTIONS:                                                 */
/* CALLED FUNCTIONS :                                                 */
/* NOTES            :  defines for the X.509 values in d2dir.h        */
/*								      */
/**********************************************************************/

static signed32 algTypeAdmValueToX509Value( signed32 alg_cert_field,/* IN  */
				Ob_id_string *alg_cert_string,      /* OUT */
				signed32 *alt_cert_len )            /* OUT */
{
d2_ret_val return_value;

    switch((int) alg_cert_field)
    {
    case 0: memcpy(*alg_cert_string, CERT_RSA, sizeof(CERT_RSA) - 1);
	    *alt_cert_len = sizeof(CERT_RSA) - 1;
	    return_value=D2_NOERROR;
	    break;
    case 1: memcpy(*alg_cert_string, CERT_SQMOD_N, sizeof(CERT_SQMOD_N) - 1);
	    *alt_cert_len = sizeof(CERT_SQMOD_N) - 1;
	    return_value=D2_NOERROR;
	    break;
    case 2: memcpy(*alg_cert_string, CERT_SQMOD_NWITHRSA,
		    sizeof(CERT_SQMOD_NWITHRSA) -1 );
	    *alt_cert_len = sizeof(CERT_SQMOD_NWITHRSA) - 1;
	    return_value=D2_NOERROR;
	    break;
    default: /* Value not valid	*/
	    return_value=D2_ERROR;
    }

return( return_value );
}



/**********************************************************************/
/*								      */
/* NAME             : X509ToAlgTypeAdm                                */
/* AUTHOR           : Jean-Marc Bonnaudet - SNI BU BA NM 123          */
/* DATE             : 06.12.1993                                      */
/* DESCRIPTION      : Convert a X.509 Value to the value for the      */
/*                    mask in Administration                          */
/* INPUT-PARAMETERS : X.509 value - memory should be allocated before */
/* OUTPUT-PARAMETERS: value in the administration mask field          */
/* RETURNVALUE      : D2_ERROR for unvalid input parameters,          */
/*                    D2_NOERROR else.                                */
/* CALLING FUNCTIONS:                                                 */
/* CALLED FUNCTIONS :                                                 */
/* NOTES            :  defines for the X.509 values in d2dir.h        */
/*								      */
/**********************************************************************/
static signed32 X509ToAlgTypeAdm(Ob_id_string *alg_cert_string, /* IN  */
				signed32 alt_cert_len,          /* IN  */
				signed32 *alg_cert_field )      /* OUT */
{
d2_ret_val return_value;

    if(!memcmp(*alg_cert_string, CERT_RSA, alt_cert_len))
    {
	*alg_cert_field = 0;
	return_value = D2_NOERROR;
    }
    else if(!memcmp(*alg_cert_string, CERT_SQMOD_N, alt_cert_len))
    {
	*alg_cert_field = 1;
	return_value = D2_NOERROR;
    }
    else if(!memcmp(*alg_cert_string, CERT_SQMOD_NWITHRSA, alt_cert_len))
    {
	*alg_cert_field = 2;
	return_value = D2_NOERROR;
    }
    else
    {
	*alg_cert_field = 0;
	return_value = D2_ERROR;
    }


return( return_value );
}



/**********************************************************************/
/*								      */
/* NAME             : DateToX208()                                    */
/* AUTHOR           : Jean-Marc Bonnaudet - SNI BU BA NM 123          */
/* DATE             : 03.12.1993                                      */
/* DESCRIPTION      : Convert a time in UTC_time_string format        */
/* INPUT-PARAMETERS : time through integers                           */
/* OUTPUT-PARAMETERS: converted time - memory should be allocated     */
/* RETURNVALUE      : D2_NOERROR/D2_ERROR                             */
/* CALLING FUNCTIONS:                                                 */
/* CALLED FUNCTIONS :                                                 */
/* NOTES            : format of a UTC_time_string:                    */
/*                                                                    */
/*    YYMMTTSSMMZ		(year, month, date,                   */
/*				seconds, milliseconds, 'Z')           */
/*								      */
/* or YYMMTTSSMMSS[+/-]SSMM	(year, month, date,                   */
/*				seconds, milliseconds,                */
/*				timezone)                             */
/*                                                                    */
/**********************************************************************/
static signed32 DateToX208( signed32 year, signed32 month, signed32 day,
			signed32 hours, signed32 minutes, signed32 sec,
			UTC_time_string *converted_time)
{
d2_ret_val return_value;

    /* Test values */
    if (year>=0 && year<=99 && month>=1 && month<=12 && day>=1 && day<=31
	&& hours>=0 && hours<=23 && minutes>=0 && minutes<=59 && sec>=0
	&& sec<=59)
	{
	sprintf(*converted_time, "%.2d%.2d%.2d%.2d%.2d%.2d+0000Z",
		year, month, day, hours, minutes, sec);
	return_value=D2_NOERROR;
	}
    else
	return_value=D2_ERROR;

return(return_value);
}



/**********************************************************************/
/*								      */
/* NAME             : X208ToDate()                                    */
/* AUTHOR           : Jean-Marc Bonnaudet - SNI BU BA NM 123          */
/* DATE             : 07.12.1993                                      */
/* DESCRIPTION      : Convert a time in UTC_time_string format        */
/*                    in the values for the mask                      */
/* INPUT-PARAMETERS : time in UTC_time_string format                  */
/* OUTPUT-PARAMETERS: converted time as strings (char *) with         */
/*                    allocated memory                                */
/* RETURNVALUE      : D2_NOERROR/D2_ERROR                             */
/* CALLING FUNCTIONS:                                                 */
/* CALLED FUNCTIONS :                                                 */
/* NOTES            : format of a UTC_time_string:                    */
/*                                                                    */
/*    YYMMDDHHMMZ		(year, month, date,                   */
/*				seconds, milliseconds, 'Z')           */
/*								      */
/* or YYMMDDHHMMSS[+/-]SSMM	(year, month, date,                   */
/*				seconds, milliseconds,                */
/*				timezone)                             */
/*                                                                    */
/**********************************************************************/
static signed32 X208ToDate( UTC_time_string X208time,
		signed32 *year, signed32 *month,  signed32 *date,
		signed32 *hour, signed32 *minute, signed32 *second )
{
const signed16 len = 2;			/* Common length of each field	*/
char value[3];				/* Should be len + 1		*/
d2_ret_val return_value;		/* Return value			*/

    if( (int)strlen(X208time)<11 )
	return_value = D2_ERROR;
    else
    {
	strncpy( value, X208time,      len );
	*year = atoi( value );
	strncpy( value, X208time + 2,  len );
	*month = atoi( value );
	strncpy( value, X208time + 4,  len );
	*date = atoi( value );
	strncpy( value, X208time + 6,  len );
	*hour = atoi( value );
	strncpy( value, X208time + 8,  len );
	*minute = atoi( value );
	if( X208time[9]=='Z' )
	    *second = 0;
	else
	{
	    strncpy( value, X208time + 10, len );
	    *second = atoi( value );
	}

	return_value = D2_NOERROR;
    }

return( return_value );
}



/**********************************************************************/
/*								      */
/* NAME             : d20_certp()                                     */
/* AUTHOR           : Jean-Marc Bonnaudet - SNI BU BA KP 13           */
/* DATE             : 23.07.1993                                      */
/* DESCRIPTION      : Display the SAP mask with certificate pair      */
/*                    attribute.                                      */
/* INPUT-PARAMETERS : op Operator                                     */
/* OUTPUT-PARAMETERS:                                                 */
/* RETURNVALUE      : signed32 fkt funktionkey                        */
/* CALLING FUNCTIONS:                                                 */
/* CALLED FUNCTIONS :                                                 */
/* NOTES            :                                                 */
/*								      */
/**********************************************************************/
signed32 d20_certp(	signed32 object_counter,/* Object Counter	*/
			signed32 op,		/* Operator		*/
			char *attr_name,	/* Attribute name	*/
			Cert_pair *rep_str,
			signed16 *r_len )
{
signed32		fkt;			/* Function key		*/
Cert_pair		*new_certp;		/* Certificate Pair	*/
D20_omask		sav_certpomask[D20_M_SIZE]; /* copy of CGET fields */
signed16		new_forward_cert_len,	/* Length of reverse cert.*/
			new_reverse_cert_len;	/* Length of reverse cert.*/
/* Here variables use for add operation only:				*/
Certificate		*new_forward_cert,	/* Forward Certificate	*/
			*new_reverse_cert;	/* Reverse Certificate	*/
Bool			error_occured;		/* Boolean value	*/


    /* Common initialization */
    new_certp = rep_str;

    switch ((int) op)
	{
	case DS_DISPLAY_OBJECTS:
	case DS_MODIFY_ATTRIBUTE:

	    /* enable SCROLL_UP */
	    d20_certpomask[D20_CERTPSCU].d20_fkt_id = D20_FKTX;
	    if (object_counter != 1)
		{
		/*  enable F1 and			*/
		/*  SCROLL_DOWN in Certificate mask	*/
		d20_certpomask[D20_CERTPSCD].d20_fkt_id =
		d20_certpomask[D20_CERTPF1].d20_fkt_id = D20_FKTX;
		}

	    /* Change CGET fields in CSAY fields	*/
	    d20_save_mask(d20_certpomask, sav_certpomask);

	    /* Put the values in the fields of the mask	*/

	    /* Initializations of the fields	*/
	    strcpy(d20_attr_name, attr_name);

	    /* Forward Certificate	*/
	    if (new_certp->forward == (Certificate *)NULL)
		*(signed32 *)(d20_certpifields[0]) = 0;	/* NO		*/
	    else
		*(signed32 *)(d20_certpifields[0]) = 1;	/* YES		*/

	    /* Reverse Certificate	*/
	    if (new_certp->reverse == (Certificate *)NULL)
		*(signed32 *)(d20_certpifields[1]) = 0;	/* NO		*/
	    else
		*(signed32 *)(d20_certpifields[1]) = 1;	/* YES		*/

	    fkt = d20_handlemask(d20_certpomask);

	    /* Restore the CGET fields					*/
	    d20_restore_mask(d20_certpomask, sav_certpomask);

	    /* disable SCROLL_UP and F1 and SCROLL_DOWN	*/
	    /* in Certificate Pair mask	*/
	    d20_certpomask[D20_CERTPSCD].d20_fkt_id =
	    d20_certpomask[D20_CERTPSCU].d20_fkt_id =
	    d20_certpomask[D20_CERTPF1].d20_fkt_id = D20_NO_OP;

	    /* Display Certicate mask with Forward Certificate		*/
	    /* if necessary						*/

	    if ( ((fkt == FKTMENU) || (fkt == FKTCR)) &&
		new_certp->forward != (Certificate *)NULL)
	    {
		fkt = d20_cert(object_counter,	/* Object Counter	*/
				op,		/* Operator ==		*/
						/* DS_DISPLAY_OBJECTS	*/
						/* of course		*/
				attr_name,	/* Attribute name	*/
				new_certp->forward,
				&new_forward_cert_len );/* Not initia-	*/
						/* lized but also not	*/
						/* used !		*/
	    }

	    /* Display Certicate mask with Reverse Certificate		*/
	    /* if necessary						*/

	    if ( ((fkt == FKTMENU) || (fkt == FKTCR)) &&
		new_certp->reverse != (Certificate *)NULL)
	    {
		fkt = d20_cert(object_counter,	/* Object Counter	*/
				op,		/* Operator ==		*/
						/* DS_DISPLAY_OBJECTS	*/
						/* of course		*/
				attr_name,	/* Attribute name	*/
				new_certp->reverse,
				&new_reverse_cert_len );/* Not initia-	*/
						/* lized but also not	*/
						/* used !		*/
	    }

	    break;

	case DS_ADD_OBJECT:
	case DS_DELETE_ATTRIBUTE:

	    /* Initializations of the fields	*/
	    strcpy(d20_attr_name, attr_name);
	    *(signed32 *)(d20_certpifields[0]) = 0; /* Forward Cert.	*/
	    *(signed32 *)(d20_certpifields[1]) = 0; /* Reverse Cert.	*/
	    *(signed32 *)(d20_certpifields[2]) = 3; /* Modification:	*/
						/* Delete Value		*/
						/* No other value is	*/
						/* allowed		*/

	    for (;;)
		{
		/* Initializations */
		error_occured = FALSE;

		fkt = d20_handlemask(d20_certpomask);

		if( fkt != FKTDEL )		/* Save values		*/
		    {
		    if ( *(signed32 *)(d20_certpifields[0]) == 0     /* NO */
			&& *(signed32 *)(d20_certpifields[1]) == 0 ) /* NO */
			{				/* not allowed */
			error_occured = TRUE;
			d20_qomask[D20_Q1POS].d20_ioinfo =
					    d20_err_table[D20_E123IDX];
			d20_handlemask(d20_qomask);
			}
		    else
			{

			if ( *(signed32 *)(d20_certpifields[0]) == 0 ) /* NO */
			    new_certp->forward = (Certificate *)NULL;
			else					/* YES	 */
			    {
			    /* Allocate some memory for the Forw. Cert.	*/
			    if(
				(new_forward_cert =
				(Certificate *)malloc(sizeof(Certificate)))
				== (Certificate *)NULL
			      )
				{
				/* Error: not enough memory     */
				error_occured = TRUE;
				d20_qomask[D20_Q1POS].d20_ioinfo =
						    d20_err_table[D20_E04IDX];
				d20_handlemask(d20_qomask);
				break;
				}

			    fkt = d20_cert(object_counter,
					    op,
					    attr_name,
					    new_forward_cert,
					    &new_forward_cert_len );

			    if( fkt != FKTDEL )	/* Save values		*/
				new_certp->forward = new_forward_cert;
			    else break;
			    }

			if ( *(signed32 *)(d20_certpifields[1]) == 0 ) /* NO */
			    new_certp->reverse = (Certificate *)NULL;
			else					 /* YES	*/
			    {
			    /* Allocate some memory for the Forw. Cert.	*/
			    if(
				(new_reverse_cert =
				(Certificate *)malloc(sizeof(Certificate)))
				== (Certificate *)NULL
			      )
				{
				/* Error: not enough memory     */
				error_occured = TRUE;
				d20_qomask[D20_Q1POS].d20_ioinfo =
						    d20_err_table[D20_E04IDX];
				d20_handlemask(d20_qomask);
				break;
				}

			    fkt = d20_cert(object_counter,
					    op,
					    attr_name,
					    new_reverse_cert,
					    &new_reverse_cert_len );
			    if( fkt != FKTDEL )	/* Save values		*/
				new_certp->reverse = new_reverse_cert;
			    else break;
			    }
			}

		    *r_len = sizeof(*new_certp);
		    }

		if( !error_occured ) break;
		}
	    break;

	default:	/* Programming error! Operation not allowed!	*/
	    fkt = D2_ERROR;
        }

return( fkt );
}



/**********************************************************************/
/*								      */
/* NAME             : d20_certl()                                     */
/* AUTHOR           : Jean-Marc Bonnaudet - SNI BU BA KP 13           */
/* DATE             : 23.07.1993                                      */
/* DESCRIPTION      : Display the SAP mask with certificate list      */
/*                    attribute.                                      */
/* INPUT-PARAMETERS : op Operator                                     */
/* OUTPUT-PARAMETERS:                                                 */
/* RETURNVALUE      : signed32 fkt funktionkey                        */
/* CALLING FUNCTIONS:                                                 */
/* CALLED FUNCTIONS :                                                 */
/* NOTES            :                                                 */
/*								      */
/**********************************************************************/
signed32 d20_certl(	signed32 object_counter,/* Object Counter	*/
			signed32 op,		/* Operator		*/
			char *attr_name,	/* Attribute name	*/
			Cert_list *rep_str,
			signed16 *r_len )
{
signed32 fkt;
Cert_list	*new_certl;
Bool		error_occured;	/* Boolean value			*/
Bool		mem_allocated;	/* is mem already allocated		*/
signed32	len_to_copy;	/* Length which should be copied	*/
signed32        alg_id_len;	/* Length of an algorithm to add	*/
signed16	rcert_len;
D20_omask	sav_certlomask[D20_M_SIZE]; /* copy of CGET fields	*/

    /* Common initialization */
    new_certl = rep_str;

    switch ((int) op)
    {
	case DS_DISPLAY_OBJECTS:
	case DS_MODIFY_ATTRIBUTE:

	    /* enable SCROLL_UP */
	    d20_certlomask[D20_CERTLSCU].d20_fkt_id = D20_FKTX;
	    if (object_counter != 1)
		{
		/*  enable F1 and SCROLL_DOWN	*/
		/*  in Certificate List mask	*/
		d20_certlomask[D20_CERTLSCD].d20_fkt_id =
		d20_certlomask[D20_CERTLF1].d20_fkt_id = D20_FKTX;
		}

	    /* Change CGET fields in CSAY fields	*/
	    d20_save_mask(d20_certlomask, sav_certlomask);

	    /* Put the values in the fields of the mask	*/

	    /* Initializations of the fields	*/
	    strcpy(d20_attr_name, attr_name);

	    if ( X509ToAlgTypeAdm(		/* Signature		*/
		&new_certl->to_be_signed.signature.algorithm.d2_type,
		new_certl->to_be_signed.signature.algorithm.d2_typ_len,
		(signed32 *)(d20_certlifields[0])) == D2_ERROR )
	    {
		fkt = D2_ERROR;
		break;
	    }
	    if( new_certl->to_be_signed.signature.parameters != NULL )
		*(signed32 *)(d20_certlifields[1]) =  /* Parameter	*/
		    *(new_certl->to_be_signed.signature.parameters);
	    else
		*(char *)(d20_certlifields[1]) = D2_EOS;
	    /* Convert the DN in issuer to a printable string		*/
	    d20_gen_dnintf((String)new_certl->to_be_signed.issuer);
	    len_to_copy = MIN(
	    (int)strlen((char *)new_certl->to_be_signed.issuer),
				(D20_CERT_LINE_LEN * D20_T61_CONV_FACT) );
	    strncpy(d20_certlifields[2],	/* Issuer line 1	*/
		(char *)new_certl->to_be_signed.issuer,
		len_to_copy);
	    d20_certlifields[2][len_to_copy] = D2_EOS;
	    d20_certlifields[3][0] = D2_EOS;
	    if (strlen((char *)new_certl->to_be_signed.issuer) >
		(size_t)(D20_CERT_LINE_LEN * D20_T61_CONV_FACT))
	    {
		len_to_copy = MIN(
		    (int)strlen((char *)new_certl->to_be_signed.issuer) -
		    (D20_CERT_LINE_LEN * D20_T61_CONV_FACT),
		    (D20_CERT_LINE_LEN * D20_T61_CONV_FACT) );
		strncpy( d20_certlifields[3],	/* Issuer line 2	*/
		    (char *)new_certl->to_be_signed.issuer +
		    (D20_CERT_LINE_LEN * D20_T61_CONV_FACT),
		    len_to_copy);
		d20_certlifields[3][len_to_copy] = D2_EOS;
	    }
	    if( X208ToDate( new_certl->to_be_signed.last_update,
		(signed32 *)(d20_certlifields[4]),  /* Year		*/
		(signed32 *)(d20_certlifields[5]),  /* Month		*/
		(signed32 *)(d20_certlifields[6]),  /* Day		*/
		(signed32 *)(d20_certlifields[7]),  /* Hours		*/
		(signed32 *)(d20_certlifields[8]),  /* Minutes		*/
		(signed32 *)(d20_certlifields[9]) ) /* Seconds		*/
		== D2_ERROR
	      )
	    {
		d20_qomask[D20_Q1POS].d20_ioinfo = d20_err_table[D20_E122IDX];
		d20_handlemask(d20_qomask);
		fkt = D2_ERROR;
		break;
	    }
	    if(new_certl->to_be_signed.rev_certs == (Rev_certs *)NULL)
					    /* Revoked Certificates	*/
		*(signed32 *)(d20_certlifields[10]) = 0;  /* NO		*/
	    else
		*(signed32 *)(d20_certlifields[10]) = 1;  /* YES	*/
	    *(signed32 *)(d20_certlifields[11]) = 3;  /* Modification	*/

	    /* Display mask now */
	    fkt = d20_handlemask(d20_certlomask);

	    /* Restore the CGET fields					*/
	    d20_restore_mask(d20_certlomask, sav_certlomask);

	    /* disable SCROLL_UP and F1 and SCROLL_DOWN	*/
	    /* in Certificate List mask	*/
	    d20_certlomask[D20_CERTLSCD].d20_fkt_id =
	    d20_certlomask[D20_CERTLSCU].d20_fkt_id =
	    d20_certlomask[D20_CERTLF1].d20_fkt_id = D20_NO_OP;

	    /* Display revoked certificate if required			*/
	    if ( ((fkt == FKTMENU) || (fkt == FKTCR)) &&
		new_certl->to_be_signed.rev_certs != (Rev_certs *)NULL)
	    {
		/* fkt = d20_handlemask(d20_rcertomask); */
		fkt = d20_rcert( object_counter,
				op,
				attr_name,
				&(new_certl->to_be_signed.rev_certs),
				(signed16 *)NULL /* unused */ );
	    }

	    break;

	case DS_ADD_OBJECT:
	case DS_DELETE_ATTRIBUTE:

	    /* Initializations of the fields	*/
	    strcpy(d20_attr_name, attr_name);
	    *(signed32 *)(d20_certlifields[0]) = 0; /* Algorithm	*/
	    *(signed32 *)(d20_certlifields[1]) = 0; /* Parameter	*/
	    d20_certlifields[2][0] = D2_EOS;	/* Issuer line 1	*/
	    d20_certlifields[3][0] = D2_EOS;	/* Issuer line 2	*/
	    *(signed32 *)(d20_certlifields[4]) = 0;  /* Year		*/
	    *(signed32 *)(d20_certlifields[5]) = 0;  /* Month		*/
	    *(signed32 *)(d20_certlifields[6]) = 0;  /* Day		*/
	    *(signed32 *)(d20_certlifields[7]) = 0;  /* Hours		*/
	    *(signed32 *)(d20_certlifields[8]) = 0;  /* Minutes		*/
	    *(signed32 *)(d20_certlifields[9]) = 0;  /* Seconds		*/
	    *(signed32 *)(d20_certlifields[10]) = 0; /* Revoked Certificates */
	    *(signed32 *)(d20_certlifields[11]) = 3; /* Modification	*/

	    mem_allocated = FALSE;

	    for (;;)
	    {
		/* Initializations */
		error_occured = FALSE;

		fkt = d20_handlemask(d20_certlomask);

		if( fkt != FKTDEL )		/* Save values		*/
		{
		    if( !mem_allocated )
		    {
			/* Initialize pointers / Allocate memory	*/
			if( (
			      (new_certl->to_be_signed.signature.parameters =
			       (signed32 *)malloc(sizeof(signed32)))
			       == (signed32 *)NULL
			    )
			  ||
			    (
			      (new_certl->to_be_signed.
					      signature.algorithm.d2_type =
			       (Ob_id_string)malloc(sizeof(Ob_id_string)
						* CERT_ALG_ID_MAX_LEN))
			       == (Ob_id_string)NULL
			    )
			  ||
			    (
			      (new_certl->to_be_signed.issuer =
			       (D2_name_string)malloc(D2_DNL_MAX))
			       == (D2_name_string)NULL
			    )
			  ||
			    (
			      (new_certl->to_be_signed.last_update =
			       (UTC_time_string)malloc(
					       sizeof(UTC_time_string)*20))
			       == (UTC_time_string)NULL
			    )
			  ||
			    (
			      (new_certl->alg.algorithm.d2_type =
			       (Ob_id_string)malloc(sizeof(Ob_id_string) * 20))
			       == (Ob_id_string)NULL
			    )
			  ||
			    (
			      (new_certl->encr_str.d2_value =
			       (unsigned char *)malloc(
					       sizeof(unsigned char *)*20))
			       == (unsigned char *)NULL
			    )
			  ||
			    (
			      (new_certl->alg.parameters =
			       (signed32 *)malloc(sizeof(signed32)))
			       == (signed32 *)NULL
			    )
			  )
			{
			    /* Error: not enough memory     */
			    error_occured = TRUE;
			    d20_qomask[D20_Q1POS].d20_ioinfo =
						d20_err_table[D20_E04IDX];
			    d20_handlemask(d20_qomask);
			    break;
			}
		    }

		    /* Fill structure fields now			*/
		    if( algTypeAdmValueToX509Value(
			    *(signed32 *)(d20_certlifields[0]),
			    &new_certl->to_be_signed.
				    signature.algorithm.d2_type,
			    &alg_id_len) == D2_ERROR )
		    {
			    error_occured = TRUE;
		    }
		    new_certl->to_be_signed.signature.algorithm.d2_typ_len =
				alg_id_len;
		    *(new_certl->to_be_signed.signature.parameters) =
				*(signed32 *)(d20_certlifields[1]);
		    strcpy((char *)new_certl->to_be_signed.issuer,
				d20_certlifields[2]);
		    strcat((char *)new_certl->to_be_signed.issuer,
				d20_certlifields[3]);
		    if( ds0_508_check_name(
			(char *)new_certl->to_be_signed.issuer)
			    == D2_NMSYNTAX_ERROR )
			{
			error_occured = TRUE;
			d20_qomask[D20_Q1POS].d20_ioinfo =
					    d20_errtab[D20_E106IDX];
			d20_handlemask(d20_qomask);
			fkt = D2_ERROR;
			}
		    if (DateToX208(
			 *(signed32 *)(d20_certlifields[4]),
			 *(signed32 *)(d20_certlifields[5]),
			 *(signed32 *)(d20_certlifields[6]),
			 *(signed32 *)(d20_certlifields[7]),
			 *(signed32 *)(d20_certlifields[8]),
			 *(signed32 *)(d20_certlifields[9]),
			&new_certl->to_be_signed.last_update)
			== D2_ERROR
		       )
		    {
			error_occured = TRUE;
			d20_qomask[D20_Q1POS].d20_ioinfo =
					    d20_err_table[D20_E122IDX];
			d20_handlemask(d20_qomask);
			fkt = D2_ERROR;
		    }

		    memcpy(new_certl->alg.algorithm.d2_type, CERT_RSA,
				    sizeof(CERT_RSA) - 1);
		    new_certl->alg.algorithm.d2_typ_len = sizeof(CERT_RSA) - 1;
		    *(new_certl->alg.parameters) = 0;

		    (new_certl->encr_str.d2_value)[0] = D2_EOS;
		    new_certl->encr_str.d2_size = 0;

		    /* Display mask for revoked certificate now		*/
		    if( *(signed32 *)(d20_certlifields[10]) == 1   /* YES */
			&& !error_occured )
		    {
			/* Display the revoked certificate mask		*/
			fkt = d20_rcert( object_counter,
					op,
					attr_name,
					&(new_certl->to_be_signed.rev_certs),
					&rcert_len );
		    }
		    else					/* NO	*/
		    {
			new_certl->to_be_signed.rev_certs =
							(Rev_certs *)NULL;
		    }

		    *r_len = sizeof(*new_certl);
		}

		if( !error_occured ) break;
	    }

	    if( error_occured )
		{
		fkt = D2_ERROR;
		}

	    break;

	default:	/* Programming error! Operation not allowed!	*/
	    fkt = D2_ERROR;
    }

return( fkt );
}



/**********************************************************************/
/*								      */
/* NAME             : d20_rcert()                                     */
/* AUTHOR           : Jean-Marc Bonnaudet - SNI BU BA KP 13           */
/* DATE             : 23.07.1993                                      */
/* DESCRIPTION      : Display the SAP mask with revoked certificate   */
/*                    attribute.                                      */
/* INPUT-PARAMETERS : op Operator                                     */
/* OUTPUT-PARAMETERS:                                                 */
/* RETURNVALUE      : signed32 fkt funktionkey                        */
/* CALLING FUNCTIONS:                                                 */
/* CALLED FUNCTIONS :                                                 */
/* NOTES            :                                                 */
/*								      */
/**********************************************************************/
signed32 d20_rcert(	signed32 object_counter,/* Object Counter	*/
			signed32 op,		/* Operator		*/
			char *attr_name,	/* Attribute name	*/
			Rev_certs **rep_str,
			signed16 *r_len )
{
/* Variables used for display only */
static Rev_certs	new_rcert;	/* What will be returned	*/
D20_omask	sav_rcertomask[D20_M_SIZE]; /* copy of CGET fields	*/

/* Variables used for add only */
Rev_certs	*new_rcert_to_add;	/* Revoked certificate to add	*/
Bool		error_occured;	/* Boolean value			*/
Bool		mem_allocated;	/* is mem already allocated ?		*/
signed32	len_to_copy;	/* Length which should be copied	*/
signed32        alg_id_len;	/* Length of an algorithm to add	*/
signed32	fkt;		/* Function key				*/

/* Variabled used by all */
Rev_cert_body	*current_rcert_body;
signed16	rcert_body_cnt = 0;	/* Counter of rev.cert.body	*/


    switch ((int) op)
    {
	case DS_DISPLAY_OBJECTS:
	case DS_MODIFY_ATTRIBUTE:

	    /* enable SCROLL_UP */
	    d20_rcertomask[D20_RCERTSCU].d20_fkt_id = D20_FKTX;
	    if (object_counter != 1)
		{
		/*  enable F1 and SCROLL_DOWN	*/
		/*  in Revoked Certificate mask	*/
		d20_rcertomask[D20_RCERTSCD].d20_fkt_id =
		d20_rcertomask[D20_RCERTF1].d20_fkt_id = D20_FKTX;
		}

	    /* Change CGET fields in CSAY fields	*/
	    d20_save_mask(d20_rcertomask, sav_rcertomask);

	    /* Put the values in the fields of the mask	*/
	    new_rcert = **rep_str;

	    /* Initializations of the fields	*/

	    /* strcpy(d20_attr_name, attr_name); */	/* Already set	*/

	    for( /* rcert_body_cnt = 0, */	/* Already initialized	*/
		 current_rcert_body = new_rcert.to_be_signed.rev_cert;
		 rcert_body_cnt < new_rcert.to_be_signed.no_rev_certs;
		 current_rcert_body++, rcert_body_cnt++ )
	    {
		if ( X509ToAlgTypeAdm(		/* Signature		*/
		    &current_rcert_body->signature.algorithm.d2_type,
		    current_rcert_body->signature.algorithm.d2_typ_len,
		    (signed32 *)(d20_rcertifields[0])) == D2_ERROR )
		{
		    fkt = D2_ERROR;
		    break;
		}

		if( current_rcert_body->signature.parameters != NULL )
		    *(signed32 *)(d20_rcertifields[1]) =  /* Parameter	*/
			*(current_rcert_body->signature.parameters);
		else
		    *(char *)(d20_rcertifields[1]) = D2_EOS;

		d20_gen_dnintf((String)current_rcert_body->issuer);
		len_to_copy = MIN(
		    (int)strlen((char *)current_rcert_body->issuer),
		    (D20_CERT_LINE_LEN * D20_T61_CONV_FACT) );
		strncpy(d20_rcertifields[2],	/* Issuer line 1	*/
			(char *)current_rcert_body->issuer,
			len_to_copy);
		d20_rcertifields[2][len_to_copy] = D2_EOS;
		d20_rcertifields[3][0] = D2_EOS;
		if ((int)strlen((char *)current_rcert_body->issuer) >
		    (D20_CERT_LINE_LEN * D20_T61_CONV_FACT))
		{
		    len_to_copy = MIN(
		    (int)strlen((char *)current_rcert_body->issuer) -
		    (D20_CERT_LINE_LEN * D20_T61_CONV_FACT),
		    (D20_CERT_LINE_LEN * D20_T61_CONV_FACT) );
		    strncpy( d20_rcertifields[3], /* Issuer line 2	*/
			(char *)current_rcert_body->issuer +
			(D20_CERT_LINE_LEN * D20_T61_CONV_FACT),
			len_to_copy);
		    d20_rcertifields[3][len_to_copy] = D2_EOS;
		}

		*(signed32 *)(d20_rcertifields[4]) =/* Serial Number	*/
			current_rcert_body->user_cert;

		if( X208ToDate( current_rcert_body->rev_date,
		    (signed32 *)(d20_rcertifields[5]),	/* Year		*/
		    (signed32 *)(d20_rcertifields[6]),	/* Month	*/
		    (signed32 *)(d20_rcertifields[7]),	/* Day		*/
		    (signed32 *)(d20_rcertifields[8]),	/* Hours	*/
		    (signed32 *)(d20_rcertifields[9]),	/* Minutes	*/
		    (signed32 *)(d20_rcertifields[10]) )/* Seconds	*/
		    == D2_ERROR
		  )
		{
		    d20_qomask[D20_Q1POS].d20_ioinfo =
			    d20_err_table[D20_E122IDX];
		    d20_handlemask(d20_qomask);
		    fkt = D2_ERROR;
		    break;
		}

		/* Set the "Other Revoked Certificate:" "YES/NO" line	*/
		if( (rcert_body_cnt+1) < new_rcert.to_be_signed.no_rev_certs )
		    *(signed32 *)(d20_rcertifields[11]) = 1; /* YES	*/
		else
		    *(signed32 *)(d20_rcertifields[11]) = 0; /* NO	*/

		*(signed32 *)(d20_rcertifields[12]) = 3; /* Modification*/

		/* Display mask	*/
		fkt = d20_handlemask(d20_rcertomask);
	    }

	    /* Restore the CGET fields			*/
	    d20_restore_mask(d20_rcertomask, sav_rcertomask);

	    /* disable SCROLL_UP and F1 and SCROLL_DOWN	*/
	    /* in Certificate List mask	*/
	    d20_rcertomask[D20_RCERTSCD].d20_fkt_id =
	    d20_rcertomask[D20_RCERTSCU].d20_fkt_id =
	    d20_rcertomask[D20_RCERTF1].d20_fkt_id = D20_NO_OP;

	    break;

	case DS_ADD_OBJECT:
	case DS_DELETE_ATTRIBUTE:

	    error_occured = FALSE;

	    /* We have to allocate some new place so that next		*/
	    /* revoked certificate will not overwritte these data	*/
	    if( (new_rcert_to_add = (Rev_certs *)malloc(sizeof(Rev_certs)))
		== (Rev_certs *)NULL )
		error_occured = TRUE;

	    /* Initialize the Rev_cert_body pointer			*/
	    if( (new_rcert_to_add->to_be_signed.rev_cert =
		(Rev_cert_body *)malloc(sizeof(Rev_cert_body)*MAX_RCERT_BODY))
		== (Rev_cert_body *)NULL )
		error_occured = TRUE;

	    if( error_occured )
	    {
		/* Error: not enough memory     */
		error_occured = TRUE;
		d20_qomask[D20_Q1POS].d20_ioinfo =
				    d20_err_table[D20_E04IDX];
		d20_handlemask(d20_qomask);
	    }
	    else
	    {			/* I did not ident to keep enough place	*/

	    do			/* Add one rev. cert. body at each loop	*/
	    {
		/* This points to the Rev_cert_body which will be added	*/
		current_rcert_body = new_rcert_to_add->to_be_signed.rev_cert +
				     rcert_body_cnt++;

		/* Initializations of the fields	*/
		*(signed32 *)(d20_rcertifields[0]) = 0;	/* Algorithm	*/
		*(signed32 *)(d20_rcertifields[1]) = 0;	/* Parameter	*/
		d20_rcertifields[2][0] = D2_EOS;	/* Issuer line 1*/
		d20_rcertifields[3][0] = D2_EOS;	/* Issuer line 1*/
		*(signed32 *)(d20_rcertifields[4]) = 0;	/* User Certificate*/
		*(signed32 *)(d20_rcertifields[5]) = 0;	/* Year		*/
		*(signed32 *)(d20_rcertifields[6]) = 0;	/* Month	*/
		*(signed32 *)(d20_rcertifields[7]) = 0;	/* Day		*/
		*(signed32 *)(d20_rcertifields[8]) = 0;	/* Hours	*/
		*(signed32 *)(d20_rcertifields[9]) = 0;	/* Minutes	*/
		*(signed32 *)(d20_rcertifields[10]) = 0;/* Seconds	*/
		*(signed32 *)(d20_rcertifields[11]) = 0;/* Other rev. cert.*/
		*(signed32 *)(d20_rcertifields[12]) = 3;/* Modification	*/

		mem_allocated = FALSE;

		for (;;)
		{
		    /* Initializations */
		    error_occured = FALSE;

		    fkt = d20_handlemask(d20_rcertomask);

		    if( fkt != FKTDEL )		/* Save values		*/
		    {
			if( !mem_allocated )
			{
			    /* Initialize pointers / Allocate memory	*/
			    if( (
				  (current_rcert_body->signature.parameters =
				   (signed32 *)malloc(sizeof(signed32)))
				   == (signed32 *)NULL
				)
			      ||
				(
				  (current_rcert_body->
				      signature.algorithm.d2_type =
				  (Ob_id_string)malloc(sizeof(Ob_id_string)
						    * CERT_ALG_ID_MAX_LEN))
				   == (Ob_id_string)NULL
				)
			      ||
				(
				  (current_rcert_body->issuer =
				   (D2_name_string)malloc(D2_DNL_MAX))
				   == (D2_name_string)NULL
				)
			      ||
				(
				  (current_rcert_body->rev_date =
				   (UTC_time_string)
				   malloc(sizeof(UTC_time_string)*20))
				   == (UTC_time_string)NULL
				)
			      )
			    {
				/* Error: not enough memory     */
				error_occured = TRUE;
				d20_qomask[D20_Q1POS].d20_ioinfo =
						    d20_err_table[D20_E04IDX];
				d20_handlemask(d20_qomask);
			    break;
			    }
			}

			/* Fill structure fields now			*/
			if( algTypeAdmValueToX509Value(
				*(signed32 *)(d20_rcertifields[0]),
				&current_rcert_body->
				  signature.algorithm.d2_type,
				&alg_id_len) == D2_ERROR )
			{
				error_occured = TRUE;
			}

			current_rcert_body->signature.algorithm.d2_typ_len =
				alg_id_len;
			*(current_rcert_body->signature.parameters) =
				*(signed32 *)(d20_rcertifields[1]);

			strcpy((char *)current_rcert_body->issuer,
				d20_rcertifields[2]);
			strcat((char *)current_rcert_body->issuer,
				d20_rcertifields[3]);
			if(ds0_508_check_name(
				(char *)current_rcert_body->issuer )
				== D2_NMSYNTAX_ERROR)
			    {
			    error_occured = TRUE;
			    d20_qomask[D20_Q1POS].d20_ioinfo =
						d20_errtab[D20_E106IDX];
			    d20_handlemask(d20_qomask);
			    fkt = D2_ERROR;
			    }

			current_rcert_body->user_cert =
				*(signed32 *)(d20_rcertifields[4]);

			if (DateToX208(
			     *(signed32 *)(d20_rcertifields[5]),
			     *(signed32 *)(d20_rcertifields[6]),
			     *(signed32 *)(d20_rcertifields[7]),
			     *(signed32 *)(d20_rcertifields[8]),
			     *(signed32 *)(d20_rcertifields[9]),
			     *(signed32 *)(d20_rcertifields[10]),
			     &current_rcert_body->rev_date)
			    == D2_ERROR
			   )
			{
			    error_occured = TRUE;
			    d20_qomask[D20_Q1POS].d20_ioinfo =
						d20_err_table[D20_E122IDX];
			    d20_handlemask(d20_qomask);
			    fkt = D2_ERROR;
			}
		    }

		    if( !error_occured ) break;
		}

	    }

	    while(*(signed32 *)(d20_rcertifields[11]) /* Other rev. cert.*/
		    == 1 /* YES */
		 && rcert_body_cnt <= MAX_RCERT_BODY
		 && fkt != FKTDEL);

	    if(*(signed32 *)(d20_rcertifields[11]) /* Other rev. cert.*/
		    == 1 /* YES */
		 && rcert_body_cnt == MAX_RCERT_BODY )
	    {
		/* Display a message :					*/
		/* "The administration can't add more than ten revoked	*/
		/* certificates."					*/
		d20_qomask[D20_Q1POS].d20_ioinfo =
				    d20_err_table[D20_E119IDX];
		d20_handlemask(d20_qomask);
	    } /* of for(;;)				*/

	    } /* of if( error_occured ) ... else	*/

	    if( error_occured )
	    {
		fkt = D2_ERROR;
	    }
	    else	/* The revoked certificate has to be completed	*/
	    {
		/* Allocate memory		*/
		if( (
		      (new_rcert_to_add->alg.algorithm.d2_type =
		       (Ob_id_string)malloc(sizeof(Ob_id_string) * 20))
		       == (Ob_id_string)NULL
		    )
		  ||
		    (
		      (new_rcert_to_add->encr_str.d2_value =
		       (unsigned char *)malloc(sizeof(unsigned char *)*20))
		       == (unsigned char *)NULL
		    )
		  ||
		    (
		      (new_rcert_to_add->alg.parameters =
		       (signed32 *)malloc(sizeof(signed32)))
		       == (signed32 *)NULL
		    )
		  )
		{
		    /* Error: not enough memory     */
		    error_occured = TRUE;
		    d20_qomask[D20_Q1POS].d20_ioinfo =
					d20_err_table[D20_E04IDX];
		    d20_handlemask(d20_qomask);
		    break;
		}

		memcpy(new_rcert_to_add->alg.algorithm.d2_type, CERT_RSA,
				sizeof(CERT_RSA) - 1);
		new_rcert_to_add->alg.algorithm.d2_typ_len =
				sizeof(CERT_RSA) - 1;
		*(new_rcert_to_add->alg.parameters) = 0;

		(new_rcert_to_add->encr_str.d2_value)[0] = D2_EOS;
		new_rcert_to_add->encr_str.d2_size = 0;

				/* Number of revoked certificate body:	*/
		new_rcert_to_add->to_be_signed.no_rev_certs = rcert_body_cnt;
	    }

	    *rep_str = new_rcert_to_add;
	    *r_len = sizeof(*new_rcert_to_add);

	    break;

	default:
	    fkt = D2_ERROR;
	    break;
    }

return( fkt );
}



/**********************************************************************/
/*								      */
/* NAME             : d20_dme_disp()                                  */
/* AUTHOR           : Jean-Marc Bonnaudet - SNI BU BA KP 13           */
/* DATE             : 16.07.1993                                      */
/* DESCRIPTION      : Display the DME mask with attributes.           */
/* INPUT-PARAMETERS : op Operator                                     */
/* OUTPUT-PARAMETERS:                                                 */
/* RETURNVALUE      : signed32 fkt funktionkey                        */
/* CALLING FUNCTIONS:                                                 */
/* CALLED FUNCTIONS :                                                 */
/* NOTES            :                                                 */
/*								      */
/**********************************************************************/
signed32 d20_dme_disp(	signed32 object_counter,/* Object Counter	*/
			signed32 op,		/* Operator		*/
			char *attr_name,	/* Attribute name	*/
			Octet_string *rep_str,
			signed16 *r_len )
{
signed32	fkt;		/* Returned funktion key		*/
D2_alt_addr	*alt_addr;	/* Alternate address for the mask	*/
D2_alt_addr	alt_addr_struct;/* For Add, memory should be allocated	*/
D2_obj_id	*obj_id;	/* The current object id		*/
D2_obj_id	*first_obj_id;	/* The first object id			*/
static D2_obj_id obj_id_list[DME_MAX_OBJECT_ID];
				/* To store the object id to add	*/
static unsigned char	obj_id_type[DME_MAX_OBJECT_ID][DME_OBJECT_ID_LEN + 1];
		/* The object id struct has to point to some place	*/
signed32	obj_id_cnt;	/* Object identifier counter		*/
signed32	obj_id_nr;	/* Number of object id, useful for add	*/
D2_str		*str_ptr;	/* Pointer for ??code_alt_ddr(...)	*/
D2_str		str_struct;	/* Parameter for ??code_alt_addr(...)	*/
D20_omask	sav_dmeomask[D20_M_SIZE]; /* Temp. copy of CGET fields	*/
signed32	len_to_copy;	/* Length which should be copied	*/
signed32	str_position;	/* Current position in the string	*/
unsigned char  *str_to_copy;	/* String which should be copied	*/
Bool		error_occured;	/* Boolean value			*/
char obj_id_str[D2_OBJ_IDL_MAX + 1];
				/* For display				*/
D2_obj_id junk_obj_id;		/* For obj id format checking in add	*/
unsigned char junk_obj_id_str[DME_OBJECT_ID_LEN + 1];
				/* space for junk_obj_id	*/

    /* Processing	*/
    switch((int) op)
	{
	case DS_DISPLAY_OBJECTS:

	    /* enable SCROLL_UP */
	    d20_dmeomask[D20_DMESCU].d20_fkt_id = D20_FKTX;
	    if (object_counter != 1)
		{
		/*  enable F1 and			*/
		/*  SCROLL_DOWN in DME mask		*/
		d20_dmeomask[D20_DMESCD].d20_fkt_id =
		d20_dmeomask[D20_DMEF1].d20_fkt_id = D20_FKTX;
		}

	    /* Change CGET fields in CSAY fields	*/
	    d20_save_mask(d20_dmeomask, sav_dmeomask);

	    /* Fill D2_alt_addr structure with the octet string	*/
	    /* Mask is filled with the values of the struct then.	*/
	    str_struct.d2_size = (signed32)*r_len;
	    str_struct.d2_value = (unsigned char *)*rep_str;
	    if(gds_decode_alt_addr( &str_struct, &alt_addr ) == D2_ERROR)
		{ fkt = D2_ERROR; break; } /* Memory allocation problem */

	    /*** Copy the values in the corresponding fields	*/

	    /** Address			*/

	    /* Clean-up the fields		*/
	    d20_dme_addr[0]  = d20_dme_addr1[0] = d20_dme_addr2[0] =
	    d20_dme_addr3[0] = d20_dme_addr4[0] = d20_dme_addr5[0] = D2_EOS;

	    /* Initialize			*/
	    str_to_copy = alt_addr->d2_addr.d2_value;
	    str_position = 0;

	    /* Copy value in line 1 of address	*/
	    len_to_copy = MIN(alt_addr->d2_addr.d2_size/* - str_position */,
			      DME_ADDRESS_FIRST_LINE_LEN * D20_T61_CONV_FACT);
	    strncpy(d20_dme_addr1, (char *)str_to_copy, len_to_copy);
	    d20_dme_addr1[len_to_copy] = D2_EOS;
	    str_position = len_to_copy;

	    /* Copy value in line 2 of address	*/
	    if(alt_addr->d2_addr.d2_size > str_position)
		{
		str_to_copy += len_to_copy;
		len_to_copy = MIN(alt_addr->d2_addr.d2_size - str_position,
				  DME_ADDRESS_LINE_LEN * D20_T61_CONV_FACT);
		strncpy(d20_dme_addr2, (char *)str_to_copy, len_to_copy);
		d20_dme_addr2[len_to_copy] = D2_EOS;
		str_position += len_to_copy;

		/* Copy value in line 3 of address	*/
		if(alt_addr->d2_addr.d2_size > str_position)
		    {
		    str_to_copy += len_to_copy;
		    len_to_copy = MIN(alt_addr->d2_addr.d2_size - str_position,
				      DME_ADDRESS_LINE_LEN * D20_T61_CONV_FACT);
		    strncpy(d20_dme_addr3, (char *)str_to_copy, len_to_copy);
		    d20_dme_addr3[len_to_copy] = D2_EOS;
		    str_position += len_to_copy;

		    /* Copy value in line 4 of address	*/
		    if(alt_addr->d2_addr.d2_size > str_position)
			{
			str_to_copy += len_to_copy;
			len_to_copy = MIN(
				alt_addr->d2_addr.d2_size - str_position, 
				DME_ADDRESS_LINE_LEN * D20_T61_CONV_FACT);
			strncpy(d20_dme_addr4,
				(char *)str_to_copy,
				len_to_copy);
			d20_dme_addr4[len_to_copy] = D2_EOS;
			str_position += len_to_copy;

			/* Copy value in line 5 of address	*/
			if(alt_addr->d2_addr.d2_size > str_position)
			    {
			    str_to_copy += len_to_copy;
			    len_to_copy = 
				MIN(alt_addr->d2_addr.d2_size - str_position, 
				DME_ADDRESS_LINE_LEN * D20_T61_CONV_FACT);
			    strncpy(d20_dme_addr5,
				    (char *)str_to_copy,
				    len_to_copy);
			    d20_dme_addr5[len_to_copy] = D2_EOS;
			    /* str_position += len_to_copy;	*/
			    }		/* Line 5	*/
			}		/* Line 4	*/
		    }			/* Line 3	*/
		}			/* Line 2	*/

	    /*** Objects identifiers	*/
	    for(obj_id_cnt = 0, obj_id = (alt_addr->d2_ob_id_set).d2_obj_id;
		obj_id_cnt < (alt_addr->d2_ob_id_set).d2_num;
		obj_id_cnt++, obj_id++)
		    {
		    /* The conversion routine does't seem to like	*/
		    /* empty values (returns some junk) so this has	*/
		    /* to be checked.					*/
		    if(obj_id->d2_typ_len != 0)
			{
			if(d27_301_oid_str(obj_id, obj_id_str) == D2_ERROR)
			    {
			    /* An error has occured.			*/
			    /* Copy the real value without conversion	*/
			    len_to_copy = MIN(obj_id->d2_typ_len,
					    DME_OBJECT_ID_LEN);
			    strncpy(d20_dme_object_id[obj_id_cnt],
				    (char *)obj_id->d2_type,
				    len_to_copy);
			    }
			else
			    {
			    len_to_copy = MIN( (int)strlen(obj_id_str),
					    DME_OBJECT_ID_LEN);
			    strncpy(d20_dme_object_id[obj_id_cnt],
				    obj_id_str,
				    len_to_copy);
			    }
			}
		    else
			len_to_copy = 0;

		    d20_dme_object_id[obj_id_cnt][len_to_copy] = D2_EOS;
		    }
	    /* Put an EOS in the empty fields	*/
	    for( ; obj_id_cnt < DME_MAX_OBJECT_ID; obj_id_cnt++)
		d20_dme_object_id[obj_id_cnt][0] = D2_EOS;

	    /*** Display mask with the values	*/
	    fkt = d20_handlemask(d20_dmeomask);

	    /* Restore the CGET fields		*/
	    d20_restore_mask(d20_dmeomask, sav_dmeomask);

	    /* disable SCROLL_UP and F1 and	*/
	    /* SCROLL_DOWN in DME mask		*/
	    d20_dmeomask[D20_DMESCD].d20_fkt_id =
	    d20_dmeomask[D20_DMESCU].d20_fkt_id =
	    d20_dmeomask[D20_DMEF1].d20_fkt_id = D20_NO_OP;

	    break;

	case DS_ADD_OBJECT:
	case DS_ADD_ATTRIBUTE:

	    /* Clean-up the fields		*/
	    d20_dme_addr[0]  = d20_dme_addr1[0] = d20_dme_addr2[0] =
	    d20_dme_addr3[0] = d20_dme_addr4[0] = d20_dme_addr5[0] = D2_EOS;
	    for(obj_id_cnt = 0; obj_id_cnt < DME_MAX_OBJECT_ID; obj_id_cnt++)
		d20_dme_object_id[obj_id_cnt][0] = D2_EOS;
	    junk_obj_id.d2_type = junk_obj_id_str;

	    /* Display mask to be filled	*/
	    for (;;)
		{
		/* Initialization	*/
		error_occured = FALSE;

		fkt = d20_handlemask(d20_dmeomask);

		if( fkt != FKTDEL )
		    {
		    /* Concatenate the 5 lines of the address in one string */
		    strcpy( d20_dme_addr, d20_dme_addr1 );
		    strcat( d20_dme_addr, d20_dme_addr2 );
		    strcat( d20_dme_addr, d20_dme_addr3 );
		    strcat( d20_dme_addr, d20_dme_addr4 );
		    strcat( d20_dme_addr, d20_dme_addr5 );

		    /* Check if an address is empty or not		*/
		    /* Entry of any object id is optional		*/
		    if ((d20_dme_addr[0] == DS_DEFAULT_VALUE ||
			 d20_dme_addr[0] == D2_EOS))
			    {
			    d20_qomask[D20_Q1POS].d20_ioinfo=
				d20_err_table[D20_E116IDX];
			    d20_handlemask(d20_qomask);
			    error_occured = TRUE;
			    }
		    else
		    /* Check if the object id attribute syntax is correct */
			{
			for(obj_id_cnt = 0;
			    obj_id_cnt < DME_MAX_OBJECT_ID;
			    obj_id_cnt++)
			    {
			    if( strlen( d20_dme_object_id[obj_id_cnt] ) != 0)
				{ /* This function also copy the value	*/
				if(d27_302_str_oid(d20_dme_object_id[obj_id_cnt],
				    &junk_obj_id) == D2_ERROR)
				    {
				    d20_qomask[D20_Q1POS].d20_ioinfo=
					d20_err_table[D20_E117IDX];
				    d20_handlemask(d20_qomask);
				    error_occured = TRUE;
				    break;
				    }
				}
			    }
			}
		    }

		if( !error_occured ) break;
		}

	    if (fkt != FKTDEL)
		{
		/* Initialization	*/
		error_occured = FALSE;

		/*** Save values in the D2_alt_addr struct	***/

		/** Address		*/

		/* Allocate memory	*/
		alt_addr = &alt_addr_struct;

		/* Set the values	*/
		alt_addr->d2_addr.d2_size = strlen(d20_dme_addr);
		alt_addr->d2_addr.d2_value = (unsigned char *)d20_dme_addr;
		for(obj_id_cnt = 0;
		    obj_id_cnt < DME_MAX_OBJECT_ID;
		    obj_id_cnt++)
		    obj_id_list[obj_id_cnt].d2_type = obj_id_type[obj_id_cnt];

		/** Objects identifiers	*/

		/* Allocate memory	*/
		if( (first_obj_id = obj_id =
			    (D2_obj_id *)malloc(DME_MAX_OBJECT_ID *
			    sizeof(D2_obj_id)) ) == (D2_obj_id *)NULL)
		    fkt = D2_ERROR;	/* Error: not enough memory	*/

		/* Set the values	*/
		for(obj_id_cnt = 0, obj_id_nr = 0;
		    obj_id_cnt < DME_MAX_OBJECT_ID;
		    obj_id_cnt++, obj_id++)
			{
			/* Convert */
			if( strlen( d20_dme_object_id[obj_id_cnt] ) != 0)
			    {
			    if(d27_302_str_oid(d20_dme_object_id[obj_id_cnt],
				&obj_id_list[obj_id_cnt]) == D2_ERROR)
				{
				error_occured = TRUE;
				/*** Don't put the value like it is.
				obj_id->d2_typ_len =
				    strlen(d20_dme_object_id[obj_id_cnt]);
				obj_id->d2_type =
				(unsigned char *)d20_dme_object_id[obj_id_cnt];
				***/
				}
			    else
				{
				obj_id->d2_typ_len =
					obj_id_list[obj_id_cnt].d2_typ_len;
				obj_id->d2_type =
					obj_id_list[obj_id_cnt].d2_type;
				}
			    }
			else
			    {
			    obj_id->d2_typ_len = 0;
			    }

			/* Save the position of the last non-null field	*/
			if(obj_id->d2_typ_len != 0) obj_id_nr = obj_id_cnt;
			}
		if ( error_occured )
		    {
		    d20_qomask[D20_Q1POS].d20_ioinfo =
			d20_err_table[D20_E117IDX];
		    d20_handlemask(d20_qomask);
		    fkt = D2_ERROR;
		    }
		else
		    {
		    (alt_addr->d2_ob_id_set).d2_num = obj_id_nr + 1;
		    (alt_addr->d2_ob_id_set).d2_obj_id = first_obj_id;

		    /*** Convert the struct in an octet_string	***/
		    if(gds_encode_alt_addr(alt_addr, &str_ptr) == D2_ERROR)
			{ fkt = D2_ERROR; break; }
			/* Memory allocation problem */
		    *r_len = (signed16)(str_ptr->d2_size);
		    *rep_str = (Octet_string)(str_ptr->d2_value);
		    }
		}
	    break;

	default:	/* Programming error! Operation not allowed!	*/
	    fkt = D2_ERROR;
	}

    return( fkt );
}


/**********************************************************************/
/*								      */
/* Funktions to copy the values from the structure into the fields    */
/*								      */
/**********************************************************************/

 static signed32 d2_cp_from_ttxid(
   D2_ttxid   *ttxid)

{
signed32	copy_len;

copy_len = ttxid->d2_terminal.d2_size <= D20_TT_TERM_FLD_LEN ? 
	   ttxid->d2_terminal.d2_size : D20_TT_TERM_FLD_LEN;

strncpy(d20_ttxidifields[DS_TT_TERM], (char *)ttxid->d2_terminal.d2_value,
    	copy_len);
d20_ttxidifields[DS_TT_TERM][copy_len] = D2_EOS;

if (ttxid->d2_controls.d2_size != 0)
	{
	copy_len = ttxid->d2_controls.d2_size <= 
	(D20_TT_REST_FLD_LEN * D20_T61_CONV_FACT) ? 
	ttxid->d2_controls.d2_size : (D20_TT_REST_FLD_LEN * D20_T61_CONV_FACT);

	strncpy(d20_ttxidifields[DS_TT_CONTR], 
		(char *)ttxid->d2_controls.d2_value, copy_len);
	d20_ttxidifields[DS_TT_CONTR][copy_len] = D2_EOS;
	}

if (ttxid->d2_graphics.d2_size != 0)
	{
	copy_len = ttxid->d2_graphics.d2_size <= 
	(D20_TT_REST_FLD_LEN * D20_T61_CONV_FACT) ? 
	ttxid->d2_graphics.d2_size : (D20_TT_REST_FLD_LEN * D20_T61_CONV_FACT);

	strncpy(d20_ttxidifields[DS_TT_GRAPH], 
		(char *)ttxid->d2_graphics.d2_value, copy_len);
	d20_ttxidifields[DS_TT_GRAPH][copy_len] = D2_EOS;
	}

if (ttxid->d2_miscel.d2_size != 0)
	{
	copy_len = ttxid->d2_miscel.d2_size <= 
	(D20_TT_REST_FLD_LEN * D20_T61_CONV_FACT) ? 
	ttxid->d2_miscel.d2_size : (D20_TT_REST_FLD_LEN * D20_T61_CONV_FACT);

	strncpy(d20_ttxidifields[DS_TT_MISCL], 
		(char *)ttxid->d2_miscel.d2_value, copy_len);
	d20_ttxidifields[DS_TT_MISCL][copy_len] = D2_EOS;
	}

if (ttxid->d2_pages.d2_size != 0)
	{
	copy_len = ttxid->d2_pages.d2_size <= D20_TT_REST_FLD_LEN ? 
		   ttxid->d2_pages.d2_size : D20_TT_REST_FLD_LEN;

	strncpy(d20_ttxidifields[DS_TT_PAGES], 
		(char *)ttxid->d2_pages.d2_value, copy_len);
	d20_ttxidifields[DS_TT_PAGES][copy_len] = D2_EOS;
	}

if (ttxid->d2_privates.d2_size != 0)
	{
	copy_len = ttxid->d2_privates.d2_size <= D20_TT_REST_FLD_LEN ? 
		   ttxid->d2_privates.d2_size : D20_TT_REST_FLD_LEN;

	strncpy(d20_ttxidifields[DS_TT_PRIV], 
		(char *)ttxid->d2_privates.d2_value, copy_len);
	d20_ttxidifields[DS_TT_PRIV][copy_len] = D2_EOS;
	}
}


 static signed32 d2_cp_from_telex(
   D2_telex_number   *telex)

{
signed32	copy_len;


copy_len = telex->d2_t_number.d2_size <= D20_TL_NO_FLD_LEN ? 
	   telex->d2_t_number.d2_size : D20_TL_NO_FLD_LEN;

strncpy(d20_telexifields[DS_TL_TELEX], (char *)telex->d2_t_number.d2_value,
	copy_len);
d20_telexifields[DS_TL_TELEX][copy_len] = D2_EOS;

copy_len = telex->d2_c_code.d2_size <= D20_TL_CC_FLD_LEN ? 
	   telex->d2_c_code.d2_size : D20_TL_CC_FLD_LEN;

strncpy(d20_telexifields[DS_TL_CODE], (char *)telex->d2_c_code.d2_value,
	copy_len);
d20_telexifields[DS_TL_CODE][copy_len] = D2_EOS;

copy_len = telex->d2_answer.d2_size <= D20_TL_ANS_FLD_LEN ? 
	   telex->d2_answer.d2_size : D20_TL_ANS_FLD_LEN;

strncpy(d20_telexifields[DS_TL_ANSW], (char *)telex->d2_answer.d2_value,
	copy_len);
d20_telexifields[DS_TL_ANSW][copy_len] = D2_EOS;

}


 static signed32 d2_cp_from_fax(
   D2_faxnumber     *fax)

{
signed32     	i;
signed32	copy_len;

copy_len = fax->d2_tel_nr.d2_size <= D20_FX_NO_FLD_LEN ? 
	   fax->d2_tel_nr.d2_size : D20_FX_NO_FLD_LEN;

strncpy(d20_faxifields[DS_FX_NR], (char *)fax->d2_tel_nr.d2_value, copy_len);
d20_faxifields[DS_FX_NR][copy_len] = D2_EOS;

    if (fax->d2_par_defined == TRUE) {
	   for (i=DS_FX_A3W; i <= DS_FX_UNLIML; i++) {
		   if ((fax->d2_par & fax_bits[i-1]) == fax_bits[i-1])
			  *(signed32 *)d20_faxifields[i] = TRUE;
	   }
    }
}


 static signed32 d2_cp_from_t61(
   D2_T61_seq     *t61)

{
signed32      i;
signed32	copy_len;

for (i=0; i < t61->d2_nb_item; i++) {
	copy_len = t61->d2_t61_item[i].d2_size <= 
	(D20_POST_ADD_FLD_LEN * D20_T61_CONV_FACT) ? 
	t61->d2_t61_item[i].d2_size:(D20_POST_ADD_FLD_LEN * D20_T61_CONV_FACT);

	strncpy(d20_t61ifields[i], (char *)t61->d2_t61_item[i].d2_value, 
		copy_len);
	d20_t61ifields[i][copy_len] = D2_EOS;
	}
}


 static signed32 d2_cp_from_str_list(
   D2_str_list     *str_list)

{
signed32      i, copy_len;

i=0; 

while (str_list != (D2_str_list *)NULL) 
	{
	copy_len = str_list->d2_str.d2_size <= 
	(D20_POST_ADD_FLD_LEN * D20_T61_CONV_FACT) ? 
	str_list->d2_str.d2_size : (D20_POST_ADD_FLD_LEN * D20_T61_CONV_FACT);

	strncpy(d20_t61ifields[i], (char *)str_list->d2_str.d2_value,
		copy_len);
	d20_t61ifields[i][copy_len] = D2_EOS;
	str_list = str_list->d2_next;
	i++;
	}
}


 static signed32 d2_cp_from_dnscell(
   x500_dns_cell_t  *dns_cell)

{
strncpy(d20_dnscellifields[DS_DCELL_NSPCTS], dns_cell->nsp_uuid,D20_DC_FLD_LEN);
d20_dnscellifields[DS_DCELL_NSPCTS][D20_DC_FLD_LEN] = D2_EOS;

if (dns_cell->root_uuid != (char *) NULL)
	{	
	strncpy(d20_dnscellifields[DS_DCELL_RCTS], dns_cell->root_uuid, 
	       D20_DC_FLD_LEN);
	d20_dnscellifields[DS_DCELL_RCTS][D20_DC_FLD_LEN] = D2_EOS;
	}

if (dns_cell->root_name != (char *) NULL)
	{
	strncpy(d20_dnscellifields[DS_DCELL_RNM], dns_cell->root_name, 
	       D20_DC_FLD_LEN);
	d20_dnscellifields[DS_DCELL_RNM][D20_DC_FLD_LEN] = D2_EOS;
	}
}


 static signed32 d2_cp_from_dnsrep(
   x500_replica_t  *dns_rep)

{
char     *cp;
signed32  i;
signed32  len;

*(signed32 *) d20_dnsrepifields[DS_REP_TYP] = dns_rep->type;

strncpy(d20_dnsrepifields[DS_REP_CHCTS], dns_rep->ch_uuid, D20_DR_FLD_LEN);
d20_dnsrepifields[DS_REP_CHCTS][D20_DR_FLD_LEN] = D2_EOS;

strncpy(d20_dnsrepifields[DS_REP_CHNMA], dns_rep->ch_name, D20_DR_FLD_LEN);
d20_dnsrepifields[DS_REP_CHNMA][D20_DR_FLD_LEN] = D2_EOS;

if ((int)strlen(dns_rep->ch_name) > D20_DR_FLD_LEN)                            
	{                                                                       
	strncpy(d20_dnsrepifields[DS_REP_CHNMB], 
		dns_rep->ch_name + D20_DR_FLD_LEN, D20_DR_FLD_LEN);                                                            
	d20_dnsrepifields[DS_REP_CHNMB][D20_DR_FLD_LEN] = D2_EOS;               
	}                                                                       
else                                                                      
	d20_dnsrepifields[DS_REP_CHNMB][0] = D2_EOS;                            


/* provide the tower-fields */
len = 0;
cp = (char *)dns_rep->tower;

strncpy(d20_dnsrepifields[DS_REP_TW1], cp, D20_DR_FLD_LEN);
d20_dnsrepifields[DS_REP_TW1][D20_DR_FLD_LEN] = D2_EOS;

while (*cp++ != '\0') {
    len++;
}
len++;

for(i=DS_REP_TW2; i <= DS_REP_TW5; i++) {
  if (len == dns_rep->tower_len) {
      break;
  }
  else {
       strncpy(d20_dnsrepifields[i], cp, D20_DR_FLD_LEN);
       d20_dnsrepifields[i][D20_DR_FLD_LEN] = D2_EOS;

       while(*cp++ != '\0') {
	     len++;
       }
       len++;
  }
}

for(; i <= DS_REP_TW5; i++) {
  *d20_dnsrepifields[i] = D2_EOS;
}

}


/**********************************************************************/
/*								      */
/* Funktions to copy the inputvalues from the field into the structure*/
/*								      */
/**********************************************************************/

 static signed32 d2_cp_to_ttxid(
   D2_ttxid   *ttxid)

{
    strcpy((char *)ttxid->d2_terminal.d2_value,
	    d20_ttxidifields[DS_TT_TERM]);
    ttxid->d2_terminal.d2_size = strlen((char *)ttxid->d2_terminal.d2_value);

    if(d20_ttxidifields[DS_TT_CONTR][0] != DS_DEFAULT_VALUE &&
       d20_ttxidifields[DS_TT_CONTR][0] != '\0') {
	  strcpy((char *)ttxid->d2_controls.d2_value,
		  d20_ttxidifields[DS_TT_CONTR]);
	  ttxid->d2_controls.d2_size =
		  strlen((char *)ttxid->d2_controls.d2_value);
    } else
	  ttxid->d2_controls.d2_size = 0;

    if(d20_ttxidifields[DS_TT_GRAPH][0] != DS_DEFAULT_VALUE &&
       d20_ttxidifields[DS_TT_GRAPH][0] != '\0') {
	  strcpy((char *)ttxid->d2_graphics.d2_value,
		  d20_ttxidifields[DS_TT_GRAPH]);
	  ttxid->d2_graphics.d2_size =
		  strlen((char *)ttxid->d2_graphics.d2_value);
    } else
	  ttxid->d2_graphics.d2_size = 0;

    if(d20_ttxidifields[DS_TT_MISCL][0] != DS_DEFAULT_VALUE &&
       d20_ttxidifields[DS_TT_MISCL][0] != '\0')
	{
	  strcpy((char *)ttxid->d2_miscel.d2_value,
		      d20_ttxidifields[DS_TT_MISCL]);
	  ttxid->d2_miscel.d2_size = strlen((char *)ttxid->d2_miscel.d2_value);
	} else
	  ttxid->d2_miscel.d2_size = 0;

    if(d20_ttxidifields[DS_TT_PAGES][0] != DS_DEFAULT_VALUE &&
       d20_ttxidifields[DS_TT_PAGES][0] != '\0') {
	  strcpy((char *)ttxid->d2_pages.d2_value,
		      d20_ttxidifields[DS_TT_PAGES]);
	  ttxid->d2_pages.d2_size = strlen((char *)ttxid->d2_pages.d2_value);
    } else
	  ttxid->d2_pages.d2_size = 0;

    if(d20_ttxidifields[DS_TT_PRIV][0] != DS_DEFAULT_VALUE &&
       d20_ttxidifields[DS_TT_PRIV][0] != '\0') {
	  strcpy((char *)ttxid->d2_privates.d2_value,
		  d20_ttxidifields[DS_TT_PRIV]);
	  ttxid->d2_privates.d2_size =
		  strlen((char *)ttxid->d2_privates.d2_value);
    } else
	  ttxid->d2_privates.d2_size = 0;
}


 static signed32 d2_cp_to_telex(
   D2_telex_number   *telex)

{
    strcpy((char *)telex->d2_t_number.d2_value, d20_telexifields[DS_TL_TELEX]);
    telex->d2_t_number.d2_size = strlen((char *)telex->d2_t_number.d2_value);

    strcpy((char *)telex->d2_c_code.d2_value, d20_telexifields[DS_TL_CODE]);
    telex->d2_c_code.d2_size = strlen((char *)telex->d2_c_code.d2_value);

    strcpy((char *)telex->d2_answer.d2_value, d20_telexifields[DS_TL_ANSW]);
    telex->d2_answer.d2_size = strlen((char *)telex->d2_answer.d2_value);
}


 static signed32 d2_cp_to_fax(
   D2_faxnumber     *fax)

{
signed32 bit_par ;
signed32 i;

    fax->d2_par_defined = FALSE;
    bit_par = 0;

    for (i=DS_FX_A3W; i <= DS_FX_UNLIML; i++) {
	    if (*((signed32 *) d20_faxifields[i]) != FALSE) {
		     bit_par |= fax_bits[i-1];
		     fax->d2_par_defined = TRUE;
	    }
    }

    fax->d2_par = bit_par;
    strcpy((char *)fax->d2_tel_nr.d2_value, d20_faxifields[DS_FX_NR]);
    fax->d2_tel_nr.d2_size = strlen((char *)fax->d2_tel_nr.d2_value);
}


 static signed32 d2_cp_to_t61(
   D2_T61_seq     *t61)

{
signed32  i, nb;

    for (i=0, nb=0; i < D20_MAX_NB_ITEM; i++)
	  {
	  if ((d20_t61ifields[i][0] != '\0' &&
	       d20_t61ifields[i][0] != DS_DEFAULT_VALUE))
		  {
		  strcpy((char *)t61->d2_t61_item[nb].d2_value,
			      d20_t61ifields[i]);
		  t61->d2_t61_item[nb].d2_size =
			      strlen((char *)t61->d2_t61_item[nb].d2_value);
		  nb++;
		  }
	  }
    t61->d2_nb_item = nb;
}


 static signed32 d2_cp_to_str_list(
   D2_str_list     *str_list)

{
signed32  i;

    for (i=0; i < D20_MAX_NB_ITEM; i++)
	  {
	  if ((d20_t61ifields[i][0] != '\0' &&
	       d20_t61ifields[i][0] != DS_DEFAULT_VALUE))
		  {
		  str_list->d2_next = str_list + 1;
		
		  if (d20_printable == TRUE)
			  str_list->d2_rep = D2_PRINTABLE;
		  else
			  str_list->d2_rep = D2_T61;

		  strcpy((char *)str_list->d2_str.d2_value, d20_t61ifields[i]);
		  str_list->d2_str.d2_size =
			      strlen((char *)str_list->d2_str.d2_value);
		  str_list++;
		  }
	  }

    (str_list - 1)->d2_next = (D2_str_list *)NULL;
}


 static signed32 d2_cp_to_dnscell(
   x500_dns_cell_t  *dns_cell)

{
    strcpy(dns_cell->nsp_uuid, d20_dnscellifields[DS_DCELL_NSPCTS]);

    if(d20_dnscellifields[DS_DCELL_RCTS][0] != DS_DEFAULT_VALUE &&
       d20_dnscellifields[DS_DCELL_RCTS][0] != '\0')
	  strcpy(dns_cell->root_uuid, d20_dnscellifields[DS_DCELL_RCTS]);
    else
	  dns_cell->root_uuid = (char *) NULL;

    if(d20_dnscellifields[DS_DCELL_RNM][0] != DS_DEFAULT_VALUE &&
       d20_dnscellifields[DS_DCELL_RNM][0] != '\0')
	  strcpy(dns_cell->root_name, d20_dnscellifields[DS_DCELL_RNM]);
    else
	  dns_cell->root_name = (char *) NULL;
}


 static signed32 d2_cp_to_dnsrep(
   x500_replica_t  *dns_rep)

{
signed32  i;
char  *cp;
signed16  len;

    dns_rep->type = (char) * ((signed32 *) d20_dnsrepifields[DS_REP_TYP]);
    strcpy(dns_rep->ch_uuid, d20_dnsrepifields[DS_REP_CHCTS]);
    strcpy(dns_rep->ch_name, d20_dnsrepifields[DS_REP_CHNMA]);
    strcat(dns_rep->ch_name, d20_dnsrepifields[DS_REP_CHNMB]);
    strcpy(dns_rep->tower, d20_dnsrepifields[DS_REP_TW1]);
    cp = (char *) dns_rep->tower;
    cp += (strlen(dns_rep->tower) + 1);
    len = (signed16) (strlen(dns_rep->tower) + 1);
    for (i=DS_REP_TW2; i <= DS_REP_TW5; i++)
	    if (d20_dnsrepifields[i][0] != DS_DEFAULT_VALUE &&
		d20_dnsrepifields[i][0] != '\0') {
		  strcpy(cp, d20_dnsrepifields[i]);
		  cp += (strlen(d20_dnsrepifields[i]) + 1);
		  len += (strlen(d20_dnsrepifields[i]) + 1);
	    }
    dns_rep->tower_len = len;
}
