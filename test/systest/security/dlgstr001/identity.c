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
 * $Log: identity.c,v $
 * Revision 1.1.4.2  1996/02/17  23:28:28  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:45:27  marty]
 *
 * Revision 1.1.4.1  1995/12/11  22:45:33  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:21:38  root]
 * 
 * Revision 1.1.2.3  1994/10/06  20:42:41  agd
 * 	expand copyright
 * 	[1994/10/06  14:31:43  agd]
 * 
 * Revision 1.1.2.2  1994/09/27  21:28:25  gmd
 * 	The establish_identity() routine had an #ifdef/#else for PROTOTYPES
 * 	with mismatched arguments. The #ifdef is not necessary - removed.
 * 	[1994/09/27  21:27:21  gmd]
 * 
 * Revision 1.1.2.1  1994/09/19  18:17:24  bhaim
 * 	multidelegate test
 * 	[1994/09/19  17:50:48  bhaim]
 * 
 * $EndLog$
 */
#include <pthread.h>
#include <dce/sec_login.h>
#include <dce/keymgmt.h>
#include <dce/rpcbase.h>
#include <dce/dce_cf.h>
#include <dce/dce_error.h>		/* DCE error facility */

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "identity.h"


/*
**********************************************************************
**
**  F O R W A R D   D E C L A R A T I O N S
**
**********************************************************************
*/
#ifdef PROTOTYPES
static void setup_refresh
    (
	/* [in,out] */ identity_info_p_t    identity_info_p
    );
static pthread_addr_t refresh_identity
    (
	/* [in]  */ pthread_addr_t arg
    );
static void setup_key_mgmt
    (
	/* [in,out] */ identity_info_p_t  identity_info_p
    );
static pthread_addr_t manage_key
    (
	/* [in]  */ pthread_addr_t arg
    );
#else
static void setup_refresh();
static pthread_addr_t refresh_identity();
static void setup_key_mgmt();
static pthread_addr_t manage_key();
#endif


