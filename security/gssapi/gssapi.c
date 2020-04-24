/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: gssapi.c,v $
 * Revision 1.1.9.2  1996/10/03  14:49:39  arvind
 * 	changes to silence compiler warnings.
 * 	[1996/09/16  23:34 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.9.1  1996/06/04  21:58:05  arvind
 * 	Merge u2u change for new sec_krb_get_cred() args from mb_u2u
 * 	[1996/05/06  20:50 UTC  burati  /main/DCE_1.2/2]
 * 
 * 	merge u2u work
 * 	[1996/04/29  21:54 UTC  burati  /main/DCE_1.2/mb_u2u/1]
 * 
 * 	u2u work off Mothra, before 1.2 branch is available
 * 	[1996/04/29  21:32 UTC  burati  /main/HPDCE02/mb_mothra8/1]
 * 
 * 	merge from the private branch. OT 13090.
 * 	[1995/10/03  15:45 UTC  nav  /main/HPDCE02/5]
 * 
 * 	fix for OT 13090. gss_unseal and gss_verify do not use conf_state and qop_state parameters
 * 	properly. Include the diffs from the OT.
 * 	[1995/09/29  16:26 UTC  nav  /main/HPDCE02/nav_mothra_3/1]
 * 
 * 	Change dce_svc_printf's to DCE_SVC_DEBUG's to increase performance
 * 	[1995/09/29  20:52 UTC  jrr  /main/HPDCE02/4]
 * 
 * 	Change dce_svc_printf's to DCE_SVC_DEBUG's to increase performance
 * 	[1995/09/29  20:49 UTC  jrr  /main/HPDCE02/jrr_dcecp_01/1]
 * 
 * 	merge
 * 	[1995/09/11  22:07 UTC  rps  /main/HPDCE02/3]
 * 
 * 	CHFts16274
 * 	[1995/09/08  16:13 UTC  rps  /main/HPDCE02/rps_moth4/1]
 * 
 * 	Merge mem leak fix from mb_mothra5
 * 	[1995/07/28  19:16 UTC  burati  /main/HPDCE02/2]
 * 
 * 	Memory leaks.
 * 	[1995/07/28  18:46 UTC  burati  /main/HPDCE02/mb_mothra5/1]
 * 
 * 	gss_inquire_cred doesn't return correct results (CHFts15003).
 * 	[1995/05/03  19:38 UTC  jrr  /main/HPDCE02/1]
 * 
 * 	Fix CR 12838: gss_inquire_cred fails for default cred
 * 	[1995/04/26  22:48 UTC  jrr  /main/jrr_sec_chfts15003/1]
 * 
 * Revision 1.1.7.3  1996/02/18  00:08:07  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:59:20  marty]
 * 
 * Revision 1.1.7.2  1995/12/18  17:18:25  psn
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/12/15  20:02 UTC  psn
 * 	international libdce mopup
 * 	[1995/12/13  01:39 UTC  rps  /main/HPDCE02/rps_moth5/1]
 * 
 * 	HP revision /main/HPDCE02/5  1995/10/03  15:45 UTC  nav
 * 	merge from the private branch. OT 13090.
 * 
 * 	HP revision /main/HPDCE02/nav_mothra_3/1  1995/09/29  16:26 UTC  nav
 * 	fix for OT 13090. gss_unseal and gss_verify do not use conf_state and qop_state parameters
 * 	properly. Include the diffs from the OT.
 * 
 * 	HP revision /main/HPDCE02/4  1995/09/29  20:52 UTC  jrr
 * 	Change dce_svc_printf's to DCE_SVC_DEBUG's to increase performance
 * 
 * 	HP revision /main/HPDCE02/jrr_dcecp_01/1  1995/09/29  20:49 UTC  jrr
 * 	Change dce_svc_printf's to DCE_SVC_DEBUG's to increase performance
 * 
 * 	HP revision /main/HPDCE02/3  1995/09/11  22:07 UTC  rps
 * 	merge
 * 
 * 	HP revision /main/HPDCE02/rps_moth4/1  1995/09/08  16:13 UTC  rps
 * 	CHFts16274
 * 
 * 	HP revision /main/HPDCE02/2  1995/07/28  19:16 UTC  burati
 * 	Merge mem leak fix from mb_mothra5
 * 
 * 	HP revision /main/HPDCE02/mb_mothra5/1  1995/07/28  18:46 UTC  burati
 * 	Memory leaks.
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/03  19:38 UTC  jrr
 * 	gss_inquire_cred doesn't return correct results (CHFts15003).
 * 
 * 	HP revision /main/jrr_sec_chfts15003/1  1995/04/26  22:48 UTC  jrr
 * 	Fix CR 12838: gss_inquire_cred fails for default cred
 * 	[1995/12/18  17:17:14  psn]
 * 
 * Revision 1.1.2.11  1994/10/05  21:47:39  wray
 * 	CR 12513
 * 	[1994/10/05  21:44:30  wray]
 * 
 * Revision 1.1.2.10  1994/09/23  19:52:17  wray
 * 	CR 12215 & 12073 - Cred management
 * 	[1994/09/23  19:49:12  wray]
 * 
 * Revision 1.1.2.9  1994/09/16  15:52:54  wray
 * 	Fix incomplete submission for CR 12190/12200
 * 	[1994/09/16  15:51:59  wray]
 * 
 * Revision 1.1.2.8  1994/09/15  19:21:33  wray
 * 	CR 10895, 11894, 12047, 11540, 12190, 12200
 * 	[1994/09/15  19:19:20  wray]
 * 
 * Revision 1.1.2.7  1994/08/22  13:27:02  wray
 * 	CRs 11376, 11762, 11763, 11765
 * 	[1994/08/22  13:24:50  wray]
 * 
 * Revision 1.1.2.6  1994/08/03  15:18:58  wray
 * 	Workaround for OT 10805
 * 	[1994/08/03  15:00:11  wray]
 * 
 * Revision 1.1.2.5  1994/06/29  18:29:06  wray
 * 	Fix compilation warnings (OT 11130)
 * 	[1994/06/29  18:28:06  wray]
 * 
 * Revision 1.1.2.4  1994/06/28  20:00:52  wray
 * 	GSSAPI BL3 (OT 10824)
 * 	[1994/06/28  19:25:08  wray]
 * 
 * Revision 1.1.2.3  1994/06/17  18:41:55  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:08:32  devsrc]
 * 
 * Revision 1.1.2.2  1994/03/23  16:47:27  wray
 * 	GSSAPI BL2 (OT#10179)
 * 	[1994/03/23  16:43:51  wray]
 * 
 * 	GSSAPI BL2 (OT#10179)
 * 
 * Revision 1.1.2.1  1994/03/09  15:50:53  wray
 * 	Initial submission (OT#10019)
 * 	[1994/03/09  15:38:37  wray]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) 1993, 1994 by
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
 * FACILITY:	Generic Security Service API (GSSAPI)
 *
 * ABSTRACT:	Main-line GSSAPI code.  Contains all public GSSAPI entrypoints.
 *	
 * ENVIRONMENT:	portable
 *
 * AUTHOR:
 *           	John C Wray
 *		Distributed Processing Engineering (DPE)
 *
 * DATE:	June 1993
 *
 * REVISION HISTORY:
 *		See CMS commentary at end
 *
 *
 */


#include <stdio.h>
#include <stdarg.h>
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <gssapi-int.h>
#include <gssapi-support.h>
#include <dce/dce_error.h>
#include <dce/binding.h>
#include <dce/dce_cf.h>
#include <dce/sec_cred.h>
#include <sec_cred_internal.h>
#include <u_str.h>

/****************************************************************************/
/*									    */
/*                        Configuration section.			    */
/*									    */
/****************************************************************************/

/* This section contains various #defines that customize GSSAPI in various  */
/* ways.								    */



/* Exportable version should define NOENCRYPTION to disable DES privacy	    */
/* protection in gss_seal.						    */
/*									    */
/* #define NOENCRYPTION							    */


/* Define the following symbol to enable graceful degradation to	    */
/* integrity-only protection in a NOENCRYPTION environment when the caller  */
/* explicitly requests confidentiality.  If this symbol is not defined,	    */
/* such a request will return an error to the caller.  The IETF spec calls  */
/* for graceful degradation, although an error may be more user-friendly.   */
#define CONFIDENTIALITY_FAILOVER


/* Define the symbol GSS_DEBUG_VARIABLE to equate to a quoted string	    */
/* containing the name of the environment variable that will control the    */
/* generation of non-OSF routed debug info.  To completely disable dynamic  */
/* debugging (in the absence of OSF svc code), undefine the symbol.	    */

#ifdef NO_OSF_SVC_CODE
#define GSS_DEBUG_VARIABLE "SVC_GSS"					    
#endif


/* Define the symbol GSS_DELEGATION to enable delegation support within	    */
/* GSSAPI.								    */
#define GSS_DELEGATION


/* Define the symbol DCE_V11 to use new sec_cred routines instead of	    */
/* wandering around inside DCE 1.0 PACs.				    */
#define DCE_V11


/* Define the symbol VERBOSE_DEBUG_MESSAGES to enable some additional	    */
/* tracing.  This stuff produces traces at the debug9 level in nauseating   */
/* detail, so it's really only wanted if there's a hard-to-locate bug.	    */
/* Even without this symbol, debug9 level tracing still produces a ton of   */
/* messages.								    */
#define VERBOSE_DEBUG_MESSAGES


/* Define the symbol IMPORT_CONTEXTS_TO_BOTH_MODE_CREDS to allow	    */
/* gssdce_login_context_to_cred to create BOTH-mode credentials.  Without   */
/* this definition, the routine will only create INIT-mode creds.	    */
#define IMPORT_CONTEXTS_TO_BOTH_MODE_CREDS


/* Set up the attributes for the verious mutexes used within GSSAPI.	    */
/* All mutexes are non-recursive except individual element mutexes, which   */
/* are recursive.							    */
#define GSSAPI_MUTEX_ATTRIBUTES MUTEX_NONRECURSIVE_NP
#define CRED_LIST_MUTEX_ATTRIBUTES MUTEX_NONRECURSIVE_NP
#define CTX_LIST_MUTEX_ATTRIBUTES MUTEX_NONRECURSIVE_NP
#define RGY_MUTEX_ATTRIBUTES MUTEX_NONRECURSIVE_NP
#define GSSAPI_ELEMENT_MUTEX_ATTRIBUTES MUTEX_RECURSIVE_NP

/* The following symbols relate to static (compile-time) debugging.	    */
/* Generally this should be disabled, since the new dynamic debugging takes */
/* its place.								    */
#if defined(DEBUG) || defined(DCE_DEBUG) || defined(DCE_GSS_DEBUG)
/* then we want to debug everything					    */
#define DEBUG_OID
#define DEBUG_LIST
#define DEBUG_TIME
#define DEBUG_RGY
#define DEBUG_CRED
#define DEBUG_AUTH
#define DEBUG_WINDOW
#define DEBUG_PROT
#define DEBUG_NAME
#define DCE_GSS_DEBUG
#endif

#if defined(DCE_DEBUG) || defined(DEBUG_OID) || defined(DEBUG_LIST) \
|| defined(DEBUG_TIME) || defined(DEBUG_RGY) || defined(DEBUG_CRED) \
|| defined(DEBUG_AUTH) || defined(DEBUG_WINDOW) || defined(DEBUG_PROT) \
|| defined(DEBUG_NAME) \
|| defined(DCE_GSS_DEBUG)
#include <stdlib.h>
#define DEBUG_ANY
#else
#if defined(GSS_DEBUG_VARIABLE)
#include <stdlib.h>
#endif
#endif


#define TOK_ID_SIGN 1
#define TOK_ID_SEAL 2
#define TOK_ID_DELETE 3

/* The following symbol sets the default integrity protection algorithm	    */
#define DEFAULT_SIGN_ALGORITHM SIGN_ALG_MD5


/****************************************************************************/
/*			    Extern definitions				    */
/****************************************************************************/

extern void irep_to_string(gss_name_t source_irep_name,
			  sec_rgy_name_t resultant_string_name,
			  error_status_t * error);
/* Convert an internal-format global name to sec_rgy_name_t string-format.  */

extern void string_to_irep(sec_rgy_name_t string_name,
			  gss_name_t * resultant_irep_name,
			  error_status_t * error);
/* Convert a sec_rgy_name_t string-format name to internal-format.	    */

extern void lookup_reg_server_arg(sec_rgy_name_t server_name,
				  void ** arg,
				  error_status_t * err);
/* Return the "arg" associated with the specified registered server.	    */




/****************************************************************************/
/*			   Some internal constants			    */
/****************************************************************************/

#define MAX_RGY_RETRIES 4
/* Number of time we'll try to establish contact with a registry server	    */
/* before returning a failure to the caller.				    */

#define MAX_CRED_ID 32767
/* Maximum number of simultaneously-held credentials.			    */

#define MAX_CTX_ID 32767
/* Maximum number of simultaneously-held contexts.			    */


#define MAX_SEQ 0xfffffffful
/* Maximum value that a 32-bit sequence number shall be allowed to reach    */
/* before it wraps.							    */

#define INCREMENT_SEQ(x) \
    if ((x)>=MAX_SEQ) (x)=0; else (x)+=1
/* Macro to increment a wrapping 32-bit sequence number			    */

#define GTR(a,b) a > b
/* Macro to compare two sequence numbers				    */


/****************************************************************************/
/*			    Temporary fix-ups				    */
/****************************************************************************/

void irep_to_string(gss_name_t source_irep_name,
		  sec_rgy_name_t resultant_string_name,
		  error_status_t * error) {
    int i, done;

    done = 0; i = 0;
    while (i < sizeof(sec_rgy_name_t)) {
	if (!done) {
	    if ((resultant_string_name[i] = source_irep_name[i]) == 0)
		done = 1;
	} else resultant_string_name[i] = 0;
	i++;
    };
    *error = error_status_ok;
}

extern void string_to_irep(sec_rgy_name_t string_name,
			  gss_name_t * resultant_irep_name,
			  error_status_t * error) {
    int i;
    unsigned char res_name[1024];
    char * cell_name;

    *resultant_irep_name = NULL;
    dce_cf_get_cell_name(&cell_name, error);
    if (*error != dce_cf_st_ok) return;

    if (strncmp((char *)string_name, "/...", 4) == 0) {
/* We've got an absolute name...					    */

	strncpy((char *)res_name, 
		(char *)string_name, 
		sizeof(res_name));
	res_name[sizeof(res_name)-1] = 0;

    } else if (strncmp((char *)string_name, "/.:", 3) == 0) {

	strncpy((char *)res_name, 
		cell_name, 
		sizeof(res_name) - strlen((char *)res_name));
	strncat((char *)res_name, 
		(char *)&string_name[3], 
		sizeof(res_name) - strlen((char *)res_name));
    } else {

	strncpy((char *)res_name, 
		cell_name, 
		sizeof(res_name) - strlen((char *)res_name));
	strncat((char *)res_name, 
		"/", 
		sizeof(res_name) - 1);
	strncat((char *)res_name, 
		(char *)&string_name[0], 
		sizeof(res_name) - strlen((char *)res_name));
    };



    free(cell_name);

    if ((*resultant_irep_name = 
		(char *)malloc(strlen((char *)res_name)+ 1)) == NULL) {
	dce_svc_printf(GSS_S_NOMEM_1_MSG);
	*error = gssdce_s_out_of_memory;
	return;	
    };
    for (i=0;
	 i < strlen((char *)res_name);
	 i++) {
	(*resultant_irep_name)[i] = res_name[i];
    };
    (*resultant_irep_name)[strlen((char *)res_name)] = 0;
    
    *error = error_status_ok;


}


static gss_OID_set_desc dce_mech_set;

/****************************************************************************/
/*									    */
/*	                Static routine definitions			    */
/*									    */
/****************************************************************************/

#ifndef DEBUG_ANY
static
#endif
char * ctx_state_str(ctx_info * ctx_ptr);

#ifndef DEBUG_ANY
static
#endif
char * ctx_authn_str(ctx_info * ctx_ptr);

#ifndef DEBUG_ANY
static
#endif
char * ctx_authz_str(ctx_info * ctx_ptr);

/****************************************************************************/
/*			        OID stuff				    */
/****************************************************************************/

static void nm_oid_copy(gss_OID src,
		        gss_OID * dst);
/* Copies the OID							    */

static int nm_oid_set_test_member(gss_OID member,
				  gss_OID_set set);
/* Returns non-zero if the member is in the set				    */

static void nm_oid_set_create_empty(gss_OID_set * set);
/* Creates an empty OID set						    */

static void nm_oid_set_add_elem(gss_OID_set * set,
			        gss_OID element);
/* Adds an element to an OID set					    */

static void nm_oid_set_free(gss_OID_set * set);
/* Deletes an OID set							    */

static void nm_oid_free(gss_OID oid, int free_desc);
/* Deletes a single OID.  If free_desc is true, frees the OID itself,	    */
/* otherwise just frees the referenced storage.				    */

static int nm_oid_equal(gss_OID oid1, gss_OID oid2);
/* Returns true if OIDs are equal					    */




/****************************************************************************/
/*			Credential and context stuff			    */
/****************************************************************************/

static cred_info * create_cred(OM_uint32 flags,
                               sec_login_handle_t login_context);
/* Creates a new cred_info record and initializes it with the specified	    */
/* flags and login context.						    */

static void destroy_cred(cred_info * cred);
/* Deletes a cred_info record						    */

static cred_info * resolve_cred(gss_cred_id_t cred_id);
/* Returns the cred_info record associated with cred_id, or NULL if the	    */
/* record does not exist.						    */

static gss_cred_id_t cred_info_to_gss(cred_info * cred_ptr);
/* Returns the gss_cred_id_t associated with the cred_info record.	    */


static ctx_info * create_ctx(void);
/* Creates a new ctx_info record and initializes it			    */

static void destroy_ctx(ctx_info * ctx);
/* Deletes a ctx_info record						    */

static ctx_info * resolve_ctx(gss_ctx_id_t ctx_id);
/* Returns the ctx_info record associated with ctx_ptr, or NULL if the	    */
/* record does not exist.						    */

static gss_ctx_id_t ctx_info_to_gss(ctx_info * ctx_ptr);
/* Returns the gss_ctx_id_t associated with the ctx_info record.	    */

static OM_uint32 cred_expand_id(cred_info * cred_ptr);
/* Fills in either the name or the UUID field of a credential, assuming	    */
/* that the other field is already there.				    */

static void tidy_cred(cred_info * cred_ptr);
static void tidy_ctx(ctx_info * ctx_ptr);
/* Deletes any storage associated with a context or credential (e.g.	    */
/* external names or login-contexts)					    */

ctx_info_list * ctx_info_head = NULL;

cred_info_list * cred_info_head = NULL;

static cred_info_list * alloc_cred(OM_uint32 flags,
                                   sec_login_handle_t login_context);


static OM_uint32 make_cred_id( OM_uint32 * context);
/* Delivers the next unused cred ID value, and also updates context.	    */
/* CRED_LIST_LOCK must be held prior to calling this routine.		    */

static OM_uint32 make_ctx_id( OM_uint32 * context);
/* Delivers the next unused ctx ID value, and also updates context.	    */
/* CTX_LIST_LOCK must be held prior to calling this routine.		    */

static cred_info * resolve_cred(gss_cred_id_t cred_id);
/* Returns the cred_info record associated with cred_ptr, or NULL if the    */
/* record does not exist.						    */

static OM_uint32 check_cred( gss_cred_id_t  cred_id);
/* Returns non-zero if the specified credential exists.			    */
/* CRED_LIST_LOCK must be held prior to calling this routine.		    */

static OM_uint32 check_ctx( gss_cred_id_t  cred_id);
/* Returns non-zero if the specified context exists.			    */
/* CTX_LIST_LOCK must be held prior to calling this routine.		    */

static void set_cred_manage_context(cred_info * cred_ptr);
/* Marks the contained login-context in a credential so that it will be	    */
/* deleted by GSSAPI when the credential is deleted.			    */

static void set_cred_unmanage_context(cred_info * cred_ptr);
/* Marks the contained login-context in a credential so that it will not be */
/* deleted by GSSAPI when the credential is deleted.			    */

/****************************************************************************/
/*			Time conversion routines			    */
/****************************************************************************/

static OM_uint32 utc_time_to_delta_seconds(utc_t *utc_time);

static void delta_seconds_to_utc_time(OM_uint32 secs, utc_t *utc_time);

			


/****************************************************************************/
/*									    */
/*	                      Locking routines				    */
/*									    */
/****************************************************************************/

/* These macros should be used for ensuring exclusive access to GSSAPI	    */
/* objects.  They return 0 for success, non-zero for a locking protocol	    */
/* violation.								    */
/*									    */
/* GSSAPI locking policy:						    */
/*									    */
/*     Each queue (cred_info_list & ctx_info_list) is protected at the list */
/*     level by a mutex, that can be locked unlocked with the macros	    */
/*     LOCK_CRED_LIST/UNLOCK_CRED_LIST and LOCK_CTX_LIST/UNLOCK_CTX_LIST.   */
/*     Any routine that 'walks the pointers' of these lists _must_ take out */
/*     the appropriate mutex first, and hold on to it as long as it wants   */
/*     to ensure that any list pointers it holds remain valid.  After	    */
/*     locating a particular element of interest within the list (ie a	    */
/*     credential or context), the individual element may be locked (using  */
/*     the LOCK_ELEMENT/UNLOCK_ELEMENT macros).  After locking an	    */
/*     individual element, the list lock may be released.		    */
/*									    */
/*     A potential deadlock during element deletion is avoided by the use   */
/*     of the "delete_pending" field of each element.  This integer is	    */
/*     initialized to 0 when an element is created.  Any routine that	    */
/*     acquires a handle to an element (involving taking out a lock on the  */
/*     element) should, after taking out the element lock but before	    */
/*     releasing the list-lock, check this flag.  If non-zero, then the	    */
/*     element is being destroyed and the handle acquisition routine must   */
/*     immediately release the element-lock, followed be releasing the	    */
/*     list-lock and failing.						    */


#define LOCK_GSSAPI local_lock(&gssapi_mutex)
#define UNLOCK_GSSAPI pthread_mutex_unlock(&gssapi_mutex)
#define LOCK_CRED_LIST local_lock(&cred_list_mutex)
#define UNLOCK_CRED_LIST pthread_mutex_unlock(&cred_list_mutex)
#define LOCK_CTX_LIST local_lock(&ctx_list_mutex)
#define UNLOCK_CTX_LIST pthread_mutex_unlock(&ctx_list_mutex)
#define LOCK_RGY local_lock(&rgy_mutex)
#define UNLOCK_RGY pthread_mutex_unlock(&rgy_mutex)
#define LOCK_ELEMENT(x) local_lock(&((x)->mutex))
#define UNLOCK_ELEMENT(x) do {if ((x) != NULL) {pthread_mutex_unlock(&((x)->mutex));}} while(0)

static pthread_once_t gssapi_once_block = pthread_once_init;

static pthread_mutexattr_t gssapi_mutex_attributes;
pthread_mutex_t gssapi_mutex;
static pthread_mutexattr_t cred_list_mutex_attributes;
static pthread_mutex_t cred_list_mutex;
static pthread_mutexattr_t ctx_list_mutex_attributes;
static pthread_mutex_t ctx_list_mutex;
static pthread_mutexattr_t rgy_mutex_attributes;
static pthread_mutex_t rgy_mutex;
static pthread_mutexattr_t gssapi_element_mutex_attributes;



static void gssapi_initialization(void);

static int local_lock(pthread_mutex_t * mutex);

#ifndef GSS_UNLOCK_PROCESSING
static int local_unlock(pthread_mutex_t * mutex);
#endif

#define SMALLER(a,b) ((a<b)?a:b)


/****************************************************************************/
/*									    */
/*	                     Serviceability routines			    */
/*									    */
/****************************************************************************/

#if defined(GSS_DEBUG_VARIABLE)
#define gss__svc_debug_routing(where, st) dce_svc_debug_routing(where, st)
#else

/*
static void * gss__debug_stream;
static gss__svc_debug_routing(const char * where, error_status_t *st) {}
*/
#endif



/****************************************************************************/
/*									    */
/*	                      More locking stuff			    */
/*									    */
/****************************************************************************/


static int initialization_error_value = 0;
static int initialization_error_loc = 0;
static OM_uint32 gssapi_next_cred_id = 0;
static OM_uint32 gssapi_next_ctx_id = 0;


#define UNEXPECTED_RETURN \
    *minor_status = gssdce_s_unexpected_error; \
    return GSS_S_FAILURE

#define UNIMPLEMENTED_RETURN \
    *minor_status = gssdce_s_unimplemented_feature; \
    return GSS_S_FAILURE

#define GSS_INIT \
{if (!gssapi_next_cred_id) \
    pthread_once(&gssapi_once_block, gssapi_initialization);}



static int local_lock(pthread_mutex_t * mutex) {
/* Version of pthread_mutex_lock that ensures that the mutexes have been    */
/* created before it tries to use them. 				    */
    int res;
    GSS_INIT;
    res = pthread_mutex_lock(mutex);
    if (res) {
	dce_svc_printf(GSS_S_LOCKING_ERROR_MSG);
    };

    return res;
}

#ifndef GSS_UNLOCK_PROCESSING
static int local_unlock(pthread_mutex_t * mutex) {
    return pthread_mutex_unlock(mutex);
}
#endif

static void gssapi_initialization(void) {
/* Main purpose of GSSAPI initialization is to initialize mutexes.	    */
/* Subsiduary task is to read the environment variable GSS_DEBUG to enable  */
/* dynamic tracing.							    */

    int status;
    char * where_string;
    error_status_t st;

    
#ifdef NO_OSF_SVC_CODE

    gss__debug_stream = NULL;

#ifdef GSS_DEBUG_VARIABLE

    if ((where_string = getenv(GSS_DEBUG_VARIABLE)) == NULL) {
	gss__debug_flags = 0; /* No debug info */
    } else {
	gss__svc_debug_routing(where_string, & st);
    };
#else
    gss__debug_flags = 0; /* No debug info */
#endif
#else
    gss__svc_handle = dce_svc_register(gss__svc_table, 
				       (idl_char *)"gss", 
				       & st);
    if (st) {
	fprintf(stderr, "GSSAPI: Can't initialize SVC code\n");
	fflush(stderr);
    };
#endif    


/* We have to be real careful about the next two messages, if we're using   */
/* the home-grown versions of the serviceability routines.  We're inside a  */
/* once-only routine, so there should be no problems invoking the svc	    */
/* routines directly; however, the svc routines will try to protect	    */
/* themselves against multiple access, and the mutexes they're going to use */
/* don't exist yet.  The static variable svc_locks_established (initialized */
/* to 0) will tell the svc routines not to try to lock themselves yet.	    */



/* Install a default message catalog for GSSAPI...			    */


    dce_msg_define_msg_table(gss__table,
			     sizeof(gss__table) / sizeof(gss__table[0]), 
			     &st);

    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, 
                   svc_c_debug8, "GSSAPI initialization"));
    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, 
                   svc_c_debug9, "GSSAPI build of %s", __DATE__ " " __TIME__));

    if (st) {
	dce_svc_printf(GSS_S_CANT_REG_TABLE_MSG, st);
    };


    /*
     * gssapi makes use of sec_krb code and its mutexes -- which are
     * allocated only when one actually makes authenticated rpc calls.
     * Thus we call its init routine in lieu of rpc modules doing so.
     */
    sec_krb_init();


    gssapi_next_cred_id = 1;
    gssapi_next_ctx_id = 1;

    if ((status = pthread_mutexattr_create(&gssapi_mutex_attributes)) != 0) {
	initialization_error_value = status;
	initialization_error_loc = 101;
	dce_svc_printf(GSS_S_CANT_CREATE_GSS_MUTEX_ATTR_MSG, status);
    };

    if ((status = pthread_mutexattr_setkind_np(&gssapi_mutex_attributes,
					       GSSAPI_MUTEX_ATTRIBUTES)) != 0) {
	initialization_error_value = status;
	initialization_error_loc = 102;
	dce_svc_printf(GSS_S_CANT_SET_GSS_ATTRIBUTES_MSG, status);
    };

    if ((status = pthread_mutex_init(&gssapi_mutex, 
				     gssapi_mutex_attributes)) != 0) {
	initialization_error_value = status;
	initialization_error_loc = 103;
	dce_svc_printf(GSS_S_CANT_CREATE_GSS_MUTEX_MSG, status);
    };


    if ((status = pthread_mutexattr_delete(&gssapi_mutex_attributes)) != 0) {
	initialization_error_value = status;
	initialization_error_loc = 104;
	dce_svc_printf(GSS_S_CANT_DELETE_GSS_MUTEX_ATTR_MSG, status);
    };


    if ((status = pthread_mutexattr_create(&cred_list_mutex_attributes)) != 0) {
	initialization_error_value = status;
	initialization_error_loc = 5;
	dce_svc_printf(GSS_S_CANT_CREATE_CRED_MUTEX_ATTR_MSG, status);
    };

    if ((status = pthread_mutexattr_setkind_np(&cred_list_mutex_attributes,
					       CRED_LIST_MUTEX_ATTRIBUTES)) != 0) {
	initialization_error_value = status;
	initialization_error_loc = 6;
	dce_svc_printf(GSS_S_CANT_SET_CRED_ATTRIBUTES_MSG, status);
    };

    if ((status = pthread_mutex_init(&cred_list_mutex, 
				     cred_list_mutex_attributes)) != 0) {
	initialization_error_value = status;
	initialization_error_loc = 7;
	dce_svc_printf(GSS_S_CANT_CREATE_CRED_MUTEX_MSG, status);
    };

    if ((status = pthread_mutexattr_delete(&cred_list_mutex_attributes)) != 0) {
	initialization_error_value = status;
	initialization_error_loc = 8;
	dce_svc_printf(GSS_S_CANT_DELETE_CRED_MUTEX_ATTR_MSG, status);
    };


    if ((status = pthread_mutexattr_create(&ctx_list_mutex_attributes)) != 0) {
	initialization_error_value = status;
	initialization_error_loc = 9;
	dce_svc_printf(GSS_S_CANT_CREATE_CTX_MUTEX_ATTR_MSG, status);
    };

    if ((status = pthread_mutexattr_setkind_np(&ctx_list_mutex_attributes,
					       CTX_LIST_MUTEX_ATTRIBUTES)) != 0) {
	initialization_error_value = status;
	initialization_error_loc = 10;
	dce_svc_printf(GSS_S_CANT_SET_CTX_ATTRIBUTES_MSG, status);
    };

    if ((status = pthread_mutex_init(&ctx_list_mutex, 
				     ctx_list_mutex_attributes)) != 0) {
	initialization_error_value = status;
	initialization_error_loc = 11;
	dce_svc_printf(GSS_S_CANT_CREATE_CTX_MUTEX_MSG, status);
    };

    if ((status = pthread_mutexattr_delete(&ctx_list_mutex_attributes)) != 0) {
	initialization_error_value = status;
	initialization_error_loc = 12;
	dce_svc_printf(GSS_S_CANT_DELETE_CTX_MUTEX_ATTR_MSG, status);
    };


    if ((status = pthread_mutexattr_create(&rgy_mutex_attributes)) != 0) {
	initialization_error_value = status;
	initialization_error_loc = 13;
	dce_svc_printf(GSS_S_CANT_CREATE_RGY_MUTEX_ATTR_MSG, status);
    };

    if ((status = pthread_mutexattr_setkind_np(&rgy_mutex_attributes,
					       RGY_MUTEX_ATTRIBUTES)) != 0) {
	initialization_error_value = status;
	initialization_error_loc = 14;
	dce_svc_printf(GSS_S_CANT_SET_RGY_ATTRIBUTES_MSG, status);
    };

    if ((status = pthread_mutex_init(&rgy_mutex, 
				     rgy_mutex_attributes)) != 0) {
	initialization_error_value = status;
	initialization_error_loc = 15;
	dce_svc_printf(GSS_S_CANT_CREATE_RGY_MUTEX_MSG, status);
    };

    if ((status = pthread_mutexattr_delete(&rgy_mutex_attributes)) != 0) {
	initialization_error_value = status;
	initialization_error_loc = 16;
	dce_svc_printf(GSS_S_CANT_DELETE_RGY_MUTEX_ATTR_MSG, status);
    };


    if ((status = pthread_mutexattr_create(&gssapi_element_mutex_attributes)) != 0) {
	initialization_error_value = status;
	initialization_error_loc = 17;
	dce_svc_printf(GSS_S_CANT_CREATE_ELEM_MUTEX_ATTR_MSG, status);
    };

    if ((status = pthread_mutexattr_setkind_np(
			&gssapi_element_mutex_attributes,
			GSSAPI_ELEMENT_MUTEX_ATTRIBUTES)) != 0) {
	initialization_error_value = status;
	initialization_error_loc = 18;
	dce_svc_printf(GSS_S_CANT_SET_ELEM_ATTRIBUTES_MSG, status);
    };
/* gssapi_element_mutex_attributes will be retained as the attribute type   */
/* for any list element mutexes that we may need to create.		    */


    dce_mech_set.count = 1;
    dce_mech_set.elements = GSSDCE_C_OID_DCE_KRBV5_DES;

}

/****************************************************************************/
/*									    */
/*	                Misc time-conversion routines			    */
/*									    */
/****************************************************************************/


