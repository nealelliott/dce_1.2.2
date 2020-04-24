/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cds_structure.c,v $
 * Revision 1.1.4.2  1996/02/18  19:25:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:08:42  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:03:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:37:11  root]
 * 
 * Revision 1.1.2.2  1992/12/30  12:39:06  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:23:02  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:23:35  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#if !defined(lint) && !defined(SABER)
static char *SccsID = "@(#)cds_structure.c\t7\t(DECdns)\t9/24/1991";
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

#define  CDS_STRUCTURE

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
** Local Routines
*/

void        LclLinkAddEntry         ();
EntryPtr    LclLinkDeleteEntry      ();
static void	   LclDeallocateComponent  ();
static void        LclDetermineValidity    ();
static void        LclAdjustRightJustified ();
static void        LclDetermineMaxWidth    ();
static EntryPtr    LclStructGetCachePtr    ();

/*
** Here is the SetEntry public entry point using the C language interface.
**
** This routine has a precondition that the entry being set is currently
** INVALID.  The source module can force this via the InvalidateEntry
** high level routine.
*/

void SetEntry (w, entry_number, width, height, num_components, sensitivity, entry_tag, index_window)

  dns_widget w;
  int entry_number, width, height, num_components;
  Boolean sensitivity;
  unsigned int entry_tag;
  Boolean index_window;

{
/*
**  Local data declarations
*/
    EntryPtr entry;


/*
**  Get the entry structure that was allocated during the AddEntries call.
*/
    entry = StructGetEntryPtr (w, entry_number);


/*
**  Set the information in the entry that can only be set in this call.
*/
    entry->width       = width;
    entry->orig_height = height;


/*
**  Set the other information
*/
    entry->entry_tag    = entry_tag;
    entry->index_window = index_window;


/*
**  If index_window is true, then set index_window_needed
*/
    if (index_window) w->browse.index_window_needed = TRUE;


/*
**  Sensitivity is the opposite of grayed.
*/
    if (sensitivity)
         entry->grayed = FALSE;
    else entry->grayed = TRUE;


/*
**  Call the specialized procedure to set the number of components in the entry.
*/
    SetEntryNumComponents (w, entry_number, num_components);


/*
**  Call the local routine which will determine whether the correct number of
**  components for the entry has been defined.
*/
    LclDetermineValidity (entry);
}

/*
**  Here is the SetEntrySensitivity public entry point using the C
**  language interface.  The sensitivity is the opposite Boolean value of
**  the grayed one...
*/

void SetEntrySensitivity (w, entry_number, sensitivity)

  dns_widget w;
  int entry_number;
  Boolean  sensitivity;

{
/*
**  Routine data declarations
*/
    EntryPtr entry;


/*
**  Get the entry record from the struct module.  Use the get valid one to be
**  sure that the information is accurate with the source module.
*/
    entry = StructGetEntryPtr (w, entry_number);


/*
**  If the new value is the same as the old, then change the bit and tell
**  display about it.
*/
    if (entry->grayed == sensitivity)
       {
        if (entry->grayed) entry->grayed = FALSE;
        else entry->grayed = TRUE;

        if ((entry->grayed) && (entry->selected))
           SelectClearSelections (w, entry_number, entry_number);

        DisplaySetEntrySensitivity (w, entry_number);
       }
}

/*
** Here is the SetEntryNumComponents public entry point using the C language interface.
**
** This routine has a precondition that the entry being set is currently
** INVALID.  The source module can force this via the InvalidateEntry
** high level routine.
**
** Note that this procedure will not release component information if the number
** of components has changed.  This is different than previous incarnations of
** this procedure.  Callers may need to call invalidate entry explicitly.
*/

void SetEntryNumComponents (w, entry_number, num_components)

  dns_widget w;
  int entry_number, num_components;

{
/*
**  Local data declarations
*/
    EntryPtr entry;
    int i;


/*
**  Get the entry structure that was allocated during the AddEntries call.
*/
    entry = StructGetEntryPtr (w, entry_number);


/*
**  If components have never been allocated for this entry, then allocate
**  memory for the component array initialized to zeros.  Commit the number
**  of components to avoid invalidating each of the components later.
*/
    if (entry->num_allocated == 0)
       {
         entry->entrycompPtr  = (CompPtr) XtCalloc (num_components, (sizeof(CompStruct)));
         entry->num_allocated = num_components;
       }


/*
**  If there are less components allocated than are currently needed, then
**  extend the component array to hold the new components.  Initialize all of
**  the new component types to KcompNotSet.  This is because the reallocate
**  will not zero out the memory for us.
*/
    if (entry->num_allocated < num_components)
       {
	 entry->entrycompPtr = (CompStruct *) XtRealloc (entry->entrycompPtr, num_components * sizeof(CompStruct));

         for (i = entry->num_allocated;  i <= num_components - 1;  i++)
             entry->entrycompPtr[i].type = KcompNotSet;

         entry->num_allocated = num_components;
       }


/*
**  At this point, we know that the number of components allocated is large
**  enough.
**
**  If we now have more components, then mark the entry as being invalid.
*/
    if (entry->num_components < num_components)
       {
         entry->num_components = num_components;
         entry->valid = FALSE;
       }


/*
**  If the number of components less than before, then we don't have to change
**  the component allocation, but we do need to invalidate the components that
**  are no longer going to be used.
*/
    if (entry->num_components > num_components)
       {
         for (i = num_components;  i <= entry->num_components - 1;  i++)
	     {
	       if (entry->entrycompPtr[i].type == KcompText)
                  XtFree (entry->entrycompPtr[i].var.is_text.text);
	       if (entry->entrycompPtr[i].type == KcompCString)
		  XmStringFree(entry->entrycompPtr[i].var.is_cstring.cstring);

	       entry->entrycompPtr[i].type = KcompNotSet;
	     }

         entry->num_components = num_components;
       }
}

/*
** Here is the SetEntryIndexWindow entry point using the C language interface.
*/

void SetEntryIndexWindow (w, entry_number, index_window)

  dns_widget w;
  int entry_number, index_window;

{
/*
**  Local Data
*/
    EntryPtr entry;


/*
**  Get the entry structure that was allocated during the AddEntries call.
*/
    entry = StructGetEntryPtr (w, entry_number);


/*
**  Set the information in the entry.
*/
    entry->index_window = index_window;


/*
**  If index_window is true, then set index_window_needed
*/
    if (index_window) w->browse.index_window_needed = TRUE;

}

