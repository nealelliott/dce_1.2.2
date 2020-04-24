/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: db_unix.c,v $
 * Revision 1.1.10.2  1996/02/18  19:36:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:16:37  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:30:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:10  root]
 * 
 * Revision 1.1.7.4  1994/08/03  19:04:08  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:24:52  mccann]
 * 
 * Revision 1.1.7.3  1994/06/09  18:43:21  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:15:52  devsrc]
 * 
 * Revision 1.1.7.2  1994/04/14  14:51:16  peckham
 * 	Remove CMA, VMS and UNIX conditionals.
 * 	Remove dnstables dependencies.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	[1994/04/11  20:20:15  peckham]
 * 
 * Revision 1.1.7.1  1994/03/12  22:11:16  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:42:11  peckham]
 * 
 * Revision 1.1.5.3  1992/12/30  14:21:33  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:21:45  zeliff]
 * 
 * Revision 1.1.5.2  1992/09/29  19:15:31  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:16:50  weisman]
 * 
 * Revision 1.1.2.2  1992/06/30  19:25:20  peckham
 * 	OT#3745: Replace sys_errno() with deb_ascii_errno()
 * 	[1992/06/30  19:24:40  peckham]
 * 
 * Revision 1.1  1992/01/19  15:25:51  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module db_unix.c
 *
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
 * MODULE DESCRIPTION:
 *
 * Networks & Communications Software Engineering
 *
 */

#include <server.h>
#include <unistd.h>
#include <dce/assert.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

#include <back.h>
#include <dbdef.h>
#include <dns_strings.h>

/*
 * The dthreads package for DECnet has non-preemptive scheduling which
 * can cause lockouts of threads performing I/O.   To best simulate a
 * more complete threads package, we yield control to any runnable threads
 * after I/Os have completed.
 */

# define NON_PREEMPTIVE_YIELD

typedef struct file_info_unix {
	int	file_extension_size;
	int	file_fd;
#define MAX_FILE_PATH (OS_DATA_SIZE-2*sizeof(int))
	char	file_path[MAX_FILE_PATH];
} file_info_unix_t;

static char *
deb_ascii_file_info_unix (
    char *const buf,
    const void *const ptr);
#define DEB_ASCII_LEN_FILE_INFO_UNIX \
    (DEB_ASCII_LEN_ptr+DEB_ASCII_LEN_int+MAX_FILE_PATH+4)
#define DEB_ASCII_BUF_file_info_unix(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_FILE_INFO_UNIX)
#define LOG_ASCII_BUF_file_info_unix(nam) \
	LOG_ASCII_BUF(nam,DEB_ASCII_LEN_FILE_INFO_UNIX)

static char 
*deb_ascii_file_info_unix (char *const        buf,
                           const void *const  ptr)
{
    const file_info_unix_t *const file_p =(file_info_unix_t *)ptr;
    char *buf_p = (char *)buf;
    int file_path_len;

    if (file_p) {
	buf_p = deb_ascii_ptr_to_buf(buf_p, ptr);
	if (file_p->file_path[0] != '\0') {
	    file_path_len = strlen(file_p->file_path);
	    dce_assert(cds__svc_handle,
		       (file_path_len < MAX_FILE_PATH));

	    if (0 <= file_p->file_fd)
		(void)sprintf(buf_p, "->{%d,%s}", file_p->file_fd,
							file_p->file_path);
	    else
		(void)sprintf(buf_p, "->%s", file_p->file_path);
	}

	return(buf);
    } else
	return("NULL file_info_unix_t");
}

static int file_mode = 0600;	/* default mode for file creation */

static char version_str[]    = DNS_SVR_VERSION;
static char tlog_str[]       = DNS_SVR_TLOG;
static char checkpoint_str[] = DNS_SVR_CKPT;

static char version_format[] = DNS_VERSION_FMT; /* for sprintf */

/*
 * local prototypes
 */
static dns_status_t
make_name (
    file_info_unix_t	*,
    char		*,
    char		*,
    int			);

static dns_status_t
create_file (
    file_info_unix_t	*,
    int			,
    int			);

static dns_status_t
open_file (
    file_info_unix_t	*);


