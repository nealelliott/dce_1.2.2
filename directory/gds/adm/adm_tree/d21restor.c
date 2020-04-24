/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21restor.c,v $
 * Revision 1.1.10.2  1996/02/18  19:40:08  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:21:12  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:15:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:29  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:05:01  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:07:49  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:58:06  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:26:03  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  09:48:41  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:24:58  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  17:48:50  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:03:16  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/18  08:26:56  marrek
 * 	Various bug fixes for mask handling found at SNI
 * 	[1992/12/17  16:26:26  marrek]
 * 
 * Revision 1.1.4.2  1992/11/27  14:24:51  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  15:37:52  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:50:02  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:14:11  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21restor.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:40:08 $";
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
/* NAME         : d20restore.o   [restoring of subtrees]              */
/*								      */
/* AUTHOR       : N. Serra, D 525 (CDS)                               */
/* DATE         : 19.07.89                                            */
/*								      */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOK.-NR.	: DS-Design-Spez.				      */
/*			.					      */
/*			.					      */
/*								      */
/* PRD#/VERS.   :                                                     */
/*								      */
/* DESCRIPTION  :                                                     */
/*								      */
/* SYSTEM-DEPENDENCIES:                                               */
/*                      COMMON                                        */
/*								      */
/* HISTORY      :                                                     */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*      0.1 | 19.07.89| Original                       | ws |         */
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
#include <gdsext.h>
#include <dce/d27util.h>
#include <d2dump.h>
#include <d21.h>
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

extern     D2_pbhead    *d20_head;

extern     char         *d20_beg1;	/* start address of DN-strings*/
extern     char      	*d20_beg2;      /* start address of DN-strings*/

/**********************************************************************/
/*								      */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*								      */
/**********************************************************************/


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d21t000_restore_appnd()                               */
/*								      */
/* AUTHOR:      Serra (D AP 11)                                       */
/* DATE:        19.07.89                                              */
/*								      */
/* SYNTAX:      signed16 d21t000_restore_appnd(fd1, pdn1, dn1, dn_no1,*/
/*                    name1, attr1_len, file2,     */
/*                    inherit, overwrite, old_objects, parent_obj,    */
/*                    subtree, bind_id)                               */
/*              int           fd1;                                    */
/*              D20_offset  **pdn1;                                   */
/*              D20_offset  **dn1;                                    */
/*              signed32      dn_no1;                                 */
/*              char         *name1;                                  */
/*              signed32      attr1_len;                              */
/*              String        file2;                                  */
/*              signed32      inherit;                                */
/*              Bool          overwrite;                              */
/*              Name_string **old_objects;                            */
/*              Name_string   parent_obj;                             */
/*              Name_string   subtree;                                */
/*              signed16      bind_id;                                */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function handles the information of the saved    */
/*              subtree and the one of the target DSA. It checks      */
/*              both internally information and the appropiate update */
/*              function will be chosen.                              */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*              int           fd1;      file descriptor of file1      */
/*              D20_offset  **pdn1;     pointer array of DN offsets   */
/*              D20_offset  **dn1;      offsets of DN (of DSA1)       */
/*              signed32      dn_no1;   number of DNs found in DSA1   */
/*              char        **name1;    pointer to memory area for DNs*/
/*                                      of DSA1                       */
/*              signed32      attr1_len;   maximum length of          */
/*                                         attribute list for one DN  */
/*              String        file2;    "search file" of DSA2         */
/*              signed32      inherit;  New or old ACL                */
/*              Bool          overwrite;                              */
/*              Name_string   parent_obj;  Parent object in the DSA   */
/*              Name_string   subtree;     Parent obj. of the subtree */
/*              signed16      bind_id;     Bind identifier            */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              Name_string **old_objects;    Name collisions         */
/*								      */
/* RETURNVALUE:							      */
/*								      */
/*      D2_NOERROR:  No error occurred                                */
/*      D2_ERROR:    Error occurred                                   */
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

 signed16 d21t000_restore_appnd(
   int           fd1,
   D20_offset  **pdn1,
   D20_offset  **dn1,
   signed32      dn_no1,
   char         *name1,
   signed32      attr1_len,
   String        file2,
   signed32      inherit,
   Bool          overwrite,
   signed32      new_parent,
   Name_string **old_objects,
   Name_string   parent_obj,
   Name_string   subtree,
   signed16      bind_id)

