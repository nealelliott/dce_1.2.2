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
 * $Log: gdb_db44.c,v $
 * Revision 1.1.2.1  1996/10/03  20:24:28  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/03  19:19:27  arvind]
 *
 * Revision /main/DCE_1.2.2/1  1996/09/09  21:22 UTC  arvind
 * 	PKSS drop from DEC (DCE1.2.2)
 * 	[1996/08/30  15:36 UTC  arvind  /main/arvind_pkss/1]
 * 
 * 	Remove CDS server references (CDS compare routines, server.h, etc.).
 * 	[1996/07/10  18:48:00  zee]
 * 	 *
 * 
 * 	Remove dependency on dthread macros.  Use pthread calls
 * 	directly
 * 	[1996/07/08  20:05:15  farrell]
 * 
 * 	updates for generalization of comparison and debug routines
 * 	[1996/06/28  17:06:35  farrell]
 * 
 * 	GLORIA (R1.2.2) revision.
 * 	[1996/06/24  20:47:38  zee]
 * 
 * $EndLog$
 */
/*
 * Module: gdb_db44.c
 *     Generic DataBase (GDB) interface definition for the modified
 *     UCB DB44.
 */


#ifdef GDB_DEBUG
#define CHECK_IN_TXN(x) if (!(x)->temporary) dce_assert(cds__svc_handle,((x)->txn == TRUE));
#else
#define CHECK_IN_TXN(x)
#endif

#include <dce/dce.h>
#include <dce/assert.h>
#include <dcecdssvc.h>
#include <dcecdsmsg.h>
#include <dcecdsmac.h>

#include <gdb_db44.h>
#include <dns_record.h>  /* For ALIGN and COPY macros */

/*
 * Prototypes
 */
static int
assemble_entry(stored_entry_t **,
	       size_t *,
	       size_t, char *,
	       size_t, char *);
static int
disassemble_entry(stored_entry_t *, 
		  size_t *, char **,
		  size_t *, char **); 
static
int lookup_context (storage_handle_t,
		    char **,
		    unsigned int *,
		    char **,
		    unsigned int *,
		    gdb_context_t *,
		    index_t,
		    int);

static void 
cleanup_locks (pthread_addr_t);


/*
 * Pre-defined display routines for database keys:
 *
 * Users can define their own or use one of ours.
 * Currently we just have a noop.  It allows the user
 * to evade the default.
 */
void gdb_display_noop (int size1, char *keyp) {
}

/*
 * assemble_entry
 * 
 * Assemble db44 entry that includes a secondary key
 *
 * Input: 
 *    db44_entry_pp = address for put address of database
 *                    buffer
 *    db44_entry_size = address to put size of resultiing
 *                      database buffer
 *    key2_size = size in bytes of secondary key
 *    key2_p = address of secondary key
 *    gdb_entry_size = size in bytes of entry
 *    gdb_entry_p = address of entry data
 *
 * Returns: CDS_GDB_SUCCESS, CDS_GDB_ERROR
 */
static int
assemble_entry(stored_entry_t **db44_entry_pp, 
	       size_t *db44_entry_size_p,
	       size_t key2_size, char *key2_p,
	       size_t gdb_entry_size, char *gdb_entry_p) 
{
    size_t *buf_key2_size_p;
    char *buf_key2_p;


    /*
     * Calculate size of buffer we'll need
     */
    *db44_entry_size_p = key2_size + ALIGN_LEN(gdb_entry_size) 
      + 2 * ALIGN_LEN(sizeof(size_t));

    /*
     * Allocate space to assemble our entry.  Caller must free it 
     * when done with it.
     */
    *db44_entry_pp = malloc (*db44_entry_size_p);
    if (*db44_entry_pp == NULL)
      return(CDS_GDB_ERROR);

    /*
     * Move the entry and size of entry into the db44 entry buffer.
     */
    (*db44_entry_pp)->has_key.data_size = gdb_entry_size;
    COPY_bytes( gdb_entry_p, (*db44_entry_pp)->has_key.data, gdb_entry_size );

    /*
     * Move the key size into the buffer.
     */
    buf_key2_size_p = (size_t *)ALIGN_PTR((*db44_entry_pp)->has_key.data + 
					  gdb_entry_size);
    *buf_key2_size_p = key2_size;

    /*
     * Move the key into the buffer.
     */
    buf_key2_p = (char *)buf_key2_size_p + sizeof(*buf_key2_size_p);
    COPY_bytes( key2_p, buf_key2_p, key2_size );

#ifdef DEBUG
    {
	size_t real_size;
	/*
	 * Calculate size of buffer
	 */
	real_size = (buf_key2_p - (char *)*db44_entry_pp) + key2_size;
	if (real_size > *db44_entry_size_p) {
	    fprintf(stderr, "real_size = %d, estimated = %d\n", 
		    real_size, *db44_entry_size_p);
	}
    }
#endif

    return(CDS_GDB_SUCCESS);
}

/*
 * disassemble_entry
 *
 * Disassemble db44 entry that includes a secondary key.
 * Caller must free *gdb_entry_pp and *key2_pp when done,
 * as well as the original entry.
 *
 * Input:
 *    db44_entry_p = The entry from db44 to disassemble
 *    key2_size_p  = NULL if don't need key2 returned, or 
 *                   address to put size of key
 *    key2_pp      = an address to put the entry address
 *                   (if we return CDS_GDB_SUCCESS, the caller
 *                   must free this region)
 *    gdb_entry_size_p = NULL if don't need entry returned, or 
 *                       an address to put the size of entry
 *    gdb_entry_pp = an address to put the entry address
 *                   (if we return CDS_GDB_SUCCESS, the caller
		      *                   must free this region)
		      * 
		      * Returns: CDS_GDB_SUCCESS, CDS_GDB_ERROR
		      */
