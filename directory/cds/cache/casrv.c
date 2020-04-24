/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: casrv.c,v $
 * Revision 1.1.4.2  1996/02/18  19:24:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:08:19  marty]
 *
 * Revision 1.1.4.1  1995/12/08  00:10:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:53:42  root]
 * 
 * Revision 1.1.2.1  1994/08/11  20:35:11  peckham
 * 	We need to remove the clearinghouse from the wan file
 * 	when the cached server is cleared.
 * 	[1994/08/11  18:44:25  peckham]
 * 
 * 	CR#9765 - cds client better disaster recovery.
 * 	Maintain cached server file. This is a file which contains
 * 	all the cached server information, so if the cache file
 * 	is munged, an isolated clerk does not get left in the cold.
 * 	[1994/08/11  04:47:54  peckham]
 * 
 * $EndLog$
 */
/*
 * Module: casrv.c
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
 */
#include <cache.h>
#include <dce/assert.h>
#include <dns_config.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <dns_cfg.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <dns_strings.h>
#include <tower.h>

/*
 * Cache server file maintainence
 */

#define MAX_TOWERS	63			/* max num of towers */
#define MAX_LINE	4096			/* max line length in file */

typedef struct {
    int		sf_in;				/* input file */
    int		sf_out;				/* updated file */
    int		sf_pos;				/* current pos in file */
    short	sf_rw;				/* if writing new file */
    short	sf_len;				/* length of line read */
    union {
      struct {
	char f_file1[_DNS_MAXPATHLEN+1];	/* original file name */
#define sf_file1 b.f.f_file1
	char f_file2[_DNS_MAXPATHLEN+1];	/* updated file name */
#define sf_file2 b.f.f_file2
      } f;
      char b_buf[MAX_LINE];			/* line buffer */
#define sf_buf b.b_buf
    } b;
} server_file;

const static char srvcle[] = "clearinghouse:";
const static char srvcnt[] = "tower_count:";
const static char srvtwr[] = "tower:";

/*
 * Convert binary towers to ascii
 */
static void
CA_towers_to_towers (const byte_t *const in_p, char *out_p)
{
  const int num_twrs = NUM_Set(in_p);
  int i;

  *out_p++ = num_twrs;

  for (i = 0; i < num_twrs; i++) {
    const MemberValue_t *const mv_p = (MemberValue_t *)INDEX_Set(in_p, i);

    /* Use internal conversion routine */
    out_p += get_binding_string((byte_t *)mv_p->mv_value, out_p);
  }
}

/*
 * CA_SrvReadClose - close the input file
 */
static void
CA_SrvReadClose (server_file *const sf_p)
{
  (void) close(sf_p->sf_in);
  sf_p->sf_in = -1;
}

/*
 * CA_SrvRead - read input line at offset in file and
 * if line begins with given tag, return pointer to argument.
 */
static char *
CA_SrvRead (server_file *const sf_p, const char *const tag_p)
{
  if (0 <= sf_p->sf_in) {
    const int tag_len = strlen(tag_p);
    const int read_len = sizeof(sf_p->sf_buf) - 1;
    int stat;

    /* find our place in the file */

    if ((stat = lseek(sf_p->sf_in, sf_p->sf_pos, SEEK_SET)) < 0) {
      const int save_errno = errno;

      /* seek error */

      DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_cache,
	svc_c_debug3,
	"E CA_SrvRead(%p,%s) %d=lseek(%d,%u,) errno(%d)",
	sf_p, tag_p, stat, sf_p->sf_in, (unsigned)sf_p->sf_pos, save_errno));

    } else

    /* read in a mouthful */

    if ((stat = read(sf_p->sf_in, sf_p->sf_buf, read_len)) < 0) {
      const int save_errno = errno;

      /* read error */

      DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_cache,
	svc_c_debug3,
	"E CA_SrvRead(%p,%s) %d=read(%d,,%d) errno(%d)",
	sf_p, tag_p, stat, sf_p->sf_in, read_len, save_errno));

    } else

    /* and if the read was good */

    if (0 < stat) {
      /* check for a newline */
      char *const p = strchr(sf_p->sf_buf, '\n');

      if (p) {

	/* line terminates here */

	stat = (p - sf_p->sf_buf);
      } else if (read_len <= stat) {

	/* line too long for buffer - we didn't generate this */

	DCE_SVC_DEBUG((
	  cds__svc_handle,
	  cds_svc_cache,
	  svc_c_debug3,
	  "F CA_SrvRead(%p,%s) %d=read(%d,,%d) line too long",
	  sf_p, tag_p, stat, sf_p->sf_in, read_len));

	stat = 0;
      }

      /* NULL at end of line (cover newline) */
      sf_p->sf_buf[stat] = '\0';

      /* if we have something, check if it is the right parameter */
      if ((0 < (sf_p->sf_len = stat))
	  && (strncmp(tag_p, sf_p->sf_buf, tag_len) == 0)) {
	char *const arg = &sf_p->sf_buf[tag_len];

	/* return pointer to argument */
	return &arg[strspn(arg, " \t")];
      }
    }

    /* bad file contents */
    CA_SrvReadClose(sf_p);
  }

  return(NULL);
}

