/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20init.c,v $
 * Revision 1.1.10.2  1996/02/18  19:38:55  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:19:53  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:12:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:02  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:03:05  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:02:41  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:56:33  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:15:35  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  08:46:38  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:10:40  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:45:53  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:59:12  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  13:40:15  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  14:35:43  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:34:44  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:08:28  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20init.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:38:55 $";
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
/* NAME         : d20init.c                                           */
/*								      */
/* AUTHOR       : R. Horn, D AP 11                                    */
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
/*   1.0    | 20.01.88| Original                       | ho |         */
/*   2.0    | 14.12.88| Fehler bei BIND                | hv |     52  */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/*exon ***********************************************************************/
/*                                                                           */
/*  FUNCTION:  d20_init ()                                                   */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The function initializes the common parameter block used at the      */
/*      IAPL-interface of the directory system V2. The exchanging of all     */
/*      informations between the administration and the interface specific   */
/*      functions of the directory system are done through this parameter    */
/*      block.                                                               */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*                    None                                                   */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*      info        = Initialized parameter block.                           */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*      D20_NOERROR = The function has been performed successfully.          */
/*      D20_ERROR   = Allocation of required memory has failed.              */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                        DATE: 4.3.88       */
/*exoff **********************************************************************/

#include <gds.h>

#include <dce/d2dir.h>
#include <gdsext.h>
#include <d2adm.h>
#include <d2info.h>
#include <d20proto.h>

/* -------------------- declaration of global data ------------------------- */

Ds_v2_info	info ;		/* common IAPL-interface parameter block */
signed16        d20_bid, d20_dir_id ;     /* bind-, directory identifier */
char            *d20_object;
Bool            d20_cache = FALSE;   /* 'using cache'-flag */
Bool            d20_shadow = FALSE;  /* 'using shadow'-flag */
D2_Fi_filter    d20_finfo[D2_NP_MAX+1] ;  /* filter information area */

/* ------------------- declaration of local data --------------------------- */

/* declaration of all data belonging to the common IAPL-parameter block */

static D2_a_info   d20_ainfo[D20_AT_MAX] ;      /* attribute information area */
static D2_a_modinf d20_minfo[D2_AT_MAX] ;	/* modify attribute information area */
static D2_a_value  d20_aval[D20_AT_MAX] ;       /* attribute value reference area */
static char	   d20_values[D20_MATSIZE] ;	/* attribute value area */
static D2_a_type   d20_atypes[D2_AT_MAX] ;	/* attribute type list area */

static char        d20_obname[D2_DNL_MAX+1] ;   /* DN of object */
static char        d20_alname[D2_DNL_MAX+1] ;   /* Alias name of Object */
static char        d20_rdnname[D2_NP_LEN+D2_NP_ID_LEN+3] ;
						/* RDN of Object */

static Bool        init = FALSE;

/* ------------------------------------------------------------------------- */

d2_ret_val d20_init (void) 
{

	   info.bind_id = d20_bid ;
	   info.dir_id = d20_dir_id ;
	   info.filename = NULL ;
	   info.filter_present = FALSE;
	if (d20_cache == TRUE)
	   info.serv_cntrl.d2_serv_cntrls = D2_DUACACHE | D2_RESIDENT_CLASS;
	else {
	   info.serv_cntrl.d2_serv_cntrls = (d20_shadow) ?
	   D2_LOCALSCOPE | D2_NOREFERRAL | D2_USEDSA | D2_DREFALIAS
	   | D2_DONT_STORE :
	   D2_NOCACHE | D2_NOREFERRAL | D2_USEDSA | D2_DREFALIAS
	   | D2_DONT_STORE ;
	}

	info.serv_cntrl.d2_serv_cntrls |= D2_PREF_ADM_FUN;

	if (init == FALSE)   {
	   info.serv_cntrl.d2_extension = (D2_extension *) 0;
	   info.serv_cntrl.d2_op_progress.d2_nr_phase = D2_NOT_STARTED;
	   info.serv_cntrl.d2_op_progress.d2_next_rdn = 0;
	   info.serv_cntrl.d2_al_rdn = 0;
	   info.serv_cntrl.d2_priority = D2_PRIO_LOW ;
	   info.serv_cntrl.d2_time_limit = D2_T_UNLIMITED ;
	   info.serv_cntrl.d2_size_limit = D2_S_UNLIMITED ;
	   info.serv_cntrl.d2_scope_ref = D2_WORLD_SCOPE;

	   info.filter.d2_fi_v.fi_and_or.d2_fi_filter = d20_finfo ;
	   info.entry = (D2_name_string)d20_obname ;
	   d20_object = (char *)((D2_name_string)info.entry + 1);
	   info.alias = (D2_name_string)d20_alname ;
	   info.rdn = (D2_name_string)d20_rdnname ;

	   info.reqinfo.d2_r_val_at = D2_R_VAL_TYP ;
	   info.reqinfo.d2_r_type_at = d20_atypes ;
	   info.attrinfo = d20_ainfo ;
	   info.avalinfo = d20_aval ;
	   info.avalues = (Octet_string) d20_values ;
	   info.modinfo = d20_minfo ;
	   info.resentry = (D2_ei_info *) NULL;
	   info.error = (D2_error *) NULL;
	   init = TRUE;
	}
	return (D2_NOERROR) ;
}
