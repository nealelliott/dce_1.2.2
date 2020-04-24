/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cds_display.c,v $
 * Revision 1.1.8.2  1996/02/18  19:24:55  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:08:35  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:02:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:36:59  root]
 * 
 * Revision 1.1.6.2  1993/06/10  19:16:10  sommerfeld
 * 	Clean up include paths; treat GC as an opaque type.
 * 	[1993/06/08  18:08:34  sommerfeld]
 * 
 * Revision 1.1.2.2  1992/12/30  12:38:33  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:22:19  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:23:29  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#if !defined(lint) && !defined(SABER)
static char *SccsID = "@(#)cds_display.c\t12\t(DECdns)\t9/24/1991";
#endif	/* neither lint nor saber-C */
/*
*/
/*
*=======================================================================
*
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990,1991. ALL RIGHTS RESERVED.
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

#define  CDS_DISPLAY

#ifdef VMS
/*#pragma nostandard*/
#include <XmP.h>
#include <MrmPublic.h>
#include <StringDefs.h>
#include <Text.h>
#include <cursorfont.h>
#include <descrip.h>
extern char *VMSDescToNull();  /* jmg */
/*#pragma standard*/
#else
#include <Xm/XmP.h>
#include <Mrm/MrmPublic.h>
#include <X11/StringDefs.h>
#include <Xm/Text.h>
#include <X11/cursorfont.h>
#endif

#include <pane.h>
#include <cds_decwmp.h>
#include <cds_private.h>

/*
**  This module defines all display routines that can be called from
**  applications and from other modules within the widget.  This
**  module then dispatches to either the outline mode or the tree mode.
**
*/

/*
** Forward Declarations
*/
XmFontList DisplayFontList();


/*
**  XtCallbackRec structures for callbacks
*/
    static XtCallbackRec OutlineScrollHelp_CBstruct [2] = {{(XmVoidProc)OutlineScrollHelp, 0},{(XmVoidProc)NULL,0}};
    static XtCallbackRec OutlineVScroll_CBstruct    [2] = {{(XmVoidProc)OutlineVScroll,    0},{(XmVoidProc)NULL,0}};
    static XtCallbackRec OutlineHScroll_CBstruct    [2] = {{(XmVoidProc)OutlineHScroll,    0},{(XmVoidProc)NULL,0}};
    static XtCallbackRec DisplayNavButton_CBstruct  [2] = {{(XmVoidProc)DisplayNavButton,  0},{(XmVoidProc)NULL,0}};


/*
**  Translations/Actions for totop and tobottom buttons.  Note that these pass
**  an argument to the called procedure.
*/
#if 0
    static char TopButtonTranslation[] =
      "~Shift ~Ctrl ~Meta <Btn1Down>:      SCROLLBUTTONDOWN(1)\n\
        Shift ~Ctrl ~Meta <Btn1Down>:      SCROLLBUTTONDOWN(2)\n\
       ~Shift ~Ctrl ~Meta <Btn1Up>:     SCROLLBUTTONUP()\n\
        Shift ~Ctrl ~Meta <Btn1Up>:     SCROLLBUTTONUP()\n\
        Help<BtnUp>:                            DWTPBHELP()";
#else
    static char TopButtonTranslation[] =
      "~Shift ~Ctrl ~Meta <Btn1Down>:      SCROLLBUTTONDOWN(1)\n\
        Shift ~Ctrl ~Meta <Btn1Down>:      SCROLLBUTTONDOWN(2)\n\
       ~Shift ~Ctrl ~Meta <Btn1Up>:     SCROLLBUTTONUP()\n\
        Shift ~Ctrl ~Meta <Btn1Up>:     SCROLLBUTTONUP()";
#endif

#if 0
    static char BotButtonTranslation[] =
      "~Shift ~Ctrl ~Meta <Btn1Down>:      SCROLLBUTTONDOWN(3)\n\
        Shift ~Ctrl ~Meta <Btn1Down>:      SCROLLBUTTONDOWN(4)\n\
       ~Shift ~Ctrl ~Meta <Btn1Up>:     SCROLLBUTTONUP()\n\
        Shift ~Ctrl ~Meta <Btn1Up>:     SCROLLBUTTONUP()\n\
        Help<BtnUp>:                            DWTPBHELP()";
#else
    static char BotButtonTranslation[] =
      "~Shift ~Ctrl ~Meta <Btn1Down>:      SCROLLBUTTONDOWN(3)\n\
        Shift ~Ctrl ~Meta <Btn1Down>:      SCROLLBUTTONDOWN(4)\n\
       ~Shift ~Ctrl ~Meta <Btn1Up>:     SCROLLBUTTONUP()\n\
        Shift ~Ctrl ~Meta <Btn1Up>:     SCROLLBUTTONUP()";
#endif

    static XtActionsRec ButtonActionsList[] = {
      {"SCROLLBUTTONDOWN",   (XtActionProc)OutlineScrollButtonDown},
      {"SCROLLBUTTONUP",     (XtActionProc)OutlineScrollButtonUp},
      {(char *) NULL, (XtActionProc)NULL} };


/*
**  Definition of Nav button icons
*/
#define nav_open_width 17
#define nav_open_height 17
static char nav_open_bits[] = {
   0x00, 0x00, 0x00, 0xfe, 0xff, 0x00, 0x02, 0x80, 0x00, 0xfa, 0xbf, 0x00,
   0x0a, 0xa0, 0x00, 0xea, 0xaf, 0x00, 0x2a, 0xa8, 0x00, 0xaa, 0xab, 0x00,
   0xaa, 0xaa, 0x00, 0xaa, 0xab, 0x00, 0x2a, 0xa8, 0x00, 0xea, 0xaf, 0x00,
   0x0a, 0xa0, 0x00, 0xfa, 0xbf, 0x00, 0x02, 0x80, 0x00, 0xfe, 0xff, 0x00,
   0x00, 0x00, 0x00};

#define nav_close_width 17
#define nav_close_height 17
static char nav_close_bits[] = {
   0x00, 0x00, 0x00, 0xfe, 0xff, 0x00, 0x02, 0x80, 0x00, 0xfa, 0xbf, 0x00,
   0x0a, 0xa0, 0x00, 0xea, 0xaf, 0x00, 0x2a, 0xa8, 0x00, 0x2a, 0xa8, 0x00,
   0x2a, 0xa8, 0x00, 0x2a, 0xa8, 0x00, 0x2a, 0xa8, 0x00, 0xea, 0xaf, 0x00,
   0x0a, 0xa0, 0x00, 0xfa, 0xbf, 0x00, 0x02, 0x80, 0x00, 0xfe, 0xff, 0x00,
   0x00, 0x00, 0x00};

#define no_nav_width 17
#define no_nav_height 17
static char no_nav_bits[] = {
   0x00, 0x00, 0x00, 0xfe, 0xff, 0x00, 0x02, 0x80, 0x00, 0x02, 0x80, 0x00,
   0x02, 0x80, 0x00, 0x02, 0x80, 0x00, 0x02, 0x80, 0x00, 0x02, 0x80, 0x00,
   0x02, 0x80, 0x00, 0x02, 0x80, 0x00, 0x02, 0x80, 0x00, 0x02, 0x80, 0x00,
   0x02, 0x80, 0x00, 0x02, 0x80, 0x00, 0x02, 0x80, 0x00, 0xfe, 0xff, 0x00,
   0x00, 0x00, 0x00};


#define up_icon_width 17
#define up_icon_height 17
static char up_icon_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x01, 0x00, 0x80, 0x02, 0x00, 0x40, 0x04, 0x00, 0x20, 0x08, 0x00,
   0x10, 0x10, 0x00, 0x08, 0x20, 0x00, 0xfc, 0x7f, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00};

#define down_icon_width 17
#define down_icon_height 17
static char down_icon_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x7f, 0x00, 0x08, 0x20, 0x00,
   0x10, 0x10, 0x00, 0x20, 0x08, 0x00, 0x40, 0x04, 0x00, 0x80, 0x02, 0x00,
   0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00};

/*
**  Routine:  MapPosition
**
**  This routine must determine the entry number and component number that are
**  being displayed at the given x,y coordinate.  We will assume that this
**  coordinate is relative to the window.
*/

void MapPosition (w, findx, findy, entry_number, component, tag)

  dns_widget w;
  int findx;
  int findy;
  int *entry_number;
  int *component;
  unsigned int *tag;

{
/*
**  Dispatch to the appropriate routine
*/
    switch (w->browse.display_mode)
      {
        case KdisplayOutline: OutlineMapPosition (w, findx, findy, entry_number, component, tag);  break;
      }
}

/*
**  Routine:  PositionDisplay
**
**  This routine tries to satisfy an applications request to make a particular
**  entry displayed at a particular position.
*/

int PositionDisplay (w, entry_number, position)

  dns_widget w;
  int entry_number;
  int position;

{
/*
**  Local data declarations
*/
    int status;


/*
**  Dispatch to the appropriate routine
*/
    switch (w->browse.display_mode)
      {
        case KdisplayOutline: status = OutlinePositionDisplay (w, entry_number, position);  break;
      }


/*
**  Return the value
*/
    return (status);
}

/*
**  Routine:  AutoScrollCheck
**
**  This routine is called when the screen may need autoscrolled.  The module
**  uses this routine in dragging and in range selection.
*/

int AutoScrollCheck (w, x, y)

  dns_widget w;
  int x;
  int y;

{
/*
**  Local data declarations
*/
    int status;


/*
**  Dispatch to the appropriate routine
*/
    switch (w->browse.display_mode)
      {
        case KdisplayOutline: status = OutlineAutoScrollCheck (w, y);  break;
      }


/*
**  Return the status
*/
    return (status);
}

/*
**  Routine:  AutoScrollDisplay
**
**  This routine is called when the screen may need autoscrolled.  The module
**  uses this routine in dragging and in range selection.
*/

void AutoScrollDisplay (w, x, y)

  dns_widget w;
  int x;
  int y;

{
/*
**  Dispatch to the appropriate routine
*/
    switch (w->browse.display_mode)
      {
        case KdisplayOutline: OutlineAutoScrollDisplay (w, y);  break;
      }
}

/*
** Routine:  GetNumDisplayed
*/
int GetNumDisplayed (w)

  dns_widget w;

{
/*
**  Local data declarations
*/
    int count;


/*
**  Return to the caller the current number of entries being displayed.
*/
    switch (w->browse.display_mode)
      {
        case KdisplayColumns: count = OutlineGetNumDisplayed (w);  break;
        case KdisplayOutline: count = OutlineGetNumDisplayed (w);  break;
        case KdisplayTree:    count = 0;  break;
      }


/*
**  Return the count
*/
    return count;
}

/*
** Routine:  GetDisplayed
**
**  Call the display specific routine to get the list.
*/

void GetDisplayed (w, entry_nums, entry_tags, y_coords, num_array_entries)

  dns_widget w;
  int *entry_nums;
  unsigned int *entry_tags;
  int *y_coords;
  int num_array_entries;

