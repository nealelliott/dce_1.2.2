/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/* 
 * HISTORY
 * $Log: cht_hash.h,v $
 * Revision 1.1.8.1  1996/10/17  18:25:08  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:52:41  damon]
 *
 * Revision 1.1.2.3  1993/01/11  18:29:36  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:07:12  htf]
 * 
 * Revision 1.1.2.2  1992/09/25  17:14:02  jaffe
 * 	Transarc delta: fred-checkin-itl 1.1
 * 	 Selected comments:
 * 	   Port of Interactive Test Language from Encina BDE to DCE pthreads.
 * 	   Initial checkin.
 * 	[1992/09/23  21:01:57  jaffe]
 * 
 * $EndLog$
 */
/*
 *  cht_hash.h
 * 
 */

/*
 *  The macros below expand to routines that implement a hash table that
 *  supports concurrent access.
 *
 *  The CHT_GENERATE_HEADERS macro should be called in a .h file if the
 *  generated routines are exported to other .c files.  You must supply the
 *  prefix string for the routine names, the record_t name, and the
 *  key_t name (see below).  The calling conventions for the generated
 *  routines are described below.  Here is a sample invocation of this macro
 *  using the examples given below:
 *
 *      CHT_GENERATE_HEADERS(symbol_,symRecord_t,char *)
 *
 *
 *  The CHT_GENERATE_ROUTINES macro should be called in the .c file
 *  that implements the hash table.  This macro generates routines which
 *  can be used in the module or in other modules.  Each argument is
 *  described below, in the order in which it should be specified.
 *
 *    functionUse - This parameter specifies whether the interface is
 *              EXPORT, HIDDEN, SHARED, or PRIVATE.  It should be one
 *              of these four values.
 *
 *    prefix - This parameter will be used as a prefix for all routines
 *	        and global variables generated.  For example, symbol_.
 *              Note that an underscore ("_") is not automatically inserted,
 *		(Due to limitations of the C preprocessor, you cannot have
 *		any white space on either side of this parameter.)
 *
 *    tableSize - This is the size of the hash table.  It should be a 
 *              power of two to allow for compiler optimizations.
 *
 *    record_t - The hash table will store pointers to records of this
 *		type.  This type must be a structure that contains key and
 *		link fields (see below).  For example, symRecord_t, where
 *		    typedef struct symRecord {
 *			char              symbol[MAX_SYMBOL_LENGTH];
 *			long		  myValue;
 *			cht_link_t        symLink;
 *		    } symRecord_t;
 *
 *    linkFieldName - The structure named by the record_t parameter must
 *		contain a field of type cht_link_t.  This field will be
 *		used to link records together in the hash table.  The
 *		linkFieldName parameter specifies the name of this field.
 *		For example, symLink.
 *
 *    key_t - The type specification used to pass keys around.
 *		For example, char *.
 *
 *    keyExtractionFunctName - This function (or macro) takes one argument
 *		of type (record_t *) and returns something of type
 *		(key_t).  For example, GIVE_KEY, where
 *		    #define GIVE_KEY(x) ((x)->symbol)
 *
 *    hashFunctName - This FunctName (or macro) takes one argument of type
 *		(key_t) and returns a hash value.  The value must be an
 *              integer (it may be unsigned).  For example:
 *		    #define HASH_KEY(x) ((unsigned int) (x[0]))
 *	        (This would be a terrible hash FunctName to use in practice.)
 * 
 *    equalFunctName - This function (or macro) takes two arguments of type
 *		(key_t) and returns true if they are equal.  For example,
 *		EQUAL_KEY, where
 *		    #define EQUAL_KEY(x,y) (strcmp(x,y) == 0)
 *
 *  If we used the examples given above we would have the following:
 *
 *	#include <utils/cht_hash.h>
 *
 *	typedef struct symRecord {
 *		char		symbol[MAX_SYMBOL_LENGTH];
 *		long		myValue;
 *		cht_link_t      symLink;
 *	} symRecord_t;
 *
 *	#define GIVE_KEY(x) ((x)->symbol)
 *	#define HASH_KEY(x)    (x[0])
 *	#define EQUAL_KEY(x,y) (strcmp(x,y) == 0)
 *
 *	CHT_GENERATE_ROUTINES(EXPORT, symbol_, 64, symRecord_t, symLinkP,
 *			      char *, GIVE_KEY, HASH_KEY, EQUAL_KEY)
 *
 *  In this example, the following routines are generated:
 *
 *      ** initializes the bucket latches for each entry in the
 *	   hash table. **    
 *	void SymbolInit()
 *	...
 *
 *      ** takes a key and returns the record, or NULL 
 *         if non-existent, returns a latch if so specified. **
 *	symRecord_t *symbol_Lookup(key, latch, unlatch, latchPP)
 *	    char *key;
 *	    boolean_t latch, unlatch;
 *	    cht_mutex_t *latchPP;
 *	...
 *
 *      ** inserts a record into the table, assumes the record does
 *         not exist, returns a latch if so specified.  **
 *	void symbol_Insert(recordP, latch, unlatch, latchPP)
 *	    symRecord_t *recordP;
 *	    boolean_t latch, unlatch;
 *	    cht_mutex_t *latchPP;
 *	...
 *
 *      ** given a key, deletes its record and returns 
 *         a pointer to the record, or NULL if not found **
 *	symRecord_t *symbol_Delete(key, latch, unlatch, latchPP)
 *	    char *key;
 *	    boolean_t latch, unlatch;
 *	    cht_mutex_t *latchPP;
 *	...
 *
 *      ** given a record, deletes the record from the hash table **
 *	void symbol_Del(recordP, latch, unlatch, latchPP)
 *	    symRecord_t *recordP;
 *	    boolean_t latch, unlatch;
 *	    cht_mutex_t *latchPP;
 *	...
 *
 *      ** given a function of type void, applies the function
 *         to each record in the hash table and returns a latch
 *         if so specified.  Function f must take a record and 
 *         context pointer.
 *	boolean_t symbol_Forall(f, contextP, latch, unlatch, latchPP)
 *	    void (*f)();
 *	    pointer_t contextP;
 *	    boolean_t latch, unlatch;
 *	    cht_mutex_t *latchPP;
 *	...
 *
 *      ** returns statistics for analyzing the hash table structure **
 *	void symbol_Analyze(latch, unlatch, latchPP, recordCountP, 
 *                          usedBucketCountP, totalPathLengthP, 
 *			    maxBucketDepthP)
 *	    boolean_t latch, unlatch;
 *	    cht_mutex_t *latchPP;
 *	    int *recordCountP,*usedBucketCountP,
 *              *totalPathLengthP,*maxBucketDepthP;
 *	...
 *
 *
 *  Three parameters are used to specify table latching behavior:
 *		latch, unlatch and latchPP
 *  The latch and unlatch parameters are booleans specifying whether to
 *  latch the hash bucket prior to the operation, and whether to unlatch 
 *  the bucket after the operation.  If unlatch is FALSE and latchPP is
 *  not NULL, the latch is returned in *latchPP, so that the latch can
 *  be released when desired, via the following call:
 * 
 *	   cht_unlock(*latchPP);
 * 
 *  A previously unlocked bucket can be locked again via the following call,
 *  but the wise hacker will remember that the record he/she looked up may have
 *  been deleted while the bucket was unlocked.
 *
 *	   cht_lock(*latchPP);
 * 
 * 
 *  The macros CHT_LATCH_NONE, CHT_LATCH_MAINTAIN, CHT_LATCH_KEEP,
 *  CHT_LATCH_RELEASE, and CHT_LATCH_BRACKET are provided to conveniently
 *  specify all the possible locking behaviors.
 * 
 * CHT_LATCH_NONE -- Does not get or release a latch.  Useful for nonconcurrent
 * applications and operations in atomic sequences.
 *
 * CHT_LATCH_MAINTAIN -- Does not get or release a latch.  It is the same
 * as CHT_LATCH_NONE, but it looks cleaner for operations in atomic sequences.
 *
 * CHT_LATCH_KEEP -- Obtains a latch prior to performing the operation and does
 * not release it upon completion. This macro only specifies the booleans
 * (latch and unlatch), allowing the user to specify latchPP.
 *
 * CHT_LATCH_RELEASE -- Does not obtain the latch before the operation, and
 * releases it when the operation is complete.  The latch must have been
 * obtained via a previous operation on the same record with CHT_LATCH_KEEP.
 *
 * CHT_LATCH_BRACKET -- Obtains the latch prior to the operation and
 * drops it when the operation is complete.
 *
 *  See the interface document for the cht module in 
 *  	doc/utils/cht/interface.tex for a complete description with examples.
 */

