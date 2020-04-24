/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26utilis.c,v $
 * Revision 1.1.12.2  1996/02/18  18:15:11  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:37:43  marty]
 *
 * Revision 1.1.12.1  1995/12/08  15:49:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:08:21  root]
 * 
 * Revision 1.1.10.2  1994/05/10  15:53:00  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:26:17  marrek]
 * 
 * Revision 1.1.10.1  1994/02/22  18:40:28  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:21:48  marrek]
 * 
 * Revision 1.1.8.1  1993/10/29  15:11:23  keutel
 * 	CR 9261
 * 	[1993/10/29  14:06:37  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  15:40:40  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:22:20  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:43:16  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:21:45  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  20:26:03  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  11:20:07  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:53:42  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:58:54  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d26utilis.c,v $ $Revision: 1.1.12.2 $ $Date: 1996/02/18 18:15:11 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*								      */
/* TYPE         : MODULE                                              */
/*								      */
/* NAME         : d26utilis.c                                         */
/*								      */
/* AUTHOR       : Schreck, D AP 11                                    */
/* DATE         : 07.01.88                                            */
/*								      */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOC.-NR.     : DS-Design-Spec.                                     */
/*			.					      */
/*			.					      */
/*								      */
/* PRD#/VERS.   : <Productnr. from Productstructure - <Versionnr.>    */
/*								      */
/* DESCRIPTION  : This module contains the functions to maintain      */
/*                the distinguished name list in file and core        */
/*								      */
/* SYSTEM DEPENDENCIES: SINIX C-ISAM                                  */
/*								      */
/* HISTORY      :                                                     */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | ID | CR# EM# */
/*   0.1    |07.01.88 | Original                       | as |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*								      */
/*			  I N C	L U D E	S			      */
/*								      */
/**********************************************************************/

/*****  internal Includes    *****/

#include <d26dsa.h>
#include <d26schema.h>
#include <d26isam.h>

/*****  Operating-System-Includes  *****/

/*****  external Includes    *****/

#include <dce/d27util.h>
#include <d26svcis.h>

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

/******** EXPORT ********/

signed32   d26_oreclen;             /* maximal record length (objects)*/
signed32   d26_areclen;             /* maximal record length (attr.)  */
char       *d26_ob_record;          /* record containing objects      */
char       *d26_sn_record;          /* record containing superior node*/
char       *d26_a_record;           /* record containing attribute val*/
D26_fildes *d26_ofildes;            /* Description for object files   */
boolean    *d26_ofile_searched;     /* Indicates which object files   */
				    /* are searched already           */
D26_fildes *d26_afildes;            /* Description for attribute files*/

				    /* fd's in /chdb-directory        */
int        d26_sch_fd[]  = {-1,-1,-1,-1};

/******** LOCAL  ********/

/**********************************************************************/
/*								      */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*								      */
/**********************************************************************/

/*exon ****************************************************************/
/*								      */
/* TYPE         : C-FUNCTION                                          */
/*								      */
/* NAME         : d26_i14_unlock                                      */
/*								      */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 02.02.88                                            */
/*								      */
/* DESCRIPTION  : This function will terminate the DSA service        */
/*                function and release all locked C-ISAM records      */
/*                and files.                                          */
/*								      */
/* RETURN VALUE :   -                                                 */
/*								      */
/*exoff ***************************************************************/

void d26_i14_unlock(void)

{                                   /*  d26_i14_unlock()              */

  D26_fildes *p_fildes;

  register signed16 i;

  for                   /* all objectfiles                            */
     (i = 0; i < d26_nofile; i++)
  {                     /* get actual filetable entry                 */
    p_fildes = d26_ofildes + i;
    if                  /* file is locked                             */
      (p_fildes->d26_in_use == TRUE &&
       !(d26_requestor.d26_sm_flag == D26_SM_PENDING && i == 0))
    {                   /* release C-ISAM file                        */
      D26_ISRELEASE(svc_c_sev_warning,p_fildes->d26_isfd);
			/* reset indicator                            */
      p_fildes->d26_in_use = FALSE;
    }
  }

/* if schema modification is pending and scheme files shall be opened */
/* then files in the subdirectory /chdb have to be closed             */
  for (i=0; i < 4; i++)
  {
    if (d26_sch_fd[i] >= 0)
    {
      D26_ISCLOSE(svc_c_sev_warning,d26_sch_fd[i]);
      d26_sch_fd[i] = -1;
    }
  }

}                                   /* d26_i14_unlock                 */
