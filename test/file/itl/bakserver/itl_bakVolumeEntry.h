/*
 * @OSF_COPYRIGHT@
 *
 * (C) Copyright 1993 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * itl_bakVolumeEntry.h -- 
 *    The definitions and declarations for the budb_VolumeEntry data type.
 * 
 * $Header: /u0/rcs_trees/dce/rcs/test/file/itl/bakserver/itl_bakVolumeEntry.h,v 1.1.6.1 1996/10/17 18:25:03 damon Exp $
 */
/*
 * HISTORY
 * $Log: itl_bakVolumeEntry.h,v $
 * Revision 1.1.6.1  1996/10/17  18:25:03  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:52:38  damon]
 *
 * $EndLog$
 */

#ifndef ITL_BAK_VOLUME_ENTRY_H
#define ITL_BAK_VOLUME_ENTRY_H

extern int itlBak_volumeEntryDataType;

/*
 *  External function declarations.
 */

extern void itlBak_VolumeEntryInit _TAKES((
					void
					));

extern void itlBak_VolumeEntryGet _TAKES((
					 OUT budb_volumeEntry *volumeEntryP
					 ));

/*
 *  itlBak_VolumeEntryCompare() --
 *    Compare two volume entries for equality.  Needed by the bak list
 *    data types.
 *
 *  RETURN CODES:
 *    TRUE  - they are equal
 *    FALSE - they are not equal
 */
extern boolean_t itlBak_VolumeEntryCompare _TAKES((
						   IN budb_volumeEntry *leftP,
						   IN budb_volumeEntry *rightP
						   ));

#endif  /* ITL_BAK_VOLUME_ENTRY_H */
