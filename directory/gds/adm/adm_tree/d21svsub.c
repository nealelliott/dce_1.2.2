/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21svsub.c,v $
 * Revision 1.1.10.2  1996/02/18  19:40:10  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:21:14  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:15:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:31  root]
 * 
 * Revision 1.1.8.3  1994/06/21  14:43:26  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  12:09:34  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:05:03  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:07:53  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:58:08  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:26:15  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  09:49:50  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:25:13  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:48:53  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:03:20  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  14:25:09  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  15:38:08  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:50:15  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:14:20  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21svsub.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:40:10 $";
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
/*                                                                    */
/* NAME         : d21svsub.o                                          */
/*                                                                    */
/* AUTHOR       : Serra & Pulpillo, D525 (CDS)                        */
/* DATE         : 15.09.89                                            */
/*                                                                    */
/* KOMPONENTE   : DS                                                  */
/*                                                                    */
/* DOK.-NR.     : DS-Design-Spez.                                     */
/*                      .                                             */
/*                      .                                             */
/*                                                                    */
/* PRD#/VERS.   : <Produktnr. aus Produktstruktur> - <Versionsnr.>    */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* SYSTEM-DEPENDENCIES:                                               */
/*                      COMMON                                        */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*      0.1 | 16.01.89| Erstellung                     | hv |         */
/*                                                                    */
/*datoff **************************************************************/
 
/**********************************************************************/
/*                                                                    */
/*                     Modulidentification                            */
/*                                                                    */
/**********************************************************************/
 
/**********************************************************************/
/*                                                                    */
/*                        I N C L U D E S                             */
/*                                                                    */
/**********************************************************************/
 
#include <gds.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
 
#include <dce/d2dir.h>
#include <gdsext.h>
#include <d21iapl.h>
#include <dce/d27util.h>
#include <d2delupd.h>
#include <d2dump.h>
#include <d20proto.h>
 
/**********************************************************************/
/*                                                                    */
/*                        D E F I N E S                               */
/*                                                                    */
/**********************************************************************/
 
/**********************************************************************/
/*                                                                    */
/*                 T Y P E  -  D E F I N I T I O N S                  */
/*                                                                    */
/**********************************************************************/
 
/**********************************************************************/
/*                                                                    */
/*                 D E K L A R A T I O N E N      DATEN               */
/*                                                                    */
/**********************************************************************/
 
/**********************************************************************/
/*                                                                    */
/*                D E K L A R A T I O N E N      FUNCTIONEN           */
/*                                                                    */
/**********************************************************************/

 
/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d21_14_save()                                          */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function will save either master information or shadow   */
/*      information for a given object/subtree into a local file.     */
/*      The function will save all the required objects and for each  */
/*      object all its attributes.                                    */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      signed16     bind_id;                                         */
/*      signed16     version;                                         */
/*      Name_string  object;                                          */
/*      signed32     subset;                                          */
/*      signed32     serv_cntrls;                                     */
/*      String       file_name;                                       */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      Errcode     *return_code;       error values                  */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_NOERROR  = The function has been performed successfully.   */
/*      D2_ERROR    = function fails                                  */
/*                                                                    */
/*exoff ***************************************************************/
 
 d2_ret_val d21_14_save(
   signed16     bind_id,
   Errcode     *return_code,
   signed16     version,
   Name_string  object,
   signed32     subset,
   signed32     serv_cntrls,
   String       file_name)
{
D20_pB      pb;
D2_pbhead   head;
signed16    dir_id;
D2_c_arg    save_c_arg;
d2_ret_val  ret_value = D2_NOERROR;
char        obj_norm[D2_DNL_MAX];
char        local_dsa[D2_DNL_MAX];
char        subset_char[D20_LOG_SUBSET];
 
/*
d21d009_errlog("ENTRY d21_14_save", NULL);
*/
 
/*
d27_009_trace_dn("  Object:", (char *)object);
*/
sprintf(subset_char, "%d", subset);
/*
d27_009_trace_dn("  Subset:", subset_char);
*/

				/* check version, bind-id */
if (d21t009_param_check(bind_id, return_code, version, &dir_id)
                        == D2_ERROR)
{  
    /*
    d21d009_errlog("EXIT  d21_14_save", return_code);
    */
    return(D2_ERROR);
}
pb.d20_Bdir_id = dir_id;
 
                                /* check object interpretation        */
if (subset != OBJECT && subset != OBJECT_SUBORDINATES)
{   return_code->retcode  = CALLERR;
    return_code->errclass = PARERR;
    return_code->errvalue = DS_INV_SEARCH_SEL;
    /*
    d21d009_errlog("EXIT  d21_14_save", return_code);
    */
    return(D2_ERROR);
}
 
pb.d20_B1bind_id = bind_id;
pb.d20_Bpbhead = &head;
head.d2_version = DS_V021;
pb.d20_Bo_file = NULL;
if (object == NULL)
{   pb.d20_Bsubtree = (D2_name_string)"";
}
else
{   strcpy(obj_norm, (char *)object);
    d20_norm(obj_norm, strlen(obj_norm));
    pb.d20_Bsubtree = (D2_name_string)obj_norm;
}
pb.d20_Bsingle_ob = (subset == OBJECT) ? TRUE : FALSE;
pb.d20_B1c_a = &save_c_arg;
save_c_arg.d2_serv_cntrls = D2_NOREFERRAL | D2_USEDSA | D2_DREFALIAS |
                            D2_DONT_STORE | D2_PREF_ADM_FUN;

save_c_arg.d2_serv_cntrls |= serv_cntrls;

save_c_arg.d2_priority = D2_PRIO_LOW ;
save_c_arg.d2_time_limit = D2_T_UNLIMITED ;
save_c_arg.d2_size_limit = D2_S_UNLIMITED ;
pb.d20_Bn_file = file_name;
local_dsa[0] = D2_EOS;             /* The DSA name is not used           */
pb.d20_Bdsa=(D2_name_string)local_dsa;
 
                                /* save object/subtree                */
if                              /* saving fails                       */
   (d20_B_handle_subtrees(&pb, (D2_name_string)NULL))
{   if (pb.d20_Bpbhead->d2_errvalue == D2_NFOUND)
    {   return_code->retcode  = CALLERR;
	return_code->errclass = PARERR;
	return_code->errvalue = DS_NFOUND;
    }
    else
    {   return_code->retcode  = PERMERR;
	return_code->errclass = SYSERR;
	return_code->errvalue = pb.d20_Bpbhead->d2_errvalue;
    }
    ret_value = D2_ERROR;
}
 
/*
d21d009_errlog("EXIT  d21_14_save", return_code);
*/
 
return(ret_value);
}
