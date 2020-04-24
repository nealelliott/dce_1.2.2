/*
 * @OSF_COPYRIGHT@
 *
 * (C) Copyright 1993 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * itl_bakListType.h -- 
 *    The definitions and declarations for the generic list type for
 *    the backup server.  This is for the types,
 *      budb_volumeList
 *      budb_dumpList
 *      budb_tapeList
 * 
 * $Header: /u0/rcs_trees/dce/rcs/test/file/itl/bakserver/itl_bakListType.h,v 1.1.6.1 1996/10/17 18:24:50 damon Exp $

 */
/*
 * HISTORY
 * $Log: itl_bakListType.h,v $
 * Revision 1.1.6.1  1996/10/17  18:24:50  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:52:31  damon]
 *
 * $EndLog$
 */

#ifndef ITL_BAK_LIST_TYPE_H
#define ITL_BAK_LIST_TYPE_H

/*
 *  The itlBak_listType_t and the macros to create specific list, help to
 *  hide the fact that the four list types are all impelemented by the same
 *  code.
 *
 *  Each of the BAK_CREATE macros takes a pointer to a character string
 *  that will be used as the name of the new variable that is created and,
 *  a pointer to the list of the
 *  type being created, e.g. BAK_CREATE_VOLUME_LIST takes a pointer to a
 *  budb_volumeList.
 */
typedef enum {
    bakVolumeList,
    bakDumpList,
    bakTapeList
} itlBak_listTypes_t;

#define BAK_CREATE_VOLUME_LIST(_vname, _lp)    \
        itlBak_ListCreate(bakVolumeList, (_vname), (_lp));
#define BAK_CREATE_DUMP_LIST(_vname, _lp)      \
        itlBak_ListCreate(bakDumpList, (_vname), (_lp));
#define BAK_CREATE_TAPE_LIST(_vname, _lp)      \
        itlBak_ListCreate(bakTapeList, (_vname), (_lp));

extern int itlBak_listDataType;

/*
 *  External function declarations.
 */

extern void itlBak_ListInit _TAKES((
				    void
				    ));

extern void itlBak_ListCreate _TAKES((
				      IN itlBak_listTypes_t listType,
				      IN char *varName,
				      IN pointer_t listP
				      ));

#endif  /* ITL_BAK_LIST_TYPE_H */
