/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20chgmas.c,v $
 * Revision 1.1.10.2  1996/02/18  19:39:38  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:20:40  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:14:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:59  root]
 * 
 * Revision 1.1.8.3  1994/05/10  15:49:15  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:09:12  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:04:39  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:05:58  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:57:32  marrek
 * 	Update for dce1.1.
 * 	[1994/02/09  14:47:21  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:21:54  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  09:24:10  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:19:04  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:47:39  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:01:38  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  14:16:48  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  15:31:17  marrek]
 * 
 * Revision 1.1.2.3  1992/06/30  20:53:40  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:32:22  melman]
 * 
 * Revision 1.1.2.2  1992/06/01  21:44:08  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:11:41  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20chgmas.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:39:38 $";
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
/*                                                                    */
/* NAME         : d20chgmas.c                                         */
/*                                                                    */
/* AUTHOR       : Josep Pulpillo D525 (CDS)                           */
/* DATE         : 21.09.89                                            */
/*                                                                    */
/* KOMPONENTE   : DS                                                  */
/*                                                                    */
/* DOK.-NR.     : DS-Design-Spez.                                     */
/*                      .                                             */
/*                      .                                             */
/*                                                                    */
/* PRD#/VERS.   : <Produktnr. aus Produktstruktur> - <Versionsnr.>    */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* SYSTEM-DEPENDENCIES:                                               */
/*                      COMMON                                        */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*      0.1 | 21.09.89| Erstellung                     | hv |         */
/*                                                                    */
/*datoff **************************************************************/
 
#include <gds.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
 
#include <dce/d2dir.h>
#include <gdsext.h>
#include <dce/d27util.h>
#include <d2adm.h>
#include <d2msk.h>
#include <d21iapl.h>
#include <d20proto.h>
 
extern D20_omask   d20_6omask[];
extern D20_omask   d20_8omask[];
extern char       *d20_8ifields[];
 
extern D20_omask   d20_qomask[];
extern char       *d20_err_table[];
extern char       *d20_errtab[];
extern char       *d20_errmess[];
 
extern signed16    d20_bid;         /* global bind id                 */
 
