/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tom.c,v $
 * Revision 1.1.9.1  1996/10/02  18:12:09  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:45:22  damon]
 *
 * Revision 1.1.4.1  1994/07/13  21:31:21  devsrc
 * 	Created this file.  tom (table of memory) is a user
 * 	interface program to extract memory statistics from
 * 	the kernel and disply this information symbolically
 * 	using the kernel symbol table.
 * 	[1994/04/04  15:14:41  mbs]
 * 
 * $EndLog$
 */

/*
 * TOM - table of memory
 */

/*
 * TODO - this program will only work on HPUX.  It makes the following
 * assumptions:
 *
 *     (1) the kernel image file is /opt/dcelocal/ext/ALL.out
 *     (2) kload is the tool used to load kernel extensions and
 *         report on their location and size
 *     (3) the output of kload is in a specific format
 *     (4) the extensions are loaded from /opt/dcelocal/ext, and have
 *          the names dce_kprc, dfs_core, dfs_client, and dfs_server.
 *     (5) the output of nm -x is in a specific format.
 *
 *
 * There are limits imposed by the use of fixed size tables.  (They are
 * fixed size because we don't know how much to malloc before we start
 * loading the table -- but this is probably not too hard to fix.
 */

#define MEM_TYPE_STRINGS       /* Must go first */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/syscall.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_MEM_RECS 5000
#define MAX_ALLOC_RECS 5000
#define SYMTABSIZE 12000    /* size allocated for symtab */
#define PCTABSIZE 1000
#define EXTTABSIZE 5
#define SYMFILE "/opt/dcelocal/ext/ALL.out"
#define NM "/bin/nm -x"     /* For HPUX */

/*
 * An in-memory symbol table is built from SYMFILE.
 */
typedef struct entry {
    caddr_t address;            /* address of symbol */
    char *name;                 /* name of symbol */
} entry_t;

typedef struct symaddr {
    entry_t *entry;	        /* name of immediately preceding function */
    unsigned offset;            /* offset from start of function */
} symaddr_t;

/*
 * Structure to record address, size, and name of an extension.
 */

typedef struct extaddr {
    caddr_t start;
    size_t size;
    char *name;
    int enable;
} extaddr_t;

/*
 * Static tables.
 */
entry_t symtab[SYMTABSIZE];
extaddr_t exttab[EXTTABSIZE];
struct osi_allocrec allocbuf[MAX_ALLOC_RECS];

int symtabsize;                 /* actual size of symtab */
int exttabsize;                 /* actual size of exttab */

/*
 * XXX - using afs_syscall kinda forces you into assuming sizeof(int)
 * is the same as sizeof(char *).  This will need to be fixed for
 * 64 bit integration.
 */
int
dfs_plumber(
    int cmd,
    int *cookie,
    int size,
    int *countp,
    void *bufferp)
{
    return afs_syscall(AFSCALL_PLUMBER, (int)cookie, size, (int)countp,
        (int) bufferp, cmd);
}

/*
 * Initialize the extensions table.
 */
void
get_extensions(void)
{
    FILE *fp;
    int i;
    char *p;
    int ext;
    char name[128];
    caddr_t addr;
    int size;
    int k;

    exttabsize = 0;

    if ((fp = popen("/opt/dcelocal/ext/kload", "r")) == NULL) {
        fprintf(stderr, "popen for kload failed\n");
        exit(1);
    }

    for (i = 0; i < EXTTABSIZE; i++) {
        if ((k = fscanf(fp, "Ext %d: Addr %x Size %x Name /opt/dcelocal/ext/%s\n",
            &ext, &addr, &size, name)) != 4) {
            return;
        }
        fscanf(fp, "\n");	/* skip blank line */

        exttab[i].start = addr;
        exttab[i].size = size;
        exttab[i].enable = 0;
        if ((exttab[i].name = (char *)malloc(strlen(name) + 1)) == NULL) {
            fprintf(stderr, "malloc for extension name failed.\n");
            exit(1);
        }
        strcpy(exttab[i].name, name);
        exttabsize++;
    }
    pclose(fp);
}

void
show_extensions()
{
    int i;

    for (i = 0; i < exttabsize; i++)
        printf("addr=0x%x, size=0x%x, name=%s\n",
            exttab[i].start, exttab[i].size, exttab[i].name);
}

