/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20modattr.c,v $
 * Revision 1.1.725.2  1996/02/18  19:41:08  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:21:53  marty]
 *
 * Revision 1.1.725.1  1995/12/08  15:17:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:52:02  root]
 * 
 * Revision 1.1.723.4  1994/06/21  14:43:41  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:24:30  marrek]
 * 
 * Revision 1.1.723.3  1994/05/10  15:49:40  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:12:03  marrek]
 * 
 * Revision 1.1.723.2  1994/03/23  15:05:28  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:10:03  keutel]
 * 
 * Revision 1.1.723.1  1994/02/22  15:58:43  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:30:34  marrek]
 * 
 * Revision 1.1.721.2  1993/10/14  16:46:29  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  16:44:16  keutel]
 * 
 * Revision 1.1.721.1  1993/10/13  17:29:38  keutel
 * 	October 1993 code drop
 * 	[1993/10/11  07:35:20  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  10:14:07  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:31:16  marrek]
 * 
 * Revision 1.1.4.5  1993/01/29  08:36:49  marrek
 * 	Use function key.
 * 	[1993/01/28  10:37:16  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  18:08:58  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:04:43  bbelch]
 * 
 * Revision 1.1.4.3  1992/11/30  09:15:03  marrek
 * 	Initialize *spec_attr = D2_FALSE;
 * 	[1992/11/30  09:13:19  marrek]
 * 
 * Revision 1.1.4.2  1992/11/27  13:15:16  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  14:11:21  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:56:06  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:16:25  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20modattr.c,v $ $Revision: 1.1.725.2 $ $Date: 1996/02/18 19:41:08 $";
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
/* NAME         : modattr.c   [Administrations-Schnittstelle]         */
/*								      */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 01.02.88                                            */
/*								      */
/* KOMPONENTE	: DS						      */
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
/* HISTORY 	:						      */
/*								      */
/* Vers.Nr. |  Date   |	 Updates    		       | KZ | CR# FM# */
/*      0.1 | 01.02.88| Erstellung                     | ws |         */
/*								      */
/*datoff **************************************************************/

/* Includes */

#include <gds.h>
#include <stdio.h>   /* required for the def. of NULL  */
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <gdsext.h>
#include <d2adm.h>
#include <d2msk.h>
#include <cget.h>
#include <d2spec_msk.h>
#include <d20proto.h>

/* Defines */

#define HEX                     "x'"

/* Variables */

extern int errno;
extern signed32  rbatch;

extern D20_omask d20_8omask[];         /* Input fields for mask 8     */
extern char      *d20_8ifields[];      /* Mask 8                      */
extern char      *d20_8hflds[];        /* help fields                 */

extern D20_omask d20_msgomask[];       /* Input fields for message    */
				       /* mask                        */
extern D20_omask d20_qomask[];         /* Input fields for error mask */
extern char      *d20_errtab[];        /* Error mask                  */
extern char      *d20_err_table[];     /* Error mask                  */

extern char      *d20_bool[];

extern D20_omask d20_dnsrepomask[];
extern char      *d20_dnsrepifields[];
extern D20_omask d20_dnscellomask[];
extern char      *d20_dnscellifields[];
extern D20_omask d20_ttxidomask[];
extern char      *d20_ttxidifields[];
extern D20_omask d20_telexomask[];
extern char      *d20_telexifields[];
extern D20_omask d20_faxomask[];
extern char      *d20_faxifields[];
extern D20_omask d20_t61omask[];
extern char      *d20_t61ifields[];

extern   D2_OR_address   array_oradd[];
extern   D2_OR_name      array_orname[];
extern	 D2_str_list	 post_str_list[][D20_MAX_NB_ITEM];

extern   char		 	mog_space[];
extern   D2_dl_submit_perm      array_dlsp[];
extern 	Bool	d20_printable;

/* Static functions */
static  signed16 ds0_fetch_attr(
	D20_srt		  *srt_ent,
	D20_oct		  *oct_ent,
	D20_at 		  **at_ent,
	Ds_at_entry       *attr_entry,
	Bool              *spec_attr);
static  signed16 ds0_check_input(
	D20_oct		 *oct_ent,
	D20_at		 *at_ent,
	Ds_at_entry      *attr_entry);
static  signed16 ds0_modify_mode(signed16 nr_recur, signed16 *change_mode);
static  signed32 ds0_handle_spec_attr(
	D2_obj_id          at_type,
	unsigned16         rep,
	char              *a_name,
	D20_oct           *oct_ent,
	Ds_at_entry       *attr_entry);

/********* LOCAL DATA ******/
static 	Bool	old_present;
static 	Bool	new_present;


/*inon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : ds0_fetch_attr()                                      */
/*								      */
/* AUTHOR:      Schmid (D AP 11)                                      */
/* DATE:        04.03.88                                              */
/*								      */
/* SYNTAX:      signed16 ds0_fetch_attr(srt_ent,oct_ent,at_ent,       */
/*                                   attr_entry, spec_attr)           */
/*								      */
/*		D20_srt		 *srt_ent;			      */	
/*		D20_oct		 *oct_ent;			      */	
/*              D20_at 		 **at_ent;                            */
/*              Ds_at_entry       *attr_entry;                        */
/*              Bool              *spec_attr;                         */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*	This function calls the function which handles selection of   */
/*	an attribute.						      */
/*	It calls functions to handle special attributes.	      */
/*	For normal attributes it reads the old value.		      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		D20_srt		  *srt_ent;	pointer to relevant   */
/*					        structural rule entry */
/*		D20_oct		  *oct_ent;	pointer to relevant   */
/*					        object class entry    */
/*              Ds_at_entry       *attr_entry;                        */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              D20_at           **at_ent;     actual pointer to AT   */
/*              Bool              *spec_attr;  flag indicating if     */
/*                                             special attributes are */
/*                                             modified               */
/*								      */
/* RETURNVALUE:							      */
/*								      */
/*       0      :       No error occurred                             */
/*      -1      :       Error occurred                                */
/*								      */
/* STANDARD-HEADER: None                                              */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*              Bool              old_present;  old attr. val. is     */
/*						present or not.       */
/*              Bool              new_present;  new attr. val. is     */
/*						prsent or not.        */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                      ds0_check_input()                             */
/*                      ds4_b02_fill_attr()                           */
/*                      d20_handlemask()                              */
/*                      d20_get_attribute()                           */
/*                      strlen()                                      */
/*                      strnmp()                                      */
/*                      strcpy()                                      */
/*								      */
/* NOTES:                                                             */
/*								      */
/*inoff ***************************************************************/

static  signed16 ds0_fetch_attr(
	D20_srt		  *srt_ent,
	D20_oct		  *oct_ent,
	D20_at 		  **at_ent,
	Ds_at_entry       *attr_entry,
	Bool              *spec_attr)
{                               /*  ds0_fetch_attr                    */
signed32 no_selected_attrs;
unsigned16 rep;
D20_at	**attr_ptr_space;


*spec_attr = FALSE;

/* get the attribute name.		*/
if (d20_handle_attr_list(DS_MODIFY_ATTRIBUTE, srt_ent, oct_ent, &attr_ptr_space,
						&no_selected_attrs) == D2_ERROR)
	{
	if (attr_ptr_space != (D20_at **)NULL)
		free((char *)attr_ptr_space);

	return(D2_ERROR);
	}

if (no_selected_attrs == 0)
	{
	if (attr_ptr_space != (D20_at **)NULL)
		free((char *)attr_ptr_space);

	return(D2_ERROR);
	}

*at_ent = *attr_ptr_space;
free((char *)attr_ptr_space);

if ((rep = (unsigned16) d27_401_syntax_to_rep((signed16)(*at_ent)->at.att_syntax)) == D2_T61_PR)
	if (d20_printable == TRUE)
		rep = D2_PRINTABLE;
	else
		rep = D2_T61;

/*  attribute is ACL                  */
if (d20_cmp_objid(&((*at_ent)->obj_id), sizeof(D2_ACL), (Ob_id_string) D2_ACL))
	{
	*spec_attr = TRUE;
	return(ds0_507_handle_acl(DS_MODIFY_ATTRIBUTE, TRUE, 0, 
				 (char *)NULL));
	}

if (d20_cmp_objid(&((*at_ent)->obj_id), sizeof(D2_CDS_CELL),
		(Ob_id_string) D2_CDS_CELL) ||
    d20_cmp_objid(&((*at_ent)->obj_id), sizeof(D2_CDS_REPLICA),
		(Ob_id_string) D2_CDS_REPLICA))
	{
	*spec_attr = TRUE;
	return(ds0_handle_spec_attr((*at_ent)->obj_id, rep,
		     (*at_ent)->at.att_name, oct_ent, attr_entry));
	}

switch ((int)rep)
  	{
	case D2_PSAP_STX:
  	case D2_TTXID_STX:
  	case D2_FAXNR_STX:
  	case D2_TLXNR_STX:
  	case D2_T61_LIST:
	case D2_T61_PR_LIST:
  	case D2_MHS_ORADR_STX:
  	case D2_MHS_ORNAME_STX:
  	case D2_MHS_DLSP_STX:
		*spec_attr = TRUE;
		return(ds0_handle_spec_attr((*at_ent)->obj_id, rep, 
		(*at_ent)->at.att_name, oct_ent, attr_entry));
		break;
  	}

/* for normal attributes when oct_ent is known, (Modify for single 	*/
/* object) read old attribute value 					*/

if (oct_ent != D20_UNKNOWN)
	{
	attr_entry->attr_id.d2_type = (*at_ent)->obj_id.d2_type;
	attr_entry->attr_id.d2_typ_len = (*at_ent)->obj_id.d2_typ_len;

	ds4_b02_fill_attr(D20_ASTART | D20_AINSERT | D20_AEND | D20_ATYPE_ONLY, 
							attr_entry, 1);

	/*  attribute could not be read       */
	if (ds4_b11_call_directory(DS_READ_PROPERTY))
		/*  return(D2_ERROR)                  */
		return(D2_ERROR);
	}

return(D2_NOERROR);

}                               /*  ds0_fetch_attr                    */



