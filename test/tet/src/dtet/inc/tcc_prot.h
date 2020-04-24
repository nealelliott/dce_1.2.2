/*
 * Copyright 1990 Open Software Foundation (OSF)
 * Copyright 1990 Unix International (UI)
 * Copyright 1990 X/Open Company Limited (X/Open)
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of OSF, UI or X/Open not be used in 
 * advertising or publicity pertaining to distribution of the software 
 * without specific, written prior permission.  OSF, UI and X/Open make 
 * no representations about the suitability of this software for any purpose.  
 * It is provided "as is" without express or implied warranty.
 *
 * OSF, UI and X/Open DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, 
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO 
 * EVENT SHALL OSF, UI or X/Open BE LIABLE FOR ANY SPECIAL, INDIRECT OR 
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF 
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR 
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR 
 * PERFORMANCE OF THIS SOFTWARE.
 *
 */

/************************************************************************

SCCS:          @(#)tcc_prot.h    1.9 1/25/93 DTET release 1.0b
NAME:          tcc_prot.h
PRODUCT:       DTET (Distributed Test Environment Toolkit)
AUTHOR:        J B Goode, OSF
DATE CREATED:  14 May 1991
CONTENTS:      Prototypes and extern variables for TCC

MODIFICATIONS:

               "TET Rework"
               David G. Sawyer, UniSoft Ltd,  July 1991.

	       Changed declaration for obtain_tool_lock() to include char *
               David G. Sawyer, UniSoft Ltd,  31 Oct 1991.

	       Changed tc_search to resume_status
               David G. Sawyer, UniSoft Ltd,  13 Feb 1992.

	       DTET development - this file is based on Release 1.10 of the TET
	       David G. Sawyer and John-Paul Leyland, Unisoft Ltd, June 1992

	       Andrew Dingwall, UniSoft Ltd., October 1992
	       changed tet_error() to tcc_error() so as to avoid a
	       name clash with the API function of the same name

************************************************************************/

/* The TET environment structure */
typedef struct env_t
{
    char *name, *value;
    bool perm;
} ENV_T;


/* The Results Code structure */
typedef struct rescode {
    int num;
    char *name;
    char action[RESCODE_AC_LEN];
} RESCODE;


/* Test Purpose Result Structure */
typedef struct tprs  
{
    int num;     /* The message sequence number; ++ed for each entry */
    int context; /* The process that generated the entry */
    int block;   /* The message block number; ++ed by tp request */
    int type;    /* The type of message in the file */
    int tpnum;   /* This num uniquely identifies the tp within a tc */
    char *data;  /* The line of info in the file */

    struct tprs *next,*prev;
} TPRS;


/* structure used in tracing of malloc()/realloc() and free() */
typedef struct memcheck
{
	long	ptr;
	int	vol;
	char	file[50];
	int	line;
} MEMCHECK;


/* Testcase Storage structure */
struct tc_store
{
    char *tc_name;			/* the TC string */
    int line_number;			/* scenario reference line number */
    int element_number;			/* scenario reference element number */
    struct sysid_store *sysid_store;	/* Pointer to remote des struct */
    int num_sysid;			/* Number of remote designations */
    pid_t process_ID;			/* Process ID of TC if in a group */
    char *tmp_jnl_name;			/* Tmp journal for TC's in a group */
    int duplicate;			/* TC name duplicated in a group */
    int ret_val;			/* Return value from last b/e/c */
    int override_eip;			/* override EXEC_IN_PLACE variable ? */
    int ignore;				/* don't process this TC (-y/-n opts) */
};


/* Remote designation structure */
struct sysid_store
{
    int sysid;				/* remote system number */
    int logged_on;			/* are we logged onto remote sys */
    int duplicate;			/* TC name duplicated in a group */
    int override_eip;			/* override EXEC_IN_PLACE variable ? */
};


/* Remote enviroment structure */
struct rem_env
{
    int sysid;
    ENV_T *bld_env;
    ENV_T *exe_env;
    ENV_T *cln_env;
};


/***************************************************************************** 
 *									     *
 *    external definitions						     *
 * 									     *
 *****************************************************************************/

/* tcc.c */

extern    int       debug_flag;
extern    int       cpid;
extern    int       jnl_fd;
extern    int       lock_type;
extern    int       interrupted;
extern    int       eip_set;
extern    char      error_mesg[];
extern    long      snid;