/*
** Here is the SetEntryIndexWindow entry point using the C language interface.
*/

void SetEntryTag (w, entry_number, entry_tag)

  dns_widget w;
  int entry_number;
  unsigned int entry_tag;

{
/*
**  Local data
*/
    EntryPtr entry;


/*
**  Get the entry structure that was allocated during the AddEntries call.
*/
    entry = StructGetEntryPtr (w, entry_number);


/*
**  Set the information in the entry.
*/
    entry->entry_tag = entry_tag;
}

/*
** Here is the SetComponentHidden public entry point using the C language interface.
*/

void SetComponentHidden (w, entry_number, component_number, hidden_mode)

  dns_widget w;
  int entry_number, component_number, hidden_mode;

{
/*
**  Local data
*/
    EntryPtr entry;


/*
**  Get the entry structure
*/
    entry = StructGetEntryPtr (w, entry_number);


/*
**  Set the hidden_mode field
*/
    entry->entrycompPtr[component_number-1].hidden_mode = hidden_mode;
}

/*
** Here is the SetComponentText public entry point using the C language interface.
*/

int SetComponentText (w, entry_number, comp_number, x, y, text, font, justification, max_pixels, max_chars)

  dns_widget w;
  int entry_number, comp_number, x, y;
  char *text;
  XFontStruct *font;
  int justification, max_pixels, max_chars;

{
/*
**  Local data
*/
    EntryPtr entry;


/*
**  Make it a bit more convenient
*/
    int comp_offset = comp_number - 1;


/*
**  Get the entry and component structures
*/
    entry = StructGetEntryPtr (w, entry_number);


/*
**  Free the old text component based on the old component type...  This is
**  needed if changing component types...
*/
    if (entry->entrycompPtr[comp_offset].type == KcompText)
       XtFree(entry->entrycompPtr[comp_offset].var.is_text.text);
    if (entry->entrycompPtr[comp_offset].type == KcompCString)
       XmStringFree(entry->entrycompPtr[comp_offset].var.is_cstring.cstring);


/*
**  Default the string to a space if it is NULL
*/
    if (text == (char *) NULL) text = " ";


/*
**  Set the component information.  The string must be copied to
**  dynamic memory.  We can assume that the delete routine is responsible for
**  returning any managed memory.
*/
    entry->entrycompPtr[comp_offset].var.is_text.text = XtMalloc (strlen(text)+1);
    strcpy (entry->entrycompPtr[comp_offset].var.is_text.text,text);


/*
**  Fill in the common component information
*/
    entry->entrycompPtr[comp_offset].type   = KcompText;
    entry->entrycompPtr[comp_offset].orig_x = x;
    entry->entrycompPtr[comp_offset].orig_y = y;


/*
**  Fill in the information pertaining to a text entry
*/
    entry->entrycompPtr[comp_offset].var.is_text.font = font;


/*
**  Remember whether this text was right justified or not.
*/
    if (justification == XmALIGNMENT_END)
         entry->entrycompPtr[comp_offset].orig_rj = TRUE;
    else entry->entrycompPtr[comp_offset].orig_rj = FALSE;


/*
**  Adjust the X position if this is RJ text, otherwise just store the
**  width if later truncation is necessary
*/
    if (justification == XmALIGNMENT_END)
         LclAdjustRightJustified (w, &entry->entrycompPtr[comp_offset], max_pixels, max_chars, entry->level);
    else LclDetermineMaxWidth    (w, &entry->entrycompPtr[comp_offset], max_pixels, max_chars, entry->level);


/*
**  Call the local routine which will determine whether the correct number of
**  components for the entry has been defined.
*/
    LclDetermineValidity (entry);


/*
**  Return the adjusted X position
*/
    return (entry->entrycompPtr[comp_offset].orig_x);
}

/*
**  Here is the SetComponentCString public entry point using the C language interface.
*/

void SetComponentCString (w, entry_number, component_number, x, y, cs, font)

  dns_widget w;
  int entry_number, component_number, x, y;
  XmString cs;
  XFontStruct *font;

{
#ifdef DECW_V2
/*
**  Local data
*/
    EntryPtr entry;


/*
**  Make it a bit more convenient
*/
    int comp_offset = component_number - 1;


/*
**  Get the entry and component structures
*/
    entry = StructGetEntryPtr (w, entry_number);


/*
**  Free the old text component based on the old component type...  This is
**  needed if changing component types...
*/
    if (entry->entrycompPtr[comp_offset].type == KcompText)
       XtFree(entry->entrycompPtr[comp_offset].var.is_text.text);
    if (entry->entrycompPtr[comp_offset].type == KcompCString)
       XmStringFree(entry->entrycompPtr[comp_offset].var.is_cstring.cstring);


/*
**  Set the component information.  Must make a local copy of the string in
**  dynamic memory.  We can assume that the delete routine is responsible for
**  returning any managed memory.
*/
    entry->entrycompPtr[comp_offset].var.is_cstring.cstring = XmStringCopy(cs);


/*
**  Fill in the common component information
*/
    entry->entrycompPtr[comp_offset].type   = KcompCString;
    entry->entrycompPtr[comp_offset].orig_x = x;
    entry->entrycompPtr[comp_offset].orig_y = y;


/*
**  Fill in the information pertaining to a text entry
*/
    entry->entrycompPtr[comp_offset].var.is_cstring.font_list = (XmFontList)font;

/*
**  Call the local routine which will determine whether the correct number of
**  components for the entry has been defined.
*/
    LclDetermineValidity (entry);

#else
/*
**  Local data
*/
    XmStringContext context;
    char * text;
    int status;
    XmStringCharSet charset;     /* not used */
    XmStringDirection direction; /* not used */
    Boolean separator;		 /* not used */

/*
**  Initialize the compound string context
*/
    status = XmStringInitContext (&context, cs);


/*
**  If that succeeded, then get the first segment into the text variable
*/
    if (status == TRUE) status = XmStringGetNextSegment (context, &text, &charset, &direction, &separator);


/*
**  If the status failed in either case, then set the text pointer to a NULL
*/
    if (status != TRUE) text = (char *) NULL;


/*
**  Call the SetComponentText procedure...
*/
    SetComponentText (w, entry_number, component_number, x, y, text, font, XmALIGNMENT_BEGINNING, 0, 0);
#endif
}

