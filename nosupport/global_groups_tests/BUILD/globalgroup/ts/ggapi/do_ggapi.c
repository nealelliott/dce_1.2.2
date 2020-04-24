#include <stdio.h>
#include <dce/pthread.h>
#include <dce/pgo.h>
#include <dce/dce_error.h>

/*
 * Global variables
 */
unsigned char		*env_registry_site_name;/* from REGISTRY_SITE_NAME */
sec_rgy_handle_t	context;		/* from sec_rgy_site_open */
unsigned char		*site_name;		/* from sec_rgy_site_get */

char			*program_name;	/* program name (argv[0]) */

/*
 * status code for internal
 */
#define	ggapi_s_expected	0	/* goes on expectedly (pass / fail) */
#define	ggapi_s_unexpected	1	/* unexpected error has occurred */

struct	_error_status_in	{
	error_status_t	c;
	char		*s;
};

static	struct	_error_status_in	error_status_in[] = {
	{error_status_ok,		"error_status_ok",		},
	{sec_rgy_no_more_entries,	"sec_rgy_no_more_entries",	},
	{sec_rgy_not_authorized,	"sec_rgy_not_authorized",	},
	{sec_rgy_bad_domain,		"sec_rgy_bad_domain",		},
	{sec_rgy_object_not_found,	"sec_rgy_object_not_found",	},
	{sec_rgy_server_unavailable,	"sec_rgy_server_unavailable",	},
	{sec_login_s_no_current_context,	"sec_login_s_no_current_context",	},
};

const	char	*which_error(
error_status_t	status
)
{
	static	char	status_buf[] = "0x12345678";
	int	i;

	for (i = 0; i < (sizeof(error_status_in) / sizeof(error_status_in[0])); i++) {
		if (status == error_status_in[i].c) {
			return(error_status_in[i].s);
		}
	}

	sprintf(status_buf, "0x%08x", status);
	return(status_buf);
}

void	usage(
void
)
{
	static	char	*usage_text = "\
usage:\n\
  %s sec_rgy_pgo_add_member name_domain go_name person_name\n\
  %s sec_rgy_pgo_delete_member name_domain go_name person_name\n\
  %s sec_rgy_pgo_is_member name_domain go_name person_name\n\
  %s sec_rgy_pgo_get_members name_domain go_name max_members\n\
  - environment variable REGISTRY_SITE_NAME can specify the registry site\n\
";
	char	*p = program_name;

	fprintf(stderr, usage_text, p, p, p, p);
	return;
}

