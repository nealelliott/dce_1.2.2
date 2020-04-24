/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnsmalloc.c,v $
 * Revision 1.1.8.2  1996/02/18  19:34:24  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:14:21  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:24:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:46:27  root]
 * 
 * Revision 1.1.6.8  1994/09/22  18:35:26  mccann
 * 	move defines to cds.mk
 * 	[1994/09/22  18:34:02  mccann]
 * 
 * Revision 1.1.6.7  1994/08/03  19:03:07  mccann
 * 	includes file cleanup
 * 	[1994/07/19  18:09:53  mccann]
 * 
 * Revision 1.1.6.6  1994/06/30  19:17:20  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:02:00  mccann]
 * 
 * Revision 1.1.6.5  1994/06/09  18:41:56  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:14:07  devsrc]
 * 
 * Revision 1.1.6.4  1994/06/03  18:44:37  jd
 * 	(10744) Added missing DNS_INIT in dns_get_buffer.
 * 	[1994/06/03  18:38:15  jd]
 * 
 * Revision 1.1.6.3  1994/04/29  15:54:18  peckham
 * 	Use dcecdsmac.h macros for dce_svc_printf().
 * 	[1994/04/29  14:24:19  peckham]
 * 
 * Revision 1.1.6.2  1994/03/22  20:00:10  griffin
 * 	Modified instrumentation events to Level 9 so the serviceability
 * 	logs aren't as cluttered.
 * 	[1994/03/22  16:41:07  griffin]
 * 
 * Revision 1.1.6.1  1994/03/12  22:07:13  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:30:56  peckham]
 * 
 * Revision 1.1.3.5  1993/03/03  17:25:34  peckham
 * 	OT#7358: Whoops! This module is not thread-safe. Add a mutex
 * 	and lock it when we are fiddling with links and common data.
 * 	[1993/03/03  17:24:44  peckham]
 * 
 * Revision 1.1.3.4  1993/02/01  20:26:41  hinman
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  15:58:24  hinman]
 * 
 * Revision 1.1.3.3  1992/12/30  13:57:49  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:13:08  zeliff]
 * 	Revision 1.1.4.2  1993/01/11  16:09:31  hinman
 * 	[hinman] - Check in merged SNI version
 * 
 * Revision 9.5.1.4  92/11/17  16:25:28  blurie
 * 	Took out ifdef which set memory checking on for SVR4 by default.
 * 	[92/11/17  16:24:53  blurie]
 * 
 * Revision 9.5.1.3  92/09/30  14:16:38  root
 * 	Acceptance of OSF rev 1.1.3.2
 * 
 * Revision 9.12.1.2  92/09/30  09:44:56  hinman
 * 	Auto checkin of OSF rev 1.1.3.2
 * 
 * Revision 1.1.3.2  1992/09/29  18:52:17  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:15:47  weisman]
 * 
 * Revision 1.1  1992/01/19  15:18:38  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* Module dnsmalloc.c
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990-1994. ALL RIGHTS RESERVED.
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
 * AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 *
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
 *
 */
#include <dce/dce.h>
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>

/*
 * Module Conditional-Compilation behaviors:
 *
 *   VMS - Uses LIB$GET_VM and zones for improved performance
 *   DNS_MALLOC
 */
#include <dce/dns_config.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <dns_malloc.h>
#include <dns_strings.h>




/* 
 * Local Function Prototypes...
 */
static int
show_debug(char       *, 
	   void		*);

/*
 * This routine should be "static" since it is NOT used in any
 * modules, but rather it's only for use in a debugger such as dbx.
 * Because the gcc compiler warns of it being unused, it's
 * made non static. 
 */
void 
print_malloc_info (
	    void);


/*
 * No data is safe if you are running threads...
 */
static pthread_once_t cdsmalloc__once = pthread_once_init;
static pthread_mutex_t cdsmalloc__mutex;


/*
 * Initialize this module's statics
 */
static void 
cdsmalloc__init (void)
{
    pthread_mutex_init(&cdsmalloc__mutex, pthread_mutexattr_default);
}


#define DNS_INIT       { pthread_once(&cdsmalloc__once, cdsmalloc__init); }
#define DNS_LOCK       { pthread_mutex_lock(&cdsmalloc__mutex); }
#define DNS_END_LOCK   { pthread_mutex_unlock(&cdsmalloc__mutex); }

/*
 *          Typed Memory Pools (Buffer Farm) Definitions
 *
 * See dns_get_buffer routine description for buffer farm operation.
 *
 * The buffer_farm vector is guaranteed by C to be initialized to null
 * pointers.
 */

#define MAXIMUM_BUFFER_POOLS 20
#define POOL_CHECK_VALUE 0x47474747

typedef struct typed_mem_list_s {
    struct typed_mem_list_s *link;
    int    buffer_size;
    int    available;
    int    reuse_count;
    int    pool_check_word;
} typed_mem_list_t;

static typed_mem_list_t *buffer_farm[MAXIMUM_BUFFER_POOLS];


/* ------------------------------------------------------------------------- */

/*
 *               Implementation-Specific Macros, Definitions, etc.
 */

/* ------------------------------------------------------------------------- */

/*
 *        Diagnostic and Instrumentation Support Structure Definitions
 */


/*
 * lastmallocs is a ring-buffer of that holds the recent malloc/free history
 */

typedef struct mem_save_list {
    void	*addr;				/* memory being accessed */
    char   	*filename_p;			/* Identify file name of call*/
    mem_type_t 	type;				/* Identify type of memory  */
    int	   	allocno;			/* Uniquely identify malloc  */
    int		freeno;				/* Uniquely identify free  */
    int		length;				/* Length of allocation  */
    int	   	lineno;				/* Identify line of malloc */
} mem_save_list;

#if defined(DNS_MALLOC)

#  define LASTNMALLOCS	500 		/* Keep track of the last  */
static mem_save_list lastmallocs[ LASTNMALLOCS]; /* so many memory allocations*/
static int lastndx = 0;