/*
 * Return a pointer to the entry such that
 * entry->address <= addr < (entry + 1)->address.
 *
 * The C library bsearch function can't be used since
 * it will only find an exact match.
 */
entry_t *
lookup_sym(caddr_t addr)
{
    entry_t *lower = &symtab[0];
    entry_t *upper;
    int n = symtabsize;
    int pivot;

    if (addr >= symtab[symtabsize -1].address)
        return &symtab[symtabsize -1];

    if (addr <= symtab[0].address)
        return &symtab[0];

    while (n > 0) {
        pivot = n >> 1;
        upper = lower + pivot;

        if (addr < upper->address) {
            n = pivot;
        } 
        else if (addr >= upper->address && addr < (upper +1)->address) {
            return upper;
        }
        else {
            lower = upper + 1;
            n -= pivot + 1;
       }
    }
    /*NOTREACHED*/
}	 
	 
/*
 * Given an address, find its extension.
 */
extaddr_t *
lookup_ext(caddr_t addr)
{
    extaddr_t *xp;

    for (xp = &exttab[0]; xp < &exttab[exttabsize]; xp++) {
        if (addr >= xp->start && addr < xp->start + xp->size) {
            return xp;
        }
    }
    return NULL;
}

/*
 * Given two pointers to struct osimem records,
 * return
 *
 *     -1, if first less than second
 *      0, if first equals second
 *      1, if first greater than second
 */
int
sort_memrec_by_pc(
    const void *a1,
    const void *a2)
{
    caddr_t c1, c2;

    c1 = ((struct osimem *)a1)->caller;
    c2 = ((struct osimem *)a2)->caller;

    if (c1 < c2) 
        return -1;
    if (c1 == c2)
        return 0;
    return 1;
}


/*
 * Given two pointers to struct osi_allocrec records,
 * return
 *
 *     -1, if first less than second
 *      0, if first equals second
 *      1, if first greater than second
 */
int
sort_allocrec_by_pc(
    const void *a1,
    const void *a2)
{
    caddr_t c1, c2;

    c1 = ((struct osi_allocrec *)a1)->caller;
    c2 = ((struct osi_allocrec *)a2)->caller;

    if (c1 < c2) 
        return -1;
    if (c1 == c2)
        return 0;
    return 1;
}

/*
 * Given two pointers to entry_t records,
 * return
 *
 *     -1, if first less than second
 *      0, if first equals second
 *      1, if first greater than second
 */
int
sort_by_addr(
    const void *a1,
    const void *a2)
{
    caddr_t c1, c2;

    c1 = ((entry_t *)a1)->address;
    c2 = ((entry_t *)a2)->address;

    if (c1 < c2) 
        return -1;
    if (c1 == c2)
        return 0;
    return 1;
}

void
make_symtab(void)
{
    FILE *fp;
    int line = 0;
    char *ap, *sp, *tp;
    entry_t *ep = &symtab[0];
    
    char buf[256];

    if ((fp = popen(NM " " SYMFILE, "r")) == NULL) {
        fprintf(stderr, "popen for nm failed\n");
        exit(1);
    }

    while (line++ < 6)
        fgets(buf, 256, fp);        /* skip header lines */

    while (fgets(buf, 256, fp) != NULL && ep < &symtab[SYMTABSIZE]) {
        sp = strtok(buf, "| \t");     /*  name */
        ap = strtok(NULL, "| \t");    /* value (address) */
        (void) strtok(NULL, "| \t");  /* scope (extern or static) */
        tp = strtok(NULL, "| \t");    /* type (data, code, entry, abs, etc.) */
        if (strcmp(tp, "entry") != 0)
            continue;

         if ((ep->name = (char *)malloc(strlen(sp) + 1)) == NULL) {
            fprintf(stderr, "Malloc of symbol string failed.\n");
            exit(1);
	}
        strcpy(ep->name, sp);
        ep->address = (caddr_t)strtoul(ap, NULL, 0);

        ep++;
    }
    if (ep == &symtab[SYMTABSIZE])
        fprintf(stderr, "WARNING:  symbol table too small.\n");

    pclose(fp);

    symtabsize = ep - &symtab[0];

    /* 
     * Now sort the table so binary search may be used later.
     */
    qsort((void *)&symtab[0], (size_t)symtabsize, (size_t)sizeof(entry_t),
          sort_by_addr);
}

