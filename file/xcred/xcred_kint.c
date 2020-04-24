/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xcred_kint.c,v $
 * Revision 1.1.489.1  1996/10/02  19:03:16  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:51:44  damon]
 *
 * Revision 1.1.484.3  1994/07/13  22:30:47  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  12:12:06  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  16:04:54  mbs]
 * 
 * Revision 1.1.484.2  1994/06/09  14:25:44  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:37:42  annie]
 * 
 * Revision 1.1.484.1  1994/02/04  20:36:28  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:20:45  devsrc]
 * 
 * Revision 1.1.482.1  1993/12/07  17:38:31  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/07  16:55:30  jaffe]
 * 
 * $EndLog$
 */
/*
 * (C) Copyright 1995, 1990, 1993 Transarc Corporation
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 *
 * xcred_kint.c
 *	Implementation of the package for manipulating extended credentials
 *	via the system call interface.
 */
#include <dcedfs/param.h>		/*AFS system definitions*/
#include <dcedfs/sysincludes.h>	/*Basic kernel include files*/
#include <dcedfs/osi.h>		/*osi_suser()*/
#include <dcedfs/osi_cred.h>
#include <xcred_kint.h>		/*Exported interface*/

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/xcred/xcred_kint.c,v 1.1.489.1 1996/10/02 19:03:16 damon Exp $")

#ifdef  TESTING_XCRED_SYSCALL
/*
 * Define the arguments actually appearing in the user area for this syscall.
 */
typedef struct {
    long aopcode;			/*Opcode to execute*/
    xcred_kint_opargs_t *aopargsP;	/*Ptr to opcode args*/
} xcred_kint_args_t;


/*------------------------------------------------------------------------
 * PRIVATE xcred_kint_PrintPListEntry
 *
 * Description:
 *	Routine passed to xcred_EnumerateProp() to print out each plist
 *	entry attached to an xcred.
 *
 * Arguments:
 *	arockP	: Currently unused.
 *	axcredP	: Ptr to related xcred.
 *	aPLP	: Ptr to plist entry upon which to operate.
 *
 * Returns:
 *	0 if everything went well,
 *	Otherwise, returns whatever the lower-level routines do.
 *
 * Environment:
 *	Nothing interesting.
 *
 * Side Effects:
 *	Prints to the debugging output.
 *------------------------------------------------------------------------*/

static long xcred_kint_PrintPListEntry(arockP, axcredP, aPLP)
    char *arockP;
    xcred_t *axcredP;
    xcred_PListEntry_t *aPLP;

{ /*xcred_kint_PrintPListEntry*/

	printf("\nPlist entry 0x%x for xcred 0x%x:\n", aPLP, axcredP);
	printf("\tprevP=0x%x, nextP=0x%x\n", aPLP->prevP, aPLP->nextP);
	printf("\tderiv: baseP=0x%x, childP=0x%x, prevSiblingP=0x%x, nextSiblingP=0x%x\n",
	       aPLP->deriv.baseP,
	       aPLP->deriv.childP,
	       aPLP->deriv.prevSiblingP,
	       aPLP->deriv.nextSiblingP);
    if (aPLP->deriv.baseP)
	printf("\t[Derived from attribute '%s']\n", aPLP->deriv.baseP->attributeP);
    printf("\tattr at 0x%x, '%s' (%d bytes), value at 0x%x (%d bytes), flags=0x%x\n",
	   aPLP->attributeP, aPLP->attributeP, aPLP->attrBytes,
	   aPLP->valueP, aPLP->valueBytes, aPLP->flags);

    return(0);

} /*xcred_kint_PrintPListEntry*/

/*------------------------------------------------------------------------
 * PRIVATE xcred_kint_TestSuite
 *
 * Description:
 *	Routine to exercise the extended credential package.
 *
 * Arguments:
 *	None.
 *
 * Returns:
 *	0 if everything went well.
 *	Otherwise, returns whatever the xcred routines do.
 *
 * Environment:
 *	Nothing interesting.
 *
 * Side Effects:
 *	Fiddles with just about everything, then removes all its
 *	effects before returning.
 *------------------------------------------------------------------------*/

