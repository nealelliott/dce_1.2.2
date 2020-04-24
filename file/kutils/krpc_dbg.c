/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: krpc_dbg.c,v $
 * Revision 1.1.101.1  1996/10/02  17:53:10  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:41:29  damon]
 *
 * Revision 1.1.96.3  1994/07/13  22:28:33  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  11:38:13  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:59:46  mbs]
 * 
 * Revision 1.1.96.2  1994/06/09  14:12:07  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:25:37  annie]
 * 
 * Revision 1.1.96.1  1994/02/04  20:21:31  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:14:26  devsrc]
 * 
 * Revision 1.1.94.1  1993/12/07  17:27:17  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/06  13:41:38  jaffe]
 * 
 * $EndLog$
 */

/* These routines help krpc logging interface with the
 * dfs icl package.
 */

#include <dcedfs/icl.h>
#include <stdarg.h>
#include <krpcdbg.h>

#ifdef AFS_OSF11_ENV
#define DEFAULT_LOGSIZE 4*1024
#else
#define DEFAULT_LOGSIZE 60*1024
#endif

struct icl_set *krpc_iclSetp=0;
struct icl_log *krpc_logp=0;

int
afscall_krpcdbg(opcode, p1, p2, p3, p4, retval)
long opcode;
long p1, p2, p3, p4;
long *retval;

{
    char logname[65];
    char debug_level[15];
    long temp;
    long code;

    if (!osi_suser(osi_getucred()))
         return EACCES;
    code=0;

    switch (opcode){
    case KRPC_DBG_INIT:
        if (krpc_logp == (struct icl_log *) 0){
            code = osi_copyinstr(p1, logname, sizeof(logname), &temp);
	    if (code)
                return(code);
            code = icl_CreateLog(logname, DEFAULT_LOGSIZE, &krpc_logp);
            if (code)
                return(code);
	  }
        if (krpc_iclSetp == (struct icl_set *) 0){
            code=icl_CreateSet("krpc", krpc_logp, (struct icl_log *) 0, &krpc_iclSetp);
       
	  }
        break;
    case KRPC_DBG_SET: 
         if ((krpc_logp == (struct icl_log *)0) ||
             (krpc_iclSetp == (struct icl_set *)0))
             return(EIO);
         code = osi_copyinstr(p1, debug_level, sizeof (debug_level), &temp);
         if (!code)
             rpc__dbg_set_switches(debug_level, &code);
         break;
    default:
         return EINVAL;
    }
    return (code);
}

char krpc_icl_pbuf[1024];

dfs_icl_printf(char *fmt, ... )
{
    va_list ap;

    va_start(ap, fmt);
    fmt = va_arg(ap, char *);

#ifdef	AFS_HPUX_ENV
    {
	char *iclbufp = krpc_icl_pbuf;
	int len = sizeof(krpc_icl_pbuf);

	/* this is the in-kernel sprintf */
	prf(fmt, ap, (int)3, &iclbufp, &len);

	/* remove any trailing NEWLINE */
	iclbufp -= 2;
	if (*iclbufp == '\n')
	  *iclbufp = '\0';
    }
#else
    vsprintf(krpc_icl_pbuf, fmt, ap);
#endif	/* AFS_HPUX_ENV */

    va_end(ap);    

    icl_Trace1(krpc_iclSetp, DFS_TRACE_RPC, ICL_TYPE_STRING, krpc_icl_pbuf);

}
