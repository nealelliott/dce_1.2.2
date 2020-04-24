/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20displst.c,v $
 * Revision 1.1.722.2  1996/02/18  19:40:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:21:31  marty]
 *
 * Revision 1.1.722.1  1995/12/08  15:16:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:47  root]
 * 
 * Revision 1.1.720.4  1994/06/21  14:43:33  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:23:39  marrek]
 * 
 * Revision 1.1.720.3  1994/05/10  15:49:29  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:10:53  marrek]
 * 
 * Revision 1.1.720.2  1994/03/23  15:05:13  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:08:44  keutel]
 * 
 * Revision 1.1.720.1  1994/02/22  15:58:20  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:28:04  marrek]
 * 
 * Revision 1.1.718.2  1993/10/14  16:46:24  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  16:43:36  keutel]
 * 
 * Revision 1.1.718.1  1993/10/13  17:29:29  keutel
 * 	October 1993 code drop
 * 	[1993/10/11  07:34:45  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  09:59:34  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:27:34  marrek]
 * 
 * Revision 1.1.4.5  1993/01/29  08:34:17  marrek
 * 	Correct use of F8 function key.
 * 	[1993/01/28  10:25:41  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  17:49:15  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:03:52  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/18  08:27:52  marrek
 * 	Various bug fixes for mask handling found at SNI
 * 	[1992/12/17  16:27:37  marrek]
 * 
 * Revision 1.1.4.2  1992/11/27  13:10:03  marrek
 * 	November 1992 code drop
 * 	[1992/11/26  08:51:55  marrek]
 * 
 * Revision 1.1.2.3  1992/06/30  20:53:53  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:32:37  melman]
 * 
 * Revision 1.1.2.2  1992/06/01  21:52:47  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:15:21  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20displst.c,v $ $Revision: 1.1.722.2 $ $Date: 1996/02/18 19:40:34 $";
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
/* NAME         : displist.c  [Administrations-Schnittstelle]         */
/*								      */
/* AUTHOR       : Volpers, D AP 11                                    */
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
/*      0.2 | 08.12.88| Fehlermeldung display default  | hv |     45  */
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
#include <stdlib.h>
#include <string.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <gdsext.h>
#include <d2adm.h>
#include <d2msk.h>
#include <cget.h>
#include <d21iapl.h>
#include <d2spec_msk.h>
#include <d20proto.h>

/**********************************************************************/
/*								      */
/*			  D E F	I N E S				      */
/*								      */
/**********************************************************************/

#define HEX             "x'"
#define D20_ATTR_COUNT  3

#define SUMMARY_LINES_PER_PAGE 16

#define PREVIOUS -1
#define SAME      0
#define NEXT      1
#define EXTRA_ATTR	10

#define	MAX_T61_DISP_STR_LEN	(D20_MAX_DISP_STR_LEN * D20_T61_CONV_FACT)

/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*		   D E C L A R A T I O N S       DATA		      */
/*								      */
/**********************************************************************/

extern char      *d20_bool[];

extern D20_omask d20_6bomask[];        /* Input fields for mask 6b    */

extern D20_omask d20_4aomask[];        /* Input fields for mask 4a    */
extern char      *d20_4aifields[];     /* Mask 4a                     */

extern D20_omask d20_7omask[];          /* Input fields for mask 7    */
extern char      *d20_7ifields[];       /* Mask 7                     */

extern D20_omask d20_7aomask[];         /* Input fields for mask 7a    */
extern char      *d20_7aifields[];      /* Mask 7                      */
extern char      *d20_6bifields[];

extern D20_omask d20_msgomask[];       /* Input fields for message     */
				       /* mask                        */
extern D20_omask d20_qomask[];         /* Input fields for error mask  */
extern char      *d20_errtab[];        /* Error mask                  */
extern char      *d20_errmess[];       /* Error messages              */
extern char      *d20_err_table[];     /* and more error messages     */
extern D20_omask d20_12omask[];    
extern D20_omask d20_12bomask[];    
extern char      *d20_12ifields[];  
extern char      *d20_12bifields[];  
extern char      *d20_12barmenu[];
extern D20_omask d20_13omask[];
extern char      *d20_13ifields[];
extern char      *d20_13ofields[];
extern D20_omask d20_14omask[];
extern char      *d20_14oheader[];

extern  D20_omask  d20_t61omask[] ;
extern  D20_omask  d20_ttxidomask[] ;                                           
extern  D20_omask  d20_telexomask[] ;
extern  D20_omask  d20_faxomask[] ;
extern  D20_omask  d20_dnscellomask[];
extern  D20_omask  d20_dnsrepomask[];

extern D2_ei_info *d20_entptr;
extern signed32    getwerg;
extern signed32    rbatch;
extern char	getfill;
extern D20_at		*d2_p_d20at;
extern signed32		 d2_nd20at;
extern D20_oct		*d2_p_d20oct;
extern signed32		 d2_nd20oct;

static signed32 object_counter;
static signed32 max_attr_cnt; /* space allocated for so many summary items */
			      /* in case of attribute list handling.	   */
static signed32 display_all;  /* display all objects without selecting */
static signed32 cursor_stay;
			/* if true, don't reset cursor to beginning of page */
static struct summary_item /* a summary list item */
	{
	char summary_name[MAX_T61_DISP_STR_LEN + 1];
	signed32  selected;    /* is the object selected for display ? */
	D2_ei_info *ei_info; /* for fetching attribute information */
			     /* and for fetching object information */
	} *first_list;
static struct summary_list_type /* list of all objects matching search params */
	{
	signed32     max_page;  /* maximum number of pages of summary list*/
	signed32     current_page; /*currently displayed page of summary list*/
	signed32     current_line; /*currently active line of summary page */
	signed32     current_display; /* object currently selected for display*/
	signed32     num_selected; /*number of objects with selected_flag set*/
	struct summary_item *summary_list; /* pointer to an item */
	} summary;

extern	Bool 	d20_cache;
extern	Bool 	d20_shadow;

extern	At	*d2_p_at;
extern	Oct	*d2_p_oct;
extern	Srt	*d2_p_srt;
extern	signed32 d2_nat;
extern	signed32 d2_noct;
extern	signed32 d2_nsrt;

static	D20_at	dsa_type;

/**********************************************************************/
/*								      */
/*		  D E C L A R A T I O N S      FUNCTIONS	      */
/*								      */
/**********************************************************************/

/******** LOCAL  ********/

static	signed32 d20_show_attr(signed16 attr_mode);
static	void     d20_disp_val(signed16 len, Octet_string val, signed16 at_pos,
			      signed16	syntax);
static	signed16 d20_attr(D2_a_type *a_type, char **a_name, signed16 *a_syntax);
static	void     d20_600_display_objects(String tree);
static	void     d20_610_display_errobj(D2_name_string *old_objects);
static	void     d20_611_convert_dn(char *name, char *field);
static	signed32 d20_612_display_mask_20(void);
static	void     d20_613_set_mask_data_20(void);
static	void     d20_614_handle_key_20(signed32 key_pressed);
static	void     display_objects(signed32 key_pressed, String tree, 
			Bool m18_on);
static	void     prep_disp_data(String tree);
static	void     fill_disp_lists(char *name);
static	signed32 display_mask_6(char *tree);
static	void     set_mask_data_6(char *tree);
static	signed32 display_mask_18(void);
static	void     set_mask_data_18(void);
static	void     scroll(signed32 scroll_mode);
static	void     print_summary_list(void);
static	void     handle_barmenu(void);
static	signed32 get_selected_object(signed32 mode);
static	signed32 summarycmp(struct summary_item *s_i1,
			struct summary_item *s_i2);
static	signed32 dnrevcmp(String str1, String str2);
static	signed32 make_attr_list( signed32 operation, D20_srt *srt_ent,
			D20_oct	*oct_ent, signed32 *mand_cnt,
			signed32 *opt_cnt);
static	signed32 make_schema_list(signed32 operation,
			signed32 *schema_index_space,
			signed32 no_selected_entries);
static	signed32 display_attr_list(signed32 operation, signed32 mand_cnt,
			D20_oct	*oct_ent);
static	signed32 get_attr_list_rbatch(D20_oct *oct_ent, signed32 operation,
			D20_at	***attr_ptr_space, signed32 *no_selected_attrs);
static	signed32 get_aux_obj_rbatch(D20_oct *oct_ent, Bool bool_array[]);
static	signed32 search_in_valid_attrs(char *attr_name);
static	D20_at	 *get_attr_ptr(D20_oct	*oct_ent, char	*attr_name);
static	signed32 display_oclass_sch(void);
static	signed32 extend_attr_list(D20_oct *, Bool *);

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : ds0_504_display_list      [display_list]              */
/*								      */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        04.09.87                                              */
/*								      */
/* SYNTAX:      void  ds0_504_display_list(tree)   	      	      */
/*								      */
/*              String            tree;                               */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function determines which display function to    */
/*              execute, then executes it, passing the input params   */ 
/*		along to the appropriate function.   		      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              String            tree;         indicates structure   */
/*                                              of entries            */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              None                                                  */
/*								      */
/* RETURNVALUE:							      */
/*              None                                                  */
/*								      */
/* STANDARD-HEADER: None                                              */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/*exoff ***************************************************************/

void ds0_504_display_list(String tree)
{

   char  *val;
   signed16 mode = D20_BEGIN;
   D20_omask    sav_6bomask[D20_M_SIZE];
   D20_omask    sav_7omask[D20_M_SIZE];
   D20_omask    sav_7aomask[D20_M_SIZE];

if (rbatch)
{   return ;
}

d20_save_mask(d20_6bomask, sav_6bomask);
d20_save_mask(d20_7omask, sav_7omask);
d20_save_mask(d20_7aomask, sav_7aomask);

object_counter = 0; 

/* get next object name.	*/
while (!d20_dnget(mode,(D2_name_string *) &val))
	{
	/* compare object structure of val with tree.	*/
	if (d20_cmp_structure(val, tree) == TRUE)
		object_counter++; /* count # of objects in hit list */
        mode = D20_CONTINUE;
	}

if (object_counter == 0)
{       d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab[D20_E19IDX];
        d20_handlemask(d20_qomask); /* no objects */
}
else
{       summary.max_page = object_counter % SUMMARY_LINES_PER_PAGE;
        if (!summary.max_page)
	     summary.max_page = object_counter / SUMMARY_LINES_PER_PAGE;
        else
	     summary.max_page = object_counter / SUMMARY_LINES_PER_PAGE + 1;

        if ( (summary.summary_list = (struct summary_item *)malloc
	               (object_counter * sizeof(struct summary_item))) == 0)
	{
		d20_qomask[D20_Q1POS].d20_ioinfo = d20_err_table[D20_E04IDX];
  		d20_handlemask(d20_qomask); /* memory error */
	}
        else
	{
		summary.current_page = 1;
		summary.current_line = 1;
		summary.num_selected = 0;
		summary.current_display = 0;
		d20_600_display_objects(tree);
		free(first_list);
	}
}

d20_restore_mask(d20_6bomask, sav_6bomask);
d20_restore_mask(d20_7omask, sav_7omask);
d20_restore_mask(d20_7aomask, sav_7aomask);
}

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : ds0_530_display_errlist   [display_errlist]           */
/*								      */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        21.12.89                                              */
/*								      */
/* SYNTAX:      void  ds0_530_display_errlist(old_objects,head_no)    */
/*								      */
/*              D2_name_string   *old_objects;                        */
/*              signed16          head_no;                            */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function counts the objects to be displayed and  */
/*              allocates memory for them and calls the displaying    */
/*              function.                                             */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              old_objects       Name_string * pointer to list of    */
/*                                              erroneous names       */
/*              head_no           signed16      number of headline    */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              None                                                  */
/*								      */
/* RETURNVALUE:							      */
/*              None                                                  */
/*								      */
/* STANDARD-HEADER: None                                              */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*              d20_err_table   char * []   error messages            */
/*              d20_14oheader   char * []   array of headlines        */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*              object_counter  signed32    count of objects in       */
/*                                          error-list                */
/*              first_list      struct summary_item *   first element */
/*                                          of error-list             */
/*              d20_qomask      D20_omask   errormask to display      */
/*              d20_14omask     D20_omask   mask to display           */
/*              summary         struct summary_list_type    Array of  */
/*                                      objects to display (converted)*/
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*              d20_handlemask()                                      */
/*              d20_610_display_errobj()                              */
/*								      */
/*exoff ***************************************************************/

void ds0_530_display_errlist(D2_name_string *old_objects, signed16 head_no)
{                       /*  ds0_530_display_errlist                   */

if (rbatch)
{   
    return ;
} 

for                     /*  count all objects in list                 */
   (object_counter = 0; old_objects[object_counter] != NULL;
    object_counter++);

if                      /*  objects in list                           */
  (object_counter > 0)
{                       /*  initialize mask                           */
  d20_14omask[1].d20_ioinfo = d20_14oheader[head_no];

  summary.max_page = object_counter % SUMMARY_LINES_PER_PAGE;
  if (!summary.max_page)
	summary.max_page = object_counter / SUMMARY_LINES_PER_PAGE;
  else
	summary.max_page = object_counter / SUMMARY_LINES_PER_PAGE + 1;

  if ( (summary.summary_list = (struct summary_item *)malloc
	 (object_counter * sizeof(struct summary_item))) == 0)
	{
		d20_qomask[D20_Q1POS].d20_ioinfo = d20_err_table[D20_E04IDX];
  		d20_handlemask(d20_qomask); /* memory error */
	}
  else
	{
		summary.current_page = 1;
		summary.current_line = 1;
		summary.num_selected = 0;
		summary.current_display = 0;
		first_list = summary.summary_list;
		d20_610_display_errobj(old_objects);
		free(first_list);
	}
}                       

}                       /*  ds0_530_display_errlist                   */

/*inon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_550_disp_def_dsa                                  */
/*								      */
/* AUTHOR:      Volpers,  D AP 11                                     */
/* DATE:        01.08.88                                              */
/*								      */
/* SYNTAX:      void d20_550_disp_def_dsa ()                          */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will display the local and default      */
/*              DSA.                                                  */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              None                                                  */
/*								      */
/* RETURNVALUE:							      */
/*              None                                                  */
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
/*inoff ***************************************************************/

void d20_550_disp_def_dsa()
{                               /*  d20_550_disp_def_dsa()            */

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

   signed16       i = 0;
   signed16       mode = D20_BEGIN;
   D2_a_type      a_type;       /* attribute type                 */
   signed16       a_len;        /* attribute length               */
   Octet_string   a_val;        /* attribute value                */
   D2_a_value    *attr_val;
   Bool           loc_dsa = TRUE;
   Bool           def_dsa = TRUE;
   signed16       flen = D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT;
   Bool           mask_full;
   Bool           first_mask;

static D2_pbhead head = { D21V022 } ;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

				  /* read special entries from cache  */
    d20_166_set_all_attr();

    d20_162_set_name(D2_N_LOC_DSA);
    if (d20_a0_read(&head) == D2_NOERROR)
    {                             /* fill mask                       */
      if (!d20_get_attribute(mode, &a_type, &attr_val))
      {   
	a_val = attr_val->d2_at_value.d2_oct_a_v;
	if (d20_gen_dnintf((String) a_val) != D2_NOERROR)
		{
		d20_232_spec_exit();
		}
	a_len = (signed16)(strlen((char *)a_val) + 1)  /* including EOS */;

	strncpy(d20_4aifields[DS_SPLOC], (char *)a_val, flen);
	if                        
	   (a_len > flen)
	{                         /* fill second line                */
	  strncpy(d20_4aifields[DS_SP1LOC], (char *)(a_val + flen), flen);
	}
else
{
	  *d20_4aifields[DS_SP1LOC] = D2_EOS;
	} 
      }
else
{
	loc_dsa = FALSE;
	*d20_4aifields[DS_SPLOC] = D2_EOS;
	*d20_4aifields[DS_SP1LOC] = D2_EOS;
      } 
    }
else
{
      loc_dsa = FALSE;
    } 

    d20_166_set_all_attr();
				  /* read default dsa from cache     */
    d20_162_set_name(D2_N_DEF_DSA);
    if (d20_a0_read(&head) == D2_NOERROR)
    {   
      if (d20_get_attribute(D20_BEGIN, &a_type, &attr_val) !=
			    D2_NOERROR)
      {   
	def_dsa = FALSE;
      }
else
{
	a_val = attr_val->d2_at_value.d2_oct_a_v;
	a_len = attr_val->d2_a_v_len;
      } 
    }
else
{
      def_dsa = FALSE;
    } 

    *d20_4aifields[DS_SPBREAK] = DS_DEFAULT_VALUE;

    if                          /* no local and no default dsa exist  */
      (loc_dsa == FALSE && def_dsa == FALSE)
    {                           /* error message                      */
      d20_qomask[D20_Q1POS].d20_ioinfo =
				  d20_errmess[map_errnum(head.d2_errvalue)];
      d20_handlemask (d20_qomask) ;
    }
else
{
      for (first_mask = TRUE;;) {
	    mask_full = TRUE;
	    for (i = 0; i < 4 && !d20_get_attribute(mode, &a_type,
						    &attr_val); i++)
	    { 
	      a_val = attr_val->d2_at_value.d2_oct_a_v;
	      a_len = attr_val->d2_a_v_len;
	if (d20_gen_dnintf((String) a_val) != D2_NOERROR)
		{
		d20_232_spec_exit();
		}
	a_len = (signed16)(strlen((char *)a_val) + 1)  /* including EOS */;
	      strncpy(d20_4aifields[2*i+2], (char *)a_val, flen);
	      if                      
		 (a_len > flen)
	      {                       /* fill second line                */
		strncpy(d20_4aifields[2*i+3], (char *)(a_val + flen), flen);
	      }
else
{
		*d20_4aifields[2*i+3] = D2_EOS;
	      } 
	      mode = D20_CONTINUE;
	    } 
	    if (i == 0 && first_mask == FALSE)
	    {                        /* no more default-DSAs */
	      break;
	    } 
	    for ( ; i < 4; i++)
	    { 
	      *d20_4aifields[2*i+2] = D2_EOS;
	      *d20_4aifields[2*i+3] = D2_EOS;
	      mask_full = FALSE;
	    } 
	    first_mask = FALSE;

	    d20_handlemask(d20_4aomask);
	    if (mask_full == FALSE)
			break;
	    
      }
    } 

}                               /*  d20_550_disp_def_dsa()            */


