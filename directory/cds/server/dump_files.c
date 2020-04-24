/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dump_files.c,v $
 * Revision 1.1.10.2  1996/02/18  19:36:12  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:16:44  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:30:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:17  root]
 * 
 * Revision 1.1.7.6  1994/08/24  20:14:34  mccann
 * 	fix server shutdown, add annotation strings, start message delay:OT11239,11245 and 11766
 * 	[1994/08/24  20:08:50  mccann]
 * 
 * Revision 1.1.7.5  1994/08/03  19:04:13  mccann
 * 	fix assert
 * 	[1994/07/21  19:36:59  mccann]
 * 
 * 	final sams cleanup drop
 * 	[1994/07/19  18:25:19  mccann]
 * 
 * Revision 1.1.7.4  1994/06/09  18:43:27  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:15:59  devsrc]
 * 
 * Revision 1.1.7.3  1994/06/08  20:18:01  peckham
 * 	Initialize serviceability and message catalogs.
 * 	[1994/05/31  18:56:47  peckham]
 * 
 * Revision 1.1.7.2  1994/04/14  14:51:19  peckham
 * 	Remove CMA, VMS and UNIX conditionals.
 * 	Remove dnstables dependencies.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	[1994/04/11  20:20:39  peckham]
 * 
 * Revision 1.1.7.1  1994/03/12  22:11:36  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:42:57  peckham]
 * 
 * Revision 1.1.5.5  1992/12/30  14:21:51  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:22:10  zeliff]
 * 
 * Revision 1.1.5.4  1992/12/15  15:55:08  tom
 * 	Bug 6409 - add setlocale call for i18n.
 * 	[1992/12/15  15:17:36  tom]
 * 
 * Revision 1.1.5.3  1992/11/13  15:29:50  peckham
 * 	OT#5072: change abort()s to exit(1) when non-existant file supplied.
 * 	[1992/11/13  15:29:12  peckham]
 * 
 * Revision 1.1.5.2  1992/09/29  19:15:50  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:17:02  weisman]
 * 
 * Revision 1.1.2.2  1992/06/30  19:49:06  peckham
 * 	OT#4475: fix usage output.
 * 	[1992/06/30  19:48:22  peckham]
 * 
 * Revision 1.1  1992/01/19  15:25:35  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990-1994. ALL RIGHTS RESERVED.
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
 *
 *  FACILITY:
 *
 *      DECdns Dump Facility
 *
 *  ABSTRACT:
 *
 *      This module dumps checkpoint and transaction log files.
 *
 */


/*
 *
 *  INCLUDE FILES
 *
 */

#include <server.h>
#include <dce/dcesvcmsg.h>
#include <errno.h>
#include <stdio.h>
#include <ctype.h>
#include <locale.h>
#include <fcntl.h>

#include <dump.h>
#include <dbdef.h>
#include <tlog.h>

static char dashes[80] = "-------------------------------------------------------------------------------";

/*
 * prototypes
 */
int 
main (
    int   ,
    char  **); 

void 
dump_tlog_file (
    char    *);

void 
dump_checkpoint_file (
    char    *);

void 
dump_message_log_file (
    char    *);


/*
 * ROUTINE - dump_tlog_file
 *
 * This routine dumps a name server transaction log file.
 *
 * It accepts a file name as input, opens the file, and reads the transaction
 * records within it.  When a complete transaction record has been assembled
 * in memory, the data structure dumper is called to interpret it.
 */
void 
dump_tlog_file (char *file_name)
{

    tlog_hdr_t *tbuf;

    struct tlog_buffer {
        char filler[BLOCK_SIZE];
        char rest[1];
    } *trest;


    FILE *TLog;
    int read_count;
    int blocks_to_read;
    int entry_number = 0;
    int vbn = 0;
    int sequence_number = -1;

    /*
     * Allocate buffer space for the transaction log.
     * Overlay the other structure definition on top of this buffer
     * via pointers.
     */
    tbuf = (tlog_hdr_t *)malloc(32000);
    trest = (struct tlog_buffer *)tbuf;

    TLog = fopen(file_name,"r");
    if (TLog == NULL) 
    {
	dce_svc_printf(CDS_S_DUMP_NOLOG_EB_MSG, strerror(errno));
    }

    /*
     *  Read through the transaction log.
     *
     *  The first read brings in the header block.  We check that
     *  for validity and stop the dump if anything is awry.
     *  Then we read in the remainder of the transaction entry based
     *  on the block count we find in the header area.
     *  After we've done this, we pass the transaction to the dump
     *  navigator for interpretation.
     */
    while (TRUE) {

        read_count = fread((char *)tbuf, BLOCK_SIZE, 1, TLog);

        if (read_count == 0) 
	{
            break;
        }

        entry_number++;
        vbn++;
        if (entry_number == 1) {
            sequence_number = tbuf->t_seq;
        }

        if (sequence_number == 0) 
	{
            dce_fprintf(cds_s_dump_nulllog);
            break;
        }

        if (tbuf->t_seq != sequence_number) 
	{
	    dce_fprintf(cds_s_dump_mismatch);
            break;
        }

	fprintf(stdout, "%s", dashes);

	dce_fprintf(stdout, cds_s_dump_entry, vbn, entry_number);

	fprintf(stdout, "%s", dashes);

        blocks_to_read = tbuf->t_cnt - 1;
        vbn = vbn + blocks_to_read;

        read_count = fread(trest->rest, BLOCK_SIZE, blocks_to_read, TLog);

        (void)navigate((char *)tbuf, TLOG_STRUCTURE, 0);
    }
}



