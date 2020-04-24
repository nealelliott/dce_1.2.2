/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cds_display_outline.c,v $
 * Revision 1.1.4.2  1996/02/18  19:24:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:08:37  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:03:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:37:02  root]
 * 
 * Revision 1.1.2.2  1992/12/30  12:38:45  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:22:37  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:23:31  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#if !defined(lint) && !defined(SABER)
static char *SccsID = "@(#)cds_display_outline.c\t6\t(DECdns)\t9/24/1991";
#endif	/* neither lint nor saber-C */
#define SYNC_ON_SCROLL
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

#define CDS_DISPLAY_OUTLINE

#include <stdlib.h>
#ifdef VMS
/*#pragma nostandard*/
#include <XmP.h>
/*#pragma standard*/
#else
#include <Xm/XmP.h>
#endif

#include <cds_decwmp.h>
#include <cds_private.h>

/*
**  Local routine declarations
*/
static void LclOutlineDrawEntry         ();  /*  Draw an entry             */
static void LclOutlineDrawEntryTop      ();
static void LclOutlineDrawLine          ();  /*  Draw the path line        */
static void LclOutlineClearLine         ();  /*  Clear the path line       */
static void LclOutlineClearEOS          ();  /*  Clear the end of screen   */
static int  LclOutlineFindOffset        ();  /*  Finds offset given x, y   */
       int  LclOutlineMapNumberToOffset ();  /*  Maps number to offset     */
       int  LclOutlineMapOffsetToY      ();  /*  Maps offset into a Y      */
static int  LclOutlineEntryAppend       ();  /*  Appends entry at bottom   */


/*
**  Helper routines.
*/
static    Boolean LclOutlineIntersects ();


/*
**  Local positioning routines
*/
static    void     LclOutlinePositionJump        ();
static    int      LclOutlineAtBottom            ();
static    Position LclOutlineGetPositionGivenTop ();


/*
**  Forward routine declarations for scrollbar callbacks
*/
static    int  LclOutlineScrollUpCandidate ();
static    void LclOutlineScrollUp          ();
static    void LclOutlineScrollDown        ();
static    void LclOutlineChangeBaseX       ();
static    void LclOutlineVScrollDrag       ();
static    void LclOutlineScrollAdjust      ();


/*
**  Scroll button action routines
*/
static    void LclOutlineScrollButton      ();
static    void LclOutlineScrollTimeout     ();


/*
** Here is the OutlineMapPosition public entry point using the C language
** interface.  This routine must determine the entry number and component
** number that are being displayed at the given x,y coordinate.  We will
** assume that this coordinate is relative to the window.
*/

void OutlineMapPosition (w, findx, findy, entry_number, component_number, entry_tag)

  dns_widget w;
  int findx, findy;
  int *entry_number;
  int *component_number;
  unsigned int *entry_tag;

{
/*
**  Cast the widget passed into an Widget data structure.
*/
    EntryPtr entry;
    int dummy_arg, offset, i, left_x;
    unsigned int dummy_unsigned_arg;


/*
**  Set up the arguments so that everything looks like its passed.
*/
    if (component_number == (int *) NULL) component_number = &dummy_arg;
    if (entry_tag == (unsigned int *) NULL) entry_tag = &dummy_unsigned_arg;


/*
**  Find out at what display offset maps into this x and y position.
*/
    offset = LclOutlineFindOffset (w, findx, findy);


/*
**  If it's not found, then get out of here now...
*/
    if (offset == 0)
       {
         *entry_number = 0;
         *component_number = 0;
         *entry_tag = 0;
         return;
       };


/*
**  Give the application the entry number
*/
    *entry_number = w->browse.entries[offset];


/*
**  Get the entry structure...
*/
    entry = StructGetEntryPtr (w, *entry_number);


/*
**  Give the caller the tag...
*/
    *entry_tag = entry->entry_tag;


/*
**  Save away the level number
*/
    w->browse.map_level = entry->level;


/*
**  If the X coordinate is within the left hand window, then
**  compute the component number and place it into 'i'.
*/
    if (findx < XtWidth (w->browse.lhs_window_widget))
       {
	/*
	**  This will tell us the maximum component on the left hand side
	*/
	    int max_lhs_comp = entry->num_components;


	/*
	**  See if there are less.
	*/
            if ((w->browse.display_mode == KdisplayColumns) || w->browse.rhs_components_unmapped)
	       if (w->browse.start_column_component != 0)
                  max_lhs_comp = w->browse.start_column_component - 1;


        /*
        **  If max_lhs_comp is greater than the actual number...
        */
            if (max_lhs_comp > entry->num_components)
                max_lhs_comp = entry->num_components;


	/*
	**  Compute the base x value based on the entries level number
	*/
	    left_x = w->browse.margin_width
		     + (entry->level * w->browse.indent_margin)
		     - (int) w->browse.window_basex;


	/*
	**  Loop through all of the lhs components.  This loop assumes that the
	**  component width has been set.  This is safe because we know that
	**  the entry is being displayed.
	*/
	    for (i = 1;  i < max_lhs_comp;  i++)
		{
		  if (findx < left_x + (int) entry->entrycompPtr[i-1].width) break;
		  left_x = left_x + (int) entry->entrycompPtr[i-1].width;
		};
       };


/*
**  If the X coordinate is within the right hand window, then
**  compute the component number and return.
*/
    if (findx > XtWidth (w->browse.lhs_window_widget))
       {
	/*
	**  Use a local X value to normalize the findx value
	*/
	    int normal_x = findx + w->browse.rhs_base_x - XtWidth (w->browse.lhs_window_widget) - mullion_width;


	/*
	**  Compute the base x value based on the scrolled window.
	*/
	    left_x = w->browse.default_spacing / 2;


	/*
	**  Default 'i' to zero in case they press the button in
	**  the right hand side on an entry which has no right hand
	**  side components.
	*/
	    i = 0;


	/*
	**  If this entry has right hand side components, then see
	**  which component it was...
	*/
            if (entry->num_components >= w->browse.start_column_component)
	       if (w->browse.start_column_component != 0)
	          for (i = w->browse.start_column_component;  i < entry->num_components;  i++)
 	              {
		        if (normal_x < left_x + (int) w->browse.column_widths[i-1] + (w->browse.default_spacing / 2)) break;
		        left_x = left_x + (int) w->browse.column_widths[i-1] + w->browse.default_spacing;
	   	      };
       };


/*
**  Set the value to 'i'
*/
    *component_number = i;
}

/*
**  This local routine returns the Y coordinate of a given entry if the
**  other supplied entry were to be placed as the first entry under the path
**  to the root.
*/

static Position LclOutlineGetPositionGivenTop (w, top_entry_number, entry_number)

  dns_widget w;
  int top_entry_number;
  int entry_number;

{
/*
**  Local data declarations
*/
    int i, level, num_path, y_position;
    EntryPtr entry;


/*
**  Get at the entry for the top_entry_number.  It needs to be valid so that
**  we can get at both the height and the level number.
*/
    entry = StructGetValidEntryPtr (w, top_entry_number);


/*
**  Based purely on the level number, we can now set the number of entries in
**  the path to the root.
*/
    if (w->browse.show_path_to_root)
         num_path = entry->level;
    else num_path = 0;


/*
**  Initialize the y position to include the first entry under the path to the
**  root plus the margin height of the window.  Don't add in the top entry if
**  it is the same as the target entry.
*/
    if (entry_number == top_entry_number)
         y_position = w->browse.margin_height;
    else y_position = w->browse.margin_height + entry->height;


/*
**  Figure out the path to the root.  The looping variable reflects which
**  entry number we are looking at.  The level variable reflects the level
**  number that we are looking for.
**
**  Since we are looking at the heights, we must get the valid entry.
*/
    if (num_path != 0)
       {
         level = entry->level - 1;
         for (i = top_entry_number - 1;  i > 0;  i--)
            {
              entry = StructGetValidEntryPtr (w, i);
              if (entry->level == level)
                 {
                   y_position = y_position + entry->height;
                   if (level == 0) break;
                   level--;
                 };
            };
       };


/*
**  The y_position variable now includes the path plus the first entry after
**  the path.   Loop sequentially now adding in the heights of the entries
**  after the path up to but not including the desired entry.
*/
    for (i = top_entry_number + 1;  i < entry_number;  i++)
        {
          entry = StructGetValidEntryPtr (w, i);
          y_position = y_position + entry->height;
        };


/*
**  Return the y position to the calling program.
*/
    return y_position;
}

/*
**  This routine is the high level interface to PositionDisplay.  This
**  routine can assume that the widget is disabled.
*/

int OutlinePositionDisplay (w, entry_number, position)

  dns_widget w;
  int entry_number, position;

{
/*
**  Local data declarations
*/
    int new_top_entry;
    Position new_position;
    EntryPtr entry;


/*
**  Take care of our only possible failing condition
*/
    if (position == KpositionNextPage)
       if (LclOutlineAtBottom (w))
          return FALSE;


/*
**  Disable the widget
*/
    DisableDisplay (w);


/*
**  Turn previous page into a position bottom call
*/
    if (position == KpositionPreviousPage)
      {
        position = KpositionBottom;
        entry_number = w->browse.entries [w->browse.num_path + 1];
      };


/*
**  Turn next page into a position top call
*/
    if (position == KpositionNextPage)
      {
        position = KpositionTop;
        entry_number = w->browse.entries [w->browse.display_count];
      };


/*
**  Get the valid entry so that we can use the height field.
*/
    entry = StructGetValidEntryPtr (w, entry_number);


/*
**  Default the passed entry to be the top entry under the path to the root.
*/
    new_top_entry = entry_number;


/*
**  If the desired location is in the middle...
*/
    if (position == KpositionMiddle)

       while (new_top_entry != 1)

         {
          /*
          **  Get this entries position based on the new_top_entry
          */
              new_position = LclOutlineGetPositionGivenTop (w, new_top_entry, entry_number);


          /*
          **  If our entry is hanging off of the bottom, then backoff 1 if we
          **  had moved the top entry.  Otherwise, we haven't much choice about
          **  the entry hanging over...
          */
              if (new_position + entry->height > XtY(w->browse.hscroll))
                 {
                   if (new_top_entry != entry_number) new_top_entry++;
                   break;
                 };


           /*
           **  If we are passed the middle, then we are done...
           */
               if (new_position + entry->height > (XtY(w->browse.hscroll) / 2))
                  break;


           /*
           **  Decrement the top entry and try again...
           */
               new_top_entry--;
         };


/*
**  If the desired location is at the bottom...
*/
    if (position == KpositionBottom)

       while (new_top_entry != 0)

         {
          /*
          **  Get this entries position based on the new_top_entry
          */
              new_position = LclOutlineGetPositionGivenTop (w, new_top_entry, entry_number);


          /*
          **  If our entry is hanging off of the bottom, then backoff 1 if we
          **  had moved the top entry.  Otherwise, we haven't much choice about
          **  the entry hanging over...
          */
              if (new_position + entry->height > XtY(w->browse.hscroll))
                 {
                   if (new_top_entry != entry_number) new_top_entry++;
                   break;
                 };


           /*
           **  Decrement the top entry and try again...
           */
               new_top_entry--;
         };


/*
**  If the new top entry is zero, then make it a one...
*/
    if (new_top_entry == 0)
       new_top_entry = 1;


/*
**  If this was a position bottom call, then we must get out of this routine
**  before attempting to resolve any whitespace problems.  Commit to this new
**  display and leave.
*/
    if (position == KpositionBottom)
       {
         LclOutlinePositionJump (w, new_top_entry);
         EnableDisplay (w);
         return TRUE;
       };


/*
**  Now we must determine if the entries that are going to be appended will
**  fill the screen.  The way we will do this is to validate entries after
**  the target entry until we go passed the bottom.  If we run out of entries
**  before passing the bottom, we will again call ourselves to position the
**  last entry at the bottom.
**
**  Optimization:  The following expensive loop is trying to see if the screen
**                 needs pulled down.  This is only applicable if the new top
**                 entry is within the max number of display elements away from
**                 the end of the list.  This is a good optimization for products
**		   having more than max_display entries.
**
**		   This code approximates the number of entries able to be
**		   displayed.
*/
    if (new_top_entry + (XtY(w->browse.hscroll) / entry->height) > w->browse.num_entries)
       {
	new_position = LclOutlineGetPositionGivenTop (w, new_top_entry, entry_number);
	new_position = new_position + entry->height;

	while (new_position < XtY(w->browse.hscroll))
	   {
	     if (entry_number == w->browse.num_entries)
		{
		  OutlinePositionDisplay (w, w->browse.num_entries, KpositionBottom);
		  EnableDisplay (w);
		  return TRUE;
		};
	     entry_number++;
	     entry = StructGetValidEntryPtr (w, entry_number);
	     new_position = new_position + entry->height;
	   };
       };


/*
**  We fell out of the loop only if the screen filled up.  Commit the new
**  screen layout.
*/
    LclOutlinePositionJump (w, new_top_entry);
    EnableDisplay (w);
    return TRUE;
}

/*
**  This routine is called to map a particular rectangle into a list of
**  selections that intersect that rectangle.  The calling program is
**  responsible for allocating the array into which the intersecting entry
**  numbers are placed.  The return value of this procedure is the number of
**  entries entered into the array.
**
**  The X and Y coordinates are relative to the window...
*/

int LclOutlineMapRectangle (w, entry_numbers, low_x, low_y, high_x, high_y)

  dns_widget w;
  int * entry_numbers;
  int low_x, low_y, high_x, high_y;

{
/*
**  Local data declarations
*/
    int i, count = 0;


/*
**  For each entry that is on the screen;  compute the x, y, width, and height
**  of that entry.  If this entry intersects the box in any way, then add it
**  the list of selections.  We will check intersection by examining the four
**  corners of the entry.
*/
    for (i = 1;  i <= w->browse.display_count;  i++)
        if (LclOutlineIntersects (w, i, low_x, low_y, high_x, high_y) == TRUE)
           {
             entry_numbers[count] = w->browse.entries[i];
             count++;
           };


/*
**  Return the number of selections
*/
    return count;
}

/*
**  This routine is called to map a particular rectangle to the lowest
**  intersecting entry.
*/

int LclOutlineMapRectangleLow (w, low_x, low_y, high_x, high_y)

  dns_widget w;
  int low_x, low_y, high_x, high_y;

{
/*
**  Local data declarations
*/
    int i;


/*
**  For each entry that is on the screen;  compute the x and the width value.
**  Break out of the loop if we intersect an entry.
*/
    for (i = 1;  i <= w->browse.display_count;  i++)
        if (LclOutlineIntersects (w, i, low_x, low_y, high_x, high_y) == TRUE)
           return (w->browse.entries[i]);


/*
**  No intersections, return 0
*/
    return 0;
}

/*
**  This routine is called to map a particular rectangle to the highest
**  intersecting entry.
*/

int LclOutlineMapRectangleHigh (w, low_x, low_y, high_x, high_y)

  dns_widget w;
  int low_x, low_y, high_x, high_y;

{
/*
**  Local data declarations
*/
    int i;


/*
**  For each entry that is on the screen;  compute the x and the width value.
*/
    for (i = w->browse.display_count;  i >= 1;  i--)
        if (LclOutlineIntersects (w, i, low_x, low_y, high_x, high_y) == TRUE)
           return w->browse.entries[i];


/*
**  No intersections, then return 0
*/
    return 0;
}


/*
**  Store the Range hook so range select works over multiple pages
*/
void OutlineSetRangeHook (w)

  dns_widget w;

{
/*
**  Local variable declarations
*/
    int num_hits;


/*
**  Map the rectangle into an entry number
*/
    num_hits = LclOutlineMapRectangle (w, &w->browse.range_hook, 0,
			               w->browse.box_base_y, (int) XtWidth (w),
				       w->browse.box_base_y);


/*
**  Deposit the first hit into the range hook
*/
    if (num_hits == 0)
       if (w->browse.entries[w->browse.display_count] != w->browse.num_entries)
	  w->browse.range_hook = w->browse.entries[w->browse.display_count] + 1;
}

