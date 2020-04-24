/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d25stev.c,v $
 * Revision 1.1.10.2  1996/02/18  18:23:27  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:46:23  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:15:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:21:01  root]
 * 
 * Revision 1.1.8.1  1994/02/22  17:55:29  marrek
 * 	February 1994 code drop
 * 	[1994/02/21  09:22:21  marrek]
 * 
 * 	February 1994 code drop
 * 	[1994/02/18  12:43:52  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:52:36  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  08:31:20  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:20:29  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  22:04:28  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:48:16  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  18:29:08  marrek
 * 	November 1992 code drop
 * 	[1992/11/23  15:09:15  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:10:11  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:33:40  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char d25stev_rcsid[] = "@(#)$RCSfile: d25stev.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:23:27 $";
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
      D25_S1NA(2) | D25_S1SU(D25_IBIND_PENDING) |
      D25_S2NA(1) | D25_S2SU(D25_IRASSOCIATION) | D25_SAPAR(0) ,
      d25_getres, d25_rosbinrq, d25_ipcresult } ,
    { D25_SEV(D25_RBIND_EV) | D25_SST(D25_IRIDLE) |
      D25_S1NA(4) | D25_S1SU(D25_RBIND_PENDING) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) | D25_CONTFLG ,
      d25_getres, d25_rostrrq, d25_rosprbf, d25_ipcbind } ,
    { D25_SEV(D25_RBIND_EV) | D25_SST(D25_IRIDLE) |
      D25_S1NA(1) | D25_S1SU(D25_RBIND_PENDING) |
      D25_S2NA(2) | D25_S2SU(D25_IRIDLE) | D25_SAPAR(0) | D25_EXTFLG ,
      d25_ipcinvoke, d25_rosaborq, d25_relres } ,
    { D25_SEV(D25_RRETBUF_EV) | D25_SST(D25_IRIDLE) |
      D25_S1NA(1) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_relbuf } ,
    { D25_SEV(D25_REXCPT_EV) | D25_SST(D25_IRIDLE) |
      D25_S1NA(1) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_rosaborq } ,
    { D25_SEV(D25_IEXCPT_EV) | D25_SST(D25_IRIDLE) |
      D25_S1NA(2) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_ipcabort, d25_relres } ,

    /* *** STATE: IPC-BIND-PENDING *** */

    { D25_SEV(D25_RCFBIND_EV) | D25_SST(D25_IBIND_PENDING) |
      D25_S1NA(2) | D25_S1SU(D25_IRASSOCIATION) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_rostrrq, d25_ipcresult } ,
    { D25_SEV(D25_RRJBIND_EV) | D25_SST(D25_IBIND_PENDING) |
      D25_S1NA(2) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_ipcresult, d25_relres } ,
    { D25_SEV(D25_RABORT_EV) | D25_SST(D25_IBIND_PENDING) |
      D25_S1NA(2) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_ipcabort, d25_relres } ,

    /* *** STATE: ROS-BIND-PENDING *** */

    { D25_SEV(D25_IRESULT_EV) | D25_SST(D25_RBIND_PENDING) |
      D25_S1NA(1) | D25_S1SU(D25_IRASSOCIATION) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_robinrs } ,
    { D25_SEV(D25_IERROR_EV) | D25_SST(D25_RBIND_PENDING) |
      D25_S1NA(3) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_robinrs, d25_ipcunbind, d25_relres } ,
    { D25_SEV(D25_IREJECT_EV) | D25_SST(D25_RBIND_PENDING) |
      D25_S1NA(3) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_robinrs, d25_ipcunbind, d25_relres } ,
    { D25_SEV(D25_IABORT_EV) | D25_SST(D25_RBIND_PENDING) |
      D25_S1NA(3) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_rosaborq, d25_ipcunbind, d25_relres } ,
    { D25_SEV(D25_RABORT_EV) | D25_SST(D25_RBIND_PENDING) |
      D25_S1NA(0) | D25_S1SU(D25_R1ABORTED) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0)
      } ,

    /* *** STATE: ROS-ASSOCIATION *** */

    { D25_SEV(D25_IINVOKE_EV) | D25_SST(D25_IRASSOCIATION) |
      D25_S1NA(1) | D25_S1SU(D25_IINVOKE_PENDING) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_rosinvrq } ,
    { D25_SEV(D25_IMINVOKE_EV) | D25_SST(D25_IRASSOCIATION) |
      D25_S1NA(1) | D25_S1SU(D25_IRASSOCIATION) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_ipretbuf } ,
    { D25_SEV(D25_IABORT_EV) | D25_SST(D25_IRASSOCIATION) |
      D25_S1NA(3) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(D25_GTSKIP) ,
      d25_checkres, d25_rosaborq, d25_relres } ,
    { D25_SEV(D25_IUNBIND_EV) | D25_SST(D25_IRASSOCIATION) |
      D25_S1NA(2) | D25_S1SU(D25_IUNBIND_PENDING) |
      D25_S2NA(2) | D25_S2SU(D25_IRIDLE) | D25_SAPAR(D25_GTNSUC) ,
      d25_checkres, d25_rosubnrq, d25_ipcresult, d25_relres } ,
    { D25_SEV(D25_RINVOKE_EV) | D25_SST(D25_IRASSOCIATION) |
      D25_S1NA(4) | D25_S1SU(D25_RINVOKE_PENDING) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_getres, d25_ipcbind, d25_ipcinvoke, d25_rosprbf } ,
    { D25_SEV(D25_RABORT_EV) | D25_SST(D25_IRASSOCIATION) |
      D25_S1NA(1) | D25_S1SU(D25_IR2ABORTED) |
      D25_S2NA(3) | D25_S2SU(D25_IRIDLE) | D25_SAPAR(D25_ACTNSUC) ,
      d25_checkres, d25_ipcabort, d25_ipcunbind, d25_relres } ,
    { D25_SEV(D25_RUSRREJ_EV) | D25_SST(D25_IRASSOCIATION) |
      D25_S1NA(0) | D25_S1SU(D25_IRASSOCIATION) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0)
      } ,
    { D25_SEV(D25_RPROVREJ_EV) | D25_SST(D25_IRASSOCIATION) |
      D25_S1NA(4) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_rosaborq, d25_ipcabort, d25_ipcunbind, d25_relres } ,
    { D25_SEV(D25_RUNBIND_EV) | D25_SST(D25_IRASSOCIATION) |
      D25_S1NA(2) | D25_S1SU(D25_R2UNBIND_PENDING) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_ipcinvoke, d25_rosprbf } ,
    { D25_SEV(D25_REXCPT_EV) | D25_SST(D25_IRASSOCIATION) |
      D25_S1NA(1) | D25_S1SU(D25_IRASSOCIATION) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_rorjurq } ,

    /* *** STATE: IPC-INVOKE-PENDING *** */

    { D25_SEV(D25_IINVOKE_EV) | D25_SST(D25_IINVOKE_PENDING) |
      D25_S1NA(1) | D25_S1SU(D25_IINVOKE_PENDING) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_rosinvrq } ,
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
      D25_S2NA(1) | D25_S2SU(D25_IRIDLE) | D25_SAPAR(D25_POPRNSUC) | D25_EXTFLG
