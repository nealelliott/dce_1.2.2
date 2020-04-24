/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: passwd_export_grp_override.c,v $
 * Revision 1.1.2.2  1996/02/17  23:11:56  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:26  marty]
 *
 * Revision 1.1.2.1  1995/12/11  19:53:54  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/01/27  14:58 UTC  mdf
 * 	Functional test changes
 * 
 * 	HP revision /main/HPDCE02/mdf_MOTHRA_2/1  1995/01/23  16:16 UTC  mdf
 * 	Changed the case of pass and fail so that the nightly tests will
 * 	notice failures
 * 
 * 	HP revision /main/HPDCE02/1  1995/01/20  13:42 UTC  mdf
 * 	Correct the location of GSSAPI functional tests
 * 
 * 	HP revision /main/mdf_MOTHRA_1/1  1995/01/19  18:39 UTC  mdf
 * 	New location
 * 	[1995/12/11  19:27:25  root]
 * 
 * Revision 1.1.2.2  1994/07/06  18:14:53  hasbrouc
 * 	"TETize"
 * 	[1994/07/06  18:13:54  hasbrouc]
 * 
 * Revision 1.1.2.1  1994/05/06  13:50:51  hasbrouc
 * 	not fully TET-ized; cleanup later
 * 	[1994/05/06  13:48:42  hasbrouc]
 * 
 * $EndLog$
 */

/*
*
*  Run passwd_export and check to make sure that /opt/dcelocal/group_override
*  stuff gets propogated correctly to /tmp/group (/etc/group).  This IS NOT
*  a test of passwd_export; it is a test of group_override support.
*
*/

#include <stdio.h>
#include <sys/file.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <sys/param.h>
#include <locale.h>
#include <tet_api.h>

#include <dce/binding.h>
#include <dce/acct.h>
#include <dce/override.h>
#include <dce/misc.h>
#include <dce/pgo.h>
#include <dce/rpcbase.h>
#include <dce/dce_error.h>

/* TET stuff */

void (*tet_startup)() = NULL, (*tet_cleanup)() = NULL;
void tp1 ();
struct tet_testlist tet_testlist[] = { {tp1,1}, {NULL,0}};

FILE         *fp1;
FILE         *fp2;
char         *grpfnm = "/opt/dcelocal/etc/group_override";
char         *tmpfiletst = "/tmp/group";
char         *tmpdir = "/tmp";
char         *grepfile = "/tmp/grep_tmp";
char         *tmpstrg;

void check_result
#ifndef __STDC__
    (expect, max_chars)
    char            *expect;
    int              max_chars;
#else
  (
    char            *expect,
    int             max_chars
  )
#endif
{
    char         cmd[2000];
    error_status_t s;
    char         tmpstr1[1000];

    sprintf(cmd, "fgrep \"%s\" %s > %s", expect, tmpfiletst, grepfile);
    s = system(cmd);
    if (s != 0){
	fprintf(stderr, "couldn't run '%s' command - exit status %d\n", cmd,s);
	tet_result(TET_FAIL);
	exit(1);
    }
    fp1 = fopen(grepfile,"r");
    s = fscanf(fp1, "%s", &tmpstr1[0]);

    if ((strncmp(expect,(char *) tmpstr1, strlen(expect))) || (strlen(tmpstr1) > max_chars)) {
	printf("FAILED expected: %s, got %s\n", expect, (char *) tmpstr1);
	tet_result(TET_FAIL);
    } else
	{
	    printf("PASSED check: %s in line: %s in file: %s\n", expect, (char *) tmpstr1, tmpfiletst);
	    tet_result(TET_PASS);
	}
}


void tp1 ()
{
    char         cmd[80];
    char         tmpstr1[80];
    error_status_t s;
    boolean32    grpoverexistsalready;

    tet_infoline("start of passwd_export group_override test");
    if ((fp1 = fopen(tmpfiletst, "r")) == NULL) {
	if  ((fp2 = fopen(grpfnm, "r")) == NULL) {
	    grpoverexistsalready = false;
	    sprintf(cmd,"cp group_override /opt/dcelocal/etc");
	    s = system(cmd);
	    if (s != 0){
		fprintf(stderr, "couldn't run '%s' command - exit status %d\n", cmd,s);
		tet_result(TET_FAIL);
		exit(1);
	    }
	} else {
	    grpoverexistsalready = true;
	    sprintf(cmd,"mv %s /opt/dcelocal/etc/grp_oride_saveit", grpfnm);
	    s = system(cmd);
	    if (s != 0){
		fprintf(stderr, "couldn't run '%s' command - exit status %d\n", cmd,s);
		tet_result(TET_FAIL);
		exit(1);
	    }
	    sprintf(cmd,"cp group_override /opt/dcelocal/etc");
	    s = system(cmd);
	    if (s != 0){
		fprintf(stderr, "couldn't run '%s' command - exit status %d\n", cmd,s);
		tet_result(TET_FAIL);
		exit(1);
	    }
	}
	sprintf(cmd,"passwd_export -d %s", tmpdir);
	s = system(cmd);
	if (s != 0){
	    fprintf(stderr, "couldn't run '%s' command - exit status %d\n", cmd,s);
	    tet_result(TET_FAIL);
	    exit(1);
	}

	check_result("kmem::333", 1024);

	check_result("nogroup::555:a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9", 1024);

	check_result("bin::444:", strlen("bin::444:"));

	sprintf(cmd,"rm -f %s", tmpfiletst);
	s = system(cmd);
	if (s != 0){
	    fprintf(stderr, "couldn't run '%s' command - exit status %d\n", cmd,s);
	    exit(1);
	}

	/* clean up */
	
	if (grpoverexistsalready == true){
	    sprintf(cmd,"rm /opt/dcelocal/etc/group_override");
	    s = system(cmd);
	    sprintf(cmd,"mv /opt/dcelocal/etc/grp_oride_saveit /opt/dcelocal/etc/group_override");
	    s = system(cmd);
	} else {
	    sprintf(cmd,"rm /opt/dcelocal/etc/group_override");
	    s = system(cmd);
	}

	
    } else {
	printf("/tmp/group already exists - can't run test\n");
    }
    tet_infoline("end of passwd_export group_override test");
}


