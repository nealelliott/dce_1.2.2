/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: curses_test.c,v $
 * Revision 1.1.9.1  1996/10/02  17:50:43  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:39:51  damon]
 *
 * Revision 1.1.4.1  1994/06/09  14:09:44  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:24:23  annie]
 * 
 * Revision 1.1.2.3  1993/01/21  19:38:40  zeliff
 * 	Embedding copyright notices
 * 	[1993/01/19  19:52:44  zeliff]
 * 
 * Revision 1.1.2.2  1992/08/31  19:58:09  jaffe
 * 	Transarc delta: jaffe-ot4658-fix-compiler-warnings-in-102-1.42 1.1
 * 	  Selected comments:
 * 	    cleaned up compiler warnings.
 * 	    comment text after endif
 * 	[1992/08/30  02:48:44  jaffe]
 * 
 * Revision 1.1  1992/01/19  02:41:12  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * (C) Copyright Transarc Corporation 1989
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 */

/*
 * Test of the curses package, to make sure I really understand how to use it.
 *
 * Ed. Zayas
 * Transarc Corporation
 */

#include <curses.h>

main(argc, argv)
    int argc;
    char **argv;

{ /*main*/

    WINDOW *mainscr;
    char str[128];
    int i;

    mainscr = initscr();
    scrollok(stdscr, TRUE);
    clear();
    addstr("This is my first curses string ever!\n");
    refresh();
    box(stdscr, '|', '-');
    standout();
    addstr("This is a standout string\n");
    refresh();
    standend();

#if 0
    box
    addstr("Enter a string and a number: ");
    refresh();
    scanw(stdscr, "%s %d", str, &i);
    wprintw(stdscr, "String was '%s', number was %d\n", str, i);
    refresh();
#endif /* 0 */

    endwin();

} /*main*/
