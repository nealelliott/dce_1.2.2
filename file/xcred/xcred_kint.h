/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xcred_kint.h,v $
 * Revision 1.1.490.1  1996/10/02  19:03:17  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:51:45  damon]
 *
 * Revision 1.1.485.2  1994/06/09  14:25:46  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:37:43  annie]
 * 
 * Revision 1.1.485.1  1994/02/04  20:36:30  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:20:46  devsrc]
 * 
 * Revision 1.1.483.1  1993/12/07  17:38:33  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/06  13:45:46  jaffe]
 * 
 * 	1.0.3a update from Transarc
 * 
 * Revision 1.1.3.2  1993/01/21  16:34:09  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  18:27:59  cjd]
 * 
 * Revision 1.1  1992/01/19  02:45:53  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * Copyright (C) 1990 Transarc Corporation
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 *
 * $Header: /u0/rcs_trees/dce/rcs/file/xcred/xcred_kint.h,v 1.1.490.1 1996/10/02 19:03:17 damon Exp $
 *
 * xcred_kint.h
 *	Definitions/interface for manipulating the extended
 *	credential facility via the system call interface.
 */

#ifndef _TRANSARC_XCRED_KINT_H_
#define _TRANSARC_XCRED_KINT_H_ 1

#ifdef  TESTING_XCRED_SYSCAL

/*
 *----------------------- Visible definitions  ------------------------
 *
 * These are the definitions that our clients must see in order to use
 * the kernel interface to the extended credential module.
 */

/*
 * Xcred system call, plus all of its opcodes.
 *
 *	SET_DEBUG_LEVEL	Set the debugging level
 *	CREATE		Create an xcred structure
 *	HOLD		Bump refcount on xcred
 *	RELEASE		Decrement refcount/gc an xcred
 *	ASSOCIATE_CREDS	Associate an xcred and ucred.
 *	UCRED_TO_XCRED	Find xcred associated to given ucred.
 *	XCRED_TO_UCRED	Find ucred associated to given xcred.
 *	PUT_PROP	Put an attr/value pair in a cred
 *	GET_PROP	Get the associated value from an attr
 *	ADD_GC_PROC	Add garbage collection routine
 *	DELETE_GC_PROC	Delete garbage collection routine
 *	GET_GC_PROC	Get garbage collection routine
 *	ENUMERATE_PROP	Enumerate properties in credential
 *	TEST_SUITE	Execute an entire test suite
 */
#define XCRED_SYSCALL				52

#define XCRED_SYSCALL_SET_DEBUG_LEVEL		 0
#define XCRED_SYSCALL_CREATE			 1
#define XCRED_SYSCALL_HOLD			 2
#define XCRED_SYSCALL_RELEASE			 3
#define XCRED_SYSCALL_ASSOCIATE_CREDS		 4
#define XCRED_SYSCALL_UCRED_TO_XCRED		 5
#define XCRED_SYSCALL_XCRED_TO_UCRED		 6
#define XCRED_SYSCALL_PUT_PROP			 7
#define XCRED_SYSCALL_GET_PROP			 8
#define XCRED_SYSCALL_ADD_GC_PROC		 9
#define XCRED_SYSCALL_DELETE_GC_PROC		10
#define XCRED_SYSCALL_GET_GC_PROC		11
#define XCRED_SYSCALL_ENUMERATE_PROP		12
#define XCRED_SYSCALL_TEST_SUITE		13

/*
 * Argument structures
 *
 * We define a union for all the argument structures expected by all
 * the opcodes within the system call.
 */

typedef struct {
    char anewDBLevel;			/*Desired debugging level*/
} xcred_kint_SetDebugLevelArgs_t;

typedef struct {
    xcred_t **anewXCredPP;		/*Set to point to new xcred*/
} xcred_kint_CreateArgs_t;

typedef struct {
    xcred_t *axcredP;			/*Ptr to xcred to hold*/
} xcred_kint_HoldArgs_t;

typedef struct {
    xcred_t *axcredP;			/*Ptr to xcred to release*/
} xcred_kint_ReleaseArgs_t;

typedef struct {
    xcred_t *axcredP;			/*Ptr to xcred*/
    osi_cred_t *aucredP;		/*Ptr to ucred*/
} xcred_kint_AssociateCredsArgs_t;

typedef struct {
    osi_cred_t *aucredP;		/*Ptr to Unix cred*/
    xcred_t **axcredPP;			/*Set to addr of assoc xcred*/
} xcred_kint_UCredToXCredArgs_t;


