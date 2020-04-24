/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: scout.c,v $
 * Revision 1.1.830.1  1996/10/02  18:15:05  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:46:16  damon]
 *
 * $EndLog$
 */
/*
 * Copyright (C) 1991, 1995 Transarc Corporation
 * All Rights Reserved
 */

#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#include <dcedfs/gtxwindows.h>		/*Generic window package*/
#include <dcedfs/gtxobjects.h>		/*Object definitions*/
#include <dcedfs/gtxtextobj.h>		/*Text object interface*/
#include <dcedfs/gtxlightobj.h>	/*Light object interface*/
#include <dcedfs/gtxcurseswin.h>	/*Curses window package*/
#include <dcedfs/gtxdumbwin.h>		/*Dumb terminal window package*/
#include <dcedfs/gtxX11win.h>		/*X11 window package*/
#include <dcedfs/gtxframe.h>		/*Frame package*/
#include <dcedfs/cmd.h>		/*Command interpretation library*/
#include <dcedfs/fsprobe.h>	/*Interface for fsprobe module*/
#include <dcedfs/compat_errs.h>
#include <dcedfs/osi.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsscmmac.h>
#include <dfsscmsvc.h>
#include <dfsscmmsg.h>

extern struct hostent *hostutil_GetHostByName();
extern int gtx_InputServer();
extern int gethostname();

/*
 * Command line parameter indicies.
 */
#define P_SERVER    0
#define P_BASE	    1
#if 0
#define	P_PACKAGE   2
#endif /* 0 */
#define P_HOST	    2
#define P_FREQ	    3
#define P_ATTENTION 4
#define	P_DEBUG	    5

/* Debugging Output */
#ifdef DEBUG
#define DEBUG_FLAG  1
#else
#define DEBUG_FLAG  0
#endif

/*
  * Define the width in chars for each light object on the mini-line.
  */
#define LIGHTOBJ_CONN_WIDTH	 5
#define LIGHTOBJ_FETCH_WIDTH	10
#define LIGHTOBJ_STORE_WIDTH	10
#define LIGHTOBJ_WK_WIDTH	 5
#define LIGHTOBJ_SRVNAME_WIDTH	13
#define LIGHTOBJ_DISK_WIDTH	12

/*
 * Define the types of justification we can perform.
 */
#define SCOUT_RIGHT_JUSTIFY	0
#define SCOUT_LEFT_JUSTIFY	1
#define SCOUT_CENTER		2

/*
 * Define the types of trun<cation we can perform.
 */
#define SCOUT_LEFT_TRUNC	0
#define SCOUT_RIGHT_TRUNC	1

/*
 * Define whether the value passed is a labeled disk quantity.
 */
#define SCOUT_ISNT_LDISK	0
#define SCOUT_IS_LDISK		1

/*
  * Max number of disks we receive info about.
  */
#define SCOUT_MAX_DISKS	16

/*
  * We sometimes use index-base pointers, so we need a distinguished
  * NIL value.
  */
#define SCOUT_NIL  (-1)

/*
  * Structure describing everything you want to know about a FileServer
  * disk.
  */
#define MAX_DISK_NAME 32
struct scout_disk {
    int prev;				/*Index of previous list entry*/
    int next;				/*Index of next list entry*/
    int active;				/*Is this disk known to exist?*/
    char name[MAX_DISK_NAME];		/*disk name*/
    struct onode *disk_lp;		/*Ptr to disk light object*/
};

/*
  * Structure defining all the objects in the Scout line (or lines)
  * for each server being watched.  Note that scout_disk linked list
  * for used disks is ordered alphabetically.
  */
struct mini_line {
    /*
      * Information on server location & configuration.
      */
    rpc_binding_handle_t h;             /* Server's handle */
    int numDisks;			/*Number of disks used*/
    /*
      * Screen location info.
      */
    int base_line;			/*Line number on the screen*/
    int num_lines;			/*Number of lines of info*/
    /*
      * Associated light objects.
      */
    struct onode *currConns_lp;		/*Number of current connections*/
    struct onode *fetches_lp;		/*Number of data fetches*/
    struct onode *stores_lp;		/*Number of data stores*/
    struct onode *workstations_lp;	/*Workstation info*/
    struct onode *srvName_lp;		/*Server name*/
    struct scout_disk disks[SCOUT_MAX_DISKS];	/*Info on all the disks*/
    int used_head;			/*Index of first used disk struct*/
    int used_tail;			/*Index of last used disk struct*/
    int free_head;			/*Index of first free disk struct*/
    int free_tail;			/*Index of last free disk struct*/
};

/*
  * Structure defining the contents of the Scout screen.
  */
struct mini_screen {
    int numServers;		/*Number of servers being watched*/
    int base_line_num;		/*Base mini-line number*/
    struct mini_line *line;	/*Array of screen lines*/
};

static char pn[] = "scout";		/*Program name*/
static int scout_debug = 0;		/*Is debugging turned on?*/
static FILE *scout_debugfd;		/*Debugging file descriptor*/
static int scout_gtx_initialized = 0;	/*Has gtx been initialized?*/
static struct mini_screen scout_screen;	/*Mini-screen itself*/
static int scout_probefreq;		/*Probe frequency in seconds*/
static char scout_basename[64];		/*Base server name*/
static char scout_hostname[128];	/*Name of machine we're running on*/
static int scout_showhostname = 0;	/*Show name of machine we're on?*/
static char scout_blankline[256];	/*Blank line*/
static struct gwin *scout_gwin;		/*Window to use*/
static struct gtx_frame *scout_frame;	/*Frame to use*/
static struct onode *scout_banner0_lp;	/*Banner light, line 0*/
static struct onode *scout_banner1_lp;	/*Banner light, line 1*/
static struct onode *scout_banner2_lp;	/*Banner light, line 2*/
static int scout_DiskLightLeftCol = 0;	/*Column for leftmost disk light*/
static struct gwin_sizeparams scout_frameDims;	/*Frame dimensions*/

/*
  * Attention thresholds & modes.
  */
#define SCOUT_ATTN_NOTUSED  (-1)
#define SCOUT_DISKM_PCUSED    0
#define SCOUT_DISKM_MINFREE   1

static int   scout_attn_conn	          = SCOUT_ATTN_NOTUSED;
static int   scout_attn_fetch	          = SCOUT_ATTN_NOTUSED;
static int   scout_attn_store	          = SCOUT_ATTN_NOTUSED;
static int   scout_attn_workstations      = SCOUT_ATTN_NOTUSED;
static int   scout_attn_disk_mode         = SCOUT_DISKM_PCUSED;
static int   scout_attn_disk_minfree      = 1000;
static float scout_attn_disk_pcused       = 0.95;
static char  scout_attn_disk_pcusedstr[8] = "95";

/*
  * Some strings we'll be using over and over again.
  */
static char scout_Banner[256];
static char scout_LightLabels[] =
  "Conn     Fetch      Store    Ws      Server     Disk attn:";
static char scout_LightLabelUnd[] =
  "----    -------    -------  ----  ------------  ----------";

static char **server_names;

#define   DFS_INCORE_MESSAGE_TABLE(table)                                \
                                                                         \
  dce_msg_define_msg_table(table,                                        \
			   sizeof table / sizeof table[0],               \
			   &st);                                         \
                                                                         \
  if (st != svc_s_ok)                                                    \
                                                                         \
    fprintf(stderr,                                                      \
	    "Warning Cannot register %s incore message table 0x%lx\n",   \
	    table,                                                       \
	    st);

DCE_SVC_DEFINE_HANDLE(scm_svc_handle, scm_svc_table, "scm")

void initialize_svc()
{
  error_status_t st;

  DFS_INCORE_MESSAGE_TABLE(scm__table)

  dce_svc_routing((unsigned char *) "WARNING:STDOUT:--", &st);

  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route WARNING messages  0x%lx\n", st);

  dce_svc_routing((unsigned char *) "ERROR:STDERR:--", &st);

  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route ERROR messages  0x%lx\n", st);
}

/*------------------------------------------------------------------------
 * scout_CleanExit
 *
 * Description:
 *	Exits cleanly from the Scout.  If gtx has not yet been initialized,
 *	then we simply call exit() with the value provided.  Otherwise,
 *	we call the appropriate gtx routine to exit cleanly from gtx, which
 *	must reset the terminal or window.  We also close the debugging
 *	file descriptor, if one has been opened.
 *
 * Arguments:
 *	int a_exitval : Value with which to exit the program.
 *
 * Returns:
 *	Void.
 *
 * Environment:
 *	Actions depend on scout_gtx_initialized.  
 *
 * Side Effects:
 *	This routine will always exit Scout.
 *------------------------------------------------------------------------*/

static void scout_CleanExit(a_exitval)
    int a_exitval;

{ /*scout_CleanExit*/

    static char rn[] = "scout_CleanExit"; /*Routine name*/

    if (scout_debugfd != (FILE *)0) {
      dce_svc_printf(SCM_S_CLOSE_DEBUG_FILE_MSG, rn);
      fclose(scout_debugfd);
    }

    if (scout_gtx_initialized) {
      sleep (10);     /* give a change for user's to read messages in
			 stdout and stderr */
      gtxframe_exitValue = a_exitval;
      gtxframe_ExitCmd((char *)(&gtxframe_exitValue));
    }
    else
      exit(a_exitval);

} /*scout_CleanExit*/

/*------------------------------------------------------------------------
 * mini_initLightObject
 *
 * Description:
 *	Create and initialize a light onode according to the given
 *	parameters.
 *
 * Arguments:
 *	char *a_name	   : Ptr to the light's string name.
 *	int a_x		   : X offset.
 *	int a_y		   : Y offset.
 *	int a_width	   : Width in chars.
 *	struct gwin *a_win : Ptr to window structure.
 *
 * Returns:
 *	Ptr to new light onode on success,
 *	A null pointer otherwise.
 *
 * Environment:
 *	See above.
 *
 * Side Effects:
 *	As advertised.
 *------------------------------------------------------------------------*/

static struct onode *mini_initLightObject(a_name, a_x, a_y, a_width, a_win)
    char *a_name;
    int a_x;
    int a_y;
    int a_width;
    struct gwin *a_win;

{ /*mini_initLightObject*/

    static char rn[] = "mini_initLightObject";	/*Routine name*/
    struct onode *newlightp;			/*Ptr to new light onode*/
    /*We only support curses right now*/
    struct gator_light_crparams light_crparams; /*Light creation params*/
    char *truncname;				/*Truncated name, if needed*/
    int name_len;				/*True length of name*/

    if (scout_debug) {
      dce_svc_printf(SCM_S_CALL_FOR_ONODE_MSG, rn, a_name, a_x, a_y, a_width);
      fflush(scout_debugfd);
    }
    newlightp = (struct onode *)0;

    /*
      * Set up the creation parameters according to the information we've
      * received.
      */
    light_crparams.onode_params.cr_type = GATOR_OBJ_LIGHT;
    name_len = strlen(a_name);
    if (scout_debug)
      dce_svc_printf(SCM_S_NAME_OF_ONODE_MSG, rn, a_name, name_len);
    if (name_len <= a_width)
      sprintf(light_crparams.onode_params.cr_name, "%s", a_name);
    else {
      /*
	* We need to truncate the given name, leaving a `*' at the end to
	* show us it's been truncated.
	*/
      truncname = light_crparams.onode_params.cr_name;
      strncpy(truncname, a_name, a_width-1);
      truncname[a_width-1] = '*';
      truncname[a_width]   = 0;
    }
    light_crparams.onode_params.cr_x          = a_x;
    light_crparams.onode_params.cr_y          = a_y;
    light_crparams.onode_params.cr_width      = a_width;
    light_crparams.onode_params.cr_height     = 1;
    light_crparams.onode_params.cr_window     = a_win;
    light_crparams.onode_params.cr_home_obj   = (struct onode *)0;
    light_crparams.onode_params.cr_prev_obj   = (struct onode *)0;
    light_crparams.onode_params.cr_parent_obj = (struct onode *)0;
    light_crparams.onode_params.cr_helpstring = (char *)0;

