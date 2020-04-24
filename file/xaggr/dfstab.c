/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* Copyright (C) 1996, 1989 Transarc Corporation - All rights reserved */

/*
		    Utilities for examining /etc/dfstab
 */

#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#include <dcedfs/common_data.h>
#include <fcntl.h>
#include <astab.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsxagmac.h>
#include <dfsxagsvc.h>
#include <dfsxagmsg.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/xaggr/RCS/dfstab.c,v 4.35 1996/10/16 16:50:01 gait Exp $")

char *AstabFileName;

#if defined(AFS_AIX_ENV) || defined(AFS_SUNOS5_ENV)
PRIVATE struct flock fileWlock = { F_WRLCK, 0, 0, 0, 0, 0 };
#endif
#ifdef AFS_HPUX_ENV
PRIVATE struct flock fileWlock = { F_WRLCK, 0, 0, 0, 0 };
#endif

/*
 * Return next string component
 */
static char *getstr(p)
    register char **p;
{
    char *cp = *p, *retstr;

    while (*cp && isspace(*cp))
	cp++;
    retstr = cp;
    while (*cp && !isspace(*cp))
	cp++;
    if (*cp) {
	*cp = '\0';
	cp++;
    }
    *p = cp;
    return (retstr);
}


/*
 * Return an identifier (digits) back
 */
