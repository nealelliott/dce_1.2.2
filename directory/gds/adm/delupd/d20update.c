/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20update.c,v $
 * Revision 1.1.10.2  1996/02/18  19:41:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:22:33  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:19:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:52:32  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:06:48  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:12:51  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:35:27  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:35:20  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  10:42:32  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:37:26  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:10:14  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:06:30  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  14:57:37  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  16:11:01  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  19:59:32  melman
 * 	New GDS merged drop
 * 	[1992/05/28  20:42:59  melman]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20update.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:41:48 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* NAME         : d20update.c                                         */
/*								      */
/* AUTHOR       : Schmid ,D AP 11                                     */
/* DATE         : 05.01.89                                            */
/*								      */
/*datoff **************************************************************/

#include <gds.h>
#include <stdio.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <gdsext.h>
#include <d2adm.h>
#include <d2msk.h>
#include <d2delupd.h>
#include <cget.h>
#include <d20proto.h>

/**********************************************************************/
/*								      */
/*		   D E C L A R A T I O N S       DATA		      */
/*								      */
/**********************************************************************/

extern char      *d20_E11format;
extern char      *d20_F_act_mod;
extern char      *d20_F_notact_mod;

extern D20_omask  d20_E1omask[];
extern D20_omask  d20_E2omask[];
extern D20_omask  d20_Eaomask[];
extern D20_omask  d20_Ebomask[];

extern D20_omask  d20_qomask[];
extern char      *d20_errtab[];
extern char      *d20_err_table[];

extern Bool       d20_call_cread;

/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d20_C02_update_job()                                   */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function will update a shadowing job:                    */
/*          either it was marked using DISPLAY-SHADOWING-JOBS         */
/*          or it has to be entered using several masks               */
/*             (object/subtree and DSA)                               */
/*      The selected shadowing job will be updated:                   */
/*      The kind of the new update time registration and afterwards   */
/*      the update times themselves have to be entered.               */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      parblk      contains ODT and ADT                              */
/*      disp        a flag indicating whether shadowing job has been  */
/*                  displayed                                         */
/*                                                                    */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*                    None                                            */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_NOERROR  = The function has been performed successfully.   */
/*      D2_ERROR    = function fails                                  */
/*                                                                    */
/*exoff ***************************************************************/