#else
#  define LASTNMALLOCS	1 		/* Just for things to compile*/
#endif





/*
 * When diagnostic support is enabled, all memory fragments (allocation
 * chunks, whatever) are wrapped with data controlled by the dns_malloc
 * routine:
 *
 *    +-------------------+-----------------/----------------+-----------+
 *    | mem_list_t header |    ... user requested buffer ... |upper_guard|
 *    +-------------------+-----------------/----------------+-----------+
 *
 * The mem_list_t structure records who issued the malloc call, a relative
 * timestamp (allocno) of the call, and places guard values just before
 * and just after the user area - to detect data underruns or overruns.
 *
 * All allocated chunks are linked together so they may be scanned by
 * the diagnostic and instrumentation routines.  The list is double-linked,
 * null-terminated.
 *
 */

#define	GUARD_VALUE	0x5A5A5A5A
#define GUARD_SIZE	sizeof(char *)
#define FREED_BLOCK     -1

typedef struct mem_list_s {
    struct mem_list_s *next_p;			/* Next allocated area */
    struct mem_list_s *prev_p;			/* Previous allocated area */
    char	*upper_guard_p;			/* Points to guard word */
    char	*filename_p;			/* Identify file name of call*/
    mem_type_t 	type;				/* Identify type of memory  */
    int		allocno;			/* Uniquely identify malloc  */
    int		lineno;				/* Identify line of malloc */
    int		lastmallocs_index;		/* Index into lastmallocs */
    char	superchunk_pool;		/* index of superchunk pool */
    char	superchunk_offset;		/* Offset into superchunk */
    int		lower_guard;
} mem_list_t;

#define INSTRUMENTATION_OVERHEAD (sizeof(mem_list_t) + GUARD_SIZE)


#if defined(DNS_MALLOC)

static mem_list_t *mem_list = NULL;

#endif


/*
 * Superchunks are large chunks of memory allocated to hold fixed-size
 * memory requests.  The contents are expected to be primarily permanent
 * allocations, but the algorithms handle free'd data as well.
 *
 * Two superchunk lists are maintained: one for 8K requests, and one for
 * 4K requests.
 *
 *    +-----------------------+-------------/----------------+
 *    | superchunk_list_header|    31 fixed sizes buffers    |
 *    +-----------------------+-------------/----------------+
 *
 * The superchunks (for a certain size) are all linked together.  A bitmap
 * in the header keeps track of the free and allocated buffers (elements)
 * within a superchunk.
 *
 * The list is maintained in LIFO order, with the hope that older data
 * (further down the list) will be filled in more than the recent allocations.
 * The search for a free slot is exhaustive, so new superchunks are not
 * allocated until all elements within existing superchunks are taken.
 *
 * Elements within superchunks are treated by the rest of memory management
 * just like any other fragments, e.g., they maintain the same instrumentation
 * overhead (indeed, it is required for superchunks to work).
 *
 * Each element of a superchunk maintains memory of which element of the
 * superchunk it is located at.  This allows the free routine to backtrack
 * to the superchunk header and mark the element free (the elements are
 * numbered 1 to 31, so a non-zero superchunk offset is used to activate
 * superchunk frees.
 *
 * Superchunk mode can be enabled and disabled freely during program operation
 * with no ill side-effects.
 */

typedef unsigned long superchunk_bitmap_t;

typedef struct superchunk_list_s {
    struct superchunk_list_s *link;		/* Next superchunk */
    superchunk_bitmap_t	superchunk_bitmap;	/* Usage bitmap bits */
    int			superchunk_allocno;	/* Memory clock */
    int			superchunk_checkword;	/* Integrity check */
} superchunk_list_t;

#define	SUPER_GUARD_VALUE   0x58585858

typedef struct superchunk_pool_s {
    int			superchunk_size;
    char		*superchunk_display;
    superchunk_list_t	*superchunk_head;
} superchunk_pool_t;


#if defined(DNS_MALLOC)

/*
 * This table is scanned from last to first
 * so that failure produces a zero index.
 */
static superchunk_pool_t Super[] = {
    {0, 0},
    {4096, "4K"},
    {8192, "8K"}
};

#endif


#define LAST_SUPER (&Super[2])

#define SUPERCHUNK_ELEMENT(sp) ((sp)->superchunk_size+INSTRUMENTATION_OVERHEAD)

#define SUPERCHUNK_ALLOCATION(sp) (sizeof(superchunk_list_t) \
    + (sizeof(superchunk_bitmap_t) * 8 * SUPERCHUNK_ELEMENT(sp)))


#if defined(DNS_MALLOC)

/*
 * mem_counter is a virtual clock that ticks each time a malloc or free
 * is called.  This allows us to see when memory blocks were created and
 * freed relative to each other.
 */
static int mem_counter = 0;

#endif


/*
 * Instrumentation Controls
 *
 * checking_all_memory controls whether or not all allocated blocks are
 * integrity checked each time a malloc or free is called.
 *
 * abort_on_full_memory determines the behavior of malloc when a request
 * for memory cannot be satisfied.
 */
static int checking_all_memory = TRUE;          /* Diagnostic check override */
static int abort_on_full_memory = 0;            /* Diagnostic check override */
static int superchunk_mode = 0;                 /* Make use of superchunks   */

/* ------------------------------------------------------------------------- */

/*
 *         BEGIN - INSTRUMENTED MEMORY ALLOCATION SUPPORT ROUTINES
 *
 * The following set of routines provide for instrumentation of the
 * memory allocation mechanisms.
 */

#if defined(DNS_MALLOC)


# if _DNS_OS_ == _DNS__VMS

   static int vm_zone = 0;         /* VM Zone identifier */

#  define malloc(l) get_vm(l)
#  define free(b)   free_vm(b)

