/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sdi_ui.c,v $
 * Revision 1.1.10.2  1996/02/18  19:36:27  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:17:02  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:31:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:36  root]
 * 
 * Revision 1.1.7.9  1994/08/03  19:04:28  mccann
 * 	final sams drop
 * 	[1994/08/02  14:09:42  mccann]
 * 
 * Revision 1.1.7.8  1994/08/01  15:07:39  zee
 * 	     Make default input of addresses as hex.  Increment version to 2.019.
 * 	[1994/07/29  21:04:57  zee]
 * 
 * Revision 1.1.7.7  1994/06/30  19:18:15  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:48:42  mccann]
 * 
 * Revision 1.1.7.6  1994/06/09  18:43:40  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:16:14  devsrc]
 * 
 * Revision 1.1.7.5  1994/06/08  20:18:02  peckham
 * 	Initialize serviceability and message catalogs.
 * 	[1994/05/31  18:57:02  peckham]
 * 
 * Revision 1.1.7.4  1994/04/29  15:56:13  peckham
 * 	Use dcelocal_path in file names.
 * 	[1994/04/29  14:37:25  peckham]
 * 
 * Revision 1.1.7.3  1994/04/14  14:51:26  peckham
 * 	Remove CMA, VMS and UNIX conditionals.
 * 	Remove dnstables dependencies.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	[1994/04/11  20:21:25  peckham]
 * 
 * Revision 1.1.7.2  1994/03/22  20:00:30  griffin
 * 	HCell BL3 support: Test harness commands for adding/removing cellnames.
 * 	[1994/03/22  18:10:06  griffin]
 * 
 * Revision 1.1.7.1  1994/03/12  22:12:11  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:44:43  peckham]
 * 
 * Revision 1.1.5.5  1992/12/30  14:22:52  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:23:26  zeliff]
 * 
 * Revision 1.1.5.4  1992/12/15  15:55:12  tom
 * 	Bug 6409 - add setlocale call for i18n.
 * 	[1992/12/15  15:17:43  tom]
 * 
 * Revision 1.1.5.3  1992/12/04  14:49:54  mfox
 * 	Exit if not running as root.
 * 	[1992/12/04  14:48:52  mfox]
 * 
 * Revision 1.1.5.2  1992/09/29  19:16:10  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:17:18  weisman]
 * 
 * Revision 1.1.2.2  1992/06/08  15:17:02  mfox
 * 	Print socket error messages in English
 * 	[1992/06/08  15:10:40  mfox]
 * 
 * Revision 1.1  1992/01/19  15:24:52  devrcs
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
 */
/*
 *
 * Module Description:
 *
 *   This module provides a simple command language parser for the
 *   server diagnostic interface.
 *
 */

/*
 *
 *  include files
 *
 */
#include <server.h>
#include <dce/dcesvcmsg.h>
#include <stdio.h>
#include <ctype.h>
#include <locale.h>
#include <sys_mbx.h>
#include <utl_table_mgr.h>
#include <server_diags.h>
#include <dns_strings.h>
#include <dce/dce_priv_proc.h>
#include <dns_cfg.h>


#ifndef __DATE__
#define __DATE__ ""
#endif


/*
 * If we're not sure that strtoul exists, then handle accordingly.
 */

#define STRING_TO_ADDRESS(a) strtoul(a, NULL, 0)

/*
 *  Module-global declarations
 */


static char program_version[] = "V2.019";

#define INITIAL_MESSAGE	"CDS Server Diagnostics - Version %s   (%s)\n\n"

#define MAX_IDENTIFIER_SIZE 256
#define MAX_TOKEN_SIZE 256

enum dtypes {long_val, short_val, byte_val, string_val};

enum tokens { END_OF_FILE, IDENTIFIER, CONSTANT, LITERAL,
                     END_OF_LINE, EQUALS, KEYWORD , ILLEGAL_TOKEN
                   };

/*
 * static char *readable_token[10] = {
 *    "end-of-file", "identifier", "constant", "literal",
 *    "end-of-line", "=", ";", "keyword", "illegal token"
 * };
 */

enum key_codes { EXAMINE, SUMMARIZE, SHOW, 
                        LOAD, ENABLE, DISABLE, 
                        EXIT, FLUSH, CLOSE, HELP,
                        SET, MGMT, MARK, SIGNAL, BROADCAST,
                        NAVIGATE, DUMP };

enum sub_key_codes { S_PROFILE, S_FAST_ASTS,
                            S_ACS_OVERRIDE, S_JANITOR, S_ALT_EVENT_DB,
                            S_NORMAL, S_LOGICAL, S_ALL_ON, S_ALL_OFF,
                            S_CRE_SERVER, S_DIS_SERVER,
                            S_CRE_CLE, 
                            S_INIT, S_REM_CLE, S_CLE_CLE,
                            S_FORCE_CKPT, S_FORCE_BACK, S_ON, S_OFF,
                            S_RANDOMIZE_CH, S_MEM_SWEEP, S_MEM_FULL_ABORT,
                            S_PERM, S_TEMP, S_UNK, S_IGN, S_ALL,
                            S_HISTORY, S_INHIBIT_BACK, S_NODE_VERIF,
                            S_SUPERCHUNK, S_DEL_SERVER, S_COMPRESSION_LEVEL,
                            S_AUTO, S_INHIBIT, S_STROBE_NORMAL, S_STROBE_OFF,
                            S_STROBE_ON, S_STROBE_AUTO, S_FORCE, S_ADV_PING,
                            S_BUFFER, S_AUDIT, S_V1_DEF_FILE, S_INV_ARG_BUFFER,
                            S_DHE_STATE, S_DYING_DIR, S_DYING_REP, S_DEAD,
                            S_NEW_DIR, S_NEW_REP, S_PREFERRED, 
                            S_ADD_CELL_ALIAS, S_REM_CELL_ALIAS };

enum parser_states { DONE, PARSE_ERROR, S1 };


static Hashed_Table keywords      = NULL;
static Hashed_Table sub_keys      = NULL;
static Hashed_Table structure_ids = NULL;

/*
 * current_token holds the token type (see the tokens list above) of the
 * most recent token the lexical processor has detected.
 *
 * current_token_value holds the value of that token.
 *
 */

static enum tokens current_token;                      
static char current_token_value[MAX_TOKEN_SIZE];


/*
 * Whenever a parser error is generated, the parser attempts to give a
 * hint to the user as to what went wrong (as opposed to just saying
 * ERROR).
 */

static char error_hint[255];

/*
 * Generic diagnostic switches (diag_state_t) have common processing routines.
 * The M_xxx values are bitmasks for valid token selections.
 */
#define M_INHIBIT       (1<<0)
#define M_OFF           (1<<1)
#define M_ON            (1<<2)
#define M_NORMAL        (1<<3)
#define M_AUTO          (1<<4)
#define M_STROBE_ON     (1<<5)
#define M_STROBE_OFF    (1<<6)
#define M_STROBE_NORMAL (1<<7)
#define M_STROBE_AUTO   (1<<8)
#define M_FORCE         (1<<9)

/*
 * Mailbox (socket) I/O structures, buffers, etc.
 */

static sys_mailbox diag_channel;             /* Mailbox context */
static diag_response_t response_buffer;
static diag_request_t request;


#define ISSUE_REQUEST(rbuf) \
 status = sys_mbx_write(diag_channel, (unsigned char *) &rbuf, sizeof(rbuf)); \
 if (status != DNS_SUCCESS) { perror("Socket write error"); } \
 else { print_response(diag_channel); }


/*
 * prototype declaration for main routine
 */

int
main (
    int              ,
    char            **);


/* ------------------------------------------------------------------------- */

