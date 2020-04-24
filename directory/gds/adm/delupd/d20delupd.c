/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20delupd.c,v $
 * Revision 1.1.10.2  1996/02/18  19:41:41  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:22:24  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:18:59  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:52:26  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:06:43  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:12:30  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:35:20  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:34:15  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  10:36:30  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:36:04  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:09:57  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:06:05  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  14:56:03  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  16:09:27  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  22:05:12  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:46:06  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20delupd.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:41:41 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* NAME         : d20delupd.c                                         */
/*								      */
/* AUTHOR       : Schmid ,D AP 11                                     */
/* DATE         : 05.01.89                                            */
/*								      */
/*datoff **************************************************************/

#include <gds.h>
#include <stdio.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <gdsext.h>
#include <d2adm.h>
#include <d2msk.h>
#include <d2delupd.h>
#include <d20proto.h>

/**********************************************************************/
/*								      */
/*		   D E C L A R A T I O N S       DATA		      */
/*								      */
/**********************************************************************/

extern D20_omask  d20_Domask[];

extern D20_omask  d20_E1omask[];
extern D20_omask  d20_E2omask[];
extern D20_omask  d20_Eaomask[];
extern D20_omask  d20_Ebomask[];

extern D20_omask  d20_qomask[];
extern char      *d20_errtab[];

extern char       d20_actopmsg[];
extern char      *d20_dupd_op[];
extern char      *d20_op[];

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_C_delta_update()                                  */
/*								      */
/* AUTHOR:      Schmid    (D AP 11)                                   */
/* DATE:        05.01.89                                              */
/*								      */
/* DESCRIPTION:                                                       */
/*      This function displays the mask to enter the shadowing        */
/*      administration functions.                                     */
/*      The selected shadowing administration function will be        */
/*      performed.                                                    */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              D20_pE      *parblk;                                  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/*exoff ***************************************************************/

signed16 d20_C_delta_update(
  D20_pE * parblk)
{                               /*  d20_C_delta_update                */
signed32 op;
d2_ret_val ret_value = D2_NOERROR;
Bool disp = FALSE;
char install_dir[D27_LFILE_NAME];

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

d20_init();
for (; ret_value == D2_NOERROR; )
{   strcpy(d20_actopmsg,d20_op[DS__DELTA_UPDATE]);
    *d20_Domask[DS_D_BREAK].d20_ioinfo = DS_DEFAULT_VALUE;

			/* show operations for delta updates  */
    d20_handlemask (d20_Domask);

    if                          /* break was selected                 */
       (*d20_Domask[DS_D_BREAK].d20_ioinfo != DS_DEFAULT_VALUE)
    {                           /* return(D2_NOERROR)                 */
	return(D2_NOERROR);
    }

    op = * ((signed32 *) d20_Domask[DS_D_OP].d20_ioinfo);
    switch ((int) op)
    {   case DS__END:           ret_value = D2_ERROR;
				break;
	case DS__CREATE_SHD:    strcpy(d20_actopmsg, d20_dupd_op[op]);
				d20_C00_create_shadow(parblk,
						      (signed32) SHD_AND_JOB);
				disp = FALSE;
				break;
	case DS__CRE_JOB:       strcpy(d20_actopmsg, d20_dupd_op[op]);
				d20_C00_create_shadow(parblk,
						      (signed32) JOB_ONLY);
				disp = FALSE;
				break;
	case DS__REMOVE_SHD:    strcpy(d20_actopmsg, d20_dupd_op[op]);
				(void)d20_C01_remove_shadow(parblk, disp,
						      (signed32) SHD_AND_JOB);
				disp = FALSE;
				break;
	case DS__REM_JOB:       strcpy(d20_actopmsg, d20_dupd_op[op]);
				(void)d20_C01_remove_shadow(parblk, disp,
						      (signed32) JOB_ONLY);
				disp = FALSE;
				break;
	case DS__UPD_SHD_JOB:   strcpy(d20_actopmsg, d20_dupd_op[op]);
				d20_C02_update_job(parblk, disp);
				disp = FALSE;
				break;
	case DS__DISP_SHD_JOB:  strcpy(d20_actopmsg, d20_dupd_op[op]);
				d20_C03_display_jobs(parblk->d20_Ebid,
						     parblk->d20_Edir_id,
						     &disp);
				break;
	case DS__DISP_UPD_ERR:  strcpy(d20_actopmsg, d20_dupd_op[op]);
				sprintf(install_dir, "%s%s", dcelocal_path,
					D27_SERVER_DIR);
				(void)d20_C04_show_error_file(install_dir,
							parblk->d20_Edir_id,
                                                        DS__DISP_UPD_ERR);
				disp = FALSE;
				break;

	case DS__UPD_ERR:       strcpy(d20_actopmsg, d20_dupd_op[op]);
				sprintf(install_dir, "%s%s", dcelocal_path,
					D27_SERVER_DIR);
				(void)d20_C04_show_error_file(install_dir,
							parblk->d20_Edir_id,
                                                        DS__UPD_ERR);
				disp = FALSE;
				break;
	default:                d20_qomask[D20_Q1POS].d20_ioinfo =
						      d20_errtab[D20_E00IDX];
				d20_handlemask(d20_qomask);
				break;
    }
}

return(D2_NOERROR);

}                               /*  d20_C_delta_update                */
