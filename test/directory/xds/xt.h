/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: xt.h,v $
 * Revision 1.1.12.2  1996/02/18  23:04:03  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:48  marty]
 *
 * Revision 1.1.12.1  1995/12/08  22:13:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:45:39  root]
 * 
 * Revision 1.1.10.4  1994/07/06  15:09:33  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:26:21  marrek]
 * 
 * Revision 1.1.10.3  1994/06/21  14:28:42  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  07:47:11  marrek]
 * 
 * Revision 1.1.10.2  1994/03/23  16:06:35  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  18:12:40  keutel]
 * 
 * Revision 1.1.10.1  1994/02/22  19:15:36  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/07  13:28:13  marrek]
 * 
 * Revision 1.1.8.1  1993/10/14  19:28:44  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  19:27:04  keutel]
 * 
 * 	October 1993 code drop
 * 	[1993/10/11  12:54:17  keutel]
 * 
 * Revision 1.1.6.2  1993/08/12  12:18:15  marrek
 * 	July 1993 code drop.
 * 	[1993/08/03  12:25:20  marrek]
 * 
 * Revision 1.1.4.3  1993/01/05  20:34:04  htf
 * 	Embedding copyright notice
 * 	[1993/01/05  18:48:14  htf]
 * 
 * Revision 1.1.4.2  1992/12/01  14:58:51  marrek
 * 	November 1992 code drop
 * 	[1992/12/01  14:53:15  marrek]
 * 
 * Revision 1.1.2.5  1992/07/14  19:45:29  bmw
 * 	fix syntax errors
 * 	[1992/07/14  19:44:20  bmw]
 * 
 * Revision 1.1.2.4  1992/07/06  16:37:11  melman
 * 	Added dce/ in includes of d2dir.h and d27util.h
 * 	[1992/07/06  16:36:43  melman]
 * 
 * Revision 1.1.2.3  1992/07/01  16:54:11  melman
 * 	GDS drop of 6/26/92
 * 	[1992/07/01  16:24:14  melman]
 * 
 * Revision 1.1.2.2  1992/06/11  20:00:15  melman
 * 	 gds drop
 * 	[1992/06/11  19:23:04  melman]
 * 
 * $EndLog$
 */
#ifndef _XT_H
#define _XT_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xt_rcsid[] = "@(#)$RCSfile: xt.h,v $ $Revision: 1.1.12.2 $ $Date: 1996/02/18 23:04:03 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/

/****************************************************************************/
/*                                                                          */
/*   Defines for the format of the test file.                               */
/*                                                                          */
/****************************************************************************/



#include <stdlib.h>
#include <string.h>
#include <xom.h>
#include <xomip.h>
#include <d2dir.h>
#include <xdsiapl.h>
#include <xds.h>
#include <xdsgds.h>
#include <xdscds.h>
#include <xdsbdcp.h>
#include <xdsmdup.h>
#include <xdsdme.h>
#include <xdssap.h>
#include <xmhp.h>
#include <xmsga.h>
#include <d27util.h>
#include <assert.h>
#include <limits.h> /* for LONG_MAX and LONG_MIN */


/* version 4 without dump, with abandon */
#ifdef THREADSAFE
	#define XT_VERSION    "4.0t"
#else
	#define XT_VERSION    "4.0"
#endif /* THREADSAFE */

#define XT_MAXLINE		255	/* Max length of a line in Test file.*/

#define XT_FMT_FUNCMAX		32	/* Max length of a function name     */

/* max string length for XOM string support */
#define XT_MAX_XOMS_STRING 1024
					
#define XT_DSANAME_MAX		10

#define	XT_FUNCTION	 		1
#define	XT_INCLUDE	 		2

#define XT_FMT_FUNC			"%8s%s\n"

#define	XT_FUNCTION_TOKEN	"Function"
#define	XT_INCLUDE_TOKEN	"Include"

