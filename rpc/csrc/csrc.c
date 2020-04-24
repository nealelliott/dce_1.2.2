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
 * $Log: csrc.c,v $
 * Revision 1.1.4.2  1996/02/18  18:48:16  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:02:44  marty]
 *
 * Revision 1.1.4.1  1995/12/08  00:11:17  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/01/05  22:00 UTC  kline_s
 * 
 * 	HP revision /main/kline_s_mothra_latest/1  1995/01/05  21:16 UTC  kline_s
 * 	Add exit(0) at end of main to prevent clearcase from trapping
 * 	spurious return status codes.
 * 	[1995/12/07  23:53:53  root]
 * 
 * Revision 1.1.2.4  1994/07/27  21:37:23  mori_m
 * 	CR: 11341  Use dce_strdup
 * 	[1994/07/27  21:31:57  mori_m]
 * 
 * Revision 1.1.2.3  1994/06/10  20:53:29  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:59:38  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/06  20:59:03  mori_m
 * 	CR 10031: Added "#if defined(NEED_STRDUP)" around strdup routine.
 * 	CR 9702:  Added default input file path.
 * 	[1994/05/06  20:54:40  mori_m]
 * 
 * Revision 1.1.2.1  1994/02/08  21:45:52  mori_m
 * 	CR 9702:  Initial submission for RPC runtime I18N support
 * 	[1994/02/08  21:45:41  mori_m]
 * 
 * $EndLog$
 */

#include <stdio.h>
#include <string.h>
#include <csrc.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <dce/rpc.h>
#include <dcecsrmsg.h>

static char	*csrc_g_input = FALSE;	/* Input file name */
static char	*csrc_g_output = FALSE;	/* Output file name */
entry_t		*csrc_g_entries;
int		csrc_g_entry_count;
int		csrc_g_valid_entry;
int		csrc_g_errors;
int		csrc_g_recsize;

/* 
 *  Default input file
 */
static char	def_input_file[] = "/usr/lib/nls/csr/code_set_registry.txt";


/*
**  Print a usage message and exit.
*/
static void
Usage(
	void
)
{
	dce_fprintf(stderr, csrc_s_usage);
	exit(1);
}

/*
**
** Iterative Insertion Sort for code set value.
**
*/
void c_sort (
	entry_t		**codesets, 
	int		entry_count
)
{
	int	i, j;
	long	code1, code2;
	entry_t	*temp;

	if (entry_count <= 1)
		return;

	for (i = 1; i < entry_count; i++)
	{
		temp = codesets[i];
		j = i - 1;
		while (j >= 0)
		{
			/* Little trick to deal with a big number */
			if (codesets[j]->code_set < 0)
				code1 = codesets[j]->code_set * -1;
			else
				code1 = codesets[j]->code_set;

			if (temp->code_set < 0)
				code2 = temp->code_set * -1;
			else
				code2 = temp->code_set;

			if (code1 > code2)
			{
				codesets[j + 1] = codesets[j];
				codesets[j] = temp;
			}
			j--;
		}
	}
}

