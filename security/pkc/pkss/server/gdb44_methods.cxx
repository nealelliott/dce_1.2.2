//
// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// All Rights Reserved
//

// HISTORY
// $Log: gdb44_methods.cxx,v $
// Revision 1.1.2.1  1996/10/04  14:20:55  arvind
// 	OSF DCE 1.2.2 Drop 4
// 	[1996/10/04  13:34:26  arvind]
//
// Revision /main/DCE_1.2.2/1  1996/09/09  21:28 UTC  arvind
// 	     PKSS drop from DEC (DCE1.2.2)
// 	     [1996/08/30  15:47 UTC  arvind  /main/arvind_pkss/1]
// 
// $EndLog$

//
// (c) Copyright 1996,
// Digital Equipment Corporation, Maynard, Massachusetts, USA
// All Rights Reserved
//
// Member functions for G-L-O-R-I-A class
//

#include "gdb44_methods.h"
#include "sec_pkss_util.h"
#include <dce/dce_msg.h>
#include <dce/dcecdsmsg.h>
#include <dce/dcepksmsg.h>
#include <dce/dcepkssvc.h>  
#include <dce/dcepksmac.h> 
#include <string.h>

//
// Gdb44C default key comparison function.  Since it is passed as a
// parameter to the constructor, it is not a member function.
//
// Sort comparison function take arguments
// (int key1_size, char *key1_p, int key2_size, char *key2_p) 
//  and returns:
//  >0 if key1 is "bigger" than key2, 
//   0 if equal, 
//  <0 if key1 is "smaller" than key2
//
// This is the defined behavior for a gdb44 key comparison function.  Note
// in the implementation that the keys are memcmp'ed to the minimum of their
// key lengths, then further compared by key length if required.
//

#ifndef MIN
#define MIN (x,y) (((x)<(y))?(x):(y))
#endif

int gdb44_default_key_cmp
(
  int key1_len,
  char *key1_p,
  int key2_len,
  char *key2_p
)

{
  int cmp_val = memcmp (key1_p, key2_p, MIN (key1_len, key2_len));

  if (cmp_val == 0)
    if (key1_len > key2_len)
      cmp_val = 1;
    else
      if (key1_len < key2_len)
        cmp_val = -1;

  return (cmp_val);

} // End gdb44_default_key_cmp


//
// Gdb44C constructor
//
// Opens database file given by filename_p and stores a database
// handle among the private data.
//
// Inputs:
//
// filename_p: a pointer to a character string containing the pathname
//             of the database to be opened.
//
// when:  Initializer for whenToShutdown member value; when closing the
//	  database, specifies whether to abort transactions in progress
//	  and shutdown immediately or allow transactions to finish before
//	  closing.  May be one of WHEN_NOW or WHEN_DONE.  Default is
//	  WHEN_DONE.
//
// use:  specifies whether to use disk ("permanent") storage or memory
//	 ("temporary") storage.  May be one of USE_PERMANENT or USE_TEMPORARY.
//	 Default is USE_PERMANENT.
//
// key_compare: a pointer to a key comparison function whose behavior gdb44
//		assumes will be as described in the comments for
//		gdb44_default_key_cmp (see above).  Default is
//		gdb44_default_key_cmp
//
// cache_size: specifies the size in bytes of the gdb44 cache.  Default is 0
//	       which to gdb44 means 1000*BUFSIZ
//
// key_display: a pointer to a function that dumps the contents of a database
//		key.  Default is gdb_display_noop, that is, no display
//		function is supplied.
//
// data_display: a pointer to a function that dumps the contents of a database
//		 record.  Default is gdb_display_noop, that is, no display
//		 function is supplied.
//
// outputs:
//
// estatus: an exit status value
//

Gdb44C::Gdb44C
(
  char *filename_p,
  when_t when,
  use_t use,
  int (*key_compare)(int, char *, int, char *),
  unsigned int cache_size,
  void (*key_display)(int, char *),
  void (*data_display)(int, char *)
)

{

  cstatus = 0;

  if (filename_p == 0)
  {
    dce_svc_printf(PKS_S_FILENAME_MSG);
    cstatus = -1;
  }

  else
  {
    //
    // Initialize db shutdown flag.
    //

    whenToShutdown = when;

    //
    // Attempt to open database.
    //

    cstatus = gdb_storage_open (&handle,
				use,
				filename_p,
				cache_size,
				key_compare,
				key_display,
				data_display);

    if (cstatus == CDS_GDB_SUCCESS)
      cstatus = 0;
    else
    {
      cstatus = -1;
      dce_svc_printf(PKS_S_CANT_OPEN_DATABASE_MSG, filename_p, cstatus);
    }

  } // End else

} // End Gdb44C::Gdb44C


