/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cds_selections.c,v $
 * Revision 1.1.4.2  1996/02/18  19:25:02  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:08:41  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:03:06  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:37:08  root]
 * 
 * Revision 1.1.2.2  1992/12/30  12:38:58  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:22:56  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:23:33  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#if !defined(lint) && !defined(SABER)
static char *SccsID = "@(#)cds_selections.c\t5\t(DECdns)\t8/7/1991";
#endif	/* neither lint nor saber-C */
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

#define  CDS_SELECTIONS
#ifdef VMS
/*#pragma nostandard*/
#include <XmP.h>
/*#pragma standard*/
#else
#include <Xm/XmP.h>
#endif

#include <cds_decwmp.h>
#include <cds_private.h>


#define not_first     0
#define first_of_one  1
#define first_of_many 3

/*
**  Local Routine declarations
*/
static    void LclSelectEntry    ();
static    void LclUnselectEntry  ();
static    void LclRedrawSelected ();

static    void LclHighlightEntry    ();
static    void LclUnhighlightEntry  ();
static    void LclRedrawHighlighted ();


/*
**  The following local procedure determines if the entry is selected based
**  on the multiple selection modes.  If we are not in column mode, then this
**  is exactly equivalent to the selected field in the entry.
*/
    Boolean LclEntrySelected ();

/*
** Here is the GetNumSelections public entry point.
*/

int GetNumSelections (w)

  dns_widget w;

{
/*
**  Return to the caller the current number of selections.
*/
    return w->browse.num_selections;
}

/*
** Here is the GetNumHighlighted public entry point.
*/

int GetNumHighlighted (w)

  dns_widget w;

{
/*
**  Return to the caller the current number of highlighted entries.
*/
    return w->browse.num_highlighted;
}

/*
**  Here is the GetSelections public entry point.
**
**  Go thru the entire list, if an entry is selected add the entry number to
**  the selections array.  If we get to the end of the list before all the
**  selections were found, issue a warning and return.  If there are any
**  remaining elements in the array after the selections were found, set them
**  to NULL.
**
**  If the array passed by the application is shorter than the number of
**  selections, then give the application only the first portion.
*/

void GetSelections (w, selections, comps, entry_tags, num_array_entries)

  dns_widget w;
  int *selections;
  int *comps;
  unsigned int *entry_tags;
  int num_array_entries;

{
/*
**  Local data
*/
    EntryPtr entry;
    int i, entry_number, number_to_copy;


/*
**  Figure out the number of entries to copy based on the length of the array
**  and the number of entries that are selected.
*/
    if (w->browse.num_selections > num_array_entries)
         number_to_copy = num_array_entries;
    else number_to_copy = w->browse.num_selections;


/*
**  Get the first entry, so the next entry interface can be used.
*/
    entry_number = 1;
    entry = w->browse.entryPtr;


/*
**  Loop thru the entries, setting the arrays if the entry is selected
*/
    for (i = 0;  i < number_to_copy;  i++)
        {
	  while (!entry->selected)
             {
               entry = entry->next;
               entry_number++;
             };

	  selections[i] = entry_number;

	  if (comps != (int *) NULL) comps[i] = entry->selected_comp;
	  if (entry_tags != (unsigned int *) NULL) entry_tags[i] = entry->entry_tag;

	  entry = entry->next;
	  entry_number++;
	};


/*
**  Set any remaining array elements to null
*/
    for (i = number_to_copy + 1;  i < num_array_entries;  i++)
        {
          selections[i] = 0;
          if (comps != (int *) NULL) comps[i] = 0;
          if (entry_tags != (unsigned int *) NULL) entry_tags[i] = 0;
        };
}

/*
**  Here is the GetHighlighted public entry point.
**
**  Go thru the entire list, if an entry is highlighted add the entry number to
**  the highlighted array.  If we get to the end of the list before all the
**  highlighted entries were found, issue a warning and return.  If there are
**  any remaining elements in the array after the highlighted entries were
**  found, set them to NULL.
**
**  If the array passed by the application is shorter than the number of
**  highlighted entries, then give the application only the first portion.
*/

void GetHighlighted (w, highlighted, entry_tags, num_array_entries)

  dns_widget w;
  int *highlighted;
  unsigned int *entry_tags;
  int num_array_entries;