{
/*
**  If it's not tree mode, then call the right procedure.
*/
    if (w->browse.display_mode != KdisplayTree)
         OutlineGetDisplayed (w, entry_nums, entry_tags, y_coords, num_array_entries);
    else
         {
          /*
          **  Tree mode does not support this procedure...
          */
              int i;
              for (i = 1;  i < num_array_entries;  i++)
                  {
                   if (entry_nums != (int *) NULL) entry_nums [i-1] = 0;
                   if (entry_tags != (unsigned int *) NULL) entry_tags [i-1] = 0;
                   if (y_coords   != (int *) NULL) y_coords   [i-1] = 0;
                  }
         }
}

/*
**  Routine:  DisplaySetRangeHook
**
**  This routine is called to store the starting corner of a range box.
*/

void DisplaySetRangeHook(w, x, y)

  dns_widget w;
  int x, y;

{
/*
**  Dispatch to the proper routine
*/
    switch (w->browse.display_mode)
      {
        case KdisplayOutline: OutlineSetRangeHook (w);  break;
      }
}

/*
**  Routine:  DisplayGetRangeEntries
**
**  This routine is called to map the region defined by the range hook and
**  the (x,y) point specified, into a list of entry_numbers
*/

void DisplayGetRangeEntries (w, x, y, range_flag, entry_count, entry_array)

  dns_widget w;
  int x;
  int y;
  int *range_flag;	/* TRUE if entry_array should be interpreted as a range */
  int *entry_count;
  LevelPtr *entry_array;

{
/*
**  Dispatch to the proper routine
*/
    switch (w->browse.display_mode)
      {
        case KdisplayOutline: OutlineGetRangeEntries (w, x, y, range_flag, entry_count, entry_array);  break;
      }
}

/*
**  Routine:  DisplayCreateGhost
**
**  This routine is called to create the ghost image to be used in a dragging
**  operation.  The identifier of the ghosting pixmap is saved in the instance
**  record.
*/

void DisplayCreateGhost (w)

  dns_widget w;

{
/*
**  Dispatch to the proper routine
*/
    switch (w->browse.display_mode)
      {
        case KdisplayOutline: OutlineCreateGhost (w);  break;
      }
}

/*
**  Routine:  DisplayDeleteGhost
**
**  This routine is called to delete the ghost image used in a dragging operation.
*/

void DisplayDeleteGhost (w)

  dns_widget w;

{
/*
**  Dispatch to the proper routine
*/
    switch (w->browse.display_mode)
      {
        case KdisplayOutline: OutlineDeleteGhost (w);  break;
      }
}

/*
**  Routine:  DisplayInvalidateEntry
**
**  This routine is called to do the display portion of the source
**  module invalidating an entry.  The purpose of this routine is to merely
**  record the fact that this entry was invalidated.  If the entry is not
**  currently in the display viewport, then this call is ignored.
*/

void DisplayInvalidateEntry (w, entry_number)

  dns_widget w;
  int entry_number;

{
/*
**  Dispatch to the proper routine
*/
    switch (w->browse.display_mode)
      {
        case KdisplayOutline: OutlineInvalidateEntry (w, entry_number);  break;
      }
}

/*
**  Routine:  DisplaySetEntrySensitivity
**
**  This routine is called whenever the application or source module has called
**  the SetEntrySensitivity high level call.
**
**  This routine is responsible for seeing if its currently in the display
**  viewport and if the widget is enabled, to changed the sensitivity right
**  away.  The caller of this routine has ensured that the sensitivity has
**  actually changed.  If the entry in question is not in the viewport, then
**  this call can be ignored.
**
**  Note that the sensitivity is the inverse of grayed and has already been set
**  by the corresponding Structure.C routine.
*/

void DisplaySetEntrySensitivity (w, entry_number)

  dns_widget w;
  int entry_number;

{
/*
**  Dispatch to the proper routine
*/
    switch (w->browse.display_mode)
      {
        case KdisplayOutline: OutlineSetEntrySensitivity (w, entry_number);  break;
      }
}

/*
**  Routine:  DisplayAddEntries
**
**  This routine is called indirectly from the AddEntries routine.
*/

void DisplayAddEntries (w, after, number)

  dns_widget w;
  int after;
  int number;

{
/*
**  Dispatch to the proper routine
*/
    switch (w->browse.display_mode)
      {
        case KdisplayOutline: OutlineAddEntries (w, after, number);  break;
      }
}

/*
**  Routine:  DisplayDeleteEntries
**
**  This routine is called indirectly from the DeleteEntries routine.
**  It's job is to mark the entries that will need
**  refreshed, when the widget is re-enabled.
*/

void DisplayDeleteEntries (w, after, number)

  dns_widget w;
  int after;
  int number;

{
/*
**  Dispatch to the proper routine
*/
    switch (w->browse.display_mode)
      {
        case KdisplayOutline: OutlineDeleteEntries (w, after, number);  break;
      }
}

/*
**  Routine:  DisplayEnableDisplay
**
**  This routine is called indirectly from the EnableDisplay routine.
**  It's job is to get the screen up to date based on calls
**  that were made during the disabled state.
*/

void DisplayEnableDisplay (w)

  dns_widget w;

{
/*
**  If this widget has not yet been realized, then ignore this call.  It will
**  be displayed when the expose event comes through.
*/
    if (!XtIsRealized(w)) return;


/*
**  Dispatch to the proper routine
*/
    switch (w->browse.display_mode)
      {
        case KdisplayOutline: OutlineEnableDisplay (w);  break;
      }
}

/*
**  Routine:  DisplayHighlightEntry
**
**  This routine is called when the selection status of an entry has
**  transitioned from low to high.  It may not be on the screen at this
**  time (Select All).
*/

void DisplayHighlightEntry (w, entry_number)

  dns_widget w;
  int entry_number;

{
/*
**  Dispatch to the proper routine
*/
    switch (w->browse.display_mode)
      {
        case KdisplayOutline: OutlineHighlightEntry (w, entry_number);  break;
      }
}

/*
**  Routine:  DisplayDraw
**
**  This routine is called when the widget is exposed or when the widget is
**  enabled and we don't know how many will fit.
*/

void DisplayDraw (w)

  dns_widget w;

{
/*
**  Dispatch to the proper routine
*/
    switch (w->browse.display_mode)
      {
        case KdisplayOutline: OutlineDraw (w);  break;
      }
}

/*
**  Routine:  DisplayAdjustHeight
**
**  This routine is called by the STRUCTURE module whenever a valid entry
**  is desired.  It is only called if 'height_adjusted' is false.
*/

void DisplayAdjustHeight (w, entry)

  dns_widget w;
  EntryPtr entry;

{
/*
**  Dispatch to the proper routine
*/
    switch (w->browse.display_mode)
      {
        case KdisplayOutline: OutlineAdjustHeight (w, entry);  break;
      }
}

/*
**  Procedure to change the geometry of a sub-widget.
**/

static void LclDisplayResizeSubwidget (w, wi, x, y, width, height)

   dns_widget w;
   Widget wi;
   Position x, y;
   Dimension width, height;

{
/*
**  If the widget is not realized, then complain
*/
    if (!XtIsRealized (wi)) return;


/*
**  Don't allow resizes to place widgets at negative positions or at positions
**  outside of the window...
*/
    if (x < 0) x = 0;
    if (y < 0) y = 0;

    if (x > XtWidth (w))  x = XtWidth (w);
    if (y > XtHeight(w))  y = XtHeight(w);


/*
**  Don't allow resizes to make children bigger.
*/
    if (width  > (XtWidth (w) - x))  width  = XtWidth (w) - x;
    if (height > (XtHeight(w) - y))  height = XtHeight(w) - y;


/*
**  If the X or Y position is different, then move the sub widget
*/
    if ((XtX(wi) != x) || (XtY(wi) != y))
       {
         XtMoveWidget (wi, (Position) x, (Position) y);
       }


/*
**  If the width or height is different, then set the new values and resize
**  the window.  Tell the widget about it afterwards.
*/
    if ((XtWidth(wi) != width) || (XtHeight(wi) != height))
       {
        /*
        **  Set the new width and height
        */
            XtWidth(wi)  = width;
            XtHeight(wi) = height;


        /*
        **  Resize the window
        */
            XtResizeWindow (wi);


        /*
        **  Call the widgets resize procedure
        */
            (* (XtCoreProc (wi, resize))) (wi);
       }
}

/*
**  Routine:  DisplayResize
**
**  This routine is called after the user has resized the window.  Note
**  that this procedure assumes that all border widths are 1.
*/

void DisplayResize (w)

  dns_widget w;

{
/*
**  Local data declarations
*/
    Position x, y;
    Dimension width, height, lhs_width, rhs_width;


/*
**  The pane widget needs resized.
*/
    x      = 0;
    y      = 0;
    width  = XtWidth (w) - button_height - 3;
    height = XtHeight (w);
    LclDisplayResizeSubwidget (w, w->browse.pane_widget, x, y, width, height);


/*
**  Default the lhs_width to be the whole work area in case there is no rhs.
*/
    lhs_width = width;


/*
**  Since the window has resized, we must apply the lhs_percentage.
*/
    if (w->browse.display_mode == KdisplayColumns)
       {
	/*
	**  The right hand side scroll window is the parent of
	**  the right hand side window widget.
	*/
	    Widget rhs_scroll_window = XtParent (w->browse.rhs_window_widget);

	/*
	**  Ensure legal values...
	*/
	    if (w->browse.lhs_percentage <   0) w->browse.lhs_percentage = 0;
	    if (w->browse.lhs_percentage > 100) w->browse.lhs_percentage = 100;

        /*
        **  Compute the right and left hand side widths.
	*/
            lhs_width = lhs_width - mullion_width;
            rhs_width = ((100 - w->browse.lhs_percentage) * lhs_width) / 100;
	    lhs_width = lhs_width - rhs_width;


	/*
	**  Make the Pane widget readjust
	*/
	    PaneSetMinMax (rhs_scroll_window, rhs_width, rhs_width);
	    PaneSetMinMax (rhs_scroll_window, 0, 0);
       }


/*
**  Now we must resize our work areas that are in the scroll windows.
*/
/*
**    x      = 0;
**    y      = 0;
**    width  = XtWidth (w->browse.hscroll);
**    height = XtY (w->browse.hscroll);
**    LclDisplayResizeSubwidget (w, w->browse.lhs_window_widget, x, y, width, height);
*/

/*
**  If we have a RHS...
*/
/*
**    if (w->browse.display_mode == KdisplayColumns)
**       {
**        width  = XtWidth (w->browse.rhs_hscroll);
**        height = XtY (w->browse.rhs_hscroll);
**        LclDisplayResizeSubwidget (w, w->browse.rhs_window_widget, x, y, width, height);
**       }
*/

/*
**  Set the size of the vertical scrollbar.  The Y position and the Width remain
**  the same, while the X and Height may change.
*/
    x      = XtWidth (w) - button_height - 2;
    width  = button_height;
    y      = 0;
    height = XtHeight(w) - button_height - 3;

    if (w->browse.use_scroll_buttons)
       {
	 y      = button_height + 1;
	 height = height - (2 * button_height) - 2;
       }

    LclDisplayResizeSubwidget (w, w->browse.vscroll, x, y, width, height);


/*
**  Set the size and position of the Nav button.
*/
    y      = XtHeight(w) - button_height - 2;
    x      = XtWidth (w) - button_height - 2;
    height = button_height;
    width  = button_height;
    LclDisplayResizeSubwidget (w, w->browse.nav_button, x, y, width, height);


/*
**  Move the buttons if we are using them...
*/
    if (w->browse.use_scroll_buttons)
       {
         x      = XtWidth (w) - button_height - 2;
         y      = 0;
         width  = button_height;
         height = button_height;
         LclDisplayResizeSubwidget (w, w->browse.top_button, x, y, width, height);

         y      = XtHeight(w) - (2 * button_height) - 3;
         LclDisplayResizeSubwidget (w, w->browse.bot_button, x, y, width, height);
       }


/*
**  Dispatch to the proper routine to do any remaining work.
*/
    switch (w->browse.display_mode)
      {
        case KdisplayOutline: OutlineResize (w);  break;
      }
}

