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
 * $Log: frontend.c,v $
 * Revision 1.1.13.2  1996/02/18  18:51:50  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:05:19  marty]
 *
 * Revision 1.1.13.1  1995/12/07  22:17:37  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/dat_xidl2/1  1995/11/17  17:03 UTC  dat
 * 	Merge second XIDL drop for DCE 1.2.1
 * 	[1995/12/07  21:11:29  root]
 * 
 * Revision 1.1.2.1  1995/10/23  01:48:03  bfc
 * 	oct 95 idl drop
 * 	[1995/10/23  00:43:41  bfc]
 * 
 * 	may 95 idl drop
 * 	[1995/10/21  22:57:07  bfc]
 * 
 * 	DCE for DEC OSF/1: populate from OSF DCE 1.1
 * 	[1995/10/21  17:24:24  bfc]
 * 
 * Revision 1.1.8.1  1993/10/14  12:34:30  hinxman
 * 	CR 8897 IDL compiler can fail silently with an internal error
 * 	[1993/10/14  12:34:03  hinxman]
 * 
 * Revision 1.1.6.2  1993/07/07  19:58:58  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:32:07  ganni]
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
**  NAME
**
**      FRONTEND.C
**
**  FACILITY:
**
**      Interface Definition Language (IDL) Compiler
**
**  ABSTRACT:
**
**      Mainline for IDL compilers.
**
**  VERSION: DCE 1.0
**
*/

#include <signal.h>

#ifdef vms
#  include <types.h>
#  include <stat.h>
#  include <stsdef.h>
#  include <descrip.h>
#else
#  include <sys/types.h>
#  include <sys/stat.h>
#  include <fcntl.h>
#endif

#include <time.h>

#include <frontend.h>
#include <astp.h>
#include <astp_dmp.h>
#include <checker.h>
#include <command.h>
#include <errors.h>
#include <files.h>
#include <getflags.h>
#include <propagat.h>
#include <y_tab.h>
#include <message.h>

#define CONFIG_SUFFIX ".acf"

#ifndef VMS
#include <errno.h>

/*
 * This macro will allow an umlimited number of imports, as long as
 * the depth of imports (with the c-preprosser) never exceeds the
 * platforms max file open quuta, (NT-20, OSF-64).
 *
 */

#define PCLOSE(stream) { \
   fflush(stream); \
   pclose(stream); \
   }

/*
 * To minimize the limitation on the number of open files,
 * this code has been replaced by above the pclose macro.
 * We can no longer reproduce the problem that this work-arround was
 * intended for, and therefore have disabled it...
 *
 *
 * Macro to "close" a pipe- don't actually call pclose since it can cause hangs
 * in certain instances where a parent process opens and closes several pipes,
 * apparently due to the underlying wait() call from pclose and the unpredict-
 * ability of the order of child exits.
 *
 * Silently read to eof extra lines should cause COMPABORT msg anyway
 *
 *  #define PCLOSE(stream) { \
 *    char buf[256]; \
 *    while (fgets(buf, sizeof(buf), stream) != NULL) ; \
 *    }
 */

#endif /* VMS */

extern int yyparse(
#ifdef PROTO
    void
#endif
);

extern void acf_cleanup(
#ifdef PROTO
    void
#endif
);

extern void acf_init(
#ifdef PROTO
    boolean     *cmd_opt_arr,   /* [in] Array of command option flags */
    void        **cmd_val_arr,  /* [in] Array of command option values */
    char        *acf_file       /* [in] ACF file name */
#endif
);


/* Local data definitions. */

static boolean      *saved_cmd_opt;     /* Array of command option flags */
static void         **saved_cmd_val;    /* Array of command option values */

typedef struct FE_import_file_n_t {
        struct FE_import_file_n_t * next;
    STRTAB_str_t   imported_fn_id;
    STRTAB_str_t   imported_full_fn_id;
} FE_import_file_n_t;

static FE_import_file_n_t * imported_file_list = NULL;

extern boolean ASTP_parsing_main_idl;

/*
 * Declare any lex/yacc global data that must be saved for a recursive parse.
 */
LEX_YACC_EXTERNS;


/*
**  i n i t
**
**  Frontend-specific initialization.
*/

#ifdef PROTO
static void FE_init(void)
#else
static void FE_init()
#endif
{
    saved_cmd_opt = NULL;
    saved_cmd_val = NULL;

    KEYWORDS_init();
    NAMETABLE_init();
    AST_init();
}