/*
** Here is the GetComponentText public entry point using the C language
*/

char * GetComponentText (w, entry_number, comp_number)

  dns_widget w;
  int entry_number, comp_number;

{
/*
**  Local data
*/
    EntryPtr entry;
    char *ptr;


/*
**  Make accessing easier
*/
    int comp_offset = comp_number - 1;


/*
**  Get the entry and component structures
*/
    entry = StructGetEntryPtr (w, entry_number);


/*
**  If this component number is too big, return NULL
*/
    if (entry->num_components < comp_number)
       return ((char *) NULL);


/*
**  If this component is not text, then return a NULL pointer
*/
    if (entry->entrycompPtr[comp_offset].type != KcompText)
       return ((char *) NULL);


/*
**  Allocate new memory
*/
    ptr = XtMalloc (strlen(entry->entrycompPtr[comp_offset].var.is_text.text)+1);


/*
**  Copy the text into new memory, but only if we got it...
*/
    if (ptr != (char *) NULL)
       strcpy (ptr, entry->entrycompPtr[comp_offset].var.is_text.text);


/*
**  Return this pointer
*/
    return ptr;
}

/*
** Here is the SetComponentPixmap public entry point using C
*/

void SetComponentPixmap (w, entry_number, comp_number, x, y, pixmap, width, height)

  dns_widget w;
  int entry_number, comp_number, x, y;
  Pixmap pixmap;
  int width, height;

{
/*
**  Local data
*/
    EntryPtr entry;


/*
**  Make it a bit more convenient
*/
    int comp_offset = comp_number - 1;


/*
**  Get the entry structure
*/
    entry = StructGetEntryPtr (w, entry_number);


/*
**  Free the old text component based on the old component type...  This is
**  needed if changing component types...
*/
    if (entry->entrycompPtr[comp_offset].type == KcompText)
       XtFree(entry->entrycompPtr[comp_offset].var.is_text.text);
    if (entry->entrycompPtr[comp_offset].type == KcompCString)
       XmStringFree(entry->entrycompPtr[comp_offset].var.is_cstring.cstring);


/*
**  Set the common component information.
*/
    entry->entrycompPtr[comp_offset].type        = KcompPixmap;
    entry->entrycompPtr[comp_offset].orig_x      = x;
    entry->entrycompPtr[comp_offset].orig_y      = y;
    entry->entrycompPtr[comp_offset].width       = width;
    entry->entrycompPtr[comp_offset].height      = height;
    entry->entrycompPtr[comp_offset].orig_width  = width;
    entry->entrycompPtr[comp_offset].orig_height = height;


/*
**  Set the pixmap specific component information
*/
    entry->entrycompPtr[comp_offset].var.is_pixmap.pixmap = pixmap;


/*
**  Call the local routine which will determine whether the correct number of
**  components for the entry has been defined.
*/
    LclDetermineValidity (entry);
}

/*
** Here is the SetComponentWidget public entry point using C
*/

void SetComponentWidget (w, entry_number, comp_number, x, y, subw)

  dns_widget w;
  int entry_number, comp_number, x, y;
  Widget subw;

{
/*
**  Local data
*/
    EntryPtr entry;
    Arg arguments[2];


/*
**  Make it a bit more convenient
*/
    int comp_offset = comp_number - 1;


/*
**  Mark the fact that this call had been made
*/
    w->browse.sub_widgets_used = TRUE;


/*
**  Get the entry structure
*/
    entry = StructGetEntryPtr (w, entry_number);


/*
**  Free the old text component based on the old component type...  This is
**  needed if changing component types...
*/
    if (entry->entrycompPtr[comp_offset].type == KcompText)
       XtFree(entry->entrycompPtr[comp_offset].var.is_text.text);
    if (entry->entrycompPtr[comp_offset].type == KcompCString)
       XmStringFree(entry->entrycompPtr[comp_offset].var.is_cstring.cstring);


/*
**  Set the component information.
*/
    entry->entrycompPtr[comp_offset].type   = KcompWidget;
    entry->entrycompPtr[comp_offset].orig_x = x;
    entry->entrycompPtr[comp_offset].orig_y = y;


/*
**  Set the widget specific component information
*/
    entry->entrycompPtr[comp_offset].var.is_widget.readwrite_text = subw;


/*
**  Manage the subwidget and position it out of site...
*/
    XtSetArg (arguments[0], XmNx, 0);
    XtSetArg (arguments[1], XmNy, 2 * XtHeight(w));
    XtSetValues (subw, arguments, 2);
    XtManageChild (subw);


/*
**  Call the local routine which will determine whether the correct number of
**  components for the entry has been defined.
*/
    LclDetermineValidity(entry);
}

/*
** Here is the GetEntryTag public entry point using the C language
*/

unsigned int GetEntryTag (w, entry_number)

  dns_widget w;
  int entry_number;

{
/*
**  Local data
*/
    EntryPtr entry;


/*
**  If the entry number is out of bounds, return 0
*/
    if ((entry_number <= 0) || (entry_number > w->browse.num_entries))
       return (0);


/*
**  Get the entry structure
*/
    entry = StructGetEntryPtr (w, entry_number);


/*
**  Return the tag field
*/
    return (entry->entry_tag);
}

/*
** Here is the GetEntryNumber public entry point using the C language
*/

unsigned int GetEntryNumber (w, tag)

  dns_widget w;
  unsigned int tag;

{
/*
**  Local data
*/
    EntryPtr entry;
    int entry_number;


/*
**  Get the first entry structure
*/
    entry = w->browse.entryPtr;
    entry_number = 1;


/*
**  Spin through the following loop until the entry is found or the end of
**  the list is hit.
*/
    while (entry != (EntryPtr) NULL)
       {
         if (entry->entry_tag == tag) return entry_number;
         entry_number++;
         entry = entry->next;
       }


/*
**  No hit...
*/
    return 0;
}

/*
** Here is the ValidateAll public entry point using the C language
*/

void ValidateAll (w)

  dns_widget w;

