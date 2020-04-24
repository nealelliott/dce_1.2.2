/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: d20atcmp.c,v $
 * Revision 1.1.4.2  1996/02/18  19:41:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:22:17  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:18:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:52:20  root]
 * 
 * Revision 1.1.2.3  1994/09/06  12:24:34  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:12:15  keutel]
 * 
 * Revision 1.1.2.2  1994/06/09  18:44:35  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:17:18  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:14:18  marrek
 * 	Bug fixes.
 * 	[1994/04/29  09:09:39  marrek]
 * 
 * 	Created for April Submission.
 * 	[1994/04/26  12:49:17  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20atcmp.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 19:41:34 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODULE                                              */
/*                                                                    */
/* NAME         : d22atcmp.c                                          */
/*                                                                    */
/* AUTHOR       : G. Rustemeyer, D AP 11                              */
/* DATE         : 09.07.91                                            */
/*                                                                    */
/* COMPONENT    : D2                                                  */
/*                                                                    */
/* DOC.-NR.     : D2-Design-Spec.                                     */
/*                      .                                             */
/*                      .                                             */
/*                                                                    */
/* PRD#/VERS.   : <Productnr. from Productstructure - <Versionnr.>    */
/*                                                                    */
/* DESCRIPTION  : The module compares two given attributes            */
/*                                                                    */
/* SYSTEM DEPENDENCIES: COMMON                                        */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       | ID | CR# EM# */
/*      1.0 | 09.07.91| Birth                          | gr |         */
/*datoff **************************************************************/


#include <gds.h>
#include <stdio.h>
#include <string.h>
#include <dce/d2dir.h>
#include <d23apdu.h>
#include <d23ipc.h>
#include <d22cache.h>

/* --------------------- PROTOTYPING OF LOCAL FUNCTIONS ------------------ */

Bool d22_002_crit_comp (D2_criteria *, D2_criteria *) ;


/*exon *********************************************************************/
/*                                                                         */
/*  FUNCTION:  d22_001_attr_comp ()                                        */
/*                                                                         */
/*  SHORT DESCRIPTION:                                                     */
/*      The function switches the representation-syntax of a given         */
/*      structure D2_a_value and compares the corresponding strings rsp.   */
/*      structures of the two input-structures.                            */
/*                                                                         */
/*  INPUT-PARAMETER:                                                       */
/*      p1          = pointer to first structure D2_a_value                */
/*      p2          = pointer to second structure D2_a_value               */
/*                                                                         */
/*  OUTPUT-PARAMETER:                                                      */
/*                    None                                                 */
/*                                                                         */
/*  RETURN-VALUE:                                                          */
/*      TRUE     = The two strings resp. structures are equal.          */
/*      FALSE    = The two strings resp. structures are not equal.      */
/*                                                                         */
/* ----------------------------------------------------------------------- */
/*  AUTHOR: G. Rustemeyer                                 DATE: 03.07.91   */
/*exoff ********************************************************************/

boolean d22_001_attr_comp (D2_a_value *p1, D2_a_value *p2) {

    signed16		i, nb, size;
    unsigned16		rep=p1->d2_a_rep;
    D2_str		*D2_str_p1, *D2_str_p2;
    D2_guide		*D2_guide_p1, *D2_guide_p2;
    D2_T61_seq		*D2_T61_seq_p1, *D2_T61_seq_p2;
    D2_ttxid		*D2_ttxid_p1, *D2_ttxid_p2;
    D2_telex_number	*D2_telex_number_p1, *D2_telex_number_p2;
    D2_faxnumber	*D2_faxnumber_p1, *D2_faxnumber_p2;
    signed32		*intptr1, *intptr2;

   switch (rep)
      {
	 /*----------------------------------------------------------------*/
	 /* case Distname                                                  */
	 /*----------------------------------------------------------------*/
	 case D2_DISTNAME:
	    return (d22_cignore_comp ((char *) p1->d2_at_value.d2_oct_a_v,
				      (char *) p2->d2_at_value.d2_oct_a_v) );
	 /*----------------------------------------------------------------*/
	 /* case Search-guide                                              */
	 /*----------------------------------------------------------------*/
	 case D2_GUIDE:
	     D2_guide_p1 = p1 -> d2_at_value.d2_guide_a_v;
	     D2_guide_p2 = p2 -> d2_at_value.d2_guide_a_v;
	     if ( D2_guide_p1->d2_obj_defined !=
					     D2_guide_p2->d2_obj_defined  ||
		  D2_guide_p1->d2_obj.d2_typ_len !=
					  D2_guide_p2->d2_obj.d2_typ_len  ||
		  !memcmp ((void *) D2_guide_p1->d2_obj.d2_type,
			   (void *) D2_guide_p2->d2_obj.d2_type,
			   (size_t) D2_guide_p1->d2_obj.d2_typ_len ))
		return (FALSE);
	     else
		return ( d22_002_crit_comp ( &(D2_guide_p1->d2_crit),
					       &(D2_guide_p2->d2_crit) ) );

	 /*----------------------------------------------------------------*/
	 /* case T61-sequence                                              */
	 /*----------------------------------------------------------------*/
	 case D2_T61_LIST:
	    D2_T61_seq_p1 = p1 -> d2_at_value.d2_t61lst_a_v;
	    D2_T61_seq_p2 = p2 -> d2_at_value.d2_t61lst_a_v;
	    if ((nb=D2_T61_seq_p1->d2_nb_item) != D2_T61_seq_p2->d2_nb_item)
	       return (FALSE);
	    else
	      {
		D2_str_p1 = D2_T61_seq_p1 -> d2_t61_item;
		D2_str_p2 = D2_T61_seq_p2 -> d2_t61_item;
		for (i=0; i<nb; i++)
		  {
		     if ((size=D2_str_p1->d2_size) != D2_str_p2->d2_size ||
			 !memcmp ((void *) D2_str_p1->d2_value,
				   (void *) D2_str_p2->d2_value, (size_t) size))
			return (FALSE);
		     D2_str_p1 ++;
		     D2_str_p2 ++;
		  }
	      }
	    break;
	 /*----------------------------------------------------------------*/
	 /* case TTx-Id                                                    */
	 /*----------------------------------------------------------------*/
	 case D2_TTXID_STX:
	    D2_ttxid_p1 = p1 -> d2_at_value.d2_ttxid_a_v;
	    D2_ttxid_p2 = p2 -> d2_at_value.d2_ttxid_a_v;
	    if  ((size=D2_ttxid_p1->d2_terminal.d2_size) !=
				D2_ttxid_p2->d2_terminal.d2_size  ||
		!memcmp ((void *) D2_ttxid_p1->d2_terminal.d2_value,
		   (void *) D2_ttxid_p2->d2_terminal.d2_value, (size_t) size) ||
		 (size=D2_ttxid_p1->d2_controls.d2_size) !=
				D2_ttxid_p2->d2_controls.d2_size  ||
		!memcmp ((void *) D2_ttxid_p1->d2_controls.d2_value,
		   (void *) D2_ttxid_p2->d2_controls.d2_value, (size_t) size) ||
		 (size=D2_ttxid_p1->d2_graphics.d2_size) !=
				D2_ttxid_p2->d2_graphics.d2_size  ||
		!memcmp ((void *) D2_ttxid_p1->d2_graphics.d2_value,
		   (void *) D2_ttxid_p2->d2_graphics.d2_value, (size_t) size) ||
		 (size=D2_ttxid_p1->d2_miscel.d2_size) !=
				D2_ttxid_p2->d2_miscel.d2_size  ||
		!memcmp ((void *) D2_ttxid_p1->d2_miscel.d2_value,
		   (void *) D2_ttxid_p2->d2_miscel.d2_value, (size_t) size) ||
		 (size=D2_ttxid_p1->d2_pages.d2_size) !=
				D2_ttxid_p2->d2_pages.d2_size  ||
		!memcmp ((void *) D2_ttxid_p1->d2_pages.d2_value,
		   (void *) D2_ttxid_p2->d2_pages.d2_value, (size_t) size) ||
		 (size=D2_ttxid_p1->d2_privates.d2_size) !=
				D2_ttxid_p2->d2_privates.d2_size  ||
		!memcmp ((void *) D2_ttxid_p1->d2_privates.d2_value,
		   (void *) D2_ttxid_p2->d2_privates.d2_value, (size_t) size))
	       return (FALSE);
	    break;
	 /*----------------------------------------------------------------*/
	 /* case Telex-number                                              */
	 /*----------------------------------------------------------------*/
	 case D2_TLXNR_STX:
	    D2_telex_number_p1 = p1 -> d2_at_value.d2_tlxnr_a_v;
	    D2_telex_number_p2 = p2 -> d2_at_value.d2_tlxnr_a_v;
	    if  ((size=D2_telex_number_p1->d2_t_number.d2_size) !=
				D2_telex_number_p2->d2_t_number.d2_size  ||
		!memcmp ((void *) D2_telex_number_p1->d2_t_number.d2_value,
		    	 (void *) D2_telex_number_p2->d2_t_number.d2_value,
							       (size_t) size) ||
		 (size=D2_telex_number_p1->d2_c_code.d2_size) !=
				D2_telex_number_p2->d2_c_code.d2_size  ||
		!memcmp ((void *) D2_telex_number_p1->d2_c_code.d2_value,
			 (void *) D2_telex_number_p2->d2_c_code.d2_value,
							       (size_t) size) ||
		 (size=D2_telex_number_p1->d2_answer.d2_size) !=
				D2_telex_number_p2->d2_answer.d2_size  ||
		!memcmp ((void *) D2_telex_number_p1->d2_answer.d2_value,
			 (void *) D2_telex_number_p2->d2_answer.d2_value,
								 (size_t) size))
	       return (FALSE);
	    break;
	 /*----------------------------------------------------------------*/
	 /* case faxnumber                                                 */
	 /*----------------------------------------------------------------*/
	 case D2_FAXNR_STX:
	    D2_faxnumber_p1 = p1 -> d2_at_value.d2_faxnr_a_v;
	    D2_faxnumber_p2 = p2 -> d2_at_value.d2_faxnr_a_v;
	    if  ((size=D2_faxnumber_p1->d2_tel_nr.d2_size) !=
				D2_faxnumber_p2->d2_tel_nr.d2_size  ||
		   !memcmp ((void *) D2_faxnumber_p1->d2_tel_nr.d2_value,
			    (void *) D2_faxnumber_p2->d2_tel_nr.d2_value,
							       (size_t) size) ||
		   D2_faxnumber_p1->d2_par_defined !=
				       D2_faxnumber_p2->d2_par_defined  ||
		   D2_faxnumber_p1->d2_par != D2_faxnumber_p2->d2_par      )
	       return (FALSE);
	    break;
	 /*----------------------------------------------------------------*/
	 /* case Boolean                                                   */
	 /*----------------------------------------------------------------*/
	 case D2_BOOLEAN:
	    if (p1->d2_at_value.d2_bool_a_v != p2->d2_at_value.d2_bool_a_v)
	       return (FALSE);
	    break;
	 /*----------------------------------------------------------------*/
	 /* case Integer                                                   */
	 /*----------------------------------------------------------------*/
	 case D2_INTEGER:
	    if (p1->d2_at_value.d2_int_a_v != p2->d2_at_value.d2_int_a_v)
	       return (FALSE);
	    break;
	 /*----------------------------------------------------------------*/
	 /* case Integer-list                                              */
	 /*----------------------------------------------------------------*/
	 case D2_INT_LIST:
	    intptr1 = p1->d2_at_value.d2_intlist_a_v;
	    intptr2 = p2->d2_at_value.d2_intlist_a_v;
	    for (i=0; i<(short)p1->d2_a_v_len; i+=sizeof(signed32))
	      {
		 if (*intptr1 != *intptr2)
		    return (FALSE);
		 intptr1 ++;
		 intptr2 ++;
	      }
	    break;
	 /*----------------------------------------------------------------*/
	 /* case Octet-String                                              */
	 /*----------------------------------------------------------------*/
	 case D2_T61:
	 case D2_OCTET:
	 case D2_OBJ_IDENT:
	 case D2_PSAP_STX:
	 case D2_ASN1:
	    if ( p1->d2_a_v_len != p2->d2_a_v_len             ||
		 memcmp ((void *) p1->d2_at_value.d2_oct_a_v,
			 (void *) p2->d2_at_value.d2_oct_a_v,
			  (size_t) p1->d2_a_v_len)                         )
		return (FALSE);
	    break;
	 /*----------------------------------------------------------------*/
	 /* case String                                                    */
	 /*----------------------------------------------------------------*/
	 case D2_PRINTABLE:
	 case D2_NUMERIC:
	 case D2_TIME:
	 default:
	    if ( p1->d2_a_v_len != p2->d2_a_v_len             ||
		 memcmp ( p1->d2_at_value.d2_prt_a_v,
			  p2->d2_at_value.d2_prt_a_v,
			  p1->d2_a_v_len)                         )
		return (FALSE);
	    break;
      }

   return (TRUE);

}


/*exon *********************************************************************/
/*                                                                         */
/*  FUNCTION:  d22_002_crit_comp ()                                        */
/*                                                                         */
/*  SHORT DESCRIPTION:                                                     */
/*      The function compares the contents of two given structures         */
/*      D2_criteria.                                                       */
/*                                                                         */
/*  INPUT-PARAMETER:                                                       */
/*      p1          = pointer to first structure D2_criteria               */
/*      p2          = pointer to second structure D2_criteria              */
/*                                                                         */
/*                                                                         */
/*  RETURN-VALUE:                                                          */
/*      TRUE     = The two  structures are equal.                       */
/*      FALSE    = The two  structures are not equal.                   */
/*                                                                         */
/* ----------------------------------------------------------------------- */
/*  AUTHOR: G. Rustemeyer                                 DATE: 09.07.91   */
/*exoff ********************************************************************/

Bool d22_002_crit_comp (D2_criteria *p1, D2_criteria *p2) {

   signed32   i, nb;

   if (p1->d2_c_mode != p2->d2_c_mode)
      return (FALSE);
   else
       switch (p1->d2_c_mode)
	  {
	     case D2_ITEM:
		if ( p1->d2_crit_val.type->d2_ci_mode !=
		     p2->d2_crit_val.type->d2_ci_mode   ||
		     p1->d2_crit_val.type->d2_ci_type.d2_typ_len !=
		     p2->d2_crit_val.type->d2_ci_type.d2_typ_len  ||
		     !memcmp ((void *) p1->d2_crit_val.type->d2_ci_type.d2_type,
			      (void *) p2->d2_crit_val.type->d2_ci_type.d2_type,
			  (size_t) p1->d2_crit_val.type->d2_ci_type.d2_typ_len))
		       return (FALSE);
		break;
	     case D2_AND:
	     case D2_OR:
		if ((nb=p1->d2_crit_val.d2_ci_and_or->and_or_nr) !=
			p2->d2_crit_val.d2_ci_and_or->and_or_nr         )
		  return (FALSE);
		else
		  for (i=0; i<nb; i++)
		     {
			if ( d22_002_crit_comp (
				 p1->d2_crit_val.d2_ci_and_or->and_or,
				 p2->d2_crit_val.d2_ci_and_or->and_or )
							     == FALSE )
			  return (FALSE);
		     }
		break;
	     case D2_NOT:
		return ( d22_002_crit_comp (p1->d2_crit_val.not,
					      p2->d2_crit_val.not ) );
	     default:
		break;
	  }
   return (TRUE);
}