/*inon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_show_attr()           [show attributes]           */
/*								      */
/* AUTHOR:      Schmid,  D AP 11                                      */
/* DATE:        16.05.88                                              */
/*								      */
/* SYNTAX:      signed32 d20_show_attr(attr_mode)    	              */
/*              signed16          attr_mode;                          */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will display the attributes of the      */
/*              handled object.                                       */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              signed16          attr_mode;    a flag indicating     */
/*                                              whether the attributes*/
/*                                              of the first DN       */
/*                                              should be handled or  */
/*                                              not                   */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              None                                                  */
/*								      */
/* RETURNVALUE:							      */
/*              Returns the value of key pressed during attribute     */
/*		display.					      */
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
/*inoff ***************************************************************/

static  signed32 d20_show_attr(signed16 attr_mode)
{                               /*  d20_show_attr                     */

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


   Ds_property       prty;

   D2_a_type         a_type;    /* attribute type                 */
   D2_a_type         last_type;    /* attribute type                 */
   signed16          a_len;     /* attribute length               */
   Octet_string      a_val;     /* attribute value                */
   D2_a_value        *attr_val;

   D20_oct		 *oct_ent;
   signed32	 i;

   D2_a_type         acl_type;  /* ACL attribute type             */
   signed16          acl_len;   /* ACL attribute length           */
   Octet_string      acl_val;   /* ACL attribute value            */
   Bool              acl_exist = FALSE;

   Bool              sp_exist = FALSE;
   signed32	     fkt;

				    /* number of attributes in mask   */
   signed16          attr_count = 0;
   char             *a_name;    /* attribute name in AT           */
   signed16          syntax;

   D2_obj_id         obj_id;
   char              obj_id_str[D2_OBJ_IDL_MAX + 1];
   byte              a_int_val[(D20_ATVAL_LEN * D20_T61_CONV_FACT) + 1];
   char              intlst[(D20_ATVAL_LEN * D20_T61_CONV_FACT) + 1];

static signed16          at_pos[3] = { D20_EA1_POS, D20_EA2_POS,
					D20_EA3_POS };

	d2_ret_val	ret_value;


/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

*d20_6bifields[DS_DN16_SCU] = *d20_6bifields[DS_DN17_F1] =
*d20_6bifields[DS_DN13_BREAK] = *d20_6bifields[DS_DN14_MORE] =
				DS_DEFAULT_VALUE;

				/*  enable SCROLL_UP and F1 and       */
				/*  SCROLL_DOWN in attribute mask     */
if (object_counter != 1)
{   
  d20_7omask[D20_EA_SCU_POS].d20_fkt_id =
  d20_7omask[D20_EA_F1_POS].d20_fkt_id =
  d20_7omask[D20_EA_SCD_POS].d20_fkt_id = D20_FKTX;
}
else
{
  d20_7omask[D20_EA_SCU_POS].d20_fkt_id = D20_FKTX;
  d20_7omask[D20_EA_SCD_POS].d20_fkt_id = D20_NO_OP;
} 

/*  attributes found                  */
while (!d20_get_attribute(attr_mode, &a_type, &attr_val))
	{
	attr_mode = D20_CONTINUE;

	/*  look for attribute name in AT    */
	if (d20_attr(&a_type, &a_name, &syntax))
		{
		/* attribute not found in schema	*/
		continue;
		}

	if (attr_val->d2_a_rep == D2_ASN1)
		syntax = D2_OCTET;

	for (;;)
		{
		/* attribute requires merging of attribute values. 	*/
		if (syntax == D2_PR_DM_STX || syntax == D2_MHS_PR_DM_STX)
			{
			a_val = (Octet_string)&intlst[0];
			a_len = 0;

			last_type.d2_typ_len = a_type.d2_typ_len;
			last_type.d2_type = a_type.d2_type;

			/* merge attribute values of same type of attribute. */
			do	
				{
				sprintf((char *)a_int_val, "%d ",
					attr_val->d2_at_value.d2_int_a_v);

				if ((int)(a_len + strlen((char *)a_int_val)) > 
				    (D20_ATVAL_LEN * D20_T61_CONV_FACT))
					{
					if ((a_len
					    + strlen((char *)a_int_val)
					    - 1) == 
					    (D20_ATVAL_LEN * D20_T61_CONV_FACT))
						{
						strncpy(&intlst[a_len], 
						    (char *)a_int_val,
						    strlen((char *)a_int_val)
							-1);
						a_len += strlen(&intlst[a_len])
							 + 1;
						}

					break;	/* break from while loop. */
						/* no more space left.    */
					}
				else
					{
					strcpy(&intlst[a_len],
					    (char *)a_int_val);
					a_len += strlen(&intlst[a_len]);
					}
				} 
			while (((ret_value = d20_get_attribute(attr_mode, 
				&a_type, &attr_val)) == D2_NOERROR) && 
				d20_cmp_objid(&a_type, last_type.d2_typ_len+1, 
				last_type.d2_type));

			a_len--;

			strcpy(d20_7omask[at_pos[attr_count] + 1].d20_ioinfo, 
			       a_name);
			d20_disp_val(a_len, a_val, at_pos[attr_count], syntax);
			attr_count++;

			/*  attribute mask is filled          */
			if (attr_count == D20_ATTR_COUNT)
				{/*  display attribute mask            */
				fkt = d20_handlemask(d20_7omask);


				if ((fkt == FKTDEL) || (fkt == FKTSCU) || 
				    (fkt == FKTF1) || (fkt == FKTSCD))
					{
					/*  disable SCROLL_UP and F1 in       */
					/*  attribute mask                    */
					d20_7omask[D20_EA_SCU_POS].d20_fkt_id =
					d20_7omask[D20_EA_F1_POS].d20_fkt_id = 
								D20_NO_OP;

					/*  enable SCROLL_DOWN in attribute   */
					/*  mask                              */
					d20_7omask[D20_EA_SCD_POS].d20_fkt_id = 
								D20_FKTX;

					return(fkt);
					}
				else /*  reset attribute counter           */
					attr_count = 0;
				}

			/* look in AT, for name of last attribute read.	*/	
			while (d20_attr(&a_type, &a_name, &syntax) &&
			       (ret_value = d20_get_attribute(attr_mode, 
				&a_type, &attr_val) == D2_NOERROR))
				{
				/* attribute not found in schema	*/
				continue;
				}

			/* no more attributes left in the list.	*/
			if (ret_value == D2_ERROR)
				break;

			/* loop to check syntax of last attribute.	*/
			}
		else
			break;
		}

	/* no more attributes left in the list.	*/
	if (ret_value == D2_ERROR)
		break;

  	a_val = attr_val->d2_at_value.d2_oct_a_v;
  	a_len = attr_val->d2_a_v_len;

  	/*  attribute is ACL                  */
     	if (d20_cmp_objid(&(a_type),sizeof(D2_ACL),(Ob_id_string) D2_ACL))
    		{
		acl_exist = TRUE;
    		acl_len = a_len;
    		acl_val = a_val;
    		acl_type = a_type;
    		continue;
		}

	/* attribute requires special mask handling	*/
	if ((d20_cmp_objid(&(a_type),sizeof(D2_CDS_CELL),
				(Ob_id_string) D2_CDS_CELL)) ||
	    (d20_cmp_objid(&(a_type),sizeof(D2_CDS_REPLICA),
				(Ob_id_string) D2_CDS_REPLICA)) ||
	    (d20_cmp_objid(&(a_type),sizeof(D2_DME_ALT_ADDR),
				(Ob_id_string) D2_DME_ALT_ADDR)))
		{
    		sp_exist = TRUE;
		continue;
		}

	switch((int)attr_val->d2_a_rep)
	  {
	  case D2_MHS_ORADR_STX:
	  case D2_MHS_ORNAME_STX:
	  case D2_MHS_DLSP_STX:
	  case D2_TTXID_STX:
	  case D2_TLXNR_STX:
	  case D2_FAXNR_STX:
	  case D2_T61_LIST:
	  case D2_T61_PR_LIST:
	  case D2_PSAP_STX:
	  case D2_CERTIFICATE:
	  case D2_CERT_PAIR:
	  case D2_CERT_LIST:
    		sp_exist = TRUE;
		continue;
	  }

  	/*  attr. belongs to scheme and no display*/
     	if (d20_cmp_objid(&(a_type),sizeof(D2_AT),(Ob_id_string) D2_AT) ||	
      	    d20_cmp_objid(&(a_type),sizeof(D2_OCT),(Ob_id_string) D2_OCT) ||	
      	    d20_cmp_objid(&(a_type),sizeof(D2_SRT),(Ob_id_string) D2_SRT))	
		{
    		continue;
		}

	/*  display attribute                 */

      	if (syntax == D2_DISTNAME)
		{
		if (d20_gen_dnintf((String) a_val) != D2_NOERROR)
			d20_232_spec_exit();

		a_len = (signed16)strlen((char *)a_val);
		}

	if (syntax == D2_INTEGER)
    		{
		sprintf((char *)a_int_val, "%d", 
					attr_val->d2_at_value.d2_int_a_v);
		a_val = &a_int_val[0];
		a_len = (signed16)strlen((char *)a_val);
		}

	if (syntax == D2_BOOLEAN)
    		{
		if (attr_val->d2_at_value.d2_bool_a_v == TRUE)
		      {
		      a_val = (Octet_string)d20_bool[0];
		      a_len = strlen(d20_bool[0]);
		      }
		if (attr_val->d2_at_value.d2_bool_a_v == FALSE)
		      {
		      a_val = (Octet_string)d20_bool[1];
		      a_len = strlen(d20_bool[1]);
		      }
		}

	if (syntax == D2_OBJ_IDENT)
		if (d20_cmp_objid(&(a_type),sizeof(D2_OBJ_CLASS),
				(Ob_id_string) D2_OBJ_CLASS))
			{
			for(i=0,oct_ent=d2_p_d20oct; i<d2_nd20oct; i++,oct_ent++)
				if (d20_cmp_objid(&(oct_ent->obj_id),a_len + 1,a_val))
					break;
	
			if (i != d2_nd20oct)
				{
				/* value of the attr. exists in OCT ,display */
				/* display Object class abbreviation.	     */

				a_val = (Octet_string)oct_ent->obj_abbrv;
				a_len = (signed16)strlen((char *)a_val);
				}
			else
				{
				obj_id.d2_type = 
					attr_val->d2_at_value.d2_obid_a_v;
				obj_id.d2_typ_len = attr_val->d2_a_v_len;
				d27_301_oid_str (&obj_id, obj_id_str);
				a_val = (Octet_string)obj_id_str;
				a_len = (signed16)strlen((char *)a_val);
				}
			}
		else
			{
			obj_id.d2_type = attr_val->d2_at_value.d2_obid_a_v;
			obj_id.d2_typ_len = attr_val->d2_a_v_len;
			d27_301_oid_str (&obj_id, obj_id_str);
			a_val = (Octet_string)obj_id_str;
			a_len = (signed16)strlen((char *)a_val);
			}

      	strcpy(d20_7omask[at_pos[attr_count] + 1].d20_ioinfo, a_name);
      	d20_disp_val(a_len, a_val, at_pos[attr_count], syntax);
      	attr_count++;

      	/*  attribute mask is filled          */
	if (attr_count == D20_ATTR_COUNT)
      		{/*  display attribute mask            */
		fkt = d20_handlemask(d20_7omask);

	   	if ((fkt == FKTDEL) || (fkt == FKTSCU) || (fkt == FKTF1) ||
		    (fkt == FKTSCD))
			{
			/*  disable SCROLL_UP and F1 in       */
			/*  attribute mask                    */
	     		d20_7omask[D20_EA_SCU_POS].d20_fkt_id =
	     		d20_7omask[D20_EA_F1_POS].d20_fkt_id = D20_NO_OP;

			/*  enable SCROLL_DOWN in attribute   */
			/*  mask                              */
	     		d20_7omask[D20_EA_SCD_POS].d20_fkt_id = D20_FKTX;

	     		return(fkt);
			}
		else /*  reset attribute counter           */
	     		attr_count = 0;
		}

	}

/*  still some attributes are left to be displayed 	*/
if (attr_count != 0)
	{/*  display attribute mask            */
  	for ( ; attr_count < D20_ATTR_COUNT; attr_count++)
		{
		*d20_7omask[at_pos[attr_count] + 1].d20_ioinfo =
		*d20_7omask[at_pos[attr_count] + 3].d20_ioinfo =
		*d20_7omask[at_pos[attr_count] + 4].d20_ioinfo = D2_EOS;
		}

	  	fkt = d20_handlemask(d20_7omask);

   	if ((fkt == FKTDEL) || (fkt == FKTSCU) || (fkt == FKTF1) ||
	    (fkt == FKTSCD))
		{
  		/*  disable SCROLL_UP and F1 in       */
		/*  attribute mask                    */
    		d20_7omask[D20_EA_SCU_POS].d20_fkt_id =
    		d20_7omask[D20_EA_F1_POS].d20_fkt_id = D20_NO_OP;

		/*  enable SCROLL_DOWN in attribute   */
		/*  mask                              */
    		d20_7omask[D20_EA_SCD_POS].d20_fkt_id = D20_FKTX;

    		return(fkt);
		}
	}

/*  disable SCROLL_UP and F1 in       */
/*  attribute mask                    */
d20_7omask[D20_EA_SCU_POS].d20_fkt_id =
d20_7omask[D20_EA_F1_POS].d20_fkt_id = D20_NO_OP;

/*  enable SCROLL_DOWN in attribute   */
/*  mask                              */
d20_7omask[D20_EA_SCD_POS].d20_fkt_id = D20_FKTX;

/* display attributes requiring special masks.	*/

if (sp_exist == TRUE)
	{
	attr_mode = D20_NEXT;
	while (!d20_get_attribute(attr_mode, &a_type, &attr_val))
		{
      		attr_mode = D20_CONTINUE;

  		/*  look for attribute name in AT    */
       		if (d20_attr(&a_type, &a_name, &syntax))
			{
			/* attribute not found in schema	*/
      			continue;
			}

		switch((int)attr_val->d2_a_rep)
		  {          
		  case D2_PSAP_STX:
			prty.ds_value  = attr_val->d2_at_value.d2_oct_a_v;
			prty.ds_length = attr_val->d2_a_v_len;

			/*  enable SCROLL_UP */
			d20_7aomask[D20_SP1POS].d20_fkt_id = D20_FKTX;
			if (object_counter != 1)
				{
				/*  enable F1 and			*/
				/*  SCROLL_DOWN in PSAP mask		*/
				d20_7aomask[D20_SP2POS].d20_fkt_id =
				d20_7aomask[D20_SP3POS].d20_fkt_id = D20_FKTX;
				}

			fkt = ds0_520_handle_spec_attr(DS_DISP_OBJ, &prty);

			/*  disable SCROLL_UP and F1 and      */
			/*  SCROLL_DOWN in PSAP mask          */
			d20_7aomask[D20_SP1POS].d20_fkt_id =
			d20_7aomask[D20_SP2POS].d20_fkt_id =
			d20_7aomask[D20_SP3POS].d20_fkt_id = D20_NO_OP;
			if ((fkt == FKTSCD) || (fkt == FKTSCU) || 
			    (fkt == FKTF1) || (fkt == FKTDEL) || 
			    (fkt == D2_ERROR))
				{
				if (fkt == D2_ERROR)
					return(FKTDEL);
				else
					return(fkt);
				}

			continue;

		  case D2_T61_LIST:
		    /*  enable SCROLL_UP */
		    d20_t61omask[D20_T61SCU].d20_fkt_id = D20_FKTX;
		    if (object_counter != 1)
			    {
			    /*  enable F1 and				*/
			    /*  SCROLL_DOWN in POSTAL-ADDRESS mask	*/
			    d20_t61omask[D20_T61SCD].d20_fkt_id =
			    d20_t61omask[D20_T61F1].d20_fkt_id = D20_FKTX;
			    }

		    fkt = d20_t61(DS_DISPLAY_OBJECTS, a_name,
				    attr_val->d2_at_value.d2_t61lst_a_v);

		    /*  disable SCROLL_UP and F1 and      */
		    /*  SCROLL_DOWN in POSTAL-ADDRESS mask*/
		    d20_t61omask[D20_T61SCU].d20_fkt_id =
		    d20_t61omask[D20_T61SCD].d20_fkt_id =
		    d20_t61omask[D20_T61F1].d20_fkt_id = D20_NO_OP;
		    if ((fkt == FKTSCD) || (fkt == FKTSCU) ||
				(fkt == FKTF1) || (fkt == FKTDEL))
			    { return(fkt); }

		    continue;

		  case D2_T61_PR_LIST:
		    /*  enable SCROLL_UP */
		    d20_t61omask[D20_T61SCU].d20_fkt_id = D20_FKTX;
		    if (object_counter != 1)
			    {
			    /*  enable F1 and				*/
			    /*  SCROLL_DOWN in POSTAL-ADDRESS mask	*/
			    d20_t61omask[D20_T61SCD].d20_fkt_id =
			    d20_t61omask[D20_T61F1].d20_fkt_id = D20_FKTX;
			    }

		    fkt = d20_postal(DS_DISPLAY_OBJECTS, a_name,
				    attr_val->d2_at_value.d2_post_a_v);

		    /*  disable SCROLL_UP and F1 and      */
		    /*  SCROLL_DOWN in POSTAL-ADDRESS mask*/
		    d20_t61omask[D20_T61SCU].d20_fkt_id =
		    d20_t61omask[D20_T61SCD].d20_fkt_id =
		    d20_t61omask[D20_T61F1].d20_fkt_id = D20_NO_OP;
		    if ((fkt == FKTSCD) || (fkt == FKTSCU) ||
				(fkt == FKTF1) || (fkt == FKTDEL))
			    { return(fkt); }

		    continue;

		  case D2_TLXNR_STX:
		    /*  enable SCROLL_UP */
		    d20_telexomask[D20_TLSCU].d20_fkt_id = D20_FKTX;
		    if (object_counter != 1)
			    {
			    /*  enable F1 and			*/
			    /*  SCROLL_DOWN in TELEX mask	*/
			    d20_telexomask[D20_TLSCD].d20_fkt_id =
			    d20_telexomask[D20_TLF1].d20_fkt_id = D20_FKTX;
			    }

		    fkt = d20_telex(DS_DISPLAY_OBJECTS, a_name,
				    attr_val->d2_at_value.d2_tlxnr_a_v);

		    /*  disable SCROLL_UP and F1 and      */
		    /*  SCROLL_DOWN in TELEX mask         */
		    d20_telexomask[D20_TLSCU].d20_fkt_id =
		    d20_telexomask[D20_TLSCD].d20_fkt_id =
		    d20_telexomask[D20_TLF1].d20_fkt_id = D20_NO_OP;
		    if ((fkt == FKTSCD) || (fkt == FKTSCU) ||
				(fkt == FKTF1) || (fkt == FKTDEL))
			    { return(fkt); }

		    continue;

		  case D2_FAXNR_STX:
		    /*  enable SCROLL_UP */
		    d20_faxomask[D20_FXSCU].d20_fkt_id = D20_FKTX;
		    if (object_counter != 1)
			    {
			    /*  enable F1 and			*/
			    /*  SCROLL_DOWN in FAX mask		*/
			    d20_faxomask[D20_FXSCD].d20_fkt_id =
			    d20_faxomask[D20_FXF1].d20_fkt_id = D20_FKTX;
			    }

		    fkt = d20_fax(DS_DISPLAY_OBJECTS, a_name,
				    attr_val->d2_at_value.d2_faxnr_a_v);

		    /*  disable SCROLL_UP and F1 and	*/
		    /*  SCROLL_DOWN in FAX mask		*/
		    d20_faxomask[D20_FXSCU].d20_fkt_id =
		    d20_faxomask[D20_FXSCD].d20_fkt_id =
		    d20_faxomask[D20_FXF1].d20_fkt_id = D20_NO_OP;
		    if ((fkt == FKTSCD) || (fkt == FKTSCU) ||
				(fkt == FKTF1) || (fkt == FKTDEL))
			    { return(fkt); }

		    continue;

		  case D2_TTXID_STX:
		    /*  enable SCROLL_UP */
		    d20_ttxidomask[D20_TTSCU].d20_fkt_id = D20_FKTX;
		    if (object_counter != 1)
			    {
			    /*  enable F1 and			*/
			    /*  SCROLL_DOWN in TTX mask		*/
			    d20_ttxidomask[D20_TTSCD].d20_fkt_id =
			    d20_ttxidomask[D20_TTF1].d20_fkt_id = D20_FKTX;
			    }

		    fkt = d20_ttxid(DS_DISPLAY_OBJECTS, a_name,
				    attr_val->d2_at_value.d2_ttxid_a_v);

		    /*  disable SCROLL_UP and F1 and      */
		    /*  SCROLL_DOWN in TTX mask           */
		    d20_ttxidomask[D20_TTSCU].d20_fkt_id =
		    d20_ttxidomask[D20_TTSCD].d20_fkt_id =
		    d20_ttxidomask[D20_TTF1].d20_fkt_id = D20_NO_OP;
		    if ((fkt == FKTSCD) || (fkt == FKTSCU) ||
				(fkt == FKTF1) || (fkt == FKTDEL))
			    { return(fkt); }

		    continue;

		  case D2_MHS_ORADR_STX:
			fkt = d20_oradd_disp(object_counter, a_name, 
			  		attr_val->d2_at_value.d2_or_addr_av);

		    	if ((fkt == FKTSCD) || (fkt == FKTSCU) ||
				(fkt == FKTF1) || (fkt == FKTDEL))
			    	return(fkt);

		    	continue;

		  case D2_MHS_ORNAME_STX:
			fkt = d20_orname(object_counter, DS_DISPLAY_OBJECTS,
			      a_name, attr_val->d2_at_value.d2_or_name_av,
			      NULL, NULL);

		    	if ((fkt == FKTSCD) || (fkt == FKTSCU) ||
				(fkt == FKTF1) || (fkt == FKTDEL))
			    	return(fkt);

		    	continue;

		  case D2_MHS_DLSP_STX:
			fkt = d20_dlsp(object_counter, DS_DISPLAY_OBJECTS,
			      a_name, attr_val->d2_at_value.d2_dl_s_p_av,
			      (D2_dl_submit_perm *)NULL,
			      (D2_OR_name *)NULL, (char *)NULL, 
			      (signed16 *)NULL);

		    	if ((fkt == FKTSCD) || (fkt == FKTSCU) ||
				(fkt == FKTF1) || (fkt == FKTDEL))
			    	return(fkt);

		    	continue;

		  case D2_CERTIFICATE:
			fkt = d20_cert(object_counter,
				       DS_DISPLAY_OBJECTS,
				       a_name,
				       attr_val->d2_at_value.d2_cert_a_v,
				       (signed16 *)&(attr_val->d2_a_v_len));
		    	if ((fkt == FKTSCD) || (fkt == FKTSCU) ||
				(fkt == FKTF1) || (fkt == FKTDEL))
				return(fkt);
		    	continue;

		  case D2_CERT_PAIR:
			fkt = d20_certp(object_counter,
				       DS_DISPLAY_OBJECTS,
				       a_name,
				       attr_val->d2_at_value.d2_ctpair_a_v,
				       (signed16 *)&(attr_val->d2_a_v_len));
		    	if ((fkt == FKTSCD) || (fkt == FKTSCU) ||
				(fkt == FKTF1) || (fkt == FKTDEL))
			    	return(fkt);
		    	continue;

		  case D2_CERT_LIST:
			fkt = d20_certl(object_counter,
				       DS_DISPLAY_OBJECTS,
				       a_name,
				       attr_val->d2_at_value.d2_ctlist_a_v,
				       (signed16 *)&(attr_val->d2_a_v_len));
		    	if ((fkt == FKTSCD) || (fkt == FKTSCU) ||
				(fkt == FKTF1) || (fkt == FKTDEL))
			    	return(fkt);
		    	continue;
		  }

		/* handle DNS-Cell attribute mask.		*/
		if (d20_cmp_objid(&(a_type),sizeof(D2_CDS_CELL),
				(Ob_id_string) D2_CDS_CELL))
			{
			/*  enable SCROLL_UP */
			d20_dnscellomask[D20_DCELLSCU].d20_fkt_id = D20_FKTX;
  			if (object_counter != 1)
			    {
			    /*  enable F1 and			*/
			    /*  SCROLL_DOWN in DNS-CELL mask	*/
			    d20_dnscellomask[D20_DCELLSCD].d20_fkt_id =
			    d20_dnscellomask[D20_DCELLF1].d20_fkt_id=D20_FKTX;
			    }

			fkt = d20_dnscell(DS_DISPLAY_OBJECTS, a_name,
					attr_val->d2_at_value.d2_oct_a_v,
					&(attr_val->d2_a_v_len));

			/*  disable SCROLL_UP and F1 and      */
			/*  SCROLL_DOWN in TTX mask           */
			d20_dnscellomask[D20_DCELLSCU].d20_fkt_id =
			d20_dnscellomask[D20_DCELLSCD].d20_fkt_id =
			d20_dnscellomask[D20_DCELLF1].d20_fkt_id = D20_NO_OP;
			if ((fkt == FKTSCD) || (fkt == FKTSCU) || 
				    (fkt == FKTF1) || (fkt == FKTDEL))
				{ return(fkt); }

			continue;
			}

		/* handle DNS-REPLICA attribute mask.		*/
		if (d20_cmp_objid(&(a_type),sizeof(D2_CDS_REPLICA),
				  (Ob_id_string) D2_CDS_REPLICA))
			{
			/*  enable SCROLL_UP */
			d20_dnsrepomask[D20_REPSCU].d20_fkt_id = D20_FKTX;
  			if (object_counter != 1)
			    {
			    /*  enable F1 and				*/
			    /*  SCROLL_DOWN in DNS-REPLICA mask		*/
			    d20_dnsrepomask[D20_REPSCD].d20_fkt_id =
			    d20_dnsrepomask[D20_REPF1].d20_fkt_id = D20_FKTX;
			    }

			fkt = d20_dnsrep(DS_DISPLAY_OBJECTS, a_name,
					attr_val->d2_at_value.d2_oct_a_v,
					&(attr_val->d2_a_v_len));

			/*  disable SCROLL_UP and F1 and      */
			/*  SCROLL_DOWN in TTX mask           */
			d20_dnsrepomask[D20_REPSCU].d20_fkt_id =
			d20_dnsrepomask[D20_REPSCD].d20_fkt_id =
			d20_dnsrepomask[D20_REPF1].d20_fkt_id = D20_NO_OP;
			if ((fkt == FKTSCD) || (fkt == FKTSCU) || 
				    (fkt == FKTF1) || (fkt == FKTDEL))
				{ return(fkt); }

			continue;
			}

		/* handle DME NMO attribute mask.		*/
		if (d20_cmp_objid(&(a_type), sizeof(D2_DME_ALT_ADDR),
				(Ob_id_string) D2_DME_ALT_ADDR))
			{
			/* Display DME Mask now				*/
			/* Enable and disable the funktions keys:	*/
			/* in this function.				*/
			fkt = d20_dme_disp(object_counter,
					   DS_DISPLAY_OBJECTS,
					   a_name,
					   &(attr_val->d2_at_value.d2_oct_a_v),
					   (signed16 *)&(attr_val->d2_a_v_len));

			if ((fkt == FKTSCD) || (fkt == FKTSCU) || 
			    (fkt == FKTF1) || (fkt == FKTDEL) )
				{ return(fkt); }

			continue;
			}
		}
	}

/*  attribute is ACL                  */
#ifndef CACHE_ADMIN
if (acl_exist == TRUE)
	{
	/*  enable SCROLL_UP */
	d20_6bomask[D20_6A3POS].d20_fkt_id = D20_FKTX;
  	if (object_counter != 1)
		{
		/*  enable F1 and			*/
		/*  SCROLL_DOWN in ACL mask		*/
    		d20_6bomask[D20_6A1POS].d20_fkt_id =
    		d20_6bomask[D20_6A4POS].d20_fkt_id = D20_FKTX;
		}

	/*  display ACL                       */

  	if (ds0_507_handle_acl(DS_DISPLAY_OBJECTS,
				TRUE,
				acl_len,
				(char *)acl_val))
		if ((*d20_6bifields[DS_DN16_SCU] != DS_DEFAULT_VALUE) ||
		    (*d20_6bifields[DS_DN17_F1] != DS_DEFAULT_VALUE) ||
		    (*d20_6bifields[DS_DN13_BREAK] != DS_DEFAULT_VALUE) ||
		    (*d20_6bifields[DS_DN14_MORE] != DS_DEFAULT_VALUE))
			{
			if (*d20_6bifields[DS_DN16_SCU] != DS_DEFAULT_VALUE)
				return(FKTSCU);
		    	if (*d20_6bifields[DS_DN17_F1] != DS_DEFAULT_VALUE)
				return(FKTF1);
		    	if (*d20_6bifields[DS_DN13_BREAK] != DS_DEFAULT_VALUE)
				return(FKTDEL);
		    	if (*d20_6bifields[DS_DN14_MORE] != DS_DEFAULT_VALUE)
				return(FKTSCD); 
			}
		else
			{ return(FKTDEL); }

	}
	/*  disable SCROLL_UP and F1 and      */
	/*  SCROLL_DOWN in ACL mask           */
	d20_6bomask[D20_6A1POS].d20_fkt_id =
	d20_6bomask[D20_6A3POS].d20_fkt_id =
	d20_6bomask[D20_6A4POS].d20_fkt_id = D20_NO_OP;
#endif


return(fkt);

}                               /*  d20_show_attr                     */


