/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dns_strings.h,v $
 * Revision 1.1.9.2  1996/02/18  23:33:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:28  marty]
 *
 * Revision 1.1.9.1  1995/12/08  15:15:54  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:43:02  root]
 * 
 * Revision 1.1.7.6  1994/08/11  20:35:15  peckham
 * 	Add CDS_CACHE_WAN to hold cached server definitions.
 * 	[1994/08/11  04:48:14  peckham]
 * 
 * Revision 1.1.7.5  1994/08/03  19:02:17  mccann
 * 	includes cleanup
 * 	[1994/08/01  19:53:09  mccann]
 * 
 * Revision 1.1.7.4  1994/07/25  15:14:06  proulx
 * 	Use common ACL manager for subcomponent mgmt ACLs
 * 	[1994/07/25  13:59:31  proulx]
 * 
 * Revision 1.1.7.3  1994/06/09  18:38:59  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:11:38  devsrc]
 * 
 * Revision 1.1.7.2  1994/04/29  15:50:19  peckham
 * 	Add strings for dcelocal_path use.
 * 	Remove VMS definitions.
 * 	[1994/04/29  13:57:27  peckham]
 * 
 * Revision 1.1.7.1  1994/03/12  22:02:54  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:14:25  peckham]
 * 
 * Revision 1.1.5.3  1992/12/30  13:29:59  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:48:12  zeliff]
 * 
 * Revision 1.1.5.2  1992/09/29  19:14:26  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:13:05  weisman]
 * 
 * Revision 1.1.2.3  1992/06/01  18:45:15  mfox
 * 	Add a new string to replace an instance of a wired-in "/opt/dcelocal"
 * 	[1992/06/01  18:44:08  mfox]
 * 
 * Revision 1.1.2.2  1992/03/22  22:04:07  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:26:22  weisman]
 * 
 * Revision 1.1  1992/01/19  15:16:33  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _DNS_STRINGS_H
#define _DNS_STRINGS_H
/*
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1991-1994. ALL RIGHTS RESERVED.
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
 * AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 *
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
 *
 */


/*
 * Globally used pathnames and other embedded strings
 * Many of these can be altered with dns.conf
 */

/*
 * DNS_DEFAULTS_FILE	cellname to uuid mappings
 * DNS_NAMES_FILE	local names
 * DNS_CONFIG_FILE	locations of servers
 * DNS_CDS_ATTR_FILE	objid to attr. string names
 * DNS_GDS_ATTR_FILE	objid to X.500 attr info
 * DNS_CONF_FILE	tailors runtime environment
 * DNS_CONF_PFX		prefix for config file parameters
 * DNS_CLERK_PROG_FILE	the clerk program file
 * DNS_CLERK_MSG_FILE	the clerk message file
 * DNS_CLERK_MSG_DIR	directory for clerk message file
 * DNS_ADVER_SOCK	server to advertiser
 * DNS_LIB_SOCK		library to advertiser
 * DNS_CONF_SOCK	config program to advertiser
 * DNS_ADV_TO_CONF_SOCK VMS advertiser (wr) to configure (rd)
 * DNS_CONF_TO_ADV_SOCK VMS configure (wr) to advertiser (rd)
 * DNS_CLERK_LOG_SOCK	VMS clerk logging to advertiser (rd)
 * DNS_DIAG_SOCK	cdsd_diag to server
 * DNS_DEF_CLERK_SOCK	library to clerk (w/o advertiser)
 * DNS_TMP_SOCK_TMPL	temporary name of library to clerk socket (sequence no.)
 * DNS_SOCK_TMPL	real name of above (cdsclerk_pid_login)
 * DNS_EVENT_TMPL	events to log (prog. name)
 * DNS_TRACE_TMPL	event log file
 * DNS_STRACE_OVERRIDE  server's event log override logical (VMS)
 * DNS_STRACE_TMPL      server's event log file (VMS)
 * DNS_CTRACE_TMPL	clerk's event log file
 * DNS_CLERK_TRACE_TMPL	clerk message trace file
 * DNS_LOG_TMPL		stdout & stderr log
 * DNS_CLOG_TMPL	same for clerks
 * DNS_CHFAIL_FILE	clerk's ch connect failure log file (VMS)
 * DNS_CHFAIL_FILE_OLD	oldest clerk ch fail log file version; for purging (VMS)
 * DNS_VMDATA_FILE	dns_malloc debug info
 * DNS_CORE_TMPL	core files
 * DNS_CACHE_NAME	tag for cache file
 * DNS_CACHE_FILE	the cache file
 * DNS_CACHE_VERSION	contains cache version no.
 * DNS_CACHE_WAN	the cached servers
 * DNS_ETC_DIR		directory for config files
 * DNS_CLI_WDIR		parent of client components' working directory
 * DNS_SVR_WDIR		server's working directory
 * DNS_SVR_DBDIR	directory for server databases
 * DNS_CH_FILES		contains list of all clearinghouse files
 * DNS_SVR_VERSION	version of server's checkpoint file
 * DNS_SVR_TLOG		server transaction log
 * DNS_SVR_CKPT		server checkpoint file
 * DNS_VERSION_FMT	format of version string
 * DNS_VERSION_SIZE	length of version string
 * DNS_CP_HELP_FILE	cp help info
 * DNS_CP_PARSE_FILE	cp's binary parse table
 * DNS_CP_MSG_FILE	cp's message catalog
 * DNS_CP_MSG_FILE_DEVEL cp's message catalog for devel (used by parser_aid_2)
 * DNS_CP_LOG_FILE	cp's log for stdin & stdout
 * DNS_ROOT		default local root symbol
 * DNS_LOGICAL_TABLE	VMS logical name table for root, name translations
 * DNS_ROOT_VAR		environment variable containing local root symbol
 * DNS_NAMES_VAR	environment variable for locating local names file
 * DNS_DEFAULTS_VAR	environment variable for locating defaults file
 * DNS_PRODUCT		product name in LMF PAK
 * DNS_RELEASE_DATE	1 day before product release date
 * DNS_BIND_SVRS	default path for named.ca
 */