{
/*
**  Local data
*/
    EntryPtr entry;
    int i, entry_number, number_to_copy;


/*
**  Figure out the number of entries to copy based on the length of the array
**  and the number of entries that are highlighted.
*/
    if (w->browse.num_highlighted > num_array_entries)
         number_to_copy = num_array_entries;
    else number_to_copy = w->browse.num_highlighted;


/*
**  Get the first entry, so the next entry interface can be used.
*/
    entry_number = 1;
    entry = w->browse.entryPtr;


/*
**  Loop thru the entries, setting the arrays if the entry is highlighted
*/
    for (i = 0;  i < number_to_copy;  i++)
        {
	  while (!entry->highlighted)
             {
               entry = entry->next;
               entry_number++;
             };

	  highlighted[i] = entry_number;

	  entry = entry->next;
	  entry_number++;
	};


/*
**  Set any remaining array elements to null
*/
    for (i = number_to_copy + 1;  i < num_array_entries;  i++)
        {
          highlighted[i] = 0;
          if (entry_tags != (unsigned int *) NULL) entry_tags[i] = 0;
        };
}

/*
** Here is the ClearSelection public entry point.
**
** This routine can assume that the widget is currently disabled to avoid any
** conflicts with user actions.
*/

void ClearSelection (w, entry_number)

  dns_widget w;
  int entry_number;

{
/*
**  Local data
*/
    EntryPtr entry;


/*
**  Get the entry pointer
*/
    entry = StructGetEntryPtr (w, entry_number);


/*
**  If it's selected, then unselect it...
*/
    if (entry->selected) LclUnselectEntry(w, entry, entry_number);
}

/*
** Here is the ClearHighlight public entry point.
**
** This routine can assume that the widget is currently disabled to avoid any
** conflicts with user actions.
*/

void ClearHighlight (w, entry_number)

  dns_widget w;
  int entry_number;

{
/*
**  Local data
*/
    EntryPtr entry;


/*
**  Get the entry pointer
*/
    entry = StructGetEntryPtr (w, entry_number);


/*
**  If it's highlighted, then unhighlight it...
*/
    if (entry->highlighted) LclUnhighlightEntry(w, entry,
	entry_number);
}

/*
** Here is the ClearSelections public entry point.
**
** This routine can assume that the widget is currently disabled to avoid any
** conflicts with user actions.
*/

void ClearSelections (w)

  dns_widget w;

{
/*
**  Call the local routine to clear all of the selections
*/
    SelectClearSelections(w, 1, w->browse.num_entries);
}

/*
** Here is the ClearHighlighting public entry point.
**
** This routine can assume that the widget is currently disabled to avoid any
** conflicts with user actions.
*/

void ClearHighlighting (w)

  dns_widget w;

{
/*
**  Call the local routine to clear all of the highlighting
*/
    SelectClearHighlighting(w, 1, w->browse.num_entries);
}

/*
** Here is the SelectAll public entry point.
**
** This routine can assume that the widget is currently disabled to avoid any
** conflicts with user actions.
*/

void SelectAll (w)

  dns_widget w;

{
/*
**  Local data
*/
    EntryPtr entry;
    int entry_number;
    Boolean first = first_of_many;


/*
**  Loop through all entries until the end of the list is encountered.  For
**  each entry that is currently not selected, call the local routine that
**  transitions an entry from the unselected to selected state.  We do this
**  because there is alot of activity generated during this transition like
**  informing the display manager and optionally informing the application.
*/
    entry_number = 1;
    entry = w->browse.entryPtr;

    while (entry != (EntryPtr) NULL)
	{
          if (!LclEntrySelected (w, entry, entry_number, 0))
             {
               LclSelectEntry (w, entry, entry_number, 0, CurrentTime, first);
               first = not_first;
             };
 	  entry = entry->next;
	  entry_number++;
	};
}

/*
** Here is the HighlightAll public entry point.
**
** This routine can assume that the widget is currently disabled to avoid any
** conflicts with user actions.
*/

void HighlightAll (w)

  dns_widget w;

{
/*
**  Local data
*/
    EntryPtr entry;
    int entry_number;


/*
**  Loop through all entries until the end of the list is encountered.  For
**  each entry that is currently not highlighted, call the local routine that
**  transitions an entry from the unhighlighted to highlighted state.  We do
**  this because there is alot of activity generated during this transition
**  like informing the display manager and optionally informing the
**  application.
*/
    entry_number = 1;
    entry = w->browse.entryPtr;

    while (entry != (EntryPtr) NULL)
	{
          if (!entry->highlighted) LclHighlightEntry (w, entry, entry_number);
 	  entry = entry->next;
	  entry_number++;
	};
}

