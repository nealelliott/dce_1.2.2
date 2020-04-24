/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20admin.c,v $
 * Revision 1.1.11.2  1996/02/18  19:42:18  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:23:05  marty]
 *
 * Revision 1.1.11.1  1995/12/08  15:21:28  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1994/08/17  21:01 UTC  dtruong
 * 	Merge from Hpdce02_01
 * 	[1995/12/08  14:52:57  root]
 * 
 * Revision 1.1.9.7  1994/08/08  13:20:55  marrek
 * 	Fix for OT11045.
 * 	[1994/08/08  13:18:39  marrek]
 * 
 * Revision 1.1.9.6  1994/08/01  15:31:52  marrek
 * 	Bug fixes for OT 11295, 11456, 11458, 11471.
 * 	[1994/08/01  15:30:45  marrek]
 * 
 * Revision 1.1.9.5  1994/07/06  15:06:33  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:12:09  marrek]
 * 
 * Revision 1.1.9.4  1994/06/21  14:43:55  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:25:17  marrek]
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/29  16:40 UTC  dtruong
 * 	Merge to DCE 1.1
 * 
 * 	HP revision /main/HPDCE01/2  1994/03/16  21:36 UTC  dtruong
 * 	merge to main line
 * 
 * 	HP revision /main/HPDCE01/dtruong_mothra/2  1994/03/07  19:14 UTC  dtruong
 * 	Add routine to check terminal type
 * 
 * 	HP revision /main/HPDCE01/1  1994/01/28  19:17  mgm
 * 	Merge hpdce01 and kk_final
 * 
 * Revision 1.1.7.3  1993/12/17  23:09:25  jake
 * 	GDS What string changes
 * 	[1993/12/17  23:09:01  jake]
 * 
 * Revision 1.1.7.2  1993/10/27  22:09:01  dtruong
 * 	<<<Added softkeys for hp terminal>>>
 * 	[1993/10/27  22:05:35  dtruong]
 * 
 * Revision 1.1.2.4  1993/10/19  18:14:06  root
 * 	    Loading
 * 
 * Revision 1.1.7.2  1993/08/10  11:00:01  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:41:20  marrek]
 * 
 * Revision 1.1.7.2  1993/08/10  11:00:01  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:41:20  marrek]
 * 
 * Revision 1.1.5.10  1993/02/02  10:08:31  marrek
 * 	'/' in filename is now allowed
 * 	[1993/02/01  17:43:35  marrek]
 * 
 * Revision 1.1.5.9  1993/01/29  08:39:08  marrek
 * 	corrected function is_allowed
 * 	[1993/01/28  10:44:35  marrek]
 * 
 * Revision 1.1.5.8  1992/12/31  18:10:58  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:07:38  bbelch]
 * 
 * Revision 1.1.5.7  1992/12/17  23:12:10  tom
 * 	Bug 6411 - Add setlocale call for i18n.
 * 	[1992/12/17  23:07:21  tom]
 * 
 * Revision 1.1.5.6  1992/12/15  15:24:24  marrek
 * 	Insert fflush(stdout) in systemfkt().
 * 	[1992/12/15  12:57:18  marrek]
 * 
 * Revision 1.1.5.5  1992/12/11  16:10:46  marrek
 * 	Configuration of dir-id '1' allowed
 * 	[1992/12/11  13:24:36  marrek]
 * 
 * Revision 1.1.5.4  1992/12/07  16:19:09  marrek
 * 	November 1992 code drop
 * 	changes for filename-check
 * 	[1992/12/07  16:17:23  marrek]
 * 
 * Revision 1.1.5.3  1992/11/27  15:39:54  marrek
 * 	November 1992 code drop
 * 	[1992/11/26  09:14:16  marrek]
 * 
 * Revision 1.1.5.2  1992/09/23  10:00:05  marrek
 * 	Delete lang_p, that is not needed.
 * 	[1992/09/23  09:58:51  marrek]
 * 
 * Revision 1.1.3.3  1992/06/26  07:03:43  jim
 * 	Merge AIX 3.2 changes with the latest level.
 * 	[1992/06/22  21:22:49  jim]
 * 
 * Revision 1.1.3.2  1992/06/01  20:01:39  melman
 * 	New GDS merged drop
 * 	[1992/05/28  20:45:40  melman]
 * 	Revision 1.1.1.2  1992/05/29  19:01:48  jim
 * 	Added setlocale calls for AIX 3.2
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20admin.c,v $ $Revision: 1.1.11.2 $ $Date: 1996/02/18 19:42:18 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton ***********************************************************************/
/*                                                                            */
/* TYPE         : MODUL                                                       */
/*                                                                            */
/* NAME         : d20admin.c                                                  */
/*                                                                            */
/* AUTHOR       : Fischer, D AP 11                                            */
/* DATE         : 02.02.89                                                    */
/*                                                                            */
/* COMPONENT    : DS                                                          */
/*                                                                            */
/* DOC.-NR.     : Directory-Design-Specification                              */
/*                      .                                                     */
/*                      .                                                     */
/* PRD#/VERS.   :                                                             */
/*                                                                            */
/* DESCRIPTION  : The login menu mask and the masks of the main func-         */
/*                tions are prepared and shown by this program. If            */
/*                the program gdsditadm is called with parameters,            */
/*                the output of the login menu mask is suppressed.            */
/*                You can call gdsditadm with the following parameters:       */
/*                  d20admin [ -fc [ -D -c? -d? -m? -C? -p -s? -u? -X]]       */
/*                  d20admin [ -fA [ -p -X]]                                  */
/*                  d20admin [ -fd [ -p -X]]                                  */
/*                  d20admin [ -fs [ -d? -F? -k? -M? -n? -o? -p -v? -X]]      */
/*                  d20admin [ -fr [ -d? -k? -M? -n? -o? -p -v? -X]]          */
/*                  d20admin [ -fi [ -D -p -X]]                               */
/*                  d20admin [ -fT [ -p -X]]                                  */
/*                  d20admin [ -ft [ -p -X]]                                  */
/*                                                                            */
/* SYSTEM DEPENDENCIES: COMMON                                                */
/*                                                                            */
/* HISTORY      :                                                             */
/*                                                                            */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM#         */
/*  0.1     |02.02.89 |  original                      |    |                 */
/*datoff **********************************************************************/

/******************************************************************************/
/*    P R E P R O C E S S O R   I N S T R U C T I O N S                       */
/******************************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <stdio.h>
#include <pwd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <limits.h>
#include <nl_types.h>
#include <locale.h>
#include <errno.h>
#include <fcntl.h>
#include <ctype.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d2msk.h>
#include <cget.h>
#include <d20_adm.h>
#include <d20proto.h>



/******************************************************************************/
/*     D E F I N I T I O N S                                                  */
/******************************************************************************/

extern signed32     m_dir_id ;              /* Directory Identifier           */
extern signed32     m_authmech ;            /* authentication mechanism       */
#ifdef __hpux
extern              define_keys();
extern              term_type();
#endif

extern char         wahl[] ;                /* Input in login menu mask       */
extern signed32     sprache ;               /* chosen language                */
extern signed32     m_conf_typ ;            /* chosen configuration type      */
extern signed32     m_conf_mode ;           /* chosen configuration mode      */
extern signed32     m_medium ;              /* medium (floppy, tape, file)    */
extern signed32     m_update ;              /* delta update: yes or no        */
extern signed32     no_server ;             /* number of server               */
extern signed32     no_client ;             /* number of clients              */
extern char         *m_format[] ;           /* Formate mode                   */
extern signed32     m_form ;                /* chosen formate mode            */
extern signed32     m_opmode ;              /* Operation mode for save/restore*/
extern signed32     m_volumeno ;            /* Number of the volumes          */
extern Bool         m_print_err ;           /* flag for printing message      */
extern char         m_userkey[] ;           /* user key (save and restore)    */
extern char         m_filename[] ;          /* filename (save and restore)    */

					    /* fields for d2config            */
extern char         *conftyp[] ;            /* d2config: configuration type   */
extern char         *dbtype[] ;             /* d2config: data base            */
extern char         *update[] ;             /* d2config: delta update         */

					    /* Masks                          */