    light_crparams.appearance = 0;
    light_crparams.flashfreq  = 0;
    sprintf(light_crparams.label, "%s", a_name);
    light_crparams.label_x = 0;
    light_crparams.label_y = 0;

    newlightp =
      gator_objects_create((struct onode_createparams *)(&light_crparams));

    /*
      * Return the news, happy or not.
      */
    return(newlightp);

} /*mini_initLightObject*/

/*------------------------------------------------------------------------
 * scout_initDiskLightObjects
 *
 * Description:
 *	Create and initialize all Scout light objects for a server's
 *	disks.
 *
 * Arguments:
 *	struct scout_disk *a_line : Ptr to the server object line.
 *	struct gwin *a_win	  : Ptr to window structure.
 *
 * Returns:
 *	0 on success,
 *	-1 otherwise.
 *
 * Environment:
 *	See above.
 *
 * Side Effects:
 *	As advertised.
 *------------------------------------------------------------------------*/

static int scout_initDiskLightObjects(a_line, a_win)
    struct mini_line *a_line;
    struct gwin *a_win;

{ /*scout_initDiskLightObjects*/

    static char rn[] =
      "scout_initDiskLightObjects";	/*Routine name*/
    struct scout_disk *curr_disk;	/*Ptr to current disk being set up*/
    int i;				/*Loop variable*/

    if (scout_debug) {
      dce_svc_printf(SCM_S_ENTERING_SCOUT_INITDISKLIGHTOBJECTS_MSG, rn);
      fflush(scout_debugfd);
    }

    /*
      * Set up the base linked list fields.
      */
    a_line->used_head = SCOUT_NIL;
    a_line->used_tail = SCOUT_NIL;
    a_line->free_head = 0;
    a_line->free_tail = SCOUT_MAX_DISKS - 1;

    /*
      * Sweep through the disk structures, creating the light objects and
      * marking them all as free.
      */
    curr_disk = a_line->disks;
    for (i=0; i < SCOUT_MAX_DISKS; i++) {
      /*
	* Create the disk light object.
	*/
      if ((curr_disk->disk_lp =
	 mini_initLightObject("Disk",			/*Object name*/
			      0,			/*X value*/
			      0,			/*Y value*/
			      LIGHTOBJ_DISK_WIDTH,	/*Width*/
			      a_win))			/*Window*/
	  == (struct onode *)0) {
	dce_svc_printf(SCM_S_BAD_LIGHT_OBJECT_CREATE_MSG, pn, rn, i);
	return(-1);
      }

      /*
	* Set the other fields in the disk records; Note that in the
	* fencepost cases, the prev and next pointers will have to be
	* fixed.
	*/
      curr_disk->prev   = i-1;
      curr_disk->next   = i+1;
      curr_disk->active = 0;
      bzero(curr_disk->name, sizeof(curr_disk->name));

      /*
	* Bump up to the next disk structure.
	*/
      curr_disk++;

    } /*for each disk structure*/

    /*
      * We made it all the way through.  Fix the fencepost pointers, set
      * the overall pointers, then return success.
      */
    a_line->disks[0].prev		  = SCOUT_NIL;
    a_line->disks[SCOUT_MAX_DISKS-1].next = SCOUT_NIL;
    
    return(0);

} /*scout_initDiskLightObjects*/


/*------------------------------------------------------------------------
 * mini_justify
 *
 * Description:
 *	Place the chars in the source buffer into the target buffer
 *	with the desired justification, either centered, left-justified
 *	or right-justified.  Also, support inidication of truncation
 *	with a star (*), either on the left or right of the string,
 *	and whether we're justifying a labeled disk quantity.
 *
 * Arguments:
 *	char *a_srcbuff	    : Ptr to source char buffer.
 *	char *a_dstbuff	    : Ptr to dest char buffer.
 *	int a_dstwidth	    : Width of dest buffer in chars.
 *	int a_justification : Kind of justification.
 *	int a_rightTrunc    : If non-zero, place the truncation char
 *			      on the right of the string.  Otherwise,
 *			      place it on the left.
 *	int a_isLabeledDisk : Is this a labeled disk quantity?
 *
 * Returns:
 *	0 on success,
 *	-1 otherwise.
 *
 * Environment:
 *	All it needs to know is exported by the fsprobe module, namely
 *	the data structure where the probe results are stored.
 *
 * Side Effects:
 *	As advertised.
 *------------------------------------------------------------------------*/

int mini_justify(a_srcbuff, a_dstbuff, a_dstwidth, a_justification, a_rightTrunc, a_isLabeledDisk)
    char *a_srcbuff;
    char *a_dstbuff;
    int a_dstwidth;
    int a_justification;
    int a_rightTrunc;
    int a_isLabeledDisk;

{ /*mini_justify*/

    static char rn[] = "mini_justify";	/*Routine name*/
    int leftpad_chars;			/*# of chars for left-padding*/
    int num_src_chars;			/*# of chars in source*/
    int true_num_src_chars;		/*# src chars before truncation*/
    int trunc_needed;			/*Is truncation needed?*/
    char *diskChar;			/*Disk name prefix*/

    if (scout_debug) {
      dce_svc_printf(SCM_S_CALLED_MINI_JUSTIFY_MSG, rn, a_srcbuff, a_dstwidth);
      fflush(scout_debugfd);
    }

    /*
     * Remember the disk label, if we've been passed such a thing.
     */
    if (a_isLabeledDisk)
	diskChar = a_srcbuff;

    /*
     * If the destination width will overrun the gtx string storage,
     * we automatically shorten up.
     */
    if (a_dstwidth > GATOR_LABEL_CHARS) {
      if (scout_debug) {
	dce_svc_printf(SCM_S_SET_DESTINATION_WIDTH_MSG, rn, a_dstwidth, GATOR_LABEL_CHARS);
	fflush(scout_debugfd);
      }
      a_dstwidth = GATOR_LABEL_CHARS;
    }

    /*
     * If our source string is too long, prepare for truncation.
     */
    true_num_src_chars = strlen(a_srcbuff);
    if (true_num_src_chars >= a_dstwidth) {
	trunc_needed = 1;
	num_src_chars = a_dstwidth - 1;
	leftpad_chars = 0;
	if (!a_rightTrunc)
	    a_srcbuff += (true_num_src_chars - num_src_chars);
    }
    else {
	trunc_needed = 0;
	num_src_chars = true_num_src_chars;
	
	/*
	 * Compute the necessary left-padding.
	 */
	switch (a_justification) {
	    
	  case SCOUT_RIGHT_JUSTIFY:
	    leftpad_chars = (a_dstwidth - 1) - num_src_chars;
	    break;
	    
	  case SCOUT_LEFT_JUSTIFY:
	    /*
	     * This is the really easy one.
	     */
	    leftpad_chars = 0;
	    break;
	    
	  case SCOUT_CENTER:
	    leftpad_chars = ((a_dstwidth - 1) - num_src_chars) / 2;
	    break;
	    
	  default:
	    dce_svc_printf(SCM_S_ILLEGAL_JUSTIFICATION_COMMAND_MSG, rn, a_justification);
	    return(-1);
	} /*Switch on justification type*/
    }

    /*
     * Clear out the dest buffer, then place the source string at the
     * appropriate padding location.  Remember to place a string
     * terminator at the end of the dest buffer, plus whatever truncation
     * may be needed.  If we're left-truncating, we've already shifted
     * the src buffer appropriately.
     */
    strncpy(a_dstbuff, scout_blankline, a_dstwidth);
    strncpy(a_dstbuff+leftpad_chars, a_srcbuff, num_src_chars);
    *(a_dstbuff+a_dstwidth-1) = '\0';
    if (trunc_needed) {
	if (a_rightTrunc)
	    *(a_dstbuff+a_dstwidth-2) = '*'; /*Truncate on the right*/
	else {
	    if (a_isLabeledDisk) {
		strcpy(a_dstbuff, diskChar);
		*(a_dstbuff+strlen(diskChar)) = ':';
		*(a_dstbuff+strlen(diskChar)+1) = '*';/*Truncate on the left, disk*/
	    }
	    else
		*a_dstbuff = '*';	     /*Truncate on the left, non-disk*/
	}
    } /*Handle truncations*/

    /*
     * Return the good news.
     */
    return(0);

} /*mini_justify*/

/*------------------------------------------------------------------------
 * scout_SetNonDiskLightLine
 *
 * Description:
 *	Given a mini-line and a line number, place all non-disk lights
 *	on that line.
 *
 * Arguments:
 *	struct mini_line *a_srvline : Ptr to server descriptor.
 *	int a_linenum		    : Line number to move to.
 *
 * Returns:
 *	Nothing.
 *
 * Environment:
 *	The light's location is stored not only in the onode, but in
 *	the light's rock, so we have to change both sets of value.
 *
 * Side Effects:
 *	As described.
 *------------------------------------------------------------------------*/

static void scout_SetNonDiskLightLine(a_srvline, a_linenum)
    struct mini_line *a_srvline;
    int a_linenum;

{ /*scout_SetNonDiskLightLine*/

    static char rn[] =
      "scout_SetNonDiskLightLine";		/*Routine name*/
    struct gator_lightobj *nondisk_lightdata;	/*Non-disk light data field*/
    struct gwin_strparams *nondisk_strparams;	/*Associated string params*/

    /*
      * Do the exact same operation for each non-disk light in the record.
      */
    a_srvline->currConns_lp->o_y = a_linenum;
    nondisk_lightdata =
      (struct gator_lightobj *)(a_srvline->currConns_lp->o_data);
    nondisk_strparams =
      (struct gwin_strparams *)(nondisk_lightdata->llrock);
    nondisk_strparams->y = a_linenum;

    a_srvline->fetches_lp->o_y = a_linenum;
    nondisk_lightdata =
      (struct gator_lightobj *)(a_srvline->fetches_lp->o_data);
    nondisk_strparams =
      (struct gwin_strparams *)(nondisk_lightdata->llrock);
    nondisk_strparams->y = a_linenum;

    a_srvline->stores_lp->o_y = a_linenum;
    nondisk_lightdata =
      (struct gator_lightobj *)(a_srvline->stores_lp->o_data);
    nondisk_strparams =
      (struct gwin_strparams *)(nondisk_lightdata->llrock);
    nondisk_strparams->y = a_linenum;

    a_srvline->workstations_lp->o_y = a_linenum;
    nondisk_lightdata =
      (struct gator_lightobj *)(a_srvline->workstations_lp->o_data);
    nondisk_strparams =
      (struct gwin_strparams *)(nondisk_lightdata->llrock);
    nondisk_strparams->y = a_linenum;

    a_srvline->srvName_lp->o_y = a_linenum;
    nondisk_lightdata =
      (struct gator_lightobj *)(a_srvline->srvName_lp->o_data);
    nondisk_strparams =
      (struct gwin_strparams *)(nondisk_lightdata->llrock);
    nondisk_strparams->y = a_linenum;

} /*scout_SetNonDiskLightLine*/

