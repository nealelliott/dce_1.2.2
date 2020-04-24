/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d262dnlis.c,v $
 * Revision 1.1.10.2  1996/02/18  19:44:43  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:25:50  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:42:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:06:08  root]
 * 
 * Revision 1.1.8.3  1994/06/21  14:45:06  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  12:21:03  marrek]
 * 
 * Revision 1.1.8.2  1994/05/10  15:51:43  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:19:12  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  16:05:02  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:07:57  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  13:32:22  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:00:20  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:58:39  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:17:57  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:44:47  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:52:44  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d262dnlis.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:44:43 $";
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
/* NAME         : d262dnlis.c                                          */
/*                                                                    */
/* AUTHOR       : Schreck, D AP 11                                    */
/* DATE         : 29.03.89                                            */
/*                                                                    */
/* COMPONENT    : DS                                                  */
/*                                                                    */
/* DOC.-NR.     : DS-Design-Spec.                                     */
/*                      .                                             */
/*                      .                                             */
/*                                                                    */
/* PRD#/VERS.   : <Productnr. from Productstructure - <Versionnr.>    */
/*                                                                    */
/* DESCRIPTION  : This module contains the functions to get dn or     */
/*                index of dn in dnlist reading record from file.     */
/*                This functions are used by the daemon process of    */
/*                delta updates.                                      */
/*                                                                    */
/* SYSTEM DEPENDENCIES: SINIX C-ISAM                                  */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       | ID | CR# EM# */
/*   0.1    |23.03.89 | Original                       | as |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                        I N C L U D E S                             */
/*                                                                    */
/**********************************************************************/

#include <d26dsa.h>

/*****  Operating-System-Includes  *****/

#include <stdio.h>
#include <isam.h>

/*****  external Includes    *****/

#include <dce/d27util.h>

/*****  internal Includes    *****/

#include <d2delupd.h>
#include <d26dnlis.h>

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

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*                                                                    */
/**********************************************************************/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i26_get_idx_from_file                             */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        29.03.89                                              */
/*                                                                    */
/* DESCRIPTION: This function opens the dnlist file (when necessary)  */
/*              and read the record with given dn. When no valid      */
/*              record exists a new record is created. The            */
/*              corresponding index of dnlist in core is given to     */
/*              the caller.                                           */
/*                                                                    */
/*              When last_operation is D2_TRUE the file is closed     */
/*              at the end of the function.                           */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      Bool              last_operation   indicates whether function */
/*                                         will be called later       */
/*                                D2_TRUE  last call                  */
/*                                         dnlist file is closed only */
/*                                         in this case the other     */
/*                                         parameters are ignored     */
/*                                         no index is returnedd      */
/*                                D2_FALSE not last call              */
/*                                         dnlist file is kept open   */
/*                                                                    */
/*      D2_name_string    dn               distinguished name         */
/*                                         (normed value, as inter-   */
/*                                          pretation of dn D2_SOBJ   */
/*                                          is assumed)               */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      Long              *dnlist_idx      index of given dn in core  */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*    Short   D2_NO_ERR          no error occurred                    */
/*                                                                    */
/*            D26_ERROR          C isam error occurred                */
/*                               or max. value for dnl. index exceeded*/
/*                               (== D26_MAX_DNLIDX)                  */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d26_i26_get_idx_from_file (boolean last_operation,
    D2_name_string dn, signed32 *dnlist_idx)