static int xcred_kint_TestSuite()
{ /*xcred_kint_TestSuite*/

#define VALBUFF_CHARS 128

    static char rn[] = "xcred_kint_TestSuite";	/*Routine name*/
    int code;					/*Return code*/
    xcred_t *xcredP;				/*Ptr to xcred*/
    xcred_t *checkxcredP;			/*Checks xcred attachment*/
    osi_cred_t testucred;			/*Test ucred*/
    osi_cred_t *testucredP =  &testucred;	/*Ptr to above*/
    static char attr1[] = "attr1";		/*Test attribute 1*/
    static char attr2[] = "attr2";		/*Test attribute 2*/
    static char attr3[] = "attr3";		/*Test attribute 3*/
    static char attr4[] = "attr4";		/*Test attribute 4*/
    static char value1[] = "value1";		/*Test value 1*/
    static char value2[] = "value2";		/*Test value 2*/
    static char value3[] = "value3";		/*Test value 3*/
    static char value4[] = "value4";		/*Test value 4*/
    char valbuff[VALBUFF_CHARS];		/*Buffer for value contents*/
    char *valbuffP = &valbuff[0];		/*Ptr to above array*/
    long valchars;				/*Num chars returned in above*/

    /*
     * Create an xcred to play with.
     */
    code = xcred_Create(&xcredP);
    if (code)
	return(code);
    xcred_kint_PrintXCred(xcredP, DEBUG_LEVEL_1);

    /*
     * Hold and release the new xcred.
     */
    code = xcred_Hold(xcredP);
    if (code)
	return(code);
    xcred_kint_PrintXCred(xcredP, DEBUG_LEVEL_1);

    code = xcred_Release(xcredP);
    if (code)
	return(code);
    xcred_kint_PrintXCred(xcredP, DEBUG_LEVEL_1);

    /*
     * Copy our own process' Unix cred into a test cred structure,
     * then attach our xcred to it.
     */
    *testucredP = *u.u_cred;
    code = xcred_AssociateCreds(xcredP, testucredP);
    if (code)
	return(code);
    code = xcred_Release(xcredP);
    if (code) {
	return(code);
    }
    xcred_kint_PrintUCred(testucredP, DEBUG_LEVEL_1);

    /*
     * Just to make sure the above worked, turn right around and get the
     * xcred associated with the test ucred.  Note: make sure to bring the
     * xcred's refcount back down.
     */
    code = xcred_UCredToXCred(testucredP, &checkxcredP);
    if (code) {
	return(code);
    }
    code = xcred_Release(checkxcredP);
    if (code) {
	return(code);
    }
    if (checkxcredP != xcredP) {
	return(EINVAL);
    }

    /*
     * Put a few properties on the xcred's plist, with some derivation
     * relationships.
     */
    code = xcred_PutProp(xcredP, attr1, value1, strlen(value1)+1, 0, (char *)0);
    if (code)
	return(code);
    code = xcred_PutProp(xcredP, attr2, value2, strlen(value2)+1, 0, (char *)0);
    if (code)
	return(code);
    code = xcred_PutProp(xcredP, attr3, value3, strlen(value3)+1, 0, attr1);
    if (code)
	return(code);

    code = xcred_PutProp(xcredP, attr4, value4, strlen(value4)+1, 0, attr1);
    if (code)
	return(code);

    code = xcred_GetProp(xcredP, attr1, VALBUFF_CHARS, &valbuffP, &valchars);
    if (code)
	return(EINVAL);
    if (strcmp(valbuff, value1) !=  0) {
	printf("%s: Value stored with attr1 incorrect; should be '%s', it's '%s'\n",
	       rn, value1, valbuff);
	return(EINVAL);
    }

    /*
     * Get the fourth property's value.
     */
    code = xcred_GetProp(xcredP, attr4, 128, &valbuffP, &valchars);
    if (code)
	return(EINVAL);
    if (strcmp(valbuff, value4) !=  0) {
	printf("%s: Value stored with attr4 incorrect; should be '%s', it's '%s'\n",
	       rn, value4, valbuff);
	return(EINVAL);
    }

    /*
     * Enumerate the properties sitting on the xcred.
     */
    code = xcred_EnumerateProp(xcredP, xcred_kint_PrintPListEntry, (char *)0);
    if (code)
	return(code);

    /*
     * Delete the third property, which was derived from the first.
     */
    code = xcred_PutProp(xcredP, attr3, (char *)0, 0, 0, (char *)0);
    if (code)
	return(code);

    /*
     * Enumerate the properties sitting on the xcred.
     */
    code = xcred_EnumerateProp(xcredP, xcred_kint_PrintPListEntry, (char *)0);
    if (code)
	return(code);

    /*
     * Delete the first property, which should get rid of all the rest.
     */
    code = xcred_PutProp(xcredP, attr1, (char *)0, 0, 0, (char *)0);
    if (code)
	return(code);

    /*
     * Enumerate the properties sitting on the xcred.
     */
    code = xcred_EnumerateProp(xcredP, xcred_kint_PrintPListEntry, (char *)0);
    if (code)
	return(code);

    code = xcred_PutProp(xcredP, attr2, (char *)0, 0, 0, (char *)0);
    if (code)
	return(code);

    /*
     * Finally, release our play xcred and return the results.
     */
    code = xcred_Release(xcredP);
    return(code);

} /*xcred_kint_TestSuite*/



