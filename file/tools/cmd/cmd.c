/*-
 * @TRANSARC_COPYRIGHT@
 *
 * $TALog: cmd.c,v $
 * Revision 4.42  1996/04/19  13:23:29  gait
 * Continue with other components.
 *
 * Replace printf() with dce_svc_printf() where appropriate.
 * [from r4.41 by delta gait-db7158-SAMS-dfsbind, r1.5]
 *
 * Revision 4.41  1995/04/25  19:55:23  bab
 * Fixed merge error that occurred in moving up to dfs-perf 1.12 and re-fixed
 * the problem described in version 1.11 of this delta...The fixed code
 * had apparently gotten lost...
 * [from r4.40 by delta bab-tools-cmd-add-interactive, r1.12]
 *
 * Revision 4.40  1995/04/25  18:52:20  bab
 * [merge of changes from 4.40 to 4.41 into 4.39]
 *
 * [*** log entries omitted ***]
 *
 * Revision 4.39  1995/04/25  18:51:55  bab
 * [merge of changes from 4.22 to 4.37 into 4.38]
 *
 * [*** log entries omitted ***]
 *
 * $EndTALog$
 */

#if !defined(_NOIDENT) && !defined(lint)
static char *TARCSId = "$Id: cmd.c,v 4.42 1996/04/19 13:23:29 gait Exp $";
#endif

/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cmd.c,v $
 * Revision 1.1.541.5  1994/08/30  15:56:16  sasala
 * 	DFS S12Y general cleanup - rename ftutil and tools cmd catalogs, add ftutil msgs to xaggr dfsexport, and episode salvage and vnops subcomponents
 * 	[1994/08/30  00:19:57  sasala]
 *
 * Revision 1.1.541.4  1994/08/03  16:26:21  ruby
 * 	changed revision history in an attemp to get bmerge working
 * 	[1994/08/03  15:18:00  ruby]
 * 
 * 	Implement partial S12Y (Internationalization) for DFS commands and daemons
 * 	[1994/08/02  20:24:21  ruby]
 * 
 * Revision 1.1.543.2  1994/06/29  11:54:07  devsrc
 * 	merged with bl-10
 * 
 * Revision 1.1.543.1  1994/04/28  17:42:40  mbs
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  16:03:15  mbs]
 * 
 * Revision 1.1.541.2  1994/06/09  14:22:49  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:34:22  annie]
 * 
 * Revision 1.1.541.1  1994/02/04  20:33:01  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:19:10  devsrc]
 * 
 * Revision 1.1.539.1  1993/12/07  17:35:52  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  17:18:18  jaffe]
 * 
 * Revision 1.1.4.5  1993/01/21  15:54:43  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  17:38:04  cjd]
 * 
 * Revision 1.1.4.4  1993/01/13  19:27:07  shl
 * 	Transarc delta: kazar-ot5689-cm-cmd-returns-bogus-status 1.1
 * 	  Selected comments:
 * 	    drop Jean P.'s fixes to cm command.
 * 	    get cmd parser to return proper error code on bogus help requests
 * 	[1993/01/12  22:07:41  shl]
 * 
 * Revision 1.1.4.3  1992/09/25  19:43:11  jaffe
 * 	Sync with Transarc
 * 	[1992/09/23  20:51:58  jaffe]
 * 	Transarc delta: jaffe-ot5416-cleanup-RCS-log-entries 1.1
 * 	  Selected comments:
 * 	    Cleanup extra RCS log entries.  There should only be one per file, and
 * 	    it should be closed with an EndLog comment.
 * 	[1992/09/23  19:55:24  jaffe]
 * 
 * Revision 1.1.4.2  1992/09/15  13:19:37  jaffe
 * 	Transarc delta: mcinerny-ot4940-remove-rt-pragmas 1.1
 * 	  Selected comments:
 * 	    Since we don't build for RTs, we no longer need the #ifdef __HIGHC__ or
 * 	    their enclosed pragmas.
 * 	[1992/09/14  20:40:23  jaffe]
 * 
 * Revision 1.1.2.3  1992/04/21  16:46:53  mason
 * 	Misc. changes.  Funny how bcs sometimes doesn't pick up the comments
 * 	from the file, isn't it?  Now I cannot remember what they were.
 * 	[1992/04/20  23:15:26  mason]
 * 
 * Revision 1.1.2.2  1992/04/14  04:11:01  mason
 * 	Transarc delta: bab-ot2326-cmd-support-no-initcmd 1.1
 * 	  Files modified:
 * 	    tools/cmd: cmd.c
 * 	  Selected comments:
 * 	    The changes to support those clients of the command parsing package that
 * 	    have no subcommands and initialize it using NULL, rather than "initcmd"
 * 	    didn't get into the last drop.  Those were made as this delta.
 * 	    Support clients that have no sub-commands and use NULL to set up the
 * 	    command parsing
 * 	    ot 2326
 * 	[1992/04/13  18:02:44  mason]
 * 
 * Revision 1.1  1992/01/19  02:50:53  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/* Copyright (C) 1989 Transarc Corporation - All rights reserved */

/*
 * P_R_P_Q_# (C) COPYRIGHT IBM CORPORATION 1988, 1989
 * LICENSED MATERIALS - PROPERTY OF IBM
 * REFER TO COPYRIGHT INSTRUCTIONS FORM NUMBER G120-2083
 */

#ifndef lint
static char rcsid[] = "$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/tools/cmd/RCS/cmd.c,v 4.42 1996/04/19 13:23:29 gait Exp $";
#endif

#include <sys/types.h>
#include <ctype.h>
#if !defined(SUNOS4)
#include <dcedfs/osi.h>
#endif
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <cmd.h>
#include <cmd_help.h>
#include <cmd_private.h>
#include <pthread.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfscmcmac.h>
#include <dfscmcsvc.h>
#include <dfscmcmsg.h>

/* 
 *    Note that '-' cannot be used for the following character without
 *  changes to the logic of cmd_Dispatch that checks the initial character
 *  of the command line arguments before searching for a match in the
 *  syntax structures parameter array.
 */
#define CMD_SINGLE_CHAR_QUOTE	'\\'

static int ReadToNewline _TAKES((FILE *	stream));

/* declaration of private token type */
struct cmd_token {
    struct cmd_token *next;
    char *key;
};

static int dummy;	/* non-null ptr used for flag existence */
static struct cmd_syndesc *allSyntax=0;
static int noOpcodes = 0;
static int (*beforeProc)() = 0, (*afterProc)() = 0;
static char *beforeRock, *afterRock;
static char initcmd_opcode[] = "initcmd";    /*Name of initcmd opcode*/

/* define a servicability handle for the dce_svc_printf facility */

DCE_SVC_DEFINE_HANDLE(cmc_svc_handle, cmc_svc_table, "cmc")

/* take name and string, and return null string if name is empty, otherwise return
   the concatenation of the two strings */
static char *NName(a1, a2)
char *a1, *a2; {
    static char tbuffer[80];
    if (strlen(a1) == 0) {
	return "";
    }
    else {
	strcpy(tbuffer, a1);
	strcat(tbuffer, a2);
	return tbuffer;
    }
}

/* return true if asub is a substring of amain */
static int SubString(amain, asub)
register char *amain, *asub; {
    int mlen, slen;
    register int i,j;
    mlen = strlen(amain);
    slen = strlen(asub);
    j = mlen - slen;
    if (j < 0) return 0;	/* not a substring */
    for(i=0;i<=j;i++) {
	if (strncmp(amain, asub, slen) == 0) return 1;
	amain++;
    }
    return 0;	/* didn't find it */
}

/*
 * Return the index of the parameter with the given name within the
 * given syntax descriptor.  Return -1 if no such parameter is found.
 */
EXPORT int cmd_FindParam(syntaxDescr, paramName)
     struct cmd_syndesc *	syntaxDescr;
     char *			paramName;
{
  int	i = 0;	/* for iteration through the array */
  int	rtnVal = -1;
  
  while ((i < CMD_MAXPARMS) && (rtnVal == -1)) {
    if ((syntaxDescr->parms[i].name != (char *)0) &&
	(strcmp(paramName, syntaxDescr->parms[i].name) == 0)) {
      rtnVal = i;
    }
    i++;
  }
  
  return rtnVal;
}

/* adapted from FindType, below */
EXPORT int cmd_FindAllParams(syntaxDescrP, paramNameP, altMatchesP)
     struct cmd_syndesc *	syntaxDescrP;
     char *			paramNameP;
     unsigned long *		altMatchesP;
{
  int		firstMatch = -1;
  int		i;
  int		exactFound = 0;
  unsigned int	nameLen;
  
  *altMatchesP = (unsigned long)0x0;
  nameLen = strlen(paramNameP);
  for (i = 0; (i < CMD_MAXPARMS) && (exactFound == 0); i++) {
    if ((syntaxDescrP->parms[i].name != 0) &&	/* param slot is used */
	(strlen(syntaxDescrP->parms[i].name) >= nameLen)) {/* match not possible */
      if (strcmp(syntaxDescrP->parms[i].name, paramNameP) == 0) {
	/* exact match */
	firstMatch = i;
	/* remove all potential prefix matches: */
	*altMatchesP = (unsigned long)0x0;

	exactFound = 1;	/* we have what we need, get out of the loop */
      }
      else if (strncmp(syntaxDescrP->parms[i].name, paramNameP, nameLen) == 0) {
	if (firstMatch == -1) {
	  /* this is the first potential match found */
	  firstMatch = i;
	}
	else {
	  /* only flag extras if this is not the first */
	  *altMatchesP |= CMD_BIT(i);
	}
      }
    }
  }
  
  return firstMatch;
}

static int FindType(as, aname, ambig_flagP)
register struct cmd_syndesc *as;
register char *aname; 
int *		ambig_flagP;
{
    register int i;
    int cmdlen;
    int ambig;
    int best;

    cmdlen = strlen(aname);
    ambig = 0;
    best = -1;
    for(i=0;i<CMD_MAXPARMS;i++) {
	if (as->parms[i].type == 0) continue;   /* this slot not set (seeked over) */
	if (strcmp(as->parms[i].name, aname)==0) return i;
	if (strlen(as->parms[i].name) < (unsigned)cmdlen) continue;
	if (strncmp(as->parms[i].name, aname, cmdlen) == 0) {
	    if (best != -1) ambig = 1;
	    else best = i;
	}
    }

    if (ambig_flagP) {
      *ambig_flagP = ambig;
    }
     
    return (ambig? -1 : best);
}

