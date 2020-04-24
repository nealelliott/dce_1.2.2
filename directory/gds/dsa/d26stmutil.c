/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26stmutil.c,v $
 * Revision 1.1.8.2  1996/02/18  19:46:09  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:27:33  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:48:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:07:58  root]
 * 
 * Revision 1.1.6.2  1994/05/10  15:52:47  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:24:36  marrek]
 * 
 * Revision 1.1.6.1  1994/02/22  17:52:54  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:19:02  marrek]
 * 
 * Revision 1.1.4.2  1993/08/10  15:26:54  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:18:20  marrek]
 * 
 * Revision 1.1.2.5  1993/02/02  14:52:29  marrek
 * 	*** empty log message ***
 * 
 * Revision 1.1.2.4  1993/02/02  11:26:49  marrek
 * 	Chaining for modifyRDN and removeObject now works.
 * 	[1993/02/01  15:05:17  marrek]
 * 
 * Revision 1.1.2.3  1992/12/31  19:42:23  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:21:12  bbelch]
 * 
 * Revision 1.1.2.2  1992/11/27  20:21:31  marrek
 * 	November 1992 code drop
 * 	[1992/11/26  10:25:36  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char d26stmutil_rcsid[] = "@(#)$RCSfile: d26stmutil.c,v $ $Revision: 1.1.8.2 $ $Date: 1996/02/18 19:46:09 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1992          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODUL                                               */
/*                                                                    */
/* NAME         : d26stmutil.c                                        */
/*                <Utility-Functions for standard Modify-operations>  */
/*                                                                    */
/* AUTHOR       : G.Loose, SNI AP 113                                 */
/* DATE         : 14.07.92                                            */
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

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S         FUNCTIONS           */
/*                                                                    */
/**********************************************************************/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_st01_chain_ref(...)                             */
/*                                                                    */
/* AUTHOR       : Loose, SNI AP 113                                   */
/* DATE         : 14.07.92                                            */
/*                                                                    */
/* SYNTAX       : Short d26_st01_chain_ref(search_ret,serv_ctrls,mk,  */
/*                                         master,op_id)              */
/*                                                                    */
/*                short       search_ret;                             */
/*                long        serv_ctrls;                             */
/*                long        mk;                                     */
/*                Bool        master;                                 */
/*                short       op_id;                                  */
/*                                                                    */
/* DESCRIPTION  : This function checks the return value of the inter- */
/*                nal search and the service controls to decide wether*/
/*                the DSA has to chain the operation, to give a       */
/*                referral or to return an error.                     */
/*                                                                    */
/* INPUT PARAMETERS:                                                  */
/*     search_ret   short     Return value from u00-search            */
/*     serv_ctrls   long      service controls from invoke message    */
/*     mk           long      master knowledge of object              */
/*                            (in case of AddEntry of the superior)   */
/*     master       Bool      DSA master of superior object ?         */
/*                            (for RemoveEntry and ModifyEntry, in    */
/*                             case of AddEntry of the object itself) */
/*     op_id        short     operation ID                            */
/*                                                                    */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      D2_NOERROR                     perform operation              */
/*      D2_CHAINING_REQUIRED           chain the operation            */
/*      D2_REF_ERROR                   return referral                */
/*      D2_NFOUND                      Error: name error              */
/*      D2_AFFECTS_MULTIPLE_DSAS       Error: Affects multiple DSAs   */
/*      D2_ERROR                       any other error occurred       */
/*                                                                    */
/* STANDARD HEADER :                                                  */
/*                                                                    */
/* GLOBAL DATA (read only):                                           */
/*                                                                    */
/* GLOBAL DATA (updated):                                             */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                                                                    */
/* REMARKS :                                                          */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_st01_chain_ref(signed16    search_ret,
			 signed32       serv_ctrls,
			 signed32       mk,
			 boolean        master,
			 signed16       op_id
			)

{
/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/
boolean       check_chaining;          /* flag for chaining/referral  */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

if (search_ret == D2_NO_ERR) /* object was found */
{
  if (mk == -1) /* DSA master of object */
  {
      if ((op_id == D23_REMOBJT || op_id == D23_REPLRDN) &&
	  !(master))
    {
      return(D2_AFFECTS_MULTIPLE_DSAS);
    }
    else /* for the other operations */
    {
      return(D2_NOERROR);
    }
  }
  else  /* DSA holds object (in case ADD_OBJECT the superior) as shadow */
  {
    if (serv_ctrls & D2_LOCALSCOPE) /* Local Scope set */
    {
      if (op_id == D23_ADDOBJT && master)
      {
	return(D2_AFFECTS_MULTIPLE_DSAS);
      }
      else  /* all other modify operations */
      {
	return(D2_NOERROR);
      }
    }
    else if (op_id == D23_REMOBJT || op_id == D23_REPLRDN)
    { /* int. search returned 0 AND generated referral because of 1 level */
      check_chaining = TRUE;
    }
  }  /* DSA holds object as shadow */
}
else if (search_ret == D2_REF_ERROR)
{
  check_chaining = TRUE;
}

if (check_chaining)
{
  if (serv_ctrls & D2_NOCHAINING ||
      !(serv_ctrls & D2_PREF_CHAINING))
  {
    return(D2_REF_ERROR);
  }
  else
  {
    return(D2_CHAINING_REQUIRED);
  }
}

return(D2_ERROR); /* Any other error occured (programming error) */
}