#define XT_DEF_SESSION_TOKEN	"default_session"
#define XT_DEF_LOG_FNAME	"xt_test.log"
#define XT_DEF_ALL_FNAME	"Testcases"
#define XT_DEF_NO_ITERATIONS	"1"
#define XT_DEF_TIMESLICE	"-1" /* should be out or range of MIN .. MAX */
#define XT_MIN_TIMESLICE	"1"		/* us */
#define XT_MAX_TIMESLICE	"999999"
#define XT_MIN_DELAY		"0"
#define XT_MAX_DELAY		"100000000"

/****************************************************************************/
/*                                                                          */
/*   Variables holding the above defined format strings.                    */
/*                                                                          */
/****************************************************************************/

					/*************************************/
					/* General XT defines -          */
					/*************************************/
#define XT_OK			 1
#define XT_ERROR		-1
#define XT_PARAMETER	 0
#define XT_RETURN_VAL	 1
#define XT_PASSED		 2
#define XT_FAILED		-2
#define XT_EOF			-3
#define XT_NOT_MATCHED	-4
#define XT_FILE_ERROR 	-5
#define XT_END_INPUT 	-6
#define XT_NOT_CALLED 	-7

#define XT_RUNTYPE_THREAD 96
#define XT_RUNTYPE_NONE	  97		/* Type of run as per the cmdline -  */	
#define XT_RUNTYPE_ALL	  98
#define XT_RUNTYPE_ONE	  99

#define	ONE		1		/* number of OM_attributes expected  */
#define	ZERO_OR_ONE	2
#define	ZERO_OR_MORE	3
#define	ONE_OR_MORE	4
#define	ONE_TO_SIX	5
#define	ZERO_TO_TWO	6

#define XT_LEN_TESTNAME 80		/* max length of test name */

#define UNKNOWN_SYNTAX	0
#define	NULL_ATTRIBUTE	{OM_NO_MORE_TYPES, 0, 0, 0, NULL, NULL }

#define ATTRIBUTE_OBJ_ID_1	"\x55"
#define ATTRIBUTE_OBJ_ID_2	"\x04"

#define IS_NULL_ATTRIBUTE(at) 						\
	(((at.att_type==OM_NO_MORE_TYPES)&&                             	\
	  (at.number_allowed==0)&&                                      	\
	  (at.r_number_found==0)&&                                      	\
	  (at.e_number_found==0))?1:0)

#define IS_OM_NULL_DESCRIPTOR(dc) 					\
	(((dc.type==OM_NO_MORE_TYPES)&&					\
	  (dc.syntax==OM_S_NO_MORE_SYNTAXES))?1:0)
				
#define XT_LOG_ERROR(string) xt_err(function_name,string)

char	*xt_itoa(OM_sint);
char	*xt_ptoa (char *p);

#ifdef ATT
	#define index(chr,str)	strchr(chr,str)
#endif

#ifdef TALLOC
	char		*vmalloc(size_t size);
	#define		malloc(a)	vmalloc(a)
#endif

typedef OM_sint BOOLEAN; 

					/*************************************/
					/* Global variables -            */
					/*************************************/
extern OM_sint	xt_indent;		/* Left margin indent for writing.  */
					/* Entry name read from Auto file.  */
extern OM_sint	xt_ptronly;		/* Print ptr value only, not length */
					/* or contents.                     */

extern char	xt_entry[XT_FMT_FUNCMAX+1];

extern OM_sint xt_runtype; /* type of run: -i for individual, 
						-a for sequential, -t for threads */

extern BOOLEAN xt_do_to_string_and_back; /* for testing convenience
			library - all objects are converted to strings and then
			back to objects if command line flag is set */

extern BOOLEAN xt_strict_to_string_and_back; /* flag to indicate 
			whether failure of xt_to_string_and_back
			should cause failure of test. When it is TRUE,
			then the test will fail on failure of xt_to_string_and_back */
			
extern BOOLEAN xt_verbose;
extern OM_sint xt_num_failures;


struct parm_tbl_struct {
    const char *parm;	/* string corresponding to one of the parms  */
    const OM_sint	index; 	/* index of this parameter in the associated */
				/* parameter table 			     */
    const OM_sint	type;	/* the parameter type			     */
    OM_descriptor	**table;/* the table in which to find this parameter */
};

