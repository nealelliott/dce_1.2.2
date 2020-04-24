/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: gda_rwl.h,v $
 * Revision 1.1.6.2  1996/02/18  23:34:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:54  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:16:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:43:22  root]
 * 
 * Revision 1.1.4.4  1994/08/03  19:02:27  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:58:17  mccann]
 * 
 * Revision 1.1.4.3  1994/06/23  18:30:37  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  19:18:43  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:39:16  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:11:55  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:03:37  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:17:12  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:32:19  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:51:43  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:15:50  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _GDA_RWL_H
#define _GDA_RWL_H
/*
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1991-1994. ALL RIGHTS RESERVED.
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
 */
/* rwl.h: contains all of the definitions necessary to use the readers-writers
	lock module. */

#ifndef OK
#define	OK			0
#endif 

#define	RWL_FREE_FAIL			-3

#define	READ_LOCK_FAIL			-5
#define	READ_LOCK_DELETED		-7

#define	WRITE_LOCK_FAIL			-8
#define	WRITE_LOCK_DELETED		-10

#define	READ_TO_WRITE_LOCK_FAIL		-11
#define	READ_TO_WRITE_LOCK_BLOCKED	-13

#define	WRITE_TO_READ_LOCK_FAIL		-14

#define	READ_UNLOCK_FAIL		-16

#define	WRITE_UNLOCK_FAIL		-18

#define	READ_TRYLOCK_FAIL		-20

#define	WRITE_TRYLOCK_FAIL		-22

typedef struct rwl_t {
	pthread_mutex_t	mutex;
	pthread_cond_t	readers;
	pthread_cond_t	writers;
	pthread_cond_t	readers_to_writers;
	int		writing;
	int		num_reading;
	int		reader_to_writer_waiting;
	int		num_readers_waiting;
	int		num_writers_waiting;
	int		num_writers_waiting_phase;
	int		deleted;
} rwl_t;

extern void
rwl_init (
    rwl_t          *);

extern void
rwl_free (
    rwl_t           *);

extern int	
read_lock (
    rwl_t           *);

extern int	
write_lock (
    rwl_t           *);

extern int	
read_to_write_lock (
    rwl_t           *);

extern int	
write_to_read_lock (
    rwl_t           *);

extern void
read_unlock (
    rwl_t           *);

extern void
write_unlock (
    rwl_t           *);

extern int	
read_trylock (
    rwl_t           *);

extern int	
write_trylock (
    rwl_t           *);
#endif  /* #ifndef _GDA_RWL_H */










