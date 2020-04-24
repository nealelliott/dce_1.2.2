/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d261util.c,v $
 * Revision 1.1.10.2  1996/02/18  19:44:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:25:43  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:42:03  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:06:00  root]
 * 
 * Revision 1.1.8.3  1994/07/06  15:06:47  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:12:36  marrek]
 * 
 * Revision 1.1.8.2  1994/05/10  15:51:39  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:18:49  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  16:04:55  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:07:14  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  13:26:58  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  12:58:47  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:58:23  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:17:38  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  19:31:54  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  11:01:45  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:44:06  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:52:20  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d261util.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:44:39 $";
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
/* NAME         : d261util.c <Utility-Functions for DSA-Interfaces>   */
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
/*   0.1    |08.12.87 |  Original                      | WS |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                        I N C L U D E S                             */
/*                                                                    */
/**********************************************************************/

#include <d26dsa.h>

/*****  Operating System - Includes  *****/

#include <stdlib.h>
#include <ctype.h>

/*****  external Includes  *****/

/*****  internal Includes  *****/

#include <dce/d27util.h>
#include <d26schema.h>

/**********************************************************************/
/*                                                                    */
/*                        D E F I N E S                               */
/*                                                                    */
/**********************************************************************/

/* for d26_u13_store_del_dns */
#define D26_U00_NO_MEM_DEL_DNS  50  /* memory portions which are      */
				    /* allocated for 'del_dns'        */

/* for d26_u05_acl_check */
#define D26_U01_MAXNO_CH_RIGHTS  2  /* maximum number of rights to    */
				    /* check                          */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*                                                                    */
/**********************************************************************/

/* for d26_u05_acl_check */
typedef struct {
	boolean   right_ok;              /* indicator whether right   */
					 /* is given                  */
	boolean   right_already_checked; /* indicator whether right   */
					 /* is checked                */
	} D26_right_info;

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u05_acl_check                                     */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        16.12.87                                              */
/*                                                                    */
/* DESCRIPTION: This function checks whether the requestor has the    */
/*              right given in the parameter right. When the given    */
/*              right is "modify" the function is ended when the first*/
/*              error occurs. When the given right is "read" the      */
/*              function returns for every attribute whether the      */
/*              requestor has the right.                              */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      D2_NO_ERR   signed16        access rights sufficient          */
/*                                                                    */
/*      D2_ACCESS_RIGHTS_INSUFFICIENT                                 */
/*                    access rights insufficient                      */
/*                    - when the parameter "right" was "D26_READ"     */
/*                      the parameter "attr_info[].d26_read_ok"       */
/*                      gives the attributes which are allowed to     */
/*                      read and which are not allowed to read        */
/*                    - when the parameter "right" was "D26_MODIFY"   */
/*                      the function is left when first error occurs  */
/*                                                                    */
/*      D26_ERROR     any other error occured e.g. parameter error    */
/*                    ((right != D26_READ) && (right != D26_MODIFY))  */
/*                    the function is left immediately                */
/*                    (such errors should not occur because this      */
/*                    are programmer errors or data inconsistencies,  */
/*                    e.g. at or dn list)                             */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_u05_acl_check(
			/* IN    -  object to be accessed             */
    D2_name_string entry,
			/* IN    -  access right to be checked ("read"*/
    D26_ch_right   right,           /* or "modify"                    */
			/* INOUT -  attribute types to be accessed    */
    D26_attr_info  *attr_info,
    D26_acl        *acl)/* IN    -  ACL of object to be accessed      */