/*
**  Return the entries from the range hook to the current  (x, y) position so
**  range select works over multiple pages.  The entry_array parameter is
**  allocated dynamically using XtMalloc, so it must be freed by the caller.
*/
void OutlineGetRangeEntries (w, x, y, range_flag, entry_count, entry_array)

  dns_widget w;
  int x, y;		    /* end point of range rectangle */
  int *range_flag;	    /* whether entry_array is a range or list of entires */
  int *entry_count;	    /* number of items in entry_array */
  LevelPtr *entry_array;    /* pointer to array of entries returned */

{
/*
**  Local data declarations
*/
    Position low_x, low_y, high_x, high_y;
    int low_entry_number, high_entry_number;


/*
** The outline mode always returns a range, so set range_flag to TRUE
*/
    *range_flag = TRUE;


/*
**  Save the new other x corner
*/
    w->browse.box_other_x = x;
    w->browse.box_other_y = y;


/*
**  If the release point (other_y) value is above the path to the root line,
**  then stop it there.
*/
    if (w->browse.box_other_y < w->browse.display_line_y)
        w->browse.box_other_y = w->browse.display_line_y;


/*
**  If the release point (other_y) value is below the horizontal scroll bar,
**  then stop it there.
*/
    if (w->browse.box_other_y > w->browse.hscroll->core.y)
        w->browse.box_other_y = w->browse.hscroll->core.y;


/*
**  Find the low and high x values of the rectangle.
*/
    if (w->browse.box_base_x > w->browse.box_other_x)
         { low_x  = w->browse.box_other_x;  high_x = w->browse.box_base_x;  }
    else { low_x  = w->browse.box_base_x;   high_x = w->browse.box_other_x; };


/*
**  Determine the low and high y values
*/
    if (w->browse.box_base_y > w->browse.box_other_y)
         { low_y  = w->browse.box_other_y;  high_y = w->browse.box_base_y;  }
    else { low_y  = w->browse.box_base_y;   high_y = w->browse.box_other_y; };


/*
**  If the low_y value is negative, then set the low_entry_number to the range
**  hook since it has gone out of site.  Otherwise, get the first and last
**  entries that the box intersects.
*/
    if (low_y < 0)
	 low_entry_number = w->browse.range_hook;
    else low_entry_number = LclOutlineMapRectangleLow (w, (int)low_x, (int)low_y, (int)high_x, (int)high_y);


/*
**  If it came back as zero, then no entries intersect the box so return nothing
*/
    if (low_entry_number == 0) {
	*entry_count = 0;
	return;
	};


/*
**  Get the high entry intersecting point
*/
    high_entry_number = LclOutlineMapRectangleHigh (w, (int)low_x, (int)low_y, (int)high_x, (int)high_y);


/*
**  If the hooked point is higher than the high intersection point, then that
**  is the new high point.
*/
    if (high_entry_number < w->browse.range_hook) high_entry_number = w->browse.range_hook;


/*
**  Allocate memory and return the range in the entry_array
*/
    *entry_count = 2;
    *entry_array = (LevelPtr) XtMalloc(*entry_count * sizeof(int));
    (*(*entry_array))[0] = low_entry_number;
    (*(*entry_array))[1] = high_entry_number;
}

/*
**  This routine is called when the user is testing to see if an autoscroll
**  should occur.  This routine is passed an X and Y coordinate and must
**  decide whether or not to autoscroll the screen or not.
*/

int OutlineAutoScrollCheck (w, y)

  dns_widget w;
  int y;

{
/*
**  If they are in the path to root section, then return true.
*/
    if (y < w->browse.display_line_y)
       if ((w->browse.num_path != 0) || (w->browse.entries[1] != 1))
          return 1;


/*
**  If they in the horizontal scroll bar and there are entries not being
**  shown, then scroll up
*/
    if (y > XtY(w->browse.hscroll))
       if (LclOutlineAtBottom (w) == FALSE)
	  if (LclOutlineScrollUpCandidate(w) > 0)
	     return 1;


/*
**  Nope
*/
    return 0;
}


/*
**  This routine is called when the user is dragging a set of entries or selecting
**  a range of entries.  This routine is passed an X and Y coordinate and must
**  decide whether or not to autoscroll the screen or not.  If it scrolls the
**  screen, it updates the box_base_y so that the box will be draw correctly
**  now that the screen positions have changed.
*/

void OutlineAutoScrollDisplay (w, y)

  dns_widget w;
  int y;

{
/*
**  Local data declarations
*/
    int entry_number, offset, going, old_y, new_y;


/*
**  If they are above the path to the root line and there is either a path to
**  the root or there are entries not being shown.
*/
    if (y < w->browse.display_line_y)
       if ((w->browse.num_path != 0) || (w->browse.entries[1] != 1))
	  {
           /*
           **  If we do not have a range hook set, then just scroll without
           **  worrying about a pixel count.
           */
               if (w->browse.range_hook == 0)
	          {
                    LclOutlineScrollDown (w);
                    LclOutlineScrollAdjust (w);
		    return;
                  };

           /*
           **  We do have a range_hook set
           */
               entry_number = w->browse.range_hook;
               offset = LclOutlineMapNumberToOffset (w, entry_number);
               if ((offset == 0) || (offset > w->browse.num_path + 1))
                  {
                    entry_number = w->browse.entries[w->browse.num_path + 1];
                    offset = LclOutlineMapNumberToOffset (w, entry_number);
                  };
               old_y  = LclOutlineMapOffsetToY (w, offset);
               LclOutlineScrollDown (w);
               offset = LclOutlineMapNumberToOffset (w, entry_number);
               new_y  = LclOutlineMapOffsetToY (w, offset);
	       w->browse.box_base_y = w->browse.box_base_y + (new_y - old_y);
               LclOutlineScrollAdjust (w);
          };


/*
**  If they in the horizontal scroll bar, then scroll up.
*/
    if (y > XtY(w->browse.hscroll))
       {
        /*
        **  If we do not have a range hook set, then just scroll without
        **  worrying about a pixel count.
        */
            if (w->browse.range_hook == 0)
               {
                LclOutlineScrollUp (w);
                LclOutlineScrollAdjust (w);
                return;
              };

        /*
        **  We do have a range_hook set.  If the range_hook is not present or if
        **  the range hook is going away, then we will track the last entry on
        **  the screen.  Otherwise, we will track the range hook itself.
        */
            entry_number = w->browse.range_hook;
            offset = LclOutlineMapNumberToOffset (w, entry_number);
	    going  = LclOutlineScrollUpCandidate (w);
            if ((offset == 0) || (offset == going))
               {
                 entry_number = w->browse.entries[w->browse.display_count];
                 offset = LclOutlineMapNumberToOffset (w, entry_number);
               };
            old_y  = LclOutlineMapOffsetToY (w, offset);
            LclOutlineScrollUp (w);
            offset = LclOutlineMapNumberToOffset (w, entry_number);
            new_y  = LclOutlineMapOffsetToY (w, offset);
	    w->browse.box_base_y = w->browse.box_base_y + (new_y - old_y);
            LclOutlineScrollAdjust (w);
       };
}

/*
**  This routine is called to create the ghost image to be used in a dragging
**  operation.  It is assumed that the ghost_x and ghost_y have been
**  set to the initial position in the screen from which this ghost will
**  appear attached.  These values are passed in relative to the screen itself.
**  This routine will set the ghost_basex and ghost_basey values to be relative
**  to the origin of the pixmap.
*/

void OutlineCreateGhost (w)

  dns_widget w;

{
/*
**  Local data declarations
*/
    Screen  *screen  = XtScreen (w);
    Display *display = DisplayOfScreen (screen);
    int i, min_x = 99999, max_x = 0, min_y = 99999, max_y = 0;
    int low_y = w->browse.margin_height;
    int y = w->browse.margin_height;


/*
**  Determine the rectangle size to be used in this ghost.
*/
    for (i = 1;  i <= w->browse.display_count;  i++)
        {
          EntryPtr entry = StructGetEntryPtr (w, w->browse.entries[i]);
          if (entry->selected)
             {
               int low_x  = w->browse.margin_width + (entry->level * w->browse.indent_margin);
               int high_x = low_x + entry->width;
               int high_y = low_y + entry->height;

               if (min_x > low_x)  min_x = low_x;
               if (min_y > low_y)  min_y = low_y;
               if (max_x < high_x) max_x = high_x;
               if (max_y < high_y) max_y = high_y;
             };
           low_y = low_y + entry->height;
        };


/*
**  If these are fixed width entries, then override the high x value
*/
    if (w->browse.fixed_width) max_x = XtWidth (w->browse.lhs_window_widget) - 1;


/*
**  We can now set the relative cursor holding point in the pixmap
*/
    w->browse.ghost_basex  = w->browse.ghost_x - min_x;
    w->browse.ghost_basey  = w->browse.ghost_y - min_y;
    w->browse.ghost_width  = max_x - min_x + 1;
    w->browse.ghost_height = max_y - min_y + 1;


/*
**  Create a pixmap of the appropriate size
*/
    w->browse.ghost = XCreatePixmap ( display, RootWindowOfScreen(screen), w->browse.ghost_width,
                                      w->browse.ghost_height, (unsigned) DefaultDepthOfScreen (screen));


/*
**  Clear the pixmap
*/
    XFillRectangle (XtDisplay(w),w->browse.ghost,w->browse.foreground_gc,0,0,w->browse.ghost_width,w->browse.ghost_height);


/*
**  For each entry on the screen.  If it is currently selected then draw into
**  the pixmap using the normal graphic context.
*/
    for (i = 1;  i <= w->browse.display_count;  i++)
        {
          EntryPtr entry = StructGetEntryPtr (w, w->browse.entries[i]);
          if (entry->selected)
             {
               int x  = (w->browse.margin_width + (entry->level * w->browse.indent_margin)) - min_x;
               int adjust_y  = y - min_y;
               int width  = entry->width;
               if (w->browse.fixed_width) width = w->browse.ghost_width - x - 2;
               XDrawRectangle (XtDisplay(w), w->browse.ghost, w->browse.background_gc,
			       (Position)x, (Position)adjust_y,
                               (Dimension)width, (Dimension)entry->height);
             };
          y = y + entry->height;
        };
}

/*
**  This routine is called to delete the ghost image used in a dragging operation.
*/

void OutlineDeleteGhost (w)

  dns_widget w;

{
/*
**  Delete the pixmap if it exists
*/
    if (w->browse.ghost != 0)
       XFreePixmap (XtDisplay(w), w->browse.ghost);


/*
**  Show that it is gone
*/
    w->browse.ghost = 0;
}

/*
**  This routine is called to do the display portion of the source
**  module invalidating an entry.
*/

void OutlineInvalidateEntry (w, entry_number)

  dns_widget w;
  int entry_number;

{
/*
**  Local routine data
*/
    EntryPtr entry = (EntryPtr) NULL;
    int i;


/*
**  Go through the list of displayed entries looking for this entry.
**  If the entry is found, then zero out the display_ptr which will
**  instruct the LclDraw routine to refresh this entry.
*/
    int offset = LclOutlineMapNumberToOffset (w, entry_number);

    if (offset != 0)
       {
         entry = StructGetEntryPtr (w, w->browse.entries[offset]);
	 if (w->browse.display_invalid[offset] == 0)
	    {
	     w->browse.display_invalid[offset] = entry->height;
             if (w->browse.sub_widgets_used)
                for (i = 0;  i <= entry->num_components-1;  i++)
                   if (entry->entrycompPtr[i].type == KcompWidget)
		      HIDE_WIDGET (w, entry->entrycompPtr[i].var.is_widget.readwrite_text);
            };
         w->browse.display_changed = TRUE;
       };


/*
**  Get the entry structure for this entry number
*/
    if (entry == (EntryPtr) NULL)
       entry = StructGetEntryPtr (w, entry_number);


/*
**  Show that it will need readjusted
*/
    entry->height_adjusted = FALSE;
}

/*
**  Specific screen routine to get the number being displayed.
*/
int OutlineGetNumDisplayed (w)

  dns_widget w;

{
/*
**  Return to the caller the current number of entries being displayed.
*/
    return w->browse.display_count;
}

/*
**  Display specific routine to get the list of displayed entries.
*/

void OutlineGetDisplayed (w, entry_nums, entry_tags, y_coords, num_array_entries)

  dns_widget w;
  int *entry_nums;
  unsigned int *entry_tags;
  int *y_coords;
  int num_array_entries;

{
/*
**  Local data
*/
    int i, number_to_copy;
    int y = w->browse.margin_height;


/*
**  Figure out the number of entries to copy based on the length of the array
**  and the number of entries that are displayed.
*/
    if (w->browse.display_count > num_array_entries)
         number_to_copy = num_array_entries;
    else number_to_copy = w->browse.display_count;


/*
**  Loop through the entries being displayed.
*/
    for (i = 1;  i <= number_to_copy;  i++)
        {
          EntryPtr entry = StructGetEntryPtr (w, w->browse.entries[i]);

	  if (entry_nums != (int *) NULL) entry_nums [i-1] = w->browse.entries[i];
          if (entry_tags != (unsigned int *) NULL) entry_tags [i-1] = entry->entry_tag;
          if (y_coords   != (int *) NULL) y_coords   [i-1] = y;

          y = y + entry->height;
        };


/*
**  Set any remaining array elements to null
*/
    for (i = number_to_copy + 1;  i < num_array_entries;  i++)
        {
	  if (entry_nums != (int *) NULL) entry_nums [i-1] = 0;
          if (entry_tags != (unsigned int *) NULL) entry_tags [i-1] = 0;
          if (y_coords   != (int *) NULL) y_coords   [i-1] = 0;
        };
}

/*
**  This routine is called whenever the application or source module has called
**  the SetEntrySensitivity high level call.
*/

void OutlineSetEntrySensitivity (w, entry_number)

  dns_widget w;
  int entry_number;

{
/*
**  Go through the list of displayed entries looking for this entry.
*/
    int offset = LclOutlineMapNumberToOffset (w, entry_number);


/*
**  If the entry is found, then zero out the display_ptr which will
**  instruct the LclDraw routine to refresh this entry.
*/
    if (offset != 0)
       {
         EntryPtr entry = StructGetEntryPtr (w, w->browse.entries[offset]);
	 if (w->browse.display_invalid[offset] == 0)
	     w->browse.display_invalid[offset] = entry->height;
         w->browse.display_changed = TRUE;
       };
}

/*
**  This routine is called indirectly from the AddEntries routine.
*/

void OutlineAddEntries (w, after_entry, number_added)

  dns_widget w;
  int after_entry, number_added;

{
/*
**  Local data declarations
*/
    int i, changed_offset;


/*
**  If the entry being added after is at or beyond the end of the display, then leave.
*/
    if (after_entry >= w->browse.entries[w->browse.display_count])
       {
         w->browse.display_changed = TRUE;
         return;
       };


/*
**  If there is a display portion and the entry is in that portion of the screen...
*/
    if (w->browse.display_count > w->browse.num_path)
       if (after_entry >= w->browse.entries[w->browse.num_path+1])
          {
            changed_offset = after_entry - w->browse.entries[w->browse.num_path+1];
            changed_offset = changed_offset + w->browse.num_path + 2;
            LclOutlineClearEOS (w, changed_offset);
            return;
          };


/*
**  If the entry being added after is in view (in the path), then that entry
**  that is being expanded will be made into the top entry after the path and
**  the list will be redisplayed from that entry down.
*/
    for (i = 1;  i <= w->browse.num_path;  i++)
      if (after_entry == w->browse.entries[i])
         {
           LclOutlineClearEOS (w, i + 1);
           return;
         };


/*
**  We are now in the position where the entry that had entries added to it is
**  somewhere hidden in the path to the root.  We will again not change the
**  display, but instead increment the entry numbers from that point down.
*/
    for (i = 1;  i <= w->browse.display_count;  i++)
       if (w->browse.entries[i] > after_entry)
          w->browse.entries[i] = w->browse.entries[i] + number_added;


/*
**  We must set display changed to true here in order to update the scrollbars.
*/
    w->browse.display_changed = TRUE;
}

/*
**  This routine is called indirectly from the DeleteEntries routine.
**  It's job is to mark the entries that will need
**  refreshed, when the widget is re-enabled.
*/

void OutlineDeleteEntries (w, after_entry, number_of_entries)

  dns_widget w;
  int after_entry, number_of_entries;

