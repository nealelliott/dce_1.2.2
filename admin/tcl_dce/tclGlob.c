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
 * $Log: tclGlob.c,v $
 * Revision 1.1.8.3  1996/02/18  19:18:51  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:04:24  marty]
 *
 * Revision 1.1.8.2  1995/12/08  21:34:21  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/04/28  20:26 UTC  dnguyen
 * 	Merge cleaning up of I18N-commented code
 * 
 * 	HP revision /main/dnguyen_mothra/1  1995/04/11  16:52 UTC  dnguyen
 * 	Clean up I18N comments
 * 	[1995/12/08  18:05:48  root]
 * 
 * Revision 1.1.4.1  1994/10/21  21:39:04  mori_m
 * 	CR 11259: Tcl I18N work.  Added wchar_t data type handling logic.
 * 	[1994/10/21  21:31:35  mori_m]
 * 
 * Revision 1.1.2.1  1994/07/12  21:19:13  mori_m
 * 	CR 11259:  First drop for Tcl I18N (messaging work).
 * 	[1994/07/12  21:10:49  mori_m]
 * 
 * $EndLog$
 */
/* 
 * tclGlob.c --
 *
 *	This file provides procedures and commands for file name
 *	manipulation, such as tilde expansion and globbing.
 *
 * Copyright (c) 1990-1993 The Regents of the University of California.
 * All rights reserved.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 * 
 * IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF
 * CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 */

#include "tclInt.h"
#include "tclWInt.h"
#include "tclUnix.h"
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dcetclmsg.h>

/*
 * The structure below is used to keep track of a globbing result
 * being built up (i.e. a partial list of file names).  The list
 * grows dynamically to be as big as needed.
 */

typedef struct {
    char *result;		/* Pointer to result area. */
    int totalSpace;		/* Total number of characters allocated
				 * for result. */
    int spaceUsed;		/* Number of characters currently in use
				 * to hold the partial result (not including
				 * the terminating NULL). */
    int dynamic;		/* 0 means result is static space, 1 means
				 * it's dynamic. */
} GlobResult;

/*
 * Declarations for procedures local to this file:
 */

static int		DoGlob _ANSI_ARGS_((Tcl_Interp *interp, char *dir,
			    char *rem));
static int		_WDoGlob _ANSI_ARGS_((Tcl_Interp *interp, char *dir,
			    wchar_t *rem));

