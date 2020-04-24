/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d22errhdl.c,v $
 * Revision 1.1.10.2  1996/02/18  19:43:47  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:24:47  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:27:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:54:26  root]
 * 
 * Revision 1.1.8.3  1994/09/06  12:24:55  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:13:35  keutel]
 * 
 * Revision 1.1.8.2  1994/05/10  15:51:14  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:17:11  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  16:03:20  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:53:24  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  07:33:38  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:13:59  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:40:56  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:13:35  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:29:37  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:37:14  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d22errhdl.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:43:47 $";
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
/* NAME         : d22errhdl.c                                         */
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
/*          |         |                                |    |         */
/*datoff **************************************************************/

/*exon ***********************************************************************/
/*                                                                           */
/*  FUNCTION:  d22_errorhdl ()                                               */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The function generates error information. The exact operation of the */
/*      function depends on the type of the error. If the occurred error is  */
/*      of the type fatal, an error message is written to the standard error */
/*      output and then the DUA-cachehandler process terminates.             */
/*      An error information block is generated and returned to the caller   */
/*      if the error type is not fatal.                                      */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      error       = Error number.                                          */
/*      optptr      = This parameter is considered as a pointer to a buffer  */
/*                    where the error information (in a structure of type    */
/*                    'D23_rserror') can be stored.                          */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*                    None                                                   */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*                    None                                                   */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                        DATE: 18.12.87     */
/*exoff **********************************************************************/

#include <gds.h>
#include <stdio.h>
#include <dce/d2dir.h>
#include <d23apdu.h>
#include <d23ipc.h>
#include <d22cache.h>


/* -------------------- declaration of local data -------------------------- */

				/* error value translation table */
#ifdef NORMERR
static D22_err_trans  d22_trans_tab[] = {
	D22_MSGSIZE_ERR,    D2_CALL_ERR, D2_PAR_ERR, D2_NOT_SPEC,
	D22_MSYNTAX_ERR,    D2_PERM_ERR, D2_INT_ERR, D2_NOT_SPEC,
	D22_REQUNKNOWN_ERR, D2_PERM_ERR, D2_INT_ERR, D2_NOT_SPEC,
	D22_OPTYPE_ERR,     D2_CALL_ERR, D2_PAR_ERR, D2_NOT_SPEC,
	D22_VERSION_ERR,    D2_PERM_ERR, D2_INT_ERR, D2_NOT_SPEC,
	D22_DIRID_ERR,      D2_CALL_ERR, D2_PAR_ERR, D2_DIR_UNKNOWN,
	D22_NOFNDATTR_ERR,  D2_CALL_ERR, D2_PAR_ERR, D2_ATTRIBUTE_MISSING,
	D22_NOATTR_ERR,     D2_CALL_ERR, D2_PAR_ERR, D2_CONSTRAINT_VIOLATION,
	D22_ATTSIZE_ERR,    D2_CALL_ERR, D2_PAR_ERR, D2_CONSTRAINT_VIOLATION,
	D22_MUATTR_ERR,     D2_CALL_ERR, D2_PAR_ERR, D2_CONSTRAINT_VIOLATION,
	D22_BASEOBJ_ERR,    D2_CALL_ERR, D2_PAR_ERR, D2_NOT_SPEC,
	D22_DNSIZE_ERR,     D2_CALL_ERR, D2_PAR_ERR, D2_NMSYNTAX_ERROR,
	D22_NMSYNTAX_ERR,   D2_CALL_ERR, D2_PAR_ERR, D2_NMSYNTAX_ERROR,
	D22_CHRANGE_ERR,    D2_CALL_ERR, D2_PAR_ERR, D2_NMSYNTAX_ERROR,
	D22_NOEXOBJ_ERR,    D2_CALL_ERR, D2_PAR_ERR, D2_NFOUND,
	D22_FLSIZE_ERR,     D2_PERM_ERR, D2_INT_ERR, D2_DB_ERR,
	D22_HSHTABFULL_ERR, D2_TEMP_ERR, D2_PAR_ERR, D2_DB_ERR,
	D22_NOMEM_ERR,      D2_PERM_ERR, D2_INT_ERR, D2_MEMORY_INSUFFICIENT,
	D22_INFSIZE_ERR,    D2_PERM_ERR, D2_INT_ERR, D2_DB_ERR,
	D22_INOPERABLE_ERR, D2_PERM_ERR, D2_INT_ERR, D2_NOT_SPEC,
	D22_IBUSY,	    D2_TEMP_ERR, D2_INT_ERR, D2_TOO_BUSY,
	0,                  D2_PERM_ERR, D2_NOT_SPEC,D2_NOT_SPEC } ;