# endif	/* VMS */

   static void *malloc_superchunk(
       superchunk_pool_t  *,
       int                *);
   static void free_superchunk(
       superchunk_pool_t  *,
       mem_list_t         *);

# if _DNS_OS_ == _DNS__VMS

/*
 * get_vm  - envelope for LIB$GET_VM
 * free_vm - envelope for LIB$FREE_VM
 * show_vm_info - diagnostic information
 *
 * These routines, conditionally-compiled, makes LIB$GET_VM look like malloc()
 * and LIB$FREE_VM look like free().
 */
static void *
get_vm (int length)
{
    void *base;
    register int status;

    /*
     * Create the VM zone if we haven't done so already.
     *
     * The zone we are creating tries to have optimal characteristics for
     * the DNS server.
     */
    if (vm_zone == 0) 
    {
        int zone_attributes = 0;

        zone_attributes |= 0x00000001;     /* Boundary tags (malloc emul) */


        zone_attributes |= 0x00000002;     /* Fill 00 on mallocs (get_vm) */
        zone_attributes |= 0x00000010;     /* Fill ff on frees (free_vms) */


#       include <pragma_nostandard.h>
        status = lib$create_vm_zone(&vm_zone,
                           &3,          /* Frequent sizes */
                           &6,          /* Six lookaside lists */
                           &zone_attributes,
                           0,           /* Extend size */
                           0,           /* Initial size */
                           0,           /* Default block size (8) */
                           &16,         /* Octaword alignment */
                           0,           /* No page limit */
                           0,           /* No minimum block size */
                           0,           /* Zone has no name */
                           0            /* No areas */
                           );

#       include <pragma_standard.h>

        if (status != 1) 
	{
	  dce_svc_printf(CDS_S_LIBRARY_CDSMALLOC_A_MSG);
        }
    }

    /*
     * Get a chunk of memory, return the base address like malloc does.
     */

    status = lib$get_vm(&length, &base, &vm_zone);
    if (status == 1) 
    {
        return (base);
    } 
    else 
    {
        return (NULL);
    }

}

/* ------------------------------------------------------------------------- */

static void 
free_vm (void *base)
{
    register int status;

    /*
     * Free the requested memory fragment.
     * This routine assumes that the vm_zone variable has been set by a
     * prior malloc() -- if not, there are big problems anyway.
     */

    status = lib$free_vm(0, &base, &vm_zone);

    if (status != 1) 
    {
      dce_svc_printf(CDS_S_LIBRARY_CDSMALLOC_A_MSG);
    }
}

static void 
show_vm_info (void)
{
    int detail_level = 3;  /* Show everything */

    lib$show_vm();                             /* General VM statistics */
    lib$show_vm_zone(&vm_zone, &detail_level); /* Zone statistics       */

}

#  endif  /* VMS */



#ifdef DNS_MALLOC

/*
 * ROUTINE - print_malloc_info
 *
 * Diagnostic dump of allocated memory
 *
 * This routine is meant to be called from a debugger.  It runs through
 * the allocation list and prints it to stdout.
 */
void 
print_malloc_info (void)
{
    int fragment_count = 0;
    int fragment_total = 0;

    DNS_INIT;

    fprintf(stderr, "Malloc fragment dump\nAddress   Size");

    DNS_LOCK 
    {
	mem_list_t *mem_p;

	for (mem_p = mem_list; mem_p; mem_p = mem_p->next_p) 
	{
	    const char *const upper_p = mem_p->upper_guard_p;
	    const int fragment_size = upper_p - (char *)&mem_p[1];

	    fragment_count++;
	    fragment_total += fragment_size;

	    fprintf(stderr, "%8x  %8d",  mem_p, fragment_size);

	    if ((mem_p->lower_guard != GUARD_VALUE)
		|| (upper_p[0] != ((GUARD_VALUE>> 0)&0xFF))
		|| (upper_p[1] != ((GUARD_VALUE>> 8)&0xFF))
		|| (upper_p[2] != ((GUARD_VALUE>>16)&0xFF))
		|| (upper_p[3] != ((GUARD_VALUE>>24)&0xFF))) 
	    {
	      fprintf(stderr, "print_malloc_info: memory guard word trashed");

	      abort();   /* get a core file to look at */
	    }
	}
    } DNS_END_LOCK;

    fprintf(stderr, "Total = %d bytes, %d fragments",  fragment_total,
	    fragment_count);
}
#endif /* DNS_MALLOC */

/*
 * ROUTINE - check_mem
 *
 * Do sanity check on allocated memory
 *
 * This routine runs through the allocation list and looks for corrupted
 * blocks (by examining the guard values).  It aborts if it finds a
 * corrupted block.
 */
static void 
check_mem (void)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " >check_mem"));

    DNS_LOCK {
	mem_list_t *mem_p = (mem_list_t *)&mem_list;

	for (mem_p = mem_list; mem_p; mem_p = mem_p->next_p) {
	    const char *const upper_p = mem_p->upper_guard_p;

	    if ((mem_p->lower_guard != GUARD_VALUE)
		|| (upper_p[0] != ((GUARD_VALUE>> 0)&0xFF))
		|| (upper_p[1] != ((GUARD_VALUE>> 8)&0xFF))
		|| (upper_p[2] != ((GUARD_VALUE>>16)&0xFF))
		|| (upper_p[3] != ((GUARD_VALUE>>24)&0xFF))) 
	    {
		dce_svc_printf(CDS_S_LIBRARY_CDSMALLOC_A_MSG);
	    }
	}
    } DNS_END_LOCK;
  DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug9,
        " <check_mem"));
}

/*
 * ROUTINE - dns_malloc_instr
 *
 * Our little version of malloc() - instrumented for debugging, etc.
 *
 * This is the wrapper for malloc().  It performs pool-wide integrity
 * checks of memory, maintains the diagnostic support structures, etc.
 */
