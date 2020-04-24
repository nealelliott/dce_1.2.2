/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20getat.c,v $
 * Revision 1.1.723.2  1996/02/18  19:40:41  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:21:36  marty]
 *
 * Revision 1.1.723.1  1995/12/08  15:16:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:51  root]
 * 
 * Revision 1.1.721.6  1994/07/14  19:23:56  keutel
 * 	OT 11295: type collision for d20_dir_id
 * 	[1994/07/14  19:08:42  keutel]
 * 
 * Revision 1.1.721.5  1994/07/06  15:06:26  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:11:28  marrek]
 * 
 * Revision 1.1.721.4  1994/06/21  14:43:36  marrek
 * 	June 1994 code submission.
 * 	[1994/06/17  13:10:27  marrek]
 * 
 * Revision 1.1.721.3  1994/05/10  15:49:32  marrek
 * 	Bug fixes and I18N for April 1994 submission.
 * 	[1994/05/06  09:58:13  marrek]
 * 
 * 	Bug fixes.
 * 	[1994/04/29  09:28:34  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:11:07  marrek]
 * 
 * Revision 1.1.721.2  1994/03/23  15:05:16  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:09:00  keutel]
 * 
 * Revision 1.1.721.1  1994/02/22  15:58:26  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:28:35  marrek]
 * 
 * Revision 1.1.719.2  1993/10/14  16:46:26  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  16:43:48  keutel]
 * 
 * Revision 1.1.719.1  1993/10/13  17:29:32  keutel
 * 	October 1993 code drop
 * 	[1993/10/11  07:34:54  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  10:02:59  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:28:26  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  17:49:27  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:04:04  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/04  18:01:04  marrek
 * 	November 1992 code drop
 * 	Replace cmx.h by cmx_addr.h
 * 	[1992/12/04  10:35:00  marrek]
 * 
 * Revision 1.1.4.2  1992/11/27  13:11:07  marrek
 * 	November 1992 code drop
 * 	[1992/11/26  08:52:36  marrek]
 * 
 * Revision 1.1.2.3  1992/06/30  20:54:03  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:32:53  melman]
 * 
 * Revision 1.1.2.2  1992/06/01  21:54:37  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:15:42  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20getat.c,v $ $Revision: 1.1.723.2 $ $Date: 1996/02/18 19:40:41 $";
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
/* NAME         : getattr.c   [Administrations-Schnittstelle]         */
/*								      */
/* AUTHOR       : Sanjay Jain, D AP 11                                */
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
/*      0.1 | 21.08.87| Erstellung                     | ws |         */
/*								      */
/*datoff **************************************************************/

/* Includes */

#include <gds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <gdsext.h>
#include <d2adm.h>
#include <d2msk.h>
#include <d2spec_msk.h>
#include <cget.h>
#include <d2info.h>
#include <d2mhs_msk.h>
#include <dce/cmx_addr.h>
#include <d20proto.h>

/* variables */

extern int errno;

extern D20_at	*d2_p_d20at;
extern signed32	d2_nd20at;
extern signed32	d2_nd20srt;
extern D20_srt	*d2_p_d20srt;
extern char	*d20_bool[];

extern D20_omask d20_7omask[];        /* Eingabefelder fuer maske 7  */

extern char	*d20_7ifields[];      /* Maske 7                     */
extern char	*d20_7hflds[];        /* help fields                 */

extern D20_omask d20_qomask[];        /* Eingabefelder fuer Errormask*/
extern char	*d20_errtab[];        /* Error Mask                  */
extern char     *d20_err_table[];     /* error table                 */
extern Bool	d20_cache;            /* administrate cache if TRUE  */
extern signed32	rbatch;
extern Bool	d20_printable;
extern signed16	d20_auth_mech;		/* authentication mechanism	*/
extern signed16	d20_dir_id;		/* current directory-id	in use	*/
					/* (defined in d20logon.c)	*/


 signed16     dsa_type_value;          /* a sign if default or local  */
				       /* dsa is given                */

static    char		 oradd_space[((D20_ATVAL_LEN * D20_T61_CONV_FACT * (D20_MAX_ORADD_COMP - 3))
				     + D20_LEN_PAIF
				     + D20_LEN_ALL_PAIL
				     + T_PSAPSIZE)
				     * D20_MAX_ORADD];
static   D2_str_list     oradd_comp[D20_MAX_STR_LIST * D20_MAX_ORADD];
   D2_OR_address   array_oradd[D20_MAX_ORADD];

static   char		 dir_name_space[D20_ATVAL_LEN * D20_T61_CONV_FACT * D20_MAX_ORNAME];
   D2_OR_name      array_orname[D20_MAX_ORNAME];

   char		 	mog_space[D20_ATVAL_LEN * D20_T61_CONV_FACT * D20_MAX_DLSP];
   D2_dl_submit_perm      array_dlsp[D20_MAX_DLSP];
	 D2_str_list	post_str_list[2][D20_MAX_NB_ITEM];
static	 char		post_space[2 * 
				(D20_MAX_NB_ITEM * (D20_POST_ADD_FLD_LEN * D20_T61_CONV_FACT + 1))];

/* Functions */

/******** LOCAL ********/

/* The indentation follows the hierarchy	*/

static signed32 d20_sep_special_attrs(D20_at **attr_ptr_space,
			signed32 no_selected_attrs, signed32 *no_simple_attrs);
static signed32 d20_req_special_mask(D20_at *attr_ptr);
static signed32 attr_ptr_cmp( D20_at **ptr1, D20_at **ptr2 );
static void d20_fill_attr_types(D20_at **attr_ptr_space, 
			signed32 no_selected_attrs, Ds_at_entry attr_entry[]);
static signed32 d20_add_principal_name( Ds_at_entry ** );
  static signed32 d20_check_pr_name_format( Print_string *pr_name );
static int d20_search_auth_mech( int, int * );



/*inon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : ds0_502_get_attr()                                    */
/*								      */
/* AUTHOR:      Volpers (D AP 11)                                     */
/* DATE:        27.08.87                                              */
/*								      */
/* SYNTAX:      signed16 ds0_502_get_attr(operation,rule_nr,oct_ent,  */
/*				       count, init_flds)	      */
/*              char             operation;                           */
/*		signed32	 rule_nr;			      */
/*		D20_oct		 *oct_ent;			      */
/*		signed16	 *count;			      */
/*		Bool		init_flds;			      */
/*								      */
/* DESCRIPTION:                                                       */
/*              This function will display the mask to enter the name */
/*              and value of attributes to add or delete attributes   */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              char            operation                             */
/*		signed32	rule_nr;	rule no. of the       */
/*						selected structure    */
/*		D20_oct		*oct_ent;       pointer to the object */
/*						class entry to be     */
/*						handled   	      */
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
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                      ds0_513_hexa()                                */
/*                      ds4_b02_fill_attr()                           */
/*                      sprintf()                                     */
/*                      strlen()                                      */
/*                      strncmp()                                     */
/*                      strcpy()                                      */
/*								      */
/* BEMERKUNGEN:							      */
/*								      */
/*inoff ***************************************************************/

