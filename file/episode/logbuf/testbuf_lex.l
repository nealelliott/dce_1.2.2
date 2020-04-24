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
 * $Log: testbuf_lex.l,v $
 * Revision 1.1.11.1  1996/10/02  17:25:38  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:32:46  damon]
 *
 * Revision 1.1.6.1  1994/06/09  14:01:27  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:36:43  annie]
 * 
 * Revision 1.1.4.6  1993/01/19  15:30:10  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  13:50:58  cjd]
 * 
 * Revision 1.1.4.5  1992/12/09  20:04:20  jaffe
 * 	Transarc delta: mason-ot5976-defer-io-during-recovery 1.2
 * 	  Selected comments:
 * 	    The NBS changes meant that there could be many pages which didn't really need
 * 	    I/O.  The existing simplistic algorithm was merrily reading and writing those
 * 	    pages even though no data on the pages was modified.  To fix this, we merely
 * 	    defer the I/O until we find a transaction really needing to write on the data
 * 	    page; once found we keep it around.  At the end we check to see if we ever
 * 	    read it.  If so, we write it back to disk. If not, we just go to the next
 * 	    modified disk block.
 * 	    This first delta is a checkpoint.
 * 	    Add NBS record for undo-zero
 * 	    Complete work here.  This version completes the delta.  Add several
 * 	    regression tests and enhance the testing language to support better
 * 	    concurrency guarantees.
 * 	    Modified language slightly, added new keywords.
 * 	[1992/12/03  22:12:59  jaffe]
 * 
 * Revision 1.1.4.4  1992/11/24  16:26:17  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:24:18  bolinger]
 * 
 * Revision 1.1.4.3  1992/09/15  17:38:19  jaffe
 * 	Transarc delta: jdp-epimerge-1.0-to-1.0.1b9-into-2.17 1.20
 * 	  Selected comments:
 * 	    This revision of this delta contains changes made to episode/async by the OSF
 * 	    between dce1.0 and dce1.0.1b9.  Subsequent revisions will contain merges
 * 	    for other episode subdirectories.
 * 	    This revision of this delta contains changes made to episode/dir by the OSF
 * 	    This revision of this delta contains changes made to episode/lobguf by the OSF
 * 	    Merged in changes made by the OSF between dce1.0 and dce1.0.1b9.
 * 	    This revision of this delta contains changes made to episode/salvage by the OSF
 * 	    This revision of this delta contains changes made to episode/sautils by the OSF
 * 	    This revision of this delta contains changes made to episode/tools by the OSF
 * 	    This revision of this delta contains changes made to episode/utils by the OSF
 * 	    This revision of this delta contains changes made to episode/vnops by the OSF
 * 	    between dce1.0 and dce1.0.1b9.
 * 	    This revision fixes some problems in the logbuf layer that occurred after
 * 	    the merge.
 * 	    This delta adds anode/epi_errs.msf.
 * 	    This delta fixes problems in the logbuf layer.
 * 	    This revision fixes some problems in the vnode layer.
 * 	    Fix more vnode layer problems.
 * 	    Add files that were introduced after DCE 1.0 to the .klib Makefiles.
 * 	    This delta fixes various problems that caused undefined references
 * 	    while building the AIX kernel extension.
 * 	    This revision fixes random problems encountered under OSF/1.
 * 	    This revision fixes some problems encountered when compiling test_vnodeops
 * 	    on OSF/1.
 * 	    This revision includes the OSF's changes to epimount.
 * 	Transarc delta: mason-ot3721-start-tran-not-required 1.6
 * 	  Selected comments:
 * 	    When a transaction has an end record, it should always be redone; I'm
 * 	    sure I've previously made this fix (long, long ago in a world far, far
 * 	    away).  This requires changing MarkCommitted in recovery and getting the
 * 	    regression test suite built again.  This is a CHECKPOINT only.
 * 	    Grab right version from RCS archives.
 * 	    Checkpoint first system which shows signs of really working.
 * 	    This checkpoint notes the first place where the regression test suite
 * 	    works again.  Additional work is required to verify that the original
 * 	    bug (as found by OTA) is really gone.
 * 	    This is another checkpoint.  I'm working on building the regression test.
 * 	    Modified to support new command & to print comments.
 * 	    Make changes in readlog so it is easier to use.
 * 	    Oops.  Calling wrong log record printer. [tony, not ted]
 * 	Transarc delta: mason-tran-0033 1.11
 * 	  Selected comments:
 * 	    Checkpoint for the year-end.
 * 	    Changes made earlier were lost - I carried them forward.
 * 	    Intermediate snapshot - single threaded code now working correctly.
 * 	    SHOULD NOT BE USED IN PRODUCTION.
 * 	    Moved lost changes from backup volume into main tree.
 * 	    Forced close to fix a different problem. This delta still does not work; do
 * 	    not use in production!
 * 	    Snapshot - passes all_tests.
 * 	    More work - now fixing bugs.
 * 	    Final round of testing; appears to be production quality now.
 * 	    Update internal development records.
 * 	    Found single-threaded case in TranForce which would block.
 * 	    Macro removed in delta 1.9 was used in multiple files.
 * 	    Found race condition where tran was referenced before initialization.
 * 	[1992/09/14  20:57:19  jaffe]
 * 
 * Revision 1.1.4.2  1992/08/31  19:51:05  jaffe
 * 	Add missing Header
 * 	[1992/08/30  14:08:53  jaffe]
 * 
 * Revision 1.1.2.2  1992/05/05  18:24:46  jdp
 * 	delta jdp-epimerge-1.0-to-1.0.1b9-into-2.17 1.20
 * 	delta mason-tran-0033 1.11
 * 	[1992/05/05  15:40:50  jdp]
 * 
 * 	Added copyright and history
 * 	[1992/05/05  15:39:55  jdp]
 * 
 * 	$TALog: testbuf_lex.l,v $
 * 	Revision 4.15  1994/11/01  21:28:27  cfe
 * 	Bring over the changes that the OSF made in going from their DCE 1.0.3
 * 	release to their DCE 1.1 release.
 * 	[from r4.14 by delta cfe-db6109-merge-1.0.3-to-1.1-diffs, r1.1]
 * 
 * 	Revision 4.11  1992/11/17  16:02:55  mason
 * 	Complete work here.  This version completes the delta.  Add several
 * 	regression tests and enhance the testing language to support better
 * 	concurrency guarantees.
 * 
 * 	Modified language slightly, added new keywords.
 * 	[from revision 4.10 by delta mason-ot5976-defer-io-during-recovery, r1.2]
 * 
 * Revision 4.10  1992/11/17  00:07:12  mason
 * 	The NBS changes meant that there could be many pages which didn't really need
 * 	I/O.  The existing simplistic algorithm was merrily reading and writing those
 * 	pages even though no data on the pages was modified.  To fix this, we merely
 * 	defer the I/O until we find a transaction really needing to write on the data
 * 	page; once found we keep it around.  At the end we check to see if we ever
 * 	read it.  If so, we write it back to disk. If not, we just go to the next
 * 	modified disk block.
 * 
 * 	This first delta is a checkpoint.
 * 
 * 	Add NBS record for undo-zero
 * 	[from revision 4.9 by delta mason-ot5976-defer-io-during-recovery, r1.1]
 * 
 * Revision 4.9  1992/09/17  19:27:46  jaffe
 * 	Header cleanup in order to sync with the OSF.
 * 
 * 	[from revision 4.8 by delta osf-add-osf-copyrights, revision 1.1]
 * 
 * Revision 4.8  1992/08/06  20:39:54  mason
 * 	This is another checkpoint.  I'm working on building the regression test.
 * 
 * 	Modified to support new command & to print comments.
 * 	[from revision 4.7 by delta mason-ot3721-start-tran-not-required, revision 1.4]
 * 
 * Revision 4.7  1992/07/31  22:30:29  mason
 * 	When a transaction has an end record, it should always be redone; I'm
 * 	sure I've previously made this fix (long, long ago in a world far, far
 * 	away).  This requires changing MarkCommitted in recovery and getting the
 * 	regression test suite built again.  This is a CHECKPOINT only.
 * 
 * 	Grab right version from RCS archives.
 * 	[from revision 4.5 by delta mason-ot3721-start-tran-not-required, revision 1.1]
 * 
 * $EndLog$ 
 */
