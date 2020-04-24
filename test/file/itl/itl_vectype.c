/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * COMPONENT_NAME: Encina Development Infrastructure
 *
 * The following functions list may not be complete.
 * Functions defined by/via macros may not be included.
 *
 * ORIGINS: Transarc Corp.
 *
 * (C) COPYRIGHT Transarc Corp. 1992
 * All Rights Reserved
 * Licensed Materials - Property of Transarc
 *
 * US Government Users Restricted Rights - Use, duplication or
 * disclosure restricted by GSA ADP Schedule Contract with Transarc Corp
 */
/*
 * HISTORY
 * $Log: itl_vectype.c,v $
 * Revision 1.1.415.1  1996/10/17  18:27:57  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:54:03  damon]
 *
 * Revision 1.1.410.1  1994/02/04  20:45:55  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:24:47  devsrc]
 * 
 * Revision 1.1.408.1  1993/12/07  17:45:02  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:31:24  jaffe]
 * 
 * Revision 1.1.2.4  1993/01/11  18:31:46  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:11:44  htf]
 * 
 * Revision 1.1.2.3  1992/12/09  21:32:59  jaffe
 * 	Transarc delta: fred-ot6200-write-correct-strings 1.1
 * 	  Selected comments:
 * 	    Fix bug with write command, and incorporate a fix from Ofer for the vector
 * 	    data type.
 * 	    Adde case in convert for a type that cannot be converted.  Return TRUE.
 * 	[1992/12/07  13:35:58  jaffe]
 * 
 * Revision 1.1.2.2  1992/11/18  22:01:05  jaffe
 * 	New File
 * 
 * Revision 1.1.2.2  1992/11/18  22:01:05  jaffe
 * 	New File
 * 
 * 	$TALog: itl_vectype.c,v $
 * 	Revision 1.20  1994/11/08  16:11:59  cfe
 * 	[merge of changes from 1.15 to 1.19 into 1.18]
 * 
 * 	[*** log entries omitted ***]
 * 
 * Revision 1.18  1994/09/15  18:24:09  jess
 * 	Use inlValue_Free instead of OTL_FREE.
 * 
 * 	See above.
 * 	[from r1.17 by delta jess-db6005_fix_itl_mem_leaking, r1.2]
 * 
 * Revision 1.17  1994/09/15  15:20:46  jess
 * 	This delta fixes the ITL memory leak problem from vector component.
 * 
 * 	Export the vectype definition to the header file.
 * 	[from r1.15 by delta jess-db6005_fix_itl_mem_leaking, r1.1]
 * 
 * Revision 1.15  1993/06/03  19:59:02  shl
 * 	[merge of changes from 1.9 to 1.12 into 1.14]
 * 
 * Revision 1.12  1993/04/21  19:12:52  srinivas
 * 	changes for solaris
 * 
 * 	changes for solaris
 * 	[from r1.9 by delta srinivas-itl-1, r1.1]
 * 
 * Revision 1.14  1993/05/24  15:57:59  jess
 * 	[merge of changes from 1.11 to 1.13 into 1.9]
 * 
 * Revision 1.13  1993/05/21  19:20:35  fred
 * 	To import ITL fixes from the Encina group that either fix problems we
 * 	have, or would impact the changes I made to the shellExec command.
 * 
 * 	Note:
 * 	    By accident I made changes in this delta that I should have made in
 * 	    fred-db3575-itl-spoke-thread-safeness-fixes.  So this delta and that
 * 	    one rely on one another.
 * 
 * 	Encina ITL fixes.
 * 	[from r1.11 by delta fred-db3579-encina-itl-fixes-for-shell, r1.1]
 * 
 * Revision 1.9  1993/03/08  13:52:04  fred
 * 	To import fixes to ITL from Ofer.  Also, fixed some outstanding minor
 * 	bugs we found -
 * 	 - The spoke (distributed ITL) had a bug in it's sync point code.  It was
 * 	   not removing a variable from the hash table, which caused confusion when
 * 	   a sync point name was re-used in a tight loop.  Also, it was not
 * 	   setting the timeout field correctly during sync point creation.
 * 
 * 	 - Remove the dependency of the ITL utilities library on the com_err
 * 	   library.  I wrote our own error handling function that deals with both
 * 	   dce and errno correctly.  Besides, the error_message() function in the
 * 	   com_err library is not thread-safe.
 * 
 * 	Incorporation of Encina ITL fixes.
 * 	[from r1.8 by delta fred-db3202-import-encina-itl-fixes, r1.1]
 * 
 * Revision 1.8  1993/02/02  15:44:03  fred
 * 	[merge of changes from 1.5 to 1.7 into 1.6]
 * 
 * Revision 1.7  1993/01/19  21:13:03  fred
 * 	To add the pack/unpack capabilities to ITL variables so that we can do
 * 	better distributed testing.  Also added some other ITL bug fixes from Ofer.
 * 
 * 	Not yet complete.  Need to fix the FX tests.
 * 
 * 	Added pack/unpack interfaces to datatype.
 * 	[from r1.5 by delta fred-ot6848-add-pack-unpack-capability, r1.1]
 * 
 * Revision 1.6  1993/01/14  17:38:53  jaffe
 * 	revdrop of latest OSF copyright changes to these files
 * 	[from r1.5 by delta osf-revdrop-01-12-92, r1.1]
 * 
 * 	Revision 1.4  1992/11/24  21:40:50  fred
 * 	Fix bug with write command, and incorporate a fix from Ofer for the vector
 * 	data type.
 * 
 * 	Adde case in convert for a type that cannot be converted.  Return TRUE.
 * 	[from revision 1.2 by delta fred-ot6200-write-correct-strings, r1.1]
 * 
 * Revision 1.3  1992/11/23  16:16:52  jaffe
 * 	Put in missing header after latest drop to the OSF.
 * 	[from revision 1.2 by delta osf-sync-with-nov-19-drop, r1.1]
 * 
 * 	Revision 1.2  1992/11/04  13:04:31  fred
 * 	Add support for binary data (the ITL vector type) into the read and write
 * 	routines for the file interface library.
 * 
 * 	Changed display and print routines.
 * 	[from revision 1.1 by delta fred-itl-support-binary-data, r1.1]
 * 
 * Revision 1.1  1992/11/03  15:27:58  fred
 * 	Incorporates Ofer's changes that permit procedures to have parameters
 * 	passed to them, and also includes the vector and valList data types.  The
 * 	vector data type is needed for DFS testing.  It allows us to manipulate
 * 	binary data.
 * 	[added by delta fred-itl-ofer-fixes-10-30, r1.1]
 * 
 * Revision 1.4  1992/10/30  18:20:39  oz
 * 	Replaced the use of bcmp with memcmp
 * 	[from revision 1.3 by delta oz-5124-ITL-add-vector-type, r1.5]
 * 
 * Revision 1.3  1992/10/29  21:35:08  oz
 * 	Typo: vectorTypeDef should be passed by ref to to register function
 * 	[from revision 1.2 by delta oz-5124-ITL-add-vector-type, r1.4]
 * 
 * Revision 1.2  1992/10/29  15:41:48  oz
 * 	Fixed small typos and added comments.
 * 	[from revision 1.1 by delta oz-5124-ITL-add-vector-type, r1.3]
 * 
 * Revision 1.1  1992/10/29  15:25:41  oz
 * 	[added by delta oz-5124-ITL-add-vector-type, r1.2]
 * 
 * $EndLog$
 */
