/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: environment.c,v $
 * Revision 1.1.6.2  1996/02/18  00:22:23  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:10:03  marty]
 *
 * Revision 1.1.6.1  1995/12/08  18:01:28  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1994/12/05  19:53 UTC  mullan_s
 * 
 * 	HP revision /main/mullan_mothra_bugfix/1  1994/11/29  16:04 UTC  mullan_s
 * 	Remove definition of getenv.
 * 
 * 	HP revision /main/HPDCE02/1  1994/11/15  21:23 UTC  mullan_s
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE01/1  1994/01/31  19:56  mdf
 * 	Fixed scope declaration, to remove warning.
 * 
 * 	HP revision /main/mdf_security_merge_bl0/1  1994/01/31  19:51  mdf
 * 	Added scope declaration for _findenv.
 * 	[1995/12/08  17:22:47  root]
 * 
 * Revision 1.1.4.2  1992/12/29  16:40:07  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:30:12  zeliff]
 * 
 * Revision 1.1.2.2  1992/06/30  21:54:20  burati
 * 	Initial version
 * 	[1992/06/30  18:13:54  burati]
 * 
 * $EndLog$
 */
#if !defined(lint) && !defined(_NOIDENT)
static char rcsid[] = "@(#)$RCSfile: environment.c,v $ $Revision: 1.1.6.2 $ (OSF) $Date: 1996/02/18 00:22:23 $";
#endif
/*
 * Copyright (c) 1987 Regents of the University of California.
 * This file may be freely redistributed provided that this
 * notice remains attached.
 *
 ***  "getenv.c	5.4 (Berkeley) 3/13/87";
 ***  "setenv.c	1.2 (Berkeley) 3/13/87";
 ***/

#include <sys/types.h>
#include <stdio.h>

/*
 * setenv(name,value,rewrite)
 *	Set the value of the environmental variable "name" to be
 *	"value".  If rewrite is set, replace any current value.
 */
setenv(name,value,rewrite)
	register char	*name,
			*value;
	int	rewrite;
{
	extern char	**environ;
	static int	alloced;		/* if allocated space before */
	register char	*C;
	int	l_value,
		offset;
	char	*malloc(), *realloc(), *_findenv();

	if (*value == '=')			/* no `=' in value */
		++value;
	l_value = strlen(value);
	if ((C = _findenv(name,&offset))) {	/* find if already exists */
		if (!rewrite)
			return(0);
		if (strlen(C) >= l_value) {	/* old larger; copy over */
			while (*C++ = *value++);
			return(0);
		}
	}
	else {					/* create new slot */
		register int	cnt;
		register char	**P;

		for (P = environ,cnt = 0;*P;++P,++cnt);
		if (alloced) {			/* just increase size */
			environ = (char **)realloc((char *)environ,
			    (u_int)(sizeof(char *) * (cnt + 2)));
			if (!environ)
				return(-1);
		}
		else {				/* get new space */
			alloced = 1;		/* copy old entries into it */
			P = (char **)malloc((u_int)(sizeof(char *) *
			    (cnt + 2)));
			if (!P)
				return(-1);
			bcopy(environ,P,cnt * sizeof(char *));
			environ = P;
		}
		environ[cnt + 1] = NULL;
		offset = cnt;
	}
	for (C = name;*C && *C != '=';++C);	/* no `=' in name */
	if (!(environ[offset] =			/* name + `=' + value */
	    malloc((u_int)((int)(C - name) + l_value + 2))))
		return(-1);
	for (C = environ[offset];(*C = *name++) && *C != '=';++C);
	for (*C++ = '=';*C++ = *value++;);
	return(0);
}

/*
 * unsetenv(name) --
 *	Delete environmental variable "name".
 */
void
unsetenv(name)
	char	*name;
{
	extern	char	**environ;
	register char	**P;
	int	offset;
	char	*_findenv();

	while (_findenv(name,&offset))		/* if set multiple times */
		for (P = &environ[offset];;++P)
			if (!(*P = *(P + 1)))
				break;
}

/*
 * _findenv(name,offset) --
 *	Returns pointer to value associated with name, if any, else NULL.
 *	Sets offset to be the offset of the name/value combination in the
 *	environmental array, for use by setenv(3) and unsetenv(3).
 *	Explicitly removes '=' in argument name.
 *
 *	This routine *should* be a static; don't use it.
 */
char *
_findenv(name,offset)
	register char *name;
	int	*offset;
{
	extern char	**environ;
	register int	len;
	register char	**P,
			*C;

	for (C = name,len = 0;*C && *C != '=';++C,++len);
	for (P = environ;*P;++P)
		if (!strncmp(*P,name,len))
			if (*(C = *P + len) == '=') {
				*offset = P - environ;
				return(++C);
			}
	return(NULL);
}
