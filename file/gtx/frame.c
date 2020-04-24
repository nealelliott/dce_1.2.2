/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: frame.c,v $
 * Revision 1.1.12.1  1996/10/02  17:50:49  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:39:53  damon]
 *
 * Revision 1.1.7.1  1994/06/09  14:09:48  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:24:27  annie]
 * 
 * Revision 1.1.5.3  1993/01/19  16:02:25  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  14:12:23  cjd]
 * 
 * Revision 1.1.5.2  1992/11/24  17:51:29  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  18:04:57  bolinger]
 * 
 * Revision 1.1.3.4  1992/05/20  19:54:36  mason
 * 	Transarc delta: vijay-ot3289-gtx-fix-lock-calls 1.2
 * 	  Files modified:
 * 	    gtx: Makefile, X11windows.c, dumbwindows.c, frame.c
 * 	    gtx: gtxtextcb.h, input.c, lightobject.c, objects.c
 * 	    gtx: textcb.c, textobject.c
 * 	  Selected comments:
 * 	    change the lock_ calls to afslk_ calls.
 * 	    remove a lwp.h include
 * 	    remove a spurious cml diff line. This line was not deleted upon a merge
 * 	    and gave some compilations problems.
 * 	[1992/05/20  11:35:17  mason]
 * 
 * Revision 1.1.3.3  1992/01/24  03:45:36  devsrc
 * 	Merging dfs6.3
 * 	[1992/01/24  00:13:38  devsrc]
 * 
 * Revision 1.1.3.2  1992/01/23  19:49:51  zeliff
 * 	Moving files onto the branch for dfs6.3 merge
 * 	[1992/01/23  18:34:22  zeliff]
 * 
 * $EndLog$
*/
#include <pthread.h>

#include <dcedfs/gtxobjects.h>
#include <dcedfs/gtxwindows.h>
#include <dcedfs/gtxcurseswin.h>
#include <dcedfs/gtxinput.h>
#include <dcedfs/gtxkeymap.h>
#include <dcedfs/gtxframe.h>

extern char *gtx_CopyString();
static struct keymap_map *recursiveMap=0;
static char menubuffer[1024];		/*Buffer for menu selections*/
int gtxframe_exitValue = 0;		/*Program exit value*/

gtxframe_CtrlUCmd(awindow, arock)
char *arock;
struct gwin *awindow; {
    struct gtx_frame *tframe;

    tframe = awindow->w_frame;
    if (!tframe->defaultLine) return -1;
    *(tframe->defaultLine) = 0;
    return 0;
}

gtxframe_CtrlHCmd(awindow, arock)
char *arock;
struct gwin *awindow; {
    register struct gtx_frame *tframe;
    register char *tp;
    register long pos;

    tframe = awindow->w_frame;
    if (!(tp=tframe->defaultLine)) return -1;
    pos = strlen(tp);
    if (pos == 0) return 0;	/* rubout at the end of the line */
    tp[pos-1] = 0;
    return 0;
}

gtxframe_RecursiveEndCmd(awindow, arock)
char *arock;
register struct gwin *awindow; {
    register struct gtx_frame *tframe;

    tframe = awindow->w_frame;
    tframe->flags |= GTXFRAME_RECURSIVEEND;
    tframe->flags &= ~GTXFRAME_RECURSIVEERR;
    return 0;
}

gtxframe_RecursiveErrCmd(awindow, arock)
char *arock;
register struct gwin *awindow; {
    register struct gtx_frame *tframe;

    tframe = awindow->w_frame;
    tframe->flags |= GTXFRAME_RECURSIVEEND;
    tframe->flags |= GTXFRAME_RECURSIVEERR;
    return 0;
}

gtxframe_SelfInsertCmd(awindow, arock)
long arock;
struct gwin *awindow; {
    register struct gtx_frame *tframe;
    register long pos;
    register char *tp;

    tframe = awindow->w_frame;
    if (!(tp = tframe->defaultLine)) return -1;
    pos = strlen(tp);
    tp[pos] = arock;	/* arock has char to insert */
    tp[pos+1] = 0;	/* null-terminate it, too */
    return 0;
}

