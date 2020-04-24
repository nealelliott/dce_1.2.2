/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 * @DEC_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: command.h,v $
 * Revision 1.1.10.2  1996/02/18  23:45:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:43:59  marty]
 *
 * Revision 1.1.10.1  1995/12/07  22:14:58  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/20  19:02 UTC  jrr
 * 	Merge second XIDL drop for DCE 1.2.1
 * 	[1995/11/17  17:02 UTC  dat  /main/dat_xidl2/1]
 * 
 * 	fix standard versioning
 * 	[1995/10/31  17:26:17  bfc]
 * 	 *
 * 
 * 	oct 95 idl drop
 * 	[1995/10/23  00:43:06  bfc]
 * 
 * 	may 95 idl drop
 * 	[1995/10/21  22:55:54  bfc]
 * 
 * 	DCE for DEC OSF/1: populate from OSF DCE 1.1
 * 	[1995/10/21  17:23:44  bfc]
 * 
 * 	HP revision /main/HPDCE02/2  1994/11/01  23:55 UTC  lmm
 * 	Merging idl++ mods into Mothra
 * 
 * 	HP revision /main/HPDCE02/lmm_merge_idlxx/1  1994/11/01  23:51 UTC  lmm
 * 	Merging idl++ mods into Mothra
 * 
 * 	HP revision /main/HPDCE02/1  1994/08/03  16:31 UTC  tatsu_s
 * 	Merged mothra up to dce 1.1.
 * 
 * 	HP revision /main/HPDCE01/1  1994/01/24  19:41  lmm
 * 	merge kk and hpdce01
 * 	[1995/12/07  21:10:29  root]
 * 
 * Revision 1.1.8.4  1993/09/10  15:36:34  lmm
 * 	Fixed definition of opt_instrument to account for DUMPERS
 * 	[1993/09/10  15:36:13  lmm]
 * 
 * Revision 1.1.6.2  1993/07/07  19:56:49  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:30:34  ganni]
 * 
 * $EndLog$
 */
/*
**  @OSF_COPYRIGHT@
**
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**  Digital Equipment Corporation, Maynard, Mass.
**  All Rights Reserved.  Unpublished rights reserved
**  under the copyright laws of the United States.
**
**  The software contained on this media is proprietary
**  to and embodies the confidential technology of
**  Digital Equipment Corporation.  Possession, use,
**  duplication or dissemination of the software and
**  media is authorized only pursuant to a valid written
**  license from Digital Equipment Corporation.
**
**  RESTRICTED RIGHTS LEGEND   Use, duplication, or
**  disclosure by the U.S. Government is subject to
**  restrictions as set forth in Subparagraph (c)(1)(ii)
**  of DFARS 252.227-7013, or in FAR 52.227-19, as
**  applicable.
**
**
**  NAME:
**
**      command.h
**
**  FACILITY:
**
**      Interface Definition Language (IDL) Compiler
**
**  ABSTRACT:
**
**  Definitions for IDL command line parsing.
**
**  VERSION: DCE 1.0
**
**  MODIFICATION HISTORY:
**   
**  13-Feb-92   harrow      Add support for bug 4, ship holes for arrays of
**                          ref pointers when contained within other types.
*/

#ifndef COMMAND_H
#define COMMAND_H


#include <nidl.h>               /* IDL common defs */
#include <nametbl.h>            /* Nametable defs */

/*
 *  IDL compiler command line options are kept in two parallel arrays,
 *  cmd_opt and cmd_val.  These arrays are passed as arguments to any
 *  routines that need to access command line information.  The partial
 *  signature of such routines is:
 *
 *  rtn
 *  (
 *      boolean     *cmd_opt,        * [in] Array of command option flags *
 *      void        **cmd_val,       * [in] Array of command option values *
 *      ...
 *  )
 *
 *  cmd_opt is an array of booleans.  Each entry is set to "true" if the
 *  corresponding option is selected and set to "false" if it is not.
 *  Some options, when set to "true", contain additional information in
 *  the cmd_val array.
 *
 *  cmd_val is an array of (void *) elements.  If an element in the cmd_opt
 *  array is set to "false", the corresponding element in the cmd_val array
 *  will be equal to NULL.  If an element in the cmd_opt array is set to
 *  "true", the corresponding element in the cmd_val array will in general
 *  contain additional information for that option.  The obvious exceptions
 *  to this rule are any "True/False" options, for which no additional
 *  information is necessary.
 *
 *  Valid elements of the cmd_val array point to additional data for
 *  the corresponding option.  The additional data can be of a simple type
 *  or a constructed type, depending on the information needed to describe
 *  the option.  For most is it simply (char *), a pointer to a string.
 *
 *  The list of #define's below define a set of indices into the two arrays.
 *  The meaning of each option should be obvious from its index name, which
 *  closely resembles the corresponding command option.
 *
 *  Comments appear next to those options for which there is additional
 *  information in the cmd_val array.  The comment indicates the actual
 *  data type of the cmd_val array element for that option, and what it
 *  represents.
 *
 *  *NOTE*: When adding new options to the list below, be sure to also modify
 *          the opt_info array for the dump_cmd_data function (command.c).
 */

