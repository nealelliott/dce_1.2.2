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
 * $Log: context_manager.c,v $
 * Revision 1.1.7.3  1996/02/18  18:25:56  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:48:10  marty]
 *
 * Revision 1.1.7.2  1995/12/08  20:54:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  20:49:31  root]
 * 
 * Revision 1.1.2.2  1994/10/27  20:06:24  rrizzo
 * 	expand copyright
 * 	[1994/10/27  19:38:47  rrizzo]
 * 
 * Revision 1.1.2.1  1994/10/21  19:09:16  weir
 * 	Submit sample sources (CR 12635)
 * 	[1994/10/21  19:08:09  weir]
 * 
 * $EndLog$
 */
/****************************************************************************/
/* [27.VI.94]                                                               */
/*                                                                          */
/* context_manager.c -- implementation of "store" interface.                */
/*                                                                          */
/*                                                                          */
/*   The server maintains a certain number of storage areas, only one of    */
/*    which can be (or should be) opened by a single client at a time.      */
/*    More than one client can, however, apparently be invoked (up to the   */
/*    number of separate storelets == store handles available, defined by   */
/*    the value of NUM_STORELETS). Each client keeps track of its store     */
/*    (and likewise enables the server to do the same) by means of the con- */
/*    text handle it receives when it opens its store.                      */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/*                            -77 cols-                                     */
/****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <pthread.h>
#include <dce/dce_error.h>
#include <dce/daclif.h>

#include "context.h"

#define NUM_STORELETS 10

/****************************************************************************/
/* The actual "storelet" structure...                                       */

typedef struct store_hdr{
	pthread_mutex_t ref_lock;
	unsigned32 size;
	unsigned32 refcount;
	idl_byte *storage;
} store_hdr_t;

store_hdr_t headers[NUM_STORELETS]; /* There's an array of these.           */

/****************************************************************************/
/* The store specification structure; note that it is equivalent to the     */
/*  handle; the pointer to it is returned as the handle by the store_open() */
/*  routine below...                                                        */
/*  The assumption is that all access to a given handle is serialized in a  */
/*  single thread, so no locking is needed for these.                       */

typedef struct store_spec{
	unsigned32 number;     /* The storelet number we've opened.         */
	unsigned32 offset;     /* The current read/write position.          */
} store_spec_t;  /* There is only one of these; it's the handle that gives  */
                 /*  access to one of the NUM_STORELETS set of "storelets". */


/* The server entry name:                                                   */
extern unsigned_char_p_t entry;


/* Initialization control block:                                            */
pthread_once_t init_once_blk = pthread_once_init;




/******
*
*
* store_mgmt_init -- Zeroes out all the storelet structures; executed only
*                    once per server instance, as soon as a client has
*                    called the store_open() routine.
*
*
******/
/****************************************************************************/
void 
store_mgmt_init(
)
{
	int i;
	store_hdr_t *hdr;

	fprintf(stdout, "Store Manager: Initializing Store\n");
	memset(headers, 0, sizeof(store_hdr_t) * NUM_STORELETS);
	for (i = 0; i < NUM_STORELETS; i++)
	{
		hdr = headers + i;
		pthread_mutex_init(
			(pthread_mutex_t *)hdr,
			pthread_mutexattr_default);
	}

}