/*------------------------------------------------------------------------
 * EXPORT afscall_xcredkint
 *
 * Environment:
 *	This is a trap routine.  The user's structure has a u_ap field
 *	that holds the parameters we need, and we use xcred_kint_args_t
 *	to view them correctly.
 *
 * Side Effects:
 *	As advertised.
 *------------------------------------------------------------------------*/

/* EXPORT */
int afscall_xcredkint(opcode, parm2, ignoredParm3, ignoredParm4, ignoredParm5,
			     retvalP)
     long	opcode;
     void *	parm2;
     long	ignoredParm3, ignoredParm4, ignoredParm5;
     int *	retvalP;
{ /* afscall_xcredkint */

    static char rn[] = "afscall_xcredkint";	/*Routine name*/
    register xcred_kint_args_t *uaP =
        (xcred_kint_args_t *)(u.u_ap);		/*Ptr to syscall args*/
    xcred_kint_opargs_t uopa;			/*Opcode args in kernel space*/
    xcred_t *xcredP;				/*Ptr to xcred*/
    int code;					/*Return code*/

    /*
     * Make sure our caller is root.
     */
    if (!osi_suser(osi_getucred())) {
	return(EACCES);
    }

    *retvalP = 0;

    /*
     * Copy the opcode arguments in from user space.
     */
    if (parm2 != (void *) 0) {
	code = osi_copyin(parm2, &uopa, sizeof (xcred_kint_opargs_t));
    }

    /*
     * Dispatch on the opcode.
     */
    switch (opcode) {
      case XCRED_SYSCALL_SET_DEBUG_LEVEL:
        /*
	 * Set the xcred debugging level.
	 */
	afsdebug[XCRED_DEBUG] = uopa.adbLevel.anewDBLevel;
	code = 0;
        break;

      case XCRED_SYSCALL_CREATE:
        /*
	 * Create a new xcred.
	 */
	code = xcred_Create(&xcredP);
	if (code)
	    break;
	code = osi_copyout((caddr_t)&xcredP,
			   (caddr_t)uopa.acreate.anewXCredPP,
			   sizeof (uopa.acreate.anewXCredPP));
        break;

      case XCRED_SYSCALL_HOLD:
	/*
	 * Hold a given xcred.
	 */
	code = xcred_Hold(uopa.ahold.axcredP);
	break;

      case XCRED_SYSCALL_RELEASE:
	/*
	 * Release a given xcred.
	 */
	code = xcred_Release(uopa.arel.axcredP);
        break;

      case XCRED_SYSCALL_ASSOCIATE_CREDS:
	/*
	 * Associate a given ucred and xcred.
	 * -- Currently not implemented.
	 */
	code = EINVAL;
        break;

      case XCRED_SYSCALL_UCRED_TO_XCRED:
	/*
	 * Return the xcred associated with the given xcred.
	 * -- Currently not implemented.
	 */
	code = EINVAL;
        break;

      case XCRED_SYSCALL_XCRED_TO_UCRED:
	/*
	 * Return the ucred associated with the given ucred.
	 * -- Currently not implemented.
	 */
	code = EINVAL;
        break;

      case XCRED_SYSCALL_PUT_PROP:
	/*
	 * Put an attribute/value pair on a xcred's property list.
	 * -- Currently not implemented.
	 */
	code = EINVAL;
	break;

      case XCRED_SYSCALL_GET_PROP:
	/*
	 * Get the value for the given attribute on the xcred's property list.
	 * -- Currently not implemented.
	 */
	code = EINVAL;
	break;

      case XCRED_SYSCALL_ADD_GC_PROC:
	/*
	 * Add the given proc/rock to the xcred's list of garbage collectors.
	 * -- Currently not implemented.
	 */
	code = EINVAL;
	break;

      case XCRED_SYSCALL_DELETE_GC_PROC:
	/*
	 * Remove the given proc/rock from the xcred's list of garbage
	 * collectors. -- Currently not implemented.
	 */
	code = EINVAL;
	break;

      case XCRED_SYSCALL_GET_GC_PROC:
	/*
	 * See if the given proc/rock is on the xcred's list of garbage
	 * collectors. -- Currently not implemented.
	 */
	code = EINVAL;
	break;

      case XCRED_SYSCALL_ENUMERATE_PROP:
	/*
	 * Enumerate the next item on the xcred's property list.
	 */
	code = EINVAL;
	break;

      case XCRED_SYSCALL_TEST_SUITE:
	/*
	 * Run a test suite on the extended credential package.
	 */
	code = xcred_kint_TestSuite();
	break;

      default:
	code = EINVAL;
    } /*Opcode dispatch*/

    /*
     * The shooting's over.  Tell our caller what happened.
     */
    return(code);

} /* afscall_xcredkint */

