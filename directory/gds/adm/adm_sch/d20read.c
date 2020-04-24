/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20read.c,v $
 * Revision 1.1.10.2  1996/02/18  19:39:27  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:20:28  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:13:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:44  root]
 * 
 * Revision 1.1.8.4  1994/07/06  15:06:20  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:10:49  marrek]
 * 
 * Revision 1.1.8.3  1994/06/21  14:43:15  marrek
 * 	June 1994 code submission.
 * 	[1994/06/17  13:07:57  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:04:28  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:05:11  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:57:15  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:20:10  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  09:13:48  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:16:47  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:47:14  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:01:02  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  13:55:30  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  15:02:56  marrek]
 * 
 * Revision 1.1.2.3  1992/06/30  20:53:35  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:32:15  melman]
 * 
 * Revision 1.1.2.2  1992/06/01  21:42:28  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:10:51  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20read.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:39:27 $";
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
/* NAME         : d20read.o   [read scheme interface]                 */
/*								      */
/* AUTHOR       : Sanjay, D AP 11                                     */
/* DATE         : 20.01.91					      */
/*								      */
/* COMPONENT    : DS                                                  */
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
/* HISTORY      :                                                     */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*      0.1 | 04.07.88| Original                       | ws |         */
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

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <gdsext.h>
#include <d21iapl.h>
#include <d2shm.h>
#include <d20proto.h>

/**********************************************************************/
/*								      */
/*			  D E F	I N E S				      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                 D E C L A R A T I O N S        DATA                */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*								      */
/**********************************************************************/

static void d21_reset_return_vals(signed32 *no_srt, Srt **srt, signed32 *no_oct,
			Oct **oct, signed32 *no_at, At **at);
static signed32 d21_srt_ent_cmp(Srt *srt_ent1, Srt *srt_ent2);
static signed32 d21_oct_ent_cmp(Oct *oct_ent1, Oct *oct_ent2);
static signed32 d21_at_ent_cmp(At *at_ent1, At *at_ent2);

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d21_21_rdsh()                                         */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        14.01.91					      */
/*								      */
/* SYNTAX:      d2_ret_val d21_21_rdsh(bind_id,ret_code,version,      */
/*				no_srt,	srt,no_oct,oct,no_at,at)      */
/*								      */
/*		signed16   bind_id;				      */
/*		Errcode   *ret_code;				      */
/*		signed16   version;				      */
/*		signed32  *no_srt;				      */
/*		Srt	 **srt;				      	      */
/*		signed32  *no_oct;				      */
/*		Oct	 **oct;				      	      */
/*		signed32  *no_at;			      	      */
/*		At       **at;				      	      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will read the scheme from DSA and store */
/*		SRT, OCT and AT in memory.			      */ 
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*		signed16 bind_id;				      */
/*		signed16 version;				      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*		Errcode   *return_code;				      */
/*		signed32  *no_srt;				      */
/*		Srt	 **srt;				      	      */
/*		signed32  *no_oct;				      */
/*		Oct	 **oct;				      	      */
/*		signed32  *no_at;			      	      */
/*		At       **at;				      	      */
/*								      */
/* RETURNVALUE:							      */
/*      D2_NOERROR:     No error occurred                             */
/*      D2_ERROR:       Error occurred                                */
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

 d2_ret_val d21_21_rdsh(
   signed16   bind_id,
   Errcode   *ret_code,
   signed16   version,
   signed32  *no_srt,
   Srt      **srt,
   signed32  *no_oct,
   Oct      **oct,
   signed32  *no_at,
   At       **at)