#ifndef	CHT_HASH_H
#define	CHT_HASH_H

#include <pthread.h>

#define cht_boolean_t           int
#define cht_mutex_struct        pthread_mutex_t
#define cht_mutex_init(mutexP)  pthread_mutex_init(mutexP, pthread_mutexattr_default)
#define cht_lock(mutexP)        pthread_mutex_lock(mutexP)
#define cht_unlock(mutexP)      pthread_mutex_unlock(mutexP)

typedef pthread_mutex_t *cht_mutex_t;


#if !defined(__STDC__)

#if defined(_IBMR2)

#define CHT_CONCAT(a,b) a/**/b

#else /* ! _IBMR2 */

#define CHT_IDENT(x) x
#define CHT_CONCAT(a,b) CHT_IDENT(a)b

#endif /* _IBMR2 */

#else

#define CHT_CONCAT(a,b) a ## b

#endif


#define CHT_LATCH_BUCKET \
    if (latch) cht_lock(&bucketP->latch)
#define CHT_UNLATCH_BUCKET \
    if (unlatch) cht_unlock(&bucketP->latch); \
        else if (latchPP != (cht_mutex_t *) NULL) *latchPP = &bucketP->latch

typedef struct cht_bucket {
    cht_mutex_struct    latch;
    char               *recordP;
} cht_bucket_t;

