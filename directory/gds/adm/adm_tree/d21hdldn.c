/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21hdldn.c,v $
 * Revision 1.1.10.2  1996/02/18  19:40:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:21:03  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:15:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:24  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:04:57  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:07:32  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:58:02  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:25:18  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  09:43:41  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:23:41  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  17:48:37  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:02:58  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/18  08:26:23  marrek
 * 	Various bug fixes for mask handling found at SNI
 * 	[1992/12/17  16:25:39  marrek]
 * 
 * Revision 1.1.4.2  1992/11/27  14:23:16  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  15:36:33  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:49:13  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:13:38  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21hdldn.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:40:00 $";
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
/*                                                                    */
/* NAME         : d21hdldn.o   [restoring of subtrees]                */
/*                                                                    */
/* AUTHOR       : Josep Pulpillo CDS D 525                            */
/* DATE         : 28.07.89                                            */
/*                                                                    */
/* COMPONENT    : DS                                                  */
/*                                                                    */
/* DOK.-NR.     : DS-Design-Spez.                                     */
/*                      .                                             */
/*                      .                                             */
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* SYSTEM-DEPENDENCIES:                                               */
/*                      COMMON                                        */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*      0.1 | 28.07.89| Original                       | ws |         */
/*                                                                    */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                     Modulidentification                            */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                        I N C L U D E S                             */
/*                                                                    */
/**********************************************************************/

#include <gds.h>
#include <stdio.h>
#include <string.h>

#include <dce/d2dir.h>
#include <gdsext.h>
#include <dce/d27util.h>
#include <d2dump.h>
#include <d21.h>
#include <d20proto.h>

/**********************************************************************/
/*                                                                    */
/*                        D E F I N E S                               */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                 D E C L A R A T I O N S        DATA                */
/*                                                                    */
/**********************************************************************/

extern char      *d20_beg1;             /* start address of DN-strings*/
extern char      *d20_beg2;             /* start address of DN-strings*/

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*                                                                    */
/**********************************************************************/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME       : d21t005_compare_dn_appnd()                            */
/*                                                                    */
/* AUTHOR:      Josep Pulpillo CDS D - 525                            */
/* DATE:        28.07.89                                              */
/*                                                                    */
/* SYNTAX:      signed32 d21t005_compare_dn_appnd(dn1, dn2, beg2)     */
/*              char        *dn1;                                     */
/*              D20_offset **dn2;                                     */
/*              char        *beg2;                                    */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will compare Distinguished Names pointed*/
/*              to by a pointer array.                                */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*              char        *dn1;       pointer to DN1                */
/*              D20_offset **dn2;       pointer to offset of DN2      */
/*              char        *beg2;      pointer to the begin of area  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*              -1:     DN1 < DN2                                     */
/*               0:     DN1 = DN2                                     */
/*               1:     DN1 > DN2                                     */
/*                                                                    */
/* STANDARD-HEADER: None                                              */
/*                                                                    */
/* GLOBAL  DATA  (read-only):                                         */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                      strcmp()                                      */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*exoff ***************************************************************/

 signed32 d21t005_compare_dn_appnd(
   char        *dn1,
   D20_offset **dn2,
   char        *beg2)

{                               /*  d21t005_compare_dn_appnd()        */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E  -  D E F I N I T I O N S                  */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

register int ret_value;
 char         norm_dn1[D2_DNL_MAX + 1];

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

strcpy(norm_dn1, dn1);
d20_norm(norm_dn1, strlen(norm_dn1));

if ((ret_value = strcmp(norm_dn1, beg2 + (*dn2)->d20_mem_off)) < 0)
{   
  return(-1);
}
else if (ret_value > 0)
{   
  return(1);
}
else
{
  return(0);
} 

}                               /*  d21t005_compare_dn_appnd()        */


