%{
/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* 
 * HISTORY
 * $Log: testtran_lex.l,v $
 * Revision 1.1.11.1  1996/10/02  17:25:41  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:32:48  damon]
 *
 * Revision 1.1.6.1  1994/06/09  14:01:30  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:36:44  annie]
 * 
 * Revision 1.1.4.4  1993/01/19  15:30:17  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  13:51:10  cjd]
 * 
 * Revision 1.1.4.3  1992/11/24  16:26:24  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:24:29  bolinger]
 * 
 * Revision 1.1.4.2  1992/09/25  16:21:08  jaffe
 * 	Sync with Transarc by cleaned duplicate HISTORY and Log info
 * 	[1992/09/24  17:49:22  jaffe]
 * 
 * Revision 1.1.2.2  1992/05/05  18:25:27  jdp
 * 	delta mason-tran-0033 1.11
 * 	[1992/05/04  19:30:12  jdp]
 * 
 * $EndLog$
 */
/*
 * testtran_lex.l a lexical analyzer for the transaction mgr test routines
 *
 * N.B.  This lexer is not lex conformant.  It requires flex to work properly
 * for the inclusion support!
 *
 * (C) Copyright 1992 Transarc Corporation
 * All Rights Reserved.
 */

/*
 * we have our own version of yywrap!
 */
#undef yywrap 

#include <dcedfs/param.h>
#include <dcedfs/episode/lwp.h>
#include "testtran_gram.h"

/* RCS("$Header: /u0/rcs_trees/dce/rcs/file/episode/logbuf/testtran_lex.l,v 1.1.11.1 1996/10/02 17:25:41 damon Exp $") */

#define NEWSTR(os,ns) \
do {  \
    unsigned len = strlen(os); \
    (ns) = (char *)osi_Alloc(len+1); \
    strcpy(ns,os); \
} while(0)

#define FREESTR(str) \
do { \
    unsigned len = strlen(str); \
    osi_Free(str, len); \
} while(0)

unsigned lineNumber = 1;

%}

integer (-)?[0-9]+
ws [ \t]+
comment #[^\n]*

%%
start      { return(START); }
end        { return(END);   }
quit       { exit(0);       }
[a-zA-Z\/\.][-a-zA-Z\/\.0-9]*  { NEWSTR(yytext, yylval.str); return(STRING); }
{integer}  { yylval.val = atoi(yytext);  return(INTEGER); }
{comment}  ;
\n         { lineNumber++; return('\n'); }
.          { return yytext[0]; }
%%

struct fileStack {
    YY_BUFFER_STATE bState;
    struct fileStack *nextFile;
    unsigned lineNumber;
    char *fileName;
} *fileStack;

extern char *filename;

long includeFile(fName)
  {
      struct fileStack *fsP;

      fsP = (struct fileStack *)osi_Alloc(sizeof(struct fileStack));
      
      fsP->bState = YY_CURRENT_BUFFER;
      fsP->nextFile = fileStack;
      fsP->lineNumber = lineNumber;
      fsP->fileName = filename;
      fileStack = fsP;
      lineNumber = 1;
      NEWSTR(fName, filename);
      
      yyin = fopen(fName, "r");
      if(!yyin) {
	  fprintf(stderr, "cannot open file %s\n",fName);
	  exit(1);
      }
      yy_switch_to_buffer(yy_create_buffer(yyin,YY_BUF_SIZE));
      return 0;
  }

#undef yywrap

int yywrap()
  {
      struct fileStack *fsP;
      YY_BUFFER_STATE state;

      if (!fileStack) {
	  yyterminate(); /* really hit EOF */
      }
      else {
	  /*
	   * delete the current buffer state
	   */
	  yy_delete_buffer(YY_CURRENT_BUFFER);

	  /*
	   * switch to the including buffer state
	   */
	  fsP = fileStack;
	  fileStack = fsP->nextFile;
	  state = fsP->bState;
	  lineNumber = fsP->lineNumber;
	  FREESTR(fsP->fileName);
	  osi_Free((char *)fsP, sizeof(struct fileStack));
	  yy_switch_to_buffer(state);
	  return 0;
      }
  }