static OM_uint32 utc_time_to_delta_seconds(utc_t *utc_time) {
    utc_t now;
    utc_t exp;
    reltimespec_t rel_time;
    struct tm inaccuracy;
    long ins;
    struct tm rel_tmtime;
    long tns;
    OM_uint32 time;

/* exp = utc_time - NOW							    */
    utc_subtime(&exp,
		utc_time,
		NULL);
/* rel_tmtime = CONVERT_TO_TMTIME(exp)					    */
    utc_reltime(&rel_tmtime,
		&tns,
		&inaccuracy,
		&ins,
		&exp);
/* Result should be in the future...					    */
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, 
                       svc_c_debug9, "utc_time_to_delta: Yday: %d, Hour: %d, Min: %d, Sec: %d",
		     rel_tmtime.tm_yday,
		     rel_tmtime.tm_hour,
		     rel_tmtime.tm_min,
		     rel_tmtime.tm_sec));

    if (rel_tmtime.tm_yday < 0 ||
	rel_tmtime.tm_hour < 0 ||
	rel_tmtime.tm_min < 0 ||
	rel_tmtime.tm_sec < 0) time = 0; /* Already expired */
    else time = (long)rel_tmtime.tm_sec +
		(60l * (rel_tmtime.tm_min  +
		 (60l * (rel_tmtime.tm_hour +
		  (24l * (rel_tmtime.tm_yday)
		 )))));
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, 
                       svc_c_debug9, "Delta secs: %ld", (long)time));
		 
    return time;
}

static void delta_seconds_to_utc_time(OM_uint32 secs, utc_t *utc_time) {
    struct tm rel_tmtime;
    utc_t reltime;

    memset(&rel_tmtime, 0, sizeof(struct tm));


    rel_tmtime.tm_yday = secs / (24 * 60 * 60);
    secs = secs % (24 * 60 * 60);
    rel_tmtime.tm_hour = secs / (60 * 60);
    secs = secs % (60 * 60);
    rel_tmtime.tm_min = secs / 60;
    secs = secs % 60;
    rel_tmtime.tm_sec = secs;


    utc_mkreltime(&reltime,
		  &rel_tmtime,
		  0,
		  NULL,
		  0);

    utc_addtime(utc_time,
		NULL,	/* NOW */
		&reltime);
}



static void display_uuid(FILE * file, 
			 uuid_t uuid,
			 char * s) {

    error_status_t error_status;
    unsigned char * uuid_string;
    uuid_to_string(&uuid, &uuid_string, &error_status);
    if (error_status != uuid_s_ok) {
	fprintf(file, "%s <Error (%lu)>\n", s, error_status);
    } else {
	fprintf(file, "%s %s\n", s, uuid_string);
	rpc_string_free(&uuid_string, &error_status);
    };
  

}


/****************************************************************************/
/*									    */
/*				Utility routines			    */
/*									    */
/****************************************************************************/
static error_status_t copy_pac(sec_id_pac_t *to, sec_id_pac_t * from) {
    return sec_id_pac_util_copy (to, from);
}

static void free_pac(sec_id_pac_t * p) {
    sec_id_pac_util_free(p);
}

static error_status_t copy_creds(rpc_authz_cred_handle_t * to, 
				 rpc_authz_cred_handle_t * from) {
    *to = *from;
    return 0;
}

static void free_creds(rpc_authz_cred_handle_t * p) {
    error_status_t error_status;
    error_status = sec__cred_free_cred_handle(p);
    if (error_status) {
	dce_svc_printf(GSS_S_FREE_CRED_HANDLE_FAIL_MSG, error_status);
    };
}

/****************************************************************************/
/*									    */
/*	                     Cred/ctx primitives    			    */
/*									    */
/****************************************************************************/

static OM_uint32 check_cred( gss_cred_id_t  cred_id) {
    cred_info_list * c_p;

    for (c_p = cred_info_head;
	 c_p != NULL;
	 c_p = c_p->next)
	if (cred_id == c_p->id) return 1;
    return 0;
}

static OM_uint32 check_ctx(gss_ctx_id_t  ctx_id) {
    ctx_info_list * c_p;

    for (c_p = ctx_info_head;
	 c_p != NULL;
	 c_p = c_p->next)
	if (ctx_id == c_p->id) return 1;
    return 0;
}

static OM_uint32 make_cred_id( OM_uint32 * context) {
    OM_uint32 id_val = *context;
    cred_info * cred_ptr;
    int tries = 0;

    while (check_cred(id_val)) {
	if (++tries > MAX_CRED_ID) return 0;
	if (++id_val > MAX_CRED_ID) id_val = 1;
    };    
    *context = id_val+1;
    return id_val;
}

static OM_uint32 make_ctx_id( OM_uint32 * context) {
    OM_uint32 id_val = *context;
    ctx_info * cred_ptr;
    int tries = 0;
    while (check_ctx(id_val)) {
	if (++tries > MAX_CTX_ID) return 0;
	if (++id_val > MAX_CTX_ID) id_val = 1;
    };    
    *context = id_val+1;
    return id_val;
}


static cred_info * resolve_cred(gss_cred_id_t cred_id) {
    cred_info_list * c_p;
    cred_info * ret_val;

    if (LOCK_CRED_LIST) {
	dce_svc_printf(GSS_S_RES_CRED_LOCK_FAILED_MSG);
	return NULL;
    };
    for (c_p = cred_info_head;
	c_p != NULL;
	c_p = c_p->next)
    {
	if (cred_id == c_p->id) {
	    ret_val = &(c_p->rec);
	    LOCK_ELEMENT(ret_val);
	    if (ret_val->delete_pending) {
		DCE_SVC_DEBUG((gss__svc_handle, gss_s_locks, svc_c_debug1, "Resolve_cred - delete pending."));
	
		UNLOCK_ELEMENT(ret_val);
		ret_val = NULL;
	    };
	    UNLOCK_CRED_LIST;
	    return ret_val;
	};
    };

    dce_svc_printf(GSS_S_RES_CRED_NOT_FOUND_MSG, (long)cred_id);
    UNLOCK_CRED_LIST;
    return NULL;
}

static cred_info_list * alloc_cred(OM_uint32 flags,
                                   sec_login_handle_t login_context) {
/* Caution - CRED_LIST_LOCK must be held when this routine is called!	    */
    cred_info_list * c_p;
    if ((c_p = (cred_info_list *)malloc(sizeof(cred_info_list))) == NULL) {
	dce_svc_printf(GSS_S_ALLOC_CRED_NOMEM_MSG);
	return NULL;
    };
    c_p->prev = NULL;
    c_p->next = NULL;
    if ((c_p->id = make_cred_id(&gssapi_next_cred_id)) == 0) {
	dce_svc_printf(GSS_S_ALLOC_CRED_NONEW_MSG);
	free(c_p);
	return NULL;
    };

    dce_svc_printf(GSS_S_ALLOC_CRED_ALLOCATED_MSG, (long)c_p->id);

    memset(&(c_p->rec), 0, sizeof(cred_info));
    pthread_mutex_init(&(c_p->rec.mutex), gssapi_element_mutex_attributes);
    LOCK_ELEMENT(&(c_p->rec));
    c_p->rec.flags = flags;
    c_p->rec.login_context = login_context;
    return c_p;
}

static cred_info * create_cred(OM_uint32 flags,
                               sec_login_handle_t login_context) {
    cred_info_list * c_p;
    cred_info * ret_val;
    if (LOCK_CRED_LIST) {
	dce_svc_printf(GSS_S_CREATE_CRED_NOLOCK_MSG);
	return NULL;
    };
    c_p = cred_info_head;
    if (c_p == NULL) {
	if ((c_p = cred_info_head = alloc_cred(flags, login_context)) == NULL) {
	    UNLOCK_CRED_LIST;
	    return NULL;
	};
    } else {
	while (c_p->next != NULL) c_p = c_p->next;
	if ((c_p->next = alloc_cred(flags, login_context)) != NULL)
	    c_p->next->prev = c_p;
	c_p = c_p->next;
    };
    ret_val = &(c_p->rec);

    UNLOCK_CRED_LIST;
    return ret_val;
}

static void destroy_cred(cred_info * cred) {
/* The referenced cred_info block is assumed locked on entry		    */
/* To avoid deadlock, we have to first mark the cred as being deleted, free */
/* up its lock, run throught the cred_list to find it again, re-acquire its */
/* lock, and finally delete it.						    */
/*									    */

    cred_info_list * c_p;

    cred->delete_pending = 1;
    UNLOCK_ELEMENT(cred);    

    if (LOCK_CRED_LIST) {
	return;
    };


    for (c_p = cred_info_head;
	 c_p != NULL;
	 c_p = c_p->next)
	if (&(c_p->rec) == cred)
	    break;

    if (c_p == NULL) {
	UNLOCK_CRED_LIST;
	return;
    };
/* Now we have found (again) the element we're destroying, and no-one else  */
/* can be waiting for its lock (as we're still holding the list-lock).  Go  */
/* ahead and delete the element.					    */

    LOCK_ELEMENT(&(c_p->rec));
/* Not really necessary, but (I guess) good practice.			    */

    if (c_p->prev) {
	c_p->prev->next = c_p->next;
    } else {
	cred_info_head = c_p->next;
    };
    if (c_p->next) c_p->next->prev = c_p->prev;

    UNLOCK_ELEMENT(&(c_p->rec));

    free (c_p);

    UNLOCK_CRED_LIST;
}

static gss_cred_id_t cred_info_to_gss(cred_info * cred_ptr) {
/* The referenced cred_info is assumed to be locked on entry.		    */
    cred_info_list * c_p;
    gss_cred_id_t ret_val;

    if (LOCK_CRED_LIST) {
	dce_svc_printf(GSS_S_CITG_NOLOCK_CRED_MSG);
	return GSS_C_NO_CREDENTIAL;
    };    

    for (c_p = cred_info_head;
	c_p != NULL;
	c_p = c_p->next)
    {
	if (&(c_p->rec) == cred_ptr) {
	    ret_val = (gss_cred_id_t)(c_p->id);
	    UNLOCK_CRED_LIST;
	    return ret_val;
	};
    };
    UNLOCK_CRED_LIST;
    return GSS_C_NO_CREDENTIAL;
}



static ctx_info * resolve_ctx(gss_ctx_id_t ctx_id)
{
    ctx_info_list * c_p;
    ctx_info * ret_val;
    
    if (LOCK_CTX_LIST) {
	dce_svc_printf(GSS_S_RESOLVE_CTX_NOLOCK_MSG);
	return NULL;
    };

    for (c_p = ctx_info_head;
	c_p != NULL;
	c_p = c_p->next)
    {
	if (ctx_id == c_p->id) {
	    ret_val = &(c_p->rec);
	    LOCK_ELEMENT(ret_val);
	    if (ret_val->delete_pending) {
		UNLOCK_ELEMENT(ret_val);
		ret_val = NULL;
	    };
	    UNLOCK_CTX_LIST;
	    return ret_val;
	};
    };
    UNLOCK_CTX_LIST;
    return NULL;
}

static ctx_info_list * alloc_ctx(void) {
/* Caution - CTX_LIST_LOCK must be locked on entry!			    */
    ctx_info_list * c_p;
    if ((c_p = (ctx_info_list *)malloc(sizeof(ctx_info_list))) == NULL) {
	dce_svc_printf(GSS_S_ALLOC_CTX_NOMEM_MSG);
	return NULL;
    };
    c_p->prev = NULL;
    c_p->next = NULL;
    if ((c_p->id = make_ctx_id(&gssapi_next_ctx_id)) == 0) {
	free(c_p);
	return NULL;
    };
    memset(&(c_p->rec), 0, sizeof(ctx_info));
    pthread_mutex_init(&(c_p->rec.mutex), gssapi_element_mutex_attributes);
    LOCK_ELEMENT(&(c_p->rec));
    c_p->rec.state = CTX_C_ST_CLOSED;
    c_p->rec.snd_seq = 0;
    c_p->rec.rcv_seq = 0;
    c_p->rec.local_cred = GSS_C_NO_CREDENTIAL;
    c_p->rec.deleg_cred = GSS_C_NO_CREDENTIAL;
    return c_p;
}

static ctx_info * create_ctx(void) {
    ctx_info_list * c_p;

    if (LOCK_CTX_LIST) {
	dce_svc_printf(GSS_S_CREATE_CTX_NOLOCK_MSG);
	return NULL;
	return NULL;
    };
    c_p = ctx_info_head;
    if (c_p == NULL) {
	if ((c_p = ctx_info_head = alloc_ctx()) == NULL) {
	    UNLOCK_CTX_LIST;
	    return NULL;
	};
    } else {
	while (c_p->next != NULL) c_p = c_p->next;
	if ((c_p->next = alloc_ctx()) != NULL)
	    c_p->next->prev = c_p;
	c_p = c_p->next;
    };
    
    UNLOCK_CTX_LIST;
    return &(c_p->rec);
}

static void destroy_ctx(ctx_info * ctx) {
/* See comments in destroy_cred						    */
    ctx_info_list * c_p;

    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, 
                   svc_c_debug9, "Destroying a security context"));

    ctx->delete_pending = 1;
    UNLOCK_ELEMENT(ctx);

    if (LOCK_CTX_LIST) {
	dce_svc_printf(GSS_S_DESTROY_CTX_NOLOCK_MSG);
	return;
    };
    for (c_p = ctx_info_head;
	 c_p != NULL;
	 c_p = c_p->next)
	if (&(c_p->rec) == ctx)
	    break;

    if (c_p == NULL) {
	UNLOCK_CTX_LIST;
	return;
    };

    LOCK_ELEMENT(&(c_p->rec));
    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, 
                   svc_c_debug9, "Destroy_ctx - Removing from queue..."));

    if (c_p->prev) {
	c_p->prev->next = c_p->next;
    } else {
	ctx_info_head = c_p->next;
    };
    if (c_p->next) c_p->next->prev = c_p->prev;
    UNLOCK_ELEMENT(&(c_p->rec));
    free (c_p);
    UNLOCK_CTX_LIST;
}

static gss_ctx_id_t ctx_info_to_gss(ctx_info * ctx_ptr) {
    ctx_info_list * c_p;
    gss_ctx_id_t ret_val;
    
    if (LOCK_CTX_LIST) {
	dce_svc_printf(GSS_S_CITG_NOLOCK_CTX_MSG);
	return GSS_C_NO_CONTEXT;
    };
    
    for (c_p = ctx_info_head;
	c_p != NULL;
	c_p = c_p->next)
    {
	if (&(c_p->rec) == ctx_ptr) {
	    ret_val = (gss_ctx_id_t)(c_p->id);
	    UNLOCK_CTX_LIST;
	    return ret_val;
	};
    };
    UNLOCK_CTX_LIST;
    return GSS_C_NO_CONTEXT;
}

static void tidy_cred(cred_info * cred_ptr) {
    error_status_t st;
    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, 
                   svc_c_debug9, "Tidying a credential"));
    if (cred_ptr->context_owner && cred_ptr->login_context != NULL) {
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, 
                       svc_c_debug9, "Purging owned context (%lx)", 
                       (long)(cred_ptr->login_context)));
	sec_login_purge_context(&cred_ptr->login_context, &st);
	if (st) DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, 
                               svc_c_debug9, "Purge failed (%x)", st));
	cred_ptr->context_owner = 0;
    };
    cred_ptr->login_context = NULL;
}

static void set_cred_manage_context(cred_info * cred_ptr) {
    cred_ptr->context_owner = 1;
}

static void set_cred_unmanage_context(cred_info * cred_ptr) {
    cred_ptr->context_owner = 0;
}

static void tidy_ctx(ctx_info * ctx_ptr) {
    cred_info * cred_ptr;

    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, 
                   svc_c_debug8, "Tidying a security context"));

    if (ctx_ptr->auth_session_key != NULL) {
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, 
                       svc_c_debug9, "Deleting session key..."));
	krb5_free_keyblock(ctx_ptr->auth_session_key);
	ctx_ptr->auth_session_key = NULL;
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, "Done"));
    };

    if (ctx_ptr->priv_session_key != NULL) {
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general,
                       svc_c_debug9, "Deleting privacy key..."));
	krb5_free_keyblock(ctx_ptr->priv_session_key);
	ctx_ptr->priv_session_key = NULL;
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, "Done"));
    };

    if (ctx_ptr->safe_session_key != NULL) {
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, 
                       svc_c_debug9, "Deleting integrity key..."));
	krb5_free_keyblock(ctx_ptr->safe_session_key);
	ctx_ptr->safe_session_key = NULL;
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, "Done"));
    };
    if (ctx_ptr->priv_key_schedule != NULL) {
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, 
                       svc_c_debug9, "Deleting privacy key-schedule..."));
	free(ctx_ptr->priv_key_schedule);
	ctx_ptr->priv_key_schedule = NULL;
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, "Done"));
    };
    if (ctx_ptr->safe_key_schedule != NULL) {
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, 
                       svc_c_debug9, "Deleting integrity key-schedule..."));
	free(ctx_ptr->safe_key_schedule);
	ctx_ptr->safe_key_schedule = NULL;
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, "Done"));
    };
    if (ctx_ptr->md_init) {
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, 
                       svc_c_debug9, "Deleting MD2.5 initializer..."));
	free(ctx_ptr->md_init);
	ctx_ptr->md_init = NULL;
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, "Done"));
    };
    if (ctx_ptr->actual_mech_type != NULL) {
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, 
                       svc_c_debug9, "Deleting mech-type OID..."));
	nm_oid_free(ctx_ptr->actual_mech_type, 1);
	ctx_ptr->actual_mech_type = NULL;
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, "Done"));
    };
    if (ctx_ptr->client_name != NULL) {
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, 
                       svc_c_debug9, "Deleting client name..."));
	free(ctx_ptr->client_name);
	ctx_ptr->client_name = NULL;
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, "Done"));
    };
    if (ctx_ptr->client_creds.magic != 0) {
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, 
                       svc_c_debug9, "Deleting client creds..."));
	free_creds(&ctx_ptr->client_creds);
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, "Done"));
    };

	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, 
                       svc_c_debug9, "Deleting V1 PAC..."));
    free_pac(&ctx_ptr->client_pac);
    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, "Done"));

    if (ctx_ptr->ticketted_name != NULL) {
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, 
                       svc_c_debug9, "Deleting ticketted name..."));
	free(ctx_ptr->ticketted_name);
	ctx_ptr->ticketted_name = NULL;
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, "Done"));
    };
    if (ctx_ptr->server_name != NULL) {
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, 
                       svc_c_debug9, "Deleting server name..."));
	nm_oid_free(ctx_ptr->actual_mech_type, 1);
	ctx_ptr->server_name = NULL;
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, "Done"));
    };
    if (ctx_ptr->one_time_cred) {
/* Although theoretically risky, this can't (shouldn't?) result in a	    */
/* deadlock, as the only way to find this credential ought to be via the    */
/* context we already have a lock on, as the credential is a temporary,	    */
/* one-time cred.							    */
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, 
                       svc_c_debug9, "Deleting managed credential..."));
	if ((cred_ptr = resolve_cred(ctx_ptr->local_cred)) != NULL) {
	    tidy_cred(cred_ptr);
	    destroy_cred(cred_ptr);
	};
	ctx_ptr->one_time_cred = 0;
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, "Done"));
    };
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, 
                       svc_c_debug8, "Security context tidied."));
}




/****************************************************************************/
/*									    */
/*	                       Registry stuff				    */
/*									    */
/****************************************************************************/



static OM_uint32 get_auxiliary_context(
			    cred_info * cred_ptr,
			    sec_login_handle_t * login_context_ptr) { 
    error_status_t error_status;
    sec_login_handle_t login_context;
    signed32 expiration;
    int synthesize;
    unsigned32 used_kvno;
    boolean32 reset_passwd;
    sec_login_auth_src_t auth_src;
    void * arg = NULL;

/* We have to first check whether we've got a context in the credential.    */
/* If so (and if it hasn't expired) we can use that.			    */

    login_context = (sec_login_handle_t) 0;
    *login_context_ptr = login_context;

    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, 
                   svc_c_debug9, "Entering get_auxiliary_context..."));
    synthesize = 0;

/* Try the easy one first.  Does the context have a valid pre-existing	    */
/* login-context?							    */

    if (cred_ptr->login_context != NULL) {
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, 
                       svc_c_debug9, "Cred contains login-context (simple case)"));
	login_context = cred_ptr->login_context;
	sec_login_get_expiration(login_context,
				 &expiration,
				 &error_status);
	if (error_status == sec_login_s_not_certified) {
	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, 
                           svc_c_debug8, 
                           "Login-context not certified in get_auxiliary_context"));
	    error_status = 0;
	};
	if (error_status) {
	    dce_svc_printf(GSS_S_GAC_ERR_CTX_EXP_MSG, error_status);
	    login_context = (sec_login_handle_t) 0;
	    goto get_auxiliary_context_return;
	};
	if (expiration < 1) {
	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, 
                           svc_c_debug9, 
                           "Login-context has expired.  Continuing..."));
	    synthesize = 1;
	};
    } else {
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, 
                       svc_c_debug9, "Credential has no base login-context."));
	synthesize = 1;
    };
    
    if (synthesize) {

/* Something's wrong with the credential's login context - either it	    */
/* doesn't exist, or it's expired.  Check whether we've already got an	    */
/* auxiliary login-context.						    */


	if (cred_ptr->lookup_login_context_valid) {
/* We've already created an auxiliary login context for this credential.    */
/* Is it still valid?							    */

	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, 
                           svc_c_debug9, "Found auxiliary login-context"));

	    login_context = cred_ptr->lookup_login_context;
	    sec_login_get_expiration(login_context,
				 &expiration,
				 &error_status);
	    if (error_status == sec_login_s_not_certified) {
		DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, 
                               svc_c_debug8, 
                               "Login-context not certified in get_auxiliary_context"));
		error_status = 0;
	};
	    if (error_status) {
		dce_svc_printf(GSS_S_GAC_ERR_CTX_EXP1_MSG, error_status);
		login_context = (sec_login_handle_t) 0;
		goto get_auxiliary_context_return;
	    };
	    if (expiration < 1) {
		DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, 
                               svc_c_debug9, 
                               "Auxiliary login-context has expired.  Continuing..."));
	    } else {
		DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, 
                               svc_c_debug9, "Existing auxiliary context is OK."));
		synthesize = 0;
	    };
	};
    };

    if (synthesize) {
/* We still haven't found a valid login-context.  Either there isn't an	    */
/* auxiliary login-context in the cred, or it's expired.  Either way,	    */
/* create a new one...							    */

	if (!cred_ptr->lookup_login_context_valid) {

	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, 
                           svc_c_debug8, 
                           "Suitable login-context not found - synthesizing one..."));
	    if (cred_ptr->principal_name_valid) {	    

		DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, 
                               svc_c_debug9, 
                               "Creating login-context from princ name '%s'",
			     cred_ptr->principal_name));
		lookup_reg_server_arg(cred_ptr->principal_name,
				      &arg,
				      &error_status);
		if (error_status) {
		    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                                   "Lookup_reg_server_arg failed (%x)",
				 error_status));
		    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                                   "Principal name: %s",
				 cred_ptr->principal_name));
		} else {
		    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                                   "Returned from lookup_registered_server_arg"));
		};

		DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                               "Calling sec_login_setup_identity(%s)", 
                               cred_ptr->principal_name));

		if (!sec_login_setup_identity(
			cred_ptr->principal_name,
			sec_login_credentials_private,
			&cred_ptr->lookup_login_context,
			&error_status)) {
		    dce_svc_printf(GSS_S_GAC_SLS_FAILED_MSG, error_status);
		    login_context = (sec_login_handle_t) 0;
		    goto get_auxiliary_context_return;
		};

		DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                               "Returned from sec_login_setup_identity"));
		login_context = cred_ptr->lookup_login_context;

	    } else {
/* This is a problem.  We're trying to find out the name corresponding to   */
/* the UUID in this credential.  The credential therefore doesn't contain a */
/* name, so we can't find the corresponding key, and it also doesn't	    */
/* contain a login context of its own.  What do we do here?		    */
/* For the moment, just try the default context.			    */	
		dce_svc_printf(GSS_S_GAC_CRED_INV_MSG);
		login_context = (sec_login_handle_t) 0;
		error_status = sec_login_s_context_invalid;
		goto get_auxiliary_context_return;
	    };
	} else {
	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                           "Refreshing existing auxiliary login-context"));
	};

/* Now we've got a login-context, but it needs validating. Make it so...    */


	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                       "Calling sec_login_valid_from_keytable..."));
	sec_login_valid_from_keytable(
			  cred_ptr->lookup_login_context,
			  rpc_c_authn_dce_secret,
			  arg,
			  0,
			  &used_kvno,
			  &reset_passwd,
			  &auth_src,
			  &error_status);

	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                       "Returned from sec_login_valid_from_keytable"));
	if (error_status != error_status_ok) {
	    dce_svc_printf(GSS_S_GAC_SLV_FAILED_MSG, error_status);
	    login_context = (sec_login_handle_t) 0;
	    goto get_auxiliary_context_return;
	};
	cred_ptr->lookup_login_context_valid = 1;
	login_context = cred_ptr->lookup_login_context;
	error_status = 0;
    };
	    
get_auxiliary_context_return:
    *login_context_ptr = login_context;
    return error_status;
}

static OM_uint32 fixup_registry_context(cred_info * cred_ptr) { 
    sec_rgy_name_t site_name;
    sec_login_handle_t login_context;
    sec_rgy_bind_auth_info_t binding_info;
    int retry_count = 0;
    OM_uint32 o_status;
    error_status_t error_status;

    site_name[0] = 0;
    o_status = GSSDCE_S_SUCCESS;

    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                   "Entering fixup_registry_context..."));
		   
retry:
    if (!cred_ptr->lookup_registry_context_valid) {

	o_status = get_auxiliary_context(cred_ptr, &login_context);

	if (o_status) {
	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                           "Couldn't get auxiliary context for rgy lookup (%x)", o_status));
	    goto fixup_registry_context_return;
	};

/* Now login_context points to the login context we're going to use for	    */
/* registry lookup.  Bind to an appropriate registry with that context...   */

	binding_info.info_type = sec_rgy_bind_auth_dce;
	binding_info.tagged_union.dce_info.authn_level = rpc_c_authn_level_default;
	binding_info.tagged_union.dce_info.authn_svc = rpc_c_authn_dce_secret;
	binding_info.tagged_union.dce_info.authz_svc = rpc_c_authz_dce;
	binding_info.tagged_union.dce_info.identity = login_context;
	
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                       "Binding to registry..."));
	sec_rgy_site_bind(site_name,
			  &binding_info,
			  &cred_ptr->lookup_registry_context,
			  &error_status);

	if (error_status == error_status_ok) {
	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                           "Bind succeeded"));
	    cred_ptr->lookup_registry_context_valid = 1;
	} else {
	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug1, 
                           "Registry bind attempt failed (%x)", error_status));
	    if (retry_count < MAX_RGY_RETRIES) {
		retry_count++;
		dce_svc_printf(GSS_S_FRC_BIND_RETRYING_MSG, error_status);
		site_name[0] = 0;
		goto retry;
	    };
	    dce_svc_printf(GSS_S_FRC_RETRY_EXCEEDED_MSG, error_status);
	    o_status = gssdce_s_no_rgy_available;
	    goto fixup_registry_context_return;
	};
    };

fixup_registry_context_return:
    return o_status;
}

static OM_uint32 cred_expand_id(cred_info * cred_ptr) {
/* takes a cred containing either a string name or a UUID, and expands it   */
/* to contain both name-forms.						    */


    error_status_t error_status;
    OM_uint32 o_status = gssdce_s_operation_incomplete;
    int retry_count = 0;
    



retry:

    if ((o_status = fixup_registry_context(cred_ptr)) != gssdce_s_success) {
	dce_svc_printf(GSS_S_CEI_FRC_FAILED_MSG);
	goto cred_expand_id_return;
    };

/* We now have a registry binding handle in the credential.  Decide whether */
/* we need to do UUID->name or name->UUID.				    */

    if (cred_ptr->principal_name_valid) {
/* We don't need to do UUID -> name.					    */
	if (cred_ptr->principal_uuid_valid) {
/* We don't need to do anything...					    */
	    o_status = gssdce_s_success;
	    goto cred_expand_id_return;
	} else {
/* We need to translate name -> UUID.					    */
	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                           "Cred_expand_id - Calling sec_id_parse_name"));
	    sec_id_parse_name(cred_ptr->lookup_registry_context,
			      cred_ptr->principal_name,
			      cred_ptr->principal_realm_name,
			      &cred_ptr->principal_realm_uuid,
			      cred_ptr->principal_local_name,
			      &cred_ptr->principal_uuid,
			      &error_status);
	    if (error_status == error_status_ok) {
		cred_ptr->principal_uuid_valid = 1;
		o_status = gssdce_s_success;
		goto cred_expand_id_return;
	    } else if (error_status == sec_id_e_foreign_cell_referral) {
		/* This shouldn't be returned */
		dce_svc_printf(GSS_S_CEI_REFERRAL_MSG);
		o_status = gssdce_s_no_rgy_available;
		goto cred_expand_id_return;
	    } else if (error_status == sec_rgy_server_unavailable) {
		dce_svc_printf(GSS_S_CEI_SERVER_UNAVAILABLE_MSG);
		if (retry_count++ < MAX_RGY_RETRIES) {
		    sec_rgy_site_close(cred_ptr->lookup_registry_context,
				       &error_status);
		    cred_ptr->lookup_registry_context_valid = 0;
		    goto retry;
		};
		o_status = gssdce_s_no_rgy_available;
	    } else {
		dce_svc_printf(GSS_S_CEI_PARSE_FAILED_MSG, error_status);
		o_status = gssdce_s_no_rgy_available;
		goto cred_expand_id_return;
	    };
	};
    } else {
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                       "Cred_expand_id - Calling sec_id_gen_name..."));
	sec_id_gen_name(cred_ptr->lookup_registry_context,
			&cred_ptr->principal_realm_uuid,
			&cred_ptr->principal_uuid,
			cred_ptr->principal_name,
			cred_ptr->principal_realm_name,
			cred_ptr->principal_local_name,
			&error_status);

	if (error_status == error_status_ok) {
	    cred_ptr->principal_name_valid = 1;
	    o_status = gssdce_s_success;
	    goto cred_expand_id_return;
	} else if (error_status == sec_id_e_foreign_cell_referral) {
	    dce_svc_printf(GSS_S_CEI_REFERRAL_MSG);
	    goto cred_expand_id_return;
	} else if (error_status == sec_rgy_server_unavailable) {
	    dce_svc_printf(GSS_S_CEI_SERVER_UNAVAILABLE_MSG);
	    if (retry_count++ < MAX_RGY_RETRIES) {
		sec_rgy_site_close(cred_ptr->lookup_registry_context,
				   &error_status);
		cred_ptr->lookup_registry_context_valid = 0;
		goto retry;
	    };
	    o_status = gssdce_s_no_rgy_available;
	} else {
	    dce_svc_printf(GSS_S_CEI_GN_FAILED_MSG, error_status);
	    o_status = gssdce_s_couldnt_translate_name;
	    goto cred_expand_id_return;
	};
    };
cred_expand_id_return:
    return o_status;
}



/****************************************************************************/
/*			    pac_to_name					    */
/****************************************************************************/
static error_status_t pac_to_name(ctx_info * ctx_ptr) {
    error_status_t error_status;
    error_status_t st;
    int retry_count = 0;
    sec_rgy_name_t temp_name;
    cred_info * cred_ptr = NULL;

    sec_cred_pa_handle_t    client_creds;
    sec_id_pa_t * client_identity;

    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                   "Entering pac_to_name to determine client's name"));



    if ((cred_ptr = resolve_cred(ctx_ptr->local_cred)) == NULL) {
        error_status = gssdce_s_invalid_handle;
	dce_svc_printf(GSS_S_PTN_INV_CRED_MSG);
	goto pac_to_name_return;
    };

retry:

    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                   "Binding to registry..."));
    if ((error_status = fixup_registry_context(cred_ptr)) != gssdce_s_success) {
	dce_svc_printf(GSS_S_PTN_FRC_FAILED_MSG, error_status);
	goto pac_to_name_return;
    };

    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                   "Registry bound"));
/* We have a binding handle.  The next stage is finding the UUID of the
 * context's client principal...	   			    
 */

    client_creds = sec_cred_get_initiator(ctx_ptr->client_creds,
					  &error_status);

    if (error_status != rpc_s_ok) {
	dce_svc_printf(GSS_S_PTN_SCG_FAILED_MSG, error_status);
	goto pac_to_name_return;
    };

    client_identity = sec_cred_get_pa_data(client_creds,
					   &error_status);

    if (error_status != rpc_s_ok) {
	dce_svc_printf(GSS_S_PTN_SCGP_FAILED_MSG, error_status);
	goto pac_to_name_return;
    };


    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                   "Calling sec_id_gen_name..."));

   sec_id_gen_name(cred_ptr->lookup_registry_context,
		    &client_identity->realm.uuid,
		    &client_identity->principal.uuid,
		    temp_name,
		    NULL,
		    NULL,
		    &error_status);

    sec_cred_free_pa_handle(&client_creds, &st);
/* Don't have to free client_identity here, as that's just a pointer into   */
/* the pa_handle data.							    */

    if (error_status == sec_id_e_foreign_cell_referral) {
	dce_svc_printf(GSS_S_PTN_REFERRAL_MSG);
	goto pac_to_name_return;
    } else if (error_status == sec_rgy_server_unavailable) {

	if (retry_count++ < MAX_RGY_RETRIES) {
	    dce_svc_printf(GSS_S_PTN_SERVER_UNAVAILABLE_MSG);
	    sec_rgy_site_close(cred_ptr->lookup_registry_context,
			       &error_status);
	    cred_ptr->lookup_registry_context_valid = 0;
	    goto retry;
	};
	dce_svc_printf(GSS_S_PTN_RETRY_EXCEEDED_MSG);
	goto pac_to_name_return;
    } else if (error_status != rpc_s_ok) {
	dce_svc_printf(GSS_S_PTN_GN_FAILED_MSG, error_status);
	goto pac_to_name_return;
    };

    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                   "Got client's name from registry"));

    if (ctx_ptr->client_name != NULL) {
	dce_svc_printf(GSS_S_PTN_NOT_NULL_MSG);
	free(ctx_ptr->client_name);
    };

    ctx_ptr->client_name = (unsigned char *)malloc(strlen((char *)temp_name)+1);

    if (ctx_ptr->client_name) {
	strcpy((char *)ctx_ptr->client_name, (char *)temp_name);	
	(ctx_ptr->client_name)[strlen((char *)temp_name)] = '\0';
    } else {
	error_status = gssdce_s_out_of_memory;
	dce_svc_printf(GSS_S_PTN_NOMEM_MSG);
    };
