/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20cupd.c,v $
 * Revision 1.1.10.2  1996/02/18  19:41:38  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:22:21  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:18:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:52:23  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:06:41  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:12:19  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:35:17  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:33:49  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  10:34:02  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:35:48  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:09:51  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:05:56  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  14:55:14  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  16:08:48  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  22:03:35  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:45:51  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20cupd.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:41:38 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* NAME         : d20cupd.c                                           */
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

extern char       *d20_F_act_mod;
extern char       *d20_F_notact_mod;

extern char      *d20_err_table[];
extern D20_omask  d20_E1omask[];
extern D20_omask  d20_qomask[];

/* LOCAL FUNCTIONS */

static d2_ret_val d20_C02_update_cache_job( D20_pE  * );

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_D_cache_update()                                  */
/*								      */
/* AUTHOR:      Schmid    (D AP 11)                                   */
/* DATE:        05.01.89                                              */
/*								      */
/* DESCRIPTION:                                                       */
/*      This function displays the mask to enter the shadowing        */
/*      administration functions.                                     */
/*      The selected shadowing administration function will be        */
/*      performed.                                                    */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              D20_pE      *parblk;                                  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*								      */
/*exoff ***************************************************************/

signed16 d20_D_cache_update(
  D20_pE * parblk)
{                               /*  d20_D_cache_update                */
Errcode     ret_code;
Shd_job     job;

				/*  display CACHE_JOB                 */
/* read shadowing job                 */
if (d21d027_getjb(parblk->d20_Eenv, &ret_code, READ_FIRST,
		      parblk->d20_Edir_id, &job) == D2_ERROR)
	{   
	if (ret_code.errvalue == DS_EOF)
		{   
		/* no job defined                    */
		d20_qomask[D20_Q1POS].d20_ioinfo = d20_err_table[ D20_E19IDX];
		d20_handlemask(d20_qomask);
		}
	else
		{
		/* error while reading job           */
		d20_qomask[D20_Q1POS].d20_ioinfo = d20_err_table[D20_E21IDX];
		d20_handlemask(d20_qomask);
       		}
	}
else
	{

	/* disable object name, object interpretation, and target dsa name */
	/* fields.							   */
   	d20_E1omask[DS_E1_OBJ_NAME - 1].d20_fkt_id =
   	d20_E1omask[DS_E1_OBJ_NAME].d20_fkt_id =
   	d20_E1omask[DS_E1_OBJ_NAME + 1].d20_fkt_id = D20_NO_OP;

   	d20_E1omask[DS_E1_OBJ_INTRP -1].d20_fkt_id = 
   	d20_E1omask[DS_E1_OBJ_INTRP].d20_fkt_id = D20_NO_OP;

   	d20_E1omask[DS_E1_DSA - 1].d20_fkt_id =
   	d20_E1omask[DS_E1_DSA].d20_fkt_id = 
   	d20_E1omask[DS_E1_DSA + 1].d20_fkt_id = D20_NO_OP;

   	/* fill job mask.	*/
   	d20_C11_fill_job_mask(job.shd_state, &(job.shd_time));

   	/* update cache shadowing job     */
	d20_C02_update_cache_job(parblk);

   	}  /* end of display  */

d20_E1omask[DS_E1_OBJ_NAME - 1].d20_fkt_id = 
d20_E1omask[DS_E1_OBJ_NAME].d20_fkt_id = 
d20_E1omask[DS_E1_OBJ_NAME + 1].d20_fkt_id = D20_CSAY;

d20_E1omask[DS_E1_OBJ_INTRP -1].d20_fkt_id = 
d20_E1omask[DS_E1_OBJ_INTRP].d20_fkt_id = D20_CSAY;

d20_E1omask[DS_E1_DSA - 1].d20_fkt_id = 
d20_E1omask[DS_E1_DSA].d20_fkt_id = 
d20_E1omask[DS_E1_DSA + 1].d20_fkt_id = D20_CSAY;

d21d027_getjb(parblk->d20_Eenv, &ret_code, TERMINATE,
	      parblk->d20_Edir_id, &job);

return(D2_NOERROR);

}                               /*  d20_D_cache_update                */


/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d20_C02_update_cache_job()                             */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function will update a cache shadowing job:              */
/*          either it was marked using DISPLAY-SHADOWING-JOBS         */
/*          or it has to be entered using several masks               */
/*             (object/subtree )                                      */
/*      The selected shadowing job will be updated:                   */
/*      The kind of the new update time registration and afterwards   */
/*      the update times themselves have to be entered.               */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      parblk      contains ODT and ADT                              */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*                    None                                            */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_NOERROR  = The function has been performed successfully.   */
/*      D2_ERROR    = function fails                                  */
/*                                                                    */
/*exoff ***************************************************************/

static d2_ret_val d20_C02_update_cache_job(
  D20_pE  * parblk)
{
Errcode     ret_code;
signed32    upd_mode;
Upd_time    update_time;
Upd_time   *p_time;
signed32    fkt;

/* display job 			*/
/* don't wait for input.	*/
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

upd_mode = * (signed32 *) d20_E1omask[DS_E1_JOB].d20_ioinfo;
    
if ((upd_mode == ACTIVATE) || (upd_mode == CHANGE_UPD_TIME))
	{
	/* get update times                   */
	if (upd_mode == ACTIVATE)
		{
		/* call get_time with initialization flag as TRUE	*/
    		if (d20_C07_get_time(&update_time,TRUE) == D2_ERROR)
    			return(D2_NOERROR);
		}
	else
		{
		/* call get_time with initialization flag as FALSE	*/
    		if (d20_C07_get_time(&update_time,FALSE) == D2_ERROR)
    			return(D2_NOERROR);
		}

    	p_time = &update_time;
	}
else
	p_time = NULL;

      
				/* update shadowing job               */
if (d21_025_update(parblk->d20_Edir_id, &ret_code, DS_V021, upd_mode,
		   p_time) == D2_ERROR)
   	{
   	switch ((int) ret_code.errvalue)
      		{
      		case DS_NFOUND:
           		d20_qomask[D20_Q1POS].d20_ioinfo = 
						d20_err_table[D20_E00IDX];
         		break;

      		case DS_INVMODE:
           		d20_qomask[D20_Q1POS].d20_ioinfo = 
						d20_err_table[D20_E14IDX];
           		break;
      		default:
           		d20_qomask[D20_Q1POS].d20_ioinfo = 
						d20_err_table[D20_E11IDX];
           		break;
      		}

   	d20_handlemask(d20_qomask);
   	return(D2_ERROR);
   	}

return(D2_NOERROR);
}  /* end of d20_C02_update_cache_job */
