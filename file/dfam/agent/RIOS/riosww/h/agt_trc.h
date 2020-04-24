/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * Trade Secret of Hitachi, Ltd.
 * Do not disclose without written concession from Hitachi, Ltd.
 * Copyright 1996 Hitachi, Ltd. All Rights Reserved.  Derechos Reservados.
 */
#ifdef HITACHI_REV_CHECK
# ifndef RCS_agt_trc_h_Included
# define RCS_agt_trc_h_Included
 static char *RCSID_agt_trc_h="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/h/agt_trc.h,v 1.1.2.2 1996/03/11 14:06:26 marty Exp $";
# endif /* Not RCS_agt_trc_h_Included */
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_trc.h,v $
 * Revision 1.1.2.2  1996/03/11  14:06:26  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:56:06  marty]
 *
 * Revision 1.1.2.1  1996/02/19  09:14:46  dce-sdc
 * 	First Submit
 * 	[1996/02/19  09:14:21  dce-sdc]
 * 
 * Revision 1.1.2.3  96/01/12  06:39:23  06:39:23  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  06:39:04  kishi]
 * 
 * Revision 1.1.2.2  1995/10/26  02:57:29  kishi
 * 	Change Copyright.
 * 	[1995/10/26  02:57:21  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  07:35:46  kishi
 * 	Initial revision
 * 	[1995/09/09  07:23:30  kishi]
 * 
 * $EndLog$
 */

#ifndef _AGT_TRC_H
#define _AGT_TRC_H


#define D_TRC_COMMDATA      32
#define D_TRCID_INVAL       "********"

#define D_TRC_EVE_INVALID_32 "********************************"

#define D_TRCID_EVTRC       "ET"
#define D_TRC_FILE_PATH     "/opt/dcelocal/var/dfa/adm"
#define D_TRC_FILE_BASE     "evtrace"
#define D_EVTRC_FNAME_MAX   64

#define D_TRC_LEN_MAX       2016
#define D_TRC_UNIT          32
#define D_TRC_MSG_OFFSET    12
#define D_TRC_ERRMSG_MAX    256


#define D_TRC_ST_INIT       0x00010000
#define D_TRC_ST_INIT_D     0x0001F000
#define D_TRC_ST_RUN        0x00040000
#define D_TRC_ST_TERM       0x00080000



struct d_trc {
    char tableid[4];

    int memtr_on;
    int evttr_on;
    int prctr_on;

    int memtrsize;
    int evttrfiles;
    int evttrsize;
    int prctrsize;

    char *ptbuf;
    int proc_cur;

    int tmsgbufsize;
    char *tmsgb;
    int tmsglen;


    char evfilename[D_EVTRC_FNAME_MAX];
    int tracefd;

    int secondcall;
    char errmsgbuf[D_TRC_ERRMSG_MAX];
};


extern struct d_trc trc;


extern void agt_trace(void);
extern void Agenttrace(char *id, int len, char *data, int info);

#endif
