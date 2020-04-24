/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20admin.c,v $
 * Revision 1.1.10.2  1996/02/18  19:40:28  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:21:27  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:16:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:44  root]
 * 
 * Revision 1.1.8.4  1994/06/21  14:43:30  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:23:05  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:49:26  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:10:17  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:05:10  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:08:16  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:58:15  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:27:15  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  09:56:04  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:26:34  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  17:49:08  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:03:42  bbelch]
 * 
 * Revision 1.1.4.3  1992/11/27  13:08:29  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  14:06:45  marrek]
 * 
 * Revision 1.1.4.2  1992/09/28  10:31:20  marrek
 * 	Remove || ((op == DS_ADD_ALIAS || op == DS_DELETE_OBJECT) && d20_cache)
 * 	for scheme_disp.
 * 	[1992/09/28  10:30:03  marrek]
 * 
 * Revision 1.1.2.3  1992/06/30  20:53:45  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:32:29  melman]
 * 
 * Revision 1.1.2.2  1992/06/01  21:52:13  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:14:56  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20admin.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:40:28 $";
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
/* NAME         : admin.c     [Administrations-Schnittstelle]         */
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
#include <string.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <gdsext.h>
#include <d2adm.h>
#include <d2msk.h>
#include <cget.h>
#include <d20proto.h>

/**********************************************************************/
/*								      */
/*			  D E F	I N E S				      */
/*								      */
/**********************************************************************/

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

 Bool ds_test_name;                   /* Boolean, if by         */
					    /* "special attributes"   */
					    /* the Leaf-Name should   */
					    /* check by the function  */
					    /* "tst_prop"             */

 signed32      d20_old_rule_nr;     /* rule number of structure that has  */
				    /* been selected before               */


extern char 	 *d20_4ifields[] ;	/* Input fields for mask 4     */
extern D20_omask d20_4omask[] ;		/* Mask 4 */
extern D20_omask d20_7aomask[];
extern D20_opmsg d20_opmsg[] ;		/* Operations-messages */
extern char 	 d20_actopmsg[] ;	/* Currrent operations message */
extern char     *d20_op[];
extern char     *d20_cop[];
extern char     *d20_disp[];
extern D20_omask d20_qomask[] ;		/* Mask 'quit' */
extern char	 *d20_errtab[] ;	/* Array for error messages     */
extern signed32  rbatch;
extern signed16  dsa_type_value;
extern Bool      d20_cache;
extern Bool      d20_shadow;            /* administrate shadows if TRUE */
extern D20_oct	*d2_p_d20oct;
extern D20_srt	*d2_p_d20srt;
extern signed32  d2_nd20srt;  		/* number of Srt elements       */

/**********************************************************************/
/*								      */
/*		  D E C L A R A T I O N S      FUNCTIONS	      */
/*								      */
/**********************************************************************/


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME:        d20_1_admin(parblk)                                   */
/*								      */
/* AUTHOR:      Schmid  (D AP 11)                                     */
/* DATE:        19.08.87                                              */
/*								      */
/* SYNTAX:      signed16 d20_1_admin(parblk)                          */
/*              D20_pE *parblk;                                       */
/*								      */
/* DESCRIPTION:                                                       */
/*              This function is responsible for the administration   */
/*              of the directory.                                     */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              Pds0100  *parblk;                                     */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*              -                                                     */
/*								      */
/* RETURNVALUE:							      */
/*								      */
/*       1      :       leave by 'break'                              */
/*       0      :       leave by 'exit'                               */
/*      -1      :       error --> check error values                  */
/*								      */
/* STANDARDKOPF:						      */
/*								      */
/* GLOBAL DATA (read only):                                           */
/*								      */
/* GLOBAL DATA (updated):                                             */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                      ds0_500_objects()                             */
/*                      ds0_501_object_name()                         */
/*                      ds0_502_get_attr()                            */
/*                      ds0_503_mod_attr()                            */
/*                      ds0_504_display_list()                        */
/*                      ds4_a04_get_operations()                      */
/*                      ds4_b11_call_directory()                      */
/*                      ds4_b21_allocate()                            */
/*                      ds4_b22_free()                                */
/*			d20_handlemask ()			      */
/*								      */
/* NOTES:                                                             */
/*								      */
/*exoff	***************************************************************/

signed16 d20_1_admin( D20_pE *parblk )
 
