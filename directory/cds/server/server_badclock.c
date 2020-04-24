/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: server_badclock.c,v $
 * Revision 1.1.4.2  1996/02/18  19:36:31  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:17:07  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:31:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:40  root]
 * 
 * Revision 1.1.2.1  1994/08/19  21:10:58  zee
 * 	Initial version.
 * 	[1994/08/19  18:09:40  zee]
 * 
 * $EndLog$
 */
/*
 * Module server_badclock.c
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1994. ALL RIGHTS RESERVED.
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
 * Module description:
 *
 * When a system clock (on a server with master replicas of some 
 * directories) jumps forward, there is no way for the server to detect 
 * whether or not the time is sane.  When update transactions are 
 * processed (or a background pass starts), the server will generate 
 * timestamps which the rest of the namespace servers will consider 
 * invalid.  The clearinghouse will be damaged.
 *
 * The BadClock Fixer can recover a clearinghouse from the "time jumps 
 * forward" problem with a minimum of manual intervention.  Prior to 
 * invoking the BadClock Fixer, the system clock must be restored to some 
 * earlier time, preferably the "real" time.
 *
 * The ResetBase for the analyze command is the current time (ie. 
 * the time restored to "correct" time).
 *
 * Use the RepairTimestamps_schedule routine to create a timestamp
 * repair thread.  As the thread goes on its merry way, it is expected that
 * you also call GetRepairStatus to get ongoing and final resolution of
 * the thread status so it knows when to cleanup.  Also, this software
 * assumes you'll only be running this one CH at a time per server.
 *
 *
 * Recent modifications:
 *
 *  9-Jul-1993, Gen 1 by Julie Corenzwit
 *	Original version.
 *
 *  22-Sep-1993, Gen 2 by Julie Corenzwit
 *	Count and report the number of records and directories
 *	that contain bad timestamps.
 *
 *  18-Aug-1994, Roger Zee
 *      Modified for CDS usage: Revamped to use its own thread and to
 *      report back its current state.
 */

#include <server.h>
#include <dbdef.h>
#include <sys_time.h>
#include <dns_record.h>
#include <attributes.h>
#include <find.h>
#include <cprpcp.h>

/*
 *****************************************************************************
 * External Global Declarations
 *****************************************************************************
 */
#ifdef DCE_SEC
extern nsgbl_ns_t dns_nameserver;
#endif

/*
 *****************************************************************************
 * Module-specific Declarations
 *****************************************************************************
 */

typedef enum {
    tsrep_Off = 0,		/* No ongoing badclock repair */
    tsrep_Disable,		/* CH being disabled */
    tsrep_Repair,		/* CH being repaired */
    tsrep_Checkpt,		/* CH checkpointing to disk */
    tsrep_Enable,		/* CH being enabled */
    tsrep_Done		        /* BadClock repair completed */
} ts_repair_state_t;

/*
 * Data structures for bad timestamp repair
 */
typedef struct ts_repair {
    dthread_mutex_t    ts_rep_mutex;
    ts_repair_state_t  ts_rep_state;         /* BadClock repair state */
    dthread_cond_t     ts_rep_state_changed; /* If the above changes */
    int                ts_rep_status;        /* BadClock fixer status */
    Timestamp_t	       file_badclk_ResetBase; /* ResetBase for BadClock Fixer */
    Timestamp_t	       file_badclk_LeapLB;   /* LeapLowerBound for BadClock Fixer */
    Timestamp_t	       file_badclk_LastTick; /* LastTick before ResetBase */
} ts_repair_t;
static ts_repair_t repair_info;
static dthread_t tsrep_thread;

/*
 *****************************************************************************
 * Miscellaneous Macros and Definitions
 *****************************************************************************
 */

/*
 *  DTSS UTC base time is October 15, 1582 (Gregorian).
 *  DNS base time is November 17, 1858 (Smithsonian).
 */
#define BASE_TIME_DIFF_LO   0xC7960000
#define BASE_TIME_DIFF_HI   0x0135886A
#define BASE_TIME_DIFF      0x0135886AC7960000
/* 
 * End of time falls somewhere around 22-Sep-16472.  If your
 * clock gets set this fast, watch out.  If you are using or 
 * maintaining this program this late, really watch out!!!
 */
#define ENDOF_TIME_LO	    0xFFFFFFFF
#define ENDOF_TIME_HI	    0x3FFFFFFF
#define ENDOF_TIME	    0x3FFFFFFFFFFFFFFF

/* Difference between smithsonian zero and gregorian zero */
#if LONG_BIT >= 64
  static time_quad_t base = BASE_TIME_DIFF;
  static time_quad_t two = 2;
  static time_quad_t endof_time = ENDOF_TIME;
