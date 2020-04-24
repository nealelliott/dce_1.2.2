/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* Copyright (C) 1996, 1989 Transarc Corporation - All rights reserved */

/*
 * HISTORY
 * $Log: volclient.c,v $
 * Revision 1.1.640.1  1996/10/02  19:04:51  damon
 * 	New DFS from Transarc
 * 	[1996/10/01  19:00:08  damon]
 *
 * $EndLog$
 */
#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/xvfs_vnode.h>
#include <dcedfs/syscall.h>
#include <dcedfs/volume.h>
#include <dcedfs/osi.h>
#include <vol_init.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/xvolume/volclient.c,v 1.1.640.1 1996/10/02 19:04:51 damon Exp $")

long vdesc;
char buffer[512];
char *(args[50]);

main(argc, argv)
    int argc;
    char **argv; {
    register long code = 0;

    osi_setlocale(LC_ALL, "");
    while (1) {
	char line[500];
	int nargs;

	printf("vs> ");
	gets(line);
	if (strlen(line)) {
	    char *oper;
	    register char **argp = args;
	    GetArgs(line, argp, &nargs);
	    oper = &argp[0][0];
	    ++argp, --nargs;
	    if (!strcmp(oper, "lock")) {
	        code = vol_Lock(argp);
	    } else if (!strcmp(oper, "unlock")) {
	        code = vol_Unlock(argp);
	    } else if (!strcmp(oper, "open")) {
	        code = vol_Open(argp);
	    } else if (!strcmp(oper, "seek")) {
	        code = vol_Seek(argp);
	    } else if (!strcmp(oper, "tell")) {
	        code = vol_Tell(argp);
	    } else if (!strcmp(oper, "scan")) {
	        code = vol_Scan(argp);
	    } else if (!strcmp(oper, "close")) {
	        code = vol_Close(argp);
	    } else if (!strcmp(oper, "destroy")) {
	        code = vol_Destroy(argp);
	    } else if (!strcmp(oper, "gstat")) {
	        code = vol_Getstatus(argp);
	    } else if (!strcmp(oper, "sstat")) {
	        code = vol_Setstatus(argp);
	    } else if (!strcmp(oper, "create")) {
	        code = vol_Create(argp);
	    } else if (!strcmp(oper, "read")) {
	        code = vol_Read(argp);
	    } else if (!strcmp(oper, "write")) {
	        code = vol_Write(argp);
	    } else if (!strcmp(oper, "trunc")) {
	        code = vol_Trunc(argp);
	    } else if (!strcmp(oper, "del")) {
	        code = vol_Del(argp);
	    } else if (!strcmp(oper, "geta")) {
	        code = vol_Getattr(argp);
	    } else if (!strcmp(oper, "seta")) {
	        code = vol_Setattr(argp);
	    } else if (!strcmp(oper, "clone")) {
	        code = vol_Clone(argp);
	    } else if (!strcmp(oper, "reclone")) {
	        code = vol_Reclone(argp);
	    } else if (!strcmp(oper, "unclone")) {
	        code = vol_Unclone(argp);
	    } else if (!strcmp(oper, "dump")) {
	        code = vol_Dump(argp);
	    } else if (!strcmp(oper, "copy")) {
	        code = vol_Copy(argp);
	    } else if (!strcmp(oper, "fclose")) {
	        code = vol_Fclose(argp);
	    } else if (!strcmp(oper, "setvv")) {
	        code = vol_SetVVers(argp);
	    } else if ((!strcmp(oper,"q")) || !strcmp(oper, "quit")) 
		 exit(0);
	    else {
		 printf("Unknown oper! Available operations: \n\n");
		 printf("lock <volid> <type>\n");
		 printf("unlock <volid> <type>\n");
		 printf("open <volid.high> <volid.low> <rwType> <errType>\n");
		 printf("seek <desc> <position>\n");
		 printf("tell <desc>\n");
		 printf("scan <desc> <position>\n");
		 printf("close <desc>\n");
		 printf("destroy <desc>\n");
		 printf("gstat <desc>\n");
		 printf("sstat <desc> <name> [<index>|-1] [<bindex>|-1] [<vlimit>|-1] [<alimit>|-1]\n");
		 printf("create <desc> <index> <mode> <owner> <length> <type>\n"); 
		 printf("read <desc> <hyperposition> <len>\n");
		 printf("write <desc> <hyperposition>\n");
		 printf("trunc <desc> <hypersize>\n");
		 printf("del <desc>\n");
		 printf("geta <desc>\n");
		 printf("seta <desc> <mode> <owner> <len>\n");
		 printf("clone <desc> <index> <cid.h> <cid.l> <vid.h> <vid.l> <vn> <uniq>\n");
		 printf("reclone <desc>\n");
		 printf("unclone <desc>\n");
		 printf("dump <desc>\n");
		 printf("copy <fromdesc, todesc>\n");
		 printf("fclose <volid>\n");
		 printf("setvv <volid> flags mask currtime pingtime\n");
		 printf("NOTE:  a hyper is entered <high>,,<low>\n");
	       }
	}
    }
}