{                               /*  ds0_100_admin()                   */

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

   Ds_property prty;
   signed16 ret_value = D2_NOERROR;
   signed16 type;              /* object-type (OBJECT or ALIAS)      */
   D2_a_type attrx_type;
   signed16 idx;
   signed16 opmax;             /* maximum operation number           */

   Bool  init_flds;         /* sign indicating whether the mask   */
				/* fields must be initialized         */

   char  tree[DS_TREE_LENGTH];/* tree structure of selected object*/
   signed16 count = 0;

   Ds_at_entry  attr_entry[2];
   Bool  scheme_disp;       /* indicates whether scheme object    */
				/* should be displayed in object mask */
   signed16 op;
   signed16 op_sav;            /* used when display of local info was*/
				/* selected                           */
   D2_a_value *attr_val;
   D2_a_type   a_type;

    D20_omask        sav_7aomask[D20_M_SIZE];

   signed32 rule_nr;            /* rule number of selected structure   */
D20_oct		*oct_ent;	/*current object class entry */
D20_srt		*srt_ent;	/*current structure rule entry */

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

for ( ; ; )
  {                             /*  get input from operations-mask    */
    strcpy(d20_actopmsg,d20_op[DS__BASIC_ADM]);
    if (d20_cache == TRUE)
    {   
      d20_4omask[DS_C_DISP].d20_ioinfo = d20_disp[0];
      d20_4omask[DS_C_DISP+1].d20_ioinfo = d20_disp[2];
      d20_4omask[DS_C_DISP+2].d20_ioinfo = d20_disp[4];
      d20_4omask[DS_C_DISP+3].d20_ioinfo = d20_disp[6];
      d20_4omask[DS_C_DISP+4].d20_ioinfo = d20_disp[8];
      for (idx = DS_C_MINIDX; idx <= DS_C_MAXIDX; idx++)
      { 
	d20_4omask[idx].d20_fkt_id = D20_NO_OP;
      } 
      opmax = DS_OP_CACHEMAX;
      d20_4omask[DS_E1_TYPE].d20_fkt_id =
      d20_4omask[DS_E2_TYPE].d20_fkt_id = D20_NO_OP;
    }
else
{
      d20_4omask[DS_C_DISP].d20_ioinfo = d20_disp[1];
      d20_4omask[DS_C_DISP+1].d20_ioinfo = d20_disp[3];
      d20_4omask[DS_C_DISP+2].d20_ioinfo = d20_disp[5];
      d20_4omask[DS_C_DISP+3].d20_ioinfo = d20_disp[7];
      d20_4omask[DS_C_DISP+4].d20_ioinfo = d20_disp[9];

      for (idx = DS_C_MINIDX; idx <= DS_C_MAXIDX; idx++)
      { 
	d20_4omask[idx].d20_fkt_id = D20_CSAY;
      } 
      opmax = DS_OPMAX;
      d20_4omask[DS_E1_TYPE].d20_fkt_id = D20_CSAY;
      d20_4omask[DS_E2_TYPE].d20_fkt_id = D20_CGET;
    } 

    /* display mask */
    *d20_4ifields[DS_AF_BREAK] = DS_DEFAULT_VALUE ;
    d20_handlemask (d20_4omask);


    if                          /*  "break" was   selected            */
      ((*d20_4ifields[DS_AF_BREAK] != DS_DEFAULT_VALUE) || 
      (*(signed32 *)d20_4ifields[DS_AF_FUNCTION] == DS_EXIT))
    {                           /*  return(0)                         */
      return(D2_NOERROR);
    }
else
{                           /*  set service controls for copies   */
			/*  or real objects                   */
      d20_shadow = *(signed32 *) d20_4ifields[DS_AF_E_TYPE];
      d20_init();
				/*  check selected operation          */
      op = op_sav = (signed16) *(signed32 *)d20_4ifields[DS_AF_FUNCTION];

      if (op == DS_SPEC_DISP_OBJ && d20_cache == TRUE)
      {                         /* display local and default DSA */
	d20_550_disp_def_dsa();
	continue;
      }
else if (op == DS_DISP_OBJ)
      {   
	d20_168_set_serv_cntrls();
	op = DS_DISPLAY_OBJECTS;
      }
else if (op == DS_DISPLAY_OBJECTS && d20_cache == FALSE)
      {   
	d20_172_set_master_serv_cntrls();

      }
else if (op == DS_ADD_CLIENT_ADDRESS && d20_cache == TRUE)
/* ADD CLIENT ADDRESS */
      {   
	   strcpy (d20_actopmsg, d20_cop[0]);
			    /* 1. show mask for PSAP-Address */
           if
	        ((ret_value = ds0_520_handle_spec_attr(DS_ADD_OBJECT,&prty))
                					== D2_ERROR)
           {   
                d20_qomask[D20_Q1POS].d20_ioinfo =
                                      d20_errtab[D20_100IDX];
                d20_handlemask (d20_qomask);
		continue;
	   }
else if (ret_value == FKTDEL)
	   {   
		continue;
           } 
			    /* 2. move PSAP-Address (--> "prty") to */
			    /* other memory area that is used by */
			    /* "ds4_b02_fill_attr"               */
	   attr_entry[0].a_val.d2_a_v_len = prty.ds_length;
	   attr_entry[0].a_val.d2_at_value.d2_oct_a_v = 
						(Octet_string) prty.ds_value;
	   attr_entry[0].a_val.d2_a_rep = d27_401_syntax_to_rep(D2_PSAP_STX);
	   attr_entry[0].attr_id.d2_type = (Ob_id_string)D2_PSAP_ADDRESS;
	   attr_entry[0].attr_id.d2_typ_len = sizeof(D2_PSAP_ADDRESS) - 1;
	   attr_entry[0].hex_use = FALSE;
			    /* 3. "ds4_b02_fill_attr" moves attribute     */
			    /* value (PSAP-Address) to global memory   */
			    /* area that is used by "d20_spec_call_dir" */
	   ds4_b02_fill_attr(D20_ASTART | D20_AINSERT | D20_AEND,
			     &attr_entry[0],1);
			    /* 4. "d20_spec_call_dir" adds client object in */
			    /* the cache including the client PSAP-Address   */
           if
                ((ret_value = d20_spec_call_dir(parblk->d20_Eenv,
                              D20_4_DSA_TYP)) != D2_NOERROR)
           {   
                d20_qomask[D20_Q1POS].d20_ioinfo =
                                      d20_errtab[D20_100IDX];
                d20_handlemask (d20_qomask);
	   } 
			    /* ... and go back to cache menu. */
	   continue;

      }
else if (op == DS_DISP_CLIENT_ADDRESS && d20_cache == TRUE)
      {   
/* DISPLAY CLIENT ADDRESS */
	  strcpy (d20_actopmsg, d20_cop[1]);
	  d20_162_set_name(D2_N_CLIENT);
	  d20_set_subset (D2_BASE_OBJECT);

	  attrx_type.d2_type = (Ob_id_string)D2_PSAP_ADDRESS;
	  attrx_type.d2_typ_len = sizeof(D2_PSAP_ADDRESS) - 1;

	  d20_170_set_req_attr(1,&attrx_type);
	  if ((ret_value = ds4_b11_call_directory(DS_DISPLAY_OBJECTS)) ==
			   D2_NOERROR)
	  {   
	       if ((ret_value = (signed16) d20_get_attribute(D20_BEGIN,
						  &a_type, &attr_val))
				!= D2_NOERROR)
	       {   
		  d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab [D20_100IDX];
		  d20_handlemask (d20_qomask);
		  continue;
	       } 
	       prty.ds_length = attr_val->d2_a_v_len;
	       prty.ds_value = attr_val->d2_at_value.d2_oct_a_v;
	       d20_save_mask(d20_7aomask, sav_7aomask);
	       ds0_520_handle_spec_attr(DS_DISP_OBJ,&prty);
	       d20_restore_mask(d20_7aomask, sav_7aomask);
	  } 
	  continue;
      }
else if (op == DS_DELETE_DEFAULT_DSA && d20_cache == TRUE)
      {   
/* DELETE DEFAULT DSA */ 
	  strcpy (d20_actopmsg, d20_cop[2]);
				/*  display SRT tree                  */
	  if                    /*  input tree ok                     */
	     (!ds0_500_objects(FALSE, D20_OBJECT, &rule_nr, tree))
	  {                     /*  display object-mask               */
	    init_flds = (rule_nr == d20_old_rule_nr && rbatch == 0) ? FALSE
							      : TRUE;
	    d20_old_rule_nr = rule_nr;
	    count = 0;
				/* read DEFAULT-DSA from mask         */
	    if (ds0_501_object_name(FALSE, D20_OBJECT, 
				    D20_DELETE_DEFAULT_DSA, rule_nr,tree,
				    init_flds, &count, &oct_ent) == 
				   D2_NOERROR)
	    {                     /* try to delete DEFAULT-DSA          */
	      d20_delete_default_dsa();
	    } 
	  } 
          continue;
      } 

      if                        /*  operation doesn't exist           */
	 (op < DS_OPMIN || op > opmax)
      {                         /*  put message "unknown operation"   */
	d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab[D20_E00IDX] ;
	d20_handlemask (d20_qomask) ;
      }
else
{                         /*  determine operation               */
	type = d20_opmsg[op].d20_otype ;
	strcpy (d20_actopmsg, d20_opmsg[op].d20_msgptr) ;
  
	if                      /*  single object or alias is chosen  */
	  (type == D20_OBJECT || type == D20_ALIAS || type == D20_RDN)
	{                       /*  display tree structure of objects */
	  scheme_disp = (op == DS_DISPLAY_OBJECTS || op == DS_MODIFY_ATTRIBUTE 
	  || ((op == DS_ADD_ALIAS || op == DS_DELETE_OBJECT) && d20_cache)) ? 
							TRUE : FALSE;
	  if                    /*  input tree ok                     */
	    (!ds0_500_objects(scheme_disp, type, &rule_nr, tree))
	  {                     /*  display object-mask               */
	    init_flds = (rule_nr == d20_old_rule_nr && rbatch == 0) ? FALSE
							      : TRUE;
	    d20_old_rule_nr = rule_nr;
	    srt_ent = d20_search_adm_srule(d2_nd20srt, d2_p_d20srt, rule_nr);
	    count = 0;
	    if                  /*  ADD_OBJECT without attributes selected */
	      (ds0_501_object_name(scheme_disp, type, op, rule_nr,
			tree, init_flds, &count, &oct_ent) == D2_NOERROR)
	    {                   /*  check operation                   */
	      switch((int) op)
	      {
		case DS_DELETE_OBJECT:
		  ds4_b11_call_directory(op);
		  break;
		case DS_DISPLAY_OBJECTS:
		  d20_166_set_all_attr();
		  if
		    (!ds4_b11_call_directory(op))
		  {   
				/*  display list of objects           */
		    ds0_504_display_list(tree);
		  } 
				/*  reset filter info                 */
		  d20_ffilter(D20_BEGIN, (Ds_at_entry *)NULL, 0);
		  break;
#ifndef CACHE_ADMIN
		case DS_MODIFY_ATTRIBUTE :
		  if            /*  input of attributes ok            */
		    (!ds0_503_mod_attr(srt_ent,oct_ent, attr_entry))
		  {             /*  call directory-function           */
		    ds4_b11_call_directory(op);
		  }             
		  break;
#endif
		case DS_ADD_ATTRIBUTE:
		  if            /*  input of attributes ok            */
		    (!ds0_502_get_attr(op,rule_nr,oct_ent,&count, init_flds))
		  {             /*  check whether all attributes are  */
				/*  allowed                           */
		    ds4_b11_call_directory(op);
		  }             
		  break;
		case DS_ADD_ALIAS:
		case DS_MODRDN:
				/*  call directory function           */
		  ds4_b11_call_directory(op);
		  break;
		default:
		  if            /*  input of attributes ok            */
		    (!ds0_502_get_attr(op,rule_nr,oct_ent,&count, init_flds))
		  {             /*  call directory-function           */
		    if          /* DSA-Type not = client              */
		      (dsa_type_value != D20_4_DSA_TYP)
		    {   
		      ds4_b11_call_directory(op);
		    } 
		    if        /*  DSA-Type is given                 */
		      (dsa_type_value != D20_0_DSA_TYP)
		    {           /*  fill info structure for add object*/
		      if
			((ret_value =
			 d20_spec_call_dir(parblk->d20_Eenv, dsa_type_value))
			 != D2_NOERROR)
		      {         /* give error message               */
			if (ret_value == D20_NO_PSAP)
			{   
			 d20_qomask[D20_Q1POS].d20_ioinfo =
					       d20_errtab[D20_100IDX] ;
			 d20_handlemask (d20_qomask) ;
			}
else
{
			 d20_qomask[D20_Q1POS].d20_ioinfo =
					       d20_errtab[D20_E95IDX] ;
			 d20_handlemask (d20_qomask) ;
			} 
		      } 
		    } 
		  }             
		  break;
	      }   
	    }                   
	  }                     
	}
else
{                       /*  exit was selected                 */
	  return(D2_NOERROR);
	}                       
      }                         
      if (op_sav == DS_DISP_OBJ && d20_cache != TRUE)
      {   
	d20_169_reset_serv_cntrls();
      } 
    }                           
  }                             
}                               /*  ds0_100_admin                     */