# define SET_TQ_ODD(p) p |= 1L
#else
  static time_quad_t base = {BASE_TIME_DIFF_LO, BASE_TIME_DIFF_HI};
  static time_quad_t two = {2, 0};
  static time_quad_t endof_time = {ENDOF_TIME_LO, ENDOF_TIME_HI};
# define SET_TQ_ODD(p) p.tq_lo |= 1
#endif

typedef struct report_data {
    Time_u	 localTimeShift;  /* input to CheckTimestamp routine */
    Timestamp_t	 localResetBase;  /* input to UpdateReport and CheckTimestamp */
    Timestamp_t	 LeapLowerBound;  /* min timestamp later than the ResetBase */
    Timestamp_t	 LeapUpperBound;  /* max timestamp later than the ResetBase */
    Timestamp_t	 LastTick;	  /* max timestamp earlier than the ResetBase */
    unsigned int Leaps;		  /* leap counter */
    unsigned int LeapRecords;	  /* count of records and dirs with leaps */
} report_data_t;

/*
 *****************************************************************************
 * Forward Declarations and Prototypes
 *****************************************************************************
 */

INTERNAL unsigned char
ScanRecord (
    DBSet_t	   *,
    report_data_t  *,
    unsigned char (*)());

INTERNAL void
ScanDataBase (
    file_info_t	   *,
    report_data_t  *,
    unsigned char (*)());
    
INTERNAL unsigned char
UpdateReport (
    report_data_t *,
    Timestamp_t   *);

INTERNAL unsigned char
CheckTimestamp (
    report_data_t *,
    Timestamp_t	  *);

INTERNAL void
DoAnalyze (
    file_info_t	  *,
    report_data_t *);
    
INTERNAL void
DoRepair (
    file_info_t	  *,
    report_data_t *);
    
PUBLIC int
RepairBadClock (
    file_info_t	 *);

PUBLIC int
CancelRepairState (
    file_info_t	 *);

INTERNAL dthread_address_t 
ts_repair_thread (
    dthread_address_t);

PUBLIC int
RepairTimestamps_schedule (
    file_info_t  *);

PUBLIC int
GetRepairStatus ();

INTERNAL void
CleanupTSRepair ();


/*
 * ScanRecord
 *	This routine finds all the timestamps in a record.
 *
 * Input:
 *	main_set_p = pointer to the record
 *	report_p = pointer to structure for report data
 *	ts_routine = pointer to action routine to call per timestamp
 *
 * Value:
 *	TRUE if any bad timestamps are found
 *	FALSE if no bad timestamps are found
 *
 */
INTERNAL unsigned char
ScanRecord (DBSet_t         *main_set_p,
	    report_data_t   *report_p,
	    unsigned char  (*ts_routine)())
{
    int			att_index;
    int			val_index;
    int			cnt;
    unsigned char	bad_ts = FALSE;

    /* Process deleted records of the same name */
    if (0 < (cnt = NUM_DBSet_Delete(main_set_p))) {
	delete_holder_t *delete_p =
	  (delete_holder_t *)DATA_DBSet_Delete(main_set_p);

	do
	  {
	      DCE_SVC_DEBUG((
		  cds__svc_handle,
		  cds_svc_server,
		  svc_c_debug1,
		  "  ScanRecord: processing a same-key deleted record, addr = %x",
		  delete_p));

	      /* call Timestamp action routine */
	      bad_ts = (ts_routine)(report_p, delete_p->del_ts) || bad_ts;
	      bad_ts = (ts_routine)(report_p, delete_p->del_uid) || bad_ts;
	  } while (delete_p++, --cnt);
    }

    /* Note that we don't check for an absent record.  Even
     * though a record is absent, it will be all full of "absent"
     * contents, which may have bad timestamps we have to fix.
     */

    /* Find all the attributes for this record */
    for (att_index = 0; att_index < NUM_DBSet(main_set_p); ++att_index) {
	DBSetMember_t	*value_p;
	AttributeName_t *name_p = (AttributeName_t *)
			INDEX_DBSet(main_set_p, att_index);
	DBSet_t *set_p = (DBSet_t *)SKIP_AttributeName(name_p);
	attribute_descriptor_t *dns_p = find_known_attribute(name_p);
	DEB_ASCII_BUF_DBSet(attr_set_buf)
	DEB_ASCII_BUF_AttributeName(attr_name_buf) 

	ALIGN_CHECK(set_p);

	DCE_SVC_DEBUG((
		       cds__svc_handle,
		       cds_svc_server,
		       svc_c_debug1,
		       "  ScanRecord: processing attribute %s(%s)",
		       deb_ascii_AttributeName(attr_name_buf, name_p),
		       deb_ascii_DBSet(attr_set_buf, set_p)));

	/* call Timestamp action routine */
	bad_ts = (ts_routine)(report_p, set_p->ds_ts) || bad_ts;

	if (dns_p) {
	    if (dns_p->vtype == VT_Timestamp) {
		value_p = (DBSetMember_t *)DATA_DBSet(set_p);
		if (value_p->dm_value_present) {
		    DCE_SVC_DEBUG((
		        cds__svc_handle,
			cds_svc_server,
			svc_c_debug1,
			"  ScanRecord: processing a timestamp value"));

		    /* call Timestamp action routine */
		    bad_ts = (ts_routine)(report_p, value_p->dm_data) || bad_ts;
		}
	    }
	}
	if (NUM_DBSet(set_p) != 0) {	/* non-empty set or single-valued */
	    for (val_index = 0; val_index < NUM_DBSet(set_p); val_index++) {
		value_p = (DBSetMember_t *)INDEX_DBSet(set_p, val_index);

		/* call Timestamp action routine */
		bad_ts = (ts_routine)(report_p, value_p->dm_ts) || bad_ts;
	    }
	}
    }		/* end for <main set attribute index> */

    return bad_ts;
}