#else
static D22_err_trans  d22_trans_tab[] = {
	D22_MSGSIZE_ERR,    D2_CALL_ERR, D2_PAR_ERR, D22_MSGSIZE_ERR,
	D22_MSYNTAX_ERR,    D2_PERM_ERR, D2_INT_ERR, D22_MSYNTAX_ERR,
	D22_REQUNKNOWN_ERR, D2_PERM_ERR, D2_INT_ERR, D22_REQUNKNOWN_ERR,
	D22_OPTYPE_ERR,     D2_CALL_ERR, D2_PAR_ERR, D22_OPTYPE_ERR,
	D22_VERSION_ERR,    D2_PERM_ERR, D2_INT_ERR, D22_VERSION_ERR,
	D22_DIRID_ERR,      D2_CALL_ERR, D2_PAR_ERR, D22_DIRID_ERR,
	D22_NOFNDATTR_ERR,  D2_CALL_ERR, D2_PAR_ERR, D22_NOFNDATTR_ERR,
	D22_NOATTR_ERR,     D2_CALL_ERR, D2_PAR_ERR, D22_NOATTR_ERR,
	D22_ATTSIZE_ERR,    D2_CALL_ERR, D2_PAR_ERR, D22_ATTSIZE_ERR,
	D22_MUATTR_ERR,     D2_CALL_ERR, D2_PAR_ERR, D22_MUATTR_ERR,
	D22_BASEOBJ_ERR,    D2_CALL_ERR, D2_PAR_ERR, D22_BASEOBJ_ERR,
	D22_DNSIZE_ERR,     D2_CALL_ERR, D2_PAR_ERR, D22_DNSIZE_ERR,
	D22_NMSYNTAX_ERR,   D2_CALL_ERR, D2_PAR_ERR, D22_NMSYNTAX_ERR,
	D22_CHRANGE_ERR,    D2_CALL_ERR, D2_PAR_ERR, D22_CHRANGE_ERR,
	D22_NOEXOBJ_ERR,    D2_CALL_ERR, D2_PAR_ERR, D22_NOEXOBJ_ERR,
	D22_FLSIZE_ERR,     D2_PERM_ERR, D2_INT_ERR, D22_FLSIZE_ERR,
	D22_HSHTABFULL_ERR, D2_TEMP_ERR, D2_PAR_ERR, D22_HSHTABFULL_ERR,
	D22_NOMEM_ERR,      D2_PERM_ERR, D2_INT_ERR, D22_NOMEM_ERR,
	D22_INFSIZE_ERR,    D2_PERM_ERR, D2_INT_ERR, D22_INFSIZE_ERR,
	D22_INOPERABLE_ERR, D2_PERM_ERR, D2_INT_ERR, D22_INOPERABLE_ERR,
	D22_IBUSY,	    D2_TEMP_ERR, D2_INT_ERR, D22_IBUSY,
	0,                  D2_PERM_ERR, D2_NOT_SPEC,D2_NOT_SPEC } ;
#endif

/* ------------------------------------------------------------------------- */

void d22_errorhdl (signed32 error, char *optptr) {

    D22_err_trans *p ;

    /* evaluate error no. and generate error message */
    if (error < D22_MINERRNO) {
	((D23_rserror *) optptr)->d23_Zretcod = D2_PERM_ERR ;
	((D23_rserror *) optptr)->d23_Zerrcls = D2_SYS_ERR ;
	((D23_rserror *) optptr)->d23_Zerrval = error ;
	((D23_rserror *) optptr)->d23_Zversion = D23_V1988 ;
	optptr = (char *) ((D23_rserror *) optptr + 1) ;
    } else {
	for (p = d22_trans_tab; p->d22_ival != 0; p++)
	    if (p->d22_ival == error) break ;
	((D23_rserror *) optptr)->d23_Zretcod = p->d22_ocode ;
	((D23_rserror *) optptr)->d23_Zerrcls = p->d22_oclass ;
	((D23_rserror *) optptr)->d23_Zerrval = p->d22_oval ;
	((D23_rserror *) optptr)->d23_Zversion = D23_V1988 ;
	optptr = (char *) ((D23_rserror *) optptr + 1) ;
	if (p->d22_oval == D2_NFOUND) {
	    ((D23_ds_name *) optptr)->d23_dstag = D23_NAMTAG ;
	    ((D23_ds_name *) optptr)->d23_dsoff =
				       ((D23_ds_name *) optptr)->d23_dslen = 0 ;
	    optptr = (char *) ((D23_ds_name *) optptr + 1) ;
	}
    }
    ((D23_eom *) optptr)->d23_eomtag = D23_EOMTAG ;
}