{
/*
**  Local data declarations
*/
    int i, changed_offset;


/*
**  If the entry being deleted after is at or beyond the end of the display,
**  then leave.
*/
    if (after_entry >= w->browse.entries[w->browse.display_count])
       {
         w->browse.display_changed = TRUE;
         return;
       };


/*
**  If this delete is after entry zero, then clear the whole screen
*/
    if (after_entry == 0)
       {
         LclOutlineClearEOS (w, 1);
         return;
       };


/*
**  If there is a display portion and the entry is in that portion of the screen...
*/
    if (w->browse.display_count > w->browse.num_path)
       if (after_entry >= w->browse.entries[w->browse.num_path+1])
          {
            changed_offset = after_entry - w->browse.entries[w->browse.num_path+1];
            changed_offset = changed_offset + w->browse.num_path + 2;
            LclOutlineClearEOS (w, changed_offset);
            return;
          };


/*
**  If the entry being removed after is in view (in the path) AND the first
**  entry after the path will be going away, then this entry should be made
**  the top entry after the path and the list redisplayed from that entry down.
*/
    for (i = 1;  i <= w->browse.num_path;  i++)
      if (after_entry == w->browse.entries[i])
         if ((after_entry + number_of_entries) >= w->browse.entries[w->browse.num_path+1])
            {
              LclOutlineClearEOS (w, i + 1);
              return;
            };


/*
**  We now know that the entry being deleted after is somewhere hidden above
**  the screen or somewhere in the path to root area.  What we do know is that
**  it is above the display portion of the screen.
**
**  We now must see if any of the entries in the display will be going away
**  on this delete and clear from that point down.  This is only the case
**  where the entries after this entry are siblings and not children.
*/
    for (i = 1;  i <= w->browse.display_count;  i++)
       if (w->browse.entries[i] > after_entry)
          if (w->browse.entries[i] <= after_entry + number_of_entries)
             {
               LclOutlineClearEOS (w, i);
               return;
             };


/*
**  We are now in the position where the entry that had entries removed to it is
**  somewhere hidden in the path to the root.  We will again not change the
**  display, but instead decrement the entry numbers from that point down.
*/
    for (i = 1;  i <= w->browse.display_count;  i++)
       if (w->browse.entries[i] > after_entry)
          w->browse.entries[i] = w->browse.entries[i] - number_of_entries;


/*
**  We must set display changed to true here in order to update the scrollbars.
*/
    w->browse.display_changed = TRUE;
}

/*
**  This routine is called indirectly from the EnableDisplay routine.
**  It's job is to get the screen up to date based on calls
**  that were made during the disabled state.
*/

void OutlineEnableDisplay (w)

  dns_widget w;

{
/*
**  Local data declarations
*/
    CallbackStruct temp;
    int num_entries;
    register int i, y;


/*
**  If we have nothing to do, then leave.
*/
    if (!w->browse.display_changed) return;
    w->browse.display_changed = FALSE;


/*
**  If we are not REALIZED, then leave.
*/
    if (!XtIsRealized (w)) return;


/*
**  Initialize variables
*/
    num_entries = w->browse.num_entries;
    y = w->browse.margin_height;


/*
**  Disable the widget in case new entries are added while we are here
*/
    DisableDisplay (w);


/*
**  Erase the path to root line.
*/
    LclOutlineClearLine (w);


/*
**  Ensure that all of the entries being displayed are valid.  If the
**  entry is not valid, then that means that either a subwidget has
**  resized or the application has invalidated the entry.  The screen
**  has not yet been changed.
*/
    for (i = 1;  i <= w->browse.display_count;  i++)
        {
	  /*
	  **  Get a valid entry pointer.  This will cause the widget layout
	  **  to be recalculated.
	  */
              EntryPtr entry = StructGetValidEntryPtr (w, w->browse.entries[i]);


	  /*
	  **  If the invalid bit is set...
	  */
              if (w->browse.display_invalid[i] != 0)
                 {
		   /*
		   **  The old height had been saved...
		   */
                       Dimension old_height = w->browse.display_invalid[i];

		   /*
		   **  If the height has changed, then the entire rest
		   **  of the screen is in disarray.  In fact, the number
		   **  of entries that can be displayed may change.
		   */
                       if (old_height != entry->height)
                          {
			    /*
			    **  If the entry changing height is below the
			    **  path to root, then just clear the entire end
			    **  of the screen.
			    **
			    **  If the entry is in the path to the root, then
			    **  preserve the top entry.
			    */
			        if (i > w->browse.num_path + 1) LclOutlineClearEOS (w, i);
		                else {
			              int current_top = w->browse.entries[w->browse.num_path+1];
	 	 	              LclOutlineClearEOS (w, 1);
			              LclOutlinePositionJump (w, current_top);
                                     };

			    /*
			    **  Fall out of the validations because we cleared the screen.
			    */
				break;
		          };

               w->browse.display_invalid[i] = 0;
               XClearArea (XtDisplay(w),
			   XtWindow(w->browse.lhs_window_widget),
			   0,
			   (Position)y,
		           XtWidth (w->browse.lhs_window_widget),
			   entry->height,
			   FALSE);

               if (w->browse.display_mode == KdisplayColumns)
                  XClearArea (XtDisplay(w),
			   XtWindow(w->browse.rhs_window_widget),
			   0,
			   (Position)y,
		           XtWidth (w->browse.rhs_window_widget),
			   entry->height,
			   FALSE);

               LclOutlineDrawEntry (w, entry, y);
             };
          y = y + entry->height;
        };


/*
**  Now try to append as many entries as possible
*/
    while (w->browse.entries[w->browse.display_count] < num_entries)
       if (LclOutlineEntryAppend (w, w->browse.entries[w->browse.display_count] + 1) == FALSE)
          break;


/*
**  See if the path to root line is accurate.  It may be too far south...
*/
    while (w->browse.num_path != 0)
       if (   (w->browse.num_path == w->browse.display_count)
           || (w->browse.entries[w->browse.num_path+1] == w->browse.entries[w->browse.num_path]+1))
            w->browse.num_path--;
       else break;


/*
**  Draw the path to root line.
*/
    LclOutlineDrawLine (w);


/*
**  Readjust the scroll bars only if there have been no changes since we
**  entered this routine.
*/
    if (!w->browse.display_changed)
       LclOutlineScrollAdjust (w);


/*
**  Re-Enable the widget
*/
    EnableDisplay (w);


/*
**  Issue the callback for the display having changed.
*/
    temp.reason = CRDisplayChanged;
    XtCallCallbacks (w, NdisplayChangedCallback, &temp);
}

/*
**  This routine is called when the selection status of an entry has
**  transitioned from low to high.  It may not be on the screen at this
**  time (Select All).
*/

void OutlineHighlightEntry (w, entry_number)

  dns_widget w;
  int entry_number;

{
/*
**  Local data
*/
    int y;
    EntryPtr entry;


/*
**  Get the offset of this entry.  If it is not being displayed then leave.
*/
    int offset = LclOutlineMapNumberToOffset (w, entry_number);
    if (offset == 0) return;


/*
**  Get the entry structure
*/
    entry = StructGetValidEntryPtr (w, w->browse.entries[offset]);


/*
**  If the widget is disabled, then mark this entry as needing redisplayed.
**  This will repaint the entry when the widget is re-enabled...
*/
    if (w->browse.disabled_count != 0)
       {
	 if (w->browse.display_invalid[offset] == 0)
	     w->browse.display_invalid[offset] = entry->height;
         w->browse.display_changed = TRUE;
         return;
       };


/*
**  If the widget is not disabled, then we may have a valid or an invalid
**  entry.  If its invalid, then let the display routine deal with the
**  problem.  This is because it knows how to deal with entries whose height
**  may change.
*/
    if (w->browse.display_invalid[offset] != 0)
       {
         OutlineEnableDisplay (w);
         return;
       };


/*
**  If we are not yet realized, then leave.
*/
    if (!XtIsRealized (w)) return;


/*
**  Both drawing sections below need the y position
*/
    y = LclOutlineMapOffsetToY (w, offset);


/*
**  Redraw the entry becoming unselected that is not on the path to the root line.
*/
    XClearArea (XtDisplay(w),
		XtWindow(w->browse.lhs_window_widget),
		0,
		(Position)y,
		XtWidth(w->browse.lhs_window_widget),
		entry->height,
		FALSE);

    if (w->browse.display_mode == KdisplayColumns)
       XClearArea (XtDisplay(w),
		XtWindow(w->browse.rhs_window_widget),
		0,
		(Position)y,
		XtWidth(w->browse.rhs_window_widget),
		entry->height,
		FALSE);

    LclOutlineDrawEntry (w, entry, y);


/*
**  Redraw the line if necessary...
*/
    if (y == w->browse.display_line_y)
       {
         w->browse.display_line_y = 0;
         LclOutlineDrawLine (w);
       };
}

/*
**  This routine is called when the application calls the high level equivalent.
**  It's job is to update the vertical scroll bar and to possibly append the
**  given entry number to the display.
*/
void OutlineFlushEntry (w, entry_number)

    dns_widget w;
    int entry_number;

{
/*
**  Append the entry if appropriate the entry number passed is the next logical
**  entry on the screen.
*/
    if ((w->browse.entries[w->browse.display_count] + 1) == entry_number)
         LclOutlineEntryAppend (w, entry_number);
    else LclOutlineScrollAdjust (w);

}

/*
**  This routine is called when the widget is exposed.  Note that the window
**  identifier is located in the clips_window field.
*/

void OutlineDraw (w)

  dns_widget w;

{
/*
**  Local data declarations
*/
    int i, y = w->browse.margin_height;


/*
**  Ensure that all of the entries being displayed are valid.  Only do this if
**  this routine is being entered with a disabled widget.  This can only occur
**  if the application is dispatching events during a callback.  We must also
**  avoid calling back to get a valid entry.  If the heights have changed OR
**  if the invalidated entry has not been revalidated, then just clear the end
**  of the display.
**
**  Note that this compares it to one because Redisplay disables the display
*/
    if (w->browse.disabled_count > 1)
       for (i = 1;  i <= w->browse.display_count;  i++)
           {
             EntryPtr entry = StructGetEntryPtr (w, w->browse.entries[i]);
             if (w->browse.display_invalid[i] != 0)
                {
                  Dimension old_height = w->browse.display_invalid[i];
		  if (!entry->valid || (old_height != entry->height))
                     {
                       if (i > w->browse.num_path + 1) LclOutlineClearEOS (w, i);
   		       else {
   			      int current_top = w->browse.entries[w->browse.num_path+1];
			      LclOutlineClearEOS (w, 1);
			      LclOutlinePositionJump (w, current_top);
                            };

                       break;
                     };
                  w->browse.display_invalid[i] = 0;

                  XClearArea (XtDisplay(w),
			      w->browse.clips_window,
			      0,
			      (Position)y,
		              XtWidth(w->browse.lhs_window_widget),
			      entry->height,
			      FALSE);

                  if (w->browse.display_mode == KdisplayColumns)
                     XClearArea (XtDisplay(w),
			      w->browse.clips_window,
			      0,
			      (Position)y,
		              XtWidth(w->browse.rhs_window_widget),
			      entry->height,
			      FALSE);

                  LclOutlineDrawEntry (w, entry, y);
                };
             y = y + entry->height;
           };


/*
**  Disable the widget in case new entries are added while we are here
*/
    DisableDisplay (w);


/*
**  Redraw only the exposed entries
*/
    OutlineDrawExposed (w);


/*
**  Draw the path to root line.  Set it to zero to force the drawing...
*/
    w->browse.display_line_y = 0;
    LclOutlineDrawLine (w);


/*
**  Enable the display in case display has changed
*/
    EnableDisplay (w);
}

/*
**  This routine handles any resize work after the window is done...  Keep in
**  mind that an expose on the entire window is waiting to happen...  While
**  processing the expose, the widget is disabled, displayed, and the reenabled.
*/

void OutlineResize (w)

  dns_widget w;

{
/*
**  Local data declarations
*/
    int freey, j;


/*
**  Resize clears the path to root line
*/
    w->browse.display_line_y = 0;


/*
**  Truncate off any entries whose top is beyond the horizontal scroll bar for
**  the case of the window getting smaller.
*/
    freey = w->browse.margin_height;

    for (j = 1;  j <= w->browse.display_count;  j++)
        {
          EntryPtr entry = StructGetEntryPtr (w, w->browse.entries[j]);
          if (freey > XtY(w->browse.hscroll))
             {
               w->browse.display_count = j - 1;
               break;
             };
          freey = freey + entry->height;
        };


/*
**  Mark the display as having been changed.  This will cause the entries to
**  be appended and the scroll bars to be adjusted.
*/
    w->browse.display_changed = TRUE;
}

/*
**  This routine is called indirectly when the application has called the
**  routine SetApplDragging.  It must turn on or off any subwidgets.
*/

void OutlineSetApplDragging (w, value)

  dns_widget w;
  int value;

{
/*
**  If it is being turned on, then we must desensitize our subwidgets so that
**  they do not get events.  Setting them insensitive is not sufficient.  We
**  must also create the ghost for them...
*/
    if (value)
       {
          OutlineCreateGhost (w);
       };


/*
**  If it is being turned off, then we must reenable our subwidgets and destroy
**  the ghost...
*/
    if (!value)
       {
          OutlineDeleteGhost (w);
       };

}

/*
**  This local routine determines if a rectangle and an entry intersect.  It returns
**  true or false based on the fact.  The X coordinates are relative to the
**  window.
*/

static Boolean LclOutlineIntersects (w, offset, low_x, low_y, high_x, high_y)

   dns_widget w;
   int offset, low_x, low_y, high_x, high_y;

{
/*
**  Local data declarations
*/
    EntryPtr entry;
    int x, y;


/*
**  Get the y position of this entry.
*/
    y = LclOutlineMapOffsetToY (w, offset);


/*
**  If the top line in the entry is beyond the lowest line in the rectangle
**  then it can't intersect...
*/
    if (y > high_y) return FALSE;


/*
**  Get the entry since we need to look at the height.  If the
**  bottom line in the entry is above the top line in the rectangle
**  then it can't intersect...
*/
    entry = StructGetEntryPtr (w, w->browse.entries[offset]);
    if ((y + (int)entry->height) < low_y) return FALSE;


/*
**  If the high X side of the rectangle is less than the beginning
**  X value of the entry, then we missed to the left.
*/
    x = w->browse.margin_width + (entry->level * w->browse.indent_margin) - (int) w->browse.window_basex;
    if (x > high_x) return FALSE;


/*
**  If we do not have fixed width entries, then compute the real width
**  of the entry.  the right line in the entry is to the left of the left line in the
**  rectangle, then it doesn't intersect
*/
    if (!w->browse.fixed_width)
       if ((x + entry->width) < low_x)
	  return FALSE;


/*
**  It intersects...
*/
    return TRUE;
}

/*
**  This local routine will take care of clearing the screen from a given
**  entry offset being displayed.  This routine takes care of unmapping
**  subwidgets, really issuing the clear commands, and adjusting the variables
**  needed to redisplay things later.
**
**  It redraws the column lines if necessary.
*/

static void LclOutlineClearEOS (w, offset)

  dns_widget w;
  int offset;