#ifdef	SUNOS5
#include <sys/types.h>
#endif

#include <assert.h>
#include <utils/itl/itl_stds.h>
#include <utils/itl/itl.h>
#include <utils/itl/itl_exp.h>
#include <utils/itl/itl_value.h>
#include <utils/itl/itl_vectype.h>

#ifndef MIN
#define MIN(A,B) ((A) < (B) ? (A) : (B))
#endif

#define VEC_PLEN 80

SHARED int itl_vectorType;


#define ITL_VEC_VALUE_VAL(valP)  ((itl_vectorType_t *)VALUE_VAL(valP))
#define ITL_VEC_VALUE_DATA(valP)  (ITL_VEC_VALUE_VAL(valP)->data)

/*
 *  use the macro VALUE_SIZE(valP) to get the length of the data
 */

typedef enum {
    allocated_vector,   /* space has been allocated */
    bad_vector          /* no space allocated, but allow use of this vector.
			 * The data pointer will be NULL, length intdeterminate
			 */
} itl_vector_state_t;

typedef struct {
    itl_vector_state_t state;  /* allocated or bad_vector */
    unsigned long      len;
    char      	      *data;
} itl_vectorType_t;


PRIVATE char* PrintVectorBytes _TAKES((
  IN var_val_t* valP,
  IN int numBytes,
  IN int *lenP
));

