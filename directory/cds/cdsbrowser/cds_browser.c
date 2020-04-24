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
 * $Log: cds_browser.c,v $
 * Revision 1.1.13.2  1996/02/18  19:24:49  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:08:31  marty]
 *
 * Revision 1.1.13.1  1995/12/08  15:02:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:36:50  root]
 * 
 * Revision 1.1.9.2  1993/06/10  19:15:37  sommerfeld
 * 	Clean up include paths.
 * 	[1993/06/08  18:08:06  sommerfeld]
 * 
 * Revision 1.1.3.6  1993/02/03  22:33:09  blurie
 * 	Fix for OT #7107.  Change SNI_PROPRIETARY to SNI_PRIVATE.
 * 	[1993/02/03  20:53:23  blurie]
 * 
 * Revision 1.1.3.5  1992/12/30  12:37:59  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:21:50  zeliff]
 * 
 * Revision 1.1.3.4  1992/12/18  19:18:36  mfox
 * 	Change some ints to Dimension variables to avoid crash on some platforms.
 * 	[1992/12/18  19:17:58  mfox]
 * 
 * Revision 1.1.3.3  1992/12/15  15:54:34  tom
 * 	Add setlocale call for messages and i18n.
 * 	Clean up some warnings from X.
 * 	[1992/12/15  15:16:51  tom]
 * 
 * Revision 1.1.3.2  1992/09/29  19:08:14  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:09:51  weisman]
 * 
 * Revision 1.1  1992/01/19  15:23:38  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#if !defined(lint) && !defined(SABER)
static char *SccsID = "@(#)cds_browser.c\t15\t(DECdns)\t11/20/1991";
#endif	/* neither lint nor saber-C */
/*
*/
/*
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
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#ifdef VMS
/*#pragma nostandard*/
#include <XmP.h>
#include <MainW.h>
#include <RowColumn.h>
#include <CascadeB.h>
#include <PushB.h>
#include <Shell.h>
/*#pragma standard*/
#else
#include <Xm/XmP.h>
#include <Xm/MainW.h>
#include <Xm/RowColumn.h>
#include <Xm/CascadeB.h>
#include <Xm/PushB.h>
#include <X11/Shell.h>
#endif

#include <cds_decwmp.h>
#include <dnsclerk_cds.h>

/*
**  Routine that tells us that we are on an expandable/collapsable node
*/
    extern Boolean SourceIsNodeParent ();


/*
**  Routines to open/close/toggle an expandable entry
*/
    extern void SourceToggleNode ();
    extern void SourceOpenNode   ();
    extern void SourceCloseNode  ();

/*
**  Callback routines inplemented in the source module
*/
    extern XtCallbackRec AttachCB   [2];
    extern XtCallbackRec GetEntryCB [2];


/*
**  Integer maintained by the source module
*/
    extern int  SourceNumEntries;
    extern char filter[30];

                                                                          /* */
/*
**  Forward routine declarations for callbacks
*/
    void Confirmed           ();
    void Extended            ();
    void DisplayChanged      ();
    void LclCreate           ();

    void MenuExpandAll          ();
    void MenuCollapseAll        ();
    void MenuQuit               ();
    void MenuFilterClear        ();
    void MenuFilterRPC          ();
    void MenuFilterWC           ();


/*
**  XtCallbackRec structures for callbacks
*/
    XtCallbackRec ConfirmedCB            [2] = {(XtCallbackProc)Confirmed         ,NULL};
    XtCallbackRec ExtendedCB             [2] = {(XtCallbackProc)Extended          ,NULL};
    XtCallbackRec DisplayChangedCB       [2] = {(XtCallbackProc)DisplayChanged    ,NULL};
    XtCallbackRec MenuExpandAllCB           [2] = {(XtCallbackProc)MenuExpandAll        ,NULL};
    XtCallbackRec MenuCollapseAllCB         [2] = {(XtCallbackProc)MenuCollapseAll ,NULL};
    XtCallbackRec MenuQuitCB                [2] = {(XtCallbackProc)MenuQuit             ,NULL};
    XtCallbackRec MenuFilterClearCB         [2] = {(XtCallbackProc)MenuFilterClear, NULL};
    XtCallbackRec MenuFilterRPCCB           [2] = {(XtCallbackProc)MenuFilterRPC,   NULL};
    XtCallbackRec MenuFilterWCCB            [2] = {(XtCallbackProc)MenuFilterWC, NULL};