GetArgs(line,args, nargs)
    register char *line;
    register char **args;
    register int *nargs;
{
    *nargs = 0;
    while (*line) {
	register char *last = line;
	while (*line == ' ')
	    line++;
	if (*last == ' ')
	    *last = 0;
	if (!*line)
	    break;
	*args++  = line, (*nargs)++;
	while (*line && *line != ' ')
	    line++;
    }
}


void printcode (code) 
    int code;
{
    printf ("return code is %d\n", code);
    if (code) printf ("errno is %d\n", errno);
}

void printhex (addr, len)
    caddr_t addr;
    int len;
{
    int i, j, offset;
    for (i=0;i<=((len-1)/32);i++) {
	for (j=0;j<8;j++) {
	    offset = i*32 + j*4;
	    if (offset<len) printf("%x  ",*((long *) (addr) + (offset/4)));
	    if (j==7) printf("\n");
	}
    }
}



int
dumpbuff(buff, blen)
char *buff;
int  blen;
{
    int i, j, k;
    u_char atxt[17];

    atxt[16] = '\0';
    i=0;
    while (i < blen) {
	j=0;
	k=0;
	while ((j < 16) && (i < blen)) {
	    printf("%2.2x",buff[i]);
	    atxt[j] = ((buff[i] < 0x20) || (buff[i] >= 0x7f))? 
		    '.': buff[i];
	    j++;
	    i++;
	    k++;
	    if (k == 4) {
	        printf(" ");
	        k = 0;
	    }
	}
	printf ("  |%s|\n",atxt);
    }
}

print_vol_stats(desc, st)
	int desc;
    	struct vol_status *st;
{
    printf("fileset info for desc %d:\n", desc);

    printf("fileset name <%s>, type %d, accstatus %d, accerror %d\n", 
	st->vol_st.volName, st->vol_st.type,
	st->vol_st.accStatus, st->vol_st.accError);

    printf("id(%u,,%u), parent(%u,,%u), states %#lx\n",
	   AFS_HGETBOTH(st->vol_st.volId), AFS_HGETBOTH(st->vol_st.parentId),
	   st->vol_st.states);

    printf("clonetime(%d, %d), curtime(%d, %d), pingtime(%d, %d)\n",
	st->vol_st.cloneTime.sec, st->vol_st.cloneTime.usec,
	st->vol_st.vvCurrentTime.sec, st->vol_st.vvCurrentTime.usec,
	st->vol_st.vvPingCurrentTime.sec, st->vol_st.vvPingCurrentTime.usec);