{
/*
**  Local data
*/
    int entry_number = 1;


/*
**  Leave if there are no entries
*/
    if (w->browse.num_entries == 0) return;


/*
**  Put out the watch cursor
*/
    DisplaySetWatch (w, TRUE);


/*
**  Spin in a loop getting a valid entry pointer for each entry.  We will
**  leave the loop when the number of entries is equal to
**  the entry that we just validated.
*/
    while (TRUE)

       {
         StructGetValidEntryPtr (w, entry_number);
         if (entry_number == w->browse.num_entries) break;
         entry_number++;
       }


/*
**  Cancel the watch cursor
*/
    DisplaySetWatch (w, FALSE);
}

/*
**  Here is the private StructOpenArray routine.  Given the address of an
**  array, the size of an array entry, and the maximum length of the array,
**  it will open up a hole in the array at the provided offset.
**
**  If the array is actually full, then the last entry information will be
**  lost when the hole is opened.
**
**  Example:  Array of 5 integers.  Want to open a hole at offset 2.
**
**       +-------+    +-------+
**       |   A   |    |   A   |    The array_ptr is the address of the array
**       +-------+    +-------+
**       |   B   |    |   B   |    The size_per is 4 for ints
**       +-------+    +-------+
**       |   C   |    |   0   |    The array_len is 5
**       +-------+    +-------+
**       |   D   |    |   C   |    The open_offset is 2
**       +-------+    +-------+
**       |   E   |    |   D   |    Data E is lost after the open
**       +-------+    +-------+
*/

void StructOpenArray (array_ptr, size_per, array_len, open_offset)

  char * array_ptr;
  int size_per, array_len, open_offset;

{
/*
**  Local data declarations
*/
    char * open_address;
    int    copy_length;


/*
**  The open_address will be used as the start location of the copy and
**  as the start address for the clear operation.
*/
    open_address = &array_ptr [size_per * open_offset];


/*
**  The copy length is the length of the array minus the offset minus one
**  times the size of an entry.  Note that if the array is 4 units long
**  and the open is taking place at the last entry (offset 3), then the
**  copy length is 4 - 3 - 1 which is zero.
*/
    copy_length = size_per * (array_len - open_offset - 1);


/*
**  Copy the bytes if there are any to copy
*/
    if (copy_length > 0) memcpy (open_address + size_per, open_address, copy_length);


/*
**  Set the hole to all zeros.
*/
    memset (open_address, 0, size_per);
}

/*
**  Here is the private StructCloseArray routine.  Given the address of an
**  array, the size of an array entry, and the used length of the array, it
**  will close a hole in the array at the provided offset.
**
**  Example:  Array of 5 integers.  Want to close offset 2.
**
**       +-------+    +-------+
**       |   A   |    |   A   |    The array_ptr is the address of the array
**       +-------+    +-------+
**       |   B   |    |   B   |    The size_per is 4 for ints
**       +-------+    +-------+
**       |   C   |    |   D   |    The array_len is 5
**       +-------+    +-------+
**       |   D   |    |   E   |    The close_offset is 2
**       +-------+    +-------+
**       |   E   |    |   0   |    Data C is lost after the open
**       +-------+    +-------+
**                                 The end of the array is cleared
*/

void StructCloseArray (array_ptr, size_per, array_len, close_offset)

  char * array_ptr;
  int size_per, array_len, close_offset;

{
/*
**  Local data declarations
*/
    char * close_address;
    int copy_length;


/*
**  The close_address will be used as the destination of the copy.
*/
    close_address = &array_ptr [size_per * close_offset];


/*
**  The copy length is the length of the array minus the offset minus one
**  times the size of an entry.  Note that if the array is 4 units long
**  and the close is taking place at the last entry (offset 3), then the
**  copy length is 4 - 3 - 1 which is zero.
*/
    copy_length = size_per * (array_len - close_offset - 1);


/*
**  Copy the bytes.
*/
    if (copy_length > 0) memcpy (close_address, close_address + size_per, copy_length);


/*
**  Clear the last entry in the array
*/
    memset (&array_ptr [size_per * (array_len - 1)], 0, size_per);
}

/*
**  This routine inserts a new component into each entry which already has a
**  component structure defined.  Due to the number of entries that may be
**  affected, this procedure must be as tight as possible.
**
**  Since each entry points to the next entry, we will not even concern
**  ourselves with which entry we are on.  We must keep count in case we
**  have to call invalidate entry routine.
*/

void InsertComponent (w, comp_number, width, tag)

  dns_widget w;
  int comp_number;
  Dimension width;
  unsigned int tag;

{
/*
**  Local data declarations
*/
    EntryPtr entry;
    int entry_number;


/*
**  Make it a bit more convenient
*/
    int comp_offset = comp_number - 1;


/*
**  Begin by having the first entry be in the entry variable.
*/
    entry = w->browse.entryPtr;
    entry_number = 1;


/*
**  While the entry is not null, then do that entry if the component structure
**  is already allocated.
*/
    while (entry != (EntryPtr) NULL)

      {
       /*
       **  If we have a component structure, then it may need expanded.
       */
           if (entry->num_allocated != 0)

              {
               /*
               **  Make sure that this newly inserted entry does not force holes in
               **  the entries.
               */
		   if (entry->num_allocated < comp_offset)
                      XtError(INSERTHOLE);


	       /*
               **  If the entry is valid, then invalidate it with the high level call.
               */
                   if (entry->valid)
                      InvalidateEntry (w, entry_number);


               /*
               **  Extend the memory holding the components if necessary.  We
	       **  know that that there are at least num_components entries
	       **  already.  If we need more space, we will reallocate the
	       **  entrycompPtr space.
               */
                   if (entry->num_allocated == entry->num_components)
		      {
			CompPtr tempcompPtr = (CompPtr) XtCalloc (entry->num_allocated + 1, (sizeof(CompStruct)));
			memcpy (tempcompPtr,entry->entrycompPtr,entry->num_allocated * sizeof(CompStruct));
	                XtFree (entry->entrycompPtr);
			entry->num_allocated++;
	                entry->entrycompPtr = tempcompPtr;
		      }


	       /*
               **  Move all entries out to insert a hole.
               */
                   StructOpenArray (entry->entrycompPtr, sizeof(CompStruct), entry->num_allocated, comp_offset);


	       /*
               **  Commit the new number of components in this entry
               */
	           entry->num_components++;
               }


       /*
       **  Position to the next entry
       */
	   entry = entry->next;
           entry_number++;
      }


/*
**  Push out the entries in the column_widths and column_tags array.
*/
    StructOpenArray (w->browse.column_widths,    sizeof(Dimension),    max_comps, comp_offset);
    StructOpenArray (w->browse.column_tags,      sizeof(unsigned int), max_comps, comp_offset);
    StructOpenArray (w->browse.column_width_set, sizeof(Boolean),      max_comps, comp_offset);


/*
**  Put in the new values.
*/
    w->browse.column_widths [comp_offset] = width;
    w->browse.column_tags   [comp_offset] = tag;


/*
**  Set the boolean if the width was set by application
*/
    if (width == 0)
         w->browse.column_width_set [comp_offset] = FALSE;
    else w->browse.column_width_set [comp_offset] = TRUE;


/*
**  Add the new width plus a spacing into the maximum width (only if the width is
**  not zero.  If it is zero, then adjustments will fix the width.
*/
    if (width != 0)
       w->browse.rhs_max_width = w->browse.rhs_max_width + w->browse.default_spacing + width;
}