pac_to_name_return:



    if (cred_ptr) {
	UNLOCK_ELEMENT(cred_ptr);
    };
    return error_status;
}


/****************************************************************************/
/*			    ctx_to_name					    */
/****************************************************************************/

static error_status_t ctx_to_name(ctx_info * ctx_ptr) {
    error_status_t error_status;
    unsigned_char_p_t temp_name;

    temp_name = NULL;

    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                   "Entering ctx_to_name to determine client's name"));



/* First try asking sec_cred to find the name for us.  This probably won't  */
/* work, but in future a name might be included in a PAC, and if so this    */
/* routine might suddenly start working...				    */


    sec_cred_get_client_princ_name(ctx_ptr->client_creds,
				   &temp_name,
				   &error_status);

    if (error_status) {

	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug1, 
                       "sec_cred_get_client_princ_name failed, calling pac_to_name instead"));


	return pac_to_name(ctx_ptr);
    };

    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                   "Got client's name."));
    if (ctx_ptr->client_name != NULL) {
	dce_svc_printf(GSS_S_CTN_NOT_NULL_MSG);
	free(ctx_ptr->client_name);
    };

    ctx_ptr->client_name = temp_name;
    temp_name = NULL;

ctx_to_name_return:
    if (temp_name) free(temp_name);
    return error_status;
}

/****************************************************************************/
/*									    */
/*	                   OID management routines			    */
/*									    */
/****************************************************************************/


static void nm_oid_copy(gss_OID src,
		       gss_OID * dst) {
    if (dst == NULL) return;
    if ((*dst = (gss_OID_desc *)malloc(sizeof(gss_OID_desc))) == NULL) {
	dce_svc_printf(GSS_S_OC_NOMEM_MSG);
	return;
    };
    memset(*dst, 0, sizeof(gss_OID_desc));
    (*dst)->length = src->length;
    if (((*dst)->elements = (void *)malloc(src->length)) == NULL) {
	free(*dst);
	*dst = NULL;
	dce_svc_printf(GSS_S_OC_NOMEM_MSG);
	return;
    };
    memcpy((*dst)->elements, src->elements, src->length);
}

OM_uint32 gssdce_test_oid_set_member(OM_uint32* minor_status,
				     gss_OID member,
				     gss_OID_set set,
				     int * present) {
    GSS_INIT;
    *present = nm_oid_set_test_member(member, set);
    *minor_status = gssdce_s_success;
    return GSS_S_COMPLETE;
}

static int nm_oid_set_test_member(gss_OID member,
				  gss_OID_set set) {
/* Returns non-zero if the member is in the set				    */
    int i;

    if (set == NULL) return 0;
    for (i=0;
         i<set->count;
	 i++) {
	if (set->elements[i].length != member->length) continue;
	if (!memcmp(set->elements[i].elements,
		    member->elements,
		    member->length)) return 1;
     };

    return 0;

}

static void nm_oid_free(gss_OID oid, int free_desc) {
#if defined(VERBOSE_DEBUG_MESSAGES)
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                       "Entering nm_oid_free for OID at %lx", (long)oid));
#endif
    if (oid == NULL) {
#if defined(VERBOSE_DEBUG_MESSAGES)
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                       "OID is NULL; ignoring..."));
#endif
	return;
    };
    if ((oid == GSSDCE_C_OID_DCE_KRBV5_DES) ||
	(oid == GSSDCE_C_OID_KRBV5_DES_ID) ||
	(oid == GSSDCE_C_OID_KRBV5_DES_RFC) ||
	(oid == GSSDCE_C_OID_DCENAME)) {
#if defined(VERBOSE_DEBUG_MESSAGES)
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                       "OID is a GSSAPI constant; ignoring..."));
#endif
	return;
    };
    if (oid->length != 0) {
	if ((oid->elements != GSSDCE_C_OID_DCE_KRBV5_DES->elements) &&
	    (oid->elements != GSSDCE_C_OID_KRBV5_DES_ID->elements) &&
	    (oid->elements != GSSDCE_C_OID_KRBV5_DES_RFC->elements) &&
	    (oid->elements != GSSDCE_C_OID_DCENAME->elements)) {
	    if (oid->elements != NULL) {
#if defined(VERBOSE_DEBUG_MESSAGES)
		DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                               "Freeing OID elements (%lx)", 
			     (long)(oid->elements)));
#endif
		free(oid->elements);
	    } else {
#if defined(VERBOSE_DEBUG_MESSAGES)
		DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                               "NULL elements pointer; ignoring"));
#else
		;
#endif
	    };
	} else {
#if defined(VERBOSE_DEBUG_MESSAGES)
	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                           "OID value is a GSSAPI static; ignoring..."));
#else
	    ;
#endif
	};
    };
#if defined(VERBOSE_DEBUG_MESSAGES)
    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                   "Zeroing OID descriptor..."));
#endif
    oid->elements = NULL;
    oid->length = 0;

    if (free_desc) {

#if defined(VERBOSE_DEBUG_MESSAGES)
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                       "Freeing OID descriptor"));
#endif

	free(oid);
    };
}

static void nm_oid_set_free(gss_OID_set * set) {
    gss_OID oid_list;
    int i,j, elem_count;
    gss_OID elemental_oid;

#if defined(VERBOSE_DEBUG_MESSAGES)
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                       "Entering nm_oid_set_free"));
#endif

    if (set == NULL) {
#if defined(VERBOSE_DEBUG_MESSAGES)
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                       "Set pointer is NULL"));
#endif
	return;
    };
    if ((*set) == NULL) {
#if defined(VERBOSE_DEBUG_MESSAGES)
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                       "Set is NULL"));
#endif
	return;
    };

    elem_count = (*set)->count;

#if defined(VERBOSE_DEBUG_MESSAGES)
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                       "Entering loop to free %d elements", elem_count));

	for (j=0; j<elem_count; j++) {
	    elemental_oid = &((*set)->elements[j]);
	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                           "Element %d: Oid: %lx, length: %d, Elements: %lx", 
			 j, 
			 (long)elemental_oid,
			 elemental_oid->length,
			 (long)(elemental_oid->elements)));
	};
#endif


    i = 0;
    while ((*set)->count--) {
#if defined(VERBOSE_DEBUG_MESSAGES)
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                       "Freeing member OID %d", i));

	for (j=0; j<elem_count; j++) {
	    elemental_oid = &((*set)->elements[j]);
	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                           "Element %d: Oid: %lx, length: %d, Elements: %lx", 
			 j, 
			 (long)(elemental_oid),
			 elemental_oid->length,
			 (long)(elemental_oid->elements)));
	};

#endif
	oid_list = &((*set)->elements[i++]);
	nm_oid_free( oid_list, 0 ); /* Free the individual OIDs  */
    
    };
#if defined(VERBOSE_DEBUG_MESSAGES)
    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                   "Freeing the member list"));
#endif
    free((*set)->elements ); /* Free the list of pointers		    */
#if defined(VERBOSE_DEBUG_MESSAGES)
    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                   "Freeing the set descriptor"));
#endif
    free(*set);	    /* Free the descriptor				    */
}

OM_uint32 gssdce_create_empty_oid_set(OM_uint32* minor_status,
				      gss_OID_set * set) {
    GSS_INIT;
    nm_oid_set_create_empty(set);
    *minor_status = gssdce_s_success;
    return GSS_S_COMPLETE;
}

static void nm_oid_set_create_empty(gss_OID_set * set) {
/* Creates an empty OID set						    */
    if ((*set = (gss_OID_set)malloc(sizeof(gss_OID_set_desc))) == NULL) {
	dce_svc_printf(GSS_S_OSC_NOMEM_MSG);
	return;
    };
    (*set)->count = 0;
    (*set)->elements = NULL;
}

OM_uint32 gssdce_add_oid_set_member(OM_uint32 * minor_status,
				    gss_OID member,
				    gss_OID_set * set) {
    GSS_INIT;
    nm_oid_set_add_elem(set, member);
    *minor_status = gssdce_s_success;
    return GSS_S_COMPLETE;
}

static void nm_oid_set_add_elem(gss_OID_set * set,
			        gss_OID element) {
/* Adds an element to an OID set					    */
    if (set == NULL) return;
    if (*set == NULL) {
	nm_oid_set_create_empty(set);
    };
    (*set)->elements = (gss_OID_desc *) realloc((*set)->elements,
			       ++((*set)->count) * sizeof(gss_OID_desc));
    (*set)->elements[(*set)->count-1].length = element->length;
    if (((*set)->elements[(*set)->count-1].elements =
	 (void *)malloc(element->length)) == NULL) {
	(*set)->elements[(*set)->count-1].length = 0;
	dce_svc_printf(GSS_S_OSA_NOMEM_MSG);
    } else {
	memcpy((*set)->elements[(*set)->count-1].elements,
	       element->elements,
	       element->length);    
    };
}

static int nm_oid_equal(gss_OID oid1, gss_OID oid2) {
    if (oid1 == oid2) return 1;
    if ((oid1 == NULL) || (oid2 == NULL)) return 0;
    if (oid1->length != oid2->length) return 0;
    return (!memcmp(oid1->elements, oid2->elements, oid1->length));
}


/****************************************************************************/
/*									    */
/*	                Miscellaneous window stuff			    */
/*									    */
/****************************************************************************/

static unsigned char zero_block[8] = {0, 0, 0, 0, 0, 0, 0, 0};

/* Window runs from reference - LOW_WINDOW_SIZE to reference +		    */
/* HIGH_WINDOW_SIZE.							    */

/* Unless TRACK_HIGH_WINDOW is defined, the high_window and cur_window	    */
/* regions are 'virtual' - that is, no data is actually stored there, as    */
/* the current windowing algorithm never needs to use these regions.	    */

#if !defined(DEBUG_ANY)
static
#endif
long seq_number_difference(SEQUENCE_NO candidate,
			  SEQUENCE_NO reference) {
/* Return the modular difference between the reference and the candidate.   */
    long mod_diff;
    if (candidate == reference) return 0;
    if (GTR(candidate, reference)) {
	mod_diff = candidate - reference;
	if (GTR(mod_diff, MAX_SEQ / 2)) mod_diff = mod_diff - MAX_SEQ;
    } else {
	mod_diff = reference - candidate;
	if (GTR(mod_diff , MAX_SEQ / 2)) mod_diff = mod_diff - MAX_SEQ;
	mod_diff = - mod_diff;
    };
    return mod_diff;
}




#if !defined(DEBUG_ANY)
static
#endif
int compare_seq_numbers(SEQUENCE_NO candidate,
		        SEQUENCE_NO reference) {
/* Compare_seq_numbers returns 0 for equal, -1 for 1st argument in window   */
/* but less than 2nd, +1 for 1st argument in window but greater than 2nd,   */
/* and -2 or +2 for significant difference between 1st and second	    */
/* arguments.  Note that there isn't any significance between 2 & -2.	    */

    long diff = seq_number_difference(candidate, reference);
    if (diff == 0) return 0;
    else if (diff < 0 && diff >= -LOW_WINDOW_SIZE) return -1;
    else if (diff > 0 && diff <= HIGH_WINDOW_SIZE) return 1;
    else if (diff < 0) return -2;
    else return 2;
}

static int token_received(ctx_info * ctx_ptr,
			  SEQUENCE_NO received_seq_no) {
/* First update the window so that it's centered on the current rcv_seq.    */
/* Then insert the new token.  Return 1 if the token has already been seen. */
    long mod_diff;
    long i;
    int already_received;
    SEQUENCE_NO cur_rcv_seq;


    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                   "Window: Received %ld, current: %ld",
		 (long)received_seq_no,
		 (long)(ctx_ptr->rcv_seq)));

    cur_rcv_seq = ctx_ptr->rcv_seq;


    if (ctx_ptr->window_seq != cur_rcv_seq) {
	mod_diff = seq_number_difference(cur_rcv_seq,
					 ctx_ptr->window_seq);
/* Mod_diff says how far we have to slide the window			    */
	if ((ctx_ptr->window_valid == 0) || (mod_diff > HIGH_WINDOW_SIZE)) {
/* The whole window is invalid.						    */
	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                           "Invalidating entire window"));
	    for (i=0; i<LOW_WINDOW_SIZE; i++) ctx_ptr->low_window[i] = 0;
	    ctx_ptr->window_valid = 1;
#	    ifdef TRACK_HIGH_WINDOW
		for (i=0; i<HIGH_WINDOW_SIZE; i++) ctx_ptr->high_window[i] = 0;
		ctx_ptr->cur_window = 0;
#	    endif
	    ctx_ptr->window_seq = cur_rcv_seq;
	} else if (mod_diff < 0) {
/* Error, we shouldn't be shifting the window backwards.		    */
	    dce_svc_printf(GSS_S_WND_REVERSE_MSG);
	    return 0;
	} else {
/* We should shift the window forward by mod_diff			    */
	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                           "Advancing the window"));
/* Low_window runs from index 0 (one before cur_window) to		    */
/* LOW_WINDOW_SIZE-1 (bottom of window).				    */

/* Shuffle low_window down						    */
	    for (i=LOW_WINDOW_SIZE-1; i>=mod_diff; i--)
		ctx_ptr->low_window[i] = ctx_ptr->low_window[i-mod_diff];

	    if (mod_diff > 0) {
#		ifdef TRACK_HIGH_WINDOW
/* Move cur_window into low_window					    */
		    ctx_ptr->low_window[mod_diff-1] = ctx_ptr->cur_window;
#		else
		    ctx_ptr->low_window[mod_diff-1] = 0;
#		endif
	    };

#	    ifdef TRACK_HIGH_WINDOW
/* Move high_window down, and into cur_window and low_window		    */
		for (i=0; i<HIGH_WINDOW_SIZE; i++)
		    if (i<mod_diff-1)
			ctx_ptr->low_window[mod_diff-2 - i] =
				ctx_ptr->high_window[i];
		    else if (i==mod_diff-1)
			ctx_ptr->cur_window = ctx_ptr->high_window[i];
		    else
			ctx_ptr->high_window[i-mod_diff] = ctx_ptr->high_window[i];
/* Clear top of high_window						    */
		for (i=HIGH_WINDOW_SIZE-mod_diff; i<HIGH_WINDOW_SIZE; i++)
		    ctx_ptr->high_window[i] = 0;
#	    else
		for (i=mod_diff-2; i>=0; i--)
		    ctx_ptr->low_window[i] = 0;
#	    endif

	    ctx_ptr->window_seq = cur_rcv_seq;
	};
    };

    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                   "Window fixed, checking new packet"));

    mod_diff = seq_number_difference(received_seq_no, ctx_ptr->window_seq);


    if (mod_diff == 0) {
#	ifdef TRACK_HIGH_WINDOW
	    already_received = ctx_ptr->cur_window;
	    ctx_ptr->cur_window = 1;
#	else
	    already_received = 0;
#	endif
    } else if (mod_diff > 0 && mod_diff < HIGH_WINDOW_SIZE) {
#	ifdef TRACK_HIGH_WINDOW
	    already_received = ctx_ptr->high_window[mod_diff-1];
	    ctx_ptr->high_window[mod_diff-1] = 1;
#	else
	    already_received = 0;
#	endif
    } else if (mod_diff < 0 && mod_diff >= -LOW_WINDOW_SIZE) {
	already_received = ctx_ptr->low_window[-mod_diff-1];
	ctx_ptr->low_window[-mod_diff-1] = 1;
    } else already_received = 0;

    return already_received;
}

/****************************************************************************/
/*									    */
/*	                   gss_release_cred routine			    */
/*									    */
/****************************************************************************/

OM_uint32 gss_release_cred
                 (OM_uint32* minor_status,
                  gss_cred_id_t* cred_handle) {
    cred_info * c_p;
    OM_uint32 O_status;

    GSS_INIT;
    *minor_status = gssdce_s_operation_incomplete;
    if (cred_handle == NULL) {
	*minor_status = gssdce_s_invalid_handle;
	O_status = GSS_S_DEFECTIVE_CREDENTIAL;
	goto release_cred_return;
    };
    if ((c_p = resolve_cred(*cred_handle)) == NULL) {
        *minor_status = gssdce_s_invalid_handle;
        O_status = GSS_S_DEFECTIVE_CREDENTIAL;
	goto release_cred_return;
    };

    *cred_handle = 0;

    tidy_cred(c_p);

    destroy_cred(c_p);

    *minor_status = gssdce_s_success;
    O_status = GSS_S_COMPLETE;

release_cred_return:
    return O_status;
}


/****************************************************************************/
/*									    */
/*	             gssdce_extract_PAC_from_cred routine		    */
/*									    */
/****************************************************************************/

OM_uint32 gssdce_extract_PAC_from_cred
                (OM_uint32 * minor_status,
                 gss_cred_id_t cred_handle,
                 sec_id_pac_t * output_PAC) {
    cred_info * c_p = NULL;
    error_status_t error_status;
    sec_login_net_info_t net_info;
    sec_login_handle_t login_context;
    
    GSS_INIT;
    *minor_status = gssdce_s_operation_incomplete;

    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                   "Entering gssdce_extract_pac_from_cred..."));

    if (cred_handle == GSS_C_NO_CREDENTIAL) {
/* This is a request to use the default credential.  The default credential */
/* is a "virtual" credential, using the default login-context, and	    */
/* supporting the DCE mech-type for initiate-only use.			    */
/* Extract the default login-context...					    */

	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                       "Request refers to default credential..."));

	sec_login_get_current_context(&login_context,
				      &error_status);
	if (error_status != error_status_ok) {
	    *minor_status = error_status;
	    return GSS_S_NO_CRED;
	};
    } else {
	if ((c_p = resolve_cred(cred_handle)) == NULL) {
	    *minor_status = gssdce_s_invalid_handle;
	    return GSS_S_DEFECTIVE_CREDENTIAL;
	};
/* Check to see whether this credential actually has a login-context.	    */
/* Accept-only credentials don't have login contexts...			    */
	if (!(c_p->flags & CRED_C_FL_INITIATE)) {
	    UNLOCK_ELEMENT(c_p);
	    *minor_status = gssdce_s_no_login_context;
	    return GSS_S_NO_CRED;
	};

	if ((login_context = c_p->login_context) == NULL) {
/* Shouldn't happen, but if it does, return an error.			    */
	    UNLOCK_ELEMENT(c_p);
	    *minor_status = gssdce_s_no_login_context;
	    return GSS_S_FAILURE;
	};
    };
/* Extract the PAC from the login_context.				    */
    sec_login_inquire_net_info(login_context,
			       &net_info,
			       &error_status);
    if (error_status == sec_login_s_context_invalid) {
	sec_login_free_net_info(&net_info);
	UNLOCK_ELEMENT(c_p);
	*minor_status = gssdce_s_credential_corrupt;
	return GSS_S_DEFECTIVE_CREDENTIAL;
/*
    } else if (error_status == sec_login_s_not_certified) {
	sec_login_free_net_info(&net_info);
	UNLOCK_ELEMENT(c_p);
	*minor_status = error_status;
	return GSS_S_FAILURE;
*/
    } else if ((error_status != error_status_ok) 
		&& (error_status != sec_login_s_not_certified)
    ) {
	sec_login_free_net_info(&net_info);
	UNLOCK_ELEMENT(c_p);
	UNEXPECTED_RETURN;
    };

/* Give the PAC to the caller...					    */
    if ((*minor_status = copy_pac(output_PAC, &net_info.pac)) != rpc_s_ok) {
	UNLOCK_ELEMENT(c_p);
	return GSS_S_FAILURE;
    };

/* Free the net-info...							    */
    sec_login_free_net_info(&net_info);

/* The above is slightly long-winded; I could have simply copied the	    */
/* net_info.pac pointer into the output parameter, and then not bothered    */
/* freeing the PAC.  However, the code as it stands is more robust - If a   */
/* net_info structure ever acquires more pointer fields in a future version */
/* of DCE, the code as written will continue to work, whereas the simpler   */
/* form would break.							    */


    UNLOCK_ELEMENT(c_p);
    *minor_status = gssdce_s_success;
    return GSS_S_COMPLETE;
    
}


/****************************************************************************/
/*									    */
/*	             gssdce_login_context_to_cred routine		    */
/*									    */
/****************************************************************************/

OM_uint32 gssdce_login_context_to_cred
                (OM_uint32 * minor_status,
                 sec_login_handle_t login_context,
                 OM_uint32 lifetime_req,
                 gss_OID_set desired_mechs,
                 gss_cred_id_t * output_cred_handle,
                 gss_OID_set * actual_mechs,
                 OM_uint32 * lifetime_rec) {
/* This routine always creates a new credential.			    */
    cred_info_list * c_p;
    cred_info * cred_ptr;
    OM_uint32 desired_mech_flags;
    error_status_t error_status;
#ifndef DCE_V11
    sec_login_net_info_t net_info;
#else
    sec_cred_pa_handle_t cred_pa_handle;
    sec_id_pa_t *sec_id_pas;
    signed32 contextual_lifetime_signed;
#endif
    OM_uint32 contextual_lifetime;

    *minor_status = gssdce_s_operation_incomplete;
    GSS_INIT;

    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                   "Entering login-context_to_cred..."));

    if (output_cred_handle == NULL) {
	*minor_status = gssdce_s_param_required;
	dce_svc_printf(GSS_S_LCC_NULL_MSG);

	return GSS_S_CALL_INACCESSIBLE_WRITE;
    };

    desired_mech_flags = 0;
    if (desired_mechs != NULL) {
	if (nm_oid_set_test_member(GSSDCE_C_OID_DCE_KRBV5_DES,
				   desired_mechs)) {
	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                           "Enabling DCE mech-type"));
	    desired_mech_flags |= CRED_C_FL_DCE1_MECH;
	};
	if (nm_oid_set_test_member(GSSDCE_C_OID_KRBV5_DES,
				   desired_mechs)) {
	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                           "Enabling KRB mech-type"));
	    desired_mech_flags |= CRED_C_FL_KRB5_MECH;
	};
    } else {
	desired_mech_flags = CRED_C_FL_DCE1_MECH;
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                       "Setting default mech-type (DCE-only)"));
    };
    
    desired_mech_flags |= CRED_C_FL_INITIATE;
#ifdef IMPORT_CONTEXTS_TO_BOTH_MODE_CREDS
/* Add in the ability to accept new context too.			    */
    desired_mech_flags |= CRED_C_FL_ACCEPT;
#endif

    if (login_context == NULL) {
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                       "Default login-context, calling sec_login-get_current_context..."));
	sec_login_get_current_context(&login_context,
				      &error_status);
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                       "Returned from sec_login_get_current_context"));
	if (error_status != error_status_ok) {
	    *minor_status = error_status;
	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                           "Login_context_to_cred - Get_current_context failed (%x)", error_status));
	    return GSS_S_NO_CRED;
	};

    };


/* Then we can find out the UUIDs from the login context		    */

#ifndef DCE_V11
/* The code within this #ifdef uses DCE 1.0 PACs.  For DCE 1.1 systems, the */
/* symbol DCE_V11 will be defined, and the alternative version that uses    */
/* the new sec_cred routines will be invoked instead.			    */

    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                   "Calling sec_login_inquire_net_info..."));
    sec_login_inquire_net_info(login_context,
			       &net_info,
			       &error_status);
    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                   "Returned from sec_login_inquire_net_info"));

    if (error_status == sec_login_s_no_current_context) {
	dce_svc_printf(GSS_S_LCC_NO_DEF_CTX_MSG);
	sec_login_free_net_info(&net_info);
	*minor_status = error_status;
	return GSS_S_NO_CRED;
    } else if (error_status == sec_login_s_not_certified) {
	;
    } else if (error_status != error_status_ok) {
	dce_svc_printf(GSS_S_LCC_NO_PAC_MSG, error_status);
	*minor_status = error_status;
	return GSS_S_NO_CRED;
    };

/* We now have the PAC for the desired identity, buried in net_info.pac.    */
/* We may need the server's printable name later, so we have to get the	    */
/* UUIDs now.  To do this, we have to parse the PAC.			    */

    if ((cred_ptr = create_cred(desired_mech_flags, login_context)) == NULL) {
	dce_svc_printf(GSS_S_LCC_CRED_NOMEM_MSG);
	*minor_status = gssdce_s_out_of_memory;
	return GSS_S_FAILURE;
    };

    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                   "Copying PAC data..."));
    cred_ptr->principal_uuid = net_info.pac.principal.uuid;
    cred_ptr->principal_realm_uuid = net_info.pac.realm.uuid;
    cred_ptr->principal_uuid_valid = 1;

    contextual_lifetime = net_info.identity_expiration_date;

    sec_login_free_net_info(&net_info);

#else
/* This is equivalent code to the above, only it uses DCE 1.1 se_cred EPAC  */
/* interrogation routines instead of parsing the PAC itself.  As well as    */
/* being cleaner, this also means that the login-context doesn't have to    */
/* have been built with V10 compatibility mode enabled.			    */

    cred_pa_handle = sec_login_cred_get_initiator(login_context,
						  &error_status);

    if (error_status != error_status_ok &&
	error_status != sec_login_s_not_certified) {

	dce_svc_printf(GSS_S_LCC_CGI_FAILED_MSG, error_status);
	*minor_status = error_status;
	return GSS_S_FAILURE;

    };    

    sec_id_pas = sec_cred_get_pa_data(cred_pa_handle, &error_status);

    if (error_status != error_status_ok &&
	error_status != sec_login_s_not_certified) {

	dce_svc_printf(GSS_S_LCC_GPD_FAILED_MSG, error_status);
	*minor_status = error_status;
	return GSS_S_FAILURE;

    };    


    if ((cred_ptr = create_cred(desired_mech_flags, login_context)) == NULL) {
	dce_svc_printf(GSS_S_LCC_CRED_NOMEM_MSG);
	*minor_status = gssdce_s_out_of_memory;
	return GSS_S_FAILURE;
    };


    cred_ptr->principal_uuid = sec_id_pas->principal.uuid;
    cred_ptr->principal_realm_uuid = sec_id_pas->realm.uuid;
    cred_ptr->principal_uuid_valid = 1;

    sec_login_get_expiration(login_context,
			     &contextual_lifetime_signed,
			     &error_status);
			     
    if (error_status != error_status_ok &&
	error_status != sec_login_s_not_certified) {

	dce_svc_printf(GSS_S_LCC_SLGE_FAILED_MSG, error_status);
	*minor_status = error_status;
	return GSS_S_FAILURE;

    };    




    contextual_lifetime = (OM_uint32)contextual_lifetime_signed;


#endif


#ifdef IMPORT_CONTEXTS_TO_BOTH_MODE_CREDS
/* We need to call cred_expand_id to set up the principal name in the	    */
/* cred...								    */

    if ((*minor_status = cred_expand_id(cred_ptr)) != gssdce_s_success) {
	tidy_cred(cred_ptr);
	destroy_cred(cred_ptr);
	return GSS_S_FAILURE;
    };

#endif

/* Add the lifetime_req parameter to 'now' and convert to utc to give the   */
/* expiration time of the credential.					    */
    delta_seconds_to_utc_time(lifetime_req,
			      &(cred_ptr->expiration_time));
    if (lifetime_rec != NULL) {
	/* Convert back again, just to make sure!			    */
	if (contextual_lifetime == 0) {
	    *lifetime_rec = 
		utc_time_to_delta_seconds(&(cred_ptr->expiration_time));
	} else {
	    *lifetime_rec = SMALLER(
		utc_time_to_delta_seconds(&(cred_ptr->expiration_time)),
		contextual_lifetime);
	};
    };
    if (actual_mechs != NULL) {
	nm_oid_set_create_empty(actual_mechs);
	if (cred_ptr->flags & CRED_C_FL_DCE1_MECH)
	    nm_oid_set_add_elem(actual_mechs, GSSDCE_C_OID_DCE_KRBV5_DES);
	if (cred_ptr->flags & CRED_C_FL_KRB5_MECH)
	    nm_oid_set_add_elem(actual_mechs, GSSDCE_C_OID_KRBV5_DES);
    };

    *output_cred_handle = cred_info_to_gss(cred_ptr);
    UNLOCK_ELEMENT(cred_ptr);
    *minor_status = gssdce_s_success;

    return GSS_S_COMPLETE;
}


/****************************************************************************/
/*									    */
/*	             gssdce_cred_to_login_context routine		    */
/*									    */
/****************************************************************************/

OM_uint32 gssdce_cred_to_login_context
                (OM_uint32 * minor_status,
                 gss_cred_id_t cred_handle,
                 sec_login_handle_t * output_login_context) {
    cred_info * c_p;
    error_status_t error_status;
    *minor_status = gssdce_s_operation_incomplete;

    GSS_INIT;


    if (cred_handle == GSS_C_NO_CREDENTIAL) {
/* This is a request to use the default credential.  The default credential */
/* is a "virtual" credential, using the default login-context, and	    */
/* supporting the DCE mech-type for initiate-only use.			    */
/* Extract the default login-context...					    */
	sec_login_get_current_context(output_login_context,
				      &error_status);
	if (error_status != error_status_ok) {
	    *minor_status = error_status;
	    return GSS_S_NO_CRED;
	};
    } else {
	if ((c_p = resolve_cred(cred_handle)) == NULL) {
	    *minor_status = gssdce_s_invalid_handle;
	    return GSS_S_DEFECTIVE_CREDENTIAL;
	};
/* Check to see whether this credential actually has a login-context.	    */
/* Accept-only credentials don't have login contexts...			    */
	if (!(c_p->flags & CRED_C_FL_INITIATE)) {
	    UNLOCK_ELEMENT(c_p);
	    *minor_status = gssdce_s_no_login_context;
	    return GSS_S_NO_CRED;
	};

	if ((*output_login_context = c_p->login_context) == NULL) {
/* Shouldn't happen, but if it does, return an error.			    */
	    UNLOCK_ELEMENT(c_p);
	    *minor_status = gssdce_s_no_login_context;
	    return GSS_S_FAILURE;
	};

	set_cred_unmanage_context(c_p); /* Since we're giving the	    */
					/* login-context to the app, we	    */
					/* don't want to delete it with the */
					/* cred...			    */

	UNLOCK_ELEMENT(c_p);
    };

    *minor_status = gssdce_s_success;
    return GSS_S_COMPLETE;
}



/****************************************************************************/
/*									    */
/*	                  gss_acquire_cred routine			    */
/*									    */
/****************************************************************************/