/*
 * make_name
 *	Construct file_path for file access
 *
 * Input:
 *	data_p = address of OS file block
 *	ch_filename_p = pointer to clearinghouse file name
 *	type_p = pointer to file type string
 *	sequence = file sequence number:
 *		-1 = no file sequence number
 *		 0 = current sequence number from version file
 *		 n = sequence number 'n'
 *
 * NOTE:
 *	To be totally safe, this code should be checking lengths to
 *	avoid buffer overflow. Stick this in on the next pass!!!
 */
static dns_status_t 
make_name (file_info_unix_t  *data_p,
           char              *ch_filename_p,
           char              *type_p,
           int               sequence)
{
    DEB_ASCII_BUF_file_info_unix(dataBuf)
    DEB_ASCII_BUF_errno(errnoBuf)
    char *ptr = data_p->file_path;
    dns_status_t status = DNS_SUCCESS;

    /* copy bas file name in */
    (void)strcpy(ptr, ch_filename_p);

    ptr = strchr(ptr, '\0');

    /* copy in file type */
    (void)strcpy(ptr, type_p);
    ptr += strlen(ptr);

    /* check if extension requested */
    if (sequence == 0) {
	file_info_unix_t data;
	register int u_status;

	status = make_name(&data, ch_filename_p, version_str, -1);
	if (status != DNS_SUCCESS)
	    goto leave_make_name;

	data.file_fd = u_status = open(data.file_path, O_RDONLY, 0);
	if (0 <= u_status) {
	    u_status = read(data.file_fd, ptr, DNS_VERSION_SIZE);

	    if (u_status != DNS_VERSION_SIZE)
		dce_svc_printf(CDS_S_SERVER_READ_MSG, data.file_path,
			       (long)errno);
	    (void)close(data.file_fd);
	} else
	    dce_svc_printf(CDS_S_SERVER_OPEN_MSG, data.file_path,
			       (long)errno);

	if (0 <= u_status)
	    ptr[DNS_VERSION_SIZE] = 0;
	else
	    status = DNS_DATACORRUPTION;
    } else if (0 < sequence) {
	(void)sprintf(ptr, version_format, sequence);
	ptr[DNS_VERSION_SIZE] = 0;
    }

leave_make_name:
    return(status);
}


/* -------------------------------------------------------------------------- */


/*
 * create_file
 *	Create a file for the clearinghouse.
 *
 * Input:
 *	data_p = address of OS file block
 *	extension = file extension size
 *	length = initial file length (zero-filled)
 */
static dns_status_t 
create_file (file_info_unix_t  *data_p,
             int               extension,
             int               length)
{
    DEB_ASCII_BUF_file_info_unix(dataBuf)
    DEB_ASCII_BUF_errno(errnoBuf)
    dns_status_t status;

    data_p->file_fd = open(data_p->file_path,
	O_CREAT|O_RDWR|O_TRUNC|O_SYNC, file_mode);

    if (0 <= data_p->file_fd) 
    {
	char data[100];

	ZERO_bytes(data, sizeof(data));
	data_p->file_extension_size = extension;

	while (0 < length) {
	    int count = length;

	    if (count > sizeof(data)) count = sizeof(data);

	    (void)write(data_p->file_fd, data, count);
	    length -= count;
	}
	/* set_eof(length) */
	status = DNS_SUCCESS;
    } else {
      dce_svc_printf(CDS_S_SERVER_CREATE_MSG, data_p->file_path, (long)errno);
      status = DNS_NAMESERVERBUG;
    }

    return(status);
}


/* -------------------------------------------------------------------------- */


/*
 * open_file
 *	Open a file for the clearinghouse.
 *
 * Input:
 *	os_p = address of OS file block
 */
static dns_status_t 
open_file (file_info_unix_t *data_p)
{
    DEB_ASCII_BUF_file_info_unix(dataBuf)
    DEB_ASCII_BUF_errno(errnoBuf)
    dns_status_t status;

    data_p->file_fd = open(data_p->file_path, O_RDWR, 0);

    if (0 <= data_p->file_fd)
	status = DNS_SUCCESS;
    else {
      dce_svc_printf(CDS_S_SERVER_OPEN_MSG, data_p->file_path, (long)errno);
      status = DNS_NAMESERVERBUG;
    }

    return(status);
}