PRIVATE var_val_t *NewVector(ptr, type)
    IN pointer_t ptr;
    IN int type;
{
    itl_vectorType_t *newVectorP = (itl_vectorType_t *)ptr;
    var_val_t *newValP;

    ITL_NEW(newValP, var_val_t);
    VALUE_TYPE(newValP) = type;
    VALUE_SIZE(newValP) = newVectorP->len;
    VALUE_VAL(newValP) = ptr;
    return(newValP);
}

PRIVATE var_val_t *CopyVector(valP)
  IN var_val_t *valP;
{
    itl_vectorType_t *oldVectorP = ITL_VEC_VALUE_VAL(valP);
    itl_vectorType_t *newVectorP;

    assert(VALUE_TYPE(valP) == itl_vectorType);
    ITL_NEW(newVectorP, itl_vectorType_t);
    *newVectorP = *oldVectorP;
    if (oldVectorP->state == allocated_vector) {
	ITL_ALLOC(newVectorP->data, char *, oldVectorP->len);
	ITL_MEMCOPY(oldVectorP->data, newVectorP->data,
		    oldVectorP->len);
    }
    return(itlValue_New(newVectorP, VALUE_TYPE(valP)));
}

PRIVATE boolean_t FreeVector(valP)
  IN var_val_t *valP;
{
    itl_vectorType_t *vectorP = ITL_VEC_VALUE_VAL(valP);
    if (vectorP->state == allocated_vector)
	ITL_FREE(vectorP->data);
    ITL_FREE(vectorP);
    return(TRUE);
}

/* Read a vector:
 *
 * - Input format - can be entered in several ways:
 *     - an ordinary string
 *     - a string starting with "0x".  The rest of the string will be
 *       treated as hex data
 *     - am integer.  
 *     - The special string NULL.  A NULL pointer will be generated if
 *       an attempt is made to use this buffer.
 */

PRIVATE var_val_t *ReadVector(valP, type)
  IN var_val_t *valP;
  IN int type;
{
    itl_vectorType_t *VectorP, Vector;
    int count, i;
    char buf[256];
    boolean_t b;

    if (!valP) {
	itl_PromptUser("Vector: ");
	valP = itl_GetVarVal();
    }

    if ((VALUE_TYPE(valP) == itl_stringType) &&
	(strcasecmp(STR_VALUE_VAL(valP), ITL_MAGIC_NULL_STR) == 0)) {
      /*
       *  Special case, an invalid vector
       */
      itl_vectorType_t badVector;
      var_val_t *badP;
      badVector.state = bad_vector;
      badVector.len = -1;
      badVector.data = NULL;
      badP = itlValue_New((pointer_t)&badVector, itl_vectorType);

      itl_FreeVarVal(valP);
      valP = itlValue_Copy(badP);
      ITL_FREE(badP);
      return(valP);
    }

    /* attempt a conversion */
    if (itlValue_ConvertValue(valP, itl_vectorType)) {
	return(valP);
    } else {
	itl_FreeVarVal(valP);
	return(NULL);
    }
}

/* get the length of a data vector */

PRIVATE boolean_t UnaryOpVector(inVal, Op)
  IN var_val_t *inVal;
  IN char *Op;
{
    int len;
    itl_vectorType_t *vecP;
	
    if (*Op == '#') {
	var_val_t *newValP = itlStds_IntVal(VALUE_SIZE(inVal));

	itlValue_Free(inVal);
	*inVal = *newValP;
	ITL_FREE(newValP);
	return FALSE;
    } else {
	return TRUE;
    }
}

/* Operations on vectors:
 *
 * ==
 * !=
 * * (replicate: vector LHS, integer RHS)
 * + (concatenate: vectors on both sides)
 * <<, >> - rotate left, right - vector lhs, integer rhs
 *  % truncate to length - vector lhs, integer rhs
 * Vectors given to + and * should be "allocated_vector"
 */


/*
 *  ReplicateVector
 *	Replicate the input vector the desired number of times
 *	and write it into the output vector.  Each of the vectors is a string.
 *	The number, num, must be >= 0
 *	The output string is preallocated and has enough space
 *	(for the terminating null as well)
 *  Logic:
 *	1- Create a vector of half the desired size by a recursive call
 *	2- Duplicate the half by using memcopy
 *
 *  Using this algorithm, the total number of copies is logarithmic
 *  in the replication number.
 *	
 */
