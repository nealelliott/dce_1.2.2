/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: dcd_st_dbg.c,v $
 * Revision 1.1.4.2  1996/02/17  23:26:32  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:20  marty]
 *
 * Revision 1.1.4.1  1995/12/11  21:55:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:55:10  root]
 * 
 * Revision 1.1.2.2  1994/06/17  18:46:05  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:36:41  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/26  22:44:15  fadden
 * 	Prepare for initial submission
 * 	[1994/05/26  20:10:28  fadden]
 * 
 * 	Initial version of local debug facility
 * 	[1994/05/20  20:01:27  fadden]
 * 
 * $EndLog$
 */
/*
 * MODULE NAME:    dcd_st_dbg.c
 *
 * COMPONENT:      DCD System Test
 *
 * ABSTRACT:       DCD test debug facility, records to stderr
 *
 *                 tst_init_dbg        initialize debug facility
 *                 tst_log_msg         write message to debug file
 *                 tst_get_dbglevel    set new debug level
 *                 tst_get_dbglevel    get current debug level
 */


#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <sys/types.h>
#include <dcd_st_utils.h>

#define DBGLEVEL    "DBG_LEVEL"
#define TSTDBGLEVEL    "TEST_DBG_LEVEL"

const static int    tst_deflev = DBG_NOTE;  /* default debug level */
static int          tst_loglev = DBG_UNSET;

/*
 * ROUTINE NAME:   tst_init_dbg
 *
 * DESCRIPTION:
 *
 *     initialize debug facility, setting debug level
 *
 * INPUT:
 *     level   debug level to be set if none is specified in the
 *             environment
 */

void
tst_init_dbg (int level)
{
    char    *p;

    p = getenv(TSTDBGLEVEL);
    if (p == NULL)
      p = getenv(DBGLEVEL);
    if (p != NULL)
      tst_set_dbglevel (atoi(p));
    else
      tst_set_dbglevel (level);
}



/*
 * ROUTINE NAME:   tst_log_msg
 *
 * SCOPE:          Private, declared in sds_utils.h
 *
 * DESCRIPTION:
 *  Log a message to the journal file.
 *  The message is suppressed if it's loglevel is too high.
 *
 * INPUT:
 *     level     debug level
 *
 *     fmt       format string in printf() syntax
 *
 *     ...       variable number of arguments. Must match number
 *               of placeholders in the format string
 */
void
tst_log_msg(int level, char *fmt, ... )
{
    const char *level_names[] = {"NONE", "FATAL","WARN ","NOTE ",
                                "LEV1 ","LEV2 ","LEV3 ",
                                "LEV4 ","LEV5 ","LEV6 ",
                                "LEV7 ","LEV8 "};
    struct tm *ltp;
    time_t mytime;
    char buf[1024];

    va_list args;

    if (tst_loglev == DBG_UNSET)
      tst_init_dbg(tst_deflev);

    if(level>tst_loglev)
        return;

    time(&mytime);
    ltp = localtime(&mytime);

    sprintf(buf, "%s %02d:%02d:%02d: ",
        level_names[level], ltp->tm_hour, ltp->tm_min, ltp->tm_sec);
    va_start(args, fmt);
    vsprintf(buf+strlen(buf), fmt, args);
    va_end(args);
    tet_infoline(buf);
}


/*
 * ROUTINE NAME:   tst_set_dbglevel
 *
 * SCOPE:          Private, declared in sds_utils.h
 *
 * DESCRIPTION:
 *  Set debug level for application processes.
 *  A higher level forces more output.
 *
 * INPUT:
 *     level     new debug level
 */
void
tst_set_dbglevel(int level)
{
    /*  assert (level >= 0 && level <= MAX_DBG_LEV); */
    if (level < DBG_NONE )
    {
	level=DBG_NONE;
    }
    if(level > MAX_DBG_LEV)
    {
	level=MAX_DBG_LEV;
    }
    tst_loglev=level;
}


/*
 * ROUTINE NAME:   tst_get_dbglevel
 *
 * SCOPE:          Private, declared in sds_utils.h
 *
 * DESCRIPTION:
 * return current loglevel
 *
 * OUTPUT:
 *     level     current debug level
 */
void
tst_get_dbglevel(int *level)
{
    *level = tst_loglev;
}