/*------------------------------------------------------------------------
 * scout_RecomputeLightLocs
 *
 * Description:
 *	Given a pointer to a server record, recompute its disk light
 *	locations (and keep proper track of the number of screen lines
 *	required for the server record).
 *
 * Arguments:
 *	struct mini_line *a_srvline : Ptr to server descriptor.
 *
 * Returns:
 *	0 if anything went wrong,
 *	else the number of lines used by this server record.
 *
 * Environment:
 *	One or more records have joined or left the used light list
 *	for this server.  We're not sure which ones, so we recompute
 *	them all.  We may also have had a width change in the gtx
 *	frame.  The base_line field in the server record is guaranteed
 *	to be correct at this point.
 *
 * Side Effects:
 *	As described.
 *------------------------------------------------------------------------*/

static int scout_RecomputeLightLocs(a_srvline)
    struct mini_line *a_srvline;

{ /*scout_RecomputeLightLocs*/

    static char rn[] =
      "scout_RecomputeLightLocs";	/*Routine name*/
    int lights_per_line;		/*# lights/line*/
    int lights_this_line;		/*# lights on cur line*/
    int curr_idx;			/*Current disk light idx*/
    struct scout_disk *sc_disk;		/*Ptr to disk record array*/    
    int lines_for_server;		/*Num lines in server record*/
    int curr_line;			/*Current line being filled*/
    int curr_x;				/*Current x value for light*/
    struct gator_lightobj *disk_lightdata; /*Disk light data field*/
    struct gwin_strparams *disk_strparams; /*String params for disk light*/

    if (scout_debug) {
      dce_svc_printf(SCM_S_CALLED_SCOUT_RECOMPUTELIGHTLOCS_MSG, rn);
      fflush(scout_debugfd);
    }

    /*
      * If we haven't yet computed the column for the leftmost disk light,
      * do it now.
      */
    if (scout_DiskLightLeftCol == 0)
      scout_DiskLightLeftCol =
	LIGHTOBJ_CONN_WIDTH  +
	LIGHTOBJ_FETCH_WIDTH +
	LIGHTOBJ_STORE_WIDTH +
	LIGHTOBJ_WK_WIDTH    +
	LIGHTOBJ_SRVNAME_WIDTH + 5;

    /*
      * Calculate how many disk light objects can fit in one line.
      */
    lights_per_line =
      (scout_frameDims.maxx - scout_DiskLightLeftCol)/(LIGHTOBJ_DISK_WIDTH + 1);
    if (scout_debug) {
      dce_svc_printf(SCM_S_NUMBER_LIGHTS_PER_LINE_MSG, rn, lights_per_line);
      fflush(scout_debugfd);
    }

    /*
      * Sweep through the used disk light list, computing the location
      * for each.
      */
    lights_this_line = 0;
    curr_idx	     = a_srvline->used_head;
    lines_for_server = 1;
    curr_line	     = a_srvline->base_line;
    curr_x	     = scout_DiskLightLeftCol;

    while (curr_idx != SCOUT_NIL) {
      /*
	* Bump the number of lines for the server if we've already
	* filled up the current line.
	*/
      if (lights_this_line >= lights_per_line) {
	lights_this_line = 0;
	lines_for_server++;
	curr_line++;
	curr_x = scout_DiskLightLeftCol;
      } /*Current line filled up*/

      /*
	* Set the current disk light's location.
	*/
      sc_disk = a_srvline->disks;
      sc_disk[curr_idx].disk_lp->o_x = curr_x;
      sc_disk[curr_idx].disk_lp->o_y = curr_line;
      disk_lightdata =
	(struct gator_lightobj *)(sc_disk[curr_idx].disk_lp->o_data);
      disk_strparams =
	(struct gwin_strparams *)(disk_lightdata->llrock);
      disk_strparams->x = curr_x;
      disk_strparams->y = curr_line;
      if (scout_debug) {
	dce_svc_printf(SCM_S_DISK_LIGHT_LOCATION_MSG, rn, curr_idx, curr_x, curr_line);
	fflush(scout_debugfd);
      }

      /*
	* Record the inclusion of the light and move on to the next
	* light, if any.
	*/
      lights_this_line++;
      curr_x += LIGHTOBJ_DISK_WIDTH + 1;
      curr_idx = sc_disk[curr_idx].next;

    } /*Update each used disk light*/

    /*
      * Remember to record the (possibly new) number of lines in the
      * server record before returning the value of that field.
      */
    if (a_srvline->num_lines != lines_for_server) {
	if (scout_debug) {
	    dce_svc_printf(SCM_S_CHANGED_SERVER_LINES_MSG, rn, a_srvline->num_lines);
	    fflush(scout_debugfd);
	}
      a_srvline->num_lines = lines_for_server;
    }

    if (scout_debug) {
      dce_svc_printf(SCM_S_NEW_SERVER_LINES_MSG, rn, a_srvline->num_lines);
      fflush(scout_debugfd);
    }
    return(a_srvline->num_lines);

} /*scout_RecomputeLightLocs*/

/*------------------------------------------------------------------------
 * scout_FindUsedDisk
 *
 * Description:
 *	Given a single-letter disk name and a pointer to the current server
 *	record, look for a used disk record with that name within the server.
 *	If we can't find one, we create and incorporate one, and return that
 *	fact to our caller.
 *
 * Arguments:
 *	char *a_diskname		    : disk name.
 *	struct mini_line *a_srvline : Ptr to server descriptor.
 *	int *a_record_added	    : Was a new record added?
 *
 * Returns:
 *	Index of matching used disk record,
 *	SCOUT_NIL otherwise.
 *
 * Return via parameter:
 *	a_record_added set to 1 iff record was added to the used disk list.
 *
 * Environment:
 *	Used disk records are kept in alphabetical order by the single-char
 *	disk name.  Should a matching used disk record not be found, one is
 *	pulled from the free pool.
 *
 * Side Effects:
 *	An entry may be pulled off the free list and inserted into the
 *	used list.  This entry is placed in the update list for the
 *	current gtx frame (at least not by this routine).
 *------------------------------------------------------------------------*/

static int scout_FindUsedDisk(a_diskname, a_srvline, a_record_added)
    char *a_diskname;
    struct mini_line *a_srvline;
    int *a_record_added;

{ /*scout_FindUsedDisk*/

    static char rn[] = "scout_FindUsedDisk";	/*Routine name*/
    int curr_idx;				/*Disk record index*/
    int append_idx;				/*Index to append after*/
    int new_idx;				/*Index of new used record*/
    struct scout_disk *sc_disk;			/*Ptr to disk record*/
    int code;					/*Function return value*/

    if (scout_debug) {
      dce_svc_printf(SCM_S_CALLED_SCOUT_FINDUSEDDISK_MSG, rn);
      fflush(scout_debugfd);
    }

    /*
      * Sweep through the used disk records, looking for a match.
      */
    curr_idx   = a_srvline->used_head;
    append_idx = SCOUT_NIL;
    sc_disk    = a_srvline->disks;
    if (scout_debug) {
      dce_svc_printf(SCM_S_SCANNING_USED_DISK_ENTRIES_MSG, rn, a_diskname);
      fflush(scout_debugfd);
    }
    while (curr_idx != SCOUT_NIL) {
      if (scout_debug) {
	dce_svc_printf(SCM_S_DISK_NAME_MSG, rn, curr_idx, sc_disk[curr_idx].name);
	fflush(scout_debugfd);
      }
      if (strcmp(sc_disk[curr_idx].name, a_diskname) == 0) {
	/*
	  * We found it!  Bug out.
	  */
	if (scout_debug) {
	  dce_svc_printf(SCM_S_FOUND_DISK_NAME_MSG, rn);
	  fflush(scout_debugfd);
	}
	return(curr_idx);
      }
#ifdef notdef 
       /* Since we deal with strings now, can't depend on order */
      /*
	 * If we are alphabetically past the given disk name, we
	 * know that we won't find it in the used disk list; we
	 * also have the append index set correctly.
	 */
      if (a_diskname < sc_disk[curr_idx].name) {
	if (scout_debug) {
	  dce_svc_printf(SCM_S_DISK_NOT_IN_LIST_MSG, rn, a_diskname);
	  fflush(scout_debugfd);
	}
	break;
      }
#endif
      /*
	* There's still hope we'll find it.  Move on to the next used
	* disk record, keeping this index as the best candidate so far
	* for appending a missing entry.
	*/
      append_idx = curr_idx;
      curr_idx   = sc_disk[curr_idx].next;
    } /*Look for match*/

    /*
      * We didn't find the record we wanted, which means we'll pull a
      * record out of the free pool for it.  If we didn't find a place
      * to append it, we then insert it at the beginning of the queue.
      */
    if (a_srvline->free_head == SCOUT_NIL)
      return(SCOUT_NIL);
    *a_record_added = 1;
    new_idx = a_srvline->free_head;
    if (scout_debug) {
      dce_svc_printf(SCM_S_CHOOSE_FREE_INDEX_MSG, rn, new_idx);
      fflush(scout_debugfd);
    }
    a_srvline->free_head = sc_disk[new_idx].next;
    if (a_srvline->free_head == SCOUT_NIL)
      a_srvline->free_tail = SCOUT_NIL;

	/* fix prev in new free_head to be NIL */

	if(a_srvline->free_head!=SCOUT_NIL)
		a_srvline->disks[a_srvline->free_head].prev=SCOUT_NIL;


    /*
      * Fill in the new record.
      */
    sc_disk[new_idx].active = 0;
    strcpy(sc_disk[new_idx].name, a_diskname);

    /*
      * Insert the new record where it belongs on the used disk list.
      */
    if (append_idx == SCOUT_NIL) {
      /*
	* It belongs at the beginning of the list.
	*/
      if (scout_debug) {
	dce_svc_printf(SCM_S_INSERT_AT_HEAD_MSG, rn);
	fflush(scout_debugfd);
      }

	/* fix prev in old used_head to be new used_head */

	if(a_srvline->used_head!=SCOUT_NIL)
		a_srvline->disks[a_srvline->used_head].prev=new_idx;

      sc_disk[new_idx].next = a_srvline->used_head;
      sc_disk[new_idx].prev = SCOUT_NIL;
      a_srvline->used_head  = new_idx;
      if (a_srvline->used_tail == SCOUT_NIL)
	a_srvline->used_tail = new_idx;
    }
    else {
      if (scout_debug) {
	dce_svc_printf(SCM_S_INSERTED_AFTER_INDEX_MSG, rn, append_idx);
	fflush(scout_debugfd);
      }
      sc_disk[new_idx].prev = append_idx;
      sc_disk[new_idx].next = sc_disk[append_idx].next;
      sc_disk[append_idx].next = new_idx;
      if (sc_disk[new_idx].next == SCOUT_NIL)
	a_srvline->used_tail = new_idx;
      else
	sc_disk[sc_disk[new_idx].next].prev = new_idx;
    };

    /*
      * Add the new used disk light object to the display list for
      * the scout frame.
      */
    if (scout_debug) {
      dce_svc_printf(SCM_S_ADD_AT_INDEX_MSG, rn, new_idx);
      fflush(scout_debugfd);
    }
    code = gtxframe_AddToList(scout_frame, sc_disk[new_idx].disk_lp);
    if (code) {
      if (scout_debug) {
	dce_svc_printf(SCM_S_CANT_ADD_TO_DISPLAY_LIST_MSG, rn, code);
	fflush(scout_debugfd);
      }
    }
    return(new_idx);

} /*scout_FindUsedDisk*/

/*------------------------------------------------------------------------
 * scout_RemoveInactiveDisk
 *
 * Description:
 *	Given a server record and a used disk index, remove the disk
 *	record from the used list, put it on the free list, and remove
 *	it from the gtx frame update list.
 *
 * Arguments:
 *	struct mini_line *a_srvline	: Ptr to server descriptor.
 *	int a_used_idx			: Index of used disk record.
 *
 * Returns:
 *	Nothing.
 *
 * Environment:
 *	Formerly-used disk records are returned to the free pool.
 *
 * Side Effects:
 *	Free and used disk record lists are modified for this server.
 *	The disk record in question is pulled off the gtx update list
 *	for the frame.
 *------------------------------------------------------------------------*/

