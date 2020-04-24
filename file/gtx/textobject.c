/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: textobject.c,v $
 * Revision 1.1.76.1  1996/10/02  17:51:37  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:40:25  damon]
 *
 * Revision 1.1.71.3  1994/08/03  16:15:56  ruby
 * 	Implement partial S12Y (Internationalization) for DFS commands and daemons
 * 	[1994/08/01  17:01:58  ruby]
 * 
 * Revision 1.1.71.2  1994/06/09  14:10:46  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:24:54  annie]
 * 
 * Revision 1.1.71.1  1994/02/04  20:20:43  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:14:03  devsrc]
 * 
 * Revision 1.1.69.1  1993/12/07  17:26:35  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  15:34:41  jaffe]
 * 
 * Revision 1.1.5.4  1993/01/19  16:04:20  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  14:14:13  cjd]
 * 
 * Revision 1.1.5.3  1992/11/24  17:52:53  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  18:06:24  bolinger]
 * 
 * Revision 1.1.5.2  1992/08/31  20:05:41  jaffe
 * 	Transarc delta: jaffe-ot4658-fix-compiler-warnings-in-102-1.42 1.1
 * 	  Selected comments:
 * 	    cleaned up compiler warnings.
 * 	    comment text after endif
 * 	[1992/08/30  02:49:33  jaffe]
 * 
 * Revision 1.1.3.2  1992/05/20  19:55:52  mason
 * 	Transarc delta: jess-correct-1.30-serror 1.1
 * 	  Files modified:
 * 	    cm: cm_dcache.c; gtx: objects.c, textcb.c, textobject.c
 * 	  Selected comments:
 * 	    To correct syntax errors
 * 	    extern long errno changed to extern int errno.
 * 	Transarc delta: vijay-ot3289-gtx-fix-lock-calls 1.2
 * 	  Files modified:
 * 	    gtx: Makefile, X11windows.c, dumbwindows.c, frame.c
 * 	    gtx: gtxtextcb.h, input.c, lightobject.c, objects.c
 * 	    gtx: textcb.c, textobject.c
 * 	  Selected comments:
 * 	    change the lock_ calls to afslk_ calls.
 * 	    fix a duplicate declaration
 * 	    remove a spurious cml diff line. This line was not deleted upon a merge
 * 	    and gave some compilations problems.
 * 	[1992/05/20  11:37:38  mason]
 * 
 * Revision 1.1  1992/01/19  02:41:50  devrcs
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

/*------------------------------------------------------------------------
 * textobject.c
 *
 * Description:
 *	Implementation of the gator text object.
 *
 * Author:
 *	Ed. Zayas
 *	Transarc Corporation
 *------------------------------------------------------------------------*/

#include <dcedfs/gtxtextobj.h>		/*Interface for this module*/
#include <dcedfs/gtxwindows.h>		/*Gator window interface*/
#include <dcedfs/gtxcurseswin.h>		/*Gator curses window interface*/
#include <dcedfs/gtxdumbwin.h>		/*Gator dumb terminal window interface*/
#include <dcedfs/gtxX11win.h>		/*Gator X11 window interface*/
#include <stdio.h>                      /*Standard I/O stuff*/
#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsgtxmac.h>
#include <dfsgtxsvc.h>
#include <dfsgtxmsg.h>

/*Externally-advertised array of text onode operations*/
struct onodeops gator_text_ops = {
    gator_text_destroy,
    gator_text_display,
    gator_text_release
};

static char mn[] = "gator_textobject";   /*Module name*/

#define GATOR_TEXTCB_DO_BOX	0

/*------------------------------------------------------------------------
 * gator_text_create
 *
 * Description:
 *	Create a gator text object.
 *
 * Arguments:
 *      struct onode *text_onp : Ptr to the text onode to fill out.
 *	struct onode_createparams *params : Generic ptr to creation
 *	    parameters.
 *
 * Returns:
 *	Zero if successful,
 *	Error value otherwise.
 *
 * Environment:
 *	The base onode fields have already been set.  Text onodes are
 *	empty upon creation.
 *
 * Side Effects:
 *	Upon successful creation, the onode's o_window field is
 *	replaced with a new window created for the text object,
 *	with the parent window is remembered within the new window
 *	structure.  On failure, it remains unchanged.
 *------------------------------------------------------------------------*/