    printf("ver(%u,,%u) #files %d alloclim %lu,,%lu allocused %lu,,%lu vislim %lu,,%lu visused %lu,,%lu\n", 
	   AFS_HGETBOTH(st->vol_dy.volversion),
	   st->vol_dy.minQuota, st->vol_dy.fileCount,
	   AFS_HGETBOTH(st->vol_dy.allocLimit),
	   AFS_HGETBOTH(st->vol_dy.allocUsage),
	   AFS_HGETBOTH(st->vol_dy.visQuotaLimit),
	   AFS_HGETBOTH(st->vol_dy.visQuotaUsage));

    printf("owner %d ctime(%d, %d) atime(%d, %d) utime(%d, %d) smsg %s\n",
    	st->vol_dy.owner, 
    	st->vol_dy.creationDate.sec, st->vol_dy.creationDate.usec,
    	st->vol_dy.accessDate.sec, st->vol_dy.accessDate.usec,
    	st->vol_dy.updateDate.sec, st->vol_dy.updateDate.usec,
    	st->vol_dy.statusMsg);
}


vol_Lock(argp)
    char **argp;
{
    int type, code = 0, desc;

    sscanf(&(*argp)[0], "%d", &desc);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &type);
    ++argp;    
    code = afs_syscall(AFSCALL_VOL, VOLOP_LOCK, desc, type, 0, 0);
    printcode (code);
}


vol_Unlock(argp)
    char **argp;
{
    int type, code = 0, desc;

    sscanf(&(*argp)[0], "%d", &desc);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &type);
    ++argp;    
    code = afs_syscall(AFSCALL_VOL, VOLOP_UNLOCK, desc, type, 0, 0);
    printcode (code);
}



vol_Open(argp)
    char **argp;
{
    int type, code, errorType;
    afs_hyper_t volId;
    int high;
    u_int low;

    sscanf(&(*argp)[0], "%d", &high);
    ++argp;
    sscanf(&(*argp)[0], "%u", &low);
    ++argp;
    AFS_hset64(volId, high, low);
    sscanf(&(*argp)[0], "%d", &type);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &errorType);
    ++argp;    
    code = afs_syscall(AFSCALL_VOL, VOLOP_OPEN, (int) &volId,
			type, errorType, (int) &vdesc);
    printcode (code);
    printf("desc=%d\n", vdesc);
}


vol_Seek(argp)
    char **argp;
{
    int position, code, desc;

    sscanf(&(*argp)[0], "%d", &desc);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &position);
    ++argp;    
    code = afs_syscall(AFSCALL_VOL, VOLOP_SEEK, desc, position, 0, 0);
    printcode (code);
}



vol_Tell(argp)
    char **argp;
{
    int position, code, desc;

    sscanf(&(*argp)[0], "%d", &desc);
    ++argp;    
    code = afs_syscall(AFSCALL_VOL, VOLOP_TELL, desc, (int) &position, 0, 0);
    printcode (code);
    if (!code)
	printf("position is %d\n", position);
}


vol_Scan(argp)
    char **argp;
{
    int position, code, desc;

    sscanf(&(*argp)[0], "%d", &desc);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &position);
    ++argp;    
    code = afs_syscall(AFSCALL_VOL, VOLOP_SCAN, desc, position, 0, 0);
    printcode (code);
}


vol_Close(argp)
    char **argp;
{
    int code, desc;

    sscanf(&(*argp)[0], "%d", &desc);
    ++argp;    
    code = afs_syscall(AFSCALL_VOL, VOLOP_CLOSE, desc, 0 /* !abort */, 0, 0);
    printcode (code);
}


vol_Destroy(argp)
    char **argp;
{
    int code, desc;

    sscanf(&(*argp)[0], "%d", &desc);
    ++argp;    
    code = afs_syscall(AFSCALL_VOL, VOLOP_DESTROY, desc, 0, 0, 0);
    printcode (code);
}


vol_Getstatus(argp)
    char **argp;
{
    int code, desc;
    struct vol_status status;

    sscanf(&(*argp)[0], "%d", &desc);
    ++argp;    
    code = afs_syscall(AFSCALL_VOL, VOLOP_GETSTATUS, desc, (int) &status, 0, 0);
    printcode (code);
    if (!code) {
	print_vol_stats(desc, &status);
    }
}


