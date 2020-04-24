/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * @DEC_COPYRIGHT@
 * G-L-O-R-I-A modifications:
 * Copyright (c) 1996
 * Digital Equipment Corporation, Maynard, Massachusetts, USA
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: gdb_db44.h,v $
 * Revision 1.1.2.1  1996/10/03  20:24:43  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/03  19:19:37  arvind]
 *
 * Revision /main/DCE_1.2.2/1  1996/09/09  21:22 UTC  arvind
 * 	PKSS drop from DEC (DCE1.2.2)
 * 	[1996/08/30  15:36 UTC  arvind  /main/arvind_pkss/1]
 * 
 * 	Remove include of threads_lib.h.
 * 	[1996/07/10  18:48:03  zee]
 * 	 *
 * 
 * 	Remove dependency on dthread macros.  Use pthread calls
 * 	directly
 * 	[1996/07/08  20:05:08  farrell]
 * 
 * 	updates for generalization of comparison and debug routines
 * 	[1996/06/28  17:06:36  farrell]
 * 
 * 	GLORIA (R1.2.2) revision.
 * 	[1996/06/24  20:48:02  zee]
 * 
 * $EndLog$
 */
/*
 * Module: gdb_db44.h
 *    Generic DataBase (GDB) include file.
 */

#ifndef _GDB_DB44_H_
#define _GDB_DB44_H_

#ifndef GDB_DEBUG
#define GDB_DEBUG
#endif

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <dcecdssvc.h>

/*
 * db44 include files
 */
#include <db.h>
#include <btree.h>
#include <extern.h>

/*
 * external gdbi include file 
 */
#include <gdb.h>

/*
 * Configuration defaults (Your mileage may vary)
 */
#define DEFAULT_CACHE_SIZE_BLOCKS 1000
#define DEFAULT_SECONDARY_CACHE_SIZE_BLOCKS 50

/*
 * Access control information for this data store
 */
typedef struct access {
   pthread_mutex_t lock; /* lock for this structure */   
   pthread_cond_t out;   /* condition variable to signal read or write lock is given up */
   pthread_t *writerp;   /* address of member "writer", or NULL, if no
			    writer thread is active */
   pthread_t writer;     /* thread id of thread that either is writing or will be the next to write */
   int nreaders;         /* number of readers who have called READ_LOCK, but not READ_UNLOCK */
   int nwriters;         /* number of writers who have called WANNA_WRITE */
   int write_in_process; /* WRITE_LOCK is held */
} access_t;

/*
 * Sort comparison functions take arguments
 *   (int key1_size, char *key1p, int key2_size, char *key2p) 
 * and returns:
 *    >0 if key1 is "bigger" than key2, 
 *    0 if equal, 
 *    <0 if key1 is "smaller" than key2
 */
typedef int (* compare_func_t)(int, char *, int, char *);

/*
 * Display functions take arguments
 *   (int size, char *key_or_data_pointer)
 * and return void.
 */
typedef void (* display_func_t)(int, char *);

/*
 * All the information we need for the data stored.
 * Passed to the caller as the opaque storage handle
 */
typedef struct store_info {
   DB * tree;
   DB * tree2;
   access_t control; /* contains pointers, so should be aligned */
   unsigned int temporary; /* TRUE for in-memory db's */
#ifdef GDB_DEBUG
   unsigned int txn; /* DEBUG only for checking that we don't nest txns */
#endif
   pthread_key_t lock_state_key; /* Thread specific key to get the 
				    thread's lock status.  Used for
				    freeing locks & ending txn
				    when a thread dies mid-transaction */
   display_func_t primary_key_display; 
                               /* key_display function for primary
				  tree is store here so that if a
				  secondary index is created, we'll
				  be able to provide a data (primary key)
				  display routine automagicly */
} store_info_t;