/*
 * CA_SrvWrite - an argument value has been placed in the buffer -
 * put the identifier in place, append a newline, and write it out.
 */
static int
CA_SrvWrite (server_file *const sf_p, const char *const tag_p)
{
  char *next_p = sf_p->sf_buf;
  int left = strlen(tag_p);

  /* copy in the tag */
  strncpy(next_p, tag_p, left);

  /* place a space after it, before the value */
  next_p[left] = ' ';

  /* insert a newline at the end */
  left = strlen(next_p);
  next_p[left++] = '\n';

  /* write the line out */
  do {
    int cc = write(sf_p->sf_out, next_p, left);

    if (cc <= 0) {
      const int save_errno = errno;

	DCE_SVC_DEBUG((
	  cds__svc_handle,
	  cds_svc_cache,
	  svc_c_debug3,
	  "E CA_SrvWrite(%p,%s) %d=write(%d,%p,%d) errno(%d)",
	  sf_p, tag_p, cc, sf_p->sf_out, next_p, left, save_errno));

      return(FALSE);
    }

    next_p += cc;
    left -= cc;
  } while (left);

  return(TRUE);
}

/*
 * CASysServersOpen - open files for servers update
 */
void *
CASysServersOpen (int rw)
{
  server_file *const sf_p = (server_file *)malloc(sizeof(server_file));

  DCE_SVC_DEBUG((
    cds__svc_handle,
    cds_svc_cache,
    svc_c_debug3,
    " >CASysServersOpen(%d)", rw));

  if (sf_p) {
    memset(sf_p, 0, sizeof(*sf_p));
    sf_p->sf_rw = rw;

    /* read the current file, if one exists */
    (void)sprintf(sf_p->sf_file1, CDS_CACHE_WAN, dcelocal_path);
    sf_p->sf_in = open(sf_p->sf_file1, O_RDONLY, 0);

    if (rw) {
      /* open up the new file */
      strcpy(sf_p->sf_file2, sf_p->sf_file1);
      strcat(sf_p->sf_file2, ".new");
      sf_p->sf_out = open(sf_p->sf_file2, O_WRONLY | O_CREAT | O_TRUNC, 0644);

      if (sf_p->sf_out < 0) {
	if (0 <= sf_p->sf_in) close(sf_p->sf_in);
	free(sf_p);

	DCE_SVC_DEBUG((
	  cds__svc_handle,
	  cds_svc_cache,
	  svc_c_debug3,
	  " <CASysServersOpen(%d) return(%p)", rw, NULL));

	return(NULL);
      }

      (void) fchmod(sf_p->sf_out, 0600);
    } else
      sf_p->sf_out = -1;
  }

  DCE_SVC_DEBUG((
    cds__svc_handle,
    cds_svc_cache,
    svc_c_debug3,
    " <CASysServersOpen(%d) return(%p)", rw, sf_p));

  return(sf_p);
}

/*
 * CASysServersRead - read a clearinghouse and towers from the file
 */