OM_uint32 gss_acquire_cred
                 (OM_uint32* minor_status,
                  gss_name_t desired_name,
                  OM_uint32 time_req,
                  gss_OID_set desired_mechs,
                  int cred_usage,
                  gss_cred_id_t* output_cred_handle,
                  gss_OID_set* actual_mechs,
                  OM_uint32 * time_rec) {
    cred_info * cred_ptr = NULL;
    error_status_t error_status;
    error_status_t xerr;
    OM_uint32 desired_mech_flags;
    unsigned32 used_kvno;
    unsigned32 err;
    boolean32 reset_passwd;
    sec_login_auth_src_t auth_src;
#ifndef DCE_V11
    sec_login_net_info_t net_info;
#else
    sec_cred_pa_handle_t cred_pa_handle;
    sec_id_pa_t * sec_id_pas;
#endif
    void * arg;

    *minor_status = gssdce_s_operation_incomplete;
    GSS_INIT;
    desired_mech_flags = 0;
    
    if (desired_mechs == NULL) {
	desired_mech_flags |= CRED_C_FL_DCE1_MECH;
/* Also allow Kerberos usage...						    */
	desired_mech_flags |= CRED_C_FL_KRB5_MECH;
    } else {
	if (nm_oid_set_test_member(GSSDCE_C_OID_DCE_KRBV5_DES,
				   desired_mechs))
	    desired_mech_flags |= CRED_C_FL_DCE1_MECH;
	if (nm_oid_set_test_member(GSSDCE_C_OID_KRBV5_DES_ID,
				   desired_mechs))
	    desired_mech_flags |= CRED_C_FL_KRB5_MECH;
	if (nm_oid_set_test_member(GSSDCE_C_OID_KRBV5_DES_RFC,
				   desired_mechs))
	    desired_mech_flags |= CRED_C_FL_KRB5_MECH;
    };

    if (cred_usage == GSS_C_INITIATE || cred_usage == GSS_C_BOTH)
	desired_mech_flags |= CRED_C_FL_INITIATE;
    if (cred_usage == GSS_C_ACCEPT || cred_usage == GSS_C_BOTH)
	desired_mech_flags |= CRED_C_FL_ACCEPT;
			 
    if (cred_usage == GSS_C_INITIATE) {
/* Initiate-only credential required.  This type of credential needs a	    */
/* pointer to the corresponding login context.  First, we have to locate    */
/* that context, unless the given name is NULL, in which case we use the    */
/* default login context.						    */
	if (desired_name == GSS_C_NO_NAME) {
	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                           "Calling login_context_to_cred in acquire_cred"));
	    return gssdce_login_context_to_cred
			(minor_status,
                         NULL,
			 time_req,
                         desired_mechs,
			 output_cred_handle,
			 actual_mechs,
			 time_rec);
	} else {
/* Track down the login context by name.  Can we do this?.		    */
	    dce_svc_printf(GSS_S_AC_NAMED_INIT_MSG);
	    UNIMPLEMENTED_RETURN;
	};
    } else if (cred_usage != GSS_C_ACCEPT && cred_usage != GSS_C_BOTH) {
	*minor_status = gssdce_s_bad_usage;
	dce_svc_printf(GSS_S_AC_BAD_USAGE_MSG);
	return GSS_S_FAILURE;
    } else if (desired_name == GSS_C_NO_NAME) {
/* Then we can find out the name from the default login context		    */
#ifndef DCE_V11
	sec_login_inquire_net_info(NULL,
				   &net_info,
				   &error_status);
	if (error_status == sec_login_s_no_current_context) {
	    sec_login_free_net_info(&net_info);
/*	    *minor_status = gssdce_s_no_login_context; */
	    *minor_status = error_status; 
	    dce_svc_printf(GSS_S_AC_NONAME_DEFAULT_MSG, error_status);
	    return GSS_S_NO_CRED;
	} else if (error_status == sec_login_s_not_certified) {
	    ;
	} else if (error_status != error_status_ok) {
	    dce_svc_printf(GSS_S_AC_INI_FAILED_MSG, error_status);
	    UNEXPECTED_RETURN;
	};
/* We now have the PAC for the desired identity, buried in net_info.pac.    */
/* We need the server's printable name to retrieve its key from the	    */
/* serv_id_table, so we have to parse the PAC.				    */

	if ((cred_ptr = create_cred(0, (sec_login_handle_t) 0)) == NULL) {
	    *minor_status = gssdce_s_out_of_memory;
	    dce_svc_printf(GSS_S_AC_CRED_NOMEM1_MSG);
	    return GSS_S_FAILURE;
	};

	cred_ptr->principal_uuid = net_info.pac.principal.uuid;
	cred_ptr->principal_realm_uuid = net_info.pac.realm.uuid;
	cred_ptr->principal_uuid_valid = 1;

	sec_login_free_net_info(&net_info);
#else

/* This is equivalent code to the above, only it uses DCE 1.1 se_cred EPAC  */
/* interrogation routines instead of parsing the PAC itself.  As well as    */
/* being cleaner, this also means that the login-context doesn't have to    */
/* have been built with V10 compatibility mode enabled.			    */

	cred_pa_handle = sec_login_cred_get_initiator((sec_login_handle_t) 0,
						      &error_status);

	if (error_status != error_status_ok &&
	    error_status != sec_login_s_not_certified) {

	    dce_svc_printf(GSS_S_LCC_CGI_FAILED_MSG, error_status);
	    *minor_status = error_status;
	    return GSS_S_FAILURE;
    
	};    

	sec_id_pas = sec_cred_get_pa_data(cred_pa_handle, &error_status);

	if (error_status != error_status_ok &&
	    error_status != sec_login_s_not_certified) {

	    dce_svc_printf(GSS_S_LCC_GPD_FAILED_MSG, error_status);
	    *minor_status = error_status;
	    return GSS_S_FAILURE;

	};    


	if ((cred_ptr = create_cred(0, (sec_login_handle_t) 0)) == NULL) {
	    dce_svc_printf(GSS_S_LCC_CRED_NOMEM_MSG);
	    *minor_status = gssdce_s_out_of_memory;
	    return GSS_S_FAILURE;
	};


	cred_ptr->principal_uuid = sec_id_pas->principal.uuid;
	cred_ptr->principal_realm_uuid = sec_id_pas->realm.uuid;
	cred_ptr->principal_uuid_valid = 1;

#endif


	if ((*minor_status = cred_expand_id(cred_ptr)) != gssdce_s_success) {
	    tidy_cred(cred_ptr);
	    destroy_cred(cred_ptr);
	    return GSS_S_FAILURE;
	};
    } else {
	if ((cred_ptr = create_cred(0, (sec_login_handle_t) 0)) == NULL) {
#	    if defined(DEBUG)
		fprintf(stderr,
			"Acquire_cred - Error - create_cred returned NULL\n");
		fflush(stderr);
#	    endif
	    dce_svc_printf(GSS_S_AC_CRED_NOMEM2_MSG);
	    *minor_status = gssdce_s_out_of_memory;
	    return GSS_S_FAILURE;
	};

	irep_to_string(desired_name,
		      cred_ptr->principal_name,
		      &xerr);

	if (xerr != error_status_ok) {
	    tidy_cred(cred_ptr);
	    destroy_cred(cred_ptr);
	    *minor_status = gssdce_s_bad_name;
	    return GSS_S_BAD_NAME;
	};
	cred_ptr->principal_name_valid = 1;

	/* we DO want the realm/local name fields filled in */
	if ((*minor_status = cred_expand_id(cred_ptr)) != gssdce_s_success)
	{
	    tidy_cred(cred_ptr);
	    destroy_cred(cred_ptr);
	    return GSS_S_FAILURE;
	};
    };

/* Find the appropriate key by server principal name in the server	    */
/* registration table.							    */

    lookup_reg_server_arg(cred_ptr->principal_name,
			  &arg,
			  &xerr);
    if (xerr != error_status_ok) {
/* The desired name hasn't been registered.  Register it using the default  */
/* key file and retrieval function.					    */
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug8, 
                       "Acquire_cred - Principal %s is not registered, registering...", cred_ptr->principal_name));
	arg = NULL;
	gss_server_register_auth_info(cred_ptr->principal_name,
				      rpc_c_authn_default,
				      NULL, /* default retrieval function */
				      arg, /* default key-file */
				      &err);
	if (err != rpc_s_ok) {
	    tidy_cred(cred_ptr);
	    destroy_cred(cred_ptr);
	    *minor_status = err;
/*	    *minor_status = gssdce_s_couldnt_register_server; */
	    dce_svc_printf(GSS_S_AC_REG_FAILED_MSG, err);
	    return GSS_S_FAILURE;
	};

    };
    cred_ptr->flags = desired_mech_flags;

    if (cred_usage == GSS_C_BOTH) {

/* We're going to create a login-context, therefore mark it for deletion    */
/* when the credential gets deleted...					    */

	set_cred_manage_context(cred_ptr);

/* Build and validate a login context from the key.			    */
	if (!sec_login_setup_identity(cred_ptr->principal_name,
				      sec_login_credentials_private,
				      &cred_ptr->login_context,
				      &error_status)) {
/*	    *minor_status = gssdce_s_unknown_principal_name; */
	    *minor_status = error_status;
	    tidy_cred(cred_ptr);
	    destroy_cred(cred_ptr);
	    return GSS_S_NO_CRED;
	};
	sec_login_valid_from_keytable(cred_ptr->login_context,
				      rpc_c_authn_dce_secret,
				      arg,
				      0,
				      &used_kvno,
				      &reset_passwd,
				      &auth_src,
				      &error_status);
	if (error_status != error_status_ok) {
/*	    *minor_status = gssdce_s_couldnt_validate_context; */
	    *minor_status = error_status;
	    return GSS_S_FAILURE;
	};
    };
/* Add the time_req parameter to 'now' and convert to utc to give the	    */
/* expiration time of the credential.					    */
    delta_seconds_to_utc_time(time_req,
			      &(cred_ptr->expiration_time));
    if (actual_mechs != NULL) {
	nm_oid_set_create_empty(actual_mechs);
	if (cred_ptr->flags & CRED_C_FL_DCE1_MECH)
	    nm_oid_set_add_elem(actual_mechs, GSSDCE_C_OID_DCE_KRBV5_DES);
	if (cred_ptr->flags & CRED_C_FL_KRB5_MECH)
	    nm_oid_set_add_elem(actual_mechs, GSSDCE_C_OID_KRBV5_DES);
    };
    if (time_rec != NULL) {
	*time_rec = utc_time_to_delta_seconds(&(cred_ptr->expiration_time));
    };
    *output_cred_handle = cred_info_to_gss(cred_ptr);
    UNLOCK_ELEMENT(cred_ptr);
    *minor_status = gssdce_s_success;
    return GSS_S_COMPLETE;	    
}

/****************************************************************************/
/*									    */
/*		     gss__wrap_token routine				    */
/*									    */
/****************************************************************************/

static OM_uint32 gss__wrap_token(gss_OID actual_mech_type,
				 unsigned char message_type[2],
				 sec_krb_message message,
				 gss_buffer_t output_token) {

    unsigned int ber_value_length;
    unsigned int ber_length_length;
    unsigned char * ch_ptr;

/* The wrapper for an initial context token follows the following ASN.1	    */
/* syntax:								    */
/*									    */
/* InitialContextToken ::=						    */
/*	[APLICATION 0] IMPLICIT SEQUENCE {				    */
/*			thisMech MechType,				    */
/*			innerContextToken ANY DEFINED BY thisMech	    */
/*	}								    */
/*									    */
/* where MechType ::= OBJECT IDENTIFIER					    */
/*									    */
/* The BER for this is:							    */
/* [APPLICATION 0] IMPLICIT SEQUENCE { thisMech MechType, ANY }		    */
/*      60                    ll          06  mm oo oo oo <inner-token>	    */
/*									    */
/* where "ll" = definite-encoded length of token, "mm" = length of	    */
/* thisMech OID, "oo oo oo" = octets of OID.				    */
/*									    */
/* First calculate total length of message value.  Note that we're assuming */
/* that the mechType OID's BER encoding is shorter than 128 bytes...	    */



    DCE_SVC_DEBUG((gss__svc_handle, gss_s_protocol, svc_c_debug9, 
                   "Wrapping a token, original length: %d bytes", message.length));
    if (actual_mech_type->length > 127) {
	dce_svc_printf(GSS_S_WRAP_MECH_LONG_MSG, actual_mech_type->length);
	return gssdce_s_ber_encoding_error;
    };
    
/* Work out how long our BER value is going to be.  We've got the mech-type */
/* OID (which is already the correct "V" of the BER TLV-encoding), but      */
/* we're going to have to add on the T & L (1-byte each).  We also need to  */
/* add the 2-byte message-type in front of the actual message data...       */

    ber_value_length =
	actual_mech_type->length + 4 + message.length;

/* Because the GSSAPI spec calls for the DER form of BER, we use the	    */
/* short-form length for ber_value_length less than 128 bytes, and	    */
/* long-form for longer data.						    */

    if (ber_value_length < 0x80) ber_length_length = 1;
    else if (ber_value_length < 0x100) ber_length_length = 2;
    else if (ber_value_length < 0x10000) ber_length_length = 3;
    else {
	dce_svc_printf(GSS_S_WRAP_BER_LONG_MSG, ber_value_length);
	return gssdce_s_ber_encoding_error;
    };

    output_token->length = ber_value_length + ber_length_length + 1;
    if ((output_token->value = (void *)malloc(output_token->length)
	) == NULL) {
	output_token->length = 0;
	dce_svc_printf(GSS_S_WRAP_NOMEM_MSG);
	return gssdce_s_out_of_memory;
    };
    
    ch_ptr = (unsigned char *)(output_token->value);
    *ch_ptr++ = 0x60;   /* [APPLICATION 0] IMPLICIT SEQUENCE { */
    if (ber_length_length == 1) {
	*ch_ptr++ = ber_value_length;  /* Short-form length of SEQUENCE */
    } else if (ber_length_length == 2) {
	*ch_ptr++ = 0x81;		   /* Long-form length of SEQUENCE, */
	*ch_ptr++ = ber_value_length;  /* 2 bytes used.		    */
    } else {
	*ch_ptr++ = 0x82;		   /* Long-form length of SEQUENCE, */
	*ch_ptr++ = (ber_value_length & 0xff00) >> 8; /* 3 bytes used.  */
	*ch_ptr++ = (ber_value_length & 0xff);
    };
    *ch_ptr++ = 0x06;		   /* OID tag			    */
    *ch_ptr++ = actual_mech_type->length;
				       /* OID length		    */

    memcpy(ch_ptr,			   /* Copy the mechType OID	    */
	   actual_mech_type->elements,
	   actual_mech_type->length);

    ch_ptr += actual_mech_type->length;

    *ch_ptr++ = message_type[0];
    *ch_ptr++ = message_type[1];

    memcpy(ch_ptr,			   /* Copy the Kerberos token	    */
	   message.data,
	   message.length);


    DCE_SVC_DEBUG((gss__svc_handle, gss_s_protocol, svc_c_debug9, 
                   "Wrapped token, new length: %d bytes", output_token->length));
    return gssdce_s_success;
}

/****************************************************************************/
/*									    */
/*		     gss__unwrap_token routine				    */
/*									    */
/****************************************************************************/

static OM_uint32 gss__unwrap_token(gss_buffer_t input_token_buffer,
				  gss_OID actual_mech_type,
				  unsigned char message_type[2],
				  sec_krb_message * message) {
    unsigned char * input_message;
    unsigned int byte_offset;
    size_t token_length;

/* Now to determine the mechanism used in the token we've been given.	    */
/* The wrapper for an initial context token follows the following ASN.1	    */
/* syntax:								    */
/*									    */
/* InitialContextToken ::=						    */
/*	[APLICATION 0] IMPLICIT SEQUENCE {				    */
/*			thisMech MechType,				    */
/*			innerContextToken ANY DEFINED BY thisMech	    */
/*	}								    */
/*									    */
/* where MechType ::= OBJECT IDENTIFIER					    */
/*									    */
/* The BER for this is:							    */
/* [APPLICATION 0] IMPLICIT SEQUENCE { thisMech MechType, ANY }		    */
/*      60                    ll          06  mm oo oo oo <inner-token>	    */
/*									    */
/* where "ll" = definite-encoded length of token, "mm" = length of	    */
/* thisMech OID, "oo oo oo" = octets of OID.				    */
/*									    */

    input_message = (unsigned char *)(input_token_buffer->value);
    byte_offset = 0;

    DCE_SVC_DEBUG((gss__svc_handle, gss_s_protocol, svc_c_debug9, 
                   "Unwrapping a token, original length: %d bytes", input_token_buffer->length));


    if (input_message[byte_offset++] != 0x60) {

	dce_svc_printf(GSS_S_UNWRAP_INV1_MSG);
	return gssdce_s_ber_decoding_error;
    };

    if ((input_message[byte_offset] & 0x80) == 0) {
/* Short-form, definite-length encoding					    */
	token_length = input_message[byte_offset++];
    } else if (input_message[byte_offset] == 0x81) {
/* Long-form, 2-byte encoding						    */
	byte_offset++;
	token_length = input_message[byte_offset++];
    } else if (input_message[byte_offset] == 0x82) {
	byte_offset++;
	token_length = input_message[byte_offset++] << 8;
	token_length += input_message[byte_offset++];
    } else {
	dce_svc_printf(GSS_S_UNWRAP_INV2_MSG);
	return gssdce_s_ber_decoding_error;
    };
    if (token_length > input_token_buffer->length - byte_offset) {
	dce_svc_printf(GSS_S_UNWRAP_INV3_MSG);
	return gssdce_s_ber_decoding_error;
    };
    if (input_message[byte_offset++] != 0x06) {
	dce_svc_printf(GSS_S_UNWRAP_INV4_MSG);
	return gssdce_s_ber_decoding_error;
    };
    actual_mech_type->length = input_message[byte_offset++];
    if (actual_mech_type->length > 127) {
	actual_mech_type->length = 0;
	dce_svc_printf(GSS_S_UNWRAP_INV5_MSG);
	return gssdce_s_ber_decoding_error;
    };
    actual_mech_type->elements =
	(unsigned char *)malloc(actual_mech_type->length);
    if (actual_mech_type->elements == NULL) {
	actual_mech_type->length = 0;
	dce_svc_printf(GSS_S_UNWRAP_NOMEM_MSG);
	return gssdce_s_out_of_memory;
    };

    memcpy(actual_mech_type->elements,
	   &input_message[byte_offset],
	   actual_mech_type->length);
    byte_offset += actual_mech_type->length;

    message_type[0] = input_message[byte_offset++];
    message_type[1] = input_message[byte_offset++];


    message->data = &input_message[byte_offset];
    message->length = input_token_buffer->length - byte_offset;

    DCE_SVC_DEBUG((gss__svc_handle, gss_s_protocol, svc_c_debug9, 
                   "Unwrapped token, res length: %d bytes", message->length));
    return gssdce_s_success;

}

/****************************************************************************/
/*									    */
/*			gss_generate_seq routine			    */
/*									    */
/****************************************************************************/
static void gss_generate_seq(SEQUENCE_NO * seq);

static void gss_generate_seq(SEQUENCE_NO * seq) {
/* The code below assumes that sizeof(SEQUENCE_NO) is no more than 8 bytes  */

    unsigned char rand_block[8];
    gss_generate_random_block(rand_block);
    memcpy(seq, rand_block, sizeof(SEQUENCE_NO));
}

/****************************************************************************/
/*									    */
/*		     gss_cred_get_delegation_type routine		    */
/*									    */
/****************************************************************************/

static sec_id_delegation_type_t gss_cred_get_delegation_type(
				    rpc_authz_cred_handle_t creds,
				    error_status_t * error_status) {

#if 1
    return sec__cred_deleg_type_permitted(&creds, error_status);
}
#else							

    sec_cred_pa_handle_t initiator_princ_cred;
    sec_cred_pa_handle_t cur_princ_cred;
    sec_cred_pa_handle_t next_princ_cred;
    sec_cred_cursor_t cursor;
    sec_id_delegation_type_t deleg_type;
    error_status_t tmp_stat;


	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                       "Entering gss_cred_get_delegation_type, calling sec_cred_get_initiator"));

    initiator_princ_cred = sec_cred_get_initiator(creds, error_status);

    if (*error_status) {
	dce_svc_printf(GSS_S_GDT_SCGI_FAILED_MSG, *error_status);
	return sec_id_deleg_type_none;
    };



    next_princ_cred = initiator_princ_cred;

	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                       "Initializing cred-cursor for scan..."));

    sec_cred_initialize_cursor(&cursor, error_status);
    if (*error_status) {
	dce_svc_printf(GSS_S_GDT_SCIC_FAILED_MSG, *error_status);
    } else {

	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                       "Entering cred-scan loop..."));

	while (*error_status == 0) {
	    cur_princ_cred = next_princ_cred;
	    next_princ_cred = sec_cred_get_delegate(creds,
						    &cursor,
						    error_status);
	    if (*error_status) {
		DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                               "sec_cred_get_delegate failed.  Exiting loop"));
	    } else {
		DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                               "sec_cred_get_delegate succeeded.  Got a delegate. Continuing..."));
	    };
	};
	sec_cred_free_cursor(&cursor, &tmp_stat);
    };    
/* At this point, cur_princ_cred should describe whoever is immediately	    */
/* before us in the delegation chain (either the initiator or the nearest   */
/* delegate).  Call sec_cred_get_delegation_type on it...		    */


    if (initiator_princ_cred == cur_princ_cred) {
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                       "Invoking sec_cred_get_delegation_type on initiator cred"));

    } else {
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
                       "Invoking sec_cred_get_delegation_type on delegate cred"));
    };

    deleg_type = sec_cred_get_delegation_type(cur_princ_cred,
					      error_status);

    if (*error_status) {
	dce_svc_printf(GSS_S_GDT_SCGDT_FAILED_MSG, *error_status);
    };

}
#endif

/****************************************************************************/
/*									    */
/*		     gss_login_get_delegation_type routine		    */
/*									    */
/****************************************************************************/

static void gss_login_get_delegation_type(sec_login_handle_t login_context,
					  sec_id_delegation_type_t * deleg_type,
					  error_status_t *error_status) {

    sec_cred_pa_handle_t cur_princ_cred;
    sec_cred_pa_handle_t next_princ_cred;
    sec_cred_cursor_t cursor;
    error_status_t tmp_stat;

    cur_princ_cred = sec_login_cred_get_initiator(login_context, error_status);
    if (*error_status) {
	dce_svc_printf(GSS_S_GDT_SLGI_FAILED_MSG, *error_status);
	*deleg_type = sec_id_deleg_type_none;
	return;
    };
    next_princ_cred = cur_princ_cred;

    sec_login_cred_init_cursor(&cursor, error_status);
    if (*error_status) {
	dce_svc_printf(GSS_S_GDT_SLIC_FAILED_MSG, *error_status);
    } else {
	while (*error_status == 0) {
	    cur_princ_cred = next_princ_cred;
	    next_princ_cred = sec_login_cred_get_delegate(login_context,
							  &cursor,
							  error_status);
	};
	sec_cred_free_cursor(&cursor, &tmp_stat);
    };    
/* At this point, cur_princ_cred should describe whoever is immediately	    */
/* before us in the delegation chain (either the initiator or the nearest   */
/* delegate).  Call sec_cred_get_delegation_type on it...		    */

    *deleg_type = sec_cred_get_delegation_type(cur_princ_cred,
					       error_status);
    return;
}

/****************************************************************************/
/*									    */
/*	             gss_init_sec_context routine			    */
/*									    */
/****************************************************************************/

OM_uint32 gss_init_sec_context
                 (OM_uint32* minor_status,
                  gss_cred_id_t claimant_cred_handle,
                  gss_ctx_id_t* context_handle,
                  gss_name_t target_name,
                  gss_OID mech_type,
                  int req_flags,
                  OM_uint32 time_req,
                  gss_channel_bindings_t input_chan_bindings,
                  gss_buffer_t input_token,
                  gss_OID* actual_mech_type,
                  gss_buffer_t output_token,
                  int* ret_flags,
                  OM_uint32* time_rec) {
    cred_info * cred_ptr = NULL;
    ctx_info * ctx_ptr = NULL;
    sec_login_handle_t login_context;
    sec_login_handle_t temp_login_context = (sec_login_handle_t) 0;
    OM_uint32 O_status;
    sec_rgy_name_t server_char_name;
    error_status_t error_status;
    sec_krb_ccache ccache = NULL;
    sec_krb_cred kerberos_credential = NULL;
    unsigned32 kerberos_cred_expiration;
    sec_krb_parsed_name server_name = NULL;
    sec_krb_message snd_message;
    sec_krb_message rcv_message;
    gss_buffer_desc chan_binding_hash;
    unsigned32 authenticator_format; 
    sec_id_delegation_type_t deleg_type;
    gss_OID_desc message_mech_type;
    
    int cred_created;
    
    unsigned char binding_hash_data[16];
    unsigned char token_type[2];

    snd_message.length = 0;
    snd_message.data = NULL;
    cred_created = 0;
        
    *minor_status = gssdce_s_operation_incomplete;
    GSS_INIT;
    O_status = GSS_S_FAILURE;

    if (actual_mech_type) *actual_mech_type = NULL;
    if (output_token) {
	output_token->length = 0;
	output_token->value = NULL;
    };
    if (ret_flags) *ret_flags = 0;
    if (time_rec) *time_rec = 0;



    DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, 
    "Entering gss_init_sec_ctx"));

    if (context_handle == NULL) {
	*minor_status = gssdce_s_invalid_handle;
	O_status = GSS_S_NO_CONTEXT;

	dce_svc_printf(GSS_S_IC_NULL_CONTEXT_MSG);
	goto init_sec_ctx_return;
    };
    if (*context_handle != GSS_C_NO_CONTEXT) {
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, 
        "Context handle: %d", *context_handle));
	if ((ctx_ptr = resolve_ctx(*context_handle)) == NULL) {
	    *minor_status = gssdce_s_invalid_handle;
	    O_status = GSS_S_NO_CONTEXT;
	    dce_svc_printf(GSS_S_IC_INV_CONTEXT_MSG);
	    goto init_sec_ctx_return;
	};
    };
/* Here we've been given a context.  If it's a CLOSED context, we can	    */
/* re-use it.  If it's OPENING, we're being called for second-round	    */
/* processing.  If it's in another state, error.			    */
    if (ctx_ptr == NULL || ctx_ptr->state == CTX_C_ST_CLOSED) {
	if (ctx_ptr != NULL) {
/* Then we're re-using a closed context.  Make sure it doesn't have any	    */
/* trailing pointers hanging around.					    */
	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug8, 
            "Re-using old context-handle"));
	    tidy_ctx(ctx_ptr);
	};

/* Fix up the req_flags so it reflects what we can do...		    */

#ifndef GSS_DELEGATION
	req_flags &= ~GSS_C_DELEG_FLAG;  /* Can't do delegation yet */
#endif
	req_flags |= GSS_C_MUTUAL_FLAG;  /* Always do mutual authentication */
	req_flags |= GSS_C_REPLAY_FLAG;   /* Always do replay detection */
	req_flags |= GSS_C_SEQUENCE_FLAG;   /* Always do sequence detection */
	req_flags |= GSS_C_INTEG_FLAG;   /* Always can do integrity */
#ifdef NOENCRYPTION
	req_flags &= ~GSS_C_CONF_FLAG;	 /* Can't do encryption */
#else
	req_flags |= GSS_C_CONF_FLAG;	 /* Can do encryption */
#endif    

	if (claimant_cred_handle == GSS_C_NO_CREDENTIAL) {
/* This is a request to use the default credential.  The default credential */
/* is a "virtual" credential, using the default login-context, and	    */
/* supporting both DCE and KRB mech-types for initiate-only use.	    */
/* Extract the default login-context...					    */
	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug1, 
            "Constructing default cred in gss_init_sec_ctx"));
	    sec_login_get_current_context(&login_context,
					  &error_status);
	    if (error_status != error_status_ok) {
/*		*minor_status = gssdce_s_no_login_context; */
		*minor_status = error_status; 
		O_status = GSS_S_NO_CRED;
		dce_svc_printf(GSS_S_IC_NO_DEF_CTX_MSG, error_status);
		goto init_sec_ctx_return;
	    };
	    if ((cred_ptr = create_cred(CRED_C_FL_INITIATE
					| CRED_C_FL_DCE1_MECH
					| CRED_C_FL_KRB5_MECH,
					login_context)) == NULL) {
		login_context = (sec_login_handle_t) 0;
		*minor_status = gssdce_s_out_of_memory;
		O_status = GSS_S_FAILURE;
		dce_svc_printf(GSS_S_IC_CRED_NOMEM_MSG);
		goto init_sec_ctx_return;
	    };
	    login_context = (sec_login_handle_t) 0;
	    cred_created = 1;
	} else {
	    if ((cred_ptr = resolve_cred(claimant_cred_handle)) == NULL) {
		*minor_status = gssdce_s_invalid_handle;
		O_status = GSS_S_DEFECTIVE_CREDENTIAL;
		dce_svc_printf(GSS_S_IC_INV_CRED_MSG);
		goto init_sec_ctx_return;
	    };
	    if (!(cred_ptr->flags & CRED_C_FL_INITIATE)) {
		*minor_status = gssdce_s_bad_usage;
		O_status = GSS_S_NO_CRED;
		dce_svc_printf(GSS_S_IC_CRED_NOT_INIT_MSG);
		goto init_sec_ctx_return;
	    };
	};
/* Now we have a valid credential from which to build the context.	    */
/* Allocate a fresh context if necessary.				    */
	if (ctx_ptr == NULL) {
	    /* We weren't given an old context to re-use.  Create a fresh   */
	    /* one.							    */
	    if ((ctx_ptr = create_ctx()) == NULL) {
		*minor_status = gssdce_s_out_of_memory;
		O_status = GSS_S_FAILURE;
		dce_svc_printf(GSS_S_IC_CTX_NOMEM_MSG);
		goto init_sec_ctx_return;
	    };
	    *context_handle = ctx_info_to_gss(ctx_ptr);
	} else {
	    /* Make sure that if we're re-using an old context that it's    */
	    /* completely clean.					    */
	    memset(ctx_ptr, 0, sizeof(ctx_info));
	};
	ctx_ptr->state = CTX_C_ST_CLOSED;   

/* If we created the credential, note this in the context, so that we can   */
/* delete the credential when the context goes away.			    */
	if (cred_created) ctx_ptr->one_time_cred = 1;

/* First determine what protocol to use...				    */
	if (mech_type == GSS_C_NULL_OID) {
/* We are free to pick the mechanism.  Use DCE if credential is suitable,   */
/* otherwise use Kerberos.						    */
	    if (cred_ptr->flags & CRED_C_FL_DCE1_MECH) {
		ctx_ptr->authn_protocol = rpc_c_authn_dce_secret;
		ctx_ptr->authz_protocol = rpc_c_authz_dce;	    
/*		nm_oid_copy(GSSDCE_C_OID_DCE_KRBV5_DES,			    */
/*			    &ctx_ptr->actual_mech_type);		    */
		ctx_ptr->actual_mech_type = GSSDCE_C_OID_DCE_KRBV5_DES;
		ctx_ptr->flags = CTX_C_FL_DCE1_MECH;
	    } else if (cred_ptr->flags & CRED_C_FL_KRB5_MECH) {
		ctx_ptr->authn_protocol = rpc_c_authn_dce_secret;
		ctx_ptr->authz_protocol = rpc_c_authz_name;	    
/*		nm_oid_copy(GSSDCE_C_OID_KRBV5_DES,			    */
/*			    &ctx_ptr->actual_mech_type);		    */
		ctx_ptr->actual_mech_type = GSSDCE_C_OID_KRBV5_DES;
		ctx_ptr->flags = CTX_C_FL_KRB5_MECH;
		req_flags &= ~GSS_C_DELEG_FLAG;
		    /* Can't do delegation yet with MIT Kerberos */
	    } else {
		*minor_status = gssdce_s_unsupported_mechanism;
		O_status = GSS_S_DEFECTIVE_CREDENTIAL;

		dce_svc_printf(GSS_S_IC_INV_CRED_MECH_MSG);
		goto init_sec_ctx_return;		
	    };
	} else {
	    if (nm_oid_equal(mech_type, GSSDCE_C_OID_DCE_KRBV5_DES)) {
/* Use DCE...								    */
		if (!(cred_ptr->flags & CRED_C_FL_DCE1_MECH)) {
		    *minor_status = gssdce_s_unsupported_mechanism;
		    O_status = GSS_S_NO_CRED;
		    dce_svc_printf(GSS_S_IC_CRED_NODCE_MSG);
		    goto init_sec_ctx_return;		
		};
		ctx_ptr->authn_protocol = rpc_c_authn_dce_secret;
		ctx_ptr->authz_protocol = rpc_c_authz_dce;	    
/*		nm_oid_copy(GSSDCE_C_OID_DCE_KRBV5_DES,			    */
/*			    &ctx_ptr->actual_mech_type);		    */
		ctx_ptr->actual_mech_type = GSSDCE_C_OID_DCE_KRBV5_DES;
		ctx_ptr->flags = CTX_C_FL_DCE1_MECH;
	    } else if (nm_oid_equal(mech_type, GSSDCE_C_OID_KRBV5_DES_ID)
		    || nm_oid_equal(mech_type, GSSDCE_C_OID_KRBV5_DES_RFC)) {
		if (!(cred_ptr->flags & CRED_C_FL_KRB5_MECH)) {
		    *minor_status = gssdce_s_unsupported_mechanism;
		    O_status = GSS_S_NO_CRED;
		    dce_svc_printf(GSS_S_IC_CRED_NOKRB_MSG);

		    goto init_sec_ctx_return;		
		};
		ctx_ptr->authn_protocol = rpc_c_authn_dce_secret;
		ctx_ptr->authz_protocol = rpc_c_authz_name;	    
/*		nm_oid_copy(GSSDCE_C_OID_KRBV5_DES,			    */
/*			    &ctx_ptr->actual_mech_type);		    */
		ctx_ptr->actual_mech_type = GSSDCE_C_OID_KRBV5_DES;
	    } else {
		*minor_status = gssdce_s_unsupported_mechanism;
		O_status = GSS_S_BAD_MECH;
		dce_svc_printf(GSS_S_IC_INV_REQ_MECH_MSG);
		goto init_sec_ctx_return;
	    };
	};
	ctx_ptr->local_cred = cred_info_to_gss(cred_ptr);
	ctx_ptr->deleg_cred = GSS_C_NO_CREDENTIAL;
    
/* Now we have an initialized context data structure containing all the	    */
/* elements we need to initiate a context.  Make it so...		    */

/* First step is to handle delegation (if we need to).  First check to see  */
/* whether we can use the credential's login-context directly, oer whether  */
/* we have to munge it first...						    */

	login_context = cred_ptr->login_context;

#ifdef GSS_DELEGATION
	if (!(req_flags & GSS_C_DELEG_FLAG)) {
/* We've been asked not to delegate...					    */
	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug8, 
            "No delegation requested"));
	    gss_login_get_delegation_type(login_context,
					  &deleg_type,
					  &error_status);
	    if (error_status) {
		O_status = GSS_S_FAILURE;
		*minor_status = error_status;
		dce_svc_printf(GSS_S_IC_DELEG_TYPE_UNK_MSG, error_status);
		goto init_sec_ctx_return;
	    };
	    if (deleg_type != sec_id_deleg_type_none) {
/* We don't want to delegate, but this login context will delegate if we    */
/* use it.  So we have to create a temporary context that will behave	    */
/* itself, and use that instead.					    */
		DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug8, 
                "Disabling delegation in login-context"));
		temp_login_context = 
			sec_login_disable_delegation(login_context,
						     &error_status);
		if (error_status) {
		    O_status = GSS_S_FAILURE;
		    *minor_status = error_status;
		    dce_svc_printf(GSS_S_IC_CANT_DISABLE_DELEG_MSG, error_status);
		    goto init_sec_ctx_return;
		};
		login_context = temp_login_context;
	    };
	} else {
/* We've been asked to delegate...					    */
	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug8, 
            "Delegation requested..."));
	    gss_login_get_delegation_type(login_context,
					  &deleg_type,
					  &error_status);
	    if (error_status) {
		O_status = GSS_S_FAILURE;
		*minor_status = error_status;
		dce_svc_printf(GSS_S_IC_DELEG_TYPE_UNK_MSG, error_status);
		goto init_sec_ctx_return;
	    };
	    if (deleg_type == sec_id_deleg_type_none) {
		DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug8, 
                "Enabling impersonation in login-context"));
		temp_login_context =
		    sec_login_become_initiator(login_context,
					       sec_id_deleg_type_impersonation,
					       NULL,
					       NULL,
					       NULL,
					       NULL,
					       sec_id_compat_mode_initiator,
					       &error_status);
		if (error_status) {
		    O_status = GSS_S_FAILURE;
		    *minor_status = error_status;
		    dce_svc_printf(GSS_S_IC_CANT_DELEGATE_MSG, error_status);
		    goto init_sec_ctx_return;
		};
		login_context = temp_login_context;
	    };
	};
#endif


/* First we need to get an authentication-mechanism credential from our	    */
/* login_context.  This really just creates a credential-cache entry, or    */
/* finds the pre-existing entry for this principal.			    */

	if ((error_status = sec_krb_get_cc(login_context,
					   &ccache)) != error_status_ok) {
/*	    *minor_status = gssdce_s_couldnt_get_mech_cred; */
	    *minor_status = error_status;
	    O_status = GSS_S_NO_CRED;
	    dce_svc_printf(GSS_S_IC_NO_KRB_CRED_CACHE_MSG);
	    goto init_sec_ctx_return;
	};

	DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug8, 
        "Got Kerberos credential cache..."));