void *
dns_malloc_instr (int         length,
                  mem_type_t  type,
                  int         lineno,
                  char        *filename_p)
{
    superchunk_pool_t  *sc_pool = superchunk_mode ? LAST_SUPER : Super;
    char       *adr_p;
    int         superchunk_offset;

    DNS_INIT;

    if (checking_all_memory)
        check_mem();

    while (1) {
	if (sc_pool->superchunk_size == 0) {
	    adr_p = (char *)malloc(sizeof(mem_list_t) + length + GUARD_SIZE);
	    superchunk_offset = 0;
	    break;
	}
	if (length == sc_pool->superchunk_size) {
	    adr_p = (char *)malloc_superchunk(sc_pool, &superchunk_offset);
	    break;
	}
	--sc_pool;
    }

    if (adr_p) {
	mem_list_t *const mem_p = (mem_list_t *)adr_p;

	adr_p = (char *)&mem_p[1];
	mem_p->type = type;		  /* Permanent, etc. See mem_* */
	mem_p->superchunk_offset = superchunk_offset;
	mem_p->superchunk_pool = sc_pool - Super;
	mem_p->lineno = lineno;
	mem_p->upper_guard_p = &adr_p[length];
	mem_p->filename_p = filename_p;
	mem_p->lower_guard = GUARD_VALUE;
	mem_p->upper_guard_p[0] = (GUARD_VALUE>> 0)&0xFF;
	mem_p->upper_guard_p[1] = (GUARD_VALUE>> 8)&0xFF;
	mem_p->upper_guard_p[2] = (GUARD_VALUE>>16)&0xFF;
	mem_p->upper_guard_p[3] = (GUARD_VALUE>>24)&0xFF;
	mem_p->prev_p = NULL;

	DNS_LOCK {
	    mem_save_list *const last_p = &lastmallocs[lastndx];
	    mem_list_t *const next_p = mem_list;

	    /* Load up the diagnostic structure, set up the guards */
	    if ((mem_p->next_p = next_p) != NULL)
		next_p->prev_p = mem_p;
	    mem_list = mem_p;

	    mem_p->allocno = ++mem_counter; /* Uniquely identify malloc  */
	    mem_p->lastmallocs_index = lastndx;

	    if (LASTNMALLOCS <= ++lastndx)
		lastndx = 0;

	    /* Update the history buffer */
	    last_p->addr = (void *)adr_p;
	    last_p->type = type;	 /* Keep track of last allocs */
	    last_p->length = length;
	    last_p->freeno = 0;
	    last_p->allocno = mem_counter;
	    last_p->lineno = lineno;
	    last_p->filename_p = filename_p;
	} DNS_END_LOCK;
    }

    if (!adr_p && abort_on_full_memory)		 /* Ran out of memory  */
	
    {
	dns_diag_vm_lastmallocs();
	dns_diag_vm_dumpall();

	dce_svc_printf(CDS_S_LIBRARY_CDSMALLOC_A_MSG);
    }
    return(adr_p);
}

/*
 * ROUTINE - dns_free
 *
 * Our version of free().
 *
 * Checks memory integrity, updates diagnostic support structures as
 * required.
 */
void 
dns_free (void *adr_p)
{
    mem_list_t *const mem_p = &((mem_list_t *)adr_p)[-1];
    mem_save_list *const last_p = &lastmallocs[mem_p->lastmallocs_index];

    DNS_INIT;

    /*
     * Do block validation first (all memory (optional), guard words,
     * and double free detection.
     */

    if (checking_all_memory)
        check_mem();

    if ((mem_p->lower_guard != GUARD_VALUE)
	|| (mem_p->upper_guard_p[0] != ((GUARD_VALUE>> 0)&0xFF))
	|| (mem_p->upper_guard_p[1] != ((GUARD_VALUE>> 8)&0xFF))
	|| (mem_p->upper_guard_p[2] != ((GUARD_VALUE>>16)&0xFF))
	|| (mem_p->upper_guard_p[3] != ((GUARD_VALUE>>24)&0xFF))) 
    {
	dce_svc_printf(CDS_S_LIBRARY_CDSMALLOC_A_MSG);
    }

    if (mem_p->lastmallocs_index == FREED_BLOCK) 
    {
	dce_svc_printf(CDS_S_LIBRARY_CDSMALLOC_A_MSG);
    }

    /* Update malloc history - if we can */

    DNS_LOCK {
	++mem_counter;
	if (last_p->allocno == mem_p->allocno)
	    last_p->freeno = mem_counter;

	/* Remove block from memory list, mark it as freed */

	if (mem_p->next_p)
	    mem_p->next_p->prev_p = mem_p->prev_p;

	if (mem_p->prev_p) {
	    mem_p->prev_p->next_p = mem_p->next_p;
	} else {
	    mem_list = mem_p->next_p;
	}
    } DNS_END_LOCK;

    /* Free up the block */

    if (mem_p->superchunk_pool) 
    {
	mem_p->lastmallocs_index = FREED_BLOCK;
        free_superchunk(&Super[(int)mem_p->superchunk_pool], mem_p);
    } 
    else 
    {
  	free(mem_p);
    }
}


/*
 *                     Superchunk maintenance routines
 */

/*
 * ROUTINE - malloc_superchunk
 *
 * This routine is called whenever dns_malloc detects a request for a
 * superchunk managed size fragment of memory (and superchunk_mode is on).
 */

