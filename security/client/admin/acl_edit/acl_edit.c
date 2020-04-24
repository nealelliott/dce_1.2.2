/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: acl_edit.c,v $
 * Revision 1.1.9.2  1996/03/11  13:26:05  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:09  marty]
 *
 * Revision 1.1.9.1  1995/12/08  17:55:14  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/01/30  22:42 UTC  sommerfeld
 * 	Remove reference to hpdce_version.h or hpdfs_version.h
 * 	[1995/01/27  04:33 UTC  sommerfeld  /main/HPDCE02/sommerfeld_mothmk_2/1]
 * 
 * 	HP revision /main/HPDCE02/1  1994/11/15  21:21 UTC  mullan_s
 * 	Merge.
 * 	[1995/12/08  16:28:24  root]
 * 
 * Revision 1.1.7.1  1994/03/25  14:42:19  sekhar
 * 	[ OT 10164 ]  Security SVC code drop ( sec_svc_bl1 )
 * 	[1994/03/25  14:39:50  sekhar]
 * 
 * 	[ OT 10164 ]  Security SVC code drop ( sec_svc_bl1 )
 * 
 * Revision 1.1.5.4  1993/01/08  22:16:53  tom
 * 	Bug 6413 - Call setlocale for correct i18n.
 * 	[1993/01/08  21:00:31  tom]
 * 
 * Revision 1.1.3.2  1993/05/25  16:31:24  rps
 * 	standardize version string
 * 	[1993/05/25  16:21:33  rps]
 * 
 * Revision 1.1.5.3  1992/12/29  12:36:42  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:32:38  zeliff]
 * 
 * Revision 1.1.5.2  1992/09/29  21:14:24  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:18:08  sekhar]
 * 
 * Revision 1.1.2.3  1992/05/27  21:14:48  burati
 * 	Make sure it exits with a status from main()
 * 	[1992/05/27  18:42:29  burati]
 * 
 * Revision 1.1.2.2  1992/05/08  15:26:16  burati
 * 	Add new functionality to support POSIX mask calculation
 * 	[1992/05/06  21:43:47  burati]
 * 
 * Revision 1.1  1992/01/19  14:45:27  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  acl_edit.c V=11 12/02/91 //littl/prgy/src/admin/dacl_edit
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/*  acl_edit.c
**      SEC_ACL Editor base source file
** 
*/

#include <stdio.h>
#include <string.h>
#include <locale.h>

#include <dce/dce.h>

#include <acl_edit.h>
#include <tty_base.h>
#include <deif.h>
#include <macros.h>
#include <un_io.h>

#include <dcesadsvc.h>
#include <dce/dce_msg.h>
#include <dce/dcesadmsg.h>

PUBLIC boolean32 verbose;

/*
 * Prototypes for internal routines
 */

int init_sec_acl_gui(
#ifdef __STDC__
int  argc,
char **argv
#endif
);

void run_sec_acl_gui(
#ifdef __STDC__
void
#endif
);


/*
 * Internal routines
 */
int main
#ifndef __STDC__
    ( argc, argv ) 
    int argc;
    char *argv[];
#else
  (
    int argc,
    char *argv[]
  )