{                                   /* d26_u05_acl_check              */

				    /* return value                   */
  signed16     return_value = D2_NO_ERR;
				    /* return value returned by called*/
  signed16     ret_val;             /* function                       */

		    /* array which indicates whether a particular     */
		    /* right is already checked and given             */
		    /* the index of the array indicatesthe right (s.  */
		    /* D26_aright)                                    */
		    /* the index D26_NOACCESS_RIGHTS indicates the    */
		    /* informations for READ_PUBLIC (always allowed!  */
		    /* ---> set to TRUE)                              */
  D26_right_info right_info[D2_NOACCESS_RIGHTS+1], *ptr_right_info;
  D26_attr_info  *ptr_attr_info;
  D26_acl        *ptr_acl_value;
  D26_dndesc     *ptr_dnlist_value;
  D2_name_string dn_from_dnlist;
  signed16       sup_length;        /* length of n. superior of entry */
  signed16       dnl_name_length;   /* length of name from dn list    */
  signed16       length;

  register signed16 i,j,k;          /* counter                        */
				    /* indicates whether a right is ..*/
  register boolean  already_checked = FALSE,  /* ... already checked  */
		    found = FALSE;            /* ... given or not     */

  register signed16 acl_index;
				    /* indicates whether only read    */
				    /* public attributes are to be    */
  register boolean  only_read_public;        /* checked               */
				    /* indicates whether the requestor*/
  register boolean  req_is_dsa;     /* is a DSA                       */

  register D26_ch_right check_right;/* right to check                 */

/*******************************************************/
/* exit function whith no_error when public attributes */
/* must be checked for read rights only                */
/*******************************************************/

		    /* check the requestor type (Master or not)       */
  if                        /* requestor is privileged                */
    (d26_requestor.d26_req_type == D26_PRIVIL_REQ)
  {                         /* set privileged flags                   */
    req_is_dsa = TRUE;
    only_read_public = TRUE;
  }
  else
  {                         /* set other flags                        */
    req_is_dsa       = FALSE;
    only_read_public = FALSE;
  }

  if                        /* read rights are to be checked          */
    (right == D26_READ)
  {                         /* check whether only read public         */
			    /* attributes are requested               */
    only_read_public = TRUE;
    i = 0;
    while                   /* still attributes given                 */
	 (attr_info[i].d26_at_idx != D26_EOL)
    {                       /* set pointer to attribute_info          */
      ptr_attr_info = &attr_info[i];
      if                    /* pseudo attribute encountered           */
	(ptr_attr_info->d26_at_idx == D26_IDX_NOT_DEF)
      {                     /* continue with next attribute           */
	ptr_attr_info->d26_read_ok = FALSE;
	i++;
	continue;
      }
      if                    /* first not read public attr found       */
	(d26_at[ptr_attr_info->d26_at_idx].d26a_acl != D26_PUBLIC &&
	 req_is_dsa == FALSE)
      {                     /* leave loop immediately                 */
	   only_read_public = FALSE;
	   break;
      }
      else
      {
	   ptr_attr_info->d26_read_ok = TRUE;
      }
      i++;
    }
  }

  if                        /* only public attr. are checked for read */
    (only_read_public == TRUE)
  {                         /* leave function successfull             */
    return(D2_NO_ERR);
  }

			    /* initialize variables                   */
  for (i=0; i<D2_NOACCESS_RIGHTS; i++)
  {
    ptr_right_info = &right_info[i];
    ptr_right_info->right_ok = ptr_right_info->right_already_checked = FALSE;
  }

  ptr_right_info = &right_info[D2_NOACCESS_RIGHTS];
  ptr_right_info->right_ok = ptr_right_info->right_already_checked = TRUE;

/*****************************************************/
/* verify the wished right for all given attributes  */
/*****************************************************/

  i = 0;
  while                     /* attributes given                       */
       (attr_info[i].d26_at_idx != D26_EOL &&
	(return_value == D2_NO_ERR && right == D26_MODIFY ||
	 right == D26_READ))
  {                         /* check the wished right                 */
    ptr_attr_info = &attr_info[i];
    if                      /* pseudo attribute encountered           */
      (ptr_attr_info->d26_at_idx == D26_IDX_NOT_DEF)
    {                       /* continue with next attribute           */
      ptr_attr_info->d26_read_ok = FALSE;
      i++;
      continue;
    }


    for                     /* all rights to check                    */
       (k = 0; k < D26_U01_MAXNO_CH_RIGHTS; k++)
    {
      if                    /* first check                            */
	(k == 0)
      {                     /* set right to check and found           */
	check_right = right;
	found = already_checked = FALSE;
      }
      else
      {                     /* check whether another right must be    */
			    /* checked also                           */
	if                  /* no other right must be checked         */
	  (found == TRUE || right == D26_MODIFY)
	{
	  break;
	}
	else
	{                   /* check modify rights also               */
	  check_right = D26_MODIFY;
	}
      }

			    /* set acl_index                          */
      switch(d26_at[ptr_attr_info->d26_at_idx].d26a_acl)
      {
	case D26_PUBLIC:
	  switch(right)
	  {
	    case D26_READ:
	      ptr_right_info = &right_info[D2_NOACCESS_RIGHTS];
	      break;
	    case D26_MODIFY:
	      ptr_right_info = &right_info[D2_MPUBLIC];
	      acl_index = D2_MPUBLIC;
	      break;
	    default:
	      dce_svc_printf(GDS_S_ILL_ACC_RIGHT_MSG,right);
	      return(D26_ERROR);
	  }
	  break;
	case D26_STANDARD:
	  switch(check_right)
	  {
	    case D26_READ:
	      ptr_right_info = &right_info[D2_RSTANDARD];
	      acl_index = D2_RSTANDARD;
	      break;
	    case D26_MODIFY:
	      ptr_right_info = &right_info[D2_MSTANDARD];
	      acl_index = D2_MSTANDARD;
	      break;
	    default:
	      dce_svc_printf(GDS_S_ILL_ACC_RIGHT_MSG,check_right);
	      return(D26_ERROR);
	  }
	  break;
	case D26_SENSITIVE:
	  switch(check_right)
	  {
	    case D26_READ:
	      ptr_right_info = &right_info[D2_RSENSITIVE];
	      acl_index = D2_RSENSITIVE;
	      break;
	    case D26_MODIFY:
	      ptr_right_info = &right_info[D2_MSENSITIVE];
	      acl_index = D2_MSENSITIVE;
	      break;
	    default:
	      dce_svc_printf(GDS_S_ILL_ACC_RIGHT_MSG,check_right);
	      return(D26_ERROR);
	  }
	  break;
	default:
	  dce_svc_printf(GDS_S_ILL_ACC_CLASS_MSG,
	    d26_at[ptr_attr_info->d26_at_idx].d26a_acl);
	  return(D26_ERROR);
      }

      /*************************/
      /* check acl             */
      /*************************/

	/* set already_checked for the particular right and attribute */
      already_checked = ptr_right_info->right_already_checked;
      for               /* all indices in acl or until right verified */
	 (j = 0;
	  j < D2_NOIDX_ACL && found == FALSE && already_checked == FALSE;
	  j++)
      {                 /* check acl                                  */
	ptr_acl_value = &acl[acl_index * D2_NOIDX_ACL + j];
	if              /* DN-list in referenced                      */
	  (*ptr_acl_value >= 0)
	{               /* check dn list in core                      */
	  ret_val = d26_u11_check_dn_index(*ptr_acl_value,&ptr_dnlist_value);
	  if            /* valid index to DN-list is given            */
	    (ptr_dnlist_value != (D26_dndesc *)NULL)
	  {             /* compare DNs (requestor vs DN from list)    */
	    dn_from_dnlist = ptr_dnlist_value->d26_dnname;
	    dnl_name_length = strlen((char *)dn_from_dnlist);
	    switch(ptr_dnlist_value->d26_dnint)
	    {
	      case D2_SOBJ:   /* single object                        */
			      /* name of requestor must be equal to   */
			      /* name from DN-list                    */
		if(strcmp(d26_requestor.d26_acl_name,
		   (char *)dn_from_dnlist) == 0)
		{
		  found = TRUE;
		}
		break;
	      case D2_SUBTREE:/* root of subtree                      */
	      case D2_GROUP:  /* group of objects                     */
			      /* name of requestor must be equal to   */
			      /* name from DN-list for the given      */
			      /* levels in DN-list name, that is for  */
			      /* strlen (name from DN-list)           */
		if(strncmp(d26_requestor.d26_acl_name,
		   (char *)dn_from_dnlist,dnl_name_length) == 0 &&
		   (d26_requestor.d26_acl_name[dnl_name_length]
		       == D2_RDN_SEP ||
		    d26_requestor.d26_acl_name[dnl_name_length]
		       == D2_EOS))
		{
		  found = TRUE;
		}
		break;
	      default:
		dce_svc_printf(GDS_S_ILL_DN_INTRPRTN_MSG,
		  ptr_dnlist_value->d26_dnint,dnl_name_length,
		  dn_from_dnlist);
		return(D26_ERROR);
	    }
	  }
	}
	else
	{               /* no index of DN-list                        */
	  switch (*ptr_acl_value)
	  {
	    case D26_OWNNAME:     /* own name                         */
				  /* name of requestor must be equal  */
				  /* to the entry name                */
	      if (strcmp(d26_requestor.d26_acl_name,(char *)entry)
		  == 0)
	      {
		found = TRUE;
	      }
	      break;
	    case D26_O_SUBTREE:   /* entry is root of subtree         */
				  /* name of requestor must be equal  */
				  /* to the entry name for the given  */
				  /* levels in entry_name, that is    */
				  /* for strlen (entry_name)          */
	      length = strlen ((char *)entry);
	      if (strncmp(d26_requestor.d26_acl_name,(char *)entry,
		   length) == 0 &&
		  (d26_requestor.d26_acl_name[length]
		   == D2_RDN_SEP ||
		   d26_requestor.d26_acl_name[length] == D2_EOS))
	      {
		found = TRUE;
	      }
	      break;
	    default:              /* otherwise                        */
	      if                  /* subtree level of superiors of    */
				  /* entry (e.g.: -3 father,          */
				  /* -4 grand father is root of       */
				  /* subtree) must be equal           */
				  /* to requestor                     */
		 (*ptr_acl_value < D26_O_SUBTREE &&
		  *ptr_acl_value > D26_EOL)
	      {                   /* calculate length of superior     */
		sup_length = d26_u08_calc_len(entry,*ptr_acl_value);
		if (sup_length > 0)
		{                 /* compare requestor with n. sup.   */
		  if (strncmp(d26_requestor.d26_acl_name,
			      (char *)entry,sup_length) == 0 &&
		      (d26_requestor.d26_acl_name[sup_length]
			      == D2_RDN_SEP ||
		       d26_requestor.d26_acl_name[sup_length]
			      == D2_EOS))
		  {
		    found = TRUE;
		  }
		}
		else
		{                 /* root object is root of subtree   */
		  found = TRUE;
		}
	      }
	      break;
	  }
	}
      }
      if (already_checked == TRUE)
      {   
	found = ptr_right_info->right_ok;
      }
      else
      {
	ptr_right_info->right_already_checked = TRUE;
	ptr_right_info->right_ok = found;
      } 
    }


   /****************************************/
   /* set output parameter for read rights */
   /****************************************/

    if                    /* read rights to check                     */
      (right == D26_READ)
    {                     /* set read ok flag                         */
      ptr_attr_info->d26_read_ok = found;
    }

   /***********************************************************/
   /* set return_value (in case of insufficient access right) */
   /***********************************************************/

    if                    /* insufficient access rights               */
      (found == FALSE)
    {                     /* set return_value                         */
      return_value = D2_ACCESS_RIGHTS_INSUFFICIENT;
    }

   /****************************/
   /* take next attribute      */
   /****************************/
    i++;

  }

  DCE_SVC_LOG((GDS_S_ACL_CHECK_MSG,return_value));
  return(return_value);

}                                   /* d26_u05_acl_check              */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u11_check_dn_index                                */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        5.1.88                                                */
/*                                                                    */
/* DESCRIPTION: This function checks whether the given index points   */
/*              to an valid entry in distinguished name list in core. */
/*              When a valid index is given the function returns the  */
/*              pointer to the distinguished name list entry.         */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*       D2_NO_ERR signed16         no error occurred                 */
/*                    (dnlist_ptr contains NULL pointer when index    */
/*                     was < 0 or dnlist in core or file is           */
/*                     inconsistent or pointer to dn list entry in    */
/*                     core)                                          */
/*                                                                    */
/*       D26_ERROR    error occured by reading a new distinguished    */
/*                    name list (e.g. C ISAM error, file system is    */
/*                    inconsistent, not enough memory for dn list in  */
/*                    in core; dnlist_ptr contains NULL pointer)      */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_u11_check_dn_index(
    signed32   index,   /* IN    -  index of DN in core               */
			/* OUT   -  pointer to DN-list entry          */
    D26_dndesc **dnlist_ptr)

