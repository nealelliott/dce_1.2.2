//
// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// All Rights Reserved
//
// HISTORY
// $Log: gdb44_methods.h,v $
// Revision 1.1.2.1  1996/10/04  14:21:09  arvind
// 	OSF DCE 1.2.2 Drop 4
// 	[1996/10/04  13:34:39  arvind]
//
// Revision /main/DCE_1.2.2/1  1996/09/09  21:28 UTC  arvind
// 	PKSS drop (2) from DEC (DCE1.2.2)
// 	[1996/09/04  14:25 UTC  arvind  /main/arvind_pkss/2]
// 
// $EndLog$
//
// (c) Copyright 1996,
// Digital Equipment Corporation, Maynard, Massachusetts, USA
// All Rights Reserved
//
// Class definitions for G-L-O-R-I-A.
//

#ifndef GDB_METHODS_H
#define GDB_METHODS_H


extern "C"
{
#include <dce/nbase.h>
#include <pthread.h>
#include "gdb.h"
}

//
// Default key comparison function
//

int gdb44_default_key_cmp (int, char *, int, char *);

class Gdb44C
{
public:
  Gdb44C
  (
    char *,
    when_t = WHEN_DONE,
    use_t = USE_PERMANENT,
    int (*key_compare)(int, char *, int, char *) = gdb44_default_key_cmp,
    unsigned int = 0, // 0 is gdb44 default == 1000*BUFSIZ
    void (*key_display)(int, char *) = gdb_display_noop,
    void (*data_display)(int, char *) = gdb_display_noop
  );

  ~Gdb44C ();

  error_status_t read	// arbitrary key lookup with gdb_entry_lookup
  (
    char **entry_p,
    unsigned32 *entry_len_p,
    char *key_p,
    unsigned32 key_len,
    index_t key_type,
    gdb_context_t *context_p = 0
  );

  error_status_t read  // primary key lookup with gdb_entry_lookup
  (
    char **entry_p,
    unsigned32 *entry_len_p,
    char *key_p,
    unsigned32 key_len,
    gdb_context_t *context_p = 0
  );

  error_status_t read  // gdb_entry_lookup_after
  (
    char *lkp_key_p,
    unsigned32 lkp_key_len,
    char **entry_p,
    unsigned32 *entry_len_p,
    char **ret_key_p,
    unsigned32 *ret_key_len_p,
    gdb_context_t *context_p = 0
  );

  error_status_t read  // gdb_entry_lookup_next
  (
    char **entry_p,
    unsigned32 *entry_len_p,
    char **ret_key_p,
    unsigned32 *ret_key_len_p,
    gdb_context_t *context_p = 0
  );

  error_status_t insert // insert rec using a primary key with gdb_entry_insert
  (
    char *,
    unsigned32,
    char *,
    unsigned32
  );

  error_status_t insert // insert rec using 1-ary & 2-ary key w/gdb_entry_insert
  (
    char *,
    unsigned32,
    char *,
    unsigned32,
    char *,
    unsigned32
  );

  error_status_t write // write rec using a primary key with gdb_entry_insert
  (		       // or gdb_entry_update
    char *,
    unsigned32,
    char *,
    unsigned32
  );

  error_status_t safe_insert // transactional insert with primary key only
  (
    char *,
    unsigned32,
    char *,
    unsigned32
  );

  error_status_t safe_insert // transactional insert with 1-ary & 2-ary keys
  (
    char *,
    unsigned32,
    char *,
    unsigned32,
    char *,
    unsigned32
  );

  error_status_t safe_write // transactional write with primary key only
  (
    char *,
    unsigned32,
    char *,
    unsigned32
  );

  error_status_t remove	// delete a record using gdb_entry_delete
  (
    char *,
    unsigned32
  );

  void txnBegin();	// begin transaction with gdb_txn_begin
  void txnEnd();	// commit transaction with gdb_txn_commit
  void txnCancel();	// rollback transaction with gdb_txn_rollback

  error_status_t cstatus;	// status of constructor

private:
  when_t whenToShutdown;
  storage_handle_t handle;

}; // End class Gdb44C

#endif // GDB_METHODS_H
