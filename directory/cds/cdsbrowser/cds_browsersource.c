/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cds_browsersource.c,v $
 * Revision 1.1.6.2  1996/02/18  19:24:51  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:08:32  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:02:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:36:53  root]
 * 
 * Revision 1.1.4.2  1992/12/30  12:38:12  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:22:02  zeliff]
 * 
 * Revision 1.1.2.4  1992/06/26  20:06:57  mfox
 * 	prune() now calls SourceCloseNode() to keep SourceNumEntries up to date.
 * 	Otherwise changing filters and expanding all can lead to browser crashes.
 * 	Cleaned up a lot of other cruft.
 * 	[1992/06/26  15:51:22  mfox]
 * 
 * Revision 1.1.2.3  1992/06/26  07:03:13  jim
 * 	Merge AIX 3.2 changes with the latest level.
 * 	[1992/06/22  22:40:25  jim]
 * 
 * Revision 1.1.2.2  1992/04/14  20:50:13  mfox
 * 	Fixes to allow CDS to compile cleanly with RIOS ANSI C compiler
 * 	[1992/04/14  19:39:31  mfox]
 * 	Revision 1.1.3.2  1992/05/29  18:39:31  jim
 * 	Changed to workaround AIX 3.2 Motif include file problems.
 * 
 * Revision 1.1.2.2  1992/04/14  20:50:13  mfox
 * 	Fixes to allow CDS to compile cleanly with RIOS ANSI C compiler
 * 	[1992/04/14  19:39:31  mfox]
 * 
 * Revision 1.1  1992/01/19  15:23:39  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#if !defined(lint) && !defined(SABER)
static char *SccsID = "@(#)cds_browsersource.c\t17\t(DECdns)\t11/20/1991";
#endif	/* neither lint nor saber-C */
/*
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990, 1991. ALL RIGHTS RESERVED.
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

#include <stdlib.h>
#include <stdio.h>

#ifdef DCE_SEC
#include <dce/sec_login.h>
#endif

#ifdef VMS
/*#pragma nostandard*/
#include "MainW.h"
#include "PushB.h"
#include "RowColumn.h"
#include "Text.h"
#include "TextP.h"      /* Has MAXINT */
#include "XmP.h"        /* Has XmVoidProc */
#include "MrmAppl.h"    /* Has Intrinsics, etc. */
/*#pragma standard*/
#else
#include <Xm/MainW.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/Text.h>
#ifndef AIX32
#include <Xm/TextP.h>
#endif
#include <Xm/XmP.h>
#include <Mrm/MrmAppl.h>
#endif

#include <cds_decwmp.h>

#include <dnsclerk.h>
#include <cds_browser.h>

                                                                          /* */
/*
**  Different node (DNS Entry) types.
**
**  Object types directly relate to their corresponding
**  Object Classes.
*/

#define clear_type           0

#define object_type_ceiling  clear_type

#define generic_object_type (object_type_ceiling + 1)
#define directory_type      (generic_object_type + 1)
#define slink_type          (directory_type + 1)


static char *class_name_p[] = {
                                "CDS_Clearinghouse",
                                "RPC_Entry"
                              };

/******************************************************/
/*  Local declarations needed for global definitions  */
/******************************************************/

/*
**  My local hierarchy storage structure
*/
    typedef struct node
        {
          int             level;         /* level number of children    */
          int             number;        /* number of children          */
          unsigned char  *text;          /* entry text                  */
          struct node    *sibling;       /* pointer to sibling or NULL  */
          struct node    *children;      /* pointer to children or NULL */
	  Widget	  stext;         /* stext widget for this entry */
          unsigned char   fullname[255]; /* DNS fullname                */
          unsigned char   type;          /* DNS (class) type            */
          unsigned char   filter[30];    /* user selected class filter  */
          Boolean         opened;        /* children are showing        */

        } _Node, *NodePtr;

/*
**  Forward routine declarations for callbacks
*/
    void Attach   ();
    void GetEntry ();

    void AckButton   ();

                                                                          /* */
/*******************************************/
/*  Global interface to the source module  */
/*******************************************/

/*
**  Global routines that return information about nodes.
*/
    Boolean SourceIsNodeParent ();

/*
**  Global routines that perform actions for the program
*/
    void SourceToggleNode ();
    void SourceOpenNode   ();
    void SourceCloseNode  ();

    Widget message_box_text ();

/*
**  Global data declarations
*/
    int               SourceNumEntries = 0;
    unsigned char     filter[30];
    NodePtr           new_node;
    NodePtr           prev_node;

    char *clear_class      = "CDS_Clearinghouse";
    char *rpc_class        = "RPC_Entry";

                                                                          /* */
/*
**  XtCallbackRec structures for "pure" source callbacks
*/
    XtCallbackRec AttachCB   [2] = {{(XmVoidProc)Attach  ,NULL}};
    XtCallbackRec GetEntryCB [2] = {{(XmVoidProc)GetEntry,NULL}};
    XtCallbackRec AckButtonCB   [2] = {{(XmVoidProc)AckButton  ,NULL}};

/*
**  External widget declarations
*/
    extern Widget  Browse;
    extern Widget  message_box;
    extern Widget  text1;
    extern Widget  button;

                                                                          /* */