struct class_tbl_struct {
    char	*class_str;	/* string corresponding to a class           */
    OM_string	*class_type;	/* the class type			     */
};

struct tree_parm_struct {
    char	*entry_type;	/* string corresponding to an entry type     */
    OM_sint		number;		/* number of these entries		     */
};


typedef struct attribute_struct {
	OM_type		att_type;
	OM_sint		r_number_found;
	OM_sint		e_number_found;
	OM_sint		number_allowed;
	OM_sint		*r_index;
	OM_sint		*e_index;
} attribute;


enum f_type {XT_COPY,    XT_CPYVAL,  XT_CREATE,  XT_DECODE,  XT_DELETE,
			 XT_ENCODE,  XT_GET, XT_INSTAN,  XT_PUT, XT_OMREAD,
			 XT_REMOVE,  XT_WRITE,   XT_ABANDN,  XT_ADDENT,  XT_BIND,
			 XT_COMPAR,  XT_INIT,    XT_LIST,    XT_MODENT,  XT_MODRDN,
			 XT_DSREAD,  XT_RECRES,  XT_RMENT,   XT_SEARCH,  XT_SHUTDN,
			 XT_UNBIND,  XT_VERSN,   XT_BLDTREE, XT_RMTREE,  XT_FILL,
			 XT_FILLO,   XT_EXTRACT, XT_EXTRACTA, XT_S_TO_O, XT_O_TO_S };

typedef struct jump_struct {
	char     entname[32];	/* entry name in testcase file	     */
	enum f_type  type;		/* type of function to be invoked    */
	OM_sint          (*test_func)(void);	/* test function		     */
} jump;

extern struct jump_struct jmptbl[];


#define	XT_SESSION			1
#define	XT_CONTEXT			2
#define	XT_NAME				3
#define	XT_ENTRY			4
#define	XT_AVA				5
#define	XT_CHANGES			6
#define	XT_SELECTION		9
#define	XT_SUBSET			10
#define	XT_FILTER			11
#define	XT_COMPARE_RESULT	13
#define	XT_READ_RESULT		14
#define	XT_SEARCH_RESULT	15
#define	XT_LIST_RESULT		16
#define	XT_STATUS			17
#define	XT_FEATURE_LIST		18
#define	XT_BOUND_SESSION	19
#define	XT_WORKSPACE		20
#define	XT_CLASS			21
#define	XT_BOOLEAN			23
#define	XT_RETURN_CODE		24
#define	XT_PRIVATE_OBJECT	25
#define	XT_PUBLIC_OBJECT	26
#define	XT_OBJECT			27
#define	XT_TYPE				28
#define	XT_VALUE_POSITION	29
#define	XT_EXCLUSIONS		30
#define	XT_MODIFICATION		31
#define	XT_STRING			32
#define	XT_SYNTAX			33
#define	XT_TYPE_LIST		34
#define	XT_ELEMENT_POSITION	35
#define	XT_RDN				36
#define	XT_RULES			37
#define	XT_BDCP		        38
#define	XT_XRETURN_CODE		39 /* extended XOM return code */
#define XT_ATTR_TYPE		40 /* attribute type */
#define XT_NAV_PATH			41 /* navigation path */
#define XT_ALG_ID			42 /* algorithm ID */
#define XT_CERTIFICATE		43
#define XT_CERTIFICATE_LIST	44
#define XT_CERTIFICATE_PAIR	45
#define XT_CERTIFICATE_SUBLIST	46
#define XT_SIGNATURE		47
#define XT_SAP				48


/****************************************************************************/
/*                                                                          */
/****************************************************************************/

/* xt_run.c */
		int xt_main(int argc, char *argv[]);
		OM_sint xt_run(void);
		struct jump_struct *xt_sbyname(char *name);
		OM_sint xt_build_ava(OM_object ava, char value[], OM_sint num);
		OM_sint xt_build_rdn(OM_object rdn, OM_object ava);
		OM_sint xt_build_name(OM_descriptor name[], OM_object rdns[], OM_sint count, OM_sint first);
		OM_sint xt_build_attribute_list(OM_object att_list, char *type);