/*
 * sys_write
 *	Write bytes to the file synchronously
 *
 * Input:
 *      os_p = pointer to file 
 *	bucket_p = pointer to bucket
 *	block_number = starting VBN (VBNs start at 1)
 *	count = block_count
 *
 * Value:
 *	status
 */
dns_status_t 
sys_write (byte_t         *os_p,
           unsigned char  *bucket_p,
           unsigned int   block_number,
           unsigned int   count)
{
    DEB_ASCII_BUF_file_info_unix(osBuf)
    DEB_ASCII_BUF_errno(errnoBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    file_info_unix_t *const file_p = (file_info_unix_t *)os_p;
    const off_t offset = (block_number - 1)*BLOCK_SIZE;
    const int bytes = count*BLOCK_SIZE;
    int u_status;
    dns_status_t status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " >sys_write(%s,%p,%u,%u)",
        deb_ascii_file_info_unix(osBuf,os_p),
        bucket_p,
        block_number,
        count));

    u_status = lseek(file_p->file_fd, offset, SEEK_SET);
    if (0 <= u_status) {
	u_status = write(file_p->file_fd, (char *)bucket_p, bytes);

	if (u_status == bytes) {
            NON_PREEMPTIVE_YIELD;
	    status = DNS_SUCCESS;
	    goto leave_sys_write;
        }

	dce_svc_printf(CDS_S_SERVER_WRITE_MSG, file_p->file_path, (long)errno);

	if (u_status < 0)
	    switch (errno) {
	    case EFBIG:		/* file exceeds process file size limit */
	    case ENOSPC:	/* no free space on file system */
#if defined(EDQUOT)
	    case EDQUOT:	/* user's block quota has been exhausted */
#endif
                NON_PREEMPTIVE_YIELD;
		status = DNS_NONSRESOURCES;
		goto leave_sys_write;
	    }
    } else
	dce_svc_printf(CDS_S_SERVER_LSEEK_MSG, file_p->file_path, (long)errno);

    NON_PREEMPTIVE_YIELD;
    status = DNS_NAMESERVERBUG;

leave_sys_write:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " <sys_write(%s,%p,%u,%u) return(%s)",
        deb_ascii_file_info_unix(osBuf,os_p),
        bucket_p,
        block_number,
        count,
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}


/* -------------------------------------------------------------------------- */


/*
 * sys_safe_write
 *	Write bytes to the file synchronously; force bytes to be written NOW.
 *
 * Input:
 *      os_p = pointer to file 
 *	bucket_p = pointer to bucket
 *	block_number = starting VBN (VBNs start at 1)
 *	count = block_count
 *
 * Value:
 *	status
 */
dns_status_t 
sys_safe_write (byte_t         *os_p,
                unsigned char  *bucket_p,
                unsigned int   block_number,
                unsigned int   count)
{
    DEB_ASCII_BUF_file_info_unix(osBuf)
    DEB_ASCII_BUF_errno(errnoBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    file_info_unix_t *const file_p = (file_info_unix_t *)os_p;
    const off_t offset = (block_number - 1)*BLOCK_SIZE;
    const int bytes = count*BLOCK_SIZE;
    int u_status;
    dns_status_t status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " >sys_safe_write(%s,%p,%u,%u)",
        deb_ascii_file_info_unix(osBuf,os_p),
        bucket_p,
        block_number,
        count));

    u_status = lseek(file_p->file_fd, offset, SEEK_SET);
    if (0 <= u_status) {
	u_status = write(file_p->file_fd, (char *)bucket_p, bytes);

	if (u_status == bytes) {
            while (fsync(file_p->file_fd) < 0)
		if (errno != EINTR)
		    break;

            NON_PREEMPTIVE_YIELD;
	    status = DNS_SUCCESS;
	    goto leave_sys_safe_write;
        }

	dce_svc_printf(CDS_S_SERVER_WRITE_MSG, file_p->file_path, (long)errno);
	if (u_status < 0)
	    switch (errno) {
	    case EFBIG:		/* file exceeds process file size limit */
	    case ENOSPC:	/* no free space on file system */
#if defined(EDQUOT)
	    case EDQUOT:	/* user's block quota has been exhausted */
#endif
                NON_PREEMPTIVE_YIELD;
		status = DNS_NONSRESOURCES;
		goto leave_sys_safe_write;
	    }
    } else
	dce_svc_printf(CDS_S_SERVER_LSEEK_MSG, file_p->file_path, (long)errno);

    NON_PREEMPTIVE_YIELD;
    status = DNS_NAMESERVERBUG;

