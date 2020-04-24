/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dtest.c,v $
 * Revision 1.1.73.1  1996/10/02  17:20:35  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:29:42  damon]
 *
 * Revision 1.1.67.3  1994/07/13  22:22:22  devsrc
 * 	merged with bl-10
 * 	[1994/06/28  20:43:46  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:56:52  mbs]
 * 
 * Revision 1.1.67.2  1994/06/09  14:00:12  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:35:29  annie]
 * 
 * Revision 1.1.67.1  1994/02/04  20:13:01  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:10:09  devsrc]
 * 
 * Revision 1.1.65.1  1993/12/07  17:18:35  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  13:55:33  jaffe]
 * 
 * Revision 1.1.5.5  1993/01/19  15:26:06  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  13:46:07  cjd]
 * 
 * Revision 1.1.5.4  1993/01/13  16:41:16  shl
 * 	Transarc delta: bwl-ot6299-dirLength-dead 1.1
 * 	  Selected comments:
 * 	    Dead code.
 * 	    Delete tests of dir_Length.
 * 	[1993/01/12  20:08:24  shl]
 * 
 * Revision 1.1.5.3  1992/11/24  16:24:30  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:22:27  bolinger]
 * 
 * Revision 1.1.5.2  1992/08/31  19:22:55  jaffe
 * 	Fixed RCSID
 * 	[1992/08/30  13:50:27  jaffe]
 * 
 * 	Transarc delta: jdp-epimerge-1.0-to-1.0.1b9-into-2.17 1.20
 * 	  Selected comments:
 * 	    This revision of this delta contains changes made to episode/async by the OSF
 * 	    between dce1.0 and dce1.0.1b9.  Subsequent revisions will contain merges
 * 	    for other episode subdirectories.
 * 	    This revision of this delta contains changes made to episode/dir by the OSF
 * 	    Merged in changes made by the OSF between dce1.0 and dce1.0.1b9.
 * 	    This revision of this delta contains changes made to episode/lobguf by the OSF
 * 	    This revision of this delta contains changes made to episode/salvage by the OSF
 * 	    This revision of this delta contains changes made to episode/sautils by the OSF
 * 	    This revision of this delta contains changes made to episode/tools by the OSF
 * 	    This revision of this delta contains changes made to episode/utils by the OSF
 * 	    This revision of this delta contains changes made to episode/vnops by the OSF
 * 	    between dce1.0 and dce1.0.1b9.
 * 	    This revision fixes some problems in the logbuf layer that occurred after
 * 	    the merge.
 * 	    This delta adds anode/epi_errs.msf.
 * 	    This delta fixes problems in the logbuf layer.
 * 	    This revision fixes some problems in the vnode layer.
 * 	    Fix more vnode layer problems.
 * 	    Add files that were introduced after DCE 1.0 to the .klib Makefiles.
 * 	    This delta fixes various problems that caused undefined references
 * 	    while building the AIX kernel extension.
 * 	    This revision fixes random problems encountered under OSF/1.
 * 	    This revision fixes some problems encountered when compiling test_vnodeops
 * 	    on OSF/1.
 * 	    This revision includes the OSF's changes to epimount.
 * 	Transarc delta: jdp-ot4179-lfs-encapsulate-initialization 1.3
 * 	  Selected comments:
 * 	    This delta simplifies Episode user-space initialization.  There are two parts
 * 	    to this change.  The first simplifies the process of calling the *_Init
 * 	    routines.  Previously, you typically had to call dirb_Init(), N anode init
 * 	    routines, and logbuf_Init().  With this change, you need only specify the
 * 	    initialization parameters to the necessary layers in one structure and call
 * 	    the top-most init routine.  Since each initialization routine is now
 * 	    responsible for calling the lower layers' initialization routines, only
 * 	    one call is necessary.  The second change made in this delta was the
 * 	    encapsulation of the user-space device initialization code in one routine,
 * 	    us_InitDevice().  This routine is declared in tools/us.h and defined in
 * 	    tools/us_io.c.
 * 	    Convert to use the new initialization model.
 * 	    Fix a couple of remaining compilation problems.
 * 	    Fix compilation problems on OSF/1.
 * 	Transarc delta: jess-ot3817-dir.h-funtion-prototype 1.2
 * 	  Selected comments:
 * 	    Files in episode/dir should contain prototypes for external functions.
 * 	    This causes things to break when these interfaces are changed and not all
 * 	    the callers are changed. Fix compilation errors caused by strick type
 * 	    checking.
 * 	    See above.
 * 	    Fixed compilation problem on the PMAX under OSF/1.
 * 	[1992/08/30  02:21:20  jaffe]
 * 
 * Revision 1.1.3.5  1992/05/13  21:58:42  jdp
 * 	Transarc delta: bwl-dir-create-2633 1.1
 * 	  Files modified:
 * 	    dir: dir_buffer.c, dir_buffer.h, dir_main.c, dir_salvage.c
 * 	    dir: dtest.c, test_dir.c; salvage: walk.c
 * 	    vnops: efs_dir.c, efs_vnodeops.c, quota.test
 * 	  Selected comments:
 * 	    Fix bugs in error recovery during directory insertion (create, mkdir, etc.);
 * 	    as a by-product, reorganized directory insertion so that for multi-page
 * 	    directories, only one search through the directory's pages is done.
 * 	    New specs for dir_Create and dir_Lookup.
 * 	[1992/05/13  21:05:41  jdp]
 * 
 * Revision 1.1.3.4  1992/05/05  19:04:54  jdp
 * 	delta fewer-dhop-writes 1.2
 * 	delta jdp-epimerge-1.0-to-1.0.1b9-into-2.17 1.20
 * 	[1992/05/04  21:25:17  jdp]
 * 
 * Revision 1.1.3.3  1992/01/24  03:00:18  devsrc
 * 	Merging dfs6.3
 * 	[1992/01/24  00:09:33  devsrc]
 * 
 * Revision 1.1.3.2  1992/01/23  19:42:43  zeliff
 * 	Moving files onto the branch for dfs6.3 merge
 * 	[1992/01/23  18:28:51  zeliff]
 * 	Revision 1.1.1.3  1992/01/23  22:13:04  devsrc
 * 	Fixed logs
 * 
 * $EndLog$