static int
disassemble_entry(stored_entry_t *db44_entry_p, 
		  size_t *key2_size_p, char **key2_pp,
		  size_t *gdb_entry_size_p, char **gdb_entry_pp) 
{
    size_t *buf_key2_size_p;
    char *buf_key2_p;
   
    /*
     * Take the entry and size of entry from the db44 entry buffer
     */
    if (gdb_entry_size_p != NULL) {
	*gdb_entry_size_p = db44_entry_p->has_key.data_size;
	*gdb_entry_pp = malloc(*gdb_entry_size_p);
	if (*gdb_entry_pp == NULL) {
	    return CDS_GDB_ERROR;
	}
	COPY_bytes( db44_entry_p->has_key.data, *gdb_entry_pp, *gdb_entry_size_p );
    }
    /*
     * Take the key and size of key from the db44 entry buffer
     */
    if (key2_size_p != NULL) {
	buf_key2_size_p = (size_t *)((char *)db44_entry_p + 
				     sizeof(size_t) + 
				     ALIGN_LEN(db44_entry_p->has_key.data_size));
	*key2_size_p = *buf_key2_size_p;

	*key2_pp = malloc(*key2_size_p);
	if (*key2_pp == NULL) {
	    return CDS_GDB_ERROR;
	}
	buf_key2_p = (char *)buf_key2_size_p + sizeof(size_t);
	COPY_bytes( buf_key2_p, *key2_pp, *key2_size_p);
    }

    return(CDS_GDB_SUCCESS);
}

/*
 * cleanup_locks
 */
static void 
cleanup_locks(pthread_addr_t ls) {
    
    DCE_SVC_DEBUG((cds__svc_handle,
		   cds_svc_gdb,
		   svc_c_debug2,
		   " >cleanup_locks(%p) read locks:%d, write:%d", 
		   ls, ((lock_state_t *)ls)->read_lock_depth,
		   ((lock_state_t *)ls)->write_lock_state));

    for (; 
	 ((lock_state_t *)ls)->read_lock_depth > 0;
	 ((lock_state_t *)ls)->read_lock_depth--) {
	GDB_READ_UNLOCK(((lock_state_t *)ls)->store_p);
    }
    if (((lock_state_t *)ls)->write_lock_state == STORE_WANNA_WRITE) {
	GDB_WRITE_UNLOCK(((lock_state_t *)ls)->store_p);
    }
    if (((lock_state_t *)ls)->write_lock_state == STORE_WRITE_LOCKED) {
	gdb_txn_rollback((storage_handle_t *)(((lock_state_t *)ls)->store_p));
    }
    free(ls);

    DCE_SVC_DEBUG((cds__svc_handle,
		   cds_svc_gdb,
		   svc_c_debug2,
		   " <cleanup_locks(%p)", ls));
}


/*
 * gdb_key_free
 * gdb_entry_free
 *
 * Input: 
 *     store = database storage handle
 *     p = address of gdb entry as given to caller
 *         from gdb_entry_lookup_*()
 *
 * Returns:
 *     nothing
 */
#ifdef GDB_DEBUG
void
gdb_entry_free (storage_handle_t store, char *p) {
   
    DCE_SVC_DEBUG((
		   cds__svc_handle,
		   cds_svc_gdb,
		   svc_c_debug2,
		   " >gdb_free(%p)", p));
   
    free (p);
   
    DCE_SVC_DEBUG((cds__svc_handle,
		   cds_svc_gdb,
		   svc_c_debug2,
		   " <gdb_free"));
}
void
gdb_key_free (storage_handle_t store, char *p) {
   
    DCE_SVC_DEBUG((
		   cds__svc_handle,
		   cds_svc_gdb,
		   svc_c_debug2,
		   " >gdb_free(%p)", p));
   
    free (p);
   
    DCE_SVC_DEBUG((cds__svc_handle,
		   cds_svc_gdb,
		   svc_c_debug2,
		   " <gdb_free"));
}
#endif

/*
 * gdb_storage_open
 */
int 
gdb_storage_open (storage_handle_t *store,
		  use_t             use,
		  char             *filename,
		  unsigned int      cachesize,
		  compare_func_t    compare,
		  display_func_t    key_display,
		  display_func_t    data_display) {
    
    store_info_t *s;
    DB *tree;
    BTREEINFO info;
    char *fname;
    
    
    DCE_SVC_DEBUG((
		   cds__svc_handle,
		   cds_svc_gdb,
		   svc_c_debug2,
		   " >gdb_storage_open"));
        
    /*
     * Setup input to __bt_open():
     *    - fname if this is a permanent database
     *    - cache size ??? get this from config info
     */
    if (use == USE_PERMANENT)
      fname = filename;
    else 
      fname = NULL;
    
    bzero((char *)&info,sizeof(info));
    /*
     * BUFSIZ should be defined in stdio.h, and hopefully
     * is equal to the page size (or some multiple thereof).
     */
    if (cachesize == 0) {
        info.cachesize = DEFAULT_CACHE_SIZE_BLOCKS * BUFSIZ;
    }
    else {
        info.cachesize = cachesize;
    }
    info.compare = compare;
    info.key_display = key_display;
    info.data_display = data_display;

    /*
     * Open the database, creating it if necessary.  If it already
     * exists on disk, that will be used.
     *
     * Use DB_LOCK to indicate that on processing bt_seq() & bt_get()  
     * we want db44 to copy the entries into specially allocated 
     * buffers that we can read and write and will free when done.  
     * Otherwise, we would have to make our own writable copies.
     */
    tree = __bt_open(filename, (O_RDWR|O_CREAT), 0666, &info, DB_LOCK);
    if (tree == NULL) {
	return CDS_GDB_ERROR;
    }
    
    /*
     * Now build our descriptor for this database
     */
    s = malloc (sizeof(store_info_t));
    if (s == NULL) {
	/* -- delete the tree first */
	return CDS_GDB_ERROR;
    }
    s->tree = tree;
    
#ifdef GDB_DEBUG
    /*
     * Clear txn nesting check
     */
    s->txn = FALSE;
    if (use == USE_PERMANENT)
      s->temporary = FALSE;
    else
      s->temporary = TRUE;
#endif 
    
    /*
     * Remember we don't have a secondary index yet, but
     * store the primary key display routine in case we
     * need it for the secondary index
     */
    s->tree2 = NULL; 
    s->primary_key_display = key_display;

    /*
     * Initialize access control
     */
    if (pthread_mutex_init (&(s->control.lock), 
			    pthread_mutexattr_default) != 0) {
#ifdef DEBUG      
	perror("pthread_create_mutex");
#endif
	return CDS_GDB_ERROR;
    }
    if (pthread_cond_init (&(s->control.out),
			   pthread_condattr_default) != 0) {
#ifdef DEBUG      
	perror ("pthread_cond_init");
#endif
	return CDS_GDB_ERROR;
    }

    s->control.nwriters = s->control.nreaders = 0;
    s->control.writerp = NULL;
    s->control.write_in_process = FALSE;

    /*
     * Create keys for thread specific data to keep track
     * of locks, and whose destructor routines will cleanup
     * after threads that die holding locks.
     */
    pthread_keycreate(&s->lock_state_key, cleanup_locks);

    /*
     * opaque handle for the caller 
     */
    *store = s;

    DCE_SVC_DEBUG((
		   cds__svc_handle,
		   cds_svc_gdb,
		   svc_c_debug2,
		   " <gdb_storage_open"));
   
    return CDS_GDB_SUCCESS;
}