/******
*
*
* store_open -- Opens a store and returns a handle to it. The store consists
*               of one "storelet" selected from the array of NUM_STORELETS.
*
*
******/
/****************************************************************************/
void 
store_open(
	handle_t binding,
	unsigned32 store_size,    /* Size specified for actual storage.     */
	store_handle_t *store_h,  /* To return the store handle in.         */
	error_status_t *status
)
{
	int i;                 /* Index variable.                           */
	store_spec_t *spec;    /* Store specification == handle.            */
	store_hdr_t *hdr;      /* Storelet structure.                       */

	/* Do the store initialization if this is the first open call...    */
	/* Zero out the store headers...                                    */
	pthread_once(&init_once_blk, store_mgmt_init);

	/* The following loop goes through all the storelets, looking for   */
	/*  one whose reference count is zero. As soon as one such is       */
	/*  found, a handle is allocated for it, storage is allocated for   */
	/*  its store structure, and the loop (and the call) terminates. If */
	/*  no unreferenced storelet is found, a status of -1 is returned   */
	/*  and no handle is allocated...                                   */
	for(i = 0; i < NUM_STORELETS; i++)
	{
		/* Go to the next storelet...                               */
		hdr = headers + i;

		/* Is it unreferenced?...                                   */
		if (hdr->refcount == 0)
		{
			/* If so, lock the header...                        */
			*status = pthread_mutex_lock((pthread_mutex_t *)hdr);
			if (*status != 0)
			{
				return;
			}

			/* ...and check the reference count again...        */
			if (hdr->refcount == 0)
			{
				/* Now we know we "really" have this one.   */
				/* Only one open is allowed, so lock only   */
				/*  the reference count...                  */
				hdr->refcount++;

				/* Now unlock the header so other threads   */
				/*  can continue to check it...             */
				*status = pthread_mutex_unlock((pthread_mutex_t *)hdr);
				if (*status != 0)
					return;

				/* Now allocate space for the specifica-    */
				/*  tion structure...                       */
				spec = (store_spec_t *)malloc(sizeof(store_spec_t));
				spec->number = i;
				spec->offset = 0;
				*store_h = spec;

				/* Allocate space for the storage part of   */
				/*  the header...                           */
				hdr->storage = (idl_byte *)malloc(store_size);
				hdr->size = store_size;

				/* Finally, set the return status to OK,    */
				/*  and return...                           */
				*status = error_status_ok;
				return;
			}

			/* If the reference count turned out to have        */
			/*  been accessed between our first check and our   */
			/*  locking the mutex, we must now unlock the mutex */
			/*  preparatory to looping around to check the next */
			/*  storelet...                                     */
			*status = pthread_mutex_unlock((pthread_mutex_t *)hdr);
			if (*status != 0)
			{
				return;
			}
		}
	}

	/* The following is reached only if we never found a free           */
	/*  storelet...                                                     */
	*store_h = NULL;
	*status = -1;

}




/******
*
*
* store_set_ptr -- Insert a new value into the store buffer pointer.
*
*
*******/
/****************************************************************************/
void store_set_ptr(
	store_handle_t store_h,     /* The store handle.                    */
	unsigned32 offset,    /* Value to insert into store buffer pointer. */
	error_status_t *status
)
{
	store_spec_t *spec;              /* Our pointer to store handle.    */

	spec = (store_spec_t *)store_h;  /* Get the store spec.             */
	spec->offset = offset;     /* Copy in the new buffer pointer value. */
	*status = error_status_ok;
}



/******
*
*
* store_close -- Close the opened storelet.
*
*
******/
/****************************************************************************/
void 
store_close(
	store_handle_t *store_h,                /* Store handle.            */
	error_status_t *status
)
{
	store_spec_t *spec;             /* Our pointer to store handle.     */
	store_hdr_t *hdr;               /* Pointer to a storelet.           */

	printf("Store Manager: Closing Store\n");

	spec = (store_spec_t *)*store_h; /* Get the store spec.             */
	hdr = headers + spec->number;    /* Point to the correct storelet.  */

	/* If the thing is actually opened, close it...                     */
	if (hdr->refcount > 0)
	{
		/* Lock the header first...                                 */
		*status = pthread_mutex_lock((pthread_mutex_t *)hdr);
		if (*status != 0)
		{
			printf("Close: lock failed\n");
			return;
		}

		/* Check the reference count to make sure no one slipped in */
		/*  before we could lock the header, and already closed the */
		/*  critter...                                              */
		if (hdr->refcount > 0)
		{
			/* The store is open, and it's locked by us, so we  */
			/*  can safely close it. So do it. First, decrement */
			/*  the reference count...                          */
			hdr->refcount--;

			/* Is it completely closed now?                     */
			if (hdr->refcount == 0)
			{
				/* If so, get rid of its storage space...   */
				hdr->size = 0;
				free(hdr->storage);
			}
		}

		/* If the store turned out to be closed before we could do  */
		/*  close it, we have nothing to do but release the lock... */
		*status = pthread_mutex_unlock((pthread_mutex_t *)hdr);
		if (*status != 0)
		{
			printf("Close: unlock failed\n");
			return;
		}
	}

	/* And free our handle space...                                     */
	free(spec);

	/* Be sure to NULL the context handle.  Otherwise, the context      */
	/*  will be considered to be live as long as the client is run-     */
	/*  ning...                                                         */
	*store_h = NULL; 
	*status = error_status_ok;
}



