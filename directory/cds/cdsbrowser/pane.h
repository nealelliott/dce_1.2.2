/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: pane.h,v $
 * Revision 1.1.4.2  1996/02/18  23:33:16  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:29  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:03:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:37:27  root]
 * 
 * Revision 1.1.2.2  1992/12/30  12:40:01  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:25:22  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:23:21  devrcs
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
* $Header: /u0/rcs_trees/dce/rcs/directory/cds/cdsbrowser/pane.h,v 1.1.4.2 1996/02/18 23:33:16 marty Exp $
*/

#ifndef _Pane_h
#define _Pane_h

#ifdef VMS
/*#pragma nostandard*/
#include <PanedW.h>		/* gives XmNminimum and XmNmaximum */
/*#pragma standard*/
#else
#include <Xm/PanedW.h>
#endif


#ifndef PANE
/*#pragma nostandard*/
externalref WidgetClass panewidgetclass;
/*#pragma standard*/
#endif

#define PaneMpbNotShared	0L
#define	PaneMpbShared		(1L<<0)
#define	PaneMpbViewableInPane	(1L<<1)

/***********************************************************************
 *
 * Pane Widget (subclass of CompositeClass)
 *
 ***********************************************************************/

/* New fields */
#define PaneNmullionSize	"mullionSize"
#define PaneNoverrideText	"overrideText"
#define PaneNposition		"position"
#define PaneNsharedFlag		"sharedFlag"

#define PaneCMullionSize	"MullionSize"
#define PaneCOverrideText	"OverrideText"
#define PaneCPosition		"Position"
#define PaneCSharedFlag		"SharedFlag"

extern Widget PaneCreateWidget(); /* parent,name,arglist,argCount */
    /* Widget   parent; */
    /* char     *name; */
    /* Arglist  arglist; */
    /* Cardinal argCount; */

extern Widget PaneWidget(); /* parent, name, x, y, width, height, orientation, mapcallback, helpcallback */
    /* Widget		parent; */
    /* char		*name; */
    /* Position		x, y; */
    /* Dimension	width, height; */
    /* int		orientation; */
    /* XtCallbackList	mapcallback, helpcallback; */

extern void PaneAddWidget(); /* subwidget, position, min, max, resizeable, sharedflag */
    /* Widget subwidget; */
    /* Cardinal position */
    /* Dimension min, max; */
    /* Boolean resizable */
    /* Cardinal	sharedflag */

extern void PaneGetMinMax(); /* subwidget, min, max */
    /* Widget subwidget; */
    /* Dimension *min, *max; */

extern void PaneSetMinMax(); /* subwidget, min, max */
    /* Widget subwidget; */
    /* Dimension min, max; */

extern void PaneSetMin(); /* subwidget, min */
    /* Widget subwidget; */
    /* Dimension min; */

extern void PaneSetMax(); /* subwidget, max */
    /* Widget subwidget; */
    /* Dimension max; */

extern void PaneAllowResizing(); /* pane, allow */
    /* pane_widget pane; */
    /* int allowtype; */

extern void PaneMakeViewable(); /* subwidget */
    /* Widget subwidget; */

extern unsigned int PaneInitializeForMRM();

#endif /* _Pane_h */
/* DON'T ADD STUFF AFTER THIS #endif */

/*  DEC/CMS REPLACEMENT HISTORY, Element PANE.H */
/*  *6    11-APR-1989 11:27:31 BRINKLEY "leftmullion added" */
/*  *5     5-APR-1989 09:40:03 BRINKLEY "final standard I hope" */
/*  *4    30-MAR-1989 14:16:55 BRINKLEY "definitions of global variables fixed" */
/*  *3    28-MAR-1989 11:07:53 BRINKLEY "Style Guide Conformity" */
/*  *2     6-JAN-1989 12:35:18 BRINKLEY "new version of pane widget" */
/*  *1    20-DEC-1988 14:51:23 RYAN "Initial elements for V2" */
/*  DEC/CMS REPLACEMENT HISTORY, Element PANE.H */