int	exec_api(
int	argc,
char	*argv[]
)
{
	/*
         * which API ?
         */
	if (strcmp(argv[0], "sec_rgy_pgo_add_member") == 0) {
		
		sec_rgy_domain_t	name_domain;
		char			*go_name;
		char			*person_name;
		error_status_t		status;

		if (argc < 4) {
			fprintf(stderr, "Too least arguments.\n");
			usage();
			return(ggapi_s_unexpected);
		}

		/* name_domain */
		if (strcmp(argv[1],  "sec_rgy_domain_group") == 0) {
			name_domain = sec_rgy_domain_group;
		} else
		if (strcmp(argv[1],  "sec_rgy_domain_org") == 0) {
			name_domain = sec_rgy_domain_org;
		} else
		{
			fprintf(stderr, "Illeagal argument for name_domain: %s\n", argv[1]);
			usage();
			return(ggapi_s_unexpected);
		}

		/* go_name */
		go_name = argv[2];

		/* person_name */
		person_name = argv[3];

		sec_rgy_pgo_add_member(
			context,
			name_domain,
			go_name,
			person_name,
			&status);
		fprintf(stdout, "API:\t%s\n", argv[0]);
		fprintf(stdout, "context:\t(%s)\n", site_name);
		fprintf(stdout, "name_domain:\t%s\n", argv[1]);
		fprintf(stdout, "go_name:\t%s\n", go_name);
		fprintf(stdout, "person_name:\t%s\n", person_name);
		fprintf(stdout, "status:\t%s\n", which_error(status));
	} else
	if (strcmp(argv[0], "sec_rgy_pgo_delete_member") == 0) {
		
		sec_rgy_domain_t	name_domain;
		char			*go_name;
		char			*person_name;
		error_status_t		status;

		if (argc < 4) {
			fprintf(stderr, "Too least arguments.\n");
			usage();
			return(ggapi_s_unexpected);
		}

		/* name_domain */
		if (strcmp(argv[1],  "sec_rgy_domain_group") == 0) {
			name_domain = sec_rgy_domain_group;
		} else
		if (strcmp(argv[1],  "sec_rgy_domain_org") == 0) {
			name_domain = sec_rgy_domain_org;
		} else
		{
			fprintf(stderr, "Illeagal argument for name_domain: %s\n", argv[1]);
			usage();
			return(ggapi_s_unexpected);
		}

		/* go_name */
		go_name = argv[2];

		/* person_name */
		person_name = argv[3];

		sec_rgy_pgo_delete_member(
			context,
			name_domain,
			go_name,
			person_name,
			&status);
		fprintf(stdout, "API:\t%s\n", argv[0]);
		fprintf(stdout, "context:\t(%s)\n", site_name);
		fprintf(stdout, "name_domain:\t%s\n", argv[1]);
		fprintf(stdout, "go_name:\t%s\n", go_name);
		fprintf(stdout, "person_name:\t%s\n", person_name);
		fprintf(stdout, "status:\t%s\n", which_error(status));
	} else
	if (strcmp(argv[0], "sec_rgy_pgo_is_member") == 0) {
		
		boolean32		return_value;
		sec_rgy_domain_t	name_domain;
		char			*go_name;
		char			*person_name;
		error_status_t		status;

		if (argc < 4) {
			fprintf(stderr, "Too least arguments.\n");
			usage();
			return(ggapi_s_unexpected);
		}

		/* name_domain */
		if (strcmp(argv[1],  "sec_rgy_domain_group") == 0) {
			name_domain = sec_rgy_domain_group;
		} else
		if (strcmp(argv[1],  "sec_rgy_domain_org") == 0) {
			name_domain = sec_rgy_domain_org;
		} else
		{
			fprintf(stderr, "Illeagal argument for name_domain: %s\n", argv[1]);
			usage();
			return(ggapi_s_unexpected);
		}

		/* go_name */
		go_name = argv[2];

		/* person_name */
		person_name = argv[3];

		return_value = sec_rgy_pgo_is_member(
				context,
				name_domain,
				go_name,
				person_name,
				&status);
		fprintf(stdout, "API:\t%s\n", argv[0]);
		fprintf(stdout, "context:\t(%s)\n", site_name);
		fprintf(stdout, "name_domain:\t%s\n", argv[1]);
		fprintf(stdout, "go_name:\t%s\n", go_name);
		fprintf(stdout, "person_name:\t%s\n", person_name);
		fprintf(stdout, "status:\t%s\n", which_error(status));
		fprintf(stdout, "return_value:\t%s\n", return_value ? "TRUE" : "FALSE");
	} else
	if (strcmp(argv[0], "sec_rgy_pgo_get_members") == 0) {
		
		sec_rgy_domain_t	name_domain;
		char			*go_name;
		sec_rgy_cursor_t	member_cursor;
		signed32		max_members;
		sec_rgy_member_t	*member_list;
		signed32		number_supplied;
		signed32		number_members;
		error_status_t		status;

		if (argc < 4) {
			fprintf(stderr, "Too least arguments.\n");
			usage();
			return(ggapi_s_unexpected);
		}

		/* name_domain */
		if (strcmp(argv[1],  "sec_rgy_domain_group") == 0) {
			name_domain = sec_rgy_domain_group;
		} else
		if (strcmp(argv[1],  "sec_rgy_domain_org") == 0) {
			name_domain = sec_rgy_domain_org;
		} else
		if (strcmp(argv[1],  "sec_rgy_domain_person") == 0) {
			name_domain = sec_rgy_domain_person;
		} else
		{
			fprintf(stderr, "Illeagal argument for name_domain: %s\n", argv[1]);
			usage();
			return(ggapi_s_unexpected);
		}

		/* go_name */
		go_name = argv[2];

		/* max_members */
		max_members = atoi(argv[3]);

		/* member_list */
		member_list = calloc(max_members, sizeof(sec_rgy_member_t));
		if (member_list == NULL) {
			fprintf(stderr, "Cannot alloc member_list");
			return(ggapi_s_unexpected);
		}

		sec_rgy_cursor_reset(&member_cursor);
		do {
			int	i;
			sec_rgy_pgo_get_members(
				context,
				name_domain,
				go_name,
				&member_cursor,
				max_members,
				member_list,
				&number_supplied,
				&number_members,
				&status);
			fprintf(stdout, "API:\t%s\n", argv[0]);
			fprintf(stdout, "context:\t(%s)\n", site_name);
			fprintf(stdout, "name_domain:\t%s\n", argv[1]);
			fprintf(stdout, "go_name:\t%s\n", go_name);
			fprintf(stdout, "member_cursor:\t%08x\n", &member_cursor);
			fprintf(stdout, "max_members:\t%d\n", max_members);
			fprintf(stdout, "member_list:");
			for (i = 0; i < number_supplied; i++) {
				fprintf(stdout, "\t%s", member_list[i]);
			}
			fprintf(stdout, "\n");
			fprintf(stdout, "number_supplied:\t%d\n", number_supplied);
			fprintf(stdout, "number_members:\t%d\n", number_members);
			fprintf(stdout, "status:\t%s\n", which_error(status));
			
			if (number_supplied <= 0)	break;
		} while (status == error_status_ok);

		free(member_list);
	} else
	{
		fprintf(stderr, "Unknown API: %s\n", argv[0]);
		usage();
		return(ggapi_s_unexpected);
	}

	return(ggapi_s_expected);
}