signed32 ds0_502_get_attr(
	char      operation,
	signed32  rule_nr,
	D20_oct	 *oct_ent,
	signed16 *count,
	Bool	  init_flds)
{	/*  ds0_502_get_attr                  */

/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*		    D E	K L A R	A T I O	N E N			      */
/*								      */
/**********************************************************************/

signed32 ret_value = D2_NOERROR;/* Return-Value                    */
signed32   fkt;
D20_at *ptr2;
signed16 i,j,y,len1;
char message[D20_MAXMSG];
Bool control = TRUE;  /* indicator for further input        */
signed16 count_alt;
String HEX = "x'";
Bool next_attrs = TRUE;

Ds_property    prty[3];      /* for special attributes        */

static Ds_at_entry  attr_entry[D2_AT_MAX];
static Ds_at_entry  *principal_name_entry;
static Bool         pr_name_present = FALSE;
static char     dn[3][D2_DNL_MAX + 1];

int		d20_auth_mech_configured;	/* Authentication	*/
						/* mechanism for which	*/
						/* the current dir-id	*/
						/* is configured	*/
						/* eventually different	*/
						/* of the current	*/
						/* auth. mech. in use	*/

signed16  type;
Bool   dsa_type;

char            spec_attr[3*D20_MAX_NO][(D20_MAX_LEN * D20_T61_CONV_FACT)+1];
D2_obj_id       obj_id[3];
D2_ttxid        ttx_id[3];
D2_faxnumber    fax[3];
D2_telex_number telex[3];
D2_T61_seq      t61[3];
signed32        lst_int[3][D20_MAX_INT];
D2_str          d2_str[3*D20_MAX_NB_ITEM];

byte            cell[D20_MAX_CELL];
byte            dnsrep[D20_MAX_REP];
signed16        rep_len;
signed16        cell_len;

Octet_string    dme_nmo;
signed16        dme_nmo_len;

static Certificate	cert[D20_MAX_CERT];
signed16		cert_len;
signed32		cert_cnt = 0;
static Cert_pair	cert_pair[D20_MAX_CERTP];
signed16		cert_pair_len;
signed32		certp_cnt = 0;
static Cert_list	cert_list[D20_MAX_CERTL];
signed16		cert_list_len;
signed32		certl_cnt = 0;

char            val_int[D20_ATVAL_LEN * D20_T61_CONV_FACT + 1];
char            *p_lstint;

Bool            spec_err;
unsigned16      rep;
D20_oct 	       *p_oct;
char	       *mog_space_ptr = mog_space;

static char  emp_name_fld[] = "_________________________";
static char  emp_val_fld[] = "_____________________________________________";


D20_at **must_attr = oct_ent->must_attr;
D20_srt	*srt_ent;
char		obj_id_str[D2_OBJ_IDL_MAX + 1];

signed32	oradd_cnt = 0;
signed32	orname_cnt = 0;
signed32	dlsp_cnt = 0;
signed32	no_selected_attrs;
signed32	no_simple_attrs;
signed32   	simple_attr_index = 0;
signed32	curr_page_start_index;
D20_at		**attr_ptr_space;
Bool		convert;

int cur_dir_id = d20_dir_id;


/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

/* find srt_ent to identify naming attributes.		*/
srt_ent = d20_search_adm_srule(d2_nd20srt, d2_p_d20srt, rule_nr);

/* make valid attribute list, display attribute list and get selected attrs. */
if ((ret_value = d20_handle_attr_list(operation, srt_ent, oct_ent, 
			&attr_ptr_space, &no_selected_attrs)) == D2_ERROR)
	goto exit_point;

/* return if no attr. has been selected and operation is ADD_ATTR or 	*/
/* DELETE_ATTR.								*/
if (no_selected_attrs == 0 && operation != DS_ADD_OBJECT)
	{
	ret_value = D2_ERROR;
	goto exit_point;
	}

/* in case of Delete Attribute operation, fill attribute types and return.   */
if (operation == DS_DELETE_ATTRIBUTE)
	{
	d20_fill_attr_types(attr_ptr_space, no_selected_attrs, attr_entry);
	ret_value = D2_NOERROR;
	goto exit_point;
	}

/* seperate attributes which do not require special mask and attributes    */
/* which require special mask.						   */
if ((ret_value = d20_sep_special_attrs(attr_ptr_space, no_selected_attrs, 
			&no_simple_attrs)) == D2_ERROR)
	goto exit_point;

if ((ret_value = ds0_malloc_attr(3, spec_attr, &d2_str[0],
				&ttx_id[0], &fax[0], &telex[0], &t61[0],
				&obj_id[0])) == D2_ERROR)
	goto exit_point;

ds0_assign_space_to_oradd();
ds0_assign_space_to_orname();
ds0_assign_space_to_post_add();


/*  initialize dsa_type_value         */
dsa_type_value = D20_0_DSA_TYP;

if (operation == DS_ADD_OBJECT)
	{
	/* fill object class attribute values.		*/

	for (;;must_attr++)
		{
		/* attribute is not object class attribute.	*/
		if (strcmp((*must_attr)->at.att_abbrv,S_AT_OCL) != 0)
			/* examine next mandatory attribute.	*/
			continue;

		/* attribute is object class attribute.		*/

		/* fill attribute entry with object class values.	*/
		/* object class values include obj_ids of selected      */
		/* object class and of all its superior (including 	*/
		/* selected aux. object classes.) object classes.	*/

		p_oct = oct_ent;
		j = -1;

		do
			{
			count_alt = *count;

			/* skip GTP object class.	*/
			if (p_oct->obj_id.d2_typ_len != 0) 
				{
				attr_entry[*count].attr_id.d2_type = 
						(*must_attr)->obj_id.d2_type;
				attr_entry[*count].attr_id.d2_typ_len = 
						(*must_attr)->obj_id.d2_typ_len;
				attr_entry[*count].hex_use = FALSE;

				if ((attr_entry[*count].a_val.d2_a_rep = 
				   (unsigned16) d27_401_syntax_to_rep((signed16)
				   (*must_attr)->at.att_syntax)) == D2_T61_PR)
					if (d20_printable == TRUE)
						attr_entry[*count].a_val.
						d2_a_rep = D2_PRINTABLE;
					else
						attr_entry[*count].a_val.
						d2_a_rep = D2_T61;

				attr_entry[*count].a_val.d2_a_v_len = 
						p_oct->obj_id.d2_typ_len;
				attr_entry[*count].a_val.d2_at_value.d2_oct_a_v 
				= p_oct->obj_id.d2_type;

				(*count)++;
				if (count_alt != 0)
					type = D20_AINSERT;
				else
					type = D20_ASTART | D20_AINSERT;

				ds4_b02_fill_attr(type,&attr_entry[count_alt],
						      (*count)-count_alt);
				}

			p_oct = oct_ent->sup_class[++j];

			} 
		while ( j != oct_ent->no_sup_class);

		break;	/* object class attribute values added, so break. */
		}	/* end of for loop to look for object class attr. */

	}	/* end of ADD_OBJECT case.	*/

/* handle simple attributes.			*/
while ((simple_attr_index < no_simple_attrs) || (next_attrs == FALSE))
	{
  	/*  get input from object name mask   */
	/* only if some simple attributes are there.	*/

	spec_err = FALSE;

	count_alt = *count;

	/* if next set of attributes, reset attribute value mask with new */
	/* attribute names.					          */
	if (next_attrs == TRUE)
    		{
		/*  initialize ds_flds                */
		for (i=0;i < 3;i++)
			{
			if (simple_attr_index < no_simple_attrs)
				{
				strcpy(d20_7ifields[3*i], (*(attr_ptr_space + 
				simple_attr_index))->at.att_name);
				switch(d27_401_syntax_to_rep(
				       (*(attr_ptr_space + simple_attr_index++))
				       ->at.att_syntax))
					{
					case D2_T61_PR:
					case D2_DISTNAME:
						convert = TRUE;
						break;
					default:
						convert = FALSE;
						break;
					}

				d20_7omask[D20_EA1_POS + 3 + i*5].d20_conv = 
				d20_7omask[D20_EA1_POS + 4 + i*5].d20_conv = 
				convert;
				}
			else
				strcpy(d20_7ifields[3*i],emp_name_fld);

			if (rbatch)
				d20_7omask[D20_EA1_POS + 1 + i*5].d20_fkt_id = 
								D20_CGET;
			else
				d20_7omask[D20_EA1_POS + 1 + i*5].d20_fkt_id = 
								D20_CSAY;
			strcpy(d20_7ifields[3*i+1],emp_val_fld);
			strcpy(d20_7ifields[3*i+2],emp_val_fld);
			}
		}

	/* handle attribute value mask.		*/
	fkt = d20_handlemask(d20_7omask);

    	/*  "break" was selected		*/
	if (fkt == FKTDEL)
		{
		ret_value = D2_ERROR;
		break;
		}

	/* validate/analyse the current attribute value mask.	*/

	curr_page_start_index = 3 * ((simple_attr_index - 1)/3);
	for (i=0; i < (simple_attr_index - curr_page_start_index); i++)
		{
		ptr2 = *(attr_ptr_space + curr_page_start_index + i);

		if (strcmp(ptr2->at.att_name, D2_TYPE_DSA) == 0)
			dsa_type = TRUE;
		else
			dsa_type = FALSE;

		/*  attribute is DSA-Type and cache administration */
		if (d20_cache == TRUE && dsa_type == TRUE)
			{
			/*  check value                       */
			/* len1 = length of attr_value        */
			len1 = d20_strlen(d20_7ifields[3*i+1]);

			if (len1 == 0 || d20_7ifields[3*i+1][len1-1] !=  '\'')
				{
				/* input not finish with '            */
				d20_qomask[D20_Q1POS].d20_ioinfo = 
				d20_errtab[D20_E08IDX];
				d20_handlemask(d20_qomask);
				control = FALSE;

				/* break from for loop.		*/
				break;
				}

			/*  value is allowed                  */
			if (!strncmp(d20_7ifields[3*i+1], D2_1DEFAULT,
				strlen(D2_1DEFAULT)) && strlen(D2_1DEFAULT) == 
				len1 -1)
				dsa_type_value = D20_1_DSA_TYP;
			else
				{
		 		if (!strncmp(d20_7ifields[3*i+1], D2_2LOCAL,
					strlen(D2_2LOCAL)) && strlen(D2_2LOCAL) 
					== len1 - 1)
				    {
				    dsa_type_value = D20_2_DSA_TYP;

				    /* In case of DSA-Type == local and	*/
				    /* the directory-id is configured	*/
				    /* for authentication mechanism ==	*/
				    /* DCE or DCE + SIMPLE,	*/
				    /* ask and enter the principal name	*/
				    if( d20_search_auth_mech(cur_dir_id,
					&d20_auth_mech_configured)
					== D2_NOERROR )
					{
					if( d20_auth_mech_configured
					    == D2_DCE_AUTH
					|| d20_auth_mech_configured
					    == D2_SIMPLE_DCE )
					    {
					    if( d20_add_principal_name(
						&principal_name_entry)
						== D2_ERROR )
						goto exit_point;
					    else
						pr_name_present = TRUE;
					    }
					}
				    }
				else
				    {
				    if (!strncmp(d20_7ifields[3*i+1],
				    D2_3DEFLOC,strlen(D2_3DEFLOC)) && 
				    strlen(D2_3DEFLOC) == len1 - 1)
					{
					dsa_type_value = D20_3_DSA_TYP;

					/* DSA-Type == local and the	*/
					/* directory-id is configured	*/
					/* for authentication mechanism	*/
					/* == DCE or DCE + SIMPLE, ask	*/
					/* and enter the principal name	*/
					if( d20_search_auth_mech(cur_dir_id,
					    &d20_auth_mech_configured)
					    == D2_NOERROR )
					    {
					    if( d20_auth_mech_configured
						== D2_DCE_AUTH
					    || d20_auth_mech_configured
						== D2_SIMPLE_DCE )
						{
						if( d20_add_principal_name(
						    &principal_name_entry)
						    == D2_ERROR )
						    goto exit_point;
						else
						    pr_name_present = TRUE;
						}
					    }
					}
				    else
					{
					d20_qomask[D20_Q1POS].d20_ioinfo
					= d20_errtab[D20_E94IDX];
					d20_handlemask(d20_qomask);
					control = FALSE;
					/* break from for loop.	*/
					break;        
					}
				    }
				}
			/* analyse next attribute.		*/
			continue;
			}

		/* normal attribute                   */
		/*  fill attribute entry              */
		attr_entry[*count].attr_id.d2_type = ptr2->obj_id.d2_type;
		attr_entry[*count].attr_id.d2_typ_len = ptr2->obj_id.d2_typ_len;
	
		if ((attr_entry[*count].a_val.d2_a_rep = rep = (unsigned16)
			d27_401_syntax_to_rep((signed16) ptr2->at.att_syntax)) == 
			D2_T61_PR)
			if (d20_printable == TRUE)
				attr_entry[*count].a_val.d2_a_rep = 
				rep = D2_PRINTABLE;
			else
				attr_entry[*count].a_val.d2_a_rep = 
				rep = D2_T61;

		strcpy(d20_7hflds[i],d20_7ifields[3*i+1]);
		strcat(d20_7hflds[i],d20_7ifields[3*i+2]);

		/*  determine length of attribute value */

		y = d20_strlen(d20_7hflds[i]);

		/*  length = 0 */
		if (y == 0)
			{
			/*  display error message             */
			/*  break                             */
			d20_qomask[D20_Q1POS].d20_ioinfo = 
						d20_errtab[D20_E08IDX];
			d20_handlemask(d20_qomask);
			control = FALSE;

			/* break from for loop.		*/
			break;
			}

		/*  last character != '               */
		if (d20_7hflds[i][y - 1] != '\'')
			{
			/*  display error message             */
			/*  break                             */
			d20_qomask[D20_Q1POS].d20_ioinfo = 
						d20_errtab[D20_E08IDX];
			d20_handlemask(d20_qomask);
			control = FALSE;

			/* break from for loop.		*/
			break;
			}

		/*  input is hexadecimal              */
		if (!strncmp(d20_7hflds[i],HEX,2) && y != 2)
			{
			if (ptr2->at.att_syntax == D2_DISTNAME  ||
			    ptr2->at.att_syntax == D2_BOOLEAN   ||
			    ptr2->at.att_syntax == D2_INTEGER   ||
			    ptr2->at.att_syntax == D2_OBJ_IDENT ||
			    ptr2->at.att_syntax == D2_PR_DM_STX ||
			    ptr2->at.att_syntax == D2_MHS_PR_DM_STX)
				{
				d20_qomask[D20_Q1POS].d20_ioinfo =
						   d20_errtab[D20_E72IDX];
				d20_handlemask(d20_qomask);
				control = FALSE;

				/* break from for loop.		*/
				break;
				}

			attr_entry[*count].hex_use = TRUE;

			switch((int)attr_entry[*count].a_val.d2_a_rep) 
				{
				case D2_TIME:
					attr_entry[*count].a_val.d2_at_value.d2_utc_a_v = (String)(d20_7hflds[i] + 2);
					break;

				case D2_PRINTABLE:
					attr_entry[*count].a_val.d2_at_value.d2_prt_a_v = (String)(d20_7hflds[i] + 2);
					break;

				case D2_NUMERIC:
					attr_entry[*count].a_val.d2_at_value.d2_num_a_v = (String)(d20_7hflds[i] + 2);
					break;

				case D2_T61:
					attr_entry[*count].a_val.d2_at_value.d2_t61_a_v = (byte *)(d20_7hflds[i] + 2);
					break;

				case D2_OCTET:
					attr_entry[*count].a_val.d2_at_value.d2_oct_a_v = (byte *)(d20_7hflds[i] + 2);
					break;

				case D2_IA5:
					attr_entry[*count].a_val.d2_at_value.d2_ia5_a_v = (IA5_string)(d20_7hflds[i] + 2);
					break;

				case D2_ASN1:
					attr_entry[*count].a_val.d2_at_value.d2_asn1_a_v = (byte *)(d20_7hflds[i] + 2);
					break;

				}

			/*  all characters are hexadecimal    */
			if (!ds0_513_hexa(d20_7hflds[i],y))
				/* determine new length of attr. value*/
				y = (y - 3) / 2;
			else
				{
				control = FALSE;

				/* break from for loop.		*/
				break;
				}
			}	/* end of HEX processing.	*/
		else
			{ 
			attr_entry[*count].hex_use = FALSE;

			switch((int)attr_entry[*count].a_val.d2_a_rep) 
				{
				case D2_BOOLEAN:
					if (strncmp(d20_bool[0], 
					d20_7hflds[i], y-1) == (int)NULL)
						attr_entry[*count].a_val.d2_at_value.d2_bool_a_v = TRUE;
					else
						if (strncmp(d20_bool[1],d20_7hflds[i], y-1) == (int)NULL)
				   			attr_entry[*count].a_val.d2_at_value.d2_bool_a_v = FALSE;
						else
							spec_err = TRUE;
					break;

				case D2_INT_LIST:
				      p_lstint = d20_7hflds[i];
				      for (j=0; *p_lstint != '\'' && 
					   j < D20_MAX_INT; j++)
					   {
					   sscanf(p_lstint,"%d",&lst_int[i][j]);
					   do {
						p_lstint++;
					      }
					   while (*p_lstint != ' ' && 
							*p_lstint != '\'');
					   }

					if (*p_lstint != '\'')
						spec_err = TRUE;
					else
						{
						attr_entry[*count].a_val.
						d2_a_v_len = sizeof(signed32)*j;
						attr_entry[*count].a_val.
						d2_at_value.d2_intlist_a_v = 
						  (signed32 *)(&lst_int[i][0]);
						}

					break;

				case D2_INTEGER:
					strncpy(val_int, d20_7hflds[i], y-1);
					val_int[y-1] = D2_EOS;
					errno = 0;
					attr_entry[*count].a_val.d2_at_value.
								d2_int_a_v =
					strtol(val_int, (char **)NULL, 10);
					if (errno != 0)
						{
						errno = 0;
						spec_err = TRUE;
						}
					break;

				case D2_TIME:
					attr_entry[*count].a_val.d2_at_value.d2_utc_a_v = d20_7hflds[i];
					break;

				case D2_PRINTABLE:
					attr_entry[*count].a_val.d2_at_value.d2_prt_a_v = d20_7hflds[i];
					break;

				case D2_NUMERIC:
					attr_entry[*count].a_val.d2_at_value.d2_num_a_v = (Numeric_string)d20_7hflds[i];
					break;

				case D2_T61:
					attr_entry[*count].a_val.d2_at_value.d2_t61_a_v = (T61_string)d20_7hflds[i];
					break;

				case D2_OBJ_IDENT:
					strncpy(obj_id_str, d20_7hflds[i], y-1);
					ret_value = d27_302_str_oid (obj_id_str, &obj_id[i]);
					attr_entry[*count].a_val.d2_at_value.d2_obid_a_v = obj_id[i].d2_type;
					attr_entry[*count].a_val.d2_a_v_len = obj_id[i].d2_typ_len;

					if (ret_value != D2_NOERROR)
						spec_err = TRUE;

					break;

				case D2_OCTET:
					attr_entry[*count].a_val.d2_at_value.d2_oct_a_v = (Octet_string)d20_7hflds[i];
					break;

				case D2_IA5:
					attr_entry[*count].a_val.d2_at_value.d2_ia5_a_v = (IA5_string)d20_7hflds[i];
					break;

				case D2_DISTNAME:
					attr_entry[*count].a_val.d2_at_value.d2_obj_name_a_v = (D2_name_string)d20_7hflds[i];
					break;

				case D2_ASN1:
					attr_entry[*count].a_val.d2_at_value.d2_asn1_a_v = (byte *)d20_7hflds[i];
					break;

				}

			if (spec_err == TRUE)
				{
				spec_err = FALSE;
				d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab[D20_E94IDX];
				d20_handlemask(d20_qomask);
				control = FALSE;

				/* break from for loop.		*/
				break;
				}

			y--;
			}	/* end of Non-HEX attribute values.	*/

		if (ptr2->at.att_syntax != D2_BOOLEAN   &&
			ptr2->at.att_syntax != D2_INTEGER   &&
			ptr2->at.att_syntax != D2_OBJ_IDENT &&
			ptr2->at.att_syntax != D2_PR_DM_STX &&
			ptr2->at.att_syntax != D2_MHS_PR_DM_STX)
			{
			/*  length > maximal length           */
			if (y > ptr2->at.att_upper)
				{
				/*  display error message             */
				/*  break                             */
				sprintf(message,d20_errtab[D20_E09IDX],i+1);
				d20_qomask[D20_Q1POS].d20_ioinfo = message;
				d20_handlemask(d20_qomask);
				control = FALSE;
				/* break from for loop.		*/
				break;
				}

			/*  store attribute length            */
			attr_entry[*count].a_val.d2_a_v_len = y;
			}

		if (ptr2->at.att_syntax == D2_DISTNAME)
			{
			*(d20_7hflds[i] + y) = D2_EOS;
			strcpy(dn[i],d20_7hflds[i]);

			/* check syntax                       */
			/*  syntax not correct                */
			if (ds0_508_check_name(dn[i]))
				{
				/*  show error message                */
				sprintf(message, d20_errtab[D20_E44IDX], i+1);
				d20_qomask[D20_Q1POS].d20_ioinfo = message;
				d20_handlemask(d20_qomask);
				control = FALSE;

				/* break from for loop.		*/
				break;
				}

			attr_entry[*count].a_val.d2_a_v_len = strlen(dn[i]) + 1;
			attr_entry[*count].a_val.d2_at_value.d2_obj_name_a_v = (D2_name_string)dn[i];
			}

	      	(*count)++;
		}/* end of for loop to scan attributes in the current mask.*/

	/*  error occurred                    */
	if (control == FALSE)
		{
		/*  reset number of attributes        */
	  	*count = count_alt;
	  	next_attrs = FALSE;
	  	control = TRUE;
		}
	else
		{
		/*  fill attributes                   */
		if (count_alt != 0)
			type = D20_AINSERT;
		else
			type = D20_ASTART | D20_AINSERT;

		ds4_b02_fill_attr(type,&attr_entry[count_alt],
							(*count)-count_alt);
		control = TRUE;
		next_attrs = TRUE;
		}
	}	/* end of handling of simple attributes.	*/

if (ret_value == D2_ERROR)
	goto exit_point;

/* handle attributes which require special masks.		*/
for (j=simple_attr_index; j<no_selected_attrs; j++)
	{
	ptr2 = *(attr_ptr_space + j);

#ifndef CACHE_ADMIN
	/* Attribute is ACL		*/
	if (d20_cmp_objid(&(ptr2->obj_id), sizeof(D2_ACL),
		(Ob_id_string) D2_ACL)) 
		{
		if (ds0_507_handle_acl(operation,init_flds,0,(char *)NULL))
			{
			ret_value = D2_ERROR;
			goto exit_point;
			}

		continue;
		}
#endif

	attr_entry[*count].attr_id.d2_type = ptr2->obj_id.d2_type;
	attr_entry[*count].attr_id.d2_typ_len = ptr2->obj_id.d2_typ_len;

	attr_entry[*count].hex_use = FALSE;

	if ((attr_entry[*count].a_val.d2_a_rep = rep = (unsigned16) 
		d27_401_syntax_to_rep((signed16) ptr2->at.att_syntax)) == 
		D2_T61_PR)
		if (d20_printable == TRUE)
			attr_entry[*count].a_val.d2_a_rep = rep = D2_PRINTABLE;
		else
			attr_entry[*count].a_val.d2_a_rep = rep = D2_T61;

	/* check if special mask for DNSCELL is needed */
	if (d20_cmp_objid(&(ptr2->obj_id), sizeof(D2_CDS_CELL),
		(Ob_id_string) D2_CDS_CELL))
		{
		fkt = d20_dnscell(operation,ptr2->at.att_name, cell, &cell_len);

		if ((fkt != FKTCR) && (fkt != FKTMENU))
			{
			ret_value = D2_ERROR;
			goto exit_point;
			}

		/*  fill attr_entry structure        */
		attr_entry[*count].a_val.d2_at_value.d2_oct_a_v = cell;
		attr_entry[*count].a_val.d2_a_v_len = cell_len;

		goto fill_attr;
		}

	/*  check if special mask for DNSREP is needed */
	if (d20_cmp_objid(&(ptr2->obj_id), sizeof(D2_CDS_REPLICA), 
						(Ob_id_string) D2_CDS_REPLICA))
		{
		fkt = d20_dnsrep(operation,ptr2->at.att_name, dnsrep, &rep_len);

		if ((fkt != FKTCR) && (fkt != FKTMENU))
			{
			ret_value = D2_ERROR;
			goto exit_point;
			}

		/*  fill attr_entry structure        */
		attr_entry[*count].a_val.d2_at_value.d2_oct_a_v = dnsrep;
		attr_entry[*count].a_val.d2_a_v_len = rep_len;

		goto fill_attr;
		}

	/* Check if special mask for DME NMO is needed */
	if( d20_cmp_objid(&(ptr2->obj_id), sizeof(D2_DME_ALT_ADDR),
			(Ob_id_string) D2_DME_ALT_ADDR) )
		{
		fkt = d20_dme_disp( 0, operation, ptr2->at.att_name,
						&dme_nmo, &dme_nmo_len );

		if ((fkt != FKTCR) && (fkt != FKTMENU))
			{
			ret_value = D2_ERROR;
			goto exit_point;
			}

		/*  fill attr_entry structure        */
		attr_entry[*count].a_val.d2_at_value.d2_oct_a_v = dme_nmo;
		attr_entry[*count].a_val.d2_a_v_len = dme_nmo_len;

		goto fill_attr;
		}

	switch((int)rep) 
		{
		case D2_PSAP_STX:
			fkt = ds0_520_handle_spec_attr(operation, prty);

			if ((fkt != FKTCR) && (fkt != FKTMENU))
				{
				ret_value = D2_ERROR;
				goto exit_point;
				}

			/*  fill attr_entry structure        */
			attr_entry[*count].a_val.d2_a_v_len = prty[0].ds_length;
			attr_entry[*count].a_val.d2_at_value.d2_oct_a_v = 
						(Octet_string)prty[0].ds_value;

			break;

		case D2_TTXID_STX:
			fkt = d20_ttxid(operation,ptr2->at.att_name,&ttx_id[0]);

			if ((fkt != FKTCR) && (fkt != FKTMENU))
				{
				ret_value = D2_ERROR;
				goto exit_point;
				}

			/*  fill attr_entry structure        */
			attr_entry[*count].a_val.d2_at_value.d2_ttxid_a_v = 
								&ttx_id[0];
			break;

		case D2_FAXNR_STX:
			fkt = d20_fax(operation, ptr2->at.att_name, &fax[0]);

			if ((fkt != FKTCR) && (fkt != FKTMENU))
				{
				ret_value = D2_ERROR;
				goto exit_point;
				}

			/*  fill attr_entry structure        */
			attr_entry[*count].a_val.d2_at_value.d2_faxnr_a_v = 
								&fax[0];
			break;

		case D2_TLXNR_STX:
			fkt = d20_telex(operation, ptr2->at.att_name,&telex[0]);

			if ((fkt != FKTCR) && (fkt != FKTMENU))
				{
				ret_value = D2_ERROR;
				goto exit_point;
				}

			/*  fill attr_entry structure        */
			attr_entry[*count].a_val.d2_at_value.d2_tlxnr_a_v = 
								&telex[0];
			break;

		case D2_T61_LIST:
			fkt = d20_t61(operation, ptr2->at.att_name, &t61[0]);

			if ((fkt != FKTCR) && (fkt != FKTMENU))
				{
				ret_value = D2_ERROR;
				goto exit_point;
				}

			/*  fill attr_entry structure        */
			attr_entry[*count].a_val.d2_at_value.d2_t61lst_a_v = 
								&t61[0];
			break;

		case D2_T61_PR_LIST:
			fkt = d20_postal(operation, ptr2->at.att_name, 
							&post_str_list[0][0]);

			if ((fkt != FKTCR) && (fkt != FKTMENU))
				{
				ret_value = D2_ERROR;
				goto exit_point;
				}

			/*  fill attr_entry structure        */
			attr_entry[*count].a_val.d2_at_value.d2_post_a_v = 
							&post_str_list[0][0];
			break;


		case D2_MHS_ORADR_STX:
			fkt = d20_oradd(operation, ptr2->at.att_name, 
				&array_oradd[oradd_cnt], NULL, NULL);

			if ((fkt != FKTCR) && (fkt != FKTMENU))
				{
				ret_value = D2_ERROR;
				goto exit_point;
				}

			attr_entry[*count].a_val.d2_at_value.d2_or_addr_av = 
				&array_oradd[oradd_cnt++];

			orname_cnt++;
			break;

		case D2_MHS_ORNAME_STX:
			fkt = d20_orname(0, operation, ptr2->at.att_name, 
				&array_orname[orname_cnt], NULL, NULL); 

			if ((fkt != FKTCR) && (fkt != FKTMENU))
				{
				ret_value = D2_ERROR;
				goto exit_point;
				}

			attr_entry[*count].a_val.d2_at_value.d2_or_name_av = 
				&array_orname[orname_cnt++];

			/* it is possible that one OR address structure */
			/* has been used.			        */
			oradd_cnt++;

			break;

		case D2_MHS_DLSP_STX:
			fkt = d20_dlsp(0, operation, ptr2->at.att_name, 
				&array_dlsp[dlsp_cnt], 
				(D2_dl_submit_perm *)NULL, 
				&array_orname[orname_cnt], mog_space_ptr, 
				(signed16 *)NULL); 

			if ((fkt != FKTCR) && (fkt != FKTMENU))
				{
				ret_value = D2_ERROR;
				goto exit_point;
				}

			attr_entry[*count].a_val.d2_at_value.d2_dl_s_p_av = 
				&array_dlsp[dlsp_cnt++];

			/* it is possible that one OR name structure, */
			/* one OR address structure and some space in */
			/* mog_space have been used.   		      */

			orname_cnt++;
			oradd_cnt++;
			mog_space_ptr += D20_ATVAL_LEN * D20_T61_CONV_FACT;

			break;

		case D2_CERTIFICATE:
			if( cert_cnt < D20_MAX_CERT )
			{
			    fkt = d20_cert( 0, operation, ptr2->at.att_name,
						&cert[cert_cnt], &cert_len );

			    if ((fkt != FKTCR) && (fkt != FKTMENU))
				    {
				    ret_value = D2_ERROR;
				    goto exit_point;
				    }

			    /*  fill attr_entry structure        */
			    attr_entry[*count].a_val.d2_at_value.d2_cert_a_v =
							    &cert[cert_cnt++];
			    attr_entry[*count].a_val.d2_a_v_len = cert_len;
			}
			/* else */ /* No memory reserved for more than	*/
			/* D20_MAX_CERT certificates			*/
			/* An error message should be displayed		*/

			break;

		case D2_CERT_PAIR:
			if( certp_cnt < D20_MAX_CERTP )
			{
			    fkt = d20_certp( 0, operation, ptr2->at.att_name,
				    &cert_pair[certp_cnt], &cert_pair_len );

			    if ((fkt != FKTCR) && (fkt != FKTMENU))
				    {
				    ret_value = D2_ERROR;
				    goto exit_point;
				    }

			    /*  fill attr_entry structure        */
			    attr_entry[*count].a_val.d2_at_value.d2_ctpair_a_v =
						    &cert_pair[certp_cnt++];
			    attr_entry[*count].a_val.d2_a_v_len = cert_pair_len;
			}
			/* else */ /* No memory reserved for more than	*/
			/* D20_MAX_CERTP certificates			*/
			/* An error message should be displayed		*/

			break;

		case D2_CERT_LIST:
			if( certl_cnt < D20_MAX_CERTL )
			{
			    fkt = d20_certl( 0, operation, ptr2->at.att_name,
				    &cert_list[certl_cnt], &cert_list_len );

			    if ((fkt != FKTCR) && (fkt != FKTMENU))
				    {
				    ret_value = D2_ERROR;
				    goto exit_point;
				    }

			    /*  fill attr_entry structure        */
			    attr_entry[*count].a_val.d2_at_value.
				    d2_ctlist_a_v = &cert_list[certl_cnt++];
			    attr_entry[*count].a_val.d2_a_v_len =
				    cert_list_len;
			}
			/* else */ /* No memory reserved for more than	*/
			/* D20_MAX_CERTL certificates			*/
			/* An error message should be displayed		*/

			break;

		case D2_GUIDE:
			break;
		}

fill_attr:
	if (*count != 0)
		type = D20_AINSERT;
	else 
		type = D20_ASTART | D20_AINSERT;

	/*  fill attributes                   */
	ds4_b02_fill_attr(type,(Ds_at_entry *) &attr_entry[*count], 1);
	(*count)++;
	}

/* After normal and special attributes, fill the principle name		*/
/* attribute if necessary.						*/
if(pr_name_present)
{					/* Then there is one ! Copy it !*/
    type = D20_AINSERT;			/* This attributes can be added	*/
					/* only with a DSA-Type attr.	*/
					/* That's why it's sure	 we	*/
					/* don't need D20_ASTART	*/
    ds4_b02_fill_attr(type, principal_name_entry, 1);
}					/* else nothing to do.	*/

type =  D20_AEND;

/*  fill attributes                   */
ds4_b02_fill_attr(type,(Ds_at_entry *)NULL, 0);

exit_point:
if (attr_ptr_space != (D20_at **)NULL)
	free((char *)attr_ptr_space);

return(ret_value);

} /*  ds0_502_get_attr()                */