/* Now we have our credential cache entry.  Use it to obtain a ticket for   */
/* the server.								    */

/* First we have to get a kerberos-style parsed name for the server.	    */

	irep_to_string(target_name,
		      server_char_name,
		      &error_status);

	if (error_status != error_status_ok) {
	    *minor_status = gssdce_s_couldnt_translate_name;
	    O_status = GSS_S_BAD_NAME;
		dce_svc_printf(GSS_S_IC_BAD_NAME_MSG, error_status);
	    goto init_sec_ctx_return;
	};
	
	if ((error_status = sec_krb_sec_parse_name(
		    login_context,
		    rpc_c_authn_dce_secret,
		    server_char_name,
		    &server_name)) != error_status_ok) {
/*	    *minor_status = gssdce_s_couldnt_translate_name; */
	    *minor_status = error_status;
	    O_status = GSS_S_BAD_NAME;
	    dce_svc_printf(GSS_S_IC_BAD_NAME1_MSG, error_status);
	    goto init_sec_ctx_return;
	};
     
	if ((error_status = sec_krb_get_cred(ccache,
					     server_name,
					     ctx_ptr->authn_protocol,
					     ctx_ptr->authz_protocol,
                                             0, NULL,
					     &kerberos_credential,
					     &kerberos_cred_expiration)
	    ) != error_status_ok) {
/*	    *minor_status = gssdce_s_couldnt_get_server_ticket; */
	    *minor_status = error_status;
	    O_status = GSS_S_NO_CRED;
	    dce_svc_printf(GSS_S_IC_NO_SERVER_TICKET_MSG, error_status);
	    goto init_sec_ctx_return;
	};

    DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, 
    "Got server ticket..."));
/* Now we've got either a ticket or a privileged ticket (depending on	    */
/* ctx_ptr->authz_protocol).  Use it to create a Kerberos KRB_AP_REQ	    */
/* message...								    */

/* First convert the channel bindings into hash				    */

	hash_channel_bindings(input_chan_bindings,
			      binding_hash_data);


	chan_binding_hash.value = (void *)&binding_hash_data[0];
	chan_binding_hash.length = sizeof(binding_hash_data);

/* Have to do something here if delegation was requested.		    */
	if (req_flags & GSS_C_DELEG_FLAG) {
	    ctx_ptr->delegated = 1;
	};

	ctx_ptr->context_flags = req_flags;
	
/* Finally, get Kerberos to authenticate for us.			    */


/* First, generate a send sequence number...				    */

	gss_generate_seq(&ctx_ptr->snd_seq);
	
	authenticator_format = 
	    ((ctx_ptr->authz_protocol == rpc_c_authz_name) ? KRB_FORMAT 
							   : DCE_FORMAT);
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, 
        "Calling sec_krb_gss_build_message..."));

	if ((error_status = sec_krb_gss_build_message(ccache,
						     kerberos_credential,
						     authenticator_format,
						     rpc_c_authn_level_default,
						     ctx_ptr->authz_protocol,
						     ctx_ptr->snd_seq,
						     &chan_binding_hash,
						     ctx_ptr->context_flags,
						     &ctx_ptr->auth_session_key,
						     &ctx_ptr->ctime,
						     &ctx_ptr->cusec,
						     &snd_message)


	    ) != error_status_ok) {
						
/*	    *minor_status = gssdce_s_couldnt_build_authenticator; */
	    *minor_status = error_status;
	    O_status = GSS_S_FAILURE;
	    dce_svc_printf(GSS_S_IC_SKBM_FAILED_MSG, error_status);
	    goto init_sec_ctx_return;
	};

/* Now we have a raw KRB_AP_REQ message in 'snd_message'.  Wrap it and	    */
/* return to caller.							    */


	DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug5, 
        "gss_init_sec_context: Wrapping token (Token-type INIT)"));

	*minor_status = gss__wrap_token(ctx_ptr->actual_mech_type,
					TOKEN_TYPE_INIT,
				        snd_message,
				        output_token);

	DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug5, 
        "Wrapped token, length: %d", output_token->length));

	if (*minor_status != gssdce_s_success) {
	    O_status = GSS_S_FAILURE;
	    goto init_sec_ctx_return;
	};

	O_status = GSS_S_COMPLETE | GSS_S_CONTINUE_NEEDED;

	ctx_ptr->state = CTX_C_ST_INITIATING;   

/* That's it for the initial message.					    */

    } else if (ctx_ptr->state == CTX_C_ST_INITIATING)
    {
/* Stage-two processing of context...					    */
/* Check we've been sent a reply token.  This should be either a KRB_AP_REP */
/* or a KRB_ERROR.							    */

	DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug1, 
        "Entered gss_init_sec_ctx with mutual response token"));
	if ((input_token == NULL)
	    || (input_token->length == 0)
	    || (input_token->value == NULL))
	{
	    *minor_status = gssdce_s_param_required;
	    O_status = GSS_S_DEFECTIVE_TOKEN;
	    dce_svc_printf(GSS_S_IC_NO_TOKEN_MSG);
	    goto init_sec_ctx_return;
	};

/* Unwrap the token...							    */

	*minor_status = gss__unwrap_token(input_token,
					 &message_mech_type,
					 token_type,
					 &rcv_message);

	if (*minor_status != gssdce_s_success) {
	    dce_svc_printf(GSS_S_IC_INV_WRAPPER_MSG);
	    O_status = GSS_S_DEFECTIVE_TOKEN;
	    goto init_sec_ctx_return;
	};

	if (nm_oid_equal(ctx_ptr->actual_mech_type,
			 &message_mech_type)) {
	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, 
            "Processing response token for correct mechanism"));
	    nm_oid_free(&message_mech_type, 0);
	} else {
	    dce_svc_printf(GSS_S_IC_MUTUAL_BAD_MECH_MSG);
	    O_status = GSS_S_DEFECTIVE_TOKEN;
	    *minor_status = gssdce_s_unsupported_mechanism;
	    nm_oid_free(&message_mech_type, 0);
	    goto init_sec_ctx_return;
	};

	if (((token_type[0] != TOKEN_TYPE_MUTUAL[0]) ||
	     (token_type[1] != TOKEN_TYPE_MUTUAL[1])) &&
	    ((token_type[0] != TOKEN_TYPE_ERROR[0]) ||
	     (token_type[1] != TOKEN_TYPE_ERROR[1]))) {

	    dce_svc_printf(GSS_S_IC_BAD_MUTUAL_TYPE_MSG,
			   token_type[0],
			   token_type[1]);
	    O_status = GSS_S_DEFECTIVE_TOKEN;
	    *minor_status = gssdce_s_unexpected_token_type;
	    ctx_ptr->actual_mech_type = NULL;
	    goto init_sec_ctx_return;
	};

	if ((token_type[0] == TOKEN_TYPE_ERROR[0]) &&
	    (token_type[1] == TOKEN_TYPE_ERROR[1])) {

	DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug8, 
        "Processing error token in gss_init_sec_ctx"));
/* For now... */

	    O_status = GSS_S_DEFECTIVE_TOKEN;
	    *minor_status = gssdce_s_unexpected_token_type;
	    ctx_ptr->actual_mech_type = NULL;
	    goto init_sec_ctx_return;

	};

	DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug5, 
        "Unwrapped mutual response token (Token-type MUTUAL)"));

/* Here we know we've got a mutual-repsonse token...			    */

	DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, 
        "Calling sec_krb_gss_decode_rep_message"));

	error_status = sec_krb_gss_decode_rep_message(rpc_c_authn_level_default,
						      rcv_message,
						      ctx_ptr->auth_session_key,
						      ctx_ptr->ctime,
						      ctx_ptr->cusec,
						      &ctx_ptr->rcv_seq);
	rcv_message.length = 0;
	rcv_message.data = NULL; /*	Don't want to free the user's token */
				 /*	by mistake!			    */

	if (error_status == error_status_ok) {

	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, 
            "Context acceptor's sequence number: %ld", (long)(ctx_ptr->rcv_seq)));


	    if (actual_mech_type) {
		*actual_mech_type = ctx_ptr->actual_mech_type;
	    };


	    ctx_ptr->state = CTX_C_ST_OPEN_I;

	    if (ret_flags) {
		*ret_flags = ctx_ptr->context_flags;
	    };

	    if (time_rec) {
		*time_rec = 0xffffffffu;
	    };
	    *minor_status = gssdce_s_success;
	    O_status = GSS_S_COMPLETE;
	    goto init_sec_ctx_return;
	} else {
	    *minor_status = error_status;
/*	    *minor_status = gssdce_s_couldnt_decode_response; */
	    O_status = GSS_S_DEFECTIVE_TOKEN;
	    dce_svc_printf(GSS_S_IC_MUTUAL_DEC_FAILED_MSG, error_status);
	    goto init_sec_ctx_return;
	};
    } else {
	*minor_status = gssdce_s_invalid_context_state;
	O_status = GSS_S_FAILURE;
	goto init_sec_ctx_return;
    };


init_sec_ctx_return:

    DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, 
    "Common return path from gss_init_sec_ctx"));

    if (snd_message.length != 0) {
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, 
        "Freeing message..."));
	sec_krb_message_free(&snd_message);
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, 
        "Freed"));
    };
    if (server_name != NULL) {
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, 
        "Freeing server name..."));
	sec_krb_parsed_name_free(&server_name);
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, 
        "Freed"));
    };
    if (ccache != NULL) {
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, 
        "Freeing Kerberos credential cache..."));
	sec_krb_ccache_free(&ccache);
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, 
        "Freed"));
    };

    if (temp_login_context != (sec_login_handle_t) 0) {

	DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, 
        "Freeing temporary login-context..."));

	sec_login_purge_context(&temp_login_context, &error_status);

	if (error_status) {
	    dce_svc_printf(GSS_S_IC_ERROR_PURGING_MSG, error_status);
	} else {
	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, 
            "Freed"));
	};
    };


    if (cred_ptr != NULL) {
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_locks, svc_c_debug9, 
        "Unlocking credential..."));
	UNLOCK_ELEMENT(cred_ptr);
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_locks, svc_c_debug9, 
        "Unlocked"));
    };
    if (ctx_ptr != NULL) {
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_locks, svc_c_debug9, 
        "Unlocking context..."));
	UNLOCK_ELEMENT(ctx_ptr);
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_locks, svc_c_debug9, 
        "Unlocked"));
    };

	DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, 
        "Returning to caller..."));
    return O_status;

}

/****************************************************************************/
/*									    */
/*	             gss_build_error_token routine			    */
/*									    */
/****************************************************************************/

static void gss_build_error_token(OM_uint32 minor_status, /* input!	    */
				  ctx_info * ctx_ptr,
				  gss_buffer_t output_token) {
/* Construct a KRB_ERROR message to return from a failed		    */
/* gss_accept_sec_context.						    */
    sec_krb_message out_packet;
    
    sec_krb_cn_build_err_message(rpc_c_authn_level_default,
				 minor_status,
				 0, /* Should be ctime			    */
				 0, /* Should be cusec			    */
				 &out_packet,
				 NULL);

    output_token->value = out_packet.data;
    output_token->length = out_packet.length;

}

/****************************************************************************/
/*									    */
/*	             gss_accept_sec_context routine			    */
/*									    */
/****************************************************************************/

OM_uint32 gss_accept_sec_context
                 (OM_uint32* minor_status,
                  gss_ctx_id_t* context_handle,
                  gss_cred_id_t verifier_cred_handle,
                  gss_buffer_t input_token_buffer,
                  gss_channel_bindings_t input_chan_bindings,
                  gss_name_t* src_name,
                  gss_OID* mech_type,
                  gss_buffer_t output_token,
                  int* ret_flags,
                  OM_uint32* time_rec,
                  gss_cred_id_t* delegated_cred_handle) {
    cred_info * cred_ptr = NULL;
    ctx_info * ctx_ptr = NULL;
    sec_login_handle_t login_context = (sec_login_handle_t) 0;
    sec_login_handle_t my_login_context = (sec_login_handle_t) 0;
    OM_uint32 O_status;
    sec_krb_message rcv_message;
    sec_krb_message snd_message;
    gss_buffer_desc chan_binding_hash;
    error_status_t error_status;
    sec_id_delegation_type_t deleg_type;
    unsigned char * actual_server_name;
    
    sec_rgy_name_t temp_name;
    unsigned char binding_hash_data[16];
    unsigned char token_type[2];

    snd_message.length = 0;
    snd_message.data = NULL;

    GSS_INIT;


    DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, 
    "Entering gss_accept_sec_context"));

    *minor_status = gssdce_s_operation_incomplete;
    O_status = GSS_S_FAILURE;

    if (src_name) *src_name = NULL;
    if (mech_type) *mech_type = NULL;

    if (output_token) {
	output_token->length = 0;
	output_token->value = NULL;
    };

    if (ret_flags) *ret_flags = 0;
    if (time_rec) *time_rec = 0;
    if (delegated_cred_handle) *delegated_cred_handle = 0;


    if (context_handle == NULL) {
	*minor_status = gssdce_s_invalid_handle;
	O_status = GSS_S_NO_CONTEXT;
	dce_svc_printf(GSS_S_AC_NULL_CTX_MSG);
	goto accept_sec_ctx_return;
    };
    if (*context_handle != GSS_C_NO_CONTEXT) {
	if ((ctx_ptr = resolve_ctx(*context_handle)) == NULL) {
	    *minor_status = gssdce_s_invalid_handle;
	    O_status = GSS_S_NO_CONTEXT;
	    dce_svc_printf(GSS_S_AC_BAD_CTX_MSG);
	    goto accept_sec_ctx_return;
	};
    };
/* Here we've been given a context.  If it's a CLOSED context, we can	    */
/* re-use it.  Any other state is an error.				    */
    if (ctx_ptr != NULL && ctx_ptr->state != CTX_C_ST_CLOSED) {
	*minor_status= gssdce_s_invalid_context_state;
	O_status = GSS_S_FAILURE;
	dce_svc_printf(GSS_S_AC_BAD_CTX_STATE_MSG);
	goto accept_sec_ctx_return;
    } else {
	if (ctx_ptr != NULL) {
/* Then we're re-using a closed context.  Make sure it doesn't have any	    */
/* trailing pointers hanging around.					    */
	    if (ctx_ptr->actual_mech_type != NULL) {
		tidy_ctx(ctx_ptr);
	    };
	};

	if (verifier_cred_handle == GSS_C_NO_CREDENTIAL) {
/* This is a request to use the default credential.  For		    */
/* accept_sec_context, the default credential means that any currently	    */
/* registered server name may be used to accept the context.		    */
	    cred_ptr = NULL;
	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug8, 
            "Called gss_accept_sec_ctx with default credential"));
	} else {
	    if ((cred_ptr = resolve_cred(verifier_cred_handle)) == NULL) {
		*minor_status = gssdce_s_invalid_handle;
		O_status = GSS_S_DEFECTIVE_CREDENTIAL;
		dce_svc_printf(GSS_S_AC_BAD_CRED_MSG);
		goto accept_sec_ctx_return;
	    };
	    if (!(cred_ptr->flags & CRED_C_FL_ACCEPT)) {
		*minor_status = gssdce_s_bad_usage;
		O_status = GSS_S_NO_CRED;
		goto accept_sec_ctx_return;
	    };
	};
/* Now we have a valid credential from which to build the context.	    */


/* Allocate a fresh context if necessary.				    */
	if (ctx_ptr == NULL) {
	    /* We weren't given an old context to re-use.  Create a fresh   */
	    /* one.							    */
	    if ((ctx_ptr = create_ctx()) == NULL) {
		*minor_status = gssdce_s_out_of_memory;
		O_status = GSS_S_FAILURE;
		dce_svc_printf(GSS_S_AC_CTX_NOMEM_MSG);
		goto accept_sec_ctx_return;
	    };
	    *context_handle = ctx_info_to_gss(ctx_ptr);
	} else {
	    /* Make sure that if we're re-using an old context that it's    */
	    /* completely clean.					    */
	    memset(ctx_ptr, 0, sizeof(ctx_info));
	};
	ctx_ptr->state = CTX_C_ST_CLOSED;   
	if (cred_ptr) {
	    ctx_ptr->local_cred = cred_info_to_gss(cred_ptr);
	} else ctx_ptr->local_cred = GSS_C_NO_CREDENTIAL;
	ctx_ptr->deleg_cred = GSS_C_NO_CREDENTIAL;
	ctx_ptr->actual_mech_type =
	    (gss_OID_desc *)malloc(sizeof(gss_OID_desc));
	if (ctx_ptr->actual_mech_type == NULL) {
	    *minor_status = gssdce_s_out_of_memory;
	    dce_svc_printf(GSS_S_AC_NOMEM1_MSG);
	    O_status = GSS_S_FAILURE;
	    goto accept_sec_ctx_return;
	};
	ctx_ptr->actual_mech_type->length = 0;
	ctx_ptr->actual_mech_type->elements = NULL;

	*minor_status = gss__unwrap_token(input_token_buffer,
					 ctx_ptr->actual_mech_type,
					 token_type,
					 &rcv_message);


	if (*minor_status != gssdce_s_success) {
	    dce_svc_printf(GSS_S_AC_BAD_TOKEN_WRAPPER_MSG);
	    O_status = GSS_S_DEFECTIVE_TOKEN;
	    goto accept_sec_ctx_return;
	};


	if (nm_oid_equal(ctx_ptr->actual_mech_type,
			 GSSDCE_C_OID_DCE_KRBV5_DES)) {
	    ctx_ptr->flags = CTX_C_FL_DCE1_MECH;
	    nm_oid_free(ctx_ptr->actual_mech_type, 1);
	    ctx_ptr->actual_mech_type = GSSDCE_C_OID_DCE_KRBV5_DES;
	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, 
            "Processing DCE mechanism token"));
	} else 	if (nm_oid_equal(ctx_ptr->actual_mech_type,
				 GSSDCE_C_OID_KRBV5_DES_RFC)) {
	    ctx_ptr->flags = CTX_C_FL_KRB5_MECH;
	    nm_oid_free(ctx_ptr->actual_mech_type, 1);
/* Reply to an RFC protocol request with whatever our default protocol is   */
/* configured as.							    */
	    ctx_ptr->actual_mech_type = GSSDCE_C_OID_KRBV5_DES;
	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, 
            "Processing KRB mechanism token (RFC protocol)"));
	} else 	if (nm_oid_equal(ctx_ptr->actual_mech_type,
				 GSSDCE_C_OID_KRBV5_DES_ID)) {
	    ctx_ptr->flags = CTX_C_FL_KRB5_MECH;
	    nm_oid_free(ctx_ptr->actual_mech_type, 1);
/* Reply to an IDC protocol request with an ID response.		    */
	    ctx_ptr->actual_mech_type = GSSDCE_C_OID_KRBV5_DES_ID;
	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, 
            "Processing KRB mechanism token (ID protocol)"));
	} else {
	    dce_svc_printf(GSS_S_AC_INV_MECH_MSG);
	    O_status = GSS_S_DEFECTIVE_TOKEN;
	    *minor_status = gssdce_s_unsupported_mechanism;
	    nm_oid_free(ctx_ptr->actual_mech_type, 1);
	    ctx_ptr->actual_mech_type = NULL;
	    goto accept_sec_ctx_return;
	};

	if ((token_type[0] != TOKEN_TYPE_INIT[0]) ||
	    (token_type[1] != TOKEN_TYPE_INIT[1])) {
	    dce_svc_printf(GSS_S_AC_BAD_TOK_TYPE_MSG,
			   token_type[0],
			   token_type[1]);
	    O_status = GSS_S_DEFECTIVE_TOKEN;
	    *minor_status = gssdce_s_unexpected_token_type;
	    ctx_ptr->actual_mech_type = NULL;
	    goto accept_sec_ctx_return;
	};

/* Now we have an initialized context data structure containing all the	    */
/* elements we need to accept a context.  Make it so...			    */

	DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, 
        "Unwrapped token (Token-type INIT)"));
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, 
        "Calling sec_krb_gss_decode_message..."));
	error_status = sec_krb_gss_decode_message(&rcv_message,
						  &ctx_ptr->ticketted_name,
						  &ctx_ptr->client_pac,
						  &ctx_ptr->client_creds,
						  &ctx_ptr->server_name,
						  &ctx_ptr->authn_protocol,
						  &ctx_ptr->authz_protocol,
						  &ctx_ptr->auth_session_key,
						  &ctx_ptr->expiration,
						  &chan_binding_hash,
						  &ctx_ptr->context_flags,
						  &ctx_ptr->rcv_seq,
						  &ctx_ptr->ctime,
						  &ctx_ptr->cusec);

	if (error_status != error_status_ok) {
/*	    *minor_status = gssdce_s_authentication_error; */
	    *minor_status = error_status;
	    O_status = GSS_S_FAILURE;
	    dce_svc_printf(GSS_S_AC_SKDM_FAILED_MSG, error_status);
	    goto accept_sec_ctx_return;
	};
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, 
        "Returned from sec_krb_gss_decode_message"));
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, 
        "Initiator's sequence number: %ld", (long)(ctx_ptr->rcv_seq)));

	if (cred_ptr) {
/* We have to make sure that the token was destined for this server	    */
/* principal.								    */

	    char *cred_princ;

	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, 
            "Checking explicit acceptor credential identity..."));
	    if (!cred_ptr->principal_name_valid) {
		DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, 
                "Expanding acceptor credential"));
		if ((*minor_status = cred_expand_id(cred_ptr)
		    ) != gssdce_s_success) {
		    dce_svc_printf(GSS_S_AC_CEI_FAILED_MSG, *minor_status);
		    O_status = GSS_S_DEFECTIVE_CREDENTIAL;
		    goto accept_sec_ctx_return;
		};
	    };

	    /*
	     * unparse_name returns global form, so need to canonicalize
	     * cred_ptr->principal_name to match
	     */
	    sec_krb_unparse_name(ctx_ptr->server_name, &actual_server_name);

	    cred_princ = malloc(u_strlen(cred_ptr->principal_realm_name) +
				u_strlen(cred_ptr->principal_local_name) + 2);
	    if (!cred_princ) {
		free(actual_server_name);
		goto accept_sec_ctx_return;
	    }
	    u_strcpy(cred_princ, cred_ptr->principal_realm_name);
	    u_strcat(cred_princ, "/");
	    u_strcat(cred_princ, cred_ptr->principal_local_name);

	    if (u_strcmp(cred_princ, actual_server_name)) {
		/* We accepted the context, but as the wrong server	    */
		/* principal.  Reject it.				    */
		*minor_status = gssdce_s_incorrect_server_principal;
		O_status = GSS_S_FAILURE;
		dce_svc_printf(GSS_S_AC_NAMES_DIFFER_MSG);
		free(cred_princ);
		free(actual_server_name);
		goto accept_sec_ctx_return;
	    };
	    free(cred_princ);
	    free(actual_server_name);
	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, 
            "Names match"));

	};
/* Check the channel bindings...					    */
	if (ctx_ptr->authz_protocol == rpc_c_authz_name) {
	    ctx_ptr->client_name =
		(unsigned char *)malloc(
		    strlen((char *)(ctx_ptr->ticketted_name))+1);
	    if (ctx_ptr->client_name == NULL) {
		dce_svc_printf(GSS_S_AC_NOMEM2_MSG);
		*minor_status = gssdce_s_out_of_memory;
		O_status = GSS_S_FAILURE;
		goto accept_sec_ctx_return;
	    };
	    strcpy((char *)ctx_ptr->client_name, 
		   (char *)ctx_ptr->ticketted_name);	
	} else if (ctx_ptr->authz_protocol == rpc_c_authz_dce) {

	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, 
            "Hashing channel bindings"));
	    hash_channel_bindings(input_chan_bindings,
				  binding_hash_data);

	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, 
            "Returned from hashing bindings"));
	    if (chan_binding_hash.length != sizeof(binding_hash_data)) {
		*minor_status = gssdce_s_bindings_length_error;
		O_status = GSS_S_BAD_BINDINGS;
		dce_svc_printf(GSS_S_AC_HASH_BAD_LENGTH_MSG,
			       sizeof(binding_hash_data),
			       chan_binding_hash.length);
		goto accept_sec_ctx_return;
	    };
	    if (memcmp(chan_binding_hash.value, 
		       binding_hash_data,
		       chan_binding_hash.length) != 0) {
		*minor_status = gssdce_s_bindings_value_error;
		O_status = GSS_S_BAD_BINDINGS;
		dce_svc_printf(GSS_S_AC_BINDINGS_MISMATCH_MSG);
		goto accept_sec_ctx_return;
	    };

	    if (src_name) {
		DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, 
                "Extracting client name..."));
		error_status = ctx_to_name(ctx_ptr);
		if (error_status != rpc_s_ok) {
		    dce_svc_printf(GSS_S_AC_NO_PAC_NAME_MSG, error_status);
		    *minor_status = gssdce_s_couldnt_translate_name;
		    O_status = GSS_S_FAILURE;
		   goto accept_sec_ctx_return;
		};
		DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, 
                "Got client's name"));
	    };
	} else {
	    *minor_status = gssdce_s_unsupported_authz_protocol;
	    O_status = GSS_S_FAILURE;
	    dce_svc_printf(GSS_S_AC_UNK_AUTHZ_MSG);
	    goto accept_sec_ctx_return;
	};


	rcv_message.data = NULL;
	rcv_message.length = 0;
	
/* We authenticated the client.  Now to build a reply message to him...	    */

	DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, 
        "Client succesfully authenticated"));


/* First generate a random sequence number...				    */

	gss_generate_seq(&ctx_ptr->snd_seq);

	if (ctx_ptr->context_flags & GSS_C_MUTUAL_FLAG) {

	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, 
            "Context specified mutual authentication, creating reply"));
	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, 
            "Calling sec_krb_gss_build_rep_message..."));
	    error_status = sec_krb_gss_build_rep_message(
					    rpc_c_authn_level_default,
					    /* ctx_ptr->authn_protocol,	*/
					    ctx_ptr->ctime,
					    ctx_ptr->cusec,
					    ctx_ptr->auth_session_key,
					    ctx_ptr->snd_seq,
					    &snd_message);

	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, 
            "Returned from sec_krb_gss_build_rep_message"));
	    if (error_status != error_status_ok) {
/*	    *minor_status = gssdce_s_couldnt_build_reply; */
		*minor_status = error_status;
		O_status = GSS_S_FAILURE;
		dce_svc_printf(GSS_S_AC_SKBR_FAILED_MSG, error_status);
		goto accept_sec_ctx_return;
	    };

/* Now to wrap the reply token...					    */
	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, 
            "Wrapping token (MUTUAL)"));
	    *minor_status = gss__wrap_token(ctx_ptr->actual_mech_type,
					    TOKEN_TYPE_MUTUAL,
					    snd_message,
					    output_token);
	} else {
	    output_token->length = 0;
	    output_token->value = NULL;
/* Should also mark the context as being one-way secure...		    */
	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, 
            "Context not mutually authenticated, no reply token necessary"));
	};

	if (src_name) {

	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, 
            "Copying client name to caller's buffer"));
	    strncpy((char *)temp_name, 
		    (char *)(ctx_ptr->client_name), 
		    sizeof(temp_name));
	    string_to_irep(temp_name, src_name, &error_status);
	
	    if (error_status != rpc_s_ok) {
		dce_svc_printf(GSS_S_AC_CLI_NAME_NOCONV_MSG, error_status);
		*minor_status = gssdce_s_couldnt_translate_name;
		O_status = GSS_S_FAILURE;
		goto accept_sec_ctx_return;
	    };
	};

	
	if (mech_type) {
	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, 
            "Copying mech-type OID to caller's buffer"));
	    *mech_type = ctx_ptr->actual_mech_type;
	};

	    
	if (ret_flags) {
	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, 
            "Copying ret_flags to caller's buffer"));
	    *ret_flags = ctx_ptr->context_flags;
	};

	if (time_rec) {
	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, 
            "Copying context-time to caller's buffer"));
	    *time_rec = 0xffffffffu;
	};
	if (delegated_cred_handle) {
#ifndef GSS_DELEGATION 
	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug8, 
            "Delegation not yet supported; NULLing caller's buffer"));
	    *delegated_cred_handle = 0;
#else
	    if ((*ret_flags) & GSS_C_DELEG_FLAG) {
/* We've received a delegation attempt.  Find out what type of delegation,  */
/* and act accordingly...						    */

		deleg_type =
		    gss_cred_get_delegation_type(ctx_ptr->client_creds,
						 &error_status);
		if (error_status) {
		    dce_svc_printf(GSS_S_AC_UNK_DELEG_TYPE_MSG, error_status);
		    *minor_status = error_status;
		    O_status = GSS_S_FAILURE;
		    goto accept_sec_ctx_return;
		};

/* To become a delegate or impersonator, we need to have a login-context of */
/* our own.  If the user supplied a BOTH-mode credential, we already have a */
/* login-context.  If not, we'll have to create one (just as we'd have to   */
/* to contact the registry for this user).				    */

		if (cred_ptr) {
		    if (error_status =
			    get_auxiliary_context(cred_ptr,
						  &my_login_context)
		       ) {
			dce_svc_printf(GSS_S_AC_NO_LC_MSG, error_status);
			*minor_status = gssdce_s_delegation_error;
			O_status = GSS_S_FAILURE;
			goto accept_sec_ctx_return;
		    };

		} else {
		    dce_svc_printf(GSS_S_AC_TRACE_NO_LC_MSG);
		    *minor_status = gssdce_s_delegation_error;
		    O_status = GSS_S_FAILURE;
		    goto accept_sec_ctx_return;
		};

/* Now we've got a login-context to use to accept the delegation in	    */
/* <my_login-context>.  Accept whatever delegation's been offered...	    */

		switch (deleg_type) {
		case sec_id_deleg_type_traced:
			

		    login_context = sec_login_become_delegate(
					ctx_ptr->client_creds,
					my_login_context,
					sec_id_deleg_type_traced,
					NULL,
					NULL,
					NULL,
					NULL,
					sec_id_compat_mode_caller,
					&error_status);
		    if (error_status) {
			dce_svc_printf(GSS_S_AC_NO_DELEG1_MSG, error_status);
			*minor_status = gssdce_s_delegation_error;
			O_status = GSS_S_FAILURE;
			goto accept_sec_ctx_return;
		    };
		    break;
		case sec_id_deleg_type_impersonation:
		    login_context = sec_login_become_impersonator(
					ctx_ptr->client_creds,
					my_login_context,
					sec_id_deleg_type_impersonation,
					NULL,
					NULL,
					NULL,
					NULL,
					&error_status);
		    if (error_status) {
			dce_svc_printf(GSS_S_AC_NO_DELEG2_MSG, error_status);
			*minor_status = gssdce_s_delegation_error;
			O_status = GSS_S_FAILURE;
			goto accept_sec_ctx_return;
		    };
		    
		    break;
		case sec_id_deleg_type_none:
		    dce_svc_printf(GSS_S_AC_INV_DELEG_MSG);
		    *minor_status = gssdce_s_delegation_error;
		    O_status = GSS_S_FAILURE;
		    goto accept_sec_ctx_return;
		default:
		    dce_svc_printf(GSS_S_AC_UNK_DELEG_TYPE1_MSG, deleg_type);
		    *minor_status = gssdce_s_delegation_error;
		    O_status = GSS_S_FAILURE;
		    goto accept_sec_ctx_return;
		};		
		if (login_context) {
/* Don't save the delegated credential for now				    */
		    ctx_ptr->deleg_cred = GSS_C_NO_CREDENTIAL;

		    O_status = gssdce_login_context_to_cred(
					    minor_status,
					    login_context,
					    GSS_C_INDEFINITE,
					    &dce_mech_set,
					    delegated_cred_handle,
					    NULL,
					    NULL);
		    if (*minor_status) {

			dce_svc_printf(GSS_S_AC_LCC_FAILED_MSG, *minor_status);
		    };
		};
	    } else {
		*delegated_cred_handle = 0;
	    };
#endif
	};
	*minor_status = gssdce_s_success;
	O_status = GSS_S_COMPLETE;
    };
    ctx_ptr->authn_protocol = rpc_c_authn_dce_secret;	
    ctx_ptr->state = CTX_C_ST_OPEN_A;   


accept_sec_ctx_return:

    DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, 
    "Common return path from gss_accept_sec_ctx"));



    if (snd_message.length != 0) {
	sec_krb_message_free(&snd_message);
    };
    if (cred_ptr != NULL) {
	UNLOCK_ELEMENT(cred_ptr);
    };
    if (ctx_ptr != NULL) {
	UNLOCK_ELEMENT(ctx_ptr);
    };

    return O_status;
}


/****************************************************************************/
/*									    */
/*			   gss_build_prot_message			    */
/*									    */
/****************************************************************************/