/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME       : d20_E05_chgmas()                                      */
/*                                                                    */
/* AUTHOR:      Josep Pulpillo  D525 (CDS)                            */
/* DATE:        21.09.89                                              */
/*                                                                    */
/* SYNTAX:      signed32 d20_E05_chgmas(parblk)                       */
/*                                                                    */
/*              D20_pE      *parblk;                                  */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*      This function displays the mask to enter input for changing   */
/*      master knowledge attribute for a subtree and calls the        */
/*      appropiate function to do it.                                 */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*              D20_pE      *parblk;                                  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*                                                                    */
/* STANDARD-HEADER: None                                              */
/*                                                                    */
/* GLOBAL  DATA  (read-only):                                         */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*              ds0_530_display_errlist();                            */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*exoff ***************************************************************/
 
 signed32 d20_E05_chgmas(D20_pE *parblk)
{                               /*  d20_E05_chgmas                    */
Name_string   object;
char          obj_name[D2_DNL_MAX];
char          old_master[D2_DNL_MAX];
char          new_master[D2_DNL_MAX];
char         *subtree_root;
Errcode       return_code;
signed16      bind_id = -1;
Name_string  *not_changed = NULL;
signed32      subset, i;
signed32      old_len;
signed32      new_len;
D20_at	      *run_at_ptr = parblk->d20_Eat;

	D21_p3	pb ;
	D2_c_res c_res;
	D2_pbhead	pbhead ;
	D2_c_arg	d2_c_arg;
   	static D2_a_type      reqinfo = {sizeof(D2_MASTER_KNOWLEDGE)-1,
				       (Ob_id_string) D2_MASTER_KNOWLEDGE};
   	static D2_req_info    rinf = {D2_R_VAL_TYP, 1, &reqinfo};
	d2_ret_val	result;
	char		message[D20_MAXMSG];
 
 
/* read object to be modified         */
* (signed32 *)d20_6omask[D20_60POS+1].d20_ioinfo = OBJECT_SUBORDINATES;
if (d20_C05_read_obj((D20_pE *) parblk, FALSE, &object, &subset)
                     == D2_ERROR)
	return(D2_NOERROR);

if (object == NULL)
	subtree_root = (char *)object;
else
	{
	subtree_root = obj_name;
    	strcpy(obj_name, (char *)object);
	}


/* if object is not ROOT, read old master knldg. from DSA.	*/
if (object != NULL)
	{
	/* generate parameter block */
	pbhead.d2_version = D21V022;
	pb.d21_3pbhead = &pbhead ;

	d2_c_arg.d2_extension 		 = (D2_extension *)NULL;
	d2_c_arg.d2_op_progress.d2_nr_phase = D2_NOT_STARTED;
	d2_c_arg.d2_op_progress.d2_next_rdn = 0;
	d2_c_arg.d2_al_rdn 		 = 0;
	d2_c_arg.d2_serv_cntrls 	 = D2_NOCACHE | D2_NOREFERRAL | 
					   D2_USEDSA | D2_DREFALIAS | 
					   D2_DONT_STORE | D2_PREF_ADM_FUN;
	d2_c_arg.d2_priority 		 = D2_PRIO_LOW;
	d2_c_arg.d2_time_limit 		 = D2_T_UNLIMITED;
	d2_c_arg.d2_size_limit 		 = D2_S_UNLIMITED;
	d2_c_arg.d2_scope_ref 		 = D2_WORLD_SCOPE; 
	pb.d21_3c_a = &d2_c_arg;

	pb.d21_3bind_id = d20_bid;
	pb.d21_3entry = object;
	pb.d21_3r_info = &rinf;

	pb.d21_3err = (D2_error *)NULL;
	pb.d21_3cres = &c_res;

	result = d21_3_read (&pb) ;

	if (result != D2_NOERROR)
		{
		/* problem in reading master knowledge attribute.	*/

		d20_qomask[D20_Q1POS].d20_ioinfo = d20_errmess[
			map_errnum(pb.d21_3pbhead->d2_errvalue)];

		/* special message when operation tried on alias.	*/
		if (pb.d21_3err != NULL)
			if (pb.d21_3err->d2_e_type == D2_AT_PROBLEM)
				if (pb.d21_3err->d2_e_problem.d2_e_atproblem->
			    d2_ae_prob->d2_ap_prob == D2_ATTRIBUTE_MISSING)
					/* object is an alias object.	*/
					/* operation not allowed on alias. */

					d20_qomask[D20_Q1POS].d20_ioinfo =
						   d20_errmess[D20_E19IDX];

		d20_handlemask (d20_qomask) ;

		if (pb.d21_3res != (D2_ei_info *)NULL)
			free((char *)pb.d21_3res);

		return(D2_ERROR);
		}

	strcpy(old_master,
	  (char *)pb.d21_3res->d2_ei_ai->d2_a_val->d2_at_value.d2_obj_name_a_v);
    	if (d20_gen_dnintf(old_master) != D2_NOERROR)
		{
		/* wrong master knowledge format.	*/
		d20_qomask[D20_Q1POS].d20_ioinfo = d20_errmess[D20_E00IDX];
		d20_handlemask (d20_qomask) ;

		if (pb.d21_3res != (D2_ei_info *)NULL)
			free((char *)pb.d21_3res);

		return(D2_ERROR);
		}

	ds0_disp_old_val(strlen(old_master), (Octet_string) old_master, 
		  	 D2_DISTNAME);

	}
else
	{   
	*d20_8ifields[DS_MA1OLD_VAL] = *d20_8ifields[DS_MA2OLD_VAL] =
    	*d20_8ifields[DS_MA1NEW_VAL] = *d20_8ifields[DS_MA2NEW_VAL] = D2_EOS;
	}

/* generate attribute name from attribute table.	*/
for (i=0; i<parblk->d20_Enat; i++, run_at_ptr++)
	if (d20_cmp_objid(&(run_at_ptr->obj_id),sizeof(D2_MASTER_KNOWLEDGE),
				   (Ob_id_string) D2_MASTER_KNOWLEDGE))
		{
		strcpy(d20_8omask[D20_81POS].d20_ioinfo, 
		       run_at_ptr->at.att_name);
		break;
		}

/* attribute name should only be displayed.	*/
d20_8omask[D20_81POS].d20_fkt_id = D20_CSAY;

/* read the old and new value         */
for ( ; ; )
	{   
	*d20_8ifields[DS_MA_BREAK] = DS_DEFAULT_VALUE;

	d20_handlemask(d20_8omask);

    	/* break was selected                 */
       	if (*d20_8ifields[DS_MA_BREAK] != DS_DEFAULT_VALUE)
    		{   
		d20_8omask[D20_81POS].d20_fkt_id = D20_CGET;

		if (pb.d21_3res != (D2_ei_info *)NULL)
			free((char *)pb.d21_3res);

		return(D2_ERROR);
    		}

    	strcpy(old_master,d20_8ifields[DS_MA1OLD_VAL]);
    	strcat(old_master,d20_8ifields[DS_MA2OLD_VAL]);
    	strcpy(new_master,d20_8ifields[DS_MA1NEW_VAL]);
    	strcat(new_master,d20_8ifields[DS_MA2NEW_VAL]);

	/* check if old_master name ends with ' character.	*/
	i = 0;
	while (old_master[i] != D2_EOS)
		if (old_master[i] == '\'')
			break;
		else
			i++;

	if ((old_len = i) == 0)
		{
        	d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab[D20_E06IDX];
        	d20_handlemask(d20_qomask) ;
		continue;
		}

	if (old_master[i] == D2_EOS)
		{
		d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab[D20_E08IDX];
		d20_handlemask(d20_qomask);
		continue;
		}

	old_master[i] = D2_EOS;

	if ((result = ds0_508_check_name(old_master)) != D2_NOERROR)
		{
		sprintf(message, d20_errtab[D20_E44IDX], 1);
        	d20_qomask[D20_Q1POS].d20_ioinfo = message;
        	d20_handlemask(d20_qomask) ;
		continue;
		}

	/* check if new_master name ends with ' character.	*/
	i = 0;
	while (new_master[i] != D2_EOS)
		if (new_master[i] == '\'')
			break;
		else
			i++;

	if ((new_len = i) == 0)
		{
        	d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab[D20_E05IDX];
        	d20_handlemask(d20_qomask) ;
		continue;
		}

	if (new_master[i] == D2_EOS)
		{
		d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab[D20_E08IDX];
		d20_handlemask(d20_qomask);
		continue;
		}

	new_master[i] = D2_EOS;

	if ((result=ds0_508_check_name(new_master)) != D2_NOERROR)
		{
		sprintf(message, d20_errtab[D20_E44IDX], 2);
        	d20_qomask[D20_Q1POS].d20_ioinfo = message;
        	d20_handlemask(d20_qomask) ;
		continue;
		}

	break;
	}

d20_8omask[D20_81POS].d20_fkt_id = D20_CGET;

bind_id = parblk->d20_Ebid;

                                /* change master knowledge            */
if  (d21_13_chmas(bind_id, &return_code, DS_V021, (Name_string) subtree_root,
	      (Name_string) old_master, (Name_string) new_master, &not_changed)
					== D2_ERROR)
{   switch((int) return_code.errvalue)
    {   case DS_NO_BIND:    d20_qomask[D20_Q1POS].d20_ioinfo =
                                           d20_errmess[D20_E01IDX];
                            break;
        case DS_NO_RESPONSE:d20_qomask[D20_Q1POS].d20_ioinfo =
                                           d20_errmess[D20_E24IDX];
                            break;
        case DS_SEARCH_ERR: d20_qomask[D20_Q1POS].d20_ioinfo =
                                           d20_err_table[D20_E46IDX];
                            break;
        case DS_NFOUND:     d20_qomask[D20_Q1POS].d20_ioinfo =
                                           d20_errmess[D20_E18IDX];
                            break;
        case DS_ALLOC_ERROR:d20_qomask[D20_Q1POS].d20_ioinfo =
                                           d20_err_table[D20_E04IDX];
                            break;
        case DS_MOD_ERR:    d20_qomask[D20_Q1POS].d20_ioinfo =
                                           d20_err_table[D20_E47IDX];
                            break;
        case DS_NO_NEW_MASTER: d20_qomask[D20_Q1POS].d20_ioinfo =
					   d20_err_table[D20_E48IDX];
                            break;
        default:            d20_qomask[D20_Q1POS].d20_ioinfo =
                                           d20_err_table[D20_E45IDX];
                            break;
    }
    d20_handlemask(d20_qomask);
}
 
if ((bind_id >= 0) && (bind_id != d20_bid))
{   d20__unbind(bind_id);
}
 
if (not_changed != NULL)
	{
  	ds0_530_display_errlist(not_changed,D20_EH_NCHNG);
  	free(not_changed);
	}

if (object != NULL)
	if (pb.d21_3res != (D2_ei_info *)NULL)
		free((char *)pb.d21_3res);

return(D2_NOERROR);
 
}                               /*  d20_E05_chgmas                    */
