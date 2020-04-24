/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: prof_int.h,v $
 * Revision 1.1.2.1  1996/06/05  21:24:49  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  20:14:12  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * prof-int.h
 */

#include <time.h>
#include "prof_err.h"

#if defined(__STDC__) || defined(_MSDOS)
#define PROTOTYPE(x) x
#else
#define PROTOTYPE(x) ()
#endif

typedef long errcode_t;

/*
 * This is the structure which stores the profile information for a
 * particular configuration file.
 */
struct _prf_file_t {
	errcode_t	magic;
	char	*filename;
	struct profile_node *root;
	time_t	timestamp;
	int	flags;
	struct _prf_file_t *next;
};

typedef struct _prf_file_t *prf_file_t;

/*
 * This structure defines the high-level, user visible profile_t
 * object, which is used as a handle by users who need to query some
 * configuration file(s)
 */
struct _profile_t {
	errcode_t	magic;
	prf_file_t	first_file;
};

typedef struct _profile_t *profile_t;

/*
 * This structure defines the profile_section_t object, which is
 * returned to the user when a section is searched.
 */
struct _profile_section_t {
	errcode_t	magic;
	int		top_lvl:1, top_lvl_search:1;
	char		*name;
	void		*state;
	struct profile_node	*parent, *sect;
	profile_t	profile;
	prf_file_t	file_ptr;
};

typedef struct _profile_section_t *profile_section_t;

extern errcode_t profile_get
	PROTOTYPE((const char *filename, prf_file_t *ret_prof));

extern errcode_t profile_update
	PROTOTYPE((prf_file_t profile));

extern errcode_t profile_parse_file
	PROTOTYPE((FILE *f, struct profile_node **root));

/* prof_tree.c */

extern void profile_free_node
	PROTOTYPE((struct profile_node *relation));

extern errcode_t profile_create_node
	PROTOTYPE((const char *name, const char *value,
		   struct profile_node **ret_node));

extern errcode_t profile_add_node
	PROTOTYPE ((struct profile_node *section,
		    const char *name, const char *value,
		    struct profile_node **ret_node));

extern errcode_t profile_find_node_relation
	PROTOTYPE ((struct profile_node *section,
		    const char *name, void **state,
		    char **ret_name, char **value));

extern errcode_t profile_find_node_subsection
	PROTOTYPE ((struct profile_node *section,
		    const char *name, void **state,
		    char **ret_name, struct profile_node **subsection));
		   
extern errcode_t profile_get_node_parent
	PROTOTYPE ((struct profile_node *section,
		   struct profile_node **parent));
		   
extern errcode_t profile_delete_node_relation
	PROTOTYPE ((struct profile_node *section, const char *name));

/* prof_file.c */

extern errcode_t profile_open_file
	PROTOTYPE ((const char *filename, prf_file_t *ret_prof));

extern errcode_t profile_update_file
	PROTOTYPE ((prf_file_t profile));

extern errcode_t profile_close_file
	PROTOTYPE ((prf_file_t profile));

/* prof_init.c */

errcode_t profile_init
	PROTOTYPE ((const char **filenames, profile_t *ret_profile));

extern void profile_release
	PROTOTYPE ((profile_t profile));



