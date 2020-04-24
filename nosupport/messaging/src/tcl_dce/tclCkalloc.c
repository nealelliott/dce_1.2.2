/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: tclCkalloc.c,v $
 * Revision 1.1  1996/09/27  20:01:41  wwang
 * Initial revision
 *
 * Revision 1.1  1996/09/27  19:59:14  wwang
 * Initial revision
 *
 * Revision 1.1.2.1  1994/07/12  21:18:56  mori_m
 * 	CR 11259:  First drop for Tcl I18N (messaging work).
 * 	[1994/07/12  21:03:56  mori_m]
 *
 * $EndLog$
 */
/* 
 * tclCkalloc.c --
 *    Interface to malloc and free that provides support for debugging problems
 *    involving overwritten, double freeing memory and loss of memory.
 *
 * Copyright (c) 1991-1993 The Regents of the University of California.
 * All rights reserved.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 * 
 * IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF
 * CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * This code contributed by Karl Lehenbauer and Mark Diekhans
 *
 */

#include "tclInt.h"
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dcetclmsg.h>

#define FALSE	0
#define TRUE	1

#ifdef TCL_MEM_DEBUG
#ifndef TCL_GENERIC_ONLY
#include "tclUnix.h"
#endif

#define GUARD_SIZE 8

struct mem_header {
        long               length;
        char              *file;
        int                line;
        struct mem_header *flink;
        struct mem_header *blink;
	int		   dummy;	/* Aligns body on 8-byte boundary. */
        unsigned char      low_guard[GUARD_SIZE];
        char               body[1];
};

static struct mem_header *allocHead = NULL;  /* List of allocated structures */

#define GUARD_VALUE  0341

/* static char high_guard[] = {0x89, 0xab, 0xcd, 0xef}; */

static int total_mallocs = 0;
static int total_frees = 0;
static int current_bytes_malloced = 0;
static int maximum_bytes_malloced = 0;
static int current_malloc_packets = 0;
static int maximum_malloc_packets = 0;
static int break_on_malloc = 0;
static int trace_on_at_malloc = 0;
static int  alloc_tracing = FALSE;
static int  init_malloced_bodies = TRUE;
#ifdef MEM_VALIDATE
    static int  validate_memory = TRUE;
#else
    static int  validate_memory = FALSE;
#endif

/*
 * Prototypes for procedures defined in this file:
 */

static int		MemoryCmd _ANSI_ARGS_((ClientData clientData,
			    Tcl_Interp *interp, int argc, char **argv));

/*
 *----------------------------------------------------------------------
 *
 * dump_memory_info --
 *     Display the global memory management statistics.
 *
 *----------------------------------------------------------------------
 */
static void
dump_memory_info(outFile) 
    FILE *outFile;
{
	unsigned char *i18n_msg_format;

	/* i18n 
        fprintf(outFile,"total mallocs             %10d\n", 
                total_mallocs);
        fprintf(outFile,"total frees               %10d\n", 
                total_frees);
        fprintf(outFile,"current packets allocated %10d\n", 
                current_malloc_packets);
        fprintf(outFile,"current bytes allocated   %10d\n", 
                current_bytes_malloced);
        fprintf(outFile,"maximum packets allocated %10d\n", 
                maximum_malloc_packets);
        fprintf(outFile,"maximum bytes allocated   %10d\n", 
                maximum_bytes_malloced);
	*/
	i18n_msg_format = dce_msg_get(tcl_s_ckalloc_total_mallocs);
        fprintf(outFile, (char *)i18n_msg_format, total_mallocs);
	free(i18n_msg_format);

	i18n_msg_format = dce_msg_get(tcl_s_ckalloc_total_frees);
        fprintf(outFile, (char *)i18n_msg_format, total_frees);
	free(i18n_msg_format);

	i18n_msg_format = dce_msg_get(tcl_s_ckalloc_current_packets_alloc);
        fprintf(outFile, (char *)i18n_msg_format, current_malloc_packets);
	free(i18n_msg_format);

	i18n_msg_format = dce_msg_get(tcl_s_ckalloc_current_byte_alloc);
        fprintf(outFile, (char *)i18n_msg_format, current_bytes_malloced);
	free(i18n_msg_format);

	i18n_msg_format = dce_msg_get(tcl_s_ckalloc_max_packets_alloc);
        fprintf(outFile, (char *)i18n_msg_format, maximum_malloc_packets);
	free(i18n_msg_format);

	i18n_msg_format = dce_msg_get(tcl_s_ckalloc_max_bytes_alloc);
        fprintf(outFile, (char *)i18n_msg_format, maximum_bytes_malloced);
	free(i18n_msg_format);
}