/*inon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_disp_val()                                        */
/*								      */
/* AUTHOR:      Schmid (D AP 11)                                      */
/* DATE:        16.05.88                                              */
/*								      */
/* SYNTAX:      void d20_disp_val(len, val, at_pos, syntax)           */
/*              signed16         len;       length of attribute       */
/*              Octet_string     val;       pointer to attribute      */
/*                                          value                     */
/*              signed16         at_pos;    attribute position in the */
/*					    mask. 		      */
/*		signed16	 syntax;    attribute value syntax.   */
/*								      */
/* DESCRIPTION: This function copies the attribute value into the     */
/*              mask output fields.                                   */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              signed16         len;       length of attribute       */
/*              Octet_string     val;       pointer to attribute      */
/*                                          value                     */
/*              signed16         at_pos;    attribute position in the */
/*					    mask. 		      */
/*		signed16	 syntax;    attribute value syntax.   */
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
/*inoff ***************************************************************/

static  void d20_disp_val(signed16 len, Octet_string val, signed16 at_pos,
			  signed16 syntax)
{                               /*  d20_disp_val                      */

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

 signed16        pos;           /* Property position at the           */
				/* beginning of the field in the mask */

 char         prval[2 * D20_ATVAL_LEN * D20_T61_CONV_FACT];
 Octet_string str;
 signed16     max_pos = D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT;
 signed16     prlen;
 Bool		convert;
/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

                                /*  copy property into mask fields    */
switch (d27_401_syntax_to_rep (syntax))
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
			(Octet_string)d20_7omask[at_pos + 3].d20_ioinfo,
			(Octet_string) HEX,2);
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
                                
d20_7omask[at_pos + 3].d20_conv = d20_7omask[at_pos + 4].d20_conv = convert;


if (prlen > max_pos - pos - 1)
	{   
	ds4_004_octncpy((Octet_string)(d20_7omask[at_pos + 3].d20_ioinfo + pos),
			 str, max_pos - pos);
	prlen -= (max_pos - pos);
	ds4_004_octncpy((Octet_string) d20_7omask[at_pos + 4].d20_ioinfo,
			str + max_pos - pos, prlen);
	*(d20_7omask[at_pos + 4].d20_ioinfo + prlen) = '\'';
	*(d20_7omask[at_pos + 4].d20_ioinfo + prlen + 1) = D2_EOS;
	}
else
	{
	ds4_004_octncpy((Octet_string)(d20_7omask[at_pos + 3].d20_ioinfo + pos),
			 str, prlen);
	*(d20_7omask[at_pos + 3].d20_ioinfo + pos + prlen) = '\'';
	*(d20_7omask[at_pos + 3].d20_ioinfo + pos + prlen + 1) = D2_EOS;
	*d20_7omask[at_pos + 4].d20_ioinfo = D2_EOS;
	} 

}                               /*  d20_disp_val                      */