int gator_text_create(text_onp, params)
    struct onode *text_onp;
    struct onode_createparams *params;

{ /*gator_text_create*/

    static char	rn[] = "gator_text_create";	/*Routine name*/
    struct gator_textobj_params *text_params;	/*My specific creation params*/
    struct gator_textobj *text_data;		/*Ptr to private data*/
    struct gator_textcb_hdr *newCB;		/*Ptr to CB hdr*/

    text_params = (struct gator_textobj_params *)params;
    if (objects_debug) {
      dce_svc_printf(GTX_S_TJ_PASSED_PRIVATE_DATA_MSG, mn, rn, text_onp);
      dce_svc_printf(GTX_S_TJ_MAX_ENTRIES_MSG, text_params->maxEntries, text_params->maxCharsPerEntry);
    }

    /*
      * Allocate the private data area.
      */
    if (objects_debug)
      dce_svc_printf(GTX_S_TJ_ALLOCATE_PRIVATE_BYTES_MSG, mn, rn, sizeof(struct gator_textobj));
    text_data = (struct gator_textobj *) malloc(sizeof(struct gator_textobj));
    if (text_data == (struct gator_textobj *)0) {
      dce_svc_printf(GTX_S_TJ_CANT_ALLOCATE_PRIVATE_BYTES_MSG, mn, rn, sizeof(struct gator_textobj), errno);
      return(errno);
    }

    /*
      * Create the text circular buffer for this new object.
      */
    if (objects_debug)
	dce_svc_printf(GTX_S_TJ_CREATE_CIRCULAR_BUFFER_MSG, mn, rn, text_params->maxEntries, text_params->maxCharsPerEntry);
      newCB = gator_textcb_Create(text_params->maxEntries,
				  text_params->maxCharsPerEntry);
      if (newCB == (struct gator_textcb_hdr *)0) {
	dce_svc_printf(GTX_S_TJ_CANT_CREATE_CIRCULAR_BUFFER_MSG, mn, rn);
	free(text_data);
	return(-1);
      }

    /*
      * Now that we have the private structures allocated, set them up.
      */
    text_data->llrock		= (int *)0;
    text_data->numLines		= text_onp->o_height;
    text_data->cbHdr		= newCB;
    text_data->firstEntShown	= 0;
    text_data->lastEntShown	= 0;
    if (objects_debug)
      dce_svc_printf(GTX_S_TJ_LINES_IN_WINDOW_MSG, mn, rn, text_data->numLines);

    /*
      * Attach the text-specific private
      * data to the generic onode and return the happy news.
      */
    text_onp->o_data   = (int *)text_data;
    return(0);

} /*gator_text_create*/

/*------------------------------------------------------------------------
 * gator_text_destroy
 *
 * Description:
 *	Destroy a gator text object.
 *
 * Arguments:
 *	struct onode *onp : Ptr to the text onode to delete.
 *
 * Returns:
 *	0: Success
 *	Error value otherwise.
 *
 * Environment:
 *	Nothing interesting.
 *
 * Side Effects:
 *	As advertised.
 *------------------------------------------------------------------------*/

int gator_text_destroy(onp)
    struct onode *onp;

{ /*gator_text_destroy*/

  /*
    * For now, this is a no-op.
    */
  return(0);

} /*gator_text_destroy*/

/*------------------------------------------------------------------------
 * gator_text_display
 *
 * Description:
 *	Display/redraw a gator text object.
 *
 * Arguments:
 *	struct onode *onp: Ptr to the text onode to display.
 *
 * Returns:
 *	0: Success
 *	Error value otherwise.
 *
 * Environment:
 *	Nothing interesting.
 *
 * Side Effects:
 *	As advertised.
 *------------------------------------------------------------------------*/

int gator_text_display(onp)
    struct onode *onp;

{ /*gator_text_display*/

    static char	rn[] = "gator_text_display";	/*Routine name*/
    struct gator_textobj *text_data;		/*Ptr to text obj data*/
    struct gator_textcb_hdr *cbHdr;		/*Ptr to CB header*/
    struct gwin_strparams strparams;		/*String-drawing params*/
    int currLine;				/*Current line being updated*/
    int currLinesUsed;				/*Num screen lines used*/
    int currIdx;				/*Current line index*/
    int currEnt;				/*Current entry being drawn*/
    struct gator_textcb_entry *curr_ent;	/*Ptr to current entry*/

