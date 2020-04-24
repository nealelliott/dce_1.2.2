/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: itl_utilsNameHash.c,v $
 * Revision 1.1.8.1  1996/10/17  18:28:56  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:54:33  damon]
 *
 * Revision 1.1.2.3  1993/01/11  18:32:31  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:13:42  htf]
 * 
 * Revision 1.1.2.2  1992/10/28  18:00:02  jaffe
 * 	New file
 * 	[1992/10/28  14:25:27  jaffe]
 * 
 * $EndLog$
 */
/*
 * itlu_NameHash.c -- 
 *    C functions for implementing the name hash table.
 */


#include <pthread.h>
#include <utils/itl/itl.h>
#include <utils/itl/itl_stds.h>
#include <utils/cht_hash.h>
#include <utils/itl/itl_utilsNameHash.h>

pthread_mutex_t hashTypeMutex;
unsigned int hashTypeCounter = 0;

/*
 *  DESTROY_ENTRY() --
 *    Macro that implements the steps required to destroy a hash table
 *    entry.  It does not actually delete the entry.
 *
 *  Arguments:
 *    _destroyP - a pointer to a itlu_NameHashTable_t
 *    _funcName - name of function invoking macro; for error reporting
 */
#define DESTROY_ENTRY(_destroyP, _funcName)                                  \
        MACRO_BEGIN                                                          \
          if (pthread_mutex_destroy(&((_destroyP)->latch)) != 0)             \
	  {                                                                  \
	    itl_Error("pthread_mutex_destroy failed in %s, errno = %d\n",    \
		       errno);                                               \
	    exit(-1);                                                        \
	  }                                                                  \
	  ITL_UTIL_FREE((_destroyP)->id.name);                               \
	  ITL_UTIL_FREE((_destroyP));                                        \
        MACRO_END                                                            \

/*
 *  Forward declaration of HashId() to make the compiler happy.
 */
PRIVATE unsigned int HashId _TAKES((
				    IN itlu_NameHashTableId_t *idP
				    ));

#define HASH_SIZE               256
#define HASH_KEY_TYPE           itlu_NameHashTableId_t *
#define HASH_KEY_GET(x)         (&((x)->id))
#define HASH_KEY_HASH(x)        (HashId((x)))
#define HASH_KEY_COMPARE(x,y)   \
        (((x)->type == (y)->type) && (strcmp((x)->name, (y)->name) == 0))

CHT_GENERATE_ROUTINES(PRIVATE,itlu_Hash,HASH_SIZE,itlu_NameHashTable_t,link,
		      HASH_KEY_TYPE,HASH_KEY_GET,HASH_KEY_HASH,
		      HASH_KEY_COMPARE)


/*
 * HashId() -- 
 *    Converts a filename into a hashable integer.
 * 
 * RETURN CODES: unsigned integer (as required by CHT package)
 */
PRIVATE unsigned int HashId(idP)
  IN itlu_NameHashTableId_t *idP;   /* passed in by CHT package */
{
    register unsigned int i;
    register unsigned int result = 1000003;
    register unsigned int length;
    register char *fastName;

    fastName = idP->name;
    length = strlen(fastName);

    /*
     * For each character multiple the result by 5 and add the 
     * character.
     */
    for (i = 0; i < length; i++)
    {
	result = (result << 2) + result;
	result += fastName[i];
    }
    result += idP->type;

    return(result);
}

PRIVATE void NameTableInit()
{
    itlu_HashInit();
    if (pthread_mutex_init(&hashTypeMutex, pthread_mutexattr_default) != 0)
    {
	itl_Error("pthread_mutex_init failed in NameTableInit.\n");
	exit(-1);
    }
}

/*
 * itlu_NameHashInit() -- 
 *    So the CHT can be initialized.
 * 
 * RETURN CODES: none
 */
SHARED void itlu_NameHashInit()
{
    static pthread_once_t hashOnce = pthread_once_init;

    if (pthread_once(&hashOnce, (pthread_initroutine_t) NameTableInit))
	itl_PrintError("pthread_once failed in NameTableInit\n");
}

/*
 *  itlu_NameHashGetType() --
 *    Generate a unique hash type identifier.
 */
SHARED unsigned int itlu_NameHashGetType()
{
    unsigned int retVal;

    pthread_mutex_lock(&hashTypeMutex);
    retVal = ++hashTypeCounter;
    pthread_mutex_unlock(&hashTypeMutex);
    return(retVal);
}

/*
 * itlu_NameHashAdd() --
 *    Adds a name of a specific type to the hash table.
 *
 * RETURN CODES: 
 *    FALSE - Successful addition.
 *    TRUE  - Unsuccessful addition, filename exists or is invalid.
 */
