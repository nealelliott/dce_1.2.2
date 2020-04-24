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
 * $Log: cds.c,v $
 * Revision 1.1.10.2  1996/02/18  19:24:46  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:08:29  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:02:47  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:36:47  root]
 * 
 * Revision 1.1.7.4  1993/08/18  21:18:49  pwang
 * 	A workaround for a NULL pointers ref. bug in motif 1.2 for
 * 	motif 1.2 users. The bug has been fix in motif 1.2.3. The
 * 	workaround is to initialize the base class extention record
 * 	in cdsbrowser code so that the pointers referenced by motif
 * 	macros will ne be NULLs. This workaround is introduced by
 * 	pulsinelli@apollo.hp.com.
 * 	[1993/08/18  21:15:53  pwang]
 * 
 * Revision 1.1.7.3  1993/08/13  20:46:22  pwang
 * 	(1) Added XtNforegound to browser Widget resource, removed
 * 	    misinitialized widget foreground_pixel statement in
 * 	    Initialize() and removed the grey color from the grey_gc
 * 	    to use foreground color instead until a better grey pixmap_bits
 * 	    comes out.
 * 	    [OT#2005]
 * 	(2) Called XCreateFontCursor to initialize the watch_cursor.
 * 	    [OT#7978]
 * 	(3) Changed default font to a known default font.
 * 	[1993/08/13  20:35:16  pwang]
 * 
 * Revision 1.1.7.2  1993/06/10  19:14:59  sommerfeld
 * 	Clean up include paths.
 * 	[1993/06/08  18:05:23  sommerfeld]
 * 
 * Revision 1.1.3.3  1992/12/30  12:37:55  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:21:36  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/29  19:08:01  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:09:42  weisman]
 * 
 * Revision 1.1  1992/01/19  15:23:41  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#if !defined(lint) && !defined(SABER)
static char *SccsID = "@(#)cds.c\t14\t(DECdns)\t12/6/1991";
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

#define CDS

#ifdef VMS
/*#pragma nostandard*/
#include <XmP.h>
#include <MrmPublic.h>
#include <StringDefs.h>
#include <Text.h>
#include <cursorfont.h>
#include <descrip.h>
extern char *VMSDescToNull();
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
#include <dnsclerk_cds.h>
                                                                          /* */
/*
**  Defines for amount of time values (in milliseconds)
*/

#define DELAYDOUBLECLICK  250
#define DELAYDRAGGING     250


/*
**  Define modes
*/

#define MODEIDLE          0
#define MODERANGE         1
#define MODERANGEEXTEND   2
#define MODEDRAG          3
#define MODEAWAITDOUBLE   4
#define MODEAWAITDRAG     5
#define MODEAPPLDRAG      6
                                                                          /* */
/*
**  Forward routine declarations
*/
    static void LclProcessRange    ();
    static void LclManageRange     ();
    static void LclManageDragging  ();
    static void LclDrawRangeBox    ();
    static void LclDrawGhostBox    ();
    static void LclSetGCs          ();
    static void LclTimerTrigger    ();

    static void ClassInitialize	();
    static void Initialize	();
    static void Realize		();
    static void Destroy		();
    static void Resize		();
    static void Redisplay	();
    static Boolean SetValues	();

    Widget CreateBrowseWidget ();

/*
**  Action routine declarations
*/
    void buttonup        ();
    void browsebuttondown();
    void shiftbuttondown ();
    void browsehelp      ();
    void workareafixup   ();
                                                                          /* */
/*
**  Translation Table
*/

static char translations[] =
        "~Shift ~Ctrl ~Meta <Btn1Down>: BROWSEBUTTONDOWN()\n\
          Shift ~Ctrl ~Meta <Btn1Down>: SHIFTBUTTONDOWN()\n\
         ~Shift ~Ctrl ~Meta <Btn1Up>:   BUTTONUP()\n\
          Shift ~Ctrl ~Meta <Btn1Up>:   BUTTONUP()\n";
          /* Help<BtnUp>:                            BROWSEHELP()"; */

/*#pragma nostandard*/
externaldef(parsed_translations) XtTranslations parsed_translations;
/*#pragma standard*/

/*
**  Action Table
*/

static XtActionsRec ActionsList[] = {
    {"REDISPLAY",	    (XtActionProc)Redisplay},
    {"BUTTONUP",	    (XtActionProc)buttonup},
    {"BROWSEBUTTONDOWN",    (XtActionProc)browsebuttondown},
    {"SHIFTBUTTONDOWN",  (XtActionProc)shiftbuttondown},
    {"BROWSEHELP",       (XtActionProc)browsehelp},
    {"WORKAREAFIXUP",    (XtActionProc)workareafixup}
    };
                                                                          /* */
/*
**  Default Widget Resources  -  this is linked into the Class Record
*/

#ifndef XmDefaultFont
#define XmDefaultFont "-*-*-Medium-R-Normal--*-120-*-*-P-*-ISO8859-1"
#endif

static XtResource resources[] = {
   {XmNforeground, XmCForeground, XmRPixel, sizeof(Pixel),
     XtOffset(dns_widget, browse.foreground_pixel), XmRString,
     XtExtdefaultforeground},

   {XmNfont, XmCFontList, XmRFontList, sizeof(XmFontList),
     XtOffset(dns_widget,browse.default_fontlist), XtRString, XmDefaultFont},

   {Nfont, XmCFont, XtRFontStruct, sizeof(XFontStruct*),
     XtOffset(dns_widget,browse.default_font), XtRString, XmDefaultFont},

   {NselectAndConfirmCallback,XtCCallback, XtRCallback, sizeof (XtCallbackList),
     XtOffset (dns_widget,browse.SelectAndConfirm_callback), XtRCallback,
     (caddr_t)NULL},

   {NextendConfirmCallback, XtCCallback, XtRCallback, sizeof (XtCallbackList),
     XtOffset (dns_widget,browse.ExtendConfirm_callback), XtRCallback,
     (caddr_t)NULL},

   {NentrySelectedCallback, XtCCallback, XtRCallback, sizeof (XtCallbackList),
     XtOffset (dns_widget,browse.EntrySelected_callback), XtRCallback, (caddr_t)NULL},

   {NentryUnselectedCallback, XtCCallback, XtRCallback, sizeof (XtCallbackList),
     XtOffset (dns_widget,browse.EntryUnselected_callback), XtRCallback, (caddr_t)NULL},

   {NtransitionsDoneCallback, XtCCallback, XtRCallback, sizeof (XtCallbackList),
     XtOffset (dns_widget,browse.TransitionsDone_callback), XtRCallback, (caddr_t)NULL},

   {NdisplayChangedCallback, XtCCallback, XtRCallback, sizeof (XtCallbackList),
     XtOffset (dns_widget,browse.DisplayChanged_callback), XtRCallback, (caddr_t)NULL},

   {NattachToSourceCallback, XtCCallback, XtRCallback, sizeof (XtCallbackList),
     XtOffset (dns_widget,browse.AttachToSource_callback), XtRCallback, (caddr_t)NULL},

   {NdetachFromSourceCallback, XtCCallback, XtRCallback, sizeof (XtCallbackList),
     XtOffset (dns_widget,browse.DetachFromSource_callback), XtRCallback, (caddr_t)NULL},

   {NselectionsDraggedCallback, XtCCallback, XtRCallback, sizeof (XtCallbackList),
     XtOffset (dns_widget,browse.SelectionsDragged_callback), XtRCallback, (caddr_t)NULL},

   {NgetEntryCallback, XtCCallback, XtRCallback, sizeof (XtCallbackList),
     XtOffset (dns_widget,browse.GetEntry_callback), XtRCallback, (caddr_t)NULL},

   {NdraggingCallback, XtCCallback, XtRCallback, sizeof (XtCallbackList),
     XtOffset (dns_widget,browse.Dragging_callback), XtRCallback, (caddr_t)NULL},

   {NdraggingEndCallback, XtCCallback, XtRCallback, sizeof (XtCallbackList),
     XtOffset (dns_widget,browse.DraggingEnd_callback), XtRCallback, (caddr_t)NULL},

   {NhelpRequestedCallback, XtCCallback, XtRCallback, sizeof (XtCallbackList),
     XtOffset (dns_widget,browse.Help_callback), XtRCallback, (caddr_t)NULL}

};
                                                                          /* */
/*
**  This is the class geometry management routine.
*/

static XtGeometryResult GeometryManager (wi, g)
    Widget wi;
    XtWidgetGeometry *g;

{
/*
**  Fulfill the request from the subwidget
*/
    if (g->request_mode & CWX          ) wi->core.x            = g->x;
    if (g->request_mode & CWY          ) wi->core.y            = g->y;
    if (g->request_mode & CWWidth      ) wi->core.width        = g->width;
    if (g->request_mode & CWHeight     ) wi->core.height       = g->height;
    if (g->request_mode & CWBorderWidth) wi->core.border_width = g->border_width;


/*
**  Return confirmation
*/
    return XtGeometryYes;
}
                                                                          /* */
#if XtSpecificationRelease == 5
/*
 * A workaround for Motif1.2. The Motif1.2 macros that test for subclass
 * try to access the base class extention record's subclass flags, (this
 * wasn't the case before there was an extension record ...)  Sooo....,
 * we add an extension record, initialize the record_type in the Class
 * Initialize routine, and let the XmIs{subclas} (i.e. XmIsGadget, etc.)
 * index into the extension record, PROPERLY! (No more segment violation!!!)
 *
 * Note: This only seems to happen with the compositeClassRec ...
 */
 static XmBaseClassExtRec widgetBaseClassExtRec = {
     NULL,                               /* next_extension       */
     NULLQUARK,                          /* recored_type         */
     XmBaseClassExtVersion,              /* version              */
     sizeof(XmBaseClassExtRec),          /* record_size          */
     NULL,                               /* initializePrehook    */
     NULL,                               /* setValuesPrehook     */
     NULL,                               /* initializePosthook   */
     NULL,                               /* setValuesPosthook    */
     NULL,                               /* secondaryObjectClass */
     NULL,                               /* secondaryCreate      */
     NULL,                               /* getSecResData        */
     { 0 },                              /* fastSubclass flags   */
     NULL,                               /* getValuesPrehook     */
     NULL,                               /* getValuesPosthook    */
     NULL,                               /* classPartInitPrehook */
     NULL,                               /* classPartInitPosthook*/
     NULL,                               /* ext_resources        */
     NULL,                               /* compiled_ext_resources*/
     0,                                  /* num_ext_resources    */
     FALSE,                              /* use_sub_resources    */
     NULL,                               /* widgetNavigable      */
     NULL,                               /* focusChange          */
     NULL                                /* wrapperData          */
};

static XmBaseClassExtRec windowBaseClassExtRec = {
    NULL,                               /* next_extension       */
    NULLQUARK,                          /* recored_type         */
    XmBaseClassExtVersion,              /* version              */
    sizeof(XmBaseClassExtRec),          /* record_size          */
    NULL,                               /* initializePrehook    */
    NULL,                               /* setValuesPrehook     */
    NULL,                               /* initializePosthook   */
    NULL,                               /* setValuesPosthook    */
    NULL,                               /* secondaryObjectClass */
    NULL,                               /* secondaryCreate      */
    NULL,                               /* getSecResData        */
    { 0 },                              /* fastSubclass flags   */
    NULL,                               /* getValuesPrehook     */
    NULL,                               /* getValuesPosthook    */
    NULL,                               /* classPartInitPrehook */
    NULL,                               /* classPartInitPosthook*/
    NULL,                               /* ext_resources        */
    NULL,                               /* compiled_ext_resources*/
    0,                                  /* num_ext_resources    */
    FALSE,                              /* use_sub_resources    */
    NULL,                               /* widgetNavigable      */
    NULL,                               /* focusChange          */
    NULL                                /* wrapperData          */
    };
#endif

/*
**  This is the class record that gets set at compile/link time this is what
**  is passed to the widgetcreate routine as the the class.  All fields must
**  be inited at compile time.
**
**  This definition is for BL8.4 and has been compared against the LISTBOX
*/
/*#pragma nostandard*/
externaldef(widget_classrec)
/*#pragma standard*/
WidClassRec widget_classrec
  = {
      {
         /* CORE CLASS RECORD   */
         /* superclass          */   (WidgetClass) &compositeClassRec,
         /* class_name          */   ClassName,
         /* widget_size         */   sizeof(Rec),
         /* class_initialize    */   ClassInitialize,
         /* class_part_init     */   (XtWidgetClassProc) NULL,
         /* class_inited        */   FALSE,
         /* initialize          */   Initialize,
         /* initialize_hook     */   (XtArgsProc) NULL,
         /* realize             */   Realize,
         /* actions             */   ActionsList,
         /* num_actions         */   XtNumber(ActionsList),
         /* resources           */   resources,
         /* num_resources       */   XtNumber(resources),
         /* xrm_class           */   NULLQUARK,
         /* compress_motion     */   TRUE,
         /* compress_exposure   */   FALSE,
         /* compress_enterleave */   FALSE,
         /* visible_interest    */   FALSE,
         /* destroy             */   Destroy,
         /* resize              */   Resize,
         /* expose              */   Redisplay,
         /* set_values          */   SetValues,
         /* set_values_hook     */   (XtArgsFunc) NULL,
         /* set_values_almost   */   XtInheritSetValuesAlmost,
         /* get_values_hook     */   (XtArgsProc) NULL,
         /* accept_focus        */   (XtAcceptFocusProc) NULL,
         /* version (temp off)  */   XtVersionDontCheck,
         /* callback offsetlst  */   NULL,
         /* tm_table            */   NULL,
         /* query geometry      */   (XtGeometryHandler) NULL,
         /* display accelerator */   (XtStringProc) NULL,
#if XtSpecificationRelease == 5
	  /* extension           */   (XtPointer)&widgetBaseClassExtRec
#else
          /* extension           */   (caddr_t) NULL
#endif

      },
      {
         /* COMP CLASS RECORD   */
         /* childrens geom mgr  */   GeometryManager,
         /* set changed proc    */   XtInheritChangeManaged,
         /* add a child         */   XtInheritInsertChild,
         /* remove a child      */   XtInheritDeleteChild,
         /* extension           */   (caddr_t) NULL,
      },
      {
         /* extension           */   (caddr_t) NULL,
      }
    };
/*#pragma nostandard*/
externaldef(widget_class)
/*#pragma standard*/
WidClass widget_class = &widget_classrec;
                                                                          /* */
/*
**  This is the widget's class initialize routine.
*/

static void ClassInitialize ()
{
#if XtSpecificationRelease == 5
    widgetBaseClassExtRec.record_type = XmQmotif;
#endif
    parsed_translations = XtParseTranslationTable(translations);
}

#if XtSpecificationRelease == 5
static void WindowClassInitialize ()
{
    windowBaseClassExtRec.record_type = XmQmotif;
}
#endif

/*
**  This is the widget's instance initialize routine.  It is called once for
**  each widget.
*/

static void Initialize (unused_req, w)

  dns_widget unused_req, w;

{
/*
**  Routine data declarations.
*/
    CallbackStruct temp;


/*
**  Local data for the watch cursor
*/
    XColor   cursor_colors [2];
    Font     cursor_font;
    int      cursor_wait;
    Display *display;
    Screen  *screen;


/*
**  Force minimum width and height of 75
*/
    if (w->core.width  < 75)  w->core.width  = 75;
    if (w->core.height < 75)  w->core.height = 75;


/*
**  Set up the colors for the watch cursor
*/
    display = XtDisplay (w);
    screen  = DefaultScreenOfDisplay (display);

    cursor_colors[0].pixel = WhitePixelOfScreen (screen);
    cursor_colors[1].pixel = BlackPixelOfScreen (screen);

    XQueryColors (display, DefaultColormapOfScreen(screen), cursor_colors, 2);


/*
**  Create the watch cursor and NULL out the Nav cursor
*/
    w->browse.watch_cursor = XCreateFontCursor (display, XC_watch);

    w->browse.nav_box_cursor = 0;

/*
**  Set fields of the widget.
*/
    w->browse.level0_font              = 0;
    w->browse.level1_font              = 0;
    w->browse.level2_font              = 0;
    w->browse.level3_font              = 0;
    w->browse.level4_font              = 0;
    w->browse.level0_fontlist          = 0;
    w->browse.level1_fontlist          = 0;
    w->browse.level2_fontlist          = 0;
    w->browse.level3_fontlist          = 0;
    w->browse.level4_fontlist          = 0;
    w->browse.margin_width             = 2;
    w->browse.margin_height            = 2;
    w->browse.user_data                = (char *)NULL;
    w->browse.indent_margin            = 16;
    w->browse.fixed_width              = TRUE;
    w->browse.truncate_strings         = FALSE;
    w->browse.num_entries              = 0;
    w->browse.display_mode             = KdisplayOutline;
    w->browse.lhs_percentage           = 50;
    w->browse.multiple_selections      = TRUE;
    w->browse.ghost                    = (Pixmap)NULL;
    w->browse.ghost_basex              = 0;
    w->browse.ghost_basey              = 0;
    w->browse.ghost_width              = 0;
    w->browse.ghost_height             = 0;
    w->browse.default_spacing          = 12;
    w->browse.use_scroll_buttons       = TRUE;
    w->browse.expect_highlighting      = FALSE;
    w->browse.force_seq_get_entry      = FALSE;
    w->browse.show_path_to_root        = TRUE;
    w->browse.level_spacing            = 40;
    w->browse.sibling_spacing          = 5;

    w->browse.arc_width                = 15;
    w->browse.centered_components      = FALSE;
    w->browse.perpendicular_lines      = TRUE;
    w->browse.index_all                = TRUE;
    w->browse.entry_shadows            = TRUE;
    w->browse.tree_entry_outlines      = TRUE;
    w->browse.nav_window_title         = (XmString)NULL;
    w->browse.column_lines             = FALSE;
    w->browse.start_column_component   = 0;
    w->browse.rhs_components_unmapped  = FALSE;
    w->browse.rhs_base_x               = 0;
    w->browse.selection_mode           = KselectEntry;
    w->browse.pane_widget              = (Widget)NULL;
    w->browse.lhs_window_widget        = (Widget)NULL;
    w->browse.rhs_window_widget        = (Widget)NULL;
    w->browse.hscroll                  = (Widget)NULL;

    w->browse.show_selections      =  TRUE;
    w->browse.show_highlighting    =  FALSE;

    w->browse.window_basex         =  0;
    w->browse.max_width            =  0;
    w->browse.rhs_max_width        =  0;
    w->browse.disabled_count       =  0;
    w->browse.entryPtr             =  (EntryPtr) NULL;
    w->browse.num_entries          =  0;
    w->browse.num_selections       =  0;
    w->browse.num_highlighted      =  0;

    w->browse.index_window         =  0;
    w->browse.index_window_needed  =  FALSE;
    w->browse.index_window_shown   =  0;

    w->browse.vscroll              =  (Widget) NULL;
    w->browse.top_button           =  (Widget) NULL;
    w->browse.bot_button           =  (Widget) NULL;

    w->browse.sub_widgets_used     =  FALSE;
    w->browse.num_path             =  0;
    w->browse.display_line_y       =  0;
    w->browse.display_changed      =  FALSE;
    w->browse.display_count        =  0;
    w->browse.entries[0]           =  0;
    w->browse.entries[1]           =  1;

    w->browse.transitions_made     =  FALSE;

    w->browse.cache_pointer        =  (EntryPtr) NULL;
    w->browse.cache_number         =  0;
    w->browse.range_hook           =  0;

    w->browse.clip_count           =  0;


/*
**  Allocate the initial Level arrays
*/
    w->browse.max_level = DEFAULT_MAX_LEVEL;
    w->browse.levelx = (LevelPtr) XtMalloc (sizeof(int) * (DEFAULT_MAX_LEVEL + 1));
    w->browse.levely = (LevelPtr) XtMalloc (sizeof(int) * (DEFAULT_MAX_LEVEL + 1));


/*
**  Initialize scroll bar flags
*/
    w->browse.vscroll_in_progress = FALSE;
    w->browse.hscroll_in_progress = FALSE;


/*
**  Initialize various tree mode values
*/
    w->browse.update_in_progress   =  0;
    w->browse.tree_width	   =  0;
    w->browse.tree_height	   =  0;
    w->browse.current_entry	   =  (EntryPtr) NULL;

    w->browse.button_mode          = MODEIDLE;
    w->browse.timerid              = 0;

    w->browse.button_down          = FALSE;
    w->browse.button_timerid       = 0;
    w->browse.button_waitms        = 470;
    w->browse.button_repeatms      = 30;


/*
**  Initialize Nav Window information
*/
    w->browse.nav_window_popup	    = (Widget) NULL;
    w->browse.nav_window	    = (Widget) NULL;
    w->browse.nav_window_box_width  = 0;
    w->browse.nav_window_box_height = 0;


/*
**  Get the navigation window title information set up.
*/
    if (w->browse.nav_window_title == NULL)
	w->browse.nav_window_title =  XmStringCreate("Navigation Window", XmSTRING_DEFAULT_CHARSET);
    else
        w->browse.nav_window_title = XmStringCopy(w->browse.nav_window_title);


/*
**  Initialize various tree parameters
*/
    if (w->browse.tree_style == KuserDefinedTree) {
	w->browse.tree_connections = FALSE;
	w->browse.mapx = 0;
	w->browse.mapy = 0;
	}
    else {
	w->browse.tree_connections = TRUE;
	}


/*
**  Initialize the graphic contexts.
*/
    LclSetGCs (w);


/*
**  Create the children.  They are created here because they need to use
**  the Graphics Contexts created above.
*/
    DisplayCreate (w);


/*
**  Attach to the source at the resource specified location.
*/
    DisableDisplay (w);
    temp.reason = CRAttachToSource;
    temp.entry_number = 0;
    temp.component_number = 0;
    temp.entry_level = 0;
    XtCallCallbacks (w, NattachToSourceCallback, &temp);
    EnableDisplay (w);
}
                                                                          /* */
/*
**  This routine is called when the widget is being destroyed.  It also is
**  called once per widget instance.  It must ensure that all of the memory
**  allocated by this widget has been released.
*/

static void Destroy (w)

  dns_widget w;

{
/*
**  Routine data declarations
*/
    CallbackStruct temp;


/*
**  Disable (and never re-enable) the widget
*/
    DisableDisplay (w);


/*
**  We must deallocate all of our structures off of the instance record.
*/
    if (w->browse.num_entries != 0)
       StructDeleteEntries (w, 0, w->browse.num_entries);


/*
**  Now we must detach from the source module.
*/
    temp.reason = CRDetachFromSource;
    XtCallCallbacks (w, NdetachFromSourceCallback, &temp);


/*
**  Free all the GCs that were created
*/
    XFreeGC (XtDisplay(w), w->browse.grey_gc      );
    XFreeGC (XtDisplay(w), w->browse.foreground_gc);
    XFreeGC (XtDisplay(w), w->browse.background_gc);
    XFreeGC (XtDisplay(w), w->browse.inverse_gc);


/*
**  Free these Pixmaps and GCs only if allocated
*/
    if (w->browse.drag_gc          ) XFreeGC (XtDisplay(w), w->browse.drag_gc);
    if (w->browse.tree_highlight_gc) XFreeGC (XtDisplay(w), w->browse.tree_highlight_gc);

    if (w->browse.top_pixmap           ) XFreePixmap(XtDisplay(w), w->browse.top_pixmap);
    if (w->browse.bot_pixmap           ) XFreePixmap(XtDisplay(w), w->browse.bot_pixmap);
    if (w->browse.tree_nav_close_pixmap) XFreePixmap(XtDisplay(w), w->browse.tree_nav_close_pixmap);
    if (w->browse.tree_nav_open_pixmap ) XFreePixmap(XtDisplay(w), w->browse.tree_nav_open_pixmap);
    if (w->browse.tree_highlight_pixmap) XFreePixmap(XtDisplay(w), w->browse.tree_highlight_pixmap);
    if (w->browse.outline_nav_pixmap   ) XFreePixmap(XtDisplay(w), w->browse.outline_nav_pixmap);


/*
**  Free any cursors we created
*/
    XFreeCursor(XtDisplay(w),w->browse.watch_cursor);
    if (w->browse.nav_box_cursor) XFreeCursor(XtDisplay(w),w->browse.nav_box_cursor);


/*
**  Free the navigation window title
*/
    if (w->browse.nav_window_title != NULL) XmStringFree (w->browse.nav_window_title);
}
                                                                          /* */
/*
**  This routine is called when the parent's geometry manager resizes the
**  widget.  Make sure that proper flags are turned off.
*/

static void Resize (w)

  dns_widget w;

{
/*
**  Resize the scroll bars only if we have already been through Realize
*/
    if (XtIsRealized(w))
       DisplayResize (w);
}
                                                                          /* */
/*
**  This routine will redisplay the widget.  We will leave it separate in case
**  we get smart about exposing partial windows.  The draw routine will always
**  be used to display the whole thing...
*/

static void Redisplay (wi, event)

  Widget wi;
  XExposeEvent *event;

{
/*
**  Routine data declarations
*/
    XEvent new_event;
    dns_widget w;


/*
**  Go up through the parent chain looking for the class widget.
*/
    w = StructFindWidget (wi);


/*
**  If redisplay, then leave.
*/
    if (wi == (Widget) w) return;


/*
**  For outline mode, ignore all previous clips
*/
    if (   (w->browse.display_mode == KdisplayOutline)
	|| (w->browse.display_mode == KdisplayColumns))
	w->browse.clip_count = 0;


/*
**  Add the clipping rectangle to our array of clipping rectangles
*/
    if (w->browse.clip_count < max_clips)
	{
	w->browse.clips[w->browse.clip_count].x      = event->x;
	w->browse.clips[w->browse.clip_count].y      = event->y;
	w->browse.clips[w->browse.clip_count].width  = event->width;
	w->browse.clips[w->browse.clip_count].height = event->height;
	w->browse.clip_count++;
	}


/*
**  Show what window is associated with these clipping rectangles
*/
    w->browse.clips_window = event->window;


/*
**  Stack up the rest of the Expose events for this window.
*/
    while (XCheckTypedWindowEvent(event->display, event->window, Expose, &new_event ))
       {
	 if (w->browse.clip_count < max_clips)
	     {
	     w->browse.clips[w->browse.clip_count].x      = new_event.xexpose.x;
	     w->browse.clips[w->browse.clip_count].y      = new_event.xexpose.y;
	     w->browse.clips[w->browse.clip_count].width  = new_event.xexpose.width;
	     w->browse.clips[w->browse.clip_count].height = new_event.xexpose.height;
	     w->browse.clip_count++;
	     }
       }


/*
**  Stack up the rest of the GraphicsExpose events
*/
    while (XCheckTypedWindowEvent(event->display, event->window, GraphicsExpose, &new_event ))
       {
	 if (w->browse.clip_count < max_clips)
	     {
	     w->browse.clips[w->browse.clip_count].x      = new_event.xgraphicsexpose.x;
	     w->browse.clips[w->browse.clip_count].y      = new_event.xgraphicsexpose.y;
	     w->browse.clips[w->browse.clip_count].width  = new_event.xgraphicsexpose.width;
	     w->browse.clips[w->browse.clip_count].height = new_event.xgraphicsexpose.height;
	     w->browse.clip_count++;
	     }
       }


/*
**  There aren't anymore, commit to the refresh and then reset things
*/
    if (XtIsRealized(w))

       {
	/*
	**  If there is no more room for clip rectangles, then just draw everything
	**  and return.
	*/
	if (w->browse.clip_count >= max_clips)
	    {
	     /*
	     **  Eat up all further expose events, and clear the existing clipping
	     **  rectangles.
	     */
	     while (XCheckTypedWindowEvent(event->display, event->window, Expose,         &new_event )){}
	     while (XCheckTypedWindowEvent(event->display, event->window, GraphicsExpose, &new_event )){}
	     w->browse.clip_count = 0;
	     w->browse.refresh_all = TRUE;
	    }


         /*
         **  Merge the clip rectangles...
         */
             if (w->browse.clip_count != 0)
		DisplayMergeClips (w);


	 /*
	 **  Call the DisplayDraw procedure.
	 */
             DisableDisplay (w);
             DisplayDraw (w);
             EnableDisplay (w);
       }
}
                                                                          /* */
/*
**  This procedure is called when a configure notify event has been registered
**  on a child window.  It simply ignores the event and calls the Display
**  routine
**
**  There is a second parameter that is passed but not used.  It is an XEvent
**  pointer.
*/

void workareafixup (wi)

  Widget wi;

{
/*
**  Local data declarations
*/
    dns_widget w;


/*
**  Go up through the parent chain looking for the class widget.
*/
    w = StructFindWidget (wi);


/*
**  If the widget passed in is a class, then exposes mean that one of the
**  work areas is not covering the whole area.  In that case, we will call the
**  DisplayWorkAreaFixup procedure to clean it up.
*/
    DisplayWorkAreaFixup (w);
}
                                                                          /* */
/*
** SetValue on the callback is interpreted as replacing all callbacks
**
** This routine has been "exactly" copied from DECWMISC.C
*/

static void LclUpdateCallback (rstruct, w, sstruct, argname)
    dns_widget 		w;
    XtCallbackList	*rstruct, *sstruct;
    char 		* argname;
{
#if 0

[[no longer needed in Motif??]]

/*
**  Local data declarations
*/
    XtCallbackList list;


/*
**  If a new callback has been specified in the scratch widget,
**  remove and deallocate old callback and init new
**/
    if (*rstruct != *sstruct)
       {
        /*
        **  Get the current list
        */
            list = *sstruct;


	/*
	**  Copy the old callback list into the new widget, since
	**  XtRemoveCallbacks needs the "real" widget
    	**/
            *sstruct = *rstruct;
            XtRemoveAllCallbacks(w, argname);
            *sstruct = NULL;
            XtAddCallbacks(w, argname, list);
       }
#endif
}
                                                                          /* */
/*
**  This routine is a subroutine for SetValues to reduce the size of that
**  procedure.  This routine deals with updating all callbacks.
*/

static void SetValuesCallbacks (oldw, neww)

  dns_widget oldw;
  dns_widget neww;

{
#if 0

[[no longer needed in Motif???]]

/*
**  Update each of the callbacks
*/
    LclUpdateCallback (         &(oldw->browse.SelectAndConfirm_callback),
                       neww, &(neww->browse.SelectAndConfirm_callback),
                       NselectAndConfirmCallback);

    LclUpdateCallback (         &(oldw->browse.ExtendConfirm_callback),
                       neww, &(neww->browse.ExtendConfirm_callback),
                       NextendConfirmCallback);

    LclUpdateCallback (         &(oldw->browse.EntrySelected_callback),
                       neww, &(neww->browse.EntrySelected_callback),
                       NentrySelectedCallback);

    LclUpdateCallback (         &(oldw->browse.EntryUnselected_callback),
                       neww, &(neww->browse.EntryUnselected_callback),
                       NentryUnselectedCallback);

    LclUpdateCallback (         &(oldw->browse.TransitionsDone_callback),
                       neww, &(neww->browse.TransitionsDone_callback),
                       NtransitionsDoneCallback);

    LclUpdateCallback (         &(oldw->browse.DisplayChanged_callback),
                       neww, &(neww->browse.DisplayChanged_callback),
                       NdisplayChangedCallback);

    LclUpdateCallback (         &(oldw->browse.AttachToSource_callback),
                       neww, &(neww->browse.AttachToSource_callback),
                       NattachToSourceCallback);

    LclUpdateCallback (         &(oldw->browse.DetachFromSource_callback),
                       neww, &(neww->browse.DetachFromSource_callback),
                       NdetachFromSourceCallback);

    LclUpdateCallback (         &(oldw->browse.SelectionsDragged_callback),
                       neww, &(neww->browse.SelectionsDragged_callback),
                       NselectionsDraggedCallback);

    LclUpdateCallback (         &(oldw->browse.GetEntry_callback),
                       neww, &(neww->browse.GetEntry_callback),
                       NgetEntryCallback);

    LclUpdateCallback (         &(oldw->browse.Dragging_callback),
                       neww, &(neww->browse.Dragging_callback),
                       NdraggingCallback);

    LclUpdateCallback (         &(oldw->browse.DraggingEnd_callback),
                       neww, &(neww->browse.DraggingEnd_callback),
                       NdraggingEndCallback);

    LclUpdateCallback (         &(oldw->browse.Help_callback),
                       neww, &(neww->browse.Help_callback),
                       NhelpRequestedCallback);
#endif
}
                                                                          /* */
/*
**  This routine will take care of any changes that have been made to resource values.
*/

static Boolean SetValues (oldw, unused_req, neww)

  dns_widget oldw;
  Widget unused_req;
  dns_widget neww;

{
/*
**  Routine data declarations
*/
    Boolean redisplay_rhs   = FALSE;    /*  rhs_base_x...                */
    Boolean redisplay_lhs   = FALSE;    /*  indent_margin...             */
    Boolean redisplay_both  = FALSE;    /*  Fonts...                     */
    Boolean redisplay_nav   = FALSE;    /*  layout, lines                */

    Boolean recompute_entry = FALSE;    /*  Things like default spacing  */
    Boolean recompute_tree  = FALSE;    /*  Lines...                     */
    Boolean anchor_tree     = FALSE;


/*
**  Update all of the callbacks
*/
#if 0

[[no longer needed in Motif??]]

    SetValuesCallbacks (oldw, neww);
#endif

/*
**  The following resources are always readonly and cannot be changed
*/
    neww->browse.num_entries       = oldw->browse.num_entries;
    neww->browse.pane_widget       = oldw->browse.pane_widget;
    neww->browse.lhs_window_widget = oldw->browse.lhs_window_widget;
    neww->browse.rhs_window_widget = oldw->browse.rhs_window_widget;
    neww->browse.hscroll           = oldw->browse.hscroll;


/*
**  The following resources cannot be changed after realize time
*/
    if (XtIsRealized(oldw))
       {
          neww->browse.expect_highlighting = oldw->browse.expect_highlighting;
          neww->browse.use_scroll_buttons  = oldw->browse.use_scroll_buttons;
       }


/*
**  If use_scroll_buttons if different, then unmanage only...
*/
    if (neww->browse.use_scroll_buttons == 0)
       if (oldw->browse.use_scroll_buttons)
	  {
            XtUnmanageChild (oldw->browse.top_button);
            XtUnmanageChild (oldw->browse.bot_button);
          }

/*
**  These are resources that change where a refresh of both sides will fix
**  everything up.
*/
    redisplay_both =
	  ((oldw->browse.foreground_pixel  != neww->browse.foreground_pixel )
        || (oldw->core.background_pixel != neww->core.background_pixel)
        || (oldw->browse.default_font      != neww->browse.default_font     )
        || (oldw->browse.level0_font       != neww->browse.level0_font      )
        || (oldw->browse.level1_font       != neww->browse.level1_font      )
        || (oldw->browse.level2_font       != neww->browse.level2_font      )
        || (oldw->browse.level3_font       != neww->browse.level3_font      )
        || (oldw->browse.level4_font       != neww->browse.level4_font      )
        || (oldw->browse.margin_height	   != neww->browse.margin_height    )
        || (oldw->core.sensitive	   != neww->core.sensitive       )
        || (oldw->browse.selection_mode    != neww->browse.selection_mode   ));


/*
**  These are resource changes where a repaint of the left hand side will fix
**  things up.
*/
    redisplay_lhs =
          ((oldw->browse.indent_margin	   != neww->browse.indent_margin    )
        || (oldw->browse.margin_width	   != neww->browse.margin_width     )
        || (oldw->browse.fixed_width	   != neww->browse.fixed_width      ));


/*
**  The following resource changes affect only the drawing of the right side.
*/
    redisplay_rhs =
	  ((oldw->browse.column_lines != neww->browse.column_lines)
	|| (oldw->browse.rhs_base_x   != neww->browse.rhs_base_x  ));


/*
**  The following changes will cause all entries to be recomputed.
*/
    recompute_entry =
          ((oldw->browse.truncate_strings        != neww->browse.truncate_strings       )
	|| (oldw->browse.centered_components	 != neww->browse.centered_components)
	|| (oldw->browse.arc_width		 != neww->browse.arc_width          )
        || (oldw->browse.entry_shadows		 != neww->browse.entry_shadows      )
	|| (oldw->browse.default_spacing         != neww->browse.default_spacing        )
        || (oldw->browse.start_column_component  != neww->browse.start_column_component )
        || (oldw->browse.rhs_components_unmapped != neww->browse.rhs_components_unmapped));


/*
**  If display mode changes, we must layout the tree and recalculate the
**  height of each entry.  We know that changing the display mode automatically
**  destroys all of the entry calculations.
*/
    if (oldw->browse.display_mode != neww->browse.display_mode)
       {
         DisplayChangeMode (oldw, neww);
         recompute_entry = FALSE;
       }


/*
**  The following resource changes will cause trees to be recomputed which also
**  means that the navigation window will be in need.
*/
    recompute_tree =
	  ((recompute_entry)
        || (oldw->browse.tree_entry_outlines != neww->browse.tree_entry_outlines)
	|| (oldw->browse.perpendicular_lines != neww->browse.perpendicular_lines));


/*
**  If they are changing the percentage of the left hand side, then change it if it
**  is currently visible.
*/
    if (oldw->browse.lhs_percentage != neww->browse.lhs_percentage)
       if (neww->browse.display_mode == KdisplayColumns)
	  if (XtIsRealized (oldw))
             {

               /*
               **  The right hand side scroll window is the parent of
               **  the right hand side window widget.
               */
                   Widget rhs_scroll_window = XtParent (oldw->browse.rhs_window_widget);
		   Dimension rhs_width;

	       /*
	       **  Ensure legal values...
	       */
	           if (neww->browse.lhs_percentage <   0) neww->browse.lhs_percentage = 0;
	           if (neww->browse.lhs_percentage > 100) neww->browse.lhs_percentage = 100;

               /*
               **  Compute the right and left hand side widths.
               */
		   rhs_width = XtWidth(neww) - button_height - 3;
		   rhs_width = rhs_width - mullion_width;
		   rhs_width = ((100 - neww->browse.lhs_percentage) * rhs_width) / 100;

               /*
	       **  Make the Pane widget readjust
	       */
	           PaneSetMinMax (rhs_scroll_window, rhs_width, rhs_width);
	           PaneSetMinMax (rhs_scroll_window, 0, 0);
	     }


/*
**  We will redisplay the nav window if we either recompute the tree or redisplay
**  the left hand side.
*/
    redisplay_nav = redisplay_lhs || redisplay_both || recompute_tree;


/*
**  We need to anchor the tree if we are changing tree styles and are going
**  to be displaying in tree mode or if we have already determined to
**  recompute the tree.
*/
    anchor_tree =
	  ((recompute_tree)
	|| (oldw->browse.tree_style != neww->browse.tree_style));


/*
**  Do the anchoring of the tree and set the fact that we need to redraw the
**  left hand side.
*/
/*  if (anchor_tree)
**     if (neww->browse.num_entries > 0)
**        {
**          neww->browse.display_changed = TRUE;
**          TopTreeSetCurrentEntry(oldw);
**          neww->browse.current_entry = oldw->browse.current_entry;
**          neww->browse.prevx = 0;
**          neww->browse.prevy = 0;
**        }
*/

/*
**  User defined trees get no connections
*/
    neww->browse.tree_connections = TRUE;
    if (neww->browse.tree_style == KuserDefinedTree)
       {
         neww->browse.tree_connections = FALSE;
	 neww->browse.clip_count = 0;
       }


/*
**  If entries need recomputed, then do it...
*/
    if (recompute_entry)
       {
        /*
        **  Local data
	*/
	    int i;
            for (i = 1; i <= neww->browse.num_entries; i++)
		{
	         EntryPtr entry = StructGetEntryPtr(neww, i);
	         entry->height_adjusted = FALSE;
	        }
            neww->browse.display_changed = TRUE;
	    redisplay_both = TRUE;
       }


/*
**  If we need to update the nav box, then do it.
*/
    if (redisplay_nav)
       {
	 neww->browse.update_nav_window = TRUE;
	 neww->browse.refresh_all = TRUE;
       }


/*
**  If they changed the nav_window_title, then free the old one and make a
**  copy of the new one...  If the nav window popup has already been
**  created, then issue a set values on him...
*/
    if (neww->browse.nav_window_title != oldw->browse.nav_window_title)
       {
         XmStringFree (oldw->browse.nav_window_title);
         neww->browse.nav_window_title = XmStringCopy(neww->browse.nav_window_title);
         if (neww->browse.nav_window_popup != NULL)
	    {
              Arg arguments[2];
	      XtSetArg (arguments[0], XmNdialogTitle, neww->browse.nav_window_title);
              XtSetValues (neww->browse.nav_window_popup, arguments, 1);
            }
       }


/*
**  If we are to redisplay both, then set the individual values.
*/
    if (redisplay_both)
       {
         redisplay_lhs = TRUE;
         redisplay_rhs = TRUE;
       }


/*
**  Because the pane widget and it's children are entirely occluding the
**  window, we must generate expose events on the two children.  Return false
**  after that since it does no good.
*/
    if (XtIsRealized (oldw))
       {
	if (redisplay_lhs)
	   XClearArea (XtDisplay (neww),
		       XtWindow  (neww->browse.lhs_window_widget),
		       0, 0,
		       XtWidth   (neww->browse.lhs_window_widget),
		       XtHeight  (neww->browse.lhs_window_widget),
		       TRUE);

	if ((redisplay_rhs) && (XtIsManaged (neww->browse.rhs_window_widget)))
	   XClearArea (XtDisplay (neww),
		       XtWindow  (neww->browse.rhs_window_widget),
		       0, 0,
		       XtWidth   (neww->browse.rhs_window_widget),
		       XtHeight  (neww->browse.rhs_window_widget),
		       TRUE);
       }


/*
**  Return false in order not to try a clear area.
*/
    return FALSE;
}
                                                                          /* */
/*
**  This routine will realize the widget.  It is responsible for creating the
**  widget window.
*/

static void Realize (w, valuemask, attributes)

  dns_widget w;
  Mask *valuemask;
  XSetWindowAttributes *attributes;

{
/*
**  Create the X window
*/
    XtCreateWindow (w, InputOutput, CopyFromParent, *valuemask, attributes);


/*
**  Realize all of our children because we are going to resize then to fit the
**  current window layout.
*/
    XtRealizeWidget (w->browse.pane_widget);
    XtRealizeWidget (w->browse.vscroll);
    XtRealizeWidget (w->browse.nav_button);
    if (w->browse.use_scroll_buttons)
       {
         XtRealizeWidget (w->browse.top_button);
         XtRealizeWidget (w->browse.bot_button);
       }


/*
**  Call the DisplayResize procedure since the scroll bars may have been
**  created incorrectly.  The height and width fields of the widget are not
**  stable until this phase.
*/
    DisplayResize (w);
}
                                                                          /* */
/*
**  Here is the universal routine that registers on behalf of applications.
*/

unsigned int InitializeForMRM ()

{
/*
**  Local data declarations
*/
    unsigned int status;


/*
**  Initialize MRM... required , just in case user forgot to do this.
*/
    MrmInitialize ();


/*
**  Register the class
*/

    status = MrmRegisterClass (URMwcUnknown, "dns_widget", "CreateBrowseWidget", CreateBrowseWidget, (WidgetClass)widget_class);


/*
**  Register with all caps if that worked
*/
    if (status == MrmSUCCESS)
       {
         status = MrmRegisterClass (URMwcUnknown, "dns_widget", "CREATEBROWSEWIDGET", CreateBrowseWidget, (WidgetClass)widget_class);
       }


/*
**  Print a message on failure
*/
    if (status != MrmSUCCESS)
       {
 	 printf ("widget MRM registration failed\n");
       }


/*
**  Return the status
*/
    return status ;
}
                                                                          /* */
/*
**  Here is the Widget public entry point using the C language interface.  The
**  callback parameters may need broken out into separate parameters.  The low
**  level interface should be used in BL1A.
*/

Widget BrowseWidget (parent, name, x, y, width, height, confirmcallback,
                helpcallback, attachcallback, detachcallback, getentrycallback,
                extendconfirmcallback, entryselectedcallback,
                entryunselectedcallback, selectionsdraggedcallback,
                draggingcallback, draggingendcallback)

  Widget          parent;
  char           *name;
  int             x, y, width, height;
  XtCallbackList  confirmcallback, helpcallback, attachcallback, detachcallback;
  XtCallbackList  getentrycallback, extendconfirmcallback, entryselectedcallback;
  XtCallbackList  entryunselectedcallback, selectionsdraggedcallback;
  XtCallbackList  draggingcallback, draggingendcallback;

{
/*
**  Routine data declarations
*/
    Arg arglist[20];


/*
**  Pack the supplied arguments for the low level routine.
*/
    XtSetArg (arglist[0],  XmNx,                         x                         );
    XtSetArg (arglist[1],  XmNy,                         y                         );
    XtSetArg (arglist[2],  XmNwidth,                     width                     );
    XtSetArg (arglist[3],  XmNheight,                    height                    );
    XtSetArg (arglist[4],  NselectAndConfirmCallback,  confirmcallback           );
    XtSetArg (arglist[5],  NhelpRequestedCallback,     helpcallback              );
    XtSetArg (arglist[6],  NattachToSourceCallback,    attachcallback            );
    XtSetArg (arglist[7],  NdetachFromSourceCallback,  detachcallback            );
    XtSetArg (arglist[8],  NgetEntryCallback,          getentrycallback          );
    XtSetArg (arglist[9],  NextendConfirmCallback,     extendconfirmcallback     );
    XtSetArg (arglist[10], NentrySelectedCallback,     entryselectedcallback     );
    XtSetArg (arglist[11], NentryUnselectedCallback,   entryunselectedcallback   );
    XtSetArg (arglist[12], NselectionsDraggedCallback, selectionsdraggedcallback );
    XtSetArg (arglist[13], NdraggingCallback,          draggingcallback          );
    XtSetArg (arglist[14], NdraggingEndCallback,       draggingendcallback       );


/*
**  Call the low level interface routine
*/
    return (CreateBrowseWidget (parent, name, arglist, 15));
}
                                                                          /* */
/*
** Here is the public entry point using the C language interface
*/

Widget CreateBrowseWidget (parent, name, arglist, argCount)

  Widget   parent;
  char   * name;
  Arg    * arglist;
  int      argCount;

{
/*
**  The XtCreateWidget routine will indirectly call the class initialize, and
**  the initialize routine.  In addition, it allocates the class and instance
**  records for the widget.
*/
    return (XtCreateWidget (name, widget_class, parent, arglist, argCount));
}
                                                                          /* */
/*
** Here is the DisableDisplay public entry point using the C language interface
*/

void DisableDisplay (w)

  dns_widget w;

{
/*
**  Simply increment the disabled count.
*/
    w->browse.disabled_count = w->browse.disabled_count + 1;
}
                                                                          /* */
/*
** Here is the EnableDisplay public entry point using the C language interface
*/

void EnableDisplay (w)

  dns_widget w;

{
/*
**  Decrement the disabled count.
*/
    w->browse.disabled_count = w->browse.disabled_count - 1;


/*
**  Tell the display module to update the screen if this is the last one.
*/
    if (w->browse.disabled_count == 0)
       DisplayEnableDisplay (w);

}
                                                                          /* */
/*
** Here is the AddEntries public entry point using the C language interface
*/

void AddEntries (w, after_entry, number_of_entries, level, entry_tags, index_window)

  dns_widget w;
  int after_entry, number_of_entries, level;
  unsigned int *entry_tags;
  int index_window;

{

/*
**  Return if they are not adding any...
*/
    if (number_of_entries <= 0) return;


/*
**  Tell the display routine about entries being added.
*/
    DisplayAddEntries (w, after_entry, number_of_entries);


/*
**  Add the entries to the structure.
*/
    StructAddEntries (w, after_entry, number_of_entries, level, entry_tags, index_window);


/*
**  Bump the number of entries in this instance.
*/
    w->browse.num_entries = w->browse.num_entries + number_of_entries;
}
                                                                          /* */
/*
** Here is the DeleteEntries public entry point using the C language interface
*/

void DeleteEntries (w, after_entry, number_of_entries)

  dns_widget w;
  int after_entry, number_of_entries;

{

/*
**  Max out so that we do not delete too many...
*/
    if (w->browse.num_entries < after_entry + number_of_entries)
       number_of_entries = w->browse.num_entries - after_entry;


/*
**  If we have nothing to delete, then leave
*/
    if (number_of_entries <= 0) return;


/*
**  Ensure that all of the entries being deleted are not selected.
*/
    SelectClearSelections (w, after_entry+1, after_entry+number_of_entries);


/*
**  Ensure that all of the entries being deleted are not highlighted.
*/
    SelectClearHighlighting (w, after_entry+1, after_entry+number_of_entries);


/*
**  Tell the display routine about entries being deleted.
*/
    DisplayDeleteEntries (w, after_entry, number_of_entries);


/*
**  Tell the structure module to delete them.
*/
    StructDeleteEntries (w, after_entry, number_of_entries);


/*
**  Decrease the number of entries in this instance.
*/
    w->browse.num_entries = w->browse.num_entries - number_of_entries;


/*
**  If we now have zero entries, then reset the maximum entry width to zero
**  because some applications reuse widgets.
*/
    if (w->browse.num_entries == 0)
       {
        int i;
        w->browse.max_width = 0;
        w->browse.rhs_max_width = 0;
        for (i = 0;  i < max_comps;  i++)
          if (w->browse.column_width_set [i] == FALSE)
             w->browse.column_widths [i] = 0;
       }
}
                                                                          /* */
/*
** Here is the InvalidateEntry public entry point using the C language interface.
*/

void InvalidateEntry (w, entry_number)

  dns_widget w;
  int entry_number;

{

/*
**  Tell STRUCTURE to invalidate the entry
*/
    StructInvalidateEntry (w, entry_number);


/*
**  Tell display about an entry being invalidated.
*/
    DisplayInvalidateEntry (w, entry_number);
}
                                                                          /* */
/*
**  Here is the SetApplDragging public entry point using the C
**  language interface.  It changes the internal widget mode and calls the
**  display routine to do the insensitizing.
*/

void SetApplDragging (w, value)

  dns_widget w;
  int value;

{
/*
**  Tell the display routine about this...
*/
    DisplaySetApplDragging (w, value);


/*
**  Turn us into or out of this mode
*/
    if (value)
         w->browse.button_mode = MODEAPPLDRAG;
    else w->browse.button_mode = MODEIDLE;
}

                                                                          /* */
/*
**  This is the public entry point for getting the left hand side
**  work widget.  It had been created so people can modify the
**  translation table in Motif.
*/
Widget GetLhsWorkWidget (w)

  dns_widget w;

{
/*
**  Simply return the widget ID.
*/
    return(w->browse.lhs_window_widget);
}
/*
**  This is the public entry point for getting the right hand side
**  work widget.  It had been created so people can modify the
**  translation table in Motif.
*/
Widget GetRhsWorkWidget (w)

  dns_widget w;

{
/*
**  Simply return the widget ID.
*/
    return(w->browse.rhs_window_widget);
}

                                                                          /* */
#define inverse_bits  GCFunction   | GCPlaneMask  | GCSubwindowMode | GCFont
#define drag_bits     GCFunction   | GCPlaneMask  | GCSubwindowMode | GCFont
#define nav_box_bits  GCFunction   | GCPlaneMask  | GCLineWidth | GCFont
/*
**  The grey_gc which adds grey to object names and clearinghouse names 
**  to stand for nonexpansible entries,  also makes these names almost 
**  invisible in some display devices. Here we remove the grey from these
**  names to make them visible until a better grey pixmap_bits comes
**  out.
** 
**  This whole mechanism is defined as, 
**  #define grey_gc_bits  GCForeground | GCBackground | GCTile | GCFillStyle 
** 			  | GCLineWidth | GCDashList | GCFont
**  w->browse.grey_pixmap = XtGrayPixmap(XtScreen(w));
**  values.tile           = w->browse.grey_pixmap;
**  w->browse.grey_gc     = XCreateGC (XtDisplay(w),
**		       	               XDefaultRootWindow(XtDisplay(w)),
**				       grey_gc_bits,
**				       &values);
**
*/
#define grey_gc_bits  GCForeground | GCBackground | GCFillStyle | GCLineWidth | GCDashList | GCFont


/*
**  This routine will set all the gc's that are used.  The drawable used
**  to create the GC's are not XtWindow(w) but instead we use the routine
**  XDefaultRootWindow(XtDisplay(w)).  This is because the window does not
**  exist yet because we are in the Initialize phase and not Realize.
*/


typedef struct _PixmapCache {
    Screen *screen;
    Pixmap pixmap;
    struct _PixmapCache *next;
  } CacheEntry;

static unsigned short pixmap_bits[] = {
	0xaaaa, 0xaaaa, 0x5555, 0x5555,
	0xaaaa, 0xaaaa, 0x5555, 0x5555,
	0xaaaa, 0xaaaa, 0x5555, 0x5555,
	0xaaaa, 0xaaaa, 0x5555, 0x5555,
	0xaaaa, 0xaaaa, 0x5555, 0x5555,
	0xaaaa, 0xaaaa, 0x5555, 0x5555,
	0xaaaa, 0xaaaa, 0x5555, 0x5555,
	0xaaaa, 0xaaaa, 0x5555, 0x5555,
	0xaaaa, 0xaaaa, 0x5555, 0x5555,
	0xaaaa, 0xaaaa, 0x5555, 0x5555,
	0xaaaa, 0xaaaa, 0x5555, 0x5555,
	0xaaaa, 0xaaaa, 0x5555, 0x5555,
	0xaaaa, 0xaaaa, 0x5555, 0x5555,
	0xaaaa, 0xaaaa, 0x5555, 0x5555,
	0xaaaa, 0xaaaa, 0x5555, 0x5555,
	0xaaaa, 0xaaaa, 0x5555, 0x5555
    };

static CacheEntry *pixmapCache = NULL;

/* Kept for backwards compatibility */

Pixmap XtGrayPixmap( screen )
    Screen *screen;
/*
 *	Creates a gray pixmap of depth DefaultDepth(screen)
 *	caches these so that multiple requests share the pixmap
 */
{
    Display *display = DisplayOfScreen(screen);
    XImage image;
    CacheEntry *cachePtr;
    Pixmap gray_pixmap;
    GC gc;
    XGCValues gcValues;

/*
 *	Creates a gray pixmap of depth DefaultDepth(screen)
 *	caches these so that multiple requests share the pixmap
 */

#define pixmap_width 32
#define pixmap_height 32

    /* see if we already have a pixmap suitable for this screen */
    for (cachePtr = pixmapCache; cachePtr; cachePtr = cachePtr->next) {
	if (cachePtr->screen == screen)
	    return( cachePtr->pixmap );
    }

    /* nope, we'll have to construct one now */
    image.height = pixmap_height;
    image.width = pixmap_width;
    image.xoffset = 0;
    image.format = XYBitmap;
    image.data = (char*) pixmap_bits;
    image.byte_order = ImageByteOrder(display);
    image.bitmap_pad = BitmapPad(display);
    image.bitmap_bit_order = BitmapBitOrder(display);
    image.bitmap_unit = BitmapUnit(display);
    image.depth = 1;
    image.bytes_per_line = pixmap_width/8;
    image.obdata = NULL;

    gray_pixmap = XCreatePixmap( display, RootWindowOfScreen(screen),
				 (unsigned) image.width,
				 (unsigned) image.height,
				 (unsigned) DefaultDepthOfScreen(screen) );

    /* and insert it at the head of the cache */
    cachePtr = XtNew(CacheEntry);
    cachePtr->screen = screen;
    cachePtr->pixmap = gray_pixmap;
    cachePtr->next = pixmapCache;
    pixmapCache = cachePtr;

    /* now store the image into it */
    gcValues.foreground = BlackPixelOfScreen(screen);
    gcValues.background = WhitePixelOfScreen(screen);
    gcValues.fill_style = FillTiled;
    gcValues.tile = gray_pixmap;
    gc = XCreateGC( display, RootWindowOfScreen(screen),
		    GCForeground | GCBackground | GCFillStyle | GCTile,
		    &gcValues );

    XPutImage( display, gray_pixmap, gc, &image, 0, 0, 0, 0,
	       (unsigned) image.width, (unsigned) image.height);

    XFreeGC( display, gc );

    return( gray_pixmap );
}

static void LclSetGCs (w)

  dns_widget w;

{
/*
**  Routine data declarations
*/
    XGCValues values;


/*
**  Initialize uncreated ones
*/
    w->browse.drag_gc = NULL;


/*
**  Get the grey pixmap from the Xt routine
*/
    w->browse.grey_pixmap = XtGrayPixmap(XtScreen(w));
/*
**  Set the common values
*/
    values.function       = GXinvert;
    values.fill_style     = FillTiled;
    values.plane_mask     = w->browse.foreground_pixel ^ w->core.background_pixel;
    values.foreground     = w->browse.foreground_pixel;
    values.background     = w->core.background_pixel;
    values.tile           = w->browse.grey_pixmap;
#if !defined(SNI_SVR4)
    values.font		  = w->browse.default_font->fid;
#else
    {
	XFontStruct *fontstruct;
        fontstruct                      =       XLoadQueryFont( XtDisplay( w), "9x15" );
        values.font                     =       fontstruct->fid;
    }
#endif
    values.dashes         = 1;
    values.line_width     = 0;
    values.subwindow_mode = IncludeInferiors;


/*
**  Create the inverted, grey, and foreground
*/
    w->browse.inverse_gc    = XCreateGC (XtDisplay(w),
					 XDefaultRootWindow(XtDisplay(w)),
					 inverse_bits,
					 &values);

    w->browse.grey_gc       = XCreateGC (XtDisplay(w),
				         XDefaultRootWindow(XtDisplay(w)),
					 grey_gc_bits,
					 &values);

    w->browse.foreground_gc = XCreateGC (XtDisplay(w),
					 XDefaultRootWindow(XtDisplay(w)),
					 GCForeground | GCBackground | GCFont,
					 &values);


/*
**  Create the highlighting GC if highlighting or shadows are enabled
*/
    if (w->browse.expect_highlighting || w->browse.entry_shadows)
	{
	values.line_width = highlight_width;
	w->browse.tree_highlight_gc = XCreateGC (XtDisplay(w),
					         XDefaultRootWindow(XtDisplay(w)),
	                                         GCForeground | GCBackground | GCLineWidth | GCFont,
                                                 &values);
	}


/*
**  Create a special GC for the nav box using the border color if it is
**  different than the foreground color.  Otherwise just use the foregound.
**  Use a 2 pixel wide line.
*/
    values.line_width = 2;
    if (w->core.border_pixel != w->core.background_pixel)
	values.plane_mask = w->core.border_pixel ^ w->core.background_pixel;
    else
	values.plane_mask = w->browse.foreground_pixel ^ w->core.background_pixel;
    w->browse.nav_box_gc    = XCreateGC (XtDisplay(w),
                                         XDefaultRootWindow(XtDisplay(w)),
                                         nav_box_bits,
                                         &values);

/*
**  Create the background
*/
    values.foreground = w->core.background_pixel;
    values.background = w->browse.foreground_pixel;
    w->browse.background_gc = XCreateGC (XtDisplay(w),
                                         XDefaultRootWindow(XtDisplay(w)),
                                         GCForeground | GCBackground | GCFont,
                                         &values);
}
                                                                          /* */
/*
**  This routine draws the box used in range selection.  The box_x and box_y
**  values are assumed to be the anchor points.  The values passed are
**  assumed to be the new opposite corner.
**
**  This routine does not draw zero width or zero height boxes in order to
**  get rid of little dots on a VS2000.
*/

static void LclDrawRangeBox (w, x, y)

  dns_widget w;
  int x, y;

{
/*
**  Local variables to deal with not drawing above the path to the root line
**  in the outline mode.  Note that we need only the Y coordinates.
*/
    Position box_other_y, box_base_y;


/*
**  Save the new corner
*/
    w->browse.box_other_x = x;
    w->browse.box_other_y = y;


/*
**  Save the rectangle corners locally
*/
    box_base_y  = w->browse.box_base_y;
    box_other_y = w->browse.box_other_y;


/*
**  If this is outline mode, then make sure that the y coordinates are at least
**  greater than or equal to the path to the root line.
*/
    if (   (w->browse.display_mode == KdisplayOutline)
        || (w->browse.display_mode == KdisplayColumns))
       {
        if (box_base_y  <= w->browse.display_line_y) box_base_y  = w->browse.display_line_y + 1;
        if (box_other_y <= w->browse.display_line_y) box_other_y = w->browse.display_line_y + 1;
       }


/*
**  If the width or the height of the box is zero, then return
*/
    if (w->browse.box_base_x == w->browse.box_other_x) return;
    if (w->browse.box_base_y == w->browse.box_other_y) return;


/*
**  Draw the new rectangle...
*/
    if (w->browse.box_base_x < w->browse.box_other_x)

         if (box_base_y < box_other_y)

              XDrawRectangle (XtDisplay(w), XtWindow(w), w->browse.inverse_gc,
                              (Position)w->browse.box_base_x,
			      (Position)box_base_y,
                              (Dimension)w->browse.box_other_x - w->browse.box_base_x,
                              (Dimension)box_other_y - box_base_y);

         else XDrawRectangle (XtDisplay(w), XtWindow(w), w->browse.inverse_gc,
                              (Position)w->browse.box_base_x,
			      (Position)box_other_y,
                              (Dimension)w->browse.box_other_x - w->browse.box_base_x,
                              (Dimension)box_base_y - box_other_y);

    else if (box_base_y < box_other_y)

              XDrawRectangle (XtDisplay(w), XtWindow(w), w->browse.inverse_gc,
                              (Position)w->browse.box_other_x,
			      (Position)box_base_y,
                              (Dimension)w->browse.box_base_x - w->browse.box_other_x,
                              (Dimension)box_other_y - box_base_y);

         else XDrawRectangle (XtDisplay(w), XtWindow(w), w->browse.inverse_gc,
                              (Position)w->browse.box_other_x,
			      (Position)box_other_y,
                              (Dimension)w->browse.box_base_x - w->browse.box_other_x,
                              (Dimension)box_base_y - box_other_y);
}
                                                                          /* */
/*
**  This routine draws the box used in dragging.  The x and y positions are
**  assumed to be the base x and base y positions.
*/

static void LclDrawGhostBox (w, x, y)

  dns_widget w;
  int x, y;

{
/*
**  Save the x and y positions
*/
    w->browse.ghost_x = x;
    w->browse.ghost_y = y;


/*
**  Create the drag GC if we haven't yet
*/
    if (w->browse.drag_gc == NULL)
       {
         XGCValues values;
         values.subwindow_mode = IncludeInferiors;
         values.function   = GXxor;
#if !defined(SNI_SVR4)
	 values.font = w->browse.default_font->fid;
#else
         {
         XFontStruct *fontstruct;
         fontstruct = XLoadQueryFont( XtDisplay( w), "9x15" );
         values.font = fontstruct->fid;
         }
#endif
         values.plane_mask = w->browse.foreground_pixel ^ w->core.background_pixel;
         w->browse.drag_gc = XCreateGC (XtDisplay(w), XtWindow(w), drag_bits, &values);
       }


/*
**  Draw the ghost using the drag graphics context...
*/
    XCopyArea (XtDisplay(w), w->browse.ghost, XtWindow(w),
	       w->browse.drag_gc,
	       (Position)0,
	       (Position)0,
               (Dimension)w->browse.ghost_width,
               (Dimension)w->browse.ghost_height,
               (Position)w->browse.ghost_x - w->browse.ghost_basex,
               (Position)w->browse.ghost_y - w->browse.ghost_basey);
}
                                                                          /* */
/*
**  This routine is called when the user releases MB3.  This is help.
*/

void browsehelp (wi, event)

  Widget wi;
  XButtonEvent *event;

{
/*
**  Local data declarations
*/
    dns_widget w;
    CallbackStruct temp;

    w = StructFindWidget (wi);


/*
**  Get the entry that we are positioned on
*/
    MapPosition (w, event->x, event->y, &temp.entry_number, &temp.component_number, &temp.entry_tag);


/*
**  Pass this directly in the help callback.
*/
    temp.reason      = CRHelpRequested;
    temp.time        = event->time;
    temp.entry_level = w->browse.map_level;

    XtCallCallbacks (w, NhelpRequestedCallback, &temp);
}
                                                                          /* */
/*
**  This routine is called when the user presses MB1.  This routine makes one
**  assumption that may not always be perfectly valid.  This assumption is
**  that if the doubleclick timer is set, then we own the selections.
*/

void browsebuttondown (wi, event)

  Widget wi;
  XButtonEvent *event;

{
/*
**  Local data declarations
*/
    dns_widget w;
    CallbackStruct temp;
    EntryPtr entry;

    w = StructFindWidget (wi);

/*
**  If we are in application dragging mode, then ignore the button down.  I
**  really don't know what else to do in this situation.
*/
    if (w->browse.button_mode == MODEAPPLDRAG) return;


/*
**  See if the timer would have fired...  Set the mode idle if we waited too
**  long.
*/
    if (w->browse.button_mode == MODEAWAITDOUBLE)
       if (event->time > w->browse.button_up_event_time + DELAYDOUBLECLICK)
          w->browse.button_mode = MODEIDLE;


/*
**  Lose the timestamp
*/
    w->browse.button_up_event_time = (Time) 0;


/*
**  Get the entry that we are positioned on
*/
    MapPosition (w, event->x, event->y, &temp.entry_number, &temp.component_number, &temp.entry_tag);


/*
**  If we are positioned in whitespace and multiple selections are allowed, then set
**  the mode to range selection and leave.
*/
    if (temp.entry_number == 0)
       {
         if (w->browse.multiple_selections == FALSE) return;
         if (event->y <= w->browse.display_line_y) return;
         w->browse.box_base_x = event->x;   w->browse.box_other_x = event->x;
         w->browse.box_base_y = event->y;   w->browse.box_other_y = event->y;
         w->browse.button_mode = MODERANGE;
         LclManageRange (w);
         return;
       }


/*
**  Double click
*/
    if (   (w->browse.button_mode     == MODEAWAITDOUBLE  )
        && (w->browse.timer_entry     == temp.entry_number))
       {
         w->browse.button_mode = MODEIDLE;
         temp.time = event->time;

         if (w->browse.num_selections == 1) temp.reason = CRSelectAndConfirm;
         else temp.reason = CRExtendConfirm;

         DisableDisplay (w);
	 DisplaySetWatch (w, TRUE);

         temp.entry_level = w->browse.map_level;

         if (w->browse.num_selections == 1)
            XtCallCallbacks (w, NselectAndConfirmCallback, &temp);
         else if (w->browse.ExtendConfirm_callback != (XtCallbackList) NULL)
                 XtCallCallbacks (w, NextendConfirmCallback, &temp);
              else {
                    /*
                    **  We have multiple selections but no ExtendConfirm callback...
                    **  Go backwards through the list calling the single select
		    **  and confirm for each one.
                    */
                        int num_selected = w->browse.num_selections;
                        int * numbers;
			int * tags;
                        numbers = (int *) XtMalloc (num_selected * sizeof(int));
                        tags    = (int *) XtMalloc (num_selected * sizeof(int));
		        temp.reason = CRSelectAndConfirm;
			GetSelections (w, numbers, NULL, tags, num_selected);
			for (; num_selected > 0;  num_selected--)
                            {
                              temp.entry_number = numbers [num_selected - 1];
                              temp.entry_tag    = tags    [num_selected - 1];
                              XtCallCallbacks (w, NselectAndConfirmCallback, &temp);
                            }
		        XtFree (numbers);
		        XtFree (tags);
                   }


	 DisplaySetWatch (w, FALSE);
         EnableDisplay (w);
         return;
       }


/*
**  The mode is now idle
*/
    w->browse.button_mode = MODEIDLE;


/*
**  Get the entry pointer for this entry.
*/
    entry = StructGetEntryPtr (w, temp.entry_number);


/*
**  If the entry is insensitive, then ignore it.
*/
    if (entry->grayed) return;


/*
**  If we are not showing selections, then unselect all of the entries
*/
    if (w->browse.show_selections == FALSE)
       SelectClearSelections (w, 1, w->browse.num_entries);


/*
**  Select this one and unselect the rest.
*/
    SelectSelectSet (w, temp.entry_number, temp.entry_number, temp.component_number, event->time);


/*
**  Tell the caller that we are done with transitioning selections
*/
    SelectReportTransitions (w);


/*
**  Set the mode for dragging...
*/
    if (w->browse.SelectionsDragged_callback != (XtCallbackList) NULL)
       w->browse.button_mode = MODEAWAITDRAG;

    if (w->browse.Dragging_callback != (XtCallbackList) NULL)
       w->browse.button_mode = MODEAWAITDRAG;


/*
**  Set the timer if necessary
*/
   if (w->browse.button_mode == MODEAWAITDRAG)
      {
        w->browse.timerid = XtAppAddTimeOut(XtWidgetToApplicationContext(w), DELAYDRAGGING, LclTimerTrigger, w);
        w->browse.timer_entry = temp.entry_number;
      }
}
                                                                          /* */
/*
**  This routine is called when the user presses the mouse button while holding
**  the Shift key.  This accomplishes a TOGGLE selection.  Note that there is
**  no longer a Double click shifted...
*/

void shiftbuttondown (wi, event)

  Widget wi;
  XButtonEvent *event;

{
/*
**  Local data declarations
*/
    dns_widget w;
    EntryPtr entry;
    int entry_number, component_number;
    unsigned int entry_tag;

    w = StructFindWidget (wi);


/*
**  If we are in application dragging mode, then ignore the button down.  I
**  really don't know what else to do in this situation.
*/
    if (w->browse.button_mode == MODEAPPLDRAG) return;


/*
**  If we are hiding the selections, then treat this as a non-shift MB1
*/
    if (w->browse.show_selections == FALSE)
       {
         browsebuttondown (w, event);
         return;
       }


/*
**  Get the entry that we are positioned on
*/
    MapPosition (w, event->x, event->y, &entry_number, &component_number, &entry_tag);


/*
**  Double clicks are handled by the other routine.  Only call it if there is
**  at least one entry selected and it will pass the double click testing in
**  that other routine.  If the shift double clicked happens on an entry that
**  is initially selected, then it will not be part of the set.
**
**  If the timer expired, then forget it.
*/
    if (w->browse.button_mode == MODEAWAITDOUBLE)
       if (event->time > w->browse.button_up_event_time + DELAYDOUBLECLICK)
          w->browse.button_mode = MODEIDLE;


/*
**  If we are still checking double click and we are still on the same entry,
**  then pass control to the other routine...  Ensure that at least one entry
**  is currently selected...
*/
    if (w->browse.button_mode == MODEAWAITDOUBLE)
       if (w->browse.timer_entry == entry_number)
          if (w->browse.num_selections > 0)
             {
               browsebuttondown (w, event);
               return;
             }


/*
**  Lose the timer and the mode if still awaiting double clicks
*/
    if (w->browse.button_mode == MODEAWAITDOUBLE)
       {
         w->browse.button_mode = MODEIDLE;
         w->browse.button_up_event_time = (Time) 0;
       }


/*
**  If we are positioned in whitespace, then set the mode to range selection and leave.
*/
    if (entry_number == 0)
       {
         if (w->browse.multiple_selections == FALSE) return;
         if (event->y <= w->browse.display_line_y) return;
         w->browse.box_base_x = event->x;   w->browse.box_other_x = event->x;
         w->browse.box_base_y = event->y;   w->browse.box_other_y = event->y;
         w->browse.button_mode = MODERANGEEXTEND;
         LclManageRange (w);
         return;
       }


/*
**  Get the entry pointer for this entry.
*/
    entry = StructGetEntryPtr (w, entry_number);


/*
**  If the entry is insensitive, then ignore it.
*/
    if (entry->grayed) return;


/*
**  If multiple selections are not allowed, there is already a selection made,
**  AND it is not this entry, then treat this as a normal MB1 down.
*/
    if ((w->browse.multiple_selections == FALSE) && (w->browse.num_selections == 1) && (!entry->selected))
       {
         w->browse.button_mode = MODEIDLE;
         browsebuttondown (w, event);
         return;
       }


/*
**  Toggle the selected state...
*/
    SelectToggleSelections (w, entry_number, entry_number, component_number, event->time);


/*
**  Tell the caller that all transitions are completed.
*/
    SelectReportTransitions (w);


/*
**  Set the mode
*/
    w->browse.button_mode = MODEIDLE;


/*
**  If the entry is selected we will se if dragging should be turned on
*/
    if (entry->selected)
       {
         if (w->browse.SelectionsDragged_callback != (XtCallbackList) NULL)
            w->browse.button_mode = MODEAWAITDRAG;

         if (w->browse.Dragging_callback != (XtCallbackList) NULL)
            w->browse.button_mode = MODEAWAITDRAG;
       }


/*
**  Set the timer if necessary
*/
   if (w->browse.button_mode == MODEAWAITDRAG)
      {
        w->browse.timerid = XtAppAddTimeOut(XtWidgetToApplicationContext(w), DELAYDRAGGING, LclTimerTrigger, w);
        w->browse.timer_entry = entry_number;
      }
}
                                                                          /* */
/*
**  This routine is called when the mouse button is released.
*/

void buttonup (wi, event)

  Widget wi;
  XButtonEvent *event;

{
/*
**  Local data declarations
*/
    dns_widget w;
    CallbackStruct temp;
    EntryPtr entry;

    w = StructFindWidget (wi);


/*
**  Cancel the drag timer if it is still set.
*/
    if (w->browse.timerid != 0)
       {
         XtRemoveTimeOut (w->browse.timerid);
         w->browse.timerid = 0;
       }


/*
**  Get the entry that we were left on.  We will set the double click timer
**  only if we were left on a selected entry.
*/
    MapPosition (w, event->x, event->y, &temp.entry_number, &temp.component_number, &temp.entry_tag);


/*
**  If we are in application dragging mode, then notify them that we have
**  completed this dragging in this window.
*/
    if (w->browse.button_mode == MODEAPPLDRAG)
       {
         temp.reason = CRDraggingEnd;
         temp.time   = event->time;
         temp.x      = event->x;
         temp.y      = event->y;

         DisableDisplay (w);
         XtCallCallbacks (w, NdraggingEndCallback, &temp);
         EnableDisplay (w);
         return;
       }


/*
**  Default the mode to being idle.
*/
    w->browse.button_mode = MODEIDLE;


/*
**  If we were on whitespace, then we are done.
*/
    if (temp.entry_number == 0) return;


/*
**  Get the structure
*/
    entry = StructGetEntryPtr (w, temp.entry_number);


/*
**  If the entry is selected, then set the timer
*/
    if (entry->selected)
       {
         w->browse.button_mode = MODEAWAITDOUBLE;
         w->browse.button_up_event_time = event->time;
         w->browse.timer_entry = temp.entry_number;
       }
}
                                                                          /* */
/*
**  This routine is called after the range selection rectangle is completed.
*/

static void LclProcessRange (w, time, x, y)

  dns_widget w;
  Time time;
  int x, y;

{
/*
**  Local data declarations
*/
    int low_entry_number, high_entry_number;
    int range_flag;
    int entry_count;
    LevelPtr entry_array;


/*
**  If the cursor was released in an auto-scroll area, then we will cancel the
**  range selection as if the user had pressed MB2 before releasing MB1.
*/
    if (AutoScrollCheck(w, x, y) != 0) return;


/*
**  Get the items in the range
*/
    DisplayGetRangeEntries (w, x, y, &range_flag, &entry_count, &entry_array);


/*
**  If the GetRangeEntries returns a range things are simpler.
*/
    if (range_flag == TRUE) {
	/*
	**  Get the first and last entries that the box intersects, and
	**  free the array.
	*/
	    if (entry_count != 2) return;
	    low_entry_number  = (*entry_array)[0];
	    high_entry_number = (*entry_array)[1];
	    XtFree(entry_array);

	/*
	**  If it came back as zero, then no entries intersect the box and we won't select
	**  anything...
	*/
	    if (low_entry_number == 0) return;


	/*
	**  If we are not showing selections, then unselect all of the entries
	*/
	    if (w->browse.show_selections == FALSE)
	       SelectClearSelections (w, 1, w->browse.num_entries);


	/*
	**  If we were in MODERANGE we must change the set of selected entries to be
	**  from the lowest entry to the highest entry.
	*/
	    if (w->browse.button_mode == MODERANGE)
	       SelectSelectSet (w, low_entry_number, high_entry_number, 0, time);


	/*
	**  Select all entries by clearing and then toggling there values
	*/
	    if (w->browse.button_mode == MODERANGEEXTEND)
	       SelectToggleSelections (w,low_entry_number,high_entry_number,0,time);
	}



/*
**  If we have a list of entries handle each one seperately
*/
    else {
	int i;	/* loop index into array of entries */

	if (entry_count < 1) return;

	/*
	**  If we are not showing selections, then unselect all of the entries
	*/
	    if (w->browse.show_selections == FALSE)
	       SelectClearSelections (w, 1, w->browse.num_entries);


	/*
	**  If we were in MODERANGE we must change the set of selected entries to be
	**  from the lowest entry to the highest entry.
	*/
	    if (w->browse.button_mode == MODERANGE) {
		ClearSelections(w);
		for (i = 0; i < entry_count; i++)
		   SelectEntry (w, (*entry_array)[i]);
		}

	/*
	**  Select all entries by clearing and then toggling there values
	*/
	    if (w->browse.button_mode == MODERANGEEXTEND)
	       for (i = 0; i < entry_count; i++)
		   SelectEntry (w, (*entry_array)[i]);

	    XtFree(entry_array);
	}


/*
**  Tell the caller that all transitions are completed.
*/
    SelectReportTransitions (w);


/*
**  Map the point in which the mouse was released for double clicking if we
**  were in non-shift range selection mode.
*/
    if (w->browse.button_mode == MODERANGE)
       {
         /*
         **  Local data declarations
         */
             EntryPtr entry = (EntryPtr) NULL;
             int entry_number;


         /*
         **  Get the entry that we were left on.
         */
             MapPosition (w, x, y, &entry_number, (int *)NULL, (unsigned int *)NULL);


         /*
         **  If we were not on whitespace, then continue...
         */
             if (entry_number != 0)
                entry = StructGetEntryPtr (w, entry_number);


         /*
         **  If we have a selected entry, then set the timer
         */
             if ((entry != (EntryPtr) NULL) && (entry->selected))
                {
                  w->browse.button_mode = MODEAWAITDOUBLE;
                  w->browse.button_up_event_time = time;
                  w->browse.timer_entry = entry_number;
                }
       }


/*
**  Set idle if not double clicking...
*/
    if (w->browse.button_mode != MODEAWAITDOUBLE)
       {
         w->browse.button_mode = MODEIDLE;
         w->browse.button_up_event_time = (Time) 0;
       }
}
                                                                          /* */
/*
**  This routine is called when the mode has been changed to MODERANGE or
**  MODERANGEEXTEND.
*/

static void LclManageRange (w)

  dns_widget w;

{
/*
**  Local data declarations
*/
    Window root_return, child_return;
    int root_x_return, root_y_return, win_x_return, win_y_return, status;
    unsigned int state_mask_return;


/*
**  Do a while forever loop until the rectangle is finished or until it is
**  canceled.
*/
    while (TRUE)
      {
        /*
        **  Find out the current state of the pointer and mouse buttons.
        */
            status = XQueryPointer (XtDisplay(w), XtWindow(w),
                                    &root_return, &child_return,
                                    &root_x_return, &root_y_return,
                                    &win_x_return, &win_y_return,
                                    &state_mask_return);

        /*
        **  If the return status is bad or one of the other buttons is pressed,
        **  then cancel this range selection.
        */
            if ((status == 0) || (state_mask_return & Button2Mask) || (state_mask_return & Button3Mask))
               {
                 LclDrawRangeBox (w, w->browse.box_other_x, w->browse.box_other_y);
                 break;
               }


        /*
        **  If the button has been released, then process the selection
        */
            if ((state_mask_return & Button1Mask) == FALSE)
               {
                 LclDrawRangeBox (w, w->browse.box_other_x, w->browse.box_other_y);
                 LclProcessRange (w, CurrentTime, win_x_return, win_y_return);
                 break;
               }


        /*
        **  If the range hook is not set, then set it...
        */
            if (w->browse.range_hook == 0)
		DisplaySetRangeHook (w, win_x_return, win_y_return);


        /*
        **  See if the screen needs autoscrolled.
        */
            status = AutoScrollCheck (w, win_x_return, win_y_return);


        /*
        **  No autoscroll.  Repaint if X or Y has changed.
        */
            if (status == 0)
               if ((w->browse.box_other_x != win_x_return) || (w->browse.box_other_y != win_y_return))
                  {
                    LclDrawRangeBox (w, w->browse.box_other_x, w->browse.box_other_y);
                    LclDrawRangeBox (w, win_x_return, win_y_return);
                  }


        /*
        **  Auto scroll needs performed
        */
            if (status != 0)
               {
                 /*
                 **  Erase the old box
                 */
                     LclDrawRangeBox (w, w->browse.box_other_x, w->browse.box_other_y);


                 /*
                 **  Auto scroll the screen.
                 */
                     AutoScrollDisplay (w, win_x_return, win_y_return);


                 /*
                 **  Redraw the box
                 */
                     LclDrawRangeBox (w, win_x_return, win_y_return);
               }
      }


/*
**  Set the mode idle
*/
    w->browse.button_mode = MODEIDLE;
    w->browse.range_hook  = 0;
}
                                                                          /* */
/*
**  This routine is called on the first motion event while the button is being
**  held down.  This could be in response to a drag operation or to a range
**  selection.  This routine handles the former.
*/

static void LclManageDragging (w)

  dns_widget w;

{
/*
**  Local data declarations
*/
    CallbackStruct temp;
    Window root_return, child_return;
    unsigned int state_mask_return;
    int root_x_return, root_y_return, win_x_return, win_y_return;
    int status;


/*
**  Get the current pointer location
*/
    status = XQueryPointer (XtDisplay(w), XtWindow(w), &root_return, &child_return,
                            &root_x_return, &root_y_return, &win_x_return, &win_y_return,
                            &state_mask_return);


/*
**  Map that location into an entry number
*/
    MapPosition (w, win_x_return, win_y_return, &temp.entry_number, &temp.component_number, &temp.entry_tag);


/*
**  If we are not on the same entry that the timer was set on, then forget
**  that we ever got called...  This may be a pain, but should reduce errors
**  made by the user...
*/
    if (temp.entry_number != w->browse.timer_entry)
       {
         w->browse.button_mode = MODEIDLE;
         return;
       }


/*
**  Record the current location of the pointer to be the drag holding point
*/
    w->browse.ghost_x = win_x_return;
    w->browse.ghost_y = win_y_return;


/*
**  Determine if the application is under control of this drag.  We wouldn't
**  be here if dragging wasn't supported at all.
*/
    if (w->browse.Dragging_callback != (XtCallbackList) NULL)
       {
          temp.reason           = CRDragging;
          temp.x                = win_x_return;
          temp.y                = win_y_return;

          DisableDisplay (w);
          XtCallCallbacks (w, NdraggingCallback, &temp);
          EnableDisplay (w);
          return;
       }


/*
**  We are now under control of the dragging
*/
    w->browse.button_mode = MODEDRAG;


/*
**  Create and display a ghosting pixmap
*/
    DisplayCreateGhost (w);
    LclDrawGhostBox (w, win_x_return, win_y_return);


/*
**  Do a while forever loop until the user is completed or cancelled.
*/
    while (TRUE)

      {
        /*
        **  Find out the current state of the pointer and mouse buttons.
        */
            status = XQueryPointer (XtDisplay(w), XtWindow(w), &root_return, &child_return,
                                    &root_x_return, &root_y_return, &win_x_return, &win_y_return,
                                    &state_mask_return);


        /*
        **  If the return status is bad or one of the other buttons is pressed, then cancel
        */
            if ((status == 0) || (state_mask_return & Button2Mask) || (state_mask_return & Button3Mask))
               {
                 LclDrawGhostBox (w, w->browse.ghost_x, w->browse.ghost_y);
                 break;
               }


        /*
        **  If the button has been released, then process the selection
        */
            if ((state_mask_return & Button1Mask) == FALSE)
               {
                 LclDrawGhostBox (w, w->browse.ghost_x, w->browse.ghost_y);

                 MapPosition (w, w->browse.ghost_x, w->browse.ghost_y, &temp.entry_number,
                                     &temp.component_number, &temp.entry_tag);

                 temp.reason = CRSelectionsDragged;
                 temp.x      = w->browse.ghost_x;
                 temp.y      = w->browse.ghost_y;

                 DisableDisplay (w);
                 XtCallCallbacks (w, NselectionsDraggedCallback, &temp);
                 EnableDisplay (w);
                 break;
               }


        /*
        **  See if the screen needs autoscrolled.
        */
            status = AutoScrollCheck (w, win_x_return, win_y_return);


        /*
        **  If it does, then Erase and redraw the box after performing the autoscrolling.
        */
            if (status != 0)
               {
                 LclDrawGhostBox (w, w->browse.ghost_x, w->browse.ghost_y);
                 AutoScrollDisplay (w, win_x_return, win_y_return);
                 LclDrawGhostBox (w, win_x_return, win_y_return);
               }


        /*
        **  If it doesn't autoscroll, then repaint if X or Y has changed
        */
            if (status == 0)
               if ((w->browse.ghost_x != win_x_return) || (w->browse.ghost_y != win_y_return))
                  {
                    LclDrawGhostBox (w, w->browse.ghost_x, w->browse.ghost_y);
                    LclDrawGhostBox (w, win_x_return, win_y_return);
                  }
      }


/*
**  Delete the ghost and set the mode idle
*/
    DisplayDeleteGhost (w);
    w->browse.button_mode = MODEIDLE;
}
                                                                          /* */
/*
**  Local routine that handles the dragging timers triggering.
*/

static void LclTimerTrigger (w)

  dns_widget w;

{
/*
**  Set the timerid to zero to show that it is no longer set.
*/
    w->browse.timerid = 0;


/*
**  If we were awaiting a dragging timer, then begin dragging mode
*/
    if (w->browse.button_mode == MODEAWAITDRAG)
       LclManageDragging (w);
}
                                                                          /* */
/*
**  The following page is the class record and procedures for a new class
**  called the Window.  The only procedures used in this class are the
**  Realize procedure (which creates the window) and the geometry_manager
**  routine which will adjust the entry height.
**
**  The procedures are placed first so that forward references are not needed.
*/
                                                                          /* */
/*
**  Data structures for the Window Class.  Note that nobody actually looks
**  into the instance record.
*/
typedef struct
    {
    caddr_t dummy;
    } WindowClassPart;

typedef struct
    {
    caddr_t dummy;
    } WindowPart;

typedef struct _WindowClassRec
    {
    CoreClassPart             core_class;
    CompositeClassPart        composite_class;
    WindowClassPart        window_class;
    } WindowClassRec, *WindowClass;

typedef struct _WindowRec
    {
    CorePart              core;
    CompositePart         composite;
    WindowPart         window;
    } WindowRec, *WindowWidget;
                                                                          /* */
/*
**  This routine will realize the Window widget.  It is responsible for
**  creating the widget window.
*/

static void WindowRealize (ww, valuemask, attributes)

  WindowWidget ww;
  Mask *valuemask;
  XSetWindowAttributes *attributes;

{
/*
**  Local data declarations
*/
    Mask lclValueMask = *valuemask;


/*
**  Make it forget gravity
*/
    attributes->bit_gravity = ForgetGravity;
    lclValueMask |= CWBitGravity;


/*
**  Create the X window
*/
    XtCreateWindow (ww, InputOutput, CopyFromParent, lclValueMask, attributes);
}
                                                                          /* */
/*
**  This routine will get called when this window is resized.
*/

static void WindowResize (unused_ww)

  WindowWidget unused_ww;

{
/*
**  Do nothing.  This routine is necessary because the module calls
**  the resize procedure of the widgets.  I tried XtInheritResize, but could
**  not get it to work.
*/
}
                                                                          /* */
/*
**  This is the class geometry management routine.
*/

static XtGeometryResult WindowGeometryManager (wi, g)

    Widget wi;
    XtWidgetGeometry *g;

{
/*
**  Local Variables
*/
    dns_widget w;
    EntryPtr entry;
    int entry_number, comp_offset;


/*
**  Fulfill the request from the subwidget
*/
    if (g->request_mode & CWX          ) wi->core.x            = g->x;
    if (g->request_mode & CWY          ) wi->core.y            = g->y;
    if (g->request_mode & CWBorderWidth) wi->core.border_width = g->border_width;
    if (g->request_mode & CWWidth      ) wi->core.width        = g->width;
    if (g->request_mode & CWHeight     ) wi->core.height       = g->height;


/*
**  Go up through the parent chain looking for the class widget.
*/
    w = StructFindWidget (wi);


/*
**  Get the first entry structure
*/
    entry = w->browse.entryPtr;
    entry_number = 1;


/*
**  Spin through the following loop until the entry with this as a subwidget
**  is found or the end of the list is hit.
*/
    while (entry != (EntryPtr) NULL)
       {
	 for (comp_offset = 0;  comp_offset <= entry->num_components - 1;  comp_offset++)
             if (entry->entrycompPtr[comp_offset].var.is_widget.readwrite_text == wi)
	        {
		  /*
		  **  Disable the display, and redraw the entry and leave the while loop.
		  */
		      DisableDisplay (w);
		      DisplayInvalidateEntry (w, entry_number);
		      EnableDisplay (w);
		      break;
		}
         entry_number++;
         entry = entry->next;
        }


/*
**  Return confirmation
*/
    return XtGeometryYes;
}
                                                                          /* */
/*
**  This is the class record that gets set at compile/link time this is what
**  is passed to the widgetcreate routine as the class.  All fields must be
**  inited at compile time.
*/
/*#pragma nostandard*/
externaldef(windowwidgetclassrec)
/*#pragma standard*/
WindowClassRec windowwidgetclassrec
  = {
      {
         /* CORE CLASS RECORD   */
         /* superclass          */   (WidgetClass) &compositeClassRec,
         /* class_name          */   "Window",
         /* widget_size         */   sizeof(WindowRec),
#if XtSpecificationRelease == 5
	 /* class_initialize    */   WindowClassInitialize,
#else
	 /* class_initialize    */   NULL,
#endif
         /* class_part_init     */   NULL,
         /* class_inited        */   FALSE,
         /* initialize          */   NULL,
         /* initialize_hook     */   NULL,
         /* realize             */   WindowRealize,
         /* actions             */   NULL,
         /* num_actions         */   0,
         /* resources           */   NULL,
         /* num_resources       */   0,
         /* xrm_class           */   NULLQUARK,
         /* compress_motion     */   TRUE,
         /* compress_exposure   */   FALSE,
         /* compress_enterleave */   FALSE,
         /* visible_interest    */   FALSE,
         /* destroy             */   NULL,
         /* resize              */   WindowResize,
         /* expose              */   NULL,
         /* set_values          */   NULL,
         /* set_values_hook     */   NULL,
         /* set_values_almost   */   XtInheritSetValuesAlmost,
         /* get_values_hook     */   NULL,
         /* accept_focus        */   NULL,
         /* version (temp off)  */   XtVersionDontCheck,
         /* callback offsetlst  */   NULL,
         /* tm_table            */   NULL,
         /* query geometry      */   NULL,
         /* display accelerator */   NULL,
#if XtSpecificationRelease == 5
	 /* extension           */   (XtPointer)&windowBaseClassExtRec
#else
         /* extension           */   NULL
#endif
      },
      {
         /* COMP CLASS RECORD   */
         /* childrens geom mgr  */   WindowGeometryManager,
         /* set changed proc    */   XtInheritChangeManaged,
         /* add a child         */   XtInheritInsertChild,
         /* remove a child      */   XtInheritDeleteChild,
         /* extension           */   NULL,
      },
      {
         /* extension           */   NULL,
      }
    };

/*#pragma nostandard*/
externaldef(windowwidgetclass)
/*#pragma standard*/
WindowClass windowwidgetclass = &windowwidgetclassrec;
                                                                          /* */
/*
**  This routine creates a window widget...
*/

Widget BrowseWindow (parent, name, width, height)

  Widget          parent;
  char           *name;
  int             width, height;

{
/*
**  Routine data declarations
*/
    Arg arglist[5];


/*
**  Pack the supplied arguments and default the x and y and borders
*/
    XtSetArg (arglist[0], XmNx, 0);
    XtSetArg (arglist[1], XmNy, 0);
    XtSetArg (arglist[2], XmNborderWidth, 0);
    XtSetArg (arglist[3], XmNwidth, width);
    XtSetArg (arglist[4], XmNheight, height);


/*
**  The XtCreateWidget routine will allocate the class and instance records for the widget.
*/
    return (XtCreateWidget (name, windowwidgetclass, parent, arglist, 5));
}