/*
 * gdb_storage_create_index
 *
 * Create a secondary index on the specified database. 
 *
 * With db44, the secondary index is implemented as a separate
 * database which maps secondary index to primary index.
 *
 * Input:
 *     store = storage handle for existing database
 *   compare = comparision routine provided for 
 * cachesize = cache size in bytes
 *
 * Returns:
 *    CDS_GDB_SUCCESS for success,
 *    CDS_GDB_SECONDARY if a secondary index already exists for
 *                      this store
 *    CDS_GDB_ERROR if unable to create the new database for the 
 *                  secondary index.
 */
int 
gdb_storage_create_index (storage_handle_t store,
			  unsigned int     cachesize,
			  compare_func_t   compare,
                          display_func_t   key_display)
{
   store_info_t *s;
   BTREEINFO info;


   DCE_SVC_DEBUG((cds__svc_handle,
		  cds_svc_gdb,
		  svc_c_debug2,
		  " >gdb_storage_create_index"));
   
   s = (store_info_t *)store;

   /*
    * Error if we already have a secondary index 
    */
#ifdef DEBUG
   if (s->tree2 != NULL) {
      return CDS_GDB_SECONDARY;
   }
#endif

   /*
    * Specify desired cache size
    */
   bzero((char *)&info,sizeof(info));
   if (cachesize == 0) {
       info.cachesize = DEFAULT_SECONDARY_CACHE_SIZE_BLOCKS * BUFSIZ;
   }
   else {
       info.cachesize = cachesize;
   }
   info.compare = compare;
   info.key_display = key_display;
   info.data_display = s->primary_key_display;/* primary key is our data! */

   /*
    * Open the database, creating it if necessary.  If it already
    * exists on disk, that will be used.
    *
    * Use DB_LOCK to indicate that we want writable copies of the
    * data when we do a bt_get(), or bt_seq()
    *
    * Using a null filename here is cheating because we know that
    * we only use a secondary index on the in-memory db.  Find
    * a better way! :-)  ???? 
    */
   s->tree2 = __bt_open(NULL, (O_RDWR|O_CREAT), 0666, &info, DB_LOCK);
   if (s->tree2 == NULL) {
      return CDS_GDB_ERROR;
   }

   DCE_SVC_DEBUG((
		  cds__svc_handle,
		  cds_svc_gdb,
		  svc_c_debug2,
		  " <gdb_storage_create_index"));
   
   return CDS_GDB_SUCCESS;
}

/*
 * gdb_storage_delete 
 *
 * Delete the whole store, including on-disk image, if any.
 *
 * Input:
 *      store - storage handle for store 
 *   filename - filename of on-disk image.
 */
int 
gdb_storage_delete (storage_handle_t  store,
		    const char       *filename)
{
    store_info_t *s = (store_info_t *)store;

    DCE_SVC_DEBUG((
		   cds__svc_handle,
		   cds_svc_gdb,
		   svc_c_debug2,
		   " >gdb_storage_delete(%p)",
		   s));   

    GDB_WANNA_WRITE(s);
    GDB_WRITE_LOCK(s);
    if (s->tree) {
	__bt_close_and_delete(s->tree, filename);
    }
    if (s->tree2) {
	__bt_close_and_delete(s->tree2, filename);
    }
    GDB_WRITE_UNLOCK(s);

    /*
     * Now free memory used for the handle itself
     */
    free (store);
   
    DCE_SVC_DEBUG((cds__svc_handle,
		   cds_svc_gdb,
		   svc_c_debug2,
		   " <gdb_storage_delete() returns CDS_GDB_SUCCESS"));

    return CDS_GDB_SUCCESS;
}


/*
 * gdb_storage_close 
 *
 * Close the whole database.  If the caller says now, then
 * cancel any threads in process, otherwise wait patiently
 * to acquire the write_lock
 */
int
gdb_storage_close (storage_handle_t store, when_t when)
{
    store_info_t *s = (store_info_t *)store;
    DB *tree;
    int status = CDS_GDB_SUCCESS;

    DCE_SVC_DEBUG((cds__svc_handle,
		   cds_svc_gdb,
		   svc_c_debug2,
		   " >gdb_storage_close(%p,WHEN_DONE)", s));
   
    if (when == WHEN_DONE) {
	GDB_WANNA_WRITE(s);
	GDB_WRITE_LOCK(s);
	if (s->tree) {
	    __bt_close(s->tree);
	}
	if (s->tree2) {
	    __bt_close(s->tree2);
	}
	GDB_WRITE_UNLOCK(s);
    }
    else {
	/*
	 * Cancel threads in process of accessing the
	 * database

	 ??? Shouldn't this be done at a higher level, it
	 isn't really database work and shouldn't be in GDBI!!!!
	 
	 */
	status = CDS_GDB_ERROR;
    }
    
    /*
     * Destroy mutex & condition variable used for access control
     */
    if (pthread_mutex_destroy (&(s->control.lock)) != 0) {
	status = CDS_GDB_ERROR;
    }
    if (pthread_cond_destroy (&(s->control.out)) != 0) {
	status = CDS_GDB_ERROR;
    }
    
    /*
     * Now free memory used for the handle itself
     */
    free (store);
    
    DCE_SVC_DEBUG((cds__svc_handle,
		   cds_svc_gdb,
		   svc_c_debug2,
		   " <gdb_storage_close returns CDS_GDB_SUCCESS"));
    
    return status;
}