/*
 * ROUTINE - next_token
 *
 * This is the lexical analyzer for the parser.  It scans the input file
 * and generates a token for each call.  The token type and its value are 
 * stored in the module-global variables: current_token and
 * current_token_value, the routine also returns the current token type for
 * convenience purposes.
 *
 */

static enum tokens 
next_token (void)
{
    int c;       /* c is the current character we are looking at */
    char *ctv;   /* used as a pointer to the current_token_value string */

    /* 
     * Get a character from the input file; skip over whitespace.
     */
    c = fgetc(stdin);
    while (isspace(c) && c != '\n') {
        c = fgetc(stdin);
    }

    /*
     * Non-whitespace character detected. Based on the value of the current
     * character, emit or start building a token.
     *
     * Quoted strings (LITERALs) are processed by looking for a matching
     * double-quotes; no folding or internal string processing is performed.
     *
     * Numeric constants are identified by a leading digit in the token.
     * The constant builder looks for other (hex) digits.
     *
     * Identifiers are identified by a leading alpha character.
     */

    if (c == EOF) {

        strcpy(current_token_value, "");
        return current_token = END_OF_FILE;

    } else if (c == '\n') {

        strcpy(current_token_value, "");
        return current_token = END_OF_LINE;

    /*
     * The following block is commented out to suppress active parsing
     * of the "=" token.  It isn't used anywhere in the diagnostic mini
     * language and the token is useful in X.500 names.   The code is
     * here if we ever want it back.
     */

#ifndef SDI_UI_NO_EQUALS_HACK
    } else if (c == '=') {

        strcpy(current_token_value, "=");
        return current_token = EQUALS;
#endif

    } else if (c == '\"') {

        /* 
         * Got a literal...  Starting with the next character,
         * spin through the input looking for a matching double-quote
         * building up the current_token_value as we go along.
         * Yes, I know I don't check for buffer overflows...
         */

        c = fgetc(stdin); /* quotes aren't part of the token value */

        for (ctv = current_token_value; ((c != '\"') && (c != '\n')); ) {
            *ctv++ = c;
            c = fgetc(stdin);
        }

        *ctv = '\0';
        return current_token = LITERAL;

    } else if (isdigit(c)) {

        /* 
         * Constant detected.  This works pretty much like the literal
         * processing except that we use the current character as part of
         * the token value.
         */

        for (ctv = current_token_value; isxdigit(c) || (c == 'x'); ) {
            *ctv++ = c;
            c = fgetc(stdin);
        }

        *ctv = '\0';
        ungetc(c, stdin);  /* Put back the separator */
        return current_token = CONSTANT;

    } else if (isalpha(c) || (c == '<') || (c == '.') || (c == '/')) {

        /*
         * Identifier detected. Like constants (which are like literals),
         * we build up the token value looking for a separator.
         * After we have an identifier, we check it against the keyword
         * table to determine if the identifier we have just found is
         * a reserved keyword or not.
         */

        for (ctv = current_token_value; isalnum(c) ||
                                        (strchr("<_:/$.[]", c) != NULL); ) {
            *ctv++ = c;
            c = fgetc(stdin);
        }

        *ctv = '\0';
        ungetc(c, stdin);  /* Put back the separator */

        if (lookup_table(keywords, current_token_value, NULL) == TRUE) {
            return current_token = KEYWORD;
        } else {
            return current_token = IDENTIFIER;
        }

    } else {

        return current_token = ILLEGAL_TOKEN;

    }
    
}

/* ------------------------------------------------------------------------- */

/*
 * ROUTINE - rest_of_line
 *
 * This routine retrieves the rest of the command line and treats it as
 * a literal.
 */
static enum tokens 
rest_of_line (void)
{
    int c;       /* c is the current character we are looking at */
    char *ctv;   /* used as a pointer to the current_token_value string */

    /* 
     * Get rest of line (up to newline)
     */
    c = fgetc(stdin);            /* Skip over the separator */
    c = fgetc(stdin);
    for (ctv = current_token_value; c != '\n'; ) {
        *ctv++ = c;
        c = fgetc(stdin);
    }

    *ctv = '\0';
    return current_token = LITERAL;
}

/* ------------------------------------------------------------------------- */

/*
 * ROUTINE - load_keyword_tables
 *
 * This routine initializes the keywords table, a hashed table maintained
 * by the utl_table_mgr module.  We don't have any user-defined data
 * associated with the keywords, so we just pass a NULL pointer to the
 * table manager.
 */