#define opt_caux             0  /* (char *)     Client auxiliary file name */
#define opt_cc_cmd           1  /* (char *)     C command line */
#define opt_cc_opt           2  /* (char *)     Addtl C command line options */
#define opt_cepv             3
#define opt_confirm          4
#define opt_cpp              5  /* (char *)     Filespec of CPP to invoke */
#define opt_cpp_def          6  /* (char **)    Array of define strs for CPP */
#define opt_cpp_opt          7  /* (char *)     Addtl CPP cmd line options */
#define opt_cpp_undef        8  /* (char **)    Array of undef strs for CPP */
#define opt_cstub            9  /* (char *)     Client stub file name */
#define opt_def_idir        10
#define opt_do_bug          11  /* (boolean *)  Array of "bug" flags */
#define opt_emit_cstub      12
#define opt_emit_sstub      13
#define opt_header          14  /* (char *)     Header file name */
#define opt_idir            15  /* (char **)    Array of include dirs */
#define opt_keep_c          16
#define opt_keep_obj        17
#define opt_mepv            18
#define opt_out             19  /* (char *)     Output directory */
#define opt_saux            20  /* (char *)     Server auxiliary file name */
#define opt_source          21  /* (char *)     Source IDL file name */
#define opt_space_opt       22
#define opt_sstub           23  /* (char *)     Server stub file name */
#define opt_stdin           24
#define opt_syntax_check    25
#define opt_verbose         26
#define opt_version         27
#define opt_warn            28
#define opt_dia             29
#define opt_standard        30	/* (int)     Standard level */
#define opt_cxxmgr          40
#define opt_lang            41




/*
 * Remaining options are valid only when code built with DUMPERS.
 */
#ifndef DUMPERS
#define NUM_OPTS            opt_lang+1
#else
#define opt_dump_acf        opt_lang+1
#define opt_dump_ast        opt_dump_acf+1
#define opt_dump_ast_after  opt_dump_ast+1
#define opt_dump_cmd        opt_dump_ast_after+1
#define opt_dump_debug      opt_dump_cmd+1
#define opt_dump_flat       opt_dump_debug+1
#define opt_dump_mnode      opt_dump_flat+1
#define opt_dump_mool       opt_dump_mnode+1
#define opt_dump_nametbl    opt_dump_mool+1
#define opt_dump_recs       opt_dump_nametbl+1
#define opt_dump_sends      opt_dump_recs+1
#define opt_dump_unode      opt_dump_sends+1
#define opt_dump_uool       opt_dump_unode+1
#define opt_dump_yy         opt_dump_uool+1
#define opt_ool             opt_dump_yy+1
#define NUM_OPTS            opt_ool+1
#endif



/*
 * Indices into the array of booleans pointed to by cmd_val[opt_do_bug].
 * Note that valid indices start at 1, not 0!
 */
#define bug_array_align  1
#define bug_array_align2 2
#define bug_boolean_def  3
#define bug_array_no_ref_hole 4 /* Leave no hole for array of ref pointers */
#define NUM_BUGS         4

/* Flag values for check and standard */
#define opt_standard_dce_1_0 100
#define opt_standard_dce_1_1 110
#define opt_standard_dce_1_2 115

/* Flag values for lang */
#define opt_lang_c       0
#define opt_lang_cxx	 1
#if defined(MIA) && (defined(VMS) || defined(__osf__) || (defined(_ALPHA_) && defined(_MSDOS)) || defined(DUMPERS))
#else
#endif

/* Data exported by command.c */

extern char *CMD_def_cpp_cmd;   /* Default cpp command */

/* Functions exported by command.c */

extern boolean CMD_parse_args(
#ifdef PROTO
    int             argc,
    char            **argv,
    boolean         **p_cmd_opt,
    void            ***p_cmd_val,
    STRTAB_str_t    *idl_sid
#endif
);

extern void CMD_explain_args(
#ifdef PROTO
    void
#endif
);

extern void FE_set_lang_com(
#ifdef PROTO
    void
#endif
);

extern boolean CMD_DCL_interface;
extern boolean   *CMD_opts; /* True/False values for command options */
extern void     **CMD_vals; /* Values associated w/ options (if any) */

#endif /* COMMAND_H */
