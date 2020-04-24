/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: pkss_config_common.cxx,v $
 * Revision 1.1.2.1  1996/10/04  14:21:24  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/04  13:37:00  arvind]
 *
 * Revision /main/DCE_1.2.2/1  1996/09/09  21:28 UTC  arvind
 * 	PKSS drop from DEC (DCE1.2.2)
 * 	[1996/08/30  15:48 UTC  arvind  /main/arvind_pkss/1]
 * 
 * $EndLog$
 */
/* (c) Copyright 1996,
 * Digital Equipment Corporation, Maynard, Massachusetts, USA
 * All Rights Reserved
 *
 */

#include <pkss_config_common.h>
#include <pkss_config_int.h>
#include <dce/dce_cf.h>

#ifdef DEBUG
#include <stdio.h>
#endif

/* PKSS configuration routines, common to client and server.		    */


#define SERVICE_PROFILE_PARAM	    "cds.service_profile_name"
#define SERVER_GROUP_PARAM	    "security.server_group_name"


char * pkss__this_host = NULL;

char * default_service_profile_name = "/.:/cell-profile";
char * default_server_group_name = "/.:/subsys/dce/pkss-servers";


pthread_once_t configOnceBlock = pthread_once_init;
char initialized = 0;

static void pkss_once_routine(void) {
    pthread_mutex_init(&init_mutex, pthread_mutexattr_default);
    initialized = 1;
}

unsigned32 pkss__config_initialize(const char * imagename) {
    unsigned32 status;
    int mstatus;
    pthread_once(&configOnceBlock,
                 pkss_once_routine);
    mstatus = pthread_mutex_trylock(&init_mutex);
    if (mstatus == 0) return pks_s_already_initialized;

    dns_cfginit(PKSS_CONF_FILE, (char *)imagename);
    
    dce_cf_get_host_name(&pkss__this_host, &status);

    return status;
}

char * service_profile_name = NULL;
char * server_group_name = NULL;

static int cell_name_length(const char * c) {
// Return the length of any cell part of the name.
    int i;
    if (c[0] != '/') return 0;
    i = 1;
    while ((c[i] != 0) && (c[i] != '/')) i++;
    if (c[i] == '/') return i+1;
    return 1;
}

unsigned32 pkss_load_config(char * src, char * &dst, int absolute) {
    if (absolute==1) {
	if (cell_name_length(src) == 0) {
	    dst = (char *) malloc(strlen(src)+5);
	    if (dst == NULL) return pks_s_no_memory;
	    strcpy(dst, "/.:/");
	    strcat(dst, src);
	} else {
	    dst = (char *) malloc(strlen(src)+1);
	    if (dst == NULL) return pks_s_no_memory;
	    strcpy(dst, src);
	};
    } else if (absolute==0) {
	int cell_part = cell_name_length(src);
	dst = (char *) malloc(strlen(src)+1-cell_part);
	if (dst == NULL) return pks_s_no_memory;
	strcpy(dst, &src[cell_part]);
    } else {
	dst = (char *) malloc(strlen(src)+1);
	if (dst == NULL) return pks_s_no_memory;
	strcpy(dst, src);
    };

    return 0;
}

unsigned32 pkss_config__init(const char * imagename) {
    unsigned32 status;
    char * file;

    status = pkss__config_initialize(imagename);
    if ((status != 0) && (status != pks_s_already_initialized)) {
#ifdef DEBUG
	fprintf(stderr,
                "DEBUG: pkss__config_initialize(%s) returned %X\n",
                imagename,
                status);
        fflush(stderr);
#endif
	return status;
    };
    
    if ((file = dns_cfgopen(NULL, NULL)) == NULL) return pks_s_no_config_file;

    status = pkss_load_config(dns_cfgtostr(file, 
                                           SERVICE_PROFILE_PARAM,
                                           default_service_profile_name),
                              service_profile_name,
			      1);
    if (status) {
	dns_cfgclose(file);
	return status;
    };
    
    status = pkss_load_config(dns_cfgtostr(file, 
                                           SERVER_GROUP_PARAM,
                                           default_server_group_name),
                              server_group_name,
			      0);

    dns_cfgclose(file);
    return status;

    return 0;    
}



char * pkss_config__cds__service_profile_name(void) {
    if (!initialized) return NULL;
    return service_profile_name;
}


 
char * pkss_config__security__server_group_name(void) {
    if (!initialized) return NULL;
    return server_group_name;
}

