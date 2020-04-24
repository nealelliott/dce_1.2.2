/*
 * @OSF_COPYRIGHT@
 *
 * (C) Copyright 1993 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * itl_bakPrincipal.h -- 
 *    The definitions and declarations for the budb_principal data type.
 * 
 * $Header: /u0/rcs_trees/dce/rcs/test/file/itl/bakserver/itl_bakPrincipal.h,v 1.1.6.1 1996/10/17 18:24:53 damon Exp $
 */
/*
 * HISTORY
 * $Log: itl_bakPrincipal.h,v $
 * Revision 1.1.6.1  1996/10/17  18:24:53  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:52:33  damon]
 *
 * $EndLog$
 */

#ifndef ITL_BAK_PRINCIPAL_H
#define ITL_BAK_PRINCIPAL_H

extern int itlBak_principalDataType;

/*
 *  External function declarations.
 */

extern void itlBak_PrincipalInit _TAKES((
					void
					));

extern boolean_t itlBak_PrincipalCompare _TAKES((
						 IN budb_principal *principal1,
						 IN budb_principal *principal2
						 ));

extern char *itlBak_PrincipalDisplay _TAKES((
					     IN budb_principal *principalP,
					     OUT int *lengthP
					     ));

extern var_val_t *itlBak_PrincipalCreate _TAKES((
						 IN budb_principal *principalP
						 ));

extern void itlBak_PrincipalGet _TAKES((
					 OUT budb_principal *principalP
					 ));

#endif  /* ITL_BAK_PRINCIPAL_H */