/*
 *----------------------------------------------------------------------
 *
 * ValidateMemory --
 *     Procedure to validate allocted memory guard zones.
 *
 *----------------------------------------------------------------------
 */
static void
ValidateMemory (memHeaderP, file, line, nukeGuards)
    struct mem_header *memHeaderP;
    char              *file;
    int                line;
    int                nukeGuards;
{
    unsigned char *hiPtr;
    int   idx;
    int   guard_failed = FALSE;
    int byte;
    unsigned char *i18n_msg_format;
    
    for (idx = 0; idx < GUARD_SIZE; idx++) {
        byte = *(memHeaderP->low_guard + idx);
        if (byte != GUARD_VALUE) {
            guard_failed = TRUE;
            fflush (stdout);
	    byte &= 0xff;
	    i18n_msg_format = dce_msg_get(tcl_s_ckalloc_low_guard_byte);
            fprintf(stderr, (char *)i18n_msg_format, idx, byte,
		(isprint(UCHAR(byte)) ? byte : ' '));
	    free(i18n_msg_format);
        }
    }
    if (guard_failed) {
        dump_memory_info (stderr);

	i18n_msg_format = dce_msg_get(tcl_s_ckalloc_low_guard_failed);
        fprintf (stderr, (char *)i18n_msg_format, memHeaderP->body, file, line);
	free(i18n_msg_format);

        fflush (stderr);  /* In case name pointer is bad. */

	i18n_msg_format = dce_msg_get(tcl_s_ckalloc_bytes_allocated_at);
        fprintf (stderr, (char *)i18n_msg_format, memHeaderP->length,
		memHeaderP->file, memHeaderP->line);
	free(i18n_msg_format);

	i18n_msg_format = dce_msg_get(tcl_s_ckalloc_mem_val_failure);
        panic ((char *)i18n_msg_format);
    }

    hiPtr = (unsigned char *)memHeaderP->body + memHeaderP->length;
    for (idx = 0; idx < GUARD_SIZE; idx++) {
        byte = *(hiPtr + idx);
        if (byte != GUARD_VALUE) {
            guard_failed = TRUE;
            fflush (stdout);
	    byte &= 0xff;
	
	    i18n_msg_format = dce_msg_get(tcl_s_ckalloc_hi_guard_byte);
            fprintf(stderr, (char *)i18n_msg_format, idx, byte,
		(isprint(UCHAR(byte)) ? byte : ' '));
	    free(i18n_msg_format);
        }
    }

    if (guard_failed) {
        dump_memory_info (stderr);

	i18n_msg_format = dce_msg_get(tcl_s_ckalloc_hi_guard_failed);
        fprintf (stderr, (char *)i18n_msg_format, memHeaderP->body, file, line);
	free(i18n_msg_format);

        fflush (stderr);  /* In case name pointer is bad. */

	i18n_msg_format = dce_msg_get(tcl_s_ckalloc_bytes_allocated_at);
        fprintf (stderr, (char *)i18n_msg_format, memHeaderP->length,
		memHeaderP->file, memHeaderP->line);
	free(i18n_msg_format);

	i18n_msg_format = dce_msg_get(tcl_s_ckalloc_mem_val_failure);
        panic ((char *)i18n_msg_format);
    }

    if (nukeGuards) {
        memset ((char *) memHeaderP->low_guard, 0, GUARD_SIZE); 
        memset ((char *) hiPtr, 0, GUARD_SIZE); 
    }

}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_ValidateAllMemory --
 *     Validates guard regions for all allocated memory.
 *
 *----------------------------------------------------------------------
 */