vol_Setstatus(argp)
    char **argp;
{
    int code, mask=0, desc;
    long index, backindex, vislimit, alloclimit;
    struct vol_status status;

    bzero((char *)&status, sizeof(struct vol_status));
    sscanf(&(*argp)[0], "%d", &desc);
    ++argp;    
    sscanf(&(*argp)[0], "%s", status.vol_st.volName);
    mask |= VOL_STAT_VOLNAME;
    ++argp;    
    sscanf(&(*argp)[0], "%d", &index);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &backindex);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &vislimit);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &alloclimit);
    ++argp;    
    if (index != -1) {
	status.vol_dy.index = index;
	mask |= VOL_STAT_INDEX;
    }
    if (backindex != -1) {
	status.vol_dy.backupIndex = backindex;
	mask |= VOL_STAT_BACKVOLINDEX;
    }
    if (vislimit != -1) {
	AFS_hset64(status.vol_dy.visQuotaLimit, 0, vislimit);
	AFS_hleftshift(status.vol_dy.visQuotaLimit, 10);
	mask |= VOL_STAT_VISLIMIT;
    }
    if (alloclimit != -1) {
	AFS_hset64(status.vol_dy.allocLimit, 0, alloclimit);
	AFS_hleftshift(status.vol_dy.allocLimit, 10);
	mask |= VOL_STAT_ALLOCLIMIT;
    }
    code = afs_syscall(AFSCALL_VOL, VOLOP_SETSTATUS, desc,
		       mask, (int) &status, 0);
    printcode (code);
}


vol_Create(argp)
    char **argp;
{
    int code, desc;
    int index, mode, owner, length, type;
    struct xvfs_attr xvattr;

    bzero((void *)&xvattr, sizeof (xvattr));
    sscanf(&(*argp)[0], "%d", &desc);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &index);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &mode);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &owner);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &length);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &type);
    ++argp;    
    osi_vattr_init(&xvattr.vattr, OSI_VA_MODE | OSI_VA_UID | OSI_VA_SIZE);
    xvattr.vattr.va_mode = mode;
    xvattr.vattr.va_uid = owner;
    xvattr.vattr.va_size = length;
    xvattr.vattr.va_type = type;
    code = afs_syscall(AFSCALL_VOL, VOLOP_CREATE, desc, index, (int) &xvattr, 0);
    printcode (code);
}


vol_Read(argp)
    char **argp;
{
    int code, desc;
    int outlen, positionhi, positionlo, len;
    struct io_rwHyperDesc iodesc;

    bzero(buffer, 512);
    bzero((char *)&iodesc, sizeof(struct io_rwDesc));
    sscanf(&(*argp)[0], "%d", &desc);
    ++argp;    
    sscanf(&(*argp)[0], "%d,,%d", &positionhi, &positionlo);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &len);
    ++argp;    
    AFS_hset64(iodesc.position, positionhi, positionlo);
    iodesc.length = len;
    iodesc.bufferp = buffer;
    code = afs_syscall(AFSCALL_VOL, VOLOP_READ, desc, (int) &iodesc,
		       (int) &outlen, 0);
    printcode (code);
    if (!code) {
	printf("outlen=%x\n", outlen);
	printf("data (hex):\n");
	dumpbuff (buffer, outlen);
    }
}


vol_Write(argp)
    char **argp;
{
    int code, desc;
    int positionhi, positionlo;
    struct io_rwHyperDesc iodesc;
    char *pnt = "THIS IS A SILLY SILLY TEST\n";

    bzero((char *)&iodesc, sizeof(struct io_rwDesc));
    sscanf(&(*argp)[0], "%d", &desc);
    ++argp;    
    sscanf(&(*argp)[0], "%d,,%d", &positionhi, &positionlo);
    ++argp;    
    AFS_hset64(iodesc.position, positionhi, positionlo);
    iodesc.length = strlen(pnt);
    iodesc.bufferp = pnt;
    code = afs_syscall(AFSCALL_VOL, VOLOP_WRITE, desc, (int) &iodesc, 0, 0);
    printcode (code);
}