/*
**  This routine removes a component from each entry which already has a
**  component structure defined.  Due to the number of entries that may be
**  affected, this procedure must be as tight as possible.
*/

void RemoveComponent (w, comp_number)

  dns_widget w;
  int comp_number;

{
/*
**  Local data declarations
*/
    EntryPtr entry;
    int entry_number;
    int comp_offset = comp_number - 1;


/*
**  Begin by having the first entry be in the entry variable.
*/
    entry = w->browse.entryPtr;
    entry_number = 1;


/*
**  While the entry is not null, then do that entry if the component structure
**  is already allocated.
*/
    while (entry != (EntryPtr) NULL)
      {
       /*
       **  If we have a component structure, then we can squeeze out this
       **  undesired child...
       */
           if (entry->num_allocated != 0)

              {
               /*
               **  Make sure that this entry has that component number.
               */
		   if (entry->num_allocated < comp_number)
                      XtError(REMOVEMISS);

	       /*
               **  If the entry is valid, then invalidate it with the high
	       **  level call.  It could be that this component's removal
	       **  would change the height of the entry.
               */
                   if (entry->valid)
		      DisplayInvalidateEntry (w, entry_number);

	       /*
	       **  Get rid of the component data before we squish it.
	       */
                   if (entry->entrycompPtr[comp_offset].type == KcompText)
                      XtFree (entry->entrycompPtr[comp_offset].var.is_text.text);
		   if (entry->entrycompPtr[comp_offset].type == KcompCString)
		      XmStringFree(entry->entrycompPtr[comp_offset].var.is_cstring.cstring);

	       /*
               **  Move all entries up unless this is the last component.
               */
                   StructCloseArray (entry->entrycompPtr, sizeof(CompStruct), entry->num_allocated, comp_offset);

	       /*
               **  Commit the new number of components in this entry
               */
	           entry->num_components--;
               }


       /*
       **  Position to the next entry
       */
	   entry = entry->next;
           entry_number++;
      }


/*
**  Subtract this column plus spacing from the width if it is not zero...
*/
    if (w->browse.column_widths [comp_offset] != 0)
       {
        w->browse.rhs_max_width = w->browse.rhs_max_width - w->browse.default_spacing;
        w->browse.rhs_max_width = w->browse.rhs_max_width - w->browse.column_widths [comp_offset];
       }

/*
**  Squeeze out the column width and column tag
*/
    StructCloseArray (w->browse.column_widths,    sizeof(Dimension),    max_comps, comp_offset);
    StructCloseArray (w->browse.column_tags,      sizeof(unsigned int), max_comps, comp_offset);
    StructCloseArray (w->browse.column_width_set, sizeof(Boolean),      max_comps, comp_offset);
}

/*
**  This routine sets a new component width.  It must force all entries to be
**  readjusted in height and redrawn on the screen.
*/

void SetComponentWidth (w, comp_number, width)

  dns_widget w;
  int comp_number;
  Dimension width;

{
/*
**  Make it a bit more convenient
*/
    int comp_offset = comp_number - 1;


/*
**  Set the boolean if the width is being set by application
*/
    if (width == 0)
         w->browse.column_width_set [comp_offset] = FALSE;
    else w->browse.column_width_set [comp_offset] = TRUE;


/*
**  If the column width is not changing, then leave early.
*/
    if (w->browse.column_widths [comp_offset] == width) return;


/*
**  Ensure that all entries are redrawn when the widget is reenabled
*/
    XClearArea (XtDisplay(w),
	    XtWindow(w->browse.rhs_window_widget),
	    0, 0,
	    XtWidth(w->browse.rhs_window_widget),
	    XtHeight(w->browse.rhs_window_widget),
	    TRUE);


/*
**  Subtract this column plus spacing from the width if it is not zero...
*/
    if (w->browse.column_widths [comp_offset] != 0)
       {
        w->browse.rhs_max_width = w->browse.rhs_max_width - w->browse.default_spacing;
        w->browse.rhs_max_width = w->browse.rhs_max_width - w->browse.column_widths [comp_offset];
       }


/*
**  Put in the new value.
*/
    w->browse.column_widths [comp_offset] = width;


/*
**  Add the new width plus a spacing into the maximum width (only if the width is
**  not zero.  If it is zero, then adjustments will fix the width.
*/
    if (width != 0)
       w->browse.rhs_max_width = w->browse.rhs_max_width + w->browse.default_spacing + width;
}

/*
**  This routine gets a component width.
*/

Dimension GetComponentWidth (w, comp_number)

  dns_widget w;
  int comp_number;

{
/*
**  Return the value.
*/
    return w->browse.column_widths [comp_number - 1];
}

/*
**  This routine sets a new component tag.
*/

void SetComponentTag (w, comp_number, tag)

  dns_widget w;
  int comp_number;
  unsigned int tag;

{
/*
**  Put in the new value.
*/
    w->browse.column_tags [comp_number - 1] = tag;
}

/*
**  This routine gets a component tag.
*/

unsigned int GetComponentTag (w, comp_number)

  dns_widget w;
  int comp_number;

{
/*
**  Return the value.
*/
    return w->browse.column_tags [comp_number - 1];
}