/*
 *----------------------------------------------------------------------
 *
 * DoGlob --
 *
 *	This recursive procedure forms the heart of the globbing
 *	code.  It performs a depth-first traversal of the tree
 *	given by the path name to be globbed.
 *
 * Results:
 *	The return value is a standard Tcl result indicating whether
 *	an error occurred in globbing.  After a normal return the
 *	result in interp will be set to hold all of the file names
 *	given by the dir and rem arguments.  After an error the
 *	result in interp will hold an error message.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

static int
DoGlob(interp, dir, rem)
    Tcl_Interp *interp;			/* Interpreter to use for error
					 * reporting (e.g. unmatched brace). */
    char *dir;				/* Name of a directory at which to
					 * start glob expansion.  This name
					 * is fixed: it doesn't contain any
					 * globbing chars. */
    char *rem;				/* Path to glob-expand. */
{
    /*
     * When this procedure is entered, the name to be globbed may
     * already have been partly expanded by ancestor invocations of
     * DoGlob.  The part that's already been expanded is in "dir"
     * (this may initially be empty), and the part still to expand
     * is in "rem".  This procedure expands "rem" one level, making
     * recursive calls to itself if there's still more stuff left
     * in the remainder.
     */

    Tcl_DString newName;		/* Holds new name consisting of
					 * dir plus the first part of rem. */
    register char *p;
    register char c;
    char *openBrace, *closeBrace, *name, *dirName;
    int gotSpecial, baseLength;
    int result = TCL_OK;
    struct stat statBuf;
    unsigned char *i18n_msg_buf;
    unsigned char *i18n_msg_format;
    char *temp_posix_err;

    /*
     * Make sure that the directory part of the name really is a
     * directory.  If the directory name is "", use the name "."
     * instead, because some UNIX systems don't treat "" like "."
     * automatically. Keep the "" for use in generating file names,
     * otherwise "glob foo.c" would return "./foo.c".
     */

    if (*dir == '\0') {
	dirName = ".";
    } else {
	dirName = dir;
    }
    if ((stat(dirName, &statBuf) != 0) || !S_ISDIR(statBuf.st_mode)) {
	return TCL_OK;
    }
    Tcl_DStringInit(&newName);

    /*
     * First, find the end of the next element in rem, checking
     * along the way for special globbing characters.
     */

    gotSpecial = 0;
    openBrace = closeBrace = NULL;
    for (p = rem; ; p++) {
	c = *p;
	if ((c == '\0') || ((openBrace == NULL) && (c == '/'))) {
	    break;
	}
	if ((c == '{') && (openBrace == NULL)) {
	    openBrace = p;
	}
	if ((c == '}') && (openBrace != NULL) && (closeBrace == NULL)) {
	    closeBrace = p;
	}
	if ((c == '*') || (c == '[') || (c == '\\') || (c == '?')) {
	    gotSpecial = 1;
	}
    }

    /*
     * If there is an open brace in the argument, then make a recursive
     * call for each element between the braces.  In this case, the
     * recursive call to DoGlob uses the same "dir" that we got.
     * If there are several brace-pairs in a single name, we just handle
     * one here, and the others will be handled in recursive calls.
     */

    if (openBrace != NULL) {
	char *element;

	if (closeBrace == NULL) {
	    Tcl_ResetResult(interp);
	    interp->result = (char *)dce_msg_get(tcl_s_glob_unmatched_open_brace);
	    interp->freeProc = (Tcl_FreeProc *) free;

	    result = TCL_ERROR;
	    goto done;
	}
	Tcl_DStringAppend(&newName, rem, openBrace-rem);
	baseLength = newName.length;
	for (p = openBrace; *p != '}'; ) {
	    element = p+1;
	    for (p = element; ((*p != '}') && (*p != ',')); p++) {
		/* Empty loop body. */
	    }
	    Tcl_DStringAppend(&newName, element, p-element);
	    Tcl_DStringAppend(&newName, closeBrace+1, -1);
	    result = DoGlob(interp, dir, newName.string);
	    if (result != TCL_OK) {
		goto done;
	    }
	    newName.length = baseLength;
	}
	goto done;
    }

    /*
     * Start building up the next-level name with dir plus a slash if
     * needed to separate it from the next file name.
     */

    Tcl_DStringAppend(&newName, dir, -1);
    if ((dir[0] != 0) && (newName.string[newName.length-1] != '/')) {
	Tcl_DStringAppend(&newName, "/", 1);
    }
    baseLength = newName.length;

    /*
     * If there were any pattern-matching characters, then scan through
     * the directory to find all the matching names.
     */

    if (gotSpecial) {
	DIR *d;
	struct dirent *entryPtr;
	char savedChar;

	d = opendir(dirName);
	if (d == NULL) {
	    Tcl_ResetResult(interp);
	    i18n_msg_format = dce_msg_get(tcl_s_glob_dir_unreadable);
	    temp_posix_err = Tcl_PosixError(interp);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(dirName)
				+ strlen(temp_posix_err)
				+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, dirName, temp_posix_err);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    result = TCL_ERROR;
	    goto done;
	}

	/*
	 * Temporarily store a null into rem so that the pattern string
	 * is now null-terminated.
	 */

	savedChar = *p;
	*p = 0;

	while (1) {
	    entryPtr = readdir(d);
	    if (entryPtr == NULL) {
		break;
	    }

	    /*
	     * Don't match names starting with "." unless the "." is
	     * present in the pattern.
	     */

	    if ((*entryPtr->d_name == '.') && (*rem != '.')) {
		continue;
	    }
	    if (Tcl_StringMatch(entryPtr->d_name, rem)) {
		newName.length = baseLength;
		Tcl_DStringAppend(&newName, entryPtr->d_name, -1);
		if (savedChar == 0) {
		    Tcl_AppendElement(interp, newName.string);
		} else {
		    result = DoGlob(interp, newName.string, p+1);
		    if (result != TCL_OK) {
			break;
		    }
		}
	    }
	}
	closedir(d);
	*p = savedChar;
	goto done;
    }

    /*
     * The current element is a simple one with no fancy features.  Add
     * it to the new name.  If there are more elements still to come,
     * then recurse to process them.
     */

    Tcl_DStringAppend(&newName, rem, p-rem);
    if (*p != 0) {
	result = DoGlob(interp, newName.string, p+1);
	goto done;
    }

    /*
     * There are no more elements in the pattern.  Check to be sure the
     * file actually exists, then add its name to the list being formed
     * in interp-result.
     */

    name = newName.string;
    if (*name == 0) {
	name = ".";
    }
    if (access(name, F_OK) != 0) {
	goto done;
    }
    Tcl_AppendElement(interp, name);

    done:
    Tcl_DStringFree(&newName);
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_TildeSubst --
 *
 *	Given a name starting with a tilde, produce a name where
 *	the tilde and following characters have been replaced by
 *	the home directory location for the named user.
 *
 * Results:
 *	The result is a pointer to a static string containing
 *	the new name.  If there was an error in processing the
 *	tilde, then an error message is left in interp->result
 *	and the return value is NULL.  The result may be stored
 *	in bufferPtr; the caller must call Tcl_DStringFree(bufferPtr)
 *	to free the name.
 *
 * Side effects:
 *	Information may be left in bufferPtr.
 *
 *----------------------------------------------------------------------
 */