int
CASysServersRead (void *handle, char *name_p, char *towers_p)
{
  DCE_SVC_DEBUG((
    cds__svc_handle,
    cds_svc_cache,
    svc_c_debug3,
    " >CASysServersRead(%p,%p,%p)", handle, name_p, towers_p));

  if (handle) {
    server_file *const sf_p = handle;
    char *p;
    int num_twrs;

    /*
     * Parse out the clearinghouse name
     */
    if ((p = CA_SrvRead(sf_p, srvcle)) == NULL)
      goto fail_CASysServersRead;

    strcpy(name_p, p);

    /* done with that record */
    sf_p->sf_pos += sf_p->sf_len + 1;

    /*
     * Now get a count of the towers
     */
    if ((p = CA_SrvRead(sf_p, srvcnt)) == NULL)
      goto fail_CASysServersRead;

    /* we need at least one tower */
    if ((num_twrs = atoi(p)) == 0) {

      DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_cache,
	svc_c_debug3,
	"E CASysServersRead(%p,%p,%p) name(%s) no towers!!",
	handle, name_p, towers_p, name_p));

cancel_CASysServersRead:

      CA_SrvReadClose(sf_p);

fail_CASysServersRead:

      DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_cache,
	svc_c_debug3,
	" <CASysServersRead(%p,%p,%p) return(FALSE)",
	handle, name_p, towers_p));

      return(FALSE);
    }

    *towers_p++ = num_twrs;

    /* done with that record */
    sf_p->sf_pos += sf_p->sf_len + 1;

    do {
      if ((p = CA_SrvRead(sf_p, srvtwr)) == NULL)
	goto cancel_CASysServersRead;

      do
	*towers_p++ = *p;
      while (*p++);

      sf_p->sf_pos += sf_p->sf_len + 1; /* done with that record */
    } while (--num_twrs);
  }

  DCE_SVC_DEBUG((
    cds__svc_handle,
    cds_svc_cache,
    svc_c_debug3,
    " <CASysServersRead(%p,%p,%p) return(TRUE)", handle, name_p, towers_p));

  return(TRUE);
}

/*
 * CASysServersWrite - write a clearinghouse and towers to the file
 */
void
CASysServersWrite (void *handle, char *name_p, char *towers_p)
{
  DCE_SVC_DEBUG((
    cds__svc_handle,
    cds_svc_cache,
    svc_c_debug3,
    " >CASysServersWrite(%p,%p,%p)", handle, name_p, towers_p));

  if (handle) {
    server_file *const sf_p = handle;

    /* write the clearinghouse tag */
    strcpy(&sf_p->sf_buf[sizeof(srvcle)], name_p);

    if (!CA_SrvWrite(sf_p, srvcle))
      goto leave_CASysServersWrite;

    {
      char *const buf_p = &sf_p->sf_buf[sizeof(srvtwr)];
      const int buflen = sizeof(sf_p->sf_buf) - sizeof(srvtwr);
      const int num_twrs = *towers_p++;
      int i;

      /* write number of towers to the file */

      sprintf(&sf_p->sf_buf[sizeof(srvcnt)], "%d", num_twrs);
      if (!CA_SrvWrite(sf_p, srvcnt))
	goto leave_CASysServersWrite;

      /*
       * Convert the towers
       */
      for (i = 0; i < num_twrs; i++) {
	strcpy(buf_p, towers_p);

	/* write tower to the file */
	if (!CA_SrvWrite(sf_p, srvtwr))
	  goto leave_CASysServersWrite;

	towers_p = strchr(towers_p, '\0') + 1;
      }
    }
  }

leave_CASysServersWrite:

  DCE_SVC_DEBUG((
    cds__svc_handle,
    cds_svc_cache,
    svc_c_debug3,
    " <CASysServersWrite(%p,%p,%p)", handle, name_p, towers_p));
}

/*
 * CASysServersClose - close the files and make
 * the new file permannent
 */
void
CASysServersClose (void *handle)
{
  DCE_SVC_DEBUG((
    cds__svc_handle,
    cds_svc_cache,
    svc_c_debug3,
    " >CASysServersClose(%p)", handle));

  if (handle) {
    server_file *const sf_p = handle;

    if (sf_p->sf_rw && (0 <= sf_p->sf_out)) {
      close(sf_p->sf_out);

      /* reconstruct wan file name */
      (void)sprintf(sf_p->sf_file1, CDS_CACHE_WAN, dcelocal_path);

      /* and this is the file we just created */
      strcpy(sf_p->sf_file2, sf_p->sf_file1);
      strcat(sf_p->sf_file2, ".new");

      /* make our new file the wan file */
      rename(sf_p->sf_file2, sf_p->sf_file1);
      /* HANDLE FAILURE */
    }

    if (0 <= sf_p->sf_in) close(sf_p->sf_in);
    free(sf_p);
  }

  DCE_SVC_DEBUG((
    cds__svc_handle,
    cds_svc_cache,
    svc_c_debug3,
    " <CASysServersClose(%p)", handle));
}