extern D20_omask    diradm_mask1[] ;        /* Login menu masks               */
extern D20_omask    diradm_mask2[] ;
extern D20_omask    menupar_mask[] ;        /* Menuparameter mask             */
extern D20_omask    confpar_mask[] ;        /* Configuration masks            */
extern D20_omask    conf1_mask[] ;
extern D20_omask    conf1d_mask[] ;
extern D20_omask    conf2_mask[] ;
extern D20_omask    conf3_mask[] ;
extern D20_omask    conf_mask[] ;
extern D20_omask    info_mask[] ;           /* State information masks        */
extern D20_omask    info1_mask[] ;
extern D20_omask    dirsi_mask[] ;          /* Masks to save data             */
extern D20_omask    dirsiA_mask[] ;         /* Masks to save data (A)         */
extern D20_omask    dirsiF_mask[] ;         /* Masks to save data (F)         */
extern D20_omask    dirsi0_mask[] ;
extern D20_omask    dirsi1_mask[] ;
extern D20_omask    dirsi2_mask[] ;
extern D20_omask    dirsi3_mask[] ;
extern D20_omask    dirsi4_mask[] ;
extern D20_omask    dirsi4F_mask[] ;
extern D20_omask    direi_mask[] ;          /* Masks to restore data          */
extern D20_omask    direiA_mask[] ;         /* Masks to restore data (A)      */
extern D20_omask    direiF_mask[] ;         /* Masks to restore data (F)      */
extern D20_omask    direi0_mask[] ;
extern D20_omask    direi1_mask[] ;
extern D20_omask    dirsiei1F_mask[] ;
extern D20_omask    direi2_mask[] ;
extern D20_omask    direi3_mask[] ;
extern D20_omask    direi3F_mask[] ;
extern D20_omask    error_mask[] ;          /* Error mask                     */
extern D20_omask    clear_mask[] ;          /* Clear mask                     */
extern char         *err_mess[MAXERRMSG] ;  /* error messages                 */
extern nl_catd      nl_help_fd;		    /* for help masks 		      */
extern signed32     getwerg;
extern char         *d20_hdlmsk_errmsg[];   /* Error messages for handlemask */

extern signed32     m_dbtype ;              /* database system   */
					    /* local variables                */
static char         *cl_sv_cs_ld;           /* possible configuation types    */
static char         *only_cl;               /* which can be chosen in the     */
static char	    *only_no;		    /* conf mask (depending on de-    */
static char	    *no_yes;		    /* livery units)                  */

static char         etc_path [D27_LFILE_NAME] ;
static char         dir1_path [D27_LFILE_NAME] ;
static char         dir2_path [D27_LFILE_NAME] ;
static char         *dir ;
static char         *logs ;

static signed32     taste;
static int	    rv;
static Bool         err_flag ;
static signed32     input;
static Bool         trace = FALSE ;
static FILE         *log_fd = NULL ;
static FILE         *lock_fd;

/* lock function if already somebody is saving, restoring or activating */
static struct flock    flock_arg = { F_WRLCK, SEEK_SET, 0L, 0L };

static char         cmdbuf[CMDBUF] ;
static char         admbuf[CMDBUF] ;
static char         tmpbuf[L_ZEILE] ;
static char         si1_tmp[L_ZEILE] ;
static char         si3_tmp[L_ZEILE] ;
static char         si4_tmp[L_ZEILE] ;
static char         ei1_tmp[L_ZEILE] ;
static char         ei2_tmp[L_ZEILE] ;
static char         ei3_tmp[L_ZEILE] ;
static char         log_n[D27_LFILE_NAME];
static char         log_name[D27_LFILE_NAME] ;
static char         func_array[NO_OF_FUNC];
static signed32     func_pos[] = { POS_AD, POS_KO, POS_AK, POS_DE, POS_SI,
		    POS_EI, POS_WE, POS_DS, POS_TA, POS_TE };

signed32            sel_cmd;                /* selected command               */
signed32            act_params;             /* actual parameters (bit field)  */
Bool                m_display = FALSE;   /* TRUE: mask displayed           */
					    /* FALSE: mask not displayed      */
Bool                empty_mask = TRUE;   /* indicates whether an empty mask*/
					    /* for saving or restoring of data*/
					    /* has to be shown                */
					    /* TRUE: show empty mask          */
					    /* FALSE: don't show empty mask   */
Bool                init_cget = FALSE;   /* flag indicating whether "cget" */
					    /* was already initialised        */
Bool                ret_value;              /* return value of function       */


/******************************************************************************/
/*     F U N C T I O N   D E C L A R A T I O N S                              */
/******************************************************************************/

static signed32 systemfkt (char *cmd );
static signed32 d20_read_mask_file(void);
static signed32 exit_func(signed32 ex_value);
static signed32 set_lock(signed32 flag, signed32 id);
static d2_ret_val error_mess_nr(signed32, signed16 *, signed16 *);

/* These functions are not defined currently
 * static d2_ret_val d20_shell_call()
 * static d2_ret_val d20_administration_call()
 * static d2_ret_val d20_configure()
 * static d2_ret_val d20_activate()
 * static d2_ret_val d20_deactivate()
 * static d2_ret_val d20_save()
 * static d2_ret_val d20_restore()
 * static d2_ret_val d20_state_inform()
 * static d2_ret_val d20_trace_activate()
 * static d2_ret_val d20_trace_deactivate()
 * static d2_ret_val d20_admin_trace_activate()
 * static d2_ret_val d20_admin_trace_deactivate()
 */



