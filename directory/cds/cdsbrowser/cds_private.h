/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 *(c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
*/
/*
 * HISTORY
 * $Log: cds_private.h,v $
 * Revision 1.1.6.2  1996/02/18  23:33:15  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:27  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:03:04  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:37:06  root]
 * 
 * Revision 1.1.4.3  1992/12/30  12:38:50  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:22:49  zeliff]
 * 
 * Revision 1.1.4.2  1992/09/29  19:08:26  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:09:58  weisman]
 * 
 * Revision 1.1  1992/01/19  15:23:19  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
*=======================================================================
*
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
*
*=======================================================================
*/


/*
**  Redefine XtIsRealized since windows are integer and cannot be standard
**  compared to NULL.
*/
#undef  XtIsRealized
#define XtIsRealized(widget)	((widget)->core.window != 0)

/*
**  Width & height of scroll and nav buttons.
*/
#define button_height		17



/*
**  Width of line used in highlighting entries in tree mode
*/
#define highlight_width	    4
#define mullion_width       5


/*
**  Global macros for manipulating sub widgets.
*/
/*
  #define HIDE_WIDGET(w,wid) XtUnmapWidget(wid)
  #define SHOW_WIDGET(wid,newx,newy,do_expose)				    \
    {									    \
	XtMoveWidget(wid,newx,newy);					    \
	XtMapWidget(wid);							    \
	if (do_expose)                                                      \
	   {                                                                \
	    XExposeEvent event;                                             \
	    event.type = Expose;                                            \
	    event.display = XtDisplay(w);                                \
	    event.window = XtWindow(wid);                                     \
	    event.x = 0;                                                    \
	    event.y = 0;                                                    \
	    event.width = XtWidth(wid);                                       \
	    event.height = XtHeight(wid);                                     \
	    event.count = 0;                                                \
	    (* (XtCoreProc (wid, expose))) (wid, &event);                       \
	   };                                                               \
    }
*/

#define HIDE_WIDGET(w,wid) XtMoveWidget(wid, (Position)XtWidth(w)*2, (Position)XtHeight(w)*2)
#define SHOW_WIDGET(w,wid,newx,newy,do_expose)   			    \
    {									    \
	XtMoveWidget(wid,newx,newy);					    \
	if (do_expose)                                                      \
	   {                                                                \
	    XExposeEvent event;                                             \
	    event.type = Expose;                                            \
	    event.display = XtDisplay(w);                                \
	    event.window = XtWindow(wid);                                     \
	    event.x = 0;                                                    \
	    event.y = 0;                                                    \
	    event.width = XtWidth(wid);                                       \
	    event.height = XtHeight(wid);                                     \
	    event.count = 0;                                                \
	    (* (XtCoreProc (wid, expose))) (wid, &event, (Region)0);                       \
	   };                                                               \
    }

#define OFFSCREENY(wid)      (XtHeight(wid) * 2)
#define REALHEIGHT(wid)      (int)(XtHeight(wid)-button_height)
#define REALWIDTH(wid)       (int)(XtWidth(wid)-button_height)
#define IDXW_WIDTH(w)   (REALWIDTH(w) >> 1)
#define IDXW_HEIGHT(w)  (REALHEIGHT(w) >> 1)


/*
** Convert a world coordinate into a window coordinate
*/
#define X_TO_WINDOW(w, x)	    ((x) - w->browse.mapx)
#define Y_TO_WINDOW(w, y)	    ((y) - w->browse.mapy)


/*
** Convert a window coordinate into a world coordinate
*/
#define X_TO_WORLD(w, x)	    ((x) + w->browse.mapx)
#define Y_TO_WORLD(w, y)	    ((y) + w->browse.mapy)

/*
** get the Y position of an entry in world coordinates
*/
#define X_POS(w, entry1)				\
    ((w->browse.tree_style == KtopTree) ?		\
        (entry1->x) :				\
	((w->browse.tree_style == KleftTree) ||	\
	 (w->browse.tree_style == KoutlineTree) ?	\
	    ((*w->browse.levely)[entry1->level]) :	\
	    (entry1->px)))

#define Y_POS(w, entry1)				\
    ((w->browse.tree_style == KtopTree) ?		\
	((*w->browse.levely)[entry1->level]) :	\
	((w->browse.tree_style == KleftTree) ||	\
	 (w->browse.tree_style == KoutlineTree) ?	\
	    (entry1->x) :				\
	    (entry1->py)))

#define PX_POS(wid, entry1)				\
    ((wid->browse.tree_style == KtopTree) ?		\
	(entry1->px) :				\
	((entry1->level == 0) ?			\
	    ((wid->browse.tree_style != KoutlineTree) ? 0 : (w->browse.level_spacing/2)) : \
	    ((*wid->browse.levely)[entry1->level-1] + entry1->py)))

#define PY_POS(w, entry1)				\
    ((w->browse.tree_style == KtopTree) ?		\
	((entry1->level == 0)? 0 : ((*w->browse.levely)[entry1->level-1] + entry1->py)) : \
	(entry1->px))


/*#pragma nostandard*/
#define RECT_COORDS(w, entry, ex, ey, ew, eh, x1, y1, x2, y2)	    \
{                                                                           \
    x1 = ex;                                                                \
    y1 = ey;		                                                    \
    x2 = ex + ew;							    \
    y2 = ey + eh;							    \
}

#define FILL_RECT_DATA(w, entry, ex, ey, ew, eh, x, y, width, height) \
{                                                                           \
    x = ex;                                                                 \
    y = ey;                                                                 \
    width = ew;                                                             \
    height = eh;                                                            \
	                                                                    \
    if ((w->browse.show_highlighting) && (entry->highlighted)) {	    \
	y += highlight_width + 1;					    \
	width -= 2 * highlight_width;					    \
	x += highlight_width;						    \
	height -= (2 * highlight_width) + 1;				    \
	}								    \
									    \
    if (w->browse.arc_width != 0) {					    \
	x += w->browse.arc_width;					    \
	width -= w->browse.arc_width * 2;				    \
	}								    \
									    \
}


#define FILL_RECT_COORDS(w, entry, ex, ey, ew, eh, x1, y1, x2, y2)    \
{                                                                           \
    FILL_RECT_DATA (w, entry, ex, ey, ew, eh, x1, y1, x2, y2)         \
    x2 += x1;                                                               \
    y2 += y1;                                                               \
}


#define FILL_LEFT_ARC_DATA(w, entry, ex, ey, ew, eh, x, y, width, height, start_angle, end_angle) \
{                                                                           \
    if ((w->browse.show_highlighting) && (entry->highlighted)) {	    \
	width = (2 * w->browse.arc_width);				    \
	height = entry->height - (2 * highlight_width);		    \
	x = ex + highlight_width;					    \
	y = ey + highlight_width;					    \
	start_angle = -90*64;                                               \
	end_angle = -180*64;                                                \
	}								    \
    else {								    \
	LEFT_ARC_DATA(w, entry, ex, ey, ew, eh, x, y, width, height, start_angle, end_angle); \
	}								    \
}


#define FILL_RIGHT_ARC_DATA(w, entry, ex, ey, ew, eh, x, y, width, height, start_angle, end_angle) \
{                                                                           \
    if ((w->browse.show_highlighting) && (entry->highlighted)) {	    \
	width = (2 * w->browse.arc_width);			   	    \
	height = eh - (2 * highlight_width);				    \
	x = ex + ew - (2 * w->browse.arc_width) - highlight_width;	    \
	y = ey + highlight_width;					    \
	start_angle = 90*64;                                                \
	end_angle = -180*64;                                                \
	}								    \
    else {								    \
	RIGHT_ARC_DATA(w, entry, ex, ey, ew, eh, x, y, width, height, start_angle, end_angle); \
	}								    \
}


#define HIGHLIGHT_RECT_DATA(w, entry, ex, ey, ew, eh, x, y, width, height)      \
{                                                                           \
    x = ex + highlight_width/2;						    \
    y = ey + (highlight_width/2);					    \
    width = ew - highlight_width + 1;					    \
    height = eh - highlight_width + 1;					    \
}

#define HIGHLIGHT_RECT_COORDS(w, entry, ex, ey, ew, eh, x1, y1, x2, y2)         \
{                                                                           \
    GET_HIGHLIGHT_RECT_DATA (w, entry, ex, ey, ew, eh, x1, y1, x2, y2)\
    x2 += x1;                                                               \
    y2 += y1;                                                               \
}


#define HIGHLIGHT_TOP_COORDS(w, entry, ex, ey, ew, eh, x1, y1, x2, y2)\
{                                                                           \
    x1 = ex + highlight_width/2 + w->browse.arc_width;			    \
    y1 = ey + (highlight_width/2);					    \
    x2 = ex + ew - highlight_width/2 - w->browse.arc_width;		    \
    y2 = ey + (highlight_width/2);					    \
}


#define HIGHLIGHT_BOT_COORDS(w, entry, ex, ey, ew, eh, x1, y1, x2, y2)\
{                                                                           \
    x1 = ex + highlight_width/2 + w->browse.arc_width;			    \
    y1 = ey + eh - (highlight_width/2) + 1;				    \
    x2 = ex + ew - highlight_width/2 - w->browse.arc_width;		    \
    y2 = ey + eh - (highlight_width/2);					    \
}


#define HIGHLIGHT_LEFT_ARC_DATA(w, entry, ex, ey, ew, eh, x, y, width, height, start_angle, end_angle) \
{                                                                           \
    x = ex + (highlight_width/2);					    \
    y = ey + (highlight_width/2) - 1;					    \
    width = w->browse.arc_width * 2;                                        \
    height = entry->height - (highlight_width/2);			    \
    start_angle = -90*64;                                                   \
    end_angle = -180*64;                                                    \
}


#define HIGHLIGHT_RIGHT_ARC_DATA(w, entry, ex, ey, ew, eh, x, y, width, height, start_angle, end_angle) \
{                                                                           \
    width = w->browse.arc_width * 2;                                        \
    height = eh - (highlight_width/2);					    \
    x = ex + ew - (2 * w->browse.arc_width) - (highlight_width/2);	    \
    y = ey + (highlight_width/2) - 1;					    \
    start_angle = 90*64;                                                    \
    end_angle = -180*64;                                                    \
}


#define LEFT_ARC_DATA(w, entry, ex, ey, ew, eh, x, y, width, height, start_angle, end_angle) \
{                                                                           \
    x = ex;			                                            \
    y = ey;					                            \
    width = w->browse.arc_width * 2;                                        \
    height = eh;					                    \
    start_angle = -90*64;                                                   \
    end_angle = -180*64;                                                    \
}


#define RIGHT_ARC_DATA(w, entry, ex, ey, ew, eh, x, y, width, height, start_angle, end_angle) \
{                                                                           \
    width = w->browse.arc_width * 2;                                        \
    height = eh;		                                            \
    x = ex + ew - (2 * w->browse.arc_width);				    \
    y = ey;					                            \
    start_angle = 90*64;                                                    \
    end_angle = -180*64;                                                    \
}


#define TOP_LINE_COORDS(w, entry, ex, ey, eh, ew, x1, y1, x2, y2) \
{                                                                           \
    x1 = ex +  w->browse.arc_width;					    \
    y1 = ey;								    \
    x2 = ex + ew - w->browse.arc_width;					    \
    y2 = ey;								    \
}


#define BOT_LINE_COORDS(w, entry, ex, ey, eh, ew, x1, y1, x2, y2)	    \
{                                                                           \
    x1 = ex +  w->browse.arc_width;					    \
    y1 = ey + eh;							    \
    x2 = ex + ew - w->browse.arc_width;					    \
    y2 = ey + eh;							    \
}
/*#pragma standard*/


/*
**  calculate connection points
*/
#define CONNECTION_DATA(w, points, num_points, x1, y1, x2, y2, tl)	    \
{									    \
if (w->browse.tree_connections) {					    \
    if (w->browse.perpendicular_lines) {				    \
	int xm1, xm2, ym1, ym2;  /* middle points of connection line */	    \
									    \
	if (w->browse.tree_style == KoutlineTree) {			    \
	    points[0].x = x1;						    \
	    points[0].y = y1;						    \
	    points[1].x = x1;						    \
	    points[1].y = y2;						    \
	    points[2].x = x2;						    \
	    points[2].y = y2;						    \
	    num_points = 3;						    \
	    }								    \
	else {								    \
	    if (w->browse.tree_style == KtopTree) {			    \
		ym1 = y2 - tl;						    \
		ym2 = ym1;						    \
		xm1 = x1;						    \
		xm2 = x2;						    \
		}							    \
	    else { /* for Left tree points are reversed */		    \
		xm1 = x2 - tl;						    \
		xm2 = xm1;						    \
		ym1 = y1;						    \
		ym2 = y2;						    \
		}							    \
									    \
	    points[0].x = x1;						    \
	    points[0].y = y1;						    \
	    points[1].x = xm1;						    \
	    points[1].y = ym1;						    \
	    points[2].x = xm2;						    \
	    points[2].y = ym2;						    \
	    points[3].x = x2;						    \
	    points[3].y = y2;						    \
	    num_points = 4;						    \
	    }								    \
	}								    \
    else {								    \
	points[0].x = x1;						    \
	points[0].y = y1;						    \
	points[1].x = x2;						    \
	points[1].y = y2;						    \
	num_points = 2;							    \
	}								    \
    }									    \
else									    \
    num_points = 0;							    \
}


/*
** Clear the screen for a redraw operation
*/
#define CLEAR_SCREEN(w)									\
{												\
    EntryPtr entry;									\
    int i, j;											\
												\
    if (w->browse.sub_widgets_used)								\
	for (i = 1; i <= w->browse.num_entries; i++) {						\
	    entry = StructGetEntryPtr (w,i);						\
	    for (j = 0;  j <= entry->num_components - 1;  j++) {	                        \
		if (entry->entrycompPtr[j].type == KcompWidget)			        \
		    HIDE_WIDGET(w, entry->entrycompPtr[j].var.is_widget.readwrite_text);  \
		};										\
	    };											\
												\
    XClearArea (XtDisplay(w), XtWindow(w->browse.lhs_window_widget), 0, 0, XtWidth(w), XtHeight(w), FALSE);   \
}


/*
**  Buffer macros for lines (segments)
*/
#define BUFF_SEG_INIT(name,size)				    \
struct {							    \
    int bmax;							    \
    int bnum;							    \
    GC bgc;							    \
    Window bw;							    \
    dns_widget bfw;						    \
    XSegment bdata[size];					    \
    } name


#define BUFF_SEG_SET_ATTRS(name,w,wid,gc)		\
{							\
    name.bfw = w;					\
    name.bw= wid;						\
    name.bgc = gc;					\
    name.bnum = 0;					\
    name.bmax = sizeof(name.bdata)/sizeof(XSegment);	\
}


#define BUFF_SEG_FLUSH(name)				\
{							\
    if (name.bnum != 0)					\
	XDrawSegments(XtDisplay(name.bfw),		\
	    name.bw, name.bgc, name.bdata, name.bnum);	\
    name.bnum = 0;					\
}

#define BUFF_SEG(name,ix1,iy1,ix2,iy2)			\
{							\
    if (name.bnum >= name.bmax)				\
	BUFF_SEG_FLUSH(name);				\
							\
    name.bdata[name.bnum].x1 = ix1;			\
    name.bdata[name.bnum].y1 = iy1;			\
    name.bdata[name.bnum].x2 = ix2;			\
    name.bdata[name.bnum++].y2 = iy2;			\
}



/*
**  Buffer Macros for Rectangles
*/
#define BUFF_RECT_INIT(name,size)				    \
struct {							    \
    int bnum;							    \
    int bmax;							    \
    int bfill;							    \
    GC bgc;				    			    \
    Window bw;							    \
    dns_widget bfw;  						    \
    XRectangle bdata[size];					    \
    } name


#define BUFF_RECT_SET_ATTRS(name,w,wid,gc,fill)	\
{							\
    name.bfw = w;					\
    name.bw= wid;						\
    name.bgc = gc;					\
    name.bnum = 0;					\
    name.bfill = fill;					\
    name.bmax = sizeof(name.bdata)/sizeof(XRectangle);	\
}


#define BUFF_RECT_FLUSH(name)				\
{							\
    if (name.bnum != 0) {				\
      if (name.bfill) { 				\
	XFillRectangles(XtDisplay(name.bfw),		\
	    name.bw, name.bgc, name.bdata, name.bnum);	\
	}						\
      else {						\
	XDrawRectangles(XtDisplay(name.bfw),		\
	    name.bw, name.bgc, name.bdata, name.bnum);	\
	}						\
      }							\
							\
    name.bnum = 0;					\
}


#define BUFF_RECT(name,x1,y1,wid,h)			\
{							\
    if (name.bnum >= name.bmax)				\
	BUFF_RECT_FLUSH(name);				\
							\
    name.bdata[name.bnum].x = x1;			\
    name.bdata[name.bnum].y = y1;			\
    name.bdata[name.bnum].width = wid;			\
    name.bdata[name.bnum++].height = h;			\
}



/*
**  Buffer macros fro Arcs
*/
#define BUFF_ARC_INIT(name,size)				    \
struct {							    \
    int bnum;							    \
    int bmax;							    \
    int bfill;							    \
    GC bgc;							    \
    Window bw;							    \
    dns_widget bfw;						    \
    XArc bdata[size];						    \
    } name


#define BUFF_ARC_SET_ATTRS(name,w,wid,gc,fill)		\
{							\
    name.bfw = w;					\
    name.bw= wid;						\
    name.bgc = gc;					\
    name.bnum = 0;					\
    name.bfill = fill;					\
    name.bmax = sizeof(name.bdata)/sizeof(XArc);	\
}


#define BUFF_ARC_FLUSH(name)				\
{							\
    if (name.bnum != 0) {				\
      if (name.bfill) {					\
	XFillArcs(XtDisplay(name.bfw),		\
	    name.bw, name.bgc, name.bdata, name.bnum);	\
        }						\
      else {						\
	XDrawArcs(XtDisplay(name.bfw),		\
	    name.bw, name.bgc, name.bdata, name.bnum);	\
	}						\
      }							\
							\
    name.bnum = 0;					\
}


/*
#define BUFF_ARC_FLUSH(name)				\
{							\
    int i;						\
    if (name.bnum != 0) {				\
      if (name.bfill) {					\
	for (i = 0; i < name.bnum; i++)			\
	    XFillArc(XtDisplay(name.bfw),		\
		name.bw, name.bgc,			\
		name.bdata[i].x,			\
		name.bdata[i].y,			\
		name.bdata[i].width,			\
		name.bdata[i].height,			\
		name.bdata[i].angle1,			\
		name.bdata[i].angle2);			\
        }						\
      else {						\
	for (i = 0; i < name.bnum; i++)			\
	    XDrawArc(XtDisplay(name.bfw),		\
		name.bw, name.bgc,			\
		name.bdata[i].x,			\
		name.bdata[i].y,			\
		name.bdata[i].width,			\
		name.bdata[i].height,			\
		name.bdata[i].angle1,			\
		name.bdata[i].angle2);			\
	}						\
      }							\
							\
    name.bnum = 0;					\
}
*/

#define BUFF_ARC(name,x1,y1,wid,h,a1,a2)			\
{							\
    if (name.bnum >= name.bmax)				\
	BUFF_ARC_FLUSH(name);				\
							\
    name.bdata[name.bnum].x = x1;			\
    name.bdata[name.bnum].y = y1;			\
    name.bdata[name.bnum].width = wid;			\
    name.bdata[name.bnum].height = h;			\
    name.bdata[name.bnum].angle1 = a1;			\
    name.bdata[name.bnum++].angle2 = a2;		\
							\
}

/*
**  Routines
*/
    extern Widget BrowseWindow ();


/*
**  Routines in display dispatch module and in service modules
*/

    extern void MapPosition ();
    extern void OutlineMapPosition ();
    extern void TopTreeMapPosition ();

    extern int  PositionDisplay ();
    extern int  OutlinePositionDisplay ();
    extern int  TopTreePositionDisplay ();

    extern void DisplayCreateGhost ();
    extern void OutlineCreateGhost ();
    extern void TopTreeCreateGhost ();

    extern void DisplayDeleteGhost ();
    extern void OutlineDeleteGhost ();
    extern void TopTreeDeleteGhost ();

    extern void DisplayInvalidateEntry ();
    extern void OutlineInvalidateEntry    ();
    extern void TopTreeInvalidateEntry    ();

    extern void DisplaySetEntrySensitivity ();
    extern void OutlineSetEntrySensitivity    ();
    extern void TopTreeSetEntrySensitivity    ();

    extern void DisplayAddEntries ();
    extern void OutlineAddEntries    ();
    extern void TopTreeAddEntries    ();

    extern void DisplayDeleteEntries ();
    extern void OutlineDeleteEntries    ();
    extern void TopTreeDeleteEntries    ();

    extern void DisplayEnableDisplay ();
    extern void OutlineEnableDisplay    ();
    extern void TopTreeEnableDisplay    ();

    extern void DisplayHighlightEntry ();
    extern void OutlineHighlightEntry ();
    extern void TopTreeHighlightEntry ();

    extern void AutoScrollDisplay ();
    extern void OutlineAutoScrollDisplay ();
    extern void TopTreeAutoScrollDisplay ();

    extern int AutoScrollCheck ();
    extern int OutlineAutoScrollCheck ();
    extern int TopTreeAutoScrollCheck ();

    extern void DisplayDraw ();
    extern void OutlineDraw ();
    extern void TopTreeDraw ();

    extern void DisplayAdjustHeight ();
    extern void OutlineAdjustHeight ();
    extern void TopTreeAdjustHeight ();

    extern void DisplayResize ();
    extern void OutlineResize ();
    extern void TopTreeResize ();

    extern void DisplaySetApplDragging ();
    extern void OutlineSetApplDragging ();
    extern void TopTreeSetApplDragging ();

    extern int  DisplayChangeMode ();

    extern void DisplayCreate ();
    extern void DisplayDrawEntry ();
    extern void DisplayDrawColumns ();
    extern void DisplayDrawColumnsBox ();
    extern void DisplayGraphicsExpose ();

    extern void	OutlineDrawExposed ();
    extern void	TopTreeDrawExposed ();

    extern void DisplayNavButton ();
    extern void TopTreeNavButton ();

    extern void DisplayAdjustEntrySize ();
    extern void OutlineAdjustEntrySize ();
    extern void TopTreeAdjustEntrySize ();

    extern void DisplaySetRangeHook ();
    extern void OutlineSetRangeHook ();
    extern void TopTreeSetRangeHook ();

    extern int  OutlineGetNumDisplayed ();
    extern void OutlineGetDisplayed ();

    extern void DisplayGetRangeEntries ();
    extern void OutlineGetRangeEntries ();
    extern void TopTreeGetRangeEntries ();

    extern void OutlineFlushEntry ();

    extern void OutlineScrollHelp ();

    extern void OutlineVScroll ();
    extern void TopTreeVScroll ();

    extern void OutlineHScroll ();
    extern void TopTreeHScroll ();

    extern void OutlineScrollButtonDown ();
    extern void OutlineScrollButtonUp ();

    extern void TopTreeScrollButton ();
    extern XFontStruct * DisplayFont ();
    extern void DisplayAdjustEntryHeight();
    extern void DisplayCompData ();

    extern void TopTreeSetCurrentEntry ();
    extern void DisplayExpose ();
    extern void DisplayMergeClips ();
    extern void DisplaySetWatch ();
    extern void DisplayWorkAreaFixup ();


/*
**  Distributed functionality defined.
*/
    extern void SelectToggleSelections  ();
    extern void SelectClearSelections   ();
    extern void SelectSelectSet         ();
    extern void SelectClearHighlighting ();
    extern void SelectReportTransitions ();


/*
**  Distributed functionality defined.
*/
    extern EntryPtr StructGetValidEntryPtr   ();
    extern EntryPtr StructGetEntryPtr        ();
    extern void        StructAddEntries      ();
    extern void        StructDeleteEntries   ();
    extern dns_widget  StructFindWidget      ();
    extern void        StructInvalidateEntry ();
    extern void        StructOpenArray          ();
    extern void        StructCloseArray         ();

/*
**  debugging diagnostics
*/
#define ILLENTNUM    "%DNS-F-ILLENTNUM, Invalid entry number"
#define NOSUCHENTNUM "%DNS-F-NOSUCHENTRY, The entry number specified does not exist"
#define DELTOOMANY   "%DNS-F-DELTOOMANY, Attempt to delete a nonexistant entry number"
#define NOTDIS       "%DNS-W-NOTDIS, should be disabled for this call"
#define NOTINVAL     "%DNS-W-NOTINVAL, This call should be made only on invalid entry numbers"
#define TOOENABLED   "%DNS-W-TOOENABLED, More calls to enable then disable"
#define INSERTHOLE   "%DNS-W-INSERTHOLE, Component number too large in InsertComponent call"
#define REMOVEMISS   "%DNS-W-REMOVEMISS, Component number not found in RemoveComponent call"
