/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20delutil.c,v $
 * Revision 1.1.10.2  1996/02/18  19:41:42  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:22:25  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:19:04  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:52:27  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:06:44  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:12:34  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:35:21  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:34:26  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  10:37:30  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:36:17  marrek]
 * 
 * Revision 1.1.4.4  1993/01/29  08:38:35  marrek
 * 	180_get_dsa calling parameters changed.
 * 	[1993/01/28  10:42:21  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:10:00  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:06:10  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  14:56:22  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  16:09:42  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  22:05:25  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:46:13  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20delutil.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:41:42 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* NAME         : d20delutil.o                                        */
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
#include <cget.h>
#include <d2delupd.h>
#include <d20proto.h>

/**********************************************************************/
/*								      */
/*		   D E C L A R A T I O N S       DATA		      */
/*								      */
/**********************************************************************/

extern D20_omask  d20_E2omask[];
extern D20_omask  d20_Eaomask[];
extern D20_omask  d20_Ebomask[];
extern D20_omask  d20_Ecomask[];
extern D20_omask  d20_Edomask[];
extern D20_omask  d20_5omask[];
extern D20_omask  d20_6omask[];

extern D20_omask  d20_qomask[];
extern char      *d20_errtab[];

extern char      *d20_object;
extern char      *d20_F_mints;
extern char      *d20_F_hrs;
extern char      *d20_F_days;

extern Bool       d20_call_cread;
extern signed32   d20_old_rule_nr;
			 	/* structure rule number of object that has   */
				/* been selected before               	      */

#ifndef CACHE_ADMIN

/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d20_C05_read_obj(parblk, inp_allowed, object, subset)  */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function reads the object type and the appropriate       */
/*      RDNs from the screen and builds the object/subtree name.      */
/*      (--> *d20_object)                                             */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      parblk      contains D20_srt, D20_oct, and D20_at.	      */
/*      inp_allowed indicates whether input is allowed in the field   */
/*                  SINGLE OBJECT / OBJECT AND SUBORDINATES           */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      object      = pointer to object name                          */
/*                    (== NULL, if ROOT was selected)                 */
/*      subset      = object interpretation                           */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_NORROR  = The function has been performed successfully.   */
/*      D2_ERROR   = <DEL> has been selected.                        */
/*                                                                    */
/*exoff ***************************************************************/

d2_ret_val d20_C05_read_obj(
  D20_pE * parblk,
  Bool inp_allowed,
  Name_string * object,
  signed32 * subset)
{
signed32 rule_nr;               /* structure rule number of selected object  */
Bool     init_flds;             /* sign indicating whether the mask   */
				/* fields must be initialized         */

char    tree[DS_TREE_LENGTH];   /* tree structure of selected object  */

signed16   help_count;
signed16 ret_value;
D20_oct		*oct_ent;	/*current object class entry */

				/* show tree structure                */
d20_5omask[D20_5ROOT_POS].d20_fkt_id = D20_CSAY;
ret_value = ds0_500_objects(TRUE, D20_OBJECT, &rule_nr, tree);
d20_5omask[D20_5ROOT_POS].d20_fkt_id = D20_NO_OP;

if (ret_value != D2_NOERROR)
				/* break was selected                 */
{   return(D2_ERROR);
}

if (rule_nr != 0)
				/* Root was not selected              */
{   init_flds = (rule_nr == d20_old_rule_nr ) ? FALSE : TRUE;
    d20_old_rule_nr = rule_nr;

    d20_6omask[D20_60POS].d20_fkt_id = D20_CSAY;
    d20_6omask[D20_60POS+1].d20_fkt_id = (inp_allowed == TRUE)
					 ? D20_CGET : D20_CSAY;
				/* --> enter object name              */
    ret_value = ds0_501_object_name(TRUE, D20_OBJECT, DS_DUMP, rule_nr, tree,
				    init_flds, &help_count, &oct_ent);
    d20_6omask[D20_60POS].d20_fkt_id = d20_6omask[D20_60POS+1].d20_fkt_id =
				       D20_NO_OP;

    if                          /* obj name is not correct            */
       (ret_value != D2_NOERROR)
    {   return(D2_ERROR);
    }
    else
    {   *object = (Name_string)d20_object;
	*subset = *(signed32 *) d20_6omask[D20_60POS+1].d20_ioinfo;
    }
}
else
{   *object = NULL;
    *subset = OBJECT_SUBORDINATES;
}

return(D2_NOERROR);
}

/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d20_C06_read_dsa(p_env, dsa)                           */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function will display the mask to enter the DSA name.    */
/*      It returns the DSa-name read from screen.                     */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      p_env       environment of directory installation             */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      dsa         DSA name read from mask                           */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_NOERROR = The function has been performed successfully.    */
/*      D2_ERROR =   <DEL> has been selected.                         */
/*                                                                    */
/*exoff ***************************************************************/

d2_ret_val d20_C06_read_dsa(
  char * p_env,
  char * dsa)
{
for ( ; ; )
				/* get destination DSA                */
{   if                          /* error on reading DSA               */
       (d20_180_get_dsa(dsa, p_env))
    {   return(D2_ERROR);
    }

    if                          /* no DSA-name was given              */
       (*dsa == D2_EOS)
    {                           /* dipslay error message              */
	d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab[D20_E93IDX];
	d20_handlemask(d20_qomask);
	continue;
    }
    else
    {   break;
    }
}

return(D2_NOERROR);
}

#endif

/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d20_C07_get_time()                                     */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function will display the mask to enter the update times */
/*      for shadowing jobs. Depending on the choice of the update     */
/*      time registration two different masks are shown to enter      */
/*      the update times.                                             */
/*      The update times will be returned in the given structure.     */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      upd_time    = update time                                     */
/*      init	      boolean flag to indicate if time fields are to  */
/*		      be initialized.				      */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_NOERROR = The function has been performed successfully.    */
/*      D2_ERROR =   <DEL> has been selected.                         */
/*                                                                    */
/*exoff ***************************************************************/

d2_ret_val d20_C07_get_time(
  Upd_time * upd_time,
  Bool init)
{
signed32 fkt;
signed32 sav_freq;
signed32 sav_days;

for ( ; ; )
	{ /* loop until correct time is entered.	*/
   
    	/* get update frequency                    */
	/* if init is TRUE initialize the time fields, otherwise initialize */
	/* only if there is change in prev. time field.			    */

	if (init)
    		*((signed32 *) d20_E2omask[DS_E2_UPD_FREQ].d20_ioinfo) = UPD_HIGH;
	else
    		sav_freq = *((signed32 *) d20_E2omask[DS_E2_UPD_FREQ].d20_ioinfo);

	fkt = d20_handlemask (d20_E2omask);

    	/* break was selected                 */
       	if (fkt == FKTDEL)
    		return(D2_ERROR);


    	/* update frequency is HIGH 	*/
    	if  (* ((signed32 *) d20_E2omask[DS_E2_UPD_FREQ].d20_ioinfo) == UPD_HIGH)
		{
		upd_time->upd_t_mode = UPD_HIGH;
    		if ((init) || (sav_freq != UPD_HIGH))
			{
       	 		d20_Eaomask[DS_EA_UPD_TIME].d20_oformat = d20_F_mints;
        		*((signed32 *) d20_Eaomask[DS_EA_UPD_TIME].d20_ioinfo) =
								EVERY_5_MIN;
			}
		}
    	else
       		if (* ((signed32 *) d20_E2omask[DS_E2_UPD_FREQ].d20_ioinfo) == 
								UPD_MEDIUM)
			{
	    		upd_time->upd_t_mode = UPD_MEDIUM;
    			if ((init) || (sav_freq != UPD_MEDIUM))
				{
        			d20_Eaomask[DS_EA_UPD_TIME].d20_oformat = 
								d20_F_hrs;
        			*((signed32 *) d20_Eaomask[DS_EA_UPD_TIME].
						d20_ioinfo) = EVERY_HOUR;
				}
			}
		else
			{
			/* in case of low frequency, don't erase old times */
			/* (hours, day of week etc.)			   */

			d20_Eaomask[DS_EA_EMPTY_LN].d20_fkt_id = D20_NO_OP;
			d20_Eaomask[DS_EA_EMPTY_LN + 1].d20_fkt_id = D20_NO_OP;
			d20_Eaomask[DS_EA_EMPTY_LN + 2].d20_fkt_id = D20_NO_OP;
			d20_Eaomask[DS_EA_EMPTY_LN + 3].d20_fkt_id = D20_NO_OP;

			/* low frequency case.	*/
    			if ((init) || (sav_freq != *((signed32 *) d20_E2omask[
						DS_E2_UPD_FREQ].d20_ioinfo)))
				{
        			d20_Eaomask[DS_EA_UPD_TIME].d20_oformat = 
								d20_F_days;
        			*((signed32 *) d20_Eaomask[DS_EA_UPD_TIME].
						d20_ioinfo) = ALL_TIMES;
				}
			else
        			sav_days = *(signed32 *)d20_Eaomask[
						DS_EA_UPD_TIME].d20_ioinfo;
			
     			} 

    	/* get update times.	*/
	fkt = d20_handlemask (d20_Eaomask);

	d20_Eaomask[DS_EA_EMPTY_LN].d20_fkt_id = D20_CSAY;
	d20_Eaomask[DS_EA_EMPTY_LN + 1].d20_fkt_id = D20_CSAY;
	d20_Eaomask[DS_EA_EMPTY_LN + 2].d20_fkt_id = D20_CSAY;
	d20_Eaomask[DS_EA_EMPTY_LN + 3].d20_fkt_id = D20_CSAY;

	/* break was selected                 */
	if  (fkt == FKTDEL)
		return(D2_ERROR);

	/* low update frequency.	*/
    	if  ((*(signed32 *)d20_E2omask[DS_E2_UPD_FREQ].d20_ioinfo != UPD_HIGH) &&
    	     (*(signed32 *)d20_E2omask[DS_E2_UPD_FREQ].d20_ioinfo != UPD_MEDIUM))
		{
	    	/* Every Day.	*/
	    	if (*((signed32 *)d20_Eaomask[DS_EA_UPD_TIME].d20_ioinfo) ==
								 ALL_TIMES)
			{
	    		upd_time->upd_t_mode = UPD_LOW_1;
	    		upd_time->upd_1d_of_week = ALL_POSSIBLE_TIMES;

			/* get Hour in a Day.	*/
			if ((init) || (sav_days != ALL_TIMES))
	   			*((signed32 *)d20_Ebomask[DS_EB_HOURS].d20_ioinfo) 
								= ZERO_HOUR;

	    		fkt = d20_handlemask (d20_Ebomask);

	    		/* break was selected                 */
	       		if (fkt == FKTDEL)
	    			return(D2_ERROR);

			/* set hour in update structure.	*/
	    		upd_time->upd_1hour = 
				*((signed32 *)d20_Ebomask[DS_EB_HOURS].d20_ioinfo);

			}
	    	else
			{
			/* Once a Week	*/
	    		if (*((signed32 *) d20_Eaomask[DS_EA_UPD_TIME].d20_ioinfo) 
								== UPD_LOW_1)
				{
	    			upd_time->upd_t_mode = UPD_LOW_1;

				/* get Day and Hour.	*/

				if ((init) || (sav_days != UPD_LOW_1))
					{
		   			*((signed32 *)d20_Ecomask[DS_EC_DOW].
						d20_ioinfo) = SUNDAY;
		   			*((signed32 *)d20_Ecomask[DS_EC_HOURS].
						d20_ioinfo) = ZERO_HOUR;
					}

			    	fkt = d20_handlemask (d20_Ecomask);

		    		/* break was selected    */
		       		if (fkt == FKTDEL)
		    			return(D2_ERROR);

				/* set day and hour in update structure.*/
	    			upd_time->upd_1d_of_week = *((signed32 *)d20_Ecomask[
						DS_EC_DOW].d20_ioinfo);
	    			upd_time->upd_1hour = *((signed32 *)d20_Ecomask[
						DS_EC_HOURS].d20_ioinfo);

				}
			else /* Twice a Week.	*/
				{
	    			upd_time->upd_t_mode = UPD_LOW_2;

				/* get Two Days and Two Hours.	*/

				if ((init) || (sav_days != UPD_LOW_2))
					{
		   			*((signed32 *)d20_Edomask[DS_ED_DOW1].
							d20_ioinfo) = SUNDAY;
		   			*((signed32 *)d20_Edomask[DS_ED_HOURS1].
							d20_ioinfo) = ZERO_HOUR;
		   			*((signed32 *)d20_Edomask[DS_ED_DOW2].
							d20_ioinfo) = SUNDAY;
		   			*((signed32 *)d20_Edomask[DS_ED_HOURS2].
							d20_ioinfo) = ZERO_HOUR;
					}

			    	fkt = d20_handlemask (d20_Edomask);

		    		/* break was selected    */
		       		if (fkt == FKTDEL)
		    			return(D2_ERROR);


				/* set days and hours in update structure.*/

	    			upd_time->upd_1d_of_week = *((signed32 *)d20_Edomask[
						DS_ED_DOW1].d20_ioinfo);
	    			upd_time->upd_1hour = *((signed32 *)d20_Edomask[
						DS_ED_HOURS1].d20_ioinfo);
				upd_time->upd_2d_of_week = *((signed32 *)d20_Edomask[
					   	DS_ED_DOW2].d20_ioinfo);
	    			upd_time->upd_2hour = *((signed32 *)d20_Edomask[
						DS_ED_HOURS2].d20_ioinfo);
				}
			}
		}
	else
       		if (* ((signed32 *) d20_E2omask[DS_E2_UPD_FREQ].d20_ioinfo) == 
								UPD_MEDIUM)
	    		upd_time->upd_hour = *((signed32 *) d20_Eaomask[
						DS_EA_UPD_TIME].d20_ioinfo);
		else
			upd_time->upd_min = *((signed32 *)d20_Eaomask[
						DS_EA_UPD_TIME].d20_ioinfo);


	/* check update time                  */
    	if (d21d001_check_time(upd_time) == D2_NOERROR)
    		return(D2_NOERROR);

	d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab[D20_E32IDX];
	d20_handlemask(d20_qomask);
	init = FALSE;

	} /* end of for loop.	*/

} /* end of d20_C07_get_time */