typedef struct cht_link {
    char	       *recordP;
    cht_bucket_t       *bucketP;
} cht_link_t;

#if defined(__STDC__) 

#define CHT_GENERATE_HEADERS(prefix,record_t,key_t)      	    \
extern void CHT_CONCAT(prefix,Init)(void);               	    \
extern record_t *CHT_CONCAT(prefix,Lookup)			    \
    (key_t key,							    \
     cht_boolean_t latch,cht_boolean_t unlatch,cht_mutex_t *latchP);\
extern void CHT_CONCAT(prefix,Insert)				    \
    (record_t *pRecord,						    \
     cht_boolean_t latch,cht_boolean_t unlatch,cht_mutex_t *latchP);\
extern record_t *CHT_CONCAT(prefix,Delete)			    \
    (key_t key,							    \
     cht_boolean_t latch,cht_boolean_t unlatch,cht_mutex_t *latchP);\
extern void CHT_CONCAT(prefix,Del)				    \
    (record_t *pRecord,						    \
     cht_boolean_t latch,cht_boolean_t unlatch,cht_mutex_t *latchP);\
typedef int (*CHT_CONCAT(prefix,ForallFunction_t))		    \
    (record_t *pRecord, char *contextP);			    \
extern cht_boolean_t CHT_CONCAT(prefix,Forall)                      \
    (CHT_CONCAT(prefix,ForallFunction_t),                           \
     pointer_t,cht_boolean_t,cht_boolean_t,cht_mutex_t*);               \
extern void CHT_CONCAT(prefix,Analyze)			            \
    (cht_boolean_t latch,cht_boolean_t unlatch,cht_mutex_t *latchP, \
     int *recordCountP,int *usedBucketCountP,int *totalPathLengthP, \
     int *maxBucketDepthP);

#else

#define CHT_GENERATE_HEADERS(prefix,record_t,key_t)      	    \
extern void CHT_CONCAT(prefix,Init)();               	    	    \
extern record_t *CHT_CONCAT(prefix,Lookup)();			    \
extern void CHT_CONCAT(prefix,Insert)();			    \
extern record_t *CHT_CONCAT(prefix,Delete)();			    \
extern void CHT_CONCAT(prefix,Del)();				    \
typedef int (*CHT_CONCAT(prefix,ForallFunction_t))();		    \
extern cht_boolean_t CHT_CONCAT(prefix,Forall)();                   \
extern void CHT_CONCAT(prefix,Analyze)();

#endif