/*
**  Here is the GetComponentNumber which finds a component number given
**  a component tag value.  Zero means no hit.
*/

int GetComponentNumber (w, comp_tag)

  dns_widget w;
  unsigned int comp_tag;

{
/*
**  Local data
*/
    int comp_number;


/*
**  Go through the list of comp tags.  If a hit is found, return the number.
*/
    for (comp_number = 1;  comp_number <= max_comps;  comp_number++)
	if (w->browse.column_tags [comp_number-1] == comp_tag)
	   return comp_number;


/*
**  No hit...
*/
    return 0;
}

/*
** Here is the private StructAddEntries routine
*/

void StructAddEntries (w, after_entry, number_of_entries, level, entry_tags, index_window)

  dns_widget w;
  int after_entry, number_of_entries, level;
  unsigned int * entry_tags;
  int index_window;

{
/*
**  Local data declarations
*/
    EntryPtr NewEntry, AfterEntry;
    int i;


/*
**  The level arrays must be larger than the deepest level in the tree.  When
**  allocating the actual space, we add one because C arrays are zero-based.
**  The arrays are indexed [0..max_level].
*/
    if (level >= w->browse.max_level) {
	w->browse.max_level = level + 1;
	w->browse.levelx = (LevelPtr) XtRealloc(w->browse.levelx,
					(w->browse.max_level + 1) * sizeof(int));
 	w->browse.levely = (LevelPtr) XtRealloc(w->browse.levely,
					(w->browse.max_level + 1) * sizeof(int));
	}


/*
**  If entries are being added to an empty structure, then initialize the
**  cache to be NULL and zeros.  This alleviates the need to initialize this
**  value from outside of this module.
*/
    if (w->browse.num_entries == 0)
       {
         w->browse.cache_number  = 0;
         w->browse.cache_pointer = (EntryPtr) NULL;
       }


/*
**  Get the entry after which these will be placed.
*/
    if (after_entry == 0)
         AfterEntry = (EntryPtr) NULL;
    else AfterEntry = StructGetEntryPtr (w, after_entry);


/*
**  If entries are being added before the cache entry, then we will back the
**  cache back to that entry.
*/
    if (w->browse.cache_number != 0)
       if (w->browse.cache_number > after_entry)
          {
            w->browse.cache_number  = after_entry;
            w->browse.cache_pointer = AfterEntry;
          }


/*
**  Allocate memory, create the required number of entries
*/
    for (i = 1;  i <= number_of_entries;  i++)
	{
          /*
          **  Get the new entry
          */
              NewEntry = (EntryPtr) XtCalloc (1, (sizeof(EntryStruct)));


          /*
          **  Set the known values
          */
              NewEntry->index_window = index_window;
              NewEntry->level = level;


          /*
          **  Set the optional values
          */
              if (entry_tags != (unsigned int *) NULL) NewEntry->entry_tag = entry_tags [i-1];


          /*
          **  Link it into the list and bump the pointer
          */
              LclLinkAddEntry(w, AfterEntry, NewEntry);
              AfterEntry = NewEntry;
	}


/*
**  If index_window is true, then set index_window_needed
*/
    if (index_window) w->browse.index_window_needed = TRUE;
}

/*
** Here is the private StructDeleteEntries routine
*/

void StructDeleteEntries (w, after_entry, number_of_entries)

  dns_widget w;
  int after_entry, number_of_entries;

{
/*
**  Local data declarations
*/
    EntryPtr CurrentEntry;
    EntryPtr NextEntry;
    int i;


/*
**  Get the first entry that is going away.
*/
    CurrentEntry = StructGetEntryPtr (w, after_entry+1);


/*
**  If the after entry is less than the cache pointer, then back the entry
**  pointer down to that entry...
*/
    if (after_entry < w->browse.cache_number)
         {
           w->browse.cache_number  = after_entry;
           w->browse.cache_pointer = CurrentEntry->prev;
         }


/*
**  Loop through the entries...
*/
    for (i = 1;  i <= number_of_entries;  i++)
        {
          NextEntry = LclLinkDeleteEntry (w, CurrentEntry);
          LclDeallocateComponent (CurrentEntry);
          XtFree (CurrentEntry);
          CurrentEntry = NextEntry;
        }
}

/*
** Here is the private StructInvalidateEntry routine
*/

void StructInvalidateEntry (w, entry_number)

  dns_widget w;
  int entry_number;

{
/*
**  Local data declarations
*/
    EntryPtr entry = StructGetEntryPtr (w, entry_number);


/*
**  Set the invalid bits in the record.
*/
    entry->valid = FALSE;
}

/*
** Here is the private StructGetEntryPtr routine
*/

EntryPtr StructGetEntryPtr (w, entry_number)

  dns_widget w;
  int entry_number;

{
/*
**  Local data declarations
*/
    EntryPtr current_entry = (EntryPtr) NULL;
    int current_number;


/*
**  Return entry 1 very fast...
*/
    if (entry_number == 1) return w->browse.entryPtr;


/*
**  If the desired entry is beyond the cache, then start at the cache...
*/
    if (entry_number > w->browse.cache_number)
       {
         current_number = w->browse.cache_number;
         current_entry  = w->browse.cache_pointer;
       }


/*
**  If the desired entry is closer to the cache than to the front of the list,
**  then also start at the cache...
*/
    if (entry_number < w->browse.cache_number)
       if (entry_number >= (w->browse.cache_number - entry_number))
          {
            current_number = w->browse.cache_number;
            current_entry  = w->browse.cache_pointer;
          }


/*
**  If the current entry pointer is still NULL, then start at the beginning...
*/
    if (current_entry == (EntryPtr) NULL)
       {
         current_number = 1;
         current_entry  = w->browse.entryPtr;
       }


/*
**  See if we should go forward to the desired entry.
*/
    if (entry_number > current_number)
       while (entry_number != current_number)
          {
            current_number++;
            current_entry = current_entry->next;
          }


/*
**  Well, see if we should go backward...
*/
    if (entry_number < current_number)
       while (entry_number != current_number)
          {
            current_number--;
            current_entry = current_entry->prev;
          }


/*
**  Return the current entry pointer...
*/
    return current_entry;
}