static void 
load_keyword_tables (void)
{
    if (keywords != NULL) {
        return;
    }

    keywords = declare_table(101);
    sub_keys = declare_table(101);

    enter_table(keywords, "examine",        (void *)EXAMINE);
    enter_table(keywords, "ex",             (void *)EXAMINE);
    enter_table(keywords, "summarize",      (void *)SUMMARIZE);
    enter_table(keywords, "sum",            (void *)SUMMARIZE);
    enter_table(keywords, "su",             (void *)SUMMARIZE);
    enter_table(keywords, "set",            (void *)SET);
    enter_table(keywords, "show",           (void *)SHOW);
    enter_table(keywords, "sh",             (void *)SHOW);
    enter_table(keywords, "enable",         (void *)ENABLE);
    enter_table(keywords, "ena",            (void *)ENABLE);
    enter_table(keywords, "disable",        (void *)DISABLE);
    enter_table(keywords, "dis",            (void *)DISABLE);
    enter_table(keywords, "load",           (void *)LOAD);
    enter_table(keywords, "exit",           (void *)EXIT);
    enter_table(keywords, "quit",           (void *)EXIT);
    enter_table(keywords, "q",              (void *)EXIT);
    enter_table(keywords, "flush",          (void *)FLUSH);
    enter_table(keywords, "close",          (void *)CLOSE);
    enter_table(keywords, "help",           (void *)HELP);
    enter_table(keywords, "mgmt",           (void *)MGMT);
    enter_table(keywords, "manage",         (void *)MGMT);
    enter_table(keywords, "mark",           (void *)MARK);
    enter_table(keywords, "signal",         (void *)SIGNAL);
    enter_table(keywords, "sig",            (void *)SIGNAL);
    enter_table(keywords, "broadcast",      (void *)BROADCAST);
    enter_table(keywords, "br",             (void *)BROADCAST);
    enter_table(keywords, "navigate",       (void *)NAVIGATE);
    enter_table(keywords, "nav",            (void *)NAVIGATE);
    enter_table(keywords, "dump",           (void *)DUMP);

    enter_table(sub_keys, "profile",        (void *)S_PROFILE);
    enter_table(sub_keys, "fast_asts",      (void *)S_FAST_ASTS);
    enter_table(sub_keys, "acs_override",   (void *)S_ACS_OVERRIDE);
    enter_table(sub_keys, "acs_overide",    (void *)S_ACS_OVERRIDE);
    enter_table(sub_keys, "janitor",        (void *)S_JANITOR);
    enter_table(sub_keys, "alt_edb",        (void *)S_ALT_EVENT_DB);
    enter_table(sub_keys, "altedb",         (void *)S_ALT_EVENT_DB);
    enter_table(sub_keys, "events",         (void *)S_ALT_EVENT_DB);
    enter_table(sub_keys, "force_back",     (void *)S_FORCE_BACK);
    enter_table(sub_keys, "force_ckpt",     (void *)S_FORCE_CKPT);
    enter_table(sub_keys, "randomize",      (void *)S_RANDOMIZE_CH);
    enter_table(sub_keys, "randomizeCH",    (void *)S_RANDOMIZE_CH);
    enter_table(sub_keys, "randomizech",    (void *)S_RANDOMIZE_CH);
    enter_table(sub_keys, "mem_sweep",      (void *)S_MEM_SWEEP);
    enter_table(sub_keys, "mem_full_abort", (void *)S_MEM_FULL_ABORT);
    enter_table(sub_keys, "inhibit_back",   (void *)S_INHIBIT_BACK);
    enter_table(sub_keys, "node_verification", (void *)S_NODE_VERIF);
    enter_table(sub_keys, "node_verif",     (void *)S_NODE_VERIF);
    enter_table(sub_keys, "node_ver",       (void *)S_NODE_VERIF);
    enter_table(sub_keys, "superchunk",     (void *)S_SUPERCHUNK);
    enter_table(sub_keys, "superchunks",    (void *)S_SUPERCHUNK);
    enter_table(sub_keys, "superc",         (void *)S_SUPERCHUNK);
    enter_table(sub_keys, "compression",    (void *)S_COMPRESSION_LEVEL);
    enter_table(sub_keys, "adv_ping",       (void *)S_ADV_PING);
    enter_table(sub_keys, "dhe_state",      (void *)S_DHE_STATE);


    enter_table(sub_keys, "normal",         (void *)S_NORMAL);
    enter_table(sub_keys, "logical",        (void *)S_LOGICAL);
    enter_table(sub_keys, "allon",          (void *)S_ALL_ON);
    enter_table(sub_keys, "alloff",         (void *)S_ALL_OFF);
    enter_table(sub_keys, "buffer",         (void *)S_BUFFER);
    enter_table(sub_keys, "off",            (void *)S_OFF);
    enter_table(sub_keys, "on",             (void *)S_ON);
    enter_table(sub_keys, "force",          (void *)S_FORCE);
    enter_table(sub_keys, "auto",           (void *)S_AUTO);
    enter_table(sub_keys, "inhibit",        (void *)S_INHIBIT);
    enter_table(sub_keys, "strobe_off",     (void *)S_STROBE_OFF);
    enter_table(sub_keys, "strobe_on",      (void *)S_STROBE_ON);
    enter_table(sub_keys, "strobe_normal",  (void *)S_STROBE_NORMAL);
    enter_table(sub_keys, "strobe_auto",    (void *)S_STROBE_AUTO);
    enter_table(sub_keys, "dyingDir",       (void *)S_DYING_DIR);
    enter_table(sub_keys, "dyingReplica",   (void *)S_DYING_REP);
    enter_table(sub_keys, "dead",           (void *)S_DEAD); 
    enter_table(sub_keys, "newReplica",     (void *)S_NEW_REP);
    enter_table(sub_keys, "newDir",         (void *)S_NEW_DIR);

    enter_table(sub_keys, "cs",             (void *)S_CRE_SERVER);
    enter_table(sub_keys, "ds",             (void *)S_DIS_SERVER);
    enter_table(sub_keys, "cc",             (void *)S_CRE_CLE);
    enter_table(sub_keys, "in",             (void *)S_INIT);
    enter_table(sub_keys, "rc",             (void *)S_REM_CLE);
    enter_table(sub_keys, "fc",             (void *)S_CLE_CLE);
    enter_table(sub_keys, "xs",             (void *)S_DEL_SERVER);
    enter_table(sub_keys, "aca",            (void *)S_ADD_CELL_ALIAS);
    enter_table(sub_keys, "rca",            (void *)S_REM_CELL_ALIAS);
    enter_table(sub_keys, "preferred",      (void *)S_PREFERRED);

    enter_table(sub_keys, "perm",           (void *)S_PERM);
    enter_table(sub_keys, "permanent",      (void *)S_PERM);
    enter_table(sub_keys, "temp",           (void *)S_TEMP);
    enter_table(sub_keys, "temporary",      (void *)S_TEMP);
    enter_table(sub_keys, "unk",            (void *)S_UNK);
    enter_table(sub_keys, "unknown",        (void *)S_UNK);
    enter_table(sub_keys, "ign",            (void *)S_IGN);
    enter_table(sub_keys, "ignore",         (void *)S_IGN);
    enter_table(sub_keys, "all",            (void *)S_ALL);
    enter_table(sub_keys, "history",        (void *)S_HISTORY);
    enter_table(sub_keys, "audit",          (void *)S_AUDIT);
    enter_table(sub_keys, "v1",             (void *)S_V1_DEF_FILE);
    enter_table(sub_keys, "invargbuffer",   (void *)S_INV_ARG_BUFFER);
    enter_table(sub_keys, "iab",            (void *)S_INV_ARG_BUFFER);

}


/* ------------------------------------------------------------------------- */

/*
 * ROUTINE - load_structure_id_table
 *
 * This routine initializes the keywords table, a hashed table maintained
 * by the utl_table_mgr module.  We don't have any user-defined data
 * associated with the keywords, so we just pass a NULL pointer to the
 * table manager.
 */

static void 
load_structure_id_table (void)
{
    if (structure_ids != NULL) {
        return;
    }

    structure_ids = declare_table(101);

    enter_table(structure_ids, "atp",        (void *)ATP_STRUCT);
    enter_table(structure_ids, "trq",        (void *)TRQ_STRUCT);
    enter_table(structure_ids, "ttq",        (void *)TTQ_STRUCT);
    enter_table(structure_ids, "ncb",        (void *)NCB_STRUCT);
    enter_table(structure_ids, "appl",       (void *)NCB_STRUCT);
    enter_table(structure_ids, "tcb",        (void *)TCB_STRUCT);
    enter_table(structure_ids, "mutex",      (void *)MUTEX_STRUCT);
    enter_table(structure_ids, "cv",         (void *)CV_STRUCT);
    enter_table(structure_ids, "thlog",      (void *)THLOG_STRUCT);
    enter_table(structure_ids, "thstats",    (void *)TSTAT_STRUCT);
    enter_table(structure_ids, "aob",        (void *)AOB_STRUCT);
    enter_table(structure_ids, "aoq",        (void *)AOQ_STRUCT);
    enter_table(structure_ids, "aostats",    (void *)AOS_STRUCT);
    enter_table(structure_ids, "vmstats",    (void *)VMSTAT_STRUCT);
    enter_table(structure_ids, "fip",        (void *)FIP_STRUCT);
    enter_table(structure_ids, "fib",        (void *)FIB_STRUCT);
    enter_table(structure_ids, "dhe",        (void *)DHE_STRUCT);
    enter_table(structure_ids, "nsb",        (void *)NSB_STRUCT);
    enter_table(structure_ids, "chb",        (void *)CHB_STRUCT);
    enter_table(structure_ids, "dht",        (void *)DHT_STRUCT);
    enter_table(structure_ids, "stm",        (void *)STM_STRUCT);
    enter_table(structure_ids, "mtrace",     (void *)MEM_STRUCT);
    enter_table(structure_ids, "mem_trace",  (void *)MEM_STRUCT);
    enter_table(structure_ids, "adv",        (void *)ADV_STRUCT);
    enter_table(structure_ids, "port",       (void *)PORT_STRUCT);
    enter_table(structure_ids, "rqst",       (void *)RQST_STRUCT);
    enter_table(structure_ids, "rpool",      (void *)RPOOL_STRUCT);

}


/* ------------------------------------------------------------------------- */

/*
 * ROUTINE - print_response
 *
 * 
 */
static void 
print_response (sys_mailbox channel)
{
    int status;
    int bytes_received;

    while (1) {
        bytes_received = sizeof(response_buffer);
        status = sys_mbx_read(channel, (unsigned char *) &response_buffer, &bytes_received);
        if (status != DNS_SUCCESS) {
            printf("Diag mbx read error: 0x%x\n", status);
            break;
        }
        if (response_buffer.response_code == end_of_responses) {
            break;
        } else if (response_buffer.response_code == normal_response) {
            printf("%.132s\n", response_buffer.response_text);
        } else if (response_buffer.response_code == error_response) {
            printf("Diagnostics signalled error: 0x%x\n", 
                   response_buffer.response_error);
        } else {
            printf("Diagnostic protocol error.  (response code %d)\n",
                   response_buffer.response_code);
        }
    }
    
}