static u_long getLong(p, validP)
    register char **p;
    int *validP;
{
    register u_long value = 0;
    char *cp = *p;

    *validP = 0;	/* not yet valid */
    while (*cp && isspace(*cp))
	cp++;
    for (; *cp && isdigit(*cp); cp++) {
	value *= 10;
	value += *cp - '0';
	*validP = 1;	/* this is a valid u_long */
    }
    while (*cp && !isspace(*cp))
	cp++;
    if (*cp) {
	*cp = '\0';
	cp++;
    }
    *p = cp;
    return (value);
}

 /*
  * Parse a whole line and return the astab entry associated with it. The
  * number of fields found in the line is the return value.
  */
 static parseEntry(filep, bufferp, tabp, fstabp)
     FILE *filep;
     char *bufferp;
     struct astab *tabp;			/* data about aggregate */
     struct ufs_astab *fstabp;		/* file-system-specific data;
					  * UFS only, at present */
 {
     char savebuf[257];
     char *cp, *str;
     int validLong;

     bzero((char *)tabp, sizeof(*tabp));
     bzero((char *)fstabp, sizeof(*fstabp));
     do {
	 cp = fgets(bufferp, 256, filep);
	 if (cp == NULL) {
	     return (EOF);
	 }
     } while (*cp == '#' || *cp == '\n');

     /* may need this for error messages */
     strncpy(savebuf, bufferp, 256);

     /* Block special device pathname */
     str = getstr(&cp);
     if (strlen(str) >= (unsigned)ASTABSIZE_SPEC) return 0;
     strncpy(tabp->as_spec, str, ASTABSIZE_SPEC);
     if (*cp == '\0') {
	 (void) dce_svc_printf(XAG_S_ENTRY_NOT_CMPLT_MSG, savebuf);
	 return (1);
     }

     /* Aggregate name */
     str = getstr(&cp);
     if (strlen(str) >= (unsigned)ASTABSIZE_DIR) return 0;
     strncpy(tabp->as_aggrName, str, ASTABSIZE_DIR);
     if (*cp == '\0') {
	 (void) dce_svc_printf(XAG_S_ENTRY_NOT_CMPLT_MSG, bufferp);
	 return (2);
     }

     /* Aggregate type */
     str = getstr(&cp);
     if (strlen(str) >= (unsigned)ASTABSIZE_TYPE) return 0;
     strncpy(tabp->as_type, str, ASTABSIZE_TYPE);

     /* Now that we have type, see if need to do special UFS things */
     if (strncmp(tabp->as_type, ASTABTYPE_UFS, ASTABSIZE_TYPE) == 0)
	 strncpy(fstabp->uas_mountedon, tabp->as_aggrName, ASTABSIZE_DIR);

     if (*cp == '\0') {
	 (void) dce_svc_printf(XAG_S_ENTRY_NOT_CMPLT_MSG, savebuf);
	 return (3);
     }

     /* Aggregate ID */
     tabp->as_aggrId = getLong(&cp, &validLong);
     if (!validLong) {
	 (void) dce_svc_printf(XAG_S_IMPROPER_AGGR_ID_MSG, savebuf);
	 return (3);	/* no aggregate ID */
     }

     /* Volume ID (for UFS only) */
     if (strncmp(tabp->as_type, ASTABTYPE_UFS, ASTABSIZE_TYPE) == 0) {
	 if (dfsh_StrToHyper(cp, &(fstabp->uas_volId), &cp) == 0)
	     return (5);
	 else {
	     (void) dce_svc_printf(XAG_S_IMPROPER_FS_ID_MSG, savebuf);
	     return (0);	/* no volume ID */
	 }
     } else
	 return (4);
 }


 /*
  * Return next valid /etc/dfstab entry back to the caller
  */
 int as_getent(filep, bufferp, astabp, fstabp)
     FILE *filep;
     char *bufferp;
     register struct astab *astabp;
     struct ufs_astab *fstabp;
 {
     int code;

     code = parseEntry(filep, bufferp, astabp, fstabp);
     if (strcmp(astabp->as_type, ASTABTYPE_UFS) == 0
	     ? code != 5
	     : code != 4)
	 return 0;
     return 1;
 }

 void as_init()
 {
     int code;
     static char dcelcl_Path[] = DCELOCAL_PATH;
     static char astab_Infix[] = ASTAB_INFIX;
     static char astab_Sfx[] = ASTAB_SFX;

     code = sizeof(dcelcl_Path) + sizeof(astab_Infix) + sizeof(astab_Sfx);
     AstabFileName = (char *) malloc(code);
     if (AstabFileName == NULL) {
	 dce_svc_printf(XAG_S_NO_MEMORY_MSG);
	 exit(1);
     }
     strcpy(AstabFileName, dcelcl_Path);	/* "/opt/dcelocal" */
     strcat(AstabFileName, astab_Infix);	/* "/opt/dcelocal/var/dfs/" */
     strcat(AstabFileName, astab_Sfx);    /* "/opt/dcelocal/var/dfs/dfstab" */
 }

 /*
  * Append the entry represented by astabp to atabfilep. 
  */
 int addtoatab(atabfilep, astabp, fstabp)
     char *atabfilep;
     register struct astab *astabp;
     struct ufs_astab *fstabp;
 {
     FILE *filep;
     int fd;

     if ((filep = fopen(atabfilep, "r+")) == NULL) {
	 fd = open(atabfilep, O_CREAT | O_TRUNC, 0644);
	 if (fd <= 0) {
	     fprintf(stderr, "dfsexport: Can't create %s (err=%d)\n", atabfilep, errno);
	     return(1);
	 }
	 close(fd);
	 if ((filep = fopen(atabfilep, "r+")) == NULL) {
	     fprintf(stderr, "dfsexport: Can't open for append %s (err=%d)\n", atabfilep, errno);
	     return(1);
	 }	    
     }
 #if defined(AFS_AIX_ENV) || defined(AFS_HPUX_ENV) || defined(AFS_SUNOS5_ENV)
     if (fcntl(fileno(filep), F_SETLK, &fileWlock) < 0)
 #else
     if (flock(fileno(filep), LOCK_EX) < 0)
 #endif
     {
	 fclose(filep);
	 return -1;
     }
     if (fseek(filep, 0, 2) < 0)
	 return -1;
     if (astabp && fstabp)
	 fprintf(filep, "%s %s %s %lu %lu,,%lu\n",
		 astabp->as_spec, astabp->as_aggrName, 
		 astabp->as_type, astabp->as_aggrId,
		 AFS_HGETBOTH(fstabp->uas_volId));
    fclose(filep);
    return 0;
}

void printastab(stabp, fstabp, stream)
    struct astab *stabp;
    struct ufs_astab *fstabp;
    FILE *stream;
{
    fprintf(stream, "%-20s %-20s %-7s %-10lu",
	    stabp->as_spec, stabp->as_aggrName, stabp->as_type, 
	    stabp->as_aggrId);

    if (strcmp(stabp->as_type, ASTABTYPE_UFS) == 0) 
	fprintf(stream, " %lu,,%lu", AFS_HGETBOTH(fstabp->uas_volId));
}
