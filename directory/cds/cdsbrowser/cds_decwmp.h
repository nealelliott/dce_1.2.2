/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cds_decwmp.h,v $
 * Revision 1.1.4.2  1996/02/18  23:33:13  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:25  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:02:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:36:57  root]
 * 
 * Revision 1.1.2.2  1992/12/30  12:38:24  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:22:12  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:23:18  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*=======================================================================
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


#include <cds_decwm.h>


/*----------------------------------*/
/* widget part of class record      */
/*----------------------------------*/

typedef struct
    {
    caddr_t dummy;
    } ClassPart;


/*----------------------------------*/
/* full widget class record         */
/*----------------------------------*/

typedef struct _WidClassRec
    {
    CoreClassPart             core_class;
    CompositeClassPart        composite_class;
    ClassPart                 dns_class;
    } WidClassRec, *WidClass;


/*----------------------------------------*/
/* widget private component structure     */
/*----------------------------------------*/

typedef struct
    {
    union
    {
      struct {
        char           *text;            /* string address               */
        XFontStruct    *font;            /* font identifier              */
        long           text_len;         /* number of characters to draw */
        } is_text;

      struct {
        XmString	cstring;	/* Compound string 	    */
        XmFontList	font_list;	/* font list  */
        } is_cstring;

      struct {
        Pixmap          pixmap;          /* Pixmap structure             */
        } is_pixmap;

      struct {
        Widget          readwrite_text;  /* text widget identifier       */
        } is_widget;
    } var;

    Position        orig_x;          /* original relative to entry   */
    Position        orig_y;          /* original relative to entry   */
    Position        x;               /* relative to entry            */
    Position        y;               /* relative to entry            */

    Dimension       orig_width;      /* original width               */
    Dimension       orig_height;     /* original height              */
    Dimension       height;          /* height of component          */
    Dimension       width;           /* width of component           */

    short           orig_rj;         /* originally right justified   */
    short           type;            /* type of component            */
    short           hidden_mode;     /* displaymode to be hidden     */
    } CompStruct, *CompPtr;



/*-----------------------------------------*/
/* literal defining the max display length */
/*-----------------------------------------*/

#define max_comps    30
#define max_display  75
#define max_clips    25


/*-----------------------------------------*/
/* Pointer to the level array for tree     */
/* and default size of the array.          */
/*-----------------------------------------*/

#define DEFAULT_MAX_LEVEL 5
typedef int (*LevelPtr)[];


/*-----------------------------------------*/
/* Define component type literals.  We'll  */
/*  use the Dwt ones so that we can use    */
/*  some of their support routines.  We'll */
/*  define the widget one as 99 so that we */
/*  don't conflict with them.              */
/*-----------------------------------------*/

#define KcompNotSet   0
#define KcompText     1
#define KcompText16   2
#define KcompPixmap   3
#define KcompCString  4
#define KcompWidget   5


/*----------------------------------------*/
/* widget private entry structure         */
/*----------------------------------------*/

typedef struct entrynode
    {
    struct entrynode   * next;               /* pointer to next entry      */
    struct entrynode   * prev;               /* pointer to previous entry  */
    CompStruct      * entrycompPtr;       /* pointer to component       */
    unsigned int         entry_tag;          /* application controlled tag */
    int                  x;                  /* x position of entry        */
    int                  px;                 /* connection x pos to parent */
    int                  py;                 /* connection y pos to parent */
    Dimension            width;              /* the width of this entry    */
    Dimension            height;             /* the height of this entry   */
    Dimension            orig_height;        /* height specified in set    */
    short                num_allocated;      /* number of allocated comps  */
    short                num_components;     /* the number of components   */
    short                level;              /* the entries level number   */
    short                selected_comp;      /* component selected         */
    Boolean              valid;              /* in synch with the source   */
    Boolean              height_adjusted;    /* added top and bottom       */
    Boolean              selected;           /* the entry is selected      */
    Boolean              highlighted;        /* the entry is highlighted   */
    Boolean              grayed;             /* the entry is unselectable  */
    Boolean              index_window;       /* show in index window       */
    } EntryStruct, *EntryPtr;


/*----------------------------------------*/
/* widget part of instance record         */
/*----------------------------------------*/

/*
**  In order to run on MIPS with minimal fixup for natural boundary access, the
**  fields are ordered according to size.  The biggest ones are first, followed
**  by smaller and smaller fields.
*/

