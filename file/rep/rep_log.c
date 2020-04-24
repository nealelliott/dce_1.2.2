/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rep_log.c,v $
 * $EndLog$
 */
/*
 * (C) Copyright 1990, 1995 Transarc Corporation
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 */

/* rep_log.c: logging operations. */

#include <time.h>
#include <dcedfs/param.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdio.h>
#include <dcedfs/osi.h>
#include <dcedfs/ftutil.h>

/* SAMS support */

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsrpsmac.h>
#include <dfsrpssvc.h>
#include <dfsrpsmsg.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

static FILE *logIO = NULL;

void Log(s, a, b, c, d, e, f, g, h, i, j)
char *s;
{
    long bogusSignedNOW;
    struct tm *NowP;
    static char *months[12] = {
	"Jan", "Feb", "Mar", "Apr", "May", "Jun",
	"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };

    bogusSignedNOW = osi_Time();
    NowP = (struct tm *) localtime(&bogusSignedNOW);
    fprintf(logIO, "%02d-%s-%02d %02d:%02d:%02d ",
	     (NowP->tm_year) % 100,
	     months[NowP->tm_mon],
	     NowP->tm_mday,
	     NowP->tm_hour, NowP->tm_min, NowP->tm_sec);
    fprintf(logIO, s, a, b, c, d, e, f, g, h, i, j);
    putc('\n', logIO);
    fflush(logIO);
}

void initRepLog(myName)
char *myName;
{/* Initialize the log service. */
    char LOGFILE[500], OLOGFILE[500];

    strcpy(LOGFILE, DCELOCAL_PATH);
    strcat(LOGFILE, "/var/dfs/adm/RepLog");
    strcpy(OLOGFILE, LOGFILE);
    strcat(OLOGFILE, ".old");
    rename(LOGFILE, OLOGFILE);	/* don't check error code */
    logIO = fopen(LOGFILE, "w");
    if (!logIO) {
	dce_svc_printf(RPS_S_BAD_OPEN_MSG,LOGFILE);
	exit(1);
    }
    ftu_SetLogStream(logIO);
    Log("%s: log initialized to %s", myName, LOGFILE);
}
