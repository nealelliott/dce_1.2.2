/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: panep.h,v $
 * Revision 1.1.4.2  1996/02/18  23:33:17  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:30  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:03:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:37:28  root]
 * 
 * Revision 1.1.2.2  1992/12/30  12:40:06  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:25:26  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:23:22  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990. ALL RIGHTS RESERVED.
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
 * AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 *
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
 */

/*
 * $Header: /u0/rcs_trees/dce/rcs/directory/cds/cdsbrowser/panep.h,v 1.1.4.2 1996/02/18 23:33:17 marty Exp $
 */

#ifndef _PanePrivate_h
#define _PanePrivate_h

#include <pane.h>

/*
 *  Redefine XtIsRealized since windows are integer and cannot be standard
 *  compared to NULL.
*/
#undef  XtIsRealized
#define XtIsRealized(widget)	((widget)->core.window != 0)

/*
 * New fields for the Pane widget class record
 */
typedef struct {
    XmOffsetPtr	paneoffsets;
    int mumble;   /* No new procedures */
} PaneClassPart;

/*
 * Full class record declaration
 */
typedef struct _PaneClassRec {
    CoreClassPart	core_class;
    CompositeClassPart  composite_class;
    ConstraintClassPart constraint_class;
    PaneClassPart	pane_class;
} PaneClassRec, *PaneClass;


/*
 * New fields for the Pane widget record
 */
typedef struct _PanePart {
    XtCallbackList    helpcallback,	/* Help callback */
                      map_callback,     /* about to be mapped */
		      unmap_callback,   /* just been unmapped */
		      focus_callback;	/* grabbed input focus */
    unsigned char orientation;		/* Horizontal or Vertical pane. */
    Pixel foreground;		/* Foreground color for mullions, borders, etc. */
    Dimension mullionsize;	/* Size of mullions. same as borderwidth */
    Dimension mullionlength;	/* Actual Size of mullions. */
    Boolean leftmullion;	/* Have we actually left the mullion area */
    Dimension intersub;		/* Number of pixels between subwidgets. */
    WidgetList children;	/* List of subwidgets. */
    Cardinal num_children;	/* Number of subwidgets. */
    int resize_mode;		/* If TRUE, children are allowed to change */
				/* their length to whatever they want, and */
				/* we don't attempt to make things add up to */
				/* the correct total length. */
    GC invgc;			/* GC to use to draw magic borders */
    Widget whichadjust;		/* Which window we're currently dragging. */
    Dimension origloc;		/* Where the button was originally pressed. */
    Boolean overridetext;	/* Whether to override text bindings. */
    Widget lasthadfocus;	/* The last widget to which we gave focus. */
} PanePart;

/****************************************************************
 *
 * Full instance record declaration
 *
 ****************************************************************/

typedef struct _PaneRec {
    CorePart	    core;
    CompositePart   composite;
    ConstraintPart  constraint;
    PanePart	    pane;
} PaneRec, *pane_widget;

/*
 * Data to be kept for each child.
 */
typedef struct _ConstraintsRec {
    Dimension min, max;		/* Limits on length of this widget. */
    Dimension dlength;		/* Desired length for this widget. */
    Widget mullion;		/* Widget used for the mullion below this */
				/* widget (if any) */
    Dimension magicborder;	/* Where the last magic border was drawn. */
    Position position;		/* Where to place this beast */
    Cardinal sharedflag;	/* The position is shared with another widget */
				/* Also is the subwidget viewable */
    Boolean resizable;		/* allow widget to be resized by pane? */

} ConstraintsRec, *Constraints;

#ifndef PANE
/*#pragma nostandard*/
externalref PaneClassRec panewidgetclassrec;
/*#pragma standard*/
#endif

/***********************************************************************
 *
 * Pane Widget Private Data
 *
 ***********************************************************************/

typedef struct _MullionPart {
    XtCallbackList helpcallback;
} MullionPart;

typedef struct _MullionRec {
    CorePart	core;
    MullionPart	mullion;
} MullionRec, *MullionWidget;
/* $Log: panep.h,v $
 * Revision 1.1.4.2  1996/02/18  23:33:17  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:30  marty]
 *
 * Revision 1.1.5.2  1996/02/18  22:21:30  marty
 * 	Update OSF copyright years
 *
 * Revision 1.1.4.1  1995/12/08  15:03:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:37:28  root]
 *
 * Revision 1.1.3.2  1995/12/08  14:37:28  root
 * 	Submit OSF/DCE 1.2.1
 *
 * Revision 1.1.2.2  1992/12/30  12:40:06  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:25:26  zeliff]
 *
 * Revision 1.1.1.2  1992/12/29  22:25:26  zeliff
 * 	Embedding copyright notices
 *
 * Revision 1.1  1992/01/19  15:23:22  devrcs
 * Initial revision
 *
 * Revision 1.2  89/01/05  17:58:38  gringort
 * fixed position
 *
 * Revision 1.1  89/01/03  16:59:56  gringort
 * Initial revision
 *
 * Revision 1.3  88/02/26  13:29:52  weissman
 * Add focus handling to subwindows.
 *
 * Revision 1.2  87/12/16  15:31:12  weissman
 * Conversion to BL6.
 *
 * Revision 1.1  87/12/16  15:31:12  asente
 * Initial revision
 *
 * Revision 1.1  87/11/12  13:36:57  susan
 * Initial revision
 *
 * Revision 1.1  87/11/04  15:03:12  chow
 * Initial revision
 *
 * Revision 1.1  87/11/01  12:25:01  haynes
 * Initial revision
 *
 *  */


#endif /* _PanePrivate_h */
/* DON'T ADD STUFF AFTER THIS #endif */

/*  DEC/CMS REPLACEMENT HISTORY, Element PANEP.H */
/*  *4    30-MAR-1989 14:17:07 BRINKLEY "definitions of global variables fixed" */
/*  *3    28-MAR-1989 11:07:58 BRINKLEY "Style Guide Conformity" */
/*  *2     6-JAN-1989 12:36:17 BRINKLEY "new version" */
/*  *1    20-DEC-1988 14:51:26 RYAN "Initial elements for V2" */
/*  DEC/CMS REPLACEMENT HISTORY, Element PANEP.H */