/******
*
*
* store_read -- Read a certain number of bytes from the opened store.
*
*
******/
/****************************************************************************/
void
store_read(
	store_handle_t store_h,   /* Store handle.                          */
	unsigned32 buf_size,      /* Number of bytes to read.               */
	store_buf_t buffer,       /* Space to return data read in.          */
	unsigned32 *data_size,    /* To return number of bytes read in.     */
	error_status_t *status
)
{
	store_spec_t *spec;       /* Our handle pointer.                    */
	store_hdr_t *hdr;         /* Pointer to a storelet.                 */

	spec = (store_spec_t *)store_h;  /* Get the storelet spec.          */
	hdr = headers + spec->number;    /* Point to the correct storelet.  */

	/* If the amount we're to read is less than the amount left to be   */
	/*  read, then read it...                                           */
	if (buf_size <= hdr->size)
	{

		/* Copy bytes from the storelet storage, beginning at off-  */
		/*  set, into the return buffer, up to the size of the      */
		/*  buffer...                                               */
		memcpy(buffer, hdr->storage + spec->offset, buf_size);

		/* Update the storelet buffer pointer past what we've just  */
		/*  read...                                                 */
		spec->offset += buf_size;

		/* Show return size of data read...                         */
		*data_size = buf_size;
		*status = error_status_ok;
		return;
	}

	/* If there's less data left than has been specified to read, don't */
	/*  read it...                                                      */
	*data_size = 0;
	*status = -1;
}


/******
*
*
* store_write -- Write some data into the opened store.
*
*
******/

void 
store_write(
	/* handle_t IDL_handle,*/  /* If the server ACF declares            */
					/*  [explicit_handle]               */
	store_handle_t store_h,    /* Store handle.                         */
	unsigned32 buf_size,       /* Number of bytes to write.             */
	store_buf_t buffer,        /* Data to be written.                   */
	unsigned32 *data_size,     /* To return number of bytes written.    */
	error_status_t *status
)
{
	store_spec_t *spec;            /* Our pointer to store handle.      */
	store_hdr_t *hdr;              /* Pointer to a storelet.            */

	/* Do an access check on IDL_handle here...                         */
	/* [--ORIGINAL NOTE] -- I don't know what the above means.          */

	spec = (store_spec_t *)store_h;  /* Get the storelet spec.          */
	hdr = headers + spec->number;    /* Point to the correct storelet.  */

	/* If the amount of unused room left in the storelet is greater     */
	/*  than what we're supposed to write in it, write it...            */
	if ((hdr->size - spec->offset) > buf_size)
	{

		/* Copy bytes from the buffer into the storelet storage,    */
		/*  beginning at the current read/write position...         */
		memcpy(hdr->storage + spec->offset, buffer, buf_size);

		/* Update the storelet buffer pointer to point past what    */
		/*  we've just written...                                   */
		spec->offset += buf_size;

		/* Add a null in case we want to read the store as a        */
		/*  string...                                               */
		*(hdr->storage + spec->offset) = 0; 

		/* Show return size of data written...                      */
		*data_size = buf_size;
		*status = error_status_ok;
		return;
	}

	/* If we don't have romm to write the whole buffer, don't write     */
	/*  anything...                                                     */
	*data_size = 0;
	*status = error_status_ok;
}


/******
*
*
* store_handle_t_rundown -- Closes the opened storelet.
*
*
******/
/****************************************************************************/
void
store_handle_t_rundown(
	store_handle_t store_h
)
{
	error_status_t st;

	printf("Store Manager: Running down context.\n");
	store_close(&store_h, &st);
}



/******
 *
 * print_manager_error-- Manager version. Prints text associated with bad status code.
 *
 *
 *
 ******/
void
print_manager_error(
char *caller,  /* String identifying the routine that received the error.   */
error_status_t status) /* the status we want to print the message for.      */
{
	dce_error_string_t error_string;
	int print_status;

	dce_error_inq_text(status, error_string, &print_status);
	fprintf(stderr," Manager: %s: %s\n", caller, error_string);

}