/*
**  Routine:  DisplayWorkAreaFixup
**
**  This routine is called when one of the work areas is not totally occupying
**  the scroll window areas.  This situation is triggered by EXPOSE events
**  being generated on the  window itself.
*/

void DisplayWorkAreaFixup (w)

  dns_widget w;

{
/*
**  Local data declarations
*/
    Dimension lhs_width, rhs_width, height;


/*
**  Now we must resize our work areas that are in the scroll windows.
*/
    lhs_width  = XtWidth (w->browse.hscroll);
    height = XtY (w->browse.hscroll);
    LclDisplayResizeSubwidget (w, w->browse.lhs_window_widget, 0, 0, lhs_width, height);


/*
**  If we have a RHS...
*/
    if (w->browse.display_mode == KdisplayColumns)
       {
        rhs_width  = XtWidth (w->browse.rhs_hscroll);
        height = XtY (w->browse.rhs_hscroll);
        LclDisplayResizeSubwidget (w, w->browse.rhs_window_widget, 0, 0, rhs_width, height);
        w->browse.display_changed = TRUE;
	w->browse.lhs_percentage = (lhs_width * 100) / (lhs_width + rhs_width);
       }
}

/*
**  Routine:  DisplayCreate
**
**  This routine is called during the Initialize phase of the widget.
**
**  This routine creates the vertical and horizontal scroll bars.  It also
**  creates the index window but does not manage it.
**
**  This routine only creates the widgets.  During the realize stage of the
**  initialization all of the widgets are positioned correctly anyway, so
**  just try to get as much of them right as possible.
**
**     widget
**     |
**     +---- Top arrow push button
**     |
**     +---- Vertical scroll bar
**     |
**     +---- Bottom arrow push button
**     |
**     +---- Corner navigation button
**     |
**     +---- Pane widget (Readonly resource)
**            |
**            +---- Scroll Window
**            |      |
**            |      +---- Simple window
**            |      |
**            |      +---- Horizontal scroll bar
**            |
**            +---- Scroll window
**                   |
**                   +---- Simple window
**                   |
**                   +---- Horizontal scroll bar
*/

void DisplayCreate (w)

  dns_widget w;

