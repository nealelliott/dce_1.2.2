/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: gtxtextobj.h,v $
 * Revision 1.1.11.1  1996/10/02  17:51:18  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:40:06  damon]
 *
 * Revision 1.1.6.1  1994/06/09  14:10:17  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:24:43  annie]
 * 
 * Revision 1.1.4.3  1993/01/19  16:03:27  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  14:13:17  cjd]
 * 
 * Revision 1.1.4.2  1992/11/24  17:52:11  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  18:05:42  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/28  22:10:16  garyf
 * 	fix incorrect include
 * 	[1992/05/28  22:07:09  garyf]
 * 
 * Revision 1.1  1992/01/19  02:41:43  devrcs
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

#ifndef __gator_textobject_h
#define	__gator_textobject_h  1

#include <dcedfs/gtxobjects.h>	/*Basic gator object definitions*/
#include <dcedfs/gtxtextcb.h>	/*Text object's circular buffer facility*/

/*Value for onode o_type field*/
#define	GATOR_OBJ_TEXT	0

/*Scroll directions*/
#define GATOR_TEXT_SCROLL_DOWN	0
#define GATOR_TEXT_SCROLL_UP	1

/*Private data for text onode*/
struct gator_textobj {
    int	*llrock;			/*Rock for lower-level graphics layer*/
    int numLines;			/*Num lines we can display*/
    struct gator_textcb_hdr *cbHdr;	/*Ptr to circular buffer header*/
    int	firstEntShown;			/*ID of first text entry displayed*/
    int	lastEntShown;			/*ID of last text entry displayed*/
};

/*Text object's creation parameters*/
struct gator_textobj_params {
    struct onode_createparams onode_params;	/*Params for the whole onode*/
    int	maxEntries;				/*Max text entries to store*/
    int maxCharsPerEntry;			/*Max chars per text entry*/
};

/*Text object's creation routine*/

extern int gator_text_create();
    /*
     * Summary:
     *    Create a gator text object.
     *
     * Args:
     *	  struct onode *text_onp : Ptr to the text onode to fill out.
     *	  struct onode_createparams *params : Ptr to creation params.
     *       (Note: this actually points to a gator_text_crparams
     *        structure, but we use the generic version of the ptr)
     *
     * Returns:
     *	  Zero if successful,
     *	  Error value otherwise.
     */

/*Text object's generic onode routines*/

extern int gator_text_destroy();
    /*
     * Summary:
     *    Destroy a gator text object.
     *
     * Args:
     *	  struct onode *onp : Ptr to the text onode to delete.
     *
     * Returns:
     *	  0: Success.
     *	  Error value otherwise.
     */

extern int gator_text_display();
    /*
     * Summary:
     *    Display/redraw a gator text object.
     *
     * Args:
     *	  struct onode *onp: Ptr to the text onode to display.
     *
     * Returns:
     *	  0: Success.
     *	  Error value otherwise.
     */

extern int gator_text_release();
    /*
     * Summary:
     *    Drop the refcount on a gator text object.
     *
     * Args:
     *	  struct onode *onp : Ptr to the onode whose refcount is
     *	                               to be dropped.
     *
     * Returns:
     *	  0: Success.
     *	  Error value otherwise.
     */

/*
 * Additional, text-specific operations.
 */

extern int gator_text_Scroll();
    /*
     * Summary:
     *    Scroll a text object some number of lines.
     *
     * Args:
     *	  struct onode *onp	: Ptr to the text onode to be scrolled.
     *	  int nlines		: Number of lines to scroll.
     *	  int down		: Scroll down?
     *
     * Returns:
     *	  0: Success.
     *	  Error value otherwise.
     */

extern int gator_text_Write();
    /*
     * Summary:
     *    Write the given string to the end of the gator text object.
     *
     * Args:
     *	  struct onode *onp	: Ptr to the text onode to which we're
     *					writing.
     *	  char *strToWrite	: String to write.
     *	  int numChars		: Number of chars to write.
     *	  int highlight		: Use highlighting?
     *	  int skip		: Force a skip to the next line?
     *
     * Returns:
     *	  0: Success.
     *	  Error value otherwise.
     */

extern int gator_text_BlankLine();
    /*
     * Summary:
     *    Write a given number of blank lines to the given text object.
     *
     * Args:
     *	  struct onode *onp : Ptr to the onode to which we're writing.
     *	  int numBlanks	    : Number of blank lines to write.
     *
     * Returns:
     *	  0: Success.
     *	  Error value otherwise.
     */

/*
 * Set of exported generic text onode operations.
 */
extern struct onodeops gator_text_ops;

#endif /* __gator_textobject_h */