PRIVATE void ReplicateVector _TAKES((
  IN	char *inStr,
  IN	int   len,	/*  The length of inStr  */
  IN	int   num,
  OUT	char *outStr
));
PRIVATE void ReplicateVector(inStr, len, num, outStr)
  IN	char *inStr;
  IN	int   len;	/*  The length of inStr  */
  IN	int   num;
  OUT	char *outStr;
{
    int halfSize = len * ((num+1) / 2);
    int size = len * num;
    assert(num >= 0);
    if (num <= 1){
	ITL_MEMCOPY(inStr, outStr, size);
    } else {
	ReplicateVector(inStr, len, (num+1)/2, outStr);
	ITL_MEMCOPY(outStr, outStr + halfSize, size - halfSize);
    }
}

PRIVATE boolean_t VectorShift _TAKES((
  IN var_val_t *leftVal,
  IN var_val_t *rightVal,
  IN boolean_t leftShift
));
PRIVATE boolean_t VectorShift(leftVal, rightVal, leftShift)
  IN var_val_t *leftVal;
  IN var_val_t *rightVal;
  IN boolean_t leftShift;
{
    int shiftVal;
    char *prefix;
    char *str = ITL_VEC_VALUE_DATA(leftVal);
    int len = VALUE_SIZE(leftVal);
    if (!itlValue_ConvertValue(rightVal, itl_intType))
	return TRUE;
    shiftVal = INT_VALUE_VAL(rightVal) % len;

    if ((shiftVal == 0) || (len == 0))
	return FALSE;

    if ( !leftShift ) {
	shiftVal = -shiftVal;
    }
    if (shiftVal < 0)
	shiftVal += len;

    ITL_ALLOC(prefix, char *, shiftVal);
    ITL_MEMCOPY(str, prefix, shiftVal);
    ITL_MEMCOPY(&str[shiftVal], str, (len - shiftVal));
    ITL_MEMCOPY(prefix, &str[len - shiftVal], shiftVal);
    ITL_FREE(prefix);
    return FALSE;
}

PRIVATE boolean_t VectorCmp _TAKES((
  INOUT var_val_t *leftVal,
  IN var_val_t *rightVal,
  IN char *Op
));
PRIVATE boolean_t VectorCmp(leftVal, rightVal, Op)
  INOUT var_val_t *leftVal;
  IN var_val_t *rightVal;
  IN char *Op;
{
    int cmpVal;
    var_val_t *newVal;
    int res;
    int minLen = MIN(VALUE_SIZE(leftVal), VALUE_SIZE(rightVal));

    if (!itlValue_ConvertValue(rightVal, itl_vectorType))
	return TRUE;
    cmpVal = memcmp(ITL_VEC_VALUE_DATA(leftVal),
		    ITL_VEC_VALUE_DATA(rightVal),
		    minLen);
    if ((cmpVal == 0) && (VALUE_SIZE(leftVal) != VALUE_SIZE(rightVal))) {
	cmpVal = (VALUE_SIZE(leftVal) > minLen ? -1 : 1);
    }
		  
    switch (*Op) {
      case '<':
	res =  (Op[1] == '=') ? cmpVal <= 0 : cmpVal < 0;
	break;
      case '=':
	res =  cmpVal == 0 ;
	break;
      case '>':
	res =  (Op[1] == '=') ? cmpVal >= 0 : cmpVal > 0;
	break;
      case '!':
	res =  cmpVal != 0;
	break;
    }
    newVal = itlStds_IntVal(res);
    itlValue_Free(leftVal);
    *leftVal = *newVal;
    ITL_FREE(newVal);
    return FALSE;
}