/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : ds0_disp_old_val()                                    */
/*								      */
/* AUTHOR:      Schmid (D AP 11)                                      */
/* DATE:        06.03.88                                              */
/*								      */
/* SYNTAX:      void ds0_disp_old_val(len, val, syntax)               */
/*								      */
/* DESCRIPTION: This function copies the property value into the      */
/*              mask input fields.                                    */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              signed16         len;       length of attribute       */
/*              Octet_string     val;       pointer to attribute      */
/*                                          value                     */
/*		signed16	 syntax;    attribute value syntax    */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE: None                                                  */
/*								      */
/* STANDARD-HEADER: None                                              */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                  ds4_004_octncpy()                                 */
/*                  asc_form()                                        */
/*                  hex_asc()                                         */
/*								      */
/* NOTES:                                                             */
/*								      */
/*exoff ***************************************************************/

void ds0_disp_old_val(signed16 len, Octet_string val, signed16 syntax)
{                               /*  ds0_disp_old_val                  */

 signed16        pos;           /* Property position at the           */
				/* beginning of the field in the mask */

 char         prval[2 * D20_ATVAL_LEN * D20_T61_CONV_FACT];
 Octet_string str;
 signed16     max_pos = D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT;
 signed16     prlen;
 Bool		convert;


*d20_8ifields[DS_MA1OLD_VAL] = D2_EOS;
*d20_8ifields[DS_MA2OLD_VAL] = D2_EOS;
*d20_8ifields[DS_MA1NEW_VAL] = D2_EOS;
*d20_8ifields[DS_MA2NEW_VAL] = D2_EOS;

switch(d27_401_syntax_to_rep(syntax))
	{
	case D2_T61_PR:
	case D2_DISTNAME:
		convert = TRUE;

		prlen = (len >= (D20_ATVAL_LEN * D20_T61_CONV_FACT)) ? 
			(D20_ATVAL_LEN * D20_T61_CONV_FACT - 1) : len;
		pos = 0;
		str = val;
		break;

	default:
		convert = FALSE;

		if (!asc_form((char *) val, len))
			{   
			prlen =                                                 
                        (len >= (((D20_ATVAL_LEN * D20_T61_CONV_FACT) - 3) / 2))
                        ? ((D20_ATVAL_LEN * D20_T61_CONV_FACT)-3) : (2 * len);  
                        hex_asc(prval,(char *) val,2*prlen);                    
			ds4_004_octncpy(
			(Octet_string) d20_8ifields[DS_MA1OLD_VAL],
			(Octet_string) HEX, 2);
                        pos = 2;                                                
                        str = (Octet_string) prval;                             
			}
		else
			{
			prlen = (len >= (D20_ATVAL_LEN * D20_T61_CONV_FACT)) ? 
				(D20_ATVAL_LEN * D20_T61_CONV_FACT - 1) : len;
			pos = 0;
			str = val;
			} 

		break;
	}

/* set convert flag for the old attribute value.	*/
d20_8omask[D20_83POS + 1].d20_conv = convert;
d20_8omask[D20_83POS + 2].d20_conv = convert;

/* set convert flag for the new attribute value.	*/
d20_8omask[D20_83POS + 4].d20_conv = convert;
d20_8omask[D20_83POS + 5].d20_conv = convert;

if (prlen > max_pos - pos - 1)
{   
  ds4_004_octncpy((Octet_string) d20_8ifields[DS_MA1OLD_VAL] + pos, str,
			max_pos - pos);
  prlen -= (max_pos - pos);
  ds4_004_octncpy((Octet_string) d20_8ifields[DS_MA2OLD_VAL],
			str + max_pos - pos, prlen);
  d20_8ifields[DS_MA2OLD_VAL][prlen] = '\'';
  d20_8ifields[DS_MA2OLD_VAL][prlen + 1] = D2_EOS;
  ds4_004_octncpy((Octet_string) d20_8ifields[DS_MA1NEW_VAL],
			(Octet_string) d20_8ifields[DS_MA1OLD_VAL], max_pos);
  ds4_004_octncpy((Octet_string) d20_8ifields[DS_MA2NEW_VAL],
			(Octet_string) d20_8ifields[DS_MA2OLD_VAL], max_pos);
}
else
{
  ds4_004_octncpy((Octet_string) d20_8ifields[DS_MA1OLD_VAL] + pos, str, prlen);
  d20_8ifields[DS_MA1OLD_VAL][pos + prlen] = '\'';
  d20_8ifields[DS_MA1OLD_VAL][pos + prlen + 1] = D2_EOS;
  ds4_004_octncpy((Octet_string) d20_8ifields[DS_MA1NEW_VAL],
			(Octet_string) d20_8ifields[DS_MA1OLD_VAL], max_pos);
} 

}                               /*  ds0_disp_old_val                  */


/*inon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : ds0_handle_spec_attr()                                */
/*								      */
/* AUTHOR:      Mueller V. (SNI AP 11)                                */
/* DATE:        22.04.91                                              */
/*								      */
/* SYNTAX:      signed16 ds0_handle_spec_attr()                       */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              D2_obj_id      at_type;                               */
/*              usigned16      rep;                                   */
/*              char          *a_name;  attribute name                */
/*              D20_oct       *oct_ent;                               */
/*              Ds_at_entry   *attr_entry;                            */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*              Ds_at_entry   *attr_entry;                            */
/*								      */
/* RETURNVALUE:							      */
/*								      */
/*       0      :       No error occurred                             */
/*      -1      :       Error occurred                                */
/*								      */
/* STANDARD-HEADER: None                                              */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                      ds4_b02_fill_attr()                           */
/*                      ds4_b11_call_directory()                      */
/*                      d20_get_attribute()                           */
/*								      */
/* NOTES:                                                             */
/*								      */
/*inoff ***************************************************************/

