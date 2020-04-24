/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d24stev.c,v $
 * Revision 1.1.10.2  1996/02/18  18:23:07  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:46:03  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:14:52  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:20:36  root]
 * 
 * Revision 1.1.8.3  1994/05/10  16:03:20  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:12:01  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:47:21  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:33:30  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  19:21:54  marrek
 * 	again
 * 	[1994/02/18  16:41:12  marrek]
 * 
 * 	February 1994 code drop
 * 	[1994/02/18  12:43:48  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:50:28  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  08:19:08  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:17:40  marrek]
 * 
 * Revision 1.1.4.3  1993/04/20  19:45:22  mhickey
 * 	DCT @ OSF (for mhickey)
 * 	Add new state to stevtab to
 * 	fix systest bug.
 * 	[1993/04/20  19:44:58  mhickey]
 * 
 * Revision 1.1.4.2  1992/12/31  22:03:24  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:47:28  bbelch]
 * 
 * Revision 1.1.2.3  1992/07/06  18:13:44  melman
 * 	CR 3975: xds test core dump client to client/server
 * 	[1992/07/06  18:11:36  melman]
 * 
 * Revision 1.1.2.2  1992/06/01  23:08:11  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:29:49  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char d24stev_rcsid[] = "@(#)$RCSfile: d24stev.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:23:07 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/


/* ------------------ STUB - state/event table ----------------------------- */