/*
 * Write the cached servers to a file
 *
 * Assume Global is locked.
 */
void
CA_write_servers (void)
{
  void *handle;

  DCE_SVC_DEBUG((
    cds__svc_handle,
    cds_svc_cache,
    svc_c_debug3,
    " >CA_write_servers()"));

  if ((handle = CASysServersOpen(1)) != NULL) {
    struct {
      byte_t *towers_p;			/* clearinghouse towers */
      char name[CDS_STR_SNAME_MAX+1];	/* clearinghouse tag */
    } tags[MAX_TOWERS+1], *tag_p = tags; /* enumerate caches servers */
    char towers[MAXATTRIBUTE];
    char name[CDS_STR_SNAME_MAX+1];
    CleEntry_t *next_p = NULL;
    int cle_handle = 0;

    /*
     * First, find all the cached servers in the cache.
     */
    while ((next_p = CANextCle(next_p, &cle_handle)) != NULL) {
      int name_len = sizeof(tag_p->name);
      dns_status_t status;

      tag_p->towers_p = SKIP_FullName(next_p->ch_name);
      status = cdsCvtSimpleToStr((cds_simple_name_t *)SKIP_Set(tag_p->towers_p),
				 (unsigned char *)tag_p->name, &name_len);

      if (status != CDS_SUCCESS) {

      } else if (tag_p->name[0])
	if (tag_p < &tags[MAX_TOWERS]) {

	  DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_cache,
	    svc_c_debug3,
	    "  CA_write_servers() found(%s) in cache", tag_p->name));

	  tag_p++;
	} else
	  /* TOO MANY TAGS */
	  break;
    }

    /* mark the end of the list */
    tag_p->name[0] = '\0';

    /*
     * Now go through the file, correcting each entry
     * that is in the cache.
     */
    while (CASysServersRead(handle, name, towers))
      for (tag_p = tags; TRUE; tag_p++) {

	if (!tag_p->name[0]) {

	  DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_cache,
	    svc_c_debug3,
	    "  CA_write_servers() file entry(%s) not in cache", name));

	  CASysServersWrite(handle, name, towers);
	  break;
	}

	if (strcmp(tag_p->name, name) != 0)
	  continue;

	/* the towers are zapped if a match was found */
	if (tag_p->towers_p) {

	  DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_cache,
	    svc_c_debug3,
	    "  CA_write_servers() file entry(%s) in cache", name));

	  CA_towers_to_towers (tag_p->towers_p, towers);
	  CASysServersWrite(handle, tag_p->name, towers);
	  tag_p->towers_p = NULL;
	}

	break;
      }

    /*
     * Any unmatched entries are appended to the file.
     */
    for (tag_p = tags; tag_p->name[0]; tag_p++)
      if (tag_p->towers_p) {

	DCE_SVC_DEBUG((
	  cds__svc_handle,
	  cds_svc_cache,
	  svc_c_debug3,
	  "  CA_write_servers() cache entry(%s) not in file", tag_p->name));

	CA_towers_to_towers (tag_p->towers_p, towers);
	CASysServersWrite(handle, tag_p->name, towers);
      }

    CASysServersClose(handle);
  }

    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug3,
      " <CA_write_servers()"));
}

/*
 * Delete a cached server from the file
 *
 * Assume Global is locked.
 */
void
CA_delete_server (const char *const name_p)
{
  void *handle;

  DCE_SVC_DEBUG((
    cds__svc_handle,
    cds_svc_cache,
    svc_c_debug3,
    " >CA_delete_server(%s)"));

  if ((handle = CASysServersOpen(1)) != NULL) {
    char towers[MAXATTRIBUTE];
    char name[CDS_STR_SNAME_MAX+1];

    /*
     * Go through the file and delete the specified entry
     */
    while (CASysServersRead(handle, name, towers))
      if (strcmp(name_p, name) != 0)
	CASysServersWrite(handle, name, towers);

    CASysServersClose(handle);
  }

    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug3,
      " <CA_delete_server(%s)"));
}