/* xt_rValue.c */
		OM_sint		xt_cmp_string(OM_string, OM_string);
		OM_sint		xt_cmp_boolean(OM_boolean, OM_boolean);
		OM_sint		xt_cmp_status(OM_private_object, OM_descriptor *, enum f_type);
		OM_sint		xt_cmp_enum(OM_enumeration, OM_enumeration);
		OM_sint		xt_cmp_integer(OM_sint, OM_sint);
		OM_sint		xt_cmp_class(OM_object_identifier, OM_object_identifier);
		OM_sint		xt_cmp_read_result(OM_private_object, OM_descriptor *);
		OM_sint		xt_cmp_list_result(OM_private_object, OM_descriptor *);
		OM_sint		xt_cmp_session(OM_private_object, OM_descriptor *);
		OM_sint		xt_cmp_attribute_value(OM_public_object, OM_public_object);
		OM_sint		xt_cmp_compare_result(OM_private_object, OM_descriptor *);
		OM_sint		xt_cmp_search_result(OM_private_object, OM_descriptor *);
		OM_sint		xt_cmp_system_error(OM_public_object, OM_public_object);
		OM_sint		xt_cmp_update_error(OM_public_object, OM_public_object);	
		OM_sint		xt_cmp_library_error(OM_public_object, OM_public_object);	
		OM_sint		xt_cmp_communications_error(OM_public_object, OM_public_object);
		OM_sint		xt_cmp_attribute_error(OM_public_object, OM_public_object);
		OM_sint		xt_cmp_name_error(OM_public_object, OM_public_object);
		OM_sint		xt_cmp_referral(OM_public_object, OM_public_object);
		OM_sint		xt_cmp_security_error(OM_public_object, OM_public_object);
		OM_sint		xt_cmp_service_error(OM_public_object, OM_public_object);
		OM_sint		xt_cmp_abandon_error(OM_public_object, OM_public_object);
		OM_sint		xt_cmp_return_code(OM_uint, OM_uint);
	

/* xt_omFuncts.c */
		OM_sint xt_om_copy(void);
		OM_sint xt_om_copy_value(void);
		OM_sint xt_om_create(void); 
		OM_sint xt_om_decode(void);
		OM_sint xt_om_delete(void);
		OM_sint xt_om_encode(void);
		OM_sint xt_om_get(void);
		OM_sint xt_om_instance(void);
		OM_sint xt_om_put(void); 
		OM_sint xt_om_read(void);
		OM_sint xt_om_remove(void);
		OM_sint xt_om_write(void);
		char *xt_om_error_to_string(const OM_sint i);



/* xt_dsFuncs.c */
		OM_sint xt_ds_abandon(void);
		OM_sint xt_ds_add_entry(void);
		OM_sint xt_ds_bind(void);
		OM_sint xt_ds_compare(void);
		OM_sint xt_ds_initialize(void);
		OM_sint xt_ds_list(void); 
		OM_sint xt_ds_modify_entry(void);
		OM_sint xt_ds_modify_rdn(void);
		OM_sint xt_ds_read(void); 
		OM_sint xt_ds_receive_result(void);
		OM_sint xt_ds_remove_entry(void);
		OM_sint xt_ds_search(void); 
		OM_sint xt_ds_shutdown(void);
		OM_sint xt_ds_unbind(void);
		OM_sint xt_ds_version(void);
		OM_sint xt_build_tree(void);
		OM_sint xt_remove_tree(void);
		OM_sint xt_omX_fill(void);
		OM_sint xt_omX_fill_oid(void);
		OM_sint xt_omX_extract(void);
		OM_sint xt_dsX_extract_attr_values(void); 
		OM_sint xt_omX_string_to_object(void);
		OM_sint xt_omX_object_to_string(void);
		OM_return_code xt_to_string_and_back(OM_object *pobject,
										const char *message);