extern    int       oc_set;
extern    int       scenario_line;
extern    int       old_line;
extern    int       scenario_element;
extern    int       old_element;
extern	  int 	    group_line;
extern	  int 	    group_element;

extern    char      *result_pat;    
extern    char      *bld_cfg_file_name;
extern    char      *exe_cfg_file_name;
extern    char      *cln_cfg_file_name;
extern    char      *dis_cfg_file_name;
extern    char      *scen_file_name;
extern    char      *suite_name;
extern    char      *scenario_name;
extern    char      **yes_list;
extern    char      **no_list;
extern    char      **save_files;

extern    MEMCHECK  *memtable;

extern    ENV_T     *bld_env;
extern    ENV_T     *exe_env;
extern    ENV_T     *cln_env;
extern    ENV_T     *dis_env;
extern    ENV_T     *com_env;

extern    FILE      *old_jnl_fp;

extern    char      results_dir[];
extern    char      start_dir[];

extern    struct tc_store *remote_tc;
extern    int       num_remote_tc;
extern    struct tc_store *parallel_tc;
extern    int       num_parallel_tc;

/* XXX Used all over the place XXX */
extern    char      line[];


/* XXX Still to be checked XXX */
extern    int       exec_ctr;
extern    int       g_max_rescode;
extern    int       tmode;
extern    int       exec_all_flag;
extern    int       g_ic_num;
extern    int       total_sysid; /* Number of remote systems in this sceanrio.*/
extern    struct    sysid_store *all_sysids;   /* All system IDs in scenario. */
extern    sigjmp_buf   jmpbuf;
extern    int       gnumsys;
extern    int *     gsyslist;

/*****************************************************************************/

#if __STDC__

/**********   Function Prototypes For ANSI C   **********/

/* again.c */
extern    int       do_again(int, int *, int *);
extern    void      rerun_flush(int, char *, int *);
extern    int       compare_ics(const void  *, const void *);
extern    void      make_ic_list(char *, int **);


/* bld_cln.c */
extern    int       bld_cln_tc(char *, int, int, int, int, int *);
extern    int       do_bld_cln_tc(char *, int, int, int *);


/* config.c */
extern    void      init_config(ENV_T **); 
extern    void      do_config_file(char *, int, ENV_T **);
extern    void      dump_config(char *, int, ENV_T *);
extern    void      default_cfg_file(char **, char *, char *, char *, char *);
extern    void      init_env(ENV_T **);


/* dconfig.c */
extern    void      config_systems(int, struct sysid_store *);
extern    char      **form_config(ENV_T *, int *);
extern    void      free_config(char **,int *);
extern    void      make_rem_tetenv(int, int, char **, int);
extern    ENV_T     **add_remenv_ptr(int, int);
extern    ENV_T     **get_remenv_ptr(int, int);
extern    ENV_T     **get_remenv_mode(int, int);
extern    void      send_sysids(int, struct sysid_store *);
extern	  void      send_config(int, char *, char **, int);
extern    char **   rec_config(int, int*);
extern    char **   store_config(char **, int *, char *);


/* dist.c */
extern    int       dtcc(int, struct sysid_store *, int, char **);
extern    int       start_dtcc(int, char **);
extern    int       start_syncd();
extern    int       start_xresd();
extern    int       start_tccd(int, struct sysid_store *);
extern    void      end_tccd(int, struct sysid_store *);
extern    void      end_dtcc(int, struct sysid_store *);
extern    int       make_rem_results(int, struct sysid_store *);
extern    char *    rem_results_dir(char *, int, int);
extern    int       make_rem_tmpdir(int, struct sysid_store *);
extern    int       valildate_rem_paths(int, struct sysid_store *);


/* error.c */
extern    void      tet_shutdown();
extern    void      exec_sig_hdlr(int);
extern    void      tool_sig_hdlr(int);
extern    void      siginthdlr(int);
extern    void      sighdlr(int);
extern    void      tet_cleanup();
extern    void      tcc_error(char *, int, char *, int);
extern    void      dtccerror(int, char *, int, char *, char *);
extern    void      dtccfatal(int, char *, int, char *, char *);