/*
 * gdb_storage_dump
 */
int 
gdb_storage_dump (storage_handle_t store)
{
    store_info_t *s = (store_info_t *)store;
    DB *tree = s->tree;
    
    __bt_stat (tree);

    return CDS_GDB_SUCCESS;
}

/*
 * gdb_storage_verify
 */
int 
gdb_storage_verify (storage_handle_t store) {

    return CDS_GDB_SUCCESS;
}


/*
 * gdb_entry_insert
 *
 * Returns:
 *   CDS_GDB_SUCCESS - success
 *   CDS_GDB_EXISTS - key was found in database
 *   CDS_GDB_NO_SECONDARY - secondary index was not found in database
 *   CDS_GDB_ERROR - other error
 */
int 
gdb_entry_insert (storage_handle_t store,
		  char *entry_p,
		  unsigned int entry_size,
		  char *key_p,
		  unsigned int key_size,
		  char *key2_p,
		  unsigned int key2_size) {
    DB *tree;
    DBT key, key2, data, buffer_key2_p;
    store_info_t *s;
    stored_entry_t *db_entry_p;
    int status;
    int flags = R_NOOVERWRITE;		/* if we already have an entry for either
					   key, we don't want to replace it      */


    DCE_SVC_DEBUG((cds__svc_handle,
		   cds_svc_gdb,
		   svc_c_debug2,
		   " >gdb_entry_insert"));
   
    s = (store_info_t *)store;
    tree = s->tree;

    CHECK_IN_TXN(s);

    /*
     * Check parameters to some extent 
     *
     * ???? Waste of time?
     */
    if (key_p == NULL || entry_p == NULL)
      return CDS_GDB_ERROR;
    if ((s->tree2 != NULL && key2_p == NULL) || 
	(s->tree2 == NULL && key2_p != NULL)) 
      return CDS_GDB_NO_SECONDARY;
   
    /*
     * Put the key in the descriptor db44 wants (DBT)
     */
    key.size = (size_t)key_size;
    key.data = (char *)key_p;
   
    /*
     * Put the data in the format we want it to be stored by db44.  This
     * must include the secondary key for db's with a secondary index.
     * Then put together the descriptor db44 wants for it (DBT).
     */
    if (s->tree2 == NULL) {
	data.data = entry_p;


	data.size = (size_t)entry_size;
    }
    else {
	/*
	 * Build the entry for db44 with key2
	 */
	status = assemble_entry((stored_entry_t **)&data.data,
				&data.size,
				key2_size,
				key2_p,
				entry_size,
				entry_p);
	if (status != CDS_GDB_SUCCESS)
	  return(CDS_GDB_ERROR);
    }
    
    /*
     * The write lock is taken at the last possible time because we
     * hate to block out other readers and writers longer than we have to.
     * We may need to wait here, if others are busy.
     */
    GDB_WRITE_LOCK(s);
    
    /*
     * Put the data in the db44 database using the primary index
     */
    status = __bt_put ( tree, &key, &data, flags );
    
    if (data.data != entry_p)		/* Free memory if we created a special buffer */
      free(data.data);
    
    if (status == RET_SPECIAL) {
	return CDS_GDB_EXISTS;
    }   
    if (status != RET_SUCCESS) {
	return CDS_GDB_ERROR;
    }
    
    /*
     * If there's a secondary index, put it in there too.  This 
     * time the primary key is our data.
     */
    if (s->tree2 != NULL) {
	key2.size = (size_t)key2_size;
	key2.data = (char *)key2_p;
      
	status = __bt_put (s->tree2, &key2, &key, flags );
	if (status != RET_SUCCESS) {
	    __bt_delete (tree, &key, 0); /* Remove entry made for primary key */
	    if (status == RET_SPECIAL) 
	      return CDS_GDB_EXISTS;
	    else
	      return CDS_GDB_ERROR;
	}
    }

    DCE_SVC_DEBUG((cds__svc_handle,
		   cds_svc_gdb,
		   svc_c_debug2,
		   " <gdb_entry_insert() returns CDS_GDB_SUCCESS"));

    return CDS_GDB_SUCCESS;
}


/*
 * gdb_entry_delete
 *
 *
 *
 * Returns:
 *   CDS_GDB_SUCCESS - success
 *   CDS_GDB_NO_SECONDARY - no entry
 *   CDS_GDB_ERROR - other error 
 */
