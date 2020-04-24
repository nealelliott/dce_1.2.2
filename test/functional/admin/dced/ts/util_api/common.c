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
 * $Log: common.c,v $
 * Revision 1.1.7.3  1996/02/18  00:31:27  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:17:15  marty]
 *
 * Revision 1.1.7.2  1995/12/11  15:20:01  root
 * 	Submit
 * 	[1995/12/11  14:33:35  root]
 * 
 * Revision 1.1.2.3  1994/10/27  20:07:13  rrizzo
 * 	expand copyright
 * 	[1994/10/27  19:39:29  rrizzo]
 * 
 * Revision 1.1.2.2  1994/10/19  20:50:00  max
 * 	fix includes for all reference platforms
 * 	[1994/10/19  20:46:39  max]
 * 
 * Revision 1.1.2.1  1994/10/18  20:55:41  max
 * 	Initial submission of FVTs for dced utility API
 * 	[1994/10/18  20:47:54  max]
 * 
 * $EndLog$
 */
#include <sys/types.h>
#include <sys/time.h>

#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include <dce/sec_login.h>


#include <dtapi_impl.h>

#include <common.h>

#ifdef AIX32
#include <sys/select.h>
#endif

const char * tet_unresolved_c_str = "TET_UNRESOLVED";
const char * tet_pass_c_str       = "TET_PASS";
const char * tet_fail_c_str       = "TET_FAIL";

int setup_failed_g_int = 0;

const char *fifo_name_c_str = "/tmp/dce_util_server_fifo";

char * cell_admin_name_g_str = NULL;
char * cell_admin_pwd_g_str  = NULL;

char * attr_value_0_g_str = "first_value";

sec_login_handle_t    lcontext_g_hdl;

#define SYS_CMD_SZ 240
char syscmd_buf[SYS_CMD_SZ];

#define SND_INFOLINE_SZ 240
char rcv_infoline_buf[SND_INFOLINE_SZ];



int common_setup (void)
{


    sec_login_auth_src_t        auth_src;
    sec_passwd_rec_t            prec;
    error_status_t              st;
    int certify = 0;
    boolean32 valid;
    boolean32 reset_passwd;
    char pwd_buf[200];


    cell_admin_name_g_str = NULL;
    cell_admin_pwd_g_str  = NULL;

    /* initialize our globals */
    cell_admin_name_g_str = tet_getvar( "T_CELL_ADMIN" );
    cell_admin_pwd_g_str  = tet_getvar( "T_CELL_ADMIN_PWD" );

    if ( cell_admin_name_g_str == NULL ) {
	tet_infoline ("Failed setup : T_CELL_ADMIN tet var missing\n");
	setup_failed_g_int = 1;
	return 0;
    }
    if ( cell_admin_pwd_g_str == NULL ) {
	tet_infoline ("Failed setup : T_CELL_ADMIN_PWD tet var missing\n");
	setup_failed_g_int = 1;
	return 0;
    }
    
    /* here we are doing our login */
        
    if (!sec_login_setup_identity((unsigned_char_p_t )cell_admin_name_g_str,
                                 sec_login_no_flags, &lcontext_g_hdl, &st)) {
	tet_infoline( "Failed setup. Error from sec_login_setup_identity\n");
	setup_failed_g_int = 1;
	return 0;
    }

    if (st != sec_rgy_status_ok) {
	tet_infoline( "Failed setup. Error from sec_login_setup_identity\n");
	setup_failed_g_int = 1;
	return 0;
    }
        
    strcpy ( pwd_buf, cell_admin_pwd_g_str);
    
    prec.key.key_type = sec_passwd_plain;
    prec.key.tagged_union.plain = (unsigned char *) pwd_buf;
    prec.pepper = NULL;
    prec.version_number = sec_passwd_c_version_none;
    
    if (certify) {
	valid = sec_login_valid_and_cert_ident(lcontext_g_hdl, &prec,
					       &reset_passwd, &auth_src,
					       &st);
    } else {
	valid = sec_login_validate_identity(lcontext_g_hdl, &prec, &reset_passwd,
					    &auth_src, &st);
    }
    
    if (!valid) {
	tet_infoline( "Failed setup. Error from sec_login_validate_identity\n");
	setup_failed_g_int = 1;
	return 0;
    }
    
    sec_login_set_context(lcontext_g_hdl, &st);
    if ( st != error_status_ok) {
	tet_infoline( "Failed setup. Error from sec_login_set_context\n");
	setup_failed_g_int = 1;
	return 0;
    }


    { /* We want to create the server. Due to time constraints and the fact that
	 this is only a setup function we are shamelessly going to create it with tcl */
	
	sprintf( syscmd_buf, "echo \"source tutil.tcl; tutil_setup %s\" | dcecp ",
		cell_admin_pwd_g_str);
	
	if (system ( syscmd_buf )) {
	    tet_infoline ("Failed to setup server's account an keytab\n");
	    setup_failed_g_int = 1;
	    return 0;
	}
	
    }

    {   /* create the fifo to communicate with the server */
	int ret;

	unlink (fifo_name_c_str);
	ret = mkfifo( fifo_name_c_str, S_IRWXU | S_IRWXG | S_IRWXO );
	if ( ret < 0) {
	    tet_infoline ( "Failed to create fifo for communication with server\n");
	    setup_failed_g_int = 1;
	    return 0;
	}
    }

}