{
/*
**  Local data declarations
*/
    int i, y, j;


/*
**  If the offset exceeds the number being displayed, then leave.
*/
    if (offset > w->browse.display_count)  return;


/*
**  We know that the display is being changed
*/
    w->browse.display_changed = TRUE;


/*
**  Get the y position that will be the top.
*/
    y = LclOutlineMapOffsetToY (w, offset);


/*
**  Clear our text from the screen in one fell swoop.  Hopefully the server will
**  be smart about this and realize that exposes for the moved widgets will not
**  be necessary.  If the height is less than y don't bother because we won't
**  see any difference.
*/
    if ((XtHeight(w) > y) && (XtIsRealized(w)))
	{
	  XClearArea (XtDisplay(w),
		      XtWindow(w->browse.lhs_window_widget),
		      0,
		      (Position)y,
		      XtWidth(w->browse.lhs_window_widget),
		      XtHeight(w)-y,
		      FALSE);

          if (w->browse.display_mode == KdisplayColumns)
	     XClearArea (XtDisplay(w),
		      XtWindow(w->browse.rhs_window_widget),
		      0,
		      (Position)y,
		      XtWidth(w->browse.rhs_window_widget),
		      XtHeight(w)-y,
		      FALSE);
	};


/*
**  If we cleared the path to the root line, then keep things clean.
*/
    if (y <= w->browse.display_line_y) w->browse.display_line_y = 0;


/*
**  For each of the entries being removed, unmap their subwidgets and lose the
**  pointer to the entry.
*/
    if (w->browse.sub_widgets_used)
       for (i = offset;  i <= w->browse.display_count;  i++)
          {
            /*
            **  Local data declarations
            */
                EntryPtr entry = StructGetEntryPtr (w, w->browse.entries[i]);

            /*
            **  For each component of the entry being cleared, unmap its subwidgets.
            */
                for (j = 0;  j <= entry->num_components-1;  j++)
                   if (entry->entrycompPtr[j].type == KcompWidget)
		      HIDE_WIDGET (w, entry->entrycompPtr[j].var.is_widget.readwrite_text);
          };


/*
**  If the offset was in the path to root, then update the number of entries.
*/
    if (offset <= w->browse.num_path) w->browse.num_path = offset - 1;


/*
**  Update the number of entries being displayed
*/
    w->browse.display_count = offset - 1;


/*
**  If this is columns mode, then redraw the lines on the right hand side.
*/
    if (w->browse.display_mode == KdisplayColumns)
       if (w->browse.start_column_component != 0)
          if (w->browse.column_lines)
           {
	    /*
	    **  Local data declarations
	    */
                int comp_offset = w->browse.start_column_component - 1;
                int column_x = 0 - w->browse.rhs_base_x;


	    /*
	    **  Loop on all possible right hand side components.  We will stop
            **  this loop when we see a column width of zero or until we go off
            **  the right side.
	    */
		while (TRUE)
		  {
		    /*
		    **  If this component has a width of zero, then leave.
		    */
		        if (w->browse.column_widths [comp_offset] == 0) break;


		    /*
		    **  If the next component has a width of zero, then leave.
		    */
		        if (w->browse.column_widths [comp_offset + 1] == 0) break;


 		    /*
		    **  Add in the width of this component.  We are drawing lines
                    **  after the components because we do not have one in front
		    **  of the first rhs component
		    */
	                column_x = column_x + w->browse.column_widths [comp_offset] + w->browse.default_spacing;


                    /*
		    **  If we are beyond the window, then leave.
		    */
		        if (column_x > XtWidth (w->browse.rhs_window_widget)) break;


		    /*
                    **  If we are in the window, then draw the line.
		    */
	                if (column_x > 0)
                           XDrawLine (XtDisplay(w),
	                              XtWindow(w->browse.rhs_window_widget),
	                              w->browse.foreground_gc,
                                      column_x,
	                              w->browse.margin_height,
			              column_x,
		                      XtHeight(w->browse.rhs_window_widget) - w->browse.margin_height);


                    /*
                    **  Increment to the next component and the next column
                    */
                        comp_offset++;
                  };
           };

}

/*
**  This routine jumps to a location at the top...  All callers of this routine
**  disable and reenable the widget.  This means that only the path and the
**  first entry under the path need dealt with.  This routine then sets the
**  display_changed to TRUE to take care of appending new entries and updating
**  the scroll bars.
*/

static void LclOutlinePositionJump (w, entry_number)

  dns_widget w;
  int entry_number;

{
/*
**  Local data declarations
*/
    int i, level, differ, count, num_path, entries[max_display];
    EntryPtr entry;


/*
**  Get at the entry for the entry_number.  It need not be valid because we
**  only need the level number.
*/
    entry = StructGetEntryPtr (w, entry_number);


/*
**  Based purely on the level number, we can now set the num_path and count
**  variables.
*/
    if (w->browse.show_path_to_root)
         num_path = entry->level;
    else num_path = 0;

    count = num_path + 1;


/*
**  See if maybe we're already at our destination...
*/
    if (   (num_path == w->browse.num_path)
        && (w->browse.display_count > count)
        && (w->browse.entries[count] == entry_number)) return;


/*
**  Move the initial entry_number and pointer into the proper position.  Note
**  that indexing into both arrays is one-based.
*/
    entries [count] = entry_number;


/*
**  Figure out the path to the root.  The looping variable reflects which
**  entry number we are looking at.  The level variable reflects the level
**  number that we are looking for.
*/
    if (num_path != 0)
       {
         level = entry->level - 1;
         for (i = entry_number - 1;  i > 0;  i--)
            {
              entry = StructGetEntryPtr (w, i);
              if (entry->level == level)
                 {
                   entries [level+1] = i;
                   if (level == 0) break;
                   level--;
                 };
            };
       };


/*
**  Now we're ready to make this visible to the user.  We have setup a local
**  structure and we have the currently being displayed structure in the
**  instance record.
**
**  Find the point at which the two structures are different.  Place that
**  point into the differ variable.  Default this value to one passed the
**  end of our local display.
*/
    if (w->browse.display_count == 0) differ = 1;
    else {
           differ = count + 1;
           for (i = 1;  i <= count;  i++)
               if ((entries[i] != w->browse.entries[i]) || (w->browse.display_invalid[i] != 0))
                  {
                   differ = i;
                   break;
                  };
         };


/*
**  If the differing point is beyond what is currently being displayed, then
**  just set display changed to true and leave.  Also be sure that it's bigger
**  than what we deem count to be...
*/
    if ((differ > count) && (differ > w->browse.display_count))
       {
         w->browse.display_changed = TRUE;
         return;
       };


/*
**  Clear to the end of the screen.  The instance record path line and path
**  length is adjusted if it is cleared.  This routine also takes care of
**  setting display_changed to TRUE.
*/
    LclOutlineClearEOS (w, differ);


/*
**  Now append the entries that we know about.
*/
    for (i = differ;  i <= count;  i++)
        LclOutlineEntryAppend (w, entries[i]);


/*
**  Commit the new path length
*/
    w->browse.num_path = num_path;
}

/*
**  This routine is called by the STRUCTURE module whenever a valid entry
**  is desired.   It is only called if 'height_adjusted' is false.
*/

void OutlineAdjustHeight (w, entry)

  dns_widget w;
  EntryPtr entry;

{
/*
**  Local Variables
*/
    int real_width;


/*
**  Call the common routine to do most of the work
*/
    DisplayAdjustEntryHeight (w, entry);


/*
**  Bump the entry height and width.  Do it differently for expectHighlighting.
*/
    if (w->browse.expect_highlighting)
       {
         entry->height += 5;
         entry->width  += 4;
       }
    else
       {
         entry->height += 2;
         entry->width  += 1;
       };


/*
**  See if this entry is bigger than the biggest so far.  First adjust the width
**  for the indention amount.
*/
    real_width = entry->width + w->browse.margin_width + (entry->level * w->browse.indent_margin);
    if (w->browse.max_width < real_width)  w->browse.max_width = real_width;


}

/*
**  This routine is called by the module whenever a widget component
**  wants to change size.
*/

void OutlineAdjustEntrySize (w, entry_number)

  dns_widget w;
  int entry_number;

{
/*
**  Invalidate Entry does all the work we need
*/
    OutlineInvalidateEntry (w, entry_number);


}

/*
**  Routine that redraws those entries that intersect clipping rectangles.
*/

void OutlineDrawExposed (w)

  dns_widget w;

{
/*
**  Routine data declarations
*/
    int entry_numbers[max_display], i, j, count;
    Boolean gotone = FALSE;
    int y = w->browse.margin_height;
    Dimension x_adjust = 0;


/*
**  Return if no displayed entries.
*/
    if (w->browse.display_count == 0)  return;


/*
**  If we are not refreshing all and there are no clips, then return
*/
    if ((w->browse.clip_count == 0) && (!w->browse.refresh_all))  return;


/*
**  If this is a forced refresh, then ignore the clipping rectangles and
**  refresh all of the entries.
*/
    if (w->browse.refresh_all)
       {
	 w->browse.clip_count = 0;
         w->browse.refresh_all = FALSE;
         gotone = TRUE;
         for (i = 1;  i <= w->browse.display_count;  i++)
            w->browse.display_invalid [i] = TRUE;
       };


/*
**  Loop on each of the clipping rectangles marking the intersected entries
**  as invalid.  We do it this way because there may be multiple clipping
**  rectangles and they may intersect common entries.  If you display the
**  entry in the first loop, you may be displaying it many times.
*/
    if (w->browse.clip_count != 0)
       {
       /*
       **  If the clips are in the RHS, then we must setup a variable
       **  called the x_adjust.  This is because the routine that maps
       **  a rectangle to intersecting entries assumes that the x and y
       **  positions are relative to the window and not the RHS window.
       */
       if (w->browse.clips_window == XtWindow (w->browse.rhs_window_widget))
          {
	  x_adjust = XtWidth (w->browse.lhs_window_widget) + mullion_width;
	  };


       /*
       **  There are some clipping rectangles, so set them in the GCs
       */
       XSetClipRectangles (XtDisplay(w),w->browse.grey_gc,      0,0,w->browse.clips,w->browse.clip_count,Unsorted);
       XSetClipRectangles (XtDisplay(w),w->browse.foreground_gc,0,0,w->browse.clips,w->browse.clip_count,Unsorted);
       XSetClipRectangles (XtDisplay(w),w->browse.background_gc,0,0,w->browse.clips,w->browse.clip_count,Unsorted);

       for (i = 0;  i < w->browse.clip_count;  i++)
          {
	  count = LclOutlineMapRectangle (w, entry_numbers,
				          (int)w->browse.clips[i].x + x_adjust,
					  (int)w->browse.clips[i].y,
                                          (int)w->browse.clips[i].x + x_adjust + w->browse.clips[i].width,
                                          (int)w->browse.clips[i].y + w->browse.clips[i].height);

	  for (j = 0;  j < count;  j++)
	      w->browse.display_invalid[LclOutlineMapNumberToOffset(w,entry_numbers[j])] = TRUE;

	  if (count) gotone=TRUE;
          };
       };


/*
**  Redraw those entries that had intersected
*/
    if (gotone)
       for (i = 1;  i <= w->browse.display_count;  i++)
           {
             EntryPtr entry = StructGetValidEntryPtr (w, w->browse.entries[i]);
             if (w->browse.display_invalid[i])
                {
                  LclOutlineDrawEntry (w, entry, y);
                  w->browse.display_invalid[i] = 0;
                };
             y = y + entry->height;
           };


/*
**  Clear any clip rectangles now that they have been serviced
*/
    if (w->browse.clip_count != 0)
	{
	w->browse.clip_count = 0;
	XSetClipMask (XtDisplay(w), w->browse.grey_gc,       None);
	XSetClipMask (XtDisplay(w), w->browse.foreground_gc, None);
	XSetClipMask (XtDisplay(w), w->browse.background_gc, None);
	}
}

/*
**  This routine does processing when the display type is changed to Outline.
*/
void OutlineChangeMode (w)

    dns_widget	w;

{
/*
**  Local variables
*/
    int entry_number;


/*
**  Set-up code...
*/
    if (w->browse.display_mode != KdisplayTree) {
	Arg args[1];


	/*
	**  Set the display position
	*/
        LclOutlineClearEOS (w, 1);
	if (w->browse.current_entry_number > 0)
	    OutlinePositionDisplay(w, w->browse.current_entry_number, KpositionTop);


	/*
	**  Insensitize the nav button, and make it look inactive
	*/
	XtSetSensitive(w->browse.nav_button,FALSE);
	XtSetArg (args[0] ,XmNlabelPixmap, w->browse.outline_nav_pixmap);
	XtSetValues (w->browse.nav_button, args, 1);
	}

/*
**  Clean-up code...
*/
    else {
	/*
	**  Enable the nav button.
	*/
	XtSetSensitive(w->browse.nav_button,TRUE);


	/*
	**  Set the current entry for the Tree code.
	*/
	w->browse.current_entry = NULL;
	entry_number = w->browse.entries[w->browse.num_path + 1];

	/*
	**  Return if nothing displayed
	*/
	if (entry_number == 0) return;

	/*
	**  If entry number is the root, let position default
	*/
	if (entry_number == 1) return;


	/*
	**  Otherwise set current to one found.
	*/
	w->browse.current_entry = StructGetEntryPtr(w, entry_number);
	w->browse.prevx = 0;
	w->browse.prevy = 0;
	}

} /* OutlineChangeMode */

/*
**  This routine scrolls the screen up one line.
*/

static void LclOutlineScrollUp (w)

  dns_widget w;

{
/*
**  Local data declarations
*/
    CallbackStruct temp;
    EntryPtr oldentry, lastentry, entry;
    int i, j, oldy, lasty, offset, adjust = 0;
    Position copy_src_y, copy_dst_y;
    Dimension copy_height;
    int repaint_last;


/*
**  If we are displaying the last entry, then ignore it too.
*/
    if (LclOutlineAtBottom (w) == TRUE)
       return;


/*
**  Call the local routine that returns as its value the offset into the
**  path that should be removed.
*/
    offset = LclOutlineScrollUpCandidate (w);


/*
**  If really was at bottom, or couldn't scroll because display too small,
**  then return.
*/
    if (offset < 0) return;


/*
**  Get the entry pointers
*/
    oldentry  = StructGetEntryPtr (w, w->browse.entries[offset]);
    lastentry = StructGetEntryPtr (w, w->browse.entries[w->browse.display_count]);

    oldy      = LclOutlineMapOffsetToY (w, offset);
    lasty     = LclOutlineMapOffsetToY (w, w->browse.display_count);


/*
**  For each component of the entry being cleared, unmap its subwidgets.
*/
    if (w->browse.sub_widgets_used)
       for (i = 0;  i <= oldentry->num_components-1;  i++)
           if (oldentry->entrycompPtr[i].type == KcompWidget)
	      HIDE_WIDGET (w, oldentry->entrycompPtr[i].var.is_widget.readwrite_text);

/*
**  If the oldy is the path to the root line, then adjust by 1.  If it is
**  not the path to the root line, the we must first clear the line.  We
**  will repaint the line before returning.
*/
    if (oldy == w->browse.display_line_y) adjust = 1;
    else LclOutlineClearLine (w);


/*
**  Compute the amount area being copied.
*/
    copy_src_y  = oldy + oldentry->height + adjust;
    copy_dst_y  = oldy + adjust;
    copy_height = (lasty + lastentry->height) - (oldy + oldentry->height) - adjust;

    repaint_last = FALSE;

    if ((copy_height > 0) && ((copy_src_y + copy_height) > XtY(w->browse.hscroll)))
       {
         repaint_last = TRUE;
         copy_height = XtY(w->browse.hscroll) - copy_src_y;
       };


/*
**  Shift the screen up to remove the entry
*/
    if (copy_height > 0)
       {
        XCopyArea (XtDisplay(w),
		   XtWindow(w->browse.lhs_window_widget),
		   XtWindow(w->browse.lhs_window_widget),
		   w->browse.foreground_gc,
                   0, (Position)copy_src_y,
		   XtWidth(w->browse.lhs_window_widget),
		   (Dimension)copy_height,
		   0, (Position)copy_dst_y);

        if (w->browse.display_mode == KdisplayColumns)
           XCopyArea (XtDisplay(w),
		      XtWindow(w->browse.rhs_window_widget),
		      XtWindow(w->browse.rhs_window_widget),
		      w->browse.foreground_gc,
                      0, (Position)copy_src_y,
		      XtWidth(w->browse.rhs_window_widget),
		      (Dimension)copy_height,
		      0, (Position)copy_dst_y);
	};


/*
**  Move all of the widgets up by oldentry->height pixels.  Only the last
**  one moved gets exposed.
*/
    if (w->browse.sub_widgets_used)
       for (i = offset + 1;  i <= w->browse.display_count;  i++)
          {
            entry = StructGetEntryPtr (w, w->browse.entries[i]);
            for (j = 0;  j <= entry->num_components-1;  j++)
                if (entry->entrycompPtr[j].type == KcompWidget)
		   SHOW_WIDGET (w, entry->entrycompPtr[j].var.is_widget.readwrite_text,
	 	                XtX(entry->entrycompPtr[j].var.is_widget.readwrite_text),
                                XtY(entry->entrycompPtr[j].var.is_widget.readwrite_text) - oldentry->height,
				i == w->browse.display_count);
          };


/*
**  Reflect the removal in the structure
*/
    StructCloseArray (w->browse.entries, sizeof(short), w->browse.display_count + 1, offset);
    StructCloseArray (w->browse.display_invalid, sizeof(Dimension), w->browse.display_count + 1, offset);


/*
**  Reflect the removal in the counter
*/
    w->browse.display_count--;


/*
**  Set the path length
*/
    w->browse.num_path = offset - 1;


/*
**  Get the y position and entry of the last entry.  Then change lasty to be
**  just beyond the entry.
*/
    lastentry = StructGetEntryPtr (w, w->browse.entries[w->browse.display_count]);
    lasty     = LclOutlineMapOffsetToY (w, w->browse.display_count);


/*
**  Repaint this entry if we were told to do so...
*/
    if (lasty == w->browse.display_line_y) adjust = 1;
    else adjust = 0;

    if (repaint_last == TRUE)
       {
         XClearArea (XtDisplay(w), XtWindow(w->browse.lhs_window_widget), 0,
	             (Position)lasty+adjust,
		     XtWidth (w->browse.lhs_window_widget),
		     (Dimension)lastentry->height-adjust, FALSE);

        if (w->browse.display_mode == KdisplayColumns)
           XClearArea (XtDisplay(w), XtWindow(w->browse.rhs_window_widget), 0,
	             (Position)lasty+adjust,
		     XtWidth (w->browse.rhs_window_widget),
		     (Dimension)lastentry->height-adjust, FALSE);

         LclOutlineDrawEntry (w, lastentry, lasty);
       };


/*
**  Make lasty be just beyond the last entry
*/
    lasty = lasty + lastentry->height;


/*
**  Clear the bottom of the screen if we need to.
*/
    if (lasty < XtY(w->browse.hscroll))
       {
        XClearArea (XtDisplay(w),XtWindow(w->browse.lhs_window_widget),0,
		    (Position)lasty+adjust,
	 	    XtWidth (w->browse.lhs_window_widget),
		    (Dimension)XtY(w->browse.hscroll)-lasty-adjust,FALSE);

        if (w->browse.display_mode == KdisplayColumns)
           XClearArea (XtDisplay(w),XtWindow(w->browse.rhs_window_widget),0,
		    (Position)lasty+adjust,
	 	    XtWidth (w->browse.rhs_window_widget),
		    (Dimension)XtY(w->browse.hscroll)-lasty-adjust,FALSE);
       };


/*
**  Call XSync to keep GraphicsExpose events in order
*/
#ifdef SYNC_ON_SCROLL
    XSync(XtDisplay(w),FALSE);
#endif

/*
** Don't do this for now because the events will be handled in the module.
*/
    w->browse.clips_window = XtWindow (w->browse.lhs_window_widget);
    DisplayGraphicsExpose (w);
    OutlineDrawExposed(w);

    if (w->browse.display_mode == KdisplayColumns)
       {
        w->browse.clips_window = XtWindow (w->browse.rhs_window_widget);
        DisplayGraphicsExpose (w);
        OutlineDrawExposed(w);
       };

/*
**  Redraw the line
*/
    LclOutlineDrawLine (w);


/*
**  Append as many entries as possible
*/
    while (w->browse.entries[w->browse.display_count] < w->browse.num_entries)
       if (LclOutlineEntryAppend (w, w->browse.entries[w->browse.display_count] + 1) == FALSE)
          break;


/*
**  Issue the callback for the display having changed.
*/
    temp.reason = CRDisplayChanged;
    XtCallCallbacks (w, NdisplayChangedCallback, &temp);
}