int 
gdb_entry_delete (storage_handle_t store,
		  char *key_p,
		  unsigned int key_size,
		  index_t index) {

   DBT key, key2, data;
   store_info_t *s;
   stored_entry_t *db_entry_p;
   int status;


   DCE_SVC_DEBUG((cds__svc_handle,
		  cds_svc_gdb,
		  svc_c_debug2,
		  " >gdb_entry_delete"));

   s = (store_info_t *)store;

   CHECK_IN_TXN(s);

   if (index == KEY_PRIMARY) {
      /*
       * We've been given the primary key.  We're responsible for
       * deleting the entry in the secondary index also if there is
       * a one.
       */
       key.size = (size_t)key_size;
       key.data = (char *)key_p;
       
       /*
	* There is a secondary tree.  Delete that entry first.
	*/
       if (s->tree2 != NULL) {
	   status =__bt_get (s->tree, &key, &data, 0);
	   if (status != RET_SUCCESS) {
	       if (status == RET_SPECIAL) {
		   status = CDS_GDB_NOT_FOUND;
	       }
	       else {
		   status = CDS_GDB_ERROR;
	       }
	       DCE_SVC_DEBUG((cds__svc_handle,
			      cds_svc_gdb,
			      svc_c_debug2,
			      " <gdb_entry_delete() returns %d, local=1", status));
	       return status;
	   }

	   disassemble_entry ((stored_entry_t *)data.data,
			      &key2.size,
			      (char **)&key2.data,
			      NULL,
			      NULL);
	   free(data.data);
	   if (status != CDS_GDB_SUCCESS) {
	       DCE_SVC_DEBUG((cds__svc_handle,
			      cds_svc_gdb,
			      svc_c_debug2,
			      " <gdb_entry_delete() returns %d, local=2", status));
	       return status;
	   }

	   GDB_WRITE_LOCK(s);
	 
	   /*
	    * Delete the entry from the secondary tree
	    */
	   status = __bt_delete ( s->tree2, &key2, 0 );
	   free (key2.data);
	   if (status != RET_SUCCESS) {
	       DCE_SVC_DEBUG((cds__svc_handle,
			      cds_svc_gdb,
			      svc_c_debug2,
			      " <gdb_entry_delete() returns CDS_GDB_ERROR, local=3"));
	       return CDS_GDB_ERROR;
	   }
       }
       /*
	* Delete the entry from the primary tree 
	*/
       status = __bt_delete ( s->tree, &key, 0 );
       if (status != RET_SUCCESS) {
	   DCE_SVC_DEBUG((cds__svc_handle,
			  cds_svc_gdb,
			  svc_c_debug2,
			  " <gdb_entry_delete() returns CDS_GDB_ERROR, local=4"));
	   return CDS_GDB_ERROR;
       }
       
   }
   else if (index == KEY_SECONDARY && s->tree2 != NULL) {
       /*
	* We've been given the secondary key and there is such an index 
	*/
       key2.size = (size_t)key_size;
       key2.data = (char *)key_p;
   
       /*
	* Get the primary key out of the secondary index tree (it's
	* the data).  Delete the entry in the primary tree, and
	* the secondary tree.
	*/
       if ((status =__bt_get (s->tree2, &key2, &key, 0)) != RET_SUCCESS) {
	   if (status == RET_SPECIAL) {
	       status = CDS_GDB_NOT_FOUND;
	   }
	   else {
	       status = CDS_GDB_ERROR;
	   }
	   DCE_SVC_DEBUG((cds__svc_handle,
			  cds_svc_gdb,
			  svc_c_debug2,
			  " <gdb_entry_delete() returns %d, local=5", status));
	   return status;
       }

       GDB_WRITE_LOCK(s);

       /*
	* Delete the entry in the primary tree
	*/
       status = __bt_delete ( s->tree, &key, 0 );
       free (key.data);
       if (status != RET_SUCCESS) {
	   DCE_SVC_DEBUG((cds__svc_handle,
			  cds_svc_gdb,
			  svc_c_debug2,
			  " <gdb_entry_delete() returns CDS_GDB_ERROR, local=6"));
	   return CDS_GDB_ERROR;
       }

       /*
	* Delete the entry in the secondary tree
	*/
       status = __bt_delete ( s->tree2, &key2, 0 );
       if (status != RET_SUCCESS) {
	   DCE_SVC_DEBUG((cds__svc_handle,
			  cds_svc_gdb,
			  svc_c_debug2,
			  " <gdb_entry_delete() returns CDS_GDB_ERROR, local=7"));
	   return CDS_GDB_ERROR;
       }

   }
   else if (index == KEY_SECONDARY) {
       /*
	* We've been given the secondary key and there's no
	* such index
	*/
       DCE_SVC_DEBUG((cds__svc_handle,
		      cds_svc_gdb,
		      svc_c_debug2,
		      " <gdb_entry_delete() returns CDS_GDB_NO_SECONDARY"));
       return CDS_GDB_NO_SECONDARY;
   }
   
   
   DCE_SVC_DEBUG((cds__svc_handle,
		  cds_svc_gdb,
		  svc_c_debug2,
		  " <gdb_entry_delete() returns CDS_GDB_SUCCESS"));
   
   return CDS_GDB_SUCCESS;
}


/*
 * gdb_entry_lookup_next
 */
int 
gdb_entry_lookup_next (storage_handle_t store,
		       gdb_context_t *context_p,
		       char **entry_pp,
		       unsigned int *entry_size_p,
		       char **key_pp,
		       unsigned int *key_size_p,
		       index_t index) {
    
    DCE_SVC_DEBUG((cds__svc_handle,
		   cds_svc_gdb,
		   svc_c_debug2,
		   " >gdb_entry_lookup_next() calling lookup_context"));
    
    return( lookup_context(store, entry_pp, entry_size_p,
			   key_pp, key_size_p, 
			   context_p, index, R_NEXT) );
}

/*
 * gdb_entry_lookup_first
 */
int 
gdb_entry_lookup_first (storage_handle_t store,
			char **entry_pp,
			unsigned int *entry_size_p,
			char **key_pp,
			unsigned int *key_size_p,
			gdb_context_t *context_p, 
			index_t index) {
    
    
    DCE_SVC_DEBUG((cds__svc_handle,
		   cds_svc_gdb,
		   svc_c_debug2,
		   " >gdb_entry_lookup_first() calling lookup_context"));
    
    return( lookup_context(store, entry_pp, entry_size_p,
			   key_pp, key_size_p, 
			   context_p, index, R_FIRST) );
}
/*
 * gdb_entry_lookup 
 */
int 
gdb_entry_lookup (storage_handle_t store,
		  char **entry_pp,
		  unsigned int *entry_size_p,
		  char *key_p,
		  unsigned int key_size,
		  gdb_context_t *context_p, 
		  index_t index) {
    
    char *tmp_key_p = key_p;
    unsigned int tmp_key_size = key_size;
    
    
    DCE_SVC_DEBUG((cds__svc_handle,
		   cds_svc_gdb,
		   svc_c_debug2,
		   " >gdb_entry_lookup() calling lookup_context"));
    
    return( lookup_context(store, entry_pp, entry_size_p,
			   &tmp_key_p, &tmp_key_size, 
			   context_p, index, R_ECURSOR) );
}


