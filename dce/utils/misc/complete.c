/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: complete.c,v $
 * Revision 1.1.4.2  1996/02/18  19:21:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:06:49  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:37:04  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:08:55  root]
 * 
 * Revision 1.1.2.4  1994/08/28  13:21:07  rsalz
 * 	Add tilde expansion to filename completion (OT CR 11790).
 * 	[1994/08/28  13:16:19  rsalz]
 * 
 * Revision 1.1.2.3  1994/07/27  17:12:50  rsalz
 * 	Use dce_strdup (OT CR 11341) and do code-cleanup.
 * 	[1994/07/27  17:03:03  rsalz]
 * 
 * Revision 1.1.2.2  1994/03/14  20:31:31  rsalz
 * 	Add rl_strdup wrapper function (OT CR 9885).
 * 	[1994/03/14  20:23:55  rsalz]
 * 
 * Revision 1.1.2.1  1994/01/25  22:53:13  bowe
 * 	Initial checkin - moved here from ./dce/utils
 * 	[1994/01/25  22:38:48  bowe]
 * 
 * Revision 1.1.2.3  1993/12/10  00:04:08  melman
 * 	Made editline.h a private editlinep.h and added a public editline.h
 * 	[1993/12/10  00:03:51  melman]
 * 
 * Revision 1.1.2.2  1993/07/14  19:08:58  melman
 * 	Initial version
 * 	[1993/07/14  18:49:24  melman]
 * 
 * $EndLog$
 */
/*  $Revision: 1.1.4.2 $
**
**  History and file completion functions for editline library.
*/
#include <editlinep.h>
#if	defined(EXPAND_TILDE)
#include <pwd.h>
#endif	/* defined(EXPAND_TILDE) */

/*
**  strcmp-like sorting predicate for qsort.
*/
static int
compare(
    const void	*p1,
    const void	*p2
)
{
    const char	**v1;
    const char	**v2;

    v1 = (const char **)p1;
    v2 = (const char **)p2;
    return strcmp(*v1, *v2);
}

/*
**  Fill in *avp with an array of names that match file, up to its length.
**  Ignore . and .. .
*/
static int
FindMatches(
    char	*dir,
    char	*file,
    char	***avp
)
{
    char	**av;
    char	**new;
    char	*p;
    DIR		*dp;
    DIRENTRY	*ep;
    SIZE_T	ac;
    SIZE_T	len;

    if ((dp = opendir(dir)) == NULL)
	return 0;

    av = NULL;
    ac = 0;
    len = strlen(file);
    while ((ep = readdir(dp)) != NULL) {
	p = ep->d_name;
	if (p[0] == '.' && (p[1] == '\0' || (p[1] == '.' && p[2] == '\0')))
	    continue;
	if (len && strncmp(p, file, len) != 0)
	    continue;

	if ((ac % MEM_INC) == 0) {
	    if ((new = NEW(char*, ac + MEM_INC)) == NULL)
		break;
	    if (ac) {
		COPYFROMTO(new, av, ac * sizeof (char **));
		DISPOSE(av);
	    }
	    *avp = av = new;
	}

	if ((av[ac] = dce_strdup(p)) == NULL) {
	    if (ac == 0)
		DISPOSE(av);
	    break;
	}
	ac++;
    }

    /* Clean up and return. */
    (void)closedir(dp);
    if (ac)
	qsort(av, ac, sizeof (char **), compare);
    return ac;
}

#if	defined(EXPAND_TILDE)
static char *
tilde(
    char		*head
)
{
    char		*tail;
    char		*top;
    char		*new;
    struct passwd	*pw;

    /* Split "~.../xxx" into and parse first part. */
    if ((tail = strchr(head, '/')) != NULL)
	*tail++ = '\0';
    if (head[1] == '\0')
	top = getenv("HOME");
    else {
	pw = getpwnam(&head[1]);
	top = pw ? pw->pw_dir : NULL;
    }
    if (top == NULL)
	return NULL;

    /* Glue new front part with old tail. */
    if (tail == NULL)
	return dce_strdup(top);
    if ((new = malloc(strlen(top) + 1 + strlen(tail) + 1)) == NULL)
	return NULL;
    sprintf(new, "%s/%s", top, tail);
    return new;
}
#endif	/* defined(EXPAND_TILDE) */

