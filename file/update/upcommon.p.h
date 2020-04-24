/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: upcommon.p.h,v $
 * Revision 1.1.9.1  1996/10/02  21:05:55  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:50:04  damon]
 *
 * Revision 1.1.4.1  1994/06/09  14:23:44  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:35:40  annie]
 * 
 * Revision 1.1.2.3  1993/01/21  16:27:58  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  18:20:59  cjd]
 * 
 * Revision 1.1.2.2  1992/08/31  21:41:26  jaffe
 * 	Transarc delta: jess-ot3775-remove-DCELOCAL-dups 1.1
 * 	  Selected comments:
 * 	    Now that both reference ports use DCEPATHS to provide definitions of
 * 	    DCELOCAL_PATH and DCESHARED_PATH, the user space server can be cleaned
 * 	    upto not have local definitions of these parameters.
 * 	    see above.
 * 	[1992/08/30  13:29:22  jaffe]
 * 
 * Revision 1.1  1992/01/19  02:56:29  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/* $Header: 
 * $Source:
 */

#ifndef UPCOMMON_H_
#define UPCOMMON_H_ 1

/* upitem - server's list of exportable items
 */

typedef struct upitemS	*upitemP;
typedef struct upitemS
{
    upitemP next;		/* ptr to next item */
    long type;			/* object type i.e. dir or file */
    long level;			/* encryption level */
    char *name;			/* pathname of object */
} upitemT;
typedef	updateFileStatT	*updateFileStatP;

typedef struct upLogS
{
    char *logfile;
    FILE *stream;
} upLogT;
typedef upLogT	*upLogP;

/* object types. Used in
 *	filestr structures	(client's list of objects to import)
 *	upitem structures	(server's list of exportable items)
 */

#define FSTR_DIRECTORY  1
#define FSTR_FILE       2

/* default location information for log file */


#define	LOGPATH		"/var/dfs/adm/"		/* suffix for DCELOCAL */
#define	LOGFILE		"UpLog"			/* server log file */

#define IS_COMM_ERR(s)  (s == rpc_s_ok ? 0 : 1)

#endif /* UPCOMMON_H_ */