/*
 * keys_equal - bytewise compare two database keys
 *
 * Returns: 
 *     TRUE if keys are equal
 *     FALSE otherwise
 */
static int 
keys_equal(char *key1_p, int key1_size, char *key2_p, int key2_size) {
    
    if (key1_size != key2_size) return FALSE;

    return ( memcmp(key1_p, key2_p, key1_size)==0 ? TRUE : FALSE );
}


/*
 * gdb_entry_lookup_after
 *
 *
 * Caller gives us a key (that may or may not truly be in the database), 
 * we return the entry and its key that follows it in the database (or 
 * would if it were in the database).  
 *
 * Here, we do this in two steps, first we position the database cursor 
 * with a lookup of the key, then we do a "get next" call.
 * -- This badly needs optimization! (???)
 */
int 
gdb_entry_lookup_after (storage_handle_t store,
			char *after_key_p,
			unsigned int after_key_size,
			char **entry_pp,
			unsigned int *entry_size_p,
			char **key_pp,
			unsigned int *key_size_p,
			gdb_context_t *context_p,
			index_t index) 
{    
    char *tmp_key_p = after_key_p;	/* tmp so don't overwrite caller's */
    unsigned int tmp_key_size = after_key_size; /* " */
    int status;


    DCE_SVC_DEBUG((cds__svc_handle,
		   cds_svc_gdb,
		   svc_c_debug2,
		   " >gdb_entry_lookup_after(%p,,,,,,,,)",store));

    /*
     * First lookup the key given.  If it doesn't exist, get the  
     * one after it.  This will position the cursor properly for
     * subsequent lookups.
     */
    status = lookup_context(store, entry_pp, entry_size_p,
			    &tmp_key_p, &tmp_key_size, 
			    context_p, index, R_CURSOR);
    if (status != CDS_GDB_SUCCESS) {
	goto leave_gdb_entry_lookup_after;
    }

    if (keys_equal(after_key_p, after_key_size, tmp_key_p, tmp_key_size)) {
	/*
	 * We read in the entry for the key given by the caller.
	 * Free this buffer, the lookup set the cursor properly to
	 * read the next entry, so do that.
	 */
	gdb_entry_free(store, *entry_pp);
	
	/*
	 * Lookup the following key and return that info
	 */
	status = lookup_context(store, entry_pp, entry_size_p, key_pp, 
				key_size_p, context_p, index, R_NEXT);
    }
    else {
	*key_pp = tmp_key_p;
	*key_size_p = tmp_key_size;
    }

  leave_gdb_entry_lookup_after:

    DCE_SVC_DEBUG((cds__svc_handle,
		   cds_svc_gdb,
		   svc_c_debug2,
		   " <gdb_entry_lookup_after() returns %d",status));
		 
    return status;
}

/*
 * lookup_context
 *
 * Common code for gdb_entry_lookup_[next|first|after]
 *
 * Input:
 *   how = R_CURSOR | R_NEXT | R_FIRST | R_ECURSOR
 *         R_ECURSOR--> get specified key, and set cursor
 *         R_CURSOR --> get the matching key, or one that would 
 *                      follow it and set cursor
 *         R_NEXT   --> get next key using cursor
 *         R_FIRST  --> get first key in database, and set cursor
 *
 * Returns:
 *   CDS_GDB_SUCCESS - success
 *   CDS_GDB_NOT_FOUND - end of database was found
 *   CDS_GDB_ERROR - other error
 */
static int 
lookup_context (storage_handle_t store,
		char **entry_pp,
		unsigned int *entry_size_p,
		char **key_pp,
		unsigned int *key_size_p,
		gdb_context_t *context_p,
		index_t index,
		int how) 
{
    DBT key, key2, data;
    int status;
    store_info_t *s;
    char *db44_data_p;
    stored_entry_t *db_entry_p;
    int local = 0;

    
    DCE_SVC_DEBUG((cds__svc_handle,
		   cds_svc_gdb,
		   svc_c_debug2,
		   " >lookup_context"));
    
    /*
     * Get the input so we can use it 
     */
    s = (store_info_t *)store;
    
    /*
     * Get read lock and do the lookup based on which key has been
     * given.
     */
    GDB_READ_LOCK(s);
    
    if (index == KEY_PRIMARY) {

	local = 2;

	key.size = (size_t)*key_size_p;
	key.data = *key_pp;
   
	/*
	 * We've been given the primary key, so look the data up
	 * and leave a cursor in place.
	 */
	status = __bt_seq ( s->tree, &key, &data, how );
	if (status != RET_SUCCESS) {   
	    if (status == RET_SPECIAL) 
	      status = CDS_GDB_NOT_FOUND;
	    else 
	      status = CDS_GDB_ERROR;
	    goto leave_lookup_context;
	}
      
	/*
	 * Return the key info to the caller in case it didn't have
	 * them (like on a call for the next entry)
	 */
	*key_size_p = (unsigned int)key.size;
	*key_pp = key.data;

	if (s->tree2 == NULL) {
	    *entry_pp = data.data;
	    *entry_size_p = (unsigned int)data.size;
	}
	else {
	    size_t tmp_size;
	 
	    /*
	     * If this tree does have a secondary index, then the alternate
	     * format for the data buffer was stored in db44 (stored_entry_t).  
	     * We find the actual caller data following its size in the buffer.
	     */
	    status = disassemble_entry ((stored_entry_t *)data.data,
					NULL, NULL,
					&tmp_size, entry_pp);
	    free (data.data);
	    if (status != CDS_GDB_SUCCESS) {
		goto leave_lookup_context;
	    }
	    *entry_size_p = (unsigned int)tmp_size;
	}

    } 
    else if (index == KEY_SECONDARY && s->tree2 != NULL) {
	size_t tmp_key_size, tmp_data_size;

	local = 3;

	/*
	 * We've been given the secondary key and there is such an index 
	 */
	key2.size = (size_t)*key_size_p;
	key2.data = *key_pp;
   
	/*
	 * Get the primary key out of the secondary index tree (it's
	 * the data).  Don't need to leave the cursor there.
	 */
	if ((status = __bt_seq (s->tree2, &key2, &key, how)) != RET_SUCCESS) {
	    if (status == RET_SPECIAL) 
	      status = CDS_GDB_NOT_FOUND;
	    else 
	      status = CDS_GDB_ERROR;
	    goto leave_lookup_context;
	}

	local = 4;

	/*
	 * Now, get the data from the primary tree.  
	 */
	status = __bt_get (s->tree, &key, &data, 0);
	free (key.data);
	if (status != RET_SUCCESS) {
	    if (status == RET_SPECIAL) 
	      status = CDS_GDB_NOT_FOUND;
	    else 
	      status = CDS_GDB_ERROR;
	    goto leave_lookup_context;
	}

	local = 5;

	/*
	 * Because this tree has a secondary index, then the alternative
	 * format for the data buffer was stored in db44.  We find the 
	 * actual caller data following its size in the buffer.
	 */
	status = disassemble_entry ((stored_entry_t *)(data.data),
				    &tmp_key_size, key_pp,
				    &tmp_data_size, entry_pp);
	free(data.data);
	if (status != CDS_GDB_SUCCESS) {
	    goto leave_lookup_context;
	}
	*key_size_p = (unsigned int)tmp_key_size;
	*entry_size_p = (unsigned int)tmp_data_size;
    }
    else if (index == KEY_SECONDARY) {
	status = CDS_GDB_NO_SECONDARY;
	goto leave_lookup_context;
    }
    
    /*
     * Successful exit, either CDS_GDB_SUCCESS or CDS_GDB_NOT_EXACT
     */
    status = CDS_GDB_SUCCESS;

    
  leave_lookup_context:
    
    GDB_READ_UNLOCK(s);
    
    DCE_SVC_DEBUG((cds__svc_handle,
		   cds_svc_gdb,
		   svc_c_debug2,
		   " <lookup_context() returns %ld, local = %d", status, local));
    
    return status;
}