/* exec.c */
extern    int       exec_tc(char *, int, int, int, int *, int);
extern    int       do_exec_tc(char *, char *, int, int, int *, char *);


/* generic.c */
extern    void      tet_srcpath(char *, char *, int, int);
extern    void      tet_exepath(char *, char *, int, int);
extern    int       tet_chdir(char *, int, int);
extern    int       tet_mkdir(char *, int, int);
extern    int       tet_unlink(char *, int, int);
extern    int       tet_rmdir(char *, int, int);
extern    int       tet_putenv(char *, int, int);
extern    int       tet_open(char *, int, int);
extern    int       tet_close(int, int, int);
extern    int       tet_puts(int, char *, int, int);
extern    int       tet_excllock(char *, int, int, int);
extern    char *    tet_sharedlock(char *, int, int, int);
extern    void      tet_savefiles(char *, char **, int, int, int);
extern    char *    tet_mktmpdir(char *, int, int);
extern    int       tet_access(char *, int, int, int);
extern    int       server_dead();


/* group.c */
extern    void      bec_group(struct tc_store *, int, int *, int);
extern    void      proc_tc_grp(struct tc_store *, int, int, int *, int);
extern    void      signal_group(struct tc_store *, int, int);
extern    void      signal_parent(int *);
extern    pid_t     group_fork(struct tc_store *);
extern    void      child_wait();
extern    void      parent_wait(struct tc_store *, int *);
extern    pid_t     dofork();
extern    void      wait_on_processes(struct tc_store *, int);
extern    void      set_group_jnl_fd();
extern    int       create_tmp_jnl(struct tc_store *, int);
extern    int       merge_group_jnls(struct tc_store *, int);
extern    void      signal_handler();
extern    void      prev_tc_name(struct tc_store *, int);
extern    void      write_directive(struct tc_store *);


/* journal.c */
extern    char      *get_jnl_dir();
extern    void      jnl_tool_end(int, int, char *, int);
extern    void      jnl_tool_start(int, char *, char *, int, int);
extern    void      jnl_config_end();
extern    void      jnl_config_start(char *, int);
extern    void      jnl_config_value(char *);
extern    void      jnl_rem_config(int, int, char **, int);
extern    void      jnl_entry(char *);
extern    void      jnl_entry_captured(int, char *);
extern    void      jnl_entry_invoke_tc(int, char *, char *, int);
extern    void      jnl_entry_scen(char *);
extern    void      jnl_entry_tcc_end(char *);
extern    void      jnl_entry_tcc_start(char *);
extern    void      jnl_tc_end(int, int, char *);
extern    void      jnl_tc_message(char *);
extern    void      jnl_tp_result(int, int, int, char *);
extern    void      jnl_user_abort(char *);
extern    void      jnl_para_start();
extern    void      jnl_para_end();
extern    void      jnl_repeat_start();
extern    void      jnl_repeat_end();
extern    void      jnl_remote_start();
extern    void      jnl_remote_end();
extern    void      truncate_line(FILE *, char *, int);
extern    int       tpstart(char *);
extern    int       tpend(char *);
extern    int       icstart(char *);
extern    int       icend(char *);
extern    int       modestart(char *);
extern    int       modeend(char *);
extern    int       directivestart(char *);
extern    int       directiveend(char *);


/* lock.c */
extern    int       obtain_lock(int, char *, int, int, int);
extern    int       obtain_alt_lock(char *, int, int);
extern    char *    lockfile(char *, int, int);
extern    char *    place_lock(char *, int);
extern    void      release_locks();
extern    int       lock(char *, int, int, int, int);
extern    int       unlock(char *, char *, int, int, int);


/* main.c */
/*        int       main(int, argv); */


/* misc.c */
extern    int       setmode(int, int);
extern    int       ifmode(int);
extern    int       set_gtimeout(int, int);
extern    int       gtimeout();
extern    int       mode_message(int);
extern    int       setup_complete(int);
extern    int       abort_requested();
extern    int       set_abort_req(int);
extern    int       xresd_syncd_init(struct sysid_store *, int);


