/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20remove.c,v $
 * Revision 1.1.10.2  1996/02/18  19:41:47  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:22:31  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:19:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:52:31  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:06:48  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:12:48  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:35:26  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:35:11  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  10:41:36  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:37:14  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:10:11  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:06:26  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  14:57:24  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  16:10:48  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  19:59:24  melman
 * 	New GDS merged drop
 * 	[1992/05/28  20:42:53  melman]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20remove.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:41:47 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* NAME         : d20remove.c                                         */
/*								      */
/* AUTHOR       : Schmid ,D AP 11                                     */
/* DATE         : 05.01.89                                            */
/*								      */
/*datoff **************************************************************/

#include <gds.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <dce/d21iapl.h>
#include <gdsext.h>
#include <d2adm.h>
#include <d2msk.h>
#include <d2delupd.h>
#include <cget.h>
#include <d2dump.h>
#include <d20proto.h>

/**********************************************************************/
/*								      */
/*		   D E C L A R A T I O N S       DATA		      */
/*								      */
/**********************************************************************/

extern D20_omask  d20_E1omask[];

extern D20_omask  d20_qomask[];
extern char      *d20_errtab[];
extern char      *d20_err_table[];

extern char		d20_m_dsa[];	/* master DSA of objects whose	*/
					/* shadows are being handled    */

/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d20_C01_remove_shadow()                                */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function will remove a shadowing job:                    */
/*          either it was marked using DISPLAY-SHADOWING-JOBS         */
/*          or it has to be entered using several masks               */
/*             (object/subtree and DSA)                               */
/*      The selected shadowing job will be removed (incl. the shadows */
/*      itself).                                                      */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      parblk      contains ODT and ADT                              */
/*      disp        a flag indicating whether shadowing job has been  */
/*                  displayed                                         */
/*      mode        indicates what's to be removed                    */
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