/*
 * ScanDataBase
 *	This routine traverses the clearinghouse data base.
 *
 * Input:
 *	file_p = pointer to the file info block
 *	report_p = pointer to structure for report data
 *	ts_routine = pointer to action routine to call per timestamp
 *
 * Value:
 *	
 */
INTERNAL void
ScanDataBase (file_info_t     *file_p,
	      report_data_t   *report_p,
	      unsigned char  (*ts_routine)())
{
    int			index;
    dir_hash_entry_t	*hash_p;
    node_header_t	*node_hdr_p;
    unsigned int	pos;
    entry_key_t		*entry_p;
    DBSet_t		*main_set_p;
    DEB_ASCII_BUF_dir_hash_entry(hash_entry_buf)
    DEB_ASCII_BUF_directory_bucket(dir_bkt_buf)
    DEB_ASCII_BUF_entry_key(entry_key_buf)

    for (index = 0; index < UID_HASH_LEN; ++index) {
	hash_p = file_p->file_uid_table[index];

	for (; hash_p; hash_p = hash_p->dir_uid_collision_p) {
	    DCE_SVC_DEBUG((
	        cds__svc_handle,
		cds_svc_server,
		svc_c_debug1,
		"  ScanDataBase: processing a hash table entry (%s)",
	        deb_ascii_dir_hash_entry(hash_entry_buf, hash_p)));

	    /*
	     * Don't do clearinghouses.
	     */
	    if (hash_p->dir_type == ET_clearinghouse) {
		DCE_SVC_DEBUG((
	            cds__svc_handle,
		    cds_svc_server,
		    svc_c_debug1,
		    "  ScanDataBase: skipping a clearinghouse entry (%s)",
		    deb_ascii_directory_bucket(dir_bkt_buf, hash_p->dir_p)));

		continue;
	    }

	    /*
	     * Do the directory bucket
	     */
	    DCE_SVC_DEBUG((
	        cds__svc_handle,
		cds_svc_server,
		svc_c_debug1,
		"  ScanDataBase: processing a directory entry (%s)",
		deb_ascii_directory_bucket(dir_bkt_buf, hash_p->dir_p)));

	    main_set_p = (DBSet_t *)hash_p->dir_p->dir_data;
	    ALIGN_CHECK(main_set_p);

	    /* Find all the Timestamps in the directory bucket */
	    if (ScanRecord(main_set_p, report_p, ts_routine)) {
		report_p->LeapRecords++;
	    }

	    /*
	     * Find all the data buckets for this directory
	     */
	    node_hdr_p = db_btree_get_first_data_bucket(hash_p->dir_entry_p);
	    while (node_hdr_p != 0) {
		if (node_hdr_p->node_count == 0)	continue;
		/*
		 * Find all the records in this bucket
		 */
		for (pos = 0; pos < node_hdr_p->node_count; ++pos) {
		    /* See if this is dummy record */
		    if (!node_hdr_p->node_next_p && (pos == node_hdr_p->node_count-1))
		        break;

		    /* Get pointer to data record in data bucket */
		    entry_p = (entry_key_t *)INDEX_node_rec(node_hdr_p, pos);

		    DCE_SVC_DEBUG((
	                cds__svc_handle,
		        cds_svc_server,
		        svc_c_debug1,
		        "  ScanDataBase: skipping a clearinghouse entry (%s)",
			deb_ascii_entry_key(entry_key_buf, entry_p)));

		    main_set_p = (DBSet_t *)DATA_entry_data_record(entry_p);
		    ALIGN_CHECK(main_set_p);

		    /* Find all the Timestamps in this record. */
		    if (ScanRecord(main_set_p, report_p, ts_routine)) {
			report_p->LeapRecords++;
		    }
		}		/* end for <all records> */
		node_hdr_p = node_hdr_p->node_next_p;
	    } 		/* end while <all buckets> */
	}	    /* end for <hash uid collision chain> */
    }		/* end for <hash table index> */

    return;
}

