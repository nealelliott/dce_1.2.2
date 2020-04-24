/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20update.c,v $
 * Revision 1.1.10.2  1996/02/18  19:39:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:20:36  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:14:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:52  root]
 * 
 * Revision 1.1.8.4  1994/07/06  15:06:23  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:11:06  marrek]
 * 
 * Revision 1.1.8.3  1994/06/21  14:43:18  marrek
 * 	June 1994 code submission.
 * 	[1994/06/17  13:08:09  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:04:35  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:05:39  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:57:25  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:21:08  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  09:19:09  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:18:09  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:47:28  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:01:21  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  13:57:14  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  15:04:26  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:43:21  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:11:21  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20update.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:39:34 $";
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
/* NAME         : d20update.o   [scheme update interface]             */
/*								      */
/* AUTHOR       : Sanjay, D AP 11                                     */
/* DATE         : 08.02.91                                            */
/*								      */
/* COMPONENT    : DS                                                  */
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
/* HISTORY      :                                                     */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*      0.1 | 04.07.88| Original                       | ws |         */
/*      0.2 | 24.01.90| version 2.2 length objclass 28 | as |         */
/*          |         | D2_LOBJ_CLASS 5 ---> 28        | ws |         */
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

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <gdsext.h>
#include <d20proto.h>

/**********************************************************************/
/*								      */
/*			  D E F	I N E S				      */
/*								      */
/**********************************************************************/

#define D20__ATNO  5            /* number of attributes to be inserted*/
				/* when creating a new object type    */
#define D20_PUBLIC 0

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

/**********************************************************************/
/*								      */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*								      */
/**********************************************************************/


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : gds_upsh()                                 	      */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        04.07.88                                              */
/*								      */
/* SYNTAX:      signed32 gds_upsh(ret_code, version, upd_info) 	      */
/*		Errcode		*ret_code;			      */
/*		signed16	 version;			      */
/*		S_upd_info	*upd_info;			      */
/*								      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This module calls valid functions for validation of   */
/*		modification information. If upd_info is valid then   */
/*		modifications are made in SRT/OCT/AT in memory.	      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		signed16	 version;			      */
/*		S_upd_info	*upd_info;			      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*		Errcode		*ret_code;			      */
/*								      */
/* RETURNVALUE:							      */
/*								      */
/*      D2_NOERROR:     No error occurred                             */
/*      D2_ERROR:       Error occurred                                */
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

 signed32 gds_upsh(
   Errcode    *ret_code,
   signed16    version,
   S_upd_info *upd_info)
 
{                               /*  gds_upsh()             */

/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                  D E C L A R A T I O N S                           */
/*								      */
/**********************************************************************/

 signed32       ret_value = D2_NOERROR;
 D2_pbhead	head;

/*
static char *f_name = "gds_upsh";
static char *f_entry = "ENTRY";
static char *f_exit =  "EXIT";
*/

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

head.d2_version = version;
d20_errcode(ret_code,D2_NOERROR, D2_NOERROR, D2_NOERROR);
d20_error(&head,D2_NOERROR, D2_NOERROR, D2_NOERROR);
/* d21_l03_trace_err(0, f_name, f_entry, &head); */

/*  version is wrong                  */
if (version != DS_V03)
	{/*  set error values                  */
  	d20_errcode(ret_code, CALLERR, ILLVERS, DS_NOT_SPEC);
  	d20_error(&head, CALLERR, ILLVERS, DS_NOT_SPEC);
  	/* d21_l03_trace_err(0, f_name, f_exit, &head); */
  	return(D2_ERROR);
	}

switch ((int) upd_info->upd_mode)  /*  operation is                  */
	{
	case	ADD_AT_ENTRY:
		/*  attribute is valid              */
		if ((ret_value = d20_010_check_attr(ret_code,upd_info))
				       				== D2_NOERROR)
			{
			/*  add new attribute to AT          */
			ret_value = d20_011_add_attr(ret_code, 
					upd_info->upd_val.a_m_at->at_entry, 
					&(upd_info->upd_val.a_m_at->no_at),
					&(upd_info->upd_val.a_m_at->at));
			}
		break;
	case	MOD_AT_ENTRY:
		/*  attribute is valid              */
		if ((ret_value = d20_010_check_attr(ret_code,upd_info))
				       				== D2_NOERROR)
			{
			/*  modify attribute entry in AT          */
			ret_value = d20_013_mod_attr(
					upd_info->upd_val.a_m_at->at_entry, 
					upd_info->upd_val.a_m_at->no_at,
					upd_info->upd_val.a_m_at->at);
			}
		break;
	case	DEL_AT_ENTRY:
		/*  attribute is valid              */
		if ((ret_value = d20_010_check_attr(ret_code,upd_info))
				       				== D2_NOERROR)
			{
			/*  delete attribute from AT		*/
			ret_value = d20_012_del_attr(
					upd_info->upd_val.d_at->at_abbrv,
					&(upd_info->upd_val.d_at->no_at),
					upd_info->upd_val.d_at->at);
			}
		break;
	case  	ADD_OCT_ENTRY:
		/*  Object class is valid              */
		if ((ret_value = d20_020_check_obj(ret_code,upd_info))
				       				== D2_NOERROR)
			{
			/*  add new object class to OCT          */
			ret_value = d20_021_add_obj(ret_code, 
					upd_info->upd_val.a_m_oct->oct_entry, 
					&(upd_info->upd_val.a_m_oct->no_oct),
					&(upd_info->upd_val.a_m_oct->oct));
			}
		break;
	case  	MOD_OCT_ENTRY:
		/*  Object class is valid              */
		if ((ret_value = d20_020_check_obj(ret_code,upd_info))
				       				== D2_NOERROR)
			{
			/*  modify object class in OCT          */
			ret_value = d20_023_mod_obj(
					upd_info->upd_val.a_m_oct->oct_entry, 
					upd_info->upd_val.a_m_oct->no_oct,
					upd_info->upd_val.a_m_oct->oct);
			}
		break;
	case  	DEL_OCT_ENTRY:
		/*  object class is valid              */
		if ((ret_value = d20_020_check_obj(ret_code,upd_info))
				       				== D2_NOERROR)
			{
			/*  delete object class from OCT		*/
			ret_value = d20_022_del_obj(
					upd_info->upd_val.d_oct->oct_abbrv,
					&(upd_info->upd_val.d_oct->no_oct),
					upd_info->upd_val.d_oct->oct);
			}
		break;
	case	ADD_SRT_ENTRY:
		/*  structure rule is valid              */
		if ((ret_value = d20_030_check_srule(ret_code,upd_info))
				       				== D2_NOERROR)
			{
			/*  add new structure rule to SRT 	*/
			ret_value = d20_031_add_srule(ret_code, 
					upd_info->upd_val.a_m_srt->srt_entry, 
					&(upd_info->upd_val.a_m_srt->no_srt),
					&(upd_info->upd_val.a_m_srt->srt));
			}
		break;
	case	MOD_SRT_ENTRY:
		/*  structure rule is valid              */
		if ((ret_value = d20_030_check_srule(ret_code,upd_info))
				       				== D2_NOERROR)
			{
			/*  modify structure rule in SRT	*/
			ret_value = d20_033_mod_srule(
					upd_info->upd_val.a_m_srt->srt_entry, 
					upd_info->upd_val.a_m_srt->no_srt,
					upd_info->upd_val.a_m_srt->srt);
			}
		break;
	case	DEL_SRT_ENTRY:
		/*  structure rule is valid              */
		if ((ret_value = d20_030_check_srule(ret_code,upd_info))
				       				== D2_NOERROR)
			{
			/*  delete structure rule from SRT	*/
			ret_value = d20_032_del_srule(
					upd_info->upd_val.d_srt->rule_nr,
					&(upd_info->upd_val.d_srt->no_srt),
					upd_info->upd_val.d_srt->srt);
			}
		break;
  	default:                      
		d20_errcode(ret_code, CALLERR, PARERR, DS_ILL_OP);
		d20_error(&head, CALLERR, PARERR, DS_ILL_OP);
		ret_value = D2_ERROR;
		break;
	}

/* d21_l03_trace_err(0, f_name, f_exit, &head); */
				/*  return(ret_value)                 */
return(ret_value);

}                               /*  gds_upsh()             */
