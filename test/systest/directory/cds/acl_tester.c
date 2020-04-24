/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: acl_tester.c,v $
 * Revision 1.1.2.2  1996/03/09  20:50:34  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:28:39  marty]
 *
 * Revision 1.1.2.1  1995/12/11  21:56:13  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/08/02  20:26 UTC  bissen
 * 	merge from HPDCE01
 * 
 * 	HP revision /main/HPDCE01/1  1994/04/01  17:44 UTC  truitt
 * 
 * 	HP revision /main/truitt_junk/1  1994/04/01  16:58 UTC  truitt
 * 	move file to cds directory
 * 
 * 	HP revision /main/HPDCE01/1  1994/02/22  16:45  bissen
 * 
 * 	HP revision /main/bissen_systest/1  1994/02/22  16:41  bissen
 * 	change OM_element_position to OM_uint32
 * 	[1995/12/11  20:56:05  root]
 * 
 * 	Loading drop Dce1_0_1fnl
 * 	[1992/10/02  14:44:06  tatsu_s]
 * 
 * Revision 1.1.7.2  1993/07/09  10:56:13  root
 * 	Initial King Kong branch
 * 	[1993/07/09  10:55:44  root]
 * 
 * Revision 1.1.5.2  1993/04/21  19:37:07  giza
 * 	DCE 1.0.1 Release
 * 	[1993/04/21  19:10:54  giza]
 * 
 * $EndLog$
 */
/*
 * (c) Copyright 1990, 1991, 1992 OPEN SOFTWARE FOUNDATION, INC. 
 * ALL RIGHTS RESERVED 
 */
/*
 * OSF DCE Version 1.0, UPDATE 1.0.1
 */
/*
 *
 *   	FILE_NAME: acl_tester.c
 *
 *   	COMPONENT_NAME: dce.systest
 *
 *   	FUNCTIONS: process_command_line_arguments()
 *		   general_setup()
 *		   create_object()
 *		   attempt_read()
 *		   attempt_write()
 *		   fill_in_structures()
 *		   fill_in_strings()
 *		   my_get_attribute()
 *
 *   	ORIGINS: 27
 *
 *   	(C) COPYRIGHT International Business Machines Corp. 1990
 *   	All Rights Reserved
 *
 *   	FOR_TEST: dcegdacl
 *
 *   	USAGE:
 *
 *		acl_tester -T <run time> -C <country> -O <organization> 
 *			   -U <organizational unit>
 *		where:
 *		    run time is the time in seconds to run this test
 *			(default:  3600).
 *		    country is the country name of the object to create.
 *		    organization is the organization name of the object to 
 *		    	create.
 *		    organizational unit is the organizational unit name of the 
 *		    	object to create.
 *
 *		Note:  the country, organization and organizational unit
 *		names input must exist in the DSA before this test is run.
 *
 *   	DESCRIPTION:
 *
 *		This test exercises the ACL's under GDS.  
 *	
 *		An object is added to the DSA with a public attribute 
 *		(telephone number), a standard attribute (postal address) and 
 *		a private attribute (user password).
 *
 *		Five users are added to the DSA, each having a different
 *		set of access rights.  User Danna will have read and modify
 *		public rights.  User Ken will have the same rights as Danna
 *		and read standard rights.  User Wayne will have the same rights
 *		as Ken and modify standard rights.  User Jerod will have the 
 *		same rights as Wayne and read sensitive rights.  User Helmut 
 *		will have the same rights as Jerod and modify sensitive rights. 
 *
 *		A random number between 0 and 4 is generated and this is used
 *		to index into an array of structures that contain the five users
 *		and their passwords.  A ds_bind() is conducted with that user
 *		and attempts to read and write all the attributes are attempted.
 *		The results are checked for consistency (e.g. User Danna should
 *		only be able to read and modify the telephone number of Gerald.
 *		All other read and modify attempts should fail).
 *
 *   	HISTORY:
 *      	05/10/91        Gerald Cantor           Initial design/code
 *      	11/02/91        Gerald Cantor           ANSIfied functions, 
 *							updated prologs and
 *							comments.
 *		12/05/91	Gerald Cantor		Changed strcpy and 
 *							strcmp to strncpy and 
 *							strncmp.
 */
