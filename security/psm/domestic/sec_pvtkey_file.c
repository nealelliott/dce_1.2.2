/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_pvtkey_file.c,v $
 * Revision 1.1.2.2  1996/11/13  18:09:40  arvind
 * 	Merge to DCE 1.2.2
 * 	[1996/10/30  23:02 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce12_tp_preauth2/1]
 *
 * 	sec_pvtkey_file_get(): do not free file context on error because
 * 	this routine did not allocate it.
 * 	[1996/10/24  14:19 UTC  cuti  /main/DCE_1.2.2/12]
 *
 * 	correct preprocessing error
 * 	[1996/10/24  13:09 UTC  cuti  /main/DCE_1.2.2/11]
 *
 * 	Correct AIX error
 * 	[1996/10/23  18:21 UTC  cuti  /main/DCE_1.2.2/10]
 *
 * 	Open file will be done in each operation within file_lock to avoid 0 file creation
 * 	[1996/10/16  19:05 UTC  cuti  /main/DCE_1.2.2/cuti_pk_fix/10]
 *
 * 	Review update
 * 	[1996/10/10  15:49 UTC  cuti  /main/DCE_1.2.2/cuti_pk_fix/9]
 *
 * 	Sec_pvtkey_file_store should take the input kvno
 * 	[1996/10/10  12:24 UTC  cuti  /main/DCE_1.2.2/cuti_pk_fix/8]
 *
 * 	Remove mutex lock
 * 	[1996/10/08  19:24 UTC  cuti  /main/DCE_1.2.2/cuti_pk_fix/7]
 *
 * 	update
 * 	[1996/10/08  19:22 UTC  cuti  /main/DCE_1.2.2/cuti_pk_fix/6]
 *
 * 	Make lock working
 * 	[1996/10/08  18:40 UTC  cuti  /main/DCE_1.2.2/cuti_pk_fix/5]
 *
 * 	check consistent passphrase & make sec_pvtkey_file_update working robustically
 * 	[1996/09/17  15:57 UTC  cuti  /main/DCE_1.2.2/cuti_pk_fix/4]
 *
 * Revision 1.1.2.1  1996/10/03  20:35:22  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/03  19:36:26  arvind]
 * 
 * 	OSF DCE 1.2.2 Drop 4
 * 
 * Revision /main/DCE_1.2.2/9  1996/09/17  18:45 UTC  cuti
 * 	Merge checking NULL handle.
 * 
 * 	CHFts19896: public key version support
 * 	[1996/09/16  20:31 UTC  aha  /main/DCE_1.2.2/8]
 * 
 * 	Modify for public key version numbers and sec_pk_domain_dce_pk_login
 * 	change to sec_pk_domain_dce_general
 * 	[1996/09/13  23:45 UTC  aha  /main/DCE_1.2.2/aha_pk9_3/1]
 * 	Revision /main/DCE_1.2.2/cuti_pk_fix/4  1996/09/17  15:57 UTC  cuti
 * 	Make canonical name in open
 * 
 * Revision /main/DCE_1.2.2/cuti_pk_fix/3  1996/09/17  13:11 UTC  cuti
 * 	Remove old lines
 * 
 * Revision /main/DCE_1.2.2/cuti_pk_fix/2  1996/09/17  13:07 UTC  cuti
 * 	Change sec_id_parse_name to sec_id_global_parse_name
 * 
 * Revision /main/DCE_1.2.2/cuti_pk_fix/1  1996/09/16  16:41 UTC  cuti
 * 	Use sec_id_parse_name instead of sec_rgy_pgo_name_to_id
 * 
 * Revision /main/DCE_1.2.2/7  1996/09/10  17:53 UTC  cuti
 * 	Merge.
 * 
 * Revision /main/DCE_1.2.2/cuti_pk_bugfix/11  1996/09/10  16:14 UTC  cuti
 * 	Merge DEC's change.
 * 
 * 	[1996/09/10  16:10 UTC  cuti  /main/DCE_1.2.2/cuti_pk_bugfix/10]
 * 	reverse to version 8
 * 
 * 	Change sec_pvtkey_store to take version number as input
 * 	[1996/09/10  15:38 UTC  cuti  /main/DCE_1.2.2/cuti_pk_bugfix/9]
 * 
 * 	Make read_and_match routine more robust and flow more smooth
 * 	[1996/08/24  19:00 UTC  cuti  /main/DCE_1.2.2/cuti_pk_bugfix/8]
 * 	Revision /main/DCE_1.2.2/6  1996/09/09  21:32 UTC  arvind
 * 	PKSS drop from DEC
 * 	[1996/09/04  22:20 UTC  arvind  /main/DCE_1.2.2/arvind_pkss_bl06/1]
 * 
 * 	Merge CHFts19625 fix.
 * 	[1996/08/05  13:33 UTC  cuti  /main/DCE122_PK/cuti_pk_migration/2]
 * 	*
 * 	check the status of rgy lookup
 * 	[1996/07/11  15:00 UTC  cuti  /main/DCE122_PK/cuti_pk_migration/1]
 * 	*
 * 	Not resetting st of check_handle in sec_pvtkey_close
 * 	[1996/07/03  12:48 UTC  cuti  /main/DCE122_PK/4]
 * 	*
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/06/11  18:00 UTC  aha  /main/DCE_1.2.2/1]
 * 	*
 * 
 * 	OSF DCE1.2.2 Drop3
 * 	sec_pvtkey.c,v
 * 	[1996/07/08  20:21:23  arvind  1.1.2.1]
 * 
 * 	Merge from cuti_pk_export to DCE_1.2.2
 * 
 * 	Change capability definition
 * 	[1996/06/27  14:05 UTC  cuti  /main/DCE122_PK/cuti_pk_export/2]
 * 
 * 	Merge out from cuti_pk_new to cuti_pk_export
 * 
 * 	Remove EXTERN and INITVAL
 * 	[1996/06/11  15:23 UTC  cuti  /main/DCE12_PK/cuti_pk_new/7]
 * 
 * 	Add match = false when bsafe_decrypt fails
 * 	[1996/06/06  19:44 UTC  cuti  /main/DCE12_PK/cuti_pk_new/6]
 * 
 * 	Move directory
 * 	[1996/06/04  18:00 UTC  cuti  /main/DCE12_PK/cuti_pk_new/5]
 * 
 * 	remove double indirection.
 * 	[1996/06/11  18:59 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_indir/1]
 * 
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/06/11  18:00 UTC  aha  /main/DCE_1.2.2/1]
 * 
 * 	Review change
 * 	[1996/05/24  20:38 UTC  cuti  /main/DCE12_PK/cuti_pk_new/4]
 * 
 * Revision /main/DCE_1.2.2/5  1996/08/21  14:23 UTC  cuti
 * 	Merged in memory leak fix.
 * 
 * Revision /main/DCE_1.2.2/cuti_pk_bugfix/7  1996/08/20  19:29 UTC  cuti
 * 	Minor update
 * 
 * Revision /main/DCE_1.2.2/cuti_pk_bugfix/5  1996/08/20  14:05 UTC  cuti
 * 	Add debugging info
 * 
 * Revision /main/DCE_1.2.2/cuti_pk_bugfix/4  1996/08/16  15:15 UTC  cuti
 * 	fix rpc_string_free(x, status) call
 * 
 * Revision /main/DCE_1.2.2/cuti_pk_bugfix/3  1996/08/16  13:59 UTC  cuti
 * 	set debug to 0
 * 
 * Revision /main/DCE_1.2.2/cuti_pk_bugfix/2  1996/08/14  17:33 UTC  cuti
 * 	Free princ_ids
 * 
 * Revision /main/DCE_1.2.2/cuti_pk_bugfix/1  1996/08/12  18:57 UTC  cuti
 * 	Merge Bill's change
 * 
 * Revision /main/DCE_1.2.2/4  1996/08/05  18:06 UTC  cuti
 * 	Merge CHFts19625 fix.
 * 
 * Revision /main/DCE122_PK/cuti_pk_migration/2  1996/08/05  13:33 UTC  cuti
 * 	check the status of rgy lookup
 * 
 * Revision /main/DCE122_PK/cuti_pk_migration/1  1996/07/11  15:00 UTC  cuti
 * 	Not resetting st of check_handle in sec_pvtkey_close
 * 
 * Revision /main/DCE122_PK/4  1996/07/03  12:48 UTC  cuti
 * 	Merge from cuti_pk_export to DCE_1.2.2
 * 
 * 	Change capability definition
 * 	[1996/06/27  14:05 UTC  cuti  /main/DCE122_PK/cuti_pk_export/2]
 * 
 * 	Merge out from cuti_pk_new to cuti_pk_export
 * 
 * 	Remove EXTERN and INITVAL
 * 	[1996/06/11  15:23 UTC  cuti  /main/DCE12_PK/cuti_pk_new/7]
 * 
 * 	Add match = false when bsafe_decrypt fails
 * 	[1996/06/06  19:44 UTC  cuti  /main/DCE12_PK/cuti_pk_new/6]
 * 
 * 	Move directory
 * 	[1996/06/04  18:00 UTC  cuti  /main/DCE12_PK/cuti_pk_new/5]
 * 
 * 	remove double indirection.
 * 	[1996/06/11  18:59 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_indir/1]
 * 
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/06/11  18:00 UTC  aha  /main/DCE_1.2.2/1]
 * 
 * Revision /main/DCE122_PK/3  1996/06/13  20:08 UTC  sommerfeld
 * 	remove double indirection.
 * 	[1996/06/11  18:59 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_indir/1]
 * 
 * Revision /main/DCE122_PK/2  1996/06/10  14:58 UTC  aha
 * 	Merge to DCE122_PK
 * 
 * Revision /main/DCE122_PK/aha_pk6/1  1996/06/06  21:31 UTC  aha
 * 	Remove EXTERN AND INITVAL
 * 
 * Revision /main/DCE122_PK/1  1996/06/04  18:53 UTC  psn
 * 	DCE12_PK -> DCE122_PK
 * 
 * Revision /main/DCE12_PK/3  1996/05/28  18:00 UTC  cuti
 * 	Merge psm, pvtkey work
 * 
 * 	Merge udpated pvtkey stuff
 * 
 * 	Add O_RDWR to open (O_CREAT) call in sec_pvtkey_open
 * 	[1996/05/13  18:06 UTC  cuti  /main/DCE12_PK/cuti_pk_new/1]
 * 	Revision /main/DCE12_PK/cuti_pk_new/4  1996/05/24  20:38 UTC  cuti
 * 	Review change
 * 
 * Revision /main/DCE12_PK/cuti_pk_new/3  1996/05/24  19:24 UTC  cuti
 * 	Using sec_rgy_pgo_name_to_id instead of sec_id_parse_name
 * 
 * Revision /main/DCE12_PK/cuti_pk_new/2  1996/05/23  18:41 UTC  cuti
 * 	Change pvtkey file name.
 * 
 * Revision /main/DCE12_PK/cuti_pk_new/1  1996/05/13  18:06 UTC  cuti
 * 	Add O_RDWR to open (O_CREAT) call in sec_pvtkey_open
 * 
 * Revision /main/DCE12_PK/1  1996/05/08  13:45 UTC  cuti
 * 	Prepare for review of pvtkey.
 * 
 * $EndLog$
 */