/*
**  This routine determines who gets axed when a scroll up operation
**  is being performed.  If can't find a candidate (such as when the display
**  is too small to hold the path-to-root) then return -1.
*/

static int LclOutlineScrollUpCandidate (w)

  dns_widget w;

{
/*
**  Local data declarations
*/
    EntryPtr entry1, entry2, entry3;
    int i, previous_level;


/*
**  If we are not showing the path to the root, then return offset 1.
*/
    if (!w->browse.show_path_to_root) return 1;


/*
**  If there is currently no path to root, then go through each entry and find
**  the first two that are on the same level.  Set the path to root counter and
**  the display top variable to make it look as if the path to root was established.
*/
    if (w->browse.num_path == 0)
       for (i = 1;  i < w->browse.display_count;  i++)
           {
             entry1 = StructGetEntryPtr (w, w->browse.entries[i]);
             entry2 = StructGetEntryPtr (w, w->browse.entries[i+1]);

             if (entry1->level > entry2->level)
                {
                 w->browse.num_path = i - 1;
                 break;
                };

             if (entry1->level == entry2->level)
                {
                 w->browse.num_path = i;
                 break;
                };
           };


/*
**  If we have don't have at least num_path plus one entries displayed...
*/
    if (w->browse.display_count <= w->browse.num_path)
       return -1;


/*
**  Now we know we have a path to the root section.
**
**  Since we're scrolling up, we must remove an entry from the path to root
**  array.  Our choices are to delete one of three entries depending on the
**  current top entry and the level number of the last in the path to root
**  and the next entries level number.
**
**  Our first choice is to see if the last two entries in the real path to root
**  are at the same level.  If they are then remove the latter of the two.
*/
    if (w->browse.num_path > 1)
       {
         entry1 = StructGetEntryPtr (w, w->browse.entries[w->browse.num_path]);
         entry2 = StructGetEntryPtr (w, w->browse.entries[w->browse.num_path-1]);
         if (entry1->level == entry2->level) return (w->browse.num_path);
       };


/*
**  Our next candidate is the last guy in the path to root if it is at the
**  same level as the guy at the top of the display portion.
*/
    if (w->browse.num_path > 0)
       {
         entry1 = StructGetEntryPtr (w, w->browse.entries[w->browse.num_path]);
         entry2 = StructGetEntryPtr (w, w->browse.entries[w->browse.num_path+1]);
         if (entry1->level == entry2->level) return (w->browse.num_path);
       };


/*
**  If we at the bottom, then return offset 0;
*/
    if ((LclOutlineAtBottom(w) == TRUE) || (w->browse.num_path == 0))
       {
         return (-1);
       };


/*
**  Our next candidate will be the top entry if it is at the same level or is
**  at a greater level than the one below it.  Note that we may not have a
**  second entry underneath the path to the root.
*/
    entry1 = StructGetEntryPtr (w, w->browse.entries[w->browse.num_path+1]);
    entry2 = StructGetEntryPtr (w, w->browse.entries[w->browse.num_path+1]+1);
    if (entry2 == (EntryPtr) NULL) entry2 = entry1->next;
    if (entry2 == (EntryPtr) NULL) return (-1);
    if (entry1->level >= entry2->level) return (w->browse.num_path+1);


/*
**  Now we know that the top entry is at a lower level than the one below it.
**
**  If the top entry is at the same or at a lower level than the last one in the
**  path, then axe the last entry in the path to root.
*/
    entry3 = StructGetEntryPtr (w, w->browse.entries[w->browse.num_path]);
    if (entry1->level <= entry3->level) return (w->browse.num_path);


/*
**  We must go forward until we find two entries at the same level.  We will
**  axe the first of those two.
*/
    for (i = w->browse.num_path + 1;  i <= w->browse.display_count;  i++)
        {
          previous_level = entry2->level;
          entry2 = StructGetEntryPtr (w, w->browse.entries[i]+2);
          if (entry2->level <= previous_level) return i + 1;
        };


/*
**  We should not get here, but just in case we'll return top.
*/
    return (w->browse.num_path);
}

/*
**  This routine scrolls all of the entries down one position.  We will pull
**  down the one that had just disappeared.
*/

static void LclOutlineScrollDown (w)

  dns_widget w;

{
/*
**  Local data declarations
*/
    int i, j, k, newy, freey, offset, entry_number, before_level = -1, adjust = 0;
    int num_path;
    CallbackStruct temp;
    EntryPtr newentry, lastentry, entry;


/*
**  If we are already at the top, then ignore this call.
*/
    if ((w->browse.num_path == 0) && (w->browse.entries[1] == 1))
       return;


/*
**  If the entire path to root plus 1 was not being displayed, then don't scroll.
*/
    if ((w->browse.show_path_to_root) && (w->browse.display_count <= w->browse.num_path))
       return;


/*
**  If we do not allow a path, then we know that we are going to bring in the
**  next entry from the top...
*/
    if (w->browse.show_path_to_root)
         {
	   offset = w->browse.num_path + 1;
           entry_number = w->browse.entries[offset-1]+1;
         }
    else
	 {
           offset = 1;
           entry_number = w->browse.entries[1] - 1;
         };


/*
**  Set a variable to hold the level of the entry that will be directly above
**  the new entry.
*/
    if (offset != 1)
       {
         entry = StructGetEntryPtr (w, w->browse.entries[offset-1]);
         before_level = entry->level;
       };


/*
**  If the entry at the given offset is at the same level as the one before it,
**  then bring in the one before it.  Otherwise loop for it...
*/
    if (w->browse.show_path_to_root)
       {
         entry = StructGetEntryPtr (w, w->browse.entries[offset]-1);

         if (entry->level == before_level)
            entry_number = w->browse.entries[offset] - 1;

         if (entry->level != before_level)
            for (i = w->browse.entries[offset] - 1;  i > w->browse.entries[offset-1];  i--)
               {
                 entry = StructGetEntryPtr (w, i);
                 if (entry->level == before_level + 1)
                    {
                      entry_number = i;
                      break;
                    };
               };
       };


/*
**  Get the new pointer and y position.  Disable the widget around the call to
**  avoid drawing code on last enable.
*/
    w->browse.disabled_count++;
    newentry  = StructGetValidEntryPtr (w, entry_number);
    w->browse.disabled_count--;

    newy      = LclOutlineMapOffsetToY (w, offset);


/*
**  Adjust to skip over the line if its the new y position
*/
    if (w->browse.display_line_y == newy) adjust = 1;


/*
**  Shift the screen down by the new entry amount.
*/
    {
     int height = XtY(w->browse.hscroll) - newy - newentry->height - adjust;
     if (height > 0)
	{
	  XCopyArea (XtDisplay(w),
		     XtWindow(w->browse.lhs_window_widget),
		     XtWindow(w->browse.lhs_window_widget),
		     w->browse.foreground_gc, 0, (Position)newy + adjust,
  		     XtWidth (w->browse.lhs_window_widget),
	             (Dimension)height, 0,
		     (Position)newy + newentry->height + adjust);

          if (w->browse.display_mode == KdisplayColumns)
	     XCopyArea (XtDisplay(w),
		     XtWindow(w->browse.rhs_window_widget),
		     XtWindow(w->browse.rhs_window_widget),
		     w->browse.foreground_gc, 0, (Position)newy + adjust,
  		     XtWidth (w->browse.rhs_window_widget),
	             (Dimension)height, 0,
		     (Position)newy + newentry->height + adjust);
        };
    };


/*
**  Patch up the entry that used to be under the line...
*/
    if (adjust == 1)
       {
         lastentry = StructGetEntryPtr (w, w->browse.entries[offset]);
         LclOutlineDrawEntryTop (w, lastentry, (int)(newy + newentry->height));
       };


/*
**  Move all of the widgets down by oldentry->height pixels.  None of them get
**  exposed.
*/
    if (w->browse.sub_widgets_used)
       for (i = w->browse.display_count;  i >= offset;  i--)
          {
            entry = StructGetEntryPtr (w, w->browse.entries[i]);
            for (j = 0;  j <= entry->num_components - 1;  j++)
                if (entry->entrycompPtr[j].type == KcompWidget)
        	    SHOW_WIDGET (w, entry->entrycompPtr[j].var.is_widget.readwrite_text,
	 	                  XtX(entry->entrycompPtr[j].var.is_widget.readwrite_text),
                                  XtY(entry->entrycompPtr[j].var.is_widget.readwrite_text) + newentry->height,
				  FALSE);
          };


/*
**  Clear the area out that we will display to
*/
    XClearArea (XtDisplay(w),XtWindow(w->browse.lhs_window_widget),0,
	        (Position)newy+adjust,
	        XtWidth (w->browse.lhs_window_widget),
		(Dimension)newentry->height-adjust,FALSE);

    if (w->browse.display_mode == KdisplayColumns)
       XClearArea (XtDisplay(w),XtWindow(w->browse.rhs_window_widget),0,
	        (Position)newy+adjust,
	        XtWidth (w->browse.rhs_window_widget),
		(Dimension)newentry->height-adjust,FALSE);

    LclOutlineDrawEntry (w, newentry, newy);


/*
**  Call the expose procedure for each subwidget in the new entry since we may
**  be autoscrolling.
*/
    for (j = 0;  j <= newentry->num_components - 1;  j++)
	if (newentry->entrycompPtr[j].type == KcompWidget)
	    SHOW_WIDGET (w, newentry->entrycompPtr[j].var.is_widget.readwrite_text,
			 XtX(newentry->entrycompPtr[j].var.is_widget.readwrite_text),
			 XtY(newentry->entrycompPtr[j].var.is_widget.readwrite_text),
			 TRUE);

/*
**  Reflect the insertion in the structure.  We must use + 2 here so that
**  the path detection code below works.
*/
    StructOpenArray (w->browse.entries, sizeof(short), w->browse.display_count + 2, offset);
    StructOpenArray (w->browse.display_invalid, sizeof(Dimension), w->browse.display_count + 2, offset);


/*
**  Add in the new entry
*/
    w->browse.display_count++;
    w->browse.entries[offset] = entry_number;
    w->browse.display_invalid[offset] = 0;


/*
**  Buzz backwards to determine the number of entries that are part of the
**  path to the root.
*/
    if (w->browse.show_path_to_root)
       for (num_path = w->browse.display_count;  num_path > 1;  num_path--)
           if (w->browse.entries [num_path] != w->browse.entries[num_path-1] + 1)
              break;


/*
**  Truncate off any entries whose top is beyond the horizontal scroll bar
*/
    freey = w->browse.margin_height;

    for (j = 1;  j <= w->browse.display_count;  j++)
        {
          EntryPtr entry = StructGetEntryPtr (w, w->browse.entries[j]);
          if (freey > XtY(w->browse.hscroll))
             {
               if (w->browse.sub_widgets_used)
                  for (i = j;  i <= w->browse.display_count;  i++)
		      {
                        entry = StructGetEntryPtr (w, w->browse.entries[i]);
			for (k = 0;  k <= entry->num_components-1;  k++)
			    if (entry->entrycompPtr[k].type == KcompWidget)
				HIDE_WIDGET (w, entry->entrycompPtr[k].var.is_widget.readwrite_text);
                      };

               w->browse.display_count = j - 1;
               break;
             };
          freey = freey + entry->height;
        };


/*
**  If the path line is changing, and the line is still there, and we may need
**  to patch up the top of the entry...
*/
    if ((w->browse.show_path_to_root) && (w->browse.num_path != num_path-1))
       {
         if (w->browse.display_line_y)
            {
              LclOutlineClearLine (w);
              if (!w->browse.show_highlighting)
                 {
                   lastentry = StructGetEntryPtr (w, w->browse.entries[w->browse.num_path+1]);
                   if (lastentry->selected)
                      {
                        int y_position = LclOutlineMapOffsetToY (w, w->browse.num_path+1);
                        LclOutlineDrawEntryTop (w, lastentry, y_position);
                      };
                 };
            };
         w->browse.num_path = num_path - 1;
       };


/*
**  Call XSync to keep GraphicsExpose events in order
*/
#ifdef SYNC_ON_SCROLL
    XSync(XtDisplay(w),FALSE);
#endif


/*
**  See if we have had a graphic exposure event arrive.  This is caused when an
**  occluding window got in the way of the scroll.
*/
    w->browse.clips_window = XtWindow (w->browse.lhs_window_widget);
    DisplayGraphicsExpose (w);
    OutlineDrawExposed (w);

    if (w->browse.display_mode == KdisplayColumns)
       {
	w->browse.clips_window = XtWindow (w->browse.rhs_window_widget);
	DisplayGraphicsExpose (w);
	OutlineDrawExposed (w);
       };


/*
**  Redraw the line
*/
    LclOutlineDrawLine (w);


/*
**  Issue the callback for the display having changed.
*/
    temp.reason = CRDisplayChanged;
    XtCallCallbacks (w, NdisplayChangedCallback, &temp);
}

