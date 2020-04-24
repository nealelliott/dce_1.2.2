/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: itl_utilsNameHash.h,v $
 * Revision 1.1.8.1  1996/10/17  18:28:58  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:54:34  damon]
 *
 * Revision 1.1.2.3  1993/01/11  18:32:33  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:13:46  htf]
 * 
 * Revision 1.1.2.2  1992/10/28  18:00:11  jaffe
 * 	New file
 * 	[1992/10/28  14:26:15  jaffe]
 * 
 * $EndLog$
 */
/*
 * itl_utilsNameHash.h --
 *    Declarations for using the utilites name hash table.
 * 
 * USAGE:
 *   This package implements a generic hash table, that is keyed by a
 *   name.  So it should be usable for filenames, directory names,
 *   fileset names, etc.  This was believed simpler, from a maintenance
 *   viewpoint, then having separate hash tables for each "named" entitiy.
 *   Since it is storing names of many things, it uses a type variable to
 *   distinguish similar names.  However, name collision is expected to be
 *   rare since full-pathnames will be used in most cases.
 *
 *   The usage model is the following.
 *     1)  Get a unique type for your entities by calling the 
 *         itlu_NameHashGetType() function.
 *     1)  Add "names" to the hash table.
 *     2)  Carry out operations on entities in the hash table by using the
 *         itlu_NameHashLookup() function. Besides the
 *         bucket locks kept by the CHT, a latch is held on each entry to
 *         increase concurrency. (For stress type tests this is important.)
 *     3)  When you complete an operation on an entitiy you drop the latch
 *         on that entity, using the itlu_NameHashRelease() function.
 *     4)  When the entity is no longer needed you remove it from the table.
 */

#ifndef ITL_UTILS_NAME_HASH_H
#define ITL_UTILS_NAME_HASH_H

#include <pthread.h>
#include <utils/itl/itl_stds.h>
#include <utils/cht_hash.h>

/**
 **  Data definitions
 **/

/*
 *  itlu_NameHashTableId_t -
 *    The name and type of the name table entry must be grouped together
 *    so that the itlFile_NameTable_t can be used in conjunction with the CHT
 *    package.  The CHT package requires an equality function.  In our case
 *    two itlu_NameHashTable_t's are equal if both their names are the same,
 *    and their types are the same.
 */
typedef struct {
    char         *name;   /* name of object - must be unique by type */
    unsigned int type;    /* type of object - for name verification */
} itlu_NameHashTableId_t;

typedef struct {
    itlu_NameHashTableId_t id;    /* see above */
    void                 *dataP;  /* pointer to the actual data structure */
    pthread_mutex_t     latch;    /* latch for this entry.  This way we can */
                                  /* drop the bucket latches and increase */
                                  /* the concrurency within the program */
    cht_link_t           link;    /* for CHT linking */
} itlu_NameHashTable_t;

#define ITLU_NAME_HASH_NULL (itlu_NameHashTable_t *) 0

/*
 *  ITLU_NAME_HASH_DROP_LATCH() --
 *    Drops the latch on a name table entry.
 *
 *  Arguments:
 *    _entryP - pointer  to a itlu_NameHashTable_t.
 */
#define ITLU_NAME_HASH_DROP_LATCH(_entryP)     \
        pthread_mutex_unlock(&((_entryP)->latch));


/*
 * ITLU_NAME_HASH_GET_DATA()
 *    Macro to return the dataP field to the user.  The macro expects a 
 *    pointer to a itlu_NameHashTable_t and a type specifier.
 *
 * NOTE:
 *    Caller is expected to be holding the latch on the hash entry.
 *
 */
#define ITLU_NAME_HASH_GET_DATA(_entryP, _type) ((_type *)(_entryP)->dataP)


/*
 * ITLU_NAME_HASH_GET_NAME()
 *    Macro to return the name field to the user.  The macro expects a 
 *    pointer to a itlu_NameHashTable_t.
 *
 * NOTE:
 *    Caller is expected to be holding the latch on the hash entry.
 *
 */
#define ITLU_NAME_HASH_GET_NAME(_entryP) (_entryP)->id.name


/**
 **  Function declarations
 **/

extern void itlu_NameHashInit _TAKES((
				      void
				      ));

extern unsigned int itlu_NameHashGetType _TAKES((
						void
						));

extern boolean_t itlu_NameHashAdd _TAKES((
					 IN char *nameP,
					 IN unsigned int type,
					 IN void *dataP
					 ));

extern void *itlu_NameHashDelete _TAKES((
					IN char *nameP,
					IN unsigned int type
					));

extern itlu_NameHashTable_t *itlu_NameHashLookup _TAKES((
						       IN char *nameP,
						       IN unsigned int type
						       ));

extern boolean_t itlu_NameHashChangeName _TAKES((
						IN itlu_NameHashTable_t *entryP,
						IN char *newNameP
						));

extern void itlu_NameHashRelease _TAKES((
					itlu_NameHashTable_t *entryP
					));

#endif /* ITL_UTILS_NAME_HASH_H */