static OM_uint32 gss_build_prot_message(gss_buffer_t  message_buffer,
				        ctx_info *    ctx_ptr,
				        int	      tok_type,
				        unsigned int  sign_alg_type,
					unsigned int  seal_alg_type,
				        gss_buffer_t  token_buffer) {
    OM_uint32 O_status;
    MD5_CTX context;
    unsigned char cksum_iv[8];
    unsigned char md_block[8];
    int data_start = 0;
    unsigned char seq_buffer[8];
    unsigned char confounder[8];
    int i;
    int i_am_initiator;
    unsigned char seq_pad_val;
    gss_buffer_desc internal_buffer;
    sec_krb_message fake_buffer;
    
#   if defined(DEBUG_PROT)
    unsigned char debug_c_buffer[8];
#   endif

    O_status = gssdce_s_success;

    DCE_SVC_DEBUG((gss__svc_handle, gss_s_protect, svc_c_debug9, 
    "Entering gss_build_prot_message"));
    
/* Signature is composed as follows:
 * First field is a two-byte token-id, containing either 01 01, 02 01 or
 * 01 02.
 * This field is set from the token_type parameter:
 *	01 01 - GSS_Sign() token
 *	01 02 - Context deletion token
 *	02 01 - GSS_Seal() token 
 *
 * Second field is a two-byte signataure algorithm ID.  Currently, the
 * following algorithms are supported:
 *	00 - DES-CBC-MAC of MD5
 *	01 - MD5 (MD2.5)
 *      02 - DES-CBC-MAC
 *
 * Third field is a two-byte confidentiality algorithm ID for TOK_ID = 02 01,
 * otherwise 0xffff.
 *
 * Fourth field is filler 0xffff.
 *
 * Fifth field is an 8-byte encrypted sequence number, calculated by
 * creating a DES-block from the context's 4-byte send sequence number
 * (little-endian format), wiht either 4 bytes of 0x00 (if the packet is being
 * sent by the context initiator) or four bytes of 0xff (if the packet is being
 * sent by the context acceptor).  The resulting 8-byte field is then
 * encrypted using the context's session key, and the first 8 bytes of the
 * following checksum field as an IV.
 *
 * Then we calculate a checksum over the message data, according to the
 * algorithm specified  by sig_alg_type.  A DES-CBC MAC is 8-bytes, while a
 * MD5 checksum is 16-bytes.  However, to avoid a checksum chaining attack,
 * we only store the first 8 bytes of an MD5 checksum, so all supported
 * checksums on the wire are 8 bytes.
 *
 * This allows the receiver to verify the data independently of the sequence
 * number, and then to optionally check the sequence number.  The sequence
 * number is conveyed in a valid packet, allowing the receiver to
 * re-synchronize in the event of mis-ordered delivery, whether or not
 * out-of-sequence delivery protection is requested.
 */

    token_buffer->length = 0;
    internal_buffer.length = 0;

    if (ctx_ptr->state == CTX_C_ST_OPEN_I) {
	i_am_initiator = 1;
    } else if (ctx_ptr->state == CTX_C_ST_OPEN_A) {
	i_am_initiator = 0;
    } else {
	dce_svc_printf(GSS_S_BP_INV_STATE_MSG);
	return gssdce_s_invalid_context_state;
    };

    seq_pad_val = (i_am_initiator ? 0x00 : 0xff);

/* First validate the input parameters.					    */


    if (sign_alg_type != SIGN_ALG_MD5 &&
	sign_alg_type != SIGN_ALG_DES_MAC &&
	sign_alg_type != SIGN_ALG_DES_MD5) {
	O_status = gssdce_s_unsupported_signature_algorithm;
	dce_svc_printf(GSS_S_BP_UNK_SIG_ALG_MSG);
	goto gss_build_prot_message_return;
    };

    if (tok_type != TOK_ID_SIGN &&
        tok_type != TOK_ID_DELETE &&
	tok_type != TOK_ID_SEAL) {
	O_status = gssdce_s_unrecognized_token_type;
	dce_svc_printf(GSS_S_BP_INV_TOK_TYPE_MSG);
	goto gss_build_prot_message_return;
    };

    if ((tok_type == TOK_ID_SIGN && seal_alg_type != SEAL_ALG_NONE)
	|| (tok_type == TOK_ID_DELETE && seal_alg_type != SEAL_ALG_NONE))
    {
	O_status = gssdce_s_seal_algorithm_not_allowed;
	dce_svc_printf(GSS_S_BP_NOSEAL_MSG);
	goto gss_build_prot_message_return;
    };


    if ((tok_type == TOK_ID_SEAL) && 
	(seal_alg_type != SEAL_ALG_DES) &&
	(seal_alg_type != SEAL_ALG_NONE)) {
	O_status = gssdce_s_unsupported_encryption_algorithm;
	dce_svc_printf(GSS_S_BP_UNK_ENC_ALG_MSG);
	goto gss_build_prot_message_return;
    };

/* Calculate total message length.  Basic sign header is 24 bytes. For	    */
/* tok_id 1, that's all there is; for tok_id 2 and 3 we have the	    */
/* data_length, rounded up to the next highest multiple of 8 bytes, plus 8  */
/* bytes of confounder.							    */

    internal_buffer.length = 24;

    switch (tok_type) {
    case TOK_ID_SIGN:
    case TOK_ID_DELETE:
	/* No extra length over the sign header				    */
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_protect, svc_c_debug9, 
        "SIGN/DELETE token-type"));
	break;
    case TOK_ID_SEAL:
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_protect, svc_c_debug9, 
        "SEAL token-type"));
	data_start = internal_buffer.length;
	switch (seal_alg_type) {
	case SEAL_ALG_DES:
	case SEAL_ALG_NONE:
	    internal_buffer.length += 8 * ((message_buffer->length + 8) / 8);
	    /* The padded message					    */
	    internal_buffer.length += 8;
	    /* And the confounder					    */
	    break;
	default:
	    /* This is an error condition */
	    dce_svc_printf(GSS_S_BP_INV_SEAL_ALG_MSG);
	    break;
	};
	break;
    };

    if ((internal_buffer.value = (void *)malloc(internal_buffer.length)
	) == NULL) {
	O_status = gssdce_s_out_of_memory;
	internal_buffer.length = 0;
	dce_svc_printf(GSS_S_BP_NOMEM1_MSG);
	goto gss_build_prot_message_return;
    };

    DCE_SVC_DEBUG((gss__svc_handle, gss_s_protect, svc_c_debug9, 
    "Setting token type field..."));

    switch (tok_type) {
    case TOK_ID_SIGN:
	*((unsigned char *)internal_buffer.value)     = TOKEN_TYPE_SIGN[0];
	*((unsigned char *)internal_buffer.value + 1) = TOKEN_TYPE_SIGN[1];
	break;	
    case TOK_ID_SEAL:
	*((unsigned char *)internal_buffer.value)     = TOKEN_TYPE_SEAL[0];
	*((unsigned char *)internal_buffer.value + 1) = TOKEN_TYPE_SEAL[1];
	break;	
    case TOK_ID_DELETE:
	*((unsigned char *)internal_buffer.value)     = TOKEN_TYPE_DELETE[0];
	*((unsigned char *)internal_buffer.value + 1) = TOKEN_TYPE_DELETE[1];
	break;	
    };

    DCE_SVC_DEBUG((gss__svc_handle, gss_s_protect, svc_c_debug9, 
    "Setting token sign-alg field..."));

/* SIGN_ALG								    */
    *((unsigned char *)internal_buffer.value + 2) =
		    sign_alg_type & 0xffu;
    *((unsigned char *)internal_buffer.value + 3) =
		    (sign_alg_type & 0xff00u) >> 8;
		    
    DCE_SVC_DEBUG((gss__svc_handle, gss_s_protect, svc_c_debug9, 
    "Setting token seal-alg field..."));

/* SEAL_ALG								    */
    *((unsigned char *)internal_buffer.value + 4) =
		    seal_alg_type & 0xffu;
    *((unsigned char *)internal_buffer.value + 5) =
		    (seal_alg_type & 0xff00u) >> 8;

/* Padding								    */
    *((unsigned char *)internal_buffer.value + 6) = 0xff;
    *((unsigned char *)internal_buffer.value + 7) = 0xff;

    DCE_SVC_DEBUG((gss__svc_handle, gss_s_protect, svc_c_debug9, 
    "Signing token..."));

    switch (tok_type) {
    case TOK_ID_SIGN:
    case TOK_ID_DELETE:

	switch (sign_alg_type) {
	case SIGN_ALG_DES_MAC:
/* First part of signature covers plaintext packet header.		    */

	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_protect, svc_c_debug9, 
            "DES-MAC signature..."));
	    if (tok_type == TOK_ID_SIGN) {
		gss_DES_MAC(zero_block,
			    internal_buffer.value,
			    8,
			    ctx_ptr,
			    cksum_iv);
/* Next part is over data field.					    */
		gss_DES_MAC(cksum_iv,		    /* IV		    */
			    message_buffer->value,  /* Data		    */
			    message_buffer->length, /* Length		    */
			    ctx_ptr,		    /* key_info		    */
			    (unsigned char *)internal_buffer.value + 16);
	    } else {
		gss_DES_MAC(zero_block,
			    internal_buffer.value,
			    8,
			    ctx_ptr,
			    (unsigned char *)internal_buffer.value + 16);
	    };
	    break;
	case SIGN_ALG_MD5:

	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_protect, svc_c_debug9, 
            "MD5 signature..."));

	    gss_fixup_md_init(ctx_ptr);
	    gss_MD5Init(&context);
/* First part is key-dependent initializer				    */
	    gss_MD5Update(&context,
			  ctx_ptr->md_init,
			  ctx_ptr->md_init_length);
/* Next part is over plaintext header					    */

	    gss_MD5Update(&context,
			  internal_buffer.value,
			  8);
	    if (tok_type == TOK_ID_SIGN) {
/* Final part is over data						    */
		gss_MD5Update(&context,
			      message_buffer->value,
			      message_buffer->length);
	    };
	    gss_MD5Final(&context);
	    memcpy((unsigned char*)internal_buffer.value + 16,
		   context.digest,
		   8);
	    break;
	case SIGN_ALG_DES_MD5:
	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_protect, svc_c_debug9, 
            "DES(MD5) signature..."));
	    gss_MD5Init(&context);
/* First part is over plaintext header					    */
	    gss_MD5Update(&context,
			  internal_buffer.value,
			  8);
	    if (tok_type == TOK_ID_SIGN) {
/* Final part is over data						    */
		gss_MD5Update(&context,
			      message_buffer->value,
			      message_buffer->length);
	    };
	    gss_MD5Final(&context);
	    gss_DES_MAC(zero_block,
			context.digest,
			16,
			ctx_ptr,
			(unsigned char*)internal_buffer.value + 16);
	    break;
	};
	break;
    case TOK_ID_SEAL:
/* Copy and pad the data into the message				    */

	DCE_SVC_DEBUG((gss__svc_handle, gss_s_protect, svc_c_debug9, 
        "Generating confounder..."));

	gss_generate_random_block(confounder);	

	memcpy((unsigned char *)internal_buffer.value + data_start,
	       confounder,
	       8);

	DCE_SVC_DEBUG((gss__svc_handle, gss_s_protect, svc_c_debug9, 
        "Copying %d byte message into token...", message_buffer->length));
	memcpy((unsigned char *)internal_buffer.value + data_start+8,
	       message_buffer->value,
	       message_buffer->length);

	DCE_SVC_DEBUG((gss__svc_handle, gss_s_protect, svc_c_debug9, 
        "Adding %d bytes of padding...", 8 - (message_buffer->length %8)));
	memset((unsigned char *)internal_buffer.value +
				data_start+8+message_buffer->length,
	       8 - (message_buffer->length % 8),
	       8 - (message_buffer->length % 8));

	DCE_SVC_DEBUG((gss__svc_handle, gss_s_protect, svc_c_debug9, 
        "Total msg length including header, confounder GSS_S_BP_TOT_LENGTH_MSG padding is %d", internal_buffer.length));

	switch (sign_alg_type) {
	case SIGN_ALG_DES_MAC:
/* First part of signature covers plaintext packet header.		    */
	    gss_DES_MAC(zero_block,
			internal_buffer.value,
			8,
			ctx_ptr,
			cksum_iv);
/* Next part is over data field.					    */
	    gss_DES_MAC(cksum_iv,		  /* IV			    */
			(unsigned char *)internal_buffer.value + data_start,
						  /* Data		    */
			internal_buffer.length - data_start,
						  /* Length		    */
			ctx_ptr,		  /* key_info		    */
			(unsigned char*)internal_buffer.value + 16);
	    break;
	case SIGN_ALG_MD5:
	    gss_fixup_md_init(ctx_ptr);
	    gss_MD5Init(&context);
/* First part is key-dependent initializer				    */
	    gss_MD5Update(&context,
			  ctx_ptr->md_init,
			  ctx_ptr->md_init_length);
/* Next part is over plaintext header					    */
	    gss_MD5Update(&context,
			  internal_buffer.value,
			  8);
/* Final part is over padded data					    */
	    gss_MD5Update(&context,
			  (unsigned char *)internal_buffer.value + data_start,
			  internal_buffer.length - data_start);
	    gss_MD5Final(&context);
	    memcpy((unsigned char*)internal_buffer.value + 16,
		   context.digest,
		   8);
	    break;
	case SIGN_ALG_DES_MD5:
	    gss_MD5Init(&context);
/* First part is over plaintext header					    */
	    gss_MD5Update(&context,
			  internal_buffer.value,
			  8);
/* Final part is over padded data					    */
	    gss_MD5Update(&context,
			  (unsigned char *)internal_buffer.value + data_start,
			  internal_buffer.length - data_start);
	    gss_MD5Final(&context);
	    gss_DES_MAC(zero_block,
			context.digest,
			16,
			ctx_ptr,
			(unsigned char*)internal_buffer.value + 16);
	    break;
	    
	};
    };

    DCE_SVC_DEBUG((gss__svc_handle, gss_s_protect, svc_c_debug9, 
    "Setting sequence number..."));

/* Now for the sequence number						    */
    seq_buffer[0] = ctx_ptr->snd_seq & 0xfful;
    seq_buffer[1] = (ctx_ptr->snd_seq & 0xff00ul) >> 8 ;
    seq_buffer[2] = (ctx_ptr->snd_seq & 0xff0000ul) >> 16;
    seq_buffer[3] = (ctx_ptr->snd_seq & 0xff000000ul) >> 24;

    seq_buffer[4] = seq_buffer[5] = seq_buffer[6] = seq_buffer[7] = seq_pad_val;


    INCREMENT_SEQ(ctx_ptr->snd_seq);

    DCE_SVC_DEBUG((gss__svc_handle, gss_s_protect, svc_c_debug9, 
    "Encrypting sequence number..."));
    gss_encrypt_DES_CBC(
	((unsigned char *)internal_buffer.value) + 16,
				/* IV					    */
	seq_buffer,		/* Data					    */
	8,			/* Length				    */
	ctx_ptr,		/* Key					    */
	INTEGRITY_KEY,
	((unsigned char *)internal_buffer.value) + 8);
				/* Output				    */



    switch (tok_type) {
    case TOK_ID_SIGN:
    case TOK_ID_DELETE:
	break;
    case TOK_ID_SEAL:
#ifndef NOENCRYPTION
	if (seal_alg_type == SEAL_ALG_DES) {
/* Encrypt data field in-place.						    */

	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_protect, svc_c_debug9, 
            "Encrypting data..."));
	    gss_encrypt_DES_CBC(
		zero_block,				/* IV		    */
		(unsigned char *)internal_buffer.value + data_start,
							/* Data		    */
		internal_buffer.length - data_start,	/* Length	    */
		ctx_ptr,				/* Key		    */
		PRIVACY_KEY,
		(unsigned char *)internal_buffer.value + data_start);
							/* Output in place  */
	};
#endif
	break;
    };




/* The wrapping routines deal with the 2-byte token-type field.  Since	    */
/* we've already generated it (we had to, since it's included under the	    */
/* signature calculations), we have to strip it off so that the wrapping    */
/* routine can add it back again.					    */

    fake_buffer.length = internal_buffer.length - 2;
    fake_buffer.data = (unsigned char *)internal_buffer.value + 2;

    DCE_SVC_DEBUG((gss__svc_handle, gss_s_protect, svc_c_debug9, 
    "Wrapping token..."));

    gss__wrap_token(ctx_ptr->actual_mech_type,
		    (unsigned char *)internal_buffer.value,
		    fake_buffer,
		    token_buffer);

    free(internal_buffer.value);
    
gss_build_prot_message_return:
    
    return O_status;
    
}

/****************************************************************************/
/*									    */
/*		     verify_received_sequence_number			    */
/*									    */
/****************************************************************************/

static OM_uint32 verify_received_sequence_number(OM_uint32* minor_status,
					         OM_uint32 received_seq_no,
					         ctx_info * ctx_ptr) {

    OM_uint32 O_status;
    int seq_range;
    int token_registered;
    
    *minor_status = gssdce_s_operation_incomplete;
    token_registered = 0;

    DCE_SVC_DEBUG((gss__svc_handle, gss_s_protect, svc_c_debug9, 
    "Checking received sequence number %ld, expecting %ld",
		 (long)received_seq_no,
		 (long)(ctx_ptr->rcv_seq)));
    
    seq_range = compare_seq_numbers(received_seq_no,
				    ctx_ptr->rcv_seq);
/* Compare_seq_numbers returns 0 for equal, -1 for 1st argument in window   */
/* but less than 2nd, +1 for 1st argument in window but greater than 2nd,   */
/* and -2 or +2 for significant difference between 1st and second arguments */

    switch (seq_range) {
    case 0:
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_protect, svc_c_debug9, 
        "Sequence number valid"));
	INCREMENT_SEQ(ctx_ptr->rcv_seq);
	O_status = GSS_S_COMPLETE;
	*minor_status = gssdce_s_success;
	break;
    case 1:
/* We have a packet that's been received ahead of time - in other words	    */
/* we've missed one or more packets.  We may get them later, but for now    */
/* update rcv_seq and let our caller know that a packet is missing.	    */

	DCE_SVC_DEBUG((gss__svc_handle, gss_s_protect, svc_c_debug9, 
        "Sequence number received early"));
	ctx_ptr->rcv_seq = received_seq_no;
	INCREMENT_SEQ(ctx_ptr->rcv_seq);
	O_status = GSS_S_UNSEQ_TOKEN;
	*minor_status = gssdce_s_message_omitted;
	break;	    
    case -1:
/* We have a packet whose sequence number is lower than we were expecting.  */
/*  It's either a duplicate packet, or it's just been received late.  Don't */
/*  alter our expected rcv_seq, but tell our caller that the packet is out  */
/*  of order.								    */

	DCE_SVC_DEBUG((gss__svc_handle, gss_s_protect, svc_c_debug9, 
        "Sequence number received late"));

	O_status = GSS_S_UNSEQ_TOKEN | GSS_S_OLD_TOKEN;
	if (token_received(ctx_ptr, received_seq_no)) {
/* Token_received registers the sequence number, and returns non-zero if    */
/* the sequence number had already been registered.			    */
	    *minor_status = gssdce_s_message_already_seen;
	    O_status |= GSS_S_DUPLICATE_TOKEN;
	} else *minor_status = gssdce_s_message_late;
	token_registered = 1;
	break;
    default:
/* We have a packet that's severely out of order.			    */

	dce_svc_printf(GSS_S_VRS_SEQ_INVALID_MSG);
	*minor_status = gssdce_s_message_out_of_order;
	O_status = GSS_S_UNSEQ_TOKEN;
	break;
    };	

    if (!token_registered) token_received(ctx_ptr, received_seq_no);

verify_received_sequence_number_return:

    return O_status;

}



/****************************************************************************/
/*									    */
/*			     decode_message_header 			    */
/*									    */
/****************************************************************************/
static OM_uint32 decode_message_header(gss_buffer_t  token_buffer,
				       ctx_info *    ctx_ptr,
				       int *	     tok_id_type,
				       int *	     sign_alg_type,
				       int *	     seal_alg_type,
				       unsigned char* msg_checksum,
				       unsigned char* checksum_iv,
				       OM_uint32 *  sequence_number,
				       int *	    data_offset) {
    unsigned checksum_size;
    unsigned char loc_seq_buffer[8];
    int i;
    OM_uint32 O_status = gssdce_s_success;
    int i_am_initiator;
    unsigned char seq_pad_val;
    OM_object_identifier actual_mech_type;
    sec_krb_message inbuf;
    unsigned char token_type[2];
        
/* Signature is composed as follows:
 * First field is a one-byte token-id, containing either 01 01, 02 01 or 01 02.
 * This field is set from the tok_id_type parameter:
 *	01 01 - GSS_Sign() token
 *	01 02 - Context deletion token
 *	02 01 - GSS_Seal() token
 * 
 * Second field is a two-byte signataure algorithm ID.  Currently, the
 * following algorithms are supported:
 *	00 - DES-CBC MAC
 *	01 - MD5
 *
 * Third field is a two-byte confidentiality algorithm ID for TOK_ID = 02 01,
 * otherwise 0xffff.
 *
 * Fourth field is filler 0xffff.
 *
 * Fifth field is an 8-byte encrypted sequence number, calculated by
 * creating a DES-block from the context's 4-byte send sequence number
 * (little-endian format), followed by either four bytes of 0x00 (if packet was
 * sent by context initiator) or four bytes of 0xff (if packet was sent by
 * context acceptor).  This is then encrypted using the context's session
 * key, and the first 8 bytes of the following checksum field as an IV.
 *
 * Then we calculate a checksum over the message data, according to the
 * algorithm specified  by sig_alg_type.  A DES-CBC MAC is 8-bytes, while a
 * MD5 checksum is 16-bytes.  However, to avoid a checksum chaining attack,
 * we only store the first 8 bytes of an MD5 checksum, so all supported
 * checksums on the wire are 8 bytes.
 *
 * This allows the receiver to verify the data independently of the sequence
 * number, and then to optionally check the sequence number.  The sequence
 * number is conveyed in a valid packet, allowing the receiver to
 * re-synchronize in the event of mis-ordered delivery, whether or not
 * out-of-sequence delivery protection is requested.
 */


    if (ctx_ptr->state == CTX_C_ST_OPEN_I) {
	i_am_initiator = 1;
    } else if (ctx_ptr->state == CTX_C_ST_OPEN_A) {
	i_am_initiator = 0;
    } else {
	return gssdce_s_invalid_context_state;
    };

    seq_pad_val = (i_am_initiator ? 0xff : 0x00);


    if (O_status = gss__unwrap_token(token_buffer,
				     &actual_mech_type,
				     token_type,
				     &inbuf)) {
	dce_svc_printf(GSS_S_DMH_UNWRAP_FAILED_MSG, O_status);
	goto decode_message_header_return;	
    };



    if (!nm_oid_equal(&actual_mech_type,
		      ctx_ptr->actual_mech_type)) {
	free(actual_mech_type.elements);
	O_status = gssdce_s_unsupported_mechanism;
	goto decode_message_header_return;	
    };
    free(actual_mech_type.elements);



    if (inbuf.length < 6) {
	O_status = gssdce_s_short_token;
	dce_svc_printf(GSS_S_DMH_SHORT_TOKEN_MSG);
	goto decode_message_header_return;	
    };    

    if ((token_type[0] == TOKEN_TYPE_SIGN[0]) &&
        (token_type[1] == TOKEN_TYPE_SIGN[1]))
	*tok_id_type = TOK_ID_SIGN;
    else if ((token_type[0] == TOKEN_TYPE_SEAL[0]) &&
	     (token_type[1] == TOKEN_TYPE_SEAL[1]))
	*tok_id_type = TOK_ID_SEAL;
    else if ((token_type[0] == TOKEN_TYPE_DELETE[0]) &&
	     (token_type[1] == TOKEN_TYPE_DELETE[1]))
	*tok_id_type = TOK_ID_DELETE;
    else {
	O_status = gssdce_s_unrecognized_token_type;
	dce_svc_printf(GSS_S_DMH_UNK_TOK_TYPE_MSG);
	goto decode_message_header_return;	
    };

    
    *sign_alg_type = (((unsigned char *)(inbuf.data))[0])
		  +  ((((unsigned char *)(inbuf.data))[1]) << 8);

    if (*sign_alg_type != SIGN_ALG_DES_MAC &&
	*sign_alg_type != SIGN_ALG_MD5 &&
	*sign_alg_type != SIGN_ALG_DES_MD5) {
	O_status = gssdce_s_unsupported_signature_algorithm;
	dce_svc_printf(GSS_S_DMH_UNK_SIGN_ALG_MSG);
	goto decode_message_header_return;	
    };


    *seal_alg_type = (((unsigned char *)(inbuf.data))[2])
		  + ((((unsigned char *)(inbuf.data))[3]) << 8);

    if (*seal_alg_type != SEAL_ALG_DES &&
	*seal_alg_type != SEAL_ALG_NONE) {
	O_status = gssdce_s_unsupported_encryption_algorithm;
	dce_svc_printf(GSS_S_DMH_UNK_ENC_ALG_MSG);
	goto decode_message_header_return;	
    };

    checksum_size = 8;

    if (*seal_alg_type != SEAL_ALG_NONE && *tok_id_type != TOK_ID_SEAL) {
	O_status = gssdce_s_unexpected_token_type;
	dce_svc_printf(GSS_S_DMH_SEAL_SD_MSG);
	goto decode_message_header_return;	
    };

    if (inbuf.length < 22) {
	O_status = gssdce_s_short_token;
	dce_svc_printf(GSS_S_DMH_SHORT_TOKEN_MSG);
	goto decode_message_header_return;	
    };    

/* Now we know that we've got valid data up to the checksum		    */


    memcpy(msg_checksum,
	   (unsigned char *)inbuf.data + 14,
	   checksum_size);

    checksum_iv[0] = token_type[0];
    checksum_iv[1] = token_type[1];

    memcpy(&checksum_iv[2],
	   (unsigned char *)inbuf.data,
	   6);

    gss_decrypt_DES_CBC(
	((unsigned char *)inbuf.data) + 14,
				/* IV					    */
	((unsigned char *)inbuf.data) + 6,
				/* Data					    */
	8,			/* Length				    */
	ctx_ptr,		/* Key					    */
	INTEGRITY_KEY,
	loc_seq_buffer);	/* Output				    */

    if ((loc_seq_buffer[4] != seq_pad_val) ||
        (loc_seq_buffer[5] != seq_pad_val) ||
        (loc_seq_buffer[6] != seq_pad_val) ||
        (loc_seq_buffer[7] != seq_pad_val)) {

	O_status = gssdce_s_bad_sig_seq;    

	dce_svc_printf(GSS_S_DMH_SEQ_NUM_INV_MSG);

	if ((loc_seq_buffer[4] == (unsigned char)(~seq_pad_val)) &&
	    (loc_seq_buffer[5] == (unsigned char)(~seq_pad_val)) &&
	    (loc_seq_buffer[6] == (unsigned char)(~seq_pad_val)) &&
	    (loc_seq_buffer[7] == (unsigned char)(~seq_pad_val))) {
	    O_status = gssdce_s_reflected_packet;    
	    dce_svc_printf(GSS_S_DMH_REFL_TOK_MSG);
	};
	goto decode_message_header_return;	
    };
     
    *sequence_number = (OM_uint32) loc_seq_buffer[0] +
		      ((OM_uint32)loc_seq_buffer[1] << 8) +
		      ((OM_uint32)loc_seq_buffer[2] << 16) +
		      ((OM_uint32)loc_seq_buffer[3] << 24);

	   
    switch (*tok_id_type) {
    case TOK_ID_SIGN:
    case TOK_ID_DELETE:
	*data_offset = -1;
	break;
    case TOK_ID_SEAL:
/* 22 bytes + ammount that was removed by gss_unwrap_token		    */
	*data_offset = 22 + ((unsigned char *)inbuf.data-
			     (unsigned char *)token_buffer->value);
	break;
    };

decode_message_header_return:
    return O_status;
}

static int normalize_qop(int qop_req) {
    if (qop_req == GSSDCE_C_QOP_MD5) return SIGN_ALG_MD5;
    if (qop_req == GSSDCE_C_QOP_DES_MAC) return SIGN_ALG_DES_MAC;
    if (qop_req == GSSDCE_C_QOP_DES_MD5) return SIGN_ALG_DES_MD5;
    return DEFAULT_SIGN_ALGORITHM;
}

/****************************************************************************/
/*									    */
/*	             gss_process_context_token routine			    */
/*									    */
/****************************************************************************/

OM_uint32 gss_process_context_token
                 (OM_uint32* minor_status,
                  gss_ctx_id_t context_handle,
                  gss_buffer_t token_buffer) {


    ctx_info * ctx_ptr = NULL;
    OM_uint32 O_status;

    unsigned char packet_checksum[8];
    int tok_id_type;
    int sign_alg_type;
    int seal_alg_type;
    int data_offset;

    unsigned char loc_checksum_buffer[8];
    unsigned char checksum_iv[8];
    unsigned char cksum_temp[8];

    MD5_CTX context;
    unsigned char md_block[8];

    OM_uint32 received_seq_no;
    int seq_range;
    int i;

    *minor_status = gssdce_s_operation_incomplete;
    GSS_INIT;

    if (token_buffer ==NULL) {
        *minor_status = gssdce_s_no_buffer;
        O_status = GSS_S_FAILURE;
	dce_svc_printf(GSS_S_PCT_NO_TOKEN_MSG);
        goto gss_proc_token_return;
    };

    if ((context_handle == GSS_C_NO_CONTEXT) ||
        ((ctx_ptr = resolve_ctx(context_handle)) == NULL)) {
	*minor_status = gssdce_s_invalid_handle;
        O_status = GSS_S_NO_CONTEXT;
	dce_svc_printf(GSS_S_PCT_INV_CTX_MSG, context_handle);
        goto gss_proc_token_return;
    };


    if ((ctx_ptr->state & CTX_M_ST_OPEN) == 0) {
        *minor_status = gssdce_s_invalid_context_state;
        O_status = GSS_S_FAILURE;
	dce_svc_printf(GSS_S_PCT_INV_CTX_STATE_MSG, context_handle);
        goto gss_proc_token_return;
    };


    memset(packet_checksum, 0, sizeof(packet_checksum));

    if ((*minor_status = decode_message_header(token_buffer,
                                               ctx_ptr,
                                               &tok_id_type,
                                               &sign_alg_type,
                                               &seal_alg_type,
                                               packet_checksum,
                                               checksum_iv,
                                               &received_seq_no,
                                               &data_offset)
        ) != 0) {
        O_status = GSS_S_DEFECTIVE_TOKEN;
        goto gss_proc_token_return;
    };

    if (tok_id_type != TOK_ID_DELETE) {
        *minor_status = gssdce_s_unexpected_token_type;
        O_status = GSS_S_DEFECTIVE_TOKEN;
        goto gss_proc_token_return;
    };



    memset(loc_checksum_buffer, 0, sizeof(loc_checksum_buffer));

    switch (sign_alg_type) {
    case SIGN_ALG_DES_MAC:
/* First part of signature covers plaintext packet header.                  */
        gss_DES_MAC((unsigned char *)zero_block,
                     checksum_iv,
                     8,
                     ctx_ptr,
                     loc_checksum_buffer);

        break;
    case SIGN_ALG_DES_MD5:
	gss_MD5Init(&context);
/* First part is over plaintext header					    */
        gss_MD5Update(&context,
                      checksum_iv,
                      8);
	gss_MD5Final(&context);
	gss_DES_MAC(zero_block,
		    context.digest,
		    16,
		    ctx_ptr,
		    loc_checksum_buffer);
	break;

    case SIGN_ALG_MD5:
        gss_fixup_md_init(ctx_ptr);
        gss_MD5Init(&context);

/* First part is key-dependent initializer                                  */
        gss_MD5Update(&context,
                      ctx_ptr->md_init,
                      ctx_ptr->md_init_length);

/* Next part is over plaintext header                                       */
        gss_MD5Update(&context,
                      checksum_iv,
                      8);

        gss_MD5Final(&context);
        memcpy(loc_checksum_buffer,
               context.digest,
               8);

        break;


    default:
        *minor_status = gssdce_s_unsupported_signature_algorithm;
        O_status = GSS_S_DEFECTIVE_TOKEN;
        goto gss_proc_token_return;
    };

    if (memcmp(loc_checksum_buffer,
               packet_checksum,
               sizeof(packet_checksum)) != 0) {
        *minor_status = gssdce_s_bad_signature;
        O_status = GSS_S_BAD_SIG;
	dce_svc_printf(GSS_S_PCT_INV_CKSUM_MSG);
        goto gss_proc_token_return;
    };

/* Now for the sequence number                                              */

    O_status = verify_received_sequence_number(minor_status,
                                               received_seq_no,
                                               ctx_ptr);
    tidy_ctx(ctx_ptr);
    destroy_ctx(ctx_ptr);


gss_proc_token_return:


    return O_status;
}


/****************************************************************************/
/*									    */
/*	                 gss_context_time routine			    */
/*									    */
/****************************************************************************/

OM_uint32 gss_context_time
                 (OM_uint32* minor_status,
                  gss_ctx_id_t context_handle,
                  OM_uint32* time_rec) {
    GSS_INIT;
    *minor_status = gssdce_s_operation_incomplete;

    if (time_rec != NULL) *time_rec = 0xfffffffful;

    *minor_status = 0;
    return GSS_S_COMPLETE;
}


/****************************************************************************/
/*									    */
/*	             gss_delete_sec_context routine			    */
/*									    */
/****************************************************************************/

OM_uint32 gss_delete_sec_context
                 (OM_uint32* minor_status,
                  gss_ctx_id_t* context_handle_ptr,
                  gss_buffer_t output_token) {

    ctx_info * ctx_ptr = NULL;
    cred_info * cred_ptr = NULL;
    OM_uint32 O_status;
    gss_ctx_id_t context_handle;

    *minor_status = gssdce_s_operation_incomplete;
    GSS_INIT;

    if (context_handle_ptr == NULL) {
	*minor_status = gssdce_s_invalid_handle;
	dce_svc_printf(GSS_S_DSC_INV_CTX_MSG);
	return GSS_S_NO_CONTEXT;
    };
    context_handle = *context_handle_ptr;

    if (output_token) {
	output_token->length = 0;
	output_token->value = NULL;
    };

    if ( ( ctx_ptr = resolve_ctx(context_handle) ) == NULL )
    {
	*minor_status = gssdce_s_invalid_handle;
	return GSS_S_NO_CONTEXT;
    };

    if ((ctx_ptr->state & CTX_M_ST_OPEN) == 0) {
	*minor_status = gssdce_s_invalid_context_state;
	O_status = GSS_S_FAILURE;
	goto rel_sec_ctx_return;
    };


    if (output_token) {

	if ((*minor_status = gss_build_prot_message(NULL,
						    ctx_ptr,
						    TOK_ID_DELETE,
						    DEFAULT_SIGN_ALGORITHM,
						    SEAL_ALG_NONE,
						    output_token)
	     ) != gssdce_s_success) {
	    O_status = GSS_S_FAILURE;
	    goto rel_sec_ctx_return;
	};
    };

    tidy_ctx(ctx_ptr);
    destroy_ctx(ctx_ptr);    
    ctx_ptr = NULL;

    O_status = GSS_S_COMPLETE;
    *minor_status = gssdce_s_success;


rel_sec_ctx_return:
    if (ctx_ptr != NULL) {
	UNLOCK_ELEMENT(ctx_ptr);
    };

    return GSS_S_COMPLETE;

}