{
/*
**  Local data declarations
*/
/*#pragma nostandard*/
    externalref XtTranslations parsed_translations;
/*#pragma standard*/
    Arg arglist[20];
    int	argcnt = 0;

    Dimension pane_width, lhs_width, rhs_width, height;
    XtTranslations compiled_translations = NULL;
    Widget lhs_scroll_window, rhs_scroll_window;


/*
**  Translation tables for lhs/rhs scroll windows
*/

static char simple_window_translations [] =
   "<Expose>:		REDISPLAY()\n\
    <GraphicsExpose>:	REDISPLAY()";

static char scroll_window_translations [] =
   "<ConfigureNotify>:  WORKAREAFIXUP()";


/*
**  Compute the default pane width based on the width.  Make
**  the minimum width 100 pixels.
*/
    pane_width = XtWidth(w) - button_height;
    if (pane_width < 100) pane_width = 100;


/*
**  Compute the right hand side and left hand side widths
**  to be 1/2 of the pane.
*/
    lhs_width = pane_width;
    rhs_width = pane_width;


/*
**  Try to do better if this is in columns mode.
*/
    if (w->browse.display_mode == KdisplayColumns)
	{
          lhs_width = lhs_width - mullion_width;
          rhs_width = ((100 - w->browse.lhs_percentage) * lhs_width) / 100;
	  lhs_width = lhs_width - rhs_width;
        }


/*
**  Compute the height (minimum of 100)
*/
    height = XtHeight (w);
    if (height < 100) height = 100;


/*
**  Create the Pane widget.  We remove the translations from the Pane widget
**  because it causes button events to be absorbed just due to it asking for
**  help modified buttons.
*/
    argcnt = 0;
    XtSetArg (arglist[argcnt], XmNresizePolicy, XmRESIZE_ANY); argcnt++;
    XtSetArg (arglist[argcnt], XmNorientation, XmHORIZONTAL); argcnt++;
    XtSetArg (arglist[argcnt], XmNborderWidth, 0); argcnt++;
    XtSetArg (arglist[argcnt], XmNwidth, pane_width); argcnt++;
    XtSetArg (arglist[argcnt], XmNheight, height); argcnt++;

    w->browse.pane_widget = (Widget) PaneCreateWidget (w, "pane", arglist, argcnt);
    XtUninstallTranslations (w->browse.pane_widget);
    XtManageChild (w->browse.pane_widget);


/*
**  Create the two scroll windows and put them in the Pane widget.
*/
    argcnt = 0;
    XtSetArg(arglist[argcnt], XmNx, 0); argcnt++;
    XtSetArg(arglist[argcnt], XmNy, 0); argcnt++;
    XtSetArg(arglist[argcnt], XmNwidth, lhs_width); argcnt++;
    XtSetArg(arglist[argcnt], XmNheight, height); argcnt++;
    XtSetArg(arglist[argcnt], XmNscrollingPolicy, XmAPPLICATION_DEFINED); argcnt++;
    XtSetArg(arglist[argcnt], XmNborderWidth, 0); argcnt++;
    lhs_scroll_window = (Widget) XmCreateScrolledWindow(w->browse.pane_widget,
			"lhs",
			arglist, argcnt);

    argcnt = 0;
    XtSetArg(arglist[argcnt], XmNx, 0); argcnt++;
    XtSetArg(arglist[argcnt], XmNy, 0); argcnt++;
    XtSetArg(arglist[argcnt], XmNwidth, rhs_width); argcnt++;
    XtSetArg(arglist[argcnt], XmNheight, height); argcnt++;
    XtSetArg(arglist[argcnt], XmNscrollingPolicy, XmAPPLICATION_DEFINED); argcnt++;
    XtSetArg(arglist[argcnt], XmNborderWidth, 0); argcnt++;
    rhs_scroll_window = (Widget) XmCreateScrolledWindow(w->browse.pane_widget,
			"rhs",
			arglist, argcnt);


/*
**  Manage the right hand side only if we are in columns mode
*/
    XtManageChild (lhs_scroll_window);
    if (w->browse.display_mode == KdisplayColumns)
       XtManageChild (rhs_scroll_window);


/*
**  Create the simple  windows parented to the scroll windows.  Note that they are always
**  created without borders.
*/
    w->browse.rhs_window_widget = BrowseWindow (rhs_scroll_window, "rhs_window", rhs_width, height);
    w->browse.lhs_window_widget = BrowseWindow (lhs_scroll_window, "lhs_window", lhs_width, height);
    XtManageChild (w->browse.rhs_window_widget);
    XtManageChild (w->browse.lhs_window_widget);


/*
**  Compile the translations and add them to the widgets
*/
    compiled_translations = XtParseTranslationTable (simple_window_translations);
    XtAugmentTranslations (w->browse.lhs_window_widget, parsed_translations);
    XtAugmentTranslations (w->browse.rhs_window_widget, parsed_translations);
    XtAugmentTranslations (w->browse.lhs_window_widget, compiled_translations);
    XtAugmentTranslations (w->browse.rhs_window_widget, compiled_translations);

    compiled_translations = XtParseTranslationTable (scroll_window_translations);
    XtAugmentTranslations (lhs_scroll_window, compiled_translations);
    XtAugmentTranslations (rhs_scroll_window, compiled_translations);


/*
**  Create the work area and the horizontal scroll for the left hand side
*/
    argcnt = 0;
    XtSetArg(arglist[argcnt], XmNx, 0); argcnt++;
    XtSetArg(arglist[argcnt], XmNy, 0); argcnt++;
    XtSetArg(arglist[argcnt], XmNwidth, lhs_width); argcnt++;
    XtSetArg(arglist[argcnt], XmNheight, button_height); argcnt++;
    XtSetArg(arglist[argcnt], XmNincrement, 1); argcnt++;
    XtSetArg(arglist[argcnt], XmNpageIncrement, 1); argcnt++;
    XtSetArg(arglist[argcnt], XmNsliderSize, 4); argcnt++;
    XtSetArg(arglist[argcnt], XmNvalue, 1); argcnt++;
    XtSetArg(arglist[argcnt], XmNminimum, 1); argcnt++;
    XtSetArg(arglist[argcnt], XmNmaximum, 5); argcnt++;
    XtSetArg(arglist[argcnt], XmNorientation, XmHORIZONTAL); argcnt++;
    XtSetArg(arglist[argcnt], XmNvalueChangedCallback, 	OutlineHScroll_CBstruct); argcnt++;
    XtSetArg(arglist[argcnt], XmNhelpCallback, 		OutlineScrollHelp_CBstruct); argcnt++;
    XtSetArg(arglist[argcnt], XmNincrementCallback, 	OutlineHScroll_CBstruct); argcnt++;
    XtSetArg(arglist[argcnt], XmNdecrementCallback, 	OutlineHScroll_CBstruct); argcnt++;
    XtSetArg(arglist[argcnt], XmNpageIncrementCallback, OutlineHScroll_CBstruct); argcnt++;
    XtSetArg(arglist[argcnt], XmNpageDecrementCallback, OutlineHScroll_CBstruct); argcnt++;
    XtSetArg(arglist[argcnt], XmNtoTopCallback, 	OutlineHScroll_CBstruct); argcnt++;
    XtSetArg(arglist[argcnt], XmNtoBottomCallback, 	OutlineHScroll_CBstruct); argcnt++;
    XtSetArg(arglist[argcnt], XmNdragCallback, 		OutlineHScroll_CBstruct); argcnt++;
    w->browse.hscroll = (Widget) XmCreateScrollBar(lhs_scroll_window,
			"lhs_scrollbar",
			arglist, argcnt);

/*
**  Manage the two scroll bars
*/
    XtManageChild (w->browse.hscroll);


/*
**  Create the horizontal scroll for the left hand side
*/
    argcnt = 0;
    XtSetArg(arglist[argcnt], XmNx, 0); argcnt++;
    XtSetArg(arglist[argcnt], XmNy, 0); argcnt++;
    XtSetArg(arglist[argcnt], XmNwidth, rhs_width); argcnt++;
    XtSetArg(arglist[argcnt], XmNheight, button_height); argcnt++;
    XtSetArg(arglist[argcnt], XmNincrement, 1); argcnt++;
    XtSetArg(arglist[argcnt], XmNpageIncrement, 1); argcnt++;
    XtSetArg(arglist[argcnt], XmNsliderSize, 4); argcnt++;
    XtSetArg(arglist[argcnt], XmNvalue, 1); argcnt++;
    XtSetArg(arglist[argcnt], XmNminimum, 1); argcnt++;
    XtSetArg(arglist[argcnt], XmNmaximum, 5); argcnt++;
    XtSetArg(arglist[argcnt], XmNorientation, XmHORIZONTAL); argcnt++;
    XtSetArg(arglist[argcnt], XmNvalueChangedCallback, 	OutlineHScroll_CBstruct); argcnt++;
    XtSetArg(arglist[argcnt], XmNhelpCallback, 		OutlineScrollHelp_CBstruct); argcnt++;
    XtSetArg(arglist[argcnt], XmNincrementCallback, 	OutlineHScroll_CBstruct); argcnt++;
    XtSetArg(arglist[argcnt], XmNdecrementCallback, 	OutlineHScroll_CBstruct); argcnt++;
    XtSetArg(arglist[argcnt], XmNpageIncrementCallback, OutlineHScroll_CBstruct); argcnt++;
    XtSetArg(arglist[argcnt], XmNpageDecrementCallback, OutlineHScroll_CBstruct); argcnt++;
    XtSetArg(arglist[argcnt], XmNtoTopCallback, 	OutlineHScroll_CBstruct); argcnt++;
    XtSetArg(arglist[argcnt], XmNtoBottomCallback, 	OutlineHScroll_CBstruct); argcnt++;
    XtSetArg(arglist[argcnt], XmNdragCallback, 		OutlineHScroll_CBstruct); argcnt++;
    w->browse.rhs_hscroll = (Widget) XmCreateScrollBar(rhs_scroll_window,
			    "rhs_scrollbar",
			    arglist, argcnt);


/*
**  Manage the two scroll bars
*/
    XtManageChild (w->browse.rhs_hscroll);


#if 0

Motif does not allow Scrolling Policy to be set with SetValues.
Scrolling Policy must be set during create time!

/*
**  Set the horizontal scroll bars and information on the scroll windows
*/
    XtSetArg (arglist[0], XmNscrollingPolicy, XmAPPLICATION_DEFINED);
    XtSetArg (arglist[1], XmNborderWidth, 0);
    XtSetValues (lhs_scroll_window, arglist, 2);
    XtSetValues (rhs_scroll_window, arglist, 2);

#endif

/*
**  Set the work area and scrollbars to the ScrollWindows
*/
    XmScrolledWindowSetAreas (lhs_scroll_window, w->browse.hscroll, NULL, w->browse.lhs_window_widget);
    XmScrolledWindowSetAreas (rhs_scroll_window, w->browse.rhs_hscroll, NULL, w->browse.rhs_window_widget);


/*
**  Create the vertical scrollbar widget
*/
    argcnt = 0;
    XtSetArg(arglist[argcnt], XmNx, 0); argcnt++;
    XtSetArg(arglist[argcnt], XmNy, 0); argcnt++;
    XtSetArg(arglist[argcnt], XmNwidth, button_height); argcnt++;
    XtSetArg(arglist[argcnt], XmNheight, height); argcnt++;
    XtSetArg(arglist[argcnt], XmNincrement, 1); argcnt++;
    XtSetArg(arglist[argcnt], XmNpageIncrement, 1); argcnt++;
    XtSetArg(arglist[argcnt], XmNsliderSize, 4); argcnt++;
    XtSetArg(arglist[argcnt], XmNvalue, 1); argcnt++;
    XtSetArg(arglist[argcnt], XmNminimum, 1); argcnt++;
    XtSetArg(arglist[argcnt], XmNmaximum, 5); argcnt++;
    XtSetArg(arglist[argcnt], XmNorientation, XmVERTICAL); argcnt++;
    XtSetArg(arglist[argcnt], XmNvalueChangedCallback, 	OutlineVScroll_CBstruct); argcnt++;
    XtSetArg(arglist[argcnt], XmNhelpCallback, 		OutlineScrollHelp_CBstruct); argcnt++;
    XtSetArg(arglist[argcnt], XmNincrementCallback, 	OutlineVScroll_CBstruct); argcnt++;
    XtSetArg(arglist[argcnt], XmNdecrementCallback, 	OutlineVScroll_CBstruct); argcnt++;
    XtSetArg(arglist[argcnt], XmNpageIncrementCallback, OutlineVScroll_CBstruct); argcnt++;
    XtSetArg(arglist[argcnt], XmNpageDecrementCallback, OutlineVScroll_CBstruct); argcnt++;
    XtSetArg(arglist[argcnt], XmNtoTopCallback, 	OutlineVScroll_CBstruct); argcnt++;
    XtSetArg(arglist[argcnt], XmNtoBottomCallback, 	OutlineVScroll_CBstruct); argcnt++;
    XtSetArg(arglist[argcnt], XmNdragCallback, 		OutlineVScroll_CBstruct); argcnt++;
    w->browse.vscroll = (Widget) XmCreateScrollBar(w,
			"SVSc",
			arglist, argcnt);

    XtManageChild (w->browse.vscroll);


/*
**  Set up the common arglist information for all of our pushbuttons
*/
    argcnt = 0;
    XtSetArg (arglist[argcnt], XmNwidth, button_height); argcnt++;
    XtSetArg (arglist[argcnt], XmNheight, button_height); argcnt++;
    XtSetArg (arglist[argcnt], XmNlabelType, XmPIXMAP); argcnt++;
    XtSetArg (arglist[argcnt], XmNhelpCallback, OutlineScrollHelp_CBstruct); argcnt++;
#if 0
    XtSetArg (arglist[argcnt],
>>>Mapping unknown: XmNshadowThickness-Primitive-
       ,FALSE                     ); argcnt++;
#endif

/*
**  The pushbutton x position is the same as the vertical scrollbar
*/
    if (w->browse.use_scroll_buttons)
       {
        /*
        **  Local data declarations
        */
            XtTranslations top_parsed = XtParseTranslationTable (TopButtonTranslation);
            XtTranslations bot_parsed = XtParseTranslationTable (BotButtonTranslation);


        /*
        **  Add our actions table in...
        */
            XtAddActions (ButtonActionsList, XtNumber(ButtonActionsList));


        /*
        **  Create the pixmaps...
        */
	    w->browse.top_pixmap = XCreatePixmapFromBitmapData (
		XtDisplay(w),			/* (IN) display */
		XDefaultRootWindow(XtDisplay(w)),	/* (IN) drawable */
		up_icon_bits,				/* (IN) bitmap data */
		up_icon_width,				/* (IN) pixmap width */
		up_icon_height,				/* (IN) pixmap height */
		w->browse.foreground_pixel,		/* (IN) foreground pixel */
		w->core.background_pixel,		/* (IN) background pixel */
		DefaultDepthOfScreen(XtScreen(w)));	/* (IN) pixmap depth */

	    w->browse.bot_pixmap = XCreatePixmapFromBitmapData (
		XtDisplay(w),			/* (IN) display */
		XDefaultRootWindow(XtDisplay(w)),	/* (IN) drawable */
		down_icon_bits,				/* (IN) bitmap data */
		down_icon_width,			/* (IN) pixmap width */
		down_icon_height,			/* (IN) pixmap height */
		w->browse.foreground_pixel,		/* (IN) foreground pixel */
		w->core.background_pixel,		/* (IN) background pixel */
		DefaultDepthOfScreen(XtScreen(w)));	/* (IN) pixmap depth */

        /*
        **  Create the top push button
        */
            XtSetArg (arglist[argcnt], XmNlabelPixmap, w->browse.top_pixmap);
            XtSetArg (arglist[argcnt+1], XmNtranslations, top_parsed);

            w->browse.top_button = (Widget) XmCreatePushButton (w, "T", arglist, argcnt+2);
            XtManageChild (w->browse.top_button);


        /*
        **  Create the bottom button
        */
            XtSetArg (arglist[argcnt], XmNlabelPixmap, w->browse.bot_pixmap);
            XtSetArg (arglist[argcnt+1], XmNtranslations, bot_parsed);

            w->browse.bot_button = (Widget) XmCreatePushButton (w, "B", arglist, argcnt+2);
            XtManageChild (w->browse.bot_button);
      }


/*
**  Set the fields to zero if we were not using the buttons.
*/
    if (!w->browse.use_scroll_buttons)
       {
        w->browse.top_pixmap = (Pixmap) NULL;
        w->browse.bot_pixmap = (Pixmap) NULL;
        w->browse.top_button = (Widget) NULL;
        w->browse.bot_button = (Widget) NULL;
      }


/*
**  Create the nav button pixmap
*/
    w->browse.tree_nav_open_pixmap = XCreatePixmapFromBitmapData (
	XtDisplay(w),			/* (IN) display */
	XDefaultRootWindow(XtDisplay(w)),	/* (IN) drawable */
	nav_open_bits,				/* (IN) bitmap data */
	nav_open_width,				/* (IN) pixmap width */
	nav_open_height,			/* (IN) pixmap height */
	w->browse.foreground_pixel,		/* (IN) foreground pixel */
	w->core.background_pixel,		/* (IN) background pixel */
	DefaultDepthOfScreen(XtScreen(w)));	/* (IN) pixmap depth */

    w->browse.tree_highlight_pixmap = XCreatePixmapFromBitmapData (
	XtDisplay(w),			/* (IN) display */
	XDefaultRootWindow(XtDisplay(w)),	/* (IN) drawable */
	nav_open_bits,				/* (IN) bitmap data */
	nav_open_width,				/* (IN) pixmap width */
	nav_open_height,			/* (IN) pixmap height */
	w->core.background_pixel,		/* (IN) background pixel */
	w->browse.foreground_pixel,		/* (IN) foreground pixel */
	DefaultDepthOfScreen(XtScreen(w)));	/* (IN) pixmap depth */

    w->browse.tree_nav_close_pixmap = XCreatePixmapFromBitmapData (
	XtDisplay(w),			/* (IN) display */
	XDefaultRootWindow(XtDisplay(w)),	/* (IN) drawable */
	nav_close_bits,				/* (IN) bitmap data */
	nav_close_width,			/* (IN) pixmap width */
	nav_close_height,			/* (IN) pixmap height */
	w->browse.foreground_pixel,		/* (IN) foreground pixel */
	w->core.background_pixel,		/* (IN) background pixel */
	DefaultDepthOfScreen(XtScreen(w)));	/* (IN) pixmap depth */

    w->browse.outline_nav_pixmap = XCreatePixmapFromBitmapData (
	XtDisplay(w),			/* (IN) display */
	XDefaultRootWindow(XtDisplay(w)),	/* (IN) drawable */
	no_nav_bits,				/* (IN) bitmap data */
	no_nav_width,				/* (IN) pixmap width */
	no_nav_height,				/* (IN) pixmap height */
	w->browse.foreground_pixel,		/* (IN) foreground pixel */
	w->core.background_pixel,		/* (IN) background pixel */
	DefaultDepthOfScreen(XtScreen(w)));	/* (IN) pixmap depth */


/*
**  Create corner button to control the nav window
*/
    XtSetArg (arglist[argcnt], XmNactivateCallback, DisplayNavButton_CBstruct);
    XtSetArg (arglist[argcnt+1], XmNarmPixmap , w->browse.tree_highlight_pixmap);

    if (w->browse.display_mode == KdisplayTree)
	 XtSetArg (arglist[argcnt+2], XmNlabelPixmap, w->browse.tree_nav_open_pixmap);
    else XtSetArg (arglist[argcnt+2], XmNlabelPixmap, w->browse.outline_nav_pixmap);

    w->browse.nav_button = (Widget) XmCreatePushButton (w, "NavPb", arglist, argcnt+3);

    if (w->browse.display_mode != KdisplayTree)
	XtSetSensitive(w->browse.nav_button,FALSE);

    XtManageChild (w->browse.nav_button);

}

