/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: isclustr.c,v $
 * Revision 1.1.10.2  1996/02/18  19:44:11  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:25:13  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:29:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:54:51  root]
 * 
 * Revision 1.1.8.1  1994/02/22  16:04:07  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:58:06  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  13:01:37  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:05:01  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:50:19  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:15:35  bbelch]
 * 
 * Revision 1.1.2.3  1992/06/01  23:36:56  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:40:34  zeliff]
 * 
 * Revision 1.1.2.2  1992/04/09  20:06:38  jim
 * 	Comment strings after #endif and #else statements.
 * 	[1992/04/09  19:31:28  jim]
 * 
 * Revision 1.1  1992/01/19  15:12:31  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: isclustr.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:44:11 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/***************************************************************************
 *
 *			   INFORMIX SOFTWARE, INC.
 *
 *			      PROPRIETARY DATA
 *
 *	THIS DOCUMENT CONTAINS TRADE SECRET DATA WHICH IS THE PROPERTY OF 
 *	INFORMIX SOFTWARE, INC.  THIS DOCUMENT IS SUBMITTED TO RECIPIENT IN
 *	CONFIDENCE.  INFORMATION CONTAINED HEREIN MAY NOT BE USED, COPIED OR 
 *	DISCLOSED IN WHOLE OR IN PART EXCEPT AS PERMITTED BY WRITTEN AGREEMENT 
 *	SIGNED BY AN OFFICER OF INFORMIX SOFTWARE, INC.
 *
 *	THIS MATERIAL IS ALSO COPYRIGHTED AS AN UNPUBLISHED WORK UNDER
 *	SECTIONS 104 AND 408 OF TITLE 17 OF THE UNITED STATES CODE. 
 *	UNAUTHORIZED USE, COPYING OR OTHER REPRODUCTION IS PROHIBITED BY LAW.
 *
 *
 *  Title:	iscluster.c
 *  Description:
 *		iscluster() - make index into cluster index
 *
 ***************************************************************************
 */


/*
 *	Tuoc Luong - add this module to libisam to handle clustering
 */


#include <isdec.c>

#ifdef PCDOS
char istname[] = "isNNXXXX";
char istname2[] = "issNXXXX";
#else
char istname[] = "isNNNXXXXX";
char istname2[] = "issNNXXXXX";
#endif

char *malloc();


/*
 .prolog
 * iscluster
 *
 * original author: Tuoc Luong
 *
 * last modified by: Tuoc Luong
 *
 * purpose:
 *	make existing index a cluster one
 *
 * function:
 *	1. make new file to have all indexes of old file
 *  	2. read old file according to index 
 *	3. write to new file
 *	4. rename file to original
 *
 * test scripts:
 *	
 * parameters:
 *	int isfd;
 *	struct keydesc *keydesc;
 *
 *	modifies:
 *	<define all parameters modified by the function>
 *	<definition should include expected range of input and output values>
 *
 * globals:
 *	<define all global variables NOT modified by the function>
 *	<definition should include expected range of input and output values>
 *
 *	modifies:
 *	<define all global variables modified by the function>
 *	<definition should include expected range of input and output values>
 * returns:
 *	new isfd if successful
 *	-1 if error, iserrno is set already
 *
 .eprolog
 */