/*
**  Program widgets
*/
    Widget Browse;

    Widget toplevel;
    Widget mainwindow;
    Widget menubar;

    Widget filePulldown;
    Widget filePullEntry;
    Widget filtersPulldown;
    Widget filtersPullEntry;

    Widget message_box;
    Widget button;

                                                                          /* */


#ifdef SNI_PRIVATE
#endif 


/*
** Main routine
*/

int main (argc, argv)

  int argc;
  char **argv;

{
/*
**  Arguments for the widgets
*/

    Arg al[10];
    int ac = 0;
    Arg arguments[20];
    Dimension menubar_height;
    Dimension height;
    XmString cs;

    Pixel background_pixel;
    Pixel foreground_pixel;


#ifdef SNI_PRIVATE
#endif

    setlocale (LC_ALL, "");


#ifdef _DECTHREADS_
/*
**  CMA initialization
*/
    cma_init();
#endif

/*
**  Toolkit initialization
*/
    toplevel = XtInitialize ("DCE Browser", "DCE", NULL, 0, &argc, argv);

/*
** Tell the top level widget to allow its children to resize
*/
    XtSetArg (arguments[0], XtNallowShellResize, TRUE);
    XtSetValues (toplevel, arguments, 1);

/*
**  Move window to proper startup position.
*/
    XtSetArg    (arguments[0], XmNx, 30);
    XtSetArg    (arguments[1], XmNy, 120);
    XtSetValues (toplevel, arguments, 2);

/*
**  Create the main window
*/

	XtSetArg(al[ac], XmNx, 0); ac++;
	XtSetArg(al[ac], XmNy, 0); ac++;
	XtSetArg(al[ac], XmNwidth, 512); ac++;
	XtSetArg(al[ac], XmNheight, 0); ac++;
        mainwindow = XmCreateMainWindow(toplevel, "BrowserTest", al, ac);
        XtManageChild (mainwindow);


/*
**  Create the menu bar
*/

    ac = 0;
    XtSetArg(al[ac], XmNentryCallback, NULL); ac++;
    XtSetArg(al[ac], XmNhelpCallback, NULL); ac++;
    menubar = XmCreateMenuBar(mainwindow, "menubar", al, ac);
    XtManageChild (menubar);

/*
**  Create the widget
*/
    LclCreate ();

                                                                          /* */
/**
**  Create the file Pulldown menus.  This is not a managed child...
***/

    ac = 0;
    XtSetArg(al[ac], XmNorientation, XmVERTICAL); ac++;
    filePulldown = XmCreatePulldownMenu(menubar, "", al, ac);

/**
**  Create the Entry
***/
    ac = 0;
    XtSetArg(al[ac], XmNlabelString, XmStringCreate("File",
            XmSTRING_DEFAULT_CHARSET)); ac++;
    XtSetArg(al[ac], XmNsubMenuId, filePulldown); ac++;

    XtManageChild(XmCreateCascadeButton(menubar, "", al, ac));

/*
**  Make pushbuttons inside of the menu.
*/

    ac = 0;
    cs = XmStringCreate("Expand All", XmSTRING_DEFAULT_CHARSET);
    XtSetArg(al[ac], XmNlabelString, cs); ac++;
    XtSetArg(al[ac], XmNactivateCallback, MenuExpandAllCB); ac++;

    filePullEntry = XmCreatePushButton(filePulldown, "", al, ac);
    XtManageChild (filePullEntry);
    XmStringFree (cs);


    ac = 0;
    cs = XmStringCreate("Collapse All", XmSTRING_DEFAULT_CHARSET);
    XtSetArg(al[ac], XmNlabelString, cs); ac++;
    XtSetArg(al[ac], XmNactivateCallback, MenuCollapseAllCB); ac++;

    filePullEntry = XmCreatePushButton(filePulldown, "", al, ac);
    XtManageChild (filePullEntry);
    XmStringFree (cs);

    ac = 0;
    cs = XmStringCreate("Quit", XmSTRING_DEFAULT_CHARSET);
    XtSetArg(al[ac], XmNlabelString, cs); ac++;
    XtSetArg(al[ac], XmNactivateCallback, MenuQuitCB); ac++;

    filePullEntry = XmCreatePushButton(filePulldown, "", al, ac);
    XtManageChild (filePullEntry);
    XmStringFree (cs);

/*
**  Create the filters Pulldown menu.  This is not a managed child...
*/

    ac = 0;
    XtSetArg(al[ac], XmNorientation, XmVERTICAL); ac++;
    filtersPulldown = XmCreatePulldownMenu(menubar, "", al, ac);


/*
**  Create the Entry
*/

    ac = 0;
    XtSetArg(al[ac], XmNlabelString, XmStringCreate("Filters",
                                     XmSTRING_DEFAULT_CHARSET)); ac++;
    XtSetArg(al[ac], XmNsubMenuId, filtersPulldown); ac++;

    XtManageChild(XmCreateCascadeButton(menubar, "", al, ac));

/*
**  Make pushbuttons inside of the menu.
*/

    ac = 0;
    cs = XmStringCreate("CDS_Clearinghouse", XmSTRING_DEFAULT_CHARSET);
    XtSetArg(al[ac], XmNlabelString, cs); ac++;
    XtSetArg(al[ac], XmNactivateCallback, MenuFilterClearCB ); ac++;

    filtersPullEntry= XmCreatePushButton(filtersPulldown, "", al, ac);
    XtManageChild (filtersPullEntry);
    XmStringFree (cs);

    ac = 0;
    cs = XmStringCreate("RPC_Entry", XmSTRING_DEFAULT_CHARSET);
    XtSetArg(al[ac], XmNlabelString, cs); ac++;
    XtSetArg(al[ac], XmNactivateCallback, MenuFilterRPCCB ); ac++;

    filtersPullEntry= XmCreatePushButton(filtersPulldown, "", al, ac);
    XtManageChild (filtersPullEntry);
    XmStringFree (cs);

    ac = 0;
    cs = XmStringCreate("*", XmSTRING_DEFAULT_CHARSET);
    XtSetArg(al[ac], XmNlabelString, cs); ac++;
    XtSetArg(al[ac], XmNactivateCallback, MenuFilterWCCB ); ac++;

    filtersPullEntry= XmCreatePushButton(filtersPulldown, "", al, ac);
    XtManageChild (filtersPullEntry);
    XmStringFree (cs);

/*
**  Recompute the height of the mainwindow to be the height of the menubar
**  plus the height of the widget.
*/
    XtSetArg    (arguments[0], XmNheight, &menubar_height);
    XtGetValues (menubar, arguments, 1);
    XtSetArg    (arguments[0], XmNheight, &height);
    XtGetValues (Browse, arguments, 1);
    XtSetArg    (arguments[0], XmNheight, menubar_height + height + 10);
    XtSetValues (mainwindow, arguments, 1);

/*
**  Set the areas
*/

    XmMainWindowSetAreas(mainwindow,menubar,NULL,NULL,NULL,Browse);

/*
**  Realize the widgets
*/
    XtRealizeWidget (toplevel);


/*
**  Cycle on the events
*/
    XtMainLoop();


/*
**  Never gets here...
*/
    return (0);
}
                                                                          /* */