/*
 * UpdateReport
 *	This routine updates any fields in the report structure
 *	that need to be changed based on the provided timestamp.
 *
 * Input:
 *	report_p = pointer to the structure containing the reporting variables
 *	timestamp_p = pointer to a timestamp encountered in the data base
 *
 * Value:
 *	TRUE if the timestamp is bad
 *	FALSE if the timestamp is ok
 *	
 */
INTERNAL unsigned char
UpdateReport (report_data_t *report_p,
	      Timestamp_t   *timestamp_p)
{
    unsigned char bad_ts = FALSE;
    DEB_ASCII_BUF_Timestamp(ts_buf)

    if (GT_Time(timestamp_p->ts_time, report_p->localResetBase.ts_time)) {
	if (LT_Time(timestamp_p->ts_time, report_p->LeapLowerBound.ts_time))
	    COPY_Timestamp(timestamp_p, &report_p->LeapLowerBound);
	if (GT_Time(timestamp_p->ts_time, report_p->LeapUpperBound.ts_time))
	    COPY_Timestamp(timestamp_p, &report_p->LeapUpperBound);
	report_p->Leaps++;
	bad_ts = TRUE;
    }
    else if (LT_Time(timestamp_p->ts_time, report_p->localResetBase.ts_time)) {
	if (GT_Time(timestamp_p->ts_time, report_p->LastTick.ts_time))
	    COPY_Timestamp(timestamp_p, &report_p->LastTick);
    }

    return bad_ts;
}

/*
 * CheckTimestamp
 *	This routine calls the UpdateReport routine (so the Repair phase will 
 *	also produce an analysis report) and if the provided timestamp is 
 *	later than the ResetBase, it adjusts it downwards by the difference 
 *	between the ResetBase and the LeapLowerBound.
 *
 * Input:
 *	report_p = pointer to the structure containing the reporting variables
 *	timestamp_p = pointer to a timestamp encountered in the data base
 *
 * Value:
 *	TRUE if the timestamp is bad
 *	FALSE if the timestamp is ok
 *	
 */
INTERNAL unsigned char
CheckTimestamp (report_data_t *report_p,
		Timestamp_t   *timestamp_p)
{
    DEB_ASCII_BUF_Timestamp(ts_buf)

    /*
     * Create a new report in case the ResetBase was overridden.
     */
    UpdateReport(report_p, timestamp_p);

    /*
     * Shift this timestamp back in time if it is past the ResetBase.
     */
    if (GT_Time(timestamp_p->ts_time, report_p->localResetBase.ts_time)) {
	SUB_Time((time_quad_t *) timestamp_p->ts_time,
		 (time_quad_t *) report_p->localTimeShift, 
		 (time_quad_t *) timestamp_p->ts_time);
	DCE_SVC_DEBUG((
		       cds__svc_handle,
		       cds_svc_server,
		       svc_c_debug1,
		       "  CheckTimestamp: timestamp changed to %s",
		       deb_ascii_Timestamp(ts_buf, timestamp_p)));
	return TRUE;
    }

    return FALSE;
}

/*
 * DoAnalyze
 *	This routine initializes the BadClock analysis variables 
 *	and traverses the clearinghouse data base.
 *
 * Input:
 *	file_p = pointer to the file info block
 *	report_p = pointer to structure for report data
 *
 * Value:
 *	
 *
 */