static  signed32 ds0_handle_spec_attr(
	D2_obj_id          at_type,
	unsigned16         rep,
	char              *a_name,
	D20_oct           *oct_ent,
	Ds_at_entry       *attr_entry)
{                               /*  ds0_handle_spec_attr              */
 char            spec_attr[2*D20_MAX_NO][(D20_MAX_LEN * D20_T61_CONV_FACT) + 1];
 D2_obj_id       obj_id[2];
 D2_ttxid        ttx_id[2];
 D2_faxnumber    fax[2];
 D2_telex_number telex[2];
 D2_T61_seq      t61[2];
 D2_str          d2_str[2*D20_MAX_NB_ITEM];
 byte            cell[2][D20_MAX_CELL];
 byte            replica[2][D20_MAX_REP];
 signed16        cell_len;
 signed16        replica_len;

 signed16        attr_mode;
 D2_a_type      attr_id;
 D2_a_value    *attr_val;
 signed32       fkt;
 signed16       mod_type;           /* type of modification    */
 d2_ret_val     ret_value;
 signed32       i, j;
 D2_str        *p_t61;
 Bool           menu;
 char           *mog_space_ptr;
 Ds_property    prty[2];
 char           psap_old_val[DS_PSAP_LEN];
 D2_str_list	*src_str_list;
 D2_str_list	*target_str_list;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

if ((ret_value = ds0_malloc_attr(2, spec_attr, &d2_str[0],
				    &ttx_id[0], &fax[0], &telex[0],
				    &t61[0], &obj_id[0])) == D2_ERROR)
	{
	return(D2_ERROR);
	}

ds0_assign_space_to_oradd();
ds0_assign_space_to_orname();
ds0_assign_space_to_post_add();

attr_entry[0].attr_id = at_type;
attr_entry[1].attr_id = at_type;
attr_entry[0].hex_use = attr_entry[1].hex_use = FALSE;
attr_entry[0].mod_type = attr_entry[1].mod_type = D2_REP_AV;
attr_entry[0].a_val.d2_a_rep = attr_entry[1].a_val.d2_a_rep = rep;
attr_entry[0].a_val.d2_a_v_len = attr_entry[1].a_val.d2_a_v_len = 0;

if (oct_ent == D20_UNKNOWN)
    {
    if (d20_cmp_objid(&(at_type), sizeof(D2_CDS_REPLICA),
			 (Ob_id_string) D2_CDS_REPLICA))
	   {
	   for (j=0; j < 2; j++)
		 {
		 fkt = d20_dnsrep(DS_ADD_ATTRIBUTE, a_name, replica[j],
				  &replica_len);

		 if ((fkt != FKTCR) && (fkt != FKTMENU))
		      {
		      return(D2_ERROR);
		      }
		 else
		      {
		      attr_entry[j].a_val.d2_at_value.d2_oct_a_v = replica[j];
		      attr_entry[j].a_val.d2_a_v_len = replica_len;
		      }
		 }
	   }

    if (d20_cmp_objid(&(at_type), sizeof(D2_CDS_CELL),
			(Ob_id_string) D2_CDS_CELL))
	   {
	   for (j=0; j < 2; j++)
		 {
		 fkt = d20_dnscell(DS_ADD_ATTRIBUTE, a_name, cell[j],
				  &cell_len);

		 if ((fkt != FKTCR) && (fkt != FKTMENU))
		      {
		      return(D2_ERROR);
		      }
		 else
		      {
		      attr_entry[j].a_val.d2_at_value.d2_oct_a_v = cell[j];
		      attr_entry[j].a_val.d2_a_v_len = cell_len;
		      }
		 }
	   }

    switch((int)rep)
      {
      case D2_T61_LIST:
	for (j=0; j < 2; j++)
	      {
	      fkt = d20_t61(DS_ADD_ATTRIBUTE, a_name, &t61[j]);
	      if ((fkt != FKTCR) && (fkt != FKTMENU))
		   {
		   return(D2_ERROR);
		   }
	      else
		   {
		   attr_entry[j].a_val.d2_at_value.d2_t61lst_a_v = &t61[j];
		   }
	      }
	break;
      case D2_T61_PR_LIST:
	for (j=0; j < 2; j++)
	      {
	      fkt = d20_postal(DS_ADD_ATTRIBUTE, a_name, &post_str_list[j][0]);
	      if ((fkt != FKTCR) && (fkt != FKTMENU))
		   {
		   return(D2_ERROR);
		   }
	      else
		   {
		   attr_entry[j].a_val.d2_at_value.d2_post_a_v = 
							&post_str_list[j][0];
		   }
	      }
	break;
      case D2_MHS_ORADR_STX:
	for (j=0; j < 2; j++)
	      {
	      fkt = d20_oradd(DS_ADD_ATTRIBUTE, a_name, &array_oradd[j], 
				NULL, NULL);
	      if ((fkt != FKTCR) && (fkt != FKTMENU))
		   {
		   return(D2_ERROR);
		   }
	      else
		   {
		   attr_entry[j].a_val.d2_at_value.d2_or_addr_av = 
							&array_oradd[j];
		   }
	      }
	 break;
      case D2_MHS_ORNAME_STX:
	for (j=0; j < 2; j++)
	      {
	      fkt = d20_orname(0, DS_ADD_ATTRIBUTE, a_name, &array_orname[j], 
				NULL, NULL);
	      if ((fkt != FKTCR) && (fkt != FKTMENU))
		   {
		   return(D2_ERROR);
		   }
	      else
		   {
		   attr_entry[j].a_val.d2_at_value.d2_or_name_av = 
							&array_orname[j];
		   }
	      }
	 break;
      case D2_MHS_DLSP_STX:
	for (j=0, mog_space_ptr = mog_space; j < 2; 
	     j++, mog_space_ptr += D20_ATVAL_LEN * D20_T61_CONV_FACT)
	      {
	      fkt = d20_dlsp(0, DS_ADD_ATTRIBUTE, a_name, &array_dlsp[j], 
			      (D2_dl_submit_perm *)NULL, &array_orname[j],
			      mog_space_ptr, (signed16 *)NULL); 

	      if ((fkt != FKTCR) && (fkt != FKTMENU))
		   {
		   return(D2_ERROR);
		   }
	      else
		   {
		   attr_entry[j].a_val.d2_at_value.d2_dl_s_p_av = 
							&array_dlsp[j];
		   }
	      }
	 break;
      }

    }
else
    {
				    /*  fill info structure               */
    ds4_b02_fill_attr(D20_ASTART | D20_AINSERT | D20_AEND | D20_ATYPE_ONLY,
		      attr_entry,1);
    if                              /*  attr could not be read            */
       (ds4_b11_call_directory(DS_READ_PROPERTY))
    {                               /*  return(D2_ERROR)                  */
      return(D2_ERROR);
    }
else if                       /*  attribute not found               */
	  (d20_get_attribute(D20_BEGIN, &attr_id, &attr_val))
    {                               /*  display error message             */
      d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab[D20_E48IDX];
      d20_handlemask(d20_qomask);
				    /*  return(D2_ERROR)                  */
      return(D2_ERROR);
    }
else
{                               /*  handle spec_attributes            */
      attr_mode = D20_BEGIN;
      fkt = FKTCR;
      ret_value = D2_NOERROR;
      menu = FALSE;
      for ( ; ; )
	    {
	    ret_value = d20_get_attribute(attr_mode, &attr_id, &attr_val);

	    if (ret_value != D2_NOERROR && fkt == FKTSCD)
		   {
		   d20_msgomask[D20_M1POS].d20_ioinfo = d20_errtab[D20_E03IDX];
		   d20_handlemask(d20_msgomask);
		   }

		       /* handle DNS-Replica attribute mask.           */
	    if (d20_cmp_objid(&(attr_id), sizeof(D2_CDS_REPLICA),
				 (Ob_id_string) D2_CDS_REPLICA))
		   {
		   replica_len = attr_val->d2_a_v_len;
		   (void) memcpy(replica[0], attr_val->d2_at_value.d2_oct_a_v,
				 replica_len);
		   attr_entry[0].a_val.d2_at_value.d2_oct_a_v =
				 attr_val->d2_at_value.d2_oct_a_v;
		   attr_entry[0].a_val.d2_a_v_len = attr_val->d2_a_v_len;

		   d20_dnsrepomask[D20_REPSCD].d20_fkt_id = D20_FKTX;
		   d20_dnsrepomask[D20_REPMOD].d20_fkt_id = D20_CSAY;
		   d20_dnsrepomask[D20_REPCHG].d20_fkt_id = D20_CGET;
		   fkt = d20_dnsrep(DS_MODIFY_ATTRIBUTE, a_name, replica[0],
				    &replica_len);
		   if (fkt == FKTSCD)
			{
			attr_mode = D20_CONTINUE;
			continue;
			}

		   if ((fkt != FKTCR) && (fkt != FKTMENU))
			{
			d20_dnsrepomask[D20_REPSCD].d20_fkt_id =
			d20_dnsrepomask[D20_REPMOD].d20_fkt_id =
			d20_dnsrepomask[D20_REPCHG].d20_fkt_id = D20_NO_OP;
			return(D2_ERROR);
			}
		   else
			{
			attr_entry[0].mod_type = attr_entry[1].mod_type =
			    (signed16) * ((signed32 *) d20_dnsrepifields[DS_REP_CHG]);
			if (attr_entry[0].mod_type == D2_DEL_AV)
				{
				attr_entry[0].a_val.d2_at_value.d2_oct_a_v = 
								replica[0];
				attr_entry[0].a_val.d2_a_v_len = replica_len;
				}
			else
				{
				attr_entry[1].a_val.d2_at_value.d2_oct_a_v = 
								replica[0];
				attr_entry[1].a_val.d2_a_v_len = replica_len;
				}

			d20_dnsrepomask[D20_REPSCD].d20_fkt_id =
			d20_dnsrepomask[D20_REPMOD].d20_fkt_id =
			d20_dnsrepomask[D20_REPCHG].d20_fkt_id = D20_NO_OP;
			break;
			}
		   }

		       /* handle DNS-Cell attribute mask.              */
	    if (d20_cmp_objid(&(attr_id), sizeof(D2_CDS_CELL),
			(Ob_id_string) D2_CDS_CELL))
		   {
		   cell_len = attr_val->d2_a_v_len;
		   (void) memcpy(cell[0], attr_val->d2_at_value.d2_oct_a_v,
				 cell_len);
		   attr_entry[0].a_val.d2_at_value.d2_oct_a_v =
				 attr_val->d2_at_value.d2_oct_a_v;
		   attr_entry[0].a_val.d2_a_v_len = attr_val->d2_a_v_len;

		   d20_dnscellomask[D20_DCELLSCD].d20_fkt_id = D20_FKTX;
		   d20_dnscellomask[D20_DCELLMOD].d20_fkt_id = D20_CSAY;
		   d20_dnscellomask[D20_DCELLCHG].d20_fkt_id = D20_CGET;
		   fkt = d20_dnscell(DS_MODIFY_ATTRIBUTE, a_name, cell[0],
				    &cell_len);
		   if (fkt == FKTSCD)
			{
			attr_mode = D20_CONTINUE;
			continue;
			}

		   if ((fkt != FKTCR) && (fkt != FKTMENU))
			{
			d20_dnscellomask[D20_DCELLSCD].d20_fkt_id =
			d20_dnscellomask[D20_DCELLMOD].d20_fkt_id =
			d20_dnscellomask[D20_DCELLCHG].d20_fkt_id = D20_NO_OP;
			return(D2_ERROR);
			}
		   else
			{
			attr_entry[0].mod_type = attr_entry[1].mod_type =
			    (signed16) * ((signed32 *) d20_dnscellifields[DS_DCELL_CHG]);
			if (attr_entry[0].mod_type == D2_DEL_AV)
				{
				attr_entry[0].a_val.d2_at_value.d2_oct_a_v = 
								cell[0];
				attr_entry[0].a_val.d2_a_v_len = cell_len;
				}
			else
				{
				attr_entry[1].a_val.d2_at_value.d2_oct_a_v = 
								cell[0];
				attr_entry[1].a_val.d2_a_v_len = cell_len;
				}

			d20_dnscellomask[D20_DCELLSCD].d20_fkt_id =
			d20_dnscellomask[D20_DCELLMOD].d20_fkt_id =
			d20_dnscellomask[D20_DCELLCHG].d20_fkt_id = D20_NO_OP;
			break;
			}
		   }

	    switch((int)attr_val->d2_a_rep)
	      {
	      case D2_TTXID_STX:
		   strncpy((char *)ttx_id[0].d2_terminal.d2_value,
			   (char *)attr_val->d2_at_value.d2_ttxid_a_v->
						       d2_terminal.d2_value,
			   attr_val->d2_at_value.d2_ttxid_a_v->
						       d2_terminal.d2_size);
		   ttx_id[0].d2_terminal.d2_size = attr_val->
			   d2_at_value.d2_ttxid_a_v->d2_terminal.d2_size;

		   if (attr_val->d2_at_value.d2_ttxid_a_v->d2_controls.d2_size != 0)
			   {
			   strncpy((char *)ttx_id[0].d2_controls.d2_value,
				   (char *)attr_val->
				       d2_at_value.d2_ttxid_a_v->
				       d2_controls.d2_value,
				   attr_val->d2_at_value.d2_ttxid_a_v->
				       d2_controls.d2_size);
			   ttx_id[0].d2_controls.d2_size =
			       attr_val->d2_at_value.d2_ttxid_a_v->
						       d2_controls.d2_size;
			   }
		   else
			   ttx_id[0].d2_controls.d2_size = 0;

		   if (attr_val->d2_at_value.d2_ttxid_a_v->d2_graphics.d2_size != 0)
			   {
			   strncpy((char *)ttx_id[0].d2_graphics.d2_value,
				   (char *)attr_val->
				       d2_at_value.d2_ttxid_a_v->
				       d2_graphics.d2_value,
				   attr_val->d2_at_value.d2_ttxid_a_v->
				       d2_graphics.d2_size);
			   ttx_id[0].d2_graphics.d2_size =
			       attr_val->d2_at_value.d2_ttxid_a_v->
						       d2_graphics.d2_size;
			   }
		   else
			   ttx_id[0].d2_graphics.d2_size = 0;

		   if (attr_val->d2_at_value.d2_ttxid_a_v->d2_miscel.d2_size != 0)
			   {
			   strncpy((char *)ttx_id[0].d2_miscel.d2_value,
				   (char *)attr_val->
				       d2_at_value.d2_ttxid_a_v->
				       d2_miscel.d2_value,
				   attr_val->d2_at_value.d2_ttxid_a_v->
				       d2_miscel.d2_size);
			   ttx_id[0].d2_miscel.d2_size =
			       attr_val->d2_at_value.d2_ttxid_a_v->
						       d2_miscel.d2_size;
			   }
		   else
			   ttx_id[0].d2_miscel.d2_size = 0;

		   if (attr_val->d2_at_value.d2_ttxid_a_v->d2_pages.d2_size != 0)
			   {
			   strncpy((char *)ttx_id[0].d2_pages.d2_value,
				   (char *)attr_val->
				       d2_at_value.d2_ttxid_a_v->
				       d2_pages.d2_value,
				   attr_val->d2_at_value.d2_ttxid_a_v->
				       d2_pages.d2_size);
			   ttx_id[0].d2_pages.d2_size =
			       attr_val->d2_at_value.d2_ttxid_a_v->
						       d2_pages.d2_size;
			   }
		   else
			   ttx_id[0].d2_pages.d2_size = 0;

		   if (attr_val->d2_at_value.d2_ttxid_a_v->d2_privates.d2_size != 0)
			   {
			   strncpy((char *)ttx_id[0].d2_privates.d2_value,
				   (char *)attr_val->
				       d2_at_value.d2_ttxid_a_v->
				       d2_privates.d2_value,
				   attr_val->d2_at_value.d2_ttxid_a_v->
				       d2_privates.d2_size);
			   ttx_id[0].d2_privates.d2_size =
			       attr_val->d2_at_value.d2_ttxid_a_v->
						       d2_privates.d2_size;
			   }
		   else
			   ttx_id[0].d2_privates.d2_size = 0;

		   attr_entry[0].a_val.d2_at_value.d2_ttxid_a_v =
				 attr_val->d2_at_value.d2_ttxid_a_v;
		   attr_entry[0].a_val.d2_a_v_len = attr_val->d2_a_v_len;

		   d20_ttxidomask[D20_TTSCD].d20_fkt_id = D20_FKTX;
		   d20_ttxidomask[D20_TTMOD].d20_fkt_id = D20_CSAY;
		   d20_ttxidomask[D20_TTCHG].d20_fkt_id = D20_CGET;
		   fkt = d20_ttxid(DS_MODIFY_ATTRIBUTE, a_name, &ttx_id[0]);
		   if (fkt == FKTSCD)
			{
			attr_mode = D20_CONTINUE;
			continue;
			}

		   if ((fkt != FKTCR) && (fkt != FKTMENU))
			{
			d20_ttxidomask[D20_TTSCD].d20_fkt_id =
			d20_ttxidomask[D20_TTMOD].d20_fkt_id =
			d20_ttxidomask[D20_TTCHG].d20_fkt_id = D20_NO_OP;
			return(D2_ERROR);
			}
		   else
			{
			attr_entry[0].mod_type = attr_entry[1].mod_type =
			    (signed16) * ((signed32 *) d20_ttxidifields[DS_TT_CHG]);

			if (attr_entry[0].mod_type == D2_DEL_AV)
				attr_entry[0].a_val.d2_at_value.d2_ttxid_a_v = 
								&ttx_id[0];
			else
				attr_entry[1].a_val.d2_at_value.d2_ttxid_a_v = 
								&ttx_id[0];

			d20_ttxidomask[D20_TTSCD].d20_fkt_id =
			d20_ttxidomask[D20_TTMOD].d20_fkt_id =
			d20_ttxidomask[D20_TTCHG].d20_fkt_id = D20_NO_OP;
			menu = TRUE;
			}
		   break;

	      case D2_TLXNR_STX:
		   strncpy((char *)telex[0].d2_t_number.d2_value,
			   (char *)attr_val->d2_at_value.d2_tlxnr_a_v->
			       d2_t_number.d2_value,
			   attr_val->d2_at_value.d2_tlxnr_a_v->
			       d2_t_number.d2_size);
		   telex[0].d2_t_number.d2_size =
		       attr_val->d2_at_value.d2_tlxnr_a_v->d2_t_number.d2_size;

		   strncpy((char *)telex[0].d2_c_code.d2_value,
			   (char *)attr_val->d2_at_value.d2_tlxnr_a_v->
			       d2_c_code.d2_value,
			   attr_val->d2_at_value.d2_tlxnr_a_v->
			       d2_c_code.d2_size);
		   telex[0].d2_c_code.d2_size =
		       attr_val->d2_at_value.d2_tlxnr_a_v->d2_c_code.d2_size;

		   strncpy((char *)telex[0].d2_answer.d2_value,
			   (char *)attr_val->d2_at_value.d2_tlxnr_a_v->
			       d2_answer.d2_value,
			   attr_val->d2_at_value.d2_tlxnr_a_v->
			       d2_answer.d2_size);
		   telex[0].d2_answer.d2_size =
		       attr_val->d2_at_value.d2_tlxnr_a_v->d2_answer.d2_size;

		   attr_entry[0].a_val.d2_at_value.d2_tlxnr_a_v =
				 attr_val->d2_at_value.d2_tlxnr_a_v;
		   attr_entry[0].a_val.d2_a_v_len = attr_val->d2_a_v_len;

		   d20_telexomask[D20_TLSCD].d20_fkt_id = D20_FKTX;
		   d20_telexomask[D20_TLMOD].d20_fkt_id = D20_CSAY;
		   d20_telexomask[D20_TLCHG].d20_fkt_id = D20_CGET;
		   fkt = d20_telex(DS_MODIFY_ATTRIBUTE, a_name, &telex[0]);
		   if (fkt == FKTSCD)
			{
			attr_mode = D20_CONTINUE;
			continue;
			}

		   if ((fkt != FKTCR) && (fkt != FKTMENU))
			{
			d20_telexomask[D20_TLSCD].d20_fkt_id =
			d20_telexomask[D20_TLMOD].d20_fkt_id =
			d20_telexomask[D20_TLCHG].d20_fkt_id = D20_NO_OP;
			return(D2_ERROR);
			}
		   else
			{
			attr_entry[0].mod_type = attr_entry[1].mod_type =
			    (signed16) * ((signed32 *) d20_telexifields[DS_TL_CHG]);

			if (attr_entry[0].mod_type == D2_DEL_AV)
				attr_entry[0].a_val.d2_at_value.d2_tlxnr_a_v = 
								&telex[0];
			else
				attr_entry[1].a_val.d2_at_value.d2_tlxnr_a_v = 
								&telex[0];

			d20_telexomask[D20_TLSCD].d20_fkt_id =
			d20_telexomask[D20_TLMOD].d20_fkt_id =
			d20_telexomask[D20_TLCHG].d20_fkt_id = D20_NO_OP;
			menu = TRUE;
			}
		   break;

	      case D2_FAXNR_STX:
		   strncpy((char *)fax[0].d2_tel_nr.d2_value,
			   (char *)attr_val->d2_at_value.d2_faxnr_a_v->
			       d2_tel_nr.d2_value,
			   attr_val->d2_at_value.d2_faxnr_a_v->
			       d2_tel_nr.d2_size);
		   fax[0].d2_tel_nr.d2_size =
		       attr_val->d2_at_value.d2_faxnr_a_v->d2_tel_nr.d2_size;
		   fax[0].d2_par_defined = attr_val->d2_at_value.d2_faxnr_a_v->d2_par_defined;
		   fax[0].d2_par = attr_val->d2_at_value.d2_faxnr_a_v->d2_par;

		   attr_entry[0].a_val.d2_at_value.d2_faxnr_a_v =
				 attr_val->d2_at_value.d2_faxnr_a_v;
		   attr_entry[0].a_val.d2_a_v_len = attr_val->d2_a_v_len;

		   d20_faxomask[D20_FXSCD].d20_fkt_id = D20_FKTX;
		   d20_faxomask[D20_FXMOD].d20_fkt_id = D20_CSAY;
		   d20_faxomask[D20_FXCHG].d20_fkt_id = D20_CGET;
		   fkt = d20_fax(DS_MODIFY_ATTRIBUTE, a_name, &fax[0]);
		   if (fkt == FKTSCD)
			{
			attr_mode = D20_CONTINUE;
			continue;
			}

		   if ((fkt != FKTCR) && (fkt != FKTMENU))
			{
			d20_faxomask[D20_FXSCD].d20_fkt_id =
			d20_faxomask[D20_FXMOD].d20_fkt_id =
			d20_faxomask[D20_FXCHG].d20_fkt_id = D20_NO_OP;
			return(D2_ERROR);
			}
		   else
			{
			attr_entry[0].mod_type = attr_entry[1].mod_type =
			    (signed16) * ((signed32 *) d20_faxifields[DS_FX_CHG]);

			if (attr_entry[0].mod_type == D2_DEL_AV)
				attr_entry[0].a_val.d2_at_value.d2_faxnr_a_v = 
								&fax[0];
			else
				attr_entry[1].a_val.d2_at_value.d2_faxnr_a_v = 
								&fax[0];

			d20_faxomask[D20_FXSCD].d20_fkt_id =
			d20_faxomask[D20_FXMOD].d20_fkt_id =
			d20_faxomask[D20_FXCHG].d20_fkt_id = D20_NO_OP;
			menu = TRUE;
			}
		   break;

	    case D2_T61_LIST:
		   p_t61 = attr_val->d2_at_value.d2_t61lst_a_v->d2_t61_item;
		   for (i=0; i < attr_val->d2_at_value.d2_t61lst_a_v->d2_nb_item;
			  i++, p_t61++)
			  {
			  strncpy((char *)(t61[0].d2_t61_item[i].d2_value),
				    (char *)p_t61->d2_value,
				    p_t61->d2_size);
			  t61[0].d2_t61_item[i].d2_size = p_t61->d2_size;
			  }
		   t61[0].d2_nb_item =
		       attr_val->d2_at_value.d2_t61lst_a_v->d2_nb_item;

		   attr_entry[0].a_val.d2_at_value.d2_t61lst_a_v =
				 attr_val->d2_at_value.d2_t61lst_a_v;
		   attr_entry[0].a_val.d2_a_v_len = attr_val->d2_a_v_len;

		   d20_t61omask[D20_T61SCD].d20_fkt_id = D20_FKTX;
		   d20_t61omask[D20_T61MOD].d20_fkt_id = D20_CSAY;
		   d20_t61omask[D20_T61CHG].d20_fkt_id = D20_CGET;
		   fkt = d20_t61(DS_MODIFY_ATTRIBUTE, a_name, &t61[0]);
		   if (fkt == FKTSCD)
			{
			attr_mode = D20_CONTINUE;
			continue;
			}

		   if ((fkt != FKTCR) && (fkt != FKTMENU))
			{
			d20_t61omask[D20_T61SCD].d20_fkt_id =
			d20_t61omask[D20_T61MOD].d20_fkt_id =
			d20_t61omask[D20_T61CHG].d20_fkt_id = D20_NO_OP;
			return(D2_ERROR);
			}
		   else
			{
			attr_entry[0].mod_type = attr_entry[1].mod_type =
			    (signed16) * ((signed32 *) d20_t61ifields[DS_T61_CHG]);
			
			if (attr_entry[0].mod_type == D2_DEL_AV)
				attr_entry[0].a_val.d2_at_value.d2_t61lst_a_v =
								 &t61[0];
			else
				attr_entry[1].a_val.d2_at_value.d2_t61lst_a_v =
								 &t61[0];
			d20_t61omask[D20_T61SCD].d20_fkt_id =
			d20_t61omask[D20_T61MOD].d20_fkt_id =
			d20_t61omask[D20_T61CHG].d20_fkt_id = D20_NO_OP;
			menu = TRUE;
			}
		   break;

	    case D2_T61_PR_LIST:
		   src_str_list = attr_val->d2_at_value.d2_post_a_v;
		   target_str_list = &post_str_list[0][0];
		   while (src_str_list != (D2_str_list *)NULL)
			  {
			  target_str_list->d2_next = target_str_list + 1;

			  target_str_list->d2_rep = src_str_list->d2_rep;

			  target_str_list->d2_str.d2_size = 
			  src_str_list->d2_str.d2_size > 
			  D20_POST_ADD_FLD_LEN * D20_T61_CONV_FACT ?
			  D20_POST_ADD_FLD_LEN * D20_T61_CONV_FACT : 
			  src_str_list->d2_str.d2_size;

		  	  strncpy((char *)(target_str_list->d2_str.d2_value), 
				  (char *)src_str_list->d2_str.d2_value,
				  target_str_list->d2_str.d2_size);

			  src_str_list = src_str_list->d2_next;
			  target_str_list = target_str_list->d2_next;
			  }

		   (target_str_list - 1)->d2_next = (D2_str_list *)NULL;

		   attr_entry[0].a_val.d2_at_value.d2_post_a_v =
				 attr_val->d2_at_value.d2_post_a_v;
		   attr_entry[0].a_val.d2_a_v_len = attr_val->d2_a_v_len;

		   d20_t61omask[D20_T61SCD].d20_fkt_id = D20_FKTX;
		   d20_t61omask[D20_T61MOD].d20_fkt_id = D20_CSAY;
		   d20_t61omask[D20_T61CHG].d20_fkt_id = D20_CGET;
		   fkt = d20_postal(DS_MODIFY_ATTRIBUTE, a_name, 
							&post_str_list[0][0]);
		   if (fkt == FKTSCD)
			{
			attr_mode = D20_CONTINUE;
			continue;
			}

		   if ((fkt != FKTCR) && (fkt != FKTMENU))
			{
			d20_t61omask[D20_T61SCD].d20_fkt_id =
			d20_t61omask[D20_T61MOD].d20_fkt_id =
			d20_t61omask[D20_T61CHG].d20_fkt_id = D20_NO_OP;
			return(D2_ERROR);
			}
		   else
			{
			attr_entry[0].mod_type = attr_entry[1].mod_type =
			    (signed16) * ((signed32 *) d20_t61ifields[DS_T61_CHG]);
			
			if (attr_entry[0].mod_type == D2_DEL_AV)
				attr_entry[0].a_val.d2_at_value.d2_post_a_v =
							 &post_str_list[0][0];
			else
				attr_entry[1].a_val.d2_at_value.d2_post_a_v =
							 &post_str_list[0][0];
			d20_t61omask[D20_T61SCD].d20_fkt_id =
			d20_t61omask[D20_T61MOD].d20_fkt_id =
			d20_t61omask[D20_T61CHG].d20_fkt_id = D20_NO_OP;
			menu = TRUE;
			}
		   break;

	    case D2_MHS_ORADR_STX:
		   attr_entry[0].a_val.d2_at_value.d2_or_addr_av =
				 attr_val->d2_at_value.d2_or_addr_av;

		   fkt = d20_oradd(DS_MODIFY_ATTRIBUTE, a_name, 
			 attr_val->d2_at_value.d2_or_addr_av, &array_oradd[0],
							      &mod_type);

		   if (fkt == FKTSCD)
			{
			attr_mode = D20_CONTINUE;
			continue;
			}

		   if ((fkt != FKTCR) && (fkt != FKTMENU))
			return(D2_ERROR);
		   else
			{
			attr_entry[0].mod_type = attr_entry[1].mod_type =
								mod_type;
			if (mod_type == D2_DEL_AV)
				attr_entry[0].a_val.d2_at_value.d2_or_addr_av = 
							&array_oradd[0];
			else
				attr_entry[1].a_val.d2_at_value.d2_or_addr_av = 
							&array_oradd[0];

			menu = TRUE;
			}
		   break;

   	    case D2_MHS_ORNAME_STX:
		   attr_entry[0].a_val.d2_at_value.d2_or_name_av =
				 attr_val->d2_at_value.d2_or_name_av;

		   fkt = d20_orname(0, DS_MODIFY_ATTRIBUTE, a_name,
			 attr_val->d2_at_value.d2_or_name_av, &array_orname[0],
							      &mod_type);

		   if (fkt == FKTSCD)
			{
			attr_mode = D20_CONTINUE;
			continue;
			}

		   if ((fkt != FKTCR) && (fkt != FKTMENU))
			return(D2_ERROR);
		   else
			{
			attr_entry[0].mod_type = attr_entry[1].mod_type =
								mod_type;
			if (mod_type == D2_DEL_AV)
				attr_entry[0].a_val.d2_at_value.d2_or_name_av = 
							&array_orname[0];
			else
				attr_entry[1].a_val.d2_at_value.d2_or_name_av = 
							&array_orname[0];

			menu = TRUE;
			}
		   break;

	    case D2_MHS_DLSP_STX:
		   attr_entry[0].a_val.d2_at_value.d2_dl_s_p_av =
				 attr_val->d2_at_value.d2_dl_s_p_av;

		   fkt = d20_dlsp(0, DS_MODIFY_ATTRIBUTE, a_name,
				  attr_val->d2_at_value.d2_dl_s_p_av,
			          &array_dlsp[0], 
			          &array_orname[0],
			          mog_space,
				  &mod_type); 

		   if (fkt == FKTSCD)
			{
			attr_mode = D20_CONTINUE;
			continue;
			}

		   if ((fkt != FKTCR) && (fkt != FKTMENU))
			return(D2_ERROR);
		   else
			{
			attr_entry[0].mod_type = attr_entry[1].mod_type =
								mod_type;
			if (mod_type == D2_DEL_AV)
				attr_entry[0].a_val.d2_at_value.d2_dl_s_p_av = 
							&array_dlsp[0];
			else
				attr_entry[1].a_val.d2_at_value.d2_dl_s_p_av = 
							&array_dlsp[0];

			menu = TRUE;
			}
		   break;

	      case D2_PSAP_STX:
		  prty[1].ds_value = attr_val->d2_at_value.d2_oct_a_v;
		  prty[1].ds_length = attr_val->d2_a_v_len;
		  ds4_004_octncpy((Octet_string) psap_old_val, 
			attr_val->d2_at_value.d2_oct_a_v, attr_val->d2_a_v_len);

		  /*  show PSAP-mask (for new value)    */
		  fkt = ds0_520_handle_spec_attr(DS_MODIFY_ATTRIBUTE, &prty[1]);

   		  if ((fkt != FKTCR) && (fkt != FKTMENU))
  			return(D2_ERROR);

		  attr_entry[0].a_val.d2_a_v_len = attr_val->d2_a_v_len;
		  attr_entry[0].a_val.d2_at_value.d2_oct_a_v = 
						(Octet_string)psap_old_val;
		  attr_entry[1].a_val.d2_a_v_len = prty[1].ds_length;
		  attr_entry[1].a_val.d2_at_value.d2_oct_a_v = 
						(Octet_string)prty[1].ds_value;

		  menu = TRUE;

		  break;
	      }

	    if (menu == TRUE)
		  break;
	    }
    }                               
    }
				    /*  fill modify info structure        */
d20_fill_modify(D20_ASTART | D20_AINSERT | D20_AEND, attr_entry, 1);

return(D2_NOERROR);

}                               /*  ds0_handle_spec_attr              */