/* modes.c */
extern    int       export(ENV_T *, int, int, int); 
extern    int       check_line(char *);
extern    void      add_tetenv(char *, char *, int, ENV_T **);
extern    char *    get_tetenv(char *, ENV_T *);
extern    ENV_T *   get_env_ptr(int, int, int);
extern    void      create_oc_file();
extern    void      tool_oc_jnl(int, int);
extern    void      exec_oc_jnl(int, int);
extern    char *    get_output_file(char *, int, int);
extern    void      clear_output_file(char *, int, int);
extern    void      write_env_file(ENV_T *, char *, char *, int, int);
extern    void      write_res_file(char *, char *, int, int);


/* remote.c */
extern    int       store_sysid(struct sysid_store **, int, int);
extern    int       str_to_sysids(struct sysid_store **, char *, int);
extern    int	    sysid_isstored(struct sysid_store *, int, int);
extern    void      clear_sysid_store(struct sysid_store **, int);


/* rescode.c */
extern    int       get_rescode_file();
extern    int       do_rescode_file(FILE *);
extern    char *    rescode_num_to_action(int);
extern    int       rescode_name_to_num(char *);
extern    int       rescode_num_to_index(int);
extern    char *    rescode_num_to_name(int);


/* resfile.c */
extern    int       copy_results_file(FILE *, int);
extern    int       make_tprs_list(FILE *, char *);
extern    void      parse_tprs_list();
extern    void      pr_list_to_jnl(TPRS *);
extern    int       analyze_jnl_entry(char *, int *, int *, int *, int *);
extern    TPRS *    collate_and_sort(TPRS *, int);
extern    int       compare_seq_num(const void *, const void *);
extern    void      pr_array_to_jnl(TPRS **, int);

/* save.c */
extern    char **   get_savefiles(int, int);
extern    void      clear_savefiles(char **);
extern    void      savefiles(char *, int, int);
extern    char *    make_savefiles_dir(char *, int, int, int);
extern    void      remove_savefiles_dir(char *, int, int, int);
extern    void      do_save( char *, char **);

/* scenario.c */
extern    int       perform_scen(int);
extern    int       analyse_line(char *, char **);
extern    char *    process_line(char *, int, int);
extern    void      inclusion_file(char *, char **, int);
extern    int       make_alt_scenario(char *, char **);


/* spawn.c */
extern    char *    do_copy(char *, char *, int, int, int);


/* store.c */
extern    int       store_tc(struct tc_store **,char *,struct sysid_store *, int, int, int, int);
extern    int	    isstored(struct tc_store **, int, int, int);
extern    void      clear_store(struct tc_store **, int);


/* syntax.c */
extern    int       check_scenario(char *, char *);
extern    int       scan_scen(char *, char *, int, int, int, int);
extern    int       match(char *, char *, char *, FILE *, char *, char **, int *, int, int, int, int);
extern    int       find_scen(FILE *, char *, char **,char *);
extern    void      go_to_line(FILE *,char *, int);
extern    void      parse_param(int, char *, char **);
extern    void      syntax_error(char *, char *, char *, int, int);
extern    int       get_scenario_item(FILE *, char *, char **, int *);
extern    int       find_item(char **);


/* tcc.c */
extern    int 	    tcc(int, char **);
extern    void      show_synopsis();


/* testcase.c */
extern    void      bec_tc(char *, struct sysid_store *, int);
extern    int       proc_tc_dir(char *, struct sysid_store *, int, int, int, int *, int);
extern    void      make_gsyslist(struct sysid_store *, int);
extern    void      clean_gsyslist();
extern    int       rem_grp(char *, struct sysid_store *, int, int, int, int *);
extern    int       gsys_wrapper(char *, struct sysid_store *, int, int, int, int *, int);
extern    int       rem_tc(char *, struct sysid_store *, int, int, int *, int);
extern    int       run_tc(char *, int, int, int, int, int *, int);
extern    int       logonoff_sys(int, int);