EXPORT struct cmd_syndesc * cmd_FindSyntax(aname, aambig)
int *aambig;
char *aname; {
    register struct cmd_syndesc *ts;
    struct cmd_syndesc *best;
    int cmdLen;
    int ambig;

    cmdLen = strlen(aname);
    best = (struct cmd_syndesc *) 0;
    ambig = 0;
    if (aambig) *aambig = 0;	/* initialize to unambiguous */
    for(ts=allSyntax;ts;ts=ts->next) {
	if (strcmp(aname, ts->name)==0) return(ts);
	if (strlen(ts->name) < (unsigned)cmdLen) continue;    /* we typed more than item has */
	if (strncmp(ts->name, aname, cmdLen) == 0) {
	    if (best) ambig = 1;	/* ambiguous name */
	    else best = ts;
	}
    }
    if (ambig) {
	if(aambig) *aambig = ambig;	/* if ambiguous and they care, tell them */
	return (struct cmd_syndesc *) 0;	/* fails */
    }
    else return best;	/* otherwise its not ambiguous, and they know */
}

#ifdef biggernotdef
/* print the help for a single parameter */
static PrintParmHelp(aparm)
register struct cmd_parmdesc *aparm; {
    if (aparm->type == CMD_FLAG) {
#ifdef notdef
	/* doc people don't like seeing this information */
	if (aparm->help)
	    printf("(%s)", aparm->help);
#endif
    }
    else if (aparm->help) {
	printf("<%s>", aparm->help);
	if (aparm->type == CMD_LIST) printf("+");
    }
    else if (aparm->type == CMD_SINGLE)
	printf("<arg>");
    else if (aparm->type == CMD_LIST)
	printf("<arg>+");
}
#endif /* biggernotdef */

/*
 * Test whether or not anything should be printed for given parameter (with the given
 * index).  Only the first parameter within a group of mutually exclusive alternatives
 * has anything printed for it.  Returns 0 if no printing should be done.
 */
static int cmd_TestParamPrint(theParam, paramIndex)
     struct cmd_parmdesc *	theParam;
     int			paramIndex;
{
  int		rtnVal = (theParam->type != 0);
  unsigned long	alternativesSet = theParam->alternatives;
  int		i;
  
  if (rtnVal && alternativesSet) {
    for (i = 0; i < paramIndex; i++) {
      if (CMD_IS_BIT_SET(i, alternativesSet)) {
	rtnVal = 0;
	break;
      }
    }
  }
  
  return rtnVal;
}

/*
 * Print a single help string (from the given help link) for the given parameter.
 */
void cmd_PrintSingleParamHelp(theParam, theHelp)
     struct cmd_parmdesc *	theParam;
     cmd_parmHelpLink_t *	theHelp;
{
    if (theParam->type == CMD_FLAG) {
#ifdef notdef
      /* doc people don't like seeing this information */
      if (theHelp != (cmd_parmHelpLink_t *)0)
	printf("(%s)", theHelp->helpString);
#endif
    }
    else if (theHelp != (cmd_parmHelpLink_t *)0) {
      if (theHelp->flags & CMD_PARM_LITERAL) {
	printf("%s", theHelp->helpString);
      }
      else {
	printf("<%s>", theHelp->helpString);
      }
    }
    else {
      printf("<arg>");
    }
}

/*
 * Print all the help strings for the given parameter.
 */
void cmd_PrintParamHelp(theParam)
     struct cmd_parmdesc *	theParam;
{
  cmd_parmHelpLink_t *	thisHelp;
  
  if (theParam->helpCount > 1) {
    if (theParam->firstHelp->flags & CMD_PARM_LITERAL) {
      printf(" {%s", theParam->firstHelp->helpString);
    }
    else {
      printf(" {<%s>", theParam->firstHelp->helpString);
    }
    
    for (thisHelp = theParam->firstHelp->next; thisHelp != (cmd_parmHelpLink_t *)0;
	 thisHelp = thisHelp->next) {
      printf(" | ");
      cmd_PrintSingleParamHelp(theParam, thisHelp);
    }
    
    printf("}");
  }
  else {
    if (theParam->type != CMD_FLAG) {
      printf(" ");
    }
    cmd_PrintSingleParamHelp(theParam, theParam->firstHelp);
  }
  
  if (theParam->type == CMD_LIST) printf("...");
}

/*
 * Print information about the parameter at the given index within the given
 * syntax descriptor.
 */
void cmd_PrintParam(syntaxDesc, paramIndex)
     struct cmd_syndesc *	syntaxDesc;
     int			paramIndex;
{
  int		i;
  unsigned long	alternativesSet = syntaxDesc->parms[paramIndex].alternatives;


  if (cmd_TestParamPrint(&(syntaxDesc->parms[paramIndex]), paramIndex)) {
    printf(" ");
    if ((syntaxDesc->parms[paramIndex].flags & CMD_OPTIONAL) != 0) {
      printf("[");
    }
    
    if (alternativesSet) {
      /*
       * we know that the given param is the first in the list, or cmd_TestPrint
       * would not have returned non-zero
       */
      printf("{%s", syntaxDesc->parms[paramIndex].name);
      cmd_PrintParamHelp(&(syntaxDesc->parms[paramIndex]));
      
      for (i = (paramIndex + 1); i < CMD_MAXPARMS; i++) {
	if (CMD_IS_BIT_SET(i, alternativesSet)) {
	  printf(" | %s", syntaxDesc->parms[i].name);
	  cmd_PrintParamHelp(&(syntaxDesc->parms[i]));
	}
      }

      printf("}");

    }	/* End if there is an alternative set */
    else {
      printf("%s", syntaxDesc->parms[paramIndex].name);
      cmd_PrintParamHelp(&(syntaxDesc->parms[paramIndex]));
    }

    if ((syntaxDesc->parms[paramIndex].flags & CMD_OPTIONAL) != 0) {
      printf("]");
    }
  }  
  
}


PrintSyntax(as)
register struct cmd_syndesc *as; {
    register int i;
#ifdef notdef
    register struct cmd_parmdesc *tp;
#endif /* notdef */

    /* now print usage, from syntax table */
    if (noOpcodes || (strcmp(as->name, initcmd_opcode) == 0))
	dce_printf(cmc_s_cm_usage, as->a0name);
    else
	dce_printf(cmc_s_cm_usage_name, NName(as->a0name, " "), as->name);

    for(i=0;i<CMD_MAXPARMS;i++) {
#ifdef notdef
	tp = &as->parms[i];
	if (tp->type == 0) continue;    /* seeked over slot */
	if (tp->flags & CMD_OPTIONAL) printf("[");
	printf("%s ", tp->name);
	PrintParmHelp(tp);
	if (tp->flags & CMD_OPTIONAL) printf("] ");
	else printf(" ");
#endif /* notdef */
	cmd_PrintParam(as, i);
    }

    if (as->allowInteractive) {
      fprintf(stdout, " [%c]", INTERACTIVE_KEY_CHAR);
    }
    
    printf("\n");
}

#define	CMD_COLUMNS	4

/* must print newline in any case, to terminate preceding line */
static PrintAliases(as)
register struct cmd_syndesc *as; {
    register struct cmd_syndesc *ts;

    if (as->flags & CMD_ALIAS) {
	ts = as->aliasOf;
	dce_printf(cmc_s_cm_alias, ts->name);
    }
    else {
	printf("\n");
	if (!as->nextAlias) return;	    /* none, print nothing */
	dce_printf(cmc_s_cm_aliases);
	for(as=as->nextAlias;as;as=as->nextAlias) {
	    printf("%s ", as->name);
	}
	printf("\n");
    }
}

static AproposProc(as, arock)
     char *arock;
     register struct cmd_syndesc *as; 
{
  register struct cmd_syndesc *ts;
  char *tsub;
  long rtnVal = CMD_APROPOS_NOMATCH;	/* the easiest assumption */
  
  tsub = as->parms[0].items->data;
  for(ts=allSyntax;ts;ts=ts->next) {
    if (ts->flags & CMD_ALIAS) continue;
    if (SubString(ts->help, tsub)) {
      printf("%s: %s\n", ts->name, ts->help);
      rtnVal = 0;	/* we did match something */
    }
    else if (SubString(ts->name, tsub)) {
      printf("%s: %s\n", ts->name, ts->help);
      rtnVal = 0;
    }
  }

  if (rtnVal)
    dce_printf(cmc_s_cm_no_commands);

  return rtnVal;
}

static HelpProc(as, arock)
char *arock;
register struct cmd_syndesc *as; {
    register struct cmd_syndesc *ts;
    register struct cmd_item *ti;
    int ambig;

    if (as->parms[0].items == 0) {
	dce_printf(cmc_s_cm_commands_are, NName(as->a0name, ": "));
	for(ts=allSyntax; ts; ts=ts->next) {
	    if (ts->flags & CMD_ALIAS) continue;
	    printf("%-15s %s\n", ts->name, ts->help);
	}
    }
    else {
	/* print out individual help topics */
	for(ti = as->parms[0].items; ti; ti=ti->next) {
	    ts = cmd_FindSyntax(ti->data, &ambig);
	    if (ts) {
		/* print out command name and help */
		printf("%s%s: %s ", NName(as->a0name, " "), ts->name, ts->help);
		ts->a0name = as->a0name;
		PrintAliases(ts);
		PrintSyntax(ts);
	    }
	    else {
		if (!ambig) {
		    dce_printf(cmc_s_cm_unknown_topic, NName(as->a0name, ": "), ti->data);
		    return 1;
		}
		else {
		    /* ambiguous, list 'em all */
		    dce_printf(cmc_s_cm_ambiguous_topic,
			   NName(as->a0name, ": "), 
	                   ti->data);
		    return 1;
		}
	    }
	}
    }
    return 0;
}

cmd_SetBeforeProc(aproc, arock)
int (*aproc)();
char *arock; {
    beforeProc = aproc;
    beforeRock = arock;
    return 0;
}

cmd_SetAfterProc(aproc, arock)
int (*aproc)();
char *arock; {
    afterProc = aproc;
    afterRock = arock;
    return 0;
}

/* thread on list in alphabetical order */
static SortSyntax(as)
struct cmd_syndesc *as; {
    register struct cmd_syndesc **ld, *ud;

    for(ld = &allSyntax, ud = *ld; ud; ld = &ud->next, ud = *ld) {
	if (strcmp(ud->name, as->name) > 0) { /* next guy is bigger than us */
	    break;
	}
    }
    /* thread us on the list now */
    *ld = as;
    as->next = ud;
    return 0;
}