char *
Tcl_TildeSubst(interp, name, bufferPtr)
    Tcl_Interp *interp;		/* Interpreter in which to store error
				 * message (if necessary). */
    char *name;			/* File name, which may begin with "~/"
				 * (to indicate current user's home directory)
				 * or "~<user>/" (to indicate any user's
				 * home directory). */
    Tcl_DString *bufferPtr;	/* May be used to hold result.  Must not hold
				 * anything at the time of the call, and need
				 * not even be initialized. */
{
    char *dir;
    register char *p;
    unsigned char *i18n_msg_buf;
    unsigned char *i18n_msg_format;

    Tcl_DStringInit(bufferPtr);
    if (name[0] != '~') {
	return name;
    }

    if ((name[1] == '/') || (name[1] == '\0')) {
	dir = getenv("HOME");
	if (dir == NULL) {
	    Tcl_ResetResult(interp);
	    i18n_msg_format = dce_msg_get(tcl_s_glob_no_home_env);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(name)
				+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, name);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return NULL;
	}
	Tcl_DStringAppend(bufferPtr, dir, -1);
	Tcl_DStringAppend(bufferPtr, name+1, -1);
    } else {
	struct passwd *pwPtr;

	for (p = &name[1]; (*p != 0) && (*p != '/'); p++) {
	    /* Null body;  just find end of name. */
	}
	Tcl_DStringAppend(bufferPtr, name+1, p - (name+1));
	pwPtr = getpwnam(bufferPtr->string);
	if (pwPtr == NULL) {
	    endpwent();
	    Tcl_ResetResult(interp);
	    i18n_msg_format = dce_msg_get(tcl_s_glob_no_user);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(bufferPtr->string)
				+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, bufferPtr->string);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return NULL;
	}
	Tcl_DStringFree(bufferPtr);
	Tcl_DStringAppend(bufferPtr, pwPtr->pw_dir, -1);
	Tcl_DStringAppend(bufferPtr, p, -1);
	endpwent();
    }
    return bufferPtr->string;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_GlobCmd --
 *
 *	This procedure is invoked to process the "glob" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_GlobCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
    int i, result, noComplain, firstArg;
    unsigned char *i18n_msg_buf;
    unsigned char *i18n_msg_format;

    if (argc < 2) {
	notEnoughArgs:
	i18n_msg_format = dce_msg_get(tcl_s_glob_wnum_args_switch_name);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
				+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    noComplain = 0;
    for (firstArg = 1; (firstArg < argc) && (argv[firstArg][0] == '-');
	    firstArg++) {
	if (strcmp(argv[firstArg], "-nocomplain") == 0) {
	    noComplain = 1;
	} else if (strcmp(argv[firstArg], "--") == 0) {
	    firstArg++;
	    break;
	} else {
	    i18n_msg_format = dce_msg_get(tcl_s_glob_bad_switch);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[firstArg])
				+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[firstArg]);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
    }
    if (firstArg >= argc) {
	goto notEnoughArgs;
    }

    for (i = firstArg; i < argc; i++) {
	char *thisName;
	Tcl_DString buffer;

	thisName = Tcl_TildeSubst(interp, argv[i], &buffer);
	if (thisName == NULL) {
	    return TCL_ERROR;
	}
	if (*thisName == '/') {
	    if (thisName[1] == '/') {
		/*
		 * This is a special hack for systems like those from Apollo
		 * where there is a super-root at "//":  need to treat the
		 * double-slash as a single name.
		 */
		result = DoGlob(interp, "//", thisName+2);
	    } else {
		result = DoGlob(interp, "/", thisName+1);
	    }
	} else {
	    result = DoGlob(interp, "", thisName);
	}
	Tcl_DStringFree(&buffer);
	if (result != TCL_OK) {
	    return result;
	}
    }
    if ((*interp->result == 0) && !noComplain) {
	char *sep = "";

	if (argc == 2)
	{
		i18n_msg_format = dce_msg_get(tcl_s_glob_no_files_match_single);
		Tcl_AppendResult(interp, (char *)i18n_msg_format, (char *) NULL);
		free(i18n_msg_format);
	} else
	{
		i18n_msg_format = dce_msg_get(tcl_s_glob_no_files_match_plural);
		Tcl_AppendResult(interp, (char *)i18n_msg_format, (char *) NULL);
		free(i18n_msg_format);
	}

	for (i = firstArg; i < argc; i++) {
	    Tcl_AppendResult(interp, sep, argv[i], (char *) NULL);
	    sep = " ";
	}
	Tcl_AppendResult(interp, "\"", (char *) NULL);
	return TCL_ERROR;
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WTildeSubst --
 *
 *	Given a name starting with a tilde, produce a name where
 *	the tilde and following characters have been replaced by
 *	the home directory location for the named user.
 *
 * Results:
 *	The result is a pointer to a static string containing
 *	the new name.  If there was an error in processing the
 *	tilde, then an error message is left in interp->result
 *	and the return value is NULL.  The result may be stored
 *	in bufferPtr; the caller must call Tcl_DStringFree(bufferPtr)
 *	to free the name.
 *
 * Side effects:
 *	Information may be left in bufferPtr.
 *
 *----------------------------------------------------------------------
 */

char *
_Tcl_WTildeSubst(interp, name, bufferPtr)
    Tcl_Interp *interp;		/* Interpreter in which to store error
				 * message (if necessary). */
    char *name;			/* File name, which may begin with "~/"
				 * (to indicate current user's home directory)
				 * or "~<user>/" (to indicate any user's
				 * home directory). */
    Tcl_DString *bufferPtr;	/* May be used to hold result.  Must not hold
				 * anything at the time of the call, and need
				 * not even be initialized. */
{
    char *dir;
    register char *p;
    unsigned char *i18n_msg_buf, *i18n_msg_format;
    wchar_t *wname;
    size_t wlen, st;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return NULL;
    }

    _Multibyte_to_WChar(interp, wlen, name, wname, st)
    if (st == (size_t)-1)
    {
	return NULL;
    }

    Tcl_DStringInit(bufferPtr);
    if (wname[0] != i18n_special_chars.w_tilde) {
	return name;
    }

    if ((wname[1] == i18n_special_chars.w_slash) 
		|| (wname[1] == i18n_special_chars.w_esc_zero)) {
	dir = getenv("HOME");
	if (dir == NULL) {
	    Tcl_ResetResult(interp);
	    i18n_msg_format = dce_msg_get(tcl_s_glob_no_home_env);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(name)
				+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, name);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    ckfree(wname);
	    return NULL;
	}
	Tcl_DStringAppend(bufferPtr, dir, -1);
	Tcl_DStringAppend(bufferPtr, name+1, -1);
    } else {
	struct passwd *pwPtr;

	/* 
	 * Since '/' is not typically included in a part of multibyte
	 * characters, assume this routine is multibyte safe 
	 */
	for (p = &name[1]; (*p != 0) && (*p != '/'); p++) {
	    /* Null body;  just find end of name. */
	}
	Tcl_DStringAppend(bufferPtr, name+1, p - (name+1));
	pwPtr = getpwnam(bufferPtr->string);
	if (pwPtr == NULL) {
	    endpwent();
	    Tcl_ResetResult(interp);
	    i18n_msg_format = dce_msg_get(tcl_s_glob_no_user);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(bufferPtr->string)
				+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, bufferPtr->string);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    ckfree(wname);
	    return NULL;
	}
	Tcl_DStringFree(bufferPtr);
	Tcl_DStringAppend(bufferPtr, pwPtr->pw_dir, -1);
	Tcl_DStringAppend(bufferPtr, p, -1);
	endpwent();
    }
    ckfree(wname);
    return bufferPtr->string;
}

