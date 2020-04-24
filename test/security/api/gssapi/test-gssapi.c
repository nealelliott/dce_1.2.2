/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: test-gssapi.c,v $
 * Revision 1.1.6.1  1996/07/09  15:03:20  arvind
 * 	Include locale.h
 * 	[1996/02/17  23:24:54  marty  1.1.4.4]
 *
 * Revision 1.1.4.4  1996/02/17  23:24:54  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:43:07  marty]
 * 
 * Revision 1.1.4.3  1996/02/07  17:48:27  parul
 * 	DCE 1.2.1 final drop from HP
 * 	[1996/02/07  16:14:11  parul]
 * 
 * 	DCE 1.2.1 final drop from HP
 * 
 * Revision 1.1.2.9  1994/10/05  21:47:44  wray
 * 	CR 12513
 * 	[1994/10/05  21:45:09  wray]
 * 
 * Revision 1.1.2.8  1994/09/23  19:52:22  wray
 * 	CR 11894 & 12215 - Cred management tests
 * 	[1994/09/23  19:51:03  wray]
 * 
 * Revision 1.1.2.7  1994/09/15  19:21:37  wray
 * 	CR 10895, 11894, 12047, 11540, 12190, 12200
 * 	[1994/09/15  19:19:58  wray]
 * 
 * Revision 1.1.2.6  1994/08/03  15:19:03  wray
 * 	Workaround for OT 10805
 * 	[1994/08/03  15:00:46  wray]
 * 
 * Revision 1.1.2.5  1994/07/20  15:02:10  wray
 * 	Fix submission
 * 	[1994/07/20  15:01:32  wray]
 * 
 * 	Make tet-compliant (if ENVIRONMENT_TET symbol is defined)
 * 	[1994/07/19  19:31:25  wray]
 * 
 * Revision 1.1.2.4  1994/06/28  20:01:10  wray
 * 	GSSAPI BL3 (OT 10824)
 * 	[1994/06/28  19:25:55  wray]
 * 
 * Revision 1.1.2.3  1994/06/10  20:45:23  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:16:55  devsrc]
 * 
 * Revision 1.1.2.2  1994/03/23  16:47:34  wray
 * 	GSSAPI BL2 (OT#10179)
 * 	[1994/03/23  16:46:29  wray]
 * 
 * Revision 1.1.2.1  1994/03/09  15:51:11  wray
 * 	Initial submission (OT#10019)
 * 	[1994/03/09  15:39:54  wray]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) 1993 by
 * Digital Equipment Corporation, Maynard, Mass.
 * 
 * This software is furnished under a license and may be used and copied
 * only  in  accordance  with  the  terms  of such  license and with the
 * inclusion of the above copyright notice.  This software or any  other
 * copies thereof may not be provided or otherwise made available to any
 * other person.  No title to and ownership of the  software  is  hereby
 * transferred.
 * 
 * The information in this software  is subject to change without notice
 * and  should  not be  construed  as a commitment by Digitial Equipment
 * Corporation.
 * 
 * Digital assumes no responsibility for the use or  reliablity  of its
 * software on equipment which is not supplied by Digital.
 * 
 * FACILITY: 	Generic Security Service API (GSSAPI)
 * 
 * ABSTRACT:	Test program for GSSAPI calls.  Exercises both DCE and
 *              Kerberos authentication mechanisms, tests confidentiality
 *	        and integrity services.
 * 
 *	
 * ENVIRONMENT:	portable
 * 
 * AUTHOR:
 *              John C Wray
 *		Distributed Processing Engineering (DPE)
 * 
 * DATE:	March 1993
 * 
 */

/* This program is now tet-compatible, when built with the ENVIRONMENT_TET  */
/* symbol defined.  The instructions below describe how to run it outside   */
/* the tet environment; For running it as a tet test-purpose, see the	    */
/* README file in the gssapi functional test directory.			    */
/*									    */
/* To run this program, you must have DCE credentials established, and you  */
/* must have access to a DCE keytable containing a principal key (not	    */
/* necessarily for the principal you're logged in as).			    */
/*									    */
/* The program is invoked as follows:					    */
/*									    */
/* % test-gssapi [-p <pwd>] [-l] [-i] [-t <tsts>] <cli> <svr> [<keytab>]    */
/*									    */
/* <tsts> is a list of the form "1,2,5,6", where each number enables a      */
/* single test.								    */
/* <cli> is the principal name you're logged in as.			    */
/* <svr> is the principal name for the key in the keytable.		    */
/* <keytab> is the pathname of the key file. If omitted, the default	    */
/* keytable will be used.						    */
/* <pwd> is the password of <cli>.  If the -p argument is present, the test */
/* program will log-in as <cli> prior to performing any tests.		    */
/*									    */
/* -l  Displays additional (long-form) tracing information; shows what	    */
/*     GSSAPI calls are being made.					    */
/* -i  Specifies interactive mode, where you're given a menu of tests to    */
/*     pick from.  If you've also specified the -t parameter, those tests   */
/*     will be pre-selected for you.					    */


#include <dce/gssapi.h>
#include <dce/sec_cred.h>
#include <dce/dce_msg.h>
/* #include "gssapi-int.h" */
#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <varargs.h>


#ifdef ENVIRONMENT_TET

#include <tet_api.h>

static void startup(void);
static void cleanup(void);
static void tp(void);

void (*tet_startup)() = startup;
void (*tet_cleanup)() = cleanup;

struct tet_testlist tet_testlist[] = {
{tp, 1},
{NULL, 0}
};

static char msg[256];

#define GSSAPI_TEST_ARGUMENTS "GSSAPI_TEST_ARGUMENTS"

#endif

#define DONT_CHECK_PURGE

#define TIME_ERROR 3
/* Above is maximum difference in context time allowed between		    */
/* accept_sec_context and immediate context_time call.			    */

 
static int error = 0;
static int test = 0;
static int subtest = 0;
static int long_form = 0;
static int interactive = 0;

#ifdef ENVIRONMENT_TET
#define EXPORTABLE(FLAG)						\
	if (FLAG & GSS_C_CONF_FLAG) {					\
            privacy_supported = 1;					\
	}								
#else
#define EXPORTABLE(FLAG)						\
	if (FLAG & GSS_C_CONF_FLAG) {					\
	    privacy_supported = 1;					\
	    fprintf(stderr, "\n\n\tWARNING! Non-exportable GSSAPI!\n\n");\
	    fflush(stderr);						\
	} else {							\
	    fprintf(stderr, "Exportable (non-encrypting) GSSAPI\n");	\
	    fflush(stderr);						\
	}
#endif

#ifdef ENVIRONMENT_TET                                                
#define TEST(x)                                                       \
    subtest = 1;                                                      \
    { char text[256];                                                 \
      sprintf(text, "\n%d.  %s\n", ++test, x);                      \
      tet_infoline(text);                                             \
    }
#else
#define TEST(x) subtest = 1; fprintf(stdout, "\n%d.  %s\n", ++test, x)
#endif


#ifdef ENVIRONMENT_TET                                                
#define SUBTEST(x)                                                    \
    { char text[256];                                                 \
      sprintf(text, "%d.%d:  %s\n", test, ++subtest, x);          \
      tet_infoline(text);                                             \
    }
#else
#define SUBTEST(x) fprintf(stdout, "%d.%d:  %s\n", test, ++subtest, x)
#endif

#define TRACE(x) if (long_form) \
    {fprintf(stdout, "    %s\n", x); fflush(stdout);}


#define CHECK_STATUS(MSG, ERROR_STATUS)					\
	if (ERROR_STATUS) {						\
	    fprintf(stderr, "FAILED %s \n", MSG);			\
	    fflush(stderr);						\
	}

#define CHECK(MSG)   							\
    temp_c_ptr = NULL;							\
    temp_c_ptr = (char *)dce_msg_get_default_msg(MSG, &error_status);	\
    CHECK_STATUS("dce_msg_get_default_msg", error_status);		\
    temp_c_ptr = NULL;							\
    temp_c_ptr = (char *)dce_msg_get_cat_msg(MSG, &error_status);	\
    CHECK_STATUS("dce_msg_get_cat_msg", error_status);			\
    temp_c_ptr = NULL;							\
    temp_c_ptr = (char *)dce_msg_get_msg(MSG, &error_status);		\
    CHECK_STATUS("dce_msg_get_msg", error_status);			\
    if (temp_c_ptr) free(temp_c_ptr);


extern int optind, opterr;
extern char * optarg;
/* extern int getopt(int argc, char * argv[], char * str); */

lprint(va_alist)
va_dcl
{
        va_list vargs;
        char msg[1024];                 /* buffer for info lines */
        char *fmt;

        va_start(vargs);
        fmt = va_arg(vargs, char *);
        (void) vsprintf(msg, fmt, vargs);
        va_end(vargs);
#ifdef ENVIRONMENT_TET                                                
        tet_infoline (msg);
#else
        fprintf (stdout, msg);
	fflush(stdout);	
#endif
}


#ifdef ENVIRONMENT_TET

static void startup(void) {
}

static void cleanup(void) {
}

#endif

void display_major_status(OM_uint32 major_status) {
    gss_buffer_desc message_buffer;
    int message_context;
    OM_uint32 maj_stat, min_stat;

    message_context = 0;

    do {
	message_buffer.value = NULL;
	message_buffer.length = 0;
	TRACE("Calling gss_display_status for major status code");
	maj_stat = gss_display_status(&min_stat,
				      major_status,
				      GSS_C_GSS_CODE,
				      NULL,
				      &message_context,
				      &message_buffer);
	if (!GSS_ERROR(maj_stat)) {
	    fprintf(stderr,
		    "   %.*s\n",
		    message_buffer.length,
		    message_buffer.value);
	} else {
	    fprintf(stderr,
		    "   Received error (%X) while translating major status\n",
		    maj_stat);
	};
	fflush(stderr);
    } while (message_context != 0);
}

void display_minor_status(OM_uint32 minor_status) {
    gss_buffer_desc message_buffer;
    int message_context;
    OM_uint32 maj_stat, min_stat;

    message_context = 0;
    message_buffer.value = NULL;
    message_buffer.length = 0;
    TRACE("Calling gss_display_status for minor status code");
    maj_stat = gss_display_status(&min_stat,
				  minor_status,
				  GSS_C_MECH_CODE,
				  NULL,
				  &message_context,
				  &message_buffer);
    if (!GSS_ERROR(maj_stat)) {
	fprintf(stderr,
		"   %.*s\n",
		message_buffer.length,
		message_buffer.value);
    } else {
	fprintf(stderr,
		"   Received error (%X) while translating minor status\n",
		maj_stat);
    };
    fflush(stderr);
}

int check_status(char * test_string,
		  OM_uint32 major_status,
		  OM_uint32 minor_status,
		  OM_uint32 exp_major_status,
		  OM_uint32 exp_minor_status) {
    if (major_status != exp_major_status) {
	fprintf(stderr, "Error - %d, %s - Major status mismatch\n",
		test, test_string);
	fflush(stderr);
	fprintf(stderr, "Expecting %X, received %X\n",
		exp_major_status, major_status);
	fflush(stderr);
	fprintf(stderr, "Minor status: received %X\n", minor_status);
	fflush(stderr);
	display_major_status(major_status);
	display_minor_status(minor_status);
	error = 1;
        fprintf(stderr, "\n");
        fflush(stderr);
    } else if (minor_status != exp_minor_status) {
	fprintf(stderr, "Error - %d, %s - Minor status mismatch\n", 
		test, test_string);
	fflush(stderr);
	fprintf(stderr, "Expecting %X, received %X\n",
		exp_minor_status, minor_status);
	fflush(stderr);
	display_minor_status(minor_status);
	error = 1;
        fprintf(stderr, "\n");
        fflush(stderr);
    };
    return error;
}

int parse_list(char * line, int test_array[], int max_index) {
    char * start_ptr;
    char * end_ptr;
    int error = 0;
    int test_number;
    int i;
    char overflow[128];


    if (strlen(line) > sizeof(overflow)-1) return 1; /* Can't parse long    */
						     /*	lines.		    */

    start_ptr = line;
    end_ptr = start_ptr;

/*
    fprintf(stderr, "Parsing \"%s\"\n", start_ptr);
    fflush(stderr);
*/
    while ((*start_ptr == ' ') || (*start_ptr == ',')) start_ptr++;
    
    while (1) {
	if ((start_ptr == NULL) || (*start_ptr == '\0')) {
/*
	    fprintf(stderr, "Done...\n");
	    fflush(stderr);
*/
	    break;
	};

	if ((*end_ptr == '\0') || (*end_ptr == ',') || (*end_ptr == ' ')) {
/*
	    fprintf(stderr, "Found an atom\n");
	    fflush(stderr);
*/
	    if (*end_ptr != '\0') *end_ptr++ = '\0';

/*
	    fprintf(stderr, "Atom: \"%s\". Parsing...\n", start_ptr);
	    fflush(stderr);
*/
/* Now start_ptr points to a single null-terminated test-number.  Parse	it. */
	    if (sscanf(start_ptr,
		       "%u%s",
		       &test_number,
		       overflow) != 1) {

/*
		fprintf(stderr, "Error detected in atom\n");
		fflush(stderr);
*/
		error = 1; break;
	    };
	    if (test_number <= max_index) {
/*
		fprintf(stderr, "Activating test %d\n", test_number);
		fflush(stderr);
*/
		test_array[test_number-1] = !test_array[test_number-1];
	    };

	    if (*end_ptr == '\0') {
/*
		fprintf(stderr, "No more atoms\n");
		fflush(stderr);
*/
		break;
	    };
	    start_ptr = end_ptr;
	    while ((*start_ptr == ' ') || (*start_ptr == ',')) start_ptr++;
	    end_ptr = start_ptr;
	} else end_ptr++;
    };

parse_exit:
    return error;
}


void pause(void) {
    char buffer[80];
    
    if (!interactive) return;

    fflush(stdout);
    fflush(stderr);
    fprintf(stderr, "Press return to continue...");
    fflush(stderr);
    fgets(buffer, sizeof(buffer), stdin);
}

void display_dce_error(error_status_t stat) {
    char * temp_c_ptr = NULL;
    error_status_t error_status;

    fprintf(stderr, 
	    "Message: %s\n", 
	    temp_c_ptr = (char *)dce_msg_get_msg(stat, &error_status));
    fflush(stderr);

    if (error_status) {
	fprintf(stderr,
		"Error %X retrieving error text\n",
		error_status);
	fflush(stderr);
    };
    if (temp_c_ptr) free(temp_c_ptr);
}

int verify_login_context(sec_login_handle_t lc, 
			 error_status_t * error_status);

int verify_login_context(sec_login_handle_t lc, 
			 error_status_t * error_status) {
    signed32 exp;
    unsigned32 num_groups;
    signed32* group_set;


    if (long_form) {
	lprint( ">>>Checking login-context %lx\n", (long)lc);
    };


    TRACE("Calling sec_login_get_expiration");
    sec_login_get_expiration(lc,
			     &exp,
			     error_status);
    TRACE("Returned from sec_login_get_expiration");

    if ((*error_status != 0) && (*error_status != sec_login_s_not_certified)) {
	return 1;
    };

    
    TRACE("Calling sec_login_get_groups");
    sec_login_get_groups(lc,
			 &num_groups,
			 &group_set,
			 error_status);
    TRACE("Returned from sec_login_get_groups");

    if ((*error_status != 0) && (*error_status != sec_login_s_not_certified)) {
	return 1;
    };


    if (long_form) {
	lprint( ">>>Exp: %ld\n", (long)exp);
	lprint( ">>>num_groups: %ld\n", (long)num_groups);
    };

/* Else return a success indication...					    */
    return 0;

}

/*
 * Misalign buffers -- used to ensure that auth substrates
 * don't rely on the (usual) natural alignment of tokens
 */
int misalign_token_buffer(gss_buffer_desc *token, gss_buffer_desc *token_ua)
{
    token_ua->value = malloc(token->length + 1);
    if (token_ua->value == NULL) {
	return(0);
    };

    token_ua->length = token->length;
    token_ua->value = (idl_byte *)token_ua->value + 1;
    memcpy(token_ua->value, token->value, token->length);

    if (long_form) {
	lprint(">>>Token buffer @ 0x%08lx\n", (long)token_ua->value);
    };

    return(1);
}

typedef struct error_info_t_struct {
    OM_uint32 value;
    char * name;
} error_info_t;

#ifndef ENVIRONMENT_TET
int main(int argc, char * argv[]) {
#else
int loc_main(int argc, char * argv[]) {
#endif
    error_status_t error_status;
    
    gss_name_t iname2;
    gss_name_t client_name;
    gss_name_t server_name;
    char * client_char_name = NULL;
    char * server_char_name = NULL;
    char * server_table = NULL;
    OM_uint32 major_status, minor_status;
    gss_buffer_desc buffer1;
    gss_buffer_desc buffer2;
    gss_buffer_desc client_message_1;
    gss_buffer_desc client_message_2;
    gss_buffer_desc client_message_3;
    gss_OID name_type;
    int errflg = 0;
    char c;
    gss_cred_id_t dce_man_cred;
    gss_cred_id_t server_dce_cred;
    gss_cred_id_t client_dce_cred;
    gss_cred_id_t server_krb_cred;
    gss_cred_id_t client_krb_cred;
    gss_cred_id_t delegated_cred_handle;
    gss_OID_set	  actual_mech_set;
    gss_OID	  actual_mech;
    OM_uint32 time_rec;
    OM_uint32 time_rec_1;
    OM_uint32 lifetime;
    int cred_usage;
    gss_OID_set mechanisms;
    gss_OID_set krb_mechanism_only;
    gss_OID_set temp_oid_set;

    gss_buffer_desc empty_buffer;

    gss_ctx_id_t client_context;
    gss_ctx_id_t server_context;

    gss_ctx_id_t client_temp_ctx;
    gss_ctx_id_t server_temp_ctx;

    struct gss_channel_bindings_struct client_bindings;
    struct gss_channel_bindings_struct server_bindings;

    gss_buffer_desc token_1, token_1ua;
    gss_buffer_desc token_2, token_2ua;
    gss_buffer_desc token_3, token_3ua;

    sec_cred_attr_cursor_t  pa_cursor;
    sec_attr_t extended_attr;
    int client_ret_flags;
    int server_ret_flags;
    int printed_flags;

    int conf_state;

    int privacy_supported = 0;

    char * test_data = "This is some test data.";
    char * bad_data =  "That is some test data.";

    int message_qop;
    int i;

    sec_id_pac_t clients_pac;
    rpc_authz_cred_handle_t clients_creds;
    sec_cred_pa_handle_t clients_pa_handle;

    char * temp_c_ptr;

    sec_login_handle_t initial_login_context = 0;

    sec_login_handle_t client_login_context = 0;
    sec_login_handle_t client_impersonation_login_context = 0;
    sec_login_handle_t client_traced_login_context = 0;

    sec_login_handle_t man_login_context = 0;
    sec_login_handle_t saved_man_login_context = 0;

    gss_cred_id_t client_impersonation_cred;
    gss_cred_id_t client_traced_cred;

    gss_OID_set test_oid_set;
    int is_present;

    
    gss_OID test_oid;
        
#define MAX_TESTS 25

    int test_wanted[MAX_TESTS] = {0};
    char * preselected_test_names = "";
    char * client_password = NULL;
    sec_passwd_rec_t password_record;
    boolean32 reset_password;
    sec_login_auth_src_t auth_src;
    char *test_desc[MAX_TESTS] = {"Error codes",
				  "Name Conversions",
				  "Server keytable registration",
				  "Credential establishment",
				  "Kerberos authentication",
				  "DCE authentication",
				  "Sign service",
				  "Seal service (non-encrypting)",
				  "Seal service (encrypting)",
				  "PAC extraction routines (obsolete)",
				  "Context deletion",
				  "DCE Credential extraction",
				  "DCE login-context extraction",
				  "Delegation test 1: Refusing impersonation",
				  "Delegation test 2: Refusing traced delegation",
				  "Delegation test 3: Accepting impersonation",
				  "Delegation test 4: Accepting traced delegation",
				  "Delegation test 5: Constructing impersonation",
				  "Misc routines",
				  "Credential management",
				 };


#define TEST_ERRORS 0
#define TEST_NAMES 1
#define TEST_SRVTAB 2
#define TEST_CRED_DCE 3
#define TEST_KRB 4
#define TEST_DCE 5
#define TEST_SIGN 6
#define TEST_SEAL_INT 7
#define TEST_SEAL_CONF 8
#define TEST_PAC_EXTRACT 9
#define TEST_DEL_CTX 10
#define TEST_DCE_CRED_EXTRACT 11
#define TEST_DCE_CRED_TO_LOGIN 12
#define TEST_DELEG_REFUSE_IMPERS 13
#define TEST_DELEG_REFUSE_TRACED 14
#define TEST_DELEG_DO_IMPERS 15
#define TEST_DELEG_DO_TRACED 16
#define TEST_DELEG_MAKE_IMPERS 17
#define TEST_MISC 18
#define TEST_CRED_MAN 19

    int highest_test;
    int done;
    char selection_line[80];
    int ch;
    int test_selection;
    OM_uint32 status_code;
    int message_context;
    gss_buffer_desc message_buffer;
    int message_count;
    int selected;
    
    error_info_t status[50] = 
		  {{GSS_S_BAD_MECH, "GSS_S_BAD_MECH"},
		   {GSS_S_BAD_NAME, "GSS_S_BAD_NAME"},
		   {GSS_S_BAD_NAMETYPE, "GSS_S_BAD_NAMETYPE"},
		   {GSS_S_BAD_BINDINGS, "GSS_S_BAD_BINDINGS"},
		   {GSS_S_BAD_STATUS, "GSS_S_BAD_STATUS"},
		   {GSS_S_BAD_SIG, "GSS_S_BAD_SIG"},
		   {GSS_S_NO_CRED, "GSS_S_NO_CRED"},
		   {GSS_S_NO_CONTEXT, "GSS_S_NO_CONTEXT"},
		   {GSS_S_DEFECTIVE_TOKEN, "GSS_S_DEFECTIVE_TOKEN"},
		   {GSS_S_DEFECTIVE_CREDENTIAL, "GSS_S_DEFECTIVE_CREDENTIAL"},
		   {GSS_S_CREDENTIALS_EXPIRED, "GSS_S_CREDENTIALS_EXPIRED"},
		   {GSS_S_CONTEXT_EXPIRED, "GSS_S_CONTEXT_EXPIRED"},
		   {GSS_S_FAILURE, "GSS_S_FAILURE"},
		   {GSS_S_BAD_SIG | GSS_S_OLD_TOKEN, "BAD_SIG+OLD_TOKEN"}};




    empty_buffer.length = 0;
    empty_buffer.value = NULL;

#ifdef ENVIRONMENT_TET
    tet_infoline("GSSAPI tester started");
#else
    fprintf(stdout, "GSSAPI tester\n\n\n");
#endif

    while ((c = getopt(argc, argv, "lit:p:")) != (char)EOF) {
    
	switch(c) {
	case 'l':
	    long_form = 1;
	    break;
	case 'i':
	    interactive = 1;
	    break;
	case 'p':
	    client_password = optarg;
	    break;
	case 't':
	    preselected_test_names = optarg;

/*
	    fprintf(stderr, "'t' arg: \"%s\"\n", preselected_test_names);
	    fflush(stderr);
*/
	    break;
	default:
	    errflg = 1;
	};
    };

    if (optind < argc) {
	client_char_name = argv[optind++];
	if (optind < argc) {
	    server_char_name = argv[optind++];
	    if (optind < argc) {
		server_table = argv[optind];
	    } else {
		server_table = NULL;
	    };
	} else {
#ifdef ENVIRONMENT_TET
	    tet_infoline("Error parsing server name");
#endif
	    fprintf(stderr, "Error parsing server name\n");
	    errflg = 1;
	};
    } else {
#ifdef ENVIRONMENT_TET
	tet_infoline("Error parsing client name");
#endif
	fprintf(stderr, "Error parsing client name\n");
	errflg = 1;
    };


    if (errflg) {
#ifdef ENVIRONMENT_TET
	tet_infoline("Usage error (see stderr for correct syntax)");
#endif
	fprintf(stderr,
		"Usage: %s [-l] [-i] [-t tests] [-p password] <client> <server> [<keytab>]\n",
		argv[0]);
	return EXIT_FAILURE;
    };


/* Parse the pre-selected test-list..					    */
    parse_list(preselected_test_names, test_wanted, MAX_TESTS);

/* Check to see whether we have to log-in...				    */

    if (client_password != NULL) {
/* We've been given a password, so we're expected to log-in as the client.  */

	lprint( "Logging in as \"%s\"...\n", client_char_name);

	password_record.version_number = 1;
	password_record.pepper = NULL;
	password_record.key.key_type = sec_passwd_plain;
	password_record.key.tagged_union.plain = (idl_char *)client_password;

	if (sec_login_setup_identity((unsigned_char_p_t)client_char_name,
				     sec_login_no_flags,
				     &initial_login_context,
				     &error_status)) {
	    if (sec_login_validate_identity(initial_login_context,
					    &password_record,
					    &reset_password,
					    &auth_src,
					    &error_status)) {
		sec_login_set_context(initial_login_context,
				      &error_status);
		if (error_status) {
#ifdef ENVIRONMENT_TET
		    tet_infoline("Can't set context");
#endif
		    fprintf(stderr,
			    "Error - sec_login_set_context failed (%X)\n",
			    error_status);
		    error = 1;
		    goto finish;
		};
	    } else {
#ifdef ENVIRONMENT_TET
		tet_infoline("Can't validate identity");
#endif
		fprintf(stderr,
			"Error - sec_login_validate_identity failed (%X)\n",
			error_status);
		fflush(stderr);
		error = 1;
		goto finish;
	    };

	} else {
#ifdef ENVIRONMENT_TET
	    tet_infoline("Can't setup identity");
#endif
	    fprintf(stderr,
		    "Error - sec_login_setup_identity failed (%X)\n",
		    error_status);
	    fflush(stderr);
	    error = 1;
	    goto finish;
	};

	lprint( "Succesfully logged in as client.\n");

    };

    if (interactive) {
	lprint( "Tests available:\n\n");
	for (i=0; i<MAX_TESTS; i++) {
	    if (test_desc[i]) {
		lprint( "%2.1u:  %s\n", i+1, test_desc[i]);
	    } else break;
	};
	highest_test = i-1;
	done = 0;
	lprint( "\n");
	while (!done) {
	    lprint( "Selected tests: ");
	    selected = 0;
	    for (i=0; i<MAX_TESTS; i++) {
		if (test_wanted[i] && test_desc[i]) {
		    lprint( "%d ",i+1);
		    selected = 1;
		};
	    };
	    if (!selected) lprint( "<none>");
	    lprint( "\nSelect test, 'A' to select all, 0 or 'S' to start:");

	    for (i=0; i < 79; i++) {
		if (((ch = getc(stdin)) == EOF) || (ch == '\n')) {
		    break;
		} else {
		    selection_line[i] = ch;
		};
	    };
	    selection_line[i] = 0;

	    if (parse_list(selection_line, test_wanted, MAX_TESTS) != 0) {
    
		if (!strcmp(selection_line, "A") || 
		    !strcmp(selection_line, "a")) {
		    for (i=0; i<MAX_TESTS; i++) {
			test_wanted[i] = 1;
		    };
		} else if (!strcmp(selection_line, "S") || 
			   !strcmp(selection_line, "s")) {
		    done = 1;
		} else {
		    lprint( "\a");
		};
	    };
	};

    } else {
	selected = 0;

	for (i=0; i<MAX_TESTS; i++) {
	    if (test_wanted[i]) selected = 1;
	};

	if (!selected) {
	    lprint( "No tests specified, performing all tests\n");
	    for (i=0; i<MAX_TESTS; i++) {
		test_wanted[i] = 1;
	    };
	};
    };

/* Sanity-check the test selections, adding any pre-requisite tests...	    */


    if ((test_wanted[TEST_SIGN] 
	 || test_wanted[TEST_DEL_CTX] 
	 || test_wanted[TEST_SEAL_INT] 
	 || test_wanted[TEST_SEAL_CONF]
	 || test_wanted[TEST_PAC_EXTRACT]
	 || test_wanted[TEST_DCE_CRED_EXTRACT])
	&& !test_wanted[TEST_DCE]) {
	lprint( 
		"Tests %d, %d, %d, %d, %d and %d require that test %d also be run.\n",
		TEST_SIGN+1,
		TEST_DEL_CTX+1,
		TEST_SEAL_INT+1,
		TEST_SEAL_CONF+1,
		TEST_PAC_EXTRACT+1,
		TEST_DCE_CRED_EXTRACT+1,
		TEST_DCE+1);
	test_wanted[TEST_DCE] = 1;	
    };



    if ((test_wanted[TEST_DELEG_REFUSE_IMPERS] ||
	 test_wanted[TEST_DELEG_REFUSE_TRACED] ||
	 test_wanted[TEST_DELEG_DO_IMPERS] ||
	 test_wanted[TEST_DELEG_DO_TRACED] ||
	 test_wanted[TEST_DELEG_MAKE_IMPERS])
	&& (!test_wanted[TEST_DCE] ||
	    !test_wanted[TEST_DCE_CRED_TO_LOGIN])) {
#ifndef ENVIRONMENT_TET
	lprint( 
		"Tests %d, %d, %d, %d and %d require that tests %d and %d also be run.\n",
		TEST_DELEG_REFUSE_IMPERS+1,
		TEST_DELEG_REFUSE_TRACED+1,
		TEST_DELEG_DO_IMPERS+1,
		TEST_DELEG_DO_TRACED+1,
		TEST_DELEG_MAKE_IMPERS+1,
		TEST_DCE+1,
		TEST_DCE_CRED_TO_LOGIN+1);
#endif
	test_wanted[TEST_DCE] = 1;	
	test_wanted[TEST_DCE_CRED_TO_LOGIN] = 1;	
    };


    if (test_wanted[TEST_DCE_CRED_TO_LOGIN] && !test_wanted[TEST_DCE]) {
#ifndef ENVIRONMENT_TET
	lprint( 
		"Test %d requires that test %d also be run.\n",
		TEST_DCE_CRED_TO_LOGIN+1,
		TEST_DCE+1);
#endif
	test_wanted[TEST_DCE] = 1;	
    };


    if (test_wanted[TEST_DCE] && !test_wanted[TEST_CRED_DCE]) {
#ifndef ENVIRONMENT_TET
	lprint( 
		"Test %d requires that test %d also be run.\n",
		TEST_DCE+1,
		TEST_CRED_DCE+1);
#endif
	test_wanted[TEST_CRED_DCE] = 1;	
    };


    if ((test_wanted[TEST_KRB]
	 || test_wanted[TEST_CRED_DCE])
	 && !test_wanted[TEST_SRVTAB]
	 && (server_table != NULL)) {
#ifndef ENVIRONMENT_TET
	lprint( 
    "The requested tests require that a server credential be created.\n");
	lprint( 
    "Since you have specified a server keytable (%s),\n",
		server_table);
	lprint( 
    "the server registration procedure will also be tested.\n");
#endif
	test_wanted[TEST_SRVTAB] = 1;	
    };

    if (test_wanted[TEST_SRVTAB] && (server_table == NULL)) {
#ifndef ENVIRONMENT_TET
	lprint( 
		"Testing the server registration routines requires\n");
	lprint( 
	    "that you specify a server keytable as a third parameter.\n");
	lprint( 
	    "Server registration will not be tested.\n");
#endif
	test_wanted[TEST_SRVTAB] = 0;
    };


#ifndef ENVIRONMENT_TET
    lprint( "\nPerforming following tests:\n");
    for (i=0; i<MAX_TESTS; i++) {
	if (test_desc[i]) {
	    if (test_wanted[i])
		lprint( "%2.1u:  %s\n", i+1, test_desc[i]);
	} else break;
    };


    lprint(
"\n===========================================================================\n");
#endif


/* Call setlocale since we are going to be making message catalog requests */
    setlocale (LC_ALL, "");

    if (test_wanted[TEST_ERRORS]) {
	TEST("Error message routines");
	TRACE("Checking default message availability via dce_msg_get_default_msg()\n");

/* Make a harmless GSS call to initialize GSSAPI...			    */
	gssdce_create_empty_oid_set(&minor_status, &temp_oid_set);

	CHECK(rpc_s_ok);
	CHECK(GSSDCE_S_MESSAGE_LATE);

	lprint("Supplementary info bits...\n");

	status_code = 1;
	message_count = 0;
	do {
	    message_context = 0;
	    lprint("Bit %2.2u:\n", message_count);
	    do {
		TRACE("Calling gss_display_status for bit code");
		major_status = gss_display_status(&minor_status,
						  status_code,
						  GSS_C_GSS_CODE,
						  NULL,
						  &message_context,
						  &message_buffer);
		TRACE("Returned from gss_display_status");
		if (!GSS_ERROR(major_status)) {
		    lprint("   %.*s\n",
			    message_buffer.length,
			    message_buffer.value);

		    TRACE("Calling gss_release_buffer");
		    major_status = gss_release_buffer(&minor_status,
						      &message_buffer);
		    TRACE("Returned from gss_release_buffer");

		} else {
		    lprint("   <No message>\n");
		};
	    } while (message_context != 0);
	    status_code = status_code << 1;
	    message_count++;
	} while (message_count < 16);

	if (interactive) pause();
	lprint("\nNamed error codes:\n");
	    
	message_count = 0;
	while (status[message_count].value != 0) {
	    message_context = 0;
	    lprint( "%s:\n", status[message_count].name);
	    do {
		TRACE("Calling gss_display_status for code");
		major_status = gss_display_status(&minor_status,
						  status[message_count].value,
						  GSS_C_GSS_CODE,
						  NULL,
						  &message_context,
						  &message_buffer);
		if (!GSS_ERROR(major_status)) {
		    lprint(
			    "   %.*s\n",
			    message_buffer.length,
			    message_buffer.value);
		    major_status = gss_release_buffer(&minor_status,
						      &message_buffer);
		} else {
		    lprint(
			    "   <No message>\n");
		};
	    } while (message_context != 0);
	    message_count++;
	};




	lprint(
"\n===========================================================================\n");

#ifdef ENVIRONMENT_TET
	mtet_infoline("PASSED GSSAPI Error message routines\n");
#endif
    };

    if (test_wanted[TEST_MISC]) {
	TEST("Miscellaneous routine tests");


	SUBTEST("OID manipulation, set creation");

	TRACE ("Calling GSSDCE_CREATE_EMPTY_OID_SET");
	major_status = gssdce_create_empty_oid_set(&minor_status, 
						   &test_oid_set);
	TRACE ("Returned from GSSDCE_CREATE_EMPTY_OID_SET");
    

	if (check_status("gssdce_create_empty_oid_set", 
			 major_status, minor_status,
			 GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};

	if (test_oid_set == NULL) {
	    fprintf(stderr, "Error - Empty OID set is NULL pointer\n");
	    error = 1;
	    goto finish;
	} else if (test_oid_set->count != 0) {
	    fprintf(stderr, "Error - Empty OID set has non-zero element-count\n");
	    error = 1;
	    goto finish;
	} else if (test_oid_set->elements != NULL) {
	    fprintf(stderr, "Error - Empty OID set has non-NULL element pointer\n");
	    error = 1;
	    goto finish;
	};


	TRACE ("Calling GSSDCE_ADD_OID_SET_MEMBER");
	major_status = gssdce_add_oid_set_member(&minor_status,
						 GSSDCE_C_OID_KRBV5_DES,
						 &test_oid_set);
	TRACE ("Returned from GSSDCE_ADD_OID_SET_MEMBER");


	if (check_status("gssdce_add_oid_set_member", 
			 major_status, minor_status,
			 GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};

	if (test_oid_set == NULL) {
	    fprintf(stderr, "Error - OID set is NULL pointer\n");
	    error = 1;
	    goto finish;
	} else if (test_oid_set->count != 1) {
	    fprintf(stderr,
		    "Error - single-element OID set has element-count = %d\n",
		    test_oid_set->count);
	    error = 1;
	    goto finish;
	} else if (test_oid_set->elements == NULL) {
	    fprintf(stderr,
		    "Error - single-element OID set has NULL element pointer\n");
	    error = 1;
	    goto finish;
	};



	TRACE ("Calling GSSDCE_ADD_OID_SET_MEMBER");
	major_status = gssdce_add_oid_set_member(&minor_status,
						 GSSDCE_C_OID_DCENAME,
						 &test_oid_set);
	TRACE ("Returned from GSSDCE_ADD_OID_SET_MEMBER");


	if (check_status("gssdce_add_oid_set_member", 
			 major_status, minor_status,
			 GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};



	if (test_oid_set == NULL) {
	    fprintf(stderr, "Error - OID set is NULL pointer\n");
	    error = 1;
	    goto finish;
	} else if (test_oid_set->count != 2) {
	    fprintf(stderr,
		    "Error - double-element OID set has element-count = %d\n",
		    test_oid_set->count);
	    error = 1;
	    goto finish;
	} else if (test_oid_set->elements == NULL) {
	    fprintf(stderr,
		    "Error - double-element OID set has NULL element pointer\n");
	    error = 1;
	    goto finish;
	};




	
	for (i=0;i<2;i++) {
	    lprint(
		    "Element %d: length=%d, Addr=%p\n",
		    i,
		    test_oid_set->elements[i].length,
		    test_oid_set->elements[i].elements);	
	};




	SUBTEST("OID manipulation, set interrogation 1");

	TRACE ("Calling GSSDCE_TEST_OID_SET_MEMBER");
	major_status = gssdce_test_oid_set_member(&minor_status,
						  GSSDCE_C_OID_DCENAME,
						  test_oid_set,
						  &is_present);
	TRACE ("Returned from GSSDCE_TEST_OID_SET_MEMBER");

	if (check_status("gssdce_test_oid_set_member", 
			 major_status, minor_status,
			 GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};

	if (!is_present) {
	    fprintf(stderr, 
		    "Error - GSSDCE_C_OID_DCENAME not present in OID set\n");
	    fflush(stderr);
	    error = 1;
	};


	for (i=0;i<2;i++) {
	    lprint(
		    "Element %d: length=%d, Addr=%p\n",
		    i,
		    test_oid_set->elements[i].length,
		    test_oid_set->elements[i].elements);	
	};


	SUBTEST("OID manipulation, set interrogation 2");

	TRACE ("Calling GSSDCE_TEST_OID_SET_MEMBER");
	major_status = gssdce_test_oid_set_member(&minor_status,
						  GSSDCE_C_OID_DCE_KRBV5_DES,
						  test_oid_set,
						  &is_present);
	TRACE ("Returned from GSSDCE_TEST_OID_SET_MEMBER");

	if (check_status("gssdce_test_oid_set_member", 
			 major_status, minor_status,
			 GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};

	if (is_present) {
	    fprintf(stderr, 
		    "Error - GSSDCE_C_OID_DCE_KRBV5_DES _is_ present in OID set\n");
	    fflush(stderr);
	    error = 1;
	};

	for (i=0;i<2;i++) {
	    lprint(
		    "Element %d: length=%d, Addr=%p\n",
		    i,
		    test_oid_set->elements[i].length,
		    test_oid_set->elements[i].elements);	
	};


	SUBTEST("OID manipulation, set interrogation 3");

	TRACE ("Calling GSSDCE_TEST_OID_SET_MEMBER");
	major_status = gssdce_test_oid_set_member(&minor_status,
						  GSSDCE_C_OID_KRBV5_DES,
						  test_oid_set,
						  &is_present);
	TRACE ("Returned from GSSDCE_TEST_OID_SET_MEMBER");

	if (check_status("gssdce_test_oid_set_member", 
			 major_status, minor_status,
			 GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};

	if (!is_present) {
	    fprintf(stderr, 
		    "Error - GSSDCE_C_OID_KRBV5_DES not present in OID set\n");
	    fflush(stderr);
	    error = 1;
	};

	if (error) goto finish;
	
	for (i=0;i<2;i++) {
	    lprint(
		    "Element %d: length=%d, Addr=%p\n",
		    i,
		    test_oid_set->elements[i].length,
		    test_oid_set->elements[i].elements);	
	};


	SUBTEST("OID manipulation, set free");

	TRACE ("Calling GSS_RELEASE_OID_SET");
	major_status = gss_release_oid_set(&minor_status,
					   &test_oid_set);
	TRACE ("Returned from GSS_RELEASE_OID_SET");


	SUBTEST("GSS_INDICATE_MECHS");

	TRACE ("Calling GSS_INDICATE_MECHS");
	major_status = gss_indicate_mechs(&minor_status,
					  &test_oid_set);
	TRACE ("Returned from GSS_INDICATE_MECHS");


	SUBTEST("GSS_INDICATE_MECHS, verifying results");

	TRACE ("Calling GSSDCE_TEST_OID_SET_MEMBER");
	major_status = gssdce_test_oid_set_member(&minor_status,
						  GSSDCE_C_OID_DCENAME,
						  test_oid_set,
						  &is_present);
	TRACE ("Returned from GSSDCE_TEST_OID_SET_MEMBER");

	if (check_status("gssdce_test_oid_set_member", 
			 major_status, minor_status,
			 GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};

	if (is_present) {
	    fprintf(stderr, 
		    "Error - GSSDCE_C_OID_DCENAME _is_ present in mech-list\n");
	    fflush(stderr);
	    error = 1;
	};


	TRACE ("Calling GSSDCE_TEST_OID_SET_MEMBER");
	major_status = gssdce_test_oid_set_member(&minor_status,
						  GSSDCE_C_OID_DCE_KRBV5_DES,
						  test_oid_set,
						  &is_present);
	TRACE ("Returned from GSSDCE_TEST_OID_SET_MEMBER");

	if (check_status("gssdce_test_oid_set_member", 
			 major_status, minor_status,
			 GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};

	if (!is_present) {
	    fprintf(stderr, 
		    "Error - GSSDCE_C_OID_DCE_KRBV5_DES not present in mech-list\n");
	    fflush(stderr);
	    error = 1;
	};


	TRACE ("Calling GSSDCE_TEST_OID_SET_MEMBER");
	major_status = gssdce_test_oid_set_member(&minor_status,
						  GSSDCE_C_OID_KRBV5_DES,
						  test_oid_set,
						  &is_present);
	TRACE ("Returned from GSSDCE_TEST_OID_SET_MEMBER");

	if (check_status("gssdce_test_oid_set_member", 
			 major_status, minor_status,
			 GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};

	if (!is_present) {
	    fprintf(stderr, 
		    "Error - GSSDCE_C_OID_KRBV5_DES not present in mech-list\n");
	    fflush(stderr);
	    error = 1;
	};

	if (error) goto finish;
	

	SUBTEST("Releasing mech-set");

	TRACE ("Calling GSS_RELEASE_OID_SET");
	major_status = gss_release_oid_set(&minor_status,
					   &test_oid_set);
	TRACE ("Returned from GSS_RELEASE_OID_SET");


	SUBTEST("Attempting release of static OID");

	TRACE ("Calling GSSDCE_RELEASE_OID");
	major_status = gssdce_release_oid(&minor_status,
					  &GSSDCE_C_OID_DCE_KRBV5_DES);
	TRACE ("Returned from GSS_RELEASE_OID");

	SUBTEST("Verifying static OID after release attempt");

	if (GSSDCE_C_OID_DCE_KRBV5_DES == NULL) {
	    fprintf(stderr, "Error - gssdce_release_oid has clobbered static");
	    fflush(stderr);
	    error = 1;
	    goto finish;
	};

	if (GSSDCE_C_OID_DCE_KRBV5_DES->length != 4) {
	    fprintf(stderr, "Error in static OID length after release\n");
	    fflush(stderr);
	    error = 1;
	    goto finish;
	};

	if (GSSDCE_C_OID_DCE_KRBV5_DES->elements == NULL) {
	    fprintf(stderr, "Error - Elements field is NULL after release");
	    fflush(stderr);
	    error = 1;
	    goto finish;
	};

	if (memcmp(GSSDCE_C_OID_DCE_KRBV5_DES->elements,
		   "\x2b\x18\x09\x08",
		   4) != 0) {
	    fprintf(stderr, "Error in static OID value after release\n");
	    fflush(stderr);
	    error = 1;
	    goto finish;
	};
	


	SUBTEST("Attempting release of constant OID");

	test_oid = GSSDCE_C_OID_DCE_KRBV5_DES;

	TRACE ("Calling GSSDCE_RELEASE_OID");
	major_status = gssdce_release_oid(&minor_status,
					  &test_oid);
	TRACE ("Returned from GSS_RELEASE_OID");

	SUBTEST("Verifying static OID after release attempt");

	if (GSSDCE_C_OID_DCE_KRBV5_DES == NULL) {
	    fprintf(stderr, "Error - gssdce_release_oid has clobbered static");
	    fflush(stderr);
	    error = 1;
	    goto finish;
	};

	if (GSSDCE_C_OID_DCE_KRBV5_DES->length != 4) {
	    fprintf(stderr, "Error in static OID length after release\n");
	    fflush(stderr);
	    error = 1;
	    goto finish;
	};

	if (GSSDCE_C_OID_DCE_KRBV5_DES->elements == NULL) {
	    fprintf(stderr, "Error - Elements field is NULL after release");
	    fflush(stderr);
	    error = 1;
	    goto finish;
	};

	if (memcmp(GSSDCE_C_OID_DCE_KRBV5_DES->elements,
		   "\x2b\x18\x09\x08",
		   4) != 0) {
	    fprintf(stderr, "Error in static OID value after release\n");
	    fflush(stderr);
	    error = 1;
	    goto finish;
	};
	


	lprint(
"\n===========================================================================\n");
#ifdef ENVIRONMENT_TET
	mtet_infoline ("PASSED GSSAPI Misc routines\n");
#endif
    };

    if (test_wanted[TEST_NAMES]) {
	TEST("Name conversion routines");


	lprint( 
		"   Converting client-name \"%s\" to internal form\n", 
		client_char_name);

	buffer1.length = strlen(client_char_name);
	buffer1.value = (void *)client_char_name;
	    
	TRACE("Calling GSS_IMPORT_NAME");

	major_status = gss_import_name(&minor_status,
				       &buffer1,
				       GSS_C_NULL_OID,
				       &client_name); 
	
	TRACE("Returned from GSS_IMPORT_NAME");

	if (check_status("import_name", major_status, minor_status,
			 GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};

	TRACE("Calling GSS_DISPLAY_NAME");

	major_status = gss_display_name(&minor_status,
					client_name,
					&buffer1,
					&name_type); 

	TRACE("Returned from GSS_DISPLAY_NAME");

	if (check_status("display_name", major_status, minor_status,
			 GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	if (long_form) {
	    lprint( "   Name converted: \"%.*s\"\n",
		    buffer1.length, buffer1.value);
	};
	
	TRACE("Calling GSS_RELEASE_BUFFER");

	major_status = gss_release_buffer(&minor_status,
					  &buffer1);

	TRACE("Returned from GSS_RELEASE_BUFFER");

	if (check_status("release_buffer", major_status, minor_status,
			 GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};





	SUBTEST("GSS_Import_name for server name");


	lprint( 
		"   Converting server-name \"%s\" to internal form\n", 
		server_char_name);

	buffer1.length = strlen(server_char_name);
	buffer1.value = (void *)server_char_name;
	    
	TRACE("Calling GSS_IMPORT_NAME");

	major_status = gss_import_name(&minor_status,
				       &buffer1,
				       GSS_C_NULL_OID,
				       &server_name); 
	
	TRACE("Returned from GSS_IMPORT_NAME");

	if (check_status("import_name", major_status, minor_status,
			    GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {

	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};

	TRACE("Calling GSS_RELEASE_NAME");

	major_status = gss_release_name(&minor_status,
					&client_name);

	client_name = GSS_C_NO_NAME;

	TRACE("Returned from GSS_RELEASE_NAME");

	if (check_status("release_name (client)", major_status, minor_status,
			 GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};

	major_status = gss_release_name(&minor_status,
					&server_name);

	server_name = GSS_C_NO_NAME;

	TRACE("Returned from GSS_RELEASE_NAME");

	if (check_status("release_name (server)", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};



	lprint(
"\n===========================================================================\n");
	
#ifdef ENVIRONMENT_TET
	mtet_infoline ("PASSED GSSAPI Name conversion routines\n");
#endif
    };

    buffer1.length = strlen(server_char_name);
    buffer1.value = (void *)server_char_name;
	
    TRACE("Calling GSS_IMPORT_NAME");

    major_status = gss_import_name(&minor_status,
				   &buffer1,
				   GSS_C_NULL_OID,
				   &server_name); 

    buffer1.length = strlen(client_char_name);
    buffer1.value = (void *)client_char_name;
	
    TRACE("Calling GSS_IMPORT_NAME");

    major_status = gss_import_name(&minor_status,
				   &buffer1,
				   GSS_C_NULL_OID,
				   &client_name);


    if (test_wanted[TEST_SRVTAB]) {

	lprint( "Registering server in table %s\n", server_table);

	TRACE("Calling GSSDCE_REGISTER_ACCEPTOR_IDENTITY");

	major_status = gssdce_register_acceptor_identity(
				&minor_status,
				server_name,
				NULL,
				server_table);

	TRACE("Returned from GSSDCE_REGISTER_ACCEPTOR_IDENTITY");

	if (check_status("gssdce_register_acceptor_identity",
			 major_status, minor_status,
			 GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	if (check_status("release_buffer", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};

#ifdef ENVIRONMENT_TET
	mtet_infoline ("PASSED GSSAPI Server keytable registration\n");
#endif

    };


/* Initial context testing using Kerberos mechanism type.		    */
/*-KRB-**********************************************************************/

    if (test_wanted[TEST_KRB]) {

	int is_equal;
	
	TEST("Kerberos tests");

	SUBTEST("GSS_Acquire_cred (as KRB server)");

	TRACE("Calling CREATE_EMPTY_OID_SET ");

	major_status = gssdce_create_empty_oid_set(&minor_status,
						   &krb_mechanism_only);    
	TRACE("Returned from CREATE_EMPTY_OID_SET");

	if (check_status("create_empty_oid_set", major_status, minor_status,
			 GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	TRACE ("Calling ADD_OID_SET_MEMBER");
	
	major_status = gssdce_add_oid_set_member(&minor_status,
						 GSSDCE_C_OID_KRBV5_DES,
						 &krb_mechanism_only);    

	TRACE("Returned from ADD_OID_SET_MEMBER");

	if (check_status("add_oid_set_member", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	major_status = gss_acquire_cred(
			    &minor_status,
			    server_name,
			    24*60*60,
			    krb_mechanism_only,
			    GSS_C_ACCEPT,
			    &server_krb_cred,
			    &actual_mech_set,
			    &time_rec);

	TRACE ("returned from GSS_ACQUIRE_CRED");

	lprint( "Got cred handle %lu\n", server_krb_cred);

#if defined(DEBUG)
	lprint( "Credential details: \n%s\n", dump_cred(server_krb_cred));
#endif

	if (check_status("acquire_cred", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	TRACE ("Calling GSS_RELEASE_OID_SET");

	major_status = gss_release_oid_set(&minor_status,
					   &actual_mech_set);

	TRACE ("Returned from GSS_RELEASE_OID_SET");
					   
	TRACE ("Calling GSS_INQUIRE_CRED");

	iname2 = GSS_C_NO_NAME;

	major_status = gss_inquire_cred(&minor_status,
					server_krb_cred,
					&iname2,
					&lifetime,
					&cred_usage,
					&mechanisms);

	TRACE ("returned from GSS_INQUIRE_CRED");

	if (check_status("inquire_cred", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};



	TRACE("Calling GSS_DISPLAY_NAME");

	major_status = gss_display_name(&minor_status,
					iname2,
					&buffer1,
					&name_type); 

	TRACE("Returned from GSS_DISPLAY_NAME");

	if (check_status("display_name", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	lprint( "   Server Name: \"%.*s\"\n",
		buffer1.length, buffer1.value);

	TRACE("Calling GSS_RELEASE_BUFFER");

	major_status = gss_release_buffer(&minor_status,
					  &buffer1);

	TRACE("Returned from GSS_RELEASE_BUFFER");

	if (check_status("release_buffer", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};



	major_status = gss_compare_name(&minor_status,
					iname2,
					server_name,
					&is_equal);
	if (check_status("comapre_name",  major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};

	if (!is_equal) {
	    fprintf(stderr, "Error - server names differ.\n");
	    fflush(stderr);
	    error = 1;
	    goto finish;
	};



	major_status = gss_compare_name(&minor_status,
					iname2,
					client_name,
					&is_equal);
	if (check_status("comapre_name",  major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};

	if (is_equal) {
	    fprintf(stderr, "Error - client and server names equal.\n");
	    fflush(stderr);
	    error = 1;
	    goto finish;
	};

	TRACE("Calling GSS_RELEASE_NAME");

	major_status = gss_release_name(&minor_status,
					&iname2);

	iname2 = GSS_C_NO_NAME;

	TRACE("Returned from GSS_RELEASE_NAME");

	if (check_status("release_name", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};



	lprint( "   Expiration (secs): %lu\n", lifetime);

	lprint( "   Mechanism count: %d\n", mechanisms->count);

	if (cred_usage == GSS_C_ACCEPT) {
	    lprint( "   Credential-type: ACCEPT\n");
	} else if (cred_usage == GSS_C_INITIATE) {
	    lprint( "   Credential-type: INITIATE\n");
	} else if (cred_usage == GSS_C_BOTH) {
	    lprint( "   Credential-type:BOTH\n");
	} else {
	    lprint( "   Credential-type: Unknown (%d)\n", cred_usage);
	};




/*****************************************************************************/
	SUBTEST("GSS_Acquire_cred (as KRB client)");

	TRACE ("Calling GSS_ACQUIRE_CRED");

	major_status = gss_acquire_cred(
			    &minor_status,
			    GSS_C_NO_NAME,
			    24*60*60,
			    krb_mechanism_only,
			    GSS_C_INITIATE,
			    &client_krb_cred,
			    &actual_mech_set,
			    &time_rec);

	TRACE ("returned from GSS_ACQUIRE_CRED");
	lprint( "Got cred handle %lu\n", client_krb_cred);

#if defined(DEBUG)
	lprint( "Credential details: \n%s\n", dump_cred(client_krb_cred));
#endif

	if (check_status("acquire_cred", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	TRACE ("Calling GSS_RELEASE_OID_SET");

	major_status = gss_release_oid_set(&minor_status,
					   &actual_mech_set);

	TRACE ("Returned from GSS_RELEASE_OID_SET");
					   
	if (check_status("release_oid_set", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	TRACE ("Calling GSS_INQUIRE_CRED");

	major_status = gss_inquire_cred(&minor_status,
					client_krb_cred,
					&iname2,
					&lifetime,
					&cred_usage,
					&mechanisms);

	TRACE ("returned from GSS_INQUIRE_CRED");

	if (check_status("inquire_cred", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};



	TRACE("Calling GSS_DISPLAY_NAME");

	major_status = gss_display_name(&minor_status,
					iname2,
					&buffer1,
					&name_type); 

	TRACE("Returned from GSS_DISPLAY_NAME");

	if (check_status("display_name", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	TRACE("Calling GSS_RELEASE_NAME");

	major_status = gss_release_name(&minor_status,
					&iname2);
	iname2 = GSS_C_NO_NAME;

	TRACE("Returned from GSS_RELEASE_NAME");

	if (check_status("release_name", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	lprint( "   Client Name: \"%.*s\"\n",
		buffer1.length, buffer1.value);

	TRACE("Calling GSS_RELEASE_BUFFER");

	major_status = gss_release_buffer(&minor_status,
					  &buffer1);

	TRACE("Returned from GSS_RELEASE_BUFFER");

	if (check_status("release_buffer", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};



	lprint( "   Expiration (secs): %lu\n", lifetime);

	lprint( "   Mechanism count: %d\n", mechanisms->count);

	if (cred_usage == GSS_C_ACCEPT) {
	    lprint( "   Credential-type: ACCEPT\n");
	} else if (cred_usage == GSS_C_INITIATE) {
	    lprint( "   Credential-type: INITIATE\n");
	} else if (cred_usage == GSS_C_BOTH) {
	    lprint( "   Credential-type:BOTH\n");
	} else {
	    lprint( "   Credential-type: Unknown (%d)\n", cred_usage);
	};

/*****************************************************************************/
	SUBTEST("Init_Sec_Context (KRB client)");

	client_context = GSS_C_NO_CONTEXT;
	client_bindings.initiator_addrtype = GSS_C_AF_NULLADDR;
	client_bindings.initiator_address.value = NULL;
	client_bindings.initiator_address.length = 0;
	client_bindings.acceptor_addrtype = GSS_C_AF_NULLADDR;
	client_bindings.acceptor_address.value = NULL;
	client_bindings.acceptor_address.length = 0;
	client_bindings.application_data.value = NULL;
	client_bindings.application_data.length = 0;

	TRACE ("Calling GSS_INIT_SEC_CONTEXT");


	major_status = gss_init_sec_context(&minor_status,
					    client_krb_cred,
					    &client_context,
					    server_name,
					    GSS_C_NULL_OID,
					    GSS_C_MUTUAL_FLAG,
					    60*60,
					    &client_bindings,
					    &empty_buffer,
					    &actual_mech,
					    &token_1,
					    &client_ret_flags,
					    &time_rec);


	TRACE ("returned from INIT_SEC_CONTEXT");

	if (check_status("init_sec_ctx", major_status, minor_status,
			GSS_S_CONTINUE_NEEDED, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	/*
	 * Intentionally mis-align the returned token...
	 */
	if (! misalign_token_buffer(&token_1, &token_1ua)) {
	    fprintf(stderr,"\n\n Error allocating token_1ua, aborting testing...\n\n");
	    goto finish;
	};


	lprint( "Got ctx handle %lu\n", client_context);

#if defined(DEBUG)
	lprint( "Context details: \n%s\n", dump_context(client_context));
#endif




/*****************************************************************************/
	SUBTEST("Accept_Sec_Context (KRB server)");

	server_context = GSS_C_NO_CONTEXT;
	server_bindings.initiator_addrtype = GSS_C_AF_NULLADDR;
	server_bindings.initiator_address.value = NULL;
	server_bindings.initiator_address.length = 0;
	server_bindings.acceptor_addrtype = GSS_C_AF_NULLADDR;
	server_bindings.acceptor_address.value = NULL;
	server_bindings.acceptor_address.length = 0;
	server_bindings.application_data.value = NULL;
	server_bindings.application_data.length = 0;

	TRACE ("Calling GSS_ACCEPT_SEC_CONTEXT");


	major_status = gss_accept_sec_context(&minor_status,
					      &server_context,
					      server_krb_cred,
					      &token_1ua,
					      &server_bindings,
					      &iname2,
					      &actual_mech,
					      &token_2,
					      &server_ret_flags,
					      &time_rec,
					      &delegated_cred_handle);


	TRACE ("returned from ACCEPT_SEC_CONTEXT");

	if (check_status("accept_sec_ctx", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	TRACE("Calling GSS_RELEASE_BUFFER");

	major_status = gss_release_buffer(&minor_status,
					  &token_1);

	TRACE("Returned from GSS_RELEASE_BUFFER");

	if (check_status("release_buffer", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};

	/* Free misaligned copy */
	free((idl_byte *)token_1ua.value-1);


	lprint( "Got ctx handle %lu\n", server_context);


#if defined(DEBUG)
	lprint( "Context details: \n%s\n", dump_context(server_context));
#endif

	
	EXPORTABLE(server_ret_flags);

	if (iname2 == GSS_C_NO_NAME) {

	    fprintf(stderr, "Error - No client name returned\n");
	    error = 1;

	} else {
	    TRACE("Calling GSS_DISPLAY_NAME");

	    major_status = gss_display_name(&minor_status,
					    iname2,
					    &buffer1,
					    &name_type); 

	    TRACE("Returned from GSS_DISPLAY_NAME");

	    if (check_status("display_name", major_status, minor_status,
			    GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
		fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
		goto finish;
	    };


	    TRACE("Calling GSS_RELEASE_NAME");

	    major_status = gss_release_name(&minor_status,
					    &iname2);

	    TRACE("Returned from GSS_RELEASE_NAME");

	    if (check_status("release_name", major_status, minor_status,
			    GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
		fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
		goto finish;
	    };


	    lprint( "   Client Name: \"%.*s\"\n",
		    buffer1.length, buffer1.value);

	    TRACE("Calling GSS_RELEASE_BUFFER");

	    major_status = gss_release_buffer(&minor_status,
					      &buffer1);

	    TRACE("Returned from GSS_RELEASE_BUFFER");

	    if (check_status("release_buffer", major_status, minor_status,
			    GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
		fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
		goto finish;
	    };

	};

	lprint( "Flags:\n");
	printed_flags = 0;
	if (server_ret_flags & GSS_C_DELEG_FLAG) {
	    lprint( "  Delegation\n");
	    printed_flags = 1;
	};
	if (server_ret_flags & GSS_C_MUTUAL_FLAG) {
	    lprint( "  Mutual\n");
	    printed_flags = 1;
	};
	if (server_ret_flags & GSS_C_REPLAY_FLAG) {
	    lprint( "  Replay detection\n");
	    printed_flags = 1;
	};
	if (server_ret_flags & GSS_C_SEQUENCE_FLAG) {
	    lprint( "  Out-of-sequence detection\n");
	    printed_flags = 1;
	};
	if (server_ret_flags & GSS_C_CONF_FLAG) {
	    lprint( "  Confidentiality (NON-EXPORTABLE!)\n");
	    privacy_supported = 1;
	    printed_flags = 1;
	};
	if (server_ret_flags & GSS_C_INTEG_FLAG) {
	    lprint( "  Integrity\n");
	    printed_flags = 1;
	};


	if (token_2.length == 0) {
	    fprintf(stderr, "Error - no return token\n");
	    error = 1;
	}
	else if (! misalign_token_buffer(&token_2, &token_2ua)) {
	    fprintf(stderr,"\n\n Error allocating token_2ua, aborting testing...\n\n");
	    error = 1;
	}
	else {


/*****************************************************************************/
	    SUBTEST("Init_Sec_Context (client, 2nd time)");

	    TRACE ("Calling GSS_INIT_SEC_CONTEXT");


	    major_status = gss_init_sec_context(&minor_status,
						client_krb_cred,
						&client_context,
						server_name,
						GSS_C_NULL_OID,
						GSS_C_MUTUAL_FLAG,
						60*60,
						&client_bindings,
						&token_2ua,
						&actual_mech,
						&token_3,
						&client_ret_flags,
						&time_rec);


	    TRACE ("returned from INIT_SEC_CONTEXT");
	    lprint( "Got ctx handle %lu\n", client_context);

#if defined(DEBUG)
	    lprint( "Context details: \n%s\n", dump_context(client_context));
#endif

	    if (check_status("init_sec_ctx", major_status, minor_status,
			    GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
		fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
		goto finish;
	    };

	    
	    TRACE("Calling GSS_RELEASE_BUFFER");

	    major_status = gss_release_buffer(&minor_status,
					      &token_2);

	    /* Free misaligned copy */
	    free((idl_byte *)token_2ua.value-1);

	    TRACE("Returned from GSS_RELEASE_BUFFER");

	    if (check_status("release_buffer", major_status, minor_status,
			    GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
		fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
		goto finish;
	    };

	    lprint( "Flags:\n");
	    printed_flags = 0;
	    if (client_ret_flags & GSS_C_DELEG_FLAG) {
		lprint( "  Delegation\n");
		printed_flags = 1;
	    };
	    if (client_ret_flags & GSS_C_MUTUAL_FLAG) {
		lprint( "  Mutual\n");
		printed_flags = 1;
	    };
	    if (client_ret_flags & GSS_C_REPLAY_FLAG) {
		lprint( "  Replay detection\n");
		printed_flags = 1;
	    };
	    if (client_ret_flags & GSS_C_SEQUENCE_FLAG) {
		lprint( "  Out-of-sequence detection\n");
		printed_flags = 1;
	    };
	    if (client_ret_flags & GSS_C_CONF_FLAG) {
		lprint( "  Confidentiality\n");
		privacy_supported = 1;
		printed_flags = 1;
	    };
	    if (client_ret_flags & GSS_C_INTEG_FLAG) {
		lprint( "  Integrity\n");
		printed_flags = 1;
	    };

	    if (client_ret_flags != server_ret_flags) {
		fprintf(stderr, "Client/server flags mismatch\n");
		fprintf(stderr, "Client: %x, Server: %x\n",
			client_ret_flags, server_ret_flags);
		error = 1;
	    };
	    

	    EXPORTABLE(client_ret_flags);
	    if (token_3.length != 0) {
		fprintf(stderr, "Error - Token returned\n");
		error = 1;
	    };

	};

	TRACE ("Calling GSS_RELEASE_OID_SET");

	major_status = gss_release_oid_set(&minor_status,
					   &krb_mechanism_only);

	if (check_status("release_oid_set", major_status, minor_status,
		     GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	TRACE ("Calling GSS_RELEASE_CRED (KRB server)");

	major_status = gss_release_cred(&minor_status, &server_krb_cred);

	if (check_status("release_cred(KRB server)", major_status, minor_status,
		     GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	TRACE ("Calling GSS_RELEASE_CRED (KRB client)");

	major_status = gss_release_cred(&minor_status, &client_krb_cred);

	if (check_status("release_cred(KRB client)", major_status, minor_status,
		     GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};



	if (error) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};



	fflush(stderr);
	lprint(
"\n===========================================================================\n");
#ifdef ENVIRONMENT_TET
	mtet_infoline ("PASSED GSSAPI Kerberos authentication\n");
#endif

    };


    if (test_wanted[TEST_CRED_DCE]) {

	TEST("Credential establishment tests");

	SUBTEST("Establishing Kerberos mechanism credentials");

	TRACE("Calling CREATE_EMPTY_OID_SET ");

	major_status = gssdce_create_empty_oid_set(&minor_status,
						   &krb_mechanism_only);    
	TRACE("Returned from CREATE_EMPTY_OID_SET");

	if (check_status("create_empty_oid_set", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	TRACE ("Calling ADD_OID_SET_MEMBER");

	major_status = gssdce_add_oid_set_member(&minor_status,
						 GSSDCE_C_OID_KRBV5_DES,
						 &krb_mechanism_only);    

	TRACE("Returned from ADD_OID_SET_MEMBER");

	if (check_status("add_oid_set_member", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};



	TRACE ("Calling GSS_ACQUIRE_CRED (KRB server)");

	major_status = gss_acquire_cred(
			    &minor_status,
			    server_name,
			    24*60*60,
			    krb_mechanism_only,
			    GSS_C_ACCEPT,
			    &server_krb_cred,
			    NULL,
			    NULL);

	TRACE ("returned from GSS_ACQUIRE_CRED (KRB server)");

	TRACE ("Calling GSS_ACQUIRE_CRED (KRB client)");

	major_status = gss_acquire_cred(
			    &minor_status,
			    GSS_C_NO_NAME,
			    24*60*60,
			    krb_mechanism_only,
			    GSS_C_INITIATE,
			    &client_krb_cred,
			    &actual_mech_set,
			    &time_rec);

	TRACE ("returned from GSS_ACQUIRE_CRED (KRB client)");



/* Repeat for (default) DCE mechanism					    */
/****************************************************************************/

	SUBTEST("GSS_Acquire_cred (as DCE server)");

	buffer1.length = strlen(server_char_name);
	buffer1.value = (void *)server_char_name;
	    
	TRACE("Calling GSS_IMPORT_NAME");

	major_status = gss_import_name(&minor_status,
				       &buffer1,
				       GSS_C_NULL_OID,
				       &server_name); 
	
	TRACE("Returned from GSS_IMPORT_NAME");

	if (check_status("import_name", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	TRACE ("Calling GSS_ACQUIRE_CRED");

	major_status = gss_acquire_cred(
			    &minor_status,
			    server_name,
			    24*60*60,
			    GSS_C_NULL_OID_SET,
			    GSS_C_ACCEPT,
			    &server_dce_cred,
			    &actual_mech_set,
			    &time_rec);

	TRACE ("returned from GSS_ACQUIRE_CRED");

	lprint( "Got cred handle %lu\n", server_dce_cred);

#if defined(DEBUG)
	lprint( "Credential details: \n%s\n", dump_cred(server_dce_cred));
#endif

	if (check_status("acquire_cred", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	TRACE ("Calling GSS_RELEASE_OID_SET");

	major_status = gss_release_oid_set(&minor_status,
					   &actual_mech_set);

	TRACE ("Returned from GSS_RELEASE_OID_SET");
					   
	TRACE ("Calling GSS_INQUIRE_CRED");

	iname2 = GSS_C_NO_NAME;

	major_status = gss_inquire_cred(&minor_status,
					server_dce_cred,
					&iname2,
					&lifetime,
					&cred_usage,
					&mechanisms);

	TRACE ("returned from GSS_INQUIRE_CRED");

	if (check_status("inquire_cred", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};



	TRACE("Calling GSS_DISPLAY_NAME");

	major_status = gss_display_name(&minor_status,
					iname2,
					&buffer1,
					&name_type); 

	TRACE("Returned from GSS_DISPLAY_NAME");

	if (check_status("display_name", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	lprint( "   Server Name: \"%.*s\"\n",
		buffer1.length, buffer1.value);

	TRACE("Calling GSS_RELEASE_BUFFER");

	major_status = gss_release_buffer(&minor_status,
					  &buffer1);

	TRACE("Returned from GSS_RELEASE_BUFFER");

	if (check_status("release_buffer", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	TRACE("Calling GSS_RELEASE_NAME");

	major_status = gss_release_name(&minor_status,
					&iname2);

	iname2 = GSS_C_NO_NAME;

	TRACE("Returned from GSS_RELEASE_NAME");

	if (check_status("release_name", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};



	lprint( "   Expiration (secs): %lu\n", lifetime);

	lprint( "   Mechanism count: %d\n", mechanisms->count);

	if (cred_usage == GSS_C_ACCEPT) {
	    lprint( "   Credential-type: ACCEPT\n");
	} else if (cred_usage == GSS_C_INITIATE) {
	    lprint( "   Credential-type: INITIATE\n");
	} else if (cred_usage == GSS_C_BOTH) {
	    lprint( "   Credential-type:BOTH\n");
	} else {
	    lprint( "   Credential-type: Unknown (%d)\n", cred_usage);
	};

#ifdef ENVIRONMENT_TET
	mtet_infoline ("PASSED GSSAPI Credential establishment\n");
#endif
    }


/*****************************************************************************/

    if (test_wanted[TEST_DCE]) {    

	TEST("DCE authentication tests");

	SUBTEST("GSS_Acquire_cred (as client)");

	TRACE ("Calling GSS_ACQUIRE_CRED");

	major_status = gss_acquire_cred(
			    &minor_status,
			    GSS_C_NO_NAME,
			    24*60*60,
			    GSS_C_NULL_OID_SET,
			    GSS_C_INITIATE,
			    &client_dce_cred,
			    &actual_mech_set,
			    &time_rec);

	TRACE ("returned from GSS_ACQUIRE_CRED");
	lprint( "Got cred handle %lu\n", client_dce_cred);

#if defined(DEBUG)
	lprint( "Credential details: \n%s\n", dump_cred(client_dce_cred));
#endif

	if (check_status("acquire_cred", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	TRACE ("Calling GSS_RELEASE_OID_SET");

	major_status = gss_release_oid_set(&minor_status,
					   &actual_mech_set);

	TRACE ("Returned from GSS_RELEASE_OID_SET");
					   
	if (check_status("release_oid_set", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	TRACE ("Calling GSS_INQUIRE_CRED");

	major_status = gss_inquire_cred(&minor_status,
					client_dce_cred,
					&iname2,
					&lifetime,
					&cred_usage,
					&mechanisms);

	TRACE ("returned from GSS_INQUIRE_CRED");

	if (check_status("inquire_cred", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};



	TRACE("Calling GSS_DISPLAY_NAME");

	major_status = gss_display_name(&minor_status,
					iname2,
					&buffer1,
					&name_type); 

	TRACE("Returned from GSS_DISPLAY_NAME");

	if (check_status("display_name", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	TRACE("Calling GSS_RELEASE_NAME");

	major_status = gss_release_name(&minor_status,
					&iname2);
	iname2 = GSS_C_NO_NAME;

	TRACE("Returned from GSS_RELEASE_NAME");

	if (check_status("release_name", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	lprint( "   Client Name: \"%.*s\"\n",
		buffer1.length, buffer1.value);

	TRACE("Calling GSS_RELEASE_BUFFER");

	major_status = gss_release_buffer(&minor_status,
					  &buffer1);

	TRACE("Returned from GSS_RELEASE_BUFFER");

	if (check_status("release_buffer", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};



	lprint( "   Expiration (secs): %lu\n", lifetime);

	lprint( "   Mechanism count: %d\n", mechanisms->count);

	if (cred_usage == GSS_C_ACCEPT) {
	    lprint( "   Credential-type: ACCEPT\n");
	} else if (cred_usage == GSS_C_INITIATE) {
	    lprint( "   Credential-type: INITIATE\n");
	} else if (cred_usage == GSS_C_BOTH) {
	    lprint( "   Credential-type:BOTH\n");
	} else {
	    lprint( "   Credential-type: Unknown (%d)\n", cred_usage);
	};

/*****************************************************************************/
	SUBTEST("Init_Sec_Context (DCE client)");

	client_context = GSS_C_NO_CONTEXT;
	client_bindings.initiator_addrtype = GSS_C_AF_NULLADDR;
	client_bindings.initiator_address.value = NULL;
	client_bindings.initiator_address.length = 0;
	client_bindings.acceptor_addrtype = GSS_C_AF_NULLADDR;
	client_bindings.acceptor_address.value = NULL;
	client_bindings.acceptor_address.length = 0;
	client_bindings.application_data.value = NULL;
	client_bindings.application_data.length = 0;

	TRACE ("Calling GSS_INIT_SEC_CONTEXT");


	major_status = gss_init_sec_context(&minor_status,
					    client_dce_cred,
					    &client_context,
					    server_name,
					    GSS_C_NULL_OID,
					    GSS_C_MUTUAL_FLAG,
					    60*60,
					    &client_bindings,
					    &empty_buffer,
					    &actual_mech,
					    &token_1,
					    &client_ret_flags,
					    &time_rec);


	TRACE ("returned from INIT_SEC_CONTEXT");
	lprint( "Got ctx handle %lu\n", client_context);

#if defined(DEBUG)
	lprint( "Context details: \n%s\n", dump_context(client_context));
#endif

	if (check_status("init_sec_ctx", major_status, minor_status,
			GSS_S_CONTINUE_NEEDED, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	/*
	 * Intentionally mis-align the returned token...
	 */
	if (! misalign_token_buffer(&token_1, &token_1ua)) {
	    fprintf(stderr,"\n\n Error allocating token_1ua, aborting testing...\n\n");
	    goto finish;
	};




/*****************************************************************************/
	SUBTEST("Accept_Sec_Context (DCE server)");

	server_context = GSS_C_NO_CONTEXT;
	server_bindings.initiator_addrtype = GSS_C_AF_NULLADDR;
	server_bindings.initiator_address.value = NULL;
	server_bindings.initiator_address.length = 0;
	server_bindings.acceptor_addrtype = GSS_C_AF_NULLADDR;
	server_bindings.acceptor_address.value = NULL;
	server_bindings.acceptor_address.length = 0;
	server_bindings.application_data.value = NULL;
	server_bindings.application_data.length = 0;

	TRACE ("Calling GSS_ACCEPT_SEC_CONTEXT");


	major_status = gss_accept_sec_context(&minor_status,
					      &server_context,
					      server_dce_cred,
					      &token_1ua,
					      &server_bindings,
					      &iname2,
					      &actual_mech,
					      &token_2,
					      &server_ret_flags,
					      &time_rec,
					      &delegated_cred_handle);


	TRACE ("returned from ACCEPT_SEC_CONTEXT");

	if (check_status("accept_sec_ctx", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};

	TRACE("Calling GSS_RELEASE_BUFFER");

	major_status = gss_release_buffer(&minor_status,
					  &token_1);

	TRACE("Returned from GSS_RELEASE_BUFFER");

	if (check_status("release_buffer", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};

	lprint( "Got ctx handle %lu\n", server_context);

	major_status = gss_context_time(&minor_status,
					server_context,
					&time_rec_1);

	if (((time_rec - time_rec_1) > TIME_ERROR) ||
	    ((time_rec - time_rec_1) < 0)) {
	    fprintf(stderr,
		    "Error, discrepancy in context time:\n");
	    fprintf(stderr,
		    "       From accept_sec_context: %lu\n",
		    (long)time_rec);
	    fprintf(stderr,
		    "       From context_time: %lu\n",
		    (long)time_rec_1);
	};


#if defined(DEBUG)
	lprint( "Context details: \n%s\n", dump_context(server_context));
#endif


	EXPORTABLE(server_ret_flags);

	if (iname2 == GSS_C_NO_NAME) {

	    fprintf(stderr, "Error - No client name returned\n");
	    error = 1;

	} else {
	    TRACE("Calling GSS_DISPLAY_NAME");

	    major_status = gss_display_name(&minor_status,
					    iname2,
					    &buffer1,
					    &name_type); 

	    TRACE("Returned from GSS_DISPLAY_NAME");

	    if (check_status("display_name", major_status, minor_status,
			    GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
		fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
		goto finish;
	    };


	    TRACE("Calling GSS_RELEASE_NAME");

	    major_status = gss_release_name(&minor_status,
					    &iname2);

	    TRACE("Returned from GSS_RELEASE_NAME");

	    if (check_status("release_name", major_status, minor_status,
			    GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
		fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
		goto finish;
	    };


	    lprint( "   Client Name: \"%.*s\"\n",
		    buffer1.length, buffer1.value);

	    TRACE("Calling GSS_RELEASE_BUFFER");

	    major_status = gss_release_buffer(&minor_status,
					      &buffer1);

	    TRACE("Returned from GSS_RELEASE_BUFFER");

	    if (check_status("release_buffer", major_status, minor_status,
			    GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
		fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
		goto finish;
	    };

	};

	lprint( "Flags:\n");
	printed_flags = 0;
	if (server_ret_flags & GSS_C_DELEG_FLAG) {
	    lprint( "  Delegation\n");
	    printed_flags = 1;
	};
	if (server_ret_flags & GSS_C_MUTUAL_FLAG) {
	    lprint( "  Mutual\n");
	    printed_flags = 1;
	};
	if (server_ret_flags & GSS_C_REPLAY_FLAG) {
	    lprint( "  Replay detection\n");
	    printed_flags = 1;
	};
	if (server_ret_flags & GSS_C_SEQUENCE_FLAG) {
	    lprint( "  Out-of-sequence detection\n");
	    printed_flags = 1;
	};
	if (server_ret_flags & GSS_C_CONF_FLAG) {
	    lprint( "  Confidentiality\n");
	    privacy_supported = 1;
	    printed_flags = 1;
	};
	if (server_ret_flags & GSS_C_INTEG_FLAG) {
	    lprint( "  Integrity\n");
	    printed_flags = 1;
	};


	if (token_2.length == 0) {
	    fprintf(stderr, "Error - no return token\n");
	    error = 1;
	}
	else if (! misalign_token_buffer(&token_2, &token_2ua)) {
	    fprintf(stderr,"\n\n Error allocating token_2ua, aborting testing...\n\n");
	    error = 1;
	}
	else {

/*****************************************************************************/
	    SUBTEST("Init_Sec_Context (client, 2nd time)");

	    TRACE ("Calling GSS_INIT_SEC_CONTEXT");


	    major_status = gss_init_sec_context(&minor_status,
						client_dce_cred,
						&client_context,
						server_name,
						GSS_C_NULL_OID,
						GSS_C_MUTUAL_FLAG,
						60*60,
						&client_bindings,
						&token_2ua,
						&actual_mech,
						&token_3,
						&client_ret_flags,
						&time_rec);


	    TRACE ("returned from INIT_SEC_CONTEXT");

	    if (check_status("init_sec_ctx", major_status, minor_status,
			    GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
		fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
		goto finish;
	    };

	    TRACE("Calling GSS_RELEASE_BUFFER");

	    major_status = gss_release_buffer(&minor_status,
					      &token_2);

	    /* Free misaligned copy */
	    free((idl_byte *)token_2ua.value-1);

	    TRACE("Returned from GSS_RELEASE_BUFFER");

	    if (check_status("release_buffer", major_status, minor_status,
			    GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
		fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
		goto finish;
	    };

	    lprint( "Got ctx handle %lu\n", client_context);

#if defined(DEBUG)
	    lprint( "Context details: \n%s\n", dump_context(client_context));
#endif


	    

	    lprint( "Flags:\n");
	    printed_flags = 0;
	    if (client_ret_flags & GSS_C_DELEG_FLAG) {
		lprint( "  Delegation\n");
		printed_flags = 1;
	    };
	    if (client_ret_flags & GSS_C_MUTUAL_FLAG) {
		lprint( "  Mutual\n");
		printed_flags = 1;
	    };
	    if (client_ret_flags & GSS_C_REPLAY_FLAG) {
		lprint( "  Replay detection\n");
		printed_flags = 1;
	    };
	    if (client_ret_flags & GSS_C_SEQUENCE_FLAG) {
		lprint( "  Out-of-sequence detection\n");
		printed_flags = 1;
	    };
	    if (client_ret_flags & GSS_C_CONF_FLAG) {
		lprint( "  Confidentiality\n");
		privacy_supported = 1;
		printed_flags = 1;
	    };
	    if (client_ret_flags & GSS_C_INTEG_FLAG) {
		lprint( "  Integrity\n");
		printed_flags = 1;
	    };

	    if (client_ret_flags != server_ret_flags) {
		fprintf(stderr, "Client/server flags mismatch\n");
		fprintf(stderr, "Client: %x, Server: %x\n",
			client_ret_flags, server_ret_flags);
		error = 1;
	    };
	    

	    EXPORTABLE(client_ret_flags);

	    if (token_3.length != 0) {
		fprintf(stderr, "Error - Token returned\n");
		error = 1;
	    };

	};



/*****************************************************************************/

	if (error) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};



	fflush(stderr);
	lprint(
"\n===========================================================================\n");
#ifdef ENVIRONMENT_TET
	mtet_infoline ("PASSED GSSAPI DCE authentication\n");
#endif

    };


/* We already have a DCE context established (client_context and	    */
/* server_context).  Use this to test out the per-message services.	    */
/******************************************************************************/

if (test_wanted[TEST_SIGN]) {

    TEST("GSS_SIGN (integrity protection)");


	buffer1.value = test_data;
	buffer1.length = strlen(test_data);
	buffer2.value = bad_data;
	buffer2.length = strlen(bad_data);

	TRACE ("Calling GSS_SIGN for client packet 1");

	major_status = gss_sign(&minor_status,
				client_context,
				1,
				&buffer1,
				&client_message_1);
	/* Output in client_message_1					    */


	TRACE ("returned from GSS_SIGN");
	
	if (check_status("gss_sign", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};




	TRACE ("Calling GSS_SIGN for client packet 2");

	major_status = gss_sign(&minor_status,
				client_context,
				1,
				&buffer1,
				&client_message_2);
	/* Output in client_message_2					    */


	TRACE ("returned from GSS_SIGN");
	

	TRACE ("Calling GSS_SIGN for client packet 3");

	major_status = gss_sign(&minor_status,
				client_context,
				1,
				&buffer1,
				&client_message_3);
	/* Output in client_message_3					    */


	TRACE ("returned from GSS_SIGN");
	
	if (check_status("gss_sign", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};









	TRACE("Calling server GSS_VERIFY with client message 1");
	
	major_status = gss_verify(&minor_status,
				  server_context,
				  &buffer1,
				  &client_message_1,
				  &message_qop);

	TRACE ("returned from GSS_VERIFY");
	
	if (check_status("gss_verify", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	TRACE ("Calling server GSS_VERIFY with client message 1 again");

	major_status = gss_verify(&minor_status,
				  server_context,
				  &buffer1,
				  &client_message_1,
				  &message_qop);

	TRACE ("returned from GSS_VERIFY");
	
	if (check_status("gss_verify", major_status, minor_status,
GSS_S_COMPLETE | GSS_S_UNSEQ_TOKEN | GSS_S_OLD_TOKEN | GSS_S_DUPLICATE_TOKEN,
			GSSDCE_S_MESSAGE_ALREADY_SEEN)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};



	TRACE ("Calling client GSS_VERIFY with client message 1 ");

	major_status = gss_verify(&minor_status,
				  client_context,
				  &buffer1,
				  &client_message_1,
				  &message_qop);

	TRACE ("returned from GSS_VERIFY");
	
	if (check_status("gss_verify", major_status, minor_status,
		     GSS_S_DEFECTIVE_TOKEN,
		     GSSDCE_S_REFLECTED_PACKET)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};




	TRACE("Calling server GSS_VERIFY with client token 1, bad data");
	
	major_status = gss_verify(&minor_status,
				  server_context,
				  &buffer2,
				  &client_message_1,
				  &message_qop);

	TRACE ("returned from GSS_VERIFY");
	
	if (check_status("gss_verify", major_status, minor_status,
			GSS_S_BAD_SIG, GSSDCE_S_BAD_SIGNATURE)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};






	TRACE("Calling server GSS_VERIFY with client token 3");
	
	major_status = gss_verify(&minor_status,
				  server_context,
				  &buffer1,
				  &client_message_3,
				  &message_qop);

	TRACE ("returned from GSS_VERIFY");
	
	if (check_status("gss_verify", major_status, minor_status,
			GSS_S_COMPLETE | GSS_S_UNSEQ_TOKEN,
			GSSDCE_S_MESSAGE_OMITTED)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};




	TRACE("Calling server GSS_VERIFY with client token 2");
	
	major_status = gss_verify(&minor_status,
				  server_context,
				  &buffer1,
				  &client_message_2,
				  &message_qop);

	TRACE ("returned from GSS_VERIFY");
	
	if (check_status("gss_verify", major_status, minor_status,
		GSS_S_COMPLETE | GSS_S_UNSEQ_TOKEN | GSS_S_OLD_TOKEN,
			GSSDCE_S_MESSAGE_LATE)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};





	TRACE("Calling server GSS_VERIFY with client token 3 again");
	
	major_status = gss_verify(&minor_status,
				  server_context,
				  &buffer1,
				  &client_message_3,
				  &message_qop);

	TRACE ("returned from GSS_VERIFY");
	
	if (check_status("gss_verify", major_status, minor_status,
GSS_S_COMPLETE | GSS_S_UNSEQ_TOKEN | GSS_S_OLD_TOKEN | GSS_S_DUPLICATE_TOKEN,
			GSSDCE_S_MESSAGE_ALREADY_SEEN)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};




	TRACE("Calling server GSS_VERIFY with client token 2 again");
	
	major_status = gss_verify(&minor_status,
				  server_context,
				  &buffer1,
				  &client_message_2,
				  &message_qop);

	TRACE ("returned from GSS_VERIFY");
	
	if (check_status("gss_verify", major_status, minor_status,
GSS_S_COMPLETE | GSS_S_UNSEQ_TOKEN | GSS_S_OLD_TOKEN | GSS_S_DUPLICATE_TOKEN,
			GSSDCE_S_MESSAGE_ALREADY_SEEN)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};



	TRACE("Calling GSS_RELEASE_BUFFER for message 1");

	major_status = gss_release_buffer(&minor_status,
					  &client_message_1);

	TRACE("Returned from GSS_RELEASE_BUFFER");

	check_status("release_buffer", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS);


	TRACE("Calling GSS_RELEASE_BUFFER for message 2");

	major_status = gss_release_buffer(&minor_status,
					  &client_message_2);

	TRACE("Returned from GSS_RELEASE_BUFFER");

	check_status("release_buffer", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS);


	TRACE("Calling GSS_RELEASE_BUFFER for message 3");

	major_status = gss_release_buffer(&minor_status,
					  &client_message_3);

	TRACE("Returned from GSS_RELEASE_BUFFER");

	check_status("release_buffer", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS);

	if (error) {
	    fprintf(stderr,
	    "\n\n Errors detected in GSS_SIGN/VERIFY, aborting testing...\n\n");
	    goto finish;
	};


    fflush(stderr);
    lprint(
"\n===========================================================================\n");
#ifdef ENVIRONMENT_TET
	mtet_infoline ("PASSED GSSAPI Sign service\n");
#endif
};

/******************************************************************************/

if (test_wanted[TEST_SEAL_INT]) {
    TEST("GSS_SEAL (integrity protection)");

	buffer1.value = test_data;
	buffer1.length = strlen(test_data);

	TRACE ("Calling GSS_SEAL for client packet 4");

	conf_state = 2;

	major_status = gss_seal(&minor_status,
				client_context,
				0,  /* No confidentiality */
				1,
				&buffer1,
				&conf_state,
				&client_message_1);
	/* Output in client_message_1					    */


	TRACE ("returned from GSS_SEAL");
	
	if (check_status("gss_seal", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	if (conf_state) {
	    fprintf(stderr, 
		"Conf-state indicates confidentiality, none requested\n");
	    fflush(stderr);
	    error = 1;
	};


	TRACE ("Calling GSS_SEAL for client packet 5");

	major_status = gss_seal(&minor_status,
				client_context,
				0,  /* No confidentiality */
				1,
				&buffer1,
				&conf_state,
				&client_message_2);
	/* Output in client_message_2					    */


	TRACE ("returned from GSS_SEAL");
	
	if (check_status("gss_seal", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	if (conf_state) {
	    fprintf(stderr, 
		"Conf-state indicates confidentiality, none requested\n");
	    fflush(stderr);
	    error = 1;
	};


	TRACE ("Calling GSS_SEAL for client packet 6");

	major_status = gss_seal(&minor_status,
				client_context,
				0,  /* No confidentiality */
				1,
				&buffer1,
				&conf_state,
				&client_message_3);
	/* Output in client_message_3					    */


	TRACE ("returned from GSS_SEAL");
	
	if (check_status("gss_seal", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	if (conf_state) {
	    fprintf(stderr, 
		"Conf-state indicates confidentiality, none requested\n");
	    fflush(stderr);
	    error = 1;
	};








	TRACE("Calling server GSS_UNSEAL with client packet 4");
	
	major_status = gss_unseal(&minor_status,
				  server_context,
				  &client_message_1,
				  &buffer2,
				  &conf_state,  
				  &message_qop);

	TRACE ("returned from GSS_UNSEAL");
	
	if (check_status("gss_unseal", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	if (conf_state) {
	    fprintf(stderr, 
		"Conf-state indicates confidentiality, none requested\n");
	    fflush(stderr);
	    error = 1;
	};

	if (strncmp(buffer2.value, buffer1.value, buffer2.length)) {
	    fprintf(stderr, "Error - unsealed data incorrect:\n");
	    fprintf(stderr, "Original: '%.*s'\n",
		    buffer1.length, buffer1.value);
	    fprintf(stderr, "Unsealed: '%.*s'\n",
		    buffer2.length, buffer2.value);
	};

	TRACE("Calling GSS_RELEASE_BUFFER");

	major_status = gss_release_buffer(&minor_status,
					  &buffer2);

	TRACE("Returned from GSS_RELEASE_BUFFER");


	TRACE ("Calling server GSS_UNSEAL with client message 1 again");

	major_status = gss_unseal(&minor_status,
				  server_context,
				  &client_message_1,
				  &buffer2,
				  &conf_state, 
				  &message_qop);

	TRACE ("returned from GSS_UNSEAL");
	
	if (check_status("gss_unseal", major_status, minor_status,
GSS_S_COMPLETE | GSS_S_UNSEQ_TOKEN | GSS_S_OLD_TOKEN | GSS_S_DUPLICATE_TOKEN,
			GSSDCE_S_MESSAGE_ALREADY_SEEN)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	if (conf_state) {
	    fprintf(stderr, 
		"Conf-state indicates confidentiality, none requested\n");
	    fflush(stderr);
	    error = 1;
	};

	if (strncmp(buffer2.value, buffer1.value, buffer2.length)) {
	    fprintf(stderr, "Error - unsealed data incorrect:\n");
	    fprintf(stderr, "Original: '%.*s'\n",
		    buffer1.length, buffer1.value);
	    fprintf(stderr, "Unsealed: '%.*s'\n",
		    buffer2.length, buffer2.value);
	};

	TRACE("Calling GSS_RELEASE_BUFFER");

	major_status = gss_release_buffer(&minor_status,
					  &buffer2);

	TRACE("Returned from GSS_RELEASE_BUFFER");


	TRACE("Calling server GSS_UNSEAL with client packet 6");
	

	major_status = gss_unseal(&minor_status,
				  server_context,
				  &client_message_3,
				  &buffer2,
				  &conf_state, 
				  &message_qop);

	TRACE ("returned from GSS_UNSEAL");
	
	if (check_status("gss_unseal", major_status, minor_status,
			GSS_S_COMPLETE | GSS_S_UNSEQ_TOKEN,
			GSSDCE_S_MESSAGE_OMITTED)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	if (conf_state) {
	    fprintf(stderr, 
		"Conf-state indicates confidentiality, none requested\n");
	    fflush(stderr);
	    error = 1;
	};

	if (strncmp(buffer2.value, buffer1.value, buffer2.length)) {
	    fprintf(stderr, "Error - unsealed data incorrect:\n");
	    fprintf(stderr, "Original: '%.*s'\n",
		    buffer1.length, buffer1.value);
	    fprintf(stderr, "Unsealed: '%.*s'\n",
		    buffer2.length, buffer2.value);
	};

	TRACE("Calling GSS_RELEASE_BUFFER");

	major_status = gss_release_buffer(&minor_status,
					  &buffer2);

	TRACE("Returned from GSS_RELEASE_BUFFER");




	TRACE("Calling server GSS_UNSEAL with client packet 5");
	
	major_status = gss_unseal(&minor_status,
				  server_context,
				  &client_message_2,
				  &buffer2,
				  &conf_state,  
				  &message_qop);

	TRACE ("returned from GSS_UNSEAL");
	
	if (check_status("gss_unseal", major_status, minor_status,
		GSS_S_COMPLETE | GSS_S_UNSEQ_TOKEN | GSS_S_OLD_TOKEN,
			GSSDCE_S_MESSAGE_LATE)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	if (conf_state) {
	    fprintf(stderr, 
		"Conf-state indicates confidentiality, none requested\n");
	    fflush(stderr);
	    error = 1;
	};

	if (strncmp(buffer2.value, buffer1.value, buffer2.length)) {
	    fprintf(stderr, "Error - unsealed data incorrect:\n");
	    fprintf(stderr, "Original: '%.*s'\n",
		    buffer1.length, buffer1.value);
	    fprintf(stderr, "Unsealed: '%.*s'\n",
		    buffer2.length, buffer2.value);
	};

	TRACE("Calling GSS_RELEASE_BUFFER");

	major_status = gss_release_buffer(&minor_status,
					  &buffer2);

	TRACE("Returned from GSS_RELEASE_BUFFER");





	TRACE("Calling server GSS_UNSEAL with client packet 6");


	major_status = gss_unseal(&minor_status,
				  server_context,
				  &client_message_3,
				  &buffer2,
				  &conf_state,
				  &message_qop);

	TRACE ("returned from GSS_UNSEAL");
	
	if (check_status("gss_unseal", major_status, minor_status,
GSS_S_COMPLETE | GSS_S_UNSEQ_TOKEN | GSS_S_OLD_TOKEN | GSS_S_DUPLICATE_TOKEN,
			GSSDCE_S_MESSAGE_ALREADY_SEEN)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	if (conf_state) {
	    fprintf(stderr, 
		"Conf-state indicates confidentiality, none requested\n");
	    fflush(stderr);
	    error = 1;
	};

	if (strncmp(buffer2.value, buffer1.value, buffer2.length)) {
	    fprintf(stderr, "Error - unsealed data incorrect:\n");
	    fprintf(stderr, "Original: '%.*s'\n",
		    buffer1.length, buffer1.value);
	    fprintf(stderr, "Unsealed: '%.*s'\n",
		    buffer2.length, buffer2.value);
	};

	TRACE("Calling GSS_RELEASE_BUFFER");

	major_status = gss_release_buffer(&minor_status,
					  &buffer2);

	TRACE("Returned from GSS_RELEASE_BUFFER");


	TRACE("Calling server GSS_UNSEAL with client token 2 again");
	
	major_status = gss_unseal(&minor_status,
				  server_context,
				  &client_message_2,
				  &buffer2,
				  &conf_state, 
				  &message_qop);

	TRACE ("returned from GSS_UNSEAL");
	
	if (check_status("gss_unseal", major_status, minor_status,
GSS_S_COMPLETE | GSS_S_UNSEQ_TOKEN | GSS_S_OLD_TOKEN | GSS_S_DUPLICATE_TOKEN,
		    GSSDCE_S_MESSAGE_ALREADY_SEEN)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	if (conf_state) {
	    fprintf(stderr, 
		"Conf-state indicates confidentiality, none requested\n");
	    fflush(stderr);
	    error = 1;
	};

	if (strncmp(buffer2.value, buffer1.value, buffer2.length)) {
	    fprintf(stderr, "Error - unsealed data incorrect:\n");
	    fprintf(stderr, "Original: '%.*s'\n",
		    buffer1.length, buffer1.value);
	    fprintf(stderr, "Unsealed: '%.*s'\n",
		    buffer2.length, buffer2.value);
	};

	TRACE("Calling GSS_RELEASE_BUFFER");

	major_status = gss_release_buffer(&minor_status,
					  &buffer2);

	TRACE("Returned from GSS_RELEASE_BUFFER");


	TRACE("Calling GSS_RELEASE_BUFFER for message 1");

	major_status = gss_release_buffer(&minor_status,
					  &client_message_1);

	TRACE("Returned from GSS_RELEASE_BUFFER");

	check_status("release_buffer", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS);


	TRACE("Calling GSS_RELEASE_BUFFER for message 2");

	major_status = gss_release_buffer(&minor_status,
					  &client_message_2);

	TRACE("Returned from GSS_RELEASE_BUFFER");

	check_status("release_buffer", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS);


	TRACE("Calling GSS_RELEASE_BUFFER for message 3");

	major_status = gss_release_buffer(&minor_status,
					  &client_message_3);

	TRACE("Returned from GSS_RELEASE_BUFFER");

	check_status("release_buffer", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS);


	if (error) {
	    fprintf(stderr,
	    "\n\n Errors detected in GSS_SEAL/UNSEAL, aborting testing...\n\n");
	    goto finish;
	};

    fflush(stderr);
    lprint(
"\n===========================================================================\n");
#ifdef ENVIRONMENT_TET
	mtet_infoline ("PASSED GSSAPI Seal service (non-encrypting)\n");
#endif
};

/******************************************************************************/

if (test_wanted[TEST_SEAL_CONF]) {
    TEST("GSS_SEAL (confidentiality protection)");

    buffer1.value = test_data;
    buffer1.length = strlen(test_data);

    TRACE ("Calling GSS_SEAL for client packet 4");

    conf_state = 2;

    major_status = gss_seal(&minor_status,
			    client_context,
			    1,  /* Yes confidentiality */
			    1,
			    &buffer1,
			    &conf_state,
			    &client_message_1);
    /* Output in client_message_1					    */


    TRACE ("returned from GSS_SEAL");
    
    if (check_status("gss_seal", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };


    if (!conf_state) {
	fprintf(stderr, 
	    "Conf-state indicates no confidentiality, was requested\n");
	if (privacy_supported) {
	    fprintf(stderr,
		    "Context indicated privacy should be available\n");
	    error = 1;
	} else {
	    fprintf(stderr,
		    "This is expected for exportable GSSAPI\n");
	};
	fflush(stderr);
    };


    TRACE ("Calling GSS_SEAL for client packet 5");

    major_status = gss_seal(&minor_status,
			    client_context,
			    1,  /* Yes confidentiality */
			    1,
			    &buffer1,
			    &conf_state,
			    &client_message_2);
    /* Output in client_message_2					    */


    TRACE ("returned from GSS_SEAL");
    
    if (check_status("gss_seal", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };


    if (!conf_state) {
	fprintf(stderr, 
	    "Conf-state indicates no confidentiality, was requested\n");
	if (privacy_supported) {
	    fprintf(stderr,
		    "Context indicated privacy should be available\n");
	    error = 1;
	} else {
	    fprintf(stderr,
		    "This is expected for exportable GSSAPI\n");
	};
	fflush(stderr);
    };


    TRACE ("Calling GSS_SEAL for client packet 6");

    major_status = gss_seal(&minor_status,
			    client_context,
			    1,  /* Yes confidentiality */
			    1,
			    &buffer1,
			    &conf_state,
			    &client_message_3);
    /* Output in client_message_3					    */


    TRACE ("returned from GSS_SEAL");
    
    if (check_status("gss_seal", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };


    if (!conf_state) {
	fprintf(stderr, 
	    "Conf-state indicates no confidentiality, was requested\n");
	if (privacy_supported) {
	    fprintf(stderr,
		    "Context indicated privacy should be available\n");
	    error = 1;
	} else {
	    fprintf(stderr,
		    "This is expected for exportable GSSAPI\n");
	};
	fflush(stderr);
    };








    TRACE("Calling server GSS_UNSEAL with client message 1");
    
    major_status = gss_unseal(&minor_status,
			      server_context,
			      &client_message_1,
			      &buffer2,
			      &conf_state, 
			      &message_qop); 

    TRACE ("returned from GSS_UNSEAL");
    
    if (check_status("gss_unseal", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };


    if (!conf_state) {
	fprintf(stderr, 
	    "Conf-state indicates no confidentiality, was requested\n");
	if (privacy_supported) {
	    fprintf(stderr,
		    "Context indicated privacy should be available\n");
	    error = 1;
	} else {
	    fprintf(stderr,
		    "This is expected for exportable GSSAPI\n");
	};
	fflush(stderr);
    };

    if (strncmp(buffer2.value, buffer1.value, buffer2.length)) {
	fprintf(stderr, "Error - unsealed data incorrect:\n");
	fprintf(stderr, "Original: '%.*s'\n",
		buffer1.length, buffer1.value);
	fprintf(stderr, "Unsealed: '%.*s'\n",
		buffer2.length, buffer2.value);
    };

    TRACE("Calling GSS_RELEASE_BUFFER");

    major_status = gss_release_buffer(&minor_status,
				      &buffer2);

    TRACE("Returned from GSS_RELEASE_BUFFER");


    TRACE ("Calling server GSS_UNSEAL with client message 1 again");

    major_status = gss_unseal(&minor_status,
			      server_context,
			      &client_message_1,
			      &buffer2,
			      &conf_state,
			      &message_qop);

    TRACE ("returned from GSS_UNSEAL");
    
    if (check_status("gss_unseal", major_status, minor_status,
GSS_S_COMPLETE | GSS_S_UNSEQ_TOKEN | GSS_S_OLD_TOKEN | GSS_S_DUPLICATE_TOKEN,
		    GSSDCE_S_MESSAGE_ALREADY_SEEN)) {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };


    if (!conf_state) {
	fprintf(stderr, 
	    "Conf-state indicates no confidentiality, was requested\n");
	if (privacy_supported) {
	    fprintf(stderr,
		    "Context indicated privacy should be available\n");
	    error = 1;
	} else {
	    fprintf(stderr,
		    "This is expected for exportable GSSAPI\n");
	};
	fflush(stderr);
    };

    if (strncmp(buffer2.value, buffer1.value, buffer2.length)) {
	fprintf(stderr, "Error - unsealed data incorrect:\n");
	fprintf(stderr, "Original: '%.*s'\n",
		buffer1.length, buffer1.value);
	fprintf(stderr, "Unsealed: '%.*s'\n",
		buffer2.length, buffer2.value);
    };

    TRACE("Calling GSS_RELEASE_BUFFER");

    major_status = gss_release_buffer(&minor_status,
				      &buffer2);

    TRACE("Returned from GSS_RELEASE_BUFFER");


    TRACE("Calling server GSS_UNSEAL with client token 3");
    

    major_status = gss_unseal(&minor_status,
			      server_context,
			      &client_message_3,
			      &buffer2,
			      &conf_state,
			      &message_qop);

    TRACE ("returned from GSS_UNSEAL");
    
    if (check_status("gss_unseal", major_status, minor_status,
		    GSS_S_COMPLETE | GSS_S_UNSEQ_TOKEN,
		    GSSDCE_S_MESSAGE_OMITTED)) {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };


    if (!conf_state) {
	fprintf(stderr, 
	    "Conf-state indicates no confidentiality, was requested\n");
	if (privacy_supported) {
	    fprintf(stderr,
		    "Context indicated privacy should be available\n");
	    error = 1;
	} else {
	    fprintf(stderr,
		    "This is expected for exportable GSSAPI\n");
	};
	fflush(stderr);
    };

    if (strncmp(buffer2.value, buffer1.value, buffer2.length)) {
	fprintf(stderr, "Error - unsealed data incorrect:\n");
	fprintf(stderr, "Original: '%.*s'\n",
		buffer1.length, buffer1.value);
	fprintf(stderr, "Unsealed: '%.*s'\n",
		buffer2.length, buffer2.value);
    };

    TRACE("Calling GSS_RELEASE_BUFFER");

    major_status = gss_release_buffer(&minor_status,
				      &buffer2);

    TRACE("Returned from GSS_RELEASE_BUFFER");




    TRACE("Calling server GSS_UNSEAL with client token 2");
    
    major_status = gss_unseal(&minor_status,
			      server_context,
			      &client_message_2,
			      &buffer2,
			      &conf_state,
			      &message_qop);

    TRACE ("returned from GSS_UNSEAL");
    
    if (check_status("gss_unseal", major_status, minor_status,
	    GSS_S_COMPLETE | GSS_S_UNSEQ_TOKEN | GSS_S_OLD_TOKEN,
		    GSSDCE_S_MESSAGE_LATE)) {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };


    if (!conf_state) {
	fprintf(stderr, 
	    "Conf-state indicates no confidentiality, was requested\n");
	if (privacy_supported) {
	    fprintf(stderr,
		    "Context indicated privacy should be available\n");
	    error = 1;
	} else {
	    fprintf(stderr,
		    "This is expected for exportable GSSAPI\n");
	};
	fflush(stderr);
    };

    if (strncmp(buffer2.value, buffer1.value, buffer2.length)) {
	fprintf(stderr, "Error - unsealed data incorrect:\n");
	fprintf(stderr, "Original: '%.*s'\n",
		buffer1.length, buffer1.value);
	fprintf(stderr, "Unsealed: '%.*s'\n",
		buffer2.length, buffer2.value);
    };

    TRACE("Calling GSS_RELEASE_BUFFER");

    major_status = gss_release_buffer(&minor_status,
				      &buffer2);

    TRACE("Returned from GSS_RELEASE_BUFFER");





    TRACE("Calling server GSS_UNSEAL with client token 3 again");


    major_status = gss_unseal(&minor_status,
			      server_context,
			      &client_message_3,
			      &buffer2,
			      &conf_state,
			      &message_qop);

    TRACE ("returned from GSS_UNSEAL");
    
    if (check_status("gss_unseal", major_status, minor_status,
GSS_S_COMPLETE | GSS_S_UNSEQ_TOKEN | GSS_S_OLD_TOKEN | GSS_S_DUPLICATE_TOKEN,
		    GSSDCE_S_MESSAGE_ALREADY_SEEN)) {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };


    if (!conf_state) {
	fprintf(stderr, 
	    "Conf-state indicates no confidentiality, was requested\n");
	if (privacy_supported) {
	    fprintf(stderr,
		    "Context indicated privacy should be available\n");
	    error = 1;
	} else {
	    fprintf(stderr,
		    "This is expected for exportable GSSAPI\n");
	};
	fflush(stderr);
    };

    if (strncmp(buffer2.value, buffer1.value, buffer2.length)) {
	fprintf(stderr, "Error - unsealed data incorrect:\n");
	fprintf(stderr, "Original: '%.*s'\n",
		buffer1.length, buffer1.value);
	fprintf(stderr, "Unsealed: '%.*s'\n",
		buffer2.length, buffer2.value);
    };

    TRACE("Calling GSS_RELEASE_BUFFER");

    major_status = gss_release_buffer(&minor_status,
				      &buffer2);

    TRACE("Returned from GSS_RELEASE_BUFFER");


    TRACE("Calling server GSS_UNSEAL with client token 2 again");
    
    major_status = gss_unseal(&minor_status,
			      server_context,
			      &client_message_2,
			      &buffer2,
			      &conf_state,
			      &message_qop);

    TRACE ("returned from GSS_UNSEAL");
    
    if (check_status("gss_unseal", major_status, minor_status,
GSS_S_COMPLETE | GSS_S_UNSEQ_TOKEN | GSS_S_OLD_TOKEN | GSS_S_DUPLICATE_TOKEN,
		GSSDCE_S_MESSAGE_ALREADY_SEEN)) {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };


    if (!conf_state) {
	fprintf(stderr, 
	    "Conf-state indicates no confidentiality, was requested\n");
	if (privacy_supported) {
	    fprintf(stderr,
		    "Context indicated privacy should be available\n");
	    error = 1;
	} else {
	    fprintf(stderr,
		    "This is expected for exportable GSSAPI\n");
	};
	fflush(stderr);
    };

    if (strncmp(buffer2.value, buffer1.value, buffer2.length)) {
	fprintf(stderr, "Error - unsealed data incorrect:\n");
	fprintf(stderr, "Original: '%.*s'\n",
		buffer1.length, buffer1.value);
	fprintf(stderr, "Unsealed: '%.*s'\n",
		buffer2.length, buffer2.value);
    };

    TRACE("Calling GSS_RELEASE_BUFFER");

    major_status = gss_release_buffer(&minor_status,
				      &buffer2);

    TRACE("Returned from GSS_RELEASE_BUFFER");


    TRACE("Calling GSS_RELEASE_BUFFER for message 1");

    major_status = gss_release_buffer(&minor_status,
				      &client_message_1);

    TRACE("Returned from GSS_RELEASE_BUFFER");

    if (check_status("release_buffer", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };



    TRACE("Calling GSS_RELEASE_BUFFER for message 2");

    major_status = gss_release_buffer(&minor_status,
				      &client_message_2);

    TRACE("Returned from GSS_RELEASE_BUFFER");

    if (check_status("release_buffer", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };



    TRACE("Calling GSS_RELEASE_BUFFER for message 3");

    major_status = gss_release_buffer(&minor_status,
				      &client_message_3);

    TRACE("Returned from GSS_RELEASE_BUFFER");

    check_status("release_buffer", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS);


    if (error) {
	fprintf(stderr,
	"\n\n Errors detected in GSS_SEAL/UNSEAL, aborting testing...\n\n");
	goto finish;
    };
#ifdef ENVIRONMENT_TET
	mtet_infoline ("PASSED GSSAPI Seal service (encrypting)\n");
#endif
};




/****************************************************************************/
if (test_wanted[TEST_PAC_EXTRACT]) {
    TEST("PAC extraction");

/* We have an extablished server-side DCE context in server_context.  Try   */
/* to extract a PAC from it...						    */

    major_status = gssdce_extract_PAC_from_sec_context(&minor_status,
						      server_context,
						      &clients_pac);

    check_status("extract_PAC_from_sec_context", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS);



    if (error) {
	fprintf(stderr,
	"\n\n Errors detected in extract_PAC_from_sec_context.  Aborting testing...\n\n");
	goto finish;
    };


    major_status = gssdce_extract_PAC_from_cred(&minor_status,
						client_dce_cred,
						&clients_pac);

    check_status("extract_PAC_from_cred", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS);


    if (error) {
	fprintf(stderr,
	"\n\n Errors detected in extract_PAC_from_cred.  Aborting testing...\n\n");
	goto finish;
    };
#ifdef ENVIRONMENT_TET
	mtet_infoline ("PASSED GSSAPI PAC extraction routines (obsolete)\n");
#endif

};


/****************************************************************************/
if (test_wanted[TEST_DCE_CRED_EXTRACT]) {
    TEST("DCE credentials extraction");

/* We have an extablished server-side DCE context in server_context.  Try   */
/* to extract a set of credentials from it...				    */

    TRACE("Calling gssdce_extract_creds_from_sec_context");

    major_status = gssdce_extract_creds_from_sec_context(&minor_status,
						         server_context,
						         &clients_creds);

    TRACE("Returned from gssdce_extract_creds_from_sec_context");

    check_status("extract_creds_from_sec_context", 
		 major_status, 
		 minor_status,
		 GSS_S_COMPLETE, GSSDCE_S_SUCCESS);

/* Now to verify the credentials.  Just pass it to a few sec_cred_ routines */
/* to make sure they don't barf...					    */

    TRACE("Verifying returned creds by calling sec_cred_get_initiator");

    clients_pa_handle = 
	sec_cred_get_initiator(clients_creds,
			       &error_status);

    TRACE("Returned from sec_cred_get_initiator");

    if (error_status != rpc_s_ok) {
	fprintf(stderr,
	 "sec_cred_get_initiator failed on GSSAPI-derived authz_cred_handle\n");
	fprintf(stderr,
	 "Return status: %X\n", error_status);
	display_dce_error(error_status);
	fflush(stderr);
	error = 1;
    };

    TRACE("Calling sec_cred_iniatialize_attr_cursor");
    sec_cred_initialize_attr_cursor(&pa_cursor, &error_status);
    TRACE("Returned from sec_cred_iniatialize_attr_cursor");

    if (error_status != error_status_ok) {
	fprintf(stderr,
	 "sec_cred_initialize_attr_cursor failed on GSSAPI-derived authz_cred_handle\n");
	fprintf(stderr,
	 "Return status: %X\n", error_status);
	display_dce_error(error_status);
	fflush(stderr);
	error = 1;
    };				    

    TRACE("Calling sec_cred_get_extended_attrs");
    sec_cred_get_extended_attrs(clients_pa_handle,
				&pa_cursor,
				&extended_attr,
				&error_status);
    TRACE("Returned from sec_cred_get_extended_attrs");

    if (error_status != error_status_ok) {
	fprintf(stderr,
	 "sec_cred_get_extended_attrs failed on GSSAPI-derived authz_cred_handle\n");
	fprintf(stderr,
	 "Return status: %X\n", error_status);
	display_dce_error(error_status);
	if (error_status != sec_cred_s_no_more_entries) {
	    error = 1;
	} else {
	    fprintf(stderr, 
		"This error is expected, and does not cause a test failure\n");
	};
	fflush(stderr);
    
    };

    if (error) {
	fprintf(stderr,
	"\n\n Errors detected in DCE credential extraction.  Aborting testing...\n\n");
	goto finish;
    };


#ifdef ENVIRONMENT_TET
	mtet_infoline ("PASSED GSSAPI DCE Credential extraction\n");
#endif
};


/****************************************************************************/
if (test_wanted[TEST_DCE_CRED_TO_LOGIN]) {

    sec_cred_pa_handle_t credentials;
    sec_id_pa_t *cred_data;
    sec_id_t principal;
    
    TEST("Extraction of login-context from credential");

/* Using the client_dce_cred, attempt to extract a login context from it.   */


    TRACE("Calling gssdce_cred_to_login_context");

    major_status = gssdce_cred_to_login_context(&minor_status,
					         client_dce_cred,
					         &client_login_context);

    TRACE("Returned from gssdce_cred_to_login_context");

    check_status("gssdce_cred_to_login_context", 
		 major_status, 
		 minor_status,
		 GSS_S_COMPLETE, GSSDCE_S_SUCCESS);

    if (error) {
	fprintf(stderr,
	"\n\n Errors detected in gssdce_cred_to_login_context, aborting testing...\n\n");
	goto finish;
    };


/* Verify the returned login-context...					    */


/* Extract name from client_login_context...				    */


    credentials = sec_login_cred_get_initiator(client_login_context,
					       &error_status);

    if (error_status != 0 && error_status != sec_login_s_not_certified) {
	fprintf(stderr, "Error - sec_login_cred_get_initiator failed\n");
	display_dce_error(error_status);
	error = 1;
	goto finish;
    };

    cred_data = sec_cred_get_pa_data(credentials, &error_status);

    if (error_status != 0) {
	fprintf(stderr, "Error - sec_cred_get_pa_data failed\n");
	display_dce_error(error_status);
	error = 1;
	goto finish;
    };



#ifdef ENVIRONMENT_TET
	mtet_infoline ("PASSED GSSAPI DCE login-context extraction\n");
#endif
}

/****************************************************************************/
if (test_wanted[TEST_DELEG_REFUSE_IMPERS]) {
    TEST("Delegation functional test 1: Refusal of offered impersonation");
    
/* Create a credential that enables impersonation-style delegation, and	    */
/* demonstrate that it doesn't actually cause impersonation if we set the   */
/* delegate flag to NO.							    */

/* We've already got the client login-context (in <client_login_context>).  */
/* Tweak it to enable impersonation...					    */

    if (client_impersonation_login_context == 0) {
	client_impersonation_login_context =
	    sec_login_become_initiator(client_login_context,
				       sec_id_deleg_type_impersonation,
				       NULL,
				       NULL,
				       NULL,
				       NULL,
/*				       sec_id_compat_mode_initiator, */
				       sec_id_compat_mode_none,
				       &error_status);

	if (error_status) {
	    fprintf(stderr,
		    "Error %x returned from sec_login_become_initiator\n",
	    	    error_status);

	    display_dce_error(error_status);
	    error = 1;

	};

    };    


    if (error) {
	fprintf(stderr,
	"\n\n Errors detected.  Aborting testing...\n\n");
	goto finish;
    };


/* Now we have a login-context capable of supporting impersonation.   Turn  */
/* it into a credential...						    */

    TRACE("Calling gssdce_login_context_to_cred");

    major_status = gssdce_login_context_to_cred(
				    &minor_status,
				    client_impersonation_login_context,
				    GSS_C_INDEFINITE,
				    GSS_C_NULL_OID_SET,
				    &client_impersonation_cred,
				    NULL,
				    NULL);
				    

    TRACE("Returned from login_context_to_cred");

    check_status("gssdce_login_context_to_cred", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS);

    if (error) {
	fprintf(stderr,
	"\n\n Errors detected in gssdce_login_context_to_cred, aborting testing...\n\n");
	goto finish;
    };


/* Attempt to use it to create a non-delegating context, and make sure that */
/* we don't end up delegating.						    */


    client_temp_ctx = GSS_C_NO_CONTEXT;
    client_bindings.initiator_addrtype = GSS_C_AF_NULLADDR;
    client_bindings.initiator_address.value = NULL;
    client_bindings.initiator_address.length = 0;
    client_bindings.acceptor_addrtype = GSS_C_AF_NULLADDR;
    client_bindings.acceptor_address.value = NULL;
    client_bindings.acceptor_address.length = 0;
    client_bindings.application_data.value = NULL;
    client_bindings.application_data.length = 0;

    TRACE ("Calling GSS_INIT_SEC_CONTEXT");


    major_status = gss_init_sec_context(&minor_status,
					client_impersonation_cred,
					&client_temp_ctx,
					server_name,
					GSS_C_NULL_OID,
					GSS_C_MUTUAL_FLAG, /* Deleg=No  */
					60*60,
					&client_bindings,
					&empty_buffer,
					&actual_mech,
					&token_1,
					&client_ret_flags,
					&time_rec);


    TRACE ("returned from INIT_SEC_CONTEXT");
    lprint( "Got ctx handle %lu\n", client_temp_ctx);


    if (check_status("init_sec_ctx", major_status, minor_status,
		    GSS_S_CONTINUE_NEEDED, GSSDCE_S_SUCCESS)) {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };




/*****************************************************************************/

    server_temp_ctx = GSS_C_NO_CONTEXT;
    server_bindings.initiator_addrtype = GSS_C_AF_NULLADDR;
    server_bindings.initiator_address.value = NULL;
    server_bindings.initiator_address.length = 0;
    server_bindings.acceptor_addrtype = GSS_C_AF_NULLADDR;
    server_bindings.acceptor_address.value = NULL;
    server_bindings.acceptor_address.length = 0;
    server_bindings.application_data.value = NULL;
    server_bindings.application_data.length = 0;

    TRACE ("Calling GSS_ACCEPT_SEC_CONTEXT");


    major_status = gss_accept_sec_context(&minor_status,
					  &server_temp_ctx,
					  server_dce_cred,
					  &token_1,
					  &server_bindings,
					  &iname2,
					  &actual_mech,
					  &token_2,
					  &server_ret_flags,
					  &time_rec,
					  &delegated_cred_handle);


    TRACE ("returned from ACCEPT_SEC_CONTEXT");

    if (check_status("accept_sec_ctx", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };

    TRACE("Calling GSS_RELEASE_BUFFER for token.1");

    major_status = gss_release_buffer(&minor_status,
				      &token_1);

    TRACE("Returned from GSS_RELEASE_BUFFER");

    if (check_status("release_buffer", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };

    lprint( "Got ctx handle %lu\n", server_temp_ctx);


    EXPORTABLE(server_ret_flags);

    if (server_ret_flags & GSS_C_DELEG_FLAG) {
	fprintf(stderr,
	    "Error - Delegation occurred, although it was not requested");
	fflush(stderr);
	error = 1;
    };


    if (delegated_cred_handle != GSS_C_NO_CREDENTIAL) {

	fprintf(stderr,
		"Error - delegated credential %d returned\n",
		delegated_cred_handle);
	fflush(stderr);
	error = 1;
    };

    if (iname2 == GSS_C_NO_NAME) {

	fprintf(stderr, "Error - No client name returned\n");
	error = 1;

    } else {
	TRACE("Calling GSS_DISPLAY_NAME");

	major_status = gss_display_name(&minor_status,
					iname2,
					&buffer1,
					&name_type); 

	TRACE("Returned from GSS_DISPLAY_NAME");

	if (check_status("display_name", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	TRACE("Calling GSS_RELEASE_NAME");

	major_status = gss_release_name(&minor_status,
					&iname2);

	TRACE("Returned from GSS_RELEASE_NAME");

	if (check_status("release_name", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	lprint( "   Client Name: \"%.*s\"\n",
		buffer1.length, buffer1.value);

	TRACE("Calling GSS_RELEASE_BUFFER");

	major_status = gss_release_buffer(&minor_status,
					  &buffer1);

	TRACE("Returned from GSS_RELEASE_BUFFER");

	if (check_status("release_buffer", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};

    };

    lprint( "Flags:\n");
    printed_flags = 0;
    if (server_ret_flags & GSS_C_DELEG_FLAG) {
	lprint( "  Delegation\n");
	printed_flags = 1;
    };
    if (server_ret_flags & GSS_C_MUTUAL_FLAG) {
	lprint( "  Mutual\n");
	printed_flags = 1;
    };
    if (server_ret_flags & GSS_C_REPLAY_FLAG) {
	lprint( "  Replay detection\n");
	printed_flags = 1;
    };
    if (server_ret_flags & GSS_C_SEQUENCE_FLAG) {
	lprint( "  Out-of-sequence detection\n");
	printed_flags = 1;
    };
    if (server_ret_flags & GSS_C_CONF_FLAG) {
	lprint( "  Confidentiality\n");
	privacy_supported = 1;
	printed_flags = 1;
    };
    if (server_ret_flags & GSS_C_INTEG_FLAG) {
	lprint( "  Integrity\n");
	printed_flags = 1;
    };


    if (token_2.length == 0) {
	fprintf(stderr, "Error - no return token\n");
	error = 1;
    } else {

/*****************************************************************************/

	TRACE ("Calling GSS_INIT_SEC_CONTEXT again");


	major_status = gss_init_sec_context(&minor_status,
					    client_impersonation_cred,
					    &client_temp_ctx,
					    server_name,
					    GSS_C_NULL_OID,
					    GSS_C_MUTUAL_FLAG,
					    60*60,
					    &client_bindings,
					    &token_2,
					    &actual_mech,
					    &token_3,
					    &client_ret_flags,
					    &time_rec);


	TRACE ("returned from INIT_SEC_CONTEXT");

	if (check_status("init_sec_ctx", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};

	TRACE("Calling GSS_RELEASE_BUFFER for token.2");

	major_status = gss_release_buffer(&minor_status,
					  &token_2);

	TRACE("Returned from GSS_RELEASE_BUFFER");

	if (check_status("release_buffer", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};

	lprint( "Got ctx handle %lu\n", client_temp_ctx);

	lprint( "Flags:\n");
	printed_flags = 0;
	if (client_ret_flags & GSS_C_DELEG_FLAG) {
	    lprint( "  Delegation\n");
	    printed_flags = 1;
	};
	if (client_ret_flags & GSS_C_MUTUAL_FLAG) {
	    lprint( "  Mutual\n");
	    printed_flags = 1;
	};
	if (client_ret_flags & GSS_C_REPLAY_FLAG) {
	    lprint( "  Replay detection\n");
	    printed_flags = 1;
	};
	if (client_ret_flags & GSS_C_SEQUENCE_FLAG) {
	    lprint( "  Out-of-sequence detection\n");
	    printed_flags = 1;
	};
	if (client_ret_flags & GSS_C_CONF_FLAG) {
	    lprint( "  Confidentiality\n");
	    privacy_supported = 1;
	    printed_flags = 1;
	};
	if (client_ret_flags & GSS_C_INTEG_FLAG) {
	    lprint( "  Integrity\n");
	    printed_flags = 1;
	};

	if (client_ret_flags != server_ret_flags) {
	    fprintf(stderr, "Client/server flags mismatch\n");
	    fprintf(stderr, "Client: %x, Server: %x\n",
		    client_ret_flags, server_ret_flags);
	    error = 1;
	};
	

    	EXPORTABLE(client_ret_flags);

	if (token_3.length != 0) {
	    fprintf(stderr, "Error - Token returned\n");
	    error = 1;
	};

    };



/*****************************************************************************/

    if (error) {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };



    major_status = gss_delete_sec_context(&minor_status,
					  &client_temp_ctx,
					  &token_1);

    check_status("delete_sec_context", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS);


    if (token_1.length == 0) {
	fprintf(stderr, "Error - no token returned from delete_sec_ctx\n");
	error = 1;
	goto finish;
    };

    major_status = gss_process_context_token(&minor_status,
					     server_temp_ctx,
					     &token_1);

    check_status("process_context_token", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS);
					     

    major_status = gss_release_buffer(&minor_status,
				      &token_1);

    TRACE("Returned from GSS_RELEASE_BUFFER");

    if (check_status("release_buffer", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };
#ifdef ENVIRONMENT_TET
	mtet_infoline ("PASSED GSSAPI Delegation test 1: Refusing impersonation\n");
#endif

};


/****************************************************************************/
if (test_wanted[TEST_DELEG_REFUSE_TRACED]) {
    TEST("Delegation functional test 2: Refusal of offered traced delegation");
    

/* Create a credential that enables traced delegation, and demonstrate that */
/* it doesn't actually cause delegation if we set the delegate flag to NO.  */

/* We've already got the client login-context (in <client_login_context>).  */
/* Tweak it to enable delegation...					    */

    if (client_traced_login_context == 0) {
	client_traced_login_context =
	    sec_login_become_initiator(client_login_context,
				       sec_id_deleg_type_traced,
				       NULL,
				       NULL,
				       NULL,
				       NULL,
				       sec_id_compat_mode_none,
				       &error_status);

	if (error_status) {
	    fprintf(stderr,
		    "Error %x returned from sec_login_become_initiator\n",
	    	    error_status);

	    display_dce_error(error_status);
	    error = 1;

	};

    };    


    if (error) {
	fprintf(stderr,
	"\n\n Errors detected.  Aborting testing...\n\n");
	goto finish;
    };


/* Now we have a login-context capable of supporting delegation.   Turn it  */
/* into a credential...							    */

    TRACE("Calling gssdce_login_context_to_cred");

    major_status = gssdce_login_context_to_cred(
				    &minor_status,
				    client_traced_login_context,
				    GSS_C_INDEFINITE,
				    GSS_C_NULL_OID_SET,
				    &client_traced_cred,
				    NULL,
				    NULL);
				    

    TRACE("Returned from login_context_to_cred");

    check_status("gssdce_login_context_to_cred", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS);

    if (error) {
	fprintf(stderr,
	"\n\n Errors detected in gssdce_login_context_to_cred, aborting testing...\n\n");
	goto finish;
    };


/* Attempt to use it to create a non-delegating context, and make sure that */
/* we don't end up delegating.						    */


    client_temp_ctx = GSS_C_NO_CONTEXT;
    client_bindings.initiator_addrtype = GSS_C_AF_NULLADDR;
    client_bindings.initiator_address.value = NULL;
    client_bindings.initiator_address.length = 0;
    client_bindings.acceptor_addrtype = GSS_C_AF_NULLADDR;
    client_bindings.acceptor_address.value = NULL;
    client_bindings.acceptor_address.length = 0;
    client_bindings.application_data.value = NULL;
    client_bindings.application_data.length = 0;

    TRACE ("Calling GSS_INIT_SEC_CONTEXT");


    major_status = gss_init_sec_context(&minor_status,
					client_traced_cred,
					&client_temp_ctx,
					server_name,
					GSS_C_NULL_OID,
					GSS_C_MUTUAL_FLAG, /* Deleg=No  */
					60*60,
					&client_bindings,
					&empty_buffer,
					&actual_mech,
					&token_1,
					&client_ret_flags,
					&time_rec);


    TRACE ("returned from INIT_SEC_CONTEXT");
    lprint( "Got ctx handle %lu\n", client_temp_ctx);


    if (check_status("init_sec_ctx", major_status, minor_status,
		    GSS_S_CONTINUE_NEEDED, GSSDCE_S_SUCCESS)) {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };




/*****************************************************************************/

    server_temp_ctx = GSS_C_NO_CONTEXT;
    server_bindings.initiator_addrtype = GSS_C_AF_NULLADDR;
    server_bindings.initiator_address.value = NULL;
    server_bindings.initiator_address.length = 0;
    server_bindings.acceptor_addrtype = GSS_C_AF_NULLADDR;
    server_bindings.acceptor_address.value = NULL;
    server_bindings.acceptor_address.length = 0;
    server_bindings.application_data.value = NULL;
    server_bindings.application_data.length = 0;

    TRACE ("Calling GSS_ACCEPT_SEC_CONTEXT");


    major_status = gss_accept_sec_context(&minor_status,
					  &server_temp_ctx,
					  server_dce_cred,
					  &token_1,
					  &server_bindings,
					  &iname2,
					  &actual_mech,
					  &token_2,
					  &server_ret_flags,
					  &time_rec,
					  &delegated_cred_handle);


    TRACE ("returned from ACCEPT_SEC_CONTEXT");

    if (check_status("accept_sec_ctx", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };

    TRACE("Calling GSS_RELEASE_BUFFER for token.1");

    major_status = gss_release_buffer(&minor_status,
				      &token_1);

    TRACE("Returned from GSS_RELEASE_BUFFER");

    if (check_status("release_buffer", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };

    lprint( "Got ctx handle %lu\n", server_temp_ctx);


    EXPORTABLE(server_ret_flags);

    if (server_ret_flags & GSS_C_DELEG_FLAG) {
	fprintf(stderr,
	    "Error - Delegation occurred, although it was not requested");
	fflush(stderr);
	error = 1;
    };


    if (delegated_cred_handle != GSS_C_NO_CREDENTIAL) {

	fprintf(stderr,
		"Error - delegated credential %d returned\n",
		delegated_cred_handle);
	fflush(stderr);
	error = 1;
    };

    if (iname2 == GSS_C_NO_NAME) {

	fprintf(stderr, "Error - No client name returned\n");
	error = 1;

    } else {
	TRACE("Calling GSS_DISPLAY_NAME");

	major_status = gss_display_name(&minor_status,
					iname2,
					&buffer1,
					&name_type); 

	TRACE("Returned from GSS_DISPLAY_NAME");

	if (check_status("display_name", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	TRACE("Calling GSS_RELEASE_NAME");

	major_status = gss_release_name(&minor_status,
					&iname2);

	TRACE("Returned from GSS_RELEASE_NAME");

	if (check_status("release_name", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	lprint( "   Client Name: \"%.*s\"\n",
		buffer1.length, buffer1.value);

	TRACE("Calling GSS_RELEASE_BUFFER");

	major_status = gss_release_buffer(&minor_status,
					  &buffer1);

	TRACE("Returned from GSS_RELEASE_BUFFER");

	if (check_status("release_buffer", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};

    };

    lprint( "Flags:\n");
    printed_flags = 0;
    if (server_ret_flags & GSS_C_DELEG_FLAG) {
	lprint( "  Delegation\n");
	printed_flags = 1;
    };
    if (server_ret_flags & GSS_C_MUTUAL_FLAG) {
	lprint( "  Mutual\n");
	printed_flags = 1;
    };
    if (server_ret_flags & GSS_C_REPLAY_FLAG) {
	lprint( "  Replay detection\n");
	printed_flags = 1;
    };
    if (server_ret_flags & GSS_C_SEQUENCE_FLAG) {
	lprint( "  Out-of-sequence detection\n");
	printed_flags = 1;
    };
    if (server_ret_flags & GSS_C_CONF_FLAG) {
	lprint( "  Confidentiality\n");
	privacy_supported = 1;
	printed_flags = 1;
    };
    if (server_ret_flags & GSS_C_INTEG_FLAG) {
	lprint( "  Integrity\n");
	printed_flags = 1;
    };


    if (token_2.length == 0) {
	fprintf(stderr, "Error - no return token\n");
	error = 1;
    } else {

/*****************************************************************************/

	TRACE ("Calling GSS_INIT_SEC_CONTEXT again");


	major_status = gss_init_sec_context(&minor_status,
					    client_impersonation_cred,
					    &client_temp_ctx,
					    server_name,
					    GSS_C_NULL_OID,
					    GSS_C_MUTUAL_FLAG,
					    60*60,
					    &client_bindings,
					    &token_2,
					    &actual_mech,
					    &token_3,
					    &client_ret_flags,
					    &time_rec);


	TRACE ("returned from INIT_SEC_CONTEXT");

	if (check_status("init_sec_ctx", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};

	TRACE("Calling GSS_RELEASE_BUFFER for token.2");

	major_status = gss_release_buffer(&minor_status,
					  &token_2);

	TRACE("Returned from GSS_RELEASE_BUFFER");

	if (check_status("release_buffer", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};

	lprint( "Got ctx handle %lu\n", client_temp_ctx);

	lprint( "Flags:\n");
	printed_flags = 0;
	if (client_ret_flags & GSS_C_DELEG_FLAG) {
	    lprint( "  Delegation\n");
	    printed_flags = 1;
	};
	if (client_ret_flags & GSS_C_MUTUAL_FLAG) {
	    lprint( "  Mutual\n");
	    printed_flags = 1;
	};
	if (client_ret_flags & GSS_C_REPLAY_FLAG) {
	    lprint( "  Replay detection\n");
	    printed_flags = 1;
	};
	if (client_ret_flags & GSS_C_SEQUENCE_FLAG) {
	    lprint( "  Out-of-sequence detection\n");
	    printed_flags = 1;
	};
	if (client_ret_flags & GSS_C_CONF_FLAG) {
	    lprint( "  Confidentiality\n");
	    privacy_supported = 1;
	    printed_flags = 1;
	};
	if (client_ret_flags & GSS_C_INTEG_FLAG) {
	    lprint( "  Integrity\n");
	    printed_flags = 1;
	};

	if (client_ret_flags != server_ret_flags) {
	    fprintf(stderr, "Client/server flags mismatch\n");
	    fprintf(stderr, "Client: %x, Server: %x\n",
		    client_ret_flags, server_ret_flags);
	    error = 1;
	};
	

        EXPORTABLE(client_ret_flags);

	if (token_3.length != 0) {
	    fprintf(stderr, "Error - Token returned\n");
	    error = 1;
	};

    };



/*****************************************************************************/

    if (error) {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };



    major_status = gss_delete_sec_context(&minor_status,
					  &client_temp_ctx,
					  &token_1);

    check_status("delete_sec_context", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS);


    if (token_1.length == 0) {
	fprintf(stderr, "Error - no token returned from delete_sec_ctx\n");
	error = 1;
	goto finish;
    };

    major_status = gss_process_context_token(&minor_status,
					     server_temp_ctx,
					     &token_1);

    check_status("process_context_token", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS);
					     

    major_status = gss_release_buffer(&minor_status,
				      &token_1);

    TRACE("Returned from GSS_RELEASE_BUFFER");

    if (check_status("release_buffer", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };
#ifdef ENVIRONMENT_TET
mtet_infoline ("PASSED GSSAPI Delegation test 2: Refusing traced delegation\n");
#endif

};


/****************************************************************************/
if (test_wanted[TEST_DELEG_DO_IMPERS]) {
    TEST("Delegation functional test 3: Acceptance of offered impersonation");


/* Create a credential that enables impersonation, and demonstrate that it  */
/* causes delegation if we set the delegate flag to YES.		    */

/* We've already got the client login-context (in <client_login_context>).  */
/* Tweak it to enable delegation...					    */

    if (client_impersonation_login_context == 0) {
	client_impersonation_login_context =
	    sec_login_become_initiator(client_login_context,
				       sec_id_deleg_type_impersonation,
				       NULL,
				       NULL,
				       NULL,
				       NULL,
				       sec_id_compat_mode_none,
				       &error_status);

	if (error_status) {
	    fprintf(stderr,
		    "Error %x returned from sec_login_become_initiator\n",
	    	    error_status);

	    display_dce_error(error_status);
	    error = 1;

	};

    };    


    if (error) {
	fprintf(stderr,
	"\n\n Errors detected.  Aborting testing...\n\n");
	goto finish;
    };


/* Now we have a login-context capable of supporting delegation.   Turn it  */
/* into a credential...							    */

    TRACE("Calling gssdce_login_context_to_cred");

    major_status = gssdce_login_context_to_cred(
				    &minor_status,
				    client_impersonation_login_context,
				    GSS_C_INDEFINITE,
				    GSS_C_NULL_OID_SET,
				    &client_impersonation_cred,
				    NULL,
				    NULL);
				    

    TRACE("Returned from login_context_to_cred");

    check_status("gssdce_login_context_to_cred", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS);

    if (error) {
	fprintf(stderr,
	"\n\n Errors detected in gssdce_login_context_to_cred, aborting testing...\n\n");
	goto finish;
    };


/* Attempt to use it to create a non-delegating context, and make sure that */
/* we don't end up delegating.						    */


    client_temp_ctx = GSS_C_NO_CONTEXT;
    client_bindings.initiator_addrtype = GSS_C_AF_NULLADDR;
    client_bindings.initiator_address.value = NULL;
    client_bindings.initiator_address.length = 0;
    client_bindings.acceptor_addrtype = GSS_C_AF_NULLADDR;
    client_bindings.acceptor_address.value = NULL;
    client_bindings.acceptor_address.length = 0;
    client_bindings.application_data.value = NULL;
    client_bindings.application_data.length = 0;

    TRACE ("Calling GSS_INIT_SEC_CONTEXT");


    major_status = gss_init_sec_context(&minor_status,
					client_impersonation_cred,
					&client_temp_ctx,
					server_name,
					GSS_C_NULL_OID,
					GSS_C_MUTUAL_FLAG | GSS_C_DELEG_FLAG,
					60*60,
					&client_bindings,
					&empty_buffer,
					&actual_mech,
					&token_1,
					&client_ret_flags,
					&time_rec);


    TRACE ("returned from INIT_SEC_CONTEXT");
    lprint( "Got ctx handle %lu\n", client_temp_ctx);


    if (check_status("init_sec_ctx", major_status, minor_status,
		    GSS_S_CONTINUE_NEEDED, GSSDCE_S_SUCCESS)) {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };




/*****************************************************************************/

    server_temp_ctx = GSS_C_NO_CONTEXT;
    server_bindings.initiator_addrtype = GSS_C_AF_NULLADDR;
    server_bindings.initiator_address.value = NULL;
    server_bindings.initiator_address.length = 0;
    server_bindings.acceptor_addrtype = GSS_C_AF_NULLADDR;
    server_bindings.acceptor_address.value = NULL;
    server_bindings.acceptor_address.length = 0;
    server_bindings.application_data.value = NULL;
    server_bindings.application_data.length = 0;

    TRACE ("Calling GSS_ACCEPT_SEC_CONTEXT");


    major_status = gss_accept_sec_context(&minor_status,
					  &server_temp_ctx,
					  server_dce_cred,
					  &token_1,
					  &server_bindings,
					  &iname2,
					  &actual_mech,
					  &token_2,
					  &server_ret_flags,
					  &time_rec,
					  &delegated_cred_handle);


    TRACE ("returned from ACCEPT_SEC_CONTEXT");

    if (check_status("accept_sec_ctx", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };

    TRACE("Calling GSS_RELEASE_BUFFER for token.1");

    major_status = gss_release_buffer(&minor_status,
				      &token_1);

    TRACE("Returned from GSS_RELEASE_BUFFER");

    if (check_status("release_buffer", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };

    lprint( "Got ctx handle %lu\n", server_temp_ctx);


    EXPORTABLE(server_ret_flags);

    if (server_ret_flags & GSS_C_DELEG_FLAG) {
	lprint( "Delegation occurred as requested\n");
    } else {
	fprintf(stderr,
	    "Error - Delegation occurred, although it was not requested");
	fflush(stderr);
	error = 1;
    };


    if (delegated_cred_handle != GSS_C_NO_CREDENTIAL) {
	lprint(
		"A delegated credential (%d) was returned as expected\n",
		delegated_cred_handle);
    } else {
	fprintf(stderr,
		"Error - delegated credential %d returned\n",
		delegated_cred_handle);
	fflush(stderr);
	error = 1;
    };

    if (iname2 == GSS_C_NO_NAME) {

	fprintf(stderr, "Error - No client name returned\n");
	error = 1;

    } else {
	TRACE("Calling GSS_DISPLAY_NAME");

	major_status = gss_display_name(&minor_status,
					iname2,
					&buffer1,
					&name_type); 

	TRACE("Returned from GSS_DISPLAY_NAME");

	if (check_status("display_name", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	TRACE("Calling GSS_RELEASE_NAME");

	major_status = gss_release_name(&minor_status,
					&iname2);

	TRACE("Returned from GSS_RELEASE_NAME");

	if (check_status("release_name", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	lprint( "   Client Name: \"%.*s\"\n",
		buffer1.length, buffer1.value);

	TRACE("Calling GSS_RELEASE_BUFFER");

	major_status = gss_release_buffer(&minor_status,
					  &buffer1);

	TRACE("Returned from GSS_RELEASE_BUFFER");

	if (check_status("release_buffer", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};

    };

    lprint( "Flags:\n");
    printed_flags = 0;
    if (server_ret_flags & GSS_C_DELEG_FLAG) {
	lprint( "  Delegation\n");
	printed_flags = 1;
    };
    if (server_ret_flags & GSS_C_MUTUAL_FLAG) {
	lprint( "  Mutual\n");
	printed_flags = 1;
    };
    if (server_ret_flags & GSS_C_REPLAY_FLAG) {
	lprint( "  Replay detection\n");
	printed_flags = 1;
    };
    if (server_ret_flags & GSS_C_SEQUENCE_FLAG) {
	lprint( "  Out-of-sequence detection\n");
	printed_flags = 1;
    };
    if (server_ret_flags & GSS_C_CONF_FLAG) {
	lprint( "  Confidentiality\n");
	privacy_supported = 1;
	printed_flags = 1;
    };
    if (server_ret_flags & GSS_C_INTEG_FLAG) {
	lprint( "  Integrity\n");
	printed_flags = 1;
    };


    if (token_2.length == 0) {
	fprintf(stderr, "Error - no return token\n");
	error = 1;
    } else {

/*****************************************************************************/

	TRACE ("Calling GSS_INIT_SEC_CONTEXT again");


	major_status = gss_init_sec_context(&minor_status,
					    client_impersonation_cred,
					    &client_temp_ctx,
					    server_name,
					    GSS_C_NULL_OID,
					    GSS_C_MUTUAL_FLAG | GSS_C_DELEG_FLAG,
					    60*60,
					    &client_bindings,
					    &token_2,
					    &actual_mech,
					    &token_3,
					    &client_ret_flags,
					    &time_rec);


	TRACE ("returned from INIT_SEC_CONTEXT");

	if (check_status("init_sec_ctx", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};

	TRACE("Calling GSS_RELEASE_BUFFER for token.2");

	major_status = gss_release_buffer(&minor_status,
					  &token_2);

	TRACE("Returned from GSS_RELEASE_BUFFER");

	if (check_status("release_buffer", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};

	lprint( "Got ctx handle %lu\n", client_temp_ctx);

	lprint( "Flags:\n");
	printed_flags = 0;
	if (client_ret_flags & GSS_C_DELEG_FLAG) {
	    lprint( "  Delegation\n");
	    printed_flags = 1;
	};
	if (client_ret_flags & GSS_C_MUTUAL_FLAG) {
	    lprint( "  Mutual\n");
	    printed_flags = 1;
	};
	if (client_ret_flags & GSS_C_REPLAY_FLAG) {
	    lprint( "  Replay detection\n");
	    printed_flags = 1;
	};
	if (client_ret_flags & GSS_C_SEQUENCE_FLAG) {
	    lprint( "  Out-of-sequence detection\n");
	    printed_flags = 1;
	};
	if (client_ret_flags & GSS_C_CONF_FLAG) {
	    lprint( "  Confidentiality\n");
	    privacy_supported = 1;
	    printed_flags = 1;
	};
	if (client_ret_flags & GSS_C_INTEG_FLAG) {
	    lprint( "  Integrity\n");
	    printed_flags = 1;
	};

	if (client_ret_flags != server_ret_flags) {
	    fprintf(stderr, "Client/server flags mismatch\n");
	    fprintf(stderr, "Client: %x, Server: %x\n",
		    client_ret_flags, server_ret_flags);
	    error = 1;
	};
	

        EXPORTABLE(client_ret_flags);

	if (token_3.length != 0) {
	    fprintf(stderr, "Error - Token returned\n");
	    error = 1;
	};

    };



/*****************************************************************************/

    if (error) {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };



    major_status = gss_delete_sec_context(&minor_status,
					  &client_temp_ctx,
					  &token_1);

    check_status("delete_sec_context", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS);


    if (token_1.length == 0) {
	fprintf(stderr, "Error - no token returned from delete_sec_ctx\n");
	error = 1;
	goto finish;
    };

    major_status = gss_process_context_token(&minor_status,
					     server_temp_ctx,
					     &token_1);

    check_status("process_context_token", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS);
					     

    major_status = gss_release_buffer(&minor_status,
				      &token_1);

    TRACE("Returned from GSS_RELEASE_BUFFER");

    if (check_status("release_buffer", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };



    TRACE ("Calling GSS_RELEASE_CRED (delegated credential)");

    major_status = gss_release_cred(&minor_status, &delegated_cred_handle);

    if (check_status("release_cred(delegated cred)", major_status, minor_status,
		 GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };


#ifdef ENVIRONMENT_TET
	mtet_infoline ("PASSED GSSAPI Delegation test 3: Accepting impersonation\n");
#endif
};


/****************************************************************************/
if (test_wanted[TEST_DELEG_DO_TRACED]) {
    TEST("Delegation functional test 4: Acceptance of offered traced delegation");



/* Create a credential that enables traced delegation, and demonstrate that */
/* it causes delegation if we set the delegate flag to YES.		    */

/* We've already got the client login-context (in <client_login_context>).  */
/* Tweak it to enable delegation...					    */

    if (client_traced_login_context == 0) {
	client_traced_login_context =
	    sec_login_become_initiator(client_login_context,
				       sec_id_deleg_type_traced,
				       NULL,
				       NULL,
				       NULL,
				       NULL,
				       sec_id_compat_mode_none,
				       &error_status);

	if (error_status) {
	    fprintf(stderr,
		    "Error %x returned from sec_login_become_initiator\n",
	    	    error_status);

	    display_dce_error(error_status);
	    error = 1;

	};

    };    


    if (error) {
	fprintf(stderr,
	"\n\n Errors detected.  Aborting testing...\n\n");
	goto finish;
    };


/* Now we have a login-context capable of supporting delegation.   Turn it  */
/* into a credential...							    */

    TRACE("Calling gssdce_login_context_to_cred");

    major_status = gssdce_login_context_to_cred(
				    &minor_status,
				    client_traced_login_context,
				    GSS_C_INDEFINITE,
				    GSS_C_NULL_OID_SET,
				    &client_traced_cred,
				    NULL,
				    NULL);
				    

    TRACE("Returned from login_context_to_cred");

    check_status("gssdce_login_context_to_cred", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS);

    if (error) {
	fprintf(stderr,
	"\n\n Errors detected in gssdce_login_context_to_cred, aborting testing...\n\n");
	goto finish;
    };


/* Attempt to use it to create a non-delegating context, and make sure that */
/* we don't end up delegating.						    */


    client_temp_ctx = GSS_C_NO_CONTEXT;
    client_bindings.initiator_addrtype = GSS_C_AF_NULLADDR;
    client_bindings.initiator_address.value = NULL;
    client_bindings.initiator_address.length = 0;
    client_bindings.acceptor_addrtype = GSS_C_AF_NULLADDR;
    client_bindings.acceptor_address.value = NULL;
    client_bindings.acceptor_address.length = 0;
    client_bindings.application_data.value = NULL;
    client_bindings.application_data.length = 0;

    TRACE ("Calling GSS_INIT_SEC_CONTEXT");


    major_status = gss_init_sec_context(&minor_status,
					client_traced_cred,
					&client_temp_ctx,
					server_name,
					GSS_C_NULL_OID,
					GSS_C_MUTUAL_FLAG | GSS_C_DELEG_FLAG,
					60*60,
					&client_bindings,
					&empty_buffer,
					&actual_mech,
					&token_1,
					&client_ret_flags,
					&time_rec);


    TRACE ("returned from INIT_SEC_CONTEXT");
    lprint( "Got ctx handle %lu\n", client_temp_ctx);


    if (check_status("init_sec_ctx", major_status, minor_status,
		    GSS_S_CONTINUE_NEEDED, GSSDCE_S_SUCCESS)) {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };




/*****************************************************************************/

    server_temp_ctx = GSS_C_NO_CONTEXT;
    server_bindings.initiator_addrtype = GSS_C_AF_NULLADDR;
    server_bindings.initiator_address.value = NULL;
    server_bindings.initiator_address.length = 0;
    server_bindings.acceptor_addrtype = GSS_C_AF_NULLADDR;
    server_bindings.acceptor_address.value = NULL;
    server_bindings.acceptor_address.length = 0;
    server_bindings.application_data.value = NULL;
    server_bindings.application_data.length = 0;

    TRACE ("Calling GSS_ACCEPT_SEC_CONTEXT");


    major_status = gss_accept_sec_context(&minor_status,
					  &server_temp_ctx,
					  server_dce_cred,
					  &token_1,
					  &server_bindings,
					  &iname2,
					  &actual_mech,
					  &token_2,
					  &server_ret_flags,
					  &time_rec,
					  &delegated_cred_handle);


    TRACE ("returned from ACCEPT_SEC_CONTEXT");

    if (check_status("accept_sec_ctx", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };

    TRACE("Calling GSS_RELEASE_BUFFER for token.1");

    major_status = gss_release_buffer(&minor_status,
				      &token_1);

    TRACE("Returned from GSS_RELEASE_BUFFER");

    if (check_status("release_buffer", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };

    lprint( "Got ctx handle %lu\n", server_temp_ctx);


    EXPORTABLE(server_ret_flags);

    if (server_ret_flags & GSS_C_DELEG_FLAG) {
	lprint( "Delegation occurred as requested\n");
    } else {
	fprintf(stderr,
	    "Error - Delegation occurred, although it was not requested");
	fflush(stderr);
	error = 1;
    };


    if (delegated_cred_handle != GSS_C_NO_CREDENTIAL) {
	lprint(
		"A delegated credential (%d) was returned as expected\n",
		delegated_cred_handle);
    } else {
	fprintf(stderr,
		"Error - delegated credential %d returned\n",
		delegated_cred_handle);
	fflush(stderr);
	error = 1;
    };

    if (iname2 == GSS_C_NO_NAME) {

	fprintf(stderr, "Error - No client name returned\n");
	error = 1;

    } else {
	TRACE("Calling GSS_DISPLAY_NAME");

	major_status = gss_display_name(&minor_status,
					iname2,
					&buffer1,
					&name_type); 

	TRACE("Returned from GSS_DISPLAY_NAME");

	if (check_status("display_name", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	TRACE("Calling GSS_RELEASE_NAME");

	major_status = gss_release_name(&minor_status,
					&iname2);

	TRACE("Returned from GSS_RELEASE_NAME");

	if (check_status("release_name", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	lprint( "   Client Name: \"%.*s\"\n",
		buffer1.length, buffer1.value);

	TRACE("Calling GSS_RELEASE_BUFFER");

	major_status = gss_release_buffer(&minor_status,
					  &buffer1);

	TRACE("Returned from GSS_RELEASE_BUFFER");

	if (check_status("release_buffer", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};

    };

    lprint( "Flags:\n");
    printed_flags = 0;
    if (server_ret_flags & GSS_C_DELEG_FLAG) {
	lprint( "  Delegation\n");
	printed_flags = 1;
    };
    if (server_ret_flags & GSS_C_MUTUAL_FLAG) {
	lprint( "  Mutual\n");
	printed_flags = 1;
    };
    if (server_ret_flags & GSS_C_REPLAY_FLAG) {
	lprint( "  Replay detection\n");
	printed_flags = 1;
    };
    if (server_ret_flags & GSS_C_SEQUENCE_FLAG) {
	lprint( "  Out-of-sequence detection\n");
	printed_flags = 1;
    };
    if (server_ret_flags & GSS_C_CONF_FLAG) {
	lprint( "  Confidentiality\n");
	privacy_supported = 1;
	printed_flags = 1;
    };
    if (server_ret_flags & GSS_C_INTEG_FLAG) {
	lprint( "  Integrity\n");
	printed_flags = 1;
    };


    if (token_2.length == 0) {
	fprintf(stderr, "Error - no return token\n");
	error = 1;
    } else {

/*****************************************************************************/

	TRACE ("Calling GSS_INIT_SEC_CONTEXT again");


	major_status = gss_init_sec_context(&minor_status,
					    client_traced_cred,
					    &client_temp_ctx,
					    server_name,
					    GSS_C_NULL_OID,
					    GSS_C_MUTUAL_FLAG | GSS_C_DELEG_FLAG,
					    60*60,
					    &client_bindings,
					    &token_2,
					    &actual_mech,
					    &token_3,
					    &client_ret_flags,
					    &time_rec);


	TRACE ("returned from INIT_SEC_CONTEXT");

	if (check_status("init_sec_ctx", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};

	TRACE("Calling GSS_RELEASE_BUFFER for token.2");

	major_status = gss_release_buffer(&minor_status,
					  &token_2);

	TRACE("Returned from GSS_RELEASE_BUFFER");

	if (check_status("release_buffer", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};

	lprint( "Got ctx handle %lu\n", client_temp_ctx);

	lprint( "Flags:\n");
	printed_flags = 0;
	if (client_ret_flags & GSS_C_DELEG_FLAG) {
	    lprint( "  Delegation\n");
	    printed_flags = 1;
	};
	if (client_ret_flags & GSS_C_MUTUAL_FLAG) {
	    lprint( "  Mutual\n");
	    printed_flags = 1;
	};
	if (client_ret_flags & GSS_C_REPLAY_FLAG) {
	    lprint( "  Replay detection\n");
	    printed_flags = 1;
	};
	if (client_ret_flags & GSS_C_SEQUENCE_FLAG) {
	    lprint( "  Out-of-sequence detection\n");
	    printed_flags = 1;
	};
	if (client_ret_flags & GSS_C_CONF_FLAG) {
	    lprint( "  Confidentiality\n");
	    privacy_supported = 1;
	    printed_flags = 1;
	};
	if (client_ret_flags & GSS_C_INTEG_FLAG) {
	    lprint( "  Integrity\n");
	    printed_flags = 1;
	};

	if (client_ret_flags != server_ret_flags) {
	    fprintf(stderr, "Client/server flags mismatch\n");
	    fprintf(stderr, "Client: %x, Server: %x\n",
		    client_ret_flags, server_ret_flags);
	    error = 1;
	};
	

    	EXPORTABLE(client_ret_flags);

	if (token_3.length != 0) {
	    fprintf(stderr, "Error - Token returned\n");
	    error = 1;
	};

    };



/*****************************************************************************/

    if (error) {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };



    major_status = gss_delete_sec_context(&minor_status,
					  &client_temp_ctx,
					  &token_1);

    check_status("delete_sec_context", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS);


    if (token_1.length == 0) {
	fprintf(stderr, "Error - no token returned from delete_sec_ctx\n");
	error = 1;
	goto finish;
    };

    major_status = gss_process_context_token(&minor_status,
					     server_temp_ctx,
					     &token_1);

    check_status("process_context_token", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS);
					     

    major_status = gss_release_buffer(&minor_status,
				      &token_1);

    TRACE("Returned from GSS_RELEASE_BUFFER");

    if (check_status("release_buffer", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };


    TRACE ("Calling GSS_RELEASE_CRED (delegated credential)");

    major_status = gss_release_cred(&minor_status, &delegated_cred_handle);

    if (check_status("release_cred(delegated cred)", major_status, minor_status,
		 GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };

#ifdef ENVIRONMENT_TET
	mtet_infoline ("PASSED GSSAPI Delegation test 4: Accepting traced delegation\n");
#endif

};


/****************************************************************************/

if (test_wanted[TEST_DELEG_MAKE_IMPERS]) {
    TEST("Delegation functional test 5: Initiation of impersonation");

/* Attempt to use it to create a delegating context from a non-delegating   */
/* credential, and make sure that we do end up delegating.		    */


    client_temp_ctx = GSS_C_NO_CONTEXT;
    client_bindings.initiator_addrtype = GSS_C_AF_NULLADDR;
    client_bindings.initiator_address.value = NULL;
    client_bindings.initiator_address.length = 0;
    client_bindings.acceptor_addrtype = GSS_C_AF_NULLADDR;
    client_bindings.acceptor_address.value = NULL;
    client_bindings.acceptor_address.length = 0;
    client_bindings.application_data.value = NULL;
    client_bindings.application_data.length = 0;

    TRACE ("Calling GSS_INIT_SEC_CONTEXT");


    major_status = gss_init_sec_context(&minor_status,
					client_dce_cred,
					&client_temp_ctx,
					server_name,
					GSS_C_NULL_OID,
					GSS_C_MUTUAL_FLAG | GSS_C_DELEG_FLAG,
/* Deleg=No  */
					60*60,
					&client_bindings,
					&empty_buffer,
					&actual_mech,
					&token_1,
					&client_ret_flags,
					&time_rec);


    TRACE ("returned from INIT_SEC_CONTEXT");
    lprint( "Got ctx handle %lu\n", client_temp_ctx);


    if (check_status("init_sec_ctx", major_status, minor_status,
		    GSS_S_CONTINUE_NEEDED, GSSDCE_S_SUCCESS)) {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };




/*****************************************************************************/

    server_temp_ctx = GSS_C_NO_CONTEXT;
    server_bindings.initiator_addrtype = GSS_C_AF_NULLADDR;
    server_bindings.initiator_address.value = NULL;
    server_bindings.initiator_address.length = 0;
    server_bindings.acceptor_addrtype = GSS_C_AF_NULLADDR;
    server_bindings.acceptor_address.value = NULL;
    server_bindings.acceptor_address.length = 0;
    server_bindings.application_data.value = NULL;
    server_bindings.application_data.length = 0;

    TRACE ("Calling GSS_ACCEPT_SEC_CONTEXT");


    major_status = gss_accept_sec_context(&minor_status,
					  &server_temp_ctx,
					  server_dce_cred,
					  &token_1,
					  &server_bindings,
					  &iname2,
					  &actual_mech,
					  &token_2,
					  &server_ret_flags,
					  &time_rec,
					  &delegated_cred_handle);


    TRACE ("returned from ACCEPT_SEC_CONTEXT");

    if (check_status("accept_sec_ctx", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };

    TRACE("Calling GSS_RELEASE_BUFFER for token.1");

    major_status = gss_release_buffer(&minor_status,
				      &token_1);

    TRACE("Returned from GSS_RELEASE_BUFFER");

    if (check_status("release_buffer", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };

    lprint( "Got ctx handle %lu\n", server_temp_ctx);


    EXPORTABLE(server_ret_flags);

    if (server_ret_flags & GSS_C_DELEG_FLAG) {
	lprint( "Delegation occurred as requested\n");
    } else {
	fprintf(stderr,
	    "Error - Delegation occurred, although it was not requested");
	fflush(stderr);
	error = 1;
    };


    if (delegated_cred_handle != GSS_C_NO_CREDENTIAL) {
	lprint(
		"A delegated credential (%d) was returned as expected\n",
		delegated_cred_handle);
    } else {
	fprintf(stderr,
		"Error - delegated credential %d returned\n",
		delegated_cred_handle);
	fflush(stderr);
	error = 1;
    };

    if (iname2 == GSS_C_NO_NAME) {

	fprintf(stderr, "Error - No client name returned\n");
	error = 1;

    } else {
	TRACE("Calling GSS_DISPLAY_NAME");

	major_status = gss_display_name(&minor_status,
					iname2,
					&buffer1,
					&name_type); 

	TRACE("Returned from GSS_DISPLAY_NAME");

	if (check_status("display_name", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	TRACE("Calling GSS_RELEASE_NAME");

	major_status = gss_release_name(&minor_status,
					&iname2);

	TRACE("Returned from GSS_RELEASE_NAME");

	if (check_status("release_name", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};


	lprint( "   Client Name: \"%.*s\"\n",
		buffer1.length, buffer1.value);

	TRACE("Calling GSS_RELEASE_BUFFER");

	major_status = gss_release_buffer(&minor_status,
					  &buffer1);

	TRACE("Returned from GSS_RELEASE_BUFFER");

	if (check_status("release_buffer", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};

    };

    lprint( "Flags:\n");
    printed_flags = 0;
    if (server_ret_flags & GSS_C_DELEG_FLAG) {
	lprint( "  Delegation\n");
	printed_flags = 1;
    };
    if (server_ret_flags & GSS_C_MUTUAL_FLAG) {
	lprint( "  Mutual\n");
	printed_flags = 1;
    };
    if (server_ret_flags & GSS_C_REPLAY_FLAG) {
	lprint( "  Replay detection\n");
	printed_flags = 1;
    };
    if (server_ret_flags & GSS_C_SEQUENCE_FLAG) {
	lprint( "  Out-of-sequence detection\n");
	printed_flags = 1;
    };
    if (server_ret_flags & GSS_C_CONF_FLAG) {
	lprint( "  Confidentiality\n");
	privacy_supported = 1;
	printed_flags = 1;
    };
    if (server_ret_flags & GSS_C_INTEG_FLAG) {
	lprint( "  Integrity\n");
	printed_flags = 1;
    };


    if (token_2.length == 0) {
	fprintf(stderr, "Error - no return token\n");
	error = 1;
    } else {

/*****************************************************************************/

	TRACE ("Calling GSS_INIT_SEC_CONTEXT again");


	major_status = gss_init_sec_context(&minor_status,
					    client_traced_cred,
					    &client_temp_ctx,
					    server_name,
					    GSS_C_NULL_OID,
					    GSS_C_MUTUAL_FLAG | GSS_C_DELEG_FLAG,
					    60*60,
					    &client_bindings,
					    &token_2,
					    &actual_mech,
					    &token_3,
					    &client_ret_flags,
					    &time_rec);


	TRACE ("returned from INIT_SEC_CONTEXT");

	if (check_status("init_sec_ctx", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};

	TRACE("Calling GSS_RELEASE_BUFFER for token.2");

	major_status = gss_release_buffer(&minor_status,
					  &token_2);

	TRACE("Returned from GSS_RELEASE_BUFFER");

	if (check_status("release_buffer", major_status, minor_status,
			GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	    fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	    goto finish;
	};

	lprint( "Got ctx handle %lu\n", client_temp_ctx);

	lprint( "Flags:\n");
	printed_flags = 0;
	if (client_ret_flags & GSS_C_DELEG_FLAG) {
	    lprint( "  Delegation\n");
	    printed_flags = 1;
	};
	if (client_ret_flags & GSS_C_MUTUAL_FLAG) {
	    lprint( "  Mutual\n");
	    printed_flags = 1;
	};
	if (client_ret_flags & GSS_C_REPLAY_FLAG) {
	    lprint( "  Replay detection\n");
	    printed_flags = 1;
	};
	if (client_ret_flags & GSS_C_SEQUENCE_FLAG) {
	    lprint( "  Out-of-sequence detection\n");
	    printed_flags = 1;
	};
	if (client_ret_flags & GSS_C_CONF_FLAG) {
	    lprint( "  Confidentiality\n");
	    privacy_supported = 1;
	    printed_flags = 1;
	};
	if (client_ret_flags & GSS_C_INTEG_FLAG) {
	    lprint( "  Integrity\n");
	    printed_flags = 1;
	};

	if (client_ret_flags != server_ret_flags) {
	    fprintf(stderr, "Client/server flags mismatch\n");
	    fprintf(stderr, "Client: %x, Server: %x\n",
		    client_ret_flags, server_ret_flags);
	    error = 1;
	};
	

        EXPORTABLE(client_ret_flags);

	if (token_3.length != 0) {
	    fprintf(stderr, "Error - Token returned\n");
	    error = 1;
	};

    };



/*****************************************************************************/

    if (error) {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };



    major_status = gss_delete_sec_context(&minor_status,
					  &client_temp_ctx,
					  &token_1);

    check_status("delete_sec_context", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS);


    if (token_1.length == 0) {
	fprintf(stderr, "Error - no token returned from delete_sec_ctx\n");
	error = 1;
	goto finish;
    };

    major_status = gss_process_context_token(&minor_status,
					     server_temp_ctx,
					     &token_1);

    check_status("process_context_token", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS);
					     

    major_status = gss_release_buffer(&minor_status,
				      &token_1);

    TRACE("Returned from GSS_RELEASE_BUFFER");

    if (check_status("release_buffer", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };


    TRACE ("Calling GSS_RELEASE_CRED (delegated credential)");

    major_status = gss_release_cred(&minor_status, &delegated_cred_handle);

    if (check_status("release_cred(delegated cred)", major_status, minor_status,
		 GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };

#ifdef ENVIRONMENT_TET
	mtet_infoline ("PASSED GSSAPI Delegation test 5: Constructing impersonation\n");
#endif
};

/****************************************************************************/
if (test_wanted[TEST_DEL_CTX]) {
    TEST("Context deletion");
    

    major_status = gss_delete_sec_context(&minor_status,
					  &server_context,
					  &token_1);

    check_status("delete_sec_context", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS);


    if (token_1.length == 0) {
	fprintf(stderr, "Error - no token returned from delete_sec_ctx\n");
	error = 1;
	goto finish;
    };

    major_status = gss_process_context_token(&minor_status,
					     client_context,
					     &token_1);

    check_status("process_context_token", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS);
					     

    major_status = gss_release_buffer(&minor_status,
				      &token_1);

    TRACE("Returned from GSS_RELEASE_BUFFER");

    if (check_status("release_buffer", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS)) {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };

#ifdef ENVIRONMENT_TET
    mtet_infoline ("PASSED GSSAPI Context deletion\n");
#endif
};


/****************************************************************************/
if (test_wanted[TEST_CRED_MAN]) {
    TEST("Credential management");

    SUBTEST("Creating a BOTH-mode credential from the keytable");

    TRACE ("Calling GSS_ACQUIRE_CRED");

    major_status = gss_acquire_cred(
			&minor_status,
			server_name,
			24*60*60,
			GSS_C_NULL_OID_SET,
			GSS_C_BOTH,
			&dce_man_cred,
			&actual_mech_set,
			&time_rec);

    TRACE ("returned from GSS_ACQUIRE_CRED");

    lprint( "Got cred handle %lu\n", dce_man_cred);

#if defined(DEBUG)
    lprint( "Credential details: \n%s\n", dump_cred(dce_man_cred));
#endif

    if (check_status("acquire_cred", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS))
    {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };


    TRACE ("Calling GSS_RELEASE_OID_SET");

    major_status = gss_release_oid_set(&minor_status,
				       &actual_mech_set);

    TRACE ("Returned from GSS_RELEASE_OID_SET");
				       
    TRACE ("Calling GSS_INQUIRE_CRED");

    iname2 = GSS_C_NO_NAME;

    major_status = gss_inquire_cred(&minor_status,
				    dce_man_cred,
				    &iname2,
				    &lifetime,
				    &cred_usage,
				    &mechanisms);

    TRACE ("returned from GSS_INQUIRE_CRED");

    if (check_status("inquire_cred", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS))
    {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };


    TRACE("Calling GSS_DISPLAY_NAME");

    major_status = gss_display_name(&minor_status,
				    iname2,
				    &buffer1,
				    &name_type); 

    TRACE("Returned from GSS_DISPLAY_NAME");

    if (check_status("display_name", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS))
    {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };


    lprint( "   Server Name: \"%.*s\"\n",
	    buffer1.length, buffer1.value);

    TRACE("Calling GSS_RELEASE_BUFFER");

    major_status = gss_release_buffer(&minor_status,
				      &buffer1);

    TRACE("Returned from GSS_RELEASE_BUFFER");

    if (check_status("release_buffer", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS))
    {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };


    TRACE("Calling GSS_RELEASE_NAME");

    major_status = gss_release_name(&minor_status,
				    &iname2);

    iname2 = GSS_C_NO_NAME;

    TRACE("Returned from GSS_RELEASE_NAME");

    if (check_status("release_name", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS))
    {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };



    lprint( "   Expiration (secs): %lu\n", lifetime);

    lprint( "   Mechanism count: %d\n", mechanisms->count);

    if (cred_usage == GSS_C_ACCEPT)
    {
	lprint( "   Credential-type: ACCEPT\n");
    } else if (cred_usage == GSS_C_INITIATE)
    {
	lprint( "   Credential-type: INITIATE\n");
    } else if (cred_usage == GSS_C_BOTH)
    {
	lprint( "   Credential-type:BOTH\n");
    } else
    {
	lprint( "   Credential-type: Unknown (%d)\n", cred_usage);
    };


    TRACE ("Calling GSS_RELEASE_OID_SET");

    major_status = gss_release_oid_set(&minor_status,
				       &mechanisms);

    TRACE ("Returned from GSS_RELEASE_OID_SET");



    SUBTEST("Extracting login-context from new credential");


    TRACE("Calling gssdce_cred_to_login_context");

    major_status = gssdce_cred_to_login_context(&minor_status,
					        dce_man_cred,
					        &man_login_context);

    TRACE("Returned from gssdce_cred_to_login_context");

    check_status("gssdce_cred_to_login_context", 
		 major_status, 
		 minor_status,
		 GSS_S_COMPLETE, GSSDCE_S_SUCCESS);

    if (error)
    {
	fprintf(stderr,
	"\n\n Errors detected in gssdce_cred_to_login_context, aborting testing...\n\n");
	goto finish;
    };


    lprint( "Got context %lx\n", (long)man_login_context);

    SUBTEST("Verifying newly-extracted login-context");


    if (verify_login_context(man_login_context, &error_status)) {

	fprintf(stderr,
		"\n\n Error %X returned from verify_login_context\n",
		error_status);
	display_dce_error(error_status);
	error = 1;
	fprintf(stderr,
		"\n\n Aborting testing...\n");
	fflush(stderr);
	goto finish;
    };

/* Now we've got a login-context that should not be managed by the	    */
/* credential.  Delete the credential and verify that the login-context is  */
/* still OK.								    */

    SUBTEST("Releasing credential...");
    
    TRACE("Calling gss_release_cred");
    major_status = gss_release_cred(&minor_status, &dce_man_cred);
    TRACE("Returned from gss_release_cred");

    if (check_status("release_cred", major_status, minor_status,
		 GSS_S_COMPLETE, GSSDCE_S_SUCCESS))
    {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };


    SUBTEST("Verifying login-context again...");


    if (verify_login_context(man_login_context, &error_status)) {

	fprintf(stderr,
		"\n\n Error %X returned from verify_login_context\n",
		error_status);
	display_dce_error(error_status);
	error = 1;
	fprintf(stderr,
		"\n\n Aborting testing...\n");
	fflush(stderr);
	goto finish;
    };


    SUBTEST("Deleting extracted context");
    lprint( "Purging login context %lx\n", (long)man_login_context);

    saved_man_login_context = man_login_context;

    TRACE("Calling sec_login_purge_context");
    sec_login_purge_context(&man_login_context,
			    &error_status);
    TRACE("Returned from sec_login_purge_context");

    if (error_status)
    {
	fprintf(stderr,
		"\n\n Error %X returned from sec_login_purge_context\n",
		error_status);
	display_dce_error(error_status);
	error = 1;
	fprintf(stderr,
		"\n\n Aborting testing...\n");
	fflush(stderr);
	goto finish;
    };

#ifndef DONT_CHECK_PURGE
    SUBTEST("Verifying context deletion");

    if (verify_login_context(saved_man_login_context, &error_status)) {

	fprintf(stderr,
		"\n\n Error %X returned from verify_login_context\n",
		error_status);
	display_dce_error(error_status);
	fprintf(stderr,
		"This error is expected, and does not cause a test failure\n");
	fflush(stderr);

    } else {

	fprintf(stderr,
		"\n\n verify_login_context returned %X\n",
		error_status);
	display_dce_error(error_status);
	fprintf(stderr,  "A real error should have occurred\n");
	error = 1;
	fprintf(stderr,
		"\n\n Aborting testing...\n");
	fflush(stderr);
	goto finish;
    };
#endif


/* Now we do it all over again, but this time mark the context as managed   */
/* before we delete the credential...					    */

    SUBTEST("Creating another BOTH-mode credential from the keytable");

    TRACE ("Calling GSS_ACQUIRE_CRED");

    major_status = gss_acquire_cred(
			&minor_status,
			server_name,
			24*60*60,
			GSS_C_NULL_OID_SET,
			GSS_C_BOTH,
			&dce_man_cred,
			&actual_mech_set,
			&time_rec);

    TRACE ("returned from GSS_ACQUIRE_CRED");

    lprint( "Got cred handle %lu\n", dce_man_cred);

#if defined(DEBUG)
    lprint( "Credential details: \n%s\n", dump_cred(dce_man_cred));
#endif

    if (check_status("acquire_cred", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS))
    {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };


    TRACE ("Calling GSS_RELEASE_OID_SET");

    major_status = gss_release_oid_set(&minor_status,
				       &actual_mech_set);

    TRACE ("Returned from GSS_RELEASE_OID_SET");
				       
    TRACE ("Calling GSS_INQUIRE_CRED");

    iname2 = GSS_C_NO_NAME;

    major_status = gss_inquire_cred(&minor_status,
				    dce_man_cred,
				    &iname2,
				    &lifetime,
				    &cred_usage,
				    &mechanisms);

    TRACE ("returned from GSS_INQUIRE_CRED");

    if (check_status("inquire_cred", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS))
    {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };


    TRACE("Calling GSS_DISPLAY_NAME");

    major_status = gss_display_name(&minor_status,
				    iname2,
				    &buffer1,
				    &name_type); 

    TRACE("Returned from GSS_DISPLAY_NAME");

    if (check_status("display_name", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS))
    {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };


    lprint( "   Server Name: \"%.*s\"\n",
	    buffer1.length, buffer1.value);

    TRACE("Calling GSS_RELEASE_BUFFER");

    major_status = gss_release_buffer(&minor_status,
				      &buffer1);

    TRACE("Returned from GSS_RELEASE_BUFFER");

    if (check_status("release_buffer", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS))
    {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };


    TRACE("Calling GSS_RELEASE_NAME");

    major_status = gss_release_name(&minor_status,
				    &iname2);

    iname2 = GSS_C_NO_NAME;

    TRACE("Returned from GSS_RELEASE_NAME");

    if (check_status("release_name", major_status, minor_status,
		    GSS_S_COMPLETE, GSSDCE_S_SUCCESS))
    {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };



    lprint( "   Expiration (secs): %lu\n", lifetime);

    lprint( "   Mechanism count: %d\n", mechanisms->count);

    if (cred_usage == GSS_C_ACCEPT)
    {
	lprint( "   Credential-type: ACCEPT\n");
    } else if (cred_usage == GSS_C_INITIATE)
    {
	lprint( "   Credential-type: INITIATE\n");
    } else if (cred_usage == GSS_C_BOTH)
    {
	lprint( "   Credential-type:BOTH\n");
    } else
    {
	lprint( "   Credential-type: Unknown (%d)\n", cred_usage);
    };

    TRACE ("Calling GSS_RELEASE_OID_SET");

    major_status = gss_release_oid_set(&minor_status,
				       &mechanisms);

    TRACE ("Returned from GSS_RELEASE_OID_SET");


    SUBTEST("Extracting login-context from new credential");


    TRACE("Calling gssdce_cred_to_login_context");

    major_status = gssdce_cred_to_login_context(&minor_status,
					        dce_man_cred,
					        &man_login_context);

    TRACE("Returned from gssdce_cred_to_login_context");

    check_status("gssdce_cred_to_login_context", 
		 major_status, 
		 minor_status,
		 GSS_S_COMPLETE, GSSDCE_S_SUCCESS);

    if (error)
    {
	fprintf(stderr,
	"\n\n Errors detected in gssdce_cred_to_login_context, aborting testing...\n\n");
	goto finish;
    };



    SUBTEST("Verifying newly-extracted login-context");


    if (verify_login_context(man_login_context, &error_status)) {

	fprintf(stderr,
		"\n\n Error %X returned from verify_login_context\n",
		error_status);
	display_dce_error(error_status);
	error = 1;
	fprintf(stderr,
		"\n\n Aborting testing...\n");
	fflush(stderr);
	goto finish;
    };



/* Now we've got a login-context that should not be managed by the	    */
/* credential.  Make it be managed...					    */

    SUBTEST("Setting cred GSSAPI-managed");
    TRACE("Calling gssdce_set_cred_context_ownership");
    major_status = gssdce_set_cred_context_ownership(&minor_status,
						     dce_man_cred,
						     GSSDCE_C_OWNERSHIP_GSSAPI);
    TRACE("Returned from gssdce_set_cred_context_ownership");

    check_status("gssdce_set_cred_context_ownership", 
		 major_status, 
		 minor_status,
		 GSS_S_COMPLETE, GSSDCE_S_SUCCESS);

    if (error)
    {
	fprintf(stderr,
	"\n\n Errors detected in gssdce_set_cred_context_ownership, aborting testing...\n\n");
	goto finish;
    };

/* Delete the credential and verify that the extracted login-context stops  */
/* working too.								    */

    SUBTEST("Releasing credential...");
    
    TRACE("Calling gss_release_cred");
    major_status = gss_release_cred(&minor_status, &dce_man_cred);
    TRACE("Returned from gss_release_cred");

    if (check_status("release_cred", major_status, minor_status,
		 GSS_S_COMPLETE, GSSDCE_S_SUCCESS))
    {
	fprintf(stderr,"\n\n Errors detected, aborting testing...\n\n");
	goto finish;
    };


#ifndef DONT_CHECK_PURGE
    SUBTEST("Verifying login-context again...");


    if (verify_login_context(man_login_context, &error_status)) {

	fprintf(stderr,
		"\n\n Error %X returned from verify_login_context\n",
		error_status);
	display_dce_error(error_status);
	fprintf(stderr,
		"This error is expected, and does not cause a test failure\n");
	fflush(stderr);

    } else {

	fprintf(stderr,
		"\n\n verify_login_context returned %X\n",
		error_status);
	display_dce_error(error_status);
	fprintf(stderr,  "A real error should have occurred\n");
	error = 1;
	fprintf(stderr,
		"\n\n Aborting testing...\n");
	fflush(stderr);
	goto finish;
    };
#endif

#ifdef ENVIRONMENT_TET
	mtet_infoline ("PASSED GSSAPI Credential management\n");
#endif
};


/****************************************************************************/

finish:
    if (error) {
	fprintf(stderr, "\n\nTests completed with error(s)\n");
#ifdef ENVIRONMENT_TET
	mtet_infoline( "FAILED GSSAPI tests\n");
#endif
	return EXIT_FAILURE;
    } else {
	lprint("\n\nAll tests successfully completed\n");
	return EXIT_SUCCESS;
    };


}

#ifdef ENVIRONMENT_TET

static void tp(void) {

    char *argv[128];
    int argc;
    char * progname = "gssapi_test_main";
    char arg_buffer[256];
    char * arglist = NULL;
    char * cp1;
    char * cp2;
    int result;
    
    tet_infoline("Retrieving test-driver argument-list");
    arglist = getenv(GSSAPI_TEST_ARGUMENTS);

    argc = 1;
    argv[0] = progname;

    if (arglist != NULL) {
	tet_infoline("Parsing argument-list");
	strncpy(arg_buffer, arglist, sizeof(arg_buffer));
	arg_buffer[sizeof(arg_buffer)-1] = 0;    
	cp1 = arg_buffer;
		
	while (cp1[0] != 0) {
/* Advance cp1 over leading spaces...					    */
	    while (cp1[0] == ' ') cp1++;
	    if (cp1[0] == 0) continue;
/* Now cp1 points to the start of the next argument...			    */
 	    cp2 = cp1;
/* Advance cp2 over the argument...					    */
	    while (cp2[0] != ' ' && cp2[0] != 0) cp2++;
/* Now cp1 points to the first character of the argument, cp2 points to one */
/* past the end.							    */
	    argv[argc++] = cp1;
	    if (cp2[0] == 0) {
		cp1 = cp2;
	    } else {
		cp1 = cp2+1;
		cp2[0] = 0;
	    };
	};
    } else {
	tet_infoline("No arguments found");
    };

    tet_infoline("Invoking main test program");

    result = loc_main(argc, argv);    

    tet_infoline("Returned from main test program");

    if (result) {
	tet_infoline("Test failed");
	tet_result(TET_FAIL);
    } else {
	tet_infoline("Tests passed");
	tet_result(TET_PASS);
    };
    return;    
}
#endif

/*  DEC/CMS REPLACEMENT HISTORY, Element TEST-GSSAPI.C */
/*  *23   27-SEP-1994 10:50:29 WRAY "" */
/*  *22   16-SEP-1994 11:43:25 WRAY "Add support rtn tests; Beef up some existing tests" */
/*  *21    4-AUG-1994 15:01:14 WRAY "" */
/*  *20    4-AUG-1994 14:49:42 WRAY "test for OT 10895 workaround" */
/*  *19   20-JUL-1994 14:00:16 WRAY "" */
/*  *18    8-JUL-1994 15:08:20 WRAY "GSSAPI BL3" */
/*  *17   22-MAR-1994 09:38:34 WRAY "" */
/*  *16   21-MAR-1994 09:23:46 WRAY "Expand message catalog testing" */
/*  *15   18-MAR-1994 15:48:20 WRAY "Fix -t flag" */
/*  *14   18-MAR-1994 13:54:41 WRAY "For BL2" */
/*  *13    8-MAR-1994 15:21:57 WRAY "" */
/*  *12   18-FEB-1994 17:18:48 WRAY "" */
/*  *11   18-FEB-1994 14:33:12 WRAY "" */
/*  *10   20-JAN-1994 12:24:19 WRAY "cms sho elem" */
/*  *9    17-JAN-1994 12:35:32 WRAY "" */
/*  *8    18-NOV-1993 15:30:14 WRAY "" */
/*  *7    18-NOV-1993 14:41:58 WRAY "Fix exportability testing" */
/*  *6    15-NOV-1993 16:08:56 WRAY "" */
/*  *5    11-NOV-1993 14:21:57 WRAY "Add test for exportability" */
/*  *4     9-NOV-1993 15:28:45 WRAY "Add server registration; Enable non-privileged use" */
/*  *3    16-MAR-1993 11:34:59 WRAY "Improvements to UI" */
/*  *2     2-MAR-1993 11:32:05 WRAY "Added principal name params & copyright notice" */
/*  *1     1-MAR-1993 10:35:45 WRAY "Test harness for DCE/GSSAPI" */
/*  DEC/CMS REPLACEMENT HISTORY, Element TEST-GSSAPI.C */