/* util.c */
extern    int       do_mkdir(char *);
extern    char *    number_suffix( int);
extern    char *    strinstr(char *, char *);
extern    void      get_time();
extern    char *    basename(char *);
extern    int       optget(int, char **, char *);
extern    void      tet_free(void *, char *, int);
extern    void *    tet_malloc(size_t, char *, int, int);
extern    void *    tet_realloc(void *, size_t, char *, int, int);
extern    void      memory_table(int);
extern    void      mem_setup();
extern    char *    get_results_dir(int, int);
extern    int       put_tcenv(char *);
extern    char *    get_ts_root(int, int);
extern    char *    get_tet_root(int, int);
extern    char *    get_alt_exec_dir(int, int);
extern    char *    get_temp_dir(int, int);
extern    void      cat_path(char *, char *);
extern    void      comb_paths(char *, char *, char *);
extern    char *    store_paths(char *, char *);
extern    int       make_path(char *, char *, int, int);
extern    int       make_unique_dir(char *, char *, int, int, int);
extern    int       remove_path(char *, char *, int, int);
extern    int       validate_pathnames(int, int);
extern    int       check_access(int, int);
extern    char *    store_str(char *);
extern    char *    combine_str(char *, char *);
extern    int       remove_dir(char *, int, int);


/**********   End of ANSI C Prototypes   ***********/

#else

/**********   Function Declarations For Non-ANSI C   **********/


/* again.c */
extern    int       do_again();
extern    void      rerun_flush();
extern    int       compare_ics();
extern    void      make_ic_list();


/* bld_cln.c */
extern    int       bld_cln_tc();
extern    int       do_bld_cln_tc();


/* config.c */
extern    void      init_config(); 
extern    void      do_config_file();
extern    void      dump_config();
extern    void      default_cfg_file();
extern    void      init_env();


/* dconfig.c */
extern    void      config_systems();
extern    char      **form_config();
extern    void      free_config();
extern    void      make_rem_tetenv();
extern    ENV_T     **add_remenv_ptr();
extern    ENV_T     **get_remenv_ptr();
extern    ENV_T     **get_remenv_mode();
extern    void      send_sysids();
extern	  void      send_config();
extern    char **   rec_config();
extern    char **   store_config();


/* dist.c */
extern    int       dtcc();
extern    int       start_dtcc();
extern    int       start_syncd();
extern    int       start_xresd();
extern    int       start_tccd();
extern    void      end_tccd();
extern    void      end_dtcc();
extern    int       make_rem_results();
extern    char *    rem_results_dir();
extern    int       make_rem_tmpdir();
extern    int       valildate_rem_paths();


/* error.c */
extern    void      tet_shutdown();
extern    void      exec_sig_hdlr();
extern    void      tool_sig_hdlr();
extern    void      siginthdlr();
extern    void      sighdlr();
extern    void      tet_cleanup();
extern    void      tcc_error();
extern    void      dtccerror();
extern    void      dtccfatal();


/* exec.c */
extern    int       exec_tc();
extern    int       do_exec_tc();


/* generic.c */
extern    void      tet_srcpath();
extern    void      tet_exepath();
extern    int       tet_chdir();
extern    int       tet_mkdir();
extern    int       tet_unlink();
extern    int       tet_rmdir();
extern    int       tet_putenv();
extern    int       tet_open();
extern    int       tet_close();
extern    int       tet_puts();
extern    int       tet_excllock();
extern    char *    tet_sharedlock();
extern    void      tet_savefiles();
extern    char *    tet_mktmpdir();
extern    int       tet_access();
extern    int       server_dead();


/* group.c */
extern    void      bec_group();
extern    void      proc_tc_grp();
extern    void      signal_group();
extern    void      signal_parent();
extern    pid_t     group_fork();
extern    void      child_wait();
extern    void      parent_wait();
extern    pid_t     dofork();
extern    void      wait_on_processes();
extern    void      set_group_jnl_fd();
extern    int       create_tmp_jnl();
extern    int       merge_group_jnls();
extern    void      signal_handler();
extern    void      prev_tc_name();
extern    void      write_directive();


/* journal.c */
extern    char      *get_jnl_dir();
extern    void      jnl_tool_end();
extern    void      jnl_tool_start();
extern    void      jnl_config_end();
extern    void      jnl_config_start();
extern    void      jnl_config_value();
extern    void      jnl_rem_config();
extern    void      jnl_entry();
extern    void      jnl_entry_captured();
extern    void      jnl_entry_invoke_tc();
extern    void      jnl_entry_scen();
extern    void      jnl_entry_tcc_end();
extern    void      jnl_entry_tcc_start();
extern    void      jnl_tc_end();
extern    void      jnl_tc_message();
extern    void      jnl_tp_result();
extern    void      jnl_user_abort();
extern    void      jnl_para_start();
extern    void      jnl_para_end();
extern    void      jnl_repeat_start();
extern    void      jnl_repeat_end();
extern    void      jnl_remote_start();
extern    void      jnl_remote_end();
extern    void      truncate_line();
extern    int       tpstart();
extern    int       tpend();
extern    int       icstart();
extern    int       icend();
extern    int       modestart();
extern    int       modeend();
extern    int       directivestart();
extern    int       directiveend();