scout_RemoveInactiveDisk(a_srvline, a_used_idx)
    struct mini_line *a_srvline;
    int a_used_idx;

{ /*scout_RemoveInactiveDisk*/

	int error;

    static char rn[] = "scout_RemoveInactiveDisk";	/*Routine name*/

    if (scout_debug) {
      dce_svc_printf(SCM_S_CALLED_SCOUT_REMOVEINACTIVEDISK_MSG, rn);
      fflush(scout_debugfd);
    }

	/* code to free the disk structure */

	error=free_disk(a_srvline,a_used_idx);

	if(error<0)
		return(-1);

	/* remove the disk object from the display list */

	error=gtxframe_RemoveFromList(scout_frame,
		a_srvline->disks[a_used_idx].disk_lp);

	if(error<0)
		return(-1);

	return(0);
	
    /*code = gtxframe_RemoveFromList(scout_frame->window, lightobj);*/

}/*scout_RemoveInactiveDisk*/

free_disk(pline,i)
struct mini_line *pline;
{
	int j;

	/* free already? */

	for(j=pline->free_head;j<SCOUT_MAX_DISKS;j=pline->disks[j].next)
	{
		if(i==j)
		{
			/* oh oh */

			return(-1);
		}
		if(j==pline->free_tail)
			break;
	}

	/* OK to free */

	/* detach */

	if(i==pline->used_head)
		pline->used_head=pline->disks[i].next;
	else
		pline->disks[pline->disks[i].prev].next=
			pline->disks[i].next;
	if(i==pline->used_tail)
		pline->used_tail=pline->disks[i].prev;
	else
		pline->disks[pline->disks[i].next].prev=
			pline->disks[i].prev;

	/* put at head of free list */

	pline->disks[i].prev=SCOUT_NIL;
	pline->disks[i].next=pline->free_head;
	if(pline->free_tail==SCOUT_NIL)
		pline->free_tail=i;
	if(pline->free_head!=SCOUT_NIL)
		pline->disks[pline->free_head].prev=i;
	pline->free_head=i;

	return(0);
}

/*------------------------------------------------------------------------
 * mini_PrintDiskStats
 *
 * Description:
 *	Given server indexing and light object information, a pointer
 *	to a set of statistics, and whether the probe that produced these
 * 	stats succeeded or not, print out the stats in a nice way.
 *
 * Arguments:
 *	struct mini_line *a_srvline	: Ptr to server descriptor.
 *	afsStatistics *a_stats	        : Ptr to current stats.
 *	int a_probeOK			: Was the probe OK?
 *	int a_width_changed		: Has the frame width changed?
 *	int a_fix_line_num		: Is the line number wrong?
 *	int a_delta_line_num		: Change in line number.
 *
 * Returns:
 *	0 if something went wrong,
 *	else the number of lines taken up by this server record.
 *
 * Environment:
 *	Nothing interesting.
 *
 * Side Effects:
 *	As advertised.
 *------------------------------------------------------------------------*/

static int mini_PrintDiskStats(a_srvline, a_stats, a_probeOK, a_width_changed,
			       a_fix_line_num, a_delta_line_num)
    struct mini_line *a_srvline;
    struct afsStatistics *a_stats;
    int a_probeOK;
    int a_fix_line_num;
    int a_delta_line_num;

{ /*mini_PrintDiskStats*/

    static char rn[] = "mini_PrintDiskStats";	/*Routine name*/
    int code;					/*Return code*/
    char s[128];				/*String buffer*/
    struct onode *curr_disklight;		/*Ptr to current disk light*/
    struct onode *srvname_light;		/*Ptr to server name light*/
    afsDisk *curr_diskstat;			/*Ptr to current disk stat*/
    int curr_disk;				/*Current disk stat number*/
    int used_disk_idx;				/*Used disk index*/
    int next_used_idx;				/*Ditto*/
    int pastthreshold;				/*Was disk past threshold?*/
    struct gator_lightobj *diskdata;		/*Private light data*/
    struct gwin_strparams *disk_strparams;	/*String params for disk light*/
    char *diskname;				/*Name of disk*/
    int found_idx;				/*Idx of matching disk*/
    char *srv_name;				/*Server name*/
    struct scout_disk *sc_disk;			/*Ptr to scout disk desc*/
    int fix_light_locs;				/*Recompute disk light locs?*/

    if (scout_debug) {
	dce_svc_printf(SCM_S_CALLED_MINI_PRINTDISKSTATS_MSG, rn);
	fflush(scout_debugfd);
    }
    
    /*
     * Remember the current server's print name, don't recompute light
     * locations yet.
     */
    srvname_light = a_srvline->srvName_lp;
    srv_name = ((struct gator_lightobj *)(srvname_light->o_data))->label;
    fix_light_locs = 0;
    if (scout_debug) {
	dce_svc_printf(SCM_S_VALUE_OF_A_DELTA_LINE_NUM_MSG, rn, a_delta_line_num);
	fflush(scout_debugfd);
    }
    
    /*
     * If the probe failed, we simply blank out all the used disk
     * objects.  Note: a NON-ZERO value of a_probeOK implies failure.
     */
    if (a_probeOK) {
	used_disk_idx = a_srvline->used_head;
	while (used_disk_idx != SCOUT_NIL) {
	    /*
	     * Point to the current used disk's light, blank out its
	     * contents, and make sure highlighting is turned off.  We
	     * also take this opportunity to fix the line numbers if
	     * needed.
	     */
	    curr_disklight = a_srvline->disks[used_disk_idx].disk_lp;
	    diskdata = (struct gator_lightobj *)(curr_disklight->o_data);
	    if (scout_debug) {
		dce_svc_printf(SCM_S_PREV_VALUE_OF_DISK_LIGHT_MSG, rn, used_disk_idx, diskdata->label);
		fflush(scout_debugfd);
	    }
	    code = mini_justify(" ",			/*Src buffer*/
				diskdata->label,	/*Dest buffer*/
				LIGHTOBJ_DISK_WIDTH,	/*Dest's width*/
				SCOUT_RIGHT_JUSTIFY,	/*Right-justify*/
				SCOUT_LEFT_TRUNC,	/*Left-truncate*/
				SCOUT_ISNT_LDISK);	/*Not a labeled disk*/
	    code = gator_light_set(curr_disklight, 0);
	    if (a_fix_line_num) {
		curr_disklight->o_y += a_delta_line_num;
		disk_strparams =
		    (struct gwin_strparams *)(diskdata->llrock);
		disk_strparams->y += a_delta_line_num;
	    }
	    
	    /*
	     * Advance to next used disk, if any.
	     */
	    used_disk_idx = a_srvline->disks[used_disk_idx].next;
	    
	} /*Blank out disk name field*/
	
	/*
	 * If the frame width has changed, we have to recompute all disk
	 * light locations.  After that, the number of lines in the server
	 * record will be accurate, and we return them.
	 */
	if (a_width_changed)
	    scout_RecomputeLightLocs(a_srvline);
	
	return(a_srvline->num_lines);
	
    } /*Probe failed for the server*/
    
    /*
     * Probe was successful.  Sweep through the statistics records,
     * and put up all values having to do with AFS partitions.  First,
     * mark all used disk objects for this server as inactive and fix
     * their line numbers if needed.
     */
    sc_disk = a_srvline->disks;
    used_disk_idx = a_srvline->used_head;
    while (used_disk_idx != SCOUT_NIL) {
	if (scout_debug) {
	    dce_svc_printf(SCM_S_MARK_DISK_INACTIVE_MSG, rn, used_disk_idx);
	    fflush(scout_debugfd);
	}
	sc_disk = (a_srvline->disks) + used_disk_idx;
	sc_disk->active = 0;
	used_disk_idx = sc_disk->next;
	if (a_fix_line_num) {
	    sc_disk->disk_lp->o_y += a_delta_line_num;
	    diskdata =
		(struct gator_lightobj *)(sc_disk->disk_lp->o_data);
	    disk_strparams =
		(struct gwin_strparams *)(diskdata->llrock);
	    disk_strparams->y += a_delta_line_num;
	}
    } /*Mark used disks inactive*/
    
    curr_diskstat = (afsDisk *) &(a_stats->Disk1);
    for (curr_disk = 0; curr_disk < SCOUT_MAX_DISKS; curr_disk++) {
	/*
	 * An AFS partition name must be prefixed by `/vicep`.
	 */
	if (scout_debug) {
	    dce_svc_printf(SCM_S_DISK_STATS_MSG, rn, curr_diskstat, curr_diskstat->Name);
	    fflush(scout_debugfd);
	}
	    /*
	     * Pull out the single-letter name (actually, abbreviation)
	     * of the disk and look for such an entry in the used disks.
	     */
	    diskname = (char *)curr_diskstat->Name;
	    if (strlen(diskname)) {
	    found_idx =
		scout_FindUsedDisk(diskname,            /*Disk name*/
				   a_srvline,		/*Server record*/
				   &fix_light_locs);	/*Recompute?*/
	    if (found_idx == SCOUT_NIL) {
		dce_svc_printf(SCM_S_CANT_DISPLAY_ON_SERVER_MSG, rn, diskname, srv_name);
	    }
	    else {
		/*
		 * Found (or created) record for this disk.  Fill in the single-
		 * letter name of the disk followed by the number of free blocks.
		 * Turn the disk light on if the number of free blocks exceeds
		 * the threshold the user set, either % used or min free blocks.
		 */
		sprintf(s, "%s:%d", diskname, curr_diskstat->BlocksAvailable);
		if (scout_attn_disk_mode == SCOUT_DISKM_PCUSED) {
		    if ((float)(curr_diskstat->TotalBlocks -
				curr_diskstat->BlocksAvailable) /
			(float)(curr_diskstat->TotalBlocks) > scout_attn_disk_pcused)
			pastthreshold = 1;
		    else
			pastthreshold = 0;
		}
		else
		    pastthreshold =
			(curr_diskstat->BlocksAvailable < scout_attn_disk_minfree) ?
			    1 : 0;
		sc_disk = (a_srvline->disks) + found_idx;
		sc_disk->active = 1;
		diskdata = (struct gator_lightobj *)(sc_disk->disk_lp->o_data);
		if (scout_debug) {
		    dce_svc_printf(SCM_S_JUSTIFYING_DISK_MSG, rn, s, found_idx, diskdata->label);
		    fflush(scout_debugfd);
		}
		code = mini_justify(s,			  /*Src buffer*/
				    diskdata->label,	  /*Dest buffer*/
				    LIGHTOBJ_DISK_WIDTH,  /*Dest's width*/
				    SCOUT_LEFT_JUSTIFY,   /*Left-justify*/
				    SCOUT_LEFT_TRUNC,     /*Left-truncate*/
				    SCOUT_IS_LDISK);	  /*Labeled disk*/
		
		code = gator_light_set(sc_disk->disk_lp, pastthreshold);
		
	    } /*Found disk record*/
	} /* strlen(diskname) */
	/*
	 * Advance to the next disk statistics record.
	 */
	curr_diskstat++;
    } /*For each statistics record*/
    
    /*
     * We've now pulled out all the disk statistics from the probe.
     * See if any used disks that were there from the last time are
     * now gone.  If so, we remove them.
     */
    if (scout_debug) {
	dce_svc_printf(SCM_S_SCAN_USED_RECOFRDS_MSG, rn);
	fflush(scout_debugfd);
    }
    used_disk_idx = a_srvline->used_head;
    while (used_disk_idx != SCOUT_NIL) {
	if (scout_debug) {
	    dce_svc_printf(SCM_S_EXAMINE_ENTRY_AT_INDEX_MSG, rn, used_disk_idx);
	    fflush(scout_debugfd);
	}
	sc_disk = (a_srvline->disks) + used_disk_idx;
	next_used_idx = sc_disk->next;
	if (!(sc_disk->active)) {
	    code=scout_RemoveInactiveDisk(a_srvline,		/*Server record*/
				     used_disk_idx);	/*Disk index to nuke*/
	    fix_light_locs = 1;
	}
	used_disk_idx = next_used_idx;
	
    } /*Remove inactive used disks*/
    
    /*
     * If we've had to add or remove disks to/from the used list,
     * or if the frame width has changed, we recompute the light
     * locations before returning.
     */
    if (fix_light_locs || a_width_changed)
	scout_RecomputeLightLocs(a_srvline);
    
    /*
     * Return the (possibly new) size of the current server record.
     */
    return(a_srvline->num_lines);

} /*mini_PrintDiskStats*/

