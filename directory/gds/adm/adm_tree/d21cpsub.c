/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21cpsub.c,v $
 * Revision 1.1.10.2  1996/02/18  19:39:57  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:21:00  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:15:18  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:21  root]
 * 
 * Revision 1.1.8.3  1994/06/21  14:43:24  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  12:09:22  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:04:55  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:07:16  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:57:59  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:24:44  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  09:40:58  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:23:11  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:48:29  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:02:47  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  14:22:10  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  15:35:43  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:48:49  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:13:15  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21cpsub.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:39:57 $";
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
/* NAME         : d21cpsub.o                                          */
/*                                                                    */
/* AUTHOR       : Serra & Pulpillo, D AP 11                           */
/* DATE         : 13.09.89                                            */
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
 
#include <dce/d2dir.h>
#include <gdsext.h>
#include <dce/d27util.h>
#include <d2delupd.h>
#include <d2dump.h>
#include <d20proto.h>
 
/**********************************************************************/
/*                                                                    */
/*                        D E F I N E S                               */
/*                                                                    */
/**********************************************************************/
 
#define D20_COPY  "COPY"        /* Name of the temporary file         */

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
/*  FUNCTION:  d21_11_copy()                                          */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function will call the d21_14_save and d21_12_appnd      */
/*      to write a given object/subtree into a temporary file and     */
/*      append this information under an specified object in another  */
/*      DSA.                                                          */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      signed16      source_bind_id;                                 */
/*      signed16      version;                                        */
/*      Name_string   object;                                         */
/*      signed32      subset;                                         */
/*      C_arg        *c_arg;                                          */
/*      signed16      sink_bind_id;                                   */
/*      Name_string   parent_name;                                    */
/*      signed32      inherit;                                        */
/*      Bool          overwrite;                                      */
/*      Name_string **old_objects;                                    */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      Errcode     *return_code;       error values                  */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_NOERROR  = The function has been performed successfully.   */
/*      D2_ERROR    = function fails                                  */
/*                                                                    */
/*exoff ***************************************************************/
 
 d2_ret_val d21_11_copy(
   signed16      source_bind_id,
   Errcode      *return_code,
   signed16      version,
   Name_string   object,
   signed32      subset,
   C_arg        *c_arg,
   signed16      sink_bind_id,
   Name_string   parent_name,
   signed32      inherit,
   Bool          overwrite,
   Name_string **old_objects)

{
d2_ret_val     ret_value = D2_NOERROR;
char           filename[D27_LFILE_NAME]; 
char           filename_1[D27_LFILE_NAME];
char        subset_char[D20_LOG_SUBSET];
char        inh_ovr[D20_LOG_INHOVR];
 
/*
d21d009_errlog("ENTRY d21_11_copy", NULL);
*/

/*
d27_009_trace_dn("  Object:", (char *)object);
*/
sprintf(subset_char, "%d", subset);
/*
d27_009_trace_dn("  Subset:", subset_char);
*/

/*
d27_009_trace_dn("  New Parent Name:", (char *)parent_name);
*/
sprintf(inh_ovr, "%d %d", inherit, overwrite);
/*
d27_009_trace_dn("  Inherit/Overwrite:", inh_ovr);
*/

                          /* Building the name of the temporary file  */
sprintf(filename,"%s%d",D20_COPY,getpid()); 
sprintf(filename_1,"%s%d_1",D20_COPY,getpid()); 

                          /* Call to the save function                */
if ((ret_value = d21_14_save(source_bind_id, return_code, version,
		 object, subset, c_arg->serv_cntrls, filename )) == D2_NOERROR) {
                          /* if OK, call to the append function       */
   ret_value = d21_12_appnd(sink_bind_id, return_code, version, filename,
			parent_name, inherit, overwrite, old_objects);
   if (ret_value != D2_NOERROR && return_code->errvalue == DS_NFOUND)
	return_code->errvalue = DS_PAR_NFOUND;
}
 
unlink(filename);         /* Delete temporary file                    */
unlink(filename_1);       /* Delete temporary file                    */
/*
d21d009_errlog("EXIT  d21_11_copy", return_code);
*/
 
return(ret_value);
}
