/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20create.c,v $
 * Revision 1.1.10.2  1996/02/18  19:41:35  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:22:19  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:18:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:52:21  root]
 * 
 * Revision 1.1.8.3  1994/06/21  14:43:49  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:24:56  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:06:40  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:12:16  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:35:15  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:33:40  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  10:33:10  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:35:37  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:09:48  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:05:52  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  14:55:00  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  16:08:35  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  22:03:24  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:45:45  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20create.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:41:35 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* NAME         : d20create.o                                         */
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
/*			  D E F	I N E S				      */
/*								      */
/**********************************************************************/

#define D27_DP_FILE     "/dpfile"

/**********************************************************************/
/*								      */
/*		   D E C L A R A T I O N S       DATA		      */
/*								      */
/**********************************************************************/

extern D20_omask  d20_E1omask[];
extern D20_omask  d20_E2omask[];
extern D20_omask  d20_Eaomask[];
extern D20_omask  d20_Ebomask[];

extern D20_omask  d20_qomask[];
extern char      *d20_errtab[];
extern char      *d20_err_table[];
extern char      *d20_errmess[];

extern char      *d20_E12format;

extern Bool       d20_call_cread;

extern signed32   d20_dsastr;         /* actual structure of dsa name */

/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d20_C00_create_shadow()                                */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function will create shadows from the local DSA in the   */
/*      given destination DSA.                                        */
/*      All input parameter are from screen (object/subtree, DSA,     */
/*      update times).                                                */
/*      The objects will be dumped and the shadows are generated.     */
/*      If update times are defined the shadowing job will be updated */
/*      in the internal shadowing job file.                           */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      parblk      contains ODT and ADT                              */
/*      mode        indicates what's to be created                    */
/*                      JOB_ONLY:   only shadowing job                */
/*                      SHD_AND_JOB: both shadows and shadowing job   */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*                    None                                            */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_NOERROR = The function has been performed successfully.    */
/*      D2_ERROR   = function fails                                   */
/*                                                                    */
/*exoff ***************************************************************/