    if (objects_debug)
      dce_svc_printf(GTX_S_TJ_DISPLAY_TEXT_OBJECT_MSG, mn, rn, onp);
    text_data = (struct gator_textobj *)(onp->o_data);
    cbHdr = text_data->cbHdr;
    if (objects_debug)
      dce_svc_printf(GTX_S_DISPLAY_OBJECT_SPECIFIC_MSG, mn, rn, onp, text_data);

    /*
      * Update each line in the screen buffer with its proper contents.
      */
    currEnt = text_data->firstEntShown;
    currLinesUsed = text_data->lastEntShown - currEnt + 1;
    currIdx = (cbHdr->oldestEntIdx + (currEnt - cbHdr->oldestEnt)) % cbHdr->maxEntriesStored;
    curr_ent = cbHdr->entry + currIdx;

    if (objects_debug)
      dce_svc_printf(GTX_S_TJ_DRAW_LINES_MSG, mn, rn, currLinesUsed, currEnt, currIdx, curr_ent);

    strparams.x = onp->o_x;
    strparams.y = onp->o_y;
    for (currLine = 0; currLine < text_data->numLines; currLine++) {
      /*
	* Draw the current entry.
	*/
      if (currLinesUsed > 0) {
	/*
	  * Drawing a populated line.  We need to iterate if there are
	  * inversions (I don't feel like doing this now).
	  */
	strparams.s	    = curr_ent->textp;
	strparams.highlight = curr_ent->highlight;
	WOP_DRAWSTRING(onp->o_window, &strparams);

	currLinesUsed--;
	currEnt++;
	currIdx++;
	if (currIdx >= cbHdr->maxEntriesStored) {
	  currIdx = 0;
	  curr_ent = cbHdr->entry;
	}
	else
	  curr_ent++;
      }
      else {
	/*
	  * Draw a blank line.
	  */
	strparams.s         = cbHdr->blankLine;
	strparams.highlight = 0;
	WOP_DRAWSTRING(onp->o_window, &strparams);
      }

      /*
	* Adjust the X and Y locations.
	*/
      strparams.x = 0;
      strparams.y++;
      
    } /*Update loop*/

    /*
      * Box the window before we leave.
      */
#if GATOR_TEXTCB_DO_BOX
    if (objects_debug)
      dce_svc_printf(GTX_S_TJ_BOX_WINDOW_MSG, mn, rn, onp->o_window);
    WOP_BOX(onp->o_window);
#endif /* GATOR_TEXTCB_DO_BOX */

    /*
     * For now, this is all we do.
     */
  return(0);

} /*gator_text_display*/

/*------------------------------------------------------------------------
 * gator_text_release
 *
 * Description:
 *	Drop the refcount on a gator text object.
 *
 * Arguments:
 *	struct onode *onp : Ptr to the onode whose refcount is
 *	                             to be dropped.
 *
 * Returns:
 *	0: Success
 *	Error value otherwise.
 *
 * Environment:
 *	Nothing interesting.
 *
 * Side Effects:
 *	As advertised.
 *------------------------------------------------------------------------*/

int gator_text_release(onp)
    struct onode *onp;

{ /*gator_text_release*/

  /*
    * For now, this is a no-op.
    */
  return(0);

} /*gator_text_release*/

/*------------------------------------------------------------------------
 * gator_text_Scroll
 *
 * Description:
 *	Scroll a text object some number of lines.
 *
 * Arguments:
 *	struct onode *onp : Ptr to the text onode to be scrolled.
 *	int nlines	  : Number of lines to scroll.
 *	int direction	  : Scroll up or down?
 *
 * Returns:
 *	0: Success,
 *	Error value otherwise.
 *
 * Environment:
 *	Invariant: the text object's firstEntShown and lastEntShown
 *		are always between oldestEnt and currEnt (inclusive).
 *
 * Side Effects:
 *	As advertised.
 *------------------------------------------------------------------------*/

int gator_text_Scroll(onp, nlines, direction)
    struct onode *onp;
    int nlines;
    int direction;