//
// Gdb44C destructor
//
// Closes database given by handle.  Default whenToShutdown = WHEN_NOW.
//
 
Gdb44C::~Gdb44C ()
{
  error_status_t estatus = 0;

  estatus = gdb_storage_close (handle, whenToShutdown);

  if (estatus != CDS_GDB_SUCCESS)
  {
    dce_svc_printf(PKS_S_STORAGE_CLOSE_MSG, estatus);
  }

} // End Gdb44C::~Gdb44C


//
// Read an arbitrary database record.
//
// Inputs:
//
// key_p:  a pointer to the primary key for the record to lookup.
//
// key_len:  the length of the key given by key_p.
//
// key_type:  one of KEY_PRIMARY or KEY_SECONDARY.
//
// entry_p:  a pointer to the record returned by the read.  This record's
//           memory is allocated by GDB and must be explicitly freed by
//           the caller.
//
// entry_len_p:  a pointer to the length of the data given by entry_p.
//
// context_p: a pointer to a lookup context (optional; default is 0)
//

error_status_t Gdb44C::read
(
  char **entry_p,
  unsigned32 *entry_len_p,
  char *key_p,
  unsigned32 key_len,
  index_t key_type,
  gdb_context_t *context_p
)

{
  error_status_t estatus = 0;
  unsigned int loc_entry_len;

  if ((key_type != KEY_PRIMARY) &&
      (key_type != KEY_SECONDARY))
  {
    // error
  }

  else
  {
    estatus = gdb_entry_lookup (handle,
			        entry_p,
			        &loc_entry_len,
			        key_p,
			        key_len,
			        context_p,
			        key_type);

    if (estatus == CDS_GDB_SUCCESS) {
      estatus = 0;
      *entry_len_p = loc_entry_len;
    };
  }

  //
  // Exit this function.
  //

  return (estatus);

} // End Gdb44C::read


//
// Read an arbitrary database record using its primary key.
//
// Inputs:
//
// key_p:  a pointer to the primary key for the record to lookup.
//
// key_len:  the length of the key given by key_p.
//
// entry_p:  a pointer to the record returned by the read.  This record's
//           memory is allocated by GDB and must be explicitly freed by
//           the caller.
//
// entry_len_p:  a pointer to the length of the data given by entry_p.
//
// context_p: a pointer to a lookup context (optional; default is 0)
//

error_status_t Gdb44C::read
(
  char **entry_p,
  unsigned32 *entry_len_p,
  char *key_p,
  unsigned32 key_len,
  gdb_context_t *context_p
)

{
  error_status_t estatus = 0;
  unsigned int loc_entry_len;
  estatus = gdb_entry_lookup (handle,
			      entry_p,
			      &loc_entry_len,
			      key_p,
			      key_len,
			      context_p,
			      KEY_PRIMARY);

  if (estatus == CDS_GDB_SUCCESS) {
    estatus = 0;
    *entry_len_p = loc_entry_len;
  }
  
  //
  // Exit this function.
  //

  return (estatus);

} // End Gdb44C::read


//
// Read the first record after the record given by lkp_key_p (primary key).
//
// Inputs:
//
// lkp_key_p:  a pointer to the primary key for the record to lookup.
//
// lkp_key_len:  the length of the key given by lkp_key_p.
//
// ret_key_p:  a pointer to key returned from lookup.
//
// ret_key_len:  the length of the key given by ret_key_p.
//
// entry_p:  a pointer to the record returned by the read.  This record's
//           memory is allocated by GDB and must be explicitly freed by
//           the caller.
//
// entry_len_p:  a pointer to the length of the data given by entry_p.
//
// context_p: a pointer to a lookup context (optional; default is 0)
//

error_status_t Gdb44C::read
(
  char *lkp_key_p,
  unsigned32 lkp_key_len,
  char **entry_p,
  unsigned32 *entry_len_p,
  char **ret_key_p,
  unsigned32 *ret_key_len_p,
  gdb_context_t *context_p
)

{
  error_status_t estatus = 0;
  unsigned int loc_entry_len;
  unsigned int loc_key_len;
  estatus = gdb_entry_lookup_after (handle,
				    lkp_key_p,
				    lkp_key_len,
				    entry_p,
				    &loc_entry_len,
				    ret_key_p,
				    &loc_key_len,
				    context_p,
				    KEY_PRIMARY);
  if (estatus == CDS_GDB_SUCCESS) {
    estatus = 0;

    *entry_len_p = loc_entry_len;
    *ret_key_len_p = loc_key_len;
  }

  //
  // Exit this function.
  //

  return (estatus);

} // End Gdb44C::read