#include <sys/file.h>
#include <fcntl.h>
#include <pthread.h>
#include <dce/rgynbase.h>
#include <sec_pvtkey_file.h>
#include <sec_svc.h>
#include <dcepsmsvc.h>
#include <un_io.h>


/* Internal state */
static boolean32 pvtfile_mutex_inited = false;
static pthread_mutex_t pvtfile_mutex;
static pthread_once_t  pvtfile_mutex_once = pthread_once_init;

PRIVATE void pvtfile_mutex_init(
#ifdef __STDC__
    void
#endif
)
{
    pthread_mutex_init(&pvtfile_mutex, pthread_mutexattr_default);
    pvtfile_mutex_inited = true;
}

#define PVTFILE_MUTEX_INIT() \
    if (!pvtfile_mutex_inited) pthread_once(&pvtfile_mutex_once, pvtfile_mutex_init)


#define PVTFILE_MUTEX_LOCK() \
    PVTFILE_MUTEX_INIT(); \
    pthread_mutex_lock(&pvtfile_mutex);

#define PVTFILE_MUTEX_UNLOCK() \
    pthread_mutex_unlock(&pvtfile_mutex);

#if defined(__STDC__) && !defined(_K_R_PORT_CHECK_)
#define PVTKEY_FILE_PREFIX DCELOCAL_PATH "/var/security/pk_file/"
#else
 char *sec_pvtkey_path = NULL;
#   define PVTKEY_FILE_PREFIX \
    sec_util_string_init(&sec_pvtkey_path, DCELOCAL_PATH, "/var/security/pk_file/","")
    /* there is potential memory leak here.  Need to free sec_pvtkey_path after referencing
       PVTKEY_FILE_PREFIX */
#endif

#define CREATE_PK_DESCRIPTOR(desc) sec_pk_data_init(desc)

#define CLEANUP_DESCRIPTOR(desc) sec_pk_data_free(desc)

#define CLEANUP_PK_DESCRIPTOR(desc) sec_pk_data_free(desc)

#define CREATE_STORAGE(desc, size, status) \
	if (((desc)->data = (unsigned char *)malloc(size)) == NULL) { \
	    *status = sec_pvtkey_no_more_memory; \
	} else {\
            memset((desc)->data, 0, size); \
	}

typedef struct {
    void                 *magic;
    char                 *name;      /* user's name */
    uuid_t               user_uuid;  /* user's uuid in RGY */
    sec_pk_domain_t      domain_id;  /* the domain user is interested in */   
    int                  dfd;   /* pvtkey file descriptor */
    char                 *file_name; /* user's private key file name */
} sec_pk_file_mechanism_context_t, sec_pk_file_mechanism_context_p_t;


typedef struct {
    sec_pk_domain_t domain_id;
    unsigned32 key_usage;
    unsigned32 key_vno;
    unsigned32 name_len;
    unsigned32 pvtkey_len;
    unsigned32 pbe_params_len;
    unsigned32 key_hash_len;
    unsigned32 varying_len;
} pvtkey_fixed_data_t;


sec_pk_pvtkey_capability SEC_PVTKEY_CAP_KEYGEN = {   
  /* 3b199b81-cb89-11cf-8a1e-08002b918ea1 */
  0x3b199b81,
  0xcb89, 
  0x11cf, 
  0x8a, 
  0x1e,
  { 0x08, 0x00, 0x2b, 0x91, 0x8e, 0xa1 }
};

sec_pk_pvtkey_capability SEC_PVTKEY_CAP_ANYALG = {    
  /* 3d2901e0-cb89-11cf-93d8-08002b918ea1 */
  0x3d2901e0, 
  0xcb89, 
  0x11cf, 
  0x93, 
  0xd8,
  { 0x08, 0x00, 0x2b, 0x91, 0x8e, 0xa1 }
};

sec_pk_pvtkey_capability SEC_PVTKEY_CAP_KEYALG = {
  /* 3db8f521-cb89-11cf-850c-08002b918ea1 */
  0x3db8f521, 
  0xcb89, 
  0x11cf, 
  0x85, 
  0x0c,
  { 0x08, 0x00, 0x2b, 0x91, 0x8e, 0xa1 }
};

sec_pk_pvtkey_capability SEC_PVTKEY_CAP_KEYDEL = {    
  /* 3e2c9ca0-cb89-11cf-8ed1-08002b918ea1 */
  0x3e2c9ca0, 
  0xcb89, 
  0x11cf, 
  0x8e, 
  0xd1,
  { 0x08, 0x00, 0x2b, 0x91, 0x8e, 0xa1 }
};


typedef enum { MODE_WRITE, MODE_READ } lock_mode_t;

/* create a lock */

static error_status_t pvtkey_file_lock(
    lock_mode_t    mode,
    sec_pk_file_mechanism_context_t  *file_context
    )
{
    int oflag = O_RDWR;

#ifdef DCE_SEC_POSIX_F_LOCK
    struct flock    lock_arg;
#endif
    error_status_t st;

    CLEAR_STATUS(&st);
    PVTFILE_MUTEX_LOCK();

    if (file_context->dfd == 0) {
      switch(mode) {
      case MODE_WRITE: oflag = O_RDWR|O_CREAT;
	break;
      case MODE_READ:
      default: MODE_READ: oflag = O_RDWR;
      }

      file_context->dfd = open((char *)file_context->file_name, oflag, 0600);
    
      /* Check file descriptor.  */
      if (file_context->dfd < 0) { /* check permission */
	PVTFILE_MUTEX_UNLOCK();

	if (errno == EACCES) {
	  return(sec_pvtkey_privileged);
	}
	else
	  return(errno);
      }
    }

#ifdef DCE_SEC_POSIX_F_LOCK
    switch (mode) {
    case MODE_WRITE: lock_arg.l_type = F_WRLCK;
      break;
    case MODE_READ:
    default: lock_arg.l_type = F_RDLCK;
    }
    lock_arg.l_whence = 0;
    lock_arg.l_start = 0;
    lock_arg.l_len = 0;

    if (fcntl(file_context->dfd, F_SETLK, &lock_arg) == -1)  {
      if (errno == EACCES) 
	SET_STATUS(&st, sec_pvtkey_file_locked);
      else 
	SET_STATUS(&st, sec_pvtkey_file_cannot_open_key_file);
      PVTFILE_MUTEX_UNLOCK();
    }
#else
    if (flock(file_context->dfd, LOCK_EX|LOCK_NB) != 0) {
      if (errno == EWOULDBLOCK)
	SET_STATUS(&st, sec_pvtkey_file_locked);
      else
	SET_STATUS(&st, sec_pvtkey_file_cannot_open_key_file);
      PVTFILE_MUTEX_UNLOCK()
    }
#endif /* DCE_SEC_POSIX_F_LOCK */
    
    return st;

}



static void pvtkey_file_unlock(
 int lfd )
{

  TRY {

#ifdef DCE_SEC_POSIX_F_LOCK
    struct flock    lock_arg;

    lock_arg.l_type = F_UNLCK;
    lock_arg.l_whence = 0;
    lock_arg.l_start = 0;
    lock_arg.l_len = 0;

    fcntl(lfd, F_SETLK, &lock_arg);

#else
    flock(lfd, LOCK_UN);
#endif /* DCE_SEC_POSIX_F_LOCK */

    PVTFILE_MUTEX_UNLOCK();
  } CATCH_ALL {
    PVTFILE_MUTEX_UNLOCK();
    RERAISE;
  } ENDTRY;

}


/* check the validity of the file mechanism handle
 *  error: sec_pvtkey_mechanism_not_init
 *         sec_pvtkey_invalid_handle
 */

