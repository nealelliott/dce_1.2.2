/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rprog1.c,v $
 * Revision 1.1.124.1  1996/10/17  18:29:41  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:54:55  damon]
 *
 * Revision 1.1.119.1  1994/02/04  20:47:14  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:25:26  devsrc]
 * 
 * Revision 1.1.117.1  1993/12/07  17:46:12  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:39:31  jaffe]
 * 
 * Revision 1.1.10.2  1993/07/19  19:44:52  zeliff
 * 	HP port of DFS
 * 	[1993/07/19  18:33:38  zeliff]
 * 
 * Revision 1.1.8.3  1993/07/16  22:14:15  kissel
 * 	Get rid of the damn quotes in #include.
 * 	[1993/06/21  15:12:20  kissel]
 * 
 * Revision 1.1.5.5  1993/03/09  21:23:46  jaffe
 * 	Transarc delta: jess-ot7194-fs-low-kerneldump 1.1
 * 	  Selected comments:
 * 	    Change the icl_KernelDump parameter according to the changes
 * 	    from icl utilities.
 * 	    See above.
 * 	[1993/03/09  20:58:08  jaffe]
 * 
 * Revision 1.1.5.4  1993/01/11  18:33:41  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:16:14  htf]
 * 
 * Revision 1.1.5.3  1992/11/24  21:46:38  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:46:48  bolinger]
 * 
 * Revision 1.1.5.2  1992/10/28  18:13:56  jaffe
 * 	Transarc delta: jess-add-dumpkernel-in-low-tests 1.5
 * 	  Selected comments:
 * 	    Add function call (icl_DumpKernel) in the test suite. In case of running
 * 	    error of C programmes in the test suite the "icl_DumpKernel" function
 * 	    will be called in errex() to do the kernel trace. The dump file is located
 * 	    in "/tmp/prog#.kernel.dump.pid". You have to have the debug flag set to
 * 	    have the dump file.
 * 	    See above.
 * 	    Process another C file and add "low" as dump file header.
 * 	    Turn on the prog3 debug flags.
 * 	    It should not create the zero length dumpfile when there is nothing wrong.
 * 	    Fix the problem.
 * 	[1992/10/28  14:37:02  jaffe]
 * 
 * Revision 1.1.3.3  1992/01/24  04:15:42  devsrc
 * 	Merging dfs6.3
 * 	[1992/01/24  00:24:08  devsrc]
 * 
 * Revision 1.1.3.2  1992/01/23  20:31:59  zeliff
 * 	Moving files onto the branch for dfs6.3 merge
 * 	[1992/01/23  18:49:03  zeliff]
 * 	Revision 1.1.1.3  1992/01/23  22:26:46  devsrc
 * 	Fixed logs
 * 
 * $EndLog$
*/
/*****************************************************************************/
/* Copyright (C) 1990 Transarc Corporation - All rights reserved */
/*****************************************************************************/

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <common.h>
#include <dcedfs/icl.h>
#include <string.h>



#define BUFSIZE    1024
#define NR_LEVELS  5

int debug=FALSE;

void errex(), errex0();

extern int errno;

/*****************************************************************************/

main(argc,argv) int argc; char *argv[];
{
char buf[BUFSIZE],*dir="tdir.r1",fn[21],subd[256],wd[256],z[256],z1[256];
int fd,i,l;
struct stat s;

setbuf(stdout,(char *)NULL);


printf("Change-root tests.\n");

printf("\nTesting chroot().\n");
(void)getwd(wd);

strcpy(subd,wd);
for(i=0; i<=NR_LEVELS; ++i)
   {
   if(i == 0) strcpy(fn,dir);
   else sprintf(fn,"dir_%d",i);
   if(mkdir(fn,0777)) errex("mkdir(1)");
   if(chdir(fn)) errex("chdir(1)");
   strcat(subd,"/"); strcat(subd,fn);
   (void)getwd(z);
   if(!EQ(subd,z)) errex0("getwd compare(1)");
   if(debug) printf("   Made and moved to %s.\n",z);

   sprintf(fn,"file_%d",i);
   if((fd=open(fn,O_WRONLY|O_CREAT,0644)) < 0) errex("open(1)");
   sprintf(z,"This is %s.",fn); l=strlen(z);
   if(write(fd,z,(RWA3)(l+1)) != l+1) errex("write(1)");
   if(close(fd)) errex("close(1)");
   if(debug) printf("   Wrote %s.\n",fn);
   }

if(debug) printf("\nReady to do chroot().\n");
if(chroot(wd)) errex("chroot(1)");
if(chdir("/")) errex("chdir(2)");
(void)getwd(z);
if(!EQ(z,"/")) errex0("getwd compare(2)");
if(debug) printf("Did chroot() to %s and moved to new root.\n\n",wd);

*subd = '\0';
for(i=0; i<=NR_LEVELS; ++i)
   {
   if(i == 0) strcpy(fn,dir);
   else sprintf(fn,"dir_%d",i);
   if(chdir(fn)) errex("chdir(3)");
   strcat(subd,"/"); strcat(subd,fn);
   (void)getwd(z);
   if(!EQ(subd,z)) errex0("getwd compare(3)");
   if(debug) printf("   Moved to %s.\n",z);
   }
for(i=NR_LEVELS; i>=0; --i)
   {
   sprintf(fn,"file_%d",i);
   if((fd=open(fn,O_RDONLY)) < 0) errex("open(2)");
   sprintf(z,"This is %s.",fn); l=strlen(z);
   if(read(fd,z1,(RWA3)sizeof(z1)) < 0) errex("read(1)");
   if(close(fd)) errex("close(2)");
   if(unlink(fn) != 0) errex("unlink(1)");
   if(debug) printf("   Read and unlinked %s.\n",fn);

   if(chdir("..")) errex("chdir(4)");
   if(i == 0) strcpy(fn,dir);
   else sprintf(fn,"dir_%d",i);
   if(rmdir(fn) != 0) errex("rmdir(1)");
   if(debug) printf("   Unlinked %s.\n",fn);
   }

(void)getwd(z);
if(debug) printf("   Ended up back in %s.\n",z);
if(!EQ(z,"/")) errex0("getwd compare(4)");

printf("Done testing chroot().\n");

printf("\nDone with change-root tests.\n");
exit(0);
}

/*****************************************************************************/
void errex(s) char *s;
{
        FILE *dumpfilep;
        char dumpfile[256];

        printf("\n%s failed; errno = %d\n",s,errno);
        if (debug) {
            sprintf(dumpfile,"/tmp/low.rprog1.kernel.dump.%d",getpid());
            dumpfilep = fopen(dumpfile, "w");
            icl_DumpKernel(dumpfilep, NULL, ICL_DUMP_FORMATTED);
            fclose(dumpfilep);
        }
        exit(1);
}

void errex0(s) char *s;
{
        FILE *dumpfilep;
        char dumpfile[256];

        printf("\n%s failed\n",s);
        if (debug) {
            sprintf(dumpfile,"/tmp/low.rprog1.kernel.dump.%d",getpid());
            dumpfilep = fopen(dumpfile, "w");
            icl_DumpKernel(dumpfilep, NULL, ICL_DUMP_FORMATTED);
            fclose(dumpfilep);
        }
        exit(1);
}


/*****************************************************************************/
