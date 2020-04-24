/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20display.c,v $
 * Revision 1.1.10.2  1996/02/18  19:41:43  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:22:27  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:19:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:52:28  root]
 * 
 * Revision 1.1.8.3  1994/05/10  15:49:58  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:13:44  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:06:45  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:12:38  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:35:22  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:34:39  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  10:38:37  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:36:34  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:10:03  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:06:14  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  14:56:43  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  16:10:01  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  22:05:36  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:46:20  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20display.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:41:43 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* NAME         : d20display.c                                        */
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

extern char      *d20_fd_of_week;

extern char      *d20_F_mints;
extern char      *d20_F_hrs;
extern char      *d20_F_days;
extern char      *d20_F_job_state;
extern Bool	 d20_call_cread;

extern D20_omask  d20_E1omask[];
extern D20_omask  d20_E2omask[];
extern D20_omask  d20_Eaomask[];
extern D20_omask  d20_Ebomask[];
extern D20_omask  d20_Ecomask[];
extern D20_omask  d20_Edomask[];

extern D20_omask  d20_qomask[];
extern char      *d20_errtab[];
extern char      *d20_err_table[];

#ifndef CACHE_ADMIN

/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d20_C03_display_jobs()                                 */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function displays all the shadowing jobs in a loop.      */
/*      Depending on the kind of the update times the display mask    */
/*      has three different layouts.                                  */
/*      If <CR> is entered the last displayed job will be stored.     */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*     bind_id      bind identification                               */
/*     dir_id       directory identifier                              */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*     *disp        a flag indicating whether shadowing job has been  */
/*                  displayed                                         */
/*                                                                    */
/*exoff ***************************************************************/

void d20_C03_display_jobs(
  signed16 bind_id,
  signed16 dir_id,
  Bool * disp)
{
Errcode     ret_code;
signed32    mode = READ_FIRST;
Bool        job_found = FALSE;
Shd_job     act_job;
signed32    fkt;

d20_E1omask[DS_E1_FORWARD].d20_fkt_id = 
d20_Eaomask[DS_EA_FORWARD].d20_fkt_id = 
d20_Ebomask[DS_EB_FORWARD].d20_fkt_id = 
d20_Ecomask[DS_EC_FORWARD].d20_fkt_id = 
d20_Edomask[DS_ED_FORWARD].d20_fkt_id = D20_FKTX;

for (*disp = FALSE ; ; )
	{   

	/* read shadowing job                 */
    	if (d21_20_getjb(bind_id, &ret_code, DS_V021, mode, &act_job) == D2_ERROR)
    		{   
		if (ret_code.errvalue == DS_EOF)
			{   
			if (job_found == FALSE)
	    			{ 
	                  	/* no jobs defined                    */
				d20_qomask[D20_Q1POS].d20_ioinfo = 
						d20_err_table[D20_E19IDX];
				d20_handlemask(d20_qomask);
				break;
	    			}
	    		else
	    			{
				/* last job reached                   */
				d20_qomask[D20_Q1POS].d20_ioinfo = 
						d20_err_table[D20_E20IDX];
				d20_handlemask(d20_qomask);
	    			}
			}
		else
			{
			/* error while reading jobs           */
	    		d20_qomask[D20_Q1POS].d20_ioinfo = 
						d20_err_table[D20_E21IDX];
	    		d20_handlemask(d20_qomask);
	    		break;
			}
    		}
    	else
    		mode = READ_NEXT;

    	job_found = TRUE;


    	/* put object name to mask field      */
    	d20_gen_dnintf(act_job.shd_objname);
    	d20_C08_copy_name(act_job.shd_objname,
			  d20_E1omask[DS_E1_OBJ_NAME].d20_ioinfo,
			  d20_E1omask[DS_E1_OBJ_NAME + 1].d20_ioinfo);

    	/* object interpretation              */
    	*(signed32 *)d20_E1omask[DS_E1_OBJ_INTRP].d20_ioinfo = act_job.shd_subset;

    	/* put DSA name to mask field         */
    	d20_gen_dnintf(act_job.shd_dsa);
    	d20_C08_copy_name(act_job.shd_dsa, d20_E1omask[DS_E1_DSA].d20_ioinfo,
			  d20_E1omask[DS_E1_DSA + 1].d20_ioinfo);

	/* fill job mask.	*/
	d20_C11_fill_job_mask(act_job.shd_state, &(act_job.shd_time));

	/* display job mask and wait for input.	*/
	fkt = d20_C10_display_job_mask(TRUE);
	
	if (fkt == FKTDEL)
		break;

	if (fkt == FKTCR)
		{
		*disp = TRUE;
		break;
		}
	} /* end of display loop.	*/

d20_E1omask[DS_E1_FORWARD].d20_fkt_id = 
d20_Eaomask[DS_EA_FORWARD].d20_fkt_id = 
d20_Ebomask[DS_EB_FORWARD].d20_fkt_id = 
d20_Ecomask[DS_EC_FORWARD].d20_fkt_id = 
d20_Edomask[DS_ED_FORWARD].d20_fkt_id = D20_NO_OP;

				/* close file                         */
(void)d21_20_getjb(bind_id, &ret_code, DS_V021, TERMINATE, &act_job);
}