/*****************************/
/*  Local data declarations  */
/*****************************/

    static _Node root = {1, 0, NULL, NULL, NULL, NULL, "/.:",
                         directory_type, "*", FALSE};

    unsigned char   entry_value[DNS_STR_FNAME_MAX];

    int    entry_len;
    int    number_of_entries;
    int    child_count;
    int    obj_count;
    int    slink_count;

/*
**  DNS Directory ICON.
*/
#include <dnsdir21.bitmap>
static Pixmap dnsdir21;

/*
**  Generic DNS Object ICON.
*/
#include <dnsobj21.bitmap>
static Pixmap dnsobj21;

/*
**  DNS Clearinghouse ICON.
*/
#include <dnsclear21.bitmap>
 static Pixmap dnsclear21;

/*
**  DNS Soft Link ICON.
*/
#include <dnsslink21.bitmap>
 static Pixmap dnsslink21;

                                                                          /* */
/********************************/
/*  Local routine declarations  */
/********************************/

/*
**  Forward routine declarations
*/
    NodePtr LclGetNodePtr     ();
    void    LclCloseNode      ();

    void    LclSetUpPixmaps   ();
    Pixmap  LclCreatePixmap   ();
    void    prune             ();


                                                                          /* */
/*
** Internal function to determine if authentication is required
*/
static int
isunauth()
{
#ifndef DCE_SEC
    return(0);
#else
    static int unauth = -1;
    error_status_t sec_status;
    sec_login_handle_t login_context_h;

    if (unauth < 0) {
	/* Get current context and save it */
	sec_login_get_current_context(&login_context_h, &sec_status);
	unauth = (sec_status != error_status_ok);
    }
    return(unauth);
#endif
}
                                                                          /* */
/*
**  Global routine that tells the caller if the given node has child nodes.
*/

Boolean SourceIsNodeParent (node_number, entry_tag)

  int node_number;
  unsigned int entry_tag;

{

/*
**  Local data declarations
*/
    NodePtr  node;

/*
**  Get at the node
*/
   node = LclGetNodePtr (node_number);

/*
** If the node is a directory, we can expand it
*/
   if (node->type == directory_type) {
        return TRUE;
   }
}

                                                                          /* */
/*
**  AttachToSource callback routine
*/

void Attach (w)

  Widget w;

{
/*
**  Local data declarations
*/
    unsigned int entry_tags [1];

/*
**  Make room for the entries.  I will pass the tag array here since I
**  know that I have exactly one entry and its easy to figure out the tag.
*/
    entry_tags[0] = (unsigned int) &root;
    AddEntries (w, 0, 1, 0, entry_tags, TRUE);

/*
**  Reflect this addition in the global count.
*/
    SourceNumEntries = 1;

/*
**  Position entry 1 in the middle.  This is to test a bug about positioning
**  before Realize has occurred.
**
**  PositionDisplay (w, 1, PositionMiddle);
*/

/*
**  Set up the DNS class filter
*/
    strcpy((char *)filter, "*");

}


                                                                          /* */
/*
**  This routine is called when the widget wants the Source module to set
**  the information associated with a particular entry.
*/

void GetEntry (w, unused_tag, data)

  Widget w;
  int unused_tag;
  CallbackStruct *data;

{
/*
**  Local data declarations
*/
    NodePtr node;

/*
**  Get at the node
*/
   node = LclGetNodePtr (data->entry_number);

/*
**  Set up the pixmaps
*/
    LclSetUpPixmaps (w);

/*
**  Set the entry information.
*/
     switch (node->type)
        {
        case directory_type:
	   SetEntryNumComponents (w, data->entry_number, 2);
	   SetEntryTag(w, data->entry_number, node);
           SetEntryIndexWindow(w, data->entry_number, TRUE);
	   SetComponentPixmap (w, data->entry_number, 1, 0, 0,
	                              dnsdir21, dnsdir21_width, dnsdir21_height);
           SetComponentText (w, data->entry_number, 2, 1, 0, node->fullname,
         			    NULL, XmALIGNMENT_BEGINNING, 50, 0);
           break;
        case generic_object_type:
	   SetEntryNumComponents (w, data->entry_number, 2);
	   SetEntryTag(w, data->entry_number, node);
           SetEntryIndexWindow(w, data->entry_number, TRUE);
	   SetComponentPixmap (w, data->entry_number, 1, 0, 0,
	                              dnsobj21, dnsobj21_width, dnsobj21_height);
           SetComponentText (w, data->entry_number, 2, 1, 0, node->fullname,
         			    NULL, XmALIGNMENT_BEGINNING, 50, 0);
           break;
        case clear_type:
	   SetEntryNumComponents (w, data->entry_number, 2);
	   SetEntryTag(w, data->entry_number, node);
           SetEntryIndexWindow(w, data->entry_number, TRUE);
	   SetComponentPixmap (w, data->entry_number, 1, 0, 0,
	                              dnsclear21, dnsclear21_width, dnsclear21_height);
           SetComponentText (w, data->entry_number, 2, 1, 0, node->fullname,
         			    NULL, XmALIGNMENT_BEGINNING, 50, 0);
           break;
        case slink_type:
	   SetEntryNumComponents (w, data->entry_number, 2);
	   SetEntryTag(w, data->entry_number, node);
           SetEntryIndexWindow(w, data->entry_number, TRUE);
	   SetComponentPixmap (w, data->entry_number, 1, 0, 0,
	                              dnsslink21, dnsslink21_width, dnsslink21_height);
           SetComponentText (w, data->entry_number, 2, 1, 0, node->fullname,
         			    NULL, XmALIGNMENT_BEGINNING, 50, 0);
           break;
        default:
	   SetEntryNumComponents (w, data->entry_number, 2);
	   SetEntryTag(w, data->entry_number, node);
           SetEntryIndexWindow(w, data->entry_number, TRUE);
	   SetComponentPixmap (w, data->entry_number, 1, 0, 0,
	                              dnsobj21, dnsobj21_width, dnsobj21_height);
           SetComponentText (w, data->entry_number, 2, 1, 0, node->fullname,
         			    NULL, XmALIGNMENT_BEGINNING, 50, 0);
           break;
        }

}

                                                                          /* */
