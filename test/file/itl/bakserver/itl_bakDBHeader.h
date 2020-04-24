/*
 * @OSF_COPYRIGHT@
 *
 * (C) Copyright 1993 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * itl_bakDBHeader.h -- 
 *    The definitions and declarations for the DbHeader data type.
 * 
 * $Header: /u0/rcs_trees/dce/rcs/test/file/itl/bakserver/itl_bakDBHeader.h,v 1.1.6.1 1996/10/17 18:24:41 damon Exp $
 */
/*
 * HISTORY
 * $Log: itl_bakDBHeader.h,v $
 * Revision 1.1.6.1  1996/10/17  18:24:41  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:52:28  damon]
 *
 * $EndLog$
 */

#ifndef ITL_BAK_DB_HEADER_H
#define ITL_BAK_DB_HEADER_H

extern int itlBak_dbHeaderDataType;

/*
 *  External function declarations.
 */

extern void itlBak_DBHeaderInit _TAKES((
					void
					));

extern void itlBak_DBHeaderGet _TAKES((
				       OUT DbHeader *headerP
				       ));

#endif  /* ITL_BAK_DB_HEADER_H */
