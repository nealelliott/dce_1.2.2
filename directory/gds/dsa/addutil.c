/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: addutil.c,v $
 * Revision 1.1.716.2  1996/02/18  19:44:35  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:25:39  marty]
 *
 * Revision 1.1.716.1  1995/12/08  15:41:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:05:55  root]
 * 
 * Revision 1.1.714.3  1994/06/21  14:45:02  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  11:51:56  marrek]
 * 
 * Revision 1.1.714.2  1994/05/10  15:51:36  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:18:32  marrek]
 * 
 * Revision 1.1.714.1  1994/02/22  16:04:50  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:06:38  marrek]
 * 
 * Revision 1.1.712.2  1993/10/14  17:16:13  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:07:14  keutel]
 * 
 * Revision 1.1.712.1  1993/10/13  17:30:16  keutel
 * 	October 1993 code drop
 * 	[1993/10/11  08:32:42  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  13:22:32  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  12:57:43  marrek]
 * 
 * Revision 1.1.4.4  1993/02/02  14:43:34  marrek
 * 	*** empty log message ***
 * 
 * Revision 1.1.4.3  1993/02/02  11:15:23  marrek
 * 	Add max_ubound and D2_POST_AD_STX, D2_TLXNR_STX, D2_IA5
 * 	[1993/02/01  14:28:15  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:58:15  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:17:25  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:43:37  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:51:57  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: addutil.c,v $ $Revision: 1.1.716.2 $ $Date: 1996/02/18 19:44:35 $";
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
/*                                                                    */
/* NAME         : addutil.c <Utility-Functions for DS_ADD_OBJECT>     */
/*                                                                    */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 08.12.87                                            */
/*                                                                    */
/* COMPONENT    : DS                                                  */
/*                                                                    */
/* DOC.-NR.     : Directory-Design-Specification                      */
/*                      .                                             */
/*                      .                                             */
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* SYSTEM DEPENDENCIES: COMMON                                        */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   0.1    |01.02.86 |  Original                      | WS |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                     Module Identification                          */
/*                                                                    */
/**********************************************************************/


/**********************************************************************/
/*                                                                    */
/*                        I N C L U D E S                             */
/*                                                                    */
/**********************************************************************/

#include <d26dsa.h>

/*****  Operating System - Includes  *****/
#include <ctype.h>
#include <string.h>

/*****  internal Includes  *****/

#include <dce/d27util.h>
#include <d26schema.h>

/**********************************************************************/
/*                                                                    */
/*                        D E F I N E S                               */
/*                                                                    */
/**********************************************************************/

/* constants for phonetic string generation                           */
#define D26UNDEFINED_CLASS  0x3F    /* '?'                            */
#define D26IGNORE_CLASS     0x30    /* '0'                            */

#define D20_S_BLANK " "             /* anonymous requestor name       */

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

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S         FUNCTIONS           */
/*                                                                    */
/**********************************************************************/

/******** LOCAL  ********/
static   int   d26_u21_compare_dn(const void * str1, const void * str2);
static   int   d26_u25_compare_idx(const void * str1, const void * str2);

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_u20_sort_dn()                                   */
/*                                                                    */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 20.01.88                                            */
/*                                                                    */
/* SYNTAX       : Void  d26_u20_sort_dn(own_name, dn_ptr, dn_no,      */
/*                                      dn_sort, dn_act)              */
/*                D2_name_string  own_name;                           */
/*                D26_dns        *dn_ptr[];                           */
/*                Short           dn_no;                              */
/*                D26_dns        *dn_sort[];                          */
/*                Short          *dn_act;                             */
/*                                                                    */
/* DESCRIPTION  : This function will sort the list of Distinguished   */
/*                names (incl. DN-interpretation, occurrency,         */
/*                DN-index adress). The Distinguished Names are       */
/*                referenced by the input pointer field "dn_ptr".     */
/*                Another pointer list will be set (incl. number of   */
/*                pointers); it contains all DNs in increasing order. */
/*                                                                    */
/*                REMARK: Two fields in the array pointed to by       */
/*                        "dn_ptr" will be changed:                   */
/*                              - dn_ptr[i]->d26_dno: number of       */
/*                                                    occurrencies    */
/*                              - dn_ptr[i]->d26_didx: memory address */
/*                                                     that contains  */
/*                                                     index to DNL   */
/*                        If later on, the indices of the DNs in the  */
/*                        Distinguished Name List are to be used,     */
/*                        they must be referenced by                  */
/*                             idx = *(dn_ptr[i]->d26_dno)            */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*                D2_name_string own_name;  own entry name            */
/*                                          (normed value and all     */
/*                                          blanks removed)           */
/*                D26_dns    *dn_ptr[];     array of pointers pointing*/
/*                                          to DNs (incl. Interpretion*/
/*                                          occurrency, DN-index)     */
/*                Short       dn_no;        number of pointers (see   */
/*                                          above)                    */
/*                                                                    */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*                D26_dns    *dn_sort[];    array of pointers pointing*/
/*                                          to DNs (incl. Interpretion*/
/*                                          occurrency, DN-index)     */
/*                                          --> sorted list           */
/*                Short      *dn_act;       number of sorted pointers */
/*                                                                    */
/* RETURNVALUE     :  Void                                            */
/*                                                                    */
/* STANDARD-HEADER: -                                                 */
/*                                                                    */
/* GLOBAL DATA (read only):                                           */
/*                  -                                                 */
/*                                                                    */
/* GLOBAL DATA (updated) :                                            */
/*                  -                                                 */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                          strcmp()                                  */
/*                          strncmp()                                 */
/*                          strlen()                                  */
/*                          qsort()                                   */
/*                          d26_u21_compare_dn()                      */
/*                          d26_u22_count_del()                       */
/*                                                                    */
/* REMARKS :                                                          */
/*                                                                    */
/*exoff ***************************************************************/

void d26_u20_sort_dn
(
  D2_name_string  own_name,
  D26_dns        *dn_ptr[],
  signed16        dn_no,
  D26_dns        *dn_sort[],
  signed16       *dn_act
)

{                                   /*  d26_u20_sort_dn()             */

/**********************************************************************/
/*                                                                    */
/*                 T Y P  -  D E F I N I T I O N E N                  */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E K L A R A T I O N E N                         */
/*                                                                    */
/**********************************************************************/

register signed16       i, j, k;
register signed16       length;        /* length of own entry name       */
register signed16       l_act;         /* length of actual name in list  */
register signed16       no_del;        /* number of delimiters in own name */
register D26_dns    *dn1;
     signed32         level;

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

				    /*  determine length of own entry name*/
length = strlen((char *)own_name);
				    /*  determine number of delimiters in name */
no_del = d26_u22_count_del(own_name,length);
for                                 /*  all DNs                       */
    (i = *dn_act = 0; i < dn_no; i++)
{                                   /*                                */
  if                                /*  index determined              */
     (*((dn1 = dn_ptr[i])->d26_didx) >= 0)
  {                                 /*  copy DN in list to be sorted  */
    dn_sort[(*dn_act)++] = dn1;
  } else if                         /*  index is unknown              */
	(*(dn1->d26_didx) == D26_IDX_NOT_DEF)
  {                                 /*  look for own DN-name          */
    if                              /*  DN-name found                 */
       (!strcmp((char *)dn1->d26_dname, (char *)own_name))
    {                               /*                                */
      if                            /*  DN interpretation is "single object"*/
	 (dn1->d26_dint == D2_SOBJ)
      {                             /*  index = D26_OWNNAME           */
	*(dn1->d26_didx) = D26_OWNNAME;
      } else if                     /*  DN interpretation is "whole subtree" */
	    (dn1->d26_dint == D2_SUBTREE)
      {                             /*  index = D2_O_SUBTREE          */
	*(dn1->d26_didx) = D26_O_SUBTREE;
      } else {                      /*  copy DN in list to be sorted  */
	dn_sort[(*dn_act)++] = dn1;
      }                             /*                                */
    } else if                       /*  DN-interpretation is "single object" */
				    /*  or "group"                    */
	  (dn1->d26_dint == D2_SOBJ || dn1->d26_dint == D2_GROUP)
    {                               /*  copy DN in list to be sorted  */
      dn_sort[(*dn_act)++] = dn1;
    } else if                       /*  DN-interpretation is "ignore" */
	  (dn1->d26_dint == D2_IGNORE)
    {                               /*  index = D26_EOL               */
      *(dn1->d26_didx) = D26_EOL;
    } else {                        /*  take length of new DN         */
      l_act = strlen((char *)dn1->d26_dname);
      if                            /*  DN to be checked is ROOT      */
	 (l_act == 0)
      {                             /*  set DN index to a negative number */
	*(dn1->d26_didx) = D26_O_SUBTREE - no_del;
      } else if                     /*  actual length <= own name length  */
	    (l_act <= length)
      {                             /*  look for equality of superior parts */
	if                          /*  superior parts are equal      */
	   ((*(own_name + l_act) == D2_RDN_SEP || *(own_name + l_act) == D2_EOS)
	    && !strncmp((char *)dn1->d26_dname, (char *)own_name, l_act))
	{                           /*  determine subtree level       */
	  level = no_del - d26_u22_count_del(dn1->d26_dname, l_act);
	  *(dn1->d26_didx) = D26_O_SUBTREE - level;
	} else {                    /*  copy DN in list to be sorted  */
	  dn_sort[(*dn_act)++] = dn1;
	}                           /*                                */
      } else {                      /*  copy DN in list to be sorted  */
	dn_sort[(*dn_act)++] = dn1;
      }                             /*                                */
    }                               /*                                */
  }                                 /*                                */
}                                   /*                                */

				    /*  sort DNs                      */
qsort((char *) &dn_sort[0], *dn_act, sizeof(String), d26_u21_compare_dn);

for                                 /*  all DNs in sorted list        */
    (i = 0, k = 1, j = *dn_act - 1; i < j; k++)
{                                   /*  check equal indices           */
  if                                /*  DNs are equal                 */
     (!d26_u21_compare_dn((String) &dn_sort[i], (String) &dn_sort[k]))
  {                                 /*  determine new occurrency      */
    dn_sort[i]->d26_dno += dn_sort[k]->d26_dno;
    if                              /*  DN-index of DN1 is unknown    */
       (*(dn_sort[i]->d26_didx) == D26_IDX_NOT_DEF)
    {                               /*  DN1-index = DN2-index         */
      *(dn_sort[i]->d26_didx) = *(dn_sort[k]->d26_didx);
    }                               /*                                */
				    /*  DN2-index-adress = DN1-index-address */
    dn_sort[k]->d26_didx = dn_sort[i]->d26_didx;
				    /*  decrease number of sorted entries */
    (*dn_act)--;
    j--;
  } else {                          /*  take next DN                  */
    dn_sort[++i] = dn_sort[k];
  }                                 /*                                */
}                                   /*                                */

}                                   /*  d26_u20_sort_dn()             */


/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_u21_compare_dn()                                */
/*                                                                    */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 20.01.88                                            */
/*                                                                    */
/* SYNTAX       : Int d26_u21_compare_dn(str1, str2)                  */
/*                String str1;                                        */
/*                String str2;                                        */
/*                                                                    */
/* DESCRIPTION  : This function compares two distinguished names      */
/*                (incl. DN-interpretation).                          */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*                String    str1;   char-pointer to first DN          */
/*                String    str2;   char-pointer to second DN         */
/*                                                                    */
/* OUTPUT-PARAMETERS: -                                               */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*                < 0 :     DN1 < DN2                                 */
/*                = 0 :     DN1 = DN2                                 */
/*                > 0 :     DN1 > DN2                                 */
/*                                                                    */
/* STANDARD-HEADER: -                                                 */
/*                                                                    */
/* GLOBAL DATA (read only):                                           */
/*                  -                                                 */
/*                                                                    */
/* GLOBAL DATA (updated) :                                            */
/*                  -                                                 */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                          strcmp()                                  */
/*                                                                    */
/* REMARKS :                                                          */
/*                                                                    */
/*inoff ***************************************************************/

static  int d26_u21_compare_dn
(
  const void  * str1,
  const void  * str2
)

{                                   /*  d26_u21_compare_dn()          */

/**********************************************************************/
/*                                                                    */
/*                 T Y P  -  D E F I N I T I O N E N                  */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E K L A R A T I O N E N                         */
/*                                                                    */
/**********************************************************************/

register D26_dns    **dn1 = (D26_dns **) str1;
register D26_dns    **dn2 = (D26_dns **) str2;

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

if                         /*  DN1-interpretation < DN2-interpretation*/
   ((*dn1)->d26_dint < (*dn2)->d26_dint)
{                                   /*  return(-1)                    */
  return(-1);
} else if                  /*  DN1-interpretation > DN2-interpretation*/
      ((*dn1)->d26_dint > (*dn2)->d26_dint)
{                                   /*  return(1)                     */
  return(1);
} else {                            /*  compare DN-names              */
				    /*  return compare result         */
  return((int) strcmp((char *)(*dn1)->d26_dname, (char *)(*dn2)->d26_dname));
}                                   /*                                */

}                                   /*  d26_u21_compare_dn()          */


/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_u22_count_del()                                 */
/*                                                                    */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 25.01.88                                            */
/*                                                                    */
/* SYNTAX       : Short d26_u22_count_del(name, length)               */
/*                D2_name_string name;                                */
/*                Short          length;                              */
/*                                                                    */
/* DESCRIPTION  : This function counts the number of delimiters ("!") */
/*                in the given name.                                  */
/*                Additionally the EOS will be counted as delimiter,  */
/*                too.                                                */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*                D2_name_string name;   entry name                   */
/*                Short          length; length of entry name         */
/*                                                                    */
/* OUTPUT-PARAMETERS: -                                               */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*                >= 0:     number of delimiters in entry name        */
/*                                                                    */
/* STANDARD-HEADER: -                                                 */
/*                                                                    */
/* GLOBAL DATA (read only):                                           */
/*                  -                                                 */
/*                                                                    */
/* GLOBAL DATA (updated) :                                            */
/*                  -                                                 */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                                                                    */
/* REMARKS :                                                          */
/*                                                                    */
/*inoff ***************************************************************/

signed16 d26_u22_count_del
(
  D2_name_string name,
  signed16      length
)

{                                   /*  d26_u22_count_del()           */

/**********************************************************************/
/*                                                                    */
/*                 T Y P  -  D E F I N I T I O N E N                  */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E K L A R A T I O N E N                         */
/*                                                                    */
/**********************************************************************/

        signed16          no_del = 1;
register    D2_name_string max_adr = name + length;

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

				    /*  count number of delimiters    */
for ( ; name < max_adr; )
{ 
  if (*name++ == D2_RDN_SEP)
  {   
    no_del++;
  } 
} 
				    /*  return number of delimiters   */
return(no_del);

}                                   /*  d26_u22_count_del()           */


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_u23_get_acl_names()                             */
/*                                                                    */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 26.01.88                                            */
/*                                                                    */
/* SYNTAX       : Short d26_u23_get_acl_names(acl_attr, acl_length,   */
/*                                            dnl_ptr, dns, acl_idx)  */
/*                D2_name_string   acl_attr;                          */
/*                Ushort           acl_length;                        */
/*                D26_dns        **dnl_ptr;                           */
/*                D26_dns         *dns;                               */
/*                D26_acl         *acl_idx;                           */
/*                                                                    */
/* DESCRIPTION  : This function analyses the given ACL-attributes and */
/*                stores the ACL-information in an internal format.   */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*                D2_name_string   acl_attr;    ACL-attribute to be   */
/*                                              analysed              */
/*                Ushort           acl_length;  length of ACL-attr.   */
/*                D26_dns        **dnl_ptr;     address of pointer    */
/*                                              array where the       */
/*                                              pointers pointing to  */
/*                                              internal structure are*/
/*                                              stored                */
/*                D26_dns         *dns;         address of internal   */
/*                                              structure where the   */
/*                                              ACL information can be*/
/*                                              stored                */
/*                D26_acl         *acl_idx;     address of array of   */
/*                                              "shorts" where the    */
/*                                              index in the DNL can  */
/*                                              be stored             */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*                D26_dns        **dnl_ptr;     pointers that are set */
/*                                              (pointing to "dns")   */
/*                D26_dns         *dns;         filled structure of   */
/*                                              ACL information       */
/*                                                                    */
/* RETURNVALUE     :  D2_NO_ERR:    ACL-attribute is correct          */
/*                    D26_ERROR:    ACL-attribute is not correct      */
/*                                                                    */
/* STANDARD-HEADER: -                                                 */
/*                                                                    */
/* GLOBAL DATA (read only):                                           */
/*                  -                                                 */
/*                                                                    */
/* GLOBAL DATA (updated) :                                            */
/*                  -                                                 */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                                                                    */
/* REMARKS :    The ACl-Distinguished Names will be changed to        */
/*              normed strings. Blanks must have been removed.        */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_u23_get_acl_names
(
  D2_name_string   acl_attr,
  signed16         acl_length,
  D26_dns        **dnl_ptr,
  D26_dns         *dns,
  D26_acl         *acl_idx
)

{                                   /*  d26_u23_get_acl_names()       */

/**********************************************************************/
/*                                                                    */
/*                 T Y P  -  D E F I N I T I O N E N                  */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E K L A R A T I O N E N                         */
/*                                                                    */
/**********************************************************************/

     boolean            correct;
     signed16          len_acl;

register D2_name_string  max_adr = acl_attr + acl_length;
register D26_dns        *max_dns = dns + D2_ACLIDX_MAX;
register D26_dns        *sav_dns = dns;
register D26_dns        *max1;

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

for                                 /*  all ACL-names                 */
    ( ; dns < max_dns && acl_attr < max_adr ; )
{                                   /*                                */
  if                                /*  ACL-interpretation is not correct */
     (*acl_attr != D2_SOBJ  && *acl_attr != D2_SUBTREE &&
      *acl_attr != D2_GROUP && *acl_attr != D2_IGNORE)
  {                                 /*  return(D26_ERROR)             */
    return(D26_ERROR);
  } else {                          /*  store ACL-interpretation      */
    dns->d26_dint = *acl_attr;
  }                                 /*                                */
				    /*  store ACL-name                */
  dns->d26_dname = ++acl_attr;

  len_acl = (signed16) strlen((char *)acl_attr);
				    /*  remove blanks in ACL name     */
  d27_012_remove_blanks((char *) acl_attr, len_acl, (char *) acl_attr);
				    /* exact name for DN-List */
  strcpy(dns->d26_edname, (char *) dns->d26_dname);
				    /*  norm ACL-name                 */
  d27_007_norm_name((char *) acl_attr, len_acl, (char *) acl_attr);
  acl_attr += len_acl + 1;          /* incl. EOS                      */

				    /*  number of occurrencies = 1    */
  dns->d26_dno   = 1;
				    /*  store address of ACL-index    */
  dns->d26_didx  = acl_idx;
				    /*  ACL-Index = D26_IDX_NOT_DEF   */
  *acl_idx++ = D26_IDX_NOT_DEF;
				    /*  set pointer to DNL-structure  */
  *dnl_ptr++ = dns++;
}                                   /*                                */
if                                  /*  ACL-attribute is correct      */
   (dns == max_dns && acl_attr == max_adr)
{                                   /*  check rights                  */
  for                               /*  all ACL-names                 */
      (dns = sav_dns; dns < max_dns; dns++)
  {                                 /*                                */
    for                             /*  all ACL-Names per access right*/
	(correct = FALSE, max1 = dns + D2_NOIDX_ACL; dns < max1; dns++)
    {                               /*                                */
      if                            /*  ACL-name is given             */
	 (dns->d26_dint != D2_IGNORE)
      {                             /*  set flag "ACL OK"             */
	correct = TRUE;
      }                             /*                                */
    }                               /*                                */
    if                              /*  ACL is "NOT_OK"               */
       (correct == FALSE)
    {                               /*  return(D26_ERROR)             */
      return(D26_ERROR);
    }                               /*                                */
  }                                 /*                                */
				    /*  return(D2_NO_ERR)             */
  return(D2_NO_ERR);
} else {                            /*  return(D26_ERROR)             */
  return(D26_ERROR);
}                                   /*                                */

}                                   /*  d26_u23_get_acl_names()       */


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_u24_sort_acl_idx()                              */
/*                                                                    */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 27.01.88                                            */
/*                                                                    */
/* SYNTAX       : Void  d26_u24_sort_acl_idx(dn_ptr, dn_no, dn_sort,  */
/*                                           dn_act)                  */
/*                D26_dns        *dn_ptr[];                           */
/*                Short           dn_no;                              */
/*                D26_dns        *dn_sort[];                          */
/*                Short          *dn_act;                             */
/*                                                                    */
/* DESCRIPTION  : This function will sort the list of ACL-indices     */
/*                pointing to DNL.                                    */
/*                In the internal struccture "dn_ptr" only two fields */
/*                must be set:                                        */
/*                      - d26_dno:  number of occurrencies            */
/*                      - d26_didx: address of ACL-idx                */
/*                Another pointer list will be set (incl. number of   */
/*                pointers); it contains all ACL-idx in increasing    */
/*                order.                                              */
/*                                                                    */
/*                REMARK: One field in the array pointed to by        */
/*                        "dn_ptr" will be changed:                   */
/*                              - dn_ptr[i]->d26_dno: number of       */
/*                                                    occurrencies    */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*                D26_dns    *dn_ptr[];     array of pointers pointing*/
/*                                          to ACL-idx                */
/*                Short       dn_no;        number of pointers (see   */
/*                                          above)                    */
/*                                                                    */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*                D26_dns    *dn_sort[];    array of pointers pointing*/
/*                                          to ACL-idx                */
/*                                          --> sorted list           */
/*                Short      *dn_act;       number of sorted pointers */
/*                                                                    */
/* RETURNVALUE     :  Void                                            */
/*                                                                    */
/* STANDARD-HEADER: -                                                 */
/*                                                                    */
/* GLOBAL DATA (read only):                                           */
/*                  -                                                 */
/*                                                                    */
/* GLOBAL DATA (updated) :                                            */
/*                  -                                                 */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                          qsort()                                   */
/*                          d26_u25_compare_idx()                     */
/*                          d26_u22_count_del()                       */
/*                                                                    */
/*                                                                    */
/* REMARKS :                                                          */
/*                                                                    */
/*exoff ***************************************************************/

void d26_u24_sort_acl_idx
(
  D26_dns        *dn_ptr[],
  signed16        dn_no,
  D26_dns        *dn_sort[],
  signed16       *dn_act
)

{                                   /*  d26_u24_sort_acl_idx()        */

/**********************************************************************/
/*                                                                    */
/*                 T Y P  -  D E F I N I T I O N E N                  */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E K L A R A T I O N E N                         */
/*                                                                    */
/**********************************************************************/

register signed16       i, j, k;
register D26_dns    *dn1;

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

for                                 /*  all DNs                       */
    (i = *dn_act = 0; i < dn_no; i++)
{                                   /*                                */
  if                                /*  index >= 0 ("real index") or "unknown" */
     (*((dn1 = dn_ptr[i])->d26_didx) >= 0 ||
      *(dn1->d26_didx) == D26_IDX_NOT_DEF)
  {                                 /*  copy ACl-idx in list to be sorted */
    dn_sort[(*dn_act)++] = dn1;
  }                                 /*                                */
}                                   /*                                */

				    /*  sort ACL-idx                  */
qsort((char *) &dn_sort[0], *dn_act, sizeof(String), d26_u25_compare_idx);

for                                 /*  all DNs in sorted list        */
    (i = 0, k = 1, j = *dn_act - 1; i < j; k++)
{                                   /*  check equal indices           */
  if                                /*  ACL-idx are equal             */
     ( *(dn_sort[i]->d26_didx) == *(dn_sort[k]->d26_didx) )
  {                                 /*  determine new occurrency      */
    dn_sort[i]->d26_dno += dn_sort[k]->d26_dno;
				    /*  DN2-index-adress = DN1-index-address */
    dn_sort[k]->d26_didx = dn_sort[i]->d26_didx;
				    /*  decrease number of sorted entries */
    (*dn_act)--;
    j--;
  } else {                          /*  take next ACL-idx             */
    dn_sort[++i] = dn_sort[k];
  }                                 /*                                */
}                                   /*                                */

}                                   /*  d26_u24_sort_acl_idx()        */


/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_u25_compare_idx()                               */
/*                                                                    */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 27.01.88                                            */
/*                                                                    */
/* SYNTAX       : Int d26_u25_compare_dn(str1, str2)                  */
/*                String str1;                                        */
/*                String str2;                                        */
/*                                                                    */
/* DESCRIPTION  : This function compares two DNL-idx.                 */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*                String    str1;   char-pointer to first DNL-idx     */
/*                String    str2;   char-pointer to second DNL-idx    */
/*                                                                    */
/* OUTPUT-PARAMETERS: -                                               */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*                < 0 :     idx1 < idx2                               */
/*                = 0 :     idx1 = idx2                               */
/*                > 0 :     idx1 > idx2                               */
/*                                                                    */
/* STANDARD-HEADER: -                                                 */
/*                                                                    */
/* GLOBAL DATA (read only):                                           */
/*                  -                                                 */
/*                                                                    */
/* GLOBAL DATA (updated) :                                            */
/*                  -                                                 */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                                                                    */
/* REMARKS :                                                          */
/*                                                                    */
/*inoff ***************************************************************/

static  int d26_u25_compare_idx
(
  const void * str1,
  const void * str2
)

{                                   /*  d26_u25_compare_idx()         */

/**********************************************************************/
/*                                                                    */
/*                 T Y P  -  D E F I N I T I O N E N                  */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E K L A R A T I O N E N                         */
/*                                                                    */
/**********************************************************************/

register D26_dns    **dn1 = (D26_dns **) str1;
register D26_dns    **dn2 = (D26_dns **) str2;

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

if                                  /*  index1 < index2               */
   ( *((*dn1)->d26_didx) < *((*dn2)->d26_didx) )
{                                   /*  return(-1)                    */
  return(-1);
} else if                           /*  index1 > index2               */
      ( *((*dn1)->d26_didx) > *((*dn2)->d26_didx) )
{                                   /*  return(1)                     */
  return(1);
} else {                            /*  return(0)                     */
  return(0);
}                                   /*                                */

}                                   /*  d26_u25_compare_idx()         */


/*exon ****************************************************************/
/*                                                                    */
/* TYP          : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_u28_copy_dn(...)                                */
/*                                                                    */
/* AUTHOR       : W. Schmid, D AP 11                                  */
/* DATE         : 09.02.88                                            */
/*                                                                    */
/*                                                                    */
/* DESCRIPTION  : The function will copy the Distinguished Name       */
/*                (of message APDU 1) in message 2.                   */
/*                The fields "from" and "to" are pointers to the      */
/*                appropriate positions in both messages.             */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*                Char          **from; pointer to source DN          */
/*                Char          **to;   pointer to destination DN     */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*                Char          **from; pointer to source DN (actual  */
/*                                      position)                     */
/*                Char          **to;   pointer to destination DN     */
/*                                      (actual position)             */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*                None                                                */
/*                                                                    */
/* STANDARDHEADER  :                                                  */
/*                                                                    */
/* GLOBAL  DATA  (read):                                              */
/*                None                                                */
/*                                                                    */
/* GLOBAL  DATA  (changed):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*                                                                    */
/* REMARKS:                                                           */
/*               Nothing                                              */
/*                                                                    */
/*exoff ***************************************************************/

void d26_u28_copy_dn
(
  char   **from,
  char   **to
)

{                                   /*  d26_u28_copy_dn()             */

/**********************************************************************/
/*                                                                    */
/*                 T Y P  -  D E F I N I T I O N E N                  */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E K L A R A T I O N E N                         */
/*                                                                    */
/**********************************************************************/

 signed16 len;
 signed16 off;

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

((D23_ds_name *)*to)->d23_dstag = ((D23_ds_name *)*from)->d23_dstag;
off = ((D23_ds_name *)*to)->d23_dsoff = ((D23_ds_name *)*from)->d23_dsoff;
len = ((D23_ds_name *)*to)->d23_dslen = ((D23_ds_name *)*from)->d23_dslen;
*to += D23_S_NM;
*from += D23_S_NM;
memcpy(*to, *from , len);
*to += off;
*from += off;

}                                   /*  d26_u28_copy_dn()             */


/*exon ****************************************************************/
/*                                                                    */
/* TYP          : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_u96_attr_len_check()                            */
/*                                                                    */
/* AUTHOR       : W. Schmid, D AP 11                                  */
/* DATE         : 20.06.91                                            */
/*                                                                    */
/* DESCRIPTION  : This function will check the attribute length of    */
/*                an attribute depending on it's syntax.              */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*                   D26_at     *at;        attribute description     */
/*                   Ushort      len;       attribute length          */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*                None                                                */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*                  D2_NOERROR: attribute length is correct           */
/*                  D26_ERROR:  attribute length not correct          */
/*                                                                    */
/* STANDARDHEADER  :                                                  */
/*                                                                    */
/* GLOBAL  DATA  (read):                                              */
/*                None                                                */
/*                                                                    */
/* GLOBAL  DATA  (changed):                                           */
/*                None                                                */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*                                                                    */
/* REMARKS:                                                           */
/*               Nothing                                              */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d26_u96_attr_len_check
(
  D26_at     *at,
  signed16    len
)

{                                   /*  d26_u96_attr_len_check()      */

/**********************************************************************/
/*                                                                    */
/*                 T Y P  -  D E F I N I T I O N E N                  */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E K L A R A T I O N E N                         */
/*                                                                    */
/**********************************************************************/
short  max_ubound;          /* upper bound (bigger then in AT in case */
			    /* of structured attributes)              */
/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

max_ubound = at->d26a_ubound;

switch ((int) at->d26a_syntax)          /*  attribute syntax is       */
{
  case D2_INTEGER:      if ((len % sizeof(int)) != 0)
			{   
			  return(D26_ERROR);
			} 
			break;
  case D2_MHS_PR_DM_STX:
  case D2_PR_DM_STX:    if ((len % sizeof(int)) != 0)
			{   
			  return(D26_ERROR);
			} 
			len /= sizeof(int);
			break;
  case D2_BOOLEAN:      if ((len % sizeof(Bool)) != 0)
			{   
			  return(D26_ERROR);
			} 
			break;
  case D2_POST_AD_STX:  max_ubound = at->d26a_ubound +
				     2 * D26_UB_POSTAL_LINE - 1;
			break;
  case D2_TLXNR_STX:    max_ubound =
				   at->d26a_ubound + D26_NR_COMP_TELEX_NR -1;
			break;
  default:                          /*  otherwise                     */
			break;
}

if                                  /*  length not correct            */
   (len < (signed16)at->d26a_lbound || len > (signed16)max_ubound)
{                                   /*  return value = D26_ERROR      */
  return(D26_ERROR);
}
else
{
  return(D2_NOERROR);
}                                   /*                                */

}                                   /*  d26_u96_attr_len_check()      */


/*exon ****************************************************************/
/*                                                                    */
/* TYP          : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_u98_attr_val_cmp()                              */
/*                                                                    */
/* AUTHOR       : W. Schmid, D AP 11                                  */
/* DATE         : 15.04.91                                            */
/*                                                                    */
/* DESCRIPTION  : This function will compare two attribute values.    */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*                   Octet      *val1;      first attribute value     */
/*                   Ushort      len1;      first attribute length    */
/*                   Octet      *val2;      second attribute value    */
/*                   Ushort      len2;      second attribute length   */
/*                   Short       syntax;    attribute syntax          */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*                None                                                */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*                  D2_TRUE:    attributes are equal                  */
/*                  D2_FALSE:   attributes are different              */
/*                                                                    */
/* STANDARDHEADER  :                                                  */
/*                                                                    */
/* GLOBAL  DATA  (read):                                              */
/*                None                                                */
/*                                                                    */
/* GLOBAL  DATA  (changed):                                           */
/*                None                                                */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*      d26_un1_norm_or_address()                                     */
/*      d26_un2_norm_or_name()                                        */
/*      d26_un3_norm_submit_perm()                                    */
/*                                                                    */
/* REMARKS:                                                           */
/*               Nothing                                              */
/*                                                                    */
/*exoff ***************************************************************/

boolean d26_u98_attr_val_cmp
(
  byte       *val1,
  signed16    len1,
  byte       *val2,
  signed16    len2,
  signed16    syntax
)

{                                   /*  d26_u98_attr_val_cmp()        */

/**********************************************************************/
/*                                                                    */
/*                 T Y P  -  D E F I N I T I O N E N                  */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E K L A R A T I O N E N                         */
/*                                                                    */
/**********************************************************************/

  signed32  normed_val1[D26_MAXVALLEN / sizeof(signed32)];
  signed32  normed_val2[D26_MAXVALLEN / sizeof(signed32)];
  signed16  n_len1;                 /* length of normed value         */
  signed16  n_len2;                 /* length of normed value         */

  boolean   ret;

  switch ((int) syntax)
  {
    case D2_DISTNAME:               /*  Distinguished name            */
      if                          /*  attribute lengths are different */
	 (len1 != len2)
      {                           /*  attributes are different        */
	ret = FALSE;
      }
      else
      {                           /*  normalize values                */
	d27_007_norm_name((char *) val1, len1, (char *) normed_val1);
	d27_007_norm_name((char *) val2, len2, (char *) normed_val2);
	ret = (memcmp(normed_val1, normed_val2, (size_t) len1) == 0)
	      ? TRUE : FALSE;
      }               /*                                */
      break;
  case D2_ACCL:                     /*  Acccess control list          */
    if                            /*  attribute lengths are different */
       (len1 != len2)
    {                             /* attributes are different         */
      ret = FALSE;
    }
    else
    {                             /* normalize values                 */
      d26_uso_norm_acl(val1,(byte *)normed_val1);
      d26_uso_norm_acl(val2,(byte *)normed_val2);
      ret = (memcmp(normed_val1, normed_val2, (size_t) len1) == 0)
	      ? TRUE : FALSE;
    }
    break;
  case D2_CIGNORE:                  /*  Case ignore string            */
    if                            /*  attribute lengths are different */
       (len1 != len2)
    {                               /*  attributes are different      */
      ret = FALSE;
    }
    else
    {                               /*  normalize values              */
      d27_651_norm_T61String(val1,(byte *)normed_val1,len1);
      d27_651_norm_T61String(val2,(byte *)normed_val2,len2);
      ret = (memcmp((char *)normed_val1,(char *)normed_val2,
	(size_t)len1) == 0) ? TRUE : FALSE;
    }
    break;
  case D2_CIGN_LIST:                /*  Case ignore list (only for DSA) */
  case D2_POST_AD_STX:
    if                            /*  attribute lengths are different */
       (len1 != len2)
    {                             /* attributes are different         */
      ret = FALSE;
    }
    else
    {                             /* normalize values                 */
      d26_usp_norm_ci_list(val1,(byte *)normed_val1,len1);
      d26_usp_norm_ci_list(val2,(byte *)normed_val2,len2);
      ret = (memcmp((char *)normed_val1,(char *)normed_val2,
	(size_t)len1) == 0) ? TRUE : FALSE;
    }
    break;
  case D2_COUNTRY_STX:              /*  Country-Syntax                */
  case D2_IA5:
    if                            /*  attribute lengths are different */
      (len1 != len2)
    {                               /*  attributes are different      */
      ret = FALSE;
    }
    else
    {                               /*  normalize values              */
      d27_031_norm_pr_string(val1,(byte *)normed_val1,len1);
      d27_031_norm_pr_string(val2,(byte *)normed_val2,len2);
      ret = (memcmp((char *)normed_val1,(char *)normed_val2,
		(size_t)len1) == 0) ? TRUE : FALSE;
    }
    break;
  case D2_NUMERIC:
  case D2_TELNR_STX:
    n_len1 = d26_u28_norm_numbers((char *) val1, len1,(char *)normed_val1);
    n_len2 = d26_u28_norm_numbers((char *) val2, len2,(char *)normed_val2);
    if                          /* attribute lengths are different    */
       (n_len1 != n_len2)
    {                           /* attributes are different           */
      ret = FALSE;
    }
    else
    {                           /* compare normalized values          */
      ret = (memcmp((char *)normed_val1,(char *)normed_val2, (size_t)n_len1)
		    == 0) ? TRUE : FALSE;
    }
    break;
  case D2_TIME:
    d26_us9_norm_utc_time((char *) val1,(char *)normed_val1);
    d26_us9_norm_utc_time((char *) val2,(char *)normed_val2);
    ret = (memcmp((char *)normed_val1,(char *)normed_val2,(size_t)len1)
		  == 0) ? TRUE : FALSE;
    break;
  case D2_CEXACT:                   /*  Case exact  string            */
  case D2_PRINTABLE:
  case D2_TLXNR_STX:
  case D2_OCTET:
  case D2_TTXID_STX:
  case D2_FAXNR_STX:
  case D2_PASSWD_STX:
  case D2_OBJ_IDENT:
  case D2_BOOLEAN:
  case D2_INTEGER:
  case D2_MHS_PR_DM_STX:
    if                              /* attribute lengths are different */
       (len1 != len2)
    {                               /* attributes are different        */
      ret = FALSE;
    }
    else
    {                               /* normalize values                */
      ret = (memcmp(val1, val2, (size_t) len1) == 0) ? TRUE : FALSE;
    }
    break;
  case D2_MHS_DLSP_STX:
    n_len1 = d26_un3_norm_submit_perm((char *)val1,len1,(char *)normed_val1);
    n_len2 = d26_un3_norm_submit_perm((char *)val2,len2,(char *)normed_val2);
    if                              /* attribute lengths are different */
       (n_len1 != n_len2)
    {                               /*  attributes are different      */
      ret = FALSE;
    }
    else
    {                               /* compare values                  */
      ret = (memcmp((char *)normed_val1,(char *)normed_val2,(size_t)n_len1)
	     == 0) ? TRUE : FALSE;
    }                                /*                                */
    break;
  case D2_MHS_ORADR_STX:
    n_len1 = d26_un1_norm_or_address((char *)val1,len1,(char *)normed_val1);
    n_len2 = d26_un1_norm_or_address((char *)val2,len2,(char *)normed_val2);
    if                              /* attribute lengths are different */
       (n_len1 != n_len2)
    {                               /* attributes are different        */
      ret = FALSE;
    }
    else
    {                               /* compare values                  */
      ret = (memcmp((char *)normed_val1,(char *)normed_val2,(size_t)n_len1)
	    == 0) ? TRUE : FALSE;
    }                               /*                                 */
    break;
  case D2_MHS_ORNAME_STX:
    n_len1 = d26_un2_norm_or_name((char *)val1,len1,(char *)normed_val1);
    n_len2 = d26_un2_norm_or_name((char *)val2,len2,(char *)normed_val2);
    if                              /* attribute lengths are different */
       (n_len1 != n_len2)
    {                               /*  attributes are different       */
      ret = FALSE;
    }
    else
    {                               /* compare values                  */
      ret = (memcmp(val1, val2, (size_t)n_len1) == 0)
	    ? TRUE : FALSE;
    }                               /*                                 */
    break;
  case D2_ANY_STX:
  case D2_ASN1:
  case D2_GUIDE:
  case D2_PR_DM_STX:
  case D2_CERTIFICATE:
  case D2_CERT_PAIR:
  case D2_CERT_LIST:
    ret = FALSE;
    break;
  case D2_PSAP_STX:
  default:                          /*  otherwise                     */
    ret = TRUE;
    break;
}
return(ret);

}                                   /*  d26_u98_attr_val_cmp()        */


/*exon ****************************************************************/
/*                                                                    */
/* TYP          : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_u99_handle_blanks(...)                          */
/*                                                                    */
/* AUTHOR       : W. Schmid, D AP 11                                  */
/* DATE         : 30.08.88                                            */
/*                                                                    */
/* DESCRIPTION  : This function will remove blanks in the attribute   */
/*                value when necessary.                               */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*                   D26_at     *at;    pointer to AT                 */
/*                   char       *val;   attribute value               */
/*                   Ushort      len;   attribute length              */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*                None                                                */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*                len:                  length of attribute value     */
/*                                      after removing blanks         */
/*                                                                    */
/* STANDARDHEADER  :                                                  */
/*                                                                    */
/* GLOBAL  DATA  (read):                                              */
/*                None                                                */
/*                                                                    */
/* GLOBAL  DATA  (changed):                                           */
/*                None                                                */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*                      d27_012_remove_blanks()                       */
/*                                                                    */
/* REMARKS:                                                           */
/*               Nothing                                              */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_u99_handle_blanks
(
  D26_at     *at,
  char       *val,
  signed16    len
)

{                                   /*  d26_u99_handle_blanks()       */

/**********************************************************************/
/*                                                                    */
/*                 T Y P  -  D E F I N I T I O N E N                  */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E K L A R A T I O N E N                         */
/*                                                                    */
/**********************************************************************/


/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

switch ((int) at->d26a_syntax)      /*  attribute syntax is           */
{
  case D2_DISTNAME:                 /*  Distinguished name            */
  case D2_CEXACT:                   /*  Case exact  string            */
  case D2_CIGNORE:                  /*  Case ignore string            */
  case D2_ACCL:                     /*  Acccess control list          */
  case D2_COUNTRY_STX:              /*  Country-Syntax                */
  case D2_POST_AD_STX:
  case D2_PRINTABLE:
  case D2_NUMERIC:
  case D2_TELNR_STX:
  case D2_TLXNR_STX:
				    /*  eliminate blanks              */
		   len = d27_012_remove_blanks(val, len, val);
		   break;
  default:                          /*  otherwise                     */
		   break;
}

				    /*  return attribute length       */
return(len);

}                                   /*  d26_u99_handle_blanks()       */
