/*-
 * @TRANSARC_COPYRIGHT@
 *
 * $TALog: cmd.p.h,v $
 * Revision 4.7  1995/04/25  18:52:32  bab
 * [merge of changes from 4.4 to 4.5 into 4.6]
 *
 * [*** log entries omitted ***]
 *
 * $EndTALog$
 */

/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cmd.p.h,v $
 * Revision 1.1.12.1  1996/10/02  18:47:31  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:48:54  damon]
 *
 * Revision 1.1.7.1  1994/06/09  14:22:50  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:34:24  annie]
 * 
 * Revision 1.1.5.2  1993/01/21  15:54:51  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  17:38:11  cjd]
 * 
 * Revision 1.1.3.2  1992/04/21  16:50:14  mason
 * 	Yet another good log message mysteriously stripped out.  Miscellaneous changes.
 * 	[1992/04/20  23:19:21  mason]
 * 
 * Revision 1.1  1992/01/19  02:50:49  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/* $Header: /u0/rcs_trees/dce/rcs/file/tools/cmd/cmd.p.h,v 1.1.12.1 1996/10/02 18:47:31 damon Exp $ */
/* $Source: /u0/rcs_trees/dce/rcs/file/tools/cmd/cmd.p.h,v $ */

/*
 * P_R_P_Q_# (C) COPYRIGHT IBM CORPORATION 1988, 1989
 * LICENSED MATERIALS - PROPERTY OF IBM
 * REFER TO COPYRIGHT INSTRUCTIONS FORM NUMBER G120-2083
 */

#ifndef __CMD_INCL__
#define	__CMD_INCL__	    1

/* parmdesc types */
#define	CMD_FLAG	1	/* no parms */
#define	CMD_SINGLE	2	/* one parm */
#define	CMD_LIST	3	/* two parms */

/* syndesc flags */
#define	CMD_ALIAS	1	/* this is an alias */

#define CMD_HELPPARM	31	/* last one is used by -help switch */
#define	CMD_MAXPARMS	32	/* max number of parm types to a cmd line */

#include <dcedfs/cmd_help.h>

/* parse items are here */
struct cmd_item {
    struct cmd_item *next;
    char *data;
};

typedef struct cmd_parmHelpLink {
  char *			helpString;
  unsigned long			flags;
  struct cmd_parmHelpLink *	next;
} cmd_parmHelpLink_t;
     
struct cmd_parmdesc {
    char *name;			/* switch name */
    int	type;			/* flag, single or list */
    struct cmd_item *items;	/* list of cmd items */
    long flags;			/* flags */

    unsigned long 		alternatives;	/* bit set of indices of mutually
						   exclusive alternatives */

    int				helpCount;	/* number of help strings */
    cmd_parmHelpLink_t *	firstHelp;	/* link for the first help string */
    cmd_parmHelpLink_t *	lastHelp;	/* link for the last help string */

    int				(*checkop)();	/* parameter validation flag */
    int				wasSwitchTyped;	/* used to fix anomaly in which a user
						 * can type the switch name of an optional
						 * non-flag parameter at the end of a
						 * command line, and not get an
						 * error message
						 */
    cmd_help_entry_t *		helpEntryP;	/* help info for interactive mode */
};

/* cmd_parmdesc flags */
/*
 * These flags must be disjoint from the cmd_parmHelpLink flags, since they all need
 * to be able to get through the cmd_AddParm interface.
 */
#define	CMD_REQUIRED	    0
#define	CMD_OPTIONAL	    1
#define	CMD_EXPANDS	    2	/* if list, try to eat tokens through eoline,
				   instead of just 1 */

/* cmd_parmHelpLink flags */
/*
 * These flags must be disjoint from the cmd_parmdesc flags, since they all need
 * to be able to get through the cmd_AddParm interface.
 */
#define CMD_PARM_LITERAL	(0x4)
#define CMD_PARM_NUMERIC	(0x8)
#define CMD_PARM_SIGNED		(0x10)
#define CMD_PARM_REAL		(0x20)
#define CMD_PARM_COMMAS		(0x40)
#define CMD_PARM_ALLNUMERICS	\
  (CMD_PARM_NUMERIC | CMD_PARM_SIGNED | CMD_PARM_REAL | CMD_PARM_COMMAS)

#define CMD_PARM_VALIDATIONFLAGS	(CMD_PARM_LITERAL | CMD_PARM_ALLNUMERICS)

struct cmd_syndesc {
    struct cmd_syndesc *next;	/* next one in system list */
    struct cmd_syndesc *nextAlias;  /* next in alias chain */
    struct cmd_syndesc *aliasOf;    /* back ptr for aliases */
    char *name;		    /* subcommand name */
    char *a0name;	    /* command name from argv[0] */
    char *help;		    /* help description */
    int (*proc)();
    char *rock;
    int	nParms;		    /* number of parms */
    long flags;		    /* random flags */
    int		allowInteractive;	/* whether or not to allow interactive mode */
    struct cmd_parmdesc parms[CMD_MAXPARMS];	/* parms themselves */
};

#if 0
#define	CMD_TOOMANY	    1900
#define	CMD_SYNTAX	    1901
#define	CMD_AMBIG	    1902
#define	CMD_TOOFEW	    1903
#endif

/* interfaces for routines exported by the command package */

/* first, a few definitions we may need to make the declarations work */
#if !defined(_TAKES)
#define __CMD_INCL_SET_TAKES__	1

#if defined(__STDC__) || defined(mips) || defined(_IBMR2)
#define _TAKES(x) x
#else /* __STDC__ */
#define _TAKES(x) ()
#endif /* __STDC__ */

#endif	/* _TAKES */

extern int cmd_SetBeforeProc _TAKES((
				     int (*aproc)(),
				     char * arock
				   ));
extern int cmd_SetAfterProc _TAKES((
				     int (*aproc)(),
				     char * arock
				   ));
extern struct cmd_syndesc *cmd_CreateSyntax _TAKES((
						    char *aname,
						    int (*aproc)(),
						    char *arock,
						    char *ahelp
						  ));
extern int cmd_CreateAlias _TAKES((
				   struct cmd_syndesc *as,
				   char *aname
				 ));
extern int cmd_AddParm _TAKES((
			       struct cmd_syndesc *as,
			       char *aname,
			       int atype,
			       long aflags,
			       char *ahelp
			     ));
extern int cmd_ParseLine _TAKES((
				 char *aline,
				 char **argv,
				 long *an,
				 long amaxn
			       ));
extern long cmd_Dispatch _TAKES((
				 int		argc,
				 char **	argv
			       ));
extern int cmd_FreeArgv _TAKES((
				char **argv
			      ));
extern int cmd_Seek _TAKES((
			    struct cmd_syndesc *as,
			    int apos
			  ));
extern int PrintSyntax _TAKES((
			       struct cmd_syndesc *as
			     ));
extern long cmd_MakeAlternate _TAKES((
				      struct cmd_syndesc *	syntaxDesc,
				      char *			columnA,
				      char *			columnB
				    ));
extern long cmd_AddParamHelp _TAKES((
				     struct cmd_syndesc *	syntaxDesc,
				     char *			paramName,
				     char *			newHelp,
				     unsigned long		flags
				   ));


/* try not to interfere with other declarations of the macro */
#if defined(__CMD_INCL_SET_TAKES__)
#undef _TAKES
#endif

#endif /* __CMD_INCL__ */