vol_Trunc(argp)
    char **argp;
{
    int code, sizehi, sizelo, desc;
    afs_hyper_t realSize;

    sscanf(&(*argp)[0], "%d", &desc);
    ++argp;    
    sscanf(&(*argp)[0], "%d,,%d", &sizehi, &sizelo);
    ++argp;    
    AFS_hset64(realSize, sizehi, sizelo);
    code = afs_syscall(AFSCALL_VOL, VOLOP_TRUNCATE, desc, (int) &realSize, 0, 0);
    printcode (code);
}


vol_Del(argp)
    char **argp;
{
    int code, desc;

    sscanf(&(*argp)[0], "%d", &desc);
    ++argp;    
    code = afs_syscall(AFSCALL_VOL, VOLOP_DELETE, desc, 0, 0, 0);
    printcode (code);
}


vol_Getattr(argp)
    char **argp;
{
    int code, desc;
    struct xvfs_attr xvattr;

    sscanf(&(*argp)[0], "%d", &desc);
    ++argp;    
    code = afs_syscall(AFSCALL_VOL, VOLOP_GETATTR, desc, (int) &xvattr, 0, 0);
    printcode (code);
    if (!code) {
	printf("attributes (hex):\n");
	printhex (&xvattr, sizeof(xvattr));
    }
}


vol_Setattr(argp)
    char **argp;
{
    int code, mode, owner, length, desc;
    struct xvfs_attr xvattr;

    bzero((void *)&xvattr, sizeof(xvattr));
    sscanf(&(*argp)[0], "%d", &desc);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &mode);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &owner);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &length);
    ++argp;    
    osi_vattr_init(&xvattr.vattr, OSI_VA_MODE | OSI_VA_UID | OSI_VA_SIZE);
    xvattr.vattr.va_mode = mode;
    xvattr.vattr.va_uid = owner;
    xvattr.vattr.va_size = length;
    code = afs_syscall(AFSCALL_VOL, VOLOP_SETATTR, desc, (int) &xvattr, 0, 0);
    printcode (code);
}


vol_Clone(argp)
    char **argp;
{
    int code, index, desc;
    struct afsFid fid;
    int high;
    u_int low;

    sscanf(&(*argp)[0], "%d", &desc);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &index);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &high);
    ++argp;    
    sscanf(&(*argp)[0], "%u", &low);
    ++argp;    
    AFS_hset64(fid.Cell, high, low);
    sscanf(&(*argp)[0], "%d", &high);
    ++argp;    
    sscanf(&(*argp)[0], "%u", &low);
    ++argp;    
    AFS_hset64(fid.Volume, high, low);
    sscanf(&(*argp)[0], "%d", &fid.Vnode);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &fid.Unique);
    ++argp;
    code = afs_syscall(AFSCALL_VOL, VOLOP_CLONE, desc, index, (int) &fid, 0);
    printcode (code);
}


vol_Reclone(argp)
    char **argp;
{
    int code, desc;

    sscanf(&(*argp)[0], "%d", &desc);
    ++argp;    
    code = afs_syscall(AFSCALL_VOL, VOLOP_RECLONE, desc, 0, 0, 0);
    printcode (code);
}


vol_Unclone(argp)
    char **argp;
{
    int code, desc;

    sscanf(&(*argp)[0], "%d", &desc);
    ++argp;    
    code = afs_syscall(AFSCALL_VOL, VOLOP_UNCLONE, desc, 0, 0, 0);
    printcode (code);
}

#define TBSIZE 8192