#include <stdio.h>
#include <xx_lib.h>
#include <dce/rpc.h>
#include <xom.h>
#include <xds.h>
#include <xdsbdcp.h>
#include <xdsgds.h>
#include <systest.h>
#include "acl.h"
#include "acl_util.h"

/*
 * buf and header declared in acl_util.c.
 */
extern char buf[128], header[512];

/*
 * Global functions and variables.
 */
void fill_in_structures(OM_object, int, OM_type, OM_syntax, OM_object); 
void fill_in_strings(OM_object, int, OM_type, OM_syntax, char *); 
void my_get_attribute(OM_private_object, char **);

char *prog, *ctry, *org, *org_unit_name;

main(int argc, char **argv)
{
	/* 
 	 * main's functions and variables.
 	 */
	void process_command_line_arguments(int, char **, long *, char **, 
					    char **, char **); 
	void general_setup(OM_private_object *, OM_workspace);
	void create_object_and_users(OM_private_object); 
	void attempt_read(OM_private_object, int, char *); 
	void attempt_write(OM_private_object, int, char *);

	/*
 	 * This structure holds the users and their passwords.
	 */
	struct user_info_t
	{
		OM_descriptor *user;	/* the user in object form */
		char *user_passwd;	/* the password of that user */
	} users[5];

	DS_status st;
	OM_workspace workspace;
	OM_private_object def_session, session, user_session;
	OM_sint invoke;
	long run_time, stop_time;
	int index, om_status;

	prog = argv[0];
	make_header(prog);
	process_command_line_arguments(argc, argv, &run_time, &ctry, &org, 
				       &org_unit_name);
	workspace = ds_initialize();
	if (workspace == NULL)
	{
		sprintf(buf, "%s(%s):  Cannot initialize workspace", header, 
			prog);
		xx_severe(buf);
		exit(1);
	}

	general_setup(&def_session, workspace);
	create_object_and_users(def_session);

	users[0].user = Danna;
        users[0].user_passwd = "Danna";
        users[1].user = Ken;
        users[1].user_passwd = "Ken";
        users[2].user = Wayne;
        users[2].user_passwd = "Wayne";
        users[3].user = Jerod;
        users[3].user_passwd = "Jerod";
        users[4].user = Helmut;
        users[4].user_passwd = "Helmut";

	st = ds_unbind(def_session);
	sprintf(buf, "(%s):  Cannot unbind from default session\n", prog);
	gds_check_status(st, DS_SUCCESS, NOT_EQUAL, buf, FALSE);

	stop_time = time((long *)NULL) + run_time;
	
	while ((stop_time - time((long *)NULL)) > 0)
	{
		srand((int) time((long *)NULL));
		index = rand() % 5;

		/* 
	 	 * Fill in missing elements of array credentials.
		 */
		fill_in_structures(credentials, 0, DS_REQUESTOR, OM_S_OBJECT, 
				   (OM_object)users[index].user);
		fill_in_strings(credentials, 1, DSX_PASSWORD, 
                                OM_S_OCTET_STRING, users[index].user_passwd);

		/*
		 * Create object session.
		 */
		om_status = om_create(DSX_C_GDS_SESSION, OM_TRUE, workspace,
				      &session);
		sprintf(buf, "(%s):  Cannot create object session\n", prog);
		check_status(om_status, OM_SUCCESS, NOT_EQUAL, buf, TRUE);

		/*
		 * Fill in credentials to session.
		 */
		om_status = om_put(session, OM_REPLACE_ALL, credentials, 0, 0,
				   0);
		sprintf(buf, "(%s):  Cannot put credentials into session\n", 
			prog);
		check_status(om_status, OM_SUCCESS, NOT_EQUAL, buf, TRUE);
	
		sprintf(buf, "(%s):  Binding as user %s", prog, 
			users[index].user_passwd);
		xx_log(buf);
		st = ds_bind(session, workspace, &user_session);
		sprintf(buf, "(%s):  Cannot bind to session with user %s\n", 
			prog, users[index].user_passwd);
		gds_check_status(st, DS_SUCCESS, NOT_EQUAL, buf, TRUE);

		attempt_read(user_session, index, users[index].user_passwd);
		attempt_write(user_session, index, users[index].user_passwd);

		st = ds_unbind(user_session);
		sprintf(buf, "(%s):  Cannot unbind from %s's session\n", prog,
			users[index].user_passwd);
		gds_check_status(st, DS_SUCCESS, NOT_EQUAL, buf, FALSE);

		om_status = om_delete(session);
		sprintf(buf, "(%s):  Cannot delete session\n", prog);
		check_status(om_status, OM_SUCCESS, NOT_EQUAL, buf, FALSE);
	}
	exit(0);
}