,
      d25_checkres, d25_relres } ,
    { D25_SEV(D25_IABORT_EV) | D25_SST(D25_IINVOKE_PENDING) |
      D25_S1NA(3) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(D25_GTSKIP) ,
      d25_checkres, d25_rosaborq, d25_relres } ,
    { D25_SEV(D25_RRESULT_EV) | D25_SST(D25_IINVOKE_PENDING) |
      D25_S1NA(2) | D25_S1SU(D25_IINVOKE_PENDING) |
      D25_S2NA(0) | D25_S2SU(D25_IRASSOCIATION) | D25_SAPAR(D25_ENDNSUC) ,
      d25_ipcresult, d25_checkres } ,
    { D25_SEV(D25_RERROR_EV) | D25_SST(D25_IINVOKE_PENDING) |
      D25_S1NA(1) | D25_S1SU(D25_IRASSOCIATION) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_ipcresult } ,
    { D25_SEV(D25_RUSRREJ_EV) | D25_SST(D25_IINVOKE_PENDING) |
      D25_S1NA(1) | D25_S1SU(D25_IRASSOCIATION) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_ipcresult } ,
    { D25_SEV(D25_RPROVREJ_EV) | D25_SST(D25_IINVOKE_PENDING) |
      D25_S1NA(1) | D25_S1SU(D25_IRASSOCIATION) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_ipcresult } ,
    { D25_SEV(D25_RABORT_EV) | D25_SST(D25_IINVOKE_PENDING) |
      D25_S1NA(2) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_ipcabort, d25_relres } ,

    /* *** STATE: ROS-INVOKE-PENDING *** */

    { D25_SEV(D25_IRESULT_EV) | D25_SST(D25_RINVOKE_PENDING) |
      D25_S1NA(2) | D25_S1SU(D25_IRASSOCIATION) |
      D25_S2NA(2) | D25_S2SU(D25_IRIDLE) | D25_SAPAR(D25_GTNSUC) ,
      d25_rosresrq, d25_checkres, d25_ipcunbind, d25_relres } ,
    { D25_SEV(D25_IMRESULT_EV) | D25_SST(D25_RINVOKE_PENDING) |
      D25_S1NA(1) | D25_S1SU(D25_RINVOKE_PENDING) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_ipretbuf } ,
    { D25_SEV(D25_IRETBUF_EV) | D25_SST(D25_RINVOKE_PENDING) |
      D25_S1NA(1) | D25_S1SU(D25_RINVOKE_PENDING) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_ipcinvoke } ,
    { D25_SEV(D25_IERROR_EV) | D25_SST(D25_RINVOKE_PENDING) |
      D25_S1NA(2) | D25_S1SU(D25_IRASSOCIATION) |
      D25_S2NA(2) | D25_S2SU(D25_IRIDLE) | D25_SAPAR(D25_GTNSUC) ,
      d25_roerrrq, d25_checkres, d25_ipcunbind, d25_relres } ,
    { D25_SEV(D25_IREJECT_EV) | D25_SST(D25_RINVOKE_PENDING) |
      D25_S1NA(2) | D25_S1SU(D25_IRASSOCIATION) |
      D25_S2NA(2) | D25_S2SU(D25_IRIDLE) | D25_SAPAR(D25_GTNSUC) ,
      d25_rorjurq, d25_checkres, d25_ipcunbind, d25_relres } ,
    { D25_SEV(D25_IABORT_EV) | D25_SST(D25_RINVOKE_PENDING) |
      D25_S1NA(4) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(D25_CONTSKIP) | D25_CONTFLG,
      d25_checkres, d25_rorjurq, d25_checkres, d25_rosaborq } ,
    { D25_SEV(D25_IABORT_EV) | D25_SST(D25_RINVOKE_PENDING) |
      D25_S1NA(2) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) | D25_EXTFLG,
      d25_ipcunbind, d25_relres } ,
    { D25_SEV(D25_RABORT_EV) | D25_SST(D25_RINVOKE_PENDING) |
      D25_S1NA(0) | D25_S1SU(D25_R1ABORTED) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0)
      } ,
    { D25_SEV(D25_RPROVREJ_EV) | D25_SST(D25_RINVOKE_PENDING) |
      D25_S1NA(0) | D25_S1SU(D25_R1ABORTED) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0)
      } ,
    { D25_SEV(D25_RUNBIND_EV) | D25_SST(D25_RINVOKE_PENDING) |
      D25_S1NA(1) | D25_S1SU(D25_R1UNBIND_PENDING) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      } ,

    /* *** STATE: IPC-UNBIND-PENDING *** */

    { D25_SEV(D25_RABORT_EV) | D25_SST(D25_IUNBIND_PENDING) |
      D25_S1NA(2) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_ipcabort, d25_relres } ,
    { D25_SEV(D25_RCFUBIND_EV) | D25_SST(D25_IUNBIND_PENDING) |
      D25_S1NA(2) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_ipcresult, d25_relres } ,

    /* *** STATE: ROS-UNBIND-PENDING 1 *** */

    { D25_SEV(D25_IRESULT_EV) | D25_SST(D25_R1UNBIND_PENDING) |
      D25_S1NA(4) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(D25_GTSKIP) | D25_CONTFLG,
      d25_ipcabort, d25_ipcunbind, d25_checkres, d25_rosaborq } ,
    { D25_SEV(D25_IRESULT_EV) | D25_SST(D25_R1UNBIND_PENDING) |
      D25_S1NA(1) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) | D25_EXTFLG,
      d25_relres } ,
    { D25_SEV(D25_IMRESULT_EV) | D25_SST(D25_R1UNBIND_PENDING) |
      D25_S1NA(4) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(D25_GTSKIP) | D25_CONTFLG,
      d25_ipcabort, d25_ipcunbind, d25_checkres, d25_rosaborq } ,
    { D25_SEV(D25_IMRESULT_EV) | D25_SST(D25_R1UNBIND_PENDING) |
      D25_S1NA(1) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) | D25_EXTFLG,
      d25_relres } ,
    { D25_SEV(D25_IERROR_EV) | D25_SST(D25_R1UNBIND_PENDING) |
      D25_S1NA(4) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(D25_GTSKIP) | D25_CONTFLG,
      d25_ipcabort, d25_ipcunbind, d25_checkres, d25_rosaborq } ,
    { D25_SEV(D25_IERROR_EV) | D25_SST(D25_R1UNBIND_PENDING) |
      D25_S1NA(1) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) | D25_EXTFLG,
      d25_relres } ,
    { D25_SEV(D25_IREJECT_EV) | D25_SST(D25_R1UNBIND_PENDING) |
      D25_S1NA(4) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(D25_GTSKIP) | D25_CONTFLG,
      d25_ipcabort, d25_ipcunbind, d25_checkres, d25_rosaborq } ,
    { D25_SEV(D25_IREJECT_EV) | D25_SST(D25_R1UNBIND_PENDING) |
      D25_S1NA(1) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) | D25_EXTFLG,
      d25_relres } ,
    { D25_SEV(D25_IABORT_EV) | D25_SST(D25_R1UNBIND_PENDING) |
      D25_S1NA(4) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(D25_GTSKIP) ,
      d25_ipcunbind, d25_checkres, d25_rosaborq, d25_relres } ,
    { D25_SEV(D25_RABORT_EV) | D25_SST(D25_R1UNBIND_PENDING) |
      D25_S1NA(0) | D25_S1SU(D25_R1ABORTED) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0)
      } ,

    /* *** STATE: ROS-UNBIND-PENDING 2 *** */

    { D25_SEV(D25_IRESULT_EV) | D25_SST(D25_R2UNBIND_PENDING) |
      D25_S1NA(3) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_ipcunbind, d25_roubnrs, d25_relres } ,
    { D25_SEV(D25_IERROR_EV) | D25_SST(D25_R2UNBIND_PENDING) |
      D25_S1NA(4) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_ipcabort, d25_ipcunbind, d25_rosaborq, d25_relres } ,
    { D25_SEV(D25_IREJECT_EV) | D25_SST(D25_R2UNBIND_PENDING) |
      D25_S1NA(4) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_ipcabort, d25_ipcunbind, d25_rosaborq, d25_relres } ,
    { D25_SEV(D25_IABORT_EV) | D25_SST(D25_R2UNBIND_PENDING) |
      D25_S1NA(3) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_ipcunbind, d25_rosaborq, d25_relres } ,
    { D25_SEV(D25_RABORT_EV) | D25_SST(D25_R2UNBIND_PENDING) |
      D25_S1NA(0) | D25_S1SU(D25_R1ABORTED) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0)
      } ,

    /* *** STATE: ROS-ABORTED 1 *** */

    { D25_SEV(D25_IRESULT_EV) | D25_SST(D25_R1ABORTED) |
      D25_S1NA(3) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_ipcabort, d25_ipcunbind, d25_relres } ,
    { D25_SEV(D25_IMRESULT_EV) | D25_SST(D25_R1ABORTED) |
      D25_S1NA(3) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_ipcabort, d25_ipcunbind, d25_relres } ,
    { D25_SEV(D25_IRETBUF_EV) | D25_SST(D25_R1ABORTED) |
      D25_S1NA(3) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_ipcabort, d25_ipcunbind, d25_relres } ,
    { D25_SEV(D25_IERROR_EV) | D25_SST(D25_R1ABORTED) |
      D25_S1NA(3) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_ipcabort, d25_ipcunbind, d25_relres } ,
    { D25_SEV(D25_IREJECT_EV) | D25_SST(D25_R1ABORTED) |
      D25_S1NA(3) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_ipcabort, d25_ipcunbind, d25_relres } ,
    { D25_SEV(D25_IABORT_EV) | D25_SST(D25_R1ABORTED) |
      D25_S1NA(2) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) ,
      d25_ipcunbind, d25_relres } ,

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
      d25_ipccltatt, d25_regproc, d25_ipcbind, d25_getpsap } ,
    { D25_SEV(0) | D25_SST(D25_IRINIT) |
      D25_S1NA(4) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(D23_FASYNC_EM) | 
						D25_CONTFLG | D25_EXTFLG,
      d25_unregproc, d25_ipcltdet, d25_ipccltatt, d25_ipcsvratt } ,
    { D25_SEV(0) | D25_SST(D25_IRINIT) |
      D25_S1NA(2) | D25_S1SU(D25_IRIDLE) |
      D25_S2NA(0) | D25_S2SU(0) | D25_SAPAR(0) | D25_EXTFLG,
      d25_rosattach, d25_rosprbf }
} ;
