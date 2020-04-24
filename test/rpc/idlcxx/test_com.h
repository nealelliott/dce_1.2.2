/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: test_com.h,v $
 * Revision 1.1.4.2  1996/09/10  18:21:15  bfc
 * 	define STDC for TET/C++
 * 	[1996/09/10  18:14:27  bfc]
 *
 * Revision 1.1.4.1  1996/09/09  20:59:29  bfc
 * 	finish TETizing and cleanup
 * 	[1996/09/09  19:48:20  bfc]
 * 
 * Revision 1.1.2.2  1996/03/09  20:50:24  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:28:32  marty]
 * 
 * Revision 1.1.2.1  1995/12/11  19:36:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:19:08  root]
 * 
 * $EndLog$
 */
#ifdef __cplusplus
    extern "C" {
#endif

extern int client_aux(
#ifdef __cplusplus
...
#endif
);

#ifdef _AIX
#define setenv(a,b,c) /* Not supported - similar API is putenv */
#endif

#if defined(VMS) || defined(__VMS)
#    include <dce/pthread_exc.h>    /* needs to come in before stdlib */
#endif

#ifdef _MSDOS
#define main _CRTAPI1 main
#define setenv(a,b,c) /* Not supported */
#define random rand
#include <fcntl.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <io.h>
#define O_RDONLY _O_RDONLY
#define O_CREAT _O_CREAT
#define O_TRUNC _O_TRUNC
#define O_WRONLY _O_WRONLY
#ifndef __cplusplus
#define open _open
#define close _close
#define write _write
#define read _read
#endif
#ifndef DCENT
#define rpc_ss_allocate MIDL_user_allocate
#define rpc_ss_free MIDL_user_free
#endif /* DCENT */
#else
#   if defined(VMS) || defined(__VMS)
#	pragma nostandard
#       include <file.h>
#   else
#       include <sys/file.h>
#   endif
#endif

#if defined(_MSDOS) && !defined(DCENT)
# include <rpc.h>
# include <rpcdcep.h>
# include <rpcndr.h>
# include "dceport.h"
# define error_text(st) "No error text available"
# define globalref extern
# define globaldef
#else
# include <dce/rpc.h>
# include <dce/rpcexc.h>
# ifdef _MSDOS
#    include <dce/pthread_.h>
# else
#    include <dce/pthread_exc.h>
# endif
#endif
#include <dce/dce_error.h>

#if defined(VMS) || defined(__VMS)
#pragma standard
static int optind;
static char *optarg;
#include <descrip.h>
extern LIB$SET_LOGICAL(
#ifdef __cplusplus
...
#endif
);
#define random rand
#define setenv(name,val,dummy) {\
        $DESCRIPTOR(logical,(name));\
        $DESCRIPTOR(value,"");\
        value.dsc$a_pointer = (val);\
        value.dsc$w_length = strlen(value.dsc$a_pointer);\
        LIB$SET_LOGICAL(&logical,&value);}
#endif /* VMS */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(__alpha) && defined(__VMS) 
#undef ndr_hyper_int
#define ndr_hyper_int      struct ndr_hyper_int_rep
#undef ndr_uhyper_int
#define ndr_uhyper_int     struct ndr_uhyper_int_rep
#endif
#if defined(__alpha) && !defined(VMS) && !defined(__VMS)
#define INTEGER64
#define INTEGER64_LE    /* little endian */
#endif

#ifdef INTEGER64_LE
#undef ndr_hyper_int
/*struct ndr_hyper_int_rep   {ndr_ulong_int low; ndr_long_int high;};*/
#define ndr_hyper_int      struct ndr_hyper_int_rep
#undef ndr_uhyper_int
/*struct ndr_uhyper_int_rep  {ndr_ulong_int low; ndr_ulong_int high;};*/
#define ndr_uhyper_int     struct ndr_uhyper_int_rep
#endif

