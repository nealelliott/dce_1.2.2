/*
 * @OSF_COPYRIGHT@
 *
 * (C) Copyright 1993 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * itl_bakTapeEntry.h -- 
 *    The definitions and declarations for the budb_TapeEntry data type.
 * 
 * $Header: /u0/rcs_trees/dce/rcs/test/file/itl/bakserver/itl_bakTapeEntry.h,v 1.1.6.1 1996/10/17 18:24:56 damon Exp $
 */
/*
 * HISTORY
 * $Log: itl_bakTapeEntry.h,v $
 * Revision 1.1.6.1  1996/10/17  18:24:56  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:52:34  damon]
 *
 * $EndLog$
 */

#ifndef ITL_BAK_TAPE_ENTRY_H
#define ITL_BAK_TAPE_ENTRY_H

extern int itlBak_tapeEntryDataType;

/*
 *  External function declarations.
 */

extern void itlBak_TapeEntryInit _TAKES((
					void
					));

extern void itlBak_TapeEntryGet _TAKES((
					 OUT budb_tapeEntry *principalP
					 ));

/*
 *  itlBak_TapeEntryCompare() --
 *    Compare two Tape entries for equality.  Needed by the bak list
 *    data types.
 *
 *  RETURN CODES:
 *    TRUE  - they are equal
 *    FALSE - they are not equal
 */
extern boolean_t itlBak_TapeEntryCompare _TAKES((
						 IN budb_tapeEntry *leftP,
						 IN budb_tapeEntry *rightP
						 ));

#endif  /* ITL_BAK_TAPE_ENTRY_H */