/*
**  c p p
**
**  Sends the source file through CPP before giving it to lex.
**  The cpp_output argument is a file ID for the output from cpp.
**      UNIX only:      cpp_output is connected to piped output from cpp.
**      VAX/VMS only:   cpp_output is connected to a temp file of cpp output.
*/

#if defined(CPP)
static void cpp
#ifdef PROTO
(
    char        *cpp_cmd,       /* [in] Base command to invoke cpp */
    char        *cpp_opt,       /* [in] Addtl command options for cpp */
    char        *file_name,     /* [in] Source full filespec; "" => stdin */
    char        *dst_file_name, /* [in] Target filespec (VMS) */
    char        **def_strings,  /* [in] List of #define's for preprocessor */
    char        **undef_strings,/* [in] List of #undefine's for preprocessor */
    char        **idir_list,    /* [in] List of -I directories */
    FILE        **cpp_output    /*[out] File ID of cpp output */
)
#else
(cpp_cmd, cpp_opt, file_name, dst_file_name, def_strings, undef_strings,
idir_list, cpp_output)
    char        *cpp_cmd;       /* [in] Command to invoke cpp */
    char        *cpp_opt;       /* [in] Addtl command options for cpp */
    char        *file_name;     /* [in] Source full filespec; "" => stdin */
    char        *dst_file_name; /* [in] Target filespec (VMS) */
    char        **def_strings;  /* [in] List of #define's for preprocessor */
    char        **undef_strings;/* [in] List of #undefine's for preprocessor */
    char        **idir_list;    /* [in] List of -I directories */
    FILE        **cpp_output;   /*[out] File ID of cpp output */
#endif

{
    extern FILE *yyin;
    extern FILE *popen();
#ifdef VMS
    boolean     paren_flag;
    char        dir[max_string_len], name[max_string_len], type[max_string_len];
    char        expanded_file_name[max_string_len];
    int         system_status;
#endif
    char        cmd[max_string_len];    /* Command to spawn cpp */

    cmd[0] = '\0';

    /* Put together beginning of command. */

    strcpy(cmd, cpp_cmd);
    strcat(cmd, " ");
#ifdef VMS
    strcat(cmd, "/PREPROCESS=");
    strcat(cmd, dst_file_name);
    strcat(cmd, " ");
#endif
    strcat(cmd, cpp_opt);
    strcat(cmd, " ");
#ifndef VMS
    strcat(cmd, file_name);
#else
    /* On VMS, hack so source filespec in U*ix format still works. */
    FILE_parse(file_name, dir, name, type);
    FILE_form_filespec(name, dir, type, (char *)NULL, expanded_file_name);
    strcat(cmd, expanded_file_name);
#endif

    /* Append the -D strings. */

#ifdef VMS
    if (*def_strings)
    {
        paren_flag = TRUE;
        strcat(cmd, " /DEFINE=(");
    }
    else
        paren_flag = FALSE;
#endif

    while (*def_strings)
    {
#ifndef VMS
        strcat(cmd, " -D");
#endif
        strcat(cmd, *def_strings++);
#ifdef VMS
        strcat(cmd, ",");
#endif
    }

#ifdef VMS
    if (paren_flag)
        /* Overwrite trailing comma with paren. */
        cmd[strlen(cmd)-1] = ')';
#endif

    /* Append the -U strings. */

#ifdef VMS
    if (*undef_strings)
    {
        paren_flag = TRUE;
        strcat(cmd, " /UNDEFINE=(");
    }
    else
        paren_flag = FALSE;
#endif

    while (*undef_strings)
    {
#ifndef VMS
        strcat(cmd, " -U");
#endif
        strcat(cmd, *undef_strings++);
#ifdef VMS
        strcat(cmd, ",");
#endif
    }

#ifdef VMS
    if (paren_flag)
        /* Overwrite trailing comma with paren. */
        cmd[strlen(cmd)-1] = ')';
#endif

    /* If cpp_cmd is the default, append the -I directories. */

    if (strcmp(cpp_cmd, CMD_def_cpp_cmd) == 0)
    {
#ifdef VMS
        if (*idir_list)
        {
            paren_flag = TRUE;
            strcat(cmd, " /INCLUDE_DIRECTORY=(");
        }
        else
            paren_flag = FALSE;
#endif

        while (*idir_list)
        {
#ifndef VMS
            strcat(cmd, " -I");
#endif
            strcat(cmd, *idir_list++);
#ifdef VMS
            strcat(cmd, ",");
#endif
        }

#ifdef VMS
        if (paren_flag)
            /* Overwrite trailing comma with paren. */
            cmd[strlen(cmd)-1] = ')';
#endif
    }

    /* Now execute the cpp command and open output file or pipe. */

    if (saved_cmd_opt[opt_verbose])
        message_print(NIDL_RUNCPP,cmd);

#ifdef VMS
    {
        $DESCRIPTOR(vcmd, cmd);
        vcmd.dsc$w_length = strlen(cmd);
        system_status = LIB$SPAWN(&vcmd);
        if (($VMS_STATUS_SEVERITY(system_status) == STS$K_ERROR) ||
            ($VMS_STATUS_SEVERITY(system_status) == STS$K_SEVERE))
        {
            idl_error_list_t errvec[2];
            errvec[0].msg_id = NIDL_INVOKECPP;
            errvec[1].msg_id = NIDL_SYSERRMSG;
#ifdef VAXC
            errvec[1].arg1   = strerror(EVMSERR,system_status);
#else
            errvec[1].arg1   = strerror(EVMSERR);
#endif
            error_list(2, errvec, TRUE);
        }
    }
    FILE_open(dst_file_name, cpp_output);
#endif

#ifndef VMS
#ifdef WIN95
    strcat(cmd, " > ");
    strcat(cmd, dst_file_name);
    system(cmd);

    fflush(stdout);
    fflush(stdin);
    if ((*cpp_output = fopen(dst_file_name, "r")) == NULL)
        error(NIDL_INVOKECPP);
    
#else
    if ((*cpp_output = popen(cmd, "r")) == 0)
        error(NIDL_INVOKECPP);
#endif
#endif
}
#endif