struct cmd_syndesc *cmd_CreateSyntax(aname, aproc, arock, ahelp)
int (*aproc)();
char *ahelp;
char *arock;
char *aname; 
{
    register struct cmd_syndesc *td;
    static int initd = 0; /*Is this the first time this routine has been called? */

    /* Init dce servicability the first time through. */
    if (!initd) {
	error_status_t st;

	initd = 1;
	dce_msg_define_msg_table(cmc__table,
				 sizeof cmc__table / sizeof cmc__table[0],
				 &st);
	if (st != svc_s_ok)
	  fprintf(stderr,
		  "Warning Cannot register cmc_table as an incore message table 0x%lx\n",
		  st);
      }
    /* can't have two cmds in no opcode mode */
    if (noOpcodes) return (struct cmd_syndesc *) 0;

    td = (struct cmd_syndesc *) malloc(sizeof(struct cmd_syndesc));
    bzero((char *)td, sizeof(*td));
    
    /* copy in name, etc */
    if (aname) {
	td->name = (char *) malloc(strlen(aname)+1);
	strcpy(td->name, aname);
    }
    else {
	td->name = (char *) 0;
	noOpcodes = 1;
    }
    if (ahelp) {
	td->help = (char *) malloc(strlen(ahelp)+1);
	strcpy(td->help, ahelp);
    }
    else td->help = (char *) 0;
    td->proc = aproc;
    td->rock = arock;
    
    SortSyntax(td);

    cmd_Seek(td, CMD_HELPPARM);
    cmd_AddParm(td, "-help", CMD_FLAG, CMD_OPTIONAL, "get detailed help");
    cmd_Seek(td, 0);

    return td;
}

int cmd_CreateAlias(as, aname)
register struct cmd_syndesc *as;
char *aname; {
    register struct cmd_syndesc *td;
    
    td = (struct cmd_syndesc *) malloc(sizeof(struct cmd_syndesc));
    *td = *as;
    td->name = (char *) malloc(strlen(aname)+1);
    strcpy(td->name, aname);
    td->flags |= CMD_ALIAS;
    /* if ever free things, make copy of help string, too */
    
    /* thread on list */
    SortSyntax(td);
    
    /* thread on alias lists */
    td->nextAlias = as->nextAlias;
    as->nextAlias = td;
    td->aliasOf = as;

    return 0;	/* all done */
}

int cmd_Seek(as, apos)
register struct cmd_syndesc *as;
int apos; {
    if (apos >= CMD_MAXPARMS) return CMD_EXCESSPARMS;
    as->nParms = apos;
    return 0;
}

/*
 * add the given string to the list of help strings for the parameter with
 * the given name within the given syntax descriptor
 */
long cmd_AddParamHelp(syntaxDesc, paramName, newHelp, flags) 
     struct cmd_syndesc *	syntaxDesc;
     char *			paramName;
     char *			newHelp;
     unsigned long		flags;
{
  int			rtnVal = 0;
  int			paramIndex;
  cmd_parmHelpLink_t *	thisHelp;

  paramIndex = cmd_FindParam(syntaxDesc, paramName);
  
  if (newHelp != (char *)0) {	/* don't bother if we've been handed a null */
    if (paramIndex != -1) {
      thisHelp = (cmd_parmHelpLink_t *)malloc(sizeof(cmd_parmHelpLink_t));
      thisHelp->helpString = malloc(strlen(newHelp) + 1);
      (void)strcpy(thisHelp->helpString, newHelp);
      thisHelp->flags = flags;
      thisHelp->next = (cmd_parmHelpLink_t *)0;
      
      if (syntaxDesc->parms[paramIndex].helpCount == 0) {
	syntaxDesc->parms[paramIndex].firstHelp =
	  syntaxDesc->parms[paramIndex].lastHelp =
	  thisHelp;
      }
      else {
	syntaxDesc->parms[paramIndex].lastHelp->next = thisHelp;
	syntaxDesc->parms[paramIndex].lastHelp = thisHelp;
      }
      
      syntaxDesc->parms[paramIndex].helpCount++;
    }
    else {
      rtnVal = CMD_UNKNOWNSWITCH;
    }
  }
  
  return rtnVal;
}


int cmd_AddParm(as, aname, atype, aflags, ahelp)
register struct cmd_syndesc *as;
char *aname;
int atype;
char *ahelp;
long aflags;{
    register struct cmd_parmdesc *tp;

    if (as->nParms >= CMD_MAXPARMS) return CMD_EXCESSPARMS;
    tp = &as->parms[as->nParms++];
    
    tp->name = (char *) malloc(strlen(aname)+1);
    strcpy(tp->name, aname);
    tp->type = atype;
    tp->flags = (aflags & (~CMD_PARM_VALIDATIONFLAGS));	/* the validation flags cannot be
							   used here */
    tp->items = (struct cmd_item *) 0;
#ifdef notdef
    if (ahelp) {
	tp->help = (char *) malloc(strlen(ahelp)+1);
	strcpy(tp->help, ahelp);
    }
    else tp->help = (char *) 0;
#endif /* notdef */
    (void)cmd_AddParamHelp(as, aname, ahelp, aflags);
    return 0;
}

/*
 * make the parameters having the given names mutually exclusive alternatives within
 * the given syntax descriptor
 */
long cmd_MakeAlternate(syntaxDesc, columnA, columnB)
     struct cmd_syndesc *	syntaxDesc;
     char *			columnA;
     char *			columnB;
{
    long			rtnVal = 0;
    int			columnAIndex;
    int			columnBIndex;
    long			alternateArray = 0L;
    long			lastAlternateArray = 0L;
    int			i;
    int			optional = 0;
    struct cmd_syndesc *	otherAlias;

    /* find the parameters corresponding to the two alternatives */
    columnAIndex = cmd_FindParam(syntaxDesc, columnA);
    columnBIndex = cmd_FindParam(syntaxDesc, columnB);

    if ((columnAIndex != -1) && (columnBIndex != -1)) {
	alternateArray = (CMD_BIT(columnAIndex) | CMD_BIT(columnBIndex) |
			  syntaxDesc->parms[columnAIndex].alternatives | 
			  syntaxDesc->parms[columnBIndex].alternatives);

	/* perform transitive closure on the their sets of alternatives */
	while (alternateArray != lastAlternateArray) {
	    lastAlternateArray = alternateArray;

	    for (i = 0; i < CMD_MAXPARMS; i++) {
		if (CMD_IS_BIT_SET(i, alternateArray)) {
		    alternateArray |= syntaxDesc->parms[i].alternatives;
		    if (syntaxDesc->parms[i].flags & CMD_OPTIONAL) {
			/* record the fact that at least one of the alts is optional */
			optional = 1;
		    }
		}
	    }
	}	/* end while */

	for (i = 0; i < CMD_MAXPARMS; i++) {
	    if (CMD_IS_BIT_SET(i, alternateArray)) {
		syntaxDesc->parms[i].alternatives = alternateArray;
		/*
		 * make sure that, if any of the alts thinks it's optional, they all
		 * think they are optional
		 */
		if (optional) {
		    syntaxDesc->parms[i].flags |= CMD_OPTIONAL;
		}
	    }
	}

	/* now, propagate the info to any aliases */
	for (otherAlias = syntaxDesc->nextAlias;
	     otherAlias != (struct cmd_syndesc *)0;
	     otherAlias = otherAlias->nextAlias) {
	    /* this is a bit of overkill, but the code is clearer */
	    for (i = 0; i < CMD_MAXPARMS; i++) {
		if (syntaxDesc->parms[i].type != 0) {	/* if parameter slot is used */
		    otherAlias->parms[i].alternatives =
		      syntaxDesc->parms[i].alternatives;
		}
	    }
	}

	/* now, go backwards through the other aliases */
	for (otherAlias = syntaxDesc->aliasOf;
	     otherAlias != (struct cmd_syndesc *)0;
	     otherAlias = otherAlias->aliasOf) {
	    /* this is a bit of overkill, but the code is clearer */
	    for (i = 0; i < CMD_MAXPARMS; i++) {
		if (syntaxDesc->parms[i].type != 0) {	/* if parameter slot is used */
		    otherAlias->parms[i].alternatives =
		      syntaxDesc->parms[i].alternatives;
		}
	    }
	}
    }
    else {
	rtnVal = CMD_UNKNOWNSWITCH;
    }

    return rtnVal;
}

long cmd_AddValidationRoutine(syntaxDescP, paramName, validationRoutineP)
     struct cmd_syndesc *	syntaxDescP;
     char *			paramName;
     int			(*validationRoutineP)();
{
  long		rtnVal = 0;
  int		paramIndex;
  
  paramIndex = cmd_FindParam(syntaxDescP, paramName);
  
  if (paramIndex != -1) {
    syntaxDescP->parms[paramIndex].checkop = validationRoutineP;
  }
  else {
    rtnVal = CMD_UNKNOWNSWITCH;
  }

  return rtnVal;
}

       
/*
 * Determine whether or not there are any parameters used in the given syntax
 * descriptor that conflict with the parameter at the given index.  Returns
 * the index of a conflicting parameter usage, or -1 if there is no
 * conflicting usage.
 */
static int cmd_CheckConflictingUsage(syntaxDesc, paramIndex)
     struct cmd_syndesc *	syntaxDesc;
     int			paramIndex;
{	
  int		rtnVal = -1;
  int		i;
  unsigned long	alternativesSet = syntaxDesc->parms[paramIndex].alternatives;
  
  if ((syntaxDesc->parms[paramIndex].items != (struct cmd_item *)0) &&
      alternativesSet) {
    for (i = 0; i < CMD_MAXPARMS; i++) {
      if ((CMD_IS_BIT_SET(i, alternativesSet)) && (i != paramIndex) &&
	  (syntaxDesc->parms[i].items != (struct cmd_item *)0)) {
	rtnVal = i;
	break;
      }
    }
  }

  return rtnVal;
}

static int cmd_CheckAlternateUsage(syntaxDesc, paramIndex)
     struct cmd_syndesc *	syntaxDesc;
     int			paramIndex;
{
  int		rtnVal = (syntaxDesc->parms[paramIndex].items != (struct cmd_item *)0);
  int		i;
  unsigned long alternativesSet = syntaxDesc->parms[paramIndex].alternatives;

  if ((!rtnVal) && (alternativesSet)) {
    for (i = 0; (i < CMD_MAXPARMS) && (!rtnVal); i++) {
      if ((CMD_IS_BIT_SET(i, alternativesSet)) &&
	  (syntaxDesc->parms[i].items != (struct cmd_item *)0)) {
	rtnVal = 1;
      }
    }
  }
  return rtnVal;
}

/* add a text item to the end of the parameter list */
static AddItem(aparm, aval)
register struct cmd_parmdesc *aparm;
register char *aval; {
    register struct cmd_item *ti, *ni;
    ti = (struct cmd_item *) malloc(sizeof(struct cmd_item));
    bzero((char *)ti, sizeof(*ti));

    if (aval[0] == CMD_SINGLE_CHAR_QUOTE) {	/* skip quote character */
      aval++;
    }

    ti->data = (char *) malloc(strlen(aval)+1);
    strcpy(ti->data, aval);
    /* now put ti at the *end* of the list */
    if (ni=aparm->items) {
	for (; ni; ni=ni->next) if (ni->next == 0) break;	/* skip to last one */
	ni->next = ti;
    }
    else aparm->items = ti;			/* we're first */
    return 0;
}