sec_pk_file_mechanism_context_t *  sec_pvtkey_check_handle(
    sec_pk_mechanism_handle_t        handle,
    error_status_t                   *stp) 
{

    sec_pk_file_mechanism_context_t *file_context=handle;


    CLEAR_STATUS(stp);

    /* check if handle is NULL, stop here, otherwise we'll just die */
    if (!handle) { /* handle is NULL */
        SET_STATUS(stp, sec_pvtkey_mechanism_not_init);
        return file_context;
    }

    /* Make sure handle is not a garbage. */
    if (file_context->magic != file_context) {
        SET_STATUS(stp, sec_pvtkey_invalid_handle);
	}

    return(GOOD_STATUS(stp) ? file_context: NULL);



}




/* Prepare the private key data for writing to file 
   There are fixed part (length of varying data and integer) and 
   varying part (varying length of name, key) of data.  Need to
   handle them separetely. Users need to free varying after done using it */

/* error msg:  sec_pvtkey_no_more_memory
 *
 */

/* JCW 8/22/96: Added static keyword */
static error_status_t form_pvtkey_file_data(
    char                        *name,            /* in */
    sec_pk_domain_t       	*domain_id,       /* in */
    sec_pk_usage_flags_t  	key_usage,       /* in */
    sec_pk_data_t       	*pvtkey,          /* in */
    unsigned32             	key_vno,          /* in */
    sec_pk_data_t               *pbe_params,      /* in */
    sec_pk_gen_data_t           *digest,          /* in */
    pvtkey_fixed_data_t         *fixed,           /* out*/
    char                        **varying         /* out*/)
{
    unsigned32 varying_len=0;
    char *c;
    error_status_t st;

    CLEAR_STATUS(&st);

    /* gather all the fixed size stuff, (length and integer)
        and keep track of the varying len */
    memcpy(&(fixed->domain_id), domain_id, sizeof(sec_pk_domain_t));
    fixed->key_usage = key_usage;
    fixed->key_vno = key_vno;

    /* gather varying data's len */
    fixed->name_len = strlen(name);
    varying_len += fixed->name_len;
    fixed->pvtkey_len = pvtkey->len;
    varying_len += fixed->pvtkey_len;
    fixed->pbe_params_len = pbe_params->len;
    varying_len += fixed->pbe_params_len;
    fixed->key_hash_len = digest->len;
    varying_len += fixed->key_hash_len;
    fixed->varying_len = varying_len;
    
    
    /* malloc varying stuff and gather them */
    /* Users need to free varying after done using it */
    *varying = (char *)malloc(varying_len);
    if (varying == NULL) {
	memset(fixed, 0, sizeof(pvtkey_fixed_data_t));
	return(sec_pvtkey_no_more_memory);
    }

    c = *varying;
    memcpy(c, name, fixed->name_len);
    c += fixed->name_len;

    memcpy(c, pvtkey->data, fixed->pvtkey_len);
    c += fixed->pvtkey_len;

    memcpy(c, pbe_params->data, fixed->pbe_params_len);
    c += fixed->pbe_params_len;

    memcpy(c, digest->data, digest->len);

    return(st);

}

/* error: sec_pvtkey_internal_error.
 */

/* JCW 8/22/96: Added static keyword */
static error_status_t write_pvtkey_file_data(
    int                 dfd,                  /* in */
    pvtkey_fixed_data_t *fixed,               /* in */
    char                *varying              /* out */)
{
    int   nbytes;
    error_status_t st;

    CLEAR_STATUS(&st);

     /* write fixed-length portion and check the status */
        nbytes = write(dfd, (char *) fixed, sizeof(pvtkey_fixed_data_t));
        if (nbytes != sizeof(pvtkey_fixed_data_t)) {
            SET_STATUS(&st, sec_pvtkey_internal_error);
            return st;
        }
        /* write variable length portion and check the status */
        if (GOOD_STATUS(&st)) {
            nbytes = write(dfd, varying, fixed->varying_len);
            if (nbytes != fixed->varying_len) {
                SET_STATUS(&st, sec_pvtkey_internal_error);
            }
        }
    return st;
}


/* NEW error : sec_pvtkey_no_more_memory
 *             sec_pvtkey_internal_error 
 */
/* read one record at a time.  User need to free varying
   when done using it  */

/* JCW 8/22/96: Added static keyword */
boolean32 read_pvtkey_file_data(
    int                 dfd,
    pvtkey_fixed_data_t *fixed,
    char                **varying,
    error_status_t      *stp      )
{
   int                 nbytes=0;


   /* Initilize the data
    */
    CLEAR_STATUS(stp);   
    memset(fixed, 0, sizeof(pvtkey_fixed_data_t));

    /* read fixed length portion of data 
     */
    nbytes = read(dfd, (char *) fixed, sizeof(pvtkey_fixed_data_t));

   if (nbytes == 0) {/* end of file */
       SET_STATUS(stp, sec_pvtkey_no_more_data);
       return(false);
   }

   if (nbytes == -1) {/* error encounter */
       SET_STATUS(stp, errno);
       return(false);
   }

    if (nbytes != sizeof(pvtkey_fixed_data_t)) {
        SET_STATUS(stp, sec_pvtkey_internal_error);
	return(false);
    }
    else {

	/* Malloc varying portion and initialize it 
	 */
        *varying = (char *)malloc(fixed->varying_len);
        if (*varying == NULL) {
            SET_STATUS(stp, sec_pvtkey_no_more_memory);
	    return(false);
        }
        memset(*varying, 0, fixed->varying_len);

        /* which tells us how much variable length data to read */
        nbytes = read(dfd, *varying, fixed->varying_len);
        if (nbytes != fixed->varying_len) {
            SET_STATUS(stp, sec_pvtkey_internal_error);
	    return(false);
        }
    }

    return(true);

}


/* Match the private key's attributes.  If matched, return pointer to 
 * varying component's address.  So there is no need to free these output
 * params.  If the value of key_usage or key_vno is 0, then those attributes
 * can be ignored.  
/* error: sec_pvtkey_internal_error
 *
 */

/* JCW 8/22/96: Added static keyword */
static boolean32 match_pvtkey(
    char                 *name, 
    sec_pk_domain_t      *domain_id, 
    sec_pk_usage_flags_t key_usage,
    unsigned32           *key_vno, 
    pvtkey_fixed_data_t  *fixed,
    char                 *varying,
    sec_pk_data_t        *pbe_params,   /* out */
    sec_pk_gen_data_t    *digest,       /* out */
    sec_pk_data_t        *pvtkey,       /* out */
    error_status_t       *stp           /* out */)
{
    char *p=varying;


    /* Initialize output data. */

    CLEAR_STATUS(stp);
    pbe_params->data = NULL;
    digest->data = NULL;
    pvtkey->data = NULL;

    /* Check NULL fixed or varying pointer to avoid core dump */

    if (!fixed || !varying) { /* both fixed and varying cant be NULL */
        SET_STATUS(stp, sec_pvtkey_internal_error);
	return false;
    }
    /* Now we are safe to reference the fixed and varying pointer
     * do name match 
     */
    if (memcmp(p, name, fixed->name_len))  /* match fail */
	return false;
    p += fixed->name_len;

    /* compare domain_id */
    if (!uuid_equal(&fixed->domain_id, domain_id, stp)) {
        /* uuid match fail */
        return false;
    }

    /* compare key usage, 0 key_usage means ignore key_usate match */
    if (key_usage != 0) {
      if (fixed->key_usage != key_usage)
        return false;
    }

    /* 0 key_vno means ignore key version match */
    if (*key_vno != 0) {
      if (fixed->key_vno != *key_vno) {
	*stp = sec_pvtkey_version_not_exist;
	return false;
      }
    }
    else {
      *key_vno = fixed->key_vno;
    }


    /* Now we meet all the requirement: name, domain, key_usage.
     * Since varying is malloced, returned pointer can just point to varing's
     * component.  Later varying is freed, so is these pointers data. 
     */
    /* Get encrypted private key. */
    pvtkey->len = fixed->pvtkey_len;
    pvtkey->data = (unsigned char *)p;
    /* advance pointer to next component */
    p += fixed->pvtkey_len; 

    /*  get pbe_params */
    pbe_params->len = fixed->pbe_params_len;
    pbe_params->data = (unsigned char *)p;
    /* advance pointer to next component */
    p += fixed->pbe_params_len; 


    /* get digest */
    digest->len = fixed->key_hash_len;
    digest->data = (unsigned char *)p;

    return true;

}