/*
**  This callback routine is called when the Acknowledged button
**  is selected.
*/

void AckButton ()

{

/*
**  Destroy the Message Box widget.
*/
    XtDestroyWidget( message_box );

}

                                                                          /* */
/*
**  Routine that maps a node_number into a node structure pointer.
*/

NodePtr LclGetNodePtr (node_number)

  int node_number;

{
/*
**  Local routine data
*/
    int i;
    NodePtr current_node = &root;


/*
**  Loop through until it's found.  If we hit the end of the list, then
**  we'll return a null pointer.
*/
    if (node_number != 1)
       for (i = 2;  i <= node_number;  i++)
            if (current_node == NULL)
                 break;
            else if ((current_node->opened) && (current_node->number > 0))
                     current_node = current_node->children;
                     else current_node = current_node->sibling;

/*
**  Return the node address
*/
    return current_node;

}

                                                                          /* */
/*
**  Global routine that opens a closed node or closes an open node.
*/

void SourceToggleNode (node_number, entry_tag, w)

  int node_number;
  unsigned int entry_tag;
  Widget w;

{
/*
**  Local data declarations
*/
    NodePtr node;

/*
**  Get at the node.
*/
   node = LclGetNodePtr (node_number);

/*
**  If it is opened, then close it.  Otherwise open it.
*/
    if (node->opened == TRUE)
         SourceCloseNode (node_number, entry_tag);
    else SourceOpenNode  (node_number, entry_tag, w);

}

                                                                          /* */
/*
**  Global routine that opens a node, given the node number
*/

void SourceOpenNode (node_number, entry_tag, w)

  int node_number;
  unsigned int entry_tag;
  Widget w;

{

/*
** Local data declarations
*/
    NodePtr  node;
    NodePtr  child_node;
    int      i;
    int      browser_status;
    int      x, y;

                                                                          /* */

/*
** we haven't expanded any nodes yet - this var also serves as a flag to
** determine if we have to do special linking later on.
*/
   number_of_entries = 0;
   browser_status = BROWSER_SUCCESS;

/*
**  Get at the node.
*/
   node = LclGetNodePtr (node_number);

   /*
    * If it's not a parent (i.e., its not a directory), return
    */
   if (!SourceIsNodeParent( node_number, entry_tag ))
      {
      return;
      }

   /*
    * If the filter has changed and it's not empty, prune the
    * the tree and make sure that the node is closed
    */
   if ((strcmp((char *)node->filter, (char *)filter) != 0) &&
       (node->filter[0] != '\0'))
      {
      prune(node, node_number);
      node->opened = FALSE;
      }

   /*
    * If node already open, return
    */
   if (node->opened)
      {
      return;
      }

   /*
    * Set the first child of this node.
    */
   child_node = node->children;

                                                                          /* */
   /*
    * Get the DNS entries into the tree if they're not there yet.
    */
   if (child_node == NULL)
      {
      /*
       * point to the current node as the 'previous' node, that is, the node
       * the we link the new node off
       */
      prev_node = node;

      /*
       * get the node name that we're expanding
       */
      strcpy((char *)entry_value, (char *)node->fullname);

      entry_len = strlen( (char *)entry_value );

      /*
       * Enumerate the subordinates (children, objects,
       * soft links) of this Directory.
       */

      browser_status = browser_enum( node );

      if ( browser_status == BROWSER_ERROR )
          {
          /*
          **  Announce the error and prune the tree.
          */
          prune(node, node_number);
          node->opened = FALSE;
          number_of_entries = 0;
          return;
          }

         }/* end if */

      /*
       * link the last new node to the parent's sibling
       */
      if (number_of_entries > 0)
         {
         new_node->sibling = node->sibling;
         }

      /*
       * mark the node as open
       */
      node->opened = TRUE;

      SourceNumEntries = SourceNumEntries + node->number;

      /*
       * save the current filter
       */
      strcpy((char *)node->filter, (char *)filter);

/*
**  Add the entries.  This code does not yet use the entry_tags array.
*/
      AddEntries(Browse, node_number, node->number, node->level, NULL, TRUE);


/*
** Get to the first child of this node.
*/
      child_node = node->children;

/*
**  Make non-directories not selectable (insensitive).
*/
      DisableDisplay(Browse);
      for (i = 1;  i <= node->number;  i++)
          {
            if (child_node->type != directory_type) {
               SetEntrySensitivity(Browse, (node_number + i), FALSE);
            }
            child_node = child_node->sibling;
          }
      EnableDisplay(Browse);

/*
** Reset to the first child of this node.
*/
      child_node = node->children;

/*
**  For each child, call SetEntry if the child has children.  Also set their
**  positions in case we are in we have a UserDefined Tree Style.
*/
      GetEntryPosition(Browse, node_number, FALSE, &x, &y);
      for (i = 1;  i <= node->number;  i++)
          {
            if (child_node->children != 0)
               SetEntry (Browse, node_number+i, 0, 0, 2, 1, 0, TRUE);
            child_node = child_node->sibling;
      	  x += 30;
     	  y += 30;
      	  SetEntryPosition(Browse, node_number+i, FALSE, x, y);
          }

   }

                                                                          /* */