/*
 * ROUTINE - dump_checkpoint_file
 *
 * This routine dumps a name server checkpoint file.
 *
 * It accepts a file name as input, opens the file, and reads the database
 * buckets within it.  When a complete bucket has been assembled
 * in memory, the data structure dumper is called to interpret it.
 */
void 
dump_checkpoint_file (char *file_name)
{

    directory_bucket_t *cbuf,*oldbuf;
    int bufsiz = MAX_BUCKET_SIZE;

    struct checkpoint_buffer {
        char filler[sizeof(directory_bucket_t)];
        char rest[1];
    } *crest;


    file_header_t checkpoint_header;

    FILE *Check;
    int read_count;
    int blocks_to_read;
    int entry_number;
    int vbn = 0;

    cbuf = (directory_bucket_t *)malloc(bufsiz);
    crest =  (struct checkpoint_buffer *)cbuf;

    Check = fopen(file_name,"r");
    if (Check == NULL) 
    {
	dce_svc_printf(CDS_S_DUMP_NOCHECKPT_EB_MSG, strerror(errno));   
    }

    read_count = fread((char *)&checkpoint_header,
					sizeof(file_header_t), 1, Check);

    if (read_count == 0) {
         return;
    }

    vbn++;


    fprintf(stdout, "%s", dashes);
    dce_fprintf(stdout, cds_s_dump_filehdr, vbn);
    fprintf(stdout, "%s", dashes);

    (void)navigate((char *)&checkpoint_header, CHECKPOINT_HEADER, 0);

    /*
     * Read through the checkpoint file.
     *
     * The first read brings in a directory bucket's worth of data.
     * Directory buckets are the smallest unit of storage in a checkpoint
     * file.  If the bucket header indicates that the entry is actually
     * larger than an directory bucket, additional blocks are read in
     * based on the block count we find in the header area. After we've
     * done this, we pass the entry to the dump navigator for interpretation.
     */

    entry_number = 0;

    while (TRUE) {

        blocks_to_read = sizeof(directory_bucket_t) / BLOCK_SIZE;
        read_count = fread((char *)cbuf, BLOCK_SIZE, blocks_to_read, Check);

        if (read_count == 0) 
	{
            break;
        }

        /*
         * If it isn't a data bucket, then we assume we have a directory
         * bucket of some form.
         */

        if (cbuf->dir_type != DATA_BUCKET) {
            entry_number++;
        }

        if (entry_number > checkpoint_header.hdr_dir_count) 
	{
	    dce_fprintf(stdout, cds_s_dump_unexpecteddir);
            break;
        }

	fprintf(stdout, "%s", dashes);
	dce_fprintf(stdout, cds_s_dump_dir, vbn, entry_number);

	fprintf(stdout, "%s", dashes);

        vbn = vbn + DIRECTORY_BUCKET_BLOCKS;
	if (cbuf->dir_blocks > bufsiz/BLOCK_SIZE) {
	    oldbuf = (directory_bucket_t *)cbuf;
	    bufsiz = (cbuf->dir_blocks + 1) * BLOCK_SIZE;
	    cbuf = (directory_bucket_t *)malloc(bufsiz);
	    crest =  (struct checkpoint_buffer *)cbuf;
	    COPY_bytes( oldbuf, cbuf, blocks_to_read*BLOCK_SIZE);
	    free( oldbuf);
	    }
        blocks_to_read = cbuf->dir_blocks - DIRECTORY_BUCKET_BLOCKS;
        vbn = vbn + blocks_to_read;

        if (blocks_to_read > 0) {
            read_count = fread(crest->rest, BLOCK_SIZE, blocks_to_read, Check);
        }

        (void)navigate((char *)cbuf, CHECKPOINT_STRUCTURE, 0);
    }
}



/*
 * MAIN ROUTINE
 *
 * Gets and checks the arguments, then calls the desired dump routine
 * with the file specification the user specified.
 */
int 
main (int   argc,
      char  *argv[])

#define DUMP_TYPE (tolower(*argv[1]))
#define FILE_TO_DUMP argv[2]

{
    char svcname[128];
    char *myname_p;
    error_status_t st;

    /*
     * Get our filename, stripped of directories
     */
    if (myname_p = strrchr(argv[0], '/'))
	myname_p++;
    else
	myname_p = argv[0];

    setlocale(LC_ALL, "");

    (void)sprintf(svcname, "%s(%ld)", myname_p, (long)getpid());
    dce_svc_set_progname(svcname, &st);
    dce_assert(cds__svc_handle, (st == svc_s_ok));

    dce_msg_define_msg_table(cds_msg_table,
			     sizeof(cds_msg_table) / sizeof(cds_msg_table[0]),
			     &st);
    dce_assert(cds__svc_handle, (st == msg_s_ok));

    if (argc != 3) 
    {
	/*
	 * wrong number of args - print usage
	 */
	dce_fprintf(stdout, cds_s_dump_usage_eb, argv[0]);
    }

    dthread_init();

    if (DUMP_TYPE == 'c') 
    {
        dump_checkpoint_file(FILE_TO_DUMP);
    } 
    else
	if (DUMP_TYPE == 't') 
	{
	    dump_tlog_file(FILE_TO_DUMP);
	} 
	else 
	    {
		/*
		 * Invalid cmd line arg - print usage
		 */
	      dce_fprintf(stdout, cds_s_dump_usage_eb, argv[0]);
	    }
    exit(1);
}
