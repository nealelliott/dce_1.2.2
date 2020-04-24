/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cfgefs.c,v $
 * Revision 1.1.63.1  1996/10/02  17:24:26  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:32:04  damon]
 *
 * Revision 1.1.58.2  1994/06/09  14:00:33  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:35:51  annie]
 * 
 * Revision 1.1.58.1  1994/02/04  20:13:22  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:10:24  devsrc]
 * 
 * Revision 1.1.56.1  1993/12/07  17:19:02  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  13:58:32  jaffe]
 * 
 * Revision 1.1.4.3  1993/01/19  15:26:53  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  13:47:16  cjd]
 * 
 * Revision 1.1.4.2  1992/09/15  17:36:29  jaffe
 * 	Transarc delta: jaffe-ot4455-cfglfs-print-warning-if-already-loaded 1.1
 * 	  Selected comments:
 * 	    print a warning if the kernel extension is already loaded.
 * 	[1992/09/14  20:27:57  jaffe]
 * 
 * Revision 1.1.2.2  1992/05/05  19:07:38  jdp
 * 	delta jdp-epimerge-add-libefs 1.2
 * 	[1992/05/04  21:47:12  jdp]
 * 
 * Revision 1.1  1992/01/19  02:39:22  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * load/configure the episode kernel extension
 */
#include <dcedfs/osi.h>
#include <fcntl.h>
#include <dcedfs/osi_device.h>
#include <dcedfs/episode/efs_opcode.h>
#include <sys/sysconfig.h>
#include <sys/uio.h>
#include <sys/ldr.h>
#include <setjmp.h>

extern int	sysconfig(int cmd, void *arg, int len);

long	parms[10];
char	*loadfile;	
int	add;

main(argc, argv)
    char **argv; 
{
    register c;
    mid_t kmid;
    struct cfg_load cload;
    struct cfg_kmod cmod;

    osi_setlocale(LC_ALL, "");
    /* set default parameters */

    parms[0] = EFS_DEFAULT_BUFFER_POOL_SIZE;
    parms[1] = EFS_DEFAULT_ANODE_TABLE_SIZE;
    parms[2] = EFS_DEFAULT_DIR_BUFFER_POOL_SIZE;
    parms[3] = EFS_DEFAULT_VNODE_TABLE_SIZE;
    parms[4] = EFS_DEFAULT_FLOCK_POOL_SIZE;
    parms[5] = 1;				/* single, or remote */
    
    add = 0;

    if (getargs(argc, argv) != 0)
	exit(1);

    if (add) {

	if (dfs_check_load("dcelfs_kmid")) {
		printf("%s: dcelfs already loaded\n", argv[0]);
		exit(0);
	}

	cload.path = loadfile;
	if (sysconfig(SYS_KLOAD, &cload, sizeof(cload)) == -1) {
	    char *buf[1024];

	    buf[0] = "execerror";
	    buf[1] = "cfgefs";
	    loadquery(L_GETMESSAGES, &buf[2], sizeof buf - 8);
	    execvp("/etc/execerror", buf);
	    exit(1);
	}
	
	cmod.kmid   = cload.kmid;
	cmod.cmd    = CFG_INIT;
	cmod.mdiptr = (caddr_t) parms;
	cmod.mdilen = sizeof(parms);
	
	if (sysconfig(SYS_CFGKMOD, &cmod, sizeof(cmod)) == -1) {
	    perror("SYS_CFGKMOD");
	    cload.kmid = cload.kmid;
	    sysconfig(SYS_KULOAD, &cload, sizeof(cload));
	    exit(1);
	}
	exit(0);
    }
}

/*
 * getargs -- decipher the argument list
 *
 * Return codes:
 *  2 -- syntax error
 *  0 -- success
 */

getargs(argc, argv)
    int argc;
    char *argv[];
{

    int ac;				/* argument counter */
    int anyminus = 0;                   /* minus sign seen yet? */

    static char UsageArgs[] =
"-a <filename> [-single] [-remote] [-bcache <1K blocks>] [-vnodes <num vnodes>] \
[-anodes <num anodes>] [-dircache <buffers>] [-locks <num locks>]";

    for (ac = 1; ac < argc; ++ac) {
	if (anyminus && argv[ac][0] != '-' && ac < (argc - 1)) {
	    printf("Unidentified argument: ``%s''\nUsage: %s %s\n",
		   argv[ac], argv[0], UsageArgs);
	    return (2);
	}

	if (argv[ac][0] == '-') anyminus = 1;

	if (strcmp (argv[ac], "-a") == 0) {
	    add = 1;
	    ++ac;
	    loadfile = argv[ac];
	    if (!loadfile) {
		printf("Missing filename: \nUsage: %s %s\n",
			argv[0], UsageArgs);
		return(2);
	    }
	} else if (strcmp (argv[ac], "-bcache") == 0) {
	    ++ac;
	    parms[0] = atoi(argv[ac]) << 10;
	} else if (strcmp (argv[ac], "-dircache") == 0) {
	    ++ac;
	    parms[2] = atoi(argv[ac]);
	} else if (strcmp (argv[ac], "-vnodes") == 0) {
	    ++ac;
	    parms[3] = atoi(argv[ac]);
	} else if (strcmp (argv[ac], "-locks") == 0) {
	    ++ac;
	    parms[4] = atoi(argv[ac]);
	} else if (strcmp (argv[ac], "-anodes") == 0) {
	    ++ac;
	    parms[1] = atoi(argv[ac]);
	} else if (strcmp (argv[ac], "-single") == 0) {
	    parms[5] = 1;
	} else if (strcmp (argv[ac], "-remote") == 0) {
	    parms[5] = 0;
	} else {
	    printf("Bad option: ``%s''. Usage: %s %s\n",
		   argv[ac], argv[0], UsageArgs);
	    return (2);
	}
    }

    if (!add) {
	fprintf (stderr, "Bad args\nUsage: %s %s\n", argv[0], UsageArgs);
	return (2);
    }
    return(0);
}