/*inon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : ds0_check_input()                                     */
/*								      */
/* AUTHOR:      Schmid (D AP 11)                                      */
/* DATE:        04.03.88                                              */
/*								      */
/* SYNTAX:      signed16 ds0_check_input(oct_ent, at_ent, attr_entry) */
/*								      */
/*		D20_oct		 *oct_ent; pointer to relevant OCT    */
/*				 	   entry		      */
/*		D20_at		 *at_ent; pointer to relevant AT      */
/*				 	   entry		      */
/*              Ds_at_entry      *attr_entry;   pointer to attribute  */
/*                                              information           */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will check the input of the "modify"    */
/*              mask.                                                 */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		D20_oct		 *oct_ent; pointer to relevant OCT    */
/*				 	   entry		      */
/*		D20_at		 *at_ent; pointer to relevant AT      */
/*				 	   entry		      */
/*              Ds_at_entry      *attr_entry;   pointer to attribute  */
/*                                              information           */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*								      */
/*       0      :       No error occurred                             */
/*      -1      :       Error occurred                                */
/*								      */
/* STANDARD-HEADER: None                                              */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*              Bool              old_present;  old attr. val. is     */
/*						present or not.       */
/*              Bool              new_present;  new attr. val. is     */
/*						prsent or not.        */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                      ds0_513_hexa()                                */
/*                      d20_handlemask()                              */
/*                      strlen()                                      */
/*                      strnmp()                                      */
/*                      strcpy()                                      */
/*								      */
/* NOTES:                                                             */
/*								      */
/*inoff ***************************************************************/

