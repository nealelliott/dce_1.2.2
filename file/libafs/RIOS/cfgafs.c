/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cfgafs.c,v $
 * Revision 1.1.51.1  1996/10/02  17:53:57  damon
 * 	New DFS from Transarc
 * 	[1996/10/01  18:58:32  damon]
 *
 * Revision 1.1.46.2  1994/06/09  14:12:49  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:26:07  annie]
 * 
 * Revision 1.1.46.1  1994/02/04  20:22:21  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:14:45  devsrc]
 * 
 * Revision 1.1.44.1  1993/12/07  17:27:49  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  15:46:40  jaffe]
 * 
 * Revision 1.1.3.4  1993/01/19  16:07:06  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  15:10:30  cjd]
 * 
 * Revision 1.1.3.3  1993/01/13  18:16:55  shl
 * 	Transarc delta: rajesh-ot6571-cfgdfs-prints-already-loaded-ext-message 1.2
 * 	  Selected comments:
 * 	    Make cfgdfs print a message if the kernel extension has already been
 * 	    loaded like cfglfs does instead of exiting silently.
 * 	    Add brackets for the if statement that checks to see if the
 * 	    extension is already loaded.
 * 	[1993/01/12  21:31:19  shl]
 * 
 * Revision 1.1.3.2  1992/12/03  23:15:50  delgado
 * 	try again...
 * 	[1992/12/03  23:15:05  delgado]
 * 
 * 	Add support for kprc helper driver
 * 	[1992/12/03  21:07:00  delgado]
 * 
 * Revision 1.1  1992/01/19  02:54:52  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * $Header: /u0/rcs_trees/dce/rcs/file/libafs/RIOS/cfgafs.c,v 1.1.51.1 1996/10/02 17:53:57 damon Exp $
 * $Locker:  $
 *
 * Copyright (c) 1990, Transarc Corporation.  All rights reserved.
 *
 * cfgafs -	load/configure the AFS kernel extension
 */
#include <dcedfs/osi.h>
#include <dcedfs/osi_device.h>
#include <fcntl.h>
#include <sys/sysconfig.h>
#include <sys/uio.h>
#include <sys/ldr.h>
#include <setjmp.h>
#include <cf.h>
#include <sys/cfgdb.h>
#include <sys/cfgodm.h>
#include <dce/ker/krpc_helper.h>

static long cfg_krpchdd();

extern int	sysconfig(int cmd, void *arg, int len);

main(argc, argv)
char **argv; {
    register add;
    register c;
    int res;
    int i;
    char *file;
    mid_t kmid;
    struct cfg_load cload;
    struct cfg_kmod cmod;
    long krpchdd_major;

    struct sym_table {
	char * ext_name;
	char * sym_name;
    } sym_table[] = {
	{"dfscore.ext", "dfscore_kmid"},
	{"dfscmfx.ext", "dfscmfx_kmid"},
	{"dcelfs.ext", "dcelfs_kmid"},
	{"",""}
	};
   
    osi_setlocale(LC_ALL, ""); 
    add = 0;
    
    while ((c = getopt(argc, argv, "a:")) != EOF) {
	switch (c) {
	  case 'a':
	    add  = 1;
	    file = optarg;
	    if (!file)
		usage();
	    break;
	    
	  default:
	    usage();
	    break;
	}
    }
    
    if (add) {
	char *buf[1024];

	/* Determine if extension is already loaded */
	for (i=0; sym_table[i].ext_name[0] != '\0'; i++) {
	    if (strstr (file, sym_table[i].ext_name)) {
		if (dfs_check_load(sym_table[i].sym_name)) {
		    printf("%s: %s already loaded\n",
			   argv[0], sym_table[i].sym_name);
		    exit(0);	/* already loaded */
		}
		break;		/* finished search */
	    }
	}
		
	buf[0] = "execerror";
	buf[1] = "cfgdfs";
	cload.path = file;
	res = sysconfig(SYS_KLOAD, &cload, sizeof(cload));
	if (res != 0) {
	    perror("SYS_KLOAD");
	    loadquery(L_GETMESSAGES, &buf[2], sizeof buf - 8);
	    execvp("/etc/execerror", buf);
	    exit(1);
	}
	
	cmod.kmid   = cload.kmid;
	cmod.cmd    = CFG_INIT;
	cmod.mdiptr = 0;
	cmod.mdilen = 0;

	if (strstr(file, "dfscmfx.ext")) {
	    if ((krpchdd_major = cfg_krpchdd()) > 0) {
		cmod.mdiptr = (caddr_t) &krpchdd_major;
		cmod.mdilen = sizeof(krpchdd_major);
	    } else
		exit(1);
	}
	
	res = sysconfig(SYS_CFGKMOD, &cmod, sizeof(cmod));
	if (res != 0) {
	    perror("SYS_CFGKMOD");
	    cload.kmid = cload.kmid;
	    sysconfig(SYS_KULOAD, &cload, sizeof(cload));
	    exit(1);
	}
	exit(0);
    } else
	usage();

}

usage() {
    
    fprintf(stderr, "usage: cfgdfs [-a mod_file]\n");
    exit(1);
}


static long
cfg_krpchdd()
{
    long major_num;
    int rc;

    unlink(KRPCH_DEV);
    if ((rc = odm_initialize()) != -1) {
	major_num = genmajor("krpchdd");
	odm_terminate();
    }
    if ((rc == 0) && (major_num > 0)) {
	rc = mknod(KRPCH_DEV, _S_IFCHR|0600, makedev(major_num, 0));
    }
    if ((rc == 0) && (major_num > 0))
	return(major_num);
    else
	return (-1);
}