/******************************************************************************/
/*     C O D E                                                                */
/******************************************************************************/

 int main(
   int   argc,
   char *argv[])
{
    register signed32    i, k, result = 0;
    FILE     *fd, *fdvol;
    FILE     *in_fd;
    signed32 nopar, no_lines, old_conf_typ, old_dbtype, volno;
    signed32 o_noserver, o_noclient, o_dbtype, o_update, o_conf_type,
								o_authmech;
    Bool     dirdisabled, contflag ;
    Bool     all_flag ;             /* TRUE: all parameters are given */
    Bool     res = FALSE ; 
    Bool     masks_read = FALSE; /* indicates whether NLS mask file*/
    Bool     super_err_mess_flag = FALSE;	/* whether another set	*/
						/* of error message	*/
						/* should be used	*/

				    /* was already read               */
    struct passwd   *pwptr ;

    Bool     adm_flag = FALSE;   /* indicates whether gdsditadm exists */
    Bool     dsa_flag = FALSE;   /* indicates whether gdsdsa exists    */
    struct stat statinfo;
    signed32 fkt;
    char	sav_wahl;
#ifdef __hpux
    int terminal = 0;
#endif
    setlocale(LC_ALL, "");

    /* variables for calling with parameters (change configuration) */
    o_noserver = o_noclient  = o_dbtype   =
    o_update   = o_conf_type = o_authmech = -1;

    /* check whether "gdsditadm" was called with trace or help request */
    {
    char trace_arg[3], help_arg[3];

	trace_arg[0] = '-';
	trace_arg[1] = D20_XON;
	trace_arg[2] = D2_EOS;
	help_arg[0] = '-';
	help_arg[1] = D20_CLHELP;
	help_arg[2] = D2_EOS;

	for (i = 0, trace = m_print_err = FALSE; i < argc; i++)
	{   if (strcmp(argv[i], trace_arg) == 0)	/* arg is -X	*/
	    {   m_print_err = TRUE;
		break;
	    }
	    else if(strcmp(argv[i], help_arg) == 0)	/* arg is -h	*/
		d20_disp_clhelp();	/* Display help on stdout	*/
	}
    }

    /* set pathes for '$CL', '$SV' and etc-path	*/
    sprintf (dir1_path, "%s%s",dcelocal_path, D27_CLIENT_DIR);
    sprintf (dir2_path, "%s%s", dcelocal_path, D27_SERVER_DIR);
    sprintf (etc_path, "%s%s",dcelocal_path, D27_ETC_DIR);
    dir = dir1_path;
    logs = getenv (TRACE) ;

    if (dir == (char *) NULL ) {
	    if (m_print_err == TRUE)
	    {   fprintf (stderr, FAIL_ERROR) ;
		sleep (SLEEP_TIME) ;
	    }
	    exit (EXIT52) ;
    }

    /* remove logfiles */
    sprintf (cmdbuf, FINDCMD, dir1_path, D27_ADM_DIR);
    systemfkt(cmdbuf);

    /* set trace mode */
    trace = m_print_err;
    if (logs != (char *) NULL && strcmp (logs, LOG_ON) == 0)
	trace = TRUE ;

			/*************************************************/
			/* set user-ID and group-ID (necessary to create */
			/* files and directories)                        */
			/*************************************************/
    sprintf (cmdbuf, USERCMD, dir1_path) ;
    if ((fd = (FILE *) popen (cmdbuf, "r")) == (FILE *) NULL) {
	    if (m_print_err == TRUE) {
		fprintf (stderr, FAIL_ERR2) ;
		sleep (SLEEP_TIME) ;
	    }
	    exit (EXIT57) ;
    }
    if (fscanf (fd, "%s", cmdbuf) != 1) {
	    if (m_print_err == TRUE) {
		fprintf (stderr, FAIL_ERR2) ;
		sleep (SLEEP_TIME) ;
	    }
	    exit (EXIT57) ;
    }
    fclose (fd) ;

    if ((pwptr = (struct passwd *) getpwnam (cmdbuf)) == NULL) {
	    if (m_print_err == TRUE) {
		fprintf (stderr, FAIL_ERR2) ;
		sleep (SLEEP_TIME) ;
	    }
	    exit (EXIT57) ;
    }
    setuid (pwptr->pw_uid) ;
    setgid (pwptr->pw_gid) ;

		/* set logfile name and open it, if trace = TRUE */
    sprintf (log_name, LOGFNAME, dir1_path, getpid()) ;
    if (trace == TRUE)
    {       if ((log_fd = fopen (log_name, "a")) == (FILE *)0)
	    {       trace = FALSE ;
		    strcpy(log_n, NULLDEV);
	    }
	    else
	    {       strcpy (log_n, log_name) ;
				/* trace arguments */
		    fprintf(log_fd, FUNCMSG);
		    for (i = 0; i < argc; i++)
			fprintf(log_fd, "%s ", argv[i]);
		    fprintf(log_fd, "\n");
	    }
    }
    else
	    strcpy(log_n, NULLDEV);

    /* Checks, if there are parameters and write them into act_params */
    d20_get_params(argc, argv, etc_path, &sel_cmd, &act_params, &all_flag);

    /* Reading of language file for masks */
    if (all_flag == FALSE) {
	    if (d20_read_mask_file() == D2_ERROR) {
		if (m_print_err == TRUE) {
			fprintf(stderr, FAIL_ERR3);
			sleep(SLEEP_TIME);
		}
		exit(EXIT53);
	    }
	    else
	    {   masks_read = TRUE;
	    }
    }
    else
    {       masks_read = FALSE;
    }

    /* Check state of delta update operations */
    if (argc <= 1)
    {
	sprintf (cmdbuf, CHECK_UPD, dir2_path, dir2_path) ;
	if (systemfkt (cmdbuf) != 0)
	    diradm_mask1[UPDMSG].d20_fkt_id = D20_CSAY ;
    }

    /* Set mask parameters */
    getdel[0] = 0 ;
    getfill = '_' ;

    if (all_flag != TRUE || ((sel_cmd == SI || sel_cmd == EI)
	&& (act_params & D20C_OPMODE) != D20C_OPMODE))
    {   cgetinit () ;
	init_cget = TRUE;
    }

    /* output of administration masks in an for-ever-loop */
    sav_wahl = wahl[0] = func_array[0];
    for (;;) {
	if (argc == 1) {        /* Call of gdssysadm without parameters */

#ifdef __hpux
	    if ((terminal = term_type()) == 1) {
               fputs ("\033&s1A", stdout); /* set HP terminal to use key pad */
               define_keys(1, 0, "   f1     HELP  ", "\033>");
               define_keys(2, 0, "   f2     MENU  ", "\012");
               define_keys(3, 0, "   f3    SCL-UP ", "\033S");
               define_keys(4, 0, "   f4    SCL-DN ", "\033T");
               define_keys(7, 0, "   f7    CANCEL ", "\003");
               define_keys(8, 0, "   f8     EXIT  ", "\004");
               fflush  (stdout);
	    }
#endif

	    getwerg = 0;
	    wahl[0] = sav_wahl;
	    if ((taste = d20_handlemask(diradm_mask1)) == FKTEND)
		    exit_func(EXIT50) ;

	    diradm_mask1[UPDMSG].d20_fkt_id = D20_NO_OP ;
	    if ((sav_wahl = wahl[0]) == func_array[WE]) {
		    /* change from diradm_mask1 to diradm_mask2 is chosen */
		    getwerg = 0;
		    wahl[0]=diradm_mask2[POS_DS].d20_ioinfo[0];
		    if ((taste = d20_handlemask(diradm_mask2)) == FKTDEL)
			    continue;
		    if (taste == FKTEND)
			    exit_func(EXIT50) ;
	    }
	    for (input = 0; input < NO_OF_FUNC; input++) {
		if (wahl[0] == func_array[input])
		    break;
	    }
	}
	else {                  /* Call of gdssysadm with parameters */
	    input = sel_cmd;
	}


	err_flag = FALSE;
	m_display = FALSE;
	result = 0;

	switch ((int) input) {
	case SH:     /* open a subshell  */
		cerase () ;
		cgetterm () ;
		systemfkt (SHELL) ;
		cgetinit () ;
		cerase () ;
		init_cget = TRUE;
		break ;
	case AD:     /* Administration   */
		sprintf (tmpbuf, ADMPATH, dcelocal_path, D27_BIN_DIR);
		if (adm_flag == FALSE)
		      if (stat(tmpbuf, &statinfo) == 0)
			   {
			   adm_flag = TRUE;
			   sprintf (admbuf, ADMDIT, dir1_path, log_n) ;
			   }
		      else
			   sprintf (admbuf, ADMCACHE, dir1_path, log_n) ;
		if ((result = systemfkt (admbuf)) != 0) {
			err_flag = TRUE ;
			result=EXIT40;
		}
		m_display = TRUE;
		break ;
	case KO:     /* Configuration    */
		for (;;) {
			if ((ret_value = d20_displ_mask(act_params, confpar_mask,
					  sel_cmd, &taste)) == TRUE) {
				m_display = TRUE;
				if (taste == FKTDEL || taste == FKTCR || taste == FKTMENU)
					break;
			}
			else
				break;
		}
		if (argc > 1 && taste == FKTDEL)
			exit_func(EXIT56);
		if (ret_value == TRUE && taste == FKTDEL)
			continue;
		if (ret_value == FALSE && all_flag == FALSE)
			d20_handlemask(conf_mask);

	        if ((act_params & D20C_CONFTYPE) == 0)
		       m_conf_typ = CLT_GLOBAL;

		/* set configuration types for output of conf mask */
		/* depending on the installed delivery units       */
		conf2_mask[POS_CONF_TYPE].d20_oformat = cl_sv_cs_ld;
		sprintf (tmpbuf, DSAPATH, dcelocal_path, D27_BIN_DIR);
		if (dsa_flag == FALSE)
		      if (stat(tmpbuf, &statinfo) != 0)
			   conf2_mask[POS_CONF_TYPE].d20_oformat = only_cl;
		      else
                	   dsa_flag = TRUE;
		switch ((int) m_conf_mode) {
		case KCRE:      /* create configuration data    */
			/* Update information is not available	*/
			/* at creation / configuration time.	*/
			m_update = 0;

			if (d20_displ_mask(act_params, conf1_mask, sel_cmd,
					   &taste) == TRUE) {
				m_display = TRUE;
				if (taste == FKTDEL) {
					if (argc > 1)
						exit_func(EXIT56);
					else
						continue;
				}
			}
			if ((act_params & D20C_NOCLIENT) != D20C_NOCLIENT)
			    no_client = D20_3NOCLIENT ;
			contflag = FALSE ;
			while (d20_displ_mask(act_params, conf2_mask, sel_cmd,
								      &taste)) {
				m_display = TRUE;
				if (taste == FKTDEL) {
					if (argc > 1)
						exit_func(EXIT56);
					else {
						contflag = TRUE ;
						break ;
					}
				}
				if (no_client >= D20_1NOCLIENT && no_client <=
								D20_2NOCLIENT)
					break ;
				error_mask[1].d20_ioinfo = err_mess[28] ;
				d20_handlemask (error_mask);
				d20_handlemask (clear_mask);
			}
			if (contflag) continue ;
		
			if (m_conf_typ != CLT_GLOBAL) {
				if ((act_params & D20C_NOSERVER) != D20C_NOSERVER)
				    no_server = D20_1NOSERVER + 1;

				contflag = FALSE ;
				while (d20_displ_mask(act_params, conf3_mask,
							     sel_cmd, &taste)) {
					m_display = TRUE;
					if (taste == FKTDEL) {
						if (argc > 1)
							exit_func(EXIT56);
						else {
							contflag = TRUE ;
							break ;
						}
					}
					if (no_server >= D20_1NOSERVER &&
					    no_server <= D20_2NOSERVER)
						break ;
					error_mask[1].d20_ioinfo = err_mess[28] ;
					d20_handlemask (error_mask);
					d20_handlemask (clear_mask);
				}
				if (contflag) continue ;

				sprintf(cmdbuf, CONFFKT, dir1_path, dir1_path,
					m_conf_mode, m_dir_id,
					conftyp[m_conf_typ],
					no_server, update[m_update],
					dbtype[m_dbtype], no_client, m_authmech,
					etc_path, dir2_path, dcelocal_path, log_n);
			} else {
				sprintf(cmdbuf, CONFFKT, dir1_path, dir1_path,
					m_conf_mode, m_dir_id,
					conftyp[m_conf_typ],
					0, update[0], dbtype[0], no_client, -1,
					etc_path, dir2_path, dcelocal_path,
					log_n);
			}
			if ((result = systemfkt (cmdbuf)) != 0)
				err_flag = TRUE;
			break ;
		case KDEL:      /* delete configuration data    */
			if (!(m_dir_id > D20_1DIRID && m_dir_id <= D20_2DIRID))
			    m_dir_id = D20_1DIRID + 1;
			if (d20_displ_mask(act_params, conf1d_mask, sel_cmd,
					   &taste) == TRUE) {
				m_display = TRUE;
				if (taste == FKTDEL) {
					if (argc > 1)
						exit_func(EXIT56);
					else
						continue;
				}
			}
			sprintf(cmdbuf, CONFFKT, dir1_path, dir1_path,
				m_conf_mode, m_dir_id, "-", 0, "-", "-", 0, -1,
				etc_path, dir2_path, dcelocal_path, log_n);
			if ((result = systemfkt (cmdbuf)) != 0)
				err_flag = TRUE;
			break ;
		case KDIS:      /* display configuration data   */
			if ((act_params & D20C_SUPP_MASK) == D20C_SUPP_MASK) {
				sprintf(cmdbuf, CONFFKT, dir1_path, dir1_path,
					(m_conf_mode * 10) + 2, 0, "-",
					0, "-", "-", 0, -1,
					etc_path, dir2_path, dcelocal_path, log_n);
				if ((result = systemfkt (cmdbuf)) != 0)
					err_flag = TRUE;
				break ;
			}
			sprintf(cmdbuf, CONFFKT, dir1_path, dir1_path,
				m_conf_mode, 0, "-", 0, "-", "-", 0, -1,
				etc_path, dir2_path, dcelocal_path, log_n);
			if ((result = systemfkt (cmdbuf)) != 0) {
				err_flag = TRUE;
				break;
			} else {
				d20_handlemask(info1_mask);
				m_display = TRUE;
			}
			sprintf (cmdbuf, CONFISIZECMD, etc_path);
			if ((in_fd = (FILE *) popen (cmdbuf,"r")) == (FILE *) NULL)
			{       result = EXIT2;
				err_flag = TRUE;
				break;
			}
			if (fscanf(in_fd,"%d", &no_lines) != 1)
			{       result = EXIT2;
				err_flag = TRUE;
				if (trace) {
					fprintf (log_fd, "reading no. of lines of file d2config failed") ;
					fflush (log_fd) ;
				}
				pclose(in_fd);
				break;
			}
			pclose(in_fd);
			if (no_lines > DISPLINENO) {
				confpar_mask[D20_P0CONFMODE].d20_fkt_id = D20_EOMSK ;
				d20_handlemask(confpar_mask);
				confpar_mask[D20_P0CONFMODE].d20_fkt_id = D20_CSAY ;
				m_display = TRUE;
				sprintf(cmdbuf, CONFFKT, dir1_path, dir1_path, 
				(m_conf_mode * 10) + 1, 0, "-", 0, "-", "-", 0, 
				 -1, etc_path, dir2_path, dcelocal_path, log_n);
				if ((result = systemfkt (cmdbuf)) != 0)
					err_flag = TRUE;
				else {
					d20_handlemask(info1_mask);
					m_display = TRUE;
				}
			}
			break ;
		case KCHG:      /* change configuration data    */
		    	if ((act_params & D20C_SUPP_MASK) != D20C_SUPP_MASK) {
				if (d20_displ_mask(act_params, conf1_mask,
					   sel_cmd, &taste) == TRUE) {
					m_display = TRUE;
					if (taste == FKTDEL) {
						if (argc > 1)
							exit_func(EXIT56);
						else
							continue;
					}
				}
		    	}
			sprintf (cmdbuf, PREPMASK, etc_path, m_dir_id, etc_path);
			if (trace) {
				fprintf (log_fd, REQCMDTRACE, cmdbuf) ;
				fflush (log_fd) ;
			}
			if ((in_fd = (FILE *) popen(cmdbuf,"r")) == (FILE *) NULL)
			{       result = EXIT2;
				err_flag = TRUE;
				break;
			}
			if (argc > 1) {
				/* The update field should not be changed. */
				{
				char current_update;	/* to write the */
							/* value of	*/
							/* gdsconfig	*/

				    if( d20_check_update( m_dir_id,
							 &current_update)
							 == D2_ERROR )
					/* Value was not found	*/
					o_update = 0;
				    else
					if(current_update == UPDATE_OFF)
					    o_update = 0;
					else /* current_update == UPDATE_ON */
					    o_update = 1;
				}

				if ((act_params & D20C_NOSERVER) == D20C_NOSERVER)
					o_noserver = no_server;
				if ((act_params & D20C_DBTYPE) == D20C_DBTYPE)
					o_dbtype = m_dbtype;
				if ((act_params & D20C_NOCLIENT) == D20C_NOCLIENT)
					o_noclient = no_client;
				if ((act_params & D20C_CONFTYPE) == D20C_CONFTYPE)
					o_conf_type = m_conf_typ;
				if ((act_params & D20C_AUTH_MECH) == D20C_AUTH_MECH)
					o_authmech = m_authmech;
			}
			if ((rv = fscanf(in_fd,"%d%d%d%d%d%d", &m_conf_typ,
				   &no_server, &m_update, &m_dbtype, &no_client,
				   &m_authmech)) != 6)
			{       result = EXIT2;
				err_flag = TRUE;
				if (trace)
				{
					fprintf (log_fd, REQCMDTRACE, "fscanf failed") ;
					fprintf (log_fd, "rv of fscanf = %d\nconf_type = %d\nno_server = %d\nupdate = %d\ndbtype = %d\nno_client = %d\n",
						rv,m_conf_typ,no_server,m_update,m_dbtype, no_client);
					fflush (log_fd) ;
				}
				pclose(in_fd);

				if (m_authmech == -1)
					m_authmech = D2_SIMPLE;

				break;
			}
			pclose(in_fd);
			if (m_conf_typ == -1) {
				/* no configuration information available */
				m_conf_typ = 0;
				result = EXIT6;
				err_flag = TRUE;
				break;
			}

			old_conf_typ = m_conf_typ;
			old_dbtype = m_dbtype;

			/* check which parameters have been inserted by
			   calling from shell command level */
			if (o_noserver != -1) no_server = o_noserver;
			if (o_noclient != -1) no_client = o_noclient;
			if (o_conf_type != -1) m_conf_typ = o_conf_type;
			if (o_dbtype != -1) m_dbtype = o_dbtype;
			if (o_update != -1) m_update = o_update;
			if (o_authmech != -1) m_authmech = o_authmech;

		    	if ((act_params & D20C_SUPP_MASK) != D20C_SUPP_MASK) {
				contflag = FALSE ;
				while (d20_displ_mask(act_params, conf2_mask,
						sel_cmd, &taste)) {
					m_display = TRUE;
					if (taste == FKTDEL) {
						if (argc > 1)
							exit_func(EXIT56);
						else {
							contflag = TRUE ;
							break ;
						}
					}
					if (no_client >= D20_1NOCLIENT &&
				    	no_client <= D20_2NOCLIENT)
						break ;
					error_mask[1].d20_ioinfo = err_mess[28];
					d20_handlemask (error_mask);
					d20_handlemask (clear_mask);
				}
				if (contflag) continue ;
		    	}

			if (m_conf_typ != CLT_GLOBAL) {
				/* new type is Client/Server */
			    if ((act_params & D20C_SUPP_MASK) != D20C_SUPP_MASK) {
				contflag = FALSE ;
				while (d20_displ_mask(act_params, conf3_mask,
							     sel_cmd, &taste)) {
					m_display = TRUE;
					if (taste == FKTDEL) {
						if (argc > 1)
							exit_func(EXIT56);
						else {
							contflag = TRUE ;
							break ;
						}
					}
					if (no_server >= D20_1NOSERVER &&
					    no_server <= D20_2NOSERVER)
						break ;
					error_mask[1].d20_ioinfo = err_mess[28] ;
					d20_handlemask (error_mask);
					d20_handlemask (clear_mask);
				}
				if (contflag) continue ;
			    }
			}
			if (m_conf_typ != old_conf_typ ||
			    (old_conf_typ != CLT_GLOBAL && old_dbtype != m_dbtype)) {
				/* delete configuration */
				sprintf(cmdbuf, CONFFKT, dir1_path, dir1_path,
					KDEL, m_dir_id, "-", 0, "-", "-", 0, -1,
					etc_path, dir2_path, dcelocal_path, log_n);
				if ((result = systemfkt (cmdbuf)) != 0) {
					err_flag = TRUE;
					break;
				}
				if (m_conf_typ != CLT_GLOBAL)
					/* create configuration for Client/Server */
					sprintf(cmdbuf, CONFFKT, dir1_path, dir1_path,
						KCRE, m_dir_id,
						conftyp[m_conf_typ], no_server,
						update[m_update], dbtype[m_dbtype], no_client, m_authmech,
						etc_path, dir2_path, dcelocal_path, log_n);
				else
					/* create configuration for Client */
					sprintf(cmdbuf, CONFFKT, dir1_path, dir1_path,
						KCRE, m_dir_id,
						conftyp[m_conf_typ], 0, update[0],
						dbtype[0], no_client, -1,
						etc_path, dir2_path, dcelocal_path, log_n);
			}
			else {
				if (m_conf_typ == CLT_GLOBAL) {
					/* change configuration for Client */
					/* without removing data	   */
					sprintf(cmdbuf, CONFFKT, dir1_path, dir1_path,
						m_conf_mode, m_dir_id, conftyp[m_conf_typ],
						0, update[0], dbtype[0], no_client, -1,
						etc_path, dir2_path, dcelocal_path, log_n);
				}
				else {
					/* change configuration for Client/ */
					/* Server without removing data     */
					sprintf(cmdbuf, CONFFKT, dir1_path, dir1_path,
						m_conf_mode, m_dir_id, conftyp[m_conf_typ],
						no_server, update[m_update],
						dbtype[m_dbtype], no_client, 
						m_authmech,
						etc_path, dir2_path, dcelocal_path, log_n);
				}
			}
			if ((result = systemfkt (cmdbuf)) != 0)
				err_flag = TRUE;
			break;
		default:
			break ;
		}
		break;
	case AK:     /* Activation       */
		/* lock file */
		if ((result = set_lock(D2_LOCK, 0)) != 0) {
			err_flag = TRUE;
			break;
		}
		sprintf(cmdbuf, ACTFKT, dir1_path, dir1_path,
			etc_path, dir2_path, log_n);
		if ((result = systemfkt (cmdbuf)) != 0) {
			/* unlock file */
			if (set_lock(D2_UNLOCK, 0) != 0)
				fclose(lock_fd);
			err_flag = TRUE;
			super_err_mess_flag = TRUE;
			break;
		}
		/* unlock file */
		if ((result = set_lock(D2_UNLOCK, 0)) != 0)
			err_flag = TRUE;
		fclose(lock_fd);
		break;
	case DE:     /* Deactivation */
		sprintf(cmdbuf, DEACTFKT, dir1_path, etc_path, log_n);
		if ((result = systemfkt (cmdbuf)) != 0)
			err_flag = TRUE;
		break;
	case SI:     /* Saving           */
		empty_mask = TRUE;
		if ((act_params & D20C_USERKEY) == 0)
		{   m_userkey[0] = D2_EOS;
		}
		if ((act_params & D20C_FILENAME) == 0)
		{   m_filename[0] = D2_EOS;
		}
		if (all_flag == FALSE) {
			if (d20_displ_mask(act_params, dirsi_mask, sel_cmd,
					   &taste) == TRUE) {
			    m_display = TRUE;
			    empty_mask = FALSE;
			    if (taste == FKTDEL) {
				if (argc > 1)
					exit_func(EXIT56);
				else
					break;
			    }
			}
			if (empty_mask == TRUE)
			{       d20_handlemask (dirsi0_mask) ;
				empty_mask = FALSE;
				m_display = TRUE;
			}
			for (;;) {
				if (m_medium != MEDIUM_FILE)
					/* save on diskette or tape */
					res=d20_displ_mask(act_params, dirsiA_mask,
					   	   	   sel_cmd, &taste) ;
				else 
					/* save in file */
					res=d20_displ_mask(act_params, dirsiF_mask,
					   	   	   sel_cmd, &taste) ;

				if (res == TRUE) {
			    		m_display = TRUE;
			    		empty_mask = FALSE;
			    		if (taste == FKTDEL) {
						if (argc > 1)
							exit_func(EXIT56);
						else
							break;
			    		}
				}
				for (k = strlen(m_userkey); k >= 0; k--)
				{   if (m_userkey[k] == '_')
					m_userkey[k] = D2_EOS;
				}

				/* check if filename is correct */
				if (m_medium == MEDIUM_FILE &&
				    is_allowed(m_filename) == D2_ERROR)
					result = EXIT28;

		    		if (result != 0)
			    		{
			    		error_mask[1].d20_ioinfo = (result >= EXIT1 && result <= MAX_EXIT) ?
									err_mess[result>>8] : err_mess[EXIT2 >> 8] ;
			    		d20_handlemask (error_mask);
					d20_handlemask (clear_mask);
					result = 0;
			    		continue;
			    		}
		    		else
			    		break;
			}

			if (taste == FKTDEL)
				break;

			if (empty_mask == TRUE)
			{       d20_handlemask (dirsi0_mask) ;
				empty_mask = FALSE;
				m_display = TRUE;
			}
			d20_handlemask (dirsi2_mask) ;
			sprintf (cmdbuf, DBWRIT1FKT, dir1_path,
				 dcelocal_path, m_dir_id,
				 m_userkey[0] == D2_EOS ? "-" : m_userkey,
				 m_medium, "-", 
				 m_filename[0] == D2_EOS ? "-" : m_filename,
				 log_n);
			result = systemfkt (cmdbuf) ;
			if (result != 0) {
				if (result != EXIT8) {
					sprintf (cmdbuf, DBWRIT3FKT, dir1_path, dcelocal_path,
						m_dir_id, "-", "-", "-",
						m_filename[0] == D2_EOS ? "-": m_filename, log_n);
					systemfkt (cmdbuf) ;
				}
				err_flag = TRUE;
				break;
			}

			sprintf (cmdbuf, GETVOLCMD, dir1_path, m_dir_id, log_n) ;
			if ((fdvol = (FILE *) popen (cmdbuf, "r")) != (FILE *) NULL) {
				if ((nopar = fscanf (fdvol, "%d", &volno)) == 1) {
					pclose (fdvol) ;
					sprintf (dirsi3_mask[0].d20_ioinfo, si3_tmp, volno) ;
					if (empty_mask == TRUE)
					{   d20_handlemask (dirsi0_mask) ;
					    empty_mask = FALSE;
					    m_display = TRUE;
					}
					if (m_medium != MEDIUM_FILE)
					{
						d20_handlemask (dirsi3_mask) ;
					}
				}
			}
			if (fdvol == (FILE *) NULL || nopar != 1) {
				result = EXIT2;
				err_flag = TRUE;
				sprintf (cmdbuf, DBWRIT3FKT, dir1_path, dcelocal_path,
					 m_dir_id, "-", "-", "-",
					 m_filename[0] == D2_EOS ? "-" : m_filename, log_n);
				systemfkt (cmdbuf) ;
				break ;
			}

			result = 0;
			for (i = 1; i <= volno;) {
				sprintf (dirsi1_mask[2].d20_ioinfo, si1_tmp, i) ;
				if (empty_mask == TRUE)
				{   d20_handlemask (dirsi0_mask) ;
				    empty_mask = FALSE;
				    m_display = TRUE;
				}
				if (m_medium != MEDIUM_FILE)
					fkt = d20_handlemask (dirsi1_mask);
				else
					fkt = d20_handlemask(dirsiei1F_mask);
				if (fkt == FKTDEL)
				{   if (argc > 1)
				    {   err_flag = TRUE;
					result = EXIT56;
				    }
				    break ;
				}

				if (m_medium != MEDIUM_FILE) {
					sprintf (dirsi4_mask[0].d20_ioinfo, si4_tmp, i) ;
					fkt = d20_handlemask (dirsi4_mask) ;
				} else 
					fkt = d20_handlemask (dirsi4F_mask) ;

				sprintf (cmdbuf, DBWRIT2FKT, dir1_path, dcelocal_path,
					 m_dir_id, i, m_medium, m_format[m_form],
					 m_filename[0] == D2_EOS ? "-" : m_filename, log_n);
				result = systemfkt (cmdbuf) ;
				if (result != 0) {
					/* no save to file was selected -> insert new volume */
					if (result != EXIT36) {
						error_mask[1].d20_ioinfo = (result >= EXIT1 && result <= MAX_EXIT) ?
								   	err_mess[result>>8] : err_mess[EXIT2 >> 8] ;
						d20_handlemask (error_mask);
					}
					if (result != EXIT16 && result != EXIT17)
					{   err_flag = TRUE;
					    break;
					}
				} else {
					i++ ;
				}
			}

			sprintf (cmdbuf, DBWRIT3FKT, dir1_path, dcelocal_path,
				m_dir_id, "-", "-", "-", 
				m_filename[0] == D2_EOS ? "-" : m_filename,
				log_n);
			if (err_flag == TRUE)
			    systemfkt (cmdbuf) ;
			else
			    result = systemfkt (cmdbuf) ;
			if (result != 0) {
			    err_flag = TRUE;
			    if (argc > 1 && result == EXIT56)
				exit_func(EXIT56);
			}
		}
		else {
			switch ((int) m_opmode) {
			case 1:
				sprintf (cmdbuf, DBWRIT1FKT, dir1_path,
					 dcelocal_path, m_dir_id,
					 m_userkey[0] == D2_EOS ? "-" : m_userkey,
					 m_medium, "-", 
					 m_filename[0] == D2_EOS ? "-" : m_filename, log_n);
				if ((result = systemfkt(cmdbuf)) != 0) {
					err_flag = TRUE;
				}
				break;
			case 2:
				sprintf (cmdbuf, DBWRIT2FKT, dir1_path, dcelocal_path,
					 m_dir_id, m_volumeno, m_medium, m_format[m_form],
					 m_filename[0] == D2_EOS ? "-" : m_filename, log_n);
				if ((result = systemfkt(cmdbuf)) != 0) {
					err_flag = TRUE;
				}
				break;
			case 3:
				sprintf (cmdbuf, DBWRIT3FKT, dir1_path, dcelocal_path,
					 m_dir_id, "-", "-", "-",
					 m_filename[0] == D2_EOS ? "-" : m_filename, log_n);
				if ((result = systemfkt(cmdbuf)) != 0) {
					err_flag = TRUE;
				}
				break;
			case 4:
				sprintf (cmdbuf, GETVOLCMD, dir1_path, m_dir_id, log_n) ;
				if ((fdvol = (FILE *) popen (cmdbuf, "r")) != (FILE *) NULL) {
					if ((nopar = fscanf (fdvol, "%d", &volno)) == 1) {
						fprintf(stdout, "%d\n", volno);
					}
					else
					{       err_flag = TRUE;
						result = EXIT2;
					}
					pclose (fdvol) ;
				}
				else
				{       err_flag = TRUE;
					result = EXIT2;
				}
				break;
			default:
				break;
			}
		}
		break;
	case EI:     /* Restoring        */
		empty_mask = TRUE;
		if ((act_params & D20C_USERKEY) == 0)
		{   m_userkey[0] = D2_EOS;
		}
		if ((act_params & D20C_FILENAME) == 0)
		{   m_filename[0] = D2_EOS;
		}

		if (all_flag == FALSE) {
		    if (d20_displ_mask(act_params, direi_mask, sel_cmd,
				       &taste) == TRUE) {
			m_display = TRUE;
			empty_mask = FALSE;
			if (taste == FKTDEL) {
				if (argc > 1)
					exit_func(EXIT56);
				else
					break;
			}
		    }
		    if (empty_mask == TRUE)
		    {       d20_handlemask (direi0_mask);
			    empty_mask = FALSE;
			    m_display = TRUE;
		    }
		    for(;;) {
		    	if (m_medium != MEDIUM_FILE)
				/* restore from diskette or tape */
		    		res=d20_displ_mask(act_params, direiA_mask, 
						   sel_cmd, &taste) ; 
		    	else 
				/* restore from file */
		    		res=d20_displ_mask(act_params, direiF_mask, 
						   sel_cmd, &taste) ;

		    	if (res == TRUE) {
				m_display = TRUE;
				empty_mask = FALSE;
				if (taste == FKTDEL) {
					if (argc > 1)
						exit_func(EXIT56);
					else
						break;
				}
		    	}

		    	for (k = strlen(m_userkey); k >= 0; k--)
		    	{   if (m_userkey[k] == '_')
			    	m_userkey[k] = D2_EOS;
		    	}

			/* check if filename is correct */
			if (m_medium == MEDIUM_FILE &&
			    is_allowed(m_filename) == D2_ERROR)
				result = EXIT28;

		    	if (result != 0)
			    	{
			    	error_mask[1].d20_ioinfo = (result >= EXIT1 && result <= MAX_EXIT) ?
								err_mess[result>>8] : err_mess[EXIT2 >> 8] ;
			    	d20_handlemask (error_mask);
				d20_handlemask (clear_mask);
				result = 0;
			    	continue;
			    	}
		    	else
			    	break;
		    }

		    if (taste == FKTDEL)
			break;

		    volno = 1 ; dirdisabled = FALSE ;
		    result = 0;
		    for (i = 1; i <= volno;) {
			if (empty_mask == TRUE)
			{       d20_handlemask (direi0_mask);
				empty_mask = FALSE;
				m_display = TRUE;
			}
			if (m_medium != MEDIUM_FILE) {
				sprintf (direi1_mask[2].d20_ioinfo, ei1_tmp, i);
				fkt = d20_handlemask (direi1_mask); 
			} else
				fkt = d20_handlemask (dirsiei1F_mask);

			if (fkt == FKTDEL)
			{   if (argc > 1)
			    {   err_flag = TRUE;
				result = EXIT56;
			    }
			    break ;
			}
		    	if (empty_mask == TRUE)
		    	{       d20_handlemask (direi0_mask);
			    	empty_mask = FALSE;
			    	m_display = TRUE;
		    	}
			if (m_medium != MEDIUM_FILE) {
				sprintf (direi3_mask[0].d20_ioinfo, ei3_tmp, i) ;
				d20_handlemask (direi3_mask) ;
			} else
				d20_handlemask (direi3F_mask) ;
			sprintf (cmdbuf, DBREAD1FKT, dir1_path, dcelocal_path,
				 m_dir_id, i, m_medium, m_userkey[0] == D2_EOS ? "-" : m_userkey,
				 m_filename[0] == D2_EOS ? "-" : m_filename, log_n);
			result = systemfkt (cmdbuf) ;
			if (result != 0) {
				if (i == 1 && result != EXIT8)
					dirdisabled = TRUE ;
				if (result == EXIT13 || result == EXIT38 || result == EXIT39)
				{   /* illegal user key, wrong filename */
				    err_flag = TRUE;
				    break;
				}
				error_mask[1].d20_ioinfo = (result >= EXIT1 && result <= MAX_EXIT) ?
						   	err_mess[result>>8] : err_mess[EXIT2 >> 8] ;
				d20_handlemask (error_mask);
				continue ;
			}

			if (i == 1) {
				dirdisabled = TRUE ;
				sprintf (cmdbuf, GETVOLCMD, dir1_path, m_dir_id, log_n) ;
				if ((fdvol = (FILE *) popen (cmdbuf, "r")) != (FILE *) NULL) {
					if ((nopar = fscanf (fdvol, "%d", &volno)) == 1) {
						sprintf (direi2_mask[0].d20_ioinfo, ei2_tmp, volno) ;
		    				if (m_medium != MEDIUM_FILE)
						{
							d20_handlemask (direi2_mask) ;
						}
					}
					pclose (fdvol) ;
				}
				if (fdvol == (FILE *) NULL || nopar != 1) {
					result = EXIT2;
					err_flag = TRUE;
					break ;
				}
			}
			i++ ;
		    }
		    if (dirdisabled) {
			sprintf (cmdbuf, DBREAD2FKT, dir1_path, dcelocal_path,
				m_dir_id, "-", "-", "-",
				m_filename[0] == D2_EOS ? "-" : m_filename, log_n);
			if (err_flag == TRUE)
			    systemfkt (cmdbuf) ;
			else
			    result = systemfkt (cmdbuf) ;
			if (result != 0)
				err_flag = TRUE;
		    }
		    if (argc > 1 && result == EXIT56)
			exit_func(EXIT56);
		}
		else
		{   switch((int) m_opmode) {
		    case 1:
			    sprintf (cmdbuf, DBREAD1FKT, dir1_path, dcelocal_path,
				     m_dir_id, m_volumeno, m_medium, m_userkey[0] == D2_EOS ? "-" : m_userkey,
				     m_filename[0] == D2_EOS ? "-" : m_filename, log_n);
			    if ((result = systemfkt (cmdbuf)) != 0)
				err_flag = TRUE;
			    break;
		    case 2:
			    sprintf (cmdbuf, GETKEYCMD, dir1_path, m_dir_id, log_n) ;
			    if ((fdvol = (FILE *) popen (cmdbuf, "r")) != (FILE *) NULL) {
				if (fscanf (fdvol, "%s", m_userkey) != 1) {
				    err_flag = TRUE;
				    result = EXIT2;
				    pclose (fdvol) ;
				    break;
				}
				pclose (fdvol) ;
			    }
			    else
			    {   err_flag = TRUE;
				result = EXIT2;
				break;
			    }
			    sprintf (cmdbuf, DBREAD1FKT, dir1_path, dcelocal_path,
				     m_dir_id, m_volumeno, m_medium, m_userkey[0] == D2_EOS ? "-" : m_userkey,
				     m_filename[0] == D2_EOS ? "-" : m_filename, log_n);
			    if ((result = systemfkt (cmdbuf)) != 0)
				err_flag = TRUE;
			    break;
		    case 3:
			    sprintf (cmdbuf, DBREAD2FKT, dir1_path, dcelocal_path,
				    m_dir_id, "-", "-", "-",
				    m_filename[0] == D2_EOS ? "-" : m_filename, log_n);
			    if ((result = systemfkt (cmdbuf)) != 0)
				err_flag = TRUE;
			    break;
		    case 4:
			    sprintf (cmdbuf, GETVOLCMD, dir1_path, m_dir_id, log_n) ;
			    if ((fdvol = (FILE *) popen (cmdbuf, "r")) != (FILE *) NULL) {
				if ((nopar = fscanf (fdvol, "%d", &volno)) == 1) {
				    fprintf(stdout, "%d\n", volno);
				    pclose (fdvol) ;
				}
				else
				{   err_flag = TRUE;
				    result = EXIT2;
				}
			    }
			    else
			    {   err_flag = TRUE;
				result = EXIT2;
			    }
			    break;
		    default:
			    break;
		    }
		}
		break;
	case DS:     /* State information    */
		if ((act_params & D20C_SUPP_MASK) == D20C_SUPP_MASK) {
		    sprintf (cmdbuf, DSTATEFKT, dir1_path, dir1_path, dcelocal_path, "C", log_n) ;
		    if ((result = systemfkt (cmdbuf)) != 0)
			err_flag = TRUE ;
		} else {
		    sprintf (cmdbuf, DSTATEFKT, dir1_path, dir1_path, dcelocal_path, "S", log_n) ;
		    d20_handlemask (info_mask);
		    if ((result = systemfkt (cmdbuf)) != 0)
			err_flag = TRUE ;
		    else {
			d20_handlemask(info1_mask);
			m_display = TRUE;
		    }
		}
		break;
	case TA:     /* Trace on         */
		sprintf (cmdbuf, DLOGFKT, dir1_path, dir1_path,
			 LOG_ON, log_n) ;
		if ((result = systemfkt (cmdbuf)) != 0)
			err_flag = TRUE ;
		break;
	case TE:    /* Trace off        */
		sprintf (cmdbuf, DLOGFKT, dir1_path, dir1_path, LOG_OFF, log_n) ;
		if ((result = systemfkt (cmdbuf)) != 0)
			err_flag = TRUE ;
		break;
	case TON:    /* admin trace on   */
		trace = TRUE ;
		strcpy (log_n, log_name) ;
		if (log_fd == NULL)
			if ((log_fd = fopen (log_name, "a")) == (FILE *)0) {
				trace = FALSE ;
			}
		break ;
	case TOFF:   /* admin trace off  */
		trace = FALSE ;
		strcpy (log_n, NULLDEV) ;
		if (log_fd != NULL) {
			fclose (log_fd) ;
			log_fd = NULL ;
		}
		break ;
	default:
		err_flag = TRUE ;
		result = EXIT9;
		break;
	}
	/* output of errors */
	if (err_flag == TRUE && m_print_err == TRUE) {
		if (masks_read == FALSE) {
		    if (d20_read_mask_file() == D2_ERROR) {
			if (m_print_err == TRUE) {
			    fprintf(stderr, FAIL_ERR3);
			    sleep(SLEEP_TIME);
			}
			cgetterm();
			exit( (result >= EXIT1 && result <= MAX_EXIT)
			      ? (result>>8) : (EXIT2 >> 8));
		    }
		    masks_read = TRUE;
		}
		if (init_cget == FALSE)
		{   cgetinit();
		    init_cget = TRUE;
		}

		/* Find the right error message		*/
		{
		signed16 process_index, error_index;
		char error_message[MAX_ERR_MESS_SIZE];

		    if( super_err_mess_flag )
			if( error_mess_nr(
				result,
				&process_index,
				&error_index) == D2_NOERROR )
			{
			    switch(process_index)
			    {
				case GDSIPCINIT_IDX:
				    strcpy(error_message, GDSIPCINIT_NAME);
				    break;
				case GDSIPCCHK_IDX:
				    strcpy(error_message, GDSIPCCHK_NAME);
				    break;
				case GDSCACHE_IDX:
				    strcpy(error_message, GDSCACHE_NAME);
				    break;
				case GDSCSTUB_IDX:
				    strcpy(error_message, GDSCSTUB_NAME);
				    break;
				case GDSSSTUB_IDX:
				    strcpy(error_message, GDSSSTUB_NAME);
				    break;
				case GDSDSA_IDX:
				    strcpy(error_message, GDSDSA_NAME);
				    break;
				default:
				    error_message[0] = D2_EOS;
				    break;
			    }
			    strcat(error_message, err_mess[error_index]);
			    error_mask[1].d20_ioinfo = error_message;
			}
			else
			    super_err_mess_flag = FALSE;

		    if( !super_err_mess_flag )
			if (result >= EXIT1 && result <= MAX_EXIT)
			    error_mask[1].d20_ioinfo = err_mess[result>>8];
			else
			    error_mask[1].d20_ioinfo = err_mess[EXIT2 >> 8];

		    /* Print the error message		*/
		    d20_handlemask(error_mask);
		}
	}

	/* exit, if gdsditadm was called with parameters */
	if (argc > 1) {
		if (m_display == TRUE) cerase();
		if (init_cget == TRUE) cgetterm();
#ifdef __hpux
                if (terminal == 1) {
        	   define_keys(0, 2, NULL, NULL); /* reset to default keys */
        	   fputs ("\033&j@",stdout); /* reset HP terminal */
		}
#endif
		exit(result>>8);
	}
    }
}