void
DoAnalyze(file_p, report_p)
    file_info_t	*file_p;
    report_data_t *report_p;
{
    int			index;
    DEB_ASCII_BUF_Timestamp(LowerBnd_buf)
    DEB_ASCII_BUF_Timestamp(UpperBnd_buf)
    DEB_ASCII_BUF_Timestamp(LastTick_buf)
    DEB_ASCII_BUF_Timestamp(ResetBase_buf)

    /*
     * Initialize the report variables.
     */
    SUB_Time((time_quad_t *) repair_info.file_badclk_LeapLB.ts_time,
	     (time_quad_t *) repair_info.file_badclk_ResetBase.ts_time, 
	     (time_quad_t *) report_p->localTimeShift);
    COPY_Timestamp(&repair_info.file_badclk_ResetBase, &report_p->localResetBase);
    ZERO_bytes(report_p->LeapLowerBound.ts_node, 
		LEN_Node(report_p->LeapLowerBound.ts_node));
    COPY_Time(&endof_time, report_p->LeapLowerBound.ts_time);
    ZERO_Timestamp(&report_p->LeapUpperBound);	/* beginning of time */
    ZERO_Timestamp(&report_p->LastTick);	/* beginning of time */
    report_p->Leaps = report_p->LeapRecords = 0;

    /*
     * Scan the clearinghouse data base, calling the CheckTimestamp
     * routine for each timestamp that might need fixing.
     */
    ScanDataBase(file_p, report_p, UpdateReport);

    /*
     * Make the calculated ResetBase unique by adding a couple of ticks.
     */
    ADD_Time((time_quad_t *) report_p->LastTick.ts_time,
	     &two,
	     (time_quad_t *) report_p->LastTick.ts_time);

    /*
     * Load the calculated ResetBase into the file info block.  This will be 
     * the default for the repair phase.  
     */
    COPY_Timestamp(&report_p->LastTick, &repair_info.file_badclk_ResetBase);
    /* and remember these to validate any ResetBase override on Repair */
    COPY_Timestamp(&report_p->LeapLowerBound, &repair_info.file_badclk_LeapLB);
    COPY_Timestamp(&report_p->LastTick, &repair_info.file_badclk_LastTick);

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_server,
	svc_c_debug1,
	" <DoAnalyze(%s, %s, %s, %u, %u) - ResetBase %s",
	deb_ascii_Timestamp(LowerBnd_buf, &report_p->LeapLowerBound), 
	deb_ascii_Timestamp(UpperBnd_buf, &report_p->LeapUpperBound), 
	deb_ascii_Timestamp(LastTick_buf, &report_p->LastTick), 
	report_p->Leaps, report_p->LeapRecords,
	deb_ascii_Timestamp(ResetBase_buf, &repair_info.file_badclk_ResetBase)));
    return;
}

/*
 * DoRepair
 *	This routine initializes the BadClock analysis variables 
 *	and traverses the clearinghouse data base for the Repair 
 *	phase.  During the Repair phase, since the ResetBase may 
 *	be different from the Analysis ResetBase, it also repeats
 *	the Analysis so it can report on what it did.
 *
 * Input:
 *	file_p = pointer to the file info block
 *	report_p = pointer to structure for report data
 *
 * Value:
 *	
 *
 */
INTERNAL void
DoRepair (file_info_t    *file_p,
	  report_data_t  *report_p)
{
    int		index;
    DEB_ASCII_BUF_Timestamp(LowerBnd_buf)
    DEB_ASCII_BUF_Timestamp(UpperBnd_buf)
    DEB_ASCII_BUF_Timestamp(LastTick_buf)
    DEB_ASCII_BUF_Timestamp(ResetBase_buf)

    /*
     * Initialize the report variables.
     */
    SUB_Time((time_quad_t *) repair_info.file_badclk_LeapLB.ts_time,
	     (time_quad_t *) repair_info.file_badclk_ResetBase.ts_time, 
	     (time_quad_t *) report_p->localTimeShift);
    COPY_Timestamp(&repair_info.file_badclk_ResetBase, &report_p->localResetBase);
    ZERO_bytes(report_p->LeapLowerBound.ts_node, 
		LEN_Node(report_p->LeapLowerBound.ts_node));
    COPY_Time(&endof_time, report_p->LeapLowerBound.ts_time);
    ZERO_Timestamp(&report_p->LeapUpperBound);	/* beginning of time */
    ZERO_Timestamp(&report_p->LastTick);	/* beginning of time */
    report_p->Leaps = report_p->LeapRecords = 0;

    /*
     * Scan the clearinghouse data base, calling the 
     * CheckTimestamp routine for each timestamp that might need
     * fixing. 
     */
    ScanDataBase(file_p, report_p, CheckTimestamp);

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_server,
	svc_c_debug1,
	" <DoRepair(%s, %s, %s, %u, %u) - ResetBase %s",
	deb_ascii_Timestamp(LowerBnd_buf, &report_p->LeapLowerBound), 
	deb_ascii_Timestamp(UpperBnd_buf, &report_p->LeapUpperBound), 
	deb_ascii_Timestamp(LastTick_buf, &report_p->LastTick), 
	report_p->Leaps, report_p->LeapRecords,
	deb_ascii_Timestamp(ResetBase_buf, &repair_info.file_badclk_ResetBase)));
    return;
}