/* skip to next non-flag item, if any */
static AdvanceType(as, aval)
register long aval;
register struct cmd_syndesc *as; {
    register long next, n2;
    register struct cmd_parmdesc *tp;
    unsigned long cmdBit;

    /* first see if we should try to grab rest of line for this dude */
    if (as->parms[aval].flags & CMD_EXPANDS) return aval;

    /* if not, find next non-flag used slot (that doesn't conflict
       with anything used so far) */
    for (next = aval+1; next < CMD_MAXPARMS; ++next) {
	tp = &as->parms[next];
	if (tp->type != 0 && tp->type != CMD_FLAG) {
	    /* slot exists and isn't a flag; check conflicts */
	    cmdBit = CMD_BIT(next);
	    for (n2 = 0; n2 <= aval; ++n2) {
		tp = &as->parms[n2];
		if ((tp->alternatives & cmdBit) && tp->items) {
		    break;	/* 'n2' is used and conflicts with 'next' */
		}
	    }
	    /* Return 'next' if nothing conflicted with it. */
	    if (n2 > aval) return next;
	}
    }
    return aval;	/* fail it */
}

/* discard parameters filled in by dispatch */
static ResetSyntax(as)
register struct cmd_syndesc *as; {
    int i;
    register struct cmd_parmdesc *tp;
    register struct cmd_item *ti, *ni;

    tp = as->parms;
    for(i=0;i<CMD_MAXPARMS;i++, tp++) {
	switch(tp->type) {
	    case CMD_SINGLE:
	    case CMD_LIST:
		/* free whole list in both cases, just for fun */
		for(ti=tp->items;ti;ti=ni) {
		    ni = ti->next;
		    free(ti->data);
		    free(ti);
		}
		break;

	    default:
		break;
	}
	tp->items = (struct cmd_item *)0;
	tp->wasSwitchTyped = (int)0;
    }
}

/* move the expands flag to the last one in the list */
static SetupExpandsFlag(as)
register struct cmd_syndesc *as; {
    register struct cmd_parmdesc *tp;
    register int last, i;

    last = -1;
    /* find last CMD_LIST type parameter, optional or not, and make it expandable
	if no other dude is expandable */
    for(i=0;i<CMD_MAXPARMS;i++) {
	tp = &as->parms[i];
	if (tp->type == CMD_LIST) {
	    if (tp->flags & CMD_EXPANDS) return 0;	/* done if already specified */
	    last = i;
	}
    }
    if (last >= 0) as->parms[last].flags |= CMD_EXPANDS;
    return 0;
}

/*Take the current argv & argc and alter them so that the initialization opcode is made to appear.  This is used in cases where the initialization opcode is implicitly invoked.*/
static char **InsertInitOpcode(aargc, aargv)
int *aargc;
char **aargv;
{ /*InsertInitOpcode*/

    char **newargv;	/*Ptr to new, expanded argv space*/
    char *pinitopcode;	/*Ptr to space for name of init opcode*/
    int	i;		/*Loop counter*/

    /*Allocate the new argv array, plus one for the new opcode, plus one more for the trailing null pointer*/
    newargv = (char **) malloc(((*aargc)+2) * sizeof(char *));
    if (!newargv) {
	dce_printf(cmc_s_cm_cant_create_arg_array, aargv[0], *aargc);
	return((char **)0);
    }

    /*Create space for the initial opcode & fill it in*/
    pinitopcode = (char *) malloc(sizeof(initcmd_opcode));
    if (!pinitopcode) {
	dce_printf(cmc_s_cm_cant_malloc_opcode, aargv[0]);
	return((char **)0);
    }
    strcpy(pinitopcode, initcmd_opcode);

    /*Move all the items in the old argv into the new argv, in their proper places*/
    for (i = *aargc; i>1; i--)
	newargv[i] = aargv[i-1];

    /*Slip in the opcode and the trailing null pointer, and bump the argument count up by one for the new opcode*/
    newargv[0] = aargv[0];
    newargv[1] = pinitopcode;
    (*aargc)++;
    newargv[*aargc] = (char *)0;

    /*Return the happy news*/
    return(newargv);

} /*InsertInitOpcode*/

static NoParmsOK(as)
register struct cmd_syndesc *as; {
    register int i;
    register struct cmd_parmdesc *td;

    for(i=0;i<CMD_MAXPARMS;i++) {
	td = &as->parms[i];
	if (td->type != 0 && !(td->flags & CMD_OPTIONAL)) {
	    /* found a non-optional (e.g. required) parm, so NoParmsOK
	       is false (some parms are required) */
	    return 0;
	}
    }
    return 1;
}

#define CMD_USER_REPLYINVALID	(0)
#define CMD_USER_DEFAULT	(-1)
#define CMD_USER_Y		(-2)
#define CMD_USER_N		(-3)
#define CMD_USER_X		(-4)
#define CMD_USER_Q		(-5)
#define CMD_USER_PRINTHELP	(-6)
#define CMD_USER_NEXTOPTION	(-7)
#define CMD_USER_READERROR	(-8)
/*
 *  This routine gets the user input and checks that input buffer to determine
 * acceptabilty.  Return codes are:
 *
 *	CMD_USER_REPLY_INVALID => completely unacceptable, re-prompt
 *	<any positive number> => choice number typed by user
 *	CMD_USER_DEFAULT => default choice
 *	CMD_USER_Y => 'y'
 *	CMD_USER_N => 'n'
 *	CMD_USER_X => 'x'
 *	CMD_USER_Q => 'q'
 *	CMD_USER_PRINTHELP => '?'
 *	CMD_USER_NEXTOPTION => user typed option name, indexP points to the
 *				appropriate offset
 *	CMD_USER_READERROR => input read error on clearing of line
 */
static int cmd_ValidateInteractiveInput(lineP, rangeMax, accept_y, accept_n,
					accept_x, syntaxP, indexP)
     char *			lineP;
     int			rangeMax;
     int			accept_y;
     int			accept_n;
     int			accept_x;
     struct cmd_syndesc *	syntaxP;
     int *			indexP;
{
  int		rtnVal = CMD_USER_REPLYINVALID;
  int		i;
  unsigned int	j;
  int		printEllipsis = 0;
  unsigned long	matchSet;
  
  if (lineP[strlen(lineP) - 1] == '\n') {
    lineP[strlen(lineP) - 1] = '\0';	/* get rid of the newline */
    while (isspace(*lineP)) {		/* skip over white space */
      lineP++;
    }
     
    if (strlen(lineP) == 0) {
      rtnVal = CMD_USER_DEFAULT;
    }
    else {
      /* make sure the input is valid */
      if (isalpha(lineP[0]) && (strlen(lineP) == 1)) {
	switch (lineP[0]) {
	  case 'Y': case 'y':	rtnVal = ((accept_y) ? CMD_USER_Y : 0); break;
	  case 'N': case 'n':	rtnVal = ((accept_n) ? CMD_USER_N : 0); break;
	  case 'X': case 'x':	rtnVal = ((accept_n) ? CMD_USER_X : 0); break;
	  case 'Q': case 'q':	rtnVal = CMD_USER_Q; break;
	  default:		rtnVal = CMD_USER_REPLYINVALID; break;
	}
      }
      else if ((lineP[0] == '?') && (strlen(lineP) == 1)) {
	rtnVal = CMD_USER_PRINTHELP;
      }
      else if (lineP[0] == '-') {
	/*
	 *   The user may have typed an option name...determine whether
	 * or not it is valid.
	 */
	i = cmd_FindAllParams(syntaxP, lineP, &matchSet);
	if ((i != -1) && (matchSet == (unsigned long)0x0)) {
	  if (indexP != (int *)NULL) {
	    *indexP = i;
	  }
	  
	  rtnVal = CMD_USER_NEXTOPTION;
	}
	else if ((i != -1) && (matchSet != (unsigned long)0x0)) {
	  /*
	   *    The match was ambiguous; tell the user about all possibilities
	   *  and ask for disambiguation.
	   */
	  fprintf(stderr,
		  "The string '%s' is ambiguous; it could match any of:\n",
		  lineP);
	  fprintf(stderr, "\t%s\n", syntaxP->parms[i].name);
	  for (j = 0; j < CMD_MAXPARMS; j++) {
	    if ((j != i) &&	/* don't print the first match twice */
		(CMD_IS_BIT_SET(j, matchSet))) {
	      fprintf(stderr, "\t%s\n", syntaxP->parms[j].name);
	    }
	  }
	  fprintf(stderr,
		  "Please enter enough characters for an unambiguous match.\n");
	}
      }
      else if ((isdigit(lineP[0])) && (rangeMax > 0)) {
	/* check that we only have digits */
	for (j = 0; j < strlen(lineP); j++) {
	  if (!isdigit(lineP[j])) {
	    break;
	  }
	}
	
	/* if we only had digits, do the conversion */
	if (j == strlen(lineP)) {
	  i = atoi(lineP);
	  if (i <= rangeMax) {
	    rtnVal = i;
	  }
	}
      }
    }
  }
  else {
    /* clear to newline */
    printEllipsis = 1;
    rtnVal = ReadToNewline(stdin);
  }
  
  if (rtnVal == CMD_USER_REPLYINVALID) {
    dce_svc_printf(CMC_S_CM_REPLY_INVALID_MSG,
	    lineP, ((printEllipsis) ? "..." : ""));
  }
  
  return rtnVal;
}

static void ClearParamSlot (syntaxP, indexToClear)
     struct cmd_syndesc *	syntaxP;
     int			indexToClear;
{
  struct cmd_parmdesc *	thisParamP = &(syntaxP->parms[indexToClear]);
  struct cmd_item *	thisItemP;
  struct cmd_item *	nextItemP;

  if ((thisParamP->type == CMD_SINGLE) || (thisParamP->type == CMD_LIST)) {
    for (thisItemP = thisParamP->items;
	 thisItemP != (struct cmd_item *)NULL;
	 thisItemP = nextItemP) {
      nextItemP = thisItemP->next;
      free(thisItemP->data);
      free(thisItemP);
    }
  }
  thisParamP->items = (struct cmd_item *)NULL;
  thisParamP->wasSwitchTyped = 0;
}

static void cmd_PrintCurrentArgValues(paramDescP, stream)
     struct cmd_parmdesc *	paramDescP;
     FILE *			stream;
{
  struct cmd_item *	thisItemP = paramDescP->items;
  int			dataHasSpaces;
  
  if (thisItemP != (struct cmd_item *)NULL) {
    if (paramDescP->type != CMD_FLAG) {
      /* the first item */
      dataHasSpaces = ((strchr(thisItemP->data, ' ') != (char *)NULL) ||
		       (strchr(thisItemP->data, '\t') != (char *)NULL));
      fprintf(stream, "%s%s%s",
	      (dataHasSpaces ? "\"" : ""), thisItemP->data, (dataHasSpaces ? "\"" : ""));
      
      /* subsequent items */
      for (thisItemP = thisItemP->next;
	   thisItemP != (struct cmd_item *)NULL;
	   thisItemP = thisItemP->next) {
	dataHasSpaces = ((strchr(thisItemP->data, ' ') != (char *)NULL) ||
			 (strchr(thisItemP->data, '\t') != (char *)NULL));
	fprintf(stream, " %s%s%s",
		(dataHasSpaces ? "\"" : ""), thisItemP->data, (dataHasSpaces ? "\"" : ""));
      }
    }
  }
}