/*
**********************************************************************
**
**  E S T A B L I S H _  I D E N T I T Y
**
** This routine establishes a the identity of the application server.
**
**********************************************************************
*/
void establish_identity
(
    /* [in]  */	unsigned32	authn_svc,
    /* [in]  */ unsigned_char_t    *principal_name,
    /* [in]  */ unsigned_char_t    *keytab_file_name,
    /* [out] */ identity_info_t    **identity_info
)
{
    void                  *key_data;
    boolean32             reset_pw;
    sec_login_auth_src_t  auth_src;
    unsigned_char_t       tmp_keytab_file[256];
    unsigned char         access_ok;
    identity_info_p_t     identity_info_p;
    error_status_t        status;

    identity_info_p =
	(identity_info_p_t) malloc(sizeof(identity_info_t));
    if (identity_info_p == NULL) {
	fprintf (stderr,
	    "unable to malloc memory for identity_info_p\n"
	);
	fprintf (stderr,
	    "Exiting ...\n"
	);
	exit (1);
    }

    identity_info_p->principal =
	(unsigned_char_p_t) malloc(strlen((char *)principal_name)+1);
    if (identity_info_p->principal == NULL) {
	fprintf (stderr,
	    "unable to malloc memory for identity_info_p->principal\n"
	);
	fprintf (stderr,
	    "Exiting ...\n"
	);
	exit (1);
    }

    strcpy (
	(char *)(identity_info_p->principal),
	(char *)principal_name
    );


    /*
    ** If the keytab_file_name that was passed in is not NULL and
    ** it is the empty string, then the caller wants us to build
    **  a keytab file name for them.
    */
    if (
          (keytab_file_name != NULL)
	  &&
	  (strlen((char *)keytab_file_name) == 0)
       )
    {

	char *cell_name;

	dce_cf_get_cell_name (
	    &cell_name,
	    &status
	);
	if (status == dce_cf_st_ok) {
	    char *c_ptr;

	    /*
	    ** Get the last component in the pathname of the cell
	    ** name
	    */
	    c_ptr = cell_name + strlen(cell_name);
	    while ( (c_ptr > cell_name) && (*(c_ptr-1) != '/') ) {
		c_ptr--;
	    }
	    strcpy ((char *)tmp_keytab_file, "./keytab.");
	    strcat ((char *)tmp_keytab_file, c_ptr);
	    free (cell_name);
	}
	else {
	    strcpy ((char *)tmp_keytab_file, "./keytab");
	    print_dce_error (
		status,
		"dce_cf_get_cell_name() failed with error:"
	    );
	}

	keytab_file_name = tmp_keytab_file;

    } /* end if (strlen(keytab_file_name) == 0) */


    if (keytab_file_name != NULL) {
	if (access ((char *)keytab_file_name, R_OK) == 0) {
	    access_ok = true;
	}
	else {
	    access_ok = false;
	}
    } /* end if (keytab_file_name != NULL) */

    /*
    ** Assume access is OK, though this may not be the case.  We'll
    ** find out later when we actually try to setup our identity.
    */
    else {
	access_ok = true;
    }

    if (access_ok) {

	sec_login_setup_identity (
	    principal_name,
	    sec_login_no_flags,
	    &(identity_info_p->login_context),
	    &status
	);
	if (status != error_status_ok) {
	    print_dce_error (
		status,
		"sec_login_setup_identity() failed with error:"
	    );
	    fprintf (stderr,
		"Exiting ...\n"
	    );
	    exit (1);
	} /* end if (status != rpc_s_ok) */

	sec_key_mgmt_get_key (
	    rpc_c_authn_dce_secret,   /* authentication service to use */
	    (void *)keytab_file_name, /* keytab file to use          */
	    (idl_char *)principal_name,
	    0,                        /* get the most recent key     */
	    &key_data,
	    &status
	);
	if (status != rpc_s_ok) {
	    print_dce_error (
		status,
		"sec_key_mgmt_get_key() returned with error:"
	    );
	    fprintf (stderr,
		"Exiting ...\n"
	    );
	    exit (1);
	} /* end if (status != rpc_s_ok) */

	sec_login_validate_identity (
	    identity_info_p->login_context,
	    (sec_passwd_rec_t *)key_data,
	    &reset_pw,                /* has the passwd expired? */
	    &auth_src,                /* what was the source of the auth */
	    &status
	);
	if (status == rpc_s_ok) {

	    sec_login_certify_identity (
		identity_info_p->login_context,
		&status
	    );
	    if (status != error_status_ok) {
		print_dce_error (
		    status,
		    "sec_login_certify_identity() returned with error:"
		);
		fprintf (stderr,
		    "Exiting ...\n"
		);
		exit (1);
	    }
	}
	else {
	    print_dce_error (
		status,
		"sec_login_validate_identity() returned with error:"
	    );
	    fprintf (stderr,
		"Exiting ...\n"
	    );
	    exit (1);
	} /* end else (st != rpc_s_ok) */
/* ???? */
	sec_login_set_context (
	    identity_info_p->login_context,
	    &status
	);

	if (status != rpc_s_ok) {
	    print_dce_error (
		status,
		"sec_login_set_context returned with error:"
	    );

	    fprintf (stderr,
		"Exiting ...\n"
	    );
	    exit (1);
	} /* end if (status != rpc_s_ok) */


	if (keytab_file_name == NULL) {
	    identity_info_p->keytab_file = NULL;
	}
	else {
	    identity_info_p->keytab_file =
		(unsigned_char_p_t) malloc(strlen((char *)keytab_file_name)+1);
	    if (identity_info_p->keytab_file == NULL) {
		fprintf (stderr,
		"Unable to malloc memory for identity_info_p->keytab_file\n"
		);
		fprintf (stderr,
		    "Exiting ...\n"
		);
		exit (1);
	    }
	    strcpy (
		(char *)(identity_info_p->keytab_file),
		(char *)keytab_file_name
	    );
	}
	rpc_server_register_auth_info(
		(unsigned char *)principal_name,
		authn_svc,
		NULL,
		(void *) (keytab_file_name),
	      &status);

	/*
	** Now setup a thread that will keep our new found identity from
	** expiring.
	*/
	setup_refresh (
	    identity_info_p
	);

	setup_key_mgmt (
	    identity_info_p
	);


    } /* if (access_ok) */

    else {
       fprintf (stderr,
	   "Warning: %s \"%s\".\n",
	   "No read access to \"%s\" security key file\n",
	   (keytab_file_name == NULL) ?
	       "system default" : (char *)keytab_file_name
       );
       fprintf (stderr,
	   "         %s\n",
	   "Not establishing unique identity -- using current identity."
       );
       fprintf (stderr,
	   "         %s\n",
	   "Not starting identity refresh thread."
       );

       sec_login_get_current_context (
	   &(identity_info_p->login_context),
	   &status
       );
	if (status != rpc_s_ok) {
	    print_dce_error (
		status,
		"Unable to retrieve current DCE identity:"
	    );
	    fprintf (stderr,
		"Exiting ...\n"
	    );
	   exit (1);
	} /* end if (status != rpc_s_ok) */

    } /* end else no read access to keytab file */

    if (identity_info != NULL) {
	*identity_info = identity_info_p;
    }

    return;

} /* end establish_identity() */