/*
**  p a r s e _ a c f
**
**  Invokes the ACF (Attribute Configuration File) parser on specified file.
**
**  Note:   If cmd_opt[opt_confirm], then no real work is done except to
**          issue messages for the verbose option.
**
**  Returns:    == 0    => success
**              != 0    => failure
*/

static boolean parse_acf        /* Returns true on success */
#ifdef PROTO
(
    boolean     *cmd_opt,       /* [in] Array of command option flags */
    void        **cmd_val,      /* [in] Array of command option values */
    char        *acf_file       /* [in] ACF full file name */
)
#else
(cmd_opt, cmd_val, acf_file)
    boolean     *cmd_opt;       /* [in] Array of command option flags */
    void        **cmd_val;      /* [in] Array of command option values */
    char        *acf_file;      /* [in] ACF full file name */
#endif

{
    extern int acf_yyparse(
#ifdef PROTO
        void
#endif
    );

    extern FILE *acf_yyin;
    extern int  acf_yylineno;
    extern int  acf_yynerrs;
    extern int  acf_yyprevious;
    extern char acf_yytext[];

    FILE        **yyin_sp;              /* Used to save yy pointer variables */
    int         *yylineno_sp;
    int         *yynerrs_sp;
    char        *yytext_sp;
    char        temp_path_name[max_string_len]; /* Full temp file pathname */

    if (cmd_opt[opt_verbose])
        message_print(NIDL_PROCESSACF, acf_file);
    if (cmd_opt[opt_confirm])
        return true;

    /*
     * lex & yacc intializations
     */
    yylineno = 1;                       /* Not sure why/if these 3 are needed */
    yyprevious = (int)'\n';
    yysptr= yysbuf;

    acf_yylineno = 1;
    acf_yyprevious = (int)'\n';

    /*
     * Hack the yy pointer variables to point at the ACF-specific yy variables.
     * Then parse the ACF, and restore the yy pointer variable to their
     * original state.  This hack allows us to share error reporting routines
     * with the main parser.  See errors.h for details.
     */
    yyin_sp     = yyin_p;
    yylineno_sp = yylineno_p;
    yynerrs_sp  = yynerrs_p;
    yytext_sp   = yytext_p;

    yyin_p      = &acf_yyin;
    yylineno_p  = &acf_yylineno;
    yynerrs_p   = &acf_yynerrs;
    yytext_p    = acf_yytext;

    acf_init(cmd_opt, cmd_val, acf_file);

#if defined(CPP)
    if (cmd_opt[opt_cpp])
    {
#if defined(VMS) || defined(WIN95)
        char temp_file_name[max_string_len];
        ASSERTION(max_string_len > L_tmpnam);
        FILE_parse(acf_file, (char *)NULL, temp_file_name, (char *)NULL);

        if(CMD_opts[opt_out] == FALSE)
            FILE_form_filespec(temp_file_name, (char *)NULL, ".acf",
                               (char *)NULL, temp_path_name);
        else
            FILE_form_filespec(temp_file_name, CMD_vals[opt_out], ".acf",
                               (char *)NULL, temp_path_name);
        sprintf(temp_path_name, "%s_%d",temp_path_name,clock());
#else
        temp_path_name[0] = '\0';
#endif

        cpp((char *)cmd_val[opt_cpp],
            (char *)cmd_val[opt_cpp_opt],
            acf_file,
            temp_path_name,
            (char **)cmd_val[opt_cpp_def],
            (char **)cmd_val[opt_cpp_undef],
            (char **)cmd_val[opt_idir],
            &acf_yyin);
    }
    else
#endif
        /* No cpp, just open source file */
        FILE_open(acf_file, &acf_yyin);

    if (acf_yyparse() != 0 && acf_yynerrs == 0)
        log_error(acf_yylineno, NIDL_COMPABORT);

    acf_cleanup();

#if defined(VMS) || defined(WIN95)
    fclose(acf_yyin);
    if (cmd_opt[opt_cpp])
        FILE_delete(temp_path_name);
#else
    if (cmd_opt[opt_cpp])
        PCLOSE(acf_yyin)
    else
        fclose(acf_yyin);
#endif

    yyin_p      = yyin_sp;
    yylineno_p  = yylineno_sp;
    yynerrs_p   = yynerrs_sp;
    yytext_p    = yytext_sp;

    if (acf_yynerrs != 0)
        return false;

    return true;
}


