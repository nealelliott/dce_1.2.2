/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 *(c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
*/
/*
 * HISTORY
 * $Log: mgmt_main.c,v $
 * Revision 1.1.12.2  1996/02/18  19:29:46  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:11:26  marty]
 *
 * Revision 1.1.12.1  1995/12/08  15:10:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:40:45  root]
 * 
 * Revision 1.1.9.8  1994/08/03  19:01:09  mccann
 * 	fix assert
 * 	[1994/07/21  20:02:46  mccann]
 * 
 * 	include file cleanup
 * 	[1994/07/19  17:46:29  mccann]
 * 
 * Revision 1.1.9.7  1994/07/08  15:32:22  peckham
 * 	cr10935 - Ugly error message for non-root 'cdscp dump clerk cache'
 * 	Cleanup from above - move dthread_init() to main().
 * 	[1994/06/29  19:35:34  peckham]
 * 
 * Revision 1.1.9.6  1994/06/23  18:29:48  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  19:33:52  mccann]
 * 
 * Revision 1.1.9.5  1994/06/09  18:37:27  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:09:23  devsrc]
 * 
 * Revision 1.1.9.4  1994/06/08  20:17:53  peckham
 * 	Initialize serviceability and message catalogs.
 * 	[1994/05/31  18:56:17  peckham]
 * 
 * Revision 1.1.9.3  1994/04/29  15:49:50  peckham
 * 	Use dcelocal_path in filenames.
 * 	[1994/04/29  13:56:30  peckham]
 * 
 * Revision 1.1.9.2  1994/04/04  20:06:12  peckham
 * 	Fixes for sams file cleanup
 * 	[1994/04/04  18:08:04  peckham]
 * 
 * Revision 1.1.9.1  1994/03/12  22:00:13  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:04:37  peckham]
 * 
 * Revision 1.1.6.6  1992/12/30  13:07:32  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:39:06  zeliff]
 * 
 * Revision 1.1.6.5  1992/12/15  15:54:41  tom
 * 	Bug 6409 - add setlocale call for i18n.
 * 	[1992/12/15  15:17:02  tom]
 * 
 * Revision 1.1.6.4  1992/11/17  17:21:19  m_sawyer
 * 	Removed all initialization related to [now defunct] command line recall.
 * 	Also removed a bunch of "#ifdef vms" conditionals.
 * 	[1992/11/17  15:13:40  m_sawyer]
 * 
 * Revision 1.1.6.3  1992/10/15  20:07:52  m_sawyer
 * 	#ifdef out the sleep(5) during initialization
 * 	[1992/10/15  19:59:39  m_sawyer]
 * 
 * Revision 1.1.6.2  1992/09/29  19:11:15  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:11:30  weisman]
 * 
 * Revision 1.1.2.4  1992/05/06  16:35:21  m_sawyer
 * 	made 'dropdead()' macro a procedure and added code to restore terminal
 * 	characteristics when aborting
 * 	[1992/05/06  15:57:26  m_sawyer]
 * 
 * Revision 1.1.2.3  1992/04/29  15:32:32  m_sawyer
 * 	removed unnecessary and arbitrary check for 'lotsmem' at startup
 * 	[1992/04/29  15:31:23  m_sawyer]
 * 
 * Revision 1.1.2.2  1992/03/22  21:51:13  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:22:30  weisman]
 * 
 * Revision 1.1  1992/01/19  15:23:00  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990-1994. ALL RIGHTS RESERVED.
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
 *
 * DESCRIPTION OF MODULE:
 *
 *  This is the top level module for the CDSCP.  
 */

#include <cdscp.h>
#include <dce/dcesvcmsg.h>
#include <dce/assert.h>
#include <signal.h>
#include <tower.h>
#include <locale.h>
#include <dce/cds_clerkserver.h>
#include <threads_lib.h>

#ifdef DCE_SEC
#include <dce/sec_login.h>
#include <dce/rpc.h>
sec_login_handle_t login_context_h;
#endif

#ifdef SNI_SVR4_POSIX
#include <termios.h>
#endif

#include <prsdef.h>
#include <cmdblk.h>
#include <logging.h>
#include <err_list_defs.h>
#include <dns_strings.h>
#include <mgmt_com_act_decl.h>		/* parse action routines */

#define OVERALL_SUCCESS 0
#define OVERALL_FAILURE 1

int debug = TRUE;


#define NULLTIME {0, 0}

/* control blocks that must be accessable, and their prerequisites */
static struct envblk    eb;		/* ncl environment */
static struct op_blk opbhdr;		/* the head of the output list */
static struct entblk entb;		/* head of the entity list */
static struct attrblk attrb;		/* head of the attribute list */
static struct err_blk errhdr;           /* head of the error list */
static struct logblk    lb;		/* command loging info */
#ifdef DCE_SEC
    static void do_login(void);
#endif

/*
 * main - the main procedure
 */
int
main (
    int		argc,
    char	*argv[])
{
    static char ncl_pprompt[] = "cdscp> ";  /* primary prompt for cds */
    static char ncl_sprompt[] = "_> ";	    /* secondary prompt */
    static char cmd_buf[(MAX_NCL_CMD)+2];   /* bld cmd from cmd lines here */
    static char cl_buf[(MAX_CL_LEN)+2];	    /* read command lines into here */
    static struct prs	   cpb;		/* command parse block */
    static struct cmdblk    cb;		/* level 0 command block */
    static struct op_blk preslb;	/* the present logging output block */
    static struct op_blk nextlb;	/* the next logging output block */
    char message[256];
    char filename[_DNS_MAXPATHLEN+1];
    char		*myname_p;
    error_status_t	st;
    int			minbuf;		/* size of buffer to hold command */
    int			skipargs;	/* number of args to skip */
    int			parse_status;	/* returned by parser */
    unsigned char	*where_p;	/* for squeeze in place */
    int			ignore_1;	/* accum cmd tells us. we ignore it */
    struct logparmblk	logparm;	/* parameters for logging callback */
    int			overall_status;	/* did command work ok ? */

    /* Get our filename, stripped of directories */
    if (myname_p = strrchr(argv[0], '/'))
	++myname_p;
    else
	myname_p = argv[0];

    setlocale(LC_ALL, "");

    (void)sprintf(filename, "%s(%ld)", myname_p, (long)getpid());
    dce_svc_set_progname(filename, &st);
    dce_assert(cds__svc_handle, (st == svc_s_ok));

    dce_msg_define_msg_table(cds_msg_table,
                             sizeof(cds_msg_table) / sizeof(cds_msg_table[0]),
                             &st);
    dce_assert(cds__svc_handle, (st == msg_s_ok));

    /* Initialize dthreads */
    dthread_init();

    /*
     * call the cds initialization routine in order to initiate the
     * creation of a clerk and therefore to speedup the first clerk
     * call
     */
    cdsInit();

    /* message file not loaded yet, so can not use it. */
    /* output routines not set up. */
    /* any problem reports have msgs embedded in the code. */
    /* that makes life rough on the translators, so minimize the messages. */
    /* we have no way to complain if anything goes wrong, so be sure all ok. */
    /* call nothing that can fail, for now. */

    spaces_init();			/* allow spaces routine to be used */

    /* initialize the various control blocks */
    /* these could be done by initializers on the declaration. */
    /* however, that requires knowing the order of the fields. */
    /* an added field in the middle of a block leads to subtle errors. */
    /* many of the fields do not have to initialized. */

    cds_ifspec = cds_clerkserver_v1_0_c_ifspec;

    /* initialize the environment block */
    eb.kind_of_structure_code = ENVIRONMENT_BLOCK;
    eb.version = 1;
    eb.defent = 0;			/* no default entity yet */
    eb.defacc = 0;			/* no default access yet */
    eb.defnsn = 0;			/* no default namespace yet */
    eb.log    = &lb;			/* but no logging yet */
    eb.initcmd= &cb;			/* initial command block */
    eb.curcmd = &cb;			/* current command block */
    eb.curconf= (int)dnsConfMed;	/* low confidence is the default */
    eb.curtout= 0;			/* default value until later */
    eb.uidopt = 0;			/* per NCL, uid display defaults off */
    eb.ts_opt = 1;			/* display timestamps as times */
    eb.nick_opt=3;			/* display nicknames as names */

    /* initialize the command parser context block */
    prsblk_init(&cpb, cmd_buf, sizeof(cmd_buf));

    cmdblk_init(&cb, 0);		/* initialize the command block */
    cb.envptr	= &eb;			/* link it to the environment block */
    cb.rclblk	= NULL;			/* command recall facility */
    cb.prsblk	= &cpb;			/* use parse block just established */
    cb.cmdbuf	= cmd_buf;		/* use this command buffer */
    cb.cmdlen   = sizeof(cmd_buf);	/* to limit accum_cmd */
    cb.cipbuf	= cl_buf;		/* use this command line buffer */
    cb.ciplen	= sizeof(cl_buf);	/* to limit the read */
    cb.ipfile	= 0;			/* no input file now */
    cb.pprompt  = ncl_pprompt;		/* replace the default prompt */
    cb.sprompt  = ncl_sprompt;		/* replace the default prompt */

#ifdef DCE_SEC
    do_login();   /* set up a secure login */
#else
/* Warn the non-secure user that he is unauthenticated */

    dce_fprintf(stderr, CDSCP_NO_LOGINCONTEXT);

#endif  /* DCE_SEC */

    cb.ipfile = stdin;		/* save FILE * for fgets() */
    cb.mode = isatty(fileno(stdin)) ? INTERACTIVE : BATCH;

    /* initialize the logging parameters block */
    logparm.lp  = &lb;			/* know where to find logging block */
    logparm.cbp = &cb;			/* know where to find command block */

    /* initialize the output blocks for logging. present and next. */
    init_op_list_element(&preslb);	/* fill in most of the fields */
    init_op_list_element(&nextlb);	/* fill in most of the fields */
    preslb.callback = log_output;	/* select the callback routine */
    preslb.callparm = &logparm;		/* know how to get more logging info */

    /* initialize the command logging block */
    init_logblk(&lb);			/* fill in most of the fields */
    lb.pres = &preslb;			/* point to logging file info */
    lb.next = &nextlb;			/* point to where new info goes */
    lb.pres->spec_alloc = 0;		/* spec is not in malloced space */
    lb.envptr	= &eb;			/* link up to environment block */

    /* initialize the head of the attribute list */
    attrblk_init(&attrb);
    attrb.blkalloc = 0;			/* the block is in static storage */

    /* this might be a good place to check for adequate stack space */
    /* on vms it can be done but is not needed. */
    /* the desired amount of stack space can be reserved at link time. */
    /* if that is not enough, more is automatically allocated. */
    /* on ultrix the stack expands automatically. */
    /* on ms-dos, the stack may be limited to 64kb, for any memory model. */
    /* i do not know how to do it on ms-dos. is it possible? is it needed ? */

    /* set up the various control blocks for starting up. */
    /* be careful to insure there are no calls that can fail. */
    /* all messages belong in the message file, so they can be translated. */
    /* messages in the code might not get translated. */
    /* have only the minimum number of messages in the code. */
    /* we need access to the message file, and a path to the user. */

    op_default_init();			/* format, use terminal, etc */
    op_list_hdr_init(&opbhdr);		/* establish head of output list */
    opbhdr.memcode = 0;			/* not allocated dynamically */
    use_default_op_list(&opbhdr);	/* enable output in case of gotchas */

    err_list_hdr_init (&errhdr);
	
    /* this is a major milestone.  now we can issue messages. */

    parse_init(&eb, &cb, &cpb, &opbhdr);	/* help parser find things */
    ent_one_onetime(&entb, 0);		/* init the head of the entity list */
					/* this never holds a real entity */

    ent_onetime();			/* init the prototype entities */

    /*									    
    ** Now load the parse table for commands. 
    ** there may eventually be several parse tables.
    */
    (void)sprintf(filename, CDS_CP_PARSE_FILE, dcelocal_path);

    /* Exits on error in parse file */
    load_parse_file(&cpb, filename, 0);
    skipargs =
	process_args(argc,argv);	/* find all command line options */
    if (debug_requested())
	{
	  debug = TRUE;
	  
	}
    
    withhead_init(withlist_pointer());	/* start the with list */
    init_default_entity();		/* prepare to have a default ent */
    reset_default_entity(		/* establish initial default ent */
			 &eb,		/* ptr to environment block */
			 &cb,		/* ptr to command block, level 0 */
			 &opbhdr);	/* ptr to head of output list */
    
    use_startup_file();		/* customize options as user likes */

    /*								    
     ** If there is any command on the invoking command line, execute it. 
     ** Anything after the flags and switches might be a command or it
     ** might just be white space or comments.		    
     */ 
    minbuf = argv_size(argc, argv, skipargs);
    if (minbuf > sizeof(cmd_buf))
	{
	  /* temporary error report */
	  cds_printf(CDSCP_CMD_TOO_BIG, "command too big\n");
	  
	  emit_line_fmt(&opbhdr, CDSCP_CMD_TOOBIG,
		     "Command requires %d byte buffer, parser has %d bytes\n",
			minbuf, sizeof(cmd_buf));

	  cb.func = FUNC_EXIT;	/* anything except null */
	}
    else
	{				/* command will fit in buffer */
	  overall_status = DNS_ERROR;	/* so many ways to fail */
	  reassemble_command(argc,	/* make complete command */
			     argv,
			     skipargs,
			     cmd_buf);
	  where_p = (unsigned char *)cmd_buf;	/* squeeze it in place */
	  ignore_1 = 
	      accum_cmd(cmd_buf, (char **)&where_p);	/* remove comments */
	  log_command(		/* save a copy of the command */
		      &lb,		/* ptr to logging control block */
		      &cb,		/* ptr to command lock */
		      &opbhdr);	/* ptr to output list */
	  delete_all_op_list_elements(&opbhdr);   /* parse replaces list */
	  make_withlist_empty(withlist_pointer());/* parse replaces list */
	  parse_status =
	      parse_command(&cb);	/* figure out what it means */
	  form_with_list();		/* set up filters */
	  if (parse_status == RET_SUCCESS)	/* is it all fine and dandy? */
	      {			/* yes.  no problems. */
		allow_default_entity(&entb);
		allow_default_op_list(&opbhdr);
		open_all_files(&opbhdr);
		exec_cmd(		/* so execute the command */
			 &cb,		/* ptr to command block */
			 &opbhdr,	/* ptr to head of output list */
			 &lb,		/* ptr to logging block */
			 &overall_status);	/* where to leave status */
                close_all_files(&opbhdr);
	      }
	  else if (parse_status == RET_EOF)	/* end of file signal ? */
	      {			/* yes. ignore whaever else is there */
		cb.func = FUNC_EXIT;	/* anything but FUNC_NULL */
	      }			/* the program will terminate */
	  else
	      {			/* some kind of parse error */
		use_default_op_list(&opbhdr);
		report_parse_error(&cb, parse_status);
		cb.func = FUNC_EXIT;	/* anything but FUNC_NULL */
	      }			/* the program will terminate */
	}
    
    /* if there was a command, we are done. */
    /* if no command, we ask for and execute commands. */
    if (cb.func == NULLCMD)
	{
	  /* get and execute commands */
	  overall_status =
	      process_commands(&cb, &opbhdr, &entb, &attrb, &lb);
	}

    if (overall_status == DNS_SUCCESS)
	{				/* tell operating system success */
	exit(OVERALL_SUCCESS);
	}
    else
        {				/* tell operating system we failed */
	exit(OVERALL_FAILURE);
        }
}


/* tiny routines to avoid global variables */

struct op_blk *
get_op_blk_ptr (void)			/* return a pointer to head of */
    { return (&opbhdr); }		/* the list of output blocks */

struct err_blk *
get_err_blk_ptr (void)	      /* return ptr to head of error report list */
    { return (&errhdr); }

struct envblk *
get_envblk_ptr (void)			/* return ptr to */
    { return (&eb); }			/* environment block */

struct entblk *
entitylist_pointer (void)		/* return pointer to */
    { return (&entb); }			/* entity block list head */

struct attrblk *			/* return pointer to */
get_attrblk_ptr(void)			/* head of attribute blocks list */
    { return (&attrb); }

#ifdef DCE_SEC

int Authentication = 0;

/* Aquire principal's name and password and do a secure login.
 * Principal's name obtained from environ variable 'PRINCIPAL'.
 * Principal's password obtained from environ variable 'PASSWORD'.
 * If no principal name supplied, do unauthenticated requests.
*/

static void
do_login (void)
{
  error_status_t sec_status;

  /* Get current context and save it */
  Authentication = 0;
  sec_login_get_current_context(&login_context_h, &sec_status);
  if (sec_status == error_status_ok)
    Authentication = 1;
  else {
    if (sec_status == sec_login_s_no_current_context) {
      dce_fprintf(stderr, CDSCP_NO_LOGINCONTEXT);
    }
    else {
      unsigned char *msg_p;
      unsigned32 st;

      msg_p = dce_msg_get_msg(sec_status, &st);
      fprintf(stderr, (char *)msg_p);
      free(msg_p);
    }
    Authentication = 0;
 }
}
#endif  /* DCE_SEC */