PRIVATE boolean_t BinOpVector(leftVal, rightVal, Op)
  INOUT var_val_t *leftVal;
  IN var_val_t *rightVal;
  IN char* Op;
{
    boolean_t retVal = FALSE;
    int newSize;
    int rightValue;

    if (ITL_VEC_VALUE_VAL(leftVal)->state == bad_vector) {
	return TRUE;
    }

    switch (*Op) {
      case '*':  {
	  /*  Pattern creation by duplication:
	   *  The right side is interpreted as an integer:
	   */
	  if (!itlValue_ConvertValue(rightVal, itl_intType))
	      return TRUE;
	  rightValue = INT_VALUE_VAL(rightVal);
	  
	  if (rightValue < 0) {
	      rightValue = 0;
	  } 
	  newSize = VALUE_SIZE(leftVal) * rightValue;
	  ITL_UTIL_REALLOC(ITL_VEC_VALUE_DATA(leftVal), char *, newSize);
	  ReplicateVector(ITL_VEC_VALUE_DATA(leftVal),
			  VALUE_SIZE(leftVal),
			  rightValue,
			  ITL_VEC_VALUE_DATA(leftVal));
	  ITL_VEC_VALUE_VAL(leftVal)->len = newSize;
	  VALUE_SIZE(leftVal) = newSize;
	  break;
      }
      case '%': {	/*  Truncate to the desired size  */
	  if (!itlValue_ConvertValue(rightVal, itl_intType)) {
	      return TRUE;
	  }
	  rightValue = INT_VALUE_VAL(rightVal);
	  if ((VALUE_SIZE(leftVal) > rightValue) &&
	      (rightValue >= 0)) {
	      VALUE_SIZE(leftVal) = rightValue;
	      ITL_VEC_VALUE_VAL(leftVal)->len = rightValue;
	      ITL_REALLOC(VALUE_VAL(leftVal), pointer_t, VALUE_SIZE(leftVal));
	  }
	  break;
      }
      case '+': {
	  /*
	   *  concatenation
	   */
	  if (!itlValue_VarValConvert(rightVal, itl_vectorType)) {
	      return TRUE;
	  }
	  
	  newSize = VALUE_SIZE(leftVal) + VALUE_SIZE(rightVal);
	  ITL_UTIL_REALLOC(ITL_VEC_VALUE_DATA(leftVal), char *, newSize);
	  
	  ITL_UTIL_MEMCOPY(ITL_VEC_VALUE_DATA(rightVal),
			   ITL_VEC_VALUE_DATA(leftVal) + VALUE_SIZE(leftVal),
			   VALUE_SIZE(rightVal));
	  ITL_VEC_VALUE_VAL(leftVal)->len = newSize;
	  VALUE_SIZE(leftVal) = newSize;
	  break;
      }
      case '<':
	if (Op[1] == '<') {
	    retVal = VectorShift(leftVal, rightVal, TRUE);
	}
	else if ((Op[1] != '\0') && (Op[1] != '=')) {
	    leftVal = NULL;
	    retVal = TRUE;
	}
	else {
	    retVal = VectorCmp(leftVal, rightVal, Op);
	}
	break;

      case '=':
	if (Op[1] != '=') {
	    leftVal = NULL;
	    retVal = TRUE;
	}
	else {
	    retVal = VectorCmp(leftVal, rightVal, Op);
	}
	break;
      case '!':
	if (Op[1] != '=') {
	    leftVal = NULL;
	    retVal = TRUE;
	}
	else {
	    retVal = VectorCmp(leftVal, rightVal, Op);
	}
	break;
      case '>':
	if (Op[1] == '>') {
	    retVal = VectorShift(leftVal, rightVal, FALSE);
	}
	else if ((Op[1] != '\0') && (Op[1] != '=')) {
	    retVal = TRUE;
	}
	else {
	    retVal = VectorCmp(leftVal, rightVal, Op);
	}
	break;

      default: 
	return(TRUE);
    }
    return(retVal);
}

#define HEXVAL(c)                                    \
    ((((c) >= '0') && ((c) <= '9')) ? ((c)-'0') :    \
     ((((c) >= 'a') && ((c) <= 'f')) ? (c)-'a'+0xa : \
      ((((c) >= 'A') && ((c) <= 'F')) ? (c)-'A'+0xA : -1)))

#define HEXCHAR(c) (((c) >= 0) && ((c) <= 9) ?  '0' + (c) :\
		    (((c) >= 0xa) && ((c) <= 0xf)) ? 'a' + (c) - 0xa : '.')