/* Each case would be better in a separate function.
 *
 * static d2_ret_val d20_shell_call()
 * {
 * }
 * 
 * static d2_ret_val d20_administration_call()
 * {
 * }
 * 
 * static d2_ret_val d20_configure()
 * {
 * }
 * 
 * static d2_ret_val d20_activate()
 * {
 * }
 * 
 * static d2_ret_val d20_deactivate()
 * {
 * }
 * 
 * static d2_ret_val d20_save()
 * {
 * }
 * 
 * static d2_ret_val d20_restore()
 * {
 * }
 * 
 * static d2_ret_val d20_state_inform()
 * {
 * }
 * 
 * static d2_ret_val d20_trace_activate()
 * {
 * }
 * 
 * static d2_ret_val d20_trace_deactivate()
 * {
 * }
 * 
 * static d2_ret_val d20_admin_trace_activate()
 * {
 * }
 * 
 * static d2_ret_val d20_admin_trace_deactivate()
 * {
 * }
 *
 */



/*****************************************************************************/
/*                                                                           */
/*  This function makes the system calls and writes the log file.            */
/*                                                                           */
/*****************************************************************************/

 static signed32 systemfkt (
   char	*cmd )
{
    int     result ;

    if (trace) {
	    fprintf (log_fd, REQCMDTRACE, cmd) ;
	    fflush (log_fd) ;
    }
    result = system (cmd) ;
    if (trace) {
	    fprintf (log_fd, RESCMDTRACE, result >> 8) ;
	    fflush (log_fd) ;
    }
    return ((signed32) result) ;
}