/*
 * RepairBadClock 
 *	This routine goes through the following states:
 *        - Disables the CH
 *        - Analyzes and Repairs bad timestamps
 *        - Checkpoints the fixed, in-memory database to disk
 *        - Enables the CH
 *        - Done.
 *
 *      ASSUMES YOU HAVE THE dns_nameserver.ns_mutex LOCKED!!
 *
 * Input:
 *	file_p = pointer to open stream
 *
 * Value:
 *	CDS_SUCCESS
 */
PUBLIC int
RepairBadClock (file_info_t  *file_p)
{
    int		   status = CDS_SUCCESS;
    int            thstatus;
    time_quad_t	   current_time;
    Timestamp_t    current_ts;
    file_state_t   curr_state;
    report_data_t  report;
    ObjUID_t       saved_chuid;
    nsgbl_ch_t    *ch_p;
    nsgbl_ns_t    *nsgbl_p = &dns_nameserver;

    /* 
     * Disable the clearinghouse and set the new state
     *
     * What we would really like here is to lock, disable, change state from 
     * loaded to repair, and finally unlock.  Unfortunately, db_disable does 
     * its own locking and unlocking.  We will assume that after we have 
     * disabled, someone re-enables before we change state to repair.
     */
    while (1) {
	SERVER_LOCK(file_p->file_mutex)
	/* Beginning of the 'protected code' */
	{

	    SERVER_LOCK(repair_info.ts_rep_mutex)
	    repair_info.ts_rep_state = tsrep_Disable;
	    if ((thstatus = dthread_broadcast(&(repair_info.ts_rep_state_changed))) < 0) {
		dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, (long)thstatus);
	    }
	    SERVER_END_LOCK(repair_info.ts_rep_mutex)
	
	    if ((file_p->file_state == file_loaded) || 
		(file_p->file_state == file_badclk_repair))
	      break;	/* get out of loop still holding lock !!! */

	}	/* End of the 'protected code' */
	SERVER_END_LOCK(file_p->file_mutex)

	back_propagate_kill(file_p);

	status = db_disable(&file_p->file_ch_p->ch_uid);
	if (status != CDS_BADCLOCK && status != CDS_SUCCESS) {
	    return status;
	}
    }  /* end while */

    if (file_p->file_state == file_loaded) {
	file_p->file_state = file_badclk_repair;
	if ((thstatus = dthread_broadcast(&(file_p->file_state_changed))) < 0) {
	    dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, (long)thstatus);
	}
	SERVER_LOCK(repair_info.ts_rep_mutex)
	repair_info.ts_rep_state = tsrep_Repair;
	if ((thstatus = dthread_broadcast(&(repair_info.ts_rep_state_changed))) < 0) {
	    dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, (long)thstatus);
	}
	SERVER_END_LOCK(repair_info.ts_rep_mutex)
    }
    else {
	DCE_SVC_DEBUG((
		       cds__svc_handle,
		       cds_svc_server,
		       svc_c_debug1,
		       "  RepairBadClock: Bad file state, state = (%d)",
		       file_p->file_state));
	status = CDS_WRONGSTATE;
    }

    /* For when you break out of the above while loop still holding the lock */
    SERVER_END_LOCK(file_p->file_mutex)


    if (status != CDS_SUCCESS) {
	return status;
    }
    /*
     * Determine the current time.  First get a full-blown timestamp.  Then 
     * replace the time field of the timestamp with the true current time, 
     * since the timestamp generator is probably corrupted with a future time. 
     * We will print this even if a ResetBase was provided on the command
     * line, just in case anyone is curious. 
     */
    (void)ts_new(&current_ts);	/* ignore CDS_BADCLOCK status; we don't care */

    sys_time_to_utc(NULL, &current_time);
    SET_TQ_ODD(current_time);
    COPY_Time(&current_time, current_ts.ts_time);

    /*
     * Use the current time as the reset base.
     */
    COPY_Timestamp(&current_ts, &repair_info.file_badclk_ResetBase); 

    /* Off and running */
    DoAnalyze(file_p, &report);
    DoRepair(file_p, &report);

    /* Reset the file state */
    CancelRepairState(file_p);

    /* Important!!  Reset the various saved latest times used */
    ts_set_last_ts(&current_ts);
    COPY_Timestamp(&current_ts,
		   (Timestamp_t *)file_p->file_ckpt_hdr.hdr_last_cts);

    SERVER_LOCK(file_p->file_mutex)
      /* Beginning of the 'protected code' */
    {
	SERVER_LOCK(repair_info.ts_rep_mutex)
	repair_info.ts_rep_state = tsrep_Checkpt;
	if ((thstatus = dthread_broadcast(&(repair_info.ts_rep_state_changed))) < 0) {
	    dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, (long)thstatus);
	}
	SERVER_END_LOCK(repair_info.ts_rep_mutex)
    }	/* End of the 'protected code' */
    SERVER_END_LOCK(file_p->file_mutex)

    status = db_checkpoint(file_p);
    if (status != CDS_SUCCESS) {
	DCE_SVC_DEBUG((
		       cds__svc_handle,
		       cds_svc_server,
		       svc_c_debug1,
		       "  RepairBadClock: db_checkpoint returns (%d)",
		       status));
	return status;
    }

    /*
     *  Close current tlog file
     */
    (void)db_tlog_close(file_p);

    /* Save clearinghouse uid */
    COPY_ObjUID(&file_p->file_ch_p->ch_uid, &saved_chuid);

    /* Now release clearinghouse from memory. */
    file_info_free(file_p);

    /* Re-enable the clearinghouse */
    /*
     * Find clearinghouse
     */
    ch_p = mgmt_find_chuid (nsgbl_p, &saved_chuid);
    if (!ch_p) {
	DCE_SVC_DEBUG((
		       cds__svc_handle,
		       cds_svc_server,
		       svc_c_debug2,
		       "  srv_force_clh: return (CDS_UNKNOWNCLEARINGHOUSE)"));

	return (CDS_UNKNOWNCLEARINGHOUSE);
    }

    SERVER_LOCK(repair_info.ts_rep_mutex)
    repair_info.ts_rep_state = tsrep_Enable;
    if ((thstatus = dthread_broadcast(&(repair_info.ts_rep_state_changed))) < 0) {
	dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, (long)thstatus);
    }
    SERVER_END_LOCK(repair_info.ts_rep_mutex)

    if ((status = db_setup (ch_p)) == CDS_SUCCESS) {
	status = db_enable(ch_p, FALSE);
    }
    if (status != CDS_SUCCESS) {
	return status;
    }

    SERVER_LOCK(ch_p->ch_mutex)
    /* Beginning of the ch 'protected code' */
    {

	ch_p->ch_state = dns_On;

	if ((thstatus = dthread_broadcast(&(ch_p->ch_state_changed))) < 0) {
	    dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, (long)thstatus);
	}

    }	/* End of the ch 'protected code' */
    SERVER_END_LOCK(ch_p->ch_mutex)

    return status;
}