/*
**  Global routine that closes a node, given the node number
*/

void SourceCloseNode (node_number, entry_tag)

  int node_number;
  unsigned int entry_tag;

{
/*
**  Local data declarations
*/
    NodePtr node;

/*
**  Get at the node.
*/
   node = LclGetNodePtr (node_number);

/*
**  Call the local recursive close routine.
*/
    LclCloseNode (node, node_number);

}

                                                                          /* */
/*
**  Recursively close all nodes given a current node pointer
**  and a current node number.
*/

void LclCloseNode (node, node_number)

  NodePtr node;
  int node_number;

{
/*
**  Local data declarations
*/
    int i;
    NodePtr child_node;

/*
**  If the current node is not opened, then return
*/
    if (node->opened == FALSE)
       return;

/*
**  Get to the first child of this node
*/
    child_node = node->children;

/*
**  For each child, call CloseNode on each child
*/
    for (i=1;  i<=node->number;  i++)
        {
          LclCloseNode (child_node, node_number);
          child_node = child_node->sibling;
        }

/*
**  Remove children
*/
    DeleteEntries (Browse, node_number, node->number);

/*
**  Mark the node closed
*/
    node->opened = FALSE;
    if (node->stext != NULL) XtUnmanageChild(node->stext);

/*
**  Reflect this removal in the global count.
*/
    SourceNumEntries = SourceNumEntries - node->number;

}

                                                                          /* */
void LclSetUpPixmaps (w)

  Widget w;

{
/*
**  Local data declarations
*/

    Pixel background_pixel;
    Pixel foreground_pixel;
    Arg args [2];

/*
**  Get the foreground/background colors and match them for the icons
*/
    XtSetArg    (args[0], XmNforeground, &foreground_pixel);
    XtSetArg    (args[1], XmNbackground, &background_pixel);
    XtGetValues (w, args, 2);

/*
**  Create the pixmaps if it hasn't already been done.
*/
    if (dnsdir21 == 0)
       {
       dnsdir21 = LclCreatePixmap(w, dnsdir21_bits,
                                  dnsdir21_width, dnsdir21_height,
                                  background_pixel, foreground_pixel);
       }
    if (dnsobj21 == 0)
       {
       dnsobj21 = LclCreatePixmap(w, dnsobj21_bits,
                                  dnsobj21_width, dnsobj21_height,
                                  background_pixel, foreground_pixel);
       }
    if (dnsclear21 == 0)
       {
       dnsclear21 = LclCreatePixmap(w, dnsclear21_bits,
                                    dnsclear21_width, dnsclear21_height,
                                    background_pixel, foreground_pixel);
       }
    if (dnsslink21 == 0)
       {
       dnsslink21 = LclCreatePixmap(w, dnsslink21_bits,
                                    dnsslink21_width, dnsslink21_height,
                                    background_pixel, foreground_pixel);
       }
}


                                                                          /* */

Pixmap LclCreatePixmap(w, pixmap_bits, pixmap_height, pixmap_width, background_pixel, foreground_pixel)

  Widget           w;
  unsigned short  *pixmap_bits;
  int              pixmap_height;
  int              pixmap_width;
  Pixel            background_pixel;
  Pixel            foreground_pixel;

{
/*
**  Local data declarations
*/

    Screen  *screen  = XtScreen(w);
    Display *display = DisplayOfScreen (screen);
    Pixmap  dwtpixmap;

/*
**  Create the pixmap.
*/
    dwtpixmap = XCreatePixmapFromBitmapData (
	display,				/* (IN) display */
	XDefaultRootWindow(display),		/* (IN) drawable */
	(char *)pixmap_bits,			/* (IN) bitmap data */
	pixmap_width,				/* (IN) pixmap width */
	pixmap_height,				/* (IN) pixmap height */
	foreground_pixel,			/* (IN) foreground pixel */
	background_pixel,			/* (IN) background pixel */
	DefaultDepthOfScreen (screen));		/* (IN) pixmap depth */

    return( dwtpixmap );
}

                                                                          /* */
/*
**  Recursively delete all nodes given a current node pointer
**  and a current node number.
*/

void prune (node, node_number)

  NodePtr node;
  int node_number;

{
/*
**  Local data declarations
*/
    int i;
    NodePtr child_node;
    NodePtr temp_node;

    SourceCloseNode( node_number, 0);

/*
**  Get to the first child of this node
*/
    child_node = node->children;

/*
**  For each child, call prune on each child
*/
    for (i=1;  i<=node->number;  i++)
        {
          prune(child_node, node_number);
          temp_node = child_node;
          child_node = child_node->sibling;
          free(temp_node);
        }
/*
**  Mark the node closed
*/
    node->opened = FALSE;
    node->children = NULL;

    node->number   = 0;

    if (node->stext != NULL) XtUnmanageChild(node->stext);
}


                                                                          /* */