/*inon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_attr()                                            */
/*								      */
/* AUTHOR:      Schmid (D AP 11)                                      */
/* DATE:        16.05.88                                              */
/*								      */
/* SYNTAX:      signed16 d20_attr(a_type, a_name, a_syntax)           */
/*              D2_a_type           *a_type;                          */
/*              char               **a_name;                          */
/*              signed16            *a_syntax;                        */
/*								      */
/* DESCRIPTION: This function looks for the attribute name of the     */
/*              attribute (type).                                     */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              D2_a_type            *a_type;  pointer to attribute   */
/*					       type 		      */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*              char               **a_name;    pointer to attribute  */
/*                                              name                  */
/*              signed16            *a_syntax;  attribute syntax      */
/*								      */
/* RETURNVALUE:                                                       */
/*              D2_ERROR:   attribute not found                       */
/*              D2_NOERROR: attribute found                           */
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
/*inoff ***************************************************************/

static  signed16 d20_attr(
	D2_a_type *a_type,
	char      **a_name,
	signed16   *a_syntax)
{                               /*  d20_attr                          */

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

 D20_at   *max_ptr = d2_p_d20at + d2_nd20at;

 D20_at	*run_at_ptr;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

for                             /*  all attributes */
    (run_at_ptr = d2_p_d20at; run_at_ptr < max_ptr; run_at_ptr++)
{                               
  if                            /*  attribute type found              */
     (d20_cmp_objid(&(run_at_ptr->obj_id),a_type->d2_typ_len+1,a_type->d2_type))
  {                             /*  set pointer to attribute name     */
				/*  set attribute syntax              */
    *a_name = run_at_ptr->at.att_name;
    *a_syntax = run_at_ptr->at.att_syntax;
				/*  return(D2_NOERROR)                */
    return(D2_NOERROR);
  }                             
}                               

				/*  return(D2_ERROR)                  */
return(D2_ERROR);

}                               /*  d20_attr                          */



/**********************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_600_display_objects                               */
/*								      */
/* AUTHOR:      Eros, D AP 11                                         */
/* DATE:        13.11.89                                              */
/*								      */
/* SYNTAX:      void  d20_600_display_objects(tree)        	      */
/*								      */
/*              String            tree;                               */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              Multiple item display with selection list.            */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              String            tree;         indicates structure   */
/*                                              of entries            */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              None                                                  */
/*								      */
/* RETURNVALUE:							      */
/*              None                                                  */
/*								      */
/**********************************************************************/

static  void d20_600_display_objects(String tree)
{
   Bool m18_on = TRUE; /* initialize to mask 18 display (summary list) */
   signed32 key_pressed;      /* which function key did the user press */
   signed32 run_mode;         /* null, one, or plus one objects handled */
   signed32 i;

for (i = 0 ;i < D20_13_NO_FIELDS; i++)
{ 
  d20_13omask[DS_13_FIRST - 1 + 2*i].d20_fkt_id =
  d20_13omask[DS_13_FIRST + 2*i].d20_fkt_id = D20_NO_OP;
  d20_13omask[DS_13_FIRST - 1 + 2*i].d20_ioinfo = NULL;
  *d20_13omask[DS_13_FIRST + 2*i].d20_ioinfo = D2_EOS;
} 

*(signed32 *)d20_12omask[DS_12_USE].d20_ioinfo = D20_12_OBJ_LIST;

/*  erase screen first time around */
d20_12omask[0].d20_fkt_id &= ~D20_NOERASE;
first_list = summary.summary_list;
cursor_stay = FALSE;
display_all = FALSE;
prep_disp_data(tree);
if (object_counter > 1) 
	run_mode = 2;
else
	run_mode = object_counter;

getwerg = 0;

switch (run_mode)
{   case 1: /* one object to process */
	    d20_13omask[DS_13_F1].d20_fkt_id =
	    d20_13omask[DS_13_DOWN].d20_fkt_id =
	    d20_13omask[DS_13_UP].d20_fkt_id = D20_NO_OP;
	    summary.num_selected = object_counter;
	    first_list->selected = TRUE;
	    m18_on = FALSE;
	    key_pressed = FKTF1;
	    break;
    case 2: /* more than one object to process */
	    strcpy(d20_12barmenu[0], first_list->summary_name);
	    key_pressed = display_mask_18();
	    if (key_pressed == FKTCR)
		cursor_stay = TRUE;
	    break;
}
	
/*  initialize summary mask to no erase mode */
d20_12omask[0].d20_fkt_id |= D20_NOERASE;

display_objects(key_pressed, tree, m18_on);

/* activate F1, SCROLL_UP, SCROLL_DOWN again */
d20_13omask[DS_13_F1].d20_fkt_id = d20_13omask[DS_13_DOWN].d20_fkt_id =
				 d20_13omask[DS_13_UP].d20_fkt_id = D20_FKTX;
}

/**********************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_610_display_errobj                                */
/*								      */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        22.12.89                                              */
/*								      */
/* SYNTAX:      static  void  d20_610_display_errobj(old_objects)     */
/*								      */
/*              D2_name_string    *old_objects;                       */
/*                                                                    */
/* DESCRIPTION: The Function displays the erroneous objects           */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              old_objects       Name_string * pointer to list of    */
/*                                              erroneous names       */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              None                                                  */
/*								      */
/* RETURNVALUE:							      */
/*              None                                                  */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*              object_counter  signed32    count of objects in       */
/*                                          error-list                */
/*              first_list      struct summary_item *   first element */
/*                                          of error-list             */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*              d20_14omask     D20_omask   mask to display           */
/*              summary         struct summary_list_type    Array of  */
/*                                      objects to display (converted)*/
/*								      */
/* CALLED FUNCTIONS/MACROS:                                           */
/*              qsort()                                               */
/*              d20_611_convert_dn()                                  */
/*              d20_612_display_mask_20()                             */
/*              d20_614_handle_key_20()                               */
/*              summarycmp()                                          */
/*								      */
/**********************************************************************/

static  void  d20_610_display_errobj(D2_name_string *old_objects)
{                           /*  d20_610_display_errobj                */

   signed32 key_pressed;      /* which function key did the user press */
   signed32 i;

/*  erase screen first time around */
d20_14omask[0].d20_fkt_id &= ~D20_NOERASE;

for                     /*  all objects in list                       */
   (i = 0; i < object_counter; i++, summary.summary_list++)
{                       /*  convert into screen-format                */
  d20_611_convert_dn((char *)old_objects[i],
		      summary.summary_list->summary_name);
  summary.summary_list->selected = FALSE;
}                       

qsort((char *)first_list, (size_t)object_counter,
      (size_t)sizeof(struct summary_item),
      (int (*)(const void *, const void *))summarycmp);

key_pressed = d20_612_display_mask_20();
	
/*  initialize summary mask to no erase mode    */
d20_14omask[0].d20_fkt_id |= D20_NOERASE;

d20_614_handle_key_20(key_pressed);

}                           /*  d20_610_display_errobj                */

/***********************************************************************/
static  void display_objects(
	signed32  key_pressed,
	String    tree,
	Bool      m18_on)
{   
struct summary_item *q;
int i, fkt;

for (;;)
{   switch (key_pressed)
    {   case FKTCR: /* carriage return was pressed */
		    if (m18_on)
		    {
		       cursor_stay = TRUE;
		       handle_barmenu();
		       key_pressed = display_mask_18();
		    }
		    else
		    {
		       get_selected_object(SAME);
		       key_pressed = display_mask_6(tree);
		    }
		    break;
	case FKTDEL: /* delete key was pressed */
		    return;
	case FKTF1:  /* F1 key was pressed */
		    summary.current_display = 0;
		    if (display_all) /* deselect all objects */
		    {   for (q = first_list, i = 0;
			     i < object_counter;q++,i++)
			    q->selected = FALSE;
			summary.num_selected = 0;
			display_all = FALSE;
		    }
		    if ( (m18_on) || (object_counter == 1) )
		    {   if (summary.num_selected > 0)
			{   get_selected_object(SAME);
			    if (object_counter > 1)
				m18_on = FALSE;
			    key_pressed = display_mask_6(tree);
			}
			else
			{   /* F1 pressed without any
			       objects selected: display
			       all objects */
			    display_all = TRUE;
			    summary.num_selected = object_counter;
			    /* select all objects */
			    for (q = first_list, i = 0;
				 i < object_counter;q++,i++)
				q->selected = TRUE;
			    m18_on = FALSE;
			    get_selected_object(SAME);
			    key_pressed = display_mask_6(tree);
			}
		    }
		    else
		    {   d20_12omask[0].d20_fkt_id &= ~D20_NOERASE;
			m18_on = TRUE;
			key_pressed = display_mask_18();
			d20_12omask[0].d20_fkt_id |= D20_NOERASE;
		    }
		    cursor_stay = TRUE;
		    break;
	case FKTMENU: /* menu key was pressed */
		    if (m18_on) /* same as F1 in mask 18 */
		    {   if (summary.num_selected > 0)
			{   m18_on = FALSE;
			    get_selected_object(SAME);
			    key_pressed = display_mask_6(tree);
			}
			else
			{   /* menu pressed without any objects selected: */
			    /* display all objects                        */
			    display_all = TRUE;
			    summary.num_selected = object_counter;
			    /* select all objects */
			    for (q = first_list, i = 0; i < object_counter;
				 q++,i++)
				q->selected = TRUE;
			    m18_on = FALSE;
			    get_selected_object(SAME);
			    key_pressed = display_mask_6(tree);
			}
		    }
		    else
		    {   /* here display attributes mask */
			d20_entptr = summary.summary_list->ei_info;
			fkt = d20_show_attr(D20_NEXT);
			if (fkt == FKTDEL)
			{    return;
			}
			else /* scroll up */
			   if (fkt == FKTSCU)
			    {   get_selected_object(SAME);
				key_pressed = display_mask_6(tree);
			    }
			   else
				if (fkt == FKTF1)
				{   m18_on = TRUE;
				    if (display_all)
						 /* deselect all objects */
				    {   for (q = first_list, i = 0;
					     i < object_counter;q++,i++)
					    q->selected = FALSE;
					summary.num_selected = 0;
					display_all = FALSE;
				    }
				   d20_12omask[0].d20_fkt_id &= ~D20_NOERASE;
				    key_pressed = display_mask_18();
				    d20_12omask[0].d20_fkt_id |= D20_NOERASE;
				}
				else /* fat down arrow or menu */
				{   get_selected_object(NEXT);
				    key_pressed = display_mask_6(tree);
				}
		    }
		    cursor_stay = TRUE;
		    break;
	case FKTPRINT: /* print key was pressed */
		    if (m18_on)
		    {   print_summary_list();
			key_pressed = display_mask_18();
		    }
		    else
		    {   get_selected_object(SAME);
			key_pressed = display_mask_6(tree);
		    }
		    break;
	case FKTSCD: /* fat down arrow was pressed (scroll down) */
		    if (m18_on)
		    {
			cursor_stay = FALSE;
			scroll(FKTSCD);
			key_pressed = display_mask_18();
			cursor_stay = TRUE;
		    }
		    else
		    {   d20_12omask[0].d20_fkt_id &= ~D20_NOERASE;
			get_selected_object(NEXT);
			key_pressed = display_mask_6(tree);
		    }
		    d20_12omask[0].d20_fkt_id |= D20_NOERASE; /* no erase */
		    break;
	case FKTSCU:  /* fat up arrow was pressed (scroll up) */
		    if (m18_on)
		    {
			cursor_stay = FALSE;
			scroll(FKTSCU);
			key_pressed = display_mask_18();
			cursor_stay = TRUE;
		    }
		    else
		    {   d20_12omask[0].d20_fkt_id &= ~D20_NOERASE;
			get_selected_object(PREVIOUS);
			key_pressed = display_mask_6(tree);
		    }
		    d20_12omask[0].d20_fkt_id |= D20_NOERASE; /* no erase */
		    break;
    }
}
} 

/**********************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : prep_disp_data                                        */
/*								      */
/* AUTHOR:      Eros, D AP 11                                         */
/* DATE:        25.11.89                                              */
/*								      */
/* SYNTAX:      void  prep_disp_data (tree)   	      		      */
/*								      */
/*              String            tree;                               */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              Fill global summary list and mask 19 object type names*/
/*	        (i.e. "country" ," organization" etc.)		      */
/* INPUT-PARAMETERS:                                                  */
/*              String            tree;         indicates structure   */
/*                                              of entries            */
/*                                                                    */
/* GLOBAL  DATA  (read-only):                                         */
/*              object_counter  int         count of objects in       */
/*                                          error-list                */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*              d20_13omask     D20_omask   mask to display           */
/*              summary         struct summary_list_type    Array of  */
/*                                      objects to display (converted)*/
/*								      */
/* CALLED FUNCTIONS/MACROS:                                           */
/*              qsort()                                               */
/*              summarycmp()                                          */
/*                                                                    */
/**********************************************************************/

static  void prep_disp_data(String tree)
{
   signed16 i;
   Ds_obj_entry   obj_entry[D20_5A1CNT];
   char         *val;

   signed16 mode = D20_BEGIN;

/* load "Country" "Organization" etc. into mask 19 */
ds0_512_object_tree(&d20_13omask[DS_13_FIRST - 1],tree,obj_entry,&i,D20_MASK13);

/* load summary list */
while (!d20_dnget(mode,(D2_name_string *) &val))
     {
	if (d20_cmp_structure(val, tree) == TRUE)
    		fill_disp_lists(val);

    	mode = D20_CONTINUE;
    }
summary.summary_list++; /* point to the last item */

qsort((char *)first_list,(size_t)object_counter,
      (size_t)sizeof(struct summary_item),
      (int (*)(const void *, const void *))summarycmp);

}

/***********************************************************************/
/* load summary list with DNs separated by slashes for use in mask 18 */

static  void fill_disp_lists(char *name)
{
/* fill attribute information element of summary list for use by show_attr */
summary.summary_list->ei_info = d20_entptr;

d20_611_convert_dn(name, summary.summary_list->summary_name);

summary.summary_list->selected = FALSE;
summary.summary_list++;
} 

/**********************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_611_convert_dn                                    */
/*								      */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        22.12.89                                              */
/*								      */
/* SYNTAX:      LOCAL void  d20_611_convert_dn(dn,field)              */
/*								      */
/*              D2_name_string    dn;                                 */
/*              String            field;                              */
/*                                                                    */
/* DESCRIPTION: The Function converts distinguished name dn to field. */
/*              The attribute types are removed, the delimiters are   */
/*              replaced, the nameparts are reversed.                 */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              dn            D2_Name_string *  pointer to DN         */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              field         String            field where converted */
/*                                              must be written to.   */
/*								      */
/* RETURNVALUE:							      */
/*              None                                                  */
/*								      */
/**********************************************************************/

static void d20_611_convert_dn(char *name, char *field)
{
register signed16 i, no_np, max;
char *npptr[D2_NP_MAX];
char tmp_buf[D2_DNL_MAX];
char tmp_buf2[D2_DNL_MAX];
char	*dest_ptr, *src_ptr;


strcpy(tmp_buf, name);

/* sort naming attributes in an RDN in ascending order of their obj_ids. */
d27_018_sort_name((D2_name_string)tmp_buf);

no_np = 0; 
npptr[no_np++] = dest_ptr = &tmp_buf2[0];

for (i = 0, max = strlen(tmp_buf); i < max; i++) 
	{
  	if (tmp_buf[i] == D2_T61_REP || tmp_buf[i] == D2_PRTBL_REP ||
  		tmp_buf[i] == D2_NUM_REP || tmp_buf[i] == D2_IA5_REP)
		{
		/* get naming attribute abbrv. from schema and copy to target */
		/* string.						      */
	     	src_ptr = &tmp_buf[i + 1];
		continue;
		}

	if (tmp_buf[i] == D2_AVA_SEP)
		{
		/* copy naming attribute value to target string.	*/
		tmp_buf[i] = D2_EOS;
		strcpy(dest_ptr, src_ptr);
		dest_ptr += strlen(dest_ptr);
		*dest_ptr++ = D2_ADM_AVA_SEP;
		continue;
		}
		
	if (tmp_buf[i] == D2_RDN_SEP)
		{
		/* copy naming attribute value to target string.	*/
		tmp_buf[i] = D2_EOS;
		strcpy(dest_ptr, src_ptr);
		dest_ptr += strlen(dest_ptr) + 1;
		npptr[no_np++] = dest_ptr;
		continue;
		}
	}

strcpy(dest_ptr, src_ptr);

/* npptr[0] contains "de", npptr[1] contains "sanjay,ap11" etc. */
/* xfer RDN-values to output fields separated by '/' */

tmp_buf[0] = D2_EOS;

for (i = no_np-1; i >= 0; i--)
	{   
	strcat(tmp_buf,npptr[i]);

    	if (i > 0)
		strcat(tmp_buf,"/");
	}

if ((int)strlen(tmp_buf) <= MAX_T61_DISP_STR_LEN)
	strcpy(field, tmp_buf);
else
	{       
	strncpy(field, tmp_buf, MAX_T61_DISP_STR_LEN);
	field[MAX_T61_DISP_STR_LEN] = D2_EOS;
	}
}                     /*  d20_611_convert_dn                        */