static  signed16 ds0_check_input(
	D20_oct		 *oct_ent,
	D20_at		 *at_ent,
	Ds_at_entry      *attr_entry)
{                               /*  ds0_check_input                   */
   char              message[D20_MAXMSG];
   signed32          ret_value = D2_NOERROR;	/* Return-Value */
   signed16          j, i;
   signed16          length;
#ifndef RELEASE2
   char             *p_dn;
static char              dn_new[D2_DNL_MAX];
static char              dn_old[D2_DNL_MAX];
#endif
   char              obj_id_str[D2_OBJ_IDL_MAX + 1];
   D2_obj_id         obj_id[2];
static   char        obj_attr[2][D2_OBJ_IDL_MAX];
   char              val_int[(D20_ATVAL_LEN * D20_T61_CONV_FACT) + 1];
static   signed32    lst_int[2][D20_MAX_INT];
   char             *p_lstint;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

strcpy(d20_8hflds[0],d20_8ifields[DS_MA1OLD_VAL]);
strcat(d20_8hflds[0],d20_8ifields[DS_MA2OLD_VAL]);
strcpy(d20_8hflds[1],d20_8ifields[DS_MA1NEW_VAL]);
strcat(d20_8hflds[1],d20_8ifields[DS_MA2NEW_VAL]);
for                             /*  old and new attribute             */
    (j=0; j < 2; j++, attr_entry++)
{                               /*  fill attribute structure          */
  attr_entry->attr_id.d2_type = at_ent->obj_id.d2_type;
  attr_entry->attr_id.d2_typ_len = at_ent->obj_id.d2_typ_len;
  attr_entry->a_val.d2_at_value.d2_oct_a_v = (Octet_string)d20_8hflds[j];

  if ((attr_entry->a_val.d2_a_rep = d27_401_syntax_to_rep((signed16) 
					at_ent->at.att_syntax)) == D2_T61_PR)
	if (d20_printable == TRUE)
  		attr_entry->a_val.d2_a_rep = D2_PRINTABLE;
	else
  		attr_entry->a_val.d2_a_rep = D2_T61;

				/*  determine length of attribute     */
  length = attr_entry->a_val.d2_a_v_len = d20_strlen(d20_8hflds[j]);
  
  if (j == 0)
	if (length != 0)
		old_present = TRUE;
	else
		old_present = FALSE;
  else
	if (length != 0)
		new_present = TRUE;
	else
		new_present = FALSE;

  if                            /*  attribute length = 0              */
     (!length)
  {                             /*  hex_use = FALSE                   */
    attr_entry->hex_use = FALSE;
  }
else if                     /*  last character not equal '        */
	(d20_8hflds[j][length - 1] != '\'')
  {                             /*  display error message             */
				/*  return(D2_ERROR)                  */
    d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab[D20_E08IDX];
    d20_handlemask(d20_qomask);
    return(D2_ERROR);
  }
else if                     /*  input is hexadecimal              */
	(!strncmp(d20_8hflds[j],HEX,2) && length != 2)
  {                             
    if                          /*  syntax is DN, BOOLEAN, INTEGER,   */
				/*  INTEGER-LIST or OBJ-IDENT         */
       (at_ent->at.att_syntax == D2_DISTNAME  ||
	at_ent->at.att_syntax == D2_BOOLEAN   ||
	at_ent->at.att_syntax == D2_INTEGER   ||
	at_ent->at.att_syntax == D2_OBJ_IDENT ||
	at_ent->at.att_syntax == D2_PR_DM_STX ||
	at_ent->at.att_syntax == D2_MHS_PR_DM_STX)
    {                           /*  display error message             */
				/*  return(D2_ERROR)                  */
      d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab[D20_E72IDX];
      d20_handlemask(d20_qomask);
      return(D2_ERROR);
    }                           
    attr_entry->hex_use = TRUE;
    attr_entry->a_val.d2_at_value.d2_oct_a_v = (byte *)(d20_8hflds[j] + 2);
    if                          /*  converting to hexa is successful  */
       (!ds0_513_hexa(d20_8hflds[j],length))
    {                           /*  determine new length of attr value*/
      length = (length - 3) / 2;
    }
else
{                           /*  return(D2_ERROR)                  */
      return(D2_ERROR);
    }                           
  }
else
{                             /*  length -= 1                       */
    attr_entry->hex_use = FALSE;
    length--;
  }                             
  attr_entry->a_val.d2_a_v_len = length;

  if                            /*  syntax is DN                      */
     (at_ent->at.att_syntax == D2_DISTNAME)
  {                             /*  terminate DN with EOS             */
    if (length)
    {   
      *(d20_8hflds[j] + length) = D2_EOS;
      length++;
      p_dn = (j == 0) ? dn_old : dn_new;
      strcpy(p_dn,d20_8hflds[j]);
      if (ds0_508_check_name(p_dn))
	{/*  show error message                */
	sprintf(message,d20_errtab[D20_E44IDX], j);
	d20_msgomask[D20_M1POS].d20_ioinfo = message;
	d20_handlemask(d20_msgomask);
	return(D2_ERROR);
	}
      else
	{
	attr_entry->a_val.d2_a_v_len = strlen(p_dn) + 1;
	attr_entry->a_val.d2_at_value.d2_oct_a_v = (Octet_string)p_dn;
	}
    } 
  }                             

  if                            /*  syntax is BOOLEAN                 */
     (at_ent->at.att_syntax == D2_BOOLEAN)
  {                             
    if (length)
    {   
      if (strncmp(d20_bool[0], d20_8hflds[j], length) == 0)
	     {
	     attr_entry->a_val.d2_at_value.d2_bool_a_v = TRUE;
	     }
      else
	     if (strncmp(d20_bool[1],d20_8hflds[j], length) == 0)
		    {
		    attr_entry->a_val.d2_at_value.d2_bool_a_v = FALSE;
		    }
	     else
		    {
		    d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab[D20_E94IDX];
		    d20_handlemask(d20_qomask);
		    return(D2_ERROR);
		    }
    } 
  } 

  if                            
     (at_ent->at.att_syntax == D2_INTEGER)
  {                             
    if (length)
    {   
      strncpy(val_int, d20_8hflds[j], length);
      val_int[length] = D2_EOS;
      errno = 0;
      attr_entry->a_val.d2_at_value.d2_int_a_v = strtol(val_int, (char **)NULL, 10);
      if (errno != 0)
	    {
	    errno = 0;
	    d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab[D20_E94IDX];
	    d20_handlemask(d20_qomask);
	    return(D2_ERROR);
	    }
    } 
  } 

  if                            
     (at_ent->at.att_syntax == D2_PR_DM_STX ||
      at_ent->at.att_syntax == D2_MHS_PR_DM_STX)
  {   
    if (length)
    {   
      p_lstint = d20_8hflds[j];
      for (i=0; *p_lstint != '\'' && i < D20_MAX_INT; i++)
	   {
	   sscanf(p_lstint,"%d", &lst_int[j][i]);
	   do {
		p_lstint++;
	      }
	   while (*p_lstint != ' ' && *p_lstint != '\'');
	   }

       if (*p_lstint == '\'')
		{
		attr_entry->a_val.d2_a_v_len = sizeof(signed32)*i;
		attr_entry->a_val.d2_at_value.d2_intlist_a_v =
					    (signed32 *)(&lst_int[j][0]);
		}
       else
		{
		d20_qomask[D20_Q1POS].d20_ioinfo = (!j) ? 
			d20_errtab[D20_E15IDX] : d20_errtab[D20_E16IDX];
		d20_handlemask(d20_qomask);
		return(D2_ERROR);
		}
    } 
  } 

  if                            
     (at_ent->at.att_syntax == D2_OBJ_IDENT)
  {                             
    if (length)
    {   
      strncpy(obj_id_str, d20_8hflds[j], length);
      obj_id_str[length] = D2_EOS;
      obj_id[j].d2_type = (Ob_id_string)&obj_attr[j][0];
      ret_value = d27_302_str_oid (obj_id_str, &obj_id[j]);
      if (ret_value != D2_NOERROR)
	     {
	     d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab[D20_E94IDX];
	     d20_handlemask(d20_qomask);
	     return(D2_ERROR);
	     }
      attr_entry->a_val.d2_a_v_len = obj_id[j].d2_typ_len;
      attr_entry->a_val.d2_at_value.d2_obid_a_v = (Ob_id_string)&obj_attr[j][0];
    } 
  } 

  if (at_ent->at.att_syntax != D2_BOOLEAN   &&
      at_ent->at.att_syntax != D2_INTEGER   &&
      at_ent->at.att_syntax != D2_OBJ_IDENT &&
      at_ent->at.att_syntax != D2_PR_DM_STX &&
      at_ent->at.att_syntax != D2_MHS_PR_DM_STX)
	{
	if                            /*  object is known && length > MAXIMUM */
	   (oct_ent != D20_UNKNOWN && length > at_ent->at.att_upper)
	{                             /*  display error message             */
	  d20_qomask[D20_Q1POS].d20_ioinfo = (!j) ? d20_errtab[D20_E15IDX]
						  : d20_errtab[D20_E16IDX];
	  d20_handlemask(d20_qomask);
				      /*  return(D2_ERROR)                  */
	  return(D2_ERROR);
	}                             
	}
}                               

if                              /*  operation is MODIFY_ALL_ATTRIBUTES */
   (oct_ent == D20_UNKNOWN)
{                               
  if                            
     (!old_present)
  {                             /*  display error message             */
				/*  return(D2_ERROR)                  */
    d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab[D20_E06IDX];
    d20_handlemask(d20_qomask);
    return(D2_ERROR);
  }
else if                     
     (!new_present)
  {                             /*  display error message             */
				/*  return(D2_ERROR)                  */
    d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab[D20_E05IDX];
    d20_handlemask(d20_qomask);
    return(D2_ERROR);
  }                             
}                               

				/*  return(D2_NOERROR)                */
return(D2_NOERROR);

}                               /*  ds0_check_input                   */