//
// Read the next record given by the context (primary key only).
//
// Inputs:
//
// ret_key_p:  a pointer to key returned from lookup.
//
// ret_key_len:  the length of the key given by lkp_key_p.
//
// entry_p:  a pointer to the record returned by the read.  This record's
//           memory is allocated by GDB and must be explicitly freed by
//           the caller.
//
// entry_len_p:  a pointer to the length of the data given by entry_p.
//
// context_p: a pointer to a lookup context (optional; default is 0)
//

error_status_t Gdb44C::read
(
  char **entry_p,
  unsigned32 *entry_len_p,
  char **ret_key_p,
  unsigned32 *ret_key_len_p,
  gdb_context_t *context_p
)

{
  error_status_t estatus = 0;
  unsigned int loc_key_len;
  unsigned int loc_entry_len;
  estatus = gdb_entry_lookup_next (handle,
				   context_p,
				   entry_p,
				   &loc_entry_len,
				   ret_key_p,
				   &loc_key_len,
				   KEY_PRIMARY);
  if (estatus == CDS_GDB_SUCCESS) {
    estatus = 0;
    *entry_len_p = loc_entry_len;
    *ret_key_len_p = loc_key_len;
  };
  //
  // Exit this function.
  //

  return (estatus);

} // End Gdb44C::read


//
// Insert a record into the database with a primary key only.
//
// Inputs:
//
// key_p:  a pointer to the primary key for the record given by entry_p.
//
// key_len:  the length of the key given by key_p.
//
// entry_p:  a pointer to the record to insert into the database.
//
// entry_len:  the length of the data given by entry_p.
//

error_status_t Gdb44C::insert
(
  char *entry_p,
  unsigned32 entry_len,
  char *key_p,
  unsigned32 key_len
)

{
  error_status_t estatus = 0;
  estatus = gdb_entry_insert (handle,
			      entry_p,
			      entry_len,
			      key_p,
			      key_len,
			      0, 0);

  if (estatus == CDS_GDB_SUCCESS)
    estatus = 0;

  //
  // Exit this function.
  //

  return (estatus);

} // End Gdb44C::insert


//
// Write a record into the database with a primary key only.  This function
// will attempt to do a gdb_storage_insert.  If gdb_storage_insert fails with
// CDS_GDB_EXISTS, then attempt the same operation with gdb_storage_update.
//
// Inputs:
//
// key_p:  a pointer to the primary key for the record given by entry_p.
//
// key_len:  the length of the key given by key_p.
//
// entry_p:  a pointer to the record to write into the database.
//
// entry_len:  the length of the data given by entry_p.
//

error_status_t Gdb44C::write
(
  char *entry_p,
  unsigned32 entry_len,
  char *key_p,
  unsigned32 key_len
)

{
  error_status_t estatus = 0;
  estatus = gdb_entry_insert (handle,
			      entry_p,
			      entry_len,
			      key_p,
			      key_len,
			      0, 0);

  if (estatus == CDS_GDB_EXISTS)
    estatus = gdb_entry_update (handle,
			        entry_p,
			        entry_len,
			        key_p,
			        key_len,
			        KEY_PRIMARY);

  if (estatus == CDS_GDB_SUCCESS)
    estatus = 0;

  //
  // Exit this function.
  //

  return (estatus);

} // End Gdb44C::write


//
// Insert a record into the database with a primary and a secondary key.
//
// Inputs:
//
// key_p:  a pointer to the primary key for the record given by entry_p.
//
// key_len:  the length of the key given by key_p.
//
// key2_p:  a pointer to a secondary key for the record given by entry_p.
//
// key2_len:  the length of the key given by key2_p.
//
// entry_p:  a pointer to the record to insert into the database.
//
// entry_len:  the length of the data given by entry_p.
//

error_status_t Gdb44C::insert
(
  char *entry_p,
  unsigned32 entry_len,
  char *key_p,
  unsigned32 key_len,
  char *key2_p,
  unsigned32 key2_len
)

{
  error_status_t estatus = 0;
  estatus = gdb_entry_insert (handle,
			      entry_p,
			      entry_len,
			      key_p,
			      key_len,
			      key2_p,
			      key2_len);

  if (estatus == CDS_GDB_SUCCESS)
    estatus = 0;

  //
  // Exit this function.
  //

  return (estatus);

} // End Gdb44C::insert