/***********************************************************************/
static  signed32 display_mask_18() /* display summary list */
{
signed32 ret;

set_mask_data_18();   /* prepare mask for display */
ret = d20_handlemask(d20_12omask); /* display mask */

return(ret);
}

/***********************************************************************/
/* display an item selected from summary list */
static  signed32 display_mask_6(char *tree) 
{
signed32 ret;

set_mask_data_6(tree); /* prepare mask for display */ 
if ((ret = d20_handlemask(d20_13omask)) == FKTCR)
	ret = FKTMENU;

return(ret);
}

/**********************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_612_display_mask_20                               */
/*								      */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        22.12.89                                              */
/*								      */
/* SYNTAX:      static  signed32 d20_612_display_mask_20()            */
/*								      */
/* DESCRIPTION: The Function displays the mask 20                     */
/*								      */
/* INPUT-PARAMETERS: None                                             */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              None                                                  */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*              d20_14omask     D20_omask   mask to display           */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*              d20_14omask     D20_omask   mask to display           */
/*								      */
/* CALLED FUNCTIONS/MACROS:                                           */
/*              d20_613_set_mask_data_20()                            */
/*              d20_handlemask()                                      */
/*								      */
/* RETURNVALUE:							      */
/*              pressed key                                           */
/*								      */
/**********************************************************************/

static	signed32 d20_612_display_mask_20()
{                       /*  d20_612_display_mask_20                   */

static   signed32 qey14[] = { FKTDEL, FKTPRINT, FKTSCD, FKTSCU };

register signed32 i;

d20_613_set_mask_data_20();   /*  prepare mask for display            */
d20_handlemask(d20_14omask);  /*  display mask                */


for                           /*  all function-key-fields             */
   (i = DS_14_FIRST + DS_14_NO_FIELDS; i <= DS_14_LAST; i++)
{                             /*  check whether assigned key pressed  */
  if                          /*  field is marked                     */
    (*(d20_14omask[i].d20_ioinfo) == D20_MARKFKT)
  {                           /*  reset key-field                     */
    *(d20_14omask[i].d20_ioinfo) = DS_DEFAULT_VALUE;
    return(qey14[i - DS_14_FIRST - DS_14_NO_FIELDS]);
  }                           
}                             

return(0);

}                       /*  d20_612_display_mask_20                   */

/***************************************************************************/
static  void set_mask_data_6(char *tree)
{
/* 1. based on summary.current_display , fill mask 6 with object data */
signed32 i,j;
struct summary_item *p;
char name_buf[D2_DNL_MAX];

d20_12omask[0].d20_fkt_id &= ~D20_NOERASE; /* erase screen */
p = first_list; /* point to first summary list item */
for (i=0,j=0; i<object_counter; i++,p++) /* find current object to display*/
{   if (p->selected == TRUE)
    {   j++;
	if (j == summary.current_display)
	{   /* save dn_name as ds4_b03 modifies it,and...*/
	    strcpy(name_buf, (char *)(p->ei_info->d2_ei_entry));

	     /* sort naming attributes in an RDN in ascending order of their */
	     /* obj_ids.						     */
	     d27_018_sort_name((D2_name_string)name_buf);

	    /* ... load mask with object instance data */
	    ds4_b03_fill_obj_fields((D2_name_string) name_buf,d20_13ofields);
	    break;
	}
    }
}
}

/***********************************************************************/
static  void set_mask_data_18()
{
signed32 i,first_line;
struct summary_item *p;
struct summary_item *temp;
D20_omask   *p_mask;

p = first_list; /* point to first summary list item */
/* find first item to be displayed on current page */
first_line = (summary.current_page - 1) * SUMMARY_LINES_PER_PAGE + 1;
/* if the object with that number does not exist, get out */
if (first_line > object_counter)
	return;
/* fetch the first item to be displayed */
p += (first_line - 1);
temp = p;

/* initialize barmenu to first item on top */
if (!cursor_stay)
    strcpy(d20_12barmenu[0],p->summary_name);

/* copy first_line object thru either (the last) or (16th) object to the
screen fields as appropriate */
i = 0;
p = temp;
p_mask = &d20_12omask[DS_12_FIRST];
while ( (first_line <= object_counter) && (i < SUMMARY_LINES_PER_PAGE) )
{   p_mask->d20_ioinfo = p->summary_name;
    /* selected objects are displayed inverse */
    if (p->selected == TRUE)
	p_mask->d20_attribute |= INVERS;
    else
	p_mask->d20_attribute &= ~INVERS;
    p_mask->d20_fkt_id = D20_CSAY;
    i++; p++; first_line++;
    p_mask++;
}

/* set remainig fields to D20_NO_OP instead of D20_CSAY */
/* so handlemask handles the thin arrows correctly      */
for ( ; i < SUMMARY_LINES_PER_PAGE; i++, p_mask++)
    p_mask->d20_fkt_id = D20_NO_OP;
}

/**********************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_613_set_mask_data_20                              */
/*								      */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        02.01.90                                              */
/*								      */
/* SYNTAX:      static  void d20_613_set_mask_data_20()               */
/*								      */
/*                                                                    */
/* DESCRIPTION: The Function sets the data to be displayed in mask 20 */
/*								      */
/* INPUT-PARAMETERS: None                                             */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              None                                                  */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*              summary         struct summary_list_type    Array of  */
/*                                      objects to display (converted)*/
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*              d20_14omask     D20_omask   mask to display           */
/*								      */
/* RETURNVALUE:							      */
/*              pressed key                                           */
/*								      */
/**********************************************************************/

static  void d20_613_set_mask_data_20()
{                       /*  d20_613_set_mask_data_20                  */
signed32 i, first_line;
struct summary_item *p;

p = first_list;                   /* point to first summary list item */
/* find first item to be displayed on current page */
first_line = (summary.current_page - 1) * SUMMARY_LINES_PER_PAGE + 1;
/* if the object with that number does not exist, get out */
if (first_line > object_counter)
	return;
/* fetch the first item to be displayed */
p += (first_line - 1);

for                         /*  all lines in page                     */
   (i = 0; first_line <= object_counter && i < SUMMARY_LINES_PER_PAGE;
    i++, p++, first_line++)
{                           /*  copy name into mask                   */
  d20_14omask[DS_14_FIRST + i].d20_ioinfo = p->summary_name;
  d20_14omask[DS_14_FIRST + i].d20_fkt_id = D20_CSAY;
}                           

/* set remainig fields to D20_NO_OP instead of D20_CSAY */
/* so handlemask handles the thin arrows correctly      */
for ( ; i < SUMMARY_LINES_PER_PAGE; i++)
    d20_14omask[i + DS_14_FIRST].d20_fkt_id = D20_NO_OP;

}                       /*  d20_613_set_mask_data_20                  */

/**********************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_614_handle_key_20                                 */
/*								      */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        02.01.90                                              */
/*								      */
/* SYNTAX:      static  void  d20_614_handle_key_20(key_pressed)      */
/*								      */
/*              signed32               key_pressed;                   */
/*                                                                    */
/* DESCRIPTION: The Function handles the result key pressed by the    */
/*              user in mask 20.                                      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              key_pressed     signed32 key pressed by the user      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              None                                                  */
/*								      */
/* RETURNVALUE:							      */
/*              None                                                  */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*              d20_14omask     D20_omask   mask to display           */
/*								      */
/* CALLED FUNCTIONS/MACROS:                                           */
/*              print_summary_list()                                  */
/*              scroll()                                              */
/*              d20_612_display_mask_20()                             */
/*								      */
/**********************************************************************/

static  void  d20_614_handle_key_20(signed32 key_pressed)
{                           /*  d20_614_handle_key_20                 */
for (;;)
{   switch (key_pressed)
    {   case FKTDEL: /* delete key was pressed */
			return;
	case FKTPRINT: /* print key was pressed */
			print_summary_list();
			key_pressed = d20_612_display_mask_20();
			break;
	case FKTSCD: /* fat down arrow was pressed (scroll down) */
			scroll(FKTSCD);
			d20_14omask[0].d20_fkt_id &= ~D20_NOERASE;
			key_pressed = d20_612_display_mask_20();
			d20_14omask[0].d20_fkt_id |= D20_NOERASE;
			break;
	case FKTSCU:  /* fat up arrow was pressed (scroll up) */
			scroll(FKTSCU);
			d20_14omask[0].d20_fkt_id &= ~D20_NOERASE;
			key_pressed = d20_612_display_mask_20();
			d20_14omask[0].d20_fkt_id |= D20_NOERASE;
			break;
    }
}

}                           /*  d20_614_handle_key_20                 */

/*************************************************************************/
static  void scroll(signed32 scroll_mode)
/*  scroll page up/page down in summary list mask */
{
signed32 curr_pag; 

curr_pag = summary.current_page;
if (scroll_mode == FKTSCU) /* scroll up */
    summary.current_page--;
else                       /* scroll down */
    summary.current_page++;

if (summary.current_page < 1) 
{   d20_msgomask[D20_M1POS].d20_ioinfo = d20_err_table[D20_E52IDX];
    d20_handlemask(d20_msgomask); /* top of list reached */
    cursor_stay = TRUE;
    summary.current_page++;
}
else
 	getwerg = 0;

if (summary.current_page > summary.max_page) 
{   d20_msgomask[D20_M1POS].d20_ioinfo = d20_errtab[D20_E20IDX];
    d20_handlemask(d20_msgomask); /* end of list reached */
    cursor_stay = TRUE;
    summary.current_page--;
}
else
 	getwerg = 0;

/* set mode to "erase" because new mask is to be displayed or error mask */
/* has to be erased                                                      */
d20_12omask[0].d20_fkt_id &= ~D20_NOERASE; /* erase screen */
}

/************************************************************************/
static  void print_summary_list()
{
signed32 i;
FILE *outfil;
struct summary_item *p;

if ((outfil = fopen("summary.lst", "w")) != (FILE *) 0)
{   p = first_list;
    for (i=1; i<= object_counter; i++,p++)
	fprintf(outfil, "%s\n" , p->summary_name);
    fclose(outfil);
    system("lpr summary.lst");
}
}

/************************************************************************/
static  signed32 get_selected_object(signed32 mode) 
/* fetch item to be displayed in mask 19 */
{
signed32 i,j;
struct summary_item *p;

/* depending on mode, set current_display */
if (!summary.current_display)
    summary.current_display = 1;

if (mode == PREVIOUS)
    if (summary.current_display > 1)
    {   summary.summary_list--;
	summary.current_display--;
    }
    else
    {   d20_msgomask[D20_M1POS].d20_ioinfo =d20_err_table[D20_E52IDX];
	d20_handlemask(d20_msgomask); /* top of list reached */
    }

if (mode == NEXT)     
{   summary.current_display++;
    summary.summary_list++;
}

if (summary.current_display > summary.num_selected)
{   d20_msgomask[D20_M1POS].d20_ioinfo = d20_errtab[D20_E20IDX];
    d20_handlemask(d20_msgomask); /* end of list reached */
    summary.current_display--;
}

p = first_list;
for (i=0,j=0; i < object_counter; i++,p++)
{   if ( (p->selected == TRUE) || (object_counter == 1) )
    {   j++;
	if (j == summary.current_display || summary.current_display == 0)
	{   summary.summary_list = p;
	    break;
	}
    }
}
}

/************************************************************************/
static  void handle_barmenu()
/* go thru the summary list and select/deselect any objects over which the
bar menu was placed  and RETURN pressed */
{
signed32 first_line;
struct summary_item *p;

p = first_list;
first_line = (summary.current_page - 1) * SUMMARY_LINES_PER_PAGE + 1;
p += (first_line - 1);  /* set pointer to first item on the page */

p += getwerg;           /* set pointer to actually selected item */
if (p->selected)
{   p->selected=FALSE;
    summary.num_selected--; /* deselect object */
	/* reset current displayable object to zero*/
    summary.current_display = 0;
}
else
{   p->selected=TRUE; /* select object */
    summary.num_selected++;
}
}

/******************************************************************************/
/*                                                                            */
/*  NAME:  summarycmp()                                                       */
/*                                                                            */
/*  DESCRIPTION:                                                              */
/*      The function compares two summary items                               */
/*                                                                            */
/*  INPUT PARAMETER:                                                          */
/*      s_i1,s_i2  =  two summary items which should be compared              */
/*                                                                            */
/******************************************************************************/
 
static  signed32 summarycmp(
	struct summary_item *s_i1,
	struct summary_item *s_i2)
{
    return(dnrevcmp(s_i1->summary_name,s_i2->summary_name));
}

/******************************************************************************/
/*                                                                            */
/*  NAME:  dnrevcmp()                                                         */
/*                                                                            */
/*  DESCRIPTION:                                                              */
/*      The function compares two reversed DNs                                */
/*                                                                            */
/*  INPUT PARAMETER:                                                          */
/*      str1,str2  =  two reversed DNs  which should be compared              */
/*                                                                            */
/******************************************************************************/
 
static signed32 dnrevcmp(String str1, String str2)
{
    int     ret;
    char    *pos1,*pos2;

    if (strcmp(str1, str2) == 0)
	return(0);
    pos1 = strchr(str1,'/');
    pos2 = strchr(str2,'/');

    if (pos1 != NULL) *pos1 = D2_EOS;
    if (pos2 != NULL) *pos2 = D2_EOS;

    if ((ret = strcmp(str1, str2)) > 0) {
	if (pos1 != NULL) *pos1 = '/';
	if (pos2 != NULL) *pos2 = '/';
	return(1);
    }
    else if (ret < 0) {
	if (pos1 != NULL) *pos1 = '/';
	if (pos2 != NULL) *pos2 = '/';
	return(-1);
    }
    else {
	if (pos1 != NULL) *pos1 = '/';
	if (pos2 != NULL) *pos2 = '/';
	return(dnrevcmp(pos1+1, pos2+1));
    }
}

/****************************************************************************/
/*                                                                          */
/*  NAME:  display_oclass_sch()					    	    */
/*  AUTHOR:	Sanjay Jain						    */
/*  DATE:	01.04.92						    */
/*                                                                          */
/*  DESCRIPTION:                                                            */
/*	This function handles displaying auxiliary object class list	    */
/*	and schema tables (AT/OCT/SRT).					    */
/*                                                                          */
/*  INPUT PARAMETER:                                                        */
/*									    */
/*  OUTPUT PARAMETER:							    */
/*									    */
/*  RETURN VALUE:							    */
/*	FKTMENU or FKTDEL.						    */
/*									    */
/*                                                                          */
/****************************************************************************/
 
static 	signed32	display_oclass_sch()
{

signed32	key_pressed;
signed32	i;

cursor_stay = FALSE;

/* erase screen first time.	*/
d20_12omask[0].d20_fkt_id &= ~D20_NOERASE;

getwerg = 0;

for (;;)
	{
	key_pressed = display_mask_18();

	switch (key_pressed)
		{
    		case FKTCR: /* carriage return was pressed */
		       	handle_barmenu();
			cursor_stay = TRUE;
			d20_12omask[0].d20_fkt_id |= D20_NOERASE; /* no erase */
		    	break;

		case FKTDEL: /* delete key was pressed */
		    	break;

		case FKTMENU: /* menu key was pressed */
			break;

		case FKTSCD: /* fat down arrow was pressed (scroll down) */
			cursor_stay = FALSE;
			scroll(FKTSCD);
		    	break;

		case FKTSCU:  /* fat up arrow was pressed (scroll up) */
			cursor_stay = FALSE;
			scroll(FKTSCU);
		    	break;

		case FKTF1:
			break;
		}

	if (key_pressed == FKTDEL || key_pressed == FKTMENU || 
	    key_pressed == FKTF1)
		break;

    	}

/* if nothing is selected, then select all.	*/
for (i=0; i<object_counter; i++)
	if ((first_list + i)->selected == TRUE)
		break;

/* nothing is selected.	*/
if (i == object_counter)
	for (i=0; i<object_counter; i++)
		(first_list + i)->selected = TRUE;


return(key_pressed);

}

/****************************************************************************/
/*                                                                          */
/*  NAME:  d20_call_disp_aux()						    */
/*  AUTHOR:	Sanjay Jain						    */
/*  DATE:	01.04.92						    */
/*                                                                          */
/*  DESCRIPTION:                                                            */
/*	This function handles displaying auxiliary object class list.	    */
/*                                                                          */
/*  INPUT PARAMETER:                                                        */
/*	oct_ent		containing list of valid auxiliary object classes.  */
/*									    */
/*  OUTPUT PARAMETER:							    */
/*	bool_array	specifying selected aux. object classes.	    */
/*									    */
/*  RETURN VALUE:							    */
/*	FKTMENU or FKTDEL.						    */
/*									    */
/*                                                                          */
/****************************************************************************/
 