/*
**  Routine that shifts the screen to a new window_basex value.
*/
static void LclOutlineChangeBaseX (w, newvalue)

  dns_widget w;
  int newvalue;

{
/*
**  Local data declarations
*/
    Position src_x, dst_x, clr_x;
    Dimension clr_wid;
    int i, j, num_components;


/*
**  Max the value such that the widest known entry fits perfectly
*/
    if (newvalue + XtWidth (w->browse.lhs_window_widget) > w->browse.max_width)
       newvalue = w->browse.max_width - XtWidth (w->browse.lhs_window_widget);


/*
**  If the value is less than zero, then make it zero
*/
    if (newvalue < 0) newvalue = 0;


/*
**  If the base is already the same, then leave
*/
    if (w->browse.window_basex == newvalue) return;


/*
**  Compute the copy source, destination, etcetera if moving right...
*/
    if (w->browse.window_basex > newvalue)
       {
	 src_x   = 0;
         clr_x   = 0;
         dst_x   = w->browse.window_basex - newvalue;
         clr_wid = dst_x;
       };


/*
**  Compute the copy source, destination, etcetera if moving left...
*/
    if (w->browse.window_basex < newvalue)
       {
	 dst_x   = 0;
         src_x   = newvalue - w->browse.window_basex;
         clr_wid = src_x;
         clr_x   = XtWidth (w->browse.lhs_window_widget) - clr_wid;
       };


/*
**  The clearing area may exceed the actual window size if the distance that
**  we are moving exceeds the window size.  Adjust for this situation.  Note
**  that the addition of clr_x to the width is really a subtraction.
*/
    if (clr_x < 0)
       {
	 clr_wid = clr_wid + clr_x;
         clr_x   = 0;
       };


/*
**  Issue the copy and the clear.  Note that the heights and Y positions are always
**  the same regardless of copy direction.  Also note that the copy width is always
**  the width of the widget minus the width of the clear.
*/
    if (clr_wid < XtWidth (w->browse.lhs_window_widget))
       XCopyArea (XtDisplay(w),
		  XtWindow(w->browse.lhs_window_widget),
		  XtWindow(w->browse.lhs_window_widget),
		  w->browse.foreground_gc,
                  (Position)src_x,
   	          (Position)0,
   	          XtWidth (w->browse.lhs_window_widget) - clr_wid,
	          (Dimension)XtY(w->browse.hscroll),
	          (Position)dst_x,
	          (Position)0);


/*
**  Clear the area and setup a clip rectangle to handle the redisplay
*/
    XClearArea (XtDisplay(w),
		XtWindow(w->browse.lhs_window_widget),
		(Position)clr_x, 0,
	        (Dimension)clr_wid, (Dimension)XtY(w->browse.hscroll), FALSE);

    if (w->browse.clip_count < max_clips)
       {
        w->browse.clips[w->browse.clip_count].x = clr_x;
        w->browse.clips[w->browse.clip_count].y = 0;
        w->browse.clips[w->browse.clip_count].width = clr_wid;
        w->browse.clips[w->browse.clip_count].height = XtY(w->browse.hscroll);
        w->browse.clip_count++;
       }
    else
       {
        w->browse.clip_count = 0;
        w->browse.refresh_all = TRUE;
       };


/*
**  Commit the new basex value.
*/
    w->browse.window_basex = newvalue;


/*
**  Move all of the widgets to the correct x position
*/
    if (w->browse.sub_widgets_used)
       for (i = 1;  i <= w->browse.display_count;  i++)
          {
            Position basex = w->browse.margin_width;
            EntryPtr entry = StructGetEntryPtr (w, w->browse.entries[i]);
            if (w->browse.expect_highlighting)
                 basex = basex + (entry->level * w->browse.indent_margin) - w->browse.window_basex + 3;
            else basex = basex + (entry->level * w->browse.indent_margin) - w->browse.window_basex + 1;
            if ((w->browse.display_mode == KdisplayColumns) || w->browse.rhs_components_unmapped)
                 num_components = w->browse.start_column_component - 1;
            else num_components = entry->num_components;
            if ((w->browse.start_column_component == 0) || (num_components > entry->num_components))
                 num_components = entry->num_components;
            for (j = 0;  j <= num_components - 1;  j++)
                if (entry->entrycompPtr[j].type == KcompWidget)
                   SHOW_WIDGET (w, entry->entrycompPtr[j].var.is_widget.readwrite_text,
                                (Position)(basex+entry->entrycompPtr[j].x),
	 	                XtY(entry->entrycompPtr[j].var.is_widget.readwrite_text),
				FALSE);
          };


/*
**  Synchronized swimming...
*/
#ifdef SYNC_ON_SCROLL
    XSync(XtDisplay(w),FALSE);
#endif


/*
**  Catch any graphics expose event due to occluded windows.
*/
    w->browse.clips_window = XtWindow (w->browse.lhs_window_widget);
    DisplayGraphicsExpose (w);
    OutlineDrawExposed(w);


/*
**  Redraw the line
*/
    w->browse.display_line_y = 0;
    LclOutlineDrawLine (w);
}

/*
**  Routine that shifts the screen to a new rhs_base_x value.
*/
static void LclOutlineChangeRhsBaseX (w, newvalue)

  dns_widget w;
  int newvalue;

{
/*
**  Local data declarations
*/
    Position src_x, dst_x, clr_x;
    Dimension clr_wid;
    int i, j;


/*
**  Max the value such that the widest known entry fits perfectly
*/
    if (newvalue + XtWidth (w->browse.rhs_window_widget) > w->browse.rhs_max_width)
       newvalue = w->browse.rhs_max_width - XtWidth (w->browse.rhs_window_widget);


/*
**  If the value is less than zero, then make it zero
*/
    if (newvalue < 0) newvalue = 0;


/*
**  If the base is already the same, then leave
*/
    if (w->browse.rhs_base_x == newvalue) return;


/*
**  Compute the copy source, destination, etcetera if moving right...
*/
    if (w->browse.rhs_base_x > newvalue)
       {
	 src_x   = 0;
         clr_x   = 0;
         dst_x   = w->browse.rhs_base_x - newvalue;
         clr_wid = dst_x;
       };


/*
**  Compute the copy source, destination, etcetera if moving left...
*/
    if (w->browse.rhs_base_x < newvalue)
       {
	 dst_x   = 0;
         src_x   = newvalue - w->browse.rhs_base_x;
         clr_wid = src_x;
         clr_x   = XtWidth (w->browse.rhs_window_widget) - clr_wid;
       };


/*
**  The clearing area may exceed the actual window size if the distance that
**  we are moving exceeds the window size.  Adjust for this situation.  Note
**  that the addition of clr_x to the width is really a subtraction.
*/
    if (clr_x < 0)
       {
	 clr_wid = clr_wid + clr_x;
         clr_x   = 0;
       };


/*
**  Issue the copy and the clear.  Note that the heights and Y positions are always
**  the same regardless of copy direction.  Also note that the copy width is always
**  the width of the widget minus the width of the clear.
*/
    if (clr_wid < XtWidth (w->browse.rhs_window_widget))
       XCopyArea (XtDisplay(w),
		  XtWindow(w->browse.rhs_window_widget),
		  XtWindow(w->browse.rhs_window_widget),
		  w->browse.foreground_gc,
                  (Position)src_x,
   	          (Position)0,
   	          XtWidth (w->browse.rhs_window_widget) - clr_wid,
	          (Dimension)XtY(w->browse.hscroll),
	          (Position)dst_x,
	          (Position)0);


/*
**  Clear the area and setup a clip rectangle to handle the redisplay
*/
    XClearArea (XtDisplay(w),
		XtWindow(w->browse.rhs_window_widget),
		(Position)clr_x, 0,
	        (Dimension)clr_wid, (Dimension)XtY(w->browse.hscroll), FALSE);

    if (w->browse.clip_count < max_clips)
       {
        w->browse.clips[w->browse.clip_count].x = clr_x;
        w->browse.clips[w->browse.clip_count].y = 0;
        w->browse.clips[w->browse.clip_count].width = clr_wid;
        w->browse.clips[w->browse.clip_count].height = XtY(w->browse.hscroll);
        w->browse.clip_count++;
       }
    else
       {
        w->browse.clip_count = 0;
        w->browse.refresh_all = TRUE;
       };


/*
**  Commit the new basex value.
*/
    w->browse.rhs_base_x = newvalue;


/*
**  Move all of the widgets to the correct x position
*/
    if (w->browse.sub_widgets_used)
       for (i = 1;  i <= w->browse.display_count;  i++)
          {
            EntryPtr entry = StructGetEntryPtr (w, w->browse.entries[i]);
	    int column_x = w->browse.default_spacing / 2;
            if (entry->num_components >= w->browse.start_column_component)
	       if (w->browse.start_column_component != 0)
	          for (j = w->browse.start_column_component-1;  j <= entry->num_components - 1;  j++)
		      {
                       if (entry->entrycompPtr[j].type == KcompWidget)
			  SHOW_WIDGET (w, entry->entrycompPtr[j].var.is_widget.readwrite_text,
				       (Position)column_x - w->browse.rhs_base_x,
				       XtY(entry->entrycompPtr[j].var.is_widget.readwrite_text),
				       FALSE);

		       column_x = column_x + w->browse.column_widths [j] + w->browse.default_spacing;
		     };
          };


/*
**  Synchronized swimming...
*/
#ifdef SYNC_ON_SCROLL
    XSync(XtDisplay(w),FALSE);
#endif


/*
**  Catch any graphics expose event due to occluded windows.
*/
    w->browse.clips_window = XtWindow (w->browse.rhs_window_widget);
    DisplayGraphicsExpose (w);
    OutlineDrawExposed (w);


/*
**  Redraw the line
*/
    w->browse.display_line_y = 0;
    LclOutlineDrawLine (w);
}

/*
**  This routine will redisplay an entry in the widget window.  If we are
**  in Columns mode then we will draw both sides.
*/

static void LclOutlineDrawEntry (w, entry, basey)

  dns_widget w;
  EntryPtr entry;
  int basey;

{
/*
**  Local data declarations
*/
    GC gc;
    int basex, width;


/*
**  The following declarations are setup to tell us how to invert
**  selected entries.  If invert_right is true, then either all, none,
**  or a particular component is inverted.  The values are 99, 0, or
**  the component number.
*/
    Boolean invert_left  = FALSE;
    int     invert_right = 0;


/*
**  If we are not yet realized, then leave.
*/
    if (!XtIsRealized (w)) return;


/*
**  Setup the inversion values.  If there is no right hand side, then
**  just invert the left...
*/
    if ((entry->selected) && (w->browse.show_selections) && (w->browse.display_mode != KdisplayColumns))
        invert_left = TRUE;


/*
**  Do different things based on the selection mode
*/
    if ((entry->selected) && (w->browse.show_selections) && (w->browse.display_mode == KdisplayColumns))
       {
	/*
	**  Selection is on the left or range selected.
	*/
            if (entry->selected_comp < w->browse.start_column_component)
	       {
	         if (w->browse.selection_mode == KselectEntry)
		    {
	             invert_left  = TRUE;
		     invert_right = 99;
		    };

		 if (w->browse.selection_mode == KselectComp)
		    {
		     invert_left  = TRUE;
		     invert_right = 0;
		    };

		 if (w->browse.selection_mode == KselectCompAndLhs)
		    {
		     invert_left = TRUE;
		     invert_right = 0;
		    };

		 if (w->browse.selection_mode == KselectEntryOrComp)
		    {
	             invert_left  = TRUE;
		     invert_right = 99;
		    };
	       };


	/*
	**  Selection is on the right.
	*/
	    if (w->browse.start_column_component != 0)
	       if (entry->selected_comp >= w->browse.start_column_component)
		  {
		    if (w->browse.selection_mode == KselectEntry)
		       {
			invert_left  = TRUE;
			invert_right = 99;
		       };

		    if (w->browse.selection_mode == KselectComp)
		       {
			invert_left  = FALSE;
			invert_right = entry->selected_comp;
		       };

		    if (w->browse.selection_mode == KselectCompAndLhs)
		       {
			invert_left  = TRUE;
			invert_right = entry->selected_comp;
		       };

		    if (w->browse.selection_mode == KselectEntryOrComp)
		       {
			invert_left  = FALSE;
			invert_right = entry->selected_comp;
		       };
	          };
       };


/*
**  Compute the base x value based on the entries level number and on whether
**  the navigation bar is on the left side.
*/
    basex = w->browse.margin_width + (entry->level * w->browse.indent_margin) - w->browse.window_basex;


/*
**  Default the GC to either grayed or normal
*/
    if ((entry->grayed) || (XtIsSensitive(w) == FALSE))
         gc = w->browse.grey_gc;
    else gc = w->browse.foreground_gc;


/*
**  Invert the entire left hand side if it is selected and we own the global selections.
*/
    if (invert_left)
       {
        /*
        **  Compute the width of the rectangle...
        */
            width = entry->width;
            if (w->browse.fixed_width) width = XtWidth (w->browse.lhs_window_widget) - basex;

        /*
        **  Draw things differently if we are showing the highlighting...
        */
            XFillRectangle (XtDisplay(w),
			    XtWindow(w->browse.lhs_window_widget),
			    gc,
			    (Position)basex,
			    (Position)basey,
			    (Dimension)width,
			    (Dimension)entry->height);

        /*
        **  Change the GC
        */
            gc = w->browse.background_gc;
       };


/*
**  Draw each component of the entry.  The x and y coordinates are computed
**  by this routine.  If we expect highlighting draw the contents of the
**  entry slightly shifted to make room fo the highlight box when it appears.
**  Also add one to the base instead of adding one to each component position
**  inside the Draw routine.
*/
    if ((entry->selected) && (!invert_left))
	{
	 entry->selected = FALSE;
         if (w->browse.expect_highlighting)
              DisplayDrawEntry (w, XtWindow(w->browse.lhs_window_widget), entry, basex + 3, basey + 3);
         else DisplayDrawEntry (w, XtWindow(w->browse.lhs_window_widget), entry, basex + 1, basey + 1);
	 entry->selected = TRUE;
	}
    else
	{
         if (w->browse.expect_highlighting)
              DisplayDrawEntry (w, XtWindow(w->browse.lhs_window_widget), entry, basex + 3, basey + 3);
         else DisplayDrawEntry (w, XtWindow(w->browse.lhs_window_widget), entry, basex + 1, basey + 1);
        };


/*
**  If this is column mode, then draw the other side
*/
    if (w->browse.display_mode == KdisplayColumns)
       {
       DisplayDrawColumnsBox (w, entry, 0 - w->browse.rhs_base_x, basey, invert_right);
       if (w->browse.expect_highlighting)
            DisplayDrawColumns (w, entry, 0 - w->browse.rhs_base_x, basey + 3, invert_right);
       else DisplayDrawColumns (w, entry, 0 - w->browse.rhs_base_x, basey + 1, invert_right);
       };

/*
**  Box the entire entry if it is highlighted.
*/
    if ((entry->highlighted) && (w->browse.show_highlighting))
       {
         width = entry->width;
         if (w->browse.fixed_width) width = XtWidth (w->browse.lhs_window_widget) - basex;
         XDrawRectangle (XtDisplay(w),XtWindow(w->browse.lhs_window_widget), gc,
		         (Position)basex + 1, (Position)basey + 1,
			 (Dimension)width - 2, (Dimension)entry->height - 3);
       };
}

/*
** This routine will redisplay the top portion of an entry
*/

static void LclOutlineDrawEntryTop (w, entry, basey)

  dns_widget w;
  EntryPtr entry;
  int basey;