/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d20_C08_copy_name()                                    */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function copies the given name into the mask fields.     */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      name        = name to be shown in mask                        */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      field1      = first mask field                                */
/*      field2      = second mask field                               */
/*                                                                    */
/*exoff ***************************************************************/

void d20_C08_copy_name(
  char * name,
  char * field1,
  char * field2)
{
signed32 len;


if ((len = strlen(name)) > D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT)
{   strncpy(field1, name, D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT);
    len -= (D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT);
    if (len > D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT)
    {   len = D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT;
    }
    strncpy(field2, name + (D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT), len);
    if (len < D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT)
    	*(field2 + len) = D2_EOS;
}
else
  { strcpy(field1, name);
    if (len < D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT)
    	*(field1 + len) = D2_EOS;

    *field2 = D2_EOS;
  }

} /* end of d20_C08_copy_name */

#endif

/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d20_C10_display_job_mask(mode)                         */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function displays shadowing job mask. Fields of the mask */
/* 	are filled before calling this function.		      */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*	mode	boolean to indicate if handlemask should wait for     */
/*		input or not.					      */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*	function key pressed.					      */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d20_C10_display_job_mask(
  Bool mode)
{
signed32 	fkt;

d20_E1omask[DS_E1_JOB].d20_fkt_id = D20_CSAY;

d20_E2omask[DS_E2_UPD_FREQ].d20_fkt_id = D20_CSAY;

d20_Eaomask[DS_EA_UPD_TIME].d20_fkt_id = D20_CSAY;

d20_Ebomask[DS_EB_HOURS].d20_fkt_id = D20_CSAY;

d20_Ecomask[DS_EC_DOW].d20_fkt_id = D20_CSAY;
d20_Ecomask[DS_EC_HOURS].d20_fkt_id = D20_CSAY;

d20_Edomask[DS_ED_DOW1].d20_fkt_id = D20_CSAY;
d20_Edomask[DS_ED_HOURS1].d20_fkt_id = D20_CSAY;
d20_Edomask[DS_ED_DOW2].d20_fkt_id = D20_CSAY;
d20_Edomask[DS_ED_HOURS2].d20_fkt_id = D20_CSAY;

/* show shadowing job mask                */

    	if (*(signed32 *)d20_E1omask[DS_E1_JOB].d20_ioinfo == NOT_ACTIVE)
		{
		/* display deactivated job.	*/
		d20_call_cread = mode;

		if (d20_call_cread)
			fkt = d20_handlemask (d20_E1omask);
		else
			{
			d20_handlemask (d20_E1omask);
			fkt = FKTDEL;
			}
		}
	else
		{
		/* display activated job.	*/
		d20_call_cread = FALSE;

		/* display basic shadow adm. mask.	*/
		d20_handlemask (d20_E1omask);
	
		/* display update freq. mask.		*/
		d20_handlemask (d20_E2omask);

		if ((*(signed32 *) d20_E2omask[DS_E2_UPD_FREQ].d20_ioinfo == 
							UPD_HIGH) ||
		    (*(signed32 *) d20_E2omask[DS_E2_UPD_FREQ].d20_ioinfo == 
							UPD_MEDIUM))
			{
			/* enable input.	*/
			d20_call_cread = mode;

			/* display update time mask.		*/
			if (d20_call_cread)
				fkt = d20_handlemask (d20_Eaomask);
			else
				{
				d20_handlemask (d20_Eaomask);
				fkt = FKTDEL;
				}
			}
		else
			{
			/* update frequency is low.	*/
			/* display update time mask.	*/
			d20_handlemask (d20_Eaomask);

			/* enable input.	*/
			d20_call_cread = mode;

			if (*(signed32 *) d20_Eaomask[DS_EA_UPD_TIME].
						d20_ioinfo == ALL_TIMES)
				{
				/* every day.	*/	
				if (d20_call_cread)
					fkt = d20_handlemask (d20_Ebomask);
				else
					{
					d20_handlemask (d20_Ebomask);
					fkt = FKTDEL;
					}
				}
			else
				if (*(signed32 *) d20_Eaomask[DS_EA_UPD_TIME].
						d20_ioinfo == UPD_LOW_1)
					{
					/* once a week.	*/

					/* display update time mask.	*/
					if (d20_call_cread)
						fkt = d20_handlemask (
								d20_Ecomask);
					else
						{
						d20_handlemask (d20_Ecomask);
						fkt = FKTDEL;
						}
					}
				else
					{
					/* Twice a week.	*/

					/* display update time mask.	*/
					if (d20_call_cread)
						fkt = d20_handlemask (
								d20_Edomask);
					else
						{
						d20_handlemask (d20_Edomask);
						fkt = FKTDEL;
						}
					}
			} /* end of low frequency job.	*/
		} /* end of diplay active job.	*/

d20_E1omask[DS_E1_JOB].d20_fkt_id = D20_CGET;

d20_E2omask[DS_E2_UPD_FREQ].d20_fkt_id = D20_CGET;

d20_Eaomask[DS_EA_UPD_TIME].d20_fkt_id = D20_CGET;

d20_Ebomask[DS_EB_HOURS].d20_fkt_id = D20_CGET;

d20_Ecomask[DS_EC_DOW].d20_fkt_id = D20_CGET;
d20_Ecomask[DS_EC_HOURS].d20_fkt_id = D20_CGET;

d20_Edomask[DS_ED_DOW1].d20_fkt_id = D20_CGET;
d20_Edomask[DS_ED_HOURS1].d20_fkt_id = D20_CGET;
d20_Edomask[DS_ED_DOW2].d20_fkt_id = D20_CGET;
d20_Edomask[DS_ED_HOURS2].d20_fkt_id = D20_CGET;

d20_call_cread = TRUE;

return(fkt);
}