signed32 d20_call_disp_aux( D20_oct *oct_ent,
                            Bool bool_array[] )
{

signed32	key_pressed;
signed32	i;

if (rbatch)
	return(get_aux_obj_rbatch(oct_ent, bool_array));

*(signed32 *)d20_12omask[DS_12_USE].d20_ioinfo = D20_12_AUX_OCLASS_LIST;
d20_12omask[DS_12_F1_POS].d20_fkt_id = D20_NO_OP;

/* object counter is a global variable.		*/
object_counter = oct_ent->no_aux_class;

summary.max_page = object_counter % SUMMARY_LINES_PER_PAGE;
if (!summary.max_page)
	summary.max_page = object_counter / SUMMARY_LINES_PER_PAGE;
else
     	summary.max_page = object_counter / SUMMARY_LINES_PER_PAGE + 1;

if ( (summary.summary_list = (struct summary_item *)malloc
	               (object_counter * sizeof(struct summary_item))) == 0)
	{
	d20_qomask[D20_Q1POS].d20_ioinfo = d20_err_table[D20_E04IDX];
  	d20_handlemask(d20_qomask); /* memory error */
	return(D2_ERROR);
	}

summary.current_page = 1;
summary.current_line = 1;
summary.num_selected = 0;
summary.current_display = 0;

first_list = summary.summary_list;

for (i=0; i<oct_ent->no_aux_class; i++)
	{
	strncpy(summary.summary_list->summary_name, 
	oct_ent->aux_class[i]->obj_name, MAX_T61_DISP_STR_LEN);
	summary.summary_list->summary_name[MAX_T61_DISP_STR_LEN] = D2_EOS;
	summary.summary_list++->selected = FALSE;
	}

key_pressed = display_oclass_sch();

if (key_pressed == FKTMENU)
	for (i=0; i<oct_ent->no_aux_class; i++)
		bool_array[i] = (first_list + i)->selected;

free(first_list);
d20_12omask[DS_12_F1_POS].d20_fkt_id = D20_FKTX;

if (key_pressed == FKTMENU)
	return(D2_NOERROR);
else
	return(D2_ERROR);

}

/****************************************************************************/
/*                                                                          */
/*  NAME:  	get_aux_obj_rbatch()				    	    */
/*  AUTHOR:	Sanjay Jain						    */
/*  DATE:	09.06.92						    */
/*                                                                          */
/*  DESCRIPTION:                                                            */
/*	This function gets the aux. object classes when admin. is run in    */
/* 	batch mode. This function checks the validity of aux. object class  */
/*	names. It fills the bool_array to indicate which aux. object classes*/
/*	have been selected. 						    */
/*									    */
/*  INPUT PARAMETER:                                                        */
/*	oct_ent:	Contains list of all valid aux. object classes 	    */
/*			belonging to selected Object class.		    */
/*									    */
/*  OUTPUT PARAMETER:							    */
/*	bool_array	array of Bool type to indicate which aux. object    */
/*			classes have been selected.			    */
/*  RETURN VALUE:							    */
/*	D2_ERROR	when not a correct aux. object class has been 	    */
/*			specified.			    		    */
/*	D2_NOERROR	in case of no error.				    */
/*									    */
/*                                                                          */
/****************************************************************************/

static	signed32	get_aux_obj_rbatch(
    D20_oct	*oct_ent,
    Bool	bool_array[])
{

signed32 i, index, ret_value = D2_NOERROR;
char 	message[D20_MAXMSG];

for (i=0; i<oct_ent->no_aux_class; i++)
	bool_array[i] = FALSE;

/* blank is the filler character in this mask.	*/
getfill = ' ';

/* get next set of attributes.	*/
for (;;)
	{
	/* initialise input fields including More field.	*/
	for (i=0; i<=D20_12B_NO_ITEMS; i++)
		*d20_12bifields[i] = D2_EOS;

	d20_handlemask(d20_12bomask);

	/* check entered aux. object classes against list of valid aux.	*/
	/* object classes.						*/

	for (i=0; i<D20_12B_NO_ITEMS; i++)
		{
		if (strlen(d20_12bifields[i]) != 0)
			{
			for (index=0; index < oct_ent->no_aux_class; index++)
				if (strcmp(d20_12bifields[i], 
				    oct_ent->aux_class[index]->obj_name) == 0)
					break;

			/* aux. obj. class not found in list of valid aux. */
			/* object classes.				   */
		 	if (index == oct_ent->no_aux_class)
				{
				/* display error message.	*/
				sprintf(message,d20_errtab[D20_E07IDX],i+1);
				d20_qomask[D20_Q1POS].d20_ioinfo = message;
				d20_handlemask(d20_qomask);
				ret_value = D2_ERROR;
				break;
				}

			bool_array[index] = TRUE;

			}
		}
	
	if (ret_value == D2_ERROR)
		break;

	/* more field is not set.	*/
	if (*d20_12bifields[D20_12B_NO_ITEMS] == D2_EOS)
		break;
	}

getfill = DS_DEFAULT_VALUE;

return(ret_value);

}

/****************************************************************************/
/* 	Following functions handle attribute list (making, display and 	    */
/*      selection)                  					    */
/*                                                                          */
/****************************************************************************/

/****************************************************************************/
/*                                                                          */
/*  NAME:  	d20_handle_attr_list()					    */
/*  AUTHOR:	Sanjay Jain						    */
/*  DATE:	09.06.92						    */
/*                                                                          */
/*  DESCRIPTION:                                                            */
/*	This function integrates following functionality.    		    */
/*	- Make a list of valid attributes to be displayed depending on      */
/*		operation, srt_ent and oct_ent.				    */
/*	- Display this list and get the selected attributes.		    */
/*	- Return pointers to D20_at for selected attributes.		    */
/*									    */
/*  INPUT PARAMETER:                                                        */
/*	operation: 	DS_ADD_OBJECT, DS_ADD_ATTRIBUTE, DS_MODIFY_ATTRIBUTE*/
/*			and DS_DELETE_ATTRIBUTE.			    */
/*	srt_ent:	To know the naming attributes.			    */
/*	oct_ent:	Contains list of all valid attributes belonging to  */
/*			selected Object and auxiliary classes.		    */
/*									    */
/*  OUTPUT PARAMETER:							    */
/*	attr_ptr_space	containing pointers to selected D20_at entries.     */
/*	no_selected_attrs	count of selected attributes.		    */
/*  RETURN VALUE:							    */
/*	D2_ERROR	in case of error.				    */
/*	D2_NOERROR	in case of no error.				    */
/*									    */
/*                                                                          */
/****************************************************************************/

signed32 d20_handle_attr_list(
    signed32    operation,
    D20_srt    *srt_ent,
    D20_oct    *oct_ent,
    D20_at   ***attr_ptr_space,
    signed32   *no_selected_attrs)
{

signed32 mand_cnt = 0, opt_cnt = 0;
signed32 ret_value = D2_NOERROR;
signed32 i;

*attr_ptr_space = (D20_at **)NULL;

/* fill summary and summary_list with attribute names information.	*/
if (make_attr_list(operation, srt_ent, oct_ent, &mand_cnt, &opt_cnt)
						== D2_ERROR)
	return(D2_ERROR);

if (rbatch)
	/* get list of attribute names.		*/
	ret_value = get_attr_list_rbatch(oct_ent, operation, 
				attr_ptr_space, no_selected_attrs);
else
	{
	/* display attribute names.					*/

	*(signed32 *)d20_12omask[DS_12_USE].d20_ioinfo = D20_12_ATTR_LIST;

	if ((ret_value = display_attr_list(operation, mand_cnt, oct_ent)) ==
								D2_NOERROR)
		{
		if (summary.num_selected != 0)
			{
			if ((*attr_ptr_space = (D20_at **)malloc(
				sizeof(D20_at *) * summary.num_selected)) == 
							(D20_at **)NULL)
				{
				d20_qomask[D20_Q1POS].d20_ioinfo = 
						d20_err_table[D20_E04IDX];
				d20_handlemask(d20_qomask); /* memory error */
				ret_value = D2_ERROR;	
				}
			}
		}

	if (ret_value == D2_NOERROR)
		{
		/* fill attr_ptr_space with pntrs to selected D20_at entries. */

		*no_selected_attrs = 0;

		for (i=0; i<object_counter; i++)
			{
			if ((first_list + i)->selected)
				{
				*(*attr_ptr_space + *no_selected_attrs) = 
				get_attr_ptr(oct_ent, 
				(first_list + i)->summary_name);

				(*no_selected_attrs)++;

				if (operation == DS_MODIFY_ATTRIBUTE)
					break;
				}
			}
		}
	}

/* free space of summary_list which is allocated by make_attr_list function. */
/* or altered in display_attr_list by extend_attr_list.			     */
if (first_list != (struct summary_item *)NULL)
	free(first_list);

return(ret_value);

}

/****************************************************************************/
/*                                                                          */
/*  NAME:  	make_attr_list()					    */
/*  AUTHOR:	Sanjay Jain						    */
/*  DATE:	09.06.92						    */
/*                                                                          */
/*  DESCRIPTION:                                                            */
/*	This function fills the 'summary' structure and 'summary_list'      */
/*	with proper attribute names.					    */
/*                                                                          */
/*  INPUT PARAMETER:                                                        */
/*	operation: 	DS_ADD_OBJECT, DS_ADD_ATTRIBUTE, DS_MODIFY_ATTRIBUTE*/
/*			and DS_DELETE_ATTRIBUTE.			    */
/*	srt_ent:	To know the naming attributes.			    */
/*	oct_ent:	Contains list of all valid attributes belonging to  */
/*			selected Object and auxiliary classes.		    */
/*									    */
/*  OUTPUT PARAMETER:							    */
/*	mand_cnt:	Count of attributes to be highlighted always in     */
/*			case of DS_ADD_OBJECT.				    */
/*	opt_cnt:	Count of remaining attributes.			    */
/*									    */
/*  RETURN VALUE:							    */
/*	error:		if malloc for summary_list fails.		    */
/*	no error:	otherwise.					    */
/*									    */
/*                                                                          */
/****************************************************************************/

static 	signed32 make_attr_list(
	signed32  operation,
	D20_srt	 *srt_ent,
	D20_oct	 *oct_ent,
	signed32 *mand_cnt,
	signed32 *opt_cnt)
{
struct summary_item *mand_list, *opt_list;
signed32	i, j;

/* fill summary and summary list information depending on input parameters. */

/* maximum no. of attributes which could be there in summary_list.	*/
if (oct_ent == D20_UNKNOWN)
	max_attr_cnt = d2_nd20at;
else
	/* in case of cache admin., Add of DSA object (DSA-Type attribute) */
	/* in case of Add Object mand. attr. occur twice in the list.	   */
	max_attr_cnt = (oct_ent->no_must_attr + 
			oct_ent->no_must_attr + oct_ent->no_may_attr + 1);

/* allocate space for attribute pointers.				*/
if ( (summary.summary_list = (struct summary_item *)malloc ((max_attr_cnt) 
				* sizeof(struct summary_item))) == 0)
	{
	d20_qomask[D20_Q1POS].d20_ioinfo = d20_err_table[D20_E04IDX];
  	d20_handlemask(d20_qomask); /* memory error */
	return(D2_ERROR);
	}

/* case of modify an attribute of a subtree (MODIFY ALL).	*/
if (oct_ent == D20_UNKNOWN)
	{
	first_list = mand_list = opt_list = summary.summary_list;
	*mand_cnt = *opt_cnt = 0;

	for (i=0; i<d2_nd20at; i++)
		{
		if (d20_cmp_objid(&((d2_p_d20at + i)->obj_id),
			sizeof(D2_MASTER_KNOWLEDGE),
			(Ob_id_string) D2_MASTER_KNOWLEDGE))
			continue;

		/* Skip the DME alternate address which should	*/
		/* not be modified				*/
		if(d20_cmp_objid(&((d2_p_d20at + i)->obj_id),
		    sizeof(D2_DME_ALT_ADDR),(Ob_id_string) D2_DME_ALT_ADDR))
			continue;

		switch ((int) (d2_p_d20at + i)->at.att_syntax)
		  	{
		  	case D2_ANY_STX:
		  	case D2_GUIDE:
		  	case D2_TTXID_STX:
		  	case D2_FAXNR_STX:
		  	case D2_TLXNR_STX:
			case D2_PSAP_STX:
			case D2_PR_DM_STX:
			case D2_CERTIFICATE:
			case D2_CERT_PAIR:
			case D2_CERT_LIST:
				continue;

			default:
				break;
			}

		strncpy((opt_list + *opt_cnt)->summary_name, 
		(d2_p_d20at + i)->at.att_name, MAX_T61_DISP_STR_LEN);
		(opt_list + *opt_cnt)->summary_name[MAX_T61_DISP_STR_LEN] = 
									D2_EOS;

		(opt_list + *opt_cnt)->selected = FALSE;

		(*opt_cnt)++;
		}
	}	/* end of case when oct_ent is UNKNOWN.		*/
else
	{
	first_list = mand_list = summary.summary_list;
	opt_list = mand_list + oct_ent->no_must_attr;
	*mand_cnt = *opt_cnt = 0;

	/* do not handle mandatory attributes if operation is 	*/
	/* DS_DELETE_ATTRIBUTE or DS_ADD_ATTRIBUTE.		*/

	if (operation != DS_DELETE_ATTRIBUTE && operation != DS_ADD_ATTRIBUTE)
		{
		for (i=0; i<oct_ent->no_must_attr; i++)
			{
			if (operation == DS_ADD_OBJECT)
				{
				/* if naming attribute put it in optional  */
				/* attr. list and then continue;	   */

				/* check if attribute is one of the 	   */
				/* naming attributes.			   */
				for (j = 0; j<srt_ent->no_nam_attr; j++)
				if (strcmp(oct_ent->must_attr[i]->at.att_abbrv,
				    srt_ent->nam_attr[j]->at.att_abbrv) == 0)
					break;

				/* attribute is one of the naming attributes.*/
				if (j != srt_ent->no_nam_attr)
					{
					opt_list--;
					strncpy(opt_list->summary_name, 
					oct_ent->must_attr[i]->at.att_name, 
					MAX_T61_DISP_STR_LEN);
					opt_list->summary_name[
					MAX_T61_DISP_STR_LEN] = D2_EOS;
					opt_list->selected = FALSE;
					(*opt_cnt)++;
					continue;
					}
				}

			/* skip object class attribute.		 */
			if (d20_cmp_objid(&(oct_ent->must_attr[i]->obj_id),
					sizeof(D2_OBJ_CLASS),
					(Ob_id_string) D2_OBJ_CLASS))
				continue;

			/* In case of Modify Attribute, skip the DME       */
			/* alternate address which should not be modified  */
			if( operation == DS_MODIFY_ATTRIBUTE &&
			    d20_cmp_objid(&(oct_ent->must_attr[i]->obj_id),
			    	sizeof(D2_DME_ALT_ADDR),
				(Ob_id_string) D2_DME_ALT_ADDR))
				continue;

			switch(oct_ent->must_attr[i]->at.att_syntax)
			{
			    /* skip attribute with D2_GUIDE syntax since */
			    /* it is not implemented in administration.	 */
			    case D2_GUIDE:
				continue;
			    /* skip Certificate attributes		*/
			    case D2_CERTIFICATE:
			    case D2_CERT_PAIR:
			    case D2_CERT_LIST:
				if( operation == DS_MODIFY_ATTRIBUTE
				    || operation == DS_ADD_ATTRIBUTE )
				    continue;
				else
				    break;

			    default:
				break;
			}

			/* put attribute in summary list, if operation 	*/
			/* is ADD Object highlight this attr.		*/

			strncpy((mand_list + *mand_cnt)->summary_name, 
			oct_ent->must_attr[i]->at.att_name, 
			MAX_T61_DISP_STR_LEN);
			(mand_list + *mand_cnt)->summary_name[
			MAX_T61_DISP_STR_LEN] = D2_EOS;

			if (operation == DS_ADD_OBJECT)
				{
				(mand_list + *mand_cnt)->selected = TRUE;
				
				/* if attribute is recurring, put this 	*/
				/* attribute in optional list also.	*/
				/* only when it is not rbatch mode.	*/
				if (oct_ent->must_attr[i]->at.att_no_recur != 1
						&& !rbatch)
					{ 
					strncpy(
					(opt_list + *opt_cnt)->summary_name, 
					oct_ent->must_attr[i]->at.att_name, 
					MAX_T61_DISP_STR_LEN);
					(opt_list + *opt_cnt)->summary_name[
					MAX_T61_DISP_STR_LEN] = D2_EOS;

					(opt_list + *opt_cnt)->selected = 
								FALSE;

					(*opt_cnt)++;
					}
				}
			else	
				(mand_list + *mand_cnt)->selected = FALSE;

			(*mand_cnt)++;

			} /* end of for loop.	*/
		} /* end of if	*/

	/* make list of optional attributes.	*/
	for (i=0; i<oct_ent->no_may_attr; i++)
		{
		if (d20_cmp_objid(&(oct_ent->may_attr[i]->obj_id), 
			sizeof(D2_ACL), (Ob_id_string) D2_ACL) && d20_cache)
			continue;

		/* Skip the DME alternate address which should	*/
		/* not be modified				*/
		if (operation == DS_MODIFY_ATTRIBUTE && 
		    d20_cmp_objid(&(oct_ent->may_attr[i]->obj_id),
		    sizeof(D2_DME_ALT_ADDR),(Ob_id_string) D2_DME_ALT_ADDR))
			continue;

		/* skip ACL or MK when operation is DELETE_ATTR or ADD_ATTR */
		if ((operation == DS_DELETE_ATTRIBUTE || 
		     operation == DS_ADD_ATTRIBUTE) && 
		    (d20_cmp_objid(&(oct_ent->may_attr[i]->obj_id), 
		     sizeof(D2_ACL), (Ob_id_string) D2_ACL) ||
		     d20_cmp_objid(&(oct_ent->may_attr[i]->obj_id), 
		     sizeof(D2_MASTER_KNOWLEDGE),
		     (Ob_id_string) D2_MASTER_KNOWLEDGE)))
			continue;

		/* skip attribute with D2_GUIDE syntax since it is */
		/* not implemented in administration.		   */
		if (oct_ent->may_attr[i]->at.att_syntax == D2_GUIDE)
			continue;

		switch(oct_ent->may_attr[i]->at.att_syntax)
		{
		    /* skip Certificate attributes		*/
		    case D2_CERTIFICATE:
		    case D2_CERT_PAIR:
		    case D2_CERT_LIST:
			if( operation == DS_MODIFY_ATTRIBUTE
			    || operation == DS_ADD_ATTRIBUTE )
			    continue;
			else
			    break;

		    default:
			break;
		}

		/* if it is DSA Add in cache, skip the Printable Name,	*/
		/* it will come automatically after DSA-Type		*/
		if (d20_cmp_objid(&(oct_ent->may_attr[i]->obj_id),
		    sizeof(D2_PR_NM),
		    (Ob_id_string) D2_PR_NM) &&
		    d20_cache && operation == DS_ADD_OBJECT)
		    continue;

		/* operaion is modify attr., skip timestamp and  */
		/* SRT, OCT and AT attributes if schema object   */
		/* class is being modified.			 */

		if (operation == DS_MODIFY_ATTRIBUTE && 
			d20_cmp_objid(&(oct_ent->obj_id),
			sizeof(D2_OSCHEMA),(Ob_id_string) D2_OSCHEMA))
			if (d20_cmp_objid(&(oct_ent->may_attr[i]->obj_id),
				sizeof(D2_TIME_STAMP),
				(Ob_id_string) D2_TIME_STAMP) ||
				d20_cmp_objid(&(oct_ent->may_attr[i]->obj_id),
				sizeof(D2_SRT), (Ob_id_string) D2_SRT) ||
				d20_cmp_objid(&(oct_ent->may_attr[i]->obj_id),
				sizeof(D2_OCT), (Ob_id_string) D2_OCT) ||
				d20_cmp_objid(&(oct_ent->may_attr[i]->obj_id),
				sizeof(D2_AT), (Ob_id_string) D2_AT))
				continue;

		strncpy((opt_list + *opt_cnt)->summary_name, 
		oct_ent->may_attr[i]->at.att_name, MAX_T61_DISP_STR_LEN);
		(opt_list + *opt_cnt)->summary_name[MAX_T61_DISP_STR_LEN] = 
									D2_EOS;

		(opt_list + *opt_cnt)->selected = FALSE;

		(*opt_cnt)++;
		}

	/* if it is DSA Add in cache put DSA-Type in the list of 	*/
	/* optional attributes. 					*/
	if (d20_cmp_objid(&(oct_ent->obj_id), sizeof(D2_ODSA),
		(Ob_id_string) D2_ODSA) &&
		d20_cache && operation == DS_ADD_OBJECT)
		{
		strncpy((opt_list + *opt_cnt)->summary_name, 
		D2_TYPE_DSA, MAX_T61_DISP_STR_LEN);

		strcpy(dsa_type.at.att_name, D2_TYPE_DSA);
		dsa_type.at.att_no_recur = 1;
		dsa_type.at.att_syntax = D2_PRINTABLE;

		(opt_list + *opt_cnt)->summary_name[MAX_T61_DISP_STR_LEN] = 
									D2_EOS;
		(opt_list + *opt_cnt)->selected = FALSE;
		(*opt_cnt)++;
		}

	/* shift optional attribute list towards mandatory attribute 	*/
	/* list if reqrd. 						*/
	if ((mand_list + *mand_cnt) != opt_list)
		{
		memcpy((char *)(mand_list + *mand_cnt), (char *)opt_list, 
		sizeof(struct summary_item) * *opt_cnt);
		opt_list = mand_list + *mand_cnt;
		}

	}	/* end of case when particular oct_ent is being handled.  */

if (operation == DS_ADD_OBJECT)
	{
	/* sort optional and mandatatory attribute list seperatly */

	/* sort list of mandatory attributes		*/
	qsort((char *)mand_list,(size_t)*mand_cnt,
	      (size_t)sizeof(struct summary_item),
	      (int (*)(const void *, const void *))summarycmp);

	/* sort list of optional attributes		*/
	qsort((char *)opt_list,(size_t)*opt_cnt,
	      (size_t)sizeof(struct summary_item),
	      (int (*)(const void *, const void *))summarycmp);
	}
else
	{
	/* sort optional and mandatatory attribute list together */

	qsort((char *)mand_list,(size_t)(*mand_cnt + *opt_cnt),
	      (size_t)sizeof(struct summary_item),
	      (int (*)(const void *, const void *))summarycmp);

	*opt_cnt += *mand_cnt;
	*mand_cnt = 0;
	}

summary.current_page = 1;
summary.current_line = 1;
summary.num_selected = *mand_cnt;
summary.current_display = 0;

/* object_counter is a global varible, it's value has to be set.	*/
object_counter = *mand_cnt + *opt_cnt;

summary.max_page = object_counter % SUMMARY_LINES_PER_PAGE;
if (!summary.max_page)
	summary.max_page = object_counter / SUMMARY_LINES_PER_PAGE;
else
     	summary.max_page = object_counter / SUMMARY_LINES_PER_PAGE + 1;

return(D2_NOERROR);

}

