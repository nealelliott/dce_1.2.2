/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1993, 1994, 1996 Open Software Foundation, Inc.
 * Permission is hereby granted to use, copy, modify and freely distribute  
 * the software in this file and its documentation for any purpose without  
 * fee, provided that the above copyright notice appears in all copies and  
 * that both the copyright notice and this permission notice appear in  
 * supporting documentation.  Further, provided that the name of Open  
 * Software Foundation, Inc. ("OSF") not be used in advertising or  
 * publicity pertaining to distribution of the software without prior  
 * written permission from OSF.  OSF makes no representations about the  
 * suitability of this software for any purpose.  It is provided "as is"  
 * without express or implied warranty.
 * 
 * Routines for reading serviceability logs.
 */

/*
 * HISTORY
 * $Log: svclog.c,v $
 * Revision 1.1.6.2  1996/02/18  19:22:35  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:07:24  marty]
 *
 * Revision 1.1.6.1  1995/12/08  21:38:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:10:06  root]
 * 
 * Revision 1.1.4.17  1994/09/19  20:24:46  bowe
 * 	Do not set "prolog->format" in dce_svc_log_get() if dce_msg_get_msg(),
 * 	since dce_msg_get_msg() always gives something usable. [CR 12157]
 * 	[1994/09/19  20:21:31  bowe]
 * 
 * Revision 1.1.4.16  1994/09/13  18:06:17  rrizzo
 * 	Merged with changes from 1.1.4.15
 * 	[1994/09/13  18:06:05  rrizzo]
 * 
 * 	Added missing first line to copyright text.
 * 	[1994/09/13  17:56:46  rrizzo]
 * 
 * Revision 1.1.4.15  1994/09/13  15:27:22  bowe
 * 	Fix botched if test (was = instead of ==) [CR 12154]
 * 	[1994/09/13  15:27:06  bowe]
 * 
 * Revision 1.1.4.14  1994/09/06  21:27:36  bowe
 * 	Remove unused functions: dce_svc_log_get_arg(), et al. [CR 12049]
 * 	[1994/09/06  21:08:53  bowe]
 * 
 * Revision 1.1.4.13  1994/09/02  20:43:03  bowe
 * 	Check for null string before trying dce_strdup(). [CR 12037]
 * 	[1994/09/02  20:37:42  bowe]
 * 
 * Revision 1.1.4.12  1994/08/31  14:24:18  bowe
 * 	Merged with changes from 1.1.4.11
 * 	[1994/08/31  14:24:11  bowe]
 * 
 * 	Check return stat of dce_svc_bin_data_decode() before freeing.
 * 	Some better error checking throughout.  Misc cleanup. [CR 11860]
 * 	[1994/08/31  14:15:31  bowe]
 * 
 * Revision 1.1.4.11  1994/08/30  20:41:59  marty
 * 	CR 11923 - Change copyright to a public domain copyright.
 * 	[1994/08/30  20:34:32  marty]
 * 
 * Revision 1.1.4.10  1994/07/11  19:12:54  bowe
 * 	In dce_svc_log_free_entry(), free arg pointer array after loop,
 * 	not in it.  [CR 11156]
 * 	[1994/07/11  19:12:37  bowe]
 * 
 * Revision 1.1.4.9  1994/07/08  20:52:04  bowe
 * 	In dce_svc_log_free_entry() free some parts of prolog with free(),
 * 	some with rpc_sm_client_free().  [CR 11156]
 * 	[1994/07/08  20:51:38  bowe]
 * 
 * Revision 1.1.4.8  1994/07/08  15:00:42  rsalz
 * 	Add cast to message_text assignment [CR 11055].
 * 	[1994/07/08  14:25:45  rsalz]
 * 
 * Revision 1.1.4.7  1994/06/29  21:34:45  bowe
 * 	In dce_svc_log_get(), set h->prolog.message_text [CR 11055]
 * 	[1994/06/29  21:34:21  bowe]
 * 
 * Revision 1.1.4.6  1994/06/09  16:06:29  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:50:59  devsrc]
 * 
 * Revision 1.1.4.5  1994/05/27  17:44:52  bowe
 * 	Added #include <dce/idl_es.h> [CR 10483,10478]
 * 	[1994/05/27  16:43:07  bowe]
 * 
 * Revision 1.1.4.4  1994/05/26  18:51:51  bowe
 * 	Use IDL encoding services to extract binary log data.  Affected
 * 	dce_svc_log_get() and dce_svc_log_get_arg().  Added new call,
 * 	dce_svc_log_free_entry(). [CR 10483,10478]
 * 	[1994/05/26  18:27:20  bowe]
 * 
 * Revision 1.1.4.3  1994/05/05  15:33:55  rsalz
 * 	Calculate size right for string parameters (OT CR 10542).
 * 	[1994/05/05  15:33:40  rsalz]
 * 
 * Revision 1.1.4.2  1994/04/27  18:55:22  rsalz
 * 	Remove premature feof test (OT CR 10467).
 * 	[1994/04/27  18:16:44  rsalz]
 * 
 * Revision 1.1.4.1  1994/04/26  19:21:32  rsalz
 * 	Move some field from prolog into log handle (OT CR 10461)
 * 	[1994/04/26  17:02:28  rsalz]
 * 
 * Revision 1.1.2.4  1993/10/26  17:21:52  rsalz
 * 	Fix so that binary data can be read.
 * 	[1993/10/20  20:45:39  rsalz]
 * 
 * Revision 1.1.2.3  1993/09/14  18:57:12  rsalz
 * 	Fix macro call to read filename.
 * 	[1993/09/14  18:55:24  rsalz]
 * 
 * Revision 1.1.2.2  1993/08/16  18:10:02  rsalz
 * 	Initial release
 * 	[1993/08/16  18:04:42  rsalz]
 * 
 * $EndLog$
 */

