/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: gtxkeymap.h,v $
 * Revision 1.1.9.1  1996/10/02  17:51:05  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:40:00  damon]
 *
 * Revision 1.1.4.1  1994/06/09  14:09:59  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:24:35  annie]
 * 
 * Revision 1.1.2.2  1993/01/19  16:02:58  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  14:12:48  cjd]
 * 
 * Revision 1.1  1992/01/19  02:41:19  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#ifndef __GATOR_KEYMAP_H_
#define __GATOR_KEYMAP_H_

/* number of entries max per keymap */
#define KEYMAP_NENTRIES		256

/* types of keymaps */
#define KEYMAP_EMPTY		0		/* nothing in this slot */
#define KEYMAP_PROC		1		/* procedure in this slot */
#define KEYMAP_SUBMAP		2		/* submap in this slot */

/* one per entry */
struct keymap_entry {
    char type;				/* type, e.g. submap, etc */
    char pad[3];			/* padding */
    char *name;				/* descriptive name of function, if function */
    union {				/* value (proc, submap, etc) */
	int (*proc)();
	struct keymap_map *submap;
	char *generic;
    } u;
    char *rock;				/* rock to use */
};

struct keymap_map {
    short refcount;	/* reference count */
    char pad[2];	/* padding to long boundary */
    struct keymap_entry entries[KEYMAP_NENTRIES];
};

struct keymap_state {
    struct keymap_map *initMap;
    struct keymap_map *currentMap;
};

extern struct keymap_map *keymap_Create();
extern int keymap_BindToString();
extern int keymap_Delete();
extern int keymap_InitState();
extern int keymap_ProcessState();
extern int keymap_ResetState();

#endif	/* define for file */
