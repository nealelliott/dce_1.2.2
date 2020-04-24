/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20hdlmsk.c,v $
 * Revision 1.1.10.2  1996/02/18  19:40:56  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:21:44  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:16:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:55  root]
 * 
 * Revision 1.1.8.4  1994/06/21  14:43:37  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  12:09:40  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:49:34  marrek
 * 	Code submission on April 1994.
 * 	[1994/05/03  14:56:33  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:11:14  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:05:22  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:09:18  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:58:33  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:29:24  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  10:07:36  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:29:29  marrek]
 * 
 * Revision 1.1.4.6  1993/02/04  08:39:35  marrek
 * 	help_mask2 removed.
 * 	[1993/02/03  08:39:19  marrek]
 * 
 * Revision 1.1.4.5  1993/01/29  08:35:14  marrek
 * 	clear mask before display of all leading masks
 * 	[1993/01/29  08:21:39  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  18:08:42  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:04:24  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/18  08:28:26  marrek
 * 	Various bug fixes for mask handling found at SNI
 * 	[1992/12/17  16:28:20  marrek]
 * 
 * Revision 1.1.4.2  1992/11/27  13:12:27  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  14:09:33  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:55:32  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:16:09  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20hdlmsk.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:40:56 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton	***************************************************************/
/*								      */
/* TYPE         : MODUL                                               */
/*								      */
/* NAME         : d20hdlmsk.c                                         */
/*								      */
/* AUTHOR       : R. Horn, D AP 11                                    */
/* DATE         : 10.05.88                                            */
/*								      */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOC.-NR.     : Directory-Design-Specification                      */
/*			.					      */
/*			.					      */
/*								      */
/* PRD#/VERS.   :                                                     */
/*								      */
/* DESCRIPTION  :                                                     */
/*								      */
/* SYSTEM DEPENDENCIES: COMMON                                        */
/*								      */
/* HISTORY      :                                                     */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*          |         |                                |    |         */
/*datoff **************************************************************/

#include <gds.h>
#include <stdio.h>
#include <string.h>
#include <nl_types.h>

#include <dce/d2dir.h>
#include <cget.h>
#include <gdsext.h>
#include <d2adm.h>
#include <d2msk.h>
#include <d20proto.h>
#include <d27char.h>
#include <d27conv.h>

#define HELP_LINES	20	/* no of lines in one help mask.	*/
#define HDLMSK_ERRMSG_NO 2	/* no of error messages used by hdlmsk	*/

/* -------------------- declaration of global data ------------------------- */

extern D20_help_array  help_masks1[];  /* array with all (normal) masks */
extern D20_omask  d20helpomask[];

extern D20_omask  d20_frameomask[] ; /* frame mask */
extern D20_omask  d20_qomask[];	     /* error mask */
extern D20_omask  d20_msgomask[];	    
extern signed32   rbatch ;	/* operation flag: 0 = screen input */
				/*                 1 = file input */

char	*d20_hdlmsk_errmsg[HDLMSK_ERRMSG_NO];
				/* Array of error messages used by	*/
				/* handlemask. These should be		*/
				/* initialized with messages from	*/
				/* catalogs by the modules using	*/
				/* handlemask like gdsditadm or		*/
				/* gdssysadm.				*/
nl_catd    nl_help_fd;          /* filepointer for msgfile (helpmasks) */

char *d20_tfile;                /* test output file */
extern char comment[] ;         /* comment written to the test output file */
FILE *fd_out = NULL ;		/* file descriptor used for test output file */
Bool d20_call_cread = TRUE;  /* a flag indicating whether input is to be  */
				/* read from masks (used for help masks)     */

/* -------------------- declaration of local data  ------------------------- */

static	char	d20_t61_str_area[D20_MAX_T61_MASK_FV];	
static	char	d20_loc_str_area[D20_MAX_LOC_MASK_FV];

/* -------------------- declaration of functions   ------------------------- */

 static signed32 d20_handlehelp (D20_omask *p_helpmsk);
 static signed32 d20_disphelpmask(D20_help_array hmasks);

static	signed32	d20_conv_mask_to_local_form(
				D20_omask	*mask_beg_ptr);
static	D20_omask	*d20_conv_mask_to_t61_form(
				D20_omask	*mask_beg_ptr,
				Bool		ignore_conv_err);

/* ------------------------------------------------------------------------- */

/*exon ***********************************************************************/
/*                                                                           */
/*  FUNCTION:  d20_handlemask ()                                             */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The function displays a mask, described by a mask information block, */
/*      at the screen and gets all input informations which are inserted     */
/*      in the mask by a user. If the global variable 'rbatch' is set to     */
/*      one, all mask input informations are read from an input file. In     */
/*      this case only comment informations and error messages are written   */
/*      to an output file, while the mask output informations are lost.      */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      parptr      = Pointer to the information block of the mask which     */
/*                    should be handled.                                     */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*                    None                                                   */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*                    Function number                                        */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                         DATE: 29.1.87     */
/*exoff **********************************************************************/

signed32 d20_handlemask (D20_omask *parptr)

{
D20_omask	*frameptr ;
D20_omask	*mask_beg = parptr ;
signed16	d20_fval, i, d20_fkt_no = 0 ;
Bool		inputrq = FALSE ;
signed32	fkt;
char		emp_error_str[D20_ERR_MSG_LEN + 1] = "                                                                         ";
char		emp_cont_str[D20_CONT_MSG_LEN + 1] = "                        ";
Bool		ignore_conv_err;
D20_omask	*ret_mask_ptr;

struct D20_fkey {
	signed16 d20_keyval ;	/* active function key information */
	char	 *d20_fvalptr ;
	} d20_fkeyinfo [D20_MFKTKEYS] ;

struct D20_fkey	*d20_keyptr = d20_fkeyinfo ;


if ((mask_beg->d20_fkt_id & D20_NOERASE) != D20_NOERASE) 
	{
	/* erase screen */
	cerase () ;

	/* output frame mask */
	frameptr = d20_frameomask ;

	while (frameptr->d20_fkt_id != D20_EOMSK) 
		{
		/* set attribute variable */
		getmode = frameptr->d20_attribute ;

		csay (frameptr->d20_line_no, frameptr->d20_column_no,
		      frameptr->d20_ioinfo, frameptr->d20_oformat) ;

		frameptr++ ;
		}		
	} 

/* convert the given mask to the local string format.	*/
if (d20_conv_mask_to_local_form(mask_beg) == D2_ERROR)
	{
	/* display warning message.	*/
	d20_msgomask[D20_M1POS].d20_ioinfo = d20_hdlmsk_errmsg[0];

	getmode = d20_msgomask[D20_M1POS].d20_attribute ;

	csay (d20_msgomask[D20_M1POS].d20_line_no, 
	      d20_msgomask[D20_M1POS].d20_column_no,
	      d20_msgomask[D20_M1POS].d20_ioinfo, 
	      d20_msgomask[D20_M1POS].d20_oformat) ;
	}

/* loop unless correct local strings (which can be converted to T61) have */
/* been entered.							  */	
while (TRUE)
	{
	parptr = mask_beg;

	while (parptr->d20_fkt_id != D20_EOMSK) 
		{
		/* set attribute variable */
		getmode = parptr->d20_attribute ;

		switch ((signed32) (parptr->d20_fkt_id & ~D20_NOERASE)) 
			{
			case D20_CSAY:
				if (rbatch == 1 && (mask_beg == d20_qomask ||
						    mask_beg == d20_msgomask)) 
					{
					/* write error message into test */
					/* output file 			 */
					if (fd_out != NULL && 
					    parptr->d20_ioinfo != NULL) 
						{
						fprintf (fd_out, "%s\n", 
							 parptr->d20_ioinfo) ;
						fflush (fd_out) ;
						}
					} 
				else 
					{
					/* put mask information to screen */
					csay (parptr->d20_line_no, 
					      parptr->d20_column_no,
					      parptr->d20_ioinfo, 
					      parptr->d20_oformat) ;
					}

				break ;

			case D20_CGET:
				/* field information to screen */
				if (d20_call_cread == TRUE)
					{   
					if (rbatch)
					/* field is integer field.	*/
						if (*(parptr->d20_oformat + 
						    strlen(parptr->d20_oformat) 
						    - 1) == 'd')
							*(signed32 *)(
							parptr->d20_ioinfo) = 0;
						else
							*parptr->d20_ioinfo = 
									D2_EOS;

					cget (parptr->d20_line_no,
					      parptr->d20_column_no,
					      parptr->d20_ioinfo, 
					      parptr->d20_oformat);

					inputrq = TRUE ;
					}
				else
					{   
					csay (parptr->d20_line_no,
					      parptr->d20_column_no,
					      parptr->d20_ioinfo, 
					      parptr->d20_oformat);
					}

				break ;

			case D20_FKTX:
				if (rbatch == 0 && d20_fkt_no < D20_MFKTKEYS) 
					{
					/* activate function key */
					cgetfkt (parptr->d20_attribute) ;
					d20_keyptr->d20_keyval = 
							parptr->d20_attribute;
					d20_keyptr++->d20_fvalptr = 
							parptr->d20_ioinfo;
					d20_fkt_no++ ;
					} 
				else 
					{
					if (mask_beg == d20_qomask ||
					    mask_beg == d20_msgomask) 
						{
						/* error -> skip test input */
						cforts () ;				
						return(FKTNO) ;
						}
					}

				inputrq = TRUE ;
				break ;

			case D20_NO_OP :
			default:
				break ;
			}	/* end of switch.	*/

			parptr++ ;

		}	/* end of scaaning of mask fields.	*/

	/* set attribute variable */
	getmode = parptr->d20_attribute ;

	if (!inputrq || d20_call_cread == FALSE)
		/* no input request */
		d20_fval = FKTNO;
	else
		/* get mask input values */
		d20_fval = cread () ;

	if (rbatch)
		d20_fval = FKTCR;

	/* if mask is sent for execution.	*/
	if (d20_fval == FKTCR || d20_fval == FKTMENU)
		ignore_conv_err = FALSE;
	else
		ignore_conv_err = TRUE;

	/* convert mask to T61 format.	*/
	if ((ret_mask_ptr = d20_conv_mask_to_t61_form(mask_beg, ignore_conv_err)
	    ) != (D20_omask *)NULL)
		{
		/* erase the warning if present.	*/
		getmode = NORMAL;

		csay (d20_msgomask[D20_M1POS].d20_line_no, 
		      d20_msgomask[D20_M1POS].d20_column_no,
		      emp_error_str,
		      d20_msgomask[D20_M1POS].d20_oformat) ;

		/* display the error message, wait for the input */
		/* and continue 				 */
		d20_qomask[D20_Q1POS].d20_ioinfo = d20_hdlmsk_errmsg[1];
		d20_handlemask(d20_qomask);

		getmode = NORMAL;

		/* erase the error message.		*/
		csay (d20_qomask[D20_Q1POS - 1].d20_line_no, 
		      d20_qomask[D20_Q1POS - 1].d20_column_no,
		      emp_error_str, 
		      d20_qomask[D20_Q1POS - 1].d20_oformat) ;

		/* erase To Continue Press <CR> ! 	*/
		csay (d20_qomask[D20_Q1POS + 1].d20_line_no, 
		      d20_qomask[D20_Q1POS + 1].d20_column_no,
		      emp_cont_str, 
		      d20_qomask[D20_Q1POS + 1].d20_oformat) ;
		}
	else
		{
		break;	/* break from correct local strings loop. */
		}

	}	/* end of while loop to enter correct local strings.	*/

/* handle function keys */
for (d20_keyptr = d20_fkeyinfo, i = 0; i < d20_fkt_no; i++, d20_keyptr++) 
	{
	if (d20_fval == FKTHELP) 
		{
		fkt = d20_handlehelp(mask_beg);
		return (fkt);
		}

	if (d20_keyptr->d20_keyval == d20_fval && 
	    d20_keyptr->d20_fvalptr != NULL) 
		*d20_keyptr->d20_fvalptr = D20_MARKFKT ;
	}

/* handle comment information for test output file */
if (rbatch == 1 && fd_out == NULL)
	/* open test output file */
	fd_out = fopen (d20_tfile, "w") ;

if (rbatch == 1 && fd_out != NULL && (int)strlen(comment) > 0) 
	{
	/* write comment information to output file */
	fprintf (fd_out, "%s\n", comment) ;
	fflush (fd_out) ;
	/* clear comment information */
	comment[0] = '\0' ;
	}

return(d20_fval) ;

}



/*exon ***********************************************************************/
/*                                                                           */
/*  FUNCTION:  d20_handlehelp ()                                             */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The function checks wether a help mask exists for the displayed mask */
/*	If no help-mask exists, the same mask will be displayed		     */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      p_helpmsk   = Pointer to the information block of the main mask.     */
/*                    (Mask which was handled before.)                       */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*                    None                                                   */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*                    Number of function key                                 */
/*									     */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                         */
/*			d20_handlemask()			 	     */
/*			d20_disphelpmask()			 	     */
/*									     */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: V.Mueller                                     DATE: 07.08.92     */
/*exoff **********************************************************************/

static signed32 d20_handlehelp (D20_omask *p_helpmsk)
{

signed32 j;

	/* check wether normal mask was displayed */
	for (j=0; help_masks1[j].mask != (D20_omask *) 0; j++) {
		/* check wether a helpmask exists for displayed mask */
		if (p_helpmsk == help_masks1[j].mask) 
			return (d20_disphelpmask(help_masks1[j]));
	}

	return(d20_handlemask(p_helpmsk));

}


/*exon ***********************************************************************/
/*                                                                           */
/*  FUNCTION:  d20_disphelpmask()                                            */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*	The function reads the help-mask from the nlsfile and displays it.   */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      hmasks      = Array which has information about the displayed mask,  */
/*		      the msg_set, number of helpmasks and leading masks     */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*                    None                                                   */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*                    Number of function key                                 */
/*									     */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                         */
/*			d20_handlemask()			 	     */
/*			catgets()				 	     */
/*			d20_save_mask()					     */
/*			d20_restore_mask()				     */
/*									     */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: V.Mueller                                     DATE: 07.08.92     */
/*exoff **********************************************************************/

static signed32 d20_disphelpmask(D20_help_array hmasks)
{

D20_omask	sav_omask[D20_MAX_LEADM][D20_M_SIZE];
signed16	save_fktid;
char		help_array[HELP_LINES][80];
char    	*def_str = "DEFAULT";
char    	*res;
signed32      	i, y, msg_no, msg_set;
signed32      	fkt, nr;
signed32      	nr_mask_back;
Bool      	nls_err = FALSE;

	msg_set = hmasks.msg_set;
	/* get number of masks to read from NLS-file */
	for (y=0; y<hmasks.number; msg_set++, y++) {
		for (i=0; i<HELP_LINES; i++) {
			/* clean helpmask */ 
			d20helpomask[i].d20_ioinfo = help_array[i];
			strcpy(help_array[i], "");
		}
		/* read all messages from NLS-file */
		for (msg_no=1; msg_no < HELP_LINES + 1; msg_no++) {
    			if ((res = catgets(nl_help_fd, msg_set,
				   msg_no,def_str)) == def_str) {
    				fprintf(stderr, 
					"Error while reading NLS-helpfile; msg_set = %d msg_no = %d\n", msg_set, msg_no);
    				fflush(stderr);
				nls_err=TRUE;
				break;
    			}

			strcpy(help_array[msg_no-1], res);
		}
		if (nls_err == TRUE)
			break;
		else
			fkt = d20_handlemask(d20helpomask);

		if (fkt == FKTDEL)
			break;
		if (fkt == FKTSCU) {
			/* if first mask, display same mask again */
			if (msg_set == hmasks.msg_set) {
				nr_mask_back=1;
			} else
				nr_mask_back=2;
			msg_set = msg_set - nr_mask_back;
			y = y - nr_mask_back;
		}
	
	}

	/* change already handled masks to read only */
	for (nr=0; hmasks.n_mask[nr] != (D20_omask *) 0; nr++) {}
	if (nr != 1) {
		for (i=0; i<nr-1; i++) {
			d20_save_mask(hmasks.n_mask[i], sav_omask[i]);
		}
		d20_call_cread = FALSE;
	}


	/* clear mask befor display of all leading masks */
	save_fktid=hmasks.n_mask[0]->d20_fkt_id;
	if ((hmasks.n_mask[0]->d20_fkt_id & D20_NOERASE) == D20_NOERASE) {
		hmasks.n_mask[0]->d20_fkt_id=D20_CSAY;
	}

	for (i=0; hmasks.n_mask[i] != (D20_omask *) 0; i++) {
		if (i == nr-1)
			d20_call_cread = TRUE;
		if (d20_call_cread) {
			fkt = d20_handlemask(hmasks.n_mask[i]);
		} else  {
			d20_handlemask(hmasks.n_mask[i]);
			fkt=FKTDEL;
		}
	}

	/* change already handled masks to same status as before */
	hmasks.n_mask[0]->d20_fkt_id=save_fktid;
	if (nr != 1) {
		for (i=0; i<nr-1; i++) {
			d20_restore_mask(hmasks.n_mask[i], sav_omask[i]);
		}
	}

	return(fkt);

}


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_save_mask(orig_mask, sav_mask)                    */
/*								      */
/* AUTHOR:      Schmid,  D AP 11                                      */
/* DATE:        12.12.89                                              */
/*								      */
/* SYNTAX:      d2_ret_val d20_save_mask(orig_mask, sav_mask)         */
/*              D20_omask   *orig_mask;                               */
/*              D20_omask   *sav_mask;                                */
/*                                                                    */
/* DESCRIPTION: This function saves the mask attributes (D20_NO_OP,   */
/*              D20_FKTX, D20_CSAY, D20_CGET) of the original mask    */
/*              in a save area and will be set to D20_CSAY for input  */
/*              fields so that in display mode, no input is possible. */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              D20_omask   *orig_mask;     mask attributes to be     */
/*                                          saved                     */
/*              D20_omask   *sav_mask;      save area for mask        */
/*                                          attributes                */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              None                                                  */
/*								      */
/* RETURNVALUE:							      */
/*              D2_NOERROR                                            */
/*								      */
/* STANDARD-HEADER: None                                              */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/*exoff ***************************************************************/

d2_ret_val d20_save_mask(D20_omask *orig_mask, D20_omask *sav_mask)
{
    for ( ; orig_mask->d20_fkt_id != D20_EOMSK; orig_mask++, sav_mask++)
    {   sav_mask->d20_fkt_id = orig_mask->d20_fkt_id;
	if ((orig_mask->d20_fkt_id & D20_CGET) == D20_CGET)
	{   orig_mask->d20_fkt_id &= ~D20_CGET;
	    orig_mask->d20_fkt_id |= D20_CSAY;
	}
    }
    sav_mask->d20_fkt_id = D20_EOMSK;

    return(D2_NOERROR);
}


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_restore_mask(orig_mask, sav_mask)                 */
/*								      */
/* AUTHOR:      Schmid,  D AP 11                                      */
/* DATE:        12.12.89                                              */
/*								      */
/* SYNTAX:      d2_ret_val d20_restor_mask(orig_mask, sav_mask)       */
/*              D20_omask   *orig_mask;                               */
/*              D20_omask   *sav_mask;                                */
/*                                                                    */
/* DESCRIPTION: This function restores the mask attributes (D20_NO_OP,*/
/*              D20_FKTX, D20_CSAY, D20_CGET) of the original mask    */
/*              from the save area.                                   */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              D20_omask   *orig_mask;     mask attributes to be     */
/*                                          restored                  */
/*              D20_omask   *sav_mask;      save area for mask        */
/*                                          attributes                */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              None                                                  */
/*								      */
/* RETURNVALUE:							      */
/*              D2_NOERROR                                            */
/*								      */
/* STANDARD-HEADER: None                                              */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/*exoff ***************************************************************/

d2_ret_val d20_restore_mask(D20_omask *orig_mask, D20_omask *sav_mask)
{
    for ( ; sav_mask->d20_fkt_id != D20_EOMSK; sav_mask++, orig_mask++)
    {   orig_mask->d20_fkt_id = sav_mask->d20_fkt_id;
    }

    return(D2_NOERROR);
}



/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name       : d20_conv_mask_to_local_form()             */
/* Description         : convert the given mask to the local       */
/*                       string format.                            */
/* Return Value        : D2_ERROR / D2_NOERROR                     */
/* Author              : Sanjay, March 1993                        */
/*                                                                 */
/*-----------------------------------------------------------------*/
static signed32	d20_conv_mask_to_local_form(
			D20_omask	*mask_beg_ptr)	/*	IN	*/

{
D20_omask	*mask_scan_ptr = mask_beg_ptr;	/* to scan mask fields.	*/
D20_omask	*cont_mask_field_ptr;		/* to scan fields which are */
						/* together or continous.   */

char		*loc_str_scan;			/* pointer to scan local    */
						/* string generated.	    */

unsigned32	loc_str_len;			/* length of generated local*/
						/* string.		    */

unsigned32	rem_loc_str_len, curr_loc_str_len;

signed32	ret_value = D2_NOERROR;		/* return value of the 	    */
						/* function.		    */
signed32	t61_str_len;			/* length of generated t61  */
						/* string.		    */

while (mask_scan_ptr->d20_fkt_id != D20_EOMSK)
	{
	/* continous fields are to be converted to the local format.	*/
	if (((mask_scan_ptr->d20_fkt_id & D20_CSAY) == D20_CSAY ||
	     (mask_scan_ptr->d20_fkt_id & D20_CGET) == D20_CGET) &&
	     mask_scan_ptr->d20_conv == TRUE)
		{
		/* copy continuous fields to t61_area.	*/

		d20_t61_str_area[0] = D2_EOS;
		cont_mask_field_ptr = mask_scan_ptr;

		do
			{
			strcat(d20_t61_str_area, 
					cont_mask_field_ptr->d20_ioinfo);
			}
		while (cont_mask_field_ptr++->d20_next == TRUE);

		/* convert T61 string to local string.	*/
		t61_str_len = strlen(d20_t61_str_area);
		ret_value = d27_t61_to_local_code(	d20_t61_str_area, 
							t61_str_len,
							d20_loc_str_area, 
							&loc_str_len );

		/* copy the local string into continuous mask fields. */

		/* generated local string could have have some dummy  */
		/* characters because some of the T61 chars could not */
		/* be converted to the local string format.	      */

		cont_mask_field_ptr = mask_scan_ptr;
		loc_str_scan = d20_loc_str_area;
		rem_loc_str_len = loc_str_len;

		do
			{
			if (rem_loc_str_len >= cont_mask_field_ptr->d20_iolen)	
				curr_loc_str_len = 
						cont_mask_field_ptr->d20_iolen;
			else
				curr_loc_str_len = rem_loc_str_len;

			strncpy(cont_mask_field_ptr->d20_ioinfo, loc_str_scan, 
				(size_t)curr_loc_str_len);

			cont_mask_field_ptr->d20_ioinfo[curr_loc_str_len] = 
									D2_EOS;

			/* for EOS handling in cget.c module.		    */
			/* required particularly for object name mask which */
			/* is not inititialized sometimes.		    */
			/* similar initialization was done before in 	    */
			/* ds0_512_object_tree().			    */
			cont_mask_field_ptr->d20_ioinfo[
			cont_mask_field_ptr->d20_iolen] = D2_EOS;

			rem_loc_str_len -= curr_loc_str_len;
			loc_str_scan += (loc_str_len - rem_loc_str_len);
			}
		while (cont_mask_field_ptr++->d20_next == TRUE);

		/* next continuous field starts.	*/
		mask_scan_ptr = cont_mask_field_ptr;

		}	/* end of conversion required.	*/
	else
		mask_scan_ptr++;

	}	/* end of scanning mask fields.	*/

return(ret_value);

}



/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name       : d20_conv_mask_to_t61_form()               */
/* Description         :                                           */
/* Return Value        : NULL if no error, else ptr to the field   */
/*                       causing error.                            */
/* Author              : Sanjay                                    */
/* Date                : March 1993                                */
/*                                                                 */
/*-----------------------------------------------------------------*/
static D20_omask	*d20_conv_mask_to_t61_form(
			D20_omask	*mask_beg_ptr,	/*	IN	*/
			Bool		ignore_conv_err)/*	IN	*/

{
static char	*d20_t61_str_area_ptr;
D20_omask	*mask_scan_ptr = mask_beg_ptr;	/* to scan mask fields.	*/
D20_omask	*cont_mask_field_ptr;		/* to scan fields which are */
						/* together or continous.   */

char		*t61_str_scan;			/* pointer to scan t61      */
						/* string generated.	    */

unsigned32	t61_str_len;			/* length of generated t61  */
						/* string.		    */

signed32	rem_t61_str_len, curr_t61_str_len;

D20_omask	*ret_mask_ptr = (D20_omask *)NULL;
					/* return value of the function	*/
					/* NULL if conversion successful*/
					/* otherwise pointing to the 	*/
					/* field which caused error.	*/

signed16	save_fkt_id;

signed32	t61_field_len;



/* Initialization */
d20_t61_str_area_ptr = d20_t61_str_area;

/* Processing */
while (mask_scan_ptr->d20_fkt_id != D20_EOMSK)
	{
	/* continous fields are to be converted to the T61 format.	*/
	if (((mask_scan_ptr->d20_fkt_id & D20_CSAY) == D20_CSAY ||
	     (mask_scan_ptr->d20_fkt_id & D20_CGET) == D20_CGET) &&
	     mask_scan_ptr->d20_conv == TRUE)
		{
		/* copy continuous fields to the local string area.	*/

		d20_loc_str_area[0] = D2_EOS;
		cont_mask_field_ptr = mask_scan_ptr;

		do
			{
			strcat(d20_loc_str_area, 
					cont_mask_field_ptr->d20_ioinfo);
			}
		while (cont_mask_field_ptr++->d20_next == TRUE);

		/* convert local string to T61 string.	*/

		/* error in the input local string.	*/
		t61_str_len = 1; /* != 0 -> Memory will not be allocated */
		if (d27_local_to_t61(	d20_loc_str_area,
					strlen(d20_loc_str_area),
					&d20_t61_str_area_ptr,
					&t61_str_len) == D2_ERROR
				    && ignore_conv_err == FALSE )
			{
			/* convert already converted local strings */
			/* back to local string format.		   */

			/* not the first continous field conversion 	*/
			/* has failed.					*/
			if (mask_scan_ptr != mask_beg_ptr)
				{
				/* save function id. of the last field not to */
				/* converted back to local string.	      */

				save_fkt_id = mask_scan_ptr->d20_fkt_id;

				mask_scan_ptr->d20_fkt_id = D20_EOMSK;

				if (d20_conv_mask_to_local_form(mask_beg_ptr) 
								== D2_ERROR)
					{
					/* this should never happen, since we */
					/* have converted from local to T61   */
					/* and therefore we should be able to */
					/* convert back.	     	      */

					/* anyway the fields which could not  */
					/* be converted are made blank.	      */
					}
				
				mask_scan_ptr->d20_fkt_id = save_fkt_id;
				}

			ret_mask_ptr = mask_scan_ptr;

			break;	/* break from while loop to scan mask fields. */
			}
		else
			{
			/* Local char sequence converted successfully to T61 */
			/* string format or ignore_conv_err is TRUE.  	     */

			/* copy the T61  string into continuous mask fields. */

			cont_mask_field_ptr = mask_scan_ptr;
			t61_str_scan = d20_t61_str_area;
			rem_t61_str_len = t61_str_len;

			do
				{
				t61_field_len = cont_mask_field_ptr->d20_iolen 
						* D20_T61_CONV_FACT;

				if (rem_t61_str_len >= t61_field_len)
					curr_t61_str_len = t61_field_len;
				else
					curr_t61_str_len = rem_t61_str_len;

				strncpy(cont_mask_field_ptr->d20_ioinfo,
				t61_str_scan, curr_t61_str_len);

				cont_mask_field_ptr->d20_ioinfo[
						curr_t61_str_len] = D2_EOS;

				rem_t61_str_len -= curr_t61_str_len;
				t61_str_scan += (t61_str_len - rem_t61_str_len);
				}
			while (cont_mask_field_ptr++->d20_next == TRUE);

			}

		/* next continuous field starts.	*/
		mask_scan_ptr = cont_mask_field_ptr;

		}	/* end of conversion required.	*/
	else
		mask_scan_ptr++;

	}	/* end of scanning mask fields.	*/

return(ret_mask_ptr);

}
