/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20trans.c,v $
 * Revision 1.1.10.2  1996/02/18  19:41:31  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:22:13  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:18:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:52:16  root]
 * 
 * Revision 1.1.8.3  1994/06/21  14:43:44  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:24:42  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:06:38  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:12:02  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:59:04  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:33:04  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  10:27:37  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:34:25  marrek]
 * 
 * Revision 1.1.4.5  1992/12/31  18:09:30  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:05:28  bbelch]
 * 
 * Revision 1.1.4.4  1992/12/04  18:01:35  marrek
 * 	November 1992 code drop
 * 	Replace cmx.h by cmx_addr.h
 * 	[1992/12/04  10:36:44  marrek]
 * 
 * Revision 1.1.4.3  1992/11/27  13:19:49  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  14:14:46  marrek]
 * 
 * Revision 1.1.4.2  1992/09/28  11:37:11  marrek
 * 	Insert check for *p and *(p+1) at NSAP-address.
 * 	[1992/09/28  11:35:18  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  22:02:19  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:17:29  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20trans.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:41:31 $";
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
/* NAME         : trans.c     [handling of PSAP address]              */
/*								      */
/* AUTHOR       : Volpers,D AP 11                                     */
/* DATE         : 30.10.87                                            */
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
/*      0.1 | 30.10.87| Erstellung                     | hv |         */
/*	    |  1.08.90| neue Adressformate             | RH |	      */
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
#include <sys/types.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <gdsext.h>
#include <d2adm.h>
#include <d2msk.h>
#include <osiaddr.h>
#include <dce/cmx_addr.h>
#include <cget.h>
#include <d20proto.h>

/**********************************************************************/
/*								      */
/*			  D E F	I N E S				      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*		   D E C L A R A T I O N S       DATA		      */
/*								      */
/**********************************************************************/

extern D20_omask d20_7aomask[];         /* Input fields for mask 7a    */
extern char      *d20_7aifields[];      /* Mask 7a                     */

extern D20_omask d20_qomask[];         /* Input fields for error mask */
extern char      *d20_errtab[];        /* Error mask                  */
extern char      *d20_err_table[];     /* Error mask                  */

					/* Flag: macro information loaded */
static Bool	initialized = FALSE ;
static char	psap_addr[T_PSAPSIZE] ;	/* PSAP-address buffer */
static signed32	fldsize[DS_SP_F1+1] ;	/* size of mask input/output fields */

					/* Error index table */
static signed32	eindextab[] = {
	D2P_S012_ERR,		D20_E28IDX, D2P_S013_ERR,	D20_E30IDX,
	D2P_S014_ERR,		D20_E31IDX, D2P_S015_ERR,	D20_E33IDX,
	D2P_S022_ERR,		D20_E33IDX, D2P_P032_ERR,	D20_E22IDX,
	D2P_P033_ERR,		D20_E24IDX, D2P_P034_ERR,	D20_E26IDX,
	D2P_P035_ERR,		D20_E22IDX, D2P_S042_ERR,	D20_E37IDX,
	D2P_S044_ERR,		D20_E37IDX, D2P_S045_ERR,	D20_E37IDX,
	D2P_S032_ERR,		D20_E33IDX, D2P_S043_ERR,	D20_E37IDX,
	D2P_S052_ERR,		D20_E26IDX, D2P_S053_ERR,	D20_E26IDX,
	D2P_S062_ERR,		D20_E26IDX, D2P_S063_ERR,	D20_E26IDX,
	UNKNOWN_AFI_ERR,	D20_E34IDX,
	UNSUPPORTED_SYNTAX_ERR,	D20_E22IDX,
	UNKNOWN_SYNTAX_ERR,	D20_E22IDX,
	UNKNOWN_SELSYNTAX_ERR,	D20_E36IDX,
	IDI_SIZE_ERR,		D20_E24IDX, IDI_SYNTAX_ERR,	D20_E22IDX,
	DSP_NPARTS_ERR,		D20_E22IDX, DSP_SIZE_ERR,	D20_E24IDX,
	DSP_SYNTAX_ERR,		D20_E22IDX, SEL_SIZE_ERR,	D20_E35IDX,
	SEL_SIZE_ERR-1,		D20_E35IDX, SEL_SIZE_ERR-2,	D20_E35IDX,
	SEL_SYNTAX_ERR,		D20_E36IDX, SEL_SYNTAX_ERR-1,	D20_E36IDX,
	SEL_SYNTAX_ERR-2,	D20_E36IDX, NO_NSAP_ERR,	D20_E22IDX,
	NSAP_SIZE_ERR,		D20_E24IDX, PSAP_FORMAT_ERR,	D20_E27IDX,
	WRONG_AFI_ERR,		D20_E22IDX, NSAP_FORMAT_ERR,	D20_E22IDX,
	0,			0 } ;

/**********************************************************************/
/*								      */
/*		  D E C L A R A T I O N S      FUNCTIONS	      */
/*								      */
/**********************************************************************/


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : ds0_520_handle_spec_attr                              */
/*								      */
/* AUTHOR:      Volpers   (D AP 11)                                   */
/* DATE:        30.10.87                                              */
/*								      */
/* SYNTAX:      signed32 ds0_520_handle_spec_attr(operation,prty)     */
/*                                                                    */
/*              char            operation;                            */
/*              Ds_property     *ptry;                                */
/*								      */
/* DESCRIPTION: This function handles mask 7a                         */
/*              for the PSAP-Address.                                 */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              char            operation;                            */
/*              Ds_property     *prty;                                */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*								      */
/*	D2_ERROR	when error occurs.			      */
/*	key value 	otherwise.				      */
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

 signed32 ds0_520_handle_spec_attr(
   signed16     operation,
   Ds_property *prty)

{                               /*  ds0_520_handle_spec_attr          */

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

   signed16	i, plen, no_addr, no_efld, result;
   char	*p ;
   signed32	*iptr ;
   signed32	fkt;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

    if (!initialized) {
	/* determine size of variable mask fields */
	for (i = 0; i <= DS_SP_F1; i++)
	    fldsize[i] = strlen (d20_7aifields[i]) ;

	/* set 'initialization done'-flag */
	initialized = TRUE ;
    }

                                  /*  operation = modify              */
    if (operation == DS_MODIFY_ATTRIBUTE || operation == DS_DISP_OBJ) {
                                  /*  initialize mask fields          */
	for (i = 0; i <= DS_SP_F1; i++)
	    memset (d20_7aifields[i], D2_EOS, fldsize[i]) ;

	if ((result = d2p_d01_bin_to_str_psap (prty->ds_value,
					       d20_7aifields[DS_SP_PSEL],
					       d20_7aifields[DS_SP_SSEL],
					       d20_7aifields[DS_SP_TSEL],
					       DS_SP_NET5 - DS_SP_NET1 + 1,
					       &d20_7aifields[DS_SP_NET1],
					       (signed32 *)
						   (&fldsize[DS_SP_NET1])
					       )) < 0) {
	    DBGPRINT ((DEV, "Output PSAP-handling error (%d)\n", result)) ;
	    for (iptr = eindextab; *iptr != 0; iptr += 2)
		if (*iptr == result) break ;
	    d20_qomask[D20_Q1POS].d20_ioinfo =
	    d20_errtab[(*iptr != 0) ? *(iptr+1) : D20_E39IDX];
	    d20_handlemask(d20_qomask);
	    return (D2_ERROR) ;
	}
    }

                             /*  get input from mask            */
    for (;;) {
	fkt = d20_handlemask(d20_7aomask);

	                 /*  "break" or SCROLL_UP or F1 was    */
			 /*  selected                          */
	if ((fkt == FKTDEL)||(fkt == FKTSCU)||(fkt == FKTF1))
		return (fkt) ;

	if (operation == DS_DISP_OBJ)
		return (fkt) ;

			  /*  evaluate input fields */
			  /*  and generate PSAP-address */
	 for (i = DS_SP_NET1, no_addr = no_efld = 0; i <= DS_SP_NET5; i++) {
		if (*d20_7aifields[i] != DS_DEFAULT_VALUE &&
						*d20_7aifields[i] != D2_EOS) {
		    if (no_efld > 0)
			break ;
		    no_addr++ ;
		} else
		    no_efld++ ;
	 }
	 if (i <= DS_SP_NET5) {
		DBGPRINT ((DEV, "Empty net-address input field detected\n")) ;
	    	d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab[D20_E39IDX];
	    	d20_handlemask(d20_qomask);
		continue ;
	 }
	 if ((plen = d2p_p01_str_to_bin_psap (d20_7aifields[DS_SP_PSEL],
					      d20_7aifields[DS_SP_SSEL],
					      d20_7aifields[DS_SP_TSEL],
					      no_addr,
					      &d20_7aifields[DS_SP_NET1],
					      (byte *)psap_addr)) < 0) {
		DBGPRINT ((DEV, "Input PSAP-handling error (%d)\n", plen)) ;
		for (iptr = eindextab; *iptr != 0; iptr += 2)
		    if (*iptr == plen) break ;
	    	d20_qomask[D20_Q1POS].d20_ioinfo =
			      d20_errtab[(*iptr != 0) ? *(iptr+1) : D20_E39IDX];
	    	d20_handlemask(d20_qomask);
	 } else {
		prty->ds_length = plen ;
		prty->ds_value  = (Octet_string) psap_addr ;
		DBGPRINT ((DEV, "ENCODED PSAP-ADDRESS:\n\t")) ;
		for (i = 1, p = psap_addr; i <= plen; i++, p++) {
		    DBGPRINTC ((DEV, "%02x ", *p & 0xFF)) ;
		    if ((i % 16) == 0) {
			DBGPRINTC ((DEV, "\n\t")) ;
		    }
		}
		DBGPRINTC ((DEV, "\n")) ;
		return (fkt) ;
	 }
    }
}  