/*
**  Split a pathname into allocated directory and trailing filename parts.
*/
static int
SplitPath(
    char	*path,
    char	**dirpart,
    char	**filepart
)
{
    static char	DOT[] = ".";
    char	*dpart;
    char	*fpart;

    if ((fpart = strrchr(path, '/')) == NULL) {
	if ((dpart = dce_strdup(DOT)) == NULL)
	    return -1;
	if ((fpart = dce_strdup(path)) == NULL) {
	    DISPOSE(dpart);
	    return -1;
	}
    }
    else {
	if ((dpart = dce_strdup(path)) == NULL)
	    return -1;
	dpart[fpart - path + 1] = '\0';
	if ((fpart = dce_strdup(++fpart)) == NULL) {
	    DISPOSE(dpart);
	    return -1;
	}
    }
#if	defined(EXPAND_TILDE)
    if (*dpart == '~') {
	*dirpart = tilde(dpart);
	DISPOSE(dpart);
	if (*dirpart == NULL) {
	    DISPOSE(fpart);
	    return -1;
	}
    }
    else
	*dirpart = dpart;
#else
    *dirpart = dpart;
#endif	/* defined(EXPAND_TILDE) */
    *filepart = fpart;
    return 0;
}

/*
**  Attempt to complete the pathname, returning an allocated copy.
**  Fill in *unique if we completed it, or set it to 0 if ambiguous.
*/
char *
rl_complete(
    char	*pathname,
    int		*unique
)
{
    char	**av;
    char	*dir;
    char	*file;
    char	*new;
    char	*p;
    SIZE_T	ac;
    SIZE_T	end;
    SIZE_T	i;
    SIZE_T	j;
    SIZE_T	len;

    if (SplitPath(pathname, &dir, &file) < 0)
	return NULL;
    if ((ac = FindMatches(dir, file, &av)) == 0) {
	DISPOSE(dir);
	DISPOSE(file);
	return NULL;
    }

    p = NULL;
    len = strlen(file);
    if (ac == 1) {
	/* Exactly one match -- finish it off. */
	*unique = 1;
	j = strlen(av[0]) - len + 2;
	if ((p = NEW(char, j + 1)) != NULL) {
	    COPYFROMTO(p, av[0] + len, j);
	    if ((new = NEW(char, strlen(dir) + strlen(av[0]) + 2)) != NULL) {
		(void)strcpy(new, dir);
		(void)strcat(new, "/");
		(void)strcat(new, av[0]);
		rl_add_slash(new, p);
		DISPOSE(new);
	    }
	}
    }
    else {
	*unique = 0;
	if (len) {
	    /* Find largest matching substring. */
	    for (i = len, end = strlen(av[0]); i < end; i++)
		for (j = 1; j < ac; j++)
		    if (av[0][i] != av[j][i])
			goto breakout;
  breakout:
	    if (i > len) {
		j = i - len + 1;
		if ((p = NEW(char, j)) != NULL) {
		    COPYFROMTO(p, av[0] + len, j);
		    p[j - 1] = '\0';
		}
	    }
	}
    }

    /* Clean up and return. */
    DISPOSE(dir);
    DISPOSE(file);
    for (i = 0; i < ac; i++)
	DISPOSE(av[i]);
    DISPOSE(av);
    return p;
}

/*
**  Return all possible completions.
*/
int
rl_list_possib(
    char	*pathname,
    char	***avp
)
{
    char	*dir;
    char	*file;
    int		ac;

    if (SplitPath(pathname, &dir, &file) < 0)
	return 0;
    ac = FindMatches(dir, file, avp);
    DISPOSE(dir);
    DISPOSE(file);
    return ac;
}
