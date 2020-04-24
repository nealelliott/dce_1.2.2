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
 * $Log: prof_parse.c,v $
 * Revision 1.1.2.1  1996/06/05  21:25:00  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  20:14:27  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>

#include "prof_int.h"

#define SECTION_SEP_CHAR '/'

#define STATE_INIT_COMMENT	1
#define STATE_STD_LINE		2
#define STATE_GET_OBRACE	3

struct parse_state {
	int	state;
	int	group_level;
	struct profile_node *root_section;
	struct profile_node *current_section;
};

static char *skip_over_blanks(cp)
	char	*cp;
{
	while (*cp && isspace(*cp))
		cp++;
	return cp;
}

void strip_line(line)
	char	*line;
{
	char	*p;

	while (*line) {
		p = line + strlen(line) - 1;
		if ((*p == '\n') || (*p == '\r'))
			*p = 0;
		else
			break;
	}
}

static errcode_t parse_init_state(state)
	struct parse_state *state;
{
	state->state = STATE_INIT_COMMENT;
	state->group_level = 0;

	return profile_create_node("(root)", 0, &state->root_section);
}

static errcode_t parse_std_line(line, state)
	char	*line;
	struct parse_state *state;
{
	char	*cp, ch, *tag, *value;
	char	*p;
	errcode_t retval;
	int do_subsection = 0;
	void *iter = 0;
	
	if (*line == 0)
		return 0;
	if (line[0] == ';')
		return 0;
	strip_line(line);
	cp = skip_over_blanks(line);
	ch = *cp;
	if (ch == 0)
		return 0;
	if (ch == '[') {
		if (state->group_level > 0)
			return PROF_SECTION_NOTOP;
		cp++;
		p = strchr(cp, ']');
		if (p == NULL)
			return PROF_SECTION_SYNTAX;
		*p = '\0';
		retval = profile_find_node_subsection(state->root_section,
						 cp, &iter, 0,
						 &state->current_section);
		if (retval == PROF_NO_SECTION) {
			retval = profile_add_node(state->root_section,
						  cp, 0,
						  &state->current_section);
			if (retval)
				return retval;
		} else if (retval)
			return retval;

		/*
		 * Finish off the rest of the line.
		 */
		cp = p+1;
		if (*cp)
			return PROF_SECTION_SYNTAX;
		return 0;
	}
	if (ch == '}') {
		if (state->group_level == 0)
			return PROF_EXTRA_CBRACE;
		retval = profile_get_node_parent(state->current_section,
						 &state->current_section);
		if (retval)
			return retval;
		state->group_level--;
		return 0;
	}
	/*
	 * Parse the relations
	 */
	p = strchr(cp, ' ');
	if (!p)
		return PROF_RELATION_SYNTAX;
	*p = '\0';
	tag = cp;
	cp = skip_over_blanks(p+1);
	if (*cp != '=')
		return PROF_RELATION_SYNTAX;
	cp = skip_over_blanks(cp+1);
	value = cp;
	if (value[0] == 0) {
		do_subsection++;
		state->state = STATE_GET_OBRACE;
	}
	if (value[0] == '{' && value[1] == 0) 
		do_subsection++;
	if (do_subsection) {
		retval = profile_add_node(state->current_section,
					  tag, 0, &state->current_section);
		if (retval)
			return retval;

		state->group_level++;
		return 0;
	}
	profile_add_node(state->current_section, tag, value, 0);
	return 0;
}

errcode_t parse_line(line, state)
	char	*line;
	struct parse_state *state;
{
	char	*cp;
	
	switch (state->state) {
	case STATE_INIT_COMMENT:
		if (line[0] != '[')
			return 0;
		state->state = STATE_STD_LINE;
	case STATE_STD_LINE:
		return parse_std_line(line, state);
	case STATE_GET_OBRACE:
		cp = skip_over_blanks(line);
		if (*cp != '{')
			return PROF_MISSING_OBRACE;
		state->state = STATE_STD_LINE;
	}
	return 0;
}

errcode_t profile_parse_file(f, root)
	FILE	*f;
	struct profile_node **root;
{
#define BUF_SIZE	2048
	char *bptr;
	errcode_t retval;
	struct parse_state state;

	bptr = malloc (BUF_SIZE);
	if (!bptr)
		return ENOMEM;

	retval = parse_init_state(&state);
	if (retval) {
		free (bptr);
		return retval;
	}
	while (!feof(f)) {
		if (fgets(bptr, BUF_SIZE, f) == NULL)
			break;
		retval = parse_line(bptr, &state);
		if (retval) {
			free (bptr);
			return retval;
		}
	}
	*root = state.root_section;

	free (bptr);
	return 0;
}

#ifndef _WINDOWS

void dump_profile(root, level)
	struct profile_node *root;
	int level;
{
	int i;
	struct profile_node *p;
	void *iter;
	long retval;
	char *name, *value;
	
	iter = 0;
	do {
		retval = profile_find_node_relation(root, 0, &iter,
						    &name, &value);
		if (retval)
			break;
		for (i=0; i < level; i++)
			printf("   ");
		printf("%s = '%s'\n", name, value);
	} while (iter != 0);

	iter = 0;
	do {
		retval = profile_find_node_subsection(root, 0, &iter,
						      &name, &p);
		if (retval)
			break;
		for (i=0; i < level; i++)
			printf("   ");
		printf("[%s]\n", name);
		dump_profile(p, level+1);
	} while (iter != 0);
}
#endif /* ! _WINDOWS */