PRIVATE boolean_t ConvertVector(valP, type)
  INOUT var_val_t *valP;
  IN int type;
{
    var_val_t *newP;

    if (VALUE_TYPE(valP) == itl_vectorType) {
	/* convert vector valP to "type" */

	if (type == itl_intType) {
	    int val = 0;
	    ITL_MEMCOPY(ITL_VEC_VALUE_DATA(valP), &val,
			MIN(VALUE_SIZE(valP), sizeof(int)));
	    newP = itlStds_IntVal(val);
	} else if (type == itl_stringType) {
	    int len;
	    char *str = PrintVectorBytes(valP, -1, &len);
	    newP = itlValue_New((pointer_t)str, itl_stringType);
	} else {
	    return(TRUE);
	}
    } else {
	/* convert data item in valP to vector */
	itl_vectorType_t vector;

	assert(type == itl_vectorType);
	if (VALUE_TYPE(valP) == itl_stringType) {
	    char *src = STR_VALUE_VAL(valP);
	    if ((src[0] == '0') && (tolower(src[1]) == 'x')) {
		int i;
		int size = 0;
		for (i=2; i<VALUE_SIZE(valP)-1; i++) {
		    /*
		     *  We write the new value in place.  Index is
		     *  the index of the byte we are currently writing
		     */
		    if ((src[i] != '-') &&
			(src[i] != ' ') &&
			(src[i] != '\t')) {
			unsigned char byte = HEXVAL(src[i]);
			if (byte >= 16)
			    return(TRUE);
			if ( size % 2 == 0) {
			    src[size/2] = byte << 4;
			} else {
			    src[size/2] |= byte;
			}
			size++;
		    }
		}
		newP = itl_SetVectorType(src, (size+1)/2);
	    } else {
		newP = itl_SetVectorType(src, VALUE_SIZE(valP)-1);
	    }
	    
	} else if (VALUE_TYPE(valP) == itl_intType) {
	    newP = itl_SetVectorType((char *)VALUE_VAL(valP),
				     VALUE_SIZE(valP));
	} else if (VALUE_TYPE(valP) == itl_procType) {
	    newP = itl_SetVectorType(PROC_VALUE_VAL(valP), VALUE_SIZE(valP));
	} else {
	    return(TRUE);
	}
    }
    itlValue_Free(valP);
    *valP = *newP;
    ITL_FREE(newP);
    return FALSE;
}

#define CP_MSG(m,b) \
    MACRO_BEGIN \
    int len = strlen(m)+1; \
    ITL_ALLOC((b), char*, len); \
    ITL_MEMCOPY((m),(b),len); \
    MACRO_END


PRIVATE char* PrintVectorBytes(valP, numBytes, lenP)
  IN var_val_t* valP;
  IN int numBytes;
  IN int *lenP;
{
    itl_vectorType_t *sP = ITL_VEC_VALUE_VAL(valP);
    char *bufP;
    int dlen, i, ind;
    int len;
    boolean_t printable;

    switch (sP->state) {
      case allocated_vector:

	/*  Show the first few characters in the string
	 *  First, check if the vector is printable, and if so, print
	 *  as a string
	 */
	dlen = MIN(sP->len, numBytes);
	if (dlen <= 12) {

	    printable = TRUE;
	    for (i=0; i<dlen; i++) {
		if (!isprint(sP->data[i])) {
		    printable = FALSE;
		    break;
		}
	    }

	    if (printable) {
		ITL_ALLOC(bufP, char *, dlen + 27);
		ITL_MEMCOPY(sP->data, bufP, dlen);
		bufP[dlen] = '\0';
	    } else {
		ITL_ALLOC(bufP, char *, dlen * 3 + 30);

		if (dlen > 0) {
		    sprintf(bufP, "0x-");
		    ind = 3;

		    for (i=0; i < dlen; i++) {
			char c = sP->data[i];
			bufP[ind++] = HEXCHAR((c>>4) & 0xf);
			bufP[ind++] = HEXCHAR(c & 0xf);
			if ((((i+1) % 2) == 0) && (i+1 < dlen)) {
			    bufP[ind++] = '-';
			}
		    }
		} else {
		    ind = 0;
		}
		bufP[ind++] = '\0';
	    }
	    if (lenP) {
		*lenP = strlen(bufP);
	    }
	} else {
	    unsigned long newLen;
	    bufP = itl_DisplayBytes(sP->data, dlen, TRUE, &newLen);
	    if (lenP) {
		*lenP = newLen;
	    }
	    
	}
	break;

      case bad_vector:
	CP_MSG("(NULL)", bufP);
	if (lenP) {
	    *lenP = strlen(bufP);
	}
	break;
      default:
	itl_Error("Bad vector_state\n");
	bufP = NULL;
	if (lenP) {
	    *lenP = 0;
	}
    }
    return bufP;
}