/*
 * Print a usage statement to the tester.
 */
void show_usage()
{
	sprintf(buf, "Usage:  %s [OPTIONS]", prog);
	xx_log(buf);
	xx_log("Where OPTIONS are:");
	xx_log("-T <runtime> 	Time in seconds to run (default:  3600)");
	xx_log("-C <country>	Country name of object");
	xx_log("-O <org name>	Organization name of object");
	xx_log("-U <org unit>	Organizational unit name of object");
	xx_log("All options except runtime are required");
}

/*
 * process_command_line_arguments parses the parameters passed by the user
 * and assigns them to variables.
 */
void process_command_line_arguments(int ac, char **av, long *tme, char **ctry, 
				    char **org, char **unit)
{
	void show_usage(void);
	extern char *optarg;
	int c;
	void show_usage();

	while ((c = getopt(ac, av, "T:C:O:U:hH")) != EOF)
	{
		switch(c)
		{
			case 'T':
				*tme = atol(optarg);
				break;
			case 'C':
				*ctry = optarg;
				break;
			case 'O':
				*org = optarg;
				break;
			case 'U':
				*unit = optarg;
				break;
			case 'h':
			case 'H':
				show_usage();
				exit(0);
		}
	}
}

/*
 * general_setup fills in the country, organization and organizational unit
 * decriptor lists with the names the tester provided, activates the additional
 * packages and binds to the default DSA.
 */
void general_setup(OM_private_object *sess, OM_workspace workspace)
{
	DS_status st;
	char *func = "general_setup";

	fill_in_strings(country, 2, DS_ATTRIBUTE_VALUES, 
			   OM_S_PRINTABLE_STRING, ctry);
	fill_in_strings(organization, 2, DS_ATTRIBUTE_VALUES, 
			   OM_S_TELETEX_STRING, org);
	fill_in_strings(organizational_unit, 2, DS_ATTRIBUTE_VALUES, 
			   OM_S_TELETEX_STRING, org_unit_name);

	st = ds_version(feature_list, workspace);
	sprintf(buf, "(%s):  Cannot load GDS extensions\n", func);
	gds_check_status(st, DS_SUCCESS, NOT_EQUAL, buf, TRUE);

	st = ds_bind(DS_DEFAULT_SESSION, workspace, sess);
	sprintf(buf, "(%s):  Cannot bind to default session\n", func);
	gds_check_status(st, DS_SUCCESS, NOT_EQUAL, buf, TRUE);

}

/*
 * create_object_and_users adds object Gerald and users Danna, Ken, Wayne,
 * Jerod and Helmut to the DSA.
 */
