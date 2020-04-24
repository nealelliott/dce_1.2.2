/*
 * @OSF_COPYRIGHT@
 *
 * (C) Copyright 1993 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * itl_bakDumpEntry.h -- 
 *    The definitions and declarations for the budb_dumpEntry data type.
 * 
 * $Header: /u0/rcs_trees/dce/rcs/test/file/itl/bakserver/itl_bakDumpEntry.h,v 1.1.6.1 1996/10/17 18:24:45 damon Exp $
 */
/*
 * HISTORY
 * $Log: itl_bakDumpEntry.h,v $
 * Revision 1.1.6.1  1996/10/17  18:24:45  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:52:29  damon]
 *
 * $EndLog$
 */

#ifndef ITL_BAK_DUMP_ENTRY_H
#define ITL_BAK_DUMP_ENTRY_H

extern int itlBak_dumpEntryDataType;

/*
 *  External function declarations.
 */

extern void itlBak_DumpEntryInit _TAKES((
					void
					));

extern boolean_t itlBak_DumpEntryGet _TAKES((
					     OUT char **varNamePP,
					     OUT budb_dumpEntry *dumpEntryP
					     ));

/*
 *  itlBak_DumpEntryCompare() --
 *    Compare two Dump entries for equality.  Needed by the bak list
 *    data types.
 *
 *  RETURN CODES:
 *    TRUE  - they are equal
 *    FALSE - they are not equal
 */
extern boolean_t itlBak_DumpEntryCompare _TAKES((
						 IN budb_dumpEntry *leftP,
						 IN budb_dumpEntry *rightP
						 ));

#endif  /* ITL_BAK_DUMP_ENTRY_H */