/*
** a l r e a d y _ i m p o r t e d
**
** Checks whether a file is already included in the parse or not.
**
** Returns TRUE if the import file has already been parsed.
** Returns false otherwise  (Including can't find the file.)
**
*/

static boolean already_imported
#ifdef PROTO
(
    STRTAB_str_t import_path_id      /* The name to check */
)
#else
(import_path_id)
    STRTAB_str_t import_path_id;
#endif

{
    char                 new_import_full_fn[max_string_len];
    STRTAB_str_t         new_import_full_fn_id;
    STRTAB_str_t         new_import_fn_id;
    char                 base_file_name[max_string_len];
    char                 base_file_ext[max_string_len];
    struct               stat stat_buf;
    FE_import_file_n_t * imported_file;
    char              ** idir_list;
    boolean              alr_imp;
    char              * file_name;

    /*
     * Get a string to lookup.
     */
    STRTAB_str_to_string (import_path_id, &file_name);
    idir_list = (char **)saved_cmd_val[opt_idir];

    /*
     * Note that a lookup failure will not report a failure here;
     * That will be reported when we actually try to import it.
     */
    if (!FILE_lookup(file_name, idir_list, &stat_buf, new_import_full_fn))
        return false;
    new_import_full_fn_id = STRTAB_add_string(new_import_full_fn);

    /*
     * Make sure there is no partial path information.
     */
    if (!FILE_parse(new_import_full_fn, NULL, base_file_name, base_file_ext))
        return false;

    strncat(base_file_name, base_file_ext, max_string_len);
    new_import_fn_id = STRTAB_add_string(base_file_name);

    /*
     * Initialize the default return status.
     */
    alr_imp = false;

    imported_file = imported_file_list;

    while ((imported_file != NULL) && (!alr_imp))
    {
        if (new_import_fn_id == imported_file->imported_fn_id)
        {
            /*
             * A match is found in the list. We are done.
             */
            alr_imp = true;
        }
        imported_file = imported_file -> next;
    }

    /*
     * Record the import we are about to do if not imported already.
     */
    if (!alr_imp)
    {
        imported_file = (FE_import_file_n_t *) MALLOC (sizeof(FE_import_file_n_t));
        imported_file -> imported_fn_id = new_import_fn_id;
        imported_file -> imported_full_fn_id = new_import_full_fn_id;
        imported_file -> next = imported_file_list;
        imported_file_list = imported_file;
    }

    return alr_imp;
}


/*
**  p a r s e
**
**  Invokes the parser on the specified IDL file.  If the IDL file is
**  successfully parsed, and there is a related ACF file, then the ACF
**  file is parsed also.
**
**  Returns:    a pointer to the interface binding if the IDL file was parsed
**              successfully and the ACF file (if any) was parsed successfully;
**              NULL otherwise.
*/

static boolean parse
#ifdef PROTO
(
    boolean     *cmd_opt,       /* [in] Array of command option flags */
    void        **cmd_val,      /* [in] Array of command option values */
    STRTAB_str_t idl_sid,       /* [in] IDL filespec stringtable ID */
                                /*      STRTAB_NULL_STR => stdin */
    boolean     idir_valid,     /* [in] true => use import directory list */
    AST_interface_n_t **int_p   /*[out] Ptr to interface node */
)
#else
(cmd_opt, cmd_val, idl_sid, idir_valid, int_p)
    boolean     *cmd_opt;       /* [in] Array of command option flags */
    void        **cmd_val;      /* [in] Array of command option values */
    STRTAB_str_t idl_sid;       /* [in] IDL filespec stringtable ID */
                                /*      STRTAB_NULL_STR => stdin */
    boolean     idir_valid;     /* [in] true => use import directory list */
    AST_interface_n_t **int_p;  /*[out] Ptr to interface node */