/*
 * stored entry  
 *
 * For a tree that only has one key, this buffer just holds
 * the entry data.
 *
 * In the case of a database with a secondary key, it looks
 * like this.  The offset back from the entry to the start 
 * of buffer is included so that when the caller passes the 
 * entry address into gdb_free() and we can find the start
 * of buffer to do a good old fashioned free.
 *
 *  +--------------------------------------------+
 *  | offset from start of buffer to size of key2
 *  +--------------------------------------------+
 *  | entry data (variable length)
 *  ...
 *  +--------------------------------------------+
 *  | ((pad for alignment of following field))
 *  +--------------------------------------------+
 *  | size of key 2
 *  +--------------------------------------------+
 *  | key 2 (variable length)
 *    ...
 *  +--------------------------------------------+
 *
 */

/*
 *  data[1] will be set to point to the space in memory where the
 *  entry data information will be stored if there's no secondary
 *  key.
 *  If there is a secondary key, then data[1] will point to the space
 *  in memory where the secondary key is stored... 
 */

typedef union stored_entry {
   /*
    * Structure of data if there's a secondary index 
    */
   struct {
      size_t data_size; /* data_size rounded up for alignment is the 
				 offset to key2 size */
      char data[1];     /* begining of area to put entry, 
			   key2size, and key2            */
   } has_key;  
   /*
    * Data the old fashion way if there's no secondary index
    */
   char data[1];   
} stored_entry_t;



/**
 **
 ** Locking macros 
 **
 **/
#define COPY_pthread(d1,d2) memcpy((void *)&d2,(void *)&d1,sizeof(pthread_t))

/*
 * The whole reason for the thread-specific data below is
 * to protect against a thread that dies while holding a
 * read lock on the data store, or mid-transaction/holding
 * a write lock on the data store.  We keep track of the
 * thread's locks in this structure, and provide a destructor
 * routine that will unlock/rollback txn as appropriate.
 */
typedef	enum { 
    STORE_NOT_WRITER = 0, STORE_WANNA_WRITE, STORE_WRITE_LOCKED
} write_lock_state_t;

typedef struct {
    int                read_lock_depth;
    write_lock_state_t write_lock_state;
    store_info_t       *store_p;
} lock_state_t;

#define getspecific(ds,x,y) { \
    pthread_getspecific((x), (y)); \
    if (*(y) == (pthread_addr_t)NULL) { \
	*(y) = malloc(sizeof(lock_state_t)); \
	memset(*(y),0,(sizeof(int)+sizeof(write_lock_state_t))); \
        pthread_setspecific((x), *(y)); \
        ((lock_state_t *)*(y))->store_p = ds; \
    }\
}
                          
#define GDB_READ_LOCK(ds) { \
    pthread_addr_t ls; \
    pthread_t t=pthread_self(); \
    DCE_SVC_DEBUG((cds__svc_handle, \
		   cds_svc_gdb, \
		   svc_c_debug2, \
		   " >GDB_READ_LOCK(%p)", ds)); \
    pthread_mutex_lock(&(ds)->control.lock); \
    while (((ds)->control.write_in_process) && \
	   (!pthread_equal((ds)->control.writer,t))) { \
      pthread_cond_wait(&(ds)->control.out, &(ds)->control.lock); \
    } \
    (ds)->control.nreaders = (ds)->control.nreaders + 1; \
    pthread_mutex_unlock(&(ds)->control.lock); \
    getspecific(ds, (ds)->lock_state_key, &ls); \
    ((lock_state_t *)ls)->read_lock_depth++; \
    DCE_SVC_DEBUG((cds__svc_handle, \
		   cds_svc_gdb, \
		   svc_c_debug2, \
		   " <GDB_READ_LOCK(%p) lock_depth=%d", ds, \
		   ((lock_state_t *)ls)->read_lock_depth)); \
    }                                                          
                                                               