{ /*gator_text_Scroll*/

    static char rn[] = "gator_text_Scroll";	/*Routine name*/
    struct gator_textobj *text_data;		/*Ptr to text obj data*/

    /*
     * We move the markers for first & last entries displayed, depending
     * on what's available to us in the circular buffer.  We never leave
     * the window empty.
     */
    if (objects_debug)
      dce_svc_printf(GTX_S_TJ_SCROLL_OBJECT_MSG, mn, rn, nlines, (direction == GATOR_TEXT_SCROLL_UP) ? "UP" : "DOWN");

    text_data = (struct gator_textobj *)(onp->o_data);
    if (direction == GATOR_TEXT_SCROLL_DOWN) {
      /*
	* Move the object's text ``down'' by sliding the window up.
	*/
      text_data->firstEntShown -= nlines;
      if (text_data->firstEntShown < text_data->cbHdr->oldestEnt)
	text_data->firstEntShown = text_data->cbHdr->oldestEnt;

      text_data->lastEntShown -= nlines;
      if (text_data->lastEntShown < text_data->cbHdr->oldestEnt)
	text_data->lastEntShown = text_data->cbHdr->oldestEnt;

    } /*Moving down*/
    else {
      /*
	* Move the object's text ``up'' by sliding the window down.
	*/
      text_data->firstEntShown += nlines;
      if (text_data->firstEntShown > text_data->cbHdr->currEnt)
	text_data->firstEntShown = text_data->cbHdr->currEnt;

      text_data->lastEntShown += nlines;
      if (text_data->lastEntShown > text_data->cbHdr->currEnt)
	text_data->lastEntShown = text_data->cbHdr->currEnt;
      
    } /*Moving up*/

    /*
     * Return the happy news.
     */
    return(0);

} /*gator_text_Scroll*/

/*------------------------------------------------------------------------
 * gator_text_Write
 *
 * Description:
 *	Write the given string to the end of the gator text object.
 *
 * Arguments:
 *	struct onode *onp : Ptr to the onode whose to which we're
 *				writing.
 *	char *strToWrite  : String to write.
 *	int numChars	  : Number of chars to write.
 *	int highlight	  : Use highlighting?
 *	int skip	  : Force a skip to the next line?
 *
 * Returns:
 *	0: Success
 *	Error value otherwise.
 *
 * Environment:
 *	Nothing interesting.
 *
 * Side Effects:
 *	As advertised.
 *------------------------------------------------------------------------*/

int gator_text_Write(onp, strToWrite, numChars, highlight, skip)
    struct onode *onp;
    char *strToWrite;
    int numChars;
    int highlight;
    int skip;