static void *
malloc_superchunk (superchunk_pool_t  *sc_pool,
                   int                *offset)
{
    char *adr_p = (char *)0;
    const int sc_element_size = SUPERCHUNK_ELEMENT(sc_pool);

    /*
     * Search the superchunk list searching for any free memory that might
     * be available.  [An optimization for this would be to keep track of
     * the number of open slots, to avoid a needless search.]
     */

    DNS_LOCK {
	superchunk_list_t *sc_p;

	for (sc_p = sc_pool->superchunk_head; sc_p; sc_p = sc_p->link) {
	    /* Any open spots? */
	    if (sc_p->superchunk_bitmap) {
		/*
		 * Some element of the superchunk is free -- find it!
		 * Shift a bitmap mask left until a 1 bit is found.
		 */

		superchunk_bitmap_t bitmask = 1L;
		int bitmap_index = 0;

		while ( !(sc_p->superchunk_bitmap & bitmask) ) {
		    bitmask <<= 1;
		    bitmap_index++;
		}

		/*
		 * We now have an index into the superchunk which corresponds
		 * to a free area.   We will mark it as filled, and calculate
		 * the address to return to the caller.
		 */
		*offset = bitmap_index;
		sc_p->superchunk_bitmap ^= bitmask;
		adr_p = &((char *)&sc_p[1])[bitmap_index * sc_element_size];
		break;
	    }
	}
    } DNS_END_LOCK;

    /*
     * If we reached the end of the list without finding any free blocks,
     * it's time to allocate a new superchunk and fill it in.
     */

    if (!adr_p) {
	superchunk_list_t *const sc_p =
		    (superchunk_list_t *)malloc(SUPERCHUNK_ALLOCATION(sc_pool));

        if (sc_p) {
	    adr_p = (char *)&sc_p[1];
	    sc_p->superchunk_checkword = SUPER_GUARD_VALUE;
	    sc_p->superchunk_bitmap = -1L ^ 1;
	    *offset = 0;
	    DNS_LOCK {
		sc_p->superchunk_allocno = mem_counter;
		sc_p->link = sc_pool->superchunk_head;
		sc_pool->superchunk_head = sc_p;
	    } DNS_END_LOCK;
        }
    }

   return (adr_p);
}

/* ------------------------------------------------------------------------- */

/*
 * ROUTINE - free_superchunk
 *
 * This routine is called whenever dns_free detects that the memory
 * about to be freed is a member of a superchunk (superchunk_offset != 0).
 *
 * We determine the superchunk header address and mark the element as
 * free.
 *
 * No attempt is made to free up totally unused superchunks.
 */

static void 
free_superchunk (superchunk_pool_t  *sc_pool,
                 mem_list_t         *mem_p)
{
    superchunk_list_t *sc_p;
    int element_size = SUPERCHUNK_ELEMENT(sc_pool);

    /*
     * Calculate size, so we know what list we're in, then calculate
     * an offset to the superchunk header.   Once we're there, make
     * sure we've got a valid header and then mark the element free
     * in the bitmap (xor with offset bitmask).
     */

    int length = mem_p->upper_guard_p - (char *)&mem_p[1];

    if (length != sc_pool->superchunk_size) 
    {
      dce_svc_printf(CDS_S_LIBRARY_CDSMALLOC_A_MSG);
    }

    sc_p = (superchunk_list_t *)( ((char *)mem_p) -
                              (mem_p->superchunk_offset * element_size) -
                              sizeof(superchunk_list_t)
                             );
    if (sc_p->superchunk_checkword != SUPER_GUARD_VALUE) 
    {
      dce_svc_printf(CDS_S_LIBRARY_CDSMALLOC_A_MSG);
    }

    DNS_LOCK 	
    {
	sc_p->superchunk_bitmap ^= (1L << mem_p->superchunk_offset);
    } DNS_END_LOCK;
}


/* ------------------------------------------------------------------------- */


#endif  /* defined(DNS_MALLOC) */

/*
 *         END - INSTRUMENTED MEMORY ALLOCATION SUPPORT ROUTINES
 */



/* ------------------------------------------------------------------------- */

/*
 *                     DIAGNOSTIC SUPPORT ROUTINES
 */

/* ------------------------------------------------------------------------- */

/*
 * FUNCTION: 	show_debug
 * 
 * Task:	Give DEBUG output string
 *
 * Arguments:
 * 	level   - the level of "debug severity" 's' has.
 *	string 	- the string to diplay.
 * 
 * Side Effects: None.
 */
static int
show_debug(char       *string, 
	   void		*level)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        *(unsigned32 *)level,
	"%s",
        *string));

    return(1);
}


/*
 * Used when just wan't to take the default emit routine & arg.
 */
void
dns_diag_vm_totprint(void)
{
    unsigned32 dlevel = svc_c_debug4;
    

    dns_diag_vm_tot(show_debug,&dlevel);
    return;
}


/* ------------------------------------------------------------------------- */

/*
 * Diagnostic dump of totals currently
 *
 * Arguments:
 *	emit_routine	- routine to call to emit output
 *	emit_arg	- argument for emit routine
 */