/*
** Here is the SelectComponent public entry point.
**
** This routine can assume that the widget is currently disabled to avoid any
** conflicts with user actions.
*/

void SelectComponent (w, entry_number, comp_number)

  dns_widget w;
  int entry_number, comp_number;

{
/*
**  Local data
*/
    EntryPtr entry;


/*
**  Get the entry pointer
*/
    entry = StructGetEntryPtr (w, entry_number);


/*
**  Select it if necessary
*/
    if (!LclEntrySelected (w, entry, entry_number, comp_number))
       LclSelectEntry(w, entry, entry_number, comp_number, CurrentTime, first_of_one);
}

/*
** Here is the SelectEntry public entry point.
**
** This routine can assume that the widget is currently disabled to avoid any
** conflicts with user actions.
*/

void SelectEntry (w, entry_number)

  dns_widget w;
  int entry_number;

{
/*
**  Local data
*/
    EntryPtr entry;


/*
**  Get the entry pointer
*/
    entry = StructGetEntryPtr (w, entry_number);


/*
**  Select it if necessary
*/
    if (!LclEntrySelected (w, entry, entry_number, 0))
       LclSelectEntry(w, entry, entry_number, 0, CurrentTime, first_of_one);
}

/*
** Here is the HighlightEntry public entry point.
**
** This routine can assume that the widget is currently disabled to avoid any
** conflicts with user actions.
*/

void HighlightEntry (w, entry_number)

  dns_widget w;
  int entry_number;

{
/*
**  Local data
*/
    EntryPtr entry;


/*
**  Get the entry pointer
*/
    entry = StructGetEntryPtr (w, entry_number);


/*
**  Highlight it if necessary
*/
    if (!entry->highlighted)
       LclHighlightEntry(w, entry, entry_number);
}

/*
** Here is the HideSelections public entry point.
*/

void HideSelections (w)

  dns_widget w;
{
/*
**  If we already lost them before or we don't own them, then ignore this call
*/
    if (!w->browse.show_selections) return;


/*
**  Show that we own the global selection
*/
    w->browse.show_selections = FALSE;


/*
**  Actions needing taken TBS
*/
    LclRedrawSelected (w);
}

/*
** Here is the HideHighlighting public entry point.
*/

void HideHighlighting (w)

  dns_widget w;
{
/*
**  If we already lost them before or we don't own them, then ignore this call
*/
    if (!w->browse.show_highlighting) return;


/*
**  Show that highlighting is off
*/
    w->browse.show_highlighting = FALSE;


/*
**  Actions needing taken TBS
*/
    LclRedrawHighlighted (w);
}

/*
** Here is the ShowSelections public entry point.
*/

void ShowSelections (w)

  dns_widget w;
{
/*
**  If we already own them before, then ignore this request.
*/
    if (w->browse.show_selections) return;


/*
**  Show that we own the global selection
*/
    w->browse.show_selections = TRUE;


/*
**  Actions needing taken TBS
*/
    LclRedrawSelected (w);
}

/*
** Here is the ShowHighlighting public entry point.  Refuse to do this
** if expect_selections is not set.
*/

void ShowHighlighting (w)

  dns_widget w;
{
/*
**  If we already own them before, then ignore this request.
*/
    if (w->browse.show_highlighting) return;


/*
**  If they did not set the NexpectHighlighting resource to true, then ignore
**  this request...
*/
    if (w->browse.expect_highlighting == FALSE) return;


/*
**  Show that highlighting is on
*/
    w->browse.show_highlighting = TRUE;


/*
**  Actions needing taken TBS
*/
    LclRedrawHighlighted (w);
}

/*
**  Local routine that redraws all of the selected entries that is called from
**  hide and show.
*/

static void LclRedrawSelected (w)

  dns_widget w;