#include <svcimpl.h>
#include <dce/svclog.h>
#include <dce/idl_es.h>

/* For incremental decoding with encoding services */
typedef struct log_decode_state_s_t {
    idl_byte	*buf;
    FILE	*fp;
} log_decode_state_t;

/*
**  Open a log, fill in the handle.
*/
void
dce_svc_log_open(
    char			*name,
    dce_svc_log_handle_t	*hp,	
    error_status_t		*status
)
{
    dce_svc_log_handle_t	h;
    FILE			*fp;
    static struct dce_svc_log_handle_s_t	nil_h;

    *hp = NULL;
    if ((fp = fopen(name, "r")) == NULL) {
	*status = svc_s_cantopen;
	return;
    }
    if ((h = (dce_svc_log_handle_t)malloc(sizeof *h)) == NULL) {
	*status = svc_s_no_memory;
	return;
    }

    *h = nil_h;
    h->input = fp;
    *hp = h;
    *status = svc_s_ok;
}

static void
svc__log_read(
    idl_void_p_t	param,
    idl_byte		**buf,
    idl_ulong_int	*size
)
{
    log_decode_state_t	*state;
    state = (log_decode_state_t *)param;
    *size = fread(state->buf, 1, SVC_BLOCK_SIZE, state->fp);
    *buf = state->buf;
}