/*inon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : ds0_modify_mode()                                     */
/*								      */
/* AUTHOR:      Schmid (D AP 11)                                      */
/* DATE:        04.03.88                                              */
/*								      */
/* SYNTAX:      signed16 ds0_modify_mode(nr_recur, change_mode)	      */
/*								      */
/*              signed16        nr_recur;                             */
/*              signed16       *change_mode;                          */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will determine the type of modification */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              signed16        nr_recur;                             */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              signed16       *change_mode;                          */
/*								      */
/* RETURNVALUE:							      */
/*								      */
/*       0      :       No error occurred                             */
/*      -1      :       Error occurred                                */
/*								      */
/* STANDARD-HEADER: None                                              */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*              Bool              old_present;  old attr. val. is     */
/*						present or not.       */
/*              Bool              new_present;  new attr. val. is     */
/*						prsent or not.        */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                          d20_handlemask()                          */
/*								      */
/* NOTES:                                                             */
/*								      */
/*inoff ***************************************************************/

static signed16 ds0_modify_mode(signed16 nr_recur, signed16 *change_mode)
{                               /*  ds0_modify_mode                   */
if                              /*  no attribute values given         */
   (!old_present && !new_present)
{                               /*  display error message             */
  d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab[D20_E06IDX];
  d20_handlemask(d20_qomask);
  return(D2_ERROR);
}
else if                       /*  new value is missing              */
      (!new_present)
{                               
  if                            /*  attribute is no recurring one     */
     (nr_recur == 1)
  {                             /*  display error message             */
    d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab[D20_102IDX];
    d20_handlemask(d20_qomask);
    return(D2_ERROR);
  }
else
{                             /*  change-mode = D2_DEL_AV           */
    *change_mode = D2_DEL_AV;
  }                             
}
else if                       /*  old value is missing              */
      (!old_present)
{                               /*  determine change-mode             */
  if                            /*  attributre is no recurring one    */
     (nr_recur == 1)
  {                             /*  dispaly error message             */
    d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab[D20_102IDX];
    d20_handlemask(d20_qomask);
    return(D2_ERROR);
  }
else
{                             /*  chang_mode = D2_ADD_AV            */
    *change_mode = D2_ADD_AV;
  }                             
}
else
{                               /*  chang_mode = D2_REP_AV            */
  *change_mode = D2_REP_AV;
}                               
				/*  return(D2_NOERROR)                */
return(D2_NOERROR);

}                               /*  ds0_modify_mode                   */