void create_object_and_users(OM_private_object sess)
{
	DS_status st;
	OM_sint invoke;

	/*
	 * Add object Gerald now.
	 */
	st = ds_add_entry(sess, DS_DEFAULT_CONTEXT, name, entry_info, 
			  &invoke);
	sprintf(buf, "(create_users):  Cannot add entry Gerald to tree\n");
	gds_check_status(st, DS_SUCCESS, NOT_EQUAL, buf, TRUE);
	/* object Gerald created. */

	/*
	 * Add user Danna.
 	 */
	st = ds_add_entry(sess, DS_DEFAULT_CONTEXT, Danna, Danna_entry_info, 
			  &invoke);
	sprintf(buf, "(create_users):  Cannot create user Danna\n");
	gds_check_status(st, DS_SUCCESS, NOT_EQUAL, buf, TRUE);
	/* user Danna added */

	/*
	 * Add user Ken.
 	 */
	st = ds_add_entry(sess, DS_DEFAULT_CONTEXT, Ken, Ken_entry_info, 
			  &invoke);
	sprintf(buf, "(create_users):  Cannot create user Ken\n");
	gds_check_status(st, DS_SUCCESS, NOT_EQUAL, buf, TRUE);
	/* user Ken added */

	/*
	 * Add user Wayne.
 	 */
	st = ds_add_entry(sess, DS_DEFAULT_CONTEXT, Wayne, Wayne_entry_info, 
			  &invoke);
	sprintf(buf, "(create_users):  Cannot create user Wayne\n");
	gds_check_status(st, DS_SUCCESS, NOT_EQUAL, buf, TRUE);
	/* user Wayne added. */

	/*
	 * Add user Jerod.
 	 */
	st = ds_add_entry(sess, DS_DEFAULT_CONTEXT, Jerod, Jerod_entry_info, 
			  &invoke);
	sprintf(buf, "(create_users):  Cannot create user Jerod\n");
	gds_check_status(st, DS_SUCCESS, NOT_EQUAL, buf, TRUE);
	/* user Jerod added. */

	/*
	 * Add user Helmut.
 	 */
	st = ds_add_entry(sess, DS_DEFAULT_CONTEXT, Helmut, Helmut_entry_info, 
			  &invoke);
	sprintf(buf, "(create_users):  Cannot create user Helmut\n");
	gds_check_status(st, DS_SUCCESS, NOT_EQUAL, buf, TRUE);
	/* user Helmut added. */

}

/*
 * attempt_read receives a bound session from the user selected and attempts
 * to read the telephone number, postal address and user password from object
 * Gerald.  The results are verified, depending on which user was selected.
 */ 
void attempt_read(OM_private_object sess, int i, char *user)
{
	DS_status st, rd_st1, rd_st2, rd_st3;
	OM_private_object result1, result2, result3;
	char *func = "attempt_read";
	static char *telephone;
	static char *post_addr;
	static char *pw;
	OM_sint invoke_id;
	int rc1, rc2, rc3;

	telephone = (char *)NULL;
	post_addr = (char *)NULL;
	pw = (char *)NULL;

	rd_st1 = ds_read(sess, DS_DEFAULT_CONTEXT, name, phone_num, &result1, 
		         &invoke_id);

	rd_st2 = ds_read(sess, DS_DEFAULT_CONTEXT, name, user_post_addr, 
			 &result2, &invoke_id);

	rd_st3 = ds_read(sess, DS_DEFAULT_CONTEXT, name, user_password, 
			 &result3, &invoke_id);

	switch(i)
	{
		case(0):
			if (!(rd_st1 == DS_SUCCESS && rd_st2 != DS_SUCCESS && \
rd_st3 != DS_SUCCESS))
			{
				sprintf(buf, "(%s):  ds_read for %s failed", 
					func, user);
				xx_error(buf); 
				exit(1);
			}
			else
				my_get_attribute(result1, &telephone);
				
			break;

		case(1):
		case(2):
			if (!(rd_st1 == DS_SUCCESS && rd_st2 == DS_SUCCESS \
&& rd_st3 != DS_SUCCESS))
			{
				sprintf(buf, "(%s):  ds_read for %s failed", 
					func, user);
				xx_error(buf); 
				exit(1);
			}
			else
			{
				my_get_attribute(result1, &telephone);
				my_get_attribute(result2, &post_addr);
			}
				
			break;

		case(3):
		case(4):
			if (!(rd_st1 == DS_SUCCESS && rd_st2 == DS_SUCCESS \
&& rd_st3 == DS_SUCCESS))
			{
				sprintf(buf, "(%s):  ds_read for %s failed", 
					func, user);
				xx_error(buf); 
				show_gds_status(rd_st1);
				show_gds_status(rd_st2);
				show_gds_status(rd_st3);
				exit(1);
			}
			else
			{
				my_get_attribute(result1, &telephone);
				my_get_attribute(result2, &post_addr);
				my_get_attribute(result3, &pw);
			}
			break;
	}

	/*
	 * Now that the ds_read's passed, let's see if we read the right
	 * items.
	 */
	rc1 = strncmp(telephone, "555-1212", 8);
	rc2 = strncmp(post_addr, "2603", 4);
	rc3 = strncmp(pw, "Cantor", 6);

	switch(i)
	{
		case(0):	/* read and modify public */
			if (!(rc1 == 0 && rc2 != 0 && rc3 != 0))
			{
				sprintf(buf, 
				"%s(%s):  Unexpected read results from user %s", 				header, func, user);
				xx_error(buf);
				sprintf(buf, "(%s):   telephone = %s", func, 
					telephone);
				xx_error(buf);
				sprintf(buf, "(%s):   postal address = %s", 
					func, post_addr);
				xx_error(buf);
				sprintf(buf, "(%s):   password = %s", func, pw);
				xx_error(buf);
			}
			else
			{
				sprintf(buf, 
				"%s(%s):  Read attempts for user %s in order",
				header, func, user);
				xx_log(buf);
			}
			break;

		case(1):	/* read and modify public */
		case(2):	/* read standard */
			if (!(rc1 == 0 && rc2 == 0 && rc3 != 0))
			{
				sprintf(buf, 
				"%s(%s):  Unexpected read results from user %s", 				header, func, user);
				xx_error(buf);
				sprintf(buf, "(%s):   telephone = %s", func, 
					telephone);
				xx_error(buf);
				sprintf(buf, "(%s):   postal address = %s", 
					func, post_addr);
				xx_error(buf);
				sprintf(buf, "(%s):   password = %s", func, pw);
				xx_error(buf);
			}
			else
			{
				sprintf(buf, 
				"%s(%s):  Read attempts for user %s in order",
				header, func, user);
				xx_log(buf);
			}
			break;

		case(3):	/* read and modify public, */
		case(4):	/* read and write standard */
			if (!(rc1 == 0 && rc2 == 0 && rc3 == 0))
			{
				sprintf(buf, 
				"%s(%s):  Unexpected read results from user %s", 				header, func, user);
				xx_error(buf);
				sprintf(buf, "(%s):   telephone = %s", func, 
					telephone);
				xx_error(buf);
				sprintf(buf, "(%s):   postal address = %s", 
					func, post_addr);
				xx_error(buf);
				sprintf(buf, "(%s):   password = %s", func, pw);
				xx_error(buf);
			}
			else
			{
				sprintf(buf, 
				"%s(%s):  Read attempts for user %s in order",
				header, func, user);
				xx_log(buf);
			}
			break;
	}
	printf("\n");
}