/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name       : d20_fill_attr_types                       */
/* Description         :                                           */
/* Return Value        :                                           */
/* Author              :                                           */
/*                                                                 */
/*-----------------------------------------------------------------*/

static void d20_fill_attr_types(
	D20_at **attr_ptr_space,
	signed32 no_selected_attrs,
	Ds_at_entry attr_entry[])
{
signed32	i;
D20_at	*attr_ptr;
signed16 type;

for (i=0; i<no_selected_attrs; i++)
	{
	attr_ptr = *(attr_ptr_space + i);
	attr_entry[i].attr_id.d2_type = attr_ptr->obj_id.d2_type;
	attr_entry[i].attr_id.d2_typ_len = attr_ptr->obj_id.d2_typ_len;
	}

type = D20_ATYPE_ONLY | D20_ASTART | D20_AINSERT | D20_AEND;

/*  fill attribute types                  */
ds4_b02_fill_attr(type,attr_entry, no_selected_attrs);

return;
}

/*inon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : ds0_malloc_attr()                                     */
/*								      */
/* AUTHOR:      Mueller V. (SNI AP 11)                                */
/* DATE:        16.03.91                                              */
/*								      */
/* SYNTAX:      signed32 ds0_malloc_attr(c_val, attr, d2_str, ttx_id, */
/*				fax, telex, t61, obj_id)              */
/*              signed32         c_val;                               */
/*              char          attr[][D20_MAX_LEN * D20_T61_CONV_FACT];*/
/*              D2_str           d2_str[];                            */
/*              D2_ttxid         ttx_id[];                            */
/*              D2_faxnumber     fax[];                               */
/*              D2_telex_number  telex[];                             */
/*              D2_T61_seq       t61[];                               */
/*              D2_obj_id        obj_id[];                            */
/*								      */
/*								      */
/* DESCRIPTION:                                                       */
/*              This function will allocate the memory for the special*/
/*              attributes                                            */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              signed32         c_val;        number of input fields */
/*              char          attr[][D20_MAX_LEN * D20_T61_CONV_FACT];*/
/*              D2_str           d2_str[];                            */
/*              D2_ttxid         ttx_id[];                            */
/*              D2_faxnumber     fax[];                               */
/*              D2_telex_number  telex[];                             */
/*              D2_T61_seq       t61[];                               */
/*              D2_obj_id        obj_id[];                            */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*								      */
/*								      */
/* STANDARD-HEADER: None                                              */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*								      */
/* BEMERKUNGEN:							      */
/*								      */
/*inoff ***************************************************************/