static void PrintAllArgValues(syntaxP, stream)
     struct cmd_syndesc *	syntaxP;
     FILE *			stream;
{
  int			i, j;
  unsigned int		longestOptionLength = 0;
  struct cmd_parmdesc *	thisParamP;
  
  /* first, go through them all, just to get the length for formatting */
  for (i = 0; i < CMD_HELPPARM; i++) {
    thisParamP = &(syntaxP->parms[i]);
    if ((thisParamP->type != 0) && (strlen(thisParamP->name) > longestOptionLength)) {
      longestOptionLength = strlen(thisParamP->name);
    }
  }
  
  fprintf(stream, "\nThe current set of parameter values is:\n");
  for (i = 0; i < CMD_HELPPARM; i++) {
    thisParamP = &(syntaxP->parms[i]);
    if ((thisParamP->type != 0) && (thisParamP->items != (struct cmd_item *)NULL)) {
      fprintf(stream, "%s", thisParamP->name);
      if (thisParamP->type != CMD_FLAG) {
	putc(':', stream);
	for (j = strlen(thisParamP->name); j < longestOptionLength; j++) {
	  /* print the padding */
	  putc(' ', stream);
	}
	putc('\t', stream);
	cmd_PrintCurrentArgValues(thisParamP, stream);
      }
      
      fprintf(stream, "\n");
    }
  }
}

static int ReadToNewline(stream)
     FILE *	stream;
{
  int		rtnVal = 0;	/* assume success */
  char		buffer[BUFSIZ];

  do {
    if (fgets(buffer, sizeof(buffer), stream) != buffer) {
      dce_svc_printf(CMC_S_CM_READ_ERROR_MSG,errno);
      rtnVal = CMD_USER_READERROR;
    }
  } while ((buffer[strlen(buffer) - 1] != '\n') && (rtnVal == 0));

  return rtnVal;
}

#define MAXLIST		(BUFSIZ / 2)
IMPORT int cmd_param_CheckDataValid _TAKES((
					    struct cmd_parmdesc *	theParamDescP,
					    char *			dataP
					  ));
static struct cmd_item * ReadNewValue(syntaxP, paramIndex)
     struct cmd_syndesc *	syntaxP;
     int			paramIndex;
{
  struct cmd_item *	rtnValP = (struct cmd_item *)NULL;
  struct cmd_parmdesc *	thisParamP= &(syntaxP->parms[paramIndex]);
  char *		invalidStrP = (char *)NULL;
  int			printEllipsis = 0;
  struct cmd_item *	thisItemP = (struct cmd_item *)NULL;
  struct cmd_item *	nextItemP = (struct cmd_item *)NULL;
  int			choice;
  char *		argv[MAXLIST];
  long			argc;
  int			parseCode;
  int			i, j;
  int			validationCode;
  char			userBuffer[BUFSIZ];
  
  do {
    if (syntaxP->parms[paramIndex].type == CMD_FLAG) {
      fprintf(stdout, "\nUse %s flag {y, n} [%c]? ",
	      syntaxP->parms[paramIndex].name,
	      ((syntaxP->parms[paramIndex].items != (struct cmd_item *)NULL) ?
	       'y' : 'n'));
    }
    else {
      fprintf(stdout, "Please enter a%s",
	      (syntaxP->parms[paramIndex].type == CMD_LIST) ?
	      " (space separated) list of" : " single");
      cmd_PrintParamHelp(&(syntaxP->parms[paramIndex]));
      fprintf(stdout, " for %s [", thisParamP->name);
      cmd_PrintCurrentArgValues(thisParamP, stdout);	/* not a flag */
      fprintf(stdout, "]: ");
    }
    fflush(stdout);
    
    if (fgets(userBuffer, sizeof(userBuffer), stdin) == userBuffer) {
      if (userBuffer[strlen(userBuffer) - 1] == '\n') {
	userBuffer[strlen(userBuffer) - 1] = '\0';
	if (strlen(userBuffer) == 0) {
	  if ((thisParamP->flags & CMD_OPTIONAL) ||
	      (thisParamP->items != (struct cmd_item *)NULL)) {
	    rtnValP = thisParamP->items;
	  }
	  else {
	    dce_svc_printf(CMC_S_CM_TRYAGAIN_MSG);
	    validationCode = 1;
	  }
	}
	else {
	  parseCode = cmd_ParseLine(userBuffer, argv, &argc,
				    sizeof(argv) / sizeof(argv[0]));
	  if (parseCode == 0) {
	    /* cmd_ParseLine will leave the first entry blank! */
	    if ((thisParamP->type != CMD_LIST) && (argc > 2)) {
	      dce_svc_printf(CMC_S_CM_ONEARG_MSG);
	      validationCode = 1;
	    }
	    else {
	      if (thisParamP->type == CMD_FLAG) {
		if ((argv[1][0] == 'Y') || (argv[1][0] == 'y')) {
		  rtnValP = thisParamP->items = (struct cmd_item *)&dummy;
		  validationCode = 0;
		}
		else if ((argv[1][0] == 'N') || (argv[1][0] == 'n')) {
		  rtnValP = thisParamP->items = (struct cmd_item *)NULL;
		  validationCode = 0;
		}
		else {
		  dce_svc_printf(CMC_S_CM_YORN_MSG);
		  validationCode = 1;
		}
	      }
	      else {
		/*
		 *    These cmd_items must be allocated one at a time because
		 *  that is the way the non-interactive version works, so the
		 *  code to free them makes that assumption.
		 */
		rtnValP = (struct cmd_item *)malloc(sizeof(struct cmd_item));
		if (rtnValP != (struct cmd_item *)NULL) {
		  validationCode = 0;
		  for (i = 1; (i < argc) && (validationCode == 0); i++) {
		    validationCode = cmd_param_CheckDataValid(thisParamP,
							      argv[i]);
		    if (validationCode != 0) {
		      invalidStrP = argv[i];
		    }
		  }	/* end for looping over entered (valid) data */
		
		  if (validationCode == 0) {
		    for (i = 1, j = 0, thisItemP = rtnValP;
			 (i < argc) && (thisItemP != (struct cmd_item *)NULL);
			 i++, j++, thisItemP = thisItemP->next) {
		      thisItemP->data = argv[i];
		      thisItemP->next = ((i < (argc - 1)) ?
					 ((struct cmd_item *)
					  malloc(sizeof(struct cmd_item))) :
					 (struct cmd_item *)NULL);
		    }	/* end for looping over entered data */
		  }	/* end if all the data entered are valid */

		  if ((validationCode != 0) || (i < argc)) {
		    /*
		     *   Free everything alloc'd, since we have no idea how many
		     * items will be typed the next time around the loop.
		     */
		    for (thisItemP = rtnValP;
			 thisItemP != (struct cmd_item *)NULL;
			 thisItemP = nextItemP) {
		      /*
		       *    Don't free the data, since we may not have it all
		       *  in the argument structure.  We do that with
		       *  the cmd_FreeArgv procedure.
		       */
		      nextItemP = thisItemP->next;
		      free(thisItemP);
		    }
		     
		    cmd_FreeArgv(argv);
 		    free((char *)rtnValP);
		    rtnValP = (struct cmd_item *)NULL;

		    if (i < argc) {
		      dce_svc_printf(CMC_S_CM_BAD_ARGS_MSG);
		      /* get out of the loop, since we're in trouble: */
		      validationCode = 0;
		    }
		  }
 		}
		else {
		  /* malloc of rtnValP failed */
		  dce_svc_printf(CMC_S_CM_BAD_MEM_MSG);
		
		  /* free the things we were able to allocate */
		  cmd_FreeArgv(argv);

		  /* get out of the loop, since we're in trouble: */
		  validationCode = 0;
		}
	      
	      }	/* end else (param is not a flag) */
	    }	/* end else (right number of arguments) */
	  }
	  else {
	    /* parse of user-entered line failed */
	    dce_svc_printf(CMC_S_CM_CANT_PARSE_MSG,
		    userBuffer);
	    validationCode = 1;
	  }
	}	/* end else - if user typed something */
      }
      else {
	/* line entered was too long for buffer */
	printEllipsis = 1;
	invalidStrP = userBuffer;
	(void)ReadToNewline(stdin);
	validationCode = 1;
      }

      if (invalidStrP != (char *)NULL) {
	dce_svc_printf(CMC_S_CM_ILLEGAL_VAL_MSG,
		invalidStrP,
		(printEllipsis ? "..." : ""),
		thisParamP->name);
	invalidStrP = (char *)NULL;	/* don't need it after using it here */
	printEllipsis = 0;
      }
    }
    else {
      /* fgets failed */
      dce_svc_printf(CMC_S_CM_BAD_INPUT_MSG);
      validationCode = 0; /* get out of the loop, since we're in trouble */
    }
  } while (validationCode != 0);

  return rtnValP;
}

static void PrintInteractiveHelp(syntaxP, paramIndex, stream)
     struct cmd_syndesc *	syntaxP;
     int			paramIndex;
     FILE *			stream;
{
  struct cmd_parmdesc *		thisParamP = &(syntaxP->parms[paramIndex]);
  struct cmd_item *		thisItemP;
  int				i;

  if (thisParamP->helpEntryP != (cmd_help_entry_t *)NULL) {
    fprintf(stream, "\n%s:\n", thisParamP->helpEntryP->paramOptArgP);
    (*(thisParamP->helpEntryP->printHelpP))(stream);
  }
  else {
    fprintf(stream, "Sorry, no interactive help available for %s.\n", thisParamP->name);
  }

  if (thisParamP->items != (struct cmd_item *)NULL) {
    if (thisParamP->type == CMD_FLAG) {
      fprintf(stream, "\n%s flag is currently set.\n", thisParamP->name);
    }
    else {
      fprintf(stream, "\nCurrent value%s of %s: ",
	      ((thisParamP->items->next != (struct cmd_item *)NULL) ? "s" : ""),
	      thisParamP->name);
      cmd_PrintCurrentArgValues(thisParamP, stream);
      fprintf(stream, "\n");
    }
  }
  
  if (thisParamP->alternatives != 0) {
    /* this will only be called for the first member of the alt set */
    for (i = paramIndex + 1; i < CMD_HELPPARM; i++) {
      if (CMD_IS_BIT_SET(i, thisParamP->alternatives)) {
	thisParamP = &(syntaxP->parms[i]);
	if (thisParamP->helpEntryP != (cmd_help_entry_t *)NULL) {
	  fprintf(stream, "\n%s:\n", thisParamP->helpEntryP->paramOptArgP);
	  (*(thisParamP->helpEntryP->printHelpP))(stream);
	}
	else {
	  fprintf(stream, "\nSorry, no interactive help available for %s.\n",
		  thisParamP->name);
	}
	if (thisParamP->items != (struct cmd_item *)NULL) {
	  if (thisParamP->type == CMD_FLAG) {
	    fprintf(stream, "\n%s flag is currently set.\n", thisParamP->name);
	  }
	  else {
	    fprintf(stream, "\nCurrent value%s of %s: ",
		    ((thisParamP->items->next != (struct cmd_item *)NULL) ? "s" : ""),
		    thisParamP->name);
	    cmd_PrintCurrentArgValues(thisParamP, stream);
	    fprintf(stream, "\n");
	  }
	}
      }
    }
  }
  fflush(stream);
}