vol_Dump(argp)
    char **argp;
{
    int code, desc, i, bad = 0, deleted = 0, gdeleted = 0, gbad = 0;
    struct xvfs_attr xvattr;
    struct vol_status volStatus;
    char tbuffer[TBSIZE];
    struct io_rwHyperDesc iodesc;
    long isize, outlen, tlen;
    afs_hyper_t filePos;

    sscanf(&(*argp)[0], "%d", &desc);
    ++argp;    

    if (code = afs_syscall(AFSCALL_VOL, VOLOP_GETSTATUS, desc,
			   (int) &volStatus, 0, 0)) {
	printf("VOL_GETSTATUS returned %d\n", code);
	return;
    } else {
	print_vol_stats(desc, &volStatus);
    }

    for (i = VOL_ROOTINO;; i++) {
    	if (!(code = afs_syscall(AFSCALL_VOL, VOLOP_SCAN, desc, i, 0, 0)))
    		code = afs_syscall(AFSCALL_VOL, VOLOP_GETATTR, desc,
				   (int) &xvattr, 0, 0);

	if (code) {
	    if (errno == VOL_ERR_EOF || errno == VOL_ERR_EOW) {
		printf("FOUND EOF i=%d\n", i);
		code = 0;
		break;
	    } else if (errno == VOL_ERR_DELETED) {
		deleted++;
	    } else {
		printf("%d-SCAN err=%d\n", i, errno);
		bad++;
	    }
	    continue;
	}

       printf("i=%d tp=%d sz=%d md=%o own=(%d,%d) lnk=%d blks=%d\n",
	i, xvattr.vattr.va_type, xvattr.vattr.va_size, xvattr.vattr.va_mode, 
	xvattr.vattr.va_uid, xvattr.vattr.va_gid,
#ifdef AFS_OSF_ENV
	xvattr.vattr.va_nlink, xvattr.vattr.va_size
#else
	xvattr.vattr.va_nlink, xvattr.vattr.va_blocks
#endif /* AFS_OSF_ENV */
	       );

	/* Read vnode data */
	iodesc.bufferp = tbuffer;
        AFS_hzero(filePos);

	if (xvattr.vattr.va_type == VDIR) {
	    unsigned long count;
	    struct vol_dirent *vdirp;
	    int j;

	    iodesc.length = TBSIZE;
	    AFS_hzero(iodesc.position);

	    printf("Dir contents for index %d:\n", i);
	    do {
		if ((code = afs_syscall(AFSCALL_VOL, VOLOP_READDIR,
		    desc, (int) &iodesc, (int) &count, 0)) && code != VOL_ERR_EOF)
		    break;
		vdirp = (struct vol_dirent *)iodesc.bufferp;
		for (j = 0; j < count; j++) {
		    printf("  i<%d>, off<%d>, rlen<%d>, nlen<%d>, name<%s>\n",
			vdirp->vnodeNum, vdirp->offset, vdirp->reclen,
			vdirp->namelen, vdirp->name);
		    vdirp = (struct vol_dirent *)
			((char *)vdirp + vdirp->reclen);
		}
	    } while  (code != VOL_ERR_EOF);
	    printf("End dir contents\n");
	} else {
	    for (isize = xvattr.vattr.va_size; isize > 0;isize -= tlen) {
		tlen = (isize > TBSIZE ? TBSIZE : isize);
		iodesc.position = filePos;
		iodesc.length = tlen;
		if (code = afs_syscall(AFSCALL_VOL, VOLOP_READ, desc,
				       (int) &iodesc, (int)&outlen, 0)) {
		    printf("read failed on index %d size %d\n", i, isize);
		    break;
		} else {
		    printf("outlen %d, data:\n", outlen);
		    dumpbuff(tbuffer, outlen);
		}
		AFS_hadd32(filePos, tlen);
	    }
	}
    }
    printf("\n\nnumber of inodes %d (bad %d.%d, deleted %d.%d)\n", i, 
	   bad, gbad, deleted, gdeleted);
}