/*
**  Routine:  DisplaySetApplDragging
**
**  This routine is called in response to the application calling the routine
**  SetApplDragging.  The display module must turn off or on any
**  subwidgets.
*/

void DisplaySetApplDragging (w, value)

  dns_widget w;
  int value;

{
/*
**  Dispatch to the proper routine
*/
    switch (w->browse.display_mode)
      {
        case KdisplayOutline: OutlineSetApplDragging (w, value);  break;
      }
}

/*
**  Routine:  DisplayChangeMode
**
**  This routine is called when the display_mode resource is changed.  It allows
**  the new and old display routines to do any set-up or clean-up necessary when
**  the display mode is changed.
**
**  The return value is whether or not to redisplay.
*/

int DisplayChangeMode (oldw, w)

  dns_widget oldw, w;

{
/*
**  Local data
*/
   int i;
   EntryPtr entry;
   Widget rhs_scroll_window;
   Dimension rhs_width;


/*
**  Always cause each entry to be adjusted so that it will look correct.
*/
    for (i = 1; i <= w->browse.num_entries; i++) {
	entry = StructGetEntryPtr (w, i);
      	entry->height_adjusted = FALSE;
	}


/*
**  Set the max width field to zero so that the horizontal scroll bars will
**  be accurate.
*/
    w->browse.max_width = 0;


/*
**  If one of the two modes is tree, then set other things...
*/
    if ((oldw->browse.display_mode == KdisplayTree) || (w->browse.display_mode == KdisplayTree))
       {
         w->browse.display_changed = TRUE;
         w->browse.display_count = 0;
         w->browse.display_line_y = 0;
         CLEAR_SCREEN(w);
       }


/*
**  The right hand side scroll window is the parent of
**  the right hand side window widget.
*/
    rhs_scroll_window = XtParent (w->browse.rhs_window_widget);


/*
**  Call different procedures based on the old mode.
*/
    switch (oldw->browse.display_mode)

      {

        case KdisplayOutline : OutlineChangeMode (w);
				 break;
      }


/*
**  Now for the entering side...
*/
    switch (w->browse.display_mode)

      {

        case KdisplayOutline : OutlineChangeMode (w);
				 break;
      }


/*
**  Return false meaning not redisplay.
*/
    return FALSE;
}

/*
**  This routine will redisplay an entry in the widget window.  The
**  components drawn into the window will depend upon whether the RHS window
**  is mapped or not.
*/

void DisplayDrawEntry (w, wid, entry, basex, basey)

  dns_widget w;	/*  Widget */
  Window wid;		/* drawable to recieve output */
  EntryPtr entry;	/* Entry to draw */
  int basex, basey;	/* x,y location to draw entry */

{
/*
**  Local data declarations
*/
    CompPtr comp;
    XFontStruct *compfont;
    XTextItem item[5];
    GC gc;
    int compx, compy, comp_number, max_comp_number, itemcount;


/*
**  If we are not yet realized, then leave.
*/
    if (!XtIsRealized (w)) return;


/*
**  Default the GC to either grayed or normal
*/
    if ((entry->grayed) || (XtIsSensitive(w) == FALSE))
         gc = w->browse.grey_gc;
    else gc = w->browse.foreground_gc;


/*
**  Change the GC if it is selected and we own the global selections.
*/
    if ((entry->selected) && (w->browse.show_selections))
       gc = w->browse.background_gc;


/*
**  Draw each component of the entry.  The x and y coordinates are computed
**  by this routine.  The max comp number is set depending on whether or not
**  there is a left hand side and what happens to right hand side components
**  if there is no right.
*/
    comp_number = 1;
    max_comp_number = entry->num_components;

    if ((w->browse.display_mode == KdisplayColumns) || w->browse.rhs_components_unmapped)
       if (w->browse.start_column_component != 0)
           max_comp_number = w->browse.start_column_component - 1;


/*
**  If max_comp_number is less than the real number, then reduce the count
*/
    if (max_comp_number > entry->num_components)
        max_comp_number = entry->num_components;


/*
**  Loop through the components
*/
    while (comp_number <= max_comp_number)

        {
          /*
          **  Get the component
          */
	  comp = &entry->entrycompPtr[comp_number-1];


	  /*
	  **  If this component is hidden, don't draw it
	  */
	  if ((comp->hidden_mode != w->browse.display_mode) &&
              (comp->hidden_mode != KdisplayAllModes))
	      {
	      /*
	      **  Determine the component real x and real y
	      */
		  compx = basex + comp->x;
		  compy = basey + comp->y;


	      /*
	      **  Clear the bounding box of the component if tree is user
	      **  defined.
	      */
		  if ((w->browse.display_mode == KdisplayTree) &&
		      (w->browse.tree_style == KuserDefinedTree) &&
		      (!entry->selected))
		    {
		      XFillRectangle (XtDisplay(w), wid,
			    w->browse.background_gc,
			    (Position)compx - 1,
			    (Position)compy - 1,
			    (Dimension)comp->width + 2,
			    (Dimension)comp->height + 2);
		    }


	      /*
	      **  If we are drawing a pixmap...
	      */
		  if (comp->type == KcompPixmap)
		     XCopyArea (XtDisplay(w),comp->var.is_pixmap.pixmap,wid,gc,0,0,comp->width,comp->height,compx,compy);


	      /*
	      **  If we are drawing a subwidget, then map it into place
	      */
		  if (comp->type == KcompWidget)
	             SHOW_WIDGET (w, comp->var.is_widget.readwrite_text, compx, compy, FALSE);


	      /*
	      **  If we are drawing normal text...
	      */
		  if (comp->type == KcompText)
		     {
		       /*
		       **  Get and set the font if necessary
		       */
			   compfont = DisplayFont (w, entry, comp);
			   XSetFont (XtDisplay(w), gc, compfont->fid);


		       /*
		       **  Build the text item structure for the first component
		       */
			   item[0].chars  = comp->var.is_text.text;
			   item[0].nchars = comp->var.is_text.text_len;
			   item[0].delta  = 0;
			   item[0].font   = 0;

			   itemcount = 1;


		       /*
		       **  Adjust the y to the baseline
		       */
			   compy = compy + compfont->max_bounds.ascent;


		       /*
		       **  Draw the text
		       */
			   XDrawText (XtDisplay(w), wid, gc, compx, compy, item, itemcount);
		     }

#ifdef DECW_V2
	      /*
	      ** If we are drawing a compound String...
	      */
  		  if (comp->type == KcompCString) {
		     XmStringDraw (XtDisplay(w), wid,
			DisplayFontList (w, entry, comp),
			comp->var.is_cstring.cstring, gc, compx, compy, 0, 0, 0, 0);
		     }
#endif
		}


          /*
          **  Increment to the next component
          */
          comp_number++;
        }
}

/*
**  This routine will redisplay the rhs window.  The components drawn into
**  the window will depend upon whether the RHS window is mapped or not.
**
**  The X coordinates of the components are computed dynamically which will
**  prevent readjusting every entry when the maximum column width changes.
**
**  This routine now will clip components if they do not fit...
*/

void DisplayDrawColumns (w, entry, basex, basey, invert_option)

  dns_widget w;
  EntryPtr entry;
  int basex, basey, invert_option;

{
/*
**  Local data declarations
*/
    CompPtr comp;
    XFontStruct *compfont;
    XTextItem item[5];
    GC gc;
    int compx, compy, comp_number, max_comp_number, itemcount, column_x;


/*
**  If we are not yet realized, then leave.
*/
    if (!XtIsRealized (w)) return;


/*
**  If there are really no rhs components, then return
*/
    if (w->browse.start_column_component == 0) return;


/*
**  Draw each component of the entry.  The x and y coordinates are computed
**  by this routine.  The max comp number is set depending on whether or not
**  there is a left hand side and what happens to right hand side components
**  if there is no right.
*/
    comp_number = w->browse.start_column_component;
    max_comp_number = entry->num_components;


/*
**  The column_x coordinate starts at 1/2 of the default spacing
*/
    column_x = w->browse.default_spacing / 2;


/*
**  Loop through the components
*/
    while (comp_number <= max_comp_number)

        {
          /*
          **  Get the component
          */
	  comp = &entry->entrycompPtr[comp_number-1];


	  /*
	  **  If this component is hidden, don't draw it
	  */
	  if ((comp->hidden_mode != w->browse.display_mode) &&
              (comp->hidden_mode != KdisplayAllModes))
	      {
	      /*
	      **  Local boolean indicating that column clip used.
	      */
	          Boolean column_clipped = FALSE;
                  XRectangle clip;


	      /*
	      **  Determine the component real x and real y
	      */
		  compx = basex + column_x;
		  compy = basey + comp->y;


	      /*
	      **  Default the GC to either grayed or normal
	      */
		  if ((entry->grayed) || (XtIsSensitive(w) == FALSE))
		       gc = w->browse.grey_gc;
		  else gc = w->browse.foreground_gc;


	      /*
	      **  Change the GC if it is inverted
	      */
		  if ((invert_option == comp_number) || (invert_option == 99))
  	             gc = w->browse.background_gc;


	      /*
	      **  Draw lines between the components if called for.  We do nothing
	      **  on the first component, but for the rest we will draw the line
	      **  at the X coordinate minus 1/2 of the default spacing value.
	      */
	          if (w->browse.column_lines)
	             if (comp_number > w->browse.start_column_component)
                        XDrawLine (XtDisplay(w),
	                           XtWindow(w->browse.rhs_window_widget),
	                           w->browse.foreground_gc,
                                   compx - (w->browse.default_spacing / 2),
	                           w->browse.margin_height,
			           compx - (w->browse.default_spacing / 2),
		                   XtHeight(w->browse.rhs_window_widget) - w->browse.margin_height);


	      /*
	      **  If the component width is greater than the column width
	      **  the set the clip and record the fact to remove it.
	      */
	          if (comp->width > w->browse.column_widths[comp_number-1])
		     {
		       column_clipped = TRUE;

		       clip.x      = compx;
		       clip.y      = compy;
		       clip.width  = w->browse.column_widths[comp_number-1];
		       clip.height = comp->height;

		       XSetClipRectangles (XtDisplay(w), gc, 0, 0, &clip, 1, Unsorted);
		     }


	      /*
	      **  If we are drawing a pixmap...
	      */
		  if (comp->type == KcompPixmap)
		     XCopyArea (XtDisplay(w),comp->var.is_pixmap.pixmap,
				XtWindow(w->browse.rhs_window_widget),gc,
				0,0,comp->width,comp->height,compx,compy);


	      /*
	      **  If we are drawing a subwidget, then map it into place
	      */
		  if (comp->type == KcompWidget)
		     SHOW_WIDGET (w, comp->var.is_widget.readwrite_text, compx, compy, FALSE);


	      /*
	      **  If we are drawing normal text...
	      */
		  if (comp->type == KcompText)
		     {
		       /*
		       **  Get and set the font if necessary
		       */
			   compfont = DisplayFont (w, entry, comp);
			   XSetFont (XtDisplay(w), gc, compfont->fid);


		       /*
		       **  Build the text item structure for the first component
		       */
			   item[0].chars  = comp->var.is_text.text;
			   item[0].nchars = comp->var.is_text.text_len;
			   item[0].delta  = 0;
			   item[0].font   = 0;

			   itemcount = 1;


		       /*
		       **  Adjust the y to the baseline
		       */
			   compy = compy + compfont->max_bounds.ascent;


		       /*
		       **  If this was right justified text, then adjust
		       **  the X value to maintain that.  We have the width
		       **  of the column and the width of the text.
		       */
			   if (comp->orig_rj)
			      compx = compx + w->browse.column_widths[comp_number-1] - comp->width;


		       /*
		       **  Draw the text
		       */
			   XDrawText (XtDisplay(w), XtWindow (w->browse.rhs_window_widget), gc, compx, compy, item, itemcount);
		     }

#ifdef DECW_V2
	      /*
	      ** If we are drawing a compound String...
	      */
  		  if (comp->type == KcompCString) {
		     XmStringDraw (XtDisplay(w), XtWindow (w->browse.rhs_window_widget),
			DisplayFontList (w, entry, comp),
			comp->var.is_cstring.cstring, gc, compx, compy, 0, 0, 0, 0);
		     }
#endif

	      /*
	      **  Reset the clip if necessary.
	      */
	          if (column_clipped)
		     {
		      /*
		      **  Remove or set the clips
		      */
			  if (w->browse.clip_count)
                               XSetClipRectangles (XtDisplay(w), gc, 0, 0, w->browse.clips, w->browse.clip_count, Unsorted);
			  else XSetClipMask (XtDisplay(w), gc, None);

		     }
	      }


          /*
          **  Increment to the next component and the next column
          */
	  column_x = column_x + w->browse.column_widths[comp_number-1] + w->browse.default_spacing;
          comp_number++;
        }
}