leave_sys_safe_write:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " <sys_safe_write(%s,%p,%u,%u) return(%s)",
        deb_ascii_file_info_unix(osBuf,os_p),
        bucket_p,
        block_number,
        count,
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}


/* -------------------------------------------------------------------------- */


/*
 * sys_read
 *	Read bytes from the file synchronously
 *
 * Input:
 *      os_p = pointer to file
 *	bucket_p = pointer to bucket
 *	block_number = starting VBN (VBNs start at 1)
 *	count = block count
 *
 * Value:
 *	status
 */
dns_status_t 
sys_read (byte_t         *os_p,
          unsigned char  *bucket_p,
          unsigned int   block_number,
          int            count)
{
    DEB_ASCII_BUF_file_info_unix(osBuf)
    DEB_ASCII_BUF_errno(errnoBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    file_info_unix_t *const file_p = (file_info_unix_t *)os_p;
    const off_t offset = (block_number - 1)*BLOCK_SIZE;
    const int bytes = count*BLOCK_SIZE;
    int u_status;
    dns_status_t status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " >sys_read(%s,%p,%u,%d)",
        deb_ascii_file_info_unix(osBuf,os_p),
        bucket_p,
        block_number,
        count));

    u_status = lseek(file_p->file_fd, offset, SEEK_SET);
    if (0 <= u_status) {
	u_status = read(file_p->file_fd, (char *)bucket_p, bytes);

	if (u_status == bytes) {
            NON_PREEMPTIVE_YIELD;
	    status = DNS_SUCCESS;
	    goto leave_sys_read;
	}

	dce_svc_printf(CDS_S_SERVER_READ_MSG, file_p->file_path, (long)errno);

    } else
	dce_svc_printf(CDS_S_SERVER_LSEEK_MSG, file_p->file_path, (long)errno);

    NON_PREEMPTIVE_YIELD;
    status = DNS_NAMESERVERBUG;

leave_sys_read:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " <sys_read(%s,%p,%u,%d) return(%s)",
        deb_ascii_file_info_unix(osBuf,os_p),
        bucket_p,
        block_number,
        count,
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}


/* -------------------------------------------------------------------------- */


/*
 * sys_close
 *	Close the file and deassign channel
 *
 * Input:
 *	os_p = pointer to file 
 *
 */