/* 
 * test_buf.l a lexical analyzer for the buffer package test routines
 *
 * N.B.  This lexer is not lex conformant.  It requires flex to work properly
 * for the inclusion support!
 */

/*
 * we have our own version of yywrap!
 */
#undef yywrap 

#include <dcedfs/param.h>
/* #include <dcedfs/osi.h> */
#include "testbuf_gram.h"

/* RCS("$Header: /u0/rcs_trees/dce/rcs/file/episode/logbuf/testbuf_lex.l,v 1.1.11.1 1996/10/02 17:25:38 damon Exp $") */

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
{ws}       ;
debug      { return(DEBUG); }
data       { return(DATA); }
data2      { return(DATA2); }
end        { return(END); }
rstrt      { return(RSTRT); }
start      { return(STARTTRAN); }
set        { return(SET); }
logsize    { return(LOGSIZE); }
rcvrdebug  { return(RECOVERDEBUG); }
logbufdebug {return(LOGBUFDEBUG); }
testdebug  { return(TESTDEBUG); }
createlog  { return(CREATELOG); }
openlog    { return(OPENLOG); }
opendata   { return(OPENDATA); }
closelog   { return(CLOSE); }
dumplog    { return(DUMPLOG); }
pageclear  { return(PAGECLEAR); }
pageset    { return(PAGESET); }
recover    { return(RECOVER); }
force      { return(FORCE); }
fill       { return(FILL); }
uzero      { return(UZERO); }
merge      { return(MERGE); }
include    { return(INCLUDE); }
echo       { return(COMMENT); }
compare    { return(COMPARE); }
ncompare   { return(NCOMPARE); }
quit       { return(QUIT); }
yaccdebug  { return(YACCDEBUG); }
sync       { return(SYNC); }
fsync      { return(SYNCF); }
[a-zA-Z\/\.][-a-zA-Z\/\.0-9]*  { NEWSTR(yytext, yylval.str); return(STRING); }
{integer}  { yylval.val = atoi(yytext);  return(INTEGER); }
{comment}  { ECHO; printf("\n"); /* comments have useful information */ }
0x[0-9a-zA-Z]+ { yylval.val = strtoul(yytext, 0, 16); return(CHARDATA); }
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
      extern FILE *yyin;

      fsP = (struct fileStack *)osi_Alloc(sizeof(struct fileStack));
      
      fsP->bState = YY_CURRENT_BUFFER;
      fsP->nextFile = fileStack;
      fsP->lineNumber = lineNumber;
      fsP->fileName = filename;
      fileStack = fsP;
      lineNumber = 1;
      NEWSTR(fName, filename);
      
      yyin = fopen((const char *)fName, "r");
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