#endif

{
    extern FILE *yyin;
    extern int  yylineno;
    extern char yytext[];

    char        *sf;                            /* Source filespec */
    char        full_path_name[max_string_len]; /* Full source pathname */
    char        temp_path_name[max_string_len]; /* Full temp file pathname */
    STRTAB_str_t full_pn_id;                    /* Full src path string id */
    char        **idir_list;                    /* List of search directories */
    char        file_dir[max_string_len];       /* Directory part of src file */
    boolean     file_dir_is_cwd;                /* T => file_dir current dir */
    char        file_name[max_string_len];      /* File name part of src file */
    char        acf_file[max_string_len];       /* ACF file name w/o dir */
    char        full_acf_name[max_string_len];  /* Full ACF pathname */
    boolean     acf_exists;                     /* T => ACF file exists */
    struct stat stat_buf;                       /* File lookup stats */
    int         i;


    /* One-time saving of command array addresses to static storage. */
    if (saved_cmd_opt == NULL)
        saved_cmd_opt = cmd_opt;
    if (saved_cmd_val == NULL)
        saved_cmd_val = cmd_val;

    /*
     * If the idir_valid flag is set, look in the -I directories for the
     * source file.  Otherwise, just use the filespec as is.
     */
    if (idir_valid)
        idir_list = (char **)cmd_val[opt_idir];
    else
        idir_list = (char **)NULL;

    if (idl_sid == STRTAB_NULL_STR)     /* stdin */
        full_path_name[0] = '\0';
    else
    {
        STRTAB_str_to_string(idl_sid, &sf);
        if  (!FILE_lookup(sf, idir_list, &stat_buf, full_path_name))
        {
            error(NIDL_FILENOTFND, sf);
            return false;
        }
    }

    if (cmd_opt[opt_verbose] && !ASTP_parsing_main_idl)
        message_print(NIDL_IMPORTIDL, full_path_name);

#if defined(CPP)
    if (cmd_opt[opt_cpp])
    {
#if defined(VMS) || defined(WIN95)
        char temp_file_name[max_string_len];
        ASSERTION(max_string_len > L_tmpnam);
        FILE_parse(full_path_name, (char *)NULL, temp_file_name, (char *)NULL);

        if(CMD_opts[opt_out] == FALSE)
            FILE_form_filespec(temp_file_name, (char *)NULL, ".idl",
                               (char *)NULL, temp_path_name);
        else
            FILE_form_filespec(temp_file_name, CMD_vals[opt_out], ".idl",
                               (char *)NULL, temp_path_name);
        sprintf(temp_path_name, "%s_%d",temp_path_name,clock());
#else
        temp_path_name[0] = '\0';
#endif

        cpp((char *)cmd_val[opt_cpp],
            (char *)cmd_val[opt_cpp_opt],
            full_path_name,
            temp_path_name,
            (char **)cmd_val[opt_cpp_def],
            (char **)cmd_val[opt_cpp_undef],
            (char **)cmd_val[opt_idir],
            &yyin);
    }
    else
#endif
        if (full_path_name[0] == '\0')  /* stdin */
            yyin = stdin;
        else
            FILE_open(full_path_name, &yyin);

    /*
     * Setup file name for errors to the full file name
     */
    set_name_for_errors((full_path_name[0] == '\0') ? "stdin" : full_path_name);

    /*
     * lex & yacc intializations
     */
    yylineno = 1;
    yyprevious = (int)'\n';
    yysptr= yysbuf;

    /*
     * Hack the yy pointer variables to point at the IDL-specific yy variables
     * before starting the parse.  This hack allows us to share error reporting
     * routines with the ACF parser.  See errors.h for details.
     */
    yyin_p      = &yyin;
    yylineno_p  = &yylineno;
    yynerrs_p   = &yynerrs;
    yytext_p    = yytext;

    if (yyparse() != 0 && error_count == 0)
        log_error(yylineno, NIDL_COMPABORT);
    *int_p = the_interface;

#if defined(VMS) || defined(WIN95)        
    fclose(yyin);
    if (cmd_opt[opt_cpp])
        FILE_delete(temp_path_name);
#else
    if (cmd_opt[opt_cpp])
        PCLOSE(yyin)
    else
        fclose(yyin);
#endif

    if (error_count != 0)
        return false;        /* Error parsing IDL */

    /* Successful parse: save IDL filespec in interface node. */
    if (the_interface != NULL)
    {
        full_pn_id = STRTAB_add_string(full_path_name);
        the_interface->fe_info->file = full_pn_id;
    }
    else
    {
        if (ASTP_parsing_main_idl) return false;    /* Shouldn't happen */
    }

    /*
     * Now see if there is an associated Attribute Configuration File (ACF).
     * The ACF name is constructed from the IDL file name.  The ACF file can
     * be in any of the -I directories.
     */
    if (!FILE_parse(full_path_name, file_dir, file_name, (char *)NULL))
        return false;

    if (!FILE_form_filespec(file_name, (char *)NULL, CONFIG_SUFFIX,
                            (char *)NULL, acf_file))
        return false;
#ifdef UNIX
    /*
     * If the created ACF filespec matches the file_name portion of the IDL
     * filespec, it implies that the IDL filespec contains multiple '.'s - in
     * this special case, append the ACF suffix to the filespec.
     */
    if (strcmp(file_name, acf_file) == 0)
        strcat(acf_file, CONFIG_SUFFIX);
#endif

    /*
     * If the directory part of the source filespec is not the current dir,
     * tack it on at the end of the include directory list.  Note: we assume
     * that there is enough room for a temporary extra entry in the idir list.
     */
    idir_list = (char **)cmd_val[opt_idir];
    file_dir_is_cwd = FILE_is_cwd(file_dir);

    if (!file_dir_is_cwd)
    {
        for (i = 0 ; idir_list[i] != NULL ; i++)
            ;
        idir_list[i] = file_dir;
        idir_list[i+1] = NULL;
    }

    acf_exists = FILE_lookup(acf_file, idir_list, &stat_buf, full_acf_name);
#ifdef VMS
    if (!acf_exists)
    {
        /*
         * This code is for the special case where foo.idl is the source file
         * but foo is also a logical name; use the full filespec so that foo
         * isn't translated.
         */
        if (!FILE_form_filespec((char *)NULL, (char *)NULL, CONFIG_SUFFIX,
                                full_path_name, acf_file))
            return false;
        acf_exists = FILE_lookup(acf_file, (char **)NULL, &stat_buf,
                                 full_acf_name);
    }
#endif

    if (!file_dir_is_cwd)
        idir_list[i] = NULL;

    if (!acf_exists)
        return true;   /* No ACF; return success */

    /* Parse the ACF. */
    if (!parse_acf(cmd_opt, cmd_val, full_acf_name))
        return false;            /* Error parsing ACF */

    return true;       /* Both IDL and ACF parsed without errors */
}