dns_status_t 
sys_close (byte_t *os_p)
{
    DEB_ASCII_BUF_file_info_unix(osBuf)
    DEB_ASCII_BUF_errno(errnoBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    file_info_unix_t *const file_p = (file_info_unix_t *)os_p;
    const int fd = file_p->file_fd;
    dns_status_t status = DNS_SUCCESS;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " >sys_close(%s)",
        deb_ascii_file_info_unix(osBuf,os_p)));

    file_p->file_fd = -1;
    if (close(fd) < 0) {
	dce_svc_printf(CDS_S_SERVER_CLOSE_MSG, file_p->file_path, (long)errno);
	status = DNS_NAMESERVERBUG;
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " <sys_close(%s) return(%s)",
        deb_ascii_file_info_unix(osBuf,os_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}


/* -------------------------------------------------------------------------- */


/*
 * sys_delete
 *	Delete a file. The file is open on input.
 *
 * Input:
 *	os_p = pointer to OS file data.  
 */
dns_status_t 
sys_delete (byte_t *os_p)
{
    DEB_ASCII_BUF_file_info_unix(osBuf)
    DEB_ASCII_BUF_errno(errnoBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    file_info_unix_t *const file_p = (file_info_unix_t *)os_p;
    int u_status;
    dns_status_t status = DNS_SUCCESS;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " >sys_delete(%s)",
        deb_ascii_file_info_unix(osBuf,os_p)));

    u_status = unlink(file_p->file_path);

    if (0 <= file_p->file_fd)
	(void)sys_close(os_p);

    if (u_status < 0) 
    {
	dce_svc_printf(CDS_S_SERVER_UNLINK_MSG, file_p->file_path, (long)errno);
	status = DNS_NAMESERVERBUG;
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " <sys_delete(%s) return(%s)",
        deb_ascii_file_info_unix(osBuf,os_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}


/* -------------------------------------------------------------------------- */


/*
 * update_version
 *	This routine updates the version number stored in the version file.
 *
 */
dns_status_t 
update_version (char          *ch_filename_p,
                unsigned int  sequence)
{
    DEB_ASCII_BUF_errno(errnoBuf)
    file_info_unix_t data;
    char version_name[DNS_VERSION_SIZE+1];
    int count;
    dns_status_t status;

    status = make_name(&data, ch_filename_p, version_str, -1);
    if (status != DNS_SUCCESS)
	goto leave_update_version;

    data.file_fd = open(data.file_path, O_RDWR|O_CREAT, file_mode);
    if (data.file_fd != -1) {
	/* Convert sequence number to ascii text with leading zeros */
	(void)sprintf(version_name, version_format, sequence);
	/* ERROR FROM SPRINTF ??? */
	version_name[DNS_VERSION_SIZE] = '\n';

	count = write(data.file_fd, version_name, strlen(version_name));
	if (count != -1)
	    status = DNS_SUCCESS;
	else 
	{
	  dce_svc_printf(CDS_S_SERVER_WRITE_MSG, data.file_path, (long)errno);

	    switch (errno) {
	    case EFBIG:		/* file exceeds process file size limit */
	    case ENOSPC:	/* no free space on file system */
#if defined(EDQUOT)
	    case EDQUOT:	/* user's block quota has been exhausted */
#endif
		status = DNS_NONSRESOURCES;
		break;
	    default:
	        status = DNS_NAMESERVERBUG;
		break;
	    }
	    status = DNS_DATACORRUPTION;
	}

	(void)close(data.file_fd);
    } else 
    {
	dce_svc_printf(CDS_S_SERVER_OPEN_MSG, data.file_path, (long)errno);
	status = DNS_NAMESERVERBUG;
    }

leave_update_version:
    return(status);
}


/* -------------------------------------------------------------------------- */


/*
 * sys_create_checkpoint
 *	Create a checkpoint file for the clearinghouse.
 *
 * Input:
 *	ch_filename_p = pointer to name of checkpoint file
 *	os_p = address of OS file block
 *	sequence = file sequence number to be used as part of file extension
 */
dns_status_t 
sys_create_checkpoint (char          *ch_filename_p,
                       byte_t        *os_p,
                       unsigned int  sequence)
{
    DEB_ASCII_BUF_file_info_unix(osBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    file_info_unix_t *const data_p = (file_info_unix_t *)os_p;
    dns_status_t status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " >sys_create_checkpoint(%s,%s,%u)",
        ch_filename_p,
        deb_ascii_file_info_unix(osBuf,os_p),
        sequence));

    if (0 < sequence) {
	status = make_name(data_p, ch_filename_p, checkpoint_str, sequence);

	if (status == DNS_SUCCESS)
	    status = create_file(data_p, 500, 2*500);
    } else
	status = DNS_NAMESERVERBUG;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " <sys_create_checkpoint(%s,%s,%u) return(%s)",
        ch_filename_p,
        deb_ascii_file_info_unix(osBuf,os_p),
        sequence,
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}


/* -------------------------------------------------------------------------- */


/*
 * sys_delete_version
 *	Deletes (removes) version file from the file system.
 *
 * Input:
 *	ch_filename_p = pointer to version file name
 *
 * Returns:
 *	DNS_SUCCESS - version file removed/deleted/unlinked (whatever)
 *	DNS_DATACORRUPTION - error removing version file
 */
dns_status_t 
sys_delete_version (char *ch_filename_p)
{
    DEB_ASCII_BUF_errno(errnoBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    file_info_unix_t data;
    dns_status_t status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " >sys_delete_version(%s)",
        ch_filename_p));

    /*
     * Obtain base file name from clearinhouse data structure.
     */

    status = make_name(&data, ch_filename_p, version_str, -1);
    if (status == DNS_SUCCESS) {
	register int u_status;

	u_status = unlink(data.file_path);
	if (u_status < 0) 
	{
	  dce_svc_printf(CDS_S_SERVER_UNLINK_MSG, data.file_path, (long)errno);
	  status = DNS_DATACORRUPTION;
       }
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " <sys_delete_version(%s) return(%s)",
        ch_filename_p,
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}


/* -------------------------------------------------------------------------- */


/*
 * sys_delete_checkpoint
 *	Find current checkpoint file and delete it. The file is assumed
 *	not to opened.
 *
 *
 * Input:
 *	ch_filename_p = pointer to checkpoint file name
 *
 */
dns_status_t 
sys_delete_checkpoint (char *ch_filename_p)
{
    DEB_ASCII_BUF_errno(errnoBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    file_info_unix_t data;
    dns_status_t status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " >sys_delete_checkpoint(%s)",
        ch_filename_p));

    status = make_name(&data, ch_filename_p, checkpoint_str, 0);
    if (status == DNS_SUCCESS) {
	int u_status;

	u_status = unlink(data.file_path);
	if (u_status < 0) 
	{
	  dce_svc_printf(CDS_S_SERVER_UNLINK_MSG, data.file_path, (long)errno);
	  status = DNS_NAMESERVERBUG;
	}
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " <sys_delete_checkpoint(%s) return(%s)",
        ch_filename_p,
        deb_ascii_dns_status(statusBuf, status)));
    return(status);
}