{
/*
**  Local data declarations
*/
    EntryPtr entry;
    int i, entry_number;


/*
**  If there are no selections, then we are done
*/
    if (w->browse.num_selections == 0) return;


/*
**  Get the first entry, so the next entry interface can be used.
*/
    entry_number = 1;
    entry = w->browse.entryPtr;


/*
**  Loop thru the entries, calling the hightlight procedure if selected
*/
    for (i = 1;  i <= w->browse.num_selections;  i++)
        {
	  while (!entry->selected)
             {
               entry = entry->next;
               entry_number++;
             };

          DisplayHighlightEntry (w, entry_number);

	  entry = entry->next;
	  entry_number++;
	};
}

/*
**  Local routine that redraws all of the highlighted entries that is called
**  from hide and show.
*/

static void LclRedrawHighlighted (w)

  dns_widget w;

{
/*
**  Local data declarations
*/
    EntryPtr entry;
    int i, entry_number;


/*
**  If there are no highlighted entries, then we are done
*/
    if (w->browse.num_highlighted == 0) return;


/*
**  Get the first entry, so the next entry interface can be used.
*/
    entry_number = 1;
    entry = w->browse.entryPtr;


/*
**  Loop thru the entries, setting the arrays if the entry is highlighted
*/
    for (i = 1;  i <= w->browse.num_highlighted;  i++)
        {
	  while (!entry->highlighted)
             {
               entry = entry->next;
               entry_number++;
             };

          DisplayHighlightEntry (w, entry_number);

	  entry = entry->next;
	  entry_number++;
	};
}

/*
** Here is the low level widget private routine to toggle selections.  This
** routine cannot assume that the widget is disabled.  Note that since this
** routine is not invoked from outside of the widget, the widget parameter
** passed need not be cast into an  structure.
*/

void SelectToggleSelections (w, first, last, compnm, time)

  dns_widget w;
  int first, last, compnm;
  Time time;

{
/*
**  Local data
*/
    EntryPtr entry;
    int i, select_them_all = FALSE;
    Boolean first_select = first_of_many;


/*
**  Loop from first to last seeing if a single entry is selected.  If they are
**  all selected, then we will unselect all of them.  If there is one or more
**  not selected, then we will select all of them.
*/
    for (i = first;  i <= last;  i++)
        {
          entry = StructGetEntryPtr (w, i);
          if (!LclEntrySelected (w, entry, i, compnm))
             {
               select_them_all = TRUE;
               break;
             };
        };


/*
**  Either select them all or clear them all.
*/
    for (i = first;  i <= last;  i++)
        {
          entry = StructGetEntryPtr (w, i);

          if ((select_them_all) && (!LclEntrySelected (w, entry, i, compnm)))
             {
               LclSelectEntry (w, entry, i, compnm, time, first_select);
               first_select = not_first;
             };
          if ((!select_them_all) && (entry->selected))
             LclUnselectEntry (w, entry, i);
        };
}

/*
**  Here is the low level widget private routine to select a range of entries and
**  unselect all entries outside of the range.
*/

void SelectSelectSet (w, low_entry_number, high_entry_number, compnm, time)

  dns_widget w;
  int low_entry_number, high_entry_number, compnm;
  Time time;

{
/*
**  Local data
*/
    EntryPtr lowentry, highentry;
    int low_number, high_number, desired_number, num_grays_found = 0;
    Boolean first = first_of_many;


/*
**  If this is one, then first_of_one
*/
    if (low_entry_number == high_entry_number)
       first = first_of_one;


/*
**  Get the first entry pointer through the cache
*/
    lowentry  = StructGetEntryPtr (w, low_entry_number);
    highentry = lowentry;


/*
**  Loop from the first entry to the last entry.  We will exit this loop with the
**  highentry pointer pointing to the high_entry_number + 1 entry.
*/
    for ( high_number = low_entry_number;  high_number <= high_entry_number;  high_number++)

        {
          if (highentry->grayed) num_grays_found++;

          if ((!LclEntrySelected (w, highentry, high_number, compnm)) && (!highentry->grayed))
             {
               LclSelectEntry (w, highentry, high_number, compnm, time, first);
               first = not_first;
             };
          highentry = highentry->next;
        };


/*
**  Compute the desired number of selections
*/
    desired_number = high_entry_number - low_entry_number + 1;


/*
**  Adjust for the number of gray entries found
*/
    desired_number = desired_number - num_grays_found;


/*
**  If the proper number of entries are now selected, then leave.
*/
    if (w->browse.num_selections == desired_number)
       return;


/*
**  Set up everything for the unselection loop.
*/
    low_number = low_entry_number;


/*
**  Do the following loop until the desired number of selections is hit...
*/
    while (TRUE)

      {
        /*
        **  Test previous entry
        */
            if (low_number != 1)
               {
                 /*
                 **  Get to the entry...
                 */
                     lowentry = lowentry->prev;
                     low_number--;


                 /*
                 **  Unselect it if it is selected
                 */
                     if (lowentry->selected)
                        {
                          LclUnselectEntry (w, lowentry, low_number);
                          if (w->browse.num_selections == desired_number) return;
                        };
               };


        /*
        **  Test next entry
        */
            if (high_number <= w->browse.num_entries)
               {
                 /*
                 **  Unselect it if it is selected
                 */
                     if (highentry->selected)
                        {
                          LclUnselectEntry (w, highentry, high_number);
                          if (w->browse.num_selections == desired_number) return;
                        };


                 /*
                 **  Get to the entry...
                 */
                     highentry = highentry->next;
                     high_number++;

               };
      };
}