/* save map, setup recursive map and install it */
static SaveMap(aframe)
register struct gtx_frame *aframe; {
    char tstring[2];
    register long i;

    if (!recursiveMap) {
	/* setup recursive edit map if not previously done */
	recursiveMap = keymap_Create();
	keymap_BindToString(recursiveMap,
			    "\010",
			    gtxframe_CtrlHCmd,
			    (char *) 0,
			    (char *) 0);
	keymap_BindToString(recursiveMap,
			    "\177",gtxframe_CtrlHCmd,
			    (char *) 0,
			    (char *) 0);
	keymap_BindToString(recursiveMap,
			    "\025",
			    gtxframe_CtrlUCmd,
			    (char *) 0,
			    (char *) 0);
	keymap_BindToString(recursiveMap,
			    "\033",
			    gtxframe_RecursiveEndCmd,
			    (char *) 0,
			    (char *) 0);
	keymap_BindToString(recursiveMap,
			    "\015",
			    gtxframe_RecursiveEndCmd,
			    (char *) 0,
			    (char *) 0);
	keymap_BindToString(recursiveMap,
			    "\012",
			    gtxframe_RecursiveEndCmd,
			    (char *) 0,
			    (char *) 0);
	keymap_BindToString(recursiveMap,
			    "\003",
			    gtxframe_RecursiveErrCmd,
			    (char *) 0,
			    (char *) 0);
	keymap_BindToString(recursiveMap,
			    "\007",
			    gtxframe_RecursiveErrCmd,
			    (char *) 0,
			    (char *) 0);

	for(i=040; i<0177; i++) {
	    tstring[0] = i;
	    tstring[1] = 0;
	    keymap_BindToString(recursiveMap, tstring, gtxframe_SelfInsertCmd,
				(char *) 0, i);
	}
    }
    aframe->savemap = aframe->keymap;
    aframe->keymap = recursiveMap;
    keymap_InitState(aframe->keystate, aframe->keymap);
    return 0;
}

/* Restore map to previous value */
static RestoreMap(aframe)
register struct gtx_frame *aframe; {
    aframe->keymap = aframe->savemap;
    aframe->savemap = (struct keymap_map *) 0;
    keymap_InitState(aframe->keystate, aframe->keymap);
    return 0;
}

gtxframe_SetFrame(awin, aframe)
struct gtx_frame *aframe;
struct gwin *awin; {
    if (awin->w_frame) {
	/* Unthread this frame */
	awin->w_frame->window = (struct gwin *) 0;
    }
    awin->w_frame = aframe;
    aframe->window = awin;	/* Set frame's window ptr */
    return 0;
}

struct gtx_frame *gtxframe_GetFrame(awin)
struct gwin *awin; {
    return awin->w_frame;
}

/* Add a menu string to display list */
gtxframe_AddMenu(aframe, alabel, astring)
char *alabel;
char *astring;
struct gtx_frame *aframe; {
    register struct gtxframe_menu *tmenu;

    if (aframe->menus)
      for(tmenu = aframe->menus; tmenu; tmenu = tmenu->next) {
	if (strcmp(alabel, tmenu->name)==0) break;
      }
    else
      tmenu = (struct gtxframe_menu *)0;
    if (!tmenu) {
	/* Handle everything but the command string, which is handled by the
	   common-case code below */
	tmenu = (struct gtxframe_menu *) malloc(sizeof(*tmenu));
	if (tmenu == (struct gtxframe_menu *)0)
	  return(-1);
	bzero(tmenu, sizeof(*tmenu));
	tmenu->next = aframe->menus;
	aframe->menus = tmenu;
	tmenu->name = gtx_CopyString(alabel);
    }

    /*
      * Common case: redo the string labels.  Note: at this point, tmenu
      * points to a valid menu.
      */
    if (tmenu->cmdString)
      free(tmenu->cmdString);
    tmenu->cmdString = gtx_CopyString(astring);
    return 0;
}

/* Delete a given menu from a frame*/
gtxframe_DeleteMenu(aframe, alabel)
struct gtx_frame *aframe;
char *alabel; {
    register struct gtxframe_menu *tm, **lm;

    for(lm = &aframe->menus, tm = *lm; tm; lm = &tm->next, tm = *lm) {
	if (strcmp(alabel, tm->name) == 0) {
	    /* found it, remove and return success */
	    *lm = tm->next;		/* unthread from list */
	    free(tm->name);
	    free(tm->cmdString);
	    free(tm);
	    return(0);
	}
    }
    return(-1);	/* failed to find entry to delete */
}

/* Function to remove all known menus */
gtxframe_ClearMenus(aframe)
struct gtx_frame *aframe; {

    register struct gtxframe_menu *tm, *nm;

    if (aframe->menus != (struct gtxframe_menu *)0) {
      for(tm=aframe->menus; tm; tm=nm) {
	nm = tm->next;
	free(tm->name);
	free(tm->cmdString);
	free(tm);
      }
    }