*/
/*
 * Copyright (C) 1990 Transarc Corporation
 * All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/sysincludes.h>

#include <dcedfs/episode/fid.h>
#include <dcedfs/episode/logbuf.h>
#include <dir.h>
#include <dir_buffer.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/episode/dir/dtest.c,v 1.1.73.1 1996/10/02 17:20:35 damon Exp $")

typedef struct dirhandle {
    struct dh_ops *dh_ops;
    int fd;
    int uniq;
} dirhandle;

void LookupDir(register char *dname, register char *ename);
void AddEntry(register char *dname, register char *ename);
void FixDD(register char *dname);
void ListDir(register char *name); 
void ListEntry(char *name, long vnode, long unique);
void my_printf(char *text);
void CheckDir(char *name);
void SalvageDir(char *iname, char *oname);
void DelTest(char *dname, char *ename);
void CRTest(char *dname, char *ename, int count);
void IsEmptyDir(char *name);
void PScanDir(char *name, int pieceno);
void OpenDir(char *name, dirhandle *dir);
void CreateDir(char *name, dirhandle *dir);

PRIVATE int ReallyRead();
PRIVATE int ReallyWrite();
PRIVATE u_int32 RealLength();
PRIVATE void FidZero();
PRIVATE int FidEq();
PRIVATE void FidCopy();
PRIVATE int FidHash();

struct dh_ops dho = {ReallyRead, ReallyWrite, RealLength,
		     FidHash, FidEq, FidZero, FidCopy};

long fidCounter = 0;

int Uniq;

#define ONE_MEG (1024 * 1024)

main(argc,argv) 
    char **argv;
{
    struct dirb_params dirbParams;
    long code;

    DIRB_ELBB_PARAMS(dirbParams).bufferPool = (char*)malloc(ONE_MEG);
    if (DIRB_ELBB_PARAMS(dirbParams).bufferPool == NULL) {
	fprintf(stderr, "dtest: cannot allocate memory\n");
	return 1;
    }
    DIRB_ELBB_PARAMS(dirbParams).byteCount = ONE_MEG;
    DIRB_EPIG_PARAMS(dirbParams).maxHandles = 1000;
    dirbParams.abuffers = 6;
    if (code = dirb_Init(&dirbParams)) {
	com_err("dtest", code, "during module initialization");
	return 1;
    }
    argc--; argv++;
    switch ((*argv++)[1]) {
	case 'l': ListDir(*argv); break;
	case 'c': CheckDir(*argv); break;
	case 's': SalvageDir(*argv, argv[1]); break;
	case 'f': CRTest(*argv, argv[1], atoi(argv[2])); break;
	case 'd': DelTest(*argv, argv[1]); break;
	case 'r': LookupDir(*argv, argv[1]); break;
	case 'a': AddEntry(*argv, argv[1]); break;
	case 'i': IsEmptyDir(*argv); break;
	case 'p': PScanDir(*argv, atoi(argv[1])); break;
	case 'x': FixDD(*argv); break;
	default: printf("Options are -l (list), -c (check), -s (salvage), \
-f (mkdir and fill), -d (delete), -r (read), -a (add entry), -i (isempty), \
-p (piecescan), -x (fixdotdot)\n");
    }
    exit(0);
}

void LookupDir(
	register char *dname, 
	register char *ename)
{
    dirhandle dir;
    long code;
    epi_volFileId_t fid;
    long offset;

    OpenDir(dname, &dir);
    code = dir_Lookup((struct Dir_Handle *)&dir, ename, &fid, &offset);
    if (code) printf("lookup code %d\n", code);
    else {
	printf("Found fid %d.%d for file '%s'\n",
		epid_ToIndex (&fid), epid_ToUnique (&fid), ename);
    }
}

void AddEntry(register char *dname, register char *ename)
{
    dirhandle dir;
    long code;
    epi_volFileId_t fid;
    long offset;

    OpenDir(dname, &dir);
    code = dir_Lookup ((struct Dir_Handle *)&dir, ename, &fid, &offset);
    if (!code) code = EEXIST;
    if (code == ENOENT) {
	epid_Assemble (&fid, fidCounter, 3);
	fidCounter++;
	code = dir_Create((struct Dir_Handle *) &dir, ename, &fid, offset, 0, 0);
    }
    if (code) printf("create code %d\n", code);
}

void FixDD( register char *dname)
{
    dirhandle dir;
    long code;
    epi_volFileId_t fid;

    epid_Assemble (&fid, fidCounter, 9);
    fidCounter++;
    OpenDir(dname, &dir);
    code = dir_FixDotDot((struct Dir_Handle *)&dir, &fid, 0);
    if (code) printf("fixdotdot code %d\n", code);
}

void ListDir(char *name) 
{
    dirhandle dir;
    u_long pieceno;
    struct Dir_Entry *entP;
    struct Dir_Page *dpP;
    OpenDir(name, &dir);
    pieceno = 0;
    while ((pieceno = dir_PieceScan((struct Dir_Handle *)&dir, pieceno)) != 0) {
	entP = (dir_GetPiece((struct Dir_Handle *)&dir,pieceno,&dpP));
	ListEntry((char *)entP->name, epid_ToIndex (&entP->fid), epid_ToUnique (&entP->fid));
	pieceno += entP->length;
	dirb_Release((caddr_t)dpP);
    }
}

void ListEntry(char *name, long vnode, long unique)
{
    printf("%s\t%d\t%d\n", name, vnode, unique);
}

void my_printf(char *text)
{
    printf(text);
}

void CheckDir(char *name)
{
    dirhandle dir;
    OpenDir(name, &dir);
    if (dirs_OK((struct Dir_Handle *)&dir, (void (*)())printf))
	printf("Directory ok.\n");
    else printf ("Directory bad\n");
}

void SalvageDir(char * iname, char * oname)
{
    dirhandle in,out;
    OpenDir(iname, &in);
    CreateDir(oname, &out);
    dirs_Salvage((struct Dir_Handle *)&in, (struct Dir_Handle *)&out,
		 (void (*)())printf);
}

void DelTest(char *dname,char * ename)
{
    dirhandle dir;
    register long code;
    epi_volFileId_t fid;
    long offset;

    OpenDir(dname, &dir);
    code = dir_Lookup((struct Dir_Handle *)&dir, ename, &fid, &offset);
    if (!code) code = dir_Delete((struct Dir_Handle *)&dir, offset, 0);
    if (code) printf("delete code is %d\n", code);
	else printf("entry deleted:\t%d\t%d\n",
		    epid_ToIndex (&fid), epid_ToUnique (&fid));
}

void CRTest(char *dname,char * ename,int count)
{
    char tbuffer[200];
    register long i, code;
    epi_volFileId_t fid, dummyfid;
    dirhandle dir;
    long offset;

    CreateDir(dname, &dir);
    bzero((char *)&fid, sizeof(fid));
    dir_MakeDir((struct Dir_Handle *)&dir, &fid, &fid, 0);
    for(i=0;i<count;i++) {
	sprintf(tbuffer, "%s%d", ename, i);
	epid_Assemble (&fid, fidCounter, 2);
	fidCounter++;
	code = dir_Lookup((struct Dir_Handle *)&dir, tbuffer, &dummyfid, &offset);
	code = dir_Create((struct Dir_Handle *)&dir, tbuffer, &fid, offset, 0, 0);
	if (i % 100 == 0) {
	    printf("#");
	    fflush(stdout);
	}
	if (code) {
	    printf("code for '%s' is %d\n", tbuffer, code);
	    return;
	}
    }
}

void IsEmptyDir(char *name)
{
    dirhandle dir;
    OpenDir(name, &dir);
    if (dir_IsEmpty((struct Dir_Handle *)&dir))
	printf("Empty.\n");
    else
	printf("Not empty.\n");
}

void PScanDir(char *name, int pieceno)
{
    dirhandle dir;
    OpenDir(name, &dir);
    printf("New piece number: %d\n", dir_PieceScan((struct Dir_Handle *)&dir, pieceno));
}

void OpenDir(char *name, dirhandle *dir)
{
    dir->dh_ops = &dho;
    dir->fd = open(name, O_RDWR);
    if (dir->fd == -1) {
	printf("Couldn't open %s\n", name);
	exit(1);
    }
    dir->uniq = ++Uniq;
}

void CreateDir(char *name, dirhandle *dir)
{
    dir->dh_ops = &dho;
    dir->fd = open(name, O_CREAT|O_RDWR|O_TRUNC, 0666);
    dir->uniq = ++Uniq;
    if (dir->fd == -1) {
	printf("Couldn't create %s\n", name);
	exit(1);
    }
 }

PRIVATE int ReallyRead(dir, addr, data, len)
    dirhandle *dir;
    int addr;
    char *data;
    int len;
{
    if (lseek(dir->fd, addr, 0) == -1)
	return 0;
    return (read(dir->fd, data, len) == len);
}

PRIVATE int ReallyWrite(dir, tranId, addr, data, len)
    dirhandle *dir;
    logTranRec_t tranId;
    int addr;
    char *data;
    int len;
{
    if (lseek(dir->fd, addr, 0) == -1)
	return errno;
    if (write(dir->fd, data, len) == -1)
	return errno;
    return 0;
}

PRIVATE u_int32 RealLength(dir)
    dirhandle *dir;
{
    struct stat stbuf;

    fstat(dir->fd, &stbuf);
    return stbuf.st_size;
}

PRIVATE void FidZero(dir)
    dirhandle *dir;
{
    dir->fd = -1;
}

PRIVATE int FidEq(dir1, dir2)
    dirhandle *dir1, *dir2;
{
    return (dir1->uniq == dir2->uniq);
}

PRIVATE void FidCopy (todir, fromdir)
    dirhandle *todir, *fromdir;
{
    *todir = *fromdir;
}

PRIVATE int FidHash (dir, page)
    dirhandle *dir;
{
    return dir->fd + page;
}

void Die(msg)
    char *msg;
{
    printf("Something died with this message:  %s\n", msg);
}

void Log(a,b,c,d,e,f,g,h,i,j,k,l,m,n) {
	printf((char *)a,b,c,d,e,f,g,h,i,j,k,l,m,n);
}