/*****************************************************************************/
/*                                                                           */
/*  This function reads the texts from NLS-file into the masks.              */
/*                                                                           */
/*****************************************************************************/

 static signed32 d20_read_mask_file(void)
{
char      length[MAXLEN];
char     *p_length = length;
char     *p_len[1];
signed32  groesse;

char     *m_str;
nl_catd   nl_fd;
signed32  msg_set;
char     *d20_conf_type[MAXCONFTYP];
signed32  f;
signed32  f_max;

static D20_omask *mask1[] = { diradm_mask1,
			      diradm_mask2,
			      menupar_mask,
			      info_mask,
			      info1_mask,
			      confpar_mask,
			      conf1_mask,
			      conf1d_mask,
			      conf2_mask,
			      conf3_mask,
			      conf_mask,
			      dirsi_mask,
			      dirsiA_mask,
			      dirsiF_mask,
			      dirsi0_mask,
			      dirsi1_mask,
			      dirsi2_mask,
			      dirsi3_mask,
			      dirsi4_mask,
			      dirsi4F_mask,
			      direi_mask,
			      direiA_mask,
			      direiF_mask,
			      direi0_mask,
			      direi1_mask,
			      direi2_mask,
			      direi3_mask,
			      direi3F_mask,
			      dirsiei1F_mask,
			      error_mask,
			      (D20_omask *) 0 };

if ((nl_fd = catopen(D20_ADMIN_NLSFILE, 0)) == (nl_catd) -1)
    {
    if (m_print_err == TRUE)
    	fprintf(stderr, "Error while opening NLS file (%s)\n", D20_ADMIN_NLSFILE);
    return(D2_ERROR);
    }

if ((nl_help_fd = catopen(D20_ADMINHELP_NLSFILE, 0)) == (nl_catd) -1)
    {
    if (m_print_err == TRUE)
    	fprintf(stderr, "Error while opening NLS-helpfile (%s)\n", D20_ADMINHELP_NLSFILE);
    return(D2_ERROR);
    }

msg_set = 1;
if (d20_read_str(nl_fd, msg_set, 1, &p_length, p_len) == D2_ERROR)
{   catclose(nl_fd);
    return(D2_ERROR);
}
groesse = atoi(length);

if ((m_str = (char *)malloc((size_t)groesse)) == (char *) 0)
{
    if (m_print_err == TRUE)
	    fprintf(stderr, "Error while allocating memory for reading NLS file\n");
    catclose(nl_fd);
    return(D2_ERROR);
}

msg_set++;

for (f = 0; mask1[f] != (D20_omask *) 0; f++, msg_set++)
{   if (d20_mask_init(nl_fd, msg_set, &m_str, mask1[f]) == D2_ERROR)
    {   catclose(nl_fd);
	return(D2_ERROR);
    }
}

for (f = 0; f < NO_DIRADM1; f++) {
    /* variable abreviations for functions in diradm_mask1 */
    func_array[f] = diradm_mask1[func_pos[f]].d20_ioinfo[0];
}
for (f_max = NO_DIRADM1 + NO_DIRADM2; f < f_max; f++) {
    /* variable abreviations for functions in diradm_mask2 */
    func_array[f] = diradm_mask2[func_pos[f]].d20_ioinfo[0];
}
/* static abreviations for other functions */
func_array[f++] = D20_X_ON;
func_array[f++] = D20_X_OFF;
func_array[f] = D20_SHELL;

strcpy (si1_tmp, dirsi1_mask[2].d20_ioinfo) ;
strcpy (si3_tmp, dirsi3_mask[0].d20_ioinfo) ;
strcpy (si4_tmp, dirsi4_mask[0].d20_ioinfo) ;
strcpy (ei1_tmp, direi1_mask[2].d20_ioinfo) ;
strcpy (ei2_tmp, direi2_mask[0].d20_ioinfo) ;
strcpy (ei3_tmp, direi3_mask[0].d20_ioinfo) ;

if (d20_read_str(nl_fd, msg_set, MAXCONFTYP, &m_str, d20_conf_type) == D2_ERROR)
{   catclose(nl_fd);
    return(D2_ERROR);
}
cl_sv_cs_ld = d20_conf_type[0];
only_cl =     d20_conf_type[1];
only_no =     d20_conf_type[2];
no_yes =      d20_conf_type[3];

msg_set++;
if (d20_read_str(nl_fd, msg_set, MAXERRMSG, &m_str, err_mess) == D2_ERROR)
{   catclose(nl_fd);
    return(D2_ERROR);
}

d20_hdlmsk_errmsg[0] = err_mess[42];
d20_hdlmsk_errmsg[1] = err_mess[43];

catclose(nl_fd);
return(D2_NOERROR);
}