/*
 * CancelRepairState 
 *	This routine sets the clearinghouse state to file_loaded. 
 *
 * Input:
 *	file_p = pointer to open stream
 *
 * Value:
 *	CDS_SUCCESS
 */
PUBLIC int
CancelRepairState (file_info_t  *file_p)
{
    int          thstatus;
    file_state_t curr_state;

    /* 
     * Set the new state
     */
    SERVER_LOCK(file_p->file_mutex)
    /* Beginning of the 'protected code' */
    {
	if ((curr_state = file_p->file_state) == file_badclk_repair) {
	    file_p->file_state = file_loaded;
	    if ((thstatus = dthread_broadcast(&(file_p->file_state_changed))) < 0) {
		dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, (long)thstatus);
	    }
	}
    }	/* End of the 'protected code' */
    SERVER_END_LOCK(file_p->file_mutex)

    if (curr_state == file_badclk_repair) {
	DCE_SVC_DEBUG((
		       cds__svc_handle,
		       cds_svc_server,
		       svc_c_debug1,
		       "  CancelRepairState: repair canceled"));
    }
    else {
	DCE_SVC_DEBUG((
		       cds__svc_handle,
		       cds_svc_server,
		       svc_c_debug1,
		       "  CancelRepairState: Bad file state, state = %d",
		       curr_state));
    }

    return CDS_SUCCESS;
}

/*
 * RepairTimestamps_schedule
 *	This routine starts a thread to repair a CH of bad timestamps.
 *
 * Input:
 *	file_p = pointer to open stream
 *
 * Value:
 *	CDS_SUCCESS
 */
PUBLIC int
RepairTimestamps_schedule (file_info_t  *file_p)
{
    int               thstatus;

    /*
     * Start a thread to repair the bad timestamps if one isn't
     * already running.  Create associated mutex and condition variable.
     */
    if (dthread_equal(dthread_null, tsrep_thread)) {
	ZERO_bytes(&repair_info, sizeof(repair_info));
	if ((thstatus = dthread_create_mutex(&repair_info.ts_rep_mutex)) < 0) {
	  dce_svc_printf(CDS_S_PTHREADMUTEXINIT_A_MSG, (long)thstatus);
        }
	if ((thstatus = dthread_create_condition(&repair_info.ts_rep_state_changed)) < 0) {
	  dce_svc_printf(CDS_S_PTHREADCONDINIT_A_MSG, (long)thstatus);
        }
	thstatus = dthread_create (&tsrep_thread,
				   (dthread_attr_t *)NULL,
				   (dthread_start_routine)ts_repair_thread,
				   (dthread_address_t)file_p);
	if (thstatus < 0) {
	    dce_svc_printf(CDS_S_PTHREADCREATE_A_MSG, (long)thstatus);
	    return (dthread_get_errno());
	}
	else {
	    (void)dthread_detach(&tsrep_thread);
	}
    }
    /* Else, there's already a thread running, so return */

    return CDS_SUCCESS;
}

