/*
 * (C) Copyright 1995, 1994 Transarc Corporation
 * All Rights Reserved
 */

#include <dcedfs/stds.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mntutil.h>

RCSID ("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/utils/RCS/mntutil.c,v 1.6 1995/02/14 21:49:12 dlc Exp $")

int get_vfs(char *mnt_arg, struct vfstab *vp, char *fstype)
{
    struct vfstab 	findtab;
    register FILE	*vtf;
    register int	error;

    if ((vtf = fopen(VFSTAB, "r")) == NULL) {
	fprintf(stderr, "Error opening vfstab file\n");
	return(0);
    }

    /*
     * Prepare a vfstab entry for searching against.
     */
    memset(&findtab, 0, sizeof(struct vfstab));
    findtab.vfs_fstype = fstype;

    /*
     * Start the search by assuming the mount argument was a block
     * device name
     */
    findtab.vfs_special = mnt_arg;
    error = getvfsany(vtf, vp, &findtab);
    if (error == 0) {
	fclose(vtf);
	return(0);
    }
    /*
     * That failed, so try assuming the mount argument was char device name.
     */
    rewind(vtf);
    findtab.vfs_special = NULL;
    findtab.vfs_fsckdev = mnt_arg;
    error = getvfsany(vtf, vp, &findtab);
    if (error == 0) {
	fclose(vtf);
	return(0);
    }
    /*
     * That failed, so try searching as if argument was a mount point
     */
    rewind(vtf);
    findtab.vfs_fsckdev = NULL;
    findtab.vfs_mountp = mnt_arg;
    error = getvfsany(vtf, vp, &findtab);
    if (error == 0) {
	fclose(vtf);
	return(0);
    }
    /*
     * persistent failure, so punt.
     */
    fprintf(stderr, "Could not find vfstab entry for %s\n", mnt_arg);
    fclose(vtf);
    return(-1);
}


void split_up(char *inargs, char *outargs)
{
    register char *nxt_arg = inargs;

    while ((nxt_arg = strchr(inargs, ',')) != NULL) {
	*nxt_arg++ = NULL;
	*outargs++ = '-';
	sprintf(outargs, "%s ", inargs);
	outargs += strlen(outargs);
	inargs = nxt_arg;
    }
    sprintf(outargs, "-%s ", inargs);	/* fill in last arg (no , at end) */
}

/*
 * Given a string, instr, with tokens separated by the characters in the
 * set, seps, convert it to vector of string arguments in args.  Space
 * for the arguments is chopped from the provided buffer, strbuf, size
 * given by buflen.  Each argument is prefixed (if not already) with the
 * string in prefix.  The argument vector is terminated with a NULL pointer
 * to a string.  Any arguments in instr corresponding to the empty string
 * are ignored.
 *
 * Return -1 on error including insufficient space in args, bad arguments,
 * etc.  On success, return the number of arguments in the vector.
 */
int opts_to_argv(const char *instr,
		 char *strbuf,
		 int buflen,
		 char **args,
		 const int nargs,
		 const char *seps,
		 const char *prefix)
{
    int plen;
    int ind;
    char *origp, *endp;
    int curlen, usedlen;

    /*
     * Consistency check on the arguments
     */
    if (instr == NULL ||
	strbuf == NULL ||
	args == NULL ||
	nargs < 2 ||
	seps == NULL ||
	*seps == '\0' ||
	prefix == NULL) {
	return(-1);
    }

    /*
     * It is legal for the prefix string to be the empty string
     */
    plen = strlen(prefix);

    origp = strbuf;
    usedlen = 0;

    for (ind = 0; ind < nargs - 1 && *instr != '\0'; ) {
	endp = strpbrk(instr, seps);

	/*
	 * Do not copy any null arguments
	 */

	if (instr == endp) {
	    instr++;
	    continue;
	}

	if (endp != NULL) {
	    curlen = endp - instr;
	} else {
	    curlen = strlen(instr);
	}

	/*
	 * Roll this back if a string (maybe with a prefix) will not
	 * fit.
	 */
	args[ind++] = strbuf;

	/*
	 * Copy any prefix string not already in the argument
	 */
	if (plen > 0 && strncmp(instr, prefix, plen) != 0) {
	    if (usedlen + plen > buflen) {
		ind--;
		break;
	    }
	    (void) strcpy(strbuf, prefix);
	    strbuf += plen;
	    usedlen += plen;
	}

	if (usedlen + curlen + 1 > buflen) {
	    ind--;
	    break;
	}

	if (endp != NULL) {
	    (void)strncpy(strbuf, instr, curlen);
	    /*
	     * Skip the separator
	     */
	    instr += (curlen + 1);
	    strbuf += (curlen);
	    *strbuf++ = '\0';
	} else {
	    (void) strcpy(strbuf, instr);
	    instr += curlen;
	}

	usedlen += (curlen + 1);

    }

    args[ind] = NULL;

    if (*instr != '\0') {
	/*
	 * Indicate that not all of the string was converted to an
	 * argument vector
	 */
	return(-1);
    }

    return(ind);
}

/*
 * Remove all occurences of an argument string, in torm, from the argument
 * vector, args.  Return the number of arguments in the adjusted vector.
 */
int remove_arg(const char *torm, char **args)
{
    int old, new;

    old = 0;
    new = 0;
    while (args[old] != NULL) {
	if (strcmp(torm, args[old]) == 0) {
	    old++;
	    continue;
	}
	args[new++] = args[old++];
    }
    args[new] = NULL;

    return(new);
}