/*inon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : ds0_503_mod_attr()                                    */
/*								      */
/* AUTHOR:      Schmid, D AP 11                                       */
/* DATE:        06.03.88                                              */
/*								      */
/* SYNTAX:      signed16 ds0_503_mod_attr(srt_ent,oct_ent,attr_entry) */
/*								      */
/*		D20_srt		 *srt_ent;			      */
/*		D20_oct		 *oct_ent;			      */
/*              Ds_at_entry      *attr_entry;                         */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will display the mask to modify an at-  */
/*              tribute-value.                                        */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		D20_srt		*srt_ent;    pointer to relevant      */
/*					     structural rule entry    */
/*		D20_oct		*oct_ent;    pointer to relevant      */
/*					     object class entry       */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              Ds_at_entry    *attr_entry;     attribute info of     */
/*                                              attribute to be       */
/*                                              modified              */
/*								      */
/* RETURNVALUE:							      */
/*								      */
/*       0      :       No error occurred                             */
/*      -1      :       Error occurred                                */
/*								      */
/* STANDARD-HEADER: None                                              */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                      ds0_disp_old_val()                            */
/*                      sprintf()                                     */
/*                      strnmp()                                      */
/*                      strcpy()                                      */
/*								      */
/* NOTES:                                                             */
/*								      */
/*inoff ***************************************************************/