void
show_summary(void)
{
    int ret;
    struct memUsage memUsage;


    if ((ret = dfs_plumber(PLUMBER_GET_MEM_USAGE, 0, 0, NULL, &memUsage)) != 0) {
        printf("dfs_plumber: PLUMBER_GET_MEM_USAGE returned %d\n", ret);
        exit(1);
    }
    
    printf("Global Allocations:\n"
           "\tcurr_bytes: %ld\n"
	   "\thiwat_bytes: %ld\n"
	   "\tcurr_allocs: %ld\n"
	   "\thiwat_allocs: %ld\n"
	   "\talloc_buffers: %ld\n"
           "\tused_buffers: %ld\n"
	   "\talloc_IO_buffers: %ld\n"
	   "\tused_IO_buffers: %ld\n",
            memUsage.mu_curr_bytes,
            memUsage.mu_hiwat_bytes,
            memUsage.mu_curr_allocs,
            memUsage.mu_hiwat_allocs,
            memUsage.mu_alloc_buffers,
            memUsage.mu_used_buffers,
            memUsage.mu_alloc_IO_buffers,
            memUsage.mu_used_IO_buffers);
}

void
show_memusage_by_addr(void)
{
    int cookie = 0;
    int size = 0;
    int recs = 0;
    caddr_t caller = 0;
    int ret;
    int count = MAX_MEM_RECS;
    struct osimem membuf[MAX_MEM_RECS];
    struct osimem *mp;
    extaddr_t *xp;
    extaddr_t *prev_xp = NULL;

    ret = dfs_plumber(PLUMBER_GET_MEM_RECS, &cookie, sizeof(struct osimem),
        &count, &membuf[0]);            

    if (ret) {
        printf("PLUMBER_GET_MEM_RECS failed: ret=%d\n", ret);
        exit(1);
    }

    if (cookie < OSI_MEMHASHSIZE)
        printf("Warning: More than %d current allocations (mem recs).\n", MAX_MEM_RECS);

    /* Sort by PC value */
    qsort((void *)&membuf[0], count, (size_t) sizeof(struct osimem), 
          sort_memrec_by_pc);

    for (mp = &membuf[0]; mp < &membuf[count]; mp++) {
        xp = lookup_ext(mp->caller);
        if (!xp->enable) {
            continue;
        }
        if (xp != prev_xp) {
            if (recs > 0) {
                printf("[%d %ld]\n\n" , recs, size);
                recs = 0;
            }
            prev_xp = xp;
            printf("**** %s ****\n", xp->name);
	}
        if (mp->caller != caller) {
            if (recs > 0) {
                printf("[%d %ld]\n" , recs, size);
                recs = 0;
	    }
            caller = mp->caller;
            size = mp->size;
            printf("%-25s0x%08x ", lookup_sym(caller)->name, caller);
        }
        if (size == mp->size) {
            recs++;
            continue;
	}

        printf("[%d %ld] " , recs, size);
        recs = 1;
        size = mp->size;
    }
    if (recs > 0) {
        printf("[%d %ld]\n" , recs, size);
    }
}

/*
 * The kernel maintains the list in most recently used (MRU) order.
 * If the sort argument is non-zero, the records are sorted by PC
 * value.
 */