/*****************************************************************************/
/*                                                                           */
/*  This function checks if filename is correct.			     */
/*                                                                           */
/*****************************************************************************/

d2_ret_val is_allowed(
   char *name)
{
signed32 k;
d2_ret_val ret=D2_NOERROR;

/* check filename */
for ( k=0; *name != D2_EOS; k++,name++)
	{
	if ((isalnum((int) *name) == 0) && *name != '.' && *name != '_' &&
	     *name != '/')
		ret=D2_ERROR;
	}

/* no filename given */
if (k == 0)
	ret=D2_ERROR;

return(ret);

}

/*****************************************************************************/
/*                                                                           */
/*  Exit's programm with erase screen and reset of old terminal definitions. */
/*                                                                           */
/*****************************************************************************/

 static signed32 exit_func(
   signed32 ex_value)

{
	cerase () ;
	cgetterm () ;
#ifdef __hpux
	if ((term_type()) == 1) {
           define_keys(0, 2, NULL, NULL); /* reset to default keys */
           fputs ("\033&j@",stdout); /* reset HP terminal */
	}
#endif
	exit(ex_value);
}

 static signed32 set_lock(
   signed32 flag,
   signed32 id)
{
signed32 result=0;
int      file_nr;

switch ((int) input) {
case AK:     
	sprintf (cmdbuf, LOCKACT, dir1_path);
	flock_arg.l_whence=SEEK_SET;
	flock_arg.l_start=0L;
	flock_arg.l_len=0L;
	break;
case SI | EI:     
	sprintf (cmdbuf, LOCKDB, dir1_path);
	flock_arg.l_whence=SEEK_CUR;
	flock_arg.l_start=id;
	flock_arg.l_len=1L;
	break;
default:
	return(result);
}


/* set parametere to lock file */
if (flag == D2_LOCK) {
	if ((lock_fd = fopen(cmdbuf, "w")) == (FILE *) 0) {
		if (trace)
			{
			fprintf(log_fd,"Can't create lock_file: %s", cmdbuf);
			fflush(log_fd);
			}

		return(EXIT2);
	}
	flock_arg.l_type=F_WRLCK;
}

/* set parametere to unlock file */
if (flag == D2_UNLOCK)
	flock_arg.l_type=F_UNLCK;

/* get filenumber of file (lock_act) */
file_nr=fileno(lock_fd);
	
/* lock (unlock) file */
if (fcntl(file_nr, F_SETLK, &flock_arg) == (-1)) {
	if (flag == D2_LOCK) {
		if (trace)
			{
			fprintf(log_fd,"The selected function is executed by somebody else (errno: %d)!\n", errno);
			fflush(log_fd);
			}

		result=EXIT41;
	} else {
		if (trace)
			{
			fprintf(log_fd,"File %s can't be unlocked (errno: %d)\n", cmdbuf, errno);
			fflush(log_fd);
			}

		result=EXIT2;
	}
}

return(result);
}