/*
 * attempt_write receives a bound session from the user selected and attempts
 * to modify the telephone number, postal address and user password from object
 * Gerald.  The results are verified, depending on which user was selected.
 * Afterwards, the attributes are returned to their original values.  
 */ 
void attempt_write(OM_private_object sess, int i, char *user)
{
	DS_status st, wr_st1, wr_st2, wr_st3, rd_st1, rd_st2, rd_st3;
	OM_private_object result1, result2, result3;
	OM_sint invoke_id;
	char *telephone, *post_addr, *pw, *func = "attempt_write";
	int rc1, rc2, rc3;

	telephone = (char *)NULL;
	post_addr = (char *)NULL;
	pw = (char *)NULL;

	wr_st1 = ds_modify_entry(sess, DS_DEFAULT_CONTEXT, name, 
			         phone_modification, &invoke_id);
	wr_st2 = ds_modify_entry(sess, DS_DEFAULT_CONTEXT, name, 
				 post_addr_modification, &invoke_id);
	wr_st3 = ds_modify_entry(sess, DS_DEFAULT_CONTEXT, name, 
				 pw_modification, &invoke_id);
	switch(i)
	{
		case(0): /* read and modify public */
		case(1): /* read standard */
			if (!(wr_st1 == DS_SUCCESS && wr_st2 != DS_SUCCESS && wr_st3 != DS_SUCCESS)) 
			{
				sprintf(buf, 
				"%s(%s):  Unexpected write status for user %s",
				header, func, user);
				xx_severe(buf);
				show_gds_status(wr_st1);
				show_gds_status(wr_st2);
				show_gds_status(wr_st3);
				exit(1);
			}
			break;

		case(2): /* read and modify standard */
		case(3): /* read sensitive */
			if (!(wr_st1 == DS_SUCCESS && wr_st2 == DS_SUCCESS && wr_st3 != DS_SUCCESS)) 
			{
				sprintf(buf, 
				"%s(%s):  Unexpected write status for user %s",
				header, func, user);
				xx_severe(buf);
				show_gds_status(wr_st1);
				show_gds_status(wr_st2);
				show_gds_status(wr_st3);
				exit(1);
			}
			break;

		case(4): /* read and modify sensitive */
			if (!(wr_st1 == DS_SUCCESS && wr_st2 == DS_SUCCESS && wr_st3 == DS_SUCCESS)) 
			{
				sprintf(buf, 
				"%s(%s):  Unexpected write status for user %s",
				header, func, user);
				xx_severe(buf);
				show_gds_status(wr_st1);
				show_gds_status(wr_st2);
				show_gds_status(wr_st3);
				exit(1);
			}
			break;
	}

	rd_st1 = ds_read(sess, DS_DEFAULT_CONTEXT, name, phone_num, &result1,
                     &invoke_id);

	rd_st2 = ds_read(sess, DS_DEFAULT_CONTEXT, name, user_post_addr, 
			 &result2, &invoke_id);

	rd_st3 = ds_read(sess, DS_DEFAULT_CONTEXT, name, user_password, 
			 &result3, &invoke_id);

	switch(i)
	{
		case(0):
			if (!(rd_st1 == DS_SUCCESS && rd_st2 != DS_SUCCESS && \
rd_st3 != DS_SUCCESS))
			{
				sprintf(buf, "(%s):  ds_read for %s failed", 
					func, user);
				xx_error(buf); 
				exit(1);
			}
			else
				my_get_attribute(result1, &telephone);
			break;

		case(1):
		case(2):
			if (!(rd_st1 == DS_SUCCESS && rd_st2 == DS_SUCCESS \
&& rd_st3 != DS_SUCCESS))
			{
				sprintf(buf, "(%s):  ds_read for %s failed", 
					func, user);
				xx_error(buf); 
				exit(1);
			}
			else
			{
				my_get_attribute(result1, &telephone);
				my_get_attribute(result2, &post_addr);
			}		
			break;

		case(3):
		case(4):
			if (!(rd_st1 == DS_SUCCESS && rd_st2 == DS_SUCCESS \
&& rd_st3 == DS_SUCCESS))
			{
				sprintf(buf, "(%s):  ds_read for %s failed", 
					func, user);
				xx_error(buf); 
				exit(1);
			}
			else
			{
				my_get_attribute(result1, &telephone);
				my_get_attribute(result2, &post_addr);
				my_get_attribute(result3, &pw);
			}		
			break;
	}

	rc1 = strncmp(telephone, "777-0909", 8);
	rc2 = strncmp(post_addr, "2402", 4);
	rc3 = strncmp(pw, "Monroe", 6);

	switch(i)
	{
		case(0): /* read and modify public */
		case(1): /* read standard */
			if (!(rc1 == 0 && rc2 != 0 && rc3 != 0))
			{
				sprintf(buf, 
				"%s(%s):  Unexpected write results for user %s", 				header, func, user);
				xx_error(buf);
				sprintf(buf, "(%s):   telephone = %s", func, 
					telephone);
				xx_error(buf);
				sprintf(buf, "(%s):   postal address = %s", 
					func, post_addr);
				xx_error(buf);
				sprintf(buf, "(%s):   password = %s", func, pw);
				xx_error(buf);
			}
			else
			{
				sprintf(buf, 
				"%s(%s):  Write attempts for user %s in order",
				header, func, user);
				xx_log(buf);
			}
			break;	
		case(2): /* read and modify standard */
		case(3): /* read sensitive */
			if (!(rc1 == 0 && rc2 == 0 && rc3 != 0))
			{
				sprintf(buf, 
				"%s(%s):  Unexpected write results for user %s", 				header, func, user);
				xx_error(buf);
				sprintf(buf, "(%s):   telephone = %s", func, 
					telephone);
				xx_error(buf);
				sprintf(buf, "(%s):   postal address = %s", 
					func, post_addr);
				xx_error(buf);
				sprintf(buf, "(%s):   password = %s", func, pw);
				xx_error(buf);
			}
			else
			{
				sprintf(buf, 
				"%s(%s):  Write attempts for user %s in order",
				header, func, user);
				xx_log(buf);
			}
			break;
		case(4): /* read and modify sensitive */
			if (!(rc1 == 0 && rc2 == 0 && rc3 == 0))
			{
				sprintf(buf, 
				"%s(%s):  Unexpected write results for user %s", 				header, func, user);
				xx_error(buf);
				sprintf(buf, "(%s):   telephone = %s", func, 
					telephone);
				xx_error(buf);
				sprintf(buf, "(%s):   postal address = %s", 
					func, post_addr);
				xx_error(buf);
				sprintf(buf, "(%s):   password = %s", func, pw);
				xx_error(buf);
			}
			else
			{
				sprintf(buf, 
				"%s(%s):  Write attempts for user %s in order",
				header, func, user);
				xx_log(buf);
			}
			break;
	}

	/*
	 * Now change object back to what it was originally.
	 */
	fill_in_structures(phone_add, 3, DS_ATTRIBUTE_VALUES, 
                           OM_S_OCTET_STRING, (OM_object)"555-1212");		   
	fill_in_strings(postal_address_portion_mod, 1, DS_POSTAL_ADDRESS, 
			   OM_S_TELETEX_STRING, "2603");		   
	fill_in_structures(pw_add, 3, DS_ATTRIBUTE_VALUES, OM_S_OCTET_STRING, 
			   (OM_object)"Cantor");		   

	switch(i)
	{
		case(0):
		case(1):
			st = ds_modify_entry(sess, DS_DEFAULT_CONTEXT, name, 
				 	     phone_modification, &invoke_id);
			sprintf(buf, "(%s):  Unable to change phone back to orginal value\n", func);
			gds_check_status(st, DS_SUCCESS, NOT_EQUAL, buf, TRUE);
			break;
		case(2):
		case(3):
			st = ds_modify_entry(sess, DS_DEFAULT_CONTEXT, name, 
				 	     phone_modification, &invoke_id);
			sprintf(buf, "(%s):  Unable to change phone back to orginal value\n", func);
			gds_check_status(st, DS_SUCCESS, NOT_EQUAL, buf, TRUE);

			st = ds_modify_entry(sess, DS_DEFAULT_CONTEXT, name, 
				 	     post_addr_modification, 
					     &invoke_id);
			sprintf(buf, "(%s):  Unable to change postal address to orginal value\n", func);
			gds_check_status(st, DS_SUCCESS, NOT_EQUAL, buf, TRUE);
			break;
		case(4):
			st = ds_modify_entry(sess, DS_DEFAULT_CONTEXT, name, 
				 	     phone_modification, &invoke_id);
			sprintf(buf, "(%s):  Unable to change phone back to orginal value\n", func);
			gds_check_status(st, DS_SUCCESS, NOT_EQUAL, buf, TRUE);

			st = ds_modify_entry(sess, DS_DEFAULT_CONTEXT, name, 
				 	     post_addr_modification, 
					     &invoke_id);
			sprintf(buf, "(%s):  Unable to change postal address to orginal value\n", func);
			gds_check_status(st, DS_SUCCESS, NOT_EQUAL, buf, TRUE);

			st = ds_modify_entry(sess, DS_DEFAULT_CONTEXT, name, 
				 	     pw_modification, &invoke_id);
			sprintf(buf, "(%s):  Unable to change password to orginal value\n", func);
			gds_check_status(st, DS_SUCCESS, NOT_EQUAL, buf, TRUE);
			break;
	}

	/*
	 * Now put the modification descriptors back to original modify values.
	 */
	fill_in_structures(phone_add, 3, DS_ATTRIBUTE_VALUES, 
                           OM_S_OCTET_STRING, (OM_object)"777-0909");		   
	fill_in_strings(postal_address_portion_mod, 1, DS_POSTAL_ADDRESS, 
			   OM_S_TELETEX_STRING, "2402");		   
	fill_in_structures(pw_add, 3, DS_ATTRIBUTE_VALUES, OM_S_OCTET_STRING, 
			   (OM_object)"Monroe");		   

	printf("\n");
}