/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME       : d21t001_hdldns_appnd()                                */
/*                                                                    */
/* AUTHOR:      Josep Pulpillo CDS D - 525                            */
/* DATE:        28.07.89                                              */
/*                                                                    */
/* SYNTAX:      signed16 d21t001_hdldns_appnd(fd1, dn1,dn_no1,at1_mem,*/
/*                    fd2, dn2, dn_no2, at2_mem, inherit, overwrite,  */
/*                    pacl, lenacl, new_parent, parent_obj, subtree,  */
/*		      bind_dsa, name_col, at_val_area);    	      */
/*                                                                    */
/*              int           fd1;                                    */
/*              D20_offset  **dn1;                                    */
/*              signed32     *dn_no1;                                 */
/*              char         *at1_mem;                                */
/*              int           fd2;                                    */
/*              D20_offset  **dn2;                                    */
/*              signed32      dn_no2;                                 */
/*              char         *at2_mem;                                */
/*              signed32      inherit;                                */
/*              Bool          overwrite;                              */
/*              char         *pacl;                                   */
/*              signed16      lenacl;                                 */
/*              signed32      new_parent;                             */
/*              String        parent_obj;                             */
/*              String        subtree;                                */
/*		Name_string   bind_dsa;				      */
/*              Bool         *name_col;                               */
/*		char	    *at_val_area;			      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will compare Distinguished Names in both*/
/*              DSAs and will update the second DSA according to the  */
/*              results of the comparisions.                          */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*             int           fd1;       file descriptor of file1(DSA1)*/
/*             D20_offset  **dn1;       pointer list to DNs in DSA1   */
/*             signed32     *dn_no1;    number of DNs in DSA1         */
/*             char         *at1_mem;   memory area for attribute list*/
/*                                      of DNs of DSA1                */
/*                                      and for DN                    */
/*             int           fd2;       file descriptor of file2(DSA2)*/
/*             D20_offset  **dn2;       pointer list to DNs in DSA2   */
/*             signed32      dn_no2;    number of DNs in DSA2         */
/*             char         *at2_mem;   memory area for attribute list*/
/*                                      of DNs of DSA2                */
/*                                      and for DN                    */
/*             signed32      inherit;   Flag specifying how the ACL   */
/*                                      will be set                   */
/*             Bool          overwrite; Flag indicating if existing   */
/*                                      objects in DSA2 will be       */
/*                                      overwritten                   */
/*             char         *pacl;      ACL of the parent object      */
/*             signed16      lenacl;    Length of the ACL             */
/*             signed32      new_parent;Flag indicating if the parent */
/*                                      object is new or not          */
/*             String        parent_obj;Dn of the parent object under */
/*                                      wich the subtree will be      */
/*                                      appended                      */
/*             String        subtree;   DN of the object at the top   */
/*                                      of the subtree                */
/*	       Name_string   bind_dsa;				      */
/*	       char	    *at_val_area; pointer to attribute        */
/*					value area.		      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*             signed32     *dn_no1;    number of DNs in DSA1 that    */
/*                                      were handled (until eventually*/
/*                                      an error occurs --> will be   */
/*                                      used for generation of the    */
/*                                      list of name collisions)      */
/*              Bool         *name_col; D2_TRUE: DN leads to name     */
/*                                               collision            */
/*                                      D2_FALSE: no name collision   */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*                 D2_NOERROR: operation is ok                        */
/*                 D2_ERROR:   operation fails                        */
/*                                                                    */
/* STANDARD-HEADER: None                                              */
/*                                                                    */
/* GLOBAL  DATA  (read-only):                                         */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                                                                    */
/*inoff ***************************************************************/

 signed16 d21t001_hdldns_appnd(
   int           fd1,
   D20_offset  **dn1,
   signed32     *dn_no1,
   char         *at1_mem,
   int           fd2,
   D20_offset  **dn2,
   signed32      dn_no2,
   char         *at2_mem,
   signed32      inherit,
   Bool          overwrite,
   char         *pacl,
   signed16      lenacl,
   signed32      new_parent,
   Name_string   parent_obj,
   Name_string   subtree,
   Name_string   bind_dsa,
   Bool         *name_col,
   char		*at_val_area)