/****************************************************************************/
/*									    */
/*			     gss_sign routine				    */
/*									    */
/****************************************************************************/

OM_uint32 gss_sign
                 (OM_uint32* minor_status,
                  gss_ctx_id_t context_handle,
                  int qop_req,
                  gss_buffer_t message_buffer,
                  gss_buffer_t token_buffer) {
    ctx_info * ctx_ptr = NULL;
    OM_uint32 O_status;
    unsigned char seq_buffer[8];
    size_t return_length;
    int sign_alg_req;


    *minor_status = gssdce_s_operation_incomplete;
    GSS_INIT;

    sign_alg_req = normalize_qop(qop_req);

    if ((message_buffer== NULL) || (token_buffer==NULL)) {
	*minor_status = gssdce_s_no_buffer;
	O_status = GSS_S_FAILURE;
	dce_svc_printf(GSS_S_SIGN_BAD_BUFFER_MSG);
	goto gss_sign_return;
    };
    if ((context_handle == GSS_C_NO_CONTEXT) || 
	((ctx_ptr = resolve_ctx(context_handle)) == NULL)) {
	*minor_status = gssdce_s_invalid_handle;
	O_status = GSS_S_NO_CONTEXT;
	dce_svc_printf(GSS_S_SIGN_INV_CTX_MSG);
	goto gss_sign_return;	
    };
    if ((ctx_ptr->state & CTX_M_ST_OPEN) == 0) {
	*minor_status = gssdce_s_invalid_context_state;
	O_status = GSS_S_FAILURE;
	dce_svc_printf(GSS_S_SIGN_INV_CTX_STATE_MSG);
	goto gss_sign_return;
    };

    if ((*minor_status = gss_build_prot_message(message_buffer,
					        ctx_ptr,
					        TOK_ID_SIGN,
					        sign_alg_req,
					        SEAL_ALG_NONE,
					        token_buffer)
	 ) != gssdce_s_success) {
	O_status = GSS_S_FAILURE;
	dce_svc_printf(GSS_S_SIGN_BPM_FAILED_MSG, *minor_status);
	goto gss_sign_return;
    };


    O_status = GSS_S_COMPLETE;
    *minor_status = gssdce_s_success;
  
gss_sign_return:
    if (ctx_ptr) {
	UNLOCK_ELEMENT(ctx_ptr);
    };
    return O_status;
}


/****************************************************************************/
/*									    */
/*			    gss_verify routine				    */
/*									    */
/****************************************************************************/

OM_uint32 gss_verify
                 (OM_uint32* minor_status,
                  gss_ctx_id_t context_handle,
                  gss_buffer_t message_buffer,
                  gss_buffer_t token_buffer,
                  int* qop_state) {
    ctx_info * ctx_ptr = NULL;
    OM_uint32 O_status;

    unsigned char packet_checksum[8];
    int tok_id_type;
    int sign_alg_type;
    int seal_alg_type;
    int data_offset;

    unsigned char loc_checksum_buffer[8];
    unsigned char checksum_iv[8];
    unsigned char cksum_temp[8];

    MD5_CTX context;
    unsigned char md_block[8];
        
    OM_uint32 received_seq_no;
    int seq_range;
    int i;

    GSS_INIT;

    if (qop_state) *qop_state = 0;

    if ((message_buffer== NULL) || (token_buffer ==NULL)) {
	*minor_status = gssdce_s_no_buffer;
	O_status = GSS_S_FAILURE;
	dce_svc_printf(GSS_S_VFY_BAD_BUFFER_MSG);
	goto gss_verify_return;
    };
    if ((context_handle == GSS_C_NO_CONTEXT) || 
	((ctx_ptr = resolve_ctx(context_handle)) == NULL)) {
	*minor_status = gssdce_s_invalid_handle;
	O_status = GSS_S_NO_CONTEXT;
	dce_svc_printf(GSS_S_VFY_INV_CTX_MSG);
	goto gss_verify_return;	
    };
    if ((ctx_ptr->state & CTX_M_ST_OPEN) == 0) {
	*minor_status = gssdce_s_invalid_context_state;
	O_status = GSS_S_FAILURE;
	dce_svc_printf(GSS_S_VFY_INV_CTX_STATE_MSG);
	goto gss_verify_return;
    };

    memset(packet_checksum, 0, sizeof(packet_checksum));

    if ((*minor_status = decode_message_header(token_buffer,
					       ctx_ptr,
					       &tok_id_type,
					       &sign_alg_type,
					       &seal_alg_type,
					       packet_checksum,
					       checksum_iv,
					       &received_seq_no,
					       &data_offset)
	) != gssdce_s_success) {
	O_status = GSS_S_DEFECTIVE_TOKEN;
	dce_svc_printf(GSS_S_VFY_DMH_FAILED_MSG, *minor_status);
	goto gss_verify_return;
    };

    if (tok_id_type != 1) {
	*minor_status = gssdce_s_unexpected_token_type;
	O_status = GSS_S_DEFECTIVE_TOKEN;
	dce_svc_printf(GSS_S_VFY_BAD_TOK_TYPE_MSG);
	goto gss_verify_return;
    };


    memset(loc_checksum_buffer, 0, sizeof(loc_checksum_buffer));
    
    switch (sign_alg_type) {
    case SIGN_ALG_DES_MAC:
/* First part of signature covers plaintext packet header.		    */
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_protect, svc_c_debug9, 
        "Verifying a DES-MAC signature"));

	gss_DES_MAC(zero_block,
		    checksum_iv,
		    8,
		    ctx_ptr,
		    cksum_temp);
/* Next part is over data field.					    */
	gss_DES_MAC(cksum_temp,		  /* IV			    */
		    (unsigned char *)message_buffer->value,
					      /* Data		    */
		    message_buffer->length,
					      /* Length		    */
		    ctx_ptr,		  /* key_info		    */
		    loc_checksum_buffer);
	break;

    case SIGN_ALG_MD5:
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_protect, svc_c_debug9, 
        "Verifying an MD5 signature"));

	gss_fixup_md_init(ctx_ptr);
	gss_MD5Init(&context);

/* First part is key-dependent initializer				    */

	gss_MD5Update(&context,
		      ctx_ptr->md_init,
		      ctx_ptr->md_init_length);

/* Next part is over plaintext header					    */
	gss_MD5Update(&context,
		      checksum_iv,
		      8);

/* Final part is over data						    */
	gss_MD5Update(&context,
		      (unsigned char *)message_buffer->value,
		      message_buffer->length);

	gss_MD5Final(&context);
	memcpy(loc_checksum_buffer,
	       context.digest,
	       8);

    	break;

    case SIGN_ALG_DES_MD5:
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_protect, svc_c_debug9, 
        "Verifying a DES(MD5) signature"));
	gss_MD5Init(&context);

/* First part is over plaintext header					    */
	gss_MD5Update(&context,
		      checksum_iv,
		      8);

/* Final part is over data						    */
	gss_MD5Update(&context,
		      (unsigned char *)message_buffer->value,
		      message_buffer->length);

	gss_MD5Final(&context);

	gss_DES_MAC(zero_block,			  /* IV			    */
		    context.digest,		  /* Data		    */
		    16,				  /* Length		    */
		    ctx_ptr,			  /* key_info		    */
		    loc_checksum_buffer);

    	break;


    default:
	*minor_status = gssdce_s_unsupported_signature_algorithm;
	O_status = GSS_S_DEFECTIVE_TOKEN;
	dce_svc_printf(GSS_S_VFY_SIGN_UNK_MSG);
	goto gss_verify_return;
    };

    if (qop_state) {
	if (sign_alg_type == SIGN_ALG_MD5) {
	    *qop_state = GSSDCE_C_QOP_MD5;
	} else if (sign_alg_type == SIGN_ALG_DES_MAC) {
	    *qop_state = GSSDCE_C_QOP_DES_MAC;
	} else if (sign_alg_type == SIGN_ALG_DES_MD5) {
	    *qop_state = GSSDCE_C_QOP_DES_MD5;
	};
    };

      
    if (memcmp(loc_checksum_buffer,
	       packet_checksum,
	       sizeof(packet_checksum)) != 0) {
	*minor_status = gssdce_s_bad_signature;
	O_status = GSS_S_BAD_SIG;
	dce_svc_printf(GSS_S_VFY_BAD_SIG_MSG);
	goto gss_verify_return;
    };


/* Now for the sequence number						    */

    O_status = verify_received_sequence_number(minor_status,
					       received_seq_no,
					       ctx_ptr);
  
gss_verify_return:
    if (ctx_ptr) {
	UNLOCK_ELEMENT(ctx_ptr);
    };
    return O_status;
}


/****************************************************************************/
/*									    */
/*			    gss_seal routine				    */
/*									    */
/****************************************************************************/

OM_uint32 gss_seal
                 (OM_uint32* minor_status,
                  gss_ctx_id_t context_handle,
                  int conf_req_flag,
                  int qop_req,
                  gss_buffer_t message_buffer,
                  int* conf_state,
                  gss_buffer_t token_buffer) {

    ctx_info * ctx_ptr = NULL;
    OM_uint32 O_status;
    unsigned char seq_buffer[8];
    size_t return_length;

    int tok_id_type;
    int seal_alg;
    int loc_conf_state;
    int sign_alg_req;
    
    *minor_status = gssdce_s_operation_incomplete;
    GSS_INIT;

    sign_alg_req = normalize_qop(qop_req);

    if ((message_buffer== NULL) || (token_buffer==NULL)) {
	*minor_status = gssdce_s_no_buffer;
	O_status = GSS_S_FAILURE;
	goto gss_seal_return;
    };
    if ((context_handle == GSS_C_NO_CONTEXT) || 
	((ctx_ptr = resolve_ctx(context_handle)) == NULL)) {
	*minor_status = gssdce_s_invalid_handle;
	O_status = GSS_S_NO_CONTEXT;
	goto gss_seal_return;	
    };
    if ((ctx_ptr->state & CTX_M_ST_OPEN) == 0) {
	*minor_status = gssdce_s_invalid_context_state;
	O_status = GSS_S_FAILURE;
	goto gss_seal_return;
    };

    if (conf_req_flag) {
#	ifdef NOENCRYPTION
#	    if defined(CONFIDENTIALITY_FAILOVER)
		tok_id_type = TOK_ID_SEAL;
		seal_alg = SEAL_ALG_NONE;
		loc_conf_state = 0;
#	    else
		*minor_status = gssdce_s_no_conf;
		O_status = GSS_S_FAILURE;
		goto gss_seal_return;	
#	    endif
#	else
	tok_id_type = TOK_ID_SEAL;
	seal_alg = SEAL_ALG_DES;
	loc_conf_state = 1;
#   endif
    } else {
	tok_id_type = TOK_ID_SEAL;
	seal_alg = SEAL_ALG_NONE;
	loc_conf_state = 0;
    };

    if ((*minor_status = gss_build_prot_message(message_buffer,
					        ctx_ptr,
					        tok_id_type,
					        sign_alg_req,
					        seal_alg,
					        token_buffer)
	 ) != gssdce_s_success) {
	O_status = GSS_S_FAILURE;
	goto gss_seal_return;
    };

    if (conf_state) *conf_state = loc_conf_state;

    O_status = GSS_S_COMPLETE;
    *minor_status = gssdce_s_success;
  
gss_seal_return:
    if (ctx_ptr) {
	UNLOCK_ELEMENT(ctx_ptr);
    };
    return O_status;

}


/****************************************************************************/
/*									    */
/*			    gss_unseal routine				    */
/*									    */
/****************************************************************************/

OM_uint32 gss_unseal
                 (OM_uint32* minor_status,
                  gss_ctx_id_t context_handle,
                  gss_buffer_t token_buffer,
                  gss_buffer_t message_buffer,
                  int* conf_state,
                  int* qop_state) {

    ctx_info * ctx_ptr = NULL;
    OM_uint32 O_status;

    unsigned char packet_checksum[8];
    int tok_id_type;
    int sign_alg_type;
    int seal_alg_type;
    int data_offset;
    int pad_byte;
    int i;
    error_status_t st;
    
    unsigned char confounder[8];
    
    unsigned char loc_checksum_buffer[8];
    unsigned char checksum_iv[8];
    unsigned char cksum_temp[8];

    MD5_CTX context;
    unsigned char md_block[8];
        
    OM_uint32 received_seq_no;
    int seq_range;


    GSS_INIT;

    if (conf_state) *conf_state = 0;
    if (qop_state) *qop_state = 0;

    if ((message_buffer== NULL) || (token_buffer ==NULL)) {
	*minor_status = gssdce_s_no_buffer;
	O_status = GSS_S_FAILURE;
	goto gss_unseal_return;
    };
    if ((context_handle == GSS_C_NO_CONTEXT) || 
	((ctx_ptr = resolve_ctx(context_handle)) == NULL)) {
	*minor_status = gssdce_s_invalid_handle;
	O_status = GSS_S_NO_CONTEXT;
	goto gss_unseal_return;
    };
    if ((ctx_ptr->state & CTX_M_ST_OPEN) == 0) {
	*minor_status = gssdce_s_invalid_context_state;
	O_status = GSS_S_FAILURE;
	goto gss_unseal_return;
    };


    memset(packet_checksum, 0, sizeof(packet_checksum));

    if ((*minor_status = decode_message_header(token_buffer,
					       ctx_ptr,
					       &tok_id_type,
					       &sign_alg_type,
					       &seal_alg_type,
					       packet_checksum,
					       checksum_iv,
					       &received_seq_no,
					       &data_offset)
	) != gssdce_s_success) {
	O_status = GSS_S_FAILURE;
	dce_svc_printf(GSS_S_UNS_DMH_FAILED_MSG, *minor_status);
	goto gss_unseal_return;
    };


#if defined(NOENCRYPTION)
    if ((tok_id_type == TOK_ID_SEAL) && 
	(seal_alg_type != SEAL_ALG_NONE)) {
	*minor_status = gssdce_s_no_conf;
	O_status = GSS_S_FAILURE;
	goto gss_unseal_return;
    };
#endif

    if (tok_id_type == TOK_ID_SEAL) {
	
	message_buffer->length = token_buffer->length - data_offset - 8;
/* This will malloc 1 to 8 bytes extra much space, but that's OK.	    */
	if ((message_buffer->value = (void *)malloc(message_buffer->length)
	    ) == NULL) {
	    message_buffer->length = 0;
	    *minor_status = gssdce_s_out_of_memory;
	    dce_svc_printf(GSS_S_UNS_NOMEM_MSG);
	    O_status = GSS_S_FAILURE;
	    free(message_buffer->value);
	    message_buffer->value = NULL;
	    message_buffer->length = 0;
	    goto gss_unseal_return;
	};
    };

#ifndef NOENCRYPTION
    if (seal_alg_type == SEAL_ALG_DES) {


        if (conf_state) *conf_state = 1;

/* Decrypt confounder, followed by data.				    */
	gss_decrypt_DES_CBC(
	    zero_block,		    /* IV				    */
	   (unsigned char *)token_buffer->value + data_offset,
				    /* Data				    */
	    8,
	    ctx_ptr,
	    PRIVACY_KEY,
	    confounder);

	DCE_SVC_DEBUG((gss__svc_handle, gss_s_protect, svc_c_debug9, 
        "Message length: %d including padding", message_buffer->length));
	    
	if ((st = gss_decrypt_DES_CBC(
	       (unsigned char *)token_buffer->value + data_offset,
					    /* IV			    */
	       (unsigned char *)token_buffer->value + data_offset + 8,
					    /* Data			    */
/*		message_buffer->length-8, */ /* Length			    */
		message_buffer->length,   /* Length			    */
		ctx_ptr,		    /* key			    */
		PRIVACY_KEY,
		(unsigned char *)message_buffer->value)
					    /* Output			    */
	    )!= rpc_s_ok) {
	    *minor_status = gssdce_s_couldnt_decrypt_token;
	    O_status = GSS_S_DEFECTIVE_TOKEN;
	    free(message_buffer->value);
	    message_buffer->value = NULL;
	    message_buffer->length = 0;
	    dce_svc_printf(GSS_S_UNS_DECR_FAILED_MSG, st);
	    goto gss_unseal_return;
	};


/*
#ifdef DEBUG_PROT
	    {
		int printed_chars = 0;
		char temp_ch;

		fprintf(stderr, "Decrypted data:  ");
		for (i = 0; i < message_buffer->length; i++) {
		    temp_ch = ((unsigned char *)message_buffer->value)[i];
		    if (isgraph(temp_ch))
			fprintf(stderr, "%c(%2.2X) ", temp_ch, temp_ch);
		    else fprintf(stderr, ".(%2.2X) ", temp_ch);
		    printed_chars++;
		    if (printed_chars >= 8) {
			fprintf(stderr, "\n          ");
			printed_chars = 0;
		    };
		};
		fprintf(stderr, "\n");
		fflush(stderr);
	    };
#	endif
*/

    } else
#endif
    if (seal_alg_type == SEAL_ALG_NONE) {
	memcpy(confounder,
	       (unsigned char *)token_buffer->value + data_offset,
	       8);

	memcpy((unsigned char *)message_buffer->value,
	       (unsigned char *)token_buffer->value + data_offset + 8,
	       message_buffer->length);
	       	       
    } else {
	*minor_status = gssdce_s_unexpected_token_type;
	O_status = GSS_S_DEFECTIVE_TOKEN;
	goto gss_unseal_return;
    };

/* Now we have the padded plaintext data in the message_buffer, and the	    */
/* confounder in 'confounder'.						    */
    
    memset(loc_checksum_buffer, 0, sizeof(loc_checksum_buffer));
    
    switch (sign_alg_type) {
    case SIGN_ALG_DES_MAC:
/* First part of signature covers plaintext packet header.		    */
	gss_DES_MAC(zero_block,
		    checksum_iv,
		    8,
		    ctx_ptr,
		    cksum_temp);
/* Then over confounder							    */
	gss_DES_MAC(cksum_temp,
		    confounder,
		    8,
		    ctx_ptr,
		    checksum_iv);

/* Next part is over data field.					    */
	gss_DES_MAC(checksum_iv,		  /* IV			    */
		    (unsigned char *)message_buffer->value,
					      /* Data		    */
		    message_buffer->length,
					      /* Length		    */
		    ctx_ptr,		  /* key_info		    */
		    loc_checksum_buffer);
	break;


    case SIGN_ALG_MD5:

	gss_fixup_md_init(ctx_ptr);
	gss_MD5Init(&context);

/* First part is key-dependent initializer				    */
	gss_MD5Update(&context,
		      ctx_ptr->md_init,
		      ctx_ptr->md_init_length);

/* Next part is over plaintext header					    */
	gss_MD5Update(&context,
		      checksum_iv,
		      8);

/* Next part is over confounder						    */
	gss_MD5Update(&context,
		      confounder,
		      8);

/* Final part is over padded data					    */
	gss_MD5Update(&context,
		      (unsigned char *)message_buffer->value,
		      message_buffer->length);

	gss_MD5Final(&context);
	memcpy(loc_checksum_buffer,
	       context.digest,
	       8);

    	break;

    case SIGN_ALG_DES_MD5:

	gss_MD5Init(&context);

/* First part is over plaintext header					    */
	gss_MD5Update(&context,
		      checksum_iv,
		      8);

/* Next part is over confounder						    */
	gss_MD5Update(&context,
		      confounder,
		      8);

/* Final part is over padded data					    */
	gss_MD5Update(&context,
		      (unsigned char *)message_buffer->value,
		      message_buffer->length);

	gss_MD5Final(&context);

	gss_DES_MAC(zero_block,
		    context.digest,
		    16,
		    ctx_ptr,
		    loc_checksum_buffer);
    	break;

    default:
	*minor_status = gssdce_s_unsupported_signature_algorithm;
	O_status = GSS_S_DEFECTIVE_TOKEN;
	free(message_buffer->value);
	message_buffer->value = NULL;
	message_buffer->length = 0;
	goto gss_unseal_return;
    };

    if (qop_state) {
	if (sign_alg_type == SIGN_ALG_MD5) {
	    *qop_state = GSSDCE_C_QOP_MD5;
	} else if (sign_alg_type == SIGN_ALG_DES_MAC) {
	    *qop_state = GSSDCE_C_QOP_DES_MAC;
	} else if (sign_alg_type == SIGN_ALG_DES_MD5) {
	    *qop_state = GSSDCE_C_QOP_DES_MD5;
	};
    };



    if (memcmp(loc_checksum_buffer,
	       packet_checksum,
	       sizeof(packet_checksum)) != 0) {
	*minor_status = gssdce_s_bad_signature;
	O_status = GSS_S_DEFECTIVE_TOKEN;
/*
#	if defined(DEBUG_PROT)
	{
	    int printed_chars;
	    char temp_ch;

	    fprintf(stderr, "Error - bad signature in gss_unseal\n");
	    fprintf(stderr, "   Expecting : ");
	    for (i=0; i<8; i++) {
		fprintf(stderr, "%2.2X", packet_checksum[i]);
	    };
	    fprintf(stderr, "\n   Calculated: ");
	    for (i=0; i<8; i++) {
		fprintf(stderr, "%2.2X", loc_checksum_buffer[i]);
	    };
	    fprintf(stderr, "\n");
	    fprintf(stderr, "   Data:  ");
	    printed_chars = 0;
	    for (i = 0; i < message_buffer->length; i++) {
		temp_ch = ((unsigned char *)message_buffer->value)[i];
		if (isgraph(temp_ch))
		    fprintf(stderr, "%c(%2.2X) ", temp_ch, temp_ch);
		else fprintf(stderr, ".(%2.2X) ", temp_ch);
		printed_chars++;
		if (printed_chars >= 8) {
		    fprintf(stderr, "\n          ");
		    printed_chars = 0;
		};
	    };
	    fprintf(stderr, "\n");
	    fflush(stderr);
	};
#	endif
*/
	free(message_buffer->value);
	message_buffer->value = NULL;
	message_buffer->length = 0;
	goto gss_unseal_return;
    };

/* Now to strip off trailing padding in the plaintext.			    */

    pad_byte = ((unsigned char *)message_buffer->value)
					[message_buffer->length-1];

    if (pad_byte <= 0 || pad_byte > 8) {
	*minor_status = gssdce_s_bad_plaintext_padding;
	O_status = GSS_S_DEFECTIVE_TOKEN;
	free(message_buffer->value);
	message_buffer->value = NULL;
	message_buffer->length = 0;
	dce_svc_printf(GSS_S_UNS_INV_PAD_BYTE_MSG, pad_byte);
	goto gss_unseal_return;
    };

		    
    for (i=1; i<pad_byte; i++) {
	if (pad_byte != ((unsigned char *)message_buffer->value)
					[message_buffer->length-i]) {
	    *minor_status = gssdce_s_bad_plaintext_padding;
	    O_status = GSS_S_DEFECTIVE_TOKEN;
	    free(message_buffer->value);
	    message_buffer->value = NULL;
	    message_buffer->length = 0;
	    dce_svc_printf(GSS_S_UNS_INV_PAD_CHECK_MSG);
	    goto gss_unseal_return;
	};
    };
    message_buffer->length -= pad_byte;

/* Now for the sequence number						    */

    O_status = verify_received_sequence_number(minor_status,
					       received_seq_no,
					       ctx_ptr);
  
gss_unseal_return:
    if (ctx_ptr) {
	UNLOCK_ELEMENT(ctx_ptr);
    };
    return O_status;

}


/****************************************************************************/
/*									    */
/*			gss_display_status routine			    */
/*									    */
/****************************************************************************/

OM_uint32 gss_display_status
                 (OM_uint32* minor_status,
                  OM_uint32 status_value,
                  int status_type,
                  gss_OID mech_type,
                  int* message_context,
                  gss_buffer_t status_string) {
    OM_uint32 O_status;
    unsigned char * error_text;
    error_status_t error_status;

    GSS_INIT;

    *minor_status = gssdce_s_operation_incomplete;
    status_string->value = NULL;
    status_string->length = 0;
    error_text = NULL;

/*
#ifdef GSS_SVC_CODE
    gss_svc_printf(svc_c_debug9,

    GSS_MSG("Entering GSS_DISPLAY_STATUS"));
#else
    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
    "Entering GSS_DISPLAY_STATUS"));
#endif

#ifdef GSS_SVC_CODE
    gss_svc_printf(svc_c_debug9,

    GSS_MSG("Gss_display_status: code=%X, type=%d"),
		   status_value, status_type);
#else
    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
    "GSS_DISPLAY_STATUS for code %x, type %d", status_value, status_type));
#endif
*/
    if (status_type == GSS_C_GSS_CODE) {


/*
#ifdef GSS_SVC_CODE
	gss_svc_printf(svc_c_debug9,

        GSS_MSG("Entering gss_display_status for GSS code %X"),
		       status_value);
#else
	DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
        "GSS code %x", status_value));
#endif
*/
	if (*message_context == 0)
	    *message_context = status_value;

	if (GSS_CALLING_ERROR(*message_context)) {

	    switch(GSS_CALLING_ERROR(*message_context)) {
	    case GSS_S_CALL_INACCESSIBLE_READ:
		O_status = gss_display_status(
				    minor_status,
				    gss_s_int_call_inaccessible_read,
				    GSS_C_MECH_CODE,
				    GSS_C_NULL_OID,
				    0,
				    status_string);
		break;
	    case GSS_S_CALL_INACCESSIBLE_WRITE:
		O_status = gss_display_status(
				    minor_status,
				    gss_s_int_call_inaccessible_write,
				    GSS_C_MECH_CODE,
				    GSS_C_NULL_OID,
				    0,
				    status_string);
		break;
	    case GSS_S_CALL_BAD_STRUCTURE:
		O_status = gss_display_status(
				    minor_status,
				    gss_s_int_call_bad_structure,
				    GSS_C_MECH_CODE,
				    GSS_C_NULL_OID,
				    0,
				    status_string);
		break;
	    default:

		DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug1, 
                "Unknown GSS calling error %x", status_value));
		O_status = GSS_S_BAD_STATUS;
		*minor_status = gssdce_s_bad_status_value;
		*message_context = 0;
		goto display_status_return;
	    };
	    *message_context ^= GSS_CALLING_ERROR(*message_context);
	} else if (GSS_ROUTINE_ERROR(*message_context)) {
	    switch (GSS_ROUTINE_ERROR(*message_context)) {
	    case GSS_S_BAD_MECH:
		O_status = gss_display_status(
				    minor_status,
				    gss_s_int_bad_mech,
				    GSS_C_MECH_CODE,
				    GSS_C_NULL_OID,
				    0,
				    status_string);
		break;
	    case GSS_S_BAD_NAME:
		O_status = gss_display_status(
				    minor_status,
				    gss_s_int_bad_name,
				    GSS_C_MECH_CODE,
				    GSS_C_NULL_OID,
				    0,
				    status_string);
		break;
	    case GSS_S_BAD_NAMETYPE:
		O_status = gss_display_status(
				    minor_status,
				    gss_s_int_bad_nametype,
				    GSS_C_MECH_CODE,
				    GSS_C_NULL_OID,
				    0,
				    status_string);
		break;
	    case GSS_S_BAD_BINDINGS:
		O_status = gss_display_status(
				    minor_status,
				    gss_s_int_bad_bindings,
				    GSS_C_MECH_CODE,
				    GSS_C_NULL_OID,
				    0,
				    status_string);
		break;
	    case GSS_S_BAD_STATUS:
		O_status = gss_display_status(
				    minor_status,
				    gss_s_int_bad_status,
				    GSS_C_MECH_CODE,
				    GSS_C_NULL_OID,
				    0,
				    status_string);
		break;
	    case GSS_S_BAD_SIG:
		O_status = gss_display_status(
				    minor_status,
				    gss_s_int_bad_sig,
				    GSS_C_MECH_CODE,
				    GSS_C_NULL_OID,
				    0,
				    status_string);
		break;
	    case GSS_S_NO_CRED:
		O_status = gss_display_status(
				    minor_status,
				    gss_s_int_no_cred,
				    GSS_C_MECH_CODE,
				    GSS_C_NULL_OID,
				    0,
				    status_string);
		break;
	    case GSS_S_NO_CONTEXT:
		O_status = gss_display_status(
				    minor_status,
				    gss_s_int_no_context,
				    GSS_C_MECH_CODE,
				    GSS_C_NULL_OID,
				    0,
				    status_string);
		break;
	    case GSS_S_DEFECTIVE_TOKEN:
		O_status = gss_display_status(
				    minor_status,
				    gss_s_int_defective_token,
				    GSS_C_MECH_CODE,
				    GSS_C_NULL_OID,
				    0,
				    status_string);
		break;
	    case GSS_S_DEFECTIVE_CREDENTIAL:
		O_status = gss_display_status(
				    minor_status,
				    gss_s_int_defective_credential,
				    GSS_C_MECH_CODE,
				    GSS_C_NULL_OID,
				    0,
				    status_string);
		break;
	    case GSS_S_CREDENTIALS_EXPIRED:
		O_status = gss_display_status(
				    minor_status,
				    gss_s_int_credentials_expired,
				    GSS_C_MECH_CODE,
				    GSS_C_NULL_OID,
				    0,
				    status_string);
		break;
	    case GSS_S_CONTEXT_EXPIRED:
		O_status = gss_display_status(
				    minor_status,
				    gss_s_int_context_expired,
				    GSS_C_MECH_CODE,
				    GSS_C_NULL_OID,
				    0,
				    status_string);
		break;
	    case GSS_S_FAILURE:
		O_status = gss_display_status(
				    minor_status,
				    gss_s_int_failure,
				    GSS_C_MECH_CODE,
				    GSS_C_NULL_OID,
				    0,
				    status_string);
		break;
	    default:	    

		DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug1, 
                "Unknown GSS routine error %x", status_value));
		O_status = GSS_S_BAD_STATUS;
		*minor_status = gssdce_s_bad_status_value;
		*message_context = 0;
		goto display_status_return;
	    };
	    *message_context ^= GSS_ROUTINE_ERROR(*message_context);
	} else if (GSS_SUPPLEMENTARY_INFO(*message_context)) {
	    if (*message_context & GSS_S_CONTINUE_NEEDED) {
		O_status = gss_display_status(
				    minor_status,
				    gss_s_int_continue_needed,
				    GSS_C_MECH_CODE,
				    GSS_C_NULL_OID,
				    0,
				    status_string);
		*message_context ^= GSS_S_CONTINUE_NEEDED;

	    } else if (*message_context & GSS_S_DUPLICATE_TOKEN) {
		O_status = gss_display_status(
				    minor_status,
				    gss_s_int_duplicate_token,
				    GSS_C_MECH_CODE,
				    GSS_C_NULL_OID,
				    0,
				    status_string);
		*message_context ^= GSS_S_DUPLICATE_TOKEN;

	    } else if (*message_context & GSS_S_OLD_TOKEN) {
		O_status = gss_display_status(
				    minor_status,
				    gss_s_int_old_token,
				    GSS_C_MECH_CODE,
				    GSS_C_NULL_OID,
				    0,
				    status_string);
		*message_context ^= GSS_S_OLD_TOKEN;

	    } else if (*message_context & GSS_S_UNSEQ_TOKEN) {
		O_status = gss_display_status(
				    minor_status,
				    gss_s_int_unseq_token,
				    GSS_C_MECH_CODE,
				    GSS_C_NULL_OID,
				    0,
				    status_string);
		*message_context ^= GSS_S_UNSEQ_TOKEN;

	    } else {

	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug1, 
            "Unknown GSS supplementary info %x", status_value));
		O_status = GSS_S_BAD_STATUS;
		*minor_status = gssdce_s_bad_status_value;
		*message_context = 0;
		goto display_status_return;
	    };
	} else {
		DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug1, 
                "Can't subdivide message value"));
	    O_status = GSS_S_BAD_STATUS;
	    *minor_status = gssdce_s_bad_status_value;
	    *message_context = 0;
	    goto display_status_return;
	};
    } else if (status_type == GSS_C_MECH_CODE) {

/*
#ifdef GSS_SVC_CODE
	gss_svc_printf(svc_c_debug9,

        GSS_MSG("Mechanism code %X"),
		       status_value);
#else
	    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug9, 
            "GSS code %x", status_value));
#endif
*/
	if ((mech_type == GSS_C_NULL_OID) || 
	    nm_oid_equal(mech_type, GSSDCE_C_OID_DCE_KRBV5_DES) ||
	    nm_oid_equal(mech_type, GSSDCE_C_OID_KRBV5_DES_ID) ||
	    nm_oid_equal(mech_type, GSSDCE_C_OID_KRBV5_DES_RFC)
	   ) {
	    if (message_context) *message_context = 0;

	    error_text = dce_msg_get_msg(status_value,
					 &error_status);

	    if (error_status != 0) {
/* dce_msg_get_msg failed.  This routine is supposed to check the default   */
/* message catalog if an external catalog can't resolve the message, but    */
/* this doesn't always seem to work, so explicitly lookup the default	    */
/* message.								    */


		DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug1, 
                "dce_msg_get_msg failed for code %x", status_value));

		error_text = dce_msg_get_default_msg(status_value,
						     &error_status);
		if (error_status) {

		    DCE_SVC_DEBUG((gss__svc_handle, gss_s_general, svc_c_debug1, 
                    "dce_msg_get_default_msg failed too", status_value));
		};
	    };

	    if (error_status != 0) {
		*minor_status = gssdce_s_bad_status_value;
		O_status = GSS_S_BAD_STATUS;
		goto display_status_return;
	    };

	    status_string->length = strlen((char *)error_text);
	    status_string->value = error_text;
	    error_text = NULL;
	    *minor_status = gssdce_s_success;		
	    O_status = GSS_S_COMPLETE;
	    goto display_status_return;    

	} else {
	    *minor_status = gssdce_s_unsupported_mechanism;
	    O_status = GSS_S_BAD_MECH;
	    goto display_status_return;
	};
    } else {
	    dce_svc_printf(GSS_S_DS_BAD_MECH_MSG);
	*minor_status = gssdce_s_bad_status_type;
	O_status = GSS_S_FAILURE;
	goto display_status_return;
    };