/****************************************************************************/
/*                                                                          */
/*  NAME:  	display_attr_list()				    	    */
/*  AUTHOR:	Sanjay Jain						    */
/*  DATE:	09.06.92						    */
/*                                                                          */
/*  DESCRIPTION:                                                            */
/*	This function displays the attribute list. In case of DS_ADD_OBJECT */
/*	mandatory attributes are always highlighted, cursor never goes on   */
/*	them (they are automatically selected). 			    */
/*		In case of DS_MODIFY_ATTRIBUTE after one attribute is       */
/*	selected, function returns.					    */
/*                                                                          */
/*  INPUT PARAMETER:                                                        */
/*	operation:	special handling for DS_ADD_OBJECT and 		    */
/*			DS_MODIFY_ATTRIBUTE.				    */ 
/*	mand_cnt:	count of mandatory attrs, used for DS_ADD_OBJECT.   */
/*									    */
/*  OUTPUT PARAMETER:							    */
/*									    */
/*  RETURN VALUE:							    */
/*	D2_ERROR	when FKTDEL key is pressed.			    */
/*	D2_NOERROR	otherwise.					    */
/*									    */
/*                                                                          */
/****************************************************************************/

static signed32 display_attr_list(
	signed32  operation,
	signed32  mand_cnt,
	D20_oct	 *oct_ent)
{
signed32	rem_mand_cnt;
signed32	i, j;
signed32	key_pressed;
signed32	ret_value = D2_NOERROR;
Bool	extend_flag = FALSE;

d20_12omask[0].d20_fkt_id &= ~D20_NOERASE;
cursor_stay = FALSE;

/* all mandatory attributes are automatically selected in case of */
/* Add object and cursor bar should not go to these attributes for*/
/* Add Object.	     						  */
/* In case of other operations mand_cnt = 0.			  */

/* mandatory attributes left to be displayed.		*/
rem_mand_cnt = mand_cnt - ((summary.current_page - 1) * SUMMARY_LINES_PER_PAGE);

if (rem_mand_cnt <= 0)
	rem_mand_cnt = 0;
else
	if (rem_mand_cnt >= SUMMARY_LINES_PER_PAGE)
		rem_mand_cnt = SUMMARY_LINES_PER_PAGE;
	else
		{
		strcpy(d20_12barmenu[0], 
		(first_list + rem_mand_cnt)->summary_name);
		cursor_stay = TRUE;
		}

for (i=DS_12_FIRST, j=1; j <= rem_mand_cnt; i++, j++)
	d20_12omask[i].d20_attribute = NORMAL;

/* disable F1 key for display of attribute list. */
d20_12omask[DS_12_F1_POS].d20_fkt_id = D20_NO_OP;

if (operation == DS_ADD_OBJECT || operation == DS_ADD_ATTRIBUTE)
	d20_12omask[DS_12_F8_POS].d20_fkt_id = D20_FKTX;

getwerg = 0;

for (;;)
	{
	key_pressed = display_mask_18();

	switch (key_pressed)
		{
    		case FKTCR: /* carriage return was pressed */
			if (getwerg >= 0)
				getwerg += rem_mand_cnt;

		       	handle_barmenu();

			if (getwerg >= 0)
				getwerg -= rem_mand_cnt;

			cursor_stay = TRUE;
			/* no erase */
			d20_12omask[0].d20_fkt_id |= D20_NOERASE; 

		    	break;

		case FKTF8: /* F8 was pressed to specify two attribute values.*/
			if (getwerg >= 0)
				getwerg += rem_mand_cnt;

			ret_value = extend_attr_list(oct_ent, &extend_flag);

			if (ret_value == D2_ERROR)
				{
				/* display error message.	*/
				/* memory reallocation problem.	*/
				d20_msgomask[D20_M1POS].d20_ioinfo = 
						d20_errtab[D20_E20IDX];
				d20_handlemask(d20_msgomask); 
				key_pressed = FKTDEL;
				break;
				}

			/* if attr. list extended , display mask after erase. */
			if (extend_flag)
				{
				/* erase */
				d20_12omask[0].d20_fkt_id &= ~D20_NOERASE; 

				getwerg++;

				summary.max_page = object_counter % 
							SUMMARY_LINES_PER_PAGE;
				if (!summary.max_page)
					summary.max_page = 
					object_counter / SUMMARY_LINES_PER_PAGE;
				else
					summary.max_page = object_counter / 
					SUMMARY_LINES_PER_PAGE + 1;

				if (getwerg == SUMMARY_LINES_PER_PAGE)
					{
					cursor_stay = FALSE;
					key_pressed = FKTSCD;
					scroll(FKTSCD);
					}
				else
					cursor_stay = TRUE;
				}
			else
				{
				/* no erase */
				d20_12omask[0].d20_fkt_id |= D20_NOERASE; 
				cursor_stay = TRUE;
				}

			/* same page is being displayed.	*/
			if (getwerg > 0)
				getwerg -= rem_mand_cnt;

		    	break;

		case FKTDEL: /* delete key was pressed */
		    	break;

		case FKTMENU: /* menu key was pressed */
			break;

		case FKTSCD: /* fat down arrow was pressed (scroll down) */
			cursor_stay = FALSE;
			scroll(FKTSCD);
		    	break;

		case FKTSCU:  /* fat up arrow was pressed (scroll up) */
			cursor_stay = FALSE;
			scroll(FKTSCU);
		    	break;
		}

	if (key_pressed == FKTDEL || key_pressed == FKTMENU || key_pressed == 
			FKTSCD || key_pressed == FKTSCU)
		/* reset previously set attributes.	*/
		for (i=DS_12_FIRST, j=1; j <= rem_mand_cnt; i++, j++)
			d20_12omask[i].d20_attribute = WAHL;

	if (key_pressed == FKTDEL || key_pressed == FKTMENU)
		break;

	/* only one attribute is to be selected in DS_MODIFY_ATTRIBUTE.	*/
	if (operation == DS_MODIFY_ATTRIBUTE && key_pressed == FKTCR)
		break;

	/* all mandatory attributes are automatically selected in case of */
	/* Add object and cursor bar should not go to these attributes for*/
	/* Add Object.	     						  */
	if ((key_pressed == FKTSCD || key_pressed == FKTSCU))
		{
		/* set attributes for new fields.	*/

		/* mandatory attributes left to be displayed.		*/
		rem_mand_cnt = mand_cnt - 
			((summary.current_page - 1) * SUMMARY_LINES_PER_PAGE);

		if (rem_mand_cnt <= 0)
			rem_mand_cnt = 0;
		else
			if (rem_mand_cnt >= SUMMARY_LINES_PER_PAGE)
				rem_mand_cnt = SUMMARY_LINES_PER_PAGE;
			else
				{
				strcpy(d20_12barmenu[0], 
				(first_list + rem_mand_cnt)->
				summary_name);
				cursor_stay = TRUE;
				}

		for (i=DS_12_FIRST, j=1; j <= rem_mand_cnt; i++, j++)
			d20_12omask[i].d20_attribute = NORMAL;

		}

    	}

/* enable F1 key for display of object list .	*/
d20_12omask[DS_12_F1_POS].d20_fkt_id = D20_FKTX;
d20_12omask[DS_12_F8_POS].d20_fkt_id = D20_NO_OP;

if (key_pressed == FKTDEL)
	return(D2_ERROR);
else
	return(D2_NOERROR);
}

/****************************************************************************/
/*                                                                          */
/*  NAME:  	extend_attr_list()				    	    */
/*  AUTHOR:	Sanjay Jain						    */
/*  DATE:	09.06.92						    */
/*                                                                          */
/*  DESCRIPTION:                                                            */
/*	This function is called when operation is ADD_OBJECT or ADD_ATTR.    */
/*	If a recurring attribute is selected, one more entry of the same    */
/* 	attribute name is created in the summary_list.			    */
/*									    */
/*  INPUT PARAMETER:                                                        */
/*	D20_oct 	*oct_ent;	OCT entry to find out AT pointer    */
/*					to selected attribute.		    */
/*									    */
/*  OUTPUT PARAMETER:							    */
/*	Bool		*extend_flag; 	Indicates if attribute list has     */
/*					been extended. 			    */
/*									    */
/*  RETURN VALUE:							    */
/*	D2_ERROR	realloc fails.					    */
/*	D2_NOERROR	otherwise.					    */
/*									    */
/*                                                                          */
/****************************************************************************/

static signed32 extend_attr_list(D20_oct *oct_ent, Bool *extend_flag)
{
signed32 		first_line;
struct summary_item 	*p;
D20_at			*at_ptr;

p = first_list;
first_line = (summary.current_page - 1) * SUMMARY_LINES_PER_PAGE + 1;
p += (first_line - 1);  /* set pointer to first item on the page */

p += getwerg;           /* set pointer to actually selected item */


/* the current attribute has not been selected.	*/
if (p->selected == FALSE) 
	{
	/* select object */
	p->selected = TRUE; 
	summary.num_selected++;
	}

at_ptr = get_attr_ptr(oct_ent, p->summary_name);

/* attribute is recurring.	*/
if (at_ptr->at.att_no_recur != 1)
	{
	/* space for summary items is full.	*/
	if (object_counter == max_attr_cnt)
		{
		/* allocate extra space.	*/
		if ((first_list = summary.summary_list = 
			(struct summary_item *)realloc(first_list, 
			(max_attr_cnt + EXTRA_ATTR) * 
			sizeof(struct summary_item))) == 
			(struct summary_item *)NULL)
			return(D2_ERROR);

		max_attr_cnt += EXTRA_ATTR;
		p = first_list;
		p += (first_line - 1);  
		p += getwerg;          
		}

	memmove(p + 2, p + 1, (object_counter - first_line - getwerg) * 
			     sizeof(struct summary_item));

	strcpy((p + 1)->summary_name, p->summary_name);
	(p + 1)->selected = TRUE;
	summary.num_selected++;
	object_counter++;

	*extend_flag = TRUE;
	}
else
	*extend_flag = FALSE;

return(D2_NOERROR);
}


/****************************************************************************/
/*                                                                          */
/*  NAME:  	get_attr_list_rbatch()				    	    */
/*  AUTHOR:	Sanjay Jain						    */
/*  DATE:	09.06.92						    */
/*                                                                          */
/*  DESCRIPTION:                                                            */
/*	This function gets the attribute names when admin. is run in batch  */
/*	mode. This function checks the validity of attribute names. It 	    */
/*	returns pointers to D20_at of entered attributes.		    */
/*									    */
/*  INPUT PARAMETER:                                                        */
/*	operation: 	DS_ADD_OBJECT, DS_ADD_ATTRIBUTE, DS_MODIFY_ATTRIBUTE*/
/*			and DS_DELETE_ATTRIBUTE.			    */
/*			In case of MODIFY_ATTR only one attr. name can be   */
/*			entered.					    */
/*	oct_ent:	Contains list of all valid attributes belonging to  */
/*			selected Object and auxiliary classes.		    */
/*									    */
/*  OUTPUT PARAMETER:							    */
/*	attr_ptr_space	containing pointers to selected D20_at entries.     */
/*	no_selected_attrs	count of selected attributes.		    */
/*  RETURN VALUE:							    */
/*	D2_ERROR	malloc error or when not all mand. attrs. have been */
/*			entered.				    	    */
/*	D2_NOERROR	in case of no error.				    */
/*									    */
/*                                                                          */
/****************************************************************************/

