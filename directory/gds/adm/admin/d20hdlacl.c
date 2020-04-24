/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20hdlacl.c,v $
 * Revision 1.1.10.2  1996/02/18  19:40:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:21:43  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:16:54  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:54  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:05:21  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:09:13  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:58:32  marrek
 * 	Update for dce1.1.
 * 	[1994/02/09  14:47:40  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:29:11  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  10:06:35  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:29:15  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:08:39  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:04:20  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  13:12:06  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  14:09:17  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:55:20  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:16:04  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20hdlacl.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:40:52 $";
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
/* NAME         : hdlacl.c    [HANDLE_ACL]                            */
/*								      */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 04.02.88                                            */
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
/*      0.1 | 04.02.88| original                       | ws |         */
/*								      */
/*datoff **************************************************************/

#include <gds.h>
#include <stdio.h>
#include <string.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d2hdlacl.h>
#include <d20proto.h>


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_5_handle_acl()        [handle_acl  ]              */
/*								      */
/* AUTHOR:      Schmid,  D AP 11                                      */
/* DATE:        25.02.88                                              */
/*								      */
/* SYNTAX:      signed16 d20_5_handle_acl(parblk)                     */
/*              D20_p5  *parblk;                                      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will work on the current ACL attribute. */
/*              There are different possibilities to handle the ACL:  */
/*              (for each attribute seperately according to           */
/*              "d20_5mod")                                           */
/*                  - "READ" DN:    lists the Distinguished Names in  */
/*                                  the ACL attribute                 */
/*                  - "MOD"  DN:    modifies Distinguished Names in   */
/*                                  the ACL attribute                 */
/*              In the last case the new ACL attribute value          */
/*              is returned in the given memory area. The actual      */
/*              length of the new ACL is returned too.                */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              D20_p5  *parblk;                                      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              None                                                  */
/*								      */
/* RETURNVALUE:							      */
/*              D2_NOERROR:         function successfully terminated  */
/*              D20_ACC_ILLEGAL:    illegal access right              */
/*              D20_MOD_ILLEGAL:    illegal modification mode         */
/*								      */
/* STANDARD-HEADER: None                                              */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                          strlen()                                  */
/*                          strcpy()                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/*exoff ***************************************************************/

signed16 d20_5_handle_acl(D20_p5 *parblk)
{                               /*  d20_5_handle_acl                  */
   signed16 i;
   signed16 length;
   Bool     update_flag = FALSE;

					/* DN-info in current ACL     */
   D20_obj  obj[D2_ACLIDX_MAX];
   D20_obj *p_obj = &obj[0];
   D20_obj *p_max_obj = p_obj + D2_ACLIDX_MAX;

   D20_aclinfo *aclinfo = parblk->d20_5acl;
   D20_obj *dns;
   D20_obj *max_dns;
   D20_obj *last_dns;

   char     empty_dn = D2_EOS;

					/* current ACL                */
   char    *acl_attr = parblk->d20_5c_acl;
   char    *max_acl = acl_attr + parblk->d20_5len;



if                              /*  length of ACL is zero             */
   (parblk->d20_5len == 0)
{                               /*  initialize ACL with d2_IGNORE     */
  for ( ; p_obj < p_max_obj; p_obj++)
  { 
    p_obj->dn_int = D2_IGNORE;
    p_obj->dn = (D2_name_string)&empty_dn;
  } 
}
else
{
  for                           /*  all ACL-names                     */
      ( ; p_obj < p_max_obj && acl_attr < max_acl; p_obj++)
  {                             
    if                          /*  ACL-interpretation is not correct */
       (*acl_attr != D2_SOBJ  && *acl_attr != D2_SUBTREE &&
	*acl_attr != D2_GROUP && *acl_attr != D2_IGNORE)
    {                           /*  return(D20_ACL_WRONG)             */
      return(D20_ACL_WRONG);
    }
else
{                           /*  store ACL-interpretation          */
      p_obj->dn_int = *acl_attr++;
				/*  store ACL-name                    */
      acl_attr += (strlen((char *)(p_obj->dn = (D2_name_string)acl_attr)) + 1); 
							/* incl. EOS    */
    }                           
  }                             
  if                            /*  ACL-attribute is not correct      */
     (p_obj != p_max_obj || acl_attr != max_acl)
  {                             /*  return(D20_ACL_WRONG)             */
    return(D20_ACL_WRONG);
  }                             
}                               

for                             /*  all given access rights           */
    (i = 0; i < parblk->d20_5no_rights; i++, aclinfo++)
{                               
  if                            /*  illegal access right              */
     ((p_obj = &obj[0] + aclinfo->d20_5aright * D2_NOIDX_ACL) < &obj[0]
      || p_obj > p_max_obj)
  {                             /*  return(D20_ACC_ILLEGAL)           */
    return(D20_ACC_ILLEGAL);
  }                             
  dns = &(aclinfo->d20_5dns[0]);
  switch ((int) aclinfo->d20_5mod)  /*  modification type             */
  {
    case D2_MOD_DN:             /*  MODIFY DNs                        */
			update_flag = TRUE;
			if      /*  number of DNs wrong               */
			   (aclinfo->d20_5dn_count <= 0 ||
			    aclinfo->d20_5dn_count > D2_NOIDX_ACL)
			{       /*  return(D20_TOO_MANY)              */
			  return(D20_TOO_MANY);
			}       
			for     /*  all DNs                           */
			    (max_dns = p_obj + aclinfo->d20_5dn_count,
			     last_dns = p_obj + D2_NOIDX_ACL;
			     p_obj < max_dns; p_obj++, dns++)
			{       /*  set pointer to name               */
			  p_obj->dn = dns->dn;
			  p_obj->dn_int = dns->dn_int;
			}       
			for     /*  all remaining positions in ACL    */
			    ( ; p_obj < last_dns; p_obj++)
			{       /*  set pointer to (empty) name       */
			  p_obj->dn     = (D2_name_string) &empty_dn;
			  p_obj->dn_int = D2_IGNORE;
			}       
			break;
    case D2_READ_DN:            /*  READ DNs                          */
			for     /*  all DNs                           */
			    (max_dns = p_obj + D2_NOIDX_ACL; p_obj < max_dns;
			     p_obj++, dns++)
			{       /*  set pointer to name               */
			  dns->dn     = p_obj->dn;
			  dns->dn_int = p_obj->dn_int;
			}       
			aclinfo->d20_5dn_count = D2_NOIDX_ACL;
			break;
    default:                    /*  otherwise                         */
				/*  return(ILLEGAL MODIFICATION TYPE) */
			return(D20_MOD_ILLEGAL);
  }                             
}                               

parblk->d20_5act_len = 0;
if                              /*  ACL must be updated               */
   (update_flag == TRUE)
{                               /*  build new ACL                     */
  for                           /*  all new DNs                       */
      (p_obj = &obj[0], acl_attr = parblk->d20_5n_acl; p_obj < p_max_obj;
       p_obj++)
  {                             /*  determine length of DN            */
				/*  (incl. EOS and interpretation)    */
    parblk->d20_5act_len += ((length = strlen((char *)p_obj->dn)) + 2);
    if                          /*  memory exceeded                   */
       (parblk->d20_5act_len > parblk->d20_5max_len)
    {                           /*  return(D20_TOO_MANY)              */
      return(D20_TOO_MANY);
    }
    else
    {                           /*  put DN in ACL                     */
      *acl_attr++ = p_obj->dn_int;
      strcpy(acl_attr, (char *)p_obj->dn);
      acl_attr += length;
      *acl_attr++ = D2_EOS;
    }                           
  }                             
}
else
{                               /*  don't return new ACL              */
  *(parblk->d20_5n_acl) = D2_EOS;
}                               

				/*  return(D2_NOERROR)                */
return(D2_NOERROR);

}                               /*  d20_5_handle_acl                  */