/*
 * ASCII config files
 */
#define CDS_CDS_ATTR_FILE	"%s/etc/cds_attributes"
#define CDS_GDS_ATTR_FILE	"%s/etc/cds_globalnames"
#define CDS_CONF_FILE		"%s/etc/cds.conf"
#define CDS_CONF_PFX		"cds."
#define CDS_BIND_SVRS		"%s/etc/named.ca"
#define CDS_GDA_ID_FILE         "%s/etc/gda_id"

/*
 * The clerk program file
 */
#define CDS_CLERK_PROG_FILE	"%s/bin/cdsclerk"

/*
 * Socket files
 */
#define CDS_ADVER_SOCK		"%s/var/adm/directory/cds/cdsAdver"
#define CDS_LIB_SOCK		"%s/var/adm/directory/cds/cdsLib"
#define CDS_DIAG_SOCK		"%s/var/adm/directory/cds/cdsDiag"
#define CDS_DEF_CLERK_SOCK	"%s/var/adm/directory/cds/cds_clerk"
#define CDS_TMP_SOCK_TMPL	"%s/var/adm/directory/cds/dssS%d"
#define CDS_SOCK_TMPL		"%s_%d_%s"

/*
 * Debug files
 */
#define DNS_EVENT_TMPL		"./%s.events"
#define DNS_TRACE_TMPL		"./%s_pid_%d"
#define DNS_CTRACE_TMPL		"./%s_pid_%d_uid_%d"
#define DNS_CLERK_TRACE_TMPL	"./%s_pid_%d_uid_%d.trace"
#define DNS_LOG_TMPL		"./%s_pid_%d.log"
#define DNS_CLOG_TMPL		"./%s_pid_%d_uid_%d.log"
#define DNS_VMDATA_FILE		"./vm_data"
#define DNS_CORE_TMPL		"./core.%d"

/*
 * Clerk cache
 */
#define CDS_CACHE_NAME		"CdsClerkCache"
#define CDS_CACHE_VERSION	"%s/var/adm/directory/cds/cds_cache.version"
#define CDS_CACHE_FILE		"%s/var/adm/directory/cds/cds_cache.%010u"
#define CDS_CACHE_SNAPSHOT	"%s/var/adm/directory/cds/cds_cache.snapshot_of_corrupted_cache_by_pid_%d"
#define CDS_CACHE_WAN		"%s/var/adm/directory/cds/cds_cache.wan"

/*
 * Various directories
 */
#define CDS_ETC_DIR		"%s/etc/%s"
#define CDS_CLI_WDIR		"%s/var/adm/directory/cds"
#define CDS_SVR_WDIR		"%s/var/directory/cds/adm"
#define CDS_SVR_DBDIR		"%s/var/directory/cds"

/*
 * Clearinghouse files
 */
#define DNS_CH_FILES		"cds_files"
#define DNS_SVR_VERSION		".version"
#define DNS_SVR_TLOG		".tlog"
#define DNS_SVR_CKPT		".checkpoint"
#define DNS_VERSION_FMT		"%010u\n"
#define DNS_VERSION_SIZE	10	/* size of sprintf'ed version string */

/*
 * cp files
 */
#define CDS_CP_HELP_FILE	"%s/etc/cdscp.hlp"
#define CDS_CP_PARSE_FILE	"%s/etc/cdscp.bpt"
#define CDS_CP_MSG_FILE		"%s/etc/cdscp.mbf"
#define CDS_CP_LOG_FILE		"%s/etc/cdscp.log"

/*
 * Clerk/server management DACL files
*/
#ifdef DCE_SEC
#define CDS_CLERK_MGMT_ACL_FILENAME  "%s/var/adm/directory/cds/clerk_mgmt_acl.dat"
#define CDS_SERVER_MGMT_ACL_FILENAME "%s/var/directory/cds/server_mgmt_acl.dat"
#define CDS_GDA_MGMT_ACL_FILENAME "%s/var/directory/cds/gda_mgmt_acl.dat"

/* Names for new V1.1 acl files, same paths as above */
#define CDS_CLERK_MGMT_V1_ACL_FILENAME  "clerk_mgmt_acl_v1.dat"  
#define CDS_SERVER_MGMT_V1_ACL_FILENAME "server_mgmt_acl_v1.dat"
#define CDS_GDA_MGMT_V1_ACL_FILENAME    "gda_mgmt_acl_v1.dat"   

#endif /* DCE_SEC */

/*
 * Environment variables
 */
#define DNS_ROOT
#define DNS_ROOT_VAR
#define DNS_NAMES_VAR
#define DNS_DEFAULTS_VAR	"DNSDEFAULTS"

#endif  /* #ifndef _DNS_STRINGS_H */