static boolean32 check_passphrase(
    sec_pk_file_mechanism_context_t  *file_context,
    unsigned32  offset,
    char *pwd)
{
    int                         new_offset=0;
    boolean32                   match=false, exit_status=false, find=0;
    sec_pk_usage_flags_t        key_usage;
    pvtkey_fixed_data_t         fixed;
    char                        *varying=NULL;
    sec_pk_data_t               pbe_params;
    sec_pk_gen_data_t           digest;
    sec_pk_data_t               encrypted_pvtkey;
    sec_pk_data_t               local_pvtkey;
    sec_pk_gen_data_t           password;
    error_status_t              st; 
    sec_pk_gen_data_t           calc_digest;
    unsigned32                  key_vno;

    if (!pwd) {
      return false;
    }

    /* Initialize cursor */
    new_offset = lseek(file_context->dfd, offset, SEEK_SET);
    if (new_offset != offset) {
       SET_STATUS(&st,  sec_pvtkey_internal_error);
       return false;
    }

    find = read_pvtkey_file_data(file_context->dfd, &fixed, &varying, &st);
    key_usage = 0;  /* dont' care */
    key_vno = 0; /* don't care */

    if (!find)
       return true; /* no key yet, so new passphrase matched */
    

    match = match_pvtkey(file_context->name, &file_context->domain_id, 
				key_usage, &key_vno,
                                &fixed, varying, &pbe_params, &digest, 
                                &encrypted_pvtkey, &st);
    if (!match) {
      if (varying)
	free(varying);
      return true; /* No attribute match, so new passphrase matched  */
    }

    password.len = strlen(pwd);
    password.data = (unsigned char *)pwd;
    if (st = sec_bsafe_decrypt_pvtkey(&password, 
					&encrypted_pvtkey,
					&pbe_params, 
					&local_pvtkey)) {
      if (varying)
	free(varying);
      return false;  /* Key found, but no passphrase matched */
    }
	      
    if (st = sec_bsafe_compute_hash(sec_bsafe_hash_MD5, 
				      (sec_pk_gen_data_t *)&local_pvtkey, 
				      &calc_digest)) {
      /* error encountered */
       CLEANUP_PK_DESCRIPTOR(&local_pvtkey);
       if (varying)
	 free(varying); 
      return false;
    }

    if (!memcmp(digest.data, calc_digest.data, 
			       calc_digest.len)) {
                       /* pwd match */
      exit_status = true;
    }
    else {
      exit_status = false;
    }

    /* cleanup */
    CLEANUP_PK_DESCRIPTOR(&local_pvtkey);
    CLEANUP_DESCRIPTOR(&calc_digest);

    if (varying)
      free(varying);
      
    return exit_status;
		   

}


/* read through private key file and find one matching private key attributes.
   If pwd is non-NULL, check the correctness of pwd; if private_key is 
   non-NULL, return the value.  Need to free private key when done using it 
   Return true to mean found correct domain_id, and key_usage; 
   if return status is true and bad stp, means found matched attribute, however
   there are other failure factor, such as wrong passwd.

   return value true indicates a good match of domain_id and key_usage and name.
                false indicates no good match found.
   stp value indicates if there are any other failure.
   Both return value and stp should be taken into considication for correctly
   return data.
*/

/* error: sec_pvtkey_no_matched_private_key
 *        sec_pvtkey_no_more_memory
 *        sec_pvtkey_internal_error
 *        sec_pvtkey_invalid_pwd
 *        sec_s_bsafe_encryption_failure
 */

/* JCW 8/22/96: Added static keyword */
static boolean32 sec_pvt_read_and_match_pvtkey(
    sec_pk_file_mechanism_context_t  *file_context,
    char                 *pwd,           /* can be NULL */
    sec_pk_usage_flags_t key_usage, 
    sec_pk_data_t        *pvtkey,    /* DER-encoded decrpted key 
                                        Can be NULL, not interested in
                                       in returning data */
    unsigned32           *key_vno,
    int                  *entry_len,
    error_status_t       *stp)
{
    boolean32                   match=false, find=false;
    sec_pk_data_t               pbe_params;
    sec_pk_gen_data_t           digest;
    sec_pk_gen_data_t           calc_digest;
    sec_pk_gen_data_t           password;
    sec_pk_data_t               encrypted_pvtkey;
    sec_pk_data_t               local_pvtkey;
    int                         offset=0;
    char                        *varying;
    pvtkey_fixed_data_t         fixed;

    /* Initialize the output. */
    *entry_len = 0;

    CLEAR_STATUS(stp);

    /* Initialize cursor */
     offset = lseek(file_context->dfd, 0, SEEK_SET);

    /* Loop through the entries until the end of file or find a match.
     * After each loop, need to free varying which holds verying length data.
     * A match can means just match the name, domain_id, key_usage.
     * This is usually used in key_store which wants to check if the
     * matched one exists or not.  Pwd checking is not too important
     * here, so it can pass in NULL pwd.
     * More often match means pwd verification is required to be done
     * on top of matching all the attributes.
     * Private_key can be a NULL value which means not private key is
     * expected to be returned.  If private_key is asked to return, it
     * should be freed when done using it.
     */

    while (GOOD_STATUS(stp) && !match){
       varying = NULL;
       find = read_pvtkey_file_data(file_context->dfd, &fixed, &varying, stp);
       if (GOOD_STATUS(stp) && find) { /* find the right pvtkey */
           match = match_pvtkey(file_context->name, &file_context->domain_id, 
				key_usage, key_vno,
                                &fixed, varying, &pbe_params, &digest, 
                                &encrypted_pvtkey, stp);
	   
           if (GOOD_STATUS(stp) && match) {
               /* attribute match */
               if (pwd) {/* verify pwd */
		   *key_vno = fixed.key_vno;

                   password.len = strlen(pwd);
		   password.data = (unsigned char *)pwd;
               	   if (*stp = sec_bsafe_decrypt_pvtkey(&password, 
						       &encrypted_pvtkey,
						       &pbe_params, 
						       &local_pvtkey)) {
		       if (*stp == sec_s_bsafe_input_data) {
			   *stp = sec_s_bsafe_decryption_failure;
		       }
		       if (varying)
			   free(varying);
		       return match;
		   }

                   /* compute hash to verify pwd */
               	   if (*stp = sec_bsafe_compute_hash(sec_bsafe_hash_MD5, 
						     (sec_pk_gen_data_t *)&local_pvtkey, 
						     &calc_digest)) {/* faile */
		       CLEANUP_PK_DESCRIPTOR(&local_pvtkey);
		       if (varying)
                           free(varying);
		       return match;
		   }
		       
               	   if (!memcmp(digest.data, calc_digest.data, 
			       calc_digest.len)) {
                       /* pwd match */
                       *entry_len = sizeof(pvtkey_fixed_data_t) + 
                               fixed.varying_len;
                       if (pvtkey) { /* non NULL, private key
                             is asked to return, malloc and copy data */
		           CREATE_PK_DESCRIPTOR(pvtkey);

			   CREATE_STORAGE(pvtkey, local_pvtkey.len, stp);
			   if (GOOD_STATUS(stp)) {
			       pvtkey->len = local_pvtkey.len;
			       memcpy(pvtkey->data, local_pvtkey.data, local_pvtkey.len); 
			   } else {
			       CLEANUP_PK_DESCRIPTOR(pvtkey);
                              
			   }
		       }
	       	   } else { /* attributes match, but not pwd */
                       SET_STATUS(stp, sec_pvtkey_invalid_pwd);
		       CLEANUP_PK_DESCRIPTOR(&local_pvtkey);
		       CLEANUP_DESCRIPTOR(&calc_digest);
		       if (varying)
                           free(varying);
		       return match;
                   }

                   /* clean up local_pvtkey, calc_digest */
		   if (GOOD_STATUS(stp)) {
		       CLEANUP_DESCRIPTOR(&local_pvtkey);
		       CLEANUP_DESCRIPTOR(&calc_digest);
		   }
               }
	       /*  else NULL pwd, pwd verifing is not important */

           } /* end of good match */          
        } /* end of good read, free varying */
       if (varying)
            free(varying);
    } /* end while */
              
    /* if we got here, it means no match is found or everything is correct */
    if ( !match) {
       if (GOOD_STATUS(stp) || STATUS_EQUAL(stp, sec_pvtkey_no_more_data) ) { /* status hasn't reset yet */
          SET_STATUS(stp, sec_pvtkey_no_matched_private_key); /* no match */
       }
    }

     return match;
   
}


/* sec_pvtkey_file_open
 * 
 * Open (connect to) the private key storage service.  For file-based 
 * keystore implementation, it creates the file for the follow up key
 * store.  It opens the file for the follow up key retrieval or key
 * update.  It can involve a password check (please note, password check is 
 * not applicable to sys admin's operation).  For a server-based 
 * implementation, it binds to the server.
 *
 * Return status
 *   error_status_ok:   Success.
 *   other (non-zero): 
 *             for PKSS:
 *                      Can't initialize keystore container.
 *                      Can't locate keystore.
 *                      Can't communicate with keystore.
 *                      Access-control failure. 
 *             for file base:
 *                      sec_pvtkey_privileged
 *                      sec_pvtkey_no_more_memory
 *
 * Input
 *
 *    Name: Pointer to the user's canonical name within the 
 *          specified domain.
 *    domain_id: The application domain this key belongs to.
 *
 * Output
 *    handle: A pointer to an opaque handle to the private key context
 *            data.  The context data should contain the domain_id of
 *            the operation.
 *  
 */ 

/* Pwd is not checked here. It opens pvtkey file and fills up 
 * handle's context.  Each mechanism handle is mechanism-dependent.
 */ 