#endif
{
    int                         must_prompt, i;
    error_status_t              st;
    boolean32                   use_gui       = true;
    boolean32                   bind_to_entry = false;
    sec_acl_type_t              sec_acl_type  = sec_acl_type_object;
    sec_acl_edit_mask_calc_t    mask_calc_type;

    setlocale(LC_ALL, "");

#ifdef CMA_INCLUDE
    cma_init();
#endif

    dce_svc_set_progname("acl_edit", &st);
    dce_msg_define_msg_table(sad__table,
        sizeof sad__table / sizeof sad__table[0], &st);
    
        /* Unless told otherwise, POSIX mask_obj calculation */
    mask_calc_type = POSIX_MASK_OBJ_CALC;
    verbose = false;
    CLEAR_STATUS(&st);

    if (isatty(0)) {
	must_prompt = true;
    } else {
	must_prompt = false;
    }

    if (argc < 2) {
        fprintf(stderr,"No object specified for SEC_ACL manipulation.\n");
        fprintf(stderr,"Usage:\n\t");
        fprintf(stderr,
            "%s [-e] object [-ic | -io] [-n | -c] [cmd_line_options] [-ngui]\n",
            argv[0]);
        exit(1);
    }
    argv++;argc--;

    /* To be tolerant of the user specifying -ic | -io anywhere in the
     * argument list, we must check for their occurrence before binding
     * to the object, since the bind operation needs to know which of the
     * sec_acl_types we're trying to bind to.  And as long as we're
     * traversing the arguments, check for the -ngui argument, which is
     * also position independent
     */
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-ngui") == 0) {
            use_gui = false;
        } else if (strcmp(argv[i],"-v") == 0) { /* VERBOSE */
            verbose = true;
        } else if (strcmp(argv[i],"-ic") == 0) {    /* INITIAL DIR DEFAULT */
            sec_acl_type = sec_acl_type_default_container;
        } else if (strcmp(argv[i],"-io") == 0) {    /* INITIAL FILE DEFAULT */
            sec_acl_type = sec_acl_type_default_object;
#ifdef DEBUG
        } else if (strcmp(argv[i],"-dbg") == 0) {   /* RUNTIME DEBUGGING */
            rpc__dbg_set_switches("0.1", &st);
#endif
        } else if (strcmp(argv[i],"-e") == 0) {
            bind_to_entry = true;
        } else if (strcmp(argv[i],"-c") == 0) {
            if (mask_calc_type != POSIX_MASK_OBJ_CALC) {
                SET_STATUS(&st, ae_tty_incompatible_options);
                deif_status_print(&st, use_gui);
            } else {
                mask_calc_type = CALC_MASK_OBJ_AFTER;
            }
        } else if (strcmp(argv[i],"-n") == 0) {
            if (mask_calc_type != POSIX_MASK_OBJ_CALC) {
                SET_STATUS(&st, ae_tty_incompatible_options);
                deif_status_print(&st, use_gui);
            } else {
                mask_calc_type = NO_MASK_OBJ_CALC;
            }
        }
    }

    /* Can't do anything else if args are messed up */
    if (BAD_STATUS(&st)) {
        exit(1);
    }

    /*  For debugging purposes, allow "-addr tower residual" instead of the
     *  required object name.  This assumes the 1st 3 args are used to specify
     *  the object.  If -addr is specified and < 3 args exist, then abort.
     */
    if (strcmp(*argv, "-addr") == 0) {
        if (argc < 3) {
            fprintf(stderr,"Incorrect -addr opts; expected tower & residual\n");
            exit(1);
        }
        if (! deif_debug_bind(argv[1],argv[2],sec_acl_type,&mask_calc_type)) {
            fprintf(stderr,"Unable to bind to object %s %s\n", argv[1],argv[2]);
            exit(1);
        }
        else {
            argv = &(argv[3]);
            argc = argc - 3;
        }
    } else {                      /* Normal binding code */
        if (strcmp(*argv,"-e") == 0) {
            argv++;
            argc--;
            bind_to_entry = true;
        }
        if (! deif_bind(*argv, bind_to_entry, sec_acl_type, &mask_calc_type)) {
            fprintf(stderr, "Unable to bind to object %s\n", *argv);
            exit(1);
        }
        else {
            argv++;
            argc--;
        }
    }

    /* Execute cmd from args if possible */
    st = tty_process_args(argc, argv, &mask_calc_type);

    /* If there was no cmd, use one of the interfaces to manipulate entries */
    if (STATUS_EQUAL(&st, ae_tty_args_not_command)) {
        CLEAR_STATUS(&st);
        if (use_gui && init_sec_acl_gui(argc, argv))
            run_sec_acl_gui();
        else {
            tty_sec_acl_cli(must_prompt, "sec_acl_edit> ", &mask_calc_type);
        }
    }
    exit(st);
}

#ifndef GUI
int init_sec_acl_gui
#ifndef __STDC__
    (argc, argv)
int  argc;
char **argv;
#else
  (
int  argc,
char **argv
  )
#endif
{
    return(0);
}

void run_sec_acl_gui(
#ifdef __STDC__
    void
#endif
)
{
    return;
}
#endif /* GUI */