/* -------------------------------------------------------------------------- */


/*
 * sys_create_tlog
 *	Create the transaction log file.
 *
 * Input:
 *	ch_filename_p = pointer to of checkpoint file name
 *	os_p = address of os data block
 *	sequence = sequence number to be used as file extension
 */
dns_status_t 
sys_create_tlog (char          *ch_filename_p,
                 byte_t        *os_p,
                 unsigned int  sequence)
{
    DEB_ASCII_BUF_file_info_unix(osBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    file_info_unix_t *const tlog_p = (file_info_unix_t *)os_p;
    dns_status_t status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " >sys_create_tlog(%s,%s,%u)",
        ch_filename_p,
        deb_ascii_file_info_unix(osBuf,os_p),
        sequence));

    if (0 < sequence) {
	status = make_name(tlog_p, ch_filename_p, tlog_str, sequence);

	if (status == DNS_SUCCESS)
	    status = create_file(tlog_p, 200, 200);
    } else
	status = DNS_NAMESERVERBUG;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " >sys_create_tlog(%s,%s,%u) return(%s)",
        ch_filename_p,
        deb_ascii_file_info_unix(osBuf,os_p),
        sequence,
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}


/* -------------------------------------------------------------------------- */


/*
 * sys_open_checkpoint
 *	Open the last valid checkpoint file.
 *
 * Input:
 *	ch_filename_p = pointer to checkpoint file name
 *	os_p = address of data file block
 *
 */
dns_status_t 
sys_open_checkpoint (char    *ch_filename_p,
                     byte_t  *os_p)
{
    DEB_ASCII_BUF_file_info_unix(osBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    file_info_unix_t *const data_p = (file_info_unix_t *)os_p;
    dns_status_t status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " >sys_open_checkpoint(%s,%s)",
        ch_filename_p,
        deb_ascii_file_info_unix(osBuf,os_p)));

    status = make_name(data_p, ch_filename_p, checkpoint_str, 0);
    if (status == DNS_SUCCESS)
	status = open_file(data_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " <sys_open_checkpoint(%s,%s) return(%s)",
        ch_filename_p,
        deb_ascii_file_info_unix(osBuf,os_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}


/* -------------------------------------------------------------------------- */


/*
 * sys_open_tlog
 *	Open the last valid transaction log.
 *
 * Input:
 *	ch_filename_p = pointer to checkpoint file name
 *	os_p = address of data file block
 *
 */
dns_status_t 
sys_open_tlog (char    *ch_filename_p,
               byte_t  *os_p)
{
    DEB_ASCII_BUF_file_info_unix(osBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    file_info_unix_t *const tlog_p = (file_info_unix_t *)os_p;
    dns_status_t status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " >sys_open_tlog(%s)",
        ch_filename_p));

    status = make_name(tlog_p, ch_filename_p, tlog_str, 0);
    if (status == DNS_SUCCESS)
	status = open_file(tlog_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " <sys_open_tlog(%s,%s) return(%s)",
        ch_filename_p,
        deb_ascii_file_info_unix(osBuf,os_p),
	deb_ascii_dns_status(statusBuf,status)));
    return(status);
}


