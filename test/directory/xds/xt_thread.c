/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: xt_thread.c,v $
 * Revision 1.1.6.2  1996/02/18  00:24:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:12:17  marty]
 *
 * Revision 1.1.6.1  1995/12/08  22:14:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:46:29  root]
 * 
 * Revision 1.1.4.4  1994/07/06  15:09:51  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:28:29  marrek]
 * 
 * Revision 1.1.4.3  1994/05/10  15:33:31  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/06  10:24:17  marrek]
 * 
 * Revision 1.1.4.2  1994/03/23  16:06:51  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  18:14:08  keutel]
 * 
 * Revision 1.1.4.1  1994/02/22  19:16:25  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/07  13:32:40  marrek]
 * 
 * Revision 1.1.2.2  1993/08/12  12:39:38  marrek
 * 	July 1993 code drop.
 * 	[1993/08/03  12:30:54  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: xt_thread.c,v $ $Revision: 1.1.6.2 $ $Date: 1996/02/18 00:24:58 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <xt.h>
#include <xt_thread_struct.h>
#include <xdsp.h>



extern const OM_descriptor *master_session_tbl[];
extern const OM_workspace  *master_workspace_tbl[];
extern const OM_private_object master_private_object_tbl[];
extern const OM_public_object master_public_object_tbl[];
extern const OM_string master_string_tbl[];


struct thread_struct {
    pthread_t handle;
    char    name[MAX_NAME_LEN];
    char    id_and_name[MAX_NAME_LEN];
    struct thread_data_struct data;
};



/* thread table has 0'th entry for "main thread" */
static struct thread_struct *thread_table = NULL;
static OM_sint  thread_table_size = 0;

static pthread_startroutine_t xt_thread_function(
	OM_sint (*function)(void));

/* local static data */
pthread_mutex_t     xt_cond_mutex;
pthread_cond_t      xt_cond_var;
OM_sint thread_hold = 1;


OM_sint total_passed = 0, total_failed = 0, total_interrupted = 0;

/* prototypes are here, since xt_thread.h is not included here,
but only in other tester files. */

void    xt_initialize_thread_data(void);
OM_sint xt_thread_name_set( const char *name, /* name to be given to thread */
	const OM_sint thread_number);  /* number of thread to be named */
static OM_sint xt_thread_id_and_name_set(
const char *name); /* name to be given to current thread */
void    xt_thread_init(void);    /*  initialize thread table
                                mutexes, condition variable */
#if USE_GUARDS

OM_sint is_clear(OM_sint *block);
void xt_crash(void);  /* routine called to allow breakpoint to be set in debugger */
/* while data can still be examined */
void
set_guards(void);
void check_guards(void);
void    print_guard(OM_sint guard[]);
#endif /* USE_GUARDS */

struct thread_data_struct *xt_this_thread_data(void); /* returns a pointer to data area for current thread */
static OM_sint  this_thread_index(void); /* return index in thread_table of current thread */
OM_sint xt_threads_created(void); /* number of threads created so far */
char *xt_this_thread_name(void); /* name of current thread */
char *xt_this_thread_id_and_name(void); /* id and name of current thread */
void    xt_thread_fail(void); /* log thread failure */
void    xt_thread_pass(void); /* log thread success */
void    xt_check_string_tbl(void);


static OM_sint  this_thread_index(void);


/*********************************************************************/
void    xt_initialize_thread_data(void)
/*********************************************************************/
{
    OM_sint i;
    OM_sint thread_number =  0;

    /* clear all to zeros initially */
    memset(&thread_table[thread_number].data, sizeof(thread_table[thread_number].data) *  sizeof (size_t), 0);

    /*  initialize workspace_tbl: */
    for (i = 0; i < XT_LEN_WORKSPACE_TBL; i++) {
        thread_table[thread_number].data.workspace_tbl[i] = (OM_workspace *)master_workspace_tbl[i];
		/* cast to suppress warning about loss of const qualifier */
    } /* end for */

    /* initialize session_tbl: */
    for (i = 0; i < XT_LEN_SESSION_TBL; i++) {
        thread_table[thread_number].data.session_tbl[i] = (OM_descriptor *)master_session_tbl[i];
		/* cast to suppress warning about loss of const qualifier */
    } /* end for */

    /*  initialize public_object_tbl: */
    for (i = 0; i < XT_LEN_PUBLIC_OBJECT_TBL; i++) {
        thread_table[thread_number].data.public_object_tbl[i] =  master_public_object_tbl[i];
    } /* end for */

    /*  initialize private_object_tbl: */
    for (i = 0; i < XT_LEN_PRIVATE_OBJECT_TBL ; i++) {
        thread_table[thread_number].data.private_object_tbl[i] =  master_private_object_tbl[i];
    } /* end for */

    /*  initialize string_tbl: */
    for (i = 0; i < XT_LEN_STRING_TBL ; i++) {
        thread_table[thread_number].data.string_tbl[i] = master_string_tbl[i];
    } /* end for */


#if USE_GUARDS
    check_guards();
#endif /* USE_GUARDS */

} /* end xt_initialize_thread_data */





/*********************************************************************/
OM_sint xt_thread_name_set(
/*********************************************************************/
const char *name,          /* name to be given to thread */
const OM_sint thread_number)  /* number of thread to be named */
{
    char    *cptr;

    /* remove path if any */
    if ( (cptr = strrchr(name, '/')) != NULL) {
        name = cptr  + 1;
    } /* endif */

    if (strlen(name) < MAX_NAME_LEN) {
        strcpy(thread_table[thread_number].name, name);
    } else {
        printf("thread name length was %i\n", strlen(name));
        printf("thread name itself was \"%s\"\n", name);
        xt_err("xt_thread_name_set", "name too long");
    } /* endif */

} /* end xt_thread_name_set */



/*********************************************************************/
static OM_sint xt_thread_id_and_name_set(
/*********************************************************************/
/* only works when called from thread */
const char *name) /* name to be given to current thread */
{
    char    name_buffer[256];

    sprintf(name_buffer, "[%i]%s", dsP_thread_id(),  xt_this_thread_name());
    if (strlen(name) < MAX_NAME_LEN) {
        strcpy(thread_table[this_thread_index()].id_and_name, name_buffer);
    } else {
        printf("thread id and name was %s\n", name_buffer);
        xt_err("xt_thread_id_and_name_set",  "name too long");
    } /* endif */

} /* end xt_thread_id_and_name_set */


/* must be called before anything tries to use thread-specific
data, even in main thread */
/*********************************************************************/
void    xt_thread_init()    /*  initialize thread table
                                mutexes, condition variable */
/*********************************************************************/
{
    OM_sint status;
    thread_table = malloc(sizeof(struct thread_struct ));
    if (xt_verbose)  {
        printf("after alloc, thread_table was %p\n", thread_table);
    } /* endif */
    if (thread_table == NULL) {
        xt_err("xt_thread_init", "memory allocation FAILED for threads data");
        exit(XT_ERROR);
    } /* endif */

    /* initialize first handle to that of main thread */
    thread_table[0].handle = pthread_self();
    xt_initialize_thread_data();
    thread_table_size++;

    /* to check for writing outside data areas */
#if USE_GUARDS
    set_guards();
#endif /* USE_GUARDS */

    /* create mutexes */
    status = pthread_mutex_init(&xt_cond_mutex,  pthread_mutexattr_default);
    if (status != 0) {
        printf(" bad status from mutex init");
        exit(status);
    } /* endif */

    /* create condition variable */
    status = pthread_cond_init(&xt_cond_var, pthread_condattr_default);
    if (status != 0) {
        printf(" in xt_thread_init,bad status from cond init\n");
        exit(status);
    } /* endif */

    /* to check for writing outside data areas */
#if USE_GUARDS
    check_guards();
#endif /* USE_GUARDS */

} /* end xt_thread_init */


/****************************************************/
void	xt_thread_deinit(void)
/****************************************************/
{
	free(thread_table);
} /* end xt_thread_deinit */



#if USE_GUARDS
#define GUARD_VALUE 0xCC

/*********************************************************************/
OM_sint is_clear(OM_sint *block)
/*********************************************************************/
{
    OM_sint i;
    for (i = 0; i < GUARD_SIZE; i++) {
        if (block[i] != GUARD_VALUE) {
            return (0);
        } /* endif */
    } /* end for */
    return (1);
} /* end is_clear */


/*********************************************************************/
void xt_crash(void)  /* routine called to allow breakpoint to be set in debugger */
/* while data can still be examined */
/*********************************************************************/
{
    OM_sint i =  1, j = 1;
    fflush(NULL);
    i =  i /     i - j; /* force crash */
} /* end xt_crash */


/*********************************************************************/
void set_guards(void)
/*********************************************************************/
{
    /* for memory corruption check, set guards to a particular value */
    OM_sint thread_number;
    OM_sint i;

    for (thread_number = 0; thread_number < thread_table_size; thread_number++)  {
        for (i = 0; i < GUARD_SIZE; i++) {
            thread_table[0].data.guard0[i] = GUARD_VALUE;
        } /* end for */
        for (i = 0; i < GUARD_SIZE; i++) {
            thread_table[0].data.guard1[i] = GUARD_VALUE;
        } /* end for */
        for (i = 0; i < GUARD_SIZE; i++) {
            thread_table[0].data.guard2[i] = GUARD_VALUE;
        } /* end for */
        for (i = 0; i < GUARD_SIZE; i++) {
            thread_table[0].data.guard3[i] = GUARD_VALUE;
        } /* end for */
        for (i = 0; i < GUARD_SIZE; i++) {
            thread_table[0].data.guard4[i] = GUARD_VALUE;
        } /* end for */
        for (i = 0; i < GUARD_SIZE; i++) {
            thread_table[0].data.guard5[i] = GUARD_VALUE;
        } /* end for */
        for (i = 0; i < GUARD_SIZE; i++) {
            thread_table[0].data.guard6[i] = GUARD_VALUE;
        } /* end for */
        for (i = 0; i < GUARD_SIZE; i++) {
            thread_table[0].data.guard7[i] = GUARD_VALUE;
        } /* end for */
        for (i = 0; i < GUARD_SIZE; i++) {
            thread_table[0].data.guard8[i] = GUARD_VALUE;
        } /* end for */
        for (i = 0; i < GUARD_SIZE; i++) {
            thread_table[0].data.guard9[i] = GUARD_VALUE;
        } /* end for */
        for (i = 0; i < GUARD_SIZE; i++) {
            thread_table[0].data.guard10[i] = GUARD_VALUE;
        } /* end for */
        for (i = 0; i < GUARD_SIZE; i++) {
            thread_table[0].data.guard11[i] = GUARD_VALUE;
        } /* end for */
        for (i = 0; i < GUARD_SIZE; i++) {
            thread_table[0].data.guard12[i] = GUARD_VALUE;
        } /* end for */
        for (i = 0; i < GUARD_SIZE; i++) {
            thread_table[0].data.guard13[i] = GUARD_VALUE;
        } /* end for */
        for (i = 0; i < GUARD_SIZE; i++) {
            thread_table[0].data.guard14[i] = GUARD_VALUE;
        } /* end for */
        for (i = 0; i < GUARD_SIZE; i++) {
            thread_table[0].data.guard15[i] = GUARD_VALUE;
        } /* end for */
        for (i = 0; i < GUARD_SIZE; i++) {
            thread_table[0].data.guard16[i] = GUARD_VALUE;
        } /* end for */
        for (i = 0; i < GUARD_SIZE; i++) {
            thread_table[0].data.guard17[i] = GUARD_VALUE;
        } /* end for */
        for (i = 0; i < GUARD_SIZE; i++) {
            thread_table[0].data.guard18[i] = GUARD_VALUE;
        } /* end for */
        for (i = 0; i < GUARD_SIZE; i++) {
            thread_table[0].data.guard19[i] = GUARD_VALUE;
        } /* end for */
        for (i = 0; i < GUARD_SIZE; i++) {
            thread_table[0].data.guard20[i] = GUARD_VALUE;
        } /* end for */
        for (i = 0; i < GUARD_SIZE; i++) {
            thread_table[0].data.guard21[i] = GUARD_VALUE;
        } /* end for */
        for (i = 0; i < GUARD_SIZE; i++) {
            thread_table[0].data.guard22[i] = GUARD_VALUE;
        } /* end for */
        for (i = 0; i < GUARD_SIZE; i++) {
            thread_table[0].data.guard23[i] = GUARD_VALUE;
        } /* end for */
        for (i = 0; i < GUARD_SIZE; i++) {
            thread_table[0].data.guard24[i] = GUARD_VALUE;
        } /* end for */
        /* must check all guards defined in xt_thread_struct.h */
    } /* end for */
} /* end set_guards */




/*********************************************************************/
void check_guards(void)
/*********************************************************************/
{
    /* temporary check for memory corruption */
    OM_sint thread_number;
    BOOLEAN is_corrupt = FALSE;


    FILE * fp;
    fp = (FILE * )thread_table[this_thread_index()].data.xt_testcase_file;

    if (this_thread_index() != 0 &&    (fp == NULL     || fp == (FILE *  ) -  1)) {
        printf("xt_testcase_file had bad value %xl\n", fp);
        if (this_thread_index() != 0) {
            is_corrupt = TRUE;
        } /* endif */
    } /* endif */

    for (thread_number = 0; thread_number < thread_table_size; thread_number++)  {
        if (!is_clear(thread_table[thread_number].data.guard0))  {
            printf("thread number:%i, guard0\n", thread_number);
            xt_err("check_guards", "guard0 corrupted");
            print_guard(thread_table[thread_number].data.guard0);
            is_corrupt = TRUE;
            exit(XT_ERROR);
        } /* endif */

        if (!is_clear(thread_table[thread_number].data.guard1))  {
            printf("thread number:%i, guard1\n", thread_number);
            xt_err("check_guards", "guard1 corrupted");
            print_guard(thread_table[thread_number].data.guard1);
            is_corrupt = TRUE;
            exit(XT_ERROR);
        } /* endif */

        if (!is_clear(thread_table[thread_number].data.guard2))  {
            printf("thread number:%i, guard2\n", thread_number);
            xt_err("check_guards", "guard2 corrupted");
            print_guard(thread_table[thread_number].data.guard2);
            is_corrupt = TRUE;
            exit(XT_ERROR);
        } /* endif */

        if (!is_clear(thread_table[thread_number].data.guard3))  {
            printf("thread number:%i, guard3\n", thread_number);
            xt_err("check_guards", "guard3 corrupted");
            print_guard(thread_table[thread_number].data.guard3);
            is_corrupt = TRUE;
            exit(XT_ERROR);
        } /* endif */

        if (!is_clear(thread_table[thread_number].data.guard4))  {
            printf("thread number:%i, guard4\n", thread_number);
            xt_err("check_guards", "guard4 corrupted");
            print_guard(thread_table[thread_number].data.guard4);
            is_corrupt = TRUE;
            exit(XT_ERROR);
        } /* endif */

        if (!is_clear(thread_table[thread_number].data.guard5))  {
            printf("thread number:%i, guard5\n", thread_number);
            xt_err("check_guards", "guard5 corrupted");
            print_guard(thread_table[thread_number].data.guard5);
            is_corrupt = TRUE;
            exit(XT_ERROR);
        } /* endif */

        if (!is_clear(thread_table[thread_number].data.guard6))  {
            printf("thread number:%i, guard6\n", thread_number);
            xt_err("check_guards", "guard6 corrupted");
            print_guard(thread_table[thread_number].data.guard6);
            is_corrupt = TRUE;
            exit(XT_ERROR);
        } /* endif */

        if (!is_clear(thread_table[thread_number].data.guard7))  {
            printf("thread number:%i, guard7\n", thread_number);
            xt_err("check_guards", "guard7 corrupted");
            print_guard(thread_table[thread_number].data.guard7);
            is_corrupt = TRUE;
            exit(XT_ERROR);
        } /* endif */

        if (!is_clear(thread_table[thread_number].data.guard8))  {
            printf("thread number:%i, guard8\n", thread_number);
            xt_err("check_guards", "guard8 corrupted");
            print_guard(thread_table[thread_number].data.guard8);
            is_corrupt = TRUE;
            exit(XT_ERROR);
        } /* endif */

        if (!is_clear(thread_table[thread_number].data.guard9))  {
            printf("thread number:%i, guard9\n", thread_number);
            xt_err("check_guards", "guard9 corrupted");
            print_guard(thread_table[thread_number].data.guard9);
            is_corrupt = TRUE;
            exit(XT_ERROR);
        } /* endif */

        if (!is_clear(thread_table[thread_number].data.guard10)) {
            printf("thread number:%i, guard10\n", thread_number);
            xt_err("check_guards", "guard10 corrupted");
            print_guard(thread_table[thread_number].data.guard10);
            is_corrupt = TRUE;
            exit(XT_ERROR);
        } /* endif */

        if (!is_clear(thread_table[thread_number].data.guard11)) {
            printf("thread number:%i, guard11\n", thread_number);
            xt_err("check_guards", "guard12 corrupted");
            print_guard(thread_table[thread_number].data.guard11);
            is_corrupt = TRUE;
            exit(XT_ERROR);
        } /* endif */

        if (!is_clear(thread_table[thread_number].data.guard12)) {
            printf("thread number:%i, guard12\n", thread_number);
            xt_err("check_guards", "guard12 corrupted");
            print_guard(thread_table[thread_number].data.guard12);
            is_corrupt = TRUE;
            exit(XT_ERROR);
        } /* endif */

        if (!is_clear(thread_table[thread_number].data.guard12)) {
            printf("thread number:%i, guard13\n", thread_number);
            xt_err("check_guards", "guard13 corrupted");
            print_guard(thread_table[thread_number].data.guard13);
            is_corrupt = TRUE;
            exit(XT_ERROR);
        } /* endif */

        if (!is_clear(thread_table[thread_number].data.guard14)) {
            printf("thread number:%i, guard14\n", thread_number);
            xt_err("check_guards", "guard14 corrupted");
            print_guard(thread_table[thread_number].data.guard14);
            is_corrupt = TRUE;
            exit(XT_ERROR);
        } /* endif */

        if (!is_clear(thread_table[thread_number].data.guard15)) {
            printf("thread number:%i, guard15\n", thread_number);
            xt_err("check_guards", "guard15 corrupted");
            print_guard(thread_table[thread_number].data.guard15);
            is_corrupt = TRUE;
            exit(XT_ERROR);
        } /* endif */

        if (!is_clear(thread_table[thread_number].data.guard16)) {
            printf("thread number:%i, guard16\n", thread_number);
            xt_err("check_guards", "guard16 corrupted");
            print_guard(thread_table[thread_number].data.guard16);
            is_corrupt = TRUE;
            exit(XT_ERROR);
        } /* endif */

        if (!is_clear(thread_table[thread_number].data.guard17)) {
            printf("thread number:%i, guard17\n", thread_number);
            xt_err("check_guards", "guard17 corrupted");
            print_guard(thread_table[thread_number].data.guard17);
            is_corrupt = TRUE;
            exit(XT_ERROR);
        } /* endif */

        if (!is_clear(thread_table[thread_number].data.guard18)) {
            printf("thread number:%i, guard18\n", thread_number);
            xt_err("check_guards", "guard18 corrupted");
            print_guard(thread_table[thread_number].data.guard18);
            is_corrupt = TRUE;
            exit(XT_ERROR);
        } /* endif */

        if (!is_clear(thread_table[thread_number].data.guard19)) {
            printf("thread number:%i, guard19\n", thread_number);
            xt_err("check_guards", "guard19 corrupted");
            print_guard(thread_table[thread_number].data.guard19);
            is_corrupt = TRUE;
            exit(XT_ERROR);
        } /* endif */

        if (!is_clear(thread_table[thread_number].data.guard20)) {
            printf("thread number:%i, guard20\n", thread_number);
            xt_err("check_guards", "guard20 corrupted");
            print_guard(thread_table[thread_number].data.guard20);
            is_corrupt = TRUE;
            exit(XT_ERROR);
        } /* endif */

        if (!is_clear(thread_table[thread_number].data.guard21)) {
            printf("thread number:%i, guard21\n", thread_number);
            xt_err("check_guards", "guard21 corrupted");
            print_guard(thread_table[thread_number].data.guard21);
            is_corrupt = TRUE;
            exit(XT_ERROR);
        } /* endif */

        if (!is_clear(thread_table[thread_number].data.guard22)) {
            printf("thread number:%i, guard22\n", thread_number);
            xt_err("check_guards", "guard22 corrupted");
            print_guard(thread_table[thread_number].data.guard22);
            is_corrupt = TRUE;
            exit(XT_ERROR);
        } /* endif */

        if (!is_clear(thread_table[thread_number].data.guard23)) {
            printf("thread number:%i, guard23\n", thread_number);
            xt_err("check_guards", "guard23 corrupted");
            print_guard(thread_table[thread_number].data.guard23);
            is_corrupt = TRUE;
            exit(XT_ERROR);
        } /* endif */

        if (!is_clear(thread_table[thread_number].data.guard24)) {
            printf("thread number:%i, guard24\n", thread_number);
            xt_err("check_guards", "guard24 corrupted");
            print_guard(thread_table[thread_number].data.guard24);
            is_corrupt = TRUE;
            exit(XT_ERROR);
        } /* endif */

        /* must check all guards as defined in xt_thread_struct.h */
    } /* end for */

    if (is_corrupt)  {
        xt_crash();
    } /* endif */


} /* end check_guards */


/*********************************************************************/
void    print_guard(OM_sint guard[])
/*********************************************************************/
{
    OM_sint i;
    for (i = 0; i < GUARD_SIZE; i++) {
        printf(" %x", guard[i]);
    } /* end for */
} /* end print_guard */


#endif /* USE_GUARDS */

/* for debugging only */
/*********************************************************************/
static void    xt_do_break(const char *message)
/*********************************************************************/
{
    if (xt_verbose)  {
        printf ("%s: %s\n",  xt_this_thread_id_and_name(), message);
#if USE_GUARDS
        check_guards();
#endif  /* USE_GUARDS */
    } /* endif */
} /* end xt_do_break */



/*********************************************************************/
struct thread_data_struct *xt_this_thread_data(void)
/*********************************************************************/
/* returns a pointer to data area for current thread */
{
    pthread_t self = pthread_self();
    BOOLEAN thread_found =  FALSE;
    OM_sint thread_number;
    static OM_sint  last_thread_number = 0;

#if 0
    check_guards();
#endif

    if (!memcmp(&thread_table[last_thread_number].handle, &self, sizeof(pthread_t))) {
        /* for thread-safeness integer assign must be atomic */
        thread_number =  last_thread_number;
        thread_found = TRUE;
    } else {


        for (thread_number = 0; thread_number < thread_table_size; thread_number++) {
            if (!memcmp(&thread_table[thread_number].handle, &self,  sizeof(pthread_t)))  {
                thread_found = TRUE;
                break;
            } /* endif */
        } /* end for */
        last_thread_number = thread_number;
    } /* endif */

    if (thread_found) {
        return (&thread_table[thread_number].data);
    } else {
        xt_err("this_thread_data", "thread data not found");
        exit(XT_ERROR);
    } /* endif */

} /* end xt_this_thread_data */


/* since it is more important to make xt_this_thread_data fast,
the following functions are implemented in a rather convoluted
way: */


/*********************************************************************/
static OM_sint  this_thread_index(void)
/*********************************************************************/
/* return index in thread_table of current thread */
{
    pthread_t self = pthread_self();
    BOOLEAN thread_found =  FALSE;
    OM_sint thread_number;
    static OM_sint  last_thread_number = 0;

    if (!memcmp(&thread_table[last_thread_number].handle, &self, sizeof(pthread_t))) {
        thread_number =  last_thread_number;
        thread_found = TRUE;
    } else {


        for (thread_number = 0; thread_number < thread_table_size; thread_number++) {
            if (!memcmp(&thread_table[thread_number].handle, &self,  sizeof(pthread_t)))  {
                thread_found = TRUE;
                break;
            } /* endif */
        } /* end for */
        last_thread_number = thread_number;
    } /* endif */

    if (thread_found) {
        return (thread_number);
    } else {
        xt_err("this_thread_index",  "thread index not found");
        exit(XT_ERROR);
    } /* endif */
} /* end this_thread_index */


/*********************************************************************/
OM_sint xt_threads_created(void)
/*********************************************************************/
/* number of threads created so far */
{
    return thread_table_size;
} /* end xt_threads_created */


/*********************************************************************/
char *xt_this_thread_name(void)
/*********************************************************************/
/* name of current thread */
{
    return (thread_table[this_thread_index()].name);
} /* end xt_this_thread_name */


/*********************************************************************/
char *xt_this_thread_id_and_name(void)
/* id and name of current thread */
/*********************************************************************/
{
    return (thread_table[this_thread_index()].id_and_name);
} /* end xt_this_thread_id_and_name */



/*********************************************************************/
void    xt_thread_fail(void)
/*********************************************************************/
/* log thread failure */
{
    xt_log_fail (xt_this_thread_id_and_name());
    printf("\n"); /* separate printf to get thread_id in line */
    printf ("[%i]Testcase %s failed\n",  dsP_thread_id(), xt_this_thread_id_and_name());
} /* end xt_thread_fail */


/*********************************************************************/
void    xt_thread_pass(void)
/*********************************************************************/
/* log thread success */
{
    xt_log_pass (xt_this_thread_id_and_name());
    printf("\n"); /* separate printf to get thread_id in line */
    printf ("[%i]Testcase %s passed\n",  dsP_thread_id(), xt_this_thread_name());
} /* end xt_thread_pass */


/*********************************************************************/
static pthread_startroutine_t xt_thread_function(
/*********************************************************************/
	OM_sint (*function)(void)) /* "real" function to run in thread. */
	/*This function is just an
    administrative overhead */
/* function to set up thread id and name, and wait on condition
    variable before calling function parameter and dealing
    with return value */
{

    OM_sint status;
    extern int  dsP_thread_id(void);
    TRY  if (xt_verbose) {
        printf("in xt_thread_function, at start of [%d]\'%s\'\n", dsP_thread_id(), xt_this_thread_name());
    } /* endif */

    xt_thread_id_and_name_set(xt_this_thread_name());

	/* set cancellability on for -x abandon tests */
	pthread_setcancel(CANCEL_ON);

    status = pthread_mutex_lock(&xt_cond_mutex);
    if (status != 0) {
        printf("in xt_thread_function \'%s\', mutex lock bad status\n", xt_this_thread_id_and_name());
        exit(status);
    } /* endif */

    while (thread_hold)  {
        status = pthread_cond_wait(&xt_cond_var, &xt_cond_mutex);
        if (status != 0) {
            printf("in xt_thread_function \'%s\', cond wait bad status\n", xt_this_thread_id_and_name());
            exit(status);
        } /* endif */
    } /* end while */

    if (xt_verbose)  {
        printf("in xt_thread_function, after wait in \'%s\'\n", xt_this_thread_id_and_name());
    } /* endif */

    status = pthread_mutex_unlock (&xt_cond_mutex);
    if (status != 0) {
        printf("in xt_thread_function \'%s\', mutex unlock bad status\n", xt_this_thread_id_and_name());
        exit(status);
    } /* endif */


    if (xt_verbose)  {
        printf("in xt_thread_function, after mutex unlock in \'%s\'\n", xt_this_thread_id_and_name());
    } /* endif */

    /* run testcase */
    status = function();


    if (xt_verbose)  {
        printf("in xt_thread_function, after function run in \'%s\'\n", xt_this_thread_id_and_name());
    } /* endif */

    if (status == XT_OK) {
        xt_thread_pass();
    } else {
        xt_thread_fail();
    } /* endif */


    if (xt_closet() != XT_OK) {
        xt_errx("xt_main", "error closing testcase file");
    } /* endif */

    return ((pthread_startroutine_t)status);

    CATCH (exc_uninitexc_e)
    printf("in xt_thread_function, thread \'%s\', exception \'exc_uninitexc_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (exc_illaddr_e)
    printf("in xt_thread_function, thread \'%s\',exception \'exc_illaddr_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (exc_exquota_e)
    printf("in xt_thread_function, thread \'%s\',exception \'exc_exquota_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (exc_insfmem_e)
    printf("in xt_thread_function, thread \'%s\',exception \'exc_insfmem_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (exc_nopriv_e)
    printf("in xt_thread_function, thread \'%s\',exception \'exc_nopriv_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (exc_illinstr_e)
    printf("in xt_thread_function, thread \'%s\',exception \'exc_illinstr_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (exc_resaddr_e)
    printf("in xt_thread_function, thread \'%s\',exception \'exc_resaddr_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (exc_privinst_e)
    printf("in xt_thread_function, thread \'%s\',exception \'exc_privinst_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (exc_resoper_e)
    printf("in xt_thread_function, thread \'%s\',exception \'exc_resoper_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (exc_SIGTRAP_e)
    printf("in xt_thread_function, thread \'%s\',exception \'exc_SIGTRAP_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (exc_SIGIOT_e)
    printf("in xt_thread_function, thread \'%s\',exception \'exc_SIGIOT_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (exc_SIGEMT_e)
    printf("in xt_thread_function, thread \'%s\',exception \'exc_SIGEMT_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (exc_aritherr_e)
    printf("in xt_thread_function, thread \'%s\',exception \'exc_aritherr_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (exc_SIGSYS_e)
    printf("in xt_thread_function, thread \'%s\',exception \'exc_SIGSYS_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (exc_SIGPIPE_e)
    printf("in xt_thread_function, thread \'%s\',exception \'exc_SIGPIPE_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (exc_excpu_e)
    printf("in xt_thread_function, thread \'%s\',exception \'exc_excpu_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (exc_exfilsiz_e)
    printf("in xt_thread_function, thread \'%s\',exception \'exc_exfilsiz_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (exc_intovf_e)
    printf("in xt_thread_function, thread \'%s\',exception \'exc_intovf_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (exc_intdiv_e)
    printf("in xt_thread_function, thread \'%s\',exception \'exc_intdiv_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (exc_fltovf_e)
    printf("in xt_thread_function, thread \'%s\',exception \'exc_fltovf_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (exc_fltdiv_e)
    printf("in xt_thread_function, thread \'%s\',exception \'exc_fltdiv_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (exc_fltund_e)
    printf("in xt_thread_function, thread \'%s\',exception \'exc_fltund_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (exc_decovf_e)
    printf("in xt_thread_function, thread \'%s\',exception \'exc_decovf_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (exc_subrng_e)
    printf("in xt_thread_function, thread \'%s\',exception \'exc_subrng_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (pthread_cancel_e)
    printf("in xt_thread_function, thread \'%s\',exception \'pthread_cancel_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (pthread_assertion_e)
    printf("in xt_thread_function, thread \'%s\',exception \'pthread_assertion_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (pthread_badparam_e)
    printf("in xt_thread_function, thread \'%s\',exception \'pthread_badparam_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (pthread_bugcheck_e)
    printf("in xt_thread_function, thread \'%s\',exception \'pthread_bugcheck_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (pthread_exit_thread_e)
    printf("in xt_thread_function, thread \'%s\',exception \'pthread_exit_thread_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (pthread_existence_e)
    printf("in xt_thread_function, thread \'%s\',exception \'pthread_existence_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (pthread_in_use_e)
    printf("in xt_thread_function, thread \'%s\',exception \'pthread_in_use_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (pthread_use_error_e)
    printf("in xt_thread_function, thread \'%s\',exception \'pthread_use_error_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (pthread_wrongmutex_e)
    printf("in xt_thread_function, thread \'%s\',exception \'pthread_wrongmutex_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (pthread_stackovf_e)
    printf("in xt_thread_function, thread \'%s\',exception \'pthread_stackovf_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (pthread_nostackmem_e)
    printf("in xt_thread_function, thread \'%s\',exception \'pthread_nostackmem_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (pthread_notstack_e)
    printf("in xt_thread_function, thread \'%s\',exception \'pthread_notstack_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (pthread_unimp_e)
    printf("in xt_thread_function, thread \'%s\',exception \'pthread_unimp_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (pthread_inialrpro_e)
    printf("in xt_thread_function, thread \'%s\',exception \'pthread_inialrpro_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (pthread_defer_q_full_e)
    printf("in xt_thread_function, thread \'%s\',exception \'pthread_defer_q_full_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (pthread_signal_q_full_e)
    printf("in xt_thread_function, thread \'%s\',exception \'pthread_signal_q_full_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (exc_accvio_e)
    printf("in xt_thread_function, thread \'%s\',exception \'exc_accvio_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (exc_SIGILL_e)
    printf("in xt_thread_function, thread \'%s\',exception \'exc_SIGILL_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (exc_SIGFPE_e)
    printf("in xt_thread_function, thread \'%s\',exception \'exc_SIGFPE_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (exc_SIGBUS_e)
    printf("in xt_thread_function, thread \'%s\',exception \'exc_SIGBUS_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (exc_SIGSEGV_e)
    printf("in xt_thread_function, thread \'%s\',exception \'exc_SIGSEGV_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (exc_SIGXCPU_e)
    printf("in xt_thread_function, thread \'%s\',exception \'exc_SIGXCPU_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    CATCH (exc_SIGXFSZ_e)
    printf(" in xt_thread_function, thread \'%s\',exception \'exc_SIGXFSZ_e\' raised\n", xt_this_thread_id_and_name());
    RERAISE;
    ENDTRY 
} /* end xt_thread_function */



/*********************************************************************/
OM_sint xt_thread_create(
/*********************************************************************/
				const OM_sint thread_number,
				OM_sint (*function)(void),  /* func to be executed in thread */
				const char name[])
{
    OM_sint status,  retc;
    pthread_attr_t  thread_attributes;


    if (thread_number >= MAX_THREADS) {
        xt_errx("xt_thread_create",  "attempt to create more than " "MAX_THREADS threads\n");
    } /* endif */

    /* increase allocation for thread table */
    thread_table = (struct thread_struct *) realloc(thread_table, sizeof(struct thread_struct ) *(thread_table_size + 1));

    if (thread_table ==  NULL) {
        xt_errx("xt_thread_create",  "realloc FAILED for threads data");
    } /* endif */

    /* copy "global" (main thread) data into thread_specific area */
    thread_table[thread_number].data = thread_table[0].data;
#if USE_GUARDS
    set_guards();
#endif /* USE_GUARDS */

    xt_thread_name_set(name, thread_number);

    /* create thread attributes object */
    status = pthread_attr_create(&thread_attributes);
    if (status != 0) {
        printf("in xt_thread_create, pthread_attr_create bad status\n");
        exit(status);
    } /* endif */

    /* set timeslicing on */
    status = pthread_attr_setsched(&thread_attributes, SCHED_OTHER);
    if (status != 0) {
        printf("in xt_thread_create, pthread_attr_setsched bad status\n");
        exit(status);
    } else {
        retc = XT_OK;
    } /* endif */

    status = pthread_create( &thread_table[thread_number].handle,
							thread_attributes,
							(pthread_startroutine_t)xt_thread_function,
							(pthread_addr_t)function);

    if (status == 0) {
        thread_table_size++;
        retc = XT_OK;
    } else {
        retc = XT_ERROR;
    } /* endif */

    return retc;

} /* end xt_thread_create; */


/*********************************************************************/
void    xt_threads_start (void)
/*********************************************************************/
/* release threads from wait on condition variable */
{
    OM_sint status,  thread_number;
    pthread_addr_t last_status;
    /* lock mutex */
    status = pthread_mutex_lock (&xt_cond_mutex);
    if (status != 0) {
        printf("in xt_threads_start, mutex lock bad status\n");
        exit(status);
    } /* endif */


    /* unleash threads */
    thread_hold = 0;
    status = pthread_cond_broadcast (&xt_cond_var);
    if (status != 0) {
        printf("in xt_threads_start, cond broadcast bad status\n");
        exit(status);
    } /* endif */

    status = pthread_mutex_unlock (&xt_cond_mutex);
    if (status != 0) {
        printf("in xt_threads_start,  mutex unlock bad status\n");
        exit(status);
    } /* endif */

} /* end xt_threads_start */


/*********************************************************************/
void    xt_threads_join (void)
/*********************************************************************/
/* wait till threads terminate then log results */
{
    OM_sint status,  thread_number;
    pthread_addr_t last_status;


    /* wait  for threads to terminate */
    for (thread_number = 1; thread_number < thread_table_size; thread_number++)  {
        if (xt_verbose)  {
            printf (" in xt_threads_join, waiting to join \"%s\", thread number %d\n", thread_table[thread_number].id_and_name,thread_number);
        } /* endif */
        status = pthread_join(thread_table[thread_number].handle, &last_status);
		if (status == 0) {
			if (xt_verbose)  {
				printf ("joined thread \'%s\' with return status %d\n", thread_table[thread_number].id_and_name, last_status);
			} /* endif */
			if ((OM_sint)last_status ==  XT_OK) {
				++total_passed;
			} else {
				++total_failed;
			} /* endif */
		} else {
            printf(" in xt_threads_join, thread number was %d\n",thread_number);
            printf(" in xt_threads_join, thread %s bad status:%d\n",thread_table[thread_number].id_and_name,status);
			if (status == -1 ) {
				++total_interrupted;
			} /* endif */
		} /* endif */
    } /* end for */
	/* reset thread_table back to main thread */
	thread_table_size = 1;
} /* end xt_threads_join */


void
/***********************************************************/
			xt_reset_thread_results(void)
/***********************************************************/
{
	total_passed = total_failed = total_interrupted = 0;
} /* end xt_reset_thread_results */

void
/***********************************************************/
			xt_print_thread_results(void)
/***********************************************************/
{
    if (xt_verbose)  {
        printf("\n***********************************************************\n");
        printf("threads passed: %d, failed: %d, interrupted: %d\n", total_passed, total_failed, total_interrupted);
        printf("***********************************************************\n");
        printf("N.B. not including startup and shutdown\n");
    } /* endif */

    fflush(NULL);
} /* end xt_print_thread_results */

/*********************************************************************/
void    xt_threads_cancel (void)
/*********************************************************************/
/* cancel all created threads */
{
    OM_sint status,  thread_number;

    for (thread_number = 1; thread_number < thread_table_size; thread_number++)  {
        if (xt_verbose)  {
            printf (" in xt_threads_cancel, calling pthread_cancel for ""%s""\n", thread_table[thread_number].id_and_name);
        } /* endif */
        status = pthread_cancel(thread_table[thread_number].handle);

        if (status != 0) {
            printf(" in xt_threads_cancel, thread cancel bad status\n");
            exit(status);
        } /* endif */

    } /* end for */
    if (xt_verbose)  {
        printf("returning from xt_threads_cancel\n");
    } /* endif */

    fflush(NULL);
} /* end xt_threads_cancel */


/*********************************************************************/
static struct thread_data_struct *xt_this_thread_data_for_string_check(void)
/*********************************************************************/
/* returns a pointer to data area for current thread */
/* temporary version for string_tbl check to avoid recursion */
{
    pthread_t self = pthread_self();
    BOOLEAN thread_found =  FALSE;
    OM_sint thread_number;
    static OM_sint  last_thread_number = 0;

#if 0
    check_guards();
#endif

    if (!memcmp(&thread_table[last_thread_number].handle, &self, sizeof(pthread_t))) {
        /* for thread-safeness integer assign must be atomic */
        thread_number =  last_thread_number;
        thread_found = TRUE;
    } else {


        for (thread_number = 0; thread_number < thread_table_size; thread_number++) {
            if (!memcmp(&thread_table[thread_number].handle, &self,  sizeof(pthread_t)))  {
                thread_found = TRUE;
                break;
            } /* endif */
        } /* end for */
        last_thread_number = thread_number;
    } /* endif */

    if (thread_found) {
        return (&thread_table[thread_number].data);
    } else {
        xt_err("this_thread_data_for_string_check",  "thread data not found");
        exit(XT_ERROR);
    } /* endif */

} /* end xt_this_thread_data_for_string_check */


/*********************************************************************/
void    xt_check_string_tbl(void)
/*********************************************************************/
{
    extern const OM_string master_string_tbl[];
    OM_sint index;
    BOOLEAN error_found = FALSE;

    for ( index = 0; index < XT_LEN_STRING_TBL; index++) {
        if (xt_this_thread_data_for_string_check()->string_tbl[index].elements !=  master_string_tbl[index].elements) {
            printf("string_tbl[%i].elements was %s, while " "master_string_tbl[%i].elements was %s\n", index, xt_this_thread_data_for_string_check()->string_tbl[index].elements, index, master_string_tbl[index].elements);
            error_found = TRUE;
        } else if (strcmp(xt_this_thread_data_for_string_check()->string_tbl[index].elements, master_string_tbl[index].elements)) {
            printf("*string_tbl[%i].elements was %s, while " "*master_string_tbl[%i].elements was %s\n", index, xt_this_thread_data_for_string_check()->string_tbl[index].elements, index, master_string_tbl[index].elements);
            error_found = TRUE;
        } /* endif */
        if ( xt_this_thread_data_for_string_check()->string_tbl[index].length !=  master_string_tbl[index].length) {
            printf("string_tbl[%i].length was %i, while " "master_string_tbl[%i].length was %i\n", index,  xt_this_thread_data_for_string_check()->string_tbl[index].length, index, master_string_tbl[index].length);
            error_found = TRUE;
        } /* endif */
    } /* endfor */
    if ( error_found == TRUE) {
        xt_do_break("check string tbl mismatch");
    } /* endif */
} /* end xt_check_string_tbl */