iscluster(isfd, clustkey)
    register int isfd;
    register struct keydesc *clustkey;
{
    register char *buffer;
    char oldname[PATHSIZE];
    char newname[PATHSIZE];
    char tempname[PATHSIZE];
    int sverr;
    int nisfd, recsize, openmode;
    long maxserial;
#ifdef ISPCLOSE
    extern int ismaxfds;
    int sav;
#endif

    openmode = opens[isfd]->o_mode;

    /* file must be opened with exclusive lock and not have
     * audit trail on
     */
    if (!(openmode & ISEXCLLOCK))
	{
	iserrno = ENOTEXCL;
	return -1;
	}
    if ((isaudit(isfd, tempname, AUDGETNAME)) == 0)
	{
	iserrno = EAUDIT;
	return -1;
	}
    if (iserrno != EBADARG)
	return -1;

    /* create 2 temp file names: one for storing the clustered data
     * the other for renaming the original file so that the original
     * data is always available until the new file is ready
     */
    stcopy(opens[isfd]->o_filep->f_fname, oldname);
    if (ismkname(oldname, newname, tempname))
	return -1;

#ifdef ISXACTION
    /* no logging is needed while creating the new file since
     * rollforward will do the same thing
     */
     issusplog();
#endif

    /* build new file for clustered data and copy over all index
     * descriptions
     */
    if ((recsize = isbnew(isfd, clustkey, &nisfd, newname)) == -1)
	{
	sverr = iserrno;
	goto bad1;
	}

    if ((buffer = malloc(recsize)) == NULL)	 /* allocate space for record */
	{
	sverr = EBADMEM;
	goto bad3;
	}

    /* copy over data according to cluster index
     */
    if (isstart(isfd, clustkey, 0, buffer, ISFIRST))
	{					/* bad cluster key? */
	sverr = iserrno;
	goto bad4;
	}
    for ( ; ; )
	{
	if (isread(isfd, buffer, ISNEXT))
	    if (iserrno == EENDFILE)		/* done */
		break;
	    else
		{				/* read error */
		sverr = iserrno;
		goto bad4;
		}
	if (iswrite(nisfd, buffer))
	    {					/* write error */
	    sverr = iserrno;
	    goto bad4;
	    }
	}
    free(buffer);				/* buffer no longer needed */

    /* get maximum serial number from original file and set new
     * file to that number
     */
    if (isuniqueid(isfd, &maxserial))
	{
	sverr = iserrno;
	goto bad3;
	}
    if (issetunique(nisfd, maxserial))
	{
	sverr = iserrno;
	goto bad3;
	}

#ifdef ISPCLOSE
    /* this is needed for DOS so pseudo closes do not leave
     * files open
     */
    sav = ismaxfds;
    ismaxfds = 0;
#endif /* ISPCLOSE */

    if (isclose(nisfd))
	{
	sverr = iserrno;
#ifdef ISPCLOSE
	ismaxfds = sav;
#endif
	goto bad1;
	}

    /* need to turn logging on so the close for the original
     * file is logged
     */
#ifdef ISXACTION
    isresumlog();
#endif
    if (isclose(isfd))
	{
	sverr = iserrno;
#ifdef ISPCLOSE
	ismaxfds = sav;
#endif
	goto bad2;
	}
#ifdef ISXACTION
    issusplog();
#endif

#ifdef ISPCLOSE
    ismaxfds = sav;
#endif /* ISPCLOSE */

    /* rename original file to a temp name, rename new file to be
     * original file then erase original file.  this is needed so
     * the original file stays around until everything is done.
     */
    if (isrename(oldname, tempname))
	{
	/* original file is closed but cannot be reopened since isfd
	 * may be different on reopen and cannot be passed back to caller
	 */
	sverr = iserrno;
	goto bad2;
	}
    if (isrename(newname, oldname))
	{
	sverr = iserrno;
	VOID isrename(tempname, oldname);
	goto bad2;
	}
    /* not much can be done if erase original file failed
     * leaves original file behind, user can clean up later
     * one solution is to set an error here but continue
     * with the rest of the code and return this error number
     * when done, so the user is aware of the extra file
     */
    VOID iserase(tempname);

#ifdef ISXACTION
    isresumlog();			/* resume logging       */

#ifndef SINIX
    if (txflag == TRANS)		/* has transaction	*/
#else /* SINIX, TP */
    if (txflag != NOTRANS)		/* has transaction	*/
#endif /* SINIX */
	if (islogidx(ISCLUSIDX, isfd, clustkey))
	    return -1;
#endif /* ISXACTION */

    /* re-opens new clustered file for calling program
     */
    return isopen(oldname, openmode);

bad4:
    free(buffer);
bad3:
    VOID isclose(nisfd);
bad2:
    VOID iserase(newname);
bad1:
#ifdef ISXACTION
    isresumlog();			/* resume logging       */
#endif /* ISXACTION */
    iserrno = sverr;
    return -1;
}


/*
 .prolog
 * isbnew
 *
 * original author: Tuoc Luong
 *
 * last modified by: Tuoc Luong
 *
 * purpose:
 *	build identical new file with all indexes but no data
 * function:
 *	1. build new file with no index
 *	2. add each index as isindexinfo tells
 * test scripts:
 *	<names of scripts that should be run after modifying this function>
 * parameters:
 *	int isfd;		old file isam file descriptor
 *	int *nisfd;		ptr to new isam file descriptor
 *	char *newname;          ptr to name of new file
 *
 *	modifies:
 *	nisfd will point to new isam file descriptor
 *
 * globals:
 *	<define all global variables NOT modified by the function>
 *	<definition should include expected range of input and output values>
 *
 *	modifies:
 *	<define all global variables modified by the function>
 *	<definition should include expected range of input and output values>
 * returns:
 *	recsize - record size of new file
 *	-1 if error, iserrno is set already
 *
 .eprolog
 */