/*
 * my_get_attributes performs a series of om_get's on a private object to obtain
 * the value of the object.
 */
void my_get_attribute(OM_private_object res, char **attr)
{
	OM_public_object entry;
	OM_public_object attributes;
	OM_public_object attr_values, attr2_values;
	OM_descriptor *attr_val;
	OM_type entry_list[] = { DS_ENTRY, 0 };
	OM_type attributes_list[] = { DS_ATTRIBUTES, 0 };
	OM_type attr_value_list[] = { DS_ATTRIBUTE_VALUES, 0 };
	OM_type attr2_value_list[] = { DS_POSTAL_ADDRESS, 0 };
	OM_return_code st;
        OM_value_position total;
	int len_of_attr;
	char *func = "my_get_attributes";

	st = om_get(res, OM_EXCLUDE_ALL_BUT_THESE_TYPES 
		    + OM_EXCLUDE_SUBOBJECTS, entry_list, OM_FALSE, 0, 0, &entry, 		    &total);
	sprintf(buf, "(%s):  om_get on read-result failed\n", func);
	check_status(st, 0, NOT_EQUAL, buf, TRUE);

	if (total == 0)
		return;

	st = om_get(entry->value.object.object, OM_EXCLUDE_ALL_BUT_THESE_TYPES
		    + OM_EXCLUDE_SUBOBJECTS, attributes_list, OM_FALSE, 0, 0,
		    &attributes, &total);
	sprintf(buf, "(%s):  om_get for attributes failed\n", func);
	check_status(st, 0, NOT_EQUAL, buf, TRUE);

	if (total == 0)
		return;

	st = om_get(attributes->value.object.object, 
		    OM_EXCLUDE_ALL_BUT_THESE_TYPES + OM_EXCLUDE_SUBOBJECTS, 
		    attr_value_list, OM_FALSE, 0, 0, &attr_values, &total);
	sprintf(buf, "(%s):  om_get for attribute values failed\n", func);
	check_status(st, 0, NOT_EQUAL, buf, TRUE);

	if (total == 0)
		return;

	if ((attr_values->syntax & OM_S_SYNTAX) == OM_S_OBJECT)
	{
		st = om_get(attr_values->value.object.object, 
		    	OM_EXCLUDE_ALL_BUT_THESE_TYPES + OM_EXCLUDE_SUBOBJECTS, 
		    	attr2_value_list, OM_FALSE, 0, 0, &attr2_values, &total);
		sprintf(buf, "(%s):  om_get for attribute values failed\n", 
			func);
		check_status(st, 0, NOT_EQUAL, buf, TRUE);
		attr_val = attr2_values;
	}
	else
		attr_val = attr_values;
		
	st = om_delete(entry);
	sprintf(buf, "(%s):  Cannot delete object entry\n", func);
	check_status(st, OM_SUCCESS, NOT_EQUAL, buf, FALSE);

	st = om_delete(attributes);
	sprintf(buf, "(%s):  Cannot delete object attributes\n", func);
	check_status(st, OM_SUCCESS, NOT_EQUAL, buf, FALSE);

	len_of_attr = attr_val->value.string.length;
	*attr = (char *)malloc(len_of_attr);
	strncpy(*attr, attr_val->value.string.elements, len_of_attr);

	st = om_delete(attr_values);
	sprintf(buf, "(%s):  Cannot delete object attr_values\n", func);
	check_status(st, OM_SUCCESS, NOT_EQUAL, buf, FALSE);
}

/*
 * fill_in_strings indexes into an object and sets the fields.
 */
void fill_in_strings(OM_object desc, int index, OM_type type, OM_syntax syntax, 		     char *value)
{
        int length = strlen(value);

        desc[index].type = type;
        desc[index].syntax = syntax;
        desc[index].value.string.length = (OM_uint32)length;
        desc[index].value.string.elements = value;
}

/*
 * fill_in_structures indexes into an object and sets the fields.
 */
void fill_in_structures(OM_object desc, int index, OM_type type, 
			OM_syntax syntax, OM_object value)
{
	desc[index].type = type;
	desc[index].syntax = syntax;
	desc[index].value.object.padding = 0;
	desc[index].value.object.object = value;
}	