{                               /* d26_u11_check_dn_index             */

  signed16 return_value = D2_NO_ERR;/* return value                   */
  boolean  call_upd_core_dnlist;

  *dnlist_ptr = (D26_dndesc *) NULL;/* initialize dnlist_ptr          */

/*********************/
/* check index < 0   */
/*********************/

  if                    /* index valid                                */
    (index >= 0L)
  {                     /* initialize DN-list update flag             */
    call_upd_core_dnlist = FALSE;

/*****************************************************/
/* check whether the dn list in core must be updated */
/*****************************************************/

    if                  /* index greater than maximum number of       */
      (index > d26_maxidx_dnlist)     /* distinguished names in core    */
    {                   /* dn list in core must be updated            */
      call_upd_core_dnlist = TRUE;
    }
    else
    {                   /* set pointer to dn list in core             */
      *dnlist_ptr = &d26_dnlist[index];
      if                /* number of references to DN == 0            */
	((*dnlist_ptr)->d26_occurrency <= 0)
      {                 /* dn list in core must be updated            */
	call_upd_core_dnlist = TRUE;
      }
    }

/**************************/
/* update dn list in core */
/**************************/

    if                  /* DN list in core must be updated            */
      (call_upd_core_dnlist == TRUE)
    {                   /* call function for update                   */
      d26_upd_info.d26_version = 0L;

      switch (d26_i01_upd_core_dnlist())
      {
	case D26_CHANGED: /* DN list in core was changed              */
	  if              /* index greater than max no of             */
			  /* distinguished names in core              */
	    (index > d26_maxidx_dnlist)
	  {               /* NULL pointer will be returned            */
	    *dnlist_ptr = (D26_dndesc *)NULL;
	  }
	  else
	  {               /* set ptr to dn list                       */
	    *dnlist_ptr = &d26_dnlist[index];
	    if            /* no of references to DN == 0              */
	      ((*dnlist_ptr)->d26_occurrency <= 0L)
	    {             /* NULL pointer will be returned            */
	      *dnlist_ptr = (D26_dndesc *) NULL;
	    }
	  }
	  break;
	case D26_NO_CHANGE: /* Dn list in core wasn't updated         */
	  *dnlist_ptr = (D26_dndesc *) NULL;
	  break;
	case D26_ERROR:   /* error occurred during update of Dn list  */
	default:
	  *dnlist_ptr = (D26_dndesc *) NULL;
	  return_value = D26_ERROR;
	  break;
      }
    }
  }

  if                    /* no valid index given                       */
    (*dnlist_ptr == (D26_dndesc *) NULL)
  {                     /* log data inconsistency                     */
    dce_svc_printf(GDS_S_ILL_DN_INDEX_MSG,index);
  }

  return (return_value);

}                                   /* d26_u11_check_dn_index         */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u08_calc_len                                      */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        26.01.88                                              */
/*                                                                    */
/* DESCRIPTION: This function calculates the length for the wished    */
/*              level. The level is the number of nameparts which     */
/*              should be "cut off" from end of name                  */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*     signed16  >= 0       length of the name for subtree level of   */
/*                          superior's (father, grand father etc.)    */
/*                          (name[length-1] is the the last valid     */
/*                           character of superior name - next        */
/*                           character will be EOS or D26_DL1 or the  */
/*                           first character of the name when all     */
/*                           nameparts should "cut off" - then        */
/*                           return_value == 0)                       */
/*                          to cut off the name really the caller     */
/*                          must copy EOS to name[length]             */
/*                                                                    */
/*            D26_ERROR     error occurred                            */
/*            (< 0)         (wrong value for level received)          */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_u08_calc_len(
			/* IN    -  object name, for which the length */
    D2_name_string name,/*          should be calculated              */
			/* IN    -  subtree level (acl_value !)       */
    signed16       level)