/* -------------------------------------------------------------------------- */


/*
 * sys_extend
 *	Extend the file, file must already be opened.
 *
 * Input:
 * 	os_p = pointer to file 
 *
 *
 */
dns_status_t 
sys_extend (byte_t *os_p)
{
    DEB_ASCII_BUF_file_info_unix(osBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    const dns_status_t status = DNS_SUCCESS;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " >sys_extend(%s)",
        deb_ascii_file_info_unix(osBuf,os_p)));

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " <sys_extend(%s) return(%s)",
        deb_ascii_file_info_unix(osBuf,os_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}


/* -------------------------------------------------------------------------- */


/*
 * sys_purge
 *
 *	This routine deletes the previous version of the checkpoint and
 *	log files.
 *
 * Input:
 *	ch_filename_p = pointer to clearinghouse file name
 *	sequence = sequence number used in file types
 */
void 
sys_purge (char          *ch_filename_p,
           unsigned int  sequence)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " >sys_purge(%s,%u)",
        ch_filename_p,
        sequence));

    if (0 < sequence) {
	file_info_unix_t data;

	data.file_fd = -1;

	if (make_name(&data, ch_filename_p, checkpoint_str, sequence)
								== DNS_SUCCESS)
	    (void)sys_delete((byte_t *)&data);

	if (make_name(&data, ch_filename_p, tlog_str, sequence) == DNS_SUCCESS)
	    (void)sys_delete((byte_t *)&data);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " <sys_purge(%s,%u)",
        ch_filename_p,
        sequence));
}


/* -------------------------------------------------------------------------- */


/*
 * sys_filesize
 *	Return size of file in blocks
 *
 * Input:
 *      os_p = pointer to file
 *
 * Value:
 *	size of file in blocks
 */
int 
sys_filesize (byte_t *os_p)
{
    DEB_ASCII_BUF_file_info_unix(osBuf)
    DEB_ASCII_BUF_errno(errnoBuf)
    file_info_unix_t *const file_p = (file_info_unix_t *)os_p;
    register int size;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " >sys_filesize(%s)",
        deb_ascii_file_info_unix(osBuf,os_p)));

    if (0 <= (size = lseek(file_p->file_fd, 0, SEEK_END)))
	size /= BLOCK_SIZE;
    else
	dce_svc_printf(CDS_S_SERVER_LSEEK_MSG, file_p->file_path, (long)errno);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " <sys_filesize(%s) return(%d)",
        deb_ascii_file_info_unix(osBuf,os_p),
        size));
    return(size);
}


/* -------------------------------------------------------------------------- */


/*
 * sys_find_versionfile
 *	Return DNS_SUCCESS if find file, DNS_NOTFND if file doesn't exist.
 *	On error, return DNS_ERROR.
 *
 * Input:
 *	filename_p = pointer to file name
 *
 */
dns_status_t 
sys_find_versionfile (char *filename_p)
{
    DEB_ASCII_BUF_dns_status(statusBuf)
    char	filbuf[ MAX_FILE_SIZE];
    struct stat buf;
    int		u_status;
    dns_status_t status = DNS_SUCCESS;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " >sys_find_versionfile(%s)",
        filename_p));

    (void)sprintf(filbuf, "%s%s", filename_p, version_str);
    u_status = stat( filbuf, &buf);

    if (u_status < 0)
	switch (errno) {
	case ENOENT:				 /* Named file doesn't exist  */
	    status = DNS_NOTFND;
	    break;
	default:				 /* Otherwise, had an error  */
	    status = DNS_ERROR;
	    break;
	}

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " <sys_find_versionfile(%s) return(%s)",
        filename_p,
	deb_ascii_dns_status(statusBuf,status)));
    return(status);
}