{                               /* d26_i26_get_idx_from_file          */

				/* record buffer of isam file         */
  char dnlistrec[D26_I02_RECDNLISTL];

  char *dnintptr,               /* ptr to dn interpretation           */
       *dnptr,                  /* ptr to dn name                     */
       *ptroccurrency;          /* ptr to occurrency                  */

  signed32 return_value = D2_NO_ERR;
  D2_dnint dnint = D2_SOBJ;

  register signed16 rcode;      /* return code of called functions    */
  register signed32 occurrency;
				/* indicator whether the dn must be   */
  register boolean  add_dn;     /* added to dnlist                    */

  register signed32  max_short;

if (last_operation == TRUE)     /*  file should be closed only          */
{                               /*  close file                          */
     rcode = d26_i06_close_dnlistfile ();
     if (rcode != D2_NO_ERR)
     {   
	  return_value = D26_ERROR;
     } 
     return (return_value);
}

/*******************/
/* open file       */
/*******************/
if                              /* open dnlist file fails             */
  (d26_i05_open_dnlistfile () == D26_ERROR)
{                               /* return error                       */
     return (D26_ERROR);
}

/************************/
/* initialize variables */
/************************/
dnintptr = &dnlistrec[D26_I10_DNINT];
dnptr    = &dnlistrec[D26_I11_DNNAME];
ptroccurrency = &dnlistrec[D26_I12_OCCURRENCY];
add_dn = FALSE;
*dnlist_idx = D26_IDX_NOT_DEF;

/***************/
/* read record */
/***************/
if (dn != (D2_name_string) NULL)/* dn is given                        */
{                               /* read record                        */
				/* try to read record via given dnint */
				/* and dn                             */
				/* fill key value dnint and dn        */
     d26_i09_fill_dnint (dnint, dnintptr);
     d26_i08_fill_charfield (dnptr, (char *)dn, D2_DNL_MAX, D2_EOS);

				/* read record                        */
     rcode = d26_i07_read_dnlistrec(D26_I05_KEY_DNINTANDDN,dnlistrec,FALSE);

     switch (rcode)             /* return code from read              */
     {
     case D26_I13_FOUND:        /* record was found                   */
				/* check whether record is valid      */
	  occurrency = ldlong (ptroccurrency);
	  if (occurrency <= 0)  /* no valid record found              */
	  {                     /* dn must be added to dnlist         */
	       add_dn = TRUE;
	  }
	  else
	  {                     /* get dnlist_idx                     */
	    if                  /* maximum index exceeded             */
	       ((max_short = isrecnum - D26_I03_GET_CORE_INDEX)
		    > D26_MAX_DNLIDX)
	    {                   /* set return_value                   */
		  return_value = D26_ERROR;
	    }
	    else
	    {                   /* set index to dnlist in core        */
		  *dnlist_idx = max_short;
	    }                   /* max. index exceeded                */
	  }
	  break;
     case D26_I14_NOT_FOUND:    /* record wasn't found                */
				/* set indicator for record doesn't   */
				/* exist                              */
	  add_dn = TRUE;        /* dn must be added to dnlist         */
	  break;
     default:                   /* C isam error occurred              */
	  return_value = D26_ERROR;
	  break;
     }

     /*************************/
     /* add_dn when necessary */
     /*************************/
     if (add_dn == TRUE)
     {   
	  rcode = d26_i02_upd_file_dnlist (D2_ADD_AV, (D2_name_string) NULL,
					   dnint, dn, 1L,
					   FALSE, dnlist_idx);
	  if (rcode == D26_ERROR)
	  {   
	       return_value = D26_ERROR;
	  } 
     }
}
else
{                               /* no dn given                        */
     return_value = D26_ERROR;
}

return (return_value);

}                               /* d26_i26_get_idx_from_file          */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i27_get_dn_from_file                              */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        29.03.89                                              */
/*                                                                    */
/* DESCRIPTION: This function opens the dnlist file (when necessary)  */
/*              and read the record with given index of dn in core.   */
/*              The corresponding dn is given to the caller.          */
/*                                                                    */
/*              When no valid record exists D26_ERROR is returned.    */
/*                                                                    */
/*              When last_operation is D2_TRUE the file is closed     */
/*              at the end of the function.                           */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      Bool              last_operation   indicates whether function */
/*                                         will be called later       */
/*                                D2_TRUE  last call                  */
/*                                         dnlist file is closed only */
/*                                         in this case the other     */
/*                                         parameters are ignored     */
/*                                         no index is returnedd      */
/*                                D2_FALSE not last call              */
/*                                         dnlist file is kept open   */
/*                                                                    */
/*      Long              dnlist_idx       index of dn in core        */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      D2_name_string    dn               distinguished name         */
/*                                         (normed value, memory must */
/*                                          allocated by the caller,  */
/*                                          as interpretation D2_SOBJ */
/*                                          is assumed and checked)   */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*    Short   D2_NO_ERR          no error occurred                    */
/*                                                                    */
/*            D26_ERROR          C isam error occurred                */
/*                               or no valid record found             */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d26_i27_get_dn_from_file (boolean last_operation,
    signed32 dnlist_idx, D2_name_string dn)

{                               /*  d26_i27_get_dn_from_file          */

				/* record buffer of isam file         */
  char      dnlistrec[D26_I02_RECDNLISTL];

  char      *dnintptr,          /* ptr to dn interpretation           */
	    *dnptr,             /* ptr to dn name                     */
	    *ptroccurrency;     /* ptr to occurrency                  */
  signed32  return_value = D2_NO_ERR;
  D2_dnint  dnint = D2_SOBJ;

  register signed16 rcode;      /* return code of called functions    */
  register signed32 occurrency;

/********************/
/* close file       */
/********************/
if (last_operation == TRUE)     /* file should be closed only         */
{                               /* close file                         */
     rcode = d26_i06_close_dnlistfile ();
     if (rcode != D2_NO_ERR)
     {   
	  return_value = D26_ERROR;
     } 
     return (return_value);
}

/*******************/
/* open file       */
/*******************/
if                              /* open dnlist file fails               */
  (d26_i05_open_dnlistfile () == D26_ERROR)
{                               /* return error                         */
     return (D26_ERROR);
}                               /* open dnlist file fails               */

/************************/
/* initialize variables */
/************************/
dnintptr = &dnlistrec[D26_I10_DNINT];
dnptr    = &dnlistrec[MAXKEYSIZE];
ptroccurrency = &dnlistrec[D26_I12_OCCURRENCY];

/***************/
/* read record */
/***************/
if (dnlist_idx >= 0L)           /* record exists                      */
{                               /* read record via isrecnum           */
     isrecnum = dnlist_idx + D26_I03_GET_CORE_INDEX;
				/* read record                        */
     rcode = d26_i07_read_dnlistrec(D26_I04_NO_KEY,dnlistrec,FALSE);

     switch (rcode)             /* return code from read              */
     {
     case D26_I13_FOUND:        /* record was found                   */
				/* check whether record is valid      */
	  occurrency = ldlong (ptroccurrency);
	  if                    /* valid record found                 */
	     ((occurrency > 0) && (*dnintptr == dnint))
	  {                     /* get dn                             */
	       d27_007_norm_name(dnptr,strlen(dnptr) + 1,(char *)dn);
	  }
	  else
	  {                     /* no valid record found              */
	    dce_svc_printf(GDS_S_ILL_DN_INDEX_MSG,dnlist_idx);
	    return_value = D26_ERROR;
	  }
	  break;
     case D26_I14_NOT_FOUND:    /* record wasn't found                */
     default:                   /* C isam error occurred              */
				/* MA LOG                             */
				/* set return_value                   */
	  return_value = D26_ERROR;
	  break;
     }                          /* return code from read              */
}
else
{                               /* no valid index given               */
     dce_svc_printf(GDS_S_ILL_DN_INDEX_MSG,dnlist_idx);
     return_value = D26_ERROR;
}

return (return_value);

}                               /*  d26_i27_get_dn_from_file          */