int	pre_exec_api(
void
)
{
	error_status_t	status;

	env_registry_site_name = getenv("REGISTRY_SITE_NAME");

	sec_rgy_site_open(env_registry_site_name, &context, &status);
	if (status != error_status_ok) {
		fprintf(stderr, "sec_rgy_site_open failed: %s\n", which_error(status));
		return(ggapi_s_unexpected);
	}

	sec_rgy_site_get(context, &site_name, &status);
	if (status != error_status_ok) {
		fprintf(stderr, "sec_rgy_site_get failed: %s\n", which_error(status));
		return(ggapi_s_unexpected);
	}

	return(ggapi_s_expected);
}

int	post_exec_api (
void
)
{
	error_status_t	status;

	sec_rgy_site_close(context, &status);
	if (status != error_status_ok) {
		fprintf(stderr, "sec_rgy_site_close failed: %s\n", which_error(status));
		return(ggapi_s_unexpected);
	}

	return(ggapi_s_expected);
}

/*
 * main
 */
main(
int	argc,
char	*argv[]
)
{
	error_status_t  status;
	int		rv;

	program_name = argv[0];

	/* Set up the exception handling for this test run */
TRY

	/*  Make a call to the test function */
	rv = pre_exec_api();
	if (rv != ggapi_s_expected) {
		return(rv);
	}

	rv = exec_api(argc - 1, &argv[1]);
	/* save the return value for command exit code */

	post_exec_api();
	/* ignore the return code */

	/* Process any exceptions raised during the call to exec_api() */
CATCH_ALL
	rv = ggapi_s_unexpected;

	/* complete the exception handle block */
ENDTRY

	return(rv);
}