int
main (
	int	ac,
	char	*av[]
)
{
	int	i, j, k, l, n;
	entry_t	*ep;
	entry_t	**sort_id_codesets;
	entry_t	**sort_id_save;
	int	CsrFile;
	FILE	*log;
	error_status_t	msg_st;
	unsigned char	*intermediate_codesets[CS_CSRC_MAX_INTERMEDIATE];
	int		intermediate_flags[CS_CSRC_MAX_INTERMEDIATE];
	int		intermediate_count;
	char		*csrgy_file_name;
	error_status_t	status;

	dce_msg_define_msg_table (dce_csr_g_table,
				  sizeof dce_csr_g_table / sizeof dce_csr_g_table[0],
				  &msg_st);

	if (msg_st != rpc_s_ok)
		fprintf(stderr, "Unable to open message table\n");

	if (ac == 1)
		Usage();

	j = 0;
	while ((i = getopt(ac, av, "i:o:m:")) != EOF)
		switch (i)
		{
		default:
			Usage();
			/* NOTREACHED */
		case 'i':
			csrc_g_input = optarg;
			break;
		case 'o':
			csrc_g_output = optarg;
			break;
		case 'm':
			if (j >= CS_CSRC_MAX_INTERMEDIATE)
			{
				dce_fprintf(stderr, csrc_s_too_many_intermediate, j);
				for (n = 0; n < j; n++)
					free(intermediate_codesets[n]);
				exit(1);
			}

			if ((intermediate_codesets[j] 
			    = (unsigned char *)malloc(strlen(optarg))) == NULL)
			{
				dce_fprintf(stderr, csrc_s_no_memory);
				exit(1);
			}
			
			if ((strcpy((char *)(intermediate_codesets[j]), optarg)) == NULL)
			{
				dce_fprintf(stderr,  csrc_s_cannot_copy_intermediate, optarg);
				free(intermediate_codesets[j]);
				exit(1);
			}
			intermediate_flags[j] = CS_CSRC_NOT_FOUND;
			j++;
			
			break;
		}

	intermediate_count = j;

	ac -= optind;
	av += optind;
	if (!csrc_g_input)
		csrc_g_input = def_input_file;

	/*
	** Get space to store the records
	*/
	csrc_g_entry_count = 0;
	csrc_g_recsize = REC_CHUNK;
	csrc_g_entries = (entry_t *)malloc(csrc_g_recsize * sizeof *csrc_g_entries);

	/*
	** Parse the input
	*/
	yyopen(csrc_g_input);
	(void)yyparse();
	if (csrc_g_errors) {
		(void)dce_fprintf(stderr, csrc_s_no_output);
		exit(1);
	}

	if (csrc_g_output)
	{
		if ((CsrFile = creat(csrc_g_output, 0666)) == -1) {
			(void)dce_fprintf(stderr, csrc_s_cannot_open_output,
				csrc_g_output, strerror(errno));
			exit(1);
		}
	}
	else	/* Use default output file name */

	{
		if ((CsrFile = creat("/usr/lib/nls/csr/code_set_registry.db", 0666)) == -1) {
			(void)dce_fprintf(stderr, csrc_s_cannot_open_output,
				"/usr/lib/nls/loc/code_set_registry.db", strerror(errno));
			exit(1);
		}
	}

	if ((log = fopen("CSRC_LOG", "w")) == NULL) {
		(void)dce_fprintf(stderr, csrc_s_cannot_open_logfile,
			"CSRC_LOG", strerror(errno));
		exit(1);
	}
	/***                         ***/
	/***  Sort by code set value ***/
	/***                         ***/
	ep = csrc_g_entries;
	i = csrc_g_entry_count;

	if ((sort_id_codesets = (entry_t **)malloc(sizeof(entry_t *) * csrc_g_entry_count)) == NULL)
	{
		dce_fprintf(stderr, csrc_s_no_memory);
		exit(1);
	}
	sort_id_save = sort_id_codesets;
	while (i--)
	{
		*sort_id_codesets++ = ep++;
	}
	sort_id_codesets = sort_id_save;
	c_sort(sort_id_codesets, csrc_g_entry_count);


	i = csrc_g_entry_count;
	ep = csrc_g_entries;
	write(CsrFile, (char *)(&csrc_g_entry_count), sizeof(csrc_g_entry_count));
	write(CsrFile, (char *)(&csrc_g_valid_entry), sizeof(csrc_g_valid_entry));

	fprintf(log, "Total CSRC Entry Count = %d\n", csrc_g_entry_count);
	fprintf(log, "Effective CSRC Entry Count = %d\n\n", csrc_g_valid_entry);
	fprintf(log, "Intermediate Code Set Count = %d\n", intermediate_count);
	fprintf(log, "Intermediate Code Set:\n");
	for (n = 0; n < intermediate_count; n++)
		fprintf(log, "\tPriority = %d  Code Set Name = %s\n", 
				n + 1, intermediate_codesets[n]);
	fprintf(log, "\n");


	k = 0;
	while (i--)
	{
		for (n=0; n < intermediate_count; n++)
		{
			if (intermediate_flags[n])
			{
			   if ((l = strcoll((*sort_id_codesets)->code_set_name,
					(char *)intermediate_codesets[n])) == 0)
			   {
				(*sort_id_codesets)->priority = n + 1;
				intermediate_flags[n] = CS_CSRC_FOUND;
			   }
			}
		}


		write(CsrFile, (char *)(&((*sort_id_codesets)->code_set)), sizeof((*sort_id_codesets)->code_set));

		write(CsrFile, (char *)(&((*sort_id_codesets)->code_name_len)), sizeof((*sort_id_codesets)->code_name_len));
		write(CsrFile, (*sort_id_codesets)->code_set_name, ((*sort_id_codesets)->code_name_len + 1));
		write(CsrFile, (char *)((&(*sort_id_codesets)->char_sets_num)), sizeof((*sort_id_codesets)->char_sets_num));
		for (j = 0; j < (*sort_id_codesets)->char_sets_num; j++)
			write(CsrFile, (char *)((&(*sort_id_codesets)->char_sets[j])), sizeof((*sort_id_codesets)->char_sets[j]));

		write(CsrFile, (char *)((&(*sort_id_codesets)->max_bytes)), sizeof((*sort_id_codesets)->max_bytes));

		write(CsrFile, (char *)((&(*sort_id_codesets)->priority)), sizeof((*sort_id_codesets)->priority));


		sort_id_codesets++;

		/*
		** Write a log file
		*/
		fprintf(log, " %d: code_set = %lX\n", k++, ep->code_set);
		fprintf(log, "  : code_name_len = %d\n", ep->code_name_len);
		fprintf(log, "  : code_set_name = %s\n", ep->code_set_name);
		fprintf(log, "  : char_sets_num = %d\n", ep->char_sets_num);
		for (j = 0; j < ep->char_sets_num; j++)
			fprintf(log, "  : char_sets[%d] = %04X\n", j, (0x0000FFFF & ep->char_sets[j]));
		fprintf(log, "  : max_bytes = %d\n\n", ep->max_bytes);

		ep++;
	}
	close(CsrFile);
	fclose(log);
	free(csrc_g_entries);
	free(sort_id_codesets);

	for (i=0; i < intermediate_count; i++)
		free(intermediate_codesets[i]);
		
	exit(0);
}