#if !defined(VMS) && !defined(__alpha) && !defined(AIX) && !defined(_AIX)
#define TEST_SM_ROUTINES
#endif

extern idl_byte *get_interface();

#if 0
extern dce_error_inq_text(
#ifdef __cplusplus
...
#endif
);
#endif

#define FAIL(text,var) {fprintf(stderr,"***FAILURE***: %s\n",text);(var)++;}
#define FLOAT_EQUAL(x,y) (((x)-(y)) < 1e-5)
#define DOUBLE_EQUAL(x,y) (((x)-(y)) < 1e-5)

#ifdef DEBUG
#define print_exception(e) exc_report(e)
#else
#define print_exception(e) RERAISE
#endif

#ifndef IDL_ENTRY
# define IDL_ENTRY 
#endif
#ifndef IDL_ALLOC_ENTRY
# define IDL_ALLOC_ENTRY 
#endif

static char *error_text
#ifdef IDL_PROTOTYPES
(error_status_t st)
#else
(st) error_status_t st;
#endif
{
    char *buf;
    error_status_t rst=0;

    buf = (char*)malloc(1000);
#if defined(_MSDOS) && !defined(DCENT)
    strcpy(buf,"No Message Text available");
#else
    dce_error_inq_text(st, (unsigned char *) buf, (int *) &rst);
#endif
    return buf;
}


#define RESUME 0
#define ABORT  1

#define CHECK_STATUS(input_status, comment, action) \
{ \
  if (input_status != rpc_s_ok) { \
    dce_error_inq_text(input_status, error_string, &error_stat); \
    fprintf(stderr, "%s %s\n", comment, error_string); \
    if (action == ABORT) \
      exit(1); \
  } \
}

static int error_stat;
static unsigned char error_string[dce_c_error_string_len];

/* macro to read a binding handle from the binding.dat file */
#define READ_BINDING_FROM_FILE(bh) { 					\
	FILE		*fp; 						\
	unsigned_char_t	*s; 						\
	char           	prot[100], host[100], endp[100], id[100]; 	\
	unsigned32	st; 						\
	int		eof;						\
									\
	fp = fopen("binding.dat", "r"); 				\
	while (1) {							\
	  if (fscanf(fp, "%s %s %s", prot, host, endp) == EOF)		\
	    CHECK_STATUS(1, "Can't read binding:", ABORT); 		\
	  rpc_string_binding_compose(NULL, 				\
		(unsigned_char_t *) prot,    				\
		(unsigned_char_t *) host,    				\
		(unsigned_char_t *) endp,    				\
		NULL,                        				\
		&s, 							\
		&st							\
	  ); 								\
	  if (st != rpc_s_ok) continue;					\
	  CHECK_STATUS(st, "Can't compose string binding:", ABORT); 	\
	  rpc_binding_from_string_binding(s, bh, &st); 			\
	  if (st != rpc_s_ok) continue;					\
	  CHECK_STATUS(st,"Can't convert string binding to binding handle",st);\
	  rpc_string_free(&s, &st); 					\
	  CHECK_STATUS(st, "Can't free string binding handle", st); 	\
	  break;							\
	}								\
	fclose(fp); 							\
}

#ifdef TET_ENV

#ifdef __cplusplus
/* define STDC for the purposes of tet_api.h */
#ifndef AIX32
#define __STDC__ 1
#endif
extern "C" {
#endif
#include <tet_api.h>
#ifdef __cplusplus
}
#endif

#define GSSAPI_TEST_ARGUMENTS "GSSAPI_TEST_ARGUMENTS"

extern int loc_main(int, char *[]);    

static void startup(void) {}
static void cleanup(void) {}
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

void (*tet_startup)() = startup;
void (*tet_cleanup)() = cleanup;

struct tet_testlist tet_testlist[] = {
	{tp, 1},
	{NULL, 0}
};

#endif

#ifdef __cplusplus
    }
#endif