void
dce_svc_log_get(
    dce_svc_log_handle_t	h,	
    dce_svc_log_prolog_t	*prolog,
    error_status_t		*status
)
{
    log_decode_state_t		state;
    idl_es_handle_t		enc_h;
    svc_bin_data_t		bin_data;
    unsigned32			i;
    idl_byte			enc_buffer[SVC_BLOCK_SIZE+16];
    error_status_t		status2;

    /* Be pessimistic. */
    *status = svc_s_at_end;

    if (fseek(h->input, h->next_record, SEEK_SET) < 0 || ferror(h->input))
	return;

    state.fp	= h->input;
    state.buf	= (idl_byte *)(((enc_buffer - (idl_byte *)0) + 7) & ~7);

    idl_es_decode_incremental((idl_void_p_t)&state, svc__log_read,
	&enc_h, status);
    if (*status == error_status_ok) {
	dce_svc_bin_data_decode(enc_h, &bin_data, status);
	if (*status == error_status_ok)
	    idl_es_handle_free(&enc_h, &status2);
	else {
	    if (feof(h->input))
		*status = svc_s_at_end;
	    return;
	}
    }
    if (feof(h->input) || ferror(h->input)) {
	*status = svc_s_at_end;
	return;
    }
    h->next_record = ftell(h->input);
    if (*status != error_status_ok) {
	dce_printf(*status);
	return;
    }

    /* free previously gotten entry, if any */
    dce_svc_log_free_entry(h, status);

    h->prolog.version		= bin_data.version;
    h->prolog.t			= bin_data.t;
    h->prolog.attributes	= bin_data.attributes;
    h->prolog.message_index	= bin_data.message_index;
    h->prolog.line		= bin_data.line;
    /* You can't get thread id from the code... */
/*    h->prolog.thread_id		= bin_data.thread_code;*/  /* XXX */
    h->prolog.thread_code	= bin_data.thread_code;
    h->prolog.argtypes		= (char *)bin_data.argtypes;
    h->prolog.fac_name		= (char *)bin_data.fac_name;
    h->prolog.file		= (char *)bin_data.file;
    h->prolog.progname		= (char *)bin_data.progname;
    /* try the catalog or in-core table first.  If not there, use string
     * in the encoded bin_data. */
    h->prolog.message_text	= (char *)
		    dce_msg_get_msg(h->prolog.message_index, status);
    h->prolog.n_args		= bin_data.count;
    if (bin_data.count) {
	h->prolog.args		= malloc(sizeof(svc_bin_arg_t)*bin_data.count);
	for (i=0; i<bin_data.count; i++)
	    h->prolog.args[i] = bin_data.args[i];
    }
    else
	h->prolog.args		= NULL;

    *status = svc_s_ok;
    *prolog = &h->prolog;
}


void
dce_svc_log_rewind(
    dce_svc_log_handle_t	h,	
    error_status_t		*status
)
{
    rewind(h->input);
    h->next_record = 0;
    *status = svc_s_ok;
}


void
dce_svc_log_close(
    dce_svc_log_handle_t	h,	
    error_status_t		*status
)
{
    (void)fclose(h->input);
    dce_svc_log_free_entry(h, status);
    free(h);
    *status = svc_s_ok;
}

/*
 * Free storage used by a log entry.
 * Be careful - some were alloc'ed by the encoding services, some by malloc().
 */
void
dce_svc_log_free_entry(
    dce_svc_log_handle_t	h,	
    error_status_t		*status
)
{
    int		i;

    *status = svc_s_ok;
    if (h == NULL) return;

    if (h->prolog.args) {
	for (i=0; i<h->prolog.n_args; i++) {
	    if (h->prolog.args[i].bin_encoding == svc_bin_type_string &&
		    h->prolog.args[i].tagged_union.s)
	    rpc_sm_client_free(h->prolog.args[i].tagged_union.s, status);
	}
	free(h->prolog.args);
    }
    if (h->prolog.argtypes != NULL)
	rpc_sm_client_free(h->prolog.argtypes, status);
    if (h->prolog.fac_name != NULL)
	rpc_sm_client_free(h->prolog.fac_name, status);
    if (h->prolog.file != NULL)
	rpc_sm_client_free(h->prolog.file, status);
    if (h->prolog.message_text != NULL)
	free(h->prolog.message_text);
    if (h->prolog.progname != NULL)
	rpc_sm_client_free(h->prolog.progname, status);

    h->prolog.argtypes = h->prolog.fac_name = h->prolog.file = 
	h->prolog.message_text = h->prolog.progname = NULL;
    h->prolog.args = NULL;
}