/*  returns the index for first missing required parameter */
static int CheckAllRequiredParamsPresent
#if defined(__STDC__)
(
  struct cmd_syndesc *	syntaxP
)
#else /* defined(__STDC__) */
( syntaxP )
  struct cmd_syndesc *	syntaxP;
#endif /* defined(__STDC__) */
{
  int	rtnVal = -1;	/* assume that they're all there */
  int	i;
  struct cmd_parmdesc *	thisParamP;
  
  for (i = 0; (rtnVal == -1) && (i < CMD_MAXPARMS); i++) {
    thisParamP = &(syntaxP->parms[i]);
    if ((thisParamP->type != 0) && (!(thisParamP->flags & CMD_OPTIONAL)) &&
	(thisParamP->items == (struct cmd_item *)NULL) &&
	(cmd_CheckAlternateUsage(syntaxP, i) == 0)) {
      rtnVal = i;
    }
  }
   
  return rtnVal;
}

static long cmd_ProcessInteraction(syntaxP, startIndex)
     struct cmd_syndesc *	syntaxP;
     int			startIndex;
{
  long			rtnVal = 0;	/* assume success */
  int			continueInteraction = 1;
  int			startLoopHere = startIndex;
  int			thisParamIndex;
  struct cmd_parmdesc *	thisParamP;
  int			i;
  int			choice;
  int			choiceCount;
  int			prevChoice;
  int			defaultChoice;
  int			paramOptional;
  int			paramAlreadyGiven;
  int			paramEntryRequired;
  int			nextIndex;
  int			acceptYes = 0;
  struct cmd_item *	newItemsP;
  int			altSet[CMD_MAXPARMS];
  int			nextAlt;
  char			userBuffer[BUFSIZ];
  
  while (continueInteraction) {
    thisParamIndex = startLoopHere;
    while (continueInteraction && (thisParamIndex < CMD_HELPPARM)) {
      thisParamP = &(syntaxP->parms[thisParamIndex]);

      /*
       *  Skip to the next param slot if this slot is unused, or if it is part of a set
       * of mutually exclusive alternatives and is not the FIRST param in that set.
       */
      if ((thisParamP->type == 0) ||
	  ((thisParamP->alternatives) &&
	   (cmd_TestParamPrint(thisParamP, thisParamIndex) == 0))) {
	thisParamIndex++;
	continue;
      }
      
      paramOptional = ((thisParamP->flags & CMD_OPTIONAL) ? 1 : 0);
      
      /* print an appropriate prompt for this param slot */
      if (thisParamP->alternatives) {
	/* this is the first of a set of mutually exclusive alternatives... */

	/* compact and simplify the alternatives set for later calculation */
	nextAlt = 0;
	altSet[nextAlt++] = thisParamIndex;
	for (i = thisParamIndex + 1; i < CMD_HELPPARM; i++) {
	  if (CMD_IS_BIT_SET(i, thisParamP->alternatives)) {
	    altSet[nextAlt] = i;
	    nextAlt++;
	  }
	}
	altSet[nextAlt] = -1;	/* mark the end of the set (a tad redundant, but useful) */

	/*
	 *   Before we prompt, we need to find out if any of these options have
	 * already been specified.
	 */
	prevChoice = -1;
	paramAlreadyGiven = 0;
	for (i = 0; (i < nextAlt) && (!paramAlreadyGiven); i++) {
	  if (syntaxP->parms[altSet[i]].items != (struct cmd_item *)NULL) {
	    paramAlreadyGiven = 1;
	    prevChoice = altSet[i];
	  }
	}

	/* first, we print the one liner.... */
	fprintf(stdout, "\n%s of the following alternatives:\n",
		((paramAlreadyGiven) ? "Change selection" : "Choose one"));
	if (paramAlreadyGiven) {
	  fprintf(stdout, "Current selection is %s", syntaxP->parms[prevChoice].name);
	  if (syntaxP->parms[prevChoice].type != CMD_FLAG) {
	    fprintf(stdout, ", value%s ",
		    ((syntaxP->parms[prevChoice].items->next != (struct cmd_item *)NULL) ?
		     "s" : ""));
	    cmd_PrintCurrentArgValues(&(syntaxP->parms[prevChoice]), stdout);
	  }
	  fprintf(stdout, ".\n");
	}

	/* then the choices with their indices */
	for (i = 0, choiceCount = 0; i < nextAlt; i++) {
	  choiceCount++;
	  fprintf(stdout, "\t%2d. %s\n", choiceCount, syntaxP->parms[altSet[i]].name);
	}
	
	paramEntryRequired = ((!paramOptional) && (!paramAlreadyGiven));

	/* if these are optional, allow "None of the above" */
	if (paramOptional) {
	  choiceCount++;
	  fprintf(stdout, "\t %d. None of the above.\n", choiceCount);
	}
	
	defaultChoice = ((paramAlreadyGiven) ? CMD_USER_N :
			 ((paramOptional) ? choiceCount : 1 /* pretty arbitrary */));
	do {	/* prompt for and validate user input */
	  fprintf(stdout, "\nEnter number {1..%d,%s q,%s ?} [",
		  choiceCount,
		  ((paramEntryRequired) ? "" : " n, x,"),
		  ((paramEntryRequired) ? "" : " <option_name>,"));
	  if (defaultChoice == CMD_USER_N) {
	    putc('n', stdout);
	  }
	  else {
	    fprintf(stdout, "%d", defaultChoice);
	  }
	  fprintf(stdout, "]: ");
	  fflush(stdout);
	  
	  if (fgets(userBuffer, sizeof(userBuffer), stdin) == userBuffer) {
	    choice = cmd_ValidateInteractiveInput(userBuffer, choiceCount,
						  0 /* no y */,
						  !paramEntryRequired /* n if not reqd */,
						  !paramEntryRequired /* x if not reqd */,
						  syntaxP,
						  ((paramEntryRequired) ?
						   (int *)NULL : &nextIndex));
	    if (choice == CMD_USER_PRINTHELP) {
	      PrintInteractiveHelp(syntaxP, thisParamIndex, stdout);
	    }
	  }
	  else {
	    dce_svc_printf(CMC_S_CM_BAD_READ_MSG,errno);
	    choice = CMD_USER_READERROR;
	  }
	} while ((choice == CMD_USER_REPLYINVALID) || (choice == CMD_USER_PRINTHELP));
	
	if (choice == CMD_USER_DEFAULT) {	/* map the default to a set index */
	  choice = defaultChoice;
	}
	if (choice > 0) {
	  /* map the set index to a parameter index */
	  if ((paramEntryRequired) || (choice < choiceCount)) {
	    /* this is the not the "None of the Above" option */
	    choice = altSet[choice - 1];
	  }
	}
	else if (choice == CMD_USER_NEXTOPTION) {
	  choice = nextIndex;
	}
	/* other alternatives for choice values are handled below */

	/* make sure alternatives that are not chosen have been cleared */
	if ((choice != CMD_USER_X) && (choice != CMD_USER_N) &&
	    paramAlreadyGiven && (choice != prevChoice)) {
	  if (syntaxP->parms[prevChoice].items != (struct cmd_item *)NULL) {
	    ClearParamSlot(syntaxP, prevChoice);
	  }
 	}	/* end if there was a previous choice, and the user changed it */

	if ((!paramEntryRequired) && (choice == choiceCount)) {
	  /* this is the none of the above option */
	  choice = CMD_USER_N;	/* no further processing */
	}

	/* handle flags here */
	if ((choice > 0) && (syntaxP->parms[choice].type == CMD_FLAG)) {
	  syntaxP->parms[choice].items = (struct cmd_item *)&dummy;
	  choice = CMD_USER_N;	/* no further processing */
	}
      }	
      else {
	paramAlreadyGiven = ((thisParamP->items != (struct cmd_item *)NULL) ? 1 : 0);
	paramEntryRequired = ((!paramOptional) && (!paramAlreadyGiven));

	do {
	  if (thisParamP->type == CMD_FLAG) {
	    fprintf(stdout, "\nUse %s flag {y, n, x, q, <option_name>, ?} [%c]? ",
		    thisParamP->name, ((paramAlreadyGiven) ? 'y' : 'n'));
	  }
	  else {
	    if (paramAlreadyGiven) {
	      fprintf(stdout, "\nCurrent value%s of %s %s: ",
		      ((thisParamP->items->next != (struct cmd_item *)NULL) ? "s" : ""),
		      thisParamP->name,
		      ((thisParamP->items->next != (struct cmd_item *)NULL) ?
		       "are" : "is"));
	      cmd_PrintCurrentArgValues(thisParamP, stdout);
	    }
	    
	    fprintf(stdout, "\n%s%s argument %s {y,%s q,%s ?} [%c]? ",
		    ((paramAlreadyGiven) ? "Change" : "Enter"),
		    ((paramOptional) ? " (optional)" : " (required)"),
		    thisParamP->name,
		    ((paramEntryRequired) ? "" : " n, x,"),
		    ((paramEntryRequired) ? "" : " <option_name>,"),
		    ((paramEntryRequired) ? 'y' : 'n'));
	  }
	  fflush(stdout);

	  if (fgets(userBuffer, sizeof(userBuffer), stdin) == userBuffer) {
	    choice = cmd_ValidateInteractiveInput(userBuffer, -1 /* accept no numbers */,
						  1 /* always accept y */,
						  !paramEntryRequired /* n if not reqd */,
						  !paramEntryRequired /* x if not reqd */,
						  syntaxP,
						  ((paramEntryRequired) ?
						   (int *)NULL : &nextIndex));
	    if (choice == CMD_USER_PRINTHELP) {
	      PrintInteractiveHelp(syntaxP, thisParamIndex, stdout);
	    }
	  }
	  else {
	    dce_svc_printf(CMC_S_CM_BAD_READ_MSG,errno);
	    choice = CMD_USER_READERROR;
	  }
	} while ((choice == CMD_USER_REPLYINVALID) || (choice == CMD_USER_PRINTHELP));

	/* adjust the reply in some cases */
	/* we do the flag processing here, because it makes the later code cleaner */
	switch (choice) {
	  case CMD_USER_Y:
	    if (thisParamP->type != CMD_FLAG) {
	      choice = thisParamIndex;
	    }
	    else {
	      thisParamP->items = (struct cmd_item *)&dummy;
	      choice = CMD_USER_N;	/* no further processing */
	    }
	    break;
	  case CMD_USER_N:
	    if (thisParamP->type == CMD_FLAG) {
	      thisParamP->items = (struct cmd_item *)NULL;
	    }
	    break;
	  case CMD_USER_DEFAULT:
	    if (thisParamP->type != CMD_FLAG) {
	      choice = (paramEntryRequired) ? thisParamIndex : CMD_USER_N;
	    }
	    else {
	      /* default on flags is to maintain the status quo */
	      choice = CMD_USER_N;	/* no further processing */
	    }
	    break;
	  case CMD_USER_NEXTOPTION:
	    choice = nextIndex;
	    break;
	}	/* end switch */
      }
      
      /*
       *   At this point, if choice is non-negative, it indicates the index of a parameter
       * whose argument value should be read.  Otherwise, we don't read the current 
       * argument, though we may read another.
       */
      if (choice >= 0) {
	/* we have to read the user's choice for the new values */
	newItemsP = ReadNewValue(syntaxP, choice);
	if ((newItemsP != (struct cmd_item *)NULL) ||
	    (syntaxP->parms[choice].flags & CMD_OPTIONAL)) {
	  if (newItemsP != syntaxP->parms[choice].items) {	/* if there is a change */
	    if (syntaxP->parms[choice].items != (struct cmd_item *)NULL) {
	      ClearParamSlot(syntaxP, choice);	/* clear out old value */
	    }
	    
	    syntaxP->parms[choice].items = newItemsP;
	  }
	}
	else {
	  /* get out, we have a problem */
	  rtnVal = CMD_INTERNALERROR;
	  continueInteraction = 0;
	}
	
	thisParamIndex = choice + 1;
      }
      else if (choice == CMD_USER_N) {
	thisParamIndex++;
      }
      else {
	/* we have some sort of additional processing to do */
	switch (choice) {
	  case CMD_USER_Q:
	    rtnVal = CMD_ABORTED_AT_USER_REQUEST;
	    continueInteraction = 0;
	    break;
	  case CMD_USER_READERROR:
	    rtnVal = CMD_INTERNALERROR;
	    continueInteraction = 0;
	    break;
	  case CMD_USER_X:
	    continueInteraction = 0;
	    break;
	}	/* end switch */
      }
    }	/* end while loop iterating over the parameter slots */
    
    /* present all the values to the user, and ask for final validation */
    if (rtnVal == 0) {
      PrintAllArgValues(syntaxP, stdout);
      startLoopHere = CheckAllRequiredParamsPresent(syntaxP);
      acceptYes = (startLoopHere == -1);
      
      do {
	fprintf(stdout,
		"\nExecute this command {%sn, q, %s<option_name>, ?} [%c]? ",
		((acceptYes) ? "y, " : ""), ((acceptYes) ? "x, " : ""),
		((acceptYes) ? 'y' : 'n'));
	fflush(stdout);
	
	if (fgets(userBuffer, sizeof(userBuffer), stdin) == userBuffer) {
	  choice = cmd_ValidateInteractiveInput(userBuffer,
						-1 /* accept no numbers */,
						acceptYes,
						1 /* always accept n */,
						1 /* always accept x */,
						syntaxP, &nextIndex);
	  if (choice == CMD_USER_PRINTHELP) {
	    fprintf(stdout, "\n");
	    PrintSyntax(syntaxP);
	    PrintAllArgValues(syntaxP, stdout);
	    fprintf(stdout, "\n");
	  }
	}
	else {
	  dce_svc_printf(CMC_S_CM_READ_ERROR_MSG,errno);
	  choice = CMD_USER_READERROR;
	}
      } while ((choice == CMD_USER_REPLYINVALID) ||
	       (choice == CMD_USER_PRINTHELP));

      if (choice == CMD_USER_DEFAULT) {
	choice = ((acceptYes) ? CMD_USER_Y : CMD_USER_N);
      }
       
      switch (choice) {
	case CMD_USER_Y:
	case CMD_USER_X:
	  continueInteraction = 0;
	  break;
	case CMD_USER_N:
	  if (startLoopHere == -1) {
	    startLoopHere = 0;
	  }
	  continueInteraction = 1;
	  break;
	case CMD_USER_NEXTOPTION:
	  continueInteraction = 1;
	  startLoopHere = nextIndex;
	  break;
	case CMD_USER_Q:
	  rtnVal = CMD_ABORTED_AT_USER_REQUEST;
	  continueInteraction = 0;
	  break;
	case CMD_USER_READERROR:
	  rtnVal = CMD_INTERNALERROR;
	  continueInteraction = 0;
	  break;
      }	/* end switch */
    }
  }	/* end while (continueInteraction) loop */
  
  return rtnVal;
}