/*
**  Here is the local routine that looks for an entry number using the cache...
**
**  This routine deals with the cache being advanced only if the entry that it
**  is currently in the cache is valid...
*/

static EntryPtr LclStructGetCachePtr (w, entry_number)

  dns_widget w;
  int entry_number;

{
/*
**  If the entry number being asked for is less than the cache value, then go
**  to the normal non-cache get entry routine...  This is because we know that
**  if we are not in forced mode, they are all valid...
*/
    if (entry_number < w->browse.cache_number)
       return StructGetEntryPtr (w, entry_number);


/*
**  If the cache value is zero, then set it to the first entry.
*/
    if (w->browse.cache_number == 0)
       {
         w->browse.cache_number  = 1;
         w->browse.cache_pointer = w->browse.entryPtr;
       }


/*
**  See if we should go forward to the desired entry...  We must ensure that
**  the entry in the cache is valid prior to advancing the pointer...  Note
**  only when we are forced into sequential entry mode...
*/
    if (entry_number > w->browse.cache_number)
       while (entry_number != w->browse.cache_number)
          {
           /*
           **  If we are forcing sequential get entry callbacks, then make sure
           **  that the entry is valid.
           */
               if ((w->browse.cache_pointer->valid == FALSE) && (w->browse.force_seq_get_entry))
                  {
                   /*
                   **  Local data declarations
                   */
                       CallbackStruct temp;

                   /*
                   **  Fill in the callback information
                   */
		       temp.reason       = CRGetEntry;
		       temp.entry_number = w->browse.cache_number;
		       temp.entry_tag    = w->browse.cache_pointer->entry_tag;
		       temp.entry_level  = w->browse.cache_pointer->level;

                   /*
                   **  Issue the callback
                   */
	               XtCallCallbacks (w, NgetEntryCallback, &temp);
                  }


           /*
           **  Bump the cache pointer to the next entry...
           */
               w->browse.cache_number++;
               w->browse.cache_pointer = w->browse.cache_pointer->next;
          }


/*
**  Return the entry pointer that is in the cache.
*/
    return w->browse.cache_pointer;
}

/*
** Here is the private StructGetValidEntryPtr routine
*/

EntryPtr StructGetValidEntryPtr (w, entry_number)

  dns_widget w;
  int entry_number;

{
/*
**  Local data declarations
*/
    EntryPtr entry;
    int num_entries;
    CallbackStruct temp;


/*
**  Spin through looking for the entry
*/
    entry = LclStructGetCachePtr (w, entry_number);


/*
**  If the entry is not valid, then we must stay here until we get it.
*/
    if (!entry->valid)

       {
        /*
        **  Disable the widget
        */
            DisableDisplay (w);


        /*
        **  Spin in a loop until the correct conditions are met.
        */
            while (TRUE)

               {
                /*
                **  Record the number of entries currently known
                */
                    num_entries = w->browse.num_entries;


                /*
                **  Fill in the callback information
                */
                    temp.reason       = CRGetEntry;
                    temp.entry_number = entry_number;
                    temp.entry_tag    = entry->entry_tag;
                    temp.entry_level  = entry->level;


                /*
                **  Issue the callback
                */
                    XtCallCallbacks (w, NgetEntryCallback, &temp);


                /*
                **  If the number of entries has changed, then find this entry
                **  number again.
                */
                    if (w->browse.num_entries != num_entries)
                       entry = StructGetEntryPtr (w, entry_number);


                /*
                **  If the entry is valid, then break out of the loop
                */
                    if (entry->valid) break;

            }


        /*
        **  Enable the widget
        */
            EnableDisplay (w);
       }


/*
**  If the height has not yet been adjusted, then do that before returning.
*/
    if (!entry->height_adjusted)
       DisplayAdjustHeight (w, entry);


/*
**  Return the entry to the caller
*/
    return entry;
}

/*
** Here is the local routine that will add the entry to the proper position
** within the linked list.
*/
void LclLinkAddEntry (w, AfterEntry, NewEntry)

  dns_widget w;
  EntryPtr AfterEntry, NewEntry;

{
/*
**  Local data declarations
*/
    EntryPtr NextEntry;


/*
**  If AfterEntry is NULL, then insert this entry into the beginning of the list.
*/
    if (AfterEntry == (EntryPtr) NULL)
       {
         if (w->browse.entryPtr == (EntryPtr) NULL)
              {
                w->browse.entryPtr = NewEntry;
                return;
              }
         else {
                NewEntry->next = w->browse.entryPtr;
                w->browse.entryPtr->prev = NewEntry;
                w->browse.entryPtr = NewEntry;
                return;
              }
       }


/*
**  If AfterEntry is at the end of the list, then insert this at the end.
*/
    if (AfterEntry->next == (EntryPtr) NULL)
       {
         AfterEntry->next = NewEntry;
         NewEntry->prev = AfterEntry;
         return;
       }


/*
**  Insert this node in between two nodes
*/
    NextEntry = AfterEntry->next;
    NewEntry->prev = AfterEntry;
    NewEntry->next = AfterEntry->next;
    AfterEntry->next = NewEntry;
    NextEntry->prev = NewEntry;
}

/*
** Here is the local routine that will delete the entry from the linked list.
*/

EntryPtr LclLinkDeleteEntry (w, EPtr)

  dns_widget w;
  EntryPtr EPtr;

{
/*
**  Local data declarations
*/
    EntryPtr NextEntry;
    EntryPtr PrevEntry;


/*
**  If this entry is the first in the linked list, then update the head pointer
**  to point to this nodes next pointer (which may be null).  If the next guy
**  is not NULL, then have his previous pointer be NULL so that he knows that
**  he is at the beginning of the list.
*/
    if (EPtr->prev == (EntryPtr) NULL)
       {
         w->browse.entryPtr = EPtr->next;
         NextEntry = EPtr->next;
         if (EPtr->next != (EntryPtr) NULL) NextEntry->prev = (EntryPtr) NULL;
         return NextEntry;
       }


/*
**  Now we know it's not at the head of the list.  See if it is the last entry
**  in the list.
*/
    if (EPtr->next == (EntryPtr) NULL)
       {
         PrevEntry = EPtr->prev;
         PrevEntry->next = (EntryPtr) NULL;
         return ((EntryPtr) NULL);
       }


/*
**  Now we know it must be in the middle of the list.
*/
    PrevEntry = EPtr->prev;
    NextEntry = EPtr->next;
    PrevEntry->next = NextEntry;
    NextEntry->prev = PrevEntry;
    return NextEntry;
}