/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name       : error_mess_nr()                           */
/* Description         : Return an index of an error message from  */
/*                       a message catalog set corresponding       */
/*                       to a error code                           */
/* Author              : Jean-Marc Bonnaudet, SNI BU BA NM 123     */
/*                                                                 */
/*-----------------------------------------------------------------*/
static d2_ret_val error_mess_nr( signed32 error_code,	  /* IN		*/
				 signed16 *process_index, /* OUT	*/
				 signed16 *error_index)   /* OUT	*/
{
signed16 error_id, process_id;
d2_ret_val ret_value = D2_NOERROR;

    process_id = (error_id = error_code>>8)>>5;
    error_id  -= process_id<<5;

    *process_index = process_id;

    if(*process_index != 0)
	switch( error_id )
	{
	    case D27_INV_PAR:
		*error_index = IDX_INV_PAR_ERR;
		break;
	    case D27_INV_DIR_ID:
		*error_index = IDX_INV_DIR_ID;
		break;
	    case D27_CHDIR_ERR:
		*error_index = IDX_CHDIR_ERR;
		break;
	    case D27_FORK_ERR:
		*error_index = IDX_FORK_ERR;
		break;
	    case D27_CHK_KEY_ERR:
		*error_index = IDX_CHK_KEY_ERR;
		break;
	    case D27_INV_RES:
		*error_index = IDX_INV_RES;
		break;
	    case D27_INV_PRIV:
		*error_index = IDX_INV_PRIV;
		break;
	    case D27_INV_NORM:
		*error_index = IDX_INV_NORM;
		break;
	    case D27_INV_CPATH:
		*error_index = IDX_INV_CPATH;
		break;
	    case D27_INV_SWITCH:
		*error_index = IDX_INV_SWITCH;
		break;
	    case D27_INV_VALUE:
		*error_index = IDX_INV_VALUE;
		break;
	    case D27_INV_MXIASSOC:
		*error_index = IDX_INV_MXIASSOC;
		break;
	    case D27_INV_MXRASSOC:
		*error_index = IDX_INV_MXRASSOC;
		break;
	    case D27_NO_AUTH_MECH:
		*error_index = IDX_NO_AUTH_MECH;
		break;
	    case D27_INV_AUTH_MECH:
		*error_index = IDX_INV_AUTH_MECH;
		break;
	    case D27_INV_IVTIME:
		*error_index = IDX_INV_IVTIME;
		break;
	    default:
		*error_index = EXIT2>>8;
		ret_value = D2_ERROR;
		break;
	}
    else
    {
	*error_index = EXIT2>>8;
	ret_value = D2_ERROR;
    }

    return( ret_value );
}



/* End of d20admin.c */
