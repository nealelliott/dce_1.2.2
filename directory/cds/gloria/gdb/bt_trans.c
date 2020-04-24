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
 * $Log: bt_trans.c,v $
 * Revision 1.1.2.1  1996/10/03  20:22:49  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/03  19:17:59  arvind]
 *
 * Revision /main/DCE_1.2.2/1  1996/09/09  21:21 UTC  arvind
 * 	PKSS drop from DEC (DCE1.2.2)
 * 	[1996/08/30  15:36 UTC  arvind  /main/arvind_pkss/1]
 * 
 * 	GLORIA (R1.2.2) revision.
 * 	[1996/06/24  20:47:34  zee]
 * 
 * $EndLog$
 */
/*
 * Module: bt_trans.c
 *     Generic DataBase (GDB) transaction semantics modifications for
 *     the modified UCB DB44.
 */

#include <stdio.h>
#include <db.h>
#include <btree.h>


/*
 * bt_txn_begin
 */
int bt_txn_begin (dbp)
     const DB *dbp;
{
   BTREE *tree;
   MPOOL *mp;

   /*
    * Find the mpool pointer
    */
   tree = dbp->internal;
   mp = tree->bt_mp;

   /*
    * Transaction logic doesn't apply to in-memory databases
    */
   if (F_ISSET(tree, B_INMEM))
     return (RET_ERROR);

   MPOOL_TXN_BEGIN(mp);

   return (RET_SUCCESS);
}


/*
 * bt_txn_commit -- Commit the transaction to non-volatile memory
 *
 * Parameters:
 *   MPOOL *mp
 *
 * Returns:
 *   RET_SUCCESS, RET_ERROR
 */
int bt_txn_commit(dbp) 
     const DB *dbp;
{
   BTREE *tree;
   MPOOL *mp;
   int status;

   /*
    * Find the mpool pointer
    */
   tree = dbp->internal;
   mp = tree->bt_mp;
   
   /*
    * Transaction logic doesn't apply to in-memory databases
    */
   if (F_ISSET(tree, B_INMEM))
     return (RET_ERROR);

   /*
    * mpool does all the work 
    */
   status = mpool_txn_commit(mp);
   
   return (status);
}



/*
 * bt_txn_rollback -- Rollback the transaction to the last commit point,
 *                    we DON'T want to make these changes permanent
 *
 * Parameters:
 *   MPOOL *mp
 *
 * Returns:
 *   RET_SUCCESS, RET_ERROR
 */
int bt_txn_rollback(dbp) 
     const DB *dbp;
{
   BTREE *tree;
   MPOOL *mp;
   int status;
   TS_BTREE *ts;


   /*
    * Find the mpool pointer
    */
   tree = dbp->internal;
   mp = tree->bt_mp;

   GET_THREAD(tree,ts);

   /*
    * Transaction logic doesn't apply to in-memory databases
    */
   if (F_ISSET(tree, B_INMEM))
     return (RET_ERROR);

   /*
    * Throw away cheater info in the btree structure 
    */
   ts->bt_cur.page = NULL;
   ts->bt_cur.index = 0;
   ts->bt_pinned = (PAGE *)0;
   ts->bt_cursor.pg.pgno = P_INVALID;
   ts->bt_cursor.pg.index = 0;
   ts->bt_cursor.key.data = NULL; 
   ts->bt_cursor.key.size = 0;    /* 0 size signals key data is not to
				     be used */

   /*
    * We don't need to lock the tree mutex while changing members  
    * because caller must hold the writelock to make this call.  
    * So, no other thread is reading or writing.
    */
   tree->bt_last.pgno = P_ROOT;
   tree->bt_last.index = 0;
   tree->bt_order = NOT;
   tree->bt_free = P_INVALID;
   BT_CLR(ts);

   /*
    * mpool does most of the work 
    */
   status = mpool_txn_rollback(mp);
   
   return (status);
}

