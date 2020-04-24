/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: gtxtest.c,v $
 * Revision 1.1.12.1  1996/10/02  17:51:13  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:40:04  damon]
 *
 * Revision 1.1.7.3  1994/09/28  21:22:07  sasala
 * 	bos output incorrect -missing output, OT 12280, S12Y
 * 	[1994/09/28  21:20:17  sasala]
 * 
 * Revision 1.1.7.2  1994/08/03  16:12:33  ruby
 * 	Implement partial S12Y (Internationalization) for DFS commands and daemons
 * 	[1994/08/01  16:57:32  ruby]
 * 
 * Revision 1.1.7.1  1994/06/09  14:10:12  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:24:40  annie]
 * 
 * Revision 1.1.5.3  1993/01/19  16:03:21  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  14:13:07  cjd]
 * 
 * Revision 1.1.5.2  1992/11/24  17:52:00  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  18:05:31  bolinger]
 * 
 * Revision 1.1.3.3  1992/01/24  03:45:39  devsrc
 * 	Merging dfs6.3
 * 	[1992/01/24  00:13:44  devsrc]
 * 
 * Revision 1.1.3.2  1992/01/23  19:50:02  zeliff
 * 	Moving files onto the branch for dfs6.3 merge
 * 	[1992/01/23  18:34:31  zeliff]
 * 	Revision 1.1.1.3  1992/01/23  22:17:25  devsrc
 * 	Fixed logs
 * 
 * $EndLog$
*/
#include <dcedfs/gtxwindows.h>
#include <dcedfs/gtxobjects.h>
#include <dcedfs/gtxtextobj.h>
#include <dcedfs/gtxlightobj.h>
#include <dcedfs/gtxframe.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsgtxmac.h>
#include <dfsgtxsvc.h>
#include <dfsgtxmsg.h>

struct gtx_frame *frameA, *frameB;

struct onode *objectA, *objectB;
struct onode *lightA, *lightB;


dce_svc_handle_t   gtx_svc_handle;