signed32 ds0_malloc_attr(
	signed32         c_val,
	char             attr[][(D20_MAX_LEN * D20_T61_CONV_FACT) + 1],
	D2_str           d2_str[],
	D2_ttxid         *ttx_id,
	D2_faxnumber     *fax,
	D2_telex_number  *telex,
	D2_T61_seq       *t61,
	D2_obj_id        obj_id[])
{                               /*  ds0_malloc_attr                   */

/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*		    D E	K L A R	A T I O	N E N			      */
/*								      */
/**********************************************************************/

signed32      j;
signed32      pos;
signed32      nb_item;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/


for (j=0; j < c_val; j++) {
	nb_item = j*D20_MAX_NB_ITEM;
	pos = j*D20_MAX_NO;

	ttx_id[j].d2_terminal.d2_value = (unsigned char *)&attr[pos][0];
	telex[j].d2_t_number.d2_value  = (unsigned char *)&attr[pos][0];
	fax[j].d2_tel_nr.d2_value      = (unsigned char *)&attr[pos][0];
	t61[j].d2_t61_item             = &d2_str[nb_item];
	d2_str[nb_item].d2_value       = (unsigned char *)&attr[pos][0];
	obj_id[j].d2_type              = (Ob_id_string)&attr[pos][0];
	pos++;
	nb_item++;

	ttx_id[j].d2_controls.d2_value = (unsigned char *)&attr[pos][0];
	telex[j].d2_c_code.d2_value    = (unsigned char *)&attr[pos][0];
	d2_str[nb_item].d2_value       = (unsigned char *)&attr[pos][0];
	pos++;
	nb_item++;

	ttx_id[j].d2_graphics.d2_value = (unsigned char *)&attr[pos][0];
	telex[j].d2_answer.d2_value    = (unsigned char *)&attr[pos][0];
	d2_str[nb_item].d2_value       = (unsigned char *)&attr[pos][0];
	pos++;
	nb_item++;

	ttx_id[j].d2_miscel.d2_value   = (unsigned char *)&attr[pos][0];
	d2_str[nb_item].d2_value       = (unsigned char *)&attr[pos][0];
	pos++;
	nb_item++;

	ttx_id[j].d2_pages.d2_value    = (unsigned char *)&attr[pos][0];
	d2_str[nb_item].d2_value       = (unsigned char *)&attr[pos][0];
	pos++;
	nb_item++;

	ttx_id[j].d2_privates.d2_value = (unsigned char *)&attr[pos][0];
	d2_str[nb_item].d2_value       = (unsigned char *)&attr[pos][0];
}

return(D2_NOERROR);

}                               /*  ds0_malloc_attr()                */