//
// Safely insert a record into the database with the primary key only.
//
// Inputs:
//
// key_p:  a pointer to the key for the record given by entry_p.
//
// key_len:  the length of the key given by key_p.
//
// entry_p:  a pointer to the record to insert into the database.
//
// entry_len:  the length of the data given by entry_p.
//

error_status_t Gdb44C::safe_insert
(
  char *entry_p,
  unsigned32 entry_len,
  char *key_p,
  unsigned32 key_len
)

{
  error_status_t estatus = 0;
  gdb_txn_begin (handle);

  estatus = gdb_entry_insert (handle,
			      entry_p,
			      entry_len,
			      key_p,
			      key_len,
			      0, 0);

  if (estatus == CDS_GDB_SUCCESS)
    estatus = 0;

  if (estatus)
    gdb_txn_rollback (handle);
  else
    gdb_txn_commit (handle);

  //
  // Exit this function.
  //

  return (estatus);

} // End Gdb44C::safe_insert


//
// Safely insert a record into the database with a primary and a secondary key.
//
// Inputs:
//
// key_p:  a pointer to the primary key for the record given by entry_p.
//
// key_len:  the length of the key given by key_p.
//
// key2_p:  a pointer to the secondary key for the record given by entry_p.
//
// key2_len:  the length of the key given by key2_p.
//
// entry_p:  a pointer to the record to insert into the database.
//
// entry_len:  the length of the data given by entry_p.
//

error_status_t Gdb44C::safe_insert
(
  char *entry_p,
  unsigned32 entry_len,
  char *key_p,
  unsigned32 key_len,
  char *key2_p,
  unsigned32 key2_len
)

{
  error_status_t estatus = 0;
  gdb_txn_begin (handle);

  estatus = gdb_entry_insert (handle,
			      entry_p,
			      entry_len,
			      key_p,
			      key_len,
			      key2_p,
			      key2_len);

  if (estatus == CDS_GDB_SUCCESS)
    estatus = 0;

  if (estatus)
    gdb_txn_rollback (handle);
  else
    gdb_txn_commit (handle);

  //
  // Exit this function.
  //

  return (estatus);

} // End Gdb44C::safe_insert


//
// Safely write a record into the database with a primary key only.  This
// function will attempt to do a gdb_storage_insert.  If gdb_storage_insert
// fails with CDS_GDB_EXISTS, then attempt the same operation with
// gdb_storage_update.
//
// Inputs:
//
// key_p:  a pointer to the primary key for the record given by entry_p.
//
// key_len:  the length of the key given by key_p.
//
// entry_p:  a pointer to the record to write into the database.
//
// entry_len:  the length of the data given by entry_p.
//

error_status_t Gdb44C::safe_write
(
  char *entry_p,
  unsigned32 entry_len,
  char *key_p,
  unsigned32 key_len
)

{
  error_status_t estatus = 0;
  gdb_txn_begin (handle);

  estatus = gdb_entry_insert (handle,
			      entry_p,
			      entry_len,
			      key_p,
			      key_len,
			      0, 0);

  if (estatus == CDS_GDB_EXISTS)
    estatus = gdb_entry_update (handle,
			        entry_p,
			        entry_len,
			        key_p,
			        key_len,
			        KEY_PRIMARY);

  if (estatus == CDS_GDB_SUCCESS)
    estatus = 0;

  if (estatus)
    gdb_txn_rollback (handle);
  else
    gdb_txn_commit (handle);

  //
  // Exit this function.
  //

  return (estatus);

} // End Gdb44C::safe_write

//
// Remove (that is, delete) a GDB record using primary key.
//
// Inputs:
//
// key_p:  a pointer to the primary key for the record given by entry_p.
//
// key_len:  the length of the key given by key_p.
//

error_status_t Gdb44C::remove
(
  char *key_p,
  unsigned32 key_len
)

{
  error_status_t estatus = 0;
  estatus = gdb_entry_delete (handle,
			      key_p,
			      key_len,
			      KEY_PRIMARY);

  if (estatus == CDS_GDB_SUCCESS)
    estatus = 0;

  return (estatus);

} // End Gdb44C::delete


//
// Initiate a GDB transaction.
//

void Gdb44C::txnBegin()
{
  gdb_txn_begin (handle);

} // End Gdb44C::txnBegin


//
// Commit a GDB transaction.
//

void Gdb44C::txnEnd()
{
  gdb_txn_commit (handle);

} // End Gdb44C::txnEnd


//
// Rollback a GDB transaction.
//

void Gdb44C::txnCancel()
{
  gdb_txn_rollback (handle);

} // End Gdb44C::txnCancel