vol_Copy(argp)
    char **argp;
{
    int code, fdesc, tdesc, i, delflag=0;
    struct xvfs_attr xvattr;
    struct vol_status fvolStatus, tvolStatus;
    char tbuffer[TBSIZE];
    struct io_rwHyperDesc iodesc;
    long isize, outlen, tlen, mask;
    afs_hyper_t filePos;

    sscanf(&(*argp)[0], "%d", &fdesc);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &tdesc);
    ++argp;    

    if (code = afs_syscall(AFSCALL_VOL, VOLOP_GETSTATUS, fdesc,
			   (int)&fvolStatus, 0, 0)) {
	printf("VOL_GETSTATUS for %d returned %d\n", fdesc, code);
	return;
    } else
	print_vol_stats(fdesc, &fvolStatus);

    if (code = afs_syscall(AFSCALL_VOL, VOLOP_GETSTATUS, tdesc,
			   (int) &tvolStatus, 0, 0)) {
	printf("VOL_GETSTATUS for %d returned %d\n", tdesc, code);
	return;
    } else
	print_vol_stats(tdesc, &tvolStatus);

    for (i = VOL_ROOTINO;; i++) {
	/* Get from vnode */
    	if (!(code = afs_syscall(AFSCALL_VOL, VOLOP_SCAN, fdesc, i, 0, 0)))
    		code = afs_syscall(AFSCALL_VOL, VOLOP_GETATTR, fdesc,
				   (int) &xvattr, 0, 0);
	if (code) {
	    if (errno == VOL_ERR_EOF || errno == VOL_ERR_EOW) {
		code = 0;
		break;
	    } else if (errno == VOL_ERR_DELETED) {
		delflag = 1;
	    } else
	 	   continue;
	}

	/* 
	 * Look for destination.  If it exists then delete it, even
	 * if it will be recreated.
	 */
    	if (code = afs_syscall(AFSCALL_VOL, VOLOP_SCAN, tdesc, i, 0, 0)) {
	    if (errno == VOL_ERR_EOF) {
		printf("hit EOF in destination\n");
		code = 0;
		break;
	    } else if (errno != ENOENT) {
		printf("scan of vnode %d failed %d\n", i, errno);
		continue;
	    }
	} else {
	    if (code = afs_syscall(AFSCALL_VOL, VOLOP_DELETE, tdesc, 0, 0, 0))
	        printf("delete vnode %d failed %d\n", i, errno);
	}

	if (!delflag) {
    	    if (code = afs_syscall(AFSCALL_VOL, VOLOP_CREATE, 
		tdesc, i, (int) &xvattr, 0)) {
		printf("create vnode %d failed %d\n", i, errno);
	    }
	}

	/* Copy vnode data */
	iodesc.bufferp = tbuffer;
	AFS_hzero(filePos);
	isize = (delflag)? 0: xvattr.vattr.va_size;

	if (isize && (xvattr.vattr.va_type == VDIR)) {
	    unsigned long count;
	    struct vol_dirent *vdirp;
	    int j;

	    iodesc.length = TBSIZE;
	    AFS_hzero(iodesc.position);

	    printf("Copy Dir contents for index %d:\n", i);
	    do {
		if ((code = afs_syscall(AFSCALL_VOL, VOLOP_READDIR,
					fdesc, (int) &iodesc, (int) &count, 0))
		    && code != VOL_ERR_EOF)
		    break;

		/* Write to destination */
		if (code != VOL_ERR_EOF) {
		    code = afs_syscall(AFSCALL_VOL, VOLOP_APPENDDIR,
		        tdesc, (int) &iodesc, count, 0);
		    if (code) {
			printf("Appenddir failed (%x)\n", code);
			break;
		    }
		}
	    } while  (code != VOL_ERR_EOF);
	    printf("End dir copy\n");
	} else {
	    while (isize > 0) {
		tlen = (isize > TBSIZE ? TBSIZE : isize);
		iodesc.position = filePos;
		iodesc.length = tlen;
		if (code = afs_syscall(AFSCALL_VOL, VOLOP_READ, fdesc,
				       (int) &iodesc, (int) &outlen, 0)) {
		    printf("read failed on index %d size %d\n", i, isize);
		    break;
		} else {
    		    code = afs_syscall(AFSCALL_VOL, VOLOP_WRITE, 
			tdesc, (int) &iodesc, 0, 0);
		    if (code)
			printf("write to vnode %d, pos %lu,,%lu, len %d failed\n",
			       i, AFS_HGETBOTH(filePos), tlen);
		}
		AFS_hadd32(filePos, tlen);
		isize -= tlen;
	    }
	}
	/* Set the to vnode attrs */
	if (!delflag) {
	    if (xvattr.vattr.va_type == 2)
		xvattr.vattr.va_size = -1;

	    xvattr.vattr.va_blocksize = xvattr.vattr.va_rdev = -1;
#ifdef AFS_OSF_ENV
	    xvattr.vattr.va_size = 0;
	    xvattr.vattr.va_fsid = -1;
#else
	    xvattr.vattr.va_blocks = xvattr.vattr.va_fsid = -1;
#endif /* AFS_OSF_ENV */
	    xvattr.vattr.va_nodeid = xvattr.vattr.va_type = -1;
    	    if (code = afs_syscall(AFSCALL_VOL, VOLOP_SETATTR, tdesc,
				   (int)&xvattr, 0, 0))
		printf("settattr on vnode %d returned %d\n", i, errno);
	} else
		delflag = 0;
    }
    /* Update to volume attrs */
    mask = VOL_STAT_VERSION| VOL_STAT_CREATEDATE| VOL_STAT_UPDATEDATE| 
	VOL_STAT_ACCESSDATE| VOL_STAT_ALLOCLIMIT| VOL_STAT_VISLIMIT| 
	VOL_STAT_STATUSMSG;

    code = afs_syscall(AFSCALL_VOL, VOLOP_SETSTATUS, tdesc, mask,
		       (int)&fvolStatus, 0);
    if (code)
	printf("fileset setstatus failed errno=%d/n", errno);

    if (code = afs_syscall(AFSCALL_VOL, VOLOP_GETSTATUS, tdesc,
			   (int) &tvolStatus, 0, 0)) {
	printf("VOL_GETSTATUS failed for %d returned %d\n", tdesc, code);
    } else {
       printf("after setting fileset status:\n");
       print_vol_stats(tdesc, &tvolStatus);
    }
}