{                               /*  d26_u08_calc_len                  */

  signed16 return_length;       /* calculated length                  */
  signed16 length;              /* length of name                     */

  register signed16 i;          /* counter                            */
				/* max number of searched delimiters  */
  register signed16 max_no_delimiters;
				/* found number of delimiters         */
  register signed16 found_no_delimiters;
				/* no of nameparts by which name      */
				/* should be "shortened" from end of  */
				/* name to get the superior           */
  register signed16 no_nameparts;

  length = strlen((char *)name);/* calculate strlen of name           */


/**************************************************/
/* calculate no_nameparts and searched delimiters */
/**************************************************/
				/* calculate searched no of delimiters*/
				/* (max_no_delimiters)                */
  if (level <= D26_O_SUBTREE && level > D26_EOL)
  {
    no_nameparts = D26_O_SUBTREE - level;
  }
  else
  {
    no_nameparts = level;
  }

  max_no_delimiters = no_nameparts; /* */

/*****************/
/* special cases */
/*****************/

  if                         /*  no namepart should be "cut off"      */
			     /*  or length of name == 0               */
    (no_nameparts == 0 || length == 0)
  {                          /*  exit function with strlen (name)     */
    return (length);
  }



/*********************/
/* search delimiters */
/*********************/
			     /*  search delimiters                    */
  for (i = length, found_no_delimiters = 0;
       (i >= 0 && found_no_delimiters < max_no_delimiters);
       i--)
  {
    if (name[i] == D2_RDN_SEP)
    {
      found_no_delimiters++;
    }
  }

/***************************/
/* calculate return_length */
/***************************/
				/*  set return_length                 */
  if (i > 0)
  {
    return_length = i + 1;
  }
  else
  {
    return_length = 0;
  }


  return (return_length);       /*  return calculated length          */

}                               /*  d26_u08_calc_len                  */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u12_get_dnlidx                                    */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        03.03.88                                              */
/*                                                                    */
/* DESCRIPTION: This function searches a given dnint and dn in the    */
/*              dn list in core. If the dn is found and the entry     */
/*              in the list is a valid one (occurrency > 0) the index */
/*              value is returned else an index value < 0 is returned.*/
/*                                                                    */
/* RETURNVALUE:                                                       */
/*     signed32  >= 0     index to dn list in core                    */
/*               < 0      dnint and dn not found                      */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d26_u12_get_dnlidx (
			/* IN    -  Interpretation of dist. name      */
    D2_dnint       dnint,
    D2_name_string dn)  /* IN    -  normed distinguished name         */