{ /*gator_text_Write*/

    static char rn[] = "gator_text_Write";	/*Routine name*/
    register int code;				/*Return value on routines*/
    struct gator_textobj *text_data;		/*Ptr to text obj data*/
    struct gator_textcb_hdr *cbHdr;		/*Ptr to text CB header*/
    int i;					/*Loop variable*/
    int oldCurrEnt;				/*CB's old currEnt value*/
    int redisplay;				/*Redisplay after write?*/
    int shownDiff;				/*Diff between 1st & last lines*/
    int writeDiff;				/*Num lines really written*/
    int bumpAmount;				/*Amount to bump count*/

    /*
      * 
      */
    if (objects_debug) {
      dce_svc_printf(GTX_S_TJ_WRITE_CHARS_MSG, rn, numChars, onp, highlight, skip);
      for (i=0; i < numChars; i++)
	dce_svc_printf(GTX_S_TJ_WRITE_A_CHAR_MSG, strToWrite+i);
      dce_svc_printf(GTX_S_TJ_NEW_LINE_MSG);
    }

    if (numChars == 0) numChars = strlen(strToWrite);	/* simplify caller */
    text_data = (struct gator_textobj *)(onp->o_data);
    cbHdr = text_data->cbHdr;
    if (cbHdr == (struct gator_textcb_hdr *)0) {
      dce_svc_printf(GTX_S_TJ_MISSING_CIRCULAR_BUFFER_MSG, mn, rn);
      return(-1);
    }
    /*
      * If the current CB entry is being displayed, we track the write
      * visually and redisplay.
      */
    if ((cbHdr->currEnt <= text_data->lastEntShown) &&
	(cbHdr->currEnt >= text_data->firstEntShown)) {
      if (objects_debug)
	dce_svc_printf(GTX_S_TJ_CURRENT_ENTRY_MSG, mn, rn);
      oldCurrEnt = cbHdr->currEnt;
      redisplay = 1;
    }
    else {
      if (objects_debug)
	dce_svc_printf(GTX_S_TJ_NOT_CURRENT_ENTRY_MSG, mn, rn);
      redisplay = 0;
    }


    if (redisplay) {
      /*
	* We're tracking the write.  Compute the number of screen lines
	* actually written and adjust our own numbers, then call the
	* display function.
	*/
      shownDiff = text_data->lastEntShown - text_data->firstEntShown;
      writeDiff = cbHdr->currEnt - oldCurrEnt;
      if (objects_debug)
	dce_svc_printf(GTX_S_TJ_PREPARE_TO_REDISPLAY_MSG, mn, rn, shownDiff, writeDiff);
      if (shownDiff < (text_data->numLines - 1)) {
	/*
	  * We weren't showing a full screen of stuff.  Bump the last
	  * line shown by the minimum of the number of free lines still
	  * on the screen and the number of new lines actually written.
	  */
	bumpAmount = (text_data->numLines - 1) - shownDiff;
	if (writeDiff < bumpAmount)
	  bumpAmount = writeDiff;
	text_data->lastEntShown += bumpAmount;
	writeDiff -= bumpAmount;
	if (objects_debug)
	  dce_svc_printf(GTX_S_TJ_EMPTY_LINES_APPEARED_MSG, mn, rn, bumpAmount, writeDiff);
      }

      /*
	* If we have any more lines that were written not taken care
	* of by the above, we just bump the counters.
	*/
      if (writeDiff > 0) {
	if (objects_debug)
	  dce_svc_printf(GTX_S_TJ_STILL_MORE_LINES_MSG, mn, rn, writeDiff);
	text_data->firstEntShown += writeDiff;
	text_data->lastEntShown  += writeDiff;
      }
  } /*Redisplay needed*/

    /*
     * Simply call the circular buffer write op.
     */
    code = gator_textcb_Write(cbHdr, strToWrite, numChars, highlight, skip);
    if (code) {
      dce_svc_printf(GTX_S_TJ_CANT_WRITE_TO_OBJECT_MSG, mn, rn, code);
      return(code);
    }

    /*
     * Everything went well.  Return the happy news.
     */
    return(0);

} /*gator_text_Write*/

/*------------------------------------------------------------------------
 * gator_text_BlankLine
 *
 * Description:
 *	Write a given number of blank lines to the given text object.
 *
 * Arguments:
 *	struct onode *onp : Ptr to the onode to which we're writing.
 *	int numBlanks	  : Number of blank lines to write.
 *
 * Returns:
 *	0: Success,
 *	Error value otherwise.
 *
 * Environment:
 *	Nothing interesting.
 *
 * Side Effects:
 *	As advertised.
 *------------------------------------------------------------------------*/

int gator_text_BlankLine(onp, numBlanks)
    struct onode *onp;
    int numBlanks;

{ /*gator_text_BlankLine*/

    static char rn[] = "gator_text_BlankLine";	/*Routine name*/
    register int code;				/*Return value on routines*/
    struct gator_textobj *text_data;		/*Ptr to text obj data*/

    /*
     * We just call the circular buffer routine directly.
     */
    if (objects_debug)
      dce_svc_printf(GTX_S_TJ_WRITING_BLANKS_MSG, mn, rn, numBlanks, onp);

    text_data = (struct gator_textobj *)(onp->o_data);
    code = gator_textcb_BlankLine(text_data->cbHdr, numBlanks);
    if (code) {
      dce_svc_printf(GTX_S_TJ_CANT_WRITE_BLANKS_MSG, mn, rn, numBlanks, onp);
      return(code);
    }

    /*
      * Blank lines written successfully.  Adjust what lines are currently
      * shown.  Iff we were tracking the end of the buffer, we have to
      * follow the blank lines.
      */
    if (text_data->lastEntShown == text_data->cbHdr->currEnt - numBlanks) {
      text_data->firstEntShown += numBlanks;
      text_data->lastEntShown  += numBlanks;
    }

    /*
      * Return the happy news.
      */
    return(0);

} /*gator_text_BlankLine*/