/*
 *  F E _ p a r s e _ i m p o r t
 *
 *  Parse an import file.
 *  This involves pushing the state of the current parse, initing a few
 *  cells, calling parse, and restoring the state of the current parse.
 *
 *  Parametric inputs: The string table id of the filename we wish to parse.
 *  Global inputs: All the parse and lex static storage that maintains parse
 *                 state.
 *  Parametric outputs: None
 *  Global outputs: The parse state is restored to its value on entry.
 *
 *  Routine value:  None.
 */

AST_interface_n_t *FE_parse_import
#ifdef PROTO
(
    STRTAB_str_t    new_input   /* [in] string table id of file to parse */
)
#else
(new_input)
    STRTAB_str_t    new_input;  /* [in] string table id of file to parse */
#endif

{
LEX_YACC_STATE_BUFFER(saved_state);
boolean saved_ASTP_parsing_main_idl;
char saved_current_file[ PATH_MAX ];
STRTAB_str_t saved_error_file_name_id;
AST_interface_n_t *int_p;
int saved_op_count;

/* Saved interface attributes */
int             saved_interface_pointer_class;
AST_interface_n_t *saved_interface;

/*
 * Return now, if the file is already imported.
 */
    if (already_imported (new_input))
        return (AST_interface_n_t *)NULL;

/*
 * Save information used by error reporting routines.
 */
    SAVE_LEX_YACC_STATE(saved_state);
    saved_ASTP_parsing_main_idl = ASTP_parsing_main_idl;
    inq_name_for_errors(saved_current_file);
    saved_error_file_name_id = error_file_name_id;

/*
 * Save interface information
 */
    saved_interface                             = the_interface;
    saved_interface_pointer_class               = interface_pointer_class;

    /*
     * If cxx, save the operation count so can properly
     * count inherited operations.
     */
    if ((int)CMD_vals[opt_lang] == opt_lang_cxx )
        saved_op_count = the_interface->op_count;


/*
 * Initialize interface attributes
 */
    the_interface = NULL;
    interface_pointer_class = 0;


/*
 * We have now saved away all the state of the current parse.
 * Initialize a few cells, open the imported file and recursively invoke the
 * parse.
 */

    if (saved_interface->inherited_interface_name == NAMETABLE_NIL_ID) {
	ASTP_parsing_main_idl = false;
    }

/*
 * Parse the file.  Routine parse normally returns a AST_interface_n_t,
 * but since we are not parsing the main IDL, we don't care about it.
 * The "true" argument says to search -I directories for the file.
 */

    parse(saved_cmd_opt, saved_cmd_val, new_input, true, &int_p);


    if (saved_interface->inherited_interface_name == the_interface->name) {
	AST_export_n_t *ep = the_interface->exports;
	AST_export_n_t *op;

        if ((int)CMD_vals[opt_lang] == opt_lang_cxx ) {
            /*    
            **  do com style inheritance by pulling in the base 
	    **  class interface's operations.
            */
	    /*saved_interface->inherited_interface_name = NAMETABLE_NIL_ID;*/
	    saved_interface->op_count = the_interface->op_count;

	    for (; ep != NULL; ep = ep->next) {
	        if (ep->kind != AST_operation_k) continue;

		/*
		 * static and constructor operations are not inherited
		 */
	        if (AST_STATIC_SET(ep->thing_p.exported_operation)) continue;
	        if (IS_CONSTRUCTOR(ep->thing_p.exported_operation)) continue;

	        op = AST_export_node(
			 (ASTP_node_t *) ep->thing_p.exported_operation,
			 AST_operation_k
		);

		/*
		 * assign the operation number to this operation
		 * relative to any inherited operations. 
		 */
		op->thing_p.exported_operation->op_number=saved_op_count++;

	        saved_interface->exports = 
		    (AST_export_n_t * ) AST_concat_element(
			   (ASTP_node_t *) saved_interface->exports,
			   (ASTP_node_t*)op
		);
	    }

	    /*
	     * update to operation count to include base class operations
	     */
	    saved_interface->op_count = saved_op_count;
        }
    }

/*
 * Restore interface information
 */
    the_interface                         = saved_interface;
    interface_pointer_class               = saved_interface_pointer_class;


/*
 * The recursive parse is done.  Restore the state of the previous parse.
 * First, the things that are simple to restore. Essentially scalars.
 */
    RESTORE_LEX_YACC_STATE(saved_state);

/*
 * Restore information used by error reporting routines.
 */
    ASTP_parsing_main_idl = saved_ASTP_parsing_main_idl;
    set_name_for_errors(saved_current_file);
    error_file_name_id = saved_error_file_name_id;

    return int_p;
}


