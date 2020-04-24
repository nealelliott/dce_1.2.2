/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * @DEC_COPYRIGHT@
 * Copyright (c) 1996
 * Digital Equipment Corporation, Maynard, Massachusetts, USA
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: bt_thread.c,v $
 * Revision 1.1.2.1  1996/10/03  20:22:37  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/03  19:17:48  arvind]
 *
 * Revision /main/DCE_1.2.2/1  1996/09/09  21:21 UTC  arvind
 * 	PKSS drop from DEC (DCE1.2.2)
 * 	[1996/08/30  15:36 UTC  arvind  /main/arvind_pkss/1]
 * 
 * 	GLORIA (R1.2.2) revision.
 * 	[1996/06/24  20:47:32  zee]
 * 
 * $EndLog$
 */
/*
 * module: bt_thread.c
 * 
 * Contains procedures having to do with the startup and 
 * shutdown of threads which access the database.
 */
#include <stdio.h>
#include <db.h>
#include <btree.h>

/*
 * get_thread()
 *
 * Gets a thread's own TS_BTREE pointer.  If this 
 * thread doesn't have one yet, creates one.
 */
TS_BTREE *get_thread (BTREE *z) {
    int status; /* 0 = success */
    TS_BTREE *x = NULL;

    status = pthread_getspecific (z->bt_ts_key, (void *)&x);
    if (x == NULL) 
      x = bt_ts_new(z);
    return(x);
}

/*
 * bt_ts_new()
 *
 * Initialize a thread-specific data buffer for accessing
 * this tree.
 * 
 * Input:
 *       t - btree
 *
 * Returns: 
 *       initialized thread-specific buffer for this thread
 */

TS_BTREE *
bt_ts_new(t) 
     BTREE *t; 
{

   TS_BTREE *ts_buf;

   /*
    * Allocate thread-specific data buffer for this thread now,
    * associate it with the key, and insert it in the queue.
    */
   ts_buf = (TS_BTREE *)malloc(sizeof(TS_BTREE));
   if (ts_buf == NULL) {
      return (NULL);
   }
   
   if (pthread_setspecific (t->bt_ts_key, (void *)ts_buf) != 0) {
      free (ts_buf);
      return (NULL);
   }

   bzero((char *)ts_buf, sizeof(*ts_buf));

   ts_buf->bt_tree = t;
   ts_buf->bt_closed = FALSE;

   TREE_LOCK(t) {
       CIRCLEQ_INSERT_HEAD(&(t->bt_tq), ts_buf, tq);
   } TREE_UNLOCK(t);

   return (ts_buf);   
}

/*
 * bt_ts_destruct
 * 
 * Destructor called by pthreads when a thread exits that 
 * has a btree thread-specific data area.
 *
 * Input: 
 *        t = btree structure address
 *        ts = thread-specific btree info structure address
 * Returns:
 *        none
 */
void
bt_ts_destruct(ts) 
     TS_BTREE *ts;
{
   
   /*
    * Take this thread off the tree's thread queue, if 
    * the queue still exists (haven't called bt_close())
    */
   if (!ts->bt_closed ) {
       TREE_LOCK(ts->bt_tree) {
	   CIRCLEQ_REMOVE(&(ts->bt_tree->bt_tq), ts, tq);
       } TREE_UNLOCK(ts->bt_tree);
   }       
   
   /*
    * If the thread has any records pinned, then unpin.
    */
   if (ts->bt_pinned != NULL) {
      mpool_put(ts->bt_tree->bt_mp, ts->bt_pinned, 0);
      ts->bt_pinned = NULL;
   }
   
   /* 
    * Free cursor memory. 
    */
   if (ts->bt_cursor.key.data != NULL) {
       free(ts->bt_cursor.key.data);
       ts->bt_cursor.key.size = 0;
       ts->bt_cursor.key.data = NULL;
   }

   /*
    * Free data buffers
    */
   if (ts->bt_rkey.data != NULL) free(ts->bt_rkey.data);
   if (ts->bt_rdata.data != NULL) free(ts->bt_rdata.data);
}

/*
 * bt_ts_display 
 */
void bt_ts_display (t) 
     BTREE *t;
{
   TS_BTREE *tqe;


   TREE_LOCK(t) {

       for (tqe = (TS_BTREE *)t->bt_tq.cqh_first; 
	    tqe != (TS_BTREE *)&t->bt_tq; 
	    tqe = ((ts_qentry *)tqe)->cqe_next) {
	   
	   printf("thread-specific entry address: 0x%lx\n", tqe);      
	   printf("bt_tree pointer: 0x%lx\n", tqe->bt_tree);
	   printf("bt_cur: page=%d, index=%d\n", tqe->bt_cur.page, tqe->bt_cur.index);
	   printf("bt_pinned pointer: 0x%lx\n", tqe->bt_pinned);
	   printf("bt_cursor: pg=%d, key size=%d, key data=%s, flags=0x%hx\n", 
		  tqe->bt_cursor.pg, 
		  tqe->bt_cursor.key.size,
		  tqe->bt_cursor.key.data,
		  tqe->bt_cursor.flags);
	   printf("bt_rkey: size=%d, data=%s\n", tqe->bt_rkey.size, tqe->bt_rkey.data);
	   printf("bt_rdata: size=%d, data=%s\n", tqe->bt_rdata.size, tqe->bt_rdata.data);
       }

   } TREE_UNLOCK(t);

}