static D25_steventry stevtab[] = {

    /* *** STATE: IDLE *** */

    { D25_SEV(D25_IBIND_EV) | D25_SST(D25_IRIDLE) |
      D25_S1NA(3) | D25_S1SU(D25_IBIND_PENDING) |
      D25_S2NA(1) | D25_S2SU(D25_IRASSOCIATION) | D25_SAPAR(0) | D25_CONTFLG ,
      d25_getres, d25_apdutoasn1, d25_rosbinrq, d25_asn1toapdu } ,
    { D25_SEV(D25_IBIND_EV) | D25_SST(D25_IRIDLE) |
      D25_S1NA(1) | D25_S1SU(D25_IRASSOCIATION) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) | D25_EXTFLG ,
      d25_ipcresult } ,

    { D25_SEV(D25_RRETBUF_EV) | D25_SST(D25_IRIDLE) |
      D25_S1NA(1) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_relbuf } ,

    { D25_SEV(D25_IINVOKE_EV) | D25_SST(D25_IRIDLE) |
      D25_S1NA(4) | D25_S1SU(D25_IINVOKE_PENDING) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) | D25_CONTFLG ,
      d25_getres, d25_101_save_invoke_data, d25_apdutoasn1, d25_rosinvrq } ,
    { D25_SEV(D25_IINVOKE_EV) | D25_SST(D25_IRIDLE) |
      D25_S1NA(3) | D25_S1SU(D25_IRASSOCIATION) |
      D25_S2NA(1) | D25_S2SU(D25_IRIDLE) | D25_SAPAR(D25_POPRNSUC) | D25_EXTFLG ,
      d25_111_remove_cup_job , d25_ipcresult, d25_checkres, d25_relres } ,

    { D25_SEV(D25_IMINVOKE_EV) | D25_SST(D25_IRIDLE) |
      D25_S1NA(2) | D25_S1SU(D25_IINVOKE_PENDING) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_getres, d25_ipretbuf } ,

    { D25_SEV(D25_IEXCPT_EV) | D25_SST(D25_IRIDLE) |
      D25_S1NA(2) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_ipcabort, d25_relres } ,


    /* *** STATE: IPC-BIND-PENDING *** */

    { D25_SEV(D25_RCFBIND_EV) | D25_SST(D25_IBIND_PENDING) |
      D25_S1NA(3) | D25_S1SU(D25_IRASSOCIATION) |
      D25_S2NA(1) | D25_S2SU(D25_IRIDLE) | D25_SAPAR(0) | D25_CONTFLG,
      d25_rostrrq, d25_asn1toapdu, d25_ipcresult, d25_rosaborq } ,
    { D25_SEV(D25_RCFBIND_EV) | D25_SST(D25_IBIND_PENDING) |
      D25_S1NA(2) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) | D25_EXTFLG,
      d25_ipcabort, d25_relres } ,

    { D25_SEV(D25_RRJBIND_EV) | D25_SST(D25_IBIND_PENDING) |
      D25_S1NA(4) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) | D25_CONTFLG,
      d25_rostrrq, d25_asn1toapdu, d25_ipcresult, d25_relres } ,

    { D25_SEV(D25_RRJBIND_EV) | D25_SST(D25_IBIND_PENDING) |
      D25_S1NA(0) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(2) | D25_S2SU(D25_IRIDLE) | D25_SAPAR(0) | D25_EXTFLG,
      d25_ipcabort, d25_relres } ,

    { D25_SEV(D25_RABORT_EV) | D25_SST(D25_IBIND_PENDING) |
      D25_S1NA(2) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_ipcabort, d25_relres } ,


    /* *** STATE: ROS-ASSOCIATION *** */

    { D25_SEV(D25_IINVOKE_EV) | D25_SST(D25_IRASSOCIATION) |
      D25_S1NA(3) | D25_S1SU(D25_IINVOKE_PENDING) |
      D25_S2NA(1) | D25_S2SU(D25_IRASSOCIATION) | D25_SAPAR(0) | D25_CONTFLG ,
      d25_101_save_invoke_data, d25_apdutoasn1, d25_rosinvrq,
      d25_111_remove_cup_job } , 
    { D25_SEV(D25_IINVOKE_EV) | D25_SST(D25_IRASSOCIATION) |
      D25_S1NA(1) | D25_S1SU(D25_IRASSOCIATION) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) | D25_EXTFLG ,
      d25_ipcresult } ,

    { D25_SEV(D25_IMINVOKE_EV) | D25_SST(D25_IRASSOCIATION) |
      D25_S1NA(1) | D25_S1SU(D25_IINVOKE_PENDING) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_ipretbuf } ,

    { D25_SEV(D25_IABORT_EV) | D25_SST(D25_IRASSOCIATION) |
      D25_S1NA(3) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(D25_GTSKIP) ,
      d25_checkres, d25_rosaborq, d25_relres } ,

    { D25_SEV(D25_IUNBIND_EV) | D25_SST(D25_IRASSOCIATION) |
      D25_S1NA(3) | D25_S1SU(D25_IUNBIND_PENDING) |
      D25_S2NA(1) | D25_S2SU(D25_IRIDLE) | D25_SAPAR(D25_GTNSUC) | D25_CONTFLG ,
      d25_checkres, d25_apdutoasn1, d25_rosubnrq, d25_ipcresult } ,
    { D25_SEV(D25_IUNBIND_EV) | D25_SST(D25_IRASSOCIATION) |
      D25_S1NA(1) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) | D25_EXTFLG ,
      d25_relres } ,

    { D25_SEV(D25_RABORT_EV) | D25_SST(D25_IRASSOCIATION) |
      D25_S1NA(0) | D25_S1SU(D25_IR2ABORTED) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0)
      } ,


    /* *** STATE: IPC-INVOKE-PENDING *** */

    { D25_SEV(D25_IINVOKE_EV) | D25_SST(D25_IINVOKE_PENDING) |
      D25_S1NA(3) | D25_S1SU(D25_IINVOKE_PENDING) |
      D25_S2NA(1) | D25_S2SU(D25_IRASSOCIATION) | D25_SAPAR(0) | D25_CONTFLG ,
      d25_101_save_invoke_data, d25_apdutoasn1, d25_rosinvrq,
      d25_111_remove_cup_job } , 
    { D25_SEV(D25_IINVOKE_EV) | D25_SST(D25_IINVOKE_PENDING) |
      D25_S1NA(1) | D25_S1SU(D25_IRASSOCIATION) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) | D25_EXTFLG ,
      d25_ipcresult } , 

    { D25_SEV(D25_IMINVOKE_EV) | D25_SST(D25_IINVOKE_PENDING) |
      D25_S1NA(1) | D25_S1SU(D25_IINVOKE_PENDING) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_ipretbuf } ,

    { D25_SEV(D25_IRETBUF_EV) | D25_SST(D25_IINVOKE_PENDING) |
      D25_S1NA(2) | D25_S1SU(D25_IINVOKE_PENDING) |
      D25_S2NA(1) | D25_S2SU(D25_IINVOKE_PENDING) | D25_SAPAR(D25_ENDNSUC) |
						    D25_CONTFLG ,
      d25_ipcresult, d25_checkres, d25_checkres } ,

    { D25_SEV(D25_IRETBUF_EV) | D25_SST(D25_IINVOKE_PENDING) |
      D25_S1NA(1) | D25_S1SU(D25_IRASSOCIATION) |
      D25_S2NA(1) | D25_S2SU(D25_IRIDLE) | D25_SAPAR(D25_POPRNSUC) | D25_EXTFLG ,
      d25_checkres, d25_relres } ,

    { D25_SEV(D25_IABORT_EV) | D25_SST(D25_IINVOKE_PENDING) |
      D25_S1NA(3) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(D25_GTSKIP) ,
      d25_checkres, d25_rosaborq, d25_relres } ,

    { D25_SEV(D25_RRESULT_EV) | D25_SST(D25_IINVOKE_PENDING) |
      D25_S1NA(4) | D25_S1SU(D25_ICINVOKE_PENDING) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) | D25_CONTFLG ,
      d25_asn1toapdu, d25_rorjurq, d25_121_save_result_data, d25_ipcbind } ,
    { D25_SEV(D25_RRESULT_EV) | D25_SST(D25_IINVOKE_PENDING) |
      D25_S1NA(2) | D25_S1SU(D25_ICINVOKE_PENDING) |
      D25_S2NA(2) | D25_S2SU(D25_IINVOKE_PENDING) | D25_SAPAR(D25_ENDNSUC) |
						    D25_CONTFLG | D25_EXTFLG ,
      d25_ipcinvoke, d25_141_restore_env, d25_ipcresult, d25_checkres } ,
    { D25_SEV(D25_RRESULT_EV) | D25_SST(D25_IINVOKE_PENDING) |
      D25_S1NA(1) | D25_S1SU(D25_IRASSOCIATION) |
      D25_S2NA(1) | D25_S2SU(D25_IRIDLE) | D25_SAPAR(D25_POPRNSUC) | D25_EXTFLG ,
      d25_checkres, d25_relres } ,

    { D25_SEV(D25_RERROR_EV) | D25_SST(D25_IINVOKE_PENDING) |
      D25_S1NA(4) | D25_S1SU(D25_IRASSOCIATION) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) | D25_CONTFLG ,
      d25_asn1toapdu, d25_rorjurq, d25_ipcresult, d25_111_remove_cup_job } ,
    { D25_SEV(D25_RERROR_EV) | D25_SST(D25_IINVOKE_PENDING) |
      D25_S1NA(1) | D25_S1SU(D25_IRASSOCIATION) |
      D25_S2NA(1) | D25_S2SU(D25_IRIDLE) | D25_SAPAR(D25_POPRNSUC) | D25_EXTFLG ,
      d25_checkres, d25_relres } ,

    { D25_SEV(D25_RUSRREJ_EV) | D25_SST(D25_IINVOKE_PENDING) |
      D25_S1NA(3) | D25_S1SU(D25_IRASSOCIATION) |
      D25_S2NA(1) | D25_S2SU(D25_IRIDLE) | D25_SAPAR(D25_POPRNSUC) ,
      d25_ipcresult, d25_111_remove_cup_job, d25_checkres, d25_relres } ,

    { D25_SEV(D25_RPROVREJ_EV) | D25_SST(D25_IINVOKE_PENDING) |
      D25_S1NA(3) | D25_S1SU(D25_IRASSOCIATION) |
      D25_S2NA(1) | D25_S2SU(D25_IRIDLE) | D25_SAPAR(D25_POPRNSUC) ,
      d25_ipcresult, d25_111_remove_cup_job, d25_checkres, d25_relres } ,

    { D25_SEV(D25_RABORT_EV) | D25_SST(D25_IINVOKE_PENDING) |
      D25_S1NA(3) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_ipcabort, d25_111_remove_cup_job, d25_relres } ,


    /* *** STATE: CACHE-UPDATE-PENDING *** */

    { D25_SEV(D25_IRESULT_EV) | D25_SST(D25_ICINVOKE_PENDING) |
      D25_S1NA(2) | D25_S1SU(D25_ICINVOKE_PENDING) |
      D25_S2NA(2) | D25_S2SU(D25_IRIDLE) | D25_SAPAR(0) ,
      d25_151_continue_update, d25_ipcinvoke, d25_ipcunbind, d25_relres } ,

    { D25_SEV(D25_IRETBUF_EV) | D25_SST(D25_ICINVOKE_PENDING) |
      D25_S1NA(1) | D25_S1SU(D25_ICINVOKE_PENDING) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_ipcinvoke } ,

    { D25_SEV(D25_IERROR_EV) | D25_SST(D25_ICINVOKE_PENDING) |
      D25_S1NA(2) | D25_S1SU(D25_ICINVOKE_PENDING) |
      D25_S2NA(2) | D25_S2SU(D25_IRIDLE) | D25_SAPAR(0) ,
      d25_151_continue_update, d25_ipcinvoke, d25_ipcunbind, d25_relres } ,

    { D25_SEV(D25_IREJECT_EV) | D25_SST(D25_ICINVOKE_PENDING) |
      D25_S1NA(2) | D25_S1SU(D25_ICINVOKE_PENDING) |
      D25_S2NA(2) | D25_S2SU(D25_IRIDLE) | D25_SAPAR(0) ,
      d25_151_continue_update, d25_ipcinvoke, d25_ipcunbind, d25_relres } ,

    { D25_SEV(D25_IABORT_EV) | D25_SST(D25_ICINVOKE_PENDING) |
      D25_S1NA(3) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_111_remove_cup_job, d25_ipcunbind, d25_relres } ,


    /* *** STATE: IPC-UNBIND-PENDING *** */

    { D25_SEV(D25_RABORT_EV) | D25_SST(D25_IUNBIND_PENDING) |
      D25_S1NA(2) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_ipcabort, d25_relres } ,

    { D25_SEV(D25_RCFUBIND_EV) | D25_SST(D25_IUNBIND_PENDING) |
      D25_S1NA(3) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_asn1toapdu, d25_ipcresult, d25_relres } ,


    /* *** STATE: ROS-ABORTED 2 *** */

    { D25_SEV(D25_IINVOKE_EV) | D25_SST(D25_IR2ABORTED) |
      D25_S1NA(2) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_ipcabort, d25_relres } ,

    { D25_SEV(D25_IMINVOKE_EV) | D25_SST(D25_IR2ABORTED) |
      D25_S1NA(2) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_ipcabort, d25_relres } ,

    { D25_SEV(D25_IABORT_EV) | D25_SST(D25_IR2ABORTED) |
      D25_S1NA(1) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_relres } ,

    { D25_SEV(D25_IUNBIND_EV) | D25_SST(D25_IR2ABORTED) |
      D25_S1NA(2) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_ipcabort, d25_relres } ,


    /* *** STATE: INITIALIZATION *** */

    { D25_SEV(0) | D25_SST(D25_IRINIT) |
      D25_S1NA(4) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(D23_SYNC_EM) | D25_CONTFLG,
      d25_initasn1, d25_ipccltatt, d25_regproc, d25_ipcbind } ,
    { D25_SEV(0) | D25_SST(D25_IRINIT) |
      D25_S1NA(4) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(D23_FASYNC_EM) | 
						       D25_CONTFLG | D25_EXTFLG,
      d25_getpsap, d25_unregproc, d25_ipcltdet, d25_ipccltatt } ,
    { D25_SEV(0) | D25_SST(D25_IRINIT) |
      D25_S1NA(3) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(D23_FASYNC_EM) | D25_EXTFLG,
      d25_ipcsvratt, d25_rosattach, d25_rosprbf }
} ;