/*
**  SelectAndConfirm callback routine.  This routine is called when one and
**  only one Entry is selected.  The Entry number selected is provided in the
**  callback structure.
*/

void Confirmed (w, unused_tag, data)

  Widget w;
  int unused_tag;
  CallbackStruct *data;

{

/*
**  Determine if the Entry can be expanded.  If so, then tell the source module
**  to deal with it.
*/
    if (SourceIsNodeParent (data->entry_number, data->entry_tag) == TRUE)
       {
         SourceToggleNode (data->entry_number, data->entry_tag, w);
         ClearSelection (w, data->entry_number);
       }

}

                                                                          /* */
/*
**  ExtendConfirm callback routine.  This only tells us that things were
**  selected.  It does not tell us which ones.  We must make a call to the
**  get selections and get number of selections routines to determine this.
*/

void Extended (w)

  Widget w;

{
/*
**  Local data declarations
*/
    int number_selected;
    int selections [50];
    int Entry_tags [50];
    int i;

/*
**  Ask how many were selected.
*/
    number_selected = GetNumSelections (w);

/*
**  Get those that are selected
*/
    GetSelections (w, selections, NULL, Entry_tags, number_selected);

/*
**  For each Entry, determine if can be expanded.  If so, then tell the
**  source module to deal with it.  Go through the list backwards in order
**  not to throw off the Entry numbers.
*/
    for (i = number_selected;  i > 0;  i--)
        if (SourceIsNodeParent (selections[i-1], Entry_tags[i-1]) == TRUE)
           {
             SourceToggleNode (selections[i-1], Entry_tags[i-1], w);
             ClearSelection (w, selections[i-1]);
           }

}

                                                                          /* */