PRIVATE char* PrintVector(valP, lenP)
  IN var_val_t* valP;
  IN int *lenP;
{
    return(PrintVectorBytes(valP, 12, lenP));
}


PRIVATE char* DisplayVector(valP, lenP)
  IN var_val_t* valP;
  IN int *lenP;
{
    return(PrintVectorBytes(valP, -1, lenP));
}

/* Scan a string of the form "name[value]".
 * return the value in index.
 * shorten the string to just "name"
 */

PRIVATE boolean_t MatchArray _TAKES((
  INOUT char* fieldname,
  OUT int* index
));
PRIVATE boolean_t MatchArray(fieldname, index)
  INOUT char* fieldname;
  OUT int* index;
{
    int len = strlen(fieldname);
    char *p1P, *p2P;
    int val;

    p2P = fieldname+len-1;
    if (*p2P != ']')
	return FALSE;
    p1P = strchr(fieldname, '[');

    /* p1P allegedly points to [ followed by a decimal integer */
    if (sscanf(p1P+1, "%d", &val) != 1) {
	itl_Message("Bad vector subscript - must be an integer\n");
	return FALSE;
    }
    *p1P = '\0';
    *index = val;
    return TRUE;
}

PRIVATE var_val_t *GetFieldVector(valP, fieldName)
  IN var_val_t *valP;
  IN char *fieldName;
{
    itl_vectorType_t *cP = ITL_VEC_VALUE_VAL(valP);
    int i, index, *iP;
    var_val_t *vP, *rvP;
    boolean_t array;

    if (strncmp(fieldName, "length", 5) == 0) {
	vP = itlValue_New(&cP->len, itl_intType);
	rvP = itlValue_Copy(vP);
    } else if (strncmp(fieldName, "data", 4) == 0) {
	array = MatchArray(fieldName, &index);
	if (!array) {
	    itl_Message("Specify which byte!\n");
	return NULL;
	}
	if ((array >= cP->len) || (array < 0)) {
	    itl_Message("Not that many bytes\n");
	    return NULL;
	}
	ITL_NEW(iP, int); *iP = cP->data[index]; 
	vP = itlValue_New(iP, itl_intType);
	rvP = itlValue_Copy(vP);
    } else {
	itl_Message("Bad field name for a vector");
	return NULL;
    }
    ITL_FREE(vP);
    return rvP;
}

PRIVATE itlValue_varTypeInfo_t vectorTypeDef = {
    "vector",
    CopyVector,
    NewVector,
    FreeVector,
    ReadVector,
    BinOpVector,
    UnaryOpVector,
    ConvertVector,
    PrintVector,
    DisplayVector,
    GetFieldVector,
    NULL,
    NULL,
    NULL,
    "Arbitrary byte vectors.",
    "The vector type is used to hold arbitrary byte vectors.  When setting \
a vector, the user can specify an integer (which is converted to a 4 byte \
vector, or a string.  If the string starts with 0x, it is interpreted as \
a long sequence of nibbles, where white spaces and dashes are ignored \
(e.g., 0x1234-ab12-ccdd-0001).  Otherwise, the string is copied to the \
vector without the null terminator.\n\n\
Fields:\n\
  length - to obtain the length (which can also be obtained via the # \
unary operator)\n\
  data[%d] - a specified byte where %d is replaced by an int (e.g. data[3])\n\
Operations: Vectors support + for concatenation, % for truncation, * for \
replication, and <<,>> for shifting (and also logical comparisons such \
as ==, != etc." 
};

SHARED var_val_t *itl_SetVectorType(value, size)
  IN	char	*value;
  IN	int	 size;
{
    var_val_t  *tempP;
    itl_vectorType_t  myVec;
    var_val_t  *tempValP;

    myVec.len = size;
    myVec.state = allocated_vector;
    myVec.data = value;
    tempP = itlValue_New((pointer_t)&myVec, itl_vectorType);
    tempValP = itlValue_Copy(tempP);
    ITL_FREE(tempP);
    return(tempValP);
}

SHARED void itl_VecTypeInit()
{
    itl_vectorType = itlValue_RegisterVarType(&vectorTypeDef);
}

SHARED pointer_t itlVector_Value(valP)
  IN var_val_t *valP;
{
    if (VALUE_TYPE(valP) != itl_vectorType)
	return NULL;
    else
	return ITL_VEC_VALUE_DATA(valP);
}

