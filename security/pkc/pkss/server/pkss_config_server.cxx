//
// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// All Rights Reserved
//
// HISTORY
// $Log: pkss_config_server.cxx,v $
// Revision 1.1.2.1  1996/10/04  14:21:59  arvind
// 	OSF DCE 1.2.2 Drop 4
// 	[1996/10/04  13:35:11  arvind]
//
// 	OSF DCE 1.2.2 Drop 4
//
// Revision /main/DCE_1.2.2/1  1996/09/09  21:28 UTC  arvind
// 	PKSS drop from DEC (DCE1.2.2)
// 	[1996/08/30  15:50 UTC  arvind  /main/arvind_pkss/1]
// 
// $EndLog$
//
// (c) Copyright 1996,
// Digital Equipment Corporation, Maynard, Massachusetts, USA
// All Rights Reserved
//
//

#include <pkss_config_server.h>
#include <pkss_config_common.h>
#include <pkss_config_int.h>

#define DATABASE_PARAM		"local.database_filename"
#define SERVER_BINDINGS_PARAM	"cds.server_bindings_name"
#define SERVICE_GROUP_PARAM	"cds.service_group_name"
#define SERVER_PRINC_PARAM	"security.server_princ_name"
#define ADMIN_GROUP_PARAM	"security.server_admin_group_name"


char * default_service_group_name = "/.:/subsys/dce/pkssd";
char * default_server_admin_group_name = "subsys/dce/sec-admin";
char * default_database_name = "/opt/dcelocal/var/security/pkss/pkssdb";

char default_server_bindings_name[1200] = {0};
char default_server_princ_name[1200] = {0};
char * default_server_bindings_prefix = "/.:/";
char * default_server_bindings_postfix = "/pkss";
char * default_server_princ_prefix = "/.:/";
char * default_server_princ_postfix = "/pkssd";



char * server_bindings_name = NULL;
char * service_group_name = NULL;
char * server_princ_name = NULL;
char * admin_group_name = NULL;
char * database_name = NULL;



unsigned32 pkss_config__server_init(const char * imagename) {
    unsigned32 status;
    char * file;

    status = pkss_config__init(imagename);
    if ((status != 0) && (status != pks_s_already_initialized)) return status;

    strcpy(default_server_bindings_name, default_server_bindings_prefix);
    strcat(default_server_bindings_name, pkss__this_host);
    strcat(default_server_bindings_name, default_server_bindings_postfix);

    strcpy(default_server_princ_name, default_server_princ_prefix);
    strcat(default_server_princ_name, pkss__this_host);
    strcat(default_server_princ_name, default_server_princ_postfix);
    
    if ((file = dns_cfgopen(NULL, NULL)) == NULL) return pks_s_no_config_file;

    status = pkss_load_config(dns_cfgtostr(file, 
                                           SERVER_BINDINGS_PARAM,
                                           default_server_bindings_name),
                              server_bindings_name,
			      1);
    if (status) {
	dns_cfgclose(file);
	return status;
    };
    
    status = pkss_load_config(dns_cfgtostr(file, 
                                           SERVICE_GROUP_PARAM,
                                           default_service_group_name),
                              service_group_name,
			      1);
    if (status) {
	dns_cfgclose(file);
	return status;
    };

    status = pkss_load_config(dns_cfgtostr(file, 
                                           SERVER_PRINC_PARAM,
                                           default_server_princ_name),
                              server_princ_name,
			      0);
    if (status) {
	dns_cfgclose(file);
	return status;
    };

    status = pkss_load_config(dns_cfgtostr(file, 
                                           ADMIN_GROUP_PARAM,
                                           default_server_admin_group_name),
                              admin_group_name,
			      0);
    if (status) {
	dns_cfgclose(file);
	return status;
    };

    status = pkss_load_config(dns_cfgtostr(file, 
                                           DATABASE_PARAM,
                                           default_database_name),
                              database_name,
			      2);

    dns_cfgclose(file);

    return status;    
}


const char * pkss_config__cds__server_bindings_name(void) {
    if (!initialized) return NULL;
    return server_bindings_name;
}


const char * pkss_config__cds__service_group_name(void) { 
    if (!initialized) return NULL;
    return service_group_name;
}

const char * pkss_config__security__server_princ_name(void) {
    if (!initialized) return NULL;
    return server_princ_name;
}

const char * pkss_config__security__admin_group_name(void) {
    if (!initialized) return NULL;
    return admin_group_name;
}

const char * pkss_config__local__database_name(void) {
    if (!initialized) return NULL;
    return database_name;
}