void 
dns_diag_vm_tot (int       (*emit_routine)(char *, void *),
                 void	*emit_arg)
{
#if defined(DNS_MALLOC)

    char outbuf[ 255];
#   define EMIT (void)(*emit_routine)(outbuf,emit_arg)


    register int all_size    = 0;
    register int all_count   = 0;
    register int perm_size   = 0;
    register int perm_count  = 0;
    register int ignr_size   = 0;
    register int ignr_count  = 0;
    register int unkn_size   = 0;
    register int unkn_count  = 0;
    register int temp_size   = 0;
    register int temp_count  = 0;
    register int err_size    = 0;
    register int err_count   = 0;
    register int in_sc_count = 0;
    register int in_sc_size  = 0;
    register int sc_size_tot = 0;
    register int sc_count_tot = 0;
    mem_list_t *mem_p = (mem_list_t *)&mem_list;
    superchunk_pool_t *sc_pool = LAST_SUPER;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
	" >dns_diag_vm_tot"));

    DNS_LOCK {
	while ((mem_p = mem_p->next_p) != NULL) {
	    all_count++;
	    all_size += mem_p->upper_guard_p - (char *)&mem_p[1];
	    if (mem_p->superchunk_pool) {
		in_sc_count++;
		in_sc_size += mem_p->upper_guard_p - (char *)&mem_p[1];
	    }
	    switch (mem_p->type) {
	    case mem_permanent:
		perm_size += mem_p->upper_guard_p - (char *)&mem_p[1];
		perm_count++;
		break;
	    case mem_temporary:
		temp_size += mem_p->upper_guard_p - (char *)&mem_p[1];
		temp_count++;
		break;
	    case mem_ignored:
		ignr_size += mem_p->upper_guard_p - (char *)&mem_p[1];
		ignr_count++;
		break;
	    case mem_unknown:
		unkn_size += mem_p->upper_guard_p - (char *)&mem_p[1];
		unkn_count++;
		break;
	    default:
		err_size += mem_p->upper_guard_p - (char *)&mem_p[1];
		err_count++;
		break;
	    }
	}
    } DNS_END_LOCK;


    (void)sprintf(outbuf, "Memory clock: %d (mem_counter)", 
		  mem_counter);
    EMIT;

    if (checking_all_memory)
    {
        (void)sprintf(outbuf, "Continuously checking memory integrity.");
    }
    else
    {
        (void)sprintf(outbuf, "Not continuously checking memory integrity.");
    }
    EMIT;

    if (abort_on_full_memory) 	
    {
        (void)sprintf(outbuf, 
		      "Program will abort when memory has been exhausted.");
        EMIT;
    }

    if (superchunk_mode)
    {
        (void)sprintf(outbuf, "Superchunk mode is enabled.");
    }
    else
    {
        (void)sprintf(outbuf, "Superchunk mode is disabled.");
    }
    EMIT;

    (void)sprintf(outbuf, 
		  "Allocations:            Perm      Temp      Ignr      Unkn     Total");
    EMIT;

    (void)sprintf(outbuf, "             Size   %9d %9d %9d %9d %9d",
		  perm_size, 
		  temp_size, 
		  ignr_size, 
		  unkn_size, 
		  all_size);
    EMIT;

    (void)sprintf(outbuf, "             Count  %9d %9d %9d %9d %9d",
		  perm_count, 
		  temp_count, 
		  ignr_count, 
		  unkn_count, 
		  all_count);
    EMIT;

    if (err_size) 
    {
        (void)sprintf(outbuf, 
		      " ** Error blocks detected ** count: %d  size: %d",
		      err_size, 
		      err_count);
        EMIT;
    }

    if (in_sc_count) 
    {
        (void)sprintf(outbuf, "Memory in superchunks: Count: %9d  Size: %9d",
		      in_sc_count, 
		      in_sc_size);
        EMIT;
    }

    for (; sc_pool->superchunk_size; --sc_pool) 
    {
	register int sc_count = 0;

	DNS_LOCK 
	{
	    superchunk_list_t *sc_p;

	    for (sc_p = sc_pool->superchunk_head; sc_p; sc_p = sc_p->link)
		sc_count++;

	} DNS_END_LOCK;
	if (sc_count) 
	{
	    int sc_size = sc_count * SUPERCHUNK_ALLOCATION(sc_pool);

	    (void)sprintf(outbuf,
			  "%s superchunks:        Count: %9d  Size: %9d",
			  sc_pool->superchunk_display, 
			  sc_count, 
			  sc_size);
	    EMIT;

	    sc_count_tot += sc_count;
	    sc_size_tot += sc_size;
	}
    }

    if (sc_size_tot) 
    {
        (void)sprintf(outbuf, "Superchunk total:      Count: %9d  Size: %9d",
		      sc_count_tot, 	
		      sc_size_tot);
        EMIT;
    }

#else

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
	" >dns_diag_vm_tot"));

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
        " <dns_diag_vm_tot: No instrumentation - statistics unavailable"));

#endif /* not DNS_MALLOC */

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
	" <dns_diag_vm_tot"));

    return;
}


/* ------------------------------------------------------------------------- */

/*
 * Used when just wan't to take the default emit routine & arg.
 */
void
dns_diag_vm_findprint(adr)
	int adr;
{
    unsigned32 dlevel = svc_c_debug4;
    

    dns_diag_vm_find(adr,show_debug,&dlevel);
    return;
}

/* ------------------------------------------------------------------------- */

/*
 * Diagnostic search/dump for particular address.
 *
 * Arguments:
 *	adr		- address to find what memory section it belongs to
 *	emit_routine	- routine to call to emit output
 *	emit_arg	- argument for emit routine
 */
void 
dns_diag_vm_find (int                 adr,
                  int   (*emit_routine)(char *, void *),
                  void		*emit_arg)
{
    char outbuf[ 255];
#   define EMIT (void)(*emit_routine)(outbuf,emit_arg)

#if defined(DNS_MALLOC)

    mem_list_t *mem_p = (mem_list_t *)&mem_list;
    char type_arr[6];
    int found_adr = 0;


    DCE_SVC_DEBUG((
	cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
	" >dns_diag_vm_find"));

    DNS_LOCK 
    {
	while ((mem_p = mem_p->next_p) != NULL)
	{
	    if (((char *)adr < (char *)mem_p) ||
		((char *)adr > mem_p->upper_guard_p))
		continue;
	    found_adr = 1;
	    if (mem_p->type == mem_permanent)
		(void)sprintf(type_arr,"Perm");
	    else 
		if (mem_p->type == mem_temporary)
		    (void)sprintf(type_arr,"Temp");
		else 
		    if (mem_p->type == mem_ignored)
			(void)sprintf(type_arr,"Ignr");
		    else 
			if (mem_p->type == mem_unknown)
			    (void)sprintf(type_arr,"Unkn");
			else
			    (void)sprintf(type_arr,"?ERR?");
	    

	    (void)sprintf(outbuf,
			  "%5d Alloc %5d from 0x%7x, type %s from %5d, %s",
			  mem_p->allocno,
			  mem_p->upper_guard_p - (char *)&mem_p[1],
			  mem_p,
			  type_arr,
			  mem_p->lineno,
			  mem_p->filename_p);
	    EMIT;

	    }
    } DNS_END_LOCK;

    if (!found_adr) 
    {
	(void)sprintf(outbuf,
		      "Found no memory block which contains address 0x%x",
		      adr);
	EMIT;
    }

#else
 
    DCE_SVC_DEBUG((
	cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
	" >dns_diag_vm_find"));

    (void)sprintf(outbuf, "No instrumentation - statistics unavailable.");

    EMIT;

#endif /* if defined(DNS_MALLOC) */

    DCE_SVC_DEBUG((
	cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
	" <dns_diag_vm_find"));

    return;
}