error_status_t sec_pvtkey_file_open(
    idl_char                  *name,
    sec_pk_domain_t           *domain_id,
    sec_pk_mechanism_handle_t *handle
) {
    error_status_t st;
    sec_pk_file_mechanism_context_t  *file_context=NULL;
    struct stat          stbuf;
    sec_rgy_pname_t      pvtkey_file, tfile;
    sec_rgy_handle_t     rgy_handle;
    sec_rgy_name_t       pname, cell, full_name;
    uuid_t               princ_id, cell_id;
    unsigned_char_t      *princ_ids;
    sec_rgy_bind_auth_info_t auth_info;
    sec_rgy_login_name_t login_name;
    sec_rgy_cursor_t     cursor;
    sec_rgy_unix_sid_t   unix_sid;
   


    /* Initialize output data. */
    CLEAR_STATUS(&st);
    *handle = NULL;
    uuid_create_nil(&princ_id, &st);
    /* Create or open pvtkey_file */

    auth_info.info_type = sec_rgy_bind_auth_none;
    auth_info.tagged_union.dce_info.authn_level= rpc_c_protect_level_pkt_integ;
    auth_info.tagged_union.dce_info.authn_svc = rpc_c_authn_dce_secret;
    auth_info.tagged_union.dce_info.authz_svc = rpc_c_authz_dce;
    auth_info.tagged_union.dce_info.identity  = NULL;
 
    sec_id_global_parse_name(&auth_info, name, cell, &cell_id, pname, &princ_id, &st);
    if (BAD_STATUS(&st)) 
	return st;
    strcpy(full_name, cell);
    strcat(full_name, "/");
    strcat(full_name, pname);

    uuid_to_string(&princ_id, &princ_ids, &st);
    
    strcpy(pvtkey_file, PVTKEY_FILE_PREFIX);

    strcat(pvtkey_file, princ_ids);
    DCE_SVC_DEBUG((sec_svc_handle, psm_s_pvtkey, svc_c_debug1,
                   "pvtkey_file: %s, name: %s.\n", pvtkey_file, full_name));

    rpc_string_free(&princ_ids, &st);
    strcpy(tfile, pvtkey_file);
    strcat(tfile, ".bak");


    if ((stat((char *)pvtkey_file, &stbuf)) && (!stat((char *)tfile, &stbuf))) {
      /* backup file exist only, so rename it */
      rename((char *)tfile, (char *)pvtkey_file);
    }

    /* Malloc the handle and fill in the contents. */
    if (!(file_context = 
	(sec_pk_file_mechanism_context_t *)malloc(sizeof(sec_pk_file_mechanism_context_t)))) { 
	return(sec_pvtkey_no_more_memory);
    }
	
    file_context->dfd = 0; /* file opened in file_lock */
    file_context->domain_id =  *domain_id;
    if (!(file_context->name= (char *)malloc(strlen(full_name) + 1))) {
	free(file_context);
	file_context = NULL;
	return(sec_pvtkey_no_more_memory); 
    }
    strcpy(file_context->name, (char *)full_name);

    if (!(file_context->file_name = (char *)malloc(strlen(pvtkey_file) + 1))) {
	free(file_context);
	file_context = NULL;
	return(sec_pvtkey_no_more_memory);
    }
    strcpy(file_context->file_name, (char *)pvtkey_file);

    memcpy(&file_context->user_uuid, &princ_id, sizeof(uuid_t));

    file_context->magic = file_context;
    
    *handle = (sec_pk_mechanism_handle_t *)file_context;

    return st;
}



/* sec_pvtkey_file_close
 * 
 * Close the connection to the private key storage service.  It also
 * frees the handle.  If there are sensitive data, the data should
 * be zeroed first.
 *
 * Return status
 *   error_status_ok:   Success.
 *   other (non-zero):  Can't close keystore container.
 *                      Can't locate keystore.
 *                      Can't communicate with keystore.
 *                      Access-control failure.
 *              for file base:
 *                      sec_pvtkey_invalid_handle
 *                      sec_pvtkey_mechanism_not_init
 *
 * Input
 *
 *    handle: A pointer to an opaque handle to the private key context
 *            data.  The handle should be obtained through 
 *            sec_pvtkey_file_open()
 *  
 */ 


error_status_t sec_pvtkey_file_close(
    sec_pk_mechanism_handle_t        handle  /* in */
) {
    error_status_t st;
    sec_pk_file_mechanism_context_t  *file_context=NULL;

    CLEAR_STATUS(&st);

    if (file_context = sec_pvtkey_check_handle(handle, &st)) {
	if (file_context->name) {
	    free(file_context->name);
	    file_context->name = NULL;
	}

	if (file_context->file_name) {
	    free(file_context->file_name);
	    file_context->file_name = NULL;
	}

	if (file_context->dfd != 0)
	  close (file_context->dfd);
	file_context->magic = NULL;
	free(file_context);
	file_context = NULL;
	return st;    
    }
    else {
	return st;
    }
 
}


/* sec_pvtkey_file_store
 * 
 * Store the user's keypair encrypted using the user's password in
 * the storage service.  The encryption mechanism is determined
 * by the service provider.  The public-key algorithm ID (such as 
 * RSA, DSA) should be included in both the private_key and 
 * public_key structures which are the final [in] parameters.  
 * The keys will be indexed by name, domain_id, key_usage and 
 * key_version in the private key storage service in order to 
 * allow retrieval via the sec_pvtkey_file_get API.
 *
 * If the specified key already exists in the keystore, the routine
 * will leave it unchanged and return a failure status.  To replace
 * a key in a keystore, either use sec_pvtkey_file_update or
 * sec_pvtkey_file_update_generate, or call sec_pvtkey_file_delete followed by 
 * sec_pvtkey_file_store.
 * 
 * A keystore may enforce its own access-control requirements, and
 * an access-control failure will be indicated by an appropriate status
 * code, with the contents of the keystore unchanged.  For example,
 * a file-based keystore implementation may require that the caller
 * has write-access to the keystore file; a server-based implementation
 * may require that the caller has appropriate ACL-based permission
 * to create a new entry; a memory-card implementation may require that
 * the card be write-enabled.
 *
 * Return status
 *   error_status_ok:   Success.
 *   other (non-zero):  Can't initialize keystore container.
 *                      Can't locate keystore.
 *                      Can't communicate with keystore.
 *                      Access-control failure.
 *                      Same domain and usage's key already exists.
 *                for file base:
 *                      sec_pvtkey_invalid_handle
 *                      sec_pvtkey_mechanism_not_init
 *                      sec_pvtkey_no_more_memory
 *                      sec_pvtkey_internal_error
 *                      sec_pvtkey_same_domain_and_usage_key_already_exists.
 *                      sec_pvtkey_multiple_key_usages
 *
 * Input
 *
 *    handle: A pointer to an opaque handle to the private key context
 *            data.  The handle should be obtained through 
 *            sec_pvtkey_file_open()
 *    pwd:  Pointer to the user's password.
 *    key_usage: The usage permitted of this keypair.  One key_usage 
 *               at a time.
 *    key_vno: The key version number. (0 may be specified to 
 *             indicate that the routine should select the next 
 *             appropriate or newest key version number).  
 *             (In the reference implementation, key_vno starts with
 *              1, then monotonically increase 1 for each update.)
 *    private_key: Pointer to the encoded private key 
 *                 structure. 
 *    public_key: Pointer to the encoded public key structure.
 *  
 */ 
/*
 *  Need to store each key_usage separately
 */

error_status_t sec_pvtkey_file_store(
    sec_pk_mechanism_handle_t   handle,         /* in */
    idl_char                   	*pwd,           /* in */
    sec_pk_usage_flags_t  	key_usage,     /* in */
    unsigned32             	key_vno,        /* in */
    sec_pk_data_t       	*private_key,   /* in */
    sec_pk_data_t               *public_key     /* in, ignored in reference  */
){

    sec_pk_gen_data_t    digest;
    sec_pk_data_t        pbe_params;
    sec_pk_data_t        encrypted_pvtkey;
    pvtkey_fixed_data_t  fixed;
    sec_pk_gen_data_t    password;
    char                 *varying=NULL;
    error_status_t       st;
    sec_pk_file_mechanism_context_t  *file_context;
    int len, offset, i=0;
    unsigned32                  tmp_kvno=0;


    CLEAR_STATUS(&st);
    /* Check the key usage */
    if (sec_pk_get_key_usage_count(key_usage) > 1)
        return (sec_pvtkey_multiple_key_usages);

    /* Check mechansim handle */
    if (!(file_context = sec_pvtkey_check_handle(handle, &st))) {
	return st;
    }

    /* Don't do anything if we can't lock the pvtkey file for write */
    TRY {

      if (st = pvtkey_file_lock(MODE_WRITE, file_context)) {
	return st;
      }

      /* Check the existence of the key, but don't check the correctness of pwd.
       * pwd is not important here.  Here is to see if same domain and key usage
       * already exist for this user (key_vno should be ignore).
       * Call sec_pvt_read_and_match_pvtkey() with NULL pwd and NULL private_key
       * (not interesting in data) if exist return with
       * key_already_exist.   
       */

      if (sec_pvt_read_and_match_pvtkey(file_context, 
				      (char *)pwd,
				      key_usage,
				      NULL,
				      &tmp_kvno,
				      &len,
				      &st)) { 
	pvtkey_file_unlock(file_context->dfd);
        SET_STATUS(&st, sec_pvtkey_same_domain_and_usage_key_already_exist);
      }
      else {
	/* check consistent password */
	offset = 0; /* since this must not be the first one */
	if (!check_passphrase(file_context, offset, (char *)pwd)) {
	  SET_STATUS(&st, sec_pvtkey_wrong_passwd);
	}
	else {
	  /* Now this is really a new key for the key usage in the specified 
	   * domain, so encrypt the private key.  Need to free pbe_params,
	   * encrypted_pvtkey after done.
	   */

	  password.len = strlen(pwd);
	  password.data = (unsigned char *)pwd;
	  if (!(st = sec_bsafe_encrypt_pvtkey(&password, private_key, 
					&pbe_params, &encrypted_pvtkey))) {


	    /* Compute the hash value of the plain key for checking the correctness
	     * of private key.  Need to free digest after done.
	     */

	    if (!(st = sec_bsafe_compute_hash(sec_bsafe_hash_MD5, 
					    (sec_pk_gen_data_t *)private_key, 
					    &digest))) {


	      /* Form pvt_context.  Need to free varying.
	       */

	      if (!(st = form_pvtkey_file_data(file_context->name, 
					     &file_context->domain_id,
					     key_usage, &encrypted_pvtkey, key_vno, 
					     &pbe_params, &digest, &fixed, 
					     &varying))) {

	 

		/* Move file pointer to the end fo file for appending the data.
		 */

		offset = lseek(file_context->dfd, 0, SEEK_END); /* Append entry */

		/* write to disk
		 */
   
		st = write_pvtkey_file_data(file_context->dfd, &fixed, 
				varying);
	      }
	    }
	  }
	}
      }

      pvtkey_file_unlock(file_context->dfd);

    } CATCH_ALL {
        pvtkey_file_unlock(file_context->dfd);
        RERAISE;
    } ENDTRY;

    /* Free varying, pbe_params, encrypted_pvtkey, digest, done using it.
     */

    CLEANUP_DESCRIPTOR(&pbe_params);
    CLEANUP_DESCRIPTOR(&encrypted_pvtkey);
    CLEANUP_DESCRIPTOR(&digest);

    if (varying)
     free(varying);

    return st;

}

 
/* sec_pvtkey_file_get
 * 
 * Retrieve the user's keypair from the storage device. 
 * The routine allocates memory for the data returned in the 
 * private key and public key parameters.  Users should call 
 * sec_pvtkey_free() to deallocate that memory.  
 * 
 * The keypair returned will be the first one found with a 
 * stored key_usage value containing the input "key_usage" value.
 * i.e. the input value (which will typically have a single bit 
 * set) will be "AND"d with the stored value (which may have many 
 * bits set), and the will be returned if the result is not 0.  
 *
 * Return status
 *   error_status_ok:   Success.
 *   other (non-zero):  Access control failure.
 *                      Can't locate keystore.
 *                      Can't communicate with keystore.
 *                      Can't find specified key.
 *                      Invalid password.
 *                for file base:
 *                      sec_pvtkey_invalid_handle
 *                      sec_pvtkey_mechanism_not_init
 *                      sec_pvtkey_internal_error
 *                      sec_pvtkey_invalid_password
 *                      sec_pvtkey_multiple_key_usages
 *
 * Input
 *
 *    handle: A pointer to an opaque handle to the private key context
 *            data.  The handle should be obtained through 
 *            sec_pvtkey_file_open()
 *    pwd:  Pointer to the user's password.
 *    domain_id: The application domain of the desired key.
 *    key_usage: The usage of the public key of the desired key. One
 *               key_usage at a time. 
 *
 * Input/Output
 *
 *    key_vno: The key version number. (0 may be specified to 
 *             indicate that the routine should select the 
 *             newest key version number).  The return value is
 *             the key version of the key returned.
 *
 * Output
 *
 *    private_key: Pointer to the encoded private key 
 *                 structure. 
 *    public_key   Pointer to the encoded public key 
 *                 structure.
 *
 */ 