signed16 ds0_503_mod_attr(
	D20_srt		 *srt_ent,
	D20_oct		 *oct_ent,
	Ds_at_entry      *attr_entry)
{                               /*  ds0_503_mod_attr                  */
   D20_at 	     *at_ent;

   signed16      change_mode;

   D2_a_type     a_type;
   signed16      a_len;
   Octet_string  a_val;
   D2_a_value   *attr_val;
   Bool          spec_attr;

   byte          a_int_val[(D20_ATVAL_LEN * D20_T61_CONV_FACT) + 1];
   D2_obj_id     obj_id[2];
   char          obj_id_str[D2_OBJ_IDL_MAX + 1];
   char          obj_attr[2][D2_OBJ_IDL_MAX];
   char          intlst[(D20_ATVAL_LEN * D20_T61_CONV_FACT) + 1];
   signed16      attr_mode;
   signed32      fkt_key;
   Bool	         get_attr;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

/*  attribute is not correct          */
if   (ds0_fetch_attr(srt_ent, oct_ent, &at_ent, attr_entry, &spec_attr))
	/*  return(D2_ERROR)                  */
  	return(D2_ERROR);

if (spec_attr == TRUE)
  	return(D2_NOERROR);

attr_mode = D20_BEGIN;

/* set SCD function key.	*/
d20_8omask[D20_82POS].d20_fkt_id = D20_NO_OP;
if (oct_ent != D20_UNKNOWN && at_ent->at.att_no_recur != 1)
	d20_8omask[D20_82POS].d20_fkt_id = D20_FKTX;

/*  initialize ds_flds                */
strcpy(d20_8ifields[DS_MA_NAME], at_ent->at.att_name);
get_attr = TRUE;

for (;;)
	{
    	/*  fetch next attribute value when Modifying single object       */
      	/*  no recurring value found          */
	if (oct_ent != D20_UNKNOWN)
		{
		if (get_attr)
			{
			if (d20_get_attribute(attr_mode, &a_type, &attr_val))
				{
				/*  display error message             */
				d20_msgomask[D20_M1POS].d20_ioinfo = 
							d20_errtab[D20_E03IDX];
				d20_handlemask(d20_msgomask);
				}
			else
				{
				/*  display next value                */
				a_val = attr_val->d2_at_value.d2_oct_a_v;
				a_len = attr_val->d2_a_v_len;

				if (at_ent->at.att_syntax == D2_DISTNAME)
					{
					d20_gen_dnintf((String) a_val);
					a_len = strlen((char *)a_val);
					}

				if (at_ent->at.att_syntax == D2_INTEGER)
					{
					sprintf((char *)a_int_val, "%d", 
					attr_val->d2_at_value.d2_int_a_v);
					a_val = &a_int_val[0];
					a_len = strlen((char *)a_val);
					}

				if (at_ent->at.att_syntax == D2_BOOLEAN)
					{
					if (attr_val->d2_at_value.d2_bool_a_v 
								== TRUE)
					      {
					      a_val = (Octet_string)d20_bool[0];
					      a_len = strlen(d20_bool[0]);
					      }

					if (attr_val->d2_at_value.d2_bool_a_v 
								== FALSE)
					      {
					      a_val = (Octet_string)d20_bool[1];
					      a_len = strlen(d20_bool[1]);
					      }
					}

				if (at_ent->at.att_syntax == D2_OBJ_IDENT)
					{
					obj_id[0].d2_type = 
						(Ob_id_string)&obj_attr[0][0];
					(void) memcpy(obj_id[0].d2_type, 
					attr_val->d2_at_value.
					d2_obid_a_v,attr_val->d2_a_v_len);
					obj_id[0].d2_typ_len = 
							attr_val->d2_a_v_len;
					d27_301_oid_str(&obj_id[0], obj_id_str);
					a_val = (Octet_string)&obj_id_str[0];
					a_len = strlen((char *)a_val);
					}

				 if (at_ent->at.att_syntax == D2_PR_DM_STX ||
				     at_ent->at.att_syntax == D2_MHS_PR_DM_STX)
					{
					a_val = (Octet_string)&intlst[0];
					a_len = 0;
					attr_mode = D20_CONTINUE;

					/* merge attribute values of same */
					/* type of attribute. 		  */
					do	
					    {
					    sprintf((char *)a_int_val, "%d ",
					    attr_val->d2_at_value.d2_int_a_v);

					    if ((a_len +
						(int)strlen(
						    (char *)a_int_val)) >
						(D20_ATVAL_LEN * 
					         D20_T61_CONV_FACT))
						{
						if ((a_len
						    + strlen((char *)a_int_val) 
						    - 1) ==
						    (D20_ATVAL_LEN * 
						    D20_T61_CONV_FACT))
							{
							strncpy(&intlst[a_len], 
							(char *)a_int_val, 
							strlen((char *)
							    a_int_val)-1);
							a_len += 
							strlen(&intlst[a_len]) 
							+ 1;
							}

						/* break from while loop. */
						/* no more space left.    */
						break;	
						}
					    else
						{
						strcpy(&intlst[a_len], 
						(char *)a_int_val);
						a_len += strlen(&intlst[a_len]);
						}
					    } 
					while (d20_get_attribute(attr_mode, 
					&a_type, &attr_val) == D2_NOERROR);

					a_len--;
					} /* end of PR_DM_STX.	*/
				} /* end of display next value.	*/
			} /* end of get_attr TRUE	*/

		ds0_disp_old_val(a_len, a_val, at_ent->at.att_syntax);

		} /* end of OCT entry known.	*/

	/*  get input from Modify Attribute   */
	fkt_key = d20_handlemask(d20_8omask);

    	/*  "break" was selected */
       	if (fkt_key == FKTDEL)
    		/*  return(D2_ERROR) */
      		return(D2_ERROR);

    	/*  "more" was selected               */
       	if (fkt_key == FKTSCD)
		{
		attr_mode = D20_CONTINUE;
		get_attr = TRUE;
		continue;
		}

    	/*  execute was selected              */
	/*  check input                       */

      	/*  input is correct                  */
	if (!ds0_check_input(oct_ent, at_ent, attr_entry))
		{
      		/*  get modifiaction mode             */
		/*  no error occurred                 */
	   	if (!ds0_modify_mode(at_ent->at.att_no_recur, &change_mode))
			/*  break                             */
	  		break;
		}

	get_attr = FALSE;
	}

				/*  fill modify info block            */
attr_entry->mod_type = (attr_entry + 1)->mod_type = change_mode;
d20_fill_modify(D20_ASTART | D20_AINSERT | D20_AEND, attr_entry, 1);

return(D2_NOERROR);

}                                 /*  ds0_503_mod_attr()              */