/*
 * Diagnostic dump of a particular type
 */
void 
dns_diag_vm_dump (mem_type_t type)
{

#if defined(DNS_MALLOC)

    mem_list_t *mem_p = (mem_list_t *)&mem_list;
    int no_data = 1;


    DCE_SVC_DEBUG((
	cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
        " >dns_diag_vm_dump"));

    DCE_SVC_DEBUG((
	cds__svc_handle,
        cds_svc_library,
        svc_c_debug4,
        " dns_diag_vm_dump: At %d, writing all allocations of specified type.",
	mem_counter));

    switch (type)
    {
	case mem_permanent:
             DCE_SVC_DEBUG((
	         cds__svc_handle,
                 cds_svc_library,
                 svc_c_debug4,
                 " dns_diag_vm_dump: listing current permanent memory allocations."));
	    break;

	case mem_temporary:
	    DCE_SVC_DEBUG((
	         cds__svc_handle,
                 cds_svc_library,
                 svc_c_debug4,
                 " dns_diag_vm_dump: listing current temporary memory allocations."));
	    break;

	case mem_unknown:
	    DCE_SVC_DEBUG((
	         cds__svc_handle,
                 cds_svc_library,
                 svc_c_debug4,
                 " dns_diag_vm_dump: listing current unknown memory allocations."));
	    break;

	case mem_ignored:
	    DCE_SVC_DEBUG((
	         cds__svc_handle,
                 cds_svc_library,
                 svc_c_debug4,
                 " dns_diag_vm_dump: listing current ignored memory allocations."));
	    break;

	default:
	    DCE_SVC_DEBUG((
	         cds__svc_handle,
                 cds_svc_library,
                 svc_c_debug4,
                 " dns_diag_vm_dump: listing current ?ERROR? memory allocations."));
	    break;
    }

    DNS_LOCK 
    {
	while ((mem_p = mem_p->next_p) != NULL)
	{
	    if (mem_p->type != type)
            {
		continue;
            }

	    DCE_SVC_DEBUG((
	        cds__svc_handle,
                cds_svc_library,
                svc_c_debug4,
                " dns_diag_vm_dump: %6d Alloc %5d 0x%7x at %5d, %s",
  		mem_p->allocno,
		mem_p->upper_guard_p - (char *)&mem_p[1],
		mem_p,
		mem_p->lineno,
		mem_p->filename_p));

	    no_data = 0;
	}
    } DNS_END_LOCK;

    if (no_data)
    {
        DCE_SVC_DEBUG((
	    cds__svc_handle,
            cds_svc_library,
            svc_c_debug4,
            " dns_diag_vm_dump: Found no data."));
    }

#endif /* if defined(DNS_MALLOC) */

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
	" <dns_diag_vm_dump"));
	
   return;

}


/* ------------------------------------------------------------------------- */

/*
 * Diagnostic dump of last number of mallocs
 */
void 
dns_diag_vm_lastmallocs (void)
{

#if defined(DNS_MALLOC)

    int ndx;
    char type_arr[ 6];


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
	" >dns_diag_vm_lastmallocs"));
    
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug4,
	"dns_diag_vm_lastmallocs: dumping data."));

    DNS_LOCK 
    {
	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_library,
            svc_c_debug4,
            "dns_diag_vm_lastmallocs: At %d, writing DECdns Server Database up to last %d allocations",
	    mem_counter,
	    LASTNMALLOCS));

	if (mem_counter > LASTNMALLOCS)
	{
	    ndx = (lastndx+1) % LASTNMALLOCS;
	}
	else
	{
	    ndx = 0;
	}
	
	for ( ; ndx != lastndx; ndx = ++ndx % LASTNMALLOCS)
	    {
	    mem_save_list *last_p = &lastmallocs[ndx];

	    if (last_p->type == mem_permanent)
		(void)sprintf(type_arr,"Perm");
	    else 
		if (last_p->type == mem_temporary)
		    (void)sprintf(type_arr,"Temp");
		else 
		    if (last_p->type == mem_ignored)
			(void)sprintf(type_arr,"Ignr");
		    else 
			if (last_p->type == mem_unknown)
			    (void)sprintf(type_arr,"Unkn");
			else
			    (void)sprintf(type_arr,"?ERR?");

	    if (last_p->freeno)
	    {
		DCE_SVC_DEBUG((
                    cds__svc_handle,
                    cds_svc_library,
                    svc_c_debug4,
                    "dns_diag_vm_lastmallocs: %6d Alloc %5d %s at %5d, %s; %6d Freed",
		    last_p->allocno,
		    last_p->length,
		    type_arr,
		    last_p->lineno,
		    last_p->filename_p,
		    last_p->freeno));
	    }
	    else
	    {
		DCE_SVC_DEBUG((
                    cds__svc_handle,
                    cds_svc_library,
                    svc_c_debug4,
                    "dns_diag_vm_lastmallocs: %6d Alloc %5d %s at %5d, %s",
		    last_p->allocno,
		    last_p->length,
		    type_arr,
		    last_p->lineno,
		    last_p->filename_p));
	    }
	}
    } 
    DNS_END_LOCK;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug4,
        "dns_diag_vm_lastmallocs: End listing last %d allocations.", 
	LASTNMALLOCS));