/*
**  This routine is called whenever the set of displayed entries is changed.
*/

void DisplayChanged (w, unused_tag, unused_data)

  Widget w;
  int unused_tag;
  CallbackStruct *unused_data;

{
/*
**  Local data declarations
*/
    int disp_count;
    int disp_nums [75];
    int disp_ys   [75];
    int i;

/*
**  Ask how many are being displayed.
*/
    disp_count = GetNumDisplayed (w);

/*
**  Max out at 75 hardcoded
*/
    if (disp_count > 75) disp_count = 75;

/*
**  Get those that are displayed.  The null field is for a tag array.
*/
    GetDisplayed (w, disp_nums, NULL, disp_ys, disp_count);

}

                                                                          /* */
void MenuQuit ()

{
/*
**  Destroy the widgets (recursively)
*/
    XtDestroyWidget (toplevel);

/*
**  Exit with successful status code.
*/
    exit (0);
}


                                                                          /* */
void MenuExpandAll ()

{
/*
**  Local data declarations
*/
    int node_number = 1;

/*
**  Disable the widget.  This would not be necessary if this routine was
**  being called in response to a callback.
*/
    DisableDisplay (Browse);

/*
**  Continue to open and traverse until all children are found
*/
    while (node_number <= SourceNumEntries)
       {
         if (SourceIsNodeParent(node_number, 0) == TRUE)
             SourceOpenNode (node_number,0);
         node_number++;
       }

/*
**  Re-enable the Widget.
*/
    EnableDisplay (Browse);

}


                                                                          /* */
void MenuCollapseAll ()

{
/*
**  Local data declarations
*/
    int node_number = 1;

/*
**  Disable the widget.  This would not be necessary if this routine was
**  being called in response to a callback.
*/
    DisableDisplay (Browse);

/*
**  Close node number 1
*/
    SourceCloseNode (1, 0);

/*
**  Re-enable the Widget.
*/
    EnableDisplay (Browse);

}

void MenuFilterRPC ()
{
    strcpy(filter, "RPC_Entry");
}

void MenuFilterClear ()
{
    strcpy(filter, "CDS_Clearinghouse");
}

void MenuFilterWC ()
{
    strcpy(filter, "*");
}


                                                                          /* */
/*
**  Local procedure that creates the widget and sets state information
*/
void LclCreate ()

{
/*
**  Local data declarations
*/
    Arg arguments[20];
    XmString cs;

/*
**  Create the widget
*/
    XtSetArg (arguments[0], XmNwidth,                  300              );
    XtSetArg (arguments[1], XmNheight,                 600              );
    XtSetArg (arguments[2], NselectAndConfirmCallback, ConfirmedCB      );
    XtSetArg (arguments[3], NattachToSourceCallback,   AttachCB         );
    XtSetArg (arguments[4], NgetEntryCallback,         GetEntryCB       );
    XtSetArg (arguments[5], NextendConfirmCallback,    ExtendedCB       );
    XtSetArg (arguments[6], NdisplayChangedCallback,   DisplayChangedCB );

/*
**  Create the widget
*/
    Browse = (Widget) CreateBrowseWidget (mainwindow, "Browse", arguments, 7);

    XtManageChild (Browse);

}