isbnew(isfd, clustkey, nisfd, newname)
    register int isfd;
    register struct keydesc *clustkey;
    int *nisfd;
    char newname[];
{
    register int index, tisfd;
    register int recsize;
    struct keydesc keyd;
    struct dictinfo dinfo;

    if (isindexinfo(isfd, &dinfo, 0))
	return -1;

    keyd.k_nparts = 0;
    recsize = dinfo.di_recsize;

#ifdef SINIX	/* preserve primary key! TP 14.01.88 */
    if (isindexinfo(isfd, &keyd, 1))
	return -1;
    if (kycompare(&keyd, clustkey))
	keyd.k_flags &= ~ISCLUSTER;
    else
	keyd.k_flags |= ISCLUSTER;
#endif /* SINIX */

    if ((tisfd = isbuild(newname, recsize, &keyd, ISINOUT+ISEXCLLOCK)) == -1)
	return -1;

    /* copy over all index descriptions - turn on ISCLUSTER flag for the
     * current cluster index and turn off ISCLUSTER flag for previous
     * cluster index, if any
     */
#ifndef SINIX
    for (index = 1; ; index++)
#else /* SINIX: primary key already handled. TP 14.01.88 */
    for (index = 2; ; index++)
#endif /* SINIX */
	{
	if (isindexinfo(isfd, &keyd, index))
	    break;				/* no more indexes */
	if (keyd.k_nparts == 0)			/* dummy key */
	    continue;
	if (kycompare(&keyd, clustkey))
	    keyd.k_flags &= ~ISCLUSTER;
	else
	    keyd.k_flags |= ISCLUSTER;
	if (isaddindex(tisfd, &keyd))
	    goto bad;
	}

    *nisfd = tisfd;
    return recsize;

bad:
    VOID isclose(tisfd);
    VOID iserase(newname);
    return -1;
}



/*
 .prolog
 * ismkname
 *
 * original author: Tuoc Luong
 *
 * last modified by: Tuoc Luong
 *
 * purpose:
 *	create unique isam filename in same directory as old one
 * function:
 *	1. chop off base of oldname
 *	2. add on new base
 * test scripts:
 *	<names of scripts that should be run after modifying this function>
 * parameters:
 *	char oldname[];		full path name to old one
 *
 *	modifies:
 *	<define all parameters modified by the function>
 *	<definition should include expected range of input and output values>
 *
 * globals:
 *	<define all global variables NOT modified by the function>
 *	<definition should include expected range of input and output values>
 *
 *	modifies:
 *	<define all global variables modified by the function>
 *	<definition should include expected range of input and output values>
 * returns:
 *	returns new full path name if successful
 *	returns null if not sucessful
 *
 .eprolog
 */
ismkname(oldname, newname, tempname)
    char oldname[];
    char newname[], tempname[];
{
    register int len, tlen;

    len = stleng(oldname);
    tlen = stleng(istname);
    stcopy(oldname, newname);
    stcopy(oldname, tempname);
    if (mktmpname(istname, len, tlen, newname))
	return -1;
    return mktmpname(istname2, len, tlen, tempname);
}


char *isbname(fullname)
    char *fullname;
{
    register char *cp1, *cp2;

#ifndef PCDOS

#ifndef VMS
    for (cp1 = cp2 = fullname; *cp2 != 0;)
	if (*cp2++ == '/') cp1 = cp2;
#else /* VMS */
    cp2 = fullname;
    for (cp1 = cp2; *cp2 != 0; cp2++)
	if (*cp2 == '/' || *cp2 == ']') cp1 = cp2 + 1;
#endif /* VMS */
    
#else

    cp2 = fullname;
    if (*(cp2 + 1) == ':')  cp2 += 2;
    for (cp1 = cp2; *cp2 != 0; cp2++)
	if (*cp2 == '/' || *cp2 == '\\') cp1 = cp2 + 1;

#endif

    return(cp1);
}



/*
 *	ismktemp - makes unique name, taken mostly from rds's rbasename
 */
char *ismktemp(template)
    register char *template;
{
    register unsigned pid;
    register char *s;
    register char *tfid;		/* pts to 1-3 char fid */

    for (s = template; *s != 'N'; s++);	/* find start of file number */
    tfid = s;
    while (*s == 'N')			/* set file number */
	*s++ = 'a';
    while (*s == 'X')
	s++;
    s--;				/* on last X */

    pid = getpid();

    for (;;)
	{
	*s-- = pid % 10 + '0';		/* digit, (hi bit = 0) */
	pid /= 10;
	if (*s != 'X')
	    break;
	}
    
    while (access(template, 0) != -1)
	{				/* already exists */
	if (inctfid(tfid) == -1)
	    return (char *) NULL;
	}
    
    return template;
}


static inctfid(s)
    register char *s;
{
    register char *lsd;

    for (lsd = s + 2; lsd >= s; lsd--)
	{
	if ((*lsd)++ >= 'z')
	    {
	    *lsd = 'a';
	    continue;				/* next msd */
	    }
	return 0;
	}
    return -1;					/* overflow */
}



/* mktmpname
 *    function: 1. generate a unique file name
 */
mktmpname(tmplate, len, tlen, newname)
    register char *tmplate;
    register int len, tlen;
    register char *newname;
{
    register char *base, *name;

    if ((name = ismktemp(tmplate)) == NULL)
    	return -1;
    *(name + tlen) = '\0';

    base = isbname(newname);
    if (len == stleng(base))
	stcopy(name, newname);
    else
	{
	*base = '\0';
	stcat(name, newname);
	}
    return 0;
}