#endif /* if defined(DNS_MALLOC) */

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
        " <dns_diag_vm_lastmallocs"));

    return;

}


/* ------------------------------------------------------------------------- */

/*
 * Diagnostic dump of memory data
 */
void 
dns_diag_vm_dumpall (void)
{

#if defined(DNS_MALLOC)

    mem_list_t *mem_p = (mem_list_t *)&mem_list;
    char type_arr[ 6];


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
	" >dns_diag_vm_dumpall"));
    

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug4,
	"dns_diag_vm_dumpall: dumping data."));

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug4,
	"dns_diag_vm_dumpall: At %d, writing all DECdns Server current allocations.",
	mem_counter));

    DNS_LOCK 
    {
	while ((mem_p = mem_p->next_p) != NULL) 
	{
	    if (mem_p->type == mem_permanent)
		(void)sprintf(type_arr,"Perm");
	    else 
		if (mem_p->type == mem_temporary)
		    (void)sprintf(type_arr,"Temp");
		else 	
		    if (mem_p->type == mem_ignored)
			(void)sprintf(type_arr,"Ignr");
		    else 
			if (mem_p->type == mem_unknown)
			    (void)sprintf(type_arr,"Unkn");
			else
			    (void)sprintf(type_arr,"?ERR?");


	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_library,
                svc_c_debug4,
	        "dns_diag_vm_dumpall: %6d Alloc %5d %s 0x%7x at %5d, %s",
                mem_p->allocno,
		mem_p->upper_guard_p - (char *)&mem_p[1],
		type_arr,mem_p,
		mem_p->lineno,
		mem_p->filename_p));
	}
    } DNS_END_LOCK;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug4,
	"dns_diag_vm_dumpall: Finished listing Server allocations."));

#endif /* if defined(DNS_MALLOC) */
		  
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
	" <dns_diag_vm_dumpall"));

    return;

}

/* ------------------------------------------------------------------------- */

int 
dns_diag_set_memory_check (int new_setting)
{
    int prev_setting;

    prev_setting = checking_all_memory;
    checking_all_memory = new_setting;
    return (prev_setting);
}



/* ------------------------------------------------------------------------- */

int 
dns_diag_set_abort_on_full_mem (int new_setting)
{
    int prev_setting;

    prev_setting = abort_on_full_memory;
    abort_on_full_memory = new_setting;
    return (prev_setting);
}



/* ------------------------------------------------------------------------- */

int 
dns_diag_set_superchunk_mode (int new_setting)
{
    int prev_setting;

    prev_setting = superchunk_mode;
    superchunk_mode = new_setting;
    return (prev_setting);
}




/* ------------------------------------------------------------------------- */

/*
 *                        BUFFER FARM MANAGEMENT
 */

/*
 * ROUTINE - dns_get_buffer
 *
 * This routine allocates buffers from the "buffer farm" maintained by this
 * module.  The buffer farm is a set of buffer pools that are indexed by
 * buffer type.  Buffers within a pool are assumed to have similar sizes
 * and are expected to be freed soon after use.  The dns_get_buffer routine
 * will examine the buffer pool (essentially a lookaside list) for an
 * available buffer within the size constraints and return it to the caller.
 * If there isn't an available buffer in the pool, a new one is allocated
 * with the specified size and it is added to the pool.
 *
 * Once a buffer is added to a pool, it does not leave (it is not returned
 * to the general virtual memory pool).  Instead it is marked as available.
 *
 * The lookaside list is searched on a first-fit basis.  This works well
 * when the general assumptions of the buffer farm (short lifetime, similar
 * in size to others in the same pool).
 *
 * Returns: pointer to buffer of specified length.
 */
void *
dns_get_buffer (dns_buffer_type_t  buffer_type,
                int                buffer_size)
{
    char             *adr_p;
    typed_mem_list_t *work_p = buffer_farm[(int) buffer_type];

    DNS_INIT;
    /*
     * Find first available buffer -- leave it in work_p
     */
    DNS_LOCK {
	for (; work_p; work_p = work_p->link)
	    if ((work_p->available == 1) && (work_p->buffer_size >= buffer_size)) {
		work_p->available = 0;
		break;
	    }
    } DNS_END_LOCK;

    /*
     * If work_p is null, then no buffers in the pool satisfy the request
     * so we have to allocate a new one and link it into the pool.
     */
    if (work_p == NULL) {
        adr_p = (char *)dns_malloc(sizeof(typed_mem_list_t) + buffer_size);
        if (adr_p) {
            work_p = (typed_mem_list_t *)adr_p;
            work_p->buffer_size = buffer_size;
            work_p->available = 0;
            work_p->reuse_count = 0;
	    work_p->pool_check_word = POOL_CHECK_VALUE;
	    DNS_LOCK {
		work_p->link = buffer_farm[(int) buffer_type];
		buffer_farm[(int) buffer_type] = (typed_mem_list_t *)work_p;
	    } DNS_END_LOCK;
            adr_p = (char *)&work_p[1];
        }
    } else {
        work_p->reuse_count++;
	adr_p = (char *)&work_p[1];
    }

    return(adr_p);

}


/* ------------------------------------------------------------------------- */

/*
 * ROUTINE - dns_free_buffer
 *
 * This routine "frees" a buffer that was previously allocated with the
 * dns_get_buffer routine.  The buffer is simply marked as available to the
 * get routine.
 */
void 
dns_free_buffer (void *adr_p)
{
    typed_mem_list_t *work_p = &((typed_mem_list_t *)adr_p)[-1];

    /*
     * Speed is of the essence.  We simply check that the address reference
     * is likely to be something allocated by dns_get_buffer, and then we
     * mark it as available.
     */
    if ((work_p->pool_check_word == POOL_CHECK_VALUE) &&
        (work_p->available == 0)) {
        work_p->available = 1;
        return;
    }

    dce_svc_printf(CDS_S_LIBRARY_CDSMALLOC_A_MSG);
}
