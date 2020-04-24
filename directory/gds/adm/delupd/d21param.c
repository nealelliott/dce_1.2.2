/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21param.c,v $
 * Revision 1.1.10.2  1996/02/18  19:42:06  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:22:49  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:20:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:52:43  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:06:56  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:13:31  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:35:41  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:37:02  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  10:52:26  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:39:44  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:10:40  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:07:09  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  15:00:32  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  16:13:25  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:00:44  melman
 * 	New GDS merged drop
 * 	[1992/05/28  20:43:51  melman]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21param.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:42:06 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* NAME         : d21param.o                                          */
/*								      */
/* AUTHOR       : Schmid ,D AP 11                                     */
/* DATE         : 16.01.89                                            */
/*								      */
/*datoff **************************************************************/

#include <gds.h>
#include <stdio.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d21dua.h>
#include <gdsext.h>
#include <d2delupd.h>
#include <d20proto.h>

/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d21d006_param_check()                                  */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function checks version and whether a bind was done.     */
/*      Furthermore it returns the path name of the directory         */
/*      and the directory id.                                         */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_NOERROR     = The function has been performed successfully.*/
/*      D2_ERROR       = function fails                               */
/*                                                                    */
/*exoff ***************************************************************/

d2_ret_val d21d006_param_check(
  signed16 bind_id,
  Errcode * return_code,
  signed16 version,
  char ** path,
  signed32 * dir_id)
{
d21_bind_info bind_info;
Bool		dummy;
D2_pbhead	dummy_pbhead;

return_code->retcode  = return_code->errclass = NOERR;
return_code->errvalue = NOERROR;

if (version != DS_V021)
{   return_code->retcode  = CALLERR;
    return_code->errclass = ILLVERS;
    return_code->errvalue = DS_NOT_SPEC;
    return(D2_ERROR);
}

if (d21_bh8_get_bind_info(bind_id, &bind_info, &dummy, &dummy_pbhead, FALSE) 
				== D2_ERROR)
{   return_code->retcode  = CALLERR;
    return_code->errclass = PARERR;
    return_code->errvalue = DS_NO_BIND;
    return(D2_ERROR);
}
*dir_id = bind_info.d21_b_dir_id;

				/* get environment                    */
    sprintf(*path, "%s%s", dcelocal_path, D27_SERVER_DIR);

return(D2_NOERROR);
}