/*
 * gdb_entry_update 
 *
 *
 *
 * Returns:
 *   CDS_GDB_SUCCESS - success
 *   CDS_GDB_NOT_FOUND - key was not found in database
 *   CDS_GDB_ERROR - other error
 */
int 
gdb_entry_update (storage_handle_t store,
		  char *entry_p,
		  unsigned int entry_size,
		  char *key_p,
		  unsigned int key_size,
		  index_t index) {
    
    DBT key, key2, data;
    store_info_t *s;
    stored_entry_t *db_entry_p;
    stored_entry_t *new_db_entry_p =NULL, /* Used to remember addresses      */
    *new_db_key_p =NULL;		/*   we need to free at the end of */
    /*   the procedure                 */
    int status;
    int flags = 0;			/* Indicates to __bt_put() that it should overwrite the
					   entry found at the given key                         */
    int local = 0;
    
    
    DCE_SVC_DEBUG((cds__svc_handle,
		   cds_svc_gdb,
		   svc_c_debug2,
		   " >gdb_entry_update(%p,%p,%d,%p,%d,%d)",
		   store, entry_p, entry_size, key_p, key_size, index ));
    
    s = (store_info_t *)store;
    
    CHECK_IN_TXN(s);
    
    if (index == KEY_PRIMARY) {

	local = 1;

	/*
	 * We've been given the primary key.  
	 */
	key.size = (size_t)key_size;
	key.data = (char *)key_p;
	if ((status =__bt_get (s->tree, &key, &data, 0)) != RET_SUCCESS) {
	    if (status == RET_SPECIAL) {
		status = CDS_GDB_NOT_FOUND;
	    }
	    else {
		status = CDS_GDB_ERROR;
	    }
	    goto leave_gdb_entry_update;
	}
   
	local = 2;
       
	/*
	 * There is a secondary tree
	 */
	if (s->tree2 != NULL) {
	    status = disassemble_entry((stored_entry_t *)data.data, 
				       &key2.size,
				       (char **)&key2.data,
				       NULL,
				       NULL);
	    free (data.data);
	    if (status != CDS_GDB_SUCCESS) {
		goto leave_gdb_entry_update;
	    }

	    local = 3;

	    /*
	     * Build the new updated entry 
	     */
	    status = assemble_entry((stored_entry_t **)&data.data,
				    &data.size,
				    key2.size,
				    key2.data,
				    entry_size,
				    entry_p); 
	    free(key2.data);
	    if (status != CDS_GDB_SUCCESS) {
		goto leave_gdb_entry_update;
	    }
	    new_db_entry_p = data.data;
	} 
	else {
	    free (data.data);
	    data.size = (size_t)entry_size;
	    data.data = entry_p;
	}
    }
    else if (index == KEY_SECONDARY && s->tree2 != NULL) {

	local = 4;

	/*
	 * We've been given the secondary key and there is such an index 
	 */
	key2.size = (size_t)key_size;
	key2.data = (char *)key_p;
   
	/*
	 * Get the primary key out of the secondary index tree (it's
	 * the data).  We'll update the entry in the primary tree, only.
	 */
	if ((status = __bt_get (s->tree2, &key2, &key, 0)) != RET_SUCCESS) {
	    if (status == RET_SPECIAL) {
		status = CDS_GDB_NOT_FOUND;
	    }
	    else {
		status = CDS_GDB_ERROR;
	    }
	    goto leave_gdb_entry_update;
	}

	local = 5;
      
	/*
	 * Build the new updated entry 
	 */
	status = assemble_entry((stored_entry_t **)&data.data,
				&data.size,
				key2.size,
				key2.data,
				entry_size,
				entry_p); 
	if (status != CDS_GDB_SUCCESS) {
	    goto leave_gdb_entry_update;
	}
	new_db_entry_p = data.data;
	new_db_key_p = key.data;
    }
    else if (index == KEY_SECONDARY) {

	local = 6;
       
	/*
	 * We've been given the secondary key and there's no
	 * such index
	 */
	status = CDS_GDB_NO_SECONDARY;
	goto leave_gdb_entry_update;
    }
    
    /*
     * The write lock is taken at the last possible time because we
     * hate to block out other readers and writers longer than we have to.
     * We may need to wait here, if others are busy.
     */
    local = 7;
    
    GDB_WRITE_LOCK(s);
    
    /*
     * Put the new data in the primary db44 database 
     */
    status = __bt_put ( s->tree, &key, &data, 0 );
    {
	/*
	 * free temporary buffers 
	 */
	if (new_db_entry_p) free (new_db_entry_p);
	if (new_db_key_p) free (new_db_key_p);
       
	/*
	 * Check status from __bt_put()
	 */
	if (status != RET_SUCCESS) {
	    status = CDS_GDB_ERROR;
	    goto leave_gdb_entry_update;
	}
    }
    
    status = CDS_GDB_SUCCESS;
    
  leave_gdb_entry_update:
    
    DCE_SVC_DEBUG((cds__svc_handle,
		   cds_svc_gdb,
		   svc_c_debug2,
		   " <gdb_entry_update() returns %d, local = %d", status, local));
    
    return status;
}