{                               /*  d21t000_restore_appnd()           */

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

int          fd2;               /* file descriptor of file2           */
D20_offset **pdn2;
D20_offset  *dn2;               /* offsets of DN (of DSA2) relative   */
				/* to beginning of file or memory     */
				/* area                               */
signed32     dn_no2;            /* number of DNs found in DSA2        */

signed32     act_dn_no1;        /* actual number of DNs for name      */
				/* collision generation               */
				/* (handled until an error oocurs)    */

char        *name2;             /* pointer to memory area for DNs     */
signed32     l_name2;           /* length of all DNs of DSA2          */
signed32     attr2_len;         /* max. length of attribute list for  */
				/* DNs of DSA2                        */
char        *at1_mem = NULL;          /* memory area for attribute */
					 /* list of DSA1              */
					 /* and for DN                */
char        *at2_mem = NULL;          /* memory area for attribute */
					 /* list of DSA2              */
					 /* and for DN                */
signed32     ret_value = D2_NOERROR;
D20_offset **npdn;
Bool        *name_col = NULL;/* array of boolean indicating whether */
				/* name collisions                     */
Name_string  p_parent;
signed32     c1;
signed32     c2;
char        *acl_attr = (char *) NULL;
char        *sav_acl;
D2_a_value  *sch_acl;      /* ACL of scheme if   */
		    	   /*  parent is ROOT    */
D2_a_type    acl_type; 

signed32     min_pos = 0;
signed32     max_pos = 0;
Bool         found;
signed16     acl_len;
Name_string  bind_dsa;
struct d21_bind_info  bind_info;
d2_ret_val   rc;
char		*at_val_area;	/* pointer to memory area to store attribute */
			        /* values. (added because of complex attrs.) */
Bool		dummy;
D2_pbhead	dummy_pbhead;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

acl_type.d2_typ_len = sizeof(D2_ACL)-1;
acl_type.d2_type = (Ob_id_string) D2_ACL;

				/*  generate DN-list for DSA2         */
   if                           /*  operation fails                   */
      (d20_105_gen_dn(TRUE, file2, &fd2, &name2, &l_name2,(D20_offset **) &pdn2,
		      &dn2, &dn_no2, &attr2_len))
   {                            /*  return(D2_ERROR)                  */
      d20_111_free((char *) pdn1, (char *) dn1, name1, NULL, NULL, NULL, NULL,
		   NULL, NULL, NULL);
      return(D2_ERROR);
   }
   else {                       
	/* allocate memory for attribute values.	*/
	if ((at_val_area = malloc(attr1_len + attr2_len + 
				 (D2_AT_MAX * max_comp_attr() * 2))) == NULL)
		{
            	d20_head->d2_errvalue = D20_ALLOC_ERR;
            	ret_value = D2_ERROR;
         	}

      /* allocate memory for attribute lists*/
      if (attr1_len) {
         attr1_len += D2_ALIGN(D2_DNL_MAX);
         if (attr1_len < D20_BUFSIZ) {
            attr1_len = D20_BUFSIZ;
         }
         if ((at1_mem = malloc(attr1_len)) == NULL) {
            d20_head->d2_errvalue = D20_ALLOC_ERR;
            ret_value = D2_ERROR;
         }
      }
      if (attr2_len && !ret_value) {
         attr2_len += D2_ALIGN(D2_DNL_MAX);
         if (attr2_len < D20_BUFSIZ) {
            attr2_len = D20_BUFSIZ;
         }
         if ((at2_mem = malloc(attr2_len)) == NULL) {
            d20_head->d2_errvalue = D20_ALLOC_ERR;
            ret_value = D2_ERROR;
         }
      }
   }                          
   if (!ret_value) {                /*  no errors occurred                */
      d20_beg1 = name1;
      d20_beg2 = name2;
      if ((d21_bh8_get_bind_info(bind_id, &bind_info, &dummy, &dummy_pbhead,
	   FALSE) == D2_ERROR) && !new_parent) {
	 d20_111_free((char *) pdn1, (char *) dn1, name1, (char *) pdn2,
		      (char *) dn2, name2, NULL, NULL, NULL, NULL);
	 return(D2_ERROR);
      }
      else {
         bind_dsa =  bind_info.d21_b_name_dsa;
      }

      if (*subtree == D2_EOS)
      { for (c1 = 0, npdn = pdn1; c1 < dn_no1; c1++, npdn++)
	{   (*npdn)->d20_state |= D21_REAL_OBJ;
	}
      }
      else
      { for (c1 = 0, npdn = pdn1; c1 < dn_no1; c1++, npdn++) {
	    (*npdn)->d20_state |= d21t007_set_status_flag(
			   d20_beg1+(*npdn)->d20_mem_off,(char *) subtree);
	}
      }

      if (inherit == NEW_ACL) {    /* read ACL from the parent object */
         min_pos = max_pos = 0;
	 if (*parent_obj == D2_EOS)
				    /* take ACL of "scheme"           */
	 {  p_parent = (Name_string)D2_NAME_SCHEME;
	 }
	 else
	 {  p_parent = parent_obj;
	 }
         for (found = FALSE, c1 = 0, npdn = pdn2; c1 < dn_no2 && !found; 
	      c1++, npdn++) {
	    if (!d21t005_compare_dn_appnd((char *) p_parent, npdn, d20_beg2)) {
               if (d21t008_get_attr(fd2, *npdn, at2_mem, &min_pos, &max_pos,
			 acl_type, &acl_attr, &acl_len, d20_beg2) == D2_ERROR) {
		  d20_111_free((char *) pdn1, (char *) dn1, name1,(char *) pdn2,
			       (char *) dn2, name2, NULL, NULL, NULL, NULL);
		  d20_head->d2_errvalue = D20_SEARCH_ERR;
		  return(D2_ERROR);  /* D2_ACL attribute not found */
               }
	       found = TRUE;
				     /* store ACL attribute              */
	       sav_acl = acl_attr;
	       if ((acl_attr = malloc((size_t) acl_len)) != (char *) 0)
	       {   for (c1 = 0; c1 < acl_len; c1++)
		   {   *(acl_attr + c1) = *(sav_acl + c1);
		   }
	       }
            }
         }
	 if (!found) {              /* ACL attribute was not found */
	    if (*parent_obj != D2_EOS)
	    {                       /* real object was searched --> ACL */
				    /* should have been found           */
		d20_111_free((char *) pdn1, (char *) dn1, name1, (char *) pdn2,
			     (char *) dn2, name2, NULL, NULL, NULL, NULL);
		d20_head->d2_errvalue = D20_SEARCH_ERR;
		return(D2_ERROR);   /* ACL of parent_obj not found      */
	    }
	    else
	    {                       /* read ACL of "scheme"             */
		d20_161_reset_info();
		d20_162_set_name(D2_NAME_SCHEME);
		d20_set_subset(D2_BASE_OBJECT);
		d20_170_set_req_attr(1, &acl_type);
		d20_168_set_serv_cntrls();
		if ((rc = d20_call_iapl(D20_0_SEARCH)) == D2_NOERROR)
		{                   /* get ACL attribute                */
		    if ((rc = d20_get_attribute(D20__BEGIN, &acl_type,&sch_acl))
						== D2_NOERROR)
		    {               /* store ACL attribute              */
			acl_len = sch_acl->d2_a_v_len;
			if ((acl_attr = malloc((size_t) acl_len)) !=
					(char *) 0)
			{   for (c1 = 0; c1 < acl_len; c1++)
			    {   *(acl_attr + c1) = 
				*(sch_acl->d2_at_value.d2_oct_a_v + c1);
			    }
			}
		    }
		}
		d20_169_reset_serv_cntrls();
		if (rc != D2_NOERROR)
		{   d20_111_free((char *) pdn1,(char *) dn1,name1,(char *) pdn2,
				 (char *) dn2, name2, NULL, NULL, NULL, NULL);
		    if (acl_attr)
		    {   free(acl_attr);
		    }
		    d20_head->d2_errvalue = D20_SEARCH_ERR;
		    return(D2_ERROR);  /* ACL of parent_obj not found      */
		}
	    }
	 }
      }

      act_dn_no1 = dn_no1;

      if ((name_col = (Bool *) malloc((size_t) (dn_no1) *
				      sizeof(Bool))) == (Bool *) NULL)
      {  ret_value = D2_ERROR;
	 d20_head->d2_errvalue = D20_ALLOC_ERR;
      }
      else
      {
	 for (c2 = 0;c2<dn_no1;c2++)
	     *(name_col+c2) = TRUE;   /* Initialize with TRUE */

				   /*  handling of DNs of both DSAs ok   */
	 ret_value = d21t001_hdldns_appnd(fd1, pdn1, &act_dn_no1, at1_mem,
	                      fd2, pdn2, dn_no2, at2_mem, inherit,
			      overwrite, acl_attr, acl_len, new_parent,
			      parent_obj, subtree, bind_dsa, name_col, 
			      at_val_area);


				/* generate list of name collisions   */
	 if (d21t002_old_objects(act_dn_no1, pdn1,
				 old_objects, name_col,
				 subtree, parent_obj) == D2_ERROR)
	 {     ret_value = D2_ERROR;
	 }
      }
   }

   d20_111_free((char *) pdn1, (char *) dn1, name1, (char *) pdn2, (char *) dn2,
		name2, NULL,NULL, NULL,NULL);

   if (at_val_area)
     free(at_val_area);
   if (at1_mem) {
     free(at1_mem);
   }
   if (at2_mem) {
     free(at2_mem);
   }
   if (name_col)
   {    free(name_col);
   }
   if (acl_attr)
   {    free(acl_attr);
   }
   close(fd1);
   close(fd2);
                                /*  return(ret_value)                 */
   return(ret_value);
}                               /*  d21t000_restore_appnd()           */

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : max_comp_attr()                                       */
/*								      */
/* AUTHOR:      Sanjay(D AP 11)                                       */
/* DATE:        15.04.91                                              */
/*								      */
/* SYNTAX:      signed32 max_comp_attr()			      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function resturns the max. of sizes of all the   */
/*		complex attributes.				      */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*		This function resturns the max. of sizes of all the   */
/*		complex attributes.				      */
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

 signed32 max_comp_attr(void)
{
signed32	max_val = 0;

max_val = sizeof(D2_guide) > sizeof(D2_ttxid) ? sizeof(D2_guide) : sizeof(D2_ttxid);
max_val = max_val > sizeof(D2_telex_number) ? max_val : sizeof(D2_telex_number);
max_val = max_val > sizeof(D2_faxnumber) ? max_val : sizeof(D2_faxnumber);
max_val = max_val > sizeof(D2_T61_seq) ? max_val : sizeof(D2_T61_seq);

return(max_val);
}

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d21t006_new_parent()                                  */
/*								      */
/* AUTHOR:      Serra (D AP 11)                                       */
/* DATE:        19.07.89                                              */
/*								      */
/* SYNTAX:      signed32 d21t006_new_parent(parent_obj, subtree)      */
/*              char  *parent_obj;                                    */
/*              char  *subtree;                                       */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function checks if the parent object of the      */
/*              subtree to be appended is the same that it had when   */
/*              it was saved or a new one.                            */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*              char  *parent_obj;   DN of the object under witch     */
/*                                      the subtree will be appended  */
/*              char  *subtree;         Parent object of the saved    */
/*                                      subtree                       */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*								      */
/*      D21_OLD_PARENT :  The new parent of the subtree is the same   */
/*                        that the old one                            */
/*      D21_NEW_PARENT :  The new parent of the subtree is different  */
/*                        to the old one                              */
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

 signed32 d21t006_new_parent(
   char *parent_obj,
   char *subtree)
{
   char *ini_p;

   ini_p = parent_obj;
   for ( ; *parent_obj == *subtree; parent_obj++, subtree++) 
      if (*parent_obj == D2_EOS)
         return(D21_NEW_PARENT);

   if ((*parent_obj == D2_EOS && *subtree == D2_RDN_SEP) || (*ini_p == D2_EOS)) 
      {
      for (subtree++; *subtree != D2_EOS; subtree++)
         if (*subtree == D2_RDN_SEP) 
            return(D21_NEW_PARENT);

      return(D21_OLD_PARENT);
      }

   return(D21_NEW_PARENT);
}

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d21t007_set_status_flag()                             */
/*								      */
/* AUTHOR:      Serra (D AP 11)                                       */
/* DATE:        19.07.89                                              */
/*								      */
/* SYNTAX:      signed32 d21t007_set_status_flag(str1, str2)          */
/*              char  *str1;                                          */
/*              char  *str2;                                          */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function checks if an object belongs to the      */
/*              saved subtree, is a parent of it or is in another     */
/*              subtree.                                              */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*              char  *str1;   DN of the object to be checked         */
/*              char  *str2;   DN of the parent of the saved subtree  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*								      */
/*      D21_PARENT_OBJ : The object is over the parent object of the  */
/*                       subtree                                      */
/*      D21_REAL_OBJ   : The object belongs to the subtree            */
/*      D21_EXTERN_OBJ : The object does not belong to the subtree    */
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

 signed32 d21t007_set_status_flag(
   char *str1,
   char *str2)
{
   for ( ; *str1 == *str2; str1++, str2++) 
      if (*str1 == D2_EOS)
         return(D21_REAL_OBJ);
   if (*str1 == D2_RDN_SEP && *str2 == D2_EOS)
      return(D21_REAL_OBJ);
   if (*str1 == D2_EOS && *str2 == D2_RDN_SEP)
      return(D21_PARENT_OBJ);
   return(D21_EXTERN_OBJ);
}

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d21t008_get_attr()                                    */
/*								      */
/* AUTHOR:      Serra (D AP 11)                                       */
/* DATE:        19.07.89                                              */
/*								      */
/* SYNTAX:      d2_ret_val d21t008_get_attr(fd, dn, at_mem, min_pos,  */
/*                            max_pos, attr_type, attr_val, attr_len) */
/*              int           fd;                                     */
/*              D20_offset   *dn;                                     */
/*              char         *at_mem;                                 */
/*              signed32     *min_pos;                                */
/*              signed32     *max_pos;                                */
/*              D2_a_type     attr_type;                              */
/*              char        **attr_val;                               */
/*              signed16     *attr_len;                               */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function gets from the specified object the      */
/*              attribute value and the attribute length corresponding*/
/*              to "attr_type".                                       */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*              int           fd;        file descriptor              */
/*              D20_offset   *dn;        DN of the object             */
/*              char         *at_mem;    Memory area for the attribute*/
/*              D2_a_type     attr_type; Attribute type to be read    */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              signed32     *min_pos;                                */
/*              signed32     *max_pos;                                */
/*              char        **attr_val;  Attribute value read         */
/*              signed16     *attr_len;  Attribute length             */
/*								      */
/* RETURNVALUE:							      */
/*								      */
/*      D2_NOERROR:  No error occurred                                */
/*      D2_ERROR:    Error occurred                                   */
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

 d2_ret_val d21t008_get_attr(
   int              fd,
   D20_offset      *dn,
   char            *at_mem,
   signed32        *min_pos,
   signed32        *max_pos,
   D2_a_type        attr_type,
   char           **attr_val,
   signed16        *attr_len,
   char            *beg)
{
   signed16      optype = D20__BEGIN;
   D2_a_type     type;
   signed16	*len;
   unsigned16   *rep;
   char         *p_dn;
   char         *p_attr;
   char         *p_mess;
   char         *p_max;
   D2_a_type     at_type;

   if (d20_119_read_attr(fd, dn->d20_file_off, beg + dn->d20_mem_off,
			 dn->d20_attr_len, at_mem, min_pos, max_pos, &p_dn,
			 &p_attr)) {
      return(D2_ERROR);
   }
   while (!d20_attr_from_message(optype, FALSE,  p_attr, dn->d20_attr_len,
				 &type, &len, &rep, attr_val, &p_mess,
				 &p_max, &at_type)) {
      if (D27_ATTCMP(&type,&attr_type)==0) {
         *attr_len = *len;
         return(D2_NOERROR);
      }
      optype = D20__CONTINUE;
   }
   return(D2_ERROR);
}