/*
**********************************************************************
**
** S E T U P _ R E F R E S H _ I D E N T I T Y
**
**********************************************************************
*/
static void setup_refresh
#ifdef PROTOTYPES
(
    /* [in,out] */ identity_info_p_t  identity_info_p
)
#else
(identity_info_p)
    /* [in,out] */ identity_info_p_t  identity_info_p;
#endif
{
    int            thread_status;

    thread_status = pthread_create (
	&(identity_info_p->refresh_thread),    /* store thread id to detach */
	pthread_attr_default,  /* use default thread attributes */
	refresh_identity,      /* routine where thread should begin execution*/
	(pthread_addr_t)identity_info_p  /* argument to the routine */
    );

    if (thread_status != 0) {
	fprintf(stderr,
	    "%s: errno = %d\n%s\n",
	    "pthread_create() for refresh failed",
	    errno,
	    "... continuing"
	);
	return;
    }

    /*
    ** We're not interested in a return value from refresh_thread,
    ** so allow the runtime to reclaim storage as soon as the
    ** refresh thread finishes.
    */
#ifndef PTHREAD_JOIN
    thread_status = pthread_detach(&(identity_info_p->refresh_thread));

    if (thread_status != 0) {
	fprintf(stderr,
	    "%s: errno = %d\n%s\n",
	    "pthread_detach() for refresh thread failed",
	    errno,
	    "... continuing"
	);
    }

#endif /* ! PTHREAD_JOIN */

    /*
    ** Give the thread a chance to get set up.  We'll get the CPU
    ** back soon enough (like almost right away!) since the refresh
    ** thread is just going to go to sleep until refresh time.
    */
    pthread_yield ();


} /* setup_refresh() */



/*
**********************************************************************
**
** R E F R E S H _ I D E N T I T Y
**
**********************************************************************
*/
static pthread_addr_t refresh_identity
#ifdef PROTOTYPES
(
    pthread_addr_t arg
)
#else
(arg)
    pthread_addr_t arg;