/* xt_file.c */
		OM_sint xt_init(OM_sint argc, char *argv[]);
		OM_sint xt_openo(void);
		OM_sint xt_opena(void);
		OM_sint xt_closea(void);
		OM_sint xt_opent(void);
		OM_sint xt_closet(void);
		OM_sint xt_openp(const char *filename);
		OM_sint xt_closep(void);
		OM_sint xt_rtest(void);
		OM_sint xt_rtest_till_last(void);
		OM_sint xt_read_parm ( OM_sint  parm_type, OM_sint  table_type,
		OM_sint  *r_table_index, 
		char *r_value,  OM_sint  *r_void );
		OM_sint xt_read_object ( OM_sint  parm_type, OM_sint *table_type,
		OM_sint  *r_table_index, char *r_value,
		OM_sint  *r_void,    OM_descriptor **object );
		OM_sint xt_read_private_object ( OM_sint parm_type,
		OM_sint table_type,
		OM_sint *r_table_index,
		char *r_value,
		OM_sint *r_void,
		OM_private_object *private_object );
		OM_sint xt_read_public_object ( OM_sint parm_type,
		OM_sint table_type,
		OM_sint *r_table_index,
		char *r_value,
		OM_sint *r_void,
		OM_public_object *public_object );
		OM_sint xt_read_string (char *r_value);
		OM_sint xt_read_int (OM_sint *value);
		OM_sint xt_read_equals (char **r_str, OM_sint *r_val);
      	OM_sint xt_read_quoted_string (char *return_string);
		OM_sint xt_fnext(void);
		void xt_err   (const char *func, const char *msg);
		void xt_err1  (const char *func, const char *msg, const char *p1);
		void xt_err2  (const char *func, const char *msg, const char *p1, const char *p2);
		void xt_errx  (const char *func, const char *msg);
		void xt_errx1 (const char *func, const char *msg, const char *p1);
		void xt_errx2 (const char *func, const char *msg, const char *p1, const char *p2);
		void xt_perrx (const char *func, const char *msg);
		void xt_exit  (OM_sint exitc);
		void xt_log   (const char *msg);
		void xt_log_nocrlf	(const char *msg);
		void xt_log_cat		(const char *msg);
		void xt_log_newline	(void);
		void xt_log1  (const char *msg, const char *p1);
		void xt_log2  (const char *msg, const char *p1, const char *p2);
		void xt_log_head(const char *);
		void xt_log_default_context(void);
		void xt_log_default_session(void);
		OM_sint xt_log_func(const char *);
		void xt_log_pass(const char *);
		void xt_log_fail(const char *);
		void xt_log_both_nocrlf(const char *msg);
		void xt_log_both_cat(const char *msg);
		void xt_log_both_newline(void);
		void xt_log_both(const char *msg);
		void xt_log_both1(const char *msg, const char *p1);
		void xt_log_both2(const char *msg, const char *p1, const char *p2);
		OM_sint xt_usage(char *pgmname);
		OM_sint xt_dinit(void);

#ifdef THREADSAFE
/* xt_thread.c */
		void    xt_initialize_thread_data(void);
		OM_sint xt_thread_name_set( const char *name, const OM_sint thread_number);
		void    xt_thread_init(void);
		void    xt_thread_deinit(void);
		struct thread_data_struct *xt_this_thread_data(void);
		OM_sint xt_threads_created(void);
		char    *xt_this_thread_name(void);
		char    *xt_this_thread_id_and_name(void);
		void    xt_thread_fail(void);
		void    xt_thread_pass(void);
		OM_sint xt_thread_create( const OM_sint thread_number, OM_sint (*function)(void), const char name[]);
		void    xt_threads_start(void);
		void    xt_threads_join(void);
		void	xt_check_string_tbl(void);

#else
/* xt_nonthread.c */
#endif /* THREADSAFE */
		void    xt_initialize_global_data(void);
		void    xt_nonthread_init(void);

#endif /* _XT_H */