d2_ret_val d20_C01_remove_shadow(
  D20_pE * parblk,
  Bool disp,
  signed32 mode)
{
char        dsa[D2_DNL_MAX + 1];    /* distinguished name of DSA      */
char        object[D2_DNL_MAX + 1]; /* object name                    */
Name_string p_object;
Errcode     ret_code;
d2_ret_val  ret_value;
signed32    subset;
signed32    fkt;

if (disp == TRUE)
	{
	/* display job again                  */
	/* display job mask and wait for input.	*/
    	fkt = d20_C10_display_job_mask(TRUE);

    	/* break was selected                 */
    	if (fkt == FKTDEL)
    		return(D2_NOERROR);

	/* build object/subtree name          */
    	strcpy(object, d20_E1omask[DS_E1_OBJ_NAME].d20_ioinfo);
    	if (*d20_E1omask[DS_E1_OBJ_NAME + 1].d20_ioinfo != D2_EOS)
    		strcat(object, d20_E1omask[DS_E1_OBJ_NAME + 1].d20_ioinfo);

	/* convert object name into iapl format.	*/
    	ds0_508_check_name(object);
    	p_object = (strlen(object) == 0) ? (Name_string)NULL : (Name_string)object;


	/* build DSA-name                     */
    	strcpy(dsa, d20_E1omask[DS_E1_DSA].d20_ioinfo);
    	if (*d20_E1omask[DS_E1_DSA + 1].d20_ioinfo != D2_EOS)
    		strcat(dsa, d20_E1omask[DS_E1_DSA + 1].d20_ioinfo);

	/* convert dsa name into iapl format.	*/
    	ds0_508_check_name(dsa);

	/* remove shadows                     */
    	ret_value = d21_18_rmshd(parblk->d20_Ebid, &ret_code, DS_V021, p_object,
			 *(signed32 *) d20_E1omask[DS_E1_OBJ_INTRP].d20_ioinfo,
			 (Name_string)dsa,mode);
	}
else
	{      
	/* get shadowing job                  */
	/* read object                        */
    	if (d20_C05_read_obj(parblk, TRUE, &p_object, &subset) == D2_ERROR)
    		return(D2_NOERROR);

	strcpy(object, (char *)p_object);
	p_object = (Name_string)object;

	/* get destination DSA                */
    	/* error on reading DSA               */
	if (d20_C06_read_dsa(parblk->d20_Eenv, dsa) == D2_ERROR)
    		return(D2_NOERROR);

	/* remove shadows                     */
    	ret_value = d21_18_rmshd(parblk->d20_Ebid, &ret_code, DS_V021, p_object,
			 subset,(Name_string)(dsa + 1), mode);
	}

if (ret_value == D2_ERROR)
	{   
	switch((int) ret_code.errvalue)
    		{   
		case DS_DEL_ERR:        
			d20_qomask[D20_Q1POS].d20_ioinfo = 
						d20_err_table[D20_E17IDX];
			break;

		case DS_ALLOC_ERROR:    
			d20_qomask[D20_Q1POS].d20_ioinfo =
					       d20_err_table[D20_E04IDX];
			break;

		case DS_NFOUND:         
			d20_qomask[D20_Q1POS].d20_ioinfo =
					       d20_err_table[D20_E00IDX];
			break;

		case DS_SHD_FILE_ERR:   
			d20_qomask[D20_Q1POS].d20_ioinfo =
					       d20_err_table[D20_E26IDX];
			break;

		case DS_CRON_ERR:       
			d20_qomask[D20_Q1POS].d20_ioinfo =
					       d20_errtab[D20_101IDX];
			break;

		case DS_MASTB_IMP:
			d20_qomask[D20_Q1POS].d20_ioinfo =
						d20_err_table[D20_E118IDX];
			break;

		default:                
			d20_qomask[D20_Q1POS].d20_ioinfo =
					       d20_err_table[D20_E18IDX];
			break;
    		}

    	d20_handlemask(d20_qomask);
    	return(D2_ERROR);
	}
else
    	return(D2_NOERROR);

}

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_099_set_install_alias()                           */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        07.02.91                                              */
/*								      */
/* DESCRIPTION:                                                       */
/*		This function finds out if an alias object is to be   */
/*		installed on a shadow DSA or not.		      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*      D2_NOERROR:     No error occurred                             */
/*      D2_ERROR:       Error occurred 				      */
/*								      */
/*exoff ***************************************************************/