/*------------------------------------------------------------------------
 * FS_Handler
 *
 * Description:
 *	Handler routine passed to the fsprobe module.  This handler is
 *	called immediately after a poll of all the FileServers has taken
 *	place.  Its job is to write out selected data to the scout
 *	screen.
 *
 * Arguments:
 *	None.
 *
 * Returns:
 *	0 on success,
 *	-1 otherwise.
 *
 * Environment:
 *	All it needs to know is exported by the fsprobe module, namely
 *	the data structure where the probe results are stored.
 *
 * Side Effects:
 *	Recomputes disk light locations in response to reshaping the
 *	scout window or from adding/deleting disk lights to/from
 *	individual servers.
 *------------------------------------------------------------------------*/

int FS_Handler()
    
{ /*FS_Handler*/
    
    static char rn[] = "FS_Handler";	/*Routine name*/
    int code;				/*Return code*/
    afsStatistics *curr_stats;	        /*Ptr to current stats*/
    int *curr_probeOK;			/*Ptr to current probeOK field*/
    int curr_srvidx;			/*Current server index*/
    char     s[128];			/*String buffer*/
    static char sblank[] = " ";		/*Blank string buffer*/
    char *sp;				/*Ptr to string buffer*/
    struct mini_line *curr_line;	/*Current mini-line*/
    int curr_line_num;			/*Current line number*/
    struct gator_lightobj *lightdata;	/*Private light data*/
    int setting;			/*Light setting (on or off)*/
    int old_width;			/*Keep the old width value*/
    int width_changed;			/*Has the frame width changed?*/
    int fix_line_num;			/*Line number needs fixing*/
    int delta_line_num;			/*Change in line number*/
    
    /*
     * See if the size of the scout frame has changed since the last
     * time.
     */
    old_width = scout_frameDims.maxx;
    if (scout_debug) {
	dce_svc_printf(SCM_S_CALLING_FS_HANDLER_MSG, rn);
	fflush(scout_debugfd);
    }
    WOP_GETDIMENSIONS(scout_frame->window, &scout_frameDims);
    width_changed = (old_width == scout_frameDims.maxx) ? 0 : 1;
    if (scout_debug) {
	dce_svc_printf(SCM_S_FRAME_DIMENSIONS_MSG, rn, scout_frameDims.maxy, scout_frameDims.maxx);
	if (width_changed)
	    dce_svc_printf(SCM_S_NEW_WIDTH_MSG , rn, old_width);
	fflush(scout_debugfd);
    }
    
    /*
     * Print out the selected fields for each server.  We actually change
     * the light's label to the new data.
     */
    curr_line     = scout_screen.line;
    curr_stats    = fsprobe_Results.stats;
    curr_probeOK  = fsprobe_Results.probeOK;
    curr_line_num = curr_line->base_line;
    
    setting = 0;
    for (curr_srvidx = 0;
	 curr_srvidx < scout_screen.numServers;
	 curr_srvidx++) {
	/*
	 * If the current server record is set up on the wrong line, fix
	 * the non-disk light objects directly, and remember to fix the
	 * disk light objects later on.
	 */
	if (curr_line->base_line != curr_line_num) {
	    fix_line_num = 1;
	    delta_line_num = curr_line_num - curr_line->base_line;
	    curr_line->base_line = curr_line_num;
	    scout_SetNonDiskLightLine(curr_line, curr_line_num);
	}
	else {
	    fix_line_num = 0;
	    delta_line_num = 0;
	}
	
	lightdata = (struct gator_lightobj *)(curr_line->currConns_lp->o_data);
	if (*curr_probeOK == 0) {
	    sp = s;
	    sprintf(sp, "%d", curr_stats->CurrentConnections);
	}
	else
	    sp = sblank;
	code = mini_justify(sp,				/*Src buffer*/
			    lightdata->label,		/*Dest buffer*/
			    LIGHTOBJ_CONN_WIDTH,	/*Dest's width*/
			    SCOUT_RIGHT_JUSTIFY,	/*Right-justify*/
			    SCOUT_LEFT_TRUNC,		/*Left-truncate*/
			    SCOUT_ISNT_LDISK);		/*Not a labeled disk*/
	if (scout_attn_conn != SCOUT_ATTN_NOTUSED &&
	    curr_stats->CurrentConnections >= scout_attn_conn)
	    setting = 1;
	else
	    setting = 0;
	code = gator_light_set(curr_line->currConns_lp, setting);
	
	lightdata = (struct gator_lightobj *)(curr_line->fetches_lp->o_data);
	if (*curr_probeOK == 0) {
	    sp = s;
	    sprintf(sp, "%d", curr_stats->TotalFetchs);
	}
	else
	    sp = sblank;
	code = mini_justify(sp,				/*Src buffer*/
			    lightdata->label,		/*Dest buffer*/
			    LIGHTOBJ_FETCH_WIDTH,	/*Dest's width*/
			    SCOUT_RIGHT_JUSTIFY,	/*Right-justify*/
			    SCOUT_LEFT_TRUNC,		/*Left-truncate*/
			    SCOUT_ISNT_LDISK);		/*Not a labeled disk*/
	if (scout_attn_fetch != SCOUT_ATTN_NOTUSED &&
	    curr_stats->TotalFetchs >= scout_attn_fetch)
	    setting = 1;
	else
	    setting = 0;
	code = gator_light_set(curr_line->fetches_lp, setting);
	
	lightdata = (struct gator_lightobj *)(curr_line->stores_lp->o_data);
	if (*curr_probeOK == 0) {
	    sp = s;
	    sprintf(sp, "%d", curr_stats->TotalStores);
	}
	else
	    sp = sblank;
	code = mini_justify(sp,				/*Src buffer*/
			    lightdata->label,		/*Dest buffer*/
			    LIGHTOBJ_STORE_WIDTH,	/*Dest's width*/
			    SCOUT_RIGHT_JUSTIFY,	/*Right-justify*/
			    SCOUT_LEFT_TRUNC,		/*Left-truncate*/
			    SCOUT_ISNT_LDISK);		/*Not a labeled disk*/
	if (scout_attn_store != SCOUT_ATTN_NOTUSED &&
	    curr_stats->TotalStores >= scout_attn_store)
	    setting = 1;
	else
	    setting = 0;
	code = gator_light_set(curr_line->stores_lp, setting);
	
	lightdata = (struct gator_lightobj *)(curr_line->workstations_lp->o_data);
	if (*curr_probeOK == 0) {
	    sp = s;
	    sprintf(sp, "%d", curr_stats->WorkStations);
	}
	else
	    sp = sblank;
	code = mini_justify(sp,				/*Src buffer*/
			    lightdata->label,		/*Dest buffer*/
			    LIGHTOBJ_WK_WIDTH,		/*Dest's width*/
			    SCOUT_RIGHT_JUSTIFY,	/*Right-justify*/
			    SCOUT_LEFT_TRUNC,		/*Left-truncate*/
			    SCOUT_ISNT_LDISK);		/*Not a labeled disk*/
	if (scout_attn_workstations != SCOUT_ATTN_NOTUSED &&
	    curr_stats->WorkStations >= scout_attn_workstations)
	    setting = 1;
	else
	    setting = 0;
	code = gator_light_set(curr_line->workstations_lp, setting);
	
	/*
	 * We turn the server light on if there was an error in the
	 * current probe (e.g., if the curr_probeOK field is non-zero.
	 * (Don't forget to fix the light's line if it needs it).
	 */
	setting = (*curr_probeOK) ? 1 : 0;
	code = gator_light_set(curr_line->srvName_lp, setting);
	
	/*
	 * Print out the disk statistics.  The value returned is the
	 * number of lines taken up by the server record (or 0 if
	 * something went wrong).
	 */
	code =
	    mini_PrintDiskStats(curr_line,	/*Ptr to server line*/
				curr_stats,	/*Fresh disk stats*/
				*curr_probeOK,	/*Was probe OK?*/
				width_changed,	/*Has the width changed?*/
				fix_line_num,	/*Fix the line number?*/
				delta_line_num);/*Change in line number*/
	if (code == 0) {
	    dce_svc_printf(SCM_S_ERROR_PRINTING_STATS_MSG, rn);
	    return(-1);
	}
	else
	    curr_line_num += code;
	
	/*
	 * Advance the current mini_line, stats source, and probe success
	 * indication.
	 */
	curr_line++;
	curr_stats++;
	curr_probeOK++;
	
    } /*for each server probed*/
    
    /*
     * Display the scout frame.
     */
    sp  = (char *) dce_sprintf(scm_s_probe_results, fsprobe_Results.probeNum);
    gtxframe_DisplayString(scout_frame, sp);
    free(sp);

    WOP_DISPLAY(scout_gwin);
    
    /*
     * Return the happy news.
     */
    return(0);
    
} /*FS_Handler*/

/*------------------------------------------------------------------------
 * init_mini_line
 *
 * Description:
 *	Initialize each line in the mini_screen.
 *
 * Arguments:
 *	struct sockaddr_in *a_skt : Ptr to server socket info.
 *	int a_lineNum;		  : Line number being created.
 *	struct mini_line *a_line  : Ptr to mini_line to set up.
 *	char *a_srvname		  : Printable server name.
 *
 * Returns:
 *	0 on success,
 *	Error value otherwise.
 *
 * Environment:
 *	Nothing interesting.
 *
 * Side Effects:
 *	As advertised.
 *------------------------------------------------------------------------*/

static int init_mini_line(h, a_lineNum, a_line, a_srvname)
    rpc_binding_handle_t h;
    int a_lineNum;
    struct mini_line *a_line;
    char *a_srvname;

