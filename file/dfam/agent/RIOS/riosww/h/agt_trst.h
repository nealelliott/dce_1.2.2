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
# ifndef RCS_agt_trst_h_Included
# define RCS_agt_trst_h_Included
 static char *RCSID_agt_trst_h="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/h/agt_trst.h,v 1.1.2.2 1996/03/11 14:06:27 marty Exp $";
# endif /* Not RCS_agt_trst_h_Included */
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_trst.h,v $
 * Revision 1.1.2.2  1996/03/11  14:06:27  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:56:07  marty]
 *
 * Revision 1.1.2.1  1996/02/19  09:13:26  dce-sdc
 * 	First Submit
 * 	[1996/02/19  09:12:55  dce-sdc]
 * 
 * Revision 1.1.2.3  96/01/12  06:40:52  06:40:52  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  06:40:42  kishi]
 * 
 * Revision 1.1.2.2  1995/10/26  03:00:13  kishi
 * 	Change Copyright.
 * 	[1995/10/26  02:59:03  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  07:35:26  kishi
 * 	Initial revision
 * 	[1995/09/09  07:23:56  kishi]
 * 
 * $EndLog$
 */

#ifndef _AGT_TRST_H
#define _AGT_TRST_H

#include <unistd.h>
#include <netinet/in.h>
#include <signal.h>
#include <time.h>



struct d_trst_lenstr {
    int       len;
    char      str[10240000];
};


struct d_trst_fork {
    pid_t     pid;
    char      ptype[4];
    int       errno;
    struct in_addr ipaddr;
    int            port;
};

struct d_trst_signal {
    int       sig;
    int       code;
    struct sigcontext scp;
};

struct d_trst_umsgrv {
    int       errno;
    int       fromlen;
    char      fromadr[14];

};

struct d_trst_umsgsd {
    int       errno;
    int       tolen;
    char      toadr[14];

};

struct d_trst_gmsgrv {
    struct in_addr gwipaddr;
    int       gwport;
    int       errno;
    int       numrecvd;
    int       len_torecv;
    int       len_recvd;
};

struct d_trst_gmsgsd {
    struct in_addr gwipaddr;
    int       gwport;
    int       errno;
    int       numrecvd;
    int       len_tosend;
    int       len_sent;
};

struct d_trst_rcsget {
    int       rsctype;
    int       errno;
    int       key;
    int       size;
};

struct d_trst_syslog {
    int       prio;
    int       len;
};

struct d_trst_gwmesg {
    int       wdpid;
    int       len;
};

struct d_trst_kill {
    int       reason;
#define D_KILL_BY_ABORT        1
#define D_KILL_BY_RECONN       2
#define D_KILL_BY_GWDOWN       3
    pid_t     pid;
    char      ptype[4];
    int       errno;
    in_addr   ipaddr;
    int       port;
    int       conn_no;
};


#endif