typedef struct
    {
/*
**  The following are structures
*/
     XFontStruct        *default_font;                /* Nfont               */
     XmFontList		default_fontlist;             /* XtExtDefaultFont    */
     XFontStruct        *level0_font;                 /* NfontLevel0         */
     XmFontList		level0_fontlist;              /* NfontListLevel0     */
     XFontStruct        *level1_font;                 /* NfontLevel1         */
     XmFontList		level1_fontlist;              /* NfontListLevel1     */
     XFontStruct        *level2_font;                 /* NfontLevel2         */
     XmFontList		level2_fontlist;              /* NfontListLevel2     */
     XFontStruct        *level3_font;                 /* NfontLevel3         */
     XmFontList		level3_fontlist;              /* NfontListLevel3     */
     XFontStruct        *level4_font;                 /* NfontLevel4         */
     XmFontList		level4_fontlist;              /* NfontListLevel4     */
     XtCallbackList   	SelectAndConfirm_callback;    /* NselectAndConfir.   */
     XtCallbackList   	ExtendConfirm_callback;       /* NextendConfirmCa.   */
     XtCallbackList   	EntrySelected_callback;       /* NentrySelectedCa.   */
     XtCallbackList   	EntryUnselected_callback;     /* NentryUnselected    */
     XtCallbackList   	TransitionsDone_callback;     /* NtransitionsDone    */
     XtCallbackList   	DisplayChanged_callback;      /* NdisplayChangedC    */
     XtCallbackList   	AttachToSource_callback;      /* NattachToSourceC.   */
     XtCallbackList   	DetachFromSource_callback;    /* NdetachFromSourc.   */
     XtCallbackList   	SelectionsDragged_callback;   /* NselectionsDragg.   */
     XtCallbackList   	GetEntry_callback;            /* NgetEntryCallback   */
     XtCallbackList   	Dragging_callback;            /* NdraggingCallback   */
     XtCallbackList   	DraggingEnd_callback;         /* NdraggingEndCall.   */
     XtCallbackList   	Help_callback;                /* DwtNhelpRequested   */
     XRectangle         clips [max_clips];            /* Clip rectangles     */

/*
**  The following are declared as an XID which is an unsigned long - 4 bytes
*/
     Window              clips_window;                 /* window for clips    */
     Cursor              watch_cursor;                 /* Created once        */
     Cursor              nav_box_cursor;               /* Created once        */
     Pixmap              grey_pixmap;                  /* graphic contexts    */
     Pixmap              top_pixmap;                   /*                     */
     Pixmap              bot_pixmap;                   /*                     */
     Pixmap              ghost;                        /*                     */
     Pixmap              tree_nav_open_pixmap;         /* nav button pixmap   */
     Pixmap              tree_nav_close_pixmap;        /* nav button pixmap   */
     Pixmap              outline_nav_pixmap;           /* nav button pixmap   */
     Pixmap              tree_highlight_pixmap;        /* nav button pixmap   */
/*
**  These are declared as unsigned long's which are 4 bytes
*/
     Pixel               foreground_pixel;             /* XmNforeground      */
     XtIntervalId        timerid;                      /* drag/double click   */
     XtIntervalId        button_timerid;               /* id for timer        */
     Time                button_up_event_time;
/*
**  pointers are 4 bytes
*/
     GC                  inverse_gc;                   /* graphic contexts    */
     GC                  background_gc;                /* graphic contexts    */
     GC                  foreground_gc;                /* graphic contexts    */
     GC                  grey_gc;                      /* graphic contexts    */
     GC                  drag_gc;                      /* graphic contexts    */
     GC                  tree_highlight_gc;            /* GC for wide lines   */
     GC                  nav_box_gc;                   /* GC for nav box      */
     Widget              index_window;                 /* widget identifier   */
     Widget              vscroll;                      /* scroll bars         */
     Widget              hscroll;                      /* NoutlineHScrollWi   */
     Widget              rhs_hscroll;                  /*                     */
     Widget              top_button;                   /*                     */
     Widget              bot_button;                   /*                     */
     Widget              nav_button;                   /* push button for nav */
     Widget		 nav_window_popup;	       /* nav window popup    */
     Widget		 nav_window;		       /* nav window widget   */
     Widget              pane_widget;                  /* NpaneWidget         */
     Widget              lhs_window_widget;            /* NlhsWindowWidget    */
     Widget              rhs_window_widget;            /* NrhsWindowWidget    */
     XmString	         nav_window_title;	       /* NnavWindowTitle     */
     EntryPtr	         current_entry;		       /* entry to keep place */
     EntryPtr            entryPtr;                     /* pointer to entry    */
     EntryPtr            cache_pointer;                /* pointer cache       */
     LevelPtr		 levelx;		       /* ptr max x values    */
     LevelPtr		 levely;		       /* ptr y of each level */
     char               *user_data;                    /* XmNuserData         */
/*
**  int's are 4 bytes
*/
     int                 num_entries;                  /* NnumberOfEntries    */
     int                 num_selections;               /* number of selections*/
     int                 num_highlighted;              /* number of hi-lighted*/
     int                 cache_number;                 /* entry in cache      */
     int                 range_hook;                   /* entry in range      */
     int                 button_top;                   /* top or bottom       */
     int                 button_waitms;                /* ms before start     */
     int                 button_repeatms;              /* ms before repeat    */
     int		 mapx, mapy;		       /* world coord to window convert */
     int		 tree_width;		       /* width of tree */
     int		 tree_height;		       /* height of tree */
     int		 prevx, prevy;		       /* previous location of current_entry */
     int		 range_hook_x;		       /* start point of range select */
     int		 range_hook_y;		       /* start point of range select */
     int		 nav_window_box_x;	       /* position of current */
     int		 nav_window_box_y;	       /* display box in nav window */
     int		 nav_window_box_height;
     int		 nav_window_box_width;
     int		 current_entry_number;	       /* entry number for current entry */
     int		 vscroll_in_progress;
     int		 hscroll_in_progress;
     int		 map_level;
     int		 lhs_percentage;	       /* save/restore column */
     unsigned int        column_tags [max_comps];

/*
**   Dimension's are Card's which are unsigned ints which are 4 bytes
*/
     Dimension           column_widths [max_comps];
     Dimension           margin_width;                 /* XmNmarginWidth     */
     Dimension           margin_height;                /* XmNmarginHeight    */
     Dimension           indent_margin;                /* NindentMargin      */
     Dimension           default_spacing;              /* NdefaultSpacing    */
     Dimension           level_spacing;		       /* NtreeLevelSpacing  */
     Dimension           sibling_spacing;	       /* NtreeSiblingSpac   */
     Dimension           arc_width;		       /* NtreeArcWidth      */
     Dimension           max_width;                    /* needed for horiz scr*/
     Dimension           rhs_max_width;                /* needed for horiz scr*/
     Dimension           ghost_width;                  /*                     */
     Dimension           ghost_height;                 /*                     */
     Dimension           display_invalid[max_display]; /* zero or old_height  */

/*
**   Position's are ints which are 4 bytes
*/
     Position            window_basex;                 /* Horizontal scrolling*/
     Position            box_base_x;                   /* range box           */
     Position            box_base_y;                   /*                     */
     Position            box_other_x;                  /*                     */
     Position            box_other_y;                  /*                     */
     Position            ghost_x;                      /*                     */
     Position            ghost_y;                      /*                     */
     Position            ghost_basex;                  /*                     */
     Position            ghost_basey;                  /*                     */
     Position            display_line_y;               /* y position of line  */
     Position            rhs_base_x;		       /* NrhsBaseX           */

/*
**   short's are 2 bytes
*/
     short               display_mode;                 /* NdisplayMode        */
     short               tree_style;	               /* NtreeStyle	      */
     short               start_column_component;       /* NstartColumnCompo   */
     short               selection_mode;               /* NselectionMode      */
     short               disabled_count;               /* counter             */
     short               index_window_shown;           /* entry number        */
     short               button_mode;                  /* used in module      */
     short               clip_count;                   /* used rectangles     */
     short               timer_entry;                  /*                     */
     short               num_path;                     /* entries in path     */
     short               entries[max_display];         /*                     */
     short               display_count;                /* count of displayed  */
     short               max_level;                    /* number of levels in level array */

/*
**   Boolean's are char's which are 1 byte
*/
     Boolean             column_width_set [max_comps]; /* Auto adjust column  */
     Boolean             column_lines;  	       /* NcolumnLines        */
     Boolean		 fixed_width;                  /* NfixedWidthEntrie   */
     Boolean             multiple_selections;          /* NmultipleSelecti.   */
     Boolean             use_scroll_buttons;           /* NuseScrollButtons   */
     Boolean             expect_highlighting;          /* NexpectHighlighti   */
     Boolean             force_seq_get_entry;          /* NforceSeqGetEntry   */
     Boolean             show_path_to_root;            /* NshowPathToRoot     */
     Boolean		 centered_components;	       /* NtreeCenteredComp   */
     Boolean		 perpendicular_lines;	       /* NtreePerpendicula   */
     Boolean		 index_all;		       /* NtreeIndexAll       */
     Boolean		 entry_shadows;		       /* NtreeEntryShadows   */
     Boolean		 tree_entry_outlines;	       /* NtreeEntryOutline   */
     Boolean		 truncate_strings;	       /* NtruncateStrings    */
     Boolean             rhs_components_unmapped;      /* NrhsComponentsUnm   */
     Boolean             index_window_needed;          /* true or false       */
     Boolean             sub_widgets_used;             /* sub widgets are used*/
     Boolean             show_selections;              /* For global selection*/
     Boolean             show_highlighting;            /*                     */
     Boolean             ghosting;                     /* ghosting            */
     Boolean             display_changed;              /* accuracy of screen  */
     Boolean             transitions_made;
     Boolean	         refresh_all;		       /* display needs to be redrawn */
     Boolean             button_down;                  /* button autorepeat   */
     Boolean		 update_in_progress;	       /* flag that screen update is in progress */
     Boolean		 tree_connections;	       /* draw connections? */
     Boolean		 update_nav_window;	       /* Nav window needs to be redrawn */
    } Part;


typedef struct _Rec                  /* full instance record */
    {
    CorePart              core;
    CompositePart         composite;
    Part                  browse;
    } Rec, *dns_widget;

#ifndef DNS
/*#pragma nostandard*/
externalref WidClassRec       widget_classrec;
externalref WidClass          widget_class;
/*#pragma standard*/
#endif