{                               /*  d21_21_rdsh()               */

/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                  D E C L A R A T I O N S                           */
/*								      */
/**********************************************************************/
 D2_pbhead      head;

				/* requested attr. types to read      */
 D2_a_type      att_typ_arr[3];

/* These variables are used with d21_l03_trace_err()	*/
/* which is commented out currently			*/
/* static char *f_name = "d21_21_rdsh"; */
/* static char *f_entry = "ENTRY"; */
/* static char *f_exit =  "EXIT"; */

D21_p3		pb; 	/* parameter block for read operation */
D2_c_arg	c_arg;
D2_req_info	req_info;
D2_c_res	c_res;
signed16	result;
signed32	i,j,k,info_cnt;
Srt		*srt_ent;
Oct		*oct_ent;
At		*at_ent;
char		at_value[D2_ATL_MAX + 2];
Abbrv		abbrv;
D2_a_info	*at_info;
D2_a_value 	*d2_a_val;
signed16	srt_present 	= FALSE;
signed16	oct_present 	= FALSE;
signed16	at_present 	= FALSE;
signed16	other_present 	= FALSE;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/


d20_errcode(ret_code, D2_NOERROR, D2_NOERROR, D2_NOERROR);
d20_error(&head, D2_NOERROR, D2_NOERROR, D2_NOERROR);
/* d21_l03_trace_err(bind_id, f_name, f_entry, &head); */

/*  version is wrong                  */
if (version != DS_V03)
	{/*  set error values                  */
	/*  return(D2_ERROR)                  */
  	d20_errcode(ret_code, D2_CALL_ERR, D2_ILL_VERS, D2_NOT_SPEC);
  	d20_error(&head, D2_CALL_ERR, D2_ILL_VERS, D2_NOT_SPEC);
  	/* d21_l03_trace_err(bind_id, f_name, f_exit, &head); */
	d21_reset_return_vals(no_srt, srt, no_oct, oct, no_at, at);
  	return(D2_ERROR);
	}

/* set parameters for reading schema object */
head.d2_version = D21V022;
pb.d21_3pbhead = &head;

c_arg.d2_extension 		 = (D2_extension *)NULL;
c_arg.d2_op_progress.d2_nr_phase = D2_NOT_STARTED;
c_arg.d2_op_progress.d2_next_rdn = 0;
c_arg.d2_al_rdn 		 = 0;
c_arg.d2_serv_cntrls 		 = D2_NOREFERRAL | D2_USEDSA | D2_DREFALIAS | 
				   D2_DONT_STORE | D2_PREF_ADM_FUN;
c_arg.d2_priority 		 = D2_PRIO_LOW;
c_arg.d2_time_limit 		 = D2_T_UNLIMITED;
c_arg.d2_size_limit 		 = D2_S_UNLIMITED;
c_arg.d2_scope_ref 		 = D2_WORLD_SCOPE; 

pb.d21_3c_a = &c_arg;

pb.d21_3bind_id = bind_id;
pb.d21_3entry = (D2_name_string)D2_NAME_SCHEME;

att_typ_arr[0].d2_type 		=  (Ob_id_string)D2_SRT;
att_typ_arr[0].d2_typ_len 	=  sizeof(D2_SRT) - 1;
att_typ_arr[1].d2_type 		=  (Ob_id_string)D2_OCT;
att_typ_arr[1].d2_typ_len 	=  sizeof(D2_OCT) - 1;
att_typ_arr[2].d2_type 		=  (Ob_id_string)D2_AT;
att_typ_arr[2].d2_typ_len 	=  sizeof(D2_AT) - 1;

req_info.d2_r_val_at 		= D2_R_VAL_TYP;
req_info.d2_r_no_at 		= 3;
req_info.d2_r_type_at 		= att_typ_arr;

pb.d21_3r_info = &req_info;

pb.d21_3err = (D2_error *)NULL;
pb.d21_3cres = &c_res;

/* error in reading schema object */
if ((result = d21_3_read(&pb)) == D2_ERROR)
	{
    	if (pb.d21_3res != (D2_ei_info *) NULL)
    		free((char *) pb.d21_3res);

  	d20_errcode(ret_code, D2_TEMP_ERR, D2_SYS_ERR, DS_READ_FAILS);
  	d20_error(&head, D2_TEMP_ERR, D2_SYS_ERR, DS_READ_FAILS);
	/* d21_l03_trace_err(bind_id, f_name, f_exit, &head); */
	d21_reset_return_vals(no_srt, srt, no_oct, oct, no_at, at);
	return(D2_ERROR);
	}

/* get all the attributes of the schema object from the result */
for (at_info = pb.d21_3res->d2_ei_ai;at_info != (D2_a_info *)NULL;
					    at_info = at_info->d2_a_next)
	{
	/* attribute is SRT */
	if (d20_cmp_objid(&(at_info->d2_a_type), sizeof(D2_SRT),
		(Ob_id_string) D2_SRT))
		{
		srt_present = TRUE;
		*no_srt = at_info->d2_a_no_val;

		/* allocate space for SRT, extra entries and to store */
		/* srt entries limit 				      */
		if ((*srt = (Srt *)malloc(((*no_srt + D2_S_MORE) * 
					    sizeof(Srt)) + sizeof(signed32)))
		    == (Srt *)NULL)	
			{
    			free((char *) pb.d21_3res);

  			d20_errcode(ret_code, D2_TEMP_ERR, D2_SYS_ERR, 
							DS_ALLOC_ERROR);
  			d20_error(&head, D2_TEMP_ERR, D2_SYS_ERR, 
							DS_ALLOC_ERROR);
			/* d21_l03_trace_err(bind_id, f_name, f_exit, &head);*/
			d21_reset_return_vals(no_srt, srt, no_oct, oct, 
							   no_at, at);
			return(D2_ERROR);
			}


		/* get all the SRT entries from SRT attribute */
		for (i=0,srt_ent=*srt,d2_a_val=at_info->d2_a_val; 
						i<*no_srt; 
						i++,srt_ent++,d2_a_val++)
			{
			/* check for length of the SRT attribute value */
			if (d2_a_val->d2_a_v_len > (signed16)D2_L_SRT_ATT)
				goto form_error_exit;

			/*copy next value in temporary att. val array*/
			strncpy(at_value,
				(char *)d2_a_val->d2_at_value.d2_oct_a_v,
				d2_a_val->d2_a_v_len);

			*(at_value + d2_a_val->d2_a_v_len) = D2_INFO_DELIM;
			*(at_value + d2_a_val->d2_a_v_len + 1) = D2_EOS;

			/* read all information from tmp. att. val. 	     */
			/* except the naming attributes and the object class */
			info_cnt = sscanf(at_value, D2_FORMAT_SRT,
						 &(srt_ent->rule_nr),
						 &(srt_ent->par_rule_nr));

			if (info_cnt != D2_NO_INFO_SRT)
				goto form_error_exit;

			/* skip D2_NO_INFO_SRT no. of delimiters and */
			/* count the length of string read so far    */
			for (j=0,k=0;j<D2_NO_INFO_SRT;k++)
				if (at_value[k] == D2_INFO_DELIM)
					j++;

			/*get the naming attributes from tmp. att. val.*/	
			j = 0;
			while (sscanf(at_value + k,"%s",abbrv) == 1)
				{
				k += strlen(abbrv) + 1;
				
				/* seperator between naming attr. and 	*/
				/* object class encountered 		*/
				if (strcmp(abbrv,D2_ATT_SEPRATOR)==0)
					break;

				/* no more naming attributes are expected */
				if (j == NO_NAM_ATTR)
					break;

				strcpy(srt_ent->nam_attr[j++], abbrv);
				}

			/* wrong format of naming attribute list */
			if ((strcmp(abbrv,D2_ATT_SEPRATOR) != 0) ||
			    (j == 0))
				goto form_error_exit;
			
			srt_ent->no_nam_attr = j;

			/* get the object class from tmp. att. val.	*/	
			/* object class must be there.			*/
			if (sscanf(at_value + k,"%s", srt_ent->obj_class) !=1)
				goto form_error_exit;

			}	/* end of for loop to read SRT rules.	*/

		*(signed32 *)(*srt + *no_srt) = *no_srt + D2_S_MORE; 

		/* sort SRT on integer value of rule number.	*/
		qsort((char *)*srt, (size_t)*no_srt, (size_t)(sizeof(Srt)),
			(int (*)(const void *, const void *)) d21_srt_ent_cmp);

		/* get the next attribute	*/
		continue;
		}

	/* attribute is OCT */
	if (d20_cmp_objid(&(at_info->d2_a_type), sizeof(D2_OCT),
		(Ob_id_string) D2_OCT))
		{
		oct_present = TRUE;
		*no_oct = at_info->d2_a_no_val;

		/* allocate space for OCT, extra entries and to store */
		/* oct entries limit 				      */
		if ((*oct = (Oct *)malloc(((*no_oct + D2_O_MORE) * 
					    sizeof(Oct)) + sizeof(signed32)))
		    == (Oct *)NULL)	
			{
   			free((char *) pb.d21_3res);

  			d20_errcode(ret_code, D2_TEMP_ERR, D2_SYS_ERR, 
							DS_ALLOC_ERROR);
  			d20_error(&head, D2_TEMP_ERR, D2_SYS_ERR, 
							DS_ALLOC_ERROR);
			/* d21_l03_trace_err(bind_id, f_name, f_exit, &head);*/
			d21_reset_return_vals(no_srt, srt, no_oct, oct, 
						   	no_at, at);
			return(D2_ERROR);
			}


		/* get all the OCT entries from OCT attribute */
		for (i=0,oct_ent=*oct,d2_a_val=at_info->d2_a_val;
					 	i<*no_oct; 
						i++,oct_ent++,d2_a_val++)
			{
			/* check for length of the OCT attribute value */
			if (d2_a_val->d2_a_v_len > (signed16)D2_L_OCT_ATT)
				goto form_error_exit;

			/*copy next value in temporary att. val array*/
			strncpy(at_value,
				(char *)d2_a_val->d2_at_value.d2_oct_a_v,
				d2_a_val->d2_a_v_len);

			*(at_value + d2_a_val->d2_a_v_len) = D2_INFO_DELIM;
			*(at_value + d2_a_val->d2_a_v_len + 1) = D2_EOS;

			/* read all information from tmp. att. val. 	   */
			/* except the super classes, auxiliary classes and */
			/* attributes 					   */
			info_cnt = sscanf(at_value,D2_FORMAT_OCT,
						oct_ent->obj_abbrv,
						oct_ent->obj_id,
						oct_ent->obj_name,
						&(oct_ent->obj_class_kind),
						&(oct_ent->obj_file_nr));

			if (info_cnt != D2_NO_INFO_OCT)
				goto form_error_exit;

			/* skip D2_NO_INFO_OCT no. of delimiters and */
			/* count the length of string read so far    */
			for (j=0,k=0;j<D2_NO_INFO_OCT;k++)
				if (at_value[k] == D2_INFO_DELIM)
					j++;

			if (at_value[k] == D2_INFO_DELIM)
				/* no super classes, set pointer to SEPRATOR */
				k++;

			/*get the super classes from tmp. att. val.	*/	
			j = 0;
			while (sscanf(at_value + k,"%s",abbrv) == 1)
				{
				k += strlen(abbrv) + 1;
				
				/* seperator between super classes and  */
				/* auxiliary object classes encountered */
				if (strcmp(abbrv,D2_ATT_SEPRATOR)==0)
					break;

				/* no more super classes are expected 	*/
				if (j == NO_SUP_CLASS)
					break;

				strcpy(oct_ent->sup_class[j++], abbrv);
				}

			/* wrong format of super class list */
			if (strcmp(abbrv,D2_ATT_SEPRATOR) != 0)
				goto form_error_exit;
			
			oct_ent->no_sup_class = j;

			if (at_value[k] == D2_INFO_DELIM)
				/* no aux. classes, set pointer to SEPRATOR */
				k++;

			/*get the aux. object classes from tmp. att. val.*/	
			j = 0;
			while (sscanf(at_value + k,"%s",abbrv) == 1)
				{
				k += strlen(abbrv) + 1;
				
				/* seperator between aux. object classes and */
				/* mandatory attributes encountered 	     */
				if (strcmp(abbrv,D2_ATT_SEPRATOR)==0)
					break;

				/* no more aux. object classes are expected  */
				if (j == NO_AUX_CLASS)
					break;

				strcpy(oct_ent->aux_class[j++], abbrv);
				}

			/* wrong format of aux. object class list */
			if (strcmp(abbrv,D2_ATT_SEPRATOR) != 0)
				goto form_error_exit;
			
			oct_ent->no_aux_class = j;

			if (at_value[k] == D2_INFO_DELIM)
				/* no mand. attr. set pointer to SEPRATOR */
				k++;

			/*get the mandatory attr. from tmp. att. val.*/	
			j = 0;
			while (sscanf(at_value + k,"%s",abbrv) == 1)
				{
				k += strlen(abbrv) + 1;
				
				/* seperator between mandatory and */
				/* optional attributes encountered */
				if (strcmp(abbrv,D2_ATT_SEPRATOR)==0)
					break;

				/* no more attributes are */
				/* expected 		  */
				if (j == NO_MAND_ATTR)
					break;

				strcpy(oct_ent->must_attr[j++], abbrv);
				}

			/* wrong format of mandatory attribute list */
			if (strcmp(abbrv,D2_ATT_SEPRATOR) != 0)
				goto form_error_exit;
			
			oct_ent->no_must_attr = j;

			/*get the optional attr. from tmp. att. val.*/	
			j = 0;
			while (sscanf(at_value + k,"%s",abbrv)==1)
				{
				k += strlen(abbrv) + 1;
					
				/* no more attributes are */
				/* expected 		  */
				if (j == NO_OPT_ATTR)
					goto form_error_exit;

				strcpy(oct_ent->may_attr[j++], abbrv);
				}

			oct_ent->no_may_attr = j;

			if ((oct_ent->no_must_attr == 0) && 
			    (oct_ent->no_may_attr == 0)) 
				goto form_error_exit;

			} /* end of for loop to read OCT entries.	*/

		*(signed32 *)(*oct + *no_oct) = *no_oct + D2_O_MORE; 

		/* sort OCT on object class abbreviation.	*/
		qsort((char *)*oct, (size_t)*no_oct, (size_t)(sizeof(Oct)),
			(int (*)(const void *, const void *)) d21_oct_ent_cmp);

		continue;
		}

	/* attribute is AT */
	if (d20_cmp_objid(&(at_info->d2_a_type), sizeof(D2_AT),
		(Ob_id_string) D2_AT))
		{
		at_present = TRUE;
		*no_at = at_info->d2_a_no_val;

		/* allocate space for AT, extra entries and to store */
		/* at entries limit 				      */
		if ((*at = (At *)malloc(((*no_at + D2_A_MORE) * 
					    sizeof(At)) + sizeof(signed32)))
		    == (At *)NULL)	
			{
    			free((char *) pb.d21_3res);

  			d20_errcode(ret_code, D2_TEMP_ERR, D2_SYS_ERR, 
								DS_ALLOC_ERROR);
  			d20_error(&head, D2_TEMP_ERR, D2_SYS_ERR, 
								DS_ALLOC_ERROR);
			/* d21_l03_trace_err(bind_id, f_name, f_exit, &head);*/
			d21_reset_return_vals(no_srt, srt, no_oct, oct, 
						   	no_at, at);
			return(D2_ERROR);
			}


		/* get all the AT entries from AT attribute */
		for (i=0,at_ent=*at,d2_a_val=at_info->d2_a_val;
					 	i<*no_at; 
						i++,at_ent++,d2_a_val++)
			{
			/* check for length of the AT attribute value */
			if (d2_a_val->d2_a_v_len > (signed16)D2_L_AT_ATT)
				goto form_error_exit;

			/*copy next value in temporary att. val array*/
			strncpy(at_value,
				(char *)d2_a_val->d2_at_value.d2_oct_a_v,
				d2_a_val->d2_a_v_len);
/*
			*(at_value + d2_a_val->d2_a_v_len) = D2_INFO_DELIM;
			*(at_value + d2_a_val->d2_a_v_len + 1) = D2_EOS;
*/
			*(at_value + d2_a_val->d2_a_v_len) = D2_EOS;

			/* read all information from tmp. att. val. */
			info_cnt = sscanf(at_value,D2_FORMAT_AT,
						at_ent->att_abbrv,
						at_ent->att_obj_id,
						at_ent->att_name,
						&(at_ent->att_lower),
						&(at_ent->att_upper),
						&(at_ent->att_no_recur),
						&(at_ent->att_syntax),
						&(at_ent->att_phon),
						&(at_ent->att_acl),
						&(at_ent->att_ind_lev));

			if (info_cnt != D2_NO_INFO_AT)
				goto form_error_exit;

			} /* end of for loop to read AT entries.	*/

		*(signed32 *)(*at + *no_at) = *no_at + D2_A_MORE; 

		/* sort AT on attribute abbreviation.	*/
		qsort((char *)*at, (size_t)*no_at, (size_t)(sizeof(At)),
			(int (*)(const void *, const void *)) d21_at_ent_cmp);

		continue;
		}

	/* unwanted attribute present in the result */
	other_present = TRUE;
	break;
	}

if ((srt_present == FALSE) || (oct_present == FALSE) || 
    (at_present == FALSE) || (other_present == TRUE))
	{
form_error_exit:
    	free((char *) pb.d21_3res);
  	d20_errcode(ret_code, D2_TEMP_ERR, D2_SYS_ERR, DS_FORM_ERR);
  	d20_error(&head, D2_TEMP_ERR, D2_SYS_ERR, DS_FORM_ERR);
	/* d21_l03_trace_err(bind_id, f_name, f_exit, &head); */
	d21_reset_return_vals(no_srt, srt, no_oct, oct, no_at, at);
	return(D2_ERROR);
	}

free((char *) pb.d21_3res);
return(D2_NOERROR);

}                               /*  d21_21_rdsh()               */

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d21_reset_return_vals()				      */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        14.01.91					      */
/*								      */
/* SYNTAX:      d21_reset_return_vals(no_srt, srt, no_oct, oct,       */ 
/*						   no_at,at)	      */
/*								      */
/*		signed32  *no_srt;				      */
/*		Srt	 **srt;				      	      */
/*		signed32  *no_oct;				      */
/*		Oct	 **oct;				      	      */
/*		signed32  *no_at;			      	      */
/*		At       **at;				      	      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function is called in case any error has occurred*/
/*		during the schema read operation. This function frees */
/*		any space allocated in read operation and return      */
/*		pointers are set to NULL and return integer values are*/
/*		set to 0.					      */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*		Srt	**srt;				      	      */
/*		Oct	**oct;				      	      */
/*		At      **at;				      	      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*		signed32  *no_srt;				      */
/*		Srt	 **srt;				      	      */
/*		signed32  *no_oct;				      */
/*		Oct	 **oct;				      	      */
/*		signed32  *no_at;			      	      */
/*		At       **at;				      	      */
/*								      */
/* RETURNVALUE:							      */
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

 static void d21_reset_return_vals(
   signed32  *no_srt,
   Srt      **srt,
   signed32  *no_oct,
   Oct      **oct,
   signed32  *no_at,
   At       **at)

