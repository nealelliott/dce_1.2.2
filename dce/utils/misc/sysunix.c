/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sysunix.c,v $
 * Revision 1.1.4.2  1996/02/18  19:22:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:07:00  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:37:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:09:18  root]
 * 
 * Revision 1.1.2.4  1994/09/01  14:16:08  coren
 * 	Properly disable ^Z in rl_ttyset (CR 11255).
 * 	[1994/08/31  21:43:01  coren]
 * 
 * Revision 1.1.2.3  1994/07/27  17:12:55  rsalz
 * 	Use dce_strdup (OT CR 11341) and do code-cleanup.
 * 	[1994/07/27  17:03:54  rsalz]
 * 
 * Revision 1.1.2.2  1994/07/12  15:40:20  bowe
 * 	Set rl_susp. [CR 11235]
 * 	[1994/07/12  15:39:50  bowe]
 * 
 * Revision 1.1.2.1  1994/01/25  22:53:27  bowe
 * 	Initial checkin - moved here from ./dce/utils
 * 	[1994/01/25  22:38:57  bowe]
 * 
 * Revision 1.1.2.3  1993/12/10  00:04:12  melman
 * 	Made editline.h a private editlinep.h and added a public editline.h
 * 	[1993/12/10  00:03:55  melman]
 * 
 * Revision 1.1.2.2  1993/07/14  19:09:23  melman
 * 	Initial version
 * 	[1993/07/14  18:49:42  melman]
 * 
 * $EndLog$
 */
/*
**  Unix system-dependant routines for editline library.
*/
#include <editlinep.h>
#include <termios.h>

void
rl_ttyset(
    int				Reset
)
{
    static struct termios	old;
    struct termios		new;

    if (Reset == 0) {
	(void)tcgetattr(0, &old);
	rl_erase = old.c_cc[VERASE];
	rl_kill = old.c_cc[VKILL];
	rl_eof = old.c_cc[VEOF];
	rl_intr = old.c_cc[VINTR];
	rl_quit = old.c_cc[VQUIT];
#if  defined(SIGTSTP)
	rl_susp = old.c_cc[VSUSP];
#endif	/* defined(SIGTSTP) */

	new = old;
	new.c_cc[VINTR] = -1;
	new.c_cc[VQUIT] = -1;
#if  defined(SIGTSTP)
	new.c_cc[VSUSP] = -1;
#endif	/* defined(SIGTSTP) */
	new.c_lflag &= ~(ECHO | ICANON);
	new.c_iflag &= ~(ISTRIP | INPCK);
	new.c_cc[VMIN] = 1;
	new.c_cc[VTIME] = 0;
	(void)tcsetattr(0, TCSANOW, &new);
    }
    else
	(void)tcsetattr(0, TCSANOW, &old);
}

void
rl_add_slash(
    char	*path,
    char	*p
)
{
    struct stat	Sb;

    if (stat(path, &Sb) >= 0)
	(void)strcat(p, S_ISDIR(Sb.st_mode) ? "/" : " ");
}