/*
**  This routine will draw the inverted boxes for the rhs window.
*/

void DisplayDrawColumnsBox (w, entry, basex, basey, invert_option)

  dns_widget w;
  EntryPtr entry;
  int basex, basey, invert_option;

{
/*
**  Local data declarations
*/
    CompPtr comp;
    GC gc;
    int compx, comp_number, max_comp_number, column_x = 0;
    Dimension box_width;


/*
**  If we are not yet realized, then leave.
*/
    if (!XtIsRealized (w)) return;


/*
**  If there are really no rhs components, then return
*/
    if (w->browse.start_column_component == 0) return;


/*
**  If nothing needs boxes, then leave now.
*/
    if (invert_option == 0) return;


/*
**  Default the GC to either grayed or normal
*/
    if ((entry->grayed) || (XtIsSensitive(w) == FALSE))
         gc = w->browse.grey_gc;
    else gc = w->browse.foreground_gc;


/*
**  Draw each component of the entry.  The x and y coordinates are computed
**  by this routine.  The max comp number is set depending on whether or not
**  there is a left hand side and what happens to right hand side components
**  if there is no right.
*/
    comp_number = w->browse.start_column_component;
    max_comp_number = entry->num_components;


/*
**  Loop through the components
*/
    while (comp_number <= max_comp_number)

        {
          /*
          **  Get the component
          */
	  comp = &entry->entrycompPtr[comp_number-1];


	  /*
	  **  If this component is hidden, don't draw it
	  */
	  if ((comp->hidden_mode != w->browse.display_mode) &&
              (comp->hidden_mode != KdisplayAllModes))
	      {
	      /*
	      **  Determine the component real x value
	      */
		  compx = basex + column_x;


	      /*
	      **  Determine the width of the rectangle
	      */
	          if (comp_number != max_comp_number)
		       box_width = w->browse.column_widths[comp_number-1] + w->browse.default_spacing;
		  else box_width = XtWidth (w->browse.rhs_window_widget) - compx;


	      /*
	      **  Invert the component if necessary
	      */
		  if ((invert_option == comp_number) || (invert_option == 99))
		     {
		      /*
		      **  Draw the rectangle
		      */
			  XFillRectangle (XtDisplay(w), XtWindow(w->browse.rhs_window_widget),
					  gc, (Position)compx, (Position)basey,
					  box_width, (Dimension)entry->height);
		     }
	      }


          /*
          **  Increment to the next component and next column
          */
	  column_x = column_x + w->browse.column_widths[comp_number-1] + w->browse.default_spacing;
          comp_number++;
        }
}

/*
**  Routine that resolves graphic exposure events on scroll operations.  This
**  routine assumes that all entries are valid and they just need repaired to
**  solve graphic exposure problems.  This routine returns when all graphic
**  exposures are resolved.
**
**  It is assumed that the caller has deposited a window identifier into the
**  clips_window field of the widget.
*/

void DisplayGraphicsExpose (w)

  dns_widget w;

{
/*
**  Routine data declarations
*/
    XEvent new_event;


/*
**  If we are not yet realized, then leave.
*/
    if (!XtIsRealized (w)) return;


/*
**  Continue looping until we do not get a new exposure event.  We will be
**  stacking up the rectangles and marking the invalid bits in the entries
**  that need to be redisplayed.
*/
    while (XCheckTypedWindowEvent(XtDisplay(w), w->browse.clips_window, GraphicsExpose, &new_event))

       {
         /*
         **  Add the clipping rectangle to our array of clipping rectangles
         */
	 if (w->browse.clip_count < max_clips)
	    {
             w->browse.clips[w->browse.clip_count].x      = new_event.xgraphicsexpose.x;
             w->browse.clips[w->browse.clip_count].y      = new_event.xgraphicsexpose.y;
             w->browse.clips[w->browse.clip_count].width  = new_event.xgraphicsexpose.width;
             w->browse.clips[w->browse.clip_count].height = new_event.xgraphicsexpose.height;
             w->browse.clip_count++;
	    }
	  else
	    {
	     w->browse.clip_count = 0;
	     w->browse.refresh_all = TRUE;
	    }
       }
}

/*
**  Routine that resolves graphic exposure events on scroll operations.  This
**  routine assumes that all entries are valid and they just need repaired to
**  solve graphic exposure problems.  This routine returns when all graphic
**  exposures are resolved.
*/

void DisplayExpose (w, delta_x, delta_y)

  dns_widget w;
  int delta_x, delta_y;

{
/*
**  Routine data declarations
*/
    XEvent new_event;


/*
**  If we are not yet realized, then leave.
*/
    if (!XtIsRealized (w)) return;


/*
**  Continue looping until we do not get a new exposure event.  We will be
**  stacking up the rectangles and marking the invalid bits in the entries
**  that need to be redisplayed.
*/
    while (XCheckTypedWindowEvent(XtDisplay(w), w->browse.clips_window, Expose, &new_event))

       {
         /*
         **  Add the clipping rectangle to our array of clipping rectangles
         */
	 if (w->browse.clip_count < max_clips)
	    {
             w->browse.clips[w->browse.clip_count].x      = new_event.xexpose.x + delta_x;
             w->browse.clips[w->browse.clip_count].y      = new_event.xexpose.y + delta_y;
             w->browse.clips[w->browse.clip_count].width  = new_event.xexpose.width;
             w->browse.clips[w->browse.clip_count].height = new_event.xexpose.height;
             w->browse.clip_count++;
	    }
	  else
	    {
	     w->browse.clip_count = 0;
	     w->browse.refresh_all = TRUE;
	    }
       }
}

/*
**  This common display routine determines the font to use for a particular component
*/

XFontStruct *DisplayFont (w, entry, comp)

  dns_widget w;
  EntryPtr entry;
  CompPtr comp;

{
/*
**  Local data declarations
*/
    XFontStruct *candidate = (XFontStruct *) NULL;


/*
**  If a font is specified in the component, then that is the one to use
*/
    if (comp->var.is_text.font != (XFontStruct *) NULL) return (comp->var.is_text.font);


/*
**  Determine a potential default font according to the level number
*/
    switch (entry->level)
       {
         case 0: candidate = w->browse.level0_font;  break;
         case 1: candidate = w->browse.level1_font;  break;
         case 2: candidate = w->browse.level2_font;  break;
         case 3: candidate = w->browse.level3_font;  break;
         case 4: candidate = w->browse.level4_font;  break;
       }


/*
**  If the candidate font is still null, then set it to be the default
*/
    if (candidate == (XFontStruct *) NULL) candidate = w->browse.default_font;


/*
**  Return the candidate
*/
    return (candidate);
}



/*
**  This routine is called by DisplayAdjustHeight whenever a line of components
**  is desired.
**
**  This routine calculates the proper width and height of an entry based on the
**  current components.  If the application supplies a Y value for a component
**  that value is used.
**
**  This routine ensures that components within a given line do not overlap
**  other components within that line.  If they are found to overlap, then the
**  component is pushed out enough to leave a 'default_spacing' gap.  The effect
**  will be that it is a ripple effect.  The second component may move while
**  the third may not because moving the second did not force it into the third.
*/


static void LclDisplayLineData (w, entry, comps, first_comp, last_comp, y_plus)

  dns_widget w;
  EntryPtr entry;
  CompPtr comps [max_comps];
  int first_comp, last_comp;
  Dimension *y_plus;