/* Call the appropriate function, or return syntax error code.  Note: if no opcode is specified, an initialization routine exists, and it has NOT been called before, we invoke the special initialization opcode*/
#define CMD_IS_HELP_STR(strP) \
  ((strcmp("help", (strP)) == 0) || (strcmp("apropos", (strP)) == 0) || \
   (strcmp("-help", (strP)) == 0) || (strcmp("-apropos", (strP)) == 0))

long cmd_Dispatch(argc, argv)
int argc;
char **argv;
 {
    char *pname;
    register struct cmd_syndesc *ts;
    register struct cmd_parmdesc *tparm;
    struct cmd_syndesc * initCmdSyntaxP;
    register long i, j;
    long parseStartPosition = 2;	/* normal parsing starts at second elt of argv */
    int curType;
    int positional;
    int ambig;
    int ambig_switch;
    static int initd = 0;	    /*Is this the first time this routine has been called?*/
    static int initcmdpossible = 1; /*Should be consider parsing the initial command?*/

    /* variables used in checking for conflicting parameter usage */
    int 	conflictIndex;		/* the smallest conflicting parameter index */
    int		minConflictIndex;	/* used in determining which parameter name
					   is printed first */
    int		maxConflictIndex;	/* used in determining which parameter name
					   is printed first */

    int		interactionRequested = 0;

    int		rtnVal = 0;		/* provisional return value, assume success */

    if (!initd) {
      initd = 1;
      /* Add help, apropos commands once */
      if (!noOpcodes) {
	/* find out if there is an initcmd syntax */
	initCmdSyntaxP = cmd_FindSyntax(initcmd_opcode, 0);
	
	if (!initCmdSyntaxP) {
	  ts = cmd_CreateSyntax("help", HelpProc, 0, "get help on commands");
	  cmd_AddParm(ts, "-topic", CMD_LIST, CMD_OPTIONAL, "string");
	  cmd_CreateAlias(ts, "-help");
	  
	  ts = cmd_CreateSyntax("apropos", AproposProc, 0, "search by help text");
	  cmd_AddParm(ts, "-topic", CMD_SINGLE, CMD_REQUIRED, "string");
	  cmd_CreateAlias(ts, "-apropos");
	}
#ifdef notdef
	else {
	  /* we want help and apropos treated slightly differently in this case */
	  ts = cmd_CreateSyntax("help", PrintSyntax, (char *)initCmdSyntaxP,
				"get help on commands");
	  cmd_AddParm(ts, "-topic", CMD_LIST, CMD_OPTIONAL, "string");
	  cmd_CreateAlias(ts, "-help");
	  
	  /* this is slightly cheating, since it won't really do a search */
	  ts = cmd_CreateSyntax("apropos", PrintSyntax, (char *)initCmdSyntaxP,
				"search by help text");
	  cmd_AddParm(ts, "-topic", CMD_SINGLE, CMD_REQUIRED, "string");
	  cmd_CreateAlias(ts, "-apropos");
	}
#endif /* notdef */
      }
    }
    
    /*Remember the program name*/
    pname = argv[0];
    
    if (noOpcodes) {
	if (argc == 1) {
	    if (!NoParmsOK(allSyntax)) {
		dce_printf(cmc_s_cm_type_help, pname, pname);
		return(CMD_USAGE);
	    }
	}
    }
    else {
	if (argc < 2) {
	    /* if there is an initcmd, don't print an error message, just
	       setup to use the initcmd below. */
	    if (!(initcmdpossible && cmd_FindSyntax(initcmd_opcode, (int *) 0))) {
		dce_printf(cmc_s_cm_help_or_help, pname, pname, pname);
		return(CMD_USAGE);
	    }
	}
    }
    
    /* Find the syntax descriptor for this command, doing prefix matching properly */
    if (noOpcodes) {
      ts = allSyntax;
      
      /*
       * recognize the help aliases explicitly...pretty ugly, but it gets the
       * job done for now
       */
      if ((argc > 1) && CMD_IS_HELP_STR(argv[1])) {
	ts->parms[CMD_HELPPARM].items = (struct cmd_item *) &dummy;	/* force help */
	
	parseStartPosition = 2;	/* skip over help/apropos string */
      }
      else {
	parseStartPosition = 1;	/* start at first arg */
      }
    }
    else {
      ts = (argc < 2? 0 : cmd_FindSyntax(argv[1], &ambig));
      if (!ts) {
	/*First token doesn't match a syntax descriptor*/
	if (initcmdpossible) {
	  /*If initial command line handling hasn't been done yet,
	    see if there is a descriptor for the initialization opcode.
	    Only try this once.*/
	  initcmdpossible = 0;
	  ts = cmd_FindSyntax(initcmd_opcode, (int *) 0);
	  if (!ts) {
	    /*There is no initialization opcode available, so we declare
	      an error*/
	    printf("%s", NName(pname, ": "));
	    dce_printf(cmc_s_cm_operation_list, 
		   (ambig ? "Ambiguous" : "Unrecognized"),
		   argv[1], NName(pname," "));
	    
	    return(CMD_UNKNOWNCMD);
	  }
	  else {
	    /*Found syntax structure for an initialization opcode.  Fix
	      up argv and argc to relect what the user
	      ``should have'' typed*/
#if defined(notdef)
	      if (!(argv = InsertInitOpcode(&argc, argv))) {
	        dce_printf(cmc_s_cm_cant_insert_opcode,
		           NName(pname, ": "));
	        return(CMD_INTERNALERROR);
	      }
#endif /* defined(notdef) */
	    /*
	     * recognize the help aliases explicitly...pretty ugly, but it
	     * gets the job done for now
	     */
	    if ((argc > 1) && CMD_IS_HELP_STR(argv[1])) {
	      ts->parms[CMD_HELPPARM].items =
		(struct cmd_item *) &dummy;	/* force help */
	      
	      parseStartPosition = 2;	/* skip over help/apropos string */
	    }
	    else {
	      parseStartPosition = 1;	/* start at first arg */
	    }
	    
	  }
	} /*Initial opcode not yet attempted*/
	else {
	  /* init cmd already run and no syntax entry found */
	  printf("%s", NName(pname, ": "));
	  printf("%s operation '%s'; type '%shelp' for list\n",
		 (ambig ? "Ambiguous" : "Unrecognized"),
		 argv[1], NName(pname, " "));
	  return CMD_UNKNOWNCMD;
	}
      } /*Argv[1] is not a valid opcode*/
    } /*Opcodes are defined*/
    
    /* Found the descriptor; start parsing.  curType is the type we're trying to parse */
    curType = 0;
    
    /* We start off parsing in "positional" mode, where tokens are put in
       slots positionally.  If we find a name that takes args, we go
       out of positional mode, and from that point on, expect a switch
       before any particular token. */
    positional = 1; /* Are we still in the positional region of the cmd line? */
    i = parseStartPosition;
    SetupExpandsFlag(ts);
    for(;i<argc; i++) {
      /*
       *   Process an interaction request right away and get it out of the way
       * of the other processing.
       */
      if ((argv[i][0] == INTERACTIVE_KEY_CHAR) && (strlen(argv[i]) == 1)) {
	interactionRequested = 1;
      }
      
      /* Only tokens that start with a hyphen and are not followed by a digit
       * are considered switches.  This allow negative numbers. */

      /*
       *    The checking of the second character for a digit is still supported
       *  so that scripts that may have relied on not having to quote a negative
       *  number in any way will still continue to opertate correctly.
       *
       *    Note that if CMD_SINGLE_CHAR_QUOTE is ever changed to '-', the
       *  following conditional must be changed.
       */
      else if ((argv[i][0] == '-') && !isdigit(argv[i][1])) {
	/* Find switch */
	j = FindType(ts, argv[i], &ambig_switch);
	if (j < 0) {
	  dce_printf(cmc_s_cm_switch_type, NName(pname, ": "),
		 (ambig_switch ? "Ambiguous" : "Unrecognized"),
		 argv[i]);
	  if (noOpcodes || (strcmp(ts->name, initcmd_opcode) == 0))
	    dce_printf(cmc_s_cm_detailed_help, argv[0]);
	  else
	    dce_printf(cmc_s_cm_help_detailed,
		   NName(argv[0], " "), ts->name);
	  ResetSyntax(ts);
	  return(CMD_UNKNOWNSWITCH);
	}
	if (j >= CMD_MAXPARMS) {
	  dce_printf(cmc_s_cm_parsing_error, NName(pname, ": "));
	  ResetSyntax(ts);
	  return(CMD_INTERNALERROR);
	}
	if (ts->parms[j].type == CMD_FLAG) {
	  ts->parms[j].items = (struct cmd_item *) &dummy;
	}
	else {
	  ts->parms[j].wasSwitchTyped = 1;
	  positional = 0;
	  curType = j;
	}
      }
      else {
 	/* Try to fit in this descr */
	if (curType >= CMD_MAXPARMS) {
	  dce_printf(cmc_s_cm_too_many_args, NName(pname, ": "));
	  ResetSyntax(ts);
	  return(CMD_TOOMANY);
	}
	tparm = &ts->parms[curType];
	if (tparm->type == 0) {
	  /* skipped parm slot */
	  curType++;  /* Skip this slot and reprocess this parm */
	  i--;
	  continue;
	}
	if (tparm->type == CMD_FLAG) {
	  curType++;  /* This parm must be set explicitly */
	  i--;	    /* Reprocess this parameter */
	  continue;
	}
	if (tparm->type == CMD_SINGLE) {
	  if (tparm->items) {
	    dce_printf(cmc_s_cm_too_many_values,
		   NName(pname, ": "), tparm->name);
	    ResetSyntax(ts);
	    return(CMD_NOTLIST);
	  }
	  AddItem(tparm, argv[i]);    /* Add to end of list */
	}
	else if (tparm->type == CMD_LIST) {
	  AddItem(tparm, argv[i]);    /* Add to end of list */
	}
	/* Now, if we're in positional mode, advance to the next item */
	if (positional)
	  curType = AdvanceType(ts, curType);
      }
    }
    
    /* keep track of this for messages */
    ts->a0name = argv[0];
    
    /* If we make it here, all the parameters are filled in.  Check to see if this
       is a -help version.  Must do this before checking for all required parms,
       otherwise it is a real nuisance */
    if (ts->parms[CMD_HELPPARM].items) {
      PrintSyntax(ts);
      ResetSyntax(ts);
      return 0;
    }
    
    /* Parsing done, see if we have all of our required parameters */
    /* also check that we have not used multiple elements of any mutually exclusive sets */
    rtnVal = 0;
    for(i = 0; (rtnVal == 0) && (i < CMD_MAXPARMS); i++) {
      tparm = &ts->parms[i];
      if (tparm->type	== 0) continue;	/* Skipped parm slot */
      
      /* required parameter check */
      if (!(tparm->flags & CMD_OPTIONAL) && (tparm->items == 0) &&
	  (cmd_CheckAlternateUsage(ts, i) == 0)) {
	    dce_printf(cmc_s_cm_missing_param,
		   NName(pname, ": "), tparm->name);
	rtnVal = CMD_TOOFEW;
      }
      else if ((conflictIndex = cmd_CheckConflictingUsage(ts, i)) != -1) {
	/* mutually exclusive element check */
	minConflictIndex = (conflictIndex < i) ? conflictIndex : i;
	maxConflictIndex = (conflictIndex > i) ? conflictIndex : i;
	dce_printf(cmc_s_cm_bad_param_combo,
	       ts->parms[minConflictIndex].name, ts->parms[maxConflictIndex].name);
	rtnVal = CMD_PARAMCONFLICT;
      }
      else if (cmd_param_CheckItemsValid(tparm)) {
	rtnVal = CMD_PARAM_VALIDATION_ERROR;
      }
    }
    
    if (((rtnVal != 0) || (interactionRequested)) &&
	((ts->allowInteractive) && TTY_CONNECTED)) {
      /* we want and allow interactive mode */

      /*
       *   Start from the parameter slot that caused the error in previous for loop ,
       * if there is an error-causing slot.
       */
      rtnVal = cmd_ProcessInteraction(ts, ((rtnVal == 0) ? 0 : i - 1));
    }
    else if (interactionRequested) {
      /* the user requested interaction, but that request could not be granted */
      if (TTY_CONNECTED) {
	/* the sub-command does not allow interactive mode */
	dce_svc_printf(CMC_S_CM_NO_INTERACTIVE_MSG);
      }

      rtnVal = CMD_NO_INTERACTIVE_MODE_AVAILABLE;
    }
    
    if (rtnVal != 0) {
      ResetSyntax(ts);
      return rtnVal;
    }
    
    /* Now, we just call the procedure and return */
    if (beforeProc)
	i = (*beforeProc)(ts, beforeRock);
    else
	i = 0;
    if (i) {
	ResetSyntax(ts);
	return(i);
    }
    i=(*ts->proc) (ts, ts->rock);
    if (afterProc) (*afterProc)(ts, afterRock);
    ResetSyntax(ts);	/* Reset and free things */
    return(i);
}

