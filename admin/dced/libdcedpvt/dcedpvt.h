/*
 * @OSF_COPYRIGHT@
 * Copyright (c) 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dcedpvt.h,v $
 * Revision 1.1.2.2  1996/02/18  23:32:03  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:19  marty]
 *
 * Revision 1.1.2.1  1995/12/08  20:53:13  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/09/29  22:39 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/tatsu_s.dced_local.b0/2  1995/09/23  00:43 UTC  tatsu_s
 * 	Added dcedpvt_server.h.
 * 
 * 	HP revision /main/tatsu_s.dced_local.b0/1  1995/09/19  15:23 UTC  tatsu_s
 * 	Initial revision.
 * 	[1995/12/08  20:48:53  root]
 * 
 * $EndLog$
 */

#ifndef	_DCED_LIBPVT_H
#define	_DCED_LIBPVT_H

/* ______________________________________________________________________ */

typedef	struct dced_g_info_t {
    uuid_t	this_cell_uuid;		/* UUID of this_cell */
    uuid_t	thishost_prin_uuid;	/* this-host's prin's uuid */
    char	*thishost_self;		/* /.:/hosts/this-host/self */
    char	*this_cell;		/* /.:/my-cell-name */
    char	*thishost_prin;		/* hosts/this-host/self */
    char	*this_dced;		/* /.:/hosts/this-host/config */
    char	*dir_path;		/* /opt/dcelocal/var/dced */
}	dced_g_info_t;

/* ______________________________________________________________________ */

/* for initializing data */

extern dced_g_info_t	dced_g_info;

/* simple function prototypes */
extern	void	dced_init_global_data(boolean32, boolean32, boolean32,
				      boolean32, error_status_t*);
extern	void	dced_read_object_uuid(uuid_t*, error_status_t*);
extern	void	dced_setup_global_uuids(error_status_t*);
extern	void	dced_init_databases(error_status_t*);
extern	void	dced_open_databases(boolean32, error_status_t*);
extern	void	dced_close_databases(error_status_t*);
extern	void	dced_setup_acls(error_status_t*);
extern	void	dced_register_acls(error_status_t*);
extern	void	dced_unregister_acls(error_status_t*);
extern	void	dced_init_name_caches(error_status_t*);
extern	void	dced_reload_name_caches(error_status_t*);
extern	void	dced_hd_read_post_processors(error_status_t *);

#endif	/* _DCED_LIBPVT_H */