/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d20_C11_fill_job_mask(state, upd_time)                 */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*	This function fills the job mask before it is displayed.      */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*	signed32	state;	job state.			      */
/*	Upd_time	*upd_time; update time info. of a job.	      */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*                                                                    */
/*                                                                    */
/*exoff ***************************************************************/

void d20_C11_fill_job_mask(
  signed32 state,
  Upd_time * upd_time)
{

d20_E1omask[DS_E1_JOB].d20_oformat = d20_F_job_state;

if ((state & NOT_ACT_JOB) == NOT_ACT_JOB)
	{ 
	/* Job which is not active.	*/
	*(signed32 *)d20_E1omask[DS_E1_JOB].d20_ioinfo = NOT_ACTIVE;
	}
else
	{ 
	/* An active job.	*/
       	*(signed32 *) d20_E1omask[DS_E1_JOB].d20_ioinfo = ACTIVE;

	/* in the mask both UPD_LOW_1 and UPD_LOW_2 are treated as */
	/* update frequency LOW.				   */
	if (upd_time->upd_t_mode == UPD_LOW_2)
		*(signed32 *) d20_E2omask[DS_E2_UPD_FREQ].d20_ioinfo = UPD_LOW_1;
	else
		*(signed32 *) d20_E2omask[DS_E2_UPD_FREQ].d20_ioinfo =
						upd_time->upd_t_mode;

	if ((upd_time->upd_t_mode == UPD_HIGH) || 
	    (upd_time->upd_t_mode == UPD_MEDIUM))
		{ 
		if (upd_time->upd_t_mode == UPD_HIGH)
			{
        		d20_Eaomask[DS_EA_UPD_TIME].d20_oformat = d20_F_mints;
			*(signed32 *) d20_Eaomask[DS_EA_UPD_TIME].d20_ioinfo
						= upd_time->upd_min;
			}
		else
			{
        		d20_Eaomask[DS_EA_UPD_TIME].d20_oformat = d20_F_hrs;
			*(signed32 *) d20_Eaomask[DS_EA_UPD_TIME].d20_ioinfo
						= upd_time->upd_hour;
			}
		}
	else
		{
	       	/* update frequency is low.		*/
        	d20_Eaomask[DS_EA_UPD_TIME].d20_oformat = d20_F_days;

		if (upd_time->upd_t_mode == UPD_LOW_1)
			{
			/* every day.	*/	
			if (upd_time->upd_1d_of_week == ALL_POSSIBLE_TIMES)
				{
	    			*((signed32 *) d20_Eaomask[DS_EA_UPD_TIME].
						d20_ioinfo) = ALL_TIMES;
	   			*((signed32 *)d20_Ebomask[DS_EB_HOURS].
					d20_ioinfo) = upd_time->upd_1hour;
				}
			else
				{
				/* Once a week.		*/	
	    			*((signed32 *) d20_Eaomask[DS_EA_UPD_TIME].
						d20_ioinfo) = UPD_LOW_1;
	   			*((signed32 *)d20_Ecomask[DS_EC_DOW].
					d20_ioinfo) = upd_time->upd_1d_of_week;
	   			*((signed32 *)d20_Ecomask[DS_EC_HOURS].
					d20_ioinfo) = upd_time->upd_1hour;
				}
			}
		else 
			/* mode is UPD_LOW_2 	*/
			{
			/* Twice a week.	*/
	    		*((signed32 *) d20_Eaomask[DS_EA_UPD_TIME].
						d20_ioinfo) = UPD_LOW_2;
	   		*((signed32 *)d20_Edomask[DS_ED_DOW1].d20_ioinfo) =
						upd_time->upd_1d_of_week;
	   		*((signed32 *)d20_Edomask[DS_ED_HOURS1].d20_ioinfo) =
						upd_time->upd_1hour;
	   		*((signed32 *)d20_Edomask[DS_ED_DOW2].d20_ioinfo) =
						upd_time->upd_2d_of_week;
	   		*((signed32 *)d20_Edomask[DS_ED_HOURS2].d20_ioinfo) =
						upd_time->upd_2hour;
			}
		} /* end of low update frequency.	*/
	} /* end of active job.	*/

}