#endif  /* TESTING_XCRED_SYSCALL */

/*------------------------------------------------------------------------
 * EXPORT xcred_kint_PrintUCred
 *
 * Environment:
 *	Nothing interesting.
 *
 * Side Effects:
 *	May print to the debugging output.
 *------------------------------------------------------------------------*/

/* EXPORT */
void xcred_kint_PrintUCred(aucredP, alevel)
    IN osi_cred_t *aucredP;
    IN int alevel;

{ /*xcred_kint_PrintUCred*/

    static char rn[] = "xcred_kint_PrintUCred";	/*Routine name*/

#if AFSDEBUG
    long code;		/*Return code from xcred_UCredToXCred()*/
    xcred_t *xcredP;	/*Ptr to associated xcred, if any*/

    code = xcred_UCredToXCred(aucredP, &xcredP);
    if (code) {
	printf("%s: ** Can't get xcred from ucred at 0x%x\n",
	       rn, aucredP);
	return;
    }

    /*
     * Since we're just printing out the xcred, drop the refcount that
     * was bumped by xcred_UCredToXCred().
     */
    code = xcred_Release(xcredP);
    if (code) {
	printf("%s: ** Can't release xcred at 0x%x after xcred_Release()\n",
	       rn, xcredP);
    }

    printf("\tref=%d, euid=%d, egid=%d, ruid=%d, rgid=%d, xcredP=0x%x\n",
	   aucredP->cr_ref, osi_GetUID(aucredP), osi_GetGID(aucredP),
	   osi_GetRUID(aucredP), aucredP->cr_rgid, xcredP);
    if (xcredP)
	xcred_kint_PrintXCred(xcredP, alevel);
#endif AFSDEBUG

} /*xcred_kint_PrintUCred*/


/*------------------------------------------------------------------------
 * EXPORT xcred_kint_PrintXCred
 *
 * Environment:
 *	Nothing interesting.
 *
 * Side Effects:
 *	May print to the debugging output.
 *------------------------------------------------------------------------*/

/* EXPORT */
void xcred_kint_PrintXCred(axcredP, alevel)
    IN xcred_t *axcredP;
    IN int alevel;

{ /*xcred_kint_PrintXCred*/

#if AFSDEBUG
    xcred_PListEntry_t *currplP;	/*Ptr to plist entry*/
    xcred_GCHandler_t *currgcP;		/*Ptr to GC entry*/
    int curr;				/*Iterator*/
    char buf[80];			/*Printing buffer*/
    int cpychars;			/*Num chars to copy*/

    printf("\nXcred at 0x%x\n", axcredP);
    printf("\trefcount %d, ucred at 0x%x\n",
	   axcredP->refcount, axcredP->assocUCredP);

    currplP = axcredP->propListP;
    curr = 0;
    printf("\tProperty list entries:\n");
    while (currplP) {
	cpychars = ((currplP->valueBytes <= 79) ? currplP->valueBytes : 79);
	strncpy(buf, currplP->valueP, cpychars);
	buf[cpychars] = 0;
	printf("\t\t[%d] flags %x, attribute[%d bytes]='%s', value[%d bytes]='%s'\n",
	       curr, currplP->flags, currplP->attrBytes, currplP->attributeP,
	       currplP->valueBytes, buf);
	if (currplP->deriv.baseP)
	    printf("\t\t[Derived from attr '%s']\n",
		   currplP->deriv.baseP->attributeP);
	currplP = currplP->nextP;
	curr++;
    } /*Print out property list entries*/

    currgcP = axcredP->gcHandlerP;
    curr = 0;
    printf("\tGC Handler entries:\n");
    while (currgcP) {
	printf("\t\t[%d] gcProcP 0x%x, rockP 0x%x\n",
	       curr, currgcP->gcProcP, currgcP->rockP);
	curr++;
	currgcP = currgcP->nextP;
    } /*Print out GC handler entries*/

    printf("\t[Printing derived entries is not yet implemented]\n");

#endif AFSDEBUG

} /*xcred_kint_PrintXCred*/