{                               /*  d21_reset_return_vals()           */

/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                  D E C L A R A T I O N S                           */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

if (*srt != (Srt *)NULL)
	free((char *)*srt);

if (*oct != (Oct *)NULL)
	free((char *)*oct);

if (*at != (At *)NULL)
	free((char *)*at);

*no_srt = 0;
*srt = (Srt *)NULL;
*no_oct = 0;
*oct = (Oct *)NULL;
*no_at = 0;
*at = (At *)NULL;

}		/* end of d21_reset_return_vals()	*/

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d21_srt_ent_cmp()				      */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        14.01.91					      */
/*								      */
/* SYNTAX:      signed32 d21_srt_ent_cmp(srt_ent1, srt_ent2)	      */
/*		Srt	*srt_ent1;				      */
/*		Srt	*srt_ent2;				      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function defines the criteria for sorting the    */
/*		SRT entries.					      */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*		Srt	*srt_ent1;				      */
/*		Srt	*srt_ent2;				      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*		-1	srt_ent1 < srt_ent2			      */
/*		 0	srt_ent1 = srt_ent2			      */
/*		 1	srt_ent1 > srt_ent2			      */
/*								      */
/* STANDARD-HEADER: None                                              */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*			strcmp()				      */
/*								      */
/* NOTES:                                                             */
/*								      */
/*exoff ***************************************************************/

 static signed32  d21_srt_ent_cmp(
   Srt *srt_ent1,
   Srt *srt_ent2)