/* ++
 *
 * Widget
 * message_box_error( parent, title, text )
 *
 *
 *  This routine creates a message box with corresponding text...
 *
 *  Arguments:
 *    parent - the parent widget.
 *    title - the name or title given to the widget
 *    text - the text to be displayed in the box.
 *
 *  Return Codes:
 *
 *    Widget - the message box widget.
 *
 *  Example:
 *          message_box_widget = message_box_error( w,
 *           "Test", "Text message" );
 *
 *--
 */

Widget
message_box_text( parent, title, text )

   Widget  parent;           /* IN Parent widget */
   unsigned char    *title;  /* IN Name (title) given to the widget */
   unsigned char    *text;   /* IN Text message */

   {

   Arg     al[20];
   Widget  m;

   XmString title_cs = XmStringLtoRCreate((char *)title , "ISO8859-1");
   XmString  label_cs = XmStringLtoRCreate((char *)text , "ISO8859-1");

   Screen         *screen  = XtScreen( parent );
   Display        *display = DisplayOfScreen( screen );

   Position  parentx;
   Position  parenty;

   int  ac = 0;
   int  mode   = XmDIALOG_MODELESS;
   int  width = 400, height = 125;

                                                                          /* */

   XtSetArg ( al[ac], XmNx, &parentx ); ac++;
   XtSetArg ( al[ac], XmNy, &parenty ); ac++;
   XtGetValues ( parent, al, ac );


   ac = 0;
   XtSetArg ( al[ac], XmNdialogStyle, mode); ac++;
   XtSetArg ( al[ac], XmNtitle, title_cs ); ac++;
   XtSetArg ( al[ac], XmNlabelString, label_cs ); ac++;
   XtSetArg ( al[ac], XmNokCallback, AckButtonCB ); ac++;
   XtSetArg ( al[ac], XmNx,(parentx+50) ); ac++;
   XtSetArg ( al[ac], XmNy,(parenty+120) ); ac++;

   /*
    * Create the Message Box Popup.
    */
   m = (Widget)XmCreateInformationDialog( parent, " ", al, ac );
   message_box = m;

   XtManageChild( m );

   XtFree( title_cs );
   XtFree( label_cs );

   return ( m );

   }/* end message_box_text */


                                                                          /* */

/*++
 *
 *  int
 *  browser_add_node( entry_str_sname_p, node )
 *
 *  This routine takes in a "new" DNS Entry (DNS Directory,
 *  Object or Soft Link) and issues the appropriate calls
 *  to add it to the tree.
 *
 *  Arguments:
 *
 *    entry_str_fname_p - a ptr to the address of a DNS
 *                        Entry (simple name, string).
 *
 *    node - the existing node (a DNS Directory).
 *
 *  Return Codes:
 *
 *    BROWSER_SUCCESS - success.
 *    BROWSER_ERROR  - error (something went wrong).
 *
 *  Example:
 *          status = browser_add_node( &entry_str_sname, node )
 *
 *   where:
 *         unsigned char  *entry_str_sname_p;
 *         NodePtr        node;
 *--
 */

int
browser_add_node( entry_str_sname_p, node )

   unsigned char  *entry_str_sname_p; /* IN DNS Entry simple string */
   NodePtr        node;               /* IN Existing node/Directory */

   {


                                                                          /* */
    /*
     * First, allocate some memory for the new node.
     */
    new_node = ( struct node * )malloc( sizeof( _Node ) );

    /*
     * If memory is not available, tell the user and get out.
     */
    if ( new_node == 0 )
        {
         return( BROWSER_ERROR );
        }

                                                                          /* */

    /*
     * Build the Fullname in the new node.
     */
    strcpy( (char *)&new_node->fullname[0], (char *)&node->fullname[0] );

    /*
     * Special case - if the entry_value_len is 1,
     *  then it's the Root - just slap on the simple name.
     *
     * Otherwise, add a delimiter and the simple name.
     */
    if ( entry_len == 1 )
    	{
         strcpy( (char *)&new_node->fullname[entry_len],
                 (char *)entry_str_sname_p );
        }
    else
        {
	 sprintf ((char *)&new_node->fullname[entry_len], "/%s", entry_str_sname_p );
        }

    /*
     * Now fill in the new node's "other" parameters.
     */
    new_node->level     = node->level + 1;
    new_node->number    = 0;
    new_node->text      = NULL;
    new_node->sibling   = NULL;
    new_node->children  = NULL;
    new_node->stext     = NULL;
    new_node->filter[0] = '\0';
    new_node->opened    = FALSE;

                                                                          /* */


    /*
     * Point the 'previous' node at 'this' node.
     */
    if ( ( child_count == 0 ) &&
         ( obj_count == 0 ) &&
         ( slink_count == 0 ) &&
         ( number_of_entries == 0 ) )
        {
         prev_node->children = new_node;
        }
    else
        {
         prev_node->sibling = new_node;
        }

    /*
     * Make this node the previous node.
     */
    prev_node = new_node;


    return( BROWSER_SUCCESS );

 } /* end browser_add_node() */

                                                                          /* */