/*
** Here is the local routine that will deallocate memory for component array.
*/

static void LclDeallocateComponent (entry)

  EntryPtr entry;

{
/*
**  Local data declarations
*/
    int i;


/*
**  If there are no components allocated, then return
*/
    if (entry->num_allocated == 0) return;


/*
**  Go through each component deallocating any memory allocated within it.
*/
    for (i = 0;  i <= entry->num_components - 1;  i++) {
       if (entry->entrycompPtr[i].type == KcompText)
          XtFree (entry->entrycompPtr[i].var.is_text.text);
       if (entry->entrycompPtr[i].type == KcompCString)
	  XmStringFree(entry->entrycompPtr[i].var.is_cstring.cstring);
	}

/*
**  Deallocate the component itself.
*/
    XtFree (entry->entrycompPtr);


/*
**  Show that it is gone
*/
    entry->num_components = 0;
    entry->num_allocated  = 0;
}

/*
**  Here is the local routine that will check the component array to see
**  if all the components for the entry have been set.  If the number of
**  components in the array equals the total number of components for the
**  entry, entry->valid will be set to TRUE.
*/

static void LclDetermineValidity (entry)

  EntryPtr entry;

{
/*
**  Local data declarations
*/
    int i;


/*
**  It may be already valid?
*/
    if (entry->valid) return;


/*
**  If there are no components, return.
*/
    if (entry->num_components == 0) return;


/*
**  For each component,  if it is not set, then return.
*/
    for (i = 0;  i <= entry->num_components - 1;  i++)
       if (entry->entrycompPtr[i].type == KcompNotSet)
          return;


/*
**  All of the components have been set.  Set the entry valid.
*/
    entry->valid = TRUE;
}

/*
**  This routine changes the x position of an AlignmentEnd component so that the
**  rest of the software only deals with an AlignmentBegin.
**
**  The component record is assumed to already include the text and the font.  It
**  is further assumed that this is RJ text.  This routine changes the X position
**  in the component record and cannot therefore be set after this routine.
*/

static void LclAdjustRightJustified (w, comp, max_pixels, max_chars, level)

  dns_widget w;
  CompPtr comp;
  int max_pixels, max_chars, level;

{
/*
**  Local data declarations
*/
    XFontStruct *compfont = (XFontStruct *) NULL;
    int textwidth, maxwidth;


/*
**  See if the component itself specifies the font
*/
    if (comp->var.is_text.font != (XFontStruct *) NULL) compfont = comp->var.is_text.font;


/*
**  Determine a potential default font according to the level number
*/
    if (compfont == (XFontStruct *) NULL)
       switch (level)
          {
            case 0  : compfont = w->browse.level0_font;  break;
            case 1  : compfont = w->browse.level1_font;  break;
            case 2  : compfont = w->browse.level2_font;  break;
            case 3  : compfont = w->browse.level3_font;  break;
            case 4  : compfont = w->browse.level4_font;  break;
          }


/*
**  If the font is still null, then set it to be the default
*/
    if (compfont == (XFontStruct *) NULL)
       compfont = w->browse.default_font;


/*
**  Now determine the width of the text being displayed
*/
    textwidth = XTextWidth(compfont, comp->var.is_text.text, (int)strlen(comp->var.is_text.text));


/*
**  Compute maxwidth
*/
    if (max_pixels != 0)
         maxwidth = max_pixels;
    else maxwidth = compfont->max_bounds.width * max_chars;


/*
**  If the text width is greater than the maxwidth, then change the maxwidth
**  to the textwidth to avoid a negative number in the next calculation.  The
**  result will probably be overlapping text.
*/
    if (maxwidth < textwidth)
       maxwidth = textwidth;


/*
**  Change the X and ORIG_WIDTH values based on this information
*/
    comp->orig_x = comp->orig_x + (maxwidth - textwidth);
    comp->orig_width = maxwidth;
}

/*
**  This routine sets the orig_width field of the component to the maxwidth of
**  a text.  Then if Truncation is turned the display routines can just use
**  the orig_width field to do the truncation.
**
**  The component record is assumed to already include the text and the font.
*/

static void LclDetermineMaxWidth (w, comp, max_pixels, max_chars, level)

  dns_widget w;
  CompPtr comp;
  int max_pixels, max_chars, level;

{
/*
**  Local data declarations
*/
    XFontStruct *compfont = (XFontStruct *) NULL;
    int maxwidth;


/*
**  See if the component itself specifies the font
*/
    if (comp->var.is_text.font != (XFontStruct *) NULL) compfont = comp->var.is_text.font;


/*
**  Determine a potential default font according to the level number
*/
    if (compfont == (XFontStruct *) NULL)
       switch (level)
          {
            case 0  : compfont = w->browse.level0_font;  break;
            case 1  : compfont = w->browse.level1_font;  break;
            case 2  : compfont = w->browse.level2_font;  break;
            case 3  : compfont = w->browse.level3_font;  break;
            case 4  : compfont = w->browse.level4_font;  break;
          }


/*
**  If the font is still null, then set it to be the default
*/
    if (compfont == (XFontStruct *) NULL)
       compfont = w->browse.default_font;


/*
**  Compute maxwidth
*/
    if (max_pixels != 0)
         maxwidth = max_pixels;
    else maxwidth = compfont->max_bounds.width * max_chars;


/*
**  Change the WIDTH value based on this information
*/
    comp->orig_width = maxwidth;
}

/*
**  This procedure walks up from the current widget looking for the class
**  type of widget.
*/

dns_widget StructFindWidget (wi)

    Widget wi;

{
/*
**  Local data declarations
*/
    WidgetClass w_class;


/*
**  Get the class record of the widget
*/
    w_class = XtClass(wi);

    if (w_class == (WidgetClass) widget_class) return (dns_widget) wi;


/*
**  Get the superclass of this class.
*/
    w_class = w_class->core_class.superclass;
    if (w_class == (WidgetClass) widget_class) return (dns_widget) wi;


/*
**  Otherwise call ourself looking at the parent of this widget.
*/
    return (StructFindWidget (XtParent(wi)));
}