{                               /*  d26_u12_get_dnlidx                */

  signed32 dnl_idx = D26_EOL;   /* return value: index to dnl in core */

  register signed32     i;      /* counter                            */
  register D26_dndesc   *ptr_dnlist;

  for (i = 0; i <= d26_maxidx_dnlist; i++)
  {
    ptr_dnlist = &d26_dnlist[i];
    if (dnint == ptr_dnlist->d26_dnint &&
	strcmp((char *)dn,(char *)ptr_dnlist->d26_dnname) == 0 &&
	ptr_dnlist->d26_occurrency > 0L)
    {
      dnl_idx = i;
      break;
    }
  }

  return (dnl_idx);

}                               /*  d26_u12_get_dnlidx                */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u13_store_del_dns                                 */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        26.02.88                                              */
/*                                                                    */
/* DESCRIPTION: This function appends an index to dn list in core     */
/*              to the list of indices which should be 'deleted'      */
/*              from the list during update of main entry.            */
/*                                                                    */
/*              The indices are 'deleted' by the function             */
/*              d26_C07_del_dns.                                      */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*     signed16 D2_NO_ERR    no error occurred (dnl_idx was appended  */
/*                           to list                                  */
/*                                                                    */
/*              D26_ERROR    error occurred by malloc or realloc      */
/*                           (error apdu and length are suppied)      */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_u13_store_del_dns(
			/* IN    -  index value which should be       */
    signed32    dnl_idx,/*          appended to list                  */
			/* INOUT -  list of dnl index values shich    */
			/*          should be deleted from dnl in core*/
    signed32    **del_dns,
			/* INOUT -  number of dnl index values in     */
    signed16    *no_del_dns,        /* del_dns                        */
			/* INOUT -  memory is available for           */
			/*          no_mem_del_dns index values in    */
    signed16    *no_mem_del_dns,    /* del_dns                        */
			/* INOUT -  is set to TRUE to indicate that dn*/
    boolean     *upd_dnl_file,      /* list in file must be updated   */
    D23_rserror *error, /* OUT   -  error message                     */
    signed32    *l_err) /* OUT   -  length of error message           */

{                               /*  d26_u13_store_del_dns             */

  signed32 *new_mem;
  signed16 return_value = D2_NO_ERR;
  size_t   mem_size;

/************************************/
/* check and/or allocate new memory */
/************************************/

  *no_del_dns = *no_del_dns + 1;
  if                    /*  new list                                  */
   (*del_dns == NULL)
  {                     /*  allocate new memory                       */
    *no_mem_del_dns = D26_U00_NO_MEM_DEL_DNS;
    *del_dns = (signed32 *)D27_CALLOC(svc_c_sev_warning,
      (size_t)*no_mem_del_dns,sizeof(signed32));
  }
  else
  {                     /*  check available memory                    */
    if                  /*  not enough memory available               */
      (*no_del_dns > *no_mem_del_dns)
    {                   /*  reallocate memory                         */
      *no_mem_del_dns = *no_mem_del_dns + D26_U00_NO_MEM_DEL_DNS;
      mem_size = *no_mem_del_dns * sizeof(signed32);
      new_mem = (signed32 *)D27_REALLOC(svc_c_sev_warning,(void *)*del_dns,
	mem_size);
      if                 /* realloc succeeds                           */
	 (new_mem != NULL)
      {                  /* set pointer to new memory                  */
	   *del_dns = new_mem;
      }
      else
      {                  /* realloc fails                              */
	free(*del_dns);
	*del_dns = NULL;
      }
    }
  }

/******************/
/* append dnl_idx */
/******************/
  if                    /*  memory available                          */
    (*del_dns != NULL)
  {                     /*  append dnl_idx                            */
    *(*del_dns + *no_del_dns - 1) = dnl_idx;
			/*  set indicator for update dnl in file      */
    *upd_dnl_file = TRUE;
  }
  else
  {                     /*  set error apdu and length of apdu         */
    d27_010_set_error_apdu(error,D2_TEMP_ERR,D2_REM_ERR,D2_NO_RESPONSE,
      l_err);
			/*  reset variables                           */
    *no_del_dns = *no_mem_del_dns = 0;
			/*  set return value                          */
    return_value = D26_ERROR;
  }

  return(return_value);

}                               /*  d26_u13_store_del_dns             */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u79_get_sstr_dnlidx                               */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        14.04.88                                              */
/*                                                                    */
/* DESCRIPTION: This function searches the a given sustring from the  */
/*              begin of dnint and dn in the dn list in core. If the  */
/*              is found and the entry in the list is a valid one the */
/*              in the list is a valid one (occurrency > 0) the index */
/*              value is returned else an index value < 0 is returned.*/
/*                                                                    */
/* RETURNVALUE:                                                       */
/*     signed32 >= 0    index to dn list in core                      */
/*              < 0     dnint and dn not found                        */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d26_u79_get_sstr_dnlidx(
			/* IN    -  the start index where the search  */
    signed32       begin_idx,       /* begins                         */
			/* IN    -  distinguished name interpretation */
    D2_dnint       dnint,
    D2_name_string dn,  /* IN    -  normed distinguished name         */
			/* IN    -  length of distinguished name      */
    signed16       cmp_len)

{                                   /*  d26_u79_get_sstr_dnlidx       */

			/* return value: index to dnl in core         */
  signed32 dnl_idx = D26_EOL;

  register signed32     i;          /* counter                        */
  register D26_dndesc   *ptr_dnlist;

  for (i = begin_idx; i <= d26_maxidx_dnlist; i++)
  {
    ptr_dnlist = &d26_dnlist[i];
    if (dnint == ptr_dnlist->d26_dnint &&
	memcmp(dn,ptr_dnlist->d26_dnname,cmp_len) == 0 &&
	ptr_dnlist->d26_occurrency > 0L)
    {
      dnl_idx = i;
      break;
    }
  }

  return (dnl_idx);
}                                   /*  d26_u79_get_sstr_dnlidx       */