display_status_return:
    if (error_text) free(error_text);
    return O_status;
}


/****************************************************************************/
/*									    */
/*	                gss_indicate_mechs routine			    */
/*									    */
/****************************************************************************/

OM_uint32 gss_indicate_mechs
                 (OM_uint32* minor_status,
                  gss_OID_set* mech_set) {
    cred_info * cred;

    GSS_INIT;

    if (mech_set != NULL) {
	nm_oid_set_create_empty(mech_set);
	nm_oid_set_add_elem(mech_set, GSSDCE_C_OID_DCE_KRBV5_DES);
	nm_oid_set_add_elem(mech_set, GSSDCE_C_OID_KRBV5_DES);
    };
    *minor_status = gssdce_s_success;
    return GSS_S_COMPLETE;
}


/****************************************************************************/
/*									    */
/*			gss_compare_name routine			    */
/*									    */
/****************************************************************************/

OM_uint32 gss_compare_name
                 (OM_uint32* minor_status,
                  gss_name_t name1,
                  gss_name_t name2,
                  int* name_equal) {

    *minor_status = gssdce_s_operation_incomplete;
    GSS_INIT;

    *name_equal = (strcmp(name1, name2) == 0);
    *minor_status = gssdce_s_success;
    return GSS_S_COMPLETE;
}


/****************************************************************************/
/*									    */
/*			gss_display_name routine			    */
/*									    */
/****************************************************************************/

OM_uint32 gss_display_name
		 (OM_uint32* minor_status,
                  gss_name_t input_name,
                  gss_buffer_t output_name_buffer,
                  gss_OID* output_name_type) {
    sec_rgy_name_t string_name;
    error_status_t error_status;

    *minor_status = gssdce_s_operation_incomplete;
    GSS_INIT;

    irep_to_string(input_name,
		  string_name,
		  &error_status);
    if (error_status != error_status_ok) {
	*minor_status = gssdce_s_bad_name;
	return GSS_S_BAD_NAME;
    };

    if (output_name_type != NULL) {
/*	nm_oid_copy(GSSDCE_C_OID_DCENAME, output_name_type);		    */
/* We don't need to copy the OID, as our convenience routine to delete OIDs */
/* won't delete this one.						    */
	*output_name_type = GSSDCE_C_OID_DCENAME;
    };

    output_name_buffer->length = strlen((char *)string_name);

    if ((output_name_buffer->value = 
			(void *)malloc(output_name_buffer->length + 1))
	 == NULL) {
	output_name_buffer->length = 0;
	nm_oid_free(*output_name_type, 1);
	    dce_svc_printf(GSS_S_DISP_NOMEM_MSG);
	
	*minor_status = gssdce_s_out_of_memory;
	return GSS_S_FAILURE;
    };

    memcpy(output_name_buffer->value,
	   string_name,
	   output_name_buffer->length+1);

    *minor_status = gssdce_s_success;
    return GSS_S_COMPLETE;
}


/****************************************************************************/
/*									    */
/*			gss_import_name routine				    */
/*									    */
/****************************************************************************/

OM_uint32 gss_import_name
                 (OM_uint32* minor_status,
                  gss_buffer_t input_name_buffer,
                  gss_OID input_name_type,
                  gss_name_t* output_name) {
    sec_rgy_name_t string_name;
    error_status_t error_status;


    *minor_status = gssdce_s_operation_incomplete;
    GSS_INIT;

    if (input_name_type != NULL && !nm_oid_equal(input_name_type,
						 GSSDCE_C_OID_DCENAME)) {
	*minor_status = gssdce_s_unsupported_name_type;
	return GSS_S_BAD_NAMETYPE;
    };

    if (input_name_buffer->length >= sizeof(string_name)) {
	*minor_status = gssdce_s_bad_name;
	return GSS_S_BAD_NAME;
    };

    strncpy((char *)&string_name[0],
	    input_name_buffer->value,
	    input_name_buffer->length);
    memset(&string_name[0] + input_name_buffer->length,
	   0,
	   sizeof(string_name) - input_name_buffer->length);

    string_to_irep(string_name,
		  output_name,
		  &error_status);

    if (error_status != error_status_ok) {
	*minor_status = gssdce_s_bad_name;
	return GSS_S_BAD_NAME;
    };
    *minor_status = gssdce_s_success;
    return GSS_S_COMPLETE;
}


/****************************************************************************/
/*									    */
/*			gss_release_name routine			    */
/*									    */
/****************************************************************************/

OM_uint32 gss_release_name
                 (OM_uint32* minor_status,
                  gss_name_t* input_name) {
    *minor_status = gssdce_s_operation_incomplete;
    GSS_INIT;

    if (input_name == NULL) {
	    dce_svc_printf(GSS_S_RELN_NULL_NAME_MSG);
	*minor_status = gssdce_s_name_required;
	return GSS_S_BAD_NAME;
    };
    if (*input_name != NULL) {
	free(*input_name);
	*input_name = NULL;    
    };
    *minor_status = gssdce_s_success;
    return GSS_S_COMPLETE;
}


/****************************************************************************/
/*									    */
/*			gss_release_buffer routine			    */
/*									    */
/****************************************************************************/

OM_uint32 gss_release_buffer
                 (OM_uint32* minor_status,
                  gss_buffer_t buffer) {
    *minor_status = gssdce_s_success;
    GSS_INIT;

    if (buffer == NULL) return GSS_S_COMPLETE;
    if (buffer->value != NULL) free(buffer->value);
    buffer->length = 0;
    buffer->value = NULL;
    return GSS_S_COMPLETE;
}


/****************************************************************************/
/*									    */
/*			gss_release_oid_set routine			    */
/*									    */
/****************************************************************************/

OM_uint32 gss_release_oid_set
                 (OM_uint32* minor_status,
                  gss_OID_set* set) {
    *minor_status = gssdce_s_success;
    GSS_INIT;
    nm_oid_set_free(set);
    return GSS_S_COMPLETE;
}


/****************************************************************************/
/*									    */
/*			gss_inquire_cred routine			    */
/*									    */
/****************************************************************************/

OM_uint32 gss_inquire_cred (
                  OM_uint32 * minor_status,
                  gss_cred_id_t cred_handle,
                  gss_name_t * name,
                  OM_uint32 * lifetime,
                  int * cred_usage,
                  gss_OID_set * mechanisms) {
    cred_info * cred;
    error_status_t xerr;

    unsigned32 contextual_lifetime;
    sec_login_handle_t login_context;
    error_status_t error_status;
    sec_cred_pa_handle_t cred_pa_handle;
    sec_id_pa_t * sec_id_pas;
    sec_rgy_bind_auth_info_t binding_info;
    sec_rgy_name_t site_name;
    sec_rgy_handle_t rgy_context;
    int retry_count = 0;
    sec_rgy_name_t principal_name;
    sec_rgy_name_t principal_realm_name;
    sec_rgy_name_t principal_local_name;

    *minor_status = gssdce_s_operation_incomplete;
    GSS_INIT;

    site_name[0] = 0;

    if (cred_handle == GSS_C_NO_CREDENTIAL) {
        /* 
         * This is a request to use the default credential. The default credential
         * is a "virtual" credential, using the default login-context, and 
         * supporting the DCE mech-type for initiate-only use. 
         * Return the user the information he's asked for Possibilities are:
         *      name 
         *      lifetime
         *      cred_usage
         *      mechanisms 
         */

        if (name != NULL) {
            /*
             * To extract the client-name from the login context, we have to 
             * work hard. First get an explicit handle to the default 
             * login-context... 
             */
            sec_login_get_current_context(&login_context,
                                          &error_status);
            if (error_status != error_status_ok) {
                *minor_status = error_status;
                return GSS_S_NO_CRED;
            };

            /* 
             * We'll be pulling the client's UUID from the default-context's PAC.
             * To convert that to a name, we'll have to talk to the registry. 
             * We'll use the default-context to do that. Set up a binding_info 
             * structure for registry lookup... 
             */
            binding_info.info_type = sec_rgy_bind_auth_dce;
            binding_info.tagged_union.dce_info.authn_level =
                rpc_c_authn_level_default;
            binding_info.tagged_union.dce_info.authn_svc =
                rpc_c_authn_dce_secret;
            binding_info.tagged_union.dce_info.authz_svc = rpc_c_authz_dce;
            binding_info.tagged_union.dce_info.identity = login_context;

            /* Now bind to the registry... */
          retry:
            sec_rgy_site_bind(site_name,
                              &binding_info,
                              &rgy_context,
                              &error_status);
            
            if (error_status != error_status_ok) {
                if (retry_count < MAX_RGY_RETRIES) {
                    retry_count++;
                    site_name[0] = 0;
                    goto retry;
                };
                dce_svc_printf(GSS_S_FRC_RETRY_EXCEEDED_MSG, error_status);
                *minor_status = gssdce_s_no_rgy_available;
                sec_login_release_context(&login_context, &error_status);
                return GSS_S_FAILURE;
            };
            
            /* 
             * Now that we know we can speak to the registry, extract the 
             * initiator's pa-handle from the context...
             */
            cred_pa_handle = sec_login_cred_get_initiator(login_context,
                                                          &error_status);

            if (error_status != error_status_ok &&
                error_status != sec_login_s_not_certified) {
                *minor_status = error_status;
                sec_rgy_site_close(rgy_context, &error_status);
                sec_login_release_context(&login_context, &error_status);
                return GSS_S_FAILURE;
            };
            
            /* Then get down to the pa-data */
            sec_id_pas = sec_cred_get_pa_data(cred_pa_handle, &error_status);

            if (error_status != error_status_ok &&
                error_status != sec_login_s_not_certified) {
                *minor_status = error_status;
                sec_rgy_site_close(rgy_context, &error_status);
                sec_cred_free_pa_handle(&cred_pa_handle, &error_status);
                sec_login_release_context(&login_context, &error_status);
                return GSS_S_FAILURE;
            };

            /* 
             * Now we have the client UUID in sec_id_pas->principal.uuid. 
             * We have to convert this to a name for the caller. Ask the 
             * registry to translate it for us... 
             */
            sec_id_gen_name(rgy_context,
                            &sec_id_pas->realm.uuid,
                            &sec_id_pas->principal.uuid,
                            principal_name,
                            principal_realm_name,
                            principal_local_name,
                            &error_status);
            
            if (error_status != error_status_ok) {
                *minor_status = error_status;
                sec_rgy_site_close(rgy_context, &error_status);
                sec_cred_free_pa_handle(&cred_pa_handle, &error_status);
                sec_login_release_context(&login_context, &error_status);
                return GSS_S_FAILURE;
            };
            
            /* 
             * Now we have the name in principal_name. Copy it to the caller's
             * argument. First, though clean up the stuff we don't need anymore.
             */
            sec_rgy_site_close(rgy_context, &error_status);
            sec_cred_free_pa_handle(&cred_pa_handle, &error_status);
            sec_login_release_context(&login_context, &error_status);
            
            string_to_irep(principal_name,
                           name,
                           &error_status);
            if (error_status != error_status_ok) {
                *minor_status = error_status;
                return GSS_S_FAILURE;
            };
            
        };

        if (mechanisms != NULL) {
            /* The default cred is good for DCE only... */
            nm_oid_set_create_empty(mechanisms);
            nm_oid_set_add_elem(mechanisms, GSSDCE_C_OID_DCE_KRBV5_DES);
        };
        
        if (cred_usage != NULL) {
            /* It can only be used to initiate contexts... */
            *cred_usage = GSS_C_INITIATE;
        };

        if (lifetime != NULL) {
            sec_login_get_expiration(sec_login_default_handle,
                                     (signed32 *)&contextual_lifetime,
                                     &xerr);
            /* Why is lifetime in the above signed??? */
            if (xerr == sec_login_s_not_certified) {
                xerr = 0;
            };
            
            if (xerr) {
                *lifetime = 0;
                /* There's a problem with the user's login-context */
            } else {
                *lifetime = contextual_lifetime;
            };
        };

    } else {
        if ( (cred = resolve_cred(cred_handle) ) == NULL) {
#if defined(DEBUG)
            fprintf(stderr, 
                    "Gss_Inquire_cred - Error - resolve_cred returned NULL\n");
            fflush(stderr);
#endif
            *minor_status = gssdce_s_invalid_handle;
            return GSS_S_DEFECTIVE_CREDENTIAL;
        };

        /* We now have a locked pointer to the cred_info structure. */
        if (name != NULL) {
            if (!cred->principal_name_valid) {
                if ((*minor_status = cred_expand_id(cred)) != gssdce_s_success) {
#if defined(DEBUG)
                    fprintf(stderr, 
                            "Gss_Inquire_cred - Error - cred_expand_id failed\n");
                    fflush(stderr);
#endif
                    return GSS_S_FAILURE;
                };
            };
            string_to_irep(cred->principal_name,
                           name,
                           &xerr);
        };

        if (lifetime != NULL) {
            if (cred->login_context != NULL) {
                sec_login_get_expiration(cred->login_context,
                                         (signed32 *)&contextual_lifetime,
                                         &xerr);
                /* Why is lifetime in the above signed??? */
                if (xerr == sec_login_s_not_certified) {
                    xerr = 0;
                };
            
                if (xerr) {
                    *lifetime = 0;
                    /* There's a problem with the user's login-context */
                } else {
                    *lifetime = SMALLER(
                        utc_time_to_delta_seconds(&(cred->expiration_time)),
                    contextual_lifetime);
                };
            } else {
                *lifetime = utc_time_to_delta_seconds(&(cred->expiration_time));
            };
        };

        if (cred_usage != NULL) {
            if (cred->flags & (CRED_C_FL_INITIATE | CRED_C_FL_ACCEPT) == 
                (CRED_C_FL_INITIATE | CRED_C_FL_ACCEPT)) 
                *cred_usage = GSS_C_BOTH;
            else if (cred->flags & CRED_C_FL_INITIATE) *cred_usage = GSS_C_INITIATE;
            else if (cred->flags & CRED_C_FL_ACCEPT) *cred_usage = GSS_C_ACCEPT;
            else *cred_usage = 0;
        };
   
        if (mechanisms != NULL) {
            nm_oid_set_create_empty(mechanisms);
            if (cred->flags & CRED_C_FL_DCE1_MECH)
                nm_oid_set_add_elem(mechanisms, GSSDCE_C_OID_DCE_KRBV5_DES);
            if (cred->flags & CRED_C_FL_KRB5_MECH)
                nm_oid_set_add_elem(mechanisms, GSSDCE_C_OID_KRBV5_DES);
        };

        UNLOCK_ELEMENT(cred);
            
    };
  done:
    *minor_status = gssdce_s_success;
    return GSS_S_COMPLETE;
}



/****************************************************************************/
/*									    */
/*	         gssdce_extract_PAC_from_sec_context routine		    */
/*									    */
/****************************************************************************/

OM_uint32 gssdce_extract_PAC_from_sec_context
                (OM_uint32 * minor_status,
                 gss_ctx_id_t context_handle,
                 sec_id_pac_t * output_PAC) {

    ctx_info * ctx_ptr = NULL;
    OM_uint32 O_status;

    *minor_status = gssdce_s_operation_incomplete;

    GSS_INIT;

    if ((context_handle == GSS_C_NO_CONTEXT) || 
	((ctx_ptr = resolve_ctx(context_handle)) == NULL)) {

	dce_svc_printf(GSS_S_XPS_INV_CTX_MSG);

	*minor_status = gssdce_s_invalid_handle;
	O_status = GSS_S_NO_CONTEXT;
	goto gss_extract_PAC_from_sec_context_return;
    };
    

/* This routine is only applicable to a context acceptor's security	    */
/* context.								    */
    if (ctx_ptr->state != CTX_C_ST_OPEN_A) {
/* This context isn't an open acceptor context...			    */

	dce_svc_printf(GSS_S_XPS_INV_CTX_STATE_MSG);

	if (ctx_ptr->flags & CTX_M_ST_OPEN) {
	    *minor_status = gssdce_s_need_acceptor_context;
	} else {
	    *minor_status = gssdce_s_invalid_context_state;
	};
	O_status = GSS_S_NO_CONTEXT;
	goto gss_extract_PAC_from_sec_context_return;
    }; 

/* Now we have a valid, open, acceptor context.  Give the PAC to the	    */

    if (ctx_ptr->flags == CTX_C_FL_DCE1_MECH) {
/* The code below should be replaced so that it doesn't use the V1 PAC.	    */


	if ((*minor_status = copy_pac(output_PAC, &ctx_ptr->client_pac)
	    ) != rpc_s_ok) {
	    O_status = GSS_S_FAILURE;
	    goto gss_extract_PAC_from_sec_context_return;
	};
    } else {
/* We don't have a PAC.   Return an error.				    */

	dce_svc_printf(GSS_S_XPS_NO_PAC_MSG);

	O_status = GSS_S_FAILURE;
	*minor_status = gssdce_s_no_pac_available;
	goto gss_extract_PAC_from_sec_context_return;
    };

    O_status = GSS_S_COMPLETE;
    *minor_status = gssdce_s_success;
    
gss_extract_PAC_from_sec_context_return:
    if (ctx_ptr) {
	UNLOCK_ELEMENT(ctx_ptr);
    };
    return O_status;
}



/****************************************************************************/
/*									    */
/*	         gssdce_extract_creds_from_sec_context routine		    */
/*									    */
/****************************************************************************/

OM_uint32 gssdce_extract_creds_from_sec_context
                (OM_uint32 * minor_status,
                 gss_ctx_id_t context_handle,
                 rpc_authz_cred_handle_t * output_creds) {

    ctx_info * ctx_ptr = NULL;
    OM_uint32 O_status;

    *minor_status = gssdce_s_operation_incomplete;

    GSS_INIT;

    if ((context_handle == GSS_C_NO_CONTEXT) || 
	((ctx_ptr = resolve_ctx(context_handle)) == NULL)) {

	dce_svc_printf(GSS_S_XCC_INV_CTX_MSG);

	*minor_status = gssdce_s_invalid_handle;
	O_status = GSS_S_NO_CONTEXT;
	goto gss_extract_creds_from_sec_context_return;
    };
    
/* This routine is only applicable to a context acceptor's security	    */
/* context.								    */
    if (ctx_ptr->state != CTX_C_ST_OPEN_A) {
/* This context isn't an open acceptor context...			    */

	dce_svc_printf(GSS_S_XCC_INV_CTX_STATE_MSG);

	if (ctx_ptr->flags & CTX_M_ST_OPEN) {
	    *minor_status = gssdce_s_need_acceptor_context;
	} else {
	    *minor_status = gssdce_s_invalid_context_state;
	};
	O_status = GSS_S_NO_CONTEXT;
	goto gss_extract_creds_from_sec_context_return;
    }; 

/* Now we have a valid, open, acceptor context.  Give the PAC to the	    */

    if (ctx_ptr->flags == CTX_C_FL_DCE1_MECH) {
	if ((*minor_status = copy_creds(output_creds, 
					&ctx_ptr->client_creds)
	    ) != rpc_s_ok) {
	    O_status = GSS_S_FAILURE;
	    goto gss_extract_creds_from_sec_context_return;
	};
    } else {
/* We don't have ant creds.   Return an error.				    */

	dce_svc_printf(GSS_S_XCC_NO_CREDS_MSG);

	O_status = GSS_S_FAILURE;
	*minor_status = gssdce_s_no_pac_available;
	goto gss_extract_creds_from_sec_context_return;
    };

    O_status = GSS_S_COMPLETE;
    *minor_status = gssdce_s_success;
    
gss_extract_creds_from_sec_context_return:
    if (ctx_ptr) {
	UNLOCK_ELEMENT(ctx_ptr);
    };
    return O_status;
}



/****************************************************************************/
/*									    */
/*			    gssdce_release_oid				    */
/*									    */
/****************************************************************************/

OM_uint32 gssdce_release_oid
                (OM_uint32 * minor_status,
                 gss_OID *   oid) {
    *minor_status = gssdce_s_operation_incomplete;
    GSS_INIT;
    if ((oid == &GSSDCE_C_OID_KRBV5_DES) ||
	(oid == &GSSDCE_C_OID_DCE_KRBV5_DES) ||
	(oid == &GSSDCE_C_OID_KRBV5_DES_ID) ||
	(oid == &GSSDCE_C_OID_KRBV5_DES_RFC) ||
	(oid == &GSSDCE_C_OID_DCENAME)) {
	*minor_status = gssdce_s_success;
	return GSS_S_COMPLETE;
    };
    if (oid != NULL) {
	nm_oid_free(*oid, 1);
	*oid = NULL;
    };
    *minor_status = gssdce_s_success;
    return GSS_S_COMPLETE;
}



/****************************************************************************/
/*									    */
/*		      gssdce_set_cred_context_ownership			    */
/*									    */
/****************************************************************************/

OM_uint32 gssdce_set_cred_context_ownership(
    OM_uint32 *	    minor_status,
    gss_cred_id_t   cred_handle,
    int		    ownership) {

    cred_info * cred;
        
    *minor_status = gssdce_s_operation_incomplete;
    GSS_INIT;
    if ( (cred = resolve_cred(cred_handle) ) == NULL) {
	*minor_status = gssdce_s_invalid_handle;
	return GSS_S_DEFECTIVE_CREDENTIAL;
    };

    *minor_status = gssdce_s_success;
    if (ownership == GSSDCE_C_OWNERSHIP_GSSAPI) {
	set_cred_manage_context(cred);
    } else if (ownership == GSSDCE_C_OWNERSHIP_APPLICATION) {
	set_cred_unmanage_context(cred);
    } else {
	*minor_status = gssdce_s_invalid_ownership;
	UNLOCK_ELEMENT(cred);
	return GSS_S_FAILURE;
    };
    UNLOCK_ELEMENT(cred);
    return GSS_S_COMPLETE;
}


/****************************************************************************/
/*									    */
/*		      gssdce_register_acceptor_identity			    */
/*									    */
/****************************************************************************/


OM_uint32 gssdce_register_acceptor_identity(
		OM_uint32 *		    minor_status,
		gss_name_t                  server_principal_name,
		rpc_auth_key_retrieval_fn_t get_key_fn,
		void *			    arg) {

	error_status_t error;
	GSS_INIT;

	gss_server_register_auth_info((unsigned char *)server_principal_name,
				      rpc_c_authn_default,
				      get_key_fn,
				      arg,
				      &error);

	*minor_status = error;

	if (*minor_status) return GSS_S_FAILURE;
	return GSS_S_COMPLETE;
}


/****************************************************************************/
/*									    */
/*			    debugging_routines				    */
/*									    */
/****************************************************************************/

#ifndef DEBUG_ANY
static
#endif
char * ctx_state_str(ctx_info * ctx_ptr) {
    switch (ctx_ptr->state) {
    case CTX_C_ST_CLOSED: return "CTX_C_ST_CLOSED";
    case CTX_C_ST_INITIATING: return "CTX_C_ST_INITIATING";
    case CTX_C_ST_OPEN_I: return "CTX_C_ST_OPEN_I";
    case CTX_C_ST_OPEN_A: return "CTX_C_ST_OPEN_A";
    };
    return "CTX_C_ST_???? <Invalid state>";
}

#ifndef DEBUG_ANY
static
#endif
char * ctx_authn_str(ctx_info * ctx_ptr) {
    switch (ctx_ptr->authn_protocol) {
    case rpc_c_authn_dce_secret: return "rpc_c_authn_dce_secret";
    };
    return "rpc_c_authn_???? <Unexpected authn protocol>";
}


#ifndef DEBUG_ANY
static
#endif
char * ctx_authz_str(ctx_info * ctx_ptr) {
    switch (ctx_ptr->authz_protocol) {
    case rpc_c_authz_dce: return "rpc_c_authz_dce (PAC-based)";
    case rpc_c_authz_name: return "rpc_c_authz_name (Kerberos)";
    };
    return "rpc_c_authz_???? <Unexpected authz protocol>";
}

#if defined(DEBUG_ANY)

char * ctx_key_str(ctx_info * ctx_ptr) {
/* This routine isn't re-entrant. Debugging code mustn't activate it in	    */
/* simultaneous threads.						    */
    static char s[1024];
    static char b[1024];
    int i;
    s[0] = 0;
    if (ctx_ptr->auth_session_key == NULL) return "No key";
    if (ctx_ptr->auth_session_key->contents == NULL) 
				return "Invalid key (NULL contents)";

    for (i=0;i< ctx_ptr->auth_session_key->length;i++) {
	sprintf(b, "%2.2X", ctx_ptr->auth_session_key->contents[i]);
	strcat(s, b);
    };
    return s;
}


char * ctx_flag_str(ctx_info * ctx_ptr) {
/* This routine isn't re-entrant. Debugging code mustn't activate it in	    */
/* simultaneous threads.						    */
    static char s[1024];
    int d = 0;
    s[0] = 0;
    if (ctx_ptr->context_flags & GSS_C_DELEG_FLAG) {
	strcat(s, "DELEG"); d = 1;
    };
    if (ctx_ptr->context_flags & GSS_C_MUTUAL_FLAG) {
	if (d) strcat(s, " | ");
	strcat(s, "MUTUAL"); d = 1;
    };
    if (ctx_ptr->context_flags & GSS_C_REPLAY_FLAG) {
	if (d) strcat(s, " | ");
	strcat(s, "REPLAY"); d = 1;
    };
    if (ctx_ptr->context_flags & GSS_C_SEQUENCE_FLAG) {
	if (d) strcat(s, " | ");
	strcat(s, "SEQUENCE"); d = 1;
    };
    if (ctx_ptr->context_flags & GSS_C_CONF_FLAG) {
	if (d) strcat(s, " | ");
	strcat(s, "CONF"); d = 1;
    };
    if (ctx_ptr->context_flags & GSS_C_INTEG_FLAG) {
	if (d) strcat(s, " | ");
	strcat(s, "INTEG"); d = 1;
    };
    if (!d) strcpy(s, "<None>");
    return s;
}


char * ctx_client_str(ctx_info * ctx_ptr) {
    if (ctx_ptr->client_name == NULL) return "<None>";
    return (char *)ctx_ptr->client_name;
}

char * ctx_ticketor_str(ctx_info * ctx_ptr) {
    if (ctx_ptr->ticketted_name == NULL) return "<None>";
    return (char *)ctx_ptr->ticketted_name;
}

char * ctx_server_str(ctx_info * ctx_ptr) {
/* This routine isn't re-entrant. Debugging code mustn't activate it in	    */
/* simultaneous threads.						    */
    static char s[1024];
    unsigned char * sp = NULL;
    if (ctx_ptr->server_name == NULL) return "<None>";
    sec_krb_unparse_name(ctx_ptr->server_name, &sp);
    strncpy(s, (char *)sp, 1024);
    free(sp);
    return s;    
}


char * dump_context(gss_ctx_id_t ctx_handle) {
/* This routine isn't re-entrant. Debugging code mustn't activate it in	    */
/* simultaneous threads.						    */
    ctx_info * ctx_ptr;
    static char string[1024];
    if (ctx_handle == GSS_C_NO_CONTEXT) {
	sprintf(string, "GSS_C_NO_CONTEXT\n");
    } else if ((ctx_ptr = resolve_ctx(ctx_handle)) == NULL) {
	sprintf(string, "Invalid handle (%lu)\n", ctx_handle);
    } else {
	sprintf(string, 
"Handle: %lu\n\
State: %s\n\
AuthnPcol: %s\n\
AuthzPcol: %s\n\
SndSeq: %lu\n\
RcvSeq: %lu\n\
Deleg: %u\n\
LocCred: %lu\n\
DelegCred: %lu\n\
SessionKey: %s\n\
Expiration: %lu\n\
Flags: %s\n\
Client: %s\n\
Ticketor: %s\n\
Server: %s\n",

	    ctx_handle,
	    ctx_state_str(ctx_ptr),
	    ctx_authn_str(ctx_ptr),
	    ctx_authz_str(ctx_ptr),
	    ctx_ptr->snd_seq,
	    ctx_ptr->rcv_seq,
	    ctx_ptr->delegated,
	    ctx_ptr->local_cred,
	    ctx_ptr->deleg_cred,
	    ctx_key_str(ctx_ptr),
	    ctx_ptr->expiration,
	    ctx_flag_str(ctx_ptr),
	    ctx_client_str(ctx_ptr),
	    ctx_ticketor_str(ctx_ptr),
	    ctx_server_str(ctx_ptr));


	UNLOCK_ELEMENT(ctx_ptr);
    };
    return string;
}

char * cred_flags_str(cred_info * cred_ptr) {
/* This routine isn't re-entrant. Debugging code mustn't activate it in	    */
/* simultaneous threads.						    */
    static char s[1024];
    int d = 0;
    s[0] = 0;

    if (cred_ptr->flags & CRED_C_FL_INITIATE) {
	strcat(s, "INITIATE"); d = 1;
    };
    if (cred_ptr->flags & CRED_C_FL_ACCEPT) {
	if (d) strcat(s, " | ");
	strcat(s, "ACCEPT"); d = 1;
    };
    if (cred_ptr->flags & CRED_C_FL_DCE1_MECH) {
	if (d) strcat(s, " | ");
	strcat(s, "DCE"); d = 1;
    };
    if (cred_ptr->flags & CRED_C_FL_KRB5_MECH) {
	if (d) strcat(s, " | ");
	strcat(s, "Kerberos"); d = 1;
    };
    if (!d) strcpy(s, "<None>");

    return s;
}



char * dump_cred(gss_cred_id_t cred_handle) {
/* This routine isn't re-entrant. Debugging code mustn't activate it in	    */
/* simultaneous threads.						    */
    cred_info * cred_ptr;
    static char string[1024];

    if (cred_handle == GSS_C_NO_CREDENTIAL) {
	sprintf(string, "GSS_C_NO_CREDENTIAL\n");
    } else if ((cred_ptr = resolve_cred(cred_handle)) == NULL) {
	sprintf(string, "Invalid handle (%lu)\n", cred_handle);
    } else {

	sprintf(string, 
"Handle: %lu\n\
Flags: %s\n\
PrincipalNameValid: %u\n\
PrincipalName: '%s'\n\
PrincipalRealm: '%s'\n\
PrincipalLocalName: '%s'\n\
LoginContext: %lu\n",
	    cred_handle,
	    cred_flags_str(cred_ptr),
	    cred_ptr->principal_name_valid,
	    cred_ptr->principal_name,
	    cred_ptr->principal_realm_name,
	    cred_ptr->principal_local_name,
	    cred_ptr->login_context);

	UNLOCK_ELEMENT(cred_ptr);
    };
    return string;
}



#endif
/*  DEC/CMS REPLACEMENT HISTORY, Element GSSAPI.C */
/*  *29   27-SEP-1994 10:49:35 WRAY "" */
/*  *28   16-SEP-1994 12:22:37 WRAY "Fixes for mem-leaks & unmatched frees" */
/*   19A3 25-AUG-1994 15:55:52 WRAY "Fix test_set_member bug" */
/*  *27   25-AUG-1994 15:54:13 WRAY "Fix test_set_member bug" */
/*  *26   25-AUG-1994 10:49:29 WRAY "" */
/*   19A2 11-AUG-1994 15:19:15 WRAY "Fix constant names for new protocol" */
/*   19A1  9-AUG-1994 11:55:54 WRAY "Support new MIT protocol" */
/*  *25    4-AUG-1994 14:48:31 WRAY "OT 10895 workaround" */
/*  *24    8-JUL-1994 15:05:30 WRAY "GSSAPI BL3" */
/*  *23   22-MAR-1994 09:32:00 WRAY "" */
/*  *22   18-MAR-1994 15:44:47 WRAY "Fix messaging bug" */
/*  *21   18-MAR-1994 13:53:35 WRAY "For BL2" */
/*  *20   11-MAR-1994 14:35:33 WRAY "OSF BL1 version" */
/*  *19    8-MAR-1994 15:12:00 WRAY "" */
/*  *18    8-MAR-1994 09:40:15 WRAY "" */
/*  *17   25-FEB-1994 11:46:52 WRAY "" */
/*  *16   18-FEB-1994 17:05:16 WRAY "" */
/*  *15   18-FEB-1994 14:30:55 WRAY "" */
/*  *14   18-FEB-1994 09:47:49 WRAY "Add mutex for krb funcs" */
/*  *13    9-FEB-1994 13:06:06 WRAY "" */
/*  *12   20-JAN-1994 12:22:58 WRAY "" */
/*  *11   17-JAN-1994 12:34:50 WRAY "Update protocol to V1.1" */
/*  *10    3-DEC-1993 09:10:02 WRAY "Added trace points" */
/*  *9    18-NOV-1993 15:29:53 WRAY "" */
/*  *8    18-NOV-1993 14:41:23 WRAY "With tracing" */
/*  *7    11-NOV-1993 14:21:35 WRAY "Fix Krb/DCE parameterization" */
/*  *6     9-NOV-1993 15:28:01 WRAY "Fix misc bugs" */
/*  *5    22-SEP-1993 16:42:26 WRAY "Incorporate in ODE" */
/*  *4    19-AUG-1993 14:34:05 WRAY "Bugfix re memcpy of token header" */
/*  *3    19-AUG-1993 08:31:36 WRAY "Fix registry lookup bug" */
/*  *2     2-MAR-1993 11:37:46 WRAY "Added copyright notice" */
/*  *1     1-MAR-1993 10:33:44 WRAY "High-level code for DCE/GSSAPI" */
/*  DEC/CMS REPLACEMENT HISTORY, Element GSSAPI.C */
