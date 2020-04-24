/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20bind.c,v $
 * Revision 1.1.10.2  1996/02/18  19:38:42  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:19:39  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:11:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:46  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:02:54  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:02:03  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:56:12  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:13:52  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  08:37:26  marrek
 * 	July 1993 code drop
 * 	[1993/07/30  08:49:23  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  17:45:03  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:58:30  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  21:26:22  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:07:32  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20bind.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:38:42 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton	***************************************************************/
/*								      */
/* TYPE         : MODUL                                               */
/*								      */
/* NAME         : d20bind.c                                           */
/*								      */
/* AUTHOR       : Schmid , D AP 11                                    */
/* DATE         : 10.05.88                                            */
/*								      */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOC.-NR.     : Directory-Design-Specification                      */
/*			.					      */
/*			.					      */
/*								      */
/* PRD#/VERS.   :                                                     */
/*								      */
/* DESCRIPTION  :                                                     */
/*								      */
/* SYSTEM DEPENDENCIES: COMMON                                        */
/*								      */
/* HISTORY      :                                                     */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*          |         |                                |    |         */
/*datoff **************************************************************/

#include <gds.h>
#include <string.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d21iapl.h>
#include <d20proto.h>


 signed16 d20_dir_id;		/* directory identifier		*/
 signed16 d20_auth_mech;	/* authentication mechanism	*/
 D2_credent *p_usercred;
 signed16 d20_b_err;



/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20__bind()                                           */
/*								      */
/* AUTHOR:      Schmid    (D AP 11)                                   */
/* DATE:        10.05.88                                              */
/*								      */
/* SYNTAX:      d2_ret_val d20__bind(dsa_name, bind_id)               */
/*              D2_name_string  dsa_name;                             */
/*              signed16       *bind_id;                              */
/*								      */
/* DESCRIPTION:                                                       */
/*              This function will execute the "bind" operation to the*/
/*              given DSA.                                            */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              D2_name_string  dsa_name;                             */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              signed16       *bind_id;                              */
/*								      */
/* RETURNVALUE:							      */
/*              D2_NOERROR:     no errors                             */
/*              D2_ERROR:       bind failed                           */
/*								      */
/* STANDARD-HEADER: None                                              */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*								      */
/* NOTES: Modifications for security by Sanjay.                       */
/*								      */
/*exoff ***************************************************************/

d2_ret_val d20__bind(D2_name_string dsa_name, signed16 *bind_id)
{                               /*  d20__bind()                       */
D2_password pw;
D2_credent  user_cred;
D21_p1      parblk;
d2_ret_val  ret_value;

static  D2_pbhead   head = { D21V022 };


parblk.d21_1pbhead    = &head;
parblk.d21_1dir_id    = d20_dir_id;
parblk.d21_1auth_mech = d20_auth_mech;

if (p_usercred == (D2_credent *)NULL)
	parblk.d21_1u_credent = NULL;
else
	{
	parblk.d21_1u_credent = &user_cred;
	user_cred.d2_c_name   = p_usercred->d2_c_name;

	if (p_usercred->d2_c_passwd == (D2_password *)NULL)
		user_cred.d2_c_passwd = (D2_password *)NULL;
	else
		{
		user_cred.d2_c_passwd = &pw;

		pw.d2_p_len = p_usercred->d2_c_passwd->d2_p_len;
		pw.d2_p_pw = p_usercred->d2_c_passwd->d2_p_pw;
		}
	}

parblk.d21_1name_dsa  = strlen((char *)dsa_name) ? dsa_name : NULL;
parblk.d21_1addr_dsa  = NULL;

ret_value = d21_1_bind(&parblk);
if (ret_value < 0)
{   
    d20_b_err = head.d2_errvalue;
}
else
{
    *bind_id = parblk.d21_1bind_id;
} 

				/*  return(ret_value of "bind")        */
return(ret_value);

}                               /*  d20__bind()                       */