#define CHT_GENERATE_ROUTINES(functionUse,prefix,tableSize,record_t,linkFieldName,key_t,keyExtractionFunctName,hashFunctName,equalFunctName) \
								    \
PRIVATE cht_bucket_t CHT_CONCAT(prefix,Table)[tableSize];	    \
								    \
functionUse void CHT_CONCAT(prefix,Init)()	                    \
{								    \
    int i;							    \
 								    \
    for(i=0; i<tableSize; i++) {				    \
	CHT_CONCAT(prefix,Table)[i].recordP = NULL;                 \
	cht_mutex_init(&CHT_CONCAT(prefix,Table)[i].latch);         \
    }								    \
    return;							    \
}								    \
								    \
functionUse record_t *CHT_CONCAT(prefix,Lookup)(key, latch, unlatch, latchPP)\
    key_t         key;                                              \
    cht_boolean_t latch, unlatch;                                   \
    cht_mutex_t  *latchPP;                                          \
{                  						    \
    cht_bucket_t *bucketP;					    \
    record_t *currentP;					            \
								    \
    bucketP =	&CHT_CONCAT(prefix,Table)[                          \
              ((unsigned int) (hashFunctName(key))) % (tableSize)]; \
    CHT_LATCH_BUCKET;						    \
    currentP = (record_t *) bucketP->recordP;		            \
								    \
    while (currentP != (record_t *) 0) {			    \
	if (equalFunctName(key,(keyExtractionFunctName(currentP)))) {       \
	    CHT_UNLATCH_BUCKET;					    \
	    return(currentP);				      	    \
        }							    \
	currentP = (record_t *) currentP->linkFieldName.recordP;    \
    }								    \
								    \
    CHT_UNLATCH_BUCKET;						    \
    return((record_t *) 0);					    \
}								    \
    								    \
								    \
functionUse record_t *CHT_CONCAT(prefix,Delete)(key, latch, unlatch, latchPP)\
    key_t         key;                                              \
    cht_boolean_t latch, unlatch;                                   \
    cht_mutex_t  *latchPP;                                          \
{                  						    \
    cht_bucket_t *bucketP;					    \
    record_t     *currentP, **currentPP;		            \
								    \
    bucketP =  &CHT_CONCAT(prefix,Table)[           		    \
	      ((unsigned int) (hashFunctName(key))) % (tableSize)]; \
    CHT_LATCH_BUCKET;						    \
    currentPP = (record_t **) &bucketP->recordP;        	    \
								    \
    while (*currentPP != (record_t *) 0) {	       	            \
	if (equalFunctName(key,(keyExtractionFunctName(*currentPP)))) {     \
	    currentP = *currentPP;				    \
	    *currentPP = (record_t *) currentP->linkFieldName.recordP;      \
	    CHT_UNLATCH_BUCKET;					    \
	    return(currentP);					    \
	}						      	    \
	currentPP = (record_t **) &(*currentPP)->linkFieldName.recordP;     \
    }								    \
							     	    \
    CHT_UNLATCH_BUCKET;						    \
    return((record_t *) 0);					    \
}								    \
							     	    \
							       	    \
functionUse void CHT_CONCAT(prefix,Del)(recordP, latch, unlatch, latchPP)   \
    record_t     *recordP;                                          \
    cht_boolean_t latch, unlatch;                                   \
    cht_mutex_t  *latchPP;                                          \
{               						    \
    cht_bucket_t *bucketP;					    \
    record_t    **currentPP;                  			    \
							     	    \
    bucketP = recordP->linkFieldName.bucketP;		       	    \
    CHT_LATCH_BUCKET;						    \
    currentPP =  (record_t **) &bucketP->recordP;	            \
						       		    \
    while (*currentPP != (record_t *) 0) {			    \
	if (*currentPP == recordP){				    \
	    *currentPP = (record_t *) recordP->linkFieldName.recordP;       \
	    CHT_UNLATCH_BUCKET;					    \
	    return;						    \
	}							    \
	currentPP = (record_t **) &(*currentPP)->linkFieldName.recordP;     \
    }								    \
    /* specified record is not in table */                          \
    CHT_UNLATCH_BUCKET;						    \
    return;							    \
}								    \
								    \
								    \