#endif
{
    identity_info_p_t  identity_info_p = (identity_info_p_t)arg;
    void *key_data;
    boolean32 reset_pw;
    sec_login_auth_src_t auth_src;
    struct timezone   tz;
    struct timeval    now;
    struct timespec   nap_duration;
    signed32          expiration;
    signed32          remaining_time;
    error_status_t    status;
    int               count = 0;   /* prevent infinite looping */

    while (count < REFRESH_MAX_RETRIES) {

	/*
	** Get the time of day right now.  It is expressed as an 
	** absolute time in typical UN*X fashion (i.e seconds since
	** beginning of epoch) relative to UTC (aka GMT).
	*/
	if (gettimeofday (&now, &tz) < 0 ) {
	    fprintf (stderr,
		"gettimeofday() in refresh_thread() failed\n"
	    );
	    count++;
	    continue;
	}

	/*
	** The following information is empirically derived -- i.e.
	** it is not in the man pages and this is what I could
	** figure out.
	**
	** Expiration is the absolute time at which the login_context
	** will expire.  In typical UN*X fashion it is returned as the
	** number of seconds since the epoch relative to the UTC (aka GMT)
	** timezone.
	*/
	if (count < 1) {
	    sec_login_get_expiration (
		identity_info_p->login_context,
		&expiration,
		&status
	    );
	}
	else {
	    expiration = now.tv_sec;
	    status = error_status_ok;
	}

	if (
	      (status == error_status_ok)
	      ||
	      (status == sec_login_s_not_certified)
	   )
	{
	    /*
	    ** Subtract the current time from the expiration to
	    ** find out how long we've got until the login context
	    ** expires.
	    */

	    remaining_time = expiration - now.tv_sec;

	    if (remaining_time > 0) {

#ifdef FAIL_REFRESH
		remaining_time = 30;
#endif

		/*
		** We want to sleep for some amount of time and then
		** wake up to refresh our identity.  Allow ourselves
		** some fudge factor so we wake up in plenty of time
		** to accomplish the refresh -- you know, clock skew,
		** network delays, etc.  Shave off 10 minutes (600 
		** seconds) or one half the reamining time, which ever
		** is less from the nap_duration.
		*/
		nap_duration.tv_sec = remaining_time
				      -
				      MIN (600,(remaining_time/2))
				      ;
		nap_duration.tv_nsec = 0;
		now.tv_sec += nap_duration.tv_sec;

		/*
		** Now go to sleep -- sweet dreams.
		** If we actually go to sleep we can reset our loop
		** count to zero.
		*/
		if (pthread_delay_np(&nap_duration) < 0) {
		    fprintf (stderr,
		       "pthread_delay_np in refresh_identity() failed\n"
		    );

		}
		else {
		   count = 0;
		}

	    } /* end if (remaining_time > 0) */

	    if (gettimeofday (&now, &tz) < 0 ) {
		fprintf (stderr,
		    "gettimeofday() in refresh_thread() failed\n"
		);
		count++;
		continue;
	    }

	    /*
	    ** OK, so we've woken up from our nap and it is time to
	    ** refresh our identity.  Set the nap_duration to zero.
	    ** If anything fails while trying to refresh, set
	    ** nap_duration to > 0.
	    */
	    nap_duration.tv_sec = 0;
	    nap_duration.tv_nsec = 0;

#ifdef FAIL_REFRESH
	    sec_login_refresh_identity (
		&(identity_info_p->login_context),
		&status
	    );
#else
	    sec_login_refresh_identity (
		identity_info_p->login_context,
		&status
	    );
#endif

	    if (status == error_status_ok) {

		sec_key_mgmt_get_key (
		    rpc_c_authn_dce_secret,   /* use shared secret keys  */
		    (void *)(identity_info_p->keytab_file),
		    identity_info_p->principal,
		    0,                        /* get the most recent key */
		    &key_data,
		    &status
		);

		if (status != rpc_s_ok) {
			print_dce_error (
			    status,
			    "sec_key_mgmt_get_key error during refresh:"
			);

		    nap_duration.tv_sec = REFRESH_RETRY_SNOOZE_TIME;

		} /* end if (status != rpc_s_ok) */

		sec_login_validate_identity (
		    identity_info_p->login_context,
		    (sec_passwd_rec_t *)key_data,
		    &reset_pw,  /* has the passwd expired? */
		    &auth_src,  /* what was the source of the auth */
		    &status
		);

		if (status == rpc_s_ok) {

		    sec_login_certify_identity (
			identity_info_p->login_context,
			&status
		    );

		    if (status != error_status_ok) {
			print_dce_error (
			    status,
			"sec_login_certify_identity() returned with error:"
			);

			nap_duration.tv_sec = REFRESH_RETRY_SNOOZE_TIME;
		    }
		}
		else {
		    print_dce_error (
			status,
		    "sec_login_validate_identity() returned with error:"
		    );

		    nap_duration.tv_sec = REFRESH_RETRY_SNOOZE_TIME;

		} /* end else sec_login_validate_identity() failed OK */

	    } /* end if sec_login_refresh_identity() status OK */

	    else {
		print_dce_error (
		    status,
		    "sec_login_refresh_identity() error during refresh:"
		);

		nap_duration.tv_sec = REFRESH_RETRY_SNOOZE_TIME;

	    } /* end else sec_login_refresh_identity() status not OK */

	    /*
	    ** If nap_duration > 0 then something went wrong during
	    ** the refresh.  Increment count, sleep and try again.
	    */
	    if (nap_duration.tv_sec > 0) {
		char *suffix;

		count++;

		switch (count) {
		case 1:
		    suffix = "st";
		    break;
		case 2:
		    suffix = "nd";
		    break;
		case 3:
		    suffix = "rd";
		    break;
		default:
		    suffix = "th";
		    break;
		} /* end switch (count+1) */
		fprintf (stderr,
	    "Warning -- %d%s attempt to refresh identity failed (%d max).\n",
		     count,
		     suffix,
		     REFRESH_MAX_RETRIES
		);
		if (count < REFRESH_MAX_RETRIES) {
		    fprintf (stderr,
			"Will snooze for %d seconds and try again.\n",
			nap_duration.tv_sec
		    );
		}

		if (count < REFRESH_MAX_RETRIES) {
		    if (pthread_delay_np(&nap_duration) < 0) {
			fprintf (stderr,
			   "pthread_delay_np in refresh_identity() failed\n"
			);
		    } /* end else pthread_delay_np() failed */
		}

	    } /* end if (nap_duration > 0) */

	} /* end if (status = OK) || (not_certified) */

	else { /* something is wrong, increment loop count */
	    print_dce_error (
		status,
		"sec_login_get_expiration() failed with error:"
	    );
	    count++;
	}

    } /* end while (count < REFRESH_MAX_RETRIES) */

    fprintf (stderr,
	"Warning -- the identity refresh thread has terminated.\n"
    );

} /* end refresh_identity() */