/* lock.c */
extern    int       obtain_lock();
extern    int       obtain_alt_lock();
extern    char *    lockfile();
extern    char *    place_lock();
extern    void      release_locks();
extern    int       lock();
extern    int       unlock();

/* main.c */
/*        int 	    main() */


/* misc.c */
extern    int       setmode();
extern    int       ifmode();
extern    int       set_gtimeout();
extern    int       gtimeout();
extern    int       mode_message();
extern    int       setup_complete();
extern    int       abort_requested();
extern    int       set_abort_req();
extern    int       xresd_syncd_init();


/* modes.c */
extern    int       export(); 
extern    int       check_line();
extern    void      add_tetenv();
extern    char *    get_tetenv();
extern    ENV_T *   get_env_ptr();
extern    void      create_oc_file();
extern    void      tool_oc_jnl();
extern    void      exec_oc_jnl();
extern    char *    get_output_file();
extern    void      clear_output_file();
extern    void      write_env_file();
extern    void      write_res_file();

/* syntax.c */
extern    int       check_scenario();
extern    int       scan_scen();
extern    int       match();
extern    int       find_scen();
extern    void      go_to_line();
extern    void      parse_param();
extern    void      syntax_error();
extern    int       get_scenario_item();
extern    int       find_item();

/* remote.c */
extern    int       store_sysid();
extern    int       str_to_sysids();
extern    int	    sysid_isstored();
extern    void      clear_sysid_store();

/* rescode.c */
extern    int       get_rescode_file();
extern    int       do_rescode_file();
extern    char *    rescode_num_to_action();
extern    int       rescode_name_to_num();
extern    int       rescode_num_to_index();
extern    char *    rescode_num_to_name();


/* resfile.c */
extern    int       copy_results_file();
extern    int       make_tprs_list();
extern    void      parse_tprs_list();
extern    void      pr_list_to_jnl();
extern    int       analyze_jnl_entry();
extern    TPRS *    collate_and_sort();
extern    int       compare_seq_num();
extern    void      pr_array_to_jnl();


/* save.c */
extern    char **   get_savefiles();
extern    void      clear_savefiles();
extern    void      savefiles();
extern    char *    make_savefiles_dir();
extern    void      remove_savefiles_dir();
extern    void      do_save();


/* scenario.c */
extern    int       perform_scen();
extern    int       analyse_line();
extern    char *    process_line();
extern    void      inclusion_file();
extern    int       make_alt_scenario();


/* spawn.c */
extern    char *    do_copy();

/* store.c */
extern    int       store_tc();
extern    int	    isstored();
extern    void      clear_store();

/* tcc.c */
extern    int       tcc();
extern    void      show_synopsis();

/* testcase.c */
extern    void      bec_tc();
extern    int       proc_tc_dir();
extern    void      make_gsyslist();
extern    void      clean_gsyslist();
extern    int       rem_grp();
extern    int       gsys_wrapper();
extern    int       rem_tc();
extern    int       run_tc();
extern    int       logonoff_sys();

/* util.c */
extern    int       do_mkdir();
extern    char *    number_suffix();
extern    char *    strinstr();
extern    void      get_time();
extern    char *    basename();
extern    int       optget();
extern    void      tet_free();
extern    void *    tet_malloc();
extern    void *    tet_realloc();
extern    void      memory_table();
extern    void      mem_setup();
extern    int       put_tcenv();
extern    char *    get_ts_root();
extern    char *    get_tet_root();
extern    char *    get_alt_exec_dir();
extern    char *    get_temp_dir();
extern    void      cat_path();
extern    void      comb_paths();
extern    char *    store_paths();
extern    char *    get_results_dir();
extern    int       make_path();
extern    int       make_unique_dir();
extern    int       validate_pathnames();
extern    int       check_access();
extern    char *    store_str();
extern    char *    combine_str();
extern    int       remove_dir();


#endif

/**********   End of Non-ANSI C Function Declarations   **********/
