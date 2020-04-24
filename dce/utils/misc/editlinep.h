/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: editlinep.h,v $
 * Revision 1.1.4.2  1996/02/18  23:33:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:03  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:37:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:09:09  root]
 * 
 * Revision 1.1.2.3  1994/07/27  17:12:52  rsalz
 * 	Use dce_strdup (OT CR 11341) and do code-cleanup.
 * 	[1994/07/27  17:03:23  rsalz]
 * 
 * Revision 1.1.2.2  1994/07/12  15:40:18  bowe
 * 	Added #include <signal.h>, defined rl_susp.  [CR 11235]
 * 	[1994/07/12  15:38:45  bowe]
 * 
 * Revision 1.1.2.1  1994/01/25  22:53:22  bowe
 * 	Initial checkin - moved here from ./dce/utils
 * 	[1994/01/25  22:38:54  bowe]
 * 
 * Revision 1.1.2.1  1993/12/10  00:04:11  melman
 * 	Made editline.h a private editlinep.h and added a public editline.h
 * 	[1993/12/10  00:03:54  melman]
 * 
 * Revision 1.1.2.2  1993/07/14  19:09:08  melman
 * 	Initial version
 * 	[1993/07/14  18:49:32  melman]
 * 
 * $EndLog$
 */
/*
**  Internal header file for editline library.
*/
#include <dce/dce.h>
#include <dce/dce_utils.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <dirent.h>
typedef struct dirent	DIRENTRY;

#if	!defined(S_ISDIR)
#define S_ISDIR(m)		(((m) & S_IFMT) == S_IFDIR)
#endif	/* !defined(S_ISDIR) */

#if	!defined(SIZE_T)
#define SIZE_T	unsigned int
#endif	/* !defined(SIZE_T) */

typedef unsigned char	CHAR;

#define CRLF		"\r\n"

#define MEM_INC		64
#define SCREEN_INC	256

#define DISPOSE(p)	free((char *)(p))
#define NEW(T, c)	\
	((T *)malloc((unsigned int)(sizeof (T) * (c))))
#define RENEW(p, T, c)	\
	(p = (T *)realloc((char *)(p), (unsigned int)(sizeof (T) * (c))))
#define COPYFROMTO(new, p, len)	\
	(void)memcpy((char *)(new), (char *)(p), (int)(len))


/*
**  Variables and routines internal to this package.
*/
extern int	rl_eof;
extern int	rl_erase;
extern int	rl_intr;
extern int	rl_kill;
extern int	rl_quit;
#if  defined(SIGTSTP)
extern int	rl_susp;
#endif	/* defined(SIGTSTP) */
extern char	*rl_complete(char*,int*);
extern int	rl_list_possib(char*,char***);
extern void	rl_ttyset(int);
extern void	rl_add_slash(char*,char*);
