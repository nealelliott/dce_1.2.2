/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cds_decwm.h,v $
 * Revision 1.1.4.2  1996/02/18  23:33:13  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:22  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:02:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:36:56  root]
 * 
 * Revision 1.1.2.2  1992/12/30  12:38:18  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:22:07  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:23:16  devrcs
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

/*----------------------------------*/
/* Class Name                       */
/*----------------------------------*/

#define ClassName "DNS"

#define Nfont                        "fontDefault"
#define NhelpRequestedCallback        XmNhelpCallback
#define NselectAndConfirmCallback    "selectAndConfirmCallback"
#define NextendConfirmCallback       "extendConfirmCallback"
#define NentrySelectedCallback       "entrySelectedCallback"
#define NentryUnselectedCallback     "entryUnselectedCallback"
#define NtransitionsDoneCallback     "transitionsDoneCallback"
#define NattachToSourceCallback      "attachToSourceCallback"
#define NdetachFromSourceCallback    "detachFromSourceCallback"
#define NselectionsDraggedCallback   "selectionsDraggedCallback"
#define NgetEntryCallback            "getEntryCallback"
#define NdraggingCallback            "draggingCallback"
#define NdraggingEndCallback         "draggingEndCallback"
#define NdisplayChangedCallback      "displayChangedCallback"

/*----------------------------------*/
/* Position Display constants       */
/*----------------------------------*/

#define KpositionTop           1
#define KpositionMiddle        2
#define KpositionBottom        3
#define KpositionPreviousPage  4
#define KpositionNextPage      5


/*----------------------------------*/
/* Display Mode constants           */
/*----------------------------------*/

#define KdisplayNone	       0
#define KdisplayOutline        1
#define KdisplayTree	       2
#define KdisplayAllModes       3
#define KdisplayColumns        4


/*----------------------------------*/
/* Selection Mode constants         */
/*----------------------------------*/

#define KselectEntry           0
#define KselectComp            1
#define KselectCompAndLhs      2
#define KselectEntryOrComp     3


/*----------------------------------*/
/* Tree Style constants             */
/*----------------------------------*/

#define KtopTree		 1
#define KleftTree		 2
#define KoutlineTree		 3
#define KuserDefinedTree	 4


/*----------------------------------*/
/* Callback reasons                 */
/*----------------------------------*/

#define CRHelpRequested       XmCR_HELP  /* 1 */
#define CRSelectAndConfirm    2
#define CREntrySelected       3
#define CREntryUnselected     4
#define CRAttachToSource      5
#define CRDetachFromSource    6
#define CRSelectionsDragged   8
#define CRGetEntry            9
#define CRDragging            10
#define CRDraggingEnd         11
#define CRExtendConfirm       13
#define CRTransitionsDone     14
#define CRDisplayChanged      15


/*----------------------------------*/
/* Callback structure definition    */
/*----------------------------------*/

typedef struct
{
    int           reason;		/*  Used by all    */
    int           entry_number;         /*  Used by 80%    */
    int           component_number;     /*  Used by 40%    */
    int           first_selection;
    int           x;
    int           y;
    unsigned int  entry_tag;
    Time          time;                 /*  Used by Select */
    int		  entry_level;

} CallbackStruct;


/*----------------------------------*/
/* Symbols for specific help items  */
/* returned in entry_number field   */
/* of help callback		    */
/*----------------------------------*/

#define KhelpScroll		-1
#define KhelpNavButton	-2
#define KhelpNavWindow	-3


/*----------------------------------*/
/* Symbols for values returned in   */
/* entry_number field of a callback */
/*----------------------------------*/

#define KnotFirst		0
#define KfirstOfOne		1
#define KfirstOfMany		3


/*----------------------------------*/
/* widget external routines     */
/*----------------------------------*/

extern Widget       BrowseWidget();
extern Widget       CreateBrowseWidget();
extern void         DisableDisplay();
extern void         EnableDisplay();
extern void         MapPosition();
extern int          GetNumSelections();
extern void         GetSelections();
extern void         ClearSelection();
extern void         ClearSelections();
extern void         SelectAll();
extern void         SelectComponent();
extern void         SelectEntry();
extern void         SelectComponent();
extern void         AddEntries();
extern void         DeleteEntries();
extern void         InvalidateEntry();
extern void         SetEntrySensitivity();
extern void         SetEntry();
extern void         SetComponentPixmap();
extern char*        GetComponentText();
extern int          SetComponentText();
extern void         SetComponentCString();
extern void         SetComponentWidget();
extern int          PositionDisplay();
extern unsigned int GetEntryTag();
extern unsigned int GetEntryNumber();
extern void         HideSelections();
extern void         ShowSelections();
extern int          AutoScrollCheck();
extern void         AutoScrollDisplay();
extern Widget	    GetLhsWorkWidget();
extern Widget	    GetRhsWorkWidget();
extern void         SetApplDragging();
extern int          GetNumHighlighted();
extern void         GetHighlighted();
extern int          GetNumDisplayed();
extern void         GetDisplayed();
extern void         ClearHighlight();
extern void         ClearHighlighting();
extern void         HighlightAll();
extern void         HighlightEntry();
extern void         ShowHighlighting();
extern void         HideHighlighting();
extern unsigned int InitializeForMRM();
extern void         ValidateAll();
extern void	    SetComponentHidden();
extern void	    SetEntryTag();
extern void	    SetEntryIndexWindow();
extern void	    SetEntryNumComponents();
extern void	    GetTreePosition();
extern void	    SetTreePosition();
extern void         SetEntryPosition();
extern void         GetEntryPosition();
extern void         FlushEntry();
extern void         InsertComponent();
extern void         RemoveComponent();
extern void         SetComponentWidth();
extern Dimension    GetComponentWidth();
extern void         SetComponentTag();
extern unsigned int GetComponentTag();
extern int          GetComponentNumber();