/*
 *----------------------------------------------------------------------
 *
 * _WDoGlob --
 *
 *	This recursive procedure forms the heart of the globbing
 *	code.  It performs a depth-first traversal of the tree
 *	given by the path name to be globbed.
 *
 * Results:
 *	The return value is a standard Tcl result indicating whether
 *	an error occurred in globbing.  After a normal return the
 *	result in interp will be set to hold all of the file names
 *	given by the dir and rem arguments.  After an error the
 *	result in interp will hold an error message.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

static int
_WDoGlob(interp, dir, wrem)
    Tcl_Interp *interp;			/* Interpreter to use for error
					 * reporting (e.g. unmatched brace). */
    char *dir;				/* Name of a directory at which to
					 * start glob expansion.  This name
					 * is fixed: it doesn't contain any
					 * globbing chars. */
    wchar_t *wrem;			/* Path to glob-expand. */
{
    /*
     * When this procedure is entered, the name to be globbed may
     * already have been partly expanded by ancestor invocations of
     * DoGlob.  The part that's already been expanded is in "dir"
     * (this may initially be empty), and the part still to expand
     * is in "rem".  This procedure expands "rem" one level, making
     * recursive calls to itself if there's still more stuff left
     * in the remainder.
     */

    Tcl_DString newName;		/* Holds new name consisting of
					 * dir plus the first part of rem. */
    register wchar_t *wp;
    register wchar_t wc;
    wchar_t *wopenBrace, *wcloseBrace, *wtemp;
    char *name, *dirName, *ctemp;
    int gotSpecial, baseLength;
    int result = TCL_OK;
    struct stat statBuf;
    unsigned char *i18n_msg_buf;
    unsigned char *i18n_msg_format;
    char *temp_posix_err;
    size_t  clen, st;

    /*
     * Make sure that the directory part of the name really is a
     * directory.  If the directory name is "", use the name "."
     * instead, because some UNIX systems don't treat "" like "."
     * automatically. Keep the "" for use in generating file names,
     * otherwise "glob foo.c" would return "./foo.c".
     */

    if (*dir == '\0') 
    {
	dirName = ".";
    } 
    else 
    {
	dirName = dir;
    }
    if ((stat(dirName, &statBuf) != 0) || !S_ISDIR(statBuf.st_mode)) 
    {
	return TCL_OK;
    }
    Tcl_DStringInit(&newName);

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return TCL_CONV_ERROR;
    }

    /*
     * First, find the end of the next element in rem, checking
     * along the way for special globbing characters.
     */

    gotSpecial = 0;
    wopenBrace = wcloseBrace = (wchar_t *)NULL;
    for (wp = wrem; ; wp++) 
    {
	wc = *wp;
	if ((wc == i18n_special_chars.w_esc_zero) 
		|| ((wopenBrace == (wchar_t *)NULL) 
		&& (wc == i18n_special_chars.w_slash))) 
	{
	    break;
	}
	if ((wc == i18n_special_chars.w_op_cbra) 
		&& (wopenBrace == (wchar_t *)NULL)) 
	{
	    wopenBrace = wp;
	}
	if ((wc == i18n_special_chars.w_cl_cbra) 
		&& (wopenBrace != (wchar_t *)NULL) 
		&& (wcloseBrace == (wchar_t *)NULL)) 
	{
	   wcloseBrace = wp; 
	}
	if ((wc == i18n_special_chars.w_star) 
		|| (wc == i18n_special_chars.w_op_sbr) 
		|| (wc == i18n_special_chars.w_bsla) 
		|| (wc == i18n_special_chars.w_question)) 
	{
	    gotSpecial = 1;
	}
    }

    /*
     * If there is an open brace in the argument, then make a recursive
     * call for each element between the braces.  In this case, the
     * recursive call to DoGlob uses the same "dir" that we got.
     * If there are several brace-pairs in a single name, we just handle
     * one here, and the others will be handled in recursive calls.
     */

    if (wopenBrace != (wchar_t *)NULL) 
    {
	wchar_t *welement;

	if (wcloseBrace == (wchar_t *)NULL) 
	{
	    Tcl_ResetResult(interp);
	    interp->result = (char *)dce_msg_get(tcl_s_glob_unmatched_open_brace);
	    interp->freeProc = (Tcl_FreeProc *) free;

	    result = TCL_ERROR;
	    goto done;
	}

	_WChar_To_Multibyte(interp, clen, wrem, ctemp, st)
	if (st == (size_t)-1)
	{
	    result = TCL_CONV_ERROR;
	    goto done;
	}

	Tcl_DStringAppend(&newName, ctemp, wopenBrace - wrem);
	ckfree(ctemp);
	baseLength = newName.length;
	for (wp = wopenBrace; *wp != i18n_special_chars.w_cl_cbra; ) 
	{
	    welement = wp+1;
	    for (wp = welement; ((*wp != i18n_special_chars.w_cl_cbra) 
			&& (*wp != i18n_special_chars.w_comma)); wp++) 
	    {
		/* Empty loop body. */
	    }

	    _WChar_To_Multibyte(interp, clen, welement, ctemp, st)
	    if (st == (size_t)-1)
	    {
		result = TCL_CONV_ERROR;
	        goto done;
	    }
	    Tcl_DStringAppend(&newName, ctemp, wp - welement);
	    ckfree(ctemp);

	    _WChar_To_Multibyte(interp, clen, wcloseBrace+1, ctemp, st)
	    if (st == (size_t)-1)
	    {
		result = TCL_CONV_ERROR;
	        goto done;
	    }
	    Tcl_DStringAppend(&newName, ctemp, -1);
	    ckfree(ctemp);

	    _Multibyte_to_WChar(interp, clen, newName.string, wtemp, st)
	    if (st == (size_t)-1)
	    {
		result = TCL_CONV_ERROR;
	        goto done;
	    }
	    result = _WDoGlob(interp, dir, wtemp);
	    ckfree(wtemp);

	    if (result != TCL_OK) 
	    {
		goto done;
	    }
	    newName.length = baseLength;
	}
	goto done;
    }

    /*
     * Start building up the next-level name with dir plus a slash if
     * needed to separate it from the next file name.
     */

    Tcl_DStringAppend(&newName, dir, -1);
    if ((dir[0] != 0) && (newName.string[newName.length-1] != '/')) 
    {
	Tcl_DStringAppend(&newName, "/", 1);
    }
    baseLength = newName.length;

    /*
     * If there were any pattern-matching characters, then scan through
     * the directory to find all the matching names.
     */

    if (gotSpecial) 
    {
	DIR *d;
	struct dirent *entryPtr;
	wchar_t wsavedChar;

	d = opendir(dirName);
	if (d == NULL) {
	    Tcl_ResetResult(interp);
	    i18n_msg_format = dce_msg_get(tcl_s_glob_dir_unreadable);
	    temp_posix_err = Tcl_PosixError(interp);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(dirName)
				+ strlen(temp_posix_err)
				+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, dirName, temp_posix_err);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    result = TCL_ERROR;
	    goto done;
	}

	/*
	 * Temporarily store a null into rem so that the pattern string
	 * is now null-terminated.
	 */

	wsavedChar = *wp;
	*wp = i18n_special_chars.w_esc_zero;

	while (1) 
	{
	    entryPtr = readdir(d);
	    if (entryPtr == NULL) 
	    {
		break;
	    }

	    _Multibyte_to_WChar(interp, clen, entryPtr->d_name, wtemp, st)
	    if (st == (size_t)-1)
	    {
		result = TCL_CONV_ERROR;
		goto done;
	    }

	    /*
	     * Don't match names starting with "." unless the "." is
	     * present in the pattern.
	     */

	    if ((*wtemp == i18n_special_chars.w_period) 
			&& (*wrem != i18n_special_chars.w_period)) 
	    {
		ckfree(wtemp);
		continue;
	    }

	    result = _Tcl_WStringMatch(interp, wtemp, wrem);
	    ckfree(wtemp);

	    if (result == TCL_CONV_ERROR)
	    {
		result = TCL_CONV_ERROR;
		goto done;
	    }
	    if (result == 1)
	    {
		newName.length = baseLength;
		Tcl_DStringAppend(&newName, entryPtr->d_name, -1);
		if (wsavedChar == i18n_special_chars.w_esc_zero) 
		{
		    _Multibyte_to_WChar(interp, clen, newName.string, wtemp, st)
		    if (st == (size_t)-1)
		    {
			result = TCL_CONV_ERROR;
			goto done;
		    }
		    _Tcl_WAppendElement(interp, wtemp);
		    ckfree(wtemp);
		} 
		else 
		{
		    result = _WDoGlob(interp, newName.string, wp+1);
		    if (result != TCL_OK) 
		    {
			break;
		    }
		}
	    }
	}
	closedir(d);
	*wp = wsavedChar;
	goto done;
    }

    /*
     * The current element is a simple one with no fancy features.  Add
     * it to the new name.  If there are more elements still to come,
     * then recurse to process them.
     */

    _WChar_To_Multibyte(interp, clen, wrem, ctemp, st)
    if (st == (size_t)-1)
    {
	result = TCL_CONV_ERROR;
	goto done;
    }

    Tcl_DStringAppend(&newName, ctemp, wp - wrem);
    ckfree(ctemp);

    if (*wp != i18n_special_chars.w_esc_zero) 
    {
	result = _WDoGlob(interp, newName.string, wp+1);
	goto done;
    }

    /*
     * There are no more elements in the pattern.  Check to be sure the
     * file actually exists, then add its name to the list being formed
     * in interp-result.
     */

    name = newName.string;
    if (*name == 0) {
	name = ".";
    }
    if (access(name, F_OK) != 0) {
	goto done;
    }

    _Multibyte_to_WChar(interp, clen, name, wtemp, st)
    if (st == (size_t)-1)
    {
	result = TCL_CONV_ERROR;
	goto done;
    }
    _Tcl_WAppendElement(interp, wtemp);
    ckfree(wtemp);

    done:
    Tcl_DStringFree(&newName);
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WGlobCmd --
 *
 *	This procedure is invoked to process the "glob" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