/* free token list returned by parseLine */
static FreeTokens(alist)
    register struct cmd_token *alist; {
    register struct cmd_token *nlist;
    for(; alist; alist = nlist) {
	nlist = alist->next;
	free(alist->key);
	free(alist);
    }
    return 0;
}

/* free an argv list returned by parseline */
cmd_FreeArgv(argv)
register char **argv; {
    register char *tp;
    for(tp = *argv; tp; argv++, tp = *argv)
	free(tp);
    return 0;
}

/* copy back the arg list to the argv array, freeing the cmd_tokens as you go; the actual
    data is still malloc'd, and will be freed when the caller calls cmd_FreeArgv
    later on */
#define INITSTR ""
static CopyBackArgs(alist, argv, an, amaxn)
register struct cmd_token *alist;
register char **argv;
long amaxn;
long *an; {
    register struct cmd_token *next;
    long count;

    count = 0;
    if (amaxn <= 1) return CMD_TOOMANY;
    *argv = (char *) malloc(strlen(INITSTR)+1);
    strcpy(*argv, INITSTR);
    amaxn--;
    argv++;
    count++;
    while (alist) {
	if (amaxn <= 1) return CMD_TOOMANY; /* argv is too small for his many parms. */
	*argv = alist->key;
	next = alist->next;
	free(alist);
	alist = next;
	amaxn--;
	argv++;
	count++;
    }
    *(argv++) =	0;	/* use last slot for terminating null */
    /* don't count terminating null */
    *an = count;
    return 0;
}

static quote(x)
register int x; {
    if (x == '"' || x == 39 /* single quote */) return 1;
    else return 0;
}

static space(x)
register int x; {
    if (x == 0 || x == ' ' || x == '\t' || x== '\n') return 1;
    else return 0;
}

cmd_ParseLine(aline, argv, an, amaxn)
char **argv;
long *an;
long amaxn;
char *aline; {
    char tbuffer[256];
    register char *tptr;
    int inToken, inQuote;
    struct cmd_token *first, *last;
    register struct cmd_token *ttok;
    register int tc;
    
    inToken = 0;	/* not copying token chars at start */
    first = (struct cmd_token *) 0;
    last = (struct cmd_token *) 0;
    inQuote=0;		/* not in a quoted string */
    while (1) {
	tc = *aline++;
	if (tc == 0 || (!inQuote && space(tc))) {    /* terminating null gets us in here, too */
	    if (inToken) {
		inToken	= 0;	/* end of this token */
		*tptr++ = 0;
		ttok = (struct cmd_token *) malloc(sizeof(struct cmd_token));
		ttok->next = (struct cmd_token *) 0;
		ttok->key = (char *) malloc(strlen(tbuffer)+1);
		strcpy(ttok->key, tbuffer);
		if (last) {
		    last->next = ttok;
		    last = ttok;
		}
		else last = ttok;
		if (!first) first = ttok;
	    }
	}
	else {
	    /* an alpha character */
	    if (!inToken) {
		tptr = tbuffer;
		inToken = 1;
	    }
	    if (tptr - tbuffer >= sizeof(tbuffer)) {
		FreeTokens(first);
		return CMD_TOOBIG;   /* token too long */
	    }
	    if (quote(tc)) {
		/* hit a quote, toggle inQuote flag but don't insert character */
		inQuote = !inQuote;
	    }
	    else {
		/* insert character */
		*tptr++ = tc;
	    }
	}
	if (tc == 0) {
	    /* last token flushed 'cause space(0) --> true */
	    if (last) last->next = (struct cmd_token *) 0;
	    return CopyBackArgs(first, argv, an, amaxn);
	}
    }
}