{ /*init_mini_line*/

    static char rn[] = "init_mini_line";	/*Routine name*/
    int curr_x;					/*Current X position*/
    int curr_y;					/*Current Y position*/
    char s[128];				/*Scratch buffer*/
    int code;					/*Return code*/
    struct gator_lightobj *lightdata;		/*Private light data*/

    if (scout_debug) {
      dce_svc_printf(SCM_S_CALLING_INIT_MINI_LINE_MSG, rn, a_lineNum);
      fflush(scout_debugfd);
    }

    /*
      * Fill in the top fields (except the disk fields, which will be
      * done elsewhere), then create the light onodes.
      */
    a_line->h = h;	/* structure assignment */
    a_line->numDisks  = 0;
    a_line->base_line = a_lineNum + scout_screen.base_line_num;
    a_line->num_lines = 1;

    curr_x = 0;
    curr_y = a_line->base_line;
    if ((a_line->currConns_lp =
	 mini_initLightObject("Conns",
			      curr_x,
			      curr_y,
			      LIGHTOBJ_CONN_WIDTH,
			      scout_gwin))
	== (struct onode *)0) {
      dce_svc_printf(SCM_S_CANT_CREATE_LIGHT_OBJECT_MSG, pn, rn);
      return(-1);
    }
    curr_x += LIGHTOBJ_CONN_WIDTH + 1;

    if ((a_line->fetches_lp =
	 mini_initLightObject("Fetches",
			      curr_x,
			      curr_y,
			      LIGHTOBJ_FETCH_WIDTH,
			      scout_frame->window))
	== (struct onode *)0) {
      dce_svc_printf(SCM_S_CANT_CREATE_FETCHES_LIGHT_OBJECT_MSG, pn, rn);
      return(-1);
    }
    curr_x += LIGHTOBJ_FETCH_WIDTH + 1;

    if ((a_line->stores_lp =
	 mini_initLightObject("Stores",
			      curr_x,
			      curr_y,
			      LIGHTOBJ_STORE_WIDTH,
			      scout_frame->window))
	== (struct onode *)0) {
      dce_svc_printf(SCM_S_CANT_CREATE_STORES_LIGHT_OBJECT_MSG, pn, rn);
      return(-1);
    }
    curr_x += LIGHTOBJ_STORE_WIDTH + 1;

    if ((a_line->workstations_lp =
	 mini_initLightObject("WrkStn",
			      curr_x,
			      curr_y,
			      LIGHTOBJ_WK_WIDTH,
			      scout_frame->window))
	== (struct onode *)0) {
      dce_svc_printf(SCM_S_CANT_CREATE_WORKSTATIONS_LIGHT_OBJECT_MSG, pn, rn);
      return(-1);
    }
    curr_x += LIGHTOBJ_WK_WIDTH + 1;

    if ((a_line->srvName_lp =
	 mini_initLightObject(a_srvname,
			      curr_x,
			      curr_y,
			      LIGHTOBJ_SRVNAME_WIDTH,
			      scout_frame->window))
	== (struct onode *)0) {
      dce_svc_printf(SCM_S_CANT_CREATE_SERVER_LIGHT_OBJECT_MSG, pn, rn);
      return(-1);
    }
    sprintf(s, "%s", a_srvname);
    lightdata = (struct gator_lightobj *)(a_line->srvName_lp->o_data);
    code = mini_justify(s,				/*Src buffer*/
			lightdata->label,		/*Dest buffer*/
			LIGHTOBJ_SRVNAME_WIDTH,		/*Dest's width*/
			SCOUT_CENTER,			/*Centered*/
			SCOUT_RIGHT_TRUNC,		/*Right-truncate*/
			SCOUT_ISNT_LDISK);		/*Not a labeled disk*/
    if (code) {
      dce_svc_printf(SCM_S_CANT_CREATE_SERVER_NAME_IN_OBJECT_MSG, rn);
      return(code);
    }
    curr_x += LIGHTOBJ_SRVNAME_WIDTH + 1;

    if (scout_initDiskLightObjects(a_line, scout_frame->window)) {
      dce_svc_printf(SCM_S_CANT_CREATE_DISK_LIGHT_OBJECT_MSG, pn, rn);
      return(-1);
    }

    /*
      * Finally, return the happy news.
      */
    return(0);

} /*init_mini_line*/

/*------------------------------------------------------------------------
 * execute_scout
 *
 * Description:
 *	Workhorse routine that starts up the FileServer probe.
 *
 * Arguments:
 *	int a_numservers		: Length of above list.
 *	struct cmd_item *a_srvname	: List of FileServer machines to
 *					  monitor.
 *	int a_pkg			: Window package to use.
 *
 * Returns:
 *	0 on success,
 *	Error value otherwise.
 *
 * Environment:
 *	Nothing interesting.
 *
 * Side Effects:
 *	As advertised.
 *------------------------------------------------------------------------*/

static int execute_scout(a_numservers, a_srvname, a_pkg)
    int a_numservers;
    struct cmd_item *a_srvname;
    int a_pkg;

{ /*execute_scout*/

    static char	rn[] = "execute_scout";		/*Routine name*/
    static char fullsrvname[128];		/*Full server name*/
    unsigned char *hostPtr;
    int code;				        /*Return code*/

    rpc_binding_vector_t *bindingVector;        /* Server bindings */
    int bindingBytes;
    rpc_binding_handle_t curr_binding;

    struct cmd_item *curr_item;			/*Ptr to current cmd item*/
    struct mini_line *mini_lines;		/*Ptr to all mini-lines*/
    struct mini_line *curr_line;		/*Ptr to current line*/
    int i;                                      /*Generic loop variable*/
    int mini_line_bytes;			/*Num bytes in mini_lines*/
    struct timespec tv;				/*Time structure*/
    int linenum;				/*Current mini-line number*/
#if 0
    PROCESS pid;			    	/*Main thread process ID*/
    PROCESS gxlistener_ID;			/*Input Server thread process ID*/
#endif /* 0 */
    struct gator_lightobj *lightdata;		/*Private light data*/
    unsigned long curr_count;
    int localauth, noauth;
    uuid_t nil_uuid;

    if (scout_debug) {
      dce_svc_printf(SCM_S_CALLING_EXECUTE_SCOUT_MSG, rn);
      fflush(scout_debugfd);
    }

    /*
      * We have to initialize thread support before we start up any of
      * our packages.
      */

    /*
      * Initialize the gtx package.
      */
#if 0
    dce_svc_printf(SCM_S_START_GTX_PACKAGE_MSG, pn, rn);
#endif /* 0 */
    scout_gwin = gtx_Init(0,	/*Don't start up InputServer yet*/
			  -1);	/*Type of window package*/
    if (scout_gwin == (struct gwin *)0) {
      dce_svc_printf(SCM_S_GTX_INIT_FAILED_MSG, pn, rn);
      return(-1);
    }

    /*
      * Remember we've set up gtx so we can exit cleanly from now on.
      */
    scout_gtx_initialized = 1;

    /*
      * Create the frame everything will go into, set it up as our only
      * frame for this window.
      */
    scout_frame = gtxframe_Create();
    if (scout_frame == (struct gtx_frame *)0) {
      dce_svc_printf(SCM_S_GTXFRAME_CREATE_FAILED_MSG, pn, rn);
      return(-1);
    }
    gtxframe_SetFrame(scout_gwin, scout_frame);
    WOP_GETDIMENSIONS(scout_frame->window, &scout_frameDims);

    /*
      * Allocate an array of sockets to describe each FileServer we'll be
      * watching.
      */
    bindingVector = (rpc_binding_vector_t *) malloc(sizeof(rpc_binding_vector_t) +
						    (a_numservers - 1) * 
						    sizeof(rpc_binding_handle_t));
    bzero((char *)bindingVector, sizeof(rpc_binding_vector_t) + (a_numservers - 1) *
	  sizeof(rpc_binding_handle_t));
    bindingVector->count = 0;
    /*
      * Sweep through the server names provided, filling in the socket
      * info for each.  Take into account the fact that we may have a
      * base name associated for each.
      */
    curr_item = a_srvname;
    curr_count = 0;

    while (curr_item) {
      if ((*scout_basename == '\0') || (*curr_item->data == '/'))
	sprintf(fullsrvname, "%s", curr_item->data);
      else 
	sprintf(fullsrvname, "%s/%s", scout_basename, curr_item->data);

      server_names[curr_count] = (char *)malloc(strlen(fullsrvname)+1);
      if (!server_names) {
	  dce_svc_printf(SCM_S_NO_MEMORY_FOR_SEERVER_NAME_MSG, pn, rn);
	  return(-1);
      }
      strcpy (server_names[curr_count], fullsrvname);

      uuid_create_nil(&nil_uuid, (unsigned32 *)&code);
      if (code != uuid_s_ok) {
	dce_svc_printf(SCM_S_CANT_CREATE_NIL_UUID_MSG, rn, fullsrvname);
	fflush(stderr);
	scout_CleanExit(-1);
      }
      localauth = noauth = 0;    /* For Now */
      dfs_locate_rpc_server((unsigned_char_t *)fullsrvname,
			    &curr_binding, &nil_uuid, localauth, noauth,
			    (error_status_t *)&code);
      /* we assume that it's all right to force shortname noauth here, w/o warnings */
      if ((code != rpc_s_ok) && (code != COMPAT_ERROR_SHORTNAME_NOAUTH)) {
	  dce_svc_printf(SCM_S_INVALID_HOSTNAME_MSG, rn, fullsrvname);
	  fflush(stderr);
	  if (scout_debugfd) {
	  dce_svc_printf(SCM_S_DEBUD_INVALID_HOSTNAME_MSG, rn, fullsrvname);
	  fflush(scout_debugfd);
          }
	  scout_CleanExit(-1);
      }
      else {
	  bindingVector->binding_h[curr_count] = curr_binding; /* str. assgt. */
	  bindingVector->count++;
      }
      /*
	* Bump our pointers.
	*/
      curr_item = curr_item->next;
      curr_count++;

    } /*Build socket entry for each server*/

    /*
      * Create the set of mini-lines, one per server.
      */
    mini_line_bytes = a_numservers * sizeof(struct mini_line);
    mini_lines = (struct mini_line *) malloc(mini_line_bytes);
    if (mini_lines == (struct mini_line *)0) {
      dce_svc_printf(SCM_S_NO_MEMORY_FOR_SCREEN_LINES_MSG, rn, mini_line_bytes, a_numservers);
      return(-1);
  }
    bzero((char *)mini_lines, mini_line_bytes);

    /*
      * Set up each line in the mini_lines, creating and initializing
      * its light objects.
      */
    scout_screen.base_line_num = 4;
    curr_line = mini_lines;
    curr_binding = bindingVector->binding_h[0];
    linenum = 0;
    curr_item = a_srvname;
    gtxframe_ClearList(scout_frame);

    /*
      * Create the light objects that serve as banner lines.  Remember
      * to take into account the server basename, if any, and the name
      * of the host that scout if running on, if that's wanted.
      */
    if (scout_showhostname) 

      {
	char          *s;

	if (*scout_basename == '\0')
	  s  = (char *) dce_sprintf(scm_s_first_light_banner, scout_hostname);
	else
	  s  = (char *) dce_sprintf(scm_s_second_scout_banner,
			   scout_hostname, scout_basename);

	strcpy(scout_Banner, s);
	free(s);
      }

    else 

      {
	char          *s;

	if (*scout_basename == '\0')
	  s = (char *) dce_sprintf(scm_s_third_scout_banner);
	else
	  s = (char *) dce_sprintf(scm_s_fourth_scout_banner, scout_basename);

	strcpy(scout_Banner, s);
	free(s);
      }

    scout_banner0_lp =
      mini_initLightObject("Banner 0",		 /*Light name*/
			   0,			 /*X*/
			   0,			 /*Y*/
			   scout_frameDims.maxx, /*Width*/
			   scout_gwin);		 /*Window*/
    if (scout_banner0_lp != (struct onode *)0) {
      lightdata = (struct gator_lightobj *)(scout_banner0_lp->o_data);
      code = mini_justify(scout_Banner,
			  lightdata->label,
			  scout_frameDims.maxx,
			  SCOUT_CENTER,
			  SCOUT_RIGHT_TRUNC,
			  SCOUT_ISNT_LDISK);
      code = gator_light_set(scout_banner0_lp, 1);
      code = gtxframe_AddToList(scout_frame, scout_banner0_lp);

      /*Debugging*/
      if (scout_debug)
	dce_svc_printf(SCM_S_SCOUT_LABEL_MSG, rn, lightdata->label, strlen(lightdata->label));
    }

    scout_banner1_lp =
      mini_initLightObject("Banner 1",		 /*Light name*/
			   0,			 /*X*/
			   2,			 /*Y*/
			   scout_frameDims.maxx, /*Width*/
			   scout_gwin);		 /*Window*/

    if (scout_banner1_lp != (struct onode *)0) 

      {
	char          *s;

	if (scout_attn_disk_mode == SCOUT_DISKM_PCUSED) 

	  {
	    s = (char *) dce_sprintf(scm_s_fifth_scout_banner,
			    scout_LightLabels,
			    scout_attn_disk_pcusedstr);
	  }

	else 

	  {
	    s = (char *) dce_sprintf(scm_s_sixth_scout_banner,
			    scout_LightLabels, scout_attn_disk_minfree);
	  }

	strcpy(scout_Banner, s);
	free(s);

	lightdata = (struct gator_lightobj *)(scout_banner1_lp->o_data);

	code = mini_justify(scout_Banner,
			    lightdata->label,
			    scout_frameDims.maxx,
			    SCOUT_LEFT_JUSTIFY,
			    SCOUT_RIGHT_TRUNC,
			    SCOUT_ISNT_LDISK);

	code = gtxframe_AddToList(scout_frame, scout_banner1_lp);
      }

    scout_banner2_lp =
      mini_initLightObject("Banner 2",		 /*Light name*/
			   0,			 /*X*/
			   3,			 /*Y*/
			   scout_frameDims.maxx, /*Width*/
			   scout_gwin);		 /*Window*/
    if (scout_banner2_lp != (struct onode *)0) {
      lightdata = (struct gator_lightobj *)(scout_banner2_lp->o_data);
      code = mini_justify(scout_LightLabelUnd,
			  lightdata->label,
			  scout_frameDims.maxx,
			  SCOUT_LEFT_JUSTIFY,
			  SCOUT_RIGHT_TRUNC,
			  SCOUT_ISNT_LDISK);
      code = gtxframe_AddToList(scout_frame, scout_banner2_lp);
    }

    for (i=0; i < a_numservers; i++) {
      char *servName;

      /* if name is DCE-type name, display end only */
      servName = strrchr(curr_item->data, '/');
      if (servName) servName++;
      else servName = curr_item->data;
      code = init_mini_line(curr_binding, linenum, curr_line, servName);
      if (code) {
	dce_svc_printf(SCM_S_CANT_INIT_SERVER_LINE_MSG, rn, i);
	return(-1);
      }
      curr_binding = bindingVector->binding_h[i];
      curr_line++;
      linenum++;
      curr_item = curr_item->next;
    }

    /*
      * Now that all lines have been set up, we feed in the light items
      * created.  Note: the disk lights are entered at a later time,
      * as they enter the used disk list for each server.
      */
    curr_line = mini_lines;
    for (i = 0; i < a_numservers; i++) {
      code = gtxframe_AddToList(scout_frame, curr_line->currConns_lp);
      if (code) {
	dce_svc_printf(SCM_S_CANT_ADD_CONNECTION_LIGHT_MSG, rn);
	return(code);
      }

      code = gtxframe_AddToList(scout_frame, curr_line->fetches_lp);
      if (code) {
	dce_svc_printf(SCM_S_CANT_ADD_FETCHES_LISGHT_MSG, rn);
	return(code);
      }

      code = gtxframe_AddToList(scout_frame, curr_line->stores_lp);
      if (code) {
	dce_svc_printf(SCM_S_CANT_ADD_STORES_LIGHT_MSG, rn);
	return(code);
      }

      code = gtxframe_AddToList(scout_frame, curr_line->workstations_lp);
      if (code) {
	dce_svc_printf(SCM_S_CANT_ADD_WORKSTATION_LIGHT_MSG, rn);
	return(code);
      }

      code = gtxframe_AddToList(scout_frame, curr_line->srvName_lp);
      if (code) {
	dce_svc_printf(SCM_S_CANT_ADD_SEVER_LIGHT_MSG, rn);
	return(code);
      }

      /*
	* Move down to the next server record.
	*/
      curr_line++;

    } /*Add lights in server records to display list*/

#if 0
    /*
      * Set up the minimal keymap.
      */
    code =
      keymap_BindToString(scout_frame->keymap,		/*Ptr to keymap*/
			  "e",				/*Key to bind*/
			  ExitCmd,			/*Cmd to execute*/
			  (char *)0,			/*Name*/
			  (char *)0);			/*Ptr to rock*/
    if (code) {
      dce_svc_printf(SCM_S_CANT_BIND_KEY_MSG, rn, code);
      return(code);
    }
#endif /* 0 */

    /*
      * Finish setting up the overall mini_screen structure.
      */
    scout_screen.numServers = a_numservers;
    scout_screen.line	    = mini_lines;
    WOP_GETDIMENSIONS(scout_frame->window, &scout_frameDims);

    /*
      * Start up the fsprobe package, which will gather FileServer
      * statistics for us on a regular basis.
      */
    gtxframe_DisplayString(scout_frame, "Establishing File Server connection(s)...");
    code = fsprobe_Init(a_numservers,		/*Num FileServers to probe*/

			server_names,           /* FileServer Names */

			bindingVector,          /*FileServer bindings */

			scout_probefreq,	/*Probe frequency*/
			FS_Handler,		/*Handler routine*/
			DEBUG_FLAG);		/*Turn debugging output off*/
#if 0
			scout_debug);		/*Turn debugging output off*/