/* -------------------------------------------------------------------------- */

/*
 * switch_token
 *
 * This routine processes generic switch tokens and checks them against
 * a legal set for the command.
 *
 * Returns:
 *     TRUE -  Token was a acceptable value; associated value is written
 *             to arg_p.
 *     FALSE - Token was not acceptable for this command.
 */
static int 
switch_token (unsigned int  switch_mask,
              int           *arg_p)
{
    enum sub_key_codes sub_key_id;

    if (lookup_table(sub_keys, current_token_value, (void **)&sub_key_id) == FALSE) {
        return (FALSE);
    }

    switch (sub_key_id) {
    case S_OFF:
        if (switch_mask & M_OFF) {
            *arg_p = (int)diag_off;
            return (TRUE);
        }
        break;
    case S_ON:
        if (switch_mask & M_ON) {
            *arg_p = (int)diag_on;
            return (TRUE);
        }
        break;
    case S_INHIBIT:
        if (switch_mask & M_INHIBIT) {
            *arg_p = (int)diag_inhibit;
            return (TRUE);
        }
        break;
    case S_NORMAL:
        if (switch_mask & M_NORMAL) {
            *arg_p = (int)diag_normal;
            return (TRUE);
        }
        break;
    case S_AUTO:
        if (switch_mask & M_AUTO) {
            *arg_p = (int)diag_auto;
            return (TRUE);
        }
        break;
    case S_FORCE:
        if (switch_mask & M_FORCE) {
            *arg_p = (int)diag_force;
            return (TRUE);
        }
        break;
    case S_STROBE_OFF: 
        if (switch_mask & M_STROBE_OFF) {
            *arg_p = (int)diag_strobe_off;
            return (TRUE);
        }
        break;
    case S_STROBE_ON: 
        if (switch_mask & M_STROBE_ON) {
            *arg_p = (int)diag_strobe_on;
            return (TRUE);
        }
        break;
    case S_STROBE_NORMAL: 
        if (switch_mask & M_STROBE_NORMAL) {
            *arg_p = (int)diag_strobe_normal;
            return (TRUE);
        }
        break;
    case S_STROBE_AUTO: 
        if (switch_mask & M_STROBE_AUTO) {
            *arg_p = (int)diag_strobe_auto;
            return (TRUE);
        }
        break;
    default: 
	break;
    }
    
    return (FALSE);
}
    

/* ------------------------------------------------------------------------- */

/*
 * ROUTINE - help_command
 *
 * 
 */

static enum parser_states 
help_command (void)
{

    next_token();

    if (current_token != END_OF_LINE) {
        sprintf(error_hint, "Expecting end-of-line, not '%s'",
                current_token_value);
        return (PARSE_ERROR);
    }

    printf("\nCommands:\n");
    printf("  examine, summarize, show, enable, disable, signal, navigate\n");
    printf("  broadcast, load, set, flush, close, manage, mark, dump, exit\n");
    printf("\nEnable keywords:\n");
    printf("  event, fast_asts, acs_override, emaa_logging, message_logging\n");
    printf("  janitor, randomizeCH, mem_sweep, mem_full_abort, superchunks\n");
    printf("  node_verification\n");
    printf("\nSet keywords:\n");
    printf("  events {normal, allon, alloff, buffer <n>}    (was: alt_edb)\n");
    printf("  force_back <fib address> {on, off}\n");
    printf("  force_ckpt <fib address> {on, off}\n");
    printf("  inhibit_back <fib address> {on, off}\n");
    printf("  compression <fib address> <level>\n");
    printf("  adv_ping <adv address> {inhibit, normal, strobe_normal, force}\n");
    printf("  dhe_state <dhe address> {on,dead,newDir,newReplica,dyingDir,dyingReplica}\n");
    printf("\nExamine structure codes:\n");
    printf("  atp     - Active Thread Pool   | trq     - Thread Run Queue\n");
    printf("  ttq     - Thread Timer Queue   | tcb     - Thread Control Block\n");
    printf("  mutex   - Mutex                | cv      - Condition Variable\n");
    printf("  thlog   - Threads Log          | thstats - Thread Statistics\n");
    printf("  aob     - Async. Op. Block     | aoq     - Async. Op.Queue\n");
    printf("  aostats - Async. Op. Stats     | ncb     - Network Control Block\n");
    printf("  vmstats - VM Statistics        | fip     - File Info Pool\n");
    printf("  fib     - File Info. Block     | dhe     - Dir. Hash Entry\n");
    printf("  nsb     - Name Server Block    | chb     - Clearinghouse Block\n");
    printf("  dht     - Dir. Hash Table      | stm     - Database Stream\n");
    printf("  mtrace  - Alloc. Certificate   | adv     - Adv. Pinger Block\n");
    printf("  port    - Network Port         | rqst    - Request Block\n");
    printf("  rpool   - Request Pool         |\n");
    printf("\nManage (mgmt) commands:\n");
    printf(" ds      - disable server\n");
    printf("  fc <ch> - forget (clear) CH    | rc <ch> - remove CH\n");
    printf("  cc <ch> <dir> - create CH\n");
    printf("  aca|rca <fib address> <fn> <p> - add|remove cell alias\n");
    printf("\n");
    return (S1);
}


/* ------------------------------------------------------------------------- */

/*
 * ROUTINE - examine_command
 *
 * 
 */