{                               /*  d21_srt_ent_cmp()           */

/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                  D E C L A R A T I O N S                           */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

if (srt_ent1->rule_nr < srt_ent2->rule_nr)
	return(-1);

if (srt_ent1->rule_nr == srt_ent2->rule_nr)
	return(0);

if (srt_ent1->rule_nr > srt_ent2->rule_nr)
	return(1);

}		/* end of d21_srt_ent_cmp()		*/


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d21_oct_ent_cmp()				      */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        14.01.91					      */
/*								      */
/* SYNTAX:      signed32 d21_oct_ent_cmp(oct_ent1, oct_ent2)	      */
/*		Oct	*oct_ent1;				      */
/*		Oct	*oct_ent2;				      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function defines the criteria for sorting the    */
/*		OCT entries.					      */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*		Oct	*oct_ent1;				      */
/*		Oct	*oct_ent2;				      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*		-1	oct_ent1 < oct_ent2			      */
/*		 0	oct_ent1 = oct_ent2			      */
/*		 1	oct_ent1 > oct_ent2			      */
/*								      */
/* STANDARD-HEADER: None                                              */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*			strcmp()				      */
/*								      */
/* NOTES:                                                             */
/*								      */
/*exoff ***************************************************************/

 static signed32 d21_oct_ent_cmp(
   Oct *oct_ent1,
   Oct *oct_ent2)