SHARED boolean_t itlu_NameHashAdd(nameP, type, dataP)
  IN char *nameP;         /* the string to compare with */
  IN unsigned int type;   /* type of entity */
  IN void *dataP;         /* user data */
{
    boolean_t retVal = FALSE;

    if (nameP && strcmp(nameP, ""))
    {
	itlu_NameHashTable_t *entryP;
	cht_mutex_t latch;
	itlu_NameHashTableId_t checkId;

	/*
	 *  Determine if the name exists.
	 */
	checkId.name = nameP;
	checkId.type = type;
	entryP = itlu_HashLookup(&checkId, CHT_LATCH_KEEP, &latch);
	if (entryP == ITLU_NAME_HASH_NULL) 
	{
	    /*
	     *  No entry, so add this one.
	     */
	    ITL_UTIL_NEW(entryP, itlu_NameHashTable_t);
	    ITL_UTIL_ALLOC(entryP->id.name, char *, (strlen(nameP) + 1));
	    if (pthread_mutex_init(&(entryP->latch), pthread_mutexattr_default)
		!= 0)
	    {
		itl_Error("pthread_mutex_init failed in itlu_NameHashAdd, errno = %d\n",
			  errno);
		exit(-1);
	    }
	    strcpy(entryP->id.name, nameP);
	    entryP->id.type = type;
	    entryP->dataP = dataP;
	    /*
	     *  CHT_LATCH_NONE is used because we still hold the latch from the
	     *  previous itlu_HashLookup() call.
	     */
	    itlu_HashInsert(entryP, CHT_LATCH_NONE, &latch);
	}
	else 
	{
	    itl_PrintError("File, %s, already in table.\n", nameP);
	    retVal = TRUE;
	}

	/*
	 *  Drop the CHT bucket latch.
	 */
	cht_unlock(latch);
    }
    else
	retVal = TRUE;

    return(retVal);
}

/*
 * itlu_NameHashDelete() --
 *    Delete a name from the table.
 *
 * CAUTIONS:
 *    This must be implemented as a function since the CHT routines were
 *    generated as PRIVATE.
 *
 * RETURN CODES: void
 */
SHARED void *itlu_NameHashDelete(nameP, type)
  IN char *nameP;   /* name of entity to remove */
  IN unsigned int type;
{
    void *returnData = (void *) 0;

    /*
     *  Validate name is OK.
     */
    if (nameP && strcmp(nameP, ""))
    {
	itlu_NameHashTable_t *entryP;
	cht_mutex_t latch;
	itlu_NameHashTableId_t checkId;

	/*
	 *  Determine if the filename exists.
	 */
	checkId.name = nameP;
	checkId.type = type;
	entryP = itlu_HashLookup(&checkId, CHT_LATCH_KEEP, &latch);

	if (entryP != ITLU_NAME_HASH_NULL) {
	    returnData = entryP->dataP;

	    /*
	     *  Remove the entry.
	     */
	    itlu_HashDel(entryP, CHT_LATCH_RELEASE);
	    DESTROY_ENTRY(entryP, "itlu_NameHashRemove");
	}
	else {
	    /*
	     *  Drop the CHT bucket latch.
	     */
	    cht_unlock(latch);

	    itl_Message("\nWARNING: Entry, %s, NOT in hash table.\n\n", nameP);
	}
    }

    return(returnData);
}

/*
 * itlu_NameHashLookup() --
 *    Lookup and lock an entity in the hash table.
 *
 * RETURN CODES: (itlu_NameHashTable_t *)
 *    Non-NULL - successful lookup
 *    NULL     - unsuccessful lookup
 */
SHARED itlu_NameHashTable_t *itlu_NameHashLookup(nameP, type)
  IN char *nameP;         /* the string to compare with */
  IN unsigned int type;   /* type of entity */
{
    itlu_NameHashTable_t *entryP = (itlu_NameHashTable_t *) 0;

    if (nameP && strcmp(nameP, ""))
    {
	cht_mutex_t latch;
	itlu_NameHashTableId_t checkId;

	/*
	 *  Determine if the filename exists.
	 */
	checkId.name = nameP;
 	checkId.type = type;
	entryP = itlu_HashLookup(&checkId, CHT_LATCH_KEEP, &latch);
	if (entryP != ITLU_NAME_HASH_NULL) 
	{
	    /*
	     *  Latch the entry and drop the latch on the hash table bucket
	     *  before the function returns.
	     */
	    if (pthread_mutex_lock(&(entryP->latch)) != 0)
	    {
		itl_Error("pthread_mutex_lock failed in itlu_NameHashLookup, errno = %d\n",
			  errno);
		exit(-1);
	    }
	}

	/*
	 *  Drop the CHT bucket latch.
	 */
	cht_unlock(latch);
    }

    return(entryP);
}

/*
 * itlu_NameHashChangeName() --
 *    Changes the name of a entry in the hash table.
 * 
 * ASSUMPTIONS:
 *    Caller is holding the latch on the entry, i.e. has previously called 
 *    the itlu_NameHashLookup() function.
 */
SHARED boolean_t itlu_NameHashChangeName(entryP, newNameP)
  IN itlu_NameHashTable_t *entryP;  /* gotten from a itlu_NameHashLookup call */
  IN char *newNameP;               /* new name */
{
    boolean_t retVal = TRUE;

    /*
     *  Validate parameters.
     */
    if ((entryP != (itlu_NameHashTable_t *) 0) && 
	newNameP && strcmp(newNameP, ""))
    {
	if (!itlu_NameHashAdd(newNameP, entryP->id.type, entryP->dataP))
	{
	    itlu_HashDel(entryP, CHT_LATCH_NONE);
	    DESTROY_ENTRY(entryP, "itlu_NameHashChangeName");
	    retVal = FALSE;
	}
    }
    else
	itl_Error("Invalid name passed to itlu_NameHashChangeName\n");

    return(retVal);
}

/*
 *  itlu_NameHashRelease()
 *    Release the individual entity latch.  This is implemented as a function
 *    just for interface symmetry, i.e., Lookup and Release.  I did not
 *    think speed was of the essence here. 
 */
SHARED void itlu_NameHashRelease(entryP)
  IN itlu_NameHashTable_t *entryP;
{
    if (pthread_mutex_unlock(&(entryP->latch)) != 0)
    {
	itl_Error("pthread_mutex_unlock failed in itlu_NameHashRelease, errno = %d\n",
		  errno);
	exit(-1);
    }
}