void
Tcl_ValidateAllMemory (file, line)
    char  *file;
    int    line;
{
    struct mem_header *memScanP;

    for (memScanP = allocHead; memScanP != NULL; memScanP = memScanP->flink)
        ValidateMemory (memScanP, file, line, FALSE);

}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_DumpActiveMemory --
 *     Displays all allocated memory to stderr.
 *
 * Results:
 *     Return TCL_ERROR if an error accessing the file occures, `errno' 
 *     will have the file error number left in it.
 *----------------------------------------------------------------------
 */
int
Tcl_DumpActiveMemory (fileName)
    char *fileName;
{
    FILE              *fileP;
    struct mem_header *memScanP;
    char              *address;

    fileP = fopen (fileName, "w");
    if (fileP == NULL)
        return TCL_ERROR;

    for (memScanP = allocHead; memScanP != NULL; memScanP = memScanP->flink) {
        address = &memScanP->body [0];
        fprintf (fileP, "%8lx - %8lx  %7d @ %s %d", address,
                 address + memScanP->length - 1, memScanP->length,
                 memScanP->file, memScanP->line);
	(void) fputc('\n', fileP);
    }
    fclose (fileP);
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_DbCkalloc - debugging ckalloc
 *
 *        Allocate the requested amount of space plus some extra for
 *        guard bands at both ends of the request, plus a size, panicing 
 *        if there isn't enough space, then write in the guard bands
 *        and return the address of the space in the middle that the
 *        user asked for.
 *
 *        The second and third arguments are file and line, these contain
 *        the filename and line number corresponding to the caller.
 *        These are sent by the ckalloc macro; it uses the preprocessor
 *        autodefines __FILE__ and __LINE__.
 *
 *----------------------------------------------------------------------
 */
char *
Tcl_DbCkalloc(size, file, line)
    unsigned int size;
    char        *file;
    int          line;
{
    struct mem_header *result;
    unsigned char *i18n_msg_format;
    unsigned char *i18n_msg_buf;

    if (validate_memory)
        Tcl_ValidateAllMemory (file, line);

    result = (struct mem_header *)malloc((unsigned)size + 
                              sizeof(struct mem_header) + GUARD_SIZE);
    if (result == NULL) {
        fflush(stdout);
        dump_memory_info(stderr);

	i18n_msg_format = dce_msg_get(tcl_s_ckalloc_unable_to_alloc_bytes_line);
	if (i18n_msg_format == NULL)
		panic("unable to alloc %d bytes, %s line %d", size, file, line);

	i18n_msg_buf = (unsinged char *)malloc(strlen(i18n_msg_format) 
				+ sizeof(size) + strlen(file) + sizeof(line) + 1);
	if (i18n_msg_buf == NULL)
		panic("unable to alloc %d bytes, %s line %d", size, file, line);
	else
		sprintf(i18n_msg_buf, i18n_msg_format, size, file, line);
	free(i18n_msg_format);
        panic ((char *)i18n_msg_buf);
    }

    /*
     * Fill in guard zones and size.  Also initialize the contents of
     * the block with bogus bytes to detect uses of initialized data.
     * Link into allocated list.
     */
    if (init_malloced_bodies) {
        memset ((VOID *) result, GUARD_VALUE,
		size + sizeof(struct mem_header) + GUARD_SIZE);
    } else {
	memset ((char *) result->low_guard, GUARD_VALUE, GUARD_SIZE);
	memset (result->body + size, GUARD_VALUE, GUARD_SIZE);
    }
    result->length = size;
    result->file = file;
    result->line = line;
    result->flink = allocHead;
    result->blink = NULL;
    if (allocHead != NULL)
        allocHead->blink = result;
    allocHead = result;

    total_mallocs++;
    if (trace_on_at_malloc && (total_mallocs >= trace_on_at_malloc)) {
        (void) fflush(stdout);

	i18n_msg_format = dce_msg_get(tcl_s_ckalloc_reached_malloc_trace_enable);
        fprintf(stderr, (char *)i18n_msg_format, total_mallocs);
	free(i18n_msg_format);

        fflush(stderr);
        alloc_tracing = TRUE;
        trace_on_at_malloc = 0;
    }

    if (alloc_tracing)
        fprintf(stderr,"ckalloc %lx %d %s %d\n", result->body, size, 
                file, line);

    if (break_on_malloc && (total_mallocs >= break_on_malloc)) {
        break_on_malloc = 0;
        (void) fflush(stdout);

	i18n_msg_format = dce_msg_get(tcl_s_ckalloc_reached_malloc_break_limit);
        fprintf(stderr, (char *)i18n_msg_format, total_mallocs);
	free(i18n_msg_format);

	i18n_msg_format = dce_msg_get(tcl_s_ckalloc_program_enter_debugger);
        fprintf(stderr, (char *)i18n_msg_format);
	free(i18n_msg_format);

        (void) fflush(stderr);
	abort();
    }

    current_malloc_packets++;
    if (current_malloc_packets > maximum_malloc_packets)
        maximum_malloc_packets = current_malloc_packets;
    current_bytes_malloced += size;
    if (current_bytes_malloced > maximum_bytes_malloced)
        maximum_bytes_malloced = current_bytes_malloced;

    return result->body;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_DbCkfree - debugging ckfree
 *
 *        Verify that the low and high guards are intact, and if so
 *        then free the buffer else panic.
 *
 *        The guards are erased after being checked to catch duplicate
 *        frees.
 *
 *        The second and third arguments are file and line, these contain
 *        the filename and line number corresponding to the caller.
 *        These are sent by the ckfree macro; it uses the preprocessor
 *        autodefines __FILE__ and __LINE__.
 *
 *----------------------------------------------------------------------
 */

int
Tcl_DbCkfree(ptr, file, line)
    char *  ptr;
    char     *file;
    int       line;
{
    struct mem_header *memp = 0;  /* Must be zero for size calc */

    /*
     * Since header ptr is zero, body offset will be size
     */
#ifdef _CRAYCOM
    memp = (struct mem_header *)((char *) ptr  - (sizeof(int)*((unsigned)&(memp->body))));
#else
    memp = (struct mem_header *)(((char *) ptr) - (int)memp->body);
#endif

    if (alloc_tracing)
        fprintf(stderr, "ckfree %lx %ld %s %d\n", memp->body, 
                memp->length, file, line);

    if (validate_memory)
        Tcl_ValidateAllMemory (file, line);

    ValidateMemory (memp, file, line, TRUE);
    if (init_malloced_bodies) {
	memset((VOID *) ptr, GUARD_VALUE, memp->length);
    }

    total_frees++;
    current_malloc_packets--;
    current_bytes_malloced -= memp->length;

    /*
     * Delink from allocated list
     */
    if (memp->flink != NULL)
        memp->flink->blink = memp->blink;
    if (memp->blink != NULL)
        memp->blink->flink = memp->flink;
    if (allocHead == memp)
        allocHead = memp->flink;
    free((char *) memp);
    return 0;
}

/*
 *--------------------------------------------------------------------
 *
 * Tcl_DbCkrealloc - debugging ckrealloc
 *
 *	Reallocate a chunk of memory by allocating a new one of the
 *	right size, copying the old data to the new location, and then
 *	freeing the old memory space, using all the memory checking
 *	features of this package.
 *
 *--------------------------------------------------------------------
 */
char *
Tcl_DbCkrealloc(ptr, size, file, line)
    char *ptr;
    unsigned int size;
    char *file;
    int line;
{
    char *new;
    unsigned int copySize;
    struct mem_header *memp = 0;  /* Must be zero for size calc */

#ifdef _CRAYCOM
    memp = (struct mem_header *)((char *) ptr  - (sizeof(int)*((unsigned)&(memp->body))));
#else
    memp = (struct mem_header *)(((char *) ptr) - (int)memp->body);
#endif
    copySize = size;
    if (copySize > memp->length) {
	copySize = memp->length;
    }
    new = Tcl_DbCkalloc(size, file, line);
    memcpy((VOID *) new, (VOID *) ptr, (int) copySize);
    Tcl_DbCkfree(ptr, file, line);
    return(new);
}

/*
 *----------------------------------------------------------------------
 *
 * MemoryCmd --
 *     Implements the TCL memory command:
 *       memory info
 *       memory display
 *       break_on_malloc count
 *       trace_on_at_malloc count
 *       trace on|off
 *       validate on|off
 *
 * Results:
 *     Standard TCL results.
 *
 *----------------------------------------------------------------------
 */
	/* ARGSUSED */
static int
MemoryCmd (clientData, interp, argc, argv)
    ClientData  clientData;
    Tcl_Interp *interp;
    int         argc;
    char      **argv;
{
    char *fileName;
    Tcl_DString buffer;
    int result;
    unsigned char *i18n_msg_buf;
    unsigned char *i18n_msg_format;

    if (argc < 2) {
	/* i18n
	Tcl_AppendResult(interp, "wrong # args:  should be \"",
		argv[0], " option [args..]\"", (char *) NULL);
	*/
	i18n_msg_format = dce_msg_get(tcl_s_ckalloc_wnum_args_option);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
				+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }

    if (strcmp(argv[1],"trace") == 0) {
        if (argc != 3) 
            goto bad_suboption;
        alloc_tracing = (strcmp(argv[2],"on") == 0);
        return TCL_OK;
    }
    if (strcmp(argv[1],"init") == 0) {
        if (argc != 3)
            goto bad_suboption;
        init_malloced_bodies = (strcmp(argv[2],"on") == 0);
        return TCL_OK;
    }
    if (strcmp(argv[1],"validate") == 0) {
        if (argc != 3)
             goto bad_suboption;
        validate_memory = (strcmp(argv[2],"on") == 0);
        return TCL_OK;
    }
    if (strcmp(argv[1],"trace_on_at_malloc") == 0) {
        if (argc != 3) 
            goto argError;
        if (Tcl_GetInt(interp, argv[2], &trace_on_at_malloc) != TCL_OK)
                return TCL_ERROR;
         return TCL_OK;
    }
    if (strcmp(argv[1],"break_on_malloc") == 0) {
        if (argc != 3) 
            goto argError;
        if (Tcl_GetInt(interp, argv[2], &break_on_malloc) != TCL_OK)
                return TCL_ERROR;
        return TCL_OK;
    }

    if (strcmp(argv[1],"info") == 0) {
        dump_memory_info(stdout);
        return TCL_OK;
    }
    if (strcmp(argv[1],"active") == 0) {
        if (argc != 3) {
	    /* i18n
	    Tcl_AppendResult(interp, "wrong # args:  should be \"",
		    argv[0], " active file", (char *) NULL);
	     */

	    i18n_msg_format = dce_msg_get(tcl_s_ckalloc_wnum_args_active_file);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
				+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	fileName = Tcl_TildeSubst(interp, argv[2], &buffer);
	if (fileName == NULL) {
	    return TCL_ERROR;
	}
	result = Tcl_DumpActiveMemory (fileName);
	Tcl_DStringFree(&buffer);
	if (result != TCL_OK) {
	    /* i18n
	    Tcl_AppendResult(interp, "error accessing ", argv[2], 
		    (char *) NULL);
	     */
	    i18n_msg_format = dce_msg_get(tcl_s_ckalloc_error_accessing);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[2]) 
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[2]);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	return TCL_OK;
    }
    /* i18n
    Tcl_AppendResult(interp, "bad option \"", argv[1],
	    "\":  should be info, init, active, break_on_malloc, ",
	    "trace_on_at_malloc, trace, or validate", (char *) NULL);
     */
    i18n_msg_format = dce_msg_get(tcl_s_ckalloc_bad_option);
    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[1]) 
					+ strlen((char *)i18n_msg_format) + 1);
    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[1]);
    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
    free(i18n_msg_format);
    ckfree(i18n_msg_buf);

    return TCL_ERROR;

argError:
    /* i18n
    Tcl_AppendResult(interp, "wrong # args:  should be \"", argv[0],
	    " ", argv[1], "count\"", (char *) NULL);
     */
    i18n_msg_format = dce_msg_get(tcl_s_ckalloc_wnum_args_count);
    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0]) 
					+ strlen(argv[1])
					+ strlen((char *)i18n_msg_format) + 1);
    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0], argv[1]);
    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
    free(i18n_msg_format);
    ckfree(i18n_msg_buf);

    return TCL_ERROR;

bad_suboption:
    /* i18n
    Tcl_AppendResult(interp, "wrong # args:  should be \"", argv[0],
	    " ", argv[1], " on|off\"", (char *) NULL);
     */
    i18n_msg_format = dce_msg_get(tcl_s_ckalloc_wnum_args_on_off);
    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0]) 
					+ strlen(argv[1])
					+ strlen((char *)i18n_msg_format) + 1);
    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0], argv[1]);
    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
    free(i18n_msg_format);
    ckfree(i18n_msg_buf);

    return TCL_ERROR;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_InitMemory --
 *     Initialize the memory command.
 *
 *----------------------------------------------------------------------
 */
void
Tcl_InitMemory(interp)
    Tcl_Interp *interp;
{
Tcl_CreateCommand (interp, "memory", MemoryCmd, (ClientData) NULL, 
                  (Tcl_CmdDeleteProc *) NULL);
}

#else


/*
 *----------------------------------------------------------------------
 *
 * Tcl_Ckalloc --
 *     Interface to malloc when TCL_MEM_DEBUG is disabled.  It does check
 *     that memory was actually allocated.
 *
 *----------------------------------------------------------------------
 */
VOID *
Tcl_Ckalloc (size)
    unsigned int size;
{
        char *result;
	unsigned char *i18n_msg_format;
	unsigned char *i18n_msg_buf;

        result = malloc(size);
        if (result == NULL) 
	{
		/* i18n
                panic("unable to alloc %d bytes", size);
		 */
		i18n_msg_format = dce_msg_get(tcl_s_ckalloc_unable_to_alloc);
		if (i18n_msg_format == NULL)
			panic("unable to alloc %d bytes", size);

		i18n_msg_buf = (unsigned char *)malloc(strlen((char *)i18n_msg_format) 
				+ sizeof(size) + 1);
		if (i18n_msg_buf == NULL)
			panic("unable to alloc %d bytes", size);
		else
			sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, size);
		free(i18n_msg_format);
        	panic ((char *)i18n_msg_buf);
	}
        return result;
}

/*
 *----------------------------------------------------------------------
 *
 * TckCkfree --
 *     Interface to free when TCL_MEM_DEBUG is disabled.  Done here rather
 *     in the macro to keep some modules from being compiled with 
 *     TCL_MEM_DEBUG enabled and some with it disabled.
 *
 *----------------------------------------------------------------------
 */
void
Tcl_Ckfree (ptr)
    VOID *ptr;
{
        free (ptr);
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_InitMemory --
 *     Dummy initialization for memory command, which is only available 
 *     if TCL_MEM_DEBUG is on.
 *
 *----------------------------------------------------------------------
 */
	/* ARGSUSED */
void
Tcl_InitMemory(interp)
    Tcl_Interp *interp;
{
}

#endif