{                               /*  d21_oct_ent_cmp()           */

/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                  D E C L A R A T I O N S                           */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

if (strcmp(oct_ent1->obj_abbrv, oct_ent2->obj_abbrv) < 0)
	return(-1);

if (strcmp(oct_ent1->obj_abbrv, oct_ent2->obj_abbrv) == 0)
	return(0);

if (strcmp(oct_ent1->obj_abbrv, oct_ent2->obj_abbrv) > 0)
	return(1);

}		/* end of d21_oct_ent_cmp()		*/


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d21_at_ent_cmp()				      */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        14.01.91					      */
/*								      */
/* SYNTAX:      signed32 d21_at_ent_cmp(at_ent1, at_ent2)      	      */
/*		At	*at_ent1;			              */
/*		At	*at_ent2;			              */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function defines the criteria for sorting the    */
/*		AT entries.					      */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*		At	*at_ent1; 				      */
/*		At	*at_ent2; 				      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*		-1	at_ent1 < at_ent2			      */
/*		 0	at_ent1 = at_ent2			      */
/*		 1	at_ent1 > at_ent2			      */
/*								      */
/* STANDARD-HEADER: None                                              */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*			strcmp()				      */
/*								      */
/* NOTES:                                                             */
/*								      */
/*exoff ***************************************************************/

 static signed32 d21_at_ent_cmp(
   At *at_ent1,
   At *at_ent2)

{                               /*  d21_at_ent_cmp()           */

/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                  D E C L A R A T I O N S                           */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

if (strcmp(at_ent1->att_abbrv, at_ent2->att_abbrv) < 0)
	return(-1);

if (strcmp(at_ent1->att_abbrv, at_ent2->att_abbrv) == 0)
	return(0);

if (strcmp(at_ent1->att_abbrv, at_ent2->att_abbrv) > 0)
	return(1);

}		/* end of d21_at_ent_cmp()		*/