{
/*
**  Local data declarations
*/
    GC gc;
    int basex, width;


/*
**  If we are not yet realized, then leave.
*/
    if (!XtIsRealized (w)) return;


/*
**  Compute the base x value based on the entries level number and on whether
**  the navigation bar is on the left side.
*/
    basex = w->browse.margin_width + (entry->level * w->browse.indent_margin) - w->browse.window_basex;


/*
**  Compute the width of the rectangle...
*/
    width = entry->width;
    if (w->browse.fixed_width) width = XtWidth (w->browse.lhs_window_widget) - basex;


/*
**  Clear the whole line
*/
    XClearArea (XtDisplay(w), XtWindow(w->browse.lhs_window_widget), 0, (Position)basey,
	        XtWidth (w->browse.lhs_window_widget), (Dimension)1, FALSE);


/*
**  Decide whether to draw the meat of it as a forground or a background GC.
**  Default to showing the background and then switch if appropriate.
*/
    gc = w->browse.background_gc;

    if ((w->browse.show_selections) && (!w->browse.show_highlighting) && (entry->selected))
       gc = w->browse.foreground_gc;


/*
**  Draw the line in the computed GC
*/
    XDrawLine (XtDisplay(w), XtWindow(w->browse.lhs_window_widget), gc, basex, basey, basex + width - 1, basey);
    if (w->browse.display_mode == KdisplayColumns)
       XDrawLine (XtDisplay(w), XtWindow(w->browse.rhs_window_widget), gc, basex, basey, basex + width - 1, basey);
}

/*
**  This routine finds out what offset corresponds to a given X and Y location.
**  It returns the offset number or zero if no entry is displayed at that
**  X and Y location.
*/

static int LclOutlineFindOffset (w, find_x, find_y)

  dns_widget w;
  int find_x, find_y;

{
/*
**  Routine data declarations
*/
    int offset, left_x, top_y, bottom_y, found = FALSE;
    EntryPtr entry;


/*
**  If there are no entries being displayed...
*/
    if (w->browse.display_count == 0) return 0;


/*
**  The first entry is displayed at "margin_height"
*/
    top_y = w->browse.margin_height;
    if (find_y < top_y) return 0;


/*
**  Spin through the entries to find the offset.  Default 'found' to false.
*/
    for (offset = 1;  offset <= w->browse.display_count;  offset++)
        {
          entry = StructGetEntryPtr (w, w->browse.entries[offset]);
          bottom_y = top_y + entry->height;
          if (find_y < bottom_y) {found = TRUE;  break;};
          top_y = bottom_y;
        };

    if (found == FALSE) return 0;


/*
**  If the X position is not passed (zero value), then return the found
**  offset without checking the X position.
*/
    if (find_x == 0) return offset;


/*
**  If the X position is less than the starting display point, then return zero
*/
    left_x = w->browse.margin_width + (entry->level * w->browse.indent_margin) - w->browse.window_basex;
    if (find_x < left_x) return 0;


/*
**  If we have fixed width entries, then it was a hit.
*/
    if (w->browse.fixed_width) return offset;


/*
**  Now we must see if the X position is passed the end of the entry.
*/
    if (find_x <= left_x + entry->width) return offset;


/*
**  If it is in the right hand window, then return the offset.
*/
    if (find_x > XtWidth (w->browse.lhs_window_widget)) return offset;


/*
**  It was beyond the right side of the entry
*/
    return 0;
}

/*
**  This routine is given a display offset and must calculate the Y position of
**  that offset.  This routine can be passed an offset between 1 and the number
**  of displayed entries plus 1.
*/

int LclOutlineMapOffsetToY (w, offset)

  dns_widget w;
  int offset;

{
/*
**  Local data declarations
*/
    register int i, top_y;


/*
**  The first position starts at "margin_height"
*/
    top_y = w->browse.margin_height;


/*
**  Loop from 1 to just one shy of the desired offset.  In the loop add in the
**  height of the entry at that offset.  In order to stop one before the offset
**  the loop uses a less than operator.
*/
    for (i = 1;  i < offset;  i++)
        {
          EntryPtr entry = StructGetEntryPtr (w, w->browse.entries[i]);
          top_y = top_y + entry->height;
        };


/*
**  Return the y position
*/
    return top_y;
}

/*
**  This routine searches the entries being displayed and returns the offset
**  at which a particular entry was found or zero if not found.
*/

int LclOutlineMapNumberToOffset (w, entry_number)

  dns_widget w;
  int entry_number;

{
/*
**  Local data declarations
*/
    int offset;


/*
**  Loop through the path to the root entries plus the first entry.  If it's
**  less than the current entry, but not equal to, then it was not found.
*/
    for (offset = 1;  offset <= w->browse.num_path + 1;  offset++)
        if (entry_number < w->browse.entries[offset]) return 0;
	else
	    if (entry_number == w->browse.entries[offset]) return offset;


/*
**  The entry is in the main display portion.  We can compute the offset based
**  on the first entry after the path to the root.
*/
    offset = entry_number - w->browse.entries [w->browse.num_path+1];
    offset = offset + w->browse.num_path + 1;


/*
**  If this offset is not being displayed (off the bottom), then return 0.
*/
    if (offset > w->browse.display_count) offset = 0;


/*
**  Return the offset
*/
    return offset;
}

/*
**  This local routine adds an entry after the last entry if it will fit.  It
**  updates the number being displayed and returns TRUE/FALSE as to whether it
**  fit.
*/

static int LclOutlineEntryAppend (w, entry_number)

  dns_widget w;
  int entry_number;

{
/*
**  Local data declarations
*/
    EntryPtr newentry;
    int lasty;


/*
**  Get the Y position for the appendage
*/
    lasty = LclOutlineMapOffsetToY (w, w->browse.display_count + 1);


/*
**  If the Y position goes beyond the horizontal scroll bar, then forget it.
*/
    if (lasty > XtY(w->browse.hscroll))
	return FALSE;


/*
**  Get the entry pointer for the new entry.  Note that we sort of disable and
**  reenable around this code so that we don't go recursive...
*/
    w->browse.disabled_count++;
    newentry = StructGetValidEntryPtr (w, entry_number);
    w->browse.disabled_count--;


/*
**  Display the new entry at the bottom
*/
    LclOutlineDrawEntry (w, newentry, lasty);


/*
**  Change the display structure and counter
*/
    w->browse.display_count++;
    w->browse.entries[w->browse.display_count] = entry_number;
    w->browse.display_invalid[w->browse.display_count] = 0;


/*
**  Return a success status
*/
    return TRUE;
}

/*
**  This local routine erases the path to root line (if present).  It takes
**  care of reestablishing the selected area if needed.
*/

static void LclOutlineClearLine (w)

  dns_widget w;

{
/*
**  If there isn't a line, then return.
*/
    if (w->browse.display_line_y == 0) return;


/*
**  Erase the old line if it is present and we are realized
*/
    if (XtIsRealized (w))
       {
         XDrawLine (XtDisplay(w),
		    XtWindow(w->browse.lhs_window_widget),
		    w->browse.background_gc,
                    w->browse.margin_width - w->browse.window_basex,
                    w->browse.display_line_y,
	            XtWidth (w->browse.lhs_window_widget),
                    w->browse.display_line_y);

         if (w->browse.display_mode == KdisplayColumns)
            XDrawLine (XtDisplay(w),
		    XtWindow(w->browse.rhs_window_widget),
		    w->browse.background_gc,
                    0,
                    w->browse.display_line_y,
	            XtWidth (w->browse.rhs_window_widget),
                    w->browse.display_line_y);
       };


/*
**  Show that the line has gone away.
*/
    w->browse.display_line_y = 0;
}

/*
**  This routine establishes and draws the path to the root line.
*/

static void LclOutlineDrawLine (w)

  dns_widget w;

{
/*
**  If there are no entries in the path to root, then return
*/
    if (w->browse.num_path == 0) return;


/*
**  If the line is already drawn, then ignore this request
*/
    if (w->browse.display_line_y != 0) return;


/*
**  Compute the y position for the line.
*/
    w->browse.display_line_y = LclOutlineMapOffsetToY (w, w->browse.num_path + 1);


/*
**  Draw the line.
*/
    if (XtIsRealized (w))
       {
         XDrawLine (XtDisplay(w),
		    XtWindow(w->browse.lhs_window_widget),
		    w->browse.foreground_gc,
                    w->browse.margin_width - w->browse.window_basex,
		    w->browse.display_line_y,
		    XtWidth (w->browse.lhs_window_widget),
		    w->browse.display_line_y);

         if (w->browse.display_mode == KdisplayColumns)
            XDrawLine (XtDisplay(w),
		    XtWindow(w->browse.rhs_window_widget),
		    w->browse.foreground_gc,
                    0,
		    w->browse.display_line_y,
		    XtWidth (w->browse.rhs_window_widget),
		    w->browse.display_line_y);
       };
}

/*
**  This routine returns true or false if we are at the bottom of the display.
**  The reason that it is now a routine is because of displaying partial
**  entries.
*/

static int LclOutlineAtBottom (w)

  dns_widget w;

{
/*
**  Local data declarations
*/
    EntryPtr entry;
    int y;


/*
**  If there are no entries at all, then we must be at the bottom.
*/
    if (w->browse.num_entries == 0)
       return TRUE;


/*
**  If we are not displaying the last entry, then we cannot be at the bottom.
**  We check for less than and not equal to because of partial entry displays.
*/
    if (w->browse.entries[w->browse.display_count] < w->browse.num_entries)
       return FALSE;


/*
**  If we are displaying the last entry, then see if we are displaying the
**  whole entry.
*/
    entry = StructGetEntryPtr (w, w->browse.entries[w->browse.display_count]);


/*
**  Get the y position of this entry.
*/
    y = LclOutlineMapOffsetToY (w, w->browse.display_count);


/*
**  If the y plus the height is above the horizontal scroll bar then we are
**  displaying the whole thing... and are at the bottom...
*/
    if (y + entry->height <= XtY(w->browse.hscroll))
       return TRUE;


/*
**  Return that we are not at the bottom yet...
*/
    return FALSE;
}

/*
**  This routine is called when size of the vertical scroll bar needs changed.
**  We will not change the slider when the path to the root line changes, but
**  only in response to an add entries, delete entries, or resize.
**
**  We will change the scroll bar only when the size actually changes...
*/

static void LclOutlineScrollAdjust (w)

  dns_widget w;

{
/*
**  Local data declarations.  The normal names are unnormalized while the
**  names prefixed with n are the normalized (1 to 100) values;
*/
    Arg arglist[4];
    int argcount, cvalue, cshown, cminim, cmaxim, value, shown, minim, maxim;


/*
**  If we are not yet realized, then leave.
*/
    if (!XtIsRealized (w)) return;


/*
**  Get the current vertical scroll bar values.
*/
    XtSetArg(arglist[0], XmNvalue,   &cvalue);
    XtSetArg(arglist[1], XmNsliderSize, &cshown);
    XtSetArg(arglist[2], XmNminimum, &cminim);
    XtSetArg(arglist[3], XmNmaximum, &cmaxim);

    XtGetValues (w->browse.vscroll, arglist, 4);


/*
**  If less than 5 entries are known about, then fake it...
*/
    if (w->browse.num_entries < 5)
       {
         value = 1;
         shown = 4;
         minim = 1;
         maxim = 5;
       };


/*
**  If everything is not being shown, then we will set the real values.
*/
    if (w->browse.num_entries >= 5)
       {
         value = w->browse.entries[w->browse.num_path+1];
         shown = w->browse.display_count - w->browse.num_path;
         if (shown < 4) shown = 4;
         minim = 1;
         maxim = w->browse.num_entries + 1;
       };


/*
**  If everything is shown make sure that we do not include a half an entry
**  in the slider size.
*/
    if (w->browse.num_entries >= 5)
       {
         EntryPtr entry;
         Position y;
         entry = StructGetEntryPtr (w, w->browse.entries[w->browse.display_count]);
         y = LclOutlineMapOffsetToY (w, w->browse.display_count);
         if (y + entry->height > XtY(w->browse.hscroll))
            shown = shown - 1;
       };


/*
**  Set the vertical scrollbar values if they are different
*/
    argcount = 0;

    if (cvalue != value) {XtSetArg(arglist[argcount], XmNvalue,   value);  argcount++; };
    if (cshown != shown) {XtSetArg(arglist[argcount], XmNsliderSize, shown);  argcount++; };
    if (cminim != minim) {XtSetArg(arglist[argcount], XmNminimum, minim);  argcount++; };
    if (cmaxim != maxim) {XtSetArg(arglist[argcount], XmNmaximum, maxim);  argcount++; };

    if (argcount != 0) XtSetValues (w->browse.vscroll, arglist, argcount);


/*
**  Get the current horizontal scroll bar values.
*/
    XtSetArg(arglist[0], XmNvalue,   &cvalue);
    XtSetArg(arglist[1], XmNsliderSize, &cshown);
    XtSetArg(arglist[2], XmNminimum, &cminim);
    XtSetArg(arglist[3], XmNmaximum, &cmaxim);

    XtGetValues (w->browse.hscroll, arglist, 4);


/*
**  Compute the values needed for the horizontal scroll bar.
*/
    value =  w->browse.window_basex;
    shown =  XtWidth (w->browse.lhs_window_widget);
    minim =  0;


/*
**  Compute the maximum value for the horizontal scrollbar.  This has to take
**  into account the case where the widest entry right hand side is in view or
**  not in view.
*/
    if ((w->browse.max_width - w->browse.window_basex) <= XtWidth (w->browse.lhs_window_widget))
         maxim =  XtWidth (w->browse.lhs_window_widget) + w->browse.window_basex;
    else maxim =  w->browse.max_width;


/*
**  Set the horizontal scrollbar values if they are different
*/
    argcount = 0;

    if (cvalue != value) {XtSetArg(arglist[argcount], XmNvalue,   value);  argcount++; };
    if (cshown != shown) {XtSetArg(arglist[argcount], XmNsliderSize, shown);  argcount++; };
    if (cminim != minim) {XtSetArg(arglist[argcount], XmNminimum, minim);  argcount++; };
    if (cmaxim != maxim) {XtSetArg(arglist[argcount], XmNmaximum, maxim);  argcount++; };

    if (argcount != 0) XtSetValues (w->browse.hscroll, arglist, argcount);


/*
**  Do the RHS if necessary
*/
    if (w->browse.display_mode == KdisplayColumns)
       {
	/*
	**  Get the current horizontal scroll bar values.
	*/
	    XtSetArg(arglist[0], XmNvalue,   &cvalue);
	    XtSetArg(arglist[1], XmNsliderSize, &cshown);
	    XtSetArg(arglist[2], XmNminimum, &cminim);
	    XtSetArg(arglist[3], XmNmaximum, &cmaxim);

	    XtGetValues (w->browse.rhs_hscroll, arglist, 4);


	/*
	**  Compute the values needed for the horizontal scroll bar.
	*/
	    value =  w->browse.rhs_base_x;
	    shown =  XtWidth (w->browse.rhs_window_widget);
	    minim =  0;


	/*
	**  Compute the maximum value for the horizontal scrollbar.  This has to take
	**  into account the case where the widest entry right hand side is in view or
	**  not in view.
	*/
	    if ((w->browse.rhs_max_width - w->browse.rhs_base_x) <= XtWidth (w->browse.rhs_window_widget))
		 maxim =  XtWidth (w->browse.rhs_window_widget) + w->browse.rhs_base_x;
	    else maxim =  w->browse.rhs_max_width;


	/*
	**  Set the horizontal scrollbar values if they are different
	*/
	    argcount = 0;

	    if (cvalue != value) {XtSetArg(arglist[argcount], XmNvalue,   value);  argcount++; };
	    if (cshown != shown) {XtSetArg(arglist[argcount], XmNsliderSize, shown);  argcount++; };
	    if (cminim != minim) {XtSetArg(arglist[argcount], XmNminimum, minim);  argcount++; };
	    if (cmaxim != maxim) {XtSetArg(arglist[argcount], XmNmaximum, maxim);  argcount++; };

	    if (argcount != 0) XtSetValues (w->browse.rhs_hscroll, arglist, argcount);
	};
}

/*
**  This callback routine is invoked from all of the buttons and the scroll
**  bars when the user requests help.
*/

void OutlineScrollHelp (wi, unused_tag, scroll)

  Widget wi;
  int unused_tag;
  XmScrollBarCallbackStruct *scroll;

{
/*
**  The parent of all of them is the widget
*/
    dns_widget w = StructFindWidget (wi);
    CallbackStruct temp;


/*
**  Set up the structure...  Set the component number field to the widget id of
**  the scroll bar making the call.  This is for VMSmail.
*/
    temp.reason = CRHelpRequested;
    if (wi == w->browse.nav_button)
	temp.entry_number = KhelpNavButton;
    else
	temp.entry_number = KhelpScroll;
    temp.time = scroll->event->xbutton.time;
    temp.component_number = (int) wi;


/*
**  Make the callback
*/
    XtCallCallbacks (w, NhelpRequestedCallback, &temp);
}