/*
**********************************************************************
**
** S E T U P _ K E Y _ M G M T
**
**********************************************************************
*/
static void setup_key_mgmt
#ifdef PROTOTYPES
(
    /* [in,out] */ identity_info_p_t  identity_info_p
)
#else
(identity_info_p)
    /* [in,out] */ identity_info_p_t  identity_info_p;
#endif
{
    int            thread_status;
    unsigned char  access_ok;

    if (identity_info_p->keytab_file != NULL) {
	if (access ((char *)(identity_info_p->keytab_file), W_OK) == 0) {
	    access_ok = true;
	}
	else {
	    access_ok = false;
	}
    }
    else {
	access_ok = true;
    }

    if (access_ok) {

	thread_status = pthread_create (
	    &(identity_info_p->keymgmt_thread),  /* store thread id to detach */
	    pthread_attr_default,  /* use default thread attributes */
	    manage_key,     /* routine where thread should begin execution*/
	    (pthread_addr_t)identity_info_p /* argument to the routine */
	);

	if (thread_status != 0) {
	    fprintf(stderr,
		"%s\n%s: errno = %d\n%s\n",
		"unable to start a thread for key management",
		"pthread_create() failed",
		errno,
		"continuing anyway ..."
	    );
	    return;
	}

	/*
	** We're not interested in a return value from manage_key
	** so allow the runtime to reclaim storage as soon as
	** the key_mgmt thread finishes.
	*/
#ifndef PTHREAD_JOIN
	thread_status = pthread_detach(&(identity_info_p->keymgmt_thread));

	if (thread_status != 0) {
	    fprintf(stderr,
		"%s: errno = %d\n%s\n",
		"pthread_detach() for key_mgmt thread failed",
		errno,
		"continuing anyway ...\n"
	    );
	}
#endif

	/*
	** Give the thread a chance to get set up.  We'll get the CPU
	** back soon enough (like almost right away!) since the key_mgmt
	** thread is just going to go to sleep until it is time to 
	** generate a new key.
	*/
	pthread_yield ();

    } /* end if write access to keytab file is OK */

    else {

       fprintf (stderr,
	   "Warning: %s \"%s\"."
	   "No write access to security key file",
	   identity_info_p->keytab_file
       );
       fprintf (stderr,
	   "         %s\n",
	   "Will not be able to update the key."
       );
       fprintf (stderr,
	   "         %s\n",
	   "Not starting key management thread."
       );

    } /* end else no write access to keytab file. */

    return;

} /* setup_key_mgmt() */



/*
**********************************************************************
**
** M A N A G E _ K E Y
**
**********************************************************************
*/
static pthread_addr_t manage_key
#ifdef PROTOTYPES
(
    pthread_addr_t arg
)
#else
(arg)
    pthread_addr_t arg;
#endif
{
    identity_info_p_t  identity_info_p = (identity_info_p_t)arg;
    struct timespec nap_duration;
    error_status_t status;
    dce_error_string_t    dce_err_string;
    int            inq_status;

    while (true) {

	sec_key_mgmt_manage_key (
	    rpc_c_authn_dce_secret,
	    (void *)(identity_info_p->keytab_file),
	    (idl_char *)(identity_info_p->principal),
	    &status
	);
	if (status != rpc_s_ok) {

	    print_dce_error (
		status,
		"sec_key_mgmt_manage_key() error during manage_key():"
	    );
	} /* end if (status != rpc_s_ok) */


	/*
	** I don't exactly know why we sleep before looping around
	** but the examples I've seen all do this so like a little
	** sheep, I follow.
	*/
	nap_duration.tv_sec = 600;  /* 600 seconds == ten minutes */
	nap_duration.tv_nsec = 0;
	pthread_delay_np (&nap_duration);

    } /*end while (true) */

} /* end manage_key() */




/*
**********************************************************************
**
**  P R I N T _ D C E _ E R R O R
**
**********************************************************************
*/
void print_dce_error
#ifdef PROTOTYPES
(
    error_status_t status,
    char *msg
)
#else
(status, msg)
    error_status_t status;
    char *msg;
#endif
{
    int       inq_st;                    /* status for dce_error_inq_text */
    dce_error_string_t	dce_err_string;  /* text describing error code */

    dce_error_inq_text (status, dce_err_string, &inq_st);

    if (msg != NULL) {
	fprintf (stderr,
	    "%s\n   %s\n",
	    msg,
	    (inq_st == 0) ?
	    (char *)dce_err_string
	    :
	    "No dce error text available"
	);
    }
    else {
	fprintf (stderr,
	    "%s\n",
	    (inq_st == 0) ?
	    (char *)dce_err_string
	    :
	    "No dce error text available"
	);
    }
} /* end print_dce_error() */