{
/*
**  Local data declarations
*/
    Dimension below, max_below = 0, max_above = 0, max_height = 0;
    Boolean do_columns;
    int i;


/*
**  Set the boolean do_columns based on the display mode and whether this is
**  the first line in an entry.
*/
    do_columns = (   (first_comp == 1)
                  && (w->browse.display_mode == KdisplayColumns)
                  && (w->browse.start_column_component != 0));


/*
**  Compute the baseline y position and maximum descent values for each of the
**  components.
*/
    for (i = first_comp;  i <= last_comp;  i++)
      {
	DisplayCompData(w, entry, comps[i], &below);

	if (comps[i]->type == KcompText)
	    {
	     if (max_above < comps[i]->height) max_above = comps[i]->height;
	     if (max_below < below) max_below = below;
	    }

	if (comps[i]->type != KcompText)
	    {
	     if (max_height < comps[i]->height) max_height = comps[i]->height;
	    }
      }


/*
**  If this is the first line of an entry and we are in columns mode,
**  then use the columns in the max_above and max_below calculations.
*/
    if (do_columns)
       for (i = w->browse.start_column_component;  i <= entry->num_components;  i++)
           {
	     DisplayCompData(w, entry, comps[i], &below);

	     if (comps[i]->type == KcompText)
		 {
		  if (max_above < comps[i]->height) max_above = comps[i]->height;
		  if (max_below < below) max_below = below;
		 }

	     if (comps[i]->type != KcompText)
		 {
		  if (max_height < comps[i]->height) max_height = comps[i]->height;
		 }
           }


/*
**  Now we have the maximum height of things that are to be centered
**  along with the maximum ascent/descent of text strings that are to
**  be lined up.  If the ascent/descents are more than the max_height
**  then make that the max_height.
*/
    if (max_height < max_above + max_below)
        max_height = max_above + max_below;


/*
**  If max_height is greater than max_above plus the max_below, then
**  adjust the max_above value to center text within the pixmap.
*/
    if (max_height > max_above + max_below)
       max_above = max_above + ((max_height - (max_above + max_below)) / 2);


/*
**  If application did not supply a Y value, align each y value
**  according to their ascent values.  Do this for columns also
**  if it is appropriate.
*/
    for (i = first_comp;  i <= last_comp;  i++)
        if (comps[i]->y == 0)
	   {
	     if (comps[i]->type == KcompText)
                  comps[i]->y = (max_above - comps[i]->height) + *y_plus;
	     else comps[i]->y = ((max_height - comps[i]->height) / 2) + *y_plus;
	   }

    if (do_columns)
       for (i = w->browse.start_column_component;  i <= entry->num_components;  i++)
           if (comps[i]->y == 0)
	      {
	        if (comps[i]->type == KcompText)
                     comps[i]->y = (max_above - comps[i]->height) + *y_plus;
	        else comps[i]->y = ((max_height - comps[i]->height) / 2) + *y_plus;
	      }


/*
**  Do the overlap detection pass comparing component X values and widths.  Note
**  that this is only necessary when there is more than one component.  Note also
**  that is two components are up against each other, but not overlapping that
**  this will leave it alone.
*/
    for (i = first_comp + 1;  i <= last_comp;  i++)
        if (comps[i]->x < comps[i-1]->x + comps[i-1]->width)
            comps[i]->x = comps[i-1]->x + comps[i-1]->width + w->browse.default_spacing;


/*
**  Pass through the components setting the column width field on the widget
**  if it is zero.
*/
    if (do_columns)
       {
         Boolean redisplay = FALSE;
         for (i = w->browse.start_column_component;  i <= entry->num_components;  i++)
	     {
  	       /*
	       **  If there is none at all, then set it to this width
	       */
                   if (w->browse.column_widths [i-1] == 0)
	              {
                        w->browse.column_widths [i-1] = comps[i]->width;
                        w->browse.rhs_max_width = w->browse.rhs_max_width + w->browse.default_spacing + comps[i]->width;
	   	      }


               /*
	       **  If the old one is too small and it was calculated,
	       **  then do the computations and set a flag to redisplay all entries.
	       */
		   if (w->browse.column_width_set [i-1] == FALSE)
	              if (w->browse.column_widths [i-1] < comps[i]->width)
	  	         {
		           w->browse.rhs_max_width = w->browse.rhs_max_width - w->browse.column_widths [i-1];
                           w->browse.column_widths [i-1] = comps[i]->width;
		           w->browse.rhs_max_width = w->browse.rhs_max_width + w->browse.column_widths [i-1];
		           redisplay = TRUE;
		         }
             }

         if (redisplay)
            XClearArea (XtDisplay(w), XtWindow(w->browse.rhs_window_widget), 0, 0,
	            XtWidth(w->browse.rhs_window_widget), XtHeight(w->browse.rhs_window_widget), TRUE);
       }


/*
**  Use the last component for width calculations
*/
    if (entry->width < comps[last_comp]->width + comps[last_comp]->x + 1)
        entry->width = comps[last_comp]->width + comps[last_comp]->x + 1;


/*
**  Set the y_plus to include current line height.
*/
    *y_plus = *y_plus + max_height;
}

/*
**  This routine is called by the STRUCTURE module whenever a valid entry
**  is desired.   It is only called if 'height_adjusted' is false.
*/

void DisplayAdjustEntryHeight (w, entry)

  dns_widget w;
  EntryPtr entry;

{
/*
**  Local data declarations
*/
    int i, first_comp = 1, last_comp_x = -1, num_components;
    Dimension y_plus = 0;
    CompPtr comps [max_comps];


/*
**  Clear the entry width and height
*/
    entry->width  = 0;
    entry->height = 0;


/*
**  Initialize all components of the entry to their original dimensions.
*/
    for (i = 1;  i <= entry->num_components;  i++)
	{
	 /*
	 **  Get the address of the widget components locally
	 */
             comps[i] = &entry->entrycompPtr[i-1];


	 /*
	 **  Fill in working fields from original values
	 */
	     comps[i]->x      = comps[i]->orig_x;
	     comps[i]->y      = comps[i]->orig_y;
	     comps[i]->width  = comps[i]->orig_width;
	     comps[i]->height = comps[i]->orig_height;
	}


/*
**  Set the number of components locally.  This is for the current number of
**  components in the left hand side.  The lower level line level procedure
**  will handle the left hand side components when adjusting the first line.
*/
    if ((w->browse.display_mode == KdisplayColumns) || w->browse.rhs_components_unmapped)
         num_components = w->browse.start_column_component - 1;
    else num_components = entry->num_components;


/*
**  If there are really no rhs components, then restore the old value
*/
    if (num_components > entry->num_components)
       num_components = entry->num_components;


/*
**  Process the first lines of an entry.  The last line (and in most cases the
**  only line) will not be processed in this loop.
*/
    for (i = 1;  i <= num_components;  i++)
	{
	  /*
	  **  If hidden in this display_mode, then set it's x,y position
	  **  to zero to keep it from contributing to the entry width.
	  **  Copy the X position from the previous entry so that overlapping
	  **  will still be eliminated.
	  */
	  if ((comps[i]->hidden_mode == w->browse.display_mode) || (comps[i]->hidden_mode == KdisplayAllModes))
	    {
	      if (i == 1) comps[i]->x = 0;
	      else comps[i]->x = comps[i-1]->x;

	      comps[i]->y = 0;
	    }


	  /*
	  **  Process all components seen thus far as a group if the current
	  **  component is not hidden in this mode.
	  */
	  if ((comps[i]->x <= last_comp_x) &&
	      (comps[i]->hidden_mode != w->browse.display_mode) &&
	      (comps[i]->hidden_mode != KdisplayAllModes))

	     {
	       LclDisplayLineData (w, entry, comps, first_comp, i-1, &y_plus);
	       first_comp = i;
	     }

	  last_comp_x = comps[i]->x;
	}


/*
**  Figure out the last line.  Add the height of that line into the height
**  of the entry.  Also see if the last line is the longest line.
*/
    LclDisplayLineData (w, entry, comps, first_comp, num_components, &y_plus);


/*
**  The y_plus value contains the entry height.
*/
    entry->height = y_plus;


/*
**  If the user said that it was taller than that, then trust them
*/
    if (entry->height < entry->orig_height)
        entry->height = entry->orig_height;


/*
**  Reflect the fact that the adjustment has been made
*/
    entry->height_adjusted = TRUE;
}

/*
**  This routine determines the width and height (both above and below the
**  baseline) of a component.  The above value is put into the component
**  height.  The width is put into the component width.
*/

void DisplayCompData (w, entry, comp, below)

  dns_widget w;
  EntryPtr entry;
  CompPtr comp;
  Dimension *below;

{
/*
**  Local data declarations
*/
    XFontStruct *compfont;


/*
**  If display_mode is tree then make invisible components width and height 0
*/
    if ((comp->hidden_mode == w->browse.display_mode) ||
        (comp->hidden_mode == KdisplayAllModes)) {
	comp->height = 0;
	comp->width  = 0;
	*below = 0;
	return;
	}


/*
**  Compute the baseline y position and maximum descent values for each of the
**  components.
*/
    switch (comp->type)
	{
	case KcompText:
		compfont = DisplayFont (w, entry, comp);
		comp->height = compfont->ascent;
		*below = compfont->descent;

		/*
		**  If we are suppose to truncate strings, or the field width is
		**  zero, then use the with of the string as the field width.
		*/
		if ((!w->browse.truncate_strings) || (comp->width == 0))
		   {
		   comp->var.is_text.text_len = strlen(comp->var.is_text.text);
		   comp->width = XTextWidth(compfont, comp->var.is_text.text, (int)comp->var.is_text.text_len);
		   }
		else
		   {
		   int len = strlen(comp->var.is_text.text);

		   /*
		   **	Keep incrementing the length until it no longer fits
		   **	into the width specified.
		   */
		   for (comp->var.is_text.text_len = 0; comp->var.is_text.text_len <= len; comp->var.is_text.text_len++)
			{
			if (comp->width < XTextWidth(compfont, comp->var.is_text.text, comp->var.is_text.text_len))
			    break;
			}
		   }
		break;

	case KcompText16:
	    compfont = DisplayFont (w, entry, comp);
	    comp->height = compfont->ascent;
	    *below = compfont->descent;

	    /*
	    **  If we are suppose to truncate strings, or the field width is
	    **  zero, then use the with of the string as the field width.
	    */
	    if ((!w->browse.truncate_strings) || (comp->width == 0))
	       {
	       comp->var.is_text.text_len = strlen(comp->var.is_text.text);
	       comp->width = XTextWidth16(compfont, (XChar2b *)comp->var.is_text.text, comp->var.is_text.text_len);
	       }
	    else
	       {
	       int len = strlen(comp->var.is_text.text);

	       /*
	       **  Keep incrementing the length until it no longer fits
	       **  into the width specified.
	       */
	       for (comp->var.is_text.text_len = 0; comp->var.is_text.text_len <= len; comp->var.is_text.text_len++)
		    {
		    if (comp->width < XTextWidth(compfont, comp->var.is_text.text, comp->var.is_text.text_len))
			break;
		    }
	       }
	    break;

	case KcompPixmap: *below = 0;
			      break;

	case KcompWidget: comp->height = XtHeight (comp->var.is_widget.readwrite_text);
			     comp->width = XtWidth(comp->var.is_widget.readwrite_text);
			    *below = 0;
			     break;

#ifdef DECW_V2
	case KcompCString: *below = 0;
			    compfont = DisplayFontList (w, entry, comp);
			    XmStringExtent(compfont, comp->var.is_cstring.cstring,
				&comp->width, &comp->height);
#endif
	}
}

/*
**  Routine:  DisplayNavButton
**
**  This routine is called in response to a push of the Nav button
*/

void DisplayNavButton (wi)

  Widget wi;

{
/*
**  Determine the  widget assocatiated with this button.
*/
    dns_widget w = StructFindWidget (wi);


} /* DisplayNavButton */

/*
**  Routine:  DisplayAdjustEntrySize
**
**  This routine is called during geometry requests so that the display
**  routines can accomodate the new entry size.
*/

void DisplayAdjustEntrySize (w, entry_number)

  dns_widget w;
  int entry_number;