{                               /*  d21t001_hdldns_appnd()            */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E  -  D E F I N I T I O N S                  */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

 D20_offset **dn1_max = dn1 + *dn_no1;
 D20_offset **dn2_max = dn2 + dn_no2;
 D20_offset **sav_dn1 = dn1;
 D20_offset **sav_dn2;

 signed32     state;
 String       p_newdn;
 char         newdn[D2_DNL_MAX];
 signed32     ret;
 Bool         dn_generated;     /* a flag indicating whether the  */
				    /* new DN has too be generated or */
				    /* whether it had already been    */
				    /* generated                      */

 signed32     min1 = 0; /* minimum position of read attributes 1  */
 signed32     max1 = 0; /* maximum position of read attributes 1  */
 signed32     min2 = 0; /* minimum position of read attributes 2  */
 signed32     max2 = 0; /* maximum position of read attributes 2  */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

dn_generated = FALSE;
while                           /*  DNs in both DSA exist             */
      (dn1 < dn1_max && dn2 < dn2_max)
{                               
   state =
	(*dn1)->d20_state & (D21_PARENT_OBJ | D21_REAL_OBJ | D21_EXTERN_OBJ);
   switch((int)state)
   {
      case D21_PARENT_OBJ:      /*  The object exists before the      */
				/*  subtree root object               */
				/*  no name collision                 */
	 *name_col++ = FALSE;
	 dn1++;                 /*  This object must not be handled   */
	 dn_generated = FALSE;
	 break;
      case D21_REAL_OBJ:        /*  The object belongs to the subtree */
				/*  Must have a different DN          */
	 if
	    (new_parent)
	 {   
	   if (dn_generated == FALSE)
	   {   
	     if (d21t010_change_dn((Name_string) (d20_beg1+(*dn1)->d20_mem_off),
				parent_obj, subtree, newdn) == D2_ERROR)
	     {   
	       return(D2_ERROR);
	     }
else
{
	       dn_generated = TRUE;
	       p_newdn = newdn;
	     } 
	   } 
	 }
else
{
	   p_newdn = d20_beg1+(*dn1)->d20_mem_off;
	 } 
				/*  compare DNs                       */
	 switch ((int) d21t005_compare_dn_appnd(p_newdn, dn2, d20_beg2))
	 {
	    case -1:            /*  add new DN to DSA2                */
	       if               /*  operation fails                   */
		  (d21t003_add_appnd(fd1, *dn1, at1_mem, &min1, &max1,
				     inherit, parent_obj, subtree,
				     new_parent, bind_dsa,
				     lenacl, pacl,at_val_area,APPEND_SUB))
	       {                /*  return(D2_ERROR)                  */
		 *name_col++ = FALSE;
				/*  set the number of handled DNs in  */
				/*  list 1                            */
		 *dn_no1 = dn1 - sav_dn1;
		 return(D2_ERROR);
	       }                
	       *name_col++ = FALSE;
				/*  take next DN from DSA1            */
	       dn1++;
	       dn_generated = FALSE;
	       break;
	    case 0:             /*  Object exists in the target DSA   */
				/*  modify entry                      */
				/*  if overwrite is FALSE, it will not*/
				/*  be modified; but it will be checked */
				/*  the entries are different in any  */
				/*  attribute and "*name_col" will be */
				/*  set depending of this result      */
				/*  if overwrite is TRUE, it will be  */
				/*  modified if the attributes are    */
				/*  different and only in this case   */
				/*  "*name_col" will be set           */
	       if               /*  operation fails                   */
		  (d21t004_modify_appnd(fd1, *dn1, at1_mem, &min1,
					&max1, fd2, *dn2, at2_mem, &min2,
					&max2, inherit, parent_obj, subtree,
					new_parent, pacl, lenacl, bind_dsa, 
					overwrite, name_col,
					at_val_area,APPEND_SUB))
	       {                /*  return(D2_ERROR)                  */
				/*  set the number of handled DNs in  */
				/*  list 1                            */
		 *dn_no1 = dn1 - sav_dn1;
		 return(D2_ERROR);
	       }                
				/*  take next DN from DSA1 and DSA2   */
	       dn1++;
	       dn2++;
	       name_col++;
	       dn_generated = FALSE;
	       break;
	    case 1:             /*  this entry will remain unchanged  */
				/*  take next DN from DSA2            */
	       dn2++;
	       break;
	 }                      
	 break;
      case D21_EXTERN_OBJ:      /*   DSA names                        */
	 p_newdn = d20_beg1+(*dn1)->d20_mem_off;
	 switch ((int) d21t005_compare_dn_appnd(p_newdn, dn2, d20_beg2))
	 {
	    case -1:            /*  add new DN to DSA2                */
	       if               /*  operation fails                   */
		  (d21t003_add_appnd(fd1, *dn1, at1_mem, &min1, &max1,
				     inherit, parent_obj, subtree,
				     new_parent, bind_dsa,
				     lenacl, pacl,at_val_area,APPEND_SUB))
	       {                /*  return(D2_ERROR)                  */
		 *name_col++ = FALSE;
				/*  set the number of handled DNs in  */
				/*  list 1                            */
		 *dn_no1 = dn1 - sav_dn1;
		 return(D2_ERROR);
	       }                
	       *name_col++ = FALSE;
				/*  take next DN from DSA1            */
	       dn1++;
	       dn_generated = FALSE;
	       break;
	    case 0:             /*  Object exists in the target DSA   */
	       dn1++;
	       dn2++;
	       dn_generated = FALSE;
				/*  no name collision                 */
	       *name_col++ = FALSE;
	       break;
	    case 1:             /*  this entry will remain unchanged  */
				/*  take next DN from DSA2            */
	       sav_dn2 = dn2 + 1;
	       while            /*  not end of list 2 reached         */
		     (sav_dn2 < dn2_max)
	       {                /*  look if DN is in the list2 at the */
				/*  end:                              */
				/*  NOTE: As the SEARCH in list 2 was */
				/*        done with another base object */
				/*        the DN1 to be looked for now, */
				/*        may be after the actual DN2 */
				/*        (which may represent the new*/
				/*        subtree)                    */
		 if ((ret = (signed32) d21t005_compare_dn_appnd(p_newdn,
					  sav_dn2, d20_beg2)) == -1)
		 {              /*  add DN                            */
		   if           /*  operation fails                   */
		      (d21t003_add_appnd(fd1, *dn1, at1_mem, &min1, &max1,
					 inherit, parent_obj, subtree,
					 new_parent, bind_dsa, lenacl, 
					 pacl,at_val_area,APPEND_SUB))
		   {            /*  return(D2_ERROR)                  */
		     *name_col++ = FALSE;
				/*  set the number of handled DNs in  */
				/*  list 1                            */
		     *dn_no1 = dn1 - sav_dn1;
		     return(D2_ERROR);
		   }
else
{            /*  take next DN from DSA1            */
		     *name_col++ = FALSE;
		     dn1++;
		     dn_generated = FALSE;
		     break;
		   }            
		 }
else if (ret == 0)
		 {              /*  Object exists in the target DSA   */
		   dn1++;
		   dn_generated = FALSE;
				/*  no name collision                 */
		   *name_col++ = FALSE;
		   break;
		 }
else
{              /*  take next DN from list 2          */
		   sav_dn2++;
		 }              
	       }                

	       if               /*  DN not found in list 2            */
		  (sav_dn2 == dn2_max)
	       {                /*  add DN to DSA                     */
		 if             /*  operation fails                   */
		    (d21t003_add_appnd(fd1, *dn1, at1_mem, &min1, &max1,
				       inherit, parent_obj, subtree,
				       new_parent, bind_dsa, lenacl, pacl,
				       at_val_area,APPEND_SUB))
		 {              /*  return(D2_ERROR)                  */
		   *name_col++ = FALSE;
				/*  set the number of handled DNs in  */
				/*  list 1                            */
		   *dn_no1 = dn1 - sav_dn1;
		   return(D2_ERROR);
		 }              
		 *name_col++ = FALSE;
				/*  take next DN from DSA1            */
		 dn1++;
		 dn_generated = FALSE;
	       }                
	       break;
	 }                      
	 break;
   }   
}

while                           /*  DNs in first DSA exist            */
      (dn1 < dn1_max)
{                               /*  add DN to second DSA              */
  if                            /*  operation fails                   */
     (d21t003_add_appnd(fd1, *dn1, at1_mem, &min1, &max1, inherit,
			parent_obj, subtree, new_parent, bind_dsa, lenacl, 
			pacl,at_val_area,APPEND_SUB))
  {                             /*  return(D2_ERROR)                  */
    *name_col++ = FALSE;
				/*  set the number of handled DNs in  */
				/*  list 1                            */
    *dn_no1 = dn1 - sav_dn1;
    return(D2_ERROR);
  }                             
  *name_col++ = FALSE;
				/*  take next DN from DSA1            */
  dn1++;
}                               

*dn_no1 = dn1 - sav_dn1;
				/*  return(D2_NOERROR)                */
return(D2_NOERROR);

}                               /*  d21t001_hdldns_appnd()            */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME       :  d21t010_change_dn()                                  */
/*                                                                    */
/* AUTHOR:      Josep Pulpillo CDS D - 525                            */
/* DATE:        25.07.89                                              */
/*                                                                    */
/* SYNTAX:      signed32  d21t010_change_dn()                         */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function builds the new DN of an object.         */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*              Name_string     old_name;   DN of the changing object */
/*              Name_string     new_par;    New Relative DN.          */
/*              Name_string     old_par;    Old Relative DN.          */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              char           *new_dn;     new generated DN          */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*              D2_NOERROR:     DN could be built                     */
/*              D2_ERROR:       error while generating new DN         */
/*                                                                    */
/* STANDARD-HEADER: None                                              */
/*                                                                    */
/* GLOBAL  DATA  (read-only):                                         */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*exoff ***************************************************************/

 d2_ret_val d21t010_change_dn(
   Name_string  old_name,
   Name_string  new_par,
   Name_string  old_par,
   char        *new_dn)
{                               /*  d21t010_change_dn()               */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E  -  D E F I N I T I O N S                  */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

        char           *p = (char *) 0;
        char            sav_dn[D2_DNL_MAX];
        signed32        l1;
        signed32        l2;

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

l2 = strlen((char *)new_par);

if (l2 == 0)
{   strcpy(new_dn, (char *)old_name);
    return(D2_NOERROR);
}

if (old_par != (Name_string) 0 && strlen((char *)old_par) != 0)
{   strcpy(sav_dn, (char *)old_par);

	/* get first RDN seperator from behind the old_par object */
    if ((p = strrchr(sav_dn, (int) D2_RDN_SEP)) != (char *) 0)
				/* --> more than one RDN in parent name */
    {   *p = D2_EOS;
	l1 = strlen(sav_dn);
    }
}

if (new_par == NULL)
{   *new_dn = D2_EOS;
}
else
{   strcpy(new_dn, (char *)new_par);
}

if (p != (char *) 0)
{   strcat(new_dn, (char *)(old_name + l1));
}
else
{   *(new_dn+l2) = D2_RDN_SEP;
    *(new_dn+l2+1) = D2_EOS;
    strcat(new_dn, (char *)old_name);
}

return(D2_NOERROR);
}                               /*  d21t010_change_dn()               */