functionUse void CHT_CONCAT(prefix,Insert)(recordP, latch, unlatch, latchPP) \
    record_t     *recordP;                                          \
    cht_boolean_t latch, unlatch;                                   \
    cht_mutex_t  *latchPP;                                          \
{               						    \
    cht_bucket_t *bucketP;					    \
								    \
    bucketP = &CHT_CONCAT(prefix,Table)[			    \
	((unsigned int) (hashFunctName(keyExtractionFunctName(recordP)))) %  \
							 (tableSize)];       \
    CHT_LATCH_BUCKET;						    \
    recordP->linkFieldName.recordP = bucketP->recordP;		    \
    recordP->linkFieldName.bucketP = bucketP;			    \
    bucketP->recordP =  (char *) recordP;	       		    \
    CHT_UNLATCH_BUCKET;						    \
    return;							    \
}								    \
								    \
								    \
functionUse cht_boolean_t CHT_CONCAT(prefix,Forall)(f,contextP,latch,unlatch,latchPP) \
    int (*f)();                                                     \
    pointer_t contextP;                                                 \
    cht_boolean_t latch, unlatch;                                   \
    cht_mutex_t  *latchPP;                                          \
{               						    \
    unsigned int i;						    \
    record_t    *currentP, *nextP;				    \
								    \
    for(i=0; i < tableSize; i++) {				    \
	cht_bucket_t *bucketP;					    \
								    \
	bucketP = &CHT_CONCAT(prefix,Table)[i];	                    \
	CHT_LATCH_BUCKET;					    \
	currentP = (record_t *) bucketP->recordP;		    \
	while (currentP != (record_t *) 0) {         	            \
            /* save nextP now, f may delete currentP */		    \
	    nextP = (record_t *) currentP->linkFieldName.recordP;   \
	    if (!f(currentP, contextP))	{			    \
		CHT_UNLATCH_BUCKET;				    \
		return(FALSE);					    \
	    }							    \
	    currentP = nextP;					    \
	}							    \
	CHT_UNLATCH_BUCKET;					    \
    }								    \
    return(TRUE);						    \
}                                                                   \
								    \
								    \
functionUse void CHT_CONCAT(prefix,Analyze)                         \
            (latch, unlatch, latchPP, recordCountP, usedBucketCountP,\
             totalPathLengthP, maxBucketDepthP)                      \
    cht_boolean_t latch, unlatch;                                   \
    cht_mutex_t  *latchPP;                                          \
    int          *recordCountP, *usedBucketCountP, *totalPathLengthP,\
                 *maxBucketDepthP;                                  \
{               						    \
    cht_bucket_t *bucketP;					    \
    record_t     *currentP;         				    \
    int           pathLength;				            \
    unsigned int   i;						    \
								    \
    *recordCountP = *usedBucketCountP = *totalPathLengthP = *maxBucketDepthP = 0; \
    for(i=0; i < tableSize; i++) {				    \
	bucketP = &CHT_CONCAT(prefix,Table)[i];	                    \
	CHT_LATCH_BUCKET;					    \
	currentP = (record_t *) bucketP->recordP;		    \
        if (currentP != (record_t *) 0) {			    \
            (*usedBucketCountP)++;				    \
            pathLength = 0;          				    \
            do {						    \
 	        (*recordCountP)++;                                  \
		(*totalPathLengthP) += ++pathLength;                \
                currentP = (record_t *) currentP->linkFieldName.recordP;\
            } while (currentP != (record_t *) 0);      	            \
            if (pathLength > *maxBucketDepthP)                      \
                *maxBucketDepthP = pathLength;                      \
        }							    \
	CHT_UNLATCH_BUCKET;					    \
    }								    \
    return;							    \
}

#define CHT_LATCH_NONE		FALSE, FALSE, (cht_mutex_t *) NULL

#define CHT_LATCH_MAINTAIN	CHT_LATCH_NONE

#define CHT_LATCH_KEEP		TRUE,  FALSE

#define CHT_LATCH_RELEASE	FALSE, TRUE,  (cht_mutex_t *) NULL

#define CHT_LATCH_BRACKET	TRUE,  TRUE,  (cht_mutex_t *) NULL

#endif /* CHT_HASH_H */