#define GDB_READ_UNLOCK(ds) { \
    pthread_addr_t ls; \
    DCE_SVC_DEBUG((cds__svc_handle, \
		   cds_svc_gdb, \
		   svc_c_debug2, \
		   " >GDB_READ_UNLOCK(%p)", ds)); \
    getspecific(ds, (ds)->lock_state_key, &ls); \
    ((lock_state_t *)ls)->read_lock_depth--; \
    pthread_mutex_lock(&(ds)->control.lock); \
    (ds)->control.nreaders = (ds)->control.nreaders - 1; \
    pthread_mutex_unlock(&(ds)->control.lock); \
    pthread_cond_broadcast(&(ds)->control.out); \
    DCE_SVC_DEBUG((cds__svc_handle, \
		   cds_svc_gdb, \
		   svc_c_debug2, \
		   " <GDB_READ_UNLOCK(%p) lock_depth=%p", ds, \
                   ((lock_state_t *)ls)->read_lock_depth)); \
    }                                                          
                                                               
#define GDB_WANNA_WRITE(ds) { \
    pthread_addr_t ls; \
    pthread_t t=pthread_self(); \
    DCE_SVC_DEBUG((cds__svc_handle, \
		   cds_svc_gdb, \
		   svc_c_debug2, \
		   " >GDB_WANNA_WRITE(%p)", ds)); \
     pthread_mutex_lock(&(ds)->control.lock); \
     (ds)->control.nwriters = (ds)->control.nwriters + 1; \
     while ((!pthread_equal((ds)->control.writer,t)) && \
	    ((ds)->control.writerp != NULL)) { \
      pthread_cond_wait(&(ds)->control.out, &(ds)->control.lock); \
     } \
     COPY_pthread(t, (ds)->control.writer); \
     (ds)->control.writerp = &((ds)->control.writer); \
     pthread_mutex_unlock(&(ds)->control.lock); \
     getspecific(ds, (ds)->lock_state_key, &ls);\
     ((lock_state_t *)ls)->write_lock_state = STORE_WANNA_WRITE; \
     DCE_SVC_DEBUG((cds__svc_handle, \
		    cds_svc_gdb, \
		    svc_c_debug2, \
		    " <GDB_WANNA_WRITE(%p)", ds)); \
    }
                                                               
#define GDB_WRITE_LOCK(ds) { \
     pthread_addr_t ls; \
     DCE_SVC_DEBUG((cds__svc_handle, \
		   cds_svc_gdb, \
		   svc_c_debug2, \
		   " >GDB_WRITE_LOCK(%p)", ds)); \
     pthread_mutex_lock(&(ds)->control.lock); \
     while ((ds)->control.nreaders != 0) { \
      pthread_cond_wait(&(ds)->control.out, &(ds)->control.lock); \
     } \
     (ds)->control.write_in_process = TRUE; \
     dce_assert(cds__svc_handle, ((ds)->control.writerp != NULL)); \
     pthread_mutex_unlock(&(ds)->control.lock); \
     getspecific(ds, (ds)->lock_state_key, &ls); \
     ((lock_state_t *)ls)->write_lock_state = STORE_WRITE_LOCKED; \
     DCE_SVC_DEBUG((cds__svc_handle, \
		   cds_svc_gdb, \
		   svc_c_debug2, \
		   " <GDB_WRITE_LOCK(%p)", ds)); \
    }                                                          
                                                               
#define GDB_WRITE_UNLOCK(ds) { \
     pthread_addr_t ls; \
     DCE_SVC_DEBUG((cds__svc_handle, \
		   cds_svc_gdb, \
		   svc_c_debug2, \
		   " >GDB_WRITE_UNLOCK(%p)", ds)); \
     getspecific(ds, (ds)->lock_state_key, &ls);\
     ((lock_state_t *)ls)->write_lock_state = STORE_NOT_WRITER; \
     pthread_mutex_lock(&(ds)->control.lock); \
     (ds)->control.nwriters = (ds)->control.nwriters - 1; \
     (ds)->control.writerp = NULL; \
     (ds)->control.write_in_process = FALSE; \
     pthread_mutex_unlock(&(ds)->control.lock); \
     pthread_cond_broadcast(&(ds)->control.out); \
     DCE_SVC_DEBUG((cds__svc_handle, \
		    cds_svc_gdb, \
		    svc_c_debug2, \
		   " <GDB_WRITE_UNLOCK(%p)", ds)); \
}

#endif /* _GDB_DB44_H_ */
