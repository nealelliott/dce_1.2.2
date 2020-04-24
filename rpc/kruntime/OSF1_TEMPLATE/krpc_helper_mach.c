/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: krpc_helper_mach.c,v $
 * Revision 1.1.37.2  1996/02/18  00:00:19  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:53:35  marty]
 *
 * Revision 1.1.37.1  1995/12/08  00:14:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:55:56  root]
 * 
 * Revision 1.1.35.1  1994/01/21  22:31:42  cbrooks
 * 	RPC Code Cleanup
 * 	[1994/01/21  20:41:21  cbrooks]
 * 
 * Revision 1.1.2.5  1993/01/03  22:35:28  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:51:05  bbelch]
 * 
 * Revision 1.1.2.4  1992/12/23  19:37:28  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  14:49:26  zeliff]
 * 
 * Revision 1.1.2.3  1992/12/10  21:28:06  bolinger
 * 	As part of DFS upgrade to OSF/1 1.1.1, let krpchdd_select()
 * 	dereference its revents pointer only if the pointer is non-null --
 * 	this kernel can call the routine with a null pointer.  [OT defect
 * 	5236]
 * 	[1992/12/10  21:25:12  bolinger]
 * 
 * Revision 1.1.2.2  1992/12/03  22:34:58  delgado
 * 	New interface for kprc helper
 * 	[1992/12/03  22:34:22  delgado]
 * 
 * $EndLog$
 */

#include <commonp.h>
#include <com.h>

#include <sys/select.h>
#include <krpc_helper.h>
#include <krpc_helper_data.h>

extern struct krpc_helper *pending;

/*
 * Platform dependent routines for the krpc_helper module
 * live here
 */



krpchdd_open(dev)
dev_t dev;
{
    return(krpch_open());
}


krpchdd_close(dev)
dev_t dev;
{
    krpch_close();
    return (0);
}



krpchdd_ioctl(dev, cmd, data, flag)
dev_t   dev;
int     cmd;
caddr_t data;
int	flag;
{
    return (krpch_ioctl(cmd, data));
}



krpchdd_read(dev, uio, flag)
dev_t dev;
struct uio *uio;
{
    return(krpch_read(uio));
}


krpchdd_write(dev, uio)
dev_t dev;
struct uio *uio;
{
    return(krpch_write(uio));
}



struct sel_queue krpch_selq;

/*
 *  krpchdd_select
 *
 *     The only events we watch for are POLLNORM and POLLOUT
 *     POLLOUT is always true;  POLLNORM is true when there is
 *     a request on the pending queue.
 */


int krpchdd_select (dev, events, revents, scanning)
     dev_t dev;
     short *events, *revents;
     int scanning;
{
     if (revents)
		*revents = 0;
     if (scanning){
         if (*events & POLLNORM){   /* check for message available */
             if (!QUEUE_EMPTY(pending)){
	         *revents |= POLLNORM;
	     }
             else
                 select_enqueue(&krpch_selq);
	 }
	 if (*events & POLLOUT)     /* always writeable */
	     *revents |= POLLOUT;  
     }else
         select_dequeue(&krpch_selq);

     return(0);
} 



#ifdef USE_FILEOPS

int
init_file_descrip(fd)
    int *fd;
{
    int indx, error=0;
    struct file *fp;

    if (error = falloc(&fp, &indx))
        return(error);
    ASSERT(u.u_ofile[indx] == U_FD_RESERVED);
    ASSERT(fp->f_count == 1 && fp->f_type == DTYPE_RESERVED);
    FP_LOCK(fp);
    fp->f_flag = FREAD|FWRITE;
    fp->f_type = DTYPE_DFS_SPECIAL;
    fp->f_ops = &krpc_helper_fops;
    fp->f_data = (caddr_t) NULL;
    FP_UNLOCK(fp);
    U_FD_SET(indx, fp, &u.u_file_state);
    *fd = indx;
    return(0);
}


#endif
