/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: audrel001.c,v $
 * Revision 1.1.5.2  1996/02/17  23:26:29  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:18  marty]
 *
 * Revision 1.1.5.1  1995/12/11  21:54:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:54:58  root]
 * 
 * Revision 1.1.2.10  1994/09/12  18:00:03  bhaim
 * 	Fix intermittent failures with audrel001.c
 * 	[1994/09/12  17:59:41  bhaim]
 * 
 * Revision 1.1.2.9  1994/08/17  17:48:46  bhaim
 * 	fix for 11701
 * 	[1994/08/17  17:48:12  bhaim]
 * 
 * Revision 1.1.2.8  1994/07/29  17:09:38  bhaim
 * 	audrel001 killing auditd before dcecp
 * 	[1994/07/29  17:09:20  bhaim]
 * 
 * Revision 1.1.2.7  1994/06/10  20:47:17  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:18:13  devsrc]
 * 
 * Revision 1.1.2.6  1994/06/07  17:30:50  bhaim
 * 	Insulate from SIGVTALRM
 * 	[1994/06/07  17:30:30  bhaim]
 * 
 * Revision 1.1.2.5  1994/05/23  14:38:18  bhaim
 * 	Use dcecp
 * 	[1994/05/23  14:37:40  bhaim]
 * 
 * Revision 1.1.2.4  1994/04/21  16:01:07  bhaim
 * 	Fix bug 10395
 * 	[1994/04/21  16:00:42  bhaim]
 * 
 * Revision 1.1.2.3  1994/04/07  15:06:45  bhaim
 * 	OT 10307 test fails in small window
 * 	[1994/04/07  15:05:14  bhaim]
 * 
 * Revision 1.1.2.2  1994/04/04  20:19:30  bhaim
 * 	Bring down auditd at end of test
 * 	[1994/04/04  20:18:50  bhaim]
 * 
 * Revision 1.1.2.1  1994/03/25  15:41:55  bhaim
 * 	Add aud prefix
 * 	[1994/03/25  15:41:34  bhaim]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <tet_api.h>
#include <dce/sec_login.h>
#include <dce/dce_error.h>
#include <dce/audit.h>
#include <time.h>


#define CHECK_STATUS(where, status) \
if (*status != 0) { \
    printf("%s returns bad status, test failed, exiting\n", where); \
    tet_result(TET_FAIL); \
    exit(1); \
} 


#define ST_AUD_FIRST_EVT_NUMBER         0x0600
#define EVT_ISSUE  	                0x0600
#define ST_AUD_NUM_OF_EVTS              1


void startup(), cleanup();
void (*tet_startup)() = startup, (*tet_cleanup)() = cleanup;
void tp1();

struct tet_testlist tet_testlist[] = { {tp1,1}, {NULL,0} };

void startup()
{
    char		cmd_line[1024];
    int                 is_running = 0, result;
    int                 i = 0;

    error_status_t 	status;

    if (is_running == 0) {
        is_running=system("env COLUMNS=160 ps -aef | grep -v grep | grep auditd > /dev/null");
        if (is_running == 0) {
		fprintf(stderr,"AUDITD is running on this node\n");
        } else {
		fprintf(stderr,"AUDITD is NOT running on this node\n");
		fprintf(stderr,"Must configure and start auditd first\n");
                tet_infoline("AUDITD is NOT running");
                tet_infoline("Must configure and start auditd first");
		exit(1);
	}
    }
    if (!access("/tmp/auditd_trail",F_OK))
    	system("rm -f /tmp/auditd_trail");
    if (!access("/tmp/auditd_trail.md_index",F_OK))
        system("rm -f /tmp/auditd_trail.md_index");
    if (!access("/opt/dcelocal/etc/audit/ec/systest",F_OK))
        system("rm -f /opt/dcelocal/etc/audit/ec/systest");
    system("cp ./systest /opt/dcelocal/etc/audit/ec/systest");
    if (access("/opt/dcelocal/etc/audit/ec/systest",F_OK))
	{
                tet_infoline("Could not create event controller file systest\n");
                tet_result(TET_FAIL);
		exit(1);
	}

    tet_infoline("Do tst_dce_login");
    tst_dce_login("cell_admin","-dce-",&status);

   if (status != error_status_ok) {
 	   fprintf(stderr,"error %d in tst_dce_login\n", status);
    	   exit(1);
   }


}

void cleanup()
{

}