_Tcl_WGlobCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    int i, result, noComplain, firstArg;
    unsigned char *i18n_msg_buf, *i18n_msg_format;
    char *ctemp;
    wchar_t *wtemp;
    size_t len, st;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return TCL_ERROR;
    }

    if (argc < 2) 
    {
	notEnoughArgs:
	i18n_msg_format = dce_msg_get(tcl_s_glob_wnum_args_switch_name);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("glob")
				+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "glob");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    noComplain = 0;
    for (firstArg = 1; (firstArg < argc) 
	    && (wargv[firstArg][0] == i18n_special_chars.w_minus); firstArg++) 
    {
	_WChar_To_Multibyte(interp, len, wargv[firstArg], ctemp, st)
	if (st == (size_t)-1)
	    return TCL_ERROR;

	if (strcmp(ctemp, "-nocomplain") == 0) 
	{
	    noComplain = 1;
	} 
	else if (strcmp(ctemp, "--") == 0) 
	{
	    firstArg++;
	    break;
	} 
	else 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_glob_bad_switch);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(ctemp)
				+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, ctemp);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(ctemp);

	    return TCL_ERROR;
	}
	ckfree(ctemp);
    }
    if (firstArg >= argc) 
    {
	goto notEnoughArgs;
    }

    for (i = firstArg; i < argc; i++) 
    {
	char *thisName;
	Tcl_DString buffer;

	_WChar_To_Multibyte(interp, len, wargv[i], ctemp, st)
	if (st == (size_t)-1)
	    return TCL_ERROR;

	thisName = _Tcl_WTildeSubst(interp, ctemp, &buffer);
	if (thisName == NULL) 
	{
	    ckfree(ctemp);
	    return TCL_ERROR;
	}
	ckfree(ctemp);

	_Multibyte_to_WChar(interp, len, thisName, wtemp, st)
	if (st == (size_t)-1)
	    return TCL_ERROR;

	if (*wtemp == i18n_special_chars.w_slash) 
	{
	    if (wtemp[1] == i18n_special_chars.w_slash) 
	    {
		/*
		 * This is a special hack for systems like those from Apollo
		 * where there is a super-root at "//":  need to treat the
		 * double-slash as a single name.
		 */
		result = _WDoGlob(interp, "//", wtemp+2);
	    } 
	    else 
	    {
		result = _WDoGlob(interp, "/", wtemp+1);
	    }
	} 
	else 
	{
	    result = _WDoGlob(interp, "", wtemp);
	}
	ckfree(wtemp);

	Tcl_DStringFree(&buffer);
	if (result != TCL_OK) 
	{
	    return result;
	}
    }
    if ((*interp->result == 0) && !noComplain) 
    {
	char *sep = "";

	if (argc == 2)
	{
		i18n_msg_format = dce_msg_get(tcl_s_glob_no_files_match_single);
		Tcl_AppendResult(interp, (char *)i18n_msg_format, (char *) NULL);
		free(i18n_msg_format);
	} 
	else
	{
		i18n_msg_format = dce_msg_get(tcl_s_glob_no_files_match_plural);
		Tcl_AppendResult(interp, (char *)i18n_msg_format, (char *) NULL);
		free(i18n_msg_format);
	}

	for (i = firstArg; i < argc; i++) 
	{
	    _WChar_To_Multibyte(interp, len, wargv[i], ctemp, st)
	    if (st == (size_t)-1)
		return TCL_ERROR;

	    Tcl_AppendResult(interp, sep, ctemp, (char *) NULL);
	    sep = " ";
	    ckfree(ctemp);
	}
	Tcl_AppendResult(interp, "\"", (char *) NULL);
	return TCL_ERROR;
    }
    return TCL_OK;
}
