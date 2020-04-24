/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * @DEC_COPYRIGHT@
 * Copyright (c) 1996
 * Digital Equipment Corporation, Maynard, Massachusetts, USA
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: gdb.h,v $
 * Revision 1.1.2.1  1996/10/03  20:24:12  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/03  19:19:11  arvind]
 *
 * Revision /main/DCE_1.2.2/1  1996/09/09  21:22 UTC  arvind
 * 	PKSS drop from DEC (DCE1.2.2)
 * 	[1996/08/30  15:36 UTC  arvind  /main/arvind_pkss/1]
 * 
 * 	Remove CDS-specific compare routine prototypes.
 * 	[1996/07/10  18:47:59  zee]
 * 	 *
 * 
 * 	updates for generalization of comparison and debug routines
 * 	[1996/06/28  17:06:33  farrell]
 * 
 * 	GLORIA (R1.2.2) revision.
 * 	[1996/06/24  20:48:00  zee]
 * 
 * $EndLog$
 */
#ifndef _GDB_H_
#define _GDB_H_
/*
 * gdb.h
 *
 * Include file for callers of gdb_* and gdbi 
 */

typedef void * storage_handle_t;
     
/*
 * Patience levels for gdb_storage_close()
 */
typedef enum when {
   WHEN_NOW = 1,
   WHEN_DONE
} when_t;

/*
 * Usage types for gdb_storage_open()
 */
typedef enum use {
   USE_TEMPORARY = 1,
   USE_PERMANENT
} use_t;

/*
 * Database key indicators 
 */
typedef enum index {
   KEY_PRIMARY = 1,
   KEY_SECONDARY
} index_t;

/*
 * Context  --- G-L-O-R-I-A  (opaque)
 */
typedef char gdb_context_t;

/*
 * Prototypes 
 */
#ifdef GDB_DEBUG
void gdb_entry_free (storage_handle_t, char *);
void gdb_key_free (storage_handle_t, char *);  /* use macro */

#else
#define gdb_entry_free(s,x) free(x)
#define gdb_key_free(s,x) free(x)
#endif

int gdb_storage_open (storage_handle_t *,
		      use_t             ,
		      char             *,
		      unsigned int      ,
		      int             (*)(int, char *, int, char *),
		      void            (*)(int, char *),
		      void            (*)(int, char *) );

int gdb_storage_close (storage_handle_t ,
		   when_t           );

int gdb_storage_create_index (storage_handle_t  ,
			      unsigned int      ,
			      int             (*)(int, char *, int, char *),
			      void            (*)(int, char *) );

int gdb_storage_delete (storage_handle_t  ,
			const char       *);

int gdb_storage_dump (storage_handle_t );

int gdb_storage_verify (storage_handle_t );

int gdb_entry_insert (storage_handle_t  ,
		      char             *,
		      unsigned int      ,
		      char             *,
		      unsigned int      ,
		      char             *,
		      unsigned int      );

int gdb_entry_delete (storage_handle_t  ,
		      char             *,
		      unsigned int      ,
		      index_t           );

int gdb_entry_lookup_next (storage_handle_t  ,
			   gdb_context_t    *,
			   char            **,
			   unsigned int     *,
			   char            **,
			   unsigned int     *,
			   index_t           );

int gdb_entry_lookup_first (storage_handle_t  ,
			    char            **,
			    unsigned int     *,
			    char            **,
			    unsigned int     *,
			    gdb_context_t    *,
			    index_t           );

int gdb_entry_lookup (storage_handle_t  ,
		      char            **,
		      unsigned int     *,
		      char             *,
		      unsigned int      ,
		      gdb_context_t    *,
		      index_t           );

int gdb_entry_lookup_after (storage_handle_t  ,
		      char                   *,
		      unsigned int            ,
		      char                  **,
		      unsigned int           *,
		      char                  **,
		      unsigned int           *,
		      gdb_context_t          *,
		      index_t                 );

int gdb_entry_update (storage_handle_t  ,
		      char             *,
		      unsigned int      ,
		      char             *,
		      unsigned int      ,
		      index_t           );

void gdb_txn_begin (storage_handle_t );

void gdb_txn_commit (storage_handle_t );

void gdb_txn_rollback (storage_handle_t );

/*
 * Pre-defined GDB display function
 */
void gdb_display_noop (int, char *);

#endif /* not defined _GDB_H_ */