    aframe->menus = (struct gtxframe_menu *) 0;
    return 0;
}

gtxframe_AskForString(aframe, aprompt, adefault, aresult, aresultSize)
register struct gtx_frame *aframe;
char *aprompt;
char *adefault;
char *aresult;
long aresultSize; {
    register long code;
    register char *tp;

    /* Ensure recursive-edit map is initialized */
    SaveMap(aframe);

    /* Set up display */
    if (aframe->promptLine)
	free(aframe->promptLine);
    if (aframe->defaultLine)
	free(aframe->defaultLine);
    aframe->promptLine = gtx_CopyString(aprompt);
    tp = aframe->defaultLine = (char *) malloc(1024);
    if (tp == (char *)0)
      return(-1);
    if (adefault)
	strcpy(tp, adefault);
    else
	*tp = 0;

    /* Do recursive edit */
    gtx_InputServer(aframe->window);
    tp = aframe->defaultLine;	/* In case command reallocated it */

    /* Back from recursive edit, check out what's happened */
    if (aframe->flags & GTXFRAME_RECURSIVEERR) {
	code = -1;
	goto done;
    }
    code = strlen(tp);
    if (code+1 > aresultSize) {
	code = -2;
	goto done;
    }
    strcpy(aresult, tp);
    code = 0;

    /* Fall through to cleanup and return code */
  done:
    RestoreMap(aframe);
    if (aframe->promptLine)
	free(aframe->promptLine);
    if (aframe->defaultLine)
	free(aframe->defaultLine);
    aframe->defaultLine = aframe->promptLine = (char *) 0;
    if (code)
	gtxframe_DisplayString(aframe, "[Aborted]");
    return(code);
}

gtxframe_DisplayString(aframe, amsgLine)
char *amsgLine;
register struct gtx_frame *aframe; {
    if (aframe->messageLine)
      free(aframe->messageLine);
    aframe->messageLine = gtx_CopyString(amsgLine);
}

/* Called by input processor to try to clear the dude */
gtxframe_ClearMessageLine(aframe)
struct gtx_frame *aframe; {
    /* If we haven't shown message long enough yet, just return */
    if (aframe->flags & GTXFRAME_NEWDISPLAY)
      return(0);
    if (aframe->messageLine)
	free(aframe->messageLine);
    aframe->messageLine = (char *) 0;
    return(0);
}

static ShowMessageLine(aframe)
struct gtx_frame *aframe; {
    struct gwin_strparams strparms;
    struct gwin_sizeparams sizeparms;
    register char *tp;

    if (!aframe->window) return -1;

    /* First, find window size */
    WOP_GETDIMENSIONS(aframe->window, &sizeparms);

    if (aframe->promptLine) {
	bzero(&strparms, sizeof(strparms));
	strparms.x = 0;
	strparms.y = sizeparms.maxy-1;
	strparms.highlight = 1;
	tp = strparms.s = (char *) malloc(1024);
	strcpy(tp, aframe->promptLine);
	strcat(tp, aframe->defaultLine);
	WOP_DRAWSTRING(aframe->window, &strparms);
	aframe->flags |= GTXFRAME_NEWDISPLAY;
    }
    else if (aframe->messageLine) {
	/* Otherwise we're visible, print the message at the bottom */
	bzero(&strparms, sizeof(strparms));
	strparms.highlight = 1;
	strparms.x = 0;
	strparms.y = sizeparms.maxy-1;
	strparms.s = aframe->messageLine;
	WOP_DRAWSTRING(aframe->window, &strparms);
	aframe->flags |= GTXFRAME_NEWDISPLAY;
    }
    return(0);
}

/* Exit function, returning whatever has been put in its argument */
int gtxframe_ExitCmd(a_exitValuep)
    char *a_exitValuep;

{ /*gtxframe_ExitCmd*/

    int exitval;	/*Value we've been asked to exit with*/

    /* This next call should be type independent! */
    gator_cursesgwin_cleanup(&gator_basegwin);

    exitval = *((int *)(a_exitValuep));
    exit(exitval);

} /*gtxframe_ExitCmd*/

struct gtx_frame *gtxframe_Create() {
    struct gtx_frame *tframe;
    struct keymap_map *newkeymap;
    struct keymap_state *newkeystate;

    /*
      * Allocate all the pieces first: frame, keymap, and key state.
      */
    tframe = (struct gtx_frame *) malloc (sizeof(struct gtx_frame));
    if (tframe == (struct gtx_frame *)0) {
      return((struct gtx_frame *)0);
    }

