/*
 * @OSF_COPYRIGHT@
 * Copyright (c) 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: hdmisc.c,v $
 * Revision 1.1.4.1  1996/08/09  11:53:53  arvind
 * 	Remove memory leak by doing rpc_ss_{enable,disable}_allocate()
 * 	around the dce_db_fetch_by_uuid() call.
 * 	[1996/07/09  19:40 UTC  jrr  /main/HPDCE02/jrr_patch_1/1]
 *
 * Revision 1.1.2.2  1996/02/18  19:16:11  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:02:08  marty]
 * 
 * Revision 1.1.2.1  1995/12/08  20:53:19  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/09/29  22:37 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/tatsu_s.dced_local.b0/1  1995/09/19  15:30 UTC  tatsu_s
 * 	Initial revision: dced splitting (libdcedpvt).
 * 	[1995/12/08  20:48:56  root]
 * 
 * Revision 1.1.2.10  1994/08/28  00:58:38  rsalz
 * 	hd_post_proc passes in the item name through dced_fork (OT CR 11895).
 * 	[1994/08/26  19:14:14  rsalz]
 * 
 * Revision 1.1.2.9  1994/08/25  16:32:18  rsalz
 * 	Add dced_waitpid and fix hostdata postprocessor reaping.
 * 	[1994/08/25  16:30:08  rsalz]
 * 
 * Revision 1.1.2.8  1994/07/29  19:56:07  rsalz
 * 	Use dce_strdup not strdup (OT CR 11343).
 * 	[1994/07/29  19:37:03  rsalz]
 * 
 * Revision 1.1.2.7  1994/05/26  19:42:11  rsalz
 * 	EP functionality.
 * 	[1994/05/26  19:35:41  rsalz]
 * 
 * 	Checkpointing.
 * 	[1994/05/18  19:32:57  rsalz]
 * 
 * Revision 1.1.2.6  1994/05/10  20:15:10  rsalz
 * 	Cleanup and more functionality
 * 	[1994/05/10  20:10:39  rsalz]
 * 
 * Revision 1.1.2.5  1994/05/05  20:07:33  rsalz
 * 	Initial server functionality drop.
 * 	[1994/05/05  20:00:58  rsalz]
 * 
 * Revision 1.1.2.4  1994/04/28  18:38:44  rsalz
 * 	Clean up some compiler warnings; remove dead code.
 * 	[1994/04/28  18:37:52  rsalz]
 * 
 * Revision 1.1.2.3  1994/04/27  22:08:09  bowe
 * 	Implement init_hostdata()
 * 	[1994/04/27  22:01:05  bowe]
 * 
 * Revision 1.1.2.2  1994/04/11  20:22:05  rsalz
 * 	Final set of RFC revision fixes.
 * 	[1994/04/11  20:08:24  rsalz]
 * 
 * Revision 1.1.2.1  1994/03/09  14:25:50  bowe
 * 	Initial checkin
 * 	[1994/03/09  14:24:20  bowe]
 * 
 * $EndLog$
 */

#include <libimpl.h>
#include <hdimpl.h>
#include <ctype.h>
#include <sys/wait.h>

typedef struct hd_postproctable_s_t {
    int			count;
    int			size;
    hd_postproc_t	*list;
} hd_postproctable_t;

static hd_postproctable_t	postprocs;

/* ______________________________________________________________________ */

void
dced_hd_read_post_processors(
    error_status_t	*st
)
{
    dced_entry_data_t	id_d;
    int			i;
    FILE		*F;
    char		buff[500];
    char		*p;

    *st = error_status_ok;

    /* first, free up the old one, if we've read it before */
    if (postprocs.list) {
	for (i = 0; i < postprocs.count; i++)
	    free(postprocs.list[i].cmd);
	free(postprocs.list);
	postprocs.list = NULL;
	postprocs.count = postprocs.size = 0;
    }

    rpc_ss_enable_allocate();

    dce_db_fetch_by_uuid(hd_db, &dced_g_uuid_hostdata_post_proc, &id_d, st);
    if (*st == db_s_key_not_found) {
	*st = dced_s_no_postprocessors;
        rpc_ss_disable_allocate();
	return;
    }
    CHECK_STAT_RET(*st);

    if ((F = fopen((char *)id_d.data.storage_tag, "r")) == NULL) {
	if (errno != ENOENT)
	    *st = dced_s_postprocessor_file_fail;
        rpc_ss_disable_allocate();
	return;
    }

    rpc_ss_disable_allocate();

    /* Allocate space for an initial array */
    postprocs.size = 20;
    postprocs.list = malloc(postprocs.size * sizeof *postprocs.list);

    while (fgets(buff, sizeof buff, F)) {
	if (buff[0] == '#') continue;		/* skip comment */
	if (buff[0] == '\n') continue;		/* skip blank lines */
	if ((p = strchr(buff, '\n'))) *p = '\0';	/* eat newline */
	p = buff;
	while (!isspace(*p)) p++;
	*p++ = '\0';
	while (isspace(*p)) p++;
	/* buf now points to the uuid, p to the command */

	/* be sure there's space in array */
	if (postprocs.count + 1 >= postprocs.size) {
	    postprocs.size += 10;
	    postprocs.list = realloc(postprocs.list,
		postprocs.size * sizeof *postprocs.list);
	}

	uuid_from_string((unsigned char *)buff,
	    &postprocs.list[postprocs.count].uuid, st);
	postprocs.list[postprocs.count++].cmd = dce_strdup(p);
    }
    fclose(F);
}

void
hd_post_process(
    dced_entry_t	*ep,
    error_status_t	*st
)
{
    int			i;
    pid_t		p;
    char		*argv[3];

    *st = error_status_ok;
    for (i = 0; i < postprocs.count; i++) {
	if (uuid_equal(&postprocs.list[i].uuid, &ep->id, st)) {
	    argv[0] = postprocs.list[i].cmd;
	    argv[1] = (char *)ep->name;
	    argv[2] = NULL;
	    if (dced_is_server) {
		dced_fork(argv, &p, st);
		if (*st == error_status_ok) {
		    dced_waitpid(p, &i, st);
		    if (WIFEXITED(i) && WEXITSTATUS(i) == 0)
			dce_svc_printf(DCED_S_POSTPROCESSOR_OK_MSG, ep->name);
		    else
			dce_svc_printf(DCED_S_POSTPROCESSOR_BAD_MSG,
				       ep->name, WEXITSTATUS(i));
		}
	    }
	    else {
		dced_invoke(argv, &i, st);
		if (*st == error_status_ok) {
		    if (WIFEXITED(i) && WEXITSTATUS(i) == 0)
			dce_svc_printf(DCED_S_POSTPROCESSOR_OK_MSG, ep->name);
		    else
			dce_svc_printf(DCED_S_POSTPROCESSOR_BAD_MSG,
				       ep->name, WEXITSTATUS(i));
		}
	    }
	    break;
	}
    }
}