/*
 *
 *  ts_repair_thread
 *      This routine repairs the timestamps causing BadClock failures.
 *      It is called as a new thread.
 *
 *  INPUTS:
 *      arg             argument passed to us when our thread was created
 */

INTERNAL dthread_address_t 
ts_repair_thread (dthread_address_t arg)
{
    int status;
    int thstatus;
    file_info_t *file_p = (file_info_t *) arg;

    status = RepairBadClock(file_p);

    SERVER_LOCK(repair_info.ts_rep_mutex)
    repair_info.ts_rep_status = status;

    if (status == CDS_SUCCESS) {
	repair_info.ts_rep_state = tsrep_Done;
	if ((thstatus = dthread_broadcast(&(repair_info.ts_rep_state_changed))) < 0) {
	    dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, (long)thstatus);
	}
    }
    SERVER_END_LOCK(repair_info.ts_rep_mutex)

    return (dthread_address_t) status;
}

/*
 *
 *  GetRepairStatus
 *      This routine does a timed wait and returns the current repair state
 *      or an error if one occurred in the ts repair thread.
 *
 *  INPUTS:
 *      none.
 *  OUTPUTS:
 *      One of CDS_TSREP_* or error.
 */

PUBLIC int
GetRepairStatus ()
{
    ts_repair_state_t    rep_state;	/* current repair state */
    int                  rep_status;	/* current repair status */
    time_local_t         rep_time;
    dthread_date_time_t  timeout;
    int                  thstatus;
    int                  status;
    int                  errnum;

    /* Schedule wake up for ten seconds hence */
    sys_time_from_interval(&rep_time, 0, 0, 10);
    (void)dthread_get_expiration(&rep_time, &timeout);

    /* Get lock for timed wait */
    SERVER_LOCK(repair_info.ts_rep_mutex)

    if ((thstatus = dthread_timed_wait(&repair_info.ts_rep_state_changed,
				       &repair_info.ts_rep_mutex,
				       &timeout)) < 0) {
	if ((errnum = dthread_get_errno()) != EAGAIN) {
	    dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, (long)errnum);
	}
	/*
	 * Documentation for the pthread_cond_timedwait call does not
	 * explicitly state that the mutex is reacquired on error.
	 * I'm not seeing that is does, so reacquire the lock.
	 */
	SERVER_LOCK(repair_info.ts_rep_mutex)
    }

    rep_state = repair_info.ts_rep_state;
    rep_status = repair_info.ts_rep_status;

    SERVER_END_LOCK(repair_info.ts_rep_mutex)

    if (rep_status != 0 && rep_status != CDS_SUCCESS) {
	CleanupTSRepair();
	return rep_status;
    }
    switch (rep_state) {
      case tsrep_Off:
	status = CDS_TSREP_OFF;
	break;
      case tsrep_Disable:
	status = CDS_TSREP_DISABLE;
	break;
      case tsrep_Repair:
	status = CDS_TSREP_REPAIR;
	break;
      case tsrep_Checkpt:
	status = CDS_TSREP_CHECKPT;
	break;
      case tsrep_Enable:
	status = CDS_TSREP_ENABLE;
	break;
      case tsrep_Done:
	status = CDS_TSREP_DONE;
	CleanupTSRepair();
	break;
      default:
	status = CDS_WRONGSTATE;
	break;
    } /* end switch */

    return status;
}

/*
 *
 *  CleanupTSRepair
 *
 *      This routine cleans up the thread overhead for the ts repair thread.
 *
 */

INTERNAL void
CleanupTSRepair ()
{
    int  thstatus;

    if (!dthread_equal(tsrep_thread, dthread_null)) {
	dthread_assign(dthread_null, &tsrep_thread);
	if ((thstatus = dthread_delete_condition(&repair_info.ts_rep_state_changed)) < 0) {
	    dce_svc_printf(CDS_S_PTHREADCONDDESTROY_A_MSG, (long)thstatus);
	}

	if ((thstatus = dthread_delete_mutex(&repair_info.ts_rep_mutex)) < 0) {
	    dce_svc_printf(CDS_S_PTHREADMUTEXDESTROY_A_MSG, (long)thstatus);
	}
    }
}