#endif /* 0 */
    if (code) {
      dce_svc_printf(SCM_S_ERROR_IN_FSPROBE_INIT_MSG, rn, code);
      return(-1);
    }


    code = gtx_InputServer(scout_gwin);
    if (code) {
      dce_svc_printf(SCM_S_ERROR_EX0T_FROM_GTX_INPUTSERVER_MSG, rn, code);
      scout_CleanExit(code);
    }

    /*
      * We fall into a loop, sleeping forever.
      */
    while (1) {
      tv.tv_sec  = 60*60;	/*Sleep for an hour at a time*/
      tv.tv_nsec = 0;
      code = pthread_delay_np(&tv);
    } /*Sleep forever*/

#if 0
    /*
      * How did we get here?  Oh, well, clean up our windows and
      * return sweetness and light anyway.
      */
    WOP_CLEANUP(&scout_gwin);
    return(0);
#endif /* 0 */

} /*execute_scout*/

/*------------------------------------------------------------------------
 * countServers
 *
 * Description:
 *	Given a pointer to the list of servers we'll be polling,
 *	compute the length of the list.
 *
 * Arguments:
 *	struct cmd_item *a_firstItem : Ptr to first item in list.
 *
 * Returns:
 *	Length of the above list.
 *
 * Environment:
 *	Nothing interesting.
 *
 * Side Effects:
 *	As advertised.
 *------------------------------------------------------------------------*/

static int countServers(a_firstItem)
    struct cmd_item *a_firstItem;

{ /*countServers*/

    int list_len;		/*List length*/
    struct cmd_item *curr_item;	/*Ptr to current item*/

    list_len = 0;
    curr_item = a_firstItem;

    /*
      * Count 'em up.
      */
    while (curr_item) {
      list_len++;
      curr_item = curr_item->next;
    }

    /*
      * Return our tally.
      */
    return(list_len);

} /*countServers*/

/*------------------------------------------------------------------------
 * scout_AdoptThresholds
 *
 * Description:
 *	Parse and adopt one or more threshold values, as read off the
 *	command line.
 *
 * Arguments:
 *	struct cmd_item *a_thresh_item : Ptr to item on command-line
 *					 threshold list.
 *
 * Returns:
 *	Nothing (but may exit the entire program on error!)
 *
 * Environment:
 *	Valid keywords are:
 *	    conn, disk, fetch, store, ws
 *	The disk value, if it has a % sign, signifies that attention
 *	will be triggered when the disk is more than that percentage
 *	full; otherwise, it will specify the minimum number of free
 *	blocks.
 *
 * Side Effects:
 *	As specified.
 *------------------------------------------------------------------------*/

static void scout_AdoptThresholds(a_thresh_item)
    struct cmd_item *a_thresh_item;

{ /*scout_AdoptThresholds*/

    static char rn[] =
      "scout_AdoptThresholds";		/*Routine name*/
    struct cmd_item *curr_item;		/*Current item*/
    char *curr_name;			/*Current name half of pair*/
    char *curr_value;			/*Current value half of pair*/
    int diskval_len;			/*Length of disk attn value*/
    int err = 0;                        
    int val;  