typedef struct {
    xcred_t *axcredP;			/*Ptr to xcred*/
    osi_cred_t **aucredPP;		/*Set to addr of assoc Unix cred*/
} xcred_kint_XCredToUCredArgs_t;

typedef struct {
    xcred_t *axcredP;			/*Ptr to xcred*/
    char *aattributeP;			/*String attribute name*/
    char *avalueP;			/*Counted array of bytes for value*/
    long alength;			/*Value length in bytes*/
    long aflags;			/*Various flags*/
    char *abaseAttr;			/*Attribute name we're derived from*/
} xcred_kint_PutPropArgs_t;

typedef struct {
    xcred_t *axcredP;			/*Ptr to xcred*/
    char *aattributeP;			/*String attribute name*/
    long amaxLength;			/*Max bytes in value buffer*/
    char **avaluePP;			/*Loc of value buffer address*/
    long *realLengthP;			/*Num bytes put in *avaluePP*/
} xcred_kint_GetPropArgs_t;

typedef struct {
    xcred_t *axcredP;			/*Ptr to xcred*/
    int (*aprocP)();			/*Addr of new GC handler routine*/
    char *arockP;			/*Ptr to param for above*/
} xcred_kint_AddGCProcArgs_t;


typedef struct {
    xcred_t *axcredP;			/*Ptr to xcred*/
    int (*aprocP)();			/*Addr of doomed GC handler routine*/
    char *arockP;			/*Ptr to param for above*/
} xcred_kint_DeleteGCProcArgs_t;

typedef struct {
    xcred_t *axcredP;			/*Ptr to xcred*/
    int (*aprocP)();			/*Addr of GC handler routine to find*/
    char *arockP;			/*Ptr to param for above*/
} xcred_kint_GetGCProcArgs_t;

typedef struct {
    xcred_t *axcredP;			/*Ptr to xcred*/
    long (*aprocP)();			/*Ptr to fcn to call on each plist entry*/
    char *arockP;			/*First param passed to (*apropP)()*/
} xcred_kint_EnumeratePropArgs_t;


/*
 * Union of all the opcode argument structures.
 */
typedef union {
    xcred_kint_SetDebugLevelArgs_t	adbLevel;
    xcred_kint_CreateArgs_t		acreate;
    xcred_kint_HoldArgs_t		ahold;
    xcred_kint_ReleaseArgs_t		arel;
    xcred_kint_AssociateCredsArgs_t	aassoc;
    xcred_kint_UCredToXCredArgs_t	aUtoX;
    xcred_kint_XCredToUCredArgs_t	aXtoU;
    xcred_kint_PutPropArgs_t		aputProp;
    xcred_kint_GetPropArgs_t		agetProp;
    xcred_kint_AddGCProcArgs_t		aaddGC;
    xcred_kint_DeleteGCProcArgs_t	adelGC;
    xcred_kint_GetGCProcArgs_t		agetGC;
    xcred_kint_EnumeratePropArgs_t	aenumProp;
} xcred_kint_opargs_t;

/*
 *------------------------ Visible functions  ------------------------
 */

EXPORT int xcred_kint_syscall _TAKES(( void ));
    /*
     * Summary:
     *    The xcred system call.
     *
     * Args:
     *	  None via the function call.  However, the user's structure
     *	  has a u_ap field that holds the parameters we need, a part
     *	  of which is defined by xcred_kint_opargs_t.
     *
     * Returns:
     *	  0       if everything goes well,
     *	  EACCESS should the caller not be root,
     *	  EINVAL  should something else go wrong.
     *
     *	  *** NOTE *** The return code is also placed in the
     *		       user's error field.
     */

EXPORT void xcred_kint_PrintUCred _TAKES((
    IN osi_cred_t *aucredP,
    IN int alevel));

    /*
     * Summary:
     *    Given a pointer to a Unix cred, print out its contents
     *	  on the debugging output at the given severity level.
     *
     * Args:
     *	  IN osi_cred_t *aucredP : Ptr to Unix cred to print.
     *	  IN int alevel		   : Severity level to use.
     *
     * Returns:
     *	  Nothing.
     */

EXPORT void xcred_kint_PrintXCred _TAKES((
    IN xcred_t *axcredP,
    IN int alevel));

    /*
     * Summary:
     *    Given a pointer to an xcred, print out its contents
     *	  on the debugging output at the given severity level.
     *
     * Args:
     *	  IN xcred_t *axcredP : Ptr to xcred to print.
     *	  IN int alevel	      : Severity level to use.
     *
     * Returns:
     *	  Nothing.
     */
#endif  /* TESTING_XCRED_SYSCALL */

#endif /* _TRANSARC_XCRED_KINT_H_ */