d2_ret_val d20_C02_update_job(
  D20_pE * parblk,
  Bool disp)
{
char        dsa[D2_DNL_MAX + 1];/* distinguished name of DSA          */
char        object[D2_DNL_MAX + 1]; /* object name                    */
char        display_dsa[D2_DNL_MAX + 1];/* display format of dsa name */
char        display_object[D2_DNL_MAX + 1]; /* display format of object name */
Name_string       p_obj;
Errcode     ret_code;
signed32    subset;
signed32    upd_mode;
Upd_time    update_time;
Upd_time   *p_time;
signed32    fkt;
Bool	    init;


if (disp == TRUE)
	{
	/* display job again                  */
	/* ignore the value returned.	*/
	d20_C10_display_job_mask(FALSE);

	/* display basic mask again without overwriting the previous mask.*/
    	d20_E1omask[0].d20_fkt_id |= D20_NOERASE;
	if (*(signed32 *)d20_E1omask[DS_E1_JOB].d20_ioinfo == ACTIVE)
		d20_E1omask[DS_E1_JOB].d20_oformat = d20_F_act_mod;
	else
		d20_E1omask[DS_E1_JOB].d20_oformat = d20_F_notact_mod;

	fkt = d20_handlemask (d20_E1omask);

    	d20_E1omask[0].d20_fkt_id = D20_CSAY;

    	/* break was selected                 */
       	if (fkt == FKTDEL)
    		return(D2_NOERROR);

	/* job is not to be updated.	*/
	if ((*(signed32 *)d20_E1omask[DS_E1_JOB].d20_ioinfo == ACTIVE) ||
	    (*(signed32 *)d20_E1omask[DS_E1_JOB].d20_ioinfo == NOT_ACTIVE))
		return(D2_NOERROR);

	
	/* build object/subtree name          */
    	strcpy(object, d20_E1omask[DS_E1_OBJ_NAME].d20_ioinfo);
    	if (*d20_E1omask[DS_E1_OBJ_NAME + 1].d20_ioinfo != D2_EOS)
    		strcat(object, d20_E1omask[DS_E1_OBJ_NAME + 1].d20_ioinfo);

	ds0_508_check_name(object);
    	p_obj = (strlen(object) == 0) ? NULL : (Name_string)object;

    	subset = *(signed32 *) d20_E1omask[DS_E1_OBJ_INTRP].d20_ioinfo;

	/* build DSA-name                     */
    	strcpy(dsa + 1, d20_E1omask[DS_E1_DSA].d20_ioinfo);
    	if (*d20_E1omask[DS_E1_DSA + 1].d20_ioinfo != D2_EOS)
    		strcat(dsa + 1, d20_E1omask[DS_E1_DSA + 1].d20_ioinfo);

	ds0_508_check_name(dsa + 1);
	}
else

	{
	/* get shadowing job                  */

	/* read object                        */
    	if (d20_C05_read_obj(parblk, TRUE, &p_obj, &subset) == D2_ERROR)
    		return(D2_NOERROR);

	strcpy(display_object, (char *)p_obj);
	d20_gen_dnintf(display_object);

    	/* set the OBJECT NAME fields in the mask.	*/
    	d20_C08_copy_name(display_object,d20_E1omask[DS_E1_OBJ_NAME].d20_ioinfo,
		  d20_E1omask[DS_E1_OBJ_NAME + 1].d20_ioinfo);

    	/* set the SUBSET field in the mask.	*/
    	*(signed32 *) d20_E1omask[DS_E1_OBJ_INTRP].d20_ioinfo = subset;

	/* get destination DSAs               */
    	/* error on reading DSA               */
	if (d20_C06_read_dsa(parblk->d20_Eenv, dsa) == D2_ERROR)
    		return(D2_NOERROR);

	strcpy(display_dsa, dsa + 1);
	d20_gen_dnintf(display_dsa);

    	/* set the TARGET DSA fields in the mask.	*/
    	d20_C08_copy_name(display_dsa,d20_E1omask[DS_E1_DSA].d20_ioinfo,
		  d20_E1omask[DS_E1_DSA + 1].d20_ioinfo);

				/* get job                            */
    	d20_E1omask[DS_E1_JOB].d20_oformat = d20_E11format;
    	*(signed32 *) d20_E1omask[DS_E1_JOB].d20_ioinfo = ACTIVATE;

       	fkt = d20_handlemask (d20_E1omask);

    	/* break was selected                 */
       	if (fkt == FKTDEL)
    		return(D2_NOERROR);
	}

upd_mode = * (signed32 *) d20_E1omask[DS_E1_JOB].d20_ioinfo;
    
if ((upd_mode == ACTIVATE) || (upd_mode == CHANGE_UPD_TIME))
	{
	/* get update times                   */
	if (disp == TRUE)
		{
		/* set initialization flag depending on the upd_mode.	*/
		if (upd_mode == ACTIVATE)
			init = TRUE;
		else
			init = FALSE;
		}
	else
		/* set initialization flag to D2_TRUE.	*/
		init = TRUE;

    	if (d20_C07_get_time(&update_time,init) == D2_ERROR)
    		return(D2_NOERROR);

    	p_time = &update_time;
	}
else
	p_time = NULL;

				/* update shadowing job               */
if (d21_19_updat(parblk->d20_Ebid, &ret_code, DS_V021, upd_mode, p_obj, subset,
	     (Name_string)(dsa + 1), p_time) == D2_ERROR)
{   switch((int) ret_code.errvalue)
    {   case DS_NFOUND:     d20_qomask[D20_Q1POS].d20_ioinfo =
						   d20_err_table[D20_E00IDX];
			    break;
	case DS_INVMODE:    d20_qomask[D20_Q1POS].d20_ioinfo =
						   d20_err_table[D20_E14IDX];
			    break;
	case DS_CRON_ERR:   d20_qomask[D20_Q1POS].d20_ioinfo =
						  d20_errtab[D20_101IDX];
			    break;
	default:            d20_qomask[D20_Q1POS].d20_ioinfo =
						   d20_err_table[D20_E11IDX];
			    break;
    }
    d20_handlemask(d20_qomask);
    return(D2_ERROR);
}

return(D2_NOERROR);
}