    curr_item = a_thresh_item;
    while (curr_item) {
      /*
	* If there isn't a corresponding value for the current
	* attention field, bitch & die.
	*/
	if (curr_item->next == (struct cmd_item *)0) {
	  dce_svc_printf(SCM_S_NO_THRESHOLD_VALUE_MSG, rn, curr_item->data);
	  scout_CleanExit(-1);
	}

	curr_name  = curr_item->data;
	curr_value = curr_item->next->data;

	if (strcmp(curr_name, "conn") == 0) {
	  if (scout_debug) {
	    dce_svc_printf(SCM_S_SET_CONN_ATTN_VALUE_MSG, rn, curr_value, scout_attn_conn);
	    fflush(scout_debugfd);
	  }
	  scout_attn_conn = osi_atoi(curr_value, &err);
	  if (scout_attn_conn <= 0 || err < 0) {
		  dce_svc_printf(SCM_S_INVALID_CONNECTION_THRESHOLD_MSG,
				 rn, curr_value);
		  fflush(stderr);
	      if (scout_debug) {
		  dce_svc_printf(SCM_S_DEBUG_INVALID_CONNECTION_THRESHOLD_MSG, rn, curr_value);
		  fflush(scout_debugfd);
	      }
	      scout_CleanExit(-1);
	      }
      }
	else if (strcmp(curr_name, "disk") == 0) {
	  /*
	    * If there's a '%' as the last character in the value,
	    * we use percentage mode.
	    */
	  diskval_len = strlen(curr_value);
	  if (curr_value[diskval_len-1] == '%') {
	    curr_value[diskval_len-1] = '\0';
	    val  = osi_atoi(curr_value, &err); 
	    if (val  <= 0 || val  > 100 || err < 0) {
		dce_svc_printf(SCM_S_AGAIN_DISK_THRESHOLD_VALUE_MSG,
			       rn, curr_value, scout_attn_disk_pcused);
		fflush(stderr);
		if (scout_debug) {
		    dce_svc_printf(SCM_S_AGAIN_DISK_THRESHOLD_VALUE_MSG, rn, curr_value, scout_attn_disk_pcused);
		    fflush(scout_debugfd);
		}
		scout_CleanExit(-1);
	    }
	    sprintf(scout_attn_disk_pcusedstr, "%s", curr_value);
	    scout_attn_disk_pcused =
	      (float)(val)/((float)(100));

	      dce_svc_printf(SCM_S_NEW_DISK_ATTN_VALUE_MSG,
			     rn, curr_value, scout_attn_disk_pcused);
	      fflush(stderr);
	    if (scout_debug) {
	      dce_svc_printf(SCM_S_DEBUG_DISK_THRESHOLD_VALUE_MSG,
			     rn, curr_value, scout_attn_disk_pcused);
	      fflush(scout_debugfd);
            }
	  } /*Percentage mode*/
	  else {
	    /* Min disk blocks free mode */
	      dce_svc_printf(SCM_S_SECOND_DISK_ATTN_VALUE_MSG,
			     rn, curr_value, scout_attn_disk_pcused);
	      fflush(stderr);
	    if (scout_debug) {
	      dce_svc_printf(SCM_S_SECOND_DEBUG_DISK_ATTN_VALUE_MSG,
			     rn, curr_value, scout_attn_disk_pcused);
	      fflush(scout_debugfd);
	    }
	    scout_attn_disk_mode = SCOUT_DISKM_MINFREE;
	    scout_attn_disk_minfree = osi_atoi(curr_value, &err);
	    if (scout_attn_disk_minfree <= 0 || err < 0) {
		    dce_svc_printf(SCM_S_INVALID_DISK_ATTN_VALUE_MSG,
				   rn, curr_value, scout_attn_disk_pcused);
		    fflush(stderr);
		if (scout_debug) {
		    dce_svc_printf(SCM_S_DEBUG_INVALID_DISK_ATTN_VALUE_MSG, rn, curr_value,  scout_attn_disk_pcused);
		    fflush(scout_debugfd);
		}
		scout_CleanExit(-1);
	    }
	  } /*Min free blocks mode*/
      }
	else if (strcmp(curr_name, "fetch") == 0) {
	  if (scout_debug) {
	    dce_svc_printf(SCM_S_SET_FETCH_ATTN_VALUE_MSG,
			   rn, curr_value, scout_attn_fetch);
	    fflush(stderr);
	    dce_svc_printf(SCM_S_DEBUG_SET_FETCH_ATTN_VALUE_MSG,
			   rn, curr_value, scout_attn_fetch);
	    fflush(scout_debugfd);
	  }
	  scout_attn_fetch = osi_atoi(curr_value, &err);
	  if (scout_attn_fetch <= 0 || err < 0) {
		  dce_svc_printf(SCM_S_INVALID_FETCH_ATTN_VALUE_MSG,
				 rn, curr_value);
		  fflush(stderr);
	      if (scout_debug) {
		  dce_svc_printf(SCM_S_DEBUG_INVALID_FETCH_ATTN_VALUE_MSG, rn, curr_value);
		  fflush(scout_debugfd);
	      }
	      scout_CleanExit(-1);
	      }
      }
	else if (strcmp(curr_name, "store") == 0) {
	    dce_svc_printf(SCM_S_SET_STORE_ATTN_VALUE_MSG,
			   rn, curr_value, scout_attn_store);
	    fflush(stderr);
	  if (scout_debug) {
	    dce_svc_printf(SCM_S_DEBUG_SET_STORE_ATTN_VALUE_MSG,
			   rn, curr_value, scout_attn_store);
	    fflush(scout_debugfd);
	  }
	  scout_attn_store = osi_atoi(curr_value, &err);
	  if (scout_attn_store <= 0 || err < 0) {
		  dce_svc_printf(SCM_S_INVALIDSTORE_ATTN_VALUE_MSG,
				 rn, curr_value);
		  fflush(stderr);
	      if (scout_debug) {
		  dce_svc_printf(SCM_S_DEBUG_INVALIDSTORE_ATTN_VALUE_MSG, rn, curr_value);
		  fflush(scout_debugfd);
	      }
	      scout_CleanExit(-1);
	  }
	}
	else if (strcmp(curr_name, "ws") == 0) {
	    dce_svc_printf(SCM_S_SET_WORKSTATION_ATTN_VALUE_MSG,
			   rn, curr_value, scout_attn_workstations);
	    fflush(stderr);
	  if (scout_debug) {
	    dce_svc_printf(SCM_S_DEBUG_SET_WORKSTATION_ATTN_VALUE_MSG,
			   rn, curr_value, scout_attn_workstations);
	    fflush(scout_debugfd);
	  }
	  scout_attn_workstations = osi_atoi(curr_value, &err);
	  if (scout_attn_workstations <= 0 || err < 0) {
		  dce_svc_printf(SCM_S_INVALIDWORKSTATION_ATTN_VALUE_MSG,
				 rn, curr_value);
		  fflush(stderr);
	      if (scout_debug) {
		  dce_svc_printf(SCM_S_DEBUG_INVALIDWORKSTATION_ATTN_VALUE_MSG, rn, curr_value);
		  fflush(scout_debugfd);
	      }
	      scout_CleanExit(-1);
	  }
	  
	}
	else {
		dce_svc_printf(SCM_S_UNKNOWN_ATTENTION_ITEM_MSG, rn, curr_item->data);
		fflush(stderr);
	    if (scout_debug) {
		dce_svc_printf(SCM_S_DEBUG_UNKNOWN_ATTENTION_ITEM_MSG, rn, curr_item->data);
		fflush(scout_debugfd);
	    }
	    scout_CleanExit(-1);
	    }

	/*
	  * Advance past the just-processed pair.
	  */
	curr_item = curr_item->next->next;

      }     /*Interpret each name-value pair*/

} /*scout_AdoptThresholds*/
/*------------------------------------------------------------------------
 * scoutInit
 *
 * Description:
 *	Routine called when Scout is invoked, responsible for basic
 *	initialization, command line parsing, and calling the
 *	routine that does all the work.
 *
 * Arguments:
 *	as	: Command syntax descriptor.
 *	arock	: Associated rock (not used here).
 *
 * Returns:
 *	Zero (but may exit the entire program on error!)
 *
 * Environment:
 *	Nothing interesting.
 *
 * Side Effects:
 *	Initializes this program.
 *------------------------------------------------------------------------*/

static int scoutInit(as, arock)
    struct cmd_syndesc *as;
    char *arock;

{ /*scoutInit*/

    static char	rn[] = "scoutInit";	/*Routine name*/
    int code;				/*Return code*/
    int	wpkg_to_use;			/*Window package to use*/
    int server_count;			/*Number of servers to watch*/
    int err; 

    if (as->parms[P_DEBUG].items != 0) {
 	error_status_t                       st;
	char                                 debug_route [100];

	strcpy(debug_route, "NOTICE:TEXTFILE:");

	strncat(debug_route,
		as->parms[P_DEBUG].items->data,
		sizeof(debug_route) - (strlen(debug_route) + 1));

	dce_svc_routing((unsigned char *) debug_route, &st);

	if (st != svc_s_ok) {
	    fprintf(stderr, "Cannot route NOTICE messages  0x%lx\n", st);
	    scout_CleanExit(-1);
	}
	scout_debug = 1;
	dce_svc_printf(SCM_S_WRITING_TO_DEBUGGING_FILE_MSG, 
		       rn,
		       as->parms[P_DEBUG].items->data);
    }
    if (scout_debug) {
      dce_svc_printf(SCM_S_CALLING_SCOUTINIT_MSG, rn);
      fflush(scout_debugfd);
    }

#if 0
    wpkg_to_use = osi_atoi(as->parms[P_PACKAGE].items->data, &err);
#endif /* 0 */
    wpkg_to_use = 2; /*Always use curses for now*/
#if 0
    dce_svc_printf(SCM_S_USING_GRAPHICS_PACKAGE_MSG, pn, rn, wpkg_to_use);
    switch (wpkg_to_use) {
	case GATOR_WIN_CURSES:
	    dce_svc_printf(SCM_S_CURSES_MSG);
	    break;
	case GATOR_WIN_DUMB:
	    dce_svc_printf(SCM_S_DUMB_TERMINAL_MSG);
	    break;
	case GATOR_WIN_X11:
	    dce_svc_printf(SCM_S_X11_MSG);
	    break;
	default:

	    scout_CleanExit(-1);
    } /*end switch (wpkg_to_use)*/
#endif /* 0 */

    err = 0;
    if (as->parms[P_FREQ].items != 0 && as->parms[P_FREQ].items->data !=0) 
      scout_probefreq = osi_atoi(as->parms[P_FREQ].items->data, &err);
    else
      scout_probefreq = 60;

    /* Check for a valid frequency value */
    if (scout_probefreq <= 0 || err < 0) {
      if (scout_debugfd != (FILE *)0)
        dce_svc_printf(SCM_S_INVALID_PROBING_FREQUENCY_MSG, rn, scout_probefreq);
      dce_svc_printf(SCM_S_SECOND_INVALID_PROBING_FREQUENCY_MSG, rn, scout_probefreq);
      scout_CleanExit(-1);
    }
    /*
      * See if we've been fed a base server name.
      */
    if (as->parms[P_BASE].items != 0)
      sprintf(scout_basename, "%s", as->parms[P_BASE].items->data);
    else
      *scout_basename = '\0';

    /*
      * Count the number of servers we've been asked to monitor.
      */
    server_count = countServers(as->parms[P_SERVER].items);

    server_names = (char **)malloc(server_count * sizeof(char *));
    if (!server_names) {
	dce_svc_printf(SCM_S_NO_MEMORY_FOR_SERVER_NAMES_MSG, pn, rn);
	return(-1);
    }
    
    /*
      * Create a line of blanks, a generally-useful thing.
      */
    sprintf(scout_blankline, "%255s", " ");

    /*
      * Pull in the name of the host we're executing on if we've been
      * asked to.  If we can't get the name, we provide a default.
      */
    if (as->parms[P_HOST].items != 0) {
      scout_showhostname = 1;
      *scout_hostname = '\0';
      code = gethostname(scout_hostname, 128);

      if (code)
	{
        char  *s;
        s = (char *) dce_sprintf(scm_s_no_hostname);
	strcpy(scout_hostname, s);
	free(s);
        }
    }

    /*
      * Pull in any and all attention/highlighting thresholds.
      */
    if (as->parms[P_ATTENTION].items != 0)
      scout_AdoptThresholds(as->parms[P_ATTENTION].items);

    /*
     * Now, drive the sucker.
     */
    code = execute_scout(server_count,			/*Num servers*/
			 as->parms[P_SERVER].items,	/*Ptr to srv names*/
			 wpkg_to_use);			/*Graphics pkg*/
    if (code) {
      dce_svc_printf(SCM_S_ERROR_EXECUTING_SCOUT_MSG, rn, code);
      scout_CleanExit(-1);
    }

    /*
     * We initialized (and ran) correctly, so return the good news.
     */
    return(0);

} /*scoutInit*/

main(argc, argv)
    int argc;
    char **argv;

{ /*main*/

    register long code;			/*Return code*/
    register struct cmd_syndesc	*ts;	/*Ptr to cmd line syntax descriptor*/
    unsigned char   *cmd_explanation;
    error_status_t  st;

    osi_setlocale(LC_ALL, "");

    initialize_svc();

    /*
     * Set up the commands we understand.
     */

    cmd_explanation = dce_msg_get_msg(scm_s_cmd_initd, &st);
    ts = cmd_CreateSyntax("initcmd", scoutInit, 0,
			  (char *) cmd_explanation);
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(scm_s_cmd_machine, &st);
    cmd_AddParm(ts, "-server",     CMD_LIST,   CMD_REQUIRED,
		(char *) cmd_explanation);
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(scm_s_cmd_common_prefix, &st);
    cmd_AddParm(ts, "-basename",   CMD_SINGLE, CMD_OPTIONAL,
		(char *) cmd_explanation);
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(scm_s_cmd_host_name, &st);
    cmd_AddParm(ts, "-host",	   CMD_FLAG,   CMD_OPTIONAL,
		(char *) cmd_explanation);
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(scm_s_cmd_seconds, &st);
    cmd_AddParm(ts, "-frequency",  CMD_SINGLE, CMD_OPTIONAL,
		(char *) cmd_explanation);
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(scm_s_cmd_stat_threshold, &st);
    cmd_AddParm(ts, "-attention",  CMD_LIST, CMD_OPTIONAL,
		(char *) cmd_explanation);
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(scm_s_cmd_filename, &st);
    cmd_AddParm(ts, "-debug",      CMD_SINGLE,  CMD_OPTIONAL,
		(char *) cmd_explanation);
    free(cmd_explanation);

    /*
     * Parse command-line switches & execute the test, then get the heck
     * out of here.
     */

    code = cmd_Dispatch(argc, argv);

    scout_CleanExit((code) ? 1 : 0);
} /*main*/