/* ++
 *
 * int
 * browser_set_node_type( object_class_p )
 *
 *  This routine will return an object type given
 *  an object class.
 *
 *  Arguments:
 *    object_class_p - a ptr to the object's DNS$CLASS string name.
 *
 *  Return Codes:
 *
 *    int - the object's type.
 *
 *  Example:
 *    node->type = browser_set_node_type( class_str_sname_p )
 *
 *  where:
 *          unsigned char  *class_str_sname_p;
 *
 *--
 */

int
browser_set_node_type( object_class_p )

   unsigned char *object_class_p;

   {

   int  i;

   /*
    * Loop thru and compare the given object class with the known
    * list of object classes.  If there's a match, return the
    * corresponding object class type.
    */
   for (i = 0;  i <= object_type_ceiling;  i++) {

        if ( strcmp( (char *)object_class_p, class_name_p[i] ) == 0 )
           return( i );
   }

   /*
    * If no match, return unknown/generic object type.
    */

   return( generic_object_type );


   }/* end browser_set_node_type */

                                                                          /* */

/* ++
 *
 * int
 * browser_enum_child( node )
 *
 *  This routine looks at node passed (a DNS Directory)
 *  and issues the appropriate DNS Portable C Clerk Interface
 *  calls to enumerate the Child Directories of this node.
 *
 *  Arguments:
 *    node - the existing node (a DNS Directory).
 *
 *  Return Codes:
 *
 *    BROWSER_SUCCESS - success.
 *    BROWSER_ERROR  - error (something went wrong).
 *
 *  Example:
 *          status = browser_enum_child( node )
 *
 *  where:
 *          NodePtr  node;
 *
 *--
 */

int
browser_enum_child( node )

   NodePtr  node;       /* IN Existing node */

   {

   dns_handle          enum_child_handle;
   struct dnsFlagStat  enum_child_flags;
   unsigned char       child_str_sname[DNS_STR_SNAME_MAX];
   dns_opq_fname       dir_opq_fname;
   int                 browser_status;
   int                 dns_status;
   int                 child_str_sname_len=DNS_STR_SNAME_MAX;

   memset(&enum_child_flags,0,sizeof(struct dnsFlagStat));
   browser_status  = BROWSER_SUCCESS;
   dns_status      = DNS_SUCCESS;

   enum_child_flags.fsConf = dnsConfLow;
   enum_child_flags.fsWait = 30;
   enum_child_flags.fsNameType = dnsStrDCE;
   enum_child_flags.fsUnAuth = enum_child_flags.fsTrustAll = isunauth();

   /*
    * Get the handle for the enumerate child call.
    */
   enum_child_handle = ( dns_handle )dnsGetHandle();

   if ( enum_child_handle == ( dns_handle )0 )
       {
        return( BROWSER_ERROR );
       }

   /*
    * Loop thru and enumerate all of the children of this directory.
    */
   while( dns_status == DNS_SUCCESS )
       {
        dns_status = dnsEnumChild( (dns_full_name_t *)&node->fullname[0],
                                   enum_child_handle,
                                   (dns_simple_name_t *) NULL,
                                   (dns_simple_name_t **) NULL,
                                   child_str_sname,
                                   &child_str_sname_len,
                                   &enum_child_flags );

        /*
         * If there is an error, tell the user,
         * free the handle and get out.
         */
        if ( dns_status == DNS_ERROR )
	    {
             dnsFreeHandle( enum_child_handle );

             return( BROWSER_ERROR );
	    }

        /*
         * If there are no (more) entries, leave the loop.
         * Also tell the user if there are no entries at all.
         * Reset the status.
         */
        if ( dns_status == DNS_EMPTY )
	    {
             if ( child_count == 0 )
	         {
		 }

             dns_status = DNS_SUCCESS;
             break;
	    }

        /*
         * Add this Child Directory (node) to the tree.
         */
        browser_status = browser_add_node( child_str_sname,
                                           node );

        /*
         * If there is an error, free the handle and get out.
         */
        if ( browser_status == BROWSER_ERROR )
	    {
             dnsFreeHandle( enum_child_handle );

             return( BROWSER_ERROR );
 	    }

        /*
         * The call was successful, bump the child count and reset length.
         */
        child_count = child_count + 1;
        child_str_sname_len = DNS_STR_SNAME_MAX;

        /*
         * Set the node type to reflect the fact that
         * this is a Directory.
         */
        new_node->type = directory_type;

       } /* end while( dns_status ==... */

                                                                          /* */

   /*
    * Free the handle.
    */
   dnsFreeHandle( enum_child_handle );

   return( BROWSER_SUCCESS );


   }/* end browser_enum_child */

                                                                          /* */

/* ++
 *
 * int
 * browser_enum_obj()
 *
 *  This routine looks at node passed (a DNS Directory)
 *  and issues the appropriate DNS Portable C Clerk Interface
 *  calls to enumerate the Objects of this node.
 *
 *  Arguments:
 *    node - the existing node (a DNS Directory).
 *
 *  Return Codes:
 *
 *    BROWSER_SUCCESS - success.
 *    BROWSER_ERROR  - error (something went wrong).
 *
 *  Example:
 *          status = browser_enum_obj( node )
 *
 *  where:
 *          NodePtr  node;
 *
 *--
 */