/*
**  p a r s e _ i d l
**
**  Parse the source IDL file.
**
**  Side Effects:   Returns the interface in the int_p parameters and
**      a boolean status if any errors were encountered.
*/

static boolean parse_idl        /* Returns true on success */
#ifdef PROTO
(
    boolean     *cmd_opt,       /* [in] Array of command option flags */
    void        **cmd_val,      /* [in] Array of command option values */
    STRTAB_str_t idl_sid,       /* [in] IDL filespec stringtable ID */
                                /*      STRTAB_NULL_STR => stdin */
    AST_interface_n_t **int_p   /*[out] Ptr to interface node */
)
#else
(cmd_opt, cmd_val, idl_sid, int_p)
    boolean     *cmd_opt;       /* [in] Array of command option flags */
    void        **cmd_val;      /* [in] Array of command option values */
    STRTAB_str_t idl_sid;       /* [in] IDL filespec stringtable ID */
                                /*      STRTAB_NULL_STR => stdin */
    AST_interface_n_t **int_p;  /*[out] Ptr to interface node */
#endif

{
    boolean status;                     /* Status to return */
    FE_import_file_n_t *imported_file;  /* Main IDL file info */
    char    *file_name;                 /* Main IDL file name */
    char    imported_fn[max_string_len];/* Main IDL full file name */
    struct stat stat_buf;               /* File lookup info */
    char    file_name_part[max_string_len];/* Main IDL file name part */
    char    file_type_part[max_string_len];/* Main IDL file type part */
    boolean name_warning = false;       /* Warn on name used */

    if (idl_sid != STRTAB_NULL_STR)
    {
        /*
         * Record the main IDL as if it is an imported file.
         * This preserves idempotency in case the main IDL is also imported.
         */
        STRTAB_str_to_string(idl_sid, &file_name);
        FILE_parse(file_name, NULL, file_name_part, file_type_part);
        strcat(file_name_part, file_type_part);

        /*
         * Issue a warning on any system IDL files a user might
         * accidentally chose one of those name and get strange behavior.
         */
        if (!strcmp(file_name_part,"iovector.idl")) name_warning = true;
        else if (!strcmp(file_name_part,"lbase.idl")) name_warning = true;
        else if (!strcmp(file_name_part,"nbase.idl")) name_warning = true;
        else if (!strcmp(file_name_part,"ncastat.idl")) name_warning = true;
        else if (!strcmp(file_name_part,"ndrold.idl")) name_warning = true;
        else if (!strcmp(file_name_part,"rpc.idl")) name_warning = true;
        else if (!strcmp(file_name_part,"rpcsts.idl")) name_warning = true;
        else if (!strcmp(file_name_part,"rpctypes.idl")) name_warning = true;
        else if (!strcmp(file_name_part,"twr.idl")) name_warning = true;
        else if (!strcmp(file_name_part,"uuid.idl")) name_warning = true;

        if (name_warning)
            message_print(NIDL_SYSIDLNAME,file_name);

        /*
         * Note that a lookup failure will not report a failure here;
         * That will be reported when we actually try to parse it.
         */
        if (FILE_lookup(file_name, (char **)NULL, &stat_buf, imported_fn))
        {
            imported_file = (FE_import_file_n_t *)
                            MALLOC(sizeof(FE_import_file_n_t));
            imported_file->imported_fn_id = STRTAB_add_string(file_name_part);
            imported_file->imported_full_fn_id = STRTAB_add_string(imported_fn);
            imported_file->next = imported_file_list;
            imported_file_list = imported_file;
        }
    }

    /*
     * Parse the top-level IDL file.  The "false" argument tells parse not
     * to scan the import directories for the source IDL file.
     */
    *int_p = NULL;
    status = parse(cmd_opt, cmd_val, idl_sid, false, int_p);

    /* Terminate if there were any syntax errors in the top level IDL file. */
    if (*int_p == NULL)
        status = false;

    return status;
}