/*
 * gdb_txn_begin
 *
 * If this doesn't work, we're in trouble.  Any errors
 * are fatal.
 */
void 
gdb_txn_begin (storage_handle_t store) {

    store_info_t *s = (store_info_t *)store;
    int status = RET_SUCCESS;

    DCE_SVC_DEBUG((cds__svc_handle,
		   cds_svc_gdb,
		   svc_c_debug2,
		   " >gdb_txn_begin(%p)", s));

    /*
     * Take locks as needed at this time
     */
    GDB_WANNA_WRITE(s);

#ifdef GDB_DEBUG
    /*
     * Check to be sure we aren't trying to nest
     */
    dce_assert(cds__svc_handle, (s->txn == FALSE));
    s->txn = TRUE;
#endif 

    /*
     * If this is a permanent database, begin
     * the transaction at all levels.  Temporary
     * databases only need to have locks taken above,
     * because recovery mechanisms are not in place 
     * (DB44 only).
     */
    if (!s->temporary) {
	/*
	 * Have locks, begin transaction 
	 */
	status = bt_txn_begin(s->tree);
	if (status == RET_SUCCESS && s->tree2 != NULL)
	  status = bt_txn_begin(s->tree2);
    }

    if (status != RET_SUCCESS) {
	dce_svc_printf(CDS_S_GDB_BEGIN_XTN_MSG, (long)status);
    }
       
    DCE_SVC_DEBUG((cds__svc_handle,
		   cds_svc_gdb,
		   svc_c_debug2,
		   " <gdb_txn_begin() returns successfully"));
}

/*
 * gdb_txn_commit
 *
 * Note(!!!) that there is not automicity between the
 * commit of the two indexes.  We can get away with this 
 * only by restricting the use of the secondary to temporary
 * databases.
 */
void
gdb_txn_commit (storage_handle_t store) {

    store_info_t *s = (store_info_t *)store;
    int status = RET_SUCCESS;

    DCE_SVC_DEBUG((cds__svc_handle,
		   cds_svc_gdb,
		   svc_c_debug2,
		   " >gdb_txn_commit(%p)", s));

#ifdef GDB_DEBUG
    /*
     * Check to be sure we aren't trying to nest
     */
    dce_assert(cds__svc_handle, (s->txn == TRUE));
    s->txn = FALSE;
#endif 

    /*
     * For permanent databases, commit the transaction
     * to disk.  Temporary databases have no such mechanism
     * and call this routine only for its locking function.
     */
    if (!s->temporary) {
	/*
	 * Commit the transaction
	 */
	status = bt_txn_commit(s->tree);
	if (status == RET_SUCCESS && s->tree2 != NULL)
	  status = bt_txn_commit(s->tree2);
    }

    /*
     * If we didn't succeed, fatal error
     */
    if (status != RET_SUCCESS) {
	dce_svc_printf(CDS_S_GDB_COMMIT_XTN_MSG, (long)status);
    }

    /*
     * Release locks
     */
    GDB_WRITE_UNLOCK(s);

    DCE_SVC_DEBUG((cds__svc_handle,
		   cds_svc_gdb,
		   svc_c_debug2,
		   " <gdb_txn_commit() returns successfully"));

}

/*
 * gdb_txn_rollback
 */
void
gdb_txn_rollback (storage_handle_t store) {

    store_info_t *s = (store_info_t *)store;
    int status= RET_SUCCESS;


    DCE_SVC_DEBUG((cds__svc_handle,
		   cds_svc_gdb,
		   svc_c_debug2,
		   " >gdb_txn_rollback(%p)", s));

#ifdef GDB_DEBUG
    /*
     * Check to be sure we aren't trying to nest
     */
    dce_assert(cds__svc_handle, (s->txn == TRUE));
    s->txn = FALSE;
#endif 

    /*
     * For permanent databases, rollback the transaction
     * on disk.  Temporary databases have no such mechanism
     * and call this routine only for its locking function.
     */
    if (!s->temporary) {
	/*
	 * Rollback the transaction
	 */
	status = bt_txn_rollback(s->tree);
	if (status == RET_SUCCESS && s->tree2 != NULL)
	  status = bt_txn_rollback(s->tree2);
    }       
   
    /*
     * If we didn't succeed, fatal error
     */
    if (status != RET_SUCCESS) {
	dce_svc_printf(CDS_S_GDB_ROLLBACK_XTN_MSG, (long)status);
    }

    /*
     * Release locks
     */
    GDB_WRITE_UNLOCK(s);

    DCE_SVC_DEBUG((cds__svc_handle,
		   cds_svc_gdb,
		   svc_c_debug2,
		   " <gdb_txn_rollback() returns successfully"));

}
