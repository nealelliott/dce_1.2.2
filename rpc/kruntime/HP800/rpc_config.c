/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * @HP_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: rpc_config.c,v $
 * Revision 1.1.81.2  1996/02/18  00:00:08  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:53:29  marty]
 *
 * Revision 1.1.81.1  1995/12/08  00:14:07  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1995/06/20  21:48 UTC  brezak
 * 	Fix for CHFts15430
 * 
 * 	HP revision /main/HPDCE02/brezak_mothra10/1  1995/06/03  02:25 UTC  brezak
 * 	Use dynamic major number assignment for hpux1000p.
 * 
 * 	HP revision /main/HPDCE02/2  1994/10/21  19:08 UTC  mort
 * 	Fix abuse of __hpux1000p directive
 * 
 * 	HP revision /main/HPDCE02/mort_dfsbind800_fix/1  1994/10/21  14:37 UTC  mort
 * 	Fix abuse of __hpux1000p directive
 * 
 * 	HP revision /main/HPDCE02/1  1994/08/03  16:33 UTC  tatsu_s
 * 	Merged mothra up to dce 1.1.
 * 
 * 	HP revision /main/HPDCE01/5  1994/05/26  13:24 UTC  kissel
 * 	Remove io_mgr_entry stuff for HP800 on 10.0 since it is now io-converged.
 * 
 * 	HP revision /main/HPDCE01/4  1994/05/04  13:27 UTC  rmm
 * 	add param.h for NULL define and add a define for GIO_SUCCESS
 * 
 * 	HP revision /main/HPDCE01/3  1994/05/03  15:23 UTC  mort
 * 	include conf.h instead of gio.h and cdio.h as of gsP
 * 
 * 	HP revision /main/HPDCE01/2  1994/05/03  02:14 UTC  mort
 * 	Rodan: use CDIO arch. to install krpc pseudo driver
 * 
 * 	HP revision /main/HPDCE01/mort_10.0_port/1  1994/04/15  18:38 UTC  mort
 * 	Rodan: use CDIO arch. to install krpc pseudo driver
 * 	[1995/12/07  23:55:45  root]
 * 
 * Revision 1.1.4.4  1993/12/21  00:21:42  brezak
 * 	Only mention what devno got assigned for device if it isn't what
 * 	we expected.
 * 	[1993/12/20  23:13:44  brezak]
 * 
 * Revision 1.1.4.3  1993/08/09  12:58:54  robinson
 * 	Change name of XTRA_CONFIG_LOGIC to include "__hp9000s800".
 * 	[1993/08/06  18:59:55  robinson]
 * 
 * 	1. replace constant with external variable:
 * 	       nchrdev = length of cdevsw[] table
 * 	2. Add code to add io_mgr_entry to io_mgr linked
 * 	   list on 800 so that the krpch device will be
 * 	   treated as a legitimate entry in cdevsw[]
 * 	[1993/07/16  16:45:04  robinson]
 * 
 * Revision 1.1.79.2  1994/06/10  20:54:13  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:59:58  devsrc]
 * 
 * Revision 1.1.79.1  1994/01/21  22:31:28  cbrooks
 * 	RPC Code Cleanup
 * 	[1994/01/21  20:33:04  cbrooks]
 * 
 * Revision 1.1.6.2  1993/07/19  19:38:14  zeliff
 * 	HP port of DFS
 * 	[1993/07/19  18:29:35  zeliff]
 * 
 * Revision 1.1.4.2  1993/07/08  09:25:25  root
 * 	Initial King Kong branch
 * 	[1993/07/08  09:24:54  root]
 * 
 * Revision 1.1.2.2  1993/06/04  21:26:03  kissel
 * 	Initial HPUX RP version.
 * 	[1993/06/04  21:24:08  kissel]
 * 
 * Revision 1.1.2.2  1993/05/19  17:47:24  kissel
 * 	This was in rpc/kruntime, but it is HP specific so it really belongs here.
 * 	[1993/05/18  18:25:27  kissel]
 * 
 * Revision 1.1.4.2  1993/04/09  19:41:47  toml
 * 	GAMERA Merge.
 * 	[1993/03/29  19:27:57  toml]
 * 
 * Revision 1.1.2.2  1993/03/29  19:11:44  toml
 * 	Initial revision.
 * 	[1993/03/29  19:03:26  toml]
 * 
 * $EndLog$
 */

#include <sys/param.h>
#include <sys/conf.h>
#include <sys/errno.h>

#include <sys/kload.h>

/* XXX this used to be included in gio.h and is missing from 
   conf.h ... */ 
