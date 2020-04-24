/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20sadmsch.c,v $
 * Revision 1.1.10.2  1996/02/18  19:39:29  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:20:30  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:13:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:46  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:04:30  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:05:15  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:57:18  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:20:20  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  09:14:54  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:17:03  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:47:17  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:01:07  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  13:55:48  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  15:03:16  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:42:38  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:10:57  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20sadmsch.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:39:29 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODUL                                               */
/*								      */
/* NAME         : d20_store_admsch [store administration schema ]     */
/*								      */
/* AUTHOR       : SANJAY JAIN (Siemens India) 			      */
/* DATE         : 26.11.90 					      */
/*								      */
/* COMPONENT    : 						      */
/*								      */
/* DOK.-NR.	: 						      */
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
#include <stdio.h>
#include <string.h>

#include <dce/d2dir.h>
#include <gdsext.h>
#include <d2adm.h>
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


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_store_admsch() 				      */
/*								      */
/* AUTHOR:      SANJAY JAIN (Siemens India)			      */
/* DATE:        05.11.90					      */
/*								      */
/* SYNTAX:	d2_ret_val d20_store_admsch(fid, no_admat, admat,     */
/*				no_admoct, admoct, no_admsrt, admsrt) */
/*		FILE	*fid;					      */
/*		signed32 no_admat,no_admoct,no_admsrt;		      */
/*		D20_at	*admat;			      		      */
/*		D20_oct	*admoct;			      	      */
/*		D20_srt	*admsrt;		      		      */
/*								      */
/* DESCRIPTION:                                                       */
/*		This function stores the administration schema in     */
/*		a file.						      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		fid		pointer to admscheme file descriptor  */
/*		no_admat	no. of elements in AT 		      */
/*		admat		pointer to AT			      */
/*		no_admoct	no. of elements in admin OCT 	      */
/*		admoct		pointer to admin OCT 		      */
/*		no_admsrt	no. of elements in SRT 		      */
/*		admsrt		pointer to SRT			      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*								      */
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

 d2_ret_val d20_store_admsch(
   FILE     *fid,
   signed32  no_admat,
   D20_at   *admat,
   signed32  no_admoct,
   D20_oct  *admoct,
   signed32  no_admsrt,
   D20_srt  *admsrt)

{

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

signed32 i,j;
signed32 no_attr_abbrv, no_sup_class, no_obj_class;
signed32 par_rule_nr;
D20_oct	*tmp_admobj;
D20_srt	*tmp_srtent;
At      *tmp_at;
char	prt_obj_id_str[D2_OBJ_IDL_MAX + 1];

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

/**************** write AT in admsch_file ********************/

/* write AT header */
fprintf(fid,ADMAT_HEAD,no_admat);

/* for all attribute entries */
for (i=0;i<no_admat;i++,admat++)
	{
	tmp_at = &(admat->at);
	fprintf(fid,ADMAT_FORMAT,tmp_at->att_abbrv,tmp_at->att_obj_id,
				 tmp_at->att_name, tmp_at->att_lower,
				 tmp_at->att_upper,tmp_at->att_no_recur,
				 tmp_at->att_syntax,tmp_at->att_acl,
				 tmp_at->att_ind_lev, tmp_at->att_phon);
	}

/**************** write OCT in admsch_file ********************/

/* compute total no. of attribute names and superclass names used in OCT */

no_attr_abbrv = 0;
no_sup_class = 0;
for (i=0,tmp_admobj=admoct;i<no_admoct;i++,tmp_admobj++)
	{
	no_attr_abbrv += tmp_admobj->no_must_attr + 
			 tmp_admobj->no_may_attr;
	no_sup_class += tmp_admobj->no_sup_class;
	}

/* write OCT header */
fprintf(fid, ADMOCT_HEAD, no_admoct, no_attr_abbrv, no_sup_class);

/* for all object class entries */
for (i=0;i<no_admoct;i++,admoct++)
	{
	if (admoct->obj_id.d2_typ_len == 0)
		strcpy(prt_obj_id_str, D2_INVALID_PRT_OBJ_ID);
	else
		if (d27_301_oid_str(&(admoct->obj_id),prt_obj_id_str)==D2_ERROR)
			return(D2_ERROR);

	fprintf(fid,ADMOCT_FORMAT,admoct->obj_abbrv,
				  prt_obj_id_str,
				  admoct->obj_name,
				  admoct->no_sup_class,
				  admoct->no_aux_class,
				  admoct->no_must_attr,
				  admoct->no_may_attr);

	/* write super classes.		*/
	fprintf(fid,"Scls: ");

	for (j=0;j<admoct->no_sup_class;j++)
		fprintf(fid,"%s ",admoct->sup_class[j]->obj_abbrv);

	/* write auxiliary classes */
	fprintf(fid,"Acls: ");

	for (j=0;j<admoct->no_aux_class;j++)
		fprintf(fid,"%s ",admoct->aux_class[j]->obj_abbrv);

	/* write mandatory attributes */
	fprintf(fid,"Mand: ");

	for (j=0;j<admoct->no_must_attr;j++)
		fprintf(fid,"%s ",admoct->must_attr[j]->at.att_abbrv);

	/* write optional attributes */
	fprintf(fid,"Opt: ");

	for (j=0;j<admoct->no_may_attr;j++)
		fprintf(fid,"%s ",admoct->may_attr[j]->at.att_abbrv);

	fprintf(fid,"\n");
	}

/**************** write SRT in admsch_file ********************/

/* compute total no. of object class names used in SRT */

no_obj_class = 0;
for (i=0,tmp_srtent=admsrt;i<no_admsrt;i++,tmp_srtent++)
	no_obj_class += tmp_srtent->no_obj_class;

/* write SRT header */
fprintf(fid,ADMSRT_HEAD,no_admsrt, no_obj_class);

/* for all structural rule entries */
for (i=0;i<no_admsrt;i++,admsrt++)
	{
	if (admsrt->par_rule == (D20_srt *)NULL)
		par_rule_nr = 0;
	else
		par_rule_nr = admsrt->par_rule->rule_nr;

	fprintf(fid,ADMSRT_FORMAT,admsrt->rule_nr,
				  par_rule_nr,
				  admsrt->no_nam_attr,
				  admsrt->no_obj_class);

	/* write naming attributes */
	fprintf(fid,"Nam-Attr: ");

	for (j=0;j<admsrt->no_nam_attr;j++)
		fprintf(fid,"%s ",admsrt->nam_attr[j]->at.att_abbrv);

	/* write object classes */
	fprintf(fid,"Obj_Cls: ");

	for (j=0;j<admsrt->no_obj_class;j++)
		fprintf(fid,"%s ",admsrt->obj_class[j]->obj_abbrv);

	fprintf(fid,"\n");
	}

return(D2_NOERROR);
}