static enum parser_states 
examine_command (int detail_level)
{
    int structure_id;
    void *structure_address;
    int status;

    next_token();

    if (current_token != IDENTIFIER) {
        sprintf(error_hint, "Expecting identifier. '%s' isn't",
                current_token_value);
        return (PARSE_ERROR);
    }
    
    if (lookup_table(structure_ids, current_token_value, (void **)&structure_id) == FALSE) {
        sprintf(error_hint, "Expecting structure id. '%s' isn't one",
                current_token_value);
        return (PARSE_ERROR);
    }

    next_token();

    if (current_token == END_OF_LINE) {
        structure_address = NULL;
    } else {
        if (current_token != CONSTANT) {
            sprintf(error_hint, "Expecting address constant. '%s' isn't one",
                    current_token_value);
            return (PARSE_ERROR);
        }
        structure_address = (void *) STRING_TO_ADDRESS(current_token_value);
        next_token();
        if (current_token != END_OF_LINE) {
            sprintf(error_hint, "Expecting end-of-line, not '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        }
    }

    request.opcode = examine_op;
    request.subcode = structure_id;
    request.args.type_1.arg1 = detail_level;
    request.args.type_1.arg2 = structure_address;

    ISSUE_REQUEST(request);

    return (S1);
}




/* ------------------------------------------------------------------------- */

/*
 * ROUTINE - enable_command
 *
 * 
 */

static enum parser_states 
enable_command (void)
{
    enum sub_key_codes sub_key_id;
    void *structure_address;
    int status;

    next_token();

    if (current_token != IDENTIFIER) {
        sprintf(error_hint, "Expecting identifier. '%s' isn't",
                current_token_value);
        return (PARSE_ERROR);
    }
    
    if (lookup_table(sub_keys, current_token_value, (void **)&sub_key_id) == FALSE) {
        sprintf(error_hint, "Enable a what?  (%s!)",
                current_token_value);
        return (PARSE_ERROR);
    }

   if (sub_key_id == S_FAST_ASTS) {
        next_token();

        if (current_token != END_OF_LINE) {
            sprintf(error_hint, "Expecting end-of-line, not '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        }

        request.subcode = FAST_ASTS_SUBCODE;

    } else if (sub_key_id == S_ACS_OVERRIDE) {
        next_token();

        if (current_token != END_OF_LINE) {
            sprintf(error_hint, "Expecting end-of-line, not '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        }

        request.subcode = ACS_OVERRIDE_SUBCODE;

    } else if (sub_key_id == S_JANITOR) {
        next_token();

        if (current_token != END_OF_LINE) {
            sprintf(error_hint, "Expecting end-of-line, not '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        }

        request.subcode = JANITOR_INHIBIT_SUBCODE;

    } else if (sub_key_id == S_RANDOMIZE_CH) {
        next_token();

        if (current_token != END_OF_LINE) {
            sprintf(error_hint, "Expecting end-of-line, not '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        }

        request.subcode = RANDOMIZE_CH_SUBCODE;

    } else if (sub_key_id == S_MEM_SWEEP) {
        next_token();

        if (current_token != END_OF_LINE) {
            sprintf(error_hint, "Expecting end-of-line, not '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        }

        request.subcode = MEM_SWEEP_SUBCODE;

    } else if (sub_key_id == S_MEM_FULL_ABORT) {
        next_token();

        if (current_token != END_OF_LINE) {
            sprintf(error_hint, "Expecting end-of-line, not '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        }

        request.subcode = MEM_FULL_ABORT_SUBCODE;

    } else if (sub_key_id == S_NODE_VERIF) {
        next_token();

        if (current_token != END_OF_LINE) {
            sprintf(error_hint, "Expecting end-of-line, not '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        }

        request.subcode = NODE_VERIF_SUBCODE;

    } else if (sub_key_id == S_SUPERCHUNK) {
        next_token();

        if (current_token != END_OF_LINE) {
            sprintf(error_hint, "Expecting end-of-line, not '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        }

        request.subcode = SUPERCHUNK_SUBCODE;

    } else {
        sprintf(error_hint, "Enable a what?  (%s!)",
                current_token_value);
        return (PARSE_ERROR);
    }

    request.opcode = enable_op;

    if (request.subcode == EVENT_SUBCODE) {
        if (strncmp(request.args.type_2.arg1, "<", 1) == 0) {
            FILE *event_file;
            char *file_name = request.args.type_2.arg1;
          
            file_name++;
            event_file = fopen(file_name, "r");
            if (event_file == NULL) {
                sprintf(error_hint, "Can't open file (%s)", file_name);
                return (PARSE_ERROR);
            }
            while (fscanf(event_file, "%s", request.args.type_2.arg1) != EOF) {
                if (request.args.type_2.arg1[0] != '#') {
                    ISSUE_REQUEST(request);
                }
            }
            fclose(event_file);
        } else {
            ISSUE_REQUEST(request);
        }
    } else {
        ISSUE_REQUEST(request);
    }

    return (S1);
}



/* ------------------------------------------------------------------------- */

/*
 * ROUTINE - disable_command
 *
 * 
 */

static enum parser_states 
disable_command (void)
{
    enum sub_key_codes sub_key_id;
    void *structure_address;
    int status;

    next_token();

    if (current_token != IDENTIFIER) {
        sprintf(error_hint, "Expecting identifier. '%s' isn't",
                current_token_value);
        return (PARSE_ERROR);
    }
    
    if (lookup_table(sub_keys, current_token_value, (void **)&sub_key_id) == FALSE) {
        sprintf(error_hint, "Enable a what?  (%s!)",
                current_token_value);
        return (PARSE_ERROR);
    }

    if (sub_key_id == S_FAST_ASTS) {
        next_token();

        if (current_token != END_OF_LINE) {
            sprintf(error_hint, "Expecting end-of-line, not '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        }

        request.subcode = FAST_ASTS_SUBCODE;

    } else if (sub_key_id == S_ACS_OVERRIDE) {
        next_token();

        if (current_token != END_OF_LINE) {
            sprintf(error_hint, "Expecting end-of-line, not '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        }

        request.subcode = ACS_OVERRIDE_SUBCODE;

    } else if (sub_key_id == S_JANITOR) {
        next_token();

        if (current_token != END_OF_LINE) {
            sprintf(error_hint, "Expecting end-of-line, not '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        }

        request.subcode = JANITOR_INHIBIT_SUBCODE;

    } else if (sub_key_id == S_RANDOMIZE_CH) {
        next_token();

        if (current_token != END_OF_LINE) {
            sprintf(error_hint, "Expecting end-of-line, not '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        }

        request.subcode = RANDOMIZE_CH_SUBCODE;

    } else if (sub_key_id == S_MEM_SWEEP) {
        next_token();

        if (current_token != END_OF_LINE) {
            sprintf(error_hint, "Expecting end-of-line, not '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        }

        request.subcode = MEM_SWEEP_SUBCODE;

    } else if (sub_key_id == S_MEM_FULL_ABORT) {
        next_token();

        if (current_token != END_OF_LINE) {
            sprintf(error_hint, "Expecting end-of-line, not '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        }

        request.subcode = MEM_FULL_ABORT_SUBCODE;

    } else if (sub_key_id == S_NODE_VERIF) {
        next_token();

        if (current_token != END_OF_LINE) {
            sprintf(error_hint, "Expecting end-of-line, not '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        }

        request.subcode = NODE_VERIF_SUBCODE;

    } else if (sub_key_id == S_SUPERCHUNK) {
        next_token();

        if (current_token != END_OF_LINE) {
            sprintf(error_hint, "Expecting end-of-line, not '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        }

        request.subcode = SUPERCHUNK_SUBCODE;

    } else {
        sprintf(error_hint, "Disable a what?  (%s!)",
                current_token_value);
        return (PARSE_ERROR);
    }

    request.opcode = disable_op;

    if (request.subcode == EVENT_SUBCODE) {
        if (strncmp(request.args.type_2.arg1, "<", 1) == 0) {
            FILE *event_file;
            char *file_name = request.args.type_2.arg1;
          
            file_name++;
            event_file = fopen(file_name, "r");
            if (event_file == NULL) {
                sprintf(error_hint, "Can't open file (%s)", file_name);
                return (PARSE_ERROR);
            }
            while (fscanf(event_file, "%s", request.args.type_2.arg1) != EOF) {
                if (request.args.type_2.arg1[0] != '#') {
                    ISSUE_REQUEST(request);
                }
            }
            fclose(event_file);
        } else {
            ISSUE_REQUEST(request);
        }
    } else {
        ISSUE_REQUEST(request);
    }

    return (S1);
}



/* ------------------------------------------------------------------------- */

/*
 * ROUTINE - flush_command
 *
 * 
 */
static enum parser_states 
flush_command (void)
{

    int status;
    int bytes_read;

    printf("Flushing diagnostic output (abort when done)");

    while (1) {
        status = sys_mbx_read(diag_channel, (unsigned char *)&response_buffer, &bytes_read);
        if (status != DNS_SUCCESS) {
            printf("Diag mbx read error: 0x%x", status);
            break;
        }
        printf(".");
    }

    return (S1);
}



/* ------------------------------------------------------------------------- */

/*
 * ROUTINE - close_command
 *
 * 
 */

static enum parser_states 
close_command (void)
{

    enum sub_key_codes sub_key_id;
    int status;

    next_token();

    if (current_token != IDENTIFIER) {
        sprintf(error_hint, "Expecting identifier. '%s' isn't",
                current_token_value);
        return (PARSE_ERROR);
    }
    
    if (lookup_table(sub_keys, current_token_value, (void **)&sub_key_id) == FALSE) {
        sprintf(error_hint, "Close a what?  (%s!)",
                current_token_value);
        return (PARSE_ERROR);
    }

    sprintf(error_hint, "Close a what?  (%s!)",
	    current_token_value);
    return (PARSE_ERROR);

    request.opcode = close_op;

    ISSUE_REQUEST(request);

    return (S1);
}



/* ------------------------------------------------------------------------- */

/*
 * ROUTINE - set_command
 *
 * 
 */

static enum parser_states 
set_command (void)
{

    enum sub_key_codes sub_key_id;
    void *structure_address;
    int status;

    next_token();

    if (current_token != IDENTIFIER) {
        sprintf(error_hint, "Expecting identifier. '%s' isn't",
                current_token_value);
        return (PARSE_ERROR);
    }
    
    if (lookup_table(sub_keys, current_token_value, (void **)&sub_key_id) == FALSE) {
        sprintf(error_hint, "Set what?  (%s!)",
                current_token_value);
        return (PARSE_ERROR);
    }

    if (sub_key_id == S_ALT_EVENT_DB) {
        next_token();
        if (current_token != IDENTIFIER) {
            sprintf(error_hint, "Expecting identifier. '%s' isn't",
                    current_token_value);
            return (PARSE_ERROR);
        }
        if (lookup_table(sub_keys, current_token_value, (void **)&sub_key_id) == FALSE) {
            sprintf(error_hint, "Set events to what?  (%s!)",
                    current_token_value);
            return (PARSE_ERROR);
        }
        switch (sub_key_id) {
        case S_NORMAL:
            request.args.type_1.arg1 = 0; break;
        case S_LOGICAL:
            request.args.type_1.arg1 = 1; break;
        case S_ALL_ON:
            request.args.type_1.arg1 = 2; break;
        case S_ALL_OFF:
            request.args.type_1.arg1 = 3; break;
        case S_BUFFER:
            request.args.type_1.arg1 = 4;
            next_token();
            if (current_token != CONSTANT) {
                sprintf(error_hint, "Expecting buffer size. '%s' isn't one",
                        current_token_value);
                return (PARSE_ERROR);
            }
            request.args.type_1.arg3 = atoi(current_token_value);
            break;
        default:
            sprintf(error_hint, "Set events to what?  (%s!)",
                    current_token_value);
            return (PARSE_ERROR);
        }
        next_token();
        if (current_token != END_OF_LINE) {
            sprintf(error_hint, "Expecting end-of-line, not '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        }
        request.subcode = ALT_EVENT_DB_SUBCODE;

    } else if ((sub_key_id == S_FORCE_CKPT) ||
               (sub_key_id == S_FORCE_BACK) ||
               (sub_key_id == S_INHIBIT_BACK)) {

        switch (sub_key_id) {
        case S_FORCE_CKPT:   request.subcode = FORCE_CKPT_SUBCODE;   break;
        case S_FORCE_BACK:   request.subcode = FORCE_BACK_SUBCODE;   break;
        case S_INHIBIT_BACK: request.subcode = INHIBIT_BACK_SUBCODE; break;
	default: break;
        }

        next_token();
        if (current_token != CONSTANT) {
            sprintf(error_hint, "Expecting file_info address. '%s' isn't one",
                    current_token_value);
            return (PARSE_ERROR);
        }
        structure_address = (void *) STRING_TO_ADDRESS(current_token_value);
        request.args.type_1.arg2 = structure_address;

        next_token();
        if (current_token != IDENTIFIER) {
            sprintf(error_hint, "Expecting identifier. '%s' isn't",
                    current_token_value);
            return (PARSE_ERROR);
        }

        if (lookup_table(sub_keys, current_token_value, (void **)&sub_key_id) == FALSE) {
            sprintf(error_hint, "Set it to what?  (%s!)",
                   current_token_value);
            return (PARSE_ERROR);
        }


        if (sub_key_id == S_ON) {
            request.args.type_1.arg1 = 1;
        } else if (sub_key_id == S_OFF) {
            request.args.type_1.arg1 = 0;
        } else {
            sprintf(error_hint, "Take your pick: on or off",
                   current_token_value);
            return (PARSE_ERROR);
        }

        next_token();
        if (current_token != END_OF_LINE) {
            sprintf(error_hint, "Expecting end-of-line, not '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        }

    } else if (sub_key_id == S_COMPRESSION_LEVEL) {
        next_token();
        if (current_token != CONSTANT) {
            sprintf(error_hint, "Expecting file_info address. '%s' isn't one",
                    current_token_value);
            return (PARSE_ERROR);
        }
        structure_address = (void *) STRING_TO_ADDRESS(current_token_value);
        request.args.type_1.arg2 = structure_address;

        next_token();
        if (current_token != CONSTANT) {
            sprintf(error_hint, "Expecting compression level. '%s' isn't one",
                    current_token_value);
            return (PARSE_ERROR);
        }
        request.args.type_1.arg1 = atoi(current_token_value);

        next_token();
        if (current_token != END_OF_LINE) {
            sprintf(error_hint, "Expecting end-of-line, not '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        }

        request.subcode = COMPRESSION_SUBCODE;

    } else if (sub_key_id == S_ADV_PING) {
        next_token();
        if (current_token != CONSTANT) {
            sprintf(error_hint, "Expecting nsb address. '%s' isn't one",
                    current_token_value);
            return (PARSE_ERROR);
        }
        structure_address = (void *) STRING_TO_ADDRESS(current_token_value);
        request.args.type_1.arg2 = structure_address;

        next_token();
        if (current_token != IDENTIFIER) {
            sprintf(error_hint, "Expecting identifier. '%s' isn't",
                    current_token_value);
            return (PARSE_ERROR);
        }
        if (switch_token((M_INHIBIT | M_NORMAL | M_STROBE_NORMAL | M_FORCE),
                         &request.args.type_1.arg1) == FALSE) {
            sprintf(error_hint, "Set it to what?  (%s!)",
                   current_token_value);
            return (PARSE_ERROR);
        }

        next_token();
        if (current_token != END_OF_LINE) {
            sprintf(error_hint, "Expecting end-of-line, not '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        }

        request.subcode = ADV_PING_SUBCODE;

    } else if (sub_key_id == S_DHE_STATE) {
        next_token();
        if (current_token != CONSTANT) {
            sprintf(error_hint, "Expecting dhe address. '%s' isn't one",
                    current_token_value);
            return (PARSE_ERROR);
        }
        structure_address = (void *) STRING_TO_ADDRESS(current_token_value);
        request.args.type_1.arg2 = structure_address;

        next_token();
        if (current_token != IDENTIFIER) {
            sprintf(error_hint, "Expecting identifier. '%s' isn't",
                    current_token_value);
            return (PARSE_ERROR);
        }
        if (lookup_table(sub_keys, current_token_value, (void **)&sub_key_id) == FALSE) {
            sprintf(error_hint, "Set dhe_state to what?  (%s!)",
                    current_token_value);
            return (PARSE_ERROR);
        }
        switch (sub_key_id) {
        case S_ON:
            request.args.type_1.arg1 = RS_on; break;
        case S_NEW_DIR:
            request.args.type_1.arg1 = RS_newDir; break;
        case S_NEW_REP:
            request.args.type_1.arg1 = RS_newReplica; break;
        case S_DYING_DIR:
            request.args.type_1.arg1 = RS_dyingDir; break;
        case S_DYING_REP:
            request.args.type_1.arg1 = RS_dyingReplica; break;
        case S_DEAD:
            request.args.type_1.arg1 = RS_dead; break;
        default:
            sprintf(error_hint, "Set dhe_state to what?  (%s!)",
                    current_token_value);
            return (PARSE_ERROR);
        }
        next_token();
        if (current_token != END_OF_LINE) {
            sprintf(error_hint, "Expecting end-of-line, not '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        }

        request.subcode = DHE_STATE_SUBCODE;

    } else {
        sprintf(error_hint, "Set what?  (%s!)",
                current_token_value);
        return (PARSE_ERROR);
    }

    request.opcode = set_op;

    ISSUE_REQUEST(request);

    return (S1);
}



/* ------------------------------------------------------------------------- */

/*
 * ROUTINE - mgmt_command
 *
 * 
 */

static enum parser_states 
mgmt_command (void)
{

    enum sub_key_codes sub_key_id;
    enum sub_key_codes pref_id;
    int status;
    char namespace_name[80];
    char ch_name[80];
    char owner_name[80];
    char dir_name[80];
    int x;
    

    next_token();

    if (current_token != IDENTIFIER) {
        sprintf(error_hint, "Expecting identifier. '%s' isn't",
                current_token_value);
        return (PARSE_ERROR);
    }
    
    if (lookup_table(sub_keys, current_token_value, (void **)&sub_key_id) == FALSE) {
        sprintf(error_hint, "Invalid management subcommand: %s",
                current_token_value);
        return (PARSE_ERROR);
    }


    switch (sub_key_id) {
    case S_DIS_SERVER:
        next_token();
        if (current_token != END_OF_LINE) {
            sprintf(error_hint, "Expecting end-of-line, not '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        }
        request.subcode = DISABLE_SERVER_SUBCODE;
        break;
    case S_CRE_CLE:
        for (x = 0; x < 2; x++) {
            next_token();
            if (current_token == END_OF_LINE) {
                sprintf(error_hint, "Expecting create clearinghouse parameter, not '%s'",
                        current_token_value);
                return (PARSE_ERROR);
            }
            switch (x) {
            case 0: strcpy(ch_name, current_token_value); break;
            case 1: strcpy(dir_name, current_token_value); break;
            }
        }
        next_token();
        if (current_token != END_OF_LINE) {
            sprintf(error_hint, "Expecting end-of-line, not '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        }
        strcpy(request.args.type_3.arg1, ch_name);
        strcpy(request.args.type_3.arg2, dir_name);
        request.subcode = CREATE_CLE_SUBCODE;
        break;
    case S_REM_CLE:
        next_token();
        if (current_token == END_OF_LINE) {
            sprintf(error_hint, "Expecting clearinghouse name, not '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        }
        strcpy(ch_name, current_token_value);
        next_token();
        if (current_token != END_OF_LINE) {
            sprintf(error_hint, "Expecting end-of-line, not '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        }
        strcpy(request.args.type_3.arg1, ch_name);
        request.subcode = REMOVE_CLE_SUBCODE;
        break;
    case S_CLE_CLE:
        next_token();
        if (current_token == END_OF_LINE) {
            sprintf(error_hint, "Expecting clearinghouse name, not '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        }
        strcpy(ch_name, current_token_value);
        next_token();
        if (current_token != END_OF_LINE) {
            sprintf(error_hint, "Expecting end-of-line, not '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        }
        strcpy(request.args.type_3.arg1, ch_name);
        request.subcode = CLEAR_CLE_SUBCODE;
        break;
    case S_INIT:
        for (x = 0; x < 3; x++) {
            next_token();
            if (current_token == END_OF_LINE) {
                sprintf(error_hint, "Expecting initialize parameter, not '%s'",
                        current_token_value);
                return (PARSE_ERROR);
            }
            switch (x) {
            case 0: strcpy(namespace_name, current_token_value); break;
            case 1: strcpy(ch_name, current_token_value); break;
            case 2: strcpy(owner_name, current_token_value); break;
            }
        }
        next_token();
        if (current_token != END_OF_LINE) {
            sprintf(error_hint, "Expecting end-of-line, not '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        }
        if (strlen(namespace_name) > 50) {
            sprintf(error_hint, "Namespace name must be < 50 characters",
                    current_token_value);
            return (PARSE_ERROR);
        }
        if (strlen(ch_name) > 50) {
            sprintf(error_hint, "Clearinghouse name must be < 50 characters",
                    current_token_value);
            return (PARSE_ERROR);
        }
        strcpy(request.args.type_3.arg1, namespace_name);
        strcpy(request.args.type_3.arg2, ch_name);
        strcpy(request.args.type_3.arg3, owner_name);
        request.subcode = INIT_NS_SUBCODE;
        break;
    case S_ADD_CELL_ALIAS:
    case S_REM_CELL_ALIAS:
        next_token();

        if (current_token == END_OF_LINE) {
            sprintf(error_hint, "Expecting cell name, not '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        } 
        if (current_token != CONSTANT) {
            sprintf(error_hint, "Expecting fib address. '%s' isn't one",
                    current_token_value);
            return (PARSE_ERROR);
        }
        request.args.type_1.arg2 = (void *) STRING_TO_ADDRESS(current_token_value);
        next_token();
        if (current_token == END_OF_LINE) {
            sprintf(error_hint, "Expecting cell name, not '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        }
        strcpy(namespace_name, current_token_value);
        next_token();
        if (lookup_table(sub_keys, current_token_value, &pref_id) == FALSE) {
            sprintf(error_hint, "Invalid cell alias type: %s",
                    current_token_value);
            return (PARSE_ERROR);
        }
        if ((pref_id != S_NORMAL) && (pref_id != S_PREFERRED)) {
            sprintf(error_hint, "Invalid cell alias type: '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        }
        next_token();
        if (current_token != END_OF_LINE) {
            sprintf(error_hint, "Expecting end-of-line, not '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        }
        strcpy(request.args.type_3.arg2, namespace_name);
        if (pref_id == S_NORMAL) {
            request.args.type_1.arg1 = 0;    /* Normal */
        } else {
            request.args.type_1.arg1 = 1;    /* Preferred */
        }
        if (sub_key_id == S_ADD_CELL_ALIAS) {
            request.subcode = ADD_CELL_ALIAS_SUBCODE;
        } else {
            request.subcode = REM_CELL_ALIAS_SUBCODE;
        }
        break;

    default:
        sprintf(error_hint, "Invalid management subcommand: %s",
                current_token_value);
        return (PARSE_ERROR);
        break;
    }

    request.opcode = mgmt_op;

    ISSUE_REQUEST(request);

    return (S1);
}



/* ------------------------------------------------------------------------- */

/*
 * ROUTINE - mark_command
 */

static enum parser_states 
mark_command (void)
{

    int status;

    rest_of_line();

    if (current_token != LITERAL) {
        sprintf(error_hint, "Expecting something to say");
        return (PARSE_ERROR);
    }

    strncpy(request.args.type_2.arg1, current_token_value, 70);
    request.opcode = mark_op;

    ISSUE_REQUEST(request);

    return (S1);
}



/* ------------------------------------------------------------------------- */

/*
 * ROUTINE - signal_command
 *
 * 
 */

static enum parser_states 
signal_command (int signal_type)
{
    void *structure_address;
    int status;

    next_token();

    if (current_token == END_OF_LINE) {
        structure_address = NULL;
    } else {
        if (current_token != CONSTANT) {
            sprintf(error_hint, "Expecting address constant. '%s' isn't one",
                    current_token_value);
            return (PARSE_ERROR);
        }
        structure_address = (void *) STRING_TO_ADDRESS(current_token_value);
        next_token();
        if (current_token != END_OF_LINE) {
            sprintf(error_hint, "Expecting end-of-line, not '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        }
    }

    request.opcode = signal_op;
    request.subcode = signal_type;
    request.args.type_1.arg2 = structure_address;

    ISSUE_REQUEST(request);

    return (S1);
}



/* ------------------------------------------------------------------------- */

/*
 * ROUTINE - navigate_command
 *
 * 
 */

static enum parser_states 
navigate_command (void)
{
    void *structure_address;
    int structure_type;
    int status;


    next_token();

    if (current_token != CONSTANT) {
        sprintf(error_hint, "Expecting navigation structure code. '%s' isn't",
                current_token_value);
        return (PARSE_ERROR);
    }
    structure_type = atol(current_token_value);

    next_token();

    if (current_token == END_OF_LINE) {
        structure_address = NULL;
    } else {
        if (current_token != CONSTANT) {
            sprintf(error_hint, "Expecting address constant. '%s' isn't one",
                    current_token_value);
            return (PARSE_ERROR);
        }
        structure_address = (void *) STRING_TO_ADDRESS(current_token_value);
        next_token();
        if (current_token != END_OF_LINE) {
            sprintf(error_hint, "Expecting end-of-line, not '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        }
    }

    request.opcode = navigate_op;
    request.subcode = structure_type;
    request.args.type_1.arg2 = structure_address;

    ISSUE_REQUEST(request);

    return (S1);
}


/* ------------------------------------------------------------------------- */

/*
 * ROUTINE - dump_command
 *
 * 
 */

static enum parser_states 
dump_command (void)
{
    enum sub_key_codes sub_key_id;
    int status;


    next_token();

    if (current_token != IDENTIFIER) {
        sprintf(error_hint, "Expecting identifier. '%s' isn't",
                current_token_value);
        return (PARSE_ERROR);
    }
    
    if (lookup_table(sub_keys, current_token_value, (void **)&sub_key_id) == FALSE) {
        sprintf(error_hint, "Enable a what?  (%s!)",
                current_token_value);
        return (PARSE_ERROR);
    }

    if (sub_key_id == S_PERM) {
        next_token();

        if (current_token != END_OF_LINE) {
            sprintf(error_hint, "Expecting end-of-line, not '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        }

        request.subcode = PERM_MEM_SUBCODE;

    } else if (sub_key_id == S_TEMP) {
        next_token();

        if (current_token != END_OF_LINE) {
            sprintf(error_hint, "Expecting end-of-line, not '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        }

        request.subcode = TEMP_MEM_SUBCODE;

    } else if (sub_key_id == S_UNK) {
        next_token();

        if (current_token != END_OF_LINE) {
            sprintf(error_hint, "Expecting end-of-line, not '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        }

        request.subcode = UNK_MEM_SUBCODE;

    } else if (sub_key_id == S_IGN) {
        next_token();

        if (current_token != END_OF_LINE) {
            sprintf(error_hint, "Expecting end-of-line, not '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        }

        request.subcode = IGN_MEM_SUBCODE;

    } else if (sub_key_id == S_ALL) {
        next_token();

        if (current_token != END_OF_LINE) {
            sprintf(error_hint, "Expecting end-of-line, not '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        }

        request.subcode = ALL_MEM_SUBCODE;

    } else if (sub_key_id == S_HISTORY) {
        next_token();

        if (current_token != END_OF_LINE) {
            sprintf(error_hint, "Expecting end-of-line, not '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        }

        request.subcode = HISTORY_MEM_SUBCODE;

    } else if (sub_key_id == S_AUDIT) {
        next_token();

        if (current_token != END_OF_LINE) {
            sprintf(error_hint, "Expecting end-of-line, not '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        }

        request.subcode = AUDIT_MEM_SUBCODE;

    } else if (sub_key_id == S_V1_DEF_FILE) {
        next_token();

        if (current_token != END_OF_LINE) {
            sprintf(error_hint, "Expecting end-of-line, not '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        }

        request.subcode = V1_DEF_FILE_SUBCODE;

    } else if (sub_key_id == S_INV_ARG_BUFFER) {
        next_token();

        if (current_token != END_OF_LINE) {
            sprintf(error_hint, "Expecting end-of-line, not '%s'",
                    current_token_value);
            return (PARSE_ERROR);
        }

        request.subcode = INV_ARG_BUFFER_SUBCODE;

    } else {
        sprintf(error_hint, "Dump what?  (%s!)",
                current_token_value);
        return (PARSE_ERROR);
    }


    request.opcode = dump_op;

    ISSUE_REQUEST(request);

    return (S1);
}




/* ------------------------------------------------------------------------- */

/*
 * ROUTINE - state_1
 *
 * This routine implements state #1 of the parser.
 */

static enum parser_states 
state_1 (void)
{
    enum key_codes keyword_id;

    next_token();

    if (current_token == END_OF_FILE) {
        return DONE;
    }
    
    if (current_token == KEYWORD) {
        lookup_table(keywords, current_token_value, (void **)&keyword_id);
        switch (keyword_id) {
            case SUMMARIZE:  return (examine_command(0));
            case EXAMINE:    return (examine_command(1));
            case DISABLE:    return (disable_command());
            case ENABLE:     return (enable_command());
            case SET:        return (set_command());
            case FLUSH:      return (flush_command());
            case CLOSE:      return (close_command());
            case HELP:       return (help_command());
            case MGMT:       return (mgmt_command());
            case MARK:       return (mark_command());
            case SIGNAL:     return (signal_command(SIGNAL_SUBCODE));
            case BROADCAST:  return (signal_command(BROADCAST_SUBCODE));
            case NAVIGATE:   return (navigate_command());
            case DUMP:       return (dump_command());
            case EXIT:       exit(0);
            default:      sprintf(error_hint,"Unsupp command."); return (PARSE_ERROR); break;
        }
    } else {
        sprintf(error_hint, "Expecting keyword. '%s' isn't",
                current_token_value);
        return (PARSE_ERROR);
    }

    return (S1);
}




/* ------------------------------------------------------------------------- */

/*
 * ROUTINE - main
 *
 * This is the main loop for the program.  It is a thread so we can
 * take advantage of the thread-synchronous mailbox/domain socket facility.
 *
 * Terminal I/O is assumed to be process-synchronous, but since this is the
 * only thread, it really isn't much of a problem.
 */

int 
main (int   argc,
      char  **argv)
{
    char filename[_DNS_MAXPATHLEN+1];
    char *myname_p;
    enum parser_states parser_state;     /* Holds the current parser state */
    error_status_t st;
    int status;

    /*
     * Get our filename, stripped of directories
     */
    if (myname_p = strrchr(argv[0], '/'))
	myname_p++;
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

    if (!dce_priv_proc_is_appropriate() ) {
	fputs("Permission denied\n", stderr);
	exit(1);
    }

    dns_cfginit((char *)0, argv[0]);

    dthread_init();

    load_keyword_tables();
    load_structure_id_table();

    printf(INITIAL_MESSAGE, program_version, __DATE__);

    sprintf(filename, CDS_DIAG_SOCK, dcelocal_path);

    status = sys_openmbx((unsigned char *) filename,
                         DIAG_MAX_MESSAGE_SIZE,
                         NULL, 
			 1,	/* set block mode */
                         dthread_attr_default,
                         &diag_channel); 
    if (status != DNS_SUCCESS) {
        printf("Diagnostic channel open status: 0x%x\n", status);
    }


    parser_state = S1;

    while (parser_state != DONE) {
        if (parser_state == PARSE_ERROR) {
            printf("Parser error. %s\n", error_hint);
            /* Flush to end of line or end of file */
            while (1) {
                if (current_token == END_OF_LINE) {
                    parser_state = S1;
                    break;
                }
                if (current_token == END_OF_FILE) {
                    parser_state = DONE;
                    break;
                }
                next_token();
            }
        } else if (parser_state == S1) {
            printf("diag> ");
            parser_state = state_1();
        } else {
            printf("Internal parser error - unknown state (%d)\n", parser_state);
            break;
        } 
    }
    return(0);
}