d2_ret_val d20_C00_create_shadow(
  D20_pE * parblk,
  signed32 mode)
{
				/* distinguished name of local DSA    */
char        local_dsa[D2_DNL_MAX + 1];
char        dsa[D2_DNL_MAX + 1];/* distinguished name of DSA          */
char        disp_dsa[D2_DNL_MAX + 1];/* distinguished name of DSA in display */
				     /* format.				     */

Bool        first = TRUE;    /* a flag indicating whether the loop */
				/* has already been performed or not  */
signed32    upd_mode;           /* a flag indicating whether update   */
				/* times are defined or not           */
Upd_time    update_time;
Upd_time   *p_time;
C_arg       c_arg;
Errcode     ret_code;
char        file_name[D27_LFILE_NAME];
Name_string p_obj_name;
char	    obj_name[D2_DNL_MAX + 1];
char        disp_obj_name[D2_DNL_MAX + 1]; /* object name in display format. */
signed32    subset;
Subtree    *subtree;
FILE       *fp;
signed32    fkt;


file_name[0] = D2_EOS;

/* read object                        */
if (d20_C05_read_obj(parblk, TRUE, &p_obj_name, &subset) == D2_ERROR)
	return(D2_NOERROR);

if (p_obj_name == NULL)
	/* ROOT of DIT is involved.	*/
	obj_name[0] = '\0';
else
	strcpy(obj_name, (char *)p_obj_name);

strcpy(disp_obj_name, obj_name);
d20_gen_dnintf(disp_obj_name);

/* set the OBJECT NAME fields in the mask.	*/
d20_C08_copy_name(disp_obj_name,d20_E1omask[DS_E1_OBJ_NAME].d20_ioinfo,
		  d20_E1omask[DS_E1_OBJ_NAME + 1].d20_ioinfo);

/* set the SUBSET field in the mask.	*/
*(signed32 *) d20_E1omask[DS_E1_OBJ_INTRP].d20_ioinfo = subset;

d20_E1omask[DS_E1_JOB].d20_oformat = d20_E12format;

for ( ; ; )
	{
	/* get destination DSAs               */
   	/* error on reading DSA               */
       	if (d20_C06_read_dsa(parblk->d20_Eenv, dsa) == D2_ERROR)
    		break;

	/* syntax-check for distinguished name */
	if (d20_198_distname_syntax (dsa, d20_dsastr) == D2_ERROR)
	   {
	      continue;
	   }


    	strcpy(disp_dsa, dsa + 1);
    	d20_gen_dnintf(disp_dsa);

    	/* set the TARGET DSA fields in the mask.	*/
    	d20_C08_copy_name(disp_dsa,d20_E1omask[DS_E1_DSA].d20_ioinfo,
		  d20_E1omask[DS_E1_DSA + 1].d20_ioinfo);

	/* get job                            */
    	* ((signed32 *)d20_E1omask[DS_E1_JOB].d20_ioinfo) = ACTIVATE;

	fkt = d20_handlemask (d20_E1omask);

    	/* break was selected                 */
       	if (fkt == FKTDEL)
    		break;

    	/* JOB = activate updates             */
       	if ((upd_mode = *(signed32 *) d20_E1omask[DS_E1_JOB].d20_ioinfo) == ACTIVATE)
    		/* get update times                   */
		/* call get_time with initialization flag on.	*/
		if (d20_C07_get_time(&update_time, TRUE) == D2_ERROR)
			break;

    	if (first == TRUE && mode == SHD_AND_JOB)
    		{
		/* get local DSA-name                 */
		sprintf(file_name, "%s%s%s%s%d%s", dcelocal_path,
			D27_SERVER_DIR, D27_DSA_DIR, D27_SUBDSA_DIR,
			parblk->d20_Edir_id, D27_DSANAME);
		if ((fp = fopen(file_name, "r")) == (FILE *) NULL)
			{
			d20_qomask[D20_Q1POS].d20_ioinfo = 
						d20_err_table[D20_E15IDX];
	    		d20_handlemask(d20_qomask);
	    		return(D2_ERROR);
			}

		if (fgets(local_dsa, D2_DNL_MAX - 1, fp) == NULL)
			{
			d20_qomask[D20_Q1POS].d20_ioinfo = 
						d20_err_table[D20_E15IDX];
	    		d20_handlemask(d20_qomask);
	    		fclose(fp);
	    		return(D2_ERROR);
			}

		fclose(fp);

		sprintf(file_name, "%s%s%s%-d", parblk->d20_Eenv, D27_ADM_DIR,
						D27_DP_FILE, getpid());

		c_arg.serv_cntrls = D2_LOCALSCOPE;

		/* dump shadows                       */
		if  (d21_16_dump(parblk->d20_Ebid, &ret_code, DS_V021, 
		     (Name_string)local_dsa,(Name_string)obj_name,
				subset, &c_arg, file_name) == D2_ERROR)
			{   
			switch((int) ret_code.errvalue)
	    			{   
				case DS_NFOUND:
					d20_qomask[D20_Q1POS].d20_ioinfo =
						   d20_err_table[D20_E00IDX];
					break;
				case DS_NO_RESPONSE:
					d20_qomask[D20_Q1POS].d20_ioinfo =
						   d20_err_table[D20_E10IDX];
				    	break;

				case DS_SEARCH_ERR: 
					d20_qomask[D20_Q1POS].d20_ioinfo =
						   d20_err_table[D20_E01IDX];
				    	break;

				case DS_FILE_ERR:   
					d20_qomask[D20_Q1POS].d20_ioinfo =
						   d20_err_table[D20_E03IDX];
				    	break;

				case DS_ALLOC_ERROR:
					d20_qomask[D20_Q1POS].d20_ioinfo =
						   d20_err_table[D20_E04IDX];
				    	break;

				case D2_ON_ALIAS_NOT_ALLOWED:
					d20_qomask[D20_Q1POS].d20_ioinfo =
						   d20_errmess[D20_E19IDX];
				    	break;

				case DS_MASTB_IMP:
					d20_qomask[D20_Q1POS].d20_ioinfo =
						   d20_err_table[D20_E118IDX];
					break;

				default:            
					d20_qomask[D20_Q1POS].d20_ioinfo =
						   d20_err_table[D20_E02IDX];
					break;
	    			}

	    		d20_handlemask(d20_qomask);
	    		break;
			}

		first = FALSE;
    		}

	/* generate shadows                   */
    	if (d21_17_gnshd(parblk->d20_Ebid, &ret_code, DS_V021, mode,
			(Name_string)obj_name, subset, file_name,
			(Name_string)(dsa + 1), FALSE, &subtree) == D2_ERROR)
    		{   
		switch((int) ret_code.errvalue)
			{   
			case DS_SEARCH_ERR:     
				d20_qomask[D20_Q1POS].d20_ioinfo =
						   d20_err_table[D20_E05IDX];
			    	break;

	    		case DS_ADD_ERR:        
				d20_qomask[D20_Q1POS].d20_ioinfo =
						   d20_err_table[D20_E06IDX];
				break;

	    		case DS_MOD_ERR:        
				d20_qomask[D20_Q1POS].d20_ioinfo =
						   d20_err_table[D20_E07IDX];
				break;

	    		case DS_DEL_ERR:        
				d20_qomask[D20_Q1POS].d20_ioinfo =
						   d20_err_table[D20_E08IDX];
				break;

	    		case DS_NO_FILE:
	    		case DS_FILE_ERR:       
				d20_qomask[D20_Q1POS].d20_ioinfo =
						   d20_err_table[D20_E09IDX];
				break;

	    		case DS_OVERLAY:        
				d20_qomask[D20_Q1POS].d20_ioinfo =
						   d20_err_table[D20_E13IDX];
				break;

	    		case DS_CRON_ERR:        
				d20_qomask[D20_Q1POS].d20_ioinfo =
						   d20_errtab[D20_101IDX];
				break;

	    		case DS_ALLOC_ERROR:    
				d20_qomask[D20_Q1POS].d20_ioinfo =
						   d20_err_table[D20_E04IDX];
				break;

	    		case DS_DEST_LOC:    
				d20_qomask[D20_Q1POS].d20_ioinfo =
						   d20_errtab[D20_E76IDX];
				break;

			case DS_MASTB_IMP:
				d20_qomask[D20_Q1POS].d20_ioinfo =
						   d20_err_table[D20_E118IDX];
				break;

	    		default:                
				d20_qomask[D20_Q1POS].d20_ioinfo =
						   d20_err_table[D20_E02IDX];
				break;
			}

		d20_handlemask(d20_qomask);
		continue;
    		}
	else	/* Change gdsconfig file	*/
		d20_turn_shadowing_job( parblk->d20_Edir_id, D2_ON );

    	if (subtree != (Subtree *) 0)
    		free((char *) subtree);

    	if (upd_mode == ACTIVATE)
    		{
		p_time = &update_time;

		if (d21_19_updat(parblk->d20_Ebid, &ret_code, DS_V021, 
			upd_mode,(Name_string)obj_name, subset,
				(Name_string)(dsa + 1), p_time) == D2_ERROR)
			{   
			d20_qomask[D20_Q1POS].d20_ioinfo = 
						d20_err_table[D20_E11IDX];
	    		d20_handlemask(d20_qomask);
			}
    		}
	}

if (file_name[0] != D2_EOS)
	{
	unlink(file_name);
    	strcat(file_name, "_1");
    	unlink(file_name);
	}

return(D2_NOERROR);
}
