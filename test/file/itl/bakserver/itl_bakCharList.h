/*
 * @OSF_COPYRIGHT@
 *
 * (C) Copyright 1993 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * itl_bakCharListType.h -- 
 *    The definitions and declarations for the charListT type.
 * 
 * $Header: /u0/rcs_trees/dce/rcs/test/file/itl/bakserver/itl_bakCharList.h,v 1.1.6.1 1996/10/17 18:24:28 damon Exp $

 */
/*
 * HISTORY
 * $Log: itl_bakCharList.h,v $
 * Revision 1.1.6.1  1996/10/17  18:24:28  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:52:24  damon]
 *
 * $EndLog$
 */

#ifndef ITL_BAK_CHAR_LIST_TYPE_H
#define ITL_BAK_CHAR_LIST_TYPE_H

extern int itlBak_charListDataType;

/*
 *  External function declarations.
 */

extern void itlBak_CharListInit _TAKES((
					void
					));

extern boolean_t itlBak_CharListVariableGet _TAKES((
						    OUT char **namePP,
						    OUT charListT **charListPP
						    ));

extern void itlBak_CharListDataGet _TAKES((
					   OUT charListT *charListP
					   ));

#endif  /* ITL_BAK_CHAR_LIST_TYPE_H */