void initialize_svc()
{
  error_status_t st;

  gtx_svc_handle = dce_svc_register(gtx_svc_table,  (idl_char *)"gtx",  &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot register svc 0x%lx\n", st);
  dce_svc_routing((unsigned char *) "NOTICE:STDOUT:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route NOTICE messages  0x%lx\n", st);
  dce_svc_routing((unsigned char *) "WARNING:STDOUT:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route WARNING messages  0x%lx\n", st);
  dce_svc_routing((unsigned char *) "ERROR:STDERR:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route ERROR messages  0x%lx\n", st);

  dce_msg_define_msg_table(gtx__table,
        sizeof gtx__table / sizeof gtx__table[0], &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Warning Cannot register gtx incore message table 0x%lx\n", st);
}



static int ChangeMenuCmd(awin, arock)
long arock;
register struct gwin *awin; {
    register struct gtx_frame *tf;
    register long code;

    tf = awin->w_frame;
    if (!tf) return -1;

    if (arock == 1) {
	gtxframe_ClearMenus(tf);
    }
    else if (arock == 2) {
	code = gtxframe_DeleteMenu(tf, "NewLabel");
	if (code) gtxframe_DisplayString(tf, "Can't delete menu!");
    }
    else if (arock == 3) {
	code = gtxframe_ClearMenus(tf);
	gtxframe_AddMenu(frameA, "FrameB", "b");
	gtxframe_AddMenu(frameA, "NewLabel", "c");
    }
    return 0;
}

static int ChangeListCmd(awin, arock)
long arock;
register struct gwin *awin; {
    register struct gtx_frame *tf;
    register long code;

    tf = awin->w_frame;
    if (!tf) return -1;

    if (arock == 1) {
	gtxframe_ClearList(tf);
    }
    else if (arock == 2) {
	code = gtxframe_RemoveFromList(tf, lightA);
	if (code) gtxframe_DisplayString(tf, "Can't delete light!");
    }
    else if (arock == 3) {
	code = gtxframe_ClearList(tf);
	gtxframe_AddToList(frameA, objectA);
	gtxframe_AddToList(frameA, lightA);
    }
    return 0;
}

static int NoCallCmd(awin)
struct gwin *awin; {
    gtxframe_DisplayString(awin->w_frame, "Function should be mapped on '$d', not 'd'");
    return 0;
}

static int ChangeCmd(awin)
struct gwin *awin; {
    char tbuffer[100];
    register long code;

    code = gtxframe_AskForString(awin->w_frame, "New object string: ", "TestNew",
			  tbuffer, sizeof(tbuffer));
    if (code == 0) {
	/* have new value, write it to object A */
	gator_text_Write(objectA, tbuffer, 0, 0, 0);
    }
    return 0;
}

static int StupidCmd(awin)
struct gwin *awin; {
    gtxframe_DisplayString(awin->w_frame, "You're already showing that frame!");
    return 0;
}

static int SwitchToACmd(awin)
struct gwin *awin; {
    gtxframe_SetFrame(awin, frameA);
    return 0;
}

static int SwitchToBCmd(awin)
struct gwin *awin; {
    gtxframe_SetFrame(awin, frameB);
    return 0;
}

main() {
    struct gwin *win;
    struct gator_textobj_params textcrparams;
    struct gator_light_crparams lightcrparams;
    register struct keymap_map *tmap;


    initialize_svc();


    win = gtx_Init(0, -1);

    /* create a couple of objects, a and b, and have the "a" and "b" keys
       switch the display from one to the other */
    textcrparams.onode_params.cr_type	     = GATOR_OBJ_TEXT;
    strcpy(textcrparams.onode_params.cr_name, "Text1-A");
    textcrparams.onode_params.cr_x	     = 30;
    textcrparams.onode_params.cr_y	     = 10;
    textcrparams.onode_params.cr_width	     = 35;
    textcrparams.onode_params.cr_height     =  7;
    textcrparams.onode_params.cr_window     = win;	/* ???? */
    textcrparams.onode_params.cr_home_obj   = (struct onode *)0;
    textcrparams.onode_params.cr_prev_obj   = (struct onode *)0;
    textcrparams.onode_params.cr_parent_obj = (struct onode *)0;
    textcrparams.onode_params.cr_helpstring = "Help string for text";
    textcrparams.maxEntries 	   =  7;
    textcrparams.maxCharsPerEntry = 35;

    objectA = gator_objects_create((struct onode_createparams *)(&textcrparams));
    gator_text_Write(objectA, "This is object A", 0, 0, 0);
    
    /* create a couple of objects, a and b, and have the "a" and "b" keys
       switch the display from one to the other */
    textcrparams.onode_params.cr_type	     = GATOR_OBJ_TEXT;
    strcpy(textcrparams.onode_params.cr_name, "Text2-B");
    textcrparams.onode_params.cr_x	     = 30;
    textcrparams.onode_params.cr_y	     = 10;
    textcrparams.onode_params.cr_width	     = 35;
    textcrparams.onode_params.cr_height     =  7;
    textcrparams.onode_params.cr_window     = win;	/* ???? */
    textcrparams.onode_params.cr_home_obj   = (struct onode *)0;
    textcrparams.onode_params.cr_prev_obj   = (struct onode *)0;
    textcrparams.onode_params.cr_parent_obj = (struct onode *)0;
    textcrparams.onode_params.cr_helpstring = "Help string for text";
    textcrparams.maxEntries 	   =  7;
    textcrparams.maxCharsPerEntry = 35;

    objectB = gator_objects_create((struct onode_createparams *)(&textcrparams));
    gator_text_Write(objectB, "This is object B", 0, 0, 0);

    lightcrparams.onode_params.cr_type	     = GATOR_OBJ_LIGHT;
    lightcrparams.onode_params.cr_x	     = 10;
    lightcrparams.onode_params.cr_y	     = 10;
    lightcrparams.onode_params.cr_width	     = 10;
    lightcrparams.onode_params.cr_height     = 10;
    lightcrparams.onode_params.cr_window     = win;	/* ???? */
    lightcrparams.onode_params.cr_home_obj   = (struct onode *)0;
    lightcrparams.onode_params.cr_prev_obj   = (struct onode *)0;
    lightcrparams.onode_params.cr_parent_obj = (struct onode *)0;
    lightcrparams.onode_params.cr_helpstring = "Help string for text";
    strcpy(lightcrparams.label, "Light-1");
    lightcrparams.label_x = 0;
    lightcrparams.label_y = 0;
    lightcrparams.flashfreq = 100;
    lightcrparams.appearance = GATOR_LIGHTMASK_INVVIDEO;
    lightA = gator_objects_create((struct onode_createparams *)(&lightcrparams));

    /* create basic frames */
    frameA = gtxframe_Create();
    frameB = gtxframe_Create();

    /* setup A's frame */
    gtxframe_ClearList(frameA);
    gtxframe_AddToList(frameA, objectA);
    gtxframe_AddToList(frameA, lightA);
    keymap_BindToString(frameA->keymap, "b", SwitchToBCmd, (char *) 0, (char *) 0);
    keymap_BindToString(frameA->keymap, "a", StupidCmd, (char *) 0, (char *) 0);
    keymap_BindToString(frameA->keymap, "c", ChangeCmd, (char *) 0, (char *) 0);
    keymap_BindToString(frameA->keymap, "\033a", ChangeMenuCmd, "ChangeMenu", (char *) 1);
    keymap_BindToString(frameA->keymap, "\033b", ChangeMenuCmd, "ChangeMenu", (char *) 2);
    keymap_BindToString(frameA->keymap, "\033c", ChangeMenuCmd, "ChangeMenu", (char *) 3);
    keymap_BindToString(frameA->keymap, "\0331", ChangeListCmd, "ChangeList", (char *) 1);
    keymap_BindToString(frameA->keymap, "\0332", ChangeListCmd, "ChangeList", (char *) 2);
    keymap_BindToString(frameA->keymap, "\0333", ChangeListCmd, "ChangeList", (char *) 3);
    gtxframe_AddMenu(frameA, "FrameB", "b");
    gtxframe_AddMenu(frameA, "NewLabel", "c");

    /* setup B's frame */
    gtxframe_ClearList(frameB);
    gtxframe_AddToList(frameB, objectB);
    keymap_BindToString(frameB->keymap, "a", SwitchToACmd, (char *) 0, (char *) 0);
    keymap_BindToString(frameB->keymap, "b", StupidCmd, (char *) 0, (char *) 0);
    keymap_BindToString(frameB->keymap, "d", NoCallCmd, (char *) 0, (char *) 0);
    keymap_BindToString(frameB->keymap, "d", (int (*)()) 0, (char *) 0, (char *) 0);
    keymap_BindToString(frameB->keymap, "\033d", NoCallCmd, (char *) 0, (char *) 0);
    gtxframe_AddMenu(frameB, "FrameA", "a");

    /* finally setup the first window */
    gtxframe_AddToList(frameA, objectA);
    gtxframe_SetFrame(win, frameA);

    /* play with maps for a while */
    tmap = (struct keymap_map *) keymap_Create();
    keymap_BindToString(tmap, "d", NoCallCmd, "test", (char *) 1);
    keymap_BindToString(tmap, "cd", NoCallCmd, "bozo", (char *) 0);
    keymap_Delete(tmap);

    gtx_InputServer(win);
}
