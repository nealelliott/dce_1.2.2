/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20search.c,v $
 * Revision 1.1.10.2  1996/02/18  19:38:41  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:19:38  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:11:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:42  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:02:52  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:01:57  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:56:09  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:13:35  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  08:25:24  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:04:56  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  17:44:53  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:58:23  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  21:25:51  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:07:22  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20search.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:38:41 $";
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
/* NAME         : d20search.c                                         */
/*								      */
/* AUTHOR       : Volpers,D AP 11                                     */
/* DATE         : 04.05.88                                            */
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
/*      0.1 | 15.04.88| Erstellung                     | hv |         */
/*      0.2 | 12.12.88| Absturz bei dump & restore     | hv |     49  */
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

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d2dump.h>
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

extern D2_pbhead  *d20_head;          /* import of pbhead             */

/**********************************************************************/
/*								      */
/*		  D E C L A R A T I O N S      FUNCTIONS	      */
/*								      */
/**********************************************************************/


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_165_call_search                                   */
/*								      */
/* AUTHOR:      Volpers   (D AP 11)                                   */
/* DATE:        26.04.88                                              */
/*								      */
/* SYNTAX:      signed16 d20_165_call_search                          */
/*                                                                    */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*								      */
/*       0      :       no error                                      */
/*      -1      :       break is selected                             */
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

 signed16 d20_165_call_search(
   signed16          b_id,
   Bool              single_ob,
   D2_c_arg         *com_arg,
   D2_name_string    subtree,
   char             *filename,
   Bool              second,        /* a flag indicating whether second  */
				    /* search is performed or not        */
   int              *fd1,
   char            **name1,
   signed32         *l_name1,
   D20_offset      **pdn1,
   D20_offset      **dn1,
   signed32         *dn_no1,
   signed32         *attr1_len)

{                               /*  d20_165_call_search               */

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

 signed16     rdn_pos[D2_NP_MAX];
 signed16     rdn_anz;      /* number of RDNs                     */
 Bool         sup_found = TRUE;
 Bool         obj_found ;

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*								      */
/**********************************************************************/

				  /* initialize INFO structure        */
d20_160_set_info(b_id, com_arg, filename);
				  /* set all attributes requested     */
d20_166_set_all_attr();
				  /* find number of RDN's in given DN */
d20_114_get_rdns((char *)subtree, &rdn_anz, rdn_pos);

if
  (single_ob == FALSE)
{   
  rdn_anz--;
} 
if                              /*  operation fails                   */
  (d20_113_search_dn(0, rdn_anz, rdn_pos, (char *)subtree, &obj_found))
{                               /*  return(D2_ERROR)                  */
  if                            /* second search is not performed     */
    (second == FALSE)
  {                             /* return error                       */
    if (d20_head->d2_errvalue != D2_NFOUND)
    {   
      d20_head->d2_errvalue = D20_SEARCH_ERR;
    } 
    return(D2_ERROR);
  }
else if                         /* if NFOUND then NOERROR             */
    (d20_head->d2_errvalue != D2_NFOUND)
  {                             /* return error                       */
    d20_head->d2_errvalue = D20_SEARCH_ERR;
    return(D2_ERROR);
  }
else
{
    sup_found = FALSE;
  } 
} 
if (single_ob == FALSE && sup_found == TRUE)
{                               /*  call search for the whole subtree */
				/* set subset                         */
  d20_set_subset (D2_WHOLE_SUBTREE);
  if (d20_call_iapl(D20_0_SEARCH))
  {   
    if                          /* second search is not perfomed      */
      (second == FALSE)
    {                           /* return error                       */
      d20_head->d2_errvalue = D20_SEARCH_ERR;
      return(D2_ERROR);
    }
else if                         /* if NFOUND then NOERROR             */
      (d20_head->d2_errvalue != D2_NFOUND)
    {                           /* return error                       */
      d20_head->d2_errvalue = D20_SEARCH_ERR;
      return(D2_ERROR);
    } 
  } 
} 
d20_167_set_file_mode();

if                             /* second search is not performed      */
  (second == FALSE)
{   
  if
    (d20_105_gen_dn(FALSE, filename, fd1, name1, l_name1, pdn1, dn1,
		    dn_no1, attr1_len))
  {   
    return(D2_ERROR);
  } 
} 

return(D2_NOERROR);

}                               /*  d20_165_call_search               */