int common_cleanup(void)
{
    error_status_t              st;

    unlink (fifo_name_c_str);

    { 
	strcpy( syscmd_buf, "dcecp -c \"source tutil.tcl; tutil_cleanup \"");
	
	if (system ( syscmd_buf )) {
	    tet_infoline ("Failed to cleanup server's account an keytab\n");
	    tet_result (TET_UNRESOLVED);
	    return 0;
	}
    }

    sec_login_purge_context(&lcontext_g_hdl, &st);

    return 0;
}



int common_server_func (char * func)
{

    int fd;
    
    if (setup_failed_g_int) {
	tet_result (TET_UNRESOLVED);    
	return -1;
    }

    /* need to open the fifo, register the server with the appropriate
       flags, then launch the server, read its output from the fifo,
       send it to tet_infoline and finally delete the server */
    

    /* now open the fifo for reading. We need to open in
       non-blocking mode, otherwise we would get stuck until
       someone opens it for writing.  But then we cannot go
       on and launch the server process */
    fd = open ( fifo_name_c_str, O_RDONLY | O_NDELAY, 0); 
    if ( fd < 0) {
	tet_infoline ( "Failed to open fifo for communication with server\n");
	tet_result ( TET_UNRESOLVED);
	return -1;
    }
    /* now reset the NDELAY flag for the reads */
    fcntl( fd, F_SETFL, fcntl(fd, F_GETFL) & ~O_NDELAY);

    
    /* We want to create the server. Due to time constraints and the fact that
       this is only a setup function we are shamelessly going to create it with tcl */
    
    sprintf( syscmd_buf, "dcecp -c \"source tutil.tcl;"
	    "tutil_run %s {-f %s" " -o %s}\"",
	    attr_value_0_g_str,
	    func,
	    fifo_name_c_str
	    );
	
    if (system ( syscmd_buf )) {
	tet_infoline ("Failed to setup and launch the server\n");
	tet_result (TET_UNRESOLVED);
	return -1;
    }
	
    common_fifo_to_tet_info (fd );

    /* time to stop the server */
    strcpy( syscmd_buf, "dcecp -c \"source tutil.tcl; "
		                        "tutil_stop\"");

    if (system ( syscmd_buf )) {
	tet_infoline ("Failed cleanup\n");
	tet_result (TET_UNRESOLVED);
	return -1;
    }

    close (fd);
}




int common_fifo_to_tet_info ( int fd )
{
    int count, ret, done;
    fd_set  rmask;
    struct timeval timeout;
    int sleep_time;
    int comm_timeout = 100;


    timeout.tv_sec  = 100;
    timeout.tv_usec = 0;
    
    FD_ZERO (&rmask);

    for (done = 0 ; !done; ) {
	count = 0;
	sleep_time = 0;
	while (count < SND_INFOLINE_SZ)  {

#ifdef UNUSED /* there are other problems with the fifo */
	    FD_SET (fd, &rmask);

	    ret = select ( fd + 1 , &rmask, NULL, NULL, &timeout);
	    if (ret <0) {
		sprintf(rcv_infoline_buf, "Error returned by select: %d" , errno );
		tet_infoline (rcv_infoline_buf);
		done = 1;
		break;
	    }

	    if (ret == 0) {
		tet_infoline ("Communication with test server timed out");		
		done = 1;
		break;
	    }
	    if ( ret > 0){
		if (!FD_ISSET(fd, &rmask)) { /* timeout */
		    tet_infoline ("Error returning from select: mask in unexpected state");		
		    done = 1;
		    break;
		}
	    }

	    FD_CLR(fd, &rmask);
#endif

	    ret = read ( fd, &rcv_infoline_buf[count],  SND_INFOLINE_SZ - count);
	    if ( ret < 0 && (errno != EINTR || errno != EAGAIN)) {
		sprintf(rcv_infoline_buf, "Error returned by read: %d" , errno );
		tet_infoline (rcv_infoline_buf);
		done = 1;
		break;
	    }
	    if (ret == 0 ) {
		sleep(3); /* just sleep a bit */
		sleep_time += 3;
		if (sleep_time > comm_timeout) {
		    tet_infoline ( "Communincation with test server timed out");
		    done = 1;
		    break;
		}
	    }
	    if (ret > 0) 
		count += ret;
	}
	if (count > 0) {
	    rcv_infoline_buf[SND_INFOLINE_SZ -1] = '\0'; /* null-terminate to be sure */
	    
	    if ( strcmp ( rcv_infoline_buf, tet_pass_c_str) == 0) {
		tet_result( TET_PASS);
		return 0;
	    }
	    if ( strcmp ( rcv_infoline_buf, tet_fail_c_str) == 0) {
		tet_result( TET_FAIL);
		return 0;
	    }
	    if ( strcmp ( rcv_infoline_buf, tet_unresolved_c_str) == 0) {
		tet_result( TET_UNRESOLVED);
		return 0;
	    }
	    
	    tet_infoline (rcv_infoline_buf);
	}
	else {
	    tet_infoline ("Test server closed down it's end of the fifo");
	    tet_result( TET_UNRESOLVED);
	    return 0;
	}
    }
}