signed16 d20_099_set_install_alias(
  signed16 bind_id,
  D20_offset ** dn,
  signed32 dn_no,
  char * d20_beg,
  char * filename,
  signed32 attr_len)
{
signed32	count, j;
int		fd;
signed32	min_pos = 0;
signed32	max_pos = 0;
Bool		found;
char 		par_obj[D2_DNL_MAX + 1];
char		*at_mem;
D2_a_type	mk_type;
char		*mk_attr = NULL;
signed16	mk_len;
D21_p3		pb ;
D2_c_res 	c_res;
signed16	result ;
D2_pbhead	pbhead;
D2_c_arg	c_arg;
D2_a_type       req_att;
D2_req_info    	req_info;
char	norm_mk_attr[D2_DNL_MAX + 1];

req_att.d2_typ_len = mk_type.d2_typ_len = sizeof(D2_MASTER_KNOWLEDGE)-1;
req_att.d2_type    = mk_type.d2_type = (Ob_id_string) D2_MASTER_KNOWLEDGE;

req_info.d2_r_val_at = D2_R_VAL_TYP;
req_info.d2_r_no_at = 1;
req_info.d2_r_type_at = &req_att;

/* open search file.	*/
if ((fd = open(filename, O_RDONLY)) == -1)
	{
	return(D2_ERROR);
	}

attr_len += D2_ALIGN(D2_DNL_MAX);
if (attr_len < D20_BUFSIZ) 
	attr_len = D20_BUFSIZ;
 
/* allocate memory for attribute list.	*/
if ((at_mem = malloc(attr_len)) == NULL)
	{
	close(fd);
	return(D2_ERROR);
	}

/* for all objects	*/
for (count=0; count < dn_no; count++)
	{
	/* if object is an alias	*/
	if (((*(dn + count))->d20_state & D21_ALIAS_OBJ) == D21_ALIAS_OBJ)
		{
		/* find the parent of the alias object	*/
            	if (d21t033_parent_object_name(d20_beg + 
			(*(dn + count))->d20_mem_off, par_obj) == FALSE)
			{
			strcpy(par_obj, D2_NAME_SCHEME);
			d20_norm(par_obj, strlen(par_obj));
			}

		/* find out if parent has already been read	*/
		found = FALSE;

		for (j=0; j < count; j++)
			if (strcmp(par_obj, d20_beg + 
				(*(dn + j))->d20_mem_off) == 0)
				{
				found = TRUE;
				break;
				}

		/* parent has already been read, read the master 	*/
		/* knowledge from the search file result		*/
		if (found == TRUE)
			{
               		if (d21t008_get_attr(fd, *(dn + j), at_mem, 
			    &min_pos, &max_pos, mk_type, &mk_attr, &mk_len, 
			    d20_beg) == D2_ERROR)
				{
				free(at_mem);
				close(fd);
				return(D2_ERROR);
				}

			/* compare the master knowledge of the parent object */
			/* with the master dsa name 			*/
			/*parent is master, set the install flag for the alias*/

			strcpy(norm_mk_attr, mk_attr);
			d20_norm(norm_mk_attr, strlen(norm_mk_attr));

			if (strcmp(norm_mk_attr, d20_m_dsa) == 0)
				(*(dn + count))->d20_state |= D20_INSTALL;
			}
		else
			{
			/* parent has not been read, read the master 	*/
			/* knowledge of the parent object from bind DSA	*/

			/* generate parameter block */
			pbhead.d2_version = D21V022;
			pb.d21_3pbhead = &pbhead ;

			c_arg.d2_extension 	= (D2_extension *)NULL;
			c_arg.d2_op_progress.d2_nr_phase = D2_NOT_STARTED;
			c_arg.d2_op_progress.d2_next_rdn = 0;
			c_arg.d2_al_rdn 	 = 0;
			c_arg.d2_serv_cntrls 	 = 
			D2_NOREFERRAL | D2_USEDSA | D2_DREFALIAS | 
			D2_DONT_STORE | D2_LOCALSCOPE | D2_PREF_ADM_FUN;
			c_arg.d2_priority 	 = D2_PRIO_LOW;
			c_arg.d2_time_limit 	 = D2_T_UNLIMITED;
			c_arg.d2_size_limit 	 = D2_S_UNLIMITED;
			c_arg.d2_scope_ref 	 = D2_WORLD_SCOPE; 

			pb.d21_3c_a = &c_arg;
			pb.d21_3bind_id = bind_id ;
			pb.d21_3entry = (Name_string)par_obj;
			pb.d21_3r_info = &req_info ;

			pb.d21_3err = (D2_error *)NULL;
			pb.d21_3cres = &c_res;

			result = d21_3_read (&pb) ;
			if (result == D2_ERROR)
				{
    				if (pb.d21_3res != (D2_ei_info *) NULL)
    					free((char *) pb.d21_3res);

				free(at_mem);
				close(fd);
				return(D2_ERROR);
				}

			mk_attr = (char *)pb.d21_3res->d2_ei_ai->d2_a_val->d2_at_value.
				  d2_obj_name_a_v;

			/* compare the master knowledge of the parent object */
			/* with the master dsa name 			*/
			/* parent is master,set the install flag for the alias*/

			strcpy(norm_mk_attr, mk_attr);
			d20_norm(norm_mk_attr, strlen(norm_mk_attr));

			if (strcmp(norm_mk_attr, d20_m_dsa) == 0)
				(*(dn + count))->d20_state |= D20_INSTALL;

   			free((char *) pb.d21_3res);
			}

		}

	continue;
	}

free(at_mem);
close(fd);
return(D2_NOERROR);
}