/*
**  F E _ m a i n
**
**  Main frontend routine.  Invokes each major frontend component.
**
**  Note:   If cmd_opt[opt_confirm], then no real work is done except to call
**          the components that can issue messages for the verbose option.
*/

boolean FE_main                 /* Returns true on success */
#ifdef PROTO
(
    boolean     *cmd_opt,       /* [in] Array of command option flags */
    void        **cmd_val,      /* [in] Array of command option values */
    STRTAB_str_t idl_sid,       /* [in] IDL filespec stringtable ID */
                                /*      STRTAB_NULL_STR => stdin */
    AST_interface_n_t **int_p   /*[out] Ptr to interface node */
)
#else
(cmd_opt, cmd_val, idl_sid, int_p)
    boolean     *cmd_opt;       /* [in] Array of command option flags */
    void        **cmd_val;      /* [in] Array of command option values */
    STRTAB_str_t idl_sid;       /* [in] IDL filespec stringtable ID */
                                /*      STRTAB_NULL_STR => stdin */
    AST_interface_n_t **int_p;  /*[out] Ptr to interface node */
#endif

{
    boolean status;

    /* Frontend-specific initialization. */
    FE_init();

    ASTP_add_internal_types();

    /* Parse the source IDL file (and related ACF if applicable). */
    status = parse_idl(cmd_opt, cmd_val, idl_sid, int_p);

#ifdef DUMPERS
    /* Dump the nametable if requested. */
    if (cmd_opt[opt_dump_nametbl])
        NAMETABLE_dump_tab();
#endif

#ifdef DUMPERS
    /* Dump the AST (before checking) if requested. */
    if (cmd_opt[opt_dump_ast])
        AST_dump_interface(*int_p);
#endif

    /* Propagate attributes throughout the AST. */
    if (status && !cmd_opt[opt_confirm])
        status = PROP_main(cmd_opt, cmd_val, *int_p);

    /* Do semantic checking of the interface. */
    if (status)
    {
        if (!cmd_opt[opt_confirm])
            status = CHECKER_main(cmd_opt, cmd_val, *int_p);
    }
    else
        message_print(NIDL_NOSEMCHECK);


#ifdef DUMPERS
    /* Dump the AST (after other frontend components) if requested. */
    if (cmd_opt[opt_dump_ast_after])
        AST_dump_interface(*int_p);
#endif

    /* Cancel filename for error processing because we are done with source */
    set_name_for_errors(NULL);

    return status;
}