int
browser_enum_obj( node )

   NodePtr   node;        /* IN Existing node */

   {

   dns_handle          enum_obj_handle;

   struct dnsFlagStat  enum_obj_flags;

   unsigned char       obj_str_sname[DNS_STR_SNAME_MAX];
   unsigned char       class_str_sname[DNS_STR_SNAME_MAX];
   dns_simple_name_t   *obj_opq_sname_p;
   dns_simple_name_t   *class_opq_sname_p;
   dns_simple_name_t   name_filter_opq_sname;
   dns_simple_name_t   class_filter_opq_sname;
   dns_full_name_t     dir_opq_fname;

   int                 ret_class;
   int                 browser_status;
   int                 dns_status;
   int                 dir_opq_fname_len=DNS_OPQ_FNAME_MAX;
   int                 name_filter_opq_sname_len=DNS_OPQ_SNAME_MAX;
   int                 class_filter_opq_sname_len=DNS_OPQ_SNAME_MAX;
   int                 obj_str_sname_len=DNS_STR_SNAME_MAX;
   int                 class_str_sname_len=DNS_STR_SNAME_MAX;

                                                                          /* */

   memset(&enum_obj_flags,0,sizeof(struct dnsFlagStat));
   browser_status  = BROWSER_SUCCESS;
   dns_status      = DNS_SUCCESS;
   ret_class       = 0;

   enum_obj_flags.fsConf      = dnsConfLow;
   enum_obj_flags.fsWait      = 30;
   enum_obj_flags.fsNameType = dnsStrDCE;
   enum_obj_flags.fsUnAuth = enum_obj_flags.fsTrustAll = isunauth();


   /*
    * If no filter is set (filter would be "*"), find out
    * what Class this object is, by setting Return Class to
    * TRUE for the dnsEmumObj call. The DNS$CLASS attr should
    * then be returned.
    */
   if ( strcmp( (char *)&filter[0], "*" ) == 0 )
       {
        ret_class = 1;
       }

   /*
    * Get the handle for the enumerate obj call.
    */
   enum_obj_handle = ( dns_handle )dnsGetHandle();

   if ( enum_obj_handle == ( dns_handle )0 )
       {
        return( BROWSER_ERROR );
       }

   /*
    * Loop thru and enumerate all of the objects of this directory.
    */
   while( dns_status == DNS_SUCCESS )
       {
        dns_status = dnsEnumObj( (dns_full_name_t *)&node->fullname[0],
                                 enum_obj_handle,
                                 (dns_simple_name_t *)&filter[0],
                                 (dns_simple_name_t *) NULL,
                                 &ret_class,
                                 (dns_simple_name_t **) NULL,
                                 obj_str_sname,
                                 &obj_str_sname_len,
                                 (dns_simple_name_t **) NULL,
                                 class_str_sname,
                                 &class_str_sname_len,
                                 &enum_obj_flags );

        if ( dns_status == DNS_ERROR )
	    {
             dnsFreeHandle( enum_obj_handle );
             return( BROWSER_ERROR );
	    }

        /*
         * If there are no (more) entries, leave the loop.
         * Also tell the user if there are no entries at all.
         * Reset the status.
         */
        if ( dns_status == DNS_EMPTY )
	    {
             if ( obj_count == 0 )
	         {
		 }

             dns_status = DNS_SUCCESS;
             break;
	    }


        /*
         * Add this Object (node) to the tree.
         */
        browser_status = browser_add_node( obj_str_sname,
                                           node );

        /*
         * If there is an error, free the handle and get out.
         */
        if ( browser_status == BROWSER_ERROR )
	    {
             dnsFreeHandle( enum_obj_handle );
             return( BROWSER_ERROR );
 	    }

        /*
         * The call was successful, bump the object count & reset the length.
         */
        obj_count = obj_count + 1;
        obj_str_sname_len = DNS_STR_SNAME_MAX;
        class_str_sname_len = DNS_STR_SNAME_MAX;

        /*
         * Set this object's type (object class).  If the filter
         * is not set, send the returned class instead.
         */

        if ( strcmp( (char *)&filter[0], "*" ) == 0 ) {

              new_node->type = browser_set_node_type( class_str_sname );
        }
        else {

              new_node->type = browser_set_node_type( &filter[0] );
        }

       } /* end while( dns_status ==... */

   /*
    * Free the handle.
    */
   dnsFreeHandle( enum_obj_handle );
   return ( BROWSER_SUCCESS );

   }/* end browser_enum_obj */

                                                                          /* */

/* ++
 *
 * int
 * browser_enum_slink( node )
 *
 *  This routine looks at node passed (a DNS Directory)
 *  and issues the appropriate DNS Portable C Clerk Interface
 *  calls to enumerate the Soft Links of this node.
 *
 *  Arguments:
 *    node - the existing node (a DNS Directory).
 *
 *  Return Codes:
 *
 *    BROWSER_SUCCESS - success.
 *    BROWSER_ERROR  - error (something went wrong).
 *
 *  Example:
 *          status = browser_enum_slink( node )
 *
 *  where:
 *          NodePtr  node;
 *
 *--
 */