vol_Fclose(argp)
    char **argp;
{
    int code;
    int low;
    afs_hyper_t volId;

    sscanf(&(*argp)[0], "%u", &low);
    ++argp;    
    AFS_hset64(volId, 0, low);
    code = afs_syscall(AFSCALL_VOL, VOLOP_FCLOSE, (int) &volId, 0, 0, 0);
    printcode (code);
}


vol_SetVVers(argp)
    char **argp;
{
    int code, desc;
    struct vol_SetVV vvInfo;
    u_long timeArg;

    bzero((char *)&vvInfo, sizeof(vvInfo));
    sscanf(&(*argp)[0], "%d", &desc);
    ++argp;
    sscanf(&(*argp)[0], "%x", &vvInfo.vv_Flags);
    ++argp;
    sscanf(&(*argp)[0], "%d", &vvInfo.vv_Mask);
    ++argp;
    sscanf(&(*argp)[0], "%d", &timeArg);
    ++argp;
    vvInfo.vv_Curr.sec = timeArg;
    vvInfo.vv_Curr.usec = 0;
    sscanf(&(*argp)[0], "%d", &timeArg);
    ++argp;
    vvInfo.vv_PingCurr.sec = timeArg;
    vvInfo.vv_PingCurr.usec = 0;
    code = afs_syscall(AFSCALL_VOL, VOLOP_SETVV, desc, (int) &vvInfo, 0, 0);
    printcode (code);
}
