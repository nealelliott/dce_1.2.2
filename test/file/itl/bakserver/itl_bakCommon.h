/*
 * ID: $Id: itl_bakCommon.h,v 1.2 1993/06/21 21:06:19 fred Exp $
 *
 * COMPONENT_NAME: itl_bakCommon.h
 *
 * ORIGINS: Transarc Corp.
 *
 * (C) COPYRIGHT Transarc Corp. 1993
 * All Rights Reserved
 * Licensed Materials - Property of Transarc
 *
 * US Government Users Restricted Rights - Use, duplication or
 * disclosure restricted by GSA ADP Schedule Contract with Transarc Corp
 */
/*
 * HISTORY
 * $Log: $
 *
 * $EndLog$
 */

#ifndef ITL_BAK_COMMON_H
#define ITL_BAK_COMMON_H

#include <assert.h>
#include <time.h>
#include <utils/itl/itl_stds.h>
#include <utils/itl/itl.h>

#define CHARS_IN_DATE_STRING 25  /* chars. in string from ctime() w/o '\0' */

/*
 *  ITL_BAK_STR_LEN_ERROR
 *    Just a standard error message for strings that are too long.
 *
 *  Arguments:
 *    _strTypeP - type of string, e.g. "Path"
 *    _badStrP  - pointer to string that is invalid.
 *    _maxLen   - the maximum length that was exceeded. 
 */
#define ITL_BAK_STR_LEN_ERROR(_strTypeP, _badStrP, _maxLen)                   \
        itl_Error("%s string length error - max. length is %d\nSTRING: %s\n", \
		  (_strTypeP), (_maxLen), (_badStrP))

#define ITL_BAK_NAME_ERROR(_nameStrP)                                         \
            ITL_BAK_STR_LEN_ERROR("Name", (_nameStrP), AFS_NAMEMAX)

#define ITL_BAK_PATH_ERROR(_pathStrP)                                         \
            ITL_BAK_STR_LEN_ERROR("Path", (_pathStrP), AFS_PATHMAX)

#define ITL_BAK_DFS_PROT_SEQ   "ncadg_ip_udp"


/*
 *  External function declarations.
 */

extern void itlBak_CommonInit _TAKES((
				      void
				      ));

extern time_t itlBak_CommonGetLimitedStr _TAKES((
						 IN char *fieldName,
						 IN unsigned int maxLength,
						 OUT char *dataP
						 ));

extern time_t itlBak_CommonGetTime _TAKES((
					   IN char *fieldName,
					   IN boolean_t acceptFull
					   ));

extern boolean_t itlBak_CommonGetArrayIndex _TAKES((
						    IN char *fieldName,
						    OUT int *indexP
						    ));

#endif /* ITL_BAK_COMMON_H */