/*
**  Here is the low level widget private routine to clear selections.  This
**  routine cannot assume that the widget is disabled.  Note that since this
**  routine is not invoked from outside of the widget, the widget parameter
**  passed need not be cast into an  structure.
*/

void SelectClearSelections (w, first, last)

  dns_widget w;
  int first, last;

{
/*
**  Local data declarations
*/
    EntryPtr entry;
    int i;


/*
**  If there are no selections, get out fast.
*/
    if (w->browse.num_selections == 0) return;


/*
**  Get the first entry pointer
*/
    entry = StructGetEntryPtr (w, first);


/*
**  Loop through all entries from first to last on the list.  For
**  each entry that is currently selected, call the local routine that
**  transitions an entry from the selected to ed state.  We do this
**  because there is alot of activity generated during this transition like
**  informing the display manager and optionally informing the application.
*/
    for (i = first;  i <= last;  i++)
	{
	  if (entry->selected)
             {
               LclUnselectEntry(w, entry, i);
               if (w->browse.num_selections == 0) return;
             };
 	  entry = entry->next;
	};
}

/*
** Here is the local routine that actually selects the entry.  A precondition
** is that the entry is currently unselected. The routine will notify both the
** application and the display module.  The selected bit will be set and the
** number of selected entries will be incremented.
*/

static void LclSelectEntry (w, entry, entnm, compnm, time, first)

  dns_widget w;
  EntryPtr entry;
  int entnm, compnm;
  Time time;
  int first;

{
/*
**  Local data declarations
*/
    CallbackStruct temp;


/*
**  Ignore call if entry is insensitive
*/
    if (entry->grayed) return;


/*
**  Set the values
*/
    entry->selected = TRUE;
    entry->selected_comp = compnm;
    w->browse.num_selections++;


/*
**  Notify the  display module
*/
    DisplayHighlightEntry (w, entnm);


/*
**  Notify the application
*/
    temp.reason = CREntrySelected;
    temp.entry_number = entnm;
    temp.component_number = compnm;
    temp.entry_tag = entry->entry_tag;
    temp.time = time;
    temp.first_selection = first;
    temp.entry_level = entry->level;


/*
**  Issue the callback
*/
    XtCallCallbacks (w, NentrySelectedCallback, &temp);


/*
**  Show that they may need reported
*/
    w->browse.transitions_made = TRUE;
}

/*
** Here is the local routine that actually unselects the entry.  A precondition
** is that the entry is currently selected. The routine will notify both the
** application and the display module.  The selected bit will be off and the
** number of selected entries will be decremented.
*/

static void LclUnselectEntry (w, entry, entnm)

  dns_widget w;
  EntryPtr entry;
  int entnm;

{
/*
**  Local data declarations
*/
    CallbackStruct temp;


/*
**  Unselect this entry
*/
    entry->selected = FALSE;
    entry->selected_comp = 0;
    w->browse.num_selections--;


/*
**  Notify the  display module
*/
    DisplayHighlightEntry (w, entnm);


/*
**  Notify the application
*/
    temp.reason = CREntryUnselected;
    temp.entry_number = entnm;
    temp.entry_tag = entry->entry_tag;
    temp.entry_level = entry->level;


/*
**  Issue the callback
*/
    XtCallCallbacks (w, NentryUnselectedCallback, &temp);


/*
**  Show that they may need reported
*/
    w->browse.transitions_made = TRUE;
}