#ifndef GIO_SUCCESS
#define GIO_SUCCESS 0
#endif

/*
 * XXX This is only used for 9.0; hpux1000p uses dynamic 
 * major device allocation; retrieved via mksf in rc file
 */
#define KRPC_DEV 71

#if !defined(__hp9000s700) && defined(__hp9000s800) && !(__hpux1000p)

#include <machine/../sio/llio.h>

/*
 * THIS CODE IS *NOT* MP SAFE!
 */

#define XTRA_CONFIG_LOGIC__hp9000s800

extern io_mgr_entry *io_mgr_ptr;
extern io_major_link c_link[];
extern io_name_ptr add_io_name();

	/*
	 * fill in our io_mgr_entry in the linked list.
	 * things we can't do 100% correctly (but don't need to):
	 *     - can't extend the iotree_names list (unfortunately).
	 *     - can't extend the mgr_table (due to iotree_names).
	 */
static io_mgr_entry krpc_mgr_entry =
		{0 /* fill in */,   0,			    0,
		0,		    {1,0,0,0,0,0,0,0,0},    NONE,
		KRPC_DEV,	    NULL_IO_TREE_PTR,	    NULL_IO_MGR_PTR};

#endif /* !defined(__hp9000s700) && defined(__hp9000s800) && !(__hpux1000p) */

#include <dce/dce.h>

extern int krpchdd_open(), krpchdd_close(), krpchdd_read(), krpchdd_write(), krpchdd_ioctl(), krpchdd_select();
extern int nodev();
extern int nchrdev;

#if __hpux1000p
/* Declare the CDIO driver-specific fields */
/* Flags DRV_CHAR/DRV_BLOCK/DRV_PSEUDO/DRV_SCAN/DRV_MP_SAFE/DRV_SAFE_CONF */
static drv_info_t	krpchdd_info = {
	"krpc",		/* char *name	for device type (XXX krpch? XXX)*/
	"pseudo",	/* char *name	for device class */
	DRV_PSEUDO|DRV_CHAR,	/* ubit32 flags	pseudo? block? char? scan? */
	-1,		/* int b_major	maj dev# if block type */
	-1,		/* DYNAMIC; int c_major	maj dev# if char type */
	NULL,		/* struct cdio *cdio	drivers set to NULL */
	NULL,		/* void *gio_private	drivers set to NULL */
	NULL		/* void *cdio_private	drivers set to NULL */
};

/* Declare the driver entry points */
static drv_ops_t	krpchdd_ops = {
	krpchdd_open,	/* open <function pointer> */
	krpchdd_close,	/* close */
	NULL,		/* strategy */
	NULL,		/* dump */
	NULL,		/* psize */
	NULL,		/* mount */
	krpchdd_read,	/* read */
	krpchdd_write,	/* write */
	krpchdd_ioctl,	/* ioctl */
	krpchdd_select,	/* select */
	NULL,		/* option_1 */
	NULL,		/* reserved1 */
	NULL,		/* reserved2 */
	NULL,		/* reserved3 */
	NULL,		/* reserved4 [<drv>_link] */
	0		/* flag */	/* Stoft - FIXME - add flag */
};
#endif /* __hpux1000p */


static int krpc_dev = 0;

int krpc_verbose = 0;

extern unsigned char rpc_g_initialized;

/*
 * Configuration routine for dynamic loading of kernel extension.
 */