static	signed32 get_attr_list_rbatch(
	D20_oct	   *oct_ent,
	signed32    operation,
	D20_at	 ***attr_ptr_space,
	signed32   *no_selected_attrs)
{

signed32 *selected_array;
signed32  i, index, ret_value = D2_NOERROR;
char 	  message[D20_MAXMSG];
signed32  max_sel_attrs; /* size of space to store pointers to selected attrs.*/

/* allocate space for integers to store selected attribute info.	*/
if ((selected_array = (signed32 *)malloc(object_counter * sizeof(signed32))) == 
							(signed32 *)NULL)
	{
	d20_qomask[D20_Q1POS].d20_ioinfo = d20_err_table[D20_E04IDX];
  	d20_handlemask(d20_qomask); /* memory error */
	return(D2_ERROR);
	}

/* allocate space for attribute pointers.	*/
if ((*attr_ptr_space = (D20_at **)malloc(sizeof(D20_at *) * object_counter)) ==
							(D20_at **)NULL)
	{
	d20_qomask[D20_Q1POS].d20_ioinfo = d20_err_table[D20_E04IDX];
  	d20_handlemask(d20_qomask); /* memory error */
	free((char *)selected_array);
	return(D2_ERROR);
	}

max_sel_attrs = object_counter;

/* initialise the selected_array.	*/
for (i=0; i<object_counter; i++)
	*(selected_array + i) = FALSE;

/* blank is the filler character in this mask.	*/
getfill = ' ';

if (operation == DS_MODIFY_ATTRIBUTE)
	{
	/* in case of MODIFY_ATTR only one attr. name is allowed to be entered*/
	for (i=D20_12B_START_POS+1; i<D20_12B_START_POS + D20_12B_NO_ITEMS; i++)
		d20_12bomask[i].d20_fkt_id = D20_NO_OP;

	/* make more fields NO_OP.	*/
	d20_12bomask[i].d20_fkt_id = D20_NO_OP;
	d20_12bomask[i + 1].d20_fkt_id = D20_NO_OP;
	}

*no_selected_attrs = 0;

/* get next set of attributes.	*/
for (;;)
	{
	/* initialise input fields including More field.	*/
	for (i=0; i<=D20_12B_NO_ITEMS; i++)
		*d20_12bifields[i] = D2_EOS;

	d20_handlemask(d20_12bomask);

	/* check attributes against list of valid attributes.	*/
	for (i=0; i<D20_12B_NO_ITEMS; i++)
		{
		if (strlen(d20_12bifields[i]) != 0)
			{
			index = search_in_valid_attrs(d20_12bifields[i]);

			/* attribute not found in list of valid attributes. */
			if (index < 0)
				{
				/* display error message.	*/
				sprintf(message,d20_errtab[D20_E10IDX],i+1);
				d20_qomask[D20_Q1POS].d20_ioinfo = message;
				d20_handlemask(d20_qomask);
				ret_value = D2_ERROR;
				break;
				}

			*(selected_array + index) = TRUE;

			if (*no_selected_attrs == max_sel_attrs)
				if ((*attr_ptr_space = (D20_at **)realloc(
					*attr_ptr_space, sizeof(D20_at *) * 
					(max_sel_attrs + EXTRA_ATTR))) ==
							(D20_at **)NULL)
					{
					d20_qomask[D20_Q1POS].d20_ioinfo = 
						d20_err_table[D20_E04IDX];
					/* memory error */
					d20_handlemask(d20_qomask); 
					ret_value = D2_ERROR;
					break;
					}
				else
					max_sel_attrs += EXTRA_ATTR;

			*(*attr_ptr_space + *no_selected_attrs) = 
			get_attr_ptr(oct_ent, 
					(first_list + index)->summary_name);

			(*no_selected_attrs)++;
			}
		}
	
	if (ret_value == D2_ERROR)
		break;

	if (*d20_12bifields[D20_12B_NO_ITEMS] == D2_EOS)
		break;
	}

if (ret_value == D2_NOERROR)
	{
	/* check if all mandatory attributes have been entered.	*/
	for (i=0; i<object_counter; i++)
		if ((first_list + i)->selected == TRUE && 
					*(selected_array + i) == FALSE)
			{
			/* not all mandatory attributes have been entered. */
			d20_qomask[D20_Q1POS].d20_ioinfo = 
							d20_errmess[D20_E75IDX];
			d20_handlemask(d20_qomask);
			ret_value = D2_ERROR;
			break;
			}
			
	}

getfill = DS_DEFAULT_VALUE;

if (operation == DS_MODIFY_ATTRIBUTE)
	{
	/* reset the original status of the fields.	*/
	for (i=D20_12B_START_POS+1; i<D20_12B_START_POS + D20_12B_NO_ITEMS; i++)
		d20_12bomask[i].d20_fkt_id = D20_CGET;

	/* make more fields active.	*/
	d20_12bomask[i].d20_fkt_id = D20_CSAY;
	d20_12bomask[i + 1].d20_fkt_id = D20_CGET;
	}

/* release space for integers to store selected attribute info.	*/
free((char *)selected_array);

return(ret_value);

}

/****************************************************************************/
/*                                                                          */
/*  NAME:  	search_in_valid_attrs()				    	    */
/*  AUTHOR:	Sanjay Jain						    */
/*  DATE:	09.06.92						    */
/*                                                                          */
/*  DESCRIPTION:                                                            */
/*	This function looks if an attribute name is in the list of valid    */
/*	attributes.							    */
/*									    */
/*  INPUT PARAMETER:                                                        */
/*	attr_name to be searched in summary_list.			    */
/*									    */
/*  OUTPUT PARAMETER:							    */
/*									    */
/*  RETURN VALUE:							    */
/*	index in summary_list if attribute found in the list.		    */
/*	-1 if not found.						    */	
/*									    */
/*                                                                          */
/****************************************************************************/

static  signed32 search_in_valid_attrs(char *attr_name)
{
Bool	 found = FALSE;
signed32 i, sav_index;

for (i=0; i<object_counter; i++)
	{
	if (strncmp(attr_name, (first_list + i)->summary_name, 
							strlen(attr_name)) == 0)
		{
		if (found == TRUE)
			return(-1);

		found = TRUE;
		sav_index = i;
		}
	}

if (found == TRUE)
	return(sav_index);
else
	return(-1);
}

/****************************************************************************/
/*                                                                          */
/*  NAME:  	get_attr_ptr()					    	    */
/*  AUTHOR:	Sanjay Jain						    */
/*  DATE:	09.06.92						    */
/*                                                                          */
/*  DESCRIPTION:                                                            */
/*	This function looks for attribute name in AT or in the list of      */
/*	mand. and opt. attributes of an oct_entry.			    */
/* 	If it is not found any where it must be DSA-Type attribute.	    */
/*									    */
/*  INPUT PARAMETER:                                                        */
/*	oct entry 							    */
/*	attribute name.							    */
/*									    */
/*	If oct entry is UNKNOWN then look for attribute name in AT.	    */
/*	otherwise look in mand. and opt. attributes of oct entry.	    */
/*									    */
/*  OUTPUT PARAMETER:							    */
/*									    */
/*  RETURN VALUE:							    */
/*	pointer to D20_at.						    */
/*									    */
/*                                                                          */
/****************************************************************************/

static 	D20_at	*get_attr_ptr(
    D20_oct	*oct_ent,
    char	*attr_name)
{
signed32	j;

if (oct_ent == D20_UNKNOWN)
	{
	for (j=0; j<d2_nd20at; j++)
		if (strcmp(attr_name, (d2_p_d20at + j)->at.att_name) == 0)
			break;

	return((D20_at *)(d2_p_d20at + j)); 

	}
else
	{
	/* search in must attribute list.	*/
	for (j=0; j<oct_ent->no_must_attr; j++)
		if (strcmp(attr_name, oct_ent->must_attr[j]->at.att_name) ==0)
			break;			

	if (j < oct_ent->no_must_attr)
		/* attribute name found.	*/
		return(oct_ent->must_attr[j]);

	/* search in may attribute list.	*/
	for (j=0; j<oct_ent->no_may_attr; j++)
		if (strcmp(attr_name,oct_ent->may_attr[j]->at.att_name) == 0)
			break;			

	if (j < oct_ent->no_may_attr)
		/* attribute name found.	*/
		return(oct_ent->may_attr[j]);

	/* attribute is DSA-Type.	*/
	return(&dsa_type);
	}

}

/****************************************************************************/
/* 	Following functions handle schema list (making, display and 	    */
/*      selection)                  					    */
/*                                                                          */
/****************************************************************************/

/****************************************************************************/
/*                                                                          */
/*  NAME:  	d20_handle_schema_list()				    */
/*  AUTHOR:	Sanjay Jain						    */
/*  DATE:	09.06.92						    */
/*                                                                          */
/*  DESCRIPTION:                                                            */
/*	This function integrates following functionality.    		    */
/*	- Make a list of key components of schema table entries.	    */
/*	- Display this list and get the selected entries.		    */
/*	- Return indices of selected entries in schema table .		    */
/*									    */
/*  INPUT PARAMETER:                                                        */
/*	operation: 	DS__AT_DISPLAY, DS__OCT_DISPLAY or DS__SRT_DISPLAY  */
/*									    */
/*  OUTPUT PARAMETER:							    */
/*	schema_index_space 	containing indices to selected schema 	    */
/*				entries.				    */
/*	no_selected_entries	count of selected entries.		    */
/*  RETURN VALUE:							    */
/*	D2_ERROR	in case of error.				    */
/*	D2_NOERROR	in case of no error.				    */
/*									    */
/*  GLOBAL VARS:							    */
/*	At	*d2_p_at;						    */
/*	Oct	*d2_p_oct;						    */
/*	Srt	*d2_p_srt;						    */
/*	signed32 d2_nat;					    	    */
/*	signed32 d2_noct;						    */
/*	signed32 d2_nsrt;						    */
/*									    */
/*                                                                          */
/****************************************************************************/

signed32 d20_handle_schema_list(signed32 operation,
                           signed32 *schema_index_space,
			   signed32 *no_selected_entries)
{
signed32	key_pressed;
signed32	i;

/* fill summary and summary_list with schema entry information.	*/
if (make_schema_list(operation, schema_index_space, *no_selected_entries) 
							== D2_ERROR)
	return(D2_ERROR);

switch (operation)
	{
	case DS__AT_DISPLAY:
		*(signed32 *)d20_12omask[DS_12_USE].d20_ioinfo = D20_12_AT_LIST;
		break;

	case DS__OCT_DISPLAY:
		*(signed32 *)d20_12omask[DS_12_USE].d20_ioinfo = D20_12_OCT_LIST;
		break;

	case DS__SRT_DISPLAY:
		*(signed32 *)d20_12omask[DS_12_USE].d20_ioinfo = D20_12_SRT_LIST;
		break;
	}

/* display schema table entries.					*/

key_pressed = display_oclass_sch();

if (key_pressed == FKTMENU || key_pressed == FKTF1)
	{
	/* fill schema_index_space. */

	*no_selected_entries = 0;

	for (i=0; i<object_counter; i++)
		{
		if ((first_list + i)->selected)
			{
			*(schema_index_space + *no_selected_entries) = i;
			(*no_selected_entries)++;
			}
		}
	}

/* free space of summary_list which is allocated by make_attr_list function. */
free(first_list);

if (key_pressed == FKTMENU || key_pressed == FKTF1)
	return(D2_NOERROR);
else
	return(D2_ERROR);

}

/****************************************************************************/
/*                                                                          */
/*  NAME:  	make_schema_list()					    */
/*  AUTHOR:	Sanjay Jain						    */
/*  DATE:	09.06.92						    */
/*                                                                          */
/*  DESCRIPTION:                                                            */
/*	This function fills the 'summary' structure and 'summary_list'      */
/*	with key components of schema table entries.			    */
/*                                                                          */
/*  INPUT PARAMETER:                                                        */
/*	operation: 	DS__AT_DISPLAY, DS__OCT_DISPLAY or DS__SRT_DISPLAY  */
/*	schema_index_space:	to know already selected entries.	    */
/* 	no_selected_entries:	no. of selected entries.		    */
/*									    */
/*  OUTPUT PARAMETER:							    */
/*									    */
/*  RETURN VALUE:							    */
/*	error:		if malloc for summary_list fails or invalid 	    */
/*			operation.					    */
/*	no error:	otherwise.					    */
/*									    */
/*                                                                          */
/****************************************************************************/

static signed32 make_schema_list(
	signed32  operation,
	signed32 *schema_index_space,
	signed32  no_selected_entries)
{
signed32 i, j, entry_cnt;
signed32 abbrv_len;
char	 tmp_at_oct_str[L_ABBRV + L_OBJ_ID + L_MAX_NAME + 3];

/* fill summary and summary list information depending on input parameters. */

switch (operation)
	{
	case DS__AT_DISPLAY:
		entry_cnt = d2_nat;
		break;

	case DS__OCT_DISPLAY:
		entry_cnt = d2_noct;
		break;

	case DS__SRT_DISPLAY:
		entry_cnt = d2_nsrt;
		break;

	default:
		return(D2_ERROR);
	}

/* allocate space for entries.				*/
if ( (summary.summary_list = (struct summary_item *)malloc ((entry_cnt) * 
	sizeof(struct summary_item))) == 0)
	{
	d20_qomask[D20_Q1POS].d20_ioinfo = d20_err_table[D20_E04IDX];
  	d20_handlemask(d20_qomask); /* memory error */
	return(D2_ERROR);
	}

first_list = summary.summary_list;

switch (operation)
	{
	case DS__AT_DISPLAY:
		for (i=0; i<d2_nat; i++)
			{
			sprintf(tmp_at_oct_str, "%-*s %-*s %-*s", 
			L_ABBRV, (d2_p_at + i)->att_abbrv, 
			L_MAX_NAME, (d2_p_at + i)->att_name, 
			strlen((d2_p_at + i)->att_obj_id),
			(d2_p_at + i)->att_obj_id);

			strncpy((first_list + i)->summary_name, tmp_at_oct_str,
			MAX_T61_DISP_STR_LEN);
			(first_list + i)->summary_name[MAX_T61_DISP_STR_LEN] = 
									D2_EOS;

			(first_list + i)->selected = FALSE;
			}

		break;

	case DS__OCT_DISPLAY:
		for (i=0; i<d2_noct; i++)
			{	
			sprintf(tmp_at_oct_str, "%-*s %-*s %-*s",
			L_ABBRV, (d2_p_oct + i)->obj_abbrv, 
			L_MAX_NAME, (d2_p_oct + i)->obj_name, 
			strlen((d2_p_oct + i)->obj_id),
			(d2_p_oct + i)->obj_id);

			strncpy((first_list + i)->summary_name, tmp_at_oct_str,
			MAX_T61_DISP_STR_LEN);
			(first_list + i)->summary_name[MAX_T61_DISP_STR_LEN] = 
									D2_EOS;

			(first_list + i)->selected = FALSE;
			}
		break;

	case DS__SRT_DISPLAY:
		for (i=0; i<d2_nsrt; i++)
			{
			sprintf((first_list + i)->summary_name, "%-*d %-*d",
			4, (d2_p_srt + i)->rule_nr, 4, (d2_p_srt + i)->par_rule_nr);

			for (j=0; j<(d2_p_srt + i)->no_nam_attr; j++)
				{
				/* last naming attribute being handled.  */
				if (j == (d2_p_srt + i)->no_nam_attr - 1)
					/* take the actual length.	*/
					abbrv_len = 
					strlen((d2_p_srt + i)->nam_attr[j]);
				else
					/* take max. length.	*/
					abbrv_len = L_ABBRV;

				sprintf((first_list + i)->summary_name + 
				strlen((first_list + i)->summary_name), "%-*s", 
				abbrv_len, (d2_p_srt + i)->nam_attr[j]);
				}
			
			(first_list + i)->selected = FALSE;
			}
		break;

	}

/* mark already selected entries.	*/
for (i=0; i<no_selected_entries; i++)
	(first_list + *(schema_index_space + i))->selected = TRUE;

summary.current_page = 1;
summary.current_line = 1;
summary.num_selected = no_selected_entries;
summary.current_display = 0;

/* object_counter is a global varible, it's value has to be set.	*/
object_counter = entry_cnt;

summary.max_page = object_counter % SUMMARY_LINES_PER_PAGE;
if (!summary.max_page)
	summary.max_page = object_counter / SUMMARY_LINES_PER_PAGE;
else
     	summary.max_page = object_counter / SUMMARY_LINES_PER_PAGE + 1;

return(D2_NOERROR);

}