/* Public key is not expected to return in this implementation.  Users
   need to free private key when done using it. */

error_status_t sec_pvtkey_file_get(
    sec_pk_mechanism_handle_t handle,                   /* in */
    idl_char                  *pwd,                     /* in */
    sec_pk_usage_flags_t      key_usage,                /* in */
    unsigned32                *key_vno,                 /* in, out */
    sec_pk_data_t             *private_key,             /* out */
    sec_pk_data_t             *public_key               /* out */
){
    error_status_t  st;
    boolean32 found = false;
    sec_pk_file_mechanism_context_t  *file_context = NULL;
    int entry_len;



    if (sec_pk_get_key_usage_count(key_usage) > 1)
        return (sec_pvtkey_multiple_key_usages);
    
    /* Check the handle. */
    if (!(file_context = sec_pvtkey_check_handle(handle, &st))) {
        return st;
    }

    /*Initialize return data (private_key, public key) to NULL. */
    CLEAR_STATUS(&st);
    if (public_key) {
	CREATE_PK_DESCRIPTOR(public_key);
    }
    if (private_key) {
	CREATE_PK_DESCRIPTOR(private_key);
    }


    if (GOOD_STATUS(&st)) {
      TRY {
      if (st = pvtkey_file_lock(MODE_READ, file_context)) {
	return st;
      }

	/* find the right private key and verify pwd. */
	found = sec_pvt_read_and_match_pvtkey(file_context, 
					      (char *)pwd, key_usage, 
					      private_key, 
					      key_vno,
					      &entry_len, &st);
	pvtkey_file_unlock(file_context->dfd);

      } CATCH_ALL {
        pvtkey_file_unlock(file_context->dfd);
        RERAISE;
      } ENDTRY;

    }

    return st;

}

static error_status_t cleanup_copy_file(
    sec_pk_file_mechanism_context_t  *file_context,
    int   offset)
{


    pvtkey_fixed_data_t fixed;
    int dfd;
    char                *varying;
    sec_rgy_pname_t    tmp_file, tfile;
    int new_offset;
    error_status_t    st;
    int                 nbin=0, nbout=0;
    char         c;
    boolean32    do_write;

    CLEAR_STATUS(&st);   
    strcpy(tmp_file, file_context->file_name);
    strcat(tmp_file, ".new");
    strcpy(tfile, file_context->file_name);
    strcat(tfile, ".bak");

   dfd = open((char *)tmp_file, (O_RDWR|O_CREAT), 0600);

   if (dfd < 0) { /* check permission */
	if (errno == EACCES) {
	    return(sec_pvtkey_privileged);
	}
        else
            return(errno);
    }

    
   new_offset = lseek(file_context->dfd, 0, SEEK_SET);

   while(true) {
     /* read fixed length portion of data 
      */
     
     new_offset = lseek(file_context->dfd, 0, SEEK_CUR);
     if (new_offset == offset)
       do_write = false;
     else 
       do_write = true;

     nbin = read(file_context->dfd, (char *) &fixed, sizeof(pvtkey_fixed_data_t));
     if (nbin == 0) {/* end of file */
       break;
     }

     if (nbin != sizeof(pvtkey_fixed_data_t)) {
       SET_STATUS(&st, sec_pvtkey_internal_error);
     }
     else {
       if (do_write) {
	 nbout = write(dfd, (char *) &fixed, sizeof(pvtkey_fixed_data_t));

	 /* write fixed-length portion and check the status */
	 if (nbout != sizeof(pvtkey_fixed_data_t)) {
	   SET_STATUS(&st, sec_pvtkey_internal_error);
	 }
	 else {
	   varying = (char *)malloc(fixed.varying_len);
	   if (!*varying) {
	     SET_STATUS(&st, sec_pvtkey_no_more_memory);
	   }
	   else {
	     nbin = read(file_context->dfd, varying, fixed.varying_len);
	     if (nbin != fixed.varying_len) {
	       SET_STATUS(&st, sec_pvtkey_internal_error);
	     }
	     else {
	       nbout = write(dfd, varying, fixed.varying_len);
	       if (nbout != fixed.varying_len) {
		 SET_STATUS(&st, sec_pvtkey_internal_error);
	       }
	     }
	     if (varying)
	       free(varying);
	   }
	 }
       }
       else { /* skip rest of the entry */
	 new_offset = lseek(file_context->dfd, fixed.varying_len, SEEK_CUR);
       }
     }
   }  /* end while */

   /*  close files and rename .bak, unlink */

   fsync(dfd);
   close(dfd);
   if (GOOD_STATUS(&st)) {
     close(file_context->dfd);
     rename((char *)file_context->file_name, (char *)tfile);
     rename((char *)tmp_file, (char *)file_context->file_name);
     unlink((char *)tfile);

     /* reopen the file and reassign the file pointer */
     dfd = open((char *)file_context->file_name, O_RDWR, 0);
     if (dfd < 0) { /* check permission */
	if (errno == EACCES) {
	    return(sec_pvtkey_privileged);
	}
        else
            return(errno);
    }

     file_context->dfd = dfd;
   }
   else {
     unlink((char *)tmpfile);
   }

   return st;

}

static error_status_t sec_pvtkey_remove_one_key(
    sec_pk_file_mechanism_context_t  *file_context,
    idl_char                   	     *pwd,             /* in */
    idl_char                         *new_pwd,         /* in */
    boolean32                        check_pwd,        /* in */
    sec_pk_usage_flags_t  	     key_usage,        /* in */
    unsigned32             	     *key_vno,          /* in */
    sec_pk_data_t                    *target_pvtkey,
    int                              *entry_len)
{
    error_status_t st;
    boolean32         found = false;
    int               offset, new_offset, nbytes;

     CLEAR_STATUS(&st);
   
      /* Check the key usage */
      if (sec_pk_get_key_usage_count(key_usage) > 1)
        return (sec_pvtkey_multiple_key_usages);

      CREATE_PK_DESCRIPTOR(target_pvtkey);

      found = sec_pvt_read_and_match_pvtkey(file_context, 
					    (char *)pwd, 
					    key_usage, 
					    target_pvtkey, 
					    key_vno,
					    entry_len, 
					    &st);

      if (!found || BAD_STATUS(&st) ) 
	return(st);

      

    /* check owner's user pid, owner should present current_pwd */
    if (!pwd) {
      struct stat          stbuf; 
      uid_t                uid;
      int st;

      if (!fstat(file_context->dfd, &stbuf)) {
	uid = getuid();
	if (stbuf.st_uid == uid) {
	  return(sec_pvtkey_old_pass_required);
	}
      }
    }

      /* Move file pointer the the beginning of the item */
      new_offset = lseek(file_context->dfd, (0-*entry_len) , SEEK_CUR);

    if (check_pwd) {

      /* check the passphrase */
      /* This getting ugly. There is no easy way to check the consistency of
       * passphrase.  My theory is that either old_passphrase should be same
       * other key usages, like the first one to get updated, or the new
       * passphrase is the same as other usages, like the later update.
       */
      if (new_offset != 0) {
	/* updated key is not in the first of the file */
	offset = 0; 
      }
      else {
	offset = *entry_len;
      }

      if (!check_passphrase(file_context, offset, (char *)new_pwd) &&
	  !check_passphrase(file_context, offset, (char *)pwd)) {
        return(sec_pvtkey_wrong_passwd);
      }
    }

      /* cleanup file */
     st = cleanup_copy_file(file_context, new_offset);

     return st;

}

