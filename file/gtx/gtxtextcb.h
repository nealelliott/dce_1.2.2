/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: gtxtextcb.h,v $
 * Revision 1.1.58.1  1996/10/02  17:51:15  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:40:05  damon]
 *
 * Revision 1.1.53.2  1994/06/09  14:10:15  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:24:41  annie]
 * 
 * Revision 1.1.53.1  1994/02/04  20:20:26  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:13:55  devsrc]
 * 
 * Revision 1.1.51.1  1993/12/07  17:26:24  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  15:32:24  jaffe]
 * 
 * Revision 1.1.4.3  1993/01/19  16:03:24  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  14:13:12  cjd]
 * 
 * Revision 1.1.4.2  1992/11/24  17:52:06  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  18:05:36  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/20  19:54:49  mason
 * 	Transarc delta: vijay-ot3289-gtx-fix-lock-calls 1.2
 * 	  Files modified:
 * 	    gtx: Makefile, X11windows.c, dumbwindows.c, frame.c
 * 	    gtx: gtxtextcb.h, input.c, lightobject.c, objects.c
 * 	    gtx: textcb.c, textobject.c
 * 	  Selected comments:
 * 	    change the lock_ calls to afslk_ calls.
 * 	    change lock_data to afslk_data
 * 	    remove a spurious cml diff line. This line was not deleted upon a merge
 * 	    and gave some compilations problems.
 * 	[1992/05/20  11:35:34  mason]
 * 
 * Revision 1.1  1992/01/19  02:41:41  devrcs
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

#ifndef __gator_textcb_h
#define	__gator_textcb_h  1

/*------------------------------------------------------------------------
 * gator_textcb.h
 *
 * Definitions and interface for the gator circular buffer package for
 * its scrollable text object.
 *
 * Author:
 *	Ed. Zayas
 *	Transarc Corporation
 *------------------------------------------------------------------------*/

#include <dcedfs/lock.h>

#define GATOR_TEXTCB_MAXINVERSIONS 10		/*Max highlight inversions*/

/*
  * Description of the text hanging off each circular buffer entry.
  */
struct gator_textcb_entry {
    int ID;					/*Overall ID (number)*/
    int highlight;				/*(Starting) highlight value*/
    int inversion[GATOR_TEXTCB_MAXINVERSIONS];	/*Highlighting inversions*/
    int numInversions;				/*Num of above inversions*/
    int charsUsed;				/*Num chars used*/
    char *textp;				/*Ptr to text buffer itself*/
};

/*
 * Circular buffer header.  Note: we actually allocate one more char
 * 	per line than we admit, to make sure we can always null-
 *	terminate each line.
 */
struct gator_textcb_hdr {
    osi_dlock_t cbLock;			/*Lock for this circular buffer*/
    int maxEntriesStored;		/*Max num. text entries we store*/
    int maxCharsPerEntry;		/*Max characters in each entry*/
    int currEnt;			/*Entry currently being written*/
    int currEntIdx;			/*Index of current entry*/
    int oldestEnt;			/*Oldest entry stored*/
    int oldestEntIdx;			/*Index of oldest entry*/
    struct gator_textcb_entry *entry;	/*Ptr to array of text entries*/
    char *blankLine;			/*Ptr to blank line*/
};

/*
  * Operations for text circular buffers.
  */
extern int gator_textcb_Init();
    /*
     * Summary:
     *    Initialize this package.  MUST BE THE FIRST ROUTINE CALLED!
     *
     * Args:
     *    int a_debug : Should debugging output be turned on?
     *
     * Returns:
     *    Zero if successful,
     *	  Error code otherwise.
     */

extern struct gator_textcb_hdr *gator_textcb_Create();
    /*
     * Summary:
     *    Create a new text circular buffer.
     *
     * Args:
     *	  int a_maxEntriesStored : How many entries should it have?
     *	  int a_maxCharsPerEntry : Max chars in each entry.
     *
     * Returns:
     *    Ptr to the fully-initialized circular buffer hdr if successful,
     *	  Null pointer otherwise.
     */

extern int gator_textcb_Write();
    /*
     * Summary:
     *    Write the given string to the text circular buffer.  Line
     *	  breaks are caused either by overflowing the current text
     *	  line or via explicit '\n's.
     *
     * Args:
     *	  struct gator_textcb_hdr *a_cbhdr : Ptr to circ buff hdr.
     *	  char *a_textToWrite		   : Ptr to text to insert.
     *    int a_numChars		   : Number of chars to write.
     *	  int a_highlight		   : Use highlighting?
     *	  int a_skip;			   : Force a skip to the next line?
     *
     * Returns:
     *    Zero if successful,
     *    Error code otherwise.
     */

extern int gator_textcb_BlankLine();
    /*
     * Summary:
     *    Write out some number of blank lines to the given circular
     *	  buffer.
     *
     * Args:
     *	  struct gator_textcb_hdr *a_cbhdr : Ptr to circ buff hdr.
     *	  int a_numBlanks		   : Num. blank lines to write.
     *
     * Returns:
     *    Zero if successful,
     *    Error code otherwise.
     */

extern int gator_textcb_Delete();
    /*
     * Summary:
     *    Delete the storage used by the given circular buffer, including
     *	  the header itself.
     *
     * Args:
     *	  struct gator_textcb_hdr *a_cbhdr : Ptr to the header of the
     *						circ buffer to reap.
     *
     * Returns:
     *    Zero if successful,
     *    Error code otherwise.
     */

#endif /* __gator_textcb_h */