    newkeymap = keymap_Create();
    if (newkeymap == (struct keymap_map *)0) {
      /*
	* Get rid of the frame before exiting.
	*/
      free(tframe);
      return((struct gtx_frame *)0);
    }

    newkeystate = (struct keymap_state *)
      malloc(sizeof(struct keymap_state));
    if (newkeystate == (struct keymap_state *)0) {
      /*
	* Get rid of the frame AND the keymap before exiting.
	*/
      free(tframe);
      free(newkeymap);
      return((struct gtx_frame *)0);
    }

    /*
      * Now that all the pieces exist, fill them in and stick them in
      * the right places.
      */
    bzero(tframe, sizeof(struct gtx_frame));
    tframe->keymap   = newkeymap;
    tframe->keystate = newkeystate;
    keymap_InitState(tframe->keystate, tframe->keymap);
    keymap_BindToString(tframe->keymap,
			"\003",
			gtxframe_ExitCmd,
			"ExitCmd",
			(char *) (&gtxframe_exitValue));

    /*
      * At this point, we return successfully.
      */
    return(tframe);
}

gtxframe_Delete(aframe)
register struct gtx_frame *aframe; {
    keymap_Delete(aframe->keymap);
    free(aframe->keystate);
    if (aframe->messageLine)
      free(aframe->messageLine);
    free(aframe);
    return 0;
}

gtxframe_Display(aframe, awm)
struct gwin *awm;
register struct gtx_frame *aframe; {
    register struct gtxframe_dlist *tlist;
    register struct gtxframe_menu *tm;
    struct gwin_strparams strparms;

    /* Run through the menus, displaying them on the top line */
    *menubuffer = 0;
    for(tm = aframe->menus; tm; tm = tm->next) {
	strcat(menubuffer, tm->name);
	strcat(menubuffer, ":");
	strcat(menubuffer, tm->cmdString);
	strcat(menubuffer, " ");
    }
    if (menubuffer[0] != 0) {
      bzero(&strparms, sizeof(strparms));
      strparms.x = 0;
      strparms.y = 0;
      strparms.s = menubuffer;
      strparms.highlight = 1;
      WOP_DRAWSTRING(awm, &strparms);
    }

    /* Run through the display list, displaying all objects */
    for(tlist=aframe->display; tlist; tlist=tlist->next) {
	OOP_DISPLAY(((struct onode *)(tlist->data)));
    }

    /* Finally, show the message line */
    ShowMessageLine(awm->w_frame);
    return(0);
}

/* Add an object to a window's display list */
gtxframe_AddToList(aframe, aobj)
struct onode *aobj;
register struct gtx_frame *aframe; {
    register struct gtxframe_dlist *tlist;

    for(tlist = aframe->display; tlist; tlist=tlist->next) {
	if (tlist->data == (char *) aobj) {
	  /*
	    * Don't add the same thing twice.
	    */
	  return(-1);
	}
    }

    /*
      * OK, it's not alreadyt there.  Create a new list object, fill it
      * in, and splice it on.
      */
    tlist = (struct gtxframe_dlist *) malloc(sizeof(struct gtxframe_dlist));
    if (tlist == (struct gtxframe_dlist *)0)
      return(-1);
    tlist->data = (char *) aobj;
    tlist->next = aframe->display;
    aframe->display = tlist;
    return(0);
}

/* Remove an object from a display list, if it is already there */
gtxframe_RemoveFromList(aframe, aobj)
struct onode *aobj;
register struct gtx_frame *aframe; {
    register struct gtxframe_dlist *tlist, **plist;
    
    plist = &aframe->display;
    for(tlist = *plist; tlist; plist = &tlist->next, tlist = *plist) {
	if (tlist->data == (char *) aobj) {
	    *plist = tlist->next;
	    free(tlist);
	    return 0;
	}
    }
    return(-1);	/* Item not found */
}

/* Clear out everything on the display list for the given frame*/
gtxframe_ClearList(aframe)
register struct gtx_frame *aframe; {
    register struct gtxframe_dlist *tlist, *nlist;

    if (aframe->display != (struct gtxframe_dlist *)0) {
      /*
	* Throw away each display list structure (we have at least
	* one).
	*/
      for(tlist = aframe->display; tlist; tlist = nlist) {
	nlist = tlist->next;
	free(tlist);
      }
    }

    aframe->display = (struct gtxframe_dlist *) 0;
    return 0;
}