int verify_count(num)
int num;
{
FILE *f;
char header[80];
int count;

#if 1
if ((f = popen("dcecp -c audtrail show /tmp/auditd_trail | grep -c Dumping", "r")) == NULL)
{
	tet_infoline("could not run dcecp program");
	return(1);
}

if ( fgets(header, 60, f) == NULL)
{
	tet_infoline("no output from dcecp");
	return(1);
}

if ( sscanf(header,"%d", &count) < 1 )
{
	tet_infoline("sscanf failed reading dcecp output");
	return(1);
}



pclose(f);

if ( count == num )
{
	tet_infoline("number of records in audit trail matches expected val");
	return(0);
}
else
{
	tet_infoline("number of records in audit trail does not match expected");
    	tst_tet_printf("There were only %d audit records found",count);
    	tst_tet_printf("Expecting %d audit records",num);
	return(1);
}
#endif
return(0);

}


void tp1()
{
dce_aud_trail_t st_aud_trail;
dce_aud_rec_t   st_aud_rec;
error_status_t stat;
int is_running;
char *duration, *delay;
int  dur, del;
int  trail_count = 0, verify_count();
time_t now, end_time, period;

#if 0
    /* for debugging, to see if auditd has really started */
    is_running=system("env COLUMNS=160 ps -aef | grep -v grep | grep auditd"); 
#endif


    if ((duration = tet_getvar("AUDIT_DURATION")) == NULL)
    {
           tet_infoline("Cannot get environment variable AUDIT_DURATION");
	   tet_result(TET_FAIL);
	   exit(1);
    }

    if ((delay = tet_getvar("AUDIT_EVENT_DELAY")) == NULL)
    {
           tet_infoline("Cannot get environment variable AUDIT_EVENT_DELAY");
	   tet_result(TET_FAIL);
	   exit(1);
    }

   if (sscanf(duration,"%d",&dur) != 1)
    {
           	tet_infoline("Could not obtain duration from string");
	   	tet_result(TET_FAIL);
                exit(1);
    }


   if (sscanf(delay,"%d",&del) != 1)
    {
           	tet_infoline("Could not obtain delay from string");
	   	tet_result(TET_FAIL);
                exit(1);
    }


    tet_infoline("Do dce_aud_open");
    signal(SIGVTALRM, SIG_IGN);
    dce_aud_open(aud_c_trl_open_write, "/tmp/auditd_trail",
        ST_AUD_FIRST_EVT_NUMBER, ST_AUD_NUM_OF_EVTS, &st_aud_trail, &stat);
    CHECK_STATUS("dce_aud_open", &stat);

    if (time(&now) < 0 )
    {
	tet_infoline("Could not obtain initial time");
	tet_result(TET_FAIL);
	exit(1);
    }
    end_time = now + 60*dur;
    while ( now < end_time )
    {
	    period = now + del;
	    sleep(del);
    	    if (time(&now) < 0 )
    	    {
			tet_infoline("Could not obtain time after sleep");
			tet_result(TET_FAIL);
			exit(1);
    	    }
	    while ( now < period )
	    {
    		if (time(&now) < 0 )
    		{
			tet_infoline("Could not obtain time in while loop");
			tet_result(TET_FAIL);
			exit(1);
    		}
	    }
	    tet_infoline("Do dce_aud_start_with_name");
	    dce_aud_start_with_name(EVT_ISSUE, (unsigned_char_t *) "cell_admin", NULL,
                     aud_c_evt_always_log, aud_c_esl_cond_success, &st_aud_rec, &stat);
	    CHECK_STATUS("dce_aud_start_with_name", &stat);
	    tet_infoline("Do dce_aud_commit");
	    dce_aud_commit(st_aud_trail, st_aud_rec, aud_c_evt_always_log, 0, aud_c_esl_cond_success, &stat);
	    tet_infoline("Finished dce_aud_commit");
	    CHECK_STATUS("dce_aud_commit", &stat);
	    tet_infoline("Issued event SUCCESS");
	    trail_count = trail_count + 1;
    	    if (time(&now) < 0 )
    	    {
			tet_infoline("Could not obtain time after event issued");
			tet_result(TET_FAIL);
			exit(1);
    	    }
	    printf("Successfully added to audit trail at %s\n", ctime(&now));
    }
    dce_aud_close(st_aud_trail, &stat);
    CHECK_STATUS("dce_aud_close", &stat);
    /* wait for everything to calm down */
    sleep(5);

    if (!verify_count(trail_count))
    	tet_result(TET_PASS);
    else
        tet_result(TET_FAIL);

}