/* sec_pvtkey_file_update
 * 
 * Update the user's keypair record in a keystore.  This API may be 
 * used to change either the password, or the keypair, or both.  
 * It may be invoked either by the user to change her own 
 * keypair record (in which case the current_pwd parameter must 
 * be supplied), or by a system administrator to change another 
 * user's keypair record (in which case the current_pwd parameter 
 * should be specified as NULL.  
 *
 * Keystore implementations may impose their own access-control
 * requirements.  In particular, some keystore implementations 
 * may not support administrative updates, or may require that
 * the caller have valid administrative DCE credentials.
 *
 * Return status
 *   error_status_ok:   Success.
 *   other (non-zero):  Access control failure.
 *                      Can't locate keystore.
 *                      Can't communicate with keystore.
 *                      Can't find specified record.
 *                      Invalid password.
 *                 for file base:
 *			sec_pvtkey_invalid_handle
 *			sec_pvtkey_mechanism_not_init
 *			sec_pvtkey_private_key_is_not_supplied
 *			sec_pvtkey_new_password_required
 *			sec_pvtkey_no_more_memory
 *			sec_pvtkey_internal_error
 *			sec_pvtkey_no_matched_private_key
 *                      sec_pvtkey_Invalid_password.
 *                      sec_pvtkey_multiple_key_usages
 *
 * Input
 *
 *    handle: A pointer to an opaque handle to the private key context
 *            data.  The handle should be obtained through 
 *            sec_pvtkey_file_open()
 *    current_pwd: Pointer to the user's current password.
 *                 Used to authenticate the user when changing her own
 *                 password or keypair.  If this parameter is NULL, 
 *                 the keystore will assume that the update is an 
 *                 administrative action, and will enforce 
 *                 administrative access-control
 *    new_pwd: Pointer to the user's new password.  If the user
 *             is changing her own key, and does not wish to change
 *             the associated password, this argument may be NULL
 *             (subject to any restrictions imposed by the keystore).
 *             If this is an administrative action, a valid new_pwd
 *             must be supplied.
 *    key_usage: The allowed usage for this keypair.  One
 *               key_usage at a time. 
 *    private_key: Pointer to the encoded private key structure. 
 *    public_key:  Pointer to the encoded public key structure.
 *                 If the user is changing her own password, and does
 *                 not wish to change the corresponding keypair, this
 *                 parameter may be NULL (subject to any restrictions
 *                 imposed by the keystore).  If this is an administrative
 *                 action, valid private and public keys must be supplied.
 *
 * Output
 *    key_vno: The key version number. (0 may be specified to 
 *             indicate that the routine should select the next 
 *             appropriate or newest key version number).  
 *             (In the reference implementation, key_vno starts with
 *              1, then monotonically increase 1 for each update.)
 *
 */ 

error_status_t sec_pvtkey_file_update(
    sec_pk_mechanism_handle_t handle,                    /* in */
    idl_char                  *current_pwd,              /* in */
    idl_char                  *new_pwd,                  /* in */
    sec_pk_usage_flags_t      key_usage,                /* in */
    unsigned32                *key_vno,                  /* in, out */ 
    sec_pk_data_t 	      *private_key,              /* in */ 
    sec_pk_data_t             *public_key                /* in */
   )
{

    error_status_t    st;
    sec_pk_data_t     pbe_params;
    sec_pk_gen_data_t digest;
    int               entry_len=0;
    sec_pk_data_t     target_pvtkey, encrypted_pvtkey;
    sec_pk_data_t     *pvtkey_to_encrypt;
    char              *pwd;
    sec_pk_gen_data_t password;
    sec_pk_file_mechanism_context_t  *file_context;
    int               offset=0, new_offset=0;
    pvtkey_fixed_data_t  fixed;
    char                 *varying=NULL;
    int                  i = 0;
    unsigned32           kvno;


    /* Check the handle. */
    CLEAR_STATUS(&st);
    if (!(file_context = sec_pvtkey_check_handle(handle, &st))) {
	return st;
    }

    /* Don't do anything if we can't lock the local registry for write */
    TRY {
      if (st = pvtkey_file_lock(MODE_WRITE, file_context)) {
	return st;
      }

      /* remove key no matter what key version */
      kvno = 0;
      if (!(st = sec_pvtkey_remove_one_key(file_context, current_pwd, 
                                       new_pwd, true, /* check_pwd */
                                       key_usage, &kvno,
				       &target_pvtkey, &entry_len))) {



	/* Reencrypt key with new_pwd, prepare key and pwd to be 
	 * used in encryption
	 */

	if (current_pwd) { 
	  /* non-NULL current_pwd, so the user issues the command.  
	   * Need to check the validity of the user 
	   * verify the current_pwd,  find the right private key. 
	   */


	  /* Reencrypt key with new_pwd, prepare key and pwd to be 
	   * used in encryption
	   */

	  if (private_key) { /* change key */
	    pvtkey_to_encrypt = private_key;
	  } 
	  else {
	    pvtkey_to_encrypt = &target_pvtkey;
	  }

	  if (new_pwd) { /* change pwd */
	    pwd = (char *)new_pwd;
	  }
	  else
	    pwd = (char *)current_pwd;
       
	  password.len = strlen(pwd);
	  password.data = (unsigned char *)pwd;
	  st = sec_bsafe_encrypt_pvtkey(&password, 
					pvtkey_to_encrypt,
					&pbe_params,
					&encrypted_pvtkey);
	}
	else {

	  /* no old_pwd, admin is doing it, need new private key and new pwd */
	  if (private_key == NULL) {
	    SET_STATUS(&st, sec_pvtkey_private_key_is_not_supplied);
	  }

	  if (new_pwd == NULL) {
	    SET_STATUS(&st, sec_pvtkey_new_password_required);
	  }

	  pvtkey_to_encrypt = private_key;

	  password.len = strlen(new_pwd);
	  password.data = (unsigned char *)new_pwd;
	  st = sec_bsafe_encrypt_pvtkey(&password, 
					pvtkey_to_encrypt,
					&pbe_params, 
					&encrypted_pvtkey);
	}
 
	if (GOOD_STATUS(&st)) {
	  /* recompute hash */
	  if (!(st = sec_bsafe_compute_hash(SEC_PK_HASH,
					    (sec_pk_gen_data_t *)pvtkey_to_encrypt,
					    &digest))) {
	    /* Form pvt_context, */
	    if (!(st = form_pvtkey_file_data(file_context->name, 
					     &file_context->domain_id,
					     key_usage,  
					     &encrypted_pvtkey,
					     *key_vno, 
					     &pbe_params, 
					     &digest, 
					     &fixed, 
					     &varying))) {

	      /* Append the new key, write to disk */
	      offset = lseek(file_context->dfd, 0, SEEK_END);
	      st = write_pvtkey_file_data(file_context->dfd, 
					  &fixed, 
					  varying);
	    }
	  }
	}
      }
      pvtkey_file_unlock(file_context->dfd);

    } CATCH_ALL {
      pvtkey_file_unlock(file_context->dfd);
      RERAISE;
    } ENDTRY;


    /* Free varying, pbe_params, encrypted_pvtkey, digest.
     * Free target_pvtkey which is returned from readn_and match if no 
     * private_key passed in.  
     */   
    CLEANUP_DESCRIPTOR(&target_pvtkey);
    CLEANUP_DESCRIPTOR(&pbe_params);
    CLEANUP_DESCRIPTOR(&encrypted_pvtkey);
    CLEANUP_DESCRIPTOR(&digest);

    if (varying)
      free(varying);

    return st;

}


/* sec_pvtkey_file_capabilities
 *
 * Inquire about the capabilities of a key storage service.  Multiple 
 * storage services may be supported by a client system, selected 
 * according to username and/or domain_id.  If no keystore service 
 * is defined for a given username and domain_id, all sec_pvtkey 
 * operations for that name/domain will return the error 
 * "Can't locate keystore".  Some services may not 
 * support all storage service operations, and the 
 * sec_pvtkey_file_capabilities function allows the PSM module to 
 * interrogate a particular storage service to determine whether a 
 * given feature or operation is supported.
 *
 * Return status
 *   error_status_ok:    Success.
 *   other (non-zero):   Can't locate keystore.
 *                       Can't communicate with keystore.
 *                  for file base:
 *                       sec_pvtkey_capability_not_supported
 *
 * Input
 *    Name: Pointer to the user's canonical name within the 
 *          specified domain.
 *    capability_parameter: Pointer to the paramter specific to the
 *              capability specified.  In KEYGEN capability, it is
 *              public-key algorithm.  If specified as
 *              NULL, the default public-key algorithm (RSA)
 *              is implied.  Many capability queries will
 *              ignore this parameter.
 *    Feature:  Pointer to a uuid code identifying a service feature.
 *
 *              The following features are currently defined:
 *              Feature code            Feature
 *              ------------            -------
 *              SEC_PVTKEY_CAP_KEYGEN   Service can generate keys for the
 *                                      specified algorithm.
 *              SEC_PVTKEY_CAP_ANYALG   Service can store keys for any
 *                                      algorithm.
 *              SEC_PVTKEY_CAP_KEYALG   Service can store keys for the
 *                                      specified algorithm.
 *              SEC_PVTKEY_CAP_KEYDEL   Service can delete user's key.
 *
 *   
 * Output
 *    Supported: true if the feature is available, false if not.
 *
 */