int
browser_enum_slink( node )

   NodePtr  node;       /* IN Existing node */

   {

   dns_handle          enum_slink_handle;

   struct dnsFlagStat  enum_slink_flags;

   unsigned char       slink_str_sname[DNS_STR_SNAME_MAX];
   dns_opq_sname       *slink_opq_sname_p;


   dns_opq_sname       name_filter_opq_sname;

   dns_opq_fname       dir_opq_fname;

   int                 browser_status;
   int                 dns_status;
   int                 dir_opq_fname_len=DNS_OPQ_FNAME_MAX;
   int                 name_filter_opq_sname_len=DNS_OPQ_SNAME_MAX;
   int                 slink_str_sname_len=DNS_STR_SNAME_MAX;


                                                                          /* */
   memset(&enum_slink_flags,0,sizeof(struct dnsFlagStat));
   browser_status  = BROWSER_SUCCESS;
   dns_status      = DNS_SUCCESS;

   enum_slink_flags.fsConf = dnsConfLow;
   enum_slink_flags.fsWait = 30;
   enum_slink_flags.fsNameType = dnsStrDCE;
   enum_slink_flags.fsUnAuth = enum_slink_flags.fsTrustAll = isunauth();

   /*
    * Get the handle for the enumerate soft link call.
    */
   enum_slink_handle = ( dns_handle )dnsGetHandle();

   if ( enum_slink_handle == ( dns_handle )0 )
       {
        return( BROWSER_ERROR );
       }

   /*
    * Loop thru and enumerate all of the soft links
    * of this directory.
    */
   while( dns_status == DNS_SUCCESS )
       {
        dns_status = dnsEnumSLink( (dns_full_name_t *)&node->fullname[0],
                                   enum_slink_handle,
                                   (dns_simple_name_t *) NULL,
                                   (dns_simple_name_t **) NULL,
                                   slink_str_sname,
                                   &slink_str_sname_len,
                                   &enum_slink_flags );

        /*
         * If there is an error, tell the user,
         * free the handle and get out.
         */
        if ( dns_status == DNS_ERROR )
	    {
             dnsFreeHandle( enum_slink_handle );

             return( BROWSER_ERROR );
	    }

        /*
         * If there are no (more) entries, leave the loop.
         * Also tell the user if there are no entries at all.
         * Reset the status.
         */
        if ( dns_status == DNS_EMPTY )
	    {
             if ( slink_count == 0 )
	         {
		 }

             dns_status = DNS_SUCCESS;
             break;
	    }

        /*
         * Add this Soft Link (node) to the tree.
         */
        browser_status = browser_add_node( slink_str_sname,
                                           node );

        /*
         * If there is an error, free the handle and get out.
         */
        if ( browser_status == BROWSER_ERROR )
	    {
             dnsFreeHandle( enum_slink_handle );

             return( BROWSER_ERROR );
 	    }

        /*
         * The call was successful, bump the soft link count & reset length.
         */
        slink_count = slink_count + 1;
        slink_str_sname_len = DNS_STR_SNAME_MAX;

        /*
         * Set the node type to reflect the fact that
         * this is a Soft Link.
         */
        new_node->type = slink_type;

       } /* end while( dns_status ==... */

                                                                          /* */

   /*
    * Free the handle.
    */
   dnsFreeHandle( enum_slink_handle );

   return( BROWSER_SUCCESS );


   }/* end browser_enum_slink */


                                                                          /* */
/* ++
 *
 * int
 * browser_enum()
 *
 *
 *  This routine looks at node passed (a DNS Directory)
 *  and issues the appropriate DNS Portable C Clerk Interface
 *  calls to enumerate the subordinate entries (Child Directory
 *  Pointers, Object and Soft Links) of this node.
 *
 *  Arguments:
 *    node - the existing node (a DNS Directory).
 *
 *  Return Codes:
 *
 *    BROWSER_SUCCESS - success.
 *    BROWSER_ERROR  - error (something went wrong).
 *
 *  Example:
 *          status = browser_enum( node )
 *
 *--
 */

int
browser_enum( node )

   NodePtr  node;       /* IN Existing node */

   {

   int  browser_status;

                                                                          /* */
   browser_status = BROWSER_SUCCESS;

   child_count = 0;
   obj_count   = 0;
   slink_count = 0;

   /*
    * Enumerate all of the Children (child directory pointers)
    * of this Directory first.
    */
   browser_status = browser_enum_child( node );

   if ( browser_status == BROWSER_ERROR )
       {
        return( BROWSER_ERROR );
       }

   /*
    * Accumulate the new entries.
    */

   number_of_entries = number_of_entries + child_count;

   /*
    * Now, enumerate the all of the Objects of this Directory.
    */
   browser_status = browser_enum_obj( node );

   if ( browser_status == BROWSER_ERROR )
       {
        return( BROWSER_ERROR );
       }

   /*
    * Again, accumulate the new entries.
    */

   number_of_entries = number_of_entries + obj_count;

   /*
    * Finally, enumerate the all of the Soft Links
    * of this Directory.
    */
   browser_status = browser_enum_slink( node );

   if ( browser_status == BROWSER_ERROR )
       {
        return( BROWSER_ERROR );
       }

   /*
    * Again, accumulate the new entries.
    */

   number_of_entries = number_of_entries + slink_count;
   node->number      = number_of_entries;

   return ( BROWSER_SUCCESS );

   }/* end browser_enum */