/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name       : ds0_assign_space_to_post_add              */
/* Description         :                                           */
/* Return Value        :                                           */
/* Author              :                                           */
/*                                                                 */
/*-----------------------------------------------------------------*/

void ds0_assign_space_to_post_add()
{
char	 *space_ptr = &post_space[0];
signed32  i,j;

for (i=0; i<2; i++)
	for (j=0; j<D20_MAX_NB_ITEM; j++)
		{
		post_str_list[i][j].d2_str.d2_value = 
						(unsigned char *)space_ptr;
		space_ptr += (D20_POST_ADD_FLD_LEN * D20_T61_CONV_FACT) + 1;
		}
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name       : ds0_assign_space_to_oradd                 */
/* Description         :                                           */
/* Return Value        :                                           */
/* Author              :                                           */
/*                                                                 */
/*-----------------------------------------------------------------*/

void ds0_assign_space_to_oradd()
{
signed32	no_oradd, no_comp;
char		*space_ptr;
D2_str_list	*str_list_ptr;
signed32	pail_index;

no_comp = 0;
space_ptr = oradd_space;

for (no_oradd=0; no_oradd < D20_MAX_ORADD; no_oradd++)
	{
	array_oradd[no_oradd].d2_admd_name = &oradd_comp[no_comp];
	oradd_comp[no_comp].d2_str.d2_value = (unsigned char *)space_ptr;
	no_comp++;
	space_ptr += D20_ATVAL_LEN * D20_T61_CONV_FACT;

	array_oradd[no_oradd].d2_common_name = &oradd_comp[no_comp];
	oradd_comp[no_comp].d2_str.d2_value = (unsigned char *)space_ptr;
	no_comp++;
	space_ptr += D20_ATVAL_LEN * D20_T61_CONV_FACT;

	array_oradd[no_oradd].d2_country_name = &oradd_comp[no_comp];
	oradd_comp[no_comp].d2_str.d2_value = (unsigned char *)space_ptr;
	no_comp++;
	space_ptr += D20_ATVAL_LEN * D20_T61_CONV_FACT;

	array_oradd[no_oradd].d2_domain_type1 = &oradd_comp[no_comp];
	oradd_comp[no_comp].d2_str.d2_value = (unsigned char *)space_ptr;
	no_comp++;
	space_ptr += D20_ATVAL_LEN * D20_T61_CONV_FACT;

	array_oradd[no_oradd].d2_domain_type2 = &oradd_comp[no_comp];
	oradd_comp[no_comp].d2_str.d2_value = (unsigned char *)space_ptr;
	no_comp++;
	space_ptr += D20_ATVAL_LEN * D20_T61_CONV_FACT;

	array_oradd[no_oradd].d2_domain_type3 = &oradd_comp[no_comp];
	oradd_comp[no_comp].d2_str.d2_value = (unsigned char *)space_ptr;
	no_comp++;
	space_ptr += D20_ATVAL_LEN * D20_T61_CONV_FACT;

	array_oradd[no_oradd].d2_domain_type4 = &oradd_comp[no_comp];
	oradd_comp[no_comp].d2_str.d2_value = (unsigned char *)space_ptr;
	no_comp++;
	space_ptr += D20_ATVAL_LEN * D20_T61_CONV_FACT;

	array_oradd[no_oradd].d2_domain_value1 = &oradd_comp[no_comp];
	oradd_comp[no_comp].d2_str.d2_value = (unsigned char *)space_ptr;
	no_comp++;
	space_ptr += D20_ATVAL_LEN * D20_T61_CONV_FACT;

	array_oradd[no_oradd].d2_domain_value2 = &oradd_comp[no_comp];
	oradd_comp[no_comp].d2_str.d2_value = (unsigned char *)space_ptr;
	no_comp++;
	space_ptr += D20_ATVAL_LEN * D20_T61_CONV_FACT;

	array_oradd[no_oradd].d2_domain_value3 = &oradd_comp[no_comp];
	oradd_comp[no_comp].d2_str.d2_value = (unsigned char *)space_ptr;
	no_comp++;
	space_ptr += D20_ATVAL_LEN * D20_T61_CONV_FACT;

	array_oradd[no_oradd].d2_domain_value4 = &oradd_comp[no_comp];
	oradd_comp[no_comp].d2_str.d2_value = (unsigned char *)space_ptr;
	no_comp++;
	space_ptr += D20_ATVAL_LEN * D20_T61_CONV_FACT;

	array_oradd[no_oradd].d2_generation = &oradd_comp[no_comp];
	oradd_comp[no_comp].d2_str.d2_value = (unsigned char *)space_ptr;
	no_comp++;
	space_ptr += D20_ATVAL_LEN * D20_T61_CONV_FACT;

	array_oradd[no_oradd].d2_given_name = &oradd_comp[no_comp];
	oradd_comp[no_comp].d2_str.d2_value = (unsigned char *)space_ptr;
	no_comp++;
	space_ptr += D20_ATVAL_LEN * D20_T61_CONV_FACT;

	array_oradd[no_oradd].d2_initials = &oradd_comp[no_comp];
	oradd_comp[no_comp].d2_str.d2_value = (unsigned char *)space_ptr;
	no_comp++;
	space_ptr += D20_ATVAL_LEN * D20_T61_CONV_FACT;

	array_oradd[no_oradd].d2_isdn_number = space_ptr;
	space_ptr += D20_ATVAL_LEN * D20_T61_CONV_FACT;

	array_oradd[no_oradd].d2_isdn_subaddress = space_ptr;
	space_ptr += D20_ATVAL_LEN * D20_T61_CONV_FACT;

	array_oradd[no_oradd].d2_numeric_user_id = space_ptr;
	space_ptr += D20_ATVAL_LEN * D20_T61_CONV_FACT;

	array_oradd[no_oradd].d2_org_name = &oradd_comp[no_comp];
	oradd_comp[no_comp].d2_str.d2_value = (unsigned char *)space_ptr;
	no_comp++;
	space_ptr += D20_ATVAL_LEN * D20_T61_CONV_FACT;

	array_oradd[no_oradd].d2_org_unit_name1 = &oradd_comp[no_comp];
	oradd_comp[no_comp].d2_str.d2_value = (unsigned char *)space_ptr;
	no_comp++;
	space_ptr += D20_ATVAL_LEN * D20_T61_CONV_FACT;

	array_oradd[no_oradd].d2_org_unit_name2 = &oradd_comp[no_comp];
	oradd_comp[no_comp].d2_str.d2_value = (unsigned char *)space_ptr;
	no_comp++;
	space_ptr += D20_ATVAL_LEN * D20_T61_CONV_FACT;

	array_oradd[no_oradd].d2_org_unit_name3 = &oradd_comp[no_comp];
	oradd_comp[no_comp].d2_str.d2_value = (unsigned char *)space_ptr;
	no_comp++;
	space_ptr += D20_ATVAL_LEN * D20_T61_CONV_FACT;

	array_oradd[no_oradd].d2_org_unit_name4 = &oradd_comp[no_comp];
	oradd_comp[no_comp].d2_str.d2_value = (unsigned char *)space_ptr;
	no_comp++;
	space_ptr += D20_ATVAL_LEN * D20_T61_CONV_FACT;

	array_oradd[no_oradd].d2_post_addr_details = &oradd_comp[no_comp];
	oradd_comp[no_comp].d2_str.d2_value = (unsigned char *)space_ptr;
	no_comp++;
	space_ptr += D20_ATVAL_LEN * D20_T61_CONV_FACT;

	array_oradd[no_oradd].d2_post_addr_in_full = (T61_string)space_ptr;
	space_ptr += D20_LEN_PAIF;

	array_oradd[no_oradd].d2_post_code = &oradd_comp[no_comp];
	oradd_comp[no_comp].d2_str.d2_value = (unsigned char *)space_ptr;
	no_comp++;
	space_ptr += D20_ATVAL_LEN * D20_T61_CONV_FACT;

	array_oradd[no_oradd].d2_post_country_name = &oradd_comp[no_comp];
	oradd_comp[no_comp].d2_str.d2_value = (unsigned char *)space_ptr;
	no_comp++;
	space_ptr += D20_ATVAL_LEN * D20_T61_CONV_FACT;

	array_oradd[no_oradd].d2_post_deliv_pt_name = &oradd_comp[no_comp];
	oradd_comp[no_comp].d2_str.d2_value = (unsigned char *)space_ptr;
	no_comp++;
	space_ptr += D20_ATVAL_LEN * D20_T61_CONV_FACT;

	array_oradd[no_oradd].d2_post_deliv_sys_name = space_ptr;
	space_ptr += D20_ATVAL_LEN * D20_T61_CONV_FACT;

	array_oradd[no_oradd].d2_post_gen_deliv_addr = &oradd_comp[no_comp];
	oradd_comp[no_comp].d2_str.d2_value = (unsigned char *)space_ptr;
	no_comp++;
	space_ptr += D20_ATVAL_LEN * D20_T61_CONV_FACT;

	array_oradd[no_oradd].d2_post_locale = &oradd_comp[no_comp];
	oradd_comp[no_comp].d2_str.d2_value = (unsigned char *)space_ptr;
	no_comp++;
	space_ptr += D20_ATVAL_LEN * D20_T61_CONV_FACT;

	array_oradd[no_oradd].d2_post_office_box_nbr = &oradd_comp[no_comp];
	oradd_comp[no_comp].d2_str.d2_value = (unsigned char *)space_ptr;
	no_comp++;
	space_ptr += D20_ATVAL_LEN * D20_T61_CONV_FACT;

	array_oradd[no_oradd].d2_post_office_name = &oradd_comp[no_comp];
	oradd_comp[no_comp].d2_str.d2_value = (unsigned char *)space_ptr;
	no_comp++;
	space_ptr += D20_ATVAL_LEN * D20_T61_CONV_FACT;

	array_oradd[no_oradd].d2_post_office_nbr = &oradd_comp[no_comp];
	oradd_comp[no_comp].d2_str.d2_value = (unsigned char *)space_ptr;
	no_comp++;
	space_ptr += D20_ATVAL_LEN * D20_T61_CONV_FACT;

	array_oradd[no_oradd].d2_post_org_name = &oradd_comp[no_comp];
	oradd_comp[no_comp].d2_str.d2_value = (unsigned char *)space_ptr;
	no_comp++;
	space_ptr += D20_ATVAL_LEN * D20_T61_CONV_FACT;

	array_oradd[no_oradd].d2_post_patron_details = &oradd_comp[no_comp];
	oradd_comp[no_comp].d2_str.d2_value = (unsigned char *)space_ptr;
	no_comp++;
	space_ptr += D20_ATVAL_LEN * D20_T61_CONV_FACT;

	array_oradd[no_oradd].d2_post_patron_name = &oradd_comp[no_comp];
	oradd_comp[no_comp].d2_str.d2_value = (unsigned char *)space_ptr;
	no_comp++;
	space_ptr += D20_ATVAL_LEN * D20_T61_CONV_FACT;

	array_oradd[no_oradd].d2_post_street_addr = &oradd_comp[no_comp];
	oradd_comp[no_comp].d2_str.d2_value = (unsigned char *)space_ptr;
	no_comp++;
	space_ptr += D20_ATVAL_LEN * D20_T61_CONV_FACT;

	array_oradd[no_oradd].d2_presentation_addr = (Psap_string)space_ptr;
	space_ptr += T_PSAPSIZE;

	array_oradd[no_oradd].d2_prmd_name = &oradd_comp[no_comp];
	oradd_comp[no_comp].d2_str.d2_value = (unsigned char *)space_ptr;
	no_comp++;
	space_ptr += D20_ATVAL_LEN * D20_T61_CONV_FACT;

	array_oradd[no_oradd].d2_surname = &oradd_comp[no_comp];
	oradd_comp[no_comp].d2_str.d2_value = (unsigned char *)space_ptr;
	no_comp++;
	space_ptr += D20_ATVAL_LEN * D20_T61_CONV_FACT;

	array_oradd[no_oradd].d2_terminal_id = space_ptr;
	space_ptr += D20_ATVAL_LEN * D20_T61_CONV_FACT;

	array_oradd[no_oradd].d2_x121_addr = space_ptr;
	space_ptr += D20_ATVAL_LEN * D20_T61_CONV_FACT;

	array_oradd[no_oradd].d2_post_addr_in_lines = &oradd_comp[no_comp];

	for (str_list_ptr = &oradd_comp[no_comp], pail_index = 0;
	     pail_index < 6; pail_index++)
		{
		str_list_ptr->d2_str.d2_value = (unsigned char *)space_ptr;
		no_comp++;
		str_list_ptr->d2_next = &oradd_comp[no_comp];
		str_list_ptr = &oradd_comp[no_comp];
		space_ptr += D20_LEN_ONE_PAIL;
		}

	}

}



/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name       : ds0_assign_space_to_orname                */
/* Description         :                                           */
/* Return Value        :                                           */
/* Author              :                                           */
/*                                                                 */
/*-----------------------------------------------------------------*/

void ds0_assign_space_to_orname()
{
signed32  no_orname, no_oradd = 0;
char	 *space_ptr = dir_name_space;

for (no_orname=0; no_orname<D20_MAX_ORNAME; no_orname++)
	{
	array_orname[no_orname].d2_dir_name = (D2_name_string)space_ptr;
	space_ptr += D20_ATVAL_LEN * D20_T61_CONV_FACT;

	array_orname[no_orname].d2_or_address = &array_oradd[no_oradd++];
	}
}



/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name       : d20_sep_special_attrs                     */
/* Description         :                                           */
/* Return Value        :                                           */
/* Author              :                                           */
/*                                                                 */
/*-----------------------------------------------------------------*/

static  signed32 d20_sep_special_attrs(
	D20_at   **attr_ptr_space,
	signed32   no_selected_attrs,
	signed32  *no_simple_attrs)
{
D20_at	 **tmp_attr_ptr_space;
signed32   curr_simple_index = 0;
signed32   curr_complex_index = no_selected_attrs - 1;
signed32   i;

if (no_selected_attrs != 0)
	if ((tmp_attr_ptr_space = (D20_at **)malloc(no_selected_attrs * 
				sizeof(D20_at *))) == (D20_at **)NULL)
		return(D2_ERROR);

/* put in attr_ptr_space first attributes which do not require special mask */
/* and then attributes which require special mask.			    */

for (i=0; i<no_selected_attrs; i++)
	if (d20_req_special_mask(*(attr_ptr_space + i)))
		*(tmp_attr_ptr_space + curr_complex_index--) = 
							*(attr_ptr_space + i);
	else
		*(tmp_attr_ptr_space + curr_simple_index++) = 
							*(attr_ptr_space + i);

/* copyback simple attributes.	*/
for (i=0; i<curr_simple_index; i++)
	*(attr_ptr_space + i) = *(tmp_attr_ptr_space + i);

/* copyback complex attributes in the same order in which they were present */
/* initially in attr_ptr_space.						    */
for (i=curr_simple_index, curr_complex_index=no_selected_attrs - 1; 
     i<no_selected_attrs; i++, curr_complex_index--)
	*(attr_ptr_space + i) = *(tmp_attr_ptr_space + curr_complex_index);

if (!rbatch)
	{
	/* sort simple and special attributes separately.	*/

	/* sort simple attributes.				*/
	qsort((char *)attr_ptr_space,(size_t)curr_simple_index,
		      (size_t)sizeof(D20_at *),
		      (int (*)(const void *, const void *))attr_ptr_cmp);

	/* sort special attributes.				*/
	qsort((char *)(attr_ptr_space + curr_simple_index),
		(size_t)(no_selected_attrs - curr_simple_index),
		(size_t)sizeof(D20_at *),
		(int (*)(const void *, const void *))attr_ptr_cmp);
	}

*no_simple_attrs = curr_simple_index;

if (no_selected_attrs != 0)
	free((char *)tmp_attr_ptr_space);

return(D2_NOERROR);
}



/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name       : attr_ptr_cmp                              */
/* Description         :                                           */
/* Return Value        :                                           */
/* Author              :                                           */
/*                                                                 */
/*-----------------------------------------------------------------*/

static signed32 attr_ptr_cmp( D20_at **ptr1, D20_at **ptr2 )
{
    return(strcmp((*ptr1)->at.att_name, (*ptr2)->at.att_name));
}



/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name       : d20_req_special_mask                      */
/* Description         :                                           */
/* Return Value        :                                           */
/* Author              :                                           */
/*                                                                 */
/*-----------------------------------------------------------------*/

static signed32 d20_req_special_mask(D20_at *attr_ptr)
{
unsigned16 rep;

if ((rep = (unsigned16) d27_401_syntax_to_rep(
		(signed16) attr_ptr->at.att_syntax)) == D2_T61_PR)
	if (d20_printable == TRUE)
		rep = D2_PRINTABLE;
	else
		rep = D2_T61;

/*  check whether a special mask is needed */
if (d20_cmp_objid(&(attr_ptr->obj_id),sizeof(D2_CDS_CELL),
		(Ob_id_string) D2_CDS_CELL) ||
   d20_cmp_objid(&(attr_ptr->obj_id), sizeof(D2_CDS_REPLICA),
		(Ob_id_string) D2_CDS_REPLICA) ||
   d20_cmp_objid(&(attr_ptr->obj_id), sizeof(D2_ACL),
		(Ob_id_string) D2_ACL) ||
   d20_cmp_objid(&(attr_ptr->obj_id),sizeof(D2_DME_ALT_ADDR),
		(Ob_id_string) D2_DME_ALT_ADDR) ||
   rep == D2_TTXID_STX || rep == D2_FAXNR_STX || rep == D2_GUIDE || 
   rep == D2_T61_LIST || rep == D2_T61_PR_LIST || rep == D2_TLXNR_STX || 
   rep == D2_MHS_ORADR_STX || rep == D2_MHS_ORNAME_STX || 
   rep == D2_MHS_DLSP_STX || rep == D2_PSAP_STX ||
   rep == D2_CERTIFICATE || rep == D2_CERT_PAIR || rep == D2_CERT_LIST )
	return(TRUE);
else
	return(FALSE);

}



/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name       : d20_add_principal_name                    */
/* Description         : Display the mask for 'normal' attribute,  */
/*                       asking for a principal name with:         */
/*                       type  : printable string                  */
/*                       format: X.500 cell Name or DNS cell name  */
/* Return Value        : D2_NOERROR if value valid                 */
/*                       D2_ERROR   if user cancels with <DEL>     */
/* Author              : Jean-Marc Bonnaudet, SNI BU BA NM 123     */
/* Date of creation    : Fri Feb 18 14:35:12 MEZ 1994              */
/*                                                                 */
/*-----------------------------------------------------------------*/

static signed32 d20_add_principal_name( Ds_at_entry **pr_name_entry_ptr )
{
signed32    fkt;				/* Key pressed		*/
d2_ret_val  ret_value = D2_NOERROR;		/* Return value		*/
static char principal_name[] = D2_PR_NAME;	/* Attribute name in	*/
						/* attribute list	*/
D20_omask   sav_pr_name_omask[D20_M_SIZE];
static Ds_at_entry pr_name_entry;
static char pr_name_val[D20_ATVAL_LEN + 2];


    /*** Display mask and query this principle name ***/

    /** Initialize mask **/
    d20_save_mask( d20_7omask, sav_pr_name_omask );

    /* This d20_save_mask changes the CGET fields in CSAY fields,	*/
    /* but here we need CGET, let's change it back...			*/
    if(rbatch)
	d20_7omask[D20_EA1_POS+1].d20_fkt_id = D20_CGET;
    d20_7omask[D20_EA1_POS+3].d20_fkt_id = D20_CGET;
    d20_7omask[D20_EA1_POS+4].d20_fkt_id = D20_CGET;

    strcpy(d20_7ifields[DS_EA1_NAME], principal_name);	/* attr. name	*/
    d20_7ifields[DS_EA1_1VAL][0] = D2_EOS;	/* value line 1	*/
    d20_7ifields[DS_EA1_2VAL][0] = D2_EOS;	/* value line 2	*/

    /* We do not need a second value here */
    d20_7omask[D20_EA2_POS].d20_fkt_id = D20_NO_OP;	/* "Name:"	*/
    d20_7omask[D20_EA2_POS+1].d20_fkt_id = D20_NO_OP;	/* attr. name	*/
    d20_7omask[D20_EA2_POS+2].d20_fkt_id = D20_NO_OP;	/* "Value:"	*/
    d20_7omask[D20_EA2_POS+3].d20_fkt_id = D20_NO_OP;	/* value line 1	*/
    d20_7omask[D20_EA2_POS+4].d20_fkt_id = D20_NO_OP;	/* value line 2	*/

    /* We do not need a third value also ! */
    d20_7omask[D20_EA3_POS].d20_fkt_id = D20_NO_OP;	/* "Name:"	*/
    d20_7omask[D20_EA3_POS+1].d20_fkt_id = D20_NO_OP;	/* attr. name	*/
    d20_7omask[D20_EA3_POS+2].d20_fkt_id = D20_NO_OP;	/* "Value:"	*/
    d20_7omask[D20_EA3_POS+3].d20_fkt_id = D20_NO_OP;	/* value line 1	*/
    d20_7omask[D20_EA3_POS+4].d20_fkt_id = D20_NO_OP;	/* value line 2	*/


    /** Display mask **/
    for(;;)
    {
	fkt = d20_handlemask(d20_7omask);

	/*  "break" was selected		*/
	if (fkt == FKTDEL)
	    {
	    ret_value = D2_ERROR;
	    break;
	    }
	else if ( fkt == FKTMENU || fkt == FKTCR )
	    {
	    strcpy(pr_name_val, d20_7ifields[DS_EA1_1VAL]);
	    strcat(pr_name_val, d20_7ifields[DS_EA1_2VAL]);

	    if(d20_check_pr_name_format( (Print_string *)pr_name_val )
		    == D2_ERROR)
		{	/* Print an error message	*/
		d20_qomask[D20_Q1POS].d20_ioinfo = d20_err_table[D20_E120IDX];
		d20_handlemask(d20_qomask);
		}
	    else break;
	    }

	/* Something else pressed ? Display the mask again ! */
    }

    d20_restore_mask( d20_7omask, sav_pr_name_omask );


    /*** Add the principle name in a Ds_a_value structure ***/

    if( ret_value != D2_ERROR )
    {
	/* Already done before */
	/*
	strcpy(pr_name_val, d20_7ifields[DS_EA1_1VAL]);
	strcat(pr_name_val, d20_7ifields[DS_EA1_2VAL]);
	*/

	pr_name_entry.attr_id.d2_typ_len = (signed32)(sizeof(D2_PR_NM) - 1);
						/* exclude EOS character */
	pr_name_entry.attr_id.d2_type    = (Ob_id_string)D2_PR_NM;
	pr_name_entry.hex_use            = FALSE;     /* Bool  */
	pr_name_entry.mod_type           = D2_ADDATT; /* short */
	pr_name_entry.a_val.d2_a_v_len   = (signed16)strlen(pr_name_val);
	pr_name_entry.a_val.d2_a_rep     = (unsigned16)D2_PRINTABLE;
	pr_name_entry.a_val.d2_at_value.d2_prt_a_v = (Print_string)pr_name_val;

	*pr_name_entry_ptr = &pr_name_entry;  /* This is an output parameter */
    }
    else
	*pr_name_entry_ptr = (Ds_at_entry *)NULL;

    return( ret_value );
}



/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name       : d20_check_pr_name_format                  */
/* Description         : Check the format of a principal name      */
/*                       attribute.                                */
/*                       Algorithm: very simple: just check        */
/*                       if the string begins with /.../           */
/* Return Value        : D2_NOERROR if the format is correct       */
/*                       D2_ERROR   if the format is NOT correct   */
/* Author              : Jean-Marc Bonnaudet, SNI BU BA NM 123     */
/* Date of creation    : Tue Feb 22 15:05:49 MEZ 1994              */
/*                                                                 */
/*-----------------------------------------------------------------*/

static signed32 d20_check_pr_name_format( Print_string *pr_name )
{
static const char printable_string_begin[] = "/.../";
d2_ret_val    return_value;


    if( strncmp((char *)pr_name,
		printable_string_begin,
		strlen(printable_string_begin)) == 0 )
	return_value = D2_NOERROR;
    else
	return_value = D2_ERROR;

    return( return_value );
}



/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name       : d20_search_auth_mech()                    */
/* Description         : Search in the config file                 */
/*                       (/opt/dcelocal/etc/gdsconfig)             */
/*                       which authentication mechanism is used    */
/*                       for the given Directory-id                */
/* Return Value        : D2_NOERROR / D2_ERROR                     */
/* Author              : Jean-Marc Bonnaudet, SNI BU BA NM 123     */
/*                                                                 */
/*-----------------------------------------------------------------*/
static int d20_search_auth_mech( int my_dir_id, int *my_auth_mech )
{
char	gdsconfig_file_name[D27_LFILE_NAME];
char	line[GDSCONFIG_LINE_LEN];	/* One line of gdsconfig	*/
char	word[GDSCONFIG_LINE_LEN];	/* One word of gdsconfig	*/
char	*current_ptr;			/* A pointer on 'line'		*/
FILE	*fd;				/* File descriptor for gdsconfig*/
Bool	dir_id_found = FALSE;		/* Line for current dir-id found*/
Bool	auth_mech_found = FALSE;	/* Line for current dir-id found*/
int	return_value = D2_ERROR;	/* return value			*/


    /* Open gdsconfig */
    sprintf(gdsconfig_file_name, "%s%s%s",
	    dcelocal_path, D27_ETC_DIR, D27_GDSCONFIG);

    if( ( fd = fopen(gdsconfig_file_name, "r") ) != NULL )
    {
	/* Search the authentication mechanism in gdsconfig */
	while( !auth_mech_found &&
		(fgets(line, GDSCONFIG_LINE_LEN, fd) != NULL) )
	{
	    current_ptr = line;
	    while( !auth_mech_found && (current_ptr < line + strlen(line)) )
	    {
		sscanf( current_ptr, "%s", word );
		if( !strcmp( word, DIR_ID_KEY ))
		{
		    current_ptr += strlen(word) + 1;
		    sscanf( current_ptr, "%s", word );
		    if(atoi(word) == my_dir_id)
		    {
			dir_id_found = TRUE;
		    }
		}
		if( dir_id_found && !strcmp( word, AUTH_MECH_KEY ))
		{
		    current_ptr += strlen(word) + 1;
		    sscanf( current_ptr, "%s", word );
		    *my_auth_mech = atoi( word );
		    auth_mech_found = TRUE;
		    return_value = D2_NOERROR;
		    break;
		}
		current_ptr += strlen(word) + 1;
	    }
	}

	fclose(fd);
    }

    return( return_value );
}



/* End of d20getat.c */
