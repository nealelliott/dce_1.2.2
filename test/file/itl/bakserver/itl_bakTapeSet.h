/*
 * @OSF_COPYRIGHT@
 *
 * (C) Copyright 1993 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * itl_bakTapeSet.h -- 
 *    The definitions and declarations for the budb_tapeSet data type.
 * 
 * $Header: /u0/rcs_trees/dce/rcs/test/file/itl/bakserver/itl_bakTapeSet.h,v 1.1.6.1 1996/10/17 18:24:59 damon Exp $
 */
/*
 * HISTORY
 * $Log: itl_bakTapeSet.h,v $
 * Revision 1.1.6.1  1996/10/17  18:24:59  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:52:36  damon]
 *
 * $EndLog$
 */

#ifndef ITL_BAK_TAPE_SET_H
#define ITL_BAK_TAPE_SET_H

extern int itlBak_tapeSetDataType;

/*
 *  External function declarations.
 */

extern void itlBak_TapeSetInit _TAKES((
					void
					));

extern boolean_t itlBak_TapeSetCompare _TAKES((
					       IN budb_tapeSet *tapeSet1,
					       IN budb_tapeSet *tapeSet2
					       ));

extern char *itlBak_TapeSetDisplay _TAKES((
					   IN budb_tapeSet *tapeSetP,
					   OUT int *lengthP
					   ));

extern var_val_t *itlBak_TapeSetCreate _TAKES((
					       IN budb_tapeSet *tapeSetP
					       ));

extern void itlBak_TapeSetGet _TAKES((
				      OUT budb_tapeSet *tapeSetP
				      ));

#endif  /* ITL_BAK_TAPE_SET_H */