{
/*
**  Dispatch to the appropriate routine
*/
    switch (w->browse.display_mode)
      {
        case KdisplayOutline: OutlineAdjustEntrySize (w, entry_number);  break;
      }
}

/*
**  Routine:  SetEntryPosition
**
**  This routine sets the position of an entry in user define tree mode.  If
**  the tree style is not userDefined then this routine does nothing.
*/

void SetEntryPosition (w, entry_number, window_mode, x, y)

    dns_widget w;
    int entry_number;
    Boolean window_mode;
    int x, y;

{
/*
**  Local Variables
*/
    EntryPtr entry;
    int new_x, new_y;
    int old_x, old_y;
    int width, height;


/*
**  If this is not a user defined tree just return
*/
    if (w->browse.display_mode != KdisplayTree) return;
    if (w->browse.tree_style != KuserDefinedTree) return;


/*
**  If position is in window mode, then change it to world coordinates
*/
    if (window_mode == TRUE) {
	new_x = x;
	new_y = y;
	x = X_TO_WORLD(w, x);
	y = Y_TO_WORLD(w, y);
	}
    else {
	new_x = X_TO_WINDOW(w, x);
	new_y = Y_TO_WINDOW(w, y);
	}


/*
**  Get the entry and fill in the specified position
*/
    entry = StructGetValidEntryPtr(w, entry_number);
    old_x = X_TO_WINDOW(w, X_POS(w, entry));
    old_y = Y_TO_WINDOW(w, Y_POS(w, entry));
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    entry->px = x;
    entry->py = y;
    width = entry->width + 1;
    height = entry->height + 1;


/*
**  Set the window of interest for clip rectangles
*/
    w->browse.clips_window = XtWindow (w->browse.lhs_window_widget);


/*
**  TBS -- We need a concept of keeping these clips around until we get enabled again!
**  for now clear the old area.
*/
    XClearArea (XtDisplay(w),
	        w->browse.clips_window,
		(Position)old_x,
		(Position)old_y,
		(Dimension)width,
		(Dimension)height,
		FALSE);


/*
**  Set the clipping rectangles on both the source and destination
**  of this entry.
*/
    if (w->browse.clip_count < max_clips - 1) {
	w->browse.clips[w->browse.clip_count].x = old_x;
	w->browse.clips[w->browse.clip_count].y = old_y;
	w->browse.clips[w->browse.clip_count].width = width;
	w->browse.clips[w->browse.clip_count].height = height;
	w->browse.clip_count++;
	w->browse.clips[w->browse.clip_count].x = new_x;
	w->browse.clips[w->browse.clip_count].y = new_y;
	w->browse.clips[w->browse.clip_count].width = width;
	w->browse.clips[w->browse.clip_count].height = height;
	w->browse.clip_count++;
	}
    else {
	w->browse.refresh_all = TRUE;
	w->browse.clip_count = 1;
	}


/*
**  Force Nav window update
*/
    w->browse.update_nav_window = TRUE;


} /* SetEntryPosition */

/*
**  Routine:  FlushEntry
**
**  This routine does an append of this entry if appropriate.  It only applies
**  to outline mode.
*/
void FlushEntry (w, entry_number)

    dns_widget w;
    int entry_number;

{
/*
**  Dispatch to the appropriate routine
*/
    switch (w->browse.display_mode)
      {
        case KdisplayColumns: OutlineFlushEntry (w, entry_number);  break;
        case KdisplayOutline: OutlineFlushEntry (w, entry_number);  break;
      }
}

/*
**  Routine:  GetEntryPosition
**
**  This routine sets the position of an entry in user define tree mode.  If
**  the tree style is not userDefined then this routine does nothing.
*/
void GetEntryPosition (w, entry_number, window_mode, x, y)

    dns_widget w;
    int entry_number;
    Boolean window_mode;
    int *x, *y;

{
/*
**  Local Variables
*/
    EntryPtr entry;


/*
**  Get the entry and extract the position info
*/
    entry = StructGetValidEntryPtr(w, entry_number);


/*
**  If Outline mode call local support routine to find the position
*/
    if (w->browse.display_mode != KdisplayTree)
	{
	/*
	**  Determine the offset of the entry
	*/
	int offset = LclOutlineMapNumberToOffset(w, entry_number);


	/*
	**  Return -1 if not visible, or request was for internal coordinates
	*/
	if ((offset == 0) || (window_mode == FALSE))
	    {
	    *x = -1;
	    *y = -1;
	    return;
	    }


	/*
	**  Map that offset into a Y window coordinate, and compute the base x
	**  value based on the entries level number and on whether the
	**  navigation bar is on the left side.
	*/
	*y = LclOutlineMapOffsetToY(w, offset);
	*x = w->browse.margin_width + (entry->level * w->browse.indent_margin) - w->browse.window_basex;
	return;
	}



/*
**  Otherwise, Just use the macros to get position
*/
    *x = X_POS(w, entry);
    *y = Y_POS(w, entry);


/*
**  If position is desired in window mode, then change it to window coordinates
*/
    if (window_mode == TRUE) {
	*y = Y_TO_WINDOW(w, *x);
	*x = X_TO_WINDOW(w, *y);
	}


} /* GetEntryPosition */

/*
**  Routine:  GetTreePosition
**
**  This routine gets the position of the tree in tree mode.  This routine
**  is only valid when the display mode is tree.
*/
void GetTreePosition (w, x, y)

    dns_widget w;
    int *x, *y;

{
/*
**  Return the map position
*/
    *x = w->browse.mapx;
    *y = w->browse.mapy;


} /* GetTreePosition */

/*
**  Routine:  SetTreePosition
**
**  This routine sets the position of the tree in tree mode.  This routine
**  is only valid when the display mode is tree.
*/
void SetTreePosition (w, x, y)

    dns_widget w;
    int x, y;

{
/*
**  Change the tree position
*/
    w->browse.mapx = x;
    w->browse.mapy = y;
    w->browse.refresh_all = TRUE;

} /* SetTreePosition */

/*
** This routine puts the clipping rectangles in the proper form.  It removes
** overlapping clips and may merge some clips together.  In the future it could
** be enhanced to sort the clips to improve performance.  It may return 0 or
** more clips.  If a clip that is the size of the drawing area is found, then
** all of the clips are deleted so the entire display is redrawn.
**
*/
void DisplayMergeClips(w)

    dns_widget w;

{
    int i, j;	/* loop index */
    int min_x, min_y, max_y, max_x;


/*
**  Loop through merging overlapping clips
*/
    for (i = 0; i < w->browse.clip_count; i++)
	{
	/*
	**  Skip deleted clips
	*/
	if (w->browse.clips[i].width == 0) continue;


	/*
	**  Compare this clip against all others looking for overlap
	*/
	for (j = 0; j < (w->browse.clip_count - 1); j++)
	    {
	    /*
	    **  If i and j are the same clip, skip it
	    */
	    if (i == j) continue;


	    /*
	    **  Skip deleted clips
	    */
	    if (w->browse.clips[j].width == 0) continue;


	    /*
	    **  ignore this one if they don't overlap
	    */
	    if ((w->browse.clips[i].x + w->browse.clips[i].width ) < (w->browse.clips[j].x)) continue;
	    if ((w->browse.clips[i].y + w->browse.clips[i].height) < (w->browse.clips[j].y)) continue;
	    if ((w->browse.clips[j].x + w->browse.clips[j].width ) < (w->browse.clips[i].x)) continue;
	    if ((w->browse.clips[j].y + w->browse.clips[j].height) < (w->browse.clips[i].y)) continue;


	    /*
	    **  They do overlap, so figure out the bounding box
	    */
	    if (w->browse.clips[i].y < w->browse.clips[j].y)
		 min_y = w->browse.clips[i].y;
	    else min_y = w->browse.clips[j].y;


	    if (w->browse.clips[i].x < w->browse.clips[j].x)
		 min_x = w->browse.clips[i].x;
	    else min_x = w->browse.clips[j].x;


	    if ((w->browse.clips[i].y + w->browse.clips[i].height) > (w->browse.clips[j].y + w->browse.clips[j].height))
		 max_y = w->browse.clips[i].y + w->browse.clips[i].height;
	    else max_y = w->browse.clips[j].y + w->browse.clips[j].height;


	    if ((w->browse.clips[i].x + w->browse.clips[i].width) > (w->browse.clips[j].x + w->browse.clips[j].width))
		 max_x = w->browse.clips[i].x + w->browse.clips[i].width;
	    else max_x = w->browse.clips[j].x + w->browse.clips[j].width;


	    /*
	    **  Store the bounding box of the two clips as the new clip
	    **  and mark the other as deleted
	    */
	    w->browse.clips[j].x = min_x;
	    w->browse.clips[j].y = min_y;
	    w->browse.clips[j].width  = max_x - min_x;
	    w->browse.clips[j].height = max_y - min_y;
	    w->browse.clips[i].width  = 0;
	    }
	}


/*
**  Second pass:  Loop through the clips deleting the holes made by the first
**  pass.
*/
    j = 0;
    for (i = 0; i < w->browse.clip_count; i++)
	{
	/*
	**  Skip deleted clips
	*/
	if (w->browse.clips[i].width == 0) continue;


	/*
	**  If we need to move the clip, do so
	*/
	if (i != j)
	    w->browse.clips[j] = w->browse.clips[i];


	/*
	**  Increment the number of clips
	*/
	j++;
	}


/*
**  Reset the number of clips
*/
    w->browse.clip_count = j;


} /* DisplayMergeClips */

/*
**  This routine either turns on or turns off the watch cursor.
*/

void DisplaySetWatch (w, flag)

  dns_widget w;
  Boolean flag;

{
/*
**  If we are not yet realized, then leave.
*/
    if (!XtIsRealized (w)) return;


/*
**  If the flag is TRUE, then turn on the watch and flush it out.
*/
    if (flag)
         XDefineCursor   (XtDisplay(w), XtWindow(w), w->browse.watch_cursor);
    else XUndefineCursor (XtDisplay(w), XtWindow(w));


/*
**  Flush out the request
*/
    XFlush (XtDisplay(w));
}



/*
**  This common display routine determines the font to use for a particular component
*/

XmFontList DisplayFontList (w, entry, comp)

  dns_widget w;
  EntryPtr entry;
  CompPtr comp;

{
/*
**  Local data declarations
*/
    XmFontList candidate = (XmFontList)NULL;


/*
**  If a font is specified in the component, then that is the one to use
*/
    if (comp->var.is_cstring.font_list != (XmFontList) NULL) return (comp->var.is_cstring.font_list);


/*
**  Determine a potential default font according to the level number
*/
    switch (entry->level)
       {
         case 0: candidate = w->browse.level0_fontlist;  break;
         case 1: candidate = w->browse.level1_fontlist;  break;
         case 2: candidate = w->browse.level2_fontlist;  break;
         case 3: candidate = w->browse.level3_fontlist;  break;
         case 4: candidate = w->browse.level4_fontlist;  break;
       }


/*
**  If the candidate font is still null, then set it to be the default
*/
    if (candidate == (XmFontList) NULL) candidate = w->browse.default_fontlist;


/*
**  Return the candidate
*/
    return (candidate);
}