/*
**  This callback routine gets all of the vertical scroll bar callbacks.  We
**  will either handle the work here or dispatch to a local routine.
*/

void OutlineVScroll (wi, unused_tag, scroll)

  Widget wi;
  int unused_tag;
  XmScrollBarCallbackStruct *scroll;

{
/*
**  The parent of the vertical scroll bar is the widget
*/
    dns_widget w = StructFindWidget (wi);


/*
**  As a workaround to the multiple callback bug in the scrollbar.  If scroll
**  is already in progress, then ignore this call and return.  Otherwise set
**  the VScrollInProgress flag in case we get called before this routine
**  completes (If we have to do a GetEntry callback the application callback
**  routine may dispatch some events).
*/
    if (w->browse.vscroll_in_progress) return;
    w->browse.vscroll_in_progress = TRUE;


/*
**  If the widget is empty, then ignore all calls from the scrollbar
*/
    if (w->browse.num_entries == 0)
       {
	  w->browse.vscroll_in_progress = FALSE;
          return;
       };


/*
**  Adjust the value if it is greater than the number of entries.  This is
**  a necessary step...
*/
    if (scroll->value > w->browse.num_entries) scroll->value = w->browse.num_entries;


/*
**  XmCR_VALUE_CHANGED reason code.  We'll only use this when the index window is
**  being used...  This will always be indicated by a shown value of non-zero
**  which is guaranteed true after a call to the dragging routine...
*/
    if ((scroll->reason == XmCR_VALUE_CHANGED) && (w->browse.index_window_shown == 0))
	{
	  w->browse.vscroll_in_progress = FALSE;
	  return;
	}


    if (scroll->reason == XmCR_VALUE_CHANGED)
       {
         if (scroll->value != w->browse.entries[w->browse.num_path+1])
	    {
	    if (scroll->value == 1) OutlinePositionDisplay (w,1,KpositionTop);
            else {
		 int entry_number;
		 entry_number = scroll->value + (w->browse.display_count - w->browse.num_path);
		 if (entry_number > w->browse.num_entries) entry_number = w->browse.num_entries;
                 OutlinePositionDisplay (w,entry_number,KpositionBottom);
   	         };
	    };
         if (w->browse.index_window != 0) XtDestroyWidget (w->browse.index_window);
         w->browse.index_window = 0;
         w->browse.index_window_shown = 0;
	 w->browse.vscroll_in_progress = FALSE;
         return;
       };


/*
**  Case on the reason code for the rest
*/
    switch (scroll->reason)

      {
        case XmCR_INCREMENT  : LclOutlineScrollUp (w);
                               LclOutlineScrollAdjust (w);
			       break;

        case XmCR_DECREMENT  : LclOutlineScrollDown (w);
                               LclOutlineScrollAdjust (w);
			       break;

        case XmCR_PAGE_INCREMENT  : DisplaySetWatch (w, TRUE);
				    OutlinePositionDisplay (w, 0, KpositionNextPage);
				    DisplaySetWatch (w, FALSE);
				    break;

        case XmCR_PAGE_DECREMENT  : DisplaySetWatch (w, TRUE);
				    OutlinePositionDisplay (w, 0, KpositionPreviousPage);
				    DisplaySetWatch (w, FALSE);
				    break;

        case XmCR_DRAG     : LclOutlineVScrollDrag (w, scroll->value);
			     break;

        case XmCR_TO_TOP    : if (w->browse.num_entries > 4)
				{
			          int offset, entry_number;
			          offset = LclOutlineFindOffset (w, 0, scroll->pixel);
                                  if (offset == 0) entry_number = scroll->value;
				  else entry_number = w->browse.entries[offset];
                                  if (entry_number != w->browse.entries[w->browse.num_path+1])
                                     OutlinePositionDisplay (w, entry_number, KpositionTop);
                                };
                             break;

        case XmCR_TO_BOTTOM : if (w->browse.num_entries>4)
				{
			          int offset, entry_number;
			          offset = LclOutlineFindOffset (w, 0, scroll->pixel);
                                  if (offset == 0) entry_number = scroll->value;
				  else entry_number = w->browse.entries[offset];
                                  OutlinePositionDisplay (w, entry_number, KpositionBottom);
                                };
                             break;
      };


    w->browse.vscroll_in_progress = FALSE;
}

/*
**  This routine gets all of the horizontal scroll callbacks for the
**  right hand side window.
*/

void LclOutlineRhsHScroll (w, scroll)

  dns_widget w;
  XmScrollBarCallbackStruct *scroll;

{
/*
**  Switch on the reason code
*/
    switch (scroll->reason)

      {
        case XmCR_VALUE_CHANGED  : LclOutlineChangeRhsBaseX (w, scroll->value);
                                   LclOutlineScrollAdjust (w);
                                   break;

        case XmCR_INCREMENT      : LclOutlineChangeRhsBaseX (w, w->browse.rhs_base_x+10);
                                   LclOutlineScrollAdjust (w);
                                   break;

        case XmCR_DECREMENT      : LclOutlineChangeRhsBaseX (w, w->browse.rhs_base_x-10);
                                   LclOutlineScrollAdjust (w);
                                   break;

        case XmCR_PAGE_INCREMENT : LclOutlineChangeRhsBaseX (w,
					w->browse.rhs_base_x+(XtWidth(w->browse.rhs_window_widget)/2));
				   LclOutlineScrollAdjust (w);
				   break;

        case XmCR_PAGE_DECREMENT : LclOutlineChangeRhsBaseX (w,
					w->browse.rhs_base_x-(XtWidth(w->browse.rhs_window_widget)/2));
                                   LclOutlineScrollAdjust (w);
                                   break;
      };
}

/*
**  This routine gets all of the horizontal scroll callbacks.
*/

void OutlineHScroll (wi, unused_tag, scroll)

  Widget wi;
  int unused_tag;
  XmScrollBarCallbackStruct *scroll;

{
/*
**  The parent of the vertical scroll bar is the widget
*/
    dns_widget w = StructFindWidget (wi);


/*
**  As a workaround to the multiple callback bug in the scrollbar.  If scroll
**  is already in progress, then ignore this call and return.  Otherwise set
**  the HScrollInProgress flag in case we get called before this routine
**  completes (If we have to do a GetEntry callback the application callback
**  routine may dispatch some events).
*/
    if (w->browse.hscroll_in_progress) return;
    w->browse.hscroll_in_progress = TRUE;


/*
**  See if this is the RHS scroll bar...
*/
    if (wi == w->browse.rhs_hscroll)
       {
          LclOutlineRhsHScroll (w, scroll);
	  w->browse.hscroll_in_progress = FALSE;
          return;
       };


/*
**  Switch on the reason code
*/
    switch (scroll->reason)

      {
        case XmCR_VALUE_CHANGED  : LclOutlineChangeBaseX (w, scroll->value);
                                   LclOutlineScrollAdjust (w);
                                   break;

        case XmCR_INCREMENT      : LclOutlineChangeBaseX (w, w->browse.window_basex+10);
                                   LclOutlineScrollAdjust (w);
                                   break;

        case XmCR_DECREMENT      : LclOutlineChangeBaseX (w, w->browse.window_basex-10);
                                   LclOutlineScrollAdjust (w);
                                   break;

        case XmCR_PAGE_INCREMENT : LclOutlineChangeBaseX (w,
					w->browse.window_basex+(XtWidth(w->browse.lhs_window_widget)/2));
                                   LclOutlineScrollAdjust (w);
                                   break;

        case XmCR_PAGE_DECREMENT : LclOutlineChangeBaseX (w,
					w->browse.window_basex-(XtWidth(w->browse.lhs_window_widget)/2));
                                   LclOutlineScrollAdjust (w);
                                   break;
      };


    w->browse.hscroll_in_progress = FALSE;
}

/*
**  This routine is called when the user is dragging the slider in the vertical
**  scroll bar.
*/

static void LclOutlineVScrollDrag (w, value)

  dns_widget w;
  int value;

{
/*
**  Local data declarations
*/
    EntryPtr window_entry;
    XmString cs;
    Arg arglist[5];
    int i, window_number;


/*
**  If we don't need an index window, then set the index_window_shown
**  value to non zero to show that we have done some drag operation.
*/
    if (w->browse.index_window_needed == FALSE)
       {
         w->browse.index_window_shown = 1;
         return;
       };


/*
**  See if we have our index window.  Set XmNrecomputeSize to false so that
**  the widget doesn't resize itself each time to hold the label string
**  (it stays a fixed length).
*/
    if (w->browse.index_window == 0)
       {
         XtSetArg(arglist[0], XmNalignment, XmALIGNMENT_BEGINNING);
         XtSetArg(arglist[1], XmNborderWidth, 3);
         XtSetArg(arglist[2], XmNwidth, (XtWidth(w)/2));
         XtSetArg(arglist[3], XmNx, (XtWidth (w)/2) - 3);
         XtSetArg(arglist[4], XmNrecomputeSize, FALSE);
         w->browse.index_window = (Widget) XmCreateLabel (w, "IndexNotFound", arglist, 5);
         XtManageChild (w->browse.index_window);
         w->browse.index_window_shown = 0;
       };


/*
**  Get to the desired entry
*/
    window_number = value;
    window_entry = StructGetEntryPtr (w, window_number);


/*
**  Now go backwards until we find an indexable entry...
*/
    while (!window_entry->index_window)
       {
         if (window_number == 1) break;
         window_number--;
         window_entry = StructGetEntryPtr (w, window_number);
       };


/*
**  If this is the one currently being shown, then we are done.
*/
    if (w->browse.index_window_shown == window_number) return;
    w->browse.index_window_shown = window_number;


/*
**  Get the entry valid if necessary.  We CANNOT go into the fix screen up code.
*/
    w->browse.disabled_count++;
    window_entry = StructGetValidEntryPtr (w, window_number);
    w->browse.disabled_count--;


/*
**  Now we need a pointer to the first text component of that entry.  We will
**  use this in the label.
*/
    for (i = 0;  i <= window_entry->num_components-1;  i++)
        if (window_entry->entrycompPtr[i].type == KcompText)
           if (strcmp(window_entry->entrycompPtr[i].var.is_text.text, " ") != 0)
              break;


/*
**  If we did not find a text component, then leave
*/
    if (window_entry->entrycompPtr[i].type != KcompText) return;


/*
**  Set the current value
*/
    cs = XmStringCreate(window_entry->entrycompPtr[i].var.is_text.text, XmSTRING_DEFAULT_CHARSET);
    XtSetArg (arglist[0], XmNlabelString, cs);
    XtSetValues (w->browse.index_window, arglist, 1);
    XmStringFree (cs);
}

/*
**  This routine processes the top and bottom buttons.
**
**       1 - MB1 on top button
**       2 - SHIFT/MB1 on top button
**       3 - MB1 on bottom button
**       4 - SHIFT/MB1 on bottom button
*/

static void LclOutlineScrollButton (w)

    dns_widget w;

{
/*
**  Local data declarations
*/
    EntryPtr entry;
    int entry_number, entry_level;


/*
**  If it's the top button and we're already at the top, then leave
*/
    if (w->browse.button_top <= 2)
       if (w->browse.entries[w->browse.num_path+1] == 1)
          return;


/*
**  If it's the bottom button and we're already at the bottom, then leave
*/
    if (w->browse.button_top > 2)
       if (LclOutlineAtBottom (w))
          return;


/*
**  Start the entry pointers at the top or the bottom entry.
*/
    if (w->browse.button_top <= 2) entry_number = w->browse.entries[w->browse.num_path+1];
    else entry_number = w->browse.entries[w->browse.display_count];


/*
**  Get the entry
*/
    entry = StructGetEntryPtr (w, entry_number);


/*
**  If we are looking at the last entry and it is partially displayed, then
**  back off by one entry.
*/
    if (entry_number == w->browse.entries[w->browse.display_count])
       {
         Position y = LclOutlineMapOffsetToY (w, w->browse.display_count);
         if (y + entry->height > XtY(w->browse.hscroll))
            {
              entry_number = entry_number - 1;
              entry = StructGetEntryPtr (w, entry_number);
            };
       };


/*
**  Get the entry's level
*/
    entry_level = entry->level;


/*
**  Compute the entry number for the top button without the shift key
*/
    if (w->browse.button_top == 1)
       {
        /*
        **  Go backwards looking for the first entry whose level is different
        */
            if (w->browse.index_window_needed == TRUE)
               while (TRUE)
                  {
                    if (entry_number == 1) break;
                    entry = entry->prev;  entry_number--;
                    if (entry->index_window == TRUE) break;
                  };

        /*
        **  Go backwards looking for the first entry whose level is different
        */
            if (w->browse.index_window_needed == FALSE)
               while (TRUE)
                  {
                    if (entry_number == 1) break;
                    entry = entry->prev;  entry_number--;
                    if (entry->level != entry_level) break;
                  };
       };


/*
**  Compute the entry number for the top button with the shift key
*/
    if (w->browse.button_top == 2)
       while (TRUE)
          {
            if (entry_number == 1) break;
            entry = entry->prev;  entry_number--;
            if (entry->level == 0) break;
          };


/*
**  Bottom button, no shift key...
*/
    if (w->browse.button_top == 3)
       {
         /*
         **  Go forward looking for the next expandable entry
         */
             if (w->browse.index_window_needed == TRUE)
                while (TRUE)
                  {
                    if (entry_number == w->browse.num_entries) break;
                    entry = entry->next;  entry_number++;
                    if (entry->index_window == TRUE) break;
                  };


         /*
         **  Go forwards looking for the first entry whose level is different
         */
             if (w->browse.index_window_needed == FALSE)
                while (TRUE)
                  {
                    if (entry_number == w->browse.num_entries) break;
                    entry = entry->next;  entry_number++;
                    if (entry->level != entry_level) break;
                  };
       };


/*
**  Shift Bottom button
*/
    if (w->browse.button_top == 4)
       while (TRUE)
          {
            if (entry_number == w->browse.num_entries) break;
            entry = entry->next;  entry_number++;
            if (entry->level == 0) break;
          };


/*
**  Finish up with a position top or position bottom call.
*/
    if (w->browse.button_top <= 2)
         OutlinePositionDisplay (w, entry_number, KpositionTop);
    else OutlinePositionDisplay (w, entry_number, KpositionBottom);
}

/*
**  This routine is called when the user presses the button on the outer parts
**  of the scrollbar.
*/

void OutlineScrollButtonDown (wi, unused_event, argv)

    Widget       wi;
    XButtonEvent *unused_event;
    char **argv;

{
/*
**  Local data declarations
*/
    dns_widget w = StructFindWidget (wi);


/*
**  Save this event information
*/
    w->browse.button_down = TRUE;
    w->browse.button_top  = atoi(*argv);


/*
**  Process the button
*/
    LclOutlineScrollButton (w);


/*
**  Set the scrolling timer...
*/
    w->browse.button_timerid = XtAppAddTimeOut (XtWidgetToApplicationContext(w), w->browse.button_waitms,
                                                LclOutlineScrollTimeout, w);
}

/*
**  This routine is called when the user releases the button on the scrollbar.
*/

void OutlineScrollButtonUp  (wi)

    Widget wi;

{
/*
**  Local data declarations
*/
    dns_widget w = StructFindWidget (wi);


/*
**  Save this event information
*/
    w->browse.button_down = FALSE;


/*
**  Cancel the timer...
*/
    if (w->browse.button_timerid != 0)
       {
         XtRemoveTimeOut(w->browse.button_timerid);
         w->browse.button_timerid = 0;
       };
}

/*
**  This routine will get called when the timeout on scroll buttons happen
*/

static void LclOutlineScrollTimeout (w)

    dns_widget w;

{
/*
**  If the button is not down, then leave
*/
    if (w->browse.button_down == FALSE)  return;


/*
**  Process the button
*/
    LclOutlineScrollButton (w);


/*
**  Reset the scrolling timer...
*/
    w->browse.button_timerid = XtAppAddTimeOut (XtWidgetToApplicationContext(w), w->browse.button_repeatms,
                                                LclOutlineScrollTimeout, w);
}