/*
**  Here is the low level widget private routine to clear highlighting.  This
**  routine cannot assume that the widget is disabled.  Note that since this
**  routine is not invoked from outside of the widget, the widget parameter
**  passed need not be cast into an  structure.
*/

void SelectClearHighlighting (w, first, last)

  dns_widget w;
  int first, last;

{
/*
**  Local data declarations
*/
    EntryPtr entry;
    int i;


/*
**  If there are no highlighted entries, get out fast.
*/
    if (w->browse.num_highlighted == 0) return;


/*
**  Get the first entry pointer
*/
    entry = StructGetEntryPtr (w, first);


/*
**  Loop through all entries from first to last on the list.  For
**  each entry that is currently highlighted, call the local routine that
**  transitions an entry from the highlighted to ed state.  We do this
**  because there is alot of activity generated during this transition like
**  informing the display manager and optionally informing the application.
*/
    for (i = first;  i <= last;  i++)
	{
	  if (entry->highlighted)
             {
               LclUnhighlightEntry(w, entry, i);
               if (w->browse.num_highlighted == 0) return;
             };
 	  entry = entry->next;
	};
}

/*
**  This routine is called to conditionally report the end of a set of
**  select/unselect transitions...
*/

void SelectReportTransitions (w)

  dns_widget w;

{
/*
**  Local data declarations
*/
    CallbackStruct temp;


/*
**  If the flag is false, then simply return...
*/
    if (!w->browse.transitions_made) return;


/*
**  Show that they have been reported
*/
    w->browse.transitions_made = FALSE;


/*
**  Issue the callback
*/
    temp.reason = CRTransitionsDone;
    XtCallCallbacks (w, NtransitionsDoneCallback, &temp);
}

/*
** Here is the local routine that actually highlights the entry.  A
** precondition is that the entry is currently unhighlighted. The routine will
** notify both the application and the display module.  The highlighted bit
** will be set and the number of highlighted entries will be incremented.
*/

static void LclHighlightEntry (w, entry, entnm)

  dns_widget w;
  EntryPtr entry;
  int entnm;

{
/*
**  Set the values
*/
    entry->highlighted = TRUE;
    w->browse.num_highlighted++;


/*
**  Notify the  display module
*/
    DisplayHighlightEntry (w, entnm);
}

/*
** Here is the local routine that actually unhighlights the entry.  A
** precondition is that the entry is currently highlighted. The routine will
** notify both the application and the display module.  The highlighted bit
** will be off and the number of highlighted entries will be decremented.
*/

static void LclUnhighlightEntry (w, entry, entnm)

  dns_widget w;
  EntryPtr entry;
  int entnm;

{
/*
**  Unhighlight this entry
*/
    entry->highlighted = FALSE;
    w->browse.num_highlighted--;


/*
**  Notify the  display module
*/
    DisplayHighlightEntry (w, entnm);
}

/*
**  This procedure determines if an entry is selected according to the
**  selection modes.  This only matters in columns display mode since the user
**  can select a particular component on the right hand side.  All of this work
**  is so a user can select component 'n' and then select component 'n+1'.  The
**  callers of this procedure want to know if a particular entry is selected or
**  not.
**
**  This routine will automatically unselect entries that are necessary.
*/

Boolean LclEntrySelected (w, entry, entry_number, comp_number)

  dns_widget w;
  EntryPtr entry;
  int entry_number, comp_number;

{
/*
**  If the entry is not selected, then return FALSE right away.
*/
    if (!entry->selected) return FALSE;


/*
**  If we are not in column mode, then return TRUE right away.
*/
    if (w->browse.display_mode != KdisplayColumns) return TRUE;


/*
**  If they are on the same component, then return TRUE.
*/
    if (comp_number == entry->selected_comp) return TRUE;


/*
**  If we are in KselectEntry mode, then reverse video does not
**  change.  Return TRUE right away.
*/
    if (w->browse.selection_mode == KselectEntry) return TRUE;


/*
**  If the new component number is on the left and the selected component
**  is also on the left, then return TRUE (that the entry is selected).
*/
    if ((comp_number < w->browse.start_column_component) && (entry->selected_comp < w->browse.start_column_component))
       return TRUE;


/*
**  We now know that at least one of the two components is on the right
**  and that the components are different.
*/
    LclUnselectEntry (w, entry, entry_number);
    return FALSE;
}