error_status_t sec_pvtkey_file_capabilities(
    sec_pk_mechanism_handle_t handle,                  /* in */
    void                      *capability_parameter,   /* in */
    sec_pk_pvtkey_capability  *feature,                /* in */
    boolean32                 *supported               /* out */
) {
    error_status_t st;

    SET_STATUS(&st, error_status_ok);
    *supported = false;  /* no special operations supported */
    SET_STATUS(&st, sec_pvtkey_capability_not_supported);
    return (st); 

}



/* sec_pvtkey_file_delete
 * 
 * Delete the user's keypair from the storage device. 
 *
 * Return status
 *   error_status_ok:   Success.
 *   other (non-zero):  Can't initialize keystore container.
 *                      Can't locate keystore.
 *                      Can't communicate with keystore.
 *                      Access-control failure.
 *                 for file base:
 *                      sec_pvtkey_multiple_key_usages.
 *
 * Input
 *
 *    handle: A pointer to an opaque handle to the private key context
 *            data.  The handle should be obtained through 
 *            sec_pvtkey_file_open()
 *    pwd:  Pointer to the user's password.
 *    key_usage: The usage of the key pair of the desired key. One
 *               key_usage at a time or NULL key usage.
 *               NULL value means "delete all the key_usages
 *               related to the user.
 *    key_vno: The version number of the desired key.  If 0 (zero) is
 *             specified, the function will select the most recent key
 *             key version number.  (or should it be "all the key version?)
 *  
 */ 

error_status_t sec_pvtkey_file_delete(
    sec_pk_mechanism_handle_t   handle,                  /* in */
    idl_char                   	*pwd,                    /* in */
    sec_pk_usage_flags_t  	key_usage,               /* in */
    unsigned32             	key_vno                 /* in */
){
    sec_pk_file_mechanism_context_t  *file_context;
    error_status_t st;
    sec_pk_data_t     target_pvtkey;
    int               entry_len=0;
    boolean32         found = false;
    int               offset, nbytes;
    pvtkey_fixed_data_t  fixed;
    char                *varying;
    unsigned32          varying_len=0;    


    CLEAR_STATUS(&st);
    /* Check mechansim handle */
    if (!(file_context = sec_pvtkey_check_handle(handle, &st))) {
	return st;
    }

    if (!key_usage) {
      /* remove all the keys, flush the pvtkey file */
      (void) unlink(file_context->file_name);
    }
    else {
      /* remove one key */

      TRY {
	if (st = pvtkey_file_lock(MODE_WRITE, file_context)) {
	  return st;
	}

	st = sec_pvtkey_remove_one_key(file_context, pwd, NULL, false,
                                    key_usage, &key_vno,  
				    &target_pvtkey, &entry_len);

	CLEANUP_PK_DESCRIPTOR(&target_pvtkey);
	pvtkey_file_unlock(file_context->dfd);

      } CATCH_ALL {
	pvtkey_file_unlock(file_context->dfd);
	RERAISE;
      } ENDTRY;

    }

    return st;

}

/* sec_pvtkey_file_generate
 * 
 * Generate a new keypair and store it in the keystore encrypted using 
 * the user's password in the storage service.  The encryption 
 * mechanism is determined by the service provider. 
 * The keys will be indexed by name, domain_id, key_usage and 
 * key_version in the private key storage service in order to allow 
 * retrieval via the sec_pvtkey_file_get API.
 *
 * If the specified key already exists in the keystore, the routine
 * will leave it unchanged and return a failure status.  To replace
 * a key in a keystore, either use sec_pvtkey_file_update or
 * sec_pvtkey_file_update_generate, or call sec_pvtkey_file_delete followed by 
 * sec_pvtkey_file_store.
 * 
 * A keystore may enforce its own access-control requirements, and
 * an access-control failure will be indicated by an appropriate status
 * code, with the contents of the keystore unchanged.  For example,
 * a file-based keystore implementation may require that the caller
 * has write-access to the keystore file; a server-based implementation
 * may require that the caller has approriate ACL-based permission
 * to create a new entry; a memory-card implementation may require that
 * the card be write-enabled.
 *
 * Return status
 *   error_status_ok:   Success.
 *   other (non-zero):  Can't initialize keystore container.
 *                      Can't locate keystore.
 *                      Can't communicate with keystore.
 *                      Access-control failure.
 *                      Same domain and usage's key already exits.
 *                      Key generation not supported.
 *		for file base:
 *                    	sec_pvtkey_key_generation_not_supported
 *
 * Input
 *
 *     handle: A pointer to an opaque handle to the private key context
 *             data.  The handle should be obtained through 
 *             sec_pvtkey_file_open()
 *     pwd:    Pointer to the user's password.
 *     alg_id: Desired public-key algorithm.  If specified as
 *             NULL, the default public-key algorithm (RSA)
 *             shall be used.
 *     key_length: Desired key-length.  The length of the desired key.
 *                 Interpretation of this parameter is algorithm-dependent;
 *                 For RSA, it shall be interpreted as the bit-length of
 *                 the key.  For any algorithm, a value of 0xffffffff
 *                 shall mean the algorithm-specific default.
 *     key_usage: The usage of the public key this key pair 
 *                 shall belongs to.
 *     key_vno: The key version number. (0 may be specified to 
 *             indicate that the routine should select the next 
 *             appropriate or newest key version number).  
 *             (In the reference implementation, key_vno starts with
 *              1, then monotonically increase 1 for each update.)
 *
 * Output
 *
 *     public_key: Pointer to the encoded public key structure of the
 *     newly-generated key.
 *
 */ 

error_status_t sec_pvtkey_file_generate(
    sec_pk_mechanism_handle_t   handle,                  /* in */
    idl_char                   	*pwd,                    /* in */
    sec_pk_algorithm_id_t       *alg_id,                 /* in */
    unsigned32                  key_length,              /* in */
    sec_pk_usage_flags_t  	key_usage,               /* in */
    unsigned32             	key_vno,                 /* in */
    sec_pk_data_t       	*public_key             /* out */
) {
    /* initialize out params */
    CREATE_PK_DESCRIPTOR(public_key);

    return(sec_pvtkey_key_generation_not_supported);

}

/* sec_pvtkey_file_update_generate
 * 
 * Update the user's keypair record for a keystore that supports
 * key generation.  This API may be used to change
 * either the keypair alone, or both the keypair and the password.
 * It may be invoked either by the user to change her own keypair 
 * record (in which case the current_pwd parameter must be supplied), 
 * or by a system administrator to change another user's keypair 
 * record (in which case the current_pwd parameter should be specified 
 * as NULL.  
 *
 * Keystore implementations may impose their own access-control
 * requirements.  In particular, some keystore implementations 
 * may not support administrative updates, or may require that
 * the caller have valid administrative DCE credentials.
 *
 *
 * Return status
 *   error_status_ok:    Success.
 *   other (non-zero):  Access control failure.
 *                      Can't locate keystore.
 *                      Can't communicate with keystore.
 *                      Can't find specified record.
 *                      Key generation not supported.
 *                      Invalid password.
 *		for file base:
 *			sec_pvtkey_key_generation_not_supported
 *
 * Input
 *
 *    handle: A pointer to an opaque handle to the private key context
 *            data.  The handle should be obtained through 
 *            sec_pvtkey_file_open()
 *    current_pwd: Pointer to the user's current password.
 *                 Used to authenticate the user when changing her own
 *                 password or keypair.  If this parameter is NULL, 
 *                 the keystore will assume that the update is an 
 *                 administrative action, and will enforce 
 *                 administrative access-control
 *    new_pwd: Pointer to the user's new password.  If the user
 *             is changing her own key, and does not wish to change
 *             the associated password, this argument may be NULL
 *             (subject to any restrictions imposed by the keystore).
 *             If this is an administrative action, a valid new_pwd
 *             must be supplied.
 *     alg_id: Desired public-key algorithm.  If specified as
 *             NULL, the default public-key algorithm (RSA)
 *             shall be used.
 *     key_length: Desired key-length.  The length of the desired key.
 *                 Interpretation of this parameter is algorithm-dependent;
 *                 For RSA, it shall be interpreted as the bit-length of
 *                 the key.  For any algorithm, a value of 0xffffffff
 *                 shall mean the algorithm-specific default.
 *     key_usage: The allowed usage for this keypair.  One at a time.
 *
 * Output
 *
 *    key_vno: The key version number. (0 may be specified to 
 *             indicate that the routine should select the next 
 *             appropriate or newest key version number).  
 *             (In the reference implementation, key_vno starts with
 *              1, then monotonically increase 1 for each update.)
 *    public_key:  The newly-generated public-key.
 *
 */ 

error_status_t sec_pvtkey_file_update_generate(
    sec_pk_mechanism_handle_t handle,                    /* in */
    idl_char                  *current_pwd,              /* in */
    idl_char                  *new_pwd,                  /* in */
    sec_pk_algorithm_id_t     *alg_id,                   /* in */
    unsigned32                key_length,                /* in */
    sec_pk_usage_flags_t      key_usage,                /* in */
    unsigned32                *key_vno,                  /* in */
    sec_pk_data_t             *public_key                /* out */
)
{

    /* initialize return data */
    CREATE_PK_DESCRIPTOR(public_key);
    
    return(sec_pvtkey_key_generation_not_supported);

}