void
show_memusage_by_pc(int sort, int type)
{
    int ret;
    int count = MAX_ALLOC_RECS;
    struct osi_allocrec *arp;
    entry_t *ep;
    extaddr_t *xp;
    extaddr_t *prev_xp = NULL;

    ret = dfs_plumber(PLUMBER_GET_ALLOC_RECS, 0, 0, &count, &allocbuf[0]);

    if (sort) {
        /* Sort by PC value */
        qsort((void *)&allocbuf[0], count, (size_t) sizeof(struct osi_allocrec),
        sort_allocrec_by_pc);
    }

    for (arp = &allocbuf[0]; arp < &allocbuf[count]; arp++) {
        xp = lookup_ext(arp->caller);
        if (!xp->enable) {
            continue;
        }
        if (xp != prev_xp) {
            prev_xp = xp;
            printf("\n**** %s ****\n", xp->name);
	}
        ep = lookup_sym(arp->caller);
        if (type) {
            char buf[128];

            sprintf(buf, "%s(%s)", ep->name, M_NAME(arp->type));
            printf("%-40s 0x%08x %8ld "
        	   "[%ld %ld %ld] " 
		   "[%ld %ld %ld]\n",
        	   buf, ep->address, arp->last, 
        	   arp->total, arp->held, arp->bytes_hiwat,
        	   arp->allocs, arp->blocks, arp->allocs_hiwat);
        	
        }
        else {
            printf("%-25s 0x%08x %8ld "
        	   "[%ld %ld %ld] " 
		   "[%ld %ld %ld]\n",
        	   ep->name, ep->address, arp->last,
        	   arp->total, arp->held, arp->bytes_hiwat,
        	   arp->allocs, arp->blocks, arp->allocs_hiwat);
        }
    }
}

void
usage(void)
{
    fprintf(stderr, 
        "tom (Table Of Memory):"
        "\n\t[-h] -- Print help information"
        "\n\t[-s] -- Print summary information"
        "\n\t[-p] -- Print per-program counter allocation info"
        "\n\t[-m] -- Print per-allocation info"
        "\n\t[-x] -- Print the location and size of extensions"
        "\n\t[-o] -- Print the -p output sorted by PC instead of MRU"
        "\n\t[-t] -- Print datatypes with -p output"
        "\n\t[-e] -- Retrict -p and/or -m output to specified extension"
        "\n\t\t  This option may be used multiple times.  Possible extensions are"
        "\n\t\t  dce_krpc, dfs_core, dfs_client, dfs_server, and dfs_episode\n");
}

main(int argc, char *argv[])
{
    int c;
    extern char *optarg;
    extern int optind;
    extaddr_t *xap = NULL;

    int sflag = 0;
    int eflag = 0;
    int xflag = 0;
    int mflag = 0;
    int pflag = 0;
    int oflag = 0;
    int tflag = 0;

    get_extensions();

    while ((c = getopt(argc, argv, "thopmxse:")) != EOF) {
        switch (c) {
        /*
         * summarize memory usage
         */
        case 's':
            sflag++;
            break;

        /*
         * print memory usage for specified extensions
         */
        case 'e':
            for (xap = &exttab[0]; xap < &exttab[exttabsize]; xap++) {
                if (strcmp(optarg, xap->name) == 0)
                    break;
            }
            if (xap == &exttab[exttabsize]) {
                fprintf(stderr, "No such extension.  Known extensions are:\n");
                for (xap = &exttab[0]; xap < &exttab[exttabsize]; xap++) {
                    printf("%s ", xap->name);
                }
                printf("\n");
                exit(1);
            }
            xap->enable = 1;
            eflag++;
            break;

        /*
         * show where extensions are loaded and their sizes
         */
        case 't':
            tflag++;
            break;
        /*
         * show where extensions are loaded and their sizes
         */

        case 'x':
            xflag++;
            break;
        /*
         * show memory usage per allocation address
         */
        case 'm':
            mflag++;
            break;

        case 'o':
            oflag++;
            break;

        /*
         * show memory usage per program counter value
         */
        case 'p':
            pflag++;
            break;

        /*
         * get help message
         */
	case 'h':
            usage();
            exit(0);

        case '?':
            usage();
            exit(1);
            break;
        }
    }

    /*
     * If we didn't select any particular extensions, then enable printing
     * info on all extensions.
     */
    if (!eflag) {
        for (xap = &exttab[0]; xap < &exttab[exttabsize]; xap++) {
            xap->enable = 1;
        }
    }

    /*
     * If no output is asked for, give them at least the
     * summary.
     */
    if (xflag + sflag + mflag + pflag == 0) {
        sflag++;
    }

    if (xflag) {
        show_extensions();
    }

    if (sflag) {
        show_summary();
    }

    if (pflag || mflag) {
        make_symtab();
    }

    if (mflag) {
        show_memusage_by_addr();
    }

    if (pflag) {
        show_memusage_by_pc(oflag, tflag);
    }

    exit(0);
}