int dce_krpc_config
#ifdef _DCE_PROTO_
(
    int initop
)
#else
(initop)
int initop;
#endif
{

	struct cdevsw *cp;
	int i;
#ifdef XTRA_CONFIG_LOGIC__hp9000s800
	io_mgr_entry *iomp;
	io_name_ptr ionp;

	/*
	 * complete the initialization of the io_mgr_entry by
	 * adding the io_name of "krpch" and using that
	 * io_name_ptr.  we need only do this once (since it
	 * need not be undone by UNCONFIG).  this must happen
	 * before the search for krpc_dev since add_io_name()
	 * may sleep...
	 */
	if (krpc_mgr_entry.name == 0) {
		ionp = add_io_name("krpch");
		krpc_mgr_entry.name = ionp;
	}

#endif /* XTRA_CONFIG_LOGIC__hp9000s800 */

	if (initop == KLOAD_INIT_CONFIG) {
		if (krpc_dev != 0) {
			uprintf ("rpc_config: 2nd config without unconfig!\n");
			return(-1);
		}

#if __hpux1000p

		if (install_driver(&krpchdd_info,&krpchdd_ops) != GIO_SUCCESS) {
			uprintf ("rpc_config: could not install into switch table at major number %d!\n", KRPC_DEV);
			return(-1);
		} else if (krpc_verbose)
			uprintf ("rpc_config: installed krpc device at major number %d\n", KRPC_DEV);

		krpc_dev = KRPC_DEV;

#else /* not __hpux1000p */

		/*
		 * Scan the cdevsw table for a free entry.
		 * Hope for slot KRPC_DEV (71).
		 */
		for (i=KRPC_DEV; i<nchrdev; i++) {

			if (cdevsw[i].d_open    == &nodev &&
			    cdevsw[i].d_close   == &nodev &&
			    cdevsw[i].d_read    == &nodev &&
			    cdevsw[i].d_write   == &nodev &&
			    cdevsw[i].d_ioctl   == &nodev &&
			    cdevsw[i].d_select  == &nodev &&
			    cdevsw[i].d_option1 == &nodev &&
			    cdevsw[i].d_flags   == 0)

				break;
	
		}

		if (i >= nchrdev) {
			uprintf ("rpc_config: no cdevsw slots available!\n");
			return(-1);
		} else if (i != KRPC_DEV || krpc_verbose)
			uprintf ("rpc_config: installed krpc device at major number %d\n", i);

		/*
		 * Install the krpc device into cdevsw table.
		 */
		cp = &cdevsw[i];
		cp->d_open    = &krpchdd_open;
		cp->d_close   = &krpchdd_close;
		cp->d_read    = &krpchdd_read;
		cp->d_write   = &krpchdd_write;
		cp->d_ioctl   = &krpchdd_ioctl;
		cp->d_select  = &krpchdd_select;

#ifdef XTRA_CONFIG_LOGIC__hp9000s800

		/*
		 * Find the last entry in the io_mgr_ptr linked list.
		 * Then add our new entry so that we can point the
		 * c_link[i] entry at it (which will allow the logic
		 * to call our stuff rather than returning ENXIO).
		 * Then point c_link at it.
		 */
		krpc_mgr_entry.next = NULL_IO_MGR_PTR;
		if (io_mgr_ptr == NULL_IO_MGR_PTR) {
			io_mgr_ptr = &krpc_mgr_entry;
		} else {
			for (iomp = io_mgr_ptr;
			     iomp->next != NULL_IO_MGR_PTR;
			     iomp = iomp->next)
				;
			iomp->next = &krpc_mgr_entry;
		}

		c_link[i].manager = &krpc_mgr_entry;
		c_link[i].class   = NULL_IO_CLASS_PTR;	/* just in case?! */

#endif /* XTRA_CONFIG_LOGIC__hp9000s800 */

		krpc_dev = i;

#endif /* __hpux1000p */

	} else if (initop == KLOAD_INIT_UNCONFIG) {

		if (rpc_g_initialized)
			return(EBUSY);

#if __hpux1000p

		if (uninstall_driver(&krpchdd_info) != GIO_SUCCESS) {
			uprintf ("rpc_config: could not uninstall from switch table at major number %d!\n", KRPC_DEV);
			return(-1);
		} else if (krpc_verbose)
			uprintf ("rpc_config: uninstalled krpc device at major number %d\n", KRPC_DEV);
		    

#else /* not __hpux1000p */

		cp = &cdevsw[krpc_dev];

		cp->d_open    = &nodev;
		cp->d_close   = &nodev;
		cp->d_read    = &nodev;
		cp->d_write   = &nodev;
		cp->d_ioctl   = &nodev;
		cp->d_select  = &nodev;

#ifdef XTRA_CONFIG_LOGIC__hp9000s800

		/*
		 * remove the krpc_mgr_entry from the io_mgr_ptr's list
		 * then fixup the c_link stuff:
		 */
		if (io_mgr_ptr == &krpc_mgr_entry) {
			io_mgr_ptr = krpc_mgr_entry.next;
		} else {
			for (iomp = io_mgr_ptr;
			     (iomp->next != &krpc_mgr_entry) && (iomp->next != NULL_IO_MGR_PTR);
			     iomp = iomp->next)
				;
			if (iomp->next == &krpc_mgr_entry)
				iomp->next = krpc_mgr_entry.next;
			/* else panic("no krpc_mgr_entry in io_mgr list!"); */
		}

		c_link[krpc_dev].manager = NULL_IO_MGR_PTR;
		c_link[krpc_dev].class   = NULL_IO_CLASS_PTR;	/* completeness */

#endif /* XTRA_CONFIG_LOGIC__hp9000s800 */

#endif /* __hpux1000p */

		krpc_dev = 0;

	}

	return (0);

}
